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

#ifndef PANORAMAIMPL_H
#define PANORAMAIMPL_H

class MImagePanoramaPlugin;
class MVFTrackerPlugin;

/**
* Concrete implementation of CPanoramaTransform
*/
NONSHARABLE_CLASS(CPanoramaTransformImpl) : public CBase
	{
public:
	static CPanoramaTransformImpl* NewL();
	
	void GetTranslation(TPoint& aDxDy, TSize& aDims) const;
	void SetTranslationL(const TPoint& aDxDy, const TSize& aDims);
	void GetDirection(TDirection& aDirection);
	void GetDirection(TDirection& aDirection, TInt& aMinimumDelta);
	void SetDirection(TDirection& aDirection);
	void SetDirection(TDirection& aDirection, TInt aMinDelta);
	
	~CPanoramaTransformImpl();
	
private:
	CPanoramaTransformImpl();
	void ConstructL();
	
private:
	TPoint iDxDy;
	TSize iDims;
	TDirection iDirection;
	TInt iMinDelta;
	};

/**
* Concrete implementation of CImagePanorama
*/

NONSHARABLE_CLASS(CImagePanoramaImpl) : public CBase
	{
public:
	static CImagePanoramaImpl* NewL();
	
	void AddFileL(const TDesC& aFilename, CPanoramaTransform& aPanTrans);
	void AddBufferL(const TDesC8& aData, CPanoramaTransform& aPanTrans);
	void AddImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans);
	void AddImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans);

	void CurrentImageSizeL(TSize& aSize);

	void RenderL(const TDesC& aFilename); 
	void RenderL(const TDesC& aFilename, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid); 

	void RenderL(RFile& aFile); 
	void RenderL(RFile& aFile, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid);

	void RenderL(HBufC8*& aDestData);	
	void RenderL(HBufC8*& aDestData, TUint32 aMaxBufferSize, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid);
	
	void RenderL(const CFbsBitmap& aBitmap); 
	void RenderL(const CFbsBitmap& aBitmap, const TBool aMaintainAspectRatio);
	
	void RenderL(const CImageFrame& aImageFrame); 
	void RenderL(const CImageFrame& aImageFrame, const TBool aMaintainAspectRatio); 

	void InitializeL(const TSize & aSize, const TPanoramaLens& aPanoramaLens, const TDirection& aDirection);
	MImagePanoramaPlugin* GetPanoramaPlugin();

	TAny* Extension(TUid aExtension);
	
	~CImagePanoramaImpl();
	
private:
	CImagePanoramaImpl();
	void ConstructL();

private:
	MImagePanoramaPlugin* iPlugin;
	TUid iDestructorKey;
	};

/**
* Concrete implementation of CVTracker
*/	
NONSHARABLE_CLASS(CVFTrackerImpl) : public CBase
	{
public:
	static CVFTrackerImpl* NewL();
	
	void RegisterImageL(const CFbsBitmap& aBitmap,CPanoramaTransform& aPanTrans);
	void RegisterImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans);
	void Reset();
	
	TBool IsTimeToCapture(TDirection& aDirection, TInt aOverlap) const;
	TBool IsTimeToCapture(TDirection& aDirection);

	void InitializeL(const TSize& aSize);	
	MVFTrackerPlugin* GetVFTrackerPlugin();

	TAny* Extension(TUid aExtension);
	
	~CVFTrackerImpl();

private:
	CVFTrackerImpl();
	void ConstructL();

private:
	MVFTrackerPlugin* iPlugin;
	TUid iDestructorKey;
	};
	
#endif //PANORAMIMPL_H

