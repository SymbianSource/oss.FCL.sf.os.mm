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
//

#ifndef __TMDACORE_H__
#define __TMDACORE_H__

#include <e32std.h>
#include <e32base.h>
#include <e32test.h>
#include <f32file.h>
#include <fbs.h>
#include <bitdev.h>

class CMdaCoreTestEngine;
class CMdaCoreTest : public CBase
	{
public:
	virtual ~CMdaCoreTest();
	void SetEngine(CMdaCoreTestEngine* aEngine);
	void TestL();
protected:
	void CheckBitmaps(CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2,TInt aQuarterTurns = 0);
private:
	virtual void DoTestL() = 0;
	void SessionClose();
protected:
	CMdaCoreTestEngine* iEngine;
	};

class CMdaCoreTestEngine : public CBase
	{
public:
	static CMdaCoreTestEngine* NewL();
	virtual ~CMdaCoreTestEngine();
	void RunTestsL();
	void TestFailed(TInt aError);
	void Print(const TDesC& aString);
private:
	CMdaCoreTestEngine() {}
private:
	TInt iTest;
	TInt iTestsFailed;
public:
	CConsoleBase* iConsole;
	};

#endif

