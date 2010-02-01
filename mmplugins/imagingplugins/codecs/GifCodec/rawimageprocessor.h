// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef RAWIMAGEPROCESSOR_H
#define RAWIMAGEPROCESSOR_H

#include <icl/imageprocessor.h>
#include "ImageProcessorPriv.h"

/**
   @file
   @internalComponent
*/
class CRawImageProcessor: public CImageProcessor
	{
public:
	static CRawImageProcessor* NewL();
// from the CImageProcessor
	TBool SetPixels(TRgb* aColorBuffer,TInt aBufferLength);
	TBool SetMonoPixel(TInt /*aGray256*/);
	TBool SetMonoPixelRun(TInt /*aGray256*/,TInt /*aCount*/);
	TBool SetMonoPixels(TUint32* /*aGray256Buffer*/,TInt /*aBufferLength*/);
	TBool SetMonoPixelBlock(TUint32* /*aGray256Buffer*/);
	void PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect);
	void PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& /*aImageRect*/,const TSize& /*aRgbBlockSize*/);
	void SetYPosIncrement(TInt /*aYInc*/);
	void SetLineRepeat(TInt /*aLineRepeat*/);
	void SetPixelPadding(TInt /*aNumberOfPixels*/);

	TBool SetPixelBlock(TRgb* /*aColorBuffer*/);
	TBool SetPixelRun(TRgb /*aColor*/,TInt /*aCount*/);
	TBool SetPixel(TRgb /*aColor*/);
	TBool SetPos(const TPoint& aPosition);
	TBool FlushPixels();

protected:
	CRawImageProcessor();
	
protected:	
	CFbsBitmap*	iBitmap;	// destination bitmap, not owned
	TUint8* iBitmapData;
	TUint8* iBitmapDataPtr; // destination bitmap data ptr, not owned
	TUint8* iBitmapDataPtrLimit; 
	TInt	iPixelSize; // size of pixels in bytes
	TInt	iScanlineSize;
	TRect	iImageRect;

	};

class CRawImageUtilProcessor: public CRawImageProcessor
	{

public:
	static CRawImageUtilProcessor* NewL();
// from the CImageProcessor
	TBool SetPixels(TRgb* aColorBuffer,TInt aBufferLength);
	TBool SetMonoPixels(TUint32* aGray256Buffer,TInt aBufferLength);
	void PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect);
	
	TBool SetPos(const TPoint& aPosition);

protected:
	TImageBitmapUtil iUtil;
	TPoint iPosition;
	TSize iSize;
	};

#endif // RAWIMAGEPROCESSOR_H

