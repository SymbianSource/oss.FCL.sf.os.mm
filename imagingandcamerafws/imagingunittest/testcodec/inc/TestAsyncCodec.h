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

#ifndef __TESTASYNCCODEC_H__
#define __TESTASYNCCODEC_H__

//#include <bitdev.h>
#include <icl/imagecodec.h>

#include "TestAsyncConvert.h"

class CTestAsyncDecoder;

// MTimerObserver
// mixin class to handle callbacks from a CUtilityTimer object
class MTimerObserver
	{
public:
	virtual void TimerExpired() = 0;
	};


// CUtilityTimer
class CUtilityTimer : public CTimer
	{
public:
	static CUtilityTimer* NewL(TTimeIntervalMicroSeconds32& aDelay, MTimerObserver& aObserver);
	~CUtilityTimer();

	void InitializeTimer();

private:
    CUtilityTimer(MTimerObserver& aObserver);
    void ConstructL(TTimeIntervalMicroSeconds32& aDelay);

	// from CActive
    void RunL();
	void DoCancel();

private:
    TTimeIntervalMicroSeconds32 iDelay;
	MTimerObserver& iObserver;
    };


// Test Read codec.
class CTestAsyncReadCodec : public CImageReadCodec, public MTimerObserver
	{
public:
	virtual ~CTestAsyncReadCodec();
	static CTestAsyncReadCodec* NewL(CTestAsyncDecoder &aDecoder);
	
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	virtual void Complete();

	// from MTimerObserver
	virtual void TimerExpired();

private:
	CTestAsyncReadCodec(CTestAsyncDecoder &aDecoder);
	void ConstructL();

private:
	CTestAsyncDecoder& iDecoder;
	CUtilityTimer* iTimer;
	};

// Test Write codec.
class CTestAsyncWriteCodec : public CImageWriteCodec, public MTimerObserver
	{
public:
	virtual ~CTestAsyncWriteCodec();
	static CTestAsyncWriteCodec* NewL(CTestAsyncEncoder &aEncoder);
	
	// From CImageWriteCodec
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);

	
//	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	//virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	//virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	
	void Complete();

	// from MTimerObserver
	virtual void TimerExpired();

private:
	CTestAsyncWriteCodec(CTestAsyncEncoder &aEncoder);
	void ConstructL();

private:
	CTestAsyncEncoder& iEncoder;
	CUtilityTimer* iTimer;
	TBufPtr8* iDst;	// not owned
	};

#endif

