// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "tsi_icl_pluginsupport_010step.h"
#include "te_tsi_icl_pluginsupport_01suitedefs.h"

Ctsi_icl_pluginsupport_010Step::~Ctsi_icl_pluginsupport_010Step()
/**
 * Destructor
 */
	{
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_pluginsupport_010Step::Ctsi_icl_pluginsupport_010Step()
/**
 * Constructor
 */
	{
	SetTestStepName(Ktsi_icl_pluginsupport_010step);
	}

TVerdict Ctsi_icl_pluginsupport_010Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	User::LeaveIfError(iFs.Connect());
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_pluginsupport_010Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Simple instantiation test for CImgProcessor ***"));

		__UHEAP_MARK;		
		
		ImageProcessor::CImgProcessor* imgProcessor = NULL;
		TRAPD(err, imgProcessor = ImageProcessor::CImgProcessor::NewL(iFs, *this /*, TUid::Null()*/));  // Get highest numbered plugin - default
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("\tUnable to instantiate CImgProcessor.  Failed with %d"), err);
			User::Leave(err);
			}
		
		TRAP(err, imgProcessor->InitializeL());				
		
#ifdef ADAPTATION_PLUGINS_PRESENT

		INFO_PRINTF1(_L("\tTest .mmp macro ADAPTATION_PLUGINS_PRESENT defined"));
		
		if(err == KErrNone)
			{
			INFO_PRINTF1(_L("\tA CImgProcessor object was successfully instantiated."));
			INFO_PRINTF1(_L("\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EPass);
			}
		else
			{
			INFO_PRINTF1(_L("\tA CImgProcessor object was not instantiated."));
			INFO_PRINTF1(_L("\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EFail);
			}

#else // ADAPTATION_PLUGINS_PRESENT

		INFO_PRINTF1(_L("\tTest .mmp macro ADAPTATION_PLUGINS_PRESENT not defined"));
		
		if(err == KErrNone)
			{
			INFO_PRINTF1(_L("\tA CImgProcessor object was successfully instantiated."));
			INFO_PRINTF1(_L("\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("\tA CImgProcessor object was not instantiated."));
			INFO_PRINTF1(_L("\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EPass);
			}

#endif // ADAPTATION_PLUGINS_PRESENT
		
		delete imgProcessor;
		imgProcessor = NULL;
		
		REComSession::FinalClose();
		
		__UHEAP_MARKEND;
		}
	
	return TestStepResult();
	}


/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict Ctsi_icl_pluginsupport_010Step::doTestStepPostambleL()
	{
	return TestStepResult();
	}

/**
	Callback implementations for future use.
*/
void Ctsi_icl_pluginsupport_010Step::ImageProcessorInitializingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aError)
	{
	// Unused by Ctsi_icl_pluginsupport_010Step
	INFO_PRINTF2(_L("\tImageProcessorInitializingComplete() callback with error code %d"), aError);
	PrintCallbackBehaviour();
	}

void Ctsi_icl_pluginsupport_010Step::ImageProcessorPreviewInitializingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aPreviewId, TInt aError)
	{
	// Unused by Ctsi_icl_pluginsupport_010Step
	INFO_PRINTF3(_L("\tImageProcessorPreviewInitializingComplete() callback with aPreviewId=%d, and error code %d"), aPreviewId, aError);
	PrintCallbackBehaviour();
	}

void Ctsi_icl_pluginsupport_010Step::ImageProcessingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aError)
	{
	// Unused by Ctsi_icl_pluginsupport_010Step
	INFO_PRINTF2(_L("\tImageProcessingComplete() callback with error code %d"), aError);
	PrintCallbackBehaviour();
	}

void Ctsi_icl_pluginsupport_010Step::ImageProcessorPreviewRenderingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aPreviewId, TInt aError)
	{
	// Unused by Ctsi_icl_pluginsupport_010Step
	INFO_PRINTF3(_L("\tImageProcessorPreviewRenderingComplete() callback with aPreviewId=%d, and error code %d"), aPreviewId, aError);
	PrintCallbackBehaviour();
	}

void Ctsi_icl_pluginsupport_010Step::ImageProcessorEvent(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aEventId, TUid aUid, TInt aId)
	{
	// Unused by Ctsi_icl_pluginsupport_010Step
	INFO_PRINTF4(_L("\tImageProcessorEvent() callback with (TEvent)aEventID=%d, aUid=%d, aId=%d"), aEventId, aUid, aId);
	PrintCallbackBehaviour();
	}

void Ctsi_icl_pluginsupport_010Step::PrintCallbackBehaviour()
	{
#ifdef ADAPTATION_PLUGINS_PRESENT

	INFO_PRINTF1(_L("\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));

#else // ADAPTATION_PLUGINS_PRESENT

	INFO_PRINTF1(_L("\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));

#endif // ADAPTATION_PLUGINS_PRESENT
	}
