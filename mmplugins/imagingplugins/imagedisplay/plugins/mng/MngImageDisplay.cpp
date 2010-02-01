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
#include <caf/data.h>
#include <icl/imagedisplayplugin.h>
#include <icl/imagedisplaypaniccodes.h>
#include <bitmaptransforms.h>
#include "MngImageDisplay.h"
#include "MngRenderer.h"
#include "MngReadStream.h"

#include "MngPlayer.h"


CMngImageDisplayPlugin* CMngImageDisplayPlugin::NewL()
	{
	CMngImageDisplayPlugin* self = new(ELeave) CMngImageDisplayPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMngImageDisplayPlugin::ConstructL()
	{
	}

CMngImageDisplayPlugin::~CMngImageDisplayPlugin()
	{
	if (iMngStream!=NULL)
		{
		iMngStream->Release();
		iMngStream=NULL;
		}

	if (NULL != iTransformer)
		{
		iTransformer->Cancel();
		}

	delete iMngPlayer;
	delete iHeaderChunk;
	delete iTransformer;
	delete iScaledFrame;
	delete iScaledMask;
	delete iFrame;
	delete iMask;
	}

CMngImageDisplayPlugin::CMngImageDisplayPlugin()
	{
	}

void CMngImageDisplayPlugin::OpenL()
	{
	ASSERT(iMngStream==NULL);
	iValidBitmap = EFalse;
	if ((Options() & CImageDisplay::EOptionThumbnail) == CImageDisplay::EOptionThumbnail)
		{
		User::Leave(KErrNotSupported);
		}
	
	switch ( SourceType() )
		{
		case CImageDisplayPlugin::EImgSrcFileName:
			{
			ContentAccess::CContent* Content = NULL;
			TRAPD(err, Content = ContentAccess::CContent::NewL(SourceFilename(), ContentAccess::EContentShareReadOnly));
			if (err!= KErrNone && err != KErrNoMemory)
				{
				Content = ContentAccess::CContent::NewL(SourceFilename(), ContentAccess::EContentShareReadWrite);
				}
			User::LeaveIfNull(Content);
			CleanupStack::PushL(Content);
			ContentAccess::CData* Data=Content->OpenContentL(SourceDataIntent(), SourceDataId() );
			CleanupStack::PopAndDestroy(Content);
			TInt error=Data->ExecuteIntent(SourceDataIntent());
			if (error != KErrNone)
				{
				delete Data;
				User::Leave( error );
				}
			
			iMngStream = CMngFileReadStream::NewL(Data);
			}
			break;

		case CImageDisplayPlugin::EImgSrcFileHandle:
			{
			ContentAccess::CContent* Content = ContentAccess::CContent::NewLC( SourceFileHandle() );
			ContentAccess::CData* Data=Content->OpenContentL(SourceDataIntent(), SourceDataId() );
			CleanupStack::PopAndDestroy(Content);
			TInt error=Data->ExecuteIntent(SourceDataIntent());
			if (error != KErrNone)
				{
				delete Data;
				User::Leave( error );
				}
			iMngStream = CMngFileReadStream::NewL(Data);
			}
			break;

		case CImageDisplayPlugin::EImgSrcDescriptor:
			iMngStream = CMngFileReadStream::NewL( SourceData() );
			break;

		default:
			ASSERT(EFalse);
			User::Leave(KErrArgument);
		}
	TRAPD(err, 
			ReadOriginalImgSizeL();
			iMngPlayer = CMngPlayer::NewL(iMngStream, *this, EFalse) //Subframes with No Loops. third arg added as EFalse
		);

	iMngStream->Release();
	iMngStream=NULL;
	delete iFrame;
	iFrame	=NULL;
	delete iMask;
	iMask	=NULL;
	delete iScaledFrame;
	iScaledFrame = NULL;
	delete iScaledMask;
	iScaledMask = NULL;
	User::LeaveIfError(err);

	iMngPlayer->SetMngHeader(iHeaderChunk);
	iHeaderChunk = NULL;
	
	// Deal with situation when client has called SetDisplayMode to override default display mode of 16M
	TDisplayMode destDisplayMode = DisplayMode();
	TDisplayMode maskDisplayMode = iMngPlayer->IsAlphaNeeded() ? EGray256 : ENone;
	if (destDisplayMode == ENone)
		{
		destDisplayMode = EColor16M; // assume default
		}
	else if (destDisplayMode == EColor16MA)
		{
		maskDisplayMode = ENone; // Decode only to a single 16MA destinaton
		}
	
	iMngPlayer->SetRequiredDisplayModes(destDisplayMode, maskDisplayMode);
	}

void CMngImageDisplayPlugin::ReadOriginalImgSizeL()
	{
	TMngSignature Signature;
	iMngStream->ReadL(Signature);
	if (Signature.Compare(KMngSignature) != 0)
		{
		User::Leave(KErrCorrupt);
		}
	TMNGChunkHeader Header;
	Header.ReadL(*iMngStream);
	if (Header.iChunkId != KMhdrChunkId)
		{
		User::Leave(KErrCorrupt);
		}
	iHeaderChunk = new (ELeave) TMhdrChunk(Header);
	iHeaderChunk->ReadL(*iMngStream);
	iMngStream->SeekL(0);
	iImageSizes.Reset();
	TSize frameSize(iHeaderChunk->iFrameWidth, iHeaderChunk->iFrameHeight);
	const TUint options=Options();
	if ( (options & CImageDisplay::EOptionRotateCw90)!=0 && (options & CImageDisplay::EOptionRotateCw270)!=0 )
		{
		User::Leave(KErrArgument);
		}
	if ( (options & CImageDisplay::EOptionRotateCw90)!=0 || (options & CImageDisplay::EOptionRotateCw270)!=0 ) 
		{
		frameSize.SetSize(iHeaderChunk->iFrameHeight, iHeaderChunk->iFrameWidth);
		}
	User::LeaveIfError( iImageSizes.Append(frameSize) );	
	}

void CMngImageDisplayPlugin::Play()
	{
	ASSERT(iMngPlayer);
	if (iState == EInitFailed)
		{
		MngPanic(EPanicNotInitialized);
		}
	iPluginStatus = EStatusBusy;
	if (iState == ETransforming)
		{
		iMngPlayer->Renderer()->CurrentFrame(*iFrame, iClipRect);
		if (iMask)
			{
			iMngPlayer->Renderer()->CurrentMask(*iMask, iClipRect);
			}
		iTransformer->Cancel();
		iTransformer->Restart();
		}
	else
		{
		iMngPlayer->Decode();
		}
	}

void CMngImageDisplayPlugin::Pause()
	{
	ASSERT(iMngPlayer);
	iMngPlayer->Pause();
	if (iState == ETransforming)
		{
		iTransformer->Cancel();
		}
	iPluginStatus &= ~TUint(EStatusBusy);
	iPluginStatus |= EStatusPaused; 
	}

void CMngImageDisplayPlugin::StopPlay()
	{
	ASSERT(iMngPlayer);
	iFrameNumber = 0;
	iMngPlayer->StopDecode();
	if (iTransformer != NULL)
		{
		iTransformer->Cancel();
		}
	iPluginStatus &= ~TUint(EStatusBusy|EStatusPaused);
	}

TInt CMngImageDisplayPlugin::ExtensionInterface(TUid /*aIFaceUid*/, TAny*& aIFacePtr)
	{
	aIFacePtr = NULL;
	return KErrNotSupported;	
	}
	
void CMngImageDisplayPlugin::GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const
	{
	if (iState < EInitCompleted)
		{
		MngPanic(EPanicNotInitialized);
		}
	if (iScaledFrame == NULL )
		{
		aBitmap = iFrame;
		aMask	= iMask;
		}
	else
		{
		aBitmap = iScaledFrame;
		aMask	= iScaledMask;
		}
	}

const CImageDisplay::RImageSizeArray& CMngImageDisplayPlugin::RecommendedImageSizes() const
	{
	return iImageSizes;
	}

TInt CMngImageDisplayPlugin::NumFrames(TInt& aNumFrames) const
	{
	aNumFrames = 0;
	if (iMngPlayer == NULL)
		{
		return KErrNotReady;
		}
	aNumFrames = iMngPlayer->NumFrames();
	if (aNumFrames == 0)
		{
		return KErrNotSupported;
		}
	return KErrNone;
	}

TUint CMngImageDisplayPlugin::ImageStatus() const
	{
	TUint Status = CImageDisplay::EImageTypeUnknown;
	if (iMngPlayer)
		{
		Status |= (iMngPlayer->IsAlphaNeeded()? CImageDisplay::EImageMasked : 0);
		Status |= (iMngPlayer->NumFrames()==1 ? CImageDisplay::EImageSingleFrame : 
												CImageDisplay::EImageMultiFrame|CImageDisplay::EImageAnimated);
		}
	return Status;
	}

TBool CMngImageDisplayPlugin::ValidBitmap() const
	{
	return iValidBitmap;
	}

void CMngImageDisplayPlugin::OnDecodeEvent(TUint aEventFlags, TInt aErrorCode)
	{
	ASSERT((iPluginStatus & EStatusPaused)==0);

	if (KErrNone==aErrorCode && aEventFlags==0)
		{
		TRAP(aErrorCode, CompleteInitL())
		if (KErrNone!=aErrorCode)
			{
			iPluginStatus	= EStatusNoMoreToDecode;
			iState			= EInitFailed;
			iMngPlayer->StopDecode(); 
			AsyncCallbackImageReady(iFrame, EStatusNoMoreToDecode, iOutputRect, aErrorCode);
			}
		else
			{
			iState = EInitCompleted;
			}
		return;
		}

	if (KErrNone==aErrorCode)
		{
		iValidBitmap = EFalse;
		iMngPlayer->Renderer()->CurrentFrame(*iFrame, iClipRect);
		if (iMask)
			{
			iMngPlayer->Renderer()->CurrentMask(*iMask, iClipRect);
			}
		iOutputRect=TRect(TPoint(0,0),iClipRect.Size().AsPoint());
		if ((aEventFlags&EStatusNoMoreToDecode) == 0)
			{
			iMngPlayer->Pause();
			aEventFlags |=EStatusPaused;
			}

		if ( (aEventFlags&EStatusFrameReady) && iTransformer != NULL ) 
			{
			iPluginStatus = aEventFlags; 
			iPluginStatus |= EStatusBusy;
			iState = ETransforming;
			iTransformer->Transform(iFrame, iMask, iScaledFrame,iScaledMask, MaintainAspectRatio());
			}
		else
			{
			iValidBitmap	= ETrue;
			iPluginStatus	= aEventFlags; 
			AsyncCallbackImageReady(iFrame, aEventFlags, iOutputRect, KErrNone);
			}
		}
	else
		{
		iPluginStatus = EStatusNoMoreToDecode;
		AsyncCallbackImageReady(iFrame, iPluginStatus, iOutputRect, aErrorCode);
		}
	}

/*static*/
TInt CMngImageDisplayPlugin::ScaleCallback(TAny* aPtr)
	{
	reinterpret_cast<CMngImageDisplayPlugin*>(aPtr)->OnScaleFinished();
	return KErrNone;
	}

void CMngImageDisplayPlugin::OnScaleFinished()
	{
	iState = EProcessing;
	iPluginStatus &= ~TUint(EStatusBusy);
	if (iTransformer->ScaleStatus() == KErrNone)
		{
		iValidBitmap = ETrue;
		iPluginStatus|=EStatusFrameReady;
		}
	else
		{
		iPluginStatus = EStatusNoMoreToDecode;
		}
	AsyncCallbackImageReady(iScaledFrame? iScaledFrame: iFrame, iPluginStatus, TRect(0,0,iBitmapSize.iWidth, iBitmapSize.iHeight), 
														iTransformer->ScaleStatus());
	}

void CMngImageDisplayPlugin::CompleteInitL()
	{
	iValidBitmap = EFalse;

	TRect FrameRect(TPoint(0,0),iMngPlayer->FrameSize().AsPoint());
	if (!SourceRect(iClipRect))
		{
		iClipRect=FrameRect;
		}
	else
		{
		TRect ClipTest(iClipRect);
		ClipTest.Intersection(FrameRect);
		if (ClipTest != iClipRect )
			{
			User::Leave( KErrArgument );
			}
		}

	iBitmapSize = DestinationSizeInPixels();

	TDisplayMode displayMode = DisplayMode();
	if(displayMode == ENone) // default value of base class plugin
		{
		displayMode = EColor16M; // maintain legacy behaviour
		}
	else if(displayMode != EColor16M && displayMode != EColor16MA && displayMode != EColor16MU)
		{
		User::Leave(KErrNotSupported);
		}

	iFrame = new (ELeave) CFbsBitmap();
	User::LeaveIfError( iFrame->Create(iClipRect.Size(), displayMode) );
	
	const TUint rotationOptions=Options() & ( CImageDisplay::EOptionMirrorVertical |
		  (CImageDisplay::EOptionMirrorVertical - CImageDisplay::EOptionRotateCw90) );
	if ( rotationOptions || iBitmapSize != iMngPlayer->FrameSize())
		{
		iTransformer= CAsyncTransformer::NewL(TCallBack(ScaleCallback, this));
		iTransformer->SetTranformOptions(rotationOptions);

		iScaledFrame = new (ELeave) CFbsBitmap();
		User::LeaveIfError( iScaledFrame->Create(iBitmapSize, displayMode) );
		iTransformer->SetScaleSize(iBitmapSize, (iBitmapSize!= iMngPlayer->FrameSize()) );
		}

	if (iMngPlayer->IsAlphaNeeded() && displayMode != EColor16MA)
		{
		iMask=new (ELeave) CFbsBitmap();
		User::LeaveIfError( iMask->Create(iClipRect.Size(), EGray256) );
		if (iScaledFrame != NULL)
			{
			iScaledMask=new (ELeave) CFbsBitmap();
			User::LeaveIfError( iScaledMask->Create(iBitmapSize, EGray256) );
			}
		}
	}


/*static*/
CAsyncTransformer* CAsyncTransformer::NewL(const TCallBack& aCallBack)
	{
	return new (ELeave) CAsyncTransformer(aCallBack);
	}

CAsyncTransformer::CAsyncTransformer(const TCallBack& aCallBack):CActive(EPriorityNormal),
														iResultCallback(aCallBack)
	{
	CActiveScheduler::Add(this);
	}

CAsyncTransformer::~CAsyncTransformer()
	{
	delete iBitmapRotator;
	delete iBitmapScaler;
	}

TInt CAsyncTransformer::RunError(TInt aError)
	{
	iResultStatus = aError;
	iResultCallback.CallBack();
	return KErrNone;
	}

TInt CAsyncTransformer::NextTransform(TUint& aTransFormTodo1)
	{
	if ((aTransFormTodo1&CImageDisplay::EOptionMirrorVertical)==CImageDisplay::EOptionMirrorVertical)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionMirrorVertical;
		return CBitmapRotator::EMirrorVerticalAxis;
		}
	if ((aTransFormTodo1&CImageDisplay::EOptionMirrorHorizontal)==CImageDisplay::EOptionMirrorHorizontal)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionMirrorHorizontal;
		return CBitmapRotator::EMirrorHorizontalAxis;
		}

	if ((aTransFormTodo1&CImageDisplay::EOptionRotateCw90)==CImageDisplay::EOptionRotateCw90)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionRotateCw90;
		return CBitmapRotator::ERotation90DegreesClockwise;
		}

	if ((aTransFormTodo1&CImageDisplay::EOptionRotateCw180)==CImageDisplay::EOptionRotateCw180)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionRotateCw180;
		return CBitmapRotator::ERotation180DegreesClockwise;
		}

	if ((aTransFormTodo1&CImageDisplay::EOptionRotateCw270)==CImageDisplay::EOptionRotateCw270)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionRotateCw270;
		return CBitmapRotator::ERotation270DegreesClockwise;
		}
	ASSERT(EFalse);
	return -1;
	}

void CAsyncTransformer::RunL()
	{
	iResultStatus = iStatus.Int();
	User::LeaveIfError( iStatus.Int() );

	switch (iState)
		{
		case ETransform1:
			{
			CreateTransformerL();
			CBitmapRotator::TRotationAngle TransfromInstr=CBitmapRotator::TRotationAngle(NextTransform(iTransFormTodo1));
			iBitmapRotator->Rotate(&iStatus, *iFirstBitmap, *iFirstBitmapDest, TransfromInstr);
			iScaleSrc1	= iFirstBitmapDest;
			SetActive();
			if (iTransFormTodo1==0)
				{
				iState = ETransform2;
				if (iSecondBitmap == NULL )
					{
					iState = EScale1;
					}
				}
			}
			break;

		case ETransform2:
			{
			CreateTransformerL();
			CBitmapRotator::TRotationAngle TransfromInstr=CBitmapRotator::TRotationAngle(NextTransform(iTransFormTodo2));
			iBitmapRotator->Rotate(&iStatus, *iSecondBitmap, *iSecondBitmapDest, TransfromInstr);
			iScaleSrc2	= iSecondBitmapDest;
			SetActive();
			if (iTransFormTodo2==0)
				{
				iState = EScale1;
				}
			}
			break;

		case EScale1:
			if (!iScaleNeeded)
				{
				iResultCallback.CallBack();
				}
			else
				{
				CreateScalerL();
				SetActive();
				if (iScaleSrc1->Handle() != iFirstBitmapDest->Handle())
					{
					iBitmapScaler->Scale(&iStatus, *iScaleSrc1, *iFirstBitmapDest, iKeepAspect);
					}
				else
					{
					iBitmapScaler->Scale(&iStatus, *iScaleSrc1, iScaleDestSize, iKeepAspect);
					}
				
				if (iSecondBitmap != NULL && iSecondBitmapDest != NULL)
					{
					iState = EScale2;
					}
				else
					{
					iState = EDone;
					}
				}
			break;

		case EScale2:
				CreateScalerL();
				SetActive();
				if (iScaleSrc2->Handle() != iSecondBitmapDest->Handle())
					{
					iBitmapScaler->Scale(&iStatus, *iScaleSrc2, *iSecondBitmapDest, iKeepAspect);
					}
				else
					{
					iBitmapScaler->Scale(&iStatus, *iScaleSrc2, iScaleDestSize, iKeepAspect);
					}
				
				iState = EDone;
			break;

		case EDone:
			iResultCallback.CallBack();
			break;
		}
	}

void CAsyncTransformer::DoCancel()
	{
	if (iBitmapScaler)
		{
		iBitmapScaler->Cancel();
		}
	if (iBitmapRotator)
		{
		iBitmapRotator->Cancel();
		}
	}

void CAsyncTransformer::CreateScalerL()
	{
	if (iBitmapScaler == NULL)
		{
		iBitmapScaler = CBitmapScaler::NewL() ;
		}
	}

void CAsyncTransformer::CreateTransformerL()
	{
	if (iBitmapRotator==NULL)
		{
		iBitmapRotator = CBitmapRotator::NewL();
		}
	}

void CAsyncTransformer::Transform(CFbsBitmap* aImage, CFbsBitmap* aMask, 
							CFbsBitmap* aDestImage, CFbsBitmap* aDestMask, TBool aMantainAspect)
	{
	ASSERT(!IsActive());

	iFirstBitmap	= aImage;
	iFirstBitmapDest= aDestImage;
	iSecondBitmap	= aMask;
	iSecondBitmapDest=aDestMask;
	iKeepAspect		= aMantainAspect;
	Restart();
	}

void CAsyncTransformer::Restart()
	{
	iScaleSrc1 = iFirstBitmap;
	iScaleSrc2 = iSecondBitmap;
	if (iTransformOptions != 0)
		{
		iState = ETransform1;
		iTransFormTodo2 = iTransFormTodo1 = iTransformOptions;
		}
	else
		{
		iState = EScale1;
		}
	
	iResultStatus = KErrNone;
	SetActive();
	TRequestStatus* pR=&iStatus;
	User::RequestComplete(pR,KErrNone);
	}


TInt CAsyncTransformer::ScaleStatus() const
	{
	return iResultStatus;
	}

void CAsyncTransformer::SetTranformOptions(TUint aOptions)
	{
	iTransformOptions = aOptions;
	}

void CAsyncTransformer::SetScaleSize(const TSize& aSize, TBool aScaleNeeded)
	{
	iScaleDestSize	= aSize;
	iScaleNeeded	= aScaleNeeded;
	}



