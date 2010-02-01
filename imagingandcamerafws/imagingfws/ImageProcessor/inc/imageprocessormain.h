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

#ifndef __IMAGEPROCESSORMAIN_H__

#define __IMAGEPROCESSORMAIN_H__

#include <e32std.h>

enum TIPPanic
	{
	EPanicNotInitialized=1,						// 1
	EPanicProgressInfoNotAvailable=2			// 2
	};

GLDEF_C void Panic(TIPPanic aPanicCat);

#endif // ndef __IMAGEPROCESSORMAIN_H__
