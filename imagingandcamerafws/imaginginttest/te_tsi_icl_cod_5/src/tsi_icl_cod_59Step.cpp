// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file tsi_icl_cod_59Step.cpp
 @internalTechnology
*/
#include "tsi_icl_cod_59Step.h"
#include "Te_tsi_icl_cod_5SuiteDefs.h"
#include "stdio.h"

Ctsi_icl_cod_59Step::~Ctsi_icl_cod_59Step()
/**
 * Destructor
 */
	{
	delete iDestBitmap;
	delete iReference;
	iFs.Close();
	delete iActiveListener;
	delete iActiveScheduler;
	}

Ctsi_icl_cod_59Step::Ctsi_icl_cod_59Step()
/**
 * Constructor
 */
	{
	// **MUST** call SetTestStepName in the constructor as the controlling
	// framework uses the test step name immediately following construction to set
	// up the step's unique logging ID.
	SetTestStepName(Ktsi_icl_cod_59Step);
	}

TVerdict Ctsi_icl_cod_59Step::doTestStepPreambleL()
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
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_cod_59Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Test to decode multiple frames into a bitmap destination ***"));
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
		if(decoderUid == KUidIclMngPluginImplUid)
			{
			options = CImageDecoder::EOptionMngSubframesNoLoops;
			}
		
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
		do // process all frames in an image file
			{
			const TFrameInfo frameInfo = imageDecoder->FrameInfo(frameToBeProcessed);
			TRect frameRect = frameInfo.iFrameCoordsInPixels;
			//compare the frame info with reference values
			INFO_PRINTF2(_L("Checking frame info for frame %d"), frameNumber);
			INFO_PRINTF5(_L("Frame rectangle - (%d,%d,%d,%d)"),frameRect.iTl.iX, frameRect.iTl.iY, frameRect.iBr.iX, frameRect.iBr.iY);
			if(frameInfo.iFrameCoordsInPixels != KRefFrameInfo[frameNumber])
				{
				INFO_PRINTF2(_L("Frame coordinates of frame %d is not correct"), frameNumber);
				SetTestStepResult(EFail);
				}
			
			//decode frame so that frame information of next frame is available
			iDestBitmap = new (ELeave) CFbsBitmap();
			err = iDestBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16M);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("CFbsBitmap::Create() failed with %d"), err);
				User::Leave(err);
				}
			
			iActiveListener->InitialiseActiveListener();
			
			imageDecoder->Convert(&iActiveListener->iStatus, *iDestBitmap, frameToBeProcessed);
			CActiveScheduler::Start();
			
			if(iActiveListener->iStatus.Int() != KErrNone)
				{
				INFO_PRINTF2(_L("CImageDecoder::Convert() failed with %d"), iActiveListener->iStatus.Int());
				User::Leave(iActiveListener->iStatus.Int());
				}
						
			// delete for re-use
			delete iDestBitmap;
			iDestBitmap = NULL;
		
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
			
			} while (moreFramesToProcess && (TestStepResult()==EPass));
		
		CleanupStack::PopAndDestroy(imageDecoder);
		imageDecoder = NULL;
		
		RFbsSession::Disconnect();
		REComSession::FinalClose();

		__UHEAP_MARKEND;
		}
	return TestStepResult();
	}

TVerdict Ctsi_icl_cod_59Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	RFbsSession::Disconnect();
	REComSession::FinalClose();
	// process something post setting to the test step
	// uncomment the following line if you have common post setting to all the test steps in there
	// CTe_tsi_icl_cod_5SuiteStepBase::doTestStepPostambleL();
	// uncomment the following line if you have post process or remove the following line if no post process
	// SetTestStepResult(EPass);		// or EFail
	return TestStepResult();
	}
