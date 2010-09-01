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

#include "ExtendedDecoderTest.h"
#include "../../testcodec/inc/ExtendedPlugin.h"

const TUid KExtendedImageType = {0x101F7BF7};
const TUid KExtendedImageSubType = {0x101F7BF8};
const TUid KExtendedDecoderUid = {0x101F7C04};

RCExtendedDecoderTest* RCExtendedDecoderTest::NewL(CTestSuite* aSuite, TBool aUseThread)
	{
	return new (ELeave) RCExtendedDecoderTest(aSuite, aUseThread);
	}

RCExtendedDecoderTest::RCExtendedDecoderTest(CTestSuite* aSuite, TBool aUseThread):
	iUseThread(aUseThread)
	{
	iSuite = aSuite;
	if (!iUseThread)
		iTestStepName = _L("MM-ICL-FRM-U-5600-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5602-HP");
	}

TVerdict RCExtendedDecoderTest::OpenL()
	{
	INFO_PRINTF1(_L("CImageDecoder Custom Command Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(iFileSession.Connect());

	iImageType		= KExtendedImageType;
	iImageSubType   = KExtendedImageSubType;
	iDecoderUid		= KExtendedDecoderUid;

	iDecoder = NULL;

	return iTestStepResult;
	}

TVerdict RCExtendedDecoderTest::DoTestStepL()
	{
	CActiveListener* listener;
	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	CImageDecoder::TOptions options = CImageDecoder::EOptionAllowZeroFrameOpen;

	if (iUseThread)
		options = CImageDecoder::EOptionAlwaysThread;

	iDecoder = REINTERPRET_CAST(CExtendedDecoder*,CExtendedDecoder::DataNewL(iFileSession, iSourceData, options, iImageType, iImageSubType, iDecoderUid));

	TInt result;

	//check that the thread usage is as expected
	result = iDecoder->UsingThread();
	if (iUseThread)
		Compare(!result, EFalse, _L("UsingThread() unexpected returned false"));
	else
		Compare(result, EFalse, _L("UsingThread() unexpected returned true"));

	//Should leave with KErrNotSupported
	TRAP(result, iDecoder->CustomSyncL(10));
	Compare(result, KErrNotSupported, _L("CustomSyncL() did not leave with KErrNotSupported"));

	//Should return with KErrNotSupported
	listener->InitialiseActiveListener();
	iDecoder->CustomAsync(&listener->iStatus, 10);
	CActiveScheduler::Start();
	result = listener->iStatus.Int(); 
	Compare(result, KErrNotSupported, _L("CustomAsync() did not return KErrNotSupported"));


	// Should return normally
	// and call HandleCustomSyncL() that will set SyncResult() to -2
	TRAP(result, iDecoder->CustomSyncL(0));
	Compare(result, KErrNone, _L("CustomSyncL() did not return with KErrNone"));
	TInt syncResult = iDecoder->SyncResult();
	Compare(syncResult, -2, _L("HandleCustomSyncL() not called"));

	// Should return normally
	// and call InitCustomAsyncL() that will set AsyncResult() to -1
	// and call DoConvert() that will set AsyncResult() to -2
	listener->InitialiseActiveListener();
	iDecoder->CustomAsync(&listener->iStatus, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int(); 
	Compare(result, KErrNone, _L("CustomAsync() did not return KErrNone"));
	TInt asyncResult = iDecoder->AsyncResult();
	Compare(asyncResult, -2, _L("DoConvert() not called"));
	TBool doCustomAsync = iDecoder->DoCustomAsync();
	Compare(doCustomAsync, EFalse, _L("NotifyComplete() not called"));

	CleanupStack::PopAndDestroy(listener);

	return iTestStepResult;
	}

void RCExtendedDecoderTest::Close()
	{
	delete iDecoder; iDecoder = NULL;

	iFileSession.Close();
	delete iScheduler; iScheduler = NULL;
	}
