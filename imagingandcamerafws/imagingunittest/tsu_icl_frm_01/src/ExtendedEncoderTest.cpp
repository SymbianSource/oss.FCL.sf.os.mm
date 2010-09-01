// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ExtendedEncoderTest.h"
#include "ICLFrameworkTestSuite.h"
#include "../../testcodec/inc/ExtendedPlugin.h"

const TUid KExtendedImageType = {0x101F7BF7};
const TUid KExtendedImageSubType = {0x101F7BF8};
const TUid KExtendedEncoderUid = {0x101F7C05};

RCExtendedEncoderTest* RCExtendedEncoderTest::NewL(CTestSuite* aSuite, TBool aUseThread)
	{
	return new (ELeave) RCExtendedEncoderTest(aSuite, aUseThread);
	}

RCExtendedEncoderTest::RCExtendedEncoderTest(CTestSuite* aSuite, TBool aUseThread):
	iUseThread(aUseThread)
	{
	iSuite = aSuite;
	if (!iUseThread)
		iTestStepName = _L("MM-ICL-FRM-U-5601-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5603-HP");
	}

TVerdict RCExtendedEncoderTest::OpenL()
	{
	INFO_PRINTF1(_L("CImageEncoder Custom Command Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(iFileSession.Connect());

	iImageType		= KExtendedImageType;
	iImageSubType   = KExtendedImageSubType;
	iEncoderUid		= KExtendedEncoderUid;

	iDestinationData = NULL;
	iEncoder = NULL;

	return iTestStepResult;
	}

TVerdict RCExtendedEncoderTest::DoTestStepL()
	{
	CActiveListener* listener;
	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	CImageEncoder::TOptions options = CImageEncoder::EOptionNone;

	if (iUseThread)
		options = CImageEncoder::EOptionAlwaysThread;

	iEncoder = REINTERPRET_CAST(CExtendedEncoder*,CExtendedEncoder::DataNewL(iDestinationData, options, iImageType, iImageSubType, iEncoderUid));

	TInt result;

	//check that the thread usage is as expected
	result = iEncoder->UsingThread();
	if (iUseThread)
		Compare(!result, EFalse, _L("UsingThread() unexpected returned false"));
	else
		Compare(result, EFalse, _L("UsingThread() unexpected returned true"));


	//Should leave with KErrNotSupported
	TRAP(result, iEncoder->CustomSyncL(10));
	Compare(result, KErrNotSupported, _L("CustomSyncL() did not leave with KErrNotSupported"));

	//Should return with KErrNotSupported
	listener->InitialiseActiveListener();
	iEncoder->CustomAsync(&listener->iStatus, 10);
	CActiveScheduler::Start();
	result = listener->iStatus.Int(); 
	Compare(result, KErrNotSupported, _L("CustomAsync() did not return KErrNotSupported"));


	// Should return normally
	// and call HandleCustomSyncL() that will set SyncResult() to -2
	TRAP(result, iEncoder->CustomSyncL(0));
	Compare(result, KErrNone, _L("CustomSyncL() did not return with KErrNone"));
	TInt syncResult = iEncoder->SyncResult();
	Compare(syncResult, -2, _L("HandleCustomSyncL() not called"));

	// Should return normally
	// and call InitCustomAsyncL() that will set AsyncResult() to -1
	// and call DoConvert() that will set AsyncResult() to -2
	listener->InitialiseActiveListener();
	iEncoder->CustomAsync(&listener->iStatus, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int(); 
	Compare(result, KErrNone, _L("CustomAsync() did not return KErrNone"));
	TInt asyncResult = iEncoder->AsyncResult();
	Compare(asyncResult, -2, _L("DoConvert() not called"));
	TBool doCustomAsync = iEncoder->DoCustomAsync();
	Compare(doCustomAsync, EFalse, _L("NotifyComplete() not called"));

	CleanupStack::PopAndDestroy(listener);

	return iTestStepResult;
	}

void RCExtendedEncoderTest::Close()
	{
	delete iDestinationData; iDestinationData = NULL;
	delete iEncoder; iEncoder = NULL;

	iFileSession.Close();
	delete iScheduler; iScheduler = NULL;
	}
