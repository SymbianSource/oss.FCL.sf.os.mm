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
// TestStepIclPerformance.cpp
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "TestStepIclPfm.h"
#include "TestSuiteIclPfm.h"

/*
CActiveListener
*/
CActiveListener::CActiveListener()
 : CActive(CActive::EPriorityIdle)
	{
	CActiveScheduler::Add(this);
	}

/*
@xxxx
InitialiseActiveListener
*/

void CActiveListener::InitialiseActiveListener()
	{ 
    iStatus = KRequestPending; 
	SetActive(); 
	}

/*
@xxxx
RunL
*/
void CActiveListener::RunL()
	{ 
	CActiveScheduler::Stop(); 
	}

/*
@xxxx
  DoCancel()
  */
void CActiveListener::DoCancel()
	{
	}


/**
*
* CTestStepIclPerformance constructor 
*
* 
*/
CTestStepIclPerformance::CTestStepIclPerformance()
:iActiveListener(NULL),
iFs(),
iIclPerformanceSuite(NULL),
iScheduler( NULL )
	{
	iHeapSize=16*1024*1024;
	}

/**
*
* CTestStepIclPerformance destructor
*
* 
*/
CTestStepIclPerformance::~CTestStepIclPerformance()
	{
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTestSuiteBitmapTransforms
* @lib "TSU_ICL_PFM_01.lib"
*/

void CTestStepIclPerformance::SetSuite( CTestSuiteIclPerformance* aPtr )
	{
	iIclPerformanceSuite = aPtr;
	}


/**
DoTestStepPreambleL is the preamble function for test steps
It creates an active scheduler and installs it. This is necessary
since we are multithreading and each test step runs in a separate thread.
@returns TVerdict which is either EPass or Efail
 
The function can leave if there is insufficient memeory

@lib "TSU_ICL_PFM_01.lib"
*/
TVerdict CTestStepIclPerformance::DoTestStepPreambleL()
	{
	//[ mark the heap and unmark at the end of postamble ]
		__MM_HEAP_MARK;
		User::LeaveIfError(FbsStartup());
	// [ connect to the bitmap server and check the error code]
	TInt errCode = RFbsSession::Connect();
	if( errCode != KErrNone )
		{
		//[ log failed to connect to bitmap server ]
		Log(_L("Failed to connect to bitmap server in teststep preamble = %d"), errCode );
		return EFail;
		}
	
	iScheduler = new(ELeave)CActiveScheduler;
    //[ push the scheduler on the stack ]
	CleanupStack::PushL( iScheduler );
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
	
	// [Pop iScheduler ] 
	CleanupStack::Pop( iScheduler);

	iActiveListener = new (ELeave) CActiveListener;
	__ASSERT_ALWAYS(iActiveListener, User::Invariant());// ensure active listener created

	User::LeaveIfError(iFs.Connect());
	
	return EPass;
	}

/**
The post amble step cleans up the allocated memory in the preamble
@returns TVerdict EPass or EFail
@leaves can leave due to insufficient memory
@lib "TSU_ICL_PFM_01.lib"
*/

TVerdict CTestStepIclPerformance::DoTestStepPostambleL()
	{
	
	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	
	delete iActiveListener;
	iActiveListener = NULL;

	// [ since we are good citizens let's disconnect from the bitmap server]
	RFbsSession::Disconnect();

    // [ umark the heap which was marked in preamble ]
	__MM_HEAP_MARKEND;

	return EPass;
	}


/**
*
* CTestStepIclPerformance Suite accessor function
* @returns CTestSuiteBitmapTransforms*  
* @lib "TSU_ICL_PFM_01.lib"
*/

CTestSuiteIclPerformance* CTestStepIclPerformance::Suite()
	{
	return iIclPerformanceSuite;
	}




_LIT(KIclPfmPanicCategory, "TS_ICLPerformance");

GLDEF_C void Panic(TS_IclPerformancePanic aError)
	{
	User::Panic(KIclPfmPanicCategory, aError);
	}
