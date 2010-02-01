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

#include <fbs.h>
#include "BadVerTestCodec.h"


// CBadVerTestReadCodec
CBadVerTestReadCodec::CBadVerTestReadCodec(CBadVerTestDecoder& aDecoder):
	iDecoder(&aDecoder)
	{}

CBadVerTestReadCodec::~CBadVerTestReadCodec()
	{
	}

TFrameState CBadVerTestReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	return EFrameComplete;
	}

void CBadVerTestReadCodec::Complete()
	{
	}

void CBadVerTestReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool /*aDisableErrorDiffusion*/, CFbsBitmap& /*aDestination*/, CFbsBitmap* /*aDestinationMask*/)
	{
	}

//CBadVerTestWriteCodec
CBadVerTestWriteCodec::CBadVerTestWriteCodec()
	{
	}

CBadVerTestWriteCodec::~CBadVerTestWriteCodec()
	{
	}

void CBadVerTestWriteCodec::InitFrameL(TBufPtr8& /*aDst*/, const CFbsBitmap& /*aSource*/)
	{
	}

TFrameState CBadVerTestWriteCodec::ProcessFrameL(TBufPtr8& /*aDst*/)
	{
	return EFrameComplete;	
	}
	
