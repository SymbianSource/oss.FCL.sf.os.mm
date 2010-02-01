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
// Licensed under US Patent No 4,558,302 and foreign counterparts
// 
//

#ifndef GIFCONVERT_H
#define GIFCONVERT_H

#include <icl/imageplugin.h>
#include "GIFcodec.h"
#include "GIFFormat.h"

class CGifReadCodec;
class TGifImageInformation;
class CGifDecoder : public CImageDecoderPlugin
	{
public:
	static CGifDecoder* NewL();
	~CGifDecoder();

	// From CImageDecoder
	void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	TInt NumberOfImageComments() const;
	HBufC* ImageCommentL(TInt aCommentNumber) const;

protected:
	// From CImageDecoder
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

private:
	CGifDecoder();
	void ReadFormatL();
	// From CImageDecoderPlugin
	void ScanDataL();
private:
	TInt iReadBlockLength;
	TGifFileInformation iFileInfo;
	TInt iGlobalPaletteEntries;
	TRgb iGlobalPalette[KGifColorTableMaxEntries];
	TBool iLoadingFrame;
	};


// CGifEncoder
class CGifScaler;
class CGifWriteCodec;
class CGifEncoder : public CImageEncoderPlugin
	{
public:
	static CGifEncoder* NewL();
	virtual ~CGifEncoder();
	
	const CPalette* Palette() const;
protected:
	//from CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	void UpdateHeaderL();
	void WriteExifDataL(TRequestStatus*& aScaleCompletionStatus);
private:
	void ConstructL();
	CGifEncoder();
private:
	CPalette* iPalette;
	CFbsBitmap* iDest;
	CGifScaler* iGifScaler;
	};

#endif // GIFCONVERT_H
