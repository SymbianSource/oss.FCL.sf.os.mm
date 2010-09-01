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

#ifndef __TESTCONVERT_H__
#define __TESTCONVERT_H__

#include "imageconversion.h"
#include "TestCodec.h"
#include <icl/imageplugin.h>

// Decoder.
class CTestReadCodec;
class CTestDecoder : public CImageDecoderPlugin
	{

public:
	static CTestDecoder* NewL(TBool aLeaveCorrupt = EFalse);
	virtual ~CTestDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

	// From CImageDecoderPlugin
	virtual void DoConvert();
	virtual TInt NumberOfImageComments() const;
	virtual HBufC* ImageCommentL(TInt aCommentNumber) const;
	virtual TInt NumberOfFrameComments(TInt aFrameNumber) const;
	virtual HBufC* FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const;

protected:
	CTestDecoder(TBool aLeaveCorrupt = EFalse);

	// From CImageDecoderPlugin
	void ScanDataL();

private:
	TInt iMaxValue;
	TBool iLeaveCorrupt;

friend class CTestReadCodec;
	};
	

// Encoder.
class CTestWriteCodec;
class CTestEncoder : public CImageEncoderPlugin
	{

public:
	static CTestEncoder* NewL();
	virtual ~CTestEncoder();

	//From CImageEncoderPlugin
	virtual void DoConvert();
	virtual void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	virtual void UpdateHeaderL();

protected:
	CTestEncoder();

friend class CTestWriteCodec;
	};

// Used to test decoder resolving through TOptions
class CTestExtendedDecoder : public CTestDecoder
	{
public:
	static CTestExtendedDecoder* NewL();
	virtual ~CTestExtendedDecoder();
protected:
	CTestExtendedDecoder();

	// From CImageDecoderPlugin
	virtual void ScanDataL();
};

class CTestExtendedEncoder : public CTestEncoder
	{
public:
	static CTestExtendedEncoder* NewL();
	virtual ~CTestExtendedEncoder();

private:
	CTestExtendedEncoder();

friend class CTestWriteCodec;
	};
	
#endif // __TESTCONVERT_H__

