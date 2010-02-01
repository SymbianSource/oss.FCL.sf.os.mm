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

#include "omxildummycontentpipe2.h"
#include "omxildummycontentpipe2.hrh"

OMXIL_CONTENTPIPE_ECOM_ENTRYPOINT(KUidOmxILDummyContentPipe2)

/* Content Pipe Entry Point */
TInt ContentPipeInit(OMX_HANDLETYPE* aContentPipe)
	{	
	TRAPD(err, *aContentPipe = COmxILDummyContentPipe2::NewL());
	return err;
	}

/* Content Pipe Exit Point */
TInt ContentPipeDeInit(OMX_HANDLETYPE aContentPipe)
	{
	delete static_cast<COmxILDummyContentPipe2*>(reinterpret_cast<CP_PIPETYPE*>(aContentPipe));
	return KErrNone;
	}


COmxILDummyContentPipe2* COmxILDummyContentPipe2::NewL()
	{
	return new (ELeave) COmxILDummyContentPipe2();
	}

COmxILDummyContentPipe2::COmxILDummyContentPipe2()
	{
	// fill in handle structure		
	
	CP_PIPETYPE::Open = &COmxILDummyContentPipe2::Open;
	CP_PIPETYPE::Close = &COmxILDummyContentPipe2::Close;
	CP_PIPETYPE::Create = &COmxILDummyContentPipe2::Create;
	CP_PIPETYPE::CheckAvailableBytes = &COmxILDummyContentPipe2::CheckAvailableBytes;
	CP_PIPETYPE::SetPosition = &COmxILDummyContentPipe2::SetPosition;
	CP_PIPETYPE::GetPosition = &COmxILDummyContentPipe2::GetPosition;
	CP_PIPETYPE::Read = &COmxILDummyContentPipe2::Read; 
	CP_PIPETYPE::ReadBuffer = &COmxILDummyContentPipe2::ReadBuffer;
	CP_PIPETYPE::ReleaseReadBuffer = &COmxILDummyContentPipe2::ReleaseReadBuffer;
	CP_PIPETYPE::Write = &COmxILDummyContentPipe2::Write; 
	CP_PIPETYPE::GetWriteBuffer = &COmxILDummyContentPipe2::GetWriteBuffer;
	CP_PIPETYPE::WriteBuffer = &COmxILDummyContentPipe2::WriteBuffer;
	CP_PIPETYPE::RegisterCallback = &COmxILDummyContentPipe2::RegisterCallback;
	}

COmxILDummyContentPipe2::~COmxILDummyContentPipe2()
	{
	}

CPresult COmxILDummyContentPipe2::Open(CPhandle* hContent, CPstring szURI, CP_ACCESSTYPE eAccess)
	{
	COmxILDummyContentHandle* dummyContentHandle = reinterpret_cast<COmxILDummyContentHandle*>(*hContent);
	TInt err = COmxILDummyContentHandle::Open(dummyContentHandle, _L8(szURI), eAccess);
	*hContent = dummyContentHandle;
	return err;
	}
 
CPresult COmxILDummyContentPipe2::Close(CPhandle hContent)
	{
	COmxILDummyContentHandle* dummyContentHandle = reinterpret_cast<COmxILDummyContentHandle*>(hContent);
	TInt err = dummyContentHandle->Close();
	delete dummyContentHandle;
	return err;
	}
 
CPresult COmxILDummyContentPipe2::Create(CPhandle* hContent, CPstring szURI)
	{
	COmxILDummyContentHandle* dummyContentHandle = reinterpret_cast<COmxILDummyContentHandle*>(*hContent);
	TInt err = COmxILDummyContentHandle::Create(dummyContentHandle, _L8(szURI));
	*hContent = dummyContentHandle;
	return err;
	}

CPresult COmxILDummyContentPipe2::CheckAvailableBytes(CPhandle hContent, CPuint nBytesRequested, CP_CHECKBYTESRESULTTYPE* eResult)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->CheckAvailableBytes(nBytesRequested, eResult);
	}

CPresult COmxILDummyContentPipe2::SetPosition(CPhandle hContent, CPint nOffset, CP_ORIGINTYPE eOrigin)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->SetPosition(nOffset, eOrigin);
	}

CPresult COmxILDummyContentPipe2::GetPosition(CPhandle hContent, CPuint* pPosition)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->GetPosition((TUint*)pPosition);
	}

CPresult COmxILDummyContentPipe2::Read(CPhandle hContent, CPbyte* pData, CPuint nSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->Read((TUint8*)pData, nSize);
	}

CPresult COmxILDummyContentPipe2::ReadBuffer(CPhandle hContent, CPbyte** ppBuffer, CPuint* nSize, CPbool bForbidCopy)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->ReadBuffer((TUint8**)ppBuffer, (TUint*)nSize, bForbidCopy);
	}

CPresult COmxILDummyContentPipe2::ReleaseReadBuffer(CPhandle hContent, CPbyte* pBuffer)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->ReleaseReadBuffer((TUint8*)pBuffer);
	}

CPresult COmxILDummyContentPipe2::Write(CPhandle hContent, CPbyte* data, CPuint nSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->Write((TUint8*)data, nSize);
	}

CPresult COmxILDummyContentPipe2::GetWriteBuffer(CPhandle hContent, CPbyte** ppBuffer, CPuint nSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->GetWriteBuffer((TUint8**)ppBuffer, nSize);
	}

CPresult COmxILDummyContentPipe2::WriteBuffer(CPhandle hContent, CPbyte* pBuffer, CPuint nFilledSize)
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->WriteBuffer((TUint8*)pBuffer, nFilledSize);
	}

CPresult COmxILDummyContentPipe2::RegisterCallback(CPhandle hContent, CPresult (*ClientCallback)(CP_EVENTTYPE eEvent, CPuint iParam))
	{
	return reinterpret_cast<COmxILDummyContentHandle*>(hContent)->RegisterCallback((TUint (*)(CP_EVENTTYPE, TUint))ClientCallback);
	}

COmxILDummyContentPipe2::COmxILDummyContentHandle::COmxILDummyContentHandle()
	{
	}

COmxILDummyContentPipe2::COmxILDummyContentHandle::~COmxILDummyContentHandle()
	{
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::Open(COmxILDummyContentHandle*& aHandle, const TDesC8& /*aUri*/, CP_ACCESSTYPE /*aAccess*/)
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

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::Close()
	{
	return KErrNone;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::Create(COmxILDummyContentHandle*& aHandle, const TDesC8& /*aUri*/)
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

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::CheckAvailableBytes(TUint /*aBytesRequested*/, CP_CHECKBYTESRESULTTYPE* /*aResult*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::SetPosition(TInt aOffset, CP_ORIGINTYPE aOrigin)
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

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::GetPosition(TUint* aPosition)
	{
	*aPosition = iPosition;
	return KErrNone;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::Read(TUint8* /*aData*/, TUint /*aSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::ReadBuffer(TUint8** /*aBuffer*/, TUint* /*aSize*/, TBool /*aForbidCopy*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::ReleaseReadBuffer(TUint8* /*aBuffer*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::Write(TUint8* /*aData*/, TUint /*aSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::GetWriteBuffer(TUint8** /*aBuffer*/, TUint /*aSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::WriteBuffer(TUint8* /*aBuffer*/, TUint /*aFilledSize*/)
	{
	return KD_EINVAL;
	}

TInt COmxILDummyContentPipe2::COmxILDummyContentHandle::RegisterCallback(TUint (* /*ClientCallback*/)(CP_EVENTTYPE aEvent, TUint aParam))
	{
	return KD_EINVAL;
	}
