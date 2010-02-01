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

#include "Statemachine.h"



/*
*******************************************************************************
* Name            : CStateMachine
* Description     : Constructor. 
* Parameters	  : None
* Return Value	  : None
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
CStateMachine::CStateMachine():iState(EUnInitialized)
	{
	// Initailization 
	for(TInt i = 0; i < KNumOfStates; i++)
		for(TInt j = 0; j < KNumOfCommands; j++)
			iStateChanges [i][j] = EInvalidState;

	// set the valid transition states in the table

	// valid state changes from Uninitailize
	iStateChanges [EUnInitialized][EInitializeCommand] = EInitialized;
	
	// valid state changes from Initailize
	iStateChanges [EInitialized][EStartCommand]		= EStart;
	iStateChanges [EInitialized][EPauseCommand]		= EPause;
	iStateChanges [EInitialized][EResumeCommand]	= EStart;
	iStateChanges [EInitialized][EInputEndCommand]	= EInitializedInStopping;
	iStateChanges [EInitialized][EStopCommand]		= EStop;

	// valid state changes from Start
	iStateChanges [EStart][EStartCommand]		= EStart;
	iStateChanges [EStart][EPauseCommand]		= EPause;
	iStateChanges [EStart][EInputEndCommand]	= EStopping;
	iStateChanges [EStart][EStopCommand]		= EStop;

	// valid  State changes from pause
	iStateChanges [EPause][EPauseCommand]		= EPause;
	iStateChanges [EPause][EResumeCommand]		= EStart;
	iStateChanges [EPause][EInputEndCommand]	= EPauseInStopping;
	iStateChanges [EPause][EStopCommand]		= EStop;

	// valid state changes from Stop
	iStateChanges [EStop][EStartCommand]	= EStart;
	iStateChanges [EStop][EInputEndCommand] = EStopInInputEnd;
	iStateChanges [EStop][EStopCommand]		= EStop;

	// valid state changes from InitailizeInStopping
	iStateChanges [EInitializedInStopping][EStartCommand] = EStopping;
	
	// valid  State Changes from EStopping
	iStateChanges [EStopping][EPauseCommand] = EPauseInStopping;
	iStateChanges [EStopping][EStopCommand]	 = EStop;

	// valid state changes from PauseInStopping
	iStateChanges [EPauseInStopping][EResumeCommand] = EStopping;
	iStateChanges [EPauseInStopping][EStopCommand] = EStop;

	// valid state changes frm StopInInputEnd
	iStateChanges [EStopInInputEnd][EStartCommand] = EStopping;
	iStateChanges [EStopInInputEnd][EStopCommand] = EStop;
	}

/*
*******************************************************************************
* Name            : NewL
* Description     : Constructor 
* Parameters	  : None
* Return Value	  : None
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C CStateMachine* CStateMachine::NewL()
{
	CStateMachine* self = new (ELeave) CStateMachine();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return(self);
}

/*
*******************************************************************************
* Name            : ConstructL
* Description     : Symbian 2nd Phase Constrcutor. 
* Known Issues    : None
******************************************************************************
*/
void CStateMachine::ConstructL()
	{
	}

/*
*******************************************************************************
* Name            : CStateMachine::IsTransitionValid
* Description     : Checks whether transition to new state is possible or not
* Parameters	  : "aCommand"	"Command for the transition"
* Return Value	  : Return ETrue if transition is valid, else EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsTransitionValid (TCommand aCommand)
{

	if(iStateChanges[iState][aCommand] != EInvalidState)
		{
		return(ETrue);
		}
	else
		{
		return(EFalse);
		}
}

/*
*******************************************************************************
* Name            : CStateMachine::Transit
* Description     : Transits to new state. If transition cann't be done, object will
					remain in the previous state.
* Parameters	  : "aCommand"	"Command for the transition"
* Return Value	  : Return KErrNone if transition happens else error
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TInt CStateMachine::Transit (TCommand aCommand)
{
	if (!IsTransitionValid (aCommand))
	{
		return KErrGeneral;
	}
	iState = iStateChanges[iState][aCommand];
	return KErrNone;
}

/*
*******************************************************************************
* Name            : CStateMachine::IsInitialized
* Description     : Tells whether the state is initialized or not. Here 
*					initilized means if the state is in any state other than
*					EUnInitialized
* Parameters	  : None
* Return Value	  : Return ETrue, if state is initialized else EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsInitialized (void)
{
	if (iState	!=	EUnInitialized)
	{
		return ETrue;
	} 
	else
	{
		return EFalse;
	}
}

/*
*******************************************************************************
* Name            : CStateMachine::IsInputEndPending
* Description     : Tells whether the state is in any one of the input ending 
*					states i.e
*					EInitializedInStopping,
*					EStopping,
*					EPauseInStopping,
*					EStopInInputEnd
* Parameters	  : None
* Return Value	  : Return ETrue, if state is in any of the input end state else
*					EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsInputEndPending (void)
{
	if (iState == EInitializedInStopping ||
		iState == EStopping ||
		iState == EPauseInStopping ||
		iState == EStopInInputEnd)
	{
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}


/*
*******************************************************************************
* Name            : CStateMachine::IsStarted
* Description     : Tells whether state is in EStart state
* Parameters	  : None
* Return Value	  : Return ETrue if state is EStart, elase EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsStarted (void)
{
	if (iState == EStart)
	{
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}

/*
*******************************************************************************
* Name            : CStateMachine::IsPlaying
* Description     : Tells whether state is in EStart|EStopping|EPause|PauseInStopping
* Parameters	  : None
* Return Value	  : Return ETrue if state is in EStart|EStopping|EPause|PauseInStopping
					, elase EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsPlaying (void)
{
	//if (iState == EStart || iState == EStopping || iState == EPause || 
	//		iState == EPauseInStopping)
	if (iState == EStart || iState == EStopping)
	{
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}


/*
*******************************************************************************
* Name            : CStateMachine::IsPaused 
* Description     : Tells whether stae is in EPause state
* Parameters	  : None
* Return Value	  : Return ETrue if state is EPause, elase EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsPaused (void)
{
	if ((iState == EPause) || (iState == EPauseInStopping))
	{
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}


/*
*******************************************************************************
* Name            : CStateMachine::IsStopped 
* Description     : Tells whether stae is in EStop state
* Parameters	  : None
* Return Value	  : Return ETrue if state is EStop, elase EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IsStopped (void)
{
	if (iState == EStop)
	{
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}

/*
*******************************************************************************
* Name            : CStateMachine::IfIsStateInInitailize 
* Description     : Tells whether stae is in EInitailize or not 
* Parameters	  : None
* Return Value	  : Return ETrue if state is EInitailize, else EFalse
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C TBool CStateMachine::IfIsStateInInitailize(void)
{
	if (iState == EInitialized)
	{
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}
/*
*******************************************************************************
* Name            : Reset
* Description     : Resets the state machine to EUnInitialized state. 
* Parameters	  : None
* Return Value	  : None
* Assumptions     : None
* Known Issues    : None
******************************************************************************
*/
EXPORT_C void CStateMachine::Reset(void)
{
	iState	=	EUnInitialized;
}


