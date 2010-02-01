// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @param	"CTestStepBitmapTransforms* aPtrTestStep"
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
	iSuiteName = _L("TSU_ICL_PFM_01");

	AddTestStepL( new(ELeave)  CMmIclPfmTest001  );

	// Add scaling test
	AddTestStepL( new(ELeave) CMmIclPfmTest002 );
	}



/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteIclPerformance::InitSystemPath()
	{

	_LIT(KVideoTestSourcePathOnE,"e:\\ProfileTest\\Images\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\ProfileTest\\Images\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\ProfileTest\\Images\\");
	_LIT(KVideoTestSourcePathOnZ,"z:\\ProfileTest\\Images\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnD;
		}
	else if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnE;
		}
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnZ;
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
