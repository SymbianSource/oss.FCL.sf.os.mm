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
// ROTATION.CPP
// 
//

#include <mdaimageconverter.h>
#include "MdaImageConverterPriv.h"

//
// CMdaBitmapRotator
//

CMdaBitmapRotatorPriv* CMdaBitmapRotatorPriv::NewL()
	{
	CMdaBitmapRotatorPriv* self = new(ELeave) CMdaBitmapRotatorPriv;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CMdaBitmapRotatorPriv::CMdaBitmapRotatorPriv():
	CActive(CActive::EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

void CMdaBitmapRotatorPriv::ConstructL()
	{
	iBitmapRotator = CBitmapRotator::NewL();
	}

CMdaBitmapRotatorPriv::~CMdaBitmapRotatorPriv() 
	{
	Cancel();
	delete iBitmapRotator;
	}

void CMdaBitmapRotatorPriv::RotateL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aSrcBitmap,CFbsBitmap& aTgtBitmap,CMdaBitmapRotator::TRotationAngle aAngle)
	{
	CancelRotation();

	iObserver = &aObserver;

	iBitmapRotator->Rotate(&iStatus, aSrcBitmap, aTgtBitmap, MapRotationAngle(aAngle));
	SetActive();
	}

void CMdaBitmapRotatorPriv::RotateL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aBitmap,CMdaBitmapRotator::TRotationAngle aAngle)
	{	
	CancelRotation();

	iObserver = &aObserver;

	iBitmapRotator->Rotate(&iStatus, aBitmap, MapRotationAngle(aAngle));
	SetActive();
	}

void CMdaBitmapRotatorPriv::CancelRotation()
	{
	Cancel();
	}

void CMdaBitmapRotatorPriv::RunL()
	{
	ASSERT(iObserver);
	iObserver->MiuoConvertComplete(iStatus.Int());
	}

void CMdaBitmapRotatorPriv::DoCancel()
	{
	iBitmapRotator->Cancel();
	}

CBitmapRotator::TRotationAngle CMdaBitmapRotatorPriv::MapRotationAngle(CMdaBitmapRotator::TRotationAngle aAngle)
	{
	ASSERT(TInt(CBitmapRotator::ERotation90DegreesClockwise)==TInt(CMdaBitmapRotator::ERotation90DegreesClockwise));
	ASSERT(TInt(CBitmapRotator::ERotation180DegreesClockwise)==TInt(CMdaBitmapRotator::ERotation180DegreesClockwise));
	ASSERT(TInt(CBitmapRotator::ERotation270DegreesClockwise)==TInt(CMdaBitmapRotator::ERotation270DegreesClockwise));
	ASSERT(TInt(CBitmapRotator::EMirrorHorizontalAxis)==TInt(CMdaBitmapRotator::EMirrorFlipHorizontal));
	ASSERT(TInt(CBitmapRotator::EMirrorVerticalAxis)==TInt(CMdaBitmapRotator::EMirrorFlipVertical));

	// We have to swap these flags over as they have different meanings in MDA and ICL
	CBitmapRotator::TRotationAngle angle = CBitmapRotator::TRotationAngle(aAngle);
	switch (aAngle)
		{
		case CMdaBitmapRotator::EMirrorFlipHorizontal:
			angle = CBitmapRotator::EMirrorVerticalAxis;
			break;
		case CMdaBitmapRotator::EMirrorFlipVertical:
			angle = CBitmapRotator::EMirrorHorizontalAxis;
			break;
		default:
			// Do nothing
			break;
		}
	return angle;
	}
