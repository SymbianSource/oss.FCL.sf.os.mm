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
#include "TestCodec.h"
#include "BadVerTestCodec.h"
#include "TestUids.hrh"
#include "TestAsyncCodec.h"
#include "TestExifDerived.h"

#include <icl/imageconstruct.h>

//
//
//
class CTestDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CTestDecodeConstruct* NewL();
	static CTestDecodeConstruct* FailNewL();
	virtual CImageDecoderPlugin* NewPluginL() const;	// from CImageDecodeConstruct
	virtual TBool RequestThread() const;

private:
	CTestDecodeConstruct(TBool aLeaveCorrupt = EFalse);

private:
	TBool iLeaveCorrupt;
	};


CTestDecodeConstruct* CTestDecodeConstruct::NewL()
	{
	return new (ELeave) CTestDecodeConstruct();
	}

CTestDecodeConstruct* CTestDecodeConstruct::FailNewL()
	{
	return new (ELeave) CTestDecodeConstruct(ETrue);
	}

CTestDecodeConstruct::CTestDecodeConstruct(TBool aLeaveCorrupt)
 :	iLeaveCorrupt(aLeaveCorrupt)
	{
	}

CImageDecoderPlugin* CTestDecodeConstruct::NewPluginL() const
	{
	return CTestDecoder::NewL(iLeaveCorrupt);
	}

TBool CTestDecodeConstruct::RequestThread() const
	{
	return ETrue;
	}


//
//
//
class CTestEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CTestEncodeConstruct* NewL();
	virtual CImageEncoderPlugin* NewPluginL() const;	// from CImageEncodeConstruct
	virtual TBool RequestThread() const;
	};

CTestEncodeConstruct* CTestEncodeConstruct::NewL()
	{
	return new (ELeave) CTestEncodeConstruct();
	}

CImageEncoderPlugin* CTestEncodeConstruct::NewPluginL() const
	{
	return CTestEncoder::NewL();
	}

TBool CTestEncodeConstruct::RequestThread() const
	{
	return ETrue;
	}


class CTestExtendedDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CTestExtendedDecodeConstruct* NewL();
	virtual CImageDecoderPlugin* NewPluginL() const;	// from CImageDecodeConstruct

private:
	CTestExtendedDecodeConstruct();
	};

CTestExtendedDecodeConstruct* CTestExtendedDecodeConstruct::NewL()
	{
	return new (ELeave) CTestExtendedDecodeConstruct();
	}

CImageDecoderPlugin* CTestExtendedDecodeConstruct::NewPluginL() const
	{
	return CTestExtendedDecoder::NewL();
	}

CTestExtendedDecodeConstruct::CTestExtendedDecodeConstruct()
	{
	}


class CTestExtendedEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CTestExtendedEncodeConstruct * NewL();
	virtual CImageEncoderPlugin* NewPluginL() const;	// from CImageEncodeConstruct

private:
	CTestExtendedEncodeConstruct();
	};

CTestExtendedEncodeConstruct * CTestExtendedEncodeConstruct::NewL()
	{
	return new (ELeave) CTestExtendedEncodeConstruct();
	}

CImageEncoderPlugin* CTestExtendedEncodeConstruct::NewPluginL() const
	{
	return CTestExtendedEncoder::NewL();
	}

CTestExtendedEncodeConstruct::CTestExtendedEncodeConstruct()
	{
	}
	
//
//
//
class CBadVerTestDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CBadVerTestDecodeConstruct* NewL();
	virtual CImageDecoderPlugin* NewPluginL() const;	// from CImageDecodeConstruct
	};

CBadVerTestDecodeConstruct* CBadVerTestDecodeConstruct::NewL()
	{
	return new (ELeave) CBadVerTestDecodeConstruct();
	}

CImageDecoderPlugin* CBadVerTestDecodeConstruct::NewPluginL() const
	{
	return CBadVerTestDecoder::NewL();
	}


//
//
//
class CBadVerTestEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CBadVerTestEncodeConstruct* NewL();
	virtual CImageEncoderPlugin* NewPluginL() const;	// from CImageEncodeConstruct
	};

CBadVerTestEncodeConstruct* CBadVerTestEncodeConstruct::NewL()
	{
	return new (ELeave) CBadVerTestEncodeConstruct;
	}

CImageEncoderPlugin* CBadVerTestEncodeConstruct::NewPluginL() const
	{
	return CBadVerTestEncoder::NewL();
	}


//
//
//
class CTestAsyncDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CTestAsyncDecodeConstruct* NewL();
	virtual CImageDecoderPlugin* NewPluginL() const; // from CImageDecodeConstruct
	};


CTestAsyncDecodeConstruct* CTestAsyncDecodeConstruct::NewL()
	{
	return new (ELeave) CTestAsyncDecodeConstruct();
	}

CImageDecoderPlugin* CTestAsyncDecodeConstruct::NewPluginL() const
	{
	return CTestAsyncDecoder::NewL();
	}


//
//
//
class CTestAsyncEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CTestAsyncEncodeConstruct* NewL();
	virtual CImageEncoderPlugin* NewPluginL() const; // from CImageEncodeConstruct
	};


CTestAsyncEncodeConstruct* CTestAsyncEncodeConstruct::NewL()
	{
	return new (ELeave) CTestAsyncEncodeConstruct();
	}

CImageEncoderPlugin* CTestAsyncEncodeConstruct::NewPluginL() const
	{
	return CTestAsyncEncoder::NewL();
	}


//
// for testing the INC059847
//
class CTestExifDerivedEncodeConstruct : public CImageEncodeConstruct
	{
public:
	static CTestExifDerivedEncodeConstruct* NewL();
	virtual CImageEncoder* NewEncoderL() const;		// from CImageEncodeConstruct
	virtual CImageEncoderPlugin* NewPluginL() const;
	};

CTestExifDerivedEncodeConstruct* CTestExifDerivedEncodeConstruct::NewL()
	{
	return new (ELeave) CTestExifDerivedEncodeConstruct();
	}

CImageEncoderPlugin* CTestExifDerivedEncodeConstruct::NewPluginL() const
	{
	return CDummyExifEncoder::NewL();
	}
	
CImageEncoder* CTestExifDerivedEncodeConstruct::NewEncoderL() const
	{
	return CTestExifDerivedEncoder::NewL();
	}	


//
//
//	
class CTestExifDerivedDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CTestExifDerivedDecodeConstruct* NewL();
	virtual CImageDecoderPlugin* NewPluginL() const;	// from CImageDecodeConstruct
	virtual CImageDecoder* NewDecoderL() const;
	};

CTestExifDerivedDecodeConstruct* CTestExifDerivedDecodeConstruct::NewL()
	{
	return new (ELeave) CTestExifDerivedDecodeConstruct();
	}

CImageDecoderPlugin* CTestExifDerivedDecodeConstruct::NewPluginL() const
	{
	return CDummyExifDecoder::NewL();
	}	
	
CImageDecoder* CTestExifDerivedDecodeConstruct::NewDecoderL() const
	{
	return CTestExifDerivedDecoder::NewL();
	}


// Exported proxy for instantiation method resolution

// Define the Implementation UIDs for TST decoder
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KTestDecoderImplementationUidValue, CTestDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KFailTestDecoderImplementationUidValue, CTestDecodeConstruct::FailNewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestEncoderImplementationUidValue, CTestEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KBadVerTestDecoderImplementationUidValue, CBadVerTestDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KBadVerTestEncoderImplementationUidValue, CBadVerTestEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestAsyncDecoderImplementationUidValue, CTestAsyncDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestAsyncEncoderImplementationUidValue, CTestAsyncEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestExifDummyEncoderImplementationUidValue, CTestExifDerivedEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestExifDummyDecoderImplementationUidValue, CTestExifDerivedDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyDecoderExtCropUidValue, CTestExtendedDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyDecoderExtStreamUidValue, CTestExtendedDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyDecoderExtRotationUidValue, CTestExtendedDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyDecoderExtScalingUidValue, CTestExtendedDecodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyEncoderExtStreamUidValue, CTestExtendedEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyEncoderExtRotationUidValue, CTestExtendedEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyEncoderExtUseWithSetThumbnailUidValue, CTestExtendedEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyEncoderExtMirrorHorizontalVerticalAxisUidValue, CTestExtendedEncodeConstruct::NewL),
	IMPLEMENTATION_PROXY_ENTRY(KTestDummyDecoderExtMirrorHorizontalVerticalAxisUidValue, CTestExtendedDecodeConstruct::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
