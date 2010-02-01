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
//

#include "ServerConsole.h"


/**
 *
 * Static constructor for CServerConsole.
 *
 * @param	"const TDesC& aName"
 *			Display name for console
 *
 * @return	"CServerConsole*"
 *			The constructed CServerConsole
 *
 * @xxxx 
 *
 */
CServerConsole* CServerConsole::NewL(const TDesC& aName)
	{
	CServerConsole* s = new(ELeave) CServerConsole;
	CleanupStack::PushL(s);
	s->ConstructL(aName);
	CleanupStack::Pop();
	return s;
	}

/**
 *
 * Destructor for CServerConsole.
 * Destroys display console and its reader
 *
 * @xxxx
 *
 */
CServerConsole::~CServerConsole()
	{
	Cancel();
	delete iConsoleReader;
	delete iWindowName;
	delete iInstructions;
	delete iConsole;
	}

/**
 *
 * First-phase constructor for CServerConsole.
 * Adds itself to the Active Scheduler.
 *
 * @xxxx
 *
 */
CServerConsole::CServerConsole()
	:CActive(EPriorityHigh)
	{
	CActiveScheduler::Add(this);
	}

/**
 *
 * Second-phase constructor for CServerConsole.
 * Creates the display console and its reader.
 *
 * @param	"const TDesC& aName"
 *			Display name of console.
 *
 * @xxxx
 *
 */
void CServerConsole::ConstructL(const TDesC& aName)
	{
	iWindowName = aName.AllocL();
	iConsole =  Console::NewL(*iWindowName, TSize(KConsFullScreen,KConsFullScreen));
	iConsoleReader = CConsoleReader::NewL(*iConsole);
	}

/**
 *
 * Accessor for base console.
 *
 * @return	"CConsoleBase*"
 *			The base console.
 *
 * @xxxx 
 *
 */
CConsoleBase* CServerConsole::Console() const
	{
	return iConsole;
	}

/**
 *
 * Sets and shows displayable instructions.
 *
 * @param	"const TDesC& aInstructions"
 *			Displayable instruction string.
 *
 * @xxxx 
 *
 */
void CServerConsole::SetInstructionsL(const TDesC& aInstructions)
	{
	if (iInstructions)
		{
		delete iInstructions;
		iInstructions = NULL;
		}
	iInstructions = aInstructions.AllocL();
	iConsole->ClearScreen();
	iConsole->Write(*iInstructions);
	}

/**
 *
 * Starts the console reader listening for input.
 *
 * @param	"MConsoleReader& aReader"
 *			The console reader.
 *
 * @xxxx 
 *
 */
void CServerConsole::Read(MConsoleReader& aReader)
	{
	iConsoleReader->DoRead(aReader);
	}

/**
 *
 * Stops the console reader listening for input.
 *
 *
 * @xxxx 
 *
 */
void CServerConsole::ReadCancel()
	{
	iConsoleReader->Cancel();
	}

/**
 *
 * RunL method for active object CServerConsole.
 *
 * @xxxx
 *
 */
void CServerConsole::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	TBool reading = iConsoleReader->IsActive();
	iConsoleReader->Cancel();

	//listen for key input if we were before...
	if (reading)
		iConsoleReader->DoRead();
	}

/**
 *
 * DoCancel method for active object CServerConsole.
 *
 * @xxxx
 *
 */
void CServerConsole::DoCancel()
	{
	ReadCancel();
	}

/**
 *
 * Error handler for active object CServerConsole.
 * (Currently a stub)
 *
 * @param	"TInt aError"
 *			The error code
 *
 * @return	"TInt"
 *			The error code
 *
 * @xxxx
 *
 */
TInt CServerConsole::RunError(TInt aError)
	{
	return aError;
	}



/**
 *
 * Static constructor for CConsoleReader.
 *
 * @param	"CConsoleBase& aConsole"
 *			The console we are to read
 *
 * @return	"CConsoleReader*"
 *			The constructed CConsoleReader
 *
 * @xxxx
 *
 */
CConsoleReader* CConsoleReader::NewL(CConsoleBase& aConsole)
	{
	CConsoleReader* s = new(ELeave) CConsoleReader(aConsole);
	return s;
	}

/**
 *
 * First-phase constructor for CConsoleReader.
 * Adds itself to the Active Scheduler.
 *
 * @param	"CConsoleBase& aConsole"
 *			console to read from
 *
 * @xxxx
 *
 */
CConsoleReader::CConsoleReader(CConsoleBase& aConsole) :
	CActive(EPriorityUserInput),
	iConsole(aConsole)
	{
	CActiveScheduler::Add(this);
	}

/**
 *
 * Destructor for CConsoleReader.
 *
 * @xxxx
 *
 */
CConsoleReader::~CConsoleReader()
	{
	Cancel();
	}

/**
 *
 * DoRead method for active object CConsoleReader;
 * sets client and starts reading
 *
 * @param	"MConsoleReader& aClient"
 *			client MConsoleReader (which will process the input)
 *
 * @xxxx
 *
 */
void CConsoleReader::DoRead(MConsoleReader& aClient)
	{
	iClient = &aClient;
	DoRead();
	}

/**
 *
 * DoRead method for active object CConsoleReader;
 * starts reading from current client
 *
 * @xxxx
 *
 */
void CConsoleReader::DoRead()
	{
	iConsole.Read(iStatus);
	SetActive();
	}

/**
 *
 * RunL method for active object CConsoleReader;
 * fetches a keystroke and sends it to its client for processing.
 *
 * @xxxx
 *
 */
void CConsoleReader::RunL()
	{
	iKeyStroke = iConsole.KeyCode();
	if (iStatus.Int())
		iClient->Error(iStatus.Int());
	else
		iClient->InputReceived(iKeyStroke);
	}

/**
 *
 * DoCancel method for active object CConsoleReader
 *
 * @xxxx
 *
 */
void CConsoleReader::DoCancel()
	{
	iConsole.ReadCancel();
	}
