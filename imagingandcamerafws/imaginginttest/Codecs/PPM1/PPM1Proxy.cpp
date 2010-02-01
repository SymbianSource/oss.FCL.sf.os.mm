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

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include "PPM1Codec.h"
#include "PPM1Uids.hrh"

#include <icl/imageconstruct.h>

class CPpmDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CPpmDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CPpmDecodeConstruct* CPpmDecodeConstruct::NewL()
	{
	CPpmDecodeConstruct* self = new (ELeave) CPpmDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CPpmDecodeConstruct::NewPluginL() const
	{
	return CPpmDecoder::NewL();
	}

class CPpmEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CPpmEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	};

CPpmEncodeConstruct* CPpmEncodeConstruct::NewL()
	{
	CPpmEncodeConstruct* self = new (ELeave) CPpmEncodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CPpmEncodeConstruct::NewPluginL() const
	{
	return CPpmEncoder::NewL();
	}

// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for PPM decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KPpm1DecoderImplementationUidValue, CPpmDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KPpm1DecoderRAWImplementationUidValue, CPpmDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KPpm1DecoderASCIIImplementationUidValue, CPpmDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KPpm1EncoderImplementationUidValud, CPpmEncodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
