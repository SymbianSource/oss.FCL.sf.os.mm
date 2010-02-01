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
// BitmapScaling.cpp
// 
//

#include "BitmapTransformsBody.h"
#include "BitmapTransformsMain.h"
#include "BitmapTransforms.h"
#include <bitmtrans/bitmtransplugin.h>
#include <bitmtrans/bitmtransuids.hrh>

#include <ecom/ecom.h>
#include <mm/mmpluginutils.h>

/*
*The function NewL constructs a CBitmapScalerBody
*
*@return CBitmapScalerBody* 
*/
CBitmapScalerBody* CBitmapScalerBody::NewL()
	{
	CBitmapScalerBody* self = new(ELeave) CBitmapScalerBody();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
*
*CBitmapScalerBody()
*Constructor for this class. 
*/
CBitmapScalerBody::CBitmapScalerBody()
	{
	}

/*
*
*ConstructL()
*Performs second phase of construction.
*/
void CBitmapScalerBody::ConstructL()
	{
	TUid interfaceUid = {KUidBitmapScalerPlugin};
	iPlugin = 
		static_cast<MBitmapScalerPlugin*>
			(MmPluginUtils::CreateImplementationL(interfaceUid, iDestructorKey, KBitmTransPluginMatchString, KRomOnlyResolverUid));
	}


/*
*
*~CBitmapScalerBody
*This is the destructor for the CBitmapScalerBody 
*and is resposible for deallocating all resources 
*allocated by the CBitmapScalerBody.
*/
CBitmapScalerBody::~CBitmapScalerBody()
	{
	delete iPlugin;
	REComSession::DestroyedImplementation(iDestructorKey);
	REComSession::FinalClose();
	}

/*
 *
 * Begins the bitmap re-scaling operation.
 *
 * The scaling factor is based on the relative sizes of the source
 * and target bitmaps. The operation is asynchronous. When it is
 * complete, successfully or otherwise, the TRequestStatus &
 * aStatus is set, passing the state of the operation.
 *
 * @param     TRequestStatus* aRequestStatus
 *
 * @param     "CFbsBitmap& aSrcBitmap"
 *            The bitmap to be re-scaled.
 * @param     "CFbsBitmap& aTgtBitmap"
 *            The target location for the re-scaled bitmap.
 * @param     "TBool aMaintainAspectRatio = ETrue"
 *            ETrue  - the aspect ratio is retained;
 *            this is the default. The same scaling factor is
 *            applied in both the horizontal and vertical
 *            directions. This is the smaller of the horizontal
 *            scaling factor and the vertical scaling factor.
 *             EFalse  - the aspect ratio need not be
 *            retained.
 *
 *
 */
void CBitmapScalerBody::Scale(TRequestStatus* aRequestStatus,
							  CFbsBitmap& aSrcBitmap,
							  CFbsBitmap& aTgtBitmap,
							  TBool aMaintainAspectRatio)
	{
	iPlugin->Scale(aRequestStatus, aSrcBitmap, aTgtBitmap, aMaintainAspectRatio);
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
TInt CBitmapScalerBody::CustomCommand(TUid aUid, TAny* aParam)
	{
	return iPlugin->CustomCommand(aUid, aParam);
	}
