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

#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <imageconversion.h>
#include "BadVerTestCodec.h"
#include "TestPanic.h"
#include "TestDecs.h"
#include "TestUids.hrh"

//_LIT(KBadVerTestPanicCategory, "BadVerTestConvertPlugin");	// EABI warning removal


// Global panic function
GLREF_C void Panic(TInt aError);

// decoder.
CBadVerTestDecoder* CBadVerTestDecoder::NewL()
	{
	Panic(EIllegalPlugin); // we should never reach this plugin
	return NULL;
	}

CBadVerTestDecoder::CBadVerTestDecoder()
	{
	}

CBadVerTestDecoder::~CBadVerTestDecoder()
	{
	}

void CBadVerTestDecoder::ImageType(TInt /*aFrameNumber*/, TUid& /*aImageType*/, TUid& /*aImageSubType*/) const
	{
	Panic(EIllegalPlugin); // we should never reach this plugin
	}

void CBadVerTestDecoder::ScanDataL()
	{
	}

CFrameInfoStrings* CBadVerTestDecoder::FrameInfoStringsL(RFs& /*aFs*/, TInt /*aFrameNumber*/)
	{
	Panic(EIllegalPlugin); // we should never reach this plugin
	return NULL;
	}

// encoder.
CBadVerTestEncoder* CBadVerTestEncoder::NewL()
	{
	Panic(EIllegalPlugin); // we should never reach this plugin
	return NULL;
	}

CBadVerTestEncoder::CBadVerTestEncoder()
	{
	Panic(EIllegalPlugin); // we should never reach this plugin
	}

CBadVerTestEncoder::~CBadVerTestEncoder()
	{
	Panic(EIllegalPlugin); // we should never reach this plugin
	}

void CBadVerTestEncoder::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	}

void CBadVerTestEncoder::UpdateHeaderL()
	{
	}

