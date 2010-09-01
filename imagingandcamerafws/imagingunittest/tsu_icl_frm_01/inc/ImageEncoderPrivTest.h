// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined __IMAGE_ENCODER_PRIV_TEST_H__)
#define __IMAGE_ENCODER_PRIV_TEST_H__  

#include "ICLFrameWorkTestStep.h"

class CImageEncoderPrivTestAccess : public CBase
	{
public:
	CImageEncoderPrivTestAccess(CImageEncoderPriv* aEncPriv);
	HBufC8& FinalDestinationBuffer();
	void SetImageEncoderPriv(CImageEncoderPriv* aEncPriv);
private:
	CImageEncoderPriv* iImageEncoderPriv;
	};

class RCImageEncoderPrivEncodeTest : public RAllocTest, public MImageEncoderPrivSupportImp
	{
public:
	static RCImageEncoderPrivEncodeTest* NewL(CTestSuite* aSuite, TBool aFileTest = ETrue, TBool aAllocTest = EFalse);

protected:
	CImageEncodeConstruct* CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid);
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCImageEncoderPrivEncodeTest(CTestSuite* aSuite, TBool aFileTest, TBool aAllocTest);

private:
	CImageEncoderPriv* iImageEncoderPriv;
	CImageEncodeConstruct* iConstruct;

	CFbsBitmap* iSourceBitmap;
	HBufC8*		iImageData;
	HBufC8*		iRefImageData;

	TBool iFileTest; //ETrue use SetFileL() / EFalse use SetDataL()

	RFs iFileSession;
	TFileName iDestinationFilename;
	CActiveScheduler* iScheduler;
	RThread iMyThread;
	};


class RCImageEncoderPrivCustomTest : public RAllocTest, public MImageEncoderPrivSupportImp
	{
public:
	static RCImageEncoderPrivCustomTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	CImageEncodeConstruct* CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid);
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCImageEncoderPrivCustomTest(CTestSuite* aSuite, TBool aAllocTest);

private:
	CImageEncoderPriv* iImageEncoderPriv;
	CImageEncodeConstruct* iConstruct;

	CFbsBitmap* iSourceBitmap;
	HBufC8*		iImageData;
	HBufC8*		iRefImageData;

	RFs iFileSession;
	TFileName iDestinationFilename;
	CActiveScheduler* iScheduler;
	RThread iMyThread;
	};


class RCImageEncoderPrivRAEncodeTest : public RAllocTest, public MImageEncoderPrivSupportImp
	{
public:
	static RCImageEncoderPrivRAEncodeTest* NewL(CTestSuite* aSuite);

protected:
	CImageEncodeConstruct* CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid);
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCImageEncoderPrivRAEncodeTest(CTestSuite* aSuite);

private:
	CImageEncoderPriv* iImageEncoderPriv;
	CImageEncodeConstruct* iConstruct;

	HBufC8*		iImageData;
	HBufC8*		iRefImageData;

	CActiveScheduler* iScheduler;
	};


#endif // __IMAGE_ENCODER_PRIV_TEST_H__
