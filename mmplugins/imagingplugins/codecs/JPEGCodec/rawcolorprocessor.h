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

#ifndef RAWCOLORPROCESSOR_H
#define RAWCOLORPROCESSOR_H

#include "ImageProcessorPriv.h"

/**
   @file
   @internalComponent
*/

/**
	Very basic support for CImageProcessor interface - can be used
	only to put pixel block into destination bitmap
	The only methods which are supported SetPixelBlock() and SetPos()
*/
class CRawColorProcessor: public CImageProcessorExtension
	{
public:
	static CRawColorProcessor* NewL();
	void PrepareL(CFbsBitmap& aBitmap, const TRect& /*aImageRect*/, const TSize& aRgbBlockSize);
	TBool SetPixelBlock(TRgb* aColorBuffer);

// from the CImageProcessor //
	TBool SetPixels(TRgb* /*aColorBuffer*/,TInt /*aBufferLength*/);
	TBool SetMonoPixel(TInt /*aGray256*/);
	TBool SetMonoPixelRun(TInt /*aGray256*/,TInt /*aCount*/);
	TBool SetMonoPixels(TUint32* /*aGray256Buffer*/,TInt /*aBufferLength*/);
	TBool SetMonoPixelBlock(TUint32* /*aGray256Buffer*/);
	void PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& /*aImageRect*/);
	void SetLineRepeat(TInt /*aLineRepeat*/);
	TBool SetPixelRun(TRgb /*aColor*/,TInt /*aCount*/);
	TBool SetPixel(TRgb /*aColor*/);
	TBool SetPos(const TPoint& /*aPosition*/);
	TBool FlushPixels();
	
protected:
	CRawColorProcessor();
	void UpdateScanlineLimit();
	void SetPixelPadding(TInt aNumberOfPixels);
	void SetYPosIncrement(TInt aValue);

protected:	
	CFbsBitmap*	iBitmap;	// destination bitmap, not owned
	TSize		iBlockSize; // block dimesion in pixels
	TPoint		iPosition;  // current position within the target bitmap
	TInt 		iPixelSize; // size of pixels in bytes
	TInt		iScanlinesToDraw;	// No. of scanlines of the buffer to draw.
	TInt		iOutputScanlineSize;	// No. of bytes in the output bitmap scanline.
	//TInt		iBufLineInBytes;	// Number of bytes in a block scanline
	TInt		iCopyLength;		// The number of bytes to write.
	TUint8*		iDestAddr;			// Where we're writing to.
	TInt		iSrcBytesToSkip;	// No of bytes to skip to get to the next buffer scanline.
	TInt		iMaxScanlines;		// The total number of scanlines to be drawn.
	};

#endif // RAWCOLORPROCESSOR_H

