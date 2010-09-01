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

#ifndef __TESTSUITEMEDIASVR_H__
#define __TESTSUITEMEDIASVR_H__

#include <testframework.h>

/**
 *
 * CTestSuiteMediaSvr, which is the base class for 
 * the MediaSvr suite.
 *
 * @lib "TSU_ICL_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class  CTestSuiteMediaSvr : public CTestSuite
	{
public:
	void InitialiseL();
	TPtrC GetVersion() const;
	};


#endif // __TESTSUITEMEDIASVR_H__
