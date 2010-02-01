// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestAsyncCodec.h"
#include "TestAsyncConvert.h"

#include "TestPanic.h"
#include "TestDecs.h"
#include "TestUids.hrh"

//_LIT(KTestPanicCategory, "TestAsyncConvertPlugin");	// EABI warning removal

const TInt KDummyFileSize = 100;
const TInt KDummyHeaderSize = 10;
const TInt KDummyDataSize = KDummyFileSize - KDummyHeaderSize;


//********************************************
// CTestAsyncDecoder.
//********************************************
CTestAsyncDecoder* CTestAsyncDecoder::NewL()
	{
	CTestAsyncDecoder* self = new(ELeave) CTestAsyncDecoder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestAsyncDecoder::CTestAsyncDecoder()
	{
	}

void CTestAsyncDecoder::ConstructL()
	{
	}

CTestAsyncDecoder::~CTestAsyncDecoder()
	{
	delete iScheduler;
	
	CImageDecoderPlugin::Cleanup();
	}

void CTestAsyncDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeTestUid;
	aImageSubType = KImageSubTypeTestUid;
	}

void CTestAsyncDecoder::ScanDataL()
	{

	SetDataLength(10);

	
	if (!iReadHeaders)
		{
		TPtrC8 bufferDes;
		ReadDataL(0, bufferDes, (KDummyHeaderSize));
		if (bufferDes.Length() < KDummyHeaderSize)
			User::Leave(KErrUnderflow);

		iReadHeaders = ETrue;
		SetDataLength(KDummyDataSize);


		iDecodeIterations = 0;

		// create the codec
		delete iImageReadCodec;
		iImageReadCodec = NULL;
		iImageReadCodec = CTestAsyncReadCodec::NewL(*this);
		SetImageReadCodec(iImageReadCodec);
	
		// create the async decoder
		delete iScheduler;
		iScheduler = NULL;
		iScheduler = new (ELeave) CAsyncScheduler(*this);

		// create a dummy TFrameInfo
		TFrameInfo imageInfo;
		imageInfo = ImageInfo();
		imageInfo.iFrameSizeInTwips.iWidth = 0;
		imageInfo.iFrameSizeInTwips.iHeight = 0;
		imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), TPoint(256, 256));
		imageInfo.iOverallSizeInPixels = TSize(256, 256);
		imageInfo.iBitsPerPixel = 24;
		imageInfo.iDelay = 0;
		imageInfo.iFlags = TFrameInfo::ECanDither;
		imageInfo.iFrameDisplayMode = EColor16M;
		SetImageInfo(imageInfo);

		SetStartPosition(KDummyHeaderSize);
		
		ReadFrameHeadersL();
		}

	}

CFrameInfoStrings* CTestAsyncDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
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

void CTestAsyncDecoder::DoConvert()
	{
	const TInt KDecodeIterations = 10;

	TRAPD(errCode, PrepareForProcessFrameL());

	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}

	if (iError != KErrNone)
		{
		RequestComplete(iError);
		}
	// need to get more data from client app ?
	else if (SourceData().Length() < KDummyDataSize)
		{
		RequestComplete(KErrUnderflow);
		}
	else if (iDecodeIterations++ > KDecodeIterations)
		{
		RequestComplete(iError);
		}
	else 
		{
		iDecodeState = EDecodeInit;
		iScheduler->Start();
		SetSelfPending();
		}
	}

TInt CTestAsyncDecoder::NumberOfImageComments() const
	{
	return 1;
	}

HBufC* CTestAsyncDecoder::ImageCommentL(TInt /*aCommentNumber*/) const
	{
	HBufC* imageComment = HBufC::NewL(20);
	TPtr ptr(imageComment->Des());
	ptr.Copy(_L("Test Image Comment 1"));
	return imageComment;
	}

TInt CTestAsyncDecoder::NumberOfFrameComments(TInt /*aFrameNumber*/) const
	{
	return 1;
	}

HBufC* CTestAsyncDecoder::FrameCommentL(TInt /*aFrameNumber*/, TInt /*aCommentNumber*/) const
	{
	HBufC* frameComment = HBufC::NewL(20);
	TPtr ptr(frameComment->Des());
	ptr.Copy(_L("Test Frame Comment 1"));
	return frameComment;
	}

void CTestAsyncDecoder::ProcessFrameComplete(TInt aError)
	{
	iError = aError;
	iDecodeState = EDecodeFinished;
	iScheduler->Start();
	}

void CTestAsyncDecoder::MasoProcessL(TInt aError)
	{
	iError = aError;

	if (aError != KErrNone)
		{
		SelfComplete(iError);
		return;
		}

	switch(iDecodeState)
		{
		case EDecodeInit:
			iDecodeState = EDecodeInProgress;
			iScheduler->Start();
			break;

		case EDecodeInProgress:
			TRAP(iError, iImageReadCodec->ProcessFrameL(SourceData()));
			if (iError != KErrNone)
				{
				SelfComplete(iError);
				break;
				}
			// now wait for async callback to ProcessFrameComplete...
			break;

		case EDecodeFinished:
			SelfComplete(iError);
			break;
		}

	}

void CTestAsyncDecoder::Cleanup()
	{
	// cancel the state machine
	if (iScheduler)
		iScheduler->Cancel();
	
	CImageDecoderPlugin::Cleanup();
	}


//********************************************
// CTestAsyncEncoder.
//********************************************
CTestAsyncEncoder* CTestAsyncEncoder::NewL()
	{
	CTestAsyncEncoder* self = new(ELeave) CTestAsyncEncoder();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestAsyncEncoder::CTestAsyncEncoder()
	{
	}

void CTestAsyncEncoder::ConstructL()
	{
	}

CTestAsyncEncoder::~CTestAsyncEncoder()
	{
	delete iScheduler;
	
	CImageEncoderPlugin::Cleanup();
	}

void CTestAsyncEncoder::DoConvert()
	{
	const TInt KEncodeIterations = 10;

/*
	TRAPD(errCode, PrepareForProcessFrameL());

	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}
*/
	if (iError != KErrNone)
		{
		RequestComplete(iError);
		}
	
	if (iEncodeIterations++ > KEncodeIterations)
		{
		RequestComplete(iError);
		}
	else 
		{
		iEncodeState = EEncodeInit;
		iScheduler->Start();
		SetSelfPending();
		}
	}

void CTestAsyncEncoder::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	// create the codec
	delete iImageWriteCodec;
	iImageWriteCodec = NULL;
	iImageWriteCodec = CTestAsyncWriteCodec::NewL(*this);
	SetImageWriteCodec(iImageWriteCodec);

	// create the scheduler
	delete iScheduler;
	iScheduler = NULL;
	iScheduler = new (ELeave) CAsyncScheduler(*this);

	}

void CTestAsyncEncoder::UpdateHeaderL()
	{
	}

void CTestAsyncEncoder::ProcessFrameComplete(TInt aError)
	{
	iError = aError;
	iEncodeState = EEncodeFinished;
	iScheduler->Start();
	}

void CTestAsyncEncoder::MasoProcessL(TInt aError)
	{
	iError = aError;

	if (aError != KErrNone)
		{
		SelfComplete(iError);
		return;
		}

	switch(iEncodeState)
		{
		case EEncodeInit:
			iEncodeState = EEncodeInProgress;
			iScheduler->Start();
			break;

		case EEncodeInProgress:
			TRAP(iError, iImageWriteCodec->ProcessFrameL(DestinationData()));
			if (iError != KErrNone)
				{
				SelfComplete(iError);
				break;
				}
			// now wait for async callback to ProcessFrameComplete...
			break;

		case EEncodeFinished:
			SelfComplete(iError);
			break;
		}

	}

void CTestAsyncEncoder::Cleanup()
	{
	// cancel the state machine
	if (iScheduler)
		iScheduler->Cancel();
	
	CImageEncoderPlugin::Cleanup();
	}


// CAsyncScheduler
CAsyncScheduler::CAsyncScheduler(MAsyncSchedulerObserver& aSchedulerObserver) : 
	CActive(CActive::EPriorityIdle), iSchedulerObserver(aSchedulerObserver)
	{
	CActiveScheduler::Add(this); 
	}


void CAsyncScheduler::Start()
	{ 
	SetActive();
	TRequestStatus* status = &iStatus; 
	User::RequestComplete(status, KErrNone);
	}

void CAsyncScheduler::RunL()
	{
	TInt error = iStatus.Int();

	iSchedulerObserver.MasoProcessL(error);
	}


