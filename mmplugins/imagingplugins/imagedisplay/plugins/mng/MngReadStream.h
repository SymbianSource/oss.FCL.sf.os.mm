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

#ifndef __MNGREADSTREAM_H__
#define __MNGREADSTREAM_H__

/** 	@file
	@internalTechnology */

#include <caf/content.h>

#include "InputStream.h"

class CMngFileReadStream : protected CBase, public MInputStream
	{
public:

static CMngFileReadStream* NewL(ContentAccess::CData* aCafData);
static CMngFileReadStream* NewL(const TDesC8& aData);

	virtual TInt DataAvailable();
	virtual void WaitForData(TInt aDataNeeded, TRequestStatus& aReq);

	virtual void SeekL(TInt aPosition);
	virtual TInt Tell();

	virtual void Read(TDes8& aBuf, TRequestStatus& aReq, TInt32* aCrc=NULL);
	virtual void ReadL(TDes8& aBuf, TInt32* aCrc=NULL);

	virtual void ReadInt32L(TInt32& aNumber, TInt32* aCrc=NULL);
	virtual void ReadUint16L(TUint16& aNumber, TInt32* aCrc=NULL);
	virtual void ReadUint8L(TUint8& aNumber, TInt32* aCrc=NULL);

	virtual void ReadInt32L(TInt32* aFirst, TInt aCount, TInt32* aCrc=NULL);
	virtual void ReadUint16L(TUint16* aFirst,TInt aCount, TInt32* aCrc=NULL);
	virtual void ReadUint8L(TUint8* aFirst, TInt aCount, TInt32* aCrc=NULL);
	
	virtual void Release();
	virtual void AddRef();

protected:
	inline explicit CMngFileReadStream(ContentAccess::CData* aCafData=NULL);
	~CMngFileReadStream();

	void ConstructL(const TDesC8& aData);
protected:
	TPtrC8	iMemBuf;
	TInt	iPosition;	// read position within the current buffer i.e. iMemBuf

private:
	void ConstructL();

private:
	HBufC8* iOwnedData;
	ContentAccess::CData* iCafData;
	TInt	iRefCount;
	};

inline 
CMngFileReadStream::CMngFileReadStream(ContentAccess::CData* aCafData):
															iCafData(aCafData),iRefCount(1)
	{
	}
	
	
/**
	Implementation of MInputStream interface for MNG player that
	provides with adaptation over the ICL framework data reading 
	interface
*/
class CMngIclReadStream: public CMngFileReadStream 
	{
public:
	static CMngIclReadStream* NewL(MDataFeed& aDataSupplier);

	virtual void WaitForData(TInt aDataNeeded, TRequestStatus& aReq);
	virtual void SeekL(TInt aPosition);
	virtual TInt Tell();

	void InvalidateBuffer();
	void RestartL();
	void Reset();

protected:
	inline explicit CMngIclReadStream(MDataFeed& aDataSupplier);

private:
	void ConstructL();

private:
	MDataFeed&	iDataSupplier;
	TInt		iOverallPosition;	// offset of the data held in the iMemBuf within the image data
	};

inline 
CMngIclReadStream::CMngIclReadStream(MDataFeed& aDataSupplier):
													CMngFileReadStream(NULL),iDataSupplier(aDataSupplier)
	{
	}

#endif // ndef __MNGREADSTREAM_H__
