
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
// ICL Codec Tests - see ../UnitTest/ICL/Documentation/ICL Codecs Unit Test Specification.xls
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "ICLCodecTestStep.h"
#include "ICLCodecTestSuite.h"

/* 
 * NewTestSuite is exported at ordinal 1
 * this provides the interface to allow schedule test
 * to create instances of this test suite
 *
 */
EXPORT_C CTestSuite* NewTestSuiteL()
    {
	CICLCodecSuite* result = CICLCodecSuite::NewL();
	return result;
    }

CICLCodecSuite::CICLCodecSuite()
	{
	}

CICLCodecSuite* CICLCodecSuite::NewL()
    {
	CICLCodecSuite* result = new (ELeave) CICLCodecSuite;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result); // result
	return result;
    }

CICLCodecSuite::~CICLCodecSuite()
	{
	}

TPtrC CICLCodecSuite::GetVersion( void )
	{
	_LIT(KTxtVersion,"1.0");
	return KTxtVersion();
	}

void CICLCodecSuite::InitialiseL()
	{
	// store the name of this test suite 
	iSuiteName = _L("TSU_ICL_COD_04");

	TInt index=0;
	for (;;)
		{
		RTestStep* step = NULL;
		TRAPD(error, step = RICLCodecStep::NewL(index));
		if (error==KErrOverflow)
			break;
		else
			User::LeaveIfError(error);
		AddTestStepL(step);
		index++;
		}
	}




