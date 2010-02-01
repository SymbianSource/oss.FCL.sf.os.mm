// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __EXTENDED_PLUGIN_H__
#define __EXTENDED_PLUGIN_H__

#include <imageconversion.h>
#include <icl/imageplugin.h>

class CExtendedDecoder : public CImageDecoder
	{
public:
	IMPORT_C static CExtendedDecoder* NewL();

	IMPORT_C TInt SyncResult() const;
	IMPORT_C TInt AsyncResult() const;
	IMPORT_C TBool DoCustomAsync() const;

	IMPORT_C void CustomSyncL(TInt aParam);
	IMPORT_C void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);
	IMPORT_C TBool UsingThread() const;
protected:
	CExtendedDecoder();
	};

class CExtendedDecoderPlugin : public CImageDecoderPlugin
	{
public:
	IMPORT_C TBool UsingThread() const;

protected:
	//Frome CImageDecoderPlugin
	IMPORT_C virtual void DoConvert();
	IMPORT_C virtual void HandleCustomSyncL(TInt aParam);
	IMPORT_C virtual void InitCustomAsyncL(TInt aParam);
	IMPORT_C virtual void NotifyComplete();

public:
	TInt iSyncResult;
	TInt iAsyncResult;
	TBool iDoCustomAsync;
	};

class CExtendedEncoder : public CImageEncoder
	{
public:
	IMPORT_C static CExtendedEncoder* NewL();

	IMPORT_C TInt SyncResult() const;
	IMPORT_C TInt AsyncResult() const;
	IMPORT_C TBool DoCustomAsync() const;
	IMPORT_C TBool UsingThread() const;

	IMPORT_C void CustomSyncL(TInt aParam);
	IMPORT_C void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);
protected:
	CExtendedEncoder();
	};

class CExtendedEncoderPlugin : public CImageEncoderPlugin
	{
public:
	IMPORT_C TBool UsingThread() const;

protected:
	//Frome CImageEncoderPlugin
	IMPORT_C virtual void DoConvert();
	IMPORT_C virtual void HandleCustomSyncL(TInt aParam);
	IMPORT_C virtual void InitCustomAsyncL(TInt aParam);
	IMPORT_C virtual void NotifyComplete();

public:
	TInt iSyncResult;
	TInt iAsyncResult;
	TBool iDoCustomAsync;
	};

#endif // __EXTENDED_PLUGIN_H__

