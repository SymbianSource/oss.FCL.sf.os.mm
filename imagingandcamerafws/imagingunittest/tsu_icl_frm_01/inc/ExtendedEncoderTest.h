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

#if (!defined __EXTENDED_ENCODER_TEST_H__)
#define __EXTENDED_ENCODER_TEST_H__  

#include "ICLFrameWorkTestStep.h"

class CExtendedEncoder;

class RCExtendedEncoderTest : public RICLFrameworkTestStep
	{
public:
	static RCExtendedEncoderTest* NewL(CTestSuite* aSuite, TBool aUseThread);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RCExtendedEncoderTest(CTestSuite* aSuite, TBool aUseThread);

private:
	CExtendedEncoder* iEncoder;
	
	RFs			iFileSession;
	HBufC8*		iDestinationData;
	TUid		iImageType;
	TUid		iImageSubType;
	TUid		iEncoderUid;

	const TBool	iUseThread;

	CActiveScheduler* iScheduler ;
	};


#endif // __EXTENDED_ENCODER_TEST_H__
