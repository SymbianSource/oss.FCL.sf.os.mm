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

#include <gdi.h>
#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <imageconversion.h>
#include "ImageClientMain.h"
#include "ImageUtils.h"
#include <101F45AE_extra.rsg>
#include "BMPConvert.h"
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif

_LIT(KBMPPanicCategory, "BMPConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KBMPPanicCategory, aError);
	}

void WriteTUint32(TUint8*& aPtr, TUint32 aValue)
	{
	Mem::Copy(aPtr, &aValue, 4);
	aPtr += 4;
	}

CBmpDecoder* CBmpDecoder::NewL()
	{
	return new(ELeave) CBmpDecoder;
	}

void CBmpDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeBMPUid;
	aImageSubType = KNullUid;
	}

CBmpDecoder::CBmpDecoder()
	{
	}

CBmpDecoder::~CBmpDecoder()
	{
	Cleanup();
	}

void CBmpDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec() == NULL);

	TSize size = iBitmapHeader.iSizeInPixels;
	TRgb* const palette = &(iPalette[0]);

	CBmpReadCodec* imageReadCodec = NULL;


	const TFrameInfo& imageInfo = ImageInfo();
	switch(imageInfo.iFrameDisplayMode)
		{
	case EGray2:
		imageReadCodec = CBmp1BppReadCodec::NewL(size, palette);
		break;
	case EColor16:
		if (iBitmapHeader.iCompression == TBmpHeader::EFourBppRLE)
			imageReadCodec = CBmpRLE4ReadCodec::NewL(size, palette);
		else
			imageReadCodec = CBmpNoComp4BppReadCodec::NewL(size, palette);
		break;
	case EColor256:
		if (iBitmapHeader.iCompression == TBmpHeader::EEightBppRLE)
			imageReadCodec = CBmpRLE8ReadCodec::NewL(size, palette);
		else
			imageReadCodec = CBmpNoComp8BppReadCodec::NewL(size, palette);
		break;
		
	case EColor64K:
		if (iBitmapHeader.iCompression == TBmpHeader::EBitFields)
			{
			imageReadCodec = CBmpBiRgbReadCodec::NewL(size, 2, palette);
			}
		else if (iBitmapHeader.iCompression == TBmpHeader::ENone)
			{
			imageReadCodec = CBmpBiRgbReadCodec::NewL(size, 2, NULL);
			}
		break;
		
	case EColor16M:
		if(iBitmapHeader.iCompression == TBmpHeader::ENone)
			{
			imageReadCodec = CBmp24BppReadCodec::NewL(size);
			}
		break;

	case EColor16MU:
		if (iBitmapHeader.iCompression == TBmpHeader::EBitFields)
			{
			imageReadCodec = CBmpBiRgbReadCodec::NewL(size, 4, palette);
			}
		else if (iBitmapHeader.iCompression == TBmpHeader::ENone)
			{
			imageReadCodec = CBmp32BppReadCodec::NewL(size);
			}
		break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}
	
	if (!imageReadCodec)
		{
		User::Leave(KErrNotSupported);
		}

	SetImageReadCodec(imageReadCodec);

	ReadFrameHeadersL();
	}

void CBmpDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, (KBmpFileHeaderSize + KBmpInfoHeaderV1Size));

	// Validate the header.
	if (bufferDes.Length() < (KBmpFileHeaderSize + KBmpInfoHeaderV1Size))
		User::Leave(KErrUnderflow);
	
	const TUint8* ptr = &bufferDes[0];
	TUint16 sig = *ptr++;
	sig |= (*ptr++ << 8);
	if (sig != KBmpFileSignature)
		User::Leave(KErrCorrupt);

	// Read data length from header
	TInt dataLength = PtrReadUtil::ReadUint32(ptr);
	// If the data length is negative then return KErrCorrupt. Strictly speaking, 
	// the data length is a 4-byte unsigned quantity (i.e. a TUint not a TInt) and 
	// we should treat it as such, but this would involve an API change to SetDataLength()
	// and changing iDataLength in CImageDecoderPriv.from a TInt to a TUint.
	// Anyway it's reasonable to assume that if the length is > 0x7FFFFFFF, then
	// the file is corrupt.
	if (dataLength < (KBmpFileHeaderSize + KBmpInfoHeaderV1Size))
		User::Leave(KErrCorrupt);

	ptr += 4; // Increment pointer by 4 bytes

	ptr += 4; // Skip bfReserved1 & bfReserved2

	// Read image data offset from header
	TInt imageDataOffset = PtrReadUtil::ReadUint32(ptr);
	ptr += 4; // Increment pointer by 4 bytes

	if(imageDataOffset < 0 || imageDataOffset > dataLength)
		User::Leave(KErrCorrupt);
	
	dataLength -= imageDataOffset;
	SetDataLength(dataLength);

	TUint32 infoHeaderSize = PtrReadUtil::ReadUint32(ptr);
	ptr += 4; // Skip biSize
	if ((infoHeaderSize < KBmpInfoHeaderV1Size) || (infoHeaderSize > KBmpInfoMaxHeaderV2Size))
		{
		User::Leave(KErrCorrupt);
		}

	// Read image dimensions in pixels from header
	if (infoHeaderSize == KBmpInfoHeaderV1Size)
		{
		iBitmapHeader.iSizeInPixels.iWidth = PtrReadUtil::ReadUint16(ptr);
		ptr += 2;

		iBitmapHeader.iSizeInPixels.iHeight = PtrReadUtil::ReadUint16(ptr);
		ptr += 2;
		}
	else
		{
		iBitmapHeader.iSizeInPixels.iWidth = PtrReadUtil::ReadUint32(ptr);
		ptr += 4; // Increment pointer by 4 bytes

		iBitmapHeader.iSizeInPixels.iHeight = PtrReadUtil::ReadUint32(ptr);
		ptr += 4; // Increment pointer by 4 bytes
		}

	// Check for valid image size
	if (iBitmapHeader.iSizeInPixels.iWidth <= 0 || iBitmapHeader.iSizeInPixels.iHeight == 0)
		User::Leave(KErrCorrupt);
	//check for unsupported negative height
	if(iBitmapHeader.iSizeInPixels.iHeight < 0)
		{
		User::Leave(KErrNotSupported);
		}

	if (infoHeaderSize > KBmpInfoHeaderV1Size)
		{
		// read the extra data associated with v2.x headers
		ReadDataL((KBmpFileHeaderSize + KBmpInfoHeaderV1Size), bufferDes, (infoHeaderSize - KBmpInfoHeaderV1Size));
		if (bufferDes.Length() < (infoHeaderSize - KBmpInfoHeaderV1Size))
			{
			User::Leave(KErrUnderflow);
			}
		ptr = &bufferDes[0];
		}
	
	ptr += 2; // Skip biPlanes

	// Read bits per pixel from header
	iBitmapHeader.iBitCount = PtrReadUtil::ReadUint16(ptr);
	ptr += 2; // Increment pointer by 2 bytes

	// Check for valid bit count
	if (iBitmapHeader.iBitCount < 1 || iBitmapHeader.iBitCount > 32)
		User::Leave(KErrCorrupt);
	TFrameInfo imageInfo;
	imageInfo = ImageInfo();
	imageInfo.iFrameSizeInTwips.iWidth = 0;
	imageInfo.iFrameSizeInTwips.iHeight = 0;
	TInt coloursUsed = 0;

	if (infoHeaderSize == KBmpInfoHeaderV1Size)
		{
		// number of colours used equals number of bytes between the bitmap header and the bitmap data
		// divided by the size of a single palette element
		coloursUsed = (imageDataOffset - KBmpFileHeaderSize - KBmpInfoHeaderV1Size) / KBmpSizeofPaletteEntryV1;
		}
	else
		{
		// Read compresion type from header
		TInt compression = PtrReadUtil::ReadUint32(ptr);
		ptr += 4; // Increment pointer by 4 bytes
		iBitmapHeader.iCompression = STATIC_CAST(TBmpHeader::TCompression, compression);

		// Check for valid compression type
		if (iBitmapHeader.iCompression != TBmpHeader::ENone &&
			iBitmapHeader.iCompression != TBmpHeader::EEightBppRLE &&
			iBitmapHeader.iCompression != TBmpHeader::EFourBppRLE &&
			iBitmapHeader.iCompression != TBmpHeader::EBitFields)
			User::Leave(KErrCorrupt);

		TBmpCompression* compressionImageData = new(ELeave) TBmpCompression;
		compressionImageData->iCompression = STATIC_CAST(TBmpCompression::TCompression, compression);
		CleanupStack::PushL(compressionImageData);
	
		User::LeaveIfError(AppendImageData(compressionImageData));
		CleanupStack::Pop();

		//do not use image data size since it cannot be trusted
		ptr += 4; // Skip biSizeImage

		// Read Horizontal and vertical resolution
		TInt xPelsPerMeter = PtrReadUtil::ReadInt32(ptr);
		ptr += 4; // Increment pointer by 4 bytes
	
		TInt yPelsPerMeter = PtrReadUtil::ReadInt32(ptr);
		ptr += 4; // Increment pointer by 4 bytes

		TZoomFactor tempImageDevice;
	
		if (xPelsPerMeter > 0)
			{
			TInt64 twips = TInt64(iBitmapHeader.iSizeInPixels.iWidth) / TInt64(xPelsPerMeter);
			twips *= 7200000;
			twips /= 127;
			imageInfo.iFrameSizeInTwips.iWidth = I64LOW(twips);
			}
	
		if (yPelsPerMeter > 0)
			{
			TInt64 twips = TInt64(iBitmapHeader.iSizeInPixels.iHeight) / TInt64(yPelsPerMeter);
			twips *= 7200000;
			twips /= 127;
			imageInfo.iFrameSizeInTwips.iHeight = I64LOW(twips);
			}
	
		// Read colour count
		coloursUsed = PtrReadUtil::ReadUint32(ptr); // read biClrUsed
		ptr += 4; // Increment pointer by 4 bytes
		}
	if (coloursUsed == 0)
		{
		if (iBitmapHeader.iBitCount == 1)
			{
			iBitmapHeader.iPaletteEntries = 2;
			}
		else if (iBitmapHeader.iBitCount == 4)
			{
			iBitmapHeader.iPaletteEntries = 16;
			}
		else if (iBitmapHeader.iBitCount == 8)
			{
			iBitmapHeader.iPaletteEntries = 256;
			}
		else
			{
			iBitmapHeader.iPaletteEntries = 0; // 16bpp/24bpp/32bpp
			}
		}
	else
		{
		if (iBitmapHeader.iBitCount == 24)
			{
			iBitmapHeader.iPaletteEntries = 0; // biClrUsed ignored for 24bpp
			}
		else
			{
			iBitmapHeader.iPaletteEntries = coloursUsed;
			}
		}
		

	// Check for valid palette entries.
	if (iBitmapHeader.iPaletteEntries < 0 || iBitmapHeader.iPaletteEntries > KBmpMaxPaletteEntries)
		{
		User::Leave(KErrCorrupt);
		}

	imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), iBitmapHeader.iSizeInPixels);
	imageInfo.iOverallSizeInPixels = iBitmapHeader.iSizeInPixels;
	imageInfo.iBitsPerPixel = iBitmapHeader.iBitCount;
	imageInfo.iDelay = 0;
	imageInfo.iFlags = TFrameInfo::ECanDither;


	if (imageInfo.iBitsPerPixel <= 0)
		User::Leave(KErrCorrupt);

	if (imageInfo.iBitsPerPixel == 1)
		{
		imageInfo.iFrameDisplayMode = EGray2;
		}
	else
		{
		imageInfo.iFlags |= TFrameInfo::EColor;
		if (imageInfo.iBitsPerPixel == 4)
			{
			imageInfo.iFrameDisplayMode = EColor16;
			}
		else if (imageInfo.iBitsPerPixel == 8)
			{
			imageInfo.iFrameDisplayMode = EColor256;
			}
		else if (imageInfo.iBitsPerPixel == 16)
			{
			imageInfo.iFrameDisplayMode = EColor64K;		
			}
		else if (imageInfo.iBitsPerPixel == 24)
			{
			imageInfo.iFrameDisplayMode = EColor16M;
			}
		else if (imageInfo.iBitsPerPixel == 32)
			{
			imageInfo.iFrameDisplayMode = EColor16MU;
			}
		else
			{
			User::Leave(KErrCorrupt);
			}
		}
		
	// in case of EBitFields image type we have to read the channel masks		
	// EBitFields is not supported in V1
	if ((infoHeaderSize > KBmpInfoHeaderV1Size) && (iBitmapHeader.iCompression == TBmpHeader::EBitFields))
		{
		TInt numMaskBytes = imageDataOffset - (KBmpFileHeaderSize + infoHeaderSize);
		
		if ((numMaskBytes <= 0) && (infoHeaderSize != KBmpInfoDefaultHeaderV2Size))
			{
			// infoHeaderSize is possibly incorrect - correct to standard size
			infoHeaderSize = KBmpInfoDefaultHeaderV2Size;
			numMaskBytes = imageDataOffset - (KBmpFileHeaderSize + infoHeaderSize);
			}
			
		if (iBitmapHeader.iPaletteEntries || numMaskBytes <= 0 || 
				(numMaskBytes % KBmpSizeofPaletteEntryV2) != 0)
			{
			User::Leave(KErrCorrupt);
			}
		iBitmapHeader.iPaletteEntries = numMaskBytes / KBmpSizeofPaletteEntryV2;
		}
		
	if (iBitmapHeader.iPaletteEntries > KBmpMaxPaletteEntries)
		{
		User::Leave(KErrCorrupt);
		}
	
	if (iBitmapHeader.iPaletteEntries > 0)
		{
		// Read the palette.
		TInt paletteLength;
		if (infoHeaderSize == KBmpInfoHeaderV1Size)
			{
			paletteLength = iBitmapHeader.iPaletteEntries * KBmpSizeofPaletteEntryV1;
			}
		else
			{
			paletteLength = iBitmapHeader.iPaletteEntries * KBmpSizeofPaletteEntryV2;
			}

		ReadDataL((KBmpFileHeaderSize + infoHeaderSize), bufferDes, paletteLength);

		if (bufferDes.Length() < paletteLength)
			User::Leave(KErrUnderflow);

		const TUint8* ptr = &bufferDes[0];
		for (TInt count = 0; count < iBitmapHeader.iPaletteEntries; count++)
			{
			TInt blue = ptr[0];
			TInt green = ptr[1];
			TInt red = ptr[2];
			
			// Bitmaps with a palette do not contain an alpha channel
			iPalette[count] = TRgb(red, green, blue);
			ptr += (infoHeaderSize == KBmpInfoHeaderV1Size ?
					KBmpSizeofPaletteEntryV1 :
					KBmpSizeofPaletteEntryV2); // Skips RGBQUAD reserved value which is mandated 0x00
			}
		}
 
	SetImageInfo(imageInfo);
	SetStartPosition(imageDataOffset);
	}

CFrameInfoStrings* CBmpDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	const TUid KBmpCodecDllUid = {KBMPCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs, KBmpCodecDllUid, resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	const CFrameImageData& frameData = FrameData(aFrameNumber); 
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();
	const TBmpCompression* compressionImageData = STATIC_CAST(const TBmpCompression*, frameData.GetImageData(0));

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetFormatL(info);

	TInt width = frameInfo.iOverallSizeInPixels.iWidth;
	TInt height = frameInfo.iOverallSizeInPixels.iHeight;
	TInt depth = frameInfo.iBitsPerPixel;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex = (frameInfo.iFlags & TFrameInfo::EColor) ? 1 : 0;
	templte = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	info.Format(templte, depth);
	frameInfoStrings->SetDepthL(info);

	resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	formatIndex = (compressionImageData->iCompression) ? 1 : 0;
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetDetailsL(info);
	
	CleanupStack::Pop(frameInfoStrings);
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

// Bmp encoder
CBmpEncoder* CBmpEncoder::NewL()
	{
	return new(ELeave) CBmpEncoder;
	}

CBmpEncoder::CBmpEncoder()
	{
	}

CBmpEncoder::~CBmpEncoder()
	{
	Cleanup();
	}

void CBmpEncoder::Cleanup()
	{
	// Delete any objects we should get rid of
	delete iReadBuffer; iReadBuffer = NULL; // Created in DoConvert()
	// Base class included
	CImageEncoderPlugin::Cleanup();
	}

void CBmpEncoder::PrepareEncoderL(const CFrameImageData* aFrameImageData)
	{
	iBitmapHeader.iBitCount = 0;

	TInt count = (aFrameImageData) ? aFrameImageData->ImageDataCount() : 0;

	if (count == 0)
		iBitmapHeader.iBitCount = 24;	// default value

	for (TInt index = 0 ; index<count ; index++)
		{	
		const TImageDataBlock* encoderData = aFrameImageData->GetImageData(index);
		if (encoderData->DataType() == KBMPImageDataUid)
			{
			if (iBitmapHeader.iBitCount != 0)
				User::Leave(KErrCorrupt);

			const TBmpImageData* bmpImageData = STATIC_CAST(const TBmpImageData*, encoderData);
			iBitmapHeader.iBitCount = bmpImageData->iBitsPerPixel;
			}
		else
			User::Leave(KErrCorrupt);
		}

	if (iBitmapHeader.iBitCount == 0)
		User::Leave(KErrCorrupt);

	ASSERT(ImageWriteCodec() == NULL);

	CBmpWriteCodec* imageWriteCodec = NULL;
	if (iBitmapHeader.iBitCount==24)
		{
		imageWriteCodec = CBmp24BppWriteCodec::NewL();
		iBitmapHeader.iPaletteEntries = 0;
		}
	else if (iBitmapHeader.iBitCount==8)
		{
		imageWriteCodec =  CBmp8BppWriteCodec::NewL();
		iBitmapHeader.iPaletteEntries = 256;
		}
	else if (iBitmapHeader.iBitCount==4)
		{
		imageWriteCodec =  CBmp4BppWriteCodec::NewL();
		iBitmapHeader.iPaletteEntries = 16;
		}
	else if (iBitmapHeader.iBitCount==1)
		{
		imageWriteCodec =  CBmp1BppWriteCodec::NewL();
		iBitmapHeader.iPaletteEntries = 2;
		}
	else
		User::Leave(KErrNotSupported);

	SetImageWriteCodec(imageWriteCodec);

	StartPosition() = KBmpFileHeaderSize + KBmpInfoDefaultHeaderV2Size + (iBitmapHeader.iPaletteEntries * KBmpSizeofPaletteEntryV2);
	iBitmapHeader.iCompression = TBmpHeader::ENone;
	}

void CBmpEncoder::UpdateHeaderL()
	{
	TInt headerSize = KBmpFileHeaderSize + KBmpInfoDefaultHeaderV2Size + (iBitmapHeader.iPaletteEntries * KBmpSizeofPaletteEntryV2);
	if (iReadBuffer && (iReadBuffer->Length() != headerSize))
		{
		delete iReadBuffer;
		iReadBuffer = NULL;
		}
	if (!iReadBuffer)
		iReadBuffer = HBufC8::NewMaxL(headerSize);
	TUint8* headerPtr = &iReadBuffer->Des()[0];

	// BITMAPFILEHEADER
	*headerPtr++ = TUint8(KBmpFileSignature & 0xff);
	*headerPtr++ = TUint8(KBmpFileSignature >> 8);

	TInt imageSize = CurrentImageSizeL();
	WriteTUint32(headerPtr, imageSize);
	WriteTUint32(headerPtr, 0);
	WriteTUint32(headerPtr, headerSize);

	// BITMAPINFOHEADER
	WriteTUint32(headerPtr, KBmpInfoDefaultHeaderV2Size); // biSize
	WriteTUint32(headerPtr, FrameInfoOverallSizeInPixels().iWidth); // biWidth
	WriteTUint32(headerPtr, FrameInfoOverallSizeInPixels().iHeight); // biHeight
	WriteTUint32(headerPtr, 1 | (iBitmapHeader.iBitCount << 16)); // biPlanes | (biBitCount << 16)
	WriteTUint32(headerPtr, 0); // biCompression
	WriteTUint32(headerPtr, 0); // biSizeImage
	WriteTUint32(headerPtr, 0); // biXPelsPerMetre
	WriteTUint32(headerPtr, 0); // biYPelsPerMetre
	WriteTUint32(headerPtr, 0); // biClrUsed
	WriteTUint32(headerPtr, 0); // biClrImportant

	// RGBQUAD
	if (iBitmapHeader.iBitCount == 1)
		{
		for (TInt count = 0; count < iBitmapHeader.iPaletteEntries; count++)
			{
			TRgb color(TRgb::Gray2(count));
			headerPtr[0] = TUint8(color.Blue());
			headerPtr[1] = TUint8(color.Green());
			headerPtr[2] = TUint8(color.Red());
			headerPtr[3] = 0;
			headerPtr+=4;
			}
		}
	if (iBitmapHeader.iBitCount == 4)
		{
		for (TInt count = 0; count < iBitmapHeader.iPaletteEntries; count++)
			{
			TRgb color(TRgb::Color16(count));
			headerPtr[0] = TUint8(color.Blue());
			headerPtr[1] = TUint8(color.Green());
			headerPtr[2] = TUint8(color.Red());
			headerPtr[3] = 0;
			headerPtr+=4;
			}
		}
	else if (iBitmapHeader.iBitCount == 8)
		{
		for (TInt count = 0; count < iBitmapHeader.iPaletteEntries; count++)
			{
			TRgb color(TRgb::Color256(count));
			headerPtr[0] = TUint8(color.Blue());
			headerPtr[1] = TUint8(color.Green());
			headerPtr[2] = TUint8(color.Red());
			headerPtr[3] = 0;
			headerPtr+=4;
			}
		}

	TPtr8 bufferDes(iReadBuffer->Des());
	WriteDataL(0,bufferDes);
	}

