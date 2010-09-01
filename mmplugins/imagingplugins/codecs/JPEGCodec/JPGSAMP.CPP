// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>

#include "JpegTypes.h"

void TDataUnitProcessor::SetBufferPtrUtil(CRgbBufferPtr& aBufferPtrUtil)
    {
    iBufferPtrUtil = &aBufferPtrUtil;
    }
    
    
// TMonoProcessor
FORCEDINLINE TInt16 MonoPixel(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
    {
    return TInt16(
        			  (
        			  	(CRgbBufferPtr::Red(aRgbBuffer)<<1) + 
        			    CRgbBufferPtr::Green(aRgbBuffer) +
        			    (CRgbBufferPtr::Green(aRgbBuffer)<<2) + 
        			    CRgbBufferPtr::Blue(aRgbBuffer)
        			   )>>3
        	    );
    }
    
TDataUnit* TMonoProcessor::Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{
	TInt16* YPtr = iMCU.iCoeff;
	const TInt16* YPtrLimit = YPtr + KJpgDCTBlockSize;
	const TInt KLineStride = iBufferPtrUtil->NextLineOffset();
    
	do
		{
		TInt p=0;
		do 
		    {		
		    *YPtr++ = MonoPixel( CRgbBufferPtr::ShiftPtr(aRgbBuffer, p++) );
		    *YPtr++ = MonoPixel( CRgbBufferPtr::ShiftPtr(aRgbBuffer, p++) );
		    
		    } while (p < KJpgDCTBlockWidth);
		aRgbBuffer += KLineStride;
		
		} while (YPtr < YPtrLimit);

	return &iMCU;
	}

// T444Processor
TDataUnit* T444Processor::Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{
	TInt16* YPtr  = iMCU[0].iCoeff;
	TInt16* CbPtr = iMCU[1].iCoeff;
	TInt16* CrPtr = iMCU[2].iCoeff;
	
	const TInt16* YPtrLimit = YPtr + KJpgDCTBlockSize;
	const TInt KLineStride = iBufferPtrUtil->NextLineOffset();
	
	do
		{
		TInt p=0;
		do 
		    {
    		TYCbCr::RGBtoYCbCr(YPtr++, CbPtr++, CrPtr++, CRgbBufferPtr::ShiftPtr(aRgbBuffer, p++) );
		    } while (p < KJpgDCTBlockWidth);
		    
		aRgbBuffer += KLineStride;

		} while (YPtr < YPtrLimit);

	return iMCU;
	}


// T422Processor
TDataUnit* T422Processor::Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{
	TInt16* CbPtr = iMCU[2].iCoeff;
	ASSERT( CbPtr + KJpgDCTBlockSize == iMCU[3].iCoeff); // we assume that V comp is next to U one

	const TInt halfWidth = KJpgDCTBlockWidth >> 1;

	DoProcess(iMCU[0].iCoeff,CbPtr, aRgbBuffer);
	DoProcess(iMCU[1].iCoeff,CbPtr + halfWidth, CRgbBufferPtr::ShiftPtr(aRgbBuffer,KJpgDCTBlockWidth) );

	return iMCU;
	}

void T422Processor::DoProcess(TInt16* aYPtr,TInt16* aCbPtr, CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{
	const TInt KLineStride = iBufferPtrUtil->NextLineOffset();
	const TInt16* YPtrLimit = aYPtr + KJpgDCTBlockSize;

	do
		{
		TYCbCr::RGBtoYCbCr2PixUVSum(aYPtr,aCbPtr, aRgbBuffer);

		TYCbCr::RGBtoYCbCr2PixUVSum(aYPtr+2,aCbPtr+1, CRgbBufferPtr::ShiftPtr(aRgbBuffer,2));
		
		TYCbCr::RGBtoYCbCr2PixUVSum(aYPtr+4,aCbPtr+2,CRgbBufferPtr::ShiftPtr(aRgbBuffer,4));

		TYCbCr::RGBtoYCbCr2PixUVSum(aYPtr+6,aCbPtr+3,CRgbBufferPtr::ShiftPtr(aRgbBuffer,6));

		aYPtr  += KJpgDCTBlockWidth;
		aCbPtr += KJpgDCTBlockWidth;
		aRgbBuffer += KLineStride;

		} while (aYPtr < YPtrLimit);
	}


// T420Processor
TDataUnit* T420Processor::Process(CRgbBufferPtr::TConstRgbBufferPtr aRgbBuffer)
	{
	TInt16* CbPtr = iMCU[4].iCoeff;
	ASSERT( CbPtr + KJpgDCTBlockSize == iMCU[5].iCoeff); // we assume that V comp is next to U one
	
	const TInt halfWidth = KJpgDCTBlockWidth >> 1;
	const TInt halfSize = KJpgDCTBlockSize >> 1;

	DoProcess(iMCU[0].iCoeff,CbPtr, aRgbBuffer);
	DoProcess(iMCU[1].iCoeff,CbPtr + halfWidth, CRgbBufferPtr::ShiftPtr(aRgbBuffer, KJpgDCTBlockWidth));

	aRgbBuffer += (iBufferPtrUtil->NextLineOffset() << 3);
	
	CbPtr += halfSize;

	DoProcess(iMCU[2].iCoeff,CbPtr, aRgbBuffer);
	DoProcess(iMCU[3].iCoeff,CbPtr + halfWidth, CRgbBufferPtr::ShiftPtr(aRgbBuffer, KJpgDCTBlockWidth));

	return iMCU;
	}

void T420Processor::DoProcess(TInt16* aYPtr,TInt16* aCbPtr, CRgbBufferPtr::TConstRgbBufferPtr  aRgbBuffer)
	{
	const TInt16* YPtrLimit = aYPtr + KJpgDCTBlockSize;
	const TInt KLineStride = iBufferPtrUtil->NextLineOffset();
	const TInt KDoubleLineStride = KLineStride << 1;
	
	do
		{
		TYCbCr::RGBtoYCbCr4PixUVSum(aYPtr, aCbPtr, aRgbBuffer, KLineStride);

		TYCbCr::RGBtoYCbCr4PixUVSum(aYPtr+2, aCbPtr+1, CRgbBufferPtr::ShiftPtr(aRgbBuffer,2), KLineStride);

        TYCbCr::RGBtoYCbCr4PixUVSum(aYPtr+4, aCbPtr+2, CRgbBufferPtr::ShiftPtr(aRgbBuffer,4), KLineStride);
        
        TYCbCr::RGBtoYCbCr4PixUVSum(aYPtr+6, aCbPtr+3, CRgbBufferPtr::ShiftPtr(aRgbBuffer,6), KLineStride);

		aRgbBuffer 	+= KDoubleLineStride;
		aCbPtr 		+= KJpgDCTBlockWidth;
		aYPtr 		+= KJpgDCTBlockWidth << 1;
		
		} while (aYPtr < YPtrLimit);
	}


