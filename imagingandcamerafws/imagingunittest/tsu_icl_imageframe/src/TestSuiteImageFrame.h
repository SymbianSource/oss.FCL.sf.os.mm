
// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Tests for ImageFrame
// 
//

#ifndef TEST_SUITE_IMAGEFRAME_H
#define TEST_SUITE_IMAGEFRAME_H

#include <testframework.h>

class  CTestSuiteImageFrame : public CTestSuite
{
public:
	virtual ~CTestSuiteImageFrame();
	void 	InitialiseL();
	TPtrC 	GetVersion();
};

#endif // TEST_SUITE_IMAGEFRAME_H

