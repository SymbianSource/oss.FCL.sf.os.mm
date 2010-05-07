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

#ifndef OMXILGENERICILIFBODYIMPL_H
#define OMXILGENERICILIFBODYIMPL_H

#include "momxilcomponentifobserver.h"
#include <e32msgqueue.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>


class COmxBufferManager : public CBase
	{
public:

	class COmxBuffer : public CBase
		{
	public:
		static COmxBuffer* NewL(OMX_BUFFERHEADERTYPE* aBufferHeader, CMMFBuffer* aBuffer);
		~COmxBuffer();
		
		CMMFBuffer* MmfBuffer() const;
		OMX_BUFFERHEADERTYPE* BufferHeader() const;
		
		void SetPortObserver(MOmxILComponentIfObserver* aObserver);
		MOmxILComponentIfObserver* PortObserver() const;

	private:
		COmxBuffer();
		void ConstructL(OMX_BUFFERHEADERTYPE* aBufferHeader, CMMFBuffer* aBuffer);
		
		OMX_BUFFERHEADERTYPE* iBufferHeader;
		CMMFBuffer* iMmfBuffer;
		
		TBool iOwnsMmfBuffer;
		
		MOmxILComponentIfObserver* iPortObserver;
		};
		
public:
	COmxBufferManager(OMX_COMPONENTTYPE* aHandle);
	~COmxBufferManager();
	
	// Buffer lookup
	COmxBuffer* FindBuffer(const CMMFBuffer* aBuffer) const;
	COmxBuffer* FindBuffer(OMX_BUFFERHEADERTYPE* aBuffer) const;
	
	// OMX Calls
	TInt UseBuffer(CMMFBuffer& aBuffer, TUint nPortIndex);
	CMMFBuffer* AllocateBufferL(TUint nPortIndex, TUint nSizeBytes);
	TInt FreeBuffer(CMMFBuffer* aBuffer);
	
	TInt EmptyThisBuffer(const CMMFBuffer* aBuffer, MOmxILComponentIfObserver* aObserver);
	TInt FillThisBuffer(CMMFBuffer* aBuffer, MOmxILComponentIfObserver* aObserver);
	
	// Store OMX buffer pointer
	void StoreBufferL(OMX_BUFFERHEADERTYPE* aBufferHeader, CMMFBuffer* aBuffer);	
	
private:	
	RPointerArray<COmxBuffer> iBuffers;
	OMX_COMPONENTTYPE* iHandle;
	};

class COmxCallbacks : public CActive
	{
public:
	enum TMessageType 
		{
		EFillBufferCallback,
		EEmptyBufferCallback,
		EEventCallback
		};	

	class TEventParams
		{
	public:		
	 	OMX_EVENTTYPE iEvent; 
		TUint32 iData1;
		TUint32 iData2;
		TAny* iExtraData;
		};

	class TOmxMessage
		{
	public:
		TMessageType iType;
		OMX_HANDLETYPE iComponent;
		union 
			{
			COmxBufferManager::COmxBuffer* iBuffer;
			TEventParams iEventParams;
			};	
		};
public:
	static COmxCallbacks* NewL(MOmxILComponentIfObserver& aObserver);
	operator OMX_CALLBACKTYPE*();
	void RunL();
	void DoCancel();
	
	TInt FillBufferDone(OMX_HANDLETYPE aComponent, COmxBufferManager::COmxBuffer* aBuffer);
	TInt EmptyBufferDone(OMX_HANDLETYPE aComponent, COmxBufferManager::COmxBuffer* aBuffer);
	TInt EventHandler(OMX_HANDLETYPE aComponent, const TEventParams& aParams);
	virtual ~COmxCallbacks();

private:
	void ConstructL();
	COmxCallbacks(MOmxILComponentIfObserver& aObserver);

private:
	OMX_CALLBACKTYPE iHandle;
	RMsgQueue<TOmxMessage> iMsgQueue;
	MOmxILComponentIfObserver& iParent; 
	};

	
// OMX callback handler functions
OMX_ERRORTYPE EventHandler(OMX_OUT OMX_HANDLETYPE aComponent, 
					OMX_OUT TAny* aAppData,
        			OMX_OUT OMX_EVENTTYPE aEvent, 
        			OMX_OUT TUint32 aData1,
        			OMX_OUT TUint32 aData2,
        			OMX_OUT TAny* aExtra);
        			
OMX_ERRORTYPE EmptyBufferDone(
       OMX_HANDLETYPE aComponent,
       TAny* aAppData,
       OMX_BUFFERHEADERTYPE* aBuffer);
        
OMX_ERRORTYPE FillBufferDone(
       OMX_HANDLETYPE aComponent,
       TAny* aAppData,
       OMX_BUFFERHEADERTYPE* aBuffer);

#endif // OMXILGENERICILIFBODYIMPL_H
