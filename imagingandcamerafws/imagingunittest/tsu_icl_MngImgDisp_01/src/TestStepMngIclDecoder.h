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

#if !defined(TESTSTEPMNGIICLDECODER_H)
#define TESTSTEPMNGIICLDECODER_H


class CImageDecoder;
class CActiveDecodeListener;
class CTestTimer;

class CTestStepMngIclDecoder : public CTestStepMngImageDisplay
	{
public:
	CTestStepMngIclDecoder(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName);
	~CTestStepMngIclDecoder();

	TVerdict DoTestStepPreambleL();
	TVerdict DoTestStepPostambleL();
protected:
	void Cleanup();
	void SetupDecoderL(const TDesC& aFileName, TBool aOptions = EFalse);
	void CheckDecodedFrameL(TBool& aMoreToCheck);
	void ComposeFrame(const TFrameInfo& aFrameInfo);  //Defect:DEF073809
	
public:
	CFbsBitmap*			iCurrentFrame; //Defect:DEF073809
	CFbsBitmap*			iCurrentMask; //Defect:DEF073809
	TFrameInfo			iFrameInfo;
protected:
	const TFrameCheckPoint*	iNextCheckPoint;
	CImageDecoder*			iIclDecoder;
	CActiveDecodeListener*	iActListener;  
	CTestTimer*				iTestTimer;  //INC079424 - DEF080823
	CFbsBitmap*				iFrame;
	CFbsBitmap*				iMask;
	TInt					iFrameNumber;
	TTimeIntervalMicroSeconds	iPreviousDelay;
	};


class CTestStepMngIclCheckpoint : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngIclCheckpoint(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName, TBool aOptions = EFalse);

protected:
	
	TVerdict TestL();

protected:
	TBool   iOptions;
	};

class CTestStepMngIclCancelCheckpoint : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngIclCancelCheckpoint(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName, TBool aOptions = EFalse);

protected:
	
	TVerdict TestL();

protected:
	TBool   iOptions;
	};

class CTestStepMngMagnCheck : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngMagnCheck(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName,
											const TUint* aColorRef, const TUint* aAlphaRef);

protected:
	
	TVerdict TestL();

protected:
	const TUint* iColorRef;
	const TUint* iAlphaRef;
	};

/*
For DEF123773: ICL Mng decoder incorrectly handles MAGN chunks.

Test class that decodes a source file into a 32bit destination bitmap and compares
this bitmap to the data provided in a buffer (iColorRef).
*/
class CTestStepMngMagn32BitSingleDestinationCheck : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngMagn32BitSingleDestinationCheck(const TImageDisplayUnitTestParams& aTestParams,
												const TDesC& aTestName, TDisplayMode aDisplayMode,
												const TUint* aColorRef);
protected:
	TVerdict TestL();

protected:
	const TUint* iColorRef;
	TDisplayMode iDisplayMode;
	};

/*
For DEF123773: ICL Mng decoder incorrectly handles MAGN chunks.

Test class that decodes a source file into a 32bit destination bitmap plus mask and compares
these bitmaps to the data provided in buffers (iColorRef and iAlphaRef).
*/
class CTestStepMngMagn32BitDestinationAndMaskCheck : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngMagn32BitDestinationAndMaskCheck(const TImageDisplayUnitTestParams& aTestParams,
												const TDesC& aTestName, TDisplayMode aDisplayMode,
												const TUint* aColorRef, const TUint* aAlphaRef);
protected:
	TVerdict TestL();

protected:
	const TUint* iColorRef;
	const TUint* iAlphaRef;
	TDisplayMode iDisplayMode;
	};

class CTestStepMngIclNegative : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngIclNegative(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName);

protected:
	
	TVerdict TestL();

protected:
	};


class CCallbackTimer;
class CTestStepMngIclResponse : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngIclResponse(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName);

protected:
	
	TVerdict TestL();
	static TInt TimerGate(TAny* aPtr);
protected:
	CCallbackTimer*		iTimer; // not owned
	TInt				iCancelDelay;
	TTime				iCancelRequestedAt;
	};
	
class CTestStepMngIclMimeType : public CTestStepMngIclDecoder
	{
public:
	CTestStepMngIclMimeType(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName);

protected:
	TVerdict TestL();
	};	

#endif // ndef TESTSTEPMNGIICLDECODER_H

