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
// CColorQuantizer.h
// 
//

#ifndef __CCOLORQUANTIZER_H__
#define __CCOLORQUANTIZER_H__

#include <e32base.h>
#include <fbs.h>

const TUint8 KTransparencyIndex = 0xFF;
class CColorQuantizer; // declared here
NONSHARABLE_CLASS( CColorQuantizer ): public CActive
	{
public:
	~CColorQuantizer();

	// From CActive
	void DoCancel();
	
	// Color quantize a bitmap and add transparency if a mask is supplied.
	virtual void Quantize(TRequestStatus* aStatus, CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask, TInt aColorPixels) = 0;

protected:
	CColorQuantizer(CPalette* aPalette, TUint8 aTransparencyThreshold);
	void InitBitmapSettings(CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* DestinationMask, TInt aColorPixels);

	// Complete quantization.
	void RequestComplete(TInt aReason);

	// Complete current state.
	void SelfComplete(TInt aReason);

protected:
	// The source bitmap. (not owned)
	CFbsBitmap* iBitmap;

	// The destination bitmap. (not owned)
	CFbsBitmap* iDestination;

	// Destination mask. (used for adding transparency, not owned)
	CFbsBitmap* iDestinationMask;
	
	// Size of input bitmap.
	TSize iBitmapSize;

	// Total number of non-transparent pixels.
	TInt iColorPixels;

	// The output palette. (not owned)
	CPalette* iPalette;

	// Number of pixels sampled so far.
	TInt iCurrentPixel;

	// Position of the current pixel.
	TPoint iPixelPt;

	// The threshold level for transparent pixels.
	// The bitmap pixel is transparent if the corresponding mask pixel
	// is less than or equal to the transparency threshold.
	TUint8 iTransparencyThreshold;

	// Request status to signal when done. (not owned)
	TRequestStatus* iRequestStatus;
};

#endif
