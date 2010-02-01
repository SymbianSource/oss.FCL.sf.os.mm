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

#include "rawcolorprocessor.h"
#include "fwextconstants.h"

/*static*/
CRawColorProcessor* CRawColorProcessor::NewL()
	{
	return new (ELeave) CRawColorProcessor();
	}

CRawColorProcessor::CRawColorProcessor()
	{
	}

//  from the CImageProcessor //

TBool CRawColorProcessor::SetPixels(TRgb* /*aColorBuffer*/,TInt /*aBufferLength*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::SetMonoPixel(TInt /*aGray256*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::SetMonoPixelRun(TInt /*aGray256*/,TInt /*aCount*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::SetMonoPixels(TUint32* /*aGray256Buffer*/,TInt /*aBufferLength*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::SetMonoPixelBlock(TUint32* /*aGray256Buffer*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

void CRawColorProcessor::PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& /*aImageRect*/)
	{
	ASSERT(EFalse);
	}

void CRawColorProcessor::SetLineRepeat(TInt /*aLineRepeat*/)
	{
	ASSERT(EFalse);
	}

TBool CRawColorProcessor::SetPixelRun(TRgb /*aColor*/,TInt /*aCount*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::SetPixel(TRgb /*aColor*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::SetPos(const TPoint& /*aPosition*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CRawColorProcessor::FlushPixels()
	{
	ASSERT(EFalse);
	return EFalse;
	}

//
// aColorBuffer - Pixel values are copied from here to iBitmap
//
TBool CRawColorProcessor::SetPixelBlock(TRgb* aColorBuffer)
	{
	const TUint8* srcAddr = reinterpret_cast<TUint8*>(aColorBuffer);
	
	if (iPosition.iY < 0)
		{
		// We need to adjust the source read pointer to point to
		// the first pixel to be copied rather than the first
		// pixel in the buffer.
		srcAddr += (-iPosition.iY * iSrcBytesToSkip);
		iPosition.iY = 0;
		}
	
	srcAddr += iPosition.iX;
	
	while (iPosition.iY < iScanlinesToDraw)
		{
		Mem::Copy(iDestAddr, srcAddr, iCopyLength);
		iDestAddr += iOutputScanlineSize;
		srcAddr += iSrcBytesToSkip;
		iPosition.iY++;
		}
	
	UpdateScanlineLimit();

	return ETrue;
	}

//
// This function figures out how many scanlines need to be drawn
// for each call to SetPixelBlock().
//
void CRawColorProcessor::UpdateScanlineLimit()
	{
	ASSERT(iBlockSize.iHeight > 0);
	
	iScanlinesToDraw += iBlockSize.iHeight;
	iScanlinesToDraw = Min(iScanlinesToDraw, iMaxScanlines);
	}


//
//
//
void CRawColorProcessor::SetPixelPadding(TInt aNumberOfPixels)
	{
	iPosition.iX = aNumberOfPixels;
	}

//
//
//
void CRawColorProcessor::SetYPosIncrement(TInt /*aYInc*/)
	{
	ASSERT(EFalse);
	}


//
// If a clipping rect has been set, aImageRect contains the location of the
// rectangle of MCUs that are needed to render the contents of the clipping rect.
// Otherwise, it is the scaled, normal oriented
//
void CRawColorProcessor::PrepareL(CFbsBitmap& aBitmap, const TRect& aImageRect, const TSize& aRgbBlockSize)
	{
	if (aBitmap.SizeInPixels().iHeight == 0 || aBitmap.SizeInPixels().iWidth == 0)
		{
		User::Leave(KErrArgument);
		}
	
	iBitmap = &aBitmap;
	if (!iBitmap)
		{
		// No bitmap to draw to.
		User::Leave(KErrArgument);
		}
	
	iBlockSize = aRgbBlockSize;
	if ((aRgbBlockSize.iWidth <= 0) || (aRgbBlockSize.iHeight <= 0))
		{
		// No buffer to copy from.
		User::Leave(KErrArgument);
		}

	iPixelSize = TDisplayModeUtils::NumDisplayModeBitsPerPixel(iBitmap->DisplayMode()) / 8;
	ASSERT(iPixelSize != 0);
	
	// Convert any pixel padding values from  pixels to bytes.
	iPosition.iX *= iPixelSize;
	iPosition.iY = -iNumberOfScanlinesToSkip;
	
	// Get the address of the first pixel to write.
	iDestAddr = reinterpret_cast<TUint8*>(iBitmap->DataAddress());
	
	// If clipping, aImageRect.Width should be <= iBlockSize.iWidth
	// If not clipping, iBlockSize.iWidth should be >= pw
	TInt pw = iBitmap->SizeInPixels().iWidth;
	iMaxScanlines = Min(aImageRect.Height(), iBitmap->SizeInPixels().iHeight);
	iCopyLength = Min(aImageRect.Width(), Min(iBlockSize.iWidth, pw));
	iCopyLength *= iPixelSize;
	
	iOutputScanlineSize = CFbsBitmap::ScanLineLength(pw, iBitmap->DisplayMode());
	
	ASSERT(iPosition.iX >= 0);
	ASSERT(iPosition.iY <= 0);
	
	iScanlinesToDraw = iPosition.iY;
	UpdateScanlineLimit();
	
	// If this assert fails it probably means some value is not being scaled.
	ASSERT(iScanlinesToDraw > 0);
		
	iSrcBytesToSkip = iBlockSize.iWidth * iPixelSize;
	
	// Skip to a point in the output bitmap where the first scanline
	// will be drawn. The very last scanline should be drawn at (0, 0)
	if (iOperation == EDecodeRotate180 || iOperation == EDecodeRotate270 ||
		iOperation == EDecodeHorizontalFlip || iOperation == EDecodeVerticalFlipRotate90)
		{
		TInt scanlineSkip = iMaxScanlines - 1;

		scanlineSkip *= iOutputScanlineSize;
		iDestAddr += scanlineSkip;
		iOutputScanlineSize = -iOutputScanlineSize;	// Draw from higher to lower addresses.
		}
	}


