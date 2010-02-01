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

#ifndef __ICOCODEC_H__
#define __ICOCODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>

#include "ICOConvert.h"


// Ico Read codec.
class CIcoReadCodec : public CImageMaskProcessorReadCodec
	{
public:
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);

	~CIcoReadCodec();
	static CIcoReadCodec* NewL(TInt aIconHeadersToProcess);

private:
	CIcoReadCodec(TInt aIconHeadersToProcess);

protected:
	// from CImageReadCodec
	virtual void InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData);
	TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	TFrameState ProcessFrameL(TBufPtr8& aSrc);

	// New
	void DoProcessL();

	void WritePixels(TInt aNumPixels,TBool aUpdatePos = ETrue);
protected:
	TInt iImageDataBytes;
	TInt iIconHeadersToProcess;
	TFrameInfo* iFrameInfo;
	TBool iNewFrame;
	TRgb iPalette[256];
	TUint8* iDataPtr;
	TUint8* iDataPtrLimit;
	TInt iBmpBytesExpected;
	TInt iMaskBytesExpected;


	};

#endif // __ICOCODEC_H__

