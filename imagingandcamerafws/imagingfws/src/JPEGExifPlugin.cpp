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
// ExtendedPlugin.cpp
// 
//

#include "JPEGExifPlugin.h"

//CJPEGExifDecoder
/**
Constructs a new exif decoder.

A leave occurs if there is insufficient memory available.

@return A pointer to the new exif jpeg decoder.
*/
EXPORT_C CJPEGExifDecoder* CJPEGExifDecoder::NewL()
	{
	CJPEGExifDecoder* self;
	self = new (ELeave) CJPEGExifDecoder;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Initialises an exif decoder.

A leave occurs if the associated ICL plugin could not be instanciated
*/
EXPORT_C void CJPEGExifDecoder::ConstructL()
	{
	iBody = CJPEGExifDecoder::CBody::NewL(*this);
	}

/**
@publishedPartner
@released
constructor
*/
EXPORT_C CJPEGExifDecoder::CJPEGExifDecoder()
	{
	}

/**
@publishedPartner
@released
destructor
*/
EXPORT_C CJPEGExifDecoder::~CJPEGExifDecoder()
	{
	delete iBody;
	}
	
/**
gets the metadata associated to this instance of the decoder

NULL is returned if no metadata was found

@return A pointer to the metadata
*/
EXPORT_C MExifMetadata* CJPEGExifDecoder::ExifMetadata()
	{
	return iBody->ExifMetadata();		
	}

//CJPEGExifDecoder::iBody
/**
create a new body for a defined exif jpeg decoder

A leave occurs if there is insufficient memory available.

@param  aHandle
	a reference on the exif decoder this body has to be associated to.

@return A pointer to the body.
*/
EXPORT_C CJPEGExifDecoder::CBody* CJPEGExifDecoder::CBody::NewL(CJPEGExifDecoder& aHandle)
	{
	CJPEGExifDecoder::CBody* self;
	self = new (ELeave) CJPEGExifDecoder::CBody(aHandle);
	return self;
	}

/**
constructor

instanciates a new body for a defined exif jpeg decoder
A leave occurs if there is insufficient memory available.

@param  aHandle
	a reference on the exif decoder this body has to be associated to.
*/
CJPEGExifDecoder::CBody::CBody(CJPEGExifDecoder& aHandle) : iHandle(aHandle)
	{
	}
	
/**
Gets the metadata associated with this instance of the decoder

returns NULL if no metadata was found

@return A pointer to the metadata.
*/
EXPORT_C MExifMetadata* CJPEGExifDecoder::CBody::ExifMetadata()
	{
	CJPEGExifDecoderPlugin* plugin;
	plugin = static_cast<CJPEGExifDecoderPlugin*>(iHandle.Plugin());
	return plugin->ExifMetadata();		
	}


//CJPEGExifEncoder
/**
Constructs a new exif encoder.

A leave occurs if there is insufficient memory available.

@return A pointer to the new jpeg exif encoder.
*/
EXPORT_C CJPEGExifEncoder* CJPEGExifEncoder::NewL()
	{
	CJPEGExifEncoder* self;
	self = new (ELeave) CJPEGExifEncoder;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Initialises an exif encoder.

A leave occurs if the associated ICL plugin could not be instanciated
*/
EXPORT_C void CJPEGExifEncoder::ConstructL()
	{
	iBody = CJPEGExifEncoder::CBody::NewL(*this);
	}

/**
@publishedPartner
@released
constructor
*/
EXPORT_C CJPEGExifEncoder::CJPEGExifEncoder()
	{
	}

/**
@publishedPartner
@released
destructor
*/
EXPORT_C CJPEGExifEncoder::~CJPEGExifEncoder()
	{
	delete iBody;
	}
	
/**
gets the metadata associated to this instance of the encoder

NULL is returned if no metadata was found

@return A pointer to the metadata
*/
EXPORT_C MExifMetadata* CJPEGExifEncoder::ExifMetadata()
	{
	return iBody->ExifMetadata();		
	}

//CJPEGExifEncoder::iBody
/**
create a new body for a defined exif jpeg encoder

A leave occurs if there is insufficient memory available.

@param  aHandle
	a reference on the exif encoder this body has to be associated to.

@return A pointer to the body.
*/
EXPORT_C CJPEGExifEncoder::CBody* CJPEGExifEncoder::CBody::NewL(CJPEGExifEncoder& aHandle)
	{
	CJPEGExifEncoder::CBody* self;
	self = new (ELeave) CJPEGExifEncoder::CBody(aHandle);
	return self;
	}

/**
constructor

instanciates a new body for a defined exif jpeg encoder
A leave occurs if there is insufficient memory available.

@param  aHandle
	a reference on the exif decoder this body has to be associated to.
*/
CJPEGExifEncoder::CBody::CBody(CJPEGExifEncoder& aHandle) : iHandle(aHandle)
	{
	}
	
/**
Gets the metadata associated with this instance of the decoder

returns NULL if no metadata was found

@return A pointer to the metadata.
*/
EXPORT_C MExifMetadata* CJPEGExifEncoder::CBody::ExifMetadata()
	{
	CJPEGExifEncoderPlugin* plugin;
	plugin = static_cast<CJPEGExifEncoderPlugin*>(iHandle.Plugin());
	return plugin->ExifMetadata();		
	}




/**
Gets the metadata associated with this instance of the exif transform

returns NULL if no metadata was found

@return A pointer to the metadata.
*/
EXPORT_C MExifMetadata* CJPEGExifTransformExtension::ExifMetadata()
	{
	return NULL;
	}


