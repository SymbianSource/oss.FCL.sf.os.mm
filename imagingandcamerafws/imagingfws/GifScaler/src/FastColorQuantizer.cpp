// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <hal.h>
#include "FastColorQuantizer.h"

CColorQuantizer* CFastColorQuantizer::NewL(CPalette* aPalette, TUint8 aTransparencyThreshold)
	{
	CFastColorQuantizer* self = new(ELeave) CFastColorQuantizer(aPalette, aTransparencyThreshold);
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop(self);
	return self;
	}

CFastColorQuantizer::CFastColorQuantizer(CPalette* aPalette, TUint8 aTransparencyThreshold)
: CColorQuantizer(aPalette, aTransparencyThreshold)
	{
	}

CFastColorQuantizer::~CFastColorQuantizer()
	{
	// Parent object should have cancelled the quantization,
	// so assert that we are not active here.
	ASSERT(!IsActive());
	}


void CFastColorQuantizer::ConstructL()
	{
	}

void CFastColorQuantizer::Quantize(TRequestStatus* aStatus, CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask, TInt aColorPixels)
	{
	ASSERT(aStatus != NULL);
	iRequestStatus = aStatus;
	*iRequestStatus = KRequestPending;

	ASSERT(aSource.SizeInPixels() == aDestination.SizeInPixels());
	ASSERT(aDestination.DisplayMode() == EColor256);
	InitBitmapSettings(aSource, aDestination, aDestinationMask, aColorPixels);

	SelfComplete(KErrNone);
	}

void CFastColorQuantizer::WriteQuantizedBitmap()
	{
	TInt scanlineLength = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestination->DisplayMode());
	TUint8* basePos = reinterpret_cast<TUint8*>(iDestination->DataAddress());
	
	TRgb pixel;
	for (iPixelPt.iY = 0 ; iPixelPt.iY<iBitmapSize.iHeight ; iPixelPt.iY++)
		{
		for (iPixelPt.iX = 0 ; iPixelPt.iX<iBitmapSize.iWidth ; iPixelPt.iX++)
			{
			// Get the next source pixel.
			iBitmap->GetPixel(pixel, iPixelPt);

			TUint8 nearestIndex = iPalette->NearestIndex(pixel);
			if ((iCurrentPixel < iPalette->Entries()) && (iPalette->GetEntry(nearestIndex) != pixel))
				{
				// Write the palette.
				iPalette->SetEntry(iCurrentPixel, pixel);

				// Write the pixel.
				basePos[iPixelPt.iX] = TUint8(iCurrentPixel);
				
				iCurrentPixel++;
				}
			else
				{
				// Write the pixel.
				basePos[iPixelPt.iX] = nearestIndex;
				}

			}

		basePos += scanlineLength;
		}
	}

void CFastColorQuantizer::WriteQuantizedBitmapAndAddTransparency()
	{
	TInt scanlineLength = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestination->DisplayMode());
	TUint8* basePos = reinterpret_cast<TUint8*>(iDestination->DataAddress());
	
	TInt scanlineLengthMask = CFbsBitmap::ScanLineLength(iBitmapSize.iWidth, iDestinationMask->DisplayMode());
	TUint8* basePosMask = reinterpret_cast<TUint8*>(iDestinationMask->DataAddress());

	// The two scanline lengths should be the same.
	ASSERT(scanlineLength == scanlineLengthMask);

	TRgb pixel;
	for (iPixelPt.iY = 0 ; iPixelPt.iY<iBitmapSize.iHeight ; iPixelPt.iY++)
		{
		for (iPixelPt.iX = 0 ; iPixelPt.iX<iBitmapSize.iWidth ; iPixelPt.iX++)
			{
			// Write the pixel.
			TBool pixelTransparent = (basePosMask[iPixelPt.iX]<=iTransparencyThreshold);
			if (pixelTransparent)
				{
				basePos[iPixelPt.iX] = KTransparencyIndex;
				}
			else
				{
				// Get the next source pixel.
				iBitmap->GetPixel(pixel, iPixelPt);

				TUint8 nearestIndex = iPalette->NearestIndex(pixel);
				if ((iCurrentPixel < iPalette->Entries()) && (iPalette->GetEntry(nearestIndex) != pixel))
					{
					// Write a new palette entry.
					iPalette->SetEntry(iCurrentPixel, pixel);

					// Write the pixel.
					basePos[iPixelPt.iX] = TUint8(iCurrentPixel);
					
					iCurrentPixel++;
					}
				else
					{
					// Write the pixel.
					basePos[iPixelPt.iX] = nearestIndex;
					}

				}
			}

		basePos += scanlineLength;
		basePosMask += scanlineLengthMask;
		}
	}

void CFastColorQuantizer::RunL()
	{
	iCurrentPixel = 0;

	if (iDestinationMask)
		WriteQuantizedBitmapAndAddTransparency();
	else
		WriteQuantizedBitmap();

	RequestComplete(KErrNone);
	}
