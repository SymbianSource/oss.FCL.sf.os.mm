// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <ecom/ecom.h>
#include "ICLFrameWorkTestStep.h"
#include "ICLFrameworkTestSuite.h"
#include "BufferedImageDecoderTest.h"
#include "icl/ICL_UIDS.hrh"

_LIT(KTiffFile,"TIF2.TIF");
_LIT(KTiffFileRef,"TIF2.MBM");
_LIT(KGifFile,"TRANS_1.GIF");
_LIT(KGifFileRef,"TRANS_1.MBM");
_LIT(KTestFile,"TEST.TST");

_LIT(KTifMimeType,"image/tiff");

_LIT(KImageComment,"Test Image Comment 1");
_LIT(KFrameComment,"Test Frame Comment 1");
_LIT(KDecoderString,"Decoder:");
_LIT(KFormatString,"Format:");
_LIT(KDimensionString,"Dimensions:");
_LIT(KDepthString,"Colour range:");

const TUid KTestDecoderImpUid = {0x101F7BF6};


//RCBufferedImageDecoderDecodeTest
RCBufferedImageDecoderDecodeTest* RCBufferedImageDecoderDecodeTest::NewL(CTestSuite* aSuite, TBool aMaskDecode, TBool aAllocTest)
	{
	return new (ELeave) RCBufferedImageDecoderDecodeTest(aSuite, aMaskDecode, aAllocTest);
	}

RCBufferedImageDecoderDecodeTest::RCBufferedImageDecoderDecodeTest(CTestSuite* aSuite, TBool aMaskDecode, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iMaskDecode(aMaskDecode),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	// Increase the heap size for this test
	iHeapSize = 0x20000;
	iMimeTest = EFalse;
	if(!iMaskDecode && !iAllocTest)
		{
		iMimeTest = ETrue;
		iTestStepName = _L("MM-ICL-FRM-U-5500-HP");
		}
	if(!iMaskDecode && iAllocTest)		
		{
		iMimeTest = ETrue;
		iTestStepName = _L("MM-ICL-FRM-U-5501-HP");
		}
	if(iMaskDecode && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5502-HP");
	if(iMaskDecode && iAllocTest)		
		iTestStepName = _L("MM-ICL-FRM-U-5503-HP");
	}

void RCBufferedImageDecoderDecodeTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	if(!iMaskDecode && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CBufferedImageDecoder Decode Test   >>>"));
	if(!iMaskDecode && iAllocTest)		
		INFO_PRINTF1(_L("<<<   CBufferedImageDecoder Decode Alloc Test   >>>"));
	if(iMaskDecode && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CBufferedImageDecoder Mask Decode Test   >>>"));
	if(iMaskDecode && iAllocTest)		
		INFO_PRINTF1(_L("<<<   CBufferedImageDecoder Mask Decode Alloc Test   >>>"));

	if(iMimeTest)
		INFO_PRINTF1(_L("Using MIME type"));

	RFile file;
	TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
    if(iMaskDecode)
    	{
    	fileName.Append(KGifFile);
 		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
   		}
	else
		{
    	fileName.Append(KTiffFile);
		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
		}
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);
	iSourceLength = iSourceData->Length();

	if(iMaskDecode)
		{
		TFileName refFileName(KGifFileRef);
		GetTestFileNameL(fileName, refFileName);
		}
	else
		{
		TFileName refFileName(KTiffFileRef);
		GetTestFileNameL(fileName, refFileName);
		}

	iRefBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iRefBitmap->Load(fileName));

	if(iMimeTest)
		iMimeType.Copy(KTifMimeType);

	iTestBitmap = NULL;
	iTestBitmapMask = NULL;
	iImageDecoder = NULL;
	iImplementationUid.iUid = 0;

	iListener = new (ELeave) CActiveListener;
	}

void RCBufferedImageDecoderDecodeTest::TestStepL()
	{
	ResetPointer();

	if(iImageDecoder == NULL)
		iImageDecoder = CBufferedImageDecoder::NewL(iFileSession);

	if(iMimeTest)
		iImageDecoder->OpenL(iSourceDataPtr, iMimeType, CImageDecoder::EOptionAllowZeroFrameOpen);
	else
		iImageDecoder->OpenL(iSourceDataPtr, CImageDecoder::EOptionAllowZeroFrameOpen);

	while(!iImageDecoder->ValidDecoder())
		{
		IncrementPointer();
		iImageDecoder->AppendDataL(iSourceDataPtr);
		iImageDecoder->ContinueOpenL();
		}

	while(!iImageDecoder->IsImageHeaderProcessingComplete())
		{
		IncrementPointer();
		iImageDecoder->AppendDataL(iSourceDataPtr);
		iImageDecoder->ContinueProcessingHeaderL();
		}

	const TFrameInfo frameInfo = iImageDecoder->FrameInfo(0);

	if(iMaskDecode)
		{
		if(!(frameInfo.iFlags & TFrameInfo::ETransparencyPossible))
			{
			SetFail(_L("Image does not support masks"));
			iMaskDecode = EFalse;
			}
		}
	
	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iTestBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	if(iMaskDecode)
		{
		iTestBitmapMask = new (ELeave) CFbsBitmap;
		User::LeaveIfError(iTestBitmapMask->Create(frameInfo.iOverallSizeInPixels, EGray2));
		}

	iListener->InitialiseActiveListener();
	if(iMaskDecode)
		iImageDecoder->Convert(&iListener->iStatus, *iTestBitmap, *iTestBitmapMask, 0);
	else
		iImageDecoder->Convert(&iListener->iStatus, *iTestBitmap, 0);
	CActiveScheduler::Start();

	TInt status;
	status = iListener->iStatus.Int();
	if((status != KErrNone) && (status != KErrUnderflow))
		User::Leave(status);

	while(status == KErrUnderflow)
		{
		IncrementPointer();
		iImageDecoder->AppendDataL(iSourceDataPtr);
		iListener->InitialiseActiveListener();
		iImageDecoder->ContinueConvert(&iListener->iStatus);
		CActiveScheduler::Start();
		status = iListener->iStatus.Int();
		if((status != KErrNone) && (status != KErrUnderflow))
			User::Leave(status);
		}
	}

void RCBufferedImageDecoderDecodeTest::CheckResultL()
	{
	Compare(*iTestBitmap, *iRefBitmap, _L("Bitmaps are not the same"));
	//The image and mask look the same
	if(iMaskDecode)
		Compare(*iTestBitmapMask, *iRefBitmap, _L("Mask bitmaps are not the same"));
	}

void RCBufferedImageDecoderDecodeTest::Cleanup()
	{
	delete iImageDecoder; iImageDecoder = NULL;
	if(iTestBitmap)
		{delete iTestBitmap; iTestBitmap = NULL;}
	if(iTestBitmapMask)
		{delete iTestBitmapMask; iTestBitmapMask = NULL;}
	}

void RCBufferedImageDecoderDecodeTest::Close()
	{
	delete iRefBitmap; iRefBitmap = NULL;
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

void RCBufferedImageDecoderDecodeTest::ResetPointer()
	{
	iPosition = 0;
	iSourceDataPtr.Set(&iSourceData->Des()[iPosition],1,1);
	}

void RCBufferedImageDecoderDecodeTest::IncrementPointer()
	{
	iPosition += 1;
	iSourceDataPtr.Set(&iSourceData->Des()[iPosition],1,1);
	}

//RCBufferedImageDecoderFunctionTest
RCBufferedImageDecoderFunctionTest* RCBufferedImageDecoderFunctionTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new(ELeave) RCBufferedImageDecoderFunctionTest(aSuite, aAllocTest);
	}

RCBufferedImageDecoderFunctionTest::RCBufferedImageDecoderFunctionTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5510-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5511-HP");

	}

void RCBufferedImageDecoderFunctionTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	if(!iAllocTest)
		INFO_PRINTF1(_L("<<<   CBufferedImageDecoder Function Test   >>>"));
	else
		INFO_PRINTF1(_L("<<<   CBufferedImageDecoder Function Alloc Test   >>>"));

	RFile file;
	TFileName fileName;
	GetTestFileNameL(fileName, KTestFile());
	INFO_PRINTF2(_L("%S"),&fileName);

	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);

	iImageDecoder = CBufferedImageDecoder::NewL(iFileSession);
	iImageDecoder->OpenL(iSourceDataPtr, CImageDecoder::EOptionAllowZeroFrameOpen);

	iImageComment = NULL;
	iFrameComment = NULL;
	iFrameInfoStrings = NULL;
	iImplementationUid.iUid = 0;

	}

void RCBufferedImageDecoderFunctionTest::TestStepL()
	{
	iImageCommentCount = iImageDecoder->NumberOfImageComments();
	iImageComment = iImageDecoder->ImageCommentL(0);
	iFrameCommentCount = iImageDecoder->NumberOfFrameComments(0);
	iFrameComment = iImageDecoder->FrameCommentL(0,0);

	iFrameInfoStrings = iImageDecoder->FrameInfoStringsL();
	iDecoder = iFrameInfoStrings->Decoder();
	iFormat  = iFrameInfoStrings->Format();
	iDimensions = iFrameInfoStrings->Dimensions();
	iDepth = iFrameInfoStrings->Depth();

	iImplementationUid = iImageDecoder->ImplementationUid();
	}

void RCBufferedImageDecoderFunctionTest::CheckResultL()
	{

	Compare(iImageCommentCount, 1, _L("Number of image comments wrong"));
	TPtr ptr(iImageComment->Des());
	Compare(ptr, KImageComment, _L("Image comment wrong"));
	ptr.Set(iFrameComment->Des());
	Compare(ptr, KFrameComment, _L("Frame comment wrong"));

	iDecoder.SetLength(KDecoderString().Length());
	Compare(iDecoder, KDecoderString, _L("Decoder string incorrect"));
	iFormat.SetLength(KFormatString().Length());
	Compare(iFormat, KFormatString, _L("Format string incorrect"));
	iDimensions.SetLength(KDimensionString().Length());
	Compare(iDimensions, KDimensionString, _L("Dimension string incorrect"));
	iDepth.SetLength(KDepthString().Length());
	Compare(iDepth, KDepthString, _L("Depth string incorrect"));

	Compare(iImplementationUid, KTestDecoderImpUid, _L("Implementation Uid incorrect"));
	}

void RCBufferedImageDecoderFunctionTest::Cleanup()
	{
	iImplementationUid.iUid = 0;
	delete iImageComment; iImageComment = NULL;
	delete iFrameComment; iFrameComment = NULL;
	delete iFrameInfoStrings; iFrameInfoStrings = NULL;
	}

void RCBufferedImageDecoderFunctionTest::Close()
	{
	delete iImageDecoder; iImageDecoder = NULL;
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

