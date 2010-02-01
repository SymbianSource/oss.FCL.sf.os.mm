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
#include "ImageClientMain.h"
#include <101F45CA_extra.rsg>
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include "MBMCodec.h"
#include "ImageUtils.h"

// Mbm decoder.
CMbmDecoder* CMbmDecoder::NewL()
	{
	return new(ELeave) CMbmDecoder;
	}

CMbmDecoder::CMbmDecoder()
	{}

CMbmDecoder::~CMbmDecoder()
	{
	Cleanup();
	}

void CMbmDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < NumberOfFrames()), Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeMBMUid;
	aImageSubType = KNullUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CMbmDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec() == NULL);

	CMbmReadCodec* imageReadCodec = CMbmReadCodec::NewL(iRootStreamOffset);

	SetImageReadCodec(imageReadCodec);
	
	ReadFrameHeadersL();
	}

const TInt KMbmSignatureSize = 20;
const TInt KMbmSigBytes = 16; // no of bytes used in comparisons
LOCAL_D const TUint8 KMbmSignature[KMbmSignatureSize] = {0x37, 0x00, 0x00, 0x10, 0x42, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x39, 0x64, 0x39, 0x47};
void CMbmDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KMbmSignatureSize);

	// Validate the header.
	if (bufferDes.Length() < KMbmSignatureSize)
		User::Leave(KErrUnderflow);
	
	const TUint8* ptr = bufferDes.Ptr();
	if (Mem::Compare(ptr, KMbmSigBytes, KMbmSignature, KMbmSigBytes)!=0)
		User::Leave(KErrCorrupt);

	// Store the RootStreamOffset
	iRootStreamOffset = PtrReadUtil::ReadUint32(ptr+4*sizeof(TUint32)) - KMbmFirstBitmapOffset;
		// the 4th 32-bit value is used to give us the effective root stream offset

	// Set start position.
	SetStartPosition(KMbmFirstBitmapOffset);

	// Get this from the header?
	SetDataLength(KMaxTInt);
	}

CFrameInfoStrings* CMbmDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	const TUid KMbmCodecDllUid = {KMBMCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs, KMbmCodecDllUid, resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	const CFrameImageData& frameData = FrameData(aFrameNumber); 
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();
	const TMbmDecodeData* compressionImageData = STATIC_CAST(const TMbmDecodeData*, frameData.GetFrameData(0));

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
	formatIndex += (frameInfo.iFlags & TFrameInfo::EAlphaChannel) ? 1 : 0;
	templte = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	info.Format(templte, depth);
	frameInfoStrings->SetDepthL(info);

	resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	formatIndex = (compressionImageData->iCompressed) ? 1 : 0;
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetDetailsL(info);
	
	CleanupStack::Pop(frameInfoStrings);
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

// Mbm encoder.
CMbmEncoder* CMbmEncoder::NewL()
	{
	return new(ELeave) CMbmEncoder;
	}

CMbmEncoder::CMbmEncoder()
	{
	}

CMbmEncoder::~CMbmEncoder()
	{
	Cleanup();
	}

void CMbmEncoder::PrepareEncoderL(const CFrameImageData* aFrameImageData)
	{
	iWriteDisplayMode = ENone;

	TInt index;
	TInt count = (aFrameImageData) ? aFrameImageData->FrameDataCount() : 0;

	if (count == 0)
		iWriteDisplayMode = Source().DisplayMode();	// default value

	for (index = 0 ; index<count ; index++)
		{	
		const TFrameDataBlock* encoderData = aFrameImageData->GetFrameData(index);
		if (encoderData->DataType() == KMBMEncodeDataUid)
			{
			if (iWriteDisplayMode != ENone)
				User::Leave(KErrCorrupt);

			const TMbmEncodeData* mbmEncodeData = STATIC_CAST(const TMbmEncodeData*, encoderData);
			iWriteDisplayMode = mbmEncodeData->iDisplayMode;
			}
		else
			User::Leave(KErrCorrupt);
		}

	// Create codec.
	StartPosition() = KMbmFirstBitmapOffset;

	CMbmWriteCodec* imageWriteCodec = CMbmWriteCodec::NewL(iWriteDisplayMode);
	
	SetImageWriteCodec(imageWriteCodec);

	}

const TUint8 KMbmBitmapFileHeader[] = {0x37, 0, 0, 0x10, 0x42, 0, 0, 0x10, 0, 0, 0, 0, 0x39, 0x64, 0x39, 0x47};
void CMbmEncoder::UpdateHeaderL()
	{
	TBuf8<sizeof(KMbmBitmapFileHeader) + sizeof(TUint32)> mbmHeader;
	TPtrC8 mbmHeaderPtr(KMbmBitmapFileHeader, sizeof(KMbmBitmapFileHeader));
	mbmHeader.Append(mbmHeaderPtr);

	TUint32 mbmRootStreamOffset = (Position() + KMbmFirstBitmapOffset) - 8; // Set root stream offset.
	TPtrC8 mbmRootStreamOffsetPtr(REINTERPRET_CAST(TUint8*, &mbmRootStreamOffset), sizeof(TUint32));
	mbmHeader.Append(mbmRootStreamOffsetPtr);

	WriteDataL(0,mbmHeader);
	}


