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
#include <101F45CD_extra.rsg>
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include "WBMPCodec.h"

// Constants.
const TInt KWbmpMaxFileHeaderSize = 12; // Max possible header size for TInt32 width and height values
const TInt KWbmpMinFileHeaderSize = 4;	// Min possible header size for TUint8 width and height values

_LIT(KWBMPPanicCategory, "WBMPConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KWBMPPanicCategory, aError);
	}


// Wbmp decoder.
CWbmpDecoder* CWbmpDecoder::NewL()
	{
	return new(ELeave) CWbmpDecoder;
	}

CWbmpDecoder::CWbmpDecoder()
	{}

CWbmpDecoder::~CWbmpDecoder()
	{
	Cleanup();
	}

void CWbmpDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypeWBMPUid;
	aImageSubType = KNullUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CWbmpDecoder::ScanDataL()
	{
	ReadFormatL();

	ASSERT(ImageReadCodec() == NULL);

	CWbmpReadCodec* imageReadCodec = CWbmpReadCodec::NewL(iSize);

	SetImageReadCodec(imageReadCodec);
	
	ReadFrameHeadersL();
	}

void CWbmpDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KWbmpMaxFileHeaderSize);

	// Validate the header.
	if (bufferDes.Length() < KWbmpMinFileHeaderSize)
		User::Leave(KErrUnderflow);

	const TUint8* ptr = &bufferDes[0];
	const TUint8* startPtr = ptr;
	const TUint8* endPtr = startPtr + bufferDes.Length();

	TUint8 typeField = *ptr++;
	if (typeField != 0)
		User::Leave(KErrNotSupported);

	// TUint8 fixHeaderField = *ptr++;
	ptr++; // Skip fixHeaderField 

	iSize.iWidth = ReadContinuedValueL(ptr, endPtr);
	iSize.iHeight = ReadContinuedValueL(ptr, endPtr);

	TInt headerSize = ptr - startPtr;
	SetStartPosition(headerSize); // KWbmpFileHeaderSize ??

	SetDataLength(headerSize + (((iSize.iWidth + 7) / 8) * iSize.iHeight));

	TFrameInfo imageInfo;
	imageInfo = ImageInfo();
	imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), iSize);
	imageInfo.iOverallSizeInPixels = iSize;
	imageInfo.iFrameSizeInTwips = TSize(0, 0);
	imageInfo.iBitsPerPixel = 1;
	imageInfo.iDelay = 0;
	imageInfo.iFlags = 0;
	imageInfo.iFrameDisplayMode = EGray2;
	SetImageInfo(imageInfo);
	}

CFrameInfoStrings* CWbmpDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{

	const TUid KWbmpCodecDllUid = {KWBMPCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KWbmpCodecDllUid,resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetFormatL(info);

	TInt width = frameInfo.iOverallSizeInPixels.iWidth;
	TInt height = frameInfo.iOverallSizeInPixels.iHeight;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	info = resourceReader.ReadTPtrC(); // note depth is fixed
	frameInfoStrings->SetDepthL(info);

	// leave details blank

	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}


TInt CWbmpDecoder::ReadContinuedValueL(const TUint8*& aPtr, const TUint8* aEndPtr)
	{
	TUint32 value = 0;
	TInt concatenation;
    TInt bytecount = 0;
	do
		{
		if (aPtr == aEndPtr)
			{
			User::Leave(KErrUnderflow);
			}
		
		if ( bytecount > sizeof(TUint32) )
			{
				User::Leave(KErrCorrupt);
				break;
			}
		value <<= 7;
		value |= (aPtr[0] & 0x7f);
		if ( value > KWBmpMaxImageSize )
				{
				User::Leave(KErrCorrupt);
				break;
				}
		concatenation = (aPtr[0] & 0x80);
		aPtr++;
		bytecount++;
		}
	while (concatenation);
	if ( value > KWBmpMaxImageSize )
			{
			User::Leave(KErrCorrupt);
			}
	return value;
	}



