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

#include <fbs.h>
#include "JpegConsts.h"

#include "rgbbufferptr.h"


/*static*/
CRgbBufferPtr* CRgbBufferPtr::NewL(CFbsBitmap& aBitmap, const TSize& aMCUSizeinPixels)
    {
    CRgbBufferPtr* self = new (ELeave) CRgbBufferPtr(aBitmap, aMCUSizeinPixels);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
CRgbBufferPtr::CRgbBufferPtr(CFbsBitmap& aBitmap, const TSize& aMCUSizeinPixels):
                                    iBitmapSize( aBitmap.SizeInPixels() ),
									iMCUSizeinPixels( aMCUSizeinPixels ),
									iBufferPos(-1,-1),
                                    iBitmap( &aBitmap )
    {
    }
    
void CRgbBufferPtr::ConstructL()
    {
    if ((iBitmapSize.iWidth <= 0) || (iBitmapSize.iHeight <= 0))
    	{
    	User::Leave(KErrArgument);
    	}
    
    iBitmapData = iBitmap->DataAddress();
	iBitmapScanlineLength = iBitmap->ScanLineLength(iBitmapSize.iWidth, iBitmap->DisplayMode());

	if (iBitmap->DisplayMode() == EColor16M)
        {
        iBufferSizeInPixels.SetSize( iMCUSizeinPixels.iWidth, iMCUSizeinPixels.iHeight);
		iBitmapDirectAccess = ETrue;
        }
    else
        {
        // we reduce number of bitmap round-robins using horizontally-widened buffer ONLY!
        TInt width = Max(iMCUSizeinPixels.iWidth, Min(iMCUSizeinPixels.iWidth<<2, iBitmapSize.iWidth));
        if (width == iBitmapSize.iWidth )
        	{
        	TInt oddPixels = width % iMCUSizeinPixels.iWidth;
        	if (oddPixels)
        		{
        		width += iMCUSizeinPixels.iWidth - oddPixels;
        		}
        	}
        iBufferSizeInPixels.SetSize( width, iMCUSizeinPixels.iHeight);
		iBitmapDirectAccess = EFalse;
		}
    
    iOriginalRgbBuffer = reinterpret_cast<TRgbBufferPtr>(User::AllocL(Coord2Size(iBufferSizeInPixels.iWidth, iBufferSizeInPixels.iHeight) + sizeof(TUint32)));
    iRgbBuffer = Align4(iOriginalRgbBuffer);
	
    iBufferScanlineLength = iBufferSizeInPixels.iWidth * KRgbBufferPixelSize;
	}
    
CRgbBufferPtr::~CRgbBufferPtr()
    {
    User::Free( iOriginalRgbBuffer);
    iOriginalRgbBuffer = NULL;
    iRgbBuffer = NULL;    
    }
    

CRgbBufferPtr::TConstRgbBufferPtr CRgbBufferPtr::LockBuffer(const TPoint& aTopLeft)
    {
    ASSERT( aTopLeft.iX < iBitmapSize.iWidth );
    ASSERT( aTopLeft.iY < iBitmapSize.iHeight );
    
    //const TBool KDirectAccess = ( iBitmap->DisplayMode() == EColor16M );
    const TBool KInplaceAccess= iBitmapDirectAccess;
    
    if ( !KInplaceAccess &&
         aTopLeft.iY >= iBufferPos.iY && aTopLeft.iY + iMCUSizeinPixels.iHeight <= iBufferPos.iY + iBufferSizeInPixels.iHeight &&
         aTopLeft.iX >= iBufferPos.iX && aTopLeft.iX + iMCUSizeinPixels.iWidth  <= iBufferPos.iX + iBufferSizeInPixels.iWidth
       )
        {
        iLockedPtr = ShiftPtr(iRgbBuffer, aTopLeft.iX - iBufferPos.iX); 
        return iLockedPtr;
        }
    
    iLockedPtr = reinterpret_cast<TRgbBufferPtr>(iBitmapData) + iBitmapScanlineLength * aTopLeft.iY + KRgbBufferPixelSize * aTopLeft.iX;
    
    if (aTopLeft.iX + iMCUSizeinPixels.iWidth <= iBitmapSize.iWidth &&
        aTopLeft.iY + iMCUSizeinPixels.iHeight <= iBitmapSize.iHeight &&
        KInplaceAccess)
        {
        //ASSERT( iBitmapScanlineLength % KRgbBufferPixelSize == 0 );
        iNextLineOffset = iBitmapScanlineLength;
		iBufferPos.iX = -1;
        }
    else
        {
        const TInt KLineLen  = KRgbBufferPixelSize * Min(iBufferSizeInPixels.iWidth, iBitmapSize.iWidth - aTopLeft.iX);
        const TInt KNumLines = Min(iBufferSizeInPixels.iHeight, iBitmapSize.iHeight - aTopLeft.iY);
        
        const TInt KBufferLineLen = iBufferScanlineLength;
        
        TRgbBufferPtr pixelBufferPtr = iRgbBuffer;
        if (KInplaceAccess)
            {
    	    for (TInt y = KNumLines; y--;  )
                {
    		    Mem::Copy(pixelBufferPtr, iLockedPtr, KLineLen);
    		    iLockedPtr      += iBitmapScanlineLength;
    		    
    		    // now fill out hor trail using grey
    		    const TInt green = pixelBufferPtr[ KLineLen - KRgbBufferPixelSize + 1 ]; // green of the last pixel
    		    for (TInt f = KLineLen; f < KBufferLineLen; ++f)
    		        {
    		        pixelBufferPtr[ f ] = green;
    		        }
    		    pixelBufferPtr = ShiftPtr(pixelBufferPtr, iBufferSizeInPixels.iWidth );
    		    }            
            }
        else
            {
    	    for (TInt y = aTopLeft.iY; y < aTopLeft.iY + KNumLines; y++ )
                {
    		    TPtr8 pixelDes(reinterpret_cast<TUint8*>(pixelBufferPtr), KLineLen, KLineLen );
    		    // GetScanLine() is extremely slow, so if support for bitmap modes other than EColor16M is require
    		    // it shouldn't be used
    		    iBitmap->GetScanLine(pixelDes, TPoint(aTopLeft.iX, y), iBufferSizeInPixels.iWidth, EColor16M);
    		    
    		    // now fill out hor trail using grey
    		    const TInt green = pixelBufferPtr[ KLineLen - KRgbBufferPixelSize + 1 ]; // green of the last pixel
    		    for (TInt f = KLineLen; f < KBufferLineLen; ++f)
    		        {
    		        pixelBufferPtr[ f ] = green;
    		        }
    		    pixelBufferPtr  = ShiftPtr(pixelBufferPtr, iBufferSizeInPixels.iWidth );
                }
    		}
        // fill out vertical trail    		
    	if ( KNumLines < iMCUSizeinPixels.iHeight )
    	    {
        	TUint32* fillBufferPtr = reinterpret_cast<TUint32*>( pixelBufferPtr );
            // we're filling the whole buffer with mid-gray, so to reduce 
            // color entropy of padded block
            pixelBufferPtr  = ShiftPtr(pixelBufferPtr, -iBufferSizeInPixels.iWidth ); // go to the previous line
            // make a grey using green component
            TUint32 fillValue = (pixelBufferPtr[1] | pixelBufferPtr[1]<<8);
            fillValue |= fillValue << 16;
            TInt i =  Coord2Size( iBufferSizeInPixels.iWidth, iBufferSizeInPixels.iHeight - KNumLines) >> 3; // /8
            do 
                {
                *fillBufferPtr++ = fillValue;
                *fillBufferPtr++ = fillValue;
                } while (--i);    	
    	    }
    	iLockedPtr              = iRgbBuffer;
        iNextLineOffset			= iBufferScanlineLength;
        iBufferPos              = aTopLeft;
        }
    
    return iLockedPtr;
    }
    
void CRgbBufferPtr::UnlockBuffer()
    {
    ASSERT( iLockedPtr != NULL );
    iLockedPtr  = NULL;        
    }

