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
// This is an internal API for bitmap transform - supporting implementation by plugin
// 
//

/**
 @file
 @publishedPartner
 @released
*/

#ifndef BITMTRANSPLUGIN_H
#define BITMTRANSPLUGIN_H

#include <e32base.h>
#include <bitmaptransforms.h>

_LIT8(KBitmTransPluginMatchString, "*"); // ECom insists on something

/**
Plugin API for the BitmapTransforms Library bitmap rotation.
The CBitmapRotator implementation loads a plugin based on this interface class. 
Once this has been constructed, calls to method functions of CBitmapRotator are passed
verbatim to this interface. For further description of required functionality, 
see CBitmapRotator.

@see CBitmapRotator
*/
class MBitmapRotatorPlugin
	{
protected:
	typedef CBitmapRotator::TRotationAngle TRotationAngle;

public:
	/**
	This must provide an implementation as defined by CBitmapRotator::~CBitmapRotator()
	
	@see CBitmapRotator::~CBitmapRotator()
	*/	
	virtual ~MBitmapRotatorPlugin() {};
	
	/**
	This must provide an implementation as defined by 
	CBitmapRotator::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle)
	
	@param	aRequestStatus
			A pointer to the completion status of the asynchronous operation.
	@param	aSrcBitmap
			This bitmap should have been created and be of type EColor16M.
	@param	aTgtBitmap
			This bitmap should have been created and be of type EColor16M.
	@param  aAngle
			A member of the enumeration TRotationAngle that specifies the rotation mirror operation.
			
	@see CBitmapRotator::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle)
	*/	
	virtual void Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle)=0;
	
	/**
	This must provide an implementation as defined by 
	CBitmapRotator::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle)
	
	@param	aRequestStatus
			A pointer to the completion status of the asynchronous operation.
	@param	aBitmap
			A reference to a CFbsBitmap. This bitmap should have been created
			and is also an output.
	@param	aAngle
			A member of the enumeration TRotationAngle that specifies the rotation mirror operation.
			
	@see CBitmapRotator::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle)
	*/	
	virtual void Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle)=0;
	
	/**
	This must provide an implementation as defined by CBitmapRotator::CustomCommand(TUid aUid, TAny* aParam)
	
	@param aUid
	@param aParam
	@return TInt
	@see CBitmapRotator::CustomCommand(TUid aUid, TAny* aParam)
	*/	
	virtual TInt CustomCommand(TUid aUid, TAny* aParam)=0;

	/**
	This must provide an implementation as defined by CBitmapRotator::Cancel()
	
	@see CBitmapRotator::Cancel()
	*/	
	virtual void Cancel()=0;
	};

/**
@released

Plugin API for the BitmapTransforms Library bitmap scaling.
The CBitmapScaler implementation loads a plugin based on this interface class. 
Once this has been constructed, calls to method functions of CBitmapScaler are passed
verbatim to this interface. For further description of required functionality, 
see CBitmapScaler.

@see CBitmapScaler
*/
class MBitmapScalerPlugin
	{
public:

	/**
	This must provide an implementation as defined by CBitmapScaler::~CBitmapScaler()
	
	@see CBitmapScaler::~CBitmapScaler()
	*/
	virtual ~MBitmapScalerPlugin() {};
	
	/**
	This must provide an implementation as defined by 
	CBitmapScaler::Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TBool aMaintainAspectRatio = ETrue)
	
	@param	aRequestStatus
			On return, contains a pointer to the completion status of the rescale of the bitmap.
	@param	aSrcBitmap
			The bitmap to be re-scaled.
	@param	aTgtBitmap
			The target location for the re-scaled bitmap.
	@param	aMaintainAspectRatio
			A boolean indicating if the aspect ratio is maintained. ETrue means the aspect ratio is 
	        retained; this is the default. The same scaling factor is applied in both the horizontal 
	        and vertical directions. This is the smaller of the horizontal scaling factor and the 
	        vertical scaling factor. EFalse means the aspect ratio need not be retained.
	
	@see CBitmapScaler::Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TBool aMaintainAspectRatio = ETrue)
	*/
	virtual void Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TBool aMaintainAspectRatio = ETrue)=0;
	
	/**
	This must provide an implementation as defined by 
	CBitmapScaler::Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, const TSize& aDestinationSize, TBool aMaintainAspectRatio = ETrue)
	
	@param	aRequestStatus
			On return, contains a pointer to the completion status of the rescale of the bitmap.
	@param	aBitmap
			The bitmap to be re-scaled. This reference is also the
			target location for the re-scaled bitmap.
	@param	aDestinationSize
			The requested target size for the re-scaled bitmap.
	@param	aMaintainAspectRatio
	        A boolean indicating if the aspect ratio is maintained. ETrue means the aspect ratio is 
	        retained; this is the default. The same scaling factor is applied in both the horizontal 
	        and vertical directions. This is the smaller of the horizontal scaling factor and the 
	        vertical scaling factor. EFalse means the aspect ratio need not be retained.
	        
	@see CBitmapScaler::Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, const TSize& aDestinationSize, TBool aMaintainAspectRatio = ETrue)
	*/
	virtual void Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, const TSize& aDestinationSize, TBool aMaintainAspectRatio = ETrue)=0;
	
	/**
	This must provide an implementation as defined by CBitmapScaler::Cancel()
	
	@see CBitmapScaler::Cancel()
	*/
	virtual void Cancel()=0;
	
	/**
	This must provide an implementation as defined by CBitmapScaler::CustomCommand(TUid aUid, TAny* aParam)
	
	@param aUid
	@param aParam
	@return TInt
	@see CBitmapScaler::CustomCommand(TUid aUid, TAny* aParam)
	*/	
	virtual TInt CustomCommand(TUid aUid, TAny* aParam)=0;
	};

#endif BITMTRANSPLUGIN_H

