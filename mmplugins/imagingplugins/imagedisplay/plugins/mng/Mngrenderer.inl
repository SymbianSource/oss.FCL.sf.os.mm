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
void CPixelProcessor::SetPixelConsumer(CPixelConsumerBase* aSubFilter)
	{
	iPixelConsumer = aSubFilter;
	}

inline
void CPixelProcessor::SetPixelFilter(MPixelFilter* aFilter)
	{
	iPixelFilter = aFilter;
	}

inline
void CPixelProcessor::SetPixelFunction(CRgbPixelConsumer::SetPixelFunc aSetPixelFunction)
	{
	iSetPixelFunction = aSetPixelFunction;
	}

inline 
CRgbPixelConsumer* CRgbProcessor::Consumer() const
	{
	return reinterpret_cast<CRgbPixelConsumer*>(iPixelConsumer);
	}

inline 
CAlphaPixelConsumer* CAlphaProcessor::Consumer() const
	{
	return reinterpret_cast<CAlphaPixelConsumer*>(iPixelConsumer);
	}

inline
void CMngRenderer::CurrentFrame(CFbsBitmap& aBitmap, const TRect& aSrcRect) const
	{
	iRgbConsumer->GetBitmap(aBitmap, aSrcRect);
	}

inline
void CMngRenderer::CurrentMask(CFbsBitmap& aBitmap, const TRect& aSrcRect) const
	{
	if (iAlphaConsumer)
		{
		iAlphaConsumer->GetBitmap(aBitmap, aSrcRect);
		}
	}

inline
TBool CMngRenderer::OutputEnabled() const
	{
	return iOutputEnbaled;
	}

inline
void CMngRenderer::SetOutputEnabled(TBool aEnabled)
	{
	iOutputEnbaled = aEnabled;
	}

//Subframes with No Loops	
inline
const TRect& CMngRenderer::UpdatedRect() const
	{
	return iUpdateRect;
	}
	
inline 
const TPoint& CMngRenderer::ViewPortOrigin() const
	{
	return iViewPortOrigin;
	}
	
inline 
const TRect& CMngRenderer::ClippingRect() const
	{
	return iClippingRect;	
	}
//

inline
CImageProcessor& CMngRenderer::ImageProcessor()
	{
	return *iRgbProcessor;
	}

inline
CImageProcessor* CMngRenderer::MaskProcessor()
	{
	return iAlphaProcessor;
	}

inline 
TRgb CMngRenderer::BgColour() const
	{
	TRgb col;
	col.SetInternal(iBgColour);
	return col;
	}
