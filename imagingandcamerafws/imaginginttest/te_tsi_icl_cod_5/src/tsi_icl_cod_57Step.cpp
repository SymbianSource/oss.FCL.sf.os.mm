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
 @file tsi_icl_cod_57Step.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_57Step.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
Ctsi_icl_cod_57Step::~Ctsi_icl_cod_57Step()
/**
 * Destructor
 */
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReference;
	iReference = NULL;
	delete iEncodedFile;
	iEncodedFile = NULL;
	delete iActiveListener;
	delete iActiveScheduler;
	iDisplayModes.Close();
	iFs.Close();
	}

Ctsi_icl_cod_57Step::Ctsi_icl_cod_57Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_57Step);
	}

TVerdict Ctsi_icl_cod_57Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	User::LeaveIfError(RFbsSession::Connect());
	
	iActiveListener = new (ELeave) CActiveListener();
	
	// Set up the display modes to cycle through
	iDisplayModes.AppendL(EColor16MA);
	iDisplayModes.AppendL(EColor16MU);
	iDisplayModes.AppendL(EColor16M);
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_cod_57Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Positive test to check that the MBM encoder DOES support alpha channel encoding ***"));
		
		__UHEAP_MARK;
		
		TInt nanokernel_tick_period;
		HAL::Get(HAL::EFastCounterFrequency, nanokernel_tick_period);
		INFO_PRINTF2(_L("HAL::EFastCounterFrequency = %d"), nanokernel_tick_period);
		TReal nanokernel_tick_period_hires = static_cast<TReal>(nanokernel_tick_period)/KOneMicroSecond;
		
		TUid mbmEncoderUid = TUid::Uid(KMBMEncoderImplementationUidValue);
		
		for(TInt displayModeValue = 0; displayModeValue < iDisplayModes.Count(); displayModeValue++)
			{
			TPtrC sourceFilename;
			if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename))
				{
				INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
				User::Leave(KErrNotFound);
				}

			RUidDataArray propertiesArray;
			CleanupClosePushL(propertiesArray);
			
			// Ignore the return value, as the Find call will return KErrNotFound anyway
			TRAP_IGNORE(CImageEncoder::GetPluginPropertiesL(mbmEncoderUid, propertiesArray));
			
			TInt returnValue = propertiesArray.Find(TUid::Uid(KUidAlphaChannelEncodeSupported));
			CleanupStack::PopAndDestroy(&propertiesArray);
			
			if (returnValue == KErrNotFound)
				{
				INFO_PRINTF3(_L("Encoder implementation 0x%x does not support alpha encoding. Failed with %d"), mbmEncoderUid, returnValue);
				SetTestStepResult(EFail);
				return TestStepResult();
				}
		
			INFO_PRINTF2(_L("CImageEncoder::GetPluginPropertiesL on encoder implementation 0x%x correctly supports the encoding of the source alpha channel"), mbmEncoderUid);
			
			User::LeaveIfError(iFs.Connect());
			
			// Attempt encode
			CImageEncoder* imageEncoder = NULL;
			
			_LIT(KDestinationFile, "c:\\destination.mbm");

			TRAPD(err, imageEncoder = CImageEncoder::FileNewL(iFs, KDestinationFile, CImageEncoder::EOptionNone, KNullUid, KNullUid, mbmEncoderUid));
			if(err != KErrNone)
				{
				INFO_PRINTF3(_L("CImageEncoder::FileNewL(0x%X) failed with %d"), mbmEncoderUid, err);
				User::Leave(err);
				}
		
			INFO_PRINTF2(_L("Successfully loaded encoder implementation UID = 0x%X"), mbmEncoderUid);
			
			CleanupStack::PushL(imageEncoder);
			
			// Create the CFrameImageData to change the display mode of the destination bitmap
			CFrameImageData* frameImageData = CFrameImageData::NewL();
			CleanupStack::PushL(frameImageData);
			TMbmEncodeData* mbmEncodeData = new (ELeave) TMbmEncodeData();
			mbmEncodeData->iDisplayMode = iDisplayModes[displayModeValue];
			frameImageData->AppendFrameData(mbmEncodeData); // frameImageData takes ownership and so not pushed onto cleanup stack either
			
			iSourceBitmap = new (ELeave) CFbsBitmap();
			User::LeaveIfError(iSourceBitmap->Load(sourceFilename));
					
			iActiveListener->InitialiseActiveListener();
			
			TUint32 start = User::FastCounter();
			
			imageEncoder->Convert(&iActiveListener->iStatus, *iSourceBitmap, frameImageData);
			CActiveScheduler::Start();
			
			TUint32 end = User::FastCounter();
			
			if(iActiveListener->iStatus.Int() != KErrNone)
				{
				INFO_PRINTF2(_L("CImageEncoder::Convert() failed with %d"), iActiveListener->iStatus.Int());
				User::Leave(iActiveListener->iStatus.Int());
				}
			
			INFO_PRINTF4(_L("********** Convert() start=%u, end=%u, duration=%u"), start, end, end-start);
			INFO_PRINTF2(_L("********** Convert() of file took %f microseconds"), (end-start)/nanokernel_tick_period_hires);
			
			// Load the encoded file
			iEncodedFile = new (ELeave) CFbsBitmap();
			User::LeaveIfError(iEncodedFile->Load(KDestinationFile));
			
#ifdef GENERATE_REFERENCE_FILES
			TFileName saveFilename;
			saveFilename.Copy(sourceFilename);
			TInt locate = saveFilename.LocateReverseF('.');
			saveFilename.Replace(locate, KMbmExtension().Length(), KMbmExtension);
			
			switch(iDisplayModes[displayModeValue])
				{
				case EColor16MA:
					saveFilename.Insert(locate, _L("_16ma_encode"));
					break;
				case EColor16MU:
					saveFilename.Insert(locate, _L("_16mu_encode"));
					break;
				case EColor16M:
					saveFilename.Insert(locate, _L("_16m_encode"));
					break;
				default:
					INFO_PRINTF1(_L("Unknown display mode"));
					User::Leave(KErrNotFound);
				}
						
			err = iEncodedFile->Save(saveFilename);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("Failed to save output image (with %d)"), err);
				}
			
			INFO_PRINTF2(_L("%S saved."), &saveFilename);
			SetTestStepResult(EInconclusive);
#else
			// Check output
			TPtrC referenceString;
			switch(iDisplayModes[displayModeValue])
				{
				case EColor16MA:
					referenceString.Set(KTe_tsi_icl_cod_5Suite16MARefFile());
					break;
				case EColor16MU:
					referenceString.Set(KTe_tsi_icl_cod_5Suite16MURefFile());
					break;
				case EColor16M:
					referenceString.Set(KTe_tsi_icl_cod_5Suite16MRefFile());
					break;
				default:
					INFO_PRINTF1(_L("Unknown display mode"));
					User::Leave(KErrNotFound);
				}
			
			TPtrC referenceFilename;
			if(!GetStringFromConfig(ConfigSection(),referenceString, referenceFilename))
				{
				INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
				User::Leave(KErrNotFound);
				}
			
			// Load the reference and compare to encoded image
			iReference = new (ELeave) CFbsBitmap();
			User::LeaveIfError(iReference->Load(referenceFilename));
			
			TUint compareResult = CompareBitmaps(iEncodedFile, iReference);
			if(compareResult != KImagesMatch)
				{
				INFO_PRINTF1(_L("Images do not match.  FAIL."));
				SetTestStepResult(EFail);
				}

#endif // GENERATE_REFERENCE_FILES

			// Cleanup
			delete iSourceBitmap;
			iSourceBitmap = NULL;
			delete iReference;
			iReference = NULL;
			delete iEncodedFile;
			iEncodedFile = NULL;
			CleanupStack::PopAndDestroy(frameImageData); // mbmEncodeData is owned by this
			CleanupStack::PopAndDestroy(imageEncoder);
			imageEncoder = NULL;
			propertiesArray.Close();
			
			returnValue = iFs.Delete(KDestinationFile);
			if(returnValue != KErrNone)
				{
				INFO_PRINTF2(_L("There was a problem deleting the temporary file. Failed with %d"), returnValue);
				User::Leave(returnValue);
				}
			} // end for()
		
		RFbsSession::Disconnect();
		REComSession::FinalClose();
		__UHEAP_MARKEND;
		}
	return TestStepResult();
	}



TVerdict Ctsi_icl_cod_57Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
