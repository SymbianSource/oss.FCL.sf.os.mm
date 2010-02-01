// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <icl/imageconstruct.h>
#include "fwexttestcodec.h"
#include "fwexttestconvert.h"
#include "TestUids.hrh"

// Decoder construct declaration
class CFwExtTestDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CFwExtTestDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CFwExtTestDecodeConstruct* CFwExtTestDecodeConstruct::NewL()
	{
	CFwExtTestDecodeConstruct* self = new (ELeave) CFwExtTestDecodeConstruct();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CFwExtTestDecodeConstruct::NewPluginL() const
	{
	return CFwExtTestDecoderPlugin::NewL();
	}

// Encoder construct declaration
class CFwExtTestEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CFwExtTestEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	};
	
CFwExtTestEncodeConstruct* CFwExtTestEncodeConstruct::NewL()
	{
	CFwExtTestEncodeConstruct* self = new (ELeave) CFwExtTestEncodeConstruct();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CFwExtTestEncodeConstruct::NewPluginL() const
	{
	return CFwExtTestEncoderPlugin::NewL();
	}
	
// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for fwtest codec
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KTestFwExtDecoderImplementationUidValue, CFwExtTestDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestFwExtEncoderImplementationUidValue, CFwExtTestEncodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
	
