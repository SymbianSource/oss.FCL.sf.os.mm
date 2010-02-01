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
// This is the main DLL entry point for the tsu_omxil_dummycontentpipe.dll
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include "tsu_omxil_dummycontentpipesuite.h"
#include "tsu_omxil_dummycontentpipe.h"



/**
 *
 * Create an instance of this test suite.
 *
 * NewTestSuiteL is exported at ordinal 1.
 * This provides the interface to allow the Test Framework
 * to create instances of this test suite.
 *
 * @return	"CTestSuite_OMXIL_DummyContentPipe*"
 *			The newly created test suite
 */
EXPORT_C CTestSuite_OMXIL_DummyContentPipe* NewTestSuiteL() 
    { 
	CTestSuite_OMXIL_DummyContentPipe* result = new (ELeave) CTestSuite_OMXIL_DummyContentPipe;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(); // result
	return result;
    }

CTestSuite_OMXIL_DummyContentPipe::~CTestSuite_OMXIL_DummyContentPipe()
	{
	}


/**
 *
 * Get test suite version.
 *
 * @return	"TPtrC"
 *			The version string.
 *
 * @xxxx
 *
 */
_LIT(KTxtVersion,"0.1");
TPtrC CTestSuite_OMXIL_DummyContentPipe::GetVersion( void ) const
	{

	return KTxtVersion();
	}

/**
 *
 * Initialiser for test suite.
 * This creates all the test steps and stores them
 * inside test suite
 *
 * @xxxx
 * 
 */
void CTestSuite_OMXIL_DummyContentPipe::InitialiseL( void )
	{

	// store the name of this test suite
	iSuiteName = _L("TSU_OMXIL_DUMMYCONTENTPIPE");

	// add test steps
	AddTestStepL( new(ELeave) CTestStep_OMXIL_DummyContentPipe_U_0001 );
	AddTestStepL( new(ELeave) CTestStep_OMXIL_DummyContentPipe_U_0002 );
	AddTestStepL( new(ELeave) CTestStep_OMXIL_DummyContentPipe_U_0003 );
	AddTestStepL( new(ELeave) CTestStep_OMXIL_DummyContentPipe_U_0004 );
	}


// -------------------------
