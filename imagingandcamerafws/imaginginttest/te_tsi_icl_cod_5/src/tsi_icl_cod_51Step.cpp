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
 @file tsi_icl_cod_51Step.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_51Step.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

Ctsi_icl_cod_51Step::~Ctsi_icl_cod_51Step()
/**
 * Destructor
 */
	{
	delete iDestBitmap;
	delete iDestMask;
	delete iReference;
	iFs.Close();
	iDisplayModes.Close();
	delete iActiveListener;
	delete iActiveScheduler;
	}

Ctsi_icl_cod_51Step::Ctsi_icl_cod_51Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_51Step);
	}

TVerdict Ctsi_icl_cod_51Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	// uncomment the following 3 lines if you have common pre setting to all the test steps in there
	// CTe_tsi_icl_cod_5SuiteStepBase::doTestStepPreambleL();
	// if (TestStepResult()!=EPass)
	//    return   TestStepResult();
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	User::LeaveIfError(RFbsSession::Connect());
	
	iActiveListener = new (ELeave) CActiveListener();
	
	// Set up the display modes to cycle through
	iDisplayModes.AppendL(EColor16MA);
	iDisplayModes.AppendL(EColor16MU);
	iDisplayModes.AppendL(EColor16M);
	// Also using an 8 bit mask for the alpha channel
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_cod_51Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Test to decode to bitmap destination and a mask bitmap ***"));
		TPtrC sourceFilename;
		TInt decoderUid;
		TBool isEColor16MAPTest;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, decoderUid) ||
			!GetBoolFromConfig(ConfigSection(),KTe_tsi_icl_cod_5Suite16MAPTest, isEColor16MAPTest))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		if(isEColor16MAPTest && decoderUid == KPNGDecoderImplementationUidValue)
			{
			// Add special case for EColor16MAP
			iDisplayModes.AppendL(EColor16MAP);
			}
	
		__UHEAP_MARK;
		User::LeaveIfError(iFs.Connect());
		
		TInt width = 0;
		TInt height = 0;
		CImageDecoder::TOptions decoderOptions = CImageDecoder::EOptionNone;
		if(decoderUid == KWMFDecoderImplementationUidValue)
			{
			// Special case for WMF decode in order to request mask generation
			decoderOptions = CImageDecoder::EAllowGeneratedMask;
			
			// Due to WMF using twips rather than absolute pixels, we do not use the size
			// returned from FrameInfo() as it differs between HW and emulator
			if(!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteWidth, width) ||
			!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteHeight, height))
				{
				INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
				User::Leave(KErrNotFound);
				}
			}
		
		CImageDecoder* imageDecoder = NULL;
		// Create image decoder outside loop
		TRAPD(err, imageDecoder = CImageDecoder::FileNewL(iFs, sourceFilename, decoderOptions, KNullUid, KNullUid, TUid::Uid(decoderUid)));
		if(err != KErrNone)
			{
			INFO_PRINTF3(_L("CImageDecoder::FileNewL(0x%X) failed with %d"), decoderUid, err);
			User::Leave(err);
			}
		
		INFO_PRINTF2(_L("Successfully loaded decoder implementation UID = 0x%X"), decoderUid);
		
		CleanupStack::PushL(imageDecoder);
		
		TInt nanokernel_tick_period;
		HAL::Get(HAL::EFastCounterFrequency, nanokernel_tick_period);
		INFO_PRINTF2(_L("HAL::EFastCounterFrequency = %d"), nanokernel_tick_period);
		TReal nanokernel_tick_period_hires = static_cast<TReal>(nanokernel_tick_period)/KOneMicroSecond;
		for(TInt displayModeValue = 0; displayModeValue < iDisplayModes.Count(); displayModeValue++)
			{		
			const TFrameInfo frameInfo = imageDecoder->FrameInfo(0);
			
			TSize imageSize(frameInfo.iOverallSizeInPixels);
			if(decoderUid == KWMFDecoderImplementationUidValue)
				{
				imageSize.SetSize(width, height);
				}
			
			iDestBitmap = new (ELeave) CFbsBitmap();
			err = iDestBitmap->Create(imageSize, iDisplayModes[displayModeValue]);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("CFbsBitmap::Create() failed with %d"), err);
				User::Leave(err);
				}
			
			iDestMask = new (ELeave) CFbsBitmap();
			err = iDestMask->Create(imageSize, EGray256); // 8 bit mask to store alpha channel
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("CFbsBitmap::Create() of mask failed with %d"), err);
				User::Leave(err);
				}
			
			iActiveListener->InitialiseActiveListener();
			
			TUint32 start = User::FastCounter();
			
			imageDecoder->Convert(&iActiveListener->iStatus, *iDestBitmap, *iDestMask, 0);
			CActiveScheduler::Start();
			
			TUint32 end = User::FastCounter();
			
			if(iActiveListener->iStatus.Int() != KErrNone)
				{
				INFO_PRINTF2(_L("CImageDecoder::Convert() failed with %d"), iActiveListener->iStatus.Int());
				User::Leave(iActiveListener->iStatus.Int());
				}
			
			INFO_PRINTF4(_L("********** Convert() start=%u, end=%u, duration=%u"), start, end, end-start);
			INFO_PRINTF2(_L("********** Convert() of file took %f microseconds"), (end-start)/nanokernel_tick_period_hires);

#ifdef GENERATE_REFERENCE_FILES
			TFileName saveFilename;
			saveFilename.Copy(sourceFilename);
			TInt locate = saveFilename.LocateReverseF('.');
			saveFilename.Replace(locate, KMbmExtension().Length(), KMbmExtension);
			
			switch(iDisplayModes[displayModeValue])
				{
				case EColor16MAP:
					saveFilename.Insert(locate, _L("_16map_mask_decode"));
					break;
				case EColor16MA:
					saveFilename.Insert(locate, _L("_16ma_mask_decode"));
					break;
				case EColor16MU:
					saveFilename.Insert(locate, _L("_16mu_mask_decode"));
					break;
				case EColor16M:
					saveFilename.Insert(locate, _L("_16m_mask_decode"));
					break;
				default:
					INFO_PRINTF1(_L("Unknown display mode"));
					User::Leave(KErrNotFound);
				}
						
			err = iDestBitmap->Save(saveFilename);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("Failed to save output image (with %d)"), err);
				}
			
			INFO_PRINTF2(_L("%S saved."), &saveFilename);
			
			// Save mask
			locate = saveFilename.LocateReverseF('.');
			saveFilename.Insert(locate, KMask);
			err = iDestMask->Save(saveFilename);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("Failed to save output mask (with %d)"), err);
				}
			
			INFO_PRINTF2(_L("%S saved."), &saveFilename);
			SetTestStepResult(EInconclusive);
#else
			TPtrC referenceString;
			switch(iDisplayModes[displayModeValue])
				{
				case EColor16MAP:
					referenceString.Set(KTe_tsi_icl_cod_5Suite16MAPRefFile());
					break;
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
			
			TPtrC referenceMaskString(KTe_tsi_icl_cod_5Suite8BitMaskRefFile());

			TPtrC referenceFilename;
			TPtrC referenceMaskFilename;
			if(!GetStringFromConfig(ConfigSection(),referenceString, referenceFilename) ||
				!GetStringFromConfig(ConfigSection(),referenceMaskString, referenceMaskFilename))
				{
				INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
				User::Leave(KErrNotFound);
				}
			
			// Load the reference(s) and compare to decoded image
			iReference = new (ELeave) CFbsBitmap();
			User::LeaveIfError(iReference->Load(referenceFilename));
			
			TUint compareResult = CompareBitmaps(iDestBitmap, iReference);
			delete iReference;
			iReference = NULL;
			iReference = new (ELeave) CFbsBitmap();
			User::LeaveIfError(iReference->Load(referenceMaskFilename));
			compareResult += CompareBitmaps(iDestMask, iReference);
			
			if(compareResult != KImagesMatch)
				{
				INFO_PRINTF1(_L("Images do not match.  FAIL."));
				SetTestStepResult(EFail);
				}
			
			delete iReference; // for re-use
			iReference = NULL;
			
#endif // GENERATE_REFERENCE_FILES

			// delete/reset for re-use
			delete iDestBitmap;
			iDestBitmap = NULL;
			delete iDestMask;
			iDestMask = NULL;
			} // end for()
		
		CleanupStack::PopAndDestroy(imageDecoder);
		imageDecoder = NULL;
		RFbsSession::Disconnect();
		REComSession::FinalClose();
		
		__UHEAP_MARKEND;
		}
	return TestStepResult();
	}



TVerdict Ctsi_icl_cod_51Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	// process something post setting to the test step
	// uncomment the following line if you have common post setting to all the test steps in there
	// CTe_tsi_icl_cod_5SuiteStepBase::doTestStepPostambleL();
	// uncomment the following line if you have post process or remove the following line if no post process
	// SetTestStepResult(EPass);		// or EFail
	return TestStepResult();
	}
