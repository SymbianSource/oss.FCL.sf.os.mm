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
#include <101F45F2_extra.rsg>
#include "PPM2Codec.h"
#include "PPM2Panic.h"
#include "PPMDecs.h"
#include "PPM2Uids.hrh"
// #include "PPMData.h" TODO do we need this

_LIT(KPPM2PanicCategory, "PPM2ConvertPlugin");


// Global panic function
GLDEF_C void Panic(TInt aError)
	{
	User::Panic(KPPM2PanicCategory, aError);
	}


// decoder.
CPpmDecoder* CPpmDecoder::NewL()
	{
	return new (ELeave) CPpmDecoder;
	}

CPpmDecoder::CPpmDecoder()
	{}

CPpmDecoder::~CPpmDecoder()
	{
	Cleanup();
	}

void CPpmDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS(aFrameNumber == 0, Panic(EFrameNumberOutOfRange));
	aImageType = KImageTypePpmUid;
	aImageSubType = KNullUid;
	}

// Scan header.
// Validate that format is correct.
// Create codec.
// Fill in image info. (All frames)
void CPpmDecoder::ScanDataL()
	{
	ReadFormatL();
	
	ASSERT(ImageReadCodec() == NULL);
	CImageReadCodec* imageReadCodec = CPpmReadCodec::NewL(*this); 
	SetImageReadCodec(imageReadCodec);

	ReadFrameHeadersL();
	}

const TInt KPpmFileHeaderSize = 512; // use a high figure to get around possible comments - assumes high enough

void CPpmDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;
	ReadDataL(0, bufferDes, KPpmFileHeaderSize); // does setup too, but we don't use the buffer

	SetStartPosition(0);
	ResetPosition(0);
	InternalizeHeaderL();
	}

void CPpmDecoder::InternalizeHeaderL()
	{
	// first check we have either P3 or P6 at top of file
	TChar char1 = GetByteL();
	if (char1!='P')
		{
		User::Leave(KErrNotSupported);
		}		
	TChar char2 = GetByteL();
	if (char2=='3')
		{
		SetCompressed(EFalse);
		}		
	else if (char2=='6')
		{
		SetCompressed(ETrue);
		}		
	else
		{
		User::Leave(KErrNotSupported);
		}		

	SkipCommentAndWhiteSpaceL();

	TInt width = ReadIntL();

	SkipCommentAndWhiteSpaceL();

	TInt height = ReadIntL();

	SkipCommentAndWhiteSpaceL();

	TInt maxValue = ReadIntL();
	if (maxValue>255)
		{
		User::Leave(KErrNotSupported);
		}		

	TInt bitsPerPixel, power;
	for (bitsPerPixel=1, power=2; maxValue>power-1; power<<=1, bitsPerPixel+=1)
		/*loop*/;

	if (Compressed()) 
		{
		IncByte(); // skip over just the end of line
		}		

	// we are now pointing at the data

	SetStartPosition(Position());
	SetDataLength(KMaxTInt); // we don't know image size, so set big

	TSize size = TSize(width, height);
	iMaxValue = maxValue;

	TFrameInfo imageInfo = ImageInfo();
	imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), size);
	imageInfo.iOverallSizeInPixels = size;
	imageInfo.iFrameSizeInTwips = TSize(0, 0);
	imageInfo.iBitsPerPixel = bitsPerPixel;
	imageInfo.iDelay = 0;
	imageInfo.iFlags = TFrameInfo::EColor|TFrameInfo::ECanDither;
	TDisplayMode mode;
	if (bitsPerPixel<=4) // we always have rgb values
		{
		mode=EColor4K;
		}		
	else
		{
		mode=EColor16M;
		}		
	imageInfo.iFrameDisplayMode = mode;
	SetImageInfo(imageInfo);

	iDataShift = 8 - bitsPerPixel; // correct everything to 0..255
	ASSERT(iDataShift>=0);
	}

CFrameInfoStrings* CPpmDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	if (aFrameNumber!=0)
		{
		User::Leave(KErrArgument);
		}		

	const TUid ppmCodecDllUid = {KPpm2DecoderDllUidValue};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs,ppmCodecDllUid,resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<128> info;
	TBuf<128> templte;

	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetFormatL(info);

	templte = resourceReader.ReadTPtrC();
	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	const TSize& size = frameInfo.iOverallSizeInPixels;
	info.Format(templte, size.iWidth, size.iHeight);
	frameInfoStrings->SetDimensionsL(info);

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, iMaxValue);
	frameInfoStrings->SetDepthL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex = Compressed() ? 1 : 0;
	info = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	frameInfoStrings->SetDetailsL(info);
	
	CleanupStack::Pop(frameInfoStrings); 
	CleanupStack::PopAndDestroy(2); // resourceInfo + resourceFile
	return frameInfoStrings;
	}

//

void CPpmDecoder::DoConvert()
	{
	CPpmReadCodec* codec = STATIC_CAST(CPpmReadCodec*, ImageReadCodec());
	ASSERT(ValidDestination());
	TInt error=KErrNone;
#if !defined(__CONTINUE_CONVERT)
	TRAP(error, codec->ResetFrameL(*iFrameInfo[iCurrentFrame], Destination()));
#endif // !defined(__CONTINUE_CONVERT)
	if (error==KErrNone)
		{
		TRAP(error, codec->ProcessFrameL());
		}
				
	ImageReadCodec()->Complete();
	RequestComplete(error);
	}

//

TUint CPpmDecoder::GetByteL()
	{
	TUint result = PeekByteL();
	IncByte();
	return result;
	}

void CPpmDecoder::IncByte()
	{
	TInt position = Position();
	SetPosition(position + 1);
	}

TUint CPpmDecoder::PeekByteL()
	{
	TInt offset = Position() - iBase + StartPosition();

	if (!BytesValid() || !(offset>=0 && offset<SourceLength()))
		{
		TInt base = StartPosition() + Position();
		TRAPD(error, ReadDataL(base, iSourceBuffer, FrameBlockSize(0)));
		if (error!=KErrNone)
			{
			if (error == KErrEof) // should probably not occur, but just in case TODO
				{
				error = KErrUnderflow;
				}
				
			User::Leave(error);
			}
		if (iSourceBuffer == KNullDesC8) // no more data
			{
			User::Leave(KErrUnderflow);
			}
			
		iBase = base;
		offset = 0; // really iStartPosition + iPosition - iBase
		SetBytesValid(ETrue);
		ASSERT(offset>=0 && offset<SourceLength());
		}
 	return iSourceBuffer[offset];
	}

void CPpmDecoder::ResetPosition(TInt aPosition)
	{
	SetPosition(aPosition);
	SetBytesValid(EFalse);
	}

TInt CPpmDecoder::ReadIntL()
	{
	TInt result = 0;
	for(;;)
		{
		TChar ch(PeekByteL());
		if (!ch.IsDigit())
			{
			break;
			}			
		IncByte();
		TInt val = TInt(ch) - TInt('0');
		result = result*10 + val;
		}
	return result;
	}

void CPpmDecoder::SkipCommentAndWhiteSpaceL()
	{
	for (;;)
		{
		TChar peek = PeekByteL();
		if (peek.IsSpace())
			{
			do 
				{
				IncByte();
				peek = PeekByteL();
				}
			while(peek.IsSpace());
			}
		else if (peek=='#') // comment - skip to end of line
			{
			do 
				{
				peek = GetByteL();
				}
			while(peek!='\n');
			}
		else
			break;
		}
	}

// PNG encoder
CPpmEncoder* CPpmEncoder::NewL()
	{
	return new (ELeave) CPpmEncoder;
	}

CPpmEncoder::CPpmEncoder()
	{
	}

CPpmEncoder::~CPpmEncoder()
	{
	CImageEncoderPlugin::Cleanup();
	}

void CPpmEncoder::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	CPpm2WriteCodec* codec = CPpm2WriteCodec::NewL(this);
	SetImageWriteCodec(codec);		// takes ownership of imageReadCodec
	}

void CPpmEncoder::UpdateHeaderL()
	{
	}

void CPpmEncoder::DoConvert()
	{
	TRAPD(error, DoConvertL()); // encapsulate call to reduce number of traps
	RequestComplete(error);
	}

void CPpmEncoder::DoConvertL()
	{
	static_cast<CPpm2WriteCodec*>(ImageWriteCodec())->DoProcessL(Source());

	FinishConvertL();
	}

void CPpmEncoder::AppendDataL(const TDesC8& aData)
	{
	TInt& position = Position();
	WriteDataL(position, aData);
	position += aData.Length();
	}

#ifndef EKA2

// DLL entry point
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return KErrNone;
	}

#endif // EKA2
