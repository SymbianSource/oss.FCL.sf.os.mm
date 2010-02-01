// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "TestStepGifScaler.h"
#include "TestSuiteGifScaler.h"



/**
*
* Create an instance of this test suite.
*
* NewTestSuiteGifScalerL is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteGifScaler*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteGifScaler* NewTestSuiteGifScalerL() 
    { 
	CTestSuiteGifScaler* self = new (ELeave) CTestSuiteGifScaler;
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteGifScaler destructor
*
* 
*/
CTestSuiteGifScaler::~CTestSuiteGifScaler()
	{
	}

/**
*
* This test suite version string
*
*
*/
_LIT(KTxtVersion,"1.0");

/**
*
* Get test suite version.
*
* @return	"TPtrC"
*			The version string.
*
*
*/
TPtrC CTestSuiteGifScaler::GetVersion( )
	{
	return KTxtVersion();
	}

/**
*
* Add a test step into the suite.
*
* @param	"CTestStepGifScaler* aPtrTestStep"
*			The test step to be added.
* 
*/
void CTestSuiteGifScaler::AddTestStepL(CTestStepGifScaler* aPtrTestStep)
	{
    // [ assert pre condition that a valid test ptr has been supplied ]
	__ASSERT_ALWAYS(aPtrTestStep, Panic(EBadArgument));
	
	// [ set back wards reference to test suite ]
	aPtrTestStep->SetSuite(this); 
	
	//[ add the step to the test suite ]
	CTestSuite::AddTestStepL(aPtrTestStep);
	}

/**
*
* Initialiser for test suite.
* This creates all the test steps and stores them
* inside CTestSuiteGifScaler
* 
*/
void CTestSuiteGifScaler::InitialiseL( )
	{
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("GifScaler");
	
	// Add tests
	AddTestStepL(new(ELeave) CTestStepScale1);
	AddTestStepL(new(ELeave) CTestStepScale2);
	AddTestStepL(new(ELeave) CTestStepScale3);
	AddTestStepL(new(ELeave) CTestStepScale4);
	AddTestStepL(new(ELeave) CTestStepScale5);
	AddTestStepL(new(ELeave) CTestStepScale6);
	AddTestStepL(new(ELeave) CTestStepScale7);
	AddTestStepL(new(ELeave) CTestStepScale8);
	AddTestStepL(new(ELeave) CTestStepScale9);
	AddTestStepL(new(ELeave) CTestStepScale10);
	AddTestStepL(new(ELeave) CTestStepScale11);
	AddTestStepL(new(ELeave) CTestStepScale12);
	AddTestStepL(new(ELeave) CTestStepScale13);
	AddTestStepL(new(ELeave) CTestStepScale14);
	AddTestStepL(new(ELeave) CTestStepScale15);

	// Add the alloc tests
	AddTestStepL(new(ELeave) CAllocTestStepScale1);
	AddTestStepL(new(ELeave) CAllocTestStepScale2);
	AddTestStepL(new(ELeave) CAllocTestStepScale3);
	AddTestStepL(new(ELeave) CAllocTestStepScale4);
	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteGifScaler::InitSystemPath()
	{
	_LIT(KVideoTestSourcePathOnE,"e:\\TSU_ICL_GSCAL_01\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\TSU_ICL_GSCAL_01\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\TSU_ICL_GSCAL_01\\");

	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	//[ take the first drive where it exists or can be made ]
	TInt err = fs.MkDir(KVideoTestSourcePathOnC);
	if (( err == KErrNone ) || (err == KErrAlreadyExists))
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else{
        TInt err = fs.MkDir(KVideoTestSourcePathOnD);
        if (( err == KErrNone ) || (err == KErrAlreadyExists))
			{
			iDefaultPath = KVideoTestSourcePathOnD;
			}
		else
			{
			TInt err = fs.MkDir(KVideoTestSourcePathOnE);
			if (( err == KErrNone ) || (err == KErrAlreadyExists))
				{
				iDefaultPath = KVideoTestSourcePathOnE;
				}
			else
				{
				User::Panic(_L("Could not make Btrans directory"),KErrGeneral);
				}			
			}
		}

	fs.Close();
	}

const TDesC& CTestSuiteGifScaler::DefaultPath() const
	{
	return iDefaultPath;
	}
