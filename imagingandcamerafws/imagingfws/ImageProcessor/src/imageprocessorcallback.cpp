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

#include "imageprocessorcallback.h"
#include <imageprocessor/imageprocessor.h>
#include <imageprocessor/imageprocessorobserver.h>

using namespace ImageProcessor;
	
//Free slots allocated to ensure there is enough memory for future callbacks
const TInt KCallbackReserveSlots = 20;

CImageProcessorCallback* CImageProcessorCallback::NewL(CImgProcessor& aImageProcessor,MImgProcessorObserver& aObserver)
	{
	CImageProcessorCallback* self = new (ELeave) CImageProcessorCallback(aImageProcessor,aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageProcessorCallback::CImageProcessorCallback(CImgProcessor& aImageProcessor,MImgProcessorObserver& aObserver):
   CActive(EPriorityNormal),iProcessor(aImageProcessor),iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CImageProcessorCallback::ConstructL()
	{
	iCallbackInfo.ReserveL(KCallbackReserveSlots);
	}

CImageProcessorCallback::~CImageProcessorCallback()
	{
	__ASSERT_ALWAYS(!IsActive(),Cancel());
	iCallbackInfo.Close();
	}

void CImageProcessorCallback::AddCallback(TInt aEventId, TUid aUid, TInt aId, TInt aError)
	{
	TInt err = iCallbackInfo.Append(TCallbackInfo(aEventId, aUid, aId, aError));
	if (err == KErrNoMemory) 
		{
		iCallbackInfo.Reset();
		// try to reserve few slots
		iCallbackInfo.Reserve(KCallbackReserveSlots);
		iCallbackInfo.Append(TCallbackInfo(aEventId, aUid, aId, KErrNoMemory));
		iProcessor.Cancel();
		}
	SelfComplete();
	}

void CImageProcessorCallback::SelfComplete()
	{
	//Only activate if a request is not already outstanding,
	//else it will be activated from RunL()
	if(!IsActive())
		{
		TRequestStatus* request = &iStatus;
		User::RequestComplete(request,KErrNone);
		SetActive();
		}
	}
   
void CImageProcessorCallback::RunL()
	{
	if(iCallbackInfo.Count())
		{
		//Any properies that need to be modified has to be done before the
		//callbacks. The client may delete the Image Processor in the callback
		const TCallbackInfo info(iCallbackInfo[0]);
		iCallbackInfo.Remove(0);
		EnsureSpace();

		//More requests in the queue
		__ASSERT_ALWAYS(!iCallbackInfo.Count(),SelfComplete());

		switch(info.iEventId)
			{
			case CImgProcessor::EEventInitializingComplete:
		   		iObserver.ImageProcessorInitializingComplete(iProcessor, info.iError);
		   		break;

			case CImgProcessor::EEventPreviewInitializingComplete:
				iObserver.ImageProcessorPreviewInitializingComplete(iProcessor, info.iId, info.iError);
		   		break;
		   
			case CImgProcessor::EEventProcessingComplete:
		   		iObserver.ImageProcessingComplete(iProcessor, info.iError);
		   		break;
		   	
			case CImgProcessor::EEventPreviewRenderingComplete:
		   		iObserver.ImageProcessorPreviewRenderingComplete(iProcessor, info.iId, info.iError);
		   		break;
		   	
			default:
		   		iObserver.ImageProcessorEvent(iProcessor, info.iEventId, info.iUid, info.iId);
			}
		}
	}
   
void CImageProcessorCallback::DoCancel()
	{
	}

void CImageProcessorCallback::EnsureSpace()
	{
	const TInt cnt(iCallbackInfo.Count());
   //Ignore error. Just a best effort attempt to ensure enough memory in future
   //Still have some space from granularity	
	iCallbackInfo.Reserve(cnt+KCallbackReserveSlots);
	}

CImageProcessorCallback::TCallbackInfo::TCallbackInfo(TInt aEventId, TUid aUid, TInt aId, TInt aError):
	iEventId(aEventId),
	iUid(aUid),
	iId(aId),
	iError(aError)
	{
	}
//EOF
