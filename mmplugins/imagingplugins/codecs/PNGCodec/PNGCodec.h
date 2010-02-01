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

#ifndef __PNGCODEC_H__
#define __PNGCODEC_H__

#include <bitdev.h>
#include <ezcompressor.h>
#include <ezdecompressor.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>

#include "PNGConvert.h"

/** @file
	@internalComponent */

// Constants.
const TInt KPngFileSignatureLength = 8;
const TUint8 KPngSignature[KPngFileSignatureLength] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

const TInt KPngChunkIdSize = 4;
const TInt KPngMaxPLTESize = 256;
const TInt KPngNumInterlacedPasses = 8; // 7 passes plus a safety entry

const TInt KPngChunkLengthSize = 4;
const TInt KPngChunkCRCSize = 4;

const TInt KPngIHDRChunkSize = 13;
const TInt KPngIENDChunkSize = 0;
const TInt KPngcHRMChunkSize = 32;
const TInt KPnggAMAChunkSize = 4;
const TInt KPngpHYsChunkSize = 9;
const TInt KPngtIMEChunkSize = 7;

// KPngMaxImageSize is the maximum size for width and height, as defined in the PNG Specification v1.0 page 14
const TUint KPngMaxImageSize = ((TUint)2 << 30) - 1;
const TInt KPngCrcTableLength = 256;
const TUint KPngCrcMask = 0xffffffff;

_LIT8(KPngIHDRChunkId,"IHDR");
_LIT8(KPngPLTEChunkId,"PLTE");
_LIT8(KPngIDATChunkId,"IDAT");
_LIT8(KPngIENDChunkId,"IEND");
_LIT8(KPngbKGDChunkId,"bKGD");
_LIT8(KPngpHYsChunkId,"pHYs");
_LIT8(KPngtRNSChunkId,"tRNS");
_LIT8(KPngZTXTChunkId,"zTXt");
_LIT8(KPngITXTChunkId,"iTXt");
_LIT8(KPngTEXTChunkId,"tEXt");


// Helper classes.
// TPngImageInformation
class TPngImageInformation : public TFrameInfo
	{
public:
	TPngImageInformation();
public:
	enum TColorElements
		{
		EMonochrome = 0x0,
		EPaletteUsed = 0x1,
		EColorUsed = 0x2,
		EAlphaChannelUsed = 0x4
		};
	enum TColorType
		{
		EGrayscale = EMonochrome, // 0
		EDirectColor = EColorUsed, // 2
		EIndexedColor = EColorUsed | EPaletteUsed, // 3
		EAlphaGrayscale = EMonochrome | EAlphaChannelUsed, // 4
		EAlphaDirectColor = EColorUsed | EAlphaChannelUsed // 6
		};
	enum TCompressionMethod
		{
		EDeflateInflate32K = 0
		};
	enum TFilterMethod
		{
		EAdaptiveFiltering = 0
		};
	enum TInterlaceMethod
		{
		ENoInterlace = 0,
		EAdam7Interlace = 1
		};
	enum TPhysicalUnits
		{
		EUnknownUnits = 0,
		EMeters = 1
		};
public:
	// IHDR chunk
	TSize iSize; // iWidth/iHeight = 1 ... (2^31)-1
	TInt iBitDepth; // 1,2,4,8,16 subject to color type restrictions
	TColorType iColorType; // 0,2,3,4,6
	TCompressionMethod iCompressionMethod; // 0
	TFilterMethod iFilterMethod; // 0
	TInterlaceMethod iInterlaceMethod; // 0 or 1

	// PLTE chunk
	TBool iPalettePresent;
	TRgb iPalette[KPngMaxPLTESize];

	// bKGD chunk
	TBool iBackgroundPresent;
//	TRgb iBackgroundColor; // declared in TFrameInfo

	// pHYs chunk
	TBool iPhysicalPresent;
	TPhysicalUnits iPhysicalUnits;
	TSize iPhysicalSize;

	// tRNS chunk
	TBool iTransparencyPresent;
	TUint16 iTransparentGray;
	TUint16 iTransparentRed;
	TUint16 iTransparentGreen;
	TUint16 iTransparentBlue;
	TUint8 iTransparencyValue[KPngMaxPLTESize];
	};

/**
The Base class for different conversion types.
It is used when Image Processor is ignored for decoding 24 (or 32) bpp non-interlaced PNG images into 24 (or 32 bpp).
All subclasses should provide implementations of SetPixels().
*/
class CFastProcessor : public CBase
	{
public:
    static CFastProcessor* NewL(const TPngImageInformation& aImageInfo, CFbsBitmap* aDestination, CFbsBitmap* aMask, TBool aRgbaMode);
    virtual ~CFastProcessor();
    void End();
    virtual void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos) = 0;
    void Begin();
    
private:    
	void ConstructL();

protected:
    CFastProcessor(CFbsBitmap* aDestination, CFbsBitmap* aMask, TBool aRgbaMode);
    CFbsBitmap* iBitmap;
    CFbsBitmap* iMask;
    TUint8* iBitmapBuffer;
    TUint8* iMaskBuffer;
    TSize iBitmapSize;
    TPoint iPos;
    TBool iRgbaMode;
	};

// Read codec

class CPngReadSubCodec : public CBase
	{
public:
	static CPngReadSubCodec* NewL(CImageProcessor* aImageProc,CImageProcessor* aMaskProc,const TPngImageInformation& aInfo, CFastProcessor* aFastProc, TBool aFastProcessorMode);
	virtual ~CPngReadSubCodec();
	TDes8& FirstBuffer();
	TDes8& DecodeL();
	void ResetL();
	void SetRgbaMode(TBool aMode);
	void Start();
	void Complete();
	void SetAlphaMode(TBool aMode);

protected:
	CPngReadSubCodec();
	void WritePixel(TRgb aPixelColor);
	void WritePixel(TRgb aPixelColor,TUint8 aAlphaValue);
private:
	void ConstructL(CImageProcessor* aImageProc,CImageProcessor* aMaskProc,const TPngImageInformation& aInfo,CFastProcessor* aFastProc, TBool aFastProcessorMode);
	void FilterScanlineDataL(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	TInt PaethPredictor(TInt aLeft,TInt aAbove,TInt aAboveLeft);
	virtual void DoConstructL() = 0;
	virtual TInt ScanlineBufferSize(TInt aPixelLength) = 0;
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit) = 0;
	void UpdatePos();
	TInt ClampValue(TInt aValue, TInt aMinValue, TInt aMaxValue) const;
	void SetFastProcessorMode(TBool aMode); //Sets iFastProcessorMode to True or False
	
protected:
	CImageProcessor* iImageProc;
	CImageProcessor* iMaskProc;
	TPngImageInformation iInfo;
	TInt iScanlineBufferSize;
	TInt iBytesPerPixel;
	HBufC8* iScanlineBuffer1;
	HBufC8* iScanlineBuffer2;
	TPtr8 iScanlineDes1;
	TPtr8 iScanlineDes2;
	TInt iCurrentScanlineBuffer;
	TInt iInterlacedScanlineBufferSize[KPngNumInterlacedPasses];
	TInt iPass;
	TPoint iPos;
	TBool iRgbaMode;
	TRgb* iLineCache;
	TBool iFastProcessorMode; //Sets the current mode
	CFastProcessor* iFastProc;
	TBool iAlphaMode; // Are we decoding to a bitmap that contains an alpha channel?
	};


class MPngDecoder;
class CPngReadCodec : public CImageMaskProcessorReadCodec, public MEZBufferManager
	{
public:
	~CPngReadCodec();

	static CPngReadCodec* NewL(MPngDecoder& aDecoderIFace);
	void SetImageProcessor(CImageProcessor* aImageProc, TBool aOwnsProcessor=ETrue);
	void SetMaskProcessor(CImageProcessor* aMaskProc, TBool aOwnsProcessor=ETrue);
	void SetFastProcessor(CFastProcessor* aFastProc, TBool aOwnsProcessor=ETrue);

	CImageProcessor* MaskProcessor() const;
	CImageProcessor* ImageProcessor() const;
	CFastProcessor* FastProcessor() const;
	
		// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual void InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& /* aFrameImageData */);
	TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	TBool DoProcessDataL();
	void GetNewDataPosition(TInt& aPosition, TInt&  /*aLength*/ );
	TBool SkipImageProcessor(CFbsBitmap& aDestination); //Checks if Image processor is to be used or skipped. If returns ETrue then skip ImageProcessor 
	void Complete();
	void SetMissingiENDChunkFail(TBool aValue);

protected:
	CPngReadCodec(MPngDecoder& aDecoderIFace);
	void ConstructL();

protected:
	virtual void DoProcessPLTEL(const TUint8* aDataPtr,TInt aChunkLength);
	virtual void DoProcesstRNSL(const TUint8* aDataPtr,TInt aChunkLength);
	virtual void DoProcessbKGDL(const TUint8* aDataPtr,TInt aChunkLength);
	virtual void DoProcesspHYsL(const TUint8* aDataPtr,TInt aChunkLength);

private:
	void DoProcessInfoL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit);
	void DoProcessIHDRL(const TUint8* aDataPtr,TInt aChunkLength);

	TBool SetupProcessData(TBufPtr8& aSrc,
						   TUint8* aStartDataPtr,
						   const TUint8*& aDataPtr,
						   const TUint8* aDataPtrLimit);

	// From MEZBufferManager
	virtual void InitializeL(CEZZStream &aZStream);
	virtual void NeedInputL(CEZZStream &aZStream);
	virtual void NeedOutputL(CEZZStream &aZStream);
	virtual void FinalizeL(CEZZStream &aZStream);
protected:
	MPngDecoder& iDecoderIFace;
	TFrameInfo* iFrameInfo;
	TPngImageInformation iImageInfo;
	TBuf8<KPngChunkIdSize> iChunkId;
	TInt iChunkBytesRemaining;
	CPngReadSubCodec* iDecoder;
	CEZDecompressor* iDecompressor;
	TPtrC8 iDataDes;
	// used for async decode...
	const TUint8* iDataPtr;
	TBufPtr8* iSavedSrc;
	TUint8* iStartDataPtr;
	CImageProcessor* iImageProc;
	TBool iOwnsImageProcessor;
	CImageProcessor* iMaskProc;
	TBool	iOwnsMaskProcessor;
	CFastProcessor* iFastProc;
	TBool	iOwnsFastProcessor;
private:	
	TInt iNewPosition;
	TBool iReadMore;
	TBool iPreviousChunkReadFailed;
	TBool iMissingiENDChunkFail;
	TInt iPreviousDataPos;
	TInt iPreviousDataLength;
	};

// Write sub-codec
class CPngWriteSubCodec : public CBase
	{
public:
	static CPngWriteSubCodec* NewL(const TPngImageInformation& aInfo, const CFbsBitmap* aSource);
	virtual ~CPngWriteSubCodec();
protected:
	CPngWriteSubCodec();
private:
	void ConstructL(const TPngImageInformation& aInfo, const CFbsBitmap* aSource);
public:
	TDes8& Buffer();
	inline TInt BufferSize() const;
	TDes8& EncodeL(const TInt aScanline);
	inline CPalette* Palette() const;
protected:
	TUint8 ReverseBits(const TUint8 aValue) const;
	void EncodePalettedScanline(TUint8* aDataPtr, const CFbsBitmap* aSource, const TInt aScanline,
								const TInt aPixelsPerByte, const TInt aShiftValue);
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength) = 0;
	virtual void DoEncode(const CFbsBitmap* aSource, const TInt aScanline,
					TUint8* aDataPtr, const TUint8* aDataPtrLimit) = 0;
protected:
	TPngImageInformation iInfo;
	const CFbsBitmap* iSource;
	TInt iScanlineBufferSize;
	HBufC8* iScanlineBuffer;
	TPtr8 iScanlineDes;
	CPalette* iPalette;

	friend class RPngWriteSubCodecTest;
	};

// Write codec
class CPngWriteCodec : public CImageWriteCodec, public MEZBufferManager
	{
private:
	enum TPngEncoderState
		{
		EPngInit,		// Initialize compressor
		EPngDeflate,	// Deflate encoded image data
		EPngWritePLTE,	// Write PLTE chunk
		EPngWriteIDAT,	// Write IDAT chunk
		EPngEndChunk	// Write end chunk
		};
public:
	static CPngWriteCodec* NewL(CPngEncoder& aPlugin, TInt aBpp, TBool aColor, TBool aPaletted, TInt aCompressionLevel);

	virtual ~CPngWriteCodec();
public:
	void GetCrc(TUint32& aCrc, const TUint8* aPtr, const TInt aLength);
	TBool DeflateEncodedDataL();
private:
	CPngWriteCodec(CPngEncoder& aPlugin, TInt aBpp, TBool aColor, TBool aPaletted, TInt aCompressionLevel);

	void WritePngChunk(TUint8*& aDestPtr, const TDesC8& aChunkId, const TDesC8& aData, TInt& aLength);
	void InitializeCompressorL(TBufPtr8& aDst);
	void SetCompressorOutputL(TBufPtr8& aDst);
	void WritePLTEChunk(TBufPtr8& aDst);
	void WriteIDATChunk(TBufPtr8& aDst);
	void WriteEndChunk(TBufPtr8& aDst);
	TInt WriteHeaderChunk(TBufPtr8& aDst);
	void GetImageDataL(TInt& aBytesToProcess);
	void GetPngScanLine(TDes8& aBuf, const TPoint& aPixel, TInt& aLength);
	void CalcCrcTable();
	// from CImageWriteCodec
public:
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
	// from MEZBufferManager
private:
	virtual void InitializeL(CEZZStream &aZStream);
	virtual void NeedInputL(CEZZStream &aZStream);
	virtual void NeedOutputL(CEZZStream &aZStream);
	virtual void FinalizeL(CEZZStream &aZStream);
private:
	TUint32 iCrcTable[KPngCrcTableLength];
	TBool iCrcTableCalculated;
	CPngWriteSubCodec* iEncoder;
	TPngEncoderState iEncoderState;
	TInt iCompressionLevel;
	CEZCompressor* iCompressor;
	TInt iScanline;
	TUint8* iDestStartPtr;
	TUint8* iDestPtr;
	TUint8* iDestPtrLimit;
	TPngImageInformation iImageInfo;
	TPtr8 iCompressorPtr;	// data area for compressor to write to
	TBool iCallAgain;
	CPngEncoder& iPlugin;

	friend class RPngWriteCodecTest;
	};


#include "PngCodec.inl"

#endif // __PNGCODEC_H__
