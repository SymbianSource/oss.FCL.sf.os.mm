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
 @file tsi_icl_cod_51_allocStep.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_51aStep.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"

Ctsi_icl_cod_51aStep::~Ctsi_icl_cod_51aStep()
/**
 * Destructor
 */
	{
	delete iImageDecoder;
	delete iDestinationBitmap;
	delete iReferenceBitmap;
	delete iActiveListener;
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_cod_51aStep::Ctsi_icl_cod_51aStep()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_51aStep);
	}

TVerdict Ctsi_icl_cod_51aStep::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	iActiveListener = new (ELeave) CActiveListener();

	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_cod_51aStep::doTestStepL()
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
		TInt totalFramesToProcess;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, decoderUid) ||
			!GetIntFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteFramesToProcess, totalFramesToProcess) )
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}

		User::LeaveIfError(iFs.Connect());
				
		// Alloc test proper
		TBool completed = EFalse;
		TInt failCount = 1;
		TInt error = KErrNone;
		TInt currentFrame = 0;
		TInt mngCurrentFrame = 0; // to maintain count of ACTUAL frame number, for frame image comparison
		while(currentFrame < totalFramesToProcess)
			{
			__UHEAP_MARK;
			
			User::LeaveIfError(RFbsSession::Connect());
			iDestinationBitmap = new (ELeave) CFbsBitmap();
			iDestinationBitmap->Create(TSize(0,0), EColor16MA); // Tests only cover single 16MA destination
			
			// Outer loop controls the number of frames.
			// Inner loop loops through 0 to currentFrame frames
			TInt frame = 0;
			TInt framesToProcess = currentFrame + 1;
			
			while(frame < framesToProcess)
				{
				mngCurrentFrame = frame; // for mng
				CImageDecoder::TOptions decoderOptions = CImageDecoder::EOptionNone;
				if(decoderUid == KUidIclMngPluginImplUid)
					{
					// Special case for MNG decode
					decoderOptions = CImageDecoder::EOptionMngSubframesNoLoops;
					frame = 0; // always first frame for MNG
					}
	
				__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
				
				if(!iIsFileNewLDone)
					{
					TRAP(error, DoFileNewL(TUid::Uid(decoderUid), sourceFilename, decoderOptions));
					}
				else
					{
					TRAP(error, DecodeFileL(frame));
					}

				frame = mngCurrentFrame; // set mng frame number to actual frame number
				
				if(error == KErrNone)
					{
					if(!iIsFileNewLDone)
						{
						iIsFileNewLDone = ETrue; // Setup complete, now attempt to decode
						}
					else
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
						
						TPtrC referenceString(KTe_tsi_icl_cod_5Suite16MARefFile()); // 16MA reference file
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
						extension.Format(KFrameMbmExtension, frame);
						refFilename.Replace(locate, KMbmExtension().Length(), extension);
							
						// Load the reference(s) and compare to decoded image
						iReferenceBitmap = new (ELeave) CFbsBitmap();
						User::LeaveIfError(iReferenceBitmap->Load(refFilename));
												
						if(!CompareBitmaps(iDestinationBitmap, iReferenceBitmap))
							{
							INFO_PRINTF2(_L("Frame %d matches reference file"), frame);
							completed = ETrue;
							frame++; // process next frame
							}
						
						delete iReferenceBitmap;
						iReferenceBitmap = NULL;
						}
					}
				else if(error != KErrNoMemory) // something has gone wrong, so exit
					{					
					completed = ETrue;
					}
					
				__UHEAP_SETFAIL(RHeap::ENone, 0); // reset
				
				failCount++;
				}
			
			// reset for use on next loop of frames
			delete iImageDecoder;
			iImageDecoder = NULL;
			iIsFileNewLDone = EFalse;
			
			delete iDestinationBitmap;
			iDestinationBitmap = NULL;
			
			REComSession::FinalClose();
			RFbsSession::Disconnect();
			
			__UHEAP_MARKEND;
			
			currentFrame++; 
			}
		
		failCount--;
		
		if(error == KErrNone)
			{
			if(completed && currentFrame == totalFramesToProcess)
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
		else
			{
			INFO_PRINTF3(_L("Alloc test for decoder 0x%x failed at %d allocations"), decoderUid, failCount);
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}

void Ctsi_icl_cod_51aStep::DoFileNewL(TUid aDecoderUid, const TDesC& aSourceFilename, CImageDecoder::TOptions aDecoderOptions)
	{
	iImageDecoder = CImageDecoder::FileNewL(iFs, aSourceFilename, aDecoderOptions, KNullUid, KNullUid, aDecoderUid);
	}

void Ctsi_icl_cod_51aStep::DecodeFileL(TInt aFrameToProcess)
	{
	const TFrameInfo& frameInfo = iImageDecoder->FrameInfo(aFrameToProcess);
	
	User::LeaveIfError(iDestinationBitmap->Resize(frameInfo.iOverallSizeInPixels));
	
	iActiveListener->InitialiseActiveListener();

	iImageDecoder->Convert(&iActiveListener->iStatus, *iDestinationBitmap, aFrameToProcess);
	
	CActiveScheduler::Start();
	if(iActiveListener->iStatus.Int() != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	}

TVerdict Ctsi_icl_cod_51aStep::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
