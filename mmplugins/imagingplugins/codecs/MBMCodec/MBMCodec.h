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

#ifndef __MBMCODEC_H__
#define __MBMCODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>
#include <icl/imagecodecdata.h>

#include "MBMConvert.h"


// Constants.
const TInt KMbmRootStreamOffsetPosition = 16;
const TInt KMbmFirstBitmapOffset = KMbmRootStreamOffsetPosition + 4;
const TInt KMbmInfoHeaderSize = 40;

// Helper classes.
class TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit) = 0;
	void AddPixelRun(TRgb aColor,TInt aNumberOfPixels);
public:
	CImageProcessor* iImageProc;
	};

class T1BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	void AddPixelValue(TUint8 aValue);
	};

class T1BppCompressedMbmReadCodec : public T1BppMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	void AddPixelRun(TUint8 aValue,TInt aNumberOfValues);
	};

class T2BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	void AddPixelValue(TUint8 aValue);
	};

class T2BppCompressedMbmReadCodec : public T2BppMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	void AddPixelRun(TUint8 aValue,TInt aNumberOfQuads);
	};

class T4BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	virtual TRgb ColorFromIndex(TInt aIndex) = 0;
	};

class T4BppCompressedMbmReadCodec : public T4BppMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	void AddPixelRun(TRgb aColor1,TRgb aColor2,TInt aNumberOfPairs);
	};

class T4BppMonoMbmReadCodec : public T4BppMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Gray16(aIndex); }
	};

class T4BppMonoCompressedMbmReadCodec : public T4BppCompressedMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Gray16(aIndex); }
	};

class T4BppColorMbmReadCodec : public T4BppMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Color16(aIndex); }
	};

class T4BppColorCompressedMbmReadCodec : public T4BppCompressedMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Color16(aIndex); }
	};

class T8BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	virtual TRgb ColorFromIndex(TInt aIndex) = 0;
	};

class T8BppCompressedMbmReadCodec : public T8BppMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T8BppMonoMbmReadCodec : public T8BppMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Gray256(aIndex); }
	};

class T8BppMonoCompressedMbmReadCodec : public T8BppCompressedMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Gray256(aIndex); }
	};

class T8BppColorMbmReadCodec : public T8BppMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Color256(aIndex); }
	};

class T8BppColorCompressedMbmReadCodec : public T8BppCompressedMbmReadCodec
	{
public:
	virtual TRgb ColorFromIndex(TInt aIndex) { return TRgb::Color256(aIndex); }
	};

class T12BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T12BppCompressedMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T16BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T16BppCompressedMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T24BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T24BppCompressedMbmReadCodec : public T24BppMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

class T32BppMbmReadCodec : public TMbmReadCodec
	{
public:
	virtual void DoProcessL(TUint8*& aDataPtr,TUint8* aDataPtrLimit);
	};

// MBM Read codec.
class CMbmReadCodec : public CImageProcessorReadCodec
	{
public:
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* aFrameMask);

	static CMbmReadCodec* NewL(TUint32 aMbmRootStreamOffset);

protected:
	virtual ~CMbmReadCodec();

	virtual void InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);

private:
	CMbmReadCodec(TUint32 aMbmRootStreamOffset);

	TBool ProcessInfoL(TBufPtr8* aSrc);
	TInt ScanLinePixels(TInt aWidth);
	void SetCodecProcessorL();
	
	static TDisplayMode DisplayModeL(TInt aBitsPerPixel, TUint32 aFlags);
	
protected:
	TFrameInfo* iFrameInfo;
	CFrameImageData *iImageData;

	TMbmDecodeData* iFrameData;
	TBool iCompressed;
	TInt iByteLength;
	TUint8* iDataPtr;
	TUint8* iStartDataPtr;
	TUint8* iDataPtrLimit;
	TMbmReadCodec* iCodecProc;
	TInt32 iMbmRootStreamOffset;
	TInt iNumberOfFrames;
	TInt iFrameDataRemaining;
	};


// Write codec classes.
class TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) = 0;
	virtual TInt BytesToPixels(TInt aBytes) = 0;
	};

class T1BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return (aPixels + 7) >> 3; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes << 3; }
	};

class T2BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return (aPixels + 3) >> 2; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes << 2; }
	};

class T4BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return (aPixels + 1) >> 1; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes << 1; }
	};

class T8BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return aPixels; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes; }
	};

class T16BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return aPixels << 1; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes >> 1; }
	};

class T24BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return aPixels * 3; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes / 3; }
	};

class T32BppWriteCodecProcessor : public TWriteCodecProcessor
	{
public:
	virtual TInt PixelsToBytes(TInt aPixels) { return aPixels * 4; }
	virtual TInt BytesToPixels(TInt aBytes) { return aBytes / 4; }
	};

class CMbmWriteCodec : public CImageWriteCodec
	{
public:
	virtual ~CMbmWriteCodec();

	static CMbmWriteCodec* NewL(TDisplayMode aDisplayMode);

protected:
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aFrame);

	// New functions
	void DoProcessL(const CFbsBitmap& aFrame);
	TInt ScanLinePixels(TInt aWidth);

private:
	CMbmWriteCodec(TDisplayMode aDisplayMode);


protected:
	TRect iSourceRect;
	TPoint iPos;
	TDisplayMode iDisplayMode;
	TWriteCodecProcessor* iCodecProc;
	TUint8* iDataPtr;
	TUint8* iDataPtrLimit;
	TInt iPaddingBytes;
	TInt iBytesToWrite;
	TBool iStreamTablePending;
	};


#endif // __MBMCODEC_H__

