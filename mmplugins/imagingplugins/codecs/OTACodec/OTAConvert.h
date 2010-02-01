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

#ifndef __OTACONVERT_H__
#define __OTACONVERT_H__

#include "icl/ImagePlugin.h"
#include "OTACodec.h"

// Decoder.
class COtaReadCodec;
class COtaDecoder : public CImageDecoderPlugin
	{
public:
	static COtaDecoder* NewL();
	virtual ~COtaDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

private:
	void ScanDataL();
	COtaDecoder();

	// From CImageDecoderPlugin
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();

private:
	TSize iSize;
	};

#endif // __OTACONVERT_H__

