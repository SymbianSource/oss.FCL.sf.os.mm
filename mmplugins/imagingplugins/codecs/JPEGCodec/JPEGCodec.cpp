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

// CJpgReadCodec::GetHuffmanCodeL() is based heavily on HUFFMAN_DECODE() from jdhuff.h
// in the IJG code, Copyright (C) 1991-1997, Thomas G. Lane.
/**
   @file
   @internalComponent
*/

#include <fbs.h>
#include "ImageUtils.h"
#include "icl/ImageCodec.h"
#include "JpegTypes.h"
#include "rawcolorprocessor.h"
#include "jpgimageframeprocessor.h"
#include "JPEGCodec.h"
#include "ImageProcessorPriv.h"
#include "fwextconstants.h"
#include <imageprocessor/imageprocessor.h>

#ifdef JPEG_DEBUG_OUTPUT
// This can be turned off in the MMP.
#pragma message("Building Debug version of JPEG codec!")
#endif

const TInt KMaxMCUPerDraw = 100; 	// Maximum MCUs to draw per run
// new value should be 48
const TInt KMarkerLookAhead=32;		// number of bytes which are checked ahead for presense of 0xFF
const TInt KMCUDataLeftThreshhold=192; // approximate value for MCU size, used to avoid re-decoding MCUs when it span data blocks
const TInt KMCUMaxTotalDataUnits=1024; // Maximum of data units to use on this process' heap; if we need more, we'll create a local chunk to store them.


const TInt KUShiftIdx=0;	// index of U shift factor for MCU de-sampling
const TInt KVShiftIdx=1;	// index of V shift factor for MCU de-sampling

#if defined(__ARMCC__)
#pragma push
#pragma thumb
#endif

// CJpgReadCodec
CJpgReadCodec::CJpgReadCodec(const TJpgFrameInfo& aFrameInfo,const TJpgScanInfo& aScanInfo)
 :	iFrameInfo(aFrameInfo),
	iScanInfo(aScanInfo),
	iOperation(EDecodeNormal)
	{
	}


CJpgReadCodec::~CJpgReadCodec()
	{
	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		delete[] iComponent[i];
		}

	delete iImageFrameCodecPtr;
	delete iRawColorProcessor;
	User::Free(iRgbBuffer);
	iRgbBuffer = NULL;
	}

TFrameState CJpgReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	iDataPtr = CONST_CAST(TUint8*,aSrc.Ptr());
	iDataPtrLimit = iDataPtr + aSrc.Length();
	iPreviousDataLeft = iDataPtrLimit - iDataPtr;

	TFrameState frameState = EFrameComplete;
	TRAPD(err, frameState = DoProcessL());
	if (err != KErrNone)
		{
		if (err == KErrCompletion)
			{
			RestoreState();
			frameState = EFrameIncomplete;
			}
		else if (err == KErrEof)
			{
			frameState = EFrameComplete;
			}
		else
			{
			JPEG_LEAVE(err, "ProcessFrameL");
			}
		}

	aSrc.Shift(iDataPtr - aSrc.Ptr()); // Shift out used data

	return frameState;
	}
	
/**
  used to configure U,V DCT for high-speed scaling mode
*/
void CJpgReadCodec::ConfigureUVComponentDCT(TInt aCompIdx)
	{
	switch (iScalingFactor)
		{
		case -2:
			if (iHorzSampleFactor[aCompIdx]==iMaxHorzSampleFactor &&
				iVertSampleFactor[aCompIdx]==iMaxVertSampleFactor)
				{
				iCompConf[aCompIdx].iDCT 			= &iFastHalfDCT;
				iCompConf[aCompIdx].iDequantFunc	= &TQTable::FastHalfDeQuantize;
				}
			break;

		case -3:
			if (iHorzSampleFactor[aCompIdx]==iMaxHorzSampleFactor &&
				iVertSampleFactor[aCompIdx]==iMaxVertSampleFactor)
				{
				iCompConf[aCompIdx].iDCT 			= &iFastQuarterDCT;
				iCompConf[aCompIdx].iDequantFunc	= &TQTable::FastQuarterDeQuantize;
				}
			else if (iHorzSampleFactor[aCompIdx]*2 == iMaxHorzSampleFactor &&
						iVertSampleFactor[aCompIdx]*2 == iMaxVertSampleFactor)
				{
				iCompConf[aCompIdx].iDCT 			= &iFastHalfDCT;
				iCompConf[aCompIdx].iDequantFunc	= &TQTable::FastHalfDeQuantize;
				}
			break;
		default:
			{
			ASSERT(EFalse);
			}
		}
	}
//
//
//
void CJpgReadCodec::InitComponentsL()
	{
	JPEG_ASSERT(iMaxHorzSampleFactor > 0);
	JPEG_ASSERT(iMaxVertSampleFactor > 0);
	
	for (TInt i = 0; i < iFrameInfo.iNumberOfComponents; i++)
		{
		iHorzSampleFactor[i] = iFrameInfo.iComponent[i].iHorzSampleFactor;
		iVertSampleFactor[i] = iFrameInfo.iComponent[i].iVertSampleFactor;
		iHorzSampleRatio[i] = iMaxHorzSampleFactor / iHorzSampleFactor[i];
		iVertSampleRatio[i] = iMaxVertSampleFactor / iVertSampleFactor[i];
		
		TInt dataUnits = iHorzSampleFactor[i] * iVertSampleFactor[i];
		iMCUDataUnitCount[i] = dataUnits;
		if ((i == 0) || !iMonochrome)
			{
			JPEG_ASSERT(dataUnits > 0);
			delete[] iComponent[i];
			iComponent[i] = NULL;
			iComponent[i] = new(ELeave) TDataUnit[dataUnits];
			}
		}
	}

//
//
//
void CJpgReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* /*aFrameMask*/)
	{
	JPEG_DEBUG1("InitFrameL(TFrameInfo)");
	JPEG_DEBUG3(" - iFrameInfo dimensions: %d x %d",
			iFrameInfo.iSizeInPixels.iWidth,
			iFrameInfo.iSizeInPixels.iHeight);

	JPEG_DEBUG3(" - aFrame dimensions: %d x %d",
			aFrame.SizeInPixels().iWidth,
			aFrame.SizeInPixels().iHeight);

	PreInitFrameL();

	iMonochrome |= (aFrame.DisplayMode() <= EGray256);

	if ((iMaxHorzSampleFactor != iFrameInfo.iComponent[KYComp].iHorzSampleFactor) ||
		(iMaxVertSampleFactor != iFrameInfo.iComponent[KYComp].iVertSampleFactor))
		{
		// Current implementation doesn't support images with Y sampling frequency
		// lower than U or V components
		JPEG_LEAVE(KErrNotSupported, "Unsupported sampling frequency");
		}

	InitComponentsL();
	for (TInt i = 0; i < iFrameInfo.iNumberOfComponents; i++)
		{
		if ((iFrameInfo.iComponent[i].iVertSampleFactor == 3) ||
			(iFrameInfo.iComponent[i].iHorzSampleFactor == 3))
			{
			JPEG_LEAVE(KErrCorrupt, "Bad sample factor");
			}
		
		if ((i == 1) || (i == 2))
			{
			iHorzSampleRatioSh[i - 1] = iHorzSampleRatio[i] / 2;
			iVertSampleRatioSh[i - 1] = iVertSampleRatio[i] / 2;
			}
		}

	TSize requiredSize = iFrameInfo.iSizeInPixels;
	TInt err = iExtensionManager->GetDestinationSize(requiredSize);
	JPEG_LEAVE_IF_ERROR(err, "GetDestinationSize failed");
	
	if (iExtensionManager->ScalerExtensionRequested())
		{
		// Explicit scaling
		// Mandatory check that the destination size matches the calculated size
		if (requiredSize != aFrame.SizeInPixels())
			{
			JPEG_LEAVE(KErrArgument, "Destination bitmap size != GetDestinationSize");
			}
		
		TSize sizeToScale = iExtensionManager->ClippingRectExtensionRequested() ?
							iExtensionManager->ClippingRect().Size() : iFrameInfo.iSizeInPixels;
		
		if (iExtensionManager->DimensionsSwapped())
			{
			sizeToScale.SetSize(sizeToScale.iHeight, sizeToScale.iWidth);
			}
		
		err = iExtensionManager->GetScalingCoefficient(iScalingFactor, &sizeToScale);
		JPEG_LEAVE_IF_ERROR(err, "GetScalingCoefficient failed");
			
		if (iScalingFactor != -4)
			{
			if (!iHighSpeedMode || (iHighSpeedMode && iMonochrome))
				{
				// Needs to be the original image/cropped size in correct orientation
				// and NOT the GetDestinationSize() value
				requiredSize = sizeToScale;	
				}
			}
		}
	else
		{
		// Implicit scaling for compatibility
		// requiredSize now equals full, cropped or scaled in correct orientation
		iScalingFactor = ScalingCoefficient(requiredSize, aFrame.SizeInPixels());
		if ((iHighSpeedMode && !iMonochrome) || iScalingFactor == -4)
			{
			err = GetReducedSize(requiredSize, iScalingFactor, requiredSize);
			JPEG_LEAVE_IF_ERROR(err, "GetReducedSize failed");
			}
		}
	
	if (iScalingFactor == 1)
		{
		// For convenience
		iScalingFactor = -iScalingFactor;
		}
	
	JPEG_DEBUG2(" - iMonochrome = %d", iMonochrome);
	JPEG_DEBUG2(" - iProgressive = %d", iProgressive);
	JPEG_DEBUG2(" - iHighSpeedMode = %d", iHighSpeedMode);
	JPEG_DEBUG2(" - iScalingFactor = %d", iScalingFactor);

	// "raw" processor is used when we can directly write data in output format rather than TRgb
	// that provides with saving on "to/from" TRgb conversion and memory bandwidth
	TBool useRawProc = (!iMonochrome && !iProgressive &&
										(aFrame.DisplayMode() == EColor16M ||
										(aFrame.DisplayMode() == EColor64K &&
										 CPluginExtensionManager::ConvertScalingCoeffToReductionFactor(iScalingFactor) == 3))
						);

	// defaults which are equal to legacy behaviour
	iMcuWriteFunc = (iMonochrome ? &CJpgReadCodec::WriteMonoMCU : &CJpgReadCodec::WriteUnScaledMCU);

	iCompConf[KYComp].iDequantFunc = &TQTable::DeQuantize;
	iCompConf[KUComp].iDequantFunc = &TQTable::DeQuantize;
	iCompConf[KVComp].iDequantFunc = &TQTable::DeQuantize;

	TInt reductionFactor = 0;
	switch (iScalingFactor)
		{
		case -1:
			reductionFactor = 0;
			iCompConf[KYComp].iDCT = &iFullDCT;
			iCompConf[KUComp].iDCT = &iFullDCT;
			iCompConf[KVComp].iDCT = &iFullDCT;

			if (iMonochrome)
				{
				iMcuWriteFunc = &CJpgReadCodec::WriteMonoMCU;
				JPEG_DEBUG1(" - Using Mono function");
				}
			else
				{
				if (useRawProc)
					{
					iMcuWriteFunc = &CJpgReadCodec::WriteUnScaledMCU16M;
					}
				else
					{
					iMcuWriteFunc = &CJpgReadCodec::WriteUnScaledMCU;
					}
				JPEG_DEBUG1(" - Using Unscaled functions");
				}
			break;

		case -2:
			reductionFactor = 1;
			iCompConf[KYComp].iDCT = &iHalfDCT;
			iCompConf[KUComp].iDCT = &iHalfDCT;
			iCompConf[KVComp].iDCT = &iHalfDCT;
			if (!iMonochrome && iHighSpeedMode) // we do not support fast scaling for monochrome
				{
				reductionFactor = 0;
				if (useRawProc)
					{
					iMcuWriteFunc = &CJpgReadCodec::WriteDiv2ScaledMCU16M;
					}
				else
					{
					iMcuWriteFunc = &CJpgReadCodec::WriteDiv2ScaledMCU;
					}
				JPEG_DEBUG1(" - Using Div2 functions");

				iCompConf[KYComp].iDCT 			= &iFastHalfDCT;
				iCompConf[KYComp].iDequantFunc	= &TQTable::FastHalfDeQuantize;
				ConfigureUVComponentDCT(KUComp);
				ConfigureUVComponentDCT(KVComp);
				}
			else
				{
				iScalingFactor = -1;
				useRawProc = EFalse;
				}
			break;

		case -3:
			reductionFactor = 2;
			iCompConf[KYComp].iDCT = &iQuarterDCT;
			iCompConf[KUComp].iDCT = &iQuarterDCT;
			iCompConf[KVComp].iDCT = &iQuarterDCT;
			if (!iMonochrome && iHighSpeedMode) // we do not support fast scaling for monochrome
				{
				reductionFactor = 0;
				if (useRawProc)
					{
					iMcuWriteFunc = &CJpgReadCodec::WriteDiv4ScaledMCU16M;
					}
				else
					{
					iMcuWriteFunc = &CJpgReadCodec::WriteDiv4ScaledMCU;
					}
				JPEG_DEBUG1(" - Using Div4 functions");

				iCompConf[KYComp].iDCT = &iFastQuarterDCT;
				iCompConf[KYComp].iDequantFunc = &TQTable::FastQuarterDeQuantize;
				ConfigureUVComponentDCT(KUComp);
				ConfigureUVComponentDCT(KVComp);
				}
			else
				{
				iScalingFactor = -1;
				useRawProc = EFalse;
				}
			break;

		case -4:
			iCompConf[KYComp].iDCT = &iEighthDCT;
			iCompConf[KUComp].iDCT = &iEighthDCT;
			iCompConf[KVComp].iDCT = &iEighthDCT;
			iCompConf[KYComp].iDequantFunc = &TQTable::Fast18DeQuantize;
			iCompConf[KUComp].iDequantFunc = &TQTable::Fast18DeQuantize;
			iCompConf[KVComp].iDequantFunc = &TQTable::Fast18DeQuantize;
			reductionFactor = 0;
			if (!iMonochrome) // 1/8 Mono has got its own fast 1/8 inside the function
				{
				iMcuWriteFunc = useRawProc? (aFrame.DisplayMode()==EColor16M ? &CJpgReadCodec::WriteDiv8ScaledMCU16M : &CJpgReadCodec::WriteDiv8ScaledMCU64K)
											: &CJpgReadCodec::WriteDiv8ScaledMCU;
				JPEG_DEBUG1(" - Using Div8 functions");
				}
			break;

		default:
			JPEG_LEAVE(KErrArgument, "Bad scaling factor");
			break;
		}

	iCompConf[KYComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);
	iCompConf[KUComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);
	iCompConf[KVComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);

	if (!iMonochrome)
		{
		CalculateRgbIndeces();
		}

	if (iProgressive)
		{
		iFrameInfo.iMCUBlocksPerLine = 1;
		}
	else
		{
		TInt mcuWidth = iFrameInfo.MCUWidthInPixels();
		iFrameInfo.iMCUBlocksPerLine = (iFrameInfo.iSizeInPixels.iWidth + mcuWidth - 1) / mcuWidth;
		}

	// Allocate the intermediate buffer.
	iPixelSize = useRawProc ? TDisplayModeUtils::NumDisplayModeBitsPerPixel(aFrame.DisplayMode()) / 8 : sizeof(TRgb);

	TInt scaleDivisor = CJpgReadCodec::ScaleFactorToDivisorL(iScalingFactor);
	ConfigureAndAllocateBufferL(scaleDivisor);

	CImageProcessorExtension* imageProc = NULL;
	if (useRawProc)
		{
		JPEG_DEBUG1(" - Using raw processor");
		imageProc = CRawColorProcessor::NewL();
		}
	else
		{
		JPEG_DEBUG2(" - Using stock processor, reductionFactor=%d", reductionFactor);
		imageProc = ImageProcessorUtility::NewImageProcessorExtensionL(
						aFrame,
						reductionFactor,
						iMonochrome ? EGray256 : ERgb,
						aDisableErrorDiffusion || iHighSpeedMode);
		}
	
	ConfigureImageProcessorL(imageProc, aFrame, scaleDivisor, requiredSize);	
	CalculateRenderingParams(scaleDivisor);
	PostInitFrameL();
	}

//
// Configures the image processor that has been created.
// This function does not take ownership of the image processor.
//
void CJpgReadCodec::ConfigureImageProcessorL(CImageProcessorExtension* aImageProc, CFbsBitmap& aFrame, const TInt aScaleDivisor, const TSize& aPrepareLSize)
	{
	JPEG_DEBUG2("ConfigureImageProcessorL(aScaleDivisor=%d)", aScaleDivisor);
	JPEG_ASSERT(aImageProc);
	JPEG_ASSERT(iExtensionManager);
	JPEG_ASSERT(aScaleDivisor > 0);
	JPEG_ASSERT(aPrepareLSize.iWidth > 0);
	JPEG_ASSERT(aPrepareLSize.iHeight > 0);
	

#ifdef JPEG_DEBUG_OUTPUT
	TRAPD(err, iExtensionManager->TransferExtensionDataL(aImageProc));
	JPEG_LEAVE_IF_ERROR(err, "TransferExtensionDataL");
#else
	iExtensionManager->TransferExtensionDataL(aImageProc);
#endif

	SetImageProcessor(aImageProc);

	// Set the padding variables.
	TRect clip = iExtensionManager->ClippingRect();
	TInt left = (clip.iTl.iX - iMCUClipRect.iTl.iX) / aScaleDivisor;
	TInt top = (clip.iTl.iY - iMCUClipRect.iTl.iY) / aScaleDivisor;
	TInt bottom = (iMCUClipRect.iBr.iY - clip.iBr.iY) / aScaleDivisor;
	TInt right = (iMCUClipRect.iBr.iX - clip.iBr.iX) / aScaleDivisor;

	JPEG_DEBUG1(" - clipping rect offsets (scaled):");
	JPEG_DEBUG2(" -   top: %d", top);
	JPEG_DEBUG2(" -   left: %d", left);
	JPEG_DEBUG2(" -   bottom: %d", bottom);
	JPEG_DEBUG2(" -   right: %d", right);	
	JPEG_DEBUG3(" - Output size: %d x %d", aPrepareLSize.iWidth, aPrepareLSize.iHeight);
	
	//TBool bottomUp = ETrue;
	TInt initialPadding = 0;
	TInt padding = 0;

	// iRubbish is only valid for non-clipped images.
	switch (iOperation)
		{
		case EDecodeNormal:
		case EDecodeHorizontalFlip:
			initialPadding = (iUseClipRect ? top : 0);
			padding = (iUseClipRect ? left : 0);
			break;

		case EDecodeVerticalFlip:
		case EDecodeRotate180:
			initialPadding = (iUseClipRect ? top : 0);
			padding = (iUseClipRect ? right : iRubbish.iWidth);
			break;

		case EDecodeRotate90:
		case EDecodeVerticalFlipRotate90:
			initialPadding = (iUseClipRect ? left : 0);
			padding = (iUseClipRect ? bottom : iRubbish.iHeight);
			break;

		case EDecodeHorizontalFlipRotate90:
		case EDecodeRotate270:
			initialPadding = (iUseClipRect ? left : 0);
			padding = (iUseClipRect ? top : 0);
			break;

		default:
			ASSERT(EFalse);
		}

	JPEG_DEBUG2(" - initialPadding = %d", initialPadding);
	JPEG_DEBUG2(" - padding = %d", padding);

	JPEG_ASSERT(aImageProc);

	aImageProc->SetInitialScanlineSkipPadding(initialPadding);
	aImageProc->SetPixelPadding(padding);
	aImageProc->PrepareL(aFrame, aPrepareLSize, iBufSize);
	}

//
//
//
TInt CJpgReadCodec::ScaleFactorToDivisorL(TInt aScalingFactor)
	{
	switch (aScalingFactor)
		{
		case 1:
		case -1:
			return 1;

		case -2:
			return 2;

		case -3:
			return 4;

		case -4:
			return 8;

		default:
			break;
		}

	// Keep the compiler happy.
	JPEG_LEAVE(KErrArgument, "Bad scaling factor");
	return 0;
	}

//
// Calculates the necessary intermediate buffer size and allocates it.
// It also sets how many MCUs are contained in the buffer (iMCUsPerBuffer).
//
void CJpgReadCodec::ConfigureAndAllocateBufferL(const TInt aScale)
	{
	JPEG_DEBUG2("ConfigureAndAllocateBufferL(aScale=%d)", aScale);
	JPEG_ASSERT(aScale > 0);

	TInt horizMCUs = 0;
	TInt vertMCUs = 0;

	if (iUseClipRect)
		{
		// We need to translate the clipping rect from pixels to MCU space.
		// Clipping is done before any rotation.
		JPEG_ASSERT(!iProgressive);

		CalculateMCUBoundingRectL(iFrameInfo.iMCUBlocksPerLine);
		horizMCUs = iMCUClipRect.Width() / iFrameInfo.MCUWidthInPixels();
		vertMCUs = iMCUClipRect.Height() / iFrameInfo.MCUHeightInPixels();
		}
	else
		{
		if (iProgressive)
			{
			JPEG_ASSERT(aScale == 1);
			JPEG_ASSERT(iOperation == EDecodeNormal);

			horizMCUs = 1;
			vertMCUs = 1;
			}
		else
			{
			horizMCUs = iFrameInfo.iMCUBlocksPerLine;
			vertMCUs = iFrameInfo.iMCUBlocksPerColumn;

			iRubbish.iWidth = ((horizMCUs * iFrameInfo.MCUWidthInPixels()) - iFrameInfo.iSizeInPixels.iWidth) / aScale;
			iRubbish.iHeight = ((vertMCUs * iFrameInfo.MCUHeightInPixels()) - iFrameInfo.iSizeInPixels.iHeight) / aScale;

			JPEG_DEBUG3(" - iRubbish: %d x %d (scaled)", iRubbish.iWidth, iRubbish.iHeight);
			JPEG_ASSERT(iRubbish.iWidth >= 0);
			JPEG_ASSERT(iRubbish.iHeight >= 0);
			}
		}

	// We may need to swap dimensions for some rotates.
	switch (iOperation)
		{
		case EDecodeRotate90:
		case EDecodeRotate270:
		case EDecodeHorizontalFlipRotate90:
		case EDecodeVerticalFlipRotate90:
			iMCUsPerBuffer = vertMCUs;
			iBufSize.iWidth = vertMCUs * iFrameInfo.MCUHeightInPixels();
			iBufSize.iHeight = iFrameInfo.MCUWidthInPixels();
			break;

		default:
			iMCUsPerBuffer = horizMCUs;
			iBufSize.iWidth = horizMCUs * iFrameInfo.MCUWidthInPixels();
			iBufSize.iHeight = iFrameInfo.MCUHeightInPixels();
		}

	iBufSize.iWidth /= aScale;
	iBufSize.iHeight /= aScale;

	// Because JPEG doesn't round up with scaling it is possible to have a scaled image that 0x0 in size.
	// Things are ok for now because a 1x1 image is actually an 8x8 image and the maximum reduction factor
	// supported is -4 (divide by 8). It is possible that larger reduction factors will be supported in
	// the future as cameras take larger pictures. In this case an 8x8 image will scale to 0x0.
	JPEG_DEBUG3(" - scaled buffer dimensions: %d x %d", iBufSize.iWidth, iBufSize.iHeight);
	JPEG_ASSERT(iBufSize.iWidth > 0);
	JPEG_ASSERT(iBufSize.iHeight > 0);
	JPEG_ASSERT(iPixelSize > 0);

	TInt bufSize = iBufSize.iWidth * iBufSize.iHeight * iPixelSize;

	User::Free(iRgbBuffer);
	iRgbBuffer = NULL;
	iRgbBuffer = reinterpret_cast<TRgb*>(User::AllocL(bufSize));
	}


//
// Sets up the fields that control where the pixels are
// drawn into the intermediate buffer.
//
void CJpgReadCodec::CalculateRenderingParams(const TInt aScale)
	{
	JPEG_DEBUG1("CalculateRenderingParams()");

	JPEG_ASSERT(aScale > 0);

	TInt mcuPixelWidth = iFrameInfo.MCUWidthInPixels() / aScale;
	TInt mcuPixelHeight = iFrameInfo.MCUHeightInPixels() / aScale;

	switch (iOperation)
		{
		case EDecodeNormal:
		case EDecodeHorizontalFlip:
			// These are the same except FlipHorizontal will draw from bottom up.
			iFirstPixelOffset = 0;
			iPixelIncrement = 1;
			iRgbBufNextLineOffs = iBufSize.iWidth - mcuPixelWidth;
			iFillBufferBackwards = EFalse;
			iMCUHorizExtent = mcuPixelWidth;
			break;

		case EDecodeVerticalFlip:
		case EDecodeRotate180:
			iFirstPixelOffset = mcuPixelWidth - 1;
			iPixelIncrement = -1;
			iRgbBufNextLineOffs = iBufSize.iWidth + mcuPixelWidth;
			iFillBufferBackwards = ETrue;
			iMCUHorizExtent = mcuPixelWidth;
			break;

		case EDecodeRotate90:
		case EDecodeVerticalFlipRotate90:
			// These differ only in the direction the buffer is copied to the bitmap.
			iFirstPixelOffset = mcuPixelHeight - 1;
			iPixelIncrement = iBufSize.iWidth;
			iRgbBufNextLineOffs = -((iBufSize.iWidth * mcuPixelWidth) + 1);
			iFillBufferBackwards = ETrue;
			iMCUHorizExtent = mcuPixelHeight;
			break;

		case EDecodeRotate270:
		case EDecodeHorizontalFlipRotate90:
			// These differ only in the direction the buffer is copied to the bitmap.
			iFirstPixelOffset = 0;
			iPixelIncrement = iBufSize.iWidth;
			iRgbBufNextLineOffs = -(iBufSize.iWidth * mcuPixelWidth) + 1;
			iFillBufferBackwards = EFalse;
			iMCUHorizExtent = mcuPixelHeight;
			break;

		default:
			// Bad operation.
			ASSERT(EFalse);
		}

	JPEG_DEBUG2(" - iFirstPixelOffset = %d", iFirstPixelOffset);
	JPEG_DEBUG2(" - iPixelIncrement = %d", iPixelIncrement);
	JPEG_DEBUG2(" - iRgbBufNextLineOffs = %d", iRgbBufNextLineOffs);
	JPEG_DEBUG2(" - iFillBufferBackwards = %d", iFillBufferBackwards);
	JPEG_DEBUG2(" - iMCUHorizExtent = %d", iMCUHorizExtent);
	}

//
// This is called when a clipping rectangle has been set.
// Subclasses that support clipping must provide a way of
// mapping the clip rect pixel coordinates to MCUs.
//
void CJpgReadCodec::CalculateMCUBoundingRectL(TInt /*aMCUsPerLine*/)
	{
	JPEG_LEAVE(KErrNotSupported, "CalculateMCUBoundingRectL");
	}

/**
   we would go across the whole block top-to-bottom left-to-right
*/
void CJpgReadCodec::CalculateRgbIndeces()
	{
	TUVidxElemType* pixIdxPtr=iUVIndeces;

	TUint offsMask=iMaxVertSampleFactor*iMaxHorzSampleFactor*KJpgDCTBlockSize -1;
	TInt vertStep=1;

	switch (iScalingFactor)
		{
		case -4:
			offsMask &= ~TUint(KJpgDCTBlockSize - 1);
			break;

		case -3:
			vertStep = 4;
			break;

		case -2:
			vertStep = 2;
			break;

		default:
			ASSERT((iScalingFactor == 1) || (iScalingFactor == -1));
		}

	for (TInt j = 0; j < iMaxVertSampleFactor*KJpgDCTBlockWidth; j+=vertStep)
		{

		for (TInt i = 0; i < iMaxHorzSampleFactor; i++)
			{
			for (TInt k=0; k<2; ++k)
				{
				const TInt shiftIdx=k;
				const TInt comp=k+1;
				TInt hOffs		=((i*KJpgDCTBlockWidth)>>iHorzSampleRatioSh[shiftIdx])&7;
				TInt hBlkOffs	=(i>>iHorzSampleRatioSh[shiftIdx]) *KJpgDCTBlockSize;

				TInt vOffs		=((j>>iVertSampleRatioSh[shiftIdx]&7))*KJpgDCTBlockWidth;

				TInt vBlkOffs	=((j/KJpgDCTBlockWidth) >>iVertSampleRatioSh[shiftIdx]) *KJpgDCTBlockSize*iHorzSampleFactor[comp];

				*pixIdxPtr++ =  offsMask & (hOffs+  hBlkOffs + vOffs + vBlkOffs);
				}

			if (iScalingFactor == -3)
				{
				// for this factor we have 1 more entry per horizontal line
				// which is offset against previous pixel i.e. (4>>iHorzSampleRatioSh[Comp])
				*pixIdxPtr++ = vertStep>>iHorzSampleRatioSh[KUShiftIdx];
				*pixIdxPtr++ = vertStep>>iHorzSampleRatioSh[KVShiftIdx];
				}
			}
		}
	ASSERT(pixIdxPtr-iUVIndeces <= sizeof(iUVIndeces)/sizeof(iUVIndeces[0]) );
	}

void CJpgReadCodec::InitFrameL(CImageFrame& aDest)
	{
	JPEG_DEBUG1("CJpgReadCodec::InitFrameL(CImageFrame)");

	PreInitFrameL();

	// This JPEG codec does not support scaling when using destination of type CImageFrame
	// It leaves if scaling has been requested.
	//TInt reductionFactor = 0;
	
	if (iOperation != EDecodeNormal)
		{
		JPEG_LEAVE(KErrNotSupported, "No operations on CImageFrame");
		}
	
	if (iExtensionManager->ScalerExtensionRequested())
		{
		JPEG_LEAVE(KErrNotSupported, "No scaling on CImageFrame");
		}

	if (iUseClipRect)
		{
		JPEG_LEAVE(KErrNotSupported, "No clipping on CImageFrame");
		}
	
	// Check for implicit scaling.
	TInt reductionFactor = 0;
	if (!iUseClipRect)
		{
		reductionFactor = ReductionFactor(iFrameInfo.iSizeInPixels, aDest.FrameSizeInPixels());
		}
	
	if (reductionFactor != 0)
		{
		JPEG_DEBUG2(" - reductionFactor = %d", reductionFactor);
		JPEG_LEAVE(KErrNotSupported, "Bad reductionFactor");
		}

	InitComponentsL();

	iCompConf[KYComp].iDequantFunc = &TQTable::DeQuantize;
	iCompConf[KUComp].iDequantFunc = &TQTable::DeQuantize;
	iCompConf[KVComp].iDequantFunc = &TQTable::DeQuantize;

	iCompConf[KYComp].iDCT = &iFullDCT;
	iCompConf[KUComp].iDCT = &iFullDCT;
	iCompConf[KVComp].iDCT = &iFullDCT;

	iCompConf[KYComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);
	iCompConf[KUComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);
	iCompConf[KVComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);

	// Create JPEG read codec extension and the appropriate image processor
	ASSERT(iImageFrameCodecPtr == NULL);
	delete iImageFrameCodecPtr;
	iImageFrameCodecPtr = NULL;
	iImageFrameCodecPtr = CJpgImageFrameReadCodec::NewL(&aDest);
	iImageFrameCodecPtr->CreateImageProcessorL(iFrameInfo);

	PostInitFrameL();
	}

//initialization for streaming
void CJpgReadCodec::InitFrameL(TUid aFormat, TDecodeStreamCaps::TNavigation aNavigation)
	{
	JPEG_DEBUG1("InitFrameL(TUid)");

	//validate format with frameinfo
	PreInitFrameL();

	ValidateFormatL(iFrameInfo, aFormat);

	iIsBlockStreaming = ETrue;

	iStreamFormat = aFormat;
	iNavigation = aNavigation;

	InitComponentsL();

	iCompConf[KYComp].iDequantFunc = &TQTable::DeQuantize;
	iCompConf[KUComp].iDequantFunc = &TQTable::DeQuantize;
	iCompConf[KVComp].iDequantFunc = &TQTable::DeQuantize;

	iCompConf[KYComp].iDCT = &iFullDCT;
	iCompConf[KUComp].iDCT = &iFullDCT;
	iCompConf[KVComp].iDCT = &iFullDCT;

	iCompConf[KYComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);
	iCompConf[KUComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);
	iCompConf[KVComp].iDCT->SetPrecision(iFrameInfo.iSamplePrecision);

	// Create JPEG read codec extension and the appropriate image processor
	delete iImageFrameCodecPtr;
	iImageFrameCodecPtr = NULL;
	iImageFrameCodecPtr = CJpgImageFrameReadCodec::NewL(NULL);

	PostInitFrameL();
	}

#if defined(__ARMCC__)
#pragma pop
#endif

//get blocks for streaming
void CJpgReadCodec::GetBlocksL(CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	ASSERT(aNumBlocksRead);

	if(aFrame == NULL || !(iNavigation == TDecodeStreamCaps::ENavigationRandomForward || iNavigation == TDecodeStreamCaps::ENavigationRandomBackwards))
		{
		JPEG_LEAVE(KErrArgument, "GetBlocks - bad params");
		}

	ValidateImageFrameBlockL(aFrame);

	iImageFrameCodecPtr->SetImageFrameBlocksL(aFrame, iFrameInfo);

	*aNumBlocksRead = 0;
	iStreamDecodeConfig.iSeqPosition = aSeqPosition;
	iStreamDecodeConfig.iNumBlocksToGet = aNumBlocksToGet;
	iStreamDecodeConfig.iNumBlocksRead = aNumBlocksRead;
	}

//get blocks for streaming
void CSequentialJpgReadCodec::GetBlocksL(CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	if (aSeqPosition < 0)
		{
		JPEG_LEAVE(KErrArgument, "GetBlocksL - bad aSeqPosition");
		}

	iNeededMCU = aSeqPosition;

	CJpgReadCodec::GetBlocksL(aFrame, aSeqPosition, aNumBlocksToGet, aNumBlocksRead);
	}

//get blocks for streaming
void CJpgReadCodec::GetNextBlocksL(CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	ASSERT(aNumBlocksRead);
	ASSERT(aHaveMoreBlocks);

	if (aFrame == NULL || iNavigation != TDecodeStreamCaps::ENavigationSequentialForward)
		{
		JPEG_LEAVE(KErrArgument, "GetNextBlocksL - bad params");
		}

	ValidateImageFrameBlockL(aFrame);

	iImageFrameCodecPtr->SetImageFrameBlocksL(aFrame, iFrameInfo);

	*aNumBlocksRead = 0;
	*aHaveMoreBlocks = ETrue;
	iStreamDecodeConfig.iNumBlocksToGet = aNumBlocksToGet;
	iStreamDecodeConfig.iNumBlocksRead = aNumBlocksRead;
	iStreamDecodeConfig.iHaveMoreBlocks = aHaveMoreBlocks;
	}

//validates the blocks passed.
void CJpgReadCodec::ValidateImageFrameBlockL(CImageFrame* aFrame)
	{
	ASSERT(aFrame);

	TSize aBlockSizeInPixels = aFrame->FrameSizeInPixels();
	TSize aRefSizeInPixels = TSize(iFrameInfo.MCUWidthInPixels(), iFrameInfo.MCUHeightInPixels());

	const TFrameFormat& format = static_cast<const TFrameFormat&>(aFrame->FrameFormat());
	TUid imageFrameFormatCode = format.FormatCode();

	TInt oddPixelsWidth = aBlockSizeInPixels.iWidth % aRefSizeInPixels.iWidth;
	TInt oddPixelsHeight = aBlockSizeInPixels.iHeight % aRefSizeInPixels.iHeight;

	if (oddPixelsWidth != 0 || oddPixelsHeight != 0 || aBlockSizeInPixels.iHeight != aRefSizeInPixels.iHeight)
		{
		User::Leave(KErrNotSupported);
		}

	if(iIsBlockStreaming == EFalse)
		{
		User::Leave(KErrNotReady);
		}

	if(imageFrameFormatCode != KNullUid)
		{
		if(imageFrameFormatCode != iStreamFormat)
			{
			User::Leave(KErrArgument);
			}
		}
	}

//get buffer and blockSizeInPix for streaming
TInt CJpgReadCodec::GetStreamBufferSizeL(TUid aFormatCode, TSize& aBlockSizeInPixels, TInt aNumBlocks)
	{
	return CJpgImageFrameProcessorUtility::RecommendedStreamBufferSizeL(iFrameInfo, aFormatCode, aBlockSizeInPixels, aNumBlocks);
	}

//validate format
void CJpgReadCodec::ValidateFormatL(const TJpgFrameInfo& aFrameInfo, TUid aFormatCode)
	{
	TInt dataUnitCount=0;

	if (aFrameInfo.iNumberOfComponents == 1)
		{
		dataUnitCount = 1;
		}
	else
		{
		dataUnitCount = 0;
		for (TInt index = 0; index < aFrameInfo.iNumberOfComponents; index++)
			{
			dataUnitCount += aFrameInfo.iComponent[index].iHorzSampleFactor *
							 aFrameInfo.iComponent[index].iVertSampleFactor;
			}
		}

	switch (dataUnitCount)
		{

		case 1: // Monochrome
			{
			if (aFormatCode != KUidFormatYUVMonochrome)
				{
				// error transcoding not supported
				User::Leave(KErrNotSupported);
				}
			break;
			}

		case 4: // 4:2:2
			{
			if (aFormatCode != KUidFormatYUV422Interleaved)
				{
				// error transcoding not supported
				User::Leave(KErrNotSupported);
				}
			break;
			}

		case 6: // 4:2:0
			{
			if (!(aFormatCode == KUidFormatYUV420Planar || aFormatCode == KUidFormatYUV420PlanarReversed))
				{
				// error transcoding not supported
				User::Leave(KErrNotSupported);
				}
			break;
			}

		default:
			{
			User::Leave(KErrNotSupported);
			break;
			}
		}
	}

//
// This function should be called at beginning of the
// various InitFrameL versions. It performs initialisation
// that's common to all forms of image.
//
void CJpgReadCodec::PreInitFrameL()
	{
	JPEG_DEBUG1("PreInitFrameL()");
	JPEG_DEBUG2(" - MCU pixel width: %d", iFrameInfo.MCUWidthInPixels());
	JPEG_DEBUG2(" - MCU pixel height: %d", iFrameInfo.MCUHeightInPixels());

	iHorzMCUBlkCount = 0;
	iScalingFactor = 1;
	iMCUsPerBuffer = (iFrameInfo.iProgressive ? 1 : iFrameInfo.iMCUBlocksPerLine);
	ASSERT(iMCUsPerBuffer > 0);
	
	iMonochrome = (iFrameInfo.iNumberOfComponents == 1);
	
	TInt interval = iFrameInfo.iRestartInterval;
	iRestartMCUCount = (interval > 0 ? interval : KErrNotFound);
	JPEG_DEBUG2(" - iRestartMCUCount = %d", iRestartMCUCount);
	
	iMaxHorzSampleFactor = iFrameInfo.iMaxHorzSampleFactor;
	iMaxVertSampleFactor = iFrameInfo.iMaxVertSampleFactor;
	JPEG_DEBUG2(" - iMaxHorzSampleFactor = %d", iMaxHorzSampleFactor);
	JPEG_DEBUG2(" - iMaxVertSampleFactor = %d", iMaxVertSampleFactor);
	
	// Get info from the extension manager. The InitFrameL function can
	// decide whether or not the operation is supported.
	ASSERT(iExtensionManager);

	if (iAutoRotateFlag > 1 && iAutoRotateFlag < 9) 
		{
		// To ensure operation extension is created
		iExtensionManager->CreateExtensionForAutoRotateL();
		iOperation = iExtensionManager->OperationL(iAutoRotateFlag);
		}
	else 
		{
		iOperation = iExtensionManager->Operation();
		}
	
	// check out if we need to clip.
	iUseClipRect = EFalse;
	if (iExtensionManager->ClippingRectExtensionRequested())
		{
		iUseClipRect = ETrue;
		}
	
	ResetState();
	}

//
//
//
void CJpgReadCodec::PostInitFrameL()
	{
	// Default implementation does nothing.
	}

//
//
//
void CJpgReadCodec::StoreState()
	{
	iInitialDataPtr = iDataPtr;
	iInitialDataValue = iDataValue;
	iInitialBitsLeft = iBitsLeft;
	iBitBufferPtrLimit = NULL;

	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		iInitialDCPredictor[i] = iDCPredictor[i];
		}
	}

//
//
//
void CJpgReadCodec::RestoreState()
	{
	iDataPtr = iInitialDataPtr;
	iBitBufferPtrLimit = NULL;
	iDataValue = iInitialDataValue;
	iBitsLeft = iInitialBitsLeft;

	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		iDCPredictor[i] = iInitialDCPredictor[i];
		}
	}

//
//
//
void CJpgReadCodec::ResetState()
	{
	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		iDCPredictor[i] = 0;
		}

	iBitsLeft = 0;
	iBitBufferPtrLimit = NULL;
	}

//
// Returns the number of bytes that had to be skipped before the restart marker was found.
//
TInt CJpgReadCodec::RestartStateL()
	{
	const TUint8* startPtr = iDataPtr;
	const TUint8* readLimit = iDataPtrLimit - 2;

	// skip the data until the marker is found
	while (*iDataPtr != 0xff && iDataPtr < readLimit)
		{
		iDataPtr++;
		}

	if (*iDataPtr != 0xff)
		{
		User::Leave(KErrCompletion);
		}

	iDataPtr++;

	TInt marker = *iDataPtr++;

	if (marker == (KJpgEOISignature & 0x00ff))
		{
		User::Leave(KErrEof);
		}
	else if ((marker & 0xf0) != 0xd0)
		{
#if !defined(RELAX_JPEG_STRICTNESS)
		User::Leave(KErrCorrupt);
#endif
		}

	// if iRestartInterval is 0, iRestartMCUCount is set to a negative value (KErrNotFound) to skip the 0 trigger points that would call RestartStateL
	// This is done to solve the problem that on some images the iRestartInterval marker is 0 on every frame
	iRestartMCUCount = iFrameInfo.iRestartInterval > 0 ? iFrameInfo.iRestartInterval : KErrNotFound;
	ResetState();
	StoreState();
	
	return iDataPtr - startPtr;
	}

CJpgImageFrameReadCodec* CJpgReadCodec::ImageFrameCodec()
	{
	return iImageFrameCodecPtr;
	}

TInt CJpgReadCodec::ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const
	{
	aReducedSize = aOriginalSize;
	if (aReductionFactor < 0 || aReductionFactor > 3)
		{
		return KErrArgument;
		}
	else
		{
		return CImageReadCodec::ReducedSize(aOriginalSize, aReductionFactor, aReducedSize);	
		}
	}

#if defined(__ARMCC__)
// use ARM instruction for performance-critical code
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif

TInt CJpgReadCodec::FillBitBufferL(TInt aBLeft)
	{
	const TInt KBytesToFetch= 4;
	const TUint8* dataPtr   = iDataPtr;
	
	const TUint8* bitBufLim = iBitBufferPtrLimit;
	if (dataPtr + KBytesToFetch >=  bitBufLim)
		{
		bitBufLim = JpegFillBuffer(dataPtr);
		}
		
	register TUint dataValue = iDataValue;
	
	if (dataPtr + KBytesToFetch < bitBufLim )
		{
		if (aBLeft == 0)
		    {
		    aBLeft = 8;
		    dataValue = (*dataPtr++);
		    }
		dataValue = (dataValue<<8) | (*dataPtr++);
		dataValue = (dataValue<<8) | (*dataPtr++);
		dataValue = (dataValue<<8) | (*dataPtr++);

		iBitsLeft = aBLeft + (KBytesToFetch-1) * 8;    

		iDataValue= dataValue;
		iDataPtr  = dataPtr;

		return iBitsLeft;
		}
		
	if (dataPtr == iDataPtrLimit) // no more data available
		{
		if (aBLeft) // there are some bits left so make use of them
			{
			return (iBitsLeft = aBLeft);
			}
		// no more data in buffer - signal that
		User::Leave(KErrCompletion); 
		}
		
	iDataValue = (dataValue<<8) | (*iDataPtr++);
	iBitsLeft  = aBLeft + 8;

	if ((iDataValue & 0xFF) == 0xFF)
		{
		if (iDataPtr == iDataPtrLimit)
			{
			// ----+ +-------
			//  |FF| |??|??|
			// ----+ +-------
			--iDataPtr;
			User::Leave(KErrCompletion);
			}

		// When the byte sequence 0xFF occurs in encoded image data it must be followed
		// by 0x00 or the decoder will consider it a marker. The 0x00 is ignored
		// by the decoder.
		
		// --------...
		//  |FF|??|
		// --------...
		TInt marker = *iDataPtr++;
		if (marker)//Since (FF,00) stream is valid we send it.For a Non-Zero marker we need to do something
			{
			// It's a proper marker
			if (iBitsLeft > 8) // there are stiil some bits to decode
				{
			// Before: iDataValue = [xxxxxxxx][xxxxxxxx][xxxxxxxx][ffffffff]
				iBitsLeft-=8;
				iDataValue >>=8;
				iDataPtr-=2;
			// After: iDataValue = [00000000][xxxxxxxx][xxxxxxxxx][xxxxxxxx]
				return iBitsLeft;
				}
			// End of encoded image data stream?
			if (marker == (KJpgEOISignature&0xFF)) //Allow 0xFF as a valid marker only report KErrEof in case of EOI marker
				{
				User::Leave(KErrEof);	// we've reached the end of the file
				}
			else
				{
				//If marker is a special marker like 0xFF we need to give a valid iDataValue..
				if(marker == KJpgMarkerByte)
					{
					while(iDataPtr<iDataPtrLimit && *iDataPtr == KJpgMarkerByte)
						{
						iDataPtr++;
						}
					
					if (iDataPtr==iDataPtrLimit)
						{
						--iDataPtr;
						User::Leave(KErrCompletion);
						}
					else
						{
						//Check for 0x00 marker since (0xFF,0x00) is a valid stream
						if(*iDataPtr==0x00)	
							{
							iDataPtr+=1;
							return iBitsLeft;
							}
						}
					}
				iDataPtr-=1; // we'd accept some malformed files, otherwise User::Leave(KErr)
				}
			}
		}
	return iBitsLeft;
	}

//
// Examine the buffer given by ProcessL for 0xFF sequences.
// This allows for less checking in the FillBitBufferL function.
//
inline const TUint8* CJpgReadCodec::JpegFillBuffer(const TUint8* aDataPtr)
	{
	register const TUint8* limit = Min(iDataPtrLimit - 1, aDataPtr + KMarkerLookAhead + 1);

	// Try to ignore markers that are at the start of the buffer, except for EOI.
	while ((aDataPtr < limit) && (*aDataPtr != 0xFF))
		{
		++aDataPtr;
		}

	return (iBitBufferPtrLimit = (aDataPtr - 1));
	}

FORCEDINLINE TBool CJpgReadCodec::NextBitL()
	{
	if (iBitsLeft == 0)
		{
		FillBitBufferL(0);
		}
	return (iDataValue & (1 << --iBitsLeft));
	}

void CJpgReadCodec::SetAutoRotateFlag(TUint16 aAutoRotateFlag)
	{
	iAutoRotateFlag = aAutoRotateFlag;
	}

void CJpgReadCodec::GetComponentBlockL(TDataUnit& aDestination,TInt& aNumValues,TInt& aDCPrediction,const TDecHuffmanTable& aDCTable,const TDecHuffmanTable& aACTable)
	{
	TInt size = GetHuffmanCodeL(aDCTable);
	TInt amplitude = (size > 0) ? GetBinaryNumberL( size & 0x1F ) : 0;
	aDCPrediction += amplitude;

	if (aDCPrediction > KMaxTInt16 || aDCPrediction < KMinTInt16)
		{
		JPEG_LEAVE(KErrCorrupt, "Bad component block");
		}
	
	if (iScalingFactor != -4) 
	    {
		FillCompZ(aDestination, KJpgDCTBlockSize);
		TInt16* valuePtr = aDestination.iCoeff;
		TInt16* valuePtrLimit = valuePtr + KJpgDCTBlockSize;

		*valuePtr++ = TInt16(aDCPrediction);

		while (valuePtr < valuePtrLimit)
			{
			TInt s = GetHuffmanCodeL(aACTable);
			if (s == 0) // End of block
				{
				break;
				}
			else
				{
				TInt r = s >> 4;
				s &= 0x0f;
				if (s > 0)
					{
					valuePtr += r;

					if (valuePtr < valuePtrLimit)
						{
    				    *valuePtr++ = GetBinaryNumberL(s);
						}
					}
				else if (r == 15) // Zero run length
					{
					valuePtr += 16;
					}
				}
			}

		if (valuePtr > valuePtrLimit)
    	    {
			valuePtr = valuePtrLimit;
    	    }

		aNumValues = valuePtr - aDestination.iCoeff;
	    }
	else    // for 1/8 scaling we need only DC value, so perform fast block skipping
	    {
	    aNumValues      = 1;
    	
    	aDestination.iCoeff[0] = TInt16(aDCPrediction);
    	TInt numValuesRead = 1; //we've already got DC value
    	do 
    		{
    		TInt s = GetHuffmanCodeL(aACTable);
    		if (s == 0) // End of block
    		    {
    		    break;
    		    }
    		else
    			{
    			TInt r = s >> 4;
    			s &= 0x0f;
    			if (s > 0)
    				{
    				numValuesRead += r;

    				if (numValuesRead < KJpgDCTBlockSize)
    				    {
    				    numValuesRead++;
    				    SkipBitsQuickL(s);
    				    }
    				}
    			else if (r == 15) // Zero run length
    				{
    				numValuesRead += 16;
    				}
    			}
    		} while (numValuesRead < KJpgDCTBlockSize);
	    }
	}

TInt CJpgReadCodec::GetHuffmanCodeL(const TDecHuffmanTable& aTable)
	{
	TInt bLeft = iBitsLeft;
	if (bLeft < KJpgHuffmanLookAhead)
		{
		bLeft = FillBitBufferL(bLeft);
		}

	TUint dv = iDataValue;

	TInt nb=1;
	//
	if (bLeft >= KJpgHuffmanLookAhead)
		{
		
		TUint32 fastLook = (dv >> (bLeft - KJpgHuffmanLookAhead)) & KJpgHuffmanLookAheadMask;
		register TUint32 lookupEntry = aTable.GetLookupEntry(fastLook);
		if (aTable.Found(lookupEntry))
			{
			iBitsLeft = (bLeft - aTable.GetSize(lookupEntry));
			return aTable.GetCode(lookupEntry);
			}
		else
			{
			nb = (KJpgHuffmanLookAhead+1 > bLeft)? bLeft : KJpgHuffmanLookAhead+1;
			}
		}

	register TUint index   = 0;
	TInt bitCount = 0;

	ASSERT(nb>0);
	register TUint32 look = dv << (32 - bLeft);
	iBitsLeft -= nb;
	do
		{
		index = (index << 1) + 1;
		index +=((look & ((TUint32)1<<31)) != 0);
		look<<=1;
		} while (++bitCount < nb);

	const TUint8* codeIdxHash = aTable.GetCodeIdxHash();
	
	for (; bitCount <= 16; bitCount++)
		{
      	TInt first    = codeIdxHash[bitCount];
      	TInt last    = codeIdxHash[bitCount+1];
		while (last >= first)
         	{
         	register TInt notFoundPosition = (first + last) >> 1;
         	TInt codeIndex = aTable.GetIndex(notFoundPosition);

			if (index < codeIndex)
				{
				last = notFoundPosition - 1;
				}
			else if (index > codeIndex)
				{
				first = notFoundPosition + 1;
				}
			else
				{
            	return aTable.GetIndexedCode(notFoundPosition);
            	}
         	}

		index = (index << 1) + 1;
		index += (NextBitL()!=0);
		}

#if !defined(RELAX_JPEG_STRICTNESS)
	User::Leave(KErrCorrupt);
#endif

	return aTable.GetIndexedCode(0);
	}

FORCEDINLINE void CJpgReadCodec::SkipBitsQuickL(TInt aNumOfBits)
    {
    TInt bLeft = iBitsLeft;
        
    FOREVER
        {
        bLeft   -=  aNumOfBits;
        if (bLeft >= 0)
            {
            iBitsLeft = bLeft;
            return;
            }
        aNumOfBits = -bLeft;
        iBitsLeft = 0;
        bLeft = FillBitBufferL(0);
        }
    }

FORCEDINLINE TInt CJpgReadCodec::GetBinaryNumberQuickL(TInt aLength)
	{
	register TInt bLeft  = iBitsLeft;
	register TUint number = 0;
	register TBitBuffer bitBuf;

	FOREVER
		{
		bitBuf = iDataValue;
		
		bLeft -= aLength;
		if (bLeft >= 0)
		    {
		    break;
		    }
		bLeft  += aLength;
		aLength -= bLeft;
	    number |= ((bitBuf & ((1<<bLeft)-1)) << aLength);
    
	    bLeft = FillBitBufferL(0);
		}
		
	iBitsLeft = bLeft;
	
	number |= ((bitBuf>>bLeft) & ((1<<aLength)-1));
	return number;	
	}

FORCEDINLINE TInt16 CJpgReadCodec::GetPositiveBinaryNumberL(TInt aLength)
	{
    return TInt16( GetBinaryNumberQuickL(aLength) );
	}

FORCEDINLINE TInt16 CJpgReadCodec::GetBinaryNumberL(TInt aLength)
    {
	TInt mask  = (-1) << (aLength - 1);
	TInt number = GetBinaryNumberQuickL(aLength);
	return TInt16( (number & mask)? number : number + ( mask<<1 ) + 1);
    }

/**
 	This class is to provide with "write pixel" functionality
 	for writting pixels into TRgb-type buffer
*/
class TRgbWriter
	{
public:
	inline
	static void WritePixel(TRgb* aPtr, TInt aY, TInt aU, TInt aV)
		{
		*aPtr = TYCbCr::YCbCrtoRGB(aY, aU, aV);
		}
	inline
	static TRgb* ShiftPtr(TRgb* aPtr, TInt aUnitsOffs)
		{
		return  aPtr + aUnitsOffs;
		}
	};

/**
 	This class is to provide with "write pixel" functionality
 	for writting pixels into EColor16M-type buffer i.e. 3 bytes per pixel
*/
class TRawWriter
	{
public:
	inline
	static void WritePixel(TRgb* aPtr, TInt aY, TInt aU, TInt aV)
		{
		TYCbCr::YCbCrtoRawRGB(aY, aU, aV, aPtr);
		}
	inline
	static TRgb* ShiftPtr(TRgb* aPtr, TInt aUnitsOffs)
		{
		return  reinterpret_cast<TRgb*>(reinterpret_cast<TUint8*>(aPtr) + (aUnitsOffs<<1) + aUnitsOffs);
		}
	};

/**
 	This class is to provide with "write pixel" functionality
 	for writting pixels into EColor16M-type buffer i.e. 3 bytes per pixel
 	It is similar to the TRawWriter but uses inline version of YUV->RGB
 	conversion function
*/
class TRawInlineWriter
	{
public:
	inline
	static void WritePixel(TRgb* aPtr, TInt aY, TInt aU, TInt aV)
		{
		TYCbCr::YCbCrtoRawRGBInl(aY, aU, aV, aPtr);
		}
	inline
	static TRgb* ShiftPtr(TRgb* aPtr, TInt aUnitsOffs)
		{
		return  reinterpret_cast<TRgb*>(reinterpret_cast<TUint8*>(aPtr) + (aUnitsOffs<<1) + aUnitsOffs);
		}
	};

/**
 	This class is to provide with "write pixel" functionality
 	for writting pixels into EColor64K-type buffer i.e. 2 bytes per pixel
*/
class TRaw64KColorWriter
	{
public:
	inline
	static void WritePixel(TRgb* aPtr, TInt aY, TInt aU, TInt aV)
		{
		TYCbCr::YCbCrtoRaw64K(aY, aU, aV, aPtr);
		}
	inline
	static TRgb* ShiftPtr(TRgb* aPtr, TInt aUnitsOffs)
		{
		return  reinterpret_cast<TRgb*>(reinterpret_cast<TUint8*>(aPtr) + (aUnitsOffs<<1) );
		}
	};

inline void CJpgReadCodec::WriteMCU()
	{
	JPEG_ASSERT(iMcuWriteFunc);
	(this->*iMcuWriteFunc)();
	}

//
// Calculate where in the intermediate buffer this MCU should be drawn.
//
TInt CJpgReadCodec::GetMCURenderOffset()
	{
	TInt mcuPos;

	JPEG_ASSERT(iMCUHorizExtent > 0);

	if (iFillBufferBackwards)
		{
		JPEG_ASSERT(!iProgressive);
		mcuPos = iMCUsPerBuffer - iHorzMCUBlkCount - 1;
		}
	else
		{
		// Fill from left to right.
		mcuPos = iHorzMCUBlkCount;
		}

	JPEG_ASSERT(mcuPos >= 0);

	// iMCUHorizExtent has already been scaled by CalculateRenderingParams().
	return (mcuPos * iMCUHorizExtent);

	}

//
// Writes a monochrome MCU.
//
void CJpgReadCodec::WriteMonoMCU()
	{
	const TInt16* yComp = iComponent[KYComp]->iCoeff;

	if (iScalingFactor == -4)
		{
		TInt pixelsToSkip = GetMCURenderOffset() + iFirstPixelOffset;
		TRgb* writeAddress = iRgbBuffer + pixelsToSkip;
		/* Coverity may flag as overrun of array yComp by indexing, which is false positive. There is more than one TDataUnit object pointed to by 
		iComponent[KYComp], which Coverity may fail to take into account */
		for (TInt j = 0; j < iMaxVertSampleFactor; j++)
			{
			for (TInt i = 0; i < iMaxHorzSampleFactor; i++)
				{
				*writeAddress = TRgb::Gray256(ColorCcomponent::ClampColorComponent(yComp[0]));
				writeAddress += iPixelIncrement;
				yComp += KJpgDCTBlockSize;
				}
			writeAddress += iRgbBufNextLineOffs;
			}
		}
	else
		{
		const TInt KYBlockOffset	= KJpgDCTBlockWidth-(iMaxHorzSampleFactor * KJpgDCTBlockSize);

		TInt pixelsToSkip = GetMCURenderOffset() + iFirstPixelOffset;
		TRgb* writeAddress = iRgbBuffer + pixelsToSkip;

		for (TInt sf = iMaxVertSampleFactor * KJpgDCTBlockWidth; sf;)
			{
			TInt hsf = iMaxHorzSampleFactor;
			do
				{
				TInt i = KJpgDCTBlockWidth / 2;
				do
					{
					*writeAddress = TRgb::Gray256(ColorCcomponent::ClampColorComponent(*yComp++));
					writeAddress += iPixelIncrement;
					*writeAddress = TRgb::Gray256(ColorCcomponent::ClampColorComponent(*yComp++));
					writeAddress += iPixelIncrement;
					}
				while (--i);

				yComp += (KJpgDCTBlockSize - KJpgDCTBlockWidth);
				}
			while (--hsf);

			--sf;
			yComp 		+= (sf & (KJpgDCTBlockWidth-1))? KYBlockOffset: -(KJpgDCTBlockSize-KJpgDCTBlockWidth);
			writeAddress += iRgbBufNextLineOffs;
			}
		}
	}

void CJpgReadCodec::WriteDiv8ScaledMCU16M()
	{
	WriteDiv8MCUImpl<TRawWriter>();
	}

void CJpgReadCodec::WriteDiv8ScaledMCU()
	{
	WriteDiv8MCUImpl<TRgbWriter>();
	}

void CJpgReadCodec::WriteDiv8ScaledMCU64K()
	{
	WriteDiv8MCUImpl<TRaw64KColorWriter>();
	}

template <class T>
inline void CJpgReadCodec::WriteDiv8MCUImpl()
	{
	ASSERT(iScalingFactor == -4);

	TInt16* yComp = iComponent[0]->iCoeff;

	TUVidxElemType* uVIndeces = iUVIndeces;

	TInt pixelsToSkip = GetMCURenderOffset() + iFirstPixelOffset;
	TRgb* writeAddress = T::ShiftPtr(iRgbBuffer, pixelsToSkip);

	const TInt16* uComp = iComponent[KUComp]->iCoeff;
	const TInt16* vComp = iComponent[KVComp]->iCoeff;

	TInt hsf = iMaxHorzSampleFactor;
	TInt vsf = iMaxVertSampleFactor;
	/* Coverity may flag as overrun of array on indexing yComp. This is false positive. Coverity doesn't take into account that 
	iComponent[0] can point to more than one TDataUnit.
	*/
	do
		{
		do
			{
			const TInt16 uValue = uComp[*uVIndeces];
			uVIndeces++;
			const TInt16 vValue = vComp[*uVIndeces];
			uVIndeces++;
			T::WritePixel(writeAddress, *yComp, uValue, vValue);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);

			yComp += KJpgDCTBlockSize;
			}
		while (--hsf);

		hsf = iMaxHorzSampleFactor;
		uVIndeces += (hsf << 4); // 2 * hsf * KJpgBlockWidth
		writeAddress = T::ShiftPtr(writeAddress, iRgbBufNextLineOffs);
		}
	while (--vsf);
	}

void CJpgReadCodec::WriteUnScaledMCU16M()
	{
	WriteUnScaledMCUImpl<TRawWriter>();
	}

void CJpgReadCodec::WriteUnScaledMCU()
	{
	WriteUnScaledMCUImpl<TRgbWriter>();
	}

template <class T>
inline void CJpgReadCodec::WriteUnScaledMCUImpl()
	{
	ASSERT((iScalingFactor == 1) || (iScalingFactor == -1));

	TInt pixelsToSkip = GetMCURenderOffset() + iFirstPixelOffset;
	TRgb* writeAddress = T::ShiftPtr(iRgbBuffer, pixelsToSkip);

	const TInt KYBlockOffset = KJpgDCTBlockWidth - (iFrameInfo.MCUWidthInPixels() * 8);

	register const TInt ush = iHorzSampleRatioSh[KUShiftIdx];
	register const TInt vsh = iHorzSampleRatioSh[KVShiftIdx];
	const TInt16* yComp = iComponent[KYComp]->iCoeff;
	const TUVidxElemType* pixIdx = iUVIndeces;

	TInt sf = iFrameInfo.MCUHeightInPixels();
	do
		{
		TInt hsf = iMaxHorzSampleFactor;
		do
			{
			const TInt16* const u_base = iComponent[KUComp]->iCoeff + *pixIdx++;
			const TInt16* const v_base = iComponent[KVComp]->iCoeff + *pixIdx++;

#if defined(JPEG_OPTIMIZE_FOR_PERFORMCE)

			T::WritePixel(writeAddress, *yComp++, u_base[0], v_base[0]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);

			register TInt p=1;

			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			++p;
			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			++p;
			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			++p;

			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			++p;
			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			++p;
			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			++p;
			T::WritePixel(writeAddress, *yComp++, u_base [(p>>ush)], v_base [(p>>vsh)]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
#else

			register TInt p = 0;
			do
				{
				TInt y = *yComp++;
				TInt u = u_base[(p >> ush)];
				TInt v = v_base[(p >> vsh)];

				T::WritePixel(writeAddress, y, u, v);
				writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
				}
			while (++p < 8);
#endif
			yComp += (KJpgDCTBlockSize - KJpgDCTBlockWidth);
			}
		while (--hsf);

		--sf;
		yComp += (sf & (KJpgDCTBlockWidth - 1)) ? KYBlockOffset : -(KJpgDCTBlockSize - KJpgDCTBlockWidth);
		writeAddress = T::ShiftPtr(writeAddress, iRgbBufNextLineOffs);
		}
	while (sf);
	}

void CJpgReadCodec::WriteDiv2ScaledMCU()
	{
	WriteDiv2ScaledMCUImpl<TRgbWriter>();
	}

void CJpgReadCodec::WriteDiv2ScaledMCU16M()
	{
	WriteDiv2ScaledMCUImpl<TRawWriter>();
	}

template <class T>
inline void CJpgReadCodec::WriteDiv2ScaledMCUImpl()
	{
	ASSERT(iScalingFactor == -2);

	const TInt KScalingFactor = 2;
	const TInt KYBlockOffset = KJpgDCTBlockWidth -
		(iMaxHorzSampleFactor * KJpgDCTBlockSize) +
		(KScalingFactor - 1) * KJpgDCTBlockWidth;

	TInt pixelsToSkip = GetMCURenderOffset() + iFirstPixelOffset;
	TRgb* writeAddress = T::ShiftPtr(iRgbBuffer, pixelsToSkip);

	const TInt16* yComp = iComponent[KYComp]->iCoeff;
	const TUVidxElemType* pixIdx = iUVIndeces;
	register const TInt ush = iHorzSampleRatioSh[KUShiftIdx];
	register const TInt vsh = iHorzSampleRatioSh[KVShiftIdx];

	TInt sf = iMaxVertSampleFactor * KJpgDCTBlockWidth;
	/* Coverity may flag as overrun of array by accessing yComp. This is false positive. Coverity doesn't take into account that 
	iComponent[KYComp] can point to more than one TDataUnit.
	*/
	do
		{
		TInt hsf = iMaxHorzSampleFactor;
		do
			{
			const TInt16* const u_base = iComponent[KUComp]->iCoeff + *pixIdx++;
			const TInt16* const v_base = iComponent[KVComp]->iCoeff + *pixIdx++;

			T::WritePixel(writeAddress, *yComp, u_base[0], v_base[0]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			yComp += KScalingFactor;

			T::WritePixel(writeAddress, *yComp, u_base[KScalingFactor >> ush], v_base[KScalingFactor >> vsh]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			yComp += KScalingFactor;

			T::WritePixel(writeAddress, *yComp, u_base[2 * KScalingFactor >> ush], v_base[2 * KScalingFactor >> vsh]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			yComp += KScalingFactor;

			T::WritePixel(writeAddress, *yComp, u_base[3 * KScalingFactor >> ush], v_base[3 * KScalingFactor >> vsh]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			yComp += KScalingFactor + (KJpgDCTBlockSize - KJpgDCTBlockWidth);
			}
		while (--hsf);

		sf -= KScalingFactor;
		yComp += (sf & (KJpgDCTBlockWidth-1))? KYBlockOffset: KScalingFactor*KJpgDCTBlockWidth-KJpgDCTBlockSize;
		writeAddress = T::ShiftPtr(writeAddress, iRgbBufNextLineOffs);
		}
	while (sf);
	}

void CJpgReadCodec::WriteDiv4ScaledMCU()
	{
	WriteDiv4ScaledMCUImpl<TRgbWriter>();
	}

void CJpgReadCodec::WriteDiv4ScaledMCU16M()
	{
	WriteDiv4ScaledMCUImpl<TRawInlineWriter>();
	}

template <class T>
inline void CJpgReadCodec::WriteDiv4ScaledMCUImpl()
	{
	ASSERT(iScalingFactor == -3);

	const TInt KScalingFactor = 4;
	const TInt KYBlockOffset = KJpgDCTBlockWidth -
		(iMaxHorzSampleFactor * KJpgDCTBlockSize) +
		(KScalingFactor - 1) * KJpgDCTBlockWidth;

	TInt pixelsToSkip = GetMCURenderOffset() + iFirstPixelOffset;
	TRgb* writeAddress = T::ShiftPtr(iRgbBuffer, pixelsToSkip);

	const TInt16* yComp = iComponent[KYComp]->iCoeff;
	const TUVidxElemType* pixIdx=iUVIndeces;

	TInt sf = iMaxVertSampleFactor * KJpgDCTBlockWidth;
	/* Coverity may flag as overrun of array on accessing yComp. This is false positive. Coverity doesn't take into account that 
	iComponent[KYComp] can point to more than one TDataUnit.
	*/
	do
		{
		TInt hsf = iMaxHorzSampleFactor;
		do
			{
			const TInt16* const u_base1 = iComponent[KUComp]->iCoeff + *pixIdx;
			pixIdx++;
			const TInt16* const v_base1 = iComponent[KVComp]->iCoeff + *pixIdx;
			pixIdx++;

			T::WritePixel(writeAddress, *yComp, u_base1[0], v_base1[0]);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			yComp += KScalingFactor;

			const TInt16 u_base2 = u_base1[*pixIdx];
			pixIdx++;
			const TInt16 v_base2 = v_base1[*pixIdx];
			pixIdx++;

			T::WritePixel(writeAddress, *yComp, u_base2, v_base2);
			writeAddress = T::ShiftPtr(writeAddress, iPixelIncrement);
			yComp += KScalingFactor + (KJpgDCTBlockSize - KJpgDCTBlockWidth);
			}
		while (--hsf);

		sf -= KScalingFactor;
		yComp += (sf & (KJpgDCTBlockWidth-1))? KYBlockOffset: KScalingFactor*KJpgDCTBlockWidth-KJpgDCTBlockSize;
		writeAddress = T::ShiftPtr(writeAddress, iRgbBufNextLineOffs);
		}
	while (sf);
	}


TInt CJpgReadCodec::ComponentIndexL(TInt aComponentId) const
	{
	for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
		{
		if (iFrameInfo.iComponent[count].iId == aComponentId)
			return count;
		}

	User::Leave(KErrCorrupt);
	return 0;
	}

void CJpgReadCodec::SetYuvDecode(TBool aYuvDecode)
	{
	iYuvDecode = aYuvDecode;
	}

void CJpgReadCodec::SetHighSpeedMode(TBool aHighSpeedMode)
	{
	iHighSpeedMode = aHighSpeedMode;
	}

TInt CJpgReadCodec::RecommendBufferSizeL(TUid aFormatCode)
	{
	return CJpgImageFrameProcessorUtility::RecommendedBufferSizeL(iFrameInfo, aFormatCode);
	}

void CJpgReadCodec::InitDrawFrame()
	{//default implementation do nothing
	}

TBool CJpgReadCodec::DrawFrameL()
	{//default implementation do nothing
	return ETrue;
	}

void CJpgReadCodec::CleanupBuffers()
	{//default implementation do nothing
	}

void CJpgReadCodec::InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameData*/)
	{
	aFrameInfo.SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
	}


TInt CJpgReadCodec::MCUBlockPerRgbBuffer() const
	{
	return iMCUsPerBuffer;
	}

TInt CJpgReadCodec::GetHorzMCUCount()
	{
	TInt maxMCUWidth = KJpgDCTBlockWidth * iMaxHorzSampleFactor;
	return (iFrameInfo.iSizeInPixels.iWidth + maxMCUWidth - 1) / maxMCUWidth;
	}

TInt CJpgReadCodec::GetVertMCUCount()
	{
	TInt maxMCUHeight = KJpgDCTBlockWidth * iMaxVertSampleFactor;
	return (iFrameInfo.iSizeInPixels.iHeight + maxMCUHeight - 1) / maxMCUHeight;
	}

//
// aExtensionManager is not owned.
//
void CJpgReadCodec::SetExtensionManager(CPluginExtensionManager* aExtensionManager)
	{
	iExtensionManager = aExtensionManager;
	}

#if defined(__ARMCC__)
#pragma pop
#endif

#if defined(__ARMCC__)
#pragma push
#pragma thumb
#endif
//
// CSequentialJpgReadCodec
CSequentialJpgReadCodec::CSequentialJpgReadCodec(
	const TJpgFrameInfo& aFrameInfo,
	const TJpgScanInfo& aScanInfo,
	TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
	TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
	const TQTable aQTable[KJpgMaxNumberOfTables])
 :	CJpgReadCodec(aFrameInfo, aScanInfo),
	iDCHuffmanTable(aDCHuffmanTable),
	iACHuffmanTable(aACHuffmanTable),
	iQTable(aQTable)
	{
	iProgressive = EFalse;
	}

//
//
//
CSequentialJpgReadCodec::~CSequentialJpgReadCodec()
	{
	delete iMCUStore;
	iMCUStore = NULL;

	User::Free(iMCULookup);
	iMCULookup = NULL;
	}

//
//
//
CSequentialJpgReadCodec* CSequentialJpgReadCodec::NewL(
	const TJpgFrameInfo& aFrameInfo,
	const TJpgScanInfo& aScanInfo,
	TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
	TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
	const TQTable aQTable[KJpgMaxNumberOfTables])
	{
	CSequentialJpgReadCodec* self = new(ELeave) CSequentialJpgReadCodec(
			aFrameInfo,
			aScanInfo,
			aDCHuffmanTable,
			aACHuffmanTable,
			aQTable);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

//
//
//
void CSequentialJpgReadCodec::ConstructL(TBool aUseCache)
	{
	CJpgReadCodec::ConstructL();

	ASSERT(iMCULookup == NULL);

	iTotalMCUCount = iFrameInfo.TotalMCUCount();

	// Make sure iFrameInfo has had its members set.
	JPEG_ASSERT(iTotalMCUCount >= 1);

	iMCUStore = CMCUStore::NewL(iFrameInfo);

	// If the allocation of the lookup table fails, continue
	// to decode the image as it's not needed for some decodes
	// and others should be able to continue without it (with a
	// performance hit).
	if (aUseCache)
		{
		JPEG_DEBUG2(" - Cache for %d elements", iTotalMCUCount);
		TInt allocSize = iTotalMCUCount * sizeof(TMCUEntry);
		iMCULookup = reinterpret_cast<TMCUEntry*>(User::AllocZ(allocSize));
		}
	}

//
// Nothing is guaranteed to be known about the image structure at this point.
//
void CSequentialJpgReadCodec::PreInitFrameL()
	{
	CJpgReadCodec::PreInitFrameL();
	iMCUStore->Reset();
	}


//
// This function is called after the various variants of
// InitFrameL have been called. At this stage enough should
// be known about the image to set several properties.
//
void CSequentialJpgReadCodec::PostInitFrameL()
	{
	iHorzMCUBlkCount = 0;
	iStreamMCU = 0;
	iMCUStore->SetMCUsPerBuffer(iMCUsPerBuffer);
	iMCUStore->SetOperation(iOperation);
	iNeededMCU = iMCUStore->GetNextMCU();

	// Make sure iScanInfo isn't used before its members have been set.
	JPEG_ASSERT(iScanInfo.iImageOffset > 0);

	iCurrentMCUBitOffset = (iScanInfo.iImageOffset * 8);
	
	CJpgReadCodec::PostInitFrameL();
	}

//
// This is called by the framework whenever DoProcessL returns EFrameIncompleteRepositionRequest.
// After this function returns the framework should call DoProcessL again.
//
void CSequentialJpgReadCodec::GetNewDataPosition(TInt& aPosition, TInt& /*aLength*/)
	{
	if (!iMCULookup)
		{
		// Seek to the start of the image data.
		aPosition = iScanInfo.iImageOffset;
		iSeekDone = ETrue;
		return;
		}

	if ((iNeededMCU >= 0) && (iNeededMCU < iTotalMCUCount))
		{
		TMCUEntry& entry = iMCULookup[iNeededMCU];

		aPosition = (entry.iPosition >> 3);
		iSeekDone = ETrue;
		return;
		}

	// The seek shouldn't have been made if iNeededMCU is outside
	// the bounds of the lookup table.
	ASSERT(EFalse);
	}

#if defined(__ARMCC__)
#pragma pop
#endif

#if defined(__ARMCC__)
// use ARM instruction for performance-critical code
#pragma push
#pragma arm 
#pragma O3 
#pragma Otime
#endif
//
// This will be called by CJpgReadCodec once enough
// data is available to do correct calculations.
//
void CSequentialJpgReadCodec::CalculateMCUBoundingRectL(TInt aMCUsPerLine)
	{
	JPEG_DEBUG1("CalculateMCUBoundingRectL()");

	ASSERT(iExtensionManager);

	JPEG_ASSERT(!iIsBlockStreaming);
	JPEG_ASSERT(!iProgressive);
	JPEG_ASSERT(aMCUsPerLine > 0);

	TRect clipRect;
	clipRect = iExtensionManager->ClippingRect();

	// This function shouldn't be called by InitFrameL
	// if no clipping rect has been set.
	JPEG_ASSERT(!clipRect.IsEmpty());

	TInt mcuWidthInPixels = iFrameInfo.MCUWidthInPixels();
	TInt mcuHeightInPixels = iFrameInfo.MCUHeightInPixels();
	JPEG_DEBUG2(" - MCU pixel width: %d", mcuWidthInPixels);
	JPEG_DEBUG2(" - MCU pixel height: %d", mcuHeightInPixels);

	// The clipping rect is specified in pixels. We need to
	// find out which MCUs contain these pixels.
	TInt left = clipRect.iTl.iX / mcuWidthInPixels;
	TInt top = clipRect.iTl.iY / mcuHeightInPixels;
	TInt right = (clipRect.iBr.iX - 1) / mcuWidthInPixels;
	TInt bottom = (clipRect.iBr.iY - 1) / mcuHeightInPixels;

	TInt firstMCU = (top * aMCUsPerLine) + left;

	iMCUClipRect.SetRect(left, top, right + 1, bottom + 1);

	iMCUStore->SetClippingRect(firstMCU, iMCUClipRect.Width() * iMCUClipRect.Height());

	// Convert back to pixels.
	left *= mcuWidthInPixels;
	top *= mcuHeightInPixels;
	right = (right * mcuWidthInPixels) + mcuWidthInPixels;
	bottom = (bottom * mcuHeightInPixels) + mcuHeightInPixels;

	iMCUClipRect.SetRect(left, top, right, bottom);
	JPEG_DEBUG5("iMCUClipRect: (%d, %d) - (%d x %d)",
			iMCUClipRect.iTl.iX,
			iMCUClipRect.iTl.iY,
			iMCUClipRect.Width(),
			iMCUClipRect.Height());
	}

//
//
//
void CSequentialJpgReadCodec::CacheMCULocation()
	{
	JPEG_ASSERT(iStreamMCU >= 0);
	JPEG_ASSERT(!iFrameInfo.iMultiScan);

	if (!iMCULookup || (iStreamMCU >= iTotalMCUCount))
		{
		return;
		}

	TMCUEntry& entry = iMCULookup[iStreamMCU];

	if (entry.iPosition != 0)
		{
		for (TInt i = 0; i < 3; i++)
			{
			JPEG_ASSERT(entry.iDCPredictor[i] == iDCPredictor[i]);
			}
		}
	else
		{
		entry.iPosition = iCurrentMCUBitOffset;
		if (iEscapeAtEnd)
			{
			entry.iPosition -= 8;	// Go back a byte.
			}

		entry.iDCPredictor[0] = iDCPredictor[0];
		entry.iDCPredictor[1] = iDCPredictor[1];
		entry.iDCPredictor[2] = iDCPredictor[2];
		entry.iRestartMCUCount = iRestartMCUCount;
		}
	}

//
//
//
#ifdef JPEG_DEBUG_OUTPUT
void CSequentialJpgReadCodec::DumpCache()
	{
	JPEG_DEBUG1("CACHE DUMP");
	
	for (TInt i = 0; i < iTotalMCUCount; i++)
		{
		TMCUEntry& entry = iMCULookup[i];
		if (entry.iPosition == 0)
			{
			return;
			}
		
		JPEG_DEBUG7("Entry[%6d] location=%8d; predictors[%4d, %4d, %4d] restart=%d",
				i,
				entry.iPosition,
				entry.iDCPredictor[0],
				entry.iDCPredictor[1],
				entry.iDCPredictor[2],
				entry.iRestartMCUCount);
		}
	}
#endif

//
// This is called after a seek has been performed.
// It sets everything up so that we're decoding from
// the correct position in the bitstream and does
// some other housekeeping.
//
void CSequentialJpgReadCodec::RestoreAfterSeekL()
	{
	// Reset the bitstream.
	TInt bitOffset = 0;
	iBitsLeft = 0;
	iBitBufferPtrLimit = 0;
	iDataValue = 0;

	if (iMCULookup)
		{
		TMCUEntry& entry = iMCULookup[iNeededMCU];

		// Divide entry.iPosition into byte and bit offsets.
		bitOffset = (entry.iPosition & 0x07);

		// Make sure re-caching will work.
		iCurrentMCUBitOffset = entry.iPosition;
		iDCPredictor[0] = entry.iDCPredictor[0];
		iDCPredictor[1] = entry.iDCPredictor[1];
		iDCPredictor[2] = entry.iDCPredictor[2];
		iRestartMCUCount = entry.iRestartMCUCount;

		iStreamMCU = iNeededMCU;
		}
	else
		{
		// The seek was to the start of the image.
		iRestartMCUCount = iFrameInfo.iRestartInterval;
		iDCPredictor[0] = 0;
		iDCPredictor[1] = 0;
		iDCPredictor[2] = 0;
		iStreamMCU = 0;
		iCurrentMCUBitOffset = iScanInfo.iImageOffset * 8;
		}

	//FetchNext3BytesL();
	FillBitBufferL(iBitsLeft);
	iBitsLeft -= bitOffset;
	iSeekDone = EFalse;
	}


//
// Check's if the location of iNeededMCU is known in advance.
//
TBool CSequentialJpgReadCodec::QueryCache()
	{
	// Prevent an infinite seeking loop.
	if (iNeededMCU == iStreamMCU)
		{
		return EFalse;
		}

	if (!iMCULookup)
		{
		if (iNeededMCU < iStreamMCU)
			{
			// We can seek to the start of the image and start
			// decoding again from there.
			iDataPtr++;
			return ETrue;
			}

		return EFalse;
		}

	if ((iNeededMCU < 0) || (iNeededMCU >= iTotalMCUCount))
		{
		// Out of bounds.
		return EFalse;
		}

	TMCUEntry& entry = iMCULookup[iNeededMCU];
	if (entry.iPosition != 0)
		{
		// This is in order to get the framework to do the seek.
		iDataPtr++;
		return ETrue;
		}

	return EFalse;
	}


#if defined(__ARMCC__)
#pragma pop 
#endif

//
//
//
TFrameState CSequentialJpgReadCodec::DoProcessL()
	{
	if (iSeekDone)
		{
		RestoreAfterSeekL();
		}

	while (iDataPtr < iDataPtrLimit)
		{
		if (iNeededMCU > iTotalMCUCount)
			{
			JPEG_LEAVE(KErrOverflow, "iNeededMCU is out of bounds");
			}

		// See if we're done.
		if (iNeededMCU == KErrCompletion)
			{
			return EFrameComplete;
			}
		else if (QueryCache())
			{
			return EFrameIncompleteRepositionRequest;
			}

		StoreState();
		if (iRestartMCUCount == 0)
			{
			TInt skipped = RestartStateL();
			iCurrentMCUBitOffset += (skipped * 8);
			}

		CacheMCULocation();

		TInt error = KErrNone;
		TInt mcuBitSize = 0;
		// we do that "if" in order to bypass exception handling which can
		// affect performance of the decoder
		if (iPreviousDataLeft < KMCUDataLeftThreshhold)
			{
			const TUint8* const latestDataPtr = iDataPtr;
			TRAP(error, mcuBitSize = ProcessMCUL());

			// leave if it wasn't a partial MCU
			if ((error != KErrNone) &&
				(error != KErrEof ||
				latestDataPtr == iDataPtr ||
				(latestDataPtr + sizeof(TUint16) <= iDataPtrLimit &&
				PtrReadUtil::ReadBigEndianUint16(latestDataPtr) == KJpgEOISignature)))
				{
				User::Leave(error);
				}
			}
		else
			{
			mcuBitSize = ProcessMCUL();
			}

		iCurrentMCUBitOffset += mcuBitSize;

		// we would try to render the partially decoded MCU
		// in case if there is a partial MCU/incompelete image
		// and do leave with original error code later
		if (iStreamMCU == iNeededMCU)
			{
			if (iIsBlockStreaming &&
					(iNavigation == TDecodeStreamCaps::ENavigationRandomForward ||
					iNavigation == TDecodeStreamCaps::ENavigationRandomBackwards) &&
					iStreamMCU < iStreamDecodeConfig.iSeqPosition)
				{
				iNeededMCU = iMCUStore->GetNextMCU();
				}
			else
				{
				PostProcessMCUL(error != KErrNone);

				User::LeaveIfError(error);

				if (iIsBlockStreaming)
					{
					iNeededMCU++;
					if (EBlockComplete == ProcessStreaming())
						{
						iStreamMCU++;
						iRestartMCUCount--;
						return EBlockComplete;
						}
					}
				else
					{
					iNeededMCU = iMCUStore->GetNextMCU();
					}
				}
			}

		iStreamMCU++;
		iRestartMCUCount--;

		TInt dataLeft = iDataPtrLimit - iDataPtr;
		if (dataLeft < KMCUDataLeftThreshhold)
			{
			TBool needLeave = (iPreviousDataLeft > dataLeft);
			iPreviousDataLeft = dataLeft;
			if (needLeave)
				{
				StoreState();
				User::Leave(KErrCompletion);
				}
			}
		}

	return EFrameIncomplete;
	}

TFrameState CSequentialJpgReadCodec::ProcessStreaming()
	{
	if(iNavigation == TDecodeStreamCaps::ENavigationSequentialForward)
		{
		if(iTotalMCUCount > iNeededMCU)
			{
			*(iStreamDecodeConfig.iHaveMoreBlocks) = ETrue;
			}
		else
			{
			*(iStreamDecodeConfig.iHaveMoreBlocks) = EFalse;
			}
		}

	*(iStreamDecodeConfig.iNumBlocksRead) += 1;

	if(*(iStreamDecodeConfig.iNumBlocksRead) >= iStreamDecodeConfig.iNumBlocksToGet)
		{
		return EBlockComplete;
		}
	else
		{
		return EFrameIncomplete;
		}
	}

//
// This functions turns the MCU data into pixel data.
// The pixel data is written into an intermediate buffer,
// iRgbBuffer, by WriteMCU() and then if this intermediate
// buffer is full this is copied to the output bitmap by
// SetPixelBlock().
//
void CSequentialJpgReadCodec::PostProcessMCUL(TBool aForceBufferFlush)
	{
	if (!iImageFrameCodecPtr)
		{
		TBool copyIt = aForceBufferFlush;

		WriteMCU();
		iHorzMCUBlkCount++;

		// Only copy the buffer if we have rendered a row of MCUs or are forced to.
		copyIt |= (iHorzMCUBlkCount == iMCUsPerBuffer);
		if (copyIt)
			{
			CImageProcessor* proc = ImageProcessor();
			ASSERT(proc != NULL);
			proc->SetPixelBlock(iRgbBuffer);
			iMCUStore->NextLine();
			iHorzMCUBlkCount = 0;
			}
		}
	else
		{
		RArray<const TDataUnit*> dataUnits;
		CleanupClosePushL(dataUnits);
		for(TInt i = 0; i < iFrameInfo.iNumberOfComponents; i++)
			{
			TDataUnit* compPtr = iComponent[i];
			TInt numSamples = iMCUDataUnitCount[i];
			while (numSamples > 0)
				{
				numSamples--;
				User::LeaveIfError(dataUnits.Append(compPtr++));
				}
			}
		iImageFrameCodecPtr->ProcessL(dataUnits);
		CleanupStack::PopAndDestroy(1, &dataUnits);
		}
	}


TInt CSequentialJpgReadCodec::ProcessMCUL()
	{
	TDataUnit temp;
	TInt numValues;
	TInt bitsBefore = iBitsLeft;
	const TUint8* startPtr = iDataPtr;
	
	iEscapeAtEnd = EFalse;
	for (TInt i = 0; i < iScanInfo.iNumberOfComponents; i++)
		{
		TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[i];
		TInt compIndex = ComponentIndexL(scanInfo.iId);
		TJpgFrameInfo::TComponentInfo& compInfo = iFrameInfo.iComponent[compIndex];
		const TDecHuffmanTable& dcTable = iDCHuffmanTable[scanInfo.iDCCodingTable];
		const TDecHuffmanTable& acTable = iACHuffmanTable[scanInfo.iACCodingTable];
		const TQTable& qTable = iQTable[compInfo.iQTable];
		TDataUnit* compPtr = iComponent[compIndex];
		if (compPtr==NULL)
			{
			compPtr = &temp; // we'd throw it away in case of Mono mode
			}
		TInt numSamples = iMCUDataUnitCount[compIndex];

		while (numSamples > 0)
			{
			GetComponentBlockL(*compPtr, numValues, iDCPredictor[compIndex], dcTable, acTable);
			if (compIndex == 0 || !iMonochrome)
				{
				(qTable.*iCompConf[compIndex].iDequantFunc)(temp, *compPtr, numValues);
				iCompConf[compIndex].iDCT->InverseTransform(*compPtr, temp);
				compPtr++;
				}
			numSamples--;
			}
		}
	
	if (iRestartMCUCount == 1)
		{
		iBitsLeft = 0;
		}
	else
		{
		iEscapeAtEnd = ((iDataValue & 0xFF) == 0xFF);
		}

	TInt bytesRead = iDataPtr - startPtr;
	return bitsBefore + (bytesRead * 8) - iBitsLeft;
	}


#if defined(__ARMCC__)
// use ARM instruction for performance-critical code
#pragma push
#pragma thumb 
#endif

// CProgressiveJpgReadCodec
CProgressiveJpgReadCodec::CProgressiveJpgReadCodec(const TJpgFrameInfo& aFrameInfo,const TJpgScanInfo& aScanInfo,const TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],const TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],const TQTable aQTable[KJpgMaxNumberOfTables])
:	CJpgReadCodec(aFrameInfo,aScanInfo),
	iOriginalFrameInfo(aFrameInfo),
	iOriginalScanInfo(aScanInfo)
	{
	iProgressive = ETrue;
	Mem::Copy(iDCHuffmanTable,aDCHuffmanTable,sizeof(TDecHuffmanTable) * KJpgMaxNumberOfTables);
	Mem::Copy(iACHuffmanTable,aACHuffmanTable,sizeof(TDecHuffmanTable) * KJpgMaxNumberOfTables);
	Mem::Copy(iQTable,aQTable,sizeof(TQTable) * KJpgMaxNumberOfTables);
	}

CProgressiveJpgReadCodec* CProgressiveJpgReadCodec::NewL(const TJpgFrameInfo& aFrameInfo,const TJpgScanInfo& aScanInfo,const TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],const TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],const TQTable aQTable[KJpgMaxNumberOfTables])
	{
	CProgressiveJpgReadCodec* self = new(ELeave) CProgressiveJpgReadCodec(aFrameInfo, aScanInfo, aDCHuffmanTable, aACHuffmanTable, aQTable);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CProgressiveJpgReadCodec::~CProgressiveJpgReadCodec()
	{
	CleanupBuffers();
	}

void CProgressiveJpgReadCodec::DoInitFrameL()
	{
	iFrameInfo = iOriginalFrameInfo;
	iScanInfo = iOriginalScanInfo;

	TInt maxMCUWidth = KJpgDCTBlockWidth * iMaxHorzSampleFactor;
	TInt maxMCUHeight = KJpgDCTBlockWidth * iMaxVertSampleFactor;
	iHorzMCUCount = (iFrameInfo.iSizeInPixels.iWidth + maxMCUWidth - 1) / maxMCUWidth;
	iVertMCUCount = (iFrameInfo.iSizeInPixels.iHeight + maxMCUHeight - 1) / maxMCUHeight;
	iTotalMCUCount = iHorzMCUCount * iVertMCUCount;
	iCurrentMCUCount = 0;
	iCurrentMCUHorzCount = 0;
	iCurrentMCUVertCount = 0;
	iMCUChunkAllocated = EFalse;

	// We calculate how many data units we'll need in total
	TInt totalDataUnitCount = 0;
	for (TInt compIndex = 0; compIndex < iFrameInfo.iNumberOfComponents; compIndex++)
		{
		totalDataUnitCount += iMCUDataUnitCount[compIndex] * iTotalMCUCount;
		}

	TUint8* offset = NULL;
	if(totalDataUnitCount > KMCUMaxTotalDataUnits)
		{
 		iMCUChunk.Close();
		User::LeaveIfError(iMCUChunk.CreateLocal(totalDataUnitCount * sizeof(TDataUnit), totalDataUnitCount * sizeof(TDataUnit)));
		offset = iMCUChunk.Base();
		iMCUChunkAllocated = ETrue;
		}
    else
        {
        delete iMCUMemoryBuffer;
        iMCUMemoryBuffer = NULL;
        iMCUMemoryBuffer = new (ELeave) TUint8 [ totalDataUnitCount * sizeof(TDataUnit) ];
        offset = iMCUMemoryBuffer;
        }


	for (TInt compIndex = 0; compIndex < iFrameInfo.iNumberOfComponents; compIndex++)
		{
		TInt dataUnitCount = iMCUDataUnitCount[compIndex] * iTotalMCUCount;

		iMCUBuffer[compIndex] = new(offset) TDataUnit[dataUnitCount];
		offset += dataUnitCount * sizeof(TDataUnit);

		Mem::FillZ(iMCUBuffer[compIndex],dataUnitCount * sizeof(TDataUnit));
		iMCUBufferPtr[compIndex] = iMCUBuffer[compIndex];
		iMCUBufferPtrLimit[compIndex] = iMCUBuffer[compIndex] + dataUnitCount;

		iIndividualHorzMCUCount[compIndex] = ((iFrameInfo.iSizeInPixels.iWidth*iHorzSampleFactor[compIndex]) + maxMCUWidth - 1) / maxMCUWidth;
		iIndividualVertMCUCount[compIndex] = ((iFrameInfo.iSizeInPixels.iHeight*iVertSampleFactor[compIndex]) + maxMCUHeight - 1) / maxMCUHeight;
		}

	iProcessing = ETrue;
	iRefinedDCValue = 0;
	}

//
// Progressive supports scaling and normal decode only.
//
void CProgressiveJpgReadCodec::PreInitFrameL()
	{
	CJpgReadCodec::PreInitFrameL();
	
	if (iOperation != EDecodeNormal)
		{
		JPEG_LEAVE(KErrNotSupported, "No operations on Progressive");
		}
	
	if (iUseClipRect)
		{
		JPEG_LEAVE(KErrNotSupported, "No clipping on Progressive");
		}
	}

void CProgressiveJpgReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* aFrameMask)
	{
	CJpgReadCodec::InitFrameL(aFrameInfo, aFrameImageData, aDisableErrorDiffusion, aFrame, aFrameMask);

	ClearBitmapL(aFrame, KRgbWhite); // clear bitmap so sensibly draw partial decodes

	DoInitFrameL();
	}

void CProgressiveJpgReadCodec::InitFrameL(CImageFrame& aFrame)
	{
	CJpgReadCodec::InitFrameL(aFrame);

	DoInitFrameL();
	}

#if defined(__ARMCC__)
#pragma pop 
#endif

TFrameState CProgressiveJpgReadCodec::DoProcessL()
	{
	FOREVER
		{
		if (iProcessing)
			ProcessFrameL();
		else
			{
			StoreState();
			TInt dataRemaining = iDataPtrLimit - iDataPtr;
			if (dataRemaining < 2)
				return EFrameIncomplete;

			TInt sig = (iDataPtr[0] << 8) | iDataPtr[1];
			switch (sig)
				{
			case KJpgDHTSignature:
				LoadHuffmanTableL();
				break;
			case KJpgDQTSignature:
				break;
			case KJpgSOSSignature:
				LoadSOSL();
				iProcessing = ETrue;
				break;
			case KJpgRestartIntervalSignature:
				LoadRestartIntervalL();
				break;
			case KJpgEOISignature:
				iDataPtr += 2;
				return EFrameComplete;
			default:
#if defined(RELAX_JPEG_STRICTNESS)
				iDataPtr += 1;
				dataRemaining = iDataPtrLimit - iDataPtr;
				if (dataRemaining < 2)
					{
					return EFrameIncomplete;
					}
#else
				User::Leave(KErrCorrupt);
#endif
				break;
				}
			}
		}
	}

void CProgressiveJpgReadCodec::ProcessFrameL()
	{
	if (iScanInfo.iEndSpectralSelection == 0)
		{
		if (iScanInfo.iSuccessiveApproximationBitsHigh == 0)
			InitDCL();
		else
			RefineDCL();
		}
	else
		{
		if (iScanInfo.iSuccessiveApproximationBitsHigh == 0)
			InitACL();
		else
			RefineACL();
		}
	}

void CProgressiveJpgReadCodec::InitDCL()
	{
	if(iScanInfo.iNumberOfComponents == 1)
		{ //Non interleaved scan
		for (TInt scanInfoIndex = 0; scanInfoIndex < iScanInfo.iNumberOfComponents; scanInfoIndex++)
			{
			TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[scanInfoIndex];
			TInt compIndex = ComponentIndexL(scanInfo.iId);
			const TDecHuffmanTable& dcTable = iDCHuffmanTable[scanInfo.iDCCodingTable];
			TInt& dcPredictor = iDCPredictor[compIndex];
			TInt dataUnitCount = iMCUDataUnitCount[compIndex];

			TInt horzSamples = iHorzSampleFactor[compIndex];
			TInt vertSamples = iVertSampleFactor[compIndex];
			TDataUnit* dataUnit = iMCUBuffer[compIndex];
			for (; iCurrentMCUVertCount < iIndividualVertMCUCount[compIndex]; iCurrentMCUVertCount++)
				{
				TInt blockBase = (iCurrentMCUVertCount / vertSamples) * iHorzMCUCount;
				TInt unitBase = (iCurrentMCUVertCount % vertSamples) * horzSamples;
				for (; iCurrentMCUHorzCount < iIndividualHorzMCUCount[compIndex]; iCurrentMCUHorzCount++)
					{
					StoreState();
					if (iRestartMCUCount == 0)
						RestartStateL();

					TInt blockOffset = blockBase + (iCurrentMCUHorzCount / horzSamples);
					TInt unitOffset = unitBase + (iCurrentMCUHorzCount % horzSamples);

					GetDCValueL(dataUnit[(blockOffset * dataUnitCount) + unitOffset],dcTable,dcPredictor);

					iRestartMCUCount--;
					}

				iCurrentMCUHorzCount = 0;
				}
			iCurrentMCUVertCount = 0;
			}
		}
	else
		{ // Interleaved scan
		for(; iCurrentMCUCount < iTotalMCUCount; iCurrentMCUCount++)
			{
			StoreState();
			if (iRestartMCUCount == 0)
				RestartStateL();

			for (TInt scanInfoIndex = 0; scanInfoIndex < iScanInfo.iNumberOfComponents; scanInfoIndex++)
				{
				TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[scanInfoIndex];
				TInt compIndex = ComponentIndexL(scanInfo.iId);
				const TDecHuffmanTable& dcTable = iDCHuffmanTable[scanInfo.iDCCodingTable];
				TInt& dcPredictor = iDCPredictor[compIndex];
				TDataUnit* tempMCUBufferPtr = iMCUBufferPtr[compIndex];

				for (TInt count = iMCUDataUnitCount[compIndex]; count > 0; count--)
					{
					GetDCValueL(*tempMCUBufferPtr,dcTable,dcPredictor);
					tempMCUBufferPtr++;
					}
				}

			for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
				iMCUBufferPtr[count] += iMCUDataUnitCount[count];

			iRestartMCUCount--;
			}
		}

	for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
		iMCUBufferPtr[count] = iMCUBuffer[count];

	ResetState();
	iCurrentMCUCount = 0;
	iProcessing = EFalse;
	}

void CProgressiveJpgReadCodec::RefineDCL()
	{
	for(; iCurrentMCUCount < iTotalMCUCount; iCurrentMCUCount++)
		{
		StoreState();
		if (iRestartMCUCount == 0)
			RestartStateL();

		for (TInt scanInfoIndex = 0; scanInfoIndex < iScanInfo.iNumberOfComponents; scanInfoIndex++)
			{
			TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[scanInfoIndex];
			TInt compIndex = ComponentIndexL(scanInfo.iId);
			TDataUnit* tempMCUBufferPtr = iMCUBufferPtr[compIndex];

			for (TInt count = iMCUDataUnitCount[compIndex]; count > 0; count--)
				{
				if (NextBitL())
					tempMCUBufferPtr->iCoeff[0] |= iRefinedDCValue;
				tempMCUBufferPtr++;
				}
			}

		for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
			iMCUBufferPtr[count] += iMCUDataUnitCount[count];

		iRestartMCUCount--;
		}

	for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
		iMCUBufferPtr[count] = iMCUBuffer[count];

	ResetState();
	iCurrentMCUCount = 0;
	iProcessing = EFalse;
	}

void CProgressiveJpgReadCodec::InitACL()
	{
	if (iScanInfo.iNumberOfComponents > 1)
		User::Leave(KErrCorrupt);

	TInt compIndex = ComponentIndexL(iScanInfo.iComponent[0].iId);
	const TDecHuffmanTable& acTable = iACHuffmanTable[iScanInfo.iComponent[0].iACCodingTable];
	TInt dataUnitCount = iMCUDataUnitCount[compIndex];

	if (dataUnitCount == 1)
		{
		for(; iCurrentMCUCount < iTotalMCUCount; iCurrentMCUCount++)
			{
			StoreState();
			if (iRestartMCUCount == 0)
				RestartStateL();

			if (iSkipCount == 0)
				iSkipCount = GetACValuesL(*iMCUBufferPtr[compIndex],acTable);
			else
				iSkipCount--;

			iMCUBufferPtr[compIndex]++;
			iRestartMCUCount--;
			}

		iCurrentMCUCount = 0;
		}
	else
		{
		TInt horzSamples = iHorzSampleFactor[compIndex];
		TInt vertSamples = iVertSampleFactor[compIndex];
		TDataUnit* dataUnit = iMCUBuffer[compIndex];
		for (; iCurrentMCUVertCount < iIndividualVertMCUCount[compIndex]; iCurrentMCUVertCount++)
			{
			TInt blockBase = (iCurrentMCUVertCount / vertSamples) * iHorzMCUCount;
			TInt unitBase = (iCurrentMCUVertCount % vertSamples) * horzSamples;
			for (; iCurrentMCUHorzCount < iIndividualHorzMCUCount[compIndex]; iCurrentMCUHorzCount++)
				{
				StoreState();
				if (iRestartMCUCount == 0)
					RestartStateL();

				if (iSkipCount == 0)
					{
					TInt blockOffset = blockBase + (iCurrentMCUHorzCount / horzSamples);
					TInt unitOffset = unitBase + (iCurrentMCUHorzCount % horzSamples);
					iSkipCount = GetACValuesL(dataUnit[(blockOffset * dataUnitCount) + unitOffset],acTable);
					}
				else
					iSkipCount--;

				iRestartMCUCount--;
				}

			iCurrentMCUHorzCount = 0;
			}
		iCurrentMCUVertCount = 0;
		}

	for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
		iMCUBufferPtr[count] = iMCUBuffer[count];

	iSkipCount = 0;
	ResetState();
	iProcessing = EFalse;
	}

void CProgressiveJpgReadCodec::RefineACL()
	{
	if (iScanInfo.iNumberOfComponents > 1)
		User::Leave(KErrCorrupt);

	TInt compIndex = ComponentIndexL(iScanInfo.iComponent[0].iId);
	const TDecHuffmanTable& acTable = iACHuffmanTable[iScanInfo.iComponent[0].iACCodingTable];
	TInt dataUnitCount = iMCUDataUnitCount[compIndex];
	if (dataUnitCount == 1)
		{
		for(; iCurrentMCUCount < iTotalMCUCount; iCurrentMCUCount++)
			{
			StoreState();
			if (iRestartMCUCount == 0)
				RestartStateL();

			RefineACValuesL(*iMCUBufferPtr[compIndex],acTable);
			iMCUBufferPtr[compIndex]++;

			iRestartMCUCount--;
			}

		iCurrentMCUCount = 0;
		}
	else
		{
		TInt horzSamples = iHorzSampleFactor[compIndex];
		TInt vertSamples = iVertSampleFactor[compIndex];
		TDataUnit* dataUnit = iMCUBuffer[compIndex];
		for (; iCurrentMCUVertCount < iIndividualVertMCUCount[compIndex]; iCurrentMCUVertCount++)
			{
			TInt blockBase = (iCurrentMCUVertCount / vertSamples) * iHorzMCUCount;
			TInt unitBase = (iCurrentMCUVertCount % vertSamples) * horzSamples;
			for (; iCurrentMCUHorzCount < iIndividualHorzMCUCount[compIndex]; iCurrentMCUHorzCount++)
				{
				StoreState();
				if (iRestartMCUCount == 0)
					RestartStateL();

				TInt blockOffset = blockBase + (iCurrentMCUHorzCount / horzSamples);
				TInt unitOffset = unitBase + (iCurrentMCUHorzCount % horzSamples);
				RefineACValuesL(dataUnit[(blockOffset * dataUnitCount) + unitOffset],acTable);

				iRestartMCUCount--;
				}

			iCurrentMCUHorzCount = 0;
			}
		iCurrentMCUVertCount = 0;
		}

	for (TInt count = 0; count < iFrameInfo.iNumberOfComponents; count++)
		iMCUBufferPtr[count] = iMCUBuffer[count];

	ResetState();
	iProcessing = EFalse;
	}

void CProgressiveJpgReadCodec::InitDrawFrame()
	{
	iCurrentDrawMCU = 0;
	TInt numberOfComponents = iMonochrome ? 1 : iFrameInfo.iNumberOfComponents;
	for (TInt count = 0; count < numberOfComponents; count++)
		{
		iDrawMCUPtr[count] = iMCUBuffer[count];
		}

	if (!iImageFrameCodecPtr)
		{
		CImageProcessor* const imageProc = ImageProcessor();
		imageProc->SetPos(TPoint(0,0));
		}
	}

TBool CProgressiveJpgReadCodec::DrawFrameL()
	{
	TInt numberOfComponents = iMonochrome ? 1 : iFrameInfo.iNumberOfComponents;

	TDataUnit dataUnit;
	CImageProcessor* const imageProc = ImageProcessor();

	const TInt drawMCULimit = Min(iCurrentDrawMCU + KMaxMCUPerDraw, iTotalMCUCount);

	while (iCurrentDrawMCU < drawMCULimit)
		{
		for (TInt compIndex = 0; compIndex < numberOfComponents; compIndex++)
			{
			TQTable& qTable = iQTable[iFrameInfo.iComponent[compIndex].iQTable];
			TDataUnit* compPtr = iComponent[compIndex];
			for (TInt dataUnitCount = iMCUDataUnitCount[compIndex]; dataUnitCount > 0; dataUnitCount--)
				{
				qTable.DeQuantize(dataUnit, *iDrawMCUPtr[compIndex], KJpgDCTBlockSize);
				iCompConf[compIndex].iDCT->InverseTransform(*compPtr, dataUnit);
				iDrawMCUPtr[compIndex]++;
				compPtr++;
				}
			}

		if (imageProc)
			{
			WriteMCU();
			imageProc->SetPixelBlock(iRgbBuffer);
			}
		else
			{
			RArray<const TDataUnit*> dataUnits;
			CleanupClosePushL(dataUnits);
			for (TInt compIndex = 0; compIndex < iFrameInfo.iNumberOfComponents; compIndex++)
				{
				TDataUnit* compPtr = iComponent[compIndex];
				TInt numSamples = iMCUDataUnitCount[compIndex];
				while (numSamples > 0)
					{
					numSamples--;
					User::LeaveIfError(dataUnits.Append(compPtr++));
					}
				}
			iImageFrameCodecPtr->ProcessL(dataUnits);
			CleanupStack::PopAndDestroy(1, &dataUnits);
			}

		iCurrentDrawMCU++;
		}

	if (iCurrentDrawMCU < iTotalMCUCount)
		{
		return EFalse;
		}

	return ETrue;
	}

void CProgressiveJpgReadCodec::GetDCValueL(TDataUnit& aDataUnit,const TDecHuffmanTable& aHuffmanTable,TInt& aDCPredictor)
	{
	TInt size = GetHuffmanCodeL(aHuffmanTable);
	TInt amplitude = (size > 0) ? GetBinaryNumberL(size) : 0;
	aDCPredictor += amplitude;
	ASSERT(aDCPredictor <= KMaxTInt16 && aDCPredictor >= KMinTInt16);
	aDataUnit.iCoeff[0] = TInt16(aDCPredictor << iScanInfo.iSuccessiveApproximationBitsLow);
	}

TInt CProgressiveJpgReadCodec::GetACValuesL(TDataUnit& aDataUnit,const TDecHuffmanTable& aHuffmanTable)
	{
	TInt16* valuePtr = &aDataUnit.iCoeff[iScanInfo.iStartSpectralSelection];
	TInt16* valuePtrLimit = &aDataUnit.iCoeff[iScanInfo.iEndSpectralSelection + 1];

	while (valuePtr < valuePtrLimit)
		{
		TInt s = GetHuffmanCodeL(aHuffmanTable);
		const TInt r = s >> 4;
		s &= 0x0f;
		if (s > 0)
			{
			valuePtr += r;

			if (valuePtr < valuePtrLimit)
				*valuePtr++ |= TInt16(GetBinaryNumberL(s) << iScanInfo.iSuccessiveApproximationBitsLow);
			}
		else
			{
			if (r == 15) // Zero run length
				valuePtr += 16;
			else
				{
				TInt eobRun = 1 << r;
				if (r > 0)
					eobRun += GetPositiveBinaryNumberL(r);
				return eobRun - 1;
				}
			}
		}
	return 0;
	}

void CProgressiveJpgReadCodec::RefineACValuesL(TDataUnit& aDataUnit,const TDecHuffmanTable& aHuffmanTable)
	{
	TInt s = 0;
	TInt r;
	TInt16* valuePtr = &aDataUnit.iCoeff[iScanInfo.iStartSpectralSelection];
	TInt16* valuePtrLimit = &aDataUnit.iCoeff[iScanInfo.iEndSpectralSelection];

	TInt err;
	TInt numNewNonZero = 0;
	TInt16* newNonZeroPosition[KJpgDCTBlockSize];
	TInt oldEobRun = iEobRun;
	if (iEobRun == 0)
		{
		for (; valuePtr <= valuePtrLimit; valuePtr++)
			{
			TRAP(err,s = GetHuffmanCodeL(aHuffmanTable));
			if (err == KErrCompletion)
				goto OutOfData;
			User::LeaveIfError(err);
			r = s >> 4;
			s &= 0x0f;
			if (s > 0)
				{
				TBool nextBit = EFalse;
				TRAP(err,nextBit = NextBitL());
				if (err == KErrCompletion)
					goto OutOfData;
				User::LeaveIfError(err);
				if (nextBit)
					s = iP1;
				else
					s = iM1;
				}
			else
				{
				if (r != 15)
					{
					iEobRun = 1 << r;
					if (r > 0)
						{
						TRAP(err,iEobRun += GetPositiveBinaryNumberL(r));
						if (err == KErrCompletion)
							goto OutOfData;
						User::LeaveIfError(err);
						}
					break;
					}
				}

			do
				{
				TInt16* coef = valuePtr;
				if (*coef != 0)
					{
					TBool nextBit = EFalse;
					TRAP(err,nextBit = NextBitL());
					if (err == KErrCompletion)
						goto OutOfData;
					User::LeaveIfError(err);
					if (nextBit)
						{
						if ((*coef & iP1) == 0)
							{
							if (*coef >= 0)
								*coef = TInt16(*coef + iP1);
							else
								*coef = TInt16(*coef + iM1);
							}
						}
					}
				else
					{
					if (--r < 0)
						break;
					}
				valuePtr++;
				}
			while (valuePtr <= valuePtrLimit);

			if (s != 0)
				{
				*valuePtr = TInt16(s);
				newNonZeroPosition[numNewNonZero++] = valuePtr;
				}
			}
		}

	if (iEobRun > 0)
		{
		for (; valuePtr <= valuePtrLimit; valuePtr++)
			{
			TInt16* coef = valuePtr;
			if (*coef != 0)
				{
				TBool nextBit = EFalse;
				TRAP(err,nextBit = NextBitL());
				if (err == KErrCompletion)
					goto OutOfData;
				User::LeaveIfError(err);
				if (nextBit)
					{
					if ((*coef & iP1) == 0)
						{
						if (*coef >= 0)
							*coef = TInt16(*coef + iP1);
						else
							*coef = TInt16(*coef + iM1);
						}
					}
				}
			}
		iEobRun--;
		}
	return;

OutOfData:
	while (numNewNonZero > 0)
		*(newNonZeroPosition[--numNewNonZero]) = 0;
	iEobRun = oldEobRun;

	User::Leave(KErrCompletion);
	}

#if defined(__ARMCC__)
#pragma push
#pragma thumb 
#endif
	
void CProgressiveJpgReadCodec::LoadHuffmanTableL()
	{
	if (iDataPtr + 4 > iDataPtrLimit)
	    {
	    User::Leave(KErrCompletion);
	    }
		
	TInt length = (iDataPtr[2] << 8) | iDataPtr[3];

	const TUint8* dataPtrLimit = iDataPtr + length + 2;
	if (dataPtrLimit > iDataPtrLimit)
		User::Leave(KErrCompletion);

	iDataPtr += 4;
	while (iDataPtr < dataPtrLimit)
		{
		TInt index = *iDataPtr++;
		TBool dcTable = !(index & 0x10);
		index &= 0x0f;
		if (index >= KJpgMaxNumberOfTables)
			User::Leave(KErrCorrupt);
		TDecHuffmanTable& table = dcTable ? iDCHuffmanTable[index] : iACHuffmanTable[index];
		iDataPtr += table.SetL(iDataPtr,dataPtrLimit);
		}
	}

void CProgressiveJpgReadCodec::LoadSOSL()
	{
	if (iDataPtr + 4 > iDataPtrLimit)
		User::Leave(KErrCompletion);
	TInt length = (iDataPtr[2] << 8) | iDataPtr[3];

	if (iDataPtr + length + 2 > iDataPtrLimit)
		User::Leave(KErrCompletion);

	iDataPtr += 4;
	iScanInfo.iNumberOfComponents = *iDataPtr++;

	// We ony support up to 3 components, even though spec supports up to 4.
	// Additionaly, number must not be greater than in original frame SOS header
	if (iScanInfo.iNumberOfComponents < KJpgMinNumberOfComponents ||
		iScanInfo.iNumberOfComponents > iOriginalFrameInfo.iNumberOfComponents)
		User::Leave(KErrCorrupt);

	for (TInt count = 0; count < iScanInfo.iNumberOfComponents; count++)
		{
		iScanInfo.iComponent[count].iId = *iDataPtr++;

		TUint8 table = *iDataPtr++;
		TUint8 DCTable = static_cast<TUint8>(table >> 4);
		TUint8 ACTable = static_cast<TUint8>(table & 0x0f);

		if(DCTable >= KJpgMaxNumberOfTables || ACTable >= KJpgMaxNumberOfTables)
			User::Leave(KErrCorrupt);

		iScanInfo.iComponent[count].iDCCodingTable = DCTable;
		iScanInfo.iComponent[count].iACCodingTable = ACTable;

		iDCHuffmanTable[DCTable].MakeDerivedTableL();
		iACHuffmanTable[ACTable].MakeDerivedTableL();
		}

	iScanInfo.iStartSpectralSelection = *iDataPtr++;
	iScanInfo.iEndSpectralSelection = *iDataPtr++;
	TUint8 successiveApproximation = *iDataPtr++;
	iScanInfo.iSuccessiveApproximationBitsHigh = successiveApproximation >> 4;
	iScanInfo.iSuccessiveApproximationBitsLow = successiveApproximation & 0x0f;
	iP1 = 1 << iScanInfo.iSuccessiveApproximationBitsLow;
	iM1 = (-1) << iScanInfo.iSuccessiveApproximationBitsLow;

	iRefinedDCValue = TInt16(1 << iScanInfo.iSuccessiveApproximationBitsLow);

	// if iRestartInterval is 0, iRestartMCUCount is set to a negative value (KErrNotFound) to skip the 0 trigger points that would call RestartStateL
	// This is done to solve the problem that on some images the iRestartInterval marker is 0 on every frame
	iRestartMCUCount = iFrameInfo.iRestartInterval > 0 ? iFrameInfo.iRestartInterval : KErrNotFound;
	}

void CProgressiveJpgReadCodec::LoadRestartIntervalL()
	{
	if (iDataPtr + 6 > iDataPtrLimit)
		{
		User::Leave(KErrCompletion);
		}

	iFrameInfo.iRestartInterval = iDataPtr[5] | (iDataPtr[4] << 8);
	iDataPtr += 6;

	// if iRestartInterval is 0, iRestartMCUCount is set to a negative value (KErrNotFound) to skip the 0 trigger points that would call RestartStateL
	// This is done to solve the problem that on some images the iRestartInterval marker is 0 on every frame
	iRestartMCUCount = iFrameInfo.iRestartInterval > 0 ? iFrameInfo.iRestartInterval : KErrNotFound;
	}

// 03/12/03 - function added as a result of INC037134
// needed a way to cleanup buffers when decoding complete
void CProgressiveJpgReadCodec::CleanupBuffers()
	{
	if(!iMCUChunkAllocated)
		{
        delete iMCUMemoryBuffer;
        iMCUMemoryBuffer = NULL;
		}
	else
		{
		iMCUChunk.Close();
		}
    for (TInt count = 0; count < KJpgNumberOfComponents; count++)
		{
		iMCUBuffer[count] = NULL;
		}
	iMCUChunkAllocated = EFalse;
	}

// CJpgImageFrameReadCodec
CJpgImageFrameReadCodec::CJpgImageFrameReadCodec(CImageFrame* aFrame)
	{
	iDestination = aFrame;
	}

CJpgImageFrameReadCodec::~CJpgImageFrameReadCodec()
	{
	delete iImageFrameProcessorPtr;
	}

CJpgImageFrameReadCodec* CJpgImageFrameReadCodec::NewL(CImageFrame* aFrame)
	{
	CJpgImageFrameReadCodec* self = new(ELeave) CJpgImageFrameReadCodec(aFrame);
	return self;
	}

void CJpgImageFrameReadCodec::CreateImageProcessorL(const TJpgFrameInfo& aFrameInfo)
	{
	ASSERT(iImageFrameProcessorPtr==NULL);

	iDestination->SetFrameSizeInPixels(aFrameInfo.iSizeInPixels);
	CJpgImageFrameProcessorUtility::PrepareImageFrameL(aFrameInfo,*iDestination);
	iImageFrameProcessorPtr = CJpgImageFrameProcessorUtility::NewL(*iDestination);
	}

void CJpgImageFrameReadCodec::ProcessL(const RArray<const TDataUnit*>& aDataUnits)
	{
	iImageFrameProcessorPtr->WriteBlockL(aDataUnits);
	}

CImageFrame* CJpgImageFrameReadCodec::Destination()
	{
	return iDestination;
	}

void CJpgImageFrameReadCodec::SetImageFrameBlocksL(CImageFrame* aFrame, const TJpgFrameInfo& aFrameInfo)
	{
	iDestination = aFrame;

	if(iImageFrameProcessorPtr)
		{
		delete iImageFrameProcessorPtr;
		iImageFrameProcessorPtr = NULL;
		}

	CJpgImageFrameProcessorUtility::PrepareImageFrameL(aFrameInfo, *iDestination);
	iImageFrameProcessorPtr = CJpgImageFrameProcessorUtility::NewL(*iDestination);
	}

//
// Multiscan sequential read codec.
//
CMultiScanSequentialJpgReadCodec* CMultiScanSequentialJpgReadCodec::NewL(
	const TJpgFrameInfo& aFrameInfo,
	const TJpgScanInfo& aScanInfo,
	TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
	TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
	const TQTable aQTable[KJpgMaxNumberOfTables])
	{
	CMultiScanSequentialJpgReadCodec* self = new(ELeave) CMultiScanSequentialJpgReadCodec(
			aFrameInfo,
			aScanInfo,
			aDCHuffmanTable,
			aACHuffmanTable,
			aQTable);
	CleanupStack::PushL(self);
	self->ConstructL(EFalse);	// No cache needed
	CleanupStack::Pop(self);
	return self;
	}


//
//
//
CMultiScanSequentialJpgReadCodec::CMultiScanSequentialJpgReadCodec(
		const TJpgFrameInfo& aFrameInfo,
		const TJpgScanInfo& aScanInfo,
		TDecHuffmanTable aDCHuffmanTable[KJpgMaxNumberOfTables],
		TDecHuffmanTable aACHuffmanTable[KJpgMaxNumberOfTables],
		const TQTable aQTable[KJpgMaxNumberOfTables])
 :	CSequentialJpgReadCodec(aFrameInfo,
		 aScanInfo,
		 aDCHuffmanTable,
		 aACHuffmanTable,
		 aQTable)
	{
	Mem::Copy(&iFirstScan, &iScanInfo, sizeof(TJpgScanInfo));
	}


//
//
//
CMultiScanSequentialJpgReadCodec::~CMultiScanSequentialJpgReadCodec()
	{
	iMCUChunk.Close();
	}


//
//
//
void CMultiScanSequentialJpgReadCodec::PreInitFrameL()
	{
	CSequentialJpgReadCodec::PreInitFrameL();
	
	// Restore the first scan.
	Mem::Copy(&iScanInfo, &iFirstScan, sizeof(TJpgScanInfo));

	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		iCompAvailable[i] = EFalse;
		}
	
	iMCUChunk.Close();
	iMCUMemoryOffset = NULL;
	}


//
//
//
void CMultiScanSequentialJpgReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aFrame, CFbsBitmap* aFrameMask)
	{
	CJpgReadCodec::InitFrameL(aFrameInfo, aFrameImageData, aDisableErrorDiffusion, aFrame, aFrameMask);
	
	iTotalMCUBlocks = GetHorzMCUCount() * GetVertMCUCount();
	
	// Prepare RChunk memory for all components
	TInt mcuMemory = 0;
	for (TInt i = 0; i < iFrameInfo.iNumberOfComponents; i++)
		{
		mcuMemory += iTotalMCUBlocks * iMCUDataUnitCount[i] * sizeof(TDataUnit);
		}
	
	TInt err = iMCUChunk.CreateLocal(mcuMemory, mcuMemory);
	JPEG_LEAVE_IF_ERROR(err, "Chunk creation");
	
	iMCUMemoryOffset = iMCUChunk.Base();
	
	// Allocate memory for individual components in scan.
	for (TInt scanInfoIndex = 0; scanInfoIndex < iScanInfo.iNumberOfComponents; scanInfoIndex++)
		{
		TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[scanInfoIndex];
		TInt compIndex = ComponentIndexL(scanInfo.iId);
		TInt unitCount = iTotalMCUBlocks * iMCUDataUnitCount[compIndex];
		
		iCompBuf[compIndex] = reinterpret_cast<TDataUnit*>(iMCUMemoryOffset);
		iCompAvailable[compIndex] = ETrue;
		
		iMCUMemoryOffset += unitCount * sizeof(TDataUnit);
		}
	
	iCurrentMcuIdx = 0;
	}

#if defined(__ARMCC__)
#pragma pop 
#endif

//
//
//
TFrameState CMultiScanSequentialJpgReadCodec::DoProcessL()
	{
	while (iDataPtr < iDataPtrLimit)
		{
		StoreState();
		if (iRestartMCUCount == 0)
			{
			RestartStateL();
			}

		const TUint8* const latestDataPtr = iDataPtr;
		TInt error = KErrNone;
		// we do that "if" in order to bypass exception handling which can
		// affect performance of the decoder
		if (iPreviousDataLeft < KMCUDataLeftThreshhold)
			{
			TRAP(error, ProcessMCUL());
			// leave if it wasn't a partial MCU
			if (error != KErrNone && 	(error != KErrEof || latestDataPtr == iDataPtr ||
											(latestDataPtr + sizeof(TUint16) <= iDataPtrLimit
											  && PtrReadUtil::ReadBigEndianUint16(latestDataPtr)==KJpgEOISignature
											)
										)
				)
				{
				JPEG_LEAVE(error, "From ProcessMCUL_1");
				}
			}
		else
			{
			ProcessMCUL();
			}

		JPEG_LEAVE_IF_ERROR(error, "From slow ProcessMCUL");
		
		//in case of corrupt image, if MCUs exceed total MCUs then leave
		if (iCurrentMcuIdx >= iTotalMCUBlocks)
			{
			JPEG_LEAVE(KErrEof, "Too many MCUs in image");
			}
		
		for (TInt scanInfoIndex = 0; scanInfoIndex < iScanInfo.iNumberOfComponents; scanInfoIndex++)
			{
			TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[scanInfoIndex];
			TInt compIndex = ComponentIndexL(scanInfo.iId);
			CopyMCUs(compIndex);
			}
		
		iCurrentMcuIdx++;
		iRestartMCUCount--;

		if (iDataPtrLimit - iDataPtr < KMCUDataLeftThreshhold)
			{
			TBool needLeave = (iPreviousDataLeft > iDataPtrLimit - iDataPtr);
			iPreviousDataLeft = iDataPtrLimit - iDataPtr;
			if (needLeave && !iNewTableOrScan)
				{
				StoreState();
				User::Leave(KErrCompletion);
				}
			}
		}
	return EFrameIncomplete;
	}


//
// Searches Start of Scan marker(0xffda) or Huffman table marker(0xffc4) and gives the position in aPos.
//
TBool CMultiScanSequentialJpgReadCodec::SearchTableOrSosMarker(TBufPtr8& aSourceData, TInt& aPos, TUint16& aMarker)
	{
	const TUint8* ptr = aSourceData.Ptr();
	const TUint8* limit = ptr + aSourceData.Length() - 1;
	
	aPos = 0;
	while ((ptr + 1) <= limit)
		{
		// Mem::Copy must be used to avoid KERN:EXEC 3 access alignment panics on hardware.
		TUint16 data = 0;
		Mem::Copy(&data, ptr++, sizeof(TUint16));
		
		switch (data)
			{
			case 0xDAFF:	// Big-endian version of KJpgSOSSignature.
				aMarker = KJpgSOSSignature;
				return ETrue;
				
			case 0xC4FF:	// Big-endian version of KJpgDHTSignature.
				aMarker = KJpgDHTSignature;
				return ETrue;
				
			default:
				aPos++;
			}
		}
	
	return EFalse;
	}


//
// Processes frame data. Renders only on decoding all MCUs.
//
TFrameState CMultiScanSequentialJpgReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	TUint16 marker = 0;
	TInt markerPos = -1;
	iDataPtr = const_cast<TUint8*>(aSrc.Ptr());
	TInt dataLen = aSrc.Length();
	const TUint8* newMarkerAddr = NULL;
	const TUint8* bufferLimit = iDataPtr + dataLen;
	TBool switched = EFalse;
	
	// Exclude table or next scan.
	iNewTableOrScan = SearchTableOrSosMarker(aSrc, markerPos, marker);
	if (iNewTableOrScan)
		{
		newMarkerAddr = iDataPtr + markerPos;
		iDataPtrLimit = const_cast<TUint8*>(newMarkerAddr);
		}
	else
		{
		// If marker 0xff is in the last byte, exclude it.
		iDataPtrLimit = iDataPtr + dataLen;
		if (*(iDataPtrLimit - 1) == 0xFF)
			{
			iDataPtrLimit--;
			}
		}
	
	iPreviousDataLeft = iDataPtrLimit - iDataPtr;

	TFrameState frameState = EFrameComplete;
	TRAPD(err, frameState = DoProcessL());
	// If there is new scan or table, switch to it.
	if (iNewTableOrScan)
		{
		if (marker == KJpgDHTSignature)
			{
			switched = ProcessHuffmanTableL(newMarkerAddr, bufferLimit);
			}
		else if (marker == KJpgSOSSignature)
			{
			switched = SwitchScanL(newMarkerAddr, bufferLimit);
			}
		}
	
	if (err != KErrNone)
		{
		if (err == KErrCompletion)
			{
			RestoreState();
			frameState = EFrameIncomplete;
			}
		else if (err == KErrEof)
			{
			frameState = EFrameComplete;
			// Render all MCUs.
			RenderMCUsL();
			}
		else
			{
			JPEG_LEAVE(err, "From DoProcessL");
			}
		}
	
	if (switched)
		{
		iDataPtr = const_cast<TUint8*>(newMarkerAddr);
		if (marker == KJpgSOSSignature)
			{
			ResetOnNewScan();
			}
		}
	
	aSrc.Shift(iDataPtr - aSrc.Ptr()); // Shift out used data.

	return frameState;
	}


//
// Switches to new scan if sufficient header information is available.
//
TBool CMultiScanSequentialJpgReadCodec::SwitchScanL(const TUint8*& aScan, const TUint8* aDataLimit)
	{
	const TInt KSosSizeFieldLength = 2;
	if (aScan + sizeof(KJpgSOSSignature) + KSosSizeFieldLength > aDataLimit)
		{
		return EFalse;
		}
	
	// Check for SOS marker.
	TUint16 value = ReadBigEndianUint16(aScan);
	if (value != KJpgSOSSignature)
		{
		return EFalse;
		}
	
	// Read length and check if entire scan is available.
	value = ReadBigEndianUint16(aScan);
	if (aScan + value - KSosSizeFieldLength > aDataLimit)
		{
		return EFalse;
		}
	
	// Keep track of MCUs available for components.
	for (TInt i = 0; i < iScanInfo.iNumberOfComponents; i++)
		{
		TInt compIndex = ComponentIndexL(iScanInfo.iComponent[i].iId);
		iCompMcuCount[compIndex] = iCurrentMcuIdx;
		}

	iScanInfo.iNumberOfComponents = *aScan;
	// Component id, table selector bytes.
	const TInt KCompBytes = 2;
	// Scan start, scan end, successive approx bytes.
	const TInt KScanBytes = 3;
	if (aScan + (iScanInfo.iNumberOfComponents * KCompBytes) + KScanBytes > aDataLimit)
 		{
 		// Header length is wrong. Entire header is not available in the buffer.
		return EFalse;
		}
	
	TJpgScanInfoProcessor::ProcessStartOfScanL(aScan, iFrameInfo, iScanInfo, iDCHuffmanTable, iACHuffmanTable);
	
	// Move to next address after scan.
	aScan++;
	for (TInt i = 0; i < iScanInfo.iNumberOfComponents; i++)
		{
		TJpgScanInfo::TScanComponentInfo& scanInfo = iScanInfo.iComponent[i];
		TInt compIndex = ComponentIndexL(scanInfo.iId);
		if (iCompAvailable[compIndex])
			{
			// If a same component repeats, then the file is corrupt. Stop further decoding.
			JPEG_LEAVE(KErrEof, "Repeated component");
			}
		
		TInt unitCount = iTotalMCUBlocks * iMCUDataUnitCount[compIndex];
		iCompBuf[compIndex] = reinterpret_cast<TDataUnit*>(iMCUMemoryOffset);
		iCompAvailable[compIndex] = ETrue;
		
		iMCUMemoryOffset += unitCount * sizeof(TDataUnit);
		}
	
	iCurrentMcuIdx = 0;
	if (iFrameInfo.iRestartInterval > 0)
		{
		iRestartMCUCount = iFrameInfo.iRestartInterval;
		}
	else
		{
		iRestartMCUCount = KErrNotFound;
		}
	
	ResetState();
	return ETrue;
	}


//
//
//
TBool CMultiScanSequentialJpgReadCodec::ProcessHuffmanTableL(const TUint8*& aData, const TUint8* aBufferLimit)
	{
	// Length of size field for table marker.
	const TInt KTableMarkerSizeField = 2;
	if (aData + sizeof(KJpgDHTSignature) + KTableMarkerSizeField > aBufferLimit)
		{
		return EFalse;
		}
	
	// Read marker.
	TUint16 value = ReadBigEndianUint16(aData);
	if (value != KJpgDHTSignature)
		{
		return EFalse;
		}
	
	// Read length.
	value = ReadBigEndianUint16(aData);
	
	// Check if entire table data is available in buffer.
	const TUint8* dataLimit = aData + value - KTableMarkerSizeField;
	if (dataLimit > aBufferLimit)
		{
		return EFalse;
		}
	
	THuffmanTableProcessor::ProcessHuffmanTableL(aData, dataLimit, iDCHuffmanTable, iACHuffmanTable);
	
	return ETrue;
	}


//
// Copies MCUs to component buffers, where they are accumulated.
//
void CMultiScanSequentialJpgReadCodec::CopyMCUs(TInt aCompIdx)
	{
	if (iMonochrome && (aCompIdx != KYComp))
		{
		return;
		}
	
	// Assumes that iCurrentMcuIdx value start from 0.
	const TDataUnit* src = iComponent[aCompIdx];
	TDataUnit* des = iCompBuf[aCompIdx];
	
	// If Y with 4 DUs.
	if ((aCompIdx == KYComp) && (iMCUDataUnitCount[KYComp] == 4))
		{
		// Rearranges Y Data Units, so that they are rendered sequentially.
	   	TInt mcusPerLine = GetHorzMCUCount();
		TInt dusPerLine = mcusPerLine * iMCUDataUnitCount[KYComp];
		TInt line =  iCurrentMcuIdx / mcusPerLine;
		TDataUnit* lineAddr = des + (line * dusPerLine);
		TInt mcuIdxInLine = iCurrentMcuIdx - (line * mcusPerLine);
		
		// Copy offset.
		TInt offset = 0;
		if ((mcuIdxInLine + 1) <= mcusPerLine / 2)
			{
			offset = mcuIdxInLine * 8;
			}
		else
			{
			TInt idx = (mcuIdxInLine + 1) - (mcusPerLine / 2);
			offset = (idx  - 1) * 8 + 2;
			}
		
		// Copy 2 DUs at a time, giving 2 DU space.
		Mem::Copy(lineAddr + offset, src, 2 * sizeof(TDataUnit));
		Mem::Copy(lineAddr + offset + 4, src + 2, 2 * sizeof(TDataUnit));
		}
	else
		{
		des += iCurrentMcuIdx * iMCUDataUnitCount[aCompIdx];
		Mem::Copy(des, src, iMCUDataUnitCount[aCompIdx] * sizeof(TDataUnit));
		}
	}


//
// Sets up iComponent to point to the right parts of the RChunk memory.
//
void CMultiScanSequentialJpgReadCodec::PrepareToRenderMCU(TInt aMCUIndex)
	{
	JPEG_ASSERT(aMCUIndex >= 0);
	
	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		if (iCompAvailable[i])
			{
			iComponent[i] = &iCompBuf[i][aMCUIndex * iMCUDataUnitCount[i]];
			}
		}
	}


//
// Sends all MCUs of components to rendering.
//
void CMultiScanSequentialJpgReadCodec::RenderMCUsL()
	{
	for (TInt i = 0; i < iScanInfo.iNumberOfComponents; i++)
		{
		// Keep track of number of MCUs available for components.
		TInt compIndex = ComponentIndexL(iScanInfo.iComponent[i].iId);
		iCompMcuCount[compIndex] = iCurrentMcuIdx;
		}
	
	FillEmptyMCUs();
	
	while (iNeededMCU != KErrCompletion)
		{
		iStreamMCU = iNeededMCU;	// Maintain behaviour of CSequentialJpgReadCodec.

		Mem::Copy(&iComponentCpy, &iComponent, sizeof(TDataUnit*) * KJpgNumberOfComponents);
		PrepareToRenderMCU(iNeededMCU);
		PostProcessMCUL(EFalse);
		Mem::Copy(&iComponent, &iComponentCpy, sizeof(TDataUnit*) * KJpgNumberOfComponents);

		
		iNeededMCU = iMCUStore->GetNextMCU();
		}
	}


//
// If the number of available MCUs for Cb and Cr is less than that of Y, those MCUs are filled with 0x7f.
//
void CMultiScanSequentialJpgReadCodec::FillEmptyMCUs()
	{
	// MCU index to start filling. This may involve overwriting existing U or V data.
	TInt startIdx = Min(iCompMcuCount[KUComp], iCompMcuCount[KVComp]);
	for (TInt i = KUComp; i <= KVComp; i++)
		{
		if (iComponent[i] == NULL)
			{
			// If monochrome or if the component is not part of image.
			continue;
			}
		
		TDataUnit* des = NULL;
		if (iCompAvailable[i])
			{
			des = iCompBuf[i];
			des += startIdx * iMCUDataUnitCount[i];
			TInt dus = (iCompMcuCount[KYComp] - startIdx) * iMCUDataUnitCount[i];
			for (TInt k = 0; k < dus; k++)
				{
				FillEmptyDU(&des[k]);
				}
			}
		else
			{
			des = iComponent[i];
			// If component is not at all available, fill iComponent[] buffer with 0x7f.
			for (TInt k = 0; k < iMCUDataUnitCount[i]; k++)
				{
				FillEmptyDU(&des[k]);
				}
			}
		}
	}


//
// Resets the data members so that new scan can be started
//
void CMultiScanSequentialJpgReadCodec::ResetOnNewScan()
	{
	iInitialDataPtr = NULL;
	iBitBufferPtrLimit = NULL;
	iDataValue = 0;
	iInitialDataValue = 0;
	iBitsLeft = 0;
	iInitialBitsLeft = 0;
	
	for (TInt i = 0; i < KJpgNumberOfComponents; i++)
		{
		iDCPredictor[i] = 0;
		iInitialDCPredictor[i] = 0;
		}
	}


//
// Fills the given Data Unit with 0x7f.
//
void CMultiScanSequentialJpgReadCodec::FillEmptyDU(TDataUnit* pDU)
	{
	JPEG_ASSERT(pDU);
	JPEG_ASSERT(pDU->iCoeff);
	
	TDataUnit::TDataUnitElemType* ptr = pDU->iCoeff;
	for(TInt i = 0; i < KJpgDCTBlockSize; i++)
		{
		*ptr++ = 0x7f;
		}
	}



