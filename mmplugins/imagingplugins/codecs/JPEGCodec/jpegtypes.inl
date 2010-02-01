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

#if defined(__ARMCC__)
// use ARM instruction for performance-critical code
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif

inline 
void TYCbCr::YCbCrtoRawRGBInl(TInt aY,TInt aCb,TInt aCr, TRgb* aPtr)
	{
	aCb -= 128;
	
	TUint8* ptr = reinterpret_cast<TUint8*>(aPtr);
	ptr[0]=ClipColorComponent(aY + ((KBlueCbFactor * aCb) >> 16) );
	
	aCr -= 128;
	
	ptr[1]=ClipColorComponent(aY - ((KGreenCbFactor * aCb + KGreenCrFactor * aCr) >> 16));
	
	ptr[2]=ClipColorComponent( aY + ((KRedCrFactor * aCr) >> 16) );
	}
/**
    convert RGB to YUV colorspace
    Y = (R * 0.299) + (G * 0.587) + (B * 0.114)
    U = (R * 0.1687) + (G * 0.3313) + (B * 0.5)
    V = (R * 0.5) + (G * 0.4187) + (B * 0.0813)
    Scaled fixed-point multiplication is used e.g. (G * 0.587) == ((G << 16) * (0.587 << 16)) >> 16
*/
inline
void TYCbCr::RGBtoYCbCr(TDataUnit::TDataUnitElemType* aYPtr,TDataUnit::TDataUnitElemType* aCb,TDataUnit::TDataUnitElemType* aCr,CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{
	TInt red 	= CRgbBufferPtr::Red(aRgbBuffer);
	TInt green 	= CRgbBufferPtr::Green(aRgbBuffer);
	TInt blue 	= CRgbBufferPtr::Blue(aRgbBuffer);

	*aYPtr  = TInt16( TUint( (KYRedFactor * red) + (KYGreenFactor * green) + (KYBlueFactor * blue) ) >> 16 );
	*aCb	= TInt16( 128 + (((blue << 15) - (KCbRedFactor * red) - (KCbGreenFactor * green)) >> 16) );
	*aCr    = TInt16( 128 + (((red << 15) - (KCrGreenFactor * green) - (KCrBlueFactor * blue)) >> 16) );
	}
	
/*static */ // !!! This function is faster then ColorConverter::ClampColorComponent. DO NOT REMOVE IT !!!
FORCEDINLINE TInt TYCbCr::ClipColorComponent(TInt aValue)
    {
    if (TUint(aValue) > 0xFF)
        {
        aValue = aValue < 0 ? 0 : 0xFF;
        }
    return aValue;
    }

inline
THuffmanTable::THuffmanState THuffmanTable::State() const
	{
	return iState;
	}

inline
void THuffmanTable::SetState(THuffmanState aState)
	{
	iState = aState;
	}
	
template <TInt aTableSize> FORCEDINLINE TInt TEncHuffmanTable<aTableSize>::GetCode(TInt& aSize, const TInt aValue) const
	{
	ASSERT( aValue < aTableSize );
	register TUint32 tmp = iLookupTable[ aValue ];
	aSize = tmp & 0xff;
	return (tmp >> 16);
	}

FORCEDINLINE TUint32 TDecHuffmanTable::GetLookupEntry(TUint32 aFastLook) const
	{
	return iLookupTable[aFastLook];
	}

FORCEDINLINE TBool TDecHuffmanTable::Found(const TUint32 aFastLookRes) const
	{
	return (aFastLookRes > KMaxTUint8);
	}

FORCEDINLINE TInt TDecHuffmanTable::GetSize(const TUint32 aFastLookRes) const
	{
	return (aFastLookRes >> 8);
	}

FORCEDINLINE TInt TDecHuffmanTable::GetCode(const TUint32 aFastLookRes) const
	{
	return (aFastLookRes & 0xff);
	}
	
FORCEDINLINE TInt TDecHuffmanTable::GetIndex(TInt aPos) const
	{
	return iIndex[aPos];
	}

FORCEDINLINE TInt TDecHuffmanTable::GetIndexedCode(TInt aIndex) const
	{
	return iValue[aIndex];
	}

FORCEDINLINE const TUint8* TDecHuffmanTable::GetCodeIdxHash() const
	{
	return &iBitsToCodeIdxHash[0];
	}

// Helper fns.
inline TUint16 ReadBigEndianUint16(const TUint8*& aPtr)
	{
	TUint16 value = TUint16(aPtr[0] << 8 | aPtr[1]);
	aPtr += 2;
	return value;
	}

inline void WriteBigEndianUint16(TUint8*& aPtr,TInt aUint16)
	{
	aPtr[0] = TUint8(aUint16 >> 8);
	aPtr[1] = TUint8(aUint16);
	aPtr += 2;
	}

inline 
void FillCompZ(TDataUnit& aDestination, TInt aNumValues)
	{
	TUint32* fillPtr=reinterpret_cast<TUint32*>(aDestination.iCoeff);
	TUint32* const fillPtrLimit=fillPtr + (aNumValues >> 1);
	TUint64 w = 0;
	do 
		{
		*( reinterpret_cast<TUint64*>(fillPtr) )= w;
		fillPtr +=2;
		*( reinterpret_cast<TUint64*>(fillPtr) )= w;
		fillPtr +=2;
		} while (fillPtr < fillPtrLimit);	
	}

// TQTable
inline
TQTable::TQTable():
	iSixteenBitPrecision(EFalse),
	iQualityFactor(0),
	iFastQuantEnabled(ETrue)
	{}

inline
void TQTable::SetAllowFastQuant(TBool aEnable)
    {
    // but we won't do it for 16 bit tables    
    iFastQuantEnabled = aEnable;
    }

inline
void TQTable::Fast18DeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt /*aNumNonZeroValues*/) const
	{
	const TInt16* srcPtr = aSource.iCoeff;
	TInt16* dstPtr = aDestination.iCoeff;
	
	if (iSixteenBitPrecision)
		{
		const TUint16* qPtr = (const TUint16*)iValues;
		dstPtr[0] = TInt16(srcPtr[0] * qPtr[0]);
		}
	else
		{
		const TUint8* qPtr = iValues;
		dstPtr[0] = TInt16(srcPtr[0] * qPtr[0]);
		}		
	}

inline
TInt TQTable::QualityFactor() const
	{
	return iQualityFactor;
	}


// TDCT
inline
TDCT::TDCT():
	iLevelShift( KDefPrecShift )
	{}

inline
void TDCT::SetPrecision(TInt aPrecision)
	{
	iLevelShift = (aPrecision == KPrecision12Bit) ? KPrec12Shift : KDefPrecShift;
	}

FORCEDINLINE void TEighthDCT::Fast18InvTransform(TDataUnit& aDestination, TInt aDCElem) const
	{
	aDestination.iCoeff[0]  = TInt16(DESCALE(aDCElem, 3) + iLevelShift);
	}

#if defined(__ARMCC__)
#pragma pop
#endif
