// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef JPEGWRITECODEC_H

#define JPEGWRITECODEC_H

/**
   @file
   @internalComponent
*/
//#define PROFILE_JPEG_ENCODER 1
#include "imgpositionprocessor.h"
#include "JpegTypes.h"

class TPositionProcessor;
class CRgbBufferPtr;
class CImageFrame;
class CJpgImageFrameWriteCodec;
class CJpgImageFrameProcessor;
class TDataUnit;
class TQTable;


const TInt KEncDCHTSize = 11;
const TInt KEncACHTSize = 0xFA;

// CJpgWriteCodec
class CJpgWriteCodec : public CImageWriteCodec
	{
public:

	static CJpgWriteCodec* NewL(const TJpgFrameInfo& aFrameInfo,TInt aQualityFactor,TQTable* aReplacementLumaQTable,TQTable* aReplacementChromaQTable,const RPointerArray<HBufC8>& aComment,RArray<TUint>* aOperationsRequested);
	
	virtual ~CJpgWriteCodec();

	// Jpg codec support for Image Frame
	void InitFrameL(TBufPtr8& aDst, const CImageFrame* aFrame, const CFrameImageData* aFrameImageData);

	// Jpg codec support for Image Frame streaming
	void InitFrameL(TBufPtr8& aDst, TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, const CFrameImageData* aFrameImageData);
	void AppendFrameBlockL(const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void CompleteFrame();
	void SetCompleteFrame();
	
	static void InitTransformCoordinates(TPositionProcessor& aPosProcessor, const TRect& aSourceRect, const TSize& aMCUSize, const RArray<TUint>& aOperationsRequested);
	static void ValidateBlockSizeL(const TSize& aBlockSizeInPixels, const TSize& aRefSizeInPixels);
	void SetHighSpeedMode(TBool aHighSpeedMode);
protected:
	CJpgWriteCodec(const TJpgFrameInfo& aFrameInfo,TInt aQualityFactor,TQTable* aReplacementLumaQTable,TQTable* aReplacementChromaQTable,const RPointerArray<HBufC8>& aComment);
	void ConstructL(RArray<TUint>* aOperationsRequested);
	
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	void CodecInfoL();
	void WriteInfoL();
	void DoProcessL();
	void InitCompConfig();
	CRgbBufferPtr::TConstRgbBufferPtr GetPixels(const TPoint& aPos);
	void WriteDataUnitL(const TDataUnit& aDataUnit,const TEncHuffmanTable<KEncDCHTSize+1>& aDCTable,const TEncHuffmanTable<KEncACHTSize+1>& aACTable,TInt& aDCPredictor);
	inline TUint32 WriteBitsFast(TUint32 aValue, TInt aNumBits, TUint32& aBitBuffer, TInt aBitsUsed);
	void WriteBits(TUint32 aBitBuffer);
	
	void InitTransformationL(TPositionProcessor& aPosProcessor, const TRect& aSourceRect, const TSize& aMCUSize, TJpgFrameInfo& aFrameInfo, const TInt aDataUnitCount);
	void InitTransformDataUnitIndex(TInt aDataUnitCount);
	void InitTransformDataUnitCoeff(TUint8* aDestination);
	void TransformDataUnitCoeff(TDataUnit& aDestination, const TDataUnit& aSource);
	TInt GetIndexFromPos(const TPoint& aPos);
	
private:
    typedef TUint32 TBitBufferType;
	TUint8* iDestPtr;
	TUint8* iDestPtrLimit;
	TInt iBitsUsed;
	//Must be TUint32
	TUint32 iBitBuffer;

protected:
	TInt iDCPredictor[KJpgNumberOfComponents];
	
	TRect iSourceRect;
	TPoint iPos;

	TInt iQualityFactor;
	TEncHuffmanTable<KEncDCHTSize+1> iLumaDCHuffmanTable;
	TEncHuffmanTable<KEncDCHTSize+1> iChromaDCHuffmanTable;
	TEncHuffmanTable<KEncACHTSize+1> iLumaACHuffmanTable;
	TEncHuffmanTable<KEncACHTSize+1> iChromaACHuffmanTable;
	TQTable iLumaQTable;
	TQTable iChromaQTable;
	TQTable* iReplacementLumaQTable;
	TQTable* iReplacementChromaQTable;
	TDCT iDCT;	

	TSize iMCUSizeInPixels;
	
	TDataUnitProcessor* iDataUnitProcessor;
	TInt iDataUnitCount;
	
	// Jpg codec support for Image Frame
	CJpgImageFrameWriteCodec* iImageFrameCodecPtr;	
	
	// Jpg support for rotate and flip encode
	TPositionProcessor iPosProcessor;
	TUint8 iTransformedDUIdx[KJpgMaxNumOfDataUnits];
	RArray<TUint> iOperationsRequested;
	TUint8 iTransformDUCoeffIdx[KJpgDCTBlockSize];
	
	// Jpg codec support for Image Frame streaming
	TBool iIsBlockStreaming;
    class TStreamConfig
        {
    public:
		TInt iNumOfBlocksToAppend;
		TInt iNumOfBlocksAppended;
		TInt iNumOfMCUsProcessed;
		TBool iIsFrameComplete;
        };
	TStreamConfig iStreamConfig;
		
    class TSpecialDataUnit: public TDataUnit
        {
    public:
        TDataUnit::TDataUnitElemType iExtraElement;
        };
    
    class TComponentConfig
        {
    public:
        TEncHuffmanTable<KEncACHTSize+1>*  iACTable;
        TEncHuffmanTable<KEncDCHTSize+1>*  iDCTable;
        TQTable*        iQTTable;
        TInt            iDataUnitsCount;
        TInt            iDCPredictorIdx;
        TInt            iFirstDUIdx;
        };
    
	TComponentConfig iCompConfig[ KJpgNumberOfComponents ];
	TSpecialDataUnit iQuantizeTarget[ KJpgMaxNumOfDataUnits ];
	TInt iWrittingDUIdx;
	
	CRgbBufferPtr* iRgbInputBuffer;
	TJpgFrameInfo iFrameInfo;
	const RPointerArray<HBufC8> &iComment;
#if defined(PROFILE_JPEG_ENCODER)
	TUint iGetPixelsTime;
	TUint iDataUnitProcessTime;
	TUint iTransforTime;
	TUint iQuantizeTime;
	TUint iWriteDataUnitTime;
	TUint iImageFrameTime;
	TUint iOverallTime;
#endif	
	TBool iHighSpeedMode;			// flag for "fast encoding mode"	
	};


// CImageFrameWriteCodec
class CJpgImageFrameWriteCodec : public CBase
	{
public:
	static CJpgImageFrameWriteCodec* NewL(const CImageFrame* aFrame);
	~CJpgImageFrameWriteCodec();

	void CreateImageProcessorL();
	void PrepareFrameInfoL(TJpgFrameInfo& aFrameInfo, const CFrameImageData* aFrameImageData);
	void SetPosition(TPoint& aPos);
	void Position(TPoint& aPos);
	TDataUnit* ProcessL();
	TInt DataUnitCount() const;
	void InitFrameCompInfoL(TJpgFrameInfo& aFrameInfo);
	TJpegImageData::TColorSampling GetColorSamplingL(TUid aFormat);
	TUid GetSamplingSchemeL(TUid aFormat);
	
	// for streaming
	void PrepareFrameImageDataInfoL(TJpgFrameInfo& aFrameInfo, TUid aFormat, TInt aFrameNumber, const CFrameImageData* aFrameImageData);
	void AppendImageFrameBlockL(const CImageFrame* aFrame);	
	TSize ImageFrameSize() const;
	TUid SamplingScheme() const;
	
private:
	CJpgImageFrameWriteCodec(const CImageFrame* aFrame);

private:
	const CImageFrame* iSource;
	CJpgImageFrameProcessor* iImageFrameProcessorPtr;
	TInt iDataUnitCount;
	TUid iSampleScheme;
	};

#endif // ndef JPEGWRITECODEC_H

