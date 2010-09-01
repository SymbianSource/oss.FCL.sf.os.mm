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
//

#include <mdaimageconverter.h>
#include "MdaImageConverterPriv.h"

//
// CMdaBitmapRotator
//

/**
Constructs and initialises a new instance of the image rotator.

The function leaves if the image rotator object cannot be created.

@return A pointer to the new image rotator object.
*/
EXPORT_C CMdaBitmapRotator* CMdaBitmapRotator::NewL()
	{
	CMdaBitmapRotator* self = new(ELeave) CMdaBitmapRotator;
	CleanupStack::PushL(self);
	self->iProperties = CMdaBitmapRotatorPriv::NewL();
	CleanupStack::Pop(); // self
	return self;
	}

/**
Default destructor.

Frees all associated resources.
*/
EXPORT_C CMdaBitmapRotator::~CMdaBitmapRotator() 
	{
	delete iProperties;
	}

/**
Begins the rotation of a bitmap clockwise through the specified angle and writes
the output to the specified file.

The operation is asynchronous. When it is complete, successfully or
otherwise, the callback function
MMdaImageUtilObserver::MiuoConvertComplete() is called,
passing the state of the operation.

@param     aObserver
           The image utility observer interface.
@param     aSrcBitmap
           The bitmap to be rotated.
@param     aTgtBitmap
           The target location for the rotated bitmap.
@param     aAngle
           The angle through which the bitmap is to be rotated.
*/
EXPORT_C void CMdaBitmapRotator::RotateL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aSrcBitmap,CFbsBitmap& aTgtBitmap,TRotationAngle aAngle)
	{
	iProperties->RotateL(aObserver, aSrcBitmap, aTgtBitmap, aAngle);
	}

/**
Begins the rotation of a bitmap clockwise through the specified angle.

The operation is asynchronous. When it is complete, successfully or
otherwise, the callback function
MMdaImageUtilObserver::MiuoConvertComplete() is called,
passing the state of the operation.

@param     aObserver
           The image utility observer interface.
@param     aBitmap
           The bitmap to be rotated. This reference is also the
           target location for the rotated bitmap.
@param     aAngle
           The angle through which the bitmap is to be rotated.
*/
EXPORT_C void CMdaBitmapRotator::RotateL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aBitmap,TRotationAngle aAngle)
	{
	iProperties->RotateL(aObserver, aBitmap, aAngle);
	}

/**
Cancels the bitmap rotation operation.

The observer callback function
MMdaImageUtilObserver::MiuoConvertComplete() is not
called.
*/
EXPORT_C void CMdaBitmapRotator::CancelRotation()
	{
	iProperties->CancelRotation();
	}
