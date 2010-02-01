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
#include "PPM2Codec.h"
#include "PPM2Panic.h"


// CPpmReadCodec

CPpmReadCodec* CPpmReadCodec::NewL(CPpmDecoder& aDecoder)
	{
	CPpmReadCodec* result = new (ELeave) CPpmReadCodec(aDecoder);
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result);
	return result;
	}

CPpmReadCodec::CPpmReadCodec(CPpmDecoder& aDecoder):
	iDecoder(&aDecoder)
	{}

CPpmReadCodec::~CPpmReadCodec()
	{
	}

void CPpmReadCodec::ProcessFrameL()
	{
	if (iDecoder->Compressed())
		{
		DoProcessCompressedL();
		}		
	else
		{
		DoProcessUncompressedL();	
		}		
	}

TFrameState CPpmReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	__ASSERT_ALWAYS(EFalse, Panic(EIllegalCall));
	return EFrameIncomplete;
	}

void CPpmReadCodec::Complete()
	{
	ImageProcessor()->FlushPixels(); 
	}

void CPpmReadCodec::DoProcessUncompressedL()
	{
	CImageProcessor*const imageProc = ImageProcessor();
#if defined(__CONTINUE_CONVERT)
	iDecoder->ResetPosition(iMarkedPosition);
	for (; iPixelsProcessed<iPixelsExpected; iPixelsProcessed++)
#else 
	iDecoder->ResetPosition(0);
	for (TInt pixelsProcessed=0; pixelsProcessed<iPixelsExpected; pixelsProcessed++)
#endif // defined(__CONTINUE_CONVERT)
		{
		if (ShouldAbort())
			{
			break;
			}
			
		iDecoder->SkipCommentAndWhiteSpaceL();
		TUint rVal = iDecoder->ReadIntL();
		iDecoder->SkipCommentAndWhiteSpaceL();
		TUint gVal = iDecoder->ReadIntL();
		iDecoder->SkipCommentAndWhiteSpaceL();
		TUint bVal = iDecoder->ReadIntL();

		TInt dataShift = iDecoder->DataShift();
		TRgb rgb (rVal<<dataShift, gVal<<dataShift, bVal<<dataShift);

		imageProc->SetPixel(rgb);

#if defined(__CONTINUE_CONVERT)
		iMarkedPosition = iDecoder->Position(); 
#endif // defined(__CONTINUE_CONVERT)
		}
	}

void CPpmReadCodec::DoProcessCompressedL()
	{
	CImageProcessor*const imageProc = ImageProcessor();
#if defined(__CONTINUE_CONVERT)
	iDecoder->ResetPosition(iMarkedPosition);
	for (; iPixelsProcessed<iPixelsExpected; iPixelsProcessed++)
#else 
	iDecoder->ResetPosition(0);
	for (TInt pixelsProcessed=0; pixelsProcessed<iPixelsExpected; pixelsProcessed++)
#endif // defined(__CONTINUE_CONVERT)
		{
		if (ShouldAbort())
			{
			break;
			}
			
		TUint rVal = iDecoder->GetByteL();
		TUint gVal = iDecoder->GetByteL();
		TUint bVal = iDecoder->GetByteL();

		TInt dataShift = iDecoder->DataShift();
		TRgb rgb (rVal<<dataShift, gVal<<dataShift, bVal<<dataShift);

		imageProc->SetPixel(rgb);

#if defined(__CONTINUE_CONVERT)
		iMarkedPosition = iDecoder->Position(); // remember where we get to
#endif // defined(__CONTINUE_CONVERT)
		}
	}

void CPpmReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* /*aDestinationMask*/)
	{
	TSize& originalSize = aFrameInfo.iOverallSizeInPixels;
	CImageProcessor* imageProc = ImageProcessorUtility::NewImageProcessorL(aFrame,originalSize,aFrameInfo.iFrameDisplayMode, aDisableErrorDiffusion);
	SetImageProcessor(imageProc);

#if defined(__CONTINUE_CONVERT)
	ResetFrameL(aFrameInfo, aFrame);
#endif // defined(__CONTINUE_CONVERT)

	ClearBitmapL(aFrame, KRgbWhite);
		// do something sensible for partial streamed decodes

	iPixelsExpected = originalSize.iWidth * originalSize.iHeight;
#if defined(__CONTINUE_CONVERT)
	iMarkedPosition = 0; 
	iPixelsProcessed = 0;
#endif // defined(__CONTINUE_CONVERT)
	}

void CPpmReadCodec::InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameData*/)
	{
	ASSERT(aFrameInfo.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameInfo;
	aFrameInfo.SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CPpmReadCodec::ProcessFrameHeaderL(TBufPtr8&/* aData*/)
	{
	ASSERT(iFrameInfo);
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
	return EFrameComplete;
	}

void CPpmReadCodec::ResetFrameL(TFrameInfo& aFrameInfo, CFbsBitmap& aFrame)
	{
	Pos().SetXY(0,0);

	TSize& originalSize = aFrameInfo.iOverallSizeInPixels;
	CImageProcessor*const imageProc = ImageProcessor();
	ASSERT(imageProc);
	imageProc->PrepareL(aFrame,originalSize);
	}

// CPpm2WriteCodec

const TInt KMaxColourValue=255;

CPpm2WriteCodec::CPpm2WriteCodec(CPpmEncoder* aEncoder):
	iEncoder(aEncoder)
	{}

CPpm2WriteCodec::~CPpm2WriteCodec()
	{}

void CPpm2WriteCodec::ConstructL()
	{
	CImageWriteCodec::ConstructL();
	}

CPpm2WriteCodec* CPpm2WriteCodec::NewL(CPpmEncoder* aEncoder)
	{
	CPpm2WriteCodec* result = new (ELeave) CPpm2WriteCodec(aEncoder);
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(result);
	return result;
	}

void CPpm2WriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource)
	{
	SetSource(&aSource);
	TUint8* destStartPtr = const_cast<TUint8*>(aDst.Ptr());
	TUint8* destPtr = destStartPtr;
#if defined(_DEBUG)
	TUint8* destPtrLimit = destPtr + aDst.MaxLength();
#endif // defined(_DEBUG)

	TSize size = aSource.SizeInPixels();
	iSourceRect = TRect(size);
	iPos.SetXY(0,0);
	iPos.iY = iSourceRect.iBr.iY - 1;

	TBuf8<256> header; // set up header in buffer
	header.Zero();

	// now the standard header lines
	_LIT8(KMagicHeader, "P6\n");
	header.AppendFormat(KMagicHeader); // ppm magic for compressed
	_LIT8(KSizeHeader, "%d %d\n");
	header.AppendFormat(KSizeHeader, size.iWidth, size.iHeight);
	_LIT8(KMaxValueHeader, "%d\n");
	header.AppendFormat(KMaxValueHeader, KMaxColourValue);

	TInt headerSize = header.Length();
	aDst.Copy(header);
	destPtr+=headerSize;

	ASSERT(destPtr < destPtrLimit); // should always be true
	}

TFrameState CPpm2WriteCodec::ProcessFrameL(TBufPtr8& /*aDst*/)
	{
	ASSERT(EFalse); // won't be called in this version
	return EFrameIncomplete;
	}

void CPpm2WriteCodec::DoProcessL(const CFbsBitmap& aSource)
	{
	TSize size = aSource.SizeInPixels();
	TInt numLines = size.iHeight;

	TInt scanLength = aSource.ScanLineLength(size.iWidth, EColor16M);
	// create buffer of the appropriate length
	TUint8* scanBuffer = STATIC_CAST(TUint8*, User::AllocLC(scanLength));

	for (TInt line=0; line<numLines; line++)
		{
		if (ShouldAbort())
			{
			User::Leave(KErrCancel);
			}
			
		TPtr8 scanLine (scanBuffer, 0, scanLength);

		TPoint pos (0, line);
		aSource.GetScanLine(scanLine, pos, scanLength, EColor16M);
		// this comes out in order BGR, so switch
		SwitchRGB(scanLine);

		AppendDataL(scanLine);
		}

	CleanupStack::PopAndDestroy(); // scanBuffer
	}

// Assume original has pixels with BGR in order - switch to RGB
void CPpm2WriteCodec::SwitchRGB(TDes8 &aBuf)
	{
	TUint8* buf = const_cast<TUint8*>(aBuf.Ptr());
	TUint8* bufMax = buf + aBuf.Length();

	while (buf<bufMax)
		{
		TUint8 b = buf[0];
		TUint8 g = buf[1];
		TUint8 r = buf[2];
		buf[0] = r;
		buf[1] = g;
		buf[2] = b;

		buf += 3; 
		}
	}
