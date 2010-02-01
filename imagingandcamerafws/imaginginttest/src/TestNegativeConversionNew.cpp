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
// This file contains the test steps ICL negative testing for GT0210 MS3.4 File Handle APIs.
// EPOC includes
// 
//

#include <testframework.h>
#include <caf/caf.h>
#include "TestNegativeConversionNew.h"

#include "ICLFbsSessionTest.h"

using namespace ContentAccess;

const TInt KMaxMimeLength = 256;

/**
 * 
 *
 * CTestNegativeConversionNew::NewL
 *
 */
CTestNegativeConversionNew *CTestNegativeConversionNew::NewL(const TDesC& aTestName)
	{
	CTestNegativeConversionNew* self = new(ELeave) CTestNegativeConversionNew(aTestName);
	return self;
	}

/**
 * 
 *
 * Implementation of Virtual Function CTestStep::DoTestStepL()
 *
 */
TVerdict CTestNegativeConversionNew::DoTestStepL()
	{
	TVerdict	currentVerdict = EPass;

	if( ( iTestStepName.Compare(_L("MM-ICL-FRM-I-1701-HP")) == 0) ||
		( iTestStepName.Compare(_L("MM-ICL-FRM-I-1702-HP")) == 0) ||
		( iTestStepName.Compare(_L("MM-ICL-FRM-I-1703-HP")) == 0) )
		{
		currentVerdict = Step1701_1703_HPL();
		}
	else if( ( iTestStepName.Compare(_L("MM-ICL-FRM-I-1704-HP")) == 0) ||
			 ( iTestStepName.Compare(_L("MM-ICL-FRM-I-1705-HP")) == 0) ||
			 ( iTestStepName.Compare(_L("MM-ICL-FRM-I-1706-HP")) == 0) )
		{
		currentVerdict = Step1704_1706_HPL();
		}
	
	return currentVerdict;
	}


/*
 * 
 * CImageDecoder Negative tests - OpenFile by Handles.
 *
 * Step1701_1702_HP
 * 
 */
TVerdict CTestNegativeConversionNew::Step1701_1703_HPL()
	{
	//[ Local variables ]
	TVerdict result = EFail;
	
	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		{
		return EInconclusive;
		}

	//__MM_HEAP_MARK;
	//[ Connect RFs ]
	User::LeaveIfError(iFs.Connect());
	User::LeaveIfError(iFs.ShareProtected());
	CleanupClosePushL(iFs);
	
	//[ Open the Source Image File using RFile ]
	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(iFileNameSrc);

	INFO_PRINTF2(_L("Start : Open the file : %S by RFile"), &theSourceFilename);
	TInt theRes = iFile.Open(iFs, theSourceFilename, EFileWrite);
	User::LeaveIfError(theRes);
	CleanupClosePushL(iFile);
	INFO_PRINTF2(_L("End : Open the file : %S by RFile"), &theSourceFilename);

	//[ Open the File for convert, using the 'FileNewL' APIs for each test step ]
	INFO_PRINTF2(_L("Start : Open the file : %S by CImageDecoder::FileNewL"), &theSourceFilename);
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1701-HP")) == 0)
		{
		INFO_PRINTF1(_L("Read, decode and verify JPEG, with an invalid MIME Type."));
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, mimeType, EPlay));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1702-HP")) == 0)
		{
		INFO_PRINTF1(_L("Read, decode and verify JPEG, with an invalid Image Type."));
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, EPlay, CImageDecoder::EOptionNone, iImageTypeUid, KNullUid, KNullUid));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1703-HP")) == 0)
		{
		INFO_PRINTF1(_L("Decode text file."));
		TRAP(theRes, iImageDecoder=CImageDecoder::FileNewL(iFile, EPlay));
		}
	INFO_PRINTF2(_L("End : Open the file : %S by CImageDecoder::FileNewL"), &theSourceFilename);
	
	//[ Checking for the Correct Error returned ]
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF1(_L("One or More Arguments are Invalid for 'CImageDecoder::FileNewL'. The Error Returned is -1 (KErrNotFound)"));
		result = EPass;
		}
	else
		{
		INFO_PRINTF1(_L("The Expected result is NOT returned for 'CImageDecoder::FileNewL'."));
		result = EFail;
		}
		
	//[ Delete and Close ImageEncoder ]
	delete iImageDecoder;
	iImageDecoder = NULL;
	
	//[ Delete and Close RFs and RFile ]
	CleanupStack::PopAndDestroy(2);
	
	return result;
	}

/*
 * 
 * CImageEncoder Negative tests - OpenFile by Handles.
 *
 * Step1704_1706_HP
 * 
 */
TVerdict CTestNegativeConversionNew::Step1704_1706_HPL()
	{
	//[ Local variables ]
	TVerdict result = EFail ;
	TInt theErr = KErrNone;
	
	//__MM_HEAP_MARK;
	//[ Connect RFs ]
	User::LeaveIfError(iFs.Connect());
	User::LeaveIfError(iFs.ShareProtected());
	CleanupClosePushL(iFs);
			
	// [ Open the Destination File using RFile ]
	TFileName theDestinationFileName; 
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(iFileNameSrc.Left(iFileNameSrc.Length()-3));
	GetImageExtension(iImageTypeUid, theExtension);
	theDestinationFileName.Append(theExtension);

	INFO_PRINTF2(_L("Start : Open the file : %S by RFile"), &theDestinationFileName);
	TInt theRes = iFile.Replace(iFs, theDestinationFileName, EFileWrite);
	User::LeaveIfError(theRes);
	CleanupClosePushL(iFile);
	INFO_PRINTF2(_L("End : Open the file : %S by RFile"), &theDestinationFileName);
	
	//[ Connect to RFbsSession ]
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		
	//[ Open the File for convert, using the 'FileNewL' APIs for each test step ]
	INFO_PRINTF2(_L("Start : Open the file : %S by CImageEncoder::FileNewL"), &theDestinationFileName);
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1704-HP")) == 0)
		{
		INFO_PRINTF1(_L("Encode an image from bitmap to a file, specifying an invalid MIME type."));
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, mimeType));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1705-HP")) == 0)
		{
		INFO_PRINTF1(_L("Encode an image from bitmap to a file, specifying an invalid image type."));
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, CImageEncoder::EOptionNone, KImageTypeGIFUid, KNullUid, KNullUid));
		}
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1706-HP")) == 0)
		{
		INFO_PRINTF1(_L("Encode to a jpg file containing text data."));
		//Image type or encoder UID should be there, otherwise throws panic..
		TRAP(theRes, iImageEncoder=CImageEncoder::FileNewL(iFile, CImageEncoder::EOptionNone, iImageTypeUid));
		}
	INFO_PRINTF2(_L("End : Open the file : %S by CImageEncoder::FileNewL"), &theDestinationFileName);
			
	//[ Checking for the Correct Error returned ]
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF1(_L("One or More Arguments are Invalid for 'CImageEncoder::FileNewL'. The Error Returned is -1 (KErrNotFound)"));
		result = EPass;
		}
	else if(theRes == KErrNone)
		{
		INFO_PRINTF1(_L("The Expected Error is NOT returned for 'CImageEncoder::FileNewL'. The Error Returned is 0 (KErrNone). So Proceeding to Encode."));
		INFO_PRINTF2(_L("The file : %S had been opened"), &theDestinationFileName);
		
		//[ Call the main Encode Test Method ]
		TRAP(theErr, EncodeUsingEncoderL(iFileNameSrc));
		
		//[ Check the Error ]
		if(theErr == KErrNone)
			{
			INFO_PRINTF1(_L("No Error is Returned after Encoding."));
			result = EFail;
			}
		else if(theErr == KErrNotIdentical)
			{
			INFO_PRINTF1(_L("The Error Returned after Encoding is KErrNotIdentical."));
			result = EPass;
			}
		}
	else
		{
		INFO_PRINTF2(_L("Unexpected Error : %d. The Expected Error is -1 (KErrNotFound)"), theRes);
		result = EInconclusive;
		}
			
	//[ Delete and Close ImageEncoder ]
	delete iImageEncoder;
	iImageEncoder = NULL;
	
	//[ Delete and Close RFs, RFile and fbsSession]
	CleanupStack::PopAndDestroy(3);
	
	//__MM_HEAP_MARKEND;

	return result;
	}

