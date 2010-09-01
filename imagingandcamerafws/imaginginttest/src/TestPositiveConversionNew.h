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
// This file contains the test steps ICL positive testing  for GT0210 MS3.4 File Handle APIs.
// 
//


#ifndef	__TESTPOSITIVECONVERSIONNEW__
#define	__TESTPOSITIVECONVERSIONNEW__


// EPOC includes
#include "TestStepConversion.h"

class CTestPositiveConversionNew : public CTestStepConversion
{
public:
	static CTestPositiveConversionNew* NewL(const TDesC& aTestName);

protected:
	CTestPositiveConversionNew(const TDesC& aTestName);

	virtual TVerdict	DoTestStepL();
	TVerdict	Step0701_0704_CPL();
	TVerdict	Step0705_0708_CPL();
	TVerdict	Step0709_0710_CPL();
	TVerdict	Step0711_0712_CPL();
	TVerdict	TestStep0709_0710_CPL();
	TVerdict	TestStep0711_0712_CPL();
};

#endif
