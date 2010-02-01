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

#include "ExtendedTestCodec.h"


// CExtendedTestReadCodec
CExtendedTestReadCodec::CExtendedTestReadCodec(CExtendedTestDecoder& aDecoder):
	iDecoder(&aDecoder)
	{}

CExtendedTestReadCodec::~CExtendedTestReadCodec()
	{}

TFrameState CExtendedTestReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	return EFrameComplete;
	}

void CExtendedTestReadCodec::Complete()
	{}

void CExtendedTestReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool /*aDisableErrorDiffusion*/, CFbsBitmap& /*aDestination*/, CFbsBitmap* /*aDestinationMask*/)
	{}

CExtendedTestWriteCodec::CExtendedTestWriteCodec(CExtendedTestEncoder &aEncoder):
	iEncoder(&aEncoder)
	{}

CExtendedTestWriteCodec::~CExtendedTestWriteCodec()
	{}

TFrameState CExtendedTestWriteCodec::ProcessFrameL(TBufPtr8& /*aDst*/)
	{
	return EFrameComplete;
	}
