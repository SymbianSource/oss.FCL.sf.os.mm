// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalTechnology 
*/

#include "jpgimageframeprocessor.h"
#include <imageframeformats.hrh>
#include <imageframe.h>
#include "JpegConsts.h"
#include "JPEGCodec.h"

const TInt KLayoutIndex0 = 0;
const TInt KLayoutIndex1 = 1;
const TInt KLayoutIndex2 = 2;
const TInt KErrValue	 = -1;

// these are symmetrical values
const TInt KYUVMonoSampleRatio[] = {1, -1};
const TInt KYUV422SampleRatio[] =  {1, -1};
const TInt KYUV420SampleRatio[] =  {1, 2, 2, -1};

CImageFrame& CJpgImageFrameProcessor::ImageFrame() const
	{
	return *iFrame;
	}
	
void CJpgImageFrameProcessor::GetCurrentPosition(TPoint& aPosition) const
	{
	aPosition = iCurrentPosition;
	}
	
void CJpgImageFrameProcessor::SetCurrentPosition(const TPoint& aPosition)
	{
	iCurrentPosition = aPosition;
	}

CJpgImageFrameProcessor::CJpgImageFrameProcessor(CImageFrame& aFrame) : iFrame(&aFrame),
																		iIndexY(KErrValue),
																		iIndexU(KErrValue),
																		iIndexV(KErrValue)
	{
	}
	
CJpgImageFrameProcessor::~CJpgImageFrameProcessor()
	{
	delete iLayout;
	}
	
void CJpgImageFrameProcessor::ConstructL(const TFrameLayoutBase& aLayout)
	{
	iLayout = static_cast<TFrameLayout*>(aLayout.DuplicateL());
	}
	
TBool CJpgImageFrameProcessor::MoreData()
	{
	TSize remain = iMaxPlaneSize - iCurrentPosition;
	return (remain.iHeight > 0 && remain.iWidth > 0);
	}

void CJpgImageFrameProcessor::CalculateCorrections()
	{
	iWCorrection = (iMaxPlaneSize.iWidth - (iCurrentPosition.iX + iBlockSize.iWidth));
	iHCorrection = (iMaxPlaneSize.iHeight - (iCurrentPosition.iY + iBlockSize.iHeight));
	
	//compensations are negative values
	if (iWCorrection > 0)
		{
		iWCorrection = 0;
		}
		
	if (iHCorrection > 0)
		{
		iHCorrection = 0;
		}
	}

void CJpgImageFrameProcessor::CalculateNextBlockStart(TInt aScanLine)
	{
	iCurrentPosition.iX += iBlockSize.iWidth;
	if (iCurrentPosition.iX >= aScanLine)
		{
		// move to the next row
		iCurrentPosition.iX = 0;
		iCurrentPosition.iY += iBlockSize.iHeight;
		}
	}
	
void CJpgImageFrameProcessor::ValidateCurrentBlockL()
	{	
	const TFrameLayout& layout = static_cast<const TFrameLayout&>(iFrame->FrameLayout());
	
	// for all indexes check 
	for (TInt index = 0; index < layout.Planes(); index++)
		{	
		// whether lengths are correct
		if ((layout.Length(index) < layout.CurrentLength(index)) || (layout.CurrentLength(index) < 0) )
			{
			User::Leave(KErrArgument);
			}
			
        // then calculate offsets from start 
		TInt scanLength = layout.ScanLength(index);
	    TInt offset = iCurrentPosition.iY / iVertSampleRatio[index] * scanLength + 
	    			  iCurrentPosition.iX / iHorzSampleRatio[index];  
	    // and check whether the starting point is within the proper range
		if (offset < layout.Length(index))
			{
			iOffsets[index] = offset;
			}
		else 		
			{
			// position is incorrect - exceeding the allocated memory size
			User::Leave(KErrOverflow);
			}
		}	
		
	CalculateCorrections();
	}
	

void CJpgImageFrameProcessor::DoWriteBlock(TUint8* aDest, const TDataUnit& aSrc, 
										   TInt aScanline, TInt aXComp, TInt aYComp)
	{
	for (TInt m = 0, rowStart = 0, i = 0; i < KJpgDCTBlockWidth + aYComp; i++, rowStart += aScanline)
		{
		for (TInt j = 0; j < KJpgDCTBlockWidth + aXComp; j++, m++)
			{
			*(aDest+rowStart+j) = Clip(aSrc.iCoeff[m]);
			}
		m -= aXComp; //compensations are negative values 
		}
	}
	
void CJpgImageFrameProcessor::DoReadBlock(TDataUnit& aDest, const TUint8* aSrc, 
										  TInt aScanline, TInt aXComp, TInt aYComp)										  
	{
	TInt rowStart = 0;
	TInt i;
	TInt j;
	TInt m=0;
	
	for (i = 0; i < KJpgDCTBlockWidth + aYComp; i++, rowStart += aScanline)
		{
		for (j = 0; j < KJpgDCTBlockWidth + aXComp; j++, m++)
			{
			aDest.iCoeff[m]= *(aSrc+rowStart+j);
			}
		// need to repeat the last data if position falls outside of the image
		if (aXComp)
			{
			TInt16 data = aDest.iCoeff[m-1];
			for ( TInt k = j; k < KJpgDCTBlockWidth; k++, m++)
				{
				aDest.iCoeff[m] = data;
				}
			}
		}
		
	// if there a rows otside of the image, repeat the last row which falls in the image
	if (aYComp)
		{
		for (TInt k = i; k < KJpgDCTBlockWidth; k++)
			{
			Mem::Copy(&aDest.iCoeff[(k) * KJpgDCTBlockWidth], 
					  &aDest.iCoeff[(i-1) * KJpgDCTBlockWidth], 
					  KJpgDCTBlockWidth * sizeof(TInt16));	
			}
		}
	}
	
/*********************************Monochrome*******************************************/
CJpgImageFrameYUVMonoProcessor::CJpgImageFrameYUVMonoProcessor(CImageFrame& aFrame) : CJpgImageFrameProcessor(aFrame)
	{
	iIndexY = 0;
	
	iMaxHorzPlaneFactor = 1;
	iMaxVertPlaneFactor = 1;

	iHorzSampleRatio = KYUVMonoSampleRatio;
	iVertSampleRatio = KYUVMonoSampleRatio;

	iBlockSize.iWidth = KJpgDCTBlockWidth * iMaxHorzPlaneFactor;
	iBlockSize.iHeight = KJpgDCTBlockWidth * iMaxVertPlaneFactor;
	
	iMaxPlaneSize = TSize(iMaxHorzPlaneFactor * iFrame->FrameSizeInPixels().iWidth, 
						  iMaxVertPlaneFactor * iFrame->FrameSizeInPixels().iHeight);
	} 
	
TDataUnit* CJpgImageFrameYUVMonoProcessor::ReadBlockL()
	{
	ValidateCurrentBlockL();
		
	TUint8* p = &(iFrame->Data()[iLayout->Start(KLayoutIndex0) + iOffsets[KLayoutIndex0]]);
	
	// set the data to zero 
	Mem::FillZ(&iDataUnit, sizeof(TDataUnit));
	DoReadBlock(iDataUnit[0], p, iLayout->ScanLength(KLayoutIndex0), iWCorrection, iHCorrection);

	CalculateNextBlockStart(iLayout->ScanLength(KLayoutIndex0));
	return iDataUnit;
	}
	
void CJpgImageFrameYUVMonoProcessor::WriteBlockL(const RArray<const TDataUnit*>& aDataUnit)
	{
	ValidateCurrentBlockL();
	
	TUint8* p = &(iFrame->Data()[iLayout->Start(KLayoutIndex0) + iOffsets[KLayoutIndex0]]);
	
	DoWriteBlock(p, *aDataUnit[0], iLayout->ScanLength(KLayoutIndex0), 
				iWCorrection, iHCorrection);
	
	UpdateCurrentLengthL();
					
	CalculateNextBlockStart(iLayout->ScanLength(KLayoutIndex0));
	}

void CJpgImageFrameYUVMonoProcessor::UpdateCurrentLengthL()
	{
	TInt currentLength = iOffsets[KLayoutIndex0] + (iBlockSize.iHeight + iHCorrection - 1) * iLayout->ScanLength(KLayoutIndex0) + iBlockSize.iWidth + iWCorrection;
	iLayout->SetCurrentLength(KLayoutIndex0, currentLength);
    iFrame->SetFrameLayoutL(*iLayout);
	}
	
/********************************CJpgImageFrameYUV422InterleavedProcessor********************/
CJpgImageFrameYUV422InterleavedProcessor::CJpgImageFrameYUV422InterleavedProcessor(CImageFrame& aFrame) : CJpgImageFrameProcessor(aFrame)
	{
	iIndexY = 0;
	iIndexU = 2;
	iIndexV = 3;

	iHorzSampleRatio = KYUV422SampleRatio;
	iVertSampleRatio = KYUV422SampleRatio;
		
	iMaxHorzPlaneFactor = 2;
	iMaxVertPlaneFactor = 1;
	
	iBlockSize.iWidth = KJpgDCTBlockWidth * iMaxHorzPlaneFactor * 2;
	iBlockSize.iHeight = KJpgDCTBlockWidth * iMaxVertPlaneFactor;
	
	iMaxPlaneSize = TSize(iMaxHorzPlaneFactor * iFrame->FrameSizeInPixels().iWidth, 
						  iMaxVertPlaneFactor * iFrame->FrameSizeInPixels().iHeight);
	} 
	
TDataUnit* CJpgImageFrameYUV422InterleavedProcessor::ReadBlockL()
	{
	ValidateCurrentBlockL();

	TUint8* p = &(iFrame->Data()[iLayout->Start(KLayoutIndex0) + iOffsets[KLayoutIndex0]]);

	// set the data to zero 
	Mem::FillZ(&iDataUnit, KUnitsYCbCr422 * sizeof(TDataUnit));

	TInt rowStart = 0;
	TInt i;
	TInt j;
	TInt m=0;
	TInt l=0;
	
	for (i = 0; i < iBlockSize.iHeight + iHCorrection; i++, rowStart += iLayout->ScanLength(KLayoutIndex0))
		{
		iIndexY = 0;
		l = KJpgDCTBlockWidth * i;	
		
		for (j = 0; j < iBlockSize.iWidth + iWCorrection; j+=4, m++, l+=2)
			{	
			if (j == KJpgDCTBlockWidth * 2)
				{
				iIndexY = 1;
				l = KJpgDCTBlockWidth * i;
				}
			// U plane	
			iDataUnit[iIndexU].iCoeff[m]= *(p+rowStart+j);
			// Y plane
			iDataUnit[iIndexY].iCoeff[l]= *(p+rowStart+j+1);
			// V plane
			iDataUnit[iIndexV].iCoeff[m]= *(p+rowStart+j+2);	
			// Y1 plane
			iDataUnit[iIndexY].iCoeff[l+1]= *(p+rowStart+j+3);			 
			}

			if (iWCorrection)
				{
				TInt16 data = iDataUnit[iIndexY].iCoeff[l - 1];
				for (TInt k = j; k < iBlockSize.iWidth; k += 4, m++, l += 2)
					{
					if (k == KJpgDCTBlockWidth * 2)
						{
						iIndexY = 1;
						l = KJpgDCTBlockWidth * i;
						}
					// U plane	
					iDataUnit[iIndexU].iCoeff[m] = iDataUnit[iIndexU].iCoeff[m-1];
					// Y plane
					iDataUnit[iIndexY].iCoeff[l] = data;
					// V plane 
					iDataUnit[iIndexV].iCoeff[m] = iDataUnit[iIndexV].iCoeff[m-1];	
					// Y1 plane
					iDataUnit[iIndexY].iCoeff[l+1] = data; 
					}
				}
			}

	if (iHCorrection)
		{
		for (TInt k = i; k < iBlockSize.iHeight; k++)
			{
			for (TInt block = 0; block < KUnitsYCbCr422; block ++ )
				{
				Mem::Copy(&iDataUnit[block].iCoeff[(k) * KJpgDCTBlockWidth], 
						  &iDataUnit[block].iCoeff[(k-1) * KJpgDCTBlockWidth], 
						  KJpgDCTBlockWidth * sizeof(TInt16));		
				}
			}
		}
		
	CalculateNextBlockStart(iLayout->ScanLength(KLayoutIndex0));
	return iDataUnit;
	}

void CJpgImageFrameYUV422InterleavedProcessor::WriteBlockL(const RArray<const TDataUnit*>& aDataUnit)
	{
	ValidateCurrentBlockL();
	
	TUint8* p = &(iFrame->Data()[iLayout->Start(KLayoutIndex0) + iOffsets[KLayoutIndex0]]);

	TInt rowStart = 0;

	TInt i;
	TInt j;
	TInt m = 0;
	TInt l = 0;
	
 	for (i = 0; i < iBlockSize.iHeight + iHCorrection; i++, rowStart += iLayout->ScanLength(KLayoutIndex0))
		{
		iIndexY = 0;
		l = KJpgDCTBlockWidth * i;	
		
		for (j = 0; j < iBlockSize.iWidth + iWCorrection; j += 4, m++, l += 2)
			{
			// switch between Y blocks using the current index as a guidance	
			if (j == KJpgDCTBlockWidth * 2)
				{
				iIndexY = 1;
				l = KJpgDCTBlockWidth * i;
				}
				
			// U plane	
			*(p+rowStart+j)	  = Clip(aDataUnit[iIndexU]->iCoeff[m]);
			// Y plane
			*(p+rowStart+j+1) =	Clip(aDataUnit[iIndexY]->iCoeff[l]);
			// V plane
			*(p+rowStart+j+2) =	Clip(aDataUnit[iIndexV]->iCoeff[m]);	
			// Y1 plane
			*(p+rowStart+j+3) =	Clip(aDataUnit[iIndexY]->iCoeff[l+1]);			 
			}
			m = l = KJpgDCTBlockWidth * (i + 1);
		}
		
	UpdateCurrentLengthL();
	
	// where is the update for the current frame
	CalculateNextBlockStart(iLayout->ScanLength(KLayoutIndex0));
	}
	
void CJpgImageFrameYUV422InterleavedProcessor::UpdateCurrentLengthL()
	{
	TInt currentLength = iOffsets[KLayoutIndex0] + (iBlockSize.iHeight + iHCorrection - 1) * iLayout->ScanLength(KLayoutIndex0) + iBlockSize.iWidth + iWCorrection;
	iLayout->SetCurrentLength(KLayoutIndex0, currentLength);
    iFrame->SetFrameLayoutL(*iLayout);
	}

/***********************CJpgImageFrameYUV420PlanarProcessor********************/
CJpgImageFrameYUV420PlanarProcessor::CJpgImageFrameYUV420PlanarProcessor(CImageFrame& aFrame) : CJpgImageFrameProcessor(aFrame)
	{	
	iIndexY = 0;
	if (static_cast<const TFrameFormat&>(aFrame.FrameFormat()).FormatCode() == KUidFormatYUV420Planar)
		{
		iIndexU = 4;
		iIndexV = 5;
		}
	else
		{
		iIndexU = 5;
		iIndexV = 4;
		}
							
	iMaxHorzPlaneFactor = 1;
	iMaxVertPlaneFactor = 1;

	iHorzSampleRatio = KYUV420SampleRatio;
	iVertSampleRatio = KYUV420SampleRatio;

	iBlockSize.iWidth = KJpgDCTBlockWidth * 2;
	iBlockSize.iHeight = KJpgDCTBlockWidth * 2;

	iMaxPlaneSize = TSize(iMaxHorzPlaneFactor * iFrame->FrameSizeInPixels().iWidth, iMaxVertPlaneFactor * iFrame->FrameSizeInPixels().iHeight);
	} 

TDataUnit* CJpgImageFrameYUV420PlanarProcessor::ReadBlockL()
	{
	ValidateCurrentBlockL();
	
	// set the data to zero 
	Mem::FillZ(&iDataUnit[0], KUnitsYCbCr420 * sizeof(TDataUnit));

	TUint8* p = &(iFrame->Data()[iLayout->Start(KLayoutIndex0) + iOffsets[KLayoutIndex0]]);
	
	// Y plane
	TInt i;
	TInt j;
	TInt l=0;	
	TInt rowStart = 0;
	
    // initially copy the the data from the image
    // however when MCU is partially outside the image the remaining elements are filled in
    // using replication, along rows and copying rows afterwards 
	for (i = 0; i < iBlockSize.iHeight + iHCorrection; i++, rowStart += iLayout->ScanLength(KLayoutIndex0))
		{
		iIndexY = (i < KJpgDCTBlockWidth)? 0: 2;
		l = (iIndexY == 0) ? (i * KJpgDCTBlockWidth):(i * KJpgDCTBlockWidth - KJpgDCTBlockSize);

		for (j = 0; j < iBlockSize.iWidth + iWCorrection; j++, l++)
			{
			if (j == KJpgDCTBlockWidth)
				{
				l = (iIndexY == 0)?(i * KJpgDCTBlockWidth) : (i * KJpgDCTBlockWidth - KJpgDCTBlockSize);
				iIndexY++;
				}

			iDataUnit[iIndexY].iCoeff[l] = *(p+rowStart+j);
			}	
					
		// need to repeat the last data
		if (iWCorrection)
			{
			TInt16 data = iDataUnit[iIndexY].iCoeff[l - 1];
			
			for ( TInt k = j; k < iBlockSize.iWidth; k++, l++)
				{
				if (k == KJpgDCTBlockWidth)
					{
					l = (iIndexY == 0)?(i * KJpgDCTBlockWidth):(i * KJpgDCTBlockWidth - KJpgDCTBlockSize);
					iIndexY++;
					}
				iDataUnit[iIndexY].iCoeff[l] = data;
				}
			}
		}
		
	// if there are rows outside the image, use the last row to fill them in
	if (iHCorrection)
		{
		TInt iIndexYO = 0;
		iIndexY = 0;
		TInt line;
		TInt origline = iBlockSize.iHeight + iHCorrection - 1;
		
		if (origline >= KJpgDCTBlockWidth)
				{
				iIndexYO = 2;
				origline -= KJpgDCTBlockWidth;
				}
		for (TInt k = iBlockSize.iHeight + iHCorrection; k < iBlockSize.iHeight; k++)
			{
			line = k;
			if (k>=KJpgDCTBlockWidth)
				{
				iIndexY = 2;
				line = k - KJpgDCTBlockWidth;
				}
			Mem::Copy(&iDataUnit[iIndexY].iCoeff[(line) * KJpgDCTBlockWidth], 
					  &iDataUnit[iIndexYO].iCoeff[origline * KJpgDCTBlockWidth], 
					  KJpgDCTBlockWidth * sizeof(TInt16));
					  	
			Mem::Copy(&iDataUnit[iIndexY+1].iCoeff[(line) * KJpgDCTBlockWidth], 
			          &iDataUnit[iIndexYO+1].iCoeff[origline * KJpgDCTBlockWidth], 
			          KJpgDCTBlockWidth * sizeof(TInt16));	
			}
		}
			
	// U plane 
	p = &(iFrame->Data()[iLayout->Start(KLayoutIndex1) + iOffsets[KLayoutIndex1]]);
	DoReadBlock(iDataUnit[iIndexU], p, iLayout->ScanLength(KLayoutIndex1), 
				iWCorrection / iHorzSampleRatio[KLayoutIndex1], 
				iHCorrection / iVertSampleRatio[KLayoutIndex1]);	
	
	// V plane
	p = &(iFrame->Data()[iLayout->Start(KLayoutIndex2) +  iOffsets[KLayoutIndex2]]);
	DoReadBlock(iDataUnit[iIndexV], p, iLayout->ScanLength(KLayoutIndex2), 
				iWCorrection / iHorzSampleRatio[KLayoutIndex2], 
				iHCorrection / iVertSampleRatio[KLayoutIndex2]);	
	
   	CalculateNextBlockStart(iLayout->ScanLength(KLayoutIndex0));
   	
	return iDataUnit;
	}
	
void CJpgImageFrameYUV420PlanarProcessor::WriteBlockL(const RArray<const TDataUnit*>& aDataUnit)
	{
	ValidateCurrentBlockL();	
	
	TUint8* p = &(iFrame->Data()[iLayout->Start(KLayoutIndex0) + iOffsets[KLayoutIndex0]]);

	TInt l=0;
	TInt rowStart = 0;
	
    // Y plane - 4 blocks - iIndexY is used to select the correct data unit depending on the position
    // they are organised 0, 1
    //                    2, 3 
	for (TInt i = 0; i < iBlockSize.iHeight + iHCorrection; i++, rowStart += iLayout->ScanLength(KLayoutIndex0))
		{	
		iIndexY = (i < KJpgDCTBlockWidth) ? 0: 2;
		l = (iIndexY == 0) ? (i * KJpgDCTBlockWidth) : (i * KJpgDCTBlockWidth - KJpgDCTBlockSize);
		
		for (TInt j = 0; j < iBlockSize.iWidth + iWCorrection; j++, l++)
			{
			if (j == KJpgDCTBlockWidth)
				{
				l = (iIndexY == 0) ? (i * KJpgDCTBlockWidth) : (i * KJpgDCTBlockWidth - KJpgDCTBlockSize);
				iIndexY++;
				}
			*(p+rowStart+j) = Clip(aDataUnit[iIndexY]->iCoeff[l]);
			}
		}

	// U plane 
	p = &(iFrame->Data()[iLayout->Start(KLayoutIndex1) + iOffsets[KLayoutIndex1]]);
	DoWriteBlock(p, *aDataUnit[iIndexU], iLayout->ScanLength(KLayoutIndex1), 
				iWCorrection / iHorzSampleRatio[KLayoutIndex1],
				iHCorrection / iVertSampleRatio[KLayoutIndex1]);
	// V plane
	p = &(iFrame->Data()[iLayout->Start(KLayoutIndex2) + iOffsets[KLayoutIndex2]]);
   	DoWriteBlock(p, *aDataUnit[iIndexV], iLayout->ScanLength(KLayoutIndex2), 
				iWCorrection / iHorzSampleRatio[KLayoutIndex2],
				iHCorrection / iVertSampleRatio[KLayoutIndex2]);
				
	UpdateCurrentLengthL();			
	CalculateNextBlockStart(iLayout->ScanLength(KLayoutIndex0));
	}
	
void CJpgImageFrameYUV420PlanarProcessor::UpdateCurrentLengthL()
	{
	TInt currentLength;
	currentLength = iOffsets[KLayoutIndex0] + (iBlockSize.iHeight + iHCorrection - 1) * iLayout->ScanLength(KLayoutIndex0) + iBlockSize.iWidth + iWCorrection;
	iLayout->SetCurrentLength(KLayoutIndex0, currentLength);
    
    currentLength = (iOffsets[KLayoutIndex1] + (KJpgDCTBlockWidth + iHCorrection/2 - 1) * iLayout->ScanLength(KLayoutIndex1) + KJpgDCTBlockWidth + iWCorrection/2)/2;
	iLayout->SetCurrentLength(KLayoutIndex1, currentLength);

	currentLength = (iOffsets[KLayoutIndex2] + (KJpgDCTBlockWidth + iHCorrection/2 - 1) * iLayout->ScanLength(KLayoutIndex2) + KJpgDCTBlockWidth + iWCorrection/2)/2;
	iLayout->SetCurrentLength(KLayoutIndex2, currentLength);

	iFrame->SetFrameLayoutL(*iLayout);
	}

//
// all the code down there is not performance-critical so use thumb
// instruction set to save on some ROM footprint
// 
#if defined(__ARMCC__)
#pragma thumb
#endif

/****************************CJpgImageFrameProcessorUtility*********************/
CJpgImageFrameProcessor* CJpgImageFrameProcessorUtility::NewL(CImageFrame& aFrame)
	{
	// verify that the correct format and layout objects are used. 
	if (!CJpgImageFrameProcessorUtility::IsRecognisedFormatType(aFrame))
		{
		User::Leave(KErrNotSupported);
		}
		
	if (!CJpgImageFrameProcessorUtility::IsRecognisedLayoutType(aFrame))
		{
		User::Leave(KErrNotSupported);
		}
		
	CJpgImageFrameProcessor *processor = NULL;
	const TFrameFormat& format = static_cast<const TFrameFormat&>(aFrame.FrameFormat());
	
	// this frame processor supports only YCbCr or YUV colour spaces
	if ((format.ColourSpace() != KUidColourSpaceYCbCr) &&
	    (format.ColourSpace() != KUidColourSpaceYUV))
		{
		User::Leave(KErrNotSupported);
		}

	switch(format.FormatCode().iUid)
		{
		case KFormatYUVMonochromeUidValue:
	    	{
	    	CJpgImageFrameYUVMonoProcessor* self =  new(ELeave) CJpgImageFrameYUVMonoProcessor(aFrame); 
			CleanupStack::PushL(self);
			self->ConstructL(aFrame.FrameLayout());
			CleanupStack::Pop();
			processor = self;
			break;
			}
	    	
		case KFormatYUV422InterleavedUidValue:
			{
			CJpgImageFrameYUV422InterleavedProcessor* self =  new(ELeave) CJpgImageFrameYUV422InterleavedProcessor(aFrame); 
			CleanupStack::PushL(self);
			self->ConstructL(aFrame.FrameLayout());
			CleanupStack::Pop();
			processor = self;
			break;
			}
		// the two formats differ just in the ordering of the UV planes  
		// one is UV and the other is VU.
		// so just use one processor. 	
		case KFormatYUV420PlanarReversedUidValue:
		case KFormatYUV420PlanarUidValue:
			{
			CJpgImageFrameYUV420PlanarProcessor* self =  new(ELeave) CJpgImageFrameYUV420PlanarProcessor(aFrame); 
			CleanupStack::PushL(self);
			self->ConstructL(aFrame.FrameLayout());
			CleanupStack::Pop();

			processor = self;
			break;
			}
			
		default:
			User::Leave(KErrNotSupported);
			break; 	
		}
		
	return processor;
	}
	
void CJpgImageFrameProcessorUtility::PrepareImageFrameL(const TJpgFrameInfo& aFrameInfo, CImageFrame& aFrame)
	{
	// This routine checks the validity of the destination image frame and writes:
	// - the frame format based on the jpg frame info obtained from the jpeg header information.
	// - the frame layout based on the capabilities of this codec.

	// First, validate image frame
	CJpgImageFrameProcessorUtility::ValidateImageFrameL(aFrame, EFalse);


	TInt dataUnitCount;

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
 
 	const TFrameFormat& format = static_cast<const TFrameFormat&>(aFrame.FrameFormat());

	TUid imageFrameSampleScheme = format.Sampling();

	// Check that there is enough memory allocated before starting decode:
	TInt maxBufferSize = aFrame.MaxBufferSize();	
    	
	switch (dataUnitCount)
		{

		case 1: // Monochrome
			{
			if (imageFrameSampleScheme != KUidSamplingMonochrome )
				{
				if (imageFrameSampleScheme == KNullUid)
					{
					// this monochrome specific format
					TFrameFormat monochromeFrameFormat = TFrameFormat(KUidFormatYUVMonochrome);
					aFrame.SetFrameFormatL(monochromeFrameFormat);
					}
				else
					{
					// error transcoding not supported					
					User::Leave(KErrNotSupported);
					}
				}

			// this monochrome specific layout 
			TInt bufferSize = aFrame.FrameSizeInPixels().iWidth * 
						 	  aFrame.FrameSizeInPixels().iHeight;
			if (bufferSize > maxBufferSize)
				{
				User::Leave(KErrOverflow);
				}
				
			TInt scanlineLength = aFrame.FrameSizeInPixels().iWidth;
			TInt numberOfPlanes = 1;

			TFrameLayout monochromeFrameLayout = TFrameLayout(numberOfPlanes);
			monochromeFrameLayout.SetStart(0, 0);
			monochromeFrameLayout.SetLength(0, bufferSize);
			monochromeFrameLayout.SetCurrentLength(0, 0);
			monochromeFrameLayout.SetScanLength(0, scanlineLength);

			aFrame.SetFrameLayoutL(monochromeFrameLayout);

			break;		
			}

		case 3: // 4:4:4
			{
			if (imageFrameSampleScheme != KUidSamplingColor444 )
				{
				// This format is not supported by this codec					
				User::Leave(KErrNotSupported);
				}
			break;		
			}

		case 4: // 4:2:2
			{
			if (imageFrameSampleScheme != KUidSamplingColor422 )
				{
				if (imageFrameSampleScheme == KNullUid)
					{
					// this specific 4:2:2 format
					TFrameFormat this422FrameFormat = TFrameFormat(KUidFormatYUV422Interleaved);
					aFrame.SetFrameFormatL(this422FrameFormat);
					}
				else
					{
					// error transcoding not supported					
					User::Leave(KErrNotSupported);
					}
				}

			// this is specific for 4:2:2 interleaved layout - width have to be a multiple of 4 
			TInt width = (aFrame.FrameSizeInPixels().iWidth + 3) & (KMaxTInt - 3);
			TInt height = aFrame.FrameSizeInPixels().iHeight;
			TInt bufferSize = width * height * 2;
			if (bufferSize > maxBufferSize)
				{
				User::Leave(KErrOverflow);
				}
			TInt scanlineLength = width * 2;
			TInt numberOfPlanes = 1;
			
			TFrameLayout this422FrameLayout = TFrameLayout(numberOfPlanes);
			this422FrameLayout.SetStart(0, 0);
			this422FrameLayout.SetLength(0, bufferSize);
			this422FrameLayout.SetCurrentLength(0, 0);
			this422FrameLayout.SetScanLength(0, scanlineLength);

			aFrame.SetFrameLayoutL(this422FrameLayout);

			break;	
			}

		case 6: // 4:2:0
			{
			if (imageFrameSampleScheme != KUidSamplingColor420)
				{
				if (imageFrameSampleScheme == KNullUid)
					{
					// this specific 4:2:0 format
					TFrameFormat this420FrameFormat = TFrameFormat(KUidFormatYUV420Planar);
					aFrame.SetFrameFormatL(this420FrameFormat);
					}
				else
					{
					// error transcoding not supported					
					User::Leave(KErrNotSupported);
					}
				}
				
			// this specific 4:2:0 layout
			TInt bufferSizeY = aFrame.FrameSizeInPixels().iWidth * 
						 	   aFrame.FrameSizeInPixels().iHeight;


			// Round width and height of UV planes to nearest multiple of 2		
			// to avoid calculating incorrect buffer size	
			TInt widthUV = (aFrame.FrameSizeInPixels().iWidth + 1) & (KMaxTInt - 1);
			TInt heightUV = (aFrame.FrameSizeInPixels().iHeight + 1) & (KMaxTInt - 1);
			
			TInt bufferSizeUV = (widthUV * heightUV) / 4;
			
			TInt bufferSize = bufferSizeY + 2 * bufferSizeUV;
			if (bufferSize > maxBufferSize)
				{
				User::Leave(KErrOverflow);
				}

			TInt scanlineLengthY = aFrame.FrameSizeInPixels().iWidth;
			TInt scanlineLengthUV = aFrame.FrameSizeInPixels().iWidth / 2;
			TInt numberOfPlanes = 3;
			
			TFrameLayout this420FrameLayout = TFrameLayout(numberOfPlanes);
			this420FrameLayout.SetStart(0, 0);
			this420FrameLayout.SetLength(0, bufferSizeY);
			this420FrameLayout.SetCurrentLength(0, 0);
			this420FrameLayout.SetScanLength(0, scanlineLengthY);
			this420FrameLayout.SetStart(1, bufferSizeY);
			this420FrameLayout.SetLength(1, bufferSizeUV);
			this420FrameLayout.SetCurrentLength(1, 0);
			this420FrameLayout.SetScanLength(1, scanlineLengthUV);
			this420FrameLayout.SetStart(2, bufferSizeY + bufferSizeUV);
			this420FrameLayout.SetLength(2, bufferSizeUV);
			this420FrameLayout.SetCurrentLength(2, 0);
			this420FrameLayout.SetScanLength(2, scanlineLengthUV);
			
			aFrame.SetFrameLayoutL(this420FrameLayout);
			break;	
			}

		default:
			{
			User::Leave(KErrNotSupported);			
			break; 	
			}
		}

	}
	
void CJpgImageFrameProcessorUtility::ValidateImageFrameL(CImageFrame& aFrame, TBool aFullFrame)
	{
	// This routine validates the image frame. It leaves if inconsistencies are found.
		
	// Encoder expects a CImageFrame object with the reference to the memory 
	// (RChunk/Descriptor) containing the data plus a full description of the data,
	// whereas the decoder may have instantiated the CImageFrame object with just
	// the reference to the RChunk/Descriptor (without data).
	// - aFullFrame = True means validation of the source frame (encoder).
	// - aFullFrame = False means validation of the destination frame (decoder).
	if (!CJpgImageFrameProcessorUtility::IsRecognisedFormatType(aFrame))
		{
		User::Leave(KErrNotSupported);
		}
	if (!CJpgImageFrameProcessorUtility::IsRecognisedLayoutType(aFrame))
		{
		User::Leave(KErrNotSupported);
		}	
	const TFrameFormat& format = static_cast<const TFrameFormat&>(aFrame.FrameFormat());

	TUid imageFrameSampleScheme = format.Sampling();
	TUid imageFrameColourSpace = format.ColourSpace();
	TUid formatCode = format.FormatCode();

	// The format code in an image frame defines unequivocably the data layout and sampling format.
	// Check for inconsistencies and fill in the rest of the image frame fields if necessary
	switch(formatCode.iUid)
		{
		case KFormatYUVMonochromeUidValue:
	    	{
			if (((imageFrameSampleScheme == KUidSamplingMonochrome) ||
				 (imageFrameSampleScheme == KNullUid))
				&&
				((imageFrameColourSpace == KUidColourSpaceYCbCr) ||
		     	 (imageFrameColourSpace == KNullUid)))
				{
					TFrameFormat thismonochromeFrameFormat = TFrameFormat(KUidFormatYUVMonochrome);
					aFrame.SetFrameFormatL(thismonochromeFrameFormat);
	    		}
			else
				{
				User::Leave(KErrNotSupported);
				}	    		
			break;
	    	}

    	case KFormatYUV422InterleavedUidValue:
	    	{
			if (((imageFrameSampleScheme == KUidSamplingColor422) ||
				 (imageFrameSampleScheme == KNullUid))
				&&
				((imageFrameColourSpace == KUidColourSpaceYCbCr) ||
		     	 (imageFrameColourSpace == KNullUid)))
				{
				TFrameFormat this422FrameFormat = TFrameFormat(KUidFormatYUV422Interleaved);
				aFrame.SetFrameFormatL(this422FrameFormat);
	    		}
			else
				{
				User::Leave(KErrNotSupported);
				}	    		
			break;
	    	}
	    	
    	case KFormatYUV420PlanarReversedUidValue:
    	case KFormatYUV420PlanarUidValue:
   	    	{
			if (((imageFrameSampleScheme == KUidSamplingColor420) ||
				 (imageFrameSampleScheme == KNullUid))
		     	&&
				((imageFrameColourSpace == KUidColourSpaceYCbCr) ||
		     	 (imageFrameColourSpace == KNullUid)))

				{
				if (formatCode == KUidFormatYUV420PlanarReversed)
					{
					TFrameFormat this420FrameFormat = TFrameFormat(KUidFormatYUV420PlanarReversed);	
					aFrame.SetFrameFormatL(this420FrameFormat);
					}					
				else if (formatCode == KUidFormatYUV420Planar)
					{
					TFrameFormat this420FrameFormat = TFrameFormat(KUidFormatYUV420Planar);	
					aFrame.SetFrameFormatL(this420FrameFormat);
					}
				else
					{
					User::Leave(KErrNotSupported);
					}
	    		}
			break;
	    	}
	    	
    	case KNullUidValue:
   	    	{
   	    	// This is the case where the application passes an empty imageFrame
   	    	// object to the decoder (no data, no description of frame, just the 
   	    	// reference to an empty RChunk/Descriptor)
			if (aFullFrame != EFalse) 
				{
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
	
void CJpgImageFrameProcessorUtility::ValidateFrameImageDataL(const TJpegImageData::TColorSampling aSampleScheme, CImageFrame& aSource)
	{
	// Optional Frame Image data provided when calling ConvertFrame() needs to be validated
	// against the source format since transcoding is not supported by this codec.
 	const TFrameFormat& format = static_cast<const TFrameFormat&>(aSource.FrameFormat());
	TUid imageFrameSampleScheme = format.Sampling();
	
	switch (aSampleScheme)
		{
		case TJpegImageData::EMonochrome:
			{
			if (imageFrameSampleScheme != KUidSamplingMonochrome)
				{
				User::Leave(KErrNotSupported);
				}
			break;
			}
			
		case TJpegImageData::EColor420:
			{
			if (imageFrameSampleScheme != KUidSamplingColor420)
				{
				User::Leave(KErrNotSupported);
				}
			break;
			}
			
		case TJpegImageData::EColor422:
			{
				if (imageFrameSampleScheme != KUidSamplingColor422)
					{
					User::Leave(KErrNotSupported);
					}
				break;				
			}
			
		case TJpegImageData::EColor444:
			{
				// YUV 4:4:4 sampling scheme is not supported by this codec
				User::Leave(KErrNotSupported);
				break;					
			}
			
		default:
			{
				User::Leave(KErrNotSupported);	
				break;	
			}
		}
	}
//Get the size of the memory buffer to hold the returned data. calculates teh buffersize based on the format and numbe of blocks
TInt CJpgImageFrameProcessorUtility::RecommendedStreamBufferSizeL(const TJpgFrameInfo& aFrameInfo, TUid aFormatCode, TSize& aBlockSizeInPixels, TInt aNumBlocks)
	{
	TInt dataUnitCount = 0;
	TInt bufferSize = 0;
	
	if (aFrameInfo.iNumberOfComponents == 1)
		{
		dataUnitCount = 1;		
		}
	else
		{
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
			if (aFormatCode == KUidFormatYUVMonochrome)
				{
				aBlockSizeInPixels = TSize((aNumBlocks * KSamplingMonoMCUWidthInPixels), (KSamplingMonoMCUHeightInPixels));
				bufferSize = aBlockSizeInPixels.iWidth * aBlockSizeInPixels.iHeight;
				}
			else
				{
				// error transcoding not supported					
				User::Leave(KErrNotSupported);
				}
			break;		
			}

		case 4: // 4:2:2
			{
			if (aFormatCode == KUidFormatYUV422Interleaved)
				{
				aBlockSizeInPixels = TSize((aNumBlocks * KSampling422MCUWidthInPixels), (KSampling422MCUHeightInPixels));
				// this is specific for 4:2:2 interleaved layout - width have to be a multiple of 4 
				TInt width = (aBlockSizeInPixels.iWidth + 3) & (KMaxTInt - 3);
				TInt height = aBlockSizeInPixels.iHeight;
				bufferSize = width * height * 2; //in 422 sampling scheme width is twice the height
				}
			else
				{
				// error transcoding not supported					
				User::Leave(KErrNotSupported);
				}			
			break;	
			}

		case 6: // 4:2:0 
			{
			if ((aFormatCode == KUidFormatYUV420Planar) || 
				(aFormatCode == KUidFormatYUV420PlanarReversed))
				{
				aBlockSizeInPixels = TSize((aNumBlocks * KSampling420MCUWidthInPixels), (KSampling420MCUHeightInPixels));
				// this specific 4:2:0 layout
				TInt bufferSizeY = aBlockSizeInPixels.iWidth * 
							 	   aBlockSizeInPixels.iHeight;

				// Round width and height of UV planes to nearest multiple of 2		
				// to avoid calculating incorrect recommended buffer size	
				TInt widthUV = (aBlockSizeInPixels.iWidth + 1) & (KMaxTInt - 1);
				TInt heightUV = (aBlockSizeInPixels.iHeight + 1) & (KMaxTInt - 1);
			
				TInt bufferSizeUV = (widthUV * heightUV) / 4;

				bufferSize = bufferSizeY + 2 * bufferSizeUV + 15;
				}
			else
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
		
		return bufferSize;
	}

TInt CJpgImageFrameProcessorUtility::RecommendedBufferSizeL(const TJpgFrameInfo& aFrameInfo, TUid formatCode)
	{
	TInt dataUnitCount=0;
	TInt bufferSize = 0;
	
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
			if ((formatCode == KUidFormatYUVMonochrome) ||
				 (formatCode == KNullUid))
				{
				bufferSize = aFrameInfo.iSizeInPixels.iWidth * aFrameInfo.iSizeInPixels.iHeight;
				}
			else
				{
				// error transcoding not supported					
				User::Leave(KErrNotSupported);
				}
			break;		
			}

		case 4: // 4:2:2
			{
			if ((formatCode == KUidFormatYUV422Interleaved) ||
				(formatCode == KNullUid))
				{
				// this is specific for 4:2:2 interleaved layout - width have to be a multiple of 4 
				TInt width = (aFrameInfo.iSizeInPixels.iWidth + 3) & (KMaxTInt - 3);
				TInt height = aFrameInfo.iSizeInPixels.iHeight;
				bufferSize = width * height * 2;
				}
			else
				{
				// error transcoding not supported					
				User::Leave(KErrNotSupported);
				}			
			break;	
			}

		case 6: // 4:2:0 
			{
			if ((formatCode == KUidFormatYUV420Planar) || 
				(formatCode == KUidFormatYUV420PlanarReversed) || 
				(formatCode == KNullUid))
				{
				// this specific 4:2:0 layout
				TInt bufferSizeY = aFrameInfo.iSizeInPixels.iWidth * 
							 	   aFrameInfo.iSizeInPixels.iHeight;

				// Round width and height of UV planes to nearest multiple of 2		
				// to avoid calculating incorrect recommended buffer size	
				TInt widthUV = (aFrameInfo.iSizeInPixels.iWidth + 1) & (KMaxTInt - 1);
				TInt heightUV = (aFrameInfo.iSizeInPixels.iHeight + 1) & (KMaxTInt - 1);
			
				TInt bufferSizeUV = (widthUV * heightUV) / 4;

				bufferSize = bufferSizeY + 2 * bufferSizeUV + 15;
				}
			else
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
		
		return bufferSize;
	}

TBool CJpgImageFrameProcessorUtility::IsRecognisedFormatType(CImageFrame& aFrame)
	{
 	const TFrameFormat& format = static_cast<const TFrameFormat&>(aFrame.FrameFormat());

	if (&format != NULL)
		{
		return format.Type() == KUidIclImageFrameFormat;
		}
	else
		{
		return EFalse;
		}
	}

TBool CJpgImageFrameProcessorUtility::IsRecognisedLayoutType(CImageFrame& aFrame)
	{
 	const TFrameLayout& layout = static_cast<const TFrameLayout&>(aFrame.FrameLayout());
	if (&layout != NULL)
		{
		return layout.Type()==KUidIclImageFrameLayout;	
		}
	else
		{
		return EFalse;
		}		
	}
	

	
