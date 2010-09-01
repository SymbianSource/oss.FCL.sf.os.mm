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

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <icl/imagetransform.hrh>
#include "TestTransformer.h"
#include "TestTransformer.hrh"


// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for Test transformer
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidTransformImplementationJpegTestPlugin, CTestTransformer::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KUidTransformImplementationPngTestPlugin, CTestTransformer::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

