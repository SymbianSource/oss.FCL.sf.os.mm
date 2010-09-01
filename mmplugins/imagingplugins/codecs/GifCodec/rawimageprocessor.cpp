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

/**
 @file
 @internalComponent
*/

#include "rawimageprocessor.h"

// The classes below are used for Gif Decoder to optimize decoded data transfer 
// to destination bitmap. The main purpose of these classes is to provide direct access
// to bitmap memory and to remove colour transformation/not used functionality of standard image processor
// overheads.

// RawImageProcessor

CRawImageProcessor* CRawImageProcessor::NewL()
 	{
   	return new (ELeave) CRawImageProcessor();
   	}

CRawImageProcessor::CRawImageProcessor()
	{
	}
//  from the CImageProcessor //

TBool CRawImageProcessor::SetPixelBlock(TRgb* /* aColorBuffer */)
	{
    ASSERT(EFalse);
   	return ETrue;
   	}

TBool CRawImageProcessor::SetPixels(TRgb* aColorBuffer,TInt aNumOfPixels)
    {
	TInt length = aNumOfPixels * iPixelSize;
	if(iBitmapDataPtr + length > iBitmapDataPtrLimit) 
		{
		if (iBitmapDataPtrLimit != iBitmapDataPtr) 
			{
			if (iBitmapDataPtr < iBitmapDataPtrLimit) 
				{
				Mem::Copy(iBitmapDataPtr, aColorBuffer, iBitmapDataPtrLimit - iBitmapDataPtr);		
				}
			iBitmapDataPtr = iBitmapDataPtrLimit;
			return ETrue;
			}
		return EFalse;
		}
	Mem::Copy(iBitmapDataPtr, aColorBuffer, length);
	iBitmapDataPtr += length;
    return ETrue;
    }

TBool CRawImageProcessor::SetMonoPixel(TInt /*aGray256*/)
	{
	ASSERT(EFalse);
   	return EFalse;
   	}

TBool CRawImageProcessor::SetMonoPixelRun(TInt /*aGray256*/,TInt /*aCount*/)
	{
   	ASSERT(EFalse);
   	return EFalse;
   	}

TBool CRawImageProcessor::SetMonoPixels(TUint32* /*aGray256Buffer*/,TInt /*aBufferLength*/)
    {
    ASSERT(EFalse);
    return EFalse;
    }

TBool CRawImageProcessor::SetMonoPixelBlock(TUint32* /*aGray256Buffer*/)
    {
    ASSERT(EFalse);
    return EFalse;
    }

TBool CRawImageProcessor::SetPos(const TPoint& aPosition)
    {
    iBitmapDataPtr = iBitmapData + (aPosition.iY-iImageRect.iTl.iY) * iScanlineSize + (aPosition.iX-iImageRect.iTl.iX) * iPixelSize;
    return ETrue;
    }

TBool CRawImageProcessor::FlushPixels()
    {
    return ETrue;
    }

void CRawImageProcessor::PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect)
    {
   	if( aImageRect.iTl.iX<0 || aImageRect.iTl.iY<0 || aImageRect.Width()>aBitmap.SizeInPixels().iWidth || aImageRect.Height()>aBitmap.SizeInPixels().iHeight )
		User::Leave(KErrArgument);
    
    iImageRect = aImageRect;
    iBitmap 	= &aBitmap;
    iPixelSize	= TDisplayModeUtils::NumDisplayModeBitsPerPixel( aBitmap.DisplayMode() );

    if (iPixelSize & 7) // we do not support display modes with pixel size of not multiple to byte size
        {
        ASSERT(EFalse);
        User::Leave(KErrArgument);
        }

    iPixelSize >>= 3;
    
    iScanlineSize = iBitmap->ScanLineLength(aBitmap.SizeInPixels().iWidth, aBitmap.DisplayMode() );
    
    iBitmapData = reinterpret_cast<TUint8*>( iBitmap->DataAddress() ); // destination bitmap data ptr, not owned
	
	if (!iBitmapData)// not sure it is possible but anyway
		{
		User::Leave(KErrArgument);
		}

    iBitmapDataPtr = iBitmapData;
	iBitmapDataPtrLimit = iBitmapDataPtr + iScanlineSize * aBitmap.SizeInPixels().iHeight;
    }

void CRawImageProcessor::PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& /*aImageRect*/, const TSize& /*aRgbBlockSize*/)
    {
    ASSERT(EFalse);
    }

void CRawImageProcessor::SetYPosIncrement(TInt /*aYInc*/)
    {
    ASSERT(EFalse);
    }

void CRawImageProcessor::SetLineRepeat(TInt /*aLineRepeat*/)
    {
    ASSERT(EFalse);
    }

void CRawImageProcessor::SetPixelPadding(TInt /*aNumberOfPixels*/)
    {
    ASSERT(EFalse);
    }
    
TBool CRawImageProcessor::SetPixelRun(TRgb /*aColor*/,TInt /*aCount*/)
   	{
   	ASSERT(EFalse);
   	return EFalse;
   	}

TBool CRawImageProcessor::SetPixel(TRgb /*aColor*/)
   	{
   	ASSERT(EFalse);
   	return EFalse;
   	}

// RawImageUtilProcessor

CRawImageUtilProcessor* CRawImageUtilProcessor::NewL()
	{
   	return new (ELeave) CRawImageUtilProcessor();
   	}

//  from the CImageProcessor //

TBool CRawImageUtilProcessor::SetPixels(TRgb* aColorBuffer,TInt aNumOfPixels)
    {
	TPoint pos;
	pos.iX = iPosition.iX;
	pos.iY = iPosition.iY;
	
	TInt width = iSize.iWidth;
	TInt height = iSize.iHeight;
	TInt pixelToWrite = 0;
	
	while (aNumOfPixels && pos.iY < height) 
		{
		pixelToWrite = Min(width-pos.iX, aNumOfPixels);
		iUtil.SetPixels(reinterpret_cast<TUint32*>(aColorBuffer), pixelToWrite);
		aNumOfPixels -= pixelToWrite;
		aColorBuffer += pixelToWrite;
		pos.iX += pixelToWrite;
		if (pos.iX == width) 
			{
			pos.iX = 0;
			pos.iY++;
			}
		}
	
	iPosition.iX = pos.iX;
	iPosition.iY = pos.iY;
	return ETrue;
    }

TBool CRawImageUtilProcessor::SetMonoPixels(TUint32* aGray256Buffer,TInt aNumOfPixels)
    {
	TPoint pos;
	pos.iX = iPosition.iX;
	pos.iY = iPosition.iY;
	
	TInt width = iSize.iWidth;
	TInt height = iSize.iHeight;
	TInt pixelToWrite = 0;
	
	while (aNumOfPixels && pos.iY < height) 
		{
		pixelToWrite = Min(width-pos.iX, aNumOfPixels);
		iUtil.SetPixels(aGray256Buffer, pixelToWrite);
		aNumOfPixels -= pixelToWrite;
		aGray256Buffer += pixelToWrite;
		pos.iX += pixelToWrite;
		if (pos.iX == width) 
			{
			pos.iX = 0;
			pos.iY++;
			}
		}
	
	iPosition.iX = pos.iX;
	iPosition.iY = pos.iY;
	return ETrue;
    }

TBool CRawImageUtilProcessor::SetPos(const TPoint& aPosition)
    {
    iPosition.iX = aPosition.iX - iImageRect.iTl.iX;
    iPosition.iY = aPosition.iY - iImageRect.iTl.iY;
    return iUtil.SetPos(iPosition);
    }

void CRawImageUtilProcessor::PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect)
    {
    iImageRect = aImageRect;
    iSize = iImageRect.Size();
    iPosition = TPoint(0, 0);
	iUtil.SetBitmapL(&aBitmap);
	iUtil.Begin(iPosition);
	iUtil.End();
    }
    
