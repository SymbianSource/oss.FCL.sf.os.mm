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

#include <ecom/ecom.h>
#include <mm/mmpluginutils.h>
#include <panorama/panoramauids.hrh>
#include <panorama/panorama.h>
#include <panorama/panoramaplugin.h>
#include "panoramaimpl.h"

_LIT8(KPluginMatchString, "*"); // ECom insists on something

//panorama
CImagePanoramaImpl* CImagePanoramaImpl::NewL()
	{
	CImagePanoramaImpl* self = new(ELeave) CImagePanoramaImpl();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
CImagePanoramaImpl::CImagePanoramaImpl()
	{
	}
	
void CImagePanoramaImpl::ConstructL()
	{
	TUid interfaceUid = {KUidPanoramaPluginInterface};
	iPlugin = static_cast<MImagePanoramaPlugin*> (MmPluginUtils::CreateImplementationL(interfaceUid, iDestructorKey, KPluginMatchString));			
	}
	
CImagePanoramaImpl::~CImagePanoramaImpl()
	{
	if (iPlugin)
		{
		iPlugin->Destroy();	
		}
	REComSession::DestroyedImplementation(iDestructorKey);
	}

void CImagePanoramaImpl::InitializeL(const TSize & aSize, const TPanoramaLens& aPanoramaLens, const TDirection& aDirection)
	{
	iPlugin->InitializeL(aSize, aPanoramaLens, aDirection);	
	}
	
void CImagePanoramaImpl::RenderL(const TDesC& aFilename)
	{
	iPlugin->RenderL(aFilename);
	}

void CImagePanoramaImpl::RenderL(const TDesC& aFilename, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid)
	{
	iPlugin->RenderL(aFilename, aSize, aMaintainAspectRatio, aImageTypeUid);
	}
	
void CImagePanoramaImpl::RenderL(RFile& aFile)
	{
	iPlugin->RenderL(aFile);
	}
	
void CImagePanoramaImpl::RenderL(RFile& aFile, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid)
	{
	iPlugin->RenderL(aFile, aSize,  aMaintainAspectRatio, aImageTypeUid);	
	}
	
void CImagePanoramaImpl::RenderL(HBufC8*& aDestData)
	{
	iPlugin->RenderL(aDestData);
	}

void CImagePanoramaImpl::RenderL(HBufC8*& aDestData, TUint32 aMaxBufferSize, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid)
	{
	iPlugin->RenderL(aDestData, aMaxBufferSize, aSize, aMaintainAspectRatio, aImageTypeUid);
	}

void CImagePanoramaImpl::RenderL(const CFbsBitmap& aBitmap)
	{
	iPlugin->RenderL(aBitmap);
	}
	
void CImagePanoramaImpl::RenderL(const CFbsBitmap& aBitmap, const TBool aMaintainAspectRatio)
	{
	iPlugin->RenderL(aBitmap, aMaintainAspectRatio);
	}
	
void CImagePanoramaImpl::RenderL(const CImageFrame& aImageFrame)
	{
	iPlugin->RenderL(aImageFrame);
	}
	
void CImagePanoramaImpl::RenderL(const CImageFrame& aImageFrame, const TBool aMaintainAspectRatio)
	{
	iPlugin->RenderL(aImageFrame, aMaintainAspectRatio);
	}
	
void CImagePanoramaImpl::CurrentImageSizeL(TSize& aSize)
	{
	iPlugin->CurrentImageSizeL(aSize);
	}
	
void CImagePanoramaImpl::AddFileL(const TDesC& aFilename, CPanoramaTransform& aPanTrans)
	{
	iPlugin->AddFileL(aFilename, aPanTrans);
	}
	
void CImagePanoramaImpl::AddBufferL(const TDesC8& aData, CPanoramaTransform& aPanTrans)
	{
	iPlugin->AddBufferL(aData, aPanTrans);
	}
	
void CImagePanoramaImpl::AddImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans)
	{
	iPlugin->AddImageL(aBitmap, aPanTrans);
	}
	
void CImagePanoramaImpl::AddImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans)
	{
	iPlugin->AddImageL(aImageFrame, aPanTrans);
	}
	
TAny* CImagePanoramaImpl::Extension(TUid aExtension)
	{
	return iPlugin->Extension(aExtension);
	}

//Panorama transform
CPanoramaTransformImpl* CPanoramaTransformImpl::NewL()
	{
	CPanoramaTransformImpl* self = new(ELeave) CPanoramaTransformImpl();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CPanoramaTransformImpl::~CPanoramaTransformImpl()
	{
	}
	
CPanoramaTransformImpl::CPanoramaTransformImpl()
	{	
	}
	
void CPanoramaTransformImpl::ConstructL()
	{
	}

void CPanoramaTransformImpl::GetTranslation(TPoint& aDxDy, TSize& aDims) const
	{
	aDxDy = iDxDy;
	aDims = iDims;
	}
	
void CPanoramaTransformImpl::SetTranslationL(const TPoint& aDxDy, const TSize& aDims)
	{
	iDxDy = aDxDy;
	iDims = aDims;
	}
	
void CPanoramaTransformImpl::GetDirection(TDirection& aDirection)
	{
	aDirection = iDirection;
	}
	
void CPanoramaTransformImpl::GetDirection(TDirection& aDirection, TInt& aMinimumDelta)
	{
	aDirection = iDirection;
	aMinimumDelta = iMinDelta;
	}
	
void CPanoramaTransformImpl::SetDirection(TDirection& aDirection)
	{
	iDirection = aDirection;	
	}
	
void CPanoramaTransformImpl::SetDirection(TDirection& aDirection, TInt aMinDelta)
	{
	iDirection = aDirection;
	iMinDelta = aMinDelta;	
	}
	
//VFTracker
CVFTrackerImpl* CVFTrackerImpl::NewL()
	{
	CVFTrackerImpl* self = new(ELeave) CVFTrackerImpl();
	CleanupStack::PushL(self);
	self->ConstructL();	
	CleanupStack::Pop();
	return self;
	}
	
CVFTrackerImpl::CVFTrackerImpl()
	{
	}
	
void CVFTrackerImpl::ConstructL()
	{
	TUid interfaceUid = {KUidVFTrackerPluginInterface};
	iPlugin = static_cast<MVFTrackerPlugin*> (MmPluginUtils::CreateImplementationL(interfaceUid, iDestructorKey, KPluginMatchString));	
	}
	
CVFTrackerImpl::~CVFTrackerImpl()
	{
	if (iPlugin)
		{
		iPlugin->Destroy();		
		}
	REComSession::DestroyedImplementation(iDestructorKey);
	}
	
void CVFTrackerImpl::InitializeL(const TSize& aSize)
	{
	iPlugin->InitializeL(aSize);	
	}
	
void CVFTrackerImpl::RegisterImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans)
	{
	iPlugin->RegisterImageL(aBitmap, aPanTrans);
	}
	
void CVFTrackerImpl::RegisterImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans)
	{
	iPlugin->RegisterImageL(aImageFrame, aPanTrans);
	}

TBool CVFTrackerImpl::IsTimeToCapture(TDirection& aDirection)
	{
	return iPlugin->IsTimeToCapture(aDirection);
	}
	
TBool CVFTrackerImpl::IsTimeToCapture(TDirection& aDirection, TInt aOverlap) const
	{
	return iPlugin->IsTimeToCapture(aDirection, aOverlap);
	}
	
void CVFTrackerImpl::Reset()
	{
	iPlugin->Reset();
	}
	
TAny* CVFTrackerImpl::Extension(TUid aExtension)
	{
	return iPlugin->Extension(aExtension);
	}
