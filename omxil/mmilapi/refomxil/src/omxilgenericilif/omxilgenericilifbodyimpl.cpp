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
// omxilgenericifilbodyimpl.cpp
// 
//
 
#include "omxilgenericilifbodyimpl.h"
#include "omxilcoreutils.h" 
#include <e32base.h>
#include <e32msgqueue.h>
#include <mmf/server/mmfdatabuffer.h>


const TInt KMaxMsgQueueEntries = 10;

COmxBufferManager::COmxBufferManager(OMX_COMPONENTTYPE* aHandle)
	: iHandle(aHandle)
	{
	}
	
COmxBufferManager::COmxBuffer::COmxBuffer()
	{
	}
	
	
COmxBufferManager::COmxBuffer* COmxBufferManager::COmxBuffer::NewL(OMX_BUFFERHEADERTYPE* aBufferHeader, 
																   CMMFBuffer* aBuffer)
	{
	COmxBuffer* self = new (ELeave) COmxBuffer;
	CleanupStack::PushL(self);
	self->ConstructL(aBufferHeader, aBuffer);
	CleanupStack::Pop(self);
	return self;
	}
	
COmxBufferManager::COmxBuffer::~COmxBuffer()
	{
	if (iOwnsMmfBuffer)
		{
		delete iMmfBuffer;
		}
	}
	
CMMFBuffer* COmxBufferManager::COmxBuffer::MmfBuffer() const
	{
	return iMmfBuffer;
	}
	
OMX_BUFFERHEADERTYPE* COmxBufferManager::COmxBuffer::BufferHeader() const
	{
	return iBufferHeader;
	}

void COmxBufferManager::COmxBuffer::SetPortObserver(MOmxILComponentIfObserver* aObserver)
	{
	iPortObserver = aObserver;
	}

MOmxILComponentIfObserver* COmxBufferManager::COmxBuffer::PortObserver() const
	{
	return iPortObserver;
	}
		
COmxBufferManager::COmxBuffer* COmxBufferManager::FindBuffer(const CMMFBuffer* aBuffer) const
	{
	COmxBuffer* buffer = NULL;
	for (TInt i=0;i<iBuffers.Count() && !buffer;i++)
		{
		if (iBuffers[i]->MmfBuffer() == aBuffer)
			{
			buffer = iBuffers[i];
			}
		}
	return buffer;
	}
	
COmxBufferManager::COmxBuffer* COmxBufferManager::FindBuffer(OMX_BUFFERHEADERTYPE* aBuffer) const
	{
	return reinterpret_cast<COmxBuffer*>(aBuffer->pAppPrivate);
	}
	
 TInt COmxBufferManager::UseBuffer(CMMFBuffer& aBuffer, TUint aPortIndex)
	{
	if (CMMFBuffer::IsSupportedDataBuffer(aBuffer.Type()))
		{
		OMX_BUFFERHEADERTYPE* buffer;
		CMMFDataBuffer& dataBuffer = static_cast<CMMFDataBuffer&>(aBuffer);
				
        TDes8& aBufferDes = dataBuffer.Data();
        OMX_ERRORTYPE error = iHandle->UseBuffer(static_cast<OMX_HANDLETYPE>(iHandle), &buffer, aPortIndex, (void*)&aBuffer, aBufferDes.MaxLength(), const_cast<TUint8*>(aBufferDes.Ptr()));
    	if (error != OMX_ErrorNone)
    		{
    		return ConvertOmxErrorType(error);
    		}
		TRAPD(err, StoreBufferL(buffer, &aBuffer));
		return err;
		}
	else
		{
		return KErrNotSupported;
		}
	}


 CMMFBuffer* COmxBufferManager::AllocateBufferL(TUint aPortIndex, 
 												TUint aSizeBytes)
	{
	OMX_BUFFERHEADERTYPE* buffer;
	OMX_ERRORTYPE error = iHandle->AllocateBuffer(static_cast<OMX_HANDLETYPE>(iHandle), &buffer, aPortIndex, NULL, aSizeBytes);
	User::LeaveIfError(ConvertOmxErrorType(error));

	StoreBufferL(buffer,NULL); // transfers ownership

	return FindBuffer(buffer)->MmfBuffer();
	}
	

 TInt COmxBufferManager::FreeBuffer(CMMFBuffer* aBuffer)
	{
	COmxBuffer* buffer;
	for (TInt i=0;i<iBuffers.Count();i++)
		{
		buffer = iBuffers[i];
		if (buffer->MmfBuffer() == aBuffer)
			{
			iBuffers.Remove(i);
			OMX_ERRORTYPE err = iHandle->FreeBuffer(static_cast<OMX_HANDLETYPE>(iHandle), 0, buffer->BufferHeader());
			delete buffer;
			return err;
			}
		}
	return KErrNotFound;
	}
	
void COmxBufferManager::COmxBuffer::ConstructL(OMX_BUFFERHEADERTYPE* aBufferHeader, 
											   CMMFBuffer* aBuffer)
	{
	
	// Now if CMMFBuffer is NULL, this is been called from allocate buffer, and we need to 
	// Allocate a ptr buffer to correspond to the buffer created by OMX
	ASSERT(aBufferHeader);
	iBufferHeader = aBufferHeader;	
	if (aBuffer == NULL)
		{
		TPtr8 ptr(iBufferHeader->pBuffer, iBufferHeader->nFilledLen, iBufferHeader->nAllocLen);
		CMMFBuffer* mmfBuffer = CMMFPtrBuffer::NewL(ptr);
		iMmfBuffer = mmfBuffer;
		iOwnsMmfBuffer = ETrue;
		}
	else
		{
		iMmfBuffer = aBuffer;
		}
		
	// store pointer to element in array
	iBufferHeader->pAppPrivate = this;
	}

void COmxBufferManager::StoreBufferL(OMX_BUFFERHEADERTYPE* aBufferHeader, 
									 CMMFBuffer* aBuffer)
	{
	COmxBuffer* buf = COmxBuffer::NewL(aBufferHeader, aBuffer);
	CleanupStack::PushL(buf);
	iBuffers.AppendL(buf);
	CleanupStack::Pop(buf);
	}
	
 TInt COmxBufferManager::EmptyThisBuffer(const CMMFBuffer* aBuffer, 
 										 MOmxILComponentIfObserver* aObserver)
	{
	if (CMMFBuffer::IsSupportedDataBuffer(aBuffer->Type()))
		{
		COmxBuffer* omxBuffer = FindBuffer(aBuffer);
		if (!omxBuffer)
			{
			return KErrNotFound;
			}

		omxBuffer->SetPortObserver(aObserver);
		OMX_BUFFERHEADERTYPE* bufferHeader = omxBuffer->BufferHeader();
		const CMMFDataBuffer* buf = static_cast<const CMMFDataBuffer*>(aBuffer);
		const TDesC8& des = buf->Data();
		bufferHeader->nFilledLen = des.Length();
		bufferHeader->nFlags = 0;
		if (aBuffer->LastBuffer())
			{	
			bufferHeader->nFlags |= OMX_BUFFERFLAG_EOS;
			}
		else
			{
			bufferHeader->nFlags &= ~OMX_BUFFERFLAG_EOS;
			}
		return ConvertOmxErrorType(iHandle->EmptyThisBuffer(static_cast<OMX_HANDLETYPE>(iHandle), bufferHeader));
		}
	else
		{
		return KErrNotSupported;
		}
	}


 TInt COmxBufferManager::FillThisBuffer(CMMFBuffer* aBuffer, 
 										MOmxILComponentIfObserver* aObserver)
	{
	if (CMMFBuffer::IsSupportedDataBuffer(aBuffer->Type()))
		{
		COmxBuffer* omxBuffer = FindBuffer(aBuffer);
		if (!omxBuffer)
			{
			return KErrNotFound;
			}
		omxBuffer->SetPortObserver(aObserver);
		OMX_BUFFERHEADERTYPE* bufferHeader = omxBuffer->BufferHeader();
		
		bufferHeader->nFilledLen = 0;
		// clear last buffer flag
		bufferHeader->nFlags &= ~OMX_BUFFERFLAG_EOS;
		return ConvertOmxErrorType(iHandle->FillThisBuffer(static_cast<OMX_HANDLETYPE>(iHandle), bufferHeader));
		}
	else
		{
		return KErrNotSupported;
		}
	}

	
	
COmxBufferManager::~COmxBufferManager()
	{
	for (TInt i=0;i<iBuffers.Count();i++)
		{
		COmxBuffer* omxBuffer = iBuffers[i];
		iHandle->FreeBuffer(static_cast<OMX_HANDLETYPE>(iHandle), 0, omxBuffer->BufferHeader());
		delete omxBuffer;
		}
	iBuffers.Close();
	}


COmxCallbacks* COmxCallbacks::NewL(MOmxILComponentIfObserver& aObserver)
	{
	COmxCallbacks* self = new (ELeave) COmxCallbacks(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
	
void COmxCallbacks::ConstructL()
	{
	OMX_CALLBACKTYPE h =
			{
			&::EventHandler,
			&::EmptyBufferDone,
			&::FillBufferDone
			};
			
	iHandle = h;
	CActiveScheduler::Add(this);

	User::LeaveIfError(iMsgQueue.CreateLocal(KMaxMsgQueueEntries));
	iMsgQueue.NotifyDataAvailable(iStatus);
	SetActive();
	}

COmxCallbacks::COmxCallbacks(MOmxILComponentIfObserver& aObserver)
	: CActive(EPriorityStandard),
	iParent(aObserver)
	{
	}
		
	
COmxCallbacks::operator OMX_CALLBACKTYPE*()
	{
	return &iHandle;
	}
	
	
void COmxCallbacks::RunL()
	{
	TOmxMessage msg;
	
	while (iMsgQueue.Receive(msg)==KErrNone)
		{
		switch (msg.iType)
			{
			case EEmptyBufferCallback:
				{
				MOmxILComponentIfObserver* callback = msg.iBuffer->PortObserver();
				const CMMFBuffer* buffer = msg.iBuffer->MmfBuffer();
				OMX_BUFFERHEADERTYPE* bufferHeader = msg.iBuffer->BufferHeader();
				TInt portIndex = static_cast<TInt>(bufferHeader->nInputPortIndex);
				User::LeaveIfError(callback->EmptyBufferDone(buffer, portIndex));
				break;
				}				

			case EFillBufferCallback:
				{
				CMMFBuffer* mmfBuffer = msg.iBuffer->MmfBuffer();
				OMX_BUFFERHEADERTYPE* bufferHeader = msg.iBuffer->BufferHeader();

				if (CMMFBuffer::IsSupportedDataBuffer(mmfBuffer->Type()))
					{
					CMMFDataBuffer* dataBuffer = static_cast<CMMFDataBuffer*>(mmfBuffer);
					TDes8& aBufferDes = dataBuffer->Data();
					aBufferDes.SetLength(bufferHeader->nFilledLen);
					mmfBuffer->SetLastBuffer(bufferHeader->nFlags & OMX_BUFFERFLAG_EOS);
					}				
				else
					{
					ASSERT(EFalse);
					}
				MOmxILComponentIfObserver* callback = msg.iBuffer->PortObserver();
				bufferHeader = msg.iBuffer->BufferHeader();
				TInt portIndex = static_cast<TInt>(bufferHeader->nOutputPortIndex);
				User::LeaveIfError(callback->FillBufferDone(mmfBuffer, portIndex));
				break;
				}				
			case EEventCallback:
				{
				TInt err = KErrNone;
				err = iParent.EventHandler(msg.iEventParams.iEvent, 
								  		   msg.iEventParams.iData1, 
									  	   msg.iEventParams.iData2, 
									  	   msg.iEventParams.iExtraData);
				if (err != KErrNone)
					{
					// Very possible the event is not supported by the IL Client. 
					// Ignore for now
					// ASSERT(EFalse); // Uncomment this line if interested in catching this case on debug mode.
					}
				break;
				}				
			default:
				{
				// This is an invalid state
				ASSERT(EFalse);
				}					
			};
		}

	// setup for next callbacks		
	iStatus = KRequestPending;
	iMsgQueue.NotifyDataAvailable(iStatus);
	SetActive();	
	}
	
COmxCallbacks::~COmxCallbacks()
	{
	Cancel();	
	iMsgQueue.Close();
	}
	
	
void COmxCallbacks::DoCancel()
	{
	if (iMsgQueue.Handle()!=NULL)
		{
		iMsgQueue.CancelDataAvailable();
		}
	}

	
TInt COmxCallbacks::FillBufferDone(OMX_HANDLETYPE aComponent, 
								   COmxBufferManager::COmxBuffer* aBuffer)
	{
	TOmxMessage message;
	message.iType = EFillBufferCallback;
	message.iComponent = aComponent;
	message.iBuffer = aBuffer;
	return iMsgQueue.Send(message);
	}
	
TInt COmxCallbacks::EmptyBufferDone(OMX_HANDLETYPE aComponent, 
								    COmxBufferManager::COmxBuffer* aBuffer)
	{
	TOmxMessage message;
	message.iType = EEmptyBufferCallback;
	message.iComponent = aComponent;
	message.iBuffer = aBuffer;
	return iMsgQueue.Send(message);
	}
	
TInt COmxCallbacks::EventHandler(OMX_HANDLETYPE aComponent, 
								 const TEventParams& aEventParams)
	{
	TOmxMessage message;
	message.iType = EEventCallback;
	message.iComponent = aComponent;
	message.iEventParams = aEventParams;
	return iMsgQueue.Send(message);
	}



OMX_ERRORTYPE EventHandler(OMX_OUT OMX_HANDLETYPE aComponent, 
						   OMX_OUT TAny* aAppData,
        				   OMX_OUT OMX_EVENTTYPE aEvent, 
        				   OMX_OUT TUint32 aData1,
        				   OMX_OUT TUint32 aData2,
        				   OMX_OUT TAny* aExtra)
	{
	COmxCallbacks::TEventParams eventParams;
	eventParams.iEvent = aEvent;
	eventParams.iData1 = aData1;
	eventParams.iData2 = aData2;
	eventParams.iExtraData = aExtra;
	TInt error = static_cast<COmxCallbacks*>(aAppData)->EventHandler(aComponent, eventParams);
	return ConvertSymbianErrorType(error);
	}
        			
OMX_ERRORTYPE EmptyBufferDone(OMX_HANDLETYPE aComponent,
       						  TAny* aAppData,
       						  OMX_BUFFERHEADERTYPE* aBuffer)
	{
	COmxBufferManager::COmxBuffer* buffer = static_cast<COmxBufferManager::COmxBuffer*>(aBuffer->pAppPrivate);
	TInt error = static_cast<COmxCallbacks*>(aAppData)->EmptyBufferDone(aComponent, buffer);
	return ConvertSymbianErrorType(error);
	}
        
OMX_ERRORTYPE FillBufferDone(OMX_HANDLETYPE aComponent,
       						 TAny* aAppData,
       						 OMX_BUFFERHEADERTYPE* aBuffer)
	{
	COmxBufferManager::COmxBuffer* buffer = static_cast<COmxBufferManager::COmxBuffer*>(aBuffer->pAppPrivate);
	TInt error = static_cast<COmxCallbacks*>(aAppData)->FillBufferDone(aComponent, buffer);
	return ConvertSymbianErrorType(error);
	}
	
