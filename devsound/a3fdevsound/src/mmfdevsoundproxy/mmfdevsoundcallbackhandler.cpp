// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include "mmfdevsoundproxy.h"
#include "mmfdevsoundcallbackhandler.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CMsgQueueHandler::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMsgQueueHandler* CMsgQueueHandler::NewL(
						RMMFDevSoundProxy* aDevSoundProxy,
						MDevSoundObserver& aDevSoundObserver,
						RMsgQueue<TMMFDevSoundQueueItem>* aMsgQueue,
						MMMFDevSoundCustomInterfaceObserver& aDevSoundCIObserver)
	{
	CMsgQueueHandler* self = new(ELeave) CMsgQueueHandler(aDevSoundProxy,
														aDevSoundObserver,
														aMsgQueue,
														aDevSoundCIObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::CMsgQueueHandler
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CMsgQueueHandler::CMsgQueueHandler (RMMFDevSoundProxy* aDevSoundProxy,
									MDevSoundObserver& aDevSoundObserver,
									RMsgQueue<TMMFDevSoundQueueItem>* aMsgQueue,
									MMMFDevSoundCustomInterfaceObserver& aDevSoundCIObserver)
:	CActive(EPriorityStandard),
	iDevSoundProxy(aDevSoundProxy),
	iDevSoundObserver(aDevSoundObserver),
	iMsgQueue(aMsgQueue),
	iChunkDataPtr(0, 0, 0),
	iDevSoundCIObserver(aDevSoundCIObserver)
	{
	CActiveScheduler::Add(this);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::ConstructL()
	{
	iEmptyBuffer = CMMFDescriptorBuffer::NewL(0);
	iAsyncQueueFinish = new (ELeave) CAsyncCallBack(CActive::EPriorityStandard);
	TCallBack asyncCallback(AsyncQueueFinishCallback, this);
	iAsyncQueueFinish->Set(asyncCallback);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::~CMsgQueueHandler
// Destructor.
// ----------------------------------------------------------------------------
//
CMsgQueueHandler::~CMsgQueueHandler()
	{
	Cancel();
	if ( iMsgQueue )
		{
		iMsgQueue->Close();
		}
	iChunk.Close();
	
	delete iDataBuffer;
	delete iEmptyBuffer;
	delete iAsyncQueueFinish;
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::ReceiveEvents
// Subscribes for Play Error event from the DevSound server.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::ReceiveEvents()
	{
	if (!IsActive())
		{
		iMsgQueue->NotifyDataAvailable(iStatus);
		SetActive();
		}
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::RunL
// Handles active object’s request completion event.
//
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::RunL()
	{
	TInt err = iMsgQueue->Receive(iCurrentItem);

	if (err == KErrNone || err == KErrUnderflow)
		{
		// Signal that we're ready to process the next message
		ReceiveEvents();
		}

	if (err == KErrNone)
		{
		switch (iCurrentItem.iRequest)
			{
			case EMMFDevSoundProxyICEvent:
				{
				DoInitComplete();
				break;
				}
			case EMMFDevSoundProxyBTBFEvent:
				{
				iAsyncQueueFinish->Cancel(); // if still active, means previous cycle did not Finish(). Cancel.
				TRAP(err, DoBTBFCompleteL());
				if (err)
					{
					iAsyncQueueFinish->CallBack(); // async call to Finish()
					iDevSoundObserver.PlayError(err);
					}
				}
				break;
			case EMMFDevSoundProxyBTBEEvent:
				{
				iAsyncQueueFinish->Cancel(); // if still active, means previous cycle did not Finish(). Cancel.
				TRAP(err, DoBTBECompleteL());
				if (err)
					{
					iAsyncQueueFinish->CallBack(); // async call to Finish()
					iDevSoundObserver.RecordError(err);
					}
				}
				break;
			case EMMFDevSoundProxyPEEvent:
				{
				if (iCurrentItem.iErrorCode == KErrDied ||
					iCurrentItem.iErrorCode == KErrNotReady)
					{ 
					DoPlayErrorComplete();
					// "this" pointer is no longer valid here as the associated
					// instance of the DevSound has been deleted along with this
					// CMsgQueueHandler object. So, we can only return here.
					return;
					}
				else
					{
					DoPlayErrorComplete();
					break;
					}
				}
			case EMMFDevSoundProxyREEvent:
				{
				DoRecordErrorComplete();
				break;
				}
			case EMMFDevSoundProxyTFEvent:
				{
				DoToneFinishedComplete();
				break;
				}
			case EMMFDevSoundProxySETCEvent:
				{
				DoSendEventToClientComplete();
				break;
				}
			case EMMFDevSoundCustomCommandCloseMuxDemuxPair:
				{
				TMMFEvent pckgevent = iCurrentItem.iEventPckg();
				TInt handle = pckgevent.iEventType.iUid;
				iDevSoundCIObserver.CloseCustomInterface(handle);
				break;
				}
			case EMMFDevSoundProxyPausedRecordCompleteEvent:
				{
				DoPausedRecordComplete();
				break;
				}
			default:
				{
				break;
				}
			}
		}
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::RunError
// Called by CActive object framework if RunL leaves.
// ----------------------------------------------------------------------------
//
TInt CMsgQueueHandler::RunError(TInt aError)
	{
	TMMFEvent event;
	event.iErrorCode = aError;
	iDevSoundObserver.SendEventToClient(event);
	return KErrNone;
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoCancel
// Called when client cancels the wait for a completion of an outstanding
// request.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoCancel()
	{
	iMsgQueue->CancelDataAvailable();
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoInitComplete
// Handles initialization completion event.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoInitComplete()
	{
	iDevSoundObserver.InitializeComplete(iCurrentItem.iErrorCode);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoPlayErrorComplete
// Handles play completion or cancel event.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoPlayErrorComplete()
	{
	iAsyncQueueFinish->CallBack(); // async call to Finish()
	iDevSoundObserver.PlayError(iCurrentItem.iErrorCode);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoBTBFCompleteL
// Handles CMMFDevSound object's data request event to supply CMMFDevSound
// with the buffer that it needs to play.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoBTBFCompleteL()
	{
	// Returns either chunk handle or NULL
	// any error is assumed to be due to a pending PlayError(), so the error here is ignored - the PlayError() call will ensure the client remains lively
	// the chunk has been closed by the server. No action should be taken.
	TBool requestChunk = iDataBuffer==NULL; // if we have no buffer, tell server we need a chunk handle
	TInt handle = iDevSoundProxy->BufferToBeFilledData(requestChunk, iSetPckg);
	if(handle >= KErrNone)
		{
		if ( iSetPckg().iChunkOp == EOpen )
			{
			AssignDataBufferToChunkL(handle);
			}
		else
			{
			UpdateDataBufferL();
			}
		iDataBuffer->SetStatus(EAvailable);
		
		// Let the MMF fill the buffer with data
		
		iDevSoundObserver.BufferToBeFilled(iDataBuffer);
		}
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoBTBECompleteL
// Handles CMMFDevSound object's data request event to supply CMMFDevSound
// with the buffer that it needs to record.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoBTBECompleteL()
	{
	// Returns either chunk handle or NULL
	// any error is assumed to be due to a pending RecordError(), so the error here is ignored - the RecordError() call will ensure the client remains lively
	// the chunk has been closed by the server. No action should be taken.
	TInt handle = iDevSoundProxy->BufferToBeEmptiedData(iSetPckg);
	if(handle >= KErrNone)
		{
		if ( iSetPckg().iChunkOp == EOpen )
			{
			AssignDataBufferToChunkL(handle);
			}
		iDataBuffer->SetStatus(EFull);	
		iDataBuffer->Data().SetLength(iSetPckg().iRequestSize);
		iDevSoundObserver.BufferToBeEmptied(iDataBuffer);
		}
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoRecordErrorComplete
// Handles record completion or cancel event.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoRecordErrorComplete()
	{
	iAsyncQueueFinish->CallBack(); // async call to Finish()
	iDevSoundObserver.RecordError(iCurrentItem.iErrorCode);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoToneFinishedComplete
// Handles tone play completion event.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoToneFinishedComplete()
	{
	iDevSoundObserver.ToneFinished(iCurrentItem.iErrorCode);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoSendEventToClientComplete
// Sends DevSound server event completion notification to the client.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoSendEventToClientComplete()
	{
	iDevSoundObserver.SendEventToClient(iCurrentItem.iEventPckg());
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::DoPausedRecordComplete
// Handles CMMFDevSound object's data request event to supply CMMFDevSound
// with the last buffer that it needs to record.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::DoPausedRecordComplete()
	{
	ASSERT(iEmptyBuffer);
	iEmptyBuffer->SetLastBuffer(ETrue);
	iDevSoundObserver.BufferToBeEmptied(iEmptyBuffer);
	}

// ----------------------------------------------------------------------------
// CMsgQueueHandler::AssignDataBufferToChunkL
// Updates chunk handle.
// ----------------------------------------------------------------------------
//
void CMsgQueueHandler::AssignDataBufferToChunkL(TInt aHandle)
	{
	if ( iChunk.Handle() )
		{
		iChunk.Close();
		}
	User::LeaveIfError(iChunk.SetReturnedHandle(aHandle));
	// Adjust ptr to map only requested size
	// The existing clients should handle TPtr with length zero and max length
	// iSetPckg().iBufferSize.
	// When we make sure every client handles it, replace second parameter with
	// zero.
	//iChunkDataPtr.Set(iChunk.Base(), 0, iSetPckg().iBufferSize);
	iChunkDataPtr.Set(iChunk.Base(), iSetPckg().iBufferSize, iSetPckg().iBufferSize);
	if (!iDataBuffer)
		{
		iDataBuffer = CMMFPtrBuffer::NewL();	
		}
	UpdateDataBufferL(); 
	}
	
void CMsgQueueHandler::UpdateDataBufferL()
	{
	ASSERT(iDataBuffer); // to get here, we should have a data buffer
	iDataBuffer->SetPtr(iChunkDataPtr);
	iDataBuffer->SetRequestSizeL(iSetPckg().iRequestSize);
	iDataBuffer->SetLastBuffer(EFalse);	
	}
	
void CMsgQueueHandler::Finish()
	{
	if (iDataBuffer)
		{
		delete iDataBuffer;
		iDataBuffer = NULL;
		}
	if (iChunk.Handle())
		{
		iChunk.Close();
		}
	}

// 	AsyncQueueStartCallback


TInt CMsgQueueHandler::AsyncQueueFinishCallback(TAny* aPtr)
	{
	CMsgQueueHandler* self = static_cast<CMsgQueueHandler*>(aPtr);
	self->DoAsyncQueueFinishCallback();
	return KErrNone;
	}
	
void CMsgQueueHandler::DoAsyncQueueFinishCallback()
	{
	Finish();
	}


// End of File
