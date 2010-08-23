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
#include "ImageEncoderPrivTest.h"
#include "ICLFrameworkTestSuite.h"
#include "ImageResolverAPI.h"
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
_LIT(KGifFile,"GIF87a.GIF");
_LIT(KGifFileRef,"GIF87a.MBM");

const TInt KRAEncoderPrivTestPosition = 3608;
const TInt KRAEncoderPrivTestLength	= 1896;
const TInt KRAEncoderPrivTestSize = 4096;

//CImageEncoderPrivTestAccess
CImageEncoderPrivTestAccess::CImageEncoderPrivTestAccess(CImageEncoderPriv* aEncPriv)
: iImageEncoderPriv(aEncPriv)
	{
	// do nothing
	}

void CImageEncoderPrivTestAccess::SetImageEncoderPriv(CImageEncoderPriv* aEncPriv)
	{
	ASSERT(iImageEncoderPriv != NULL);

	iImageEncoderPriv = aEncPriv;
	}

HBufC8& CImageEncoderPrivTestAccess::FinalDestinationBuffer()
	{
	return *(iImageEncoderPriv->iFinalDstBuffer);
	}


//RCImageEncoderPrivEncodeTest
RCImageEncoderPrivEncodeTest* RCImageEncoderPrivEncodeTest::NewL(CTestSuite* aSuite, TBool aFileTest, TBool aAllocTest)
	{
	return new (ELeave) RCImageEncoderPrivEncodeTest(aSuite, aFileTest, aAllocTest);
	}

RCImageEncoderPrivEncodeTest::RCImageEncoderPrivEncodeTest(CTestSuite* aSuite, TBool aFileTest, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iFileTest(aFileTest)
	{
	iSuite = aSuite;
	if(iFileTest)
		{
		//Use SetFileL()
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5200-HP");
		else
			iTestStepName = _L("MM-ICL-FRM-U-5201-HP");
		}
	else
		{
		//Use SetDataL()
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5202-HP");
		else
			iTestStepName = _L("MM-ICL-FRM-U-5203-HP");
		}
	}

void RCImageEncoderPrivEncodeTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iConstruct = NULL;

	if(iFileTest)
		{
		//Use SetFileL()
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageEncoderPriv SetFileL() Encode Test   >>>"));
		else
			INFO_PRINTF1(_L("<<<   CImageEncoderPriv SetFileL() Encode Alloc Test   >>>"));
		}
	else
		{
		//Use SetDataL()
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageEncoderPriv SetDataL() Encode Test   >>>"));
		else
			INFO_PRINTF1(_L("<<<   CImageEncoderPriv SetDataL() Encode Alloc Test   >>>"));
		}

	TFileName fileName;
	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(fileName, refFileName);
	iSourceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iSourceBitmap->Load(fileName));

	RFile file;
	fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(KGifFile);
	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);
	TInt size;
	User::LeaveIfError(file.Size(size));
	iRefImageData = HBufC8::NewL(size);
	TPtr8 refImageDataPtr(iRefImageData->Des());
	User::LeaveIfError(file.Read(refImageDataPtr));
	CleanupStack::PopAndDestroy(&file);
		
	iDestinationFilename = _L("C:\\TESTFILE.GIF");

	iImageData = NULL;
	iImageEncoderPriv = NULL;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageEncoderPrivEncodeTest::TestStepL()
	{
	TUid imageTypeUid;
	TUid implementationUid;
	imageTypeUid = KImageTypeGIFUid;
	implementationUid.iUid = KGIFEncoderImplementationUidValue;

	// Use RCImageConstructTest to create a CImageEncodeConstruct
	RCImageEncodeConstructTest* constructTest = RCImageEncodeConstructTest::NewL(iSuite, EFalse);
	CleanupStack::PushL(constructTest);
	iConstruct = constructTest->CreatePluginL(imageTypeUid, implementationUid);
	CleanupStack::PopAndDestroy(constructTest);
	iImageEncoderPriv = CImageEncoderPriv::NewL(iConstruct, this);

	iImageEncoderPriv->CreatePluginL();

	if(iFileTest)
		iImageEncoderPriv->SetFileL(iFileSession, iDestinationFilename);
	else
		iImageEncoderPriv->SetDataL(iImageData);

	iListener->InitialiseActiveListener();
	iImageEncoderPriv->Convert(iMyThread, &iListener->iStatus, *iSourceBitmap, NULL);
	CActiveScheduler::Start();

	TInt status;
	status = iListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);
	}

void RCImageEncoderPrivEncodeTest::CheckResultL()
	{
	if(iFileTest)
		{
		RFile file;
		User::LeaveIfError(file.Open(iFileSession, iDestinationFilename, EFileRead|EFileShareAny));
		CleanupClosePushL(file);
		TInt size;
		User::LeaveIfError(file.Size(size));
		iImageData = HBufC8::NewL(size);
		TPtr8 imageDataPtr(iImageData->Des());
		User::LeaveIfError(file.Read(imageDataPtr));
		CleanupStack::PopAndDestroy(&file);
		}

	Compare(iImageData->Des(), iRefImageData->Des(), _L("Encoded image differ from original"));
	}

void RCImageEncoderPrivEncodeTest::Cleanup()
	{
	delete iImageEncoderPriv; iImageEncoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	delete iImageData; iImageData = NULL;
	
	if(iFileTest)
		iFileSession.Delete(iDestinationFilename);
	}

void RCImageEncoderPrivEncodeTest::Close()
	{
	delete iScheduler; iScheduler = NULL;
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iRefImageData; iRefImageData = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

//RCImageEncoderPrivCustomTest
RCImageEncoderPrivCustomTest* RCImageEncoderPrivCustomTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageEncoderPrivCustomTest(aSuite, aAllocTest);
	}

RCImageEncoderPrivCustomTest::RCImageEncoderPrivCustomTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	ASSERT(!aAllocTest); // we're using this structure, but don't use it
	iTestStepName = _L("MM-ICL-FRM-U-5210-HP");
	}

void RCImageEncoderPrivCustomTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	INFO_PRINTF1(_L("<<<   CImageEncoderPriv Custom Test   >>>"));

	TFileName fileName;
	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(fileName, refFileName);
	iSourceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iSourceBitmap->Load(fileName));

	RFile file;
	fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(KGifFile);
	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);
	TInt size;
	User::LeaveIfError(file.Size(size));
	iRefImageData = HBufC8::NewL(size);
	TPtr8 refImageDataPtr(iRefImageData->Des());
	User::LeaveIfError(file.Read(refImageDataPtr));
	CleanupStack::PopAndDestroy(&file);
		
	iDestinationFilename = _L("C:\\TESTFILE.GIF");

	iImageData = NULL;
	iImageEncoderPriv = NULL;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageEncoderPrivCustomTest::TestStepL()
	{
	TUid imageTypeUid;
	TUid implementationUid;
	imageTypeUid = KImageTypeGIFUid;
	implementationUid.iUid = KGIFEncoderImplementationUidValue;

	// Use RCImageConstructTest to create a CImageEncodeConstruct
	RCImageEncodeConstructTest* constructTest = RCImageEncodeConstructTest::NewL(iSuite, EFalse);
	CleanupStack::PushL(constructTest);
	iConstruct = constructTest->CreatePluginL(imageTypeUid, implementationUid);
	CleanupStack::PopAndDestroy(constructTest);
	iImageEncoderPriv = CImageEncoderPriv::NewL(iConstruct, this);

	iImageEncoderPriv->CreatePluginL();

	iImageEncoderPriv->SetDataL(iImageData);

	iListener->InitialiseActiveListener();
	iImageEncoderPriv->Convert(iMyThread, &iListener->iStatus, *iSourceBitmap, NULL);
	CActiveScheduler::Start();

	TRAPD(error, iImageEncoderPriv->CustomSyncL(0));
	Compare(error, KErrNotSupported, _L("Wrong error value returned from sync call"));

	iListener->InitialiseActiveListener();
	iImageEncoderPriv->CustomAsync(iMyThread, &iListener->iStatus, 10);
	CActiveScheduler::Start();

	TInt status = iListener->iStatus.Int();
	Compare(status, KErrNotSupported, _L("Wrong error value returned from async call"));
	}

void RCImageEncoderPrivCustomTest::CheckResultL()
	{
	}

void RCImageEncoderPrivCustomTest::Cleanup()
	{
	delete iImageEncoderPriv; iImageEncoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	delete iImageData; iImageData = NULL;
	}

void RCImageEncoderPrivCustomTest::Close()
	{
	delete iScheduler; iScheduler = NULL;
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iRefImageData; iRefImageData = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}


//RCImageEncoderPrivRAEncodeTest
RCImageEncoderPrivRAEncodeTest* RCImageEncoderPrivRAEncodeTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RCImageEncoderPrivRAEncodeTest(aSuite);
	}

RCImageEncoderPrivRAEncodeTest::RCImageEncoderPrivRAEncodeTest(CTestSuite* aSuite)
	:RAllocTest(ETrue)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5204-HP");
	}

void RCImageEncoderPrivRAEncodeTest::SetupL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iConstruct = NULL;

	INFO_PRINTF1(_L("<<<   CImageEncoderPriv WriteDataL() Replace+Append Encode Test   >>>"));
	INFO_PRINTF1(_L("<<<   Added to test for symptoms of DEF040758...                  >>>"));

	// get a 4096 byte buffer like TI had
	iRefImageData = HBufC8::NewMaxL(KRAEncoderPrivTestSize);
	TPtr8 tempDes = iRefImageData->Des();

	// fill it with recurring sequence of (0 - 255)
	for (TInt j = 0; j < 16; j++)
		for (TInt i = 0; i < 256; i ++)
			{
			tempDes[i * j] = static_cast <TUint8> (i);
			}

	iImageData = NULL;
	iImageEncoderPriv = NULL;
	}

void RCImageEncoderPrivRAEncodeTest::TestStepL()
	{
	TUid imageTypeUid;
	TUid implementationUid;
	imageTypeUid = KImageTypeGIFUid;
	implementationUid.iUid = KGIFEncoderImplementationUidValue;

	// Use RCImageConstructTest to create a CImageEncodeConstruct
	RCImageEncodeConstructTest* constructTest = RCImageEncodeConstructTest::NewL(iSuite, EFalse);
	CleanupStack::PushL(constructTest);
	iConstruct = constructTest->CreatePluginL(imageTypeUid, implementationUid);
	CleanupStack::PopAndDestroy(constructTest);
	
	iImageEncoderPriv = CImageEncoderPriv::NewL(iConstruct, this);

	iImageEncoderPriv->CreatePluginL();

	// tell encoder priv we are using a descriptor
	// this can be NULL for our purposes since it will not be used
	iImageEncoderPriv->SetDataL(iImageData);

	// DEF040758 - WriteDataL does not allow replace and append at same time
	// call WriteDataL first time to setup the descriptor
	iImageEncoderPriv->WriteDataL(0, iRefImageData->Des());  

	// now set length of HBufC8 to 1896 and write at position 3608
	// this copies the example of TI
	TPtrC8 tempDes = iRefImageData->Left(KRAEncoderPrivTestLength);
	iImageEncoderPriv->WriteDataL(KRAEncoderPrivTestPosition, tempDes);
	}

void RCImageEncoderPrivRAEncodeTest::CheckResultL()
	{
	// check the length of the data corresponds to what we expect
	// we expect the internal image to now be 3608 + 1896 == 5504 bytes length
	TInt expSize = KRAEncoderPrivTestLength + KRAEncoderPrivTestPosition;
	TInt size = iImageEncoderPriv->CurrentImageSizeL();

	iTestStepResult = EPass;

	if (size != expSize)
		{
		INFO_PRINTF3(_L("Error - size of image = %d bytes;  Expected %d bytes!!!"), size, expSize);
		iTestStepResult = EFail;
		return;
		}
	
	// get access to the final destination buffer
	CImageEncoderPrivTestAccess* testAccess = new (ELeave) CImageEncoderPrivTestAccess(iImageEncoderPriv);
	CleanupStack::PushL(testAccess);

	HBufC8& buffer = testAccess->FinalDestinationBuffer();
	
	// check the data inside the buffer
	TPtrC8 leftSrcChunk = iRefImageData->Left(KRAEncoderPrivTestPosition);
	TPtrC8 leftDestChunk = buffer.Left(KRAEncoderPrivTestPosition);
	
	// before the insert position
	if (leftSrcChunk.Compare(leftDestChunk) != 0)
		{
		INFO_PRINTF1(_L("Error - data is not the same before insert point!"));
		iTestStepResult = EFail;
		}
	
	// after the insert position
	TPtrC8 rightSrcChunk = iRefImageData->Left(KRAEncoderPrivTestLength);
	TPtrC8 rightDestChunk = buffer.Mid(KRAEncoderPrivTestPosition, KRAEncoderPrivTestLength);

	if (rightSrcChunk.Compare(rightDestChunk) != 0)
		{
		INFO_PRINTF1(_L("Error - data is not the same after insert point!"));
		iTestStepResult = EFail;
		}

	// cleanup the testAccess class
	CleanupStack::PopAndDestroy(1, testAccess);
	}

void RCImageEncoderPrivRAEncodeTest::Cleanup()
	{
	delete iImageEncoderPriv; iImageEncoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	delete iImageData; iImageData = NULL;
	}

void RCImageEncoderPrivRAEncodeTest::Close()
	{
	delete iScheduler; iScheduler = NULL;
	delete iRefImageData; iRefImageData = NULL;
	RAllocTest::Close();
	}

