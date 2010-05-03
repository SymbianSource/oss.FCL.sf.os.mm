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

#include "PNGCodec.h"
#include "ImageUtils.h"

const TInt KPngScanlineFilterTypeLength = 1;

const TInt KColStart[KPngNumInterlacedPasses] = { 0, 4, 0, 2, 0, 1, 0, 0 };
const TInt KRowStart[KPngNumInterlacedPasses] = { 0, 0, 4, 0, 2, 0, 1, 0 };
const TInt KColIncrement[KPngNumInterlacedPasses] = { 8, 8, 4, 4, 2, 2, 1, 0 };
const TInt KRowIncrement[KPngNumInterlacedPasses] = { 8, 8, 8, 4, 4, 2, 2, 0 };
const TInt KBlockWidth[KPngNumInterlacedPasses] = { 8, 4, 4, 2, 2, 1, 1, 0 };
const TInt KBlockHeight[KPngNumInterlacedPasses] = { 8, 8, 4, 4, 2, 2, 1, 0 };

const TInt KPngDepth1BytesPerPixel = 1;
const TInt KPngDepth3BytesPerPixel = 3;
const TInt KPngDepth4BytesPerPixel = 4;

// CFastProcessor16MAto16MA
/**This class is a specific implementation of CFastProcessor.
It provides a conversion of 24bpp + 8bpp alpha to EColor16MA display mode.
*/
class CFastProcessor16MAto16MA: public CFastProcessor
    {
public:
	CFastProcessor16MAto16MA(CFbsBitmap* aDestination, TBool aRgbaMode);
    void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos);
    };
   
// CFastProcessor16MAto16MAP
/**This class is a specific implementation of CFastProcessor.
It provides a conversion of 24bpp + 8bpp alpha to EColor16MAP display mode.
*/
class CFastProcessor16MAto16MAP: public CFastProcessor
    {
public:
	CFastProcessor16MAto16MAP(CFbsBitmap* aDestination, TBool aRgbaMode);
    void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos);
    };
   
// CFastProcessor16MtoM
/**This class is a specific implementation of CFastProcessor.
It provides a conversion of 24bpp to EColor16M display mode. 
*/
class CFastProcessor16Mto16M: public CFastProcessor
    {
public:
	CFastProcessor16Mto16M(CFbsBitmap* aDestination, TBool aRgbaMode);
    void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos);
    };

// CFastProcessor16MtoMA
/**This class is a specific implementation of CFastProcessor.
It provides a conversion of 24bpp to EColor16MU, EColor16MA or EColor16MAP display mode. 
*/
class CFastProcessor16Mto16MA: public CFastProcessor
    {
public:
	CFastProcessor16Mto16MA(CFbsBitmap* aDestination, TBool aRgbaMode);
    void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos);
    };

// CFastProcessor16MAto16MU
/**This class is a specific implementation of CFastProcessor.
It provides a conversion of a 24bpp + 8bpp alpha source to a EColor16MU (setting alpha as 0xFF).
*/
class CFastProcessor16MAto16MU: public CFastProcessor
    {
public:
	CFastProcessor16MAto16MU(CFbsBitmap* aDestination, TBool aRgbaMode);
    void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos);
    };

// CFastProcessor32bitTo32bitAndMask
/**This class is a specific implementation of CFastProcessor.
It provides a conversion of a 24bpp + 8bpp alpha source to an opaque 32 bit destination bitmap (i.e.
setting the alpha channel to 0xFF) and EGray256 mask (which contains the alpha channel of the source).
*/
class CFastProcessor32bitTo32bitAndMask: public CFastProcessor
    {
public:
	CFastProcessor32bitTo32bitAndMask(CFbsBitmap* aDestination, CFbsBitmap* aMask, TBool aRgbaMode);
    void SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos);
    };

/**
Constructs a new FastProcessor based on the conversion type.

@param  aImageInfo
        A reference to TPngImageInformation for the FastProcessor to use.
@param  aDestination
        A reference to the destination bitmap.
@param  aRgbaMode
        This flag indicates that MNG frames are being processed.
*/
CFastProcessor* CFastProcessor::NewL(const TPngImageInformation& aImageInfo, CFbsBitmap* aDestination, CFbsBitmap* aMask, TBool aRgbaMode)
    {
	CFastProcessor* self = NULL;

	if (aRgbaMode) //if MNG
		{
		if (aImageInfo.iColorType == TPngImageInformation::EDirectColor)
			{
			self = new (ELeave) CFastProcessor16Mto16MA(aDestination, aRgbaMode);	
			}
		else if(aImageInfo.iColorType == TPngImageInformation::EAlphaDirectColor)
			{
			self = new (ELeave) CFastProcessor16MAto16MA(aDestination, aRgbaMode);	
			}
		else
			{
			User::Leave(KErrNotSupported);	
			}
		}
	else
		{
		TDisplayMode mode = aDestination->DisplayMode();
		
		switch (aImageInfo.iColorType)
			{
			case TPngImageInformation::EDirectColor:
				if(EColor16M == mode)
					{
					self = new (ELeave) CFastProcessor16Mto16M(aDestination, aRgbaMode);	
					}
				else if(EColor16MAP == mode || EColor16MA == mode || EColor16MU == mode)
					{
					self = new (ELeave) CFastProcessor16Mto16MA(aDestination, aRgbaMode);	
					}
				break;

			case TPngImageInformation::EAlphaDirectColor:
				if(aMask && (EColor16MA == mode || EColor16MU == mode || EColor16MAP == mode))
					{
					self = new (ELeave) CFastProcessor32bitTo32bitAndMask(aDestination, aMask, aRgbaMode);
					}
				else if(EColor16MA == mode)
					{
					self = new (ELeave) CFastProcessor16MAto16MA(aDestination, aRgbaMode);	
					}
				else if(EColor16MAP == mode)
					{
					self = new (ELeave) CFastProcessor16MAto16MAP(aDestination, aRgbaMode);	
					}
				else if(EColor16MU == mode)
					{
					self = new (ELeave) CFastProcessor16MAto16MU(aDestination, aRgbaMode);
					}
				else
					{
					User::Leave(KErrNotSupported);	
					}
				break;
		
			default:
				User::Leave(KErrNotSupported);
				break;				
			}		
		}
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;	   
    }

/**
Destructor for this class.
Releases the lock for the current bitmap.
*/
CFastProcessor::~CFastProcessor()
    {
    }

/**
Second phase constructor.
Requests a lock for the current bitmap.
*/    
void CFastProcessor::ConstructL()
	{
	// NO-OP - reserved for changes at a later stage.
	}

/**
Default constructor 
*/
CFastProcessor::CFastProcessor(CFbsBitmap* aDestination, CFbsBitmap* aMask, TBool aRgbaMode)
    {
    iRgbaMode = aRgbaMode;
    
    if (!aRgbaMode)
	    {
	    iBitmap = aDestination;
	    iBitmapSize = iBitmap->SizeInPixels();	
		if (aMask)
			{
			ASSERT(aMask->SizeInPixels() == iBitmapSize);
			iMask = aMask;
			}
		}    	
	}

/**
Requests a lock for the current bitmap from the font & bitmap server and
sets the current position in the bitmap to the first pixel.
*/
void CFastProcessor::Begin()
	{
	if (!iRgbaMode)
		{
		if (iBitmapBuffer==NULL)
			{
			iBitmapBuffer = reinterpret_cast<TUint8*>( iBitmap->DataAddress() );
     		if (iMask)
     			{
     			iMaskBuffer = reinterpret_cast<TUint8*>( iMask->DataAddress() );
     			}
			}
		}
	}

/**
Releases a lock previously acquired using CFastProcessor::Begin().
*/
void CFastProcessor::End()
	{
	if (!iRgbaMode)
		{    
		if (iBitmapBuffer)
			{
			iBitmapBuffer = NULL;
			}
		if (iMaskBuffer)
			{
			iMaskBuffer = NULL;
			}
		}
	}

/**
Default constructor for this class.
*/
CFastProcessor16Mto16M::CFastProcessor16Mto16M(CFbsBitmap* aDestination, TBool aRgbaMode):
	CFastProcessor(aDestination, NULL, aRgbaMode)
    {}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aDataPtr.

@param  aDataPtr
        A pointer to the first element in the array.
@param  aDataPtrLimit
        A pointer to the last element in the array.
*/
void CFastProcessor16Mto16M::SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* /*aLineCache*/, TPoint& /*aPos*/)
    {       
    TUint8* scanLinePtr = (iBitmapBuffer + KPngDepth3BytesPerPixel * ( iPos.iX +  iPos.iY * (( (iBitmapSize.iWidth + 3)>>2)<<2))); //aligns the specified value onto a 4-byte boundary.
       
    iPos.iX += TUint(aDataPtrLimit - aDataPtr) / KPngDepth3BytesPerPixel;

    while (aDataPtr < aDataPtrLimit)
        {
        scanLinePtr[0] = aDataPtr[2];
        scanLinePtr[1] = aDataPtr[1];
        scanLinePtr[2] = aDataPtr[0];
      	
 		scanLinePtr += KPngDepth3BytesPerPixel;
        aDataPtr += KPngDepth3BytesPerPixel;
        }

    while (iPos.iX >= iBitmapSize.iWidth)
        {
        iPos.iY++;
        iPos.iX -= iBitmapSize.iWidth;
        }
    }

/**
Default constructor for this class.
*/
CFastProcessor16Mto16MA::CFastProcessor16Mto16MA(CFbsBitmap* aDestination, TBool aRgbaMode):
	CFastProcessor(aDestination, NULL, aRgbaMode)
    {}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aDataPtr.

@param  aDataPtr
        A pointer to the first element in the array.
@param  aDataPtrLimit
        A pointer to the last element in the array.
//used only if RgbaMode i.e MNG processing        
@param  aLineCache
        A pointer to current scanline buffer.
@param  aPos
        Current pixel position.
*/
void CFastProcessor16Mto16MA::SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos)
    {
    TUint32* scanLinePtr;
 
    if (!iRgbaMode)
	    {
		scanLinePtr = (TUint32*)(iBitmapBuffer + (KPngDepth3BytesPerPixel + 1) * ( iPos.iX +  iPos.iY * iBitmapSize.iWidth));
		iPos.iX += TUint(aDataPtrLimit - aDataPtr) / KPngDepth3BytesPerPixel;
	    }
	else
		{
		scanLinePtr = reinterpret_cast<TUint32*>(aLineCache);
		aPos.iX = TUint(aDataPtrLimit - aDataPtr) / KPngDepth3BytesPerPixel;
		}
    
    while (aDataPtr < aDataPtrLimit)
        {
        *scanLinePtr++ = ((TUint32)0xFF << 24) | ((TUint32)aDataPtr[0] << 16) | ((TUint32)aDataPtr[1] << 8) | aDataPtr[2]; 

        aDataPtr += KPngDepth3BytesPerPixel;
        }
        	
	if (!iRgbaMode)
		{
	    while (iPos.iX >= iBitmapSize.iWidth)
	        {
	        iPos.iY++;
	        iPos.iX -= iBitmapSize.iWidth;
	        }              
		}
    }

/**
Default constructor for this class.
*/
CFastProcessor16MAto16MA::CFastProcessor16MAto16MA(CFbsBitmap* aDestination, TBool aRgbaMode):
	CFastProcessor(aDestination, NULL, aRgbaMode)
    {}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aDataPtr.

@param  aDataPtr
        A pointer to the first element in the array.
@param  aDataPtrLimit
        A pointer to the last element in the array.
//used only if RgbaMode i.e MNG processing        
@param  aLineCache
        A pointer to current scanline buffer.
@param  aPos
        Current pixel position.
*/
void CFastProcessor16MAto16MA::SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos)
    {
    TUint32* scanLinePtr;

    if (!iRgbaMode)
	    {
    	scanLinePtr = (TUint32*)(iBitmapBuffer + KPngDepth4BytesPerPixel * ( iPos.iX + iPos.iY * iBitmapSize.iWidth));
    	iPos.iX += TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;	    	
	    }
    else
	    {
		scanLinePtr = reinterpret_cast<TUint32*>(aLineCache);
		aPos.iX = TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;	    	
	    }

    while (aDataPtr + sizeof(TUint32) < aDataPtrLimit)
        {       
        TUint32 pixel1 = *(TUint32*) aDataPtr;
        aDataPtr += KPngDepth4BytesPerPixel;
        TUint32 pixel2 = *(TUint32*) aDataPtr;
        aDataPtr += KPngDepth4BytesPerPixel;

        pixel1 = (pixel1 & 0xFF00FF00) | ((pixel1 & 0xFF)<<16) | ((pixel1>>16) & 0xFF);
        pixel2 = (pixel2 & 0xFF00FF00) | ((pixel2 & 0xFF)<<16) | ((pixel2>>16) & 0xFF);

        *scanLinePtr++ = pixel1;
        *scanLinePtr++ = pixel2;
        }
        
    if (aDataPtr + sizeof(TUint32) == aDataPtrLimit)
    	{
    	TUint32 pixel1 = *(TUint32*) aDataPtr;
    	
    	*scanLinePtr = (pixel1 & 0xFF00FF00) | ((pixel1 & 0xFF)<<16) | ((pixel1>>16) & 0xFF);
    	}        

	if (!iRgbaMode)
		{
	    while (iPos.iX >= iBitmapSize.iWidth)
	        {
	        iPos.iY++;
	        iPos.iX -= iBitmapSize.iWidth;
	        }		
		}
    }

/**
Default constructor for this class.
*/
CFastProcessor16MAto16MAP::CFastProcessor16MAto16MAP(CFbsBitmap* aDestination, TBool aRgbaMode):
	CFastProcessor(aDestination, NULL, aRgbaMode)
    {}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aDataPtr.

@param  aDataPtr
        A pointer to the first element in the array.
@param  aDataPtrLimit
        A pointer to the last element in the array.
//used only if RgbaMode i.e MNG processing        
@param  aLineCache
        A pointer to current scanline buffer.
@param  aPos
        Current pixel position.
*/

// function to convert PNG pixel to 16MAP pixel

static TUint32 PngTo16Map(TUint32 aPngPixel)
	{
	TUint8 const alpha = TUint8( aPngPixel >> 24 );

	if (alpha == 0)
		{
		aPngPixel = 0;
		}
	else
		{
		// PNG bytes position in TUint32 are ABGR and must be coverted to ARGB in our case

		if (alpha == 0xff)
			{
			aPngPixel = (aPngPixel & 0xFF00FF00) | ((aPngPixel & 0xFF) << 16) | ((aPngPixel >> 16) & 0xFF);
			}
		else
			{   
			// Use a bias value of 128 rather than 255, but also add 1/256 of the numerator 
			// before dividing the sum by 256.

			TUint32 scaledRB = (aPngPixel & KRBMask) * alpha + KRBBias;
			scaledRB = (scaledRB + ( (scaledRB >> 8) & KRBMask) ) >> 8;
			// swap now the R & B channels
			scaledRB = (scaledRB << 16) | (scaledRB >> 16);

			TUint32 scaledG = (aPngPixel & KGMask) * alpha + KGBias;
			scaledG = (scaledG + (scaledG >> 8)) >> 8;

			// compose the new pixel swapping R with B as we premultiplied alpha on a PNG pixel 

			aPngPixel = (aPngPixel & KAMask) | (scaledRB & KRBMask) | (scaledG & KGMask); 
			}
		}

	return aPngPixel;
	}

void CFastProcessor16MAto16MAP::SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos)
    {
    TUint32* scanLinePtr;

	if (!iRgbaMode)
		{
		scanLinePtr = (TUint32*)(iBitmapBuffer + KPngDepth4BytesPerPixel * ( iPos.iX +  iPos.iY * iBitmapSize.iWidth));
		iPos.iX += TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;      
		}
	else
		{
		scanLinePtr = reinterpret_cast<TUint32*>(aLineCache);
		aPos.iX = TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;      
		}

	// perform a direct conversion PNG -> Pixel 16MAP

    while (aDataPtr + sizeof(TUint32) < aDataPtrLimit)
		{
		TUint32 pixel1 = *(TUint32*) aDataPtr;
		aDataPtr += KPngDepth4BytesPerPixel;
		TUint32 pixel2 = *(TUint32*) aDataPtr;
		aDataPtr += KPngDepth4BytesPerPixel;

		pixel1 = PngTo16Map(pixel1);
		pixel2 = PngTo16Map(pixel2);

		*scanLinePtr++ = pixel1;
		*scanLinePtr++ = pixel2;
		}

    if (aDataPtr + sizeof(TUint32) == aDataPtrLimit)
		{
		*scanLinePtr = PngTo16Map(*(TUint32*) aDataPtr);
		}

 	if (!iRgbaMode)
  		{
		while (iPos.iX >= iBitmapSize.iWidth)
			{
			iPos.iY++;
			iPos.iX -= iBitmapSize.iWidth;
			}  
		}
    }

/**
Default constructor for this class.
*/
CFastProcessor32bitTo32bitAndMask::CFastProcessor32bitTo32bitAndMask(CFbsBitmap* aDestination, CFbsBitmap* aMask, TBool aRgbaMode)
: CFastProcessor(aDestination, aMask, aRgbaMode)
	{}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aDataPtr.

@param  aDataPtr
        A pointer to the first element in the array.
@param  aDataPtrLimit
        A pointer to the last element in the array.
//used only if RgbaMode i.e MNG processing        
@param  aLineCache
        A pointer to current scanline buffer.
@param  aPos
        Current pixel position.
*/
void CFastProcessor32bitTo32bitAndMask::SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* /*aLineCache*/, TPoint& /*aPos*/)
	{
	ASSERT(!iRgbaMode); // Not configured for MNG use
	
	TUint32* scanLinePtr;  // alpha channel set to fully opaque
	TUint8* maskScanLinePtr;  // set alpha channel in mask

	scanLinePtr = (TUint32*)(iBitmapBuffer + KPngDepth4BytesPerPixel * ( iPos.iX +  iPos.iY * iBitmapSize.iWidth));
	maskScanLinePtr = (TUint8*)(iMaskBuffer + KPngDepth1BytesPerPixel * ( iPos.iX +  iPos.iY * iBitmapSize.iWidth));
	iPos.iX += TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;
	
	if (iBitmap->DisplayMode() == EColor16MAP)
		{
	    while (aDataPtr + sizeof(TUint32) < aDataPtrLimit)
			{
			TUint32 pixel1 = *(TUint32*) aDataPtr;
			aDataPtr += KPngDepth4BytesPerPixel;
			TUint32 pixel2 = *(TUint32*) aDataPtr;
			aDataPtr += KPngDepth4BytesPerPixel;

			pixel1 = PngTo16Map(pixel1);
			pixel2 = PngTo16Map(pixel2);
			
			*scanLinePtr++ = 0xFF000000 | pixel1;
			*maskScanLinePtr++ = (pixel1 & 0xFF000000) >> 24;
			
			*scanLinePtr++ = 0xFF000000 | pixel2;
			*maskScanLinePtr++ = (pixel2 & 0xFF000000) >> 24;
			}
		
		if (aDataPtr + sizeof(TUint32) == aDataPtrLimit)
			{
			TUint32 oddPixel = *(TUint32*) aDataPtr;
			
			oddPixel = PngTo16Map(oddPixel);
			
			*scanLinePtr++ = 0xFF000000 | oddPixel;
			*maskScanLinePtr++ = (oddPixel & 0xFF000000) >> 24;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			*scanLinePtr++ = 0xFF000000 | (aDataPtr[0] << 16) | (aDataPtr[1] << 8) | aDataPtr[2];
			*maskScanLinePtr++ = (aDataPtr[3]);

			aDataPtr += KPngDepth4BytesPerPixel;
			}
		}
	
	while (iPos.iX >= iBitmapSize.iWidth)
		{
		iPos.iY++;
		iPos.iX -= iBitmapSize.iWidth;
		}
	}

/**
Default constructor for this class.
*/
CFastProcessor16MAto16MU::CFastProcessor16MAto16MU(CFbsBitmap* aDestination, TBool aRgbaMode)
: CFastProcessor(aDestination, NULL, aRgbaMode)
	{}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aDataPtr.

@param  aDataPtr
        A pointer to the first element in the array.
@param  aDataPtrLimit
        A pointer to the last element in the array.
//used only if RgbaMode i.e MNG processing        
@param  aLineCache
        A pointer to current scanline buffer.
@param  aPos
        Current pixel position.
*/
void CFastProcessor16MAto16MU::SetPixels(const TUint8* aDataPtr, const TUint8* aDataPtrLimit, TRgb* aLineCache, TPoint& aPos)
    {
    TUint32* scanLinePtr;

    if (!iRgbaMode)
	    {
    	scanLinePtr = (TUint32*)(iBitmapBuffer + KPngDepth4BytesPerPixel * ( iPos.iX + iPos.iY * iBitmapSize.iWidth));
    	iPos.iX += TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;	    	
	    }
    else
	    {
		scanLinePtr = reinterpret_cast<TUint32*>(aLineCache);
		aPos.iX = TUint(aDataPtrLimit - aDataPtr) / KPngDepth4BytesPerPixel;	    	
	    }

    while (aDataPtr < aDataPtrLimit)
        {
        *scanLinePtr++ = 0xFF000000 | (aDataPtr[0] << 16) | (aDataPtr[1] << 8) | aDataPtr[2]; 

        aDataPtr += KPngDepth4BytesPerPixel;
        }       

	if (!iRgbaMode)
		{
	    while (iPos.iX >= iBitmapSize.iWidth)
	        {
	        iPos.iY++;
	        iPos.iX -= iBitmapSize.iWidth;
	        }		
		}
    }

class CBitDepth1Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth2Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth4Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth8Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth8ColorType2Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth8ColorType4Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth8ColorType6Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth16ColorType0Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth16ColorType2Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth16ColorType4Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

class CBitDepth16ColorType6Decoder : public CPngReadSubCodec
	{
private:
	virtual void DoConstructL();
	virtual TInt ScanlineBufferSize(TInt aPixelLength);
	virtual void DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit);
	};

// CPngReadSubCodec
CPngReadSubCodec* CPngReadSubCodec::NewL(CImageProcessor* aImageProc,CImageProcessor* aMaskProc,const TPngImageInformation& aInfo, CFastProcessor* aFastProc, TBool aFastProcessorMode)
	{
	CPngReadSubCodec* self = NULL;

	switch (aInfo.iBitDepth)
		{
	case 1:
		self = new(ELeave) CBitDepth1Decoder;
		break;
	case 2:
		self = new(ELeave) CBitDepth2Decoder;
		break;
	case 4:
		self = new(ELeave) CBitDepth4Decoder;
		break;
	case 8:
		switch (aInfo.iColorType)
			{
		case TPngImageInformation::EGrayscale:
		case TPngImageInformation::EIndexedColor:
			self = new(ELeave) CBitDepth8Decoder;
			break;
		case TPngImageInformation::EDirectColor:
			self = new(ELeave) CBitDepth8ColorType2Decoder;
			break;
		case TPngImageInformation::EAlphaGrayscale:
			self = new(ELeave) CBitDepth8ColorType4Decoder;
			break;
		case TPngImageInformation::EAlphaDirectColor:
			self = new(ELeave) CBitDepth8ColorType6Decoder;
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
			}
		break;
	case 16:
		switch (aInfo.iColorType)
			{
		case TPngImageInformation::EGrayscale:
			self = new(ELeave) CBitDepth16ColorType0Decoder;
			break;
		case TPngImageInformation::EDirectColor:
			self = new(ELeave) CBitDepth16ColorType2Decoder;
			break;
		case TPngImageInformation::EAlphaGrayscale:
			self = new(ELeave) CBitDepth16ColorType4Decoder;
			break;
		case TPngImageInformation::EAlphaDirectColor:
			self = new(ELeave) CBitDepth16ColorType6Decoder;
			break;
		case TPngImageInformation::EIndexedColor:
		default:
			User::Leave(KErrNotSupported);
			break;
			}
		break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}

	CleanupStack::PushL(self);
	self->ConstructL(aImageProc,aMaskProc,aInfo, aFastProc, aFastProcessorMode);
	CleanupStack::Pop(self); 
	return self;
	}

CPngReadSubCodec::CPngReadSubCodec():
	iScanlineDes1(NULL,0),
	iScanlineDes2(NULL,0)
	{}

CPngReadSubCodec::~CPngReadSubCodec()
	{
	delete iScanlineBuffer1;
	delete iScanlineBuffer2;
	delete [] iLineCache;
	}

void CPngReadSubCodec::ConstructL(CImageProcessor* aImageProc,CImageProcessor* aMaskProc,const TPngImageInformation& aInfo, CFastProcessor* aFastProc, TBool aFastProcessorMode)
	{
	iImageProc = aImageProc;
	iMaskProc = aMaskProc;
	iFastProc = aFastProc;
	
	iInfo = aInfo;
	
	SetFastProcessorMode(aFastProcessorMode);				

	iScanlineBufferSize = ScanlineBufferSize(iInfo.iSize.iWidth);

	DoConstructL();
	if (iInfo.iInterlaceMethod != TPngImageInformation::EAdam7Interlace)
		{
		iLineCache = new (ELeave) TRgb [iInfo.iSize.iWidth + 8]; // +8 to be sure we won't exceed buffer for padded images (up to 8 pixels padding)
		}

	iScanlineBuffer1 = HBufC8::NewMaxL(iScanlineBufferSize + 7);
	iScanlineBuffer2 = HBufC8::NewMaxL(iScanlineBufferSize + 7);

	if (iInfo.iInterlaceMethod == TPngImageInformation::EAdam7Interlace)
		{
		iInterlacedScanlineBufferSize[0] = ScanlineBufferSize((iInfo.iSize.iWidth + 7) >> 3);
		iInterlacedScanlineBufferSize[1] = ScanlineBufferSize((iInfo.iSize.iWidth + 3) >> 3);
		iInterlacedScanlineBufferSize[2] = ScanlineBufferSize((iInfo.iSize.iWidth + 3) >> 2);
		iInterlacedScanlineBufferSize[3] = ScanlineBufferSize((iInfo.iSize.iWidth + 1) >> 2);
		iInterlacedScanlineBufferSize[4] = ScanlineBufferSize((iInfo.iSize.iWidth + 1) >> 1);
		iInterlacedScanlineBufferSize[5] = ScanlineBufferSize(iInfo.iSize.iWidth >> 1);
		iInterlacedScanlineBufferSize[6] = iScanlineBufferSize;
		iInterlacedScanlineBufferSize[7] = 0;
		iPass = 0;

		iScanlineDes1.Set(&(iScanlineBuffer1->Des())[0],iInterlacedScanlineBufferSize[0],iInterlacedScanlineBufferSize[0]);
		iScanlineDes2.Set(&(iScanlineBuffer2->Des())[0],iInterlacedScanlineBufferSize[0],iInterlacedScanlineBufferSize[0]);
		
		if(iImageProc)
			{
			const TInt lineRepeat = ClampValue(KBlockHeight[iPass]-1,0,iInfo.iSize.iHeight-iPos.iY-2);
			iImageProc->SetLineRepeat(lineRepeat);	
			}
		}
	else
		{
		// to align actual data per word boudary
		iScanlineDes1.Set(&(iScanlineBuffer1->Des())[3],iScanlineBufferSize,iScanlineBufferSize);
		iScanlineDes2.Set(&(iScanlineBuffer2->Des())[3],iScanlineBufferSize,iScanlineBufferSize);
		}
	}

void CPngReadCodec::SetImageProcessor(CImageProcessor* aImageProc, TBool aOwnsProcessor)
	{
	if (iOwnsImageProcessor)
		delete iImageProc;
	iImageProc = aImageProc;
	iOwnsImageProcessor = aOwnsProcessor;
	}

void CPngReadCodec::SetMaskProcessor(CImageProcessor* aMaskProc, TBool aOwnsProcessor)
	{
	if (iOwnsMaskProcessor)
		delete iMaskProc;
	iMaskProc = aMaskProc;
	iOwnsMaskProcessor = aOwnsProcessor;
	}

void CPngReadCodec::SetFastProcessor(CFastProcessor* aFastProc, TBool aOwnsProcessor)
	{
	if (iOwnsFastProcessor)
		delete iFastProc;
	iFastProc = aFastProc;
	iOwnsFastProcessor = aOwnsProcessor;		
	}

void CPngReadSubCodec::SetFastProcessorMode(TBool aMode)
	{
	iFastProcessorMode = aMode;
	}

void CPngReadSubCodec::ResetL()
	{
	iPos.SetXY(0,0);
	iPass = 0;
	if (iInfo.iInterlaceMethod == TPngImageInformation::EAdam7Interlace)
		{
		iScanlineDes1.Set(&(iScanlineBuffer1->Des())[0],iInterlacedScanlineBufferSize[0],iInterlacedScanlineBufferSize[0]);
		iScanlineDes2.Set(&(iScanlineBuffer2->Des())[0],iInterlacedScanlineBufferSize[0],iInterlacedScanlineBufferSize[0]);
	
		if(iImageProc)
			{
			const TInt lineRepeat = ClampValue(KBlockHeight[iPass]-1,0,iInfo.iSize.iHeight-iPos.iY-2);
			iImageProc->SetLineRepeat(lineRepeat);
			}
		}
	}

TDes8& CPngReadSubCodec::FirstBuffer()
	{
	iScanlineDes1.FillZ();
	iCurrentScanlineBuffer = 2;
	return iScanlineDes2;
	}

TDes8& CPngReadSubCodec::DecodeL()
	{
	TUint8* dataPtr = (iCurrentScanlineBuffer == 1) ? &iScanlineDes1[1] : &iScanlineDes2[1];
	const TUint8* dataPtrLimit = dataPtr + iScanlineDes1.Length() - 1;

	FilterScanlineDataL(dataPtr,dataPtrLimit);
	
	if (iFastProcessorMode)
		{
		iFastProc->Begin();	
		}
	
	DoDecode(dataPtr,dataPtrLimit);

	if (iFastProcessorMode)
		{
		iFastProc->End();	
		}
	
	UpdatePos();
	
	if (iCurrentScanlineBuffer == 1)
		{
		iCurrentScanlineBuffer = 2;
		return iScanlineDes2;
		}
	else
		{
		iCurrentScanlineBuffer = 1;
		return iScanlineDes1;
		}
	}

void CPngReadSubCodec::FilterScanlineDataL(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	TInt filterType = (iCurrentScanlineBuffer == 1) ? iScanlineDes1[0] : iScanlineDes2[0];

	switch (filterType)
		{
	case 0: // None
		break;
	case 1: // Sub
		{
		aDataPtr += iBytesPerPixel;

		while (aDataPtr < aDataPtrLimit)
			{
			aDataPtr[0] = TUint8(aDataPtr[0] + aDataPtr[-iBytesPerPixel]);
			aDataPtr++;
			}
		}
		break;
	case 2: // Up
		{
		TUint8* altDataPtr = (iCurrentScanlineBuffer == 1) ? &iScanlineDes2[1] : &iScanlineDes1[1];

		while (aDataPtr < aDataPtrLimit)
			{
			*aDataPtr = TUint8(*aDataPtr + *altDataPtr);
			aDataPtr++;
			altDataPtr++;
			}
		}
		break;
	case 3: // Average
		{
		const TUint8* tempDataPtrLimit = Min<const TUint8*>(aDataPtr + iBytesPerPixel,aDataPtrLimit);
		TUint8* altDataPtr = (iCurrentScanlineBuffer == 1) ? &iScanlineDes2[1] : &iScanlineDes1[1];

		while (aDataPtr < tempDataPtrLimit)
			{
			aDataPtr[0] = TUint8(aDataPtr[0] + (altDataPtr[0] / 2));
			aDataPtr++;
			altDataPtr++;
			}

		while (aDataPtr < aDataPtrLimit)
			{
			aDataPtr[0] = TUint8(aDataPtr[0] + ((altDataPtr[0] + aDataPtr[-iBytesPerPixel]) / 2));
			aDataPtr++;
			altDataPtr++;
			}
		}
		break;
	case 4: // Paeth
		{
		const TUint8* tempDataPtrLimit = Min<const TUint8*>(aDataPtr + iBytesPerPixel,aDataPtrLimit);
		TUint8* altDataPtr = (iCurrentScanlineBuffer == 1) ? &iScanlineDes2[1] : &iScanlineDes1[1];

		while (aDataPtr < tempDataPtrLimit)
			{
			aDataPtr[0] = TUint8(aDataPtr[0] + altDataPtr[0]);
			aDataPtr++;
			altDataPtr++;
			}

		while (aDataPtr < aDataPtrLimit)
			{
			aDataPtr[0] = TUint8(aDataPtr[0] + PaethPredictor(aDataPtr[-iBytesPerPixel],altDataPtr[0],altDataPtr[-iBytesPerPixel]));
			aDataPtr++;
			altDataPtr++;
			}
		}
		break;

	case 64: // support for additional MNG-defined filter (Adaptive filtering with five basic types and intrapixel differencing)
		{
		const TInt plusAlpha=(0 != (iInfo.iColorType & TPngImageInformation::EAlphaChannelUsed));
		if ( iInfo.iBitDepth == 16)
			{
			const TUint KBytesPerPixel=2*(3+plusAlpha);
			aDataPtrLimit-=KBytesPerPixel;
			while(aDataPtr < aDataPtrLimit)
				{
				const TUint32 s0   = (aDataPtr[0] << 8) | aDataPtr[1];
				const TUint32 s1   = 0x10000u + (aDataPtr[2] << 8) | aDataPtr[3];
				const TUint32 s2   = (aDataPtr[4] << 8) | aDataPtr[5];
				const TUint32 red  = ((s0+s1) & 0xffffu);
				const TUint32 blue = ((s2+s1) & 0xffffu);
				aDataPtr[0] = TUint8((red >> 8) & 0xff);
				aDataPtr[1] = TUint8(red & 0xff);
				aDataPtr[4] = TUint8((blue >> 8) & 0xff);
				aDataPtr[5] = TUint8(blue & 0xff);
				aDataPtr+=KBytesPerPixel;
				}
			}
		else if (iInfo.iBitDepth == 8)
			{
			const TUint KBytesPerPixel=3+plusAlpha;
			aDataPtrLimit-=KBytesPerPixel;
			while(aDataPtr < aDataPtrLimit)
				{
				aDataPtr[0] = TUint8((0x100u + aDataPtr[0] + aDataPtr[1])&0xffu);
				aDataPtr[1] = TUint8((0x100u + aDataPtr[2] + aDataPtr[1])&0xffu);
				aDataPtr +=KBytesPerPixel;
				}
			}
		}
		break;

	default: // Error
		User::Leave(KErrCorrupt);
		break;
		}
	}

TInt CPngReadSubCodec::PaethPredictor(TInt aLeft,TInt aAbove,TInt aAboveLeft)
	{
	TInt p = aLeft + aAbove - aAboveLeft;
	TInt pa = Abs(p - aLeft);
	TInt pb = Abs(p - aAbove);
	TInt pc = Abs(p - aAboveLeft);

	if (pa <= pb && pa <= pc)
		return aLeft;
	else if (pb <= pc)
		return aAbove;
	else
		return aAboveLeft;
	}

void CPngReadSubCodec::WritePixel(TRgb aPixelColor)
	{
	if (iInfo.iInterlaceMethod == TPngImageInformation::EAdam7Interlace)
		{
		const TInt width = ClampValue(KBlockWidth[iPass],0,iInfo.iSize.iWidth - iPos.iX);
		TPoint pos(iPos);
		iImageProc->SetPos(pos);
		iImageProc->SetPixelRun(aPixelColor,width);
		iPos.iX += KColIncrement[iPass];
		}
	else
		{
		if (iRgbaMode)
			{
			iLineCache[iPos.iX++]=aPixelColor;
			}
		else
			{
			iImageProc->SetPixel(aPixelColor);
			}
		}
	}

void CPngReadSubCodec::WritePixel(TRgb aPixelColor,TUint8 aAlphaValue)
	{
	if(iAlphaMode || iRgbaMode)
		{
		TRgb RgbaColour( aPixelColor.Internal()&0xFFFFFF, (TUint32(aAlphaValue)) );
		
		if (iInfo.iInterlaceMethod == TPngImageInformation::EAdam7Interlace)
			{
			iImageProc->SetPos(iPos);
			iImageProc->SetPixel(RgbaColour);
			iPos.iX += KColIncrement[iPass];
			}
		else
			{
			iRgbaMode ? iLineCache[iPos.iX++]=RgbaColour : iImageProc->SetPixel(RgbaColour);
			}
		}
	else
		{
		ASSERT(iMaskProc);
			{
			TRgb maskColor(TRgb::Gray256(aAlphaValue));
		
			if (iInfo.iInterlaceMethod == TPngImageInformation::EAdam7Interlace)
				{
				iImageProc->SetPos(iPos);
				iMaskProc->SetPos(iPos);
				iImageProc->SetPixel(aPixelColor);
				iMaskProc->SetPixel(maskColor);

				iPos.iX += KColIncrement[iPass];
				}
			else
				{
				iImageProc->SetPixel(aPixelColor);
				iMaskProc->SetPixel(maskColor);
				}	
			}
		}
	}

void CPngReadSubCodec::UpdatePos()
	{
	if (iInfo.iInterlaceMethod == TPngImageInformation::EAdam7Interlace)
		{
		ASSERT(iPass <= 7);

		iPos.iX = KColStart[iPass];
		iPos.iY += KRowIncrement[iPass];

		while (iPos.iX >= iInfo.iSize.iWidth || iPos.iY >= iInfo.iSize.iHeight)
			{
			iPass++;
			
			/* Coverity may flag this up as an overrun of KColStart and KRowStart.  This is a false
			positive because both arrays have a 'safety entry' at index [7], which is 0.  Thus,
			iPos.iX and iPos.iY will be be 0, and so never equal to the image width/height as we
			won't decode images with these dimensions.  Therefore, this loop will never be entered.*/
			
			iPos.iX = KColStart[iPass];
			iPos.iY = KRowStart[iPass];
			iScanlineDes1.Set(&(iScanlineBuffer1->Des())[0],iInterlacedScanlineBufferSize[iPass],iInterlacedScanlineBufferSize[iPass]);
			iScanlineDes2.Set(&(iScanlineBuffer2->Des())[0],iInterlacedScanlineBufferSize[iPass],iInterlacedScanlineBufferSize[iPass]);
			iScanlineDes1.FillZ();
			iScanlineDes2.FillZ();
			}

		if(iImageProc)
			{
			const TInt lineRepeat = ClampValue(KBlockHeight[iPass]-1,0,iInfo.iSize.iHeight-iPos.iY-2);
			iImageProc->SetLineRepeat(lineRepeat);
			}
		}
	else
		{
		if (iRgbaMode)
			{
			iImageProc->SetPixels(iLineCache, iPos.iX);
			}
		iPos.iX=0;
		}
	}


// CBitDepth1Decoder
void CBitDepth1Decoder::DoConstructL()
	{
	if (!(iInfo.iColorType & TPngImageInformation::EPaletteUsed))
		{ // Set up palette to be grayscale values
		iInfo.iPalette[0] = KRgbBlack;
		iInfo.iPalette[1] = KRgbWhite;


		if (iInfo.iTransparencyPresent)
			{
			if (iInfo.iTransparentGray <= 1)
				iInfo.iTransparencyValue[iInfo.iTransparentGray] = 0;
			}
		}

	// Replicate values to avoid shifts when decoding
	iInfo.iPalette[2] = iInfo.iPalette[1];
	iInfo.iPalette[4] = iInfo.iPalette[1];
	iInfo.iPalette[8] = iInfo.iPalette[1];
	iInfo.iPalette[16] = iInfo.iPalette[1];
	iInfo.iPalette[32] = iInfo.iPalette[1];
	iInfo.iPalette[64] = iInfo.iPalette[1];
	iInfo.iPalette[128] = iInfo.iPalette[1];

	if (iInfo.iTransparencyPresent && iInfo.iTransparencyValue[1] != 255)
		{
		iInfo.iTransparencyValue[2] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[4] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[8] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[16] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[32] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[64] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[128] = iInfo.iTransparencyValue[1];
		}

	iBytesPerPixel = 1;
	if (iInfo.iInterlaceMethod == TPngImageInformation::ENoInterlace)
		{
		TInt pixelPadding = ((iInfo.iSize.iWidth + 7) & ~7) - iInfo.iSize.iWidth;
		if(iImageProc)
			{
			iImageProc->SetPixelPadding(pixelPadding);	
			}
		if (iMaskProc)
			{
			iMaskProc->SetPixelPadding(pixelPadding);	
			}
		}
	}

TInt CBitDepth1Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return ((aPixelLength + 7) / 8) + KPngScanlineFilterTypeLength;
	}

void CBitDepth1Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt dataValue = *aDataPtr++;

			for (TUint mask=0x80; mask!=0; mask>>=1) // iterate with 0x80, 0x40 .. 0x01
				WritePixel(iInfo.iPalette[dataValue & mask],iInfo.iTransparencyValue[dataValue & mask]);
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt dataValue = *aDataPtr++;

			for (TUint mask=0x80; mask!=0; mask>>=1) // iterate with 0x80, 0x40 .. 0x01
				WritePixel(iInfo.iPalette[dataValue & mask]);
			}
		}
	}


// CBitDepth2Decoder
void CBitDepth2Decoder::DoConstructL()
	{
	if (!(iInfo.iColorType & TPngImageInformation::EPaletteUsed))
		{ // Set up palette to be grayscale values
		iInfo.iPalette[0] = KRgbBlack;
		iInfo.iPalette[1] = KRgbDarkGray;
		iInfo.iPalette[2] = KRgbGray;
		iInfo.iPalette[3] = KRgbWhite;

		if (iInfo.iTransparencyPresent)
			{
			if (iInfo.iTransparentGray <= 3)
				iInfo.iTransparencyValue[iInfo.iTransparentGray] = 0;
			}
		}

	// Replicate values to avoid shifts when decoding
	iInfo.iPalette[4] = iInfo.iPalette[1];
	iInfo.iPalette[8] = iInfo.iPalette[2];
	iInfo.iPalette[12] = iInfo.iPalette[3];

	iInfo.iPalette[16] = iInfo.iPalette[1];
	iInfo.iPalette[32] = iInfo.iPalette[2];
	iInfo.iPalette[48] = iInfo.iPalette[3];

	iInfo.iPalette[64] = iInfo.iPalette[1];
	iInfo.iPalette[128] = iInfo.iPalette[2];
	iInfo.iPalette[192] = iInfo.iPalette[3];

	if (iInfo.iTransparencyPresent)
		{
		iInfo.iTransparencyValue[4] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[8] = iInfo.iTransparencyValue[2];
		iInfo.iTransparencyValue[12] = iInfo.iTransparencyValue[3];

		iInfo.iTransparencyValue[16] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[32] = iInfo.iTransparencyValue[2];
		iInfo.iTransparencyValue[48] = iInfo.iTransparencyValue[3];

		iInfo.iTransparencyValue[64] = iInfo.iTransparencyValue[1];
		iInfo.iTransparencyValue[128] = iInfo.iTransparencyValue[2];
		iInfo.iTransparencyValue[192] = iInfo.iTransparencyValue[3];
		}

	iBytesPerPixel = 1;
	if (iInfo.iInterlaceMethod == TPngImageInformation::ENoInterlace)
		{
		TInt pixelPadding = ((iInfo.iSize.iWidth + 3) & ~3) - iInfo.iSize.iWidth;
		if(iImageProc)
			{
			iImageProc->SetPixelPadding(pixelPadding);	
			}
		if (iMaskProc)
			{
			iMaskProc->SetPixelPadding(pixelPadding);	
			}
		}
	}

TInt CBitDepth2Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return ((aPixelLength + 3) / 4) + KPngScanlineFilterTypeLength;
	}

void CBitDepth2Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt dataValue = *aDataPtr++;

			for (TInt mask=0xc0; mask!=0; mask>>=2) // iterate through 0xc0, 0x30, 0x0c and 0x03
				WritePixel(iInfo.iPalette[dataValue & mask],iInfo.iTransparencyValue[dataValue & mask]);
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt dataValue = *aDataPtr++;

			for (TInt mask=0xc0; mask!=0; mask>>=2) // iterate through 0xc0, 0x30, 0x0c and 0x03
				WritePixel(iInfo.iPalette[dataValue & mask]);
			}
		}
	}


// CBitDepth4Decoder
void CBitDepth4Decoder::DoConstructL()
	{
	if (!(iInfo.iColorType & TPngImageInformation::EPaletteUsed))
		{ // Set up palette to be grayscale values
		for (TInt index = 0; index < 16; index++)
			iInfo.iPalette[index] = TRgb::Gray16(index);

		if (iInfo.iTransparencyPresent)
			{
			if (iInfo.iTransparentGray <= 15)
				iInfo.iTransparencyValue[iInfo.iTransparentGray] = 0;
			}
		}

	iBytesPerPixel = 1;
	if (iInfo.iInterlaceMethod == TPngImageInformation::ENoInterlace)
		{
		TInt pixelPadding = ((iInfo.iSize.iWidth + 1) & ~1) - iInfo.iSize.iWidth;
		if(iImageProc)
			{
			iImageProc->SetPixelPadding(pixelPadding);
			}			
		if (iMaskProc)
			{
			iMaskProc->SetPixelPadding(pixelPadding);
			}			
		}
	}

TInt CBitDepth4Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return ((aPixelLength + 1) / 2) + KPngScanlineFilterTypeLength;
	}

void CBitDepth4Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt dataValue = *aDataPtr++;

			WritePixel(iInfo.iPalette[dataValue >> 4],iInfo.iTransparencyValue[dataValue >> 4]);
			WritePixel(iInfo.iPalette[dataValue & 0x0f],iInfo.iTransparencyValue[dataValue & 0x0f]);
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt dataValue = *aDataPtr++;

			WritePixel(iInfo.iPalette[dataValue >> 4]);
			WritePixel(iInfo.iPalette[dataValue & 0x0f]);
			}
		}
	}


// CBitDepth8Decoder
void CBitDepth8Decoder::DoConstructL()
	{
	if (!(iInfo.iColorType & TPngImageInformation::EPaletteUsed))
		{ // Set up palette to be grayscale values
		for (TInt index = 0; index < 256; index++)
			iInfo.iPalette[index] = TRgb::Gray256(index);

		if (iInfo.iTransparencyPresent)
			{
			if (iInfo.iTransparentGray <= 255)
				iInfo.iTransparencyValue[iInfo.iTransparentGray] = 0;
			}
		}

	iBytesPerPixel = 1;
	}

TInt CBitDepth8Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return aPixelLength + KPngScanlineFilterTypeLength;
	}

void CBitDepth8Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(iInfo.iPalette[aDataPtr[0]],iInfo.iTransparencyValue[aDataPtr[0]]);
			aDataPtr++;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			WritePixel(iInfo.iPalette[*aDataPtr++]);
		}
	}


// CBitDepth8ColorType2Decoder
void CBitDepth8ColorType2Decoder::DoConstructL()
	{
	iBytesPerPixel = 3;
	}

TInt CBitDepth8ColorType2Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 3) + KPngScanlineFilterTypeLength;
	}

void CBitDepth8ColorType2Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (!iFastProcessorMode)
		{
		if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
			{
			while (aDataPtr < aDataPtrLimit)
				{
				TInt red = aDataPtr[0];
				TInt green = aDataPtr[1];
				TInt blue = aDataPtr[2];
				TRgb pixelColor(red,green,blue);
				if (red == iInfo.iTransparentRed && green == iInfo.iTransparentGreen && blue == iInfo.iTransparentBlue)
					{
					WritePixel(pixelColor,0);	
					}
				else
					{
					WritePixel(pixelColor,255);	
					}
				aDataPtr += 3;
				}
			}
		else
			{
			while (aDataPtr < aDataPtrLimit)
				{
				WritePixel(TRgb(aDataPtr[0],aDataPtr[1],aDataPtr[2]));
				aDataPtr += 3;
				}
			}
		}
	else
		{
		iFastProc->SetPixels(aDataPtr, aDataPtrLimit, iLineCache, iPos);
		}
	}


// CBitDepth8ColorType4Decoder
void CBitDepth8ColorType4Decoder::DoConstructL()
	{
	iBytesPerPixel = 2;
	}

TInt CBitDepth8ColorType4Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 2) + KPngScanlineFilterTypeLength;
	}

void CBitDepth8ColorType4Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iMaskProc || iAlphaMode)
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb::Gray256(aDataPtr[0]),aDataPtr[1]);
			aDataPtr += 2;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb::Gray256(aDataPtr[0]));
			aDataPtr += 2;
			}
		}
	}


// CBitDepth8ColorType6Decoder
void CBitDepth8ColorType6Decoder::DoConstructL()
	{
	iBytesPerPixel = 4;
	}

TInt CBitDepth8ColorType6Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 4) + KPngScanlineFilterTypeLength;
	}

void CBitDepth8ColorType6Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (!iFastProcessorMode)
		{
		if (iMaskProc || iAlphaMode)
			{
			while (aDataPtr < aDataPtrLimit)
				{
				WritePixel(TRgb(aDataPtr[0],aDataPtr[1],aDataPtr[2]),aDataPtr[3]);
				aDataPtr += 4;
				}
			}
		else
			{
			while (aDataPtr < aDataPtrLimit)
				{
				WritePixel(TRgb(aDataPtr[0],aDataPtr[1],aDataPtr[2]));
				aDataPtr += 4;
				}
			}
		}
	else
		{
		iFastProc->SetPixels(aDataPtr, aDataPtrLimit, iLineCache, iPos);						
		}
	}


// CBitDepth16ColorType0Decoder
void CBitDepth16ColorType0Decoder::DoConstructL()
	{
	iBytesPerPixel = 2;
	}

TInt CBitDepth16ColorType0Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 2) + KPngScanlineFilterTypeLength;
	}

void CBitDepth16ColorType0Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt gray = (aDataPtr[0] << 8) | aDataPtr[1];
			TRgb pixelColor(TRgb::Gray256(aDataPtr[0]));
			if (gray == iInfo.iTransparentGray)
				WritePixel(pixelColor,0);
			else
				WritePixel(pixelColor,255);
			aDataPtr += 2;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb::Gray256(aDataPtr[0]));
			aDataPtr += 2;
			}
		}
	}


// CBitDepth16ColorType2Decoder
void CBitDepth16ColorType2Decoder::DoConstructL()
	{
	iBytesPerPixel = 6;
	}

TInt CBitDepth16ColorType2Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 6) + KPngScanlineFilterTypeLength;
	}

void CBitDepth16ColorType2Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iInfo.iTransparencyPresent && (iMaskProc || iAlphaMode) )
		{
		while (aDataPtr < aDataPtrLimit)
			{
			TInt red = (aDataPtr[0] << 8) | aDataPtr[1];
			TInt green = (aDataPtr[2] << 8) | aDataPtr[3];
			TInt blue = (aDataPtr[4] << 8) | aDataPtr[5];
			TRgb pixelColor(aDataPtr[0],aDataPtr[2],aDataPtr[4]);
			if (red == iInfo.iTransparentRed && green == iInfo.iTransparentGreen && blue == iInfo.iTransparentBlue)
				WritePixel(pixelColor,0);
			else
				WritePixel(pixelColor,255);
			aDataPtr += 6;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb(aDataPtr[0],aDataPtr[2],aDataPtr[4]));
			aDataPtr += 6;
			}
		}
	}


// CBitDepth16ColorType4Decoder
void CBitDepth16ColorType4Decoder::DoConstructL()
	{
	iBytesPerPixel = 4;
	}

TInt CBitDepth16ColorType4Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 4) + KPngScanlineFilterTypeLength;
	}

void CBitDepth16ColorType4Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iMaskProc || iAlphaMode)
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb::Gray256(aDataPtr[0]),aDataPtr[2]);
			aDataPtr += 4;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb::Gray256(aDataPtr[0]));
			aDataPtr += 4;
			}
		}
	}


// CBitDepth16ColorType6Decoder
void CBitDepth16ColorType6Decoder::DoConstructL()
	{
	iBytesPerPixel = 8;
	}

TInt CBitDepth16ColorType6Decoder::ScanlineBufferSize(TInt aPixelLength)
	{
	return (aPixelLength * 8) + KPngScanlineFilterTypeLength;
	}

void CBitDepth16ColorType6Decoder::DoDecode(TUint8* aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iMaskProc || iAlphaMode)
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb(aDataPtr[0],aDataPtr[2],aDataPtr[4]),aDataPtr[6]);
			aDataPtr += 8;
			}
		}
	else
		{
		while (aDataPtr < aDataPtrLimit)
			{
			WritePixel(TRgb(aDataPtr[0],aDataPtr[2],aDataPtr[4]));
			aDataPtr += 8;
			}
		}
	}
