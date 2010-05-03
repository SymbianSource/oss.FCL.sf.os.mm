// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>

// Test system includes
#include <testframework.h>
#include "ICLCodecDefectTestSuite.h"
#include "ICLCodecDefectTestStep.h"



CICLCodecDefectTestSuite::~CICLCodecDefectTestSuite()
	{
	}

EXPORT_C CICLCodecDefectTestSuite* CICLCodecDefectTestSuite::NewL()
    {
	CICLCodecDefectTestSuite* self = new (ELeave) CICLCodecDefectTestSuite;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
    }

_LIT(KTxtVersion,"0.1a (build 02-Aug-2002)");

TPtrC CICLCodecDefectTestSuite::GetVersion() const
	{
	return KTxtVersion();
	}

void CICLCodecDefectTestSuite::InitialiseL()
	{
	// store the name of this test suite
	iSuiteName = _L("TSU_ICL_COD_03");

	InitSystemPath();

	AddTestStepL(RPngDecodeFailureTest::NewL(this));
	AddTestStepL(RTestStep_5002::NewL(this));
	AddTestStepL(RTestStep_5004::NewL(this));
	AddTestStepL(RTestStep_5006::NewL(this));
	AddTestStepL(RTestStep_5008::NewL(this));
	AddTestStepL(RTestStep_5010::NewL(this));
	AddTestStepL(RTestStep_5012::NewL(this));
	AddTestStepL(RTestStep_5014::NewL(this));
	AddTestStepL(RTestStep_5016::NewL(this));
	AddTestStepL(RTestStep_5018::NewL(this));
	AddTestStepL(RTestStep_5019::NewL(this));
	AddTestStepL(RTestStep_5020::NewL(this));
	AddTestStepL(RTestStep_5021::NewL(this));
	AddTestStepL(RTestStep_5022::NewL(this));
	AddTestStepL(RTestStep_5023::NewL(this));
	AddTestStepL(RTestStep_5024::NewL(this));
	AddTestStepL(RTestStep_5025::NewL(this));
	AddTestStepL(RTestStep_5026::NewL(this));

	AddTestStepL(RTestStep_5027::NewL(this, 0));
	AddTestStepL(RTestStep_5027::NewL(this, 1));
	AddTestStepL(RTestStep_5027::NewL(this, 2));

	for (TInt i = 0 ; i<KTest5028_NumTestCases ; i++)
		AddTestStepL(RTestStep_5028::NewL(this, i));

	AddTestStepL(RTestStep_5029::NewL(this));
	AddTestStepL(RTestStep_5058::NewL(this));
	AddTestStepL(RTestStep_5059::NewL(this));
	AddTestStepL(RTestStep_5060::NewL(this));
	AddTestStepL(RTestStep_5061::NewL(this));
	AddTestStepL(RTestStep_5062::NewL(this));
	AddTestStepL(RTestStep_5063::NewL(this));
	AddTestStepL(RTestStep_5064::NewL(this));
	AddTestStepL(RTestStep_5065::NewL(this));
	AddTestStepL(RTestStep_5066::NewL(this));
	AddTestStepL(RTestStep_5067::NewL(this));
	AddTestStepL(RTestStep_5068::NewL(this));
	AddTestStepL(RTestStep_5069::NewL(this));
	AddTestStepL(RTestStep_5070::NewL(this));
	AddTestStepL(RTestStep_5071::NewL(this));
	AddTestStepL(RTestStep_5072::NewL(this));
	AddTestStepL(RTestStep_5073::NewL(this));
	AddTestStepL(RTestStep_5074::NewL(this));
	AddTestStepL(RTestStep_5075::NewL(this));
	AddTestStepL(RTestStep_5076::NewL(this));
	AddTestStepL(RTestStep_5077::NewL(this));
    AddTestStepL(RTestStep_5078::NewL(this));
    AddTestStepL(RTestStep_5079::NewL(this));
    AddTestStepL(RTestStep_5080::NewL(this));
	AddTestStepL(RTestStep_5081::NewL(this));
	AddTestStepL(RTestStep_5082::NewL(this));
	AddTestStepL(RTestStep_5083::NewL(this));
	AddTestStepL(RTestStep_5086::NewL(this));
	AddTestStepL(RTestStep_5088::NewL(this));
	AddTestStepL(RTestStep_5089::NewL(this));
	AddTestStepL(RTestStep_5090::NewL(this));
	AddTestStepL(RTestStep_5091::NewL(this));
	AddTestStepL(RTestStep_5092::NewL(this));
	AddTestStepL(RTestStep_5093::NewL(this));
	AddTestStepL(RTestStep_5094::NewL(this));
	AddTestStepL(RTestStep_5095::NewL(this));
	AddTestStepL(RTestStep_5096::NewL(this));
	AddTestStepL(RTestStep_5097::NewL(this));
	AddTestStepL(RTestStep_5098::NewL(this));
	AddTestStepL(RTestStep_5100::NewL(this));
	AddTestStepL(RTestStep_5101::NewL(this));
	AddTestStepL(RTestStep_5110::NewL(this));
	AddTestStepL(RTestStep_5111::NewL(this));
	AddTestStepL(RTestStep_5099::NewL(this));
	AddTestStepL(RTestStep_5102::NewL(this));
	AddTestStepL(RTestStep_5103::NewL(this));
	AddTestStepL(RTestStep_5104::NewL(this));
	AddTestStepL(RTestStep_5106::NewL(this));
	AddTestStepL(RTestStep_5107::NewL(this));
	AddTestStepL(RTestStep_5112::NewL(this));
	AddTestStepL(RTestStep_5113::NewL(this));
	AddTestStepL(RTestStep_5114::NewL(this));
	AddTestStepL(RTestStep_5108::NewL(this));
	AddTestStepL(RTestStep_5109::NewL(this));
	}

void CICLCodecDefectTestSuite::InitSystemPath()
	{
	_LIT(KTSU_ICL_COD_03PathOnE,"e:\\TSU_ICL_COD_03\\");
	_LIT(KTSU_ICL_COD_03PathOnD,"d:\\TSU_ICL_COD_03\\");
	_LIT(KTSU_ICL_COD_03PathOnC,"c:\\TSU_ICL_COD_03\\");
	_LIT(KTSU_ICL_COD_03PathOnZ,"z:\\TSU_ICL_COD_03\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KTSU_ICL_COD_03PathOnE,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_03PathOnE;
	else if (fs.Att(KTSU_ICL_COD_03PathOnD,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_03PathOnD;
	else if (fs.Att(KTSU_ICL_COD_03PathOnC,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_03PathOnC;
	else if (fs.Att(KTSU_ICL_COD_03PathOnZ,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_03PathOnZ;
	else
		User::Panic(_L("Test files not found"),KErrNotFound);

	fs.Close();
	}

const TDesC& CICLCodecDefectTestSuite::DefaultPath() const
	{
	return iDefaultPath;
	}
