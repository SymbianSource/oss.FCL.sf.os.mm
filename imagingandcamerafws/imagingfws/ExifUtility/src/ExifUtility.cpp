// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

//#include <e32base.h>
#include <iclexif.h>
#include  "ExifTagDescriptions.h"

#include "ExifUtility.h"

/**
Constructor

Instantiates the exif reader utility.
Note: no method on this utility should be used after the metadate has been destroyed

@param aExifMetadata
	the metadata class on which to read the tags
*/
EXPORT_C TExifReaderUtility::TExifReaderUtility(MExifMetadataReader* aExifMetadata)
	: iExifRead(aExifMetadata)
	{
	}
	

// Getters
/**
Gets the make tag value of the metadata. 

@param aParam
	on return, the make value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetMake(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag010F[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the model tag value of the metadata. 

@param aParam
	on return, the model value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetModel(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0110[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the dateTime tag value of the metadata. 

@param aParam
	on return, the dateTime value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetDateTime(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0132[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the dateTimeOriginal tag value of the metadata. 

@param aParam
	on return, the dateTimeOriginal value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetDateTimeOriginal(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9003[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the dateTimeDigitized tag value of the metadata. 

@param aParam
	on return, the dateTimeDigitized value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetDateTimeDigitized(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9004[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the interoperabilityIndex tag value of the metadata. 

@param aParam
	on return, the interoperabilityIndex value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetInteroperabilityIndex(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0001[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the interoperabilityVersion tag value of the metadata. 

@param aParam
	on return, the interoperabilityVersion value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetInteroperabilityVersion(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0002[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the exifVersion tag value of the metadata. 

@param aParam
	on return, the exifVersion value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetExifVersion(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9000[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the componentsConfiguration tag value of the metadata. 

@param aParam
	on return, the ComponentConfiguration value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetComponentsConfiguration(HBufC8*& aParam) const
	{	
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9101[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the flashpixVersion tag value of the metadata. 

@param aParam
	on return, the flashpixVersion value
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetFlashPixVersion(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA000[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the main image xResolution tag value of the metadata. 

@param aNumerator
	on return, the main image xResolution numerator value
	
@param aDenominator
	on return, the main image xResolution denominator value
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetXResolution(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag011A[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}
	
/**
Gets the main image yResolution tag value of the metadata. 

@param aNumerator
	on return, the main image yResolution numerator value
	
@param aDenominator
	on return, the main image yResolution denominator value
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetYResolution(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag011B[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}
	
/**
Gets the thumbnail xResolution tag value of the metadata. 

@param aNumerator
	on return, the main thumbnail xResolution numerator value

@param aDenominator
	on return, the main thumbnail xResolution denominator value
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetThumbXResolution(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag011A[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}
	
/**
Gets the thumbnail yResolution tag value of the metadata. 

@param aNumerator
	on return, the main thumbnail yResolution numerator value

@param aDenominator
	on return, the main thumbnail yResolution denominator value
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetThumbYResolution(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag011B[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the main image resolutionUnit tag value of the metadata. 

@param aParam
	on return, the main image resolutionUnit value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetResolutionUnit(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0128[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}
	
/**
Gets the yCbCrPositioning tag value of the metadata. 

@param aParam
	on return, the yCbCrPositioning value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetYCbCrPositioning(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0213[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}
	
/**
Gets the main image orientation tag value of the metadata. 

@param aParam
	on return, the main image orientation value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/EXPORT_C TInt TExifReaderUtility::GetOrientation(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0112[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}
	
/**
Gets the colorSpace tag value of the metadata. 

@param aParam
	on return, the colorSpace value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetColorSpace(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA001[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}
	
/**
Gets the pixelXDimension tag value of the metadata. 

@param aParam
	on return, the pixelXDimension value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetPixelXDimension(TInt& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA002[ETagValue];
	TInt ifd = KExifIfdZero;

	//This tag can be SHORT or LONG so first try to read as integer if
	//that does not work then try as SHORT
	TInt err = iExifRead->GetIntegerParam(tag, ifd, aParam);
	
	if (err == KErrArgument)
		{
		//then the tag could be short
		TUint16 paramShort = 0;
		err = iExifRead->GetShortParam(tag, ifd, paramShort);
		if (err == KErrNone)
			{
			//tag was SHORT so set aParam
			aParam = static_cast<TInt>(paramShort);
			}
		else
			{
			err = KErrArgument; //just to ensure behaviour is unaltered from 9.x
			}
		}
	return err;
	}
	
/**
Gets the pixelYDimension tag value of the metadata. 

@param aParam
	on return, the pixelYDimension value
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetPixelYDimension(TInt& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA003[ETagValue];
	TInt ifd = KExifIfdZero;

	//This tag can be SHORT or LONG so first try to read as integer if
	//that does not work then try as SHORT
	TInt err = iExifRead->GetIntegerParam(tag, ifd, aParam);
	
	if (err == KErrArgument)
		{
		//then the tag could be short
		TUint16 paramShort = 0;
		err = iExifRead->GetShortParam(tag, ifd, paramShort);
		if (err == KErrNone)
			{
			//tag was SHORT so set aParam
			aParam = static_cast<TInt>(paramShort);
			}
		else
			{
			err = KErrArgument; //just to ensure behaviour is unaltered from 9.x
			}
		}
	return err;
	}

/**
Gets the thumbnail compression tag value of the metadata. 

@param aParam
	on return, the thumbnail compression value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetThumbCompression(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0103[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}
	
/**
Gets the thumbnail resolutionUnit tag value of the metadata. 

@param aParam
	on return, the thumbnail resolutionUnit value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetThumbResolutionUnit(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0128[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}

/**
Gets the thumbnail orientation tag value of the metadata. 

@param aParam
	on return, the thumbnail orientation value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifReaderUtility::GetThumbOrientation(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0112[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}

/**
Gets the image description tag value of the metadata. 

@param aParam
	on return, the image description value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetImageDescription(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag010E[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the copyright tag value of the metadata. 

@param aParam
	on return, the copyright value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetCopyright(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag8298[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the maker note tag value of the metadata. 

@param aParam
	on return, the maker note value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetMakerNote(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag927C[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the user comment tag value of the metadata. 

@param aParam
	on return, the user comment value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

	Note: The first 8 bytes of aParam may contain a Character Code to indicate the type of encoding used. See table 6 of the Exif 2.2 specification for a list of codes.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetUserComment(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9286[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetParam8(tag, ifd, aParam);
	}
	
/**
Gets the flash tag value of the metadata. 

@param aParam
	on return, the flash value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetFlash(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9209[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}

/**
Gets the focal length tag value of the metadata. 

@param aNumerator
	on return, the focal length numerator value.
		
@param aDenominator
	on return, the focal length denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetFocalLength(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag920A[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}
	
/**
Gets the shutter speed tag value of the metadata. 

@param aNumerator
	on return, the shutter speed numerator value.
	
	Note that aNumerator is a true signed quantity, so no typecasting to an unsigned type is required.

@param aDenominator
	on return, the shutter speed denominator value.

	Note that aDenominator is a true signed quantity, so no typecasting to an unsigned type is required.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetShutterSpeedValue(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9201[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the thumbnail jpeg interchange format length of the metadata. 

@deprecated - Replaced by TExifReaderUtility::GetJpegInterchangeFormatLength(TUint& aParam).

@param aParam
	on return, the thumbnail jpeg interchange format length.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetJpegInterchangeFormatLength(TInt& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0202[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifRead->GetIntegerParam(tag, ifd, aParam);
	}

/**
Gets the thumbnail jpeg interchange format length of the metadata. 

@param aParam
	on return, the thumbnail jpeg interchange format length.
		
@return error code
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetJpegInterchangeFormatLength(TUint& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0202[ETagValue];
	TInt ifd = KExifIfdOne;
	TInt value = 0;
	TInt result = iExifRead->GetIntegerParam(tag, ifd, value);
	if (result == KErrNone)
		{
		aParam = static_cast<TUint>(value);
		}
	return result;
	}
	
/**
Gets the gps version id tag value of the metadata. 

@param aParam
	on return, the gps version id.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsVersionId(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0000[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps north or south latitude tag value of the metadata. 

@param aParam
	on return, the gps north or south latitude.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsLatitudeRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0001[ETagValue];
	 
	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps latitude tag value of the metadata. 

@param aDegNumerator
	on return, the degrees latitude numerator value.
		
@param aDegDenominator
	on return, the degrees latitude denominator value.
		
@param aMinNumerator
	on return, the minutes latitude numerator value.
		
@param aMinDenominator
	on return, the minutes latitude denominator value.
		
@param aSecNumerator
	on return, the seconds latitude numerator value.
		
@param aSecDenominator
	on return, the seconds latitude denominator value.
			
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
	KErrCorrupt if the gps latitude tag value of the metadata is corrupt. 
*/
EXPORT_C TInt TExifReaderUtility::GetGpsLatitude(TInt& aDegNumerator, TInt& aDegDenominator, TInt& aMinNumerator, TInt& aMinDenominator, TInt& aSecNumerator, TInt& aSecDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0002[ETagValue];
	TInt count = KTagGPS0002[EComponentCountValue];
	
	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	TInt result = iExifRead->GetRationalArrayParam(tag, ifd, *param);
	if (result==KErrNone)
		{
		if(param->Count()!=count)
			{
			result=KErrCorrupt;
			}
		else
			{
			aDegNumerator = param->At(0).iNumerator;
			aDegDenominator = param->At(0).iDenominator;
			aMinNumerator = param->At(1).iNumerator;
			aMinDenominator = param->At(1).iDenominator;
			aSecNumerator = param->At(2).iNumerator;
			aSecDenominator = param->At(2).iDenominator;
			}
		}		

	delete param;
	return result;
	}

/**
Gets the gps east or west longitude tag value of the metadata. 

@param aParam
	on return, the gps east or west longitude.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsLongitudeRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0003[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps longitude tag value of the metadata. 

@param aDegNumerator
	on return, the degrees longitude numerator value.
		
@param aDegDenominator
	on return, the degrees longitude denominator value.
	
@param aMinNumerator
	on return, the minutes longitude numerator value.
	
@param aMinDenominator
	on return, the minutes longitude denominator value.
	
@param aSecNumerator
	on return, the seconds longitude numerator value.

@param aSecDenominator
	on return, the seconds longitude denominator value.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
	KErrCorrupt if the gps longitude tag value of the metadata is corrupt. 
*/
EXPORT_C TInt TExifReaderUtility::GetGpsLongitude(TInt& aDegNumerator, TInt& aDegDenominator, TInt& aMinNumerator, TInt& aMinDenominator, TInt& aSecNumerator, TInt& aSecDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0004[ETagValue];
	TInt count = KTagGPS0004[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	TInt result = iExifRead->GetRationalArrayParam(tag, ifd, *param);
	if (result==KErrNone)
		{
		if(param->Count()!=count)
			{
			result=KErrCorrupt;
			}
		else
			{
			aDegNumerator = param->At(0).iNumerator;
			aDegDenominator = param->At(0).iDenominator;
			aMinNumerator = param->At(1).iNumerator;
			aMinDenominator = param->At(1).iDenominator;
			aSecNumerator = param->At(2).iNumerator;
			aSecDenominator = param->At(2).iDenominator;
			}
		}		

	delete param;
	return result;
	}

/**
Gets the gps altitude reference tag value of the metadata. 

@param aParam
	on return, the gps altitude reference.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsAltitudeRef(TUint8& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0005[ETagValue];
	TInt count = KTagGPS0005[EComponentCountValue];
	
	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	HBufC8* data = NULL;
	TRAPD(err, data=HBufC8::NewL(count));
	if (err!=KErrNone)
		{
		return KErrNoMemory;
		}
	TInt result = iExifRead->GetParam8(tag, ifd, data);
	if (result==KErrNone)
		{
		aParam = static_cast<TUint8>((*data)[0]);
		}
	delete data;
	return result;
	}
	
/**
Gets the gps altitude tag value of the metadata. 

@param aNumerator
	on return, the gps altitude numerator value.
		
@param aDenominator
	on return, the gps altitude denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsAltitude(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0006[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the gps time tag value of the metadata. 

@param aHourNumerator
	on return, the gps hour numerator value.
		
@param aHourDenominator
	on return, the gps hour denominator value.
	
@param aMinNumerator
	on return, the gps minutes numerator value.
	
@param aMinDenominator
	on return, the gps minutes denominator value.
	
@param aSecNumerator
	on return, the gps seconds numerator value.
	
@param aSecDenominator
	on return, the gps seconds denominator value.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
	KErrCorrupt if the gps longitude tag value of the metadata is corrupt. 
*/
EXPORT_C TInt TExifReaderUtility::GetGpsTimeStamp(TInt& aHourNumerator, TInt& aHourDenominator, TInt& aMinNumerator, TInt& aMinDenominator, TInt& aSecNumerator, TInt& aSecDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0007[ETagValue];
	TInt count = KTagGPS0007[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	TInt result = iExifRead->GetRationalArrayParam(tag, ifd, *param);
	if (result==KErrNone)
		{
		if(param->Count()!=count)
			{
			result=KErrCorrupt;
			}
		else
			{
			aHourNumerator = param->At(0).iNumerator;
			aHourDenominator = param->At(0).iDenominator;
			aMinNumerator = param->At(1).iNumerator;
			aMinDenominator = param->At(1).iDenominator;
			aSecNumerator = param->At(2).iNumerator;
			aSecDenominator = param->At(2).iDenominator;
			}
		}		

	delete param;
	return result;
	}

/**
Gets the gps satellites used for measurement tag value of the metadata. 

@param aParam
	on return, the gps satellites used for measurement value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsSatellites(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0008[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps receiver status tag value of the metadata. 

@param aParam
	on return, the gps receiver status value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsStatus(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0009[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps measurement mode tag value of the metadata. 

@param aParam
	on return, the gps measurement mode value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsMeasureMode(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000A[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps measurement precision tag value of the metadata. 

@param aNumerator
	on return, the gps measurement precision numerator value.
		
@param aDenominator
	on return, the gps measurement precision denominator value.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDop(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000B[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the gps speed unit tag value of the metadata. 

@param aParam
	on return, the gps speed unit value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsSpeedRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000C[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the speed of gps receiver tag value of the metadata. 

@param aNumerator
	on return, the speed of gps receiver numerator value.
		
@param aDenominator
	on return, the speed of gps receiver denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsSpeed(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000D[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the gps reference for direction of movement tag value of the metadata. 

@param aParam
	on return, the gps reference for direction of movement value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsTrackRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000E[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps direction of movement tag value of the metadata. 

@param aNumerator
	on return, the gps direction of movement numerator value.
		
@param aDenominator
	on return, the gps direction of movement denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsTrack(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000F[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the gps reference for direction of image tag value of the metadata. 

@param aParam
	on return, the gps reference for direction of image value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsImgDirectionRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0010[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps direction of image tag value of the metadata. 

@param aNumerator
	on return, the gps direction of image numerator value.
		
@param aDenominator
	on return, the gps direction of image denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsImgDirection(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0011[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the gps geodetic survey data used tag value of the metadata. 

@param aParam
	on return, the geodetic survey data used value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsMapDatum(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0012[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps reference for latitude of destination tag value of the metadata. 

@param aParam
	on return, the reference for latitude of destination value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestLatitudeRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0013[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps latitude of destination tag value of the metadata. 

@param aDegNumerator
	on return, the degrees latitude of destination numerator value.
		
@param aDegDenominator
	on return, the degrees latitude of destination denominator value.
	
@param aMinNumerator
	on return, the minutes latitude of destination numerator value.
	
@param aMinDenominator
	on return, the minutes latitude of destination denominator value.
	
@param aSecNumerator
	on return, the seconds latitude of destination numerator value.
	
@param aSecDenominator
	on return, the seconds latitude of destination denominator value.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestLatitude(TInt& aDegNumerator, TInt& aDegDenominator, TInt& aMinNumerator, TInt& aMinDenominator, TInt& aSecNumerator, TInt& aSecDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0014[ETagValue];
	TInt count = KTagGPS0014[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	TInt result = iExifRead->GetRationalArrayParam(tag, ifd, *param);
	if (result==KErrNone)
		{
		if(param->Count()!=count)
			{
			result=KErrCorrupt;
			}
		else
			{
			aDegNumerator = param->At(0).iNumerator;
			aDegDenominator = param->At(0).iDenominator;
			aMinNumerator = param->At(1).iNumerator;
			aMinDenominator = param->At(1).iDenominator;
			aSecNumerator = param->At(2).iNumerator;
			aSecDenominator = param->At(2).iDenominator;
			}
		}		

	delete param;
	return result;
	}

/**
Gets the gps reference for longitude of destination tag value of the metadata. 

@param aParam
	on return, the gps reference for longitude of destination value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestLongitudeRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0015[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps longitude of destination tag value of the metadata. 

@param aDegNumerator
	on return, the degrees longitude of destination numerator value.
		
@param aDegDenominator
	on return, the degrees longitude of destination denominator value.
	
@param aMinNumerator
	on return, the minutes longitude of destination numerator value.
	
@param aMinDenominator
	on return, the minutes longitude of destination denominator value.
	
@param aSecNumerator
	on return, the seconds longitude of destination numerator value.
	
@param aSecDenominator
	on return, the seconds longitude of destination denominator value.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestLongitude(TInt& aDegNumerator, TInt& aDegDenominator, TInt& aMinNumerator, TInt& aMinDenominator, TInt& aSecNumerator, TInt& aSecDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0016[ETagValue];
	TInt count = KTagGPS0016[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	TInt result = iExifRead->GetRationalArrayParam(tag, ifd, *param);
	if (result==KErrNone)
		{
		if(param->Count()!=count)
			{
			result=KErrCorrupt;
			}
		else
			{
			aDegNumerator = param->At(0).iNumerator;
			aDegDenominator = param->At(0).iDenominator;
			aMinNumerator = param->At(1).iNumerator;
			aMinDenominator = param->At(1).iDenominator;
			aSecNumerator = param->At(2).iNumerator;
			aSecDenominator = param->At(2).iDenominator;
			}
		}		

	delete param;
	return result;
	}

/**
Gets the gps reference for bearing of destination tag value of the metadata. 

@param aParam
	on return, the reference for bearing of destination value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestBearingRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0017[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps bearing of destination tag value of the metadata. 

@param aNumerator
	on return, the bearing of destination numerator value.
		
@param aDenominator
	on return, the bearing of destination denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestBearing(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0018[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the gps reference for distance to destination tag value of the metadata. 

@param aParam
	on return, the reference for distance to destination value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestDistanceRef(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0019[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps distance to destination tag value of the metadata. 

@param aNumerator
	on return, the distance to destination numerator value.
		
@param aDenominator
	on return, the distance to destination denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDestDistance(TInt& aNumerator, TInt& aDenominator) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001A[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Gets the name of gps processing method tag value of the metadata. 

@param aParam
	on return, the name of gps processing method value.

	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

	Note: According to the Exif 2.2 specification, the first 8 bytes of aParam should contain a Character Code to indicate the type of encoding used. See table 6 of the Exif 2.2 specification for a list of codes.
			
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsProcessingMethod(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001B[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the name of gps area tag value of the metadata. 

@param aParam
	on return, the name of gps area value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.

	Note: According to the Exif 2.2 specification, the first 8 bytes of aParam should contain a Character Code to indicate the type of encoding used. See table 6 of the Exif 2.2 specification for a list of codes.
		
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsAreaInformation(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001C[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps date tag value of the metadata. 

@param aParam
	on return, the gps date value.
	
	Ownership of the buffer pointed to by aParam transfers to the client, which is therefore responsible for deleting the buffer when required. 
	Note: The client should always initialise aParam to NULL, and the plugin should only assign to this parameter when ready to transfer ownership.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDateStamp(HBufC8*& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001D[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetParam8(tag, ifd, aParam);
	}

/**
Gets the gps differential correction tag value of the metadata. 

@param aParam
	on return, the gps differential correction value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifReaderUtility::GetGpsDifferential(TUint16& aParam) const
	{
	if(iExifRead==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001E[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifRead->GetShortParam(tag, ifd, aParam);
	}

// Setters

/**
Constructor

instantiates the exif writer utility.
Note: no method on this utility should be used after the metadate has been destroyed

@param aExifMetadata
	the metadata class on which to read the tags
*/
EXPORT_C TExifWriterUtility::TExifWriterUtility(MExifMetadataWriter* aExifMetadata):
			iExifWrite(aExifMetadata)
	{
	}

/**
Sets the make tag value of the metadata. 

@param aMake
	the make value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetMake(HBufC8* aMake)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag010F[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aMake);
	}
	
/**
Sets the model tag value of the metadata. 

@param aModel
	the model value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetModel(HBufC8* aModel)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0110[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aModel);
	}
	
/**
Sets the dateTime tag value of the metadata. 

@param aDateTime
	the dateTime value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetDateTime(HBufC8* aDateTime)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0132[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aDateTime);
	}

/**
Sets the dateTimeOriginal tag value of the metadata. 

@param aDateTime
	the dateTimeOriginal value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetDateTimeOriginal(HBufC8* aDateTime)	
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9003[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aDateTime);
	}
	
/**
Sets the dateTimeDigitized tag value of the metadata. 

@param aDateTime
	the dateTimeDigitized value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetDateTimeDigitized(HBufC8* aDateTime)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9004[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aDateTime);
	}
	
/**
Sets the interoperabilityindex tag value of the metadata. 

@param aIdx
	the interoperabilityIndex value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetInteroperabilityIndex(HBufC8* aIdx)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0001[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aIdx);
	}

/**
Sets the interoperabilityVersion tag value of the metadata. 

@param aIdx
	the interoperabilityVersion value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetInteroperabilityVersion(HBufC8* aIdx)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0002[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aIdx);
	}

/**
Sets the exifVersion tag value of the metadata. 

@param aExifVersion
	the exifVersion value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetExifVersion(HBufC8* aExifVersion)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9000[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aExifVersion);
	}

/**
Sets the componentsConfiguration tag value of the metadata. 

@param aComponentsConfig
	the componentsConfiguration value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetComponentsConfiguration(HBufC8* aComponentsConfig)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9101[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aComponentsConfig);
	}

/**
Sets the flashpixVersion tag value of the metadata. 

@param aFlashPixVer
	the flashpixVersion value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetFlashPixVersion(HBufC8* aFlashPixVer)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA000[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aFlashPixVer);
	}

/**
Sets the main image xResolution tag value of the metadata. 

@param aNumerator
	the main image xResolution value numerator
@param aDenominator
	the main image xResolution value denominator
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetXResolution(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag011A[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the main image yResolution tag value of the metadata. 

@param aNumerator
	the main image yResolution value numerator
@param aDenominator
	the main image yResolution value denominator
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetYResolution(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag011B[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the thumbnail xResolution tag value of the metadata. 

@param aNumerator
	the thumbnail xResolution value numerator
@param aDenominator
	the thumbnail xResolution value denominator
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetThumbXResolution(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag011A[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the thumbnail yResolution tag value of the metadata. 

@param aNumerator
	the thumbnail yResolution value numerator
@param aDenominator
	the thumbnail yResolution value denominator
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetThumbYResolution(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag011B[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the main image resolutionUnit tag value of the metadata. 

@param aResUnit
	the main image resolutionUnit value
	
	Note: aResUnit may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetResolutionUnit(TInt aResUnit)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0128[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetShortParam(tag, ifd, aResUnit);
	}

/**
Sets the yCbCrPositioning tag value of the metadata. 

@param aPositioning
	the yCbCrPositioning value
	
	Note: aPositioning may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetYCbCrPositioning(TInt aPositioning)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0213[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetShortParam(tag, ifd, aPositioning);
	}

/**
Sets the main image orientation tag value of the metadata. 

@param aOrientation
	the main image orientation value
	
	Note: aOrientation may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetOrientation(TInt aOrientation)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag0112[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetShortParam(tag, ifd, aOrientation);
	}

/**
Sets the colorSpace tag value of the metadata. 

@param aColorSpace
	the colorSpace value
	
	Note: aColorSpace may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetColorSpace(TInt aColorSpace)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA001[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetShortParam(tag, ifd, aColorSpace);
	}

/**
Sets the pixelXDimension tag value of the metadata. 

@param aPixelXDim
	the pixelXDimension value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetPixelXDimension(TInt aPixelXDim)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA002[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetIntegerParam(tag, ifd, aPixelXDim);
	}

/**
Sets the pixelYDimension tag value of the metadata. 

@param aPixelYDim
	the pixelYDimension value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetPixelYDimension(TInt aPixelYDim)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagA003[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetIntegerParam(tag, ifd, aPixelYDim);
	}

/**
Sets the thumbnail compression tag value of the metadata. 

@param aCompression
	the thumbnail compression value
	
	Note: aCompression may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetThumbCompression(TInt aCompression)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0103[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifWrite->SetShortParam(tag, ifd, aCompression);
	}

/**
Sets the thumbnail resolutionUnit tag value of the metadata. 

@param aResUnit
	the thumbnail resolutionUnit value
	
	Note: aResUnit may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetThumbResolutionUnit(TInt aResUnit)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0128[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifWrite->SetShortParam(tag, ifd, aResUnit);
	}

/**
Sets the thumbnail orientation tag value of the metadata. 

@param aOrientation
	the thumbnail orientation value
	
	Note: aOrientation may only take unsigned short (16-bit) values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
*/
EXPORT_C TInt TExifWriterUtility::SetThumbOrientation(TInt aOrientation)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KThumbTag0112[ETagValue];
	TInt ifd = KExifIfdOne;
	return iExifWrite->SetShortParam(tag, ifd, aOrientation);
	}

/**
Sets the image description tag value of the metadata. 

@param aImageDescription
	the image description value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetImageDescription(HBufC8* aImageDescription)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag010E[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aImageDescription);
	}

/**
Sets the copyright tag value of the metadata. 

@param aCopyright
	the copyright value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetCopyright(HBufC8* aCopyright)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag8298[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aCopyright);
	}

/**
Sets the maker note tag value of the metadata. 

@param aMakerNote
	the maker note value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetMakerNote(HBufC8* aMakerNote)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag927C[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aMakerNote);
	}

/**
Sets the user comment tag value of the metadata. 

@param aUserComment
	the user comment value.

	Note: According to the Exif 2.2 specification, the first 8 bytes of aUserComment must contain a Character Code to indicate the type of encoding used. See table 6 of the Exif 2.2 specification for a list of codes.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetUserComment(HBufC8* aUserComment)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9286[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetParam8(tag, ifd, aUserComment);
	}

/**
Sets the flash tag value of the metadata. 

@param aFlash
	the flash value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetFlash(TUint16 aFlash)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9209[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetShortParam(tag, ifd, aFlash);
	}

/**
Sets the focal length tag value of the metadata. 

@param aNumerator
	the focal length numerator value
@param aDenominator
	the focal length denominator value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetFocalLength(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag920A[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the shutter speed tag value of the metadata. 

@param aNumerator
	the shutter speed numerator value.
	
	Note that aNumerator may take signed values.

@param aDenominator
	the shutter speed denominator value.
	
	Note that aDenominator may take signed values.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetShutterSpeedValue(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTag9201[ETagValue];
	TInt ifd = KExifIfdZero;
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps version id tag value of the metadata. 

@param aGpsVersionId
	the gps version id value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsVersionId(HBufC8* aGpsVersionId)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0000[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
		
	return iExifWrite->SetParam8(tag, ifd, aGpsVersionId);
	}

/**
Sets the gps north or south latitude tag value of the metadata. 

@param aGpsLatitudeRef
	the gps north or south latitude value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsLatitudeRef(HBufC8* aGpsLatitudeRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0001[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsLatitudeRef);
	}


/**
Sets the gps latitude tag value of the metadata. 

@param aDegNumerator
	the degrees latitude numerator value.
@param aDegDenominator
	the degrees latitude denominator value.
@param aMinNumerator
	the minutes latitude numerator value.
@param aMinDenominator
	the minutes latitude denominator value.
@param aSecNumerator
	the seconds latitude numerator value.
@param aSecDenominator
	the seconds latitude denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsLatitude(TInt aDegNumerator, TInt aDegDenominator, TInt aMinNumerator, TInt aMinDenominator, TInt aSecNumerator, TInt aSecDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0002[ETagValue];
	TInt count = KTagGPS0002[EComponentCountValue];
	
	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	const TRational degs={aDegNumerator, aDegDenominator};
	const TRational mins={aMinNumerator, aMinDenominator};
	const TRational secs={aSecNumerator, aSecDenominator};
	
	TRAPD(insertErr,
		param->InsertL(0, degs);
		param->InsertL(1, mins);
		param->InsertL(2, secs); );
	if (insertErr!=KErrNone)
		{
		delete param;
		return insertErr;
		}
		
	TInt result = iExifWrite->SetRationalArrayParam(tag, ifd, *param);		

	delete param;
	return result;		
	}	

/**
Sets the east or west longitude tag value of the metadata. 

@param aGpsLongitudeRef
	the east or west longitude value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsLongitudeRef(HBufC8* aGpsLongitudeRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0003[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsLongitudeRef);
	}

/**
Sets the gps longitude tag value of the metadata. 

@param aDegNumerator
	the degrees longitude numerator value.
@param aDegDenominator
	the degrees longitude denominator value.
@param aMinNumerator
	the minutes longitude numerator value.
@param aMinDenominator
	the minutes longitude denominator value.
@param aSecNumerator
	the seconds longitude numerator value.
@param aSecDenominator
	the seconds longitude denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsLongitude(TInt aDegNumerator, TInt aDegDenominator, TInt aMinNumerator, TInt aMinDenominator, TInt aSecNumerator, TInt aSecDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0004[ETagValue];
	TInt count = KTagGPS0004[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	const TRational degs={aDegNumerator, aDegDenominator};
	const TRational mins={aMinNumerator, aMinDenominator};
	const TRational secs={aSecNumerator, aSecDenominator};
	
	TRAPD(insertErr,
		param->InsertL(0, degs);
		param->InsertL(1, mins);
		param->InsertL(2, secs); );
	if (insertErr!=KErrNone)
		{
		delete param;
		return insertErr;
		}
		
	TInt result = iExifWrite->SetRationalArrayParam(tag, ifd, *param);		

	delete param;
	return result;	
	}

/**
Sets the gps altitude reference tag value of the metadata. 

@param aGpsAltitudeRef
	the altitude reference value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
	KErrNoMemory if there was not enough memory available to complete the request.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsAltitudeRef(TUint8 aGpsAltitudeRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0005[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	HBufC8* data = NULL;
	TRAPD(err, data=HBufC8::NewL(1));
	if (err!=KErrNone)
		{
		return KErrNoMemory;
		}

	TPtr8 bufPtr(data->Des());
	bufPtr.Append(aGpsAltitudeRef);
	
	TInt result = iExifWrite->SetParam8(tag, ifd, data);
	delete data;
	return result;
	}

/**
Sets the gps altitude tag value of the metadata. 

@param aNumerator
	the altitude numerator value.
@param aDenominator
	the altitude denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsAltitude(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0006[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps time tag value of the metadata. 

@param aHourNumerator
	the hour numerator value.
@param aHourDenominator
	the hour denominator value.
@param aMinNumerator
	the minutes numerator value.
@param aMinDenominator
	the minutes denominator value.
@param aSecNumerator
	the seconds numerator value.
@param aSecDenominator
	the seconds denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsTimeStamp(TInt aHourNumerator, TInt aHourDenominator, TInt aMinNumerator, TInt aMinDenominator, TInt aSecNumerator, TInt aSecDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0007[ETagValue];
	TInt count = KTagGPS0007[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	const TRational hours={aHourNumerator, aHourDenominator};
	const TRational mins={aMinNumerator, aMinDenominator};
	const TRational secs={aSecNumerator, aSecDenominator};
	
	TRAPD(insertErr,
		param->InsertL(0, hours);
		param->InsertL(1, mins);
		param->InsertL(2, secs); );
	if (insertErr!=KErrNone)
		{
		delete param;
		return insertErr;
		}
		
	TInt result = iExifWrite->SetRationalArrayParam(tag, ifd, *param);		

	delete param;
	return result;		
	}

/**
Sets the gps satellites used for measurement tag value of the metadata. 

@param aGpsSatellites
	the gps satellites used for measurement value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsSatellites(HBufC8* aGpsSatellites)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0008[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsSatellites);
	}

/**
Sets the gps receiver status tag value of the metadata. 

@param aGpsStatus
	the gps receiver status value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsStatus(HBufC8* aGpsStatus)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0009[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsStatus);
	}

/**
Sets the gps measurement mode tag value of the metadata. 

@param aGpsMeasureMode
	the gps measurement mode value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsMeasureMode(HBufC8* aGpsMeasureMode)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000A[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsMeasureMode);
	}

/**
Sets the gps measurement precision tag value of the metadata. 

@param aNumerator
	the measurement precision numerator value.
@param aDenominator
	the measurement precision denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDop(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000B[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps speed unit tag value of the metadata. 

@param aGpsSpeedRef
	the speed unit value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsSpeedRef(HBufC8* aGpsSpeedRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000C[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsSpeedRef);
	}

/**
Sets the speed of gps receiver tag value of the metadata. 

@param aNumerator
	the speed of gps receiver numerator value.
@param aDenominator
	the speed of gps receiver denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsSpeed(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000D[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps reference for direction of movement tag value of the metadata. 

@param aGpsTrackRef
	the gps reference for direction of movement value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsTrackRef(HBufC8* aGpsTrackRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000E[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsTrackRef);
	}

/**
Sets the direction of movement tag value of the metadata. 

@param aNumerator
	the direction of movement numerator value.
@param aDenominator
	the direction of movement denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsTrack(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS000F[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps reference for direction of image tag value of the metadata. 

@param aGpsImgDirectionRef
	the gps reference for direction of image value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsImgDirectionRef(HBufC8* aGpsImgDirectionRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0010[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsImgDirectionRef);
	}

/**
Sets the direction of image tag value of the metadata. 

@param aNumerator
	the direction of image numerator value.
@param aDenominator
	the direction of image denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsImgDirection(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0011[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps geodetic survey data used tag value of the metadata. 

@param aGpsMapDatum
	the geodetic survey data used value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsMapDatum(HBufC8* aGpsMapDatum)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0012[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsMapDatum);
	}

/**
Sets the gps reference for latitude of destination tag value of the metadata. 

@param aGpsDestLatitudeRef
	the reference for latitude of destination value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestLatitudeRef(HBufC8* aGpsDestLatitudeRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0013[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsDestLatitudeRef);
	}

/**
Sets the gps latitude of destination tag value of the metadata. 

@param aDegNumerator
	on return, the degrees latitude of destination numerator value
@param aDegDenominator
	on return, the degrees latitude of destination denominator value
@param aMinNumerator
	on return, the minutes latitude of destination numerator value
@param aMinDenominator
	on return, the minutes latitude of destination denominator value
@param aSecNumerator
	on return, the seconds latitude of destination numerator value
@param aSecDenominator
	on return, the seconds latitude of destination denominator value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestLatitude(TInt aDegNumerator, TInt aDegDenominator, TInt aMinNumerator, TInt aMinDenominator, TInt aSecNumerator, TInt aSecDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0014[ETagValue];
	TInt count = KTagGPS0014[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	const TRational degs={aDegNumerator, aDegDenominator};
	const TRational mins={aMinNumerator, aMinDenominator};
	const TRational secs={aSecNumerator, aSecDenominator};
	
	TRAPD(insertErr,
		param->InsertL(0, degs);
		param->InsertL(1, mins);
		param->InsertL(2, secs); );
	if (insertErr!=KErrNone)
		{
		delete param;
		return insertErr;
		}
		
	TInt result = iExifWrite->SetRationalArrayParam(tag, ifd, *param);		

	delete param;
	return result;		
	}

/**
Sets the gps reference for longitude of destination tag value of the metadata. 

@param aGpsDestLongitudeRef
	the reference for longitude of destination value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestLongitudeRef(HBufC8* aGpsDestLongitudeRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0015[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsDestLongitudeRef);
	}

/**
Sets the gps longitude of destination tag value of the metadata. 

@param aDegNumerator
	on return, the degrees longitude of destination numerator value
@param aDegDenominator
	on return, the degrees longitude of destination denominator value
@param aMinNumerator
	on return, the minutes longitude of destination numerator value
@param aMinDenominator
	on return, the minutes longitude of destination denominator value
@param aSecNumerator
	on return, the seconds longitude of destination numerator value
@param aSecDenominator
	on return, the seconds longitude of destination denominator value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestLongitude(TInt aDegNumerator, TInt aDegDenominator, TInt aMinNumerator, TInt aMinDenominator, TInt aSecNumerator, TInt aSecDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0016[ETagValue];
	TInt count = KTagGPS0016[EComponentCountValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	CArrayFixFlat<TRational>* param = new CArrayFixFlat<TRational>(count);
	if (param == NULL)
		{
		return KErrNoMemory;
		}

	const TRational degs={aDegNumerator, aDegDenominator};
	const TRational mins={aMinNumerator, aMinDenominator};
	const TRational secs={aSecNumerator, aSecDenominator};
	
	TRAPD(insertErr,
		param->InsertL(0, degs);
		param->InsertL(1, mins);
		param->InsertL(2, secs); );
	if (insertErr!=KErrNone)
		{
		delete param;
		return insertErr;
		}
		
	TInt result = iExifWrite->SetRationalArrayParam(tag, ifd, *param);		

	delete param;
	return result;		
	}

/**
Sets the gps reference for bearing of destination tag value of the metadata. 

@param aGpsDestBearingRef
	the reference for bearing of destination value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestBearingRef(HBufC8* aGpsDestBearingRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0017[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsDestBearingRef);
	}

/**
Sets the gps bearing of destination tag value of the metadata. 

@param aNumerator
	the bearing of destination numerator value
@param aDenominator
	the bearing of destination denominator value
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestBearing(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0018[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the gps reference for distance to destination tag value of the metadata. 

@param aGpsDestDistanceRef
	the reference for distance to destination value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestDistanceRef(HBufC8* aGpsDestDistanceRef)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS0019[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	return iExifWrite->SetParam8(tag, ifd, aGpsDestDistanceRef);
	}

/**
Sets the gps distance to destination tag value of the metadata. 

@param aNumerator
	the distance to destination numerator value.
@param aDenominator
	the distance to destination denominator value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDestDistance(TInt aNumerator, TInt aDenominator)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001A[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetRationalParam(tag, ifd, aNumerator, aDenominator);
	}

/**
Sets the name of gps processing method tag value of the metadata. 

@param aGpsProcessingMethod
	the name of gps processing method value.
	
	Note: According to the Exif 2.2 specification, the first 8 bytes of aGpsProcessingMethod must contain a Character Code to indicate the type of encoding used. See table 6 of the Exif 2.2 specification for a list of codes.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsProcessingMethod(HBufC8* aGpsProcessingMethod)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001B[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsProcessingMethod);
	}

/**
Sets the name of gps area tag value of the metadata. 

@param aGpsAreaInformation
	the name of gps area value.
	
	Note: According to the Exif 2.2 specification, the first 8 bytes of aGpsAreaInformation must contain a Character Code to indicate the type of encoding used. See table 6 of the Exif 2.2 specification for a list of codes.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsAreaInformation(HBufC8* aGpsAreaInformation)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001C[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetParam8(tag, ifd, aGpsAreaInformation);
	}

/**
Sets the gps date tag value of the metadata. 

@param aDateStamp
	the gps date value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDateStamp(HBufC8* aDateStamp)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001D[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;

	return iExifWrite->SetParam8(tag, ifd, aDateStamp);
	}

/**
Sets the gps differential correction tag value of the metadata. 

@param aGpsDifferential
	the gps differential correction value.
	
@return error code
	KErrArgument if the requested data type does not match with the tag.
	KErrNotSupported if called on an encoding module.
	KErrNotFound if the tag cannot be found.
	KErrOverflow if there is an internal error with the Exif metadata.
*/
EXPORT_C TInt TExifWriterUtility::SetGpsDifferential(TUint16 aGpsDifferential)
	{
	if(iExifWrite==NULL)
		{
		return KErrOverflow;
		}
	TInt tag = KTagGPS001E[ETagValue];

	// We need to specify GPS sub IFD using the upper 16 bits.
	TUint ifd = KExifGpsIfd;
	
	return iExifWrite->SetShortParam(tag, ifd, aGpsDifferential);
	}

