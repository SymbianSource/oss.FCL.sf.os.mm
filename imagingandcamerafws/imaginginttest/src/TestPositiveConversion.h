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
// This file contains the test steps ICL positive testing
// 
//


#ifndef	__TESTPOSITIVECONVERSION__
#define	__TESTPOSITIVECONVERSION__


// EPOC includes
#include "TestStepConversion.h"

class CTestPositiveConversion : public CTestStepConversion
{
public:
	static CTestPositiveConversion* NewL(const TDesC& aTestName);
	TVerdict	Step0001_0010_CP();
	TVerdict	Step0011_0014_CP();
	TVerdict	Step0015_CP();
	TVerdict	Step0016_CP();
	TVerdict	Step0017_CP();
//-------
	TVerdict	Step0101_LP();
	TVerdict	Step0102_LP();
	TVerdict	Step0103_LP();
	TVerdict	Step0104_LP();
	TVerdict	Step0105_LP();
	TVerdict	Step0106_HP();
	TVerdict	Step0107_HP();
//-----
	TVerdict	Step0109_HP();
	TVerdict	Step0110_HP();
	TVerdict	Step0111_HP();
	TVerdict	Step0112_HP();
	TVerdict	Step0113_CP();
 	TVerdict	Step0114_HP();
 	TVerdict	Step0115_HP();

	TVerdict	Step0200_CP();
	TVerdict 	Step0300_HP();

	TVerdict	Step0600_HP();
	TVerdict	Step0601_HP();
	TVerdict	Step0608_HP();
	TVerdict	Step0609_HP();
	TVerdict	Step0610_HP();
	TVerdict	Step0611_HP();
	
	TVerdict	Step0612_HP();
	TVerdict 	Step0613_HP();
	TVerdict	Step0614_HP();
	TVerdict	Step0615_HP();
	TVerdict	Step0616_HP();
	TVerdict	Step0617_HP();

protected:
	CTestPositiveConversion(const TDesC& aTestName) : 
		 CTestStepConversion(aTestName)
			 { iHeapSize = 500000; }
	virtual TVerdict	DoTestStepL();

	TInt EncodeImageWithMinParamsL(const TDesC& aFilename, TUid aImageTypeId);
	TInt EncodePpmImageParamsL(const TDesC& aFilename, TUid aImageTypeId, const TUid aPpmEncoderUid = KNullUid);
};

#endif
