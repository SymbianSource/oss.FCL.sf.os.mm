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
#include <icl/icl_uids.hrh>
#include "BMPConvert.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

#include <icl/imageconstruct.h>

class CBmpDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CBmpDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CBmpDecodeConstruct* CBmpDecodeConstruct::NewL()
	{
	CBmpDecodeConstruct* self = new (ELeave) CBmpDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CBmpDecodeConstruct::NewPluginL() const
	{
	return CBmpDecoder::NewL();
	}

class CBmpEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CBmpEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	};

CBmpEncodeConstruct* CBmpEncodeConstruct::NewL()
	{
	CBmpEncodeConstruct* self = new (ELeave) CBmpEncodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CBmpEncodeConstruct::NewPluginL() const
	{
	return CBmpEncoder::NewL();
	}



// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for BMP decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KBMPDecoderImplementationUidValue, CBmpDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KBMPEncoderImplementationUidValue, CBmpEncodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
