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

// TDCT::Transform() is based heavily on jpeg_fdct_islow() from jfdctint.c
// in the IJG code, Copyright (C) 1991-1998, Thomas G. Lane.
// TDCT::InverseTransform() is based heavily on jpeg_idct_islow() from jidctint.c
// in the IJG code, Copyright (C) 1991-1998, Thomas G. Lane.

#include <e32base.h>

#include "JpegTypes.h"

// Constants.
const TInt CONST_BITS = 13;
const TInt PASS1_BITS = 1;
const TInt FIX_0_298631336 = 2446;
const TInt FIX_0_390180644 = 3196;
const TInt FIX_0_541196100 = 4433;
const TInt FIX_0_765366865 = 6270;
const TInt FIX_0_899976223 = 7373;
const TInt FIX_1_175875602 = 9633;
const TInt FIX_1_501321110 = 12299;
const TInt FIX_1_847759065 = 15137;
const TInt FIX_1_961570560 = 16069;
const TInt FIX_2_053119869 = 16819;
const TInt FIX_2_562915447 = 20995;
const TInt FIX_3_072711026 = 25172;
// for fast dct
const TInt FIX_0_707106781 = 5793;
const TInt FIX_0_382683433 = 3135;
const TInt FIX_1_306562965 = 10703;

//
// this file contains many performance-critical code, so use ARM instruction set for it
//
#ifdef __ARMCC__
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif

void TDCT::Transform(TDataUnit& aDestination,const TDataUnit& aSource, TBool aHighSpeedMode) const
	{
	TAligned64Du temp;
    if (!aHighSpeedMode) 
		{
		DoFirstStep(temp, aSource);
		DoSecondStep(aDestination, temp);
		}
	else 
		{
		DoFastFirstStep(temp, aSource);
		DoFastSecondStep(aDestination, temp);
		}
	}

void TDCT::DoFirstStep(TAligned64Du& aDestination,const TDataUnit& aSource) const
    {
	const TDataUnit::TDataUnitElemType* srcPtr = aSource.iCoeff;

	TInt* tempPtr = aDestination.iData;
	const TDataUnit::TDataUnitElemType* const srcPtrLimit = srcPtr + KJpgDCTBlockSize;

	do
		{
		const TInt KDblShift = KDefPrecShift << 1;
		TInt tmp0 = srcPtr[0] + srcPtr[7] - KDblShift;
		TInt tmp7 = srcPtr[0] - srcPtr[7];
		TInt tmp1 = srcPtr[1] + srcPtr[6] - KDblShift;
		TInt tmp6 = srcPtr[1] - srcPtr[6];
		TInt tmp2 = srcPtr[2] + srcPtr[5] - KDblShift;
		TInt tmp5 = srcPtr[2] - srcPtr[5];
		TInt tmp3 = srcPtr[3] + srcPtr[4] - KDblShift;
		TInt tmp4 = srcPtr[3] - srcPtr[4];

		TInt tmp10 = tmp0 + tmp3;
		TInt tmp13 = tmp0 - tmp3;
		TInt tmp11 = tmp1 + tmp2;
		TInt tmp12 = tmp1 - tmp2;

		tempPtr[0 * KJpgDCTBlockWidth] = ((tmp10 + tmp11) << PASS1_BITS);
		tempPtr[4 * KJpgDCTBlockWidth] = ((tmp10 - tmp11) << PASS1_BITS);

		TInt z1 = (tmp12 + tmp13) * FIX_0_541196100;
		tempPtr[2 * KJpgDCTBlockWidth] = DESCALE(z1 + (tmp13 * FIX_0_765366865),CONST_BITS - PASS1_BITS);
		tempPtr[6 * KJpgDCTBlockWidth] = DESCALE(z1 + (tmp12 * - FIX_1_847759065),CONST_BITS - PASS1_BITS);

		z1 = tmp4 + tmp7;
		TInt z2 = tmp5 + tmp6;
		TInt z3 = tmp4 + tmp6;
		TInt z4 = tmp5 + tmp7;
		TInt z5 = (z3 + z4) * FIX_1_175875602; /* sqrt(2) * c3 */

		tmp4 = tmp4 * FIX_0_298631336; /* sqrt(2) * (-c1+c3+c5-c7) */
		tmp5 = tmp5 * FIX_2_053119869; /* sqrt(2) * ( c1+c3-c5+c7) */
		tmp6 = tmp6 * FIX_3_072711026; /* sqrt(2) * ( c1+c3+c5-c7) */
		tmp7 = tmp7 * FIX_1_501321110; /* sqrt(2) * ( c1+c3-c5-c7) */
		z1 = z1 * - FIX_0_899976223; /* sqrt(2) * (c7-c3) */
		z2 = z2 * - FIX_2_562915447; /* sqrt(2) * (-c1-c3) */
		z3 = z3 * - FIX_1_961570560; /* sqrt(2) * (-c3-c5) */
		z4 = z4 * - FIX_0_390180644; /* sqrt(2) * (c5-c3) */

		z3 += z5;
		z4 += z5;

		tempPtr[7 * KJpgDCTBlockWidth] = DESCALE(tmp4 + z1 + z3, CONST_BITS - PASS1_BITS);
		tempPtr[5 * KJpgDCTBlockWidth] = DESCALE(tmp5 + z2 + z4, CONST_BITS - PASS1_BITS);
		tempPtr[3 * KJpgDCTBlockWidth] = DESCALE(tmp6 + z2 + z3, CONST_BITS - PASS1_BITS);
		tempPtr[1 * KJpgDCTBlockWidth] = DESCALE(tmp7 + z1 + z4, CONST_BITS - PASS1_BITS);

		srcPtr  += KJpgDCTBlockWidth;
		tempPtr += 1;
		} while (srcPtr < srcPtrLimit);
    }

// based on code from jfdctfst.c
void TDCT::DoFastFirstStep(TAligned64Du& aDestination,const TDataUnit& aSource) const
    {
	const TDataUnit::TDataUnitElemType* srcPtr = aSource.iCoeff;

	TInt* tempPtr = aDestination.iData;
	const TDataUnit::TDataUnitElemType* const srcPtrLimit = srcPtr + KJpgDCTBlockSize;

	do
		{
		const TInt KDblShift = KDefPrecShift << 1;
		TInt tmp0 = srcPtr[0] + srcPtr[7] - KDblShift;
		TInt tmp7 = srcPtr[0] - srcPtr[7];
		TInt tmp1 = srcPtr[1] + srcPtr[6] - KDblShift;
		TInt tmp6 = srcPtr[1] - srcPtr[6];
		TInt tmp2 = srcPtr[2] + srcPtr[5] - KDblShift;
		TInt tmp5 = srcPtr[2] - srcPtr[5];
		TInt tmp3 = srcPtr[3] + srcPtr[4] - KDblShift;
		TInt tmp4 = srcPtr[3] - srcPtr[4];

		// Even part
		TInt tmp10 = tmp0 + tmp3;	// phase 2
		TInt tmp13 = tmp0 - tmp3;
		TInt tmp11 = tmp1 + tmp2;
		TInt tmp12 = tmp1 - tmp2;

		tempPtr[0 * KJpgDCTBlockWidth] = ((tmp10 + tmp11) << PASS1_BITS);	// phase 3
		tempPtr[4 * KJpgDCTBlockWidth] = ((tmp10 - tmp11) << PASS1_BITS);

		TInt z1 = (tmp12 + tmp13) * FIX_0_707106781;
		tempPtr[2 * KJpgDCTBlockWidth] = DESCALE(tmp13 + z1, CONST_BITS-PASS1_BITS); // phase 5
		tempPtr[6 * KJpgDCTBlockWidth] = DESCALE(tmp13 - z1, CONST_BITS-PASS1_BITS);

		// Odd part
		tmp10 = tmp4 + tmp5;        // phase 2
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		// The rotator is modified from fig 4-8 to avoid extra negations.
		TInt z5 = (tmp10 - tmp12) * FIX_0_382683433; // c6
		TInt z2 = tmp10 * FIX_0_541196100 + z5; // c2-c6 
		TInt z4 = tmp12 * FIX_1_306562965 + z5; // c2+c6 
		TInt z3 = tmp11 * FIX_0_707106781; // c4 

		TInt z11 = tmp7 + z3;            // phase 5 
		TInt z13 = tmp7 - z3;

		tempPtr[5 * KJpgDCTBlockWidth] = DESCALE(z13 + z2,CONST_BITS - PASS1_BITS);      // phase 6 
		tempPtr[3 * KJpgDCTBlockWidth] = DESCALE(z13 - z2,CONST_BITS - PASS1_BITS);
		tempPtr[1 * KJpgDCTBlockWidth] = DESCALE(z11 + z4,CONST_BITS - PASS1_BITS);
		tempPtr[7 * KJpgDCTBlockWidth] = DESCALE(z11 - z4,CONST_BITS - PASS1_BITS);

		srcPtr  += KJpgDCTBlockWidth;
		tempPtr++;
		} while (srcPtr < srcPtrLimit);
    }


void TDCT::DoSecondStep(TDataUnit& aDestination,const TAligned64Du& aSource) const
    {
    TDataUnit::TDataUnitElemType* dstPtr= aDestination.iCoeff;
    const TInt* srcPtr                  = aSource.iData;
    const TInt* const srcPtrLimit       = srcPtr + KJpgDCTBlockSize;
	do
		{

        register TAligned64TwoCoeff tmp = *( reinterpret_cast<const TAligned64TwoCoeff*>(srcPtr + 0) );
        
		TInt tmp0 = tmp.iData[0] + srcPtr[7];
		TInt tmp7 = tmp.iData[0] - srcPtr[7];
		TInt tmp1 = tmp.iData[1] + srcPtr[6];
		TInt tmp6 = tmp.iData[1] - srcPtr[6];
		
		tmp = *( reinterpret_cast<const TAligned64TwoCoeff*>(srcPtr + 2) );
		TInt tmp2 = tmp.iData[0] + srcPtr[5];
		TInt tmp5 = tmp.iData[0] - srcPtr[5];
		TInt tmp3 = tmp.iData[1] + srcPtr[4];
		TInt tmp4 = tmp.iData[1] - srcPtr[4];

		TInt tmp10 = tmp0 + tmp3;
		TInt tmp13 = tmp0 - tmp3;
		TInt tmp11 = tmp1 + tmp2;
		TInt tmp12 = tmp1 - tmp2;

		dstPtr[0 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp10 + tmp11, PASS1_BITS+3));
		dstPtr[4 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp10 - tmp11, PASS1_BITS+3));

		TInt z1 = (tmp12 + tmp13) * FIX_0_541196100;
		dstPtr[2 * KJpgDCTBlockWidth] = TInt16(DESCALE(z1 + (tmp13 * FIX_0_765366865),CONST_BITS+PASS1_BITS+3));
		dstPtr[6 * KJpgDCTBlockWidth] = TInt16(DESCALE(z1 + (tmp12 * - FIX_1_847759065),CONST_BITS+PASS1_BITS+3));

		z1 = tmp4 + tmp7;
		TInt z2 = tmp5 + tmp6;
		TInt z3 = tmp4 + tmp6;
		TInt z4 = tmp5 + tmp7;
		TInt z5 = (z3 + z4) * FIX_1_175875602; /* sqrt(2) * c3 */

		tmp4 = tmp4 * FIX_0_298631336; /* sqrt(2) * (-c1+c3+c5-c7) */
		tmp5 = tmp5 * FIX_2_053119869; /* sqrt(2) * ( c1+c3-c5+c7) */
		tmp6 = tmp6 * FIX_3_072711026; /* sqrt(2) * ( c1+c3+c5-c7) */
		tmp7 = tmp7 * FIX_1_501321110; /* sqrt(2) * ( c1+c3-c5-c7) */
		z1 = z1 * - FIX_0_899976223; /* sqrt(2) * (c7-c3) */
		z2 = z2 * - FIX_2_562915447; /* sqrt(2) * (-c1-c3) */
		z3 = z3 * - FIX_1_961570560; /* sqrt(2) * (-c3-c5) */
		z4 = z4 * - FIX_0_390180644; /* sqrt(2) * (c5-c3) */

		z3 += z5;
		z4 += z5;

		dstPtr[7 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp4 + z1 + z3,CONST_BITS+PASS1_BITS+3));
		dstPtr[5 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp5 + z2 + z4,CONST_BITS+PASS1_BITS+3));
		dstPtr[3 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp6 + z2 + z3,CONST_BITS+PASS1_BITS+3));
		dstPtr[1 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp7 + z1 + z4,CONST_BITS+PASS1_BITS+3));

		srcPtr+=KJpgDCTBlockWidth;
		dstPtr++;
		} while (srcPtr < srcPtrLimit);
    }

// based on code from jfdctfst.c
void TDCT::DoFastSecondStep(TDataUnit& aDestination,const TAligned64Du& aSource) const
    {
    TDataUnit::TDataUnitElemType* dstPtr= aDestination.iCoeff;
    const TInt* srcPtr                  = aSource.iData;
    const TInt* const srcPtrLimit       = srcPtr + KJpgDCTBlockSize;
	do
		{
		register TAligned64TwoCoeff tmp = *( reinterpret_cast<const TAligned64TwoCoeff*>(srcPtr + 0) );
        
		TInt tmp0 = tmp.iData[0] + srcPtr[7];
		TInt tmp7 = tmp.iData[0] - srcPtr[7];
		TInt tmp1 = tmp.iData[1] + srcPtr[6];
		TInt tmp6 = tmp.iData[1] - srcPtr[6];
		
		tmp = *( reinterpret_cast<const TAligned64TwoCoeff*>(srcPtr + 2) );
		TInt tmp2 = tmp.iData[0] + srcPtr[5];
		TInt tmp5 = tmp.iData[0] - srcPtr[5];
		TInt tmp3 = tmp.iData[1] + srcPtr[4];
		TInt tmp4 = tmp.iData[1] - srcPtr[4];

		TInt tmp10 = tmp0 + tmp3; // phase 2
		TInt tmp13 = tmp0 - tmp3;
		TInt tmp11 = tmp1 + tmp2;
		TInt tmp12 = tmp1 - tmp2;

		dstPtr[0 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp10 + tmp11, PASS1_BITS+3)); // phase 3
		dstPtr[4 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp10 - tmp11, PASS1_BITS+3));

		TInt z1 = (tmp12 + tmp13) * FIX_0_707106781; // c4
		dstPtr[2 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp13 + z1, CONST_BITS+PASS1_BITS+3)); // phase 5
		dstPtr[6 * KJpgDCTBlockWidth] = TInt16(DESCALE(tmp13 - z1, CONST_BITS+PASS1_BITS+3));

		
		// Odd part
		tmp10 = tmp4 + tmp5;        // phase 2
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		// The rotator is modified from fig 4-8 to avoid extra negations.
		TInt z5 = (tmp10 - tmp12) * FIX_0_382683433; // c6
		TInt z2 = tmp10 * FIX_0_541196100 + z5; // c2-c6
		TInt z4 = tmp12 * FIX_1_306562965 + z5; // c2+c6
		TInt z3 = tmp11 * FIX_0_707106781; // c4

		TInt z11 = tmp7 + z3;            // phase 5
		TInt z13 = tmp7 - z3;

		dstPtr[5 * KJpgDCTBlockWidth] = TInt16(DESCALE(z13 + z2, CONST_BITS+PASS1_BITS+3)); // phase 6
		dstPtr[3 * KJpgDCTBlockWidth] = TInt16(DESCALE(z13 - z2, CONST_BITS+PASS1_BITS+3));
		dstPtr[1 * KJpgDCTBlockWidth] = TInt16(DESCALE(z11 + z4, CONST_BITS+PASS1_BITS+3));
		dstPtr[7 * KJpgDCTBlockWidth] = TInt16(DESCALE(z11 - z4, CONST_BITS+PASS1_BITS+3));

		srcPtr+=KJpgDCTBlockWidth;
		dstPtr++;
		} while (srcPtr < srcPtrLimit);
    }


void TDCT::InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const
	{
	const TInt16* srcPtr = aSource.iCoeff;
	TInt16* dstPtr = aDestination.iCoeff;

	TInt temp[KJpgDCTBlockSize];
	TInt* tempPtr = temp;
	TInt* tempPtrLimit = temp + KJpgDCTBlockWidth;

	do
		{
		TInt z2 = srcPtr[2 * KJpgDCTBlockWidth];
		TInt z3 = srcPtr[6 * KJpgDCTBlockWidth];

		if (z2 == 0 && z3 == 0 && srcPtr[1 * KJpgDCTBlockWidth] == 0 && 
			srcPtr[3 * KJpgDCTBlockWidth] == 0 && srcPtr[4 * KJpgDCTBlockWidth] == 0 &&
			srcPtr[5 * KJpgDCTBlockWidth] == 0 && 
			srcPtr[7 * KJpgDCTBlockWidth] == 0)
			{
			TInt dcval = srcPtr[0] << PASS1_BITS;

			tempPtr[0 * KJpgDCTBlockWidth] = dcval;
			tempPtr[1 * KJpgDCTBlockWidth] = dcval;
			tempPtr[2 * KJpgDCTBlockWidth] = dcval;
			tempPtr[3 * KJpgDCTBlockWidth] = dcval;
			tempPtr[4 * KJpgDCTBlockWidth] = dcval;
			tempPtr[5 * KJpgDCTBlockWidth] = dcval;
			tempPtr[6 * KJpgDCTBlockWidth] = dcval;
			tempPtr[7 * KJpgDCTBlockWidth] = dcval;

			srcPtr++;
			tempPtr++;
			continue;
			}

		TInt z1 = (z2 + z3) * FIX_0_541196100;
		TInt tmp2 = z1 + (z3 * - FIX_1_847759065);
		TInt tmp3 = z1 + (z2 * FIX_0_765366865);

		z2 = srcPtr[0 * KJpgDCTBlockWidth];
		z3 = srcPtr[4 * KJpgDCTBlockWidth];

		TInt tmp0 = (z2 + z3) << CONST_BITS;
		TInt tmp1 = (z2 - z3) << CONST_BITS;

		const TInt tmp10 = tmp0 + tmp3;
		const TInt tmp13 = tmp0 - tmp3;
		const TInt tmp11 = tmp1 + tmp2;
		const TInt tmp12 = tmp1 - tmp2;

		tmp0 = srcPtr[7 * KJpgDCTBlockWidth];
		tmp1 = srcPtr[5 * KJpgDCTBlockWidth];
		tmp2 = srcPtr[3 * KJpgDCTBlockWidth];
		tmp3 = srcPtr[1 * KJpgDCTBlockWidth];

		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		TInt z4 = tmp1 + tmp3;
		const TInt z5 = (z3 + z4) * FIX_1_175875602;

		tmp0 = tmp0 * FIX_0_298631336;
		tmp1 = tmp1 * FIX_2_053119869;
		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * - FIX_0_899976223;
		z2 = z2 * - FIX_2_562915447;
		z3 = z3 * - FIX_1_961570560;
		z4 = z4 * - FIX_0_390180644;

		z3 += z5;
		z4 += z5;

		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		tempPtr[0 * KJpgDCTBlockWidth] = DESCALE(tmp10 + tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[7 * KJpgDCTBlockWidth] = DESCALE(tmp10 - tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[1 * KJpgDCTBlockWidth] = DESCALE(tmp11 + tmp2, CONST_BITS - PASS1_BITS);
		tempPtr[6 * KJpgDCTBlockWidth] = DESCALE(tmp11 - tmp2, CONST_BITS - PASS1_BITS);
		tempPtr[2 * KJpgDCTBlockWidth] = DESCALE(tmp12 + tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[5 * KJpgDCTBlockWidth] = DESCALE(tmp12 - tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[3 * KJpgDCTBlockWidth] = DESCALE(tmp13 + tmp0, CONST_BITS - PASS1_BITS);
		tempPtr[4 * KJpgDCTBlockWidth] = DESCALE(tmp13 - tmp0, CONST_BITS - PASS1_BITS);

		srcPtr++;
		tempPtr++;
		} while (tempPtr < tempPtrLimit);

	tempPtr = temp;
	tempPtrLimit = temp + KJpgDCTBlockSize;

	do
		{
		TInt z2 = tempPtr[2];
		TInt z3 = tempPtr[6];
		const TInt t4 = tempPtr[4];
		
		if (z2 == 0 && z3 == 0 && tempPtr[1] == 0 && t4 == 0 && tempPtr[3] == 0 && 
			tempPtr[5] == 0 && tempPtr[7] == 0)
			{
			TUint16 dcval = TUint16( TInt16(DESCALE(tempPtr[0], PASS1_BITS+3) + iLevelShift) );
			TUint64 w=MAKE_TUINT64( ((dcval << 16) | (dcval)), ((dcval << 16) | dcval) );

			*(0 + reinterpret_cast<TUint64*>(dstPtr) ) = w;
			*(1 + reinterpret_cast<TUint64*>(dstPtr) ) = w;
			
			dstPtr += KJpgDCTBlockWidth;
			tempPtr += KJpgDCTBlockWidth;
			continue;
			}

		TInt z1 = (z2 + z3) * FIX_0_541196100;
		TInt tmp2 = z1 + (z3 * - FIX_1_847759065);
		TInt tmp3 = z1 + (z2 * FIX_0_765366865);

		TInt tmp0 = (tempPtr[0] + t4) << CONST_BITS;
		TInt tmp1 = (tempPtr[0] - t4) << CONST_BITS;

		const TInt tmp10 = tmp0 + tmp3;
		const TInt tmp13 = tmp0 - tmp3;
		const TInt tmp11 = tmp1 + tmp2;
		const TInt tmp12 = tmp1 - tmp2;

		tmp0 = tempPtr[7];
		tmp1 = tempPtr[5];
		tmp2 = tempPtr[3];
		tmp3 = tempPtr[1];

		z1 = tmp0 + tmp3;
		z2 = tmp1 + tmp2;
		z3 = tmp0 + tmp2;
		TInt z4 = tmp1 + tmp3;
		const TInt z5 = (z3 + z4) * FIX_1_175875602;

		tmp0 = tmp0 * FIX_0_298631336;
		tmp1 = tmp1 * FIX_2_053119869;
		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;

		z1 = z1 * - FIX_0_899976223;
		z2 = z2 * - FIX_2_562915447;
		z3 = z3 * - FIX_1_961570560;
		z4 = z4 * - FIX_0_390180644;

		z3 += z5;
		z4 += z5;

		tmp0 += z1 + z3;
		tmp1 += z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		const TInt KLevelShift = iLevelShift;
		dstPtr[0] = TInt16(DESCALE(tmp10 + tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[7] = TInt16(DESCALE(tmp10 - tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[1] = TInt16(DESCALE(tmp11 + tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[6] = TInt16(DESCALE(tmp11 - tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[2] = TInt16(DESCALE(tmp12 + tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[5] = TInt16(DESCALE(tmp12 - tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[3] = TInt16(DESCALE(tmp13 + tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[4] = TInt16(DESCALE(tmp13 - tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);

		tempPtr += KJpgDCTBlockWidth;
		dstPtr += KJpgDCTBlockWidth;
		} while (tempPtr < tempPtrLimit);
	}


// THalfDCT
void THalfDCT::InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const
	{
	const TInt16* srcPtr = aSource.iCoeff;
	TInt16* dstPtr = aDestination.iCoeff;

	TInt temp[KJpgDCTBlockSize];
	TInt* tempPtr = temp;
	TInt* tempPtrLimit = temp + KJpgDCTBlockWidth;

	while (tempPtr < tempPtrLimit)
		{
		TInt z2 = srcPtr[2 * KJpgDCTBlockWidth];
		
		if (z2 == 0 && srcPtr[1 * KJpgDCTBlockWidth] == 0 && 
			srcPtr[3 * KJpgDCTBlockWidth] == 0)
			{
			TInt dcval = srcPtr[0] << PASS1_BITS;

			tempPtr[0 * KJpgDCTBlockWidth] = dcval;
			tempPtr[1 * KJpgDCTBlockWidth] = dcval;
			tempPtr[2 * KJpgDCTBlockWidth] = dcval;
			tempPtr[3 * KJpgDCTBlockWidth] = dcval;
			tempPtr[4 * KJpgDCTBlockWidth] = dcval;
			tempPtr[5 * KJpgDCTBlockWidth] = dcval;
			tempPtr[6 * KJpgDCTBlockWidth] = dcval;
			tempPtr[7 * KJpgDCTBlockWidth] = dcval;

			srcPtr++;
			tempPtr++;
			continue;
			}

		TInt z1 = z2 * FIX_0_541196100;
		TInt tmp2 = z1;
		TInt tmp3 = z1 + (z2 * FIX_0_765366865);

		z2 = srcPtr[0 * KJpgDCTBlockWidth];

		TInt tmp0 = z2 << CONST_BITS;

		const TInt tmp10 = tmp0 + tmp3;
		const TInt tmp13 = tmp0 - tmp3;
		const TInt tmp11 = tmp0 + tmp2;
		const TInt tmp12 = tmp0 - tmp2;

		tmp2 = srcPtr[3 * KJpgDCTBlockWidth];
		tmp3 = srcPtr[1 * KJpgDCTBlockWidth];

		z1 = tmp3;
		z2 = tmp2;
		TInt z3 = tmp2;
		TInt z4 = tmp3;
		const TInt z5 = (z3 + z4) * FIX_1_175875602;

		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * - FIX_0_899976223;
		z2 = z2 * - FIX_2_562915447;
		z3 = z3 * - FIX_1_961570560;
		z4 = z4 * - FIX_0_390180644;

		z3 += z5;
		z4 += z5;

		tmp0 = z1 + z3;
		const TInt tmp1 = z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		tempPtr[0 * KJpgDCTBlockWidth] = DESCALE(tmp10 + tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[7 * KJpgDCTBlockWidth] = DESCALE(tmp10 - tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[1 * KJpgDCTBlockWidth] = DESCALE(tmp11 + tmp2, CONST_BITS - PASS1_BITS);
		tempPtr[6 * KJpgDCTBlockWidth] = DESCALE(tmp11 - tmp2, CONST_BITS - PASS1_BITS);
		tempPtr[2 * KJpgDCTBlockWidth] = DESCALE(tmp12 + tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[5 * KJpgDCTBlockWidth] = DESCALE(tmp12 - tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[3 * KJpgDCTBlockWidth] = DESCALE(tmp13 + tmp0, CONST_BITS - PASS1_BITS);
		tempPtr[4 * KJpgDCTBlockWidth] = DESCALE(tmp13 - tmp0, CONST_BITS - PASS1_BITS);

		srcPtr++;
		tempPtr++;
		}

	tempPtr = temp;
	tempPtrLimit = temp + KJpgDCTBlockSize;

	while (tempPtr < tempPtrLimit)
		{
		TInt z2 = tempPtr[2];
		
		if (z2 == 0 && tempPtr[1] == 0 && tempPtr[3] == 0)
			{
			TUint16 dcval = TUint16( TInt16(DESCALE(tempPtr[0], PASS1_BITS+3) + iLevelShift) );
			TUint64 w=MAKE_TUINT64( ((dcval << 16) | (dcval)), ((dcval << 16) | dcval) );

			*(0 + reinterpret_cast<TUint64*>(dstPtr) ) = w;
			*(1 + reinterpret_cast<TUint64*>(dstPtr) ) = w;
			
			dstPtr += KJpgDCTBlockWidth;
			tempPtr += KJpgDCTBlockWidth;
			continue;
			}

		TInt z1 = z2 * FIX_0_541196100;
		TInt tmp2 = z1;
		TInt tmp3 = z1 + (z2 * FIX_0_765366865);

		TInt tmp0 = tempPtr[0] << CONST_BITS;

		const TInt tmp10 = tmp0 + tmp3;
		const TInt tmp13 = tmp0 - tmp3;
		const TInt tmp11 = tmp0 + tmp2;
		const TInt tmp12 = tmp0 - tmp2;

		tmp2 = tempPtr[3];
		tmp3 = tempPtr[1];

		z1 = tmp3;
		z2 = tmp2;
		TInt z3 = tmp2;
		TInt z4 = tmp3;
		const TInt z5 = (z3 + z4) * FIX_1_175875602;

		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;

		z1 = z1 * - FIX_0_899976223;
		z2 = z2 * - FIX_2_562915447;
		z3 = z3 * - FIX_1_961570560;
		z4 = z4 * - FIX_0_390180644;

		z3 += z5;
		z4 += z5;

		tmp0 = z1 + z3;
		const TInt tmp1 = z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		const TInt KLevelShift = iLevelShift;
		dstPtr[0] = TInt16(DESCALE(tmp10 + tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[7] = TInt16(DESCALE(tmp10 - tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[1] = TInt16(DESCALE(tmp11 + tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[6] = TInt16(DESCALE(tmp11 - tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[2] = TInt16(DESCALE(tmp12 + tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[5] = TInt16(DESCALE(tmp12 - tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[3] = TInt16(DESCALE(tmp13 + tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[4] = TInt16(DESCALE(tmp13 - tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);

		tempPtr += KJpgDCTBlockWidth;
		dstPtr += KJpgDCTBlockWidth;
		}
	}

//TFastHalfDCT 
void TFastHalfDCT::InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const
	{
	const TInt KScalingFactor = 2;
	const TInt16* srcPtr = aSource.iCoeff;
	TInt16* dstPtr = aDestination.iCoeff;
	
	TInt temp[KJpgDCTBlockSize/KScalingFactor];
	
	TInt* tempPtr = temp;
	TInt* tempPtrLimit = temp + KJpgDCTBlockWidth / KScalingFactor; // scaling factor

	do 
		{
		TInt z2 = srcPtr[2 * KJpgDCTBlockWidth];
		
		if ( z2 == 0 && srcPtr[1 * KJpgDCTBlockWidth] == 0 && srcPtr[3 * KJpgDCTBlockWidth] == 0)
			{
			TInt dcval = srcPtr[0] << PASS1_BITS;

			tempPtr[0 * KJpgDCTBlockWidth / KScalingFactor] = dcval;
			tempPtr[2 * KJpgDCTBlockWidth / KScalingFactor] = dcval;
			tempPtr[4 * KJpgDCTBlockWidth / KScalingFactor] = dcval;
			tempPtr[6 * KJpgDCTBlockWidth / KScalingFactor] = dcval;

			srcPtr++;
			tempPtr++;
			continue;
			}

		TInt z1 	= z2 * FIX_0_541196100;
		TInt tmp2 	= z1;
		TInt tmp3 	= z1 + (z2 * FIX_0_765366865);

		z2 = srcPtr[0 * KJpgDCTBlockWidth];

		TInt tmp0 = z2 << CONST_BITS;

		const TInt tmp10 = tmp0 + tmp3;
		const TInt tmp13 = tmp0 - tmp3;
		const TInt tmp11 = tmp0 + tmp2;
		const TInt tmp12 = tmp0 - tmp2;

		tmp2 = srcPtr[3 * KJpgDCTBlockWidth];
		tmp3 = srcPtr[1 * KJpgDCTBlockWidth];

		z1 = tmp3;
		z2 = tmp2;
		TInt z3 = tmp2;
		TInt z4 = tmp3;
		const TInt z5 = (z3 + z4) * FIX_1_175875602;

		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * - FIX_0_899976223;
		z2 = z2 * - FIX_2_562915447;
		z3 = z3 * - FIX_1_961570560;
		z4 = z4 * - FIX_0_390180644;

		z3 += z5;
		z4 += z5;

		tmp0 = z1 + z3;
		const TInt tmp1 = z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		tempPtr[0 * KJpgDCTBlockWidth / KScalingFactor] = DESCALE(tmp10 + tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[2 * KJpgDCTBlockWidth / KScalingFactor] = DESCALE(tmp12 + tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[4 * KJpgDCTBlockWidth / KScalingFactor] = DESCALE(tmp13 - tmp0, CONST_BITS - PASS1_BITS);
		tempPtr[6 * KJpgDCTBlockWidth / KScalingFactor] = DESCALE(tmp11 - tmp2, CONST_BITS - PASS1_BITS);
		
		srcPtr++;
		tempPtr++;
		
		} while (tempPtr < tempPtrLimit);
		
	tempPtr = temp;
	tempPtrLimit = temp + KJpgDCTBlockSize / KScalingFactor;

	do 
		{
		TInt z2 = tempPtr[2];

		if ( z2 == 0 && tempPtr[1] == 0 && tempPtr[3] == 0)
			{
			TInt16 dcval = TInt16(DESCALE(tempPtr[0], PASS1_BITS+3) + iLevelShift);

			dstPtr[0] = dcval;
			dstPtr[2] = dcval;
			dstPtr[4] = dcval;
			dstPtr[6] = dcval;

			tempPtr += KJpgDCTBlockWidth;
			dstPtr 	+= KScalingFactor*KJpgDCTBlockWidth;
			continue;
			}

		TInt z1 = z2 * FIX_0_541196100;
		TInt tmp2 = z1;
		TInt tmp3 = z1 + (z2 * FIX_0_765366865);

		TInt tmp0 = tempPtr[0] << CONST_BITS;

		const TInt tmp10 = tmp0 + tmp3;
		const TInt tmp13 = tmp0 - tmp3;
		const TInt tmp11 = tmp0 + tmp2;
		const TInt tmp12 = tmp0 - tmp2;

		tmp2 = tempPtr[3];
		tmp3 = tempPtr[1];

		z1 = tmp3;
		z2 = tmp2;
		TInt z3 = tmp2;
		TInt z4 = tmp3;
		TInt z5 = (z3 + z4) * FIX_1_175875602;

		tmp2 = tmp2 * FIX_3_072711026;
		tmp3 = tmp3 * FIX_1_501321110;

		z1 = z1 * - FIX_0_899976223;
		z2 = z2 * - FIX_2_562915447;
		z3 = z3 * - FIX_1_961570560;
		z4 = z4 * - FIX_0_390180644;

		z3 += z5;
		z4 += z5;

		tmp0 = z1 + z3;
		const TInt tmp1 = z2 + z4;
		tmp2 += z2 + z3;
		tmp3 += z1 + z4;

		const TInt KLevelShift = iLevelShift;
		dstPtr[0] = TInt16(DESCALE(tmp10 + tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[2] = TInt16(DESCALE(tmp12 + tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[4] = TInt16(DESCALE(tmp13 - tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[6] = TInt16(DESCALE(tmp11 - tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);

		tempPtr += KJpgDCTBlockWidth;
		dstPtr 	+= KScalingFactor*KJpgDCTBlockWidth;
		
		} while (tempPtr < tempPtrLimit);
	}

// TQuarterDCT
void TQuarterDCT::InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const
	{
	const TInt16* srcPtr = aSource.iCoeff;
	TInt16* dstPtr = aDestination.iCoeff;
	
	TInt temp[KJpgDCTBlockSize];
	TInt* tempPtr = temp;
	TInt* tempPtrLimit = temp + KJpgDCTBlockWidth;

	do
		{
		TInt tmp3 = srcPtr[1 * KJpgDCTBlockWidth];
		
		if (tmp3 == 0)
			{
			TInt dcval = srcPtr[0] << PASS1_BITS;

			tempPtr[0 * KJpgDCTBlockWidth] = dcval;
			tempPtr[1 * KJpgDCTBlockWidth] = dcval;
			tempPtr[2 * KJpgDCTBlockWidth] = dcval;
			tempPtr[3 * KJpgDCTBlockWidth] = dcval;
			tempPtr[4 * KJpgDCTBlockWidth] = dcval;
			tempPtr[5 * KJpgDCTBlockWidth] = dcval;
			tempPtr[6 * KJpgDCTBlockWidth] = dcval;
			tempPtr[7 * KJpgDCTBlockWidth] = dcval;

			srcPtr++;
			tempPtr++;
			continue;
			}

		TInt z2 = srcPtr[0 * KJpgDCTBlockWidth];

		TInt tmp0 = z2 << CONST_BITS;

		TInt tmp10 = tmp0;

		TInt z1 = tmp3;
		TInt z4 = tmp3;
		TInt z5 = z4 * FIX_1_175875602;

		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * - FIX_0_899976223;
		z4 = z4 * - FIX_0_390180644;

		const TInt z3 = z5;
		z4 += z5;

		tmp0 = z1 + z3;
		const TInt tmp1 = z4;
		const TInt tmp2 = z3;
		tmp3 += z1 + z4;

		tempPtr[0 * KJpgDCTBlockWidth] = DESCALE(tmp10 + tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[7 * KJpgDCTBlockWidth] = DESCALE(tmp10 - tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[1 * KJpgDCTBlockWidth] = DESCALE(tmp10 + tmp2, CONST_BITS - PASS1_BITS);
		tempPtr[6 * KJpgDCTBlockWidth] = DESCALE(tmp10 - tmp2, CONST_BITS - PASS1_BITS);
		tempPtr[2 * KJpgDCTBlockWidth] = DESCALE(tmp10 + tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[5 * KJpgDCTBlockWidth] = DESCALE(tmp10 - tmp1, CONST_BITS - PASS1_BITS);
		tempPtr[3 * KJpgDCTBlockWidth] = DESCALE(tmp10 + tmp0, CONST_BITS - PASS1_BITS);
		tempPtr[4 * KJpgDCTBlockWidth] = DESCALE(tmp10 - tmp0, CONST_BITS - PASS1_BITS);

		srcPtr++;
		tempPtr++;
		} while (tempPtr < tempPtrLimit);

	tempPtr = temp;
	tempPtrLimit = temp + KJpgDCTBlockSize;

	do
		{
		TInt tmp3 = tempPtr[1];
		
		if (tmp3 == 0)
			{
			TUint16 dcval = TUint16( TInt16(DESCALE(tempPtr[0], PASS1_BITS+3) + iLevelShift) );

			TUint64 w=MAKE_TUINT64( ((dcval << 16) | (dcval)), ((dcval << 16) | dcval) );

			*(0 + reinterpret_cast<TUint64*>(dstPtr) ) = w;
			*(1 + reinterpret_cast<TUint64*>(dstPtr) ) = w;

			tempPtr += KJpgDCTBlockWidth;
			dstPtr += KJpgDCTBlockWidth;
			continue;
			}

		TInt tmp10 = tempPtr[0] << CONST_BITS;

		
		TInt z1 = tmp3;
		TInt z4 = tmp3;
		TInt z5 = z4 * FIX_1_175875602;

		tmp3 = tmp3 * FIX_1_501321110;

		z1 = z1 * - FIX_0_899976223;
		z4 = z4 * - FIX_0_390180644;

		z4 += z5;

		const TInt tmp0 = z1 + z5;
		const TInt tmp1 = z4;
		const TInt tmp2 = z5;
		tmp3 += z1 + z4;

		const TInt KLevelShift = iLevelShift;
		dstPtr[0] = TInt16(DESCALE(tmp10 + tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[7] = TInt16(DESCALE(tmp10 - tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[1] = TInt16(DESCALE(tmp10 + tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[6] = TInt16(DESCALE(tmp10 - tmp2,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[2] = TInt16(DESCALE(tmp10 + tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[5] = TInt16(DESCALE(tmp10 - tmp1,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[3] = TInt16(DESCALE(tmp10 + tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[4] = TInt16(DESCALE(tmp10 - tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);

		tempPtr += KJpgDCTBlockWidth;
		dstPtr += KJpgDCTBlockWidth;
		} while (tempPtr < tempPtrLimit);
	}

// TFastQuarterDCT
void TFastQuarterDCT::InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const
	{
	const TInt KScalingFactor=4;
	const TInt16* srcPtr= aSource.iCoeff;
	TInt16* dstPtr 		= aDestination.iCoeff;
	
	TInt temp[KJpgDCTBlockWidth];
	
	TInt* tempPtr = temp;
	TInt* tempPtrLimit = temp + KJpgDCTBlockWidth / KScalingFactor; //scaling factor

	do
		{
		TInt tmp3 = srcPtr[1 * KJpgDCTBlockWidth];
		
		if (tmp3 == 0)
			{
			TInt dcval = srcPtr[0] << PASS1_BITS;

			tempPtr[0 ] = dcval;
			tempPtr[4 ] = dcval;
	
			srcPtr	+=1;
			tempPtr	+=1;
			continue;
			}

		const TInt z2 = srcPtr[0 * KJpgDCTBlockWidth];

		TInt tmp0 = z2 << CONST_BITS;

		const TInt tmp10 = tmp0;

		TInt z1 = tmp3;
		TInt z4 = tmp3;
		const TInt z5 = z4 * FIX_1_175875602;

		tmp3 = tmp3 * FIX_1_501321110;
		z1 = z1 * - FIX_0_899976223;
		z4 = z4 * - FIX_0_390180644;

		const TInt z3 = z5;
		z4 += z5;

		tmp0 = z1 + z3;

		tmp3 += z1 + z4;

		tempPtr[0 ] = DESCALE(tmp10 + tmp3, CONST_BITS - PASS1_BITS);
		tempPtr[4 ] = DESCALE(tmp10 - tmp0, CONST_BITS - PASS1_BITS);

		srcPtr	+=1;
		tempPtr	+=1;
		} while (tempPtr < tempPtrLimit);

	tempPtr 	= temp;
	tempPtrLimit= temp + KJpgDCTBlockWidth; 

	do
		{
		TInt tmp3 = tempPtr[1];
			
		if (tmp3 == 0)
			{
			TInt16 dcval = TInt16(DESCALE(tempPtr[0], PASS1_BITS+3) + iLevelShift);

			dstPtr[0] = dcval;
			dstPtr[4] = dcval;
			
			tempPtr += KScalingFactor;
			dstPtr 	+= KScalingFactor*KJpgDCTBlockWidth;
			continue;
			}

		const TInt tmp10 = tempPtr[0] << CONST_BITS;

		TInt z4 = tmp3;
		const TInt z5 = z4 * FIX_1_175875602;

		const TInt z1 = tmp3 * - FIX_0_899976223;
		tmp3 = tmp3 * FIX_1_501321110;
		
		z4 = z4 * - FIX_0_390180644;

		const TInt z3 = z5;
		z4 += z5;

		const TInt tmp0 = z1 + z3;

		tmp3 += z1 + z4;

		const TInt KLevelShift = iLevelShift;
		dstPtr[0] = TInt16(DESCALE(tmp10 + tmp3,CONST_BITS+PASS1_BITS+3) + KLevelShift);
		dstPtr[4] = TInt16(DESCALE(tmp10 - tmp0,CONST_BITS+PASS1_BITS+3) + KLevelShift);

		tempPtr += KScalingFactor;
		dstPtr 	+= KScalingFactor*KJpgDCTBlockWidth;
		} while (tempPtr < tempPtrLimit);
	}

// TEighthDCT
void TEighthDCT::InverseTransform(TDataUnit& aDestination,const TDataUnit& aSource) const
	{
	Fast18InvTransform(aDestination, aSource.iCoeff[0]);
	}

#ifdef __ARMCC__
#pragma pop
#endif

