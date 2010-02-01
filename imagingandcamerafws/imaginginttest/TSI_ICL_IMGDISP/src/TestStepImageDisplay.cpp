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
#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>
#include "../../../../unittest/ICL/tsu_icl_imgdisp/src/TestPlugin/TestHelperIface.h"

// Test system includes
#include <testframework.h>

#include "TestStepImageDisplay.h"
#include "TestSuiteImageDisplay.h"

// Heap size for these tests.
const TInt KHeapSize = 0x100000;

_LIT(KIclPanicCategory, "TS_ImageDisplay");


GLDEF_C void Panic(TS_ImageTransformPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

	
/*
CActiveListener
*/
CActiveListener::CActiveListener(CImageDisplay& aImageDisplay) :
	CActive(CActive::EPriorityIdle), iImageDisplay(aImageDisplay)
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
	iImageDisplay.StopPlay();
	}


/**
*
* CTestStepImageDisplay constructor 
*
* 
*/
CTestStepImageDisplay::CTestStepImageDisplay() 
:  iFs(), iImageDisplaySuite(NULL), iSourceFilename(), iScheduler(NULL)
	{
	}

/**
*
* CTestStepImageDisplay destructor
*
* 
*/
CTestStepImageDisplay::~CTestStepImageDisplay()
	{
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTestSuiteImageDisplay
*/

void CTestStepImageDisplay::SetSuite(CTestSuiteImageDisplay* aPtr)
	{
	iImageDisplaySuite = aPtr;
	}


/**
DoTestStepPreambleL is the preamble function for test steps
It creates an active scheduler and installs it. This is necessary
since we are multithreading and each test step runs in a separate thread.
@returns TVerdict which is either EPass or Efail
The function can leave if there is insufficient memeory
*/
TVerdict CTestStepImageDisplay::DoTestStepPreambleL()
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

TVerdict CTestStepImageDisplay::DoTestStepPostambleL()
	{
	Cleanup();

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
* CTestStepImageDisplay Suite accessor function
* @returns CTestSuiteImageDisplay*  
*/

CTestSuiteImageDisplay* CTestStepImageDisplay::Suite()
	{
	return iImageDisplaySuite;
	}


/**
*
* CTestStepImageDisplay SetTestFilename
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&  afull file and path name
* @returns void
*/

void CTestStepImageDisplay::DefaultPath(TFileName& aName)
	{
	Suite()->DefaultPath(aName);
	}

void CTestStepImageDisplay::SetSourceFilename(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName;
	DefaultPath( fileName );
	fileName.Append(aPathNameAndExtn);
	iSourceFilename = fileName ;
	}

void CTestStepImageDisplay::SetDestFilenameL(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName;
	DefaultPath( fileName );
	
	_LIT(KOutputFolder, "output\\");
	fileName.Append(KOutputFolder);
	//create the output subfolder
	TInt err = iFs.MkDirAll(fileName);
	if (err != KErrNone && err != KErrAlreadyExists)
		{
		INFO_PRINTF2(_L("Unable to create output folder, err = %d"), err);
		User::Leave(err);
		}
	
	fileName.Append(aPathNameAndExtn);
	iDestFilename = fileName;

	}

// delete the destination file
void CTestStepImageDisplay::DeleteDestFilenameL()
	{
	TInt err = iFs.Delete(DestFilename());
	if (err != KErrNone && err != KErrNotFound)
		{
		INFO_PRINTF2(_L("Unable to delete output file, err = %d"), err);
		User::Leave(err);
		}
	}


void CTestStepImageDisplay::SetRefFilename(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName;
	DefaultPath( fileName );
	_LIT(KRefFolder, "ref\\");
	fileName.Append(KRefFolder);
	fileName.Append(aPathNameAndExtn);
	iRefFilename = fileName ;
	}


/**
*
* CTestStepImageDisplay SourceFilename
*
* 
* 
*/
TFileName& CTestStepImageDisplay::SourceFilename() 
	{
	return  iSourceFilename;
	}

/**
*
* CTestStepImageDisplay DestFilename
*
* 
* 
*/
TFileName& CTestStepImageDisplay::DestFilename() 
	{
	return  iDestFilename;
	}

/**
*
* CTestStepImageDisplay RefFilename
*
* 
* 
*/
TFileName& CTestStepImageDisplay::RefFilename() 
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
TVerdict CTestStepImageDisplay::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2) const
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


TVerdict CTestStepImageDisplay::CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const
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

CTestStepImgDisplayGeneric::CTestStepImgDisplayGeneric(const TImageDisplayUnitTestParams& aTestParams) :
	iTestParams(&aTestParams)
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
CTestStepImgDisplayGeneric::~CTestStepImgDisplayGeneric()
	{
	}

void CTestStepImgDisplayGeneric::Cleanup()
	{
	delete iImageDisplay; iImageDisplay= NULL;

	iFileHandle.Close();
	delete iSourceBuf; iSourceBuf = NULL;

	if (iFbsConnection)
		RFbsSession::Disconnect();
	iFbsConnection = EFalse;

	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepImgDisplayGeneric::DoTestStepL()
	{
	INFO_PRINTF2(_L("%S"), &iTestStepName);
	INFO_PRINTF2(_L("%s"), iTestParams->iTestDescription);

	TVerdict verdict = EFail;

	if (iTestParams->iTestType == TImageDisplayUnitTestParams::EAlloc)
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

			if ((err != KErrNoMemory) || heapTestingComplete)
				{
				INFO_PRINTF4(_L("err = %d, verdict = %d, Fail count = %d"), err, verdict, i);
				INFO_PRINTF1(_L("Alloc testing completed successfully"));
				break;
				}
			}
		}
	else
		{
		verdict = DoTestStepFbsL();
		}

	return verdict;
	}

TVerdict CTestStepImgDisplayGeneric::DoTestStepFbsL()
	{

	// [ connect to the bitmap server and check the error code]
	User::LeaveIfError(RFbsSession::Connect());
	iFbsConnection = ETrue;

	TVerdict verdict = TestL();


	return verdict;

	}

TInt CTestStepImgDisplayGeneric::CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile) const
	{
	CFbsBitmap* refBitmap=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(refBitmap);
	User::LeaveIfError( refBitmap->Load(aFile, 0) );

	if (aBitmap.DisplayMode() != refBitmap->DisplayMode() || aBitmap.SizeInPixels() != refBitmap->SizeInPixels())
		{
		CleanupStack::PopAndDestroy(refBitmap);
		return EFail;
		}
	const TInt srcScanlineLen=CFbsBitmap::ScanLineLength(refBitmap->SizeInPixels().iWidth, refBitmap->DisplayMode());
	const TInt frameSize=srcScanlineLen*refBitmap->SizeInPixels().iHeight;
	HBufC8* srcFrameBuffer=HBufC8::NewMaxLC(frameSize);

	refBitmap->LockHeap();
	Mem::Copy((TAny*)srcFrameBuffer->Des().Ptr(), refBitmap->DataAddress(), frameSize);
	refBitmap->UnlockHeap();
	aBitmap.LockHeap();
	TInt result = KErrGeneral;
	if (Mem::Compare(reinterpret_cast<const TUint16*>(aBitmap.DataAddress()), frameSize>>1, 
						reinterpret_cast<const TUint16*>(srcFrameBuffer->Ptr()), frameSize>>1) == 0)
		{
		result = KErrNone;
		}
	
	aBitmap.UnlockHeap();

	CleanupStack::PopAndDestroy(srcFrameBuffer);
	CleanupStack::PopAndDestroy(refBitmap); 

	return result;
	}

void CTestStepImgDisplayGeneric::SetImageSourceL()
	{
	ASSERT(iImageDisplay);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	switch (iTestParams->iSourceType)
		{
		case TImageDisplayUnitTestParams::ETypeFileName:
			{
			iImageDisplay->SetImageSource( TMMFileSource(SourceFilename()));
			}
			break;
		case TImageDisplayUnitTestParams::ETypeFileHandle:
			{
			iFileHandle.Close();
			User::LeaveIfError(iFs.ShareProtected());
			User::LeaveIfError( iFileHandle.Open(iFs, SourceFilename(), EFileRead | EFileShareExclusive/*EFileShareAny*/) );
			iImageDisplay->SetImageSource( TMMFileHandleSource(iFileHandle) );
			}
			break;
		case TImageDisplayUnitTestParams::ETypeData:
			{
			User::LeaveIfError( iFileHandle.Open(iFs, SourceFilename(), EFileRead | EFileShareReadersOnly) );
			TInt length;
			User::LeaveIfError( iFileHandle.Size(length) );
			delete iSourceBuf; iSourceBuf = NULL;
			iSourceBuf = HBufC8::NewL(length);
			TPtr8 buf(iSourceBuf->Des());
			User::LeaveIfError( iFileHandle.Read(buf) );
			iFileHandle.Close();
			iImageDisplay->SetImageSource( TDescriptorDataSource(*iSourceBuf) );
			}
			break;
		default:
			ASSERT(FALSE);
		}
	}

TBool CTestStepImgDisplayGeneric::ConsumeFrame(const CFbsBitmap& /*aBitmap*/)
	{
	++iFrameNumber;
	return ETrue;
	}

void CTestStepImgDisplayGeneric::MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& /*aUpdatedArea*/, TInt aError)
	{
	INFO_PRINTF3(_L("ImgReadCb Status=%d Error=%d"), aStatus, aError);
	iLastError = aError;
	TBool stopped=EFalse;
	if (KErrNone != iLastError || (aStatus & CImageDisplayPlugin::EStatusNoMoreToDecode))
		{
		CActiveScheduler::Stop();
		stopped=ETrue;
		}
		
	if ((aStatus & CImageDisplayPlugin::EStatusFrameReady) && (KErrNone == iLastError))
		{
		TBool ContinueDecoding=ConsumeFrame(*aBitmap);
		if (ContinueDecoding && iLastError == KErrNone)
			{
			iImageDisplay->Play();
			}
		else
			{
			if (!stopped)
				{
				CActiveScheduler::Stop();	
				}
			}
		}
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepImgDisplayGeneric::TestL()
	{
	// assume success
	TVerdict testStepResult = EPass;

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	
	// "normal" operation using the mimetype
	SetupPluginL();
	//
	// try to "reuse" framework
	//
	// PluginLoaded -> Configured
	iImageDisplay->Reset();
	SetupPluginL();
	iImageDisplay->Reset();
	
	TBuf8<1> dummyBuffer;
	iImageDisplay->SetImageSource( TDescriptorDataSource(dummyBuffer) );
	// Configured -> Opened
	iImageDisplay->Reset();
	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);
	iImageDisplay->SetupL();
	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		return (testStepResult = EFail );
		}
	// try PluginLoaded -> (Processing <-> Paused) -> PluginLoaded two times
	for(TInt i=2; i && KErrNone == iLastError; --i)
		{
		// start decoding
		iImageDisplay->Play();
		CActiveScheduler::Start();
		}
	if (KErrNone != iLastError && 
			!(iTestParams->iTestType==TImageDisplayUnitTestParams::EAlloc && KErrNoMemory == iLastError))
		testStepResult = EFail;
	iImageDisplay->StopPlay();
	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

void CTestStepImgDisplayGeneric::SetupPluginL()
	{
	if (0 != iTestParams->iPluginUid)
		iImageDisplay->SetPluginUid(TUid::Uid(iTestParams->iPluginUid));

	if (-1 != iTestParams->iDestWidth)
		iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), 
											iTestParams->iMaintainAspectRatio);

	if (-1 != iTestParams->iDisplayMode)
		iImageDisplay->SetDisplayMode(TDisplayMode(iTestParams->iDisplayMode));

	if (-1 != iTestParams->iImageOptions)
		iImageDisplay->SetOptions(iTestParams->iImageOptions);
	
	if (NULL != iTestParams->iMimeType)
		iImageDisplay->SetSourceMimeType(TPtrC8(iTestParams->iMimeType));

	if (iTestParams->iImageType || iTestParams->iImageSubType)
		iImageDisplay->SetSourceImageType(TUid::Uid(iTestParams->iImageType ), TUid::Uid(iTestParams->iImageSubType));
	
	if (TImageDisplayUnitTestParams::ETypeUndefined != iTestParams->iSourceType)
		SetImageSourceL();
	}

void CTestStepImgDisplayGeneric::SetFilenamesL(const TDesC& aSourceFilename)
	{
	// Define the source, destination & ref image file names
	// construct the output & reference filename from the source 
	// filename by appending the size of the dest image to it
	SetSourceFilename(aSourceFilename);
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
	SetRefFilename(outputFilename);
	}

TBool CTestStepDecode2Frames::ConsumeFrame(const CFbsBitmap& aBitmap)
	{
	if (++iFrameNumber <3)
		{
		TBuf<32> fileName ;
		fileName.AppendFormat(_L("frame_%d.mbm"),iFrameNumber);
		SetDestFilenameL(fileName);
		iLastError = const_cast<CFbsBitmap&>(aBitmap).Save(DestFilename());
		if (KErrNone == iLastError)
			return ETrue;
		}
	return EFalse;
	}

TVerdict CTestStepDecode2Frames::TestL()
	{
	TVerdict testStepResult = EPass;
// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	SetupPluginL();

	iImageDisplay->SetupL();

	SetRefFilename(_L("frame_2.mbm"));
	for(TInt i=2; i && KErrNone == iLastError; --i)
		{
		// start decoding
		iImageDisplay->Play();
		CActiveScheduler::Start();
		}
	TVerdict res=EFail;
	TRAPD(err, res=CompareFilesL(RefFilename(), DestFilename()));
	(void)(err=err+1);
	if (KErrNone != iLastError)
		{
		if (!(iTestParams->iTestType==TImageDisplayUnitTestParams::EAlloc && KErrNoMemory == iLastError))
			{
			testStepResult = EFail;
			}
		}
	else
		{
		testStepResult = res;
		}
		
	return testStepResult;
	}


TVerdict CTestStepPanic_1::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	iImageDisplay->Play();
	return EFail;
	}

TVerdict CTestStepPanic_2::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);	
	SetupPluginL();
	iImageDisplay->SetupL();
	return EFail;
	}

TVerdict CTestStepPanic_3::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);	
	SetupPluginL();
	iImageDisplay->SetupL();
	iImageDisplay->Play();
	return EFail;
	}

TVerdict CTestStepPanic_4::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	MTestHelper* ExtIFace;
	if (KErrNone != iImageDisplay->ExtensionInterface(TUid::Uid(MTestHelper::KIFaceUidValue), 
														*reinterpret_cast<TAny**>(&ExtIFace))
		)
		return EFail;
	ExtIFace->TriggerPanic_4();
	return EFail;
	}

TVerdict CTestStepPanic_6::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);	
	SetupPluginL();
	iImageDisplay->SetupL();
	iImageDisplay->Play();
	return EFail;
	}
