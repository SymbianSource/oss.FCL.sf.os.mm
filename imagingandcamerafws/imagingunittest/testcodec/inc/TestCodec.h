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

#ifndef __TESTCODEC_H__
#define __TESTCODEC_H__

#include <bitdev.h>
#include <icl/imagecodec.h>

#include "TestConvert.h"

class CTestDecoder;

// Test Read codec.
class CTestReadCodec : public CImageReadCodec
	{
public:
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);

	CTestReadCodec(CTestDecoder &aDecoder);
	virtual ~CTestReadCodec();
	void ConstructL () { CImageReadCodec::ConstructL(); } // make public, since we don't have NewL
protected:
	// from CImageReadCodec
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void Complete();

protected:
	CTestDecoder *const iDecoder;
	};

// Test Write codec
class CTestWriteCodec : public CImageWriteCodec
	{
public:
	CTestWriteCodec();
	virtual ~CTestWriteCodec();

protected:
	//From CImageWriteCodec
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);

	};

#endif // __TESTCODEC_H__

