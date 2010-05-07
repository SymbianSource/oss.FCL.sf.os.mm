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
//

#ifndef TSU_OMXIL_DUMMYCONTENTPIPE_H
#define TSU_OMXIL_DUMMYCONTENTPIPE_H

#include <testframework.h>

// forward declaring
class CTestSuite_OMXIL_DummyContentPipe;

class CTestStep_OMXIL_DummyContentPipe: public CTestStep
	{
public:
	CTestStep_OMXIL_DummyContentPipe();
	~CTestStep_OMXIL_DummyContentPipe();
	static void CleanUp(TAny *aPtr);
	void SetTestSuite(const CTestSuite_OMXIL_DummyContentPipe* aTestSuite )
		{iOMXDummyContentPipeTestSuite = aTestSuite;}  

	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
protected:
	// pointer to suite which owns this test 
	const CTestSuite_OMXIL_DummyContentPipe* iOMXDummyContentPipeTestSuite;
	};

/**
 *@class CTestStep_OMXIL_DummyContentPipe_U_0001
 *@test Req. under test REQ8336
 */
class CTestStep_OMXIL_DummyContentPipe_U_0001 : public CTestStep_OMXIL_DummyContentPipe
	{
public:
	CTestStep_OMXIL_DummyContentPipe_U_0001() ;
	~CTestStep_OMXIL_DummyContentPipe_U_0001(){} ;
	virtual TVerdict DoTestStepL( void );
	};
 
/**
 *@class CTestStep_OMXIL_DummyContentPipe_U_0002
 *@test Req. under test REQ8336
 */
class CTestStep_OMXIL_DummyContentPipe_U_0002 : public CTestStep_OMXIL_DummyContentPipe
	{
public:
	CTestStep_OMXIL_DummyContentPipe_U_0002() ;
	~CTestStep_OMXIL_DummyContentPipe_U_0002(){} ;
	virtual TVerdict DoTestStepL( void );
	};
  
/**
 *@class CTestStep_OMXIL_DummyContentPipe_U_0003
 *@test Req. under test REQ8336
 */
class CTestStep_OMXIL_DummyContentPipe_U_0003 : public CTestStep_OMXIL_DummyContentPipe
	{
	public:
	CTestStep_OMXIL_DummyContentPipe_U_0003() ;
	~CTestStep_OMXIL_DummyContentPipe_U_0003(){} ;
	virtual TVerdict DoTestStepL( void );
	};
	
/**
 *@class CTestStep_OMXIL_DummyContentPipe_U_0004
 *@test Req. under test REQ8336
 */
class CTestStep_OMXIL_DummyContentPipe_U_0004 : public CTestStep_OMXIL_DummyContentPipe
	{
	public:
	CTestStep_OMXIL_DummyContentPipe_U_0004() ;
	~CTestStep_OMXIL_DummyContentPipe_U_0004(){} ;
	virtual TVerdict DoTestStepL( void );
	};
 
#endif	// TSU_OMXIL_DUMMYCONTENTPIPE_H
