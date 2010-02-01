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
#include "MngPanic.h"

#include "MngRenderer.h"

/** 
 the color will be used as a "default" to clear background if an image doesn't
 have one defined and there is no alpha channel
*/
const TUint KDefaultColor=0; 

/*static*/
CMngRenderer* CMngRenderer::NewL(const TSize& aSize, TBool aAlphaNeeded, TDisplayMode aDestDisplayMode, TDisplayMode aMaskDisplayMode)
	{
	CMngRenderer* self=new (ELeave) CMngRenderer(aSize, aAlphaNeeded, aDestDisplayMode, aMaskDisplayMode);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMngRenderer::CMngRenderer(const TSize& aSize, TBool aAlphaNeeded, TDisplayMode aDestDisplayMode, TDisplayMode aMaskDisplayMode)
 : iAlphaNeeded(aAlphaNeeded),
   iRenderSize(aSize),
   iDestDisplayMode(aDestDisplayMode),
   iMaskDisplayMode(aMaskDisplayMode)
	{
	}

void CMngRenderer::ConstructL()
	{
	TInt pixelSize;
	CRgbPixelConsumer::SetPixelFunc setPixelFunction;
	if(iDestDisplayMode == EColor16MA || iDestDisplayMode == EColor16MU)
		{
		pixelSize = KRgbaPixelSize;
		setPixelFunction = &CRgbPixelConsumer::SetRGBAPixel;
		}
	else
		{
		pixelSize = KRgbPixelSize;
		setPixelFunction = &CRgbPixelConsumer::SetRGBPixel;
		}
		
	iRgbConsumer = new (ELeave) CRgbPixelConsumer(pixelSize);
	iRgbConsumer->InitL(iRenderSize);
			
	if (iAlphaNeeded) // as required by source image
		{
		iAlphaConsumer= new (ELeave) CAlphaPixelConsumer();
		iAlphaConsumer->InitL(iRenderSize);
		
		if (iDestDisplayMode == EColor16MA && iMaskDisplayMode != EGray256)  // No mask
			{
			// In this case, there is a single 16MA destination, so we need to put the
			// alpha data into the alpha channel, rather than 0xFF.
			iRgbConsumer->SetAlphaEnabledBitmap(ETrue);
			}
		}
	iRgbConsumer->SetAlphaConsumer(iAlphaConsumer);
		
	iRgbProcessor=CRgbProcessor::NewL(this);
	iRgbProcessor->SetPixelConsumer(iRgbConsumer);
	iRgbProcessor->SetPixelFunction(setPixelFunction);
	
	CAlphaProcessor* Alpha= NULL;
	if (iAlphaNeeded)
		{
		Alpha = CAlphaProcessor::NewL();
		iAlphaProcessor = Alpha;
		Alpha->SetPixelConsumer(iAlphaConsumer);
		}
	}

CMngRenderer::~CMngRenderer()
	{
	delete iRgbProcessor;
	delete iAlphaProcessor;

	if (NULL != iRgbFilter)
		{
		iRgbFilter->Release();
		iRgbFilter = NULL;
		}
	delete iRgbConsumer;
	delete iAlphaConsumer;
	}

void CMngRenderer::Reset()
	{
	SetOutputEnabled(ETrue);
	iBgColourSet	= EFalse;
	iBgColourIsMandatory = EFalse;
	iBgColour		= KDefaultColor;
	const TPoint KZero(0,0);
	iViewPortOrigin = KZero;
	iClippingRect	= TRect(KZero, iRenderSize.AsPoint());
	iImageClipRect	= iClippingRect;
	iBgClipRect		= iClippingRect;

	// Subframes with No Loops
	// updated rect as empty 
   	UpdateRectEmpty();
	//

	iRgbConsumer->SetClippingRect(iClippingRect);
	iRgbConsumer->SetOrigin(KZero);

	ImageProcessor().SetPos(KZero);
	ImageProcessor().SetYPosIncrement(1);

	if (iAlphaConsumer)
		{
		iAlphaConsumer->SetClippingRect(iClippingRect);
		iAlphaConsumer->SetOrigin(KZero);
		}
	}

/*
Clears the current layer clipping rect
*/
void CMngRenderer::ClearOutputRect()
	{
   	UpdateRect(iBgClipRect);

	if (iBgColourSet && iBgColourIsMandatory)
		{
		iRgbConsumer->Clear( iBgColour, iBgClipRect );
		if (iAlphaConsumer)
			{
			iAlphaConsumer->Clear(KOpaqueAlpha, iBgClipRect);
			}
		}
	else
		{
		if (iAlphaConsumer)
			{
			iAlphaConsumer->Clear(KTransparentAlpha, iBgClipRect);
			if (iDestDisplayMode == EColor16MA && iMaskDisplayMode != EGray256)
				{
				iRgbConsumer->ClearAlpha(KTransparentAlpha, iBgClipRect);
				}
			}
		else
			{
			// no alpha, so clear by available color
			iRgbConsumer->Clear(iBgColour, iBgClipRect );
			}
		}
	}

void CMngRenderer::OnNewImageL()
	{
	if (iAlphaConsumer)
		{
		iAlphaConsumer->SetEnabled(EFalse);
		}
	}

void CMngRenderer::SetBackground(const TRgb aBgColour, TBool aColourIsMandatory)
	{
	iBgColour			= aBgColour.Internal();
	iBgColourIsMandatory= aColourIsMandatory;
	iBgColourSet= ETrue;
	}

void CMngRenderer::SetImageOrigin(const TPoint& aOrigin)
	{
	iViewPortOrigin = aOrigin;
	iRgbConsumer->SetOrigin(aOrigin);
	if (iAlphaConsumer)
		{
		iAlphaConsumer->SetOrigin(aOrigin);	
		}
	}

void CMngRenderer::SetImageClipRect(const TRect& aClippingRect)
	{
	iImageClipRect	= aClippingRect;
	iClippingRect	= aClippingRect;
	iClippingRect.Intersection(iBgClipRect);

	iRgbConsumer->SetClippingRect(iClippingRect);
	if (iAlphaConsumer)
		{
		iAlphaConsumer->SetClippingRect(iClippingRect);	
		}
	}

void CMngRenderer::SetBgClipRect(const TRect& aClippingRect)
	{
	iBgClipRect		= aClippingRect;
	iBgClipRect.Intersection(TRect(TPoint(0,0), iRenderSize.AsPoint()));
	iClippingRect	= iBgClipRect;
	iClippingRect.Intersection(iImageClipRect);

	iRgbConsumer->SetClippingRect(iClippingRect);
	if (iAlphaConsumer)
		{
		iAlphaConsumer->SetClippingRect(iClippingRect);	
		}
	}

void CMngRenderer::SetRgbFilter(MPixelFilter* aPixelFilter)
	{
	if (NULL != iRgbFilter)
		{
		iRgbFilter->Release();
		}
	if (NULL != aPixelFilter)
		{
		aPixelFilter->AddRef();
		}
	iRgbFilter = aPixelFilter;
	iRgbProcessor->SetPixelFilter(aPixelFilter);
	}

//Subframes with No Loops
void CMngRenderer::UpdateRect(const TRect& aRect) 
	{
	/** Top Left Minima */
	if(iUpdateRect.iTl.iX > aRect.iTl.iX )	
		{
		iUpdateRect.iTl.iX = aRect.iTl.iX;
		}
		
	if(iUpdateRect.iTl.iY > aRect.iTl.iY)
		{
		iUpdateRect.iTl.iY = aRect.iTl.iY;
		}
		
	/** Bottom Right Maxima */
	if(iUpdateRect.iBr.iX < aRect.iBr.iX )	
		{
		iUpdateRect.iBr.iX = aRect.iBr.iX;
		}
		
	if(iUpdateRect.iBr.iY < aRect.iBr.iY)
		{
		iUpdateRect.iBr.iY = aRect.iBr.iY;
		}
	}
	
void CMngRenderer::UpdateRectEmpty()
	{
	const TPoint KZero(0,0);
	iUpdateRect.iBr = KZero;
    iUpdateRect.iTl = iRenderSize.AsPoint();
	}
//

//  from the CImageProcessor //

TBool CPixelProcessor::SetPixelBlock(TRgb* /*aColorBuffer*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CPixelProcessor::SetPixels(TRgb* /*aColorBuffer*/,TInt /*aBufferLength*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CPixelProcessor::SetMonoPixel(TInt /*aGray256*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CPixelProcessor::SetMonoPixelRun(TInt /*aGray256*/,TInt /*aCount*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CPixelProcessor::SetMonoPixels(TUint32* /*aGray256Buffer*/,TInt /*aBufferLength*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CPixelProcessor::SetMonoPixelBlock(TUint32* /*aGray256Buffer*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CPixelProcessor::SetPos(const TPoint& aPosition)
	{
	iPixelPosition = aPosition;
	return ETrue;
	}

TBool CPixelProcessor::FlushPixels()
	{
	if (NULL != iPixelFilter)
		{
		iPixelFilter->Commit();
		}
	return ETrue;
	}

void CPixelProcessor::PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& aImageRect)
	{
	iCurrentImgSize = aImageRect.Size();
	iPixelsToSkip	= iPixelPadding;
	iYInc			= 1;
	iPixelPosition.SetXY(0,0);
	if (NULL != iPixelConsumer)
		{
		iPixelConsumer->Prepare();	
		}
	if (NULL != iPixelFilter)
		{
		iPixelFilter->PrepareL(iCurrentImgSize, static_cast<CRgbPixelConsumer*>(iPixelConsumer) ); 
		}
	}

void CPixelProcessor::PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& /*aImageRect*/,const TSize& /*aRgbBlockSize*/)
	{
	ASSERT(EFalse);
	}

void CPixelProcessor::SetYPosIncrement(TInt aYInc)
	{
	iYInc = aYInc;
	}

void CPixelProcessor::SetLineRepeat(TInt /*aLineRepeat*/)
	{
	// we don't use progressive decoding
	}

void CPixelProcessor::SetPixelPadding(TInt aNumberOfPixels)
	{
	iPixelPadding = aNumberOfPixels; 
	}

/*static*/
CRgbProcessor* CRgbProcessor::NewL(CMngRenderer* aMngRenderer) 
	{
	return new (ELeave) CRgbProcessor(aMngRenderer); 
	}
	
CRgbProcessor::CRgbProcessor(CMngRenderer* aMngRenderer):iMngRenderer(aMngRenderer)	
	{
	}


TBool CRgbProcessor::SetPixels(TRgb* aColorBuffer,TInt aBufferLength)
	{
	ASSERT(aBufferLength>0);
	if (aBufferLength > iPixelsToSkip)
		{	
		aBufferLength -= iPixelsToSkip;
		iPixelsToSkip = 0;
		if (iPixelFilter==NULL)
			{		
			do 
				{
				SetPixelImpl(*aColorBuffer++);
				} while (--aBufferLength > 0);
			}
		else
			{
			do
				{
				SetPixelImplWithPf(*aColorBuffer++);
				} while (--aBufferLength > 0);
			}
		}
	else
		{
		iPixelsToSkip -= aBufferLength;
		}		
	return ETrue;
	}

TBool CRgbProcessor::SetPixelRun(TRgb aColor,TInt aCount)
	{
	if (aCount > iPixelsToSkip)
		{
		aCount -= iPixelsToSkip;
		iPixelsToSkip = 0;
		if (iPixelFilter==NULL)
			{
			do
				{
				SetPixelImpl(aColor);
				} while (--aCount);
			}
		else
			{
			do
				{
				SetPixelImplWithPf(aColor);
				} while (--aCount);
			}
		}
	else
		{
		iPixelsToSkip -= aCount;
		}
	return ETrue;
	}

//Subframes with No Loops
//added here
void CRgbProcessor::PrepareL(CFbsBitmap& aBitmap,const TRect& aImageRect)  
	{
	//updation with this new image. new image rect is defined by OriginSet(iViewPortOrigin) and OriginSet+Bottom right calc from aImageRect size
	TSize imageSize(aImageRect.Width(), aImageRect.Height());
	TRect imageRect(iMngRenderer->ViewPortOrigin(),imageSize);
	imageRect.Intersection(iMngRenderer->ClippingRect()); //Bug fix
	iMngRenderer->UpdateRect(imageRect);  

	CPixelProcessor::PrepareL(aBitmap, aImageRect);
	}
//

TBool CRgbProcessor::SetPixel(TRgb aColor)
	{
	if (iPixelsToSkip)
		{
		--iPixelsToSkip;
		return ETrue;
		}
	return iPixelFilter==NULL? SetPixelImpl(aColor) : SetPixelImplWithPf(aColor);
	}
		
inline 
TBool CRgbProcessor::SetPixelImplWithPf(TRgb aColor)
	{
	iPixelFilter->SetPixel(iPixelPosition, aColor.Internal());

	if (++iPixelPosition.iX == iCurrentImgSize.iWidth)
		{
		iPixelPosition.iX	= 0;
		iPixelPosition.iY  += iYInc;
		iPixelsToSkip		= iPixelPadding;
		}	
	return ETrue;
	}
	
inline 
TBool CRgbProcessor::SetPixelImpl(TRgb aColor)
	{
	iPixelConsumer->SetPos(iPixelPosition);
	CALL_SETPIXEL_FUNCTION(*Consumer(), iSetPixelFunction)(TRgbaColour(aColor.Internal()));

	if (++iPixelPosition.iX == iCurrentImgSize.iWidth)
		{
		iPixelPosition.iX	= 0;
		iPixelPosition.iY  += iYInc;
		iPixelsToSkip		= iPixelPadding;
		}

	return ETrue;
	}
	

/*static*/
CAlphaProcessor* CAlphaProcessor::NewL()
	{
	return new (ELeave) CAlphaProcessor();
	}

void CAlphaProcessor::PrepareL(CFbsBitmap& aBitmap, const TRect& aImageRect)
	{
	CPixelProcessor::PrepareL(aBitmap, aImageRect);
	if (Consumer())
		{
		Consumer()->SetEnabled(ETrue);	
		}
	}

TBool CAlphaProcessor::SetPixelRun(TRgb /*aColor*/,TInt /*aCount*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

TBool CAlphaProcessor::SetPixel(TRgb /*aColor*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}

