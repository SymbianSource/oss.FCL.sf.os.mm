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

#ifndef __MDA_CLIENT_TEST_H__
#include <mda/client/test.h>
#endif
#ifndef __MDA_CLIENT_RESOURCE_H__
#include <mda/client/resource.h>
#endif
#ifndef __MDA_CLIENT_VIDEO_H__
#include <mda/client/video.h>
#endif
#ifndef __TMDACORE_H__
#include "tmdacore.h"
#endif
#ifndef __MDAIMAGECONVERTER_H__
#include <mdaimageconverter.h>
#endif

class CMdaStressTestThread : public CBase
	{
public:
	RThread	iThread;
	TSglQueLink iLink;
	TInt iId;
	};

class CMdaStressTest : public CMdaCoreTest
	{
public:
	CMdaStressTest();
	~CMdaStressTest();
	enum TTestThreadId
		{
		ELoadImages1,
		ELoadImages2,
		ENumTestThreads
		};
private:
	virtual void DoTestL();
	void StartTestThreadL(TTestThreadId aId);
	void DeleteTestThread(CMdaStressTestThread* thread);
	inline TUint Random(TUint16 aMaxValue) { iRandomSeed *= 69069; return ((iRandomSeed>>16)*aMaxValue)>>16; }
private:
	TInt iThreadNameCount;
	TSglQue<CMdaStressTestThread> iThreadList;
	TSglQueIter<CMdaStressTestThread> iThreadIter;
	TUint iRandomSeed;
	};

#endif

