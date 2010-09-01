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

#include "IclMdaVidTestStep.h"

void CheckErrorValue(TInt aError)
    {
	if (aError != KErrNone)
		User::Panic(_L("System error"),aError);
	}


CIclMdaVidTestStep* CIclMdaVidTestStep::NewL(const TDesC& aTestName)
	{
	CIclMdaVidTestStep* self = new(ELeave) CIclMdaVidTestStep(aTestName);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CIclMdaVidTestStep::CIclMdaVidTestStep(const TDesC& aTestName)
	{
	iTestStepName = aTestName;
	
	//Fix for DEF-043342 increased the heap size for Cedar from 320000
	iHeapSize = 360000;
	}

CIclMdaVidTestStep::~CIclMdaVidTestStep()
	{
	}

void CIclMdaVidTestStep::ConstructL()
	{
	}

TVerdict CIclMdaVidTestStep::OpenL()
	{
	TVerdict verdict = EPass;

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	// Connect to bitmap server
	User::LeaveIfError(FbsStartup());
	TInt err = RFbsSession::Connect();
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Failed to connect to bitmap server in preamble (%d)"), err);
		verdict = EFail;
		}

	// Create test engine
	iEngine = CMdaCoreTestEngine::NewL(this);

	return verdict;
	}
	
void CIclMdaVidTestStep::Close()
	{
	delete iScheduler;
	iScheduler = NULL;

	// Disconnect from bitmap server
	RFbsSession::Disconnect();

	delete iEngine;
	iEngine = NULL;
	}

TVerdict CIclMdaVidTestStep::DoTestStepL()
	{
	TRAPD(err, iEngine->RunTestsL());
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("TMdaVid: left with error %d"), err);
		User::Leave(err);
		}
	if (iEngine->TestsFailedCount())
		{
		INFO_PRINTF2(_L("TMdaVid: %d tests failed"), iEngine->TestsFailedCount());
		return EFail;
		}
	return EPass;
	}

void CIclMdaVidTestStep::Printf(const TDesC& aDes)
	{
	INFO_PRINTF1(aDes);
	}

void CIclMdaVidTestStep::Printf(const TDesC& aDes, const TInt aNum)
	{
	INFO_PRINTF2(aDes, aNum);
	}

