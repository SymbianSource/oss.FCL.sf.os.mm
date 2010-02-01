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
// This is the public client API for the BitmapTransforms library
// 
//

#ifndef __BITMAPTRANSFORMSMAIN_H__
#define __BITMAPTRANSFORMSMAIN_H__


#include <e32std.h>

/**
@panic	ENoSourceBitmap is raised when the src bitmap supplied to one of the api's
		has not been created (i.e. it has a null handle)
		ENoDestinationBitmap is raised when the destination bitmap supplied to one 
		of the api's has not been properly created
		EBitmapHasZeroDimension is raised when a bitmap has zero dimension
*/

enum TBitmapTransformsPanic
	{
	ENoSourceBitmap,
	ENoDestinationBitmap,
	EBitmapHasZeroDimension,
	EBadArgumentScale,
	EBadArgumentRotate,
	ENullArgument,
	EBadInvariant
	};


/**

The function panic raises a panic from within the BitmapTransforms library
@param The argument is a member of the enumeration TBitmapTransformsPanic
@panic The panics raised by this function are:
	ENoSourceBitmap,
	ENoDestinationBitmap,
	EBitmapHasZeroDimension and are described in the enumeration

*/


GLDEF_C void Panic( TBitmapTransformsPanic aError);

#endif

