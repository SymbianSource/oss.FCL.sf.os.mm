// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ImageClientMain.h"
#include "GenThreadRelay.h"
#include <ecom/ecom.h>

_LIT(KSubThreadName,"ICLThread");

//
// CThreadRelay
//
/**
 * Constructor for this class.
 *
 */
CThreadRelay::CThreadRelay()
	{
	}

/**
 * Construct for client side threaded relay. Create a thread and wait for the thread
 * to start up or panic. If the thread is successfully created start an undertaker
 * that listens for panics in the thread.
 * 
 * @param	"CSubThreadRelay* (*aFactoryFunctionL)(TThreadId aMainThreadId, TAny* aCodecParam)"
 *			A Facory function that will create a CSubThread derived object.
 * @param	"TAny* aCodecParam"
 *			Parameters to be used during the threaded relay's construction.
 *
 */
void CThreadRelay::ConstructL(CSubThreadRelay* (*aFactoryFunctionL)(TThreadId aMainThreadId, TAny* aCodecParam), TAny* aCodecParam)
	{
	//Get a pointer to this thread's heap
	RHeap& thisHeap = User::Heap();

	//Parameters to send to the sub thread
	TThreadRelayParam param;

	//Add the parameters for the codec
	param.iCodecParam = aCodecParam;

	//Add the factory function to create a CSubThreadDecoderRelay derived object
	param.iFactoryFunctionL = aFactoryFunctionL;

	//The sub thread must return a pointer for us to allow direct calls
	param.iSubThreadRelay = &iSubRelay;

	//Get the id of this thread
	RThread thisThread;
	TThreadId thisThreadId = thisThread.Id();
	param.iMainThreadId = thisThreadId;

	//Get a request to signal us after setup is complete
	TRequestStatus setupComplete = KRequestPending;
	param.iSetupComplete = &setupComplete;

	//current time and the "this" pointer for a unique key
	_LIT(KFormatString,"%S.%020Lu.%08X");
   	TName threadName;
 
	TTime now;
	now.HomeTime();

	threadName.Format(KFormatString, &KSubThreadName, now.Int64(), reinterpret_cast<TUint>(this));

	//Create a new thread for CSubThreadRelay using the same heap as this thread
	TInt result = iSubThread.Create(threadName,
		ThreadFunction,
		KSubThreadStackSize,
		&thisHeap,
		&param);
	if(result != KErrNone)
		User::Leave(result);

	//Run the thread under a lower priority
	iSubThread.SetPriority(KSubThreadPriority); 

	//Wait for thread startup to complete
	TRequestStatus threadStatus = KRequestPending;
	iSubThread.Logon(threadStatus);

	//Start the sub thread
	iSubThread.Resume();

	User::WaitForRequest(threadStatus, setupComplete);

	if(threadStatus!=KRequestPending)
		{
		//Thread creation failed
		TExitType type = iSubThread.ExitType();
		TInt reason = iSubThread.ExitReason();
		if(type==EExitKill)
			User::Leave(reason);
		else
			{
			ASSERT(type==EExitPanic);
			TExitCategoryName category = iSubThread.ExitCategory();
			User::Panic(category,reason);
			}
		}

	// Thread creation was successfull
	TInt error = iSubThread.LogonCancel(threadStatus);
	if(error != KErrNone)
		User::Leave(error); // There is no outstanding request

	User::WaitForRequest(threadStatus); //Eat the signal

	ASSERT(iSubRelay!=NULL);
	iSubRelay->SetSubThread(&iSubThread);
	
	iThreadCreated = ETrue;
	User::LeaveIfError(setupComplete.Int());

	//Create a listener that will monitor the thread
	iSubThreadUndertaker = CSubThreadUndertaker::NewL(iSubThread);
	}


/**
 * Destructor for this class. Calls the shutdown function for the thread.
 *
 */
CThreadRelay::~CThreadRelay()
	{
	Shutdown();
	}

/**
 * Terminate the thread. Since the WaitForRequest() suspend the thread, the undertaker
 * is terminated and logon to the thread locally. Send a shutdown message to the thread
 * and wait for it to terminate.
 *
 */
void CThreadRelay::Shutdown()
	{
	if(iThreadCreated)
		{
		ASSERT(iSubRelay!=NULL);
    	TRequestStatus functionRequest = KRequestPending;
	    TRequestStatus threadRequest = KRequestPending;

	    /*While a function call is in progress this thread is suspended
    	 *and the undertaker will not catch panics, listen for these here
	     */
	    iSubThread.Logon(threadRequest);

	    iSubRelay->RunFunctionInThread(functionRequest, KTerminationFunction, NULL);
    	TRequestStatus* functionCallRequest = iSubRelay->Status();
	    iSubThread.RequestComplete(functionCallRequest, KErrNone);
    	User::WaitForRequest(functionRequest); //At this stage the thread termination signal has been completed
    	User::WaitForRequest(threadRequest); //Eat the thread termination signal

        iThreadCreated = EFalse;
		}

    if(iSubThreadUndertaker)
		{
		iSubThreadUndertaker->Cancel();
		delete iSubThreadUndertaker; iSubThreadUndertaker = NULL;
		}

    iSubThread.Close();
	}

/**
 * Request a function to be executed in the codec thread. Wait for the function to
 * complete or a panic, since the WaitForRequest() suspend the thread(including the
 * undertaker).
 *
 * @param	"TInt aFunctionCode"
 *			The number of the function to execute in the codec thread.
 * @param	"TAny* aParameters"
 *			Parameters for function to be executed in the codec thread.
 * @return	"TInt"
 *			The leave code of the function in the sub thread.
 */
TInt CThreadRelay::RunFunction(TInt aFunctionCode, TAny* aParameters)
	{
	ASSERT(iSubRelay!=NULL);
	TRequestStatus functionRequest = KRequestPending;
	TRequestStatus threadRequest = KRequestPending;

	/*While a function call is in progress this thread is suspended
	 *and the undertaker will not catch panics, listen for these here
	 */
	iSubThread.Logon(threadRequest);

	iSubRelay->RunFunctionInThread(functionRequest, aFunctionCode, aParameters);
	TRequestStatus* functionCallRequest = iSubRelay->Status();
	iSubThread.RequestComplete(functionCallRequest, KErrNone);
	User::WaitForRequest(threadRequest, functionRequest);

	if(threadRequest!=KRequestPending)
		{
		TInt reason = iSubThread.ExitReason();
		TExitCategoryName category = iSubThread.ExitCategory();
		User::Panic(category,reason);
		}

	// Thread is still alive and well
	iSubThread.LogonCancel(threadRequest);
	User::WaitForRequest(threadRequest); //Eat the signal

	ASSERT(functionRequest!=KRequestPending);

	return functionRequest.Int();
	}


/**
 * Return a pointer to the sub thread relay object.
 *
 * @return	"CSubThreadRelay*"
 *			A pointer to the sub thread relay object.
 */
CSubThreadRelay* CThreadRelay::SubThreadRelay()
	{
	return iSubRelay;
	}

/**
 * Return a pointer to the sub thread.
 *
 * @return	"RThread*"
 *			A pointer to the sub thread.
 */
const RThread* CThreadRelay::SubThread() const
	{
	return &iSubThread;
	}


/**
 * Change the sub thread's priority.
 *
 * @param	"TThreadPriority aPriority"
 *			The new priority for the sub thread.
 */
void CThreadRelay::SetPriority(TThreadPriority aPriority) const
	{
	iSubThread.SetPriority(aPriority);
	}

/**
 * Main thread entry point for the codec thread.
 * Create a cleanup stack for the thread and process the codec
 * inside a trap for cleanup behaviour.
 *
 * @param	"TAny* aPtr"
 *			Parameters to be used for creating the thread.
 * @return	"TInt"
 *			The error code for thread termination.
 */
TInt CThreadRelay::ThreadFunction(TAny* aPtr)
	{
	TInt error = KErrNone;

	// Create a cleanup stack for the thread
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if(cleanupStack)
		{
		//Connnect to the bitmap server
		error = RFbsSession::Connect();

		if(error==KErrNone)
			{
			//Run the plugin in a trap
			TRAP(error, ThreadTrapFunctionL(aPtr));
			RFbsSession::Disconnect();
			}
		}
	else
		error = KErrNoMemory;

	delete cleanupStack;

	return error;
	}

/**
 * Function for codec thread execution. Create an active scheduler for the thread
 * and start the function call listener. If the thread is successfully created signal
 * the main thread that the thread creation was successfull.
 *
 * @param	"TAny* aPtr"
 *			A pointer to a TThreadRelayParam object containing the startup parameters
 *          for the thread.
 */
void CThreadRelay::ThreadTrapFunctionL(TAny* aPtr)
	{
	//Create an active scheduler for the thread
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	//Call a Factory function to create a CSubThreadRelay derived object
	TThreadRelayParam* threadRelayParam;
	threadRelayParam = STATIC_CAST(TThreadRelayParam*, aPtr);

	//A factory function that will create a CSubThreadRelay derived object
	CSubThreadRelay* (*FactoryFunctionL) (TThreadId aMainThreadId, TAny* aCodecParam);
	FactoryFunctionL = threadRelayParam->iFactoryFunctionL;

	CSubThreadRelay* subThreadRelay;
	//Call the factory function
	subThreadRelay = FactoryFunctionL(threadRelayParam->iMainThreadId, threadRelayParam->iCodecParam);
	CleanupStack::PushL(subThreadRelay);

	//Send a pointer to the sub thread relay back to the main thread
	*threadRelayParam->iSubThreadRelay = subThreadRelay;

	subThreadRelay->Prime();

	// Send a signal to the main thread that the setup was completed
	subThreadRelay->SignalSetupComplete(threadRelayParam->iSetupComplete);

	CActiveScheduler::Start();	

	CleanupStack::PopAndDestroy(2,scheduler); //subThreadRelay, scheduler
	REComSession::FinalClose();
	}

//
// CSubThreadRelay
//
/**
 * Open a connection to the main (caller) thread.
 *
 */
void CSubThreadRelay::ConstructL(TThreadId aMainThreadId)
	{
	User::LeaveIfError(iMainThread.Open(aMainThreadId));
	}

/**
 * Constructor for this class.
 * Add the function call listener to the active scheduler.
 *
 */
CSubThreadRelay::CSubThreadRelay():
	CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

/**
 * Destructor for this class.
 * Close the connection to the main thread.
 *
 */
CSubThreadRelay::~CSubThreadRelay()
	{
	iMainThread.Close();
	}

/**
 * Cancel the function call listener.
 *
 */
void CSubThreadRelay::DoCancel()
	{
	}

/**
 * Send a signal to the main thread to indicate that the thread startup
 * was successfull.
 *
 */
void CSubThreadRelay::SignalSetupComplete(TRequestStatus* aSetupComplete)
	{
	iMainThread.RequestComplete(aSetupComplete, KErrNone);
	}

/**
 * Prime the function call listener.
 *
 */
void CSubThreadRelay::Prime()
	{
	ASSERT(IsActive() == EFalse);
	iStatus = KRequestPending;
	SetActive();
	}

/**
 * Execute a function in the sub thread. Signal the leave result of the function
 * to the main thread. If the termination function is called stop the active
 * scheduler for the sub thread. (This will cause the thread to terminate)
 *
 */
void CSubThreadRelay::RunL()
	{
	ASSERT(iStatus == KErrNone);

	TInt error = KErrNone;
	//Thread termination function called
	if(iFunctionCode == KTerminationFunction)
		CActiveScheduler::Stop();
	else
		{
		Prime();
		TRAP(error, RunFunctionL(iFunctionCode, iFunctionParams));
		}
	TRequestStatus *status = iCallingStatus;
	iMainThread.RequestComplete(status, error);
	}

/**
 * Save the function number and parameters for an outstanding function call in the
 * codec thread.
 *
 * @param	"TRequestStatus& aCallingStatus"
 *			Request to signal the main thread when the function call has completed.
 * @param	"TInt aFunctionCode"
 *			The function number to execute.
 * @param	"TAny* aParameters"
 *			Parameters to the function to execute.
 */
void CSubThreadRelay::RunFunctionInThread(TRequestStatus& aCallingStatus, TInt aFunctionCode, TAny* aParameters)
	{
	iCallingStatus = &aCallingStatus;
	iFunctionCode = aFunctionCode;
	iFunctionParams = aParameters;
	}

/**
 * Store a pointer to the codec thread object.
 *
 * @param	"RThread* aSubThread"
 *			A pointer to the codec thread object.
 */
void CSubThreadRelay::SetSubThread(RThread* aSubThread)
	{
	iSubThread = aSubThread;
	}

/**
 * Return a pointer to the function call listener's request status.
 * Signal this from the main thread to request a function to be executed 
 * in the thread after a call to RunFunctionInThread().
 *
 * @return	"TRequestStatus*"
 *			Request to signal the start of execution of a function in the thread.
 */
TRequestStatus* CSubThreadRelay::Status()
	{
	return &iStatus;
	}

//
//CSubThreadUndertaker
//
/**
 * Factory function for the class.
 * 
 * @param	"RThread& aSubThread"
 *			A thread to monitor for panics
 */
CSubThreadUndertaker* CSubThreadUndertaker::NewL(RThread& aSubThread)
	{
	CSubThreadUndertaker* self;
	self = new (ELeave) CSubThreadUndertaker(&aSubThread);
	return self;
	}

/**
 * Constructor for the class.
 * Add the object to the active scheduler, activate and logon to the
 * thread to monitor for panics
 * 
 * @param	"RThread* aSubThread"
 *			A thread to monitor for panics
 */
CSubThreadUndertaker::CSubThreadUndertaker(RThread* aSubThread) :
	CActive(CActive::EPriorityIdle),
	iSubThread(aSubThread)
	{
	//Add ourself to the active scheduler
	CActiveScheduler::Add(this);

	iSubThread = aSubThread;

	ASSERT(IsActive()==EFalse);
	iStatus = KRequestPending;
	SetActive();

	//Logon to the thread so we will receive signals
	iSubThread->Logon(iStatus);
	}

/**
 * Destructor for this class.
 * Make sure that the undertaker was cancelled before deleted. 
 */
CSubThreadUndertaker::~CSubThreadUndertaker()
	{
	Cancel();
	}

/**
 * This function is triggered when the thread being monitored terminates.
 * If the thread being monitored panic, panic this thread.
 *
 */
void CSubThreadUndertaker::RunL()
	{
	ASSERT(iSubThread != NULL);
	//If we got here the thread has exited
	TInt reason = iSubThread->ExitReason();
	TExitCategoryName category = iSubThread->ExitCategory();
	User::Panic(category, reason);
	}

/**
 * Stop monitoring the thread for panics. This function is executed by
 * calling Cancel() and has to be done before the object is deleted.
 *
 */
void CSubThreadUndertaker::DoCancel()
	{
	iSubThread->LogonCancel(iStatus);
	}
