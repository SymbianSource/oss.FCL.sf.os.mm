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
// This file contains the test steps ICL negative testing
// 
//



#ifndef	__TESTNEGATIVECONVERSION__
#define	__TESTNEGATIVECONVERSION__


// EPOC includes
#include "TestStepConversion.h"

class CTestNegativeConversion : public CTestStepConversion, public MFbsExtendedBitmapInitializer
{
public:
	CTestNegativeConversion(const TDesC& aTestName) : 
	  CTestStepConversion(aTestName) {}

	static CTestNegativeConversion *NewL(const TDesC& aTestName);
	  
	TVerdict	Step0501_CP();
	TVerdict	Step0502_LP();
	TVerdict	Step0503_HP();
	TVerdict	Step0504_HP();
	TVerdict	Step0505_HP();
	TVerdict	Step0506_LP();
	TVerdict	Step0507_LP();
	TVerdict	Step0508_HP();
	TVerdict	Step0509_LP();
	TVerdict	Step0510_LP();
	TVerdict	Step0511_LP();
	TVerdict	Step0512_LP_L();
	TVerdict	Step0513_HP();

	TVerdict	Step0602_HP();
	TVerdict	Step0603_HP();
	TVerdict	Step0604_HP();
	TVerdict	Step0605_HP();
	TVerdict	Step0606_HP();
	TVerdict	Step0607_HP();

	TVerdict	Step0020_CP();
	TVerdict    Step0700_CP();
	TInt InitExtendedBitmap(TAny* aData, TInt aDataSize); //ExtendedBitmap initialization callback

protected:
	TVerdict	DoTestStepL();


};

#endif
