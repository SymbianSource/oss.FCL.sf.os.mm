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
#include <e32math.h>
#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>
#include <icl/exifimagedisplayext.h>
#include <bitmaptransforms.h>
#include <exifutility.h>
#include <hal.h>

// Test system includes
#include "TestSuiteExifImageDisplay.h"

#include "TestStepExifImageDisplay.h"


// Heap size for these tests.
const TInt KHeapSize		= 0x100000;
const TInt KErrFilesDiffer	= -101;

_LIT(KIclPanicCategory, "TS_ExifImageDisplay");


GLDEF_C void Panic(TS_ExifImageDisplayPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

	
/**
*
* CTestStepIclImageDisplay constructor 
*
* 
*/
CTestStepIclImageDisplay::CTestStepIclImageDisplay() 
:  iFs(), iDiffTolerance(0), iImageDisplaySuite(NULL), iSourceFilename(), iScheduler(NULL)
	{
	}

/**
*
* CTestStepIclImageDisplay destructor
*
* 
*/
CTestStepIclImageDisplay::~CTestStepIclImageDisplay()
	{
	delete iImageDisplay;
	iImageDisplay = NULL;
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTESTSUITEMNGIMAGEDISPLAY
*/

void CTestStepIclImageDisplay::SetSuite(CTestSuiteExifImageDisplay* aPtr)
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
TVerdict CTestStepIclImageDisplay::DoTestStepPreambleL()
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

TVerdict CTestStepIclImageDisplay::DoTestStepPostambleL()
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
* CTestStepIclImageDisplay Suite accessor function
* @returns CTestSuiteExifImageDisplay*  
*/

CTestSuiteExifImageDisplay* CTestStepIclImageDisplay::Suite()
	{
	return iImageDisplaySuite;
	}


/**
*
* CTestStepIclImageDisplay SetTestFilename
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&  afull file and path name
* @returns void
*/

void CTestStepIclImageDisplay::DefaultPath(TFileName& aName)
	{
	Suite()->DefaultPath(aName);
	}

void CTestStepIclImageDisplay::SetSourceFilename(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TParse parse;
	parse.Set(aPathNameAndExtn, NULL, NULL);
	TFileName fileName;
	if (parse.DrivePresent())
		{
		fileName.Copy(aPathNameAndExtn);
		iSourceFilename = fileName;
		return;
		}
	DefaultPath( fileName );
	if (parse.DriveAndPath().Length()!=0 && parse.DriveAndPath()[0]=='\\') // we've got "absolute" path
		{
		fileName.SetLength(2); // keep drive letter only
		}

	fileName.Append(aPathNameAndExtn);
	iSourceFilename = fileName;
	}

void CTestStepIclImageDisplay::SetDestFilenameL(const TDesC& aPathNameAndExtn)
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
void CTestStepIclImageDisplay::DeleteDestFilenameL()
	{
	TInt err = iFs.Delete(DestFilename());
	if (err != KErrNone && err != KErrNotFound)
		{
		INFO_PRINTF3(_L("Unable to delete output file %S, err = %d"),&iDestFilename, err);
		User::Leave(err);
		}
	}

void CTestStepIclImageDisplay::SetRefFilename(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TParse parse;
	parse.Set(aPathNameAndExtn, NULL, NULL);
	TFileName fileName;
	if (parse.DrivePresent())
		{
		fileName.Copy(aPathNameAndExtn);
		iRefFilename = fileName;
		return;
		}
	DefaultPath( fileName );
	if (parse.DriveAndPath().Length()!=0 && parse.DriveAndPath()[0]=='\\') // we've got "absolute" path
		{
		fileName.SetLength(2); // keep drive letter only
		}
	else
		{
		_LIT(KRefFolder, "ref\\");
		fileName.Append(KRefFolder);
		}

	fileName.Append(aPathNameAndExtn);
	iRefFilename = fileName ;
	}


/**
*
* CTestStepIclImageDisplay SourceFilename
*
* 
* 
*/
TFileName& CTestStepIclImageDisplay::SourceFilename() 
	{
	return  iSourceFilename;
	}

/**
*
* CTestStepIclImageDisplay DestFilename
*
* 
* 
*/
TFileName& CTestStepIclImageDisplay::DestFilename() 
	{
	return  iDestFilename;
	}

/**
*
* CTestStepIclImageDisplay RefFilename
*
* 
* 
*/
TFileName& CTestStepIclImageDisplay::RefFilename() 
	{
	return  iRefFilename;
	}

TInt CTestStepIclImageDisplay::CompareFiles(const TDesC& aFile1,const TDesC& aFile2)
	{
	TVerdict V=EFail;
	TRAPD(err, V=CompareFilesL(aFile1, aFile2) );
	if (KErrNone != err)
		{
		return err;
		}
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
TVerdict CTestStepIclImageDisplay::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2)
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


TVerdict CTestStepIclImageDisplay::CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const
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

TInt CTestStepIclImageDisplay::CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile, TBool aAllowDifDispMode) 
	{
	CFbsBitmap* refBitmap=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(refBitmap);
	User::LeaveIfError( refBitmap->Load(aFile, 0) );

	if (aBitmap.SizeInPixels() != refBitmap->SizeInPixels() ||
			(aBitmap.DisplayMode() != refBitmap->DisplayMode() && !aAllowDifDispMode)
		)
		{
		CleanupStack::PopAndDestroy(refBitmap);
		return EFail;
		}
	const TInt srcScanlineLen=CFbsBitmap::ScanLineLength(aBitmap.SizeInPixels().iWidth, aBitmap.DisplayMode());
	HBufC8* srcScanline = HBufC8::NewMaxLC(srcScanlineLen);
	TPtr8 src(srcScanline->Des());
	HBufC8* refScanline = HBufC8::NewMaxLC(srcScanlineLen);
	TPtr8 ref(refScanline->Des());

	TInt result = KErrNone;
	TInt diffs=0;
	const TInt diffTolerance=TInt( TReal(aBitmap.SizeInPixels().iHeight*srcScanlineLen)*iDiffTolerance );
	const TUint8* pSrc=src.Ptr();
	const TUint8* pRef=ref.Ptr();
	for (TInt y=aBitmap.SizeInPixels().iHeight; y; --y)
		{
		const TPoint pnt(0,y-1);
		aBitmap.GetScanLine(src,pnt,srcScanlineLen, aBitmap.DisplayMode());
		refBitmap->GetScanLine(ref,pnt,srcScanlineLen, aBitmap.DisplayMode());
		ASSERT(src.Length()==ref.Length() && ref.Length());

		TInt i=src.Length()-1;
		if (aBitmap.DisplayMode()==EColor16M && diffTolerance != 0)
			{
			do
				{
				diffs +=(pRef[i] != pSrc[i]);
				} while (i--);
			}
		else
			{
			diffs += ( Mem::Compare(pSrc,src.Length(), pRef, src.Length()) !=0 );
			}

		if (diffs > diffTolerance)
			{
			result = KErrFilesDiffer;
			INFO_PRINTF4(_L("Diffs %d Tolerance %d got at line=%d"),diffs, diffTolerance, y);
			break;
			}
		}
	if (diffs != 0)
		{
		INFO_PRINTF4(_L("Diffs %d Tolerance %d res=%d"),diffs, diffTolerance, result);
		}

	CleanupStack::PopAndDestroy(3, refBitmap); 

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
		iFramesToDecode(aTestParams.iMaxFramesToDecode)
	{
	// Store the name of this test case
	iTestStepName = iTestParams->iTestName;

	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	if (!Math::IsNaN(iTestParams->iTolerance) && iTestParams->iTolerance<1.0 && iTestParams->iTolerance>0.0)
		{
		SetDiffTolerance(iTestParams->iTolerance);
		}
	}

/**
*
* Test step destructor.
* 
*/
CTestStepImgDisplayGeneric::~CTestStepImgDisplayGeneric()
	{
	delete iSourceBuf;
	}

void CTestStepImgDisplayGeneric::Cleanup()
	{
	delete iImageDisplay; iImageDisplay= NULL;

	iFileHandle.Close();
	delete iSourceBuf; iSourceBuf = NULL;

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
TVerdict CTestStepImgDisplayGeneric::DoTestStepL()
	{
	INFO_PRINTF2(_L("%S"), &iTestStepName);
	INFO_PRINTF2(_L("%s"), iTestParams->iTestDescription);
	if (iTestParams->iInputFilename)
		{
		INFO_PRINTF2(_L("Using the %s"), iTestParams->iInputFilename);
		}
	
	TVerdict verdict = EFail;
	
	if (!iTestStepName.Compare(_L("MM-ICL-GENICLIMGDISP-U-0011-HP")))
		{
		TInt machineUID = 0;
		iIsNaviEngine = EFalse;
		iIsH6 = EFalse;
		TInt err = HAL::Get(HAL::EMachineUid, machineUID);
 		if (err != KErrNone)
  			{
  			INFO_PRINTF2(_L("---Problem calling HAL::Get(HAL::EMachineUid); failed with %d"), err);
  			return verdict;
  			}
  		if(machineUID==HAL::EMachineUid_NE1_TB)
  			{
  			iIsNaviEngine = ETrue;
  			}
		else if (machineUID==HAL::EMachineUid_OmapH6)
	  		{
	  		iIsH6 = ETrue;
	  		}  			
  		}
  			
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
			TBool heapTestingComplete = (testAlloc == NULL || err==KErrNone);
			User::Free(testAlloc);

			Cleanup();

			__UHEAP_RESET;
			__MM_HEAP_MARKEND;

			if (heapTestingComplete)
				{
				INFO_PRINTF4(_L("err = %d, verdict = %d, Fail count = %d"), err, verdict, i);
				INFO_PRINTF1(_L("Alloc testing completed"));
				break;
				}
			else if (err != KErrNoMemory)
				{
				INFO_PRINTF3(_L("---ERROR: Got unexpected error err = %d, Fail count = %d"), err, i);
				verdict = EFail;
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
	iLastError = KErrNone;
	User::LeaveIfError(FbsStartup());
	// [ connect to the bitmap server and check the error code]
	User::LeaveIfError(RFbsSession::Connect());
	iFbsConnection = ETrue;

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

TBool CTestStepImgDisplayGeneric::ConsumeFrame(const CFbsBitmap* /*aBitmap*/, TBool /*aNoDelayCheck*/)
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
	TBool ContinueDecoding= EFalse;
	if (KErrNone == iLastError && (aStatus & CImageDisplayPlugin::EStatusFrameReady))
		{
		ContinueDecoding=ConsumeFrame(aBitmap);
		}

	if (KErrNone != iLastError)
		{
		CActiveScheduler::Stop();
		}
	else if (aStatus & CImageDisplayPlugin::EStatusFrameReady)
		{
		if (ContinueDecoding && !(aStatus & CImageDisplayPlugin::EStatusNoMoreToDecode))
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
	if (iTestParams->iDestWidth==TImageDisplayUnitTestParams::KDestSizeIsSecondDefSize)
		{
		if ( Sizes.Count() < 2)
			{
			Panic(EAPIBroken);
			}
		iImageDisplay->SetSizeInPixels(Sizes[1]);
		}
	else if (iTestParams->iDestWidth==TImageDisplayUnitTestParams::KDestSizeIsDoubleSize)
		{
		iImageDisplay->SetSizeInPixels( TSize(Sizes[0].iWidth*2, Sizes[0].iHeight*2));
		}
	else
		{
		iImageDisplay->SetSizeInPixels(Sizes[0]);
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
		
	if (iTestParams->iClipRect != NULL)
		{
		iImageDisplay->SetSourceRect(TRect(iTestParams->iClipRect[0],iTestParams->iClipRect[1],iTestParams->iClipRect[2],iTestParams->iClipRect[3]));
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
	iImageDisplay->SetSourceRect(TRect(200,200,800,800));	
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iImageDisplay->Play();
	iImageDisplay->GetBitmap(bmp,mask);
	return EFail;
	}

CTestStepPanicInitFailed::CTestStepPanicInitFailed(const TImageDisplayUnitTestParams& aTestParams):
		CTestStepNegative(aTestParams)
	{
	
	}

TVerdict CTestStepPanicInitFailed::TestL()
	{
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetSourceRect(TRect(200,200,800,800));
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iImageDisplay->Play();
	CActiveScheduler::Start();
	iImageDisplay->Play();
	return EFail;
	}

CTestStepNegativeReduction::CTestStepNegativeReduction(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepImgDisplayGeneric(aTestParams)
	{
	}


TVerdict CTestStepNegativeReduction::TestL()
	{
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	MExifImageDisplayExtension* exifExt=NULL;
	TAny* iFace;
	iLastError=iImageDisplay->ExtensionInterface(KExifImageDisplayExtensionUid, iFace);
	exifExt=reinterpret_cast<MExifImageDisplayExtension*>(iFace);
	User::LeaveIfError(iLastError);
	User::LeaveIfNull(exifExt);
	if (iTestParams->iErrorCode != exifExt->SetMaximumReductionFactor(-1))
		{
		exifExt->Release();
		INFO_PRINTF3(_L("Wrong failure Error code. Got %d Expected %d"),iLastError, iTestParams->iErrorCode);
		return EFail;
		}
	exifExt->Release();
	return EPass;
	}

CTestStepUnsupExt::CTestStepUnsupExt(const TImageDisplayUnitTestParams& aTestParams)
															:CTestStepImgDisplayGeneric(aTestParams)
	{
	}

TVerdict CTestStepUnsupExt::TestL()
	{
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	TAny* iFace;
	TInt err=iImageDisplay->ExtensionInterface(KNullUid, iFace);
	// we should've had an error
	return err==iTestParams->iErrorCode? EPass: EFail;
	}


CTestStepBigReduction::CTestStepBigReduction(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepImgDisplayGeneric(aTestParams)
	{
	}


TVerdict CTestStepBigReduction::TestL()
	{
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	MExifImageDisplayExtension* exifExt=NULL;
	TAny* iFace;
	iLastError=iImageDisplay->ExtensionInterface(KExifImageDisplayExtensionUid, iFace);
	exifExt=reinterpret_cast<MExifImageDisplayExtension*>(iFace);
	User::LeaveIfError(iLastError);
	User::LeaveIfNull(exifExt);
	if (iTestParams->iErrorCode != exifExt->SetMaximumReductionFactor(10))
		{
		exifExt->Release();
		INFO_PRINTF3(_L("Wrong failure Error code. Got %d Expected %d"),iLastError, iTestParams->iErrorCode);
		return EFail;
		}
	exifExt->Release();
	return EPass;
	}


CTestStepExifGeneric::CTestStepExifGeneric(const TImageDisplayUnitTestParams& aTestParams):
																CTestStepWithCheckpoint(aTestParams)
	{
	
	}

TVerdict CTestStepExifGeneric::TestL()
	{
	TVerdict testStepResult = EPass;
	const TRect clipRect(186,143,533,402);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	
	SetupPluginL();
	iImageDisplay->SetSourceRect(clipRect);
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	//
	// try to "reuse" framework
	//
	iImageDisplay->Reset();
	SetupPluginL();
	iImageDisplay->SetSourceRect(clipRect);
	iImageDisplay->SetupL();
	
	MExifImageDisplayExtension* exifExt=NULL;
	TAny* iFace;
	TInt err=iImageDisplay->ExtensionInterface(KExifImageDisplayExtensionUid, iFace);
	exifExt=reinterpret_cast<MExifImageDisplayExtension*>(iFace);
	if (err!= KErrNone || exifExt==NULL)
		{
		INFO_PRINTF3(_L("Failed to get Exif ext. IFace err=%d IFace=%u"),err, exifExt);
		iLastError = KErrGeneral;
		return EFail;
		}
	CleanupReleasePushL(*exifExt);
	if (exifExt->Uid() != KExifImageDisplayExtensionUid)
		{
		INFO_PRINTF2(_L("Ext. Iface reported wrong Uid %u"),exifExt->Uid().iUid);
		iLastError = KErrGeneral;
		User::Leave(iLastError);
		}		
	MExifMetadataReader* reader=NULL;
	if (KErrNone!=(err=exifExt->GetExifMetadataReader(reader)))
		{
		INFO_PRINTF3(_L("Failed to GetExifMetadataReader() IFace err=%d IFace=%u"),err, reader);
		iLastError = KErrGeneral;
		User::Leave(iLastError);
		}
	TExifReaderUtility utility(reader);
	TUint16 compr;
	err = utility.GetThumbCompression(compr);
	TUint imageStatus(iImageDisplay->ImageStatus());
	if (err == KErrNone && (imageStatus&CImageDisplay::EImageHasThumbnail)==0)
		{
		INFO_PRINTF2(_L("Error: No thumbnail is reported by ImageStatus() status=%u"),imageStatus);
		iLastError = KErrGeneral;
		User::Leave(iLastError);		
		}
		
	if ((imageStatus&(CImageDisplay::EImageAnimated)) || imageStatus==CImageDisplay::EImageTypeUnknown)
		{
		INFO_PRINTF2(_L("Error: wrong ImageStatus() status=%u"),imageStatus);
		iLastError = KErrGeneral;
		User::Leave(iLastError);		
		}
	if (KErrNone!=(iLastError=exifExt->SetMaximumReductionFactor(3)))
		{
		INFO_PRINTF2(_L("Error: SetMaximumReductionFactor(3), err=%d"),iLastError);
		User::Leave(iLastError);
		}
	if (KErrNone!=(iLastError=exifExt->SetScaleQuality(CBitmapScaler::EMinimumQuality)))
		{
		INFO_PRINTF2(_L("Error: SetScaleQuality(CBitmapScaler::EMinimumQuality), err=%d"),iLastError);
		User::Leave(iLastError);
		}	
	CleanupStack::PopAndDestroy(); // exifExt->Release()
	
	iFrameNumber = 0;
	for(TInt i=2; i && KErrNone == iLastError; --i)
		{
		ResetCheckPoints();
		iFrameNumber = 0;
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
