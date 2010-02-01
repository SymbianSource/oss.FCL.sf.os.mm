// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the extension to the plugin API for the Image Display framework
// 
//

#ifndef GENICLIMAGEDISPLAYEXT_H
#define GENICLIMAGEDISPLAYEXT_H 

/**
@file GenIclImageDisplayExt.h
@publishedPartner
*/

#include <e32base.h>
#include <icl/imagedisplaypluginext.h>
#include <icl/geniclimagedisplay.hrh>

const TUid KUidGenIclImageDisplayPluginExtUid={KUidGenIclImageDisplayPluginExt};

/**
This class extends the functionality of the Image Display Library client API defined 
in the class CImageDisplay. In order to invoke the extended functionality, the user needs to call the 
CImageDisplay::ExtensionInterface( ) with the KUidGenIclImageDisplayPluginExtUid interface UID
and an interface pointer to the object of this class. This is an optional extension and some framework 
plug-ins may not support it.

@publishedPartner
@released
*/
class MGenIclImageDisplayExtension :public MImageDisplayPluginExtension
	{
public:
	/**
	Sets the level of quality for the scaling algorithm.
	If quality level is not set by calling this function then 
	the fastest/lowest quality algorithm is used.
	Note: This function must be called before calling CImageDisplay::Play()
	
	@param aQualityLevel Quality level value for the scaling algorithm.
				See CBitmapScaler::TQualityAlgorithm for a set of possible values.
	@return A system-wide error code.
	*/	
	virtual TInt SetScaleQuality(TInt aQualityLevel)=0;
	
	/**
	Sets the maximum quality reduction factor for image decoding.
	By default, the plug-in calculates an optimal maximum reduction factor 
	and uses it to produce a series of roughly-decoded images, passing each of 
	them to the observer interface with the CImageDisplayPlugin::EStatusPartialFrame flag set.
	Note: This function must be called before calling CImageDisplay::Play().
	
	@param aMaxReductionFactor
				The maximum quality reduction factor.
				Usually the maximum reduction can be specfied in range of 0-3.
				A value of 0 means no quality reduction is allowed, so a full-quality image will be decoded as the first frame.
				A value of 1 means (1/2)^1=1/2 of the original image quality. 
				A value of 3 means (1/2)^3=1/8 of the original image quality. 
				If the reduction factor is not set by calling this function then
				the reduction factor value of 0 is used for losslessly encoded images; it is 3 for jpeg images. 
	@return KErrArgument if the aMaxReductionFactor parameter is not in the supported range, or a system-wide error code.
	*/	
	virtual TInt SetMaximumReductionFactor(TInt aMaxReductionFactor)=0;
	
	/**
	This function specifies the image size threshold at or below which no reduction factor is applied.  
	If an image exceeds this size then a reduction factor may be applied to it to speed up decoding.  
	The maximum reduction factor is set by calling SetMaximumReductionFactor().
	Note: This function must be called before calling CImageDisplay::Play()
	
	@param aSizeThreshold A value for the image size threshold in pixels.
	@return A system-wide error code.
	*/	
	virtual TInt SetMaximumNonReducedSize(const TSize& aSizeThreshold)=0;
	};

#endif // GENICLIMAGEDISPLAYEXT_H
