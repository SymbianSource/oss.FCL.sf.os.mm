
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
// This main DLL entry point for the TSU_ICL_ImageFrame
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include "TestSuiteImageFrame.h"
#include "TestStepImageFrame.h"

#ifndef EKA2
// required for all DLLs but not used
GLDEF_C TInt E32Dll(enum TDllReason)
	{
	return 0;
	}					    
#endif

/**
 * NewTestSuite is exported at ordinal 1
 * this provides the interface to allow schedule test
 * to create instances of this test suite
 */
EXPORT_C CTestSuiteImageFrame* NewTestSuiteL() 
    { 
	CTestSuiteImageFrame* self = new(ELeave) CTestSuiteImageFrame;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
    }

/**
 * Destructor
 */
CTestSuiteImageFrame::~CTestSuiteImageFrame()
	{
	}

/**
 * Get Test Suite version
 */
TPtrC CTestSuiteImageFrame::GetVersion()
	{
	_LIT(KTxtVersion, "1.0");
	return KTxtVersion();
	}

/**
 * This function initialises the test suite
 * this creates all the test steps and stores them inside CTestSuiteImageFrame 
 */
void CTestSuiteImageFrame::InitialiseL()
	{
	// store the name of this test suite 
	iSuiteName = _L("TSU_ICL_ImageFrame");

	AddTestStepL(RImageFrameDecDesc::NewL(EFalse));
	AddTestStepL(RImageFrameDecChunk::NewL(EFalse));
	AddTestStepL(RImageFrameEncDesc::NewL(EFalse));
	AddTestStepL(RImageFrameEncChunk::NewL(EFalse));
	AddTestStepL(RYuvMonoProcDesc::NewL(EFalse));
	AddTestStepL(RYuvMonoProcChunk::NewL(EFalse));
	AddTestStepL(RYuv422IntlProcDesc::NewL(EFalse));
	AddTestStepL(RYuv422IntlProcChunk::NewL(EFalse));
	AddTestStepL(RYuv420PlnProcDesc::NewL(EFalse));
	AddTestStepL(RYuv420PlnProcChunk::NewL(EFalse));
	AddTestStepL(RYuv420PlnRevProcDesc::NewL(EFalse));
	AddTestStepL(RYuv420PlnRevProcChunk::NewL(EFalse));
	
	AddTestStepL(RImageFrameDescMultiFrame::NewL(EFalse));
	AddTestStepL(RImageFrameRChunkMultiFrame::NewL(EFalse));
	
	AddTestStepL(RFormatTest::NewL(EFalse));
	AddTestStepL(RImageFrameClip::NewL(EFalse));
	
    // alloc tests should run under debug
	AddTestStepL(RImageFrameDecDesc::NewL(ETrue));
	AddTestStepL(RImageFrameDecChunk::NewL(ETrue));
	AddTestStepL(RImageFrameEncDesc::NewL(ETrue));
	AddTestStepL(RImageFrameEncChunk::NewL(ETrue));
	AddTestStepL(RYuvMonoProcDesc::NewL(ETrue));
	AddTestStepL(RYuvMonoProcChunk::NewL(ETrue));
	AddTestStepL(RYuv422IntlProcDesc::NewL(ETrue));
	AddTestStepL(RYuv422IntlProcChunk::NewL(ETrue));
	AddTestStepL(RYuv420PlnProcDesc::NewL(ETrue));
	AddTestStepL(RYuv420PlnProcChunk::NewL(ETrue));
	AddTestStepL(RYuv420PlnRevProcDesc::NewL(ETrue));
	AddTestStepL(RYuv420PlnRevProcChunk::NewL(ETrue));

	}

