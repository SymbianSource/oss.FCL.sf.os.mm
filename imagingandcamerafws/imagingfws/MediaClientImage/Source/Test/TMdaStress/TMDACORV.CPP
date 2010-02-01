// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TMDACORE.CPP
// 
//

#include "tmdacore.h"
#include "tmdatest.h"

_LIT(KTextSrvIntro,"Media Server Video Tests");
_LIT(KTextSrvSessionOpen,"Media Server session open");
_LIT(KTextSrvTestRun,"Running tests");
_LIT(KTextSrvSessionClose,"Media Server session close");
_LIT(KTextSrvWaitShutdown,"Waiting for server shutdown");
_LIT(KTextFinishedTestSuccess,"Finished all tests successfully - Press any key");
_LIT(KTextFinishedTestFail,"Finished all tests with %d failures - Press any key");
_LIT(KTextFailedTest,"!!! TEST FAILED !!! - Press any key\r\n");

//
// CMdaCoreTestEngine
//

CMdaCoreTestEngine* CMdaCoreTestEngine::NewL()
	{
	CMdaCoreTestEngine* self = new(ELeave) CMdaCoreTestEngine();
	CleanupStack::PushL(self);
	self->iConsole = Console::NewL(KTextSrvIntro,TSize(KConsFullScreen,KConsFullScreen));
	CleanupStack::Pop(); // self
	return self;
	}

CMdaCoreTestEngine::~CMdaCoreTestEngine()
	{
	delete iConsole;
	}

void CMdaCoreTestEngine::RunTestsL()
	{
	CMdaCoreTest* test = NULL;
	FOREVER
		{
		switch (iTest)
			{
		case 0:
			test = new(ELeave) CMdaStressTest;
			break;
		default:
			break;
			}

		if (!test)
			break;

		CleanupStack::PushL(test);
		test->SetEngine(this);
		test->TestL();
		CleanupStack::PopAndDestroy(); // test
		test = NULL;
		iTest++;
		}

	if (iTestsFailed == 0)
		{
		iConsole->Printf(KTextFinishedTestSuccess);
		RDebug::Print(KTextFinishedTestSuccess);
		}
	else
		{
		iConsole->Printf(KTextFinishedTestFail,iTestsFailed);
		RDebug::Print(KTextFinishedTestFail,iTestsFailed);
		}

	iConsole->Getch();
	}

void CMdaCoreTestEngine::TestFailed(TInt aError)
	{
	TBuf<64> msg(KTextFailedTest);
	msg.Append(' ');
	msg.AppendNum(aError);
	iConsole->Printf(msg);
	RDebug::Print(msg);
	iConsole->Getch();
	iTestsFailed++;
	}

void CMdaCoreTestEngine::Print(const TDesC& aString)
	{
	TBuf<64> format;
	format.Zero();
	format.Append(_L("Test "));
	format.AppendNum(iTest + 1);
	format.Append(_L(" - "));
	format.Append(aString);
	format.Append(_L("\r\n"));
	iConsole->Printf(format);
	RDebug::Print(format);
	}

//
// CMdaCoreTest
//

CMdaCoreTest::~CMdaCoreTest()
	{
	}

void CMdaCoreTest::SetEngine(CMdaCoreTestEngine* aEngine)
	{
	iEngine = aEngine;
	}

void CMdaCoreTest::TestL()
	{
	iEngine->Print(KTextSrvSessionOpen);

	iEngine->Print(KTextSrvTestRun);
	DoTestL();
	}

//
// E32Main
//

_LIT(KVideoTestSourcePathOnE,"e:\\tmdavid\\");
_LIT(KVideoTestSourcePathOnD,"d:\\tmdavid\\");
_LIT(KVideoTestSourcePathOnC,"c:\\tmdavid\\");
_LIT(KVideoTestSourcePathOnZ,"z:\\tmdavid\\");

void InitSystemPath()
	{
	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		ret = fs.SetDefaultPath(KVideoTestSourcePathOnE);
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		ret = fs.SetDefaultPath(KVideoTestSourcePathOnD);
	else if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		ret = fs.SetDefaultPath(KVideoTestSourcePathOnC);
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		ret = fs.SetDefaultPath(KVideoTestSourcePathOnZ);
	else
		User::Panic(_L("Test files not found"),KErrNotFound);

	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));
	fs.Close();
	}

void CheckErrorValue(TInt aError)
    {
	if (aError != KErrNone)
		User::Panic(_L("System error"),aError);
	}

TInt E32Main()
    {
	__UHEAP_MARK;

	CTrapCleanup* tc = CTrapCleanup::New(); // Install exception handler
	__ASSERT_ALWAYS(tc,User::Invariant());	

	CActiveScheduler* as = new CActiveScheduler;
	__ASSERT_ALWAYS(as,User::Invariant());	
	CActiveScheduler::Install(as); // Install active scheduler

	InitSystemPath();

	TInt err = FbsStartup(); // Ensure Fbserv has started up OK
	CheckErrorValue(err);
	err = RFbsSession::Connect();
	CheckErrorValue(err);

	CMdaCoreTestEngine* engine = NULL;
	TRAP(err,engine = CMdaCoreTestEngine::NewL());
	CheckErrorValue(err);
	TRAP(err,engine->RunTestsL());
	CheckErrorValue(err);
		
	RFbsSession::Disconnect();

	delete engine;
	delete as;
	delete tc;

	__UHEAP_MARKEND;

    return 0;
	}

