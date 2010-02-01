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

#include "ExifEncoder.h"
#include "ExifTagDescriptions.h"
#include "ExifTagHelper.h"

CExifEncoder* CExifEncoder::NewL(TBool aEncodeThumbnail)
	{
	CExifEncoder* self = new (ELeave) CExifEncoder(aEncodeThumbnail);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CExifEncoder::CExifEncoder(TBool aEncodeThumbnail)
	{
	iEncodeThumbnail = aEncodeThumbnail;
	}

CExifEncoder::~CExifEncoder()
	{
	delete iExifData;
	}
void CExifEncoder::SetIfd1L()
	{
	User::LeaveIfError(KErrNotSupported);
	}

TInt CExifEncoder::GetParam8(TUint /*aTag*/, TUint /*aIfd*/, HBufC8*& /*aParam*/) const
	{
	return KErrNotSupported;
	}

TInt CExifEncoder::GetParam16(TUint /*aTag*/, TUint /*aIfd*/, HBufC16*& /*aParam*/) const
	{
	return KErrNotSupported;
	}
	
TInt CExifEncoder::GetIntegerParam(TUint /*aTag*/, TUint /*aIfd*/, TInt& /*aParam*/) const
	{
	return KErrNotSupported;
	}

TInt CExifEncoder::GetRationalParam(TUint /*aTag*/, TUint /*aIfd*/, TInt& /*aNumerator*/, TInt& /*aDenominator*/) const
	{
	return KErrNotSupported;
	}
	
TInt CExifEncoder::GetShortParam(TUint /*aTag*/, TUint /*aIfd*/, TUint16& /*aParam*/) const
	{
	return KErrNotSupported;
	}
	
TInt CExifEncoder::GetIntegerArrayParam(TUint /*aTag*/, TUint /*aIfd*/, CArrayFix<TInt>& /*aParam*/) const
	{
	return KErrNotSupported;
	}
	
TInt CExifEncoder::GetShortArrayParam(TUint /*aTag*/, TUint /*aIfd*/, CArrayFix<TUint16>& /*aParam*/) const
	{
	return KErrNotSupported;
	}
	
TInt CExifEncoder::GetRationalArrayParam(TUint /*aTag*/, TUint /*aIfd*/, CArrayFix<TRational>& /*aParam*/) const
	{
	return KErrNotSupported;
	}

	
void CExifEncoder::ConstructL()
	{
    InitializeIfdsL(EFalse,ETrue);
	}
	
void CExifEncoder::SetEnableThumbnailL(TBool aEnable)
	{
	if (aEnable == iEncodeThumbnail)
		{
		return;
		}
	if (aEnable)
		{
		CIfd1* ifd = CIfd1::NewLC();
		iIfds.AppendL(ifd);
		CleanupStack::Pop(ifd);
		}
	else
		{
		int index;
		if (FindIfdIndex(EFirst, index))
			{
			delete iIfds[index];
			iIfds.Remove(index);
			}
		}
	iEncodeThumbnail = aEnable;	
	}

//	Creates the Exif Header proper, i.e. by pulling together all the appropriate data held in the IFD's.
TPtrC8 CExifEncoder::CreateExifHeaderL()
	{
	if(iExifData != NULL)
		{
		delete iExifData;
		iExifData = NULL;
		}
		
		
	iExifData = CreateExifHeaderBaseL();

	return TPtrC8(iExifData->Ptr(), iExifData->Length());
	}
	
	
TInt CExifEncoder::ThumbnailLengthOffset()
	{
	if(!iEncodeThumbnail)
		{
		return KErrNotSupported;
		}
	TUint offset = 0;
	TInt err = GetThumbnailLengthOffsetBase(offset);
	if (err!=KErrNone)
		{
		return err;
		}
	
	return offset;
	}

TInt CExifEncoder::ExifLengthOffset()
	{
	return KOffsetToApp1Size;
	}
