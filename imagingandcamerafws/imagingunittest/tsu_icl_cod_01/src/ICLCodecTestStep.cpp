
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

#include "ICLCodecTestStep.h"

struct TTestType
	{
	const TText* iTestName;
	TImageCall   iCallL;
	TImageFormat iFormat;
	TBool iIsAllocTest;
	TInt iHeapSize;
	const TText* iTestFileName;
	TBool iUseThread;
	};

// Data values for Panic tests
_LIT(KDummyImageFilename,"Dummy.jpg"); // Dummy filename, image doesn't exist
const TUid KDummyImageSubType = {0x90238943}; // Dummy value
const TUid KJPGEncoderImplementationUid = {0x101F45D9}; //KJPGEncoderImplementationUidValue

// table of tests

const TInt KDefaultHeapSize=16*1024*1024; // 16M - what we used for TImage

const TTestType KTestDetails[] =
	{
	// straight encode test
	{ _S("MM-ICL-COD-U-0011-HP"), &TImage::Image1L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0014-HP"), &TImage::Image1L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0015-HP"), &TImage::Image1L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0016-HP"), &TImage::Image1L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	// straight encode test running under thread
	{ _S("MM-ICL-COD-U-1011-HP"), &TImage::Image1L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	// main decode test
	{ _S("MM-ICL-COD-U-0021-HP"), &TImage::Image2L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0022-HP"), &TImage::Image2L, EGIF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0023-HP"), &TImage::Image2L, EICO, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0024-HP"), &TImage::Image2L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0025-HP"), &TImage::Image2L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0026-HP"), &TImage::Image2L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0027-HP"), &TImage::Image2L, ETFF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0028-HP"), &TImage::Image2L, EWMF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0029-HP"), &TImage::Image2L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-029A-HP"), &TImage::ImageIncrL, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-029B-HP"), &TImage::ImageFrameIncrL, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-029C-HP"), &TImage::ImageFrameIncrL, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	// main decode test running under thread
	{ _S("MM-ICL-COD-U-1021-HP"), &TImage::Image2L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	// decode via descriptor
	{ _S("MM-ICL-COD-U-0031-HP"), &TImage::Image3L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0032-HP"), &TImage::Image3L, EGIF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0033-HP"), &TImage::Image3L, EICO, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0034-HP"), &TImage::Image3L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0035-HP"), &TImage::Image3L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0036-HP"), &TImage::Image3L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0037-HP"), &TImage::Image3L, ETFF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0038-HP"), &TImage::Image3L, EWMF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0039-HP"), &TImage::Image3L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	// decode via descriptor running under thread
	{ _S("MM-ICL-COD-U-1031-HP"), &TImage::Image3L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	// encode via descriptor
	{ _S("MM-ICL-COD-U-0041-HP"), &TImage::Image4L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0044-HP"), &TImage::Image4L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0045-HP"), &TImage::Image4L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0046-HP"), &TImage::Image4L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	// encode via descriptor running under thread
	{ _S("MM-ICL-COD-U-1041-HP"), &TImage::Image4L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	// load with alloc fail
	{ _S("MM-ICL-COD-U-0051-HP"), &TImage::Image5L, EBMP, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0052-HP"), &TImage::Image5L, EGIF, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0053-HP"), &TImage::Image5L, EICO, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0054-HP"), &TImage::Image5L, EJPG, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0055-HP"), &TImage::Image5L, EMBM, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0056-HP"), &TImage::Image5L, EPNG, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0057-HP"), &TImage::Image5L, ETFF, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0058-HP"), &TImage::Image5L, EWMF, ETrue, KDefaultHeapSize, NULL, EFalse},
	// load with alloc fail running under thread
	{ _S("MM-ICL-COD-U-1051-HP"), &TImage::Image5L, EBMP, ETrue, KDefaultHeapSize, NULL, ETrue},
	// streamed decode
	{ _S("MM-ICL-COD-U-0061-HP"), &TImage::Image6L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0062-HP"), &TImage::Image6L, EGIF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0063-HP"), &TImage::Image6L, EICO, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0064-HP"), &TImage::Image6L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0065-HP"), &TImage::Image6L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0066-HP"), &TImage::Image6L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0067-HP"), &TImage::Image6L, ETFF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0068-HP"), &TImage::Image6L, EWMF, EFalse, KDefaultHeapSize, NULL, EFalse},
	// streamed decode with thread with thread
	{ _S("MM-ICL-COD-U-1061-HP"), &TImage::Image6L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	// descriptor and file (/1 and /8) streamed decode with alloc fail
// The following tests are excluded (redundant tests)
//	{ _S("MM-ICL-COD-U-00b1-HP"), &TImage::Image6BL, EBMP, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b2-HP"), &TImage::Image6BL, EGIF, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b3-HP"), &TImage::Image6BL, EICO, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b4-HP"), &TImage::Image6BL, EJPG, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b5-HP"), &TImage::Image6BL, EMBM, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b6-HP"), &TImage::Image6BL, EPNG, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b7-HP"), &TImage::Image6BL, ETFF, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00b8-HP"), &TImage::Image6BL, EWMF, ETrue, KDefaultHeapSize, NULL, EFalse},
	// encode with alloc fail
	{ _S("MM-ICL-COD-U-0071-HP"), &TImage::Image7L, EBMP, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0074-HP"), &TImage::Image7L, EJPG, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0075-HP"), &TImage::Image7L, EMBM, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0076-HP"), &TImage::Image7L, EPNG, ETrue, KDefaultHeapSize, NULL, EFalse},
	// encode with alloc fail under thread
	{ _S("MM-ICL-COD-U-1071-HP"), &TImage::Image7L, EBMP, ETrue, KDefaultHeapSize, NULL, ETrue},
	// decode ota and wbmp
	{ _S("MM-ICL-COD-U-0089-HP"), &TImage::Image8L, EOTA, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-008a-HP"), &TImage::Image8L, EWBP, EFalse, KDefaultHeapSize, NULL, EFalse},
	// masked decode
	{ _S("MM-ICL-COD-U-0090-HP"), &TImage::Image9L, EANY, EFalse, KDefaultHeapSize, NULL, EFalse},
	// load with mime types with alloc fail
	{ _S("MM-ICL-COD-U-00c1-HP"), &TImage::Image12L, EBMP, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00c2-HP"), &TImage::Image12L, EGIF, ETrue, KDefaultHeapSize, NULL, EFalse},
// The following tests are excluded (redundant tests)
//	{ _S("MM-ICL-COD-U-00c3-HP"), &TImage::Image12L, EICO, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00c4-HP"), &TImage::Image12L, EJPG, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00c5-HP"), &TImage::Image12L, EMBM, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00c6-HP"), &TImage::Image12L, EPNG, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00c7-HP"), &TImage::Image12L, ETFF, ETrue, KDefaultHeapSize, NULL, EFalse},
//	{ _S("MM-ICL-COD-U-00c8-HP"), &TImage::Image12L, EWMF, ETrue, KDefaultHeapSize, NULL, EFalse},

	// load with mime types 
	{ _S("MM-ICL-COD-U-00E1-HP"), &TImage::Image15L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},

	// load with mime types with alloc fail with thread
	{ _S("MM-ICL-COD-U-10c1-HP"), &TImage::Image12L, EBMP, ETrue, KDefaultHeapSize, NULL, ETrue},
	// file /1 streamed decode with alloc fail
	{ _S("MM-ICL-COD-U-00d1-HP"), &TImage::Image13L, EBMP, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d2-HP"), &TImage::Image13L, EGIF, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d3-HP"), &TImage::Image13L, EICO, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d4-HP"), &TImage::Image13L, EJPG, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d5-HP"), &TImage::Image13L, EMBM, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d6-HP"), &TImage::Image13L, EPNG, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d7-HP"), &TImage::Image13L, ETFF, ETrue, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-00d8-HP"), &TImage::Image13L, EWMF, ETrue, KDefaultHeapSize, _S("wmf4_1.wmf"), EFalse},
	// file /1 streamed decode with alloc fail with thread
	{ _S("MM-ICL-COD-U-10d1-HP"), &TImage::Image13L, EBMP, ETrue, KDefaultHeapSize, NULL, ETrue},
	// /8 normal decode with alloc fail
	{ _S("MM-ICL-COD-U-01a1-HP"), &TImage::Image10L, EBMP, ETrue, KDefaultHeapSize, _S("bmp4bpp_4.bmp"), EFalse},
	// descriptor /1 streamed decode with alloc fail
	{ _S("MM-ICL-COD-U-01e1-HP"), &TImage::Image14L, EBMP, ETrue, KDefaultHeapSize, _S("bmp4bpp_4.bmp"), EFalse},
	// file decoder with timed cancel test
	{ _S("MM-ICL-COD-U-08d1-HP"), &TImage::Image16L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d2-HP"), &TImage::Image16L, EGIF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d3-HP"), &TImage::Image16L, EICO, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d4-HP"), &TImage::Image16L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d5-HP"), &TImage::Image16L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d6-HP"), &TImage::Image16L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d7-HP"), &TImage::Image16L, ETFF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d8-HP"), &TImage::Image16L, EWMF, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-08d9-HP"), &TImage::Image16L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	// file decoder with timed cancel test with thread
	{ _S("MM-ICL-COD-U-18d1-HP"), &TImage::Image16L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d2-HP"), &TImage::Image16L, EGIF, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d3-HP"), &TImage::Image16L, EICO, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d4-HP"), &TImage::Image16L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d5-HP"), &TImage::Image16L, EMBM, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d6-HP"), &TImage::Image16L, EPNG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d7-HP"), &TImage::Image16L, ETFF, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-18d8-HP"), &TImage::Image16L, EWMF, EFalse, KDefaultHeapSize, NULL, ETrue},
	// descriptor decoder with timed cancel test with thread
	{ _S("MM-ICL-COD-U-38d1-HP"), &TImage::Image16BL, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d2-HP"), &TImage::Image16BL, EGIF, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d3-HP"), &TImage::Image16BL, EICO, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d4-HP"), &TImage::Image16BL, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d5-HP"), &TImage::Image16BL, EMBM, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d6-HP"), &TImage::Image16BL, EPNG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d7-HP"), &TImage::Image16BL, ETFF, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-38d8-HP"), &TImage::Image16BL, EWMF, EFalse, KDefaultHeapSize, NULL, ETrue},
	// encode via descriptor with cancel (blank bitmap)
	{ _S("MM-ICL-COD-U-0841-HP"), &TImage::Image17L, EBMP, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0844-HP"), &TImage::Image17L, EJPG, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0845-HP"), &TImage::Image17L, EMBM, EFalse, KDefaultHeapSize, NULL, EFalse},
	{ _S("MM-ICL-COD-U-0846-HP"), &TImage::Image17L, EPNG, EFalse, KDefaultHeapSize, NULL, EFalse},
	// encode via descriptor with cancel (actual bitmap)
	{ _S("MM-ICL-COD-U-0942-HP"), &TImage::Image17L, EGIF, EFalse, KDefaultHeapSize, _S("imagevid_8.mbm"), EFalse},
	// encoder with timed cancel test with thread
	{ _S("MM-ICL-COD-U-1841-HP"), &TImage::Image17L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-1844-HP"), &TImage::Image17L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-1845-HP"), &TImage::Image17L, EMBM, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-1846-HP"), &TImage::Image17L, EPNG, EFalse, KDefaultHeapSize, NULL, ETrue},
	// additional BMP tests CR0672
	{ _S("MM-ICL-COD-U-3201-HP"), &TImage::Image18L, EBMP, EFalse, KDefaultHeapSize, NULL, ETrue},
	// ICL extensions decoder alloc tests
	{ _S("MM-ICL-COD-U-1081-HP"), &TImage::Image19L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-1082-HP"), &TImage::Image20L, EICO, EFalse, KDefaultHeapSize, _S("icoepoc.ico"), ETrue},
	// ICL extensions encoder alloc tests
	{ _S("MM-ICL-COD-U-1083-HP"), &TImage::Image21L, EJPG, EFalse, KDefaultHeapSize, NULL, ETrue},
	{ _S("MM-ICL-COD-U-1084-HP"), &TImage::Image22L, EPNG, EFalse, KDefaultHeapSize, _S("temp.bmp"), ETrue},
	{ _S("MM-ICL-COD-U-1085-HP"), &TImage::Image21L, EEXIF, EFalse, KDefaultHeapSize, NULL, ETrue},
	};

RICLCodecStep::RICLCodecStep(TInt aIndex):
	iIndex(aIndex)
	{
	const TTestType& testDetails = KTestDetails[iIndex];
	iTestStepName = testDetails.iTestName;
	iCallL = testDetails.iCallL;
	iFormat = testDetails.iFormat;
	iIsAllocTest = testDetails.iIsAllocTest;
	iHeapSize = testDetails.iHeapSize;
	iTestFileName = testDetails.iTestFileName;
	iUseThread = testDetails.iUseThread;
	}

RICLCodecStep* RICLCodecStep::NewL(TInt aIndex)
	{
	const TInt KNumTestSteps = sizeof(KTestDetails) / sizeof(TTestType);
	if (aIndex>=KNumTestSteps)
		User::Leave(KErrOverflow);
	RICLCodecStep* result = new (ELeave) RICLCodecStep(aIndex);
	return result;
	}

TVerdict RICLCodecStep::OpenL()
	{
	if (iIsAllocTest)
		{
		// see if alloc fail stuff is enabled - ask if next alloc test will fail
		__UHEAP_SETFAIL(RHeap::EFailNext, 1);
		TAny *testAlloc = User::Alloc(1); 
		TBool workingOk = testAlloc == NULL;
		User::Free(testAlloc);
		__UHEAP_SETFAIL(RHeap::ENone ,0);

		if (!workingOk)
			{
			ERR_PRINTF1(_L("Alloc Testing does not work on this build"));
			return EInconclusive;
			}
		}

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	iTImage = new (ELeave) TImage;
	
	if (iTestStepName == _L("MM-ICL-COD-U-00d4-HP"))
		{
		iTImage->InitSystemPath4();	
		}
	else if (iTestStepName == _L("MM-ICL-COD-U-00d6-HP"))
		{
		iTImage->InitSystemPath6();	
		}
	else
		{
		iTImage->InitSystemPath();	
		}
	
	
	iTImage->SetLog(iSuite->LogSystem());
	iTImage->SetLinkToVerdict(iTestResult);
	iTImage->ConstructL();
	iTImage->SetUseThread(iUseThread);
	return EPass;
	}

void RICLCodecStep::Close()
	{
	if (iTImage)
		iTImage->End();
	delete iTImage; iTImage = NULL;

	delete iScheduler; iScheduler = NULL;

	RFbsSession::Disconnect();
	}

TVerdict RICLCodecStep::DoTestStepL( void )
	{
	TInt decuid = 0;
	iTImage->iDecoderUid.iUid = 0;
	ASSERT(iTImage); // should have been created by now
	iTestResult = EPass;
	// last 7 chars of the test case number is the name of the section in the configuration file
    iSectionName = iTestStepName.Right(KSectionNameLen);
	
	//if true, read the decoderuid if false pick the default uid		
	if ((!GetHexFromConfig(iSectionName, _L("decoderuid"), decuid)))
		{
		if(decuid != 0)
			{
			iTImage->iDecoderUid.iUid =	decuid;
			}
		}

	TPtrC testFileName;
	if (iTestFileName)
		testFileName.Set(iTestFileName);
	if(iTestStepName.Compare(_L("MM-ICL-COD-U-029B-HP")) == 0)
		{
		iTImage->iStepName = iTestStepName;
		}
	if(iTestStepName.Compare(_L("MM-ICL-COD-U-029C-HP")) == 0)
		{
		iTImage->iStepName = iTestStepName;
		}		
	(iTImage->*iCallL)(iFormat, testFileName);
	return iTestResult;
	}


//
// add missing bit of TImage
//
void TImage::TestL()
	{
	// this function should not be called in the testframework
	User::Panic(_L("TImage::TestL()"), 0);
	}


//
// PANIC TESTS
//
CTestStepICLCodecPanic::CTestStepICLCodecPanic()
	: iFs()
	{
	}

CTestStepICLCodecPanic::~CTestStepICLCodecPanic()
	{
	}

TVerdict CTestStepICLCodecPanic::DoTestStepPreambleL()
	{
	return EPass;
	}

TVerdict CTestStepICLCodecPanic::DoTestStepPostambleL()
	{
	return EPass;
	}

CTestStepICLCodecPanic_1::CTestStepICLCodecPanic_1()
	{
	_LIT(KPanicTestStep1, "MM-ICL-COD-U-001-HP");
	iTestStepName = KPanicTestStep1;
	}

CTestStepICLCodecPanic_1::~CTestStepICLCodecPanic_1()
	{
	}

TVerdict CTestStepICLCodecPanic_1::DoTestStepL()
	{
	INFO_PRINTF1(_L("Creating a new CImageDecoder with an image sub-type BUT no image type should panic with EIllegalImageSubType\n"));
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, KDummyImageFilename, CImageDecoder::EOptionNone, KNullUid, KDummyImageSubType, KNullUid);
	
	delete decoder;
	
	// Fail the test if we reach this point
	return EFail;
	}

CTestStepICLCodecPanic_2::CTestStepICLCodecPanic_2()
	{
	_LIT(KPanicTestStep2, "MM-ICL-COD-U-002-HP");
	iTestStepName = KPanicTestStep2;
	}

CTestStepICLCodecPanic_2::~CTestStepICLCodecPanic_2()
	{
	}

TVerdict CTestStepICLCodecPanic_2::DoTestStepL()
	{
	INFO_PRINTF1(_L("Creating a new CImageEncoder with no image type AND no encoder UID should panic with EIllegalImageType\n"));
	CImageEncoder* encoder = CImageEncoder::FileNewL(iFs, KDummyImageFilename, CImageEncoder::EOptionNone, KNullUid, KNullUid, KNullUid);
	
	delete encoder;
	
	// Fail the test if we reach this point
	return EFail;
	}

CTestStepICLCodecPanic_3::CTestStepICLCodecPanic_3()
	{
	_LIT(KPanicTestStep3, "MM-ICL-COD-U-003-HP");
	iTestStepName = KPanicTestStep3;
	}

CTestStepICLCodecPanic_3::~CTestStepICLCodecPanic_3()
	{
	}

TVerdict CTestStepICLCodecPanic_3::DoTestStepL()
	{
	INFO_PRINTF1(_L("Creating a new CImageEncoder with an image sub-type BUT no image type should panic with EIllegalImageSubType\n"));
	CImageEncoder* encoder = CImageEncoder::FileNewL(iFs, KDummyImageFilename, CImageEncoder::EOptionNone, KNullUid, KDummyImageSubType, KJPGEncoderImplementationUid); 
	
	delete encoder;
	
	// Fail the test if we reach this point
	return EFail;
	}

