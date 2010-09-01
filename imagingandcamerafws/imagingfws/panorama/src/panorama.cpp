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

#include <e32base.h>
#include <panorama/panorama.h>
#include "panoramaimpl.h"
#include <fbs.h>

/**
Constructs a new image panorama object.

A leave occurs if there is insufficient memory available.

@return A pointer to the new image panorama object.
*/
EXPORT_C CImagePanorama* CImagePanorama::NewL()
	{
	CImagePanorama* self = new(ELeave) CImagePanorama();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
CImagePanorama::CImagePanorama()
	{
	}
	
void CImagePanorama::ConstructL()
	{
	iImgPanImpl = CImagePanoramaImpl::NewL();
	}
	
EXPORT_C CImagePanorama::~CImagePanorama()
	{
	delete iImgPanImpl;
	}

/**
Initialises the size, direction and panoramalens on plugin and creates the CCapsPanorama,
CCapsPanoramaTransform objects.
@param  aSize
        Size of the image.
@param  aPanoramaLens
        Panorama lens which is specific to camera
@param  aDirection
        Direction of the panorama
*/	
EXPORT_C void CImagePanorama::InitializeL(const TSize & aSize, const TPanoramaLens& aPanoramaLens, const TDirection& aDirection)
	{
	iImgPanImpl->InitializeL(aSize, aPanoramaLens, aDirection);	
	}

/**
Renders the currest session image to file.
@param aFilename
	   Filename specifying where to store the output file.
*/
EXPORT_C void CImagePanorama::RenderL(const TDesC& aFilename)
	{
	iImgPanImpl->RenderL(aFilename);  	
	}
	
/**
Renders the current session image to file.
@param aFilename
	    Filename specifying where to store the output file.
@param aSize
	   Dimensions of the output image.
@param aMaintainAspectRatio
	   Aspect ratio based on which output options will be defined
@param aImageTypeUid
	   The format Uid in which to encode the output image
*/
EXPORT_C void CImagePanorama::RenderL(const TDesC& aFilename, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid)
	{
	iImgPanImpl->RenderL(aFilename, aSize, aMaintainAspectRatio, aImageTypeUid);  	
	}

/**
Renders the current session image to an open file handle.
@param aFile
	   A file handler.
*/
EXPORT_C void CImagePanorama::RenderL(RFile& aFile)
	{
	iImgPanImpl->RenderL(aFile);	
	}

/**
Renders the current session image to an open file handle.
@param aFile
	   A file handler
@param aSize
	   Dimensions of the output image.
@param aMaintainAspectRatio
	   Aspect ratio based on which output options will be defined.
@param aImageTypeUid
	   The format Uid in which to encode the output image.
*/	
EXPORT_C void CImagePanorama::RenderL(RFile& aFile, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid)
	{
	iImgPanImpl->RenderL(aFile, aSize, aMaintainAspectRatio, aImageTypeUid);	
	}
	
/**
Renders the current session image encoded to a buffer.
@param aDestData
       Pointer to a buffer which will recieve the encoded image data.
*/
EXPORT_C void CImagePanorama::RenderL(HBufC8*& aDestData)
	{
	iImgPanImpl->RenderL(aDestData);  	
	}

/**
Renders the current session image encoded to a buffer.
@param aDestData
       Pointer to a buffer which will recieve the encoded image data.
@param aMaxBufferSize
	   The maximum size of the buffer in bytes.
@param aSize
	   Dimensions of the output image.
@param aMaintainAspectRatio
	   Aspect ratio based on which output options will be defined.
@param aImageTypeUid
	   The format Uid in which to encode the output image.
*/	
EXPORT_C void CImagePanorama::RenderL(HBufC8*& aDestData, TUint32 aMaxBufferSize, const TSize& aSize, const TBool aMaintainAspectRatio, const TUid& aImageTypeUid)
	{
	iImgPanImpl->RenderL(aDestData, aMaxBufferSize, aSize, aMaintainAspectRatio, aImageTypeUid);  	
	}

/**
Renders the output to a buffer in raw format.
@param aBitmap
	   A bitmap to get the output image.
*/
EXPORT_C void CImagePanorama::RenderL(const CFbsBitmap& aBitmap)
	{
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	    {
	    User::Leave(KErrNotSupported);
	    }
	iImgPanImpl->RenderL(aBitmap);
	}

/**
Renders the output to a buffer in raw format.
@param aBitmap
	   A bitmap to get the output image.
@param aMaintainAspectRatio
	   Aspect ration based on which output options will be defined.
*/	
EXPORT_C void CImagePanorama::RenderL(const CFbsBitmap& aBitmap, const TBool aMaintainAspectRatio)
	{
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	        {
	        User::Leave(KErrNotSupported);
	        }
	
	iImgPanImpl->RenderL(aBitmap, aMaintainAspectRatio); 	
	}

/**
Renders the output to a buffer in raw format.
@param aImageFrame
	   A imageframe object to get the output image.
*/
EXPORT_C void CImagePanorama::RenderL(const CImageFrame& aImageFrame)
	{
	iImgPanImpl->RenderL(aImageFrame);	
	}
	
/**
Renders the output to a buffer in raw format.
@param aImageFrame
	   A imageframe object to get the output image.
@param aMaintainAspectRatio
	   Aspect ration based on which output options will be defined.
*/	
EXPORT_C void CImagePanorama::RenderL(const CImageFrame& aImageFrame, const TBool aMaintainAspectRatio)
	{
	iImgPanImpl->RenderL(aImageFrame, aMaintainAspectRatio);	
	}
	
/**
Retrieves the dimensions of the current session image.
@param aSize
	   Dimensions of the image.
*/
EXPORT_C void CImagePanorama::CurrentImageSizeL(TSize& aSize)
	{
	iImgPanImpl->CurrentImageSizeL(aSize);	
	}
	
/**
Add an image to the panorama creation
@param aFilename
	   Input filename
@param aPanTrans
	   Panorama transform object
*/
EXPORT_C void CImagePanorama::AddFileL(const TDesC& aFilename, CPanoramaTransform& aPanTrans)
	{
	iImgPanImpl->AddFileL(aFilename, aPanTrans);	
	}

/**
Add an image to the panorama creation
@param aData
	   The buffer containing the image data
@param aPanTrans
	   Panorama transform object
*/	
EXPORT_C void CImagePanorama::AddBufferL(const TDesC8& aData, CPanoramaTransform& aPanTrans)
	{
	iImgPanImpl->AddBufferL(aData, aPanTrans);	
	}

/**
Add an image to the panorama creation in progress
@param aBitmap
	   A bitmap that will contain the image
@param aPanTrans
	   Panorama transform object
*/	
EXPORT_C void CImagePanorama::AddImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans)
	{
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	        {
	        User::Leave(KErrNotSupported);
	        }
	
	iImgPanImpl->AddImageL(aBitmap, aPanTrans);	
	}

/**
Add an image to the panorama creation in progress
@param aImageFrame
	   A imageframe object.
@param aPanTrans
	   Panorama transform object.
*/	
EXPORT_C void CImagePanorama::AddImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans)
	{
	iImgPanImpl->AddImageL(aImageFrame, aPanTrans);	
	}

/**
Retrieves a non-standard extension on the image panorama.

@param	aExtension
		The UID of the extension to be retrieved

@return	Extension corresponding to the UID given as a parameter.
*/
EXPORT_C TAny* CImagePanorama::Extension(TUid aExtension)
	{
	return iImgPanImpl->Extension(aExtension);
	}

/**
Constructs a new panorama transform object.

A leave occurs if there is insufficient memory available.

@return A pointer to the new panorama transform object.
*/
EXPORT_C CPanoramaTransform* CPanoramaTransform::NewL()
	{
	CPanoramaTransform* self = new(ELeave) CPanoramaTransform();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;		
	}

CPanoramaTransform::CPanoramaTransform()
	{	
	}
	
void CPanoramaTransform::ConstructL()
	{
	iPanTransImpl = CPanoramaTransformImpl::NewL();	
	}

EXPORT_C CPanoramaTransform::~CPanoramaTransform()
	{
	delete iPanTransImpl;
	}
	
/**
Get the translation part of a PanoramaTransform
@param aDxDy
	   x and y coordinates
@param aDims
	   Image dimensions for the transform
*/	
EXPORT_C void CPanoramaTransform::GetTranslation(TPoint& aDxDy, TSize& aDims) const
	{
	iPanTransImpl->GetTranslation(aDxDy, aDims);	
	}
	
/**
Set the desired translation values on a panorama transform object.
@param aDxDy
	   x and y coordinates
@param aDims
	   Image dimenstion for the transform
*/
EXPORT_C void CPanoramaTransform::SetTranslationL(const TPoint& aDxDy, const TSize& aDims)
	{
	iPanTransImpl->SetTranslationL(aDxDy, aDims);	
	}

/**
Get the direction of a panorama transform
@param aDirection
	   Direction of the panorama
*/	
EXPORT_C void CPanoramaTransform::GetDirection(TDirection& aDirection)
	{
	return iPanTransImpl->GetDirection(aDirection);	
	}

/**
Get the direction of a panorama transform
@param aDirection
	   Direction of the panorama
@param aMinimumDelta
	   Minimum translation in percent.
*/	
EXPORT_C void CPanoramaTransform::GetDirection(TDirection& aDirection, TInt& aMinimumDelta)
	{
	return iPanTransImpl->GetDirection(aDirection, aMinimumDelta);	
	}

/**
Set the desired direction of a panorama transform
@param aDirection
	   Direction of the panorama
*/	
EXPORT_C void CPanoramaTransform::SetDirection(TDirection& aDirection)
	{
	iPanTransImpl->SetDirection(aDirection);	
	}

/**
Set the desired direction of a panorama transform
@param aDirection
	   Direction of the panorama
@param aMinDelta
	   Minimum translation in percent.
*/
EXPORT_C void CPanoramaTransform::SetDirection(TDirection& aDirection, TInt aMinDelta)
	{
	iPanTransImpl->SetDirection(aDirection, aMinDelta);	
	}

/**
Constructs a new VFTracker object.

A leave occurs if there is insufficient memory available.

@return A pointer to a new VFTracker object.
*/	
EXPORT_C CVFTracker* CVFTracker::NewL()
	{
	CVFTracker* self = new(ELeave) CVFTracker();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
CVFTracker::CVFTracker()
	{
	}
	
void CVFTracker::ConstructL()
	{
	iVFTrackImpl = CVFTrackerImpl::NewL();
	}
	
EXPORT_C CVFTracker::~CVFTracker()
	{
	delete iVFTrackImpl;
	}
	
/**
Initialises the dimensions of the image on the plugin and creates the VFTracker object.
@param aSize
	   Dimensions of the image.	
*/
EXPORT_C void CVFTracker::InitializeL(const TSize & aSize)	
	{
	iVFTrackImpl->InitializeL(aSize);	
	}

/**
Performs overlapping calculations from viewfinder image.
Pass all viewfinder images to this function to find out when its time to capture a new image.
@param aBitmap
	   A bitmap that needs to be registered
@param aPanTrans
	   A panorama transform object.
*/
EXPORT_C void CVFTracker::RegisterImageL(const CFbsBitmap& aBitmap, CPanoramaTransform& aPanTrans)
	{
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	        {
	        User::Leave(KErrNotSupported);
	        }
	
	iVFTrackImpl->RegisterImageL(aBitmap, aPanTrans);	
	}

/**
Performs overlapping calculations from viewfinder image.
Pass all viewfinder images to this function to find out when its time to capture a new image.
@param aImageFrame
	   A imageframe object.
@param aPanTrans
	   A panorama transform object.
*/	
EXPORT_C void CVFTracker::RegisterImageL(const CImageFrame& aImageFrame, CPanoramaTransform& aPanTrans)
	{
	iVFTrackImpl->RegisterImageL(aImageFrame, aPanTrans);	
	}

/**
Determines from a PanoramaTransform if it is time to capture a new image.
@param aDirection
	   The type of panorama being created.
*/
EXPORT_C TBool CVFTracker::IsTimeToCapture(TDirection& aDirection) const
	{
	return iVFTrackImpl->IsTimeToCapture(aDirection);	
	}

/**
Determines from a PanoramaTransform if it is time to capture a new image.
@param aDirection
	   The type of panorama being created.
@param aOverlap
	   Desired overlap in the panorama image.
*/	
EXPORT_C TBool CVFTracker::IsTimeToCapture(TDirection& aDirection, TInt aOverlap) const
	{
	return iVFTrackImpl->IsTimeToCapture(aDirection, aOverlap);	
	}

/**
Resets the transform return for the next RegisterImageL().
*/	
EXPORT_C void CVFTracker::Reset()
	{
	iVFTrackImpl->Reset();	
	}

/**
Retrieves a non-standard extension on the VFTracker

@param	aExtension
		The UID of the extension to be retrieved

@return	Extension corresponding to the UID given as a parameter.
*/
EXPORT_C TAny* CVFTracker::Extension(TUid aExtension)
	{
	return iVFTrackImpl->Extension(aExtension);	
	}

/**
Creates a TPanoramaLens object

@param  aFocalLength
		Focal length coordinates, this is an array of size KPanLensIndex.
		
@param  aPrincipalPoint
		Principal point coordinates, this is an array of size KPanLensIndex.

@param  aSkew
		Skew coefficient. @remark 0.0f means no skew

@param  aDist
		Distortion coefficients, this is an array of size KPanLensDist.

@param  aWidth
		Image width when the camera was calibrated.

@param  aHeight
		Image height when the camera was calibrated.
*/
EXPORT_C TPanoramaLens::TPanoramaLens(TReal32* aFocalLength, TReal32* aPrincipalPoint, TReal32 aSkew, TReal32* aDist, TUint32 aWidth, TUint32 aHeight)
	{
	TInt i = 0;
	for (i = 0; i < KPanLensIndex; i++)
		{
		iFocalLength[i]	= aFocalLength[i];
		iPrincipalPoint[i] = aPrincipalPoint[i];
		}
	iSkew = aSkew;
	for (i = 0; i < KPanLensDist; i++)
		{
		iDist[i] = aDist[i];
		}
	iWidth = aWidth;
	iHeight = aHeight;
	}
	
EXPORT_C TPanoramaLens::TPanoramaLens()
	{
	// Empirical values from CAPS documentation so forgive the magic numbers. The values should probably come from a configuration or resource file
	iFocalLength[0]	   = 665.0f;
	iPrincipalPoint[0] = 665.0f;
	iFocalLength[1]	   = 320.0f;
	iPrincipalPoint[1] = 240.0f;
 
 	iSkew = 0.0f;

	for (TInt i = 0; i < KPanLensDist; i++)
		{
		iDist[i] = 0.0f;
		}

	iWidth  = 640; // VGA
	iHeight = 480;
	}

