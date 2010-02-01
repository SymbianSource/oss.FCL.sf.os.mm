// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef JPEGCODEC_H
#define JPEGCODEC_H

#include "JPEGConvert.h"
#include "pluginextensionmanager.h"
#include "fwextconstants.h"

#ifdef JPEG_DEBUG_OUTPUT

#define JPEG_DEBUG_LEAVES

#define JPEG_ASSERT(a)													\
	do																	\
		{																\
		if (!(a))														\
			{															\
			RDebug::Printf("*** Assertion failed: " #a);				\
			RDebug::Printf("*** " __FILE__ " at line %d", __LINE__);	\
			ASSERT(EFalse);												\
			}															\
		}																\
	while (EFalse)

// Parameter 'a' should be a C-style string.
#define JPEG_DEBUG1(a)			RDebug::Printf("JPEG: " a)
#define JPEG_DEBUG2(a, b)		RDebug::Printf("JPEG: " a, b)
#define JPEG_DEBUG3(a, b, c)	RDebug::Printf("JPEG: " a, b, c)
#define JPEG_DEBUG4(a, b, c, d)	RDebug::Printf("JPEG: " a, b, c, d)
#define JPEG_DEBUG5(a, b, c, d, e)	RDebug::Printf("JPEG: " a, b, c, d, e)
#define JPEG_DEBUG6(a, b, c, d, e, f) RDebug::Printf("JPEG: " a, b, c, d, e, f)
#define JPEG_DEBUG7(a, b, c, d, e, f, g) RDebug::Printf("JPEG: " a, b, c, d, e, f, g)
#define JPEG_DEBUG8(a, b, c, d, e, f, g, h) RDebug::Printf("JPEG: " a, b, c, d, e, f, g, h)
#else
#define JPEG_ASSERT(a)
#define JPEG_DEBUG1(a)
#define JPEG_DEBUG2(a, b)
#define JPEG_DEBUG3(a, b, c)
#define JPEG_DEBUG4(a, b, c, d)
#define JPEG_DEBUG5(a, b, c, d, e)
#define JPEG_DEBUG6(a, b, c, d, e, f)
#define JPEG_DEBUG7(a, b, c, d, e, f, g)
#define JPEG_DEBUG8(a, b, c, d, e, f, g, h)
#endif

#ifdef JPEG_DEBUG_LEAVES

// A more debugging friendly way of leaving.
// The temporary variable is absolutely necessary and will break code if removed.
#define JPEG_LEAVE_IF_ERROR(err, reason)									\
	do																		\
		{																	\
		TInt temp = err;													\
		if (temp != KErrNone)												\
			{																\
			RDebug::Printf("JPEG: *** Leave(%d): %d \"" reason "\"", __LINE__, temp);	\
			User::Leave(temp);												\
			}																\
		}																	\
	while (EFalse)

// A more debugging friendly way of leaving.
#define JPEG_LEAVE(err, reason) 										\
	do																	\
		{																\
		TInt temp = err;												\
		RDebug::Printf("JPEG: *** Leave(%d): %d \"" reason "\"", __LINE__, temp);	\
		User::Leave(temp);												\
		}																\
	while (EFalse)

#else
#define JPEG_LEAVE_IF_ERROR(err, reason)	User::LeaveIfError(err)
#define JPEG_LEAVE(err, reason)			User::Leave(err)
#endif

class CImageFrame;
class CRawColorProcessor;
class CJpgImageFrameProcessor;
class TJpgFrameInfo;
class TDataUnit;
class TJpgScanInfo;
class CJpegDecoder;

// CJpgImageFrameReadCodec
class CJpgImageFrameReadCodec : public CBase
	{
public:
	static CJpgImageFrameReadCodec* NewL(CImageFrame* aFrame);
	~CJpgImageFrameReadCodec();

	void CreateImageProcessorL(const TJpgFrameInfo& aFrameInfo);		
	void ProcessL(const RArray<const TDataUnit*>& compPtr);
	CImageFrame* Destination();
	void SetImageFrameBlocksL(CImageFrame* aFrame, const TJpgFrameInfo& aFrameInfo);

private:
	CJpgImageFrameReadCodec(CImageFrame* aFrame);

private:
	CImageFrame* iDestination;
	CJpgImageFrameProcessor* iImageFrameProcessorPtr;
	};


//
// CJpgReadCodec
// The base class for all JPEG decoding.
//
class CJpgReadCodec : public CImageProcessorReadCodecExtension
	{
public:
	friend class CSequentialJpgReadCodec;
	friend class CMultiScanSequentialJpgReadCodec;
	//
	// Values and explanations are based on the book
	// "JPEG Still Image Data Compression Standard" by W.B.Pennebaker and J.L.Mitchell.
	//
	enum
		{
		EMarkerEscape = 0xFF00,	// Not really a marker but used to escape valid 0xFF bitstream sequences.
		EMarkerTEM = 0xFF01,	// For temporary use in arithmetic coding.
		EMarkerRES0 = 0xFF02,	// Reserved.
		EMarkerRESn = 0xFFBF,	// ...
		
		EMarkerSOF0 = 0xFFC0,	// Baseline DCT.
		EMarkerSOF1 = 0xFFC1,	// Extended sequential DCT.
		EMarkerSOF2 = 0xFFC2,	// Progressive DCT.
		EMarkerSOF3 = 0xFFC3,	// Lossless (sequential).
		EMarkerDHT = 0xFFC4,	// Define Huffman table(s).
		EMarkerSOF5 = 0xFFC5,	// Differential sequential DCT.
		EMarkerSOF6 = 0xFFC6,	// Differential progressive DCT.
		EMarkerSOF7 = 0xFFC7,	// Differential lossless.
		EMarkerJPG = 0xFFC8,	// Reserved.
		EMarkerSOF9 = 0xFFC9,	// Extended sequential DCT.
		EMarkerSOF10 = 0xFFCA,	// Progressive DCT.
		EMarkerSOF11 = 0xFFCB,	// Lossless (sequential).
		EMarkerDAC = 0xFFCC,	// Define arithmetic conditioning table(s).
		EMarkerSOF13 = 0xFFCD,	// Differential sequential DCT.
		EMarkerSOF14 = 0xFFCE,	// Differential progressive DCT.
		EMarkerSOF15 = 0xFFCF,	// Differential lossless.
		
		EMarkerRST0 = 0xFFD0,	// First restart marker.
		EMarkerRST1 = 0xFFD1,	// ...
		EMarkerRST2 = 0xFFD2,	// ...
		EMarkerRST3 = 0xFFD3,	// ...
		EMarkerRST4 = 0xFFD4,	// ...
		EMarkerRST5 = 0xFFD5,	// ...
		EMarkerRST6 = 0xFFD6,	// ...
		EMarkerRST7 = 0xFFD7,	// Last restart marker.
		EMarkerSOI = 0xFFD8,	// Start of image.
		EMarkerEOI = 0xFFD9,	// End of image.
		EMarkerSOS = 0xFFDA,	// Start of scan.
		EMarkerDQT = 0xFFDB,	// Define quantization table(s).
		EMarkerDNL = 0xFFDC,	// Define number of lines.
		EMarkerDRI = 0xFFDD,	// Define restart interval.
		EMarkerDHP = 0xFFDE,	// Define hierarchical progression.
		EMarkerEXP = 0xFFDF,	// Expand reference image(s).
	
		EMarkerAPP0 = 0xFFE0,	// Reserved for application use.
		EMarkerAPP1 = 0xFFE1,	// ...
		EMarkerAPP2 = 0xFFE2,	// ...
		EMarkerAPP3 = 0xFFE3,	// ...
		EMarkerAPP4 = 0xFFE4,	// ...
		EMarkerAPP5 = 0xFFE5,	// ...
		EMarkerAPP6 = 0xFFE6,	// ...
		EMarkerAPP7 = 0xFFE7,	// ...
		EMarkerAPP8 = 0xFFE8,	// ...
		EMarkerAPP9 = 0xFFE9,	// ...
		EMarkerAPP10 = 0xFFEA,	// ...
		EMarkerAPP11 = 0xFFEB,	// ...
		EMarkerAPP12 = 0xFFEC,	// ...
		EMarkerAPP13 = 0xFFED,	// ...
		EMarkerAPP14 = 0xFFEE,	// ...
		EMarkerAPP15 = 0xFFEF,	// ...
			
		EMarkerJPG0 = 0xFFF0,	// Reserved for JPEG extensions.
		EMarkerJPG1 = 0xFFF1,	// ...
		EMarkerJPG2 = 0xFFF2,	// ...
		EMarkerJPG3 = 0xFFF3,	// ...
		EMarkerJPG4 = 0xFFF4,	// ...
		EMarkerJPG5 = 0xFFF5,	// ...
		EMarkerJPG6 = 0xFFF6,	// ...
		EMarkerJPG7 = 0xFFF7,	// ...
		EMarkerJPG8 = 0xFFF8,	// ...
		EMarkerJPG9 = 0xFFF9,	// ...
		EMarkerJPG10 = 0xFFFA,	// ...
		EMarkerJPG11 = 0xFFFB,	// ...
		EMarkerJPG12 = 0xFFFC,	// ...
		EMarkerJPG13 = 0xFFFD,	// ...
		EMarkerCOM = 0xFFFE,	// Comment.
		}
	TMarker;
	
	void SetYuvDecode(TBool aYuvDecode);
	void SetHighSpeedMode(TBool aHighSpeedMode);
	TInt RecommendBufferSizeL(TUid aFormatCode);
	TInt GetStreamBufferSizeL(TUid aFormatCode, TSize& aBlockSizeInPixels, TInt aNumBlocks);
	virtual void InitDrawFrame();
	virtual TBool DrawFrameL();

	//From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* aFrameMask);
	CJpgReadCodec(const TJpgFrameInfo& aFrameInfo,const TJpgScanInfo& aScanInfo);
	~CJpgReadCodec();
	virtual void InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& aFrameData);
	//Jpg codec support for CImageFrame
	virtual void InitFrameL(CImageFrame& aFrame);
	virtual void PreInitFrameL();
	virtual void PostInitFrameL();
	
	//JPEG codec support for streaming
	virtual void InitFrameL(TUid aFormat, TDecodeStreamCaps::TNavigation aNavigation);
	virtual void GetBlocksL(CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);
	virtual void GetNextBlocksL(CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks);
	
	void ValidateImageFrameBlockL(CImageFrame* aFrame);
	void ValidateFormatL(const TJpgFrameInfo& aFrameInfo, TUid aFormatCode);
	
	// INC037134 - free buffers used in codec
	virtual void CleanupBuffers();
	
	CJpgImageFrameReadCodec* ImageFrameCodec();
	virtual TInt ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const;
	void SetExtensionManager(CPluginExtensionManager* aExtensionManager);
	static TInt ScaleFactorToDivisorL(TInt aScalingFactor);

	void SetAutoRotateFlag(TUint16 aAutoRotateFlag);

protected:
	void InitComponentsL();
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual TFrameState DoProcessL() = 0;
	void StoreState();
	void RestoreState();
	void ResetState();
	TInt RestartStateL();
	void GetComponentBlockL(TDataUnit& aDestination,TInt& aNumValues,TInt& aDCPrediction,const TDecHuffmanTable& aDCTable,const TDecHuffmanTable& aACTable);
	TInt GetHuffmanCodeL(const TDecHuffmanTable& aTable);
	inline void SkipBitsQuickL(TInt aNumOfBits);
	inline TInt GetBinaryNumberQuickL(TInt aLength);
	inline TInt16 GetBinaryNumberL(TInt aLength);
	inline TInt16 GetPositiveBinaryNumberL(TInt aLength);
	void CalculateRgbIndeces();					// prepares UV indeces for de-sampling process
	void ConfigureUVComponentDCT(TInt aCompIdx);// cofigures UV iDCT parameters according to the scaling factor
	void WriteMCU();
	void WriteMonoMCU();
	void CalculateRenderingParams(const TInt aScale);
	virtual void CalculateMCUBoundingRectL(TInt aMCUsPerLine);
	TInt GetMCURenderOffset();

	template<class T>
	inline void WriteDiv8MCUImpl(); //templatized implementation for writting MCU with 1/8 scaling factor
	
	template<class T>
	void WriteDiv4ScaledMCUImpl(); //templatized implementation for writting MCU with 1/4 scaling factor
	
	template<class T>
	void WriteDiv2ScaledMCUImpl(); //templatized implementation for writting MCU with 1/2 scaling factor
	
	template<class T>
	inline void WriteUnScaledMCUImpl(); //templatized implementation for writting MCU with 1/1 scaling factor

	void WriteUnScaledMCU();		// implementation for writing MCU with 1/1 scaling factor to TRgb output buffer
	void WriteDiv2ScaledMCU();		// implementation for writing MCU with 1/2 scaling factor to TRgb output buffer
	void WriteDiv4ScaledMCU();		// implementation for writing MCU with 1/4 scaling factor to TRgb output buffer
	void WriteDiv8ScaledMCU();		// implementation for writing MCU with 1/8 scaling factor to TRgb output buffer
	
	void WriteUnScaledMCU16M();		// implementation for writing MCU with 1/1 scaling factor to 24bpp output buffer
	void WriteDiv2ScaledMCU16M(); 	// implementation for writing MCU with 1/2 scaling factor to 24bpp output buffer
	void WriteDiv4ScaledMCU16M(); 	// implementation for writing MCU with 1/4 scaling factor to 24bpp output buffer
	void WriteDiv8ScaledMCU16M(); 	// implementation for writing MCU with 1/8 scaling factor to 24bpp output buffer
	
	void WriteDiv8ScaledMCU64K(); 	// implementation for writing MCU with 1/8 scaling factor to 16bpp output buffer
	
	TInt ComponentIndexL(TInt aComponentId) const;

	inline TBool NextBitL();					// returns value of next bit off the bit buffers
	inline TInt FillBitBufferL(TInt aBLeft);					// fetches up to 4 bytes into bit-buffer
	inline const TUint8* JpegFillBuffer(const TUint8* aDataPtr);				// detects safe number of bytes which can be read from input buffer
	virtual TInt MCUBlockPerRgbBuffer() const; 	// returns number of MCU block which make full horizontal line
	TInt GetHorzMCUCount();
	TInt GetVertMCUCount();
	void ConfigureAndAllocateBufferL(const TInt aScale);
	void ConfigureImageProcessorL(CImageProcessorExtension* aImageProc, CFbsBitmap& aFrame, const TInt aScaleDivisor, const TSize& aPrepareLSize);

protected:
	class TCompConf // configuration parameters for color component
		{
	public:
		TDCT* 					iDCT;			// (i)DCT implementation pointer
		TQTable::TDequantFunc	iDequantFunc;   // DeQuantization function pointer
		};
		
	TJpgFrameInfo iFrameInfo;
	TJpgScanInfo iScanInfo;

	const TUint8* iDataPtr;
	const TUint8* iDataPtrLimit;
// NOTE: there are multiple sections, which are created to achieve best CPU cache effectiveness
private://		
    typedef TUint TBitBuffer;
    
	const TUint8* iBitBufferPtrLimit;	// upper value for data buffer pointer which can be safely read without dealing with 0xFF
	TUint iDataValue;
	TUint iBitsLeft;
	
protected:		
	TInt iDCPredictor[KJpgNumberOfComponents];
	TCompConf iCompConf[KJpgNumberOfComponents];	// color component decoding implementation parameters
	
	TInt iMCUDataUnitCount[KJpgNumberOfComponents];
	TDataUnit* iComponent[KJpgNumberOfComponents];
	TBool iProcessing;
	TInt iScalingFactor;
	TInt iRestartMCUCount;
	TInt iHorzMCUBlkCount;	// current MCU number within the horizontal line
	TInt iHorzSampleFactor[KJpgNumberOfComponents];
	TInt iVertSampleFactor[KJpgNumberOfComponents];
	TInt iHorzSampleRatio[KJpgNumberOfComponents];
	TInt iVertSampleRatio[KJpgNumberOfComponents];
	TInt iMaxHorzSampleFactor;
	TInt iMaxVertSampleFactor;
	TInt iHorzSampleRatioSh[2];	// "shift" factor for calculating U/V horizontal pixel corrdinate during de-sampling
	TInt iVertSampleRatioSh[2];	// "shift" factor for calculating U/V vertical pixel corrdinate during de-sampling
	
private:	
	const TUint8* iInitialDataPtr;
	TBitBuffer iInitialDataValue;
	TUint iInitialBitsLeft;
	TInt iInitialDCPredictor[KJpgNumberOfComponents];
	typedef TUint8 TUVidxElemType;
	TInt iRgbBufNextLineOffs; // pre-calculated data offset that can be used to step to next MCU address within the iRgbBuffer
	TUVidxElemType iUVIndeces[2* KJpgMaxSampleFactor * KJpgMaxSampleFactor * KJpgDCTBlockWidth];
		// iUVIndeces are used to speed up MCU de-sampling
protected:
	TRgb* iRgbBuffer;						// RGB output buffer
	TBool iMonochrome;
	TBool iYuvDecode;
	TInt iP1;
	TInt iM1;
	TInt iPreviousDataLeft;					// used to detect if the last available data block is being processed
	CRawColorProcessor* iRawColorProcessor;	// Fast and simple Color Processor, not owned
	// Jpg codec support for Image Frame
	CJpgImageFrameReadCodec* iImageFrameCodecPtr;	
	TBool iProgressive;				// TRUE if we're dealing with progressively-encoded image
	TInt iBlocksPerLine;			// number of MCU blocks per image horizontal line

private:	
	TBool iHighSpeedMode;			// flag of "fast possbile decoding mode"
	TDCT iFullDCT;					// implemetation for the (Inverse) Discrete Cosine Transformer
	THalfDCT iHalfDCT;				// implemetation for the 1/2-sized (Inverse) Discrete Cosine Transformer
	TFastHalfDCT iFastHalfDCT;		// implemetation for the "fast" 1/2-sized (Inverse) Discrete Cosine Transformer
	TQuarterDCT iQuarterDCT; 		// implemetation for the 1/4-sized (Inverse) Discrete Cosine Transformer
	TFastQuarterDCT iFastQuarterDCT;// implemetation for the "fast" 1/4-sized (Inverse) Discrete Cosine Transformer
	TEighthDCT iEighthDCT;			// implemetation for the 1/8-sized (Inverse) Discrete Cosine Transformer

	typedef void (CJpgReadCodec::*TMCUWriteFunc)();
	TMCUWriteFunc iMcuWriteFunc;	// function pointer for the MCU to Bitmap writer implementation

protected:
	TSize iBufSize;		// The size, in pixels, of the intermediate buffer.
	TInt iPixelSize;
	TInt iCurrentMCUBitOffset;
	TBool iUseClipRect;	// Set if SetClippingRectL has been called.
	TRect iMCUClipRect;	// The clipping rect represented in MCU indeces.
	TInt iPixelIncrement;
	TInt iFirstPixelOffset;
	CPluginExtensionManager* iExtensionManager; // not owned
	TTransformOptions iOperation;
	TBool iFillBufferBackwards;	// How to fill iRgbBuffer.
	TInt iMCUHorizExtent;	// Flipping-aware pixel width of an MCU.

	//streaming information 
    class TStreamConfig
        {
    public:
    	TInt iSeqPosition;	//sequence postion of MCU
		TInt iNumBlocksToGet;	//number of blocks to decode
		TInt* iNumBlocksRead;	//number of blocks decoded
		TBool* iHaveMoreBlocks;	//are more blocks available to decode
        };
    
	TStreamConfig iStreamDecodeConfig;
	TBool iIsBlockStreaming;
	TDecodeStreamCaps::TNavigation iNavigation;
	TUid iStreamFormat;
	
	// These are members of this class as it greatly helps
	// debugging if you know what MCUs you're dealing with.
	TInt iNeededMCU;	// The MCU number that's next needed for rendering.
	TInt iStreamMCU;	// The MCU that the bit stream is currently representing.
	TBool iSeekDone;	// Is DoProcessL being called after a reposition request.
	TBool iEscapeAtEnd;	// Set if the 0xFF00 sequence occurs at the buffer boundary.
	
	TInt iMCUsPerBuffer;
	TSize iRubbish;		// The amount of 'pixels' of an MCU that aren't part of the final image.
	TUint16 iAutoRotateFlag;
	};

//
// Utility class for finding out what
// MCUs are needed to render a clipping rect.
//
class CMCUStore : public CBase
	{
public:
	static CMCUStore* NewL(TJpgFrameInfo& aFrameInfo);
	~CMCUStore();
	void SetClippingRect(const TInt aFirstMCU, const TInt aTotalMCUs);
	void SetOperation(const TTransformOptions aOperation);
	void SetMCUsPerBuffer(const TInt aMCUsPerBuffer);
	TInt GetNextMCU();
	void NextLine();
	void Reset();
		
private:
	CMCUStore(TJpgFrameInfo& aFrameInfo);
	void Prepare();

	TBool iClipped;
	TJpgFrameInfo& iFrameInfo;
	TInt iCurrentMCU;
	TTransformOptions iOperation;
	TInt iNextRowOffset;
	TInt iMCUIncrement;
	TInt iLastMCU;
	TInt iReads;
	TInt iMaxReads;
	TInt iMCUsPerBuffer;
	TBool iPrepared;
	};

//
// CSequentialJpgReadCodec
//
class CSequentialJpgReadCodec : public CJpgReadCodec
	{

	//
	// This is the structure of a lookup-table entry.
	// 
	typedef struct
		{
		TUint32 iPosition;			// The bit position of the MCU.
		TInt16 iDCPredictor[3];		// The DC predictors (needed for correct rendering).
		TInt16 iRestartMCUCount;	// Images with restart intervals will require this.
		}
	TMCUEntry;
	
public:
	static CSequentialJpgReadCodec* NewL(
			const TJpgFrameInfo& aFrameInfo,
			const TJpgScanInfo& aScanInfo,
			TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
			TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
			const TQTable aQTable[KJpgMaxNumberOfTables]
			);
	~CSequentialJpgReadCodec();
	virtual void PreInitFrameL();
	virtual void PostInitFrameL();
	virtual void GetBlocksL(CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);

protected:
#ifdef JPEG_DEBUG_OUTPUT
	void DumpCache();
#endif
	
	virtual TFrameState DoProcessL();
	void CalculateMCUBoundingRectL(TInt aMCUsPerLine);
	void ConstructL(TBool aUseCache = ETrue);
	TInt GetNextMCUPosition();
	void PostProcessMCUL(TBool aForceBufferFlush);
	TInt ProcessMCUL();
    TFrameState ProcessStreaming();
	
private:
	// Functions related to caching and seeking.
	void CacheMCULocation();
	virtual void GetNewDataPosition(TInt& aPosition, TInt& aLength);
	TBool QueryCache();
	void RestoreAfterSeekL();

	
protected:
	CSequentialJpgReadCodec(
			const TJpgFrameInfo& aFrameInfo,
			const TJpgScanInfo& aScanInfo,
			TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
			TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
			const TQTable aQTable[KJpgMaxNumberOfTables]
			);
	
	TDecHuffmanTable* iDCHuffmanTable;
	TDecHuffmanTable* iACHuffmanTable;
	const TQTable* iQTable;
	
	// A pointer to the lookup table.
	TMCUEntry* iMCULookup;
	
	// The number of MCUs in the source image.
	TInt iTotalMCUCount;

	// Keeps track of what MCUs are needed for rendering.
	CMCUStore* iMCUStore;
	};

	
//
// CMultiScanSequentialJpgReadCodec
// =================================
//
class CMultiScanSequentialJpgReadCodec : public CSequentialJpgReadCodec
	{
public:
	static CMultiScanSequentialJpgReadCodec* NewL(
			const TJpgFrameInfo& aFrameInfo,
			const TJpgScanInfo& aScanInfo,
			TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
			TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
			const TQTable aQTable[KJpgMaxNumberOfTables]);
	virtual void PreInitFrameL();
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* aFrameMask);
	~CMultiScanSequentialJpgReadCodec();
	
private:
	virtual TFrameState DoProcessL();
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	TBool SearchTableOrSosMarker(TBufPtr8& aSourceData, TInt& aPos, TUint16& aMarker);
	TBool SwitchScanL(const TUint8*& aScan, const TUint8* aDataLimit);		
	void RenderMCUsL();
	CMultiScanSequentialJpgReadCodec(
			const TJpgFrameInfo& aFrameInfo,
			const TJpgScanInfo& aScanInfo,
			TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
			TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
			const TQTable aQTable[KJpgMaxNumberOfTables]);
	void CopyMCUs(TInt aCompIdx);
	void FillEmptyMCUs();
	void FillEmptyDU(TDataUnit* pDU);
	void ResetOnNewScan();
	TBool ProcessHuffmanTableL(const TUint8*& aData, const TUint8* aDataLimit);
	void PrepareToRenderMCU(TInt aMCUIndex);

private:
	TInt iTotalMCUBlocks;
	TDataUnit* iCompBuf[KJpgNumberOfComponents];
	TInt iCurrentMcuIdx;
	TBool iNewTableOrScan;
	RChunk iMCUChunk;
	TUint8* iMCUMemoryOffset;
	//available components
	TBool iCompAvailable[KJpgNumberOfComponents];
	//count of MCUs decoded
	TInt iCompMcuCount[KJpgNumberOfComponents];
	//copy of pointer held by iComponents; this value is overwritten in rendering.
	TDataUnit* iComponentCpy[KJpgNumberOfComponents];
	
	TJpgScanInfo iFirstScan;
	};


//
// CProgressiveJpgReadCodec
// =========================
//
class CProgressiveJpgReadCodec : public CJpgReadCodec
	{
public:
	static CProgressiveJpgReadCodec* NewL(const TJpgFrameInfo& aFrameInfo,const TJpgScanInfo& aScanInfo,const TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],const TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],const TQTable aQTable[KJpgMaxNumberOfTables]);
	
	virtual void PreInitFrameL();
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* aFrameMask);
	virtual ~CProgressiveJpgReadCodec();

	//From CJpgReadCodec
	void InitDrawFrame();
	TBool DrawFrameL();
	void InitFrameL(CImageFrame& aFrame);

	// INC037134 - free buffers used in codec
	virtual void CleanupBuffers();

private:
	CProgressiveJpgReadCodec(const TJpgFrameInfo& aFrameInfo,const TJpgScanInfo& aScanInfo,const TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],const TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],const TQTable aQTable[KJpgMaxNumberOfTables]);
	virtual TFrameState DoProcessL();
	void ProcessFrameL();
	void InitDCL();
	void RefineDCL();
	void InitACL();
	void RefineACL();
	void GetDCValueL(TDataUnit& aDataUnit,const TDecHuffmanTable& aHuffmanTable,TInt& aDCPredictor);
	void SkipDCValueL(const TDecHuffmanTable& aHuffmanTable);
	TInt GetACValuesL(TDataUnit& aDataUnit,const TDecHuffmanTable& aHuffmanTable);
	void RefineACValuesL(TDataUnit& aDataUnit,const TDecHuffmanTable& aHuffmanTable);
	void LoadHuffmanTableL();
	void LoadSOSL();
	void LoadRestartIntervalL();
	void DoInitFrameL();
private:
	const TJpgFrameInfo iOriginalFrameInfo;
	const TJpgScanInfo iOriginalScanInfo;

	TDecHuffmanTable iDCHuffmanTable[KJpgMaxNumberOfTables];
	TDecHuffmanTable iACHuffmanTable[KJpgMaxNumberOfTables];
	TQTable iQTable[KJpgMaxNumberOfTables];

	TInt iHorzMCUCount;
	TInt iVertMCUCount;
	TInt iTotalMCUCount;
	TInt iCurrentMCUCount;
	TInt iCurrentMCUHorzCount;
	TInt iCurrentMCUVertCount;
	TInt iSkipCount;

	TInt16 iRefinedDCValue;
	TInt iEobRun;

	TDataUnit* iMCUBuffer[KJpgNumberOfComponents];
	TDataUnit* iMCUBufferPtr[KJpgNumberOfComponents];
	TDataUnit* iMCUBufferPtrLimit[KJpgNumberOfComponents];
	
	TUint8* iMCUMemoryBuffer;
	RChunk iMCUChunk;
	TBool iMCUChunkAllocated;

	TInt iIndividualHorzMCUCount[KJpgNumberOfComponents];
	TInt iIndividualVertMCUCount[KJpgNumberOfComponents];

	//used only by InitDrawFrame()/DrawFrame()
	TInt iCurrentDrawMCU;
	TDataUnit* iDrawMCUPtr[KJpgNumberOfComponents];
	};

#endif // JPEGCODEC_H

