/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <e32base.h>

// Constants
const TUint KNumOfStates    = 10;
const TUint	KNumOfCommands	= 7;

class CStateMachine: public CBase
{
public:
	/**
     * All the states of the state machine.	
	 */
	enum TState
	{
		EUnInitialized = 0,
		EInitialized,
		EStart,
		EPause,
		EStop,
		EInitializedInStopping,
		EStopping,
		EPauseInStopping,
		EStopInInputEnd,
		EInvalidState
	};
	/**
     * Commands that are given to state machine
	 */
	enum TCommand
	{
		EInitializeCommand = 0 ,
		EStartCommand,
		EPauseCommand,
		EResumeCommand,
		EInputEndCommand,
		EStopCommand
	};

private:
	/**
	 *Symbian 2nd phase constructor
	 *@return	"None"
	 */
	void ConstructL();

	/**
	 *Default Constructor
	 */
	CStateMachine();


public:
	/**
	 *Constructor. State will be in EUnInitialized
	 *
	 *@param	"None"
	 *@leave	"None"	
	 *@return	"None"
	 */
	IMPORT_C static CStateMachine* NewL();

	/**
	 *Checks whether transition to new state is possible or not
	 *
	 *@param	"aCommand"	"Command for the transition"
	 *@leave	"None"	
	 *@return	"Return ETrue if transition is valid, else EFalse
	 */
	IMPORT_C TBool	IsTransitionValid (TCommand aCommand);

	/**
	 *Transits to new state. If transition cann't be done, object will
	 *remain in the previous state.
	 *
	 *@param	"aCommand"	"Command for the transition"
	 *@return	"Return KErrNone if transition happens else error.
	 */
	IMPORT_C TInt	Transit (TCommand aCommand);

	/**
	 *Tells whether the state is initialized or not. Here initilized means
	 *if the state is in any state other than EUnInitialized
	 *
	 *@param	"None"
	 *@return	"Return ETrue, if state is initialized else EFalse.
	 */
	IMPORT_C TBool	IsInitialized (void);

	/**
	 *Tells whether the state is in any one of the input ending states i.e
	 *	EInitializedInStopping,
		EStopping,
		EPauseInStopping,
		EStopInInputEnd
	 *
	 *
	 *@param	"None"
	 *@return	"Return ETrue, if state is in any of the input end state else EFalse.
	 */
	IMPORT_C TBool	IsInputEndPending (void);

	/**
	 *Tells whether state is in EStart state
	 *
	 *@param	"None"
	 *@return	"Return ETrue if state is EStart, elase EFalse
	 */
	IMPORT_C TBool	IsStarted (void);

	/**
	 *Tells whether stae is in EPause state
	 *
	 *@param	"None"
	 *@return	"Return ETrue if state is EPause, elase EFalse
	 */
	IMPORT_C TBool	IsPaused (void);

	/**
	 *Tells whether stae is in EStop state
	 *
	 *@param	"None"
	 *@return	"Return ETrue if state is EStop, elase EFalse
	 */
	IMPORT_C TBool	IsStopped (void);

	IMPORT_C TBool	IsPlaying (void);

	/**
	 *Resets the state machine to EUnInitialized state
	 *
	 *@param	"None"
	 *@return	"None"
	 */
	IMPORT_C void	Reset(void);
	
	/**
	 * Returns whether the current state is in initailize or not
	 *
	 *@return	"ETrue if current state is in Initailize"
	 */
	IMPORT_C TBool IfIsStateInInitailize(void);

private:
	//Stores the state of the state machine
	TState			iState;
	TState			iStateChanges[KNumOfStates][KNumOfCommands];
};

#endif // __STATEMACHINE_H__

