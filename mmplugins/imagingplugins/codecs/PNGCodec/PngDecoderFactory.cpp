// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <ecom/ecom.h>
#include "PNGCodec.h"

#include "PngDecoderFactory.h"



/*static*/
MImageStreamDecoderFactory* CPngImageStreamDecoderFactory::NewL()
	{
	return static_cast<MImageStreamDecoderFactory*>(new (ELeave) CPngImageStreamDecoderFactory());
	}


CPngImageStreamDecoderFactory::CPngImageStreamDecoderFactory()
	{
	}

CPngImageStreamDecoderFactory::~CPngImageStreamDecoderFactory()
	{
	REComSession::DestroyedImplementation(iDtorKey);
	}

// MImageStreamDecoderFactory // 
void CPngImageStreamDecoderFactory::SetDtorKey(TUid aUid)
	{
	iDtorKey = aUid;
	}

void CPngImageStreamDecoderFactory::Release()
	{
	ASSERT(iDtorKey.iUid);
	delete this;
	}

class CPngImage: protected CPngReadCodec, protected MPngDecoder, public MImageStreamDecoder 
	{
public:
	static CPngImage* NewL();

//  from the MImageStreamDecoder //
	virtual void InitL(CImageProcessor& aImageProcessor, CImageProcessor* aMaskProcessor, CFbsBitmap* aDestination, 
						const TPtrC8& aData, MUniqueChunkDataProvider& aProvider);
	virtual void DecodeL();
	virtual void ResetL();
	virtual void Release();
	virtual const TSize& ImageSize();	

protected:
	CPngImage();

	virtual void DoProcessPLTEL(const TUint8* aDataPtr,TInt aChunkLength);
	virtual void DoProcesstRNSL(const TUint8* aDataPtr,TInt aChunkLength);
	virtual void DoProcessbKGDL(const TUint8* aDataPtr,TInt aChunkLength);
	virtual void DoProcesspHYsL(const TUint8* aDataPtr,TInt aChunkLength);

	void InitFrameL();
// from the MPngDecoder  //
	void GoToProcessDataState();
	
protected:
	CPngImage(const TBufPtr8& aData);
	TFrameInfo iFrameInfo;
	TBufPtr8 iCurrentData;
	TBufPtr8 iData;
	MUniqueChunkDataProvider* iChunkProvider;
	TBool iDataProcessing;
	TBool ibKGDChunkFound;
	TBool ipHYsChunkFound;
	TBool itRNSChunkFound;
	CFbsBitmap* iCurrentFrame; //destination bitmap; not owned.
	};

/*static*/
CPngImage* CPngImage::NewL()
	{
	return new (ELeave) CPngImage();
	}

CPngImage::CPngImage():CPngReadCodec(static_cast<MPngDecoder&>(*this))
	{
	}

void CPngImage::Release()
	{
	delete this;
	}

void CPngImage::GoToProcessDataState()
	{
	iDataProcessing = ETrue;
	}

void CPngImage::InitL(CImageProcessor& aImageProcessor, CImageProcessor* aMaskProcessor, CFbsBitmap* aDestination,
						const TPtrC8& aData, MUniqueChunkDataProvider& aProvider)
	{
	CPngReadCodec::ConstructL();
	iCurrentFrame = aDestination;
	SetImageProcessor(&aImageProcessor, EFalse);
	if (NULL != aMaskProcessor)
		{
		SetMaskProcessor(aMaskProcessor, EFalse);
		}
	iCurrentData.Set(aData);
	iData.Set(aData);
	iChunkProvider = &aProvider;

	TFrameInfo inf; 
	inf.SetCurrentFrameState(TFrameInfo::EFrameInfoUninitialised);
	CFrameImageData* DummyData=NULL;
	InitFrameHeader(inf, *DummyData );
	ProcessFrameHeaderL( iCurrentData );
	if (!ibKGDChunkFound)
		{
		const TUint8* Ptr;
		TInt ChunkLength;
		if (KErrNone == iChunkProvider->GetChunkData(KPngbKGDChunkId().Ptr(), Ptr, ChunkLength))
			{
			DoProcessbKGDL(Ptr, ChunkLength);
			}
		}
	if (!ipHYsChunkFound)
		{
		const TUint8* Ptr;
		TInt ChunkLength;
		if (KErrNone == iChunkProvider->GetChunkData(KPngpHYsChunkId().Ptr(), Ptr, ChunkLength))
			{
			DoProcesspHYsL(Ptr, ChunkLength);
			}
		}
	InitFrameL();
	}

void CPngImage::ResetL()
	{
	iCurrentData.Set(iData);
	iDataProcessing = EFalse;
	ibKGDChunkFound = EFalse;
	ipHYsChunkFound	= EFalse;
	itRNSChunkFound = EFalse;
	InitFrameL();
	}

const TSize& CPngImage::ImageSize()
	{
	return iImageInfo.iSize;
	}

void CPngImage::DoProcessPLTEL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	if (0 == aChunkLength)
		{
		const TUint8* Plte;
		if (KErrNone == iChunkProvider->GetChunkData(KPngPLTEChunkId().Ptr(), Plte, aChunkLength))
			{
			// try to get inherited tRNS as well...
			const TUint8* Trns;
			TInt TrnsLen=0;
			if (!itRNSChunkFound && KErrNone == iChunkProvider->GetChunkData(KPngtRNSChunkId().Ptr(), Trns, TrnsLen))
				{
				CPngReadCodec::DoProcesstRNSL(Trns, TrnsLen); 
				}
			aDataPtr = Plte;
			}
		}
	CPngReadCodec::DoProcessPLTEL(aDataPtr, aChunkLength);
	}

void CPngImage::DoProcesstRNSL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	if (0 == aChunkLength)
		{
		const TUint8* Trns;
		if (KErrNone == iChunkProvider->GetChunkData(KPngtRNSChunkId().Ptr(), Trns, aChunkLength))
			{
			aDataPtr = Trns;
			}
		}
	itRNSChunkFound = (0 != aChunkLength);
	CPngReadCodec::DoProcesstRNSL(aDataPtr,aChunkLength);
	}

void CPngImage::DoProcessbKGDL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	if (0 == aChunkLength || ibKGDChunkFound)
		{
		User::Leave(KErrCorrupt);
		}
	ibKGDChunkFound = ETrue;
	CPngReadCodec::DoProcessbKGDL(aDataPtr,aChunkLength);
	}

void CPngImage::DoProcesspHYsL(const TUint8* aDataPtr,TInt aChunkLength)
	{
	if (0 == aChunkLength || ipHYsChunkFound)
		{
		User::Leave(KErrCorrupt);
		}
	ipHYsChunkFound = ETrue;
	CPngReadCodec::DoProcessbKGDL(aDataPtr,aChunkLength);
	}

void CPngImage::InitFrameL()
	{
	iChunkBytesRemaining = 0;
	iChunkId = KNullDesC8;

		
	if (iDecoder == NULL) 
		{	
		TBool fastProcessorMode = EFalse;
		CFastProcessor* fastProc = NULL;
		SetFastProcessor(NULL);
		
		/*Check if Image processor is to be ignored. 
		Ignore Image processor only when decoding 24 or 32 bpp images.
		In case of non interlaced images and if destination and source height/width differ then don't skip Image processor.
		*/
		if (((iImageInfo.iBitDepth == 8) && (iImageInfo.iColorType == TPngImageInformation::EDirectColor || iImageInfo.iColorType == TPngImageInformation::EAlphaDirectColor)) && (iImageInfo.iInterlaceMethod == TPngImageInformation::ENoInterlace) && (iImageInfo.iTransparencyPresent == EFalse ))
			{
			
			CFbsBitmap* destBitmap=NULL;		
			
			fastProc = CFastProcessor::NewL(iImageInfo, destBitmap, NULL, ETrue);
			SetFastProcessor(fastProc);
			fastProcessorMode = ETrue;
			}
		
		iDecoder = CPngReadSubCodec::NewL(ImageProcessor(), MaskProcessor(), iImageInfo , FastProcessor(), fastProcessorMode);
		iDecoder->SetRgbaMode(ETrue);		
		}

	if (iDecompressor == NULL)
		{
		iDecompressor = CEZDecompressor::NewL(*this);
		}
		
	}

void CPngImage::DecodeL()
	{
	const TPoint ZeroPoint(0,0);
	const TRect ImageRect(ZeroPoint, ImageSize().AsPoint());

	if ((iImageInfo.iTransparencyPresent || (iImageInfo.iColorType & TPngImageInformation::EAlphaChannelUsed))
		 && MaskProcessor() )
		{		
		
		MaskProcessor()->PrepareL(*iCurrentFrame, ImageRect);
		MaskProcessor()->SetPos(ZeroPoint);
		}

	ImageProcessor()->PrepareL(*iCurrentFrame, ImageRect);
	ImageProcessor()->SetPos(ZeroPoint);

	while (EFrameIncomplete == ProcessFrameL( iCurrentData )) 
		{
		if (iDataProcessing)
			{
			while (  ! DoProcessDataL() ) 
				{
				(void)0;
				}
			}
		iDataProcessing = EFalse;
		}
	delete iDecompressor;
	iDecompressor=NULL;
	iDecoder->ResetL();
	}

// MImageStreamDecoderFactory // 
void CPngImageStreamDecoderFactory::CreatePngDecoderL(MImageStreamDecoder*& aPtr)
	{
	aPtr = NULL;
	aPtr = static_cast<MImageStreamDecoder*>(  CPngImage::NewL() );
	}

