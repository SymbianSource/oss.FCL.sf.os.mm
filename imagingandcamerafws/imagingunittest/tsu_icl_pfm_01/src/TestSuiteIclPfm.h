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
// TestSuiteIclPerformance.h
// 
//

#if (!defined __TEST_SUITE_ICL_PERFORMANCE_H_)
#define __TEST_SUITE_ICL_PERFORMANCE_H_

#include <testframework.h>

const TInt KProfNormalCount = 100; // no of runs when not waiting
const TInt KProfWaitingCount = 10; // no of runs when we don't wait
const TInt KProfWaitTime = 5000000; // 5s


/**
 *
 * CTestSuiteIclPerformance is the test suite
 *
 * @lib "TSU_ICL_PFM_01.lib"
 *
 */
class  CTestSuiteIclPerformance : public CTestSuite
	{
private:
	void InitSystemPath();
public:
		
	void InitialiseL( void );
	virtual ~CTestSuiteIclPerformance();
	void AddTestStepL( CTestStepIclPerformance* ptrTestStep );
	TPtrC GetVersion( void );
	const TDesC& DefaultPath() const;
	
private:
	TFileName iDefaultPath;
	};


/**
@panic  EBadArgument is raised a precondition on an argument is violated
*/

enum TS_IclPerformancePanic
	{
	EBadArgument
	};



GLDEF_C void Panic( TS_IclPerformancePanic aError);

#endif /* __TEST_SUITE_ICL_PERFORMANCE_H_  */
