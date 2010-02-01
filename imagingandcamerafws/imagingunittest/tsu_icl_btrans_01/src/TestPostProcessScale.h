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
// TestStepPostProcessScale.h
//

#if (!defined __TESTSTEP_POSTPROCESS_SCALE_H__)
#define __TESTSTEP_POSTPROCESS_SCALE_H__

#include "TestStepBitmapTransforms.h"

class  CActiveListener;
class TScaleBitmapUnitTestParams;

class CTestStepPostProcessScale : public CTestStepBitmapTransforms
	{
public:
	CTestStepPostProcessScale( TUint index );
	~CTestStepPostProcessScale();
	virtual TVerdict DoTestStepL();
private:
	const TScaleBitmapUnitTestParams *iTestParameters;
	};

class CTestStepCancelScale : public CTestStepBitmapTransforms
	{
public:
	CTestStepCancelScale();
	~CTestStepCancelScale();
	virtual TVerdict DoTestStepL();
	};

//
//
// Decode Cancel Tests...
//
//

//
// CDecoderCancelTimer - on RunL() cancel the a decoder and stop AS
//

class CCancelTimer : public CTimer
	{
public:
	static CCancelTimer* NewLC(TTimeIntervalMicroSeconds32& aCancelDelay, CActive* aListener);
	~CCancelTimer();

	void InitializeCancelTimer();

private:
    CCancelTimer(CActive* aListener);
    void ConstructL(TTimeIntervalMicroSeconds32& aCancelDelay);

	// from CActive
    void RunL();
	void DoCancel();

private:
    TTimeIntervalMicroSeconds32 iCancelDelay;
    CActive*const iListener; // not owned
    };

// variant which properly cancels the decoder, where defined
class CActiveScaleListener : public CActiveListener
	{
public:
	void InitialiseActiveListener( CBitmapScaler* aScaler) 
		{ 
		iScaler = aScaler;
		CActiveListener::InitialiseActiveListener(); 
		}
	// From CActive
	virtual void DoCancel() 
		{ 
		if (iScaler) 
			iScaler->Cancel(); 
		}
protected:
	 CBitmapScaler* iScaler; // not owned
	};


#endif // __TESTSTEP_POSTPROCESS_SCALE_H__
