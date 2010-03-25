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

#include "ecamwebcameraactive.h"


//
// CWebCameraActive
//
CWebCameraActive* CWebCameraActive::NewL(MWebCameraActiveCallBack* aOwner, RWebcameraDevice& aDriver)
	{
	CWebCameraActive* self = new(ELeave) CWebCameraActive(aOwner, aDriver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

CWebCameraActive::CWebCameraActive(MWebCameraActiveCallBack* aOwner, RWebcameraDevice& aDriver)
				 :CActive(CActive::EPriorityStandard),
				  iOwner(aOwner),
				  iDriver(aDriver)
	{
	CActiveScheduler::Add(this);
	}

CWebCameraActive::~CWebCameraActive()
	{
	}

/**
* RunL.
*/
void CWebCameraActive::RunL()
	{
	TInt wError = iStatus.Int();

	switch(iOperation)
		{
	case ECamActiveReserve:
		iOwner->ReserveCallBack(wError);
		break;

	case ECamActivePowerOn:
		iOwner->PowerOnCallBack(wError);
		break;

	case ECamActiveImageCapture:
		iOwner->ImageCaptureCallBackL(wError);
		break;
		}
	iOperation = ECamActiveIdle;
	}

void CWebCameraActive::DoCancel()
	{
	}

void CWebCameraActive::ConstructL()
	{
	}

/** 
Asynchronous function that performs any required initialisation and reserves
the camera for exclusive use.

Calls CWebCamera::ReserveCallBack when complete. 
*/
void CWebCameraActive::Reserve()
	{
	iOperation = ECamActiveReserve;

	//Because there is not a camera device, I do not handle it
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

/** 
Asynchronous method to switch on camera power.

User must have successfully called Reserve() prior to calling this function.

Calls CWebCamera::PowerOnCallBack when power on is complete. 
*/
void CWebCameraActive::PowerOn()
	{
	iOperation = ECamActivePowerOn;

	//Because there is not a camera device, I do not handle it	
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

/** 
Cancels the asynchronous still image capture. 
	
Calls CWebCamera::ImageCaptureCallBackL when capture is complete. 
	
* @param aDes descriptor to store a capture image
*/
void CWebCameraActive::ImageCapture(TDes8& aDes)
	{
	iOperation = ECamActiveImageCapture;

	// start capture image
	// The division transfer is going to support in the next version. 
	iDriver.Capture(iStatus, aDes);
	SetActive();
	}

/** 
Cancels the asynchronous still image capture. 
	
@see CWebCamera::CancelCaptureImage
*/
void CWebCameraActive::CancelCaptureImage()
	{
	iOperation = ECamActiveIdle;

	iDriver.CaptureCancel();
	SetActive();
	}

