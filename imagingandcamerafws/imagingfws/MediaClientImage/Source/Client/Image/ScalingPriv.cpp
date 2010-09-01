// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SCALING.CPP
// 
//

#include <mdaimageconverter.h>
#include "MdaImageConverterPriv.h"

//
// CMdaBitmapScalerPriv for Image Utility
//

CMdaBitmapScalerPriv* CMdaBitmapScalerPriv::NewL()
	{
	CMdaBitmapScalerPriv* self = new(ELeave) CMdaBitmapScalerPriv;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CMdaBitmapScalerPriv::CMdaBitmapScalerPriv():
	CActive(CActive::EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

void CMdaBitmapScalerPriv::ConstructL()
	{
	iBitmapScaler = CBitmapScaler::NewL();
	}

CMdaBitmapScalerPriv::~CMdaBitmapScalerPriv()
	{
	Cancel();
	delete iBitmapScaler;
	}

void CMdaBitmapScalerPriv::ScaleL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aSrcBitmap,CFbsBitmap& aTgtBitmap,TBool aMaintainAspectRatio)
	{
	CancelScaling();

	iObserver = &aObserver;
	iBitmapScaler->Scale(&iStatus, aSrcBitmap, aTgtBitmap, aMaintainAspectRatio);
	SetActive();
	}

void CMdaBitmapScalerPriv::ScaleL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aBitmap,TSize aDestSize,TBool aMaintainAspectRatio)
	{
	// Cancel any scaling operation currently in progress
	CancelScaling();

	iObserver = &aObserver;
	iBitmapScaler->Scale(&iStatus, aBitmap, aDestSize, aMaintainAspectRatio);
	SetActive();
	}

void CMdaBitmapScalerPriv::CancelScaling()
	{	
	Cancel();
	}

void CMdaBitmapScalerPriv::RunL()
	{
	ASSERT(iObserver);
	iObserver->MiuoConvertComplete(iStatus.Int());
	}

void CMdaBitmapScalerPriv::DoCancel()
	{
	iBitmapScaler->Cancel();
	}
