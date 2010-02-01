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

#ifndef __MNGRENDERER_H__

#define __MNGRENDERER_H__
/** 	@file
	@internalTechnology */

#include <icl/imageprocessor.h>
#include "PixelConsumer.h"
#include "PixelFilter.h"

class CFbsBitmap;
class CImageProcessor;
class MPixelFilter;
class CRgbPixelConsumer;
class CAlphaPixelConsumer;

class CPixelProcessor: public CImageProcessor 
	{
public:
// from the CImageProcessor //
	TBool SetPixels(TRgb* aColorBuffer,TInt aBufferLength);
	TBool SetMonoPixel(TInt aGray256);
	TBool SetMonoPixelRun(TInt aGray256,TInt aCount);
	TBool SetMonoPixels(TUint32* aGray256Buffer,TInt aBufferLength);
	TBool SetMonoPixelBlock(TUint32* aGray256Buffer);

//Subframes with No Loops 
//virtual keyword added
    virtual void PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& aImageRect);  
	void PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& /*aImageRect*/,const TSize& /*aRgbBlockSize*/);

	void SetYPosIncrement(TInt aYInc);
	void SetLineRepeat(TInt aLineRepeat);
	void SetPixelPadding(TInt aNumberOfPixels);

	TBool SetPixelBlock(TRgb* aColorBuffer);
	TBool SetPos(const TPoint& aPosition);
	TBool FlushPixels();

	inline void SetPixelConsumer(CPixelConsumerBase* aSubFilter);
	inline void SetPixelFilter(MPixelFilter* aFilter);
	inline void SetPixelFunction(CRgbPixelConsumer::SetPixelFunc aSetPixelFunction);

protected:
	TInt			iPixelsToSkip;
	TInt			iPixelPadding;
	TInt			iYInc;
	TPoint			iPixelPosition;
	TSize			iCurrentImgSize;
	CPixelConsumerBase*	iPixelConsumer;
	MPixelFilter*	iPixelFilter;
	CRgbPixelConsumer::SetPixelFunc iSetPixelFunction; // function pointer to either SetRGBPixel or SetRGBAPixel
	};

class CMngRenderer;	//Subframes with No Loops

class CAlphaProcessor;
class CRgbProcessor: public CPixelProcessor 
	{
public:
//Subframes with No Loops
	CRgbProcessor(CMngRenderer* aMngRenderer); 
	static CRgbProcessor* NewL(CMngRenderer* aMngRenderer);  
	void PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& aImageRect);
//
	TBool SetPixelRun(TRgb aColor,TInt aCount);
	TBool SetPixel(TRgb aColor);
	TBool SetPixels(TRgb* aColorBuffer,TInt aBufferLength);

protected:	
	inline CRgbPixelConsumer* Consumer() const;
	inline TBool SetPixelImpl(TRgb aColor);
	inline TBool SetPixelImplWithPf(TRgb aColor);
	
//Subframes with No Loops
private:
    CMngRenderer*   iMngRenderer;
 	};

class CAlphaProcessor: public CPixelProcessor 
	{
public:
	static CAlphaProcessor* NewL();

	TBool SetPixelRun(TRgb aColor,TInt aCount);
	TBool SetPixel(TRgb aColor);

	void PrepareL(CFbsBitmap& /*aBitmap*/,const TRect& aImageRect);

protected:
	inline CAlphaPixelConsumer* Consumer() const;

	};


class CMngRenderer: public CBase 
	{
public:

	static CMngRenderer* NewL(const TSize& aSize, TBool aAlphaNeeded, TDisplayMode aDestDisplayMode, TDisplayMode aMaskDisplayMode);
	~CMngRenderer();

	void OnNewImageL();
	void SetBackground(const TRgb aBgColour, TBool aColourIsMandatory);
	inline TRgb BgColour() const;
	void ClearOutputRect();

	void Reset();

	void SetImageOrigin(const TPoint& aOrigin);
	void SetImageClipRect(const TRect& aClippingRect);
	void SetBgClipRect(const TRect& aClippingRect);

	inline CImageProcessor& ImageProcessor();
	inline CImageProcessor* MaskProcessor();
	
	inline void CurrentFrame(CFbsBitmap& aBitmap, const TRect& aSrcRect) const;
	inline void CurrentMask(CFbsBitmap& aBitmap, const TRect& aSrcRect) const;

	void  SetRgbFilter(MPixelFilter* aPixelFilter);
	inline void  SetOutputEnabled(TBool aEnabled);
	inline TBool OutputEnabled() const;
    
	//Subframes with No Loops
	void UpdateRectEmpty();
	inline const TRect& UpdatedRect() const;
	void UpdateRect(const TRect& aRect);
	inline const TPoint& ViewPortOrigin() const;
	inline const TRect& ClippingRect() const;
	//
	
protected:
	explicit CMngRenderer(const TSize& aSize, TBool aAlphaNeeded, TDisplayMode aDestDisplayMode, TDisplayMode aMaskDisplayMode);
	void ConstructL();

protected:
	const TBool			iAlphaNeeded;
	const TSize			iRenderSize;
	TRgbaColour			iBgColour;

	TPoint				iViewPortOrigin;
	TRect				iImageClipRect;	// Current clipping rect for images (i.e. by DEFI)
	TRect				iBgClipRect;	// Current clipping rect for layers (i.e. by FRAM)
	TRect				iClippingRect;	// Current effective clipping rect (intersection of above two)

	//Subframes with No Loops
	TRect				iUpdateRect;
	//

	TPoint				iPixelPosition;
	TSize				iCurrentImgSize;

	TBool				iOutputEnbaled;
	MPixelFilter*		iRgbFilter;
	CPixelProcessor*	iRgbProcessor;
	CPixelProcessor*	iAlphaProcessor;
	CRgbPixelConsumer*	iRgbConsumer;
	CAlphaPixelConsumer*iAlphaConsumer;
	TBool				iBgColourSet;
	TBool				iBgColourIsMandatory;
	TDisplayMode		iDestDisplayMode;
	TDisplayMode		iMaskDisplayMode;
	};


#include "Mngrenderer.inl"

#endif // ndef __MNGRENDERER_H__
