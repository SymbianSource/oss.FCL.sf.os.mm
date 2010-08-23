// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// EPOC includes
#include <e32base.h>
#include <imagetransform.h>
#include <icl/imagetransformpluginext.h>
#include "TestPlugin/TestTransformerExt.h"
#include "ICLFbsSessionTest.h"

// Test system includes
#include <testframework.h>

#include "TestStepImageTransform.h"
#include "TestSuiteImageTransform.h"

#include <bitdev.h>

// Heap size for these tests.
const TInt KHeapSize = 0x100000;

const TInt KMaxMimeLength = 256;

const TUid KUidTestTransformerExt = {0x101F7C44};

_LIT(KIclPanicCategory, "TS_ImageTransform");


/** 
 This function raises a panic

@param TS_ImageTransformMain::TS_ImageTransformPanic one of the several panics codes
       that may be raised by this dll
@panic  ENoSourceBitmap is raised when the src bitmap supplied to one of the api's
	    has not been created ie has a null handle

*/

GLDEF_C void Panic(TS_ImageTransformPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

	
/*
CActiveListener
*/
CActiveListener::CActiveListener(CImageTransform& aImageTransform) :
	CActive(CActive::EPriorityIdle), iImageTransform(aImageTransform)
	{
	CActiveScheduler::Add(this);
	}

CActiveListener::~CActiveListener()
	{
	Cancel();
	}

/*
InitialiseActiveListener
*/
void CActiveListener::InitialiseActiveListener()
	{
    iStatus = KRequestPending; 
	SetActive();
	}

/*
RunL
*/

void CActiveListener::RunL()
	{
	CActiveScheduler::Stop(); 
	}

/*
  DoCancel()
  */
void CActiveListener::DoCancel()
	{	
	iImageTransform.CancelTransform();
	}


/**
*
* CTestStepImageTransform constructor 
*
* 
*/
CTestStepImageTransform::CTestStepImageTransform() 
:  iFs(), iImageTransformSuite(NULL), iSourceFilename(), iScheduler(NULL)
	{
	}

/**
*
* CTestStepImageTransform destructor
*
* 
*/
CTestStepImageTransform::~CTestStepImageTransform()
	{
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTestSuiteImageTransform
*/

void CTestStepImageTransform::SetSuite(CTestSuiteImageTransform* aPtr)
	{
	iImageTransformSuite = aPtr;
	}


/**
DoTestStepPreambleL is the preamble function for test steps
It creates an active scheduler and installs it. This is necessary
since we are multithreading and each test step runs in a separate thread.
@returns TVerdict which is either EPass or Efail
The function can leave if there is insufficient memeory
*/
TVerdict CTestStepImageTransform::DoTestStepPreambleL()
	{
	//[ mark the heap and unmark at the end of postamble ]
	__MM_HEAP_MARK;

	iScheduler = new(ELeave)CActiveScheduler;
    //[ push the scheduler on the stack ]
	CleanupStack::PushL( iScheduler );
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
	
	// [Pop iScheduler ] 
	CleanupStack::Pop( iScheduler);
	
	User::LeaveIfError(iFs.Connect());

	return EPass;
	}

/**
The post amble step cleans up the allocated memory in the preamble
@returns TVerdict EPass or EFail
@leaves can leave due to insufficient memory
*/

TVerdict CTestStepImageTransform::DoTestStepPostambleL()
	{
	iFs.Close();

	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	
    // [ umark the heap which was marked in preamble ]
	__MM_HEAP_MARKEND;

	return EPass;
	}


/**
*
* CTestStepImageTransform Suite accessor function
* @returns CTestSuiteImageTransform*  
*/

CTestSuiteImageTransform* CTestStepImageTransform::Suite()
	{
	return iImageTransformSuite;
	}


/**
*
* CTestStepImageTransform SetTestFilename
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&  afull file and path name
* @returns void
*/

void CTestStepImageTransform::SetSourceFilenameL(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName = Suite()->DefaultPath();
	fileName.Append(aPathNameAndExtn);
	iSourceFilename = fileName ;
	}

void CTestStepImageTransform::SetDestFilenameL(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName = Suite()->DefaultPath();
	
	_LIT(KOutputFolder, "output\\");
	fileName.Append(KOutputFolder);
	//create the output subfolder
	TInt err = iFs.MkDir(fileName);
	if (err != KErrNone && err != KErrAlreadyExists)
		{
		INFO_PRINTF2(_L("Unable to create output folder, err = %d"), err);
		User::Leave(err);
		}
	
	fileName.Append(aPathNameAndExtn);
	iDestFilename = fileName;

	}

// delete the destination file
void CTestStepImageTransform::DeleteDestFilenameL()
	{
	TInt err = iFs.Delete(DestFilename());
	if (err != KErrNone && err != KErrNotFound)
		{
		INFO_PRINTF2(_L("Unable to delete output file, err = %d"), err);
		User::Leave(err);
		}
	}


void CTestStepImageTransform::SetRefFilenameL(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName = Suite()->DefaultPath();
	_LIT(KRefFolder, "ref\\");
	fileName.Append(KRefFolder);
	fileName.Append(aPathNameAndExtn);
	iRefFilename = fileName ;
	}


/**
*
* CTestStepImageTransform SourceFilename
*
* 
* 
*/
TFileName& CTestStepImageTransform::SourceFilename() 
	{
	return  iSourceFilename;
	}

/**
*
* CTestStepImageTransform DestFilename
*
* 
* 
*/
TFileName& CTestStepImageTransform::DestFilename() 
	{
	return  iDestFilename;
	}

/**
*
* CTestStepImageTransform RefFilename
*
* 
* 
*/
TFileName& CTestStepImageTransform::RefFilename() 
	{
	return  iRefFilename;
	}

/**
 *
 * Compare two binary files
 *
 * @param "const TDesC& aFile1"
 *		  The first file to compare
 * @param "const TDesC& aFile2"
 *		  The second file to compare
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading either file 	
 * @return "TVerdict"
 *		  EPass if the files are identical
 *		  EFail otherwise
 * @xxxx
 *
 */
TVerdict CTestStepImageTransform::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2) const
	{
	RFile file1, file2;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	User::LeaveIfError(file1.Open(fs,aFile1,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file1);

	User::LeaveIfError(file2.Open(fs,aFile2,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file2);

	TInt fileSize1, fileSize2;

	TVerdict result = EPass;
	TInt blockSize = 1024;
	
	User::LeaveIfError(file1.Size(fileSize1));
	User::LeaveIfError(file2.Size(fileSize2));

	if (fileSize1 == fileSize2) 
		{
		HBufC8* buf1 = HBufC8::NewMaxLC(blockSize);
		TPtr8 buf1Ptr(buf1->Des());
		HBufC8* buf2 = HBufC8::NewMaxLC(blockSize);
		TPtr8 buf2Ptr(buf2->Des());

		TInt length = 0;

		while (length<fileSize1 && result == EPass)
			{
			TInt copySize;

			if (fileSize1 - length > blockSize)
				copySize = blockSize;
			else
				copySize = fileSize1 - length;
			User::LeaveIfError(file1.Read(buf1Ptr,copySize));
			User::LeaveIfError(file2.Read(buf2Ptr,copySize));

			if (Mem::Compare(buf1Ptr.Ptr(),copySize,buf2Ptr.Ptr(),copySize)!=0)
				result = EFail;

			length += copySize;
			}
		CleanupStack::PopAndDestroy(2); // buf1, buf2
		}
	else
		result = EFail;

	file1.Close();
	file2.Close();
	fs.Close();

	CleanupStack::PopAndDestroy(3); //fs, file1, file2

	return result;
	}


TVerdict CTestStepImageTransform::CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const
	{
	RFile file;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	User::LeaveIfError(file.Open(fs, aFile, EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file);

	TInt fileSize;

	TVerdict result = EPass;
	
	User::LeaveIfError(file.Size(fileSize));

	if (fileSize == aDesc.Length()) 
		{
		HBufC8* buf = HBufC8::NewMaxLC(fileSize);
		TPtr8 bufPtr(buf->Des());
		User::LeaveIfError(file.Read(bufPtr));

		if (bufPtr.Compare(aDesc) != 0)
			result = EFail;
		CleanupStack::PopAndDestroy(buf);
		}
	else
		result = EFail;

	file.Close();
	fs.Close();

	CleanupStack::PopAndDestroy(2); //fs, file

	return result;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/

CTestStepTransformGeneric::CTestStepTransformGeneric(const TImageTransformUnitTestParams& aTestParams) :
	iTestParams(&aTestParams), iSourceBufPtr(NULL, 0), iMimeType(NULL, 0)
	{
	// Store the name of this test case
	iTestStepName = iTestParams->iTestName;

	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformGeneric::~CTestStepTransformGeneric()
	{
	}

void CTestStepTransformGeneric::Cleanup()
	{
    RFbsSession::Disconnect();
	delete iSourceBuf; iSourceBuf = NULL;
	delete iDestBuf; iDestBuf = NULL;
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformGeneric::DoTestStepL()
	{
	INFO_PRINTF2(_L("%S"), &iTestStepName);
	INFO_PRINTF2(_L("%s"), iTestParams->iTestDescription);

	TVerdict verdict = EFail;

	if (iTestParams->iTestType == EAlloc)
		{
		INFO_PRINTF1(_L("Alloc test"));
		TInt i;
		TInt err;
		for (i = 1 ; ; i++)
			{
			__MM_HEAP_MARK;
			
			if (i % 5 == 0)
				INFO_PRINTF2(_L("Fail count = %d"), i);

			__UHEAP_SETFAIL(RHeap::EFailNext, i);

			TRAP(err, verdict = DoTestStepFbsL());

			TAny* testAlloc = User::Alloc(1);
			TBool heapTestingComplete = (testAlloc == NULL);
			User::Free(testAlloc);

			Cleanup();

			__UHEAP_RESET;
			__MM_HEAP_MARKEND;

			if ((err != KErrNoMemory) && heapTestingComplete)
				{
				INFO_PRINTF4(_L("err = %d, verdict = %d, Fail count = %d"), err, verdict, i);
				INFO_PRINTF1(_L("Alloc testing completed successfully"));
				break;
				}
			}
		}
	else
		{
	    
	         User::LeaveIfError(FbsStartup());
	         TInt errCode = RFbsSession::Connect();
	         if( errCode != KErrNone)
	             {
	             User::LeaveIfError(errCode);
	             }
		verdict = DoTestStepFbsL();
		}

	return verdict;
	}

// dummy Hal call for heap balance on target
// Need to do this BEFORE doing any allocation testing !	
void CTestStepTransformGeneric::DummyHalCallL()
	{
	TInt err = KErrNone;
	TInt displayMode = EColor256;
	CFbsScreenDevice* screenDevice = NULL;	
	while(displayMode < EColorLast) 
		{
		TRAP(err, screenDevice = CFbsScreenDevice::NewL(_L("NotUsed"),(TDisplayMode)displayMode));
		delete screenDevice;
		screenDevice = NULL;	
		if(err == KErrNone)
			{
			break;
			}
		displayMode++;
		}			
	User::LeaveIfError(err);
	}	

TVerdict CTestStepTransformGeneric::DoTestStepFbsL()
	{
	CICLFbsSessionTest* tempFbsSession = CICLFbsSessionTest::NewLC();
  	DummyHalCallL();
	CleanupStack::PopAndDestroy(tempFbsSession);

	__MM_HEAP_MARK;

	// [ connect to the bitmap server and check the error code]
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	TVerdict verdict = TestL();

	// disconnect from the bitmap server
	CleanupStack::PopAndDestroy(fbsSession);
	
	__MM_HEAP_MARKEND;

	return verdict;

	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformGeneric::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	
	TBool continueOnToNextStep;
	do
		{
		// Define the source, destination & ref image file names
		SetFilenamesL(TPtrC(iTestParams->iInputFilename));

		ConfigureL(*imageTransform);

		if (iTestParams->iTestType != EAlloc)
			INFO_PRINTF1(_L("Calling SetupL()"));
		imageTransform->SetupL();
		
		listener->InitialiseActiveListener();
		if (iTestParams->iTestType != EAlloc)
			INFO_PRINTF1(_L("Calling Transform()"));
		imageTransform->Transform(listener->iStatus);

		CActiveScheduler::Start(); 
		testStepResult = CheckResult(listener->iStatus.Int());

		// delete all objects we've created (for heap testing)
		Cleanup();

		if (testStepResult != EPass)
			break;
		
		continueOnToNextStep = iTestParams->iContinueOnToNextStep;
		if (continueOnToNextStep)
			{
			iTestParamsPrev = iTestParams;
			iTestParams++;
			}
		}
	while (continueOnToNextStep);

	CleanupStack::PopAndDestroy(listener); 
	listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); 
	imageTransform = NULL;

	return testStepResult;
	}


void CTestStepTransformGeneric::SetFilenamesL(const TDesC& aSourceFilename)
	{
	// Define the source, destination & ref image file names
	// construct the output & reference filename from the source 
	// filename by appending the size of the dest image to it
	SetSourceFilenameL(aSourceFilename);
	TParse parse;
	User::LeaveIfError(parse.Set(aSourceFilename, NULL, NULL));
	TFileName outputFilename;
	outputFilename.Append(parse.Drive());
	outputFilename.Append(parse.Name());
	TBuf<20> suffix;
	suffix.Format(_L("_%dx%d"), iTestParams->iDestWidth, iTestParams->iDestHeight);
	outputFilename.Append(suffix);

	if (iTestParams->iMaintainAspectRatio)
	outputFilename.Append(_L("_m"));

	outputFilename.Append(parse.Ext());
	SetDestFilenameL(outputFilename);
	DeleteDestFilenameL();
	SetRefFilenameL(outputFilename);
	}

void CTestStepTransformGeneric::ConfigureL(CImageTransform& aImageTransform)
	{

	if (!iTestParams->iSourceIsFilename)
		{
		RFile file;
		// read the source file into a descriptor
		User::LeaveIfError(file.Open(iFs, SourceFilename(), EFileShareAny|EFileStream|EFileRead));
		CleanupClosePushL(file);

		TInt fileSize;
		User::LeaveIfError(file.Size(fileSize));

		delete iSourceBuf; iSourceBuf = NULL;
		iSourceBuf = HBufC8::NewMaxL(fileSize);
		iSourceBufPtr.Set(iSourceBuf->Des());
		User::LeaveIfError(file.Read(iSourceBufPtr));

		CleanupStack::PopAndDestroy(1);	// file
		}

	// if we're re-using the API (iTestParamsPrev != NULL),
	// only set the parameters that have been changed
	TBool sourceChanged = 
		(iTestParamsPrev == NULL || 
		iTestParamsPrev->iInputFilename != iTestParams->iInputFilename ||
		iTestParamsPrev->iSourceIsFilename != iTestParams->iSourceIsFilename)
		?ETrue:EFalse;
	TBool destChanged = 
		(iTestParamsPrev == NULL || 
		iTestParamsPrev->iInputFilename != iTestParams->iInputFilename ||
		iTestParamsPrev->iDestIsFilename != iTestParams->iDestIsFilename)
		?ETrue:EFalse;
	TBool destSizeChanged = 
		(iTestParamsPrev == NULL || 
		iTestParamsPrev->iDestWidth != iTestParams->iDestWidth ||
		iTestParamsPrev->iDestHeight != iTestParams->iDestHeight)
		?ETrue:EFalse;
	TBool sourceMimeTypeChanged = 
		(iTestParamsPrev == NULL || 
		iTestParamsPrev->iMimeType != iTestParams->iMimeType)
		?ETrue:EFalse;
	TBool sourceImageTypeChanged = 
		(iTestParamsPrev == NULL || 
		iTestParamsPrev->iImageType != iTestParams->iImageType ||
		iTestParamsPrev->iImageSubType != iTestParams->iImageSubType)
		?ETrue:EFalse;
	TBool pluginUidChanged = 
		(iTestParamsPrev == NULL || 
		iTestParamsPrev->iPluginUid != iTestParams->iPluginUid)
		?ETrue:EFalse;

	if (iTestParams->iPluginUid != KNullUid.iUid && pluginUidChanged)
		{
		if (iTestParams->iTestType != EAlloc)
			INFO_PRINTF2(_L("Calling SetPluginUid(%08X)"), iTestParams->iPluginUid);
		aImageTransform.SetPluginUidL(TUid::Uid(iTestParams->iPluginUid));
		}

	if (sourceChanged)
		{
		if (iTestParams->iSourceIsFilename)
			{
			if (iTestParams->iTestType != EAlloc)
				INFO_PRINTF2(_L("Calling SetSourceFilenameL(%S)"), &SourceFilename());
			aImageTransform.SetSourceFilenameL(SourceFilename());
			}
		else
			{
			if (iTestParams->iTestType != EAlloc)
				INFO_PRINTF1(_L("Calling SetSourceDataL()"));
			aImageTransform.SetSourceDataL(iSourceBufPtr);
			}
		}
	
	if (iTestParams->iDestIsFilename)
		{
		if (destChanged || destSizeChanged)
			{
			if (iTestParams->iTestType != EAlloc)
				INFO_PRINTF2(_L("Calling SetDestFilenameL(%S)"), &DestFilename());
			aImageTransform.SetDestFilenameL(DestFilename());
			}
		}
	else
		{
		if (destChanged)
			{
			if (iTestParams->iTestType != EAlloc)
				INFO_PRINTF1(_L("Calling SetDestDataL()"));
			aImageTransform.SetDestDataL(iDestBuf);
			}
		}

	if (destSizeChanged)
		{
		if (iTestParams->iTestType != EAlloc)
			INFO_PRINTF4(_L("Calling SetDestSizeInPixelsL((%d,%d), %d)"), iTestParams->iDestWidth, iTestParams->iDestHeight, iTestParams->iMaintainAspectRatio);
		aImageTransform.SetDestSizeInPixelsL(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
		}

	if (iTestParams->iMimeType != NULL && sourceMimeTypeChanged)
		{
		TBuf16<KMaxMimeLength> mimeType;
		iMimeType.Set(iTestParams->iMimeType);
		mimeType.Copy(iMimeType);
		if (iTestParams->iTestType != EAlloc)
			INFO_PRINTF2(_L("Calling SetSourceMimeTypeL(%S)"), &mimeType);
		aImageTransform.SetSourceMimeTypeL(iMimeType);
		}
	else if ((iTestParams->iImageType != KNullUid.iUid || iTestParams->iImageSubType != KNullUid.iUid)
		&& sourceImageTypeChanged)
		{
		if (iTestParams->iTestType != EAlloc)
			INFO_PRINTF3(_L("Calling SetSourceImageTypeL(%08X, %08X)"), iTestParams->iImageType, iTestParams->iImageSubType);
		aImageTransform.SetSourceImageTypeL(TUid::Uid(iTestParams->iImageType), TUid::Uid(iTestParams->iImageSubType));
		}

	}

TVerdict CTestStepTransformGeneric::CheckResult(TInt aListenerError)
	{

	if (iTestParams->iTestType != EAlloc || aListenerError != KErrNoMemory)
		{
		INFO_PRINTF1(_L("Checking result..."));
		}

	TVerdict verdict = EPass;

	if (aListenerError != KErrNone)
		{
		if (iTestParams->iTestType != EAlloc || aListenerError != KErrNoMemory)
			INFO_PRINTF2(_L("Transform failed with error %d"), aListenerError);
		verdict = EFail;
		}
	else
		{
		TInt err;
		if (iTestParams->iDestIsFilename)
			{
			TRAP(err, verdict = CompareFilesL(DestFilename(), RefFilename()));
			}
		else
			{
			TRAP(err, verdict = CompareDescWithFileL(*iDestBuf, RefFilename()));
			}

		if (err != KErrNone || verdict != EPass)
			{
			if (iTestParams->iTestType != EAlloc || err != KErrNoMemory)
				{
				INFO_PRINTF2(_L("Transformed image does not match reference image (err = %d)"), 
					err);
				INFO_PRINTF3(_L("(%S != %S)"), &DestFilename(), &RefFilename());
				}
			verdict = EFail;
			}
		if (err == KErrNone && verdict == EPass)
			{
			INFO_PRINTF1(_L("Transformed image matches reference image"));
			INFO_PRINTF3(_L("(%S == %S)"), &DestFilename(), &RefFilename());
			}
		}
	return verdict;
	}

/****************************************************************/
/* Specific tests												*/
/****************************************************************/
/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformSpecific1::CTestStepTransformSpecific1(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformSpecific1::~CTestStepTransformSpecific1()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformSpecific1::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetSourceRect()"));
	imageTransform->SetSourceRect(TRect(TPoint(10, 10), TPoint(50, 50)));

	INFO_PRINTF1(_L("Calling SetupL()"));
	imageTransform->SetupL();

	listener->InitialiseActiveListener();
	INFO_PRINTF1(_L("Calling Transform()"));
	imageTransform->Transform(listener->iStatus);

	CActiveScheduler::Start(); 
	testStepResult = CheckResult(listener->iStatus.Int());

	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformSpecific2::CTestStepTransformSpecific2(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformSpecific2::~CTestStepTransformSpecific2()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformSpecific2::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetupL()"));
	imageTransform->SetupL();

	//Get Uid
	TUid pluginExtUid = imageTransform->Extension()->Uid();
	if (pluginExtUid != KUidTestTransformerExt)
		testStepResult = EFail;

	//Get extension
	INFO_PRINTF1(_L("Calling Extension()"));
	CTestTransformerExtension* extension = static_cast<CTestTransformerExtension*>(imageTransform->Extension());

	//compare intial preserveImageData value
	INFO_PRINTF1(_L("Calling PreserveImageData()"));
	TBool preserveImageData = extension->PreserveImageData();
	if (preserveImageData)
		testStepResult = EFail;

	//set new value	
	INFO_PRINTF1(_L("Calling SetPreserveImageData()"));
	imageTransform->SetPreserveImageData(ETrue);

	INFO_PRINTF1(_L("Calling SetupL()"));
	imageTransform->SetupL();

	//reset the extension
	extension = static_cast<CTestTransformerExtension*>(imageTransform->Extension());

	//check new value
	INFO_PRINTF1(_L("Calling PreserveImageData()"));
	preserveImageData = extension->PreserveImageData();
	if (!preserveImageData)
		testStepResult = EFail;	

	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformSpecific3::CTestStepTransformSpecific3(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformSpecific3::~CTestStepTransformSpecific3()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformSpecific3::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	const TUint invalidOption = ~TUint(CImageTransform::EOptionNone);

	TRAPD(err, imageTransform->SetOptionsL(invalidOption)) 
	if (err != KErrArgument)
		{
		testStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(imageTransform);

	return testStepResult;
	}

/****************************************************************/
/* Cancel tests													*/
/****************************************************************/

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformCancel1::CTestStepTransformCancel1(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformCancel1::~CTestStepTransformCancel1()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformCancel1::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetupL()"));
	imageTransform->SetupL();

	listener->InitialiseActiveListener();
	INFO_PRINTF1(_L("Calling Transform()"));
	imageTransform->Transform(listener->iStatus);

	INFO_PRINTF1(_L("Cancelling Transform()"));
	listener->Cancel();

	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformCancel2::CTestStepTransformCancel2(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformCancel2::~CTestStepTransformCancel2()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformCancel2::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetupL()"));
	imageTransform->SetupL();

	listener->InitialiseActiveListener();
	INFO_PRINTF1(_L("Calling Transform()"));
	imageTransform->Transform(listener->iStatus);

	INFO_PRINTF1(_L("deleting CImageTransform before transform has completed()"));
	CleanupStack::PopAndDestroy(listener); listener = NULL;
	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	// delete all objects we've created (for heap testing)
	Cleanup();

	return testStepResult;
	}



/****************************************************************/
/* Negative tests												*/
/****************************************************************/

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformNegative1::CTestStepTransformNegative1(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformNegative1::~CTestStepTransformNegative1()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformNegative1::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetupL()"));
	TRAPD(err, imageTransform->SetupL());

	if (err == KErrNotFound)
		{
		INFO_PRINTF2(_L("SetupL() left with expected error %d"), err);
		testStepResult = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("SetupL() had an unexpected error %d"), err);
		testStepResult = EFail;
		}
	
	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformNegative6::CTestStepTransformNegative6(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformNegative6::~CTestStepTransformNegative6()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformNegative6::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// call SetSourceImageTypeL(0, KImageSubTypeSilly)
	// should leave with KErrArgument
	INFO_PRINTF3(_L("Calling SetSourceImageTypeL(%08X, %08X)"), iTestParams->iImageType, iTestParams->iImageSubType);
	TRAPD(err, imageTransform->SetSourceImageTypeL(TUid::Uid(iTestParams->iImageType), TUid::Uid(iTestParams->iImageSubType)));
	if (err == KErrArgument)
		{
		INFO_PRINTF2(_L("SetSourceImageTypeL() left with expected error %d"), err);
		testStepResult = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("SetSourceImageTypeL() had an unexpected error %d"), err);
		testStepResult = EFail;
		}
	

	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}



/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformNegative7::CTestStepTransformNegative7(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformNegative7::~CTestStepTransformNegative7()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformNegative7::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	INFO_PRINTF2(_L("Calling SetSourceFilenameL(%S)"), &SourceFilename());
	TRAPD(err, imageTransform->SetSourceFilenameL(SourceFilename()));

	if (err == KErrNotFound)
		{
		INFO_PRINTF2(_L("SetupL() left with expected error %d"), err);
		testStepResult = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("SetupL() had an unexpected error %d"), err);
		testStepResult = EFail;
		}
	
	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformNegative8::CTestStepTransformNegative8(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformNegative8::~CTestStepTransformNegative8()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformNegative8::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetupL()"));
	TRAPD(err, imageTransform->SetupL());

	if (err == KErrUnderflow)
		{
		INFO_PRINTF2(_L("SetupL() left with expected error %d"), err);
		testStepResult = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("SetupL() had an unexpected error %d"), err);
		testStepResult = EFail;
		}
	
	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}



/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformNegative9::CTestStepTransformNegative9(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformNegative9::~CTestStepTransformNegative9()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformNegative9::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;
	TInt err;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);


	// call SetSourceRect()
	// this should result in SetupL() leaving...
	INFO_PRINTF1(_L("Calling SetSourceRect()"));
	imageTransform->SetSourceRect(TRect(TPoint(10, 10), TPoint(50, 50)));

	INFO_PRINTF1(_L("Calling SetupL()"));
	TRAP(err, imageTransform->SetupL());
	if (err == KErrNotFound)
		{
		INFO_PRINTF2(_L("SetupL() left with expected error %d"), err);
		}
	else
		{
		INFO_PRINTF2(_L("SetupL() had an unexpected error %d"), err);
		testStepResult = EFail;
		}

	// now try calling ClearSourceRect() and calling SetupL()
	// this should now instantiate the plugin 
	INFO_PRINTF1(_L("Calling ClearSourceRect()"));
	imageTransform->ClearSourceRect();

	INFO_PRINTF1(_L("Calling SetupL()"));
	TRAP(err, imageTransform->SetupL());
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("SetupL() had an unexpected error %d"), err);
		testStepResult = EFail;
		}
	
	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}




/****************************************************************/
/* Panic tests													*/
/****************************************************************/

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformPanic1::CTestStepTransformPanic1(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformPanic1::~CTestStepTransformPanic1()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformPanic1::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	// call Transform() without having previously called SetupL()
	// should panic with EIllegalCallSequence
	listener->InitialiseActiveListener();
	INFO_PRINTF1(_L("Calling Transform()"));
	imageTransform->Transform(listener->iStatus);

	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformPanic2::CTestStepTransformPanic2(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformPanic2::~CTestStepTransformPanic2()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformPanic2::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);

	INFO_PRINTF1(_L("Calling SetupL()"));
	imageTransform->SetupL();

	// call Transform() twice
	// should panic 
	listener->InitialiseActiveListener();
	INFO_PRINTF1(_L("Calling Transform()"));
	imageTransform->Transform(listener->iStatus);
	INFO_PRINTF1(_L("Calling Transform() again"));
	imageTransform->Transform(listener->iStatus);

	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/
CTestStepTransformPanic4::CTestStepTransformPanic4(const TImageTransformUnitTestParams& aTestParams)
	: CTestStepTransformGeneric(aTestParams)
	{
	}

/**
*
* Test step destructor.
* 
*/
CTestStepTransformPanic4::~CTestStepTransformPanic4()
	{
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepTransformPanic4::TestL()
	{
	// assume failure
	TVerdict testStepResult = EFail;

	// create an ImageTransform object
	CImageTransform* imageTransform = CImageTransform::NewL(iFs);
	CleanupStack::PushL(imageTransform);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener(*imageTransform);
	CleanupStack::PushL(listener);
	

	// Define the source, destination & ref image file names
	SetFilenamesL(TPtrC(iTestParams->iInputFilename));

	ConfigureL(*imageTransform);
	INFO_PRINTF1(_L("Calling Reset()"));
	imageTransform->Reset();

	// SetupL() should panic since source & destination not defined
	INFO_PRINTF1(_L("Calling SetupL()"));
	TRAPD(err, imageTransform->SetupL());
	if (err != KErrNotReady)
		{
		ERR_PRINTF3(_L("Expected imageTransform->SetupL call to fail with %d, got %d"),
				KErrNotReady, err);
		testStepResult = EFail;
		}
	else
		{
		testStepResult = EPass;
		}
	
	// delete all objects we've created (for heap testing)
	Cleanup();

	CleanupStack::PopAndDestroy(listener); listener = NULL;

	CleanupStack::PopAndDestroy(imageTransform); imageTransform = NULL;

	return testStepResult;
	}


