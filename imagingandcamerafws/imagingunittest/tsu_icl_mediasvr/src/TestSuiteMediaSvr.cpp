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
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>
#include "TestStepMediaSvr.h"
#include "MediaSvrTest.h"
#include "TestSuiteMediaSvr.h"




/**
 *
 * Create an instance of this test suite.
 *
 * NewTestSuiteMediaSvrL is exported at ordinal 1.
 * This provides the interface to allow the Test Framework
 * to create instances of this test suite.
 *
 * @return	"CTestSuiteMediaSvr*"
 *			The newly created test suite
 */
EXPORT_C CTestSuiteMediaSvr* NewTestSuiteMediaSvrL() 
    { 
	CTestSuiteMediaSvr* result = new (ELeave) CTestSuiteMediaSvr;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(); // result
	return result;
    }

/**
 *
 * This test suite version string
 *
 * @xxxx
 *
 */
_LIT(KTxtVersion,"1.00 (build 21-Aug-2002)");

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
TPtrC CTestSuiteMediaSvr::GetVersion() const
	{
	return KTxtVersion();
	}

/**
 *
 * Initialiser for test suite.
 * This creates all the test steps and stores them
 * inside CTestSuiteMediaSvr
 *
 * @xxxx
 * 
 */
void CTestSuiteMediaSvr::InitialiseL()
	{

	// store the name of this test suite
	iSuiteName = _L("TSU_ICL_MediaSvr");

	// add test steps
	AddTestStepL(CMediaSvrTest::NewL());
	AddTestStepL(CMediaSvrTest2::NewL());
	AddTestStepL(CMediaSvrTest3::NewL());
	AddTestStepL(CMediaSvrTest4::NewL());
	AddTestStepL(CMediaSvrTest5::NewL());
	AddTestStepL(CMediaSvrTest6::NewL());
	AddTestStepL(CMediaSvrTest7::NewL());
	AddTestStepL(CMediaSvrTest8::NewL());
	AddTestStepL(CMediaSvrTest9::NewL());
	AddTestStepL(CMediaSvrTest10::NewL());
	AddTestStepL(CMediaSvrTest11::NewL());
	AddTestStepL(CMediaSvrTest12::NewL());
	AddTestStepL(CMediaSvrTest13::NewL());
	AddTestStepL(CMediaSvrTest14::NewL());
	AddTestStepL(CMediaSvrTest15::NewL());
	AddTestStepL(CMediaSvrTest16::NewL());
	AddTestStepL(CMediaSvrTest17::NewL());
	AddTestStepL(CMediaSvrTest18::NewL());
	AddTestStepL(CMediaSvrTest19::NewL());
	AddTestStepL(CMediaSvrTest20::NewL());
	AddTestStepL(CMediaSvrTest21::NewL());
	AddTestStepL(CMediaSvrTest22::NewL());
	}
