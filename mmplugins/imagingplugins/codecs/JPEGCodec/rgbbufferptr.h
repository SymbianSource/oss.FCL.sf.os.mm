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

#ifndef RGBBUFFERPTR_H
#define RGBBUFFERPTR_H

#include <e32base.h>

/**
   @file
   @internalComponent
*/

class CFbsBitmap;

/**
	Basic support for RGB input buffer, which can lock data in-place
	or copy it to the internal buffer depending on the bitmap display mode
	and block size/position
*/
class CRgbBufferPtr : public CBase
	{
private:	
	typedef TUint8* TRgbBufferPtr;

	enum 
		{
		KRgbBufferPixelSize = 3,
		};
		
	/**
	colour component indeces when accessing EColor16M mode bitmap
	*/
	enum 
	    {
	    KBlueCompIdx    = 0,
	    KGreenCompIdx   = 1,
	    KRedCompIdx     = 2
	    };
public:
    typedef const TUint8* TConstRgbBufferPtr;
	
	static CRgbBufferPtr* NewL(CFbsBitmap& aBitmap, const TSize& aMCUSizeinPixels);
	
	~CRgbBufferPtr();
	
	TConstRgbBufferPtr LockBuffer(const TPoint& aTopLeft);
	void UnlockBuffer();
	
	// must be used only within a line, can't span lines
	static inline TConstRgbBufferPtr ShiftPtr(TConstRgbBufferPtr aPtr, TInt aOffset);

	inline TInt NextLineOffset() const;
	inline TInt PtrShiftValueMCU16(TUint aNumberOfPixels) const;
	inline TInt PtrShiftValueMCU8(TUint aNumberOfPixels) const;
		
	static inline TUint8 Red(TConstRgbBufferPtr aPtr);
	static inline TUint8 Green(TConstRgbBufferPtr aPtr);
	static inline TUint8 Blue(TConstRgbBufferPtr aPtr);
	
protected:
    static inline TRgbBufferPtr ShiftPtr(TRgbBufferPtr aPtr, TInt aOffset);
    static inline TRgbBufferPtr Coord2Ptr(TRgbBufferPtr aBase, TInt aX, TInt aY);
    static inline TInt Coord2Size(TInt aX, TInt aY);
    
private:
    CRgbBufferPtr(CFbsBitmap& aBitmap, const TSize& aMCUSizeinPixels);
    void ConstructL();
    
private:
	TConstRgbBufferPtr      iLockedPtr;
	TRgbBufferPtr           iRgbBuffer;
	TRgbBufferPtr           iOriginalRgbBuffer;
	TBool					iBitmapDirectAccess;
	TInt					iBitmapScanlineLength;
	
	const TSize             iBitmapSize;
	const TSize             iMCUSizeinPixels;

	TInt                    iNextLineOffset;
	TInt                    iBufferScanlineLength;
    TSize                   iBufferSizeInPixels;
    TPoint                  iBufferPos;

	CFbsBitmap*             iBitmap;
	TUint32*				iBitmapData;
	
	};	

#include "rgbbufferptr.inl"

#endif // ndef RGBBUFFERPTR_H


