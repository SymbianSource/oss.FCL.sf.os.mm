// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <imageconversion.h>
#include <icl/imagedisplayplugin.h>
#include "MiscUtils.h"

#include "ImagePlayer.h"

/** 	@file
	@internalComponent */

const TUint KDecodeError	= CImageDisplayPlugin::EStatusNoMoreToDecode;
const TUint KPartialFrameReady= CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusPartialFrame;
const TUint KFrameReady		= CImageDisplayPlugin::EStatusFrameReady;
const TUint KLastFrame		= CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusNoMoreToDecode;


CImagePlayer::CImagePlayer(MImagePlayerObserver& aObserver):
			CActive(EPriorityIdle),
			iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

CImagePlayer::~CImagePlayer()
	{
	Cancel();
	delete iFrame;
	delete iMask;
	delete iTmpFrame;
	delete iTmpMask;
	}

void CImagePlayer::Play(CImageDecoder& aDecoder, TDisplayMode aDisplayMode, TInt aFrameLimit)
	{
	iDecoder			= &aDecoder;
	iFrameNumber		= 0;
	iCurrentReduction	= iMaxReduction;
	iDisplayMode		= aDisplayMode;
	iNeedRunAfterResume = ETrue;
	iCurrentFrameInfo	= &iDecoder->FrameInfo();
	if (aFrameLimit==KNoFrameLimit)
		{
		iFrameLimitToDecode = iDecoder->FrameCount();
		}
	else
		{
		ASSERT(aFrameLimit > 0 && aFrameLimit <= iDecoder->FrameCount());
		iFrameLimitToDecode = aFrameLimit;
		}
	Resume();
	}

void CImagePlayer::Setup(const TSize& aDestSize,TInt aMaxReduction, TInt aMinReduction)
	{
	iDestSize		 = aDestSize;
	iMaxReduction	 = aMaxReduction;
	iMinReduction	 = aMinReduction;
	}

TInt CImagePlayer::RunError(TInt aError)
	{
	iObserver.OnPlayEvent(aError,KDecodeError,NULL, NULL);
	return KErrNone;
	}

void CImagePlayer::Pause()
	{
	iIsPaused = ETrue;
	}

void CImagePlayer::Resume()
	{
	iIsPaused = EFalse;
	if (iNeedRunAfterResume)
		{
		iNeedRunAfterResume = EFalse;
		RunAgain();
		}
	}

void CImagePlayer::RunAgain()
	{
	SetActive();
	TRequestStatus* pR=&iStatus;
	User::RequestComplete(pR,KErrNone);
	}

/*static*/
void CImagePlayer::CreateBitmapsL(CFbsBitmap*& aFrame, CFbsBitmap** aMask, const TSize& aSize, TDisplayMode aFrameMode)
	{
	if (aFrame==NULL || aFrame->DisplayMode()!=aFrameMode)
		{
		delete aFrame;
		aFrame = NULL;
		aFrame = new (ELeave) CFbsBitmap();
		User::LeaveIfError( aFrame->Create(aSize, aFrameMode) );
		}
	else if (aFrame->SizeInPixels()!=aSize)
		{
		User::LeaveIfError( aFrame->Resize(aSize) );
		}

	if ( aMask != NULL )
		{
		if (*aMask==NULL)
			{
			*aMask = new (ELeave) CFbsBitmap();
			User::LeaveIfError( (*aMask)->Create(aSize, EGray256) );
			}
		else if ((*aMask)->SizeInPixels()!=aSize)
			{
			User::LeaveIfError( (*aMask)->Resize(aSize) );
			}
		}
	}

void CImagePlayer::CreateBitmapsL()
	{
	TSize thisFrameSz(ScaleSize(iDestSize, iDecoder->FrameInfo().iFrameCoordsInPixels.Size(), 
						iCurrentFrameInfo->iFrameCoordsInPixels.Size() )
					);
	
	if (iCurrentReduction != 0)
		{
		User::LeaveIfError( iDecoder->ReducedSize(iCurrentFrameInfo->iFrameCoordsInPixels.Size(), iCurrentReduction, thisFrameSz) );
		}

	iUseTempBitmap = EFalse;
	const TBool KNeedMask= (iCurrentFrameInfo->iFlags&TFrameInfo::ETransparencyPossible) != 0;

	if (iCurrentFrameInfo->iFrameCoordsInPixels.iTl.iX != 0 || iCurrentFrameInfo->iFrameCoordsInPixels.iTl.iY != 0
		|| (iCurrentFrameInfo->iFlags&TFrameInfo::ELeaveInPlace) != 0 )
		{
		iUseTempBitmap = ETrue;
		CreateBitmapsL(iTmpFrame, (KNeedMask? &iTmpMask : NULL), thisFrameSz, iDisplayMode );
		}
	iIsAnimationSubframe = ETrue;
	// create/change bitmaps only in case if it's 1st frame and we do not need to compose frames 
	// and we'are not dealing with some kind of strange image (like multiple MBMs)
	if (iFrameNumber==0 || iCurrentReduction!=iMinReduction 
			|| (!iUseTempBitmap 
					&& iCurrentFrameInfo->iFrameCoordsInPixels!=iDecoder->FrameInfo().iFrameCoordsInPixels
					&& (iCurrentFrameInfo->iFlags&(TFrameInfo::ELeaveInPlace|TFrameInfo::ERestoreToBackground|TFrameInfo::ERestoreToPrevious))==0
				)
			)
		{
		CreateBitmapsL(iFrame, (KNeedMask? &iMask : NULL), thisFrameSz, iDisplayMode );
		iIsAnimationSubframe = EFalse;
		}
	}

void CImagePlayer::MergeFrameL()
	{
	ASSERT((iCurrentFrameInfo->iFlags&TFrameInfo::ELeaveInPlace) != 0 || (iCurrentFrameInfo->iFlags&TFrameInfo::ERestoreToBackground) != 0 );

	const TBool hasMask=((iCurrentFrameInfo->iFlags&TFrameInfo::ETransparencyPossible) != 0);
	CFbsBitmapDevice* device=CFbsBitmapDevice::NewL(iFrame);
	CleanupStack::PushL(device);
	CFbsBitGc* gc=CFbsBitGc::NewL();
	CleanupStack::PushL(gc);
	gc->Activate(device);

	const TPoint dest(ScaleSize(iFrame->SizeInPixels(),  iDecoder->FrameInfo().iFrameCoordsInPixels.Size(),
									iCurrentFrameInfo->iFrameCoordsInPixels.iTl.AsSize() ).AsPoint());

	if ((iCurrentFrameInfo->iFlags&TFrameInfo::ERestoreToBackground) != 0 )
		{
		gc->SetBrushColor(iCurrentFrameInfo->iBackgroundColor);
		gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		gc->Clear();
		}
	
	if (hasMask)
		{
		gc->BitBltMasked(dest, iTmpFrame, TRect(TPoint(0,0),iTmpFrame->SizeInPixels().AsPoint()), iTmpMask, EFalse);
		}
	else
		{
		gc->BitBlt(dest, iTmpFrame);
		}
	
	CleanupStack::PopAndDestroy(2,device);
	if (!hasMask || (iCurrentFrameInfo->iFlags&TFrameInfo::ERestoreToBackground) == 0 )
		{
		return;
		}
	device=CFbsBitmapDevice::NewL(iMask);
	CleanupStack::PushL(device);
	gc=CFbsBitGc::NewL();
	CleanupStack::PushL(gc);
	gc->Activate(device);
	gc->SetBrushColor(KRgbBlack);
	gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc->Clear();
	gc->BitBlt(iCurrentFrameInfo->iFrameCoordsInPixels.iTl, iTmpMask);
	CleanupStack::PopAndDestroy(2,device);
	}

void CImagePlayer::RunL()
	{
	if (iIsPaused)
		{
		iNeedRunAfterResume = ETrue;
		return;
		}
	switch (iState)
		{
		case EIdle:
			{
			iCurrentFrameInfo = &iDecoder->FrameInfo(iFrameNumber);
			CreateBitmapsL();

			const TBool useMask=((iCurrentFrameInfo->iFlags & TFrameInfo::ETransparencyPossible) != 0);
			if ( useMask )
				{
				iDecoder->Convert(&iStatus, *FrameBitmap(), *MaskBitmap(), iFrameNumber);
				}
			else
				{
				iDecoder->Convert(&iStatus, *FrameBitmap(), iFrameNumber);
				}
			
			iState = EDecoding;
			SetActive();
			}
			break;
		case EDecoding:
			{
			if (iUseTempBitmap)
				{
				MergeFrameL();
				}
			TUint status=KPartialFrameReady;		// first we iterate reduction factors
			if (iCurrentReduction == iMinReduction)
				{
				status = KFrameReady;				// than "true" image frames
				if (++iFrameNumber == iFrameLimitToDecode)
					{
					status		= KLastFrame;
					iFrameNumber= 0;
					}
				iCurrentReduction = iMaxReduction;
				}
			else
				{
				--iCurrentReduction;
				}
			iObserver.OnPlayEvent(iStatus.Int(), status, iFrame, iMask);
			iNeedRunAfterResume = ETrue;
			iState	= EIdle;
			}
			break;
		default:
			ASSERT(EFalse);	// shan't reach it.
		}
	}

void CImagePlayer::DoCancel()
	{
	iDecoder->Cancel();
	iNeedRunAfterResume = EFalse;
	iState 				= EIdle;
	}

TSize CImagePlayer::OriginalFrameSize() const
	{
	return iUseTempBitmap || iIsAnimationSubframe ? 
				iDecoder->FrameInfo(0).iFrameCoordsInPixels.Size() :
				iDecoder->FrameInfo(iFrameNumber).iFrameCoordsInPixels.Size();
	}

