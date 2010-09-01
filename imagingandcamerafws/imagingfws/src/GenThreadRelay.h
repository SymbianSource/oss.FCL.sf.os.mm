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

#ifndef __GEN_THREAD_RELAY_H__
#define __GEN_THREAD_RELAY_H__

#include "ImageConversion.h"

const TThreadPriority KSubThreadPriority = EPriorityLess;
const TInt KSubThreadStackSize = KDefaultStackSize;	// 0x2000 = 8k
const TInt KTerminationFunction = 0;

class CSubThreadRelay;

/**
 *
 * This class is not for public use
 *
 */
class TThreadRelayParam
	{
public:
	TThreadId iMainThreadId;
	CSubThreadRelay** iSubThreadRelay; //Not owned
	CSubThreadRelay* (*iFactoryFunctionL) (TThreadId aMainThreadId, TAny* aCodecParam); //Not owned
	TAny* iCodecParam; //Not owned
	TRequestStatus* iSetupComplete; //Not owned
	};

class CSubThreadUndertaker; // declared here
/**
 *
 * This class is not for public use
 * This AO monitor the sub thread for panics
 *
 */
NONSHARABLE_CLASS( CSubThreadUndertaker ): public CActive
	{
public:
	static CSubThreadUndertaker* NewL(RThread& aSubThread);
	virtual ~CSubThreadUndertaker();

protected:
	//From CActive
	virtual void RunL();
	virtual void DoCancel();

private:
	CSubThreadUndertaker(RThread* aSubThread);

private:
	RThread* iSubThread; //Not owned
	};

class CSubThreadRelay; // declared here
/**
 *
 * This class is not for public use
 * Implement plugin side thread interface for threaded De/Encode
 *
 */
NONSHARABLE_CLASS( CSubThreadRelay ): public CActive
	{
public:
	void Shutdown(TRequestStatus& aRequestStatus);
	virtual ~CSubThreadRelay();

	void Prime();
	void SignalSetupComplete(TRequestStatus* aSetupComplete);

	TRequestStatus* Status();

	void RunFunctionInThread(TRequestStatus& aCallingStatus, TInt aFunctionCode, TAny* aParameters);
	void SetSubThread(RThread* aSubThread);

protected:
	void ConstructL(TThreadId aMainThreadId);
	CSubThreadRelay();

	virtual void RunFunctionL(TInt aFunctionCode, TAny* aParameters) = 0;

	//From CActive
	virtual void RunL();
	virtual void DoCancel();

protected:
	RThread iMainThread;
	RThread* iSubThread; //Not owned

private:
	TRequestStatus* iCallingStatus;
	TInt iFunctionCode;
	TAny* iFunctionParams;
	};

class CThreadRelay; // declared here
/**
 *
 * This class is not for public use
 * Implement client side thread interface for threaded De/Encoding
 *
 */
NONSHARABLE_CLASS( CThreadRelay ): public CBase
	{
public:
	virtual ~CThreadRelay();

protected:
	CThreadRelay();
	void ConstructL(CSubThreadRelay* (*aFactoryFunctionL)(TThreadId aMainThreadId, TAny* aCodecParam), TAny* aCodecParam);

	TInt RunFunction(TInt aFunctionCode, TAny* aParameters);

	CSubThreadRelay* SubThreadRelay();

	const RThread* SubThread() const;
	void SetPriority(TThreadPriority aPriority) const;

private:
	static TInt ThreadFunction(TAny* aPtr);
	static void ThreadTrapFunctionL(TAny* aPtr);
	void Shutdown();

private:
	CSubThreadUndertaker* iSubThreadUndertaker;
	CSubThreadRelay* iSubRelay;
	RThread iSubThread;
	TRequestStatus iFunctionCallStatus;
	TBool iThreadCreated;
	};


#endif // __GEN_THREAD_RELAY_H__

