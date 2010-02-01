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
// This is the implementation of the BitmapTransforms Library
// 
//

#ifndef __BitmapRotationBody_inl
#define __BitmapRotationBody_inl

#include "BitmapTransformsMain.h"
#include <bitmtrans/bitmtransplugin.h>

/**
The function Rotate schedules a rotate/mirror operation on a bitmap supplied in the 
aSrcBitmap whose output overwrites aSrcBitmap.
Preconditions:
aRequestStatus is not a NULL pointer
aSrcBitmap     is a fully constructed bitmap of unknown size including zero dimension
aAngle         is a member of the enumeration TRotationAngle

@param "aRequestStatus" 
       is a pointer to the completion status of the asynchronous operation.
@param "CFbsBitmap& aBitmap" 
       is a reference to a CFbsBitmap. This bitmap should have been created
       and is also an output
@param "TRotationAngle aAngle"
       aAngle is a member of the enumeration TRotationAngle and specifies the rotation mirror operation   

@panic  This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not been constructed
ie its handle is 0

Sucess Guarantee
aSrcBitmap      contains the rotated bitmap
aRequestStatus  points to the value KErrorNone

*/

inline void CBitmapRotatorBody::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle)
	{
	iPlugin->Rotate(aRequestStatus, aBitmap, aAngle);
	}

// Pass Cancel to plugin
inline void CBitmapRotatorBody::Cancel()
	{
	iPlugin->Cancel();
	}

#endif  // __BitmapRotationBody_inl
