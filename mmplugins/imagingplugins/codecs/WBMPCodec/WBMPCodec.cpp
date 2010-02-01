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

#include <fbs.h>
#include "WBMPCodec.h"


// CWbmpReadCodec
CWbmpReadCodec::CWbmpReadCodec(const TSize& aWbmpSize):
	iOriginalSize(aWbmpSize)
	{}

CWbmpReadCodec::~CWbmpReadCodec()
	{
	}

CWbmpReadCodec* CWbmpReadCodec::NewL(const TSize& aWbmpSize)
{
	CWbmpReadCodec* self = new(ELeave) CWbmpReadCodec(aWbmpSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
}


TFrameState CWbmpReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	const TUint8* srcStart = aSrc.Ptr();
	iDataPtr = srcStart;
	iDataPtrLimit = srcStart + aSrc.Length();

	DoProcessL();

	TInt bytesUsed = iDataPtr - srcStart;
	aSrc.Shift(bytesUsed);

	iBytesProcessed += bytesUsed;
	if (iBytesProcessed >= iBytesExpected)
		{
		ImageProcessor()->FlushPixels();
		return EFrameComplete;
		}

	return EFrameIncomplete;
	}

void CWbmpReadCodec::DoProcessL()
	{
	CImageProcessor*const imageProc = ImageProcessor();
	while (iDataPtr < iDataPtrLimit)
		{
		TUint8 value = *iDataPtr++;

		// code will compare value with successive masks 0x80, 0x40 etc downto but not including 0
		for (TUint mask=0x80; mask!=0; mask >>= 1)
			imageProc->SetPixel((value & mask) ? KRgbWhite : KRgbBlack);
		}
	}

void CWbmpReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* /*aDestinationMask*/)
	{
	Pos().SetXY(0,0);

	const TSize destinationSize(aFrame.SizeInPixels());
	TInt reductionFactor = ReductionFactor(iOriginalSize, destinationSize);
	CImageProcessor* imageProc = ImageProcessorUtility::NewImageProcessorL(aFrame, reductionFactor, EGray2, aDisableErrorDiffusion);
	SetImageProcessor(imageProc);
	imageProc->PrepareL(aFrame,iOriginalSize);

	iBytesProcessed = 0;

	// clear bitmap so partial decodes don't produce rubbish on screen
	ClearBitmapL(aFrame, KRgbWhite);

	TInt actualWidth = (iOriginalSize.iWidth + 7) & ~7;
	imageProc->SetPixelPadding(actualWidth - iOriginalSize.iWidth);
	iBytesExpected = (actualWidth >> 3) * iOriginalSize.iHeight;
	}

