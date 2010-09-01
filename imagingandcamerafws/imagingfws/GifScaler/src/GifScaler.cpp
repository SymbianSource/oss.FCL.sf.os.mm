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

#include "gifscaler.h"
#include "GifScalerMain.h"
#include "GifScalerBody.h"

/**
The function NewL constructs a CGifScaler.
This version constructs a scaler for scaling and color quantizing only.
No mask data is required in this case)

@param     aSource
           The bitmap to be re-scaled.
@param     aOptions
           Scaling and color quantization options.

@leave KErrArgument if the source bitmap or the options are invalid.
@panic ENoSourceBitmap if the source bitmap has not been created.

@return A pointer to a fully constructed CGifScaler
*/

EXPORT_C CGifScaler* CGifScaler::NewL(CFbsBitmap& aSource, TOptions aOptions)
	{
	CGifScaler* self = new(ELeave) CGifScaler();
	CleanupStack::PushL(self);
	self->ConstructL(aSource, NULL, aOptions);

	CleanupStack::Pop(self);
	return self;
	}

/**
The function NewL constructs a CGifScaler.
This version constructs a scaler for scaling and color quantizing an image with transparency.

@param     aSource
           The bitmap to be re-scaled.
@param     aSourceMask
           The mask bitmap that provides the transparency data.
@param     aOptions
           Scaling and color quantization options.

@leave KErrArgument if the source bitmap, source mask bitmap, or the options are invalid.
@panic ENoSourceBitmap if the source bitmap has not been created.
@panic ENoSourceMaskBitmap if the source mask bitmap has not been created.

@return A pointer to a fully constructed CGifScaler
*/

EXPORT_C CGifScaler* CGifScaler::NewL(CFbsBitmap& aSource, CFbsBitmap& aSourceMask, TOptions aOptions)
	{
	CGifScaler* self = new(ELeave) CGifScaler();
	CleanupStack::PushL(self);
	self->ConstructL(aSource, &aSourceMask, aOptions);

	CleanupStack::Pop(self);
	return self;
	}


/*
Constructor for this class.

@internalTechnology
*/

CGifScaler::CGifScaler()
	{
	}


/**
ConstructL
Performs second phase of contruction

@internalTechnology
*/

void CGifScaler::ConstructL(CFbsBitmap& aSource, CFbsBitmap* aSourceMask, TOptions aOptions)
	{
	iBody = CBody::NewL(aSource, aSourceMask, aOptions);
	}


/**
This is the destructor for the CGifScaler
and is responsible for deallocating all resources
allocated by the CGifScaler.
*/

EXPORT_C CGifScaler::~CGifScaler()
	{
	Cancel();
	delete iBody;
	}

/**
Scale the source bitmap and mask to produce a single 8bpp bitmap and an output palette.
If a mask was supplied during construction the last index in the palette will be used for transparency.

The scaling factor is based on the relative sizes of the source
and target bitmaps. The operation is asynchronous. When it is
complete, successfully or otherwise, the TRequestStatus
aStatus is set, passing the state of the operation.

@param     aStatus
           Request status to signal when scaling is complete.
@param     aDestination
           The destination bitmap. (EColor256)
@param     aPalette
           The output palette.
@param     aMaintainAspectRatio
           ETrue - the aspect ratio is retained;
           this is the default. The same scaling factor is
           applied in both the horizontal and vertical
           directions. This is the smaller of the horizontal
           scaling factor and the vertical scaling factor.
           EFalse - the aspect ratio need not be
           retained.
*/

EXPORT_C void CGifScaler::Scale(TRequestStatus* aStatus, CFbsBitmap& aDestination, CPalette& aPalette, TBool aMaintainAspectRatio)
	{
	ASSERT(iBody);
	iBody->Scale(aStatus, aDestination, aPalette, aMaintainAspectRatio);
	}

/**
Scale the source bitmap and mask to produce a single 8bpp bitmap and an output palette,
specifying a transparency level to be used when determining whether destination pixels are transparent.
If a mask was supplied during construction the last index in the palette will be used for transparency.

The scaling factor is based on the relative sizes of the source
and target bitmaps. The operation is asynchronous. When it is
complete, successfully or otherwise, the TRequestStatus
aStatus is set, passing the state of the operation.

@param     aStatus
           Request status to signal when scaling is complete.
@param     aDestination
           The destination bitmap. (EColor256)
@param     aPalette
           The output palette.
@param     aTransparencyThreshold
           The transparency level used to determine if destination pixels are transparent.
@param     aMaintainAspectRatio
           ETrue - the aspect ratio is retained;
           this is the default. The same scaling factor is
           applied in both the horizontal and vertical
           directions. This is the smaller of the horizontal
           scaling factor and the vertical scaling factor.
           EFalse - the aspect ratio need not be
           retained.
*/

EXPORT_C void CGifScaler::ThresholdScale(TRequestStatus* aStatus, CFbsBitmap& aDestination, CPalette& aPalette, TUint8 aTransparencyThreshold, TBool aMaintainAspectRatio)
	{
	ASSERT(iBody);
	iBody->Scale(aStatus, aDestination, aPalette, aTransparencyThreshold, aMaintainAspectRatio);
	}

/**
Cancel any outstanding activity.
*/

EXPORT_C void CGifScaler::Cancel()
	{
	if (iBody)
		iBody->Cancel();
	}
