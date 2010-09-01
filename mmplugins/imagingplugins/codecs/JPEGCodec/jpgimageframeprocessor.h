// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalTechnology 
*/
#ifndef  JPGIMAGEFRAMEPROCESSOR_H
#define  JPGIMAGEFRAMEPROCESSOR_H

#include <e32base.h>
#include <imageframe.h>
#include "JpegTypes.h"

/** blocks in each supported YCbCrformat */
const TInt KUnitsYCbCrMono = 1;
const TInt KUnitsYCbCr422  = 4;
const TInt KUnitsYCbCr420  = 6;

/**
Base abstract class for ImageFrameProcessors. It covers most of the functionality, used by the 
specific implementations. It is created using a specific CImageFrame format and key parameters 
like format and sampling should not change.

Clients use WriteBlockL() to store data into the CImageFrame object and ReadBlockL() to retrieve it.
Each implementation is expected to provide format specific implementation of these functions.
All storage and retrieval are based on a single Jpeg MCU block and these differ with 
respect to the specific format.

The processor relies on internal member which is updated to keep the next position. The current position 
is obtained/set by the relevant accessors.
*/
class CJpgImageFrameProcessor : public CBase
	{
	
public:
	CImageFrame& ImageFrame() const; 
	void GetCurrentPosition(TPoint& aPosition) const;
	void SetCurrentPosition(const TPoint& aPosition);
	virtual TDataUnit* ReadBlockL() = 0;
	virtual void WriteBlockL(const RArray<const TDataUnit*>& aDataUnit) = 0;
	TBool MoreData();   
	virtual ~CJpgImageFrameProcessor();   
    
protected:	
	CJpgImageFrameProcessor(CImageFrame& aFrame);
	void ValidateCurrentBlockL();
	void CalculateCorrections();
	void CalculateNextBlockStart(TInt aScanLine);

	static void DoWriteBlock(TUint8* aDest, const TDataUnit& aSrc, TInt aScanline, TInt aXComp, TInt aYComp);
	static void DoReadBlock(TDataUnit& aDest, const TUint8* aSrc, TInt aScanline, TInt aXComp, TInt aYComp);

    static TInt8 Clip(TInt16 aData);
    
	virtual void UpdateCurrentLengthL() = 0;
	void ConstructL(const TFrameLayoutBase& aLayout);
	
protected:
	CImageFrame* iFrame;
	TFrameLayout* iLayout;	
	TPoint iCurrentPosition;
	TInt iMaxHorzPlaneFactor;
	TInt iMaxVertPlaneFactor;
	TInt iOffsets[KMaxPlanesInFrame];
	const TInt* iHorzSampleRatio;
	const TInt* iVertSampleRatio;
	TInt iWCorrection; // width correction
	TInt iHCorrection; // height correction
	TSize iBlockSize; 
	TSize iMaxPlaneSize;
	TInt iIndexY;
	TInt iIndexU;
	TInt iIndexV;
	};

/**
CJpgImageFrameYUVMonoProcessor is used to process CImageFrame storing 
YCbCr monochrome data only. Format is KFormatYUVMonochromeUidValue. 
*/	
NONSHARABLE_CLASS( CJpgImageFrameYUVMonoProcessor ): public CJpgImageFrameProcessor
	{
friend class CJpgImageFrameProcessorUtility;

private:
	explicit CJpgImageFrameYUVMonoProcessor(CImageFrame& aFrame);
	virtual TDataUnit*  ReadBlockL();
	virtual void WriteBlockL(const RArray<const TDataUnit*>& aDataUnits);
	virtual void UpdateCurrentLengthL();   
	  
private:
	TDataUnit iDataUnit[KUnitsYCbCrMono];	
	};

/**
CJpgImageFrameYUV422InterleavedProcessor is used to process CImageFrame storing 
YCbCr 422 interleaved data only. Format is KFormatYUV422InterleavedUidValue.
*/		
NONSHARABLE_CLASS( CJpgImageFrameYUV422InterleavedProcessor ): public CJpgImageFrameProcessor
	{
friend class CJpgImageFrameProcessorUtility;

private:
	explicit CJpgImageFrameYUV422InterleavedProcessor(CImageFrame& aFrame);
	virtual TDataUnit* ReadBlockL();
	virtual void WriteBlockL(const RArray<const TDataUnit*>& aDataUnits);
	virtual void UpdateCurrentLengthL();  
	 	
private:
	TDataUnit iDataUnit[KUnitsYCbCr422];
	};
		
/**
Class CJpgImageFrameYUV420PlanarProcessor is a specific processor processing only specific 
CImageFrame objects where format is either KFormatYUV420PlanarUidValue or 
KFormatYUV420PlanarReversedUidValue.
*/		
NONSHARABLE_CLASS( CJpgImageFrameYUV420PlanarProcessor ): public CJpgImageFrameProcessor
	{
friend class CJpgImageFrameProcessorUtility;

private:
	explicit CJpgImageFrameYUV420PlanarProcessor(CImageFrame& aFrame);
	virtual TDataUnit* ReadBlockL();
	virtual void WriteBlockL(const RArray<const TDataUnit*>& aDataUnits);
	virtual void UpdateCurrentLengthL();  

private: 
	TDataUnit iDataUnit[KUnitsYCbCr420];
	};	

/**
Utility class used to create specific image processors based on the 
frame format code. It also is used to prepare image frame using jpgframeinfo 
and to validate imageframes or imageframedata objects.
It is used to recommend sufficient buffer sizes.  
*/			
class CJpgImageFrameProcessorUtility : public CBase
	{
public:
	static CJpgImageFrameProcessor* NewL(CImageFrame& aFrame); 
	static void PrepareImageFrameL(const TJpgFrameInfo& aFrameInfo, CImageFrame& aFrame);
	static void ValidateImageFrameL(CImageFrame& aFrame, TBool aFullFrame);
	static void ValidateFrameImageDataL(const TJpegImageData::TColorSampling aSampleScheme, CImageFrame& aSource);
	static TInt RecommendedBufferSizeL(const TJpgFrameInfo& aFrameInfo, TUid formatCode);
	static TInt RecommendedStreamBufferSizeL(const TJpgFrameInfo& aFrameInfo, TUid aFormatCode, TSize& aBlockSizeInPixels, TInt aNumBlocks);
    static TBool IsRecognisedFormatType(CImageFrame& aFrame);
    static TBool IsRecognisedLayoutType(CImageFrame& aFrame);
	};	

FORCEDINLINE TInt8 CJpgImageFrameProcessor::Clip(TInt16 aData)
	{
  	// if in the range 0:255 return data, otherwise 255 if larger or 0 if negative
   	return  (!(aData & 0xFF00)) ? aData : (aData & 0x8000) ? 0: 0x00FF;	
	};
		
#endif // JPGIMAGEFRAMEPROCESSOR_H
