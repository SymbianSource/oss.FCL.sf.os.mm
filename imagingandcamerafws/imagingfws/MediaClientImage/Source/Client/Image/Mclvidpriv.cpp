// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <bitdev.h>
#include <mdaimageconverter.h>
#include "MdaImageConverterPriv.h"
#include <ecom/ecom.h>

#include <imageconversion.h>
#include <bitmaptransforms.h>

#include "CompatAids.h"

#if defined(__ECOM_POWERDOWN_WORKAROUND)
	const TInt KWaitTimeMax = 8;
	const TInt KWaitTimeIncrement = 250000; // 0.25s
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)

GLDEF_C void Panic (TInt aPanic)
	{
	_LIT(KMdaImageConverter, "MDA Image Converter");
	User::Panic(KMdaImageConverter, aPanic);
	}


//
// CMdaImageUtilityPriv
//

CMdaImageUtilityPriv::CMdaImageUtilityPriv(MMdaImageUtilObserver& aObserver, CMdaImageUtility* aParent):
	iObserver(aObserver), iParent(aParent)
	{}

CMdaImageUtilityPriv::~CMdaImageUtilityPriv()
	{
	delete iImageData;
	delete iFrameData;
	iFs.Close();
	CloseEcomSession();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	iWaitTimer.Close(); 
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
	}

void CMdaImageUtilityPriv::CloseEcomSession()
	{
	if (iEcomSession)
		iEcomSession->Close();
	iEcomSession = NULL;
	}

void CMdaImageUtilityPriv::OpenEcomSessionL()
	{
	if (iEcomSession==NULL)
		iEcomSession = &(REComSession::OpenL());
	}

/**
 *
 * Closes the open file-based or descriptor-based image.
 *
 * Any conversion operation in progress is cancelled.
 *
 */
void CMdaImageUtilityPriv::Close()
	{
	Close(ETrue);
	}

// officially reset at end - in practice we always make this call before opening too
void CMdaImageUtilityPriv::Close(TBool aFullClose)
	{
	ASSERT(iParent);
	iParent->Cancel();
	DoClose();
	if (aFullClose)
		CloseEcomSession();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	iWaitTimeCount = 0; 
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
	}

void CMdaImageUtilityPriv::DoClose()
	{
	}

void CMdaImageUtilityPriv::Initialize()
	{
	ASSERT(iParent);
	CActiveScheduler::Add(iParent);
	}

void CMdaImageUtilityPriv::OpenFsSessionL()
	{
	ASSERT(iFs.Handle()==0); // intended for use from ConstructL only

	User::LeaveIfError(iFs.Connect());
	}

#if defined(__ECOM_POWERDOWN_WORKAROUND)
void CMdaImageUtilityPriv::OpenWaitTimerL()
	{
	ASSERT(iWaitTimer.Handle()==0); // intended for use from ConstructL etc only
	User::LeaveIfError(iWaitTimer.CreateLocal());
	}
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)

/**
Retrieves information about the specified frame.

This function should only be called after an existing image has been
successfully opened or a new image successfully created.

@param     aFrame
           The frame number. This value is relative to zero. The
           value must be less than the total number of frames in
           the image, otherwise the function raises a USER 0
           panic
@param     aInfo
           Frame information.
*/
void CMdaImageUtilityPriv::FrameInfo(TInt /*aFrame*/,TFrameInfo& /*aInfo*/) const
	{
	__PANIC_COMPAT(Panic(EMDAImConvPanicNotImplemented)); // only implemented for data read
	}

/**
Returns the number of frames in the image.

@return   The number of frames in the image
*/
TInt CMdaImageUtilityPriv::FrameCount() const
	{
	__PANIC_COMPAT(Panic(EMDAImConvPanicNotImplemented)); // only implemented for data read
	return 0; 
	}

//
// Performs a User::RequestComplete() operation on this itself.
//
void CMdaImageUtilityPriv::SelfComplete(TInt aError)
	{
	ASSERT(iParent);
	iParent->SelfComplete(aError);
	}

// Effectively converts the old format converts to the new ones. The
// results are placed in iImageType and iImageSubType, plus perhaps iImageData
//
//	TMdaClipFormat* aFormat = NULL
//		The data format. This is optional.
//	TMdaPackage* aCodec = NULL
//		The codec to use. This is optional.
//	TMdaPackage* aExtra = NULL
//		Additional information which the Media Server may need
//		to recognise the data format. This is optional.
//
void CMdaImageUtilityPriv::SetUidsFromOrigFormatsL(TMdaClipFormat* aFormat,
											   TMdaPackage* aCodec,
											   TMdaPackage* /*aExtra*/)
	{
	// set default values first
	delete iImageData; iImageData=NULL;
	iImageType = KNullUid;
	iImageSubType = KNullUid;

	// Note in general below we ignore the codec settings, unless the latter is used
	// to indicate format. 

	if (aFormat)
		{
		TUid formatUid = aFormat->Uid();
		if (formatUid==KUidMdaBmpClipFormat)
			{
			iImageType = KImageTypeBMPUid;
			if (aCodec)
				{
				iImageData = new (ELeave) TBmpImageData;
				TBmpImageData *data = STATIC_CAST(TBmpImageData*,iImageData);
				if (aCodec->Uid() == KUidMda1BppBmpCodec)
					data->iBitsPerPixel = 1;
				else if (aCodec->Uid() == KUidMda4BppBmpCodec)
					data->iBitsPerPixel = 4;
				else if (aCodec->Uid() == KUidMda8BppBmpCodec)
					data->iBitsPerPixel = 8;
				else if (aCodec->Uid() == KUidMda24BppBmpCodec)
					data->iBitsPerPixel = 24;
				else
					{
					// unknown value - so leave undefined
					delete iImageData; iImageData = NULL;
					}
				}
			}
		else if (formatUid==KUidMdaGif87aClipFormat || formatUid==KUidMdaGif89aClipFormat) 
			iImageType = KImageTypeGIFUid; 
		else if (formatUid==KUidMdaJfifClipFormat)
			{
			TMdaJfifClipFormat *jfifFormat = STATIC_CAST(TMdaJfifClipFormat*, aFormat);
			TMdaJpgSettings::TColorSampling sampling = jfifFormat->iSettings.iSampleScheme;
			iImageType = KImageTypeJPGUid; 
			iImageData = new (ELeave) TJpegImageData;
			TJpegImageData* data = STATIC_CAST(TJpegImageData*, iImageData);
			ASSERT(TInt(TMdaJpgSettings::EMonochrome)==TInt(TJpegImageData::EMonochrome)); // check old sample type = new one
			ASSERT(TInt(TMdaJpgSettings::EColor420)==TInt(TJpegImageData::EColor420));
			ASSERT(TInt(TMdaJpgSettings::EColor422)==TInt(TJpegImageData::EColor422));
			ASSERT(TInt(TMdaJpgSettings::EColor444)==TInt(TJpegImageData::EColor444));
			ASSERT(sampling>=TMdaJpgSettings::EMonochrome && sampling <= TMdaJpgSettings::EColor444); // valid range
			data->iSampleScheme = TJpegImageData::TColorSampling(sampling); // can just copy value
			data->iQualityFactor = jfifFormat->iSettings.iQualityFactor;
			}
		else if (formatUid==KUidMdaMbmClipFormat)
			{
			TMdaMbmClipFormat *mbmFormat = STATIC_CAST(TMdaMbmClipFormat*, aFormat);
			TDisplayMode displayMode = mbmFormat->iDisplayMode;
			iImageType = KImageTypeMBMUid;
			iFrameData = new (ELeave) TMbmEncodeData;
			TMbmEncodeData* data = STATIC_CAST(TMbmEncodeData*, iFrameData);
			data->iDisplayMode = displayMode;
			}
		else if (formatUid==KUidMdaOtaClipFormat) // SMS OTA format
			iImageType = KImageTypeOTAUid; 
		else if (formatUid==KUidMdaWbmpClipFormat)
			iImageType = KImageTypeWBMPUid;
		else if (formatUid==KUidMdaPngClipFormat)
			iImageType = KImageTypePNGUid;
		else if (formatUid==KUidMdaTiffLittleEndianClipFormat ||
				 formatUid==KUidMdaTiffBigEndianClipFormat)
			{
			iImageType=KImageTypeTIFFUid;
			if (formatUid==KUidMdaTiffLittleEndianClipFormat)
				iImageSubType=KImageTypeTIFFSubTypeLittleEndianUid;
			else
				{
				ASSERT(formatUid==KUidMdaTiffBigEndianClipFormat);
				iImageSubType=KImageTypeTIFFSubTypeBigEndianUid;
				}
			}
		else if (formatUid==KUidMdaTiffBigEndianClipFormat)
			{
			iImageType = KImageTypeTIFFUid;
			iImageSubType = KImageTypeTIFFSubTypeBigEndianUid;
			}
		else if (formatUid==KUidMdaWmfClipFormat ||
			     formatUid==KUidMdaWmfApmClipFormat ||
			     formatUid==KUidMdaWmfClpClipFormat)
			{
			iImageType = KImageTypeWMFUid;
			if (formatUid==KUidMdaWmfApmClipFormat)
				iImageSubType = KImageTypeWMFSubTypeApmUid;
			else if (formatUid==KUidMdaWmfClpClipFormat)
				iImageSubType = KImageTypeWMFSubTypeClpUid;
			else
				{
				ASSERT(formatUid==KUidMdaWmfClipFormat);
				iImageSubType = KImageTypeWMFSubTypeStdUid;
				}
			}
		else
			User::Leave(KErrNotSupported);
		}
	}

//
// CMdaImageDataReadUtilityPriv
//

CMdaImageDataReadUtilityPriv::CMdaImageDataReadUtilityPriv(MMdaImageUtilObserver& aObserver,
														   CMdaImageDataReadUtility* aParent):
	CMdaImageUtilityPriv(aObserver, aParent)
	{
	}

CMdaImageDataReadUtilityPriv::~CMdaImageDataReadUtilityPriv()
	{
	Close(ETrue);
	ASSERT(iDecoder==NULL); // should have been deleted by Close
	}

void CMdaImageDataReadUtilityPriv::ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber)
	{
	ASSERT(Parent());
	if (iState != EStateOpen)
		User::Leave(KErrNotReady);

	ASSERT(iDecoder);

	TBool doContinue = EFalse;
	if (iPrevConvertUnderflowed && iKeptFrameNumber == aFrameNumber)
		{
		// if previous convert Underflowed, and same bitmap, we ContinueConvert instead of Convert
		if (iDestinationHandle == aBitmap.Handle() && 
			iDestinationDisplayMode == aBitmap.DisplayMode() && 
			iDestinationMaskHandle == 0 &&
			iDestinationSize == aBitmap.SizeInPixels())
			doContinue = ETrue;
		}

	if (doContinue)
		iDecoder->ContinueConvert(&Parent()->iStatus);
	else
		{
		iDestinationHandle = aBitmap.Handle();
		iDestinationMaskHandle = 0;
		if (aBitmap.Handle())
			{
			iDestinationDisplayMode = aBitmap.DisplayMode();
			iDestinationSize = aBitmap.SizeInPixels();
			}
		iDecoder->Convert(&Parent()->iStatus, aBitmap, aFrameNumber); 
		}
	Parent()->SetActive();
	iState = EStateConverting;
	}

void CMdaImageDataReadUtilityPriv::ConvertL(CFbsBitmap& aBitmap,CFbsBitmap& aMaskBitmap,TInt aFrameNumber)
	{
	ASSERT(Parent());
	if (iState != EStateOpen)
		User::Leave(KErrNotReady);

	ASSERT(iDecoder);

	TBool doContinue = EFalse;
	if (iPrevConvertUnderflowed && iKeptFrameNumber == aFrameNumber)
		{
		// if previous convert Underflowed, and same bitmap, we ContinueConvert instead of Convert
		if (iDestinationHandle == aBitmap.Handle() && 
			iDestinationDisplayMode == aBitmap.DisplayMode() && 
			iDestinationMaskHandle == aMaskBitmap.Handle() &&
			iDestinationMaskDisplayMode == aMaskBitmap.DisplayMode() && 
			iDestinationSize == aBitmap.SizeInPixels())
			doContinue = ETrue;
		}

	if (doContinue)
		iDecoder->ContinueConvert(&Parent()->iStatus);
	else
		{
		iDestinationHandle = aBitmap.Handle();
		iDestinationMaskHandle = aMaskBitmap.Handle();
		if (aBitmap.Handle())
			{
			iDestinationDisplayMode = aBitmap.DisplayMode();
			iDestinationSize = aBitmap.SizeInPixels();
			}
		if (aMaskBitmap.Handle())
			iDestinationMaskDisplayMode = aMaskBitmap.DisplayMode();
		iDecoder->Convert(&Parent()->iStatus, aBitmap, aMaskBitmap, aFrameNumber); 
		}
	Parent()->SetActive();
	iState = EStateConverting;
	}

void CMdaImageDataReadUtilityPriv::CancelConvertL()
	{
	Parent()->Cancel();
	}

void CMdaImageDataReadUtilityPriv::RunL()
	{
	ASSERT(Parent());
	switch (iState)
		{
		case EStateClosed:
			ASSERT(EFalse); // should not happen
			break;
		case EStateOpening:
			{
			TInt error = Parent()->iStatus.Int();
			if (error==KErrNone)
				TRAP(error, OpenEcomSessionL());
			if (error==KErrNone)
				TRAP(error, DoOpenL());
			ASSERT(error!=KErrNone || iDecoder); // if successful should have an iDecoder
			if (error==KErrNone)
				{
				iState=EStateOpen;
				iObserver.MiuoOpenComplete(error);
				}
			else
				{
#if defined(__ECOM_POWERDOWN_WORKAROUND)
				if (error==KEComErrListCurrentlyUnavailable && iWaitTimeCount < KWaitTimeMax)
					{
					iWaitTimeCount ++;
					TInt waitTime = iWaitTimeCount * KWaitTimeIncrement;
					iState = EStateOpeningWait;
					iWaitTimer.After(Parent()->iStatus, waitTime);
					Parent()->SetActive();
					}
				else
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
					{
					if (error!=KErrUnderflow) // keep ecom session on streaming
						CloseEcomSession();
					iState=EStateClosed;
					iObserver.MiuoOpenComplete(error);
					}
				}
			}
			break;
		case EStateOpeningWait:
#if defined(__ECOM_POWERDOWN_WORKAROUND)
			{
			TInt error = Parent()->iStatus.Int();	
			if (error!=KErrNone)
				{
				CloseEcomSession();
				iState = EStateClosed;
				iObserver.MiuoOpenComplete(error);
				}
			else
				{
				iState = EStateOpening;
				SelfComplete(error);
				}
			}
#else
			ASSERT(EFalse); // should not get here if __ECOM_POWERDOWN_WORKAROUND not defined
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
			break;
		case EStateOpen:
			ASSERT(EFalse); // should not happen
			break;
		case EStateConverting:
			{
			TInt error = Parent()->iStatus.Int();

			//Add comments
			if(error==KErrNone)
				TRAP(error,GetCommentL());

			iState = EStateOpen; // clear first to allow immediate re-conversion
			iPrevConvertUnderflowed = error==KErrUnderflow;
			iObserver.MiuoConvertComplete(error);
			}
			break;
		default:
			ASSERT(EFalse); // unknown state
		}
	}

void CMdaImageDataReadUtilityPriv::DoCancel()
	{
	switch (iState)
		{
		case EStateClosed:
			ASSERT(EFalse); // should not happen
			break;
		case EStateOpening:
			// ignore - makes no difference to do something here
			break;
		case EStateOpeningWait:
#if defined(__ECOM_POWERDOWN_WORKAROUND)
			iWaitTimer.Cancel();
#else
			ASSERT(EFalse);
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
			break;
		case EStateOpen:
			ASSERT(EFalse); // should not happen
			break;
		case EStateConverting:
			{
			iDecoder->Cancel(); 
			iState = EStateOpen; 
			}
			break;
		default:
			ASSERT(EFalse); // unknown state
		}
	}

void CMdaImageDataReadUtilityPriv::DoClose()
	{
	ASSERT(Parent());
	ASSERT(!Parent()->IsActive()); // Cancel should be called by this point
	if (iDecoder)
		iDecoder->Cancel();
	delete iDecoder; iDecoder = NULL;
	iComment.ResetAndDestroy();
	iPrevConvertUnderflowed = EFalse;
	iState = EStateClosed;
	}

const TDesC& CMdaImageDataReadUtilityPriv::FrameCommentL(TInt aFrameNumber)
	{
	if (aFrameNumber >= iComment.Count())
		User::Leave(KErrArgument);

	if (iComment[aFrameNumber])
		return *iComment[aFrameNumber];

	return KNullDesC;
	}

void CMdaImageDataReadUtilityPriv::GetCommentL()
	{
	if(iDecoder->IsImageHeaderProcessingComplete()&&iComment.Count()==0)
		{
		TInt noOfComments = 0;
		TInt commentNo = 0;

		const TInt noOfFrames = iDecoder->FrameCount();
		for(TInt currentFrame=0; currentFrame<noOfFrames; currentFrame++)
			{
			HBufC* nullComment = KNullDesC().AllocL();
			CleanupStack::PushL(nullComment);
			User::LeaveIfError(iComment.Append(nullComment));
			CleanupStack::Pop(nullComment);

			//Add the general comments if it is the first frame
			if(currentFrame==0)
				{
				noOfComments = iDecoder->NumberOfImageComments();
				for(commentNo = 0; commentNo < noOfComments; commentNo++)
					{
					HBufC* currentComment = iComment[currentFrame];
					HBufC* newComment = iDecoder->ImageCommentL(commentNo);
					CleanupStack::PushL(newComment);

					TInt newLength = currentComment->Length() + newComment->Length();

					iComment[currentFrame] = currentComment->ReAllocL(newLength);
					iComment[currentFrame]->Des().Append(*newComment);

					CleanupStack::PopAndDestroy(newComment);
					}
				}

			//Add the frame comments
			noOfComments = iDecoder->NumberOfFrameComments(currentFrame);
			for(commentNo = 0; commentNo < noOfComments; commentNo++)
				{
				HBufC* currentComment = iComment[currentFrame];
				HBufC* newComment = iDecoder->FrameCommentL(currentFrame,commentNo);
				CleanupStack::PushL(newComment);

				TInt newLength = currentComment->Length() + newComment->Length();

				iComment[currentFrame] = currentComment->ReAllocL(newLength);
				iComment[currentFrame]->Des().Append(*newComment);

				CleanupStack::PopAndDestroy(newComment);
				}
			}
		}
	}

/**
 *
 * Retrieves information about the specified frame.
 *
 * This function should only be called after an existing image has been
 * successfully opened or a new image successfully created.
 *
 * @param     "TInt aFrame"
 *            The frame number. This value is relative to zero. The
 *            value must be less than the total number of frames in
 *            the image, otherwise the function raises a USER 0
 *            panic
 * @param     "TFrameInfo& aInfo"
 *            Frame information.
 */
void CMdaImageDataReadUtilityPriv::FrameInfo(TInt aFrame,TFrameInfo& aInfo) const
	{
	__ASSERT_ALWAYS(iDecoder,Panic(EMDAImConvPanicNotYetOpen));
	TFrameInfo iclInfo = iDecoder->FrameInfo(aFrame);
	aInfo.iFrameCoordsInPixels = iclInfo.iFrameCoordsInPixels;
	aInfo.iFrameSizeInTwips = iclInfo.iFrameSizeInTwips;
	aInfo.iBitsPerPixel = iclInfo.iBitsPerPixel;
	aInfo.iDelay = iclInfo.iDelay;
	aInfo.iFlags = iclInfo.iFlags;
	aInfo.iOverallSizeInPixels = iclInfo.iOverallSizeInPixels;
	}

/**
 *
 * Returns the number of frames in the image.
 *
 * @return   "TInt"
 *            The number of frames in the image
 */
TInt CMdaImageDataReadUtilityPriv::FrameCount() const
	{
	__ASSERT_ALWAYS(iDecoder,Panic(EMDAImConvPanicNotYetOpen));
	return iDecoder->FrameCount();
	}

//
// CMdaImageDataWriteUtilityPriv
//

CMdaImageDataWriteUtilityPriv::CMdaImageDataWriteUtilityPriv(MMdaImageUtilObserver& aObserver,
															 CMdaImageDataWriteUtility* aParent):
	CMdaImageUtilityPriv(aObserver, aParent)
	{}

CMdaImageDataWriteUtilityPriv::~CMdaImageDataWriteUtilityPriv()
	{
	Close(ETrue);
	ASSERT(iEncoder==NULL); // should have been deleted by Close
	ASSERT(iFrameImageData==NULL); // ditto
	}

void CMdaImageDataWriteUtilityPriv::ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber)
	{
	ASSERT(Parent())
	if (iState == EStateCancelled) 
		{
		delete iEncoder;
		iEncoder = NULL;

		TRAPD(err,DoCreateL(EFalse));

		if (err==KErrNone)
			iState = EStateOpen;
		}

	if (iState != EStateOpen)
		User::Leave(KErrNotReady);

	if (aFrameNumber != 0)
		User::Leave(KErrArgument);

	ASSERT(iEncoder);

	iEncoder->Convert(&Parent()->iStatus, aBitmap, iFrameImageData);

	Parent()->SetActive();
	iState = EStateConverting;
	}

void CMdaImageDataWriteUtilityPriv::ConvertL(CFbsBitmap& /*aBitmap*/,const TRect& /*aSourceRect*/,TInt /*aFrameNumber*/)
	{
	User::Leave(KErrNotSupported);
	}

void CMdaImageDataWriteUtilityPriv::ConvertL(CFbsBitmap& /*aBitmap*/,CFbsBitmap& /*aMaskBitmap*/,TInt /*aFrameNumber*/)
	{
	User::Leave(KErrNotSupported);
	}

void CMdaImageDataWriteUtilityPriv::CancelConvertL()
	{
	ASSERT(Parent());
	Parent()->Cancel();
	}

void CMdaImageDataWriteUtilityPriv::RunL()
	{
	ASSERT(Parent());
	switch (iState)
		{
		case EStateClosed:
			ASSERT(EFalse); // should not happen
			break;
		case EStateCreating:
			{
			TInt error = Parent()->iStatus.Int();
			if (error==KErrNone)
				TRAP(error, DoCreateL());
			if (error==KErrNone)
				iState=EStateOpen;
			else
				iState=EStateClosed;
			iObserver.MiuoCreateComplete(error);
			}
			break;
		case EStateOpen:
			ASSERT(EFalse); // should not happen
			break;
		case EStateConverting:
			{
			TInt error = Parent()->iStatus.Int();
			if (error!=KErrNone)
				{
				iState = EStateOpen; // clear first to allow immediate re-conversion
				iObserver.MiuoConvertComplete(error);
				}
			else
				{
				iState = EStateCopying;
				SelfComplete(KErrNone);
				}
			}
			break;
		case EStateCopying:
			{
			TInt error = KErrNone;
			TRAP(error,DoFinalCopyL()); // assumes can't fail
			iState = EStateOpen; // even if fail, we go to "open"
			iObserver.MiuoConvertComplete(error);
			}
			break;
		case EStateCancelled:
			ASSERT(EFalse); // should not get here!
			break;
		default:
			ASSERT(EFalse);
		}
	}

void CMdaImageDataWriteUtilityPriv::DoCancel()
	{
	switch (iState)
		{
		case EStateClosed:
			ASSERT(EFalse); // should not happen
			break;
		case EStateCreating:
			// ignore - makes no difference to do something here
			break;
		case EStateOpen:
			ASSERT(EFalse); // should not happen
			break;
		case EStateConverting:
			{
			iEncoder->Cancel(); 
			iState = EStateCancelled;
			}
			break;
		case EStateCopying:
			// ignore - makes no difference to do something here
			break;
		default:
			ASSERT(EFalse);
		}
	}

CImageEncoder* CMdaImageDataWriteUtilityPriv::Encoder() const
	{
	return iEncoder;
	}

void CMdaImageDataWriteUtilityPriv::DoClose()
	{
	ASSERT(Parent());
	ASSERT(!Parent()->IsActive()); // Cancel should be called by this point
	if (iEncoder)
		iEncoder->Cancel();
	delete iEncoder; iEncoder = NULL;
	delete iFrameImageData; iFrameImageData=NULL;
	iState=EStateClosed;
	}

void CMdaImageDataWriteUtilityPriv::DoFinalCopyL()
	{
	}

void CMdaImageDataWriteUtilityPriv::CompleteEncoderCreationL()
	{
	ASSERT(iEncoder); // should have been created by now
	delete iFrameImageData; iFrameImageData=NULL;

	if(iFrameData || iImageData)
		{
		iFrameImageData = CFrameImageData::NewL();
		}

	if (iFrameData)
		{
		User::LeaveIfError(iFrameImageData->AppendFrameData(iFrameData));
		iFrameData = NULL;
		}

	if (iImageData)
		{
		User::LeaveIfError(iFrameImageData->AppendImageData(iImageData));
		iImageData = NULL;
		}

	}

//
// CMdaImageFileToBitmapUtilityPriv
//

CMdaImageFileToBitmapUtilityPriv* CMdaImageFileToBitmapUtilityPriv::NewL(MMdaImageUtilObserver& aObserver,
																		 CMdaImageFileToBitmapUtility* aParent)
	{
	CMdaImageFileToBitmapUtilityPriv* self = new(ELeave) CMdaImageFileToBitmapUtilityPriv(aObserver, aParent);
	CleanupStack::PushL(self);
	self->ConstructL();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	self->OpenWaitTimerL();
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
	CleanupStack::Pop(); // self
	return self;
	}

CMdaImageFileToBitmapUtilityPriv::CMdaImageFileToBitmapUtilityPriv(MMdaImageUtilObserver& aObserver,
																   CMdaImageFileToBitmapUtility* aParent):
	CMdaImageDataReadUtilityPriv(aObserver, aParent)
	{
	}

void CMdaImageFileToBitmapUtilityPriv::ConstructL()
	{
	Initialize(); 
	OpenFsSessionL();
	}

CMdaImageFileToBitmapUtilityPriv::~CMdaImageFileToBitmapUtilityPriv()
	{
	delete iFileName;
	}

void CMdaImageFileToBitmapUtilityPriv::OpenL(const TDesC& aFileName,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	Close(EFalse);
	ASSERT(iState == EStateClosed);
	delete iFileName; iFileName = NULL;

	iState = EStateOpening;

	TRAPD(error, iFileName = aFileName.AllocL());
	if (error==KErrNone) 
		TRAP(error,SetUidsFromOrigFormatsL(aFormat, aCodec, aExtra));

	// note handle error on AO to approximate original functionality that did not leave
	SelfComplete(error);
	}

void CMdaImageFileToBitmapUtilityPriv::DoOpenL()
	{
	ASSERT(iDecoder==NULL); // should have been deleted before this
	ASSERT(iFileName); // should already exist
	iDecoder=CImageDecoder::FileNewL(iFs, *iFileName, CImageDecoder::EOptionNone, iImageType, iImageSubType);
	}


//
// CMdaImageDescToBitmapUtilityPriv
//

CMdaImageDescToBitmapUtilityPriv* CMdaImageDescToBitmapUtilityPriv::NewL(MMdaImageUtilObserver& aObserver,
																		 CMdaImageDescToBitmapUtility* aParent)
	{
	CMdaImageDescToBitmapUtilityPriv* self = new(ELeave) CMdaImageDescToBitmapUtilityPriv(aObserver, aParent);
	CleanupStack::PushL(self);
	self->ConstructL();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	self->OpenWaitTimerL();
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
	CleanupStack::Pop(); // self
	return self; 
	}

CMdaImageDescToBitmapUtilityPriv::CMdaImageDescToBitmapUtilityPriv(MMdaImageUtilObserver& aObserver,
																   CMdaImageDescToBitmapUtility* aParent):
	CMdaImageDataReadUtilityPriv(aObserver, aParent)
	{
	}

void CMdaImageDescToBitmapUtilityPriv::OpenL(const TDesC8& aDescriptor,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	Close(EFalse);
	ASSERT(iState == EStateClosed);

	iState = EStateOpening;

	iDescriptor = &aDescriptor;

	TRAPD(error, SetUidsFromOrigFormatsL(aFormat, aCodec, aExtra));

	SelfComplete(error);
	}

void CMdaImageDescToBitmapUtilityPriv::DoOpenL()
	{
	ASSERT(iDecoder==NULL);
	iDecoder=CImageDecoder::DataNewL(iFs, *iDescriptor, CImageDecoder::EOptionNone, iImageType, iImageSubType);
	}

void CMdaImageDescToBitmapUtilityPriv::ConstructL()
	{
	Initialize(); 
	OpenFsSessionL();
	}

//
// CMdaImageBitmapToFileUtilityPriv
//

CMdaImageBitmapToFileUtilityPriv* CMdaImageBitmapToFileUtilityPriv::NewL(MMdaImageUtilObserver& aObserver,CMdaImageBitmapToFileUtility* aParent)
	{
	CMdaImageBitmapToFileUtilityPriv* self = new(ELeave) CMdaImageBitmapToFileUtilityPriv(aObserver, aParent);
	CleanupStack::PushL(self);
	self->ConstructL();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	self->OpenWaitTimerL();
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
	CleanupStack::Pop(); // self
	return self;
	}

CMdaImageBitmapToFileUtilityPriv::CMdaImageBitmapToFileUtilityPriv(MMdaImageUtilObserver& aObserver,
																   CMdaImageBitmapToFileUtility* aParent):
	CMdaImageDataWriteUtilityPriv(aObserver, aParent)
	{
	}

void CMdaImageBitmapToFileUtilityPriv::ConstructL()
	{
	Initialize(); 
	OpenFsSessionL();
	}

CMdaImageBitmapToFileUtilityPriv::~CMdaImageBitmapToFileUtilityPriv()
	{
	delete iFileName;
	}

void CMdaImageBitmapToFileUtilityPriv::CreateL(const TDesC& aFileName,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	Close(EFalse);
	ASSERT(iState == EStateClosed);
	delete iFileName; iFileName = NULL;

	iState = EStateCreating;

	TRAPD(error, iFileName = aFileName.AllocL());

	if (error==KErrNone)
		TRAP(error,SetUidsFromOrigFormatsL(aFormat, aCodec, aExtra));

	SelfComplete (error);
	}

void CMdaImageBitmapToFileUtilityPriv::DoCreateL(TBool aFullCreate)
	{
	ASSERT(iEncoder==NULL); // assumed to have been deleted first
	ASSERT(iFileName); // should be set by this point
	iEncoder = CImageEncoder::FileNewL(iFs, *iFileName, CImageEncoder::EOptionNone, iImageType, iImageSubType);
	if (aFullCreate)
		CompleteEncoderCreationL();
	}

//
// CMdaImageBitmapToDescUtilityPriv
//

CMdaImageBitmapToDescUtilityPriv* CMdaImageBitmapToDescUtilityPriv::NewL(MMdaImageUtilObserver& aObserver,
																		 CMdaImageBitmapToDescUtility* aParent)
	{
	CMdaImageBitmapToDescUtilityPriv* self = new(ELeave) CMdaImageBitmapToDescUtilityPriv(aObserver, aParent);
	self->Initialize();
#if defined(__ECOM_POWERDOWN_WORKAROUND)
	CleanupStack::PushL(self);
	self->OpenWaitTimerL();
	CleanupStack::Pop();
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)
	return self;
	}

CMdaImageBitmapToDescUtilityPriv::CMdaImageBitmapToDescUtilityPriv(MMdaImageUtilObserver& aObserver,
																   CMdaImageBitmapToDescUtility* aParent):
	CMdaImageDataWriteUtilityPriv(aObserver, aParent)
	{
	}

CMdaImageBitmapToDescUtilityPriv::~CMdaImageBitmapToDescUtilityPriv()
	{
	Close(ETrue);
	delete iSecondBuffer;
	}

void CMdaImageBitmapToDescUtilityPriv::CreateL(TDes8& aDescriptor,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	Close(EFalse);
	ASSERT(iState == EStateClosed);

	iState = EStateCreating;

	iDescriptor = &aDescriptor;

	TRAPD(error, SetUidsFromOrigFormatsL(aFormat, aCodec, aExtra));

	SelfComplete (error);
	}

void CMdaImageBitmapToDescUtilityPriv::DoCreateL(TBool aFullCreate)
	{
	ASSERT(iEncoder==NULL);
	delete iSecondBuffer; iSecondBuffer = NULL; // ensure no previous buffer left around
	iEncoder = CImageEncoder::DataNewL(iSecondBuffer, CImageEncoder::EOptionNone, iImageType, iImageSubType);
	if (aFullCreate)
		CompleteEncoderCreationL();
	}

void CMdaImageBitmapToDescUtilityPriv::DoFinalCopyL()
	{
	ASSERT(iSecondBuffer!=NULL); // should have been set during conversion
	TInt error = KErrNone;
	if (iDescriptor->MaxLength() < iSecondBuffer->Length())
		error = KErrArgument; // will leave with this
	else
		*iDescriptor = *iSecondBuffer; // safe to copy buffer
	delete iSecondBuffer; iSecondBuffer = NULL; // save space - throw away the buffer we no longer need
	User::LeaveIfError(error);
	}

// CMdaImageBitmapToBitmapUtilityPriv

CMdaImageBitmapToBitmapUtilityPriv::CMdaImageBitmapToBitmapUtilityPriv(
									MMdaImageUtilObserver& aObserver, 
									CMdaImageBitmapToBitmapUtility* aParent)
	: CMdaImageUtilityPriv(aObserver, aParent)
	{
	}

CMdaImageBitmapToBitmapUtilityPriv* CMdaImageBitmapToBitmapUtilityPriv::NewL(MMdaImageUtilObserver& aObserver, CMdaImageBitmapToBitmapUtility* aParent)
	{
	CMdaImageBitmapToBitmapUtilityPriv* self = new(ELeave) CMdaImageBitmapToBitmapUtilityPriv(aObserver, aParent);
	self->Initialize();
	return self;
	}

CMdaImageBitmapToBitmapUtilityPriv::~CMdaImageBitmapToBitmapUtilityPriv()
	{
	Close();
	delete iBitmapCopier;
	}

void CMdaImageBitmapToBitmapUtilityPriv::OpenL(CFbsBitmap& aBitmap)
	{
	if (iState != EClosed)
		Close();

	if (!iBitmapCopier)
		iBitmapCopier = CBitmapConverter::NewL();

	iSrcBitmap=&aBitmap;

	iState = EOpening;
	SelfComplete(KErrNone);	//& perform callback
	}

void CMdaImageBitmapToBitmapUtilityPriv::ConvertL(CFbsBitmap& aBitmap, TInt aFrameNumber)
	{
	if (iState != EReady)
		User::Leave(KErrNotReady);

	if (aFrameNumber != 0)
		User::Leave(KErrArgument);

	iBitmapCopier->Convert(&Parent()->iStatus, aBitmap, *iSrcBitmap);
	iState = ECopying;
	Parent()->SetActive();
	}

void CMdaImageBitmapToBitmapUtilityPriv::ConvertL(CFbsBitmap& /* aBitmap */, CFbsBitmap& /* aMaskBitmap */, TInt /* aFrameNumber = 0 */)
	{
	User::Leave(KErrNotSupported);
	}

void CMdaImageBitmapToBitmapUtilityPriv::CancelConvertL()
	{
	Parent()->Cancel();
	}

void CMdaImageBitmapToBitmapUtilityPriv::RunL()
	{
	//Called only on completion of OpenL() or ConvertL()
	TInt errorStatus = Parent()->iStatus.Int();	//should always be KErrNone
	
	switch (iState)
		{
	case EOpening:
		iState = EReady;
		iObserver.MiuoOpenComplete(errorStatus);
		return;
	case ECopying:
		iState = EReady;
		iObserver.MiuoConvertComplete(errorStatus);
		return;
	default:
		User::Invariant();
		}
	}

void CMdaImageBitmapToBitmapUtilityPriv::DoCancel()
	{
	switch (iState)
		{
	case EReady:
	case EClosed:
		return;
	case EOpening:
		//Message will have already been completed with KErrNone
		// INC037143. This unnecessary Close() call causes infinite 
		// recursion if the utility is cancelled in the EOpening state.
		//Close();
		return;
	case ECopying:
		//Copier will complete our message with KErrCancel
		iBitmapCopier->Cancel();
		return;
	default:
		User::Invariant();
		}
	}

void CMdaImageBitmapToBitmapUtilityPriv::DoClose()
	{
	if(iState != EClosed)
		{
		iState = EClosed;
		CMdaImageUtilityPriv::DoClose();
		}
	}
