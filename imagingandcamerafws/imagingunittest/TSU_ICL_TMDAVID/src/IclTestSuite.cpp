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

#include "IclMdaVidTestStep.h"
#include "IclTestSuite.h"

/* 
 * NewTestSuite is exported at ordinal 1
 * this provides the interface to allow schedule test
 * to create instances of this test suite
 *
 */
EXPORT_C CTestSuite* NewTestSuiteL()
    {
	CIclTestSuite* result = CIclTestSuite::NewL();
	return result;
    }

CIclTestSuite::CIclTestSuite()
	{
	}

CIclTestSuite* CIclTestSuite::NewL()
    {
	CIclTestSuite* self = new (ELeave) CIclTestSuite;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); // self
	return self;
    }

CIclTestSuite::~CIclTestSuite()
	{
	}

TPtrC CIclTestSuite::GetVersion( void )
	{
	_LIT(KTxtVersion,"1.0");
	return KTxtVersion();
	}

void CIclTestSuite::InitialiseL()
	{
	// store the name of this test suite 
	iSuiteName = _L("TSU_ICL_TMDAVID");
	AddTestStepL(CIclMdaVidTestStep::NewL(_L("MM-ICL-TMDAVID-U-0001")));
	}

