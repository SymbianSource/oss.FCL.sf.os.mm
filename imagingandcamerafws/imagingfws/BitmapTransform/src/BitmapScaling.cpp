// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "BitmapTransforms.h"
#include "BitmapTransformsMain.h"
#include "BitmapTransformsBody.h"

/**
Constructs a CBitmapScaler object.

@return A pointer to a CBitmapScaler object.
*/
EXPORT_C CBitmapScaler* CBitmapScaler::NewL()
	{
	CBitmapScaler* self = new(ELeave) CBitmapScaler();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/*
@internalTechnology

Constructor for this class.
*/

CBitmapScaler::CBitmapScaler()
	{
	}

/**
@internalTechnology

Performs second phase of contruction
*/

void CBitmapScaler::ConstructL()
	{
	iBody = CBitmapScalerBody::NewL();
	}


/**
This is the destructor for the CBitmapScaler
and is responsible for deallocating all resources
alloctaed by the CBitmapScaler.
*/

EXPORT_C CBitmapScaler::~CBitmapScaler()
	{
	Cancel();
	delete iBody;
	}

/**
Cancel any outstanding activity.
*/

EXPORT_C void CBitmapScaler::Cancel()
	{
	if (iBody)
		iBody->Cancel();
	}

/**
Begins the bitmap re-scaling operation.

The scaling factor is based on the relative sizes of the source
and target bitmaps. The operation is asynchronous. KErrOverFlow status is 
set if the sizes of input bitmaps are too large for the algorithm to handle.

@post
The result of the operation is pointed to by aRequestStatus.

@param	aRequestStatus
		On return, contains a pointer to the completion status of the rescale of the bitmap.
@param	aSrcBitmap
		The bitmap to be re-scaled.
@param	aTgtBitmap
		The target location for the re-scaled bitmap.
@param	aMaintainAspectRatio
		A boolean indicating if the aspect ratio is maintained. ETrue means the aspect ratio is 
        retained; this is the default. The same scaling factor is applied in both the horizontal 
        and vertical directions. This is the smaller of the horizontal scaling factor and the 
        vertical scaling factor. EFalse means the aspect ratio need not be retained.
*/
EXPORT_C void CBitmapScaler::Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TBool aMaintainAspectRatio)
	{
	ASSERT(iBody);
	iBody->Scale(aRequestStatus, aSrcBitmap, aTgtBitmap, aMaintainAspectRatio);
	}

/**
Begins the bitmap re-scaling operation.

The scaling factor is based on the relative value of the source bitmap
size and the explicitly supplied size. The operation is asynchronous.
When it is complete, successfully or otherwise, the
TRequestStatus is set, passing the state of the operation. KErrOverFlow status is set 
if the sizes of input bitmaps are too large for the algorithm to handle.
@param	aRequestStatus
		On return, contains a pointer to the completion status of the rescale of the bitmap.
@param	aBitmap
		The bitmap to be re-scaled. This reference is also the
		target location for the re-scaled bitmap.
@param	aDestinationSize
		The requested target size for the re-scaled bitmap.
@param	aMaintainAspectRatio
        A boolean indicating if the aspect ratio is maintained. ETrue means the aspect ratio is 
        retained; this is the default. The same scaling factor is applied in both the horizontal 
        and vertical directions. This is the smaller of the horizontal scaling factor and the 
        vertical scaling factor. EFalse means the aspect ratio need not be retained.
*/
EXPORT_C void CBitmapScaler::Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, const TSize& aDestinationSize, TBool aMaintainAspectRatio)
	{
	ASSERT(iBody);
	iBody->Scale(aRequestStatus, aBitmap, aDestinationSize, aMaintainAspectRatio);
	}

/**
Provides custom command capabilties on CBitmapScaler. The command is dispatched on aUid and if 
the command is not known KErrNotSupported will be returned. This function is synchronous.

The Default implementation will support one command
to enable and disable post processing on the scale operation
The uid of this command is published in the header file.
Changes to the status of Post Processing Enabled will become
effective only after an exising scaling operation has completed.

@pre
When a command requires parameters aParam is not NULL.

@param	aUid
		The ID of the command.
@param 	aParam
		The command specific information.
*/
EXPORT_C TInt CBitmapScaler::CustomCommand(TUid aUid, TAny* aParam)
		{
		ASSERT(iBody);
		return iBody->CustomCommand( aUid, aParam );
		}

