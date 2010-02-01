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

#ifndef ICOMASKPROCESSOR_H
#define ICOMASKPROCESSOR_H

#include <icl/imageprocessor.h>

/**
Image Processor specific to the ICO decoder when decoding to a 16MA destination bitmap.
This class applies the alpha channel
*/
class CIcoMaskProcessor : public CImageProcessor
	{
public:
	static CIcoMaskProcessor* NewL(TInt aReductionFactor);
	~CIcoMaskProcessor();
	
	// From CImageProcessor
	void PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect);
	void PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect,const TSize& aRgbBlockSize);
	void SetYPosIncrement(TInt aYInc);
	void SetLineRepeat(TInt aLineRepeat);
	void SetPixelPadding(TInt aNumberOfPixels);
	TBool SetPixel(TRgb aColor);
	TBool SetPixelRun(TRgb aColor,TInt aCount);
	TBool SetPixels(TRgb* aColorBuffer,TInt aBufferLength);
	TBool SetPixelBlock(TRgb* aColorBuffer);
	TBool SetMonoPixel(TInt aGray256);
	TBool SetMonoPixelRun(TInt aGray256,TInt aCount);
	TBool SetMonoPixels(TUint32* aGray256Buffer,TInt aBufferLength);
	TBool SetMonoPixelBlock(TUint32* aGray256Buffer);
	TBool SetPos(const TPoint& aPosition);
	TBool FlushPixels();

private:
	CIcoMaskProcessor(TInt aReductionFactor);
	void Reset();
	void ProcessScaledPixels();
	TBool DoFlushPixels();

private:
	class TMonochromeSum
		{
	public:
		TInt iLevel;
		TInt iCount;
		};

private:
	TInt iReductionFactor;
	CFbsBitmap* iDestBitmap; // not owned
	TRect iImageRegion;
	TPoint iPosition;
	TPoint iScaledPosition;
	TInt iYIncrement;
	TInt iPixelPadding;
	TInt iPixelsToSkip;
	
	TRgb* iMaskBuffer;
	TRgb* iMaskBufferPtr;
	TRgb* iMaskBufferPtrLimit;
	
	// For scaling
	TMonochromeSum* iScaledMaskBuffer;
	TMonochromeSum* iScaledMaskBufferPtrLimit;
	};

#endif // ICOMASKPROCESSOR_H

