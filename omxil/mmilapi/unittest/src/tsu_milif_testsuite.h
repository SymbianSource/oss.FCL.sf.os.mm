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
// This file contains the test suite class declaration for MILIF unit tests.
// 
//

#ifndef TSU_MILIF_TESTSUITE_H
#define TSU_MILIF_TESTSUITE_H

#include <e32base.h>
#include <testframework.h>

/*
 *
 * CTestSuiteMILIF
 * - Test suite class
 *
 */
class CTestSuiteMILIF : public CTestSuite
	{
public:
	virtual ~CTestSuiteMILIF();

	// From CTestSuite
	void InitialiseL();
	TPtrC GetVersion() const;

	IMPORT_C static CTestSuiteMILIF* NewTestSuiteL();

private:
	CTestSuiteMILIF();
	};

#endif // TSU_MILIF_TESTSUITE_H
