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
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "PNGCodec.h"
#include "StreamDecoderIFace.hrh"
#include "PngDecoderFactory.h"

#include <icl/imageconstruct.h>

class CPngDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CPngDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CPngDecodeConstruct* CPngDecodeConstruct::NewL()
	{
	CPngDecodeConstruct* self = new (ELeave) CPngDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CPngDecodeConstruct::NewPluginL() const
	{
	return CPngDecoder::NewL();
	}

class CPngEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CPngEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	};

CPngEncodeConstruct* CPngEncodeConstruct::NewL()
	{
	CPngEncodeConstruct* self = new (ELeave) CPngEncodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CPngEncodeConstruct::NewPluginL() const
	{
	return CPngEncoder::NewL();
	}

// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for TIFF decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KPNGDecoderImplementationUidValue, CPngDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KPNGEncoderImplementationUidValue, CPngEncodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KPngStreamDecoderFactoryImplUidValue, CPngImageStreamDecoderFactory::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
