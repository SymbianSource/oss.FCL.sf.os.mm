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
// This file contains the test steps ICL positive testing
// EPOC includes
// 
//

#include <testframework.h>
#include "TestPositiveConversion.h"

const TInt KMaxMimeLength = 256;

CTestPositiveConversion *CTestPositiveConversion::NewL(const TDesC& aTestName)
	{
	CTestPositiveConversion* self = new(ELeave) CTestPositiveConversion(aTestName);
	return self;

	}

/**
 * 
 *
 * do the test step
 *
 */
TVerdict CTestPositiveConversion::DoTestStepL()
	{
	TVerdict	currentVerdict = EPass;

	if( (iTestStepName.Compare(_L("MM-ICL-FRM-I-0001-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0002-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0003-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0004-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0005-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0006-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0007-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0008-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0009-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0010-CP")) == 0))
			currentVerdict = Step0001_0010_CP();
	else if( (iTestStepName.Compare(_L("MM-ICL-FRM-I-0011-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0012-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0013-CP")) == 0)   ||
		(iTestStepName.Compare(_L("MM-ICL-FRM-I-0014-CP")) == 0))  
			currentVerdict = Step0011_0014_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0015-CP")) == 0)
			currentVerdict = Step0015_CP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0200-CP")) == 0)
		currentVerdict = Step0200_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0016-CP")) == 0)
			currentVerdict = Step0016_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0017-CP")) == 0)
			currentVerdict = Step0017_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0101-LP")) == 0)
			currentVerdict = Step0101_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0102-LP")) == 0)
			currentVerdict = Step0102_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0103-LP")) == 0)
			currentVerdict = Step0103_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0104-LP")) == 0)
			currentVerdict = Step0104_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0105-LP")) == 0)
			currentVerdict = Step0105_LP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0106-HP")) == 0)
			currentVerdict = Step0106_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0107-HP")) == 0)
			currentVerdict = Step0107_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0109-HP")) == 0)
			currentVerdict = Step0109_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0110-HP")) == 0)
			currentVerdict = Step0110_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0111-HP")) == 0)
			currentVerdict = Step0111_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0112-HP")) == 0)
			currentVerdict = Step0112_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0113-CP")) == 0)
			currentVerdict = Step0113_CP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0114-HP")) == 0)
			currentVerdict = Step0114_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0115-HP")) == 0)
			currentVerdict = Step0115_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0300-HP")) == 0)
			currentVerdict = Step0300_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0600-HP")) == 0)
			currentVerdict = Step0600_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0601-HP")) == 0)
			currentVerdict = Step0601_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0608-HP")) == 0)
			currentVerdict = Step0608_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0609-HP")) == 0)
			currentVerdict = Step0609_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0610-HP")) == 0)
			currentVerdict = Step0610_HP();
	else if( iTestStepName.Compare(_L("MM-ICL-FRM-I-0611-HP")) == 0)
			currentVerdict = Step0611_HP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0612-HP")) == 0 )
			currentVerdict = Step0612_HP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0613-HP")) == 0 )
			currentVerdict = Step0613_HP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0614-HP")) == 0 )
			currentVerdict = Step0614_HP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0615-HP")) == 0 )
			currentVerdict = Step0615_HP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0616-HP")) == 0 )
			currentVerdict = Step0616_HP();
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0617-HP")) == 0 )
			currentVerdict = Step0617_HP();
	
	return currentVerdict;
	}
	
/**
 * 
 * Read, decode and verify different graphic formats
 *	
 * Step 0001_CP for 0010_CP
 * 
 */
TVerdict	CTestPositiveConversion::Step0001_0010_CP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	__MM_HEAP_MARK;
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

	if(iImageTypeUid == KImageTypeWBMPUid ||
		iImageTypeUid == KImageTypeOTAUid)
		{
		INFO_PRINTF2(_L("This type graphic demands UId %d"), &iImageTypeUid);
		TRAP(theErr1, theErr = OpenImageForDecodeFromFileL(iFileNameSrc, iImageTypeUid));
		}
	else
		{
		TRAP(theErr1, theErr = OpenImageForDecodeFromFileL(iFileNameSrc));
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), theErr);

	return result;
	}

/**
 * 
 * Encode and verify different graphic formats
 * 
 */
TVerdict	CTestPositiveConversion::Step0011_0014_CP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = EncodeImageToMemoryL(iFileNameSrc, iImageTypeUid));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone && theErr1 == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), theErr);

	return result;

	}


/**
 * 
 * Encode an image from memory to
 * a file  
 * 
 */
TVerdict	CTestPositiveConversion::Step0015_CP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = EncodeImageFromBitmapToFileL(iFileNameSrc, iImageTypeUid));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), theErr);
	
	return result;
	}

/**
 * 
 * Encode an image from memory to
 * a file using minimum parameters
 * 
 */
TVerdict CTestPositiveConversion::Step0200_CP()
	{
	TInt err = KErrNone;
	TInt err2 = KErrNone;

	// BMP
	__MM_HEAP_MARK;
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	iImageTypeUid = KImageTypeBMPUid;
	TRAP(err, err2 = EncodeImageWithMinParamsL(iFileNameSrc, iImageTypeUid));
	if (err != KErrNone || err2 != KErrNone)
		{
		INFO_PRINTF3(_L("BMP EncodeImageWithMinParamsL failed, err = %d, err2 = %d"), err, err2);
		return EFail;
		}
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	INFO_PRINTF1(_L("BMP encode successful"));

	// GIF
	__MM_HEAP_MARK;
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	iImageTypeUid = KImageTypeGIFUid;
	TRAP(err, err2 = EncodeImageWithMinParamsL(iFileNameSrc, iImageTypeUid));
	if (err != KErrNone || err2 != KErrNone)
		{
		INFO_PRINTF3(_L("GIF EncodeImageWithMinParamsL failed, err = %d, err2 = %d"), err, err2);
		return EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	INFO_PRINTF1(_L("GIF encode successful"));

	// JPEG
	__MM_HEAP_MARK;
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	iImageTypeUid = KImageTypeJPGUid;
	TRAP(err, err2 = EncodeImageWithMinParamsL(iFileNameSrc, iImageTypeUid));
	if (err != KErrNone || err2 != KErrNone)
		{
		INFO_PRINTF3(_L("JPEG EncodeImageWithMinParamsL failed, err = %d, err2 = %d"), err, err2);
		return EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	INFO_PRINTF1(_L("JPEG encode successful"));

	// MBM
	__MM_HEAP_MARK;
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	iImageTypeUid = KImageTypeMBMUid;
	TRAP(err, err2 = EncodeImageWithMinParamsL(iFileNameSrc, iImageTypeUid));
	if (err != KErrNone || err2 != KErrNone)
		{
		INFO_PRINTF3(_L("MBM EncodeImageWithMinParamsL failed, err = %d, err2 = %d"), err, err2);
		return EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	INFO_PRINTF1(_L("MBM encode successful"));

	// PNG
	__MM_HEAP_MARK;
	err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	iImageTypeUid = KImageTypePNGUid;
	TRAP(err, err2 = EncodeImageWithMinParamsL(iFileNameSrc, iImageTypeUid));
	if (err != KErrNone || err2 != KErrNone)
		{
		INFO_PRINTF3(_L("PNG EncodeImageWithMinParamsL failed, err = %d, err2 = %d"), err, err2);
		return EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	INFO_PRINTF1(_L("PNG encode successful"));
	
	return EPass;
	}

TInt CTestPositiveConversion::EncodeImageWithMinParamsL(const TDesC& aFilename, TUid aImageTypeId)
	{
	// Tests call to Convert with minimum parameters, allowing default params to be used.
	//
	TFileName theSourceFile;
	Directory(EReference, theSourceFile);
	theSourceFile.Append(aFilename);

	CFbsBitmap	*theBitmap = new(ELeave) CFbsBitmap;
	TInt theRes = theBitmap->Load(theSourceFile);
	
	if(theRes != KErrNone)
		{
		delete theBitmap;
		theBitmap = NULL;

		ERR_PRINTF2(_L("File %S cannot be louded" ), &theSourceFile); 
		return theRes;
		}
	User::LeaveIfError(iFs.Connect());

	// call for a convertor
	TFileName	theDestinationFileName; 
	TBuf<KLenBuffer>		theExtinsion;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(aFilename.Left(aFilename.Length()-3));

	GetImageExtension(aImageTypeId, theExtinsion);
	theDestinationFileName.Append(theExtinsion);
	
	TRAP(theRes, iImageEncoder = CImageEncoder::FileNewL(iFs,
														 theDestinationFileName, 
														 CImageEncoder::EOptionNone,
														 aImageTypeId));

	if(theRes != KErrNone)
		{
		delete theBitmap;
		theBitmap = NULL;

		iFs.Close();
		ERR_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), aImageTypeId, &theDestinationFileName); 
		if(KErrPathNotFound == theRes)
			ERR_PRINTF2(_L("Path %S not found" ), &theDestinationFileName); 
		
		if(theRes == KErrAccessDenied)
			ERR_PRINTF1(_L("Access denied" )); 

		return theRes;	
		}

	//prepare encode data
//  TO DO
//  Do we really need TConvertInfo here? Need some investigation to decide whether 
//  to use or delete it
//
//	TConvertInfo	theConvInfo;				
//	theConvInfo.iImageTypeId = iImageTypeUid;
//	theConvInfo.iDisplayMode = theBitmap->DisplayMode();

	// do conversion
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	iActiveScheduler->SetEncoder(iImageEncoder, activeListener, 0);
	iActiveScheduler->SetStep(this);

	activeListener->InitialiseActiveListener();
	iImageEncoder->Convert(&activeListener->iStatus, *theBitmap);
	CActiveScheduler::Start();
	

	theRes = activeListener->iStatus.Int();
	iActiveScheduler->SetStep(NULL);

	//delete all local value 
	delete iImageEncoder;
	iImageEncoder = NULL;

	delete theBitmap;
	theBitmap = NULL;

	CleanupStack::PopAndDestroy(1); //active sheduler

	if(theRes != KErrNone)
		{
		ERR_PRINTF1(_L("Error during conversion" )); 
		return theRes;
		}
	else
		{
		ERR_PRINTF1(_L("Conversion was successful" )); 
		}

	iFs.Close();

	return theRes; 
	}

/**
 * 
 * Read and decode an image from a
 * a descriptor
 * 
 */
TVerdict	CTestPositiveConversion::Step0016_CP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = OpenImageForDecodeFromBufferL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), theErr);
	

	return result;
	}

/**
 * 
 * Read and decode an image incrementally
 * 
 */
TVerdict	CTestPositiveConversion::Step0017_CP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = ReadAndDecodeIncrementallyL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), theErr);
	

	return result;

	}

//-------
/**
 * 
 * List available decoder MIME types
 * 
 */
TVerdict	CTestPositiveConversion::Step0101_LP()
	{
	TVerdict result = EFail ;
	TInt	theErr;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	theErr = ListAvailableMIMEDecoders();

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("List of decoders MIME types is not available,Error : %d"), theErr);
	
	return result;
	}

/**
 * 
 * List available decoders
 * 
 */
TVerdict	CTestPositiveConversion::Step0102_LP()
	{
	TVerdict result = EFail ;
	TInt	theErr;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	theErr = ListAllAvailableDecoders();

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("List of decoders is not available,Error : %d"), theErr);

	return result;
	}

/**
 * 
 * List available encoder MIME types
 * 
 */
TVerdict	CTestPositiveConversion::Step0103_LP()
	{
	TVerdict result = EFail ;
	TInt	theErr;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	theErr = ListAvailableMIMEEncoders();

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("List of encoders MIME types is not available, Error : %d"), theErr);

	return result;
	}

/**
 * 
 * List available encoders
 * 
 */
TVerdict	CTestPositiveConversion::Step0104_LP()
	{
	TVerdict result = EFail ;
	TInt	theErr;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	theErr = ListAllAvailableEncoders();

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;


	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("List of encoders is not available, Error : %d"), theErr);
	return result;
	}

/**
 * 
 * Enquire MIME type of an unopened image
 * 
 */
TVerdict	CTestPositiveConversion::Step0105_LP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = FindMimeTypeL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("MIME type of an unopened image is not available, Error : %d"), theErr);
	return result;
	}

/**
 * 
 * Enquire UID of a loaded decoder
 * 
 */
TVerdict	CTestPositiveConversion::Step0106_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = EnquireDecoderUIDAndImplementationInfoL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("UID of a loaded decoder is not available, Error : %d"), theErr);
	return result;
	}

/**
 * 
 * Enquire UID of a loaded encoder
 * 
 */
TVerdict	CTestPositiveConversion::Step0107_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = EnquireEncoderUIDL(iFileNameSrc, iImageTypeUid));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("UID of a loaded encoder is not available, Error : %d"), theErr);
	return result;
	}

//-----
/**
 * 
 * Request number of frames from a loaded image
 * 
 */
TVerdict	CTestPositiveConversion::Step0109_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = RequestNumberFramesL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Number of frame is not available, Error : %d"), theErr);
	return result;
	}

/**
 * 
 * Request frame information for frames in a loaded image
 * 
 */
TVerdict	CTestPositiveConversion::Step0110_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = RequestDecodeDataL(iFileNameSrc, KShowFrameInformation));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Number of frame is not available, Error : %d"), theErr);
	return result;
	}

/**
 * 
 * Request decode options for frames in a loaded image
 * 
 */
TVerdict	CTestPositiveConversion::Step0111_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = RequestDecodeDataL(iFileNameSrc, KShowFrameOption));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("decode options for frames are not available, Error : %d"), theErr);
	return result;

	}

/**
 * 
 * Request frame information for frames
 * in an animated GIF
 * 
 */
TVerdict	CTestPositiveConversion::Step0112_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = RequestDecodeDataL(iFileNameSrc, KShowFrameOption | KShowFrameInformation));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("frame information is sufficiently complete to define an animated GIF"));
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("frame information is not available, Error : %d"), theErr);
	return result;
	}

/**
 * 
 * Encode an image specifying extra
 * encode parameters
 * 
 */
TVerdict	CTestPositiveConversion::Step0113_CP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = EncodeImageToMemoryL(iFileNameSrc, iImageTypeUid));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error during encode an image specifying extra parameters, Error : %d"), theErr);
	return result;

	}

/**
 * 
 * Open image and retrieve embedded image comments
 * 
 */
TVerdict	CTestPositiveConversion::Step0114_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = RetrieveEmbeddedCommentsL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error during retrieving embedded image comments, Error : %d"), theErr);
	return result;

	}

/**
 * 
 * Open image and retrieve frame info
 * 
 */
TVerdict	CTestPositiveConversion::Step0115_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;

	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAPD(theErr1, theErr = RetrieveFrameInfoL(iFileNameSrc));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(theErr1 != KErrNone)
		theErr = theErr1;

	if(theErr == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error during retrieving frame info, Error : %d"), theErr);
	return result;

	}


/**
	PDEF117879 : ICL unable to load MBM
	
	Test description: Attempt to decode frame 8 from a buffer source (read from
	mbm_datanewl_0300.uic) and save this to a file.  File is compared with the 
	reference image mbm_datanewl_0300.mbm.
	
	The input image has an extension .uic (although it is an .mbm) in order to
	force ICL to recognise the correct codec plugin.
	
	The input file contains a mixture of colour-depths and compression levels,
	which results in frame #8 (12bpp compressed) falling in between a half-word
	boundary.
	
	Uses OpenBufferedImage()
 */
TVerdict CTestPositiveConversion::Step0300_HP()
	{
	TVerdict result = EFail;
		
	__MM_HEAP_MARK;
	TInt err = RFbsSession::Connect();
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
		return EInconclusive;
		}

	TRAP(err, OpenBufferedImageL(iFileNameSrc, 8)); // decode 8th frame

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("Error : %d"), err);
		}

	return result;
	}


/**
 * 
 * Read and Decoder using FileNewL with DRM intent API
 *	
 * Step 0600_HP
 * 
 */
TVerdict	CTestPositiveConversion::Step0600_HP()
	{
	TVerdict result = EFail;
	TInt err = KErrNone;
	
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

	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView));

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), err);

	return result;
	}

/**
 * 
 * Read and Decoder using FileNewL with DRM intent API
 *	
 * Step 0601_HP
 * 
 */
TVerdict	CTestPositiveConversion::Step0601_HP()
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

	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, mimeType));
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), err);

	return result;
	}



/**
 * 
 * Read and Decode DRM protected Image using FileNewL with DRM intent API
 *	
 * Step 0608_HP
 * 
 */
TVerdict	CTestPositiveConversion::Step0608_HP()
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

	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView));
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), err);

	return result;
	}

/**
 * 
 * Read and Decode DRM protected Image using FileNewL with DRM intent API, specifying mime type
 *	
 * Step 0609_HP
 * 
 */
TVerdict	CTestPositiveConversion::Step0609_HP()
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

	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, mimeType));
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), err);

	return result;
	}


/**
 * 
 * Read and Decode DRM protected Image using FileNewL with DRM intent API, image type uid
 *	
 * Step 0610_HP
 * 
 */
TVerdict	CTestPositiveConversion::Step0610_HP()
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

	TBuf8<KMaxMimeLength> mimeType;
	ReadMimeTypeFromConfig(mimeType);
	
	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, iImageTypeUid));
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), err);

	return result;
	}

/**
 * 
 * Read and Decode DRM protected Image using FileNewL with DRM intent API, specifying plugin uid
 *	
 * Step 0611_HP
 * 
 */
TVerdict	CTestPositiveConversion::Step0611_HP()
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

	TUid decoderUid;
	ReadDecoderUidFromConfig(decoderUid);

	TRAP(err, OpenImageForDecodeFromFileWithIntentL(iFileNameSrc, EView, iImageTypeUid, decoderUid));
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;

	if(err == KErrNone)
		{
		result = EPass;
		}
	else
		INFO_PRINTF2(_L("Error : %d"), err);

	return result;
	}
	
/**
 * 
 * Read and Decode PPM protected Image using FileNewL with specifying PPM1 plugin uid and file name 
 *	
 * Step 0612_HP
 * 
 */
TVerdict CTestPositiveConversion::Step0612_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	//PPM1	
	__MM_HEAP_MARK;
	theErr1 = RFbsSession::Connect();
	if (theErr1 != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), theErr1);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = OpenPpmImageForDecodeFromFileL(iFileNameSrc, iImageTypeUid, TUid::Uid(KPpm1DecoderImplementationUidValue)));
	if (theErr1 != KErrNone || theErr != KErrNone)
		{
		INFO_PRINTF3(_L("OpenPpmImageForDecodeFromFileL failed, err = %d, err = %d"), theErr1, theErr);
		result = EFail;
		}
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("PPM1 Codec decoding was successful with specifying Uid and file"));
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
 * Read and Decode PPM protected Image using FileNewL with specifying PPM2 plugin uid and file name 
 *	
 * Step 0613_HP
 * 
 */
TVerdict CTestPositiveConversion::Step0613_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	//PPM2
	__MM_HEAP_MARK;
	theErr1 = RFbsSession::Connect();
	if (theErr1 != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), theErr1);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = OpenPpmImageForDecodeFromFileL(iFileNameSrc, iImageTypeUid, TUid::Uid(KPpm2DecoderImplementationUidValue)));
	if (theErr1 != KErrNone || theErr != KErrNone)
		{
		INFO_PRINTF3(_L("OpenPpmImageForDecodeFromFileL failed, err = %d, err = %d"), theErr1, theErr);
		result = EFail;
		}
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("PPM2 Codec decoding was successful with specifying Uid and file"));
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
 * Read and Decode PPM protected Image using FileNewL with out specifying any plugin uid and with specifying file name only
 *	
 * Step 0614_HP
 * 
 */
TVerdict CTestPositiveConversion::Step0614_HP()
	{
	TVerdict result = EFail ;
	TInt	theErr = KErrNone;
	TInt	theErr1 = KErrNone;

	// dummy Hal call for heap balance on target
	if (DummyHalCall() != KErrNone)
		return EInconclusive;
	
	//PPM	
	__MM_HEAP_MARK;
	theErr1 = RFbsSession::Connect();
	if (theErr1 != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), theErr1);
		return EInconclusive;
		}

	TRAP(theErr1, theErr = OpenPpmImageForDecodeFromFileL(iFileNameSrc, iImageTypeUid));
	if (theErr1 != KErrNone || theErr != KErrNone)
		{
		INFO_PRINTF3(_L("OpenPpmImageForDecodeFromFileL failed, err = %d, err = %d"), theErr1, theErr);
		result = EFail;
		}
	
	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("PPM Codec decoding was successful with specifying file name"));
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
 * Read and Encode PPM protected Image using FileNewL with specifying PPM1 plugin uid and file name 
 *	
 * Step 0615_HP
 * 
 */
TVerdict CTestPositiveConversion::Step0615_HP()
	{
	TVerdict result = EFail ;
	TInt theErr = KErrNone;
	TInt theErr1 = KErrNone;

	// PPM1
	__MM_HEAP_MARK;
	theErr = RFbsSession::Connect();
	if (theErr != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), theErr);
		return EInconclusive;
		}
		
	TRAP(theErr1, theErr = EncodePpmImageParamsL(iFileNameSrc, iImageTypeUid , TUid::Uid(KPpm1EncoderImplementationUidValued)));
	if (theErr1 != KErrNone || theErr != KErrNone)
		{
		INFO_PRINTF3(_L("EncodePpmImageWithMinParamsL failed with Uid, err = %d, err = %d"), theErr1, theErr);
		result = EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("PPM1 Codec encoding was successful with specifying Uid"));
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
 * Read and Encode PPM protected Image using FileNewL with specifying PPM2 plugin uid and file name 
 *	
 * Step 0616_HP
 * 
 */
TVerdict CTestPositiveConversion::Step0616_HP()
	{
	TVerdict result = EFail;
	TInt theErr = KErrNone;
	TInt theErr1 = KErrNone;
	
	// PPM2	
	__MM_HEAP_MARK;
	theErr = RFbsSession::Connect();
	if (theErr != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), theErr);
		return EInconclusive;
		}
		
	TRAP(theErr1, theErr = EncodePpmImageParamsL(iFileNameSrc, iImageTypeUid, TUid::Uid(KPpm2EncoderImplementationUidValue)));
	if (theErr1 != KErrNone || theErr != KErrNone)
		{
		INFO_PRINTF3(_L("EncodePpmImageWithMinParamsL failed with Uid, err = %d, err = %d"), theErr1, theErr);
		result = EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("PPM2 Codec encoding was successful with specifying Uid"));
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
 * Read and Encode PPM protected Image using FileNewL with out specifying any plugin uid and with specifying file name only
 *	
 * Step 0617_HP
 * 
 */
TVerdict CTestPositiveConversion::Step0617_HP()
	{
	TVerdict result = EFail;
	TInt theErr = KErrNone;
	TInt theErr1 = KErrNone;
	
	// PPM
	__MM_HEAP_MARK;
	theErr = RFbsSession::Connect();
	if (theErr != KErrNone)
		{
		INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), theErr);
		return EInconclusive;
		}
		
	TRAP(theErr1, theErr = EncodePpmImageParamsL(iFileNameSrc, iImageTypeUid));
	if (theErr != KErrNone || theErr1 != KErrNone)
		{
		INFO_PRINTF3(_L("EncodePpmImageWithMinParamsL failed with file, err = %d, err = %d"), theErr1, theErr);
		result = EFail;
		}

	RFbsSession::Disconnect();

	__MM_HEAP_MARKEND;
	if(theErr1 != KErrNone)
		{
		theErr = theErr1;
		}

	if(theErr == KErrNone)
		{
		INFO_PRINTF1(_L("PPM Codec encoding was successful with specifying mime type"));
		result = EPass;
		}
	else
		{
		INFO_PRINTF2(_L("Error : %d"), theErr);
		}

	return result;	
	}

TInt CTestPositiveConversion::EncodePpmImageParamsL(const TDesC& aFilename, TUid aImageTypeId, const TUid aKPpmEncoderUid)
	{
	// Tests call to Convert with minimum parameters, allowing default params to be used.
	//
	TFileName theSourceFile;
	Directory(EReference, theSourceFile);
	theSourceFile.Append(aFilename);

	CFbsBitmap	*theBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theBitmap);
	TInt theRes = theBitmap->Load(theSourceFile);
	
	if(theRes != KErrNone)
		{
		CleanupStack::PopAndDestroy(theBitmap); 
		theBitmap = NULL;

		INFO_PRINTF2(_L("File %S cannot be loaded" ), &theSourceFile); 
		return theRes;
		}

	User::LeaveIfError(iFs.Connect());

	// call for a convertor
	TFileName	theDestinationFileName; 
	TBuf<KLenBuffer>		theExtension;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(aFilename.Left(aFilename.Length()-3));

	GetImageExtension(aImageTypeId, theExtension);
	theDestinationFileName.Append(theExtension);
	
	if( aKPpmEncoderUid == TUid::Uid(KPpm1EncoderImplementationUidValued ))
		{
		TRAP(theRes, iImageEncoder = CImageEncoder::FileNewL(iFs,
														 theDestinationFileName, 
														 CImageEncoder::EOptionNone,
														 aImageTypeId, KNullUid,aKPpmEncoderUid));
		}
		
	else 
		if(aKPpmEncoderUid == TUid::Uid(KPpm2EncoderImplementationUidValue ))
			{
			TRAP(theRes, iImageEncoder = CImageEncoder::FileNewL(iFs,
														 theDestinationFileName, 
														 CImageEncoder::EOptionNone,
														 aImageTypeId, KNullUid,aKPpmEncoderUid));
			}
			
	else
		{
		TBuf8<KMaxMimeLength> mimeType;
		ReadMimeTypeFromConfig(mimeType);
		TRAP(theRes, iImageEncoder = CImageEncoder::FileNewL(iFs,
														 theDestinationFileName,
														 mimeType, 
														 CImageEncoder::EOptionNone));
		}

	if(theRes != KErrNone)
		{
		CleanupStack::PopAndDestroy(theBitmap); 
		theBitmap = NULL;

		iFs.Close();
		INFO_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), aImageTypeId, &theDestinationFileName); 
		if(theRes == KErrPathNotFound)
			{
			INFO_PRINTF2(_L("Path %S not found" ), &theDestinationFileName); 
			}
		if(theRes == KErrAccessDenied)
			{
			INFO_PRINTF1(_L("Access denied" )); 
			}

		return theRes;	
		}

	//prepare encode data

	// do conversion
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	iActiveScheduler->SetEncoder(iImageEncoder, activeListener, 0);
	iActiveScheduler->SetStep(this);

	activeListener->InitialiseActiveListener();
	iImageEncoder->Convert(&activeListener->iStatus, *theBitmap);
	CActiveScheduler::Start();
	
	theRes = activeListener->iStatus.Int();
	iActiveScheduler->SetStep(NULL);

	//delete all local value 
	delete iImageEncoder;
	iImageEncoder = NULL;

	theBitmap->Reset();

	CleanupStack::PopAndDestroy(2); //active sheduler, theBitmap
	
	iFs.Close();
	
	if(theRes != KErrNone)
		{
		INFO_PRINTF1(_L("Error during conversion" )); 
		return theRes;
		}
	else
		{
		INFO_PRINTF1(_L("Conversion was successful" )); 
		}

	return theRes; 
	}
	

	



