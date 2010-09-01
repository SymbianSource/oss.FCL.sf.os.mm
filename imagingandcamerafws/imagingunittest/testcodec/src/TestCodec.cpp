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
#include "TestCodec.h"


// CTestReadCodec
CTestReadCodec::CTestReadCodec(CTestDecoder& aDecoder):
	iDecoder(&aDecoder)
	{}

CTestReadCodec::~CTestReadCodec()
	{
	}

TFrameState CTestReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	return EFrameComplete;
	}

void CTestReadCodec::Complete()
	{
	}

void CTestReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool /*aDisableErrorDiffusion*/, CFbsBitmap& /*aDestination*/, CFbsBitmap* /*aDestinationMask*/)
	{
	}

//CTestWriteCodec
CTestWriteCodec::CTestWriteCodec()
	{
	}

CTestWriteCodec::~CTestWriteCodec()
	{
	}

void CTestWriteCodec::InitFrameL(TBufPtr8& /*aDst*/, const CFbsBitmap& /*aSource*/)
	{
	}

TFrameState CTestWriteCodec::ProcessFrameL(TBufPtr8& /*aDst*/)
	{
	return EFrameComplete;	
	}

	


