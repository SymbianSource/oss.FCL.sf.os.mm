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
// Returns the current position of MCU Top Left coordinate in case of MCU transformation and pixel coordinate in case of data unit cooefficient transformation
// 
//

/**
 @file
 @internalComponent
*/
FORCEDINLINE void TPositionProcessor::GetCurrentPosition(TPoint& aPos) const
	{
	aPos = iCurrentPosition;	
	}
	
/**
	Returns true if end of image.
*/
FORCEDINLINE TBool TPositionProcessor::IsEndOfImage() const
	{
	//return true if end of file
	return iIsEOF;
	}

/**
	Returns true if the dimensions need to be swapped.
*/
FORCEDINLINE TBool TPositionProcessor::SwapDimensions() const
	{
	//return true if end of file
	return iSwapDimensions;
	}

/**
	Returns true if transformation is needed. 
*/
FORCEDINLINE TBool TPositionProcessor::IsTransformNeeded() const
	{
	//return true if transformation is not needed
	if(*(TUint32*) iImgCornerCoord != ENormalEncode)
		{
		return ETrue;	
		}
	else
		{
		return EFalse;
		}		
	}



    
