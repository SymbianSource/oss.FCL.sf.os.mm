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

#ifndef __BADVERTESTCONVERT_H__
#define __BADVERTESTCONVERT_H__

#include "imageconversion.h"
#include "BadVerTestCodec.h"
#include <icl/imageplugin.h>

// Decoder.
class CBadVerTestReadCodec;
class CBadVerTestDecoder : public CImageDecoderPlugin
	{

public:
	static CBadVerTestDecoder* NewL();
	virtual ~CBadVerTestDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

private:
	CBadVerTestDecoder();

	// From CImageDecoderPlugin
	void ScanDataL();

friend class CBadVerTestReadCodec;
	};

// Encoder.
class CBadVerTestWriteCodec;
class CBadVerTestEncoder : public CImageEncoderPlugin
	{

public:
	static CBadVerTestEncoder* NewL();
	virtual ~CBadVerTestEncoder();

	//From CImageEncoderPlugin
	virtual void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	virtual void UpdateHeaderL();

private:
	CBadVerTestEncoder();

friend class CBadVerTestWriteCodec;
	};

#endif // __BADVERTESTCONVERT_H__

