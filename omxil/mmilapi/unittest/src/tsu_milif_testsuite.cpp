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
// This file contains the test suite class implementation for MILIF unit tests.
// 
//

#include "tsu_milif_testsuite.h"
#include "tsu_milif_omxiliftests.h"

// Entry function - create a test suite object
EXPORT_C CTestSuiteMILIF* CTestSuiteMILIF::NewTestSuiteL()
	{
	CTestSuiteMILIF* result = new (ELeave) CTestSuiteMILIF();
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result);
	return result;
	}

/*
 *
 * CTestSuiteMILIF - Constructor
 *
 */
CTestSuiteMILIF::CTestSuiteMILIF()
	{
	iSuiteName = _L("TSU_MILIF_OMXILIF");
	}

/*
 *
 * ~CTestSuiteMILIF - Destructor
 *
 */
CTestSuiteMILIF::~CTestSuiteMILIF()
	{
	}

/*
 * GetVersion - Get test suite version
 *
 */
TPtrC CTestSuiteMILIF::GetVersion() const
	{
	_LIT(KTxtVersion, "1.0");
	return KTxtVersion();
	}

/*
 *
 * InitialiseL - Creates all the test steps and stores them inside CTestSuiteMILIF
 *
 */
void CTestSuiteMILIF::InitialiseL()
	{
	AddTestStepL(RTestStepMILIFPorts::NewL(_L("MM-MILIF-DUMMYOMXILIF-U-0001")));
	AddTestStepL(RTestStepMILIFConfig::NewL(_L("MM-MILIF-DUMMYOMXILIF-U-0002")));
	AddTestStepL(RTestStepMILIFStates::NewL(_L("MM-MILIF-DUMMYOMXILIF-U-0003")));
	AddTestStepL(RTestStepMILIFCommands::NewL(_L("MM-MILIF-DUMMYOMXILIF-U-0004")));
	AddTestStepL(RTestStepMILIFExtensions::NewL(_L("MM-MILIF-DUMMYOMXILIF-U-0005")));
	AddTestStepL(RTestStepMILIFComponentRole::NewL(_L("MM-MILIF-DUMMYOMXILIF-U-0006")));
	}
