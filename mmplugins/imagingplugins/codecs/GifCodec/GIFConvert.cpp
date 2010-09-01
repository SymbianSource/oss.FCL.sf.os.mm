// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Licensed under US Patent No 4,558,302 and foreign counterparts
// 
//

#include <imageconversion.h>
#include "ImageClientMain.h"
#include "ImageUtils.h"
#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <101F45B1_extra.rsg>
#include "GIFConvert.h"
#include "icl/ICL_UIDS.hrh"
#include <gifscaler.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif

_LIT(KGIFPanicCategory, "GIFConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KGIFPanicCategory, aError);
	}

CGifDecoder* CGifDecoder::NewL()
	{
	return new(ELeave) CGifDecoder;
	}

CGifDecoder::CGifDecoder()
	{
	}

CGifDecoder::~CGifDecoder()
	{
	CImageDecoderPlugin::Cleanup();
	}

void CGifDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < NumberOfFrames()), Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeGIFUid;
	aImageSubType = KNullUid;
	}

TInt CGifDecoder::NumberOfImageComments() const
	{
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	const CFrameImageData& frameImageData = FrameData(0);
	TInt imageCommentCount = 0;
	const TInt imageDataCount = frameImageData.ImageDataCount();
	for (TInt count = 0; count < imageDataCount; count++)
		{
		const TImageDataBlock* imageData = frameImageData.GetImageData(count);
		if (imageData->DataType() == KGIFCommentUid)
			imageCommentCount++;
		}

	return imageCommentCount;
	}

HBufC* CGifDecoder::ImageCommentL(TInt aCommentNumber) const
	{
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	__ASSERT_ALWAYS((aCommentNumber >= 0) && (aCommentNumber < NumberOfImageComments()), Panic(ECommentNumberOutOfRange));

	const CFrameImageData& frameImageData = FrameData(0);
	TInt commentCount = 0;
	TInt imageDataCount = frameImageData.ImageDataCount();
	const TImageDataBlock* imageData = NULL;
	for (TInt count = 0; count < imageDataCount; count++)
		{
		imageData = frameImageData.GetImageData(count);
		if (imageData->DataType() == KGIFCommentUid)
			{
			if (commentCount == aCommentNumber)
				{
				break;
				}
			commentCount++;
			}
		}

	const TGifComment* gifComment = STATIC_CAST(const TGifComment*,imageData);
	HBufC* comment = NULL;
	if (gifComment)
		{
		comment = HBufC::NewL(gifComment->iComment->Length());
		comment->Des().Copy(*(gifComment->iComment)); // Create a 16 bit copy of the 8 bit original	
		}
	
	return comment;
	}

void CGifDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec()==NULL);
	CGifReadCodec* imageReadCodec = CGifReadCodec::NewL(iGlobalPalette, iFileInfo.iBackgroundColorIndex, iFileInfo.iScreenSize,
	                                ( (DecoderOptions() & CImageDecoder::EPreferFastDecode) == CImageDecoder::EPreferFastDecode)
	            );

	SetImageReadCodec(imageReadCodec);
	
	if((DecoderOptions()&CImageDecoder::EOptionUseFrameSizeInPixels) == CImageDecoder::EOptionUseFrameSizeInPixels)
		{
		imageReadCodec->SetUseFrameSizeInPixels(ETrue);
		}

	ReadFrameHeadersL();
	}

void CGifDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	// Read the header (+ 1 extra byte, so that we can check the first block id is valid)
	ReadDataL(0, bufferDes, (KGifFileInformationSize + (KGifColorTableMaxEntries * KGifPaletteEntrySize) + 1));

	// Validate the header.
	if (bufferDes.Length() < KGifFileInformationSize)
		User::Leave(KErrUnderflow);

	iFileInfo.iSignature = bufferDes.Left(KGifSignatureLength);
	if (iFileInfo.iSignature != KGif87aFileSignature && 
		iFileInfo.iSignature != KGif89aFileSignature)
		User::Leave(KErrCorrupt);

	const TUint8* ptr = &bufferDes[KGifSignatureLength];
	iFileInfo.iScreenSize.iWidth = PtrReadUtil::ReadUint16(ptr);
	ptr+=2;

	iFileInfo.iScreenSize.iHeight = PtrReadUtil::ReadUint16(ptr);
	ptr+=2;

	TUint8 flags = *ptr++;
	iFileInfo.iBitsPerPixel = (flags & 0x07) + 1;
	iFileInfo.iColorResolutionBits = ((flags & 0x70) >> 4) + 1;
	iFileInfo.iGlobalColorMap = flags & 0x80;
	if (iFileInfo.iSignature == KGif89aFileSignature)
		iFileInfo.iSortedGlobalMap = flags & 0x08;

	if (iFileInfo.iGlobalColorMap)
		{
		iFileInfo.iBackgroundColorIndex = *ptr;
		iGlobalPaletteEntries = 1 << iFileInfo.iBitsPerPixel;
		}
	else
		{
		iFileInfo.iBackgroundColorIndex = KErrNotFound;
		iGlobalPaletteEntries = 0;
		}

	ptr++;
	TUint8 pixelAspectRatio = *ptr;
	if (iFileInfo.iSignature == KGif87aFileSignature)
		{
		iFileInfo.iPixelAspectRatio = pixelAspectRatio & 0x7f;
		iFileInfo.iSortedGlobalMap = pixelAspectRatio & 0x80;
		}
	else
		iFileInfo.iPixelAspectRatio = pixelAspectRatio;

	if (iFileInfo.iGlobalColorMap)
		{
		if (bufferDes.Length() < (KGifFileInformationSize + iGlobalPaletteEntries * KGifPaletteEntrySize))
			User::Leave(KErrUnderflow);

		const TUint8* ptr = &bufferDes[KGifFileInformationSize];
		TRgb* palettePtr = iGlobalPalette;
		TRgb* palettePtrLimit = iGlobalPalette + iGlobalPaletteEntries;

		while (palettePtr < palettePtrLimit)
			{
			*palettePtr++ = TRgb(ptr[0],ptr[1],ptr[2]);
			ptr += KGifPaletteEntrySize;
			}

		palettePtrLimit = iGlobalPalette + KGifColorTableMaxEntries;
		while (palettePtr < palettePtrLimit)
			*palettePtr++ = KRgbWhite;

		TGifBackgroundColor* gifBackgroundColor = new(ELeave) TGifBackgroundColor;
		gifBackgroundColor->iBackgroundColorIndex = iFileInfo.iBackgroundColorIndex;
		gifBackgroundColor->iBackgroundColor = iGlobalPalette[iFileInfo.iBackgroundColorIndex];
		CleanupStack::PushL(gifBackgroundColor);

		User::LeaveIfError(AppendImageData(gifBackgroundColor));
		CleanupStack::Pop(); // gifBackgroundColor
		}

	if (bufferDes.Length() < (KGifFileInformationSize + (iGlobalPaletteEntries * KGifPaletteEntrySize)) + 1)
		User::Leave(KErrUnderflow);
	
	// Check that first byte after the global color table is a valid block id
	TUint8 nextBlockId = bufferDes[KGifFileInformationSize + (iGlobalPaletteEntries * KGifPaletteEntrySize)];
	switch (nextBlockId)
		{
		case KGifExtensionId:
		case KGifImageDescriptorId:
		case KGifPlainTextExtensionId:
		case KGifGraphicControlExtensionId:
		case KGifCommentExtensionId:
		case KGifApplicationExtensionId:
			break;

		default:
			User::Leave(KErrCorrupt);
		}
	
	TInt startPosition = KGifFileInformationSize;
	if (iFileInfo.iGlobalColorMap)
		startPosition += iGlobalPaletteEntries * KGifPaletteEntrySize;
	SetStartPosition(startPosition);

	TFrameInfo imageInfo;
	imageInfo = ImageInfo();
	imageInfo.iOverallSizeInPixels = iFileInfo.iScreenSize;
	imageInfo.iBitsPerPixel = iFileInfo.iBitsPerPixel;
	imageInfo.iFrameSizeInTwips.SetSize(0,0);
	if (iFileInfo.iGlobalColorMap)
		{
		imageInfo.iBackgroundColor = iGlobalPalette[iFileInfo.iBackgroundColorIndex];
		if (iFileInfo.iColorResolutionBits<=4)
			imageInfo.iFrameDisplayMode = EColor4K;
		else
			imageInfo.iFrameDisplayMode = EColor16M;
		}
	else
		imageInfo.iFrameDisplayMode = EColor256;
	imageInfo.iDelay = KErrNotFound;

	SetImageInfo(imageInfo);
	SetDataLength(KMaxTInt); // Set default data length in case format header doesn't contain this information
	}

CFrameInfoStrings* CGifDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	const TUid KGifCodecDllUid = {KGIFCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KGifCodecDllUid,resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex = (iFileInfo.iSignature == KGif87aFileSignature) ? 0 : 1;
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetFormatL(info);

	TInt width = frameInfo.iOverallSizeInPixels.iWidth;
	TInt height = frameInfo.iOverallSizeInPixels.iHeight;
	TInt depth = frameInfo.iBitsPerPixel;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, depth);
	frameInfoStrings->SetDepthL(info);

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

// Gif encoder.
CGifEncoder* CGifEncoder::NewL()
	{
	CGifEncoder* self = new(ELeave) CGifEncoder;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;

	}

CGifEncoder::CGifEncoder()
	{
	}

void CGifEncoder::ConstructL()
	{
	iPalette = CPalette::NewDefaultL(EColor256);
	}

CGifEncoder::~CGifEncoder()
	{
	if (iGifScaler)
		{
		iGifScaler->Cancel();
		delete iGifScaler;
		}
	delete iPalette;
	delete iDest;
	CImageEncoderPlugin::Cleanup();
	}

const CPalette* CGifEncoder::Palette() const
	{
	return iPalette;
	}

void CGifEncoder::WriteExifDataL(TRequestStatus*& aScaleCompletionStatus)
	{
	if ((EncoderOptions() & CImageEncoder::EOptionGenerateAdaptivePalette) == CImageEncoder::EOptionGenerateAdaptivePalette)
		{
		if (!iPalette)
			{
			iPalette = CPalette::NewL(KGifColorTableMaxEntries);	
			}
		iGifScaler->Scale(aScaleCompletionStatus, *iDest, *iPalette);
		}
	else
		{
		// the framework waits for aScaleCompletionStatus to complete
		SelfComplete(KErrNone);
		}
	}

void CGifEncoder::PrepareEncoderL(const CFrameImageData* aFrameImageData)
	{
	// Create the codec.
	StartPosition() = KGifFileInformationSize;
	StartPosition() += KGifColorTableMaxEntries * KGifPaletteEntrySize;
	ASSERT(ImageWriteCodec() == NULL);
	
	// ensure iPalette is NULL to start with since CGifWriteCodec::FillBufferL() behaviour depends on it
	delete iPalette;
	iPalette = NULL;

	// check to see if client has supplied a palette
	if (aFrameImageData)
		{
		TInt frameDataCount = aFrameImageData->FrameDataCount();
		TGifColorTable* gifColorTable = NULL;
		for (TInt frameDataIndex = 0; frameDataIndex < frameDataCount; frameDataIndex++)
			{
			TFrameDataBlock* frameData = const_cast<TFrameDataBlock*>(aFrameImageData->GetFrameData(frameDataIndex));
			if (frameData->DataType() == KGIFColorTableUid)
				{
				gifColorTable = static_cast<TGifColorTable*>(frameData);
				}
			}

		if (gifColorTable)
			{
			if (!iPalette)
				{
				iPalette = CPalette::NewL(KGifColorTableMaxEntries);	
				}
			for (TInt i = 0; i < KGifColorTableMaxEntries; i++)
				{
				iPalette->SetEntry(i, gifColorTable->iPalette[i]);
				}
			}
		}

	if ((EncoderOptions() & CImageEncoder::EOptionGenerateAdaptivePalette) == CImageEncoder::EOptionGenerateAdaptivePalette)
		{
		CFbsBitmap* sourceBitmap = const_cast<CFbsBitmap*>(&Source());
		delete iGifScaler;
		iGifScaler = NULL;
		iGifScaler = CGifScaler::NewL(*sourceBitmap);

		delete iDest;
		iDest = NULL;
		iDest = new (ELeave) CFbsBitmap;
		User::LeaveIfError(iDest->Create(sourceBitmap->SizeInPixels(), EColor256));
		}

	CGifWriteCodec* imageWriteCodec = CGifWriteCodec::NewL(*this);
	
	SetImageWriteCodec(imageWriteCodec);

	}

void CGifEncoder::UpdateHeaderL()
	{
	TInt headerSize = KGifFileInformationSize + (KGifColorTableMaxEntries * KGifPaletteEntrySize);

	HBufC8* gifHeaderPtr = HBufC8::NewMaxLC(headerSize);
	TUint8* headerPtr = &gifHeaderPtr->Des()[0];

	Mem::Copy(headerPtr,&KGif87aFileSignature()[0],KGifSignatureLength); headerPtr += KGifSignatureLength;

	TInt width = Source().SizeInPixels().iWidth;
	TInt height = Source().SizeInPixels().iHeight;
	PtrWriteUtil::WriteInt16(headerPtr, width);
	headerPtr+=2;
	PtrWriteUtil::WriteInt16(headerPtr, height);
	headerPtr+=2;
	TUint8 resolutionFlag = 0;
	resolutionFlag |= 8 - 1; // Bpp - 1
	resolutionFlag |= (8 - 1) << 4; // Color Resolution
	resolutionFlag |= 0x80; // Global Color Table Flag
	*headerPtr++ = resolutionFlag;
	*headerPtr++ = 255; // Background Color Index
	*headerPtr++ = 0; // Pixel Aspect Ratio

	for (TInt paletteIndex = 0; paletteIndex < KGifColorTableMaxEntries; paletteIndex++)
		{
		TRgb entry;
		if (iPalette)
			{
			entry = iPalette->GetEntry(paletteIndex);
			}
		else
			{
			entry = TRgb::Color256(paletteIndex);			
			}

		headerPtr[0] = (TUint8)entry.Red();
		headerPtr[1] = (TUint8)entry.Green();
		headerPtr[2] = (TUint8)entry.Blue();
		headerPtr += 3;
		}

	TPtr8 bufferDes(gifHeaderPtr->Des());
	WriteDataL(0,bufferDes);

	CleanupStack::PopAndDestroy(); // gifHeaderPtr
	}

