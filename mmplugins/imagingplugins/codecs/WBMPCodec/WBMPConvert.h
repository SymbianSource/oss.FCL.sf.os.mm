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

#ifndef WBMPCONVERT_H
#define WBMPCONVERT_H

#include "icl/ImagePlugin.h"
#include "WBMPCodec.h"


// Decoder.
class CWbmpReadCodec;
class CWbmpDecoder : public CImageDecoderPlugin
	{
public:
 	static CWbmpDecoder* NewL();
	virtual ~CWbmpDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

protected:
	// From CImageDecoder
	void ScanDataL();

private:
	CWbmpDecoder();

	static TInt ReadContinuedValueL(const TUint8*& aPtr, const TUint8* aEndPtr);

	// From CImageDecoderPlugin
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();
	
private:
	TSize iSize;
	};

#endif // WBMPCONVERT_H

