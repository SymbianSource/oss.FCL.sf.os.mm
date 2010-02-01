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

#ifndef __MBMCONVERT_H__
#define __MBMCONVERT_H__

#include "icl/ImagePlugin.h"
#include "MBMCodec.h"


// Decoder.
class CMbmReadCodec;
class CMbmDecoder : public CImageDecoderPlugin
	{
public:
 	static CMbmDecoder* NewL();
	virtual ~CMbmDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

protected:
	void ScanDataL();

private:
	CMbmDecoder();

	// From CImageDecoderPlugin
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();
protected:
	TUint32 iRootStreamOffset;
	};


// Encoder.
class CMbmWriteCodec;
class CMbmEncoder : public CImageEncoderPlugin
	{
public:
	static CMbmEncoder* NewL();
	~CMbmEncoder();
	// From CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aImageData);
	void UpdateHeaderL();

private:
	CMbmEncoder();

private:
	TDisplayMode iWriteDisplayMode;
	};

#endif // __MBMCONVERT_H__
