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

#ifndef __EXIFJPEGTRANFORMEEXTENSION_H__
#define __EXIFJPEGTRANFORMEEXTENSION_H__

#include <e32base.h>
#include "JPEGExifPlugin.h"


class CExifJpegTransform;
class MExifMetadata;



/**
@publishedAll
@released

Extension to the Jpeg imageTransform plugin

This class is intended to offer a method to the user to retreive the exif metadata during an imageTransform operation.

*/
class CJpegTransformExtension : public CJPEGExifTransformExtension
	{
public:
	/**
	creates a new instance of CJpegTransformExtension
	
	@released

	@param  aOwner
		the plugin this extension is bound to.

	@return	The newly created extension
	*/
	static CJpegTransformExtension* NewL(const CExifJpegTransform* aOwner);

	/**
	constructor
	
	@released

	@param  aOwner
		the plugin this extension is bound to.
	*/
	CJpegTransformExtension(const CExifJpegTransform* aOwner);
	
	/**
	Getter of the Uid of this extension
	
	@released

	@return The Uid of this extension.
	*/
	const TUid Uid() const;

	/**
	Getter of the exif metadata

	@return the metadata. NULL if there is no metadata available.
	*/
	MExifMetadata* ExifMetadata();

private:
	const CExifJpegTransform* iOwner;
	};

#endif // __EXIFJPEGTRANFORMEEXTENSION_H__
