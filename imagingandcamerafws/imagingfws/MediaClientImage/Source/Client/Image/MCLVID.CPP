// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <bitdev.h>
#include <mdaimageconverter.h>
#include "MdaImageConverter.inl"
#include <ecom/ecom.h>
#include "CompatAids.h"

#if defined(__ECOM_POWERDOWN_WORKAROUND)
//	const TInt KWaitTimeMax = 8;						// EABI warning removal
//	const TInt KWaitTimeIncrement = 250000; // 0.25s	// EABI warning removal
#endif  // defined(__ECOM_POWERDOWN_WORKAROUND)

GLREF_C void Panic (TInt aPanic);

//
// CMdaImageUtility
//

CMdaImageUtility::CMdaImageUtility() : CActive(CActive::EPriorityStandard)
	{
	}

CMdaImageUtility::~CMdaImageUtility()
	{
	delete iProperties;
	}

/**
Closes the open file-based or descriptor-based image.

Any conversion operation in progress is cancelled.
*/
EXPORT_C void CMdaImageUtility::Close()
	{
	ASSERT(iProperties);
	iProperties->Close();
	}

void CMdaImageUtility::DoClose()
	{
	ASSERT(iProperties);
	iProperties->DoClose();
	}

void CMdaImageUtility::SelfComplete(TInt aError)
	{
	TRequestStatus *status = &iStatus;
	SetActive();
	User::RequestComplete(status, aError);
	}

/**
Retrieves information about the specified frame.

This function should only be called after an existing image has been
successfully opened or a new image successfully created.

@param  aFrame
        The frame number. This value is relative to zero. The
        value must be less than the total number of frames in
        the image, otherwise the function raises a USER 0
        panic
@param  aInfo
        A reference to the frame information.
*/
EXPORT_C void CMdaImageUtility::FrameInfo(TInt aFrame, TFrameInfo& aInfo) const
	{
	ASSERT(iProperties);
	iProperties->FrameInfo(aFrame, aInfo);
	}

/**
Returns the number of frames in the image, counting from the first frame in the image, to the next frame to be decoded.

Note:
The image conversion utilities do not scan all the image data on calling OpenL(), just the header. Therefore, if, as in 
the case of an animated gif, there is no frame count in the header, then until all the frames in the image have been 
decoded, the total frame count is unknown. Initially therefore, FrameCount() returns 1, but this increments as each frame 
is decoded, and the utility reads the next frame's header. Once the last frame has been decoded, all frames are available 
for access as the information about each frame is stored.

@return The number of frames in the image
*/
EXPORT_C TInt CMdaImageUtility::FrameCount() const
	{
	ASSERT(iProperties);
	return iProperties->FrameCount();
	}

void CMdaImageUtility::ImageUtil_Reserved1()
	{}

void CMdaImageUtility::ImageUtil_Reserved2()
	{}

void CMdaImageUtility::ImageUtil_Reserved3()
	{}

void CMdaImageUtility::ImageUtil_Reserved4()
	{}

void CMdaImageUtility::DoCancel()
	{
	iProperties->DoCancel();
	}

void CMdaImageUtility::MoscoStateChangeEvent(CBase* /*aObject*/, TInt /*aPreviousState*/,
											 TInt /*aCurrentState*/, TInt /*aErrorCode*/)
	{
	__PANIC_COMPAT(Panic(EMDAImConvPanicNotImplemented));
	}

//
// CMdaImageDataReadUtility
//

/** 
Default destructor.
*/
CMdaImageDataReadUtility::~CMdaImageDataReadUtility()
	{
	}

EXPORT_C void CMdaImageDataReadUtility::ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber)
	{
	ASSERT(Properties());
	Properties()->ConvertL(aBitmap, aFrameNumber);
	}

EXPORT_C void CMdaImageDataReadUtility::ConvertL(CFbsBitmap& aBitmap,CFbsBitmap& aMaskBitmap,TInt aFrameNumber)
	{
	ASSERT(Properties());
	Properties()->ConvertL(aBitmap, aMaskBitmap, aFrameNumber);
	}

EXPORT_C void CMdaImageDataReadUtility::CancelConvertL()
	{
	ASSERT(Properties());
	Properties()->CancelConvertL();
	}

void CMdaImageDataReadUtility::RunL()
	{
	ASSERT(Properties());
	Properties()->RunL();
	}

void CMdaImageDataReadUtility::DoCancel()
	{
	ASSERT(Properties());
	Properties()->DoCancel();
	}

/**
Returns a particular comment attached to a given frame of the image
This function has been superceded by the parallel one in CImageDecoder,
as this assumes that each frame has a comment within it, which is not always the case.
@param aFrameNumber The Frame Number within the image from which to retrieve the specified comment
@return The specified comment
*/

EXPORT_C const TDesC& CMdaImageDataReadUtility::FrameCommentL(TInt aFrameNumber)
	{
	ASSERT(Properties());
	return Properties()->FrameCommentL(aFrameNumber);
	}

// Used to ensure ordinals in MediaClientImage.def file remain unchanged
// CMdaImageDataReadUtility::SourcePort() was removed from the public API
EXPORT_C void ImageReadUtilSourcePort_Reserved()
	{Panic(EMDAImConvPanicNotImplemented);}

void CMdaImageDataReadUtility::ImageReadUtil_Reserved2()
	{}

void CMdaImageDataReadUtility::ImageReadUtil_Reserved3()
	{}

void CMdaImageDataReadUtility::ImageReadUtil_Reserved4()
	{}

void CMdaImageDataReadUtility::DoClose()
	{
	ASSERT(Properties());
	Properties()->DoClose();
	}

//
// CMdaImageDataWriteUtility
//

CMdaImageDataWriteUtility::~CMdaImageDataWriteUtility()
	{
	}

/**
@internalComponent
*/
EXPORT_C void CMdaImageDataWriteUtility::ConvertL(CFbsBitmap& aBitmap,TInt aFrameNumber)
	{
	ASSERT(Properties());
	Properties()->ConvertL(aBitmap, aFrameNumber);
	}
/**
@param	aBitmap
		Not used.
@param	aSourceRect
		Not used.
@param	aFrameNumber
		Not used.
*/
EXPORT_C void CMdaImageDataWriteUtility::ConvertL(CFbsBitmap& /*aBitmap*/,const TRect& /*aSourceRect*/,TInt /*aFrameNumber*/)
	{
	User::Leave(KErrNotSupported);
	}
/**
@see    CMdaImageUtility::ConvertL(CFbsBitmap& aBitmap,CFbsBitmap& aMaskBitmap,TInt aFrameNumber)

@param	aBitmap
		Not used.
@param	aMaskBitmap
		Not used.
@param	aFrameNumber
		Not used.
*/
EXPORT_C void CMdaImageDataWriteUtility::ConvertL(CFbsBitmap& /*aBitmap*/,CFbsBitmap& /*aMaskBitmap*/,TInt /*aFrameNumber*/)
	{
	User::Leave(KErrNotSupported);
	}

EXPORT_C void CMdaImageDataWriteUtility::CancelConvertL()
	{
	ASSERT(Properties());
	Properties()->CancelConvertL();
	}

void CMdaImageDataWriteUtility::RunL()
	{
	ASSERT(Properties());
	Properties()->RunL();
	}

void CMdaImageDataWriteUtility::DoCancel()
	{
	ASSERT(Properties());
	Properties()->DoCancel();
	}

CImageEncoder* CMdaImageDataWriteUtility::Encoder() const
	{
	ASSERT(Properties());
	return Properties()->Encoder();
	}

void CMdaImageDataWriteUtility::ImageWriteUtil_Reserved1()
	{}

void CMdaImageDataWriteUtility::ImageWriteUtil_Reserved2()
	{}

void CMdaImageDataWriteUtility::ImageWriteUtil_Reserved3()
	{}

void CMdaImageDataWriteUtility::DoClose()
	{
	ASSERT(Properties());
	Properties()->DoClose();
	}

//
// CMdaImageFileToBitmapUtility
//

/**
Constructs a new instance of the file based image to bitmap conversion
utility object.

The function leaves if this object cannot be created.

@param  aObserver
        The image utility observer interface
@param  aServer
        A pointer to a server object. The default is NULL
        which means that one is automatically constructed and
        owned by this conversion utility object. If not NULL,
        the client must construct, and be responsible for, the
        server object. This option may be more efficient if a
        number of conversion utility objects are to be
        created.
@return A pointer to the new instance of the file based image to
        bitmap conversion utility.
*/
EXPORT_C CMdaImageFileToBitmapUtility* CMdaImageFileToBitmapUtility::NewL(MMdaImageUtilObserver& aObserver,CMdaServer* /*aServer*/)
	{
	CMdaImageFileToBitmapUtility* self = new(ELeave) CMdaImageFileToBitmapUtility();
	CleanupStack::PushL(self);
	self->iProperties = CMdaImageFileToBitmapUtilityPriv::NewL(aObserver, self);
	CleanupStack::Pop(); // self
	return self;
	}

/**
Opens the existing file based image.

The function is asynchronous. When the operation is complete,
successfully or otherwise, the callback function
MMdaImageUtilObserver::MiuoOpenComplete() is called.Open
must complete successfully before starting any conversion operation.
The function can complete with KErrUnderflow, if there is
insufficient information in the file. Typically, this occurs when
using the file as a cache. In this situation, open should be performed
repeatedly until the file has accumulated enough information for the
operation to complete with KErrNone.NotesThe optional
parameters can be used to force the Media Server to interpret the file
as being of a specific type. This is necessary for formats such as WAP
wireless bitmap where the supplied file does not contain a uniquely
recognisable signature.

@param  aFileName
        The full path name of the file.
@param  aFormat
        The data format. This is optional.
@param  aCodec
        The codec to use. This is optional.
@param  aExtra
        Any additional information which the Media Server may need
        to recognise the data format. This is optional.
*/
EXPORT_C void CMdaImageFileToBitmapUtility::OpenL(const TDesC& aFileName,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	ASSERT(Properties());
	Properties()->OpenL(aFileName, aFormat, aCodec, aExtra);
	}


//
// CMdaImageDescToBitmapUtility
//

/**
Constructs a new instance of the descriptor-based image to bitmap
conversion utility object.

The function leaves if this object cannot be created.

@param  aObserver
        The image utility observer interface.
@param  aServer
        A pointer to a server object. The default is NULL
        which means that one is automatically constructed and
        owned by this conversion utility object. If not NULL,
        the client must construct, and be responsible for, the
        server object. This option may be more efficient if a
        number of conversion utility objects are to be
        created.
@return A pointer to the new instance of the descriptor-based
        image to bitmap conversion utility
*/
EXPORT_C CMdaImageDescToBitmapUtility* CMdaImageDescToBitmapUtility::NewL(MMdaImageUtilObserver& aObserver,CMdaServer* /*aServer*/)
	{
	CMdaImageDescToBitmapUtility* self = new(ELeave) CMdaImageDescToBitmapUtility();
	CleanupStack::PushL(self);
	self->iProperties = CMdaImageDescToBitmapUtilityPriv::NewL(aObserver, self);
	CleanupStack::Pop(); // self
	return self; 
	}

/**
Opens the descriptor-based image.

The function is asynchronous. When the open operation is complete,
successfully or otherwise, the callback function
MMdaImageUtilObserver::MiuoOpenComplete() is called.Open
must complete successfully before starting any conversion operation.
The function can complete with KErrUnderflow, if there is
insufficient information in the descriptor. Typically, this occurs
when using the descriptor as a cache. In this situation, open should
be performed repeatedly until the descriptor has accumulated enough
information for the operation to complete with
KErrNone.

Note:
The optional parameters can be used to
force the Media Server to interpret the descriptor-based data as being
of a specific type. This is necessary for formats such as WAP wireless
bitmap where the supplied descriptor does not contain a uniquely
recognisable signature.

@param  aDescriptor
        A descriptor containing the image.
@param  aFormat
        The data format. This is optional.
@param  aCodec
        The codec to use. This is optional.
@param  aExtra
        Additional information which the Media Server may need
        to recognise the data format. This is optional.
*/
EXPORT_C void CMdaImageDescToBitmapUtility::OpenL(const TDesC8& aDescriptor,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	ASSERT(Properties());
	Properties()->OpenL(aDescriptor, aFormat, aCodec, aExtra);
	}

//
// CMdaImageBitmapToFileUtility
//

/**
Constructs a new instance of the bitmap to file-based image conversion
utility.

The function leaves if this object cannot be created.

@param  aObserver
        The image utility observer interface
@param  aServer
        A pointer to a server object. The default is NULL
        which means that one is automatically constructed and
        owned by this conversion utility object. If not NULL,
        the client must construct, and be responsible for, the
        server object. This option may be more efficient if a
        number of conversion utility objects are to be
        created.
@return A pointer to the new instance of the bitmap to
        file-based image conversion utility.
*/
EXPORT_C CMdaImageBitmapToFileUtility* CMdaImageBitmapToFileUtility::NewL(MMdaImageUtilObserver& aObserver,CMdaServer* /*aServer*/)
	{
	CMdaImageBitmapToFileUtility* self = new(ELeave) CMdaImageBitmapToFileUtility();
	CleanupStack::PushL(self);
	self->iProperties = CMdaImageBitmapToFileUtilityPriv::NewL(aObserver, self);
	CleanupStack::Pop(); // self
	return self;
	}

/**
Creates a new file to receive the file-based image.

The function is asynchronous. When the operation is complete,
successfully or otherwise, the callback function
MMdaImageUtilObserver::MiuoCreateComplete() is called. The
file must be have been created before any conversion operation can
start. Note, however, that the file will expand to accommodate the
data i.e. image data is not truncated if the original size of the
file is too small.

@param  aFileName
        The full path name of the file to be created.
@param  aFormat
        The data format.
@param  aCodec
        The codec to use.
@param  aExtra
        Any additional information which the Media Server may need
        to recognise the data format.
*/
EXPORT_C void CMdaImageBitmapToFileUtility::CreateL(const TDesC& aFileName,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	ASSERT(Properties());
	Properties()->CreateL(aFileName, aFormat, aCodec, aExtra);
	}

//
// CMdaImageBitmapToDescUtility
//

/**
Constructs a new instance of the bitmap to descriptor-based image
conversion utility.

The function leaves if this object cannot be created.

@param  aObserver
        The image utility observer interface
@param  aServer
        A pointer to a server object. The default is NULL
        which means that one is automatically constructed and
        owned by this conversion utility object. If not NULL,
        the client must construct, and be responsible for, the
        server object. This option may be more efficient if a
        number of conversion utility objects are to be
        created.
@return A pointer to the new instance of the bitmap to
        descriptor-based image conversion utility.
*/
EXPORT_C CMdaImageBitmapToDescUtility* CMdaImageBitmapToDescUtility::NewL(MMdaImageUtilObserver& aObserver,CMdaServer* /*aServer*/)
	{
	CMdaImageBitmapToDescUtility* self = new(ELeave) CMdaImageBitmapToDescUtility();
	CleanupStack::PushL(self);
	self->iProperties = CMdaImageBitmapToDescUtilityPriv::NewL(aObserver, self);
	CleanupStack::Pop();
	return self;
	}

/**
Initialises the supplied descriptor to receive the descriptor-based
image.

The function is asynchronous. When the operation is complete,
successfully or otherwise, the callback function
MMdaImageUtilObserver::MiuoCreateComplete() is called.The
descriptor must have been successfully initialised before any
conversion operation can start.

@param     aDescriptor
           A descriptor to contain the image.
@param     aFormat
           The data format. This is optional.
@param     aCodec
           The codec to use. This is optional.
@param     aExtra
           Additional information which the Media Server may need
           to recognise the data format. This is optional.
*/
EXPORT_C void CMdaImageBitmapToDescUtility::CreateL(TDes8& aDescriptor,TMdaClipFormat* aFormat,TMdaPackage* aCodec,TMdaPackage* aExtra)
	{
	ASSERT(Properties());
	Properties()->CreateL(aDescriptor, aFormat, aCodec, aExtra);
	}

/**
Constructs and initialises a new instance of the bitmap converter.

The function leaves if the bitmap converter object cannot be created.

@param  aObserver
        The image utility observer interface.
@param  aServer
        A pointer to a server object. The default is NULL which means that one is automatically constructed
        and owned by this conversion utility object. If not NULL, the client must construct, and be responsible
        for, the server object. This option may be more efficient if a number of conversion utility objects
        are to be created.

@return A pointer to the new bitmap converter object.
*/
EXPORT_C CMdaImageBitmapToBitmapUtility* CMdaImageBitmapToBitmapUtility::NewL(MMdaImageUtilObserver& aObserver, CMdaServer* /* aServer */)
	{
	CMdaImageBitmapToBitmapUtility* self = new(ELeave) CMdaImageBitmapToBitmapUtility();
	CleanupStack::PushL(self);
	self->iProperties = CMdaImageBitmapToBitmapUtilityPriv::NewL(aObserver, self);
	CleanupStack::Pop(); // self
	return self;
	}

/**
Destructor.
*/
EXPORT_C CMdaImageBitmapToBitmapUtility::~CMdaImageBitmapToBitmapUtility()
	{
	}

/**
Opens the bitmap converter utility.

The function is asynchronous. When the open operation is complete, successfully or otherwise, the callback
function MMdaImageUtilObserver::MiuoOpenComplete() is called.

The source bitmap must be open before any conversion operation can start.

@param  aBitmap
        The bitmap to be used as the source for subsequent conversions.
*/
EXPORT_C void CMdaImageBitmapToBitmapUtility::OpenL(CFbsBitmap& aBitmap)
	{
	ASSERT(Properties());
	Properties()->OpenL(aBitmap);
	}

/**
Converts a bitmap.

The operation is asynchronous. When the conversion operation is complete, successfully or 
otherwise, the callback function MMdaImageUtilObserver::MiuoConvertComplete() is called.

@param  aBitmap
        The destination bitmap.
@param  aFrameNumber
        The frame number. This value is relative to zero.
*/
EXPORT_C void CMdaImageBitmapToBitmapUtility::ConvertL(CFbsBitmap& aBitmap, TInt aFrameNumber)
	{
	ASSERT(Properties());
	Properties()->ConvertL(aBitmap, aFrameNumber);
	}

/**
Converts a bitmap with a mask. Not supported

@param  aBitmap
        A reference to the bitmap which is the target of the conversion operation.
@param  aMaskBitmap
        The overloaded variant which takes this argument is not supported for bitmap to bitmap
        conversion. This variant leaves with KErrNotSupported.
@param  aFrameNumber
        The frame number. This value is relative to zero.

@since	Not supported
*/
EXPORT_C void CMdaImageBitmapToBitmapUtility::ConvertL(CFbsBitmap& /* aBitmap */, CFbsBitmap& /* aMaskBitmap */, TInt /* aFrameNumber = 0 */)
	{
	User::Leave(KErrNotSupported);
	}

/**
Cancels any conversion in progress.

The callback function MMdaImageUtilObserver::MiuoConvertComplete() is not called.
*/
EXPORT_C void CMdaImageBitmapToBitmapUtility::CancelConvertL()
	{
	ASSERT(Properties());
	Properties()->CancelConvertL();
	}

void CMdaImageBitmapToBitmapUtility::RunL()
	{
	ASSERT(Properties());
	Properties()->RunL();
	}

/**
Implements cancellation of an outstanding request.
*/
void CMdaImageBitmapToBitmapUtility::DoCancel()
	{
	ASSERT(Properties());
	Properties()->DoCancel();
	}


void CMdaImageBitmapToBitmapUtility::DoClose()
	{
	Properties()->DoClose();
	}

