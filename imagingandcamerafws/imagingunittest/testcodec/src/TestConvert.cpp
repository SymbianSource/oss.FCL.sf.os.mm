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
#include <101F7BF5_extra.rsg>
#include "TestCodec.h"
#include "TestPanic.h"
#include "TestDecs.h"
#include "TestUids.hrh"

_LIT(KTestPanicCategory, "TestConvertPlugin");


// Global panic function
GLDEF_C void Panic(TInt aError)
	{
	User::Panic(KTestPanicCategory, aError);
	}


// decoder.
CTestDecoder* CTestDecoder::NewL(TBool aLeaveCorrupt)
	{
	return new(ELeave) CTestDecoder(aLeaveCorrupt);
	}

CTestDecoder::CTestDecoder(TBool aLeaveCorrupt):
	iLeaveCorrupt(aLeaveCorrupt)
	{}

CTestDecoder::~CTestDecoder()
	{
	CImageDecoderPlugin::Cleanup();
	}

void CTestDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeTestUid;
	aImageSubType = KImageSubTypeTestUid;
	}

void CTestDecoder::ScanDataL()
	{
	if(iLeaveCorrupt)
		User::Leave(KErrCorrupt);

	SetDataLength(10);
	TFrameInfo newFrameInfo;
	SetImageInfo(newFrameInfo);

	CTestReadCodec* imageReadCodec = new (ELeave) CTestReadCodec(*this);
	SetImageReadCodec(imageReadCodec); 
	imageReadCodec->ConstructL();

	ReadFrameHeadersL();

	}

CFrameInfoStrings* CTestDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	if (aFrameNumber!=0)
		User::Leave(KErrArgument);

	const TUid testCodecDllUid = {KTestDecoderDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,testCodecDllUid,resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<128> info;
	TBuf<128> templte;

	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetFormatL(info);

	templte = resourceReader.ReadTPtrC();
	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	const TSize& size = frameInfo.iOverallSizeInPixels;
	info.Format(templte, size.iWidth, size.iHeight);
	frameInfoStrings->SetDimensionsL(info);

	templte = resourceReader.ReadTPtrC();
	iMaxValue = 10;
	info.Format(templte, iMaxValue);
	frameInfoStrings->SetDepthL(info);

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

void CTestDecoder::DoConvert()
	{
	while(!ShouldAbort())
		/* Wait for Cancel() to be called */;
	RequestComplete(KErrDied);
	}

TInt CTestDecoder::NumberOfImageComments() const
	{
	return 1;
	}

HBufC* CTestDecoder::ImageCommentL(TInt /*aCommentNumber*/) const
	{
	HBufC* imageComment = HBufC::NewL(20);
	TPtr ptr(imageComment->Des());
	ptr.Copy(_L("Test Image Comment 1"));
	return imageComment;
	}

TInt CTestDecoder::NumberOfFrameComments(TInt /*aFrameNumber*/) const
	{
	return 1;
	}

HBufC* CTestDecoder::FrameCommentL(TInt /*aFrameNumber*/, TInt /*aCommentNumber*/) const
	{
	HBufC* frameComment = HBufC::NewL(20);
	TPtr ptr(frameComment->Des());
	ptr.Copy(_L("Test Frame Comment 1"));
	return frameComment;
	}

// decoder.
CTestEncoder* CTestEncoder::NewL()
	{
	CTestEncoder* self;
	self = new (ELeave) CTestEncoder;
	return self;
	}

CTestEncoder::CTestEncoder()
	{
	}

CTestEncoder::~CTestEncoder()
	{
	}

void CTestEncoder::DoConvert()
	{
	while(!ShouldAbort())
		/* Wait for Cancel() to be called */;
	RequestComplete(KErrDied);
	}

void CTestEncoder::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	CImageWriteCodec* imageWriteCodec = new (ELeave) CTestWriteCodec;
	SetImageWriteCodec(imageWriteCodec);
	}

void CTestEncoder::UpdateHeaderL()
	{
	}


CTestExtendedDecoder* CTestExtendedDecoder::NewL()
	{
	CTestExtendedDecoder* self;
	self = new (ELeave) CTestExtendedDecoder;
	return self;
	}

CTestExtendedDecoder::~CTestExtendedDecoder()
	{
	
	}
	
CTestExtendedDecoder::CTestExtendedDecoder()
	{
	
	}
	
void CTestExtendedDecoder::ScanDataL()
	{
//	if(iLeaveCorrupt)
//		User::Leave(KErrCorrupt);

	SetDataLength(10);
	TFrameInfo newFrameInfo;
	SetImageInfo(newFrameInfo);

	CTestReadCodec* imageReadCodec = new (ELeave) CTestReadCodec(*this);
	SetImageReadCodec(imageReadCodec); 
	imageReadCodec->ConstructL();

	ReadFrameHeadersL();
	}


CTestExtendedEncoder* CTestExtendedEncoder::NewL()
	{
	CTestExtendedEncoder* self;
	self = new (ELeave) CTestExtendedEncoder;
	return self;
	}

CTestExtendedEncoder::~CTestExtendedEncoder()
	{
	
	}

CTestExtendedEncoder::CTestExtendedEncoder()
	{
	
	}

