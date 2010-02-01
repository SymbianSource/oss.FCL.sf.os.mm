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


#ifndef TE_TSI_ICL_PLUGINSUPPORT_01_STEP_BASE
#define TE_TSI_ICL_PLUGINSUPPORT_01_STEP_BASE
#include <test/testexecuteserverbase.h>
#include <icl/icl_uids.hrh>
#include <imageconversion.h>
#include <imageprocessor/imageprocessor.h>
#include <imageprocessor/imageprocessorobserver.h>
#include <panorama/panorama.h>
#include <imagetransform.h>
#include <icl/orientationtransformextension.h>
#include <icl/overlaytransformextension.h>
#include <icl/squeezetransformextension.h>
#include <f32file.h>

/**
 * Listener that stop the ActiveScheduler when signalled
 *
 * @xxxx
 */
class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

//
class CTe_tsi_icl_pluginsupport_01SuiteStepBase : public CTestStep
	{
public:
	virtual ~CTe_tsi_icl_pluginsupport_01SuiteStepBase();
	CTe_tsi_icl_pluginsupport_01SuiteStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();
	};

#endif // TE_TSI_ICL_PLUGINSUPPORT_01_STEP_BASE
