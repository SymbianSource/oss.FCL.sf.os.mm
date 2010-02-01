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

#ifndef MEDIACLIENTVIDEODISPLAYBODY_H
#define MEDIACLIENTVIDEODISPLAYBODY_H

#include <w32std.h>
#include <graphics/surface.h>
#include <graphics/surfaceconfiguration.h>
#include <mmf/common/mmfvideosurfacecustomcommands.h>
#include <videoplayer.h>
#include <mmf/plugin/mmfmediaclientextdisplayinterface.h>

class MMMFSurfaceEventHandler;

/**
@internalTechnology
@released

*/
NONSHARABLE_CLASS(CMediaClientVideoDisplayBody) : public CBase, public MExtDisplayConnectionProviderCallback
	{
private:
	NONSHARABLE_CLASS(TWindowData)
		{
	public:
		TWindowData() :
		   	iWindow(NULL)
			{
			}
		
		TWindowData(const RWindowBase* aWindow) :
			iWindow(const_cast<RWindowBase*>(aWindow))
			{
			}
				
		TWindowData(const RWindowBase* aWindow, const TRect& aClipRect, const TRect& aVideoExtent,
					TReal32 aScaleWidth, TReal32 aScaleHeight, TVideoRotation aRotation, TAutoScaleType aAutoScaleType,
					TInt aHorizPos, TInt aVertPos,RWindow* aWindow2) :
			iWindow(const_cast<RWindowBase*>(aWindow)),
			iClipRect(aClipRect),
			iVideoExtent(aVideoExtent),
			iScaleWidth(aScaleWidth),
			iScaleHeight(aScaleHeight),
			iRotation(aRotation),
			iAutoScaleType(aAutoScaleType),
			iHorizPos(aHorizPos),
			iVertPos(aVertPos),
			iWindow2(aWindow2)
			{
			}
		
		RWindowBase* iWindow;
		TSurfaceConfiguration iSurfaceConfig;
		TRect iClipRect;
		TRect iVideoExtent;
				
		TReal32 iScaleWidth;
		TReal32 iScaleHeight;
		TVideoRotation iRotation;
			
		TAutoScaleType iAutoScaleType;
		TInt iHorizPos;
		TInt iVertPos;
		
		// iWindow2 is set when using CVideoPlayerUtility2.  This is used to avoid 
		// casting iWindow from RWindowBase* to RWindow*.
		RWindow* iWindow2;
		

		static TBool CompareByWsHandle(const TInt* aWsHandle, const TWindowData& aWindowData)
			{
			TBool ret = EFalse;
			if (aWsHandle)
				{
				ret = (*aWsHandle == aWindowData.iWindow->WsHandle());
				}
			return ret;
			}
		};
public:

	static CMediaClientVideoDisplayBody* NewL(TInt aDisplayId);
	static CMediaClientVideoDisplayBody* NewL(TInt aDisplayId, const TSurfaceId& aSurfaceId, const TRect& aCropRect, TVideoAspectRatio aAspectRatio);
	
	~CMediaClientVideoDisplayBody();
	
	void AddDisplayL(MMMFSurfaceEventHandler& aEventHandler);
	void AddDisplayWindowL(const RWindowBase* aWindow, const TRect& aClipRect, const TRect& aCropRegion, const TRect& aVideoExtent,
							TReal32 aScaleWidth, TReal32 aScaleHeight, TVideoRotation aRotation,
							TAutoScaleType aAutoScaleType, TInt aHorizPos, TInt aVertPos, RWindow* aWindow2);
	void RemoveDisplay();
	TInt RemoveDisplayWindow(const RWindowBase& aWindow);
	
	TInt SurfaceCreated(const TSurfaceId& aSurfaceId, const TRect& aCropRect, TVideoAspectRatio aAspectRatio, const TRect& aCropRegion);
	TInt SurfaceParametersChanged(const TSurfaceId& aSurfaceId, const TRect& aCropRect, TVideoAspectRatio aAspectRatio);
	void RemoveSurface(TBool aControllerEvent);
	void RemoveBackgroundSurface(TBool aTriggerRedraw);
	TInt RedrawWindows(const TRect& aCropRegion);
		
	void SetAutoScaleL(const RWindowBase& aWindow, TAutoScaleType aScaleType, TInt aHorizPos, TInt aVertPos, const TRect& aCropRegion);
	void SetRotationL(const RWindowBase& aWindow, TVideoRotation aRotation, const TRect& aCropRegion);
	TVideoRotation RotationL(const RWindowBase& aWindow);
	void SetScaleFactorL(const RWindowBase& aWindow, TReal32 aWidthPercentage, TReal32 aHeightPercentage, const TRect& aCropRegion);
	void GetScaleFactorL(const RWindowBase& aWindow, TReal32& aWidthPercentage, TReal32& aHeightPercentage);

	void SetAutoScaleL(TAutoScaleType aScaleType, TInt aHorizPos, TInt aVertPos, const TRect& aCropRegion);
	void SetRotationL(TVideoRotation aRotation, const TRect& aCropRegion);
	void SetScaleFactorL(TReal32 aWidthPercentage, TReal32 aHeightPercentage, const TRect& aCropRegion);
	void SetWindowClipRectL(const RWindowBase& aWindow, const TRect& aWindowClipRect, const TRect& aCropRegion);
	void SetVideoExtentL(const RWindowBase& aWindow, const TRect& aVideoExtent, const TRect& aCropRegion);
	
	void SetExternalDisplaySwitchingL(TBool aControl);
	
private:
	
	CMediaClientVideoDisplayBody(TInt aDisplayId);
	CMediaClientVideoDisplayBody(TInt aDisplayId, const TSurfaceId& aSurfaceId, const TRect& aCropRect,	TVideoAspectRatio aAspectRatio);
	
	void ConstructL();
	
	CFbsBitGc::TGraphicsOrientation ConvertRotation(TVideoRotation aRotation);
	
	RArray<CMediaClientVideoDisplayBody::TWindowData>& Windows();
	TInt DisplayId() const;
	TBool IsUsed() const;
	static TInt CompareByDisplay(const TInt* aDisplayId, const CMediaClientVideoDisplayBody& aDisplay);
	static TInt Compare(const CMediaClientVideoDisplayBody& aLeft, const CMediaClientVideoDisplayBody& aRight);
	static CMediaClientVideoDisplayBody* FindDisplayWithWindowL(const RPointerArray<CMediaClientVideoDisplayBody>& aDisplays, const RWindowBase& aWindow);
	TBool IsSurfaceCreated() const;
	TInt SetBackgroundSurface(TWindowData& aWindowData, const TRect& aCropRegion);
	void CalculateExtentAndViewport(const TWindowData& aWindowData, TRect& aExtent, TRect& aViewport);
	TBool HasWindows() const;
	
    void CreateExtDisplayConnProvAndRemoveSurfaceL(TBool aRemoveBackgroundSurface);
    void RemoveExtDisplayConnProvAndRedrawL();
    void RemoveExtDisplayConnProv();
    void MedcpcExtDisplayCalculateExtentAndViewportL(TRect& aExtent, TRect& aViewport, TRect& aExternalDisplayRect);
    void MedcpcExtDisplayNotifyConnected(TBool aExtDisplayConnected);

private:

    MMMFSurfaceEventHandler* iEventHandler;
	const TInt iDisplayId;
	TSurfaceId iSurfaceId;
	TRect iCropRect;
	TVideoAspectRatio iAspectRatio;
	RArray<TWindowData> iWindows;
	
	TRect iCropRegion;
	TBool iClientRequestedExtDisplaySwitching;
	CExtDisplayConnectionProviderInterface* iExtDisplayConnectionProvider;
    TBool iExtDisplayConnected;
    TBool iExtDisplaySwitchingSupported;
	
	friend class CVideoPlayerUtility::CBody;	
	friend class CTestStepUnitMMFVidClient;
	};

#endif // MEDIACLIENTVIDEODISPLAYBODY_H

