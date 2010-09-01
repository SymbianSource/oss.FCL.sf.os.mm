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

#ifndef __OTACODEC_H__
#define __OTACODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>

#include "OTAConvert.h"


// OTA Read codec.
class COtaReadCodec : public CImageProcessorReadCodec
	{
public:
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);

	virtual ~COtaReadCodec();

	static COtaReadCodec* NewL(const TSize& aSize);

protected:
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void DoProcessL();

private:
	COtaReadCodec(const TSize& aSize);

protected:
	TSize iOriginalSize;
	TSize iFrameSize;
	const TUint8* iDataPtr; // Not owned
	const TUint8* iDataPtrLimit; // Not owned
	TInt iBytesExpected;
	TInt iBytesProcessed;
	};

#endif // __OTACODEC_H__

