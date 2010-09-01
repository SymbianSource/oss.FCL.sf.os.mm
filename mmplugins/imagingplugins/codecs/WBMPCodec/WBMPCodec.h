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

#ifndef __WBMPCODEC_H__
#define __WBMPCODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>

#include "WBMPConvert.h"

// KWBmpMaxImageSize is the maximum size for width and height
const TUint KWBmpMaxImageSize = KMaxTInt32;
// WBMP Read codec.
class CWbmpReadCodec : public CImageProcessorReadCodec
	{
public:
	static CWbmpReadCodec* NewL(const TSize& aWbmpSize);

	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	~CWbmpReadCodec();

protected:
	TFrameState ProcessFrameL(TBufPtr8& aSrc);

	void DoProcessL();

private:
	CWbmpReadCodec(const TSize& aWbmpSize);

protected:
	TSize iOriginalSize;
	TSize iFrameSize;
	const TUint8* iDataPtr;
	const TUint8* iDataPtrLimit;
	TInt iBytesExpected;
	TInt iBytesProcessed;
	};

#endif // __WBMPCODEC_H__

