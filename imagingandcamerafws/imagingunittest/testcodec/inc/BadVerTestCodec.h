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

#ifndef __BADVERTESTCODEC_H__
#define __BADVERTESTCODEC_H__

#include <bitdev.h>
#include <icl/imagecodec.h>

#include "BadVerTestConvert.h"

class CBadVerTestDecoder;

// BadVerTest Read codec.
class CBadVerTestReadCodec : public CImageReadCodec
	{
public:
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);

	CBadVerTestReadCodec(CBadVerTestDecoder &aDecoder);
	virtual ~CBadVerTestReadCodec();
	void ConstructL () { CImageReadCodec::ConstructL(); } // make public, since we don't have NewL
protected:
	// from CImageReadCodec
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void Complete();

protected:
	CBadVerTestDecoder *const iDecoder;
	};

// BadVerTest Write codec
class CBadVerTestWriteCodec : public CImageWriteCodec
	{
public:
	CBadVerTestWriteCodec();
	virtual ~CBadVerTestWriteCodec();

protected:
	//From CImageWriteCodec
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);

	};

#endif // __BADVERTESTCODEC_H__

