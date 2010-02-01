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
// TestStepIclPerformance.h
//

#if (!defined __ICL__PERFORMANCE_TESTSTEP_H__)
#define __ICL__PERFORMANCE_TESTSTEP_H__

#include <imageconversion.h>
#include <e32base.h>
#include <fbs.h>
#include <bitstd.h>
#include <bitdev.h>

/**
 *
 * CActiveListener provides the asynchronous operation
 * of an active object
 *
 * @lib "TSU_ICL_PFM_01.lib"
 *
 *
 */

class CActiveListener : public CActive
	{
public:
    CActiveListener();
    void InitialiseActiveListener();
    
    //From CActive
    virtual void RunL();
    virtual void DoCancel();
	
	};

// TImageFormat is used to restrict individual tests
enum TImageFormat
	{
	EANY,
	EBMP,
	EGIF,
	EICO,
	EJPG,
	EMBM,
	EPNG,
	ETFF,
	EWMF,
	EOTA,
	EWBP
	};


class CTestSuite;
class CTestSuiteIclPerformance;

/**
 *
 * CTestStepIclPerformance, which is the base class for all 
 * the ICL performance suite test steps, providing common functionality.
 *
 * @lib "TSU_ICL_PFM_01.lib"
 *
 */
class CTestStepIclPerformance : public CTestStep
{
public:
	CTestStepIclPerformance();
	~CTestStepIclPerformance();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
	void SetSuite( CTestSuiteIclPerformance* aPtr );
 	CTestSuiteIclPerformance* Suite();

protected:
	CActiveListener* iActiveListener;
	RFs							iFs;

private:
  	 CTestSuiteIclPerformance*		iIclPerformanceSuite;
	 CActiveScheduler*				iScheduler ;
};

#endif /* __ICL__PERFORMANCE_TESTSTEP_H__ */

