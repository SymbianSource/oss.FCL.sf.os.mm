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
#include "ICOCodec.h"
#include "icomaskprocessor.h"


// Constants.
const TInt KIcoInfoHeaderSize = 40;
const TInt KRgbQuadSize = 4;
const TInt KFullyOpaque = 0x7FFFFFFF;
const TInt KFullyTransparent = 0x00000000;
const TUint KBlack = 0xFF000000;
const TUint KWhite = 0xFFFFFFFF;


// CIcoReadCodec
CIcoReadCodec::CIcoReadCodec(TInt aIconHeadersToProcess)
	: iIconHeadersToProcess(aIconHeadersToProcess)
	{}

CIcoReadCodec::~CIcoReadCodec()
	{
	}

CIcoReadCodec* CIcoReadCodec::NewL(TInt aIconHeadersToProcess)
{
	CIcoReadCodec* self = new(ELeave) CIcoReadCodec(aIconHeadersToProcess);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

void CIcoReadCodec::InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& /* aFrameImageData */)
	{
	ASSERT(aFrameSettings.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo= &aFrameSettings;
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CIcoReadCodec::ProcessFrameHeaderL(TBufPtr8& aData)
	{

 	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrame)
		{
		if(aData.Length()>=iImageDataBytes)
			{
			aData.Shift(iImageDataBytes);
			iImageDataBytes = 0;

			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
			if ((--iIconHeadersToProcess) == 0)
				return EFrameComplete;
			}
		else
			{
			iImageDataBytes -= aData.Length();
			aData.Shift(aData.Length()); 
			}
		}

	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrameHeader)
		{ // If processing of previous frame info complete or if its the first frame.
		if (aData.Length() < KIcoInfoHeaderSize)
			User::Leave(KErrUnderflow);

		const TUint8* data = STATIC_CAST(const TUint8*, aData.Ptr());
				
		const TInt width = data[4] + (data[5] << 8) + (data[6] << 16) + (data[7] << 24);
		const TInt height = (data[8] + (data[9] << 8) + (data[10] << 16) + (data[11] << 24))/2;
				
		if(width <= 0 || height <= 0)
			User::Leave(KErrCorrupt);		

		iFrameInfo->iBitsPerPixel = data[14] + (data[15] << 8);
		const TInt colors = 1 << iFrameInfo->iBitsPerPixel;

		// Calculate image size. (Pixels)
		iFrameInfo->iOverallSizeInPixels.SetSize(width, height);

		// Calculate image size. (Bytes)
		// XOR image.
		iImageDataBytes = (width*iFrameInfo->iBitsPerPixel + 31)/32;
		
		if(iImageDataBytes <= 0) //overflow
			User::Leave(KErrCorrupt);		

		// AND image.
		iImageDataBytes += (width + 31)/32;
		
		if(iImageDataBytes <= 0) //overflow
			User::Leave(KErrCorrupt);		
		
		iImageDataBytes *= 4*height;
		
		if(iImageDataBytes <= 0) //overflow
			User::Leave(KErrCorrupt);		

		// Include the palette as well.
		iImageDataBytes += colors*KRgbQuadSize;
		
		if(iImageDataBytes <= 0) //overflow
			User::Leave(KErrCorrupt);		

		iFrameInfo->iFrameCoordsInPixels.SetRect(TPoint(0,0), iFrameInfo->iOverallSizeInPixels);
		iFrameInfo->iFrameSizeInTwips.SetSize(0, 0);
		iFrameInfo->iDelay = 0;
		iFrameInfo->iFlags |= TFrameInfo::ETransparencyPossible | TFrameInfo::EPartialDecodeInvalid;
		
		if (iFrameInfo->iFrameCoordsInPixels.iBr.iX < 0 || iFrameInfo->iFrameCoordsInPixels.iBr.iY < 0 ||
			!(iFrameInfo->iBitsPerPixel == 1 || iFrameInfo->iBitsPerPixel == 4 ||
			iFrameInfo->iBitsPerPixel == 8))
			User::Leave(KErrCorrupt);

		if (iFrameInfo->iBitsPerPixel > 1)
			iFrameInfo->iFlags |= TFrameInfo::EColor;

		switch (iFrameInfo->iBitsPerPixel)
			{
			case 1:
				iFrameInfo->iFrameDisplayMode = EGray2;
				break;

			case 4:
				iFrameInfo->iFrameDisplayMode = EColor16;
				iFrameInfo->iFlags |= TFrameInfo::EColor;
				break;

			case 8:
				iFrameInfo->iFrameDisplayMode = EColor256;
				iFrameInfo->iFlags |= TFrameInfo::EColor;
				break;
			}

		aData.Shift(KIcoInfoHeaderSize);
		TInt frameDataOffset = iFrameInfo->FrameDataOffset();
		iFrameInfo->SetFrameDataOffset(frameDataOffset + KIcoInfoHeaderSize);

		iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame);
		}

	return EFrameIncomplete;
	}

TFrameState CIcoReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{	
	if (iNewFrame)
		{
		// Extract the palette.
		TInt colors = 1 << iFrameInfo->iBitsPerPixel;

		if (aSrc.Length()<(colors*KRgbQuadSize))
			return EFrameIncomplete;

		const TUint8* aSrcPtr = aSrc.Ptr();
		TInt baseIndex;
		for (TInt paletteIndex = 0; paletteIndex < colors; paletteIndex++)
			{
			baseIndex = KRgbQuadSize*paletteIndex;
			iPalette[paletteIndex] = TRgb(aSrcPtr[baseIndex+2],aSrcPtr[baseIndex+1],aSrcPtr[baseIndex]);
			}

		aSrc.Shift(colors*KRgbQuadSize);

		iNewFrame = EFalse;
		}

	if (aSrc.Length() == 0)
		return EFrameIncomplete;

	TUint8* srcStart = CONST_CAST(TUint8*,aSrc.Ptr());
	iDataPtr = srcStart;
	iDataPtrLimit = srcStart + aSrc.Length();

	DoProcessL();

	TInt bytesUsed = iDataPtr - srcStart;
	aSrc.Shift(bytesUsed);

	ASSERT(iBmpBytesExpected>=0); // not clear if value can be negative - if it does occur, need to think about it

	if (iBmpBytesExpected > 0)
		{
		const TInt bmpBytesUsed = Min(bytesUsed, iBmpBytesExpected);
		iBmpBytesExpected -= bmpBytesUsed;
		bytesUsed -= bmpBytesUsed;
		}

	CImageProcessor*const imageProc = ImageProcessor();
	CImageProcessor*const maskProc = MaskProcessor();

	if (bytesUsed > 0 && maskProc)
		{
		const TInt maskBytesUsed = Min(bytesUsed, iMaskBytesExpected);
		iMaskBytesExpected -= maskBytesUsed;
		}

	if ((!maskProc && iBmpBytesExpected <= 0) || (maskProc && iMaskBytesExpected <= 0))
		{
		imageProc->FlushPixels();
		if (maskProc)
			maskProc->FlushPixels();

		iNewFrame = ETrue;
		Pos().SetXY(0,0);
		return EFrameComplete;
		}

	return EFrameIncomplete;
	}

void CIcoReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask)
	{

	iFrameInfo= &aFrameInfo;
	CFbsBitmap& newFrame = aDestination;

	TSize& originalSize = iFrameInfo->iOverallSizeInPixels;
	Pos().iY = originalSize.iHeight - 1;

	const TDisplayMode dispMode = (iFrameInfo->iBitsPerPixel == 1) ? EGray2 : EColor256;
	const TSize destinationSize(aDestination.SizeInPixels());
	
	TInt reductionFactor = ReductionFactor(originalSize, destinationSize);
	CImageProcessor* imageProc = ImageProcessorUtility::NewImageProcessorL(aDestination, reductionFactor, dispMode, aDisableErrorDiffusion);
	SetImageProcessor(imageProc);
	
	imageProc->PrepareL(newFrame, originalSize);
	imageProc->SetPos(TPoint(0, originalSize.iHeight - 1));
	imageProc->SetYPosIncrement(-1);

	CImageProcessor* maskProc = NULL;
	SetMaskProcessor(NULL);

	if (aDestinationMask)
		{
		CFbsBitmap& newMaskFrame = *aDestinationMask;
		maskProc = ImageProcessorUtility::NewImageProcessorL(newMaskFrame, originalSize, EGray2, aDisableErrorDiffusion);
		SetMaskProcessor(maskProc);
		maskProc->PrepareL(newMaskFrame, originalSize);
		}
	else if (newFrame.DisplayMode() == EColor16MA)
		{
		// 16MA dest and no mask - special case
		maskProc = CIcoMaskProcessor::NewL(reductionFactor);
		SetMaskProcessor(maskProc);
		maskProc->PrepareL(newFrame, originalSize); // uses destination bitmap
		}
	
	if(maskProc)
		{
		maskProc->SetPos(TPoint(0,originalSize.iHeight - 1));
		maskProc->SetYPosIncrement(-1);
		const TInt maskWidth = (originalSize.iWidth + 31) & ~31; // round up to 32-byte boundary
		maskProc->SetPixelPadding(maskWidth - originalSize.iWidth);
		iMaskBytesExpected = (maskWidth * originalSize.iHeight) / 8;
		}

	const TInt actualWidth = (((originalSize.iWidth * iFrameInfo->iBitsPerPixel) + 31) & ~31) / iFrameInfo->iBitsPerPixel; // round up to 32-byte boundary
	imageProc->SetPixelPadding(actualWidth - originalSize.iWidth);

	iBmpBytesExpected = (actualWidth * originalSize.iHeight * iFrameInfo->iBitsPerPixel) / 8; // bits are packed into bytes

	iNewFrame =ETrue;
	}
	
void CIcoReadCodec::DoProcessL()
	{
	const TUint8* bmpDataPtrLimit = Min(iDataPtrLimit, iDataPtr + iBmpBytesExpected);
	CImageProcessor*const imageProc = ImageProcessor();
	switch (iFrameInfo->iBitsPerPixel)
		{
	case 1:
		while (iDataPtr < bmpDataPtrLimit)
			{
			TUint8 value = *iDataPtr++;

			// code will compare value with successive masks 0x80, 0x80 etc downto but not including 
			for (TUint mask=0x80; mask!=0; mask >>= 1)
				imageProc->SetPixel((value & mask) ? iPalette[1] : iPalette[0]);
			}
		break;
	case 4:
		while (iDataPtr < bmpDataPtrLimit)
			{
			TUint8 value = *iDataPtr++;

			imageProc->SetPixel(iPalette[value >> 4]);
			imageProc->SetPixel(iPalette[value & 0x0f]);
			}
		break;
	case 8:
		while (iDataPtr < bmpDataPtrLimit)
			imageProc->SetPixel(iPalette[*iDataPtr++]);
		break;
	default:
		User::Leave(KErrCorrupt);
		break;
		}

	CImageProcessor*const maskProc = MaskProcessor();
	if (maskProc)
		{
		// force a flush
		imageProc->FlushPixels();
						
		const TUint8* maskDataPtrLimit = Min(iDataPtrLimit, iDataPtr + iMaskBytesExpected);

		while (iDataPtr < maskDataPtrLimit)
			{
			TUint8 value = *iDataPtr++;

			// code will compare value with successive masks 0x80, 0x80 etc downto but not including 
			for (TUint mask=0x80; mask!=0; mask >>= 1)
				{
				maskProc->SetPixel((value & mask) ? TRgb(KBlack, KFullyTransparent) : TRgb(KWhite, KFullyOpaque));				
				}
			}
		}
	}
