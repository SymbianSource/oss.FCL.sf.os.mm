// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TestSuiteIclPerformance.cpp
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "TestStepIclPfm.h"
#include "TestSuiteIclPfm.h"
#include "TestStepMeasurePfm.h"



/**
*
* Create an instance of this test suite.
*
* NewTestSuiteIclPerformanceL is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteIclPerformance*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteIclPerformance* NewTestSuiteIclPerformanceL() 
    { 
	CTestSuiteIclPerformance* self = new (ELeave) CTestSuiteIclPerformance;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteIclPerformance destructor
*
* 
*/
CTestSuiteIclPerformance::~CTestSuiteIclPerformance()
	{
	}

/**
*
* This test suite version string
*
*
*/
_LIT(KTxtVersion,"0.1");

/**
*
* Get test suite version.
*
* @return	"TPtrC"
*			The version string.
*
*
*/
TPtrC CTestSuiteIclPerformance::GetVersion( )
	{
	return KTxtVersion();
	}

/**
*
* Add a test step into the suite.
*
* @param	"CTestStepIclPerformance* aPtrTestStep"
*			The test step to be added.
* 
*/
void CTestSuiteIclPerformance::AddTestStepL( CTestStepIclPerformance* aPtrTestStep )
	{
    // [ assert pre condition that a valid test ptr has been supplied ]
	__ASSERT_ALWAYS( aPtrTestStep, Panic( EBadArgument ) );
	
	// [ set back wards reference to test suite ]
	aPtrTestStep->SetSuite(this); 
	
	//[ add the step to the test suite ]
	CTestSuite::AddTestStepL(aPtrTestStep);
	}

/**
*
* Initialiser for test suite.
* This creates all the test steps and stores them
* inside CTestSuiteIclPerformance
* 
*/
void CTestSuiteIclPerformance::InitialiseL( )
	{
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("tsu_icl_pfm_02");

	AddTestStepL( new(ELeave)  CMmIclPfmTest001 );

	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteIclPerformance::InitSystemPath()
	{

	_LIT(KTestSourcePathOnE,"e:\\TSU_ICL_PFM_02\\");
	_LIT(KTestSourcePathOnD,"d:\\TSU_ICL_PFM_02\\");
	_LIT(KTestSourcePathOnC,"c:\\TSU_ICL_PFM_02\\");
	_LIT(KTestSourcePathOnZ,"z:\\TSU_ICL_PFM_02\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KTestSourcePathOnC,att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnC;
		}
	else if (fs.Att(KTestSourcePathOnD,att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnD;
		}
	else if (fs.Att(KTestSourcePathOnE,att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnE;
		}
	else if (fs.Att(KTestSourcePathOnZ,att) == KErrNone)
		{
		iDefaultPath = KTestSourcePathOnZ;
		}
	else
		{
		User::Panic(_L("Test files not found"),KErrNotFound);
		}

	fs.Close();

	}
	
const TDesC& CTestSuiteIclPerformance::DefaultPath() const
	{
	return iDefaultPath;
	}
