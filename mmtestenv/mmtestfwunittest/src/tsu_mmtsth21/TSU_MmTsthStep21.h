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

#ifndef __TSU_MMTSTHSTEP21_H__
#define __TSU_MMTSTHSTEP21_H__

class CTestSuite;
class CTSUMmTsthSuite21;

#include "TestFrameworkServer/TestFrameworkServer.h"

// Base class for all the TSU_MMTSTH21 suite test steps
class RTSUMmTsthStep21 : public RTestStep, public MConsoleReader
	{
public:
	virtual TVerdict OpenL();
	virtual void Close();

	//from MConsoleReader
	void InputReceived(TKeyCode aKeystroke);
	void Error(TInt aError);

protected:
	// required to run a CServerConsole
	CActiveScheduler* iScheduler ;

	// An active console, receiving output from client messages and input from server
	CServerConsole* iConsole;

	// last keypress (will be used by a keyboard stuffer or similar)
	TInt iInputKey;

	};

#endif //__TSU_MMTSTHSTEP21_H__
