// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <fbs.h>
#include "PPM1Codec.h"
#include "pluginextensionmanager.h"

const TInt KRAWBytesPerPixel = 3;

// CPpmReadCodec

CPpmReadCodec* CPpmReadCodec::NewL(CPpmDecoder& aDecoder)
	{
	CPpmReadCodec* result = new (ELeave) CPpmReadCodec(aDecoder);
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result);
	return result;
	}


CPpmReadCodec::CPpmReadCodec(CPpmDecoder& aDecoder):
	iDecoder(&aDecoder)
	{}

CPpmReadCodec::~CPpmReadCodec()
	{
	}

TFrameState CPpmReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	const TUint8* srcStart = aSrc.Ptr();
	iDataPtr = srcStart;
	iDataPtrLimit = srcStart + aSrc.Length();
	
	TInt bytesUsed = 0;
	if(!iSeekRequested)
		{
		if (iDecoder->Compressed())
			{
			DoProcessCompressed();
			bytesUsed = iDataPtr - srcStart;
			}
		else
			{
			TLex8 lex (aSrc);
			TRAPD(error, DoProcessUncompressedL(lex));
			if (error!=KErrNone && error!=KErrGeneral && error!=KErrUnderflow) // trap for insufficient data
				{
				User::Leave(error);
				}
			bytesUsed = lex.MarkedOffset();
			}

		aSrc.Shift(bytesUsed);
		}

	if (iPixelsProcessed >= iPixelsExpected)
		{
		return EFrameComplete;
		}
	
	if(iSeekRequested)
		{
		/*
		Move the pointer to the end of the buffer.  This is to 
		workaround a problem with the ICL framework, where if we
		choose to seek and haven't incremented the pointer, then
		it returns KErrUnderflow.  Thus, the first seek (when 
		cropping) will fail.
		*/
		TInt bytesRemainingInBuffer = Min(iNewBytePosition, iDataPtrLimit - iDataPtr);
		aSrc.Shift(bytesRemainingInBuffer);
		iNewBytePosition -= bytesRemainingInBuffer;
		iSeekRequested = EFalse;
		return EFrameIncompleteRepositionRequest;
		}
		
	return EFrameIncomplete;
	}

void CPpmReadCodec::Complete()
	{
	ImageProcessor()->FlushPixels(); 
	}

void CPpmReadCodec::DoProcessUncompressedL(TLex8 &aLex)
	{
	CImageProcessor*const imageProc = ImageProcessor();
	for (;;) // exit of this will be a leave
		{
		TUint rVal;
		iDecoder->SkipCommentAndWhiteSpaceL(aLex);
		ASSERT(!iDecoder->InComment());
		User::LeaveIfError(aLex.Val(rVal));
		TUint gVal;
		iDecoder->SkipCommentAndWhiteSpaceL(aLex);
		ASSERT(!iDecoder->InComment());
		User::LeaveIfError(aLex.Val(gVal));
		TUint bVal;
		iDecoder->SkipCommentAndWhiteSpaceL(aLex);
		ASSERT(!iDecoder->InComment());
		User::LeaveIfError(aLex.Val(bVal));

		TInt dataShift = iDecoder->DataShift();
		TRgb rgb (rVal<<dataShift, gVal<<dataShift, bVal<<dataShift);

		imageProc->SetPixel(rgb);

		aLex.Mark(); // show we've got this far - on error, only wind back this far.

		iPixelsProcessed += 1;
		}
	}

void CPpmReadCodec::DoProcessCompressed()
	{
	CImageProcessor*const imageProc = ImageProcessor();
	
	while (iDataPtr < iDataPtrLimit-2)
		{
		TUint8 rVal = iDataPtr[0];
		TUint8 gVal = iDataPtr[1];
		TUint8 bVal = iDataPtr[2];
		iDataPtr += KRAWBytesPerPixel;
		
		TInt dataShift = iDecoder->DataShift();
		TRgb rgb (rVal<<dataShift, gVal<<dataShift, bVal<<dataShift);

		imageProc->SetPos(iCurrentPosition);
		imageProc->SetPixel(rgb);
		iPixelsProcessed += 1;
		
		if(iXAxisFirst)
			{
			iCurrentPosition.iX += iXAxisIncrement;
			if(iCurrentPosition.iX == iEndPosition.iX)
				{
				iCurrentPosition.iX = iStartPosition.iX;
				iCurrentPosition.iY += iYAxisIncrement;
				if(iExtensionManager->ClippingRectExtensionRequested())
					{
					// Clipping is required, so need to skip position in buffer
					iSeekRequested = ETrue;
					iNewBytePosition = iFrameInfo->iOverallSizeInPixels.iWidth - iExtensionManager->ClippingRect().Size().iWidth;
					iNewBytePosition *= KRAWBytesPerPixel;
					break;
					}
				}
			}
		else
			{
			iCurrentPosition.iY += iYAxisIncrement;
			if(iCurrentPosition.iY == iEndPosition.iY)
				{
				iCurrentPosition.iY = iStartPosition.iY;
				iCurrentPosition.iX += iXAxisIncrement;
				if(iExtensionManager->ClippingRectExtensionRequested())
					{
					// Clipping is required, so need to skip position in buffer
					iSeekRequested = ETrue;
					iNewBytePosition = iFrameInfo->iOverallSizeInPixels.iWidth - iExtensionManager->ClippingRect().Size().iWidth;
					iNewBytePosition *= KRAWBytesPerPixel;
					break;
					}
				}
			}
		}
	}

void CPpmReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* /*aDestinationMask*/)
	{
	ASSERT(iFrameInfo);
	ASSERT(iExtensionManager);
	
	/*
	This function is used by both compressed and uncompressed format, although some of the
	calls aren't stricly necessary for the uncompressed format (since extensions are not
	supported, except for the GetDestinationSize() call).
	*/
	
	User::LeaveIfError(ValidateDestinationSize(aFrame.SizeInPixels()));
	
	TSize finalSize = iExtensionManager->ClippingRectExtensionRequested() ?
					  iExtensionManager->ClippingRect().Size() : iFrameInfo->iOverallSizeInPixels;
	if(iExtensionManager->DimensionsSwapped())
		{
		finalSize.SetSize(finalSize.iHeight, finalSize.iWidth);
		}
	
	TInt scalingCoefficient = ScalingCoefficientL(finalSize, aFrame.SizeInPixels());
	TInt reductionFactor = CPluginExtensionManager::ConvertScalingCoeffToReductionFactor(scalingCoefficient);
	
	CImageProcessorExtension* imageProc = 
		ImageProcessorUtility::NewImageProcessorExtensionL(aFrame, reductionFactor, iFrameInfo->iFrameDisplayMode, aDisableErrorDiffusion);
	SetImageProcessor(imageProc);

	iExtensionManager->TransferExtensionDataL(imageProc);
	// Pass in full size of image or clip rect
	imageProc->PrepareL(aFrame, finalSize);
	
	// Set up member variables if Operations have been requested
	SetOperationData(finalSize);

	ClearBitmapL(aFrame, KRgbWhite);
		// do something sensible for partial streamed decodes
		
	iPixelsProcessed = 0;
	iPixelsExpected = finalSize.iWidth * finalSize.iHeight;

	// Determine how many pixels to skip on the Y axis of the original, if any
	TInt pixelsToSkip = iFrameInfo->iFrameCoordsInPixels.iBr.iX * iExtensionManager->ClippingRect().iTl.iY;
	// Add X axis pixels on the left side of the crop rect, if any
	pixelsToSkip += iExtensionManager->ClippingRect().iTl.iX - iFrameInfo->iFrameCoordsInPixels.iTl.iX;

	iNewBytePosition = pixelsToSkip*KRAWBytesPerPixel;
	iSeekRequested = iNewBytePosition > 0 ? ETrue : EFalse;	
	
	iDecoder->SetInComment(EFalse);
	}
	
void CPpmReadCodec::InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameData*/)
	{
	ASSERT(aFrameInfo.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameInfo;
	aFrameInfo.SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CPpmReadCodec::ProcessFrameHeaderL(TBufPtr8&/* aData*/)
	{
	ASSERT(iFrameInfo);
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
	return EFrameComplete;
	}

void CPpmReadCodec::GetNewDataPosition(TInt &aPosition, TInt &/*aLength*/)
	{
	aPosition += iNewBytePosition;
	}

void CPpmReadCodec::SetExtensionManager(CPluginExtensionManager* aExtensionManager)
	{
	// Is owned by the plugin, not this codec
	iExtensionManager = aExtensionManager;
	}

TInt CPpmReadCodec::ValidateDestinationSize(const TSize& aDestSize) const
	{
	ASSERT(iExtensionManager);
	
	TInt err = KErrNone;
	if(iExtensionManager->ScalerExtensionRequested())
		{
		TSize destSizeCheck = iFrameInfo->iOverallSizeInPixels;
		err = iExtensionManager->GetDestinationSize(destSizeCheck);
		if(err == KErrNone)
			{
			// Mandatory check that the destination size matches the calculated size
			if(destSizeCheck != aDestSize)
				{
				err = KErrArgument;
				}	
			}
		}
	return err;
	}

TInt CPpmReadCodec::ScalingCoefficientL(const TSize& aDestinationSize, const TSize& aFrameSize) const
	{
	ASSERT(iExtensionManager);
	
	TInt scalingCoefficient = -1; // initialise to full size
	if(iExtensionManager->ScalerExtensionRequested())
		{
		// Explicit scaling
		User::LeaveIfError(iExtensionManager->GetScalingCoefficient(scalingCoefficient, &aDestinationSize));
		}
	else
		{
		// Implicit scaling
		scalingCoefficient = ScalingCoefficient(aDestinationSize, aFrameSize);
		}
	return scalingCoefficient;
	}

void CPpmReadCodec::SetOperationData(const TRect& aDestinationBitmap)
	{
	ASSERT(iExtensionManager);
	ASSERT(!aDestinationBitmap.IsEmpty());
	ASSERT(aDestinationBitmap.IsNormalized());

	/*
	Note that iStartPosition is the starting point and so needs to
	be INSIDE the TRect, whereas iEndPosition is the end point and thus
	lies diagonally opposite the start point, and OUTSIDE the TRect.
	
		-----------------
		|tlo|   |   |tro
		-----------------
		|   |tli|tri|   |
		-----------------
		|   |bli|bri|   |
		-----------------
		|blo|   |   |bro|
		-----------------
		
		tlo = top left outside image
		tro = top right outside image
		blo = bottom left outside image
		bro = bottom right outside image (i.e. iBr)
		
		tli = top left inside image (i.e. iTl)
		tri = top right inside image
		bli = bottom left inside image
		bri = bottom right inside image
	*/
	TPoint tlOutsideImageRegion(aDestinationBitmap.iTl.iX - 1, aDestinationBitmap.iTl.iY - 1);
	TPoint trOutsideImageRegion(aDestinationBitmap.iBr.iX, aDestinationBitmap.iTl.iY - 1);
	TPoint blOutsideImageRegion(aDestinationBitmap.iTl.iX - 1, aDestinationBitmap.iBr.iY);
	TPoint brOutsideImageRegion(aDestinationBitmap.iBr.iX, aDestinationBitmap.iBr.iY);	
	TPoint tlInsideImageRegion(aDestinationBitmap.iTl.iX, aDestinationBitmap.iTl.iY);
	TPoint trInsideImageRegion(aDestinationBitmap.iBr.iX - 1, aDestinationBitmap.iTl.iY);
	TPoint blInsideImageRegion(aDestinationBitmap.iTl.iX, aDestinationBitmap.iBr.iY - 1);
	TPoint brInsideImageRegion(aDestinationBitmap.iBr.iX - 1, aDestinationBitmap.iBr.iY - 1);

	switch(iExtensionManager->Operation())
		{
		case EDecodeNormal:
			iStartPosition = tlInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = brOutsideImageRegion;
			iXAxisIncrement = 1;
			iYAxisIncrement = 1;
			iXAxisFirst = ETrue;
			break;
		case EDecodeRotate90:
			iStartPosition = trInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = blOutsideImageRegion;
			iXAxisIncrement = -1;
			iYAxisIncrement = 1;
			iXAxisFirst = EFalse;
			break;
		case EDecodeRotate180:
			iStartPosition = brInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = tlOutsideImageRegion;
			iXAxisIncrement = -1;
			iYAxisIncrement = -1;
			iXAxisFirst = ETrue;
			break;
		case EDecodeRotate270:
			iStartPosition = blInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = trOutsideImageRegion;
			iXAxisIncrement = 1;
			iYAxisIncrement = -1;
			iXAxisFirst = EFalse;
			break;
		case EDecodeHorizontalFlip:
			iStartPosition = blInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = trOutsideImageRegion;
			iXAxisIncrement = 1;
			iYAxisIncrement = -1;
			iXAxisFirst = ETrue;
			break;
		case EDecodeHorizontalFlipRotate90:
			iStartPosition = tlInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = brOutsideImageRegion;
			iXAxisIncrement = 1;
			iYAxisIncrement = 1;
			iXAxisFirst = EFalse;
			break;
		case EDecodeVerticalFlip:
			iStartPosition = trInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = blOutsideImageRegion;
			iXAxisIncrement = -1;
			iYAxisIncrement = 1;
			iXAxisFirst = ETrue;
			break;
		case EDecodeVerticalFlipRotate90:
			iStartPosition = brInsideImageRegion;
			iCurrentPosition = iStartPosition;
			iEndPosition = tlOutsideImageRegion;
			iXAxisIncrement = -1;
			iYAxisIncrement = -1;
			iXAxisFirst = EFalse;
			break;
		default:
			{
			ASSERT(EFalse);
			}
		}
	}

// CPpm1WriteCodec

const TInt KMaxColourValue = 255;

CPpm1WriteCodec::CPpm1WriteCodec()
	{}

CPpm1WriteCodec::~CPpm1WriteCodec()
	{}

void CPpm1WriteCodec::ConstructL()
	{
	CImageWriteCodec::ConstructL();
	}

CPpm1WriteCodec* CPpm1WriteCodec::NewL()
	{
	CPpm1WriteCodec* result = new (ELeave) CPpm1WriteCodec;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result);
	return result;
	}

void CPpm1WriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource)
	{
	SetSource(&aSource);
	iDestStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = iDestStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	TSize size = aSource.SizeInPixels();
	iSourceRect = TRect(size);
	iPos.SetXY(0,0);

	TBuf8<256> header; // set up header in buffer
	header.Zero();

	// now the standard header lines
	_LIT8(KMagicHeader, "P6\n");
	header.AppendFormat(KMagicHeader); // ppm magic for compressed
	_LIT8(KSizeHeader, "%d %d\n");
	header.AppendFormat(KSizeHeader, size.iWidth, size.iHeight);
	_LIT8(KMaxValueHeader, "%d\n");
	header.AppendFormat(KMaxValueHeader, KMaxColourValue);

	TInt headerSize = header.Length();
	aDst.Copy(header);
	iDestPtr+=headerSize;

	ASSERT(iDestPtr < iDestPtrLimit); // should always be true
	}

TFrameState CPpm1WriteCodec::ProcessFrameL(TBufPtr8& aDst)
	{
	//Setup buffer to use all available space
	TUint8* destStartPtr = CONST_CAST(TUint8*,aDst.Ptr());
	iDestPtr = destStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	DoProcessL(*Source());

	aDst.SetLength(iDestPtr - destStartPtr);

	// If processed all pixels
	if (iPos.iY < iSourceRect.iTl.iY)
		{
		return EFrameComplete;
		}
		
	return EFrameIncomplete;
	}

void CPpm1WriteCodec::DoProcessL(const CFbsBitmap& aFrame)
	{
	TUint8* safeDestPtrLimit = iDestPtrLimit - 2;

	while (iDestPtr < safeDestPtrLimit)
		{
		if (iPos.iY >= iSourceRect.iBr.iY)
			{
			break;
			}
			
		TInt scanLength = Min(iSourceRect.iBr.iX - iPos.iX, (iDestPtrLimit - iDestPtr) / 3);
		TInt dstLength = scanLength * 3;
		TPtr8 dstBuf(iDestPtr, dstLength, dstLength);

		aFrame.GetScanLine(dstBuf, iPos, scanLength, EColor16M);
		// this comes out in order BGR, so switch
		SwitchRGB(dstBuf);

		iPos.iX += scanLength;
		iDestPtr += dstLength;

		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY++;
			}
		}
	}

// Assume original has pixels with BGR in order - switch to RGB
void CPpm1WriteCodec::SwitchRGB(TDes8 &aBuf)
	{
	TUint8* buf = const_cast<TUint8*>(aBuf.Ptr());
	TUint8* bufMax = buf + aBuf.Length();

	while (buf<bufMax)
		{
		TUint8 b = buf[0];
		TUint8 g = buf[1];
		TUint8 r = buf[2];
		buf[0] = r;
		buf[1] = g;
		buf[2] = b;

		buf += 3; 
		}
	}


