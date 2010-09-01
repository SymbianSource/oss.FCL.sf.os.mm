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

#include <e32base.h>

#include "omxildummycontentpipe.h"
#include "omxildummycontentpipe.hrh"

// ECOM stuff...
const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidOmxILDummyContentPipe, COmxILDummyContentPipe::NewL),
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

COmxILDummyContentPipe* COmxILDummyContentPipe::NewL()
	{
	return new (ELeave) COmxILDummyContentPipe();
	}

COmxILDummyContentPipe::COmxILDummyContentPipe()
	{
	// fill in handle structure		
	
	CP_PIPETYPE::Open = &COmxILDummyContentPipe::Open;
	CP_PIPETYPE::Close = &COmxILDummyContentPipe::Close;
	CP_PIPETYPE::Create = &COmxILDummyContentPipe::Create;
	CP_PIPETYPE::CheckAvailableBytes = &COmxILDummyContentPipe::CheckAvailableBytes;
	CP_PIPETYPE::SetPosition = &COmxILDummyContentPipe::SetPosition;
	CP_PIPETYPE::GetPosition = &COmxILDummyContentPipe::GetPosition;
	CP_PIPETYPE::Read = &COmxILDummyContentPipe::Read; 
	CP_PIPETYPE::ReadBuffer = &COmxILDummyContentPipe::ReadBuffer;
	CP_PIPETYPE::ReleaseReadBuffer = &COmxILDummyContentPipe::ReleaseReadBuffer;
	CP_PIPETYPE::Write = &COmxILDummyContentPipe::Write; 
	CP_PIPETYPE::GetWriteBuffer = &COmxILDummyContentPipe::GetWriteBuffer;
	CP_PIPETYPE::WriteBuffer = &COmxILDummyContentPipe::WriteBuffer;
	CP_PIPETYPE::RegisterCallback = &COmxILDummyContentPipe::RegisterCallback;
	}

COmxILDummyContentPipe::~COmxILDummyContentPipe()
	{
	}

TInt COmxILDummyContentPipe::GetHandle(OMX_HANDLETYPE* aContentPipe)
	{
	*aContentPipe = static_cast<CP_PIPETYPE*>(this);
	return KErrNone;
	}

CPresult COmxILDummyContentPipe::Open(CPhandle* hContent, CPstring szURI, CP_ACCESSTYPE eAccess)
	{
	COmxILDummyContentHandle* dummyContentHandle = reinterpret_cast<COmxILDummyContentHandle*>(*hContent);
	TInt err = COmxILDummyContentHandle::Open(dummyContentHandle, _L8(szURI), eAccess);
	*hContent = dummyContentHandle;
	return err;
	}
 
CPresult COmxILDummyContentPipe::Close(CPhandle hContent)
	{
	COmxILDummyContentHandle* dummyContentHandle = reinterpret_cast<COmxILDummyContentHandle*>(hContent);
	TInt err = dummyContentHandle->Close();
	delete dummyContentHandle;
	return err;
	}
 
CPresult COmxILDummyContentPipe::Create(CPhandle* hContent, CPstring szURI)
	{
	COmxILDummyContentHandle* dummyContentHandle = reinterpret_cast<COmxILDummyContentHandle*>(*hContent);
	TInt err = COmxILDummyContentHandle::Create(dummyContentHandle, _L8(szURI));
	*hContent = dummyContentHandle;
	return err;
	}

CPresult COmxILDummyContentPipe::CheckAvailableBytes(CPhandle hContent, CPuint nBytesRequested, CP_CHECKBYTESRESULTTYPE* eResult)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->CheckAvailableBytes(nBytesRequested, eResult);
	}

CPresult COmxILDummyContentPipe::SetPosition(CPhandle hContent, CPint nOffset, CP_ORIGINTYPE eOrigin)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->SetPosition(nOffset, eOrigin);
	}

CPresult COmxILDummyContentPipe::GetPosition(CPhandle hContent, CPuint* pPosition)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->GetPosition((TUint*)pPosition);
	}

CPresult COmxILDummyContentPipe::Read(CPhandle hContent, CPbyte* pData, CPuint nSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->Read((TUint8*)pData, nSize);
	}

CPresult COmxILDummyContentPipe::ReadBuffer(CPhandle hContent, CPbyte** ppBuffer, CPuint* nSize, CPbool bForbidCopy)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->ReadBuffer((TUint8**)ppBuffer, (TUint*)nSize, bForbidCopy);
	}

CPresult COmxILDummyContentPipe::ReleaseReadBuffer(CPhandle hContent, CPbyte* pBuffer)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->ReleaseReadBuffer((TUint8*)pBuffer);
	}

CPresult COmxILDummyContentPipe::Write(CPhandle hContent, CPbyte* data, CPuint nSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->Write((TUint8*)data, nSize);
	}

CPresult COmxILDummyContentPipe::GetWriteBuffer(CPhandle hContent, CPbyte** ppBuffer, CPuint nSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->GetWriteBuffer((TUint8**)ppBuffer, nSize);
	}

CPresult COmxILDummyContentPipe::WriteBuffer(CPhandle hContent, CPbyte* pBuffer, CPuint nFilledSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->WriteBuffer((TUint8*)pBuffer, nFilledSize);
	}

CPresult COmxILDummyContentPipe::RegisterCallback(CPhandle hContent, CPresult (*ClientCallback)(CP_EVENTTYPE eEvent, CPuint iParam))
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->RegisterCallback((TUint (*)(CP_EVENTTYPE, TUint))ClientCallback);
	}

COmxILDummyContentPipe::COmxILDummyContentHandle::COmxILDummyContentHandle()
	{
	}

COmxILDummyContentPipe::COmxILDummyContentHandle::~COmxILDummyContentHandle()
	{
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::Open(COmxILDummyContentHandle*& aHandle, const TDesC8& /*aUri*/, CP_ACCESSTYPE /*aAccess*/)
	{
	aHandle = new COmxILDummyContentHandle;
	if (aHandle != NULL)
		{
		return KErrNone;
		}
	else
		{
		return KD_ENOMEM;
		}
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::Close()
	{
	return KErrNone;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::Create(COmxILDummyContentHandle*& aHandle, const TDesC8& /*aUri*/)
	{
	aHandle = new COmxILDummyContentHandle;
	if (aHandle != NULL)
		{
		return KErrNone;
		}
	else
		{
		return KD_ENOMEM;
		}
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::CheckAvailableBytes(TUint /*aBytesRequested*/, CP_CHECKBYTESRESULTTYPE* /*aResult*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::SetPosition(TInt aOffset, CP_ORIGINTYPE aOrigin)
	{
	switch(aOrigin)
	{
	case CP_OriginBegin:
		{
		if (aOffset < 0)
			return KD_EINVAL;
		iPosition = aOffset;
		break;
		}
		
	case CP_OriginCur:
	case CP_OriginEnd:
		{
		return KD_EINVAL;
		}
	
	default:
		{
		return KD_EINVAL;
		}
	}
	
	return KErrNone;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::GetPosition(TUint* aPosition)
	{
	*aPosition = iPosition;
	return KErrNone;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::Read(TUint8* /*aData*/, TUint /*aSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::ReadBuffer(TUint8** /*aBuffer*/, TUint* /*aSize*/, TBool /*aForbidCopy*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::ReleaseReadBuffer(TUint8* /*aBuffer*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::Write(TUint8* /*aData*/, TUint /*aSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::GetWriteBuffer(TUint8** /*aBuffer*/, TUint /*aSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::WriteBuffer(TUint8* /*aBuffer*/, TUint /*aFilledSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe::COmxILDummyContentHandle::RegisterCallback(TUint (* /*ClientCallback*/)(CP_EVENTTYPE aEvent, TUint aParam))
	{
	return KD_EINVAL;
	}
