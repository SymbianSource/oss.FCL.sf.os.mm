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
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>
#include "TS_MM_ICL_FRM_CP.h"
#include "TestSuite_TSI_ICL_FRM_00.h"

#include "TestPositiveConversion.h"
#include "TestNegativeConversion.h"

#include "TestPositiveConversionNew.h"
#include "TestNegativeConversionNew.h"

#include "TestImageDecoderFileSource.h"
#include "TestImageDecoderDRM.h"

#include "TestStepYUVConversion.h"


// Create an instance of this test suite.

// NewTestSuite_TSI_ICL_FRM_00L is exported at ordinal 1.
// This provides the interface to allow the Test Framework
// to create instances of this test suite.
EXPORT_C CTestSuite_TSI_ICL_FRM_00* NewTestSuite_TSI_ICL_FRM_00L() 
    { 
	CTestSuite_TSI_ICL_FRM_00* result = new (ELeave) CTestSuite_TSI_ICL_FRM_00;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(); // result
	return result;
    }

CTestSuite_TSI_ICL_FRM_00::~CTestSuite_TSI_ICL_FRM_00()
	{
	}

_LIT(KTxtVersion,"0.2 (build 29-Aug-2002)");

// Get version
TPtrC CTestSuite_TSI_ICL_FRM_00::GetVersion() const
	{
	return KTxtVersion();
	}

// Initialiser for test suite.
// This creates all the test steps and stores them
// inside CTestSuite
void CTestSuite_TSI_ICL_FRM_00::InitialiseL()
	{
	// store the name of this test suite
	iSuiteName = _L("TSI_ICL_FRM_00");

	// add test steps
	//------------------ positive steps------------------
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0001-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0002-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0003-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0004-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0005-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0006-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0007-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0008-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0009-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0010-CP")));
	
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0011-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0012-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0013-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0014-CP")));
	
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0015-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0200-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0016-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0017-CP")));

	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0101-LP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0102-LP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0103-LP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0104-LP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0105-LP")));

	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0106-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0107-HP")));

	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0109-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0110-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0111-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0112-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0113-CP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0114-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0115-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0300-HP")));

	//------------------negative steps------------------
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0501-CP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0502-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0503-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0504-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0505-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0506-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0507-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0509-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0510-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0511-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0512-LP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0513-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0020-CP")));

	//------------------drm steps------------------
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0600-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0601-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0602-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0603-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0604-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0605-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0606-HP")));
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0607-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0608-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0609-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0610-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0611-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0612-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0613-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0614-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0615-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0616-HP")));
	AddTestStepL(CTestPositiveConversion::NewL(_L("MM-ICL-FRM-I-0617-HP")));


	//------------------ Positive steps for GT0210 MS3.4------------------
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0701-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0702-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0703-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0704-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0705-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0706-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0707-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0708-CP")));

	//------------------ Alloc steps for GT0210 MS3.4------------------
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0709-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0710-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0711-CP")));
	AddTestStepL(CTestPositiveConversionNew::NewL(_L("MM-ICL-FRM-I-0712-CP")));

	//------------------ Negative steps for GT0210 MS3.4------------------
	AddTestStepL(CTestNegativeConversionNew::NewL(_L("MM-ICL-FRM-I-1701-HP")));
	AddTestStepL(CTestNegativeConversionNew::NewL(_L("MM-ICL-FRM-I-1702-HP")));
	AddTestStepL(CTestNegativeConversionNew::NewL(_L("MM-ICL-FRM-I-1703-HP")));
	AddTestStepL(CTestNegativeConversionNew::NewL(_L("MM-ICL-FRM-I-1704-HP")));
	AddTestStepL(CTestNegativeConversionNew::NewL(_L("MM-ICL-FRM-I-1705-HP")));
	AddTestStepL(CTestNegativeConversionNew::NewL(_L("MM-ICL-FRM-I-1706-HP")));
	

    // Tests for TMMFileSource and TMMFileHandleSource
    AddTestStepL(CTestImageDecFileSource::NewL(_L("MM-ICL-FRM-I-0800-HP"))); // TMMFileSource
    AddTestStepL(CTestImageDecFileSource::NewL(_L("MM-ICL-FRM-I-0801-HP"))); // TMMFileSource, mimeType
    AddTestStepL(CTestImageDecFileHandleSource::NewL(_L("MM-ICL-FRM-I-0802-HP"))); // TMMFileHandleSource
    AddTestStepL(CTestImageDecFileHandleSource::NewL(_L("MM-ICL-FRM-I-0803-HP"))); // TMMFileHandleSource, mimeType

    // GT 0222 MS3.6, PREQ777 DRM Support
    AddTestStepL(CTestImageDecDrmEnableAgentUI::NewL(_L("MM-ICL-FRM-I-0761-HP"))); // TMMFileSource

	// GT 0251 PREQ747
	AddTestStepL(CTestConvertYUVRChunk::NewL(_L("MM-ICL-FRM-I-1001-CP")));
	AddTestStepL(CTestConvertYUVRChunk::NewL(_L("MM-ICL-FRM-I-1002-CP")));
	
	AddTestStepL(CTestConvertYUVDesc::NewL(_L("MM-ICL-FRM-I-1003-CP")));
	AddTestStepL(CTestConvertYUVDesc::NewL(_L("MM-ICL-FRM-I-1004-CP")));
	
	AddTestStepL(CTestCSEncodeYUVExtraParams::NewL(_L("MM-ICL-FRM-I-1005-LP")));
	
	AddTestStepL(CTestCSGetPluginUID::NewL(_L("MM-ICL-FRM-I-1006-LP")));
	AddTestStepL(CTestCSGetSupportedYUVFormats::NewL(_L("MM-ICL-FRM-I-1007-LP")));
		
	AddTestStepL(CTestConvertYUVRChunk::NewL(_L("MM-ICL-FRM-I-1008-HP")));
	AddTestStepL(CTestConvertYUVRChunk::NewL(_L("MM-ICL-FRM-I-1009-HP")));
	
	AddTestStepL(CTestConvertYUVDesc::NewL(_L("MM-ICL-FRM-I-1010-HP")));
	AddTestStepL(CTestConvertYUVDesc::NewL(_L("MM-ICL-FRM-I-1011-HP")));
	
	AddTestStepL(CTestCSYUVEncoderNegative::NewL(_L("MM-ICL-FRM-I-1112-CP")));
	AddTestStepL(CTestCSYUVEncoderNegative::NewL(_L("MM-ICL-FRM-I-1113-CP")));
	AddTestStepL(CTestCSYUVEncoderNegative::NewL(_L("MM-ICL-FRM-I-1114-CP")));
	
	//Following two steps(MM-ICL-FRM-I-1115-CP and MM-ICL-FRM-I-1116-CP) are similar with existing 
	//(MM-ICL-FRM-I-1113-CP and MM-ICL-FRM-I-1114-CP), so removed.
	
	//AddTestStepL(CTestCSYUVEncoderNegative::NewL(_L("MM-ICL-FRM-I-1115-CP")));
	//AddTestStepL(CTestCSYUVEncoderNegative::NewL(_L("MM-ICL-FRM-I-1116-CP")));
	
	AddTestStepL(CTestCSYUVDecoderNegative::NewL(_L("MM-ICL-FRM-I-1117-CP")));
	AddTestStepL(CTestCSYUVDecoderNegative::NewL(_L("MM-ICL-FRM-I-1118-CP")));
	AddTestStepL(CTestCSYUVDecoderNegative::NewL(_L("MM-ICL-FRM-I-1119-CP")));
	AddTestStepL(CTestCSYUVDecoderNegative::NewL(_L("MM-ICL-FRM-I-1120-CP")));
	
	AddTestStepL(CTestImageDecFileSourceExtendedBitmap::NewL(_L("MM-ICL-FRM-I-1130-CP")));
	
	AddTestStepL(CTestNegativeConversion::NewL(_L("MM-ICL-FRM-I-0700-CP")));
	
	
	
	
    }
