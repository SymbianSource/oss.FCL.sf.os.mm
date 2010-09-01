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
 @internalComponent 
*/


#ifndef MISCUTILS_H

#define MISCUTILS_H

inline
TInt MulDiv(TInt aRefNum, TInt aMul, TInt aDiv)
	{
	return I64INT(TInt64(aRefNum)	*aMul/aDiv);
	}

inline
TSize ScaleSize(const TSize& aRefSize, const TSize& aSecondSize,const TSize& aToBeScaled)
	{
	return TSize(
					MulDiv(aToBeScaled.iWidth , aRefSize.iWidth, aSecondSize.iWidth),
					MulDiv(aToBeScaled.iHeight, aRefSize.iHeight,aSecondSize.iHeight)
					);
	}

inline
TBool IsSize2Exceeds1(const TSize& aRefSize,const TSize& aRefSize2)
	{
	return (aRefSize2.iWidth > aRefSize.iWidth || aRefSize2.iHeight > aRefSize.iHeight);
	}

inline
void RotateSize(TSize& aSize, TUint aRotationOptions)
	{
	if (((aRotationOptions&CImageDisplay::EOptionRotateCw90)!=0) ^ ((aRotationOptions&CImageDisplay::EOptionRotateCw270)!=0))
		{
		aSize.SetSize(aSize.iHeight, aSize.iWidth);
		}
	}
#endif // ndef MISCUTILS_H

