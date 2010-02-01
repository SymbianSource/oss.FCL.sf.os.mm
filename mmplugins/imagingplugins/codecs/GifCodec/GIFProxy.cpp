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
// Licensed under US Patent No 4,558,302 and foreign counterparts
// 
//


#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <icl/icl_uids.hrh>
#include "GIFConvert.h"

#include <icl/imageconstruct.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

class CGifDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CGifDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CGifDecodeConstruct* CGifDecodeConstruct::NewL()
	{
	CGifDecodeConstruct* self = new (ELeave) CGifDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CGifDecodeConstruct::NewPluginL() const
	{
	return CGifDecoder::NewL();
	}

class CGifEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CGifEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	};

CGifEncodeConstruct* CGifEncodeConstruct::NewL()
	{
	CGifEncodeConstruct* self = new (ELeave) CGifEncodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CGifEncodeConstruct::NewPluginL() const
	{
	return CGifEncoder::NewL();
	}


// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for GIF decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KGIFDecoderImplementationUidValue, CGifDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KGIFEncoderImplementationUidValue, CGifEncodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
