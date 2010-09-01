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

#ifndef __PPM2CODEC_H__
#define __PPM2CODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>

#include "PPM2Convert.h"

class CPpmDecoder;

// PPM Read codec.
class CPpmReadCodec : public CImageProcessorReadCodec
	{
public:
	static CPpmReadCodec* NewL(CPpmDecoder& aDecoder);
	void ProcessFrameL();
	void ResetFrameL(TFrameInfo& aFrameInfo, CFbsBitmap& aFrame);
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual void InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& aFrameData);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	void Complete();

	virtual ~CPpmReadCodec();
protected:
	CPpmReadCodec(CPpmDecoder& aDecoder);
	void DoProcessCompressedL();
	void DoProcessUncompressedL();
	TBool ShouldAbort() const;

private:
	// from CImageProcessorReadCodec
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
protected:
	CPpmDecoder *const iDecoder;
	TSize iFrameSize;
	const TUint8* iDataPtr; // Not owned
	const TUint8* iDataPtrLimit; // Not owned
	TInt iPixelsExpected;
#if defined(__CONTINUE_CONVERT)
	TInt iMarkedPosition;
	TInt iPixelsProcessed;
#endif // defined(__CONTINUE_CONVERT)
	TFrameInfo* iFrameInfo; // Not owned
	};

// Write codec
class CPpm2WriteCodec : public CImageWriteCodec
	{
public:
	static CPpm2WriteCodec* NewL(CPpmEncoder* aEncoder);
	virtual ~CPpm2WriteCodec();

	void DoProcessL(const CFbsBitmap& aSource);

	// from CImageWriteCodec
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
private:
	CPpm2WriteCodec(CPpmEncoder* aEncoder);
	void ConstructL ();

	TBool ShouldAbort() const;
	void AppendDataL(const TDesC8& aData);
	void SwitchRGB(TDes8 &aBuf);
private:
	TRect iSourceRect;
	TPoint iPos;

	CPpmEncoder* iEncoder; // not linked
	};

#include "PPM2Codec.inl"

#endif // __PPM2CODEC_H__

