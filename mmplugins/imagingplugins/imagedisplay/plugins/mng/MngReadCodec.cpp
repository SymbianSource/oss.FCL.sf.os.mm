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

/**
 @file
 @internalTechnology 
*/

#include <icl/imagecodec.h>
#include <icl/imagedata.h>
#include <icl/imagedisplayplugin.h>
#include "MngPlayer.h"
#include "MngChunks.h"
#include "MngRenderer.h"

#include "MngReadCodec.h"

/*static*/
CMngReadCodec* CMngReadCodec::NewL(CMngFileReadStream& aMngReadStream, MMngCodecObserver& aObserver, TBool aMngSubframesNoLoops)  //Subframes with No Loops
	{
	CMngReadCodec* self = new (ELeave) CMngReadCodec(aObserver, aMngSubframesNoLoops);
	CleanupStack::PushL(self);
	self->ConstructL(aMngReadStream);
	CleanupStack::Pop(self); 
	return self;
	}

CMngReadCodec::~CMngReadCodec()
	{
	delete iMngPlayer;
	if (iMngReadStream)
		{
		iMngReadStream->Release();
		iMngReadStream = NULL;
		}
	}

void CMngReadCodec::ConstructL(CMngFileReadStream& aMngReadStream)
	{
	CImageMaskProcessorReadCodec::ConstructL();
	
	iMngReadStream = &aMngReadStream;
	iMngReadStream->AddRef();

	CreatePlayerL();
	Restart();
	}

void CMngReadCodec::CreatePlayerL()
	{
	ASSERT(iMngPlayer==NULL);
	TMNGChunkHeader header;
	iMngReadStream->SeekL(KMngSignatureSize);
	header.ReadL(*iMngReadStream);
	if (header.iChunkId != KMhdrChunkId)
		{
		User::Leave(KErrCorrupt);
		}
	TMhdrChunk* headerChunk = new (ELeave) TMhdrChunk(header);
	CleanupDeletePushL(headerChunk);
	headerChunk->ReadL(*iMngReadStream);

	iMngPlayer= CMngPlayer::NewL(iMngReadStream, *this, iMngSubframesNoLoops);  //Subframes with No Loops
	iImageFeatures = (&headerChunk->iNominalPlayTime)[1];	// get the next word following the iNominalPlayTime
	iMngPlayer->SetMngHeader(headerChunk);
	iMngPlayer->SetInternalDelayOn(EFalse);
	CleanupStack::Pop(); // headerChunk
	iMngReadStream->SeekL(0); // set srteam position back to the beginning	
	}

void CMngReadCodec::StopDecode()
	{
	if (iMngPlayer)
		{
		iMngPlayer->StopDecode();
		}
	delete iMngPlayer;
	iMngPlayer = NULL;
	Restart();
	}

void CMngReadCodec::Restart()
	{
	iFrameStatus= 0;
	iFrameNumber= 0;
	iFrameDelay = 0;
	}

void CMngReadCodec::DecodeL()
	{
	if (iMngPlayer == NULL)
		{
		CreatePlayerL();
		//set the destination bitmap to mngplayer
		iMngPlayer->SetDestinationBitmap(iCurrentFrame);
		}
	
	iMngPlayer->SetRequiredDisplayModes(iCurrentFrame->DisplayMode(), iCurrentMask ? iCurrentMask->DisplayMode() : ENone);
		
	if (iFrameNumber !=0 ) // that's not the first frame, so fetch bitmaps from MngPlayer
		{
		User::LeaveIfError( GetBitmaps() );
		}
	// that's last frame, so we have to send it to client
	if (iFrameStatus & CImageDisplayPlugin::EStatusNoMoreToDecode) 
		{
		SendFrameUpdate(KErrNone);
		}
	else
		{
		iMngPlayer->Decode();
		}
	}

void CMngReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/,
								TBool /*aDisableErrorDiffusion*/, 
								CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask)
	{
	if (aDestination.DisplayMode() != EColor16M &&
		aDestination.DisplayMode() != EColor16MA &&
		aDestination.DisplayMode() != EColor16MU)
		{
		User::Leave(KErrNotSupported);
		}
	
	if (aDestinationMask && aDestinationMask->DisplayMode() != EGray256)
		{
		User::Leave(KErrNotSupported);
		}
		
	iCurrentFrame	= &aDestination;
	iCurrentMask	= aDestinationMask;
	//set the destination bitmap to mngplayer
	if(iMngPlayer)
		{
		iMngPlayer->SetDestinationBitmap(iCurrentFrame);
		}
	}

TFrameState CMngReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	return TFrameState();
	}


void CMngReadCodec::InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& /* aFrameImageData */)
	{
	ASSERT(aFrameSettings.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameSettings;
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame); //EFrameInfoProcessingFrameHeader ?
	iFrameInfo->iOverallSizeInPixels	=iMngPlayer->FrameSize();
	iFrameInfo->iFrameCoordsInPixels.SetSize( iFrameInfo->iOverallSizeInPixels );
	iFrameInfo->iFrameDisplayMode		= EColor16M;
	iFrameInfo->iBitsPerPixel			= 24;
	iFrameInfo->iFlags					= TFrameInfo::EColor | TFrameInfo::ERestoreToBackground |
		(iMngPlayer->IsAlphaNeeded()? TFrameInfo::ETransparencyPossible|TFrameInfo::EAlphaChannel : 0);
	}

TInt CMngReadCodec::ReducedSize(const TSize& aOriginalSize, TInt /* aReductionFactor */, TSize& aReducedSize) const
	{
	aReducedSize = aOriginalSize;
	return KErrArgument;
	}

TInt CMngReadCodec::GetBitmaps()
	{
	//updatedrect
	//Subframes with No Loops
    
    TSize mngSize;
	
	if(iMngSubframesNoLoops)
		{
		mngSize = iFrameRect.Size(); 
		}
	else
    	{
    	mngSize = iMngPlayer->FrameSize();
		iFrameRect.SetRect(TPoint(0,0), mngSize.AsPoint());
    	}
	
	TInt error=KErrNone;
	if (mngSize != iCurrentFrame->SizeInPixels())
		{
		error=iCurrentFrame->Resize(mngSize);
		}
		
	if (iCurrentMask && error==KErrNone && mngSize != iCurrentMask->SizeInPixels())
		{
		error=iCurrentMask->Resize(mngSize);
		}
	if (error!=KErrNone )
		{
		return error;
		}

	iMngPlayer->Renderer()->CurrentFrame(*iCurrentFrame, iFrameRect );
	if (iCurrentMask)
		{
		iMngPlayer->Renderer()->CurrentMask(*iCurrentMask, iFrameRect );
		}
	return KErrNone;
	}

void CMngReadCodec::OnDecodeEvent(TUint aEventFlags, TInt aErrorCode)
	{
	if (aEventFlags==0)
		{
		iObserver.OnCodecEvent(MMngCodecObserver::KDataParseCompleted, aErrorCode);
		iMngPlayer->Pause();
		return;
		}
	if (aErrorCode!=KErrNone)
		{
		iFrameInfo->iFlags &= ( ~TUint(TFrameInfo::EMngMoreFramesToDecode) );
		iObserver.OnCodecEvent(MMngCodecObserver::KDecodeCompleted, aErrorCode);
		}
	else	// we have got frame decoded
		{
		//Subframes with No Loops
		if(iMngSubframesNoLoops)
			{
			iFrameRect = iMngPlayer->Renderer()->UpdatedRect();
			
			if(iFrameRect.Height() < 0)
				{
				iFrameRect.SetRect(0,0,0,0);
				}
				
			if(iFrameRect.Width() < 0) 
				{
				iFrameRect.SetRect(0,0,0,0);
				}
				
			iMngPlayer->Renderer()->UpdateRectEmpty();
			}
		//
		if (iFrameNumber != 0) // that's not the first frame
			{
			iFrameDelay = iMngPlayer->ThisFrameDelay();
			iMngPlayer->Pause();
			SendFrameUpdate(aErrorCode);
			}
		else
			{
			aErrorCode=GetBitmaps();
			}
		++iFrameNumber;
		iFrameStatus= aEventFlags;
		// image has got only 1 frame, so send update right now
		if (iFrameNumber==1 && (iFrameStatus & CImageDisplayPlugin::EStatusNoMoreToDecode))
			{
			SendFrameUpdate(aErrorCode);
			}
		}
	}

void CMngReadCodec::SendFrameUpdate(TInt aError)
	{
	iFrameInfo->iFlags &=(TFrameInfo::ELeaveInPlace-1);
	iFrameInfo->iFlags |= ((iFrameStatus & CImageDisplayPlugin::EStatusNoMoreToDecode)==0? TFrameInfo::EMngMoreFramesToDecode : 0);
	iFrameInfo->iFlags |= TFrameInfo::ERestoreToBackground;
	iFrameInfo->iDelay = TInt64(iFrameDelay.Int());
	iFrameInfo->iFrameCoordsInPixels = iFrameRect ;
	iFrameInfo->iBackgroundColor=iMngPlayer->Renderer()->BgColour();
	iObserver.OnCodecEvent(MMngCodecObserver::KFrameReady, aError);

#if !defined(DISABLE_LOGGING) && defined(_DEBUG)
	RDebug::Print(_L("Mng Frame#%4d Delay b4 next=%5d Rect(%d,%d)-(%d,%d)"),iFrameNumber, iFrameDelay.Int(),
									iFrameInfo->iFrameCoordsInPixels.iTl.iX, iFrameInfo->iFrameCoordsInPixels.iTl.iY,
									iFrameInfo->iFrameCoordsInPixels.iBr.iX, iFrameInfo->iFrameCoordsInPixels.iBr.iY
									);
#endif
	}

