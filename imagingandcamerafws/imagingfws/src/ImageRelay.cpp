// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ImageRelay.h"
#include "icl/ImageConstruct.h"
#include "ImageClientMain.h"
#include "ImageStraightRelay.h"
#include "ThreadRelay.h"
#include "ImageClientMain.h"
#include "fwextconstants.h"
//
// MImageDecoderRelay
//
MImageDecoderRelay* MImageDecoderRelay::NewL(CImageDecodeConstruct* aConstruct, TBool aAlwaysThread)
	{
	TBool wantThread = aAlwaysThread || aConstruct->RequestThread();

	if (wantThread)
		return CThreadDecoderRelay::NewL(aConstruct);
	else
		return CImageDecoderStraightRelay::NewL(aConstruct);
	}

MImageDecoderRelay::~MImageDecoderRelay()
	{
	}

//
// CImageConvExtensionCache
//
CImageConvExtensionCache* CImageConvExtensionCache::NewL()
	{
	return new (ELeave) CImageConvExtensionCache();
	}

CImageConvExtensionCache::~CImageConvExtensionCache()
	{
	iProxyList.ResetAndDestroy();
	
	delete iOperationExtension;
	iOperationExtension = NULL;
	delete iScalerExtension;
	iScalerExtension = NULL;
	delete iStreamedDecodeExtension;
	iStreamedDecodeExtension = NULL;
	delete iStreamedEncodeExtension;
	iStreamedEncodeExtension = NULL;
	}
	
TImageConvOperation* CImageConvExtensionCache::Operation()
	{
	return iOperationExtension;
	}
	
TImageConvScaler* CImageConvExtensionCache::Scaler()
	{
	return iScalerExtension;
	}
	
TImageConvStreamedDecode* CImageConvExtensionCache::DecodeBlockStreamer()
	{
	return iStreamedDecodeExtension;
	}
	
TImageConvStreamedEncode* CImageConvExtensionCache::EncodeBlockStreamer()
	{
	return iStreamedEncodeExtension;
	}

TImageConvPrepare* CImageConvExtensionCache::Prepare()
	{
	return iPrepareExtension;
	}
	
void CImageConvExtensionCache::SetOperationExtension(TImageConvOperation* aOperationExtension)
	{
	__ASSERT_ALWAYS(!iOperationExtension, Panic(EExtensionAlreadySet));
	iOperationExtension = aOperationExtension;
	}

void CImageConvExtensionCache::SetScalerExtension(TImageConvScaler* aScalerExtension)
	{
	__ASSERT_ALWAYS(!iScalerExtension, Panic(EExtensionAlreadySet));
	iScalerExtension = aScalerExtension;
	}

void CImageConvExtensionCache::SetBlockStreamerExtension(TImageConvStreamedDecode* aStreamedDecodeExtension)
	{
	__ASSERT_ALWAYS(!iStreamedDecodeExtension, Panic(EExtensionAlreadySet));
	iStreamedDecodeExtension = aStreamedDecodeExtension;
	}
	
void CImageConvExtensionCache::SetBlockStreamerExtension(TImageConvStreamedEncode* aStreamedEncodeExtension)
	{
	__ASSERT_ALWAYS(!iStreamedEncodeExtension, Panic(EExtensionAlreadySet));
	iStreamedEncodeExtension = aStreamedEncodeExtension;
	}

void CImageConvExtensionCache::SetPrepareExtension(TImageConvPrepare* aPrepareExtension)
	{
	__ASSERT_ALWAYS(!iPrepareExtension, Panic(EExtensionAlreadySet));
	iPrepareExtension = aPrepareExtension;
	}

void CImageConvExtensionCache::SetProxyL(CImageConvProxyBase* aProxy)
	{
	iProxyList.AppendL(aProxy);
	}

//
// MImageEncoderRelay
//

MImageEncoderRelay* MImageEncoderRelay::NewL(CImageEncodeConstruct* aConstruct, TBool aAlwaysThread)
	{
	TBool wantThread = aAlwaysThread || aConstruct->RequestThread();

	if (wantThread)
		return CThreadEncoderRelay::NewL(aConstruct);
	else
		return CImageEncoderStraightRelay::NewL(aConstruct);
	}

MImageEncoderRelay::~MImageEncoderRelay()
	{
	}

TImageConvPrepare::TImageConvPrepare()
: iExtension(NULL)
	{
	}
	
void TImageConvPrepare::Prepare(TRequestStatus* aStatus)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	return iExtension->Prepare(aStatus);
	}
	
void TImageConvPrepare::SetExtension(MImageConvExtension* aExtension)
	{
	__ASSERT_ALWAYS(aExtension->Uid() == KICLPrepareUid, Panic(EInvalidFwExtensionUid));
	iExtension = static_cast<MImageConvPrepare*>(aExtension);
	}

