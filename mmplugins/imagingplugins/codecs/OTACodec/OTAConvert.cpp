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
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include <101F45D0_extra.rsg>
#include "OTACodec.h"

_LIT(KOTAPanicCategory, "OTAConvertPlugin");


// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KOTAPanicCategory, aError);
	}


// Ota decoder.
COtaDecoder* COtaDecoder::NewL()
	{
	return new(ELeave) COtaDecoder;
	}

COtaDecoder::COtaDecoder()
	{}

COtaDecoder::~COtaDecoder()
	{
	Cleanup();
	}

void COtaDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeOTAUid;
	aImageSubType = KNullUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void COtaDecoder::ScanDataL()
	{
	ReadFormatL();
	
	ASSERT(ImageReadCodec() == NULL);

	COtaReadCodec* imageReadCodec = COtaReadCodec::NewL(iSize);

	SetImageReadCodec(imageReadCodec);
	
	ReadFrameHeadersL();
	}

const TInt KOtaFileHeaderSize = 22; // InfoField + 16 extFields + width + height + depth.
const TInt KOtaUnsupportedFormats = 0x6F;
const TInt KOtaConcatenationFlag = 0x80;
const TInt KOtaSixteenBitSizeFlag = 0x10;
void COtaDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KOtaFileHeaderSize);

	// Validate the header.
	if (bufferDes.Length() < KOtaFileHeaderSize)
		User::Leave(KErrUnderflow);

	const TUint8* ptr = &bufferDes[0];
	const TUint8* startPtr = ptr;

	TUint8 infoField = *ptr++;
	if (infoField & KOtaUnsupportedFormats)
		User::Leave(KErrNotSupported); // Compression, palette and animations not supported

	TUint8 extField = infoField;
	while ((extField & KOtaConcatenationFlag) && ((ptr-startPtr) < 17)) // Skip extField concatenations
		extField = *ptr++;

	if (extField & KOtaConcatenationFlag)
		User::Leave(KErrCorrupt); // There should be an infoField and no more than 16 extFields.

	if (infoField & KOtaSixteenBitSizeFlag) // Size values are 16 bit
		{
		iSize.iWidth = (ptr[1] << 8) | ptr[0];
		iSize.iHeight = (ptr[3] << 8) | ptr[2];
		ptr += 4;
		}
	else // Size values are 8 bit
		{
		iSize.iWidth = *ptr++;
		iSize.iHeight = *ptr++;
		}

	TUint8 depth = *ptr++;
	if (depth != 1)
		User::Leave(KErrNotSupported);

	TInt startPosition = ptr - startPtr;
	SetStartPosition(startPosition);
	SetDataLength(startPosition + (((iSize.iWidth * iSize.iHeight) + 7) / 8));
	
	TFrameInfo imageInfo;
	imageInfo = ImageInfo();
	imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), iSize);
	imageInfo.iOverallSizeInPixels = iSize;
	imageInfo.iFrameSizeInTwips = TSize(0, 0);
	imageInfo.iBitsPerPixel = 1;
	imageInfo.iDelay = 0;
	imageInfo.iFlags = 0;
	imageInfo.iFrameDisplayMode = EGray2;
	SetImageInfo(imageInfo);
	}

CFrameInfoStrings* COtaDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{

	const TUid KOtaCodecDllUid = {KOTACodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KOtaCodecDllUid,resourceFile);

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

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	info = resourceReader.ReadTPtrC(); // note depth is fixed
	frameInfoStrings->SetDepthL(info);

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}


