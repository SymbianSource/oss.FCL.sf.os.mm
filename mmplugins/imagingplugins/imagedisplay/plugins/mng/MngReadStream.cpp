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
#include <caf/data.h>
#include "MngPanic.h"
#include "Crc32.h"

#include "MngReadStream.h"

static inline
void Big2SmallEndian(const TInt32 aBig, TInt32& aSmall)
	{
	aSmall = ((aBig&0xff)<<24) | ((aBig&0xff00)<<8) | ((aBig&0xff0000)>>8) | ((aBig>>24)&0xff);
	}

static inline
void Big2SmallEndian(const TUint16 aBig, TUint16& aSmall)
	{
	aSmall = TUint16( ((aBig&0xff)<<8) | ((aBig>>8)&0xff) );
	}

/*static*/
CMngFileReadStream* CMngFileReadStream::NewL(ContentAccess::CData* aCafData)
	{
	CleanupStack::PushL(aCafData);
	CMngFileReadStream* self=new (ELeave) CMngFileReadStream(aCafData);
	CleanupStack::Pop();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*static*/
CMngFileReadStream* CMngFileReadStream::NewL(const TDesC8& aData)
	{
	CMngFileReadStream* self=new (ELeave) CMngFileReadStream();
	CleanupStack::PushL(self);
	self->ConstructL(aData);
	CleanupStack::Pop(self);
	return self;
	}

void CMngFileReadStream::ConstructL()
	{
	ASSERT(iCafData);
	TInt len=0;
	iCafData->DataSizeL(len);
	iOwnedData = HBufC8::NewL(len);
	TPtr8 ptr(iOwnedData->Des());
	User::LeaveIfError(iCafData->Read(ptr));
	delete iCafData;
	iCafData = NULL;
	ConstructL(ptr);
	}

void CMngFileReadStream::ConstructL(const TDesC8& aData)
	{
	iMemBuf.Set(aData);
	}

CMngFileReadStream::~CMngFileReadStream()
	{
	delete iOwnedData;
	delete iCafData;
	}

void CMngFileReadStream::AddRef()
	{
	++iRefCount;
	}

void CMngFileReadStream::Release()
	{
	if (--iRefCount==0)
		{
		delete this;
		}
	}

TInt CMngFileReadStream::DataAvailable()
	{
	return iMemBuf.Length() - iPosition;
	}

void CMngFileReadStream::WaitForData(TInt aDataNeeded, TRequestStatus& aReq)
	{
	TRequestStatus* pReq=&aReq;
	if (iPosition + aDataNeeded <= DataAvailable() )
		{
		User::RequestComplete(pReq, KErrNone);
		}
	else
		{
		User::RequestComplete(pReq, KErrUnderflow);
		}
	}

void CMngFileReadStream::SeekL(TInt aPosition)
	{
	if (aPosition >= iPosition + DataAvailable() )
		{
		User::Leave(KErrEof);
		}
	iPosition = aPosition;
	}

TInt CMngFileReadStream::Tell()
	{
	return iPosition;
	}

void CMngFileReadStream::Read(TDes8& aBuf, TRequestStatus& aReq, TInt32* aCrc)
	{
	TRequestStatus* pReq=&aReq;
	if (aBuf.MaxLength() > DataAvailable())
		{
		User::RequestComplete(pReq, KErrEof);
		}
	else
		{
		aBuf.Copy(&iMemBuf[iPosition], aBuf.MaxLength());
		aBuf.SetLength(aBuf.MaxLength());
		iPosition +=aBuf.Length();
		if (NULL!=aCrc)
			{
			Crc32::Calc(*aCrc, aBuf.Ptr(), aBuf.Length());
			}
		User::RequestComplete(pReq, KErrNone);
		}
	}

void CMngFileReadStream::ReadL(TDes8& aBuf, TInt32* aCrc)
	{
	if (aBuf.MaxLength() > DataAvailable())
		{
		User::Leave(KErrEof);
		}
	else
		{
		aBuf.Copy(&iMemBuf[iPosition], aBuf.MaxLength());
		aBuf.SetLength(aBuf.MaxLength());
		iPosition +=aBuf.Length();
		if (NULL!=aCrc)
			{
			Crc32::Calc(*aCrc, aBuf.Ptr(), aBuf.Length());
			}
		}
	}


void  CMngFileReadStream::ReadInt32L(TInt32& aNumber, TInt32* aCrc)
	{
	if ( TInt(sizeof(aNumber)) > DataAvailable())
		{
		User::Leave(KErrUnderflow);
		}
	Mem::Copy(&aNumber, &iMemBuf[iPosition], sizeof(aNumber) );
	if (NULL!=aCrc)
		{
		Crc32::Calc(*aCrc, &aNumber, sizeof(aNumber) );
		}
	Big2SmallEndian(aNumber,aNumber);
	iPosition +=sizeof(aNumber);
	}

void CMngFileReadStream::ReadUint16L(TUint16& aNumber, TInt32* aCrc)
	{
	if ( TInt(sizeof(aNumber)) > DataAvailable())
		{
		User::Leave(KErrUnderflow);
		}
	Mem::Copy(&aNumber, &iMemBuf[iPosition], sizeof(aNumber) );
	if (NULL!=aCrc)
		{
		Crc32::Calc(*aCrc, &aNumber, sizeof(aNumber) );
		}
	Big2SmallEndian(aNumber,aNumber);
	iPosition +=sizeof(aNumber);
	}

void  CMngFileReadStream::ReadUint8L(TUint8& aNumber, TInt32* aCrc)
	{
	if (TInt(sizeof(aNumber)) > DataAvailable())
		{
		User::Leave(KErrUnderflow);
		}
	aNumber = iMemBuf[iPosition++];
	if (NULL!=aCrc)
		{
		Crc32::Calc(*aCrc, &aNumber, sizeof(aNumber) );
		}
	}

void CMngFileReadStream::ReadInt32L(TInt32* aFirst, TInt aCount, TInt32* aCrc)
	{
	const TInt Size = aCount*sizeof(*aFirst);
	if (Size > DataAvailable())
		{
		User::Leave(KErrUnderflow);
		}
	Mem::Copy(aFirst, &iMemBuf[iPosition], Size);
	if (NULL!=aCrc)
		{
		Crc32::Calc(*aCrc, aFirst, Size );
		}
	for (; aCount; --aCount,++aFirst)
		{
		Big2SmallEndian(*aFirst, *aFirst);
		}
	iPosition +=Size;
	}

void CMngFileReadStream::ReadUint16L(TUint16* aFirst,TInt aCount, TInt32* aCrc)
	{
	const TInt Size = aCount*sizeof(*aFirst);
	if (Size > DataAvailable())
		{
		User::Leave(KErrUnderflow);
		}
	Mem::Copy(aFirst, &iMemBuf[iPosition], Size);
	if (NULL!=aCrc)
		{
		Crc32::Calc(*aCrc, aFirst, Size );
		}
	for (; aCount; --aCount,++aFirst)
		{
		Big2SmallEndian(*aFirst, *aFirst);
		}
	iPosition +=Size;
	}

void CMngFileReadStream::ReadUint8L(TUint8* aFirst, TInt aCount, TInt32* aCrc)
	{
	if (aCount > DataAvailable())
		{
		User::Leave(KErrUnderflow);
		}
	Mem::Copy(aFirst, &iMemBuf[iPosition], aCount);
	if (NULL!=aCrc)
		{
		Crc32::Calc(*aCrc, aFirst, aCount );
		}
	iPosition +=aCount;
	}

/*static*/
CMngIclReadStream* CMngIclReadStream::NewL(MDataFeed& aDataSupplier)
	{
	CMngIclReadStream* self=new (ELeave) CMngIclReadStream(aDataSupplier);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMngIclReadStream::ConstructL()
	{
	RestartL();
	}

void CMngIclReadStream::RestartL()
	{
	Reset();
	User::LeaveIfError( iDataSupplier.LockBuffer(iOverallPosition,iMemBuf,64) );
	}

void CMngIclReadStream::WaitForData(TInt aDataNeeded, TRequestStatus& aReq)
	{
	TRequestStatus* pReq=&aReq;
	if (aDataNeeded <= iMemBuf.Length() - iPosition)
		{
		User::RequestComplete(pReq, KErrNone);
		return;
		}
	TInt err=iDataSupplier.LockBuffer(iOverallPosition+iPosition, iMemBuf, aDataNeeded);
	if (err == KErrNone)
		{
		iOverallPosition+=iPosition;
		iPosition=0;
		}
	User::RequestComplete(pReq, err);
	}

TInt CMngIclReadStream::Tell()
	{
	return iOverallPosition+iPosition;
	}

void CMngIclReadStream::SeekL(TInt aPosition)
	{
	if (aPosition<0)
		{
		__DEBUGGER();
		User::Leave(KErrNotSupported);
		return;
		}
	if (aPosition - iOverallPosition > DataAvailable() || aPosition < iOverallPosition)
		{
		iPosition		= 0;
		iOverallPosition= aPosition;
		iMemBuf.Set(KNullDesC8);
		}
	else
		{
		iPosition = aPosition - iOverallPosition;
		}
	}

void CMngIclReadStream::InvalidateBuffer()
	{
	iMemBuf.Set(KNullDesC8);
	}

void CMngIclReadStream::Reset()
	{
	InvalidateBuffer();
	iOverallPosition= 0;
	iPosition		= 0;
	}
	
