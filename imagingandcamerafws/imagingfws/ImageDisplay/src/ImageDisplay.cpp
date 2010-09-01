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

#include "ImageDisplay.h"
#include "ImageDisplayFramework.h"
#include "ImageDisplayMain.h"

/**
Constructs a CImageDisplay object.

@param	aFs
        A reference to a file server session for the display API to use.
@param	aCallback
        A reference to a Image Display observer object which would recieve status notifications
@return	A pointer to a fully constructed CImageDisplay.

*/
EXPORT_C CImageDisplay* CImageDisplay::NewL(MIclImageDisplayObserver& aCallback, RFs& aFs)
	{
	CImageDisplay* self = new(ELeave) CImageDisplay();
	CleanupStack::PushL(self);
	self->ConstructL(aCallback, aFs);
	CleanupStack::Pop(self);
	return self;
	}

/**
Constructor for this class.
*/
CImageDisplay::CImageDisplay()
	{
	}


/**
Performs second phase of construction

@param	aCallback
		A reference to an observer interface implementation

@param	aFs
        A reference to a file server session for the display API to use.
*/
void CImageDisplay::ConstructL(MIclImageDisplayObserver& aCallback, RFs& aFs)
	{
	iBody = CImageDisplayFramework::NewL(*this, aCallback, aFs);
	}


/**
This is the destructor for this class
and is responsible for deallocating all resources
*/

EXPORT_C CImageDisplay::~CImageDisplay()
	{
	delete iBody;
	}

/**
Specifies the UID of the image display plugin to load

@param	aPluginUid
        The plugin's implementation UID

@panic ImageDisplay 1 EIllegalCallSequence Raised when a plugin is already loaded 
*/
EXPORT_C void CImageDisplay::SetPluginUid(TUid aPluginUid)
	{
	ASSERT(iBody);
	iBody->SetPluginUid(aPluginUid);
	}

/**
Specifies the source of the image to display

@param	aSource
        The data source container. Note that the framework doesn't take a copy of actual data/filename etc.
		and they must persist during decoding
@return KErrNone if the given source type is supported, otherwise KErrNotSupported
@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage
*/
EXPORT_C TInt CImageDisplay::SetImageSource(const TMMSource& aSource)
	{
	ASSERT(iBody);
	return iBody->SetImageSource(aSource);
	}

/**
Specifies the MIME type of the source image.

@param	aMIMEType
        The MIME type of the source image 

@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage
*/
EXPORT_C void CImageDisplay::SetSourceMimeType(const TDesC8& aMIMEType)
	{
	ASSERT(iBody);
	iBody->SetSourceMimeType(aMIMEType);
	}

/**
Specifies the source image's type and (optionally) its subtype

@param	aImageType
        The UID of the source image's type
@param	aImageSubType
        The UID of the source image's subtype

@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage or
		a plugin is not loaded yet
*/
EXPORT_C void CImageDisplay::SetSourceImageType(TUid aImageType, TUid aImageSubType)
	{
	ASSERT(iBody);
	iBody->SetSourceImageType(aImageType, aImageSubType);
	}

/**
Defines a clipping region. Only the specified region will be processed.

@param	aRect
        The coordinates of the clipping region
		
@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage or
		a plugin is not loaded yet
*/
EXPORT_C void CImageDisplay::SetSourceRect(const TRect& aRect)
	{
	ASSERT(iBody);
	iBody->SetSourceRect(aRect);
	}

/**
Clears settings for the clipping region, but not the image region

@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage or
		a plugin is not loaded yet
*/
EXPORT_C void CImageDisplay::ResetSourceRect()
	{
	ASSERT(iBody);
	iBody->ResetSourceRect();
	}

/**
Specifies the requested image size. 

@param	aDestinationSize
		The requested size of the image in pixels
@param	aMaintainAspectRatio
		Requests that the aspect ratio be maintained as far as possible
		Defaults to true

@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage or
		a plugin is not loaded yet
*/
EXPORT_C void CImageDisplay::SetSizeInPixels(const TSize& aDestinationSize, TBool aMaintainAspectRatio)
	{
	ASSERT(iBody);
	iBody->SetSizeInPixels(aDestinationSize, aMaintainAspectRatio);
	}

/**
Specifies the requested image display mode. 

@param	aDisplayMode
		The requested display mode

@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage or
		a plugin is not loaded yet

*/
EXPORT_C void CImageDisplay::SetDisplayMode(TDisplayMode aDisplayMode)
	{
	ASSERT(iBody);
	iBody->SetDisplayMode(aDisplayMode);
	}

/**
Defines how the image is to be displayed. 

Note that all plugins support EOptionMainImage, whereas the availability of the other options depends on 
the image display plugin loaded.

@param	aOptions - image type selected using the TImageOptions flag set

@return KErrArgument on wrong combination of options, especially if the rotate and 
		mirror options aren't set together with EOptionThumbnail or EOptionMainImage.

@panic ImageDisplay 1 EIllegalCallSequence Raised if the function was called during decoding stage or
		a plugin is not loaded yet
*/
EXPORT_C TInt CImageDisplay::SetOptions(TUint aOptions)
	{
	ASSERT(iBody);
	return iBody->SetOptions(aOptions);
	}

/**
Requests that a suitable plugin be selected and instantiated. 
Leaves with KErrNotFound if no suitable plugin found or if the framework 
or plugin finds any error in any of the preceeding SetXXX() calls

@leave	KErrNotFound
		No suitable plugin was found
@leave	KErrArgument
		One of the previously supplied parameters (image size, mime type etc.) is invalid

@panic ImageDisplay 2 EUndefinedSourceType Raised when image source is not defined yet

*/
EXPORT_C void CImageDisplay::SetupL()
	{
	ASSERT(iBody);
	iBody->SetupL();
	}

/**
Initiates an image display operation. 
The framework will panic if no plugin has been instantiated already.
Note: That a plugin may perform some asynchronous operations within the 
current thread, so yielding to the Active Scheduler after calling this
function is almost mandatory

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded.
		
@panic ImageDisplay 3 EUndefinedDestSize 
		Raised when destination image size is not defined yet
*/
EXPORT_C void CImageDisplay::Play()
	{
	ASSERT(iBody);
	iBody->Play();
	}

/**
Instructs a plug-in to pause its operation. An operation can be resumed
by calling the Play()
The framework will panic if no plugin has been instantiated already and
Play() has not been called yet.

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded.
		
*/
EXPORT_C void CImageDisplay::Pause()
	{
	ASSERT(iBody);
	iBody->Pause();
	}
	
/**
Cancels any image display operation currently in progress.

@panic	ImageDisplay 1 EIllegalCallSequence Raised when no plugin loaded yet.
*/
EXPORT_C void CImageDisplay::StopPlay()
	{
	ASSERT(iBody);
	iBody->StopPlay();
	}

/**
Resets all SetXXX() calls so that the state is the same as that immediately 
after a call to NewL(). Deletes the plugin if one is loaded.
*/
EXPORT_C void CImageDisplay::Reset()
	{
	ASSERT(iBody);
	iBody->Reset();
	}

/**
Gets a pointer to a plugin extension
@param  aIFaceUid
		 Requested extension interface Uid
@param  aIFacePtr
		 Reference to pointer which would have interface pointer on sucessful completion, otherwise NULL
@return	KErrNotSupported if given extension interface is not supported
		or other system-wide error code
@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded.
*/
EXPORT_C TInt CImageDisplay::ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr)
	{
	ASSERT(iBody);
	return iBody->ExtensionInterface(aIFaceUid, aIFacePtr);
	}

/**
Gets a reference to the current bitmap and mask

@param  aBitmap
		 Reference to pointer which would have current frame bitmap address.
@param	aMask
		 Reference to pointer which would have current frame mask address.  This is NULL if no mask
		 is available or if aBitmap has a display mode of EColor16MA (see SetDisplayMode()).

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded or decoding was not started

@see	SetDisplayMode
*/
EXPORT_C void CImageDisplay::GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const
	{
	ASSERT(iBody);
	iBody->GetBitmap(aBitmap, aMask);
	}

/**
Check to see if the current frame can be displayed now

@return ETrue 
		The current bitmap contains a valid image. It will return ETrue only when it is called in between the ImageReady() callback 
		and a subsequent call to Play().

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded or decoding was not started
*/
EXPORT_C TBool CImageDisplay::ValidBitmap() const
	{
	ASSERT(iBody);
	return iBody->ValidBitmap();		
	}

/**
Returns an array of recommended image sizes i.e. sizes which would be processed faster 
@return a reference to the array of recommended image sizes

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded.
*/
EXPORT_C const RArray<TSize>& CImageDisplay::RecommendedImageSizes() const
	{
	ASSERT(iBody);
	return iBody->RecommendedImageSizes();
	}

/**
returns an integer, corresponding to the current image status (if known), the value will be comprised of elements of the TImageStatus bit field
@return a combination TImageStatus flags

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded.
*/
EXPORT_C TUint CImageDisplay::ImageStatus() const
	{
	ASSERT(iBody);
	return iBody->ImageStatus();
	}

/**
returns the number of frames (if known) or an error code if unknown.
@param  aNumFrames
		 a reference to frame number parameter which has meaning only if retrun value is KErrNone

@return KErrNotSupported if a plug-in can't determine number of frames

@panic	ImageDisplay 1 EIllegalCallSequence
		No plugin loaded.
*/
EXPORT_C TInt CImageDisplay::NumFrames(TInt& aNumFrames) const
	{
	ASSERT(iBody);
	return iBody->NumFrames(aNumFrames);
	}

const ContentAccess::TIntent KCCaccesUnknown=ContentAccess::EUnknown; 

/**
Constructor for the descriptor-based image datasource.
NOTE: Data is not copied, so the original buffer must be kept during image processing
@param  aData
		 a reference to the pointer containing an image data

*/
EXPORT_C 
TDescriptorDataSource::TDescriptorDataSource(const TPtrC8& aData):
 TMMSource(KUidDescDataSource, ContentAccess::KDefaultContentObject, KCCaccesUnknown),
   iData(aData)
	{
	}

/**
Returns a reference to the pointer containing an image data. Data is not owned by this object.
@return a reference to the data pointer
*/
EXPORT_C 
const TPtrC8& TDescriptorDataSource::DataBuf() const
	{
	return iData;
	}
