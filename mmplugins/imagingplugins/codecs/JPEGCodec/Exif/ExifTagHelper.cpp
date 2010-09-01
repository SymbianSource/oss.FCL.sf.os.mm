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

#include "ExifTagHelper.h"
#include "ExifTagDescriptions.h"
	
// Return ETrue if the given tag is in the given Ifd.
TBool TExifTagHelper::IsCorrectParentIfd(TUint aTag, TUint aIfd)
	{
	for(TInt i = 0; i < KIfd0NumberTags; i++)
		{
		if((KIfd0TagArray[i][ETagValue] == aTag) && (KIfd0TagArray[i][EIfdValue] == aIfd))
			{
			return ETrue;
			}
		}
		
		
	for(TInt i = 0; i < KExifSubNumberTags; i++)
		{
		if((KExifSubTagArray[i][ETagValue] == aTag) && (KExifSubTagArray[i][EIfdValue] == aIfd))
			{
			return ETrue;
			}
		}

	for(TInt i = 0; i < KInteropNumberTags; i++)
		{
		if((KInteropTagArray[i][ETagValue] == aTag) && (KInteropTagArray[i][EIfdValue] == aIfd))
			{
			return ETrue;
			}
		}

		
	for(TInt i = 0; i < KIfd1NumberTags; i++)
		{
		if((KIfd1TagArray[i][ETagValue] == aTag) && (KIfd1TagArray[i][EIfdValue] == aIfd))
			{
			return ETrue;
			}
		}

	for(TInt i = 0; i < KGpsSubNumberTags; i++)
		{
		if((KGpsSubTagArray[i][ETagValue] == aTag) && (KGpsSubTagArray[i][EIfdValue] == aIfd))
			{
			return ETrue;
			}
		}

	return EFalse;
	}


HBufC8* TExifTagHelper::CreateUnicodePrefixedBufferL(const TDesC16& aUnicodeBuffer)
	{
	HBufC8* result = HBufC8::NewL((aUnicodeBuffer.Length()*2) + KUnicode().Length());			
	result->Des().Append(KUnicode);
	result->Des().Append(TPtrC8(reinterpret_cast<const TText8*>(aUnicodeBuffer.Ptr()),aUnicodeBuffer.Size()));
	return result;
	}

// Returns ETrue if aInternalIfdNum is in the TIfd enum
TBool TExifIfdHelper::IsValidInternalIfd(const TInt aInternalIfdNum)
	{
	return (EZeroth <= aInternalIfdNum && aInternalIfdNum <= EMaxIfd);	
	}
	
// Attempts to extract the (internal) Ifd0 sub Ifd "aSubIfd" from the given 
// aExternalIfdNum. Returns ETrue if successful, else EFalse.
// The internal sub Ifd value will be found in the upper 16 bits of the
// external Ifd number.
// This is only valid for sub Ifd's of Ifd0.
TBool TExifIfdHelper::IfdContainsIfd0SubIfd(const TInt aExternalIfdNum, const TInt aSubIfd)
	{
	return ( (!(aExternalIfdNum & 0x0000FFFF)) && //IFD 0 
	          (((aExternalIfdNum & 0xFFFF0000)>>16) == aSubIfd) ); //aSubIfd
	}
