// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "imageprocessormain.h"

_LIT(KIPPanicCategory, "ImageProcessor");

/**
This function raises a panic

@param	aError
		one of the several panics codes that may be raised by this dll

@panic	EPanicNotInitialized is raised when the imageprocessor is not initialized.
@panic	EPanicProgressInfoNotAvailable is raised when the progress info is required but the 
		state of the imageprocessor is EUninitialized or EInitializing.
*/
GLDEF_C void Panic(TIPPanic aError)
	{
	User::Panic(KIPPanicCategory, aError);
	}

