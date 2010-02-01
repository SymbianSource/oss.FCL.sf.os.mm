// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Licensed under US Patent No 4,558,302 and foreign counterparts
// 
//

#ifndef GIFCODEC_H
#define GIFCODEC_H

#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>
#include <icl/imagecodecdata.h>

#include "GIFConvert.h"
#include "GIFFormat.h"

// Constants.
const TInt KGifMaxBits = 12;
const TInt KGifConversionTableSize = 1<<KGifMaxBits; // 4096
const TInt KGifHashTableSize = KGifConversionTableSize + 907; // a bit extra for 80% occupancy

const TUint8 KGifExtensionId = 0x21;
const TUint8 KGifImageDescriptorId = 0x2c;
const TUint8 KGifPlainTextExtensionId = 0x01;
const TUint8 KGifGraphicControlExtensionId = 0xf9;
const TUint8 KGifCommentExtensionId = 0xfe;
const TUint8 KGifApplicationExtensionId = 0xff;
const TInt KGifInitialCodeLengthSize = 1;
const TUint8 KGifFrameTerminatorId = 0x00;
const TUint8 KGifTerminatorId = 0x3b;
const TInt KGifCentiSecondsToMicroSeconds = 10000;

const TInt KMinGifGraphicControlBlkLen = 4;
const TInt KGifExtBlkHeaderSize = 3;
const TInt KPixelBufMaxSize = 2048;//to get proper optimization must not be less then 1024 
// (basically each additional 1024 pixels for the buffer adds about 5% to decoding speed 
// untill buffer size increases up to quarter a CPU cash size 

_LIT8(KAppIdNetscape, "NETSCAPE");
_LIT8(KAppAuthenticCode2_0, "2.0");

const TInt KAppIdBlockLength = 11;
const TInt KAppIdLength = 8;
const TInt KAppAuthenticCodeLength = 3;
const TInt KLoopIterationsOffset = 14;


// Read codec
class CGifReadCodec : public CImageMaskProcessorReadCodec
	{
public:
	virtual ~CGifReadCodec();

	static CGifReadCodec* NewL(TRgb* aGlobalPalette,TInt aBackgroundColorIndex, TSize aScreenSize, TBool aFastDecode);

	// From CImageReadCodec
	virtual void InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData);
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	void SetUseFrameSizeInPixels(TBool aUseFrameSizeInPixels);
private:
    typedef TUint32 TBitBuffer;
	typedef TUint16 T64KPixel;
	    
	void ConstructL();
	CGifReadCodec(TRgb* aGlobalPalette,TInt aBackgroundColorIndex, TSize aScreenSize, TBool aFastDecode);

	void DoProcessInfoL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit);
	void DoProcessExtensionL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit);
	void DoProcessImageDescriptorL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit);
	void DoProcessDataL();
	void DoSaveCommentsL();
	inline TInt ResetTableL();
	inline TInt NextCode(TBitBuffer& aBitBuffer, TInt& aBitBufSize, TInt aCurCodeLen);
	void WriteCodeL(TInt aCode);
	void WriteStringWithoutTransparency(TUint8* aOutputString, TInt aPixSize, TInt aNumOfPixels);
	void WriteStringWithTransparency(TUint8* aOutputString, TInt aPixSize, TUint32* aMaskString, TInt aNumOfPixels);
	
	inline void SetCurrentCodeLengthL(TInt aCodeLength);
	inline void UpdateYPos();
	void UpdateYPosInterlaced();
	void FlushPixBuffer(TInt aPixSize);
	
    template <class TPalType, TInt aPtrDelta> 
    inline TUint8* WriteGifBuffer(TUint8* aOutputStringPtr, TUint8* aOutputStringLimit);
    
private:
	
	T64KPixel* i64KPalette;
	const TRgb* iPalette;
	
	TInt iBackgroundColorIndex;
	TUint32 iOpaqueMask;
	TUint16 iTranspColIdx;
	TUint16 iBufMode;
	TInt16* iPrefixIndex;
	TUint8* iSuffixCode;
	TUint8* iOutputString;
		
	TUint32* iPixelBuffer;
	TInt     iPixBufCount;
	TInt     iPixRead;
	TInt     iPixBufferSize;
	
	TUint32* iMaskBuffer;

// do not change data members layout without a reason
// it is to have more efficient CPU caching	
	TInt iNextFree;
	TInt iBitBuffSize;
	TInt iCurrentCodeLength;
	TBitBuffer iBitBuffer;	
	
	const TUint8* iDataPtr;
	const TUint8* iDataPtrLimit;
	
	CImageProcessor* iImgProc;  // not owned
	CImageProcessor* iMaskProc; // not owned
	TUint8  iFirstChar;
	TUint8  iLatestPixSize;
	TInt iPreviousCode;
	TInt iClearCode;
	TInt iEoiCode;
    TPoint* iPPos;
    
	TBool iComplete;
	TInt iPass;
	TInt iYPosIncrement;

	TPoint iPositionOffset;
	TInt iReductionFactor;
	const TSize iScreenSize;

	TBool iTableReset;
	TInt iBlockId;
	TBool iReadingExtensionBlock;
	TBool iReadingCommentExtensionBlock;
	TBool iReadingOtherExtensionBlock;
	TBool iReadingLoopIterationExtensionBlock;

// Local frame settings.	
	TFrameInfo *iFrameInfo;
	CFrameImageData *iFrameData;	
	TRgb* iGlobalPalette;
	TUint8* iStartDataPtr;	//startDataPtr;

	RPointerArray<HBufC8> iComment;
	TInt iCommentIndex;

	// Temporary ImageData pointers. (Not owned by the codec)
	// These ones are used by convert only.
	TGifImageControl *iGifImageControl;
	TGifImageDescriptor *iGifImageDesc;
	TGifColorTable *iGifColorTable;
	TGifLZWInfo *iGifLZWInfo;

	// These ones are used by header processing only.
	TGifImageControl *iFrameImageControl;
	TGifImageDescriptor *iFrameImageDesc;
	TGifLZWInfo *iFrameLZWInfo;

	TSize iFirstFrameSize;
	TRect iFirstFrameCoords;

	TSize iFrameSize;
	TRect iFrameCoords;
	TPoint iFrameOffset;
	TBool iUseFrameSizeInPixels;
	TBool iFast64kMode;
	const TBool iFastDecode;
	TBool iFastAccessMode;
	};


// Write codec.
const TInt KGifBlockSize = 255;
const TInt KGifBufferSize = KGifBlockSize + 9;
class CGifEncoder;
class CGifWriteCodec : public CImageWriteCodec
	{
public:
	static CGifWriteCodec* NewL(const CGifEncoder& aEncoder);

	//From CImageWriteCodec
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
protected:
	CGifWriteCodec(const CGifEncoder& aEncoder);

private:
	void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	void AddToTableL(TInt aIndex, TUint32 aHashValue);
	TBool TableEntry(TInt16 aWaitingCode, TUint8 aNewCode, TInt& aIndex, TUint32& aHashValue);
	void ResetTable();
	void WriteData(TInt aIndex);
	void FillBufferL(const CFbsBitmap& aFrame);
	TBool WriteBuffer();
	TBool WriteTerminator();
private:
	TRect iSourceRect;
	TPoint iPos;
	TInt16 iWaitingCode;
	TInt16 iHashCode[KGifHashTableSize];   // must hold -1..(KGifConversionTableSize-1)
	TUint32 iHashValue[KGifHashTableSize]; // must hold  0..(KGifConversionTableSize-1)<<8|TUint8
	TUint8 iBuffer[KGifBufferSize];
	TUint8* iBufferPtr;
	TInt iBufferSize;
	TBool iBufferFull;
	TBool iImageComplete;
	TInt iCodeLength;
	TInt iNextLimit;
	TInt iClearCode;
	TInt iNextFree;
	TInt iBitsPerPixel;
	TInt iBitOffset;
	TUint8* iDestStartPtr;
	TUint8* iDestPtr;
	TUint8* iDestPtrLimit;
	const CGifEncoder& iEncoder;
	};

#endif // GIFCODEC_H

