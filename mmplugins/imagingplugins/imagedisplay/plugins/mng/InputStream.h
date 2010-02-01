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

#ifndef __INPUTSTREAM_H__

#define __INPUTSTREAM_H__

/** 	@file
	@internalTechnology */

class MInputStream 
	{
public:
	virtual TInt DataAvailable()=0;
	virtual void WaitForData(TInt aDataNeeded, TRequestStatus& aReq)=0;

	virtual void SeekL(TInt aPosition)=0;
	virtual TInt Tell()=0;

	virtual void Read(TDes8& aBuf, TRequestStatus& aReq, TInt32* aCrc=NULL)=0;
	virtual void ReadL(TDes8& aBuf, TInt32* aCrc=NULL)=0;

	virtual void ReadInt32L(TInt32& aNumber, TInt32* aCrc=NULL)=0;
	virtual void ReadUint16L(TUint16& aNumber, TInt32* aCrc=NULL)=0;
	virtual void ReadUint8L(TUint8& aNumber, TInt32* aCrc=NULL)=0;

	virtual void ReadInt32L(TInt32*, TInt aCount, TInt32* aCrc=NULL)=0;
	virtual void ReadUint16L(TUint16*,TInt aCount, TInt32* aCrc=NULL)=0;
	virtual void ReadUint8L(TUint8*, TInt aCount, TInt32* aCrc=NULL)=0;
	
	// object control
	virtual void Release()=0;
	virtual void AddRef()=0;
	};
	
/**
	Interface that can be used as a data supplier for MInputStream
	implementation
*/
class MDataFeed
	{
public:
	/**
		Should be used to provide interface client with guarantee that data will stay
		in buffer indefinitely.
	*/
	virtual TInt LockBuffer(TInt aPosition, TPtrC8& aBuffer, TInt aBytesToLock) = 0;
	};

#endif // ndef __INPUTSTREAM_H__
