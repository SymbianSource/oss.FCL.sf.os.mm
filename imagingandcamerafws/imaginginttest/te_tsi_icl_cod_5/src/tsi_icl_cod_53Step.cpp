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
 @file tsi_icl_cod_53Step.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_53Step.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"

Ctsi_icl_cod_53Step::~Ctsi_icl_cod_53Step()
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

Ctsi_icl_cod_53Step::Ctsi_icl_cod_53Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_53Step);
	}

TVerdict Ctsi_icl_cod_53Step::doTestStepPreambleL()
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


TVerdict Ctsi_icl_cod_53Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Test to decode multiple frames into a bitmap destination and a mask bitmap ***"));
		TPtrC sourceFilename;
		TInt decoderUid;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, decoderUid))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		__UHEAP_MARK;
		
		User::LeaveIfError(iFs.Connect());
		
		CImageDecoder::TOptions options = CImageDecoder::EOptionNone;
		
		if (decoderUid == KUidIclMngPluginImplUid)
			{
			options = CImageDecoder::EOptionMngSubframesNoLoops;
			}
		
		TInt nanokernel_tick_period;
		HAL::Get(HAL::EFastCounterFrequency, nanokernel_tick_period);
		INFO_PRINTF2(_L("HAL::EFastCounterFrequency = %d"), nanokernel_tick_period);
		TReal nanokernel_tick_period_hires = static_cast<TReal>(nanokernel_tick_period)/KOneMicroSecond;
		
		for(TInt displayModeValue = 0; displayModeValue < iDisplayModes.Count(); displayModeValue++)
			{
			CImageDecoder* imageDecoder = NULL;
			// Create image decoder here because once a multi-frame MNG file has been processed once (i.e.
			// all frames), a new image decoder is required to start from frame 0.
			TRAPD(err, imageDecoder = CImageDecoder::FileNewL(iFs, sourceFilename, options, KNullUid, KNullUid, TUid::Uid(decoderUid)));
			if(err != KErrNone)
				{
				INFO_PRINTF3(_L("CImageDecoder::FileNewL(0x%X) failed with %d"), decoderUid, err);
				User::Leave(err);
				}
		
			INFO_PRINTF2(_L("Successfully loaded decoder implementation UID = 0x%X"), decoderUid);
			
			CleanupStack::PushL(imageDecoder);
			
			TInt frameToBeProcessed = 0;
			TInt frameNumber = 0;  // for file saving + MNG does not use FrameCount() call
			TBool moreFramesToProcess = EFalse;
			TUint totalImageTime = 0;
			do // process all frames in an image file
				{
				const TFrameInfo frameInfo = imageDecoder->FrameInfo(frameToBeProcessed);
					
				iDestBitmap = new (ELeave) CFbsBitmap();
				err = iDestBitmap->Create(frameInfo.iOverallSizeInPixels, iDisplayModes[displayModeValue]);
				if(err != KErrNone)
					{
					INFO_PRINTF2(_L("CFbsBitmap::Create() failed with %d"), err);
					User::Leave(err);
					}
				
				iDestMask = new (ELeave) CFbsBitmap();
				err = iDestMask->Create(frameInfo.iOverallSizeInPixels, EGray256); // 8 bit mask to store alpha channel
				if(err != KErrNone)
					{
					INFO_PRINTF2(_L("CFbsBitmap::Create() of mask failed with %d"), err);
					User::Leave(err);
					}
			
				iActiveListener->InitialiseActiveListener();
				
				TUint32 start = User::FastCounter();
				
				imageDecoder->Convert(&iActiveListener->iStatus, *iDestBitmap, *iDestMask, frameToBeProcessed);
				CActiveScheduler::Start();
				
				TUint32 end = User::FastCounter();
				
				if(iActiveListener->iStatus.Int() != KErrNone)
					{
					INFO_PRINTF2(_L("CImageDecoder::Convert() failed with %d"), iActiveListener->iStatus.Int());
					User::Leave(iActiveListener->iStatus.Int());
					}
				
				INFO_PRINTF4(_L("********** Convert() start=%u, end=%u, duration=%u"), start, end, end-start);
				INFO_PRINTF2(_L("********** Convert() of frame took %f microseconds"), (end-start)/nanokernel_tick_period_hires);
				totalImageTime += (end-start)/nanokernel_tick_period_hires;
								
#ifdef GENERATE_REFERENCE_FILES
				TFileName saveFilename;
				saveFilename.Copy(sourceFilename);
				TInt locate = saveFilename.LocateReverseF('.');
				TBuf<32> extension;
				extension.Format(KFrameMbmExtension, frameNumber);
				saveFilename.Replace(locate, KMbmExtension().Length(), extension);
			
				switch(iDisplayModes[displayModeValue])
					{
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
					User::Leave(err);
					}
				
				INFO_PRINTF2(_L("%S saved."), &saveFilename);
		
				// Save mask
				locate = saveFilename.LocateReverseF('.');
				_LIT(KMask, "_mask");
				saveFilename.Insert(locate, KMask);
				err = iDestMask->Save(saveFilename);
				if(err != KErrNone)
					{
					INFO_PRINTF2(_L("Failed to save output mask (with %d)"), err);
					User::Leave(err);
					}
				
				INFO_PRINTF2(_L("%S saved."), &saveFilename);
		
				SetTestStepResult(EInconclusive);
#else
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
				
				TFileName refFilename;
				refFilename.Copy(referenceFilename);
				TInt locate = refFilename.LocateReverseF('.');
				TBuf<32> extension;
				extension.Format(KFrameMbmExtension, frameNumber);
				refFilename.Replace(locate, KMbmExtension().Length(), extension);
				
				// Load the reference(s) and compare to decoded image
				iReference = new (ELeave) CFbsBitmap();
				iReference->Load(refFilename);
				
				INFO_PRINTF2(_L("Comparing against reference file %S"), &refFilename);
				TUint compareResult = CompareBitmaps(iDestBitmap, iReference);
				delete iReference; // for re-use
				iReference = NULL;
				
				locate = refFilename.LocateReverseF('.');
				refFilename.Insert(locate, KMask);
				
				iReference = new (ELeave) CFbsBitmap();
				iReference->Load(refFilename);
				INFO_PRINTF2(_L("Comparing against reference file %S"), &refFilename);
				compareResult += CompareBitmaps(iDestMask, iReference);
				
				if(compareResult != KImagesMatch)
					{
					INFO_PRINTF1(_L("Images do not match.  FAIL."));
					SetTestStepResult(EFail);
					}
				
				delete iReference; // for reuse
				iReference = NULL;
			
#endif // GENERATE_REFERENCE_FILES

				// delete for re-use
				delete iDestBitmap;
				iDestBitmap = NULL;
				delete iDestMask;
				iDestMask = NULL;
				
				frameNumber++;
				
				if(decoderUid != KUidIclMngPluginImplUid)
					{
					if(frameToBeProcessed < imageDecoder->FrameCount()-1)
						{
						moreFramesToProcess = ETrue;
						frameToBeProcessed++;	
						}
					else
						{
						moreFramesToProcess = EFalse;
						}
					}
				else
					{
					moreFramesToProcess = imageDecoder->FrameInfo().iFlags & TFrameInfo::EMngMoreFramesToDecode;
					frameToBeProcessed = 0; // reset for MNG
					}
				
				} while (moreFramesToProcess);
			
			CleanupStack::PopAndDestroy(imageDecoder);
			imageDecoder = NULL;
			
			INFO_PRINTF2(_L("########## All frames took %d microseconds"), totalImageTime);
			
			}// end for()
		
		RFbsSession::Disconnect();
		REComSession::FinalClose();
		
		__UHEAP_MARKEND;
		}
	return TestStepResult();
	}

TVerdict Ctsi_icl_cod_53Step::doTestStepPostambleL()
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
