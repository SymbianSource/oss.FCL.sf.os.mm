/*
* Copyright (c) 2010 ISB.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* ISB - Initial contribution
*
* Contributors:
*
* Description:
*
*/

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <ecamuids.hrh>
#include <ecam.h>
#include "ecamwebcameraplugin.h"
#include "ecamwebcamerapluginuids.hrh"
#include "ecamwebcamerabuffer.h"
#include "ecamwebcameraadvset.h"
#include "ecamwebcameraactive.h"
#include "ecamwebcameravfactive.h"


_LIT(KCameraDriverPddName, "webcamera.pdd");
_LIT(KCameraDriverLddName, "ewebcamera.ldd");

const TInt KCaptureWidth     = 160;					// Capture image width
const TInt KCaptureHeight    = 120;					// Capture image height
const TInt KCaptureLineBytes = KCaptureWidth * 3;	// bytes of one line

//
// CWebCamera class
//

CWebCamera::CWebCamera():
	iCaptureBufPtr(NULL, 0)
	{
	iInfo.iHardwareVersion.iMajor = 0;
	iInfo.iHardwareVersion.iMinor = 0;
	iInfo.iHardwareVersion.iBuild = 0;
	iInfo.iSoftwareVersion.iMajor = 0;
	iInfo.iSoftwareVersion.iMinor = 0;
	iInfo.iSoftwareVersion.iBuild = 0;
	iInfo.iOrientation = TCameraInfo::EOrientationOutwards;

	iInfo.iOptionsSupported = 0;
	iInfo.iOptionsSupported |= TCameraInfo::EViewFinderDirectSupported;
	iInfo.iOptionsSupported |= TCameraInfo::EViewFinderBitmapsSupported;
	iInfo.iOptionsSupported |= TCameraInfo::EImageCaptureSupported;
	iInfo.iOptionsSupported |= TCameraInfo::EVideoCaptureSupported;
	iInfo.iOptionsSupported |= TCameraInfo::EContrastSupported;
	iInfo.iOptionsSupported |= TCameraInfo::EBrightnessSupported;
	iInfo.iOptionsSupported |= TCameraInfo::EViewFinderClippingSupported;

	iInfo.iFlashModesSupported 			= 0; // Bitfield of TFlash values
	iInfo.iExposureModesSupported		= 0; // Bitfield of TExposure values
	iInfo.iWhiteBalanceModesSupported	= 0; // Bitfield of TWhiteBalance values

	iInfo.iMinZoom = KMinTestCameraZoom;
	// KMinTestCameraZoom is zero or negative
	// note the algorithm for creating of zoom factor
	iInfo.iMinZoomFactor = TReal32(1)/TReal32(1 << -KMinTestCameraZoom);
	iInfo.iMaxZoom = KMaxTestCameraZoom;
	iInfo.iMaxZoomFactor = 1 << KMaxTestCameraZoom;

	iInfo.iMaxDigitalZoom = KMaxTestCameraDigitalZoom;
	iInfo.iMaxDigitalZoomFactor = KMaxTestCameraDigitalZoomFactor;

	iInfo.iImageFormatsSupported = 0;
	iInfo.iImageFormatsSupported |= EFormatExif;
	iInfo.iImageFormatsSupported |= EFormatFbsBitmapColor4K;
	iInfo.iImageFormatsSupported |= EFormatFbsBitmapColor64K;
	iInfo.iImageFormatsSupported |= EFormatFbsBitmapColor16M;

	iInfo.iVideoFrameFormatsSupported = 0;
	iInfo.iVideoFrameFormatsSupported |= EFormatFbsBitmapColor4K;
	iInfo.iVideoFrameFormatsSupported |= EFormatFbsBitmapColor64K;
	iInfo.iVideoFrameFormatsSupported |= EFormatFbsBitmapColor16M;
	iInfo.iMaxFramesPerBufferSupported = 1;
	iInfo.iMaxBuffersSupported = 2;
	}
	
CWebCamera::~CWebCamera()
	{
	delete iCaptureBuf;
	delete iAdvSettingsImpl;
	delete iVfActive;
	delete iActive;
	iImageSizes.Close();
	iDriver.Close();
	}
	
CWebCamera* CWebCamera::NewL()
	{
	CWebCamera* self = new(ELeave) CWebCamera;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

void CWebCamera::ConstructL()
	{
	RDebug::Print(_L("CWebCamera::ConstructL_S"));
	// Open drivers.
	TInt err;

	err = User::LoadPhysicalDevice(KCameraDriverPddName);
	RDebug::Print(_L("CWebCamera::ConstructL LoadPhysicalDevice[%d]"), err);

	if (err != KErrNone && err != KErrAlreadyExists)
		{
		User::Leave(err);
		}

	err = User::LoadLogicalDevice(KCameraDriverLddName);
	RDebug::Print(_L("CWebCamera::ConstructL LoadLogicalDevice[%d]"), err);

	if (err != KErrNone && err != KErrAlreadyExists)
		{
		User::Leave(err);
		}

	RDebug::Print(_L("CWebCamera::ConstructL iDriver.Open()"));
	iDriver.Open();

	iActive = CWebCameraActive::NewL(this, iDriver);
	iVfActive = CWebCameraVfActive::NewL(this, iDriver);

	User::LeaveIfError(iImageSizes.Append(TSize(640, 480)));	// VGA
	User::LeaveIfError(iImageSizes.Append(TSize(160, 120)));	// QQVGA
	iInfo.iNumImageSizesSupported = iImageSizes.Count();

	RDebug::Print(_L("CWebCamera::ConstructL_E"));
	}

/** 
from CCameraPlugin

A call to one of the factory functions firstly loads the plugin, followed by a
call to this function to complete construction.

Creates an object representing a camera.

@param  aObserver
        Reference to class derived from MCameraObserver2 designed to receive
        notification of asynchronous event completion.
@param	aCameraIndex
        Index from 0 to CamerasAvailable()-1 inclusive specifying the
        camera device to use.
@return Pointer to a fully constructed CCamera object. Ownership is passed
        to the caller.

@leave  May leave with KErrNoMemory or KErrNotSupported if aCameraIndex is
        out of range.	
*/
void CWebCamera::Construct2L(MCameraObserver& aObserver,TInt aCameraIndex)
	{
	iObserver = &aObserver;
	iCameraIndex = aCameraIndex;
	}

/** 
from CCameraPlugin

A call to one of the factory functions firstly loads the plugin, followed by a
call to this function to complete construction.

Duplicates the original camera object for use by, for example, multimedia systems.

May leave with KErrNoMemory or KErrNotFound if aCameraHandle is not valid.

@param  aObserver
        Reference to an observer.
@param  aCameraHandle Handle of an existing camera object.

@return Duplicate of the original camera object. 
*/
void CWebCamera::Construct2DupL(MCameraObserver& aObserver,TInt aCameraHandle)
	{
	iObserver = &aObserver;
	iCameraHandle = aCameraHandle;
	}

 /** 
from CCameraPlugin

 A call to one of the factory functions firstly loads the plugin, followed by a
call to this function to complete construction.
 
Creates an object representing a camera.

@param  aObserver
        Reference to class derived from MCameraObserver2 designed to receive
        notification of asynchronous event completion.
@param	aCameraIndex
        Index from 0 to CamerasAvailable()-1 inclusive specifying the
        camera device to use.
@param	aPriority
        Value from -100 to 100 indicating relative priority of client to
        use camera.

@return Pointer to a fully constructed CCamera object. Ownership is passed
        to the caller.

@leave  May leave with KErrNoMemory or KErrNotSupported if aCameraIndex is
        out of range.
*/
void CWebCamera::Construct2L(MCameraObserver2& aObserver,TInt aCameraIndex,TInt aPriority)
	{
	iObserver2 = &aObserver;
	iCameraIndex = aCameraIndex;
	iPriority = aPriority;
	}
	
/** 
from CCameraPlugin

Duplicates the original camera object for use by, for example, multimedia systems.

@leave  KErrNoMemory if out of memory.
@leave  KErrNotFound if aCameraHandle is not valid.	   
@leave  KErrPermissionDenied if the application does not have
        the UserEnvironment capability.

@param  aObserver
        Reference to an observer.
@param  aCameraHandle Handle of an existing camera object.

@return Duplicate of the original camera object. 

@capability	UserEnvironment
			An application that creates a CCamera object must have
			the UserEnvironment capability.
			
@note   Applications using this method to create camera object may not receive enums/uids added in future(after being baselined). 
		To receive them, they should rather use New2L() or NewDuplicate2L(), in which case, they should prepare 
		themselves to receive unrecognised values.
*/
void CWebCamera::Construct2DupL(MCameraObserver2& aObserver,TInt aCameraHandle)
	{
	iObserver2 = &aObserver;
	iCameraHandle = aCameraHandle;
	}

//
// virtual functions
//

/**
from CCamera

Gets information about the camera device.

@param  aInfo 
        On return, information about the camera device. See TCameraInfo. 
*/
void CWebCamera::CameraInfo(TCameraInfo& aInfo) const
	{
	aInfo = iInfo;
	}

/** 
from CCamera

Asynchronous function that performs any required initialisation and reserves
the camera for exclusive use.

Calls MCameraObserver:: ReserveComplete() when complete. 
*/
void CWebCamera::Reserve()
	{
	iActive->Reserve();
	}

/** 
from CCamera

De-initialises the camera, allowing it to be used by other clients. 
*/
void CWebCamera::Release()
	{
	StopViewFinder();
	iReserved = EFalse;
	}

/** 
from CCamera

Asynchronous method to switch on camera power.

User must have successfully called Reserve() prior to calling this function.

Calls MCameraObserver::PowerOnComplete() when power on is complete. 
*/
void CWebCamera::PowerOn()
	{
	iActive->PowerOn();
	}

/** 
from CCamera

Synchronous function for switching off camera power. 
*/
void CWebCamera::PowerOff()
	{
	StopViewFinder();
	iPowerOn = EFalse;
	}

/**
from CCamera

Gets the device-unique handle of this camera object.

@return  The device-unique handle of this camera object. 
*/
TInt CWebCamera::Handle()
	{
	return 0;
	}

/** 
from CCamera

Sets the zoom factor.

This must be in the range of TCameraInfo::iMinZoom to TCameraInfo::iMaxZoom
inclusive. May leave with KErrNotSupported if the specified zoom factor is
out of range.

@param aZoomFactor 
       Required zoom factor.
*/
void CWebCamera::SetZoomFactorL(TInt /*aZoomFactor = 0*/)
	{
	}

/** 
from CCamera

Gets the currently set zoom factor.

@return  The currently set zoom factor.
*/
TInt CWebCamera::ZoomFactor() const
	{
	return 0;
	}

/** 
from CCamera

Sets the digital zoom factor.

This must be in the range of 0 to TCameraInfo::iMaxDigitalZoom inclusive.

May leave with KErrNotSupported if the zoom factor is out of range.

@param  aDigitalZoomFactor
        The required digital zoom factor. 
*/
void CWebCamera::SetDigitalZoomFactorL(TInt /*aDigitalZoomFactor = 0*/)
	{
	}

/** 
from CCamera

Gets the currently set digital zoom factor.

@return  The currently set digital zoom factor. 
*/
TInt CWebCamera::DigitalZoomFactor() const
	{
	return 0;
	}

/**
from CCamera

Sets the contrast adjustment of the device.

This must be in the range of -100 to +100 or EContrastAuto. May leave with
KErrNotSupported if the specified contrast value is out of range.

@param  aContrast 
        Required contrast value. See TCameraInfo::iContrastSupported 
*/
void CWebCamera::SetContrastL(TInt /*aContrast*/)
	{
	}

/** 
from CCamera

Gets the currently set contrast value.

@return  The currently set contrast value.
*/
TInt CWebCamera::Contrast() const
	{
	return 0;
	}

/** 
from CCamera

Sets the brightness adjustment of the device.

No effect if this is not supported, see TCameraInfo::iBrightnessSupported.

This must be in the range of -100 to +100 or EBrightnessAuto. May leave
with KErrNotSupported if the brightness adjustment is out of range.

@param  aBrightness
        The required brightness adjustment. 
*/
void CWebCamera::SetBrightnessL(TInt /*aBrightness*/)
	{
	}

/** 
from CCamera

Gets the currently set brightness adjustment value.

@return  The currently set brightness adjustment value. 
*/
TInt CWebCamera::Brightness() const
	{
	return 0;
	}

/** 
from CCamera

Sets the flash mode.

No effect if this is not supported, see TCameraInfo::iFlashModesSupported.

May leave with KErrNotSupported if the specified flash mode is invalid.

@param  aFlash
        The required flash mode. 
*/
void CWebCamera::SetFlashL(TFlash /*aFlash = EFlashNone*/)
	{
	}

/** 
from CCamera

Gets the currently set flash mode.

@return  The currently set flash mode. 
@note	if CCamera::New2L() or CCamera::NewDuplicate2L() is not used to create CCamera object, it is assumed that 
application is not prepared to receive extra added enum values (unrecognised). So, any extra enum value(unrecognised)
(set in the ECAM implementation because of sharing clients) should not be returned from the ECAM implementation.
To receive extra added enum values, application should rather use CCamera::New2L() or CCamera::NewDuplicate2L() 
to create camera object. ECAM implementation, after verifying this,(by checking version no.) may send new values, if set.
In this case, application is assumed to be prepared to receive unrecognised enum values.

@see CCamera::CCameraAdvancedSettings::FlashMode()	
*/
CCamera::TFlash CWebCamera::Flash() const
	{
	return EFlashNone;
	}

/** 
from CCamera

Sets the exposure adjustment of the device.

No effect if this is not supported, see CameraInfo::iExposureModesSupported.

May leave with KErrNotSupported if the specified exposure adjustment is invalid.

@param  aExposure
        The required exposure adjustment. 
*/
void CWebCamera::SetExposureL(TExposure /*aExposure = EExposureAuto*/)
	{
	}

/** 
from CCamera

Gets the currently set exposure setting value.

@return  The currently set exposure setting value. 
*/
CCamera::TExposure CWebCamera::Exposure() const
	{
	return EExposureAuto;
	}

/** 
from CCamera

Sets the white balance adjustment of the device.

No effect if this is not supported, see TCameraInfo::iWhiteBalanceModesSupported.

@param  aWhiteBalance
        The required white balance adjustment.

@leave  KErrNotSupported if the specified white balance adjustment
        is invalid.
*/
void CWebCamera::SetWhiteBalanceL(TWhiteBalance /*aWhiteBalance = EWBAuto*/)
	{
	}

/** 
from CCamera

Gets the currently set white balance adjustment value.

@return  The currently set white balance adjustment value.

@note	if CCamera::New2L() or CCamera::NewDuplicate2L() is not used to create CCamera object, it is assumed that 
application is not prepared to receive extra added enum values (unrecognised). So, any extra enum value(unrecognised)
(set in the ECAM implementation because of sharing clients) should not be returned from the ECAM implementation.
To receive extra added enum values, application should rather use CCamera::New2L() or CCamera::NewDuplicate2L() 
to create camera object. ECAM implementation, after verifying this,(by checking version no.) may send new values, if set.
In this case, application is assumed to be prepared to receive unrecognised enum values.
Refer  CCamera::CCameraAdvancedSettings::WhiteBalanceMode() implementation

@see CCamera::CCameraAdvancedSettings::WhiteBalanceMode()
*/
CCamera::TWhiteBalance CWebCamera::WhiteBalance() const
	{
	return EWBAuto;
	}

/** 
from CCamera

Starts transfer of view finder data to the given portion of the screen using
direct screen access.

The aScreenRect parameter is in screen co-ordinates and may be modified if,
eg, the camera requires the destination to have a certain byte alignment, etc.

@param  aWs 
        Window server session.
@param  aScreenDevice 
        Screen device.
@param  aWindow 
        Displayable window.
@param  aScreenRect 
        Portion of the screen to which view finder data is to be
        transferred. This is in screen co-ordinates and may be modified if, for example,
        the camera requires the destination to have a certain byte alignment.

@leave  KErrNotReady if PowerOn() has either not
        been called, or has not yet completed.

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods. 

@see	CCamera::CCameraV2DirectViewFinder
*/
void CWebCamera::StartViewFinderDirectL(RWsSession& aWs, CWsScreenDevice& aScreenDevice, RWindowBase& aWindow, TRect& aScreenRect)
	{
	RDebug::Print(_L("CWebCamera::StartViewFinderDirectL"));
	TRect emptyRect;
	StartViewFinderDirectL(aWs, aScreenDevice, aWindow, aScreenRect, emptyRect);
	}

/** 
from CCamera

Starts transfer of view finder data to the given portion of the screen using
direct screen access and also clips to the specified portion of the screen.

The view finder has the same size and position as aScreenRect but is only
visible in the intersection of aScreenRect and aClipRect. May leave with KErrNotSupported
or KErrNotReady if Reserve() has not been called, or has not yet completed.

@param  aWs 
        Window server session.
@param  aScreenDevice 
        Screen device.
@param  aWindow 
        Displayable window.
@param  aScreenRect 
        Portion of the screen to which view finder data is to be
        transferred. This is in screen co-ordinates and may be modified if, for example,
        the camera requires the destination to have a certain byte alignment.
@param  aClipRect
        The rectangle to which the screen will be clipped.

@leave  KErrNotReady if PowerOn() hasn't been called successfully. 

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraClientViewFinder
*/
void CWebCamera::StartViewFinderDirectL(RWsSession& aWs, CWsScreenDevice& aScreenDevice, RWindowBase& aWindow, TRect& aScreenRect, TRect& aClipRect)
	{
	RDebug::Print(_L("CWebCamera::StartViewFinderDirectL'"));
	iVfActive->StartViewFinderDirectL(aWs, aScreenDevice, aWindow, aScreenRect, aClipRect);
	}

/** 
from CCamera

Starts transfer of view finder data.

Bitmaps are returned by MCameraObserver::ViewFinderFrameReady().

@param  aSize 
        On return, the size used.
@leave  KErrNotReady if PowerOn() has not been called, or has not yet completed.

@note   This method is assumed to be meant for default display only. 

@see	CCamera::CCameraClientViewFinder 
*/
void CWebCamera::StartViewFinderBitmapsL(TSize& /*aSize*/)
	{
	}
	
/** 
from CCamera

Starts transfer of view finder data and clips the bitmap to the specified clip
rectangle.

The bitmap is the size of the intersection of aSize and aClipRect, not simply
aSize padded with white space.

@param  aSize
        On return, the size used.
@param  aClipRect 
        Required clip rectangle. May be modified if, for example,
        the camera only supports certain byte alignments.

@leave  KErrInUse if Reserve() hasn't been called successfully.
@leave  KErrNotReady if PowerOn() hasn't been called successfully.

@note   This method is assumed to be meant for default display only. 

@see	CCamera::CCameraClientViewFinder
*/
void CWebCamera::StartViewFinderBitmapsL(TSize& /*aSize*/,TRect& /*aClipRect*/)
	{
	}

/** 
from CCamera

Starts transfer of view finder data.

Picture data is returned by MCameraObserver2::ViewFinderReady().

@param  aImageFormat 
        The image format requested by the client.
@param  aSize 
        On return, the size used.
@leave  KErrNotSupported 
@leave  KErrNotReady if Reserve() has not been
        called, or has not yet completed. 

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraClientViewFinder
*/
void CWebCamera::StartViewFinderL(TFormat /*aImageFormat*/,TSize& /*aSize*/)
	{
	}

/** 
from CCamera

Starts transfer of view finder data and clips the picture to the specified clip
rectangle. Picture data is returned by MCameraObserver2::ViewFinderReady().

The picture is the size of the intersection of aSize and aClipRect, not simply
aSize padded with white space.

@param  aImageFormat 
        The image format.
@param  aSize
        On return, the size used.
@param  aClipRect 
        Required clip rectangle. May be modified if, for example,
        the camera only supports certain byte alignments.

@leave  KErrInUse if Reserve() hasn't been called successfully,
@leave  KErrNotReady if PowerOn() hasn't been called successfully.

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraClientViewFinder
*/
void CWebCamera::StartViewFinderL(TFormat /*aImageFormat*/,TSize& /*aSize*/,TRect& /*aClipRect*/)
	{
	}

/** 
Stops transfer of view finder data to the screen. 

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraV2DirectViewFinder
@see	CCamera::CCameraClientViewFinder
*/
void CWebCamera::StopViewFinder()
	{
	iVfActive->StopViewFinder();
	}

/** 
from CCamera

Queries whether the view finder is active.

@return  ETrue if the view finder is active. EFalse if the view finder is not
         active. 
         
@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraV2DirectViewFinder
@see	CCamera::CCameraClientViewFinder
*/
TBool CWebCamera::ViewFinderActive() const
	{
	return iVfActive->ViewFinderActive();
	}

/** 
from CCamera

Sets whether view finder mirroring is on.

Used to switch between what the camera sees and what you would see if the
device were a mirror.

@param  aMirror 
        ETrue to set mirroring on, EFalse to set mirroring off.

@leave  KErrNotSupported.

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraV2DirectViewFinder
@see	CCamera::CCameraClientViewFinder
*/
void CWebCamera::SetViewFinderMirrorL(TBool /*aMirror*/)
	{
	}

/** 
from CCamera

Gets whether view finder mirroring is active.

@return  ETrue if mirroring is set, EFalse if mirroring is not set. 

@note   This method is assumed to be meant for default display only. KECamDefaultViewFinderHandle should be used to 
		refer viewfinders started using CCamera methods.

@see	CCamera::CCameraV2DirectViewFinder
@see	CCamera::CCameraClientViewFinder
*/
TBool CWebCamera::ViewFinderMirror() const
	{
	return EFalse;
	}

/** 
from CCamera

Performs setup and allocation of memory.

Called prior to calling CaptureImage() to keep the latency of that function
to a minimum.

Needs to be called only once for multiple CaptureImage() calls. May leave
with KErrNotSupported or KErrNoMemory or KErrInUse or KErrNotReady.

The specified image format must be one of the formats supported
(see TCameraInfo::iImageFormatsSupported).

The specified size index must be in the range of 0 to TCameraInfo::iNumImageSizesSupported-1
inclusive.

@param  aImageFormat 
        The image format.
@param  aSizeIndex 
        Size index.

@leave  KErrNotSupported
@leave  KErrNoMemory
@leave  KErrNotReady if PowerOn() hasn't been called successfully.

@see 	CCamera::CCameraPreImageCaptureControl::PrepareImageCapture(TPrepareImageParameters aPrepareImageParameters)
*/
void CWebCamera::PrepareImageCaptureL(TFormat /*aImageFormat*/,TInt /*aSizeIndex*/)
	{
	}

/** 
from CCamera

Performs setup and allocation of memory and clips the image to the specified
rectangle.

No effect unless TCameraInfo::iImageClippingSupported is set to ETrue. The
image captured is the intersection of aClipRect and the rectangle from (0,0)
to aSize. Needs to be called only once for multiple CaptureImage() calls.
May leave with KErrNotSupported or KErrNoMemory.

The specified image format must be one of the formats supported (see TCameraInfo::iImageFormatsSupported).

The specified size index must be in the range of 0 to TCameraInfo::iNumImageSizesSupported-1
inclusive.

@param  aImageFormat 
        The image format.
@param  aSizeIndex 
        Size index.
@param  aClipRect
        The rectangle to which the image is to be clipped.

@leave   KErrNotSupported
@leave   KErrNoMemory
@leave   KErrInUse if Reserve() hasn't been called successfully
@leave   KErrNotReady if PowerOn()
         hasn't been called successfully.

@see 	CCamera::CCameraPreImageCaptureControl::PrepareImageCapture(TPrepareImageParameters aPrepareImageParameters)
*/
void CWebCamera::PrepareImageCaptureL(TFormat /*aImageFormat*/,TInt /*aSizeIndex*/,const TRect& /*aClipRect*/)
	{
	}

/** 
from CCamera

Asynchronously performs still image capture.

Calls MCameraObserver::ImageReady() when complete. 

@see CCamera::CCameraImageCapture
@see CCamera::CCameraPreImageCaptureControl
*/
void CWebCamera::CaptureImage()
	{
	delete iCaptureBuf;
	iCaptureBuf = NULL;

	iCaptureBuf = HBufC8::NewL(KMaxBufSize);
	iCaptureBufPtr.Set(iCaptureBuf->Des());
	iCaptureBufPtr.SetLength(0);

	iActive->ImageCapture(iCaptureBufPtr);

	iImageCaptureActive = ETrue;
	}

/** 
from CCamera

Cancels the asynchronous still image capture. 

@see CCamera::CCameraImageCapture
*/
void CWebCamera::CancelCaptureImage()
	{
	iImageCaptureActive = EFalse;
	}

/** 
from CCamera

Enumerates through the available image capture sizes, based on the specified
size index and format

The size index must be in the range 0 to TCameraInfo::iNumImageSizesSupported-1
inclusive.

@param  aSize 
        Image size.
@param  aSizeIndex 
        Size index.
@param  aFormat 
        The image format.
*/
void CWebCamera::EnumerateCaptureSizes(TSize& aSize, TInt aSizeIndex, TFormat aFormat) const
	{
	if (aSizeIndex < 0 || aSizeIndex >= iInfo.iNumImageSizesSupported ||
		!(aFormat & iInfo.iImageFormatsSupported) )
		{
		aSize = TSize(0,0);
		}
	else
		{
		aSize = iImageSizes[aSizeIndex];
		}
	}

/** 
from CCamera

Prepares for video capture.

Performs setup and allocation of memory prior to calling StartVideoCapture()
to keep the latency of that function to a minimum.

May leave with KErrNotSupported or KErrNoMemory.

@param  aFormat 
        Format must be one of the video frame formats supported (see
        TCameraInfo::iVideoFrameFormatsSupported).
@param  aSizeIndex 
        Size index  must be in the range 0 to TCameraInfo::iNumVideoFrameSizesSupported-1
        inclusive.
@param  aRateIndex 
        The rate must be in the range 0 to TCameraInfo::iNumVideoFrameRatesSupported-1
        inclusive.
@param  aBuffersToUse 
        The number of discrete buffers to use.
@param  aFramesPerBuffer 
        How large the buffers are to be. Must be less than
        or equal to TCameraInfo::iMaxFramesPerBufferSupported. One buffer is returned
        to MCameraObserver::FrameBufferReady() at a time.

@leave  May leave with KErrNotSupported, KErrNoMemory, or KErrNotReady if PowerOn() 
		hasn't been called successfully.

@see 	CCamera::CCameraVideoCaptureControl::PrepareVideoCapture(const TPrepareVideoParameters& aPrepareVideoParameters)
*/
void CWebCamera::PrepareVideoCaptureL(TFormat /*aFormat*/,TInt /*aSizeIndex*/,TInt /*aRateIndex*/,TInt /*aBuffersToUse*/,TInt /*aFramesPerBuffer*/)
	{
	}

/** 
from CCamera

Prepares for video capture and clips the frames to the given rectangle.

Performs setup and allocation of memory prior to calling StartVideoCapture()
to keep the latency of that function to a minimum.

May leave with KErrNotSupported or KErrNoMemory.

@param  aFormat 
        Format must be one of the video frame formats supported (see
        TCameraInfo::iVideoFrameFormatsSupported).
@param  aSizeIndex 
        Size index must be in the range 0 to TCameraInfo::iNumVideoFrameSizesSupported-1
        inclusive.
@param  aRateIndex 
        The rate must be in the range 0 to TCameraInfo::iNumVideoFrameRatesSupported-1
        inclusive.
@param  aBuffersToUse 
        The number of discrete buffers to use.
@param  aFramesPerBuffer 
        How large the buffers are to be. Must be less than
        or equal to TCameraInfo::iMaxFramesPerBufferSupported. One buffer is returned
        to MCameraObserver::FrameBufferReady() at a time.
@param  aClipRect 
        The rectangle to which the image is to be clipped.
@leave  KErrNotSupported
@leave  KErrNoMemory, 
@leave  KErrInUse if Reserve() hasn't been called successfully
@leave  KErrNotReady if PowerOn() hasn't been called successfully.

@see 	CCamera::CCameraVideoCaptureControl::PrepareVideoCapture(const TPrepareVideoParameters& aPrepareVideoParameters)
*/
void CWebCamera::PrepareVideoCaptureL(TFormat /*aFormat*/,TInt /*aSizeIndex*/,TInt /*aRateIndex*/,TInt /*aBuffersToUse*/,TInt /*aFramesPerBuffer*/,const TRect& /*aClipRect*/)
	{
	}

/** 
from CCamera

Starts capturing video.

Calls MCameraObserver::FrameBufferReady() when each buffer has been filled
with the required number of frames, as set by PrepareVideoCaptureL(). 
*/
void CWebCamera::StartVideoCapture()
	{
	}

/** 
from CCamera

Stops video capture. 
*/
void CWebCamera::StopVideoCapture()
	{
	}

/** 
from CCamera

Tests whether video capture is active.

@return  ETrue if video capture is active. EFalse if video capture is not active 
*/
TBool CWebCamera::VideoCaptureActive() const
	{
	return EFalse;
	}

/** 
from CCamera

Enumerates through the available video frame sizes, based on the specified
size index and format.

@param  aSize 
        On return the available video frame sizes. Sizes should be returned
        in order, largest first, so clients do not have to iterate through every one.
@param  aSizeIndex 
        Size index. Must be in the range 0 to TCameraInfo::iNumVideoFrameSizesSupported-1
        inclusive
@param  aFormat 
        Image format. 
*/
void CWebCamera::EnumerateVideoFrameSizes(TSize& /*aSize*/,TInt /*aSizeIndex*/,TFormat /*aFormat*/) const
	{
	}

/** 
from CCamera

Enumerates through the available video frame rates, based on the specified
rate index, video frame format, size index and exposure mode.

@param  aRate 
        On return, the available video frame rates. Some rates may not
        be available due to, for example, current flash mode setting. In those cases
        a rate of 0 will be returned. Rates should be returned in order, highest first,
        so clients do not have to iterate through every one.
@param  aRateIndex 
        The rate index. Must be in the range 0 to TCameraInfo::iNumVideoFrameRatesSupported-1
        inclusive.
@param  aFormat 
        The format.
@param  aSizeIndex 
        The size index.
@param  aExposure 
        The exposure mode. 
*/
void CWebCamera::EnumerateVideoFrameRates(TReal32& /*aRate*/,TInt /*aRateIndex*/,TFormat /*aFormat*/,TInt /*aSizeIndex*/,TExposure /*aExposure = EExposureAuto*/) const
	{
	}

/** 
from CCamera

Gets the frame size currently in use.

@param  aSize 
        The frame size currently in use. 
*/
void CWebCamera::GetFrameSize(TSize& /*aSize*/) const
	{
	}

/** 
from CCamera

Gets the frame rate currently in use.

@return  The frame rate currently in use. 
*/
TReal32 CWebCamera::FrameRate() const
	{
	return 0.0;
	}

/** 
from CCamera

Gets the number of buffers currently in use.

@return  The number of buffers currently in use. 
*/
TInt CWebCamera::BuffersInUse() const
	{
	return 0;
	}

/** 
from CCamera

Gets the number of frames per buffer currently in use.

@return  The number of frames per buffer currently in use. 
*/
TInt CWebCamera::FramesPerBuffer() const
	{
	return 0;
	}

/** 
from CCamera

Sets the quality value to use if jpeg is a supported image for video format.

Ignored if jpeg is not a supported image for video format.

@param  aQuality
        The quality value to use, clamped to the range 1 to 100.
        
@see	CCamera::CCameraPreImageCaptureControl::TPrepareImageParameters::iImageMaxMemorySize
*/
void CWebCamera::SetJpegQuality(TInt /*aQuality*/)
	{
	}

/**
from CCamera

Gets the currently set jpeg quality value.

Returns 0 if not supported.

@return The currently set jpeg quality value.

@see    CCamera::CCameraPreImageCaptureControl::GetImageMaxMemorySizeL(TUint& aMemorySize)
*/
TInt CWebCamera::JpegQuality() const
	{
	return 0;
	}

/**
from CCamera

Gets a custom interface. The client has to cast the returned pointer
to the appropriate type.

@param aInterface
	   The Uid of the particular interface function required.

@return Custom interface pointer. NULL if the requested interface is not supported.
*/
TAny* CWebCamera::CustomInterface(TUid aInterface)
	{
	switch(aInterface.iUid)
		{
	// advanced settings interface pointers
	case KECamMCameraAdvancedSettingsUidValue:
		iAdvSettingsImpl = CWebCameraAdvSet::NewL(*this);
		return static_cast<MCameraAdvancedSettings*>(iAdvSettingsImpl);

	case KECamMCameraAdvancedSettings2UidValue:
		iAdvSettingsImpl = CWebCameraAdvSet::NewL(*this);
		return static_cast<MCameraAdvancedSettings2*>(iAdvSettingsImpl);

	case KECamMCameraAdvancedSettings3UidValue:
		iAdvSettingsImpl = CWebCameraAdvSet::NewL(*this);
		return static_cast<MCameraAdvancedSettings3*>(iAdvSettingsImpl);

	default:
		return NULL;
		}
	}

/**
from MWebCameraActiveCallBack

CallBack function of the Reserve
*/
void CWebCamera::ReserveCallBack(TInt aError)
	{
	RDebug::Print(_L("CWebCamera::ReserveCallBack"));

	if (iObserver)
		{
		iObserver->ReserveComplete(aError);
		}
	else if (iObserver2)
		{
		const TECAMEvent wEvent(KUidECamEventReserveComplete, aError);
		iObserver2->HandleEvent(wEvent);
		}
	}

/**
from MWebCameraActiveCallBack

CallBack function of the PowerOn
*/
void CWebCamera::PowerOnCallBack(TInt aError)
	{
	RDebug::Print(_L("CWebCamera::PowerOnCallBack"));

	if (iObserver)
		{
		iObserver->PowerOnComplete(aError);
		}
	else if (iObserver2)
		{
		const TECAMEvent wEvent(KUidECamEventPowerOnComplete, aError);
		iObserver2->HandleEvent(wEvent);
		}
	}

/**
from MWebCameraActiveCallBack

CallBack function of the ImageCapture
*/
void CWebCamera::ImageCaptureCallBackL(TInt aError)
	{
	RDebug::Print(_L("CWebCamera::ImageCaptureCallBack_S"));

	iImageCaptureActive = EFalse;

	iCaptureBitmap = NULL;
	CFbsBitmap* image = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(image);
	User::LeaveIfError(image->Create(TSize(KCaptureWidth, KCaptureHeight), EColor16M));
	CleanupStack::Pop(image);
	iCaptureBitmap = image;

////////////////////////////////////////////////////////////////////////////////////////
// output receive data log
//	RDebug::Print(_L("CWebCameraVfActive::ImageCaptureCallBack iCaptureBufPtr[%d]"), iCaptureBufPtr.Length());
//	TBuf<256> hexBuf;
//	for (TInt i = 0; i < iCaptureBufPtr.Length(); i++)
//		{
//		hexBuf.AppendFormat(_L("%02X "), iCaptureBufPtr[i]);
//		if ((i % 16) == 15)
//			{
//			RDebug::Print(hexBuf);
//			hexBuf = KNullDesC;
//			}
//		}
//	RDebug::Print(hexBuf);
////////////////////////////////////////////////////////////////////////////////////////

	RDebug::Print(_L("CWebCamera::ImageCaptureCallBack start setscanline_S"));
	for (TInt height=0; height<KCaptureHeight; height++)
		{
		TInt pos = height * KCaptureLineBytes;
		TPtrC8 posptr = iCaptureBuf->Mid(pos, KCaptureLineBytes);
		TBuf8<KCaptureLineBytes>  buf;
		buf.Copy(posptr);
		iCaptureBitmap->SetScanLine(buf, ((KCaptureHeight-1)-height));
		}
	RDebug::Print(_L("CWebCamera::ImageCaptureCallBack start setscanline_E"));

	if (iObserver)
		{
		iObserver->ImageReady(iCaptureBitmap, iCaptureBuf, aError);
		}
	else if (iObserver2)
		{
		iObserver2->ImageBufferReady(*iCaptureCameraBuf, aError);
		}

	RDebug::Print(_L("CWebCamera::ImageCaptureCallBack_E"));
	}

//from MWebCameraVfActiveCallBack
void CWebCamera::ViewFinderCallBack(TInt /*aError*/)
	{
	RDebug::Print(_L("CWebCamera::ViewFinderCallBack"));
	}


//
// CWebCameraInfo class
//

CWebCameraInfo::CWebCameraInfo()
	{
	}
	
CWebCameraInfo::~CWebCameraInfo()
	{
	}
	
CWebCameraInfo* CWebCameraInfo::NewL()
	{
	return new (ELeave) CWebCameraInfo;
	}

/** 
from CCameraInfoPlugin

Determines the number of cameras on the device.

@return Count of cameras present on the device.
*/
TInt CWebCameraInfo::CamerasAvailable()
	{
	return 1;
	}


// __________________________________________________________________________
// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(KUidOnboardWebCameraPlugin,	CWebCamera::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidOnboardWebCameraInfo,	CWebCameraInfo::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}
