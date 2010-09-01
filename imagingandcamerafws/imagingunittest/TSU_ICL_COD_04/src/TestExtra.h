
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
// TestExtra.h - TestFramework version
// 
//

#ifndef __TImageBase_h__
#define __TImageBase_h__

#include <testframework.h>

class CLog;

// RTestExtra replaces original RTest

class RTestExtra : public RTestStep
	{
public:
	TVerdict DoTestStepL()
		{
		return EPass;	
		}
	// SetLog - sets the log property - not done on construct for reasons of compatibility with original
	void SetLog(CLog *aLog);
	void SetLinkToVerdict(TVerdict& aVerdict);
	void Printf(TRefByValue<const TDesC> aFmt,...);
protected:
	RTestExtra(const TDesC& aTestName);
	void TestFailed(TInt aError);
	void TestFailed(TRefByValue<const TDesC> aFmt,...);
	void PrintList(TRefByValue<const TDesC> aFmt,VA_LIST aList);
	void Close();
protected:
	CLog* iLog; // not owned
	TVerdict* iVerdict; // not owned
	const TDesC& iTestName;
	};

#endif //__TImageBase_h
