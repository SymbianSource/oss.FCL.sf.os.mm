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

#ifndef __ICOCONVERT_H__
#define __ICOCONVERT_H__

#include "icl/ImagePlugin.h"
#include "ICOCodec.h"


// Decoder.
class CIcoReadCodec;
class CIcoDecoder : public CImageDecoderPlugin
	{
public:
 	static CIcoDecoder* NewL();
	~CIcoDecoder();

	// From CImageDecoder
	void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

protected:
	// Scan header.
	// Validate that format is correct.
	// Create codec.
	// Fill in image info. (All frames)
	void ScanDataL();
	void Cleanup();

private:
	CIcoDecoder();

	// From CImageDecoderPlugin
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();

private:
	TInt iIconCount;
	};

#endif // __ICOCONVERT_H__

