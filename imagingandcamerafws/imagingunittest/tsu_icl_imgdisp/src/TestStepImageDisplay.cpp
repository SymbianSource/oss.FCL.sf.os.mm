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
#include "TestHelperIface.h"

// Test system includes
#include <testframework.h>
#include <imageconversion.h>

#include "TestStepImageDisplay.h"
#include "TestSuiteImageDisplay.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <iclrecognizerutil.h>
#endif
// Heap size for these tests.
const TInt KHeapSize = 0x100000;

_LIT(KIclPanicCategory, "TS_ImageDisplay");


GLDEF_C void Panic(TS_ImageTransformPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
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

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError( RFbsSession::Connect() );
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


void CTestStepImageDisplay::SetRefFilenameL(const TDesC& aPathNameAndExtn)
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
 * Compare two bitmap files
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
TVerdict CTestStepImageDisplay::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2) 
	{
	
	CFbsBitmap* refBitmap=new (ELeave) CFbsBitmap();
   	CleanupStack::PushL(refBitmap);
   	User::LeaveIfError( refBitmap->Load(aFile1, 0) );
   	
   	TVerdict result=CompareBitmapWithFileL(*refBitmap, aFile2);
   	
	if (result != EPass)
		{
		INFO_PRINTF3(_L("Files do not match %S %S"), &aFile1, &aFile2);
		}

	CleanupStack::PopAndDestroy(refBitmap);

	return result;
	}

TVerdict CTestStepImageDisplay::CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile) const
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
   
	HBufC8* srcScanline = HBufC8::NewMaxLC(srcScanlineLen);
	TPtr8 src(srcScanline->Des());
	HBufC8* refScanline = HBufC8::NewMaxLC(srcScanlineLen);
	TPtr8 ref(refScanline->Des());   

	const TUint8* pSrc=src.Ptr();
	const TUint8* pRef=ref.Ptr();

	TVerdict result = EPass;
	for (TInt y=aBitmap.SizeInPixels().iHeight; y; --y)
		{
		const TPoint pnt(0,y-1);
		aBitmap.GetScanLine(src,pnt,srcScanlineLen, aBitmap.DisplayMode());
		refBitmap->GetScanLine(ref,pnt,srcScanlineLen, aBitmap.DisplayMode());   
		if (Mem::Compare(pSrc,src.Length(), pRef, src.Length()) !=0)
		 	{
		 	result = EFail;
		 	break;
		 	}
		}
   
   CleanupStack::PopAndDestroy(3,refBitmap); 

   return result;
   }


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/

CTestStepImgDisplayGeneric::CTestStepImgDisplayGeneric(const TImageDisplayUnitTestParams& aTestParams) :
	iTestParams(&aTestParams),
	iCurrentUId(aTestParams.iUniqueId)
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
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	iFbsConnection = ETrue;

	TVerdict verdict = TestL();


	return verdict;

	}

void CTestStepImgDisplayGeneric::SetImageSourceL()
	{
	ASSERT(iImageDisplay);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	
	TPtrC uniqueId(ContentAccess::KDefaultContentObject);
	if (iCurrentUId && *iCurrentUId)
		{
		uniqueId.Set(TPtrC(*iCurrentUId));
		}      
		
	switch (iTestParams->iSourceType)
		{
		case TImageDisplayUnitTestParams::ETypeFileName:
			{
			if (iTestParams->iUniqueId==NULL && iTestParams->iIntent==TImageDisplayUnitTestParams::KNoIntent)
				{
				iImageDisplay->SetImageSource(TMMFileSource(SourceFilename()));
				}
			else if (iTestParams->iIntent==TImageDisplayUnitTestParams::KNoIntent)
				{
				iImageDisplay->SetImageSource(TMMFileSource(SourceFilename(), uniqueId));
				}
			else
				{
				iImageDisplay->SetImageSource(TMMFileSource(SourceFilename(), uniqueId, iTestParams->iIntent));
				}      			
			}
			break;
		case TImageDisplayUnitTestParams::ETypeFileHandle:
			{
			iFileHandle.Close();
			User::LeaveIfError(iFs.ShareProtected());
			User::LeaveIfError( iFileHandle.Open(iFs, SourceFilename(), EFileRead | EFileShareExclusive) );

			if (iTestParams->iUniqueId==NULL && iTestParams->iIntent==TImageDisplayUnitTestParams::KNoIntent)
				{
				iImageDisplay->SetImageSource(TMMFileHandleSource(iFileHandle));
				}
			else if (iTestParams->iIntent==TImageDisplayUnitTestParams::KNoIntent)
				{
				iImageDisplay->SetImageSource(TMMFileHandleSource(iFileHandle, uniqueId));
				}
			else
				{
				iImageDisplay->SetImageSource(TMMFileHandleSource(iFileHandle, uniqueId, iTestParams->iIntent));
				}      			
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
			iImageDisplay->SetImageSource(TDescriptorDataSource(*iSourceBuf));
			}
			break;
		default:
			ASSERT(FALSE);
		}
	}

TBool CTestStepImgDisplayGeneric::ConsumeFrame(const CFbsBitmap* /*aBitmap*/)
	{
	++iFrameNumber;
	return ETrue;
	}

void CTestStepImgDisplayGeneric::MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& /*aUpdatedArea*/, TInt aError)
	{
	INFO_PRINTF3(_L("ImgReadCb Status=%d Error=%d"), aStatus, aError);
	iLastError 			= aError;
	iLatestPlugintStatus= aStatus;
	if (KErrNone != iLastError || (aStatus & CImageDisplayPlugin::EStatusNoMoreToDecode))
		{
		CActiveScheduler::Stop();
		}
	else if (aStatus & CImageDisplayPlugin::EStatusFrameReady)
		{
		TBool ContinueDecoding=ConsumeFrame(aBitmap);
		if (ContinueDecoding && iLastError == KErrNone)
			{
			iImageDisplay->Play();
			}
		else
			{
			CActiveScheduler::Stop();	
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
	iImageDisplay->SetImageSource(TDescriptorDataSource(dummyBuffer));
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
		{
		iImageDisplay->SetPluginUid(TUid::Uid(iTestParams->iPluginUid));
		}

	if (iTestParams->iDestWidth > 0)
		{
		iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), 
											iTestParams->iMaintainAspectRatio);		
		}

	if (-1 != iTestParams->iDisplayMode)
		{
		iImageDisplay->SetDisplayMode(TDisplayMode(iTestParams->iDisplayMode));
		}

	if (-1 != iTestParams->iImageOptions)
		{
		iImageDisplay->SetOptions(iTestParams->iImageOptions);
		}
	
	if (NULL != iTestParams->iMimeType)
		{
		iImageDisplay->SetSourceMimeType(TPtrC8(iTestParams->iMimeType));
		}

	if (iTestParams->iImageType || iTestParams->iImageSubType)
		{
		iImageDisplay->SetSourceImageType(TUid::Uid(iTestParams->iImageType ), TUid::Uid(iTestParams->iImageSubType));
		}
	
	if (TImageDisplayUnitTestParams::ETypeUndefined != iTestParams->iSourceType)
		{
		SetImageSourceL();
		}
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
	SetRefFilenameL(outputFilename);
	}

TBool CTestStepDecode2Frames::ConsumeFrame(const CFbsBitmap* aBitmap)
	{
	if (++iFrameNumber <3)
		{
		TBuf<32> fileName ;
		fileName.AppendFormat(_L("frame_%d.mbm"),iFrameNumber);
		SetDestFilenameL(fileName);
		iLastError = const_cast<CFbsBitmap*>(aBitmap)->Save(DestFilename());
		if (KErrNone == iLastError)
			{
			return ETrue;
			}
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

	SetRefFilenameL(_L("frame_2.mbm"));
	for(TInt i=2; i && KErrNone == iLastError; --i)
		{
		// start decoding
		iImageDisplay->Play();
		CActiveScheduler::Start();
		}
	TVerdict res=EFail;
	if (iLastError==KErrNone)
		{
		TRAP(iLastError, res=CompareFilesL(RefFilename(), DestFilename()));
		}
	
	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Got an error %d"), iLastError);
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

TBool CTestStepDrm::ConsumeFrame(const CFbsBitmap* /*aBitmap*/)
	{
	return EFalse;
	}

TVerdict CTestStepDrm::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	do 
		{
		TInt i=iTestParams->iPlayLoops+(iTestParams->iPlayLoops==0);
		TInt loopNum=1;
		INFO_PRINTF2(_L("Content unique ID '%s'"), *iCurrentUId);
		
		do 
			{		
			SetupPluginL();
			TRAP(iLastError, 
					iImageDisplay->SetupL();
				);
			if (iLastError==KErrNoMemory && iTestParams->iTestType == TImageDisplayUnitTestParams::EAlloc)
				{
				User::Leave(KErrNoMemory);
				}
			if (iLastError==KErrNone)
				{
	     		if (iTestParams->iDestWidth == 0)
					{
					iImageDisplay->SetSizeInPixels(iImageDisplay->RecommendedImageSizes()[0]);
					}				
				iImageDisplay->Play();	
				CActiveScheduler::Start();
				}
			if (iLastError==KErrNoMemory && iTestParams->iTestType == TImageDisplayUnitTestParams::EAlloc)
				{
				User::Leave(KErrNoMemory);
				}				
			iImageDisplay->Reset();
			if (iLastError!=KErrNone && i>1)
				{
				INFO_PRINTF3(_L("Got Error=%d during Loop=%d"), iLastError, loopNum);	
				return EFail;
				}
			} while (++loopNum,--i);
			
		INFO_PRINTF3(_L("Got Error=%d Expected=%d"), iLastError, iTestParams->iExpectedError);
		if (iLastError==KErrNoMemory && iTestParams->iTestType == TImageDisplayUnitTestParams::EAlloc)
			{
			User::Leave(KErrNoMemory);
			}		
		if (iLastError!=KErrNone && iLastError!=iTestParams->iExpectedError)
			{
			return EFail;
			}
		} while (iCurrentUId && *iCurrentUId && *(++iCurrentUId));
		
	return EPass;
	}

// MNG Mime Type recognition tests //
CTestStepImageDisplayRecog::CTestStepImageDisplayRecog(const TMimeTypeRecogTestParam& aTestParam):
																iTestParam(aTestParam)
	{
	iTestStepName.Copy(iTestParam.iTestName);
	}

TVerdict CTestStepImageDisplayRecog::DoTestStepPreambleL()
	{
	TVerdict v=CTestStepImageDisplay::DoTestStepPreambleL();
	if (EPass!=v)
		{
		return v;
		}
	User::LeaveIfError( iApaLsSession.Connect() );
	return EPass;
	}
	
TVerdict CTestStepImageDisplayRecog::DoTestStepPostambleL()
	{
	iApaLsSession.Close();
	return CTestStepImageDisplay::DoTestStepPostambleL();
	}
	
TVerdict CTestStepImageDisplayRecog::DoTestStepL()
	{
	INFO_PRINTF2(_L("%S"), &iTestStepName);
	INFO_PRINTF2(_L("%s"), iTestParam.iTestDescription);
	TInt error=KErrNone;
	TDataRecognitionResult  result;
	result.Reset();
	SetSourceFilename(TPtrC(iTestParam.iFileNameParam));
	if (iTestParam.iParamType==TMimeTypeRecogTestParam::EParamIsFileName)
		{
		error=iApaLsSession.RecognizeData(SourceFilename(), KNullDesC8(), result);
		}
	else if (iTestParam.iParamType==TMimeTypeRecogTestParam::EParamIsDataBuffer)
		{
		TPtrC8 dataBuf;
		TBuf8<32> buffer;
		if (iTestParam.iDataBufferParam==NULL)
			{
			RFile f;
			if ((error=f.Open(iFs, SourceFilename(), EFileRead|EFileShareAny))==KErrNone)
				{
				error=f.Read(buffer);
				}
			dataBuf.Set(buffer);
			f.Close();
			}
		else
			{
			dataBuf.Set(iTestParam.iDataBufferParam);
			}
		if (error==KErrNone)
			{
			error=iApaLsSession.RecognizeData(SourceFilename(), dataBuf, result);
			}
		}
	else
		{
		ASSERT(EFalse);
		}
		
	if (error!=KErrNone || result.iConfidence < iTestParam.iMinConfidence)
		{
		INFO_PRINTF3(_L("Got Failed to recognize data Error=%d Confidence=%d"), error,result.iConfidence);
		return EFail;
		}
	if (result.iDataType.Des8().Compare(TPtrC8(iTestParam.iExpectedMimeType))!=0)
		{
		TPtrC ptr(result.iDataType.Des());
		TFileName mime;
		mime.Copy( TPtrC8(iTestParam.iExpectedMimeType) );
		INFO_PRINTF3(_L("Got Wrong Mime type Got '%S' Expected '%S'"), &ptr, &mime);
		return EFail;
		}
	return EPass;
	}

void CTestStepImageDisplayRecog::MiidoImageReady(const CFbsBitmap* /*aBitmap*/, TUint /*aStatus*/, const TRect& /*aUpdatedArea*/, TInt /*aError*/)
	{
	ASSERT(EFalse);// we mustn't be here	
	}


// 
// Regression test case for defect: DEF082108: Browsing to www.mobil.se will crash the phone 
CTestStepImageDisplayMimeCheck::CTestStepImageDisplayMimeCheck()
	{
	iTestStepName.Copy(_L("MM-ICL-IMGDISP-U-0306-HP"));
	}

TVerdict CTestStepImageDisplayMimeCheck::DoTestStepPreambleL()
	{
	CTestStepImageDisplay::DoTestStepPreambleL();
	
	return EPass;
	}
	
TVerdict CTestStepImageDisplayMimeCheck::DoTestStepPostambleL()
	{
	CTestStepImageDisplay::DoTestStepPostambleL();
	
	return EPass;
	}
	
TVerdict CTestStepImageDisplayMimeCheck::DoTestStepL()
	{
	INFO_PRINTF1(_L("CTestStepImageDisplayMimeCheck::DoTestStepL()"));
	INFO_PRINTF1(_L("Regression test case for defect: DEF082108"));
	INFO_PRINTF1(_L("Checks that CIclRecognizerUtil::GetMimeTypeL() can handle aFileName > KMaxFileName"));
	
	TBuf8<4> KMimeType;
	TBuf<KMaxFileName*2> filename;
	filename.SetLength(KMaxFileName*2);
	
	CIclRecognizerUtil* temp = CIclRecognizerUtil::NewL();
	CleanupStack::PushL(temp);
	
	HBufC8* header = HBufC8::NewL(32);
	
	CleanupStack::Pop();
	TRAPD(err, temp->GetMimeTypeL(*header, filename, KMimeType));
	
	delete temp;
	delete header;
	
	if ( err == KErrBadName )
		{
		return EPass;	
		}

	return EFail;
	}

void CTestStepImageDisplayMimeCheck::MiidoImageReady(const CFbsBitmap* /*aBitmap*/, TUint /*aStatus*/, const TRect& /*aUpdatedArea*/, TInt /*aError*/)
	{
	ASSERT(EFalse);// we mustn't be here	
	}
