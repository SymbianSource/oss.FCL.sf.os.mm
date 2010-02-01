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
//

#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>

// Test system includes
#include "TestSuiteExifImageDisplay.h"

#include "TestStepExifImageDisplay.h"

const TInt KStopResponseTreshold	=1500000; 	// 1.5sec
const TInt KPauseDelayIncrement		=10000; 	// 10ms
const TInt KCancelDelayIncrement	=10000; 	// 10ms
const TInt KPauseDelayAfterPause	=500000; 	// 1/2sec
const TInt KPauseDelayAfterStop		=500000; 	// 1/2sec

CTestStepStopResponse::CTestStepStopResponse(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepWithCheckpoint(aTestParams)
	{
	}

/*static*/
TInt CTestStepStopResponse::TimerGate(TAny* aPtr)
	{
	CTestStepStopResponse* self=reinterpret_cast<CTestStepStopResponse*>(aPtr);
	self->DoTimerGate();
	return KErrNone;
	}

void CTestStepStopResponse::MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError)
	{
	iLastPluginStatus	= aStatus;
	iLastUpdateRect		= aUpdatedArea;
	iLastError			= aError;
	if (iStoppedByTimer)
		{
		INFO_PRINTF2(_L("Error: got callback after StopPlay() frame=%d"), ++iFrameNumber);
		iLastError = KErrGeneral;
		CActiveScheduler::Stop();
		return;
		}
	if (iLastError != KErrNone)
		{
		CActiveScheduler::Stop();
		return;
		}
	TBool continueDecode=ConsumeFrame(aBitmap, ETrue);
	if (continueDecode && (aStatus&CImageDisplayPlugin::EStatusNoMoreToDecode)==0)
		{
		iImageDisplay->Play();
		}
	else
		{
		iImageDisplay->StopPlay();
		CActiveScheduler::Stop();
		}
	}

void CTestStepStopResponse::DoTimerGate()
	{
	if (iStoppedByTimer)
		{
		CActiveScheduler::Stop();
		}
	else
		{
		iStoppedByTimer=ETrue;
		iResponseGot.HomeTime();
		TInt64 Diff=iResponseGot.Int64() - iDecodeStarted.Int64();
		if (I64INT(Diff)-iDelayRequested > KStopResponseTreshold)
			{
			INFO_PRINTF1(_L("Too late..."));
			iLastError = KErrGeneral;
			}		
		INFO_PRINTF3(_L("Delay requested %d Got response after %d"),iDelayRequested, I64INT(Diff));
		iImageDisplay->StopPlay();
		iCancelTimer->After(KPauseDelayAfterStop, TCallBack(TimerGate,(TAny*)this));
		iStoppedByTimer=ETrue;
		}
	}

TVerdict CTestStepStopResponse::TestL()
	{

	iCancelTimer=CCallbackTimer::NewLC();
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	TInt currentDelay=0;
	SetupPluginL();
	iImageDisplay->SetupL();
	if (KErrNone != (iLastError=FuncTestL()))
		{
		INFO_PRINTF2(_L("---Func test failed with error %d"),iLastError);
		return (iLatestCheckpointVerdict=EFail);
		}	
	SetDefaultDestSize();
	for (;;)
		{
		iFrameNumber 	=0;
		ResetCheckPoints();
		iStoppedByTimer	=EFalse;
		iImageDisplay->Play();
		iCancelTimer->After(currentDelay, TCallBack(TimerGate,(TAny*)this));
		iDecodeStarted.HomeTime();
		iDelayRequested = currentDelay;
		CActiveScheduler::Start();

		if (!iStoppedByTimer)
			{
			iCancelTimer->Cancel();
			}
		if ((iFrameNumber && iFrameNumber == iTestParams->iMaxFramesToDecode) || iLastError != KErrNone
			|| (iLastPluginStatus&CImageDisplayPlugin::EStatusNoMoreToDecode))
			{
			break;
			}
		currentDelay +=KCancelDelayIncrement;
		INFO_PRINTF2(_L("Next cancel delay is %d"), currentDelay);
		}
	CleanupStack::PopAndDestroy(iCancelTimer);
	iCancelTimer = NULL;
	if (iLastError != KErrNone)
		{
		INFO_PRINTF2(_L("Got error %d"), iLastError);
		return EFail;
		}
	
	if (iNextCheckPoint && iNextCheckPoint->IsValid() && ((iTestParams->iMaxFramesToDecode>0 &&
		 iNextCheckPoint->iAbsoluteFrameNumber <= iTestParams->iMaxFramesToDecode) || iTestParams->iMaxFramesToDecode==TImageDisplayUnitTestParams::KNoFrameLimit)
		)
		{
		INFO_PRINTF1(_L("-- ERROR: Not all the checpoints have been passed"));
		return EFail;
		}
	return EPass;	
	}


CCallbackTimer* CCallbackTimer::NewLC()
    {
	CCallbackTimer* self=new (ELeave) CCallbackTimer();
	CleanupStack::PushL(self);
	self->ConstructL();
    return self;
    }

/**
*
* ~CCallbackTimer
*
**/
CCallbackTimer::~CCallbackTimer()
    {
    Cancel();
    }


void CCallbackTimer::After(TTimeIntervalMicroSeconds32 aCancelDelay, TCallBack aCallback)
    {
	iCallback = aCallback;	
	CTimer::After(aCancelDelay);
    }

/**
*
* RunL
*
**/
void CCallbackTimer::RunL()
    {
    iCallback.CallBack();
    }


CCallbackTimer::CCallbackTimer()
   : CTimer(CActive::EPriorityUserInput)
	{
	CActiveScheduler::Add(this);
	}
   

CTestStepPauseResponse::CTestStepPauseResponse(const TImageDisplayUnitTestParams& aTestParams):
		CTestStepWithCheckpoint(aTestParams)
	{
	}


void CTestStepPauseResponse::Cleanup()
	{
	delete iTimer;
	CTestStepImgDisplayGeneric::Cleanup();
	}

/*static*/
TInt CTestStepPauseResponse::TimerGate(TAny* aPtr)
	{
	reinterpret_cast<CTestStepPauseResponse*>(aPtr)->DoTimerGate();
	return KErrNone;
	}

void CTestStepPauseResponse::DoTimerGate()
	{
	if (iWaitingBeforeCont)
		{
		iImageDisplay->Play();
		iWaitingBeforeCont=EFalse;
		iStoppedByTimer=ETrue;
		CActiveScheduler::Stop();
		}
	else
		{
		iResponseGot.HomeTime();
		TInt64 Diff=iResponseGot.Int64() - iDecodeStarted.Int64();
		if (I64INT(Diff)-iDelayRequested > KStopResponseTreshold)
			{
			INFO_PRINTF1(_L("Too late..."));
			iLastError = KErrGeneral;
			}		
		INFO_PRINTF3(_L("Delay requested %d Got response after %d"),iDelayRequested, I64INT(Diff));
		iImageDisplay->Pause();
		iTimer->After(KPauseDelayAfterPause, TCallBack(TimerGate,(TAny*)this));
		iWaitingBeforeCont=ETrue;
		}
	}

void CTestStepPauseResponse::MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError)
	{
	iLastPluginStatus	= aStatus;
	iLastUpdateRect		= aUpdatedArea;
	iLastError			= aError;
	if (iWaitingBeforeCont)
		{
		INFO_PRINTF2(_L("Error: got callback during PAUSE() frame=%d"), ++iFrameNumber);
		iLastError = KErrGeneral;
		CActiveScheduler::Stop();
		return;
		}
	if (iLastError != KErrNone)
		{
		CActiveScheduler::Stop();
		return;
		}
	TBool continueDecode=ConsumeFrame(aBitmap, ETrue);
	if (continueDecode && (aStatus&CImageDisplayPlugin::EStatusNoMoreToDecode)==0)
		{
		iImageDisplay->Play();
		}
	else
		{
		iImageDisplay->StopPlay();
		CActiveScheduler::Stop();
		}
	}

TVerdict CTestStepPauseResponse::TestL()
	{
	
	iTimer=CCallbackTimer::NewLC();
	CleanupStack::Pop();
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	iDelayRequested=0;
	SetupPluginL();
	iImageDisplay->SetupL();
	if (KErrNone != (iLastError=FuncTestL()))
		{
		INFO_PRINTF2(_L("---Func test failed with error %d"),iLastError);
		return (iLatestCheckpointVerdict=EFail);
		}	
	SetDefaultDestSize();
	iWaitingBeforeCont = EFalse;
	iFrameNumber = 0;
	for (;;)
		{
		iImageDisplay->Play();
		iStoppedByTimer=EFalse;
		iTimer->After(iDelayRequested, TCallBack(TimerGate,(TAny*)this));
		iDecodeStarted.HomeTime();
		CActiveScheduler::Start();

		if (!iStoppedByTimer)
			{
			iTimer->Cancel();
			}

		if ((iFrameNumber && iFrameNumber == iTestParams->iMaxFramesToDecode) || iLastError != KErrNone
			|| (iLastPluginStatus&CImageDisplayPlugin::EStatusNoMoreToDecode))
			{
			break;
			}
		iDelayRequested +=KPauseDelayIncrement;
		INFO_PRINTF2(_L("Next pause delay is %d"), iDelayRequested);
		}
	if (iLastError != KErrNone)
		{
		INFO_PRINTF2(_L("Got error %d"), iLastError);
		return EFail;
		}
	if (iNextCheckPoint && iNextCheckPoint->IsValid() && ((iTestParams->iMaxFramesToDecode>0 &&
		 iNextCheckPoint->iAbsoluteFrameNumber <= iTestParams->iMaxFramesToDecode) || iTestParams->iMaxFramesToDecode==TImageDisplayUnitTestParams::KNoFrameLimit)
		)
		{
		INFO_PRINTF1(_L("-- ERROR: Not all the checpoints have been passed"));
		return EFail;
		}
	return EPass;	
	}
