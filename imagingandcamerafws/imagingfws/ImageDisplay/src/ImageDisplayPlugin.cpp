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

#include <ecom/ecom.h>
#include "ImageDisplayPlugin.h"
#include "ImageDisplayMain.h"
#include "ImageDisplayFramework.h"


/**
@internalComponent

  Body of CImageDisplayPlugin

  This class is not for public use
*/
class CImageDisplayPluginBody;
NONSHARABLE_CLASS(CImageDisplayPluginBody): public CBase {
	friend class CImageDisplayPlugin;

public:
	static CImageDisplayPluginBody* NewL(CImageDisplayPlugin& aPlugin);
	~CImageDisplayPluginBody();

private:
	void ConstructL();
	CImageDisplayPluginBody(CImageDisplayPlugin& aPlugin);

private:
	CImageDisplayPlugin&	iPlugin;
	CAsyncCallBack*			iImageReadyCb;
	TBool					iCbRunning;
	const CFbsBitmap*		iBitmapForCallback;
	TUint					iStatusForCallBack;
	TRect					iUpdateAreaForCb;
	TInt					iErrorForCb;	

};

//static
CImageDisplayPluginBody* CImageDisplayPluginBody::NewL(CImageDisplayPlugin& aPlugin)
	{
	CImageDisplayPluginBody* self=new (ELeave) CImageDisplayPluginBody(aPlugin);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

inline
CImageDisplayPluginBody::CImageDisplayPluginBody(CImageDisplayPlugin& aPlugin):
																		iPlugin(aPlugin)
	{
	}

CImageDisplayPluginBody::~CImageDisplayPluginBody()
	{
	delete iImageReadyCb;
	}

inline
void CImageDisplayPluginBody::ConstructL()
	{
	iImageReadyCb = new (ELeave) CAsyncCallBack(TCallBack(CImageDisplayPlugin::AsyncCbGate, &iPlugin), EPriorityHigh);
	}

/**
Constructor for the CImageDisplayPlugin class.
*/
EXPORT_C CImageDisplayPlugin::CImageDisplayPlugin() 
	{
	}


/**
This is the destructor for the CImageDisplayPlugin
and is responsible for deallocating all resources.
*/
EXPORT_C CImageDisplayPlugin::~CImageDisplayPlugin()
	{
	if (iBody && iBody->iCbRunning)
		{
		Panic(ECallbackStillActive);
		}
	iImageSizes.Close();
	delete iBody;
	REComSession::DestroyedImplementation(iInstanceUid);
	}

/**
The function NewL constructs a CImageDisplayPlugin-derived ECOM plugin.

@param     aImplementationUid
           The ECOM implementation UID of the plugin
@param     aFramework
           A reference to the CImageDisplayFramework object that 
		   is creating this object

@leave KErrNotFound If the plugin could not be found

@return	A pointer to a fully constructed CImageDisplayPlugin
*/

EXPORT_C CImageDisplayPlugin* CImageDisplayPlugin::NewL(
	TUid aImplementationUid, CImageDisplayFramework& aFramework)
	{
	TUid instanceUid;

	// instantiate the plugin using ECOM
	CImageDisplayPlugin* self = 
		reinterpret_cast<CImageDisplayPlugin*>(
			REComSession::CreateImplementationL(
				aImplementationUid,
				instanceUid));

	// point back to the framework
	CleanupStack::PushL(self);

	self->ConstructL(aFramework, instanceUid);

	CleanupStack::Pop(self);
	
	return self;
	}

inline
void CImageDisplayPlugin::ConstructL(CImageDisplayFramework& aFramework, TUid aInstanceUid)
	{
	iFramework = &aFramework;

	// hand over ownership of the Instance Uid to the framework
	// so that it can call REComSession::DestroyedImplementation() on deletion.
	//iFramework->SetPluginInstanceUidL(aInstanceUid);
	iInstanceUid = aInstanceUid;

	iBody = CImageDisplayPluginBody::NewL(*this);
	
	}


/**
Return whether the source is data

@return	TImageSourceType the source of image 
*/
EXPORT_C CImageDisplayPlugin::TImageSourceType CImageDisplayPlugin::SourceType() const
	{
	ASSERT(iFramework);
	return iFramework->SourceType();
	}


/**
Gets the source file name

@return	The source file name

@panic	EImageWrongType
		The source is not a file
*/
EXPORT_C const TDesC& CImageDisplayPlugin::SourceFilename() const
	{
	ASSERT(iFramework);
	return iFramework->SourceFilename();
	}

/**
Gets the source file name

@return	The source file handle

@panic	EImageWrongType
		The source is not a file
*/
EXPORT_C RFile& CImageDisplayPlugin::SourceFileHandle() const
	{
	ASSERT(iFramework);
	return iFramework->SourceFileHandle();
	}

/**
Gets the source descriptor

@return	A reference to the source descriptor

@panic	EImageWrongType
		The source is not a descriptor
*/
EXPORT_C const TDesC8& CImageDisplayPlugin::SourceData() const
	{
	ASSERT(iFramework);
	return iFramework->SourceData();
	}

/**
Gets the source image's MIME type

@return	A reference to the source image's MIME type
		KNullDesC8 if not defined
*/
EXPORT_C const TDesC8& CImageDisplayPlugin::SourceMimeType() const
	{
	ASSERT(iFramework);
	return iFramework->SourceMimeType();
	}

/**
Gets the source image's type

@return	The UID of the source type
		KNullUid if not defined
*/
EXPORT_C const TUid CImageDisplayPlugin::SourceImageType() const
	{
	ASSERT(iFramework);
	return iFramework->SourceImageType();
	}

/**
Gets the source image's subtype

@return	The UID of the source type
		KNullUid if not defined
*/
EXPORT_C const TUid CImageDisplayPlugin::SourceImageSubType() const
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
EXPORT_C TBool CImageDisplayPlugin::SourceRect(TRect& aRect) const
	{
	ASSERT(iFramework);
	return iFramework->SourceRect(aRect);
	}

/**
Gets the requested size of the destination image

@return	The destination image size
*/
EXPORT_C const TSize& CImageDisplayPlugin::DestinationSizeInPixels() const
	{
	ASSERT(iFramework);
	return iFramework->DestinationSizeInPixels();
	}

/**
Gets the options requested by the client

@return	The options requested by the client
*/
EXPORT_C TUint CImageDisplayPlugin::Options() const
	{
	ASSERT(iFramework);
	return iFramework->Options();
	}

/**
Return whether the client wishes to maintain the source image's aspect ratio

@return	ETrue if the aspect ratio is to be maintained
*/
EXPORT_C TBool CImageDisplayPlugin::MaintainAspectRatio() const
	{
	ASSERT(iFramework);
	return iFramework->MaintainAspectRatio();
	}

/**
Return the display mode

@return Display mode of the image
*/
EXPORT_C TDisplayMode CImageDisplayPlugin::DisplayMode() const
	{
	ASSERT(iFramework);
	return iFramework->DisplayMode();
	}

/**
When a plugin extension has been implemented, this function should be implemented 
by the plugin writer to enable the client access to the extension 

@return	A pointer to a fully constructed CImageDisplayPluginExtension
		NULL, if there is no extension to this plugin
*/
EXPORT_C TInt CImageDisplayPlugin::ExtensionInterface(TUid /*aIFaceUid*/, TAny*& aIFacePtr)
	{
	aIFacePtr = NULL;
	return KErrNotSupported;
	}

EXPORT_C TBool CImageDisplayPlugin::ValidBitmap() const
	{
	return EFalse;	
	}

EXPORT_C TUint CImageDisplayPlugin::ImageStatus() const
	{
	return CImageDisplay::EImageTypeUnknown;
	}

EXPORT_C TInt CImageDisplayPlugin::NumFrames(TInt& /*aNumFrames*/) const
	{
	return KErrNotSupported;
	}

EXPORT_C void CImageDisplayPlugin::GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const
	{
	aBitmap = NULL;
	aMask	= NULL;
	}

EXPORT_C
TBool CImageDisplayPlugin::CallbackIsRunning() const
	{
	ASSERT(iBody);
	return (iBody->iCbRunning || iBody->iImageReadyCb->IsActive());
	}

EXPORT_C 
void CImageDisplayPlugin::CancelCallback()
	{
	ASSERT(iBody);
	iBody->iImageReadyCb->Cancel();
	}

/**
Callback to the client application, via the ImageDisplay framework

*/
EXPORT_C 
void CImageDisplayPlugin::AsyncCallbackImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError)
	{
	ASSERT(iBody);
	if (iBody->iImageReadyCb->IsActive())
		{
		Panic(ECallbackStillActive);
		}
	iBody->iBitmapForCallback	= aBitmap;
	iBody->iStatusForCallBack	= aStatus;
	iBody->iUpdateAreaForCb		= aUpdatedArea;
	iBody->iErrorForCb			= aError;
	iBody->iImageReadyCb->Call();
	}

//static
TInt CImageDisplayPlugin::AsyncCbGate(TAny* aPtr)
	{
	return reinterpret_cast<CImageDisplayPlugin*>(aPtr)->AsyncCbGate();
	}

inline
TInt CImageDisplayPlugin::AsyncCbGate()
	{
	ASSERT(iFramework && iBody);
	if (CallbackIsRunning())
		{
		Panic(ECallbackStillActive);
		}
	iBody->iCbRunning = ETrue;
	iFramework->CallbackImageReady(iBody->iBitmapForCallback, iBody->iStatusForCallBack, iBody->iUpdateAreaForCb, iBody->iErrorForCb);
	iBody->iCbRunning = EFalse;
	return KErrNone;
	}

EXPORT_C 
ContentAccess::TIntent CImageDisplayPlugin::SourceDataIntent() const
	{
	ASSERT(iFramework);
	return iFramework->SourceDataIntent();
	}

EXPORT_C 
const TDesC& CImageDisplayPlugin::SourceDataId() const
	{
	ASSERT(iFramework);
	return iFramework->SourceDataId();
	}
	
EXPORT_C 
void CImageDisplayPlugin::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

EXPORT_C 
void CImageDisplayPlugin::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

EXPORT_C 
void CImageDisplayPlugin::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

EXPORT_C 
void CImageDisplayPlugin::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

