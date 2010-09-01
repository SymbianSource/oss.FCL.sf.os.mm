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
#include <101F45D3_extra.rsg>
#include "ICOCodec.h"
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif

const TInt KIcoFileHeaderSize = 6;
const TInt KIcoImageHeaderSize = 16;


_LIT(KICOPanicCategory, "ICOConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KICOPanicCategory, aError);
	}

// Ico decoder.
CIcoDecoder* CIcoDecoder::NewL()
	{
	return new(ELeave) CIcoDecoder;
	}

CIcoDecoder::CIcoDecoder()
	{}

CIcoDecoder::~CIcoDecoder()
	{
	Cleanup();
	}

void CIcoDecoder::Cleanup()
	{
	// Delete any objects we should get rid of

	// Base class included
	CImageDecoderPlugin::Cleanup();
	}

void CIcoDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < NumberOfFrames()), Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeICOUid;
	aImageSubType = KNullUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CIcoDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec() == NULL);

	CIcoReadCodec* imageReadCodec;
	imageReadCodec = CIcoReadCodec::NewL(iIconCount);
	
	SetImageReadCodec(imageReadCodec);
	


	ReadFrameHeadersL();
	}

void CIcoDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KIcoFileHeaderSize);

	// Validate the header.
	if (bufferDes.Length() < KIcoFileHeaderSize)
		User::Leave(KErrUnderflow);

	const TUint8* ptr = &bufferDes[0];
	iIconCount = ptr[4] | (ptr[5] << 8);
	if (iIconCount <= 0)
		User::Leave(KErrNotFound);

	SetStartPosition(KIcoFileHeaderSize + (KIcoImageHeaderSize * iIconCount));
	SetDataLength(KMaxTInt);
	}

CFrameInfoStrings* CIcoDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{

	const TUid KIcoCodecDllUid = {KICOCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KIcoCodecDllUid,resourceFile);

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

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}


