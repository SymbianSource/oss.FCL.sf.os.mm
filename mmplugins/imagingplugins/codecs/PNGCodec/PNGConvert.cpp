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
#include <101F45C7_extra.rsg>
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include "PNGCodec.h"


_LIT(KPNGPanicCategory, "PNGConvertPlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KPNGPanicCategory, aError);
	}


// Png decoder.
CPngDecoder* CPngDecoder::NewL()
	{
	return new(ELeave) CPngDecoder;
	}

CPngDecoder::CPngDecoder()
	{}

CPngDecoder::~CPngDecoder()
	{
	Cleanup();
	}

void CPngDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypePNGUid;
	aImageSubType = KNullUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CPngDecoder::ScanDataL()
	{
	ReadFormatL();
	
	ASSERT(ImageReadCodec() == NULL);

	CPngReadCodec* imageReadCodec;
	
	imageReadCodec = CPngReadCodec::NewL(*this);
	imageReadCodec->SetMissingiENDChunkFail(DecoderOptions() & CImageDecoder::EOptionPngMissingiENDFail);
	SetImageReadCodec(imageReadCodec);		// takes ownership of imageReadCodec

	ReadFrameHeadersL();
	}

void CPngDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KPngFileSignatureLength);

	// Validate the header.
	if (bufferDes.Length() < KPngFileSignatureLength)
		User::Leave(KErrUnderflow);

	const TUint8* ptr = bufferDes.Ptr();
	if (Mem::Compare(ptr, KPngFileSignatureLength, KPngSignature, KPngFileSignatureLength)!=0)
		User::Leave(KErrCorrupt);

	// Set start position.
	SetStartPosition(KPngFileSignatureLength);

	// Get this from the header?
	SetDataLength(KMaxTInt);
	}

CFrameInfoStrings* CPngDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{

	const TUid KPngCodecDllUid = {KPNGCodecDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,KPngCodecDllUid,resourceFile);

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
	TInt depth = frameInfo.iBitsPerPixel;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex = (frameInfo.iFlags & TFrameInfo::EColor) ? 1 : 0;
	templte = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	info.Format(templte, depth);
	frameInfoStrings->SetDepthL(info);

	if(frameInfo.iFlags & TFrameInfo::ETransparencyPossible)
		{
		resourceArray = resourceReader.ReadDesCArrayL();
		CleanupStack::PushL(resourceArray);
		formatIndex = (frameInfo.iFlags & TFrameInfo::EAlphaChannel) ? 1 : 0;
		info = (*resourceArray)[formatIndex];
		frameInfoStrings->SetDetailsL(info);
		CleanupStack::PopAndDestroy(resourceArray);
		}
	
	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

void CPngDecoder::InitConvertL()
	{
	iState = EStateStart; //  when starting convert, always start from the top
	CImageDecoderPlugin::InitConvertL();
	}

void CPngDecoder::NotifyComplete()
	{
	iState = EStateStart; // ensure that however we exit one run, start at top of next DoConvert()
	}

void CPngDecoder::DoConvert()
	{
	switch (iState)
		{
		case EStateStart:
			{
			TRAPD(errCode, PrepareForProcessFrameL());

			if (errCode!=KErrNone)
				{
				RequestComplete(errCode);
				return;
				}
			iState = EStateNormalProcess;
			}
			// fall through
		case EStateNormalProcess:
			{
			TFrameState codecState = EFrameIncomplete; 

			TBufPtr8& sourceData = SourceData();
			TInt errCode=KErrNone;
			if (sourceData.Length()!=0)
				TRAP(errCode, codecState = ImageReadCodec()->ProcessFrameL(sourceData));
			
			ASSERT(iState==EStateNormalProcess || iState==EStateDataProcess); // only valid states
			if (errCode!=KErrNone || iState==EStateNormalProcess)
				{
				HandleProcessFrameResult(errCode, codecState);
				iState = EStateStart;
				}
			else
				{
				ASSERT(iState==EStateDataProcess); // kick off a data process
				SelfComplete(KErrNone); // ask for next run
				}
			}
			break;
		case EStateDataProcess:
			{
			TBool finished = EFalse;
			CPngReadCodec* readCodec = static_cast<CPngReadCodec*>(ImageReadCodec());
			TRAPD(error, finished = readCodec->DoProcessDataL());
			if (error!=KErrNone)
				{
				ASSERT(error!=KErrUnderflow); // underflow should happen later
				RequestComplete(error); // will reset state
				return;
				}

			if (finished)
				{
				iState = EStateNormalProcess;
				}
			SelfComplete(KErrNone); // ask for next run, whatever we do
			}
			break;
		default:
			ASSERT(EFalse); // should not occur
		}
	}

// Called from codec when we need to swith to EStateDataProcess
void CPngDecoder::GoToProcessDataState()
	{
	ASSERT(iState==EStateNormalProcess); // should be in this state if we get here
	iState = EStateDataProcess;
	}

// PNG encoder
CPngEncoder* CPngEncoder::NewL()
	{
	return new(ELeave) CPngEncoder;
	}

CPngEncoder::CPngEncoder()
	{
	}

CPngEncoder::~CPngEncoder()
	{
	CImageEncoderPlugin::Cleanup();
	}

void CPngEncoder::PrepareEncoderL(const CFrameImageData* aFrameImageData)
	{
	// Default encode params
	TInt bpp = 24;
	TBool color = ETrue;
	TBool paletted = EFalse;
	TInt compressionLevel = TPngEncodeData::EDefaultCompression;

	// Use encode params in aFrameImageData, if present
	const TInt count = (aFrameImageData) ? aFrameImageData->FrameDataCount() : 0;
	for (TInt i=0; i < count; i++)
		{
		const TFrameDataBlock* encoderData = aFrameImageData->GetFrameData(i);
		if (encoderData->DataType() == KPNGEncodeDataUid)
			{
			const TPngEncodeData* pngEncodeData = static_cast<const TPngEncodeData*>(encoderData);
			bpp = pngEncodeData->iBitsPerPixel;
			color = pngEncodeData->iColor;
			paletted = pngEncodeData->iPaletted;
			compressionLevel = pngEncodeData->iLevel;
			}
		}

	// Create the codec
	CPngWriteCodec* codec = CPngWriteCodec::NewL(*this, bpp, color, paletted, compressionLevel);
	
	SetImageWriteCodec(codec);		// takes ownership of imageReadCodec

	}

void CPngEncoder::InitConvertL()
	{
	iState = EStateNormalProcess; //  when starting convert, always start from the top
	CImageEncoderPlugin::InitConvertL();
	}

void CPngEncoder::NotifyComplete()
	{
	iState = EStateNormalProcess; // ensure that however we exit one run, start at top of next DoConvert()
	}

void CPngEncoder::DoConvert()
	{
	switch(iState)
		{
		case EStateNormalProcess:
			{
			TFrameState codecState = EFrameIncomplete;

			TBufPtr8& destData = DestinationData();
			TRAPD(error, codecState = ImageWriteCodec()->ProcessFrameL(destData));

			if(error!=KErrNone || iState==EStateNormalProcess)
				HandleProcessFrameResult(error, codecState);
			else
				{
				ASSERT(iState==EStateDataProcess); // kick off a data process
				SelfComplete(KErrNone); // ask for next run
				}
			}
			break;

		case EStateDataProcess:
			{
			TBool finished = EFalse;
			CPngWriteCodec* writeCodec = static_cast<CPngWriteCodec*>(ImageWriteCodec());

			TRAPD(error, finished = writeCodec->DeflateEncodedDataL());

			if(error!=KErrNone)
				{
				RequestComplete(error); // will reset state
				return;
				}

			if(finished)
				iState = EStateNormalProcess;

			SelfComplete(KErrNone); // ask for next run, whatever we do
			}
			break;

		default:
			ASSERT(EFalse); // invalid state
		}
	}

void CPngEncoder::UpdateHeaderL()
	{
	}

void CPngEncoder::GoToProcessDataState()
	{
	iState = EStateDataProcess;
	}

