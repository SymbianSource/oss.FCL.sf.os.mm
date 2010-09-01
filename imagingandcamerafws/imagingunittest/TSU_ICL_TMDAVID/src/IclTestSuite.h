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

#ifndef __ICLTESTSUITE_H__
#define __ICLTESTSUITE_H__

class CIclTestSuite : public CTestSuite
	{
public:
	static CIclTestSuite* NewL();
	~CIclTestSuite();
protected:
	CIclTestSuite();
	// frm CTestSuite
	void InitialiseL();
	TPtrC GetVersion();
	};

#endif // __ICLTESTSUITE_H__

