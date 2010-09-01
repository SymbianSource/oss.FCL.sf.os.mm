// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
Constructs a CBitmapRotator object.

If any errors are encountered this function leaves with an appropriate leave code.

@return A pointer to a CBitmapRotator object.
*/

EXPORT_C CBitmapRotator* CBitmapRotator::NewL()
	{
	CBitmapRotator* self = new(ELeave) CBitmapRotator();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
Constructor for this class.

@internalTechnology
*/
CBitmapRotator::CBitmapRotator()
	{
	}

/*
Performs second phase of contruction.

@internalTechnology
*/
void CBitmapRotator::ConstructL()
	{
	iBody = CBitmapRotatorBody::NewL();
	}

/**
Default destructor for this class.

Frees all resources owned by the object prior to its destruction.
*/

EXPORT_C CBitmapRotator::~CBitmapRotator()
	{
	Cancel();

	delete iBody;
	}

/**
Cancels any outstanding action.
*/

EXPORT_C void CBitmapRotator::Cancel()
	{
	if (iBody)
		{
		iBody->Cancel();
		}
	}

/**
Schedules a rotate/mirror operation on a bitmap supplied in the aSrcBitmap paramter and
produces the output pointed to by aTgtBitmap.
The CBitmapRotator is an active object and as such provides asynchronous operations. The result of the
operation is pointed to by aRequestStatus.

@pre
- aRequestStatus is not a NULL pointer.
- aSrcBitmap is a fully constructed bitmap of unknown size including zero dimension
- aTgtBitmap is a fully constructed bitmap of unknown size including zero dimension
- aAngle is a member of the enumeration TRotationAngle.

@post
If successful aTgtBitmap contains the rotated bitmap and aRequestStatus points to the value KErrNone.
If unsuccessful the bitmap supplied in aSrcBitmap will remain unaltered.

@param	aRequestStatus
		A pointer to the completion status of the asynchronous operation.
@param	aSrcBitmap
		This bitmap should have been created
@param	aTgtBitmap
		This bitmap should have been created
@param	aAngle
		A member of the enumeration TRotationAngle that specifies the rotation mirror operation.

@panic	ENoSourceBitmap
        This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not 
        been constructed ie. its handle is zero.
*/

EXPORT_C void CBitmapRotator::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle)
	{
	ASSERT(iBody);
	iBody->Rotate(aRequestStatus, aSrcBitmap, aTgtBitmap, aAngle);
	}

/**
The function Rotate schedules a rotate/mirror operation on a bitmap supplied in the
aBitmap whose output overwrites aBitmap. The result of the
operation is pointed to by aRequestStatus.

@pre
- aRequestStatus is not a NULL pointer.
- aBitmap is a fully constructed bitmap of unknown size including zero dimension.
- aAngle is a member of the enumeration TRotationAngle.

@param	aRequestStatus
		A pointer to the completion status of the asynchronous operation.
@param	aBitmap
		A reference to a CFbsBitmap. This bitmap should have been created
		and is also an output.
@param	aAngle
		A member of the enumeration TRotationAngle that specifies the rotation mirror operation.

@panic	ENoSourceBitmap
        This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not 
        been constructed ie. its handle is 0.

@post
- aBitmap contains the rotated bitmap.
- aRequestStatus points to the value KErrorNone.
*/

EXPORT_C void CBitmapRotator::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle)
	{
	ASSERT(iBody);
	iBody->Rotate(aRequestStatus, aBitmap, aAngle);
	}
/**
Provides custom command capabilties on CBitmapRotator. The command is dispatched on aUid and if 
the command is not known KErrNotSupported will be returned. This function is synchronous.

@pre
When a command requires parameters aParam is not NULL.

@param	aUid
		The ID of the command.
@param 	aParam
		The command specific information.
*/
EXPORT_C TInt CBitmapRotator::CustomCommand(TUid aUid, TAny* aParam)
		{
		ASSERT(iBody);
		return iBody->CustomCommand( aUid, aParam );
		}
