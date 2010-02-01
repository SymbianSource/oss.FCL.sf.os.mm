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
// This file contains the test steps ICL positive testing for GT0210 MS3.4 File Handle APIs.
// EPOC includes
// 
//

#include <testframework.h>
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "TestPositiveConversionNew.h"

#include "ICLFbsSessionTest.h"

const TInt KMaxMimeLength = 256;

/**
 * 
 *
 * Class Constructor
 *
 */
CTestPositiveConversionNew::CTestPositiveConversionNew(const TDesC& aTestName) : 
		 CTestStepConversion(aTestName)
	{
	iHeapSize = 500000;
	}

/**
 * 
 *
 * NewL Method
 *
 */
CTestPositiveConversionNew *CTestPositiveConversionNew::NewL(const TDesC& aTestName)
	{
	CTestPositiveConversionNew* self = new(ELeave) CTestPositiveConversionNew(aTestName);
	return self;
	}


/**
 * 
 *
 * do the test step
 *
 */
TVerdict CTestPositiveConversionNew::DoTestStepL()
	{
	TVerdict currentVerdict = EPass;

	if( (iTestStepName.Compare(_L("MM-ICL-FRM-I-0701-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0702-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0703-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0704-CP")) == 0) )
		{
			currentVerdict = Step0701_0704_CPL();
		}
	else if( (iTestStepName.Compare(_L("MM-ICL-FRM-I-0705-CP")) == 0) ||
			 (iTestStepName.Compare(_L("MM-ICL-FRM-I-0706-CP")) == 0) ||
			 (iTestStepName.Compare(_L("MM-ICL-FRM-I-0707-CP")) == 0) ||
			 (iTestStepName.Compare(_L("MM-ICL-FRM-I-0708-CP")) == 0) )
		{
			currentVerdict = Step0705_0708_CPL();
		}
	else if ( (iTestStepName.Compare(_L("MM-ICL-FRM-I-0709-CP")) == 0) ||
			  (iTestStepName.Compare(_L("MM-ICL-FRM-I-0710-CP")) == 0) )
		{
			currentVerdict = Step0709_0710_CPL();
		}
	else if ( (iTestStepName.Compare(_L("MM-ICL-FRM-I-0711-CP")) == 0) ||
			  (iTestStepName.Compare(_L("MM-ICL-FRM-I-0712-CP")) == 0) )
		{
			currentVerdict = Step0711_0712_CPL();
		}

	return currentVerdict;
	}

/**
 * 
 * Read, decode and verify different graphic formats
 *	
 * Step 0701_CP to 0704_CP
 * 
 */
TVerdict CTestPositiveConversionNew::Step0701_0704_CPL()
	{
	TVerdict result = EFail;
	TInt theErr = KErrNone;
	TInt theErr1 = KErrNone;
	
	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		{
		return EInconclusive;
		}
	
	//__MM_HEAP_MARK;
	//[ Connect RFs ]
	User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);
	User::LeaveIfError(iFs.ShareProtected());
	
	//[ Open the Source Image File using RFile ]
	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(iFileNameSrc);

	INFO_PRINTF2(_L("Start : Open the file : %S by RFile"), &theSourceFilename);
	TInt theRes = iFile.Open(iFs, theSourceFilename, EFileWrite);
	User::LeaveIfError(theRes);
	CleanupClosePushL(iFile);
	INFO_PRINTF2(_L("End : Open the file : %S by RFile"), &theSourceFilename);

	//[ Connect to RFbsSession ]
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	//[ Open the File for convert, using the 'FileNewL' APIs for each test step ]
	INFO_PRINTF2(_L("Start : Open the file : %S by CImageDecoder::FileNewL"), &theSourceFilename);
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0701-CP")) == 0)
		{
		INFO_PRINTF1(_L("Read a JPEG,not specifying file type. Decode and verify JPEG."));
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, EPlay));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0702-CP")) == 0)
		{
		INFO_PRINTF1(_L("Read a JPEG,specifying MIME type. Decode and verify JPEG."));
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, mimeType, EPlay));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0703-CP")) == 0)
		{
		INFO_PRINTF1(_L("Read a OTA, specifying image type. Decode and verify OTA."));
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, EPlay, CImageDecoder::EOptionNone, iImageTypeUid, KNullUid, KNullUid));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0704-CP")) == 0)
		{
		INFO_PRINTF1(_L("Read a WBMP, specifying the Decoder UID. Decode and verify WBMP."));
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, EPlay, CImageDecoder::EOptionNone, iImageTypeUid, KNullUid, TUid::Uid(KWBMPDecoderImplementationUidValue)));
		}
	User::LeaveIfError(theRes);
	CleanupStack::PushL(iImageDecoder);
	INFO_PRINTF2(_L("End : Open the file : %S by CImageDecoder::FileNewL"), &theSourceFilename);

	//[ Call the main Decode Test Method ]
	TRAP(theErr1, theErr = DecodeUsingDecoderL(iFileNameSrc));
	
	//[ Delete and Close Local ImageDecoder and iFs ]
	CleanupStack::Pop();
	delete iImageDecoder;
	iImageDecoder = NULL;
	
	//[ Delete and Close RFs, RFile and FbsSession]
	CleanupStack::PopAndDestroy(3);
	//__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}
	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("Error : %d"), theErr);
		}

	return result;
	}

/**
 * 
 * Encode an image from bitmap to a file
 *	
 * Step 0705_CP to 0708_CP
 * 
 */
TVerdict CTestPositiveConversionNew::Step0705_0708_CPL()
	{
	TVerdict result = EFail ;
	TInt theErr = KErrNone;
	TInt theErr1 = KErrNone;
	TInt theRes = KErrNone;
	
	//__MM_HEAP_MARK;

	//[ Connect RFs ]
	User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);
	User::LeaveIfError(iFs.ShareProtected());
		
	// [ Open the Destination File using RFile ]
	TFileName theDestinationFileName; 
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(iFileNameSrc.Left(iFileNameSrc.Length()-3));
	GetImageExtension(iImageTypeUid, theExtension);
	theDestinationFileName.Append(theExtension);

	INFO_PRINTF2(_L("Start : Open the file : %S by RFile"), &theDestinationFileName);
	User::LeaveIfError(iFile.Replace(iFs, theDestinationFileName, EFileWrite));
	CleanupClosePushL(iFile);
	INFO_PRINTF2(_L("End : Open the file : %S by RFile"), &theDestinationFileName);

	//[ Connect to RFbsSession ]
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	
	//[ Open the File for convert, using the 'FileNewL' APIs for each test step ]
	INFO_PRINTF2(_L("Start : Open the file : %S by CImageEncoder::FileNewL"), &theDestinationFileName);
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0705-CP")) == 0)
		{
		INFO_PRINTF1(_L("Encode an image from bitmap to a file, not specifying the file type."));
		//Image type or encoder UID should be there, otherwise throws panic..
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, CImageEncoder::EOptionNone, iImageTypeUid));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0706-CP")) == 0)
		{
		INFO_PRINTF1(_L("Encode an image from bitmap to a file, given the MIME Type"));
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, mimeType));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0707-CP")) == 0)
		{
		INFO_PRINTF1(_L("Encode an image from bitmap to a file, given the Image Type"));
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, CImageEncoder::EOptionNone, iImageTypeUid, KNullUid, KNullUid));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0708-CP")) == 0)
		{
		INFO_PRINTF1(_L("Encode an image from bitmap to a file, given the Encoder UID."));
		//The Image type has to be given for Decoder Implementation
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, CImageEncoder::EOptionNone, KNullUid, KNullUid, TUid::Uid(KJPGEncoderImplementationUidValue)));
		}
	User::LeaveIfError(theRes);
	
	INFO_PRINTF2(_L("End : Open the file : %S by CImageEncoder::FileNewL"), &theDestinationFileName);
	
	//[ Call the main Encode Test Method ]
	EncodeUsingEncoderL(iFileNameSrc);
	
	//[ Delete and Close ImageEncoder ]
	
	delete iImageEncoder;
	iImageEncoder = NULL;
	
	//[ Delete and Close RFs, RFile and fbsSession]
	CleanupStack::PopAndDestroy(3);
	
	//__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}
	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("Error : %d"), theErr);
		}

	return result;
	}

/**
 * 
 * Alloc tests to Encode an image from bitmap to a file
 *	
 * Step 0709_CP to 0710_CP
 * 
 */
TVerdict CTestPositiveConversionNew::Step0709_0710_CPL()
	{
	//Traces
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0709-CP")) == 0)
		{
		INFO_PRINTF1(_L("Alloc tests to Encode an image from bitmap to a file, specifying the MIME Type."));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0710-CP")) == 0)
		{
		INFO_PRINTF1(_L("Alloc tests to Encode an image from bitmap to a file, NOT specifying the MIME Type."));
		}
	
	//Check for Alloc Test Support
	__UHEAP_SETFAIL(RHeap::EFailNext, 1);
	TAny *testAllocSupport = User::Alloc(1);
	if (testAllocSupport!=NULL)
		{
		delete testAllocSupport;
		testAllocSupport=NULL;
		INFO_PRINTF1(_L("This build does not support ALLOC testing"));
		return EInconclusive;
		}
		
	//Initialise Variables
	TVerdict iAllocTestStepResult=EPass;
	TInt err = KErrNone;
	TBool badResult = EFalse;
	
	//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
	INFO_PRINTF1(_L("Executing the Test Method call Outside the FOR LOOP."));
	TRAP(err, iAllocTestStepResult = TestStep0709_0710_CPL());
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Encoding error, returned error code =  %d"), err);
		User::Leave(err);
		}
	else
		{
		//Check the iAllocTestStepResult
		if (iAllocTestStepResult != EPass)
			{
			badResult = ETrue;
			}
		}	
	
	TInt failCount = 1;
	TBool completed = EFalse;
	iAllocTestStepResult = EPass; // reinitialise
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for(;;)	
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
		INFO_PRINTF2(_L("Executing the Test Method call #%d."), failCount);
		TRAP(err, iAllocTestStepResult = TestStep0709_0710_CPL());
		
		completed = EFalse;
		if (err == KErrNone)
			{
			TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
			if (testAlloc == NULL)
				{
				reachedEnd = ETrue;
				failCount -= 1;
				}
			else
				{
				User::Free(testAlloc);	
				}			
			
			//Check the iAllocTestStepResult
			if (iAllocTestStepResult != EPass)
				{
				badResult = ETrue;
				}
			
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			{
			completed = ETrue;
			badResult = EFail;
			}			

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			{
			break; // exit loop
			}

		failCount++;
		}

	//failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc

	if (err != KErrNone || badResult)
		{
		if (badResult)
			{
			INFO_PRINTF3(_L("  Bad result with %d memory allocations tested. The Error returned is %d"), failCount, err);
			}
		else
			{
			INFO_PRINTF3(_L("  Error(%d) with %d memory allocations tested\n"), err, failCount);
			}
		iAllocTestStepResult = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("  Completed OK with %d memory allocations tested\n"), failCount);
		iAllocTestStepResult = EPass;
		}
		return iAllocTestStepResult;
	}

/**
 * 
 * Alloc test main test step to Encode an image from bitmap to a file
 *	
 * Step 0709_CP to 0710_CP
 * 
 */
TVerdict CTestPositiveConversionNew::TestStep0709_0710_CPL()
	{
	TInt theRes = KErrNone;
	
	//__MM_HEAP_MARK;

	//[ Connect RFs ]
	User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);
	User::LeaveIfError(iFs.ShareProtected());
	
	// [ Open the Destination File using RFile ]
	TFileName theDestinationFileName; 
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(iFileNameSrc.Left(iFileNameSrc.Length()-3));
	GetImageExtension(iImageTypeUid, theExtension);
	theDestinationFileName.Append(theExtension);

	INFO_PRINTF2(_L("Start : Open the file : %S by RFile"), &theDestinationFileName);
	User::LeaveIfError(iFile.Replace(iFs, theDestinationFileName, EFileWrite));
	CleanupClosePushL(iFile);
	INFO_PRINTF2(_L("End : Open the file : %S by RFile"), &theDestinationFileName);
	
	//[ Connect to RFbsSession ]
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	
	//[ Open the File for convert, using the 'FileNewL' APIs for each test step ]
	INFO_PRINTF2(_L("Start : Open the file : %S by CImageEncoder::FileNewL"), &theDestinationFileName);
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0709-CP")) == 0)
		{
		//INFO_PRINTF1(_L("Alloc tests to Encode an image from bitmap to a file, specifying the MIME Type."));
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, mimeType));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0710-CP")) == 0)
		{
		//INFO_PRINTF1(_L("Alloc tests to Encode an image from bitmap to a file, NOT specifying the MIME Type."));
		//The Image type has to be given for Decoder Implementation
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, CImageEncoder::EOptionNone, KNullUid, KNullUid, TUid::Uid(KJPGEncoderImplementationUidValue)));
		}
	User::LeaveIfError(theRes);

	INFO_PRINTF2(_L("End : Open the file : %S by CImageEncoder::FileNewL"), &theDestinationFileName);
	
	//[ Call the main Encode Test Method ]
	TRAPD(err, EncodeUsingEncoderL(iFileNameSrc));
	if (err != KErrNone)
	{
	delete iImageEncoder;
	iImageEncoder = NULL;

	User::Leave(err);
	}
	
	//[ Delete and Close ImageEncoder ]

	delete iImageEncoder;
	iImageEncoder = NULL;
	
	//[ Delete and Close RFs, RFile and fbsSession]
	CleanupStack::PopAndDestroy(3);
	
	//__MM_HEAP_MARKEND;

	return EPass;
	}



/**
 * 
 * Alloc tests to Read, decode and verify JPEG.
 *	
 * Step 0711_CP to 0712_CP
 * 
 */
TVerdict CTestPositiveConversionNew::Step0711_0712_CPL()
	{
	//Traces
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0711-CP")) == 0)
		{
		INFO_PRINTF1(_L("Alloc tests to Read, decode and verify JPEG, specifying the MIME Type."));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0712-CP")) == 0)
		{
		INFO_PRINTF1(_L("Alloc tests to Read, decode and verify JPEG, NOT specifying the MIME Type."));
		}
		
	//Check for Alloc Test Support
	__UHEAP_SETFAIL(RHeap::EFailNext, 1);
	TAny *testAllocSupport = User::Alloc(1);
	if (testAllocSupport!=NULL)
		{
		delete testAllocSupport;
		testAllocSupport=NULL;
		INFO_PRINTF1(_L("This build does not support ALLOC testing"));
		return EInconclusive;
		}
	
	//Initialise Variables
	TVerdict iAllocTestStepResult=EPass;
	TInt err = KErrNone;
	TBool badResult = EFalse;
	
	//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
	INFO_PRINTF1(_L("Executing the Test Method call Outside the FOR LOOP."));
	TRAP(err, iAllocTestStepResult = TestStep0711_0712_CPL());
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Decoding error, returned error code =  %d"), err);
		User::Leave(err);
		}
	else
		{
		//Check the iAllocTestStepResult
		if (iAllocTestStepResult != EPass)
			{
			badResult = ETrue;
			}
		}
	
	TInt failCount = 1;
	TBool completed = EFalse;
	iAllocTestStepResult = EPass; // reinitialise
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for(;;)	
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		//__MM_HEAP_MARK;

		//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
		INFO_PRINTF2(_L("Executing the Test Method call #%d."), failCount);
		TRAP(err, iAllocTestStepResult = TestStep0711_0712_CPL());
		
		completed = EFalse;
		if (err == KErrNone)
			{
			TAny *testAlloc = NULL;
			testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
			if (testAlloc == NULL)
				{
				reachedEnd = ETrue;
				failCount -= 1;
				}
			else
				{
				User::Free(testAlloc);
				testAlloc=NULL;
				}			
			
			//Check the iAllocTestStepResult
			if (iAllocTestStepResult != EPass)
				{
				badResult = ETrue;
				}
			
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			{
			completed = ETrue;
			badResult = EFail;
			}			

		//__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			{
			break; // exit loop
			}

		failCount++;
		}

	//failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc

	if (err != KErrNone || badResult)
		{
		if (badResult)
			{
			INFO_PRINTF3(_L("  Bad result with %d memory allocations tested. The Error returned is %d"), failCount, err);
			}
		else
			{
			INFO_PRINTF3(_L("  Error(%d) with %d memory allocations tested\n"), err, failCount);
			}
		iAllocTestStepResult = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("  Completed OK with %d memory allocations tested\n"), failCount);
		iAllocTestStepResult = EPass;
		}
		return iAllocTestStepResult;
	}

/**
 * 
 * Alloc test main test step to Read, decode and verify JPEG.
 *	
 * Step 0711_CP to 0712_CP
 * 
 */
TVerdict CTestPositiveConversionNew::TestStep0711_0712_CPL()
	{
	TInt theRes = KErrNone;
	
	// dummy Hal call for heap balance on target
	//if (DummyHalCall() != KErrNone)
	//	return EInconclusive;
	
	//__MM_HEAP_MARK;
	//[ Connect RFs ]
	User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);
	User::LeaveIfError(iFs.ShareProtected());
	
	//[ Open the Source Image File using RFile ]
	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(iFileNameSrc);

	INFO_PRINTF2(_L("Start : Open the file : %S by RFile"), &theSourceFilename);
	theRes = iFile.Open(iFs, theSourceFilename, EFileWrite);
	User::LeaveIfError(theRes);
	CleanupClosePushL(iFile);
	INFO_PRINTF2(_L("End : Open the file : %S by RFile"), &theSourceFilename);
	
	//[ Connect to RFbsSession ]
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	//[ Open the File for convert, using the 'FileNewL' APIs for each test step ]
	INFO_PRINTF2(_L("Start : Open the file : %S by CImageDecoder::FileNewL"), &theSourceFilename);
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0711-CP")) == 0)
		{
		//INFO_PRINTF1(_L("Alloc tests to Read, decode and verify JPEG, specifying the MIME Type."));
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, mimeType, EPlay));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0712-CP")) == 0)
		{
		//INFO_PRINTF1(_L("Alloc tests to Read, decode and verify JPEG, NOT specifying the MIME Type."));
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, EPlay, CImageDecoder::EOptionNone, iImageTypeUid, KNullUid, TUid::Uid(KJPGDecoderImplementationUidValue)));
		}
	User::LeaveIfError(theRes);
	CleanupStack::PushL(iImageDecoder);
	INFO_PRINTF2(_L("End : Open the file : %S by CImageDecoder::FileNewL"), &theSourceFilename);
	
	//[ Call the main Decode Test Method ]
	User::LeaveIfError(DecodeUsingDecoderL(iFileNameSrc));
	
	//[ Delete and Close Local ImageDecoder ]
	CleanupStack::Pop();
	delete iImageDecoder;
	iImageDecoder = NULL;
	
	//[ Delete and Close RFs, RFile and fbsSession]
	CleanupStack::PopAndDestroy(3);
	
	//__MM_HEAP_MARKEND;

	return EPass;
	}

