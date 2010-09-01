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

/**
 @file
 @internalComponent
*/

#if defined(__ARMCC__)
#define FORCEDINLINE __forceinline
#else
#define FORCEDINLINE inline
#endif

/*static*/
FORCEDINLINE CRgbBufferPtr::TRgbBufferPtr CRgbBufferPtr::Coord2Ptr(CRgbBufferPtr::TRgbBufferPtr aBase, TInt aX, TInt aY)
	{
	return aBase + aX * aY * KRgbBufferPixelSize;
	}
	
/*static*/
FORCEDINLINE TInt CRgbBufferPtr::Coord2Size(TInt aX, TInt aY)
	{
	return aX * aY * KRgbBufferPixelSize;
	}
	
/*static*/
FORCEDINLINE CRgbBufferPtr::TRgbBufferPtr CRgbBufferPtr::ShiftPtr(CRgbBufferPtr::TRgbBufferPtr aPtr, TInt aOffset)
	{
	return aPtr + aOffset * KRgbBufferPixelSize;
	}

/*static*/
FORCEDINLINE CRgbBufferPtr::TConstRgbBufferPtr CRgbBufferPtr::ShiftPtr(CRgbBufferPtr::TConstRgbBufferPtr aPtr, TInt aOffset)
	{
	return aPtr + aOffset * KRgbBufferPixelSize;
	}
/**
    This function relies on order of colour components within the EColor16M bitmaps
*/	
/*static*/
FORCEDINLINE TUint8 CRgbBufferPtr::Red(CRgbBufferPtr::TConstRgbBufferPtr aPtr)
	{
	return aPtr[ KRedCompIdx ];
	}
	
/**
    This function relies on order of colour components within the EColor16M bitmaps
*/
/*static*/
FORCEDINLINE TUint8 CRgbBufferPtr::Green(CRgbBufferPtr::TConstRgbBufferPtr aPtr)
	{
	return aPtr[ KGreenCompIdx ];
	}
	
/**
    This function relies on order of colour components within the EColor16M bitmaps
*/
/*static*/
FORCEDINLINE TUint8 CRgbBufferPtr::Blue(CRgbBufferPtr::TConstRgbBufferPtr aPtr)
	{
	return aPtr[ KBlueCompIdx ];
	}

FORCEDINLINE TInt CRgbBufferPtr::NextLineOffset() const
    {
    return iNextLineOffset;
    }

inline TInt CRgbBufferPtr::PtrShiftValueMCU16(TUint aNumberOfPixels) const
    {
    return ( aNumberOfPixels >> 4 ) * (iNextLineOffset / KRgbBufferPixelSize) + (aNumberOfPixels & 0xF);
    }
    
inline TInt CRgbBufferPtr::PtrShiftValueMCU8(TUint aNumberOfPixels) const
    {
    return ( aNumberOfPixels >> 3 ) * (iNextLineOffset / KRgbBufferPixelSize) + (aNumberOfPixels & 0x7);
    }    



    
