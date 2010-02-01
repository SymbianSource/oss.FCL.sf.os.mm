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


#include "BitmapTransformsMain.h"

/**
@internalTechnology

Internal string constant that is passed through user panics
and identifies the DLL
*/
_LIT(KIclPanicCategory, "BitmapTransforms");

/**
@internalTechnology

This function raises a panic

@param	aError
		one of the several panics codes that may be raised by this dll

@panic	ENoSourceBitmap is raised when the src bitmap supplied to one of the api's
		has not been created (i.e. has a null handle)
@panic	ENoDestinationBitmap is raised when the destination bitmap supplied to one
		of the api's has not been properly created
@panic	EBitmapHasZeroDimension is raised when a bitmap has zero dimension
*/
GLDEF_C void Panic(TBitmapTransformsPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

