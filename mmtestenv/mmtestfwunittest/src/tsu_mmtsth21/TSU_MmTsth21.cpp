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
// This file contains the test steps for Unit Test Suite 21 : ServerConsole.cpp
// 
//

// EPOC includes
#include <e32base.h>

#include <w32std.h>

// Test system includes
#include <testframework.h>

// Specific includes for this test suite
#include "TSU_MmTsthStep21.h"
#include "TSU_MmTsthSuite21.h"

// Specific includes for these test steps
#include "TSU_MmTsth21.h"
#include "TestFrameworkServer/ServerConsole.h"

// --------------------------------------------

// Unit Test Suite 21 : ServerConsole.cpp
// Depends on : ***

// Requires Active Scheduler in thread

// Tests :-
// Open Server Console
// NB possible changes in code (anticipated : User Input)

// --------------------------------------------
// RTestMmTsthU2101

RTestMmTsthU2101* RTestMmTsthU2101::NewL()
	{
	RTestMmTsthU2101* self = new(ELeave) RTestMmTsthU2101;
	return self;
	}

// Each test step initialises its own name.
RTestMmTsthU2101::RTestMmTsthU2101()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-TSTH-U-2101");
	}

// Do the test step.
TVerdict RTestMmTsthU2101::DoTestStepL()
	{
	INFO_PRINTF1(_L("Unit test for ServerConsole"));

	TVerdict currentVerdict = EPass;

	CServerConsole*	theConsole = NULL;
	TRAPD(err, theConsole = CServerConsole::NewL(_L("CServerConsole Test")));
	if(err != KErrNone)
		{
		ERR_PRINTF1(_L("CServerConsole::NewL() left"));
		return iTestStepResult = EFail;
		}


	iConsole = theConsole;
	// will only work if this test is an MConsoleReader
	// *** NB! if we are running TestFramework in -F mode (i.e. no console output) this MAY
	// time out.
	iConsole->Read(*this);

	// call RunL() directly - not ideal but we have to test it somehow...
	// there is no window server so we can't simulate a keyboard event to call iConsoleReader->RunL()
	// this may have to be rewritten if full async keyboard handling is to be implemented
	TRAPD(err2, iConsole->RunL());
	if(err2)
		{
		ERR_PRINTF2(_L("CServerConsole::RunL() left with error %d"), err2);
		delete iConsole;
		return iTestStepResult = EFail;
		};

	INFO_PRINTF1(_L("Returned from RunL()"));

	// cleanup
	delete iConsole;
	return iTestStepResult = currentVerdict; // should be EPass if we've got here
	}
