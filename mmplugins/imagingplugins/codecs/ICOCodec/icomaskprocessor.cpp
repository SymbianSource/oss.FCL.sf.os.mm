// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "icomaskprocessor.h"

const TInt 	KIcoMaskProcessorBufferSize = 1024; // size matches that of the CPixelWriter buffer
const TUint KRGBMask = 0x00FFFFFF;
const TInt	KAlphaShift = 24;

CIcoMaskProcessor* CIcoMaskProcessor::NewL(TInt aReductionFactor)
	{
	return new (ELeave) CIcoMaskProcessor(aReductionFactor);
	}

CIcoMaskProcessor::CIcoMaskProcessor(TInt aReductionFactor)
:iReductionFactor(aReductionFactor)
	{
	}

CIcoMaskProcessor::~CIcoMaskProcessor()
	{
	Reset();
	}

void CIcoMaskProcessor::Reset()
	{
	iReductionFactor = 0;
	iImageRegion.SetRect(0,0,0,0);
	iPosition.SetXY(0,0);
	iScaledPosition.SetXY(0,0);
	iYIncrement = 0;
	iPixelPadding = 0;
	iPixelsToSkip = 0;
	delete[] iMaskBuffer;
	iMaskBuffer = NULL;
	delete[] iScaledMaskBuffer;
	iScaledMaskBuffer = NULL;
	}

void CIcoMaskProcessor::PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect)
	{
	ASSERT(aImageRect.Width() != 0);
	ASSERT(aImageRect.Height() != 0);
	ASSERT(iMaskBuffer == NULL);
	
	iDestBitmap = &aBitmap;
	iImageRegion = aImageRect;
	
	if(iReductionFactor)
		{
		// Create the buffer to hold the unscaled pixels.
		iMaskBuffer = new (ELeave) TRgb[KIcoMaskProcessorBufferSize >> iReductionFactor];
		iMaskBufferPtr = iMaskBuffer;
		iMaskBufferPtrLimit = iMaskBuffer + (KIcoMaskProcessorBufferSize >> iReductionFactor);
		
		// Create the buffer to hold the 'merged' pixel values, represented by TMonochromeSum.
		TInt scaledBufferSize = (iImageRegion.iBr.iX + (1<<iReductionFactor) -1 ) >> iReductionFactor;
		iScaledMaskBuffer = new (ELeave) TMonochromeSum[scaledBufferSize];
		iScaledMaskBufferPtrLimit = iScaledMaskBuffer + scaledBufferSize;
		Mem::FillZ(iScaledMaskBuffer, scaledBufferSize * sizeof(TMonochromeSum));
		}
	else
		{
		// Create the buffer to hold the unscaled pixels.
		iMaskBuffer = new (ELeave) TRgb[KIcoMaskProcessorBufferSize];
		iMaskBufferPtr = iMaskBuffer;
		iMaskBufferPtrLimit = iMaskBuffer + KIcoMaskProcessorBufferSize;
		}
	}

void CIcoMaskProcessor::PrepareL(CFbsBitmap& /*aBitmap*/, const TRect& /*aImageRect*/, const TSize& /*aRgbBlockSize*/)
	{
	ASSERT(EFalse);
	}

// ICO data is stored from bottom-up, so iYIncrement is negative (draw bottom-up).
void CIcoMaskProcessor::SetYPosIncrement(TInt aYInc)
	{
	iYIncrement = aYInc;
	}

void CIcoMaskProcessor::SetLineRepeat(TInt /*aLineRepeat*/)
	{
	ASSERT(EFalse);
	}

/*
ICO data contains padding.  Each mask pixel is represented by 1 bit, thus there
can be up to 31 bits of padding in the decoded data buffer to skip.
*/
void CIcoMaskProcessor::SetPixelPadding(TInt aNumberOfPixels)
	{
	iPixelPadding = aNumberOfPixels;
	}

/*
Function to handle a mask pixel, and thus is either fully opauq white (0xFFFFFFFF) or fully
transparent black (0x00000000).
*/
TBool CIcoMaskProcessor::SetPixel(TRgb aColor)
	{
	if(iReductionFactor)
		{
		if(iScaledPosition.iX < iImageRegion.Width()) // Ignore decoded padding
			{
			// When scaling by half, for example, take every other pixel on the first two lines and
			// sum their values.  This is then processed later into a single pixel value.
			TMonochromeSum* sumPtr = iScaledMaskBuffer + (iScaledPosition.iX >> iReductionFactor);
			sumPtr->iLevel += TColorConvertor::RgbToMonochrome(aColor);
			sumPtr->iCount++;
			}
		
		iScaledPosition.iX++;
				
		if(iScaledPosition.iX == iImageRegion.Width() + iPixelPadding)
			{
			TInt newY = iScaledPosition.iY + iYIncrement;
			TBool outsideOfBuffer = ((newY ^ iScaledPosition.iY) >> iReductionFactor) != 0;
			
			iScaledPosition.iY = newY;
			iScaledPosition.iX = 0;
			
			if(outsideOfBuffer)
				{
				return FlushPixels();
				}
			}
		}
	else
		{
		*iMaskBufferPtr++ = aColor;
		if(iMaskBufferPtr == iMaskBufferPtrLimit)
			{		
			return FlushPixels();
			}
		}

	return EFalse;
	}

TBool CIcoMaskProcessor::SetPixelRun(TRgb /*aColor*/, TInt /*aCount*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetPixels(TRgb* /*aColorBuffer*/, TInt /*aBufferLength*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetPixelBlock(TRgb* /*aColorBuffer*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetMonoPixel(TInt /*aGray256*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetMonoPixelRun(TInt /*aGray256*/, TInt /*aCount*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetMonoPixels(TUint32* /*aGray256Buffer*/, TInt /*aBufferLength*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetMonoPixelBlock(TUint32* /*aGray256Buffer*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CIcoMaskProcessor::SetPos(const TPoint& aPosition)
	{
	iPosition.SetXY(aPosition.iX >> iReductionFactor, aPosition.iY >> iReductionFactor);
	iScaledPosition = aPosition;
	return ETrue;
	}
/*
Converts the pixel values in the internally held iScaledMaskBuffer into a single pixel value which is then
added to the iMaskBuffer.
*/
void CIcoMaskProcessor::ProcessScaledPixels()
	{
	TMonochromeSum* scaledSumPtr = iScaledMaskBuffer;
	TMonochromeSum* scaledSumPtrLimit = iScaledMaskBuffer + (iImageRegion.Width() >> iReductionFactor);
	TRgb* maskBufferPtr = iMaskBuffer;
	
	TInt fullCountFactor = 2 * iReductionFactor;
	TInt fullCount = 1 << fullCountFactor;

	do
		{		
		TInt level = scaledSumPtr->iLevel;
		TInt count = scaledSumPtr->iCount;

		if(count == fullCount)
			{
			level >>= fullCountFactor;
			}
		else if(count!=0)
			{
			level /= count;
			}
		else
			break;

		maskBufferPtr->SetInternal(level);
		maskBufferPtr++;

		scaledSumPtr++;
		}
	while(scaledSumPtr < scaledSumPtrLimit);
	
	iMaskBufferPtr = maskBufferPtr;
	
	TInt numPixels = scaledSumPtr - iScaledMaskBuffer;
	Mem::FillZ(iScaledMaskBuffer, numPixels * sizeof(TMonochromeSum));
	}

TBool CIcoMaskProcessor::FlushPixels()
	{
	if(iReductionFactor)
		{
		ProcessScaledPixels();
		}
	
	return DoFlushPixels();
	}

/*
The pixelvalues (i.e. mask values) in the iMaskBuffer are added to the RGB colour values held in the
destination bitmap (which have been put there by another image processor), and then put back into the
destination bitmap.
*/
TBool CIcoMaskProcessor::DoFlushPixels()
	{
	TRgb* maskBufferPtrLimit = iMaskBufferPtr;
	iMaskBufferPtr = iMaskBuffer; // reset pointer
	
	TBitmapUtil bitmapUtil(iDestBitmap);
	bitmapUtil.Begin(iPosition);
	
	for(TRgb* maskBufferPtr = iMaskBuffer; maskBufferPtr < maskBufferPtrLimit; maskBufferPtr++)
		{		
		TInt skip = Min(iPixelsToSkip, maskBufferPtrLimit - maskBufferPtr);
		maskBufferPtr += skip;
		iPixelsToSkip -= skip;
		if(iPixelsToSkip > 0 || maskBufferPtr == maskBufferPtrLimit)
			{
			break;
			}
				
		// Set the alpha channel of the bitmap
		TUint32 pixel = bitmapUtil.GetPixel();
		pixel &= KRGBMask; // clear alpha channel
		pixel |= maskBufferPtr->Internal() << KAlphaShift; // set alpha

		bitmapUtil.SetPixel(pixel);
		iPosition.iX++;
		
		if(iPosition.iX >= (iImageRegion.iBr.iX >> iReductionFactor))
			{
			iPosition.iX = 0;
			iPosition.iY += iYIncrement;
			bitmapUtil.SetPos(iPosition);
			
			if(iPosition.iY < 0)
				{
				return ETrue;
				}
			
			iPixelsToSkip = (iReductionFactor ? 0 : iPixelPadding);
			}
		else
			{
			bitmapUtil.IncXPos();
			}
		}
	
	bitmapUtil.End();
	
	return EFalse;
	}
