
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
// ICL Codec Tests - see ../UnitTest/ICL/Documentation/ICL Codecs Unit Test Specification.xls
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>
#include <barsc.h>

#include "ICLCodecTestStep.h"
#include "ICLCodecTestSuite.h"

/* 
 * NewTestSuite is exported at ordinal 1
 * this provides the interface to allow schedule test
 * to create instances of this test suite
 *
 */
EXPORT_C CTestSuite* NewTestSuiteL()
    {
	CICLCodecSuite* result = CICLCodecSuite::NewL();
	return result;
    }

CICLCodecSuite::CICLCodecSuite()
	{
	}

CICLCodecSuite* CICLCodecSuite::NewL()
    {
	CICLCodecSuite* result = new (ELeave) CICLCodecSuite;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result); // result
	return result;
    }

CICLCodecSuite::~CICLCodecSuite()
	{
	}

TPtrC CICLCodecSuite::GetVersion( void )
	{
	_LIT(KTxtVersion,"1.0");
	return KTxtVersion();
	}

void CICLCodecSuite::InitialiseL()
	{
	// store the name of this test suite 
	iSuiteName = _L("TSU_ICL_COD_02");

	// Set the default path
	InitSystemPath();

	// PNG write codec tests
	// N.B. tests that cause a CEZCompressor to be created require a heap size
	// of approx 450K (CEZCompressor allocs 399032 bytes on construction)
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0201"), ECOD_0201));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0202"), ECOD_0202));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0203"), ECOD_0203));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0204"), ECOD_0204));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0205"), ECOD_0205, 450000));
// Excluded - not used
//	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0206"), ECOD_0206));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0207"), ECOD_0207));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0208"), ECOD_0208));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0209"), ECOD_0209));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0210"), ECOD_0210, 450000));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0211"), ECOD_0211));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0212"), ECOD_0212, 450000));
	// Negative tests
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0401"), ECOD_0401));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0402"), ECOD_0402, 450000));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0403"), ECOD_0403, 450000));
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0404"), ECOD_0404, 450000));
	// Test for opening png files with invalid color type and/or invalid bit depth
	// Fix for DEF040352.
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0501"), ECOD_0501));
	// Test for cancelling decoder at the same time as it finishes with an error
	// Fix for DEF067367.
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0502"), ECOD_0502, 1024*128));
	// Test for cancelling encoder at the same time as it finishes with an error
	// Fix for DEF067367.
	AddTestStepL(RPngWriteCodecTest::NewL(_L("MM-ICL-COD-U-0503"), ECOD_0503));

	// PNG write sub-codec tests
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0230"), ECOD_0230));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0231"), ECOD_0231));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0232"), ECOD_0232));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0233"), ECOD_0233));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0234"), ECOD_0234));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0235"), ECOD_0235));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0236"), ECOD_0236));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0237"), ECOD_0237));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0238"), ECOD_0238));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0239"), ECOD_0239));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0240"), ECOD_0240));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0241"), ECOD_0241));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0242"), ECOD_0242));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0243"), ECOD_0243));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0244"), ECOD_0244));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0245"), ECOD_0245));
	AddTestStepL(RPngWriteSubCodecTest::NewL(this, _L("MM-ICL-COD-U-0250"), ECOD_0250));
	
	// CImageDecoder::FrameInfoStringsL() test
	AddTestStepL(RCodecDataTest::NewL(this, _L("MM-ICL-COD-U-2000-HP"),ECOD_2000));
	// CImageDecoder::ImageType() test
	AddTestStepL(RCodecDataTest::NewL(this, _L("MM-ICL-COD-U-2001-HP"),ECOD_2001));
	// CImageDecoder::GetMimeTypeFileL() test
	AddTestStepL(RCodecDataTest::NewL(this, _L("MM-ICL-COD-U-2002-HP"),ECOD_2002));
	// CImageDecoder::GetMimeTypeDataL() test
	AddTestStepL(RCodecDataTest::NewL(this, _L("MM-ICL-COD-U-2003-HP"),ECOD_2003));
	// RApaLsSession::AppForDocument() mime type test
	AddTestStepL(RCodecDataTest::NewL(this, _L("MM-ICL-COD-U-2004-HP"),ECOD_2004));
	// CApaIclRecognizer::DoRecognize() mime type test
	AddTestStepL(RCodecDataTest::NewL(this, _L("MM-ICL-COD-U-2005-HP"),ECOD_2005));
	// INC093688: non-standard MIME type support ("image/jpg")
	AddTestStepL(RRecogNonStandardMimeTest::NewL(this, _L("MM-ICL-COD-U-2006-HP")));
	// PDEF111744: WMF plugin should not rely on RFbsBitmap::Connect() having been called 
	AddTestStepL(RRecogNonStandardWMFTest::NewL(this, _L("MM-ICL-COD-U-2011-HP")));

	// PDEF112277:V&V:IOP:CONTENT: .wbm not recognized as image file 
 	AddTestStepL(RRecogNonStandardWBMTest::NewL(this, _L("MM-ICL-COD-U-2012-HP")));

	// ICL recognizer alloc test
	AddTestStepL(RRecogAllocTest::NewL(_L("MM-ICL-COD-U-4001-HP"),ECOD_4001));

	// ICL recognizer neg test
	AddTestStepL(RRecogNegTest::NewL(this, _L("MM-ICL-COD-U-0410-HP")));

	DummyCalls();
	}

/**
 * Scan the drives in order e: d: c: z: for testdata for this suite
 * and set the default path to the first it finds.
 */
void CICLCodecSuite::InitSystemPath()
	{
	_LIT(KTSU_ICL_COD_02PathOnE,"e:\\TSU_ICL_COD_02\\");
	_LIT(KTSU_ICL_COD_02PathOnD,"d:\\TSU_ICL_COD_02\\");
	_LIT(KTSU_ICL_COD_02PathOnC,"c:\\TSU_ICL_COD_02\\");
	_LIT(KTSU_ICL_COD_02PathOnZ,"z:\\TSU_ICL_COD_02\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KTSU_ICL_COD_02PathOnE,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_02PathOnE;
	else if (fs.Att(KTSU_ICL_COD_02PathOnD,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_02PathOnD;
	else if (fs.Att(KTSU_ICL_COD_02PathOnC,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_02PathOnC;
	else if (fs.Att(KTSU_ICL_COD_02PathOnZ,att) == KErrNone)
		iDefaultPath = KTSU_ICL_COD_02PathOnZ;
	else
		User::Panic(_L("Test files not found"),KErrNotFound);

	fs.Close();
	}

/*
 * Dummy calls to keep the compiler/linker happy
 */
void CICLCodecSuite::DummyCalls()
	{
	RResourceFile resourceFile;
	resourceFile.Close();
	}

//Use E32Dll from PngConvert.cpp
// DLL requirement
//GLDEF_C TInt E32Dll(enum TDllReason)
//	{
//	return 0;
//	}					    

const TDesC& CICLCodecSuite::DefaultPath() const
	{
	return iDefaultPath;
	}
