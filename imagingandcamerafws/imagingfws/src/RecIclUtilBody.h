// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the body of the ICL recognizer utility class
// 
//

#ifndef __RECICLUTILBODY_H__
#define __RECICLUTILBODY_H__

#include <ecom/implementationinformation.h>
#include <ecom/ecom.h>
#include "ImageConversion.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <iclrecognizerutil.h>
#endif

/**
 * @internalComponent
 *
 * Body of ICL utility class used by ICL recognizer
 * Maintains an array of CImplementationInformation objects
 * so that data headers can be speedily matched against.
 * Updates the array when notified by ECOM of a change to the global
 * interface implementation registration data.
 * 
 */
NONSHARABLE_CLASS( CIclRecognizerUtil::CBody ): public CActive
	{
public:
	static CBody* NewL();
	~CBody();
	TBool GetMimeTypeL(const TDesC8& aImageData, const TDesC& aFileName, TDes8& aMimeType);

protected:
	virtual void DoCancel();
	virtual void RunL();

private:
	CBody();
	void ConstructL();
	void StartNotification();
	void BuildControllerListL();

private:
	/** an array of pointers to CImplementationInformation objects */
	RImplInfoPtrArray iPluginArray;
	/** for opening a session to the ECom server */
	REComSession iEcomSession;
    /** used when creating decoders */
    RFs iFs;
	};

#endif
