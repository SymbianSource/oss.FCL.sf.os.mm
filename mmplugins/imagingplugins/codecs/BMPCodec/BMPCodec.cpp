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
#include "ImageUtils.h"

#include "BMPCodec.h"

/**
@file
@internalComponent
*/

// CBmpReadCodec
CBmpReadCodec::CBmpReadCodec(const TSize& aBmpSize, const TRgb* aPalette):
	iOriginalSize(aBmpSize),
	iPalette(aPalette)
	{}

CBmpReadCodec::~CBmpReadCodec()
	{
	}



TFrameState CBmpReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	const TUint8* srcStart = aSrc.Ptr();
	iDataPtr = srcStart;
	iDataPtrLimit = srcStart + aSrc.Length();

	DoProcessL();

	TInt bytesUsed = iDataPtr - srcStart;
	aSrc.Shift(bytesUsed);
	iBytesProcessed += bytesUsed;
	if (iBytesProcessed >= iBytesExpected)
		{
		ImageProcessor()->FlushPixels();
		return EFrameComplete;
		}

	return EFrameIncomplete;
	}
	
void CBmpReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* /*aDestinationMask*/)
	{
	iFrameSize = aFrameInfo.iFrameCoordsInPixels.Size();
	Pos().iY = iOriginalSize.iHeight - 1;

	const TSize destinationSize(aDestination.SizeInPixels());
	TInt reductionFactor = ReductionFactor(iOriginalSize, destinationSize);
	
	CImageProcessor* imageProc = ImageProcessorUtility::NewImageProcessorL(aDestination, reductionFactor, ERgb, aDisableErrorDiffusion);

	SetImageProcessor(imageProc);
	
	imageProc->PrepareL(aDestination, iOriginalSize);
	imageProc->SetPos(TPoint(0,iOriginalSize.iHeight - 1));
	imageProc->SetYPosIncrement(-1);
	iBytesProcessed = 0;
	DoNewFrameL();

	ClearBitmapL(aDestination, KRgbWhite);
		// clear full bitmap, in case of streaming partial decode
	}

void CBmpReadCodec::InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameData*/)
	{
	ASSERT(aFrameInfo.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameInfo;
	aFrameInfo.SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CBmpReadCodec::ProcessFrameHeaderL(TBufPtr8&/* aData*/)
	{
	ASSERT(iFrameInfo);
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
	return EFrameComplete;
	}

void CBmpReadCodec::DoNewFrameL()
	{}


// CBmp1BppReadCodec
CBmp1BppReadCodec::CBmp1BppReadCodec(const TSize& aBmpSize, const TRgb* aPalette):
	CBmpReadCodec(aBmpSize, aPalette)
	{
	ASSERT(iPalette);
	}


CBmp1BppReadCodec* CBmp1BppReadCodec::NewL(const TSize& aBmpSize, const TRgb* aPalette)
{
	CBmp1BppReadCodec* self = new(ELeave) CBmp1BppReadCodec(aBmpSize, aPalette);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
}

void CBmp1BppReadCodec::DoProcessL()
	{
	CImageProcessor *const imageProc = ImageProcessor();
	while (iDataPtr < iDataPtrLimit)
		{
		TUint8 value = *iDataPtr++;

		for(TUint8 pixelMask = 0x80; pixelMask!=0 ;pixelMask >>= 1)
			imageProc->SetPixel((value & pixelMask) ? iEntry1 : iEntry0);
		}
	}

void CBmp1BppReadCodec::DoNewFrameL()
	{
	iEntry0 = iPalette[0];
	iEntry1 = iPalette[1];

	TInt actualWidth = (iOriginalSize.iWidth + 31) & ~31;
	ImageProcessor()->SetPixelPadding(actualWidth - iOriginalSize.iWidth);
	iBytesExpected = (actualWidth >> 3) * iOriginalSize.iHeight;
	}


// CBmpNoComp4BppReadCodec
CBmpNoComp4BppReadCodec* CBmpNoComp4BppReadCodec::NewL(const TSize& aBmpSize, const TRgb* aPalette)
{
	CBmpNoComp4BppReadCodec* self = new(ELeave) CBmpNoComp4BppReadCodec(aBmpSize, aPalette);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

CBmpNoComp4BppReadCodec::CBmpNoComp4BppReadCodec(const TSize& aBmpSize, const TRgb* aPalette):
	CBmpReadCodec(aBmpSize, aPalette)
	{
	ASSERT(iPalette);
	}

void CBmpNoComp4BppReadCodec::DoNewFrameL()
	{
	TInt actualWidth = (iOriginalSize.iWidth + 7) & ~7;
	ImageProcessor()->SetPixelPadding(actualWidth - iOriginalSize.iWidth);
	iBytesExpected = (actualWidth >> 1) * iOriginalSize.iHeight;
	}

void CBmpNoComp4BppReadCodec::DoProcessL()
	{
	CImageProcessor *const imageProc = ImageProcessor();
	while (iDataPtr < iDataPtrLimit)
		{
		TUint8 value = *iDataPtr++;
		imageProc->SetPixel(iPalette[value >> 4]);
		imageProc->SetPixel(iPalette[value & 0x0f]);
		}
	}


// CBmpRLE4ReadCodec
CBmpRLE4ReadCodec* CBmpRLE4ReadCodec::NewL(const TSize& aBmpSize, const TRgb* aPalette)
{
	CBmpRLE4ReadCodec* self = new(ELeave) CBmpRLE4ReadCodec(aBmpSize, aPalette);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

CBmpRLE4ReadCodec::CBmpRLE4ReadCodec(const TSize& aBmpSize, const TRgb* aPalette):
	CBmpReadCodec(aBmpSize, aPalette)
	{
	ASSERT(iPalette);
	}

void CBmpRLE4ReadCodec::DoNewFrameL()
	{
	iBytesExpected = KMaxTInt;
	}

void CBmpRLE4ReadCodec::DoProcessL()
	{
	const TUint8* srcSafeLimit = iDataPtrLimit - 1;

	TPoint& pos = Pos();
	CImageProcessor*const imageProc = ImageProcessor();

	while (iDataPtr < srcSafeLimit || pos.iY < 0)
		{
		TUint8 index = *iDataPtr++;
		if (index > 0) // Encoded mode
			WriteRun(*iDataPtr++,index);
		else
			{
			index = *iDataPtr++;
			if (index > 2) // Absolute mode
				{
				TInt byteCount = (index + ((index & 1) ? 1 : 0)) >> 1;
				TInt padding = (byteCount & 1) ? 1 : 0;
				if ((iDataPtr + (byteCount + padding)) > iDataPtrLimit)
					{
					iDataPtr -= 2;
					break;
					}
				WriteData(index);
				if (padding)
					iDataPtr++;
				}
			else if (index == 0) // End of line
				{
				pos.iX = 0;
				pos.iY--;
				if (!imageProc->SetPos(pos))
					{
					iBytesExpected = 0;
					break;
					}
				}
			else if (index == 1) // End of bitmap
				{
				iBytesExpected = 0;
				Pos().iY = -1;
				break;
				}
			else // Delta
				{
				if ((iDataPtr + 2) > iDataPtrLimit)
					{
					iDataPtr -= 2;
					break;
					}
				imageProc->FlushPixels();
				pos.iX += *iDataPtr++;
				pos.iY -= *iDataPtr++;
				if (!imageProc->SetPos(pos))
					break;
				}
			}
		}
	if (pos.iY < 0)
	    {
	    // that may mean a dodgy image, so flag decoding immediately complete
	    iBytesExpected = 0;
	    }		
	}

void CBmpRLE4ReadCodec::WriteRun(TUint8 aValue, TInt aNumPixels)
	{
	CImageProcessor*const imageProc = ImageProcessor();
	Pos().iX += aNumPixels;
	TRgb highValue = iPalette[aValue >> 4];
	TRgb lowValue = iPalette[aValue & 0x0f];
	TInt numPixels;
	for (numPixels = aNumPixels; numPixels > 1; numPixels -= 2)
		{
		imageProc->SetPixel(highValue);
		imageProc->SetPixel(lowValue);
		}
	if (numPixels & 1)
		imageProc->SetPixel(highValue);
	}

void CBmpRLE4ReadCodec::WriteData(TInt aNumPixels)
	{
	CImageProcessor*const imageProc = ImageProcessor();
	const TUint8* dataPtrLimit = iDataPtr + (aNumPixels >> 1);
	while (iDataPtr < dataPtrLimit)
		{
		TUint8 value = *iDataPtr++;
		imageProc->SetPixel(iPalette[value >> 4]);
		imageProc->SetPixel(iPalette[value & 0x0f]);
		}
	if (aNumPixels & 1)
		{
		TUint8 value = *iDataPtr++;
		imageProc->SetPixel(iPalette[value >> 4]);
		}
	Pos().iX += aNumPixels;
	}


// CBmpNoComp8BppReadCodec
CBmpNoComp8BppReadCodec* CBmpNoComp8BppReadCodec::NewL(const TSize& aBmpSize, const TRgb* aPalette)
{
	CBmpNoComp8BppReadCodec* self = new(ELeave) CBmpNoComp8BppReadCodec(aBmpSize, aPalette);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

CBmpNoComp8BppReadCodec::CBmpNoComp8BppReadCodec(const TSize& aBmpSize, const TRgb* aPalette):
	CBmpReadCodec(aBmpSize, aPalette)
	{
	ASSERT(iPalette);
	}

void CBmpNoComp8BppReadCodec::DoNewFrameL()
	{
	TInt actualWidth = (iOriginalSize.iWidth + 3) & ~3;
	ImageProcessor()->SetPixelPadding(actualWidth - iOriginalSize.iWidth);
	iBytesExpected = actualWidth * iOriginalSize.iHeight;
	}

void CBmpNoComp8BppReadCodec::DoProcessL()
	{
	CImageProcessor*const imageProc = ImageProcessor();
	while (iDataPtr < iDataPtrLimit)
		imageProc->SetPixel(iPalette[*iDataPtr++]);
	}


// CBmpRLE8ReadCodec
CBmpRLE8ReadCodec* CBmpRLE8ReadCodec::NewL(const TSize& aBmpSize, const TRgb* aPalette)
{
	CBmpRLE8ReadCodec* self = new(ELeave) CBmpRLE8ReadCodec(aBmpSize, aPalette);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

CBmpRLE8ReadCodec::CBmpRLE8ReadCodec(const TSize& aBmpSize, const TRgb* aPalette):
	CBmpReadCodec(aBmpSize, aPalette)
	{
	ASSERT(iPalette);
	}

void CBmpRLE8ReadCodec::DoNewFrameL()
	{
	iBytesExpected = KMaxTInt;
	}

void CBmpRLE8ReadCodec::DoProcessL()
	{
	const TUint8* srcSafeLimit = iDataPtrLimit - 1;

	TPoint& pos = Pos();
	CImageProcessor*const imageProc = ImageProcessor();

	while (iDataPtr < srcSafeLimit || pos.iY < 0)
		{
		TUint8 index = *iDataPtr++;
		if (index > 0) // Encoded mode
			WriteRun(*iDataPtr++,index);
		else
			{
			index = *iDataPtr++;
			if (index > 2) // Absolute mode
				{
				TInt padding = (index & 1) ? 1 : 0;
				if ((iDataPtr + (index + padding)) > iDataPtrLimit)
					{
					iDataPtr -= 2;
					break;
					}
				WriteData(index);
				if (padding)
					iDataPtr++;
				}
			else if (index == 0) // End of line
				{
				pos.iX = 0;
				pos.iY--;
				if (!imageProc->SetPos(pos))
					{
					iBytesExpected = 0;
					break;
					}
				}
			else if (index == 1) // End of bitmap
				{
				iBytesExpected = 0;
				Pos().iY = -1;
				break;
				}
			else // Delta
				{
				if ((iDataPtr + 2) > iDataPtrLimit)
					{
					iDataPtr -= 2;
					break;
					}
				imageProc->FlushPixels();
				pos.iX += *iDataPtr++;
				pos.iY -= *iDataPtr++;
				if (!imageProc->SetPos(pos))
					break;
				}
			}
		}
	
	if (pos.iY < 0)
	    {
	    // that may indicate a dodgy image, so flag decoding complete
	    iBytesExpected = 0;
	    }
	}

void CBmpRLE8ReadCodec::WriteRun(TUint8 aValue, TInt aNumPixels)
	{
	CImageProcessor*const imageProc = ImageProcessor();
	Pos().iX += aNumPixels;
	TRgb value = iPalette[aValue];
	for (; aNumPixels > 0; aNumPixels--)
		imageProc->SetPixel(value);
	}

void CBmpRLE8ReadCodec::WriteData(TInt aNumPixels)
	{
	CImageProcessor*const imageProc = ImageProcessor();
	Pos().iX += aNumPixels;
	const TUint8* dataPtrLimit = iDataPtr + aNumPixels;
	while (iDataPtr < dataPtrLimit)
		imageProc->SetPixel(iPalette[*iDataPtr++]);
	}

/*static*/
CBmpBiRgbReadCodec* CBmpBiRgbReadCodec::NewL(const TSize& aBmpSize, TInt aPixelSize, const TRgb* aPalette)
	{
	CBmpBiRgbReadCodec* self = new(ELeave) CBmpBiRgbReadCodec(aBmpSize, aPixelSize, aPalette);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}

CBmpBiRgbReadCodec::CBmpBiRgbReadCodec(const TSize& aBmpSize, TInt aPixelSize, const TRgb* aPalette): 
	CBmpReadCodec(aBmpSize, aPalette),
	iPixelSize(aPixelSize)
	{
	}
/**
	calculate:
	1. mask used to extract channel value
	2. right shift value used to place channel value into "low" byte
	3. left shift value used to scale up channel if it has got less than 8 bits
*/	
void CBmpBiRgbReadCodec::ConstructL()
	{
	// 16 Bpp 5-5-5 bitmaps do not have masks defined
	// as they are "standard", so use predefined ones
	static const TUint32 KDefault555Masks[]=
		{
		0x7C00u,	//red
		0x03E0u,	//green
		0x001Fu  	//blue
		};
		
	CBmpReadCodec::ConstructL();
	
	const TRgb* const masks= iPalette ? iPalette : reinterpret_cast<const TRgb*>(KDefault555Masks);

	for (TInt comp=0; comp<=KBlueCompIdx; comp++)
		{
		iCompInfo[comp].iMask = 0x00FFFFFF & masks[comp].Internal(); // ignore the alpha channel
		TInt shiftVal=0;
		const TUint maskValue=iCompInfo[comp].iMask;
		while (shiftVal < sizeof(TUint32) * 8 && ((maskValue >> shiftVal) &1 )==0)
			{
			++shiftVal;
			}
		iCompInfo[comp].iRightShiftValue = shiftVal;
		while (shiftVal < sizeof(TUint32) * 8 && ((maskValue >> shiftVal) &1) !=0 )
			{
			++shiftVal;
			}
		iCompInfo[comp].iLeftShiftValue = 8 - (shiftVal - iCompInfo[comp].iRightShiftValue);
		if (iCompInfo[comp].iLeftShiftValue < 0)
			{
			// We've got image with bad mask
			User::Leave(KErrCorrupt);
			}
		}
	}
	
void CBmpBiRgbReadCodec::DoProcessL()
	{
	TInt bytesRemaining = iDataPtrLimit - iDataPtr;			
	CImageProcessor* const imageProc = ImageProcessor();
	while (bytesRemaining >= iPixelSize)
		{
		TInt bytesToWrite = iPixelByteWidth - iBytePos;
		if (bytesToWrite > 0)
			{
			TInt pixelsToWrite = Min(bytesRemaining, bytesToWrite) / iPixelSize;
			const TInt bytesWritten = pixelsToWrite * iPixelSize;
			const TUint8* const tempDataPtrLimit = iDataPtr + bytesWritten;
			bytesRemaining 	-= bytesWritten;
			iBytePos 		+= bytesWritten;
			while (iDataPtr < tempDataPtrLimit)
				{
				TUint pixelValue;
				switch (iPixelSize)
					{
					case sizeof(TUint16):
						pixelValue=PtrReadUtil::ReadUint16(iDataPtr);
						break;
						
					case sizeof(TUint16) + sizeof(TUint8):
						pixelValue=PtrReadUtil::ReadUint16(iDataPtr) | (iDataPtr[2] << 16);
						break;
						
					case sizeof(TUint32):
						pixelValue=PtrReadUtil::ReadUint32(iDataPtr);
						break;						
						
					default:
						ASSERT(EFalse);
						pixelValue = 0;
					}
				TInt red=((pixelValue & iCompInfo[KRedCompIdx].iMask) >> iCompInfo[KRedCompIdx].iRightShiftValue) << iCompInfo[KRedCompIdx].iLeftShiftValue;
				red |=red >> (8 - iCompInfo[KRedCompIdx].iLeftShiftValue);
				
				TInt green = ((pixelValue & iCompInfo[KGreenCompIdx].iMask) >> iCompInfo[KGreenCompIdx].iRightShiftValue) << iCompInfo[KGreenCompIdx].iLeftShiftValue;
				green |= green >> (8 - iCompInfo[KGreenCompIdx].iLeftShiftValue);
				
				TInt blue = ((pixelValue & iCompInfo[KBlueCompIdx].iMask) >> iCompInfo[KBlueCompIdx].iRightShiftValue) << iCompInfo[KBlueCompIdx].iLeftShiftValue;
				blue |= blue >> (8 - iCompInfo[KBlueCompIdx].iLeftShiftValue);
				
				imageProc->SetPixel( TRgb(red, green, blue) );
				iDataPtr += iPixelSize;
				}
			}

		if (iBytePos >= iPixelByteWidth)
			{
			const TInt KBytesToSkip = Min(bytesRemaining, iTotalByteWidth - iBytePos);
			bytesRemaining -= KBytesToSkip;
			iBytePos += KBytesToSkip;
			iDataPtr += KBytesToSkip;
			}

		if (iBytePos >= iTotalByteWidth)
			{
			iBytePos = 0;
			}
		}	
	}
	
void CBmpBiRgbReadCodec::DoNewFrameL()
	{
	if (iPixelSize < 4)
		{
		iTotalByteWidth = ((iOriginalSize.iWidth * iPixelSize) + iPixelSize) & ~iPixelSize;
		}
	else
		{
		iTotalByteWidth = (iOriginalSize.iWidth * iPixelSize);
		}
	
	iPixelByteWidth = (iOriginalSize.iWidth * iPixelSize);
	iPaddingByteWidth = iTotalByteWidth - iPixelByteWidth;
	iBytePos = 0;
	iBytesExpected = iTotalByteWidth * iOriginalSize.iHeight;
	}

// CBmp24BppReadCodec
CBmp24BppReadCodec* CBmp24BppReadCodec::NewL(const TSize& aBmpSize)
{
	CBmp24BppReadCodec* self = new(ELeave) CBmp24BppReadCodec(aBmpSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

CBmp24BppReadCodec::CBmp24BppReadCodec(const TSize& aBmpSize)
	: CBmpReadCodec(aBmpSize, NULL)
	{}

void CBmp24BppReadCodec::DoProcessL()
	{
	TInt bytesRemaining = iDataPtrLimit - iDataPtr;			
	CImageProcessor*const imageProc = ImageProcessor();
	while (bytesRemaining > 2)
		{
		TInt bytesToWrite = iPixelByteWidth - iBytePos;
		if (bytesToWrite > 0)
			{
			TInt pixelsToWrite = Min(bytesRemaining, bytesToWrite) / 3;
			TInt bytesWritten = pixelsToWrite * 3;
			const TUint8* tempDataPtrLimit = iDataPtr + bytesWritten;
			bytesRemaining -= bytesWritten;
			iBytePos += bytesWritten;
			while (iDataPtr < tempDataPtrLimit)
				{
				imageProc->SetPixel(TRgb(iDataPtr[2], iDataPtr[1], iDataPtr[0]));
				iDataPtr += 3;
				}
			}

		if (iBytePos >= iPixelByteWidth)
			{
			TInt bytesToSkip = Min(bytesRemaining, iTotalByteWidth - iBytePos);
			bytesRemaining -= bytesToSkip;
			iBytePos += bytesToSkip;
			iDataPtr += bytesToSkip;
			}

		if (iBytePos >= iTotalByteWidth)
			iBytePos = 0;
		}
	}

void CBmp24BppReadCodec::DoNewFrameL()
	{
	iTotalByteWidth = ((iOriginalSize.iWidth * 3) + 3) & ~3;
	iPixelByteWidth = (iOriginalSize.iWidth * 3);
	iPaddingByteWidth = iTotalByteWidth - iPixelByteWidth;
	iBytePos = 0;
	iBytesExpected = iTotalByteWidth * iOriginalSize.iHeight;
	}

// CBmp32BppReadCodec
CBmp32BppReadCodec* CBmp32BppReadCodec::NewL(const TSize& aBmpSize)
{
	CBmp32BppReadCodec* self = new(ELeave) CBmp32BppReadCodec(aBmpSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

CBmp32BppReadCodec::CBmp32BppReadCodec(const TSize& aBmpSize)
	: CBmpReadCodec(aBmpSize, NULL)
	{}

void CBmp32BppReadCodec::DoProcessL()
	{
	TInt bytesRemaining = iDataPtrLimit - iDataPtr;
	CImageProcessor*const imageProc = ImageProcessor();
	while (bytesRemaining > 3)
		{
		TInt bytesToWrite = iPixelByteWidth - iBytePos;
		if (bytesToWrite > 0)
			{
			TInt pixelsToWrite = Min(bytesRemaining, bytesToWrite) / 4;
			TInt bytesWritten = pixelsToWrite * 4;
			const TUint8* tempDataPtrLimit = iDataPtr + bytesWritten;
			bytesRemaining -= bytesWritten;
			iBytePos += bytesWritten;
			while (iDataPtr < tempDataPtrLimit)
				{
				imageProc->SetPixel(TRgb(iDataPtr[2], iDataPtr[1], iDataPtr[0]));
				iDataPtr += 4;
				}
			}

		if (iBytePos >= iPixelByteWidth)
			{
			TInt bytesToSkip = Min(bytesRemaining, iTotalByteWidth - iBytePos);
			bytesRemaining -= bytesToSkip;
			iBytePos += bytesToSkip;
			iDataPtr += bytesToSkip;
			}

		if (iBytePos >= iTotalByteWidth)
			iBytePos = 0;
		}
	}

void CBmp32BppReadCodec::DoNewFrameL()
	{
	iTotalByteWidth = iOriginalSize.iWidth * 4;
	iPixelByteWidth = iTotalByteWidth;
	iPaddingByteWidth = iTotalByteWidth - iPixelByteWidth;
	iBytePos = 0;
	iBytesExpected = iTotalByteWidth * iOriginalSize.iHeight;
	}

// CBmpWriteCodec
void CBmpWriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource)
	{
	SetSource(&aSource);
	iSourceRect = TRect(aSource.SizeInPixels());
	iPos.SetXY(0,0);
	iPos.iY = iSourceRect.iBr.iY - 1;
	iPaddingBytes = PaddingBytes();
	iBytesToWrite = 0;
	aDst.SetLength(0); //No frame header
	}

TFrameState CBmpWriteCodec::ProcessFrameL(TBufPtr8& aDst)
	{
	//Setup buffer to use all available space
	TUint8* destStartPtr = CONST_CAST(TUint8*,aDst.Ptr());
	iDestPtr = destStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	DoProcessL(*Source());

	aDst.SetLength(iDestPtr - destStartPtr);

	// If processed all pixels
	if (iPos.iY < iSourceRect.iTl.iY)
		return EFrameComplete;

	return EFrameIncomplete;
	}


// CBmp1BppWriteCodec
CBmp1BppWriteCodec* CBmp1BppWriteCodec::NewL(void)
{
	CBmp1BppWriteCodec* self = new(ELeave) CBmp1BppWriteCodec();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

TInt CBmp1BppWriteCodec::PaddingBytes()
	{
	TInt byteWidth = (iSourceRect.Size().iWidth + 7) / 8;
	return ((byteWidth + 3) & ~3) - byteWidth;
	}

void CBmp1BppWriteCodec::DoProcessL(const CFbsBitmap& aFrame)
	{
	while (iDestPtr < iDestPtrLimit)
		{
		for ( ; (iBytesToWrite > 0) && (iDestPtr < iDestPtrLimit) ; iBytesToWrite--)
			*iDestPtr++ = 0;

		if (iPos.iY < 0)
			break;

		TInt scanLength = Min(iSourceRect.iBr.iX - iPos.iX, (iDestPtrLimit - iDestPtr) * 8);
		TInt dstLength = (scanLength + 7) / 8;
		TPtr8 dstBuf(iDestPtr,dstLength,dstLength);

		aFrame.GetScanLine(dstBuf, iPos, scanLength, EGray2);

		iPos.iX += scanLength;
		TUint8* tempDestPtrLimit = iDestPtr + dstLength;
		while (iDestPtr < tempDestPtrLimit)
			{
			TUint8 tempVal = iDestPtr[0];
			TUint reverseVal = 0;
			TUint origValCopy = tempVal;
			for(TInt countBit = 0; countBit<8; countBit++)
				{
				reverseVal<<=1;
				reverseVal |= origValCopy & 1;
				origValCopy>>=1;
				}
			iDestPtr[0] = TUint8(reverseVal);
			iDestPtr++;
			}

		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY--;
			iBytesToWrite = iPaddingBytes;
			}
		}
	}


// CBmp4BppWriteCodec
CBmp4BppWriteCodec* CBmp4BppWriteCodec::NewL(void)
{
	CBmp4BppWriteCodec* self = new(ELeave) CBmp4BppWriteCodec();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

TInt CBmp4BppWriteCodec::PaddingBytes()
	{
	TInt byteWidth = (iSourceRect.Size().iWidth + 1) / 2;
	return ((byteWidth + 3) & ~3) - byteWidth;
	}

void CBmp4BppWriteCodec::DoProcessL(const CFbsBitmap& aFrame)
	{
	while (iDestPtr < iDestPtrLimit)
		{
		for ( ; (iBytesToWrite > 0) && (iDestPtr < iDestPtrLimit) ; iBytesToWrite--)
			*iDestPtr++ = 0;

		if (iPos.iY < 0)
			break;

		TInt scanLength = Min(iSourceRect.iBr.iX - iPos.iX, (iDestPtrLimit - iDestPtr) * 2);
		TInt dstLength = (scanLength + 1) / 2;
		TPtr8 dstBuf(iDestPtr,dstLength,dstLength);

		aFrame.GetScanLine(dstBuf, iPos, scanLength, EColor16);

		iPos.iX += scanLength;
		TUint8* tempDestPtrLimit = iDestPtr + dstLength;
		while (iDestPtr < tempDestPtrLimit)
			{
			TInt tempVal = iDestPtr[0];
			TInt lowNibble = tempVal << 4;
			TInt highNibble = tempVal >> 4;
			iDestPtr[0] = TUint8(lowNibble | highNibble);
			iDestPtr++;
			}

		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY--;
			iBytesToWrite = iPaddingBytes;
			}
		}
	}


// CBmp8BppWriteCodec
CBmp8BppWriteCodec* CBmp8BppWriteCodec::NewL(void)
{
	CBmp8BppWriteCodec* self = new(ELeave) CBmp8BppWriteCodec();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

TInt CBmp8BppWriteCodec::PaddingBytes()
	{
	TInt byteWidth = iSourceRect.Size().iWidth;
	return ((byteWidth + 3) & ~3) - byteWidth;
	}

void CBmp8BppWriteCodec::DoProcessL(const CFbsBitmap& aFrame)
	{
	while (iDestPtr < iDestPtrLimit)
		{
		for ( ; (iBytesToWrite > 0) && (iDestPtr < iDestPtrLimit) ; iBytesToWrite--)
			*iDestPtr++ = 0;

		if (iPos.iY < 0)
			break;

		TInt scanLength = Min(iSourceRect.iBr.iX - iPos.iX, iDestPtrLimit - iDestPtr);
		TPtr8 dstBuf(iDestPtr, scanLength, scanLength);

		aFrame.GetScanLine(dstBuf, iPos, scanLength, EColor256);

		iPos.iX += scanLength;
		iDestPtr += scanLength;

		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY--;
			iBytesToWrite = iPaddingBytes;
			}
		}
	}


// CBmp24BppWriteCodec
CBmp24BppWriteCodec* CBmp24BppWriteCodec::NewL(void)
{
	CBmp24BppWriteCodec* self = new(ELeave) CBmp24BppWriteCodec();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

TInt CBmp24BppWriteCodec::PaddingBytes()
	{
	TInt byteWidth = iSourceRect.Size().iWidth * 3;
	return ((byteWidth + 3) & ~3) - byteWidth;
	}

void CBmp24BppWriteCodec::DoProcessL(const CFbsBitmap& aFrame)
	{
	TUint8* safeDestPtrLimit = iDestPtrLimit - 2;

	while (iDestPtr < safeDestPtrLimit)
		{
		for ( ; (iBytesToWrite > 0) && (iDestPtr < iDestPtrLimit) ; iBytesToWrite--)
			*iDestPtr++ = 0;

		if (iPos.iY < 0)
			break;

		TInt scanLength = Min(iSourceRect.iBr.iX - iPos.iX, (iDestPtrLimit - iDestPtr) / 3);
		TInt dstLength = scanLength * 3;
		TPtr8 dstBuf(iDestPtr, dstLength, dstLength);

		aFrame.GetScanLine(dstBuf, iPos, scanLength, EColor16M);

		iPos.iX += scanLength;
		iDestPtr += dstLength;

		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY--;
			iBytesToWrite = iPaddingBytes;
			}
		}
	}

