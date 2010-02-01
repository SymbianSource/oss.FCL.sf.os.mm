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
// Licensed under US Patent No 4,558,302 and foreign counterparts
// 
//

#ifndef __GIFFORMAT_H__
#define __GIFFORMAT_H__

#include <icl/imageprocessor.h>

const TInt KGifFileInformationSize = 13;
const TInt KGifImageInformationSize = 10;
const TInt KGifPaletteEntrySize = 3;
const TInt KGifSignatureLength = 6;

_LIT8(KGif87aFileSignature,"GIF87a");
_LIT8(KGif89aFileSignature,"GIF89a");

class TGifFileInformation
	{
public:
	TBuf8<6> iSignature;
	TSize iScreenSize;
	TInt iBitsPerPixel;
	TInt iColorResolutionBits;
	TBool iGlobalColorMap;
	TInt iBackgroundColorIndex;
	TInt iPixelAspectRatio;
	TBool iSortedGlobalMap;
	};

class TGifFormatInfo
	{
public:
	TRgb* iGlobalPalette;
	TInt iBackgroundColorIndex;
	};

#endif

