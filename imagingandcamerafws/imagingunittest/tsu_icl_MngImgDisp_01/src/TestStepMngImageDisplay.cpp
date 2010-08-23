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

// Test system includes
#include "TestSuiteMngImageDisplay.h"

#include "TestStepMngImageDisplay.h"

#include <caf/caf.h>
#include <caf/content.h>
#include <caf/data.h>





// Heap size for these tests.
const TInt KHeapSize		= 0x100000;
const TInt KErrFilesDiffer	= -101;

_LIT(KIclPanicCategory, "TS_MngImageDisplay");


GLDEF_C void Panic(TS_MngImageDisplayPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

	
/**
*
* CTestStepMngImageDisplay constructor 
*
* 
*/
CTestStepMngImageDisplay::CTestStepMngImageDisplay(const TImageDisplayUnitTestParams &aTestParams) 
:  iFs(), iTestParams(&aTestParams), iImageDisplaySuite(NULL), iSourceFilename(), iScheduler(NULL)
	{
	}

/**
*
* CTestStepMngImageDisplay destructor
*
* 
*/
CTestStepMngImageDisplay::~CTestStepMngImageDisplay()
	{
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTESTSUITEMNGIMAGEDISPLAY
*/

void CTestStepMngImageDisplay::SetSuite(CTestSuiteMngImageDisplay* aPtr)
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
TVerdict CTestStepMngImageDisplay::DoTestStepPreambleL()
	{
	//[ mark the heap and unmark at the end of postamble ]
	__MM_HEAP_MARK;

	if (!iFbsConnection)
		{
        User::LeaveIfError(FbsStartup());
		User::LeaveIfError(RFbsSession::Connect());
		iFbsConnection = ETrue;
		}

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

TVerdict CTestStepMngImageDisplay::DoTestStepPostambleL()
	{
	Cleanup();

	iFs.Close();

	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	RFbsSession::Disconnect();
    // [ umark the heap which was marked in preamble ]
	__MM_HEAP_MARKEND;

	return EPass;
	}


/**
*
* CTestStepMngImageDisplay Suite accessor function
* @returns CTESTSUITEMNGIMAGEDISPLAY*  
*/

CTestSuiteMngImageDisplay* CTestStepMngImageDisplay::Suite()
	{
	return iImageDisplaySuite;
	}


/**
*
* CTestStepMngImageDisplay SetTestFilename
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&  afull file and path name
* @returns void
*/

void CTestStepMngImageDisplay::DefaultPath(TFileName& aName)
	{
	Suite()->DefaultPath(aName);
	}

void CTestStepMngImageDisplay::SetSourceFilename(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName;
	_LIT(KDonntSet,"z:");
	if(aPathNameAndExtn.FindC(KDonntSet) == KErrNotFound)
		{
		DefaultPath( fileName );	
		}
	fileName.Append(aPathNameAndExtn);
	iSourceFilename = fileName ;
	}

void CTestStepMngImageDisplay::SetDestFilenameL(const TDesC& aPathNameAndExtn)
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
	DeleteDestFilenameL();
	}

// delete the destination file
void CTestStepMngImageDisplay::DeleteDestFilenameL()
	{
	TInt err = iFs.Delete(DestFilename());
	if (err != KErrNone && err != KErrNotFound)
		{
		INFO_PRINTF3(_L("Unable to delete output file %S, err = %d"),&iDestFilename, err);
		User::Leave(err);
		}
	}


void CTestStepMngImageDisplay::SetRefFilename(const TDesC& aPathNameAndExtn)
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
* CTestStepMngImageDisplay SourceFilename
*
* 
* 
*/
TFileName& CTestStepMngImageDisplay::SourceFilename() 
	{
	return  iSourceFilename;
	}

/**
*
* CTestStepMngImageDisplay DestFilename
*
* 
* 
*/
TFileName& CTestStepMngImageDisplay::DestFilename() 
	{
	return  iDestFilename;
	}

/**
*
* CTestStepMngImageDisplay RefFilename
*
* 
* 
*/
TFileName& CTestStepMngImageDisplay::RefFilename() 
	{
	return  iRefFilename;
	}

TInt CTestStepMngImageDisplay::CompareFiles(const TDesC& aFile1,const TDesC& aFile2)
	{
	TVerdict V=EFail;
	TRAPD(err, V=CompareFilesL(aFile1, aFile2) );
	if (KErrNone != err)
		return err;
	return (V==EPass? KErrNone : KErrFilesDiffer );
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
TVerdict CTestStepMngImageDisplay::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2)
	{
	RFile file1, file2;

	User::LeaveIfError(file1.Open(iFs,aFile1,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file1);

	User::LeaveIfError(file2.Open(iFs,aFile2,EFileShareAny|EFileStream|EFileRead));
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
				{
				copySize = blockSize;
				}
			else
				{
				copySize = fileSize1 - length;
				}
			User::LeaveIfError(file1.Read(buf1Ptr,copySize));
			User::LeaveIfError(file2.Read(buf2Ptr,copySize));

			if (Mem::Compare(buf1Ptr.Ptr(),copySize,buf2Ptr.Ptr(),copySize)!=0)
				{
				result = EFail;
				}

			length += copySize;
			}
		CleanupStack::PopAndDestroy(2); // buf1, buf2
		}
	else
		{
		result = EFail;
		}


	CleanupStack::PopAndDestroy(2); //file1, file2

	return result;
	}


TVerdict CTestStepMngImageDisplay::CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const
	{
	RFile file;

	User::LeaveIfError(file.Open(iFs, aFile, EFileShareAny|EFileStream|EFileRead));
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
			{
			result = EFail;
			}
			
		CleanupStack::PopAndDestroy(buf);
		}
	else
		{
		result = EFail;
		}

	file.Close();

	CleanupStack::PopAndDestroy(1); //file

	return result;
	}

TInt CTestStepMngImageDisplay::CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile) const
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
   	
   	TInt result = KErrNone;
   	for (TInt y=aBitmap.SizeInPixels().iHeight; y; --y)
		{
      	const TPoint pnt(0,y-1);
      	aBitmap.GetScanLine(src,pnt,srcScanlineLen, aBitmap.DisplayMode());
      	refBitmap->GetScanLine(ref,pnt,srcScanlineLen, aBitmap.DisplayMode());	
		if (Mem::Compare(pSrc,src.Length(), pRef, src.Length()) !=0)
			{
			result = KErrGeneral;
			break;
			}
		}
	
	CleanupStack::PopAndDestroy(3,refBitmap); 

	return result;
	}


TVerdict CTestStepMngImageDisplay::CompareBitmapToRefBuffer(const CFbsBitmap* aBitmap, const TUint* aBuffer)
	{
	const TUint* bufferPtr=aBuffer;
	TUint mask=0xffffffu;
	if (aBitmap->DisplayMode() == EGray256)
		{
		mask=0xffu;
		}
	for (TInt y=0; y<aBitmap->SizeInPixels().iHeight; ++y)
		{
		for (TInt x=0; x<aBitmap->SizeInPixels().iWidth; ++x)
		{
			TRgb rgbPixel;
			aBitmap->GetPixel(rgbPixel, TPoint(x,y));
			const TUint pixel=(mask&rgbPixel.Value());
			if (*bufferPtr != pixel )
				{
				INFO_PRINTF5(_L("Bitmap check failed at (%d,%d) Got %08x Expected %08x"),x,y,pixel, *bufferPtr);
									
				return EFail;
				}
			++bufferPtr;
			}
		}
	return EPass;
	}

TVerdict CTestStepMngImageDisplay::Compare32BitBitmapToRefBuffer(const CFbsBitmap* aBitmap, const TUint* aBuffer)
	{
	const TUint* bufferPtr=aBuffer;	
	for (TInt y=0; y<aBitmap->SizeInPixels().iHeight; ++y)
		{
		for (TInt x=0; x<aBitmap->SizeInPixels().iWidth; ++x)
			{
			TRgb rgbPixel;
			aBitmap->GetPixel(rgbPixel, TPoint(x,y));
			if (*bufferPtr != rgbPixel.Internal())
				{
				INFO_PRINTF5(_L("Bitmap check failed at (%d,%d) Got %08x Expected %08x"), x, y, rgbPixel.Internal(), *bufferPtr);	
				return EFail;
				}
			++bufferPtr;
			}
		}
	return EPass;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
*/

CTestStepImgDisplayGeneric::CTestStepImgDisplayGeneric(const TImageDisplayUnitTestParams& aTestParams) :
		CTestStepMngImageDisplay(aTestParams),
		iFramesToDecode(aTestParams.iMaxFramesToDecode)
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
	delete iSourceBuf;
	delete iImageDisplay;
	iImageDisplay = NULL;
	}

void CTestStepImgDisplayGeneric::Cleanup()
	{
	delete iImageDisplay; iImageDisplay= NULL;

	iFileHandle.Close();
	delete iSourceBuf; iSourceBuf = NULL;
	CTestStepMngImageDisplay::Cleanup();
	}

void CTestStepMngImageDisplay::Cleanup()
	{
	if (iFbsConnection)
		{
		RFbsSession::Disconnect();
		}
		
	iFbsConnection = EFalse;
	}

/**
 *
 * Do the test step.
 * transform from file to file
 * @return	"TVerdict" the status of the test
 *			 The result of the test step 
 */
TVerdict CTestStepMngImageDisplay::DoTestStepL()
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
				{
				INFO_PRINTF2(_L("Fail count = %d"), i);
				}

			__UHEAP_SETFAIL(RHeap::EFailNext, i);

			TRAP(err, verdict = DoTestStepFbsL());

			TAny* testAlloc = User::Alloc(1);
			TBool heapTestingComplete = (testAlloc == NULL) && (err==KErrNone);
			User::Free(testAlloc);

			Cleanup();

			__UHEAP_RESET;
			__MM_HEAP_MARKEND;

			if ((err != KErrNoMemory ) || heapTestingComplete)
				{
				INFO_PRINTF4(_L("err = %d, verdict = %d, Fail count = %d"), err, verdict, i);
				INFO_PRINTF1(_L("Alloc testing completed successfully"));
				verdict = EPass;
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

TVerdict CTestStepMngImageDisplay::DoTestStepFbsL()
	{
	iLastError = KErrNone;
	// [ connect to the bitmap server and check the error code]
	if (!iFbsConnection)
		{
        User::LeaveIfError(FbsStartup());
		User::LeaveIfError(RFbsSession::Connect());
		iFbsConnection = ETrue;
		}

	TVerdict verdict = TestL();

	return verdict;

	}

void CTestStepImgDisplayGeneric::SetImageSourceL()
	{
	ASSERT(iImageDisplay);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	switch (iTestParams->iSourceType)
		{
		case TImageDisplayUnitTestParams::ETypeFileName:
			{
			iImageDisplay->SetImageSource(TMMFileSource(SourceFilename()));
			}
			break;
		case TImageDisplayUnitTestParams::ETypeFileHandle:
			{
			iFileHandle.Close();
			User::LeaveIfError(iFs.ShareProtected());
			User::LeaveIfError( iFileHandle.Open(iFs, SourceFilename(), EFileRead | EFileShareExclusive/*EFileShareAny*/) );
			iImageDisplay->SetImageSource(TMMFileHandleSource(iFileHandle));
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

TBool CTestStepImgDisplayGeneric::ConsumeFrameL(const CFbsBitmap* /*aBitmap*/)
	{
	if (++iFrameNumber >= iFramesToDecode && iFramesToDecode)
		{
		return EFalse;
		}
		
	return ETrue;
	}

void CTestStepImgDisplayGeneric::MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError)
	{
	if (aError != KErrNone)
		{
		INFO_PRINTF3(_L("ImgReadCb Status=0x%04x Error=%d"), aStatus, aError);	
		}
	iLastPluginStatus	= aStatus;
	iLastUpdateRect		= aUpdatedArea;
	iLastError			= aError;
	TBool continueDecoding= EFalse;
	if (KErrNone == iLastError && (aStatus & CImageDisplayPlugin::EStatusFrameReady))
		{
		TRAPD(err, continueDecoding=ConsumeFrameL(aBitmap) );
		if (err != KErrNone)
			{
			iLastError = err;
			}
		}

	if (KErrNone != iLastError)
		{
		CActiveScheduler::Stop();
		}
	else if (aStatus & CImageDisplayPlugin::EStatusFrameReady)
		{
		if (continueDecoding && !(aStatus & CImageDisplayPlugin::EStatusNoMoreToDecode))
			{
			iImageDisplay->Play();
			}
		else
			{
			CActiveScheduler::Stop();	
			}
		}
	}

void CTestStepImgDisplayGeneric::SetDefaultDestSize()
	{
	CImageDisplay::RImageSizeArray Sizes=iImageDisplay->RecommendedImageSizes();
	if ( 0 == Sizes.Count())
		{
		Panic(EAPIBroken);
		}
	iImageDisplay->SetSizeInPixels(Sizes[0]);
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
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	//
	// try to "reuse" framework
	//
	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		return (testStepResult = EFail );
		}

	iFramesToDecode = 20;
	iFrameNumber = 0;
	for(TInt i=2; i && KErrNone == iLastError; --i)
		{
		// start decoding
		iImageDisplay->Play();
		CActiveScheduler::Start();
		}

	if (KErrNone != iLastError && 
			!(iTestParams->iTestType==TImageDisplayUnitTestParams::EAlloc && KErrNoMemory == iLastError))
		{
		testStepResult = EFail;
		}

	User::LeaveIfError(iLastError);
	iImageDisplay->StopPlay();
	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

void CTestStepImgDisplayGeneric::SetupPluginL()
	{
	iFrameNumber = 0;
	if (iTestParams->iPluginUid)
		{
		iImageDisplay->SetPluginUid(TUid::Uid(iTestParams->iPluginUid));
		}

	if (iTestParams->iDestWidth != -1)
		{
		iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), 
											iTestParams->iMaintainAspectRatio);
		}

	if (iTestParams->iDisplayMode != -1)
		{
		iImageDisplay->SetDisplayMode(TDisplayMode(iTestParams->iDisplayMode));
		}
		

	if (iTestParams->iImageOptions != -1)
		{
		iImageDisplay->SetOptions(iTestParams->iImageOptions);
		}
		
	
	if (iTestParams->iMimeType != NULL)
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
	SetRefFilename(outputFilename);
	}

TVerdict CTestStepImgDisplayGeneric::CheckTiming(const TTime aStart, const TTime aEnd, 
												TInt aMicroNeeded, TInt aAllowedPercentage) const
	{
	ASSERT(aAllowedPercentage >=0 && aAllowedPercentage < 100);
	TInt64 Diff=aEnd.Int64() - aStart.Int64();
	ASSERT( I64HIGH(Diff) == 0 );

	return ( Abs( I64INT(Diff - MAKE_TINT64(0,aMicroNeeded))) <= TReal(aMicroNeeded)*(TReal(aAllowedPercentage)/100) ? EPass: EFail);
	}

CTestStepNegative::CTestStepNegative(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepImgDisplayGeneric(aTestParams)
	{
	}


TVerdict CTestStepNegative::TestL()
	{
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	TRAP(iLastError, iImageDisplay->SetupL() );
	if (iLastError == KErrNone)
		{
		SetDefaultDestSize();
		iImageDisplay->Play();
		CActiveScheduler::Start();
		}
	if (iTestParams->iMaxFramesToDecode!=TImageDisplayUnitTestParams::KNoFrameLimit 
			&& iFrameNumber != iTestParams->iMaxFramesToDecode-1)
		{
		INFO_PRINTF3(_L("Wrong frame number for failure Got %d Expected %d"),iFrameNumber, iTestParams->iMaxFramesToDecode);
		return EFail;
		}
	if  (iTestParams->iErrorCode != iLastError)
		{
		INFO_PRINTF3(_L("Wrong failure Error code. Got %d Expected %d"),iLastError, iTestParams->iErrorCode);
		return EFail;
		}
	return EPass;
	}

CTestStepClipping::CTestStepClipping(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepImgDisplayGeneric(aTestParams)
	{
	}


TBool CTestStepClipping::ConsumeFrameL(const CFbsBitmap* aBitmap)
	{
	++iFrameNumber;
	TBuf<32> fileName ;
	fileName.AppendFormat(_L("digit_%d.mbm"),10-iFrameNumber);
	TRAP(iLastError, SetDestFilenameL(fileName) );
	if (iLastError != KErrNone)
		{
		return EFalse;
		}
	SetRefFilename(fileName);
	TRAP(iLastError, iLastError=CompareBitmapWithFileL(*aBitmap,  RefFilename() ));
	if (iLastError != KErrNone )
		{
		INFO_PRINTF3(_L("Images Do not match Frame=%d Err=%d"), iFrameNumber, iLastError);
		const_cast<CFbsBitmap*>(aBitmap)->Save(DestFilename());
		return EFalse;
		}
	return ETrue;
	}

TVerdict CTestStepClipping::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetSourceRect(TRect(16,16,48,48));
	iImageDisplay->SetupL();

	SetDefaultDestSize();
	iImageDisplay->Play();
	CActiveScheduler::Start();
	return (iLastError == KErrNone ? EPass : EFail);
	}

CTestStepPanicNotInit::CTestStepPanicNotInit(const TImageDisplayUnitTestParams& aTestParams):
		CTestStepNegative(aTestParams)
	{
	
	}

TVerdict CTestStepPanicNotInit::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	const CFbsBitmap* bmp=NULL;
	const CFbsBitmap* mask=NULL;
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iImageDisplay->Play();
	iImageDisplay->GetBitmap(bmp,mask);
	return EFail;
	}

CTestStepImgDisplayCAF::CTestStepImgDisplayCAF(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepImgDisplayGeneric(aTestParams)
	{
	}	

TVerdict CTestStepImgDisplayCAF::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iImageDisplay->Play();
	CActiveScheduler::Start();
	return (iLastError == KErrNone ? EPass : EFail);
	}

