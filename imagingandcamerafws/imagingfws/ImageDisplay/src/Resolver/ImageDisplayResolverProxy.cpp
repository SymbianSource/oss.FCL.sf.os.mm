/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

/** @file
	@internalComponent */

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

#include "ImageDisplayResolver.h"
#include "../inc/icl/ImageDisplay.hrh"


// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for ImageDisplayResolver
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidDisplayImplementationResolver, CImageDisplayResolver::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
