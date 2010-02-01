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

#include "ImageTransform.h"
#include "ImageTransformFramework.h"
#include "ImageTransformMain.h"

/**
Constructs a CImageTransform object.

@param	aFs
        A reference to a file server session for the transform API to use.
@return	A pointer to a fully constructed CImageTransform.

*/
EXPORT_C CImageTransform* CImageTransform::NewL(RFs& aFs)
	{
	CImageTransform* self = new(ELeave) CImageTransform();
	CleanupStack::PushL(self);
	self->ConstructL(aFs);
	CleanupStack::Pop(self);
	return self;
	}

/**
Constructor for this class.
*/
CImageTransform::CImageTransform()
	{
	}


/**
Performs second phase of contruction

@param	aFs
        A reference to a file server session for the transform API to use.
*/
void CImageTransform::ConstructL(RFs& aFs)
	{
	iBody = CImageTransformFramework::NewL(*this, aFs);
	}


/**
This is the destructor for this class
and is responsible for deallocating all resources
*/

EXPORT_C CImageTransform::~CImageTransform()
	{
	delete iBody;
	}

/**
Specifies the UID of the image transform plugin to load

@param	aPluginUid
        The plugin's implementation UID
@leave	KErrArgument
		The UID is KNullUid
*/
EXPORT_C void CImageTransform::SetPluginUidL(TUid aPluginUid)
	{
	ASSERT(iBody);
	iBody->SetPluginUidL(aPluginUid);
	}

/**
Specifies the name of the source file containing the image to transform

@param	aFilename
        The source filename
@leave	KNotFound
		The file does not exist
*/
EXPORT_C void CImageTransform::SetSourceFilenameL(const TDesC& aFilename)
	{
	ASSERT(iBody);
	iBody->SetSourceFilenameL(aFilename);
	}

/**
Specifies the source descriptor containing the image to transform.
Note that this descriptor must persist while the the image is transformed.
i.e. the transformation operation does not make a copy of the data. 
Special care must be taken when passing a TPtrC8 or TPtr8 to this function:
The TPtrC8/TPtr8 itself (as well as the data it points to) must be persistent.

@param	aData
        The source descriptor
@leave	KErrArgument
		The length is zero
*/
EXPORT_C void CImageTransform::SetSourceDataL(const TDesC8& aData)
	{
	ASSERT(iBody);
	iBody->SetSourceDataL(aData);
	}

/**
Specifies the MIME type of the source image.

@param	aMIMEType
        The MIME type of the source image 
@leave	KErrArgument
		The length is zero
*/
EXPORT_C void CImageTransform::SetSourceMimeTypeL(const TDesC8& aMIMEType)
	{
	ASSERT(iBody);
	iBody->SetSourceMimeTypeL(aMIMEType);
	}

/**
Specifies the source image's type and (optionally) its subtype

@param	aImageType
        The UID of the source image's type
@param	aImageSubType
        The UID of the source image's subtype
@leave	KErrArgument
		aImageType is KNullUid
*/
EXPORT_C void CImageTransform::SetSourceImageTypeL(TUid aImageType, TUid aImageSubType)
	{
	ASSERT(iBody);
	iBody->SetSourceImageTypeL(aImageType, aImageSubType);
	}

/**
Defines a clipping region. Only the specified region will be converted.
It is not supported by the Symbian Jpeg ImageTransform Plugin.

@param	aRect
        The coordinates of the clipping region
*/
EXPORT_C void CImageTransform::SetSourceRect(const TRect& aRect)
	{
	ASSERT(iBody);
	iBody->SetSourceRect(aRect);
	}

/**
Clears the clipping region. 
*/
EXPORT_C void CImageTransform::ClearSourceRect()
	{
	ASSERT(iBody);
	iBody->ClearSourceRect();
	}

/**
Specifies the name of the destination file where the transformed image
is to be written to.

@param	aFilename
        The destination filename
@leave	KArgument
		The length is zero
*/
EXPORT_C void CImageTransform::SetDestFilenameL(const TDesC& aFilename)
	{
	ASSERT(iBody);
	iBody->SetDestFilenameL(aFilename);
	}

/**
Defines the destination descriptor. 
This is a reference to a pointer that is owned by the client app. 
The pointer should be NULL; the plugin will take responsibility for 
allocating (and potentially growing) the descriptor. 

@param	aData
		The destination descriptor.
@leave	KErrArgument
		The pointer is not NULL
*/
EXPORT_C void CImageTransform::SetDestDataL(HBufC8*& aData)
	{
	ASSERT(iBody);
	iBody->SetDestDataL(aData);
	}

/**
Specifies the requested destination image size. 

@param	aDestinationSize
		The requested size of the destination size in pixels.
@param	aMaintainAspectRatio
		Requests that the aspect ratio be maintained as far as possible.
		Defaults to ETrue.
@leave	KErrArgument
		The destination size is zero.

*/
EXPORT_C void CImageTransform::SetDestSizeInPixelsL(const TSize& aDestinationSize, TBool aMaintainAspectRatio)
	{
	ASSERT(iBody);
	iBody->SetDestSizeInPixelsL(aDestinationSize, aMaintainAspectRatio);
	}

/**
Defines the options for the image transformation. The specified option must be one of the 
options supported or a combination of them, see CImageTransform::TOptions.

@param	aOptions 
		The required option or combination of options from CImageTransform::TOptions
@leave	KErrArgument
		One or more of the specified option flags is invalid
*/
EXPORT_C void CImageTransform::SetOptionsL(TUint aOptions)
	{
	ASSERT(iBody);
	iBody->SetOptionsL(aOptions);
	}

/**
Defines the transformations to be supported by the desired plug-in. The specified transformation must be one of the 
transformations specified in CImageTransform::TTransformations or combination of them, see CImageTransform::TTransformations.

@param	aTransformations 
		The required transformation or combination of transformations from CImageTransform::TTransformations.
@leave	KErrArgument
		One or more of the specified transformation flags is invalid.
*/
EXPORT_C void CImageTransform::SetTransformationsL(TUint aTransformations)
	{
	ASSERT(iBody);
	iBody->SetTransformationsL(aTransformations);
	}

/**
Specifies that either the original image data is to preserved, as far as possible, 
or that the image data should be re-encoded to produce a more size-efficient image.
@param	aPreserveImageData Requests that the aspect ratio be preserved
		                   Defaults to false
*/
EXPORT_C void CImageTransform::SetPreserveImageData(TBool aPreserveImageData)
	{
	ASSERT(iBody);
	iBody->SetPreserveImageData(aPreserveImageData);
	}

/**
Requests that a suitable plugin be selected and instantiated. 

@leave	KErrNotFound
		No suitable plugin was found

@leave KErrNotReady
		A required parameter was not set before this call.

@leave	KErrArgument
		A parameter incompatible with the transformation options was previously set.
*/
EXPORT_C void CImageTransform::SetupL()
	{
	ASSERT(iBody);
	iBody->SetupL();
	}

/**
Initiates an asynchronous image transform operation. 
The framework will panic if no plugin has been instantiated already.

@param	aStatus
		Request status to signal when scaling is complete.
@panic	EIllegalCallSequence
		No plugin loaded.
*/
EXPORT_C void CImageTransform::Transform(TRequestStatus& aStatus)
	{
	ASSERT(iBody);
	iBody->Transform(aStatus);
	}

/**
Cancels any asynchronous image transform operation currently in progress.
*/
EXPORT_C void CImageTransform::CancelTransform()
	{
	ASSERT(iBody);
	iBody->CancelTransform();
	}

/**
Resets all SetXXX() calls so that the state is the same as that immediately 
after a call to NewL(). Deletes the plugin if one is loaded.
*/
EXPORT_C void CImageTransform::Reset()
	{
	ASSERT(iBody);
	iBody->Reset();
	}

/**
Gets a pointer to a plugin extension
@panic	EIllegalCallSequence
		No plugin loaded.
@return	A pointer to a fully constructed CImageTransformPluginExtension
		NULL, if there is no extension to the plugin
*/
EXPORT_C CImageTransformPluginExtension* CImageTransform::Extension() const
	{
	ASSERT(iBody);
	return iBody->Extension();
	}

/**
Gets a pointer to a plugin extension
@panic	EIllegalCallSequence
		No plugin loaded.
@param  "aExtensionUid"		
		Uid of the required extension
@param	"aError"
		System wide error
@return	A pointer to a fully constructed CImageTransformPluginExtension
		NULL, if there is no extension to the plugin
*/
EXPORT_C CImageTransformPluginExtension* CImageTransform::Extension(TUid aExtensionUid, TInt& aError) const
	{
	ASSERT(iBody);
	return iBody->Extension(aExtensionUid, aError);
	}
