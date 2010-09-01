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
 @internalComponent 
*/


#include "GenericIclWrapper.h"
#include <icl/imagedisplaypaniccodes.h>
#include "MiscUtils.h"

#include "ExifImageDisplayPanic.h"

#define KZeroDelay TTimeIntervalMicroSeconds(0)
_LIT(KGenIclWrapperPanicCategory, "GenIclWrapperImgDisplay");

CGenericImageDisplayPlugin* CGenericImageDisplayPlugin::NewL()
	{
	CGenericImageDisplayPlugin* self = new(ELeave) CGenericImageDisplayPlugin(KGenIclWrapperPanicCategory);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CGenericImageDisplayPlugin::ConstructL()
	{
	User::LeaveIfError( iFs.Connect() );
	iPlayer		= new (ELeave) CImagePlayer(*this);
	iDelayedCb	= CDelayedCallback::NewL(*this);
	}

CGenericImageDisplayPlugin::~CGenericImageDisplayPlugin()
	{
	if (iTransformer != NULL)
		{
		iTransformer->Cancel();
		delete iTransformer;
		}
	if (iMaskTransformer != NULL)
		{
		iMaskTransformer->Cancel();
		delete iMaskTransformer;
		}
	
	delete iDelayedCb;
	delete iPlayer;
	delete iDecoder;
	iFs.Close();
	}

CGenericImageDisplayPlugin::CGenericImageDisplayPlugin(const TDesC& aPanicCategoryStr):
							iPanicCategory(aPanicCategoryStr),
							iMaxReductionFactor(KMaxReductionFactor),
							iMaxUnscaledSize(KMaxTInt,KMaxTInt),
							iExtHandler(*this),
							iScaleQuality(-1),
							iPluginImgStatus(CImageDisplay::EImageTypeUnknown)
	{
	}

void CGenericImageDisplayPlugin::Panic(TInt aPanicNumber) const
	{
	User::Panic(iPanicCategory, aPanicNumber);
	}

void CGenericImageDisplayPlugin::OpenL()
	{
	iValidBitmap = EFalse;
	const TUint option=(Options() & (CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionMainImage));
	if (option==CImageDisplay::EOptionsUndefined || 
			option == (CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionMainImage))
		{
		User::Leave(KErrNotSupported);
		}

	switch ( SourceType() )
		{
		case CImageDisplayPlugin::EImgSrcFileName:
			{
			
			TMMFileSource src(SourceFilename(), SourceDataId(), SourceDataIntent());
			iDecoder = CImageDecoder::FileNewL(iFs, src, CImageDecoder::EAllowGeneratedMask, KNullUid, KNullUid, RequiredImageClass());
			}
			break;

		case CImageDisplayPlugin::EImgSrcFileHandle:
			{
			TMMFileHandleSource src(SourceFileHandle(), SourceDataId(), SourceDataIntent());
			iDecoder = CImageDecoder::FileNewL(iFs, src, CImageDecoder::EAllowGeneratedMask, KNullUid, KNullUid, RequiredImageClass());
			}
			break;

		case CImageDisplayPlugin::EImgSrcDescriptor:
			iDecoder = CImageDecoder::DataNewL(iFs, SourceData(), CImageDecoder::EAllowGeneratedMask, KNullUid, KNullUid, RequiredImageClass());
			break;

		default:
			ASSERT(EFalse);
			User::Leave(KErrArgument);
		}
	iValidBitmap		= EFalse;
	iImageIsThumbnail	= (option==CImageDisplay::EOptionThumbnail);

	if (iImageIsThumbnail)
		{
		iDecoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail ); // leaves if there is no thumbnail
		}

	AfterOpenL(); // allow derived plug-in to perform some initialization
	iRotationOptions = EffectiveRotation();
	FillRecommendedSizesL();
	CacheImageStatus();
	}

void CGenericImageDisplayPlugin::Play()
	{
	iIsPaused = EFalse;
	if (iCallBackIsPending)
		{
		iDelayedCb->CallAfter(KZeroDelay);
		iCallBackIsPending = EFalse;
		return;
		}
	if (IsStatusSet(EStatusNoMoreToDecode))
		{
		ASSERT(IsStatusSet(EStatusPaused)==EFalse);
		if (iState < EInitCompleted)
			{
			Panic(EPanicNotInitialized);
			}
		if (iValidBitmap && iLastError==KErrNone)
			{
			ClearStatusFlag(EStatusPaused|EStatusBusy);
			iDelayedCb->CallAfter(KZeroDelay);
			return;
			}
		ClearStatusFlag(EStatusNoMoreToDecode);
		iState = EInitCompleted;
		}
	else if (IsStatusSet(EStatusPaused)==EFalse && iState>=EInitCompleted)
		{
		if (iState==EInitCompleted && IsStatusSet(EStatusBusy)==EFalse)
			{
			iPlayer->Play(*iDecoder, EffectiveDispMode(), (iImageIsThumbnail? 1 : CImagePlayer::KNoFrameLimit) );
			iState = EProcessing;
			}
		return;
		}

	ClearStatusFlag(EStatusPaused|EStatusNoMoreToDecode|EStatusBusy);
	switch (iState)
		{
		case EIdle:
			TRAPD(err,	
						CompleteInitL();
				);
			iOutputRect=TRect(0,0,DestinationSizeInPixels().iWidth, DestinationSizeInPixels().iHeight);
			if (err != KErrNone)
				{
				AsyncCallbackImageReady(NULL, EStatusNoMoreToDecode, iOutputRect, err);
				iState = EInitFailed;
				return;
				}
			iPlayer->Play(*iDecoder, EffectiveDispMode(), (iImageIsThumbnail? 1 : CImagePlayer::KNoFrameLimit) );
			iState = EProcessing;
			break;

		case EInitFailed:
			Panic(EPanicInitFailed);
			break;

		case EInitCompleted:
		case EProcessing:
			iPlayer->Resume();
			iState = EProcessing;
			break;

		case ETransforming:
			iTransformer->Restart();
			break;

		case ETransformingMask:
			iMaskTransformer->Restart();
			break;

		default:
			ASSERT(EFalse);
		}
	
	SetStatusFlag(EStatusBusy);
	}

void CGenericImageDisplayPlugin::Pause()
	{
	if (iIsPaused)
		{
		return;
		}
	if ( CallbackIsRunning() )
		{
		CancelCallback();
		// actually the framework may not be able to cancel the callback
		// if the execution point has already passed to the client code and it yielded to AS somehow
		// so we check if the callback was actually cancelled by trying it after cancel attempt
		iCallBackIsPending = !CallbackIsRunning(); 
		}
	else
		{
		if (iTransformer != NULL && iState == ETransforming)
			{
			iTransformer->Cancel();
			}
		else if (iMaskTransformer != NULL  && iState == ETransformingMask)
			{
			iMaskTransformer->Cancel();
			}
		else
			{
			iPlayer->Pause();
			}
		if (!IsStatusSet(EStatusNoMoreToDecode))
			{
			SetStatusFlag(EStatusPaused);
			iIsPaused = ETrue;
			}			
		}			
	ClearStatusFlag(EStatusBusy);
	}

void CGenericImageDisplayPlugin::StopPlay()
	{
	CancelCallback();
	iNextFrameDelay	= 0;
	iIsPaused		= EFalse;
	iValidBitmap	= EFalse;
	iCallBackIsPending = EFalse;
	if (iState >= EInitCompleted)
		{
		iState = EInitCompleted;
		}
	if (iTransformer != NULL)
		{
		iTransformer->Cancel();
		}
	if (iMaskTransformer != NULL)
		{
		iMaskTransformer->Cancel();
		}
	if (iPlayer != NULL)
		{
		iPlayer->Cancel();
		}
	if (iDelayedCb != NULL)
		{
		iDelayedCb->Cancel();
		}
	ClearStatusFlag(EStatusBusy|EStatusPaused);
	}

void CGenericImageDisplayPlugin::GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const
	{
	if (iState < EInitCompleted)
		{
		Panic(EPanicNotInitialized);
		}
	if (ValidBitmap())
		{
		aBitmap = iCurrentFrame;
		aMask	= iCurrentMask;
		}
	else
		{
		aBitmap = NULL;
		aMask	= NULL;
		}
	}
	
const CImageDisplay::RImageSizeArray& CGenericImageDisplayPlugin::RecommendedImageSizes() const
	{
	return iImageSizes;
	}

TInt CGenericImageDisplayPlugin::NumFrames(TInt& aNumFrames) const
	{
	if (iDecoder->IsImageHeaderProcessingComplete())
		{
		aNumFrames = iDecoder->FrameCount();
		return KErrNone;
		}
	aNumFrames = -1;
	return KErrNotReady;
	}

TUint CGenericImageDisplayPlugin::ImageStatus() const
	{
	return iPluginImgStatus;
	}

TBool CGenericImageDisplayPlugin::ValidBitmap() const
	{
	return iValidBitmap;
	}

void CGenericImageDisplayPlugin::CacheImageStatus()
	{
	TInt numFrames;
	iPluginImgStatus=(NumFrames(numFrames)==KErrNone && numFrames > 1)? CImageDisplay::EImageMultiFrame : CImageDisplay::EImageSingleFrame;
	for (TInt i=0; i<numFrames; ++i)
		{
		if (iDecoder->FrameInfo(i).iDelay.Int64() != 0)
			{
			iPluginImgStatus|= CImageDisplay::EImageAnimated;
			if (iImageHasFloatingSubImgs)
				{
				break;
				}
			}
		if (iDecoder->FrameInfo(i).iFrameCoordsInPixels != iDecoder->FrameInfo(0).iFrameCoordsInPixels)
			{
			iImageHasFloatingSubImgs = ETrue;
			if (iPluginImgStatus & CImageDisplay::EImageAnimated)
				{
				break;
				}
			}
		}
	iPluginImgStatus|=((iDecoder->FrameInfo().iFlags&TFrameInfo::ETransparencyPossible) ? CImageDisplay::EImageMasked : 0);
	iPluginImgStatus|=(ThumbnailExists()?CImageDisplay::EImageHasThumbnail : 0);
	iPluginImgStatus|=((iDecoder->FrameInfo().iFlags&TFrameInfo::EFullyScaleable)?CImageDisplay::EImageIsFullyScalable : 0);
	}

void CGenericImageDisplayPlugin::OnPlayEvent(TInt aErrorCode, TUint aEventFlags, CFbsBitmap* aFrame, CFbsBitmap* aMask)
	{
	ASSERT(IsStatusSet(EStatusPaused)==EFalse);
	iLastError = aErrorCode;
	if (aErrorCode == KErrNone)
		{
		iValidBitmap	= EFalse;
		iPluginStatus	= aEventFlags;
		iCurrentMask	= aMask;
		if (iTransformer != NULL ) 
			{
			SetStatusFlag(EStatusBusy);
			iState = ETransforming;
			iTransformer->Singleton().SetTrueSrcSize( iPlayer->OriginalFrameSize() );
			iTransformer->Transform(aFrame, MaintainAspectRatio());
			}
		else
			{
			iValidBitmap	= ETrue;
			iCurrentFrame	= aFrame;
			ClearStatusFlag(EStatusBusy);
			if ((aEventFlags&EStatusNoMoreToDecode)==0)
				{
				SetStatusFlag(EStatusPaused);	
				}
			const TTimeIntervalMicroSeconds delay(iNextFrameDelay);
			iNextFrameDelay = iPlayer->CurrentFrameInfo().iDelay;
			iDelayedCb->CallAfter(delay);
			}
		}
	else	
		{
		iState = EInitFailed;
		iPluginStatus = EStatusNoMoreToDecode;
		iCurrentFrame = NULL;
// we've got error, so there is no need to go via delayed callback
		AsyncCallbackImageReady(iCurrentFrame, iPluginStatus, iOutputRect, aErrorCode);
		}
	}

void CGenericImageDisplayPlugin::OnTransformDone(TInt aError)
	{
	TTimeIntervalMicroSeconds delay(0);
	if (aError == KErrNone)
		{
		if (iCurrentMask != NULL && iState == ETransforming)
			{
			iMaskTransformer->Transform(iCurrentMask, MaintainAspectRatio());
			iState = ETransformingMask;
			return;
			}
		SetStatusFlag(EStatusFrameReady);
		if (!IsStatusSet(EStatusNoMoreToDecode))
			{
			SetStatusFlag(EStatusPaused);
			}
		iTransformer->GetBitmap(iCurrentFrame);
		if (iCurrentMask)
			{
			iMaskTransformer->GetBitmap(iCurrentMask);
			}
		if (iCurrentFrame != NULL)
			{
			iValidBitmap= ETrue;
			delay		= iNextFrameDelay;
			}
		iNextFrameDelay = iPlayer->CurrentFrameInfo().iDelay;
		iState = EProcessing;
		}
	else
		{
		iState			= EInitFailed;
		iValidBitmap	= EFalse;
		iCurrentFrame	= NULL;
		SetStatusFlag(EStatusNoMoreToDecode);
		ClearStatusFlag(EStatusBusy|EStatusPaused);
		}
	
	ClearStatusFlag(EStatusBusy);
	iLastError = aError;
	iDelayedCb->CallAfter(delay);
	}

void CGenericImageDisplayPlugin::FillRecommendedSizesL()
	{
	ASSERT( iDecoder->IsImageHeaderProcessingComplete() );
	TRect clipRect;
	const TBool clipDefined=SourceRect(clipRect);
	const TFrameInfo& FrameInfo=iDecoder->FrameInfo();

	TSize imageSize(clipDefined? clipRect.Size() : FrameInfo.iFrameCoordsInPixels.Size() );
	RotateSize(imageSize, iRotationOptions);
	GenRecommendedSizesL(iImageSizes, imageSize, KMaxReductionFactor);
	}

void CGenericImageDisplayPlugin::GenRecommendedSizesL(RSizeArray& aArray,const TSize& aOriginalSize,TInt aMaxReduction )
	{
	aArray.Reset();
	TSize reducedSize(aOriginalSize);
	TInt i=0;
	do 
		{
		User::LeaveIfError( aArray.Append(reducedSize) );
		if (KErrNone != iDecoder->ReducedSize(aOriginalSize, ++i, reducedSize))
			{
			break;
			}

		} while ( i<=aMaxReduction &&
					reducedSize.iWidth <= MaxUnscaledSize().iWidth && reducedSize.iHeight <= MaxUnscaledSize().iHeight);
	}

TBool CGenericImageDisplayPlugin::SetupPlayerGetScalingNeeded(const TSize& aImageSize, const TSize& aTrueImgSize)
	{
	TSize effectiveDestSize( DestinationSizeInPixels() );
	RotateSize(effectiveDestSize, iRotationOptions);

	const TSize askSize( effectiveDestSize );
	// in case of clipping and upscaling  - decode to original size, clip then upscale
	// in case of clipping and downscaling- decode to downscaled, then clip
	const TSize destSize( aImageSize == aTrueImgSize ? askSize : // no clipping so decode to dest size
			(IsSize2Exceeds1(aTrueImgSize, askSize)?aTrueImgSize: 
													ScaleSize(effectiveDestSize, aImageSize, aTrueImgSize) )
						);
	if ((iDecoder->FrameInfo().iFlags&TFrameInfo::EFullyScaleable))
		{
		iPlayer->Setup( destSize, KZeroReductionFactor, KZeroReductionFactor);
		return EFalse;
		}

	TBool scalingNeeded=EFalse;
	TInt neededReduction=iDecoder->ReductionFactor(aTrueImgSize, destSize);
	TSize reducedSize(-1,-1);
	// if given reduction factor is not supported we'd go using scaler
	iDecoder->ReducedSize(aTrueImgSize, neededReduction, reducedSize);
	// animated images suffer from "1 pixel difference" problems when they are composed of reduced parts
	// so always use scaling for such images
	const TBool KNoReductionAllowed=(iImageHasFloatingSubImgs && (iPluginImgStatus & CImageDisplay::EImageAnimated));
	if (neededReduction>=0 && neededReduction<=KMaxReductionFactor && reducedSize==destSize 
			&& !KNoReductionAllowed)
		{
		// destination size is achivable through reduction factor
		TInt maxReduction=(IsSize2Exceeds1(iMaxUnscaledSize,destSize)? Max(iMaxReductionFactor,neededReduction) : neededReduction);
		iPlayer->Setup(reducedSize, maxReduction ,neededReduction);
		scalingNeeded=(maxReduction != neededReduction);
		}
	else
		{
		scalingNeeded=ETrue;
		if (IsSize2Exceeds1(aTrueImgSize, destSize) || KNoReductionAllowed)// we've got upscaling, or no reduction is allowed
			{
			iPlayer->Setup(aTrueImgSize, KZeroReductionFactor, KZeroReductionFactor); 
			}
		else
			{
			// we do not apply reduction for sizes smaller than iMaxUnscaledSize
			TInt reduction=iDecoder->ReductionFactor(destSize, iMaxUnscaledSize);
			reduction=Max(0, Min(iMaxReductionFactor, reduction));
			iPlayer->Setup(aTrueImgSize, reduction, KZeroReductionFactor); 
			// and we have to use further downscaling to achieve final target size
			}
		}
	return scalingNeeded;
	}

void CGenericImageDisplayPlugin::CompleteInitL()
	{
	ASSERT( iDecoder->IsImageHeaderProcessingComplete() );
	iNumberOfFrames = iDecoder->FrameCount();
	TBool needsMask=EFalse;

	for (TInt i=0; i<iNumberOfFrames && !needsMask; ++i)
		{
		needsMask = ( (iDecoder->FrameInfo(i).iFlags & TFrameInfo::ETransparencyPossible) != 0 );
		}
	const TFrameInfo& frameInfo=iDecoder->FrameInfo();
	TRect frameRect(frameInfo.iFrameCoordsInPixels);
	const TSize imageSize(frameRect.Size());
	TRect clipRect;
	const TBool clipDefined=SourceRect(clipRect);
	if (!clipDefined)
		{
		clipRect=frameRect;
		}
	else
		{
		TRect clipTest(clipRect);
		clipTest.Intersection(frameRect);
		if (clipTest != clipRect ) // clipping rect is outside the image rect
			{
			User::Leave( KErrArgument );
			}
		}
	
	TBool scalingNeeded=SetupPlayerGetScalingNeeded( clipDefined?clipRect.Size():imageSize, imageSize);

	if (iRotationOptions || clipDefined || scalingNeeded )
		{
		CTransformerSharedData* singleton=CTransformerSharedData::NewL();
		CleanupReleasePushL(*singleton);
		singleton->SetTransformOptions(iRotationOptions);
		singleton->SetTrueSrcSize(imageSize);
		if (clipDefined)
			{
			singleton->SetClipRect(clipRect);
			}
		iTransformer= CAsyncTransformer::NewL(*this, *singleton, DestinationSizeInPixels());

		if (needsMask)
			{
			iMaskTransformer= CAsyncTransformer::NewL(*this, *singleton, DestinationSizeInPixels());
			}
		CleanupStack::PopAndDestroy(singleton);

		if (iScaleQuality!=-1)
			{
			User::LeaveIfError( iTransformer->SetScaleQuality(iScaleQuality) );
			if (iMaskTransformer)
				{
				User::LeaveIfError( iMaskTransformer->SetScaleQuality(iScaleQuality) );
				}
			}
		}
	iState = EInitCompleted;
	}

TInt CGenericImageDisplayPlugin::ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr)
	{
	aIFacePtr = NULL;
	if (aIFaceUid == KUidGenIclImageDisplayPluginExtUid)
		{
		aIFacePtr = static_cast<MGenIclImageDisplayExtension*>(&iExtHandler);
		return KErrNone;
		}
	return KErrNotSupported;
	}

TBool CGenericImageDisplayPlugin::ThumbnailExists() const
	{
	return EFalse;
	}

TUint CGenericImageDisplayPlugin::EffectiveRotation() const
	{
	return Options() & ( CImageDisplay::EOptionMirrorVertical |
			(CImageDisplay::EOptionMirrorVertical - CImageDisplay::EOptionRotateCw90) );
	}

TInt CGenericImageDisplayPlugin::DoSetScaleQuality(TInt aQualityValue)
	{
	iScaleQuality = aQualityValue;
	TInt err=KErrNone;
	if (iTransformer != NULL)
		{
		err=iTransformer->SetScaleQuality(aQualityValue);
		}
	if (err==KErrNone && iMaskTransformer != NULL)
		{
		err=iMaskTransformer->SetScaleQuality(aQualityValue);
		}

	return err;
	}

TInt CGenericImageDisplayPlugin::DoSetMaximumReductionFactor(TInt aMaxReductionFactor)
	{
	if (aMaxReductionFactor<0 || aMaxReductionFactor>KMaxReductionFactor)
		{
		return KErrArgument;
		}
	iMaxReductionFactor = aMaxReductionFactor;
	return KErrNone;	
	}

TInt CGenericImageDisplayPlugin::SetMaxUnscaledSize(const TSize& aSize)
	{
	if (aSize.iWidth < 1 || aSize.iHeight < 1)
		{
		return KErrArgument;
		}
	iMaxUnscaledSize = aSize;
	return KErrNone;
	}

void CGenericImageDisplayPlugin::AfterOpenL()
	{
	}

TUid CGenericImageDisplayPlugin::RequiredImageClass() const
	{
	return KNullUid;
	}

void CGenericImageDisplayPlugin::OnCallback()
	{
	if (iIsPaused)
		{
		iCallBackIsPending = ETrue;
		return;
		}
	AsyncCallbackImageReady(iCurrentFrame, iPluginStatus, iOutputRect, iLastError);
	}

/*static*/
CDelayedCallback* CDelayedCallback::NewL(MCallbackClient& aClient)
	{
	CDelayedCallback* self=new (ELeave) CDelayedCallback(aClient);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CDelayedCallback::CallAfter(const TTimeIntervalMicroSeconds& aAfter)
	{
	TTime now;	
	now.HomeTime();
	const TInt64 KTimerTreshold	=8000; // 8 ms, we don't go trought timer for delays less than that
	const TInt64 KDelayDelta	=4000; // 4 ms, make delay 4 ms less to account time before we reach client
	const TInt64 requiredDelay = aAfter.Int64() - (now.Int64() - iLatestCallbackAt.Int64());
	if ( requiredDelay <= KTimerTreshold )
		{
		iLatestCallbackAt = now;
		iClient.OnCallback();
		}
	else
		{
		CTimer::After( TTimeIntervalMicroSeconds32( I64INT(requiredDelay-KDelayDelta) ) );
		}
	}

void CDelayedCallback::Cancel()
	{
	CTimer::Cancel();
	}

void CDelayedCallback::RunL()
	{
	iLatestCallbackAt.HomeTime();
	iClient.OnCallback();
	}

const TUid TExtTie::Uid() const
	{
	return KUidGenIclImageDisplayPluginExtUid;
	}

void TExtTie::Release()
	{
	}

TInt TExtTie::SetScaleQuality(TInt aQualityLevel)
	{
	return iImplementor.DoSetScaleQuality(aQualityLevel);
	}

TInt TExtTie::SetMaximumReductionFactor(TInt aMaxReductionFactor)
	{
	return iImplementor.DoSetMaximumReductionFactor(aMaxReductionFactor);
	}
	
TInt TExtTie::SetMaximumNonReducedSize(const TSize& aSize)
	{
	return iImplementor.SetMaxUnscaledSize(aSize);
	}



