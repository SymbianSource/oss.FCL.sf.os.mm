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
// Declarations of Unit test step classes for MILIF testing.
// 
//

#ifndef TSI_MILIF_OMXILIFTESTS_H
#define TSI_MILIF_OMXILIFTESTS_H

#include <e32base.h>
#include <testframework.h>

#include "milcomponentportif.h"
#include "milifobserver.h"

enum TTestEvents
	{
	EEventInit = 0,
	EEventInitComplete,
	EEventExecuteComplete,
	EEventPauseComplete,
	EEventStopComplete,
	EEventFlushComplete,
	EEventPortDisableComplete
	};

enum TTestState
	{
	ETestStateStart = 0,
	ETestStateInitializing,
	ETestStateInitialized,
	ETestStatePausing,
	ETestStateStopping,
	ETestStateFlushing,
	ETestStateFirstPortFlushed,
	ETestStateDisablingPort
	};	


/*
 *
 * RTestStepMILIFBase
 *
 */
class RTestStepMILIFBase : 	public RAsyncTestStep,
				    		public MILIfObserver
	{
public:
	// From RAsyncTestStep
	virtual void KickoffTestL();
	virtual void CloseTest();

	// from MILIfObserver
	virtual TInt MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent);
	virtual TInt BufferDelivered(const MILComponentPortIf* aPort, const CMMFBuffer* aBuffer);

protected:
	RTestStepMILIFBase(const TDesC& aTestName);

	virtual void DoTestBody();
	virtual void DoKickoffTestL();

protected:
	MILComponentIf* iDummyComponent;
	MILComponentPortIf* iComponentInPortIf;	// Not owned
	MILComponentPortIf* iComponentOutPortIf;// Not owned
	TTestState iTestState;
	};
	
/*
 *
 * RTestStepMILIFPorts
 *
 */
class RTestStepMILIFPorts : public RTestStepMILIFBase
	{
public:
	static RTestStepMILIFPorts* NewL(const TDesC& aTestName);

private:
	RTestStepMILIFPorts(const TDesC& aTestName);

	virtual void DoKickoffTestL();
	virtual void DoTestBody();
	};

/*
 *
 * RTestStepMILIFConfig
 *
 */
class RTestStepMILIFConfig : public RTestStepMILIFBase
	{
public:
	static RTestStepMILIFConfig* NewL(const TDesC& aTestName);

private:
	RTestStepMILIFConfig(const TDesC& aTestName);

	virtual void DoTestBody();
	virtual void DoKickoffTestL();
	};
	
/*
 *
 * RTestStepMILIFStates
 *
 */
class RTestStepMILIFStates : public RTestStepMILIFBase
	{
public:
	static RTestStepMILIFStates* NewL(const TDesC& aTestName);

private:
	RTestStepMILIFStates(const TDesC& aTestName);

	virtual void DoKickoffTestL();
	virtual void DoTestBody();

	virtual TInt MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent);
	virtual void Fsm(TTestEvents aEvent, TInt aError);
	};
	
/*
 *
 * RTestStepMILIFCommands	
 *
 */
class RTestStepMILIFCommands : 	public RTestStepMILIFBase
	{
public:
	static RTestStepMILIFCommands* NewL(const TDesC& aTestName);
	
private:
	RTestStepMILIFCommands(const TDesC& aTestName);

	virtual void DoKickoffTestL();
	virtual void DoTestBody();

	virtual TInt MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent);
	virtual void Fsm(TTestEvents aEvent, TInt aError);
	};
	
/*
 *
 * RTestStepMILIFExtensions	
 *
 */
class RTestStepMILIFExtensions : 	public RTestStepMILIFBase
	{
public:
	static RTestStepMILIFExtensions* NewL(const TDesC& aTestName);
	
private:
	RTestStepMILIFExtensions(const TDesC& aTestName);

	virtual void DoKickoffTestL();
	virtual void DoTestBody();
	};	
	
/*
 *
 * RTestStepMILIFComponentRole	
 *
 */
class RTestStepMILIFComponentRole : 	public RTestStepMILIFBase
	{
public:
	static RTestStepMILIFComponentRole* NewL(const TDesC& aTestName);
	
private:
	RTestStepMILIFComponentRole(const TDesC& aTestName);

	virtual void DoKickoffTestL();
	virtual void DoTestBody();
	};	
	
#endif // TSI_MILIF_OMXILIFTESTS_H
