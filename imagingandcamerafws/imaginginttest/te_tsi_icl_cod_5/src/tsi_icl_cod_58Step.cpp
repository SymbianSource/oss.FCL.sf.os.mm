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
 @file tsi_icl_cod_58Step.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_58Step.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"

Ctsi_icl_cod_58Step::~Ctsi_icl_cod_58Step()
/**
 * Destructor
 */
	{
	delete iReference;
	delete iFilename;
	delete iImageDisplay;
	iFs.Close();
	iDisplayModes.Close();
	delete iActiveScheduler;
	}

Ctsi_icl_cod_58Step::Ctsi_icl_cod_58Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_58Step);
	}

TVerdict Ctsi_icl_cod_58Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	User::LeaveIfError(RFbsSession::Connect());
	
	// Set up the display modes to cycle through
	iDisplayModes.AppendL(EColor16MA);
	iDisplayModes.AppendL(EColor16MU);
	iDisplayModes.AppendL(EColor16M);
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_cod_58Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Image Display test to decode multiple frames into a bitmap destination and a mask bitmap ***"));
		
		TPtrC sourceFilename;
		TInt decoderUid;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, decoderUid) ||
			!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteFramesToProcess, iTotalFramesToDecode) ||
			!GetBoolFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteMaskPresent, iMaskPresent))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		__UHEAP_MARK;
		User::LeaveIfError(iFs.Connect());
		
		// required saving each frame
		iFilename = HBufC::NewL(sourceFilename.Size());
		TPtr ptr = iFilename->Des();
		ptr.Copy(sourceFilename);
		
		HAL::Get(HAL::EFastCounterFrequency, iNanokernelTickPeriod);
		INFO_PRINTF2(_L("HAL::EFastCounterFrequency = %d"), iNanokernelTickPeriod);
		iNanokernelTickPeriodHiRes = static_cast<TReal>(iNanokernelTickPeriod) / KOneMicroSecond;
		
		iImageDisplay = CImageDisplay::NewL(*this, iFs);
		
		for( ; iDisplayModeIndex < iDisplayModes.Count(); iDisplayModeIndex++)
			{
			iImageDisplay->SetPluginUid(TUid::Uid(decoderUid));
			iImageDisplay->SetDisplayMode(iDisplayModes[iDisplayModeIndex]);
			
			switch(iDisplayModes[iDisplayModeIndex])
				{
				case EColor16M:
					INFO_PRINTF1(_L("Set display mode to EColor16M"));
					break;
				case EColor16MU:
					INFO_PRINTF1(_L("Set display mode to EColor16MU"));
					break;
				case EColor16MA:
					INFO_PRINTF1(_L("Set display mode to EColor16MA"));
					break;
				default:
					INFO_PRINTF1(_L("Unknown display mode"));
					User::Leave(KErrNotFound);
				}
			
			User::LeaveIfError(iImageDisplay->SetImageSource(TMMFileSource(sourceFilename)));
			User::LeaveIfError(iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage));			
				
			TRAPD(err, iImageDisplay->SetupL());
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("iImageDisplay->SetupL() failed with %d"), err);
				User::Leave(err);
				}
			
			const CImageDisplay::RImageSizeArray& sizes = iImageDisplay->RecommendedImageSizes();
			
			iImageDisplay->SetSizeInPixels(sizes[0], ETrue); // For MNG, the RecommendedImageSizes() only returns a single size
			
			INFO_PRINTF2(_L("Setup of plugin (implementation UID = 0x%x) successful"), decoderUid);
			
			iPlayStartTime = User::FastCounter();
			iImageDisplay->Play();
			
			CActiveScheduler::Start();
			
			iImageDisplay->Reset();
			INFO_PRINTF2(_L("########## All frames took %d microseconds"), iTotalImageTime);
			}
		
		REComSession::FinalClose();
		RFbsSession::Disconnect();
		
		__UHEAP_MARK;
		}
		
	return TestStepResult();
	}

void Ctsi_icl_cod_58Step::MiidoImageReady(const CFbsBitmap* /*aBitmap*/, TUint aStatus, const TRect& /*aUpdatedArea*/, TInt aError)
	{
	iPlayEndTime = User::FastCounter();
	
	INFO_PRINTF4(_L("Ctsi_icl_cod_58Step::MiidoImageReady frame=%d, status=%d, error=%d"), iFrameCount, aStatus, aError);
	INFO_PRINTF4(_L("********** Play() start=%u, end=%u, duration=%u"), iPlayStartTime, iPlayEndTime, iPlayEndTime-iPlayStartTime);
	INFO_PRINTF2(_L("********** Play() of frame took %f microseconds"), (iPlayEndTime-iPlayStartTime)/iNanokernelTickPeriodHiRes);
	iTotalImageTime += (iPlayEndTime-iPlayStartTime)/iNanokernelTickPeriodHiRes;
			
	if((aError == KErrNone) && (aStatus & CImageDisplayPlugin::EStatusFrameReady))
		{
		const CFbsBitmap* destinationBitmap = NULL;
		const CFbsBitmap* destinationMask = NULL;
		iImageDisplay->GetBitmap(destinationBitmap, destinationMask);
		TBool pass = ValidateBitmaps(destinationBitmap, destinationMask);
		
		
		
#ifdef GENERATE_REFERENCE_FILES
		if(pass)
			{
			SaveBitmaps(destinationBitmap, destinationMask); // ignore the return code
			// Let the test continue, so no 'Stop' call
			SetTestStepResult(EInconclusive);
			}
#else
		if(pass)
			{
			pass = DoCompareBitmaps(destinationBitmap, destinationMask);
			}
		
#endif // GENERATE_REFERENCE_FILES

		if(aError == KErrNone && pass)
			{
			iFrameCount++;
			if(iFrameCount == iTotalFramesToDecode)
				{
				StopAndResetFrameCount();
				}
			else
				{
				iPlayStartTime = User::FastCounter();
				iImageDisplay->Play();	
				}
			}
		else
			{
			StopAndResetFrameCount();
			SetTestStepResult(EFail);
			}
		}
	}

// Returns ETrue if the bitmaps are as expected, EFalse if not.
TBool Ctsi_icl_cod_58Step::ValidateBitmaps(const CFbsBitmap* aDestinationBitmap, const CFbsBitmap* aDestinationMask) const
	{
	if(!aDestinationBitmap)
		{
		return EFalse;
		}
		
	if(aDestinationBitmap->DisplayMode() == EColor16MA)
		{
		if(aDestinationMask)
			{
			// There should not be a mask if the client has opted for an EColor16MA bitmap
			return EFalse;
			}
		}
	else if(iMaskPresent && !aDestinationMask)
		{
		return EFalse;
		}
	
	return ETrue;
	}

// Returns ETrue if the bitmaps saved successfully, EFalse if not.
TBool Ctsi_icl_cod_58Step::SaveBitmaps(const CFbsBitmap* aDestinationBitmap, const CFbsBitmap* aDestinationMask)
	{
	TFileName saveFilename;
	saveFilename.Copy(iFilename->Des());
	TInt locate = saveFilename.LocateReverseF('.');
	TBuf<32> extension;
	extension.Format(KFrameMbmExtension, iFrameCount);
	saveFilename.Replace(locate, KMbmExtension().Length(), extension);
	
	switch(iDisplayModes[iDisplayModeIndex])
		{
		case EColor16MA:
			saveFilename.Insert(locate, _L("_16ma_image_display_decode"));
			break;
		case EColor16MU:
			saveFilename.Insert(locate, _L("_16mu_image_display_decode"));
			break;
		case EColor16M:
			saveFilename.Insert(locate, _L("_16m_image_display_decode"));
			break;
		default:
			INFO_PRINTF1(_L("Unknown display mode"));
			return EFalse;
		}
	
	TInt err = const_cast<CFbsBitmap*>(aDestinationBitmap)->Save(saveFilename);
	
	if(aDestinationMask) // have already checked if this is expected
		{
		locate = saveFilename.LocateReverseF('.');
		_LIT(KMask, "_mask");
		saveFilename.Insert(locate, KMask);
		err += const_cast<CFbsBitmap*>(aDestinationMask)->Save(saveFilename);
		}
	
	if(err != KErrNone)
		{
		INFO_PRINTF1(_L("There was a problem saving the bitmap"));
		return EFalse;
		}
	
	return ETrue;
	}

TBool Ctsi_icl_cod_58Step::DoCompareBitmaps(const CFbsBitmap* aDestinationBitmap, const CFbsBitmap* aDestinationMask)
	{
	TPtrC referenceString;
	switch(iDisplayModes[iDisplayModeIndex])
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
			return EFalse;
		}
	
	TPtrC referenceFilename;
	if(!GetStringFromConfig(ConfigSection(),referenceString, referenceFilename))
		{
		INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
		return EFalse;
		}
	
	TFileName refFilename;
	refFilename.Copy(referenceFilename);
	TInt locate = refFilename.LocateReverseF('.');
	TBuf<32> extension;
	extension.Format(KFrameMbmExtension, iFrameCount);
	refFilename.Replace(locate, KMbmExtension().Length(), extension);
	
	// Load the reference(s) and compare to decoded image
	TRAPD(err, iReference = new (ELeave) CFbsBitmap());
	if(err)
		{
		INFO_PRINTF2(_L("Unable to create bitmap.  Failed with %d."), err);
		return EFalse;
		}
	
	err = iReference->Load(refFilename);
	if(err)
		{
		INFO_PRINTF2(_L("Unable to load reference image.  Failed with %d."), err);
		return EFalse;
		}
	
	TUint compareResult = CompareBitmaps(aDestinationBitmap, iReference); // check result after mask.
	
	delete iReference;
	iReference = NULL;
	
	if(iMaskPresent && aDestinationMask)
		{
		locate = refFilename.LocateReverseF('.');
		refFilename.Insert(locate, KMask);
		
		TRAP(err, iReference = new (ELeave) CFbsBitmap());
		if(err)
			{
			INFO_PRINTF2(_L("Unable to create bitmap.  Failed with %d."), err);
			return EFalse;
			}
		err = iReference->Load(refFilename);
		if(err)
			{
			INFO_PRINTF2(_L("Unable to load reference image.  Failed with %d."), err);
			return EFalse;
			}
		compareResult += CompareBitmaps(aDestinationMask, iReference);
		
		delete iReference;
		iReference = NULL;
		}
			
	if(compareResult != KImagesMatch)
		{
		INFO_PRINTF1(_L("Images do not match.  FAIL."));
		return EFalse;
		}
	
	return ETrue;
	}

void Ctsi_icl_cod_58Step::StopAndResetFrameCount()
	{
	iFrameCount = 0;
	CActiveScheduler::Stop();
	}

TVerdict Ctsi_icl_cod_58Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
