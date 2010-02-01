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

#include "overlaytransformextension.h"
#include <fbs.h>

/**
This function sets the position, where overlay image is to be placed
@param  "aPosition"		
		The position for overlay image
*/
EXPORT_C void COverlayTransformExtension::SetPosition(const TPoint& aPosition)
	{
	DoSetPosition(aPosition);
	}


/**
Sets the transparency level of the overlay image.
Its value can be 0-100 where 0 for no transparency and 
100 for fully transparent.
If overlay image is CFbsBitmap/CImageFrame then it can have alpha
value. In this case when the application sets the transparency level 
using this function then the alpha value must be ignored.
@param  "aLevel"		
		The transparency level
@leave  KErrNotSupported
		The plugin does not support this.
*/
EXPORT_C void COverlayTransformExtension::SetTransparencyLevelL(TUint aLevel)
	{
	DoSetTransparencyLevelL(aLevel);
	}

/**
Sets the Overlay image in the file
@param  "aFilename"		
		The overlay image filename
@param  "aOverlayImageUid"		
		The format of the overlay image
@leave  KErrNotSupported
		The plugin does not support this format of the the overlay image file
*/
EXPORT_C void COverlayTransformExtension::SetOverlayFileL(const TDesC& aFilename, const TUid& aOverlayImageUid)
	{
	DoSetOverlayFileL(aFilename, aOverlayImageUid);
	}

/**
Sets the Overlay image in the descriptor
@param  "aData"		
		The descriptor containing overlay image
@param  "aOverlayImageUid"		
		The format of the overlay image
@leave  KErrNotSupported
		The plugin does not support this format of the the overlay image file
*/
EXPORT_C void COverlayTransformExtension::SetOverlayDataL(const TDesC8& aData, const TUid& aOverlayImageUid)
	{
	DoSetOverlayDataL(aData, aOverlayImageUid);
	}

/**
Sets the Overlay image in the bitmap
@param  "aBitmap"		
		The bitmap containing overlay image
@leave  KErrNotSupported
		The plugin does not support this format of the the overlay image file
*/
EXPORT_C void COverlayTransformExtension::SetOverlayImageL(const CFbsBitmap& aBitmap)
	{	
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	    {
	    User::Leave(KErrNotSupported);
	    }
	
	DoSetOverlayImageL(aBitmap);
	}

/**
Sets the Overlay image in the image frame
@param  "aImageFrame"		
		The image frame containing the overlay image
@leave  KErrNotSupported
		The plugin does not support this format of the the overlay image file
*/
EXPORT_C void COverlayTransformExtension::SetOverlayImageL(const CImageFrame& aImageFrame)
	{
	DoSetOverlayImageL(aImageFrame);
	}

/**
Gets the formats of the overlay image that are supported 
@param  "aFormats"		
		On return, contains the supported overlay image formats 
@leave  The function leaves with one of the system wide error codes, if the operation fails
*/
EXPORT_C void COverlayTransformExtension::SupportedOverlayInputFormatsL(RArray<TUid>& aFormats) const
	{
	DoSupportedOverlayInputFormatsL(aFormats);
	}

/**
Gets the sub-formats of Overlay image that are supported 
@param  "aFormat"		
		The image format for which supported sub-format list is required
@param  "aSubFormats"		
		On return, contains the supported overlay image sub-formats 
@leave  The function leaves with one of the system wide error codes, if the operation fails
*/
EXPORT_C void COverlayTransformExtension::SupportedOverlayInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const
	{
	DoSupportedOverlayInputSubFormatsL(aFormat, aSubFormats);
	}


/**
Reserved for future-proofing
*/
EXPORT_C void COverlayTransformExtension::ReservedVirtual5() 
	{
	}

