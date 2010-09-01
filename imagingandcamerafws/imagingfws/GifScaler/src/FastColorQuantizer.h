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

#ifndef __FASTCOLORQUANTIZER_H__
#define __FASTCOLORQUANTIZER_H__

#include "ColorQuantizer.h"

class CFastColorQuantizer; // declared here
NONSHARABLE_CLASS( CFastColorQuantizer ): public CColorQuantizer
	{
public:
	static CColorQuantizer* NewL(CPalette* aPalette, TUint8 aTransparencyThreshold);
	~CFastColorQuantizer();

	// From CActive
	void RunL();
	
	// From CColorQuantizer
	void Quantize(TRequestStatus* aStatus, CFbsBitmap& aSource, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask, TInt aColorPixels);

	// Write the quantized bitmap.
	void WriteQuantizedBitmap();

	// Write the quantized bitmap and add transparency.
	void WriteQuantizedBitmapAndAddTransparency();

private:
	CFastColorQuantizer(CPalette* aPalette, TUint8 aTransparencyThreshold);
	void ConstructL();
	};

#endif
