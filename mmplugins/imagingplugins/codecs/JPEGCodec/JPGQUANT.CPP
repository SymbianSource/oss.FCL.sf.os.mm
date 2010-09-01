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
//

#include <e32base.h>

#include "JpegTypes.h"

const TInt KShift = 14;

// TQTable
TInt TQTable::Set(const TUint8* aData,const TBool aSixteenBitPrecision)
	{
	iSixteenBitPrecision = aSixteenBitPrecision;

	if (iSixteenBitPrecision)
		{
		TUint16* sixteenBitPtr = reinterpret_cast<TUint16*>(iValues);
		const TUint16* sixteenBitPtrLimit = sixteenBitPtr + KJpgQTableEntries;
		while (sixteenBitPtr < sixteenBitPtrLimit)
		    {
		    *sixteenBitPtr++ = ReadBigEndianUint16(aData);
		    }
			
		return KJpgQTableEntries * 2;
		}

    Mem::Copy(iValues, aData, KJpgQTableEntries);
    Mem::FillZ(iValues + KJpgQTableEntries,KJpgQTableEntries);
	CalcQualityFactor();

	return KJpgQTableEntries;
	}

TInt TQTable::Get(TUint8* aData) const
	{
	const TInt valuesSize = KJpgQTableEntries * (iSixteenBitPrecision ? sizeof(TUint16) : sizeof(TUint8));

	if (iSixteenBitPrecision)
		{
		TUint16* sixteenBitPtr = (TUint16*)iValues;
		const TUint16* sixteenBitPtrLimit = sixteenBitPtr + KJpgQTableEntries;
		while (sixteenBitPtr < sixteenBitPtrLimit)
		    {
		    WriteBigEndianUint16(aData,*sixteenBitPtr++);
		    }
		}
	else
	    {
	    Mem::Copy(aData,iValues,valuesSize);
	    }

	return valuesSize;
	}

void TQTable::SetQualityFactor(TInt aQualityFactor)
	{
	TInt scaleFactor = 0;
	if (aQualityFactor < 50)
		{
		aQualityFactor = Max(aQualityFactor,1);
		scaleFactor = 5000 / aQualityFactor;
		}
	else
		{
		aQualityFactor = Min(aQualityFactor,100);
		scaleFactor = 200 - (aQualityFactor * 2);
		}

	if (iSixteenBitPrecision)
		{
		TUint16* qPtr = (TUint16*)iValues;
		const TUint16* qPtrLimit = qPtr + KJpgDCTBlockSize;
		while (qPtr < qPtrLimit)
			{
			TInt qValue = (qPtr[0] * scaleFactor + 50) / 100;
			if (qValue <= 0)
				qValue = 1;
			if (qValue > KMaxTInt16)
				qValue = KMaxTInt16;
			*qPtr++ = TUint16(qValue);
			}
		}
	else
		{
        TUint8* qPtr = iValues;
        const TUint8* qPtrLimit = qPtr + KJpgDCTBlockSize;

		TInt* qPtrM = iValues2;

		while (qPtr < qPtrLimit)
			{
			TInt qValue = (qPtr[0] * scaleFactor + 50) / 100;
			if (qValue <= 0)
			    {
			    qValue = 1;
			    }
			if (qValue > TInt(KMaxTUint8) )
			    {
			    qValue = KMaxTUint8;
			    }
            *qPtrM++ = TUint16( (1 << KShift) / qValue );
			*qPtr++ = TUint8(qValue);
			}
		}
	}

void TQTable::CalcQualityFactor() // Assumes the default tables have been used
	{
	TInt scaleFactor = 0;

	if (iSixteenBitPrecision)
		{
		TUint16* qPtr = (TUint16*)iValues;
		scaleFactor = qPtr[KJpgQTableEntries - 1];
		}
	else
	    {
	    scaleFactor = iValues[KJpgQTableEntries - 1];
	    }
		
	if (scaleFactor > 100)
	    {
	    iQualityFactor = 5000 / scaleFactor;
	    }
	else
	    {
	    iQualityFactor = 100 - (scaleFactor / 2);
	    }
	}

//
// this section contains many performance-critical code, so use ARM instruction set for it
//
#ifdef __ARMCC__
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif

template <class T>
inline
void TQTable::DoQuantize(TDataUnit& aDestination, const TDataUnit& aSource) const
    {
	const TInt16* srcPtr = aSource.iCoeff;
	TInt16* dstPtr = aDestination.iCoeff;

	const TUint8* zigZagPtr     = KZigZagSequence.iZigZag;
	const TUint8* zigZagPtrLimit= zigZagPtr + KJpgDCTBlockSize;
		
	register const T* qPtr = reinterpret_cast<const T*>(iValues);
	
	do 
        {
        register TInt qValue = *qPtr++;
        register TInt dstValue = srcPtr[*zigZagPtr++];
        if (dstValue > 0)
            {
            dstValue += (qValue >> 1);
            if (dstValue >= qValue)
                {
                dstValue /= qValue;
                }
            else
                {
                dstValue = 0;
                }
            }
        	
        else
            {
            dstValue -= (qValue >> 1);
            dstValue = -dstValue;
            if (dstValue >= qValue)
                {
                dstValue /= qValue;
                dstValue = -dstValue;
                }
            else
                {
                dstValue = 0;
                }
            }
        	
        *dstPtr++ = TInt16(dstValue);

        } while (zigZagPtr < zigZagPtrLimit);
    }
    
void TQTable::Quantize(TDataUnit& aDestination,const TDataUnit& aSource, TBool aHighSpeedMode) const
	{
	if (iSixteenBitPrecision)
		{
		DoQuantize<TUint16>(aDestination, aSource);
		}
	else
	    {
	    if (!aHighSpeedMode)
	        {
	        DoQuantize<TUint8>(aDestination, aSource);
	        }
	    else
	        {
            const TInt16* srcPtr = aSource.iCoeff;
            TUint64* dstPtr = reinterpret_cast<TUint64*>(aDestination.iCoeff);

            const TUint8* zigZagPtr     = KZigZagSequence.iZigZag;
            const TUint8* zigZagPtrLimit= zigZagPtr + KJpgDCTBlockSize;
                  
            const TUint64* qPtr = &iValues2Aligment;
            // we do 2 values per loop iteration
            do 
                {
                register TAligned64Value dstValue;
				register TAligned64Value qValue;
				register TAligned64Value zValue;
				dstValue.iAligment = 0; //to remove possible RVCT warning
				zValue.iAligment = *reinterpret_cast<const TUint64*>(zigZagPtr);
                
				qValue.iAligment = *qPtr++;                
                                
                dstValue.iWord[0] = TUint16(DESCALE(srcPtr[ zValue.iByte[0] ] * qValue.iInt[0], KShift));
                dstValue.iWord[1] = TUint16(DESCALE(srcPtr[ zValue.iByte[1] ] * qValue.iInt[1], KShift));

				qValue.iAligment = *qPtr++;
				
				dstValue.iWord[2] = TUint16(DESCALE(srcPtr[ zValue.iByte[2] ] * qValue.iInt[0], KShift));
                dstValue.iWord[3] = TUint16(DESCALE(srcPtr[ zValue.iByte[3] ] * qValue.iInt[1], KShift));

				*dstPtr++ = dstValue.iAligment;
                
				qValue.iAligment = *qPtr++;                
                                
                dstValue.iWord[0] = TUint16(DESCALE(srcPtr[ zValue.iByte[4] ] * qValue.iInt[0], KShift));
                dstValue.iWord[1] = TUint16(DESCALE(srcPtr[ zValue.iByte[5] ] * qValue.iInt[1], KShift));

				qValue.iAligment = *qPtr++;
				
				dstValue.iWord[2] = TUint16(DESCALE(srcPtr[ zValue.iByte[6] ] * qValue.iInt[0], KShift));
                dstValue.iWord[3] = TUint16(DESCALE(srcPtr[ zValue.iByte[7] ] * qValue.iInt[1], KShift));

				*dstPtr++ = dstValue.iAligment;
                
				zigZagPtr += 8;
				} while (zigZagPtr < zigZagPtrLimit);
	        }
        }
	}
	
void TQTable::FastHalfDeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt aNumNonZeroValues) const
	{
	DoDeQuantize(aDestination,aSource,Min(KJpgDCTBlockSize/2, aNumNonZeroValues), KJpgDCTBlockSize/2);
	}


void TQTable::DeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt aNumNonZeroValues) const
	{
	DoDeQuantize(aDestination,aSource, aNumNonZeroValues, KJpgDCTBlockSize);
	}

void TQTable::DoDeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,
								TInt aNumNonZeroValues, TInt aBlockLen) const
	{
	FillCompZ(aDestination, aBlockLen );
		
	const TInt16* srcPtr = aSource.iCoeff;
	const TInt16* const srcPtrLimit = srcPtr + aNumNonZeroValues;
	TInt16* dstPtr = aDestination.iCoeff;
	const TUint8* zigZagPtr = KZigZagSequence.iZigZag;
	
	if (iSixteenBitPrecision)
		{
		const TUint16* qPtr = (const TUint16*)iValues;
		while (srcPtr < srcPtrLimit)
			{
			TInt16 v=(*srcPtr++);
			if (v)
				{
				dstPtr[*zigZagPtr] = TInt16(v * (*qPtr));
				}
			++qPtr;
			++zigZagPtr;
			}
		}
	else
		{
		const TUint8* qPtr = iValues;
		while (srcPtr < srcPtrLimit)
			{
			TInt16 v=*srcPtr++;
			if (v)
				{
				dstPtr[*zigZagPtr] = TInt16(v* (*qPtr));
				}
			++zigZagPtr;
			++qPtr;
			}
		}
	}
	
void TQTable::FastQuarterDeQuantize(TDataUnit& aDestination,const TDataUnit& aSource,TInt /*aNumNonZeroValues*/) const
	{
// we're interested only in result at indeces 0,1 and 8,9 for fast 1/4 iDCT, 
// so use excerpt of the ZigZag table:
// 0, 1, 8,	16,	9
//	
	const TInt16* srcPtr = aSource.iCoeff;
					
	TInt16* dstPtr = aDestination.iCoeff;
	
	if (iSixteenBitPrecision)
		{
		const TUint16* qPtr = (const TUint16*)iValues;

		dstPtr[0] = TInt16(srcPtr[0] * qPtr[0]);
		dstPtr[1] = TInt16(srcPtr[1] * qPtr[1]);
		dstPtr[8] = TInt16(srcPtr[2] * qPtr[2]);
		dstPtr[9] = TInt16(srcPtr[4] * qPtr[4]);

		}
	else
		{
		const TUint8* qPtr = iValues;
		dstPtr[0] = TInt16(srcPtr[0] * qPtr[0]);
		dstPtr[1] = TInt16(srcPtr[1] * qPtr[1]);
		dstPtr[8] = TInt16(srcPtr[2] * qPtr[2]);
		dstPtr[9] = TInt16(srcPtr[4] * qPtr[4]);
		}	
	}

#ifdef __ARMCC__
#pragma pop
#endif

