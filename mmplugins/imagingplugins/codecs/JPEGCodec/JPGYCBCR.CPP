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

#if defined(__ARMCC__)
// use ARM instruction for performance-critical code
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif

TRgb TYCbCr::YCbCrtoRGB(TInt aY,TInt aCb,TInt aCr)
	{
	aCb -= 128;
	aCr -= 128;

	register TInt red = aY + ((KRedCrFactor * aCr) >> 16);
	register TInt green = aY - ((KGreenCbFactor * aCb + KGreenCrFactor * aCr) >> 16);
	register TInt blue = aY + ((KBlueCbFactor * aCb) >> 16);

	return TRgb(ClipColorComponent(red),
		        ClipColorComponent(green),
		        ClipColorComponent(blue)
		        );
	}
/**
	Clamp and shift blue component for composing Color64K later
*/
FORCEDINLINE TUint ClampBlue64K(TInt aValue)
	{
	return (aValue < 0) ? 0 : (aValue > 255) ? (255>>3) : (aValue>>3);
	}
	
/**
	Clamp and shift green component for composing Color64K later
*/
FORCEDINLINE TUint ClampGreen64K(TInt aValue)
	{
	return (aValue < 0) ? 0 : (aValue > 255) ? (0xFC<<3) : ((aValue & 0xFC)<<3);
	}
	
/**
	Clamp and shift red component for composing Color64K later
*/
FORCEDINLINE TUint ClampRed64K(TInt aValue)
	{
	return (aValue < 0) ? 0 : (aValue > 255) ? (0xF8<<8) : ((aValue & 0xF8)<<8);
	}

/*static*/
void TYCbCr::YCbCrtoRaw64K(TInt aY,TInt aCb,TInt aCr, TRgb* aPtr)
	{
	aCb -= 128;
	aCr -= 128;
	
	(reinterpret_cast<TUint16*>(aPtr))[0]=
								ClampBlue64K(aY + ((KBlueCbFactor * aCb) >> 16) ) 
								| ClampGreen64K(aY - ((KGreenCbFactor * aCb + KGreenCrFactor * aCr) >> 16))
								| ClampRed64K( aY + ((KRedCrFactor * aCr) >> 16) );	
	}


void TYCbCr::YCbCrtoRawRGB(TInt aY,TInt aCb,TInt aCr, TRgb* aPtr)
	{
	TYCbCr::YCbCrtoRawRGBInl(aY, aCb, aCr, aPtr);
	}

/**
    Calculate Y comp for every pixel and  U,V as an average for 2 pixels
    (Accumulate an average in RGB then go to the YUV colour space)
    Scaled fixed point math is used i.e. everything which is more than 1.0 is shifted by 16 bits
    y  = 0.299 * R + 0.587G + 0.114 * B
    cr = 0.5 * R - 0.419G - 0.081 * B
    cb = 0.5 * B -0.169 * R - 0.331 * G
*/
void TYCbCr::RGBtoYCbCr2PixUVSum(TDataUnit::TDataUnitElemType* aYPtr,TDataUnit::TDataUnitElemType* aCbPtr, 
                                                    CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{

	TInt red 	= CRgbBufferPtr::Red(aRgbBuffer);
	TInt green 	= CRgbBufferPtr::Green(aRgbBuffer);
	TInt blue 	= CRgbBufferPtr::Blue(aRgbBuffer);

	aYPtr[0]= TInt16( TUint( (KYRedFactor * red) + (KYGreenFactor * green) + (KYBlueFactor * blue) ) >> 16 );
	
	aRgbBuffer 	= CRgbBufferPtr::ShiftPtr(aRgbBuffer,1);

	red 	+= CRgbBufferPtr::Red(aRgbBuffer);
	green 	+= CRgbBufferPtr::Green(aRgbBuffer);
	blue 	+= CRgbBufferPtr::Blue(aRgbBuffer);	

    //                                        blue * 0.5                                                      de-scale and div by 2 
	aCbPtr[0]               = 128 + TInt16( ((blue << 15) - (KCbRedFactor * red) - (KCbGreenFactor * green) ) >> 17);
    //                                        red * 0.5                                                      de-scale and div by 2 	
	aCbPtr[KJpgDCTBlockSize]= 128 + TInt16( ((red << 15) - (KCrGreenFactor * green) - (KCrBlueFactor * blue)) >> 17);
		
	red 		= CRgbBufferPtr::Red(aRgbBuffer);
	green 		= CRgbBufferPtr::Green(aRgbBuffer);
	blue 		= CRgbBufferPtr::Blue(aRgbBuffer);	
	
	aYPtr[1]= TInt16( TUint( (KYRedFactor * red) + (KYGreenFactor * green) + (KYBlueFactor * blue) ) >> 16 );

	}
	
/**
    Calculate Y comp for every pixel and  U,V as an average for 4 pixels square
    (Accumulate an average in RGB then go to the YUV colour space)
    using aLineStride for going to the next line
    This function effectively combines two calls to RGBtoYCbCr2PixUVSum and provides with some speed
    improvement due to better use of local vars rather than 2 calls to the RGBtoYCbCr2PixUVSum()
    See also RGBtoYCbCr2PixUVSum()
*/
void TYCbCr::RGBtoYCbCr4PixUVSum(TDataUnit::TDataUnitElemType* aYPtr, TDataUnit::TDataUnitElemType* aCb, 
                                    CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer, TInt aLineStride)
	{
	const TInt KYNextLineIdx = 8;
	TInt redS 	= CRgbBufferPtr::Red(aRgbBuffer);
	TInt greenS	= CRgbBufferPtr::Green(aRgbBuffer);
	TInt blueS 	= CRgbBufferPtr::Blue(aRgbBuffer);

	aYPtr[0]= TInt16( TUint( (KYRedFactor * redS) + (KYGreenFactor * greenS) + (KYBlueFactor * blueS) ) >> 16 );
	
	aRgbBuffer 	= CRgbBufferPtr::ShiftPtr(aRgbBuffer, 1);
	
	TInt red 	= CRgbBufferPtr::Red(aRgbBuffer);
	TInt green  = CRgbBufferPtr::Green(aRgbBuffer);
	TInt blue   = CRgbBufferPtr::Blue(aRgbBuffer);	

	redS 	+= red;
	greenS 	+= green;
	blueS 	+= blue;	
	
	aYPtr[1]= TInt16( TUint( (KYRedFactor * red) + (KYGreenFactor * green) + (KYBlueFactor * blue) ) >> 16 );
	
	aRgbBuffer = CRgbBufferPtr::ShiftPtr(aRgbBuffer, -1);
	aRgbBuffer += aLineStride;
	
	redS 	+= (red = CRgbBufferPtr::Red(aRgbBuffer));
	greenS 	+= (green = CRgbBufferPtr::Green(aRgbBuffer));
	blueS 	+= (blue = CRgbBufferPtr::Blue(aRgbBuffer));

    aYPtr[KYNextLineIdx]= TInt16( TUint( (KYRedFactor * red) + (KYGreenFactor * green) + (KYBlueFactor * blue) ) >> 16 );
    
	aRgbBuffer = CRgbBufferPtr::ShiftPtr(aRgbBuffer, 1);
	
	redS 	+= (red = CRgbBufferPtr::Red(aRgbBuffer));
	greenS 	+= (green = CRgbBufferPtr::Green(aRgbBuffer));
	blueS 	+= (blue = CRgbBufferPtr::Blue(aRgbBuffer));
	
	aYPtr[KYNextLineIdx+1]= TInt16( TUint( (KYRedFactor * red) + (KYGreenFactor * green) + (KYBlueFactor * blue) ) >> 16 );
	//                       blue * 0.5                                                      de-scale and div by 4
	aCb[0]= 128 + TInt16( ((blueS << 15) - (KCbRedFactor * redS) - (KCbGreenFactor * greenS) ) >> 18);
	//                                       red * 0.5                                                   de-scale and div by 4
	aCb[KJpgDCTBlockSize]= 128 + TInt16( ((redS << 15) - (KCrGreenFactor * greenS) - (KCrBlueFactor * blueS)) >> 18);
	}	

#if defined(__ARMCC__)
#pragma pop
#endif
