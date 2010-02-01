// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// mcucliprect.cpp
// 
//

#include "JPEGCodec.h"
#include "fwextconstants.h"

//
//
//
CMCUStore::CMCUStore(TJpgFrameInfo& aFrameInfo)
 :	iFrameInfo(aFrameInfo)
	{
	Reset();
	}

//
//
//
CMCUStore::~CMCUStore()
	{
	}

//
//
//
CMCUStore* CMCUStore::NewL(TJpgFrameInfo& aFrameInfo)
	{
	return new(ELeave) CMCUStore(aFrameInfo);
	}


//
// Resets the store to a normal, unclipped decode.
//
void CMCUStore::Reset()
	{
	iClipped = EFalse;
	iPrepared = EFalse;
	iOperation = EDecodeNormal;
	iReads = 0;
	iMCUsPerBuffer = 0;
	}

//
// Sets the orientation.
//
void CMCUStore::SetOperation(const TTransformOptions aOperation)
	{
	iPrepared = EFalse;
	iOperation = aOperation;
	
	if (!iClipped)
		{
		// Rendering the full image.
		iCurrentMCU = 0;
		iLastMCU = iFrameInfo.TotalMCUCount();
		}
	}


//
// Sets the index of the MCU that contains the pixels of the top-left corner
// of the clipping rect, and the number of MCUs in the (bounded) clipping rect.
//
void CMCUStore::SetClippingRect(const TInt aFirstMCU, const TInt aTotalMCUs)
	{
	iPrepared = EFalse;
	iClipped = ETrue;
	iCurrentMCU = aFirstMCU;
	iMaxReads = aTotalMCUs;
	}


//
// Tells the store how many MCUs the buffer can contain.
//
void CMCUStore::SetMCUsPerBuffer(const TInt aMCUsPerBuffer)
	{
	ASSERT(aMCUsPerBuffer > 0);
	
	iMCUsPerBuffer = aMCUsPerBuffer;
	}

//
// This function sets up the order the MCUs are returned
// according to the current operation and presence of the
// clipping rect.
//
void CMCUStore::Prepare()
	{
	ASSERT(iMCUsPerBuffer > 0);
	
	switch (iOperation)
		{
		case EDecodeNormal:
		case EDecodeHorizontalFlip:
		case EDecodeVerticalFlip:
		case EDecodeRotate180:
			if (iClipped)
				{
				iMCUIncrement = 1; 
				iNextRowOffset = iFrameInfo.iMCUBlocksPerLine - iMCUsPerBuffer + 1;
				}
			else
				{  
				iMCUIncrement = 1;
				iNextRowOffset = 1;
				}
			break;

		case EDecodeRotate90:
		case EDecodeRotate270:
		case EDecodeHorizontalFlipRotate90:
		case EDecodeVerticalFlipRotate90:
			if (iClipped)
				{
				iMCUIncrement = iFrameInfo.iMCUBlocksPerLine;
				iNextRowOffset = 1 - (iFrameInfo.iMCUBlocksPerLine * (iMCUsPerBuffer - 1));
				}
			else
				{
				iMCUIncrement = iFrameInfo.iMCUBlocksPerLine;
				iNextRowOffset = 1 - (iFrameInfo.iMCUBlocksPerLine * (iMCUsPerBuffer - 1));
				}
			break;

		default:
			ASSERT(EFalse);
		}
	
	iPrepared = ETrue;
	iReads = 0;
	iMaxReads = (iClipped ? iMaxReads : iFrameInfo.TotalMCUCount());	
	}

//
// This should be called by the owning codec as soon as the buffer 
// has been sent to the ImageProcessor.
//
void CMCUStore::NextLine()
	{
	if (!iPrepared)
		{
		Prepare();
		}
	
	if (iReads >= iMaxReads)
		{
		iCurrentMCU = KErrCompletion;
		}
	else
		{
		iCurrentMCU -= iMCUIncrement;
		iCurrentMCU += iNextRowOffset;
		}
	}

//
// Returns the next MCU to be rendered depending on the render mode.
// Returns KErrCompletion if there are no more MCUs required.
//
TInt CMCUStore::GetNextMCU()
	{
	if (!iPrepared)
		{
		Prepare();
		}
	
	if (iCurrentMCU == KErrCompletion)
		{
		return iCurrentMCU;
		}
	
	TInt retMCU = iCurrentMCU;
	iCurrentMCU += iMCUIncrement;
	if (iReads++ > iMaxReads)
		{
		iCurrentMCU = KErrCompletion;
		}
	
	ASSERT(retMCU == KErrCompletion || retMCU >= 0);
	return retMCU;
	}

