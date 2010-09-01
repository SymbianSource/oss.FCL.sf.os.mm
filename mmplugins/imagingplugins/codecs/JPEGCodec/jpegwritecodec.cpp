// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent
*/
#include <e32base.h>
#include "icl/ImageCodec.h"

#include "JpegTypes.h"
#include "rgbbufferptr.h"
#include "jpgimageframeprocessor.h"

#include "jpegwritecodec.h"

const TInt KDataUnitSafeBufferSz = sizeof(TDataUnit) + ( sizeof(TDataUnit) >> 1 ); // sizeof(TDataUnit) * 1.5

// Encoder tables.
static const TUint8 KLuminanceQTableValues[] = {
	16,  11,  12,  14,  12,  10,  16,  14,
	13,  14,  18,  17,  16,  19,  24,  40,
	26,  24,  22,  22,  24,  49,  35,  37,
	29,  40,  58,  51,  61,  60,  57,  51,
	56,  55,  64,  72,  92,  78,  64,  68,
	87,  69,  55,  56,  80,  109, 81,  87,
	95,  98,  103, 104, 103, 62,  77,  113,
	121, 112, 100, 120, 92,  101, 103, 99 };

static const TUint8 KChrominanceQTableValues[] = {
	17,  18,  18,  24,  21,  24,  47,  26,
	26,  47,  99,  66,  56,  66,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99,
	99,  99,  99,  99,  99,  99,  99,  99 };

static const TUint8 KLuminanceDCHuffmanValues[] = {
// Number of codes per bit-length
//  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 
	0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
// source byte definitions
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11 };



static const TUint8 KChrominanceDCHuffmanValues[] = {
// Number of codes per bit-length
//  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 
	0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
// source byte definitions
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };



static const TUint8 KLuminanceACHuffmanValues[] = {
// Number of codes per bit-length
//  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 
	0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d,
	0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
	0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
	0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa };
	
static const TUint8 KChrominanceACHuffmanValues[] = {
    // Number of codes per bit-length
	0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77,
	// Code value definitions
	0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
	0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
	0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
	0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
	0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa };

#if defined(PROFILE_JPEG_ENCODER)

#define PROFILE_START(a)\
			TUint a##_Start=User::FastCounter();
			
#define PROFILE_END(aa) aa += User::FastCounter() - aa##_Start;

#define PROFILE_MARK(aa) aa = User::FastCounter() - aa;			
			
#define PROFILE_REPORT(a)\
		RDebug::Printf("--Profile: %s\t%d",#a, a);
#else

#define PROFILE_START(a)
#define PROFILE_END(a)
#define PROFILE_REPORT(a)
#define PROFILE_MARK(a)
#endif


// CJpegWriteCodec
CJpgWriteCodec::CJpgWriteCodec(const TJpgFrameInfo& aFrameInfo,TInt aQualityFactor,TQTable* aReplacementLumaQTable,TQTable* aReplacementChromaQTable,const RPointerArray<HBufC8>& aComment)
	: iQualityFactor(aQualityFactor), iReplacementLumaQTable(aReplacementLumaQTable), iReplacementChromaQTable(aReplacementChromaQTable), iFrameInfo(aFrameInfo), iComment(aComment)
	{
	iWrittingDUIdx = KJpgMaxNumOfDataUnits;
// check that CJpgWriteCodec::TSpecialDataUnit has the right size, as we rely on that
// if it doesn't throw a compile-time error
    ASSERT( ( _FOFF(CJpgWriteCodec::TSpecialDataUnit,iExtraElement) == sizeof(TDataUnit) ) );
	}

CJpgWriteCodec::~CJpgWriteCodec()
	{
	iOperationsRequested.Close();
	delete iRgbInputBuffer;
	delete iDataUnitProcessor;
	delete iImageFrameCodecPtr;
	}

CJpgWriteCodec* CJpgWriteCodec::NewL(const TJpgFrameInfo& aFrameInfo,TInt aQualityFactor,TQTable* aReplacementLumaQTable,TQTable* aReplacementChromaQTable,const RPointerArray<HBufC8>& aComment,RArray<TUint>* aOperationsRequested)
	{
	CJpgWriteCodec* self = new(ELeave) CJpgWriteCodec(aFrameInfo, aQualityFactor, aReplacementLumaQTable, aReplacementChromaQTable, aComment);
	CleanupStack::PushL(self);
	self->ConstructL(aOperationsRequested);
	CleanupStack::Pop(self); 
	return self;
	}

void CJpgWriteCodec::ConstructL(RArray<TUint>* aOperationsRequested)
	{
	if(!aOperationsRequested)
		{
		iOperationsRequested.Reset();		
		}
	else
		{
		TInt count = aOperationsRequested->Count();
		for (TInt i = 0; i<count; i++)
			{
			iOperationsRequested.AppendL((*aOperationsRequested)[i]);
			}						
		}
	}

void CJpgWriteCodec::SetHighSpeedMode(TBool aHighSpeedMode)
	{
	iHighSpeedMode = aHighSpeedMode;
	}

TFrameState CJpgWriteCodec::ProcessFrameL(TBufPtr8& aDst)
	{
	
	TFrameState state;	
	TUint8* destStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = destStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength() - sizeof(iBitBuffer) * 2; // Subtract four to give a safety buffer
			
	if(iStreamConfig.iIsFrameComplete)
		{
		CompleteFrame();
		aDst.SetLength(iDestPtr - destStartPtr);
		state = EFrameComplete;
		if(iStreamConfig.iNumOfMCUsProcessed < ((iFrameInfo.iSizeInPixels.iWidth * iFrameInfo.iSizeInPixels.iHeight) / (iMCUSizeInPixels.iWidth * iMCUSizeInPixels.iHeight)))		
			{
			User::Leave(KErrUnderflow);
			}
		return state;
		}
	
	TRAPD(err,DoProcessL());	

	if (err != KErrNone)
		{
		if (err != KErrCompletion)
		    {
		    User::Leave(err);
		    }
		}

	state = EFrameIncomplete;

	if(iIsBlockStreaming)
		{
		if (iPosProcessor.IsEndOfImage())
			{
			state = EBlockComplete;
			}		
		}
	else
		{
		if (iPosProcessor.IsEndOfImage())
			{
			CompleteFrame();
			state = EFrameComplete;
			}
		}
	
    aDst.SetLength(iDestPtr - destStartPtr);		
	return state;
	}

void CJpgWriteCodec::SetCompleteFrame()
	{
	iStreamConfig.iIsFrameComplete = ETrue;
	}
	
void CJpgWriteCodec::CompleteFrame()
	{
	
	if (iBitsUsed >= 8)
	    {
	    do 
			{
			TUint8 byte = (TUint8)(iBitBuffer >> 24);
			iBitBuffer <<= 8;
        
			*iDestPtr++ = byte;
			if (byte == 0xff)
				{
				*iDestPtr++ = 0;
				}
        
			iBitsUsed   -= 8;
			} while (iBitsUsed >= 8);
	    }

	if (iBitsUsed > 0)
	    {
	    iDestPtr[0] = TUint8( iBitBuffer >> 24 );
	    ++iDestPtr;
	    }
	
	PROFILE_MARK(iOverallTime);
	PROFILE_REPORT(iGetPixelsTime);
	PROFILE_REPORT(iImageFrameTime);
	PROFILE_REPORT(iDataUnitProcessTime);
	PROFILE_REPORT(iTransforTime);
	PROFILE_REPORT(iQuantizeTime);
	PROFILE_REPORT(iWriteDataUnitTime);
	PROFILE_REPORT(iOverallTime);
	}

#if defined(__ARMCC__)
#pragma push
#pragma thumb
#endif

//Bitmap encoding	
void CJpgWriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource)
	{
	PROFILE_MARK(iOverallTime);

	TUint8* destStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = destStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	SetSource(&aSource);

	iSourceRect = TRect(aSource.SizeInPixels());
	iPos.SetXY(0,0);

	iFrameInfo.iSizeInPixels = iSourceRect.Size();
	iMCUSizeInPixels.SetSize(iFrameInfo.iMaxHorzSampleFactor,iFrameInfo.iMaxVertSampleFactor);
	iMCUSizeInPixels.iWidth     *= KJpgDCTBlockWidth;
	iMCUSizeInPixels.iHeight    *= KJpgDCTBlockWidth;
	
	delete iRgbInputBuffer; 
	iRgbInputBuffer = NULL;
	iRgbInputBuffer = CRgbBufferPtr::NewL(const_cast<CFbsBitmap&>(aSource), iMCUSizeInPixels);
	
    if (iFrameInfo.iNumberOfComponents == 1)
        {
        iDataUnitCount = 1;
        }
	else
		{
		iDataUnitCount = 0;
		for (TInt index = 0; index < iFrameInfo.iNumberOfComponents; index++)
			{
			TJpgFrameInfo::TComponentInfo& component = iFrameInfo.iComponent[index];
			iDataUnitCount += component.iHorzSampleFactor * component.iVertSampleFactor;
			}
		}
		
    ASSERT( iDataUnitCount <= KJpgMaxNumOfDataUnits );

	switch (iDataUnitCount)
		{
	case KJpgMonochromeDataUnitCount: // Monochrome
		iDataUnitProcessor = new(ELeave) TMonoProcessor;
		break;
	case KJpgEColor444DataUnitCount: // 4:4:4
		iDataUnitProcessor = new(ELeave) T444Processor;
		break;
	case KJpgColor422DataUnitCount: // 4:2:2
		iDataUnitProcessor = new(ELeave) T422Processor;
		break;
	case KJpgColor420DataUnitCount: // 4:2:0
		iDataUnitProcessor = new(ELeave) T420Processor;
		break;
	default:
		User::Leave(KErrNotSupported);
		}
		
    iDataUnitProcessor->SetBufferPtrUtil( *iRgbInputBuffer );
    
    InitTransformationL(iPosProcessor, iSourceRect, iMCUSizeInPixels, iFrameInfo, iDataUnitCount);
       
    CodecInfoL();
    InitCompConfig();
    
	aDst.SetLength(iDestPtr - destStartPtr);
	}

//ImageFrame encoding
void CJpgWriteCodec::InitFrameL(TBufPtr8& aDst, const CImageFrame* aFrame, const CFrameImageData* aFrameImageData)
	{
	TUint8* destStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = destStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	iSourceRect = TRect(aFrame->FrameSizeInPixels());
	iPos.SetXY(0,0);

	iFrameInfo.iSizeInPixels = iSourceRect.Size();
	
	// Create JPEG write codec extension and the appropriate image processor
	ASSERT( iImageFrameCodecPtr == NULL );
	delete iImageFrameCodecPtr;
	iImageFrameCodecPtr = NULL;
	iImageFrameCodecPtr = CJpgImageFrameWriteCodec::NewL(aFrame);
	iImageFrameCodecPtr->PrepareFrameInfoL(iFrameInfo, aFrameImageData);
	iImageFrameCodecPtr->CreateImageProcessorL();

	iDataUnitCount = iImageFrameCodecPtr->DataUnitCount();
	
	iMCUSizeInPixels.SetSize(iFrameInfo.iMaxHorzSampleFactor, iFrameInfo.iMaxVertSampleFactor);
	iMCUSizeInPixels.iWidth     *= KJpgDCTBlockWidth;
	iMCUSizeInPixels.iHeight    *= KJpgDCTBlockWidth;
	
	InitTransformationL(iPosProcessor, iSourceRect, iMCUSizeInPixels, iFrameInfo, iDataUnitCount);
	
	CodecInfoL();
	InitCompConfig();
	
	aDst.SetLength(iDestPtr - destStartPtr);	
	}

//ImageFrame stream encoding
void CJpgWriteCodec::InitFrameL(TBufPtr8& aDst, TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, const CFrameImageData* aFrameImageData)
	{
	TUint8* destStartPtr = const_cast<TUint8*>(aDst.Ptr());
	iDestPtr = destStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	iSourceRect = TRect(aFrameSizeInPixels);
	iPos.SetXY(0,0);

	iFrameInfo.iSizeInPixels = iSourceRect.Size();
	
	// Create JPEG write codec extension and the appropriate image processor
	ASSERT( iImageFrameCodecPtr == NULL );
	delete iImageFrameCodecPtr;
	iImageFrameCodecPtr = NULL;
	iImageFrameCodecPtr = CJpgImageFrameWriteCodec::NewL(NULL);
	iImageFrameCodecPtr->PrepareFrameImageDataInfoL(iFrameInfo, aFormat, aFrameNumber, aFrameImageData);
	
	iDataUnitCount = iImageFrameCodecPtr->DataUnitCount();

	iMCUSizeInPixels.SetSize(iFrameInfo.iMaxHorzSampleFactor, iFrameInfo.iMaxVertSampleFactor);
	iMCUSizeInPixels.iWidth     *= KJpgDCTBlockWidth;
	iMCUSizeInPixels.iHeight    *= KJpgDCTBlockWidth;
		
	InitTransformationL(iPosProcessor, TRect(aBlockSizeInPixels), iMCUSizeInPixels, iFrameInfo, iDataUnitCount);
	
	iIsBlockStreaming = ETrue;
	iStreamConfig.iIsFrameComplete = EFalse;
	
	CodecInfoL();
	InitCompConfig();
	
	aDst.SetLength(iDestPtr - destStartPtr);	
	}

void CJpgWriteCodec::AppendFrameBlockL(const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{	
	ValidateBlockSizeL(aBlocks.FrameSizeInPixels(), iMCUSizeInPixels);
	
	if (aBlocks.FrameSizeInPixels().iHeight != iMCUSizeInPixels.iHeight)
		{
		User::Leave(KErrNotSupported);
		}
	
	iStreamConfig.iNumOfBlocksToAppend = aNumBlocksToAdd;
	iStreamConfig.iNumOfBlocksAppended = 0;
	
	iImageFrameCodecPtr->AppendImageFrameBlockL(&aBlocks);
	
	InitTransformationL(iPosProcessor, TRect(aBlocks.FrameSizeInPixels()), iMCUSizeInPixels, iFrameInfo, iDataUnitCount);
	}

//validates the stream blocks passed and checks that the height is equal to the mcu height.
void CJpgWriteCodec::ValidateBlockSizeL(const TSize& aBlockSizeInPixels, const TSize& aRefSizeInPixels)
	{
	TInt oddPixelsWidth = aBlockSizeInPixels.iWidth % aRefSizeInPixels.iWidth;
	TInt oddPixelsHeight = aBlockSizeInPixels.iHeight % aRefSizeInPixels.iHeight;	
	
	if (oddPixelsWidth != 0 || oddPixelsHeight != 0)
		{
		User::Leave(KErrNotSupported);
		}
	}

void CJpgWriteCodec::InitTransformationL(TPositionProcessor& aPosProcessor, const TRect& aSourceRect, const TSize& aMCUSize, TJpgFrameInfo& aFrameInfo, const TInt aDataUnitCount)
	{
    InitTransformCoordinates(aPosProcessor, aSourceRect, aMCUSize, iOperationsRequested);
    
    InitTransformDataUnitIndex(aDataUnitCount);
    
	if(aPosProcessor.IsTransformNeeded())
		{
		ValidateBlockSizeL(aFrameInfo.iSizeInPixels, aMCUSize);
		
		if(aPosProcessor.SwapDimensions())
		    {
		    TInt tempDimention;
			tempDimention = aFrameInfo.iSizeInPixels.iHeight; 
			aFrameInfo.iSizeInPixels.iHeight = aFrameInfo.iSizeInPixels.iWidth; // Y
			aFrameInfo.iSizeInPixels.iWidth = tempDimention; // X
			
			if(aDataUnitCount == KJpgColor422DataUnitCount)
				{
				tempDimention = aFrameInfo.iComponent[0].iHorzSampleFactor;
				aFrameInfo.iComponent[0].iHorzSampleFactor = aFrameInfo.iComponent[0].iVertSampleFactor;
				aFrameInfo.iComponent[0].iVertSampleFactor = tempDimention;			
				}				
		    }		
		}
	
	InitTransformDataUnitCoeff(iTransformDUCoeffIdx);	
	}


void CJpgWriteCodec::InitCompConfig()
    {
	iCompConfig[ KYComp ].iFirstDUIdx = 0;
	
	switch (iDataUnitCount)
		{
	case KJpgMonochromeDataUnitCount: // Monochrome
		// in fact we don't have these comps for mono, so assign them last index
		iCompConfig[ KUComp ].iFirstDUIdx = KJpgMaxNumOfDataUnits - 1;
		iCompConfig[ KVComp ].iFirstDUIdx = KJpgMaxNumOfDataUnits - 1;		
		break;
	case KJpgEColor444DataUnitCount: // 4:4:4
		iCompConfig[ KUComp ].iFirstDUIdx = 1;
		iCompConfig[ KVComp ].iFirstDUIdx = 2;
		break;
	case KJpgColor422DataUnitCount: // 4:2:2
		iCompConfig[ KUComp ].iFirstDUIdx = 2;
		iCompConfig[ KVComp ].iFirstDUIdx = 3;		
		break;
	case KJpgColor420DataUnitCount: // 4:2:0
		iCompConfig[ KUComp ].iFirstDUIdx = 4;
		iCompConfig[ KVComp ].iFirstDUIdx = 5;				
		break;
	default:
		ASSERT( EFalse ); // shan't happen
		}
    
	iCompConfig[ KYComp ].iDCPredictorIdx = KYComp;
	iCompConfig[ KUComp ].iDCPredictorIdx = KUComp;
	iCompConfig[ KVComp ].iDCPredictorIdx = KVComp;
	
    TJpgFrameInfo::TComponentInfo& yComp    = iFrameInfo.iComponent[ KYComp ];
	iCompConfig[ KYComp ].iDataUnitsCount     = yComp.iHorzSampleFactor * yComp.iVertSampleFactor;

	if (iCompConfig[ KYComp ].iDataUnitsCount == 3)
		{
		TJpgFrameInfo::TComponentInfo& uComp = iFrameInfo.iComponent[ KUComp ];
		iCompConfig[ KUComp ].iDataUnitsCount = uComp.iHorzSampleFactor * uComp.iVertSampleFactor;
		
		TJpgFrameInfo::TComponentInfo& vComp = iFrameInfo.iComponent[ KVComp ];
		iCompConfig[ KVComp ].iDataUnitsCount = vComp.iHorzSampleFactor * vComp.iVertSampleFactor;
		}
	else
	    {
	    iCompConfig[ KVComp ].iDataUnitsCount = iCompConfig[ KUComp ].iDataUnitsCount = 0;
	    }
	    
	iCompConfig[ KYComp ].iACTable = &iLumaACHuffmanTable;
	iCompConfig[ KYComp ].iDCTable = &iLumaDCHuffmanTable;
	
	// we use the same tables for U and V comps
	iCompConfig[ KUComp ].iACTable = &iChromaACHuffmanTable;
	iCompConfig[ KUComp ].iDCTable = &iChromaDCHuffmanTable;

	iCompConfig[ KVComp ].iACTable = &iChromaACHuffmanTable;
	iCompConfig[ KVComp ].iDCTable = &iChromaDCHuffmanTable;
	
	iCompConfig[ KYComp ].iQTTable = &iLumaQTable;
	// we use the same tables for U and V comps
	iCompConfig[ KUComp ].iQTTable = &iChromaQTable;
	iCompConfig[ KVComp ].iQTTable = &iChromaQTable;
	
	iBitsUsed = 0;
    iBitBuffer= 0;    
    }

void CJpgWriteCodec::CodecInfoL()
{
	iDCT.SetPrecision(8);

	if (iReplacementLumaQTable)
		iLumaQTable = *iReplacementLumaQTable;
	else
		iLumaQTable.Set(KLuminanceQTableValues,EFalse);
	iLumaQTable.SetQualityFactor(iQualityFactor);

	const TUint8* dataPtr = KLuminanceDCHuffmanValues;
	const TUint8* dataPtrLimit = dataPtr + sizeof(KLuminanceDCHuffmanValues);
	iLumaDCHuffmanTable.SetL(dataPtr,dataPtrLimit);
	iLumaDCHuffmanTable.MakeDerivedTableL();

	dataPtr = KLuminanceACHuffmanValues;
	dataPtrLimit = dataPtr + sizeof(KLuminanceACHuffmanValues);
	iLumaACHuffmanTable.SetL(dataPtr,dataPtrLimit);
	iLumaACHuffmanTable.MakeDerivedTableL();

	if (iFrameInfo.iNumberOfComponents == 3)
		{
		if (iReplacementChromaQTable)
			iChromaQTable = *iReplacementChromaQTable;
		else
			iChromaQTable.Set(KChrominanceQTableValues,EFalse);
		iChromaQTable.SetQualityFactor(iQualityFactor);

		dataPtr = KChrominanceDCHuffmanValues;
		dataPtrLimit = dataPtr + sizeof(KChrominanceDCHuffmanValues);
		iChromaDCHuffmanTable.SetL(dataPtr,dataPtrLimit);
		iChromaDCHuffmanTable.MakeDerivedTableL();

		dataPtr = KChrominanceACHuffmanValues;
		dataPtrLimit = dataPtr + sizeof(KChrominanceACHuffmanValues);
		iChromaACHuffmanTable.SetL(dataPtr,dataPtrLimit);
		iChromaACHuffmanTable.MakeDerivedTableL();
		}

	iDCPredictor[0] = 0;
	iDCPredictor[1] = 0;
	iDCPredictor[2] = 0;

	WriteInfoL();
}

void CJpgWriteCodec::WriteInfoL()
	{
	for (TInt index = 0; index < iComment.Count(); index++)
		{
		if (!iComment[index])
			continue;

		const TInt commentLength = iComment[index]->Length();
		WriteBigEndianUint16(iDestPtr,KJpgCommentSignature);
		WriteBigEndianUint16(iDestPtr,commentLength + 2);
		Mem::Copy(iDestPtr,iComment[index]->Ptr(),commentLength);
		iDestPtr += commentLength;
		}


	if (iFrameInfo.iNumberOfComponents == 1)
		{
		WriteBigEndianUint16(iDestPtr,KJpgDQTSignature); // Luminance quantization table
		WriteBigEndianUint16(iDestPtr,3 + KJpgQTableEntries); // Lq
		*iDestPtr++ = 0; // (Pq << 4) | Tq
		iDestPtr += iLumaQTable.Get(iDestPtr); // Qk
		}
	else if (iFrameInfo.iNumberOfComponents == 3)
		{
		WriteBigEndianUint16(iDestPtr,KJpgDQTSignature); // Luminance and Chrominance quantization tables
		WriteBigEndianUint16(iDestPtr,2 + ((KJpgQTableEntries + 1) * 2)); // Lq

		*iDestPtr++ = 0; // (Pq << 4) | Tq
		iDestPtr += iLumaQTable.Get(iDestPtr); // Qk

		*iDestPtr++ = 1; // (Pq << 4) | Tq
		iDestPtr += iChromaQTable.Get(iDestPtr); // Qk
		}
	else
		User::Leave(KErrNotSupported);

	WriteBigEndianUint16(iDestPtr,KJpgBaselineDCTSOFSignature); // Frame header
	WriteBigEndianUint16(iDestPtr,8 + (3 * iFrameInfo.iNumberOfComponents)); // Lf
	*iDestPtr++ = 8; // P
	WriteBigEndianUint16(iDestPtr,iFrameInfo.iSizeInPixels.iHeight); // Y
	WriteBigEndianUint16(iDestPtr,iFrameInfo.iSizeInPixels.iWidth); // X
	*iDestPtr++ = TUint8(iFrameInfo.iNumberOfComponents); // Nf

	if (iFrameInfo.iNumberOfComponents == 3)
		{
		for (TInt i = 0; i < iFrameInfo.iNumberOfComponents; i++)
			{
			*iDestPtr++ = TUint8(i + 1); // Ci
			*iDestPtr++ = TUint8((iFrameInfo.iComponent[i].iHorzSampleFactor << 4) | iFrameInfo.iComponent[i].iVertSampleFactor);
			*iDestPtr++ = TUint8((i == 0) ? 0 : 1); // Tqi
			}
		}
	else
		{
		*iDestPtr++ = 1; // Ci
		*iDestPtr++ = 0x11; // (Hi << 4) | Vi
		*iDestPtr++ = 0; // Tqi
		}

	if (iFrameInfo.iNumberOfComponents == 1)
		{
		WriteBigEndianUint16(iDestPtr,KJpgDHTSignature); // Huffman table
 
 		TUint8* LhPtr = iDestPtr; // Remember Lh location
 		WriteBigEndianUint16(iDestPtr,0); // Dummy Lh value
 
 		*iDestPtr++ = 0x00; // (Tc << 4) | Th
 		const TInt lumaDCTableSize = iLumaDCHuffmanTable.GetL(iDestPtr);
 		iDestPtr += lumaDCTableSize;

		*iDestPtr++ = 0x10; // (Tc << 4) | Th
 		const TInt lumaACTableSize = iLumaACHuffmanTable.GetL(iDestPtr);
 		iDestPtr += lumaACTableSize;

		WriteBigEndianUint16(LhPtr,iDestPtr - LhPtr); // Real Lh value
		}
	else if (iFrameInfo.iNumberOfComponents == 3)
		{
 		WriteBigEndianUint16(iDestPtr,KJpgDHTSignature); // Huffman tables
 
 		TUint8* LhPtr = iDestPtr; // Remember Lh location
 		WriteBigEndianUint16(iDestPtr,0); // Dummy Lh value
 
 		*iDestPtr++ = 0x00; // (Tc << 4) | Th
 		const TInt lumaDCTableSize = iLumaDCHuffmanTable.GetL(iDestPtr);
		iDestPtr += lumaDCTableSize;
 
 		*iDestPtr++ = 0x10; // (Tc << 4) | Th
 		const TInt lumaACTableSize = iLumaACHuffmanTable.GetL(iDestPtr);
 		iDestPtr += lumaACTableSize;
 
   		*iDestPtr++ = 0x01; // (Tc << 4) | Th
 		const TInt chromaDCTableSize = iChromaDCHuffmanTable.GetL(iDestPtr);
 		iDestPtr += chromaDCTableSize;

   		*iDestPtr++ = 0x11; // (Tc << 4) | Th
 		const TInt chromaACTableSize = iChromaACHuffmanTable.GetL(iDestPtr);
 		iDestPtr += chromaACTableSize;
 
 		WriteBigEndianUint16(LhPtr,iDestPtr - LhPtr); // Real Lh value
		}
	else
		{
 		User::Leave(KErrNotSupported);
		}

	WriteBigEndianUint16(iDestPtr,KJpgSOSSignature); // Scan header
	WriteBigEndianUint16(iDestPtr,6 + (2 * iFrameInfo.iNumberOfComponents)); // Ls
	*iDestPtr++ = TUint8(iFrameInfo.iNumberOfComponents); // Ns
	for (TInt j = 0; j < iFrameInfo.iNumberOfComponents; j++)
		{
		*iDestPtr++ = TUint8(j + 1); // Csj
		*iDestPtr++ = TUint8((j == 0) ? 0x00 : 0x11); // (Tdj << 4) | Taj
		}
	*iDestPtr++ = 0; // Ss
	*iDestPtr++ = 63; // Se
	*iDestPtr++ = 0; // (Ah << 4) | Al

	*iDestPtr = 0; // Initialize first byte of scan data buffer
	}

#if defined(__ARMCC__)
#pragma pop
#endif

void CJpgWriteCodec::DoProcessL()
	{
	ASSERT ( iDataUnitCount > 0 );
	// only process a maximum of KMaxRgbBufferLines 
	// y position is a multiple of iMCUSizeInPixels.iHeight due to the matrix
	// way in which pixels are gathered [see GetPixels()].
	// KMaxRgbBufferLines * iMCUSizeInPixels.iHeight = no of scanlines
	
                                    // Maximum number of matrix lines to process in one process call
                                    // we have no limit for CImageFrame since it doesn't lock
                                    // FBs heap                                    
    
	TComponentConfig* compCfg[KJpgMaxNumOfDataUnits];//no more then 10 data units is supported now
	for (TInt dataUnit = 0; dataUnit < iDataUnitCount; ++dataUnit)
                {
                compCfg[dataUnit] = (dataUnit < iCompConfig[ KUComp ].iFirstDUIdx ? iCompConfig + KYComp:
                                               (dataUnit < iCompConfig[ KVComp ].iFirstDUIdx ?  iCompConfig + KUComp:
                                                               iCompConfig + KVComp)
                                            );
				}
	
	
	do
		{		
		if ( iWrittingDUIdx >= iDataUnitCount) // we have no blocks to write, so start from processing phase
		    {
		    
    		TDataUnit* dataUnitPtr = NULL;
    		
    		iPosProcessor.GetCurrentPosition(iPos);
    		
    		if 	(iImageFrameCodecPtr==NULL)
    			{
    			PROFILE_START(iGetPixelsTime);
    			// get destination scanline
    			CRgbBufferPtr::TConstRgbBufferPtr bufPtr = GetPixels(iPos);
    			PROFILE_END(iGetPixelsTime);
    			
    			PROFILE_START(iDataUnitProcessTime);
     			dataUnitPtr = iDataUnitProcessor->Process( bufPtr );
     			iRgbInputBuffer->UnlockBuffer();
     			PROFILE_END(iDataUnitProcessTime);
    			}
    		else
    			{
    			PROFILE_START(iImageFrameTime);
    			TPoint imageFramePos;
    			//set position before processing Image Frame Codec
    			if(iImageFrameCodecPtr->SamplingScheme() == KUidSamplingColor422)
	    			{
	    			imageFramePos = iPos;
	    			//the code below makes it possible to navigate the image in the multiples of 2 blocks. To be removed if the image frame write codec navigate in the multiple of a block.
	    			imageFramePos.iX = imageFramePos.iX * 2;
	    			}
	    		else
		    		{
	    			imageFramePos = iPos;		    			
		    		}
	    		iImageFrameCodecPtr->SetPosition(imageFramePos);
    			dataUnitPtr = iImageFrameCodecPtr->ProcessL();  			    			
    			PROFILE_END(iImageFrameTime);
    			if(iIsBlockStreaming)    			
	    			{
		    		iStreamConfig.iNumOfBlocksAppended++;
		    		iStreamConfig.iNumOfMCUsProcessed++;	    			
	    			if(iStreamConfig.iNumOfBlocksAppended >= iStreamConfig.iNumOfBlocksToAppend)
		    			{
		    			iPosProcessor.SetEndOfImage(ETrue);
		    			}
					
					if(iStreamConfig.iNumOfMCUsProcessed > ((iFrameInfo.iSizeInPixels.iWidth * iFrameInfo.iSizeInPixels.iHeight) / (iMCUSizeInPixels.iWidth * iMCUSizeInPixels.iHeight)))
						{
						User::Leave(KErrOverflow);
						}
	    			}    				    			
    			}
            
            iWrittingDUIdx = 0;
            TDataUnit transformedDataUnitCoeff;
    		TDataUnit transformedDataUnit;
    		// here we create a special "data unit" and place a non-zero value at the end,
    		// as that is required by the WriteDataUnitL()
            TSpecialDataUnit* quantTarget = iQuantizeTarget;
			
            for (TInt dataUnit = 0; dataUnit < iDataUnitCount; ++dataUnit)
                {
                PROFILE_START(iTransforTime);
				if(iPosProcessor.IsTransformNeeded())
					{
					//Xform Coefficients in DU
	    			TransformDataUnitCoeff(transformedDataUnitCoeff, dataUnitPtr[iTransformedDUIdx[dataUnit]]);
					
					//Xform DCT
	    			iDCT.Transform(transformedDataUnit, transformedDataUnitCoeff, iHighSpeedMode);
					}
				else
					{
					//Xform DCT
	    			iDCT.Transform(transformedDataUnit, dataUnitPtr[iTransformedDUIdx[dataUnit]], iHighSpeedMode);
					}
				PROFILE_END(iTransforTime);    			
    			
    			//Quantize
    			PROFILE_START(iQuantizeTime);
    			compCfg[dataUnit]->iQTTable->Quantize(*quantTarget, transformedDataUnit, iHighSpeedMode);
    			quantTarget->iExtraElement = KErrNotFound; // place a non-zero value at the end
    			quantTarget++;
    			PROFILE_END(iQuantizeTime);			
                }
		    }

		TSpecialDataUnit* quantTarget = iQuantizeTarget;
		
		PROFILE_START(iWriteDataUnitTime);
		/*  Coverity may report about overrun of array compCfg. This is false positive.  The value of iDataUnitCount is <= KJpgMaxNumOfDataUnits, 
		so the array access is safe. */
		for (; iWrittingDUIdx < iDataUnitCount; ++iWrittingDUIdx) 
		    {
			WriteDataUnitL( *(quantTarget + iWrittingDUIdx), *(compCfg[iWrittingDUIdx]->iDCTable), *(compCfg[iWrittingDUIdx]->iACTable), 
			                                                   iDCPredictor[ compCfg[iWrittingDUIdx]->iDCPredictorIdx ]
                            );
		    }
        PROFILE_END(iWriteDataUnitTime);		    
        iWrittingDUIdx = KJpgMaxNumOfDataUnits;
        
        iPosProcessor.MoveNext();
		
		} while(!iPosProcessor.IsEndOfImage());

	}
   
// CJpgImageFrameWriteCodec
CJpgImageFrameWriteCodec::CJpgImageFrameWriteCodec(const CImageFrame* aFrame)
	{
	iSource = aFrame;
	}

CJpgImageFrameWriteCodec::~CJpgImageFrameWriteCodec()
	{
	delete iImageFrameProcessorPtr; 
	}
	
CJpgImageFrameWriteCodec* CJpgImageFrameWriteCodec::NewL(const CImageFrame* aFrame)
	{
	CJpgImageFrameWriteCodec* self = new(ELeave) CJpgImageFrameWriteCodec(aFrame);
	return self;
	}

void CJpgImageFrameWriteCodec::CreateImageProcessorL()
	{	
	iImageFrameProcessorPtr = CJpgImageFrameProcessorUtility::NewL(*(const_cast<CImageFrame*>(iSource)));	
	}

TInt CJpgImageFrameWriteCodec::DataUnitCount() const
    {
    ASSERT( iDataUnitCount > 0 );
    return iDataUnitCount;
    }

TSize  CJpgImageFrameWriteCodec::ImageFrameSize() const
	{
	return iSource->FrameSizeInPixels();
	}
	
TUid CJpgImageFrameWriteCodec::SamplingScheme() const
	{
	return iSampleScheme;		
	}	

void CJpgImageFrameWriteCodec::PrepareFrameInfoL(TJpgFrameInfo& aFrameInfo, const CFrameImageData* aFrameImageData)
	{
	// First, validate image frame
	CImageFrame* aImageFrame = const_cast<CImageFrame*>(iSource);
	CJpgImageFrameProcessorUtility::ValidateImageFrameL(*aImageFrame, ETrue);

	// If CFrameImageData has been provided aFrameInfo contains some encoder
	// parameters already. These need to be the same as the imageFrame parameters.
	// Therefore, validate the image frame parameters against the frame image
	// data provided by the application if any
	TUid aSampling = static_cast<const TFrameFormat&>(iSource->FrameFormat()).Sampling();

	TInt count = (aFrameImageData) ? aFrameImageData->ImageDataCount() : 0;

	TBool samplingSchemeSet = EFalse;
	// look for Frame Image Data
	for (TInt index = 0 ; index<count ; index++)
		{	
		const TImageDataBlock& encoderData = *aFrameImageData->GetImageData(index);
		if (encoderData.DataType() == KJPGImageDataUid)
			{
			const TJpegImageData& jpegImageData = static_cast<const TJpegImageData&>(encoderData);

			TJpegImageData::TColorSampling sampleScheme = jpegImageData.iSampleScheme;
			CJpgImageFrameProcessorUtility::ValidateFrameImageDataL(sampleScheme, *aImageFrame);
			
        	switch (sampleScheme)
        		{
        		case TJpegImageData::EMonochrome:
        			iDataUnitCount = 1;
        			break;
        		case TJpegImageData::EColor420:
        		    iDataUnitCount = 6;
        			break;
        		case TJpegImageData::EColor422:
        		    iDataUnitCount = 4;
        			break;				
        		case TJpegImageData::EColor444:
        		    iDataUnitCount = 3;
        			break;					
        		default:
        		    iDataUnitCount = 0;
        			User::Leave(KErrNotSupported);	
        			break;	
        		};
        		
			samplingSchemeSet = ETrue;
			break;
			}
		}
		
	if (!samplingSchemeSet)
		{
		iSampleScheme = aSampling;
		InitFrameCompInfoL(aFrameInfo);
		}
	}

void CJpgImageFrameWriteCodec::Position(TPoint& aPos)
	{
	iImageFrameProcessorPtr->GetCurrentPosition(aPos);	
	}

void CJpgImageFrameWriteCodec::SetPosition(TPoint& aPos)
	{
	iImageFrameProcessorPtr->SetCurrentPosition(aPos);
	}

TDataUnit* CJpgImageFrameWriteCodec::ProcessL()
	{
	return (iImageFrameProcessorPtr->ReadBlockL());	
	}


// for streaming
void CJpgImageFrameWriteCodec::AppendImageFrameBlockL(const CImageFrame* aFrame)
	{
	if(aFrame == NULL)
		{
		User::Leave(KErrNotSupported);
		}
	
	iSource = aFrame;
	
	if(iImageFrameProcessorPtr)
		{
		delete iImageFrameProcessorPtr;
		iImageFrameProcessorPtr = NULL;
		}
	
	iImageFrameProcessorPtr = CJpgImageFrameProcessorUtility::NewL(*(const_cast<CImageFrame*>(iSource)));
	
	// First, validate image frame	
	CImageFrame* aImageFrame = const_cast<CImageFrame*>(iSource);
	CJpgImageFrameProcessorUtility::ValidateImageFrameL(*aImageFrame, ETrue);
	CJpgImageFrameProcessorUtility::ValidateFrameImageDataL(GetColorSamplingL(iSampleScheme), *aImageFrame);
	}

TJpegImageData::TColorSampling CJpgImageFrameWriteCodec::GetColorSamplingL(TUid aFormat)
	{
	TJpegImageData::TColorSampling aColorSampling = TJpegImageData::EColor420;
	switch (aFormat.iUid)
		{
		case KSamplingMonochromeUidValue:
			aColorSampling = TJpegImageData::EMonochrome;
			break;			
		case KSamplingColor422UidValue:
			aColorSampling = TJpegImageData::EColor422;
			break;
		case KSamplingColor420UidValue:
			aColorSampling = TJpegImageData::EColor420;
			break;
		default:			
			User::Leave(KErrNotSupported);
		}
	return aColorSampling;
	}

TUid CJpgImageFrameWriteCodec::GetSamplingSchemeL(TUid aFormat)
	{
	TUid samplingScheme = KUidSamplingColor420;
	switch (aFormat.iUid)
		{
		case KFormatYUVMonochromeUidValue:
			samplingScheme = KUidSamplingMonochrome;
			break;			
		case KFormatYUV422InterleavedUidValue:
			samplingScheme = KUidSamplingColor422;
			break;
		case KFormatYUV420PlanarReversedUidValue:
		case KFormatYUV420PlanarUidValue:
			samplingScheme = KUidSamplingColor420;
			break;
		default:			
			User::Leave(KErrNotSupported);			
		}
	return samplingScheme;
	}

void CJpgImageFrameWriteCodec::PrepareFrameImageDataInfoL(TJpgFrameInfo& aFrameInfo, TUid aFormat, TInt /*aFrameNumber*/, const CFrameImageData* /*aFrameImageData*/)
	{
	iSampleScheme = GetSamplingSchemeL(aFormat);
	InitFrameCompInfoL(aFrameInfo);
	}

void CJpgImageFrameWriteCodec::InitFrameCompInfoL(TJpgFrameInfo& aFrameInfo)
	{
	aFrameInfo.iComponent[0].iQTable = 0;
	if (iSampleScheme == KUidSamplingMonochrome )
		{
		aFrameInfo.iNumberOfComponents = 1;
		aFrameInfo.iComponent[0].iHorzSampleFactor = 1;
		aFrameInfo.iComponent[0].iVertSampleFactor = 1;
		iDataUnitCount = 1;
		}
	else
		{
		aFrameInfo.iNumberOfComponents = 3;
		switch (iSampleScheme.iUid)
			{
			case KSamplingColor420UidValue:
				aFrameInfo.iComponent[0].iHorzSampleFactor = 2;
				aFrameInfo.iComponent[0].iVertSampleFactor = 2;
				aFrameInfo.iComponent[1].iVertSampleFactor = 1;
				aFrameInfo.iComponent[2].iVertSampleFactor = 1;
				iDataUnitCount = 6;
				break;
			case KSamplingColor422UidValue:
				aFrameInfo.iComponent[0].iHorzSampleFactor = 2;
				aFrameInfo.iComponent[0].iVertSampleFactor = 1;
				aFrameInfo.iComponent[1].iVertSampleFactor = 1;
				aFrameInfo.iComponent[2].iVertSampleFactor = 1;
				iDataUnitCount = 4;
				break;
			case KSamplingColor444UidValue:
				aFrameInfo.iComponent[0].iHorzSampleFactor = 1;
				aFrameInfo.iComponent[0].iVertSampleFactor = 1;
				aFrameInfo.iComponent[1].iVertSampleFactor = 1;
				aFrameInfo.iComponent[2].iVertSampleFactor = 1;
				iDataUnitCount = 3;
				break;
			default:
			    iDataUnitCount = 0;
				User::Leave(KErrNotSupported);
			}

		aFrameInfo.iComponent[1].iHorzSampleFactor = 1;
		aFrameInfo.iComponent[1].iQTable = 1;
		aFrameInfo.iComponent[2].iHorzSampleFactor = 1;
		aFrameInfo.iComponent[2].iQTable = 1;
		}
		
	aFrameInfo.iMaxHorzSampleFactor = aFrameInfo.iComponent[0].iHorzSampleFactor;
	aFrameInfo.iMaxVertSampleFactor = aFrameInfo.iComponent[0].iVertSampleFactor;		
	}

void CJpgWriteCodec::InitTransformCoordinates(TPositionProcessor& aPosProcessor, const TRect& aSourceRect, const TSize& aMCUSize, const RArray<TUint>& aOperationsRequested)
	{	
    
    aPosProcessor.SetDimensions(aSourceRect,aMCUSize);
    
	TInt count = aOperationsRequested.Count();
	for (TInt i=0; i<count; i++)
		{
		switch(aOperationsRequested[i])
			{
			case KRotation90DegreesClockwise:
				{
				aPosProcessor.RotateCoordinates();
				break;						
				}
			case KRotation180DegreesClockwise:
				{
				aPosProcessor.RotateCoordinates();
				aPosProcessor.RotateCoordinates();
				break;						
				}
			case KRotation270DegreesClockwise:
				{
				aPosProcessor.RotateCoordinates();
				aPosProcessor.RotateCoordinates();
				aPosProcessor.RotateCoordinates();
				break;						
				}
			case KMirrorHorizontalAxis:
				{
				aPosProcessor.HorFlipCoordinates();
				break;						
				}
			case KMirrorVerticalAxis:
				{
				aPosProcessor.VerFlipCoordinates();
				break;						
				}
			default:
				ASSERT( EFalse );
			}
		}
	     
    aPosProcessor.MoveFirst();
		 
	}
	
void CJpgWriteCodec::TransformDataUnitCoeff(TDataUnit& aDestination,const TDataUnit& aSource)
	{	
	const TDataUnit::TDataUnitElemType* srcPtr = aSource.iCoeff;
	TDataUnit::TDataUnitElemType* dstPtr = aDestination.iCoeff;	
	TUint8* idxCoeff = iTransformDUCoeffIdx;
	
	TInt count = 0;
	do
		{		
	    TUint32 fourIndeces = *(TUint32 *)(idxCoeff+count);
	    
	    dstPtr[count++] = srcPtr[fourIndeces & 0xFF];
	    fourIndeces >>= 8;
	    
	    dstPtr[count++] = srcPtr[fourIndeces & 0xFF];
	    fourIndeces >>= 8;
	    
	    dstPtr[count++] = srcPtr[fourIndeces & 0xFF];
	    fourIndeces >>= 8;
	    
	    dstPtr[count++] = srcPtr[fourIndeces];						
		} while (count < KJpgDCTBlockSize);

	/*do
		{		
		dstPtr[count++] = srcPtr[*idxCoeff++];		
		} while (count < KJpgDCTBlockSize);*/

	}

void CJpgWriteCodec::InitTransformDataUnitCoeff(TUint8* aDestination) //TODO
	{
	TPositionProcessor posProcess;
	TPoint pos;	
		
	TUint8* dstPtr = aDestination;
	
	InitTransformCoordinates(posProcess, TRect(TPoint(0,0),TPoint(KJpgDCTBlockWidth, KJpgDCTBlockHeight)), TSize(KJpgPixelRatio, KJpgPixelRatio), iOperationsRequested);
	
	do
		{		
		posProcess.GetCurrentPosition(pos);
		TUint8 elementPos = ((pos.iY * KJpgDCTBlockWidth) + pos.iX);		
		*dstPtr++ = elementPos;
		posProcess.MoveNext();		
		} while (!posProcess.IsEndOfImage());		
	}

void CJpgWriteCodec::InitTransformDataUnitIndex(TInt aDataUnitCount)
	{
	
	TUint8 tempTransformedDUIdx[KJpgMaxNumOfDataUnits];
	TPositionProcessor posProcess;
	TPoint pos;	
	
	TUint8* dstPtr = iTransformedDUIdx;
	for(TInt i = 0; i < aDataUnitCount; i++)
		{
		iTransformedDUIdx[i] = i;
		tempTransformedDUIdx[i] = i;
		}
	
	ASSERT( aDataUnitCount > 0 && aDataUnitCount <= KJpgMaxNumOfDataUnits ); // Defensive programming
	
	switch (aDataUnitCount)
		{
	case KJpgMonochromeDataUnitCount:
		return;		
	case KJpgEColor444DataUnitCount: // 4:4:4 
		return;		
	case KJpgColor422DataUnitCount: // 4:2:2 
		InitTransformCoordinates(posProcess, TRect(TPoint(0,0),TPoint(2,1)), TSize(KJpgPixelRatio, KJpgPixelRatio), iOperationsRequested);
		break;
	case KJpgColor420DataUnitCount: // 4:2:0 
		InitTransformCoordinates(posProcess, TRect(TPoint(0,0),TPoint(2,2)), TSize(KJpgPixelRatio, KJpgPixelRatio), iOperationsRequested);
		break;
	default:
		ASSERT( EFalse );
		}
	do
		{		
		posProcess.GetCurrentPosition(pos);
		TInt elementPos = ((pos.iY * 2) + pos.iX);		
		*dstPtr++ = tempTransformedDUIdx[elementPos];		
		posProcess.MoveNext();		
		} while (!posProcess.IsEndOfImage());	
	}

//
// this section contains many performance-critical code, so use ARM instruction set for it
//
#ifdef __ARMCC__
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif
	
inline
CRgbBufferPtr::TConstRgbBufferPtr CJpgWriteCodec::GetPixels(const TPoint& aPos)
	{
    CRgbBufferPtr::TConstRgbBufferPtr ptr = iRgbInputBuffer->LockBuffer(aPos);			
	return ptr;
	}
	
#if defined(__ARMCC__) && defined(__MARM_ARMV5__)//armv6 and armv7 should be added later
__asm int count_lz(int reserved, int x) 
	{
	clz r0, r1
    bx lr
    endp
	}

inline
TInt MaxPowerOf2(TUint aValue)
	{
	return 32 - count_lz(0, aValue);
	}
#else
const TUint8 KNumBitsTable[64]=
	{
	// 0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xA 0xB 0xC 0xD 0xE 0xF 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F 
	   0,   1,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  4,  4,  4,  4,  5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5
	  ,6,   6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6  
	};

inline
TInt MaxPowerOf2(TUint aValue)
	{
	register TInt powerOf2 = 0;
	do 
		{
		powerOf2 += (aValue >= 64? 6 : KNumBitsTable[aValue]);
		aValue  >>= 6;
		} while (aValue);
	return powerOf2;
	}
#endif

void CJpgWriteCodec::WriteDataUnitL(const TDataUnit& aDataUnit,const TEncHuffmanTable<KEncDCHTSize+1>& aDCTable, const TEncHuffmanTable<KEncACHTSize+1>& aACTable, TInt& aDCPredictor)
	{
#   define MASKED_VALUE(a, num_bit) (TUint(a) & ((1 << num_bit) - 1))//TO optimize

    // make sure that we will never exceed the available buffer size
    // as we may have all the 0xFF which will result in size doubled 
    //
	if ( TUint(iDestPtrLimit - iDestPtr) < KDataUnitSafeBufferSz)
		{
		User::Leave(KErrCompletion);
		}
	TInt value = aDataUnit.iCoeff[0] - aDCPredictor;
	aDCPredictor = aDataUnit.iCoeff[0];
	TInt valueComp = value;

	if (value < 0)
		{
		value = -value;
		valueComp--;
		}

  
	TInt numBits = MaxPowerOf2(value);

	TInt size;
	TInt code=aDCTable.GetCode(size,numBits);
	
	register TUint32 bitBuffer = iBitBuffer;
	register TInt bitsUsed = iBitsUsed;

	bitsUsed = WriteBitsFast(code, size, bitBuffer, bitsUsed);
	bitsUsed = WriteBitsFast(MASKED_VALUE(valueComp, numBits), numBits, bitBuffer, bitsUsed);

	const TInt16* acPtr = aDataUnit.iCoeff + 1;
	const TInt16* acPtrLimit = aDataUnit.iCoeff + KJpgDCTBlockSize;
	TInt runLength = 0;

	FOREVER
		{
		do 
			{
			// here we assume that there is always a non-zero value
			// at the end, the caller must do that.
			value = *acPtr++;
			if (value == 0)
				{
				runLength++;
				}
			} while (value == 0);

		if (acPtr > acPtrLimit)
			{
			break;
			}

		while (runLength > 15)
			{
			code = aACTable.GetCode(size, KJpgZeroRunValue);
			bitsUsed = WriteBitsFast(code, size, bitBuffer, bitsUsed);
			runLength -= 16;
			}

		valueComp = value;
		if (value < 0)
			{
			value = -value;
			valueComp--;
			}
      
		numBits = MaxPowerOf2( (value | 1) );

		code = aACTable.GetCode(size,(runLength << 4) | numBits);
		bitsUsed = WriteBitsFast(code, size, bitBuffer, bitsUsed);
		bitsUsed = WriteBitsFast(MASKED_VALUE(valueComp,numBits), numBits, bitBuffer, bitsUsed);
		runLength = 0;

		}
		
	if (runLength > 0)
		{
		code = aACTable.GetCode(size,0);
		bitsUsed = WriteBitsFast(code, size, bitBuffer, bitsUsed);
		}

	iBitBuffer = bitBuffer;
	iBitsUsed = bitsUsed;

	ASSERT( iDestPtr < iDestPtrLimit );
		
#   undef MASKED_VALUE		
	}
	
FORCEDINLINE TUint32 CJpgWriteCodec::WriteBitsFast(TUint32 aValue, TInt aNumBits, TUint32& aBitBuffer, TInt aBitsUsed)
    {
    if ((aBitsUsed + aNumBits) >  32)
		{
		aNumBits -= (32 - aBitsUsed);
		aBitBuffer |= (aValue >> aNumBits);
		WriteBits(aBitBuffer);
		aBitBuffer = (aValue << (32 - aNumBits));
		return aNumBits;
		}
    else 
		{
		aBitsUsed   += aNumBits;
		aBitBuffer  |= (aValue << (32 - aBitsUsed));
		return aBitsUsed;
		}
    }

void CJpgWriteCodec::WriteBits(TUint32 aBitBuffer)
    {
	register TUint8* bytePtr = (TUint8*)(&aBitBuffer);
	register TUint8 byte;
        
	byte = *(bytePtr+3);
	*iDestPtr++ = byte;
	if (byte == 0xff)
		{
		*iDestPtr++ = 0;
		}

	byte = *(bytePtr+2);
	*iDestPtr++ = byte;
	if (byte == 0xff)
		{
		*iDestPtr++ = 0;
		}
	
	byte = *(bytePtr+1);
	*iDestPtr++ = byte;
	if (byte == 0xff)
		{
		*iDestPtr++ = 0;
		}
	
	byte = *bytePtr;
	*iDestPtr++ = byte;
	if (byte == 0xff)
		{
		*iDestPtr++ = 0;
		}
	}

#ifdef __ARMCC__
#pragma pop
#endif
