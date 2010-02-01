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

#include "ExtendedPlugin.h"

//CExtendedDecoder
EXPORT_C CExtendedDecoder* CExtendedDecoder::NewL()
	{
	CExtendedDecoder* self;
	self = new (ELeave) CExtendedDecoder;
	return self;
	}

CExtendedDecoder::CExtendedDecoder()
	{
	}

EXPORT_C TInt CExtendedDecoder::SyncResult() const
	{
	CExtendedDecoderPlugin* plugin;
	plugin = static_cast<CExtendedDecoderPlugin*>(Plugin());
	return plugin->iSyncResult;
	}

EXPORT_C TInt CExtendedDecoder::AsyncResult() const
	{
	CExtendedDecoderPlugin* plugin;
	plugin = static_cast<CExtendedDecoderPlugin*>(Plugin());
	return plugin->iAsyncResult;
	}

EXPORT_C TBool CExtendedDecoder::UsingThread() const
	{
	CExtendedDecoderPlugin* plugin;
	plugin = static_cast<CExtendedDecoderPlugin*>(Plugin());
	return plugin->UsingThread();
	}

EXPORT_C TInt CExtendedDecoder::DoCustomAsync() const
	{
	CExtendedDecoderPlugin* plugin;
	plugin = static_cast<CExtendedDecoderPlugin*>(Plugin());
	return plugin->iDoCustomAsync;
	}

EXPORT_C void CExtendedDecoder::CustomSyncL(TInt aParam)
	{
	CImageDecoder::CustomSyncL(aParam);
	}

EXPORT_C void CExtendedDecoder::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	CImageDecoder::CustomAsync(aRequestStatus, aParam);
	}
	
//CExtendedDecoderPlugin
EXPORT_C void CExtendedDecoderPlugin::HandleCustomSyncL(TInt aParam)
	{
	if(aParam == 0)
		iSyncResult = -2;
	else
		User::Leave(KErrNotSupported);
	}

EXPORT_C void CExtendedDecoderPlugin::InitCustomAsyncL(TInt aParam)
	{
	iDoCustomAsync = ETrue;
	if(aParam == 0)
		{
		iAsyncResult = -1;
		}
	else
		{
		iDoCustomAsync = EFalse;
		User::Leave(KErrNotSupported);
		}
	}

EXPORT_C void CExtendedDecoderPlugin::DoConvert()
	{
	if(iDoCustomAsync)
		{
		iAsyncResult = -2;
		RequestComplete(KErrNone);
		}
	else
		CImageDecoderPlugin::DoConvert();
	}

EXPORT_C void CExtendedDecoderPlugin::NotifyComplete()
	{
	iDoCustomAsync = EFalse;
	}

EXPORT_C TBool CExtendedDecoderPlugin::UsingThread() const
	{
	return AmInThread();
	}

//CExtendedEncoder
EXPORT_C CExtendedEncoder* CExtendedEncoder::NewL()
	{
	CExtendedEncoder* self;
	self = new (ELeave) CExtendedEncoder;
	return self;
	}

CExtendedEncoder::CExtendedEncoder()
	{
	}

EXPORT_C TInt CExtendedEncoder::SyncResult() const
	{
	CExtendedEncoderPlugin* plugin;
	plugin = static_cast<CExtendedEncoderPlugin*>(Plugin());
	return plugin->iSyncResult;
	}

EXPORT_C TInt CExtendedEncoder::AsyncResult() const
	{
	CExtendedEncoderPlugin* plugin;
	plugin = static_cast<CExtendedEncoderPlugin*>(Plugin());
	return plugin->iAsyncResult;
	}

EXPORT_C TBool CExtendedEncoder::UsingThread() const
	{
	CExtendedEncoderPlugin* plugin;
	plugin = static_cast<CExtendedEncoderPlugin*>(Plugin());
	return plugin->UsingThread();
	}

EXPORT_C TInt CExtendedEncoder::DoCustomAsync() const
	{
	CExtendedEncoderPlugin* plugin;
	plugin = static_cast<CExtendedEncoderPlugin*>(Plugin());
	return plugin->iDoCustomAsync;
	}

EXPORT_C void CExtendedEncoder::CustomSyncL(TInt aParam)
	{
	CImageEncoder::CustomSyncL(aParam);
	}

EXPORT_C void CExtendedEncoder::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	CImageEncoder::CustomAsync(aRequestStatus, aParam);
	}
	
//CExtendedEncoderPlugin
EXPORT_C void CExtendedEncoderPlugin::HandleCustomSyncL(TInt aParam)
	{
	if(aParam == 0)
		iSyncResult = -2;
	else
		User::Leave(KErrNotSupported);
	}

EXPORT_C void CExtendedEncoderPlugin::InitCustomAsyncL(TInt aParam)
	{
	iDoCustomAsync = ETrue;
	if(aParam == 0)
		{
		iAsyncResult = -1;
		}
	else
		{
		iDoCustomAsync = EFalse;		
		User::Leave(KErrNotSupported);
		}
	}

EXPORT_C void CExtendedEncoderPlugin::DoConvert()
	{
	if(iDoCustomAsync)
		{
		iAsyncResult = -2;
		RequestComplete(KErrNone);
		}
	else
		CImageEncoderPlugin::DoConvert();
	}

EXPORT_C void CExtendedEncoderPlugin::NotifyComplete()
	{
	iDoCustomAsync = EFalse;
	}

EXPORT_C TBool CExtendedEncoderPlugin::UsingThread() const
	{
	return AmInThread();
	}


