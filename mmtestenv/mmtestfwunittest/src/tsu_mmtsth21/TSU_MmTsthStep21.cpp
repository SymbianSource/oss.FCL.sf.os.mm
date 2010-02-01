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

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>
#include "TSU_MmTsthStep21.h"
#include "TSU_MmTsthSuite21.h"

// ------------------------------------------------
// RTSUMmTsthStep21 : generic test step for TSU_MMTSTH21

// preamble
TVerdict RTSUMmTsthStep21::OpenL()
	{
	iConsole = NULL;

	iScheduler = new(ELeave)CActiveScheduler;
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
	
	return iTestStepResult = EPass;
	}

// postamble
void RTSUMmTsthStep21::Close()
	{
	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	}

// from MConsoleReader
// process key input from console.
void RTSUMmTsthStep21::InputReceived(TKeyCode /* aKeystroke*/)
	{
	// key not processed as yet - pending implementation of async request from client
	// we would store it as : TInt theInputKey = STATIC_CAST(TInt, aKeystroke);
		
	//read from console again
	iConsole->Read(*this);
	}

// Display general error.
void RTSUMmTsthStep21::Error(TInt)
	{
	// stubbed; undefined input keys can be safely discarded
	}
