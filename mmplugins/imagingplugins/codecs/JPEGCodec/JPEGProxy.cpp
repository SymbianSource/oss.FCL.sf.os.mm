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
#include "JPEGConvert.h"
#include "ExifJpegTransform.h"
#include <iclexifimageframe.h>

#include <icl/imageconstruct.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

//
// this file doesn't contain any performance-critical code so use thumb
// instruction set to save on some ROM footprint
#if defined(__ARMCC__)
#pragma thumb
#endif

class CJpegDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CJpegDecodeConstruct* NewL();

	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;
	CImageDecoder* NewDecoderL() const;
	};

CJpegDecodeConstruct* CJpegDecodeConstruct::NewL()
	{
	CJpegDecodeConstruct* self = new (ELeave) CJpegDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderPlugin* CJpegDecodeConstruct::NewPluginL() const
	{
	return CJpegDecoder::NewL();
	}

CImageDecoder* CJpegDecodeConstruct::NewDecoderL() const
	{
	return CJPEGImageFrameDecoder::NewL();
	}


class CJpegEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CJpegEncodeConstruct* NewL();

	// from CImageEncodeConstruct
	CImageEncoderPlugin* NewPluginL() const;
	CImageEncoder* NewEncoderL() const;
	};

CJpegEncodeConstruct* CJpegEncodeConstruct::NewL()
	{
	CJpegEncodeConstruct* self = new (ELeave) CJpegEncodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderPlugin* CJpegEncodeConstruct::NewPluginL() const
	{
	return CJpegEncoder::NewL();
	}

CImageEncoder* CJpegEncodeConstruct::NewEncoderL() const
	{
	return CJPEGImageFrameEncoder::NewL();
	}



// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for JPEG decoder
const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KJPGDecoderImplementationUidValue, CJpegDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KJPGEncoderImplementationUidValue, CJpegEncodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KUidTransformJpegPluginUidValue, CExifJpegTransform::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KJPGDecoderImplementation2UidValue, CJpegDecodeConstruct::NewL),
		IMPLEMENTATION_PROXY_ENTRY(KJPGEncoderImplementation2UidValue, CJpegEncodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
