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

#ifndef JPEGTYPES_H
#define JPEGTYPES_H

#include "ImageUtils.h"
#include "JpegConsts.h"
#include <icl/imagecodecdata.h>
#include "rgbbufferptr.h"

#define DESCALE(x,n)  ((x + (1 << (n - 1))) >> n)
// Helper classes.
union TAligned64Value
	{
	TUint64	iAligment;
	TInt	iInt[2];
	TInt16	iWord[4];
	TUint8	iByte[8];
	};

class TJpgFrameInfo
	{
public:
	enum TJpgImageUnits
		{
		ENone = 0,
		EDotsPerInch = 1,
		EDotsPerCm = 2
		};
		
	class TComponentInfo
		{
	public:
		TInt iId;
		TInt iHorzSampleFactor;
		TInt iVertSampleFactor;
		TInt iQTable;
		};
		
public:

	//
	// Returns the total number of MCUs in the image.
	//
	TInt TotalMCUCount()
		{
		TInt mcuHeight = MCUHeightInPixels();
		TInt cy = ((iSizeInPixels.iHeight + mcuHeight - 1) / mcuHeight);

		TInt mcuWidth = MCUWidthInPixels();
		TInt cx = ((iSizeInPixels.iWidth + mcuWidth - 1) / mcuWidth);
		
		iMCUBlocksPerLine = cx;
		iMCUBlocksPerColumn = cy;
		
		ASSERT(iMCUBlocksPerLine > 0);
		ASSERT(iMCUBlocksPerColumn > 0);
		
		return cx * cy;
		}
	
	//
	// Returns the width of an MCU in pixels.
	//
	FORCEDINLINE TInt MCUWidthInPixels() const
		{
		ASSERT(iMaxHorzSampleFactor > 0);
		return KJpgDCTBlockWidth * iMaxHorzSampleFactor;
		}
	
	//
	// Returns the height of an MCU in pixels.
	//
	FORCEDINLINE TInt MCUHeightInPixels() const
		{
		ASSERT(iMaxVertSampleFactor > 0);
		return KJpgDCTBlockWidth * iMaxVertSampleFactor;
		}
	
	
	TInt iSamplePrecision;
	TSize iSizeInPixels;
	TSize iSizeInTwips;
	TJpgImageUnits iUnits;
	TInt iNumberOfComponents;
	TComponentInfo iComponent[KJpgNumberOfComponents];
	TBool iProgressive;
	TBool iMultiScan;
	TInt iRestartInterval;
	TInt iMaxHorzSampleFactor;
	TInt iMaxVertSampleFactor;
	TSize iThumbnailSize;
	TInt iMCUBlocksPerLine;
	TInt iMCUBlocksPerColumn;
	};


class TJpgScanInfo
	{
public:
	class TScanComponentInfo
		{
	public:
		TInt iId;
		TInt iDCCodingTable;
		TInt iACCodingTable;
		};
public:
	TInt iNumberOfComponents;
	TScanComponentInfo iComponent[KJpgNumberOfComponents];
	TInt iStartSpectralSelection;
	TInt iEndSpectralSelection;
	TInt iSuccessiveApproximationBitsHigh;
	TInt iSuccessiveApproximationBitsLow;
	TInt iImageOffset;
	};




class THuffmanTable
	{
public:
	enum THuffmanState
		{
		EUnInitialised,
		EInitialised,
		EParsed
		};
	
	THuffmanTable();
	TInt SetL(const TUint8* aData, const TUint8* aDataLimit); // Sets up table but does not sort it - returns size (max is KJpgHuffmanTableBitsSize + KJpgHuffmanTableSize)
	TInt GetL(TUint8* aData); // Writes table data to buffer - returns size (max is KJpgHuffmanTableBitsSize + KJpgHuffmanTableSize)
	inline THuffmanState State() const;
	inline void SetState(THuffmanState aState);

private:
	friend class THuffmanCodeSwap;
	friend class THuffmanValueSwap;
	friend class THuffmanIndexKey;
	friend class THuffmanValueKey;

protected:
	
	class THuffmanCode
		{
	public:
		TInt	iIndex;
		TUint16 iCode;
		TUint8	iSize;
		TUint8  iValue;
		};
	
	void GenerateSizes(THuffmanCode* aCodeArray);
	void GenerateCodes(THuffmanCode* aCodeArray);
	void SortByCode(THuffmanCode* aCodeArray, TUint8* aBitsToCodeIdxHash);
	void SortByValue(THuffmanCode* aCodeArray);
		
protected:
	TUint8 iBits[KJpgHuffmanTableBitsSize];
	TUint8 iValues[KJpgHuffmanTableSize];
	TInt iLastCode;
	THuffmanState iState;	
	};

class TDecHuffmanTable : public THuffmanTable
	{
public:
	void MakeDerivedTableL();
	//lookup
	inline TUint32 GetLookupEntry(TUint32 aFastLook) const;
	inline TBool Found(const TUint32 aFastLookRes) const;
	inline TInt GetSize(const TUint32 aFastLookRes) const;
	inline TInt GetCode(const TUint32 aFastLookRes) const;
	//
	inline TInt GetIndex(TInt aPos) const;
	inline TInt GetIndexedCode(TInt aIndex) const;

	inline const TUint8* GetCodeIdxHash() const;
	
private:
	TUint16 iLookupTable[KJpgHuffmanTableSize];
	TUint16 iValue[KJpgHuffmanTableSize];
	TInt iIndex[KJpgHuffmanTableSize];

	TUint8 iBitsToCodeIdxHash[KJpgHuffmanMaxCodeSize+2];
	};

template <TInt aTableSize> class TEncHuffmanTable : public THuffmanTable
	{
public:
	void MakeDerivedTableL();
	inline TInt GetCode(TInt& aSize, const TInt aValue) const;
	
private:
	TUint32 iLookupTable[aTableSize];
	};

class THuffmanTableProcessor
	{
public:
	static void ProcessHuffmanTableL(const TUint8*& aDataPtr, const TUint8* aDataPtrLimit, TDecHuffmanTable* aDCTable, TDecHuffmanTable* aACTable);	
	};

class TJpgScanInfoProcessor
	{
public:	
	static void ProcessStartOfScanL(const TUint8*& aPtr, const TJpgFrameInfo& aJpgFrameInfo, TJpgScanInfo& aScanInfo, TDecHuffmanTable* aDCTable, TDecHuffmanTable* aACTable);	
	};
	


class TDataUnit
	{
public:
	typedef TInt16 TDataUnitElemType;	
	union
		{
		TUint64 iValAlignment;
		TDataUnitElemType iCoeff[KJpgDCTBlockSize];
		};
	};

class TQTable
	{
public:
	typedef void (TQTable::*TDequantFunc)(TDataUnit& ,const TDataUnit&, TInt ) const;
	inline TQTable();
	
	TInt Set(const TUint8* aData,const TBool aSixteenBitPrecision);
	TInt Get(TUint8* aData) const;
	void SetQualityFactor(TInt aQualityFactor);
	
	inline TInt QualityFactor() const;
	inline void SetAllowFastQuant(TBool aEnable);
	
	void Quantize(TDataUnit& aDestination,const TDataUnit& aSource, TBool aHighSpeedMode) const;
	
	void DeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt aNumNonZeroValues) const;
	void FastQuarterDeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt /*aNumNonZeroValues*/) const;
	void FastHalfDeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt aNumNonZeroValues) const;
	void Fast18DeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt /*aNumNonZeroValues*/) const;

private:
    template <class T> void DoQuantize(TDataUnit& aDestination,const TDataUnit& aSource) const;
	void CalcQualityFactor();
	void DoDeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt aNumNonZeroValues,
						TInt aBlockLen) const;
	
private:
	TBool iSixteenBitPrecision;
	TInt iQualityFactor;
	TBool iFastQuantEnabled;
	
	union 
	    {
	    TUint64 iValues2Aligment;
	    TInt iValues2[KJpgQTableEntries];
	    };
	    
	union
	    {
	    TUint64 iValuesAligment;
	    TUint8 iValues[KJpgQTableEntries * 2]; // Room for an eight- or sixteen-bit quantization table    
	    };
	};


class TDCT
	{
private:

    enum 
        {
        KDefaultPrecision   = 8,
        KDefPrecShift       = 1<<(KDefaultPrecision - 1),
        KPrecision12Bit     = 12,
        KPrec12Shift        = 1<<(KPrecision12Bit - 1)
        };

public:        
	inline TDCT();
	inline void SetPrecision(TInt aPrecision);
	void Transform(TDataUnit& aDestination,const TDataUnit& aSource, TBool aHighSpeedMode) const;
	virtual void InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const;
	
protected:
	TInt iLevelShift;

	TInt iDoubleLevelShift;	

private:

    union TAligned64Du
        {
        TUint64	iAligment;
        TInt	iData[KJpgDCTBlockSize];
        };

	union TAligned64TwoCoeff
        {
        TUint64	iAligment;
		TInt	iData[2];
        };
	
    void DoFirstStep(TAligned64Du& aDestination,const TDataUnit& aSource) const;
    void DoSecondStep(TDataUnit& aDestination,const TAligned64Du& aSource) const;
	void DoFastFirstStep(TAligned64Du& aDestination,const TDataUnit& aSource) const;
    void DoFastSecondStep(TDataUnit& aDestination,const TAligned64Du& aSource) const;
	};

class THalfDCT : public TDCT
	{
public:
	virtual void InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const;
	};
	
class TFastHalfDCT : public TDCT
	{
public:
	virtual void InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const;
	};

class TQuarterDCT : public TDCT
	{
public:
	virtual void InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const;
	};
	
class TFastQuarterDCT : public TDCT
	{
public:
	virtual void InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const;
	};

class TEighthDCT : public TDCT
	{
public:
	virtual void InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const;
	inline void Fast18InvTransform(TDataUnit& aDestination, TInt aDCElem) const;
	};


class TYCbCr
	{
public:
    enum 
        {
        KYRedFactor = 19595,    // 0.299 << 16
        KYGreenFactor = 38470,  // 0.587 << 16
        KYBlueFactor = 7471,    // 0.114 << 16
        KCbRedFactor = 11056,   // 0.1687 << 16
        KCbGreenFactor = 21712, // 0.3313 << 16
        KCrGreenFactor = 27440, // 0.4187 << 16
        KCrBlueFactor = 5328,   // 0.0813 << 16
        // for decoding
	    KRedCrFactor    = 91881, // 1.402 << 16
	    KGreenCbFactor  = 22554, // 0.34414 << 16
	    KGreenCrFactor  = 46802, // 0.71414 << 16
	    KBlueCbFactor   = 116130 // 1.772 << 16        
        };
        
	static TRgb YCbCrtoRGB(TInt aY,TInt aCb,TInt aCr);
	static void YCbCrtoRawRGB(TInt aY,TInt aCb,TInt aCr, TRgb* aPtr);
	static void YCbCrtoRaw64K(TInt aY,TInt aCb,TInt aCr, TRgb* aPtr);
	
	static inline void YCbCrtoRawRGBInl(TInt aY,TInt aCb,TInt aCr, TRgb* aPtr);
	
	static inline void RGBtoYCbCr(TDataUnit::TDataUnitElemType* aYPtr,TDataUnit::TDataUnitElemType* aCb,TDataUnit::TDataUnitElemType* aCr,CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
	static void RGBtoYCbCr2PixUVSum(TDataUnit::TDataUnitElemType* aYPtr,TDataUnit::TDataUnitElemType*, CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
    static void RGBtoYCbCr4PixUVSum(TDataUnit::TDataUnitElemType* aYPtr, TDataUnit::TDataUnitElemType* aCb, CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer, TInt aLineStride);
    
    inline static TInt ClipColorComponent(TInt aValue);
	};

// TDataUnitProcessor
class TDataUnitProcessor
	{
public:
	virtual TDataUnit* Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer) = 0;
	void SetBufferPtrUtil(CRgbBufferPtr& aBufferPtrUtil);
protected:
    CRgbBufferPtr* iBufferPtrUtil;
	};

class TMonoProcessor : public TDataUnitProcessor
	{
public:
	virtual TDataUnit* Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
private:
	TDataUnit iMCU;
	};

class T444Processor : public TDataUnitProcessor
	{
public:
	virtual TDataUnit* Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
private:
	TDataUnit iMCU[3];
	};

class T422Processor : public TDataUnitProcessor
	{
public:
	virtual TDataUnit* Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
private:
	void DoProcess(TInt16* aYPtr,TInt16* aCbPtr,CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
private:
	TDataUnit iMCU[4];
	};

class T420Processor : public TDataUnitProcessor
	{
public:
	virtual TDataUnit* Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
private:
	void DoProcess(TInt16* aYPtr,TInt16* aCbPtr,CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer);
private:
	TDataUnit iMCU[6];
	};

//A collection of jpeg helper functions
class TJpegUtilities
	{
public:
	static void CreateJfifHeader(TDes8& aBuffer);
	};


#include "jpegtypes.inl"

#endif // __JPEGTYPES_H__

