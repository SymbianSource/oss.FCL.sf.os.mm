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

#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <imageconversion.h>
#include "ImageClientMain.h"
#include "ImageUtils.h"
#include <101F45C0_extra.rsg>
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include "TIFFCodec.h"

_LIT(KTIFFPanicCategory, "TIFFConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KTIFFPanicCategory, aError);
	}

// TTiffValueReader.
TUint32 TTiffValueReader::ReadUint32(const TUint8* aPtr) const
	{
	TUint value;
	if (iEndianness == ETiffLittleEndian)
		{
		value  = aPtr[0];
		value |= aPtr[1] << 8;
		value |= aPtr[2] << 16;
		value |= aPtr[3] << 24;
		}
	else
		{
		value  = aPtr[0] << 24;
		value |= aPtr[1] << 16;
		value |= aPtr[2] << 8;
		value |= aPtr[3];
		}
	return TUint32(value);
	}

TUint16 TTiffValueReader::ReadUint16(const TUint8* aPtr) const
	{
	TUint value;
	if (iEndianness == ETiffLittleEndian)
		{
		value  = aPtr[0];
		value |= aPtr[1] << 8;
		}
	else
		{
		value  = aPtr[0] << 8;
		value |= aPtr[1];
		}
	return TUint16(value);
	}


// Tiff decoder.
CTiffDecoder* CTiffDecoder::NewL()
	{
	return new(ELeave) CTiffDecoder(ETiffUnknownSubType);
	}

CTiffDecoder* CTiffDecoder::NewLittleEndianL()
	{
	return new(ELeave) CTiffDecoder(ETiffLittleEndianSubType);
	}

CTiffDecoder* CTiffDecoder::NewBigEndianL()
	{
	return new(ELeave) CTiffDecoder(ETiffBigEndianSubType);
	}

CTiffDecoder::CTiffDecoder(TTiffSubType aTiffSubType)
	{
	iTiffSubType = aTiffSubType;
	}

CTiffDecoder::~CTiffDecoder()
	{
	Cleanup();
	}

void CTiffDecoder::Cleanup()
	{
	// Delete any objects we should get rid of

	// Base class included
	CImageDecoderPlugin::Cleanup();
	}

void CTiffDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < NumberOfFrames()), Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeTIFFUid;
	if (iTiffSubType == ETiffLittleEndianSubType)
		aImageSubType = KImageTypeTIFFSubTypeLittleEndianUid;
	else
		aImageSubType = KImageTypeTIFFSubTypeBigEndianUid;
	}

// Scan header.
void CTiffDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec() == NULL);

	CTiffReadCodec* imageReadCodec;

	imageReadCodec = CTiffReadCodec::NewL(iFormatInfo, *this);
	
	SetImageReadCodec(imageReadCodec);
	
	ReadFrameHeadersL();
	}

#define KTiffHeaderLittleEndianDefine		{ 0x49, 0x49, 0x2A, 0x00 }
#define KTiffHeaderBigEndianDefine			{ 0x4D, 0x4D, 0x00, 0x2A }
LOCAL_D const TUint8 TiffLittleEndianSignature[KTiffSignatureLength] = KTiffHeaderLittleEndianDefine;
LOCAL_D const TUint8 TiffBigEndianSignature[KTiffSignatureLength] = KTiffHeaderBigEndianDefine;
void CTiffDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KTiffHeaderSize);

	// Validate the header.
	if (bufferDes.Length() < KTiffHeaderSize)
		User::Leave(KErrUnderflow);

	iFormatInfo.iSignature = PtrReadUtil::ReadUint32(CONST_CAST(TUint8*, bufferDes.Ptr()));

	switch (iTiffSubType)
		{
	case ETiffUnknownSubType:
		if (iFormatInfo.iSignature == *REINTERPRET_CAST(const TUint32*,TiffLittleEndianSignature))
			iFormatInfo.iEndianness = ETiffLittleEndian;
		else if (iFormatInfo.iSignature == *REINTERPRET_CAST(const TUint32*,TiffBigEndianSignature))
			iFormatInfo.iEndianness = ETiffBigEndian;
		else
			User::Leave(KErrCorrupt);
		break;

	case ETiffLittleEndianSubType:
		if (iFormatInfo.iSignature != *REINTERPRET_CAST(const TUint32*,TiffLittleEndianSignature))
			User::Leave(KErrCorrupt);

		iFormatInfo.iEndianness = ETiffLittleEndian;
		break;

	case ETiffBigEndianSubType:
		if (iFormatInfo.iSignature != *REINTERPRET_CAST(const TUint32*,TiffBigEndianSignature))
			User::Leave(KErrCorrupt);

		iFormatInfo.iEndianness = ETiffBigEndian;
		break;
		}

	TTiffValueReader valueReader = TTiffValueReader(iFormatInfo.iEndianness);

	const TUint8* ifdPtr = &bufferDes[KTiffSignatureLength];
	iFormatInfo.iFirstIfd = valueReader.ReadUint32(ifdPtr);
	
	//Ifd (offset in file) should always be pointing to bytes after header.
	if (iFormatInfo.iFirstIfd < KTiffSignatureLength)   
		{
		User::Leave(KErrCorrupt);
		}
	
	SetStartPosition(iFormatInfo.iFirstIfd);

	// Set max data length, since header does not contain this information.
	SetDataLength(KMaxTInt);
	}

CFrameInfoStrings* CTiffDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{

	const TUid KTiffCodecDllUid = {KTIFFCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KTiffCodecDllUid,resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	const CFrameImageData& frameData = FrameData(aFrameNumber);
	const TTiffImageData* tiffImageData = STATIC_CAST(const TTiffImageData*, frameData.GetFrameData(0));

	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex;
	switch (tiffImageData->iCompression)
		{
		case TTiffIfdEntry::EGroup3FaxCompression:
			if (tiffImageData->iT4Options & TTiffIfdEntry::ET4TwoDimentionalCoding)
				formatIndex = EFormatGroup3Fax2D;
			else
				formatIndex = EFormatGroup3Fax1D;
			break;

		case TTiffIfdEntry::EGroup4FaxCompression:
			formatIndex = EFormatGroup4Fax;
			break;

		default:
			formatIndex = EFormatUnknown;
			break;
		}
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetFormatL(info);

	TInt width = frameInfo.iOverallSizeInPixels.iWidth;
	TInt height = frameInfo.iOverallSizeInPixels.iHeight;
	TInt depth = frameInfo.iBitsPerPixel;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	formatIndex = (frameInfo.iFlags & TFrameInfo::EColor) ? 1 : 0;
	templte = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	info.Format(templte, depth);
	frameInfoStrings->SetDepthL(info);

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

TInt CTiffDecoder::CurrentFilePosition()
	{
	return StartPosition() + Position();
	}

