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

#include "tsi_icl_pluginsupport_011step.h"
#include "te_tsi_icl_pluginsupport_01suitedefs.h"

Ctsi_icl_pluginsupport_011Step::~Ctsi_icl_pluginsupport_011Step()
/**
 * Destructor
 */
	{
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_pluginsupport_011Step::Ctsi_icl_pluginsupport_011Step()
/**
 * Constructor
 */
	{
	SetTestStepName(Ktsi_icl_pluginsupport_011step);
	}

TVerdict Ctsi_icl_pluginsupport_011Step::doTestStepPreambleL()
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


TVerdict Ctsi_icl_pluginsupport_011Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** Simple instantiation test for CImagePanorama ***"));
		
		__UHEAP_MARK;
		
		CImagePanorama* imagePanorama = NULL;
		TRAPD(err, imagePanorama = CImagePanorama::NewL());

#ifdef ADAPTATION_PLUGINS_PRESENT

		INFO_PRINTF1(_L("\tTest .mmp macro ADAPTATION_PLUGINS_PRESENT defined"));
		
		if(err == KErrNone)
			{
			INFO_PRINTF1(_L("\tA CImagePanorama object was successfully instantiated."));
			INFO_PRINTF1(_L("\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EPass);
			}
		else
			{
			INFO_PRINTF1(_L("\tA CImagePanorama object was not instantiated."));
			INFO_PRINTF1(_L("\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EFail);
			}

#else // ADAPTATION_PLUGINS_PRESENT

		INFO_PRINTF1(_L("\tTest .mmp macro ADAPTATION_PLUGINS_PRESENT not defined"));
		
		if(err == KErrNone)
			{
			INFO_PRINTF1(_L("\tA CImagePanorama object was successfully instantiated."));
			INFO_PRINTF1(_L("\tThis is UNEXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("\tA CImagePanorama object was not instantiated."));
			INFO_PRINTF1(_L("\tThis is EXPECTED behaviour with this configuration (see the macro: ADAPTATION_PLUGINS_PRESENT)."));
			SetTestStepResult(EPass);
			}

#endif // ADAPTATION_PLUGINS_PRESENT
		
		delete imagePanorama;
		imagePanorama = NULL;
		
		REComSession::FinalClose();
		
		__UHEAP_MARKEND;
		}
	
	return TestStepResult();
	}


/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict Ctsi_icl_pluginsupport_011Step::doTestStepPostambleL()
	{
	return TestStepResult();
	}

