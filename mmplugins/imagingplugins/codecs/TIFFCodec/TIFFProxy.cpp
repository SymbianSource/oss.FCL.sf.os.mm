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
#include "TIFFCodec.h"

#include <icl/imageconstruct.h>

class CTiffDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CTiffDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CTiffDecodeConstruct* CTiffDecodeConstruct::NewL()
	{
	CTiffDecodeConstruct* self = new (ELeave) CTiffDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CTiffDecodeConstruct::NewPluginL() const
	{
	return CTiffDecoder::NewL();
	}

class CTiffLittleEndianDecodeConstruct : public CTiffDecodeConstruct
	{
public:
	static CTiffLittleEndianDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CTiffLittleEndianDecodeConstruct* CTiffLittleEndianDecodeConstruct::NewL()
	{
	CTiffLittleEndianDecodeConstruct* self = new (ELeave) CTiffLittleEndianDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CTiffLittleEndianDecodeConstruct::NewPluginL() const
	{
	return CTiffDecoder::NewLittleEndianL();
	}

class CTiffBigEndianDecodeConstruct : public CTiffDecodeConstruct
	{
public:
	static CTiffBigEndianDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CTiffBigEndianDecodeConstruct* CTiffBigEndianDecodeConstruct::NewL()
	{
	CTiffBigEndianDecodeConstruct* self = new (ELeave) CTiffBigEndianDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CTiffBigEndianDecodeConstruct::NewPluginL() const
	{
	return CTiffDecoder::NewBigEndianL();
	}

// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for TIFF decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KTIFFDecoderImplementationUidValue, CTiffDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KTIFFDecoderLittleEndianImplementationUidValue, CTiffLittleEndianDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KTIFFDecoderBigEndianImplementationUidValue, CTiffBigEndianDecodeConstruct::NewL),
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
