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

#include "ecamwebcameravfactive.h"

const TInt KVfWidth     = 160;			// viewfinder image width
const TInt KVfHeight    = 120;			// viewfinder image height
const TInt KVfLineBytes = KVfWidth * 3;	// bytes of one line

//
// CWebCameraVfActive
//
CWebCameraVfActive* CWebCameraVfActive::NewL(MWebCameraVfActiveCallBack* aOwner, RWebcameraDevice& aDriver)
	{
	CWebCameraVfActive* self = new(ELeave) CWebCameraVfActive(aOwner, aDriver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

CWebCameraVfActive::CWebCameraVfActive(MWebCameraVfActiveCallBack* aOwner, RWebcameraDevice& aDriver)
				   :CActive(CActive::EPriorityStandard),
				    iOwner(aOwner),
				    iDriver(aDriver),
				    iVfBufPtr(NULL, 0)
	{
	CActiveScheduler::Add(this);
	}

CWebCameraVfActive::~CWebCameraVfActive()
	{
	delete iVfBitmap;
	delete iDSA;
	delete iVfBuf;
	}

void CWebCameraVfActive::ConstructL()
	{
	}

/**
* RunL.
*/
void CWebCameraVfActive::RunL()
	{
	TInt wError = iStatus.Int();
	RDebug::Print(_L("CWebCameraVfActive::RunL wError = %d"), wError);

	if (iViewFinderActive)
		{
		//The whole view finder start repeats screen update and the data acquisition.
		UpdateViewL();
		Start();
		}
	}

void CWebCameraVfActive::DoCancel()
	{
	iDriver.StopViewFinder();
	}

/** 
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
*/
void CWebCameraVfActive::StartViewFinderDirectL(RWsSession& aWs, CWsScreenDevice& aScreenDevice, RWindowBase& aWindow, TRect& aScreenRect, TRect& aClipRect)
	{
	iScreenRect = aScreenRect;
	iClipRect = aClipRect;

	// create buffer for view finder data
	iVfBuf = HBufC8::NewL(KMaxBufSize);

	// create DirectScreenAccess
	delete iDSA;
	iDSA = NULL;
	iViewFinderActive = EFalse;
	iDSA = CDirectScreenAccess::NewL(aWs, aScreenDevice, aWindow, *this);
	iDSA->StartL();
	iViewFinderActive = ETrue;

	iDSA->Gc()->SetOrigin();
	if (!iClipRect.IsEmpty())
		{
		iDSA->Gc()->SetClippingRect(iClipRect);
		}

	// start view finder
	// The division transfer is going to support in the next version. 
	Start();
	}

/** 
Stops transfer of view finder data to the screen. 
*/
void CWebCameraVfActive::StopViewFinder()
	{
	if (!iViewFinderActive)
		{
		return;
		}

	iViewFinderActive = EFalse;

	delete iDSA;
	iDSA = NULL;

	delete iVfBuf;
	iVfBuf = NULL;

	//Stop view finder
	iDriver.StopViewFinder();
	Cancel();
	}

/** 
Queries whether the view finder is active.

@return  ETrue if the view finder is active. EFalse if the view finder is not
         active. 
*/
TBool CWebCameraVfActive::ViewFinderActive()
	{
	return iViewFinderActive;
	}

/** 
Start viewfinder process.
*/
void CWebCameraVfActive::Start()
	{
	RDebug::Print(_L("CWebCameraVfActive::Start"));

	iVfBufPtr.Set(iVfBuf->Des());
	iVfBufPtr.SetLength(0);
	//Start view finder
	iDriver.StartViewFinder(iStatus, iVfBufPtr);

	SetActive();
	}

/** 
Draw the image of the view finder on screen.
*/
void CWebCameraVfActive::UpdateViewL()
	{
	RDebug::Print(_L("CWebCameraVfActive::UpdateViewL"));

	delete iVfBitmap;
	iVfBitmap = NULL;

	CFbsBitmap* image = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(image);
	User::LeaveIfError(image->Create(TSize(KVfWidth, KVfHeight), EColor16M));
	CleanupStack::Pop(image);

	iVfBitmap = image;

// output received data log
//	RDebug::Print(_L("CWebCameraVfActive::UpdateView iVfBufLength[%d]"), iVfBufPtr.Length());
//	TBuf<256> hexBuf;
//	for (TInt i = 0; i < iVfBufPtr.Length(); i++)
//		{
//		hexBuf.AppendFormat(_L("%02X "), iVfBufPtr[i]);
//		if ((i % 16) == 15)
//			{
//			RDebug::Print(hexBuf);
//			hexBuf = KNullDesC;
//			}
//		}
//	RDebug::Print(hexBuf);

	// convert buffer data to Bitmap
	for (TInt height=0; height<KVfHeight; height++)
		{
		TInt pos = height * KVfLineBytes;
		TPtrC8 posptr = iVfBuf->Mid(pos, KVfLineBytes);
		TBuf8<KVfLineBytes>  buf;
		buf.Copy(posptr);
		iVfBitmap->SetScanLine(buf, (KVfHeight-1)-height);
		}

	// view position setting for test application
	iScreenRect.iTl.iY = 50;
	iScreenRect.iBr.iY = 169;

	// update view
	iDSA->Gc()->DrawBitmap(iScreenRect, iVfBitmap);
	iDSA->ScreenDevice()->Update();
	}

/** 
from MAbortDirectScreenAccess

This function is called by the window server when direct screen access must stop
(for example because a dialogue is moved in front of the area where direct screen access is taking place).

@param  aScreenDevice 
		The reason why direct screen access was terminated. 
*/
void CWebCameraVfActive::AbortNow(RDirectScreenAccess::TTerminationReasons /*aReason*/)
	{
	}

/** 
from 	from MDirectScreenAccess

This function is called by the window server as soon as direct screen access can resume.

@param  aScreenDevice 
		Provides the reason why direct screen access was terminated. 
*/
void CWebCameraVfActive::Restart(RDirectScreenAccess::TTerminationReasons /*aReason*/)
	{
	}

