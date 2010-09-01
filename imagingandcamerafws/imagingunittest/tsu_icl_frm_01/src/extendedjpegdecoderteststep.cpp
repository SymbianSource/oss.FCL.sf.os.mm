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

#include "extendedjpegdecoderteststep.h"
#include "../../testcodec/inc/ExtendedPlugin.h"

_LIT(KTestJpegFile,"c:\\TSU_ICL_FRM_01\\decodertest.jpg");

RCExtendedJpegDecoderTest* RCExtendedJpegDecoderTest::NewL(CTestSuite* aSuite, TBool aUseThread)
	{
	return new (ELeave) RCExtendedJpegDecoderTest(aSuite, aUseThread);
	}

RCExtendedJpegDecoderTest::RCExtendedJpegDecoderTest(CTestSuite* aSuite, TBool aUseThread):
	iUseThread(aUseThread)
	{
	iSuite = aSuite;
	if (!iUseThread)
		{
		iTestStepName = _L("MM-ICL-FRM-U-5604-HP");	
		}
		
	else
		{
		iTestStepName = _L("MM-ICL-FRM-U-5605-HP");		
		}
	}

TVerdict RCExtendedJpegDecoderTest::OpenL()
	{
	INFO_PRINTF1(_L("CImageDecoder Custom Command Test"));
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(iFileSession.Connect());
	iDecoder = NULL;

	return iTestStepResult;
	}

TVerdict RCExtendedJpegDecoderTest::DoTestStepL()
	{

	CImageDecoder::TOptions options = CImageDecoder::EOptionAllowZeroFrameOpen;

	if (iUseThread)
		{
		options = CImageDecoder::EOptionAlwaysThread;	
		}	
 
	TFileName sourceFile(KTestJpegFile);
		
	iDecoder = REINTERPRET_CAST(CExtendedDecoder*,CExtendedDecoder::FileNewL(iFileSession, sourceFile,options));

	//check that the thread usage is as expected
	TInt result = iDecoder->UsingThread();
	if (iUseThread)
		{
		Compare(!result, EFalse, _L("UsingThread() unexpected returned false"));	
		}
		
	else
		{
		Compare(result, EFalse, _L("UsingThread() unexpected returned true"));	
		}
	
	//Should leave with KErrNotSupported
	const TInt rogueCommand=10;
	TRAP(result, iDecoder->CustomSyncL(rogueCommand));
	Compare(result, KErrNotSupported, _L("CustomSyncL() did not leave with KErrNotSupported"));

	result==KErrNotSupported ? iTestStepResult=EPass :iTestStepResult=EFail;

	return iTestStepResult;
	}

void RCExtendedJpegDecoderTest::Close()
	{
	delete iDecoder; 
	iDecoder = NULL;
	iFileSession.Close();
	delete iScheduler; 
	iScheduler = NULL;
	}

