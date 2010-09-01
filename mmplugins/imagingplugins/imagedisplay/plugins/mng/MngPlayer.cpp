// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/** 	@file
	@internalTechnology */
#include <f32file.h>
#include <ecom/ecom.h>

#include "MngChunks.h"
#include "MngReadStream.h"
#include "MngProcessor.h"
#include "PngProcessor.h"
#include "MngRenderer.h"
#include "MngPanic.h"
#include "MngPlayerObserver.h"
#include "StreamDecoderIFace.h"
#include "StreamDecoderIFace.hrh"
#include "icl/imagedisplayplugin.h"

#include "logger.h"

#include "MngPlayer.h"

const CMngPlayer::TObserverNotifyEvent KDecodingStopError	=CImageDisplayPlugin::EStatusNoMoreToDecode
																| CImageDisplayPlugin::EStatusPaused;

const CMngPlayer::TObserverNotifyEvent KNotReadyToDecode	=CImageDisplayPlugin::EStatusBusy;

const CMngPlayer::TObserverNotifyEvent KFrameReady			=CImageDisplayPlugin::EStatusFrameReady;

const CMngPlayer::TObserverNotifyEvent KLastFrameReady		=CImageDisplayPlugin::EStatusFrameReady 
																| CImageDisplayPlugin::EStatusNoMoreToDecode;

const CMngPlayer::TObserverNotifyEvent KFinishedClearOutput	=CImageDisplayPlugin::EStatusEraseOutputContents
																| CImageDisplayPlugin::EStatusNoMoreToDecode;

const CMngPlayer::TObserverNotifyEvent KFinishedKeepOutput	=CImageDisplayPlugin::EStatusDisplayThisFrameIndefinetely
																| CImageDisplayPlugin::EStatusNoMoreToDecode;


const CMngPlayer::TObserverNotifyEvent KFinishedEraseOutputFlag=CImageDisplayPlugin::EStatusEraseOutputContents;

const CMngPlayer::TObserverNotifyEvent KFinishedKeepOutputFlag=CImageDisplayPlugin::EStatusDisplayThisFrameIndefinetely;

const CMngPlayer::TObserverNotifyEvent KInitComplete=0;

/*static*/ 
CMngPlayer*	CMngPlayer::NewL(CMngFileReadStream* aReadStream, MMngPlayerObserver& aObserver, TBool aMngSubframesNoLoops)   //Subframes with No Loops
	{
	CMngPlayer* self=new (ELeave) CMngPlayer(aObserver, aMngSubframesNoLoops);   //Subframes with No Loops
	CleanupStack::PushL(self);
	self->ConstructL(aReadStream);

	CleanupStack::Pop(self);
	return self;
	}

CMngPlayer::CMngPlayer(MMngPlayerObserver& aObserver, TBool aMngSubframesNoLoops):CActive(EPriorityIdle), //Subframes with No Loops
														iMngSubframesNoLoops(aMngSubframesNoLoops),   //Subframes with No Loops
														iObserver(aObserver),
														iPerformFrameDelayInternally(ETrue),
														i1stFrameInterframeDelay(-1),
														iFrameNumberToTerminate(-1),
														iNeedsChange(ETrue)
	{
	CActiveScheduler::Add(this);
	}

void CMngPlayer::ConstructL(CMngFileReadStream* aReadStream)
	{
	TUid DtorKey;
	iDecoderFactory = reinterpret_cast<MImageStreamDecoderFactory*>
							(REComSession::CreateImplementationL(TUid::Uid(KPngStreamDecoderFactoryImplUidValue), DtorKey));
	iDecoderFactory->SetDtorKey(DtorKey);
	
	iExecTimer		= CDelayTimer::NewL(*this);

	iMngStream		= aReadStream;
	iMngStream->AddRef();
	iMngProcessor	= CMngProcessor::NewL(*this, iMngSubframesNoLoops); //Subframes with No Loops
	iPngProcessor	= CPngProcessor::NewL(*this);

	iMngProcessor->AddSubProcessor(*iPngProcessor);
	}

CMngPlayer::~CMngPlayer()
	{
	Cancel();
	delete iExecTimer;
	if (NULL != iMngProcessor)
		{
		iMngProcessor->Stop();
		}
	delete iMngProcessor;
	delete iPngProcessor;
	if (NULL != iMngStream)
		{
		iMngStream->Release();
		iMngStream = NULL;
		}
	delete iMngRenderer;
	iObjectList.Destroy();
	iLoopStack.Close();
	iChunkProviders.Close();
	delete iMngHeader;
	delete iTerminationCtrl;
	if (NULL != iDecoderFactory)
		{
		iDecoderFactory->Release();
		}
	}

void CMngPlayer::InitL(TMhdrChunk*& aHeaderChunk)
	{
	SetMngHeader(aHeaderChunk);

	if (iGotHeaderFromStream)
		{
		User::Leave(KErrCorrupt);
		}
		
	iGotHeaderFromStream = ETrue;

	if (iMngHeader->iProfileValidity && (iMngHeader->iJngPresence
#ifdef STRICT_MNG_LC
		|| iMngHeader->iDeltaPngPresence ==1
#endif // STRICT_MNG_LC
		))
		{
		User::Leave(KErrNotSupported);
		}
	
	iMngRenderer = CMngRenderer::NewL(TSize(iMngHeader->iFrameWidth, iMngHeader->iFrameHeight), 
										IsAlphaNeeded(), iDestDisplayMode, iMaskDisplayMode );

	NotifyObserver(KInitComplete, KErrNone, TTimeIntervalMicroSeconds32(0));
	}

void CMngPlayer::Decode()
	{
	if (iExecTimer->IsActive())
		{
		return; // we'are just awating for delay to elapse
		}
	if (iOutstandingNotify)
		{
		iOutstandingNotify = EFalse;
		iExecTimer->After(TTimeIntervalMicroSeconds32(0));
		iIsPaused = EFalse;
		return;
		}
	
	switch (iState)
		{
		case EStJustBorn:
			iMngProcessor->Start(*iMngStream, iStatus);
			iMngStream->Release();
			iMngStream	= NULL;
			iState		= EStReadingFormat;
			SetActive();
			break;
		case EStReadingFormat:
			iMngProcessor->Resume();
			break;
		default: 
			if (iIsPaused)
				{
				if (!IsActive() && !iExecTimer->IsActive())
					{
					RunAgain();
					}
				}
			else
				{
				if (iState != EStPlaying)
					{
					StartPlay();
					}
				}
		}
	iIsPaused = EFalse;
	}

void CMngPlayer::StopDecode()
	{
	iOutstandingNotify = EFalse;
	if (iState == EStReadingFormat)
		{
		iMngProcessor->Pause();
		return;
		}
	Cancel();
	if (NULL != iExecTimer)
		{
		iExecTimer->Cancel();
		}
	iIsPaused				= EFalse;
	iFrameNumberToTerminate	=-1;
	i1stFrameInterframeDelay=-1;

	if (EStPlaying == iState)
		{
		iState = EStReadyToPlay;
		}
		
	if (iTerminationCtrl)
		{
		iTerminationCtrl->Reset();
		}
	}

void CMngPlayer::Pause()
	{
	if (iIsPaused)
		{
		return;
		}
	switch (iState)
		{
		case EStJustBorn:
			break;
		case EStReadingFormat:
			iMngProcessor->Pause();
			break;
		default:
			(void)0;
		}
	iIsPaused = ETrue;
	}

void CMngPlayer::RunL()
	{
	ASSERT(!iExecTimer->IsActive());
	if (KErrNone != iStatus.Int())
		{
		iPlayerErrorCode = iStatus.Int();
		StopDecode();
		iState = EStStopped;
		NotifyObserver(KDecodingStopError, iPlayerErrorCode, TTimeIntervalMicroSeconds32(0));
		return;
		}

	switch (iState)
		{
		case EStJustBorn:
			ASSERT(EFalse);
			break;

		case EStReadingFormat:
			iState = TPlayerState(iState + 1);
			delete iMngProcessor;
			iMngProcessor = NULL;
			if (iMngHeader == NULL)
				{
				User::Leave(KErrCorrupt);
				}
			StartPlay();
			break;

		case EStReadyToPlay:
			ASSERT(EFalse);
			break;

		case EStPlaying:
			OnIteration();
			break;
		
		case EStStopped:
			ASSERT(EFalse);
			break;

		case EStNeedsRepeat:
			ASSERT(EFalse);
			break;
		}
	}

void CMngPlayer::DoCancel()
	{
	TBool timerActive = EFalse;
	if (NULL != iExecTimer)
		{
		timerActive = iExecTimer->IsActive();
		iExecTimer->Cancel();
		}
		
	if (NULL != iMngProcessor)
		{
		iMngProcessor->Stop();
		}
	if (!timerActive)
		{
		iStatus = KErrCancel;
		}
	}

TInt CMngPlayer::RunError(TInt aError)
	{
	iPlayerErrorCode= aError;
	iState			= EStStopped;
	NotifyObserver(KDecodingStopError, iPlayerErrorCode, TTimeIntervalMicroSeconds32(0));
	return KErrNone;
	}

void CMngPlayer::RunAgain()
	{
	SetActive();
	TRequestStatus* pReq=&iStatus;
	User::RequestComplete(pReq, KErrNone);
	}

void CMngPlayer::StartPlay()
	{
	if (iState != EStReadyToPlay && iState != EStNeedsRepeat)
		{
		NotifyObserver(KNotReadyToDecode, KErrNotReady, TTimeIntervalMicroSeconds32(0));
		return;
		}
	iNominalDelayBeforeThisFrame = 0;
	iEndOfFrameNotification = EFalse;
	iInterframeDelay		= 0;
	iDefaultInterFrmDelay	= 0;
	iNextInterFrameDelay	= 0;
	if (0 != iMngHeader->iTicksPerSecond)
		{
		iTickLength = 1000000 / iMngHeader->iTicksPerSecond;
		if (0 >= iTickLength)
			{
			iTickLength = 1;
			}
		iInterframeDelay		= 1;
		iDefaultInterFrmDelay	= 1;
		iNextInterFrameDelay	= 1;
		}

	iCurrentObject = iObjectList.Begin();
	
	iFramingMode=EMngFramNoBgLayerExcpt1stFgLayer;
	
	iSubframeLayerNum	= 0;
	iDrawnLayerNum		= 0;
	iDrawnFrameNum		= 0;

	iDefaultClippingBnd.iTl.iX = 0;
	iDefaultClippingBnd.iTl.iY = 0;
	iDefaultClippingBnd.iBr.iX = iMngHeader->iFrameWidth;
	iDefaultClippingBnd.iBr.iY = iMngHeader->iFrameHeight;
	iFrameBeingRendered	= EFalse;

	iChunkProviders.Clear();
	iMngRenderer->Reset();
	for (RMngObjectList::TObjectIterator it=iObjectList.Begin(); it != iObjectList.End(); ++it)
		{
		iObjectList[it]->Reset();
		}

	iState = EStPlaying;
	RunAgain();
	}

void CMngPlayer::SetTerminationCtrlL(CMngTermination* aTermination)
	{
	if (NULL != iTerminationCtrl)
		{
		delete aTermination;
		User::Leave(KErrCorrupt);
		}
	iTerminationCtrl = aTermination;
	}

void CMngPlayer::AppendObjectL(CMngObject*& aObject)
	{
	aObject->SetObjectLabel(iObjectList.Count()+1);	
	TInt err=iObjectList.Append(aObject);
	CMngObject* temp=aObject;
	aObject = NULL;
	if (KErrNone != err)
		{
		delete temp;
		User::Leave(err);
		}
	temp->OnAddToPlayerL(*this);
	}
 
void CMngPlayer::OnEndOfSubFrame()
	{
	if (iChangeInterFrmDelay == EMngFramChangeUpcomingSubFrameOnly)
		{
		if(iObjectList[iCurrentObject+1]->IsSubFrame())
			{
			iNeedsChange = EFalse;
			iInterframeDelay = iNextInterFrameDelay;
			}
		iNextInterFrameDelay = iDefaultInterFrmDelay;
		}
	iChangeInterFrmDelay = EMngFramDoNotChange;

	if (iChangeClippingBoundaries == EMngFramChangeUpcomingSubFrameOnly)
		{
		iCurrentClippingBnd = iDefaultClippingBnd;
		iMngRenderer->SetBgClipRect(iCurrentClippingBnd);
		}

	if (iChangeClippingBoundaries == EMngFramChangeAndResetDefault)
		{
		iDefaultClippingBnd = iCurrentClippingBnd;
		iMngRenderer->SetBgClipRect(iCurrentClippingBnd);
		iMngRenderer->ClearOutputRect();
		}

	iChangeClippingBoundaries = EMngFramDoNotChange;

	if (!iFrameBeingRendered)
		{
		iInterframeDelay = iNextInterFrameDelay;
		}

	iSubframeLayerNum = 0;
	}

void CMngPlayer::OnNewLayer()
	{
	if (iFramingMode==EMngFramBgLayerForEachFgLayer || iDrawnLayerNum==0 ||
		 (iFramingMode==EMngFramBgLayerFor1stFgLayer && iSubframeLayerNum == 0))
		{
		iMngRenderer->ClearOutputRect();
		}
	++iDrawnLayerNum;
	++iSubframeLayerNum;
	iFrameBeingRendered = ETrue;
	}

void CMngPlayer::OnEndOfFrame(TBool aAlwaysNotify, TBool aIsLastFrame)
	{
	const TInt KTimerThreshold =1000;
	++iDrawnFrameNum;
	TUint NotifyCode=(aIsLastFrame? KLastFrameReady : KFrameReady);
	
	if (iFrameNumberToTerminate >= 0 && iFrameNumberToTerminate==iDrawnFrameNum)
		{
		NotifyCode = (KLastFrameReady|KFinishedKeepOutputFlag);
		}

	const TInt TicksDelay=(iDrawnFrameNum==1 && i1stFrameInterframeDelay>-1 ? i1stFrameInterframeDelay : iInterframeDelay);

	if ( (NotifyCode & KLastFrameReady)==KLastFrameReady)
		{
		StopDecode();
		}

	TInt delayValue=Ticks2TimeInterval(TicksDelay).Int();
	i1stFrameInterframeDelay =-1;

	if (iPerformFrameDelayInternally)
		{
		TTime CurrentTime;
		CurrentTime.HomeTime();
		
		const TInt KDelayNeeded	=delayValue;
		const TInt KMsDifference=I64HIGH(CurrentTime.MicroSecondsFrom(iEndOfPreviuosFrame).Int64()) ? KDelayNeeded +1 :
									I64LOW( CurrentTime.MicroSecondsFrom(iEndOfPreviuosFrame).Int64() );

		delayValue=KDelayNeeded - KMsDifference > KTimerThreshold ? KDelayNeeded - KMsDifference - KTimerThreshold : 0;
		LOG4("Needed %u Got %u Val %u", KDelayNeeded, KMsDifference, delayValue);
		}

	iEndOfFrameNotification = ETrue;
	if (iInterframeDelay != 0 || aAlwaysNotify )  // as per spec do not send zero-delay updates
		{
		NotifyObserver(NotifyCode, KErrNone, TTimeIntervalMicroSeconds32(delayValue));
		}
	iEndOfPreviuosFrame.HomeTime();
	
	iFrameBeingRendered	= EFalse;
	if(iNeedsChange)
		{
		iInterframeDelay = iNextInterFrameDelay;
		iNeedsChange = ETrue;
		}
	}

inline
void CMngPlayer::OnIteration()
	{
	if (iIsPaused)
		{
		return;
		}
	TBool CallAgain=EFalse;
	do 
		{
		TRAP(iPlayerErrorCode, CallAgain=DoIterationL());	
		} while (iPlayerErrorCode==KErrNone && CallAgain && !iIsPaused);
	
	if (KErrNone != iPlayerErrorCode)
		{
		StopDecode();
		iState=EStStopped;
		NotifyObserver(KDecodingStopError, iPlayerErrorCode, TTimeIntervalMicroSeconds32(0));
		}
	}

TBool CMngPlayer::DoIterationL()
	{
	CMngObject* pObject=NULL;

	if (iCurrentObject==iObjectList.End() )
		{
		if (!iLoopStack.IsEmpty())
			{
			User::Leave(KErrCorrupt);
			}
		pObject = iTerminationCtrl;
		}
	else
		{
		pObject = iObjectList[iCurrentObject];
		}

	TMngExecuteResult res;

	if (NULL != pObject)
		{
		if (pObject->IsSubFrame())
			{
			pObject->ExecuteL(*this, res);
			}
		if (pObject->IsLayer() || (pObject->IsSubFrame() && 
					(iFramingMode==EMngFramBgLayerForEachFgLayer || iFramingMode==EMngFramBgLayerFor1stFgLayer) ))
			{
			OnNewLayer();
			}
		if (!pObject->IsSubFrame())
			{
			pObject->ExecuteL(*this, res);
			}
		}
	else
		{
		ASSERT(iCurrentObject==iObjectList.End());
		res.iCommand = ECmdTerminate;
		}
		
	CMngObject* pNextObject = NULL;
	TBool MoreToRun=EFalse;
	switch (res.iCommand)
		{
		case ECmdJumpToObject:
			{
			RMngObjectList::TObjectIterator it=iObjectList.Find(res.iObjectLabel);
			ASSERT( it != iObjectList.End() );
			iCurrentObject = it;
			RunAgain();
			}
			break;
		case ECmdProcessNext:
			{
			if (res.iCommand==ECmdProcessNext)
				{
				if ((iCurrentObject+1) != iObjectList.End())
					{
					pNextObject = iObjectList[iCurrentObject+1];
					}
				else
					{
					pNextObject = iTerminationCtrl;
					}
				}
				
			if (NULL != pNextObject)
				{
				const TInt SubFrameLayerNum=iSubframeLayerNum;
				if (pNextObject->IsSubFrame() || 
						(iFramingMode==EMngFramNoBgLayerExcpt1stFgLayer && pObject->IsLayer()))
					{
					if(pNextObject->IsSubFrame())
						{
						pNextObject->ExecuteL(*this, res);
						}
					
					OnEndOfSubFrame();
					}

				switch (iFramingMode)
					{
					case EMngFramNoBgLayerExcpt1stFgLayer:
						if (iDrawnLayerNum && iFrameBeingRendered && 
									(pNextObject->IsLayer() || pNextObject->IsSubFrame())
							)
							{
							OnEndOfFrame();
							}
						break;
					case EMngFramNoBgLayerExcpt1stImage:
						if (iFrameBeingRendered && pNextObject->IsSubFrame())
							{
							OnEndOfFrame();
							}
						break;
					case EMngFramBgLayerForEachFgLayer:
						if (iFrameBeingRendered && (pNextObject->IsSubFrame()|| 
								(SubFrameLayerNum>1 && pNextObject->IsLayer() )))
							{
							OnEndOfFrame();
							}
						break;
					case EMngFramBgLayerFor1stFgLayer:
						if (pNextObject->IsSubFrame() && iDrawnLayerNum)
							{
							OnEndOfFrame();
							}
						break;
					default:
						ASSERT(EFalse);
					}
				}
			else
				{
				OnEndOfFrame(ETrue, ETrue);
				}

			++iCurrentObject;
			if (! iExecTimer->IsActive())
				{
				MoreToRun = ( pNextObject!=NULL && !pNextObject->IsLayer() );
				if (!MoreToRun)
					{
					RunAgain();
					}
				}
			}
			break;
		case ECmdTerminate:
			if (iFrameBeingRendered)
				{
				OnEndOfFrame(ETrue, ETrue);
				iNotifyEvent |=KFinishedKeepOutputFlag;
				}
			else
				{
				StopDecode();
				if(iMngSubframesNoLoops)
					{
					NotifyObserver(KFinishedKeepOutput, KErrNone, Ticks2TimeInterval(res.iDelayInTicks));  //Subframes with No Loops
					}
				else
					{
					NotifyObserver(KFinishedKeepOutput, KErrNone, TTimeIntervalMicroSeconds32(0));
					}
				}

			break;
		case ECmdTerminateClearOutput:
			if (iFrameBeingRendered)
				{
				OnEndOfFrame(ETrue, ETrue);
				iNotifyEvent |= KFinishedEraseOutputFlag;
				}
			else
				{
				StopDecode();
				NotifyObserver(KFinishedClearOutput, KErrNone, TTimeIntervalMicroSeconds32(0));
				}
			break;
		case ECmdTerminateAfter1stFrame:
		case ECmdStartFromBeginning:
			{
			if (iFrameBeingRendered)
				{
				iState = EStNeedsRepeat;
				OnEndOfFrame(ETrue);
				i1stFrameInterframeDelay = iInterframeDelay + res.iDelayInTicks;
				}
			else
				{
				iState = EStReadyToPlay;
				i1stFrameInterframeDelay = iInterframeDelay + res.iDelayInTicks;
				StartPlay(); 
				}
			}
			break;
		default:
			ASSERT(EFalse);
		}
	return MoreToRun;
	}


CDelayTimer* CDelayTimer::NewL(CMngPlayer& aPlayer)
	{
	CDelayTimer* self=new (ELeave) CDelayTimer(aPlayer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CDelayTimer::RunL()
	{
	iPlayer.NotifyObserverGate();
	}

void CDelayTimer::RunNow()
	{
	SetActive();
	iStatus = KRequestPending;
	TRequestStatus* p=&iStatus;
	User::RequestComplete(p, KErrNone);
	}

void CMngPlayer::NotifyObserver(TObserverNotifyEvent aEvent, TInt aErrorCode, TTimeIntervalMicroSeconds32 aNotifyDelay)
	{
	if (iExecTimer->IsActive() || iOutstandingNotify)
		{
		MngPanic(EPanicObserverIsBusy);
		}
	ASSERT(iIsPaused==EFalse);
	iNominalDelayBeforeThisFrame = aNotifyDelay;
	iNotifyEvent		= aEvent;
	iObserverNotifyArg	= aErrorCode;
#ifdef _DEBUG
	iNotifyStart.HomeTime();
#endif
	if (iPerformFrameDelayInternally && aNotifyDelay.Int() > 10000)
		{
		iExecTimer->After(aNotifyDelay);
		}
	else
		{
		iExecTimer->RunNow();
		}
	}

void CMngPlayer::NotifyObserverGate()
	{
#ifdef _DEBUG
	TTime Now;
	Now.HomeTime();
	LOG3("Notified after %d Net %d", I64LOW(Now.MicroSecondsFrom(iNotifyStart).Int64()), I64LOW(Now.MicroSecondsFrom(iLastNotify).Int64()) );
	iLastNotify = Now;
#endif
	if (iIsPaused)
		{
		iOutstandingNotify = ETrue;
		return;
		}
	iObserver.OnDecodeEvent(iNotifyEvent, iObserverNotifyArg);
	if (iEndOfFrameNotification)
		{
		iEndOfPreviuosFrame.HomeTime();
		}
	iEndOfFrameNotification = EFalse;

	if (iState == EStPlaying)
		{
		RunAgain();
		}
	else if (iState == EStNeedsRepeat)
		{
		StartPlay();
		}
	}

void CMngPlayer::SetLayerClipping(TMngFramChangeNextFrameOption aOption, const TRect& aClipRect, TBool aIsAdditive)
	{
	iChangeClippingBoundaries = aOption;
	if (aIsAdditive)
		{
		iCurrentClippingBnd.iTl.iX += aClipRect.iTl.iX;
		iCurrentClippingBnd.iTl.iY += aClipRect.iTl.iY;
		iCurrentClippingBnd.iBr.iX += aClipRect.iBr.iX;
		iCurrentClippingBnd.iBr.iY += aClipRect.iBr.iY;
		}
	else
		{
		iCurrentClippingBnd = aClipRect;
		}
	if (EMngFramChangeAndResetDefault == aOption)
		{
		iDefaultClippingBnd = iCurrentClippingBnd;
		}
	iMngRenderer->SetBgClipRect(iCurrentClippingBnd);
	}

void CMngPlayer::SetInterFrameDelay(TMngFramChangeNextFrameOption aOption, TInt aTicks)
	{
	iChangeInterFrmDelay= aOption;
	if (aOption != EMngFramDoNotChange)
		{
		iNextInterFrameDelay	= aTicks;
		}
	if (EMngFramChangeAndResetDefault == aOption)
		{
		iDefaultInterFrmDelay	= aTicks;
		}
	}

void CMngPlayer::SetMngHeader(TMhdrChunk*& aHeaderChunk)
	{
	if (aHeaderChunk)
		{
		delete iMngHeader;
		iMngHeader	= aHeaderChunk;
		aHeaderChunk= NULL;
		}
	}

void CMngPlayer::SetPixelFilter(MPixelFilter* aPixelFilter)
	{
	iMngRenderer->SetRgbFilter(aPixelFilter);
	}

MImageStreamDecoderFactory& CMngPlayer::DecoderFactory()
	{
	return *iDecoderFactory;
	}

void CMngPlayer::AddUniqueChunkProviderL(const TChunkId& aChunkId,  MUniqueChunkDataProvider& aProvider)
	{
	TInt Size;
	const TUint8* Dummy;
	aProvider.GetChunkData(aChunkId.iChunkIdChr, Dummy, Size);
	if (Size > 0)
		{
		User::LeaveIfError( iChunkProviders.Add(TProviderEntry(aChunkId, aProvider)) );
		}
	else
		{
		iChunkProviders.RemoveProvider(aChunkId);
		}
	}
// from the MUniqueChunkDataProvider //
TInt CMngPlayer::GetChunkData(const TText8* aChunkId, const TUint8*& aDataPtr, TInt& aSizeInBites)
	{
	aDataPtr	= NULL;
	aSizeInBites= 0;
	MUniqueChunkDataProvider* Provider = iChunkProviders.Find(TChunkId::FromPtr(aChunkId));
	return (Provider == NULL ? KErrNotFound : Provider->GetChunkData(aChunkId, aDataPtr, aSizeInBites) );
	}


RMngObjectList::TObjectIterator RMngObjectList::Find(TMngObjLabel aLabel)
	{
	for (TInt i=Count()-1; i > -1; --i)
		{
		if (aLabel == (*this)[i]->ObjectLabel() )
			{
			return TObjectIterator(i);
			}
		}
	return End();
	}

void RMngObjectList::Destroy()
	{
	for (TInt i=Count()-1; i>-1; --i)
		{
		delete (*this)[i];
		}
	Close();
	}

/*static*/
TInt RChunkProviders::OrderFunc(const TProviderEntry& aLeft, const TProviderEntry& aRight)
	{
	return aLeft.iId.iChunkIdInt - aRight.iId.iChunkIdInt;
	}

TInt RChunkProviders::Add(const TProviderEntry& aItem)
	{
	TInt res=KErrNone;
	TInt idx = FindInOrder(aItem, TMyOrder(OrderFunc) );
	if (idx>=0)
		{
		(*this)[idx] = aItem;
		}
	else
		{
		res=InsertInOrder(aItem, TMyOrder(RChunkProviders::OrderFunc) );
		}
	return res;
	}

void RChunkProviders::RemoveProvider(const TChunkId& aId)
	{
	TProviderEntry item;
	item.iId = aId;
	TInt idx = FindInOrder(item, TMyOrder(OrderFunc) );
	ASSERT(idx >= 0);
	Remove(idx);
	}

MUniqueChunkDataProvider* RChunkProviders::Find(const TChunkId& aId)
	{
	TProviderEntry item;
	item.iId = aId;
	TInt idx = FindInOrder(item, TMyOrder(OrderFunc) );
	return (idx >= 0 ?  (*this)[idx].iProvider : NULL);
	}

void RChunkProviders::Clear()
	{
	Reset();
	}
