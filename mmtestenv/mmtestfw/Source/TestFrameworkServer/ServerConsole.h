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

#ifndef __SERVERCONSOLE_H__
#define __SERVERCONSOLE_H__

#include <e32std.h>
#include <e32base.h>
#include <e32cons.h>

#include <testframework.h>

/**
 *
 * Console reader mixin class.
 * Any main program or class using a CServerConsole
 * should implement this class.
 *
 * @xxxx
 *
 */
class MConsoleReader
	{
public:
/**
 *
 * Processes a received keystroke.
 *
 * @param	"TKeyCode aKeystroke"
 *			The received keystroke.
 *
 * @xxxx 
 *
 */
	virtual void InputReceived(TKeyCode aKeystroke) = 0;
/**
 *
 * Handles console reader errors.
 *
 * @param	"TInt aError"
 *			The error code.
 *
 * @xxxx 
 *
 */
	virtual void Error(TInt aError) = 0;
	};

/**
 *
 * Keystroke reader for CServerConsole.
 *
 *
 * @xxxx
 *
 */
class CConsoleReader : public CActive
	{
public:
	static CConsoleReader* NewL(CConsoleBase& aConsole);
	~CConsoleReader();
	void DoRead(MConsoleReader& aClient);
	void DoRead();//keeping same client as before
	void RunL();

protected:
	void DoCancel();

private:
	CConsoleReader(CConsoleBase& aConsole);

private:
	MConsoleReader* iClient;
	CConsoleBase& iConsole;
	TKeyCode iKeyStroke;
	};

/**
 *
 * Active console for test input.
 * To be used where manual control of testing is required
 *
 * @xxxx
 *
 */
class CServerConsole : public CActive
	{
public:
	static CServerConsole* NewL(const TDesC& aName);
	~CServerConsole();

	void SetInstructionsL(const TDesC& aInstructions);
	void Read(MConsoleReader& aReader);
	void ReadCancel();

	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

	// accessor
	CConsoleBase* Console() const;

private:
	CServerConsole();
	void ConstructL(const TDesC& aName);

private:
	CConsoleBase* iConsole;
	TRectBuf iWindow;
	HBufC* iWindowName;
	HBufC* iInstructions;
	CConsoleReader* iConsoleReader;
	};

#endif // __SERVERCONSOLE_H__
