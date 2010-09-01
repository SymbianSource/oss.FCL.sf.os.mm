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
#include <s32mem.h>
#include "ImageClientMain.h"
#include "MBMCodec.h"

_LIT(KMBMPanicCategory, "MBMConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KMBMPanicCategory, aError);
	}


// CMbmReadCodec
CMbmReadCodec::CMbmReadCodec(TUint32 aMbmRootStreamOffset)
:iMbmRootStreamOffset(aMbmRootStreamOffset)
	{
	}

CMbmReadCodec::~CMbmReadCodec()
	{
	delete iCodecProc;
	}


CMbmReadCodec* CMbmReadCodec::NewL(TUint32 aMbmRootStreamOffset)
{
	CMbmReadCodec* self = new(ELeave) CMbmReadCodec(aMbmRootStreamOffset);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

TDisplayMode CMbmReadCodec::DisplayModeL(TInt aBitsPerPixel, TUint32 aFlags)
	{	
	if (aFlags & TFrameInfo::EColor)
		{
		switch (aBitsPerPixel)
			{
			case 4:
				return EColor16;
			case 8:
				return EColor256;
			case 12:
				return EColor4K;
			case 16:
				return EColor64K;
			case 24:
				return EColor16M;
			case 32:	
				if(aFlags & TFrameInfo::EAlphaChannel)
					{
					return EColor16MA;	
					}
				else
					{
					return EColor16MU;
					}
			default:
				User::Leave(KErrNotSupported);
			}
		}
	else
		{
		switch (aBitsPerPixel)
			{
			case 1:
				return EGray2;
			case 2:
				return EGray4;
			case 4:
				return EGray16;
			case 8:
				return EGray256;
			default:
				User::Leave(KErrNotSupported);
			}
		}

	return ENone;
	}

void CMbmReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* /*aFrameMask*/)
	{
	iFrameInfo = &aFrameInfo;
	iFrameData = STATIC_CAST(TMbmDecodeData*, aFrameImageData.GetFrameData(0));
 	iCompressed = iFrameData->iCompressed;

	iByteLength = iFrameData->iImageDataSizeInBytes;

	TPoint& pos = Pos();
	pos.SetXY(0,0);

	TSize originalSize(iFrameInfo->iFrameCoordsInPixels.Size());

	TDisplayMode displayMode = DisplayModeL(iFrameInfo->iBitsPerPixel,iFrameInfo->iFlags);
	const TSize destinationSize(aFrame.SizeInPixels());
	TInt reductionFactor = ReductionFactor(originalSize, destinationSize);
	
	CImageProcessor* imageProc = ImageProcessorUtility::NewImageProcessorL(aFrame, reductionFactor, displayMode, aDisableErrorDiffusion);
	SetImageProcessor(imageProc);

	imageProc->PrepareL(aFrame,originalSize);
	TInt scanLinePixels = ScanLinePixels(originalSize.iWidth);
	imageProc->SetPixelPadding(scanLinePixels - originalSize.iWidth);
	SetCodecProcessorL();

	ClearBitmapL(aFrame, KRgbWhite);
		// clear in case of streamed partial decodes
	}

void CMbmReadCodec::InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData)
	{
	ASSERT(aFrameSettings.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameSettings;
	iImageData = &aFrameImageData;
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CMbmReadCodec::ProcessFrameHeaderL(TBufPtr8& aData)
	{
	iStartDataPtr = CONST_CAST(TUint8*,aData.Ptr());
	iDataPtr = iStartDataPtr;
	iDataPtrLimit = iStartDataPtr + aData.Length();

	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrame)
		{
		if(iFrameDataRemaining <= aData.Length())
			iNumberOfFrames++;
		else
			{
			iMbmRootStreamOffset -= aData.Length();
			iFrameDataRemaining -= aData.Length();
			aData.Shift(aData.Length());
			return EFrameIncomplete;
			}
	
		if ((iMbmRootStreamOffset-iFrameDataRemaining) == 0)
			{
			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
			return EFrameComplete;
			}
		else if ((iMbmRootStreamOffset - iFrameDataRemaining) < KMbmInfoHeaderSize)
			{
			//Bitmap StreamLength is too short to continue processing. 
			User::Leave(KErrCorrupt);
			}
		else
			{
			aData.Shift(iFrameDataRemaining);
			iMbmRootStreamOffset -= iFrameDataRemaining;

			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
			return EFrameIncomplete;
			}
		}

	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrameHeader)
		{
		if (aData.Length()<KMbmInfoHeaderSize)
			User::Leave(KErrUnderflow);
		
		TMbmDecodeData* mbmDecodeData = new(ELeave)TMbmDecodeData;
		CleanupStack::PushL(mbmDecodeData);

		User::LeaveIfError(iImageData->AppendFrameData(mbmDecodeData));
		CleanupStack::Pop(); // mbmImageData

		SEpocBitmapHeader header;
		RDesReadStream stream;
		stream.Open(aData);
		CleanupClosePushL(stream);
		header.iBitmapSize=stream.ReadInt32L();
		header.iStructSize=stream.ReadInt32L();
		if (header.iStructSize!=sizeof(SEpocBitmapHeader)) User::Leave(KErrCorrupt);
		header.iSizeInPixels.iWidth=stream.ReadInt32L();
		header.iSizeInPixels.iHeight=stream.ReadInt32L();
		header.iSizeInTwips.iWidth=stream.ReadInt32L();
		header.iSizeInTwips.iHeight=stream.ReadInt32L();
		header.iBitsPerPixel=stream.ReadInt32L();
		header.iColor=stream.ReadUint32L();
		header.iPaletteEntries=stream.ReadInt32L();
		header.iCompression=(TBitmapfileCompression)stream.ReadUint32L();
		CleanupStack::PopAndDestroy(); //stream

		if (header.iBitmapSize<KMbmInfoHeaderSize)
			User::Leave(KErrCorrupt);

		iFrameDataRemaining = header.iBitmapSize - KMbmInfoHeaderSize;
		if (iMbmRootStreamOffset<KMbmInfoHeaderSize)
			User::Leave(KErrCorrupt);

		iMbmRootStreamOffset -= KMbmInfoHeaderSize;
		TInt frameDataOffset = iFrameInfo->FrameDataOffset();
		iFrameInfo->SetFrameDataOffset(frameDataOffset + KMbmInfoHeaderSize);
		iFrameInfo->iFrameCoordsInPixels.SetRect(TPoint(0,0),header.iSizeInPixels);
		iFrameInfo->iOverallSizeInPixels = header.iSizeInPixels;
		iFrameInfo->iFrameSizeInTwips = header.iSizeInTwips;
		iFrameInfo->iBitsPerPixel = header.iBitsPerPixel;
		iFrameInfo->iDelay = 0;
		iFrameInfo->iFlags = TFrameInfo::ECanDither;

		switch(header.iColor)
			{
			case SEpocBitmapHeader::ENoColor:
				break;
			case SEpocBitmapHeader::EColor:
				iFrameInfo->iFlags |= TFrameInfo::EColor;
				break;
			case SEpocBitmapHeader::EColorAlpha:
				iFrameInfo->iFlags |= TFrameInfo::EColor;
				iFrameInfo->iFlags |= TFrameInfo::ETransparencyPossible;
				iFrameInfo->iFlags |= TFrameInfo::EAlphaChannel;
				break;
			default:
				User::Leave(KErrNotSupported);
			}

		mbmDecodeData->iCompressed = header.iCompression;
		mbmDecodeData->iImageDataSizeInBytes = header.iBitmapSize - header.iStructSize;

		if (iFrameInfo->iFrameCoordsInPixels.iBr.iX < 0 || iFrameInfo->iFrameCoordsInPixels.iBr.iY < 0 ||
			iFrameInfo->iFrameSizeInTwips.iWidth < 0 || iFrameInfo->iFrameSizeInTwips.iHeight < 0 ||
			!(iFrameInfo->iBitsPerPixel == 1 || iFrameInfo->iBitsPerPixel == 2 || 
			iFrameInfo->iBitsPerPixel == 4 || iFrameInfo->iBitsPerPixel == 8 || 
			iFrameInfo->iBitsPerPixel == 12 || iFrameInfo->iBitsPerPixel == 16 || 
			iFrameInfo->iBitsPerPixel == 24 || iFrameInfo->iBitsPerPixel == 32))
			User::Leave(KErrCorrupt);

		switch (iFrameInfo->iBitsPerPixel)
			{
		case 1:
			iFrameInfo->iFrameDisplayMode = EGray2;
			break;

		case 2:
			iFrameInfo->iFrameDisplayMode = EGray4;
			break;

		case 4:
			iFrameInfo->iFrameDisplayMode = (iFrameInfo->iFlags & TFrameInfo::EColor) ? EColor16 : EGray16;
			break;

		case 8:
			iFrameInfo->iFrameDisplayMode = (iFrameInfo->iFlags & TFrameInfo::EColor) ? EColor256 : EGray256;
			break;

		case 12:
			iFrameInfo->iFrameDisplayMode = EColor4K;
			break;

		case 16:
			iFrameInfo->iFrameDisplayMode = EColor64K;
			break;

		case 24:
			iFrameInfo->iFrameDisplayMode = EColor16M;
			break;

		case 32:
			if(iFrameInfo->iFlags & TFrameInfo::ETransparencyPossible && iFrameInfo->iFlags & TFrameInfo::EAlphaChannel)
				{
				iFrameInfo->iFrameDisplayMode = EColor16MA;
				}
			else
				{
				iFrameInfo->iFrameDisplayMode = EColor16MU;
				}
			break;
			}

		aData.Shift(KMbmInfoHeaderSize);
		iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame);
		}

	return EFrameIncomplete;
	}

TFrameState CMbmReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	TUint8* srcPtr = CONST_CAST(TUint8*,aSrc.Ptr());
	iDataPtr = srcPtr;
	iDataPtrLimit = iDataPtr + Min(aSrc.Length(),iByteLength);

	iCodecProc->DoProcessL(iDataPtr,iDataPtrLimit);
	TInt bytesUsed = iDataPtr - srcPtr;
	aSrc.Shift(bytesUsed);
	iByteLength -= bytesUsed;

	if (iByteLength == 0)
		{
		ImageProcessor()->FlushPixels();
		delete iCodecProc;
		iCodecProc = NULL;
		return EFrameComplete;
		}
	return EFrameIncomplete;
	}

TInt CMbmReadCodec::ScanLinePixels(TInt aWidth)
	{
	switch (iFrameInfo->iBitsPerPixel)
		{
	case 1:
		return CFbsBitmap::ScanLineLength(aWidth,EGray2) << 3;
	case 2:
		return CFbsBitmap::ScanLineLength(aWidth,EGray4) << 2;
	case 4:
		return CFbsBitmap::ScanLineLength(aWidth,EGray16) << 1;
	case 8:
		return CFbsBitmap::ScanLineLength(aWidth,EGray256);
	case 12:
	case 16:
		return CFbsBitmap::ScanLineLength(aWidth,EColor64K) >> 1;
	case 24:
		return CFbsBitmap::ScanLineLength(aWidth,EColor16M) / 3;
	case 32:
		return CFbsBitmap::ScanLineLength(aWidth,EColor16MU) / 4; // for all 32bit modes
	default:
		return 0;
		}
	}

void CMbmReadCodec::SetCodecProcessorL()
	{
	delete iCodecProc;
	iCodecProc = NULL;
	const TBool color = (iFrameInfo->iFlags & TFrameInfo::EColor);

	switch (iFrameInfo->iBitsPerPixel)
		{
	case 1:
		if (iCompressed)
			iCodecProc = new(ELeave) T1BppCompressedMbmReadCodec;
		else
			iCodecProc = new(ELeave) T1BppMbmReadCodec;
		break;
	case 2:
		if (iCompressed)
			iCodecProc = new(ELeave) T2BppCompressedMbmReadCodec;
		else
			iCodecProc = new(ELeave) T2BppMbmReadCodec;
		break;
	case 4:
		if (color)
			{
			if (iCompressed)
				iCodecProc = new(ELeave) T4BppColorCompressedMbmReadCodec;
			else
				iCodecProc = new(ELeave) T4BppColorMbmReadCodec;
			}
		else
			{
			if (iCompressed)
				iCodecProc = new(ELeave) T4BppMonoCompressedMbmReadCodec;
			else
				iCodecProc = new(ELeave) T4BppMonoMbmReadCodec;
			}
		break;
	case 8:
		if (color)
			{
			if (iCompressed)
				iCodecProc = new(ELeave) T8BppColorCompressedMbmReadCodec;
			else
				iCodecProc = new(ELeave) T8BppColorMbmReadCodec;
			}
		else
			{
			if (iCompressed)
				iCodecProc = new(ELeave) T8BppMonoCompressedMbmReadCodec;
			else
				iCodecProc = new(ELeave) T8BppMonoMbmReadCodec;
			}
		break;
	case 12:
		if (iCompressed)
			iCodecProc = new(ELeave) T12BppCompressedMbmReadCodec;
		else
			iCodecProc = new(ELeave) T12BppMbmReadCodec;
		break;
	case 16:
		if (iCompressed)
			iCodecProc = new(ELeave) T16BppCompressedMbmReadCodec;
		else
			iCodecProc = new(ELeave) T16BppMbmReadCodec;
		break;
	case 24:
		if (iCompressed)
			iCodecProc = new(ELeave) T24BppCompressedMbmReadCodec;
		else
			iCodecProc = new(ELeave) T24BppMbmReadCodec;
		break;
	case 32:
		if (iCompressed)
			User::Leave(KErrNotSupported);
		else
			iCodecProc = new(ELeave) T32BppMbmReadCodec;
		break;
	default:
		User::Leave(KErrCorrupt);
		}

	iCodecProc->iImageProc = ImageProcessor();
	}


// TMbmReadCodec
void TMbmReadCodec::AddPixelRun(TRgb aColor,TInt aNumberOfPixels)
	{
	iImageProc->SetPixelRun(aColor,aNumberOfPixels);
	}


// T1BppMbmReadCodec
void T1BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		AddPixelValue(*aDataPtr++);
	}

void T1BppMbmReadCodec::AddPixelValue(TUint8 aValue)
	{
	iImageProc->SetPixel((aValue & 0x01) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x02) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x04) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x08) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x10) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x20) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x40) ? KRgbWhite : KRgbBlack);
	iImageProc->SetPixel((aValue & 0x80) ? KRgbWhite : KRgbBlack);
	}


// T1BppCompressedMbmReadCodec
void T1BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt8 count = *aDataPtr++;

		if (count >= 0)
			{
			if (aDataPtr >= aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			AddPixelRun(*aDataPtr++,count + 1);
			}
		else
			{
			TUint8* tempDataPtrLimit = aDataPtr - count;
			if (tempDataPtrLimit > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			while (aDataPtr < tempDataPtrLimit)
				AddPixelValue(*aDataPtr++);
			}
		}
	}

void T1BppCompressedMbmReadCodec::AddPixelRun(TUint8 aValue,TInt aNumberOfValues)
	{
	TRgb color0 = (aValue & 0x01) ? KRgbWhite : KRgbBlack;
	TRgb color1 = (aValue & 0x02) ? KRgbWhite : KRgbBlack;
	TRgb color2 = (aValue & 0x04) ? KRgbWhite : KRgbBlack;
	TRgb color3 = (aValue & 0x08) ? KRgbWhite : KRgbBlack;
	TRgb color4 = (aValue & 0x10) ? KRgbWhite : KRgbBlack;
	TRgb color5 = (aValue & 0x20) ? KRgbWhite : KRgbBlack;
	TRgb color6 = (aValue & 0x40) ? KRgbWhite : KRgbBlack;
	TRgb color7 = (aValue & 0x80) ? KRgbWhite : KRgbBlack;

	for (; aNumberOfValues > 0; aNumberOfValues--)
		{
		iImageProc->SetPixel(color0);
		iImageProc->SetPixel(color1);
		iImageProc->SetPixel(color2);
		iImageProc->SetPixel(color3);
		iImageProc->SetPixel(color4);
		iImageProc->SetPixel(color5);
		iImageProc->SetPixel(color6);
		iImageProc->SetPixel(color7);
		}
	}


// T2BppMbmReadCodec
void T2BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		AddPixelValue(*aDataPtr++);
	}

void T2BppMbmReadCodec::AddPixelValue(TUint8 aValue)
	{
	iImageProc->SetPixel(TRgb::Gray4(aValue & 0x3));
	iImageProc->SetPixel(TRgb::Gray4((aValue >> 2) & 0x3));
	iImageProc->SetPixel(TRgb::Gray4((aValue >> 4) & 0x3));
	iImageProc->SetPixel(TRgb::Gray4((aValue >> 6) & 0x3));
	}


// T2BppCompressedMbmReadCodec
void T2BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt8 count = *aDataPtr++;

		if (count >= 0)
			{
			if (aDataPtr >= aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			AddPixelRun(*aDataPtr++,count + 1);
			}
		else
			{
			TUint8* tempDataPtrLimit = aDataPtr - count;
			if (tempDataPtrLimit > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			while (aDataPtr < tempDataPtrLimit)
				AddPixelValue(*aDataPtr++);
			}
		}
	}

void T2BppCompressedMbmReadCodec::AddPixelRun(TUint8 aValue,TInt aNumberOfQuads)
	{
	TRgb color0 = TRgb::Gray4(aValue & 0x3);
	TRgb color1 = TRgb::Gray4((aValue >> 2) & 0x3);
	TRgb color2 = TRgb::Gray4((aValue >> 4) & 0x3);
	TRgb color3 = TRgb::Gray4((aValue >> 6) & 0x3);

	for (; aNumberOfQuads > 0; aNumberOfQuads--)
		{
		iImageProc->SetPixel(color0);
		iImageProc->SetPixel(color1);
		iImageProc->SetPixel(color2);
		iImageProc->SetPixel(color3);
		}
	}


// T4BppMbmReadCodec
void T4BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt value = *aDataPtr++;
		iImageProc->SetPixel(ColorFromIndex(value & 0xf));
		iImageProc->SetPixel(ColorFromIndex(value >> 4));
		}
	}


// T4BppCompressedMbmReadCodec
void T4BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt8 count = *aDataPtr++;

		if (count >= 0)
			{
			if (aDataPtr >= aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			TInt value = *aDataPtr++;
			AddPixelRun(ColorFromIndex(value & 0xf),ColorFromIndex(value >> 4),count + 1);
			}
		else
			{
			TUint8* tempDataPtrLimit = aDataPtr - count;
			if (tempDataPtrLimit > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			while (aDataPtr < tempDataPtrLimit)
				{
				TInt value = *aDataPtr++;
				iImageProc->SetPixel(ColorFromIndex(value & 0xf));
				iImageProc->SetPixel(ColorFromIndex(value >> 4));
				}
			}
		}
	}

void T4BppCompressedMbmReadCodec::AddPixelRun(TRgb aColor1,TRgb aColor2,TInt aNumberOfPairs)
	{
	for (; aNumberOfPairs > 0; aNumberOfPairs--)
		{
		iImageProc->SetPixel(aColor1);
		iImageProc->SetPixel(aColor2);
		}
	}


// T8BppMbmReadCodec
void T8BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		iImageProc->SetPixel(ColorFromIndex(*aDataPtr++));
	}


// T8BppCompressedMbmReadCodec
void T8BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt8 count = *aDataPtr++;

		if (count >= 0)
			{
			if (aDataPtr >= aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			AddPixelRun(ColorFromIndex(*aDataPtr++),count + 1);
			}
		else
			{
			TUint8* tempDataPtrLimit = aDataPtr - count;
			if (tempDataPtrLimit > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			while (aDataPtr < tempDataPtrLimit)
				iImageProc->SetPixel(ColorFromIndex(*aDataPtr++));
			}
		}
	}


// T12BppMbmReadCodec
void T12BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	ASSERT(!(TInt(aDataPtr) & 1));

	TUint16* tempDataPtr = (TUint16*)aDataPtr;
	TUint16* tempDataPtrLimit = tempDataPtr + ((aDataPtrLimit - aDataPtr) >> 1);

	while (tempDataPtr < tempDataPtrLimit)
		iImageProc->SetPixel(TRgb::Color4K((*tempDataPtr++) & 0x0fff));

	aDataPtr = (TUint8*)tempDataPtr;
	}


// T12BppCompressedMbmReadCodec
void T12BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	if((TInt(aDataPtr) & 1))
		{
		// Not word aligned
		while (aDataPtr < aDataPtrLimit)
			{
			TInt value = (aDataPtr[0] | aDataPtr[1] << 8);
			AddPixelRun(TRgb::Color4K(value & 0x0fff),(value >> 12) + 1);
			aDataPtr += 2;
			}
		}	
	else
		{
		//Word Aligned
		TUint16* tempDataPtr = (TUint16*)aDataPtr;
		TUint16* tempDataPtrLimit = tempDataPtr + ((aDataPtrLimit - aDataPtr) >> 1);

		while (tempDataPtr < tempDataPtrLimit)
			{
			TInt value = *tempDataPtr++;
			AddPixelRun(TRgb::Color4K(value & 0x0fff),(value >> 12) + 1);
			}

		aDataPtr = (TUint8*)tempDataPtr;
		}
	}


// T16BppMbmReadCodec
void T16BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	ASSERT(!(TInt(aDataPtr) & 1));

	TUint16* tempDataPtr = (TUint16*)aDataPtr;
	TUint16* tempDataPtrLimit = tempDataPtr + ((aDataPtrLimit - aDataPtr) >> 1);

	while (tempDataPtr < tempDataPtrLimit)
		iImageProc->SetPixel(TRgb::Color64K(*tempDataPtr++));

	aDataPtr = (TUint8*)tempDataPtr;
	}


// T16BppCompressedMbmReadCodec
void T16BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt8 count = *aDataPtr++;

		if (count >= 0)
			{
			if (aDataPtr + 2 > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			AddPixelRun(TRgb::Color64K(aDataPtr[0] | (aDataPtr[1] << 8)),count + 1);
			aDataPtr += 2;
			}
		else
			{
			TUint8* tempDataPtrLimit = aDataPtr + (count * -2);
			if (tempDataPtrLimit > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			while (aDataPtr < tempDataPtrLimit)
				{
				iImageProc->SetPixel(TRgb::Color64K(aDataPtr[0] | (aDataPtr[1] << 8)));
				aDataPtr += 2;
				}
			}
		}
	}


// T24BppMbmReadCodec
void T24BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	TUint8* tempDataPtrLimit = aDataPtrLimit - 2; // Safe limit for jumps of three bytes
	while (aDataPtr < tempDataPtrLimit)
		{
		iImageProc->SetPixel(TRgb(aDataPtr[2],aDataPtr[1],aDataPtr[0]));
		aDataPtr += 3;
		}
	}


// T24BppCompressedMbmReadCodec
void T24BppCompressedMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt8 count = *aDataPtr++;

		if (count >= 0)
			{
			if (aDataPtr + 3 > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			AddPixelRun(TRgb(aDataPtr[2],aDataPtr[1],aDataPtr[0]),count + 1);
			aDataPtr += 3;
			}
		else
			{
			TUint8* tempDataPtrLimit = aDataPtr + (count * -3);
			if (tempDataPtrLimit > aDataPtrLimit)
				{
				aDataPtr--;
				break;
				}
			while (aDataPtr < tempDataPtrLimit)
				{
				iImageProc->SetPixel(TRgb(aDataPtr[2],aDataPtr[1],aDataPtr[0]));
				aDataPtr += 3;
				}
			}
		}
	}

// T32BppMbmReadCodec
void T32BppMbmReadCodec::DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit)
	{
	TUint8* tempDataPtrLimit = aDataPtrLimit - 3; // Safe limit for jumps of four bytes
	while (aDataPtr < tempDataPtrLimit)
		{
		TRgb rgb(aDataPtr[2],aDataPtr[1],aDataPtr[0],aDataPtr[3]);
		iImageProc->SetPixel(rgb);
		aDataPtr += 4;
		}
	}

// CMbmWriteCodec
CMbmWriteCodec::CMbmWriteCodec(TDisplayMode aDisplayMode):
	iDisplayMode(aDisplayMode)
	{}

CMbmWriteCodec::~CMbmWriteCodec()
	{
	delete iCodecProc;
	}

CMbmWriteCodec* CMbmWriteCodec::NewL(TDisplayMode aDisplayMode)
{
	CMbmWriteCodec* self = new(ELeave) CMbmWriteCodec(aDisplayMode);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}

TFrameState CMbmWriteCodec::ProcessFrameL(TBufPtr8& aDst)
	{
	TUint8* destStartPtr = CONST_CAST(TUint8*,aDst.Ptr());
	iDataPtr = destStartPtr;
	iDataPtrLimit = iDataPtr + aDst.MaxLength();

	if (iStreamTablePending)
		{
		TUint32 data = 1;
		Mem::Copy(iDataPtr,&data,sizeof(TUint32));
		data = KMbmFirstBitmapOffset;
		iDataPtr += sizeof(TUint32);
		Mem::Copy(iDataPtr,&data,sizeof(TUint32));
		iDataPtr += sizeof(TUint32);
		aDst.SetLength(iDataPtr - destStartPtr);

		return EFrameComplete;
		}

	DoProcessL(*Source());

	aDst.SetLength(iDataPtr - destStartPtr);

	if (iPos.iY >= iSourceRect.iBr.iY)
		{
		if (iDataPtrLimit - iDataPtr >= 8)
			{
			TUint32 data = 1;
			Mem::Copy(iDataPtr,&data,sizeof(TUint32));
			iDataPtr += sizeof(TUint32);
			data = KMbmFirstBitmapOffset;
			Mem::Copy(iDataPtr,&data,sizeof(TUint32));
			iDataPtr += sizeof(TUint32);
			aDst.SetLength(iDataPtr - destStartPtr);
			return EFrameComplete;
			}
		else
			iStreamTablePending = ETrue;
		}

	return EFrameIncomplete;
	}

void CMbmWriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aFrame)
	{
	SetSource(&aFrame);

	TUint8* destStartPtr = CONST_CAST(TUint8*,aDst.Ptr());
	iDataPtr = destStartPtr;
	iDataPtrLimit = iDataPtr + aDst.MaxLength();

	iStreamTablePending = EFalse;

	iSourceRect = TRect(aFrame.SizeInPixels());
	iPos.SetXY(0,0);

	TInt byteWidth = CFbsBitmap::ScanLineLength(iSourceRect.Size().iWidth,iDisplayMode);
	SEpocBitmapHeader bmpHeader;

	bmpHeader.iStructSize = sizeof(SEpocBitmapHeader);
	bmpHeader.iSizeInPixels = iSourceRect.Size();
	bmpHeader.iSizeInTwips = aFrame.SizeInTwips();

	delete iCodecProc;
	iCodecProc = NULL;

	switch (iDisplayMode)
		{
	case EGray2:
		bmpHeader.iBitsPerPixel = 1;
		iCodecProc = new(ELeave) T1BppWriteCodecProcessor;
		break;
	case EGray4:
		bmpHeader.iBitsPerPixel = 2;
		iCodecProc = new(ELeave) T2BppWriteCodecProcessor;
		break;
	case EGray16:
	case EColor16:
		bmpHeader.iBitsPerPixel = 4;
		iCodecProc = new(ELeave) T4BppWriteCodecProcessor;
		break;
	case EGray256:
	case EColor256:
		bmpHeader.iBitsPerPixel = 8;
		iCodecProc = new(ELeave) T8BppWriteCodecProcessor;
		break;
	case EColor4K:
		bmpHeader.iBitsPerPixel = 12;
		iCodecProc = new(ELeave) T16BppWriteCodecProcessor;
		break;
	case EColor64K:
		bmpHeader.iBitsPerPixel = 16;
		iCodecProc = new(ELeave) T16BppWriteCodecProcessor;
		break;
	case EColor16M:
		bmpHeader.iBitsPerPixel = 24;
		iCodecProc = new(ELeave) T24BppWriteCodecProcessor;
		break;
	case EColor16MU:
	case EColor16MA:
		bmpHeader.iBitsPerPixel = 32;
		iCodecProc = new(ELeave) T32BppWriteCodecProcessor;
		break;
	default:
		Panic(EBadDisplayMode);
		break;
		}

	switch (iDisplayMode)
		{
	case EGray2:
	case EGray4:
	case EGray16:
	case EGray256:
		bmpHeader.iColor = 0;
		break;
	case EColor16:
	case EColor256:
	case EColor4K:
	case EColor64K:
	case EColor16M:
	case EColor16MU:
		bmpHeader.iColor = 1;
		break;
	case EColor16MA:
		bmpHeader.iColor = 2;
		break;
	default:
		Panic(EBadDisplayMode);
		break;
		}
	
	bmpHeader.iPaletteEntries = 0;
	bmpHeader.iCompression = ENoBitmapCompression;
	bmpHeader.iBitmapSize = bmpHeader.iStructSize + (byteWidth * iSourceRect.Size().iHeight);

	Mem::Copy(iDataPtr,&bmpHeader,sizeof(SEpocBitmapHeader));
	iDataPtr += sizeof(SEpocBitmapHeader);
	ASSERT(iDataPtr < iDataPtrLimit);

	iPaddingBytes = byteWidth - iCodecProc->PixelsToBytes(iSourceRect.Size().iWidth);

	aDst.SetLength(iDataPtr - destStartPtr);
	}

void CMbmWriteCodec::DoProcessL(const CFbsBitmap& aFrame)
	{
	TUint8* safeDataPtrLimit = iDataPtrLimit - 2;
	while (iDataPtr < safeDataPtrLimit)
		{
		while (iBytesToWrite > 0)
			{
			*iDataPtr++ = 0;
			iBytesToWrite--;
			if (iDataPtr == iDataPtrLimit)
				break;
			}

		if (iPos.iY >= iSourceRect.iBr.iY)
			break;

		TInt scanLength = Min(iSourceRect.iBr.iX - iPos.iX,iCodecProc->BytesToPixels(iDataPtrLimit - iDataPtr));
		TInt dstLength = iCodecProc->PixelsToBytes(scanLength);
		TPtr8 dstBuf(iDataPtr,dstLength,dstLength);

		aFrame.GetScanLine(dstBuf,iPos,scanLength,iDisplayMode);

		iPos.iX += scanLength;
		iDataPtr += dstLength;

		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY++;
			iBytesToWrite = iPaddingBytes;
			}
		}
	}

