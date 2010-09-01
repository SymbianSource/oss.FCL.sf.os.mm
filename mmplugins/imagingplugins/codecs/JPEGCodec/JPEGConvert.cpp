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

#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <imageconversion.h>
#include "icl/ImageCodec.h"
#include "ImageClientMain.h"
#include <101F45D6_extra.rsg>

#include "JpegTypes.h"
#include "jpegwritecodec.h"
#include "JpegYuvDecoder.h"
#include "icl/ICL_UIDS.hrh"
#include <bitmaptransforms.h>
#include "exifdecoder.h"
#include "ExifEncoder.h"
#include "exiftransform.h"
#include "ImageUtils.h"

#include "JPEGCodec.h"
#include "JPEGConvert.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif

_LIT(KJPEGPanicCategory, "JPEGConvertPlugin");

const TInt KBlockSignatureSearchSize = 256;
const TInt KSizeOfBlockSignaturePlusBlockLength = sizeof(KJpgMarker) + sizeof(TUint16);

//
// this file doesn't contain much of performance-critical code so use thumb
// instruction set to save on some ROM footprint
//
#if defined(__ARMCC__)
#pragma thumb
#endif

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KJPEGPanicCategory, aError);
	}

// Jpeg decoder.
CJpegDecoder* CJpegDecoder::NewL()
	{
	return new(ELeave)CJpegDecoder();
	}

CJpegDecoder::CJpegDecoder()
 :	iImageType(CImageDecoder::EImageTypeMain)
	{
	}

CJpegDecoder::~CJpegDecoder()
	{
	delete iExtensionManager;
	delete iStreamedDecodeExt;
		
	delete iExifDecoder;
	iComment.ResetAndDestroy();
	Cleanup();
	}

void CJpegDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeJPGUid;
	aImageSubType = KNullUid;
	}

TInt CJpegDecoder::NumberOfFrameComments(TInt aFrameNumber) const
	{
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < NumberOfFrames()), Panic(EFrameNumberOutOfRange));

	const CFrameImageData& frameImageData = FrameData(aFrameNumber);
	TInt frameCommentCount = 0;
	// Because this is a single frame format, the comments are stored
	// in the global rather than local comment block
	TInt imageDataCount = frameImageData.ImageDataCount();
	for (TInt count = 0; count < imageDataCount; count++)
		{
		const TImageDataBlock* imageData = frameImageData.GetImageData(count);
		if (imageData->DataType() == KJPGCommentUid)
			frameCommentCount++;
		}

	return frameCommentCount;
	}

HBufC* CJpegDecoder::FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const
	{
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < NumberOfFrames()), Panic(EFrameNumberOutOfRange));
	__ASSERT_ALWAYS((aCommentNumber >= 0) && (aCommentNumber < NumberOfFrameComments(aFrameNumber)), Panic(ECommentNumberOutOfRange));

	const CFrameImageData& frameImageData = FrameData(aFrameNumber);
	TInt commentCount = 0;
	// Because this is a single frame format, the comments are stored
	// in the global rather than local comment block
	TInt imageDataCount = frameImageData.ImageDataCount();
	const TImageDataBlock* imageData = NULL;
	for (TInt count = 0; count < imageDataCount; count++)
		{
		imageData = frameImageData.GetImageData(count);
		if (imageData->DataType() == KJPGCommentUid)
			{
			if (commentCount == aCommentNumber)
				{
				break;
				}
			commentCount++;
			}
		}

	const TJpegComment* jpegComment = static_cast<const TJpegComment*>(imageData);
	HBufC* comment = NULL;
	if (jpegComment)
		{
		comment = HBufC::NewL(jpegComment->iComment->Length());
		comment->Des().Copy(*(jpegComment->iComment)); // Create a 16 bit copy of the 8 bit original	
		}
	return comment;
	}

TInt CJpegDecoder::FrameHeaderBlockSize(TInt /*aFrameNumber*/) const
	{
	return KJfifHeaderBlockSizeInBytes;
	}

TInt CJpegDecoder::FrameBlockSize(TInt /*aFrameNumber*/) const
	{
	return KJfifDataBlockSizeInBytes;
	}

//
//
//
TInt CJpegDecoder::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	ASSERT(iExtensionManager);
	const TFrameInfo frameInfo = FrameInfo(aFrameNumber);
	TSize originalSize = frameInfo.iOverallSizeInPixels;
	TInt err = iExtensionManager->GetDestinationSize(originalSize);
	if(err == KErrNone)
		{
		aSize = originalSize;
		}
	return err;
	}

void CJpegDecoder::InitConvertL()
	{
	iState = EStateStart; //  when starting convert, always start from the top
	CImageDecoderPlugin::InitConvertL();
	}

void CJpegDecoder::DoConvert()
	{
	switch(iState)
		{
		case EStateStart:
			{
			TRAPD(errCode, PrepareForProcessFrameL());
			if (errCode!=KErrNone)
				{
				RequestComplete(errCode);
				return;
				}

			iCodecState = EFrameIncomplete;

			TBufPtr8& sourceData = SourceData();
			TInt sourceLength = sourceData.Length();
			if (sourceLength!=0)
				{
				TRAP(errCode, iCodecState = ImageReadCodec()->ProcessFrameL(sourceData));
				}

			if (errCode == KErrNone && (sourceData.Length() == sourceLength || iCodecState == EFrameComplete || iCodecState == EBlockComplete))
				{
				CJpgReadCodec* readCodec = static_cast<CJpgReadCodec*>(ImageReadCodec());
				JPEG_ASSERT(readCodec);
				readCodec->InitDrawFrame();
				iState=EStateDrawFrame;
				SelfComplete(KErrNone);
				}
			else
				{
				ASSERT(iState==EStateStart);
				HandleProcessFrameResult(errCode,iCodecState);
				}
			}
			break;
		case EStateDrawFrame:
			{
			CJpgReadCodec* readCodec = static_cast<CJpgReadCodec*>(ImageReadCodec());
			JPEG_ASSERT(readCodec);
			TBool result = EFalse;
			TRAPD(errCode, result = readCodec->DrawFrameL());
			if (errCode != KErrNone)
				{
				HandleProcessFrameResult(errCode, iCodecState);
				}
			else if (result)
				{
				HandleProcessFrameResult(KErrNone, iCodecState);
				iState = EStateStart;
				}
			else
				{
				ASSERT(iState==EStateDrawFrame);
				SelfComplete(KErrNone);
				}
			}
			break;
		default:
			ASSERT(EFalse);
		}
	}

MExifMetadata* CJpegDecoder::ExifMetadata()
	{
	return iExifDecoder;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CJpegDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec() == NULL);

	CJpgReadCodec* imageReadCodec = NULL;

	if (iJpgFrameInfo.iProgressive)
		{
		imageReadCodec = CProgressiveJpgReadCodec::NewL(
				iJpgFrameInfo,
				iScanInfo,
				iDCHuffmanTable,
				iACHuffmanTable,
				iQTable);
		}
	else if (iJpgFrameInfo.iNumberOfComponents == iScanInfo.iNumberOfComponents)
		{
		imageReadCodec = CSequentialJpgReadCodec::NewL(
				iJpgFrameInfo,
				iScanInfo,
				iDCHuffmanTable,
				iACHuffmanTable,
				iQTable);
		}
	else
		{
		iJpgFrameInfo.iMultiScan = ETrue;
		imageReadCodec = CMultiScanSequentialJpgReadCodec::NewL(
				iJpgFrameInfo,
				iScanInfo,
				iDCHuffmanTable,
				iACHuffmanTable,
				iQTable);
		}

	CleanupStack::PushL(imageReadCodec);
	
	if (!iExtensionManager)
		{
		// Manager settings persist over all frames.
		iExtensionManager = CPluginExtensionManager::NewL(imageReadCodec);
		}
	else
		{
		// Maintain manager settings, but reset the codec that it points to.
		iExtensionManager->ResetCodecExtension(imageReadCodec);
		}
	

	CleanupStack::Pop(); // imageReadCodec
	
	imageReadCodec->SetExtensionManager(iExtensionManager);
	imageReadCodec->SetHighSpeedMode( (DecoderOptions() & CImageDecoder::EPreferFastDecode) == CImageDecoder::EPreferFastDecode );

	// If this is the main image - pull the thumbnail out and give it to the
	// framework
	if (iImageType == CImageDecoder::EImageTypeMain)
		{
		if (iExifDecoder != NULL)
			{
			TInt err = 0;
			TInt thumbDataOffset = 0;

			HBufC8* thumbnailData = NULL;
			// Hold the thumbnail data (extracted from the Exif object).

			// Get both the offset of the thumbnail data and its length.
			err = iExifDecoder->GetIntegerParam(KJPEGInterchangeFormat, 1, thumbDataOffset);
			if(err == KErrNone)
				{
				TInt thumbDataLength = 0;
				err = iExifDecoder->GetIntegerParam(KJPEGInterchangeFormatLength, 1, thumbDataLength);
				if(err == KErrNone)
					{
					//get the thumbnail data.
					thumbnailData = iExifDecoder->GetJpegThumbnailData();
					// Pass the thumbnailData back to the framework (ownership remains here).
					SetThumbnailData(thumbnailData);
					}
				}
			}
		}

	imageReadCodec->SetAutoRotateFlag(iAutoRotateFlag);
	
	SetImageReadCodec(imageReadCodec);

	ReadFrameHeadersL();
	}

void CJpegDecoder::NotifyImageTypeChangeL(TInt aImageType)
	{
	TInt prevImageType = iImageType;
	iImageType = aImageType;
	TRAPD(err, ScanDataL());
	if (err != KErrNone)
		{
		iImageType = prevImageType;
		JPEG_LEAVE(err, "ScanDataL");
		}
	}

void CJpegDecoder::ReadFormatL()
	{
	iJpgFrameInfo.iProgressive = EFalse; // Reset

	TPtrC8 bufferDes;
	ReadDataL(0, bufferDes, KJfifInitialHeaderSize);

	// Validate the header.
	if (bufferDes.Length() < KJfifInitialHeaderSize)
		{
		JPEG_LEAVE(KErrUnderflow, "Not enough data for JFIF header");
		}

	iPtr = &bufferDes[0];
	const TUint8* ptr = iPtr;
	TInt startPosition = 6; // Move past SOISignature + first block sig/length.
	TUint16 soiSig = ReadBigEndianUint16(ptr);
	if (soiSig != CJpgReadCodec::EMarkerSOI)
		{
		JPEG_LEAVE(KErrCorrupt, "Expected SOI marker");
		}

	iJpgFrameInfo.iRestartInterval = KErrNotFound;
	SetStartPosition(startPosition);

	//delete all comments
	iComment.ResetAndDestroy();

	for (;;)
		{
		// Read the signature and length of the next block.
		iBlockSignature = ReadBigEndianUint16(ptr);

		// Skip until next block signature is valid
		if ((iBlockSignature & KJpgMarkerMask) != KJpgMarker)
			{
			TInt blockIndex = KErrNotFound;
			TChar jpgMarker = TChar(KJpgMarker >> 8);
			for (;;)
				{
				// search in blocks of size KBlockSignatureSearchSize
				// for occurrence of a jpeg marker (0xff)

				ReadDataL(startPosition, bufferDes, KBlockSignatureSearchSize);
				if (!bufferDes.Length())
					{
					// no marker found, no data left
					JPEG_LEAVE(KErrCorrupt, "No marker, no data");
					}
				blockIndex = bufferDes.Locate(jpgMarker);
				if (blockIndex != KErrNotFound)
					{
					startPosition += blockIndex;
					break;
					}
				startPosition += bufferDes.Length();
				}

			if ((bufferDes.Length() - blockIndex) < KSizeOfBlockSignaturePlusBlockLength)
				{
				// ensure there are at least KSizeOfBlockSignaturePlusBlockLength bytes in bufferDes after the blockIndex
				ReadDataL(startPosition, bufferDes, KSizeOfBlockSignaturePlusBlockLength);
				if (bufferDes.Length() < KSizeOfBlockSignaturePlusBlockLength)
					{
					JPEG_LEAVE(KErrUnderflow, "Not enough data");	// Not enough data present
					}
				blockIndex = 0;
				}
			iPtr = &bufferDes[blockIndex];
			ptr = iPtr;
			iBlockSignature = ReadBigEndianUint16(ptr);
			startPosition += KSizeOfBlockSignaturePlusBlockLength;
			SetStartPosition(startPosition);
			}

		// Read the current block's length.
		iBlockLength = ReadBigEndianUint16(ptr);

		// Read the next block plus the signature and size of block to follow
		ReadDataL(StartPosition(),bufferDes,iBlockLength+2);

		if (bufferDes.Length() < iBlockLength+2)
			{
			JPEG_LEAVE(KErrUnderflow, "Not enough data");	// Not enough data present
			}

		iPtr = &bufferDes[0];
		switch (iBlockSignature)
			{
			case CJpgReadCodec::EMarkerAPP0:
				ProcessApp0L();
				break;
			
			case CJpgReadCodec::EMarkerAPP1:
				// Process the EXIF information
				// Check if the flag to ignore is set. If yes, set the data to NULL.
				if (DecoderOptions() & CImageDecoder::EOptionIgnoreExifMetaData)
					{
					delete iExifDecoder;
					iExifDecoder = NULL;
					}
				else
					{
					TRAPD(err, ProcessApp1L());
					if ((err != KErrCorrupt) && (err != KErrNotSupported))
						{
						JPEG_LEAVE_IF_ERROR(err, "ProcessApp1L");
						}
					}
				break;
				
			case CJpgReadCodec::EMarkerAPP14:
				ProcessAppEL();
				break;
				
			case CJpgReadCodec::EMarkerDHT:
				ProcessHuffmanTableL();
				break;
			
			case CJpgReadCodec::EMarkerDQT:
				ProcessQTableL();
				break;
			
			case CJpgReadCodec::EMarkerSOF2:
				iJpgFrameInfo.iProgressive = ETrue; // Fall through to ProcessStartOfFrameL()
			case CJpgReadCodec::EMarkerSOF0:
			case CJpgReadCodec::EMarkerSOF1:
				ProcessStartOfFrameL();
				break;
			
			case CJpgReadCodec::EMarkerSOF3:
				User::Leave(KErrNotSupported);
				break;
			
			case CJpgReadCodec::EMarkerSOS:
				ProcessStartOfScanL();
				FinishedProcessing();
				// Fill in image data structures and give control to the codec.
				InitialiseImageDataL();
				return;
			
			case CJpgReadCodec::EMarkerDRI:
				ProcessRestartInterval();
				break;
			
			case CJpgReadCodec::EMarkerCOM:
				CheckCommentBlockL();
				ProcessCommentL();
				break;
			
		default:
			// ProcessOtherBlockL();
			break;
			}

		ptr = iPtr + (iBlockLength - 2); // Advance past this block.
		startPosition += iBlockLength + 2; // Advance past this block and the signature and length of the next block
		SetStartPosition(startPosition);
		}
	}

//
// APP markers are reserved for application use. Different applications are
// free to use the same APP marker for different purposes.
// In our case, we look for the EXIF header and ignore it otherwise.
//
void CJpegDecoder::ProcessApp1L()
	{
	if (iBlockLength < KExifHeaderLength)
		{
		return;
		}
	TPtrC8 exifHeaderPtr(iPtr, KExifHeaderLength);
	if ((KExifHeader().Compare(exifHeaderPtr) == KErrNone) && (iExifDecoder == NULL))
		{
		// This is the EXIF header block, so instantiate the EXIF decoder object
		iExifDecoder = CExifDecoder::NewL(iBlockLength, iPtr);
		}
	}

//
// Check that length of comment block is correct, if not
// calculate real size of comment block and adjust data read.
//
void CJpegDecoder::CheckCommentBlockL()
	{
	// INC076787 -
	// Allow successful decoding of a JPEG file with incorrect comment block length.
	TUint16 nextBlockSignature = PtrReadUtil::ReadBigEndianUint16(iPtr + iBlockLength - 2);
	if ((nextBlockSignature & KJpgMarkerMask) != KJpgMarker)
		{
		// Block length is incorrect. Calculate real length of comment block
		TPtrC8 commentBuffer;
		TInt realLength = 0;

		// First check whether the block is smaller than expected.
		nextBlockSignature = PtrReadUtil::ReadBigEndianUint16(iPtr);
		while (((nextBlockSignature & KJpgMarkerMask) != KJpgMarker) && (realLength <= (iBlockLength - 2)))
			{
			realLength++;
			nextBlockSignature = PtrReadUtil::ReadBigEndianUint16(iPtr + realLength);
			}
		if (realLength <= (iBlockLength - 2))
			{
			// Correct block length
			iBlockLength = realLength + 2; // Add the two bytes which contain length
			}
		else		// Otherwise check for more data
			{
			TInt offset = 1;
			TInt startPos = StartPosition() + iBlockLength - 2;

			ReadDataL(startPos + offset, commentBuffer, 2);
			if (!commentBuffer.Length())
				{
				// Not enough data in the 'commentBuffer'
				JPEG_LEAVE(KErrUnderflow, "Not enough data");	
				}
			nextBlockSignature = PtrReadUtil::ReadBigEndianUint16(&commentBuffer[0]);

			while ((nextBlockSignature & KJpgMarkerMask) != KJpgMarker)
				{
				offset++;
				ReadDataL(startPos + offset, commentBuffer, 2);
				if (!commentBuffer.Length())
					{
					// Not enough data in the 'commentBuffer'
					JPEG_LEAVE(KErrUnderflow, "Not enough data");
					}
				nextBlockSignature = PtrReadUtil::ReadBigEndianUint16(&commentBuffer[0]);
				}
			// Correct block length
			iBlockLength += offset;
			}

		// Re-read the comment block again plus the signature and size of block which follows
		ReadDataL(StartPosition(), commentBuffer, iBlockLength + 2);
		iPtr = &commentBuffer[0];
		}
	}


CFrameInfoStrings* CJpegDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	const TUid KJpgCodecDllUid = {KJPGCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs, KJpgCodecDllUid, resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

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
	formatIndex = (iJpgFrameInfo.iProgressive) ? 1 : 0;
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetDetailsL(info);

	CleanupStack::Pop(frameInfoStrings);
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

void CJpegDecoder::InitialiseImageDataL()
	{
	// Fill in image data info.
	TInt quality = 0;
	TInt tables = 0;
	TInt qTableIndex;
	for (qTableIndex = 0; qTableIndex < KJpgMaxNumberOfTables; qTableIndex++)
		{
		if (iQTable[qTableIndex].QualityFactor() > 0)
			{
			quality += iQTable[qTableIndex].QualityFactor();
			tables++;
			}
		}
	if (tables > 0)
		quality /= tables;

	TJpegImageData* jpegImageData = new(ELeave) TJpegImageData;
	CleanupStack::PushL(jpegImageData);

	jpegImageData->iQualityFactor = quality;
	if (iJpgFrameInfo.iNumberOfComponents == 1)
		jpegImageData->iSampleScheme = TJpegImageData::EMonochrome;
	else
		{
		if (iJpgFrameInfo.iMaxHorzSampleFactor == 1 && iJpgFrameInfo.iMaxVertSampleFactor == 1)
			jpegImageData->iSampleScheme = TJpegImageData::EColor444;
		else if (iJpgFrameInfo.iMaxHorzSampleFactor == 2 && iJpgFrameInfo.iMaxVertSampleFactor == 1)
			jpegImageData->iSampleScheme = TJpegImageData::EColor422;
		else
			jpegImageData->iSampleScheme = TJpegImageData::EColor420;
		}
	
	JPEG_LEAVE_IF_ERROR(AppendImageData(jpegImageData), "AppendImageData: jpegImageData");
	CleanupStack::Pop(jpegImageData);

	// Fill in QTable image data.
	for (qTableIndex = 0 ; qTableIndex < KJpgMaxNumberOfTables ; qTableIndex++)
		{
		if (iQTable[qTableIndex].QualityFactor() > 0)
			{
			TJpegQTable* jpegQTable = new(ELeave) TJpegQTable;
			CleanupStack::PushL(jpegQTable);

			TQTable& qTable = iQTable[qTableIndex];
			TUint8 values[2 * KJpgQTableEntries];

			const TInt precisionBytes = qTable.Get(values) / KJpgQTableEntries;
			const TUint8* zigZagPtr = KZigZagSequence.iZigZag;
			TUint8* valuePtr = values;
			const TUint8* valuePtrLimit = valuePtr + (KJpgQTableEntries * precisionBytes);

			while (valuePtr < valuePtrLimit)
				{
				jpegQTable->iEntries[*zigZagPtr++] = valuePtr[0]; // Lose the low byte of 16 bit precision values
				valuePtr += precisionBytes;
				}

			jpegQTable->iTableIndex = qTableIndex;
			JPEG_LEAVE_IF_ERROR(AppendImageData(jpegQTable), "AppendImageData: jpegQTable");
			CleanupStack::Pop();
			}
		}

	// Add the comments
	const TInt noOfComments = iComment.Count();
	for(TInt commentNo=0; commentNo < noOfComments; commentNo++)
		{
		TJpegComment* jpegComment = new(ELeave) TJpegComment;
		CleanupStack::PushL(jpegComment);

		HBufC8* comment = iComment[0];
		jpegComment->iComment = comment;

		JPEG_LEAVE_IF_ERROR(AppendImageDataBuffer(comment), "AppendImageDataBuffer");
		iComment.Remove(0);

		JPEG_LEAVE_IF_ERROR(AppendImageData(jpegComment), "AppendImageData(jpegComment)");
		CleanupStack::Pop(); // jpegComment
		}
	}

void CJpegDecoder::ProcessApp0L()
	{
	// iBlockLength must specify at least the 2 byte
	// length field itself for Application Data!
	if (iBlockLength<2)
		{
		JPEG_LEAVE(KErrCorrupt, "Not enough data for App0");
		}

	const TUint8* ptr = iPtr;

	TPtrC8 id(ptr, Min(KJpgApp0IdSize, iBlockLength-2));

	if (id == KJfifId)
		{
		// We know jfif will always have a minimum of 14 bytes of
		// data in addition to the 2 byte length field.
		if (iBlockLength<16)
			{
			JPEG_LEAVE(KErrCorrupt, "Not enough data in App0");
			}

		ptr += KJpgApp0IdSize + 1;
		TUint16 version = ReadBigEndianUint16(ptr);
		if (version != KJfifVersion0100 &&
			version != KJfifVersion0101 &&
			version != KJfifVersion0102)
			{
			JPEG_LEAVE(KErrNotSupported, "Unsupported jfif version");
			}

		iJpgFrameInfo.iUnits = TJpgFrameInfo::TJpgImageUnits(*ptr++);
		iJpgFrameInfo.iSizeInTwips.iWidth = ReadBigEndianUint16(ptr);
		iJpgFrameInfo.iSizeInTwips.iHeight = ReadBigEndianUint16(ptr);
		iJpgFrameInfo.iThumbnailSize.iWidth = *ptr++;
		iJpgFrameInfo.iThumbnailSize.iHeight = *ptr;
		
		JPEG_DEBUG2("Frame units: %d", iJpgFrameInfo.iUnits);
		JPEG_DEBUG2("Frame twips width: %d", iJpgFrameInfo.iSizeInTwips.iWidth);
		JPEG_DEBUG2("Frame twips height: %d", iJpgFrameInfo.iSizeInTwips.iHeight);
		JPEG_DEBUG2("Frame thumbnail width: %d", iJpgFrameInfo.iThumbnailSize.iWidth);
		JPEG_DEBUG2("Frame twips width: %d", iJpgFrameInfo.iThumbnailSize.iHeight);

		// skip over JFIF thumbnail data without storing it
		// there are currently no client APIs for accessing it
		ptr += iJpgFrameInfo.iThumbnailSize.iWidth * iJpgFrameInfo.iThumbnailSize.iHeight * 3;

		iJFIFMarkerPresent = ETrue;
		}
	}

void CJpegDecoder::ProcessAppEL()
	{
	const TUint8* ptr = iPtr;

	TPtrC8 id(ptr,KJpgAppEIdSize);

	if (id == KAdobeId)
		{
		iAdobeMarkerPresent = ETrue;
		iAdobeTransform = ptr[11];
		}
	}

void CJpegDecoder::ProcessHuffmanTableL()
	{
	const TUint8* dataPtr = iPtr;
	const TUint8* dataPtrLimit = dataPtr + iBlockLength - 2;
	THuffmanTableProcessor::ProcessHuffmanTableL(dataPtr, dataPtrLimit, iDCHuffmanTable, iACHuffmanTable);
	}

void CJpegDecoder::ProcessQTableL()
	{
	const TUint8* dataPtr = iPtr;
	const TUint8* dataPtrLimit = dataPtr + iBlockLength - 2;

	while (dataPtr < dataPtrLimit)
		{
		TInt index = *dataPtr++;
		TBool sixteenBitPrecision = index & 0x10;
		index &= 0x0f;
		if (index >= KJpgMaxNumberOfTables)
			{
			JPEG_LEAVE(KErrCorrupt, "table index out of range");
			}

		dataPtr += iQTable[index].Set(dataPtr,sixteenBitPrecision);
		}
	}

void CJpegDecoder::ProcessStartOfFrameL()
	{
	const TUint8* ptr = iPtr;

	iJpgFrameInfo.iSamplePrecision = *ptr++;
	if (iJpgFrameInfo.iSamplePrecision != 8)
		{
		JPEG_LEAVE(KErrNotSupported, "Unsupported sample precision");
		}

	iJpgFrameInfo.iSizeInPixels.iHeight = ReadBigEndianUint16(ptr);
	iJpgFrameInfo.iSizeInPixels.iWidth = ReadBigEndianUint16(ptr);
	
	JPEG_DEBUG2("Frame pixel width: %d", iJpgFrameInfo.iSizeInPixels.iWidth);
	JPEG_DEBUG2("Frame pixel height: %d", iJpgFrameInfo.iSizeInPixels.iHeight);
	
	if ((iJpgFrameInfo.iSizeInPixels.iHeight <= 0) || (iJpgFrameInfo.iSizeInPixels.iWidth <= 0))
		{
		JPEG_LEAVE(KErrCorrupt, "Invalid pixel height or width");		
		}
	
	if (iJpgFrameInfo.iSizeInTwips.iWidth > 0 && iJpgFrameInfo.iSizeInTwips.iHeight > 0)
		{
		if (iJpgFrameInfo.iUnits == TJpgFrameInfo::EDotsPerInch)
			{
			iJpgFrameInfo.iSizeInTwips.iWidth = iJpgFrameInfo.iSizeInPixels.iWidth * KTwipsPerInch / iJpgFrameInfo.iSizeInTwips.iWidth;
			iJpgFrameInfo.iSizeInTwips.iHeight = iJpgFrameInfo.iSizeInPixels.iHeight * KTwipsPerInch / iJpgFrameInfo.iSizeInTwips.iHeight;
			}
		else if (iJpgFrameInfo.iUnits == TJpgFrameInfo::EDotsPerCm)
			{
			iJpgFrameInfo.iSizeInTwips.iWidth = iJpgFrameInfo.iSizeInPixels.iWidth * KTwipsPerCm / iJpgFrameInfo.iSizeInTwips.iWidth;
			iJpgFrameInfo.iSizeInTwips.iHeight = iJpgFrameInfo.iSizeInPixels.iHeight * KTwipsPerCm / iJpgFrameInfo.iSizeInTwips.iHeight;
			}
		else
			iJpgFrameInfo.iSizeInTwips.SetSize(0,0);
		}

	iJpgFrameInfo.iMaxHorzSampleFactor = 0;
	iJpgFrameInfo.iMaxVertSampleFactor = 0;
	iJpgFrameInfo.iNumberOfComponents = *ptr++;
	JPEG_DEBUG2("Frame number of components: %d", iJpgFrameInfo.iNumberOfComponents);
	
	//At least one component and not more than we can cope with
	if(iJpgFrameInfo.iNumberOfComponents < KJpgMinNumberOfComponents)
		{
		JPEG_LEAVE(KErrCorrupt, "Not enough components");
		}

	if ((iJpgFrameInfo.iNumberOfComponents != KJpgMinNumberOfComponents) && (iJpgFrameInfo.iNumberOfComponents != KJpgNumberOfComponents))
		{
		JPEG_LEAVE(KErrNotSupported, "Bad number of components");
		}

	for (TInt count = 0; count < iJpgFrameInfo.iNumberOfComponents; count++)
		{
		TJpgFrameInfo::TComponentInfo& compInfo = iJpgFrameInfo.iComponent[count];
		compInfo.iId = *ptr++;
		TUint8 sampleFactor = *ptr++;

		compInfo.iHorzSampleFactor = sampleFactor >> 4;
		if(compInfo.iHorzSampleFactor < KJpgMinSampleFactor || compInfo.iHorzSampleFactor > KJpgMaxSampleFactor)
			{
			JPEG_LEAVE(KErrCorrupt, "Bad horizontal sample factor");
			}

		compInfo.iVertSampleFactor = sampleFactor & 0x0f;
		if(compInfo.iVertSampleFactor < KJpgMinSampleFactor || compInfo.iVertSampleFactor > KJpgMaxSampleFactor)
			{
			JPEG_LEAVE(KErrCorrupt, "Bad vertical sample factor");
			}

		compInfo.iQTable = *ptr++;
		
		// Number of tables should be in range (0..3)
		if ((compInfo.iQTable < 0) || (compInfo.iQTable > (KJpgMaxNumberOfTables - 1)))
			{
			JPEG_LEAVE(KErrCorrupt, "Bad QTable index");				
			}
			
		iJpgFrameInfo.iMaxHorzSampleFactor = Max(iJpgFrameInfo.iMaxHorzSampleFactor,compInfo.iHorzSampleFactor);
		iJpgFrameInfo.iMaxVertSampleFactor = Max(iJpgFrameInfo.iMaxVertSampleFactor,compInfo.iVertSampleFactor);
		}

	if (iJpgFrameInfo.iNumberOfComponents == 1)
		{
		iJpgFrameInfo.iComponent[0].iHorzSampleFactor = 1;
		iJpgFrameInfo.iComponent[0].iVertSampleFactor = 1;
		iJpgFrameInfo.iMaxHorzSampleFactor = 1;
		iJpgFrameInfo.iMaxVertSampleFactor = 1;
		}
	else if (!iJFIFMarkerPresent)
		{
		if (iAdobeMarkerPresent && (iAdobeTransform != KAdobeColorTransformYCbCr))
			{
			JPEG_LEAVE(KErrNotSupported, "Unsupported Adobe transform");
			}
		else if (!iAdobeMarkerPresent)
			{
			// No markers present, so check for RGB and YCC, which are not supported.
			if ((iJpgFrameInfo.iComponent[0].iId == 82) && (iJpgFrameInfo.iComponent[1].iId == 71) && (iJpgFrameInfo.iComponent[2].iId == 66))
				{
				JPEG_LEAVE(KErrNotSupported, "RGB and YCC not supported");
				}

			if ((iJpgFrameInfo.iComponent[0].iId == 89) && (iJpgFrameInfo.iComponent[1].iId == 67) && (iJpgFrameInfo.iComponent[2].iId == 99))
				{
				JPEG_LEAVE(KErrNotSupported, "RGB and YCC not supported");
				}
			}
		}
	
	JPEG_DEBUG2("Frame max horiz sample factor: %d", iJpgFrameInfo.iMaxHorzSampleFactor);
	JPEG_DEBUG2("Frame max vert sample factor: %d", iJpgFrameInfo.iMaxHorzSampleFactor);
	}

void CJpegDecoder::ProcessStartOfScanL()
	{
	const TUint8* ptr = iPtr;
	iImageOffset = StartPosition() + iBlockLength - 2;
	iScanInfo.iImageOffset = iImageOffset;
	TJpgScanInfoProcessor::ProcessStartOfScanL(ptr, iJpgFrameInfo, iScanInfo, iDCHuffmanTable, iACHuffmanTable);
	}

void CJpegDecoder::ProcessRestartInterval()
	{
	const TUint8* ptr = iPtr;
	iJpgFrameInfo.iRestartInterval = ReadBigEndianUint16(ptr);
	JPEG_DEBUG2("Frame restart interval: %d", iJpgFrameInfo.iRestartInterval);
	}

void CJpegDecoder::ProcessCommentL()
	{
	if(iBlockLength < 2) // need at least 2 bytes for length field
		{
		JPEG_LEAVE(KErrCorrupt, "Not enough data for comment");
		}

	TPtrC8 commentDes(iPtr, iBlockLength - 2);
	HBufC8* comment = commentDes.AllocLC();
	JPEG_LEAVE_IF_ERROR(iComment.Append(comment), "Appending comment");
	CleanupStack::Pop(comment);
	}

void CJpegDecoder::FinishedProcessing()
	{
	SetStartPosition(iImageOffset);
	SetDataLength(KMaxTInt);

	TFrameInfo imageInfo;
	imageInfo = ImageInfo();
	imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0,0),iJpgFrameInfo.iSizeInPixels);
	imageInfo.iOverallSizeInPixels = iJpgFrameInfo.iSizeInPixels;
			
	imageInfo.iFrameSizeInTwips = iJpgFrameInfo.iSizeInTwips;
	imageInfo.iBitsPerPixel = 8 * iJpgFrameInfo.iNumberOfComponents;
	imageInfo.iDelay = 0;
	imageInfo.iFlags = TFrameInfo::ECanDither;
	if (iJpgFrameInfo.iNumberOfComponents > 1)
		imageInfo.iFlags |= TFrameInfo::EColor;

	// Set display mode.
	switch (imageInfo.iBitsPerPixel)
		{
	case 1:
		imageInfo.iFrameDisplayMode = EGray2;
		break;

	case 2:
		imageInfo.iFrameDisplayMode = EGray4;
		break;

	case 4:
		imageInfo.iFrameDisplayMode = (imageInfo.iFlags & TFrameInfo::EColor) ? EColor16 : EGray16;
		break;

	case 8:
		imageInfo.iFrameDisplayMode = (imageInfo.iFlags & TFrameInfo::EColor) ? EColor256 : EGray256;
		break;

	case 12:
		imageInfo.iFrameDisplayMode = EColor4K;
		break;

	case 16:
		imageInfo.iFrameDisplayMode = EColor64K;
		break;

	case 24:
		imageInfo.iFrameDisplayMode = EColor16M;
		break;
		}

	if ((DecoderOptions() & CImageDecoder::EOptionAutoRotate) && iExifDecoder) 
		{
		TUint16 orientation = 0;
		
		// if orientation tag is not found in the Ifd it is 0 
		if (iImageType == CImageDecoder::EImageTypeMain)
			{
			iExifDecoder->GetShortParam(0x0112, KExifIfdZero, orientation);
			}
		else if (iImageType == CImageDecoder::EImageTypeThumbnail)
			{
			iExifDecoder->GetShortParam(0x0112, KExifIfdOne, orientation);
			}

		// check for corrupted orientation tag value
		if (orientation > 8)
			{
			orientation = 0;
			}
		
		if (orientation >= 4 && orientation <= 8) 
			{
			imageInfo.iOverallSizeInPixels.iWidth = iJpgFrameInfo.iSizeInPixels.iHeight;
			imageInfo.iOverallSizeInPixels.iHeight = iJpgFrameInfo.iSizeInPixels.iWidth;
			// not sure we need the lines below but just in case
			imageInfo.iFrameSizeInTwips.iWidth = iJpgFrameInfo.iSizeInTwips.iHeight;
			imageInfo.iFrameSizeInTwips.iHeight = iJpgFrameInfo.iSizeInTwips.iWidth;
			imageInfo.iFrameSizeInPixels.iWidth = iJpgFrameInfo.iSizeInPixels.iHeight;
			imageInfo.iFrameSizeInPixels.iHeight = iJpgFrameInfo.iSizeInPixels.iWidth;
			imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), TPoint(iJpgFrameInfo.iSizeInPixels.iHeight, iJpgFrameInfo.iSizeInPixels.iWidth));
			}

		iAutoRotateFlag = orientation;
		}

	SetImageInfo(imageInfo);
	}

void CJpegDecoder::InitCustomAsyncL(TInt aParam)
	{
	if (aParam == KOptionConvertFrameUidValue)
		{
		HandleConvertFrameL();
		}
	else if (aParam == KOptionContinueConvertFrameUidValue)
		{
		HandleContinueConvertFrameL();
		}
	}

void CJpegDecoder::HandleConvertFrameL()
	{
	CImageDecoderPlugin::RequestInitL(FrameNumber());

	CJpgReadCodec* jpgReadCodec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());
	JPEG_ASSERT(jpgReadCodec);
	jpgReadCodec->InitFrameL(DstImageFrame());
	}

void CJpegDecoder::HandleContinueConvertFrameL()
	{
	CJpgReadCodec* jpgReadCodec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());
	JPEG_ASSERT(jpgReadCodec);
	CJpgImageFrameReadCodec* imageframeCodec = jpgReadCodec->ImageFrameCodec();
	if(imageframeCodec == NULL)
		{
		// ConvertFrame() should have been called to initiate the image frame processor
		// before calling ContinueConvertFrame()
		JPEG_LEAVE(KErrNotReady, "Continue called before Convert");
		}
	else
		{
		if (imageframeCodec->Destination() == NULL)
			{
			// ConvertFrame() should have been called to initiate the destination
			// before calling ContinueConvertFrame()
			JPEG_LEAVE(KErrNotReady, "Continue called before Convert");
			}
		}
	}

void CJpegDecoder::HandleCustomSyncL(TInt aParam)
	{
	CJpgReadCodec* codec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());
	JPEG_ASSERT(codec);
	
	if(aParam == CJpegYuvDecoder::EOptionYuvDecode)
		{
		codec->SetYuvDecode(ETrue);
		}
	else if(aParam == KOptionRecommendBufferSizeUidValue)
		{
		TUid formatCode = FormatCode();
		TInt bufferSize = codec->RecommendBufferSizeL(formatCode);
		SetRecommendedBufferSize(bufferSize);
		}
	else
		{
		JPEG_LEAVE(KErrNotSupported, "Unsupported custom sync");
		}
	}

void CJpegDecoder::NotifyComplete()
	{
	iState = EStateStart; // ensure that however we exit one run, start at top of next DoConvert()
	CJpgReadCodec* jpgReadCodec = REINTERPRET_CAST(CJpgReadCodec*, ImageReadCodec());
	JPEG_ASSERT(jpgReadCodec);
	
	if (iCodecState == EFrameComplete)
		{
		// should be safe to clear buffers here
		jpgReadCodec->CleanupBuffers();
		}

	jpgReadCodec->SetYuvDecode(EFalse);
	}

// System Wide Define
#ifdef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
void CJpegDecoder::SetClippingRectL(const TRect* aRect)
	{
	RPointerArray<TFrameInfo> frameInfo;
	CleanupClosePushL(frameInfo);
		
	// JPEG only has a single frame
	frameInfo.AppendL(&FrameInfo(0));
	
	// The clipping operation is reset by passing a NULL rect pointer.
	ASSERT(iExtensionManager);
	iExtensionManager->SetClippingRectL(aRect, frameInfo);
	CleanupStack::PopAndDestroy();	// frameInfo
	}
#else
void CJpegDecoder::SetClippingRectL(const TRect* /*aRect*/)
	{
	User::Leave(KErrNotSupported);
	}
#endif

TInt CJpegDecoder::SamplingScheme(TJpegImageData::TColorSampling& aSamplingScheme) const
	{
	ASSERT(iJpgFrameInfo.iMaxHorzSampleFactor != 0);
	ASSERT(iJpgFrameInfo.iMaxVertSampleFactor != 0);
	
	TInt retValue = KErrNotSupported;
	if (iJpgFrameInfo.iNumberOfComponents == 1)
		{
		aSamplingScheme = TJpegImageData::EMonochrome;
		retValue = KErrNone;
		}
	else
		{
		if (iJpgFrameInfo.iMaxHorzSampleFactor == 1 && iJpgFrameInfo.iMaxVertSampleFactor == 1)
			{
			aSamplingScheme = TJpegImageData::EColor444;
			retValue = KErrNone;
			}
		else if (iJpgFrameInfo.iMaxHorzSampleFactor == 2 && iJpgFrameInfo.iMaxVertSampleFactor == 1)
			{
			aSamplingScheme = TJpegImageData::EColor422;
			retValue = KErrNone;
			}
		else if (iJpgFrameInfo.iMaxHorzSampleFactor == 2 && iJpgFrameInfo.iMaxVertSampleFactor == 2)
			{
			aSamplingScheme = TJpegImageData::EColor420;
			retValue = KErrNone;
			}
		}
	return retValue;
	}

void CJpegEncoder::WriteThumbnailL()
	{
	iThumbnailSize=0;
	if(iGenerateThumbnail)
		{
		if (iExifEncoder)
			{
			// EXIF thumbnail is surrounded by SOI and EOI
			iThumbnailSize += WriteSOIL(Position());

			// encode and write the thumbnail
			iThumbnailSize += WriteThumbnailDataL();

			iThumbnailSize += WriteEOIL(Position());
			}
		else
			{
			iThumbnailSize += WriteThumbnailDataL();
			}
		}

	StartPosition() = Position();
	Position()=0;
	}


void CJpegEncoder::CreateScaledBitmapL(TRequestStatus*& aScaleCompletionStatus)
	{
	// Create the Scaled Thumbnail
	iSourceThumbnailImage=NULL;
	if(iThumbnailImage)
		{
		delete iThumbnailImage;
		iThumbnailImage=NULL;
		}
	if(iBitmapScaler)
		{
		delete iBitmapScaler;
		iBitmapScaler=NULL;
		}
	iThumbnailImage=new (ELeave) CFbsBitmap;

	CImageWriteCodec* imageWriteCodec=ImageWriteCodec();
	JPEG_ASSERT(imageWriteCodec);


	// the scaler does not modify the source bmp, so we apply it directly on this
	iSourceThumbnailImage=const_cast<CFbsBitmap*>(imageWriteCodec->Source());

	if (iFrameInfo.iNumberOfComponents == 1) 
		{
		//DEF113733: Monochrome image conversion so thumbnail should be grayscale
		TInt err = iThumbnailImage->Create(TSize(KThumbnailWidth, KThumbnailHeight), EGray256);
		JPEG_LEAVE_IF_ERROR(err, "Failed to create thumbnail bitmap");
		}
	else 
		{
		TInt err = iThumbnailImage->Create(TSize(KThumbnailWidth, KThumbnailHeight), KThumbnailDisplayMode);
		JPEG_LEAVE_IF_ERROR(err, "Failed to create thumbnail bitmap");
		}

	iBitmapScaler=CBitmapScaler::NewL();
	*aScaleCompletionStatus=KRequestPending;
	iBitmapScaler->Scale(aScaleCompletionStatus, *iSourceThumbnailImage, *iThumbnailImage);
	}

void CJpegEncoder::Cleanup()
	{
	if (iBitmapScaler)
		{
		iBitmapScaler->Cancel();
		}
	CImageEncoderPlugin::Cleanup();
	}

void CJpegEncoder::TransformBitmapL(CFbsBitmap& aSource, CFbsBitmap& aDest)
	{
	TPositionProcessor posProcess;
	TPoint pos;

	TUint8 *sourceBitmap = reinterpret_cast<TUint8*>( aSource.DataAddress() );

	CJpgWriteCodec::InitTransformCoordinates(posProcess, TRect(TPoint(0,0),TPoint(aSource.SizeInPixels().iWidth, aSource.SizeInPixels().iHeight)), TSize(KJpgPixelRatio, KJpgPixelRatio), iOperationExtPtr->iImgConvOperations);

	if(posProcess.SwapDimensions())
		{
		User::LeaveIfError(aDest.Create(TSize(iThumbnailImage->SizeInPixels().iHeight, iThumbnailImage->SizeInPixels().iWidth), KThumbnailDisplayMode));
		}
	else
		{
		User::LeaveIfError(aDest.Create(iThumbnailImage->SizeInPixels(), KThumbnailDisplayMode));
	    TInt tempDimension = iFrameInfo.iSizeInPixels.iHeight;
		iFrameInfo.iSizeInPixels.iHeight = iFrameInfo.iSizeInPixels.iWidth; // Y
		iFrameInfo.iSizeInPixels.iWidth = tempDimension; // X
		}

	TUint8 *destBitmap = reinterpret_cast<TUint8*>( aDest.DataAddress() );
	
	TInt srcScanlineWidth = aSource.ScanLineLength(aSource.SizeInPixels().iWidth, KThumbnailDisplayMode);
	TInt destScanlineWidth = aDest.ScanLineLength(aDest.SizeInPixels().iWidth, KThumbnailDisplayMode);
	
	TInt destWidthCount = 0;
	TInt destWidth = aDest.SizeInPixels().iWidth;

	do
		{
		posProcess.GetCurrentPosition(pos);
		TUint8* srcPtr = (sourceBitmap + (pos.iX + (pos.iY * (srcScanlineWidth))));
		if(++destWidthCount >= destWidth)
			{
			destBitmap = destBitmap + (destScanlineWidth - destWidth); //move the destination bitmap pointer past the padded width to make sure we don't write actual pixel data on target padded portion. Works only when thumbnail bitmap diaplay mode is EColor256.
			destWidthCount = 0;
			}	
		*destBitmap++ = *srcPtr;
		posProcess.MoveNext();
		} while (!posProcess.IsEndOfImage());
   	}

TInt CJpegEncoder::WriteThumbnailDataL()
	{
	//TUint8 cast only works because src and dest bitmap are EColor256
	ASSERT( KThumbnailDisplayMode == EColor256);

	// iSourceThumbnailImage is a pointer on the image of the codec
	// we just need to nullify if
	iSourceThumbnailImage=NULL;
	
	if(iBitmapScaler)
		{
		delete iBitmapScaler;
		iBitmapScaler=NULL;
		}
	if(iThumbnailImage==NULL)
		{
		User::Leave(KErrNotFound);
		}

	TInt writtenSize=0;

	CFbsBitmap* tranformedThumbnailImage = NULL;
	CFbsBitmap* thumbImage = NULL;

	thumbImage = iThumbnailImage;

	if(iOperationExtPtr)
		{
		tranformedThumbnailImage = new (ELeave) CFbsBitmap;
		CleanupStack::PushL(tranformedThumbnailImage);
		TransformBitmapL(*iThumbnailImage, *tranformedThumbnailImage);
		thumbImage = tranformedThumbnailImage;
		}


	if (iExifEncoder)
		{
		// write thumbnail as EXIF
		writtenSize = WriteThumbnailAsExifL(*thumbImage);
		}
	else
		{
		// write thumbnail as JFIF
		writtenSize = WriteThumbnailAsJfifL(*thumbImage);
		}

	delete iThumbnailImage;
	iThumbnailImage = NULL;
	if(iOperationExtPtr)
		{
		CleanupStack::PopAndDestroy(tranformedThumbnailImage);
		}

	return writtenSize;
	}

TInt CJpegEncoder::WriteThumbnailAsExifL(CFbsBitmap& aBitmap)
	{
	TInt writtenSize=0;
	CImageWriteCodec* thumbnailImageWriteCodec;

	thumbnailImageWriteCodec = CJpgWriteCodec::NewL(iFrameInfo,iQualityFactor,iLumaTable,iChromaTable,iComment,NULL);

	CleanupStack::PushL(thumbnailImageWriteCodec);
	// create the buffer for the thumbnail data
	HBufC8* encodeBuffer=HBufC8::NewL(KIOBlockSize);
	CleanupStack::PushL(encodeBuffer);

	TBufPtr8 encodeBufPtr;
	encodeBuffer->Des().FillZ();
	encodeBufPtr.Set(encodeBuffer->Des());

	thumbnailImageWriteCodec->InitFrameL(encodeBufPtr, aBitmap);

	WriteDataPositionIncL(Position(), encodeBufPtr);
	writtenSize+=encodeBufPtr.Length()*sizeof(TUint8);

	TFrameState frameErr;
	while((frameErr=thumbnailImageWriteCodec->ProcessFrameL(encodeBufPtr)) == EFrameIncomplete)
		{
		WriteDataPositionIncL(Position(), encodeBufPtr);
		writtenSize+=encodeBufPtr.Length()*sizeof(TUint8);
		}

	switch(frameErr)
		{
		case EFrameComplete:
			// happy ending
			WriteDataPositionIncL(Position(), encodeBufPtr);
			writtenSize+=encodeBufPtr.Length()*sizeof(TUint8);
			break;
		case EUnexpectedEndOfData:
		case EFrameIncompleteRepositionRequest:
			User::Leave(KErrEof);
			break;
		default:
			User::Leave(KErrUnknown);
			break;
		}

	CleanupStack::PopAndDestroy(encodeBuffer);
	CleanupStack::PopAndDestroy(thumbnailImageWriteCodec);

	return writtenSize;
	}


TInt CJpegEncoder::WriteThumbnailAsJfifL(CFbsBitmap& aBitmap)
	{
	TInt writtenSize=0;
	// CBitmapScaler might not produce exact KThumbnailWidth * KThumbnailHeight
	// so get the actual size
	iGeneratedThumbnailSize = aBitmap.SizeInPixels();

	HBufC8* buffer = HBufC8::NewL(iGeneratedThumbnailSize.iWidth * iGeneratedThumbnailSize.iHeight * 3);
	CleanupStack::PushL(buffer);
	TBufPtr8 bufferPtr;
	bufferPtr.Set(buffer->Des());

	TPoint currentPos(0, 0);
	TBitmapUtil bitmapUtil(&aBitmap);
	bitmapUtil.Begin(currentPos);
	for (; currentPos.iY < iGeneratedThumbnailSize.iHeight; currentPos.iY++)
		{
		currentPos.iX = 0;
		bitmapUtil.SetPos(currentPos);
		TRgb currentPixel;
		for (; currentPos.iX < iGeneratedThumbnailSize.iWidth; currentPos.iX++)
			{
			if (iFrameInfo.iNumberOfComponents == 1)
				{
				//DEF113733: if number of components is 1, we want to use grey scale pixels
				currentPixel = TRgb::Gray256(bitmapUtil.GetPixel());
				}
			else 
				{
				currentPixel = TRgb::Color256(bitmapUtil.GetPixel());
				}
				
			bufferPtr.Append(currentPixel.Red());
			bufferPtr.Append(currentPixel.Green());
			bufferPtr.Append(currentPixel.Blue());

			bitmapUtil.IncXPos();
			}
		}
	bitmapUtil.End();

	WriteDataPositionIncL(Position(), bufferPtr);
	writtenSize += bufferPtr.Length() * sizeof(TUint8);

	CleanupStack::PopAndDestroy(buffer);

	return writtenSize;
	}

void CJpegEncoder::WriteExifDataL(TRequestStatus*& aScaleCompletionStatus)
	{
	User::LeaveIfError( iThumbnailStatus );

	if (iExifEncoder)
		{
		// we get the exifData
		// exif data is own by the exif encoder. We don't push it on the cleanupstack
		iExifDataPosition=Position();
		TPtrC8 exifDataStr=iExifEncoder->CreateExifHeaderL();
		// and write it
		iExifSize=exifDataStr.Length()*sizeof(TUint8);
		WriteDataPositionIncL(iExifDataPosition, exifDataStr);
		}

	if(iGenerateThumbnail)
		{
		CreateScaledBitmapL(aScaleCompletionStatus);
		}
	else
		{
		// the framework waits for	aScaleCompletionStatus to  complete
		SelfComplete(KErrNone);
		}
	}

// Jpeg encoder.
CJpegEncoder* CJpegEncoder::NewL()
	{
	CJpegEncoder* self=new(ELeave) CJpegEncoder;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

void CJpegEncoder::ConstructL()
	{
	}

CJpegEncoder::CJpegEncoder()
	: iGenerateThumbnail(EFalse)
	{
	// Set relevant defaults for the write codec.
	// Output precision is 8, not progressive, no restarts and no thumbnail
	iFrameInfo.iNumberOfComponents = 3;
	iFrameInfo.iComponent[0].iHorzSampleFactor = 2;
	iFrameInfo.iComponent[0].iVertSampleFactor = 2;
	iFrameInfo.iComponent[0].iQTable = 0;
	iFrameInfo.iComponent[1].iHorzSampleFactor = 1;
	iFrameInfo.iComponent[1].iVertSampleFactor = 1;
	iFrameInfo.iComponent[1].iQTable = 1;
	iFrameInfo.iComponent[2].iHorzSampleFactor = 1;
	iFrameInfo.iComponent[2].iVertSampleFactor = 1;
	iFrameInfo.iComponent[2].iQTable = 1;
	iFrameInfo.iMaxHorzSampleFactor = 2;
	iFrameInfo.iMaxVertSampleFactor = 2;

	iExtOperationActive = ENone;
	}

CJpegEncoder::~CJpegEncoder()
	{
	Cleanup();
	delete iLumaTable;
	delete iChromaTable;
	delete iExifEncoder;
	
	iSourceThumbnailImage = NULL;
	delete iBitmapScaler;
	delete iThumbnailImage;
	
	delete iOperationExtPtr;
	delete iStreamedEncodeExt;
	
	iComment.ResetAndDestroy();
	}

MExifMetadata* CJpegEncoder::ExifMetadata()
	{
	if (!iExifEncoder)
		{
		// set thumbnail on for EXIF (ensure continuity of behaviour)
		iGenerateThumbnail = ETrue;

		// creating the CExifEncoder has the required side-effect
		// of switching from JFIF to EXIF header output
		TInt err;
		TRAP(err, iExifEncoder = CExifEncoder::NewL(iGenerateThumbnail));
		}
	return iExifEncoder;
	};


void CJpegEncoder::PrepareEncoderL(const CFrameImageData* aFrameImageData)
	{
	iQualityFactor = 0;

	TInt count = (aFrameImageData) ? aFrameImageData->ImageDataCount() : 0;

	if (count == 0)
		{
		// Use the default value.
		// Note: This default value is also used by the TJpegImageData default
		// constructor.  So if it changed to a different value here, then it should
		// be changed there as well.
		iQualityFactor = 75;
		}

	TBool jpgImageDataProcessed = EFalse;
	for (TInt index = 0 ; index<count ; index++)
		{
		const TImageDataBlock& encoderData = *aFrameImageData->GetImageData(index);
		if (encoderData.DataType() == KJPGImageDataUid)
			{
			// Leave if we have already processed one of these.
			if (jpgImageDataProcessed)
				User::Leave(KErrCorrupt);

			const TJpegImageData& jpegImageData = STATIC_CAST(const TJpegImageData&, encoderData);
			iQualityFactor = jpegImageData.iQualityFactor;

			TJpegImageData::TColorSampling sampleScheme = jpegImageData.iSampleScheme;
			iFrameInfo.iComponent[0].iQTable = 0;
			if (sampleScheme == TJpegImageData::EMonochrome)
				{
				iFrameInfo.iNumberOfComponents = 1;
				iFrameInfo.iComponent[0].iHorzSampleFactor = 1;
				iFrameInfo.iComponent[0].iVertSampleFactor = 1;
				}
			else
				{
				iFrameInfo.iNumberOfComponents = 3;
				switch (sampleScheme)
					{
				case TJpegImageData::EColor420:
					iFrameInfo.iComponent[0].iHorzSampleFactor = 2;
					iFrameInfo.iComponent[0].iVertSampleFactor = 2;
					iFrameInfo.iComponent[1].iVertSampleFactor = 1;
					iFrameInfo.iComponent[2].iVertSampleFactor = 1;
					break;
				case TJpegImageData::EColor422:
					iFrameInfo.iComponent[0].iHorzSampleFactor = 2;
					iFrameInfo.iComponent[0].iVertSampleFactor = 1;
					iFrameInfo.iComponent[1].iVertSampleFactor = 1;
					iFrameInfo.iComponent[2].iVertSampleFactor = 1;
					break;
				case TJpegImageData::EColor444:
					iFrameInfo.iComponent[0].iHorzSampleFactor = 1;
					iFrameInfo.iComponent[0].iVertSampleFactor = 1;
					iFrameInfo.iComponent[1].iVertSampleFactor = 1;
					iFrameInfo.iComponent[2].iVertSampleFactor = 1;
					break;
				default:
					User::Leave(KErrNotSupported);
					}

				iFrameInfo.iComponent[1].iHorzSampleFactor = 1;
				iFrameInfo.iComponent[1].iQTable = 1;
				iFrameInfo.iComponent[2].iHorzSampleFactor = 1;
				iFrameInfo.iComponent[2].iQTable = 1;
				}

			iFrameInfo.iMaxHorzSampleFactor = iFrameInfo.iComponent[0].iHorzSampleFactor;
			iFrameInfo.iMaxVertSampleFactor = iFrameInfo.iComponent[0].iVertSampleFactor;

			jpgImageDataProcessed = ETrue;
			}
		else if (encoderData.DataType() == KJPGQTableUid)
			{
			const TJpegQTable& jpegQTable = STATIC_CAST(const TJpegQTable&, encoderData);
			TInt tableIndex = jpegQTable.iTableIndex;
			if ((tableIndex != TJpegQTable::ELumaTable) && (tableIndex != TJpegQTable::EChromaTable))
				User::Leave(KErrNotSupported);

			TUint8 values[KJpgQTableEntries];
			TUint8* valuePtr = values;
			const TUint8* zigZagPtr = KZigZagSequence.iZigZag;
			const TUint8* valuePtrLimit = valuePtr + KJpgQTableEntries;

			while (valuePtr < valuePtrLimit)
				*valuePtr++ = jpegQTable.iEntries[*zigZagPtr++];

			if (tableIndex == TJpegQTable::ELumaTable)
				{
				if (!iLumaTable)
					iLumaTable = new(ELeave) TQTable;

				iLumaTable->Set(values, EFalse);
				}
			else if (tableIndex == TJpegQTable::EChromaTable)
				{
				if (!iChromaTable)
					iChromaTable = new(ELeave) TQTable;

				iChromaTable->Set(values, EFalse);
				}
			else
				User::Leave(KErrNotSupported);
			}
		else if (encoderData.DataType() == KJPGCommentUid)
			{
			const TJpegComment& jpegComment = STATIC_CAST(const TJpegComment&, encoderData);
			if (!jpegComment.iComment)
				User::Leave(KErrNotFound);

			if ((jpegComment.iComment->Length() == 0) || (jpegComment.iComment->Length() > 65534))
				User::Leave(KErrNotSupported);

			HBufC8* comment = jpegComment.iComment->AllocL();
			TInt ret = iComment.Append(comment);
			if (ret != KErrNone)
				{
				delete comment;
				User::Leave(ret);
				}
			}
		else
			User::Leave(KErrCorrupt);
		}


	// Create codec.
	ASSERT(ImageWriteCodec() == NULL);

	CJpgWriteCodec* imageWriteCodec;

	if(!iOperationExtPtr)
		{
		imageWriteCodec = CJpgWriteCodec::NewL(iFrameInfo,iQualityFactor,iLumaTable,iChromaTable,iComment, NULL);
		}
	else
		{
		imageWriteCodec = CJpgWriteCodec::NewL(iFrameInfo,iQualityFactor,iLumaTable,iChromaTable,iComment, &iOperationExtPtr->iImgConvOperations);
		}
	
	imageWriteCodec->SetHighSpeedMode( (EncoderOptions() & CImageEncoder::EPreferFastEncode) == CImageEncoder::EPreferFastEncode);
	
	SetImageWriteCodec(imageWriteCodec);

	}

void CJpegEncoder::SetThumbnail(TBool aDoGenerateThumbnail)
	{
	if (iExifEncoder)
		{
		TRAP(iThumbnailStatus, iExifEncoder->SetEnableThumbnailL(aDoGenerateThumbnail) );
		if (iThumbnailStatus != KErrNone)
			{
			iGenerateThumbnail = EFalse;
			return;
			}
		}
	iGenerateThumbnail = aDoGenerateThumbnail;
	}

void CJpegEncoder::UpdateHeaderL()
	{
	User::LeaveIfError( iThumbnailStatus );
	// Codec writes everything, except for EOI
	WriteEOIL(StartPosition()+Position());

	if (iExifEncoder)
		{
		TBuf8<2> buffer2Bytes;
		buffer2Bytes.SetLength(2);

		TUint8* headerPtr2 = &buffer2Bytes[0];

		WriteBigEndianUint16(headerPtr2, iExifSize + iThumbnailSize - KApp1MarkerByteCount); // APP1 marker is not included in the length

		// Since we are at the end of an encode and we have an explicit offset to the buffer
		// (i.e. to write the size of the APP1 header near the start of the buffer, and so
		// Position() is irrelevant) we can use WriteDataL.
		WriteDataL(iExifDataPosition+iExifEncoder->ExifLengthOffset(),buffer2Bytes);

		if (iGenerateThumbnail)
			{
			TBuf8<4> buffer4Bytes;
			buffer4Bytes.SetLength(4);
			TUint8* headerPtr4 = &buffer4Bytes[0];

			Mem::Copy(headerPtr4, &iThumbnailSize, sizeof(TUint));
			// Write the offset of the thumbnail (from the start of the buffer) in the
			// APP1 header.  Again, Position() is irrelevant and so use WriteDataL.
			WriteDataL(iExifDataPosition+iExifEncoder->ThumbnailLengthOffset(),buffer4Bytes);
			}
		}
	else if (iGenerateThumbnail)
		{
		TInt headerSize = iThumbnailSize + KJfifApp0DataSize;
		TBuf8<2> buffer2Bytes;
		buffer2Bytes.Append(TUint8((headerSize & 0xff00) >> 8));
		buffer2Bytes.Append(TUint8(headerSize & 0xff));
		WriteDataL(KJfifLengthOffset, buffer2Bytes);

		buffer2Bytes.Zero();
		buffer2Bytes.Append(iGeneratedThumbnailSize.iWidth);
		buffer2Bytes.Append(iGeneratedThumbnailSize.iHeight);

		WriteDataL(KJfifThumbnailXYOffset, buffer2Bytes);
		}
	}

void CJpegEncoder::InitConvertL()
	{
	CImageEncoderPlugin::InitConvertL();
	// this way, the bitmap is initialized

	WriteSOIL(StartPosition()+Position());
	if (!iExifEncoder)
		{
		// no EXIF data so default to JFIF header
		WriteImageHeaderL();
		}
	}

void CJpegEncoder::InitCustomAsyncL(TInt aParam)
	{
	if (aParam == KOptionConvertFrameUidValue)
		{
		HandleConvertFrameL();
		}
	}

void CJpegEncoder::HandleConvertFrameL()
	{
	// Prepare Frame Info and instantiates write codec
	PrepareEncoderL(&FrameImageData());

	// ConvertFrame option does not support thumbnail
	SetThumbnail(EFalse);

	// ConvertFrame bypasses the framework. Force required initialization here.
	CImageEncoderPlugin::RequestInitL();

	// Initialize write codec with relevant data
	// This will also check consistency between FrameImageData and the souce ImageFrame format.
	CJpgWriteCodec* jpgWriteCodec = reinterpret_cast<CJpgWriteCodec*>(ImageWriteCodec());
	TBufPtr8& destData = DestinationData();
	jpgWriteCodec->InitFrameL(destData, &SrcImageFrame(), &FrameImageData());

	// If everything went fine start writting data to the destination file.
	WriteSOIL(StartPosition()+Position());
	WriteImageHeaderL();

	// If this image has exif metadata, then generate the exif data and write it
	if (iExifEncoder)
		{
		// we get the exifData
		// exif data is own by the exif encoder. We don't push it on the cleanupstack
		iExifDataPosition=Position();
		TPtrC8 exifDataStr=iExifEncoder->CreateExifHeaderL();
		// and write it
		iExifSize=exifDataStr.Length()*sizeof(TUint8);
		WriteDataPositionIncL(iExifDataPosition, exifDataStr);
		}
	}

void CJpegEncoder::HandleStreamInitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, const CFrameImageData* aFrameImageData)
	{
	// Prepare Frame Info and instantiates write codec
	PrepareEncoderL(aFrameImageData);

	// Stream option does not support thumbnail
	SetThumbnail(EFalse);

	// ConvertFrame bypasses the framework. Force required initialization here.
	CImageEncoderPlugin::RequestInitL();

	// Initialize write codec with relevant data
	// This will also check consistency between FrameImageData and the souce ImageFrame format.
	CJpgWriteCodec* jpgWriteCodec = reinterpret_cast<CJpgWriteCodec*>(ImageWriteCodec());
	TBufPtr8& destData = DestinationData();
	jpgWriteCodec->InitFrameL(destData, aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aFrameImageData);

	// If everything went fine start writting data to the destination file.
	WriteSOIL(StartPosition()+Position());
	WriteImageHeaderL();

	// If this image has exif metadata, then generate the exif data and write it
	if (iExifEncoder)
		{
		// we get the exifData
		// exif data is own by the exif encoder. We don't push it on the cleanupstack
		iExifDataPosition=Position();
		TPtrC8 exifDataStr=iExifEncoder->CreateExifHeaderL();
		// and write it
		iExifSize=exifDataStr.Length()*sizeof(TUint8);
		WriteDataPositionIncL(iExifDataPosition, exifDataStr);
		}
	}

void CJpegEncoder::HandleStreamAppendBlock(const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	// Initialize write codec with relevant data
	// This will also check consistency between FrameImageData and the souce ImageFrame format.
	CJpgWriteCodec* jpgWriteCodec = reinterpret_cast<CJpgWriteCodec*>(ImageWriteCodec());
	JPEG_ASSERT(jpgWriteCodec);
	
	TRAPD(errCode, jpgWriteCodec->AppendFrameBlockL(aBlocks, aNumBlocksToAdd));
	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}
	}

void CJpegEncoder::HandleStreamAddBlock(const CImageFrame& /*aBlocks*/, const TInt& /*aSeqPosition*/)
	{
	//Add blocks is not supported
	RequestComplete(KErrNotSupported);
	return;
	}

void CJpegEncoder::HandleStreamCompleteFrame()
	{
	CJpgWriteCodec* jpgWriteCodec = reinterpret_cast<CJpgWriteCodec*>(ImageWriteCodec());
	JPEG_ASSERT(jpgWriteCodec);
	
	jpgWriteCodec->SetCompleteFrame();
	}

TInt CJpegEncoder::WriteSOIL(const TInt aPosition)
	{
	// Codec writes everything, except for EOI
	TBuf8<2> buffer2Bytes;
	buffer2Bytes.SetLength(2);

	TUint8* headerPtr = &buffer2Bytes[0];
	WriteBigEndianUint16(headerPtr,KJpgSOISignature); // Start of information

	WriteDataPositionIncL(aPosition,buffer2Bytes);

	return buffer2Bytes.Length()*sizeof(TUint8);
	}



TInt CJpegEncoder::WriteImageHeaderL()
	{
	TBuf8<KJfifApp0DataSize+sizeof(KJpgApp0Signature)> buffer;

	TJpegUtilities::CreateJfifHeader(buffer);
	WriteDataPositionIncL(StartPosition()+Position(),buffer);

	return buffer.Length()*sizeof(TUint8);
	}
//Helper function to crate a default JFIF header
void TJpegUtilities::CreateJfifHeader(TDes8& aBuffer)
	{
	ASSERT(aBuffer.MaxLength() >= (KJfifApp0DataSize+sizeof(KJpgApp0Signature)));

	TUint8* destPtr = const_cast<TUint8*>(aBuffer.Ptr());
	ASSERT(destPtr != NULL);
	TUint8* startDestPtr=destPtr;

	WriteBigEndianUint16(destPtr,KJpgApp0Signature); // APP0
	WriteBigEndianUint16(destPtr,KJfifApp0DataSize); // Lp
	Mem::Copy(destPtr,KJfifId().Ptr(),KJpgApp0IdSize);
	destPtr += KJpgApp0IdSize;
	*destPtr++ = 0;
	WriteBigEndianUint16(destPtr,KJfifVersion0101); // Version
	*destPtr++ = TJpgFrameInfo::ENone; // Units
	WriteBigEndianUint16(destPtr,KJpgPixelRatio); // X density
	WriteBigEndianUint16(destPtr,KJpgPixelRatio); // Y density
	*destPtr++ = 0; // Thumbnail width
	*destPtr++ = 0; // Thumbnail height

	aBuffer.SetLength((destPtr-startDestPtr)/sizeof(destPtr[0]));
	}


TInt CJpegEncoder::WriteEOIL(const TInt aPosition)
	{
	// Codec writes everything, except for EOI
	TBuf8<2> buffer2Bytes;
	buffer2Bytes.SetLength(2);

	TUint8* headerPtr = &buffer2Bytes[0];
	WriteBigEndianUint16(headerPtr, KJpgEOISignature); // End of information
	WriteDataPositionIncL(aPosition,buffer2Bytes);

	return buffer2Bytes.Length()*sizeof(TUint8);
	}

CJpegEncoder::TExtensionOperation CJpegEncoder::GetActiveExtensionOperation() const
	{
	return iExtOperationActive;
	}

void CJpegEncoder::SetActiveExtensionOperation(CJpegEncoder::TExtensionOperation aExtOperation)
	{
	iExtOperationActive = aExtOperation;
	}

// System Wide Define
#ifdef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
void CJpegEncoder::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	switch (aExtUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			if (!iOperationExtPtr)
				{
				iOperationExtPtr = CEncodeOperationExtension::NewL(this);
				aExtPtr = iOperationExtPtr;
				}
			else
				{
				aExtPtr = iOperationExtPtr;
				}

			break;
			}

		case KUidImageConvExtStreamedEncodeValue:
			{
			if (!iStreamedEncodeExt)
				{
				iStreamedEncodeExt = CStreamedEncodeExtension::NewL(this);
				aExtPtr = iStreamedEncodeExt;
				}
			else
				{
				aExtPtr = iStreamedEncodeExt;
				}

			break;
			}

		default:
			{
			User::Leave(KErrNotSupported);
			}
		}
	}
#else
void CJpegEncoder::GetExtensionL(TUid /*aExtUid*/, MImageConvExtension*& /*aExtPtr*/)
	{
	User::Leave(KErrNotSupported);
	}
#endif

CEncodeOperationExtension* CEncodeOperationExtension::NewL(CJpegEncoder* aJpegEncoder)
	{
	return new (ELeave) CEncodeOperationExtension(aJpegEncoder);
	}

CEncodeOperationExtension::CEncodeOperationExtension(CJpegEncoder* aJpegEncoder)
:iJpegEncoder(aJpegEncoder), iOperationCaps(KJpgEncoderCapabilities)
	{
	IncrementRef();
	}

CEncodeOperationExtension::~CEncodeOperationExtension()
	{
	ASSERT(iReferenceCount == 0);
	iImgConvOperations.Close();
	}

TUid CEncodeOperationExtension::Uid() const
	{
	TUid KUidImgCovOperation = {KUidImageConvExtOperationValue};
	return KUidImgCovOperation;
	}

void CEncodeOperationExtension::IncrementRef()
	{
	iReferenceCount++;
	}

void CEncodeOperationExtension::Release()
	{
	iReferenceCount--;
	ASSERT( iReferenceCount >= 0 );
	}

TUint CEncodeOperationExtension::Capabilities() const
	{
	return iOperationCaps;
	}

void CEncodeOperationExtension::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	User::LeaveIfError(iImgConvOperations.Append(aOperation));
	//check to exclude interoperability of stream and operation interfaces.
	if(iJpegEncoder->GetActiveExtensionOperation() == CJpegEncoder::ENone)
		{
		iJpegEncoder->SetActiveExtensionOperation(CJpegEncoder::EOperationEncode);
		}
	else if(iJpegEncoder->GetActiveExtensionOperation() == CJpegEncoder::EStreamEncode)
		{
		User::Leave(KErrNotSupported);
		}
	}

void CEncodeOperationExtension::ClearOperationStack()
	{
	if(iJpegEncoder->GetActiveExtensionOperation() == CJpegEncoder::EOperationEncode)
		{
		iImgConvOperations.Reset();
		iJpegEncoder->SetActiveExtensionOperation(CJpegEncoder::ENone);
		}
	}

// Concrete Streamed Encode Extension class
CStreamedEncodeExtension* CStreamedEncodeExtension::NewL(CJpegEncoder* aJpegEncoder)
	{
	return new (ELeave) CStreamedEncodeExtension(aJpegEncoder);
	}

CStreamedEncodeExtension::CStreamedEncodeExtension(CJpegEncoder* aJpegEncoder)
:iJpegEncoder(aJpegEncoder), iReferenceCount(0)
	{
	IncrementRef();
	}

CStreamedEncodeExtension::~CStreamedEncodeExtension()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CStreamedEncodeExtension::Uid() const
	{
	TUid KUidImgCovOperation = {KUidImageConvExtStreamedEncodeValue};
	return KUidImgCovOperation;
	}

void CStreamedEncodeExtension::IncrementRef()
	{
	iReferenceCount++;
	}

void CStreamedEncodeExtension::Release()
	{
	iReferenceCount--;
	ASSERT( iReferenceCount >= 0 );
	}

void CStreamedEncodeExtension::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	aFormats.AppendL(KUidFormatYUVMonochrome);
	aFormats.AppendL(KUidFormatYUV422Interleaved);
	aFormats.AppendL(KUidFormatYUV420Planar);
	aFormats.AppendL(KUidFormatYUV420PlanarReversed);

	aOptimalFormat = KUidFormatYUV420Planar;
	}

void CStreamedEncodeExtension::GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const
	{
	TSize minBlockSizeInPixels;
	TInt optimalBlocksPerRequest = KOptimalBlockSize;
	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
	TInt maxBlocksPerRequest= KMaxTInt;

	switch (aFormat.iUid)
		{
		case KFormatYUVMonochromeUidValue:
			minBlockSizeInPixels  = TSize(KSamplingMonoMCUWidthInPixels, KSamplingMonoMCUHeightInPixels);
			break;
		case KFormatYUV422InterleavedUidValue:
			minBlockSizeInPixels  = TSize(KSampling422MCUWidthInPixels, KSampling422MCUHeightInPixels);
			break;
		case KFormatYUV420PlanarReversedUidValue:
		case KFormatYUV420PlanarUidValue:
			minBlockSizeInPixels  = TSize(KSampling420MCUWidthInPixels, KSampling420MCUHeightInPixels);
			break;
		}

	TEncodeStreamCaps streamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
								optimalBlocksPerRequest, navigation);
	aCaps = streamCaps;
	}

void CStreamedEncodeExtension::InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	ValidateInitParamsL(aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aNavigation, aFrameImageData);

	iJpegEncoder->HandleStreamInitFrameL(aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aFrameImageData);

	//check to exclude interoperability of stream and operation interfaces.
	if(iJpegEncoder->GetActiveExtensionOperation() == CJpegEncoder::ENone)
		{
		iJpegEncoder->SetActiveExtensionOperation(CJpegEncoder::EStreamEncode);
		}
	else if(iJpegEncoder->GetActiveExtensionOperation()== CJpegEncoder::EOperationEncode)
		{
		User::Leave(KErrNotSupported);
		}
	}

void CStreamedEncodeExtension::AppendBlocks(TRequestStatus* /*aReq*/, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	iJpegEncoder->HandleStreamAppendBlock(aBlocks, aNumBlocksToAdd);
	}

void CStreamedEncodeExtension::AddBlocks(TRequestStatus* /*aReq*/, const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	iJpegEncoder->HandleStreamAddBlock(aBlocks, aSeqPosition);
	}

void CStreamedEncodeExtension::Complete(TRequestStatus* /*aReq*/)
	{
	if(iJpegEncoder->GetActiveExtensionOperation() == CJpegEncoder::EStreamEncode)
		{
		iJpegEncoder->HandleStreamCompleteFrame();
		iJpegEncoder->SetActiveExtensionOperation(CJpegEncoder::ENone);
		}
	}

void CStreamedEncodeExtension::ValidateInitParamsL(TUid& aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	// Prepare Frame Info and instantiates write codec
	// Note : Can either specify format through aFormat or aImageFrameData. Conflicts should leave with KErrArgument.
	if (aFormat.iUid == KFormatYUVMonochromeUidValue || aFormat.iUid == KFormatYUV422InterleavedUidValue || aFormat.iUid == KFormatYUV420PlanarUidValue || aFormat.iUid == KFormatYUV420PlanarReversedUidValue)
		{
		if(aFrameNumber)
			{
			User::Leave(KErrArgument);
			}
		}
	else
		{
		User::Leave(KErrArgument);
		}

	TInt count = (aFrameImageData) ? aFrameImageData->ImageDataCount() : 0;
	// look for Frame Image Data
	for (TInt index = 0 ; index<count ; index++)
		{
		const TImageDataBlock& encoderData = *aFrameImageData->GetImageData(index);
		if (encoderData.DataType() == KJPGImageDataUid)
			{
			const TJpegImageData& jpegImageData = static_cast<const TJpegImageData&>(encoderData);
			TJpegImageData::TColorSampling sampleScheme = jpegImageData.iSampleScheme;

			switch (sampleScheme)
				{
				case TJpegImageData::EMonochrome:
					{
					if (aFormat.iUid != KFormatYUVMonochromeUidValue)
						{
						User::Leave(KErrNotSupported);
						}
					break;
					}

				case TJpegImageData::EColor420:
					{
					if (!(aFormat.iUid == KFormatYUV420PlanarUidValue || aFormat.iUid == KFormatYUV420PlanarReversedUidValue))
						{
						User::Leave(KErrNotSupported);
						}
					break;
					}

				case TJpegImageData::EColor422:
					{
					if (aFormat.iUid != KFormatYUV422InterleavedUidValue)
						{
						User::Leave(KErrNotSupported);
						}
					break;
					}

				case TJpegImageData::EColor444:
					{
					// YUV 4:4:4 sampling scheme is not supported by this codec
					User::Leave(KErrNotSupported);
					break;
					}

				default:
					{
					User::Leave(KErrNotSupported);
					break;
					}
				}
			//format found - break out of for loop
			break;
			}
		}

	GetCapabilities(aFormat, iEncodeCaps);

	//validate that the frame size used is multiple of min block size
	CJpgWriteCodec::ValidateBlockSizeL(aFrameSizeInPixels, iEncodeCaps.MinBlockSizeInPixels());

	//validate that the block size used is multiple of min block size
	CJpgWriteCodec::ValidateBlockSizeL(aBlockSizeInPixels, iEncodeCaps.MinBlockSizeInPixels());

	if (aBlockSizeInPixels.iHeight != iEncodeCaps.MinBlockSizeInPixels().iHeight)
		{
		User::Leave(KErrNotSupported);
		}

	if(aNavigation != iEncodeCaps.Navigation())
		{
		User::Leave(KErrNotSupported);
		}
	}
// Concrete Streamed Decode Extension class
CStreamedDecodeExtension* CStreamedDecodeExtension::NewL(CJpegDecoder* aJpegDecoder)
	{
	return new (ELeave) CStreamedDecodeExtension(aJpegDecoder);
	}

CStreamedDecodeExtension::CStreamedDecodeExtension(CJpegDecoder* aJpegDecoder)
:iJpegDecoder(aJpegDecoder)
	{
	IncrementRef();
	}

CStreamedDecodeExtension::~CStreamedDecodeExtension()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CStreamedDecodeExtension::Uid() const
	{
	TUid uidImgConvOperation = {KUidImageConvExtStreamedDecodeValue};
	return uidImgConvOperation;
	}

void CStreamedDecodeExtension::IncrementRef()
	{
	iReferenceCount++;
	}

void CStreamedDecodeExtension::Release()
	{
	iReferenceCount--;
	ASSERT( iReferenceCount >= 0 );
	}

void CStreamedDecodeExtension::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	TJpegImageData::TColorSampling samplingScheme = TJpegImageData::EMonochrome; // intialise to mono
	User::LeaveIfError(iJpegDecoder->SamplingScheme(samplingScheme));
	
	switch(samplingScheme)
		{
		case TJpegImageData::EMonochrome:
			aFormats.AppendL(KUidFormatYUVMonochrome);
			aOptimalFormat = KUidFormatYUVMonochrome;
			break;
		case TJpegImageData::EColor420:
			aFormats.AppendL(KUidFormatYUV420Planar);
			aFormats.AppendL(KUidFormatYUV420PlanarReversed);
			aOptimalFormat = KUidFormatYUV420Planar;
			break;
		case TJpegImageData::EColor422:
			aFormats.AppendL(KUidFormatYUV422Interleaved);
			aOptimalFormat = KUidFormatYUV422Interleaved;
			break;
		default:
			User::Leave(KErrNotSupported);
		}
	}

void CStreamedDecodeExtension::GetCapabilities(TUid aFormat, TInt /*aFrameNumber*/, TDecodeStreamCaps& aCaps) const
	{
	TSize minBlockSizeInPixels;
	TInt optimalBlocksPerRequest = KOptimalBlockSize;
	TDecodeStreamCaps::TNavigation navigation = static_cast<TDecodeStreamCaps::TNavigation> (TDecodeStreamCaps::ENavigationSequentialForward | TDecodeStreamCaps::ENavigationRandomForward | TDecodeStreamCaps::ENavigationRandomBackwards);

	TInt maxBlocksPerRequest= KMaxTInt;
	TInt minStreamSizeInBlocks = 1;

	switch (aFormat.iUid)
		{
		case KFormatYUVMonochromeUidValue:
			minBlockSizeInPixels = TSize(KSamplingMonoMCUWidthInPixels, KSamplingMonoMCUHeightInPixels);
			break;
		case KFormatYUV422InterleavedUidValue:
			minBlockSizeInPixels = TSize(KSampling422MCUWidthInPixels, KSampling422MCUHeightInPixels);
			break;
		case KFormatYUV420PlanarReversedUidValue:
		case KFormatYUV420PlanarUidValue:
			minBlockSizeInPixels = TSize(KSampling420MCUWidthInPixels, KSampling420MCUHeightInPixels);
			break;
		default:
			//does nothing
			maxBlocksPerRequest = 0;
		}

	TDecodeStreamCaps streamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
								optimalBlocksPerRequest, minStreamSizeInBlocks, navigation);
	aCaps = streamCaps;
	}

TInt CStreamedDecodeExtension::GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	return iJpegDecoder->HandleStreamGetBufferSize(aFormat, aBlockSizeInPixels, aNumBlocks);
	}

void CStreamedDecodeExtension::InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	if(aNavigation == TDecodeStreamCaps::ENavigationSequentialForward || aNavigation == TDecodeStreamCaps::ENavigationRandomForward || aNavigation == TDecodeStreamCaps::ENavigationRandomBackwards)
		{
		if(aFrameNumber)
			{
			User::Leave(KErrArgument);
			}
		}
	else
		{
		User::Leave(KErrNotSupported);
		}

	iJpegDecoder->HandleStreamInitFrameL(aFormat, aFrameNumber, aNavigation);
	}

void CStreamedDecodeExtension::GetBlocks(TRequestStatus* /*aStatus*/, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	iJpegDecoder->HandleStreamGetBlocks(aFrame, aSeqPosition, aNumBlocksToGet, aNumBlocksRead);
	}

void CStreamedDecodeExtension::GetNextBlocks(TRequestStatus* /*aStatus*/, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	iJpegDecoder->HandleStreamGetNextBlocks(aFrame, aNumBlocksToGet, aNumBlocksRead, aHaveMoreBlocks);
	}

//return The memory buffer size in bytes to hold the requested blocks. Returns error if unable to retrieve buffer size.
TInt CJpegDecoder::HandleStreamGetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	CJpgReadCodec* jpgReadCodec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());
	TInt bufferSize = KErrNone;
	TRAPD(errCode, bufferSize = jpgReadCodec->GetStreamBufferSizeL(aFormat, aBlockSizeInPixels, aNumBlocks));
	if (errCode!=KErrNone)
		{
		bufferSize = errCode;
		}

	return bufferSize;
	}

void CJpegDecoder::HandleStreamInitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	//Validate source image dimensions. Make sure that image is multiple of MCUs
	CJpgWriteCodec::ValidateBlockSizeL(iJpgFrameInfo.iSizeInPixels, TSize(iJpgFrameInfo.MCUWidthInPixels(), iJpgFrameInfo.MCUHeightInPixels()));
	
	if(iExtensionManager)
		{
		if(iExtensionManager->ClippingRectExtensionRequested() || iExtensionManager->ScalerExtensionRequested() || iExtensionManager->OperationExtensionRequested())
			{
			User::Leave(KErrNotSupported);
			}
		}

	CImageDecoderPlugin::RequestInitL(aFrameNumber);

	CJpgReadCodec* jpgReadCodec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());
	jpgReadCodec->InitFrameL(aFormat, aNavigation);
	}

void CJpegDecoder::HandleStreamGetBlocks(CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	CJpgReadCodec* jpgReadCodec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());

	TRAPD(errCode, jpgReadCodec->GetBlocksL(aFrame, aSeqPosition, aNumBlocksToGet, aNumBlocksRead));
	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}
	}

void CJpegDecoder::HandleStreamGetNextBlocks(CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	CJpgReadCodec* jpgReadCodec = reinterpret_cast<CJpgReadCodec*>(ImageReadCodec());
	
	TRAPD(errCode, jpgReadCodec->GetNextBlocksL(aFrame, aNumBlocksToGet, aNumBlocksRead, aHaveMoreBlocks));
	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}
	}

// System Wide Define
#ifdef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
void CJpegDecoder::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	
	switch (aExtUid.iUid)
		{
		case KUidImageConvExtStreamedDecodeValue:
			{
			if (iJpgFrameInfo.iProgressive)
				{
				User::Leave(KErrNotSupported); //codec dosen't support progressive decoding
				}
			
			if (iJpgFrameInfo.iMultiScan)
				{
				JPEG_LEAVE(KErrNotSupported, "Multiscan in GetExtensionL");
				}
			
			if (!iStreamedDecodeExt)
				{
				iStreamedDecodeExt = CStreamedDecodeExtension::NewL(this);
				}
			
			aExtPtr = iStreamedDecodeExt;
			break;
			}
			
		default:
			{
			ASSERT(iExtensionManager);
			iExtensionManager->GetExtensionL(aExtUid, aExtPtr);
			}
		}
	}
#else
void CJpegDecoder::GetExtensionL(TUid /*aExtUid*/, MImageConvExtension*& /*aExtPtr*/)
	{
	User::Leave(KErrNotSupported);
	}
#endif

