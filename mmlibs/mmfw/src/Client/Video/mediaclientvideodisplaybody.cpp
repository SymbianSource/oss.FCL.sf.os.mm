// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mediaclientvideodisplaybody.h"
#include "mediaclientvideotrace.h"
#include <surfaceeventhandler.h>

CMediaClientVideoDisplayBody* CMediaClientVideoDisplayBody::NewL(TInt aDisplayId)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::NewL +++"));
	CMediaClientVideoDisplayBody* self = new (ELeave) CMediaClientVideoDisplayBody(aDisplayId);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::NewL ---"));
	return self;
	}

CMediaClientVideoDisplayBody* CMediaClientVideoDisplayBody::NewL(TInt aDisplayId, const TSurfaceId& aSurfaceId,
		const TRect& aCropRect, TVideoAspectRatio aAspectRatio)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::NewL +++"));
	if(aSurfaceId.IsNull())
		{
		User::Leave(KErrArgument);
		}
	CMediaClientVideoDisplayBody* self = new(ELeave) CMediaClientVideoDisplayBody(aDisplayId, aSurfaceId, aCropRect, aAspectRatio);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::NewL ---"));
	return self;
	}

void CMediaClientVideoDisplayBody::ConstructL()
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::ConstructL +++"));
	
	iExtDisplaySwitchingSupported = CExtDisplayConnectionProviderInterface::ExternalDisplaySupportedL();
	
	// Try and enable display switching by default. If this leaves then do so quietly.
	// Either the client has no scheduler installed or the device does not support external
	// switching (i.e. no plugin was found) 
	TRAPD(err, SetExternalDisplaySwitchingL(ETrue));
	err = err; // remove compile warning
	DEBUG_PRINT2(_L("CMediaClientVideoDisplayBody::ConstructL SetExternalDisplaySwitchingL returned with %d"), err);
	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::ConstructL ---"));
	}

CMediaClientVideoDisplayBody::CMediaClientVideoDisplayBody(TInt aDisplayId, const TSurfaceId& aSurfaceId,
		const TRect& aCropRect, TVideoAspectRatio aAspectRatio) :
	iDisplayId(aDisplayId),
	iSurfaceId(aSurfaceId),
	iCropRect(aCropRect),
	iAspectRatio(aAspectRatio)
	{	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CMediaClientVideoDisplayBody +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CMediaClientVideoDisplayBody ---"));
	}

CMediaClientVideoDisplayBody::CMediaClientVideoDisplayBody(TInt aDisplayId) :
	iDisplayId(aDisplayId)
	{	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CMediaClientVideoDisplayBody +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CMediaClientVideoDisplayBody ---"));
	}

CMediaClientVideoDisplayBody::~CMediaClientVideoDisplayBody()
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::~CMediaClientVideoDisplayBody +++"));

	RemoveBackgroundSurface(ETrue);
		
	iWindows.Close();	
	delete iExtDisplayConnectionProvider;
	REComSession::FinalClose();
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::~CMediaClientVideoDisplayBody ---"));
	}

void CMediaClientVideoDisplayBody::AddDisplayL(MMMFSurfaceEventHandler& aEventHandler)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::AddDisplayL +++"));
	if (iEventHandler != NULL)
		{
		User::Leave(KErrInUse);
		}
		
	iEventHandler = &aEventHandler;
	
	if (IsSurfaceCreated())
		{
		iEventHandler->MmsehSurfaceCreated(iDisplayId, iSurfaceId, iCropRect, iAspectRatio);
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::AddDisplayL ---"));
	}
	
void CMediaClientVideoDisplayBody::AddDisplayWindowL(const RWindowBase* aWindow, const TRect& aClipRect, const TRect& aCropRegion, const TRect& aVideoExtent, 
															TReal32 aScaleWidth, TReal32 aScaleHeight, TVideoRotation aRotation, 
															TAutoScaleType aAutoScaleType, TInt aHorizPos, TInt aVertPos, RWindow* aWindow2)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::AddDisplayWindowL +++"));
	TInt pos = iWindows.Find(aWindow->WsHandle(), TWindowData::CompareByWsHandle);
	
	if (pos != KErrNotFound)
		{
		User::Leave(KErrInUse);
		}
	
	TWindowData winData(aWindow, aClipRect, aVideoExtent, aScaleWidth, aScaleHeight, aRotation, aAutoScaleType, aHorizPos, aVertPos, aWindow2);
	iWindows.AppendL(winData);
	
	iCropRegion = aCropRegion;
	
	if (IsSurfaceCreated())
		{
        if(iExtDisplaySwitchingSupported && iClientRequestedExtDisplaySwitching)
            {
            CreateExtDisplayConnProvAndRemoveSurfaceL(EFalse);
            }
        
        if(!iExtDisplayConnected)
            {
            User::LeaveIfError(SetBackgroundSurface(winData, aCropRegion));
            }
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::AddDisplayWindowL ---"));
	}

void CMediaClientVideoDisplayBody::RemoveDisplay()
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveDisplay +++"));
	iEventHandler = NULL;
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveDisplay ---"));
	}	

TInt CMediaClientVideoDisplayBody::RemoveDisplayWindow(const RWindowBase& aWindow)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveDisplayWindow +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	
	if (pos >= 0)
		{
		if (IsSurfaceCreated())
			{
			iWindows[pos].iWindow->RemoveBackgroundSurface(ETrue);

			// Make sure all window rendering has completed before proceeding
			RWsSession* ws = iWindows[pos].iWindow->Session();
			if (ws)
				{
				ws->Finish();
				}
			}
		iWindows.Remove(pos);
		
		if(iWindows.Count() == 0)
		    {
		    RemoveExtDisplayConnProv();
		    }
		}
	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveDisplayWindow ---"));
	return pos;
	}


TInt CMediaClientVideoDisplayBody::SurfaceCreated(const TSurfaceId& aSurfaceId, const TRect& aCropRect, TVideoAspectRatio aAspectRatio, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SurfaceCreated +++"));
	TBool emitEvent = EFalse;
	if((iSurfaceId != aSurfaceId) && (!aSurfaceId.IsNull()))
		{
		emitEvent = ETrue;
		}

	iSurfaceId = aSurfaceId;
	iCropRect = aCropRect;
	iAspectRatio = aAspectRatio;
	iCropRegion = aCropRegion;
	
	if (emitEvent && iEventHandler)
		{
		iEventHandler->MmsehSurfaceCreated(iDisplayId, iSurfaceId, iCropRect, iAspectRatio);
		}

	TInt err = KErrNone;
    if(iExtDisplaySwitchingSupported && iClientRequestedExtDisplaySwitching)
        {
        TRAP(err, CreateExtDisplayConnProvAndRemoveSurfaceL(EFalse));
        }
    
    if(!iExtDisplayConnected)
        {
        err = RedrawWindows(aCropRegion);
        }
    
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SurfaceCreated ---"));
	return err;
	}

void CMediaClientVideoDisplayBody::RemoveBackgroundSurface(TBool aTriggerRedraw)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveBackgroundSurface +++"));
	if (IsSurfaceCreated())
		{
		TInt count = iWindows.Count();
	
		RWsSession* ws = NULL;
		for (TInt i = 0; i < count; ++i)
			{
			iWindows[i].iWindow->RemoveBackgroundSurface(aTriggerRedraw);
			// Make sure all window rendering has completed before proceeding
			ws = iWindows[i].iWindow->Session();
			if (ws)
				{
				ws->Finish();
				}
			}
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveBackgroundSurface ---"));
	}

void CMediaClientVideoDisplayBody::RemoveSurface(TBool aControllerEvent)
    {
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveSurface +++"));
    if (IsSurfaceCreated())
        {
        if(!iExtDisplayConnected)
            {
            RemoveBackgroundSurface(ETrue);
            }
        
        if (iEventHandler  && aControllerEvent)
            {
            iEventHandler->MmsehRemoveSurface(iSurfaceId);
            }

        iSurfaceId = TSurfaceId::CreateNullId();

        RemoveExtDisplayConnProv();
        }
    
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveSurface ---"));
    }

TInt CMediaClientVideoDisplayBody::SurfaceParametersChanged(const TSurfaceId& aSurfaceId, const TRect& aCropRect, TVideoAspectRatio aAspectRatio)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SurfaceParametersChanged +++"));
	if (!IsSurfaceCreated())
		{
		return KErrNotSupported;
		}

	if (iSurfaceId != aSurfaceId)
		{
		return KErrInUse;
		}
	
	iCropRect = aCropRect;
	iAspectRatio = aAspectRatio;
	
	if (iEventHandler)
		{
		iEventHandler->MmsehSurfaceParametersChanged(iSurfaceId, iCropRect, iAspectRatio);
		}

	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SurfaceParametersChanged ---"));
	return KErrNone;
	}
	
TInt CMediaClientVideoDisplayBody::RedrawWindows(const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RedrawWindows +++"));
	TInt err = KErrNone;	

	iCropRegion = aCropRegion;
	
	if(IsSurfaceCreated())
		{
		TInt count = iWindows.Count();
	
		for (TInt i = 0; i < count; ++i)
			{
			err = SetBackgroundSurface(iWindows[i], aCropRegion);
		
			if (err != KErrNone)
				{
				break;
				}
			}
		}
		
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RedrawWindows ---"));
	return err;
	}

void CMediaClientVideoDisplayBody::SetAutoScaleL(const RWindowBase& aWindow, TAutoScaleType aScaleType, TInt aHorizPos, TInt aVertPos, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetAutoScaleL +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	iWindows[pos].iAutoScaleType = aScaleType;
	iWindows[pos].iHorizPos = aHorizPos;
	iWindows[pos].iVertPos = aVertPos;
	iCropRegion = aCropRegion;
	
	if (IsSurfaceCreated() && !iExtDisplayConnected)
		{
		User::LeaveIfError(SetBackgroundSurface(iWindows[pos], aCropRegion));
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetAutoScaleL ---"));
	}
	

void CMediaClientVideoDisplayBody::SetRotationL(const RWindowBase& aWindow, TVideoRotation aRotation, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetRotationL +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	iWindows[pos].iRotation = aRotation;
	iCropRegion = aCropRegion;
	
    if (IsSurfaceCreated() && !iExtDisplayConnected)
		{
		User::LeaveIfError(SetBackgroundSurface(iWindows[pos], aCropRegion));
		}
	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetRotationL ---"));
	}

TVideoRotation CMediaClientVideoDisplayBody::RotationL(const RWindowBase& aWindow)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RotationL +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RotationL ---"));
	return iWindows[pos].iRotation;
	}

void CMediaClientVideoDisplayBody::SetScaleFactorL(const RWindowBase& aWindow, TReal32 aWidthPercentage, TReal32 aHeightPercentage, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetScaleFactorL +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	if (aWidthPercentage <= 0.0 || aHeightPercentage <= 0.0)
		{
		User::Leave(KErrArgument);
		}

	
	iWindows[pos].iScaleWidth = aWidthPercentage;
	iWindows[pos].iScaleHeight = aHeightPercentage;
	iWindows[pos].iAutoScaleType = EAutoScaleNone;
	iCropRegion = aCropRegion;
	
    if (IsSurfaceCreated() && !iExtDisplayConnected)
		{
		User::LeaveIfError(SetBackgroundSurface(iWindows[pos], aCropRegion));
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetScaleFactorL ---"));
	}
	
void CMediaClientVideoDisplayBody::GetScaleFactorL(const RWindowBase& aWindow, TReal32& aWidthPercentage, TReal32& aHeightPercentage)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::GetScaleFactorL +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	aWidthPercentage = iWindows[pos].iScaleWidth;
	aHeightPercentage = iWindows[pos].iScaleHeight;
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::GetScaleFactorL ---"));
	}

void CMediaClientVideoDisplayBody::SetAutoScaleL(TAutoScaleType aScaleType, TInt aHorizPos, TInt aVertPos, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetAutoScaleL +++"));
	
	iCropRegion = aCropRegion;
	TInt count = iWindows.Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iWindows[i].iAutoScaleType = aScaleType;
		iWindows[i].iHorizPos = aHorizPos;
		iWindows[i].iVertPos = aVertPos;
	    if (IsSurfaceCreated() && !iExtDisplayConnected)
			{
			User::LeaveIfError(SetBackgroundSurface(iWindows[i], aCropRegion));
			}
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetAutoScaleL ---"));
	}


void CMediaClientVideoDisplayBody::SetRotationL(TVideoRotation aRotation, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetRotationL +++"));
	iCropRegion = aCropRegion;
	TInt count = iWindows.Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iWindows[i].iRotation = aRotation;
	    if (IsSurfaceCreated() && !iExtDisplayConnected)
			{
			User::LeaveIfError(SetBackgroundSurface(iWindows[i], aCropRegion));
			}
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetRotationL ---"));
	}
		
void CMediaClientVideoDisplayBody::SetScaleFactorL(TReal32 aWidthPercentage, TReal32 aHeightPercentage, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetScaleFactorL +++"));
	if (aWidthPercentage <= 0.0 || aHeightPercentage <= 0.0)
		{
		User::Leave(KErrArgument);
		}
	
	iCropRegion = aCropRegion;
	TInt count = iWindows.Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		iWindows[i].iScaleWidth = aWidthPercentage;
		iWindows[i].iScaleHeight = aHeightPercentage;
		iWindows[i].iAutoScaleType = EAutoScaleNone;
	    if (IsSurfaceCreated() && !iExtDisplayConnected)
			{
			User::LeaveIfError(SetBackgroundSurface(iWindows[i], aCropRegion));
			}
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetScaleFactorL ---"));
	}

void CMediaClientVideoDisplayBody::SetWindowClipRectL(const RWindowBase& aWindow, const TRect& aWindowClipRect, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetWindowClipRectL +++"));
	
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	iWindows[pos].iClipRect = aWindowClipRect;
	iCropRegion = aCropRegion;
	
    if (IsSurfaceCreated() && !iExtDisplayConnected)
		{
		User::LeaveIfError(SetBackgroundSurface(iWindows[pos], aCropRegion));
		}
	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetWindowClipRectL ---"));
	}
	
void CMediaClientVideoDisplayBody::SetVideoExtentL(const RWindowBase& aWindow, const TRect& aVideoExtent, const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetVideoExtentL +++"));
	TInt pos = iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle);
	User::LeaveIfError(pos);
	
	iWindows[pos].iVideoExtent = aVideoExtent;
	iCropRegion = aCropRegion;
	
    if (IsSurfaceCreated() && !iExtDisplayConnected)
		{
		User::LeaveIfError(SetBackgroundSurface(iWindows[pos], aCropRegion));
		}
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetVideoExtentL ---"));
	}
	
TBool CMediaClientVideoDisplayBody::HasWindows() const
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::HasWindows +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::HasWindows ---"));
	return (iWindows.Count() > 0);
	}
	
TInt CMediaClientVideoDisplayBody::SetBackgroundSurface(TWindowData& aWindowData,  
									const TRect& aCropRegion)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetBackgroundSurface +++"));
	
	// required as this private function is called directly from external friend class
	iCropRegion = aCropRegion;

    // viewport is the viewable area of surface
    TRect viewport(iCropRect);
    if (aCropRegion.Width() > 0 || aCropRegion.Height() > 0)
        {
        viewport.Intersection(aCropRegion);
        }
    
    // Viewport is 0 size, don't show any video
    if (viewport.Width() <= 0 || viewport.Height() <= 0)
        {
        return KErrArgument;
        }

	TRect extent;
	CalculateExtentAndViewport(aWindowData, extent, viewport);
	
	aWindowData.iSurfaceConfig.SetViewport(viewport);
	aWindowData.iSurfaceConfig.SetExtent(extent);
	aWindowData.iSurfaceConfig.SetOrientation(ConvertRotation(aWindowData.iRotation));
	
	aWindowData.iSurfaceConfig.SetSurfaceId(iSurfaceId);
	
	// Get the rectangle that bounds the crop rectangle and the viewport.  This should be
	// the same as the crop rectangle as long as the viewport does not go outside this area.
	TRect rect(iCropRect);
	rect.BoundingRect(viewport);
	TInt err = KErrNone;
	
	// Check if the viewport and extent can be displayed as a background surface. The viewport 
	// is valid if it is within the crop rectangle and is not empty.  The extent is valid if 
	// it is not empty. 
	if (rect == iCropRect && !viewport.IsEmpty() && !extent.IsEmpty())
		{
		err = aWindowData.iWindow->SetBackgroundSurface(aWindowData.iSurfaceConfig, ETrue);
		}

	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetBackgroundSurface ---"));
	return err;
	}

void CMediaClientVideoDisplayBody::CalculateExtentAndViewport(const TWindowData& aWindowData, TRect& aExtent, TRect& aViewport)
    {
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CalculateExtentAndViewport +++"));

    TRect videoExtent(aWindowData.iVideoExtent);
    
    TReal32 inputWidth = 0.0f;
    TReal32 inputHeight = 0.0f;
    TReal32 pixelAspectRatio = 0.0f;
    switch (aWindowData.iRotation)
        {
        case EVideoRotationNone:
        case EVideoRotationClockwise180:
            inputWidth = static_cast<TReal32>(aViewport.Width());
            inputHeight = static_cast<TReal32>(aViewport.Height());
            pixelAspectRatio = static_cast<TReal32>(iAspectRatio.iNumerator) / iAspectRatio.iDenominator;
            break;
        case EVideoRotationClockwise90:
        case EVideoRotationClockwise270:
            inputWidth = static_cast<TReal32>(aViewport.Height());
            inputHeight = static_cast<TReal32>(aViewport.Width());
            pixelAspectRatio = static_cast<TReal32>(iAspectRatio.iDenominator) / iAspectRatio.iNumerator;
            break;
        default:
            // Should never get to default unless there's been some programming error.
            User::Invariant();
            break;
        }
        
    TReal32 viewportAspect = pixelAspectRatio * inputWidth / inputHeight;
    TReal32 vidextAspect = static_cast<TReal32>(videoExtent.Width()) / static_cast<TReal32>(videoExtent.Height());
    
    // Set the extent to the display area in the window.  The final height and with is to 
    // be changed by deltaHeight and deltaWidth respectively.
    aExtent = videoExtent;
    
    TInt deltaHeight = 0;
    TInt deltaWidth = 0;
    
    if (aWindowData.iAutoScaleType == EAutoScaleBestFit)
        {
        if (viewportAspect > vidextAspect)
            {
            // Shrink height to get the correct aspect ratio
            deltaHeight = (TInt) (aExtent.Width() / viewportAspect - aExtent.Height());
            }
        else
            {
            // Shrink width to get the correct aspect ratio
            deltaWidth = (TInt) (aExtent.Height() * viewportAspect - aExtent.Width());
            }
        }
    else if (aWindowData.iAutoScaleType == EAutoScaleClip)
        {
        if (viewportAspect > vidextAspect)
            {
            // Expand width to get the correct aspect ratio
            deltaWidth = (TInt) (aExtent.Height() * viewportAspect - aExtent.Width());
            }
        else
            {
            // Expand height to get the correct aspect ratio
            deltaHeight = (TInt) (aExtent.Width() / viewportAspect - aExtent.Height());
            }
        }
    else if (aWindowData.iAutoScaleType == EAutoScaleStretch)
        {
        // Don't do anything: the extent is already set to the size of the video extent.
        }
    else if (aWindowData.iAutoScaleType == EAutoScaleNone)
        {
        // for non-square pixels, reduce one dimension
        // TBD other option is to enlarge in the other dimension
        if(pixelAspectRatio > 1)
            {
            inputHeight /= pixelAspectRatio;
            }
        else if(pixelAspectRatio < 1)
            {
            inputWidth *= pixelAspectRatio;
            }
        deltaHeight = (TInt) (inputHeight * aWindowData.iScaleHeight * 0.01 - videoExtent.Height());
        deltaWidth = (TInt) (inputWidth * aWindowData.iScaleWidth * 0.01 - videoExtent.Width());
        }
    
    // Change the width of the extent in the proper directions and propertions.
    switch (aWindowData.iHorizPos)
        {
    case EHorizontalAlignCenter:
        aExtent.iTl.iX -= deltaWidth / 2;
        aExtent.iBr.iX += deltaWidth / 2;
        break;
    case EHorizontalAlignLeft:
        aExtent.iBr.iX += deltaWidth;
        break;
    case EHorizontalAlignRight:
        aExtent.iTl.iX -= deltaWidth;
        break;
    default:
        TInt width = aExtent.Width() + deltaWidth;
        aExtent.iTl.iX += aWindowData.iHorizPos;
        aExtent.iBr.iX = aExtent.iTl.iX + width;
        break;
        }
    
    // Change the height of the extent in the proper directions and propertions.
    switch (aWindowData.iVertPos)
        {
    case EVerticalAlignCenter:
        aExtent.iTl.iY -= deltaHeight / 2;
        aExtent.iBr.iY += deltaHeight / 2;
        break;
    case EVerticalAlignTop:
        aExtent.iBr.iY += deltaHeight;
        break;
    case EVerticalAlignBottom:
        aExtent.iTl.iY -= deltaHeight;
        break;
    default:
        TInt height = aExtent.Height() + deltaHeight;
        aExtent.iTl.iY += aWindowData.iVertPos;
        aExtent.iBr.iY = aExtent.iTl.iY + height;
        break;
        }
        
    // The video should not be displayed outside the intended video extent or clipping rectangle.  
    // The extent already has the correct size and position for displaying the entire viewport.  
    // The viewport is clipped such that the video is not moved/distorted when we take the extent 
    // to be the intersection of itself and the intended video extent.
    
    TRect viewableArea(videoExtent);
    viewableArea.Intersection(aWindowData.iClipRect);
    
    // Number of pixels (in window coordinates) to be clipped on the right, bottom, top and left sides of
    // the video.
    TInt dr = Max(0, aExtent.iBr.iX - viewableArea.iBr.iX);
    TInt db = Max(0, aExtent.iBr.iY - viewableArea.iBr.iY);
    TInt dt = Max(0, viewableArea.iTl.iY - aExtent.iTl.iY);
    TInt dl = Max(0, viewableArea.iTl.iX - aExtent.iTl.iX);
    
    // Calculate the number of pixels in the video per window pixel in both x and y directions.
    TReal32 wRatio = 0.0f; 
    TReal32 hRatio = 0.0f;
    
    // Make sure we don't divide by 0
    if (aExtent.Width() != 0)
        {
        wRatio = inputWidth / static_cast<TReal32>(aExtent.Width());
        }
    
    if (aExtent.Height() != 0)
        {
        hRatio = inputHeight / static_cast<TReal32>(aExtent.Height());
        }
    
    // Clip the viewport 
    switch (aWindowData.iRotation)
        {
        case EVideoRotationNone:
            aViewport.iBr.iX -= (TInt) (wRatio * static_cast<TReal32>(dr));
            aViewport.iBr.iY -= (TInt) (hRatio * static_cast<TReal32>(db));
            aViewport.iTl.iX += (TInt) (wRatio * static_cast<TReal32>(dl));
            aViewport.iTl.iY += (TInt) (hRatio * static_cast<TReal32>(dt));
            break;
        case EVideoRotationClockwise180:
            aViewport.iBr.iX -= (TInt) (wRatio * static_cast<TReal32>(dl));
            aViewport.iBr.iY -= (TInt) (hRatio * static_cast<TReal32>(dt));
            aViewport.iTl.iX += (TInt) (wRatio * static_cast<TReal32>(dr));
            aViewport.iTl.iY += (TInt) (hRatio * static_cast<TReal32>(db));
            break;
        case EVideoRotationClockwise90:
            aViewport.iBr.iX -= (TInt) (wRatio * static_cast<TReal32>(db));
            aViewport.iBr.iY -= (TInt) (hRatio * static_cast<TReal32>(dl));
            aViewport.iTl.iX += (TInt) (wRatio * static_cast<TReal32>(dt));
            aViewport.iTl.iY += (TInt) (hRatio * static_cast<TReal32>(dr));
            break;
        case EVideoRotationClockwise270:
            aViewport.iBr.iX -= (TInt) (wRatio * static_cast<TReal32>(dt));
            aViewport.iBr.iY -= (TInt) (hRatio * static_cast<TReal32>(dr));
            aViewport.iTl.iX += (TInt) (wRatio * static_cast<TReal32>(db));
            aViewport.iTl.iY += (TInt) (hRatio * static_cast<TReal32>(dl));
            break;
        default:
            // Should never get to default unless there's been some programming error.
            User::Invariant();
            break;
        }
    
    // Clip the extent.
    aExtent.Intersection(viewableArea);
    
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CalculateExtentAndViewport ---"));
    }

TBool CMediaClientVideoDisplayBody::IsUsed() const
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::IsUsed +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::IsUsed ---"));
	return (iEventHandler != NULL || iWindows.Count() > 0);
	}
	

TBool CMediaClientVideoDisplayBody::IsSurfaceCreated() const
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::IsSurfaceCreated +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::IsSurfaceCreated ---"));
	return !(iSurfaceId.IsNull());
	}
		
TInt CMediaClientVideoDisplayBody::DisplayId() const
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::DisplayId +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::DisplayId ---"));
	return iDisplayId;
	}
	
TInt CMediaClientVideoDisplayBody::CompareByDisplay(const TInt* aDisplayId, const CMediaClientVideoDisplayBody& aDisplay)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CompareByDisplay +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CompareByDisplay ---"));
	return (*aDisplayId - aDisplay.DisplayId());
	}
		
TInt CMediaClientVideoDisplayBody::Compare(const CMediaClientVideoDisplayBody& aLeft, const CMediaClientVideoDisplayBody& aRight)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::Compare +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::Compare ---"));
	return (aLeft.DisplayId() - aRight.DisplayId());
	}
	
CFbsBitGc::TGraphicsOrientation CMediaClientVideoDisplayBody::ConvertRotation(TVideoRotation aRotation)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::ConvertRotation +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::ConvertRotation ---"));
	switch(aRotation)
		{
		case EVideoRotationNone:
			return CFbsBitGc::EGraphicsOrientationNormal;
		case EVideoRotationClockwise90:
			return CFbsBitGc::EGraphicsOrientationRotated270;
		case EVideoRotationClockwise180:
			return CFbsBitGc::EGraphicsOrientationRotated180;
		case EVideoRotationClockwise270:
			return CFbsBitGc::EGraphicsOrientationRotated90;
		default:
			// Should never get to default unless there's been some programming error.
			User::Invariant();
			// This is never reached, just to keep the compiler happy
			return CFbsBitGc::EGraphicsOrientationNormal;
		}
	}

CMediaClientVideoDisplayBody* CMediaClientVideoDisplayBody::FindDisplayWithWindowL(const RPointerArray<CMediaClientVideoDisplayBody>& aDisplays, const RWindowBase& aWindow)
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::FindDisplayWithWindowL +++"));
	TInt count = aDisplays.Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		CMediaClientVideoDisplayBody* display = aDisplays[i];
	
		if (display->iWindows.Find(aWindow.WsHandle(), TWindowData::CompareByWsHandle) != KErrNotFound)
			{
			DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::FindDisplayWithWindowL ---"));
			return display;	
			}
		}
		
	User::Leave(KErrNotFound);
	return NULL;
	}
	
RArray<CMediaClientVideoDisplayBody::TWindowData>& CMediaClientVideoDisplayBody::Windows()
	{
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::Windows +++"));
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::Windows ---"));
	return iWindows;	
	}

void CMediaClientVideoDisplayBody::MedcpcExtDisplayCalculateExtentAndViewportL(TRect& aExtent, TRect& aViewport, TRect& aExternalDisplayRect)
    {
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::MedcpcExtDisplayCalculateExtentAndViewportL +++"));
    
    TWindowData windowData;
    windowData.iVideoExtent = aExternalDisplayRect;
    windowData.iRotation = EVideoRotationNone;
    windowData.iAutoScaleType = EAutoScaleBestFit;
    windowData.iHorizPos = EHorizontalAlignCenter;
    windowData.iVertPos = EVerticalAlignCenter;
    windowData.iClipRect = aExternalDisplayRect;
    
    aViewport = iCropRect;
    // Viewport is 0 size, don't show any video
    if (aViewport.Width() <= 0 || aViewport.Height() <= 0)
        {
        User::Leave(KErrArgument);
        }
    
    CalculateExtentAndViewport(windowData, aExtent, aViewport);
    
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::MedcpcExtDisplayCalculateExtentAndViewportL ---"));
    }

void CMediaClientVideoDisplayBody::CreateExtDisplayConnProvAndRemoveSurfaceL(TBool aRemoveBackgroundSurface)
    {
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CreateExtDisplayConnProvAndRemoveSurfaceL +++"));

    if((iExtDisplayConnectionProvider == NULL) && (iWindows.Count() > 0))
        {
        iExtDisplayConnectionProvider = CExtDisplayConnectionProviderInterface::NewL(*this, iSurfaceId);
        iExtDisplayConnected = iExtDisplayConnectionProvider->ExtDisplayConnectedL();
        
        if(iExtDisplayConnected && aRemoveBackgroundSurface)
            {
            RemoveBackgroundSurface(ETrue);
            }
        }

    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::CreateExtDisplayConnProvAndRemoveSurfaceL ---"));
    }
   
void CMediaClientVideoDisplayBody::RemoveExtDisplayConnProvAndRedrawL()
    {
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveExtDisplayConnProvAndRedrawL +++"));

    if(iExtDisplayConnectionProvider != NULL)
        {
        delete iExtDisplayConnectionProvider;
        REComSession::FinalClose();
        iExtDisplayConnectionProvider = NULL;
        iExtDisplayConnected = EFalse;
        User::LeaveIfError(RedrawWindows(iCropRegion));
        }

    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveExtDisplayConnProvAndRedrawL ---"));
    }

void CMediaClientVideoDisplayBody::RemoveExtDisplayConnProv()
    {
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveExtDisplayConnProv +++"));

    if(iExtDisplayConnectionProvider != NULL)
        {
        delete iExtDisplayConnectionProvider;
        REComSession::FinalClose();
        iExtDisplayConnectionProvider = NULL;
        iExtDisplayConnected = EFalse;
        }

    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::RemoveExtDisplayConnProv ---"));
    }

void CMediaClientVideoDisplayBody::SetExternalDisplaySwitchingL(TBool aControl)
    {
    DEBUG_PRINT2(_L("CMediaClientVideoDisplayBody::SetExternalDisplaySwitchingL +++ aControl=%d"), aControl);

    // not supported
    if(!iExtDisplaySwitchingSupported)
        {
        User::Leave(KErrNotSupported);
        }
    
    // need active scheduler installed
    if(CActiveScheduler::Current() == NULL)
        {
        User::Leave(KErrNotReady);
        }

    if(iClientRequestedExtDisplaySwitching != aControl)
        {
        iClientRequestedExtDisplaySwitching = aControl;
        
        if(IsSurfaceCreated())
            {
            if(iClientRequestedExtDisplaySwitching)
                {
                CreateExtDisplayConnProvAndRemoveSurfaceL(ETrue);
                }
            else
                {
                RemoveExtDisplayConnProvAndRedrawL();
                }
            }
        else
            {
            DEBUG_PRINT(_L("No Surface exists"));
            }
        }
    
    DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::SetExternalDisplaySwitchingL ---"));
    }

void CMediaClientVideoDisplayBody::MedcpcExtDisplayNotifyConnected(TBool aExtDisplayConnected)
	{
	DEBUG_PRINT2(_L("CMediaClientVideoDisplayBody::MedcpcExtDisplayNotifyConnected +++ aExtDisplayConnected=%d"), aExtDisplayConnected);
	
	if(iExtDisplayConnected != aExtDisplayConnected)
	    {
	    iExtDisplayConnected = aExtDisplayConnected;
	    if(iExtDisplayConnected)
	        {
	        RemoveBackgroundSurface(ETrue);
	        }
	    else
	        {
	        // ignore error - no need to tell provider - but lets log it in case it fails
	        TInt err = RedrawWindows(iCropRegion);
	        DEBUG_PRINT2(_L("RedrawWindows returned with %d"), err);
	        }
	    }
	else
	    {
	    DEBUG_PRINT(_L("No change in ext display connection status"));
	    }
	
	DEBUG_PRINT(_L("CMediaClientVideoDisplayBody::MedcpcExtDisplayNotifyConnected ---"));
	}
