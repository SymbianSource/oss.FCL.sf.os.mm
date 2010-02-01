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
// BitmapRotation.cpp
// 
//

#include "BitmapTransformsBody.h"
#include "BitmapTransformsMain.h"
#include <bitmtrans/bitmtransplugin.h>
#include <bitmtrans/bitmtransuids.hrh>

#include <ecom/ecom.h>
#include <mm/mmpluginutils.h>

/**
The function NewL constructs a CBitmapRotatorBody

@returns CBitmapRotatorBody* 


*/

CBitmapRotatorBody* CBitmapRotatorBody::NewL()
	{
	CBitmapRotatorBody* self = new(ELeave) CBitmapRotatorBody();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Constructor for this class. 

*/

CBitmapRotatorBody::CBitmapRotatorBody()
	{
	}


/**
Performs second phase of contruction
*
*/

void CBitmapRotatorBody::ConstructL()
	{
	TUid interfaceUid = {KUidBitmapRotatePlugin};
	iPlugin = 
		static_cast<MBitmapRotatorPlugin*>
			(MmPluginUtils::CreateImplementationL(interfaceUid, iDestructorKey, KBitmTransPluginMatchString, KRomOnlyResolverUid));
	}

/**
*
* Default destructor for this class.
*
*/

CBitmapRotatorBody::~CBitmapRotatorBody()
	{
	delete iPlugin;
	REComSession::DestroyedImplementation(iDestructorKey);
	REComSession::FinalClose();
	}


/**
The Rotate function schedules a rotate/mirror operation on a bitmap supplied in the srcBitmap and 
produces the output in the tgtBitmap.
The CBitmapRotatorBody is an active object and as such provides asynchronous operations

Preconditions:
aRequestStatus is not a NULL pointer
aSrcBitmap     is a fully constructed bitmap of unknown size including zero dimension
aTgtBitmap     is a fully constructed bitmap of unknown size including zero dimension
aAngle         is a member of the enumeration TRotationAngle 

Sucess Guarantee:
aTgtBitmap      contains the rotated bitmap
aRequestStatus  points to the value KErrNone

Minimal Guarantee:
The bitmap supplied in aSrcBitmap is unaltered


@param "TRequestStatus* aRequestStatus"
       a pointer to the completion status of the asynchronous operation.
@param "CFbsBitmap& aSrcBitmap"
       This bitmap should have been created
@param "CFbsBitmap& aTgtBitmap"
       This bitmap should have been created
@param "TRotationAngle aAngle" 
       is a member of the enumeration TRotationAngle and specifies the rotation mirror operation


@panic This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not been constructed
       i.e. its handle is zero


*/

void CBitmapRotatorBody::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle)
	{
	iPlugin->Rotate(aRequestStatus, aSrcBitmap, aTgtBitmap, aAngle);
	}

/*
*
* CustomCommand
* @param aUid
* @param aParam
* @return 'TInt' an error code indicating success or failure of the 
* command
*
*/
TInt CBitmapRotatorBody::CustomCommand(TUid aUid, TAny* aParam)
	{
	return iPlugin->CustomCommand(aUid, aParam);
	}
