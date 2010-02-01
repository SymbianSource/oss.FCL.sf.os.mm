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

/**
 @file
 @internalTechnology 
*/

inline 
CPixelConsumerBase::CPixelConsumerBase(TInt aPixelSize)
:iPixelSize(aPixelSize)
	{
	}

inline
void CPixelConsumerBase::GetBuffer()
	{
	iNeedBufferFlush = ETrue;
	ASSERT(iFrameBuffer);
	}

inline
TBool CPixelConsumerBase::Enabled() const
	{
	return iEnabled;
	}

inline
void CPixelConsumerBase::SetEnabled(TBool aEnabled)
	{
	iEnabled = aEnabled;
	}

inline
const TRect& CPixelConsumerBase::ClippingRect() const
	{
	return iClippingRect;
	}


inline
void CPixelConsumerBase::SetPos(const TPoint& aPosition)
	{
	iCurrentPos = aPosition;
	}

inline
void CPixelConsumerBase::SetOrigin(const TPoint& aOrigin)
	{
	iOrigin = aOrigin;
	}

inline
void CPixelConsumerBase::SetAlphaEnabledBitmap(TBool aEnabled)
	{
	ASSERT(iPixelSize == KRgbaPixelSize);
	iAlphaEnabled = aEnabled;
	}

inline
TInt CPixelConsumerBase::PixelSize() const
	{
	return iPixelSize;
	}

inline
CRgbPixelConsumer::CRgbPixelConsumer(TInt aPixelSize)
:CPixelConsumerBase(aPixelSize)
	{
	ASSERT(KRedShift==16 && KGreenShift==8 && KBlueShift==0 && KAlphaShift==24);
	}

inline
TUint CRgbPixelConsumer::GetPixel(const TUint8* aPtr) const
	{
	return TUint(*aPtr) | TUint(aPtr[1]<<8) | TUint(aPtr[2]<<16);
	}

inline
void CRgbPixelConsumer::SetAlphaConsumer(CAlphaPixelConsumer* aAlphaConsumer)
	{
	iAlphaConsumer = aAlphaConsumer;
	}

inline
TUint CRgbPixelConsumer::GetPixel(const TPoint& aPosition) const
	{
	const TPoint RealPos = iOrigin + aPosition;
	if (Contains(iRealOutputRect, RealPos))
		{
		const TUint8* ptr=iFrameBuffer+KRgbPixelSize*(RealPos.iY*iFrameSize.iWidth+RealPos.iX);
		return GetPixel(ptr);
		}
	return 0;
	}

inline
void CAlphaPixelConsumer::FillBy(TUint aAlpha)
	{
	GetBuffer();
	Mem::Fill(iFrameBuffer, iFrameBufferSize, aAlpha);
	}

inline
CAlphaPixelConsumer::CAlphaPixelConsumer()
:CPixelConsumerBase(KAlphaPixelSize)
	{
	}

inline
TUint CAlphaPixelConsumer::GetPixel(const TPoint& aPosition) const
	{
	const TPoint RealPos = iOrigin + aPosition;
	if (Contains(iRealOutputRect, RealPos))
		{
		return *(iFrameBuffer + RealPos.iY*iFrameSize.iWidth + RealPos.iX );
		}
	return 0;
	}

inline 
TUint CAlphaPixelConsumer::GetPixel(TInt aBufferOffset) const
	{
	ASSERT(aBufferOffset < iFrameBufferSize);
	return TUint( iFrameBuffer[aBufferOffset] );
	}

inline
void CAlphaPixelConsumer::SetPixel(TRgbaColour aPixelAlpha)
	{
	const TPoint RealPos = iOrigin + iCurrentPos;

	TUint8* ptr=iFrameBuffer + RealPos.iY*iFrameSize.iWidth + RealPos.iX ;
	*ptr=TUint8(aPixelAlpha & 0xff);
	}
	
inline
void CAlphaPixelConsumer::SetPixelByBufferOffset(TInt aPixelOffset, TRgbaColour aPixelAlpha)
	{
	ASSERT(aPixelOffset < iFrameBufferSize);
	iFrameBuffer[aPixelOffset] = TUint8(aPixelAlpha & 0xff);
	}


