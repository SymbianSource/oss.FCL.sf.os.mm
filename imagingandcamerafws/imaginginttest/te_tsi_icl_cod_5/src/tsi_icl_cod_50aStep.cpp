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
 @file tsi_icl_cod_50aStep.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_50aStep.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
Ctsi_icl_cod_50aStep::~Ctsi_icl_cod_50aStep()
/**
 * Destructor
 */
	{
	delete iDestinationBitmap;
	delete iReferenceBitmap;
	delete iActiveListener;
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_cod_50aStep::Ctsi_icl_cod_50aStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_50aStep);
	}

TVerdict Ctsi_icl_cod_50aStep::doTestStepPreambleL()
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


TVerdict Ctsi_icl_cod_50aStep::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Single frame alloc testing for CR1468 - Add 16MA support to Symbian decoders ***"));
	
		TPtrC sourceFilename;
		TInt decoderUid;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, decoderUid) ||
			!GetBoolFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteAllocTestIsScalingTest, iIsScalingTest) )
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		if(iIsScalingTest)
			{
			if(!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteAllocReductionFactor, iReductionFactor))
				{
				INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
				User::Leave(KErrNotFound);
				}
			}	

		User::LeaveIfError(iFs.Connect());
	
		CImageDecoder::TOptions decoderOptions = CImageDecoder::EOptionNone;
		if(decoderUid == KWMFDecoderImplementationUidValue)
			{
			// Special case for WMF decode in order to request mask generation
			decoderOptions = CImageDecoder::EAllowGeneratedMask;
			
			// Due to WMF using twips rather than absolute pixels, we do not use the size
			// returned from FrameInfo() as it differs between HW and emulator
			if(!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteWidth, iWidth) ||
			!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteHeight, iHeight))
				{
				INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
				User::Leave(KErrNotFound);
				}
			}
		
		iDestinationBitmap = new (ELeave) CFbsBitmap();
		iDestinationBitmap->Create(TSize(0,0), EColor16MA); // Tests only cover single 16MA destination
		
		// Load the reference image
		TPtrC referenceString(KTe_tsi_icl_cod_5Suite16MARefFile()); // 16MA reference file
		TPtrC referenceFilename;
		if(!GetStringFromConfig(ConfigSection(),referenceString, referenceFilename))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		
		iReferenceBitmap = new (ELeave) CFbsBitmap();
		
		if(iIsScalingTest)
			{
			TFileName refFilename;
			refFilename.Copy(referenceFilename);
			TInt locate = refFilename.LocateReverseF('.');
			TBuf<32> extension;
			extension.Format(KReductionFactorMbmExtension, iReductionFactor);
			refFilename.Replace(locate, KMbmExtension().Length(), extension);
			User::LeaveIfError(iReferenceBitmap->Load(refFilename));
			}
		else
			{
			User::LeaveIfError(iReferenceBitmap->Load(referenceFilename));
			}
		
		// Alloc test proper
		TBool completed = EFalse;
		TInt failCount = 1;
		TInt error = KErrNone;
		while(ETrue)
			{
			__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
			__UHEAP_MARK;
			
			TRAP(error, DecodeFileL(TUid::Uid(decoderUid), sourceFilename, decoderOptions));
			
			if(error == KErrNone)
				{
				TAny* testAlloc = User::Alloc(1);
				if(testAlloc == NULL)
					{
					completed = ETrue;
					failCount--;
					}
				else
					{
					User::Free(testAlloc);
					}
				
				if(!CompareBitmaps(iDestinationBitmap, iReferenceBitmap))
					{
					completed = ETrue;
					}
				}
			else if(error != KErrNoMemory) // something has gone wrong, so exit
				{
				completed = ETrue;
				}
			
			REComSession::FinalClose();
				
			__UHEAP_MARKEND;
			__UHEAP_SETFAIL(RHeap::ENone, 0); // reset
			
			if(completed)
				{
				break; // exit loop
				}
			
			failCount++;
			}
	
		failCount--;
		
		if(error == KErrNone)
			{
			INFO_PRINTF3(_L("Alloc test for decoder 0x%x passed successfully with %d allocations"), decoderUid, failCount);
			SetTestStepResult(EPass);
			}
		else
			{
			INFO_PRINTF3(_L("Alloc test for decoder 0x%x failed at %d allocations"), decoderUid, failCount);
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}

void Ctsi_icl_cod_50aStep::DecodeFileL(TUid aDecoderUid, const TDesC& aSourceFilename, CImageDecoder::TOptions aDecoderOptions)
	{
	CImageDecoder* imageDecoder = CImageDecoder::FileNewL(iFs, aSourceFilename, aDecoderOptions, KNullUid, KNullUid, aDecoderUid);
	CleanupStack::PushL(imageDecoder);	
	const TFrameInfo& frameInfo = imageDecoder->FrameInfo(0);
	
	TSize imageSize(frameInfo.iOverallSizeInPixels);
	if(aDecoderUid == TUid::Uid(KWMFDecoderImplementationUidValue))
		{
		imageSize.SetSize(iWidth, iHeight);
		}
	
	if(iIsScalingTest)
		{
		imageDecoder->ReducedSize(imageSize, iReductionFactor, imageSize);	
		}
	
	User::LeaveIfError(iDestinationBitmap->Resize(imageSize));
	
	iActiveListener->InitialiseActiveListener();	
	imageDecoder->Convert(&iActiveListener->iStatus, *iDestinationBitmap, 0);
	
	CActiveScheduler::Start();
	if(iActiveListener->iStatus.Int() != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	CleanupStack::PopAndDestroy(imageDecoder);
	}

TVerdict Ctsi_icl_cod_50aStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
