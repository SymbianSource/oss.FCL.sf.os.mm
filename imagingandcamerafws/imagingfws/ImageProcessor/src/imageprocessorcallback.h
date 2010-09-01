// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __IMAGE_PROCESSOR_CALLBACK_H__
#define __IMAGE_PROCESSOR_CALLBACK_H__

#include <e32base.h>

namespace ImageProcessor 
	{

class MImgProcessorObserver;
class CImgProcessor;

/* AO for callback makes concurrency issues easier when client makes calls
from callback e.g. deletes image processor from within a callback.
Callbacks must be able to succeed (or at least a best effort to ensure
that they will succeed. e.g. client may wait indefinately if ImageProcessingComplete
callback cannot be made because of OOM.
Some callbacks are not in direct response to client request.
e.g. Progress indications - Can possibly use a pre-allocated queue.
The AO must be able to handle multiple outstanding callbacks.
At the moment simultaneous Processing & Rendering is not supported (CAPS restriction)
A work around is possible but then the callback AO should also be able to handle it.
The callback AO must be running at a higher priority than the worker AO, to ensure
callbacks are not delayed (e.g. receiving all progress callbacks only after processing had completed)*/

NONSHARABLE_CLASS(CImageProcessorCallback) : public CActive
   {
public:
   static CImageProcessorCallback* NewL(CImgProcessor& aImageProcessor,MImgProcessorObserver& aObserver);
   ~CImageProcessorCallback();

   void AddCallback(TInt aEventId, TUid aUid, TInt aId, TInt aError);

private:
	class TCallbackInfo
		{
	public:	
		TCallbackInfo(TInt aEventId, TUid aUid, TInt aId, TInt aError);
		
	public:
		TInt iEventId;
		TUid iUid;
		TInt iId;
		TInt iError;
		};

	CImageProcessorCallback(CImgProcessor& aImageProcessor, MImgProcessorObserver& aObserver);
	void ConstructL();
	void SelfComplete();
	
	//from CActive
	void RunL();
	void DoCancel();

	void EnsureSpace();
	
private:
	CImgProcessor& iProcessor;
	MImgProcessorObserver& iObserver;
	RArray<TCallbackInfo> iCallbackInfo;
	};

	}

#endif //__IMAGE_PROCESSOR_CALLBACK_H__
