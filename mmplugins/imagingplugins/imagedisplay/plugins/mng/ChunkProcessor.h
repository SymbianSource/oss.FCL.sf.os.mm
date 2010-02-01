// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CHUNKPROCESSOR_H__

#define __CHUNKPROCESSOR_H__
/** 	@file
	@internalTechnology */

class MInputStream;
class CMngPlayer;
union TChunkId;

class MChunkProcessor 
	{
public:
	virtual TBool OfferChunkType(const TChunkId& aChunkId)=0;
	virtual void Start(MInputStream& aInputStream, TRequestStatus& aRequest)=0;
	virtual void Stop()=0;
	virtual void Pause()=0;
	virtual void Resume()=0;

	virtual TInt AddSubProcessor(MChunkProcessor& aSubProcessor)=0;
	};


class CChunkProcessor: protected CActive, public MChunkProcessor 
	{
public:

// from MChunkProcessor
	void Stop();
	void Pause();
	void Resume();

protected:
	CChunkProcessor(CMngPlayer& aMngPlayer);

	void NotifyCaller(TInt aErrCode);

	virtual void DoRunL()=0;
	void DoCancel();
	TInt RunError(TInt aError);
	void RunL();
	
	void RunAgain();

protected:
	MInputStream*		iInputStream;
	TRequestStatus*		iCallerRequest;
	MChunkProcessor*	iSubProcessor;
	CMngPlayer&			iMngPlayer;	
	TBool				iIsPaused;
	TBool				iNeedRunAfterResume;
	};

#endif  // ndef __CHUNKPROCESSOR_H__
