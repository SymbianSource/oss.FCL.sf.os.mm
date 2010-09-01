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
#include <101F45B5_extra.rsg>
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include "WMFConvert.h"

_LIT(KWMFPanicCategory, "WMFConvertPlugin");

// Constants.
const TInt KWmfMaxFileHeaderSize = 40; // Max possible header size for all three Wmf types

const TInt KWmfHeaderSize = 18;
const TInt KWmfHeaderSizeInWords = KWmfHeaderSize / 2;
const TInt KWmfApmHeaderSize = 22;
const TInt KWmfApmHeaderSizeInWords = KWmfApmHeaderSize / 2;
const TInt KWmfClpHeaderSize = 16;
const TInt KWmfClpHeaderSizeInWords = KWmfClpHeaderSize / 2;

const TInt KWmfDataStartPosition = KWmfHeaderSize;
const TInt KWmfApmDataStartPosition = KWmfHeaderSize + KWmfApmHeaderSize;
const TInt KWmfClpDataStartPosition = KWmfHeaderSize + KWmfClpHeaderSize;

const TInt KWmfDefaultPixelSize = 100;

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KWMFPanicCategory, aError);
	}

CWmfDecoder* CWmfDecoder::NewL()
	{
	return new(ELeave) CWmfDecoder(EWmfUnknownSubType);
	}

CWmfDecoder* CWmfDecoder::NewStdL()
	{
	return new(ELeave) CWmfDecoder(EWmfStdSubType);
	}

CWmfDecoder* CWmfDecoder::NewApmL()
	{
	return new(ELeave) CWmfDecoder(EWmfApmSubType);
	}

CWmfDecoder* CWmfDecoder::NewClpL()
	{
	return new(ELeave) CWmfDecoder(EWmfClpSubType);
	}

CWmfDecoder::CWmfDecoder(TWmfSubType aWmfSubType)
	:iWmfSubType(aWmfSubType), iRFbsSessionIsOurs(EFalse)
	{
	}

CWmfDecoder::~CWmfDecoder()
	{
	Cleanup();
	delete iDevice; // Created in ScanDataL() 
	if (iRFbsSessionIsOurs)
		{
		RFbsSession::Disconnect();
		}
	}

void CWmfDecoder::Cleanup()
	{
	// Delete any objects we should get rid of

	// Base class included
	CImageDecoderPlugin::Cleanup();
	}

void CWmfDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeWMFUid;
	if (iWmfSubType == EWmfStdSubType)
		aImageSubType = KImageTypeWMFSubTypeStdUid;
	else if (iWmfSubType == EWmfApmSubType)
		aImageSubType = KImageTypeWMFSubTypeApmUid;
	else
		aImageSubType = KImageTypeWMFSubTypeClpUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CWmfDecoder::ScanDataL()
	{
	ReadFormatL();
	
	ASSERT(ImageReadCodec() == NULL);

	CWmfReadCodec* imageReadCodec;
	imageReadCodec = CWmfReadCodec::NewL(iWordsExpected);
	imageReadCodec->SetIgnoreViewportMetaData((DecoderOptions() & CImageDecoder::EOptionWmfIgnoreViewportMetaData) == CImageDecoder::EOptionWmfIgnoreViewportMetaData);
	SetImageReadCodec(imageReadCodec);

	ReadFrameHeadersL();
	}

void CWmfDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KWmfMaxFileHeaderSize);

	// Validate the header.
	if (bufferDes.Length() < KWmfMaxFileHeaderSize)
		User::Leave(KErrUnderflow);
	
	CheckDeviceL();

	const TUint16* ptr = REINTERPRET_CAST(const TUint16*, &bufferDes[0]);
	const TUint32* ptr32 = REINTERPRET_CAST(const TUint32*, ptr);
	if (iWmfSubType == EWmfUnknownSubType)
		{
		if ((ptr[0] == 0x01) && (ptr[1] == 0x09))
			iWmfSubType = EWmfStdSubType;
		else if ((ptr32[0] == 0x9AC6CDD7) && (ptr[2] == 0x0))
			iWmfSubType = EWmfApmSubType;
		else if ((ptr32[3] == 0x0) && (ptr[KWmfClpHeaderSizeInWords] == 0x01) && (ptr[KWmfClpHeaderSizeInWords+1] == 0x09))
			iWmfSubType = EWmfClpSubType;
		else
			User::Leave(KErrCorrupt);
		}

	TFrameInfo imageInfo;
	imageInfo = ImageInfo();
	TBool noSizeSpecified = EFalse;
	switch (iWmfSubType) // Set pixel and twips sizes
		{
	case EWmfStdSubType:
		if ((ptr[0] != 0x01) || (ptr[1] != 0x09))
			User::Leave(KErrCorrupt);

		noSizeSpecified = ETrue;
		break;
	case EWmfApmSubType:
		if ((ptr32[0] != 0x9AC6CDD7) || (ptr[2] != 0x0))
			User::Leave(KErrCorrupt);

		SetStartPosition(KWmfApmDataStartPosition);
		ProcessWmfApmHeaderL(ptr, imageInfo.iFrameCoordsInPixels, imageInfo.iOverallSizeInPixels, imageInfo.iFrameSizeInTwips);
		ptr += KWmfApmHeaderSizeInWords;
		break;
	case EWmfClpSubType:
		if ((ptr32[3] != 0x0) || (ptr[KWmfClpHeaderSizeInWords] != 0x01) || (ptr[KWmfClpHeaderSizeInWords+1] != 0x09))
			User::Leave(KErrCorrupt);

		SetStartPosition(KWmfClpDataStartPosition);
		ProcessWmfClpHeaderL(ptr, imageInfo.iFrameCoordsInPixels, imageInfo.iOverallSizeInPixels, imageInfo.iFrameSizeInTwips);
		ptr += KWmfClpHeaderSizeInWords;
		break;
	default:
		Panic(EUndefinedSourceType);
		}

	iWordsExpected = ((ptr[4] << 16) | ptr[3]) - KWmfHeaderSizeInWords;
	if (iWordsExpected < 0 )
		User::Leave(KErrCorrupt);

	SetDataLength(iWordsExpected << 1);

	imageInfo.iBitsPerPixel = 24;
	imageInfo.iDelay = 0;

	imageInfo.iFlags = TFrameInfo::EColor | TFrameInfo::EFullyScaleable;
	if (iMaskGenerationEnabled)
		imageInfo.iFlags |= TFrameInfo::ETransparencyPossible;
	
	imageInfo.iFrameDisplayMode = EColor16M;
	if (noSizeSpecified)
		{
		TSize imageSize(KWmfDefaultPixelSize, KWmfDefaultPixelSize);
		TRAP_IGNORE(imageSize = FindSetWindowExtL());
		SetStartPosition(KWmfDataStartPosition);
		imageInfo.iFrameCoordsInPixels = TRect(imageSize);
		imageInfo.iFrameSizeInTwips.iWidth = iDevice->HorizontalPixelsToTwips(imageSize.iWidth);
		imageInfo.iFrameSizeInTwips.iHeight = iDevice->VerticalPixelsToTwips(imageSize.iHeight);
		imageInfo.iOverallSizeInPixels = imageSize;
		}

	SetImageInfo(imageInfo);
	}

TSize CWmfDecoder::FindSetWindowExtL()
	{
	TPtrC8 recordDes;

	TSize imageSize(KWmfDefaultPixelSize, KWmfDefaultPixelSize);
	TInt readOffset = KWmfDataStartPosition;
	TBool lastRecord = EFalse;

	TSize windowExt(KWmfDefaultPixelSize, KWmfDefaultPixelSize);
	
	while (!lastRecord)
		{
		// read record info			
		TInt dataLengthBytes = KWmfMinRecordSizeInWords * 2;
		ReadDataL(readOffset, recordDes, dataLengthBytes);
		if (recordDes.Length() != dataLengthBytes)
			{
			User::Leave(KErrUnderflow);
			}
		readOffset += dataLengthBytes;

		const TUint16* dataPtr = REINTERPRET_CAST(const TUint16*, &recordDes[0]);
		TInt recordSizeInWords = (dataPtr[1] << 16) | dataPtr[0];
		if (recordSizeInWords < KWmfMinRecordSizeInWords || recordSizeInWords > iWordsExpected)
			{
			User::Leave(KErrCorrupt);
			}
		TInt function = dataPtr[2];
			
		dataLengthBytes = (recordSizeInWords * 2) - dataLengthBytes;
		
		// read record data

		switch (function)
			{
		case 0x020c: // SETWINDOWEXT
			{
			const TInt16* data = NULL;
			if (dataLengthBytes)
				{
				ReadDataL(readOffset, recordDes, dataLengthBytes);
				if (recordDes.Length() != dataLengthBytes)
					{
					User::Leave(KErrUnderflow);
					}
				data = REINTERPRET_CAST(const TInt16*, &recordDes[0]);
				}
			if(data==NULL)
				{
				User::Leave(KErrCorrupt);	
				}

			windowExt.iWidth = Abs(data[1]);
			windowExt.iHeight = Abs(data[0]);
			break;
			}
		case 0x0000: // last record
			{
			lastRecord = ETrue;
			break;
			}
		default:
			break;
			}

		readOffset += dataLengthBytes;
		}

	return windowExt;
	}

void CWmfDecoder::ProcessWmfApmHeaderL(const TUint16* aData, TRect& aFrameCoords, TSize& aOverallSize, TSize& aFrameSize)
	{
	TInt unitsPerInch = aData[7];
	if (unitsPerInch <= 0)
		unitsPerInch = KTwipsPerInch;

	const TInt16* coordPtr = REINTERPRET_CAST(const TInt16*, aData + 3);
	TInt horzTwips = (coordPtr[2] - coordPtr[0]) * KTwipsPerInch / unitsPerInch;
	TInt horzPixels = iDevice->HorizontalTwipsToPixels(horzTwips);

	TInt vertTwips = (coordPtr[3] - coordPtr[1]) * KTwipsPerInch / unitsPerInch;
	TInt vertPixels = iDevice->VerticalTwipsToPixels(vertTwips);

	aFrameCoords.iTl.SetXY(0, 0);
	aFrameCoords.iBr.iX = horzPixels;
	aFrameCoords.iBr.iY = vertPixels;

	aOverallSize.SetSize(horzPixels, vertPixels);
	aFrameSize.SetSize(horzTwips, vertTwips);
	}

void CWmfDecoder::ProcessWmfClpHeaderL(const TUint16* aData, TRect& aFrameCoords, TSize& aOverallSize, TSize& aFrameSize)
	{
	const TInt32* coordPtr = REINTERPRET_CAST(const TInt32*, aData);
	TInt mappingMode = coordPtr[0];
	TSize size(coordPtr[1], coordPtr[2]);

	switch (mappingMode)
		{
	case 1: // Text
	case 7: // Isotropic
	case 8: // Anisotropic
	case 2: // Low metric 0.1mm
		size.iWidth = size.iWidth * KTwipsPerInch / 254;
		size.iHeight = size.iHeight * KTwipsPerInch / 254;
		break;
	case 3: // High metric 0.01mm
		size.iWidth = size.iWidth * KTwipsPerInch / 2540;
		size.iHeight = size.iHeight * KTwipsPerInch / 2540;
		break;
	case 4: // Low English 0.01"
		size.iWidth = size.iWidth * KTwipsPerInch / 100;
		size.iHeight = size.iHeight * KTwipsPerInch / 100;
		break;
	case 5: // High English 0.001"
		size.iWidth = size.iWidth * KTwipsPerInch / 1000;
		size.iHeight = size.iHeight * KTwipsPerInch / 1000;
		break;
	case 6: // Twips
		break;
	default:
		aFrameCoords.SetRect(TPoint(0, 0), size);
		aOverallSize = size;
		aFrameSize.iWidth = iDevice->HorizontalPixelsToTwips(size.iWidth);
		aFrameSize.iHeight = iDevice->VerticalPixelsToTwips(size.iHeight);
		return;
		}

	TInt horzPixels = iDevice->HorizontalTwipsToPixels(size.iWidth);
	TInt vertPixels = iDevice->VerticalTwipsToPixels(size.iHeight);
	aFrameSize = size;
	aFrameCoords.iTl.SetXY(0, 0);
	aFrameCoords.iBr.iX = horzPixels;
	aFrameCoords.iBr.iY = vertPixels;
	aOverallSize.SetSize(horzPixels, vertPixels);
	}

void CWmfDecoder::CheckDeviceL()
	{
	if (NULL == RFbsSession::GetSession())
		{	
		TInt err = RFbsSession::Connect(); 
		User::LeaveIfError(err);

		iRFbsSessionIsOurs = ETrue;
		}
	
	const TInt KNumDisplayModes = 12;
	const TDisplayMode KDisplayMode[KNumDisplayModes] = { EGray2, EGray4, EGray16, EGray256, EColor16, EColor256,
														EColor4K, EColor64K, EColor16M, EColor16MU, EColor16MA, EColor16MAP };

	if (iDevice == NULL)
		{
		TInt err = KErrNotSupported;

		for (TInt index = 0; index < KNumDisplayModes && err == KErrNotSupported; index++)
			{
			ASSERT(iDevice==NULL);
			TRAP(err,iDevice = CFbsScreenDevice::NewL(_L("scdv"), KDisplayMode[index]));
			}

		User::LeaveIfError(err);

		ASSERT(iDevice);
		}
	}

CFrameInfoStrings* CWmfDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{

	const TUid KWmfCodecDllUid = {KWMFCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KWmfCodecDllUid,resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex = iWmfSubType-1;
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetFormatL(info);

	TInt width = frameInfo.iFrameSizeInTwips.iWidth;
	TInt height = frameInfo.iFrameSizeInTwips.iHeight;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);
 
	info = resourceReader.ReadTPtrC(); // depth is fixed
	frameInfoStrings->SetDepthL(info);

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

void CWmfDecoder::EnableMaskGeneration()
	{
	iMaskGenerationEnabled = ETrue;
	}

