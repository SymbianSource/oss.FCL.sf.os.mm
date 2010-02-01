// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The sole purpose of this class is to connect to RFbsSession on 
// Construction and Disconnect from RFbsSession on Destruction. 
// 
//

#ifndef __ICLFBSSESSIONTEST_H__
#define __ICLFBSSESSIONTEST_H__

#include <fbs.h>
#include <e32std.h>

class CICLFbsSessionTest: public CBase
	{
private:
	void ConnectToSessionL();	//	Makes an actual Connection to RFbsSession		
	CICLFbsSessionTest();		// Default Constructor

public:
	~CICLFbsSessionTest();		// Default Destructor
	static CICLFbsSessionTest* NewLC();
	};

#include "ICLFbsSessionTest.inl"

#endif //__ICLFBSSESSIONTEST_H__
