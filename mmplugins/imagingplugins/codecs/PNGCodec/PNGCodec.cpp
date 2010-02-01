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
#include "ImageClientMain.h"
#include "ImageUtils.h"
#include "PNGCodec.h"

// Constants.
const TInt KTwipsPerMeter = 56693;


// Helper classes.
// TPngImageInformation
TPngImageInformation::TPngImageInformation()
	{
	iSize.SetSize(0,0);
	iBitDepth = 0;
	iColorType = EGrayscale;
	iCompressionMethod = EDeflateInflate32K;
	iFilterMethod = EAdaptiveFiltering;
	iInterlaceMethod = ENoInterlace;
	iPalettePresent = EFalse;

#if defined(_DEBUG)
	ASSERT(sizeof(TRgb)==sizeof(TUint32)); // ie no new fields
	ASSERT(KPngMaxPLTESize%4==0);
#endif  // defined(_DEBUG)

	TRgb* palette=iPalette;
	TInt i=KPngMaxPLTESize>>2;
	do 
		{
		*palette++ = KRgbBlack;
		*palette++ = KRgbBlack;
		*palette++ = KRgbBlack;
		*palette++ = KRgbBlack;
		} while (--i);

	iBackgroundPresent = EFalse;
	iBackgroundColor = KRgbWhite;
	iPhysicalPresent = EFalse;
	iPhysicalUnits = EUnknownUnits;
	iPhysicalSize.SetSize(0,0);
	iTransparencyPresent = EFalse;
	Mem::Fill(iTransparencyValue,KPngMaxPLTESize,0xff);
	}


// CMdaPngReadCodec
CPngReadCodec::~CPngReadCodec()
	{
	delete iDecoder;
	delete iDecompressor;
	if (iOwnsImageProcessor)
		{
		delete iImageProc;
		}
	if (iOwnsMaskProcessor)
		{
		delete iMaskProc;
		}
	if (iOwnsFastProcessor)
		{
		delete iFastProc;
		}		
	}

CPngReadCodec* CPngReadCodec::NewL(MPngDecoder& aDecoderIFace)
	{
	CPngReadCodec* self = new(ELeave) CPngReadCodec(aDecoderIFace);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}


CPngReadCodec::CPngReadCodec(MPngDecoder& aDecoderIFace):	
	iDecoderIFace(aDecoderIFace), iNewPosition(0), iReadMore(EFalse)
	{
	}

void CPngReadCodec::ConstructL()
	{
	CImageMaskProcessorReadCodec::ConstructL();
	}

void CPngReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask)
	{
	CFbsBitmap& newFrame = aDestination;

	iChunkBytesRemaining = 0;
	iChunkId = KNullDesC8;
	iPreviousChunkReadFailed = EFalse;
	
	const TSize destinationSize(newFrame.SizeInPixels());
	TInt reductionFactor = ReductionFactor(iImageInfo.iSize, destinationSize);
	
	TBool fastProcessorMode = EFalse;
	
	CImageProcessor* imageProc = NULL;
	SetImageProcessor(NULL);
	
	CImageProcessor* maskProc = NULL;
	SetMaskProcessor(NULL);
	
	CFastProcessor* fastProc = NULL;
	SetFastProcessor(NULL);
	
	if (!SkipImageProcessor(aDestination))
		{
		imageProc = ImageProcessorUtility::NewImageProcessorL(newFrame, reductionFactor, ERgb, aDisableErrorDiffusion);
		SetImageProcessor(imageProc);
		imageProc->PrepareL(newFrame,iImageInfo.iSize);

		if ((iImageInfo.iTransparencyPresent || (iImageInfo.iColorType & TPngImageInformation::EAlphaChannelUsed))
			&& aDestinationMask)
			{
			maskProc = ImageProcessorUtility::NewImageProcessorL(*aDestinationMask, iImageInfo.iSize, ERgb, aDisableErrorDiffusion);
			SetMaskProcessor(maskProc);
			maskProc->PrepareL(*aDestinationMask,iImageInfo.iSize);
			// set mask to black so that unknown parts on streamed image are not drawn
			ClearBitmapL(*aDestinationMask, KRgbBlack);
			}
		}
	else
		{
		fastProc = CFastProcessor::NewL(iImageInfo, &aDestination, aDestinationMask, EFalse);
		SetFastProcessor(fastProc);
		fastProcessorMode = ETrue;
		}

	delete iDecoder;
	iDecoder = NULL;
	iDecoder = CPngReadSubCodec::NewL(imageProc,maskProc,iImageInfo, fastProc, fastProcessorMode);
	
	if (!iDecompressor)
		{
		iDecompressor = CEZDecompressor::NewL(*this);
		}
	else
		{
		iDecompressor->ResetL(*this);
		}

	if (!aDestinationMask)
		{
		// if no mask, clear destination for sensible behaviour on streamed partial images
		TRgb background = iImageInfo.iBackgroundPresent ? iImageInfo.iBackgroundColor : KRgbWhite;
		ClearBitmapL(aDestination, background);
		
		if (aDestination.DisplayMode() == EColor16MA && iFrameInfo->iFlags & TFrameInfo::ETransparencyPossible)
			{
			iDecoder->SetAlphaMode(ETrue);
			}
		}
	}

void CPngReadCodec::InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& /* aFrameImageData */)
	{
	ASSERT(aFrameSettings.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameSettings;
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CPngReadCodec::ProcessFrameHeaderL(TBufPtr8& aData)
	{
	const TUint8* startDataPtr = aData.Ptr();
	const TUint8* dataPtr = startDataPtr;
	const TUint8* dataPtrLimit = startDataPtr + aData.Length();

	
	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrameHeader)
		{
		if (dataPtr + KPngChunkLengthSize + KPngChunkIdSize + KPngIHDRChunkSize + KPngChunkCRCSize > dataPtrLimit)
			User::Leave(KErrUnderflow);

		TInt chunkLength = PtrReadUtil::ReadBigEndianUint32Inc(dataPtr);
		TPtrC8 chunkId(dataPtr,KPngChunkIdSize);

		if (chunkLength != KPngIHDRChunkSize || chunkId != KPngIHDRChunkId)
			User::Leave(KErrNotFound);

		dataPtr += KPngChunkIdSize;

		DoProcessIHDRL(dataPtr,chunkLength);

		dataPtr += KPngIHDRChunkSize + KPngChunkCRCSize;
		}

	//When there is not enough buffer to read the chunk length and chunk id,
	//the input buffer is not increased, the decoder will not be able to read more data,
	//the test will stuck in infinite loop.
	//To break the infinite loop, here it checkes whether it's reading the same data as last time,
	//if so, leaves.	
	if ((iPreviousDataPos == iNewPosition) && (iPreviousDataLength == aData.Length()))
		{
		User::Leave(KErrUnderflow);
		}
	
	TRAPD(err, DoProcessInfoL(dataPtr, dataPtrLimit));
			
	iPreviousDataPos = iNewPosition;
	iPreviousDataLength = aData.Length();

	if (err != KErrNone)
		{
		if (err == KErrNotFound)
			return EFrameComplete;
		User::Leave(err); // A real error occured
		}
	if(iReadMore)
		{
		iReadMore = EFalse;
		iNewPosition += dataPtr - startDataPtr;
		return EFrameIncompleteRepositionRequest;
		}
	aData.Shift(dataPtr - startDataPtr);

	iFrameInfo->iFrameCoordsInPixels.SetRect(TPoint(0,0),iImageInfo.iSize);
	iFrameInfo->iOverallSizeInPixels = iImageInfo.iSize;
	if (iImageInfo.iPhysicalPresent && iImageInfo.iPhysicalUnits == TPngImageInformation::EMeters)
		iFrameInfo->iFrameSizeInTwips = iImageInfo.iPhysicalSize;
	else
		iFrameInfo->iFrameSizeInTwips.SetSize(0,0);

	iFrameInfo->iBitsPerPixel = iImageInfo.iBitDepth;
	if (iImageInfo.iColorType & TPngImageInformation::EColorUsed 
		 && iImageInfo.iColorType != TPngImageInformation::EIndexedColor)
		iFrameInfo->iBitsPerPixel *= 3;
	
	iFrameInfo->iDelay = 0;
	iFrameInfo->iFlags = TFrameInfo::ECanDither;
	
	if (iImageInfo.iColorType & (TPngImageInformation::EPaletteUsed | TPngImageInformation::EColorUsed))
		iFrameInfo->iFlags |= TFrameInfo::EColor;
	
	if (iImageInfo.iColorType & TPngImageInformation::EAlphaChannelUsed)
		{
		iFrameInfo->iFlags |= TFrameInfo::ETransparencyPossible;
		iFrameInfo->iFlags |= TFrameInfo::EAlphaChannel;
		}
	else if (iImageInfo.iTransparencyPresent)
		iFrameInfo->iFlags |= TFrameInfo::ETransparencyPossible;

	const TInt bitsPerPixel = iFrameInfo->iBitsPerPixel;
	// the best mode for colour-indexed images is 16m
	if (iImageInfo.iColorType == TPngImageInformation::EIndexedColor)
		iFrameInfo->iFrameDisplayMode = EColor16M;
	else if(bitsPerPixel == 1)
		iFrameInfo->iFrameDisplayMode = EGray2;
	else if(bitsPerPixel == 2)
		iFrameInfo->iFrameDisplayMode = EGray4;
	else if((bitsPerPixel > 2)&&(bitsPerPixel <= 4))
		iFrameInfo->iFrameDisplayMode = EGray16;
	else if((bitsPerPixel > 4)&&(bitsPerPixel <= 8))
		iFrameInfo->iFrameDisplayMode = EGray256;
	else if((bitsPerPixel > 8)&&(bitsPerPixel <= 12))
		iFrameInfo->iFrameDisplayMode = EColor4K;
	else if((bitsPerPixel > 8) && (!(iFrameInfo->iFlags & TFrameInfo::EColor)))
		iFrameInfo->iFrameDisplayMode = EGray256;
	else if((bitsPerPixel > 12)&&(bitsPerPixel <= 16))
		iFrameInfo->iFrameDisplayMode = EColor64K;
	else if((bitsPerPixel > 16)&&(bitsPerPixel <= 48))
		iFrameInfo->iFrameDisplayMode = EColor16M;
	else
		User::Leave(KErrCorrupt);


	if (iImageInfo.iBackgroundPresent)
		iFrameInfo->iBackgroundColor = iImageInfo.iBackgroundColor;

	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
	return EFrameComplete;
	}

TFrameState CPngReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	CImageProcessor*const imageProc = ImageProcessor();
	CImageProcessor*const maskProc = MaskProcessor();

	
	TUint8* startDataPtr = CONST_CAST(TUint8*,aSrc.Ptr());
	TUint8* dataPtr = startDataPtr;
	const TUint8* dataPtrLimit = dataPtr + aSrc.Length();
	TBool chunkReadFailed = EFalse;
	while (dataPtr < dataPtrLimit)
		{
		if (iChunkBytesRemaining == 0)
			{
			if (iChunkId != KNullDesC8)
				{
				// this is not the first chunk - need to skip the previous chunk's CRC
				// as well as reading this chunk's length and id
				if (dataPtr + KPngChunkCRCSize + KPngChunkLengthSize + KPngChunkIdSize > dataPtrLimit)
					{
					// not enough data
					chunkReadFailed = ETrue;
					break;
					}

				dataPtr += KPngChunkCRCSize;
				}
			else
				{				
				if (dataPtr + KPngChunkLengthSize + KPngChunkIdSize > dataPtrLimit)
					{
					// not enough data
					break;
					}
				}

			// read the current chunk's length and id
			iChunkBytesRemaining = PtrReadUtil::ReadBigEndianUint32Inc(const_cast<const TUint8*&>(dataPtr));
			
			if(iChunkBytesRemaining < 0 )
			    {
			    User::Leave(KErrCorrupt);
			    }
			
			iChunkId = TPtr8(dataPtr,KPngChunkIdSize,KPngChunkIdSize);
			dataPtr += KPngChunkIdSize;
			}

		if (iChunkId == KPngIDATChunkId)
			{
			if(SetupProcessData(aSrc, dataPtr, const_cast<const TUint8*&>(dataPtr), dataPtrLimit))
				iDecoderIFace.GoToProcessDataState();
			break;
			}
		else if (iChunkId == KPngIENDChunkId)
			{
			iDecompressor->InflateL();
			if (imageProc)
				{
				imageProc->FlushPixels();	
				}
			if (maskProc)
				{
				maskProc->FlushPixels();	
				}
			return EFrameComplete;
			}
		else // Skip other chunks
			{
			TInt bytesLeft = dataPtrLimit - dataPtr;
			if (bytesLeft >= iChunkBytesRemaining)
				{
				dataPtr += iChunkBytesRemaining;
				iChunkBytesRemaining = 0;
				}
			else
				{
				dataPtr += bytesLeft;
				iChunkBytesRemaining -= bytesLeft;
				}
			}
		}

	// allow decode of png files with missing IEND chunks
	if (!iMissingiENDChunkFail && chunkReadFailed && iPreviousChunkReadFailed)
		{
		// we're completely out of data but have finished a whole chunk
		// try to decode the image
		TBool moreDataNeeded = EFalse;
		TRAPD(err, moreDataNeeded = iDecompressor->InflateL());
		if ((err == KErrNone) && !moreDataNeeded)
			{
			if(imageProc)
				{
			 	imageProc->FlushPixels();
				}			
			if (maskProc)
				{
				maskProc->FlushPixels();
				}

			return EFrameComplete;
			}
		}
	iPreviousChunkReadFailed = chunkReadFailed;
	
	aSrc.Shift(dataPtr - startDataPtr);
	return EFrameIncomplete;
	}

void CPngReadCodec::DoProcessInfoL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit)
	{
	FOREVER
		{
		if (aDataPtr + KPngChunkLengthSize + KPngChunkIdSize > aDataPtrLimit) // Check there is enough data to read the chunk length
			{
			iNewPosition = 0 ;
			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame);
			iReadMore = ETrue;
			return;
			}
		TInt chunkLength = PtrReadUtil::ReadBigEndianUint32Inc(aDataPtr);
		TPtrC8 chunkId (&aDataPtr[0],KPngChunkIdSize);

		if (chunkId == KPngIDATChunkId)
			{
			aDataPtr -= KPngChunkLengthSize; // Rewind to start of chunkLength
			break;
			}

		if (aDataPtr + KPngChunkIdSize + chunkLength + KPngChunkCRCSize > aDataPtrLimit
			 || chunkLength < 0) // Check there is enough data to read the whole chunk
			{
			if (	chunkId == KPngPLTEChunkId || chunkId == KPngbKGDChunkId ||
					chunkId == KPngpHYsChunkId || chunkId == KPngtRNSChunkId )
				{
				aDataPtr -= KPngChunkLengthSize; // Rewind to start of chunkLength
				User::Leave(chunkLength < 0 ? KErrCorrupt : KErrUnderflow);
				}
			iNewPosition = KPngChunkIdSize + chunkLength + KPngChunkCRCSize;
			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame);
			iReadMore = ETrue;
			return;
			}

		aDataPtr += KPngChunkIdSize;

		if (chunkId == KPngPLTEChunkId)
			DoProcessPLTEL(aDataPtr,chunkLength);
		else if (chunkId == KPngbKGDChunkId)
			DoProcessbKGDL(aDataPtr,chunkLength);
		else if (chunkId == KPngpHYsChunkId)
			DoProcesspHYsL(aDataPtr,chunkLength);
		else if (chunkId == KPngtRNSChunkId)
			DoProcesstRNSL(aDataPtr,chunkLength);
		else if (chunkId == KPngIHDRChunkId || chunkId == KPngIENDChunkId)
			User::Leave(KErrCorrupt);

		aDataPtr += chunkLength;
		PtrReadUtil::ReadBigEndianUint32Inc(aDataPtr); // Skip crc value
		}
	}

void CPngReadCodec::DoProcessIHDRL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	if (aChunkLength != KPngIHDRChunkSize)
		User::Leave(KErrCorrupt);
	
	iImageInfo.iSize.iWidth = PtrReadUtil::ReadBigEndianUint32Inc(aDataPtr);
	iImageInfo.iSize.iHeight = PtrReadUtil::ReadBigEndianUint32Inc(aDataPtr);
	iImageInfo.iBitDepth = aDataPtr[0];
	iImageInfo.iColorType = TPngImageInformation::TColorType(aDataPtr[1]);
	iImageInfo.iCompressionMethod = TPngImageInformation::TCompressionMethod(aDataPtr[2]);
	iImageInfo.iFilterMethod = TPngImageInformation::TFilterMethod(aDataPtr[3]);
	iImageInfo.iInterlaceMethod = TPngImageInformation::TInterlaceMethod(aDataPtr[4]);
	
	// Check is one of the PNG formats we support
	if (iImageInfo.iSize.iWidth < 1 || iImageInfo.iSize.iHeight < 1
		|| iImageInfo.iCompressionMethod != TPngImageInformation::EDeflateInflate32K
		|| iImageInfo.iFilterMethod != TPngImageInformation::EAdaptiveFiltering
		|| (iImageInfo.iInterlaceMethod != TPngImageInformation::ENoInterlace &&
		iImageInfo.iInterlaceMethod != TPngImageInformation::EAdam7Interlace))
		User::Leave(KErrCorrupt);
	
	/*
		Check if color type and bit depths are valid. 
		PNG image type		Color type		Allowed bit depths
		--------------      -----------     ------------------
		Greyscale				0				1, 2, 4, 8, 16
		Truecolour				2				8, 16
		Indexed-colour			3				1, 2, 4, 8
		Greyscale with alpha	4				8, 16
		Truecolour with alpha	6				8, 16
		
		  (See http://www.w3.org/TR/PNG/#11IHDR)
	*/
	switch(iImageInfo.iColorType)
		{
		case TPngImageInformation::EGrayscale: // 0
			{
				if (! (iImageInfo.iBitDepth == 1 
					|| iImageInfo.iBitDepth == 2 
					|| iImageInfo.iBitDepth == 4
					|| iImageInfo.iBitDepth == 8 
					|| iImageInfo.iBitDepth == 16))
				{
					User::Leave(KErrCorrupt); // Invalid bit depth for color type 0
				}
			}
			break;
		case TPngImageInformation::EDirectColor: // 2
		case TPngImageInformation::EAlphaGrayscale: // 4
		case TPngImageInformation::EAlphaDirectColor: // 6
			{
				if (! (iImageInfo.iBitDepth == 8 
					|| iImageInfo.iBitDepth == 16))
				{
					User::Leave(KErrCorrupt); // Invalid bit depth for color type 2 or 4 or 6
				}
			}
			break;
		case TPngImageInformation::EIndexedColor: //3
			{
				if (! (iImageInfo.iBitDepth == 1 
					|| iImageInfo.iBitDepth == 2 
					|| iImageInfo.iBitDepth == 4
					|| iImageInfo.iBitDepth == 8))
				{
					User::Leave(KErrCorrupt); // Invalid bit depth for color type 3
				}
			}
			break;
		default: // Invalid color depth.
			{
				User::Leave(KErrCorrupt);
			}
		}
	}

void CPngReadCodec::DoProcessPLTEL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	const TInt paletteEntries = aChunkLength / 3;

	if ((aChunkLength % 3 != 0)||(paletteEntries > KPngMaxPLTESize))
		User::Leave(KErrCorrupt);

	iImageInfo.iPalettePresent = ETrue;

	const TUint8* dataPtrLimit = aDataPtr + aChunkLength;
	TRgb* palettePtr = iImageInfo.iPalette;

	while (aDataPtr < dataPtrLimit)
		{
		*palettePtr++ = TRgb(aDataPtr[0],aDataPtr[1],aDataPtr[2]);
		aDataPtr += 3;
		}
	}

void CPngReadCodec::DoProcessbKGDL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	iImageInfo.iBackgroundPresent = ETrue;
	if (iImageInfo.iColorType == TPngImageInformation::EIndexedColor) // 3
		{
		if (aChunkLength < 1)
			User::Leave(KErrCorrupt);

		iImageInfo.iBackgroundColor = iImageInfo.iPalette[aDataPtr[0]];
		}
	else if (iImageInfo.iColorType & TPngImageInformation::EColorUsed) // 2 & 6
		{
		if (aChunkLength < 6)
			User::Leave(KErrCorrupt);

		TInt red = PtrReadUtil::ReadBigEndianUint16(&aDataPtr[0]);
		TInt green = PtrReadUtil::ReadBigEndianUint16(&aDataPtr[2]);
		TInt blue = PtrReadUtil::ReadBigEndianUint16(&aDataPtr[4]);

		//Allow negative shift on 48 bpp images
		TInt offset = 8-iImageInfo.iBitDepth;
		if(offset > 0)
			{
			red <<= offset;
			green <<= offset;
			blue <<= offset;
			}
		else
			{
			offset = -offset;
			red >>= offset;
			green >>= offset;
			blue >>= offset;
			}

		iImageInfo.iBackgroundColor = TRgb(red,green,blue);
		}
	else
		{
		// Monochome images (iColorType 0 & 4)
		ASSERT((iImageInfo.iColorType == TPngImageInformation::EGrayscale) || (iImageInfo.iColorType == TPngImageInformation::EAlphaGrayscale));
		if (aChunkLength < 2)
			User::Leave(KErrCorrupt);

		TInt grayLevel = PtrReadUtil::ReadBigEndianUint16(aDataPtr);
		switch (iImageInfo.iBitDepth)
			{
			case 16:
				grayLevel >>= 8;
				iImageInfo.iBackgroundColor = TRgb::Gray256(grayLevel);
				break;

			case 8:
				iImageInfo.iBackgroundColor = TRgb::Gray256(grayLevel);
				break;

			case 4:
				iImageInfo.iBackgroundColor = TRgb::Gray16(grayLevel);
				break;

			case 2:
				iImageInfo.iBackgroundColor = TRgb::Gray4(grayLevel);
				break;

			case 1:
				iImageInfo.iBackgroundColor = TRgb::Gray2(grayLevel);
				break;

			default:
				ASSERT(0);
			}
		}
	}

void CPngReadCodec::DoProcesspHYsL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	if (aChunkLength != KPngpHYsChunkSize)
		User::Leave(KErrCorrupt);

	iImageInfo.iPhysicalUnits = TPngImageInformation::TPhysicalUnits(aDataPtr[8]);

	if (iImageInfo.iPhysicalUnits == TPngImageInformation::EMeters)
		{
		iImageInfo.iPhysicalPresent = ETrue;

		TInt horzPixelsPerMeter = PtrReadUtil::ReadBigEndianUint32Inc(aDataPtr);
		TInt vertPixelsPerMeter = PtrReadUtil::ReadBigEndianUint32Inc(aDataPtr);

		if (horzPixelsPerMeter > 0)
			iImageInfo.iPhysicalSize.iWidth = iImageInfo.iSize.iWidth * KTwipsPerMeter / horzPixelsPerMeter;
		if (vertPixelsPerMeter > 0)
			iImageInfo.iPhysicalSize.iHeight = iImageInfo.iSize.iHeight * KTwipsPerMeter / vertPixelsPerMeter;
		}
	}

void CPngReadCodec::DoProcesstRNSL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	iImageInfo.iTransparencyPresent = ETrue;

	if (iImageInfo.iColorType == TPngImageInformation::EIndexedColor) // 3
		{
#if defined(_DEBUG)
	ASSERT(sizeof(iImageInfo.iTransparencyValue)>sizeof(TAny*)); // ie it is not an allocated ptr.
#endif  // defined
		if (aChunkLength < 1 || TInt(sizeof(iImageInfo.iTransparencyValue)) < aChunkLength)
			User::Leave(KErrCorrupt);

		Mem::Copy(iImageInfo.iTransparencyValue,aDataPtr,aChunkLength);
		}
	else if (iImageInfo.iColorType == TPngImageInformation::EGrayscale) // 0
		{
		if (aChunkLength < 2)
			User::Leave(KErrCorrupt);

		iImageInfo.iTransparentGray = TUint16((aDataPtr[0] << 8) | aDataPtr[1]);
		}
	else if (iImageInfo.iColorType == TPngImageInformation::EDirectColor) // 2
		{
		if (aChunkLength < 6)
			User::Leave(KErrCorrupt);

		iImageInfo.iTransparentRed = TUint16((aDataPtr[0] << 8) | aDataPtr[1]);
		iImageInfo.iTransparentGreen = TUint16((aDataPtr[2] << 8) | aDataPtr[3]);
		iImageInfo.iTransparentBlue = TUint16((aDataPtr[4] << 8) | aDataPtr[5]);
		}
	}

TBool CPngReadCodec::SetupProcessData(TBufPtr8& aSrc,
									 TUint8* aStartDataPtr,
									 const TUint8*& aDataPtr,
									 const TUint8* aDataPtrLimit)
	{
	iSavedSrc = &aSrc;
	iStartDataPtr = aStartDataPtr;
	TInt bytesToProcess = Min(aDataPtrLimit - aDataPtr,iChunkBytesRemaining);
	if(bytesToProcess<=0)
		return EFalse;
	iDataPtr = aDataPtr;
	iDataDes.Set(aDataPtr,bytesToProcess);
	iDecompressor->SetInput(iDataDes);
	return ETrue;
	}

const TInt KInflateLimit=4; // max times we try to Inflate on each call

TBool CPngReadCodec::DoProcessDataL()
	{	
	TInt bytesToProcess = iDataDes.Length(); // we stored this in SetupProcessData()
	ASSERT(bytesToProcess>0);
	TBool result = EFalse;
	TBool callAgain = ETrue;
	for (TInt count = 0; count < KInflateLimit &&
						 callAgain &&
						 iDecompressor->AvailIn() != 0; count++)
		{
		callAgain = iDecompressor->InflateL();
		}

	const TInt availData = iDecompressor->AvailIn();
	if (!availData)
		{
		// Run out of data, get next buffer
		iPreviousChunkReadFailed = ETrue;
		result = ETrue;

		// Advance the buffer
		iDataPtr += bytesToProcess - availData;
		iChunkBytesRemaining -= bytesToProcess - availData;
		iSavedSrc->Shift(iDataPtr - iStartDataPtr);
		}
	return result;
	}

void CPngReadCodec::InitializeL(CEZZStream& aZStream)
	{
	aZStream.SetOutput(iDecoder->FirstBuffer());
	}

void CPngReadCodec::NeedInputL(CEZZStream& /*aZStream*/)
	{
	}

void CPngReadCodec::NeedOutputL(CEZZStream& aZStream)
	{
	aZStream.SetOutput(iDecoder->DecodeL());
	}

void CPngReadCodec::FinalizeL(CEZZStream& aZStream)
	{
	TPtrC8 buffer(aZStream.OutputDescriptor());
	if(buffer.Length())
		iDecoder->DecodeL();
	}
void CPngReadCodec::GetNewDataPosition(TInt& aPosition, TInt&  /*aLength*/ )
	{
	aPosition += iNewPosition;
	}

void CPngReadCodec::SetMissingiENDChunkFail(TBool aValue)
	{
	iMissingiENDChunkFail = aValue;
	}

//Checks if Image processor is to be used or skipped. Returns ETrue if ImageProcessor is to be ignored.
TBool CPngReadCodec::SkipImageProcessor(CFbsBitmap& aDestination)
	{
	TBool skipImgProc = EFalse;
	
	// If the image is interlaced, has transparency or is required to be scaled, then use the generic image processors
	if(iImageInfo.iInterlaceMethod != TPngImageInformation::ENoInterlace || iImageInfo.iTransparencyPresent || aDestination.SizeInPixels().iWidth != iImageInfo.iSize.iWidth || aDestination.SizeInPixels().iHeight != iImageInfo.iSize.iHeight)
		{
		return EFalse;
		}
	
	TDisplayMode mode = aDestination.DisplayMode();
	// Skip ImageProcessor only when decoding 24 or 32 bpp images
	switch (iImageInfo.iBitDepth)
		{
		case 8:
			switch (iImageInfo.iColorType)
				{
				case TPngImageInformation::EDirectColor:
					if(EColor16MAP == mode || EColor16MA == mode || EColor16MU == mode || EColor16M == mode)
						{
						skipImgProc = ETrue;	
						}
					break;
				case TPngImageInformation::EAlphaDirectColor:
					if(EColor16MAP == mode || EColor16MA == mode || EColor16MU == mode)
						{
						skipImgProc = ETrue;	
						}
					break;			
				default:
					break;
				}
			break;
		default:
			break;
		}
		
	return skipImgProc;
	}

// CPngWriteCodec
CPngWriteCodec::CPngWriteCodec(CPngEncoder& aPlugin, TInt aBpp, TBool aColor, TBool aPaletted, TInt aCompressionLevel):
	iCompressionLevel(aCompressionLevel),
	iCompressorPtr(NULL, 0),
	iPlugin(aPlugin)
	{
	// Set bpp
	iImageInfo.iBitsPerPixel = aBpp;
	switch (aBpp)
		{
		case 1:
			iImageInfo.iBitDepth = 1;
			break;
		case 2:
			iImageInfo.iBitDepth = 2;
			break;
		case 4:
			iImageInfo.iBitDepth = 4;
			break;
		case 8:
		case 24:
			iImageInfo.iBitDepth = 8;
			break;
		default:
			break;
		}

	// Set color type
	if (aColor && aPaletted)
		iImageInfo.iColorType = TPngImageInformation::EIndexedColor;
	else if (aColor)
		iImageInfo.iColorType = TPngImageInformation::EDirectColor;
	else
		iImageInfo.iColorType = TPngImageInformation::EGrayscale;
	}

CPngWriteCodec::~CPngWriteCodec()
	{
	delete iCompressor;
	delete iEncoder;
	}

CPngWriteCodec* CPngWriteCodec::NewL(CPngEncoder& aPlugin, TInt aBpp, TBool aColor, TBool aPaletted, TInt aCompressionLevel)
{
	CPngWriteCodec* self = new(ELeave) CPngWriteCodec(aPlugin, aBpp, aColor, aPaletted, aCompressionLevel);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}



// from CImageWriteCodec
void CPngWriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource)
	{
	if (aDst.Length() == 0)
		User::Leave(KErrArgument);	// Not enough length for anything

	SetSource(&aSource);
	iDestStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = iDestStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	// Set image information
	const SEpocBitmapHeader& header = aSource.Header();
	iImageInfo.iSize = header.iSizeInPixels;

	switch (iImageInfo.iBitDepth)
		{
		case 1:
		case 2:
		case 4:
			if (iImageInfo.iColorType == TPngImageInformation::EDirectColor)
				{
				// Bit depths 1, 2 and 4 don't support RGB colour (color mode 2)
				// Must use paletted colour or greyscale
				User::Leave(KErrNotSupported);
				break;
				}
			// fall through to case 8
		case 8:
			break;
		default:
			User::Leave(KErrNotSupported);	// unsupported bit depth
			break;
		}

	iImageInfo.iCompressionMethod = TPngImageInformation::EDeflateInflate32K;
	iImageInfo.iFilterMethod = TPngImageInformation::EAdaptiveFiltering;
	iImageInfo.iInterlaceMethod = TPngImageInformation::ENoInterlace;

	// Create encoder
	if (iEncoder)
		{
		delete iEncoder;
		iEncoder = NULL;
		}
	iEncoder = CPngWriteSubCodec::NewL(iImageInfo, &aSource);

	// Create compressor
	if (iCompressor)
		{
		delete iCompressor;
		iCompressor = NULL;
		}
	iCompressor = CEZCompressor::NewL(*this, iCompressionLevel);

	// Initial encoder state
	if (iImageInfo.iColorType == TPngImageInformation::EIndexedColor)
		iEncoderState = EPngWritePLTE;	
	else
		iEncoderState = EPngInit;
	iCallAgain = ETrue;		// to make sure we call DeflateL

	// Write header
	User::LeaveIfError(WriteHeaderChunk(aDst));
	}

TFrameState CPngWriteCodec::ProcessFrameL(TBufPtr8& aDst)
	{
	if (aDst.MaxLength() == 0)
		User::Leave(KErrArgument);	// Not enough length for anything

	TFrameState state = EFrameIncomplete;
	iDestStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = iDestStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	// Set return buffer length to 0 initially
	aDst.SetLength(0);

	while (aDst.Length() == 0 && state != EFrameComplete)
		{
		// Loop round until we have some data to return or
		// the image is encoded
		switch (iEncoderState)
			{
			case EPngWritePLTE:
				WritePLTEChunk(aDst);
				break;
			case EPngInit:
				InitializeCompressorL(aDst);
				ASSERT(iEncoderState==EPngDeflate);
				break;
			case EPngDeflate:
			    SetCompressorOutputL(aDst);
				iPlugin.GoToProcessDataState();
				return state;
			case EPngWriteIDAT:
				WriteIDATChunk(aDst);
				ASSERT(iEncoderState==EPngDeflate || iEncoderState==EPngEndChunk);
				break;
			case EPngEndChunk:
				WriteEndChunk(aDst);
				state = EFrameComplete;
				break;
			default:
				ASSERT(EFalse);
				break;
			}
		}

	return state;
	}

void CPngWriteCodec::SetCompressorOutputL(TBufPtr8& aDst)
    {
	// Set ptr for compressed data
	const TInt dataLength = aDst.MaxLength() - KPngChunkLengthSize - KPngChunkIdSize - KPngChunkCRCSize;

	if(dataLength <= 0)
	    {
	    // this effectively means that framework is broken
	    // and can't provide encoder with sufficiently big buffer
	    ASSERT( EFalse );
	    User::Leave(KErrArgument);
	    }

	iCompressorPtr.Set(iDestPtr + KPngChunkIdSize + KPngChunkLengthSize, dataLength, dataLength);    
	iCompressor->SetOutput(iCompressorPtr);
    }

void CPngWriteCodec::InitializeCompressorL(TBufPtr8& aDst)
	{
	// Initialise input/output for compressor
	iCompressor->SetInput(iEncoder->EncodeL(iScanline));
	
	SetCompressorOutputL(aDst);
    iScanline++;
	iEncoderState = EPngDeflate;
	}

TBool CPngWriteCodec::DeflateEncodedDataL()
	{
	ASSERT(iCallAgain);
	iCallAgain = iCompressor->DeflateL();
	//finished when NeedOutputL() or FinalizeL() change the state to EPngWriteIDAT
	ASSERT(iEncoderState==EPngDeflate || iEncoderState==EPngWriteIDAT);
	return(iEncoderState!=EPngDeflate);
	}

void CPngWriteCodec::WritePLTEChunk(TBufPtr8& aDst)
	{
	ASSERT(iEncoder->Palette() &&
		   (iImageInfo.iColorType == TPngImageInformation::EIndexedColor ||
		    iImageInfo.iColorType == TPngImageInformation::EDirectColor ||
		    iImageInfo.iColorType == TPngImageInformation::EAlphaDirectColor));	// allowed color types for PLTE chunk

	// Get palette entries
	CPalette* palette = iEncoder->Palette();
	ASSERT(palette);
	const TInt count = palette->Entries();
	TUint8* ptr = iDestPtr + KPngChunkIdSize + KPngChunkLengthSize;
	TInt length = count * 3;
	TPtr8 data(ptr, length, length);
	for (TInt i=0; i < count; i++)
		{
		TRgb rgb = palette->GetEntry(i);
		*ptr = TUint8(rgb.Red());
		ptr++;
		*ptr = TUint8(rgb.Green());
		ptr++;
		*ptr = TUint8(rgb.Blue());
		ptr++;
		}
	// Write PLTE chunk
	WritePngChunk(iDestPtr, KPngPLTEChunkId, data, length);
	ASSERT(length % 3 == 0);	// length must be divisible by 3
	aDst.SetLength(length);
	iEncoderState = EPngInit;
	}

void CPngWriteCodec::WriteIDATChunk(TBufPtr8& aDst)
	{
	TPtrC8 ptr(iCompressor->OutputDescriptor());
	if (ptr.Length())
		{
		TInt length = 0;
		WritePngChunk(iDestPtr, KPngIDATChunkId, ptr, length);
		aDst.SetLength(length);

		// New output can write to the same compressor ptr
		iCompressor->SetOutput(iCompressorPtr);
		}

	if (iCallAgain)
		iEncoderState = EPngDeflate;
	else
		iEncoderState = EPngEndChunk;
	}

void CPngWriteCodec::WriteEndChunk(TBufPtr8& aDst)
	{
	// Write IEND chunk
	TInt length = 0;
	WritePngChunk(iDestPtr, KPngIENDChunkId, KNullDesC8, length);
	aDst.SetLength(length);
	}

TInt CPngWriteCodec::WriteHeaderChunk(TBufPtr8& aDst)
	{
	// Write signature
	Mem::Copy(iDestPtr, &KPngSignature[0], KPngFileSignatureLength);
	iDestPtr += KPngFileSignatureLength;

	// Write IHDR chunk
	TBuf8<KPngIHDRChunkSize> buffer;
	TUint8* ptr = const_cast<TUint8*>(buffer.Ptr());
	// Set length of data
	buffer.SetLength(KPngIHDRChunkSize);
	// Chunk data
	// width (4 bytes)
	if ((iImageInfo.iSize.iWidth == 0) ||
		(static_cast<TUint>(iImageInfo.iSize.iWidth) > KPngMaxImageSize))
		{
		return KErrArgument;	// invalid width
		}
	PtrWriteUtil::WriteBigEndianInt32(ptr, iImageInfo.iSize.iWidth);
	ptr += 4;
	// height (4 bytes)
	if ((iImageInfo.iSize.iHeight == 0) ||
		(static_cast<TUint>(iImageInfo.iSize.iHeight) > KPngMaxImageSize))
		{
		return KErrArgument;	// invalid height
		}
	PtrWriteUtil::WriteBigEndianInt32(ptr, iImageInfo.iSize.iHeight);
	ptr += 4;
	// bit depth (1 byte)
	PtrWriteUtil::WriteInt8(ptr, iImageInfo.iBitDepth);
	ptr++;
	// colour type (1 byte)
	PtrWriteUtil::WriteInt8(ptr, iImageInfo.iColorType);
	ptr++;
	// compression method (1 byte)
	PtrWriteUtil::WriteInt8(ptr, iImageInfo.iCompressionMethod);
	ptr++;
	// filter method (1 byte)
	PtrWriteUtil::WriteInt8(ptr, iImageInfo.iFilterMethod);
	ptr++;
	// interlace method (1 byte)
	PtrWriteUtil::WriteInt8(ptr, iImageInfo.iInterlaceMethod);
	ptr++;

	TInt length = 0;
	WritePngChunk(iDestPtr, KPngIHDRChunkId, buffer, length);
	aDst.SetLength(KPngFileSignatureLength + length);

	return KErrNone;
	}

void CPngWriteCodec::WritePngChunk(TUint8*& aDestPtr, const TDesC8& aChunkId, const TDesC8& aData, TInt& aLength)
	{
	// Chunk length (4 bytes)
	PtrWriteUtil::WriteBigEndianInt32(aDestPtr, aData.Length());
	aDestPtr += KPngChunkLengthSize;
	TUint8* crcPtr = aDestPtr;	// start position for calculating CRC
	// Chunk type (4 bytes)
	Mem::Copy(aDestPtr, aChunkId.Ptr(), KPngChunkIdSize);
	aDestPtr += KPngChunkIdSize;
	// Chunk data (0...n bytes)
	Mem::Copy(aDestPtr, aData.Ptr(), aData.Length());
	aDestPtr += aData.Length();
	// CRC (4 bytes)
	TUint32 crc = KPngCrcMask;
	GetCrc(crc, crcPtr, KPngChunkIdSize + aData.Length());
	crc ^= KPngCrcMask;
	PtrWriteUtil::WriteBigEndianInt32(aDestPtr, crc);
	aDestPtr += KPngChunkCRCSize;
	// Length of chunk
	aLength = KPngChunkLengthSize + KPngChunkIdSize + aData.Length() + KPngChunkCRCSize;
	}

// from MEZBufferManager
void CPngWriteCodec::InitializeL(CEZZStream& /*aZStream*/)
	{
	}

void CPngWriteCodec::NeedInputL(CEZZStream& aZStream)
	{
	// Give compressor more data from encoder
	aZStream.SetInput(iEncoder->EncodeL(iScanline));
	if (iCompressor->AvailIn() != 0)
		iScanline++;
	}

void CPngWriteCodec::NeedOutputL(CEZZStream& /*aZStream*/)
	{
	// Signal to write an IDAT chunk
	iEncoderState = EPngWriteIDAT;
	}

void CPngWriteCodec::FinalizeL(CEZZStream& /*aZStream*/)
	{
	// Signal to write an IDAT chunk
	iEncoderState = EPngWriteIDAT;
	}

// New functions
void CPngWriteCodec::GetCrc(TUint32& aCrc, const TUint8* aPtr, const TInt aLength)
	{
	if (!iCrcTableCalculated)
		CalcCrcTable();
	TUint32 code = aCrc;
	for (TInt i=0; i < aLength; i++)
		code = iCrcTable[(code ^ aPtr[i]) & 0xff] ^ (code >> 8);
	aCrc = code;
	}

void CPngWriteCodec::CalcCrcTable()
	{
	for (TInt i=0; i < KPngCrcTableLength; i++)
		{
		TUint32 code = static_cast<TUint32>(i);

		for (TInt j = 0; j < 8; j++)
			{
			if (code & 1)
				code = 0xedb88320 ^ (code >> 1);
			else
				code = code >> 1;
			}
		iCrcTable[i] = code;
		}
	iCrcTableCalculated = ETrue;
	}
