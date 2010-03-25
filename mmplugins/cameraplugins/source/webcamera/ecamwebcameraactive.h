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

#ifndef ECAMWEBCAMERAACTIVE_H
#define ECAMWEBCAMERAACTIVE_H

#include "ecamwebcameraplugin.h"

/**
 *  Perform an Async demand of the camera application
 */
class CWebCameraActive : public CActive
	{
private:
	
	/**
    Active Operation of ECamWebCamera
    */
	enum TECamWebCameraActiveOperation
		{
		ECamActiveIdle = 0,
		ECamActiveReserve,
		ECamActivePowerOn,
		ECamActiveImageCapture,
		};

public:
    /** Constructor */
    /**
     * @param aOwner 
     		  pointer to callback object.
     * @param aDriver 
     		  reference to driver.
     */
    CWebCameraActive(MWebCameraActiveCallBack* aOwner, RWebcameraDevice& aDriver);
	
	/**> Destructor */
	~CWebCameraActive();
	
    /**
     * Two-phased constructor.
     * @param aOwner 
     		  pointer to callback object.
     * @param aDriver 
     		  reference to driver.
     */
	static CWebCameraActive* NewL(MWebCameraActiveCallBack* aOwner, RWebcameraDevice& aDriver);

	/** 
	Asynchronous function that performs any required initialisation and reserves
	the camera for exclusive use.

	Calls CWebCamera::ReserveCallBack when complete. 
	*/
	void Reserve();

	/** 
	Asynchronous method to switch on camera power.

	User must have successfully called Reserve() prior to calling this function.

	Calls CWebCamera::PowerOnCallBack when power on is complete. 
	*/
	void PowerOn();
	
	/** 
	Cancels the asynchronous still image capture. 
	
	Calls CWebCamera::ImageCaptureCallBack when capture is complete. 
	
	* @param aDes descriptor to store a capture image
	*/
	void ImageCapture(TDes8& aDes);
	
	/** 
	Cancels the asynchronous still image capture. 
	
	@see CWebCamera::CancelCaptureImage
	*/
	void CancelCaptureImage();

private:
	void ConstructL();

private:
    /**
     * RunL.
     */
	void RunL();
	void DoCancel();
	void ViewFinderComplete(TInt aError);

private:

	MWebCameraActiveCallBack*		iOwner;		// origin of start.
	TECamWebCameraActiveOperation	iOperation;	// Status of the Active Operation of ECamWebCamera
	RWebcameraDevice				iDriver;	// Camera driver
	};


#endif // ECAMWEBCAMERAPLUGIN_H
