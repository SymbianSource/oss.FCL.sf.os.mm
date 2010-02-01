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

#ifndef __TIFFCONVERT_H__
#define __TIFFCONVERT_H__

#include "icl/ImagePlugin.h"
#include "TIFFCodec.h"

// Constants.
const TInt KTiffHeaderSize = 8;
const TInt KTiffSignatureLength = 4;

// Helper classes.
// Class to read little/big endian values.
enum TTiffEndianness
	{
	ETiffLittleEndian = 0,
	ETiffBigEndian = 1
	};

class TTiffValueReader
	{
public:
	inline			TTiffValueReader(TTiffEndianness aEndianness = ETiffLittleEndian);

	TUint32			ReadUint32(const TUint8* aPtr) const;
	inline TInt32	ReadInt32(const TUint8* aPtr) const;
	TUint16			ReadUint16(const TUint8* aPtr) const;
	inline TInt16	ReadInt16(const TUint8* aPtr) const;
public:
	TTiffEndianness	iEndianness;
	};

// Format info
class TTiffFormatInfo
	{
public:
	TTiffEndianness	iEndianness;
	TInt			iFirstIfd;
	TUint32			iSignature;
	};

// Decoder.
class CTiffReadCodec;
class CTiffDecoder : public CImageDecoderPlugin
	{
private:
	enum TTiffSubType
		{
		ETiffUnknownSubType, ETiffLittleEndianSubType, ETiffBigEndianSubType
		};
	enum TTiffFormatType
		{
		EFormatGroup3Fax1D = 0, // values must co-incide with .rss image_format[]
		EFormatGroup3Fax2D,
		EFormatGroup4Fax,
		EFormatUnknown
		};
	
public:
 	static CTiffDecoder* NewL();
 	static CTiffDecoder* NewLittleEndianL();
 	static CTiffDecoder* NewBigEndianL();
	~CTiffDecoder();

	// from CImageDecoder
	void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);
	TInt CurrentFilePosition();
	
protected:
	void Cleanup();
private:
	CTiffDecoder(TTiffSubType aTiffSubType);
	void ReadFormatL();
	void ScanDataL();

private:
	TTiffFormatInfo	iFormatInfo;
	TTiffSubType iTiffSubType;
	};

// Inlines
inline TTiffValueReader::TTiffValueReader(TTiffEndianness aEndianness)
	: iEndianness(aEndianness)
	{
	}

inline TInt32 TTiffValueReader::ReadInt32(const TUint8* aPtr) const
	{
	return TInt32(ReadUint32(aPtr));
	}

inline TInt16 TTiffValueReader::ReadInt16(const TUint8* aPtr) const
	{
	return TInt16(ReadUint16(aPtr));
	}

#endif // __TIFFCONVERT_H__
