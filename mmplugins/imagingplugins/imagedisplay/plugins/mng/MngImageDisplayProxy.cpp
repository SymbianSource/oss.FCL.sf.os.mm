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

/** 	@file
	@internalTechnology */
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <icl/imagedisplay.hrh>
#include <icl/mngimagedisplay.hrh>
#include "MngDecodeConstruct.h"

#include "MngImageDisplay.h"


// Exported proxy for instantiation method resolution

// Define the Implementation UIDs 
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidImageDisplayMngPluginImplUid, 	CMngImageDisplayPlugin::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KUidIclMngPluginImplUid,				CMngDecodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

