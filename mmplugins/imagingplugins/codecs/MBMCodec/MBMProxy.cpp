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
#include "MBMCodec.h"

#include <icl/imageconstruct.h>

class CMbmDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CMbmDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CMbmDecodeConstruct* CMbmDecodeConstruct::NewL()
	{
	CMbmDecodeConstruct* self = new (ELeave) CMbmDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CMbmDecodeConstruct::NewPluginL() const
	{
	return CMbmDecoder::NewL();
	}

class CMbmEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CMbmEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	};

CMbmEncodeConstruct* CMbmEncodeConstruct::NewL()
	{
	CMbmEncodeConstruct* self = new (ELeave) CMbmEncodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CMbmEncodeConstruct::NewPluginL() const
	{
	return CMbmEncoder::NewL();
	}


// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for TIFF decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KMBMDecoderImplementationUidValue, CMbmDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KMBMEncoderImplementationUidValue, CMbmEncodeConstruct::NewL),
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
