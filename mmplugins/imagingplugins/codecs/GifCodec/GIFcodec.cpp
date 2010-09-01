// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Licensed under US Patent No 4,558,302 and foreign counterparts
// 
//

// CGifWriteCodec::TableEntry() is based on compress_byte() from wrgif.c
// in the IJG V6 code, Copyright (C) 1991-1997, Thomas G. Lane.

#include <fbs.h>
#include "GIFcodec.h"
#include "rawimageprocessor.h"

const TInt KPass2StartLine = 4;
const TInt KPass3StartLine = 2;
const TInt KPass4StartLine = 1;
const TInt KPass1YPosIncrement = 8;
const TInt KPass2YPosIncrement = 8;
const TInt KPass3YPosIncrement = 4;
const TInt KPass4YPosIncrement = 2;
const TInt KPass1LineRepeat = 7;
const TInt KPass2LineRepeat = 3;
const TInt KPass3LineRepeat = 1;
const TInt KPass4LineRepeat = 0;
const TUint KLZWLimit = 20000;
const TInt KSetCommentsLimit = 64;
const TUint16 KTranspColIdxNotPresent = 0xFFFF;


// CGifReadCodec
CGifReadCodec::CGifReadCodec(TRgb* aGlobalPalette,TInt aBackgroundColorIndex, TSize aScreenSize,TBool aFastDecode):
	iBackgroundColorIndex(aBackgroundColorIndex),
	iScreenSize(aScreenSize),
	iGlobalPalette(aGlobalPalette),
	iFastDecode(aFastDecode)
	{
	}

CGifReadCodec::~CGifReadCodec()
	{
	iComment.ResetAndDestroy();
    delete [] iPrefixIndex;
	delete [] iSuffixCode;
	delete [] iOutputString;
	delete [] i64KPalette;
	delete [] iPixelBuffer;
	delete [] iMaskBuffer;
	}

CGifReadCodec* CGifReadCodec::NewL(TRgb* aGlobalPalette,TInt aBackgroundColorIndex, TSize aScreenSize,TBool aFastDecode)
	{
	CGifReadCodec* self = new(ELeave) CGifReadCodec(aGlobalPalette, aBackgroundColorIndex, aScreenSize, aFastDecode);
	CleanupStack::PushL(self);

    self->ConstructL();

	CleanupStack::Pop(self); 
	return self;
	}

void CGifReadCodec::ConstructL()
    {
    CImageMaskProcessorReadCodec::ConstructL();
    
	iPrefixIndex    = new (ELeave) TInt16[KGifConversionTableSize +1];
	iSuffixCode     = new (ELeave) TUint8 [KGifConversionTableSize+1];
	iOutputString   = new (ELeave) TUint8 [KGifConversionTableSize];
	iPPos = &Pos();
	iPixelBuffer = NULL;
	iMaskBuffer = NULL;
    }

void CGifReadCodec::InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData)
	{
	ASSERT(aFrameSettings.CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised);
	iFrameInfo = &aFrameSettings;
	iFrameData = &aFrameImageData;
	iFrameImageDesc = NULL;
	iFrameImageControl = NULL;
	iReadingOtherExtensionBlock=EFalse;
	iReadingCommentExtensionBlock = EFalse;
	iComment.ResetAndDestroy();
	iCommentIndex = 0;
	iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);
	}

TFrameState CGifReadCodec::ProcessFrameHeaderL(TBufPtr8& aData)
	{	
	iStartDataPtr = CONST_CAST(TUint8*,aData.Ptr());
	iDataPtr = iStartDataPtr;
	iDataPtrLimit = iStartDataPtr + aData.Length();

	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrame)
		{
		do	{
			//we need the block length (iDataPtr[0]+1) + the first byte of the next block
			//(used in the KGifTerminatorId evaluation below)
			if((iDataPtr + iDataPtr[0] + 2)<iDataPtrLimit)
				{// If there is enough data in the buffer to continue processing header info
				iFrameLZWInfo->iCompressedBytes += iDataPtr[0] + 1;

				// Shift the local buffer data pointer to point to the beginning of the next block
				iDataPtr += iDataPtr[0] + 1;
			}
			else
				{ // Else if there is not enough data in the buffer shift the 'aData' pointer
				// to point to the degining of the unused data and return to the calling method
				// so that the buffer can be refilled.
				aData.Shift((iDataPtr - iStartDataPtr));
				return EFrameIncomplete;
				}
			} while(iDataPtr[0]!=KGifFrameTerminatorId); //Continue until end of frame is reached

		if(iDataPtr[1] == KGifTerminatorId)
			{ // End of Image File
			DoSaveCommentsL();
			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
			return EFrameComplete;
			}
		else if ((iDataPtr[1] == KGifImageDescriptorId) || (iDataPtr[1] == KGifExtensionId))
			{ // End of Frame
			DoSaveCommentsL();
			aData.Shift((iDataPtr - iStartDataPtr)+1); 
			iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
			return EFrameIncomplete;
			}
		else
			{ // Unexpected end of data
			return EUnexpectedEndOfData;
			}
		}

	if (iFrameInfo->CurrentFrameState() == TFrameInfo::EFrameInfoProcessingFrameHeader)
		{
		TRAPD(err,DoProcessInfoL(iDataPtr,iDataPtrLimit));
		if (err == KErrCompletion)
			{
			DoSaveCommentsL();
			return EFrameComplete; // KGifTerminatorId found - no more images
			}

		if (err == KErrUnderflow)
			{
			TInt dataUsed = iDataPtr - iStartDataPtr;
			aData.Shift(dataUsed);
			TInt frameDataOffset = iFrameInfo->FrameDataOffset();
			iFrameInfo->SetFrameDataOffset(frameDataOffset + dataUsed);
			return EFrameIncomplete;
			}

		if (err!=KErrNone)
			{
			User::LeaveIfError(err); // A real error occured
			}

		if (iFrameImageDesc == NULL)
			{
			User::Leave(KErrCorrupt);
			}

		TInt frameDataOffset = iFrameInfo->FrameDataOffset();
		iFrameInfo->SetFrameDataOffset(frameDataOffset + (iDataPtr - iStartDataPtr));
		aData.Shift(iDataPtr - iStartDataPtr);

		iFrameInfo->iFrameSizeInTwips.SetSize(0,0);
		iFrameInfo->iDelay = (iFrameImageControl) ? iFrameImageControl->iDelayTimeInCentiseconds * KGifCentiSecondsToMicroSeconds : 0;
		iFrameInfo->iFlags = TFrameInfo::EColor | TFrameInfo::ECanDither | TFrameInfo::EUsesFrameSizeInPixels;
		if (iFrameImageControl)
			{
			if (iFrameImageControl->iTransparentColorIndex != KErrNotFound)
				{
				iFrameInfo->iFlags |= TFrameInfo::ETransparencyPossible;
				if(iFrameImageDesc->iInterlaced)
					iFrameInfo->iFlags |= TFrameInfo::EPartialDecodeInvalid;
				}
			switch (iFrameImageControl->iDisposalMethod)
				{
			case TGifImageControl::ELeaveInPlace:
				iFrameInfo->iFlags |= TFrameInfo::ELeaveInPlace;
				break;
			case TGifImageControl::ERestoreToBackground:
				iFrameInfo->iFlags |= TFrameInfo::ERestoreToBackground;
				break;
			case TGifImageControl::ERestoreToPrevious:
				iFrameInfo->iFlags |= TFrameInfo::ERestoreToPrevious;
				break;
			case ENone:
			default:
				break;
				};
			}

		iFrameInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame);
		}

	return EFrameIncomplete;
	}

TFrameState CGifReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	if ((iFrameSize.iWidth == 0) || (iFrameSize.iHeight == 0))
		return EFrameComplete;

	iPixRead = 0;

	while (aSrc.Length() > 2 && !iComplete)
		{
		iDataPtr = aSrc.Ptr();
		const TInt dataSize = iDataPtr[0];
		iDataPtr++;
		if (dataSize == 0 || dataSize >= aSrc.Length())
		    {
		    break;
		    }
		iDataPtrLimit = iDataPtr + dataSize;
		DoProcessDataL();

		aSrc.Shift(dataSize + 1);
		}

	TPoint& pos = Pos();
	pos = *iPPos;
	FlushPixBuffer(iLatestPixSize);
	
	if (iFastAccessMode) 
		{
		const TInt frameWidth = iFrameSize.iWidth;
		TInt rowPixels = Min(frameWidth - pos.iX, iPixRead);
		pos.iX += rowPixels;
		iPixRead -= rowPixels;
		if (pos.iX == frameWidth)
			{
			TInt rowCount = iPixRead/frameWidth+1;
			pos.iY += rowCount;
			iPixRead -= (rowCount-1) * frameWidth;
			pos.iX = iPixRead;
			}
		iPixRead = 0;
		}
	
	if (pos.iY >= (iFrameSize.iHeight + iFrameOffset.iY) || iComplete)
		{
		ImageProcessor()->FlushPixels();
		CImageProcessor*const maskProc = MaskProcessor();
		if (maskProc)
			maskProc->FlushPixels();
		pos = iFrameOffset;
		//in case of iFast64kMode == true, palette is of type T64KPixel
		if(iGifImageControl && iGifImageControl->iTransparentColorIndex != KErrNotFound && !iFast64kMode)
			{
			// reset the transparency index
			if (iTranspColIdx != KTranspColIdxNotPresent)
				{
				const_cast<TRgb*>(&iPalette[iTranspColIdx])->SetAlpha(0xFF);
				}
			}
		
		return EFrameComplete;
		}

	return EFrameIncomplete;
	}

void CGifReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask)
	{
  	iFrameInfo = &aFrameInfo;
  	iFrameData = &aFrameImageData;

	//Make the first frame's size equal to the image size
	if(CurrentFrame()==0)
		{
		iFrameSize = iFirstFrameSize;
		iFrameOffset = iFirstFrameCoords.iTl;
		}
	else
		{
		iFrameSize = iFrameInfo->iOverallSizeInPixels;
		iFrameOffset = TPoint(0,0);
		}
	iFrameCoords = iFrameInfo->iFrameCoordsInPixels;

	// If the width or height is zero return. A similar test in ProcessFrameL() will complete the image
	if ((iFrameSize.iWidth == 0) || (iFrameSize.iHeight == 0))
		{
		return;
		}

	if(iPixelBuffer == NULL || CurrentFrame() == 0)
		{
		iPixBufferSize = iFrameSize.iWidth * 8;//allocate buffer for first 8 lines
		if (iPixBufferSize > KPixelBufMaxSize) 
			{
			iPixBufferSize = KPixelBufMaxSize;
			}
		if (aDestinationMask != NULL)
			{
			iPixBufferSize /= 2;//reserve space for mask
			}
		delete[] iPixelBuffer;
		iPixelBuffer = new (ELeave) TUint32 [iPixBufferSize];
		}

	// Extract the image descriptor and control blocks.
	iGifImageDesc = NULL;
	iGifLZWInfo = NULL;
	iGifImageControl = NULL;
	iGifColorTable = NULL;

	TInt frameDataCount = iFrameData->FrameDataCount();
	for (TInt frameDataIndex = 0 ; frameDataIndex<frameDataCount ; frameDataIndex++)
		{
		TFrameDataBlock* frameData = iFrameData->GetFrameData(frameDataIndex);
		if (frameData->DataType() == KGIFImageDescriptorUid)
			{
			iGifImageDesc = STATIC_CAST(TGifImageDescriptor*, frameData);
			}
		else if(frameData->DataType() == KGIFColorTableUid)
			{
			iGifColorTable = STATIC_CAST(TGifColorTable*, frameData);
			}
		else if(frameData->DataType() == KGIFLZWInfoUid)
			{
			iGifLZWInfo = STATIC_CAST(TGifLZWInfo*, frameData);
			}
		else if(frameData->DataType() == KGIFImageControlUid)
			{
			iGifImageControl = STATIC_CAST(TGifImageControl*, frameData);
			}
		}
	iTranspColIdx = (iGifImageControl) ? iGifImageControl->iTransparentColorIndex : KTranspColIdxNotPresent;

	// Set decode info
	iPass = 1;
	iYPosIncrement = KPass1YPosIncrement;
	iBitBuffer = 0;
	iBitBuffSize = 0;
	iFirstChar = 0;
	iPreviousCode = -1;
	iComplete = EFalse;

	ASSERT(iGifImageDesc);
	ASSERT(iGifLZWInfo);

	// Set palette to use
	if(iGifImageDesc && iGifImageDesc->iLocalColorMap)
		{
		//Use of a frame palette was requested but was not found
		if(iGifColorTable == NULL)
			{
			User::Leave(KErrCorrupt);
			}
		iPalette = iGifColorTable->iPalette;
		}
	else
		{
		iPalette = iGlobalPalette;
		}

	// Set table.Check for iClearCode value between 0 and 4096.
	iClearCode = (1 << (iGifLZWInfo->iInitialCodeLength - 1));
	if((iClearCode < 0) || (iClearCode > KGifConversionTableSize))
		{
		User::Leave(KErrCorrupt);
		}
	iEoiCode = iClearCode + 1;
	for (TInt singleByteCodes = 0; singleByteCodes < iClearCode; singleByteCodes++)
		{
		iSuffixCode[singleByteCodes] = STATIC_CAST(TUint8,singleByteCodes);
		}

	TInt prefixLength = (KGifConversionTableSize + 1) * sizeof(TInt16);
	Mem::Fill(iPrefixIndex, prefixLength, TChar(static_cast<TUint>(-1)));

	iNextFree = ResetTableL();

	iReductionFactor = 0;

	CImageProcessor* imageProc = NULL;

	const TSize destinationSize(aDestination.SizeInPixels());
	
	if(iUseFrameSizeInPixels)
		{
		iReductionFactor = ReductionFactor(iFrameInfo->iFrameSizeInPixels,destinationSize);
		}
	else
		{
		iReductionFactor = ReductionFactor(iFrameInfo->iOverallSizeInPixels,destinationSize);
		}

    const TDisplayMode destMode = aDestination.DisplayMode();
    
    iFast64kMode = (iFastDecode &&
                    destMode == EColor64K && 
                    iReductionFactor == 0 && 
                    !iGifImageDesc->iInterlaced
                    );
                    
    if (iFast64kMode && i64KPalette==NULL)
        {
        i64KPalette = new (ELeave) T64KPixel[KGifColorTableMaxEntries];
        Mem::FillZ(i64KPalette, KGifColorTableMaxEntries*sizeof(T64KPixel) );
        }
        
	
	if (!iGifImageDesc->iInterlaced && iReductionFactor==0 &&
	        (iFast64kMode || destMode == EColor16M || destMode == EColor16MU || destMode == EColor16MA) )
	    {
	    if (destMode == EColor16M) 
	    	{
	    	imageProc   = CRawImageUtilProcessor::NewL();
	    	}
		else
			{
    		imageProc   = CRawImageProcessor::NewL();
			}
	    }
    
    if (iFast64kMode)
        {
		// calculate fast 64K conversion palette		
        TInt idx = 0;
        do
            {
            i64KPalette[idx] = iPalette[idx]._Color64K();
            } 
            while (++idx < KGifColorTableMaxEntries);
        }
        
    if (imageProc == NULL)
        {
     	imageProc = ImageProcessorUtility::NewImageProcessorL(aDestination, iReductionFactor, iFast64kMode? EColor64K:ERgb, aDisableErrorDiffusion);    
        }

	SetImageProcessor(imageProc);
	iImgProc = imageProc;

	iPositionOffset = TPoint(iFrameCoords.iTl.iX >> iReductionFactor,iFrameCoords.iTl.iY >> iReductionFactor);

	TRect imageRegion(iPositionOffset + iFrameOffset,iPositionOffset + iFrameOffset + iFrameSize);
	imageProc->PrepareL(aDestination,imageRegion);
	imageProc->SetPos(iPositionOffset + iFrameOffset);
	SetPos(iFrameOffset);

	TInt lineLength	= (TDisplayModeUtils::NumDisplayModeBitsPerPixel( aDestination.DisplayMode() ) * aDestination.SizeInPixels().iWidth) >> 3;

	
	iFastAccessMode = (iReductionFactor == 0 && 
						!iGifImageDesc->iInterlaced && 
						imageRegion.Size() == aDestination.SizeInPixels() && 
						lineLength == aDestination.ScanLineLength(aDestination.SizeInPixels().iWidth, aDestination.DisplayMode())
						); // for direct access to bitmap buffer line length must be aligned to 4 bytes boundary

	
	if (iGifImageDesc->iInterlaced)
		{
		imageProc->SetYPosIncrement(iYPosIncrement);
		if(!iGifImageControl || (iGifImageControl->iTransparentColorIndex==KErrNotFound))
			{
			imageProc->SetLineRepeat(KPass1LineRepeat);
			}
		}

	CImageProcessor* maskProc = NULL;
	SetMaskProcessor(NULL);
	iMaskProc = NULL;

	if(aDestinationMask != NULL)
		{

		ASSERT(aDestinationMask->SizeInPixels() == aDestination.SizeInPixels());
			// this should be trapped at CImageDecoder::Convert()

		if (iGifImageControl && (iGifImageControl->iTransparentColorIndex != KErrNotFound) && aDestinationMask->Handle())
			{
			ASSERT(MaskProcessor()==NULL);

			if (iMaskBuffer == NULL || CurrentFrame() == 0)
			    {
			    delete [] iMaskBuffer; 
				iMaskBuffer     = new (ELeave) TUint32 [iPixBufferSize];
			    }
		
			iOpaqueMask	= 0xFF;// default for 256 mask
			//Create a mask processor  and disable error diffusion
			if (iGifImageDesc->iInterlaced || iReductionFactor > 0)
				{
				maskProc = ImageProcessorUtility::NewImageProcessorL(*aDestinationMask, iReductionFactor, EGray2, ETrue);	
				}
			else
				{
				// use optimized processor for non interlaced mask
				maskProc = CRawImageUtilProcessor::NewL();
				if(aDestinationMask->DisplayMode() == EGray2)
					{
					iOpaqueMask = 1;
					}
				}
			SetMaskProcessor(maskProc);
			iMaskProc = maskProc;
			maskProc->PrepareL(*aDestinationMask,imageRegion);
			if (iGifImageDesc->iInterlaced)
				{
				maskProc->SetYPosIncrement(iYPosIncrement);
				}
			// fill mask with black, so by default unknown parts don't draw
			ClearBitmapL(*aDestinationMask,KRgbBlack);
			maskProc->SetPos(iPositionOffset + iFrameOffset);

			lineLength	= (TDisplayModeUtils::NumDisplayModeBitsPerPixel( aDestinationMask->DisplayMode() ) * aDestinationMask->SizeInPixels().iWidth) >> 3;
			
			iFastAccessMode = iFastAccessMode && lineLength == aDestinationMask->ScanLineLength(aDestinationMask->SizeInPixels().iWidth, aDestinationMask->DisplayMode()); // for direct access 
			// to bitmap buffer line length must be aligned to 4 bytes boundary
				
			}
		}
    else
		{
		if(destMode == EColor16MA && iGifImageControl && iGifImageControl->iTransparentColorIndex != KErrNotFound)
			{
			if (iTranspColIdx != KTranspColIdxNotPresent)
				{
				const_cast<TRgb*>(&iPalette[iTranspColIdx])->SetAlpha(0);
				}
			}
		delete [] iMaskBuffer;
		iMaskBuffer = NULL;
		}

	if ( iGifImageControl && (iGifImageControl->iDisposalMethod == TGifImageControl::ERestoreToBackground) ||
		maskProc==NULL)
		{
		// Clear to background colour if requested or if we have no mask, so that
		// on streamed partial decodes are background
		ClearBitmapL(aDestination,iFrameInfo->iBackgroundColor);
		}

	}

void CGifReadCodec::SetUseFrameSizeInPixels(TBool aUseFrameSizeInPixels)
	{
	iUseFrameSizeInPixels = aUseFrameSizeInPixels;
	}


void CGifReadCodec::DoProcessInfoL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit)
	{
	if(!iReadingExtensionBlock)
		{
		iBlockId = aDataPtr[0];
		while (!iBlockId)
			{
			aDataPtr++;
			if (aDataPtr >= aDataPtrLimit)
				{
				User::Leave(KErrUnderflow);
				}
			
			iBlockId = aDataPtr[0];
			}
		}

	while (iBlockId == KGifExtensionId || iReadingExtensionBlock)
		{
		iReadingExtensionBlock = ETrue;
		DoProcessExtensionL(aDataPtr,aDataPtrLimit);
		iReadingExtensionBlock = EFalse;
		
		// we skip empty blocks here
		while( aDataPtr < aDataPtrLimit && (iBlockId = *aDataPtr) == 0)
		    {
		    ++aDataPtr;
		    }

		if (aDataPtr >= aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}
		}

	if (iBlockId == KGifImageDescriptorId)
		{
		DoProcessImageDescriptorL(aDataPtr,aDataPtrLimit);
		}
	else if (iBlockId == KGifTerminatorId)
		{
		User::Leave(KErrCompletion);
		}
	else
		{
		User::Leave(KErrCorrupt);
		}
	}

void CGifReadCodec::DoProcessExtensionL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit)
	{
	// Make sure we leave a block id for DoProcessInfoL()
	aDataPtrLimit--;

	TInt blockLength;
	TInt dataPtrVal;
	const TUint8* dataPtr = aDataPtr;
	if (iReadingCommentExtensionBlock)
		{
		if ((aDataPtr + 2) > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}

		dataPtrVal = KGifCommentExtensionId;
		blockLength = aDataPtr[0];

		if ((aDataPtr + blockLength + 2) > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}
		}
	else if (iReadingOtherExtensionBlock)
		{
		if ((aDataPtr + 2) > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}

		dataPtrVal = KGifApplicationExtensionId;
		blockLength = aDataPtr[0];

		if ((aDataPtr + blockLength + 2) > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}
		}
	else
		{
		// we start reading block, there should be at least 3 bytes - id, ext. id, length
		if ((aDataPtr + KGifExtBlkHeaderSize) > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}
			
		++dataPtr;
		dataPtrVal  = *dataPtr; // get the ext. ID
		++dataPtr;
		blockLength = *dataPtr;
		
		// ensure that we've got all the block data in the buffer
		// that's possible since max. block length for GIF is 255 bytes
		if ((dataPtr + blockLength) > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}
		}

	switch (dataPtrVal)
		{
	case KGifApplicationExtensionId:
	case KGifPlainTextExtensionId:
		{
		aDataPtr = dataPtr;
		iReadingOtherExtensionBlock = ETrue;

		while (blockLength != 0) 
		    {
			if (dataPtr + blockLength >= aDataPtrLimit)
				{
				aDataPtr = dataPtr;
				User::Leave(KErrUnderflow);
				}
		    if (iReadingLoopIterationExtensionBlock)
                {
                TInt loopIterations = dataPtr[2] + (dataPtr[3] << 8);
                TGifLoopIterations* iterationsDataBlock = new(ELeave) TGifLoopIterations;
                CleanupStack::PushL(iterationsDataBlock);
                iterationsDataBlock->iLoopIterations = loopIterations;
                User::LeaveIfError(iFrameData->AppendImageData(iterationsDataBlock));
                CleanupStack::Pop(iterationsDataBlock);
                iReadingLoopIterationExtensionBlock = EFalse;
                }
            else if (blockLength == KAppIdBlockLength)
                {
                TPtrC8 appId(dataPtr + 1, KAppIdLength);
                TPtrC8 appAuthenticCode(dataPtr + 1 + KAppIdLength, KAppAuthenticCodeLength);
                if ((appId == KAppIdNetscape) && (appAuthenticCode == KAppAuthenticCode2_0))
                    {
                    iReadingLoopIterationExtensionBlock = ETrue;
                    }
                }

			dataPtr += blockLength + 1;
			if (dataPtr >= aDataPtrLimit)
			    {
			    User::Leave(KErrUnderflow);
			    }
			blockLength = dataPtr[0];
			} 

		iReadingOtherExtensionBlock = EFalse;
		aDataPtr = dataPtr;
		}
		break;

	case KGifCommentExtensionId:
		{ // The problem here is a Block header is expected when were actually half way through the block!!!!!!!
		if (!iReadingCommentExtensionBlock)
			{
			if(iCommentIndex < KSetCommentsLimit)
				{
    		    ASSERT(iComment.Count()==iCommentIndex);
	            HBufC8* comment = HBufC8::NewL(blockLength);
    			CleanupStack::PushL(comment);
    			User::LeaveIfError(iComment.Append(comment));
    			CleanupStack::Pop(comment);
				}
			iReadingCommentExtensionBlock = ETrue;
			}
		else
			{
			ASSERT(iComment.Count()==iCommentIndex+1);
			HBufC8* comment = iComment[iCommentIndex];
			iComment[iCommentIndex] = comment->ReAllocL(comment->Length()+blockLength);
			}
			
		TPtr8 commentPtr(NULL,0);
		if(iCommentIndex < KSetCommentsLimit)
			{
	 		commentPtr.Set( iComment[iCommentIndex]->Des() );
			}
		while (blockLength != 0)
			{
			// Append the block.
			TPtrC8 newBlock(&dataPtr[1], blockLength);
			if(iCommentIndex < KSetCommentsLimit)
				{
				commentPtr.Append(newBlock);
				}

			// Calculate the new block/comment length.
			dataPtr += blockLength + 1;
			if(dataPtr > aDataPtrLimit)
				{
				aDataPtr = dataPtr;
				User::Leave(KErrUnderflow);
				}
			blockLength = dataPtr[0];

			// Leave if the data for the new block is not present.
			if (blockLength && ((dataPtr+blockLength+1) >= aDataPtrLimit))
				{
				aDataPtr = dataPtr;
				User::Leave(KErrUnderflow);
				}

			// Re-allocate the comment, if there is another block.
			if (blockLength != 0&&(iCommentIndex < KSetCommentsLimit))
				{
				HBufC8* comment = iComment[iCommentIndex];
				iComment[iCommentIndex] = comment->ReAllocL(comment->Length()+blockLength);
				commentPtr.Set(iComment[iCommentIndex]->Des());
				}
			}

		iCommentIndex++;

		aDataPtr = dataPtr;
		}

		iReadingCommentExtensionBlock = EFalse;
		break;

	case KGifGraphicControlExtensionId:
		if (aDataPtr + blockLength + KGifExtBlkHeaderSize + 1 >= aDataPtrLimit)
	        {
	        User::Leave(KErrUnderflow);
	        }
	    
	    aDataPtr += (KGifExtBlkHeaderSize + blockLength);
	    // we ignore malformed extension block
	    if (blockLength < KMinGifGraphicControlBlkLen)
	        {
	        break;
	        }

		++dataPtr; //go into block data
		TUint8 flags = *dataPtr++;
		
		TGifImageControl* gifImageControl = new(ELeave) TGifImageControl;
		CleanupStack::PushL(gifImageControl);

		gifImageControl->iDelayTimeInCentiseconds = *dataPtr++;
		gifImageControl->iDelayTimeInCentiseconds |= (*dataPtr++) << 8;
		if (flags & 0x01)
			gifImageControl->iTransparentColorIndex = *dataPtr++;
		else
			{
			gifImageControl->iTransparentColorIndex = KErrNotFound;
			dataPtr++;
			}

		gifImageControl->iDisposalMethod = TGifImageControl::TDisposalMethod((flags & 0x1c) >> 2);
		gifImageControl->iUserInputFlag = (flags & 0x02) != 0;
	
		User::LeaveIfError(iFrameData->AppendFrameData(gifImageControl));

		CleanupStack::Pop(); // gifImageControl
		iFrameImageControl = gifImageControl;
		break;
		}

	if (aDataPtr[0]!=0)
		{
		User::Leave(KErrCorrupt);
		}
		
	aDataPtr++; // Absorb trailing zero
	}

void CGifReadCodec::DoProcessImageDescriptorL(const TUint8*& aDataPtr,const TUint8* aDataPtrLimit)
	{
	if (iFrameImageDesc != NULL)
		{
		User::Leave(KErrCorrupt);
		}

	if (aDataPtr + KGifImageInformationSize + KGifInitialCodeLengthSize > aDataPtrLimit)
		{
		User::Leave(KErrUnderflow);
		}

	TPoint topLeftCorner;
	topLeftCorner.iX = aDataPtr[1] + (aDataPtr[2] << 8);
	topLeftCorner.iY = aDataPtr[3] + (aDataPtr[4] << 8);

	TSize imageSize;
	imageSize.iWidth = aDataPtr[5] + (aDataPtr[6] << 8);
	imageSize.iHeight = aDataPtr[7] + (aDataPtr[8] << 8);

	//Save the first frame's real dimensions, but use the complete
	//image size for the first frame
	if(CurrentFrame()==0)
		{
		iFirstFrameSize = imageSize;
		// TPoint (0,0) means that iFrameOffset is set to 0 as all frames are now treated the same for offset data
		// But real (firstframe) imageSize is saved here as clients use first frames overallsizeinPixels
		iFirstFrameCoords.SetRect(TPoint(0,0),imageSize);
		const TSize screenSize(iScreenSize);
		// there is leniency here for rogue gifs with first frames bigger than global screen size from gif header
		TSize lenientImageSize = TSize(Max(imageSize.iWidth,screenSize.iWidth),
				Max(imageSize.iHeight,screenSize.iHeight));	
		iFrameInfo->iOverallSizeInPixels = lenientImageSize; 
		//if the first frame has an offset, put it in the frame info
		// this will be translated into the iPositionOffset variable
		iFrameInfo->iFrameCoordsInPixels.SetRect(topLeftCorner,lenientImageSize);
		}
	else
		{
		iFrameInfo->iOverallSizeInPixels = imageSize;
		iFrameInfo->iFrameCoordsInPixels.SetRect(topLeftCorner,imageSize);
		}
	iFrameInfo->iFrameSizeInPixels = imageSize;
	
	//adjust iFrameCoordsInPixels for proper behaviour
	if(iUseFrameSizeInPixels)
		{
		iFrameInfo->iFrameCoordsInPixels.SetSize(iFrameInfo->iFrameSizeInPixels);
		}

	TUint8 flags = aDataPtr[9];

	TGifImageDescriptor* gifImageDesc = new(ELeave) TGifImageDescriptor;
	CleanupStack::PushL(gifImageDesc);

	gifImageDesc->iLocalColorMap = flags & 0x80;
	gifImageDesc->iInterlaced = flags & 0x40;
	gifImageDesc->iSortedLocalMap = flags & 0x20;
	if (gifImageDesc->iLocalColorMap)
		{
		iFrameInfo->iBitsPerPixel = (flags & 0x07) + 1;
		TInt paletteEntries = 1 << iFrameInfo->iBitsPerPixel;
		TInt paletteBytes = paletteEntries * KGifPaletteEntrySize;
		if (aDataPtr + KGifImageInformationSize + paletteBytes + KGifInitialCodeLengthSize > aDataPtrLimit)
			{
			User::Leave(KErrUnderflow);
			}

		aDataPtr += KGifImageInformationSize;
		const TUint8* paletteLimit = aDataPtr + paletteBytes;

		TGifColorTable* gifColorTable = new(ELeave) TGifColorTable;
		CleanupStack::PushL(gifColorTable);
		TRgb* rgbPtr = gifColorTable->iPalette;
		while (aDataPtr < paletteLimit)
			{
			*rgbPtr++ = TRgb(aDataPtr[0],aDataPtr[1],aDataPtr[2]);
			aDataPtr += 3;
			}
		User::LeaveIfError(iFrameData->AppendFrameData(gifColorTable));
		CleanupStack::Pop(gifColorTable);
		}
	else
		{
		aDataPtr += KGifImageInformationSize;
		}

	User::LeaveIfError(iFrameData->AppendFrameData(gifImageDesc));
	CleanupStack::Pop(gifImageDesc);
	
	TGifLZWInfo* gifLZWInfo = new(ELeave) TGifLZWInfo;
	CleanupStack::PushL(gifLZWInfo);

	gifLZWInfo->iInitialCodeLength = aDataPtr[0] + 1;
	User::LeaveIfError(iFrameData->AppendFrameData(gifLZWInfo));
	aDataPtr++;

	CleanupStack::Pop(gifLZWInfo);
	iFrameImageDesc = gifImageDesc;
	iFrameLZWInfo = gifLZWInfo;
	}

void CGifReadCodec::DoSaveCommentsL()
	{
	//If we have comments add it
	TInt noOfComments = iComment.Count();
	//If we have more than KSetCommentsLimit comments just restrict them as KSetCommentsLimit
	if (noOfComments > KSetCommentsLimit)
		{
		noOfComments = KSetCommentsLimit;
		}
	for(TInt commentNo=0; commentNo<noOfComments; commentNo++)
		{
		TGifComment* comment = new(ELeave) TGifComment;
		CleanupStack::PushL(comment);
		comment->iComment = iComment[0];
		User::LeaveIfError(iFrameData->AppendImageData(comment));
		CleanupStack::Pop(); // comment

		User::LeaveIfError(iFrameData->AppendImageBuffer(iComment[0]));
		iComment.Remove(0);
		}
	}

void CGifReadCodec::DoProcessDataL()
	{
	TInt nextFree = iNextFree;
	TInt prevCode = iPreviousCode;
	TBitBuffer bBuf = iBitBuffer;
	TInt bbSize = iBitBuffSize;
	TInt curCodeLen = iCurrentCodeLength;
	iLatestPixSize = iFast64kMode ? sizeof(TUint16) : sizeof (TRgb);
	
	do
		{
		TInt code = NextCode(bBuf, bbSize, curCodeLen);
		if (code == KErrNotFound)  
			{
			break; // not enought data in the buffer to get the code
			}
			
	    if (iTableReset)
			{
			iTableReset = EFalse;
			WriteCodeL(code);
			prevCode    = code;
			}
		else if (code == iClearCode)
			{
			nextFree    = ResetTableL();
			curCodeLen  = iCurrentCodeLength;
			iTableReset = ETrue;
			}
		else if (code == iEoiCode)
			{
			iComplete = ETrue;
			break;
			}
		else
			{
			if (prevCode >= nextFree)
				{
				User::Leave(KErrCorrupt);
				}
				
			if ( (nextFree == (1 << curCodeLen)-1) &&  curCodeLen < KGifMaxBits)
				{
				++curCodeLen;
				}
				
			if (code < nextFree)
				{
				WriteCodeL(code);
				if (prevCode != -1)
					{					
				    iPrefixIndex[nextFree] = STATIC_CAST(TInt16,prevCode);
				    iSuffixCode[nextFree] = iFirstChar;
				    if (nextFree < KGifConversionTableSize )
				        {
				        nextFree++;    
				        }
					}
				}
			else
				{
				iPrefixIndex[nextFree] = STATIC_CAST(TInt16,prevCode);
				iSuffixCode[nextFree] = iFirstChar;
				WriteCodeL(nextFree);
			    if (nextFree < KGifConversionTableSize )
			        {
			        nextFree++;    
			        }
				}
				
			if(code < nextFree)
				{
				prevCode = code;
				}
			else
				{
				if(nextFree <= 1)
					{
					User::Leave(KErrCorrupt);
					}
				prevCode = iPrefixIndex[nextFree-1];
				}
			
			}
		} 
		while (iDataPtr <= iDataPtrLimit);
		
	ASSERT(iDataPtr <= iDataPtrLimit);
	
	SetCurrentCodeLengthL(curCodeLen);
	iBitBuffer      = bBuf;
	iBitBuffSize    = bbSize;
	iNextFree       = nextFree;
	iPreviousCode   = prevCode;
	}

inline
TInt CGifReadCodec::ResetTableL()
	{
	const TInt nFree = iEoiCode + 1;
	
	SetCurrentCodeLengthL(iGifLZWInfo->iInitialCodeLength);
	return nFree;
	}
	
#if defined(__ARMCC__)
__forceinline
#else
inline 
#endif
TInt CGifReadCodec::NextCode(TBitBuffer& aBitBuffer, TInt& aBitBufSize,const TInt aCurCodeLen )
	{
	
	if (aCurCodeLen > aBitBufSize)
	    {
	    const TUint8* dataPtr = iDataPtr;
	    ASSERT( dataPtr <= iDataPtrLimit );
	    
	    TInt bytesToRead = (sizeof(TBitBuffer) * 8 - aBitBufSize) >> 3;
	    if (dataPtr + bytesToRead >= iDataPtrLimit)
	        {
	        bytesToRead = iDataPtrLimit - dataPtr;
	        if (bytesToRead == 0)
	            {
	            return KErrNotFound;
	            }
	        }
	    
	    do
	        {
	        aBitBuffer |= *dataPtr++ << aBitBufSize;
	        aBitBufSize +=8;
	        } 
	        while (--bytesToRead > 0);
	    
	    iDataPtr    = dataPtr;
	    if (aBitBufSize < aCurCodeLen)
	        {
	        return KErrNotFound;
	        }

	    }
    TInt code   = aBitBuffer;
    aBitBuffer >>= aCurCodeLen;
    code &= (1 << aCurCodeLen) - 1;
    aBitBufSize -= aCurCodeLen;

    return code;
	}

template <class TPalType, TInt aPtrDelta>
inline 
TUint8* CGifReadCodec::WriteGifBuffer(TUint8* aOutputStringPtr, TUint8* aOutputStringLimit)
	{
	TInt bufUsed = iPixBufCount;
	TInt bufFree = iPixBufferSize - bufUsed;
	
	if ((aOutputStringLimit-aOutputStringPtr) > bufFree) 
		{
		aOutputStringLimit = aOutputStringPtr+bufFree;
		}
	
	register TPalType* pixelBufferPtr = reinterpret_cast<TPalType*>( iPixelBuffer ) + bufUsed;
	const TPalType* tmp = pixelBufferPtr;
	const TPalType* palette;

	if (aPtrDelta == 4)
		{
		palette = reinterpret_cast<const TPalType*>( iPalette );
		}
	else
		{
		palette = reinterpret_cast<const TPalType*>( i64KPalette );	
		}

	if (iMaskProc)
		{
		register TUint32* maskPixelBufferPtr = iMaskBuffer + bufUsed;
		const TUint16 KTranspIdx= iTranspColIdx;
		const TUint32 KTranspMask = 0;
    	const TUint32 KOpaqueMask = iOpaqueMask;
		const TPalType bgCol    = palette[ KTranspIdx ];
			
		while (aOutputStringPtr < aOutputStringLimit)
			{ 
			const TUint8 code = *aOutputStringPtr;
			if (code == KTranspIdx)
				{
				*pixelBufferPtr = bgCol; 
				*maskPixelBufferPtr = KTranspMask;
				}
			else 
				{
				*pixelBufferPtr = palette[code]; 
				*maskPixelBufferPtr = KOpaqueMask;
				}
			maskPixelBufferPtr++;
			pixelBufferPtr++;
			aOutputStringPtr++;
			}
		}
	else
		{

		while (aOutputStringPtr < aOutputStringLimit)
			{ 
			*pixelBufferPtr = palette[*aOutputStringPtr]; 
			pixelBufferPtr++;
			aOutputStringPtr++;
			}
		}                                          

	iPixBufCount += pixelBufferPtr-tmp;
	return aOutputStringPtr;	
	}

/**
    That shall only leave with KErrCorrupt
    which means that image decoding is not possible
*/
void CGifReadCodec::WriteCodeL(TInt aCode)
	{
	TUint8* outputStringLimit = iOutputString + KGifConversionTableSize;
	register TUint8* outputStringPtr = outputStringLimit;
	register TUint16 latestChar=0;

	while (aCode >= 0)
		{
		ASSERT((aCode >= 0) && (aCode <= KGifConversionTableSize));
		
		outputStringPtr--;
		latestChar = iSuffixCode[aCode];
		outputStringPtr[0] = latestChar;
		aCode = iPrefixIndex[aCode];
		}                     

	iFirstChar = latestChar; 
	if (aCode != KErrNotFound)
		{
		User::Leave(KErrCorrupt);
		}

	while (true) 
		{
		if (iFast64kMode)
	    	{
	    	outputStringPtr = WriteGifBuffer<TUint16, sizeof(TUint16)>(outputStringPtr, outputStringLimit);
	    	}
		else
	    	{
	    	outputStringPtr = WriteGifBuffer<TRgb, sizeof(TRgb)>(outputStringPtr, outputStringLimit);    
	    	}
	
    	if (outputStringPtr != outputStringLimit)
        	{
			FlushPixBuffer(iLatestPixSize);
        	}
        else
        	{
        	break;	
        	}
		}
	}

inline
void CGifReadCodec::FlushPixBuffer(TInt aPixSize)
    {
    if (iPixBufCount)
        {
        if (iMaskProc)        	
            {
            WriteStringWithTransparency(reinterpret_cast<TUint8*>(iPixelBuffer), aPixSize, iMaskBuffer, iPixBufCount);
            }        
        else
            {
            WriteStringWithoutTransparency(reinterpret_cast<TUint8*>(iPixelBuffer), aPixSize, iPixBufCount);
            }
        
        iPixBufCount = 0;
        }    
    }

void CGifReadCodec::WriteStringWithoutTransparency(TUint8* aOutputString, TInt aPixSize, TInt aNumOfPixels)
	{
	CImageProcessor*const imageProc = iImgProc;

	if (iFastAccessMode) 
		{
		imageProc->SetPixels(reinterpret_cast<TRgb*>(aOutputString), aNumOfPixels);
		iPixRead += aNumOfPixels;
		aNumOfPixels = 0;
		return;
		}

	const TInt KImgXLimit = iFrameSize.iWidth + iFrameOffset.iX;
	
	while ( aNumOfPixels )
		{
		TInt rowPixels = Min(KImgXLimit - iPPos->iX, aNumOfPixels);

		imageProc->SetPixels(reinterpret_cast<TRgb*>(aOutputString), rowPixels);
		aNumOfPixels    -= rowPixels;
		aOutputString   += rowPixels * aPixSize;

		iPPos->iX += rowPixels;
		if (iPPos->iX == KImgXLimit)
			{
			imageProc->FlushPixels();
			UpdateYPos();
			imageProc->SetPos( TPoint(iPPos->iX+iPositionOffset.iX, iPPos->iY+iPositionOffset.iY) );
			}
		}
	}

void CGifReadCodec::WriteStringWithTransparency(TUint8* aOutputString, TInt aPixSize, TUint32* aMaskString, TInt aNumOfPixels)
	{
	CImageProcessor*const imageProc = iImgProc;
	CImageProcessor*const maskProc  = iMaskProc;
	
	if (iFastAccessMode) 
		{
		imageProc->SetPixels(reinterpret_cast<TRgb*>(aOutputString), aNumOfPixels);
		if(maskProc) // For non-single 16MA bitmaps
			{
			maskProc->SetMonoPixels(reinterpret_cast<TUint32*>(aMaskString), aNumOfPixels);	
			}
		
		iPixRead += aNumOfPixels;
		aNumOfPixels = 0;
		return;
		}

	const TInt KImgXLimit = iFrameSize.iWidth + iFrameOffset.iX;
	
	while ( aNumOfPixels )
		{
		TInt rowPixels = Min(KImgXLimit - iPPos->iX, aNumOfPixels);

		imageProc->SetPixels(reinterpret_cast<TRgb*>(aOutputString), rowPixels);
		maskProc->SetMonoPixels(reinterpret_cast<TUint32*>(aMaskString), rowPixels);

		iPPos->iX       += rowPixels;
		aOutputString   += rowPixels * aPixSize;
		aMaskString     += rowPixels;
		aNumOfPixels    -= rowPixels;

		if (iPPos->iX == KImgXLimit)
			{
			imageProc->FlushPixels();
			UpdateYPos();
			const TPoint newPos(iPPos->iX+iPositionOffset.iX, iPPos->iY+iPositionOffset.iY);
			imageProc->SetPos(newPos);
			maskProc->SetPos(newPos);
			}
		}
	}

inline
void CGifReadCodec::SetCurrentCodeLengthL(TInt aCodeLength)
	{
	if ((aCodeLength > KGifMaxBits) || (aCodeLength < 0))
		{
		User::Leave(KErrCorrupt);
		}
	iCurrentCodeLength = aCodeLength;
	}

inline
void CGifReadCodec::UpdateYPos()
	{
	iPPos->iX = iFrameOffset.iX;

	if (!iGifImageDesc->iInterlaced)
		{
		iPPos->iY++;
		return;
		}
		
	UpdateYPosInterlaced();
	}

void CGifReadCodec::UpdateYPosInterlaced()
    {
    iPPos->iY += iYPosIncrement;

	CImageProcessor*const imageProc = ImageProcessor();
	while (iPPos->iY >= iFrameSize.iHeight + iFrameOffset.iY)
		{
		iPass++;
		TInt lineRepeat;
		if (iPass == 2)
			{
			iPPos->iY = iFrameOffset.iY + KPass2StartLine;
			iYPosIncrement = KPass2YPosIncrement;
			lineRepeat = KPass2LineRepeat;
			}
		else if (iPass == 3)
			{
			iPPos->iY = iFrameOffset.iY + KPass3StartLine;
			iYPosIncrement = KPass3YPosIncrement;
			lineRepeat = KPass3LineRepeat;
			}
		else if (iPass == 4)
			{
			iPPos->iY = iFrameOffset.iY + KPass4StartLine;
			iYPosIncrement = KPass4YPosIncrement;
			lineRepeat = KPass4LineRepeat;
			}
		else
			break;
		imageProc->SetYPosIncrement(iYPosIncrement);
		if(!iGifImageControl || (iGifImageControl->iTransparentColorIndex==KErrNotFound))
		    {
		    imageProc->SetLineRepeat(lineRepeat);
		    }
		}
	}

// CGifWriteCodec
CGifWriteCodec* CGifWriteCodec::NewL(const CGifEncoder& aEncoder)
	{
	CGifWriteCodec* self = new(ELeave) CGifWriteCodec(aEncoder);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}

CGifWriteCodec::CGifWriteCodec(const CGifEncoder& aEncoder)
	:iEncoder(aEncoder)
	{
	}

TFrameState CGifWriteCodec::ProcessFrameL(TBufPtr8& aDst)
	{
	iDestStartPtr = CONST_CAST(TUint8*,aDst.Ptr());
	iDestPtr = iDestStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	FillBufferL(*Source());

	if (iImageComplete)
		{
		while (iBufferSize > 0)
			{
			if (!WriteBuffer())
				{
				aDst.SetLength(iDestPtr - iDestStartPtr);
				return EFrameIncomplete;
				}
			}
		TBool terminatorWritten = WriteTerminator();
		aDst.SetLength(iDestPtr - iDestStartPtr);
		
		return (terminatorWritten) ? EFrameComplete : EFrameIncomplete;
		}
	else
		{
		WriteBuffer();
		}

	aDst.SetLength(iDestPtr - iDestStartPtr);
	return EFrameIncomplete;
	}

void CGifWriteCodec::FillBufferL(const CFbsBitmap& aFrame)
	{
	TUint pixelsScanned =0;
	TBool pixelWritten = EFalse;

	const CPalette* palette = iEncoder.Palette();
	while (iPos.iY < iSourceRect.iBr.iY)
		{
		TRgb pixelColor;
		aFrame.GetPixel(pixelColor,iPos);
		iPos.iX++;
		if (iPos.iX == iSourceRect.iBr.iX)
			{
			iPos.iX = iSourceRect.iTl.iX;
			iPos.iY++;
			}
		TUint8 nextColorIndex;
		if (palette)
			{
			nextColorIndex = palette->NearestIndex(pixelColor);	
			}
		else
			{
			nextColorIndex = STATIC_CAST(TUint8, pixelColor.Color256());
			}

		TUint32 hashValue=0;
		TInt index=-1;
		if (!TableEntry(iWaitingCode,nextColorIndex,index,hashValue))
			{
			if(index!=KErrNotFound)
				{
				pixelWritten = ETrue;
				WriteData(iWaitingCode);
				AddToTableL(index,hashValue);

				if (iNextFree > iNextLimit)
					{
					iCodeLength++;
					if (iCodeLength > KGifMaxBits)
						{
						iCodeLength=KGifMaxBits;
						ResetTable();
						}
					iNextLimit = 1 << iCodeLength;
					}
				}
			iWaitingCode = nextColorIndex;
			}
		else
			{
			iWaitingCode = iHashCode[index];	
			}

		if ((iBufferFull) || ((pixelsScanned++ > KLZWLimit) && pixelWritten !=EFalse))
		{
			return;
		}

	}


	if (iPos.iY == iSourceRect.iBr.iY)
		{
		if (iWaitingCode != KErrNotFound)
			{
			WriteData(iWaitingCode);
			}
		WriteData(iClearCode + 1); // End-Of-Information
		if (iBitOffset > 0)
			{
			iBufferSize++;
			iBitOffset = 0;
			}
		iCodeLength = 8;
		iBufferPtr++;
		iImageComplete = ETrue;
		}
	}

void CGifWriteCodec::InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource)
	{
	SetSource(&aSource);

	iDestStartPtr = CONST_CAST(TUint8*,aDst.Ptr());
	iDestPtr = iDestStartPtr;
	iDestPtrLimit = iDestPtr + aDst.MaxLength();

	iSourceRect = TRect(aSource.SizeInPixels());
	iPos.SetXY(0,0);

	iBitsPerPixel = 8;

	iBufferPtr = iBuffer;
	iBufferSize = 0;

	iClearCode = 1 << iBitsPerPixel;
	iCodeLength = iBitsPerPixel + 1;
	iWaitingCode = -1;

	ResetTable();

	*iDestPtr++ = KGifImageDescriptorId; // Image Separator Header
	*iDestPtr++ = 0; // Coordinate Left Border
	*iDestPtr++ = 0; // Coordinate Left Border
	*iDestPtr++ = 0; // Coordinate Top Border
	*iDestPtr++ = 0; // Coordinate Top Border
	*iDestPtr++ = TUint8(iSourceRect.Width());
	*iDestPtr++ = TUint8(iSourceRect.Width() >> 8);
	*iDestPtr++ = TUint8(iSourceRect.Height());
	*iDestPtr++ = TUint8(iSourceRect.Height() >> 8);
	*iDestPtr++ = 0; // Flags

	*iDestPtr++ = TUint8(iCodeLength - 1); // Initial Code Length - 1

	aDst.SetLength(iDestPtr - iDestStartPtr);
	}

void CGifWriteCodec::AddToTableL(TInt aIndex, TUint32 aHashValue)
	{
	ASSERT(aIndex<KGifHashTableSize);
	ASSERT(iNextFree<=KGifConversionTableSize);
	iHashCode[aIndex] = static_cast<TInt16>(iNextFree++);
	iHashValue[aIndex] = aHashValue;
	}

// The hash algoritm use open addressing double hashing (no chaining)
// on the prefix code / suffix character combination. A variant of Knuth's
// algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
// secondary probe.
TBool CGifWriteCodec::TableEntry(TInt16 aWaitingCode, TUint8 aNewCode, TInt& aIndex, TUint32& aHashValue)
	{
	TInt offset;

	ASSERT(aWaitingCode>=KErrNotFound && aWaitingCode<KGifConversionTableSize);

	aIndex = KErrNotFound;

	//Very first code
	if(aWaitingCode==KErrNotFound)
		return EFalse;

	aIndex = static_cast<TInt>(aNewCode<<(KGifMaxBits-8)) + aWaitingCode;
	if(aIndex>=KGifHashTableSize)
		aIndex -= KGifHashTableSize;
	ASSERT(aIndex < KGifHashTableSize);

	//hash function
	aHashValue = static_cast<TUint32>(aWaitingCode<<8)|aNewCode;

	if(iHashCode[aIndex]==KErrNotFound) //slot is empty
		return EFalse;

	//slot has an entry
	if(iHashValue[aIndex]==aHashValue) //symbol already in table
		return ETrue;

	if(aIndex==0)	// secondary hash (after G. Knott)
		offset = 1; 
	else
		offset = KGifHashTableSize - aIndex;

	FOREVER
		{
		aIndex -= offset;
		if(aIndex<0)
			aIndex += KGifHashTableSize;

		if(iHashCode[aIndex]==KErrNotFound)
			return EFalse; //slot is empty

		if(iHashValue[aIndex]==aHashValue) //symbol already in table
			return ETrue;
		}
	}

void CGifWriteCodec::ResetTable()
	{
	WriteData(iClearCode); // Send reset code

	iNextFree = iClearCode + 2;
	iCodeLength = iBitsPerPixel + 1;
	iNextLimit = 1 << iCodeLength;

	for (TInt index = 0; index < KGifHashTableSize; index++)
		iHashCode[index] = KErrNotFound;
	}

void CGifWriteCodec::WriteData(TInt aIndex)
	{
	aIndex <<= iBitOffset;
	iBufferPtr[0] |= TUint8(aIndex);
	iBufferPtr[1] |= TUint8(aIndex >> 8);
	iBufferPtr[2] |= TUint8(aIndex >> 16);

	TInt newBitOffset = iBitOffset + iCodeLength;
	iBitOffset = newBitOffset & 7;
	TInt byteOffset = newBitOffset >> 3;
	iBufferPtr += byteOffset;
	iBufferSize += byteOffset;

	if (iBufferSize >= KGifBlockSize)
		{
		ASSERT(iBufferSize <= KGifBufferSize);
		iBufferFull = ETrue;
		}
	}

TBool CGifWriteCodec::WriteBuffer()
	{
	ASSERT(iBufferSize <= KGifBufferSize);
	if (iBufferSize == 0)
		return ETrue;

	TInt blockSize = Min(KGifBlockSize,iBufferSize);
	if (iDestPtrLimit - iDestPtr < blockSize + 1)
		return EFalse;

	*iDestPtr++ = STATIC_CAST(TUint8,blockSize);
	Mem::Copy(iDestPtr,iBuffer,blockSize);
	iDestPtr += blockSize;
	iBufferFull = EFalse;

	iBufferSize -= blockSize;
	iBufferPtr = iBuffer + iBufferSize;

	TInt bitBufferSize = iBufferSize;
	if (iBitOffset)
		bitBufferSize++;
	Mem::Copy(iBuffer,&iBuffer[blockSize],bitBufferSize);
	Mem::FillZ(iBuffer + bitBufferSize,KGifBufferSize - bitBufferSize);

	return ETrue;
	}

TBool CGifWriteCodec::WriteTerminator()
	{
	if (iDestPtrLimit - iDestPtr < 2)
		return EFalse;

	*iDestPtr++ = TUint8(0);
	*iDestPtr++ = TUint8(KGifTerminatorId);
	return ETrue;
	}
