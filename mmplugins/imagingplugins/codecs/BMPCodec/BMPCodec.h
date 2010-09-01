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

#ifndef BMPCODEC_H
#define BMPCODEC_H

#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>
#include "BMPConvert.h"

/**
@file
@internalComponent
*/

// Read codecs
class CBmpDecoder;
class CBmpReadCodec : public CImageProcessorReadCodec
	{
public:

	// From CImageReadCodec
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual void InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& aFrameData);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
protected:
	
	class TCompDecodeInfo
		{
	public:
		TUint 	iMask;
		TInt 	iRightShiftValue;
		TInt 	iLeftShiftValue;
		};
	enum
		{
		KRedCompIdx 	= 0,
		KGreenCompIdx 	= 1,
		KBlueCompIdx 	= 2
		};

	CBmpReadCodec(const TSize& aBmpSize, const TRgb* aPalette);
	~CBmpReadCodec();
	virtual void DoProcessL() = 0;
	virtual void DoNewFrameL();
	void WritePixels(TInt aNumPixels, TBool aUpdatePos = ETrue);
protected:
	TSize iOriginalSize;
	TSize iFrameSize;
	const TRgb* iPalette; // Not owned
	const TUint8* iDataPtr; // Not owned
	const TUint8* iDataPtrLimit; // Not owned
	TInt iBytesExpected;
	TInt iBytesProcessed;
	TFrameInfo* iFrameInfo; // Not owned
	};

class CBmp1BppReadCodec : public CBmpReadCodec
	{
public:

	static CBmp1BppReadCodec* NewL(const TSize& aBmpSize, const TRgb* aPalette);

	
	//From CBmpReadCodec
	virtual void DoProcessL();
	virtual void DoNewFrameL();
private:

	CBmp1BppReadCodec(const TSize& aBmpSize, const TRgb* aPalette);

	TRgb iEntry0;
	TRgb iEntry1;
	};

class CBmpNoComp4BppReadCodec : public CBmpReadCodec
	{
public:

	static CBmpNoComp4BppReadCodec* NewL(const TSize& aBmpSize, const TRgb* aPalette);

	//From CBmpReadCodec
	virtual void DoNewFrameL();
	virtual void DoProcessL();

private:
	CBmpNoComp4BppReadCodec(const TSize& aBmpSize, const TRgb* aPalette);
	};

class CBmpRLE4ReadCodec : public CBmpReadCodec
	{
public:

	static CBmpRLE4ReadCodec* NewL(const TSize& aBmpSize, const TRgb* aPalette);

	//From CBmpReadCodec
	virtual void DoNewFrameL();
	virtual void DoProcessL();
private:
	CBmpRLE4ReadCodec(const TSize& aBmpSize, const TRgb* aPalette);

	void WriteRun(TUint8 aValue, TInt aNumPixels);
	void WriteData(TInt aNumPixels);
	};

class CBmpNoComp8BppReadCodec : public CBmpReadCodec
	{
public:

	static CBmpNoComp8BppReadCodec* NewL(const TSize& aBmpSize, const TRgb* aPalette);

	//From CBmpReadCodec
	virtual void DoNewFrameL();
	virtual void DoProcessL();

private:
	CBmpNoComp8BppReadCodec(const TSize& aBmpSize, const TRgb* aPalette);

	};

class CBmpRLE8ReadCodec : public CBmpReadCodec
	{
public:

	static CBmpRLE8ReadCodec* NewL(const TSize& aBmpSize, const TRgb* aPalette);

	//From CBmpReadCodec
	virtual void DoNewFrameL();
	virtual void DoProcessL();
private:
	CBmpRLE8ReadCodec(const TSize& aBmpSize, const TRgb* aPalette);
	void WriteRun(TUint8 aValue, TInt aNumPixels);
	void WriteData(TInt aNumPixels);
	};

/**
   codec which can handle "bit field" compressed bitmaps
*/
class CBmpBiRgbReadCodec : public CBmpReadCodec
	{
public:

	static CBmpBiRgbReadCodec* NewL(const TSize& aBmpSize, TInt aPixelSize, const TRgb* aPalette);

	//From CBmpReadCodec
	virtual void DoProcessL();
	virtual void DoNewFrameL();
	
protected:
	enum 
		{
		KMaxComps	= KBlueCompIdx + 1 // RGB
		};
	
	CBmpBiRgbReadCodec(const TSize& aBmpSize, TInt aPixelSize, const TRgb* aPalette);
	
	void ConstructL();
	
	const TInt iPixelSize;		// pixel size in bytes

	TInt iTotalByteWidth;		// line byte width with possible padding
	TInt iPixelByteWidth;		// line byte width without possible padding
	TInt iPaddingByteWidth;		// padding value
	TInt iBytePos;				// current byte position
	TCompDecodeInfo iCompInfo[KMaxComps]; // color comps "decompression" parameters
	};

class CBmp24BppReadCodec : public CBmpReadCodec
	{
public:

	static CBmp24BppReadCodec* NewL(const TSize& aBmpSize);

	//From CBmpReadCodec
	virtual void DoProcessL();
	virtual void DoNewFrameL();
private:
	CBmp24BppReadCodec(const TSize& aBmpSize);

	TInt iTotalByteWidth;
	TInt iPixelByteWidth;
	TInt iPaddingByteWidth;
	TInt iBytePos;
	};

class CBmp32BppReadCodec : public CBmpReadCodec
	{
public:

	static CBmp32BppReadCodec* NewL(const TSize& aBmpSize);

	//From CBmpReadCodec
	virtual void DoProcessL();
	virtual void DoNewFrameL();
private:
	CBmp32BppReadCodec(const TSize& aBmpSize);

	TInt iTotalByteWidth;
	TInt iPixelByteWidth;
	TInt iPaddingByteWidth;
	TInt iBytePos;
	};

// Write codecs
class CBmpWriteCodec : public CImageWriteCodec
	{
public:
	// from CImageWriteCodec
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
protected:

	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
	virtual void DoProcessL(const CFbsBitmap& aFrame) = 0;
	virtual TInt PaddingBytes() = 0;
protected:
	TRect iSourceRect;
	TPoint iPos;
	TUint8* iDestPtr;
	TUint8* iDestPtrLimit;
	TInt iPaddingBytes;
	TInt iBytesToWrite;
	};

class CBmp1BppWriteCodec : public CBmpWriteCodec
	{
public:

	static CBmp1BppWriteCodec* NewL(void);

	//from CBmpWriteCodec
	virtual void DoProcessL(const CFbsBitmap& aFrame);
	virtual TInt PaddingBytes();
	};

class CBmp4BppWriteCodec : public CBmpWriteCodec
	{
public:
	static CBmp4BppWriteCodec* NewL(void);

	//from CBmpWriteCodec
	virtual void DoProcessL(const CFbsBitmap& aFrame);
	virtual TInt PaddingBytes();
	};

class CBmp8BppWriteCodec : public CBmpWriteCodec
	{
public:
	static CBmp8BppWriteCodec* NewL(void);

	//from CBmpWriteCodec
	virtual void DoProcessL(const CFbsBitmap& aFrame);
	virtual TInt PaddingBytes();
	};

class CBmp24BppWriteCodec : public CBmpWriteCodec
	{
public:
	static CBmp24BppWriteCodec* NewL(void);

	//from CBmpWriteCodec
	virtual void DoProcessL(const CFbsBitmap& aFrame);
	virtual TInt PaddingBytes();
	};

#endif // BMPCODEC_H

