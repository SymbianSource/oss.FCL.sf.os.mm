// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Example CTestStep derived implementation
// 
//

/**
 @file tsi_icl_cod_56Step.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_56Step.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

Ctsi_icl_cod_56Step::~Ctsi_icl_cod_56Step()
/**
 * Destructor
 */
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iActiveListener;
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_cod_56Step::Ctsi_icl_cod_56Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_56Step);
	}

TVerdict Ctsi_icl_cod_56Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	User::LeaveIfError(RFbsSession::Connect());
	
	iActiveListener = new (ELeave) CActiveListener();
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_cod_56Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Negative test to check encoders that DO NOT support alpha channel encoding ***"));
		
		TPtrC sourceFilename;
		TInt encoderUidInt;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, encoderUidInt))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		__UHEAP_MARK;
		
		TUid encoderUid(TUid::Uid(encoderUidInt));
		
		RUidDataArray propertiesArray;
		CleanupClosePushL(propertiesArray);
			
		// Ignore the return value, as the Find call will return KErrNotFound anyway
		TRAP_IGNORE(CImageEncoder::GetPluginPropertiesL(encoderUid, propertiesArray));
			
		TInt returnValue = propertiesArray.Find(TUid::Uid(KUidAlphaChannelEncodeSupported));
		CleanupStack::PopAndDestroy(&propertiesArray);
		
		if (returnValue != KErrNotFound)
			{
			INFO_PRINTF3(_L("Encoder implementation 0x%x INCORRECTLY supports alpha encoding. Failed with %d"), encoderUid, returnValue);
			User::Leave(returnValue);
			}
	
		INFO_PRINTF2(_L("Encoder implementation 0x%x correctly does not support the encoding of the source alpha channel"), encoderUid);

		// Generate the name for the destination file
		_LIT(KDestinationFile, "c:\\te_tsi_icl_cod_05\\tsi_icl_cod56Step_encode_destination");
		TFileName destinationFile;
		destinationFile.Copy(KDestinationFile());			
		switch(encoderUid.iUid)
			{
			case KBMPEncoderImplementationUidValue:
				destinationFile.Append(KBmpExtension);
				break;
			case KJPGEncoderImplementationUidValue:
				destinationFile.Append(KJpgExtension);
				break;
			case KGIFEncoderImplementationUidValue:
				destinationFile.Append(KGifExtension);
				break;
			case KPNGEncoderImplementationUidValue:
				destinationFile.Append(KPngExtension);
				break;
			default:
				INFO_PRINTF1(_L("Unknown encoder UID"));
				User::Leave(KErrNotFound);
			}
		
		TInt nanokernel_tick_period;
		HAL::Get(HAL::EFastCounterFrequency, nanokernel_tick_period);
		INFO_PRINTF2(_L("HAL::EFastCounterFrequency = %d"), nanokernel_tick_period);
		TReal nanokernel_tick_period_hires = static_cast<TReal>(nanokernel_tick_period)/KOneMicroSecond;
		
		User::LeaveIfError(iFs.Connect());
		
		CImageEncoder* imageEncoder = NULL;
		TRAPD(err, imageEncoder = CImageEncoder::FileNewL(iFs, destinationFile, CImageEncoder::EOptionNone, KNullUid, KNullUid, encoderUid));
		if(err != KErrNone)
			{
			INFO_PRINTF3(_L("CImageEncoder::FileNewL(0x%X) failed with %d"), encoderUid, err);
			User::Leave(err);
			}
		
		INFO_PRINTF2(_L("Successfully loaded encoder implementation UID = 0x%X"), encoderUid);
			
		CleanupStack::PushL(imageEncoder);
			
		iSourceBitmap = new (ELeave) CFbsBitmap();
		User::LeaveIfError(iSourceBitmap->Load(sourceFilename));
					
		iActiveListener->InitialiseActiveListener();
		
		TUint32 start = User::FastCounter();
			
		imageEncoder->Convert(&iActiveListener->iStatus, *iSourceBitmap, 0);
		CActiveScheduler::Start();
			
		TUint32 end = User::FastCounter();
			
		if(iActiveListener->iStatus.Int() != KErrNone)
			{
			INFO_PRINTF2(_L("CImageEncoder::Convert() failed with %d"), iActiveListener->iStatus.Int());
			User::Leave(iActiveListener->iStatus.Int());
			}
			
		INFO_PRINTF4(_L("********** Convert() start=%u, end=%u, duration=%u"), start, end, end-start);
		INFO_PRINTF2(_L("********** Convert() of file took %f microseconds"), (end-start)/nanokernel_tick_period_hires);
	
#ifdef GENERATE_REFERENCE_FILES
		
		// This is an encode so the file has been saved to the desired location
		SetTestStepResult(EInconclusive);
		
#else
		// Check output
		TPtrC referenceString;
		referenceString.Set(KTe_tsi_icl_cod_5SuiteEncodeRefFile());				
		TPtrC referenceFilename;
		if(!GetStringFromConfig(ConfigSection(),referenceString, referenceFilename))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		RFile referenceFile;
		err = referenceFile.Open(iFs, referenceFilename, EFileRead);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("Attempt at opening the reference file failed with %d"), err);
			User::Leave(err);
			}
		TInt refFileSize;
		err = referenceFile.Size(refFileSize);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("Obtaining the file size failed with %d"), err);
			User::Leave(err);
			}
		HBufC8* referenceBuffer = HBufC8::NewLC(refFileSize);
				
		RFile encodedFile;
		err = encodedFile.Open(iFs, destinationFile, EFileRead);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("Attempt at opening the encoded file failed with %d"), err);
			User::Leave(err);
			}
		TInt encodedFileSize;
		err = encodedFile.Size(encodedFileSize);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("Obtaining the file size failed with %d"), err);
			User::Leave(err);
			}
		
		if(encodedFileSize != refFileSize)
			{
			INFO_PRINTF1(_L("Images do not match.  FAIL."));
			User::Leave(KErrUnknown);
			}
		
		HBufC8* encodedBuffer = HBufC8::NewLC(encodedFileSize);
		
		// Read the files into the buffers and compare them
		TPtr8 referencePtr(referenceBuffer->Des());
		TPtr8 encodedPtr(encodedBuffer->Des());
		
		err = referenceFile.Read(referencePtr);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("Error reading the reference file.  Failed with %d"), err);
			User::Leave(err);
			}
		
		err = encodedFile.Read(encodedPtr);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("Error reading the encoded file.  Failed with %d"), err);
			User::Leave(err);
			}
		
		INFO_PRINTF1(_L("Comparing reference file to encoded file"));
		TInt compareResult = Mem::Compare(referenceBuffer->Des().Ptr(), refFileSize, encodedBuffer->Des().Ptr(), encodedFileSize);

		CleanupStack::PopAndDestroy(encodedBuffer);
		CleanupStack::PopAndDestroy(referenceBuffer);
		referenceFile.Close();
		encodedFile.Close();

		if(compareResult !=  KImagesMatch)
			{
			INFO_PRINTF1(_L("Images do not match.  FAIL."));
			SetTestStepResult(EFail);
			}
		INFO_PRINTF1(_L("Reference file match encoded file"));
		
#endif // GENERATE_REFERENCE_FILES

		// Cleanup
		delete iSourceBitmap;
		iSourceBitmap = NULL;
		CleanupStack::PopAndDestroy(imageEncoder);
		imageEncoder = NULL;
		
#ifndef GENERATE_REFERENCE_FILES
		returnValue = iFs.Delete(destinationFile);
		if(returnValue != KErrNone)
			{
			INFO_PRINTF2(_L("There was a problem deleting the temporary file. Failed with %d"), returnValue);
			User::Leave(returnValue);
			}
#endif // GENERATE_REFERENCE_FILES
		
		RFbsSession::Disconnect();
		REComSession::FinalClose();
		__UHEAP_MARKEND;
		}	
	return TestStepResult();
	}



TVerdict Ctsi_icl_cod_56Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
