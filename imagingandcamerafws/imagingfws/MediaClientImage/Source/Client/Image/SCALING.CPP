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
// CMdaBitmapScaler for Image Utility
//

/**
Constructs and initialises a new instance of the image scaler.

The function leaves if the image scaler object cannot be created.

@return A pointer to the new image scaler object.
*/
EXPORT_C CMdaBitmapScaler* CMdaBitmapScaler::NewL()
	{
	CMdaBitmapScaler* self = new(ELeave) CMdaBitmapScaler;
	CleanupStack::PushL(self);
	self->iProperties = CMdaBitmapScalerPriv::NewL();
	CleanupStack::Pop(); // self
	return self;
	}

/**
Default destructor.

Frees all associated resources.
*/
EXPORT_C CMdaBitmapScaler::~CMdaBitmapScaler()
	{
	delete iProperties;
	}

/**
Begins the bitmap re-scaling operation.

The scaling factor is based on the relative sizes of the source
and target bitmaps. The operation is asynchronous. 

When it is completed, successfully or otherwise, the callback function
MMdaImageUtilObserver::MiuoConvertComplete() is called,
passing the state of the operation.

@param     aObserver
           The image utility observer interface.
@param     aSrcBitmap
           The bitmap to be re-scaled.
@param     aTgtBitmap
           The target location for the re-scaled bitmap.
@param     aMaintainAspectRatio
           ETrue - the aspect ratio is retained;
           this is the default. The same scaling factor is
           applied in both the horizontal and vertical
           directions. This is the smaller of the horizontal
           scaling factor and the vertical scaling factor.
           EFalse - the aspect ratio need not be
           retained.
*/
EXPORT_C void CMdaBitmapScaler::ScaleL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aSrcBitmap,CFbsBitmap& aTgtBitmap,TBool aMaintainAspectRatio)
	{
	iProperties->ScaleL(aObserver, aSrcBitmap, aTgtBitmap, aMaintainAspectRatio);
	}

/**
Begins the bitmap re-scaling operation.

The scaling factor is based on the relative value of the source bitmap
size and the explicitly supplied size. The operation is asynchronous.
When it is complete, successfully or otherwise, the callback function
MMdaImageUtilObserver::MiuoConvertComplete() is called,
passing the state of the operation.

@param     aObserver
           The image utility observer interface.
@param     aBitmap
           The bitmap to be re-scaled. This reference is also the
           target location for the re-scaled bitmap.
@param     aDestSize
           The requested target size for the re-scaled bitmap.
@param     aMaintainAspectRatio
           ETrue - the aspect ratio is retained;
           this is the default. The same scaling factor is
           applied in both the horizontal and vertical
           directions. This is the smaller of the horizontal
           scaling factor and the vertical scaling factor.
           EFalse - the aspect ratio need not be
           retained.
*/
EXPORT_C void CMdaBitmapScaler::ScaleL(MMdaImageUtilObserver& aObserver,CFbsBitmap& aBitmap,TSize aDestSize,TBool aMaintainAspectRatio)
	{
	iProperties->ScaleL(aObserver, aBitmap, aDestSize, aMaintainAspectRatio);
	}

/**
Cancels the bitmap re-scaling operation.

The observer callback function
MMdaImageUtilObserver::MiuoConvertComplete() is not
called.
*/
EXPORT_C void CMdaBitmapScaler::CancelScaling()
	{
	iProperties->CancelScaling();
	}
