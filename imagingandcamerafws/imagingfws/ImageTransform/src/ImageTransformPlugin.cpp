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
#include "ImageTransformPlugin.h"
#include "ImageTransformMain.h"
#include "ImageTransformFramework.h"
#include "ImageTransformPluginExt.h"
/**
Constructor for this class.
*/
EXPORT_C CImageTransformPlugin::CImageTransformPlugin() 
	{
	}


/**
This is the destructor for the CImageTransformPlugin
and is responsible for deallocating all resources.
*/
EXPORT_C CImageTransformPlugin::~CImageTransformPlugin()
	{
	}


/**
The function NewL constructs a CImageTransformPlugin-derived ECOM plugin.

@param     aImplementationUid
           The ECOM implementation UID of the plugin
@param     aFramework
           A reference to the CImageTransformFramework object that 
		   is creating this object

@leave KErrNotFound If the plugin could not be found

@return	A pointer to a fully constructed CImageTransformPlugin
*/

EXPORT_C CImageTransformPlugin* CImageTransformPlugin::NewL(
	TUid aImplementationUid, CImageTransformFramework& aFramework)
	{
	TUid instanceUid;

	// instantiate the plugin using ECOM
	CImageTransformPlugin* self = 
		reinterpret_cast<CImageTransformPlugin*>(
			REComSession::CreateImplementationL(
				aImplementationUid,
				instanceUid));

	// point back to the framework
	self->iFramework = &aFramework;

	// hand over ownership of the Instance Uid to the framework
	// so that it can call REComSession::DestroyedImplementation() on deletion.
	self->iFramework->SetPluginInstanceUidL(instanceUid);

	return self;
	}

/**
Return whether the source is a file

@return	ETrue if the source image is stored in a file
*/
EXPORT_C TBool CImageTransformPlugin::SourceIsFilename() const
	{
	ASSERT(iFramework);
	return iFramework->SourceIsFilename();
	}

/**
Return whether the source is data

@return	ETrue if the source image is stored as data
*/
EXPORT_C TBool CImageTransformPlugin::SourceIsData() const
	{
	ASSERT(iFramework);
	return iFramework->SourceIsData();
	}


/**
Gets the source file name

@return	The source file name

@panic	EImageWrongType
		The source is not a file
*/
EXPORT_C const TDesC& CImageTransformPlugin::SourceFilename() const
	{
	ASSERT(iFramework);
	return iFramework->SourceFilename();
	}

/**
Gets the source descriptor

@return	A reference to the source descriptor

@panic	EImageWrongType
		The source is not a descriptor
*/
EXPORT_C const TDesC8& CImageTransformPlugin::SourceData() const
	{
	ASSERT(iFramework);
	return iFramework->SourceData();
	}

/**
Gets the source image's MIME type

@return	A reference to the source image's MIME type
		KNullDesC8 if not defined
*/
EXPORT_C const TDesC8& CImageTransformPlugin::SourceMimeType() const
	{
	ASSERT(iFramework);
	return iFramework->SourceMimeType();
	}

/**
Gets the source image's type

@return	The UID of the source type
		KNullUid if not defined
*/
EXPORT_C const TUid CImageTransformPlugin::SourceImageType() const
	{
	ASSERT(iFramework);
	return iFramework->SourceImageType();
	}

/**
Gets the source image's subtype

@return	The UID of the source type
		KNullUid if not defined
*/
EXPORT_C const TUid CImageTransformPlugin::SourceImageSubType() const
	{
	ASSERT(iFramework);
	return iFramework->SourceImageSubType();
	}

/**
Gets the source image's clipping region

@param	aRect
		The source image's clipping region

@return	ETrue if a clipping region has been defined
*/
EXPORT_C TBool CImageTransformPlugin::SourceRect(TRect& aRect) const
	{
	ASSERT(iFramework);
	return iFramework->SourceRect(aRect);
	}

/**
Return whether the destination image is to be stored in a file

@return	ETrue If the destination image is to be stored in a file 
*/
EXPORT_C TBool CImageTransformPlugin::DestIsFilename() const
	{
	ASSERT(iFramework);
	return iFramework->DestIsFilename();
	}

/**
Return whether the destination image is to be stored as data

@return	ETrue if the destination image is to be stored as data 
*/
EXPORT_C TBool CImageTransformPlugin::DestIsData() const
	{
	ASSERT(iFramework);
	return iFramework->DestIsData();
	}

/**
Gets the destination file name

@return The destination file name

@panic	EImageWrongType
		The destination is not a file
*/
EXPORT_C const TDesC& CImageTransformPlugin::DestFilename() const
	{
	ASSERT(iFramework);
	return iFramework->DestFilename();
	}

/**
Gets the destination descriptor

@return	A reference to a pointer to be used for storing the destination image.
		The pointer is owned by the client app and is null to start with 

@panic	EImageWrongType
		The destination is not a descriptor
*/
EXPORT_C HBufC8*& CImageTransformPlugin::DestData() const
	{
	ASSERT(iFramework);
	return iFramework->DestData();
	}


/**
Gets the requested size of the destination image

@return	The destination image size
*/
EXPORT_C const TSize& CImageTransformPlugin::DestinationSizeInPixels() const
	{
	ASSERT(iFramework);
	return iFramework->DestinationSizeInPixels();
	}

/**
Gets the options requested by the client

@return	The options requested by the client
*/
EXPORT_C TUint CImageTransformPlugin::Options() const
	{
	ASSERT(iFramework);
	return iFramework->Options();
	}

/**
Return whether the client wishes to maintain the source image's aspect ratio

@return	ETrue if the aspect ratio is to be maintained
*/
EXPORT_C TBool CImageTransformPlugin::MaintainAspectRatio() const
	{
	ASSERT(iFramework);
	return iFramework->MaintainAspectRatio();
	}

/**
Return whether the client wishes to attempt to maintain the original image data

@return	ETrue if an attempt should be made to preserve the original image data
*/
EXPORT_C TBool CImageTransformPlugin::PreserveImageData() const
	{
	ASSERT(iFramework);
	return iFramework->PreserveImageData();
	}

/**
When a plugin extension has been implemented, this function should be implemented 
by the plugin writer to enable the client access to the extension 

@return	A pointer to a fully constructed CImageTransformPluginExtension
		NULL, if there is no extension to this plugin
*/
EXPORT_C CImageTransformPluginExtension* CImageTransformPlugin::Extension() const
	{
	return NULL;
	}

/**
When a plugin extension has been implemented, this function should be implemented 
by the plugin writer to enable the client access to the extension 
@param  "aExtensionUid"		
		Uid of the required extension
@param	"aError"
		System wide error
@return	A pointer to a fully constructed CImageTransformPluginExtension
		NULL, if there is no extension to this plugin
*/
EXPORT_C CImageTransformPluginExtension* CImageTransformPlugin::Extension(TUid aExtensionUid, TInt& aError) const
	{
	CImageTransformPluginExtension* extension = Extension();
	aError = KErrNotSupported;
	if (extension && extension->Uid()==aExtensionUid)
		{
		aError = KErrNone;
		return extension;
		}
	return NULL;
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPlugin::ReservedVirtual1()
	{
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPlugin::ReservedVirtual2()
	{
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPlugin::ReservedVirtual3()
	{
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPluginExtension::ReservedVirtual1()
	{
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPluginExtension::ReservedVirtual2()
	{
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPluginExtension::ReservedVirtual3()
	{
	}

/**
Reserved for future-proofing
*/
EXPORT_C void CImageTransformPluginExtension::ReservedVirtual4()
	{
	}

