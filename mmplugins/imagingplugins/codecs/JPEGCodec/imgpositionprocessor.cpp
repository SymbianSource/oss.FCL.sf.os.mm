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
//

/**
 @file
 @internalComponent
*/
#include <e32base.h>

#include "imgpositionprocessor.h"

TPositionProcessor::TPositionProcessor()
	{
	iImgCornerCoord[0] = KJpgTLCoord;
	iImgCornerCoord[1] = KJpgTRCoord;
	iImgCornerCoord[2] = KJpgBLCoord;
	iImgCornerCoord[3] = KJpgBRCoord;
	
	iIsEOF = EFalse;
	iSwapDimensions = EFalse;
	}

/**
	Set the dimensions for image rectangle and MCUs 
*/
void TPositionProcessor::SetDimensions(const TRect& aPixelFrame, const TSize& aPixelBlock)
	{
	iPixelFrame = aPixelFrame;
	iPixelBlock = aPixelBlock;
	
	iIsEOF = EFalse;
	iSwapDimensions = EFalse;
	}

/**
	Sets the end of image flag.
*/
void TPositionProcessor::SetEndOfImage(TBool aEndOfImage)
	{
	iIsEOF = aEndOfImage;
	}

/**
	Transform image corner coordinates in case of rotation.	This is a 90 degrees clockwise rotation. 
	This can be called multiple times to achieve 180, 270 or 360 degree rotation.
*/
void TPositionProcessor::RotateCoordinates()
	{
	TUint8 tempVal = iImgCornerCoord[0];
	
	iImgCornerCoord[0] = iImgCornerCoord[1];
	iImgCornerCoord[1] = iImgCornerCoord[3];
	iImgCornerCoord[3] = iImgCornerCoord[2];
	iImgCornerCoord[2] = tempVal;
	
	if(iSwapDimensions == EFalse)
		{
		iSwapDimensions = ETrue;
		}
	else
		{
		iSwapDimensions = EFalse;
		}
	}

/**
	Transform image corner coordinates in case of horizontal flip
*/
void TPositionProcessor::HorFlipCoordinates()
	{
	TUint8 tempVal = iImgCornerCoord[0];
	
	iImgCornerCoord[0] = iImgCornerCoord[2];
	iImgCornerCoord[2] = tempVal;
	tempVal = iImgCornerCoord[1];
	iImgCornerCoord[1] = iImgCornerCoord[3];
	iImgCornerCoord[3] = tempVal;
	}

/**
	Transform image corner coordinates in case of vertical flip
*/
void TPositionProcessor::VerFlipCoordinates()
	{
	TUint8 tempVal = iImgCornerCoord[0];
	
	iImgCornerCoord[0] = iImgCornerCoord[1];
	iImgCornerCoord[1] = tempVal;
	tempVal = iImgCornerCoord[2];
	iImgCornerCoord[2] = iImgCornerCoord[3];
	iImgCornerCoord[3] = tempVal;
	}

/**
	Moves to the first position of MCU in case of MCU transformation and pixel coordinate in case of data unit cooefficient transformation
*/
void TPositionProcessor::MoveFirst()
	{
	TUint32* transformType = reinterpret_cast<TUint32*>(iImgCornerCoord);

	switch (*transformType)
		{
	case ENormalEncode:
		iCurrentPosition = iPixelFrame.iTl;
		break;
	case EEncodeRotate90:
		iCurrentPosition.SetXY(iPixelFrame.iTl.iX, iPixelFrame.iBr.iY - iPixelBlock.iHeight);
		break;
	case EEncodeHorizontalFlip:
		iCurrentPosition.SetXY(iPixelFrame.iTl.iX, iPixelFrame.iBr.iY - iPixelBlock.iHeight);
		break;
	case EEncodeVerticalFlip:
		iCurrentPosition.SetXY(iPixelFrame.iBr.iX - iPixelBlock.iWidth, iPixelFrame.iTl.iY);
		break;
	case EEncodeRotate180:
		iCurrentPosition.SetXY(iPixelFrame.iBr.iX - iPixelBlock.iWidth, iPixelFrame.iBr.iY - iPixelBlock.iHeight);
		break;
	case EEncodeRotate270:
		iCurrentPosition.SetXY(iPixelFrame.iBr.iX - iPixelBlock.iWidth, iPixelFrame.iTl.iY);
		break;
	case EEncodeHorizontalFlipRotate90:
		iCurrentPosition.SetXY(iPixelFrame.iBr.iX - iPixelBlock.iWidth, iPixelFrame.iBr.iY - iPixelBlock.iHeight);
		break;
	case EEncodeVerticalFlipRotate90:
		iCurrentPosition = iPixelFrame.iTl;
		break;
	default:
		ASSERT(EFalse);
		}	
	}

/**
	Moves to the next position of MCU in case of MCU transformation and pixel coordinate in case of data unit cooefficient transformation
*/
void TPositionProcessor::MoveNext()
	{	
	TUint32* transformType = reinterpret_cast<TUint32*>(iImgCornerCoord);
	switch (*transformType)
		{
	case ENormalEncode:
		{
		iCurrentPosition.iX += iPixelBlock.iWidth;
		if (iCurrentPosition.iX >= iPixelFrame.iBr.iX)
			{
			if (iCurrentPosition.iY + iPixelBlock.iHeight>= iPixelFrame.iBr.iY)
				{
				iIsEOF = ETrue;
				return;
				}			
			iCurrentPosition.iX = iPixelFrame.iTl.iX;
			iCurrentPosition.iY += iPixelBlock.iHeight;
			}		
		}
		break;
	case EEncodeRotate90:
		{
		iCurrentPosition.iY -= iPixelBlock.iHeight;
		if (iCurrentPosition.iY < iPixelFrame.iTl.iY)
			{
			if (iCurrentPosition.iX + iPixelBlock.iWidth >= iPixelFrame.iBr.iX)
				{
				iIsEOF = ETrue;
				return;
				}			
			iCurrentPosition.iY = iPixelFrame.iBr.iY - iPixelBlock.iHeight;
			iCurrentPosition.iX += iPixelBlock.iWidth;
			}
		}
		break;
	case EEncodeHorizontalFlip:
		{
		iCurrentPosition.iX += iPixelBlock.iWidth;
		
		if (iCurrentPosition.iX >= iPixelFrame.iBr.iX)
			{
			if (iCurrentPosition.iY <= iPixelFrame.iTl.iY)
				{
				iIsEOF = ETrue;
				return;
				}			
			iCurrentPosition.iX = iPixelFrame.iTl.iX;
			iCurrentPosition.iY -= iPixelBlock.iHeight;
			}
		}
		break;
	case EEncodeVerticalFlip:
		{
		iCurrentPosition.iX -= iPixelBlock.iWidth;
		if (iCurrentPosition.iX < iPixelFrame.iTl.iX)
			{
			if (iCurrentPosition.iY + iPixelBlock.iHeight>= iPixelFrame.iBr.iY)
				{
				iIsEOF = ETrue;
				return;
				}			
			iCurrentPosition.iX = iPixelFrame.iBr.iX - iPixelBlock.iWidth;
			iCurrentPosition.iY += iPixelBlock.iHeight;
			}
		}
		break;
	case EEncodeRotate180:
		{
		iCurrentPosition.iX -= iPixelBlock.iWidth;
		if (iCurrentPosition.iX < iPixelFrame.iTl.iX)
			{
			if (iCurrentPosition.iY - iPixelBlock.iHeight < iPixelFrame.iTl.iY)
				{
				iIsEOF = ETrue;
				return;
				}			
			iCurrentPosition.iX = iPixelFrame.iBr.iX - iPixelBlock.iWidth;
			iCurrentPosition.iY -= iPixelBlock.iHeight;
			}
		}
		break;
	case EEncodeRotate270:
		{
		iCurrentPosition.iY += iPixelBlock.iHeight;
		if (iCurrentPosition.iY + iPixelBlock.iHeight > iPixelFrame.iBr.iY )
			{
			if (iCurrentPosition.iX <= iPixelFrame.iTl.iX)
				{
				iIsEOF = ETrue;
				return;
				}			
			iCurrentPosition.iY = iPixelFrame.iTl.iY;
			iCurrentPosition.iX -= iPixelBlock.iWidth;
			}
		}
		break;
	case EEncodeHorizontalFlipRotate90:
		{
		iCurrentPosition.iY -= iPixelBlock.iHeight;
		if (iCurrentPosition.iY < iPixelFrame.iTl.iY)
			{
			if (iCurrentPosition.iX - iPixelBlock.iWidth < iPixelFrame.iTl.iX)
				{
				iIsEOF = ETrue;
				return;
				}
			iCurrentPosition.iY = iPixelFrame.iBr.iY - iPixelBlock.iHeight;
			iCurrentPosition.iX -= iPixelBlock.iWidth;
			}
		}
		break;
	case EEncodeVerticalFlipRotate90:
		{
		iCurrentPosition.iY += iPixelBlock.iHeight;
		if (iCurrentPosition.iY + iPixelBlock.iHeight > iPixelFrame.iBr.iY)
			{
			if (iCurrentPosition.iX + iPixelBlock.iWidth >= iPixelFrame.iBr.iX)
				{
				iIsEOF = ETrue;
				return;
				}
			iCurrentPosition.iY = iPixelFrame.iTl.iY;
			iCurrentPosition.iX += iPixelBlock.iWidth;
			}
		}
		break;
	default:
		ASSERT(EFalse);
		}
	}

