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

#ifndef ECAMWEBCAMERAVFACTIVE_H
#define ECAMWEBCAMERAVFACTIVE_H

#include "ecamwebcameraplugin.h"

/**
 *  Perform a viewfinder function.
 */
class CWebCameraVfActive : public CActive
						 , public MDirectScreenAccess
	{
public:
	 /**
     * Two-phased constructor.
     * @param aOwner pointer to callback object.
     * @param aDriver reference to driver.
     */
	static CWebCameraVfActive* NewL(MWebCameraVfActiveCallBack* aOwner, RWebcameraDevice& aDriver);

	CWebCameraVfActive(MWebCameraVfActiveCallBack* aOwner, RWebcameraDevice& aDriver);

	/**> Destructor */
	~CWebCameraVfActive();

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
	void StartViewFinderDirectL(RWsSession& aWs, CWsScreenDevice& aScreenDevice, RWindowBase& aWindow, TRect& aScreenRect, TRect& aClipRect);

	/** 
	Stops transfer of view finder data to the screen. 
	*/
	void StopViewFinder();

	/** 
	Queries whether the view finder is active.

	@return  ETrue if the view finder is active. EFalse if the view finder is not
	         active. 
	*/
	TBool ViewFinderActive();

private:
	/** 
	from MAbortDirectScreenAccess
	
	This function is called by the window server when direct screen access must stop
	(for example because a dialogue is moved in front of the area where direct screen access is taking place).
	
	@param  aScreenDevice 
			The reason why direct screen access was terminated. 
	*/
	virtual void AbortNow(RDirectScreenAccess::TTerminationReasons aReason);

private: 
	/** 
	from 	from MDirectScreenAccess
	
	This function is called by the window server as soon as direct screen access can resume.
	
	@param  aScreenDevice 
			Provides the reason why direct screen access was terminated. 
	*/
	virtual void Restart(RDirectScreenAccess::TTerminationReasons aReason);

private:
	void ConstructL();
	
	/** 
	Start viewfinder process.
	*/
	void Start();
	
	/** 
	Draw the image of the view finder on screen.
	*/
	void UpdateViewL();

private:
	void RunL();
	void DoCancel();

private:
	MWebCameraVfActiveCallBack*		iOwner;				///< origin of start.
	TBool							iViewFinderActive;	///< Status of viewfinder
	RWebcameraDevice				iDriver;			///< Camera driver

	CFbsBitmap*						iVfBitmap;			///< Bitmap data of viewfinder.
	HBufC8*							iVfBuf;				///< Data buffer of viewfinder.
	TPtr8							iVfBufPtr;			///< Pointer to data buffer of viewfinder.

	CDirectScreenAccess*			iDSA;				///< DirectScreenAccess object.
	TRect							iScreenRect;		///< The region to draw.
	TRect							iClipRect;			///< The clipping region to draw.

	TBool							iTestFlug;			///< for test
	};


#endif // ECAMWEBCAMERAPLUGIN_H
