// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _STREAM_DECODER_IFACE_H

#define _STREAM_DECODER_IFACE_H

/** 	@file
	@internalComponent */

class MImageStreamDecoder;
class MImageStreamDecoderFactory 
	{
public:
	virtual void CreatePngDecoderL(MImageStreamDecoder*& aPtr)=0;
	virtual void SetDtorKey(TUid aUid)=0;
	virtual void Release()=0;
	};

class MUniqueChunkDataProvider;
class CImageProcessor;
class CFbsBitmap;
class MImageStreamDecoder 
	{
public:

	virtual void InitL(CImageProcessor& aImageProcessor, CImageProcessor* aMaskProcessor, CFbsBitmap* aDestination, 
						const TPtrC8& aData, MUniqueChunkDataProvider& aProvider)=0;

	virtual void DecodeL()=0;
	virtual void ResetL()=0;

	virtual const TSize& ImageSize()=0;

	virtual void Release()=0;
	};

class MUniqueChunkDataProvider 
	{
public:
   virtual TInt GetChunkData(const TText8* aChunkId, const TUint8*& aPalettePtr, TInt& aSizeInBites)=0;
	};

#endif // _STREAM_DECODER_IFACE_H
