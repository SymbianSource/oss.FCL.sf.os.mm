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
#include "ExtendedPlugin.h"
#include "ExtendedTestCodec.h"
#include "ExtendedTestConvert.h"
#include "TestUids.hrh"

#include <icl/imageconstruct.h>

//CExtendedTestDecodeConstruct
class CExtendedTestDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CExtendedTestDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	virtual CImageDecoder* NewDecoderL() const;
	virtual CImageDecoderPlugin* NewPluginL() const;
	};

CExtendedTestDecodeConstruct* CExtendedTestDecodeConstruct::NewL()
	{
	CExtendedTestDecodeConstruct* self = new (ELeave) CExtendedTestDecodeConstruct;
	return self;
	}

CImageDecoder* CExtendedTestDecodeConstruct::NewDecoderL() const
	{
	return CExtendedDecoder::NewL();
	}

CImageDecoderPlugin* CExtendedTestDecodeConstruct::NewPluginL() const
	{
	return CExtendedTestDecoderPlugin::NewL();
	}

//CExtendedTestEncodeConstruct
class CExtendedTestEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CExtendedTestEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	virtual CImageEncoder* NewEncoderL() const;
	virtual CImageEncoderPlugin* NewPluginL() const;
	};

CExtendedTestEncodeConstruct* CExtendedTestEncodeConstruct::NewL()
	{
	CExtendedTestEncodeConstruct* self = new (ELeave) CExtendedTestEncodeConstruct;
	return self;
	}

CImageEncoder* CExtendedTestEncodeConstruct::NewEncoderL() const
	{
	return CExtendedEncoder::NewL();
	}

CImageEncoderPlugin* CExtendedTestEncodeConstruct::NewPluginL() const
	{
	return CExtendedTestEncoderPlugin::NewL();
	}

// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for Extended Test decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KExtendedTestDecoderImplementationUidValue, CExtendedTestDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KExtendedTestEncoderImplementationUidValue, CExtendedTestEncodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
