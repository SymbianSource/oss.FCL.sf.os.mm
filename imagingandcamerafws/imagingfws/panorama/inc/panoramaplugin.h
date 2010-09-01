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

#ifndef PANORAMAPLUGIN_H
#define PANORAMAPLUGIN_H

#include <panorama/panorama.h>

class CFbsBitmap;
class CImageFrame;

/**
Mixin class for implementation by providers of the VFTracker Plugin.

@see	CVFTracker

@publishedPartner
@released
*/	
class MVFTrackerPlugin
	{
public:
	virtual void RegisterImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans) = 0;
	virtual void RegisterImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans) = 0;
	virtual TBool IsTimeToCapture(TDirection& aDirection, TInt aOverlap = KPanoramaDefaultOverlap) const = 0;
	virtual void Reset() = 0;
	virtual void InitializeL(const TSize& aSize) = 0;	
	virtual TAny* Extension(TUid aExtension) = 0;
	virtual void Destroy() = 0;
	};

/**
Mixin class for implementation by providers of the panorama Plugin.

@see	CImagePanorama

@publishedPartner
@released
*/		
class MImagePanoramaPlugin
	{
public:
	virtual void CurrentImageSizeL(TSize& aSize) = 0;
	virtual void AddFileL(const TDesC& aFilename, CPanoramaTransform& aPanTrans) = 0;
	virtual void AddBufferL(const TDesC8& aData, CPanoramaTransform& aPanTrans) = 0;
	virtual void AddImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans) = 0;
	virtual void AddImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans) = 0;

	virtual void InitializeL(const TSize & aSize, const TPanoramaLens& aPanoramaLens, const TDirection& aDirection) = 0;

	virtual void RenderL(const TDesC& aFilename) = 0; 
	virtual void RenderL(const TDesC& aFilename, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid) = 0; 

	virtual void RenderL(RFile& aFile) = 0; 
	virtual void RenderL(RFile& aFile, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid) = 0;

	virtual void RenderL(HBufC8*& aDestData) = 0;
	virtual void RenderL(HBufC8*& aDestData, TUint32 aMaxBufferSize, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid) = 0;

	virtual void RenderL(const CFbsBitmap& aBitmap) = 0; 
	virtual void RenderL(const CFbsBitmap& aBitmap, const TBool aMaintainAspectRatio) = 0; 

	virtual void RenderL(const CImageFrame& aImageFrame) = 0; 
	virtual void RenderL(const CImageFrame& aImageFrame, const TBool aMaintainAspectRatio) = 0; 

	virtual TAny* Extension(TUid aExtension) = 0;
	virtual void Destroy() = 0;
	};
	

#endif //PANORAMAPLUGIN_H