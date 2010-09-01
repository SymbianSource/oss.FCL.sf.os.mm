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

#ifndef __PIXELCONSUMER_H__

#define __PIXELCONSUMER_H__
/** 	@file
	@internalTechnology */

#include "ColourDefs.h"

class CFbsBitmap;

inline
TBool Contains(const TRect& aR, const TPoint &aPoint)
	{
	return !(aPoint.iX<aR.iTl.iX || aPoint.iX>=aR.iBr.iX || aPoint.iY<aR.iTl.iY || aPoint.iY>=aR.iBr.iY);
	}

/*
#define used to improve readability of the code where it is used.

@see CRgbPixelConsumer::SetRGBPixel
@see CRgbPixelConsumer::SetRGBAPixel
*/
#define CALL_SETPIXEL_FUNCTION(pixelConsumer, ptrToSetPixelFn)	((pixelConsumer).*(ptrToSetPixelFn))

class CPixelConsumerBase: public CBase 
	{
public:

	~CPixelConsumerBase();
	void InitL(const TSize& aFrameSize);
	inline void SetPos(const TPoint& aPosition);
	void Prepare();
	void GetBitmap(CFbsBitmap& aBitmap, const TRect& aSourceRect);
	inline const TRect& ClippingRect() const;
	void SetClippingRect(const TRect& aRect);
	inline void SetOrigin(const TPoint& aOrigin);
	inline TBool Enabled() const;
	inline void SetEnabled(TBool aEnabled);
	void Clear(TUint aColour, const TRect& aRect);
	inline void SetAlphaEnabledBitmap(TBool aEnabled);
	void ClearAlpha(TUint aColour, const TRect& aRect);
	inline TInt PixelSize() const;
	
protected:
	inline CPixelConsumerBase(TInt aPixelSize);
	inline void GetBuffer();

protected:
	TInt		iPixelSize;
	TUint8*		iFrameBuffer;
	TInt		iFrameBufferSize;
	TSize		iFrameSize;
	TRect		iClippingRect;
	TRect		iRealOutputRect;
	TRect		iImageRect;
	TPoint		iCurrentPos;
	TPoint		iOrigin;
	TBool		iAlphaEnabled;
private:
	TBool		iNeedBufferFlush;
	TBool		iEnabled;
	};

class CAlphaPixelConsumer;
class CRgbPixelConsumer:public CPixelConsumerBase
	{
public:
	typedef void (CRgbPixelConsumer::*SetPixelFunc)(TRgbaColour aPixelColour);
	inline CRgbPixelConsumer(TInt aPixelSize);
	void SetRGBPixel(TRgbaColour aPixelColour);
	void SetRGBAPixel(TRgbaColour aPixelColour);
	TUint GetPixel(const TPoint& aPosition) const;
	inline void SetAlphaConsumer(CAlphaPixelConsumer* aAlphaConsumer);
protected:
	inline TUint GetPixel(const TUint8* aPtr) const;
	
protected:	
	CAlphaPixelConsumer* iAlphaConsumer;
	};

class CAlphaPixelConsumer:public CPixelConsumerBase
	{
public:
	inline CAlphaPixelConsumer();
	void SetPixel(TRgbaColour aPixelAlpha);
	inline void SetPixelByBufferOffset(TInt aOffset, TRgbaColour aPixelAlpha);
	inline void FillBy(TUint aAlpha);
	inline TUint GetPixel(const TPoint& aPosition) const;
	inline TUint GetPixel(TInt aBufferOffset) const;
	};

#include "PixelConsumer.inl"

#endif // __PIXELCONSUMER_H__
