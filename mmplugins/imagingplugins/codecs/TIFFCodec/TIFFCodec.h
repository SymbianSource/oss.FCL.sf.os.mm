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

#ifndef __TIFFCODEC_H__
#define __TIFFCODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>
#include <icl/imagecodecdata.h>

#include "TIFFConvert.h"
#include "../recordtable.h"

// Constants.
const TInt KTiffIfdEntryTagOffset = 0;
const TInt KTiffIfdEntryTypeOffset = 2;
const TInt KTiffIfdEntryCountOffset = 4;
const TInt KTiffIfdEntryValueOffset = 8;
const TInt KTiffIfdEntryLength = 12;

// Helper classes.
// TTiffImageStrip
class TTiffImageStrip
	{
public:
	TInt	iOffset;
	TInt	iLength;
	};

// Tiff Image File Directory (IFD) class
class TTiffIfdEntry
	{
public:
	enum TId
		{
		ENewSubfileType = 0xFE,
		ESubfileType = 0xFF,
		EImageWidth = 0x100,
		EImageLength = 0x101,
		EBitsPerSample = 0x102,
		ECompression = 0x103,
		EPhotometricInterpretation = 0x106,
		EFillOrder = 0x10A,
		EStripOffsets = 0x111,
		ESamplesPerPixel = 0x115,
		ERowsPerStrip = 0x116,
		EStripByteCounts = 0x117,
		EXResolution = 0x11a,
		EYResolution = 0x11b,
		ET4Options = 0x124,
		ET6Options = 0x125,
		EResolutionUnit = 0x128,
		};
	enum TType
		{
		EByte = 1,
		EAscii = 2,
		EShort = 3,
		ELong = 4,
		ERational = 5,
		ESbyte = 6,
		EUndefined = 7,
		ESshort = 8,
		ESlong = 9,
		ESrational = 10,
		EFloat = 11,
		EDouble = 12,
		};
	enum TT4Options
		{
		ET4TwoDimentionalCoding = 0x1,
		ET4ContainsUncompressedData = 0x2,
		ET4ContainsPaddedEols = 0x4
		};
	enum TT6Options
		{
		ET6ContainsUncompressedData = 0x2,
		};
	enum TCompression
		{
		EUncompressed = 1,
		ECcitt1dCompression = 2,
		EGroup3FaxCompression = 3,
		EGroup4FaxCompression = 4,
		ELzwCompression = 5,
		EJpegCompression = 6,
		EPackBitsCompression = 32773,
		};
	enum TPhotometricInterpretation
		{
		EWhiteIsZero = 0,
		EBlackIsZero = 1,
		ERgb = 2,
		ERgbPalette = 3,
		ETransparencyMask = 4,
		ECmyk = 5,
		EYCbCr = 6,
		ECieLab = 8,
		};
public:
	TInt	TypeSize() const;
public:
	TId		iId;
	TType	iType;
	TInt	iCount;
	TUint32	iValue;
	TUint8*	iValuePtr;
	};


// Read codec.
class CTiffDecoder;
class CTiffReadSubCodec : public CBase
	{
public:
	virtual void	DoNewFrameL(CFbsBitmap& aBitmap, TBool aDisableErrorDiffusion, TInt aReductionFactor) = 0;
	virtual void	NewStripL(TInt aNumBytes) = 0;
	virtual TBool	ProcessStripL(TBufPtr8* aSrc) = 0;
	};

class CTiffReadCodec : public CImageReadCodec
	{
public:
	
	static CTiffReadCodec* NewL(TTiffFormatInfo aFormatInfo, CTiffDecoder& aPlugin);
	
	// from CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual void InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData);

	TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	void GetNewDataPosition(TInt& aPosition, TInt& aLength);

protected:
	~CTiffReadCodec();

private:
	CTiffReadCodec(TTiffFormatInfo aFormatInfo, CTiffDecoder& aPlugin);


private:
	TInt	ReadIfdL(const TUint8*& aPtr,const TUint8* aPtrLimit, TInt& aIfdLengthInBytes);
	void	ReadIfdEntryL(TTiffIfdEntry& aEntry,const TUint8*const aPtr);
	TInt	ReadLongValuesL(const TUint8*& aPtr, const TUint8*const aPtrLimit);
	void	ProcessIfdL();
	void	ProcessIfdEntryL(const TTiffIfdEntry& aEntry);
	TInt	IntegerIfdEntryValueL(const TTiffIfdEntry& aEntry) const;
	TInt	IntegerIfdEntryValueL(const TTiffIfdEntry& aEntry,TInt aIndex) const;
	TReal	RationalIfdEntryValueL(const TTiffIfdEntry& aEntry) const;
	void	NewStripL();
private:
	enum TProcessHeaderState
		{
		EReadIfd,
		EReadLongValues,
		EProcessIfd,
		EFinish
		};
private:
	// Local frame settings.
	TFrameInfo *iImageInfo;
	CFrameImageData *iImageData;

	TTiffImageData* iFrameImageData;
	TTiffImageData* iTiffImageData;
	TTiffImageStrip* iStripInfo;

	TInt				iNewPosition;

	TTiffFormatInfo		iFormatInfo;
	TTiffValueReader	iValueReader;
	TInt				iNextIfdOffset;

	TProcessHeaderState	iProcessHeaderState;

	HBufC8*				iIfdBuffer;
	TTiffIfdEntry*		iIfdEntries;
	TInt				iNumIfdEntries;

	HBufC8*				iLongValuesBuffer;
	TUint8*				iLongValues;
	TInt				iLongValuesSize;
	TInt				iLoadedLongValuesSize;
	TInt				iLongValuesStartOffset;
	TInt				iLongValuesEndOffset;

	TBool				iNewStrip;
	TInt				iCurrentStrip;

	CTiffReadSubCodec*	iDecoder;
	CTiffDecoder& 		iPlugin;
	TInt				iIfdOffset;
	TInt				iIfdSize;
	CRecordTable*		iRecordTable;
	};

#endif // __TIFFCODEC_H__

