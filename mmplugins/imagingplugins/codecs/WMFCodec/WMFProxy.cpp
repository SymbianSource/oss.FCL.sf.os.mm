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
#include "WMFConvert.h"

#include <icl/imageconstruct.h>

class CWmfDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CWmfDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CWmfDecodeConstruct* CWmfDecodeConstruct::NewL()
	{
	CWmfDecodeConstruct* self = new (ELeave) CWmfDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CWmfDecodeConstruct::NewPluginL() const
	{
	return CWmfDecoder::NewL();
	}

class CWmfStdDecodeConstruct : public CWmfDecodeConstruct
	{
public:
	static CWmfStdDecodeConstruct* NewL();

	// from CWmfDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CWmfStdDecodeConstruct* CWmfStdDecodeConstruct::NewL()
	{
	CWmfStdDecodeConstruct* self = new (ELeave) CWmfStdDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CWmfStdDecodeConstruct::NewPluginL() const
	{
	return CWmfDecoder::NewStdL();
	}

class CWmfApmDecodeConstruct : public CWmfDecodeConstruct
	{
public:
	static CWmfApmDecodeConstruct* NewL();

	// from CWmfDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CWmfApmDecodeConstruct* CWmfApmDecodeConstruct::NewL()
	{
	CWmfApmDecodeConstruct* self = new (ELeave) CWmfApmDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CWmfApmDecodeConstruct::NewPluginL() const
	{
	return CWmfDecoder::NewApmL();
	}

class CWmfClpDecodeConstruct : public CWmfDecodeConstruct
	{
public:
	static CWmfClpDecodeConstruct* NewL();

	// from CWmfDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	};

CWmfClpDecodeConstruct* CWmfClpDecodeConstruct::NewL()
	{
	CWmfClpDecodeConstruct* self = new (ELeave) CWmfClpDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CWmfClpDecodeConstruct::NewPluginL() const
	{
	return CWmfDecoder::NewClpL();
	}

// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for WMF decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KWMFDecoderImplementationUidValue, CWmfDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KWMFDecoderStdImplementationUidValue, CWmfStdDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KWMFDecoderApmImplementationUidValue, CWmfApmDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KWMFDecoderClpImplementationUidValue, CWmfClpDecodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
