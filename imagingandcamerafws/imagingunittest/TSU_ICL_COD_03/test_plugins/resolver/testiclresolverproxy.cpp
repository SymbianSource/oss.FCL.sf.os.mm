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

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

#include "testiclresolver.h"
#include "testiclresolver.hrh"

// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for test resolvers.
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KUidTestIclResolverPlugin1, CTestIclResolver::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidTestIclResolverPlugin2, CTestIclResolver::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidTestIclResolverPlugin3, CTestIclResolver::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidTestIclResolverPlugin4, CTestIclResolver::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
