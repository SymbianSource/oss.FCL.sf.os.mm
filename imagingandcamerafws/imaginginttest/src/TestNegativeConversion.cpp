// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the test steps ICL negative testing
// EPOC includes
// 
//

#include <testframework.h>
#include <caf/caf.h>
#include "TestNegativeConversion.h"

#include "rightsdatabase.h"

using namespace ContentAccess;

const TInt KMaxMimeLength = 256;

const TInt	KErrEncoderNonAvailable = -17014;//this error raises whenever you try to use 
											//unavaliable plugin 

//const TInt  KLenBufferSectionName = 64;	// EABI warning removal
//const TInt	KLenMemBuffer	= 1024;		// EABI warning removal

// DRM 
const TInt KDefaultRightsID = 1;
const TInt KDefaultRightsCount = 2;
const TInt KRightsNoCount = -1;


CTestNegativeConversion *CTestNegativeConversion::NewL(const TDesC& aTestName)
	{
	CTestNegativeConversion* self = new(ELeave) CTestNegativeConversion(aTestName);
	return self;

	}

/**
 * 
 *
 * do the test step
 *
 */
TVerdict CTestNegativeConversion::DoTestStepL()
	{
	TVerdict	currentVerdict = EPass;

	if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0501-CP")) == 0)
		currentVerdict = Step0501_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0502-LP")) == 0)
		currentVerdict = Step0502_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0503-HP")) == 0)
		currentVerdict = Step0503_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0504-HP")) == 0)
		currentVerdict = Step0504_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0505-HP")) == 0)
		currentVerdict = Step0505_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0506-LP")) == 0)
		currentVerdict = Step0506_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0507-LP")) == 0)
		currentVerdict = Step0507_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0508-HP")) == 0)
		currentVerdict = Step0508_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0509-LP")) == 0)
		currentVerdict = Step0509_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0510-LP")) == 0)
		currentVerdict = Step0510_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0511-LP")) == 0)
		currentVerdict = Step0511_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0512-LP")) == 0)
		currentVerdict = Step0512_LP_L();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0513-HP")) == 0)
		currentVerdict = Step0513_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0602-HP")) == 0)
		currentVerdict = Step0602_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0603-HP")) == 0)
		currentVerdict = Step0603_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0604-HP")) == 0)
		currentVerdict = Step0604_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0605-HP")) == 0)
		currentVerdict = Step0605_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0606-HP")) == 0)
		currentVerdict = Step0606_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0607-HP")) == 0)
		currentVerdict = Step0607_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0020-CP")) == 0)
		currentVerdict = Step0020_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0700-CP")) == 0)
        currentVerdict = Step0700_CP();
	
	return currentVerdict;
	}

/**
 * 
 * Request an encode for
 * usupported image format
 */
TVerdict	CTestNegativeConversion::Step0501_CP()
	{
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;

	TRAP_IGNORE(theRes = EncodeImageToMemoryL(iFileNameSrc, 
										iImageTypeUid));
		
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error during request unsupported image format, error %d\n"),
					theRes);

		result = EPass;
		}
			
	return result;

	}

/**
 * 
 * Request an encode for supported
 * image format but usupported 
 * conversion type
 * 
 *	This test was removed. Cannot specify unsupported conversion type!!! (for ex. GIF89a)
 */

TVerdict	CTestNegativeConversion::Step0502_LP()
	{

	// we have no subtypes specified for GIF
	INFO_PRINTF1(_L("Warning : test not valid - ICL has no subtypes specified but not implemented"));
	return EPass;

/*	TVerdict result = EPass ;
	TInt	theRes = KErrNotSupported;

	TRAP(theRes, theRes = EncodeImageToMemoryL(iFileNameSrc, 
										iImageTypeUid));
		
	if(theRes == KErrNone)
		result = EPass;
	
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error during request unsupported image format, error %d\n"),
					theRes);

		}

	return result;
*/
	}


/**
 * 
 * Request load and encode 
 * of corrupt image 
 * 
 */
TVerdict	CTestNegativeConversion::Step0503_HP()
	{
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;

	TInt err = FbsStartup();
	if (err != KErrNone)
        {
        INFO_PRINTF2(_L("FbsStartup failed, err = %d"), err);
        return EInconclusive;
        }

	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	//iImageTypeUid is not according to file format
	TRAP_IGNORE(theRes = OpenImageForDecodeFromBufferL(iFileNameSrc, iImageTypeUid));

	RFbsSession::Disconnect();

	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error during load and decode of corrupt image, error %d\n"),
					theRes);

			result = EPass;
		}
	
	return result;
	}

/**
 * 
 * Request load and decode 
 * of corrupt WMF image 
 * 
 */
TVerdict	CTestNegativeConversion::Step0020_CP()
	{
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;

    TInt err = FbsStartup();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("FbsStartup failed, err = %d"), err);
        return EInconclusive;
        }

    err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

	//try to open corrupted file
	TRAP_IGNORE(theRes = OpenImageForDecodeFromBufferL(iFileNameSrc, iImageTypeUid));

	RFbsSession::Disconnect();

	if(theRes == KErrCorrupt)
		{
		INFO_PRINTF2(_L("Error during load and decode of corrupt image, error %d\n"),
					theRes);

		result = EPass;	
		}

	

	return result;
	}


/**
 * 
 * Request load and decode of 
 * image from which content type
 * cannot be identified
 * 
 */
TVerdict	CTestNegativeConversion::Step0504_HP()
	{
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;

    TInt err = FbsStartup();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("FbsStartup failed, err = %d"), err);
        return EInconclusive;
        }

    err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

	TRAP_IGNORE(theRes = OpenImageForDecodeFromBufferL(iFileNameSrc));
		
	RFbsSession::Disconnect();

	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error decode cropped image format, error %d\n"),
					theRes);

		result = EPass;
		}

	return result;
	}
/**
 * 
 * Request load of non-existent 
 * image 
 * 
 */
TVerdict	CTestNegativeConversion::Step0505_HP()
	{
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;

	TRAP_IGNORE(theRes = OpenImageForDecodeFromFileL(iFileNameSrc));
	
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error load of non-existent image, error %d\n"),
					theRes);
		result = EPass;

		}
	return result;
	}

/**
 * 
 * Request incremental load and 
 * decode of an image, then break the connection
 * before completion
 *		Test was removed!!!. Every attempt to close file server causes crash
 */
TVerdict	CTestNegativeConversion::Step0506_LP()
	{
	INFO_PRINTF1(_L("Warning : test not valid - cannot break incremental load in Test Framework"));
	INFO_PRINTF1(_L("Run visual test in TImageViewer"));
	return EPass;

/*	TVerdict result = EFail ;
	TInt	theRes = KErrNone;
	TInt	theNumStepBeforeCancel = 0;	

	TBuf<KLenBufferSectionName>	nameSection;
	
	TPtrC ptr = iTestStepName;
	nameSection = ptr.Right(KLenNameInSection);

	GetIntFromConfig(nameSection, _L("NumStepBeforeCancel"), theNumStepBeforeCancel);
	
	
	TRAP(theRes, theRes = ReadAndDecodeIncrementallyL(iFileNameSrc, theNumStepBeforeCancel));
		
	if(theRes == KErrNone)
		result = EPass;
	
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error during break connection before completion, error %d\n"),
					theRes);

		}
		
	return result;
*/
	}

/**
 * 
 * Request incremental load and decode 
 * of an image, then delete the image at the server 
 * before completion
 *		Test was removed!!!. During reading and decoding image incrementally 
 *		cannot delete source file
 */
TVerdict	CTestNegativeConversion::Step0507_LP()
	{
	INFO_PRINTF1(_L("Warning : test not valid - cannot delete source image during decode in Test Framework"));
	INFO_PRINTF1(_L("Run visual test in TImageViewer"));
	return EPass;
/*
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;

	TRAP(theRes, theRes = OpenImageForDecodeFromFileL(iFileNameSrc));
		
	if(theRes == KErrNone)
		result = EPass;
	
	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Error during delete the image before completion, error %d\n"),
					theRes);

		}
	
	return result;
*/
	}

/**
 * 
 * Request incremental load and decode of an image, 
 * then cancel the incremental load at the client
 * before completion.
 *
 *	Test was removed!!!.
 *	Was replaced by MM-ICL-COD-U-08d4-HP
 *	
 */
TVerdict	CTestNegativeConversion::Step0508_HP()
	{
	INFO_PRINTF1(_L("Warning : Was replaced by MM-ICL-COD-U-08d4-HP"));
	return EFail;
/*
	TVerdict result = EFail ;
	TInt	theNumStepBeforeCancel = 0;	
	TInt	theRes = KErrNone;

	TBuf<KLenBuffer>	nameSection;
	
	TPtrC ptr = iTestStepName;
	nameSection = ptr.Right(KLenNameInSection);

	GetIntFromConfig(nameSection, _L("NumStepBeforeCancel"), theNumStepBeforeCancel);
	
	
	TRAPD(theRes1, theRes = ReadAndDecodeIncrementallyL(iFileNameSrc, theNumStepBeforeCancel));
		
	
	if(theRes == KErrCancel)
		{
		INFO_PRINTF2(_L("The incremental load and decode was canceled at the client before completion, error %d"),
					theRes);

		result = EPass;
		}
	else
		{
		
		if(theRes1 != KErrNone)
			theRes = theRes1;
		INFO_PRINTF2(_L("Unexpected error during request incremental load and decode of an image, error %d"),
					theRes);

		}

	return result;
	*/
	}

/**
 * 
 * List available encoders/decoders 
 * where no plugin are available
 * 
 */

TVerdict	CTestNegativeConversion::Step0509_LP()
	{
	TInt	theRes = KErrNone;
	TInt	theRes1 = KErrNone;

	theRes = ListAllAvailableEncoders();
	if(theRes == KErrEncoderNonAvailable)
		{
		INFO_PRINTF2(_L("List encoders plugin are not available, error %d\n"),
					theRes);

		}

	theRes1 = ListAllAvailableDecoders();
	if(theRes1 == KErrEncoderNonAvailable)
		{
		INFO_PRINTF2(_L("List decoders plugin are not available, error %d\n"),
					theRes1);

		}
	
	INFO_PRINTF1(_L("Warning : test not valid - cannot delete plugins / Ecom registry in Test Framework"));
	INFO_PRINTF1(_L("To test, run on clean system with no plugins installed"));
	return EPass;
/*
	TVerdict result = EFail ;
	if(theRes == KErrEncoderNonAvailable && theRes1 == KErrEncoderNonAvailable)
		result = EPass;
	
	return result;
*/
	}

/**
 * 
 * List MIME types / UIDs for encodes/decoders 
 * where no plugins are available
 * 
 */
TVerdict	CTestNegativeConversion::Step0510_LP()
	{
	TInt theRes = KErrNone;
	TInt theRes1 = KErrNone;
	
	__MM_HEAP_MARK;

	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	// request list of encoders
	theRes = ListAvailableMIMEEncoders();

	if(theRes == KErrEncoderNonAvailable)
		{
		INFO_PRINTF2(_L("A list of MIME types for encoders is not available, error %d\n"),
						theRes);

		}

	// request list of decoders
	theRes1 = ListAvailableMIMEDecoders();

	if(theRes1 == KErrEncoderNonAvailable)
		{
		INFO_PRINTF2(_L("A list of MIME types for decoders is not available, error %d\n"),
						theRes1);
		}
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	INFO_PRINTF1(_L("Warning : test not valid - cannot delete plugins / Ecom registry in Test Framework"));
	INFO_PRINTF1(_L("To test, run on clean system with no plugins installed"));
	return EPass;
/*
	TVerdict result = EFail ;
	if(theRes == KErrEncoderNonAvailable && theRes1 == KErrEncoderNonAvailable)
		result = EPass;

	return result;	
*/
	}

/**
 * 
 * Request a load and decode/encode 
 * from a corrupt plugin 
 * 
 */
TVerdict	CTestNegativeConversion::Step0511_LP()
	{
	INFO_PRINTF1(_L("Warning : test not valid - corrupt plugin not available"));
	return EPass;

/*	TVerdict result = EFail ;
	TInt	theRes = KErrNone;
	TInt	theRes1 = KErrNone;
	TInt	theRes2 = KErrNone;

	TRAP(theRes2, theRes = OpenImageForDecodeFromFileL(iFileNameSrc));

	if(theRes == KErrNotFound)
		{
		INFO_PRINTF2(_L("Decode plugin is not available, error %d\n"),
						theRes);

		}

	TBuf<KLenBuffer>				theNameEncode;
	TBuf<KLenBufferExtension>		theExtension;
	theNameEncode.Copy(iFileNameSrc.Left(iFileNameSrc.Length() - 3));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theNameEncode.Append(theExtension);

	TRAP(theRes2, theRes1 = EncodeImageFromBitmapToFileL(theNameEncode, KImageTypeJPGUid));

	if(theRes1 == KErrNotFound)
		{
		INFO_PRINTF2(_L("Encode plugin is not available, error %d\n"),
						theRes1);
		
		}
	if(theRes1 == KErrNotFound && theRes == KErrNotFound)
		result = EPass;
	

	return result;
*/
	}

/**
 * 
 * Attempt to encode something which is not
 * a CFbsBitmap
 * 
 */
TVerdict	CTestNegativeConversion::Step0512_LP_L()
	{

	INFO_PRINTF1(_L("Warning : test not valid"));
	return EPass;
/*
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;
	
	__MM_HEAP_MARK;
	
	TAny*	memBuf = NULL;
	TRAPD(theRes1, memBuf = User::AllocL(KLenMemBuffer));
	CleanupStack::PushL(memBuf);
	if(theRes1 != KErrNone)
		return result;

	TPtr8	buffer(STATIC_CAST(TUint8*, memBuf), 0, KLenMemBuffer);

	CFbsBitmap*	theBitmap = (CFbsBitmap*) &buffer;

	
	TRAP(theRes1, theRes = EncodeImageFromBitmapToMemoryL( iFileNameSrc, theBitmap, iImageTypeUid));

	// NB Encode Image may succeed in encoding, but the comparison will fail - then it will 
	// return KErrNotIdentical - this is also a valid return

	if(theRes == KErrCorrupt || theRes == KErrNotIdentical)
		{
		INFO_PRINTF2(_L("Error encoding random buffer, error %d"),
						theRes);

		result = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("Unexpected error during encoding random buffer, error %d"),
						theRes);

		}
	
	CleanupStack::PopAndDestroy();	// memBuf

	__MM_HEAP_MARKEND;
	return result;
*/
	}

/**
 * 
 * Request frame data where no frame is loaded
 * 
 */
TVerdict	CTestNegativeConversion::Step0513_HP()
	{
	TVerdict result = EFail ;
	TInt	theRes = KErrNone;
	
    TInt err = FbsStartup();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("FbsStartup failed, err = %d"), err);
        return EInconclusive;
        }

    err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

	TRAP_IGNORE(theRes = OpenImageForDecodeFromBufferL( iFileNameSrc ));

	RFbsSession::Disconnect();

	if(theRes == KErrUnderflow)
		{
		INFO_PRINTF2(_L("No frame is loaded, error %d\n"),
						theRes);

		result = EPass;
		}
	
	return result;

	}


/**
 * 
 * Read and Decoder using FileNewL with DRM intent API.
 * Negative Test, drm protected file with no rights
 *	
 * Step 0602_HP
 * 
 */
TVerdict	CTestNegativeConversion::Step0602_HP()
	{
	TVerdict result = EFail ;
	TInt err = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	__MM_HEAP_MARK;
    TInt err1 = FbsStartup();
    if (err1 != KErrNone)
        {
        INFO_PRINTF2(_L("FbsStartup failed, err = %d"), err1);
        return EInconclusive;
        }

    err1 = RFbsSession::Connect();
    if (err1 != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err1);
        return EInconclusive;
        }

	CRightsDatabase* database = CRightsDatabase::NewL(EFalse); // clear current database
	delete database; // delete database object

	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrCANoRights)
		{
		INFO_PRINTF1(_L("Expected Rights failure opening file"));
		result = EPass;
		}
	else
		INFO_PRINTF3(_L("Unexpected Error : %d, expected failure with %d"), err, KErrCANoRights);

	return result;
	}


/**
 * 
 * Read and Decoder using FileNewL with DRM intent API (specifying MIME Type)
 * Negative Test, drm protected file with no rights
 * Step 0603_HP
 * 
 */
TVerdict	CTestNegativeConversion::Step0603_HP()
	{
	TVerdict result = EFail ;
	TInt	err = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	__MM_HEAP_MARK;
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	CRightsDatabase* database = CRightsDatabase::NewL(EFalse); // clear current database
	delete database; // delete database object

	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, mimeType));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	
	if(err == KErrCANoRights)
		{
		INFO_PRINTF1(_L("Expected Rights failure opening file"));
		result = EPass;
		}
	else
		INFO_PRINTF3(_L("Unexpected Error : %d, expected failure with %d"), err, KErrCANoRights);


	return result;
	}

/**
 * 
 * Read and Decoder using FileNewL with DRM intent API.
 * Negative Test, drm protected file with rights count
 *	
 * Step 0604_HP
 * 
 */
TVerdict	CTestNegativeConversion::Step0604_HP()
	{

	TVerdict result = EFail ;
	TInt	err = KErrNone;
	
	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	// set the rights count
	CRightsDatabase* rights = CRightsDatabase::NewL(EFalse); // create new database overwriting current one
	CleanupStack::PushL(rights);
	rights->AddRightsL(KDefaultRightsID,KDefaultRightsCount); // add rights count 
	CleanupStack::PopAndDestroy(rights); // cleanup rights object

	//attempt KDefaultRightsCount times to open the file
	for (TInt i=0;i<KDefaultRightsCount;i++)
	{
		TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView));
	
		INFO_PRINTF2( _L("CImageDecoder: DRM FileNewL number %d"),i);
	
		if (err != KErrNone)
		{
			INFO_PRINTF2(_L("Unexpected error %d"),err);
			break;
		}
	}
	
	// now attempt to open again
	if (err == KErrNone)
	{
		TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView));
		
		INFO_PRINTF1( _L("CImageDecoder:DRM FileNewL with expired rights"));

		if (err != KErrCANoPermission)
		{
			INFO_PRINTF3(_L("Unexpected error %d, expected %d"),err,KErrCANoPermission);
		}
		else
		{
			INFO_PRINTF1(_L("Expected failure: rights denied"));
			result = EPass;
		}
	}


	RFbsSession::Disconnect();

	return result;
	}


/**
 * 
 * Read and Decoder using FileNewL with DRM intent API (specifying MIME Type)
 * Negative Test, drm protected file with rights count
 * Step 0605_HP
 * 
 */
TVerdict	CTestNegativeConversion::Step0605_HP()
	{
	TVerdict result = EFail ;
	TInt	err = KErrNone;
	
	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	//specify the MIME type
	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	// set the rights count
	CRightsDatabase* rights = CRightsDatabase::NewL(EFalse); // create new database overwriting current one
	CleanupStack::PushL(rights);
	rights->AddRightsL(KDefaultRightsID,KDefaultRightsCount); // add rights count 
	CleanupStack::PopAndDestroy(rights); // cleanup rights object

	//attempt KDefaultRightsCount times to open the file
	for (TInt i=0;i<KDefaultRightsCount;i++)
	{
		TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, mimeType));
	
		INFO_PRINTF2( _L("CImageDecoder: DRM FileNewL number %d"),i);
	
		if (err != KErrNone)
		{
			INFO_PRINTF2(_L("Unexpected error %d"),err);
			break;
		}
	}
	
	// now attempt to open again
	if (err == KErrNone)
	{
		TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, mimeType));
		
		INFO_PRINTF1( _L("CImageDecoder:DRM FileNewL with expired rights"));

		if (err != KErrCANoPermission)
		{
			INFO_PRINTF3(_L("Unexpected error %d, expected %d"),err,KErrCANoPermission);
		}
		else
		{
			INFO_PRINTF1(_L("Expected failure: rights denied"));
			result = EPass;
		}
	}


	RFbsSession::Disconnect();

	return result;
	}



/**
 * 
 * Read and Decoder using FileNewL with DRM intent API (specifying MIME Type)
 * Negative Test, drm protected file with rights count
 * Step 0606_HP
 * 
 */
TVerdict	CTestNegativeConversion::Step0606_HP()
	{
	TVerdict result = EFail ;
	TInt	err = KErrNone;
	
	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	//specify the MIME type
	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	// set the rights count
	CRightsDatabase* rights = CRightsDatabase::NewL(EFalse); // create new database overwriting current one
	CleanupStack::PushL(rights);
	rights->AddRightsL(KDefaultRightsID, KRightsNoCount, mimeType); // add rights count 
	CleanupStack::PopAndDestroy(rights); // cleanup rights object

	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView));

	if (err == KErrNone)
		{
		INFO_PRINTF1(_L("Successfully opened file, with mime type specified by DRM agent"));
		result = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("Unexpected error %d"),err);
		}

	RFbsSession::Disconnect();

	return result;
	}

/**
 * 
 * Read and Decoder using FileNewL with DRM intent API (specifying MIME Type)
 * Negative Test, drm protected file with rights count
 * Step 0607_HP
 * 
 */
TVerdict	CTestNegativeConversion::Step0607_HP()
	{
	TVerdict result = EFail ;
	TInt	err = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	//specify the MIME type
	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	// set the rights count
	CRightsDatabase* rights = CRightsDatabase::NewL(EFalse); // create new database overwriting current one
	CleanupStack::PushL(rights);
	rights->AddRightsL(KDefaultRightsID, KRightsNoCount, mimeType); // add rights count 
	CleanupStack::PopAndDestroy(rights); // cleanup rights object

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(iFileNameSrc);

	RFs fs;
	User::LeaveIfError(fs.Connect());

	TBuf8<KMaxMimeLength> returnedMimeType;

	TRAP(err, CImageDecoder::GetMimeTypeFileL(fs, theSourceFilename, returnedMimeType));
	
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Unexpected error %d while retrieving MIME type"), err);
		}
	else
		{
		if (mimeType.Compare(returnedMimeType)==0)
			{
			INFO_PRINTF1(_L("Mime type correctly retrieved from DRM agent"));
			result = EPass;
			}
		else
			{
			INFO_PRINTF3(_L("Mime type not returned correctly, expected %S, received %S"),&mimeType,&returnedMimeType);
			}
		}

	
	RFbsSession::Disconnect();

	return result;
	}

/**
 * 
 * Request load and decode 
 * of corrupt WMF image 
 * 
 */
TVerdict    CTestNegativeConversion::Step0700_CP()
    {

    TInt    theRes = KErrNone;
    
    HBufC8 *theDestinationImage = NULL;
    CImageEncoder *theImageEncoder = NULL;
    
   TInt err = RFbsSession::Connect();
   
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }
       
    TRAP(theRes, theImageEncoder = CImageEncoder::DataNewL( theDestinationImage, 
        CImageEncoder::EOptionNone, KImageTypeJPGUid ));
    
    if(theRes != KErrNone)
        {
        User::Leave(theRes);        
        }
    CleanupStack::PushL(theImageEncoder);
    
    
    
     // do conversion
    CActiveListener* activeListener = new(ELeave)CActiveListener;
    CleanupStack::PushL( activeListener );
    
    CFbsBitmap* extBitmap = new (ELeave) CFbsBitmap;
       
   CleanupStack::PushL(extBitmap);

       
   User::LeaveIfError(extBitmap->CreateExtendedBitmap(
           TSize(100,100),
           EColor16M,KImageTypeJPGUid,29143,*this));
    
    activeListener->InitialiseActiveListener();
    theImageEncoder->Convert(&activeListener->iStatus,*extBitmap);
    CActiveScheduler::Start();

    theRes = activeListener->iStatus.Int();
    
    if(theRes!=KErrNotSupported)
        {
        User::Leave(theRes);
        }

    INFO_PRINTF1(_L("Got KErrNotSupported as expected"));
    //
    delete theDestinationImage;
    theDestinationImage = NULL;

    CleanupStack::PopAndDestroy(3, theImageEncoder);
    
    RFbsSession::Disconnect();
    return EPass;
    }

TInt CTestNegativeConversion::InitExtendedBitmap(TAny* /*aData*/, TInt /*aDataSize*/)
    {
    return KErrNone;
    }
