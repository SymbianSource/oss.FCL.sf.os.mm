// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TMDATEST_H__
#define __TMDATEST_H__

#ifndef __MDA_CLIENT_RESOURCE_H__
#include <mda/client/resource.h>
#endif 
#ifndef __MDA_CLIENT_VIDEO_H__
#include <mda/client/video.h>
#endif
#ifndef __MDAIMAGECONVERTER_H__
#include <mdaimageconverter.h>
#endif

class CStressTestMonitor : public CBase
	{
public:
	void IncActiveCount();
	void DecActiveCount();
	void SetError(TInt aError);
	TInt Error();
private:
	TInt iActiveCount;
	TInt iError;
	};


class CStressTestActive : public CActive
	{
public:
	inline CStressTestActive(CStressTestMonitor& aMonitor) : CActive(EPriorityStandard), iMonitor(&aMonitor) { aMonitor.IncActiveCount(); }
	~CStressTestActive();
protected:
	// From CActive
	TInt RunError(TInt aError);
	void SetPending();

protected:
	CStressTestMonitor* iMonitor;
	};


class CImageUtil : public CStressTestActive, public MMdaImageUtilObserver
	{
public:
	CImageUtil(CStressTestMonitor& aMonitor);
private:
	virtual void MiuoCreateComplete(TInt aError);
	virtual void MiuoOpenComplete(TInt aError);
	virtual void MiuoConvertComplete(TInt aError);
	virtual void Complete(TInt aError);
	};


class CImageLoadUtil : public CImageUtil
	{
public:
	static CImageLoadUtil* NewL(CStressTestMonitor& aMonitor,const TDesC& aFileName,const TDisplayMode aDisplayMode);
	~CImageLoadUtil();
private:
	inline CImageLoadUtil(CStressTestMonitor& aMonitor) : CImageUtil(aMonitor) {};
	void RunL();
	void DoCancel();
private:
	TBuf<256> iMessage;
	TDisplayMode iDisplayMode;
	CFbsBitmap* iBitmap;
	CMdaImageFileToBitmapUtility* iMdaUtil;
	TInt iState;
	};

class CMdaStressTestThread : public CBase
	{
public:
	RThread	iThread;
	TSglQueLink iLink;
	TInt iId;
	};


class TMdaStressParam
	{
public:
	TInt iId;
	TFileName iDefaultPath;
	};

class CMdaStressTest
	{
public:
	CMdaStressTest();
	virtual ~CMdaStressTest();
	enum TTestThreadId
		{
		ELoadImages1,
		ELoadImages2,
		ENumTestThreads
		};

	virtual void DoTestL(const TDesC& aDefaultPath);
private:
	void StartTestThreadL(TMdaStressParam& aParam);
	void DeleteTestThread(CMdaStressTestThread* thread);
	inline TUint Random(TUint16 aMaxValue);
private:
	TInt iThreadNameCount;
	TSglQue<CMdaStressTestThread> iThreadList;
	TSglQueIter<CMdaStressTestThread> iThreadIter;
	TUint iRandomSeed;
	
	TMdaStressParam iParam[ENumTestThreads];
	};

const TInt KRandomSeed = 69069;

inline TUint CMdaStressTest::Random(TUint16 aMaxValue) 
	{ 
	iRandomSeed *= KRandomSeed;
	return ((iRandomSeed>>16)*aMaxValue)>>16; 
	}

#endif // __TMDATEST_H__

