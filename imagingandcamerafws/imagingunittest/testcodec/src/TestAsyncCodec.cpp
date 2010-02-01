// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestAsyncCodec.h"


// CTestAsyncReadCodec
CTestAsyncReadCodec::CTestAsyncReadCodec(CTestAsyncDecoder& aDecoder):
	iDecoder(aDecoder)
	{
	}

void CTestAsyncReadCodec::ConstructL ()
	{ 
	CImageReadCodec::ConstructL(); 

	TTimeIntervalMicroSeconds32 KQuarterSecond = 250000;
	iTimer = CUtilityTimer::NewL(KQuarterSecond, *this);
	}

CTestAsyncReadCodec* CTestAsyncReadCodec::NewL(CTestAsyncDecoder &aDecoder)
	{
	CTestAsyncReadCodec* self = new (ELeave) CTestAsyncReadCodec(aDecoder);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestAsyncReadCodec::~CTestAsyncReadCodec()
	{
	delete iTimer;
	}

TFrameState CTestAsyncReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	// pause for a quarter of a second to look like we're doing somthing
	iTimer->InitializeTimer();

	return EFrameComplete;
	}

TFrameState CTestAsyncReadCodec::ProcessFrameHeaderL(TBufPtr8& /*aData*/)
	{
	return EFrameComplete;
	}

void CTestAsyncReadCodec::Complete()
	{
	iTimer->Cancel();
	}

void CTestAsyncReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool /*aDisableErrorDiffusion*/, CFbsBitmap& /*aDestination*/, CFbsBitmap* /*aDestinationMask*/)
	{
	}

void CTestAsyncReadCodec::TimerExpired()
	{
	iDecoder.ProcessFrameComplete(KErrNone);
	}


// CTestAsyncWriteCodec
CTestAsyncWriteCodec::CTestAsyncWriteCodec(CTestAsyncEncoder& aEncoder):
	iEncoder(aEncoder)
	{
	}

void CTestAsyncWriteCodec::ConstructL ()
	{ 
	CImageWriteCodec::ConstructL(); 

	TTimeIntervalMicroSeconds32 KQuarterSecond = 250000;
	iTimer = CUtilityTimer::NewL(KQuarterSecond, *this);
	}

CTestAsyncWriteCodec* CTestAsyncWriteCodec::NewL(CTestAsyncEncoder &aEncoder)
	{
	CTestAsyncWriteCodec* self = new (ELeave) CTestAsyncWriteCodec(aEncoder);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestAsyncWriteCodec::~CTestAsyncWriteCodec()
	{
	delete iTimer;
	}

void CTestAsyncWriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& /*aSource*/)
	{
	iDst = &aDst;
	}

TFrameState CTestAsyncWriteCodec::ProcessFrameL(TBufPtr8& /*aDst*/)
	{
	// pause for a quarter of a second to look like we're doing somthing
	iTimer->InitializeTimer();

	return EFrameComplete;
	}



void CTestAsyncWriteCodec::Complete()
	{
	iTimer->Cancel();
	}


void CTestAsyncWriteCodec::TimerExpired()
	{
	iEncoder.ProcessFrameComplete(KErrNone);
	}


// CUtilityTimer
CUtilityTimer* CUtilityTimer::NewL(TTimeIntervalMicroSeconds32& aDelay, MTimerObserver& aObserver)
    {
    CUtilityTimer* self = new (ELeave) CUtilityTimer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL(aDelay);
    CleanupStack::Pop(self);
    return self;
    }

void CUtilityTimer::ConstructL(TTimeIntervalMicroSeconds32& aDelay)
    {
    CTimer::ConstructL();

    iDelay = aDelay;
    CActiveScheduler::Add(this);
    }

CUtilityTimer::~CUtilityTimer()
    {
    Cancel();
    }

void CUtilityTimer::InitializeTimer()
    {
	// Request another wait - assume not active
	CTimer::After(iDelay);
    }

void CUtilityTimer::RunL()
    {
	if (iStatus.Int() == KErrNone)
		iObserver.TimerExpired();
    }

void CUtilityTimer::DoCancel()
	{
	}


CUtilityTimer::CUtilityTimer(MTimerObserver& aObserver) :
	CTimer(CActive::EPriorityUserInput),
    iObserver(aObserver)
	{
    }

