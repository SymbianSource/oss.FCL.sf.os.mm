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

#include "icl/ICL_UIDS.hrh"
#include "ImageEncoderTest.h"
#include "ICLFrameworkTestSuite.h"
#include "fwextconstants.h"
#include "TestUids.hrh"
#include "ImageDecoderTest.h" // for CFwExtTestPrepare

_LIT(KGifFile,"GIF87a.GIF");
_LIT(KGifFileRef,"GIF87a.MBM");
_LIT(KGifMimeType,"image/gif");
_LIT(KImageSourceFile, "c:\\TSU_ICL_FRM_01\\jfif444.jpg");
_LIT(KDummyImageFilename,"Dummy.jpg"); // Dummy filename, image doesn't exist
_LIT(KPNGFile, "c:\\fwextpngtestfile.png");
_LIT(KFileDummy, "c:\\dummyjpegtemp");

const TUid KTSTImageTypeUid = {0x101F7BF7};
const TUid KTSTImageSubTypeUid = {0x101F7BF8};
const TUid KTSTEncoderUid = {0x101F7C01};
const TUid KFwExtTestEncoder = {KTestFwExtEncoderImplementationUidValue};
const TUid KPNGEncoder = {KPNGEncoderImplementationUidValue};

const TInt KBufferSize = 0x1000;

//RCImageEncoderEncodeTest
RCImageEncoderEncodeTest* RCImageEncoderEncodeTest::NewL(CTestSuite* aSuite, TContentMode aContentMode, TBool aMimeTest, TBool aAllocTest)
	{
	return new (ELeave) RCImageEncoderEncodeTest(aSuite, aContentMode, aMimeTest, aAllocTest);
	}

RCImageEncoderEncodeTest::RCImageEncoderEncodeTest(CTestSuite* aSuite, TContentMode aContentMode, TBool aMimeTest, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iContentMode(aContentMode),
	iMimeTest(aMimeTest)
	{
	iSuite = aSuite;
	switch (iContentMode)
		{
		case EContentFile:
			{
			//Use SetFileL()
			if(!iMimeTest && !iAllocTest)
				iTestStepName = _L("MM-ICL-FRM-U-5400-HP");
			if(!iMimeTest && iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5401-HP");
			if(iMimeTest && !iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5402-HP");
			if(iMimeTest && iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5403-HP");
			break;
			}
		case EContentDescriptor:
			{
			//Use SetDataL()
			if(!iMimeTest && !iAllocTest)
				iTestStepName = _L("MM-ICL-FRM-U-5404-HP");
			if(!iMimeTest && iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5405-HP");
			if(iMimeTest && !iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5406-HP");
			if(iMimeTest && iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5407-HP");
			break;
			}
		case EContentFileHandle:
			{
			//Use SetFileL()
			if(!iMimeTest && !iAllocTest)
				iTestStepName = _L("MM-ICL-FRM-U-5450-HP");
			if(!iMimeTest && iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5451-HP");
			if(iMimeTest && !iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5452-HP");
			if(iMimeTest && iAllocTest)		
				iTestStepName = _L("MM-ICL-FRM-U-5453-HP");
			break;
			}			
		}
			
	}

void RCImageEncoderEncodeTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	switch(iContentMode)
		{
		case EContentFileHandle:
			INFO_PRINTF1(_L("Testing file handle APIs"));
			// drop through to EContentFile descriptions
		case EContentFile:
			{
			if(!iAllocTest)
				INFO_PRINTF1(_L("<<<   CImageEncoder FileNewL() Encode Test   >>>"));
			else
				INFO_PRINTF1(_L("<<<   CImageEncoder FileNewL() Encode Alloc Test   >>>"));
			break;
			}
		case EContentDescriptor:
			{
			if(!iAllocTest)
				INFO_PRINTF1(_L("<<<   CImageEncoder DataNewL() Encode Test   >>>"));
			else
				INFO_PRINTF1(_L("<<<   CImageEncoder DataNewL() Encode Alloc Test   >>>"));
			}
		}

	if(iMimeTest)
		INFO_PRINTF1(_L("Using MIME type"));

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

	if(iMimeTest)
		iMimeType.Copy(KGifMimeType);

	iImageData = NULL;
	iImageEncoder = NULL;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageEncoderEncodeTest::TestStepL()
	{
	TUid imageTypeUid = KImageTypeGIFUid;

	switch (iContentMode)
		{
		case EContentFile:
			{
			if(iMimeTest)
				iImageEncoder = CImageEncoder::FileNewL(iFileSession, iDestinationFilename, iMimeType);
			else
				iImageEncoder = CImageEncoder::FileNewL(iFileSession, iDestinationFilename, CImageEncoder::EOptionNone, imageTypeUid);
			break;
			}
		case EContentDescriptor:
			{
			if(iMimeTest)
				iImageEncoder = CImageEncoder::DataNewL(iImageData, iMimeType);
			else
				iImageEncoder = CImageEncoder::DataNewL(iImageData, CImageEncoder::EOptionNone, imageTypeUid);
			break;
			}
		case EContentFileHandle:
			{
			RFile file;
			User::LeaveIfError(file.Replace(iFileSession, iDestinationFilename, EFileWrite | EFileShareAny));
			CleanupClosePushL(file);
			if(iMimeTest)
				iImageEncoder = CImageEncoder::FileNewL(file, iMimeType);
			else
				iImageEncoder = CImageEncoder::FileNewL(file, CImageEncoder::EOptionNone, imageTypeUid);
			CleanupStack::PopAndDestroy(&file);
			break;
			}
		}


	TUid encoderUid = iImageEncoder->ImplementationUid();
	TUid expectedUid;
	expectedUid.iUid = KGIFEncoderImplementationUidValue;
	Compare(encoderUid, expectedUid, _L("Encoder UID incorrect")); 

	iListener->InitialiseActiveListener();
	iImageEncoder->Convert(&iListener->iStatus, *iSourceBitmap, NULL);
	CActiveScheduler::Start();

	TInt status;
	status = iListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);
	}

void RCImageEncoderEncodeTest::CheckResultL()
	{
	if (iContentMode == EContentFile || iContentMode == EContentFileHandle)
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

void RCImageEncoderEncodeTest::Cleanup()
	{
	delete iImageEncoder; iImageEncoder = NULL;
	delete iImageData; iImageData = NULL;
	
	if(iContentMode == EContentFile || iContentMode == EContentFileHandle)
		iFileSession.Delete(iDestinationFilename);

	}

void RCImageEncoderEncodeTest::Close()
	{
	delete iScheduler; iScheduler = NULL;
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iRefImageData; iRefImageData = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

//RCImageEncoderStaticTest
RCImageEncoderStaticTest* RCImageEncoderStaticTest::NewL(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest)
	{
	return new (ELeave) RCImageEncoderStaticTest(aSuite, aTestNumber, aAllocTest);
	}

RCImageEncoderStaticTest::RCImageEncoderStaticTest(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iTestNumber(aTestNumber)
	{
	iSuite = aSuite;
	//GetImageTypesL()
	if((iTestNumber == 0 && !iAllocTest))
		iTestStepName = _L("MM-ICL-FRM-U-5420-HP");
	if((iTestNumber == 0) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5421-HP");

	//GetImageSubTypesL()
	if((iTestNumber == 1) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5422-HP");
	if((iTestNumber == 1) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5423-HP");

	//GetFileTypesL()
	if((iTestNumber == 2) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5424-HP");
	if((iTestNumber == 2) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5425-HP");
	
	//GetPluginProperties() and GetInterfaceImplementations()
	if((iTestNumber ==3 ) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5428-HP");

	}

void RCImageEncoderStaticTest::SetupL()
	{
	//GetImageTypesL()
	if((iTestNumber == 0) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetImageTypesL() Test   >>>"));
	if((iTestNumber == 0) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetImageTypesL() Alloc Test   >>>"));

	//GetImageSubTypesL()
	if((iTestNumber == 1) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetImageSubTypesL() Test   >>>"));
	if((iTestNumber == 1) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetImageSubTypesL() Alloc Test   >>>"));

	//GetFileTypesL()
	if((iTestNumber == 2) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetFileTypesL() Test   >>>"));
	if((iTestNumber == 2) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetFileTypesL() Alloc Test   >>>"));
	
	//GetPluginPropertiesL() and GetInterfaceImplementationsL()
	if((iTestNumber == 3) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageEncoder::GetPluginPropertiesL() and CImageEncoder::GetInterfaceImplementationsL() Test   >>>"));
	
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	}

void RCImageEncoderStaticTest::TestStepL()
	{
	if(iTestNumber == 0)
		CImageEncoder::GetImageTypesL(iImageTypeArray);

	if(iTestNumber == 1)
		{
		TUid imageType(KTSTImageTypeUid);
		CImageEncoder::GetImageSubTypesL(imageType, iImageSubTypeArray);
		}

	if(iTestNumber == 2)
		CImageEncoder::GetFileTypesL(iFileExtensionArray);
	
	if(iTestNumber == 3)
		{
        User::LeaveIfError(FbsStartup());
		User::LeaveIfError(iFileSession.Connect());
		User::LeaveIfError(RFbsSession::Connect());
		
		TUid implUid = {KJPGEncoderImplementation2UidValue};	
		iPropertiesArray.Reset();
		CImageEncoder::GetPluginPropertiesL(implUid, iPropertiesArray);
		
		if(iPropertiesArray.Count() == 0)
			User::Leave(KErrNotFound);
		
		iImplArray.Reset();
		CImageEncoder::GetInterfaceImplementationsL(iPropertiesArray, iImplArray);
		
		if(iImplArray.Count() == 0)
			User::Leave(KErrNotFound);
		
		TFileName filename(KImageSourceFile);
		CImageEncoder* encoder = NULL;		
		TRAP(iError, encoder = CImageEncoder::FileNewL(iFileSession, filename, CImageEncoder::EOptionNone, KNullUid, KNullUid, iImplArray[0]));
		delete encoder;
		}
	}

void RCImageEncoderStaticTest::CheckResultL()
	{
	if(iTestNumber == 0)
		Compare(iImageTypeArray.Count() > 0, ETrue, _L("GetImageTypesL() returned 0 entries"));
	if(iTestNumber == 1)
		Compare(iImageSubTypeArray.Count() > 0, ETrue, _L("GetImageSubTypesL() returned 0 entries"));
	if(iTestNumber == 2)
		Compare(iFileExtensionArray.Count() > 0, ETrue, _L("GetFileTypesL() returned 0 entries"));
	if(iTestNumber == 3)
		Compare(iError == KErrNone, ETrue, _L("GetPluginPropertiesL() or GetInterfaceImplementationsL() returned 0 entries"));
	}

void RCImageEncoderStaticTest::Cleanup()
	{
	iImageTypeArray.ResetAndDestroy();
	iImageSubTypeArray.ResetAndDestroy();
	iFileExtensionArray.ResetAndDestroy();
	iPropertiesArray.Close();
	iImplArray.Close();
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}

//RThreadedEncodeCancelTest
RThreadedEncodeCancelTest* RThreadedEncodeCancelTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RThreadedEncodeCancelTest(aSuite);
	}

RThreadedEncodeCancelTest::RThreadedEncodeCancelTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5426-HP");
	}

TVerdict RThreadedEncodeCancelTest::OpenL()
	{
	INFO_PRINTF1(_L("Threaded Encode Cancel Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	TRAPD(error, iEncoder = CImageEncoder::DataNewL(iDestinationData, CImageEncoder::EOptionAlwaysThread, KTSTImageTypeUid, KTSTImageSubTypeUid, KTSTEncoderUid));
	Compare(error, KErrNone, _L("Encoder not be created"));

	iBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iBitmap->Create(TSize(10,10), EGray2));

	return iTestStepResult;
	}

TVerdict RThreadedEncodeCancelTest::DoTestStepL()
	{

	TRequestStatus convertStatus = KRequestPending;

	//The TestEncoder is set up to return KErrDied on Cancel()
	iEncoder->Convert(&convertStatus, *iBitmap);

	//Give the encoder time to call DoConvert()
	RTimer timer;
	timer.CreateLocal();
	TRequestStatus timerStatus = KRequestPending;;
	TTimeIntervalMicroSeconds32 interval = 1000000;
	timer.After(timerStatus, interval);
	User::WaitForRequest(timerStatus);
	timer.Close();

	//Cancel() the convert operation
	iEncoder->Cancel();

	User::WaitForRequest(convertStatus);
	TInt result = convertStatus.Int(); 
	Compare(result, KErrDied, _L("Cancel() on TestEncoder should return KErrDied"));
	
	return iTestStepResult;
	}

void RThreadedEncodeCancelTest::Close()
	{
	delete iEncoder; iEncoder = NULL;
	delete iBitmap; iBitmap = NULL;
	delete iDestinationData; iDestinationData = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}



/**
*
* Ctor for REncoderInvalidMimeTypeArg
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return REncoderInvalidMimeTypeArg*: constructed REncoderInvalidMimeTypeArg object
*
*/
REncoderInvalidMimeTypeArg* REncoderInvalidMimeTypeArg::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) REncoderInvalidMimeTypeArg(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
REncoderInvalidMimeTypeArg::REncoderInvalidMimeTypeArg(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7028-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict REncoderInvalidMimeTypeArg::DoTestStepL()
    {
    INFO_PRINTF1(_L("Creating a new CImageEncoder with MIME Type, but TDesC8 length for MIME arg is null, should leave with KErrArgument\n"));
	User::LeaveIfError(iFileSession.Connect());
	CImageEncoder* encoder = NULL;
	TBuf8<1> invalidMimeType;
	invalidMimeType.SetLength(0);
	TRAPD(err, encoder=CImageEncoder::FileNewL(iFileSession, KDummyImageFilename, invalidMimeType));
	if (err == KErrArgument)
		return EPass;
	delete encoder;
	// Fail the test if we reach this point
	return EFail;	
    }
    
/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the PNG Encoder.
	PNG does not support the framework extensions.

	@param	aSuite
				This suite.
	@param	aThreaded
				Is the encode to take place in a separate thread?
	@param	aMethod
				The method to be run (TFwExtMethod).
	@param	aTestStepName
				The name of this test step.
	@return RFwExtPNGEncNotSupportedTest*
				Constructed RFwExtPNGEncNotSupportedTest object
*/
RFwExtPNGEncNotSupportedTest* RFwExtPNGEncNotSupportedTest::NewL(CTestSuite* aSuite,
														TBool aThreaded,
    													TFwExtMethod aMethod,
    													const TDesC& aTestStepName)
	{
	return new (ELeave) RFwExtPNGEncNotSupportedTest(aSuite,
													aThreaded,
													aMethod,
													aTestStepName);
	}
	
RFwExtPNGEncNotSupportedTest::RFwExtPNGEncNotSupportedTest(CTestSuite* aSuite,
													TBool aThreaded,
    												TFwExtMethod aMethod,
    												const TDesC& aTestStepName)
	{
	iSuite = aSuite;
	iThreaded = aThreaded;
	iMethod = aMethod;
	iTestStepName = aTestStepName;
	}
	
TVerdict RFwExtPNGEncNotSupportedTest::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	User::LeaveIfError(iFileSession.Connect());
	
	if(iThreaded)
		{
		iOptions = CImageEncoder::EOptionAlwaysThread;
		}
		
	INFO_PRINTF2(_L("Testing PNG plugin: Implementation Uid = 0x%X"), KPNGEncoder);
	
	TPtrC fileName(KPNGFile);
	INFO_PRINTF2(_L("Creating the file %S"), &fileName);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, KPNGFile, EFileWrite | EFileShareAny));
	CleanupClosePushL(file);

	TRAPD(err, iImageEncoder = CImageEncoder::FileNewL(iFileSession, fileName, iOptions, KNullUid, KNullUid, KPNGEncoder));
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageEncoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}	
	// Check that the plugin correctly supports threading or not		
	TInt threadErr = iImageEncoder->SetEncoderThreadPriority(EPriorityNormal);
	if(threadErr == KErrNotSupported && !iThreaded)
		{
		INFO_PRINTF1(_L("CImageEncoder not threaded"));
		}
	else if (threadErr == KErrNone && iThreaded)
		{
		INFO_PRINTF1(_L("CImageEncoder threaded"));
		}
	else
		{
		INFO_PRINTF1(_L("CImageEncoder threading issue"));
		iTestStepResult = EFail;
		}
	
	CleanupStack::PopAndDestroy(&file);
	
	return iTestStepResult;
	}
	
TVerdict RFwExtPNGEncNotSupportedTest::DoTestStepL()
	{
	TInt err = KErrNone;
	TBool extensionPresent = EFalse;
	switch(iMethod)
		{
		case EOperationL:
			{
			INFO_PRINTF1(_L("OperationL()"));
			TRAP(err, iImageEncoder->OperationL());
			break;
			}
		case EBlockStreamerL:
			{
			INFO_PRINTF1(_L("BlockStreamerL()"));
			TRAP(err, iImageEncoder->BlockStreamerL());
			break;
			}
		default:
			{
			SetFail(_L("Called unsupported function"));
			}
		}
		
	if(err != KErrNotSupported && extensionPresent == EFalse)
		{
		SetFail(_L("Unsupported function returned unexpected result"));
		}
		
	return iTestStepResult;
	}
	
void RFwExtPNGEncNotSupportedTest::Close()
	{
	delete iImageEncoder;
	iImageEncoder = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	}

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvOperation interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CEncodeOperationExtension
*/
CFwExtTestEncOperation* CFwExtTestEncOperation::NewL()
	{
	return new (ELeave) CFwExtTestEncOperation();
	}

CFwExtTestEncOperation::CFwExtTestEncOperation()
:iReferenceCount(1)
	{
	}

CFwExtTestEncOperation::~CFwExtTestEncOperation()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CFwExtTestEncOperation::Uid() const
	{
	return KICLOperationUid;
	}
	
void CFwExtTestEncOperation::IncrementRef()
	{
	iReferenceCount++;
	}
	
void CFwExtTestEncOperation::Release()
	{
	iReferenceCount--;
	}
	
TUint CFwExtTestEncOperation::Capabilities() const
	{
	return iOperationCaps;
	}

void CFwExtTestEncOperation::SetCapabilities(TUint aCaps)
	{
	iOperationCaps = aCaps;
	}
	
void CFwExtTestEncOperation::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	/*
	This bitwise OR is purely for testing purposes only.  An actual
	AddOperationL call would append the operation to a stack of operations.
	*/
	iOperations |= aOperation;
	}
	
TUint CFwExtTestEncOperation::Operations()
	{
	return iOperations;
	}

void CFwExtTestEncOperation::ClearOperationStack()
	{
	iOperations = 0;
	}

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvStreamedEncode interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CStreamedEncodeExtension
*/
CFwExtTestStreamedEncode* CFwExtTestStreamedEncode::NewL()
	{
	return new (ELeave) CFwExtTestStreamedEncode();
	}

CFwExtTestStreamedEncode::CFwExtTestStreamedEncode()
:iReferenceCount(1)
	{
	}

CFwExtTestStreamedEncode::~CFwExtTestStreamedEncode()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CFwExtTestStreamedEncode::Uid() const
	{
	return KICLStreamedEncodeUid;
	}

void CFwExtTestStreamedEncode::IncrementRef()
	{
	iReferenceCount++;
	}
	
void CFwExtTestStreamedEncode::Release()
	{
	iReferenceCount--;
	}

void CFwExtTestStreamedEncode::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	aFormats = iFormats;
	aOptimalFormat = iOptimalFormat;
	}
	
void CFwExtTestStreamedEncode::SetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat)
	{
	iFormats = aFormats;
	iOptimalFormat = aOptimalFormat;
	}

void CFwExtTestStreamedEncode::GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const
	{
	if(iFormat == aFormat)
		{
		aCaps = iEncodeStreamCaps;	
		}
	}

void CFwExtTestStreamedEncode::SetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps)
	{
	iFormat = aFormat;
	iEncodeStreamCaps = aCaps;
	}

void CFwExtTestStreamedEncode::InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	iFormatInitFrameL = aFormat;
	iFrameNumber = aFrameNumber;
	iFrameSizeInPixels = aFrameSizeInPixels;
	iBlockSizeInPixels = aBlockSizeInPixels;
	iNavigation = aNavigation;
	iFrameImageData = aFrameImageData;
	}

TInt CFwExtTestStreamedEncode::CheckInitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	if(iFormatInitFrameL == aFormat
		&& iFrameNumber == aFrameNumber
		&& iFrameSizeInPixels == aFrameSizeInPixels
		&& iBlockSizeInPixels == aBlockSizeInPixels
		&& iNavigation == aNavigation
		&& iFrameImageData == aFrameImageData
		&& iFrameImageData->FrameDataCount() == 1)
		{
		return KErrNone;
		}
	return KErrGeneral;
	}

void CFwExtTestStreamedEncode::AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	iReqAppendBlocks = aStatus;
	iBlocksAppendBlocks = &aBlocks;
	iNumBlocksToAddAppendBlocks = aNumBlocksToAdd;
	}
	
TInt CFwExtTestStreamedEncode::CheckAppendBlocks(const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	if((aBlocks.FrameLayout()).Type() == (iBlocksAppendBlocks->FrameLayout()).Type())
		{
		const TFrameLayout& frameLayout = static_cast<const TFrameLayout&>(aBlocks.FrameLayout());
		if(&frameLayout)
			{
			if(frameLayout.Planes() == KMaxPlanesInFrame)
				{
				if(aNumBlocksToAdd == iNumBlocksToAddAppendBlocks
					&& iReqAppendBlocks == NULL)
					{
					return KErrNone;
					}
				}
			}
		}
	return KErrGeneral;
	}

void CFwExtTestStreamedEncode::AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	iReqAddBlocks = aStatus;
	iBlocksAddBlocks = &aBlocks;
	iSeqPosition = aSeqPosition;
	iSeqPosition++;
	}

TInt CFwExtTestStreamedEncode::CheckAddBlocks(const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	if((aBlocks.FrameLayout()).Type() == (iBlocksAddBlocks->FrameLayout()).Type())
		{
		const TFrameLayout& frameLayout = static_cast<const TFrameLayout&>(aBlocks.FrameLayout());
		if(&frameLayout)
			{
			if(frameLayout.Planes() == KMaxPlanesInFrame)
				{
				if(aSeqPosition == iSeqPosition
					&& iReqAddBlocks == NULL)
					{
					return KErrNone;
					}
				}
			}
		}
	return KErrGeneral;
	}

void CFwExtTestStreamedEncode::Complete(TRequestStatus* aStatus)
	{
	iReqComplete = aStatus;
	}

TInt CFwExtTestStreamedEncode::CheckComplete()
	{
	if(iReqComplete == NULL)
		{
		return KErrNone;
		}
	return KErrGeneral;
	}

/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the FwExtTest Encoder.
	FwExtTest codec supports the framework extensions.

	@param	aSuite
				This suite.
	@param	aThreaded
				Is the encode to take place in a separate thread?			
	@param	aMethod
				The method to be run (TFwExtMethod).
	@param	aTestStepName
				The name of this test step.
	@return RFwExtEncSupportedTest*
				Constructed RFwExtEncSupportedTest object
*/
RFwExtEncSupportedTest* RFwExtEncSupportedTest::NewL(CTestSuite* aSuite,
    											TBool aThreaded,
    											TFwExtMethod aMethod,
    											const TDesC& aTestStepName)
	{
	return new (ELeave) RFwExtEncSupportedTest(aSuite,
											aThreaded,
											aMethod,
											aTestStepName);
	}
	
RFwExtEncSupportedTest::RFwExtEncSupportedTest(CTestSuite* aSuite,
										TBool aThreaded,
										TFwExtMethod aMethod,
										const TDesC& aTestStepName)
	{
	iSuite = aSuite;
	iThreaded = aThreaded;
	iMethod = aMethod;
	iTestStepName = aTestStepName;
	}

TVerdict RFwExtEncSupportedTest::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());
	
	if(iThreaded)
		{
		iOptions = CImageEncoder::EOptionAlwaysThread;
		}
		
	INFO_PRINTF2(_L("Testing FwExtTest encoder plugin: Implementation Uid = 0x%X"), KFwExtTestEncoder);
		
	// use DataNewL - less framework compliance in codec reqiored
	TRAPD(err, iImageEncoder = static_cast<CFwExtImageEncoder*>(CImageEncoder::DataNewL(iDestinationImage, iOptions, KNullUid, KNullUid, KFwExtTestEncoder)));
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageEncoder::DataNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}	
	// Check that the plugin correctly supports threading or not		
	TInt threadErr = iImageEncoder->SetEncoderThreadPriority(EPriorityNormal);
	if(threadErr == KErrNotSupported && !iThreaded)
		{
		INFO_PRINTF1(_L("CImageEncoder not threaded"));
		}
	else if (threadErr == KErrNone && iThreaded)
		{
		INFO_PRINTF1(_L("CImageEncoder threaded"));
		}
	else
		{
		INFO_PRINTF1(_L("CImageEncoder threading issue"));
		iTestStepResult = EFail;
		}
	
	return iTestStepResult;
	}
	
TVerdict RFwExtEncSupportedTest::DoTestStepL()
	{
	TInt err = KErrNone;
	
	switch(iMethod)
		{
		case EOperationL:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestEncOperation* sharedTestExt = CFwExtTestEncOperation::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageEncoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvOperation* operation = NULL;
			operation = iImageEncoder->OperationL();
			INFO_PRINTF1(_L("OperationL() succeeded."));
			
			TUint caps = 0x0F;
			sharedTestExt->SetCapabilities(caps);
			
			// Compare capabilities
			if(operation->Capabilities() == caps)
				{
				INFO_PRINTF1(_L("Capabilities() returned correct result."));
				}
			else
				{
				INFO_PRINTF1(_L("Capabilities() returned unexpected result."));
				return EFail;
				}

			operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
			operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				
			TUint checkOperations = TImageConvOperation::EMirrorVerticalAxis | TImageConvOperation::ERotation270DegreesClockwise;
			
			// Compare set operations
			if(sharedTestExt->Operations() == checkOperations)
				{
				INFO_PRINTF3(_L("AddOperationL() has correctly set values. 0x%x == 0x%x"), sharedTestExt->Operations(), checkOperations);
				}
			else
				{
				INFO_PRINTF3(_L("AddOperationL() has incorrectly set values. 0x%x != 0x%x"), sharedTestExt->Operations(), checkOperations);
				return EFail;
				}
			
			// reset
			operation->ClearOperationStack();
			checkOperations = 0;
			
			// Compare reset values
			if(sharedTestExt->Operations() == checkOperations)
				{
				INFO_PRINTF1(_L("ClearOperationStack() was successful."));
				}
			else
				{
				INFO_PRINTF1(_L("ClearOperationStack() was not successful."));
				return EFail;
				}	
			
			break;
			}			
		case EPrepare:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestPrepare* sharedTestExt = CFwExtTestPrepare::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageEncoder->CustomSyncL(TInt(sharedTestExtBase));
			
			iActiveListener = new (ELeave) CActiveListener();
			iActiveListener->InitialiseActiveListener();
			
			sharedTestExt->SetPrepare(ETrue);
	
			iImageEncoder->Prepare(&iActiveListener->iStatus);
			
			CActiveScheduler::Start();
			TInt statusErr = iActiveListener->iStatus.Int();
			
			if (statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("Prepare() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				// Prepare processing complete, now do the conversion proper
				iTestBitmap = new (ELeave) CFbsBitmap;
				User::LeaveIfError(iTestBitmap->Create(TSize(10,10), EColor16M));
		
				iActiveListener->InitialiseActiveListener();

				iImageEncoder->Convert(&iActiveListener->iStatus, *iTestBitmap, 0);
				
				CActiveScheduler::Start();
				statusErr = iActiveListener->iStatus.Int();

				delete iTestBitmap; iTestBitmap = NULL;

				if (statusErr != KErrNone)
					{
					INFO_PRINTF2(_L("Convert after Prepare() async call failed with %d"), statusErr);
					return EFail;
					}
				else
					{
					INFO_PRINTF2(_L("Prepare() async call passed with %d"), statusErr);
					}
				}
			break;
			}
		case EBlockStreamerLSetup:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestStreamedEncode* sharedTestExt = CFwExtTestStreamedEncode::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageEncoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvStreamedEncode* streamEncode = iImageEncoder->BlockStreamerL();
			INFO_PRINTF1(_L("BlockStreamerL() succeeded."));

			RArray<TUid> formatsCheck;
			formatsCheck.AppendL(TUid::Uid(0x11111111));
			formatsCheck.AppendL(TUid::Uid(0x22222222));
			formatsCheck.AppendL(TUid::Uid(0x99991234));
			TUid optimalFormatCheck = TUid::Uid(0x99991234);
			sharedTestExt->SetSupportedFormatsL(formatsCheck, optimalFormatCheck);

			RArray<TUid> formats;
			TUid optimalFormat;
			streamEncode->GetSupportedFormatsL(formats, optimalFormat);
			
			
			if(optimalFormat != optimalFormatCheck)
				{
				INFO_PRINTF1(_L("GetSupportedFormatsL() failed (aOptimalFormat incorrect)."));
				}
			
			if(formats.Count() == formatsCheck.Count())
				{
				for(TInt i = 0; i < formats.Count(); i++)
					{
					// Check all elements match
					if(formats[i] != formatsCheck[i])
						{
						return EFail;
						}
					}
				INFO_PRINTF1(_L("GetSupportedFormatsL() succeeded."));	
				}
			else
				{
				INFO_PRINTF1(_L("GetSupportedFormatsL() failed (aFormats returned unexpected length)."));
				return EFail;
				}
   
			TInt maxBlocksPerRequest = 3;
			TSize minBlockSizeInPixels(4,5);
			TSize optimalBlockSizeInPixels(8,9);
			TInt optimalBlocksPerRequest = 256;
			TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
			TEncodeStreamCaps streamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
										optimalBlocksPerRequest, navigation);
			sharedTestExt->SetCapabilities(optimalFormat, streamCaps);
			
			TEncodeStreamCaps caps;
			streamEncode->GetCapabilities(optimalFormat, caps);
			
			// Compare values
			if(caps.MaxBlocksPerRequest() == maxBlocksPerRequest
				&& caps.MinBlockSizeInPixels() == minBlockSizeInPixels
				&& caps.OptimalBlocksPerRequest() == optimalBlocksPerRequest
				&& caps.Navigation() == navigation)
				{
				INFO_PRINTF1(_L("GetCapabilities() succeeded in setting values."));
				}
			else
				{
				INFO_PRINTF1(_L("GetCapabilities() failed."));
				return EFail;
				}
						
			break;
			}
		case EBlockStreamerLAppendBlocks:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestStreamedEncode* sharedTestExt = CFwExtTestStreamedEncode::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageEncoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvStreamedEncode* streamEncode = iImageEncoder->BlockStreamerL();
			INFO_PRINTF1(_L("BlockStreamerL() succeeded."));
			
			TUid optimalFormat = TUid::Uid(0x99991234);
			TInt frameNumber = 51;
			TSize frameSizeInPixels(98,76);
			TSize blockSizeInPixels(44,66);
			TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
			CFrameImageData* frameImageData = CFrameImageData::NewL();
			CleanupStack::PushL(frameImageData);
			TTiffImageData data;
			data.iWidthInTwips = 9.1;
			data.iHeightInTwips =2.4;
			frameImageData->AppendFrameData(&data);

			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			CleanupStack::Pop(frameImageData);
			
			INFO_PRINTF1(_L("InitFrameL()"));
			
			HBufC8* buffer = HBufC8::NewMaxL(KBufferSize);
			CleanupStack::PushL(buffer);
	  		TPtr8 ptr = buffer->Des();

			CImageFrame* imageFrame = CImageFrame::NewL(ptr, KBufferSize);
			INFO_PRINTF1(_L("Successfully created CImageFrame for AppendBlocks()."));
			CleanupStack::PushL(imageFrame);
			TFrameLayout frameLayout(KMaxPlanesInFrame);
			imageFrame->SetFrameLayoutL(frameLayout); // Check in extension and on return
			
			TInt numBlocksToAdd = 12;
			iActiveListener = new (ELeave) CActiveListener();
			iActiveListener->InitialiseActiveListener();
			streamEncode->AppendBlocks(iActiveListener->iStatus, *imageFrame, numBlocksToAdd);
			CActiveScheduler::Start();
			TInt statusErr = iActiveListener->iStatus.Int();
			TInt err = sharedTestExt->CheckAppendBlocks(*imageFrame, numBlocksToAdd);
			CleanupStack::PopAndDestroy(imageFrame);
			CleanupStack::PopAndDestroy(buffer);
			
			if(statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("AppendBlocks() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				INFO_PRINTF2(_L("AppendBlocks() async call passed with %d"), statusErr);
				}
			
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("AppendBlocks() set values incorrectly"));
				return EFail;
				}
			else
				{
				INFO_PRINTF1(_L("AppendBlocks() set values correctly"));
				}
			
			iActiveListener->InitialiseActiveListener();
			streamEncode->Complete(iActiveListener->iStatus);
			CActiveScheduler::Start();
			statusErr = iActiveListener->iStatus.Int();
			err = sharedTestExt->CheckComplete();
			
			if(statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("Complete() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				INFO_PRINTF2(_L("Complete() async call passed with %d"), statusErr);
				}
			
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("Complete() set unexpected value in plugin"));
				return EFail;
				}
			else
				{
				INFO_PRINTF1(_L("Complete() set expected value in plugin"));
				}	
			break;
			}
		case EBlockStreamerLAddBlocks:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestStreamedEncode* sharedTestExt = CFwExtTestStreamedEncode::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageEncoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvStreamedEncode* streamEncode = iImageEncoder->BlockStreamerL();
			INFO_PRINTF1(_L("BlockStreamerL() succeeded."));
			
			TUid optimalFormat = TUid::Uid(0x99991234);
			TInt frameNumber = 51;
			TSize frameSizeInPixels(98,76);
			TSize blockSizeInPixels(44,66);
			TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
			CFrameImageData* frameImageData = CFrameImageData::NewL();
			CleanupStack::PushL(frameImageData);
			TTiffImageData data;
			data.iWidthInTwips = 9.1;
			data.iHeightInTwips =2.4;
			frameImageData->AppendFrameData(&data);

			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			CleanupStack::Pop(frameImageData);
			
			INFO_PRINTF1(_L("InitFrameL()"));
			
			HBufC8* buffer = HBufC8::NewMaxL(KBufferSize);
			CleanupStack::PushL(buffer);
	  		TPtr8 ptr = buffer->Des();

			CImageFrame* imageFrame = CImageFrame::NewL(ptr, KBufferSize);
			INFO_PRINTF1(_L("Successfully created CImageFrame for AddBlocks()."));
			CleanupStack::PushL(imageFrame);
			TFrameLayout frameLayout(KMaxPlanesInFrame);
			imageFrame->SetFrameLayoutL(frameLayout); // Check in extension and on return
			
			TInt seqPosition = 0;
			iActiveListener = new (ELeave) CActiveListener();
			iActiveListener->InitialiseActiveListener();
			streamEncode->AddBlocks(iActiveListener->iStatus, *imageFrame, seqPosition);
			CActiveScheduler::Start();
			TInt statusErr = iActiveListener->iStatus.Int();
			TInt seqPositionCheck = 1;
			err = sharedTestExt->CheckAddBlocks(*imageFrame, seqPositionCheck);
			CleanupStack::PopAndDestroy(imageFrame);
			CleanupStack::PopAndDestroy(buffer);

			if(statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("AddBlocks() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				INFO_PRINTF2(_L("AddBlocks() async call passed with %d"), statusErr);
				}
			
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("AddBlocks() set values incorrectly"));
				return EFail;
				}
			else
				{
				INFO_PRINTF1(_L("AddBlocks() set values correctly"));
				}
			
			iActiveListener->InitialiseActiveListener();
			streamEncode->Complete(iActiveListener->iStatus);
			CActiveScheduler::Start();
			statusErr = iActiveListener->iStatus.Int();
			err = sharedTestExt->CheckComplete();
			
			if(statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("Complete() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				INFO_PRINTF2(_L("Complete() async call passed with %d"), statusErr);
				}
			
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("Complete() set unexpected value in plugin"));
				return EFail;
				}
			else
				{
				INFO_PRINTF1(_L("Complete() set expected value in plugin"));
				}
			
			break;
			}
		default:
			{
			SetFail(_L("Called unsupported function"));
			}
		}
	
	return iTestStepResult;
	}

void RFwExtEncSupportedTest::Close()
	{
	delete iDestinationImage;
	delete iImageEncoder;
	iImageEncoder = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	delete iActiveListener;
	iActiveListener = NULL;
	}
	

//RThreadedFileEncodeCancelAfterConvertTest
RThreadedFileEncodeCancelAfterConvertTest* RThreadedFileEncodeCancelAfterConvertTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RThreadedFileEncodeCancelAfterConvertTest(aSuite);
	}

RThreadedFileEncodeCancelAfterConvertTest::RThreadedFileEncodeCancelAfterConvertTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-9005-HP");
	}

TVerdict RThreadedFileEncodeCancelAfterConvertTest::OpenL()
	{
	INFO_PRINTF1(_L("Threaded File Encode Cancel After Convert Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	TPtrC fileName(KFileDummy);

	INFO_PRINTF2(_L("Creating the file %S"), &fileName);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, KFileDummy, EFileWrite | EFileShareAny));
	CleanupClosePushL(file);

	TRAPD(err, iEncoder = CImageEncoder::FileNewL(iFileSession, fileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, TUid::Uid(KJPGEncoderImplementationUidValue)));
 
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageEncoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}
		
	TFileName mbmFileName;
	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(mbmFileName, refFileName);
	iBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iBitmap->Load(mbmFileName));
	
	CleanupStack::PopAndDestroy(&file);
	
	return iTestStepResult;
	}

TVerdict RThreadedFileEncodeCancelAfterConvertTest::DoTestStepL()
	{

	TRequestStatus convertStatus = KRequestPending;
	
	//The TestEncoder is set up to return KErrDied on Cancel()
	iEncoder->Convert(&convertStatus, *iBitmap);

	//Give the encoder time to call DoConvert()
	RTimer timer;
	timer.CreateLocal();
	TRequestStatus timerStatus = KRequestPending;;
	TTimeIntervalMicroSeconds32 interval = 1000000;
	timer.After(timerStatus, interval);
	User::WaitForRequest(timerStatus);
	timer.Close();

	//Cancel() the convert operation
	iEncoder->Cancel();

	User::WaitForRequest(convertStatus);
	TInt result = convertStatus.Int(); 
	Compare(result, KErrCancel, _L("Cancel() on TestEncoder should return KErrCancel"));
	

	TFileName fileName(KFileDummy);
	RFile file;
	TInt errCode = KErrNone;

	errCode = file.Open(iFileSession, fileName, EFileRead);
	
	//Open and load the file
	if (errCode != KErrNotFound)	
		{
		INFO_PRINTF2(_L("Threaded File Encode Cancel After Convert Test: Failed %d"), errCode);
		iTestStepResult = EFail;
		}

	return iTestStepResult;
	}

void RThreadedFileEncodeCancelAfterConvertTest::Close()
	{
	delete iEncoder; iEncoder = NULL;
	delete iBitmap; iBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}

//RThreadedFileHandleEncodeCancelAfterConvertTest
RThreadedFileHandleEncodeCancelAfterConvertTest* RThreadedFileHandleEncodeCancelAfterConvertTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RThreadedFileHandleEncodeCancelAfterConvertTest(aSuite);
	}

RThreadedFileHandleEncodeCancelAfterConvertTest::RThreadedFileHandleEncodeCancelAfterConvertTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-9006-HP");
	}

TVerdict RThreadedFileHandleEncodeCancelAfterConvertTest::OpenL()
	{
	INFO_PRINTF1(_L("Threaded File Handle Encode Cancel After Convert Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	TPtrC fileName(KFileDummy);

	INFO_PRINTF2(_L("Creating the file %S"), &fileName);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, KFileDummy, EFileWrite | EFileShareAny));
	CleanupClosePushL(file);
		
	TRAPD(err, iEncoder = CImageEncoder::FileNewL(file, CImageEncoder::EOptionNone, KNullUid, KNullUid, TUid::Uid(KJPGEncoderImplementationUidValue)));
 
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageEncoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}
		
	TFileName mbmFileName;
	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(mbmFileName, refFileName);
	iBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iBitmap->Load(mbmFileName));
	
	CleanupStack::PopAndDestroy(&file);
	
	return iTestStepResult;
	}

TVerdict RThreadedFileHandleEncodeCancelAfterConvertTest::DoTestStepL()
	{

	TRequestStatus convertStatus = KRequestPending;
	
	//The TestEncoder is set up to return KErrDied on Cancel()
	iEncoder->Convert(&convertStatus, *iBitmap);

	//Give the encoder time to call DoConvert()
	RTimer timer;
	timer.CreateLocal();
	TRequestStatus timerStatus = KRequestPending;;
	TTimeIntervalMicroSeconds32 interval = 1000000;
	timer.After(timerStatus, interval);
	User::WaitForRequest(timerStatus);
	timer.Close();

	//Cancel() the convert operation
	iEncoder->Cancel();

	User::WaitForRequest(convertStatus);
	TInt result = convertStatus.Int(); 
	Compare(result, KErrCancel, _L("Cancel() on TestEncoder should return KErrCancel"));
	

	TFileName fileName(KFileDummy);
	RFile file;
	TInt errCode = KErrNone;

	errCode = file.Open(iFileSession, fileName, EFileRead);
	
	//Open and load the file
	if (errCode != KErrNone)	
		{
		INFO_PRINTF2(_L("Threaded File Handle Encode Cancel After Convert Test: Failed %d"), errCode);
		iTestStepResult = EFail;
		}

	TInt size;
	User::LeaveIfError(file.Size(size));
	if (size)
		{
		INFO_PRINTF2(_L("Threaded File Handle Encode Cancel After Convert Test: Failed file size not zero %d"), size);
		iTestStepResult = EFail;		
		}
	
	return iTestStepResult;
	}

void RThreadedFileHandleEncodeCancelAfterConvertTest::Close()
	{
	delete iEncoder; iEncoder = NULL;
	delete iBitmap; iBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}

//RThreadedFileEncodeDeleteTest
RThreadedFileEncodeDeleteTest* RThreadedFileEncodeDeleteTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RThreadedFileEncodeDeleteTest(aSuite);
	}

RThreadedFileEncodeDeleteTest::RThreadedFileEncodeDeleteTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-9007-HP");
	}

TVerdict RThreadedFileEncodeDeleteTest::OpenL()
	{
	INFO_PRINTF1(_L("Threaded File Encode Delete Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	TPtrC fileName(KFileDummy);

	INFO_PRINTF2(_L("Creating the file %S"), &fileName);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, KFileDummy, EFileWrite | EFileShareAny));
	CleanupClosePushL(file);

	TRAPD(err, iEncoder = CImageEncoder::FileNewL(iFileSession, fileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, TUid::Uid(KJPGEncoderImplementationUidValue)));
 
 	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageEncoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}
		
	TFileName mbmFileName;
	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(mbmFileName, refFileName);
	iBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iBitmap->Load(mbmFileName));
	
	CleanupStack::PopAndDestroy(&file);
	
	return iTestStepResult;
	}
	
TVerdict RThreadedFileEncodeDeleteTest::DoTestStepL()
	{

	//Delete the encoder operation
	delete iEncoder; iEncoder = NULL;	

	TFileName fileName(KFileDummy);
	RFile file;
	TInt errCode = KErrNone;
	errCode = file.Open(iFileSession, fileName, EFileRead);
	
	//Open and load the file
	if (errCode != KErrNotFound)
		{
		INFO_PRINTF2(_L("Threaded File Encode Delete Test: Failed %d"), errCode);
		iTestStepResult = EFail;
		}
	
	return iTestStepResult;
	}

void RThreadedFileEncodeDeleteTest::Close()
	{
	delete iBitmap; iBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}
	
//RThreadedFileHandleEncodeDeleteTest
RThreadedFileHandleEncodeDeleteTest* RThreadedFileHandleEncodeDeleteTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RThreadedFileHandleEncodeDeleteTest(aSuite);
	}

RThreadedFileHandleEncodeDeleteTest::RThreadedFileHandleEncodeDeleteTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-9008-HP");
	}
	
TVerdict RThreadedFileHandleEncodeDeleteTest::OpenL()
	{
	INFO_PRINTF1(_L("Threaded File Handle Encode Delete Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	TPtrC fileName(KFileDummy);

	INFO_PRINTF2(_L("Creating the file %S"), &fileName);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, KFileDummy, EFileWrite | EFileShareAny));
	CleanupClosePushL(file);

	TRAPD(err, iEncoder = CImageEncoder::FileNewL(file, CImageEncoder::EOptionNone, KNullUid, KNullUid, TUid::Uid(KJPGEncoderImplementationUidValue)));
 
 	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageEncoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}
		
	TFileName mbmFileName;
	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(mbmFileName, refFileName);
	iBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iBitmap->Load(mbmFileName));
	
	CleanupStack::PopAndDestroy(&file);
	
	return iTestStepResult;
	}

TVerdict RThreadedFileHandleEncodeDeleteTest::DoTestStepL()
	{

	//Delete the encoder operation	
	delete iEncoder; iEncoder = NULL;
	
	TFileName fileName(KFileDummy);
	RFile file;
	TInt errCode = KErrNone;
	errCode = file.Open(iFileSession, fileName, EFileRead);
	
	//Open and load the file
	if (errCode != KErrNone)
		{
		INFO_PRINTF2(_L("Threaded File Handle Encode Delete Test: Failed %d"), errCode);
		iTestStepResult = EFail;
		}
	
	TInt size;
	User::LeaveIfError(file.Size(size));
	if (size)
		{
		INFO_PRINTF2(_L("Threaded File Handle Encode Delete Test: Failed file size not zero %d"), size);
		iTestStepResult = EFail;		
		}

	return iTestStepResult;
	}

void RThreadedFileHandleEncodeDeleteTest::Close()
	{

	delete iBitmap; iBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}

