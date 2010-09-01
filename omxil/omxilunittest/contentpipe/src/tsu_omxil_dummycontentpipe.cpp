// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <openmax/il/core/omxilcontentpipeif.h>

#include "../../test_plugins/dummy_cp/src/omxildummycontentpipe.hrh"
#include "../../test_plugins/dummy_cp_2/src/omxildummycontentpipe2.hrh"
#include "tsu_omxil_dummycontentpipe.h"

char* const KTestUri = "http://www.symbian.com";
const TInt KTestOffset = 5;

/**
 *
 * CTestStep_OMXIL_DummyContentPipe constructor
 *
 * @xxxx
 * 
 */
CTestStep_OMXIL_DummyContentPipe::CTestStep_OMXIL_DummyContentPipe() 
	{
	}

/**
 *
 * CTestStep_OMXIL_DummyContentPipe destructor
 *
 * @xxxx
 * 
 */
CTestStep_OMXIL_DummyContentPipe::~CTestStep_OMXIL_DummyContentPipe()
	{
	}

TVerdict CTestStep_OMXIL_DummyContentPipe::DoTestStepPreambleL()
	{
	TVerdict verdict = EPass;

	return verdict;
	}

TVerdict CTestStep_OMXIL_DummyContentPipe::DoTestStepPostambleL()
	{
	TVerdict verdict = EPass;

	return verdict;
	}


CTestStep_OMXIL_DummyContentPipe_U_0001::CTestStep_OMXIL_DummyContentPipe_U_0001()
	/** Constructor
	*/
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// Each test step initialises it's own name
	iTestStepName = _L("MM-OMXIL-DummyContentPipe-U-0001");
	}


TVerdict CTestStep_OMXIL_DummyContentPipe_U_0001::DoTestStepL( void )
/** 
 * Call the COmxILContentPipeIf::CreateImplementationL(..) with the Dummy Content Pipe UID.
 * Call COmxILContentPipeIf::GetHandle(..), verify the return value is 0. 
 * Destroy the COmxILContentPipeIf object and verify there isn't any memory leak.

 * Use case: N/A
 * @test Req. under test REQ8336
 */
	{
	INFO_PRINTF1(_L("Setting UHEAP_MARK"));
	__MM_HEAP_MARK;

	TVerdict verdict = EPass;
	TInt err;
	COmxILContentPipeIf* dummyContentPipeIf = NULL;
	OMX_HANDLETYPE contentPipeHandle;

	INFO_PRINTF1(_L("Attempting to Create Content Pipe Interface"));
	TRAP(err, dummyContentPipeIf = COmxILContentPipeIf::CreateImplementationL(TUid::Uid(KUidOmxILDummyContentPipe)));
	INFO_PRINTF2(_L("Content Pipe Interface Created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}

	INFO_PRINTF1(_L("Attempting to Init Content Pipe"));
	err = dummyContentPipeIf->GetHandle(&contentPipeHandle);
	INFO_PRINTF2(_L("Content Pipe initialised: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	delete dummyContentPipeIf;
	REComSession::FinalClose();

	INFO_PRINTF1(_L("Setting UHEAP_MARKEND"));
	__MM_HEAP_MARKEND;
 
	return verdict;
	}


//------------------------------------------------------------------

CTestStep_OMXIL_DummyContentPipe_U_0002::CTestStep_OMXIL_DummyContentPipe_U_0002()
/** Constructor
*/
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// Each test step initialises it's own name
	iTestStepName = _L("MM-OMXIL-DummyContentPipe-U-0002");
	}


TVerdict CTestStep_OMXIL_DummyContentPipe_U_0002::DoTestStepL( void )
/** 
* Call the COmxILContentPipeIf::CreateImplementationL(..) with the Dummy Content Pipe UID.
* Call COmxILContentPipeIf::GetHandle(..), verify the return value is 0. 
* Call CP_PIPETYPE::Create(..), verify the return value is 0.
* Call the CP_PIPETYPE::Close(), verify the return value is 0.
* Destroy the COmxILContentPipeIf object and verify there isn't any memory leak.

* Use case: N/A
* @test Req. under test REQ8336
*/
	{
	INFO_PRINTF1(_L("Setting UHEAP_MARK"));
	__MM_HEAP_MARK;
	
	TVerdict verdict = EPass;
	TInt err;
	COmxILContentPipeIf* dummyContentPipeIf = NULL;
	OMX_HANDLETYPE contentPipeHandle;
	CPhandle contentSourceHandle;
	
	INFO_PRINTF1(_L("Attempting to Create Content Pipe Interface"));
	TRAP(err, dummyContentPipeIf = COmxILContentPipeIf::CreateImplementationL(TUid::Uid(KUidOmxILDummyContentPipe)));
	INFO_PRINTF2(_L("Content Pipe Interface Created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Init Content Pipe"));
	err = dummyContentPipeIf->GetHandle(&contentPipeHandle);
	INFO_PRINTF2(_L("Content Pipe initialised: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Create Content Source"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->Create(&contentSourceHandle, KTestUri);
	INFO_PRINTF2(_L("Content Source created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Close Content Source"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->Close(contentSourceHandle);
	INFO_PRINTF2(_L("Content Source closed: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	delete dummyContentPipeIf;
	REComSession::FinalClose();
	
	INFO_PRINTF1(_L("Setting UHEAP_MARKEND"));
	__MM_HEAP_MARKEND;
	
	return verdict;
	}


//------------------------------------------------------------------

CTestStep_OMXIL_DummyContentPipe_U_0003::CTestStep_OMXIL_DummyContentPipe_U_0003()
/** Constructor
*/
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// Each test step initialises it's own name
	iTestStepName = _L("MM-OMXIL-DummyContentPipe-U-0003");
	}


TVerdict CTestStep_OMXIL_DummyContentPipe_U_0003::DoTestStepL( void )
/** 
* Call the COmxILContentPipeIf::CreateImplementationL(..) with the Dummy Content Pipe UID.
* Call COmxILContentPipeIf::GetHandle(..), verify the return value is 0. 
* Call CP_PIPETYPE::Create(..), verify the return value is 0.
* Call CP_PIPETYPE::SetPosition(), verify the return value is 0.
* Call CP_PIPETYPE::GetPosition(), verify the return value is 0. Verify the position has been set correctly
* Call the CP_PIPETYPE::Close(), verify the return value is 0.
* Destroy the COmxILContentPipeIf object and verify there isn't any memory leak.

* Use case: N/A
* @test Req. under test REQ8336
*/
	{
	INFO_PRINTF1(_L("Setting UHEAP_MARK"));
	__MM_HEAP_MARK;
	
	TVerdict verdict = EPass;
	TInt err;
	COmxILContentPipeIf* dummyContentPipeIf = NULL;
	OMX_HANDLETYPE contentPipeHandle;
	CPhandle contentSourceHandle;
	
	INFO_PRINTF1(_L("Attempting to Create Content Pipe Interface"));
	TRAP(err, dummyContentPipeIf = COmxILContentPipeIf::CreateImplementationL(TUid::Uid(KUidOmxILDummyContentPipe)));
	INFO_PRINTF2(_L("Content Pipe Interface Created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Init Content Pipe"));
	err = dummyContentPipeIf->GetHandle(&contentPipeHandle);
	INFO_PRINTF2(_L("Content Pipe initialised: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Create Content Source"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->Create(&contentSourceHandle, KTestUri);
	INFO_PRINTF2(_L("Content Source created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Calling SetPosition()"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->SetPosition(contentSourceHandle, KTestOffset, CP_OriginBegin);
	INFO_PRINTF2(_L("SetPosition() called: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Calling GetPosition()..."));
	TUint32 pos;
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->GetPosition(contentSourceHandle, &pos);
	INFO_PRINTF2(_L("GetPosition() called: %d"), err);
	if(err != KErrNone || pos != KTestOffset)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Close Content Source"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->Close(contentSourceHandle);
	INFO_PRINTF2(_L("Content Source closed: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	delete dummyContentPipeIf;
	REComSession::FinalClose();
	
	INFO_PRINTF1(_L("Setting UHEAP_MARKEND"));
	__MM_HEAP_MARKEND;
	
	return verdict;
	}


//------------------------------------------------------------------

CTestStep_OMXIL_DummyContentPipe_U_0004::CTestStep_OMXIL_DummyContentPipe_U_0004()
/** Constructor
*/
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// Each test step initialises it's own name
	iTestStepName = _L("MM-OMXIL-DummyContentPipe-U-0004");
	}


TVerdict CTestStep_OMXIL_DummyContentPipe_U_0004::DoTestStepL( void )
/** 
* Call the COmxILContentPipeIf::CreateImplementationL(..) with the Dummy Content Pipe 2 UID.
* Call COmxILContentPipeIf::GetHandle(..), verify the return value is 0. 
* Call CP_PIPETYPE::Create(..), verify the return value is 0.
* Call CP_PIPETYPE::SetPosition(), verify the return value is 0.
* Call CP_PIPETYPE::GetPosition(), verify the return value is 0. Verify the position has been set correctly
* Call the CP_PIPETYPE::Close(), verify the return value is 0.
* Destroy the COmxILContentPipeIf object and verify there isn't any memory leak.

* Use case: N/A
* @test Req. under test REQ8336
*/
	{
	INFO_PRINTF1(_L("Setting UHEAP_MARK"));
	__MM_HEAP_MARK;
	
	TVerdict verdict = EPass;
	TInt err;
	COmxILContentPipeIf* dummyContentPipeIf = NULL;
	OMX_HANDLETYPE contentPipeHandle;
	CPhandle contentSourceHandle;
	
	INFO_PRINTF1(_L("Attempting to Create Content Pipe Interface"));
	TRAP(err, dummyContentPipeIf = COmxILContentPipeIf::CreateImplementationL(TUid::Uid(KUidOmxILDummyContentPipe2)));
	INFO_PRINTF2(_L("Content Pipe Interface Created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Init Content Pipe"));
	err = dummyContentPipeIf->GetHandle(&contentPipeHandle);
	INFO_PRINTF2(_L("Content Pipe initialised: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Create Content Source"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->Create(&contentSourceHandle, KTestUri);
	INFO_PRINTF2(_L("Content Source created: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Calling SetPosition()"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->SetPosition(contentSourceHandle, KTestOffset, CP_OriginBegin);
	INFO_PRINTF2(_L("SetPosition() called: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Calling GetPosition()..."));
	TUint32 pos;
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->GetPosition(contentSourceHandle, &pos);
	INFO_PRINTF2(_L("GetPosition() called: %d"), err);
	if(err != KErrNone || pos != KTestOffset)
		{
		verdict = EFail;
		}
	
	INFO_PRINTF1(_L("Attempting to Close Content Source"));
	err = (reinterpret_cast<CP_PIPETYPE*>(contentPipeHandle))->Close(contentSourceHandle);
	INFO_PRINTF2(_L("Content Source closed: %d"), err);
	if(err != KErrNone)
		{
		verdict = EFail;
		}
	
	delete dummyContentPipeIf;
	REComSession::FinalClose();
	
	INFO_PRINTF1(_L("Setting UHEAP_MARKEND"));
	__MM_HEAP_MARKEND;
	
	return verdict;
	}


//------------------------------------------------------------------
