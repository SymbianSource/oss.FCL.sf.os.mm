// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @publishedPartner 
 @released 
*/

#ifndef __IMAGE_PROCESSOR_PREVIEW_INTF__
#define __IMAGE_PROCESSOR_PREVIEW_INTF__

#include <e32base.h>
#include <imageprocessor/imageprocessorpreview.h> 

class CFbsBitmap;
class CImageFrame;

namespace ImageProcessor
	{
namespace Plugin
	{
	
/**
Plugin API for the preview.

@see TPreview
@publishedPartner
@released
*/	
class MPreview
	{
public:
	/**
	This must provide an implementation as defined by TPreview::SetOutputL(CFbsBitmap& aBitmap)
	
	@see TPreview::SetOutputL(CFbsBitmap& aBitmap)
	*/	
	virtual void SetOutputL(CFbsBitmap& aBitmap) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::SetOutputL(CImageFrame& aPixelBuffer)
	
	@see TPreview::SetOutputL(CImageFrame& aPixelBuffer)
	*/
	virtual void SetOutputL(CImageFrame& aPixelBuffer) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::CreateOutputL(const TSize &aFrameSize, const TFrameFormatBase &aFrameFormat, const TFrameLayoutBase &aFrameLayout)
	
	@see TPreview::CreateOutputL(const TSize &aFrameSize, const TFrameFormatBase &aFrameFormat, const TFrameLayoutBase &aFrameLayout)
	*/
	virtual CImageFrame* CreateOutputL(const TSize &aFrameSize, const TFrameFormatBase &aFrameFormat, const TFrameLayoutBase &aFrameLayout) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::InitializeL()
	
	@see TPreview::InitializeL()
	*/
	virtual void InitializeL() = 0; // async
	
	/**
	This must provide an implementation as defined by TPreview::UninitializeL()
	
	@see TPreview::UninitializeL()
	*/
	virtual void UninitializeL() = 0; // sync

	/**
	This must provide an implementation as defined by TPreview::RenderL()
	
	@see TPreview::RenderL()
	*/
	virtual void RenderL() = 0;

	/**
	This must provide an implementation as defined by TPreview::ResetL()
	
	@see TPreview::ResetL()
	*/
	virtual void ResetL() = 0;
	
	/**
	This must provide an implementation as defined by TPreview::Cancel()
	
	@see TPreview::Cancel()
	*/
	virtual void Cancel() = 0;

	/**
	This must provide an implementation as defined by TPreview::SetZoomL(TReal32 aZoom)
	
	@see TPreview::SetZoomL(TReal32 aZoom)
	*/
	virtual void SetZoomL(TReal32 aZoom) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::SetPanL(TReal32 aPanX, TReal32 aPanY)
	
	@see TPreview::SetPanL(TReal32 aPanX, TReal32 aPanY)
	*/
	virtual void SetPanL(TReal32 aPanX, TReal32 aPanY) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::SetPreviewId(TInt aPreviewId)
	
	@see TPreview::SetPreviewId(TInt aPreviewId)
	*/
	virtual void SetPreviewId(TInt aPreviewId) = 0;

	/**
	This must provide an implementation as defined by TPreview::GetZoomLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel)
	
	@see TPreview::GetZoomLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel)
	*/
	virtual void GetZoomLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::GetPanLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel)
	
	@see TPreview::GetPanLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel)
	*/
	virtual void GetPanLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel) = 0;
	
	/**
	This must provide an implementation as defined by TPreview::ZoomL()
	
	@see TPreview::ZoomL()
	*/
	virtual TReal32 ZoomL() const = 0;
	
	/**
	This must provide an implementation as defined by TPreview::PanL(TReal32& aPanX, TReal32& aPanY)
	
	@see TPreview::PanL(TReal32& aPanX, TReal32& aPanY)
	*/
	virtual void PanL(TReal32& aPanX, TReal32& aPanY) const = 0;
	
	/**
	This must provide an implementation as defined by TPreview::SizeL()
	
	@see TPreview::SizeL()
	*/
	virtual TSize SizeL() const = 0;
	
	/**
	This must provide an implementation as defined by TPreview::CanvasAreaL()
	
	@see TPreview::CanvasAreaL()
	*/
	virtual TRect CanvasAreaL() const = 0;
	
	/**
	This must provide an implementation as defined by TPreview::PreviewId()
	
	@see TPreview::PreviewId()
	*/
	virtual TInt PreviewId() const = 0;
	
	/**
	This must provide an implementation as defined by TPreview::PreviewToCurrentCoordL(const TPoint& aPreviewPoint, TPoint& aCurrentPoint)
	
	@see TPreview::PreviewToCurrentCoordL(const TPoint& aPreviewPoint, TPoint& aCurrentPoint)
	*/
	virtual void PreviewToCurrentCoordL(const TPoint& aPreviewPoint, TPoint& aCurrentPoint) const = 0;
	
	/**
	This must provide an implementation as defined by TPreview::CanvasToCurrentCoordL(const TPoint& aCanvasPoint, TPoint& aCurrentPoint)
	
	@see TPreview::CanvasToCurrentCoordL(const TPoint& aCanvasPoint, TPoint& aCurrentPoint)
	*/
	virtual void CanvasToCurrentCoordL(const TPoint& aCanvasPoint, TPoint& aCurrentPoint) const = 0;

	/**
	This must provide an implementation as defined by TPreview::Extension(TUid aExtension)
	
	@see TPreview::Extension(TUid aExtension)
	*/
	virtual TAny* Extension(TUid aExtension) const = 0;

private:
	TInt iReserved; // future proof
	};

	}
	}

#endif //__IMAGE_PROCESSOR_PREVIEW_INTF__
