// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PNGSCANLINEENCODER_H__
#define __PNGSCANLINEENCODER_H__

#include "PNGCodec.h"
#include "ImageUtils.h"

const TInt KPngScanlineFilterTypeLength = 1;

// CBitDepth1Encoder
const TInt KPngDepth1PixelsPerByte = 8;
const TInt KPngDepth1RoundUpValue = 7;
const TInt KPngDepth1ShiftValue = 1;
class CBitDepth1Encoder : public CPngWriteSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoEncode(const CFbsBitmap* aSource, const TInt aScanline,
					TUint8* aDataPtr, const TUint8* aDataPtrLimit);
	};

// CBitDepth2Encoder
const TInt KPngDepth2PixelsPerByte = 4;
const TInt KPngDepth2RoundUpValue = 3;
const TInt KPngDepth2ShiftValue = 2;
class CBitDepth2Encoder : public CPngWriteSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoEncode(const CFbsBitmap* aSource, const TInt aScanline,
					TUint8* aDataPtr, const TUint8* aDataPtrLimit);
	};

// CBitDepth4Encoder
const TInt KPngDepth4PixelsPerByte = 2;
const TInt KPngDepth4RoundUpValue = 1;
const TInt KPngDepth4ShiftValue = 4;
class CBitDepth4Encoder : public CPngWriteSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoEncode(const CFbsBitmap* aSource, const TInt aScanline,
					TUint8* aDataPtr, const TUint8* aDataPtrLimit);
	};

// CBitDepth8ColorType2Encoder
const TInt KPngDepth8RgbBytesPerPixel = 3;
class CBitDepth8ColorType2Encoder : public CPngWriteSubCodec
	{
private:
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoEncode(const CFbsBitmap* aSource, const TInt aScanline,
					TUint8* aDataPtr, const TUint8* aDataPtrLimit);
	};

// CBitDepth8Encoder
const TInt KPngDepth8PixelsPerByte = 1;
const TInt KPngDepth8ShiftValue = 0;
class CBitDepth8Encoder : public CPngWriteSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoEncode(const CFbsBitmap* aSource, const TInt aScanline,
					TUint8* aDataPtr, const TUint8* aDataPtrLimit);
	};

#endif // __PNGSCANLINEENCODER_H__
