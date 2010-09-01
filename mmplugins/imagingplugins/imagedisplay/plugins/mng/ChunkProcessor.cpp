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

/** 	@file
	@internalTechnology */
#include <e32base.h>
#include "InputStream.h"

#include "ChunkProcessor.h"

CChunkProcessor::CChunkProcessor(CMngPlayer& aMngPlayer):
								CActive(EPriorityIdle),
								iMngPlayer(aMngPlayer)
	{
	CActiveScheduler::Add(this);
	}

void CChunkProcessor::DoCancel()
	{
	if (iCallerRequest != NULL)
		{
		User::RequestComplete(iCallerRequest, KErrCancel);
		}
		
	if (iStatus.Int() == KRequestPending)
		{
		TRequestStatus* pReq=&iStatus;
		User::RequestComplete(pReq, KErrCancel);
		}
	}

TInt CChunkProcessor::RunError(TInt aError)
	{
	NotifyCaller(aError);
	return KErrNone;
	}

void CChunkProcessor::RunAgain()
	{
	if (iIsPaused)
		{
		ASSERT(EFalse==iNeedRunAfterResume);
		iNeedRunAfterResume = ETrue;
		}
	else
		{
		iNeedRunAfterResume = EFalse;
		SetActive();
		TRequestStatus* pReq=&iStatus;
		User::RequestComplete(pReq, KErrNone);
		}
	}

void CChunkProcessor::Pause()
	{
	iIsPaused = ETrue;
	if (iSubProcessor)
		{
		iSubProcessor->Pause();
		}
	}

void CChunkProcessor::Resume()
	{
	iIsPaused=EFalse;
	if (iSubProcessor)
		{
		iSubProcessor->Resume();
		}
		
	if (iNeedRunAfterResume)
		{
		RunAgain();
		}
	}

void CChunkProcessor::Stop()
	{
	if (iInputStream != NULL)
		{
		iInputStream->Release();
		}
	iInputStream = NULL;
	if (iSubProcessor != NULL)
		{
		iSubProcessor->Stop();
		}
	Cancel();
	iNeedRunAfterResume = EFalse;
	}

void CChunkProcessor::NotifyCaller(TInt aErrCode)
	{
	User::RequestComplete(iCallerRequest, aErrCode);
	if (iInputStream != NULL)
		{
		iInputStream->Release();
		}
	iInputStream = NULL;
	}

void CChunkProcessor::RunL()
	{
	if (iStatus != KErrNone)
		{
		User::Leave(iStatus.Int());
		}
	if (iIsPaused)
		{
		ASSERT(EFalse==iNeedRunAfterResume);
		iNeedRunAfterResume = ETrue;
		}
	else
		{
		DoRunL();
		}
	}
