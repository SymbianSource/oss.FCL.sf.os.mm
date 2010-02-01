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

#ifndef __COLOURDEFS_H__

#define __COLOURDEFS_H__

/** 	@file
	@internalTechnology */

typedef TUint32 TRgbaColour;


const TUint KOpaqueAlpha	=0xFFu;
const TUint KTransparentAlpha=0;

const TUint KRedMask		=0x00FF0000u;
const TUint KGreenMask		=0x0000ff00u;
const TUint KBlueMask		=0x000000FFu;
const TUint KAlphaMask		=0xff000000u;
const TUint KRgbMask		=0x00ffffffu;

const TInt KRedShift		=16;
const TInt KGreenShift		=8;
const TInt KBlueShift		=0;
const TInt KAlphaShift		=24;

const TInt KAlphaPixelSize	=1;
const TInt KRgbPixelSize	=3;
const TInt KRgbaPixelSize	=4;

inline 
TInt Red(TRgbaColour aValue) 
	{
	return ((aValue & KRedMask)>>KRedShift);
	}

inline 
TInt Green(TRgbaColour aValue)
	{
	return((aValue & KGreenMask)>>KGreenShift);
	}

inline 
TInt Blue(TRgbaColour aValue)
	{
	return((aValue & KBlueMask)>>KBlueShift);
	}
	
inline 
TInt Alpha(TRgbaColour aValue)
	{
	return((aValue & KAlphaMask)>>KAlphaShift);
	}

#endif // __COLOURDEFS_H__
