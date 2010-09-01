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
#include <101F45F1_extra.rsg>
#include "PPM1Codec.h"
#include "PPM1Panic.h"
#include "PPMDecs.h"
#include "PPM1Uids.hrh"

_LIT(KPPM1PanicCategory, "PPM1ConvertPlugin");


// Global panic function
GLDEF_C void Panic(TInt aError)
	{
	User::Panic(KPPM1PanicCategory, aError);
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
	delete iExtensionManager;
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
	CPpmReadCodec* imageReadCodec = CPpmReadCodec::NewL(*this); 
	SetImageReadCodec(imageReadCodec);
	
	if (!iExtensionManager)
		{
		// Manager settings persist over all frames.
		iExtensionManager = CPluginExtensionManager::NewL(imageReadCodec);
		}
	else
		{
		// Maintain manager settings, but reset the codec that it points to.
		iExtensionManager->ResetCodecExtension(imageReadCodec);
		}
	
	imageReadCodec->SetExtensionManager(iExtensionManager);

	ReadFrameHeadersL();
	}

const TInt KPpmFileHeaderSize = 512; // use a high figure to get around possible comments - assumes high enough

void CPpmDecoder::ReadFormatL()
	{
	TPtrC8 bufferDes;

	ReadDataL(0, bufferDes, KPpmFileHeaderSize);

	TLex8 lex (bufferDes);
	TRAPD(error, InternalizeHeaderL(lex));

	if (error!=KErrNone)
		{
		if (error==KErrGeneral) // treat as if underflow - means expected integers were not present
			{
			error = KErrUnderflow;
			}			
		User::Leave(error);
		}
	}

void CPpmDecoder::InternalizeHeaderL(TLex8& aLex)
	{
	SetInComment(EFalse); // since we read from the top, always assume is clear

	// first check we have either P3 or P6 at top of file
	TChar char1 = aLex.Get();
	if (char1!='P')
		{
		User::Leave(KErrNotSupported);
		}		
	TChar char2 = aLex.Get();
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

	SkipCommentAndWhiteSpaceL(aLex);

	TInt width;
	User::LeaveIfError(aLex.Val(width));

	SkipCommentAndWhiteSpaceL(aLex);

	TInt height;
	User::LeaveIfError(aLex.Val(height));

	SkipCommentAndWhiteSpaceL(aLex);

	TInt maxValue;
	User::LeaveIfError(aLex.Val(maxValue));
	if (maxValue>255)
		{
		User::Leave(KErrNotSupported);
		}		

	TInt bitsPerPixel, power;
	for (bitsPerPixel=1, power=2; maxValue>power-1; power<<=1, bitsPerPixel+=1)
		/*loop*/;

	if (Compressed()) 
		{
		aLex.Inc(); // skip over just the end of line
		}		

	// we are now pointing at the data

	SetStartPosition(aLex.Offset());
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

// looks for white space or comment, and will skip if found
void CPpmDecoder::SkipCommentAndWhiteSpaceL(TLex8& aLex)
	{
	if (InComment())
		{
		DoSkipCommentL(aLex);
		}		
	for (;;)
		{
		ASSERT(!InComment());
		TChar peek = aLex.Peek();
		if (peek==0) // overflowed end of descriptor
			{
			User::Leave(KErrUnderflow);
			}			
		else if (peek.IsSpace())
			{
			aLex.SkipSpace();
			}			
		else if (peek=='#') // comment - skip to end of line
			{
			DoSkipCommentL(aLex);
			}			
		else
			{
			break;
			}			
		}
	}

// we are in comment, so skip to end of line. If do not get there, register the fact
void CPpmDecoder::DoSkipCommentL(TLex8& aLex)
	{
	TChar ch;
	do
		{
		ch = aLex.Get();
		}
	while (ch != '\n');
	if (ch==0) // reached end of descriptor
		{
		SetInComment(ETrue);
		User::Leave(KErrUnderflow);
		}
	else
		{
		SetInComment(EFalse);
		}			
	}


CFrameInfoStrings* CPpmDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	if (aFrameNumber!=0)
		{
		User::Leave(KErrArgument);
		}		

	const TUid ppmCodecDllUid = {KPpm1DecoderDllUidValue};

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

void CPpmDecoder::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	ASSERT(iExtensionManager);
	if(!Compressed())
		{
		User::Leave(KErrNotSupported);
		}
	iExtensionManager->GetExtensionL(aExtUid, aExtPtr);
	}

void CPpmDecoder::SetClippingRectL(const TRect* aClipRect)
	{
	if(!Compressed())
		{
		User::Leave(KErrNotSupported);
		}
	
	RPointerArray<TFrameInfo> frameInfo;
	CleanupClosePushL(frameInfo);
		
	// PPM only has a single frame
	frameInfo.AppendL(&FrameInfo(0));
	
	// The clipping operation is reset by passing a NULL rect pointer.
	ASSERT(iExtensionManager);
	iExtensionManager->SetClippingRectL(aClipRect, frameInfo);
	CleanupStack::PopAndDestroy(); // frameInfo
	}

TInt CPpmDecoder::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	ASSERT(iExtensionManager);
	const TFrameInfo frameInfo = FrameInfo(aFrameNumber);
	TSize originalSize = frameInfo.iOverallSizeInPixels;
	TInt err = iExtensionManager->GetDestinationSize(originalSize);
	if(err == KErrNone)
		{
		aSize = originalSize;
		}
	return err;
	}

// PNG encoder
CPpmEncoder* CPpmEncoder::NewL()
	{
	return new(ELeave) CPpmEncoder;
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
	CPpm1WriteCodec* codec = CPpm1WriteCodec::NewL();
	SetImageWriteCodec(codec);		// takes ownership of imageReadCodec
	}

void CPpmEncoder::UpdateHeaderL()
	{
	}

#ifndef EKA2

// DLL entry point
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return KErrNone;
	}

#endif // EKA2

