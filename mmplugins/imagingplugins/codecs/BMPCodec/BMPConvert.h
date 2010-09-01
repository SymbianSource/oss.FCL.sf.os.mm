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

#ifndef __BMPCONVERT_H__
#define __BMPCONVERT_H__

#include <icl/imageplugin.h>
#include "BMPCodec.h"
#include "BMPFormat.h"

class CBmpReadCodec;
class CFrameInfoStrings;
class CBmpDecoder : public CImageDecoderPlugin
	{
public:
	static CBmpDecoder* NewL();
	~CBmpDecoder();
	//From CImageEncoder
	void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const; 
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

private:
	CBmpDecoder();
	//From CImageEncoderPlugin
	void ScanDataL();
	void ReadFormatL();

private:
	TRgb iPalette[KBmpMaxPaletteEntries];
	TBmpHeader iBitmapHeader;
	};


// Bmp encoder
class CBmpEncoder : public CImageEncoderPlugin
	{
public:
	static CBmpEncoder* NewL();
	~CBmpEncoder();
protected:
	//From CImageEncoder
	void Cleanup(); 
private:
	CBmpEncoder();
	//From CImageEncoderPlugin
	void UpdateHeaderL();
	void PrepareEncoderL(const CFrameImageData* aFrameImageData); 
private:
	HBufC8* iReadBuffer;
	TBmpHeader iBitmapHeader;
	TRgb iPalette[KBmpMaxPaletteEntries];
	};

#endif // __BMPCONVERT_H__
