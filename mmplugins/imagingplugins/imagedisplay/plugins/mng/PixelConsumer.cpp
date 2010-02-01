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
#include <fbs.h>

#include "PixelConsumer.h"

inline 
void SetRgbPixel(TUint8* ptr,const TUint aR, const TUint aG,const TUint aB)
	{
	*ptr   	= TUint8(aB);
	ptr[1]	= TUint8(aG);
	ptr[2]  = TUint8(aR);
	}

inline 
void SetRgbaPixel(TUint8* ptr,const TUint aR, const TUint aG,const TUint aB, const TUint aA)
	{
	*ptr	= TUint8(aB);
	ptr[1]	= TUint8(aG);
	ptr[2]	= TUint8(aR);
	ptr[3]  = TUint8(aA);
	}

void CPixelConsumerBase::InitL(const TSize& aFrameSize)
	{
	ASSERT(iFrameBuffer == NULL);
	iFrameSize	= aFrameSize;
	iFrameBufferSize = iFrameSize.iHeight*iFrameSize.iWidth*iPixelSize;
	
	iFrameBuffer= new (ELeave) TUint8[ iFrameBufferSize ];
	
	if(iPixelSize == KRgbaPixelSize)
		{
		TUint32* ptr = reinterpret_cast<TUint32*>(iFrameBuffer);
		TUint32* end = ptr+(iFrameBufferSize/iPixelSize);
		while(ptr < end)
			{
			*ptr++ = 0xFF000000; // initialise 32bit RGB buffer with opaque alpha (for both 16MU and 16MA)
			}
		}
	else
		{
		Mem::Fill(iFrameBuffer, iFrameBufferSize, 0);	
		}

	iImageRect = TRect(TPoint(0,0),iFrameSize.AsPoint());
	SetClippingRect(iImageRect);
	iNeedBufferFlush = ETrue;
	}

CPixelConsumerBase::~CPixelConsumerBase()
	{
	delete [] iFrameBuffer;
	}

void CPixelConsumerBase::Prepare()
	{
	iNeedBufferFlush = ETrue;
	GetBuffer();
	iCurrentPos.SetXY(0,0);
	}

void CPixelConsumerBase::SetClippingRect(const TRect& aRect)
	{
	iClippingRect	= aRect;
	iRealOutputRect	= aRect;
	iRealOutputRect.Intersection(iImageRect);
	}

void CPixelConsumerBase::GetBitmap(CFbsBitmap& aBitmap, const TRect& aSourceRect)
	{
	ASSERT(aBitmap.DisplayMode()==EColor16M || aBitmap.DisplayMode()==EColor16MA || 
			aBitmap.DisplayMode()==EColor16MU || aBitmap.DisplayMode()==EGray256);
	
	TRect imgRect(TPoint(0,0), iFrameSize.AsPoint() );
	imgRect.Intersection(aSourceRect);
	ASSERT(imgRect == aSourceRect); // ensure that source rect is within the buffer

	if (iNeedBufferFlush)
		{
		TUint8* DataAddr=reinterpret_cast<TUint8*>( aBitmap.DataAddress() );
		const TUint8* FrameBufAddr	=iFrameBuffer + iPixelSize*( aSourceRect.iTl.iY*iFrameSize.iWidth+aSourceRect.iTl.iX );
		const TInt KLineSize		=iPixelSize*aSourceRect.Width();
		const TInt KFrameBufLineSize=iPixelSize*iFrameSize.iWidth;
		const TInt KBmpScanLineLength= CFbsBitmap::ScanLineLength(aBitmap.SizeInPixels().iWidth, aBitmap.DisplayMode() );
		for (TInt y=aSourceRect.iBr.iY-aSourceRect.iTl.iY; y; --y, DataAddr +=KBmpScanLineLength, FrameBufAddr+=KFrameBufLineSize)
			{
			Mem::Copy(DataAddr, FrameBufAddr, KLineSize);
			}
		}
	iNeedBufferFlush = EFalse;
	}

void CPixelConsumerBase::Clear(TUint aColour, const TRect& aRect)
	{
	GetBuffer();
	const TInt width=aRect.Width();
	if (width<1)
		{
		return;
		}
	
	switch(iPixelSize)
		{
		case KRgbPixelSize:
			{
			const TUint r=Red(aColour);
			const TUint g=Green(aColour);
			const TUint b=Blue(aColour);
			for (TInt y=aRect.iTl.iY; y < aRect.iBr.iY; ++y)
				{
				TUint8* pPixel=(iFrameBuffer+KRgbPixelSize*(y*iFrameSize.iWidth+aRect.iTl.iX));
				TInt x=width;
				do {
					SetRgbPixel(pPixel, r, g, b);
					pPixel+=KRgbPixelSize;
					} while (--x);
				}
			break;
			}
		case KRgbaPixelSize:
			{
			const TUint r=Red(aColour);
			const TUint g=Green(aColour);
			const TUint b=Blue(aColour);
			const TUint a=Alpha(aColour);
			for (TInt y=aRect.iTl.iY; y < aRect.iBr.iY; ++y)
				{
				TUint8* pPixel=(iFrameBuffer+KRgbaPixelSize*(y*iFrameSize.iWidth+aRect.iTl.iX));
				TInt x=width;
				do {
					SetRgbaPixel(pPixel, r, g, b, a);
					pPixel+=KRgbaPixelSize;
					} while (--x);
				}
			break;
			}
		case KAlphaPixelSize:
			{
			TUint8* pPixel=iFrameBuffer+KAlphaPixelSize*(aRect.iTl.iY*iFrameSize.iWidth+aRect.iTl.iX);
			for (TInt y=aRect.iTl.iY; y < aRect.iBr.iY; ++y)
				{
				Mem::Fill(pPixel, width, aColour );
				pPixel += iFrameSize.iWidth;
				}
			break;
			}
		default:
			ASSERT(EFalse);
		}
	}

void CPixelConsumerBase::ClearAlpha(TUint aColour, const TRect& aRect)
	{
	ASSERT(iPixelSize == KRgbaPixelSize);
	
	GetBuffer();
	const TInt width=aRect.Width();
	if (width<1)
		{
		return;
		}

	const TUint a=Alpha(aColour);
	for (TInt y=aRect.iTl.iY; y < aRect.iBr.iY; ++y)
		{
		TUint8* pPixel=(iFrameBuffer+KRgbaPixelSize*(y*iFrameSize.iWidth+aRect.iTl.iX));
		TInt x=width;
		do 
			{
			pPixel[3] = a;
			pPixel+=KRgbaPixelSize;
			} while (--x);
		}
	}

void CRgbPixelConsumer::SetRGBPixel(TRgbaColour aPixelColour)
	{
	const TPoint RealPos( iOrigin.iX + iCurrentPos.iX, iOrigin.iY + iCurrentPos.iY );
	if (Contains(iRealOutputRect, RealPos))
		{
		
		if (NULL != iAlphaConsumer)
			{
// IF AlphaConsumer (if trans or alpha), then get alpha value
			const TUint alpha= (iAlphaConsumer->Enabled()? Alpha(aPixelColour) : KOpaqueAlpha );
//
// alpha blending is: 
//			Aresult * Cresult	= Afg*Cfg + Abg*Cbg - Afg*Abg*Cbg OR Afg*Cfg + Cbg * Abg*(1 - Afg)
//			Aresult				= Afg + (1 - Afg) * Abg
//			where 0 <= Alpha < 1
//			However we are using fixed point where
//				  0 <= Alpha <= 0xff
//
			if (alpha)
				{
				const TInt bufferOffset=RealPos.iY*iFrameSize.iWidth+RealPos.iX;
				TUint8* ptr=iFrameBuffer+KRgbPixelSize*bufferOffset;

				if (alpha < KOpaqueAlpha)
					{
					const TUint oldA = iAlphaConsumer->GetPixel(bufferOffset);
					if (oldA)
						{
						const TUint a2=(oldA*(0xffu-alpha)+0x80)>>8;
						
						TUint oldC=TUint(*ptr);
						*ptr++ = TUint8(( Blue(aPixelColour)*alpha + oldC*a2)>>8);
												
						oldC=TUint(*ptr);
						*ptr++ = TUint8(( Green(aPixelColour)*alpha + oldC*a2)>>8);

						oldC=*ptr;
						*ptr   = TUint8(( Red(aPixelColour)*alpha + a2*oldC )>>8 );

						iAlphaConsumer->SetPixelByBufferOffset(bufferOffset, alpha + a2 );
						}
					else
						{
						// "old" pixel alpha is fully transparent, so set the pixel and its alpha
						// in the first pass, it will be 0x00 so set both pixel and alpha
						iAlphaConsumer->SetPixelByBufferOffset(bufferOffset, alpha);
						SetRgbPixel(ptr, 	Red(aPixelColour), Green(aPixelColour), Blue(aPixelColour));
						}
					}
				else
					{
					// pixel alpha is opaque, so set the pixel and its alpha
					iAlphaConsumer->SetPixelByBufferOffset(bufferOffset, alpha);
					SetRgbPixel(ptr, 	Red(aPixelColour), Green(aPixelColour), Blue(aPixelColour));
					}
				}
			}
		else
			{
			// no Alpha blending just set the pixel
			TUint8* ptr=iFrameBuffer+KRgbPixelSize*(RealPos.iY*iFrameSize.iWidth+RealPos.iX);
			SetRgbPixel(ptr, 	Red(aPixelColour), Green(aPixelColour), Blue(aPixelColour));
			}
		}

	if (++iCurrentPos.iX >= iFrameSize.iWidth)
		{
		iCurrentPos.iX = 0;
		++iCurrentPos.iY;
		}
	}

void CRgbPixelConsumer::SetRGBAPixel(TRgbaColour aPixelColour)
	{
		const TPoint RealPos( iOrigin.iX + iCurrentPos.iX, iOrigin.iY + iCurrentPos.iY );
	if (Contains(iRealOutputRect, RealPos))
		{
		const TInt bufferOffset=RealPos.iY*iFrameSize.iWidth+RealPos.iX;
		TUint8* ptr=iFrameBuffer+KRgbaPixelSize*bufferOffset;
		
		if (NULL != iAlphaConsumer)
			{
// IF AlphaConsumer (if trans or alpha), then get alpha value
			const TUint alpha= (iAlphaConsumer->Enabled()? Alpha(aPixelColour) : KOpaqueAlpha );
// alpha blending is: 
//			Aresult * Cresult	= Afg*Cfg + Abg*Cbg - Afg*Abg*Cbg OR Afg*Cfg + Cbg * Abg*(1 - Afg)
//			Aresult				= Afg + (1 - Afg) * Abg
//			where 0 <= Alpha < 1
//			However we are using fixed point where
//				  0 <= Alpha <= 0xff

			if (alpha)
				{
				if (alpha < KOpaqueAlpha)
					{
					const TUint oldA = iAlphaConsumer->GetPixel(bufferOffset);
					if (oldA)
						{
						const TUint a2=(oldA*(0xffu-alpha)+0x80)>>8;
						
						TUint oldC=TUint(*ptr);
						*ptr++ = TUint8(( Blue(aPixelColour)*alpha + oldC*a2)>>8);
												
						oldC=TUint(*ptr);
						*ptr++ = TUint8(( Green(aPixelColour)*alpha + oldC*a2)>>8);

						oldC=*ptr;
						*ptr++   = TUint8(( Red(aPixelColour)*alpha + a2*oldC )>>8 );
						
						*ptr	= iAlphaEnabled ? TUint8(Alpha((alpha + a2) << KAlphaShift)) : KOpaqueAlpha;
						
						iAlphaConsumer->SetPixelByBufferOffset(bufferOffset, alpha + a2);
						}
					else
						{
						// "old" pixel alpha is fully transparent, so set the pixel and its alpha
						// in the first pass, it will be 0x00 so set both pixel and alpha
						iAlphaConsumer->SetPixelByBufferOffset(bufferOffset, alpha);
						SetRgbaPixel(ptr, 	Red(aPixelColour), Green(aPixelColour), Blue(aPixelColour), iAlphaEnabled ? Alpha((alpha) << KAlphaShift) : KOpaqueAlpha);
						}
					}
				else
					{
					iAlphaConsumer->SetPixelByBufferOffset(bufferOffset, alpha);
					SetRgbaPixel(ptr, 	Red(aPixelColour), Green(aPixelColour), Blue(aPixelColour), KOpaqueAlpha);
					}
				}
			}
		else // dealing with source with no transparency
			{
			// Just set the pixel to fully opaque
			SetRgbaPixel(ptr, Red(aPixelColour), Green(aPixelColour), Blue(aPixelColour), KOpaqueAlpha);
			}
		}

	if (++iCurrentPos.iX >= iFrameSize.iWidth)
		{
		iCurrentPos.iX = 0;
		++iCurrentPos.iY;
		}
	}

