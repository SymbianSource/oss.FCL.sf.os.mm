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

#include "BitmapTransformsPlugin.h"
#include <bitmtrans/bitmtransuids.hrh>

#include <ecom/implementationproxy.h>

// __________________________________________________________________________
// Exported proxy for instantiation method resolution
// Define the interface UIDs
const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(KUidBitmapScalerRefPlugin,	CBitmapScalerPlugin::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidBitmapRotateRefPlugin,	CBitmapRotatorPlugin::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}


/**
@internalComponent

Internal string constant that is passed through user panics
and identifies the DLL
*/
_LIT(KIclPanicCategory, "BitmapTransformsRefDll");

/**
@internalComponent

This function raises a panic

*/
GLDEF_C void Panic(TInt aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}
