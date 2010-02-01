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
#include "TestSuiteMngImageDisplay.h"

#include "TestStepMngImageDisplay.h"

#ifdef SYMBIAN_WRITABLE_DATA_PAGING
const TInt KStopResponseTreshold=800000;
#else
const TInt KStopResponseTreshold=200000;
#endif

CTestStepStopResponse::CTestStepStopResponse(const TImageDisplayUnitTestParams& aTestParams)
																			:CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*static*/
TInt CTestStepStopResponse::TimerGate(TAny* aPtr)
	{
	CTestStepStopResponse* self=reinterpret_cast<CTestStepStopResponse*>(aPtr);
	self->DoTimerGate();
	return KErrNone;
	}

void CTestStepStopResponse::DoTimerGate()
	{
	iStoppedByTimer=ETrue;
	iResponseGot.HomeTime();
	TInt64 Diff=iResponseGot.Int64() - iDecodeStarted.Int64();
	
	// DEF113719. Comments out the INFO_PRINTF to avoid the redundant information into log file
	//INFO_PRINTF3(_L("Delay requested %d Got response after %d"),iDelayRequested, I64INT(Diff));
	if (I64INT(Diff) > KStopResponseTreshold)
		{
		iLastError = KErrGeneral;
		}
	iImageDisplay->StopPlay();
	CActiveScheduler::Stop();
	}

TVerdict CTestStepStopResponse::TestL()
	{
	const TInt KCancelDelayIncrement=1000; // 1ms
	CCallbackTimer* CancelTimer=CCallbackTimer::NewLC();
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	TInt currentDelay=0;
	SetupPluginL();
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	for (;;)
		{
		iFrameNumber = 0;
		iImageDisplay->Play();
		iStoppedByTimer=EFalse;
		CancelTimer->After(currentDelay, TCallBack(TimerGate,(TAny*)this));
		iDecodeStarted.HomeTime();
		iDelayRequested = currentDelay;
		CActiveScheduler::Start();

		if (!iStoppedByTimer)
			{
			CancelTimer->Cancel();
			}
		if (iFrameNumber == iTestParams->iMaxFramesToDecode)
			{
			break;
			}
		currentDelay +=KCancelDelayIncrement;
		// DEF113719. Comments out the INFO_PRINTF to avoid the redundant information into log file
		//INFO_PRINTF2(_L("Next cancel delay is %d"), currentDelay);
		}
	CleanupStack::PopAndDestroy(CancelTimer);
	if (iLastError != KErrNone)
		{
		INFO_PRINTF2(_L("Got error %d"), iLastError);
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
	CTimer::HighRes(aCancelDelay);
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
   

const TInt KPauseDelayIncrement=1000; // 1ms
const TInt KPauseDelayAfterPause=500000; // 1/2sec

CTestStepPauseResponse::CTestStepPauseResponse(const TImageDisplayUnitTestParams& aTestParams):
		CTestStepImgDisplayGeneric(aTestParams)
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
		// DEF113719. Comments out the INFO_PRINTF to avoid the redundant information into log file
		//INFO_PRINTF3(_L("Delay requested %d Got response after %d"),iDelayRequested, I64INT(Diff));
		iImageDisplay->Pause();
		iTimer->After(KPauseDelayAfterPause, TCallBack(TimerGate,(TAny*)this));
		iWaitingBeforeCont=ETrue;
		}
	}

void CTestStepPauseResponse::MiidoImageReady(const CFbsBitmap* /*aBitmap*/, TUint /*aStatus*/, const TRect& /*aUpdatedArea*/, TInt aError)
	{
	++iFrameNumber;
	iLastError = aError;
	if (iWaitingBeforeCont)
		{
		INFO_PRINTF2(_L("Error: got callback during PAUSE() frame=%d"), iFrameNumber);
		iLastError = KErrGeneral;
		CActiveScheduler::Stop();
		return;
		}
	if (iFrameNumber == iTestParams->iMaxFramesToDecode || iLastError != KErrNone)
		{
		CActiveScheduler::Stop();
		}
	else
		{
		iImageDisplay->Play();
		}
	}

TBool CTestStepPauseResponse::ConsumeFrameL(const CFbsBitmap* /*aBitmap*/)
	{
	return ETrue;
	}

TVerdict CTestStepPauseResponse::TestL()
	{
	
	iTimer=CCallbackTimer::NewLC();
	CleanupStack::Pop();
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	iDelayRequested=0;
	SetupPluginL();
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iWaitingBeforeCont = EFalse;
	for (;;)
		{
		iFrameNumber = 0;
		iImageDisplay->Play();
		iStoppedByTimer=EFalse;
		iTimer->After(iDelayRequested, TCallBack(TimerGate,(TAny*)this));
		iDecodeStarted.HomeTime();
		CActiveScheduler::Start();

		if (!iStoppedByTimer)
			{
			iTimer->Cancel();
			}
		if (iFrameNumber == iTestParams->iMaxFramesToDecode || iLastError != KErrNone)
			{
			break;
			}
		iDelayRequested +=KPauseDelayIncrement;
		// DEF113719. Comments out the INFO_PRINTF to avoid the redundant information into log file
		//INFO_PRINTF2(_L("Next pause delay is %d"), iDelayRequested);
		}
	if (iLastError != KErrNone)
		{
		INFO_PRINTF2(_L("Got error %d"), iLastError);
		return EFail;
		}
	return EPass;	
	}
