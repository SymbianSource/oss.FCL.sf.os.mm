// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <icl/imageprocessor.h>

//
// this file contains many performance-critical code, so use ARM instruction set for it
//
#ifdef __ARMCC__
#pragma arm
#pragma O3
#pragma Otime
#endif

/**
 	@internalComponent
*/
GLDEF_C void Panic(TImageBitmapUtilPanic aError);

/**
  	@internalComponent
*/
GLDEF_C TColorConvertor* CreateColorConvertorL(TDisplayMode aDisplayMode);

/**
Static factory function for creating instances of TColorConvertor derived classes
based on the supplied display mode.

@param  aDisplayMode
        The display mode. This determines the TColorConvertor derived type returned.

@return A pointer to a fully constructed TColorConvertor derived object.

@leave  KErrNotSupported
        The display mode is not supported.
*/
EXPORT_C TColorConvertor* TColorConvertor::NewL(TDisplayMode aDisplayMode)
	{
	return CreateColorConvertorL(aDisplayMode);
	}

//
// TImageBitmapUtil
//

/**
Default constructor for this class.
*/
EXPORT_C TImageBitmapUtil::TImageBitmapUtil():
	iBitmap(NULL),
	iBpp(0),
	iBppShift(0),
	iPixelShift(0),
	iBitShift(0),
	iScanlineWordLength(0)
	{}

/**
Requests a lock for the current bitmap from the font & bitmap server and
sets the current position in the bitmap to the first pixel.
*/
EXPORT_C void TImageBitmapUtil::Begin()
	{
	ASSERT(iBitmap && iBitmap->Handle());

	iBase.iWordPos = iBitmap->DataAddress();
	}

/**
Requests a lock for the current bitmap from the font & bitmap server and
sets the current position in the bitmap to aPosition.

@param  aPosition
        The position to move to.

@return A boolean indicating if the position was out of bounds. EFalse if the position was out of 
        bounds, otherwise ETrue.
*/
EXPORT_C TBool TImageBitmapUtil::Begin(const TPoint& aPosition)
	{
	Begin();
	if (!SetPos(aPosition))
		{
		End();
		return EFalse;
		}

	return ETrue;
	}

/**
Releases a lock previously acquired using TImageBitmapUtil::Begin().
*/
EXPORT_C void TImageBitmapUtil::End()
	{
	}

/**
Sets the current bitmap to aBitmap.

@param  aBitmap
        A pointer to the bitmap.

@leave  KErrNotFound
        The bitmap or its handle is NULL or its display mode is not recognised.
*/
EXPORT_C void TImageBitmapUtil::SetBitmapL(CFbsBitmap* aBitmap)
	{
	if (!aBitmap || !aBitmap->Handle())
		User::Leave(KErrNotFound);
	
	if(aBitmap->ExtendedBitmapType()!=KNullUid)
	    {
	    User::Leave(KErrNotSupported);
	    }

	iBitmap = aBitmap;
	iWordAccess = ETrue;

	switch(iBitmap->DisplayMode())
		{
	case EGray2:
		iBppShift = 0;
		iMask = 1;
		break;
	case EGray4:
		iBppShift = 1;
		iMask = 3;
		break;
	case EGray16:
	case EColor16:
		iBppShift = 2;
		iMask = 0xF;
		break;
	case EGray256:
	case EColor256:
		iBppShift = 3;
		iMask = 0xFF;
		break;
	case EColor4K:
	case EColor64K:
		iBppShift = 4;
		iMask = 0xFFFF;
		break;
	case EColor16M:
		iWordAccess = EFalse;
		break;
	case EColor16MU:
		iBppShift = 5;
		iMask = 0xFF000000; // | with pixel value to set alpha channel to opaque
		break;
	case EColor16MA:		
		iBppShift = 5;
		iMask = 0x00000000; // | with pixel value to set alpha channel level
		break;
	default:
		User::Leave(KErrNotFound);
		break;
		}

	iBpp = 1<<iBppShift;
	iPixelsPerWord = 32>>iBppShift;
	iPixelShift = 5-iBppShift;

	iSize = iBitmap->SizeInPixels();
	iScanlineWordLength = CFbsBitmap::ScanLineLength(iSize.iWidth,iBitmap->DisplayMode()) / 4;
	}

/**
Sets the pixel value at the current bitmap position using aPixelIndex.

@post    
The current position is updated.

@param  aPixelIndex
        The pixel index.
*/
EXPORT_C void TImageBitmapUtil::SetPixel(TUint32 aPixelIndex)
	{
	ASSERT(iPosition.iX < iSize.iWidth);

	iPosition.iX++;

	if (iWordAccess)
		{
		TUint32* dataPtr = iData.iWordPos;
		switch(iBppShift)
			{
			case 5:
				*dataPtr = aPixelIndex|iMask;
				break;
			default:
				TInt bitShift = iBitShift;

				*dataPtr &= ~(iMask << bitShift);
				*dataPtr |= aPixelIndex << bitShift;

				bitShift += iBpp;
				if (bitShift >= 32)
					{
					bitShift = 0;
					iData.iWordPos = dataPtr+1;
					}
				iBitShift = bitShift;
				break;
			}
		}
	else
		{
		TUint8* dataPtr = iData.iBytePos;

		*dataPtr++ = TUint8(aPixelIndex);
		*dataPtr++ = TUint8(aPixelIndex >> 8);
		*dataPtr++ = TUint8(aPixelIndex >> 16);

		iData.iBytePos = dataPtr;
		}

	}

/**
Sets an array of pixel values, starting at the current bitmap position using the
values supplied in aPixelIndex.

@post    
The current position is updated.

@param  aPixelIndex
        A pointer to the first element in the array.
@param  aNumberOfPixels
        The number of elements in the array.
*/
EXPORT_C void TImageBitmapUtil::SetPixels(TUint32* aPixelIndex,TInt aNumberOfPixels)
	{
	TInt newXPos = iPosition.iX + aNumberOfPixels;
	ASSERT(newXPos <= iSize.iWidth);
	
	if (newXPos == iSize.iWidth)
		{
		iPosition.iX = 0;
		iPosition.iY++;
		}
	else
		{
		iPosition.iX = newXPos;
		}

	if (iWordAccess)
		{
		TUint32*	dataPtr = iData.iWordPos;
		TInt		bitShift = iBitShift;
		TInt		bpp = iBpp;

		// Do pixels in first data word (if first pixel isn't on 32 bit word boundary)

		if(bitShift > 0)
			{
			TInt bitShiftLimit = bitShift+(aNumberOfPixels<<iBppShift);
			if(bitShiftLimit>32)
				bitShiftLimit = 32;

			TInt numMaskBits = bitShiftLimit-bitShift;
			aNumberOfPixels -= numMaskBits>>iBppShift;
			TInt mask = (1<<numMaskBits)-1;	//mask has lowest 'numMaskBits' set

			TInt wordValue = *dataPtr;

			wordValue &= ~(mask << bitShift);	//zero the destination pixels we are about to write
			do	{
				wordValue |= *aPixelIndex++ << bitShift;
				bitShift += bpp;
				}
			while(bitShift<bitShiftLimit);

			*dataPtr = wordValue;

			if(bitShift >= 32)
				{
				bitShift = 0;
				dataPtr++;
				}
			}

		// Do pixels which fill whole data words

		TUint32* dataPtrLimit = dataPtr+(aNumberOfPixels >> iPixelShift);

		aNumberOfPixels &= ~(0xFFFFFFFF<<iPixelShift);

		if(dataPtr < dataPtrLimit)
			{
			switch(iBppShift)
				{
				case 5:
					if(iMask == 0xFF000000)
						{
						do	{
							*dataPtr++ = *aPixelIndex++|0xFF000000;
							}
						while(dataPtr < dataPtrLimit);
						}
					else
						{
						do	{
							*dataPtr++ = *aPixelIndex++;
							}
						while(dataPtr < dataPtrLimit);
						}
					
					break;

				case 4:
					do	{
						TUint32 wordValue = *aPixelIndex++;
						wordValue |= *aPixelIndex++ << 16;
						*dataPtr++ = wordValue;
						}
					while(dataPtr < dataPtrLimit);
					break;

				case 3:
					do	{
						TUint32 wordValue = *aPixelIndex++;
						wordValue |= *aPixelIndex++ << 8;
						wordValue |= *aPixelIndex++ << 8*2;
						wordValue |= *aPixelIndex++ << 8*3;
						*dataPtr++ = wordValue;
						}
					while(dataPtr < dataPtrLimit);
					break;

				case 2:
					do	{
						TUint32 wordValue = *aPixelIndex++;
						wordValue |= *aPixelIndex++ << 4;
						wordValue |= *aPixelIndex++ << 4*2;
						wordValue |= *aPixelIndex++ << 4*3;
						wordValue |= *aPixelIndex++ << 4*4;
						wordValue |= *aPixelIndex++ << 4*5;
						wordValue |= *aPixelIndex++ << 4*6;
						wordValue |= *aPixelIndex++ << 4*7;
						*dataPtr++ = wordValue;
						}
					while(dataPtr < dataPtrLimit);
					break;

				case 1:
					do	{
						TUint32 wordValue = 0;
						TInt pixelShift = 0;
						do	{
							TUint32 eightPixels = *aPixelIndex++;
							eightPixels  |= *aPixelIndex++ << 2;
							eightPixels  |= *aPixelIndex++ << 2*2;
							eightPixels  |= *aPixelIndex++ << 2*3;
							eightPixels  |= *aPixelIndex++ << 2*4;
							eightPixels  |= *aPixelIndex++ << 2*5;
							eightPixels  |= *aPixelIndex++ << 2*6;
							eightPixels  |= *aPixelIndex++ << 2*7;
							wordValue |= eightPixels << pixelShift;
							pixelShift += 2*8;
							}
						while(pixelShift < 32);
						*dataPtr++ = wordValue;
						}
					while(dataPtr < dataPtrLimit);
					break;

				case 0:
					do	{
						TUint32 wordValue = 0;
						TInt pixelShift = 0;
						do	{
							TUint32 eightPixels = *aPixelIndex++;
							eightPixels  |= *aPixelIndex++ << 1;
							eightPixels  |= *aPixelIndex++ << 2;
							eightPixels  |= *aPixelIndex++ << 3;
							eightPixels  |= *aPixelIndex++ << 4;
							eightPixels  |= *aPixelIndex++ << 5;
							eightPixels  |= *aPixelIndex++ << 6;
							eightPixels  |= *aPixelIndex++ << 7;
							wordValue |= eightPixels << pixelShift;
							pixelShift += 8;
							}
						while(pixelShift < 32);
						*dataPtr++ = wordValue;
						}
					while(dataPtr < dataPtrLimit);
					break;
				default:
					Panic(ECorrupt);
					break;
				}
			}

		// Do remaining pixels for colour depths that are < 32bit.

		if(aNumberOfPixels)
			{
			TInt bitShiftLimit = aNumberOfPixels<<iBppShift;
			TInt wordValue = *dataPtr;

			wordValue &= 0xFFFFFFFF<<bitShiftLimit;		//zero the destination pixels we are about to write
			do	{
				wordValue |= *aPixelIndex++ << bitShift;
				bitShift += bpp;
				}
			while(bitShift<bitShiftLimit);

			*dataPtr = wordValue;
			}

		// Finished

		iData.iWordPos = dataPtr;
		iBitShift = bitShift;
		}
	else
		{
		TUint32* pixelPtrLimit = aPixelIndex + aNumberOfPixels;

		TUint8* dataPtr = iData.iBytePos;

		while (aPixelIndex < pixelPtrLimit)
			{
			TUint32 pixel = *aPixelIndex++;
			*dataPtr++ = TUint8(pixel);
			*dataPtr++ = TUint8(pixel >> 8);
			*dataPtr++ = TUint8(pixel >> 16);
			}

		iData.iBytePos = dataPtr;
		}

	}

/**
Sets the current position in the bitmap to aPosition.

@param  aPosition
        The position to move to.

@return A boolean indicating if the position was out of bounds. EFalse if the position was out of 
        bounds, otherwise ETrue.
*/
EXPORT_C TBool TImageBitmapUtil::SetPos(const TPoint& aPosition)
	{
	ASSERT(iBitmap);
	ASSERT(iBase.iWordPos);

	if (aPosition.iX < 0 || aPosition.iX >= iSize.iWidth || aPosition.iY < 0 || aPosition.iY >= iSize.iHeight)
		return EFalse;

	iData.iWordPos = iBase.iWordPos + (iScanlineWordLength * aPosition.iY);

	if (iWordAccess)
		{
		iData.iWordPos += aPosition.iX >> iPixelShift;
		iBitShift = (aPosition.iX << iBppShift) & 0x1f;
		}
	else
		iData.iBytePos += (aPosition.iX << 1) + aPosition.iX;

	iPosition = aPosition;
	return ETrue;
	}

