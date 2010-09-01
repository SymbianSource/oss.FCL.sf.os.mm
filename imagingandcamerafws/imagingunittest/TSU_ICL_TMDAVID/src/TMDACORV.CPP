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

#include "TMDACORE.H"
#include "TMDATEST.H"

//THE FOLLOWING DEFINE SHOULD BE UNCOMMENTED TO ENABLE PRINTS

//#define DEBUG_PRINTS_ENABLED 1

#ifdef DEBUG_PRINTS_ENABLED
#define RDEBUG_PRINT1(x) RDebug::Print(x)
#define RDEBUG_PRINT2(x,y) RDebug::Print(x,y)
#else
#define RDEBUG_PRINT1(x)
#define RDEBUG_PRINT2(x,y) 
#endif


//_LIT(KTextSrvIntro,"Media Server Video Tests");									// EABI warning removal
_LIT(KTextSrvSessionOpen,"Media Server session open");
_LIT(KTextSrvTestRun,"Running tests");
//_LIT(KTextSrvSessionClose,"Media Server session close");							// EABI warning removal
//_LIT(KTextSrvWaitShutdown,"Waiting for server shutdown");							// EABI warning removal
_LIT(KTextFinishedTestSuccess,"Finished all tests successfully - Press any key");
_LIT(KTextFinishedTestFail,"Finished all tests with %d failures - Press any key");
_LIT(KTextFailedTest,"!!! TEST FAILED !!! - Press any key\r\n");

//
// CMdaCoreTestEngine
//

_LIT(KVideoTestSourcePathOnE,"e:\\tmdavid\\");
_LIT(KVideoTestSourcePathOnD,"d:\\tmdavid\\");
_LIT(KVideoTestSourcePathOnC,"c:\\tmdavid\\");
_LIT(KVideoTestSourcePathOnZ,"z:\\tmdavid\\");

void CMdaCoreTestEngine::InitSystemPath()
	{
	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		iDefaultPath = KVideoTestSourcePathOnE;
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		iDefaultPath = KVideoTestSourcePathOnD;
	else if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		iDefaultPath = KVideoTestSourcePathOnC;
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		iDefaultPath = KVideoTestSourcePathOnZ;
	else
		User::Panic(_L("Test files not found"),KErrNotFound);
	
	fs.Close();
	}

const TDesC& CMdaCoreTestEngine::DefaultPath() const
	{
	return iDefaultPath;
	}

CMdaCoreTestEngine* CMdaCoreTestEngine::NewL(MDummyConsole* aConsole)
	{
	CMdaCoreTestEngine* self = new(ELeave) CMdaCoreTestEngine();
	CleanupStack::PushL(self);
	self->ConstructL(aConsole);
	CleanupStack::Pop(); // self
	return self;
	}

void CMdaCoreTestEngine::ConstructL(MDummyConsole* aConsole)
	{
//	iConsole = Console::NewL(KTextSrvIntro,TSize(KConsFullScreen,KConsFullScreen));
	iConsole = aConsole;
	InitSystemPath();
	User::LeaveIfError(iFs.Connect());
	iSourceFileName = iDefaultPath;
	iSourceFileName.Append(KVideoTestSourceFileName);
	}

CMdaCoreTestEngine::~CMdaCoreTestEngine()
	{
	iFs.Close();
//	delete iConsole;
	}

void CMdaCoreTestEngine::RunTestsL()
	{
	CMdaCoreTest* test = NULL;

	iTest = 4; // jf 2/11/01 - go straight to test 5 (only test utilities)

	FOREVER
		{
		switch (iTest)
			{
		case 0:
		case 1:
		case 2:
		case 3:
			ASSERT(EFalse); // tests no longer supported
			break;
		case 4:
			test = new(ELeave) CMdaVideo5Test;
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
		RDEBUG_PRINT1(KTextFinishedTestSuccess);
		}
	else
		{
		iConsole->Printf(KTextFinishedTestFail,iTestsFailed);
		RDEBUG_PRINT2(KTextFinishedTestFail,iTestsFailed);
		}

//	iConsole->Getch();
	}

void CMdaCoreTestEngine::TestFailed(TInt aError)
	{
	TBuf<64> msg(KTextFailedTest);
	msg.Append(' ');
	msg.AppendNum(aError);
	iConsole->Printf(msg);
	RDEBUG_PRINT1(msg);
//	iConsole->Getch();
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
	RDEBUG_PRINT1(format);
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

void CMdaCoreTest::CheckBitmaps(CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2,TInt aQuarterTurns)
	{
	const TSize size = aBitmap1.SizeInPixels();

	if (aQuarterTurns == 0)
		{
		for (TInt y = 0; y < size.iHeight; y++)
			{
			for (TInt x = 0; x < size.iWidth; x++)
				{
				TPoint pt(x,y);
				TRgb color1,color2;
				aBitmap1.GetPixel(color1,pt);
				aBitmap2.GetPixel(color2,pt);

				if (color1 != color2)
					{
					iEngine->TestFailed(KErrCorrupt);
					return;
					}
				}
			}
		}
	else
		{
		for (TInt y = 0; y < size.iHeight; y++)
			{
			for (TInt x = 0; x < size.iWidth; x++)
				{
				TPoint pt(x,y);
				TRgb color1,color2;
				aBitmap1.GetPixel(color1,pt);
				if (aQuarterTurns == 1)
					aBitmap2.GetPixel(color2,TPoint(size.iHeight - y - 1,x));
				else if (aQuarterTurns == 2)
					aBitmap2.GetPixel(color2,TPoint(size.iWidth - x - 1,size.iHeight - y - 1));
				else if (aQuarterTurns == 3)
					aBitmap2.GetPixel(color2,TPoint(y,size.iWidth - x - 1));

				if (color1 != color2)
					{
					iEngine->TestFailed(KErrCorrupt);
					return;
					}
				}
			}
		}
	}

