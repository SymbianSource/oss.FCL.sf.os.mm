// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __IMAGEDISPLAYMAIN_H__
#define __IMAGEDISPLAYMAIN_H__

/**    @file
   @internalComponent */
#include <e32std.h>

#include "ImageDisplayPanicCodes.h"

/** 	@file
	@internalTechnology */

// Global panic fn. for the ImageDisplay library
GLDEF_C void Panic(TImageDisplayPanic aError);

#endif  // __IMAGEDISPLAYMAIN_H__
