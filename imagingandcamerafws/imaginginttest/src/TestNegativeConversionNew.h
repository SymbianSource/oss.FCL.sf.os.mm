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
// This file contains the test steps ICL negative testing for GT0210 MS3.4 File Handle APIs.
// 
//



#ifndef	__TESTNEGATIVECONVERSIONNEW__
#define	__TESTNEGATIVECONVERSIONNEW__


// EPOC includes
#include "TestStepConversion.h"

class CTestNegativeConversionNew : public CTestStepConversion
{
public:
	CTestNegativeConversionNew(const TDesC& aTestName) : 
	  CTestStepConversion(aTestName) {}

	static CTestNegativeConversionNew *NewL(const TDesC& aTestName);
	  
	TVerdict Step1701_1703_HPL();
	TVerdict Step1704_1706_HPL();
	
protected:
	TVerdict DoTestStepL();

};

#endif
