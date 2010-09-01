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
//

#include "ColorQuantizer.h"

CColorQuantizer::CColorQuantizer(CPalette* aPalette, TUint8 aTransparencyThreshold)
: CActive(CActive::EPriorityIdle), iPalette(aPalette), iTransparencyThreshold(aTransparencyThreshold)
	{
	CActiveScheduler::Add(this);
	}

void CColorQuantizer::InitBitmapSettings(CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask, TInt aColorPixels)
	{
	iBitmap = &aSource;
	iDestination = &aDestination;
	iDestinationMask = aDestinationMask;
	iBitmapSize = aDestination.SizeInPixels();
	iColorPixels = aColorPixels;
	}

CColorQuantizer::~CColorQuantizer()
	{
	}

/*
* 
* RequestComplete
* @param "TInt aReason"
*
*/
void CColorQuantizer::RequestComplete(TInt aReason)
	{
	TRequestStatus* status = iRequestStatus;
	User::RequestComplete(status, aReason);
	}

/*
*
* SelfComplete
* This function activates the active object and 
* signals completion of the current asynchronous operation
*
* @param "TInt aReason"
*
*/
void CColorQuantizer::SelfComplete(TInt aReason)
	{
	SetActive();

	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aReason);
	}

void CColorQuantizer::DoCancel()
	{
	RequestComplete(KErrCancel);
	}
