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

#include "ExtendedTestConvert.h"

//CExtendedTestDecoderPlugin
CExtendedTestDecoderPlugin* CExtendedTestDecoderPlugin::NewL()
	{
	return new(ELeave) CExtendedTestDecoderPlugin;
	}

CExtendedTestDecoderPlugin::CExtendedTestDecoderPlugin()
	{}

void CExtendedTestDecoderPlugin::ScanDataL()
	{}

CFrameInfoStrings* CExtendedTestDecoderPlugin::FrameInfoStringsL(RFs& /*aFs*/, TInt /*aFrameNumber*/)
	{
	return NULL;
	}

void CExtendedTestDecoderPlugin::ImageType(TInt /*aFrameNumber*/, TUid& /*aImageType*/, TUid& /*aImageSubType*/) const
	{}

CExtendedTestDecoderPlugin::~CExtendedTestDecoderPlugin()
	{
	CExtendedDecoderPlugin::Cleanup();
	}


//CExtendedTestEncoderPlugin
CExtendedTestEncoderPlugin* CExtendedTestEncoderPlugin::NewL()
	{
	CExtendedTestEncoderPlugin* self;
	self = new (ELeave) CExtendedTestEncoderPlugin;
	return self;
	}

CExtendedTestEncoderPlugin::CExtendedTestEncoderPlugin()
	{}

void CExtendedTestEncoderPlugin::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	}

void CExtendedTestEncoderPlugin::UpdateHeaderL()
	{}

CExtendedTestEncoderPlugin::~CExtendedTestEncoderPlugin()
	{
	CExtendedEncoderPlugin::Cleanup();
	}


