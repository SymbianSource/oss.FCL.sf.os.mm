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

#ifndef __EXTENDED_TEST_CODEC_H__
#define __EXTENDED_TEST_CODEC_H__

#include <icl/imagecodec.h>

class CExtendedTestDecoder;
class CExtendedTestEncoder;

// Extended Test Read codec.
class CExtendedTestReadCodec : public CImageReadCodec
	{
public:
	CExtendedTestReadCodec(CExtendedTestDecoder &aDecoder);
	virtual ~CExtendedTestReadCodec();

	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	void ConstructL () { CImageReadCodec::ConstructL(); } // make public, since we don't have NewL

protected:
	// from CImageReadCodec
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void Complete();

protected:
	CExtendedTestDecoder *const iDecoder;
	};

// Extended Test Write codec.
class CExtendedTestWriteCodec : public CImageWriteCodec
	{
public:
	CExtendedTestWriteCodec(CExtendedTestEncoder &aEncoder);
	virtual ~CExtendedTestWriteCodec();

	// From CImageWriteCodec
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
	void ConstructL () { CImageWriteCodec::ConstructL(); }	

protected:
	CExtendedTestEncoder *const iEncoder;
	};

#endif // __EXTENDED_TEST_CODEC_H__

