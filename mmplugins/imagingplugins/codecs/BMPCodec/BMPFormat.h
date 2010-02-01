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

#ifndef __BMPFORMAT_H__
#define __BMPFORMAT_H__

#include <icl/imageprocessor.h>
#include "ImageConversion.h"

const TInt KBmpFileHeaderSize = 14;
const TInt KBmpInfoHeaderV1Size = 12;
const TInt KBmpInfoDefaultHeaderV2Size = 40;
const TInt KBmpInfoMaxHeaderV2Size = 64;
const TInt KBmpMaxPaletteEntries = 256;
const TInt KBmpSizeofPaletteEntryV1 = 3;
const TInt KBmpSizeofPaletteEntryV2 = 4;


const TUint16 KBmpFileSignature = 'B' | ('M' << 8);

class TBmpHeader : public TFrameInfo
	{
public:
	enum TCompression
		{
		ENone,
		EEightBppRLE,
		EFourBppRLE,
		EBitFields // This is valid only when used with 16-bpp and 32-bpp bitmaps.
		};

public:
	TSize iSizeInPixels;
	TCompression iCompression;
	TInt iPaletteEntries; // 0 - 256
	TInt iBitCount; // 1, 4, 8, 24, 32
	};

#endif // __BMPFORMAT_H__

