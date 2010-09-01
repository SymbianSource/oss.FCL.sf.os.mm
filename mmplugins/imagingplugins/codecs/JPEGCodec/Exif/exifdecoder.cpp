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

#include <f32file.h>
#include "ExifTagHelper.h"

#include "exifdecoder.h"
#include "ImageUtils.h"
#include "ExifTagDescriptions.h"


// Setters (not supported - this is a decoder)
TInt CExifDecoder::SetParam8(TUint /*aTag*/, TUint /*aIfd*/, HBufC8* /*aParam*/)
	{
	return KErrNotSupported;
	}

TInt CExifDecoder::SetParam16(TUint /*aTag*/, TUint /*aIfd*/, HBufC16* /*aParam*/)
	{
	return KErrNotSupported;	
	}

TInt CExifDecoder::SetIntegerParam(TUint /*aTag*/, TUint /*aIfd*/, TInt /*aParam*/)
	{
	return KErrNotSupported;
	}
	
TInt CExifDecoder::SetShortParam(TUint /*aTag*/, TUint /*aIfd*/, TUint16 /*aParam*/)
	{
	return KErrNotSupported;
	}
	
TInt CExifDecoder::SetRationalParam(TUint /*aTag*/, TUint /*aIfd*/, TInt /*aNumerator*/, TInt /*aDenominator*/)
	{
	return KErrNotSupported;
	}
	
TInt CExifDecoder::SetIntegerArrayParam(TUint /*aTag*/, TUint /*aIfd*/, CArrayFix<TInt>& /*aParam*/)
	{
	return KErrNotSupported;
	}
	
TInt CExifDecoder::SetShortArrayParam(TUint /*aTag*/, TUint /*aIfd*/, CArrayFix<TUint16>& /*aParam*/)
	{
	return KErrNotSupported;
	}
	
TInt CExifDecoder::SetRationalArrayParam(TUint /*aTag*/, TUint /*aIfd*/, CArrayFix<TRational>& /*aParam*/)
	{
	return KErrNotSupported;
	}

/*static*/
CExifDecoder* CExifDecoder::NewL(TUint aApp1Size, const TUint8* aData)
	{
	CExifDecoder* self = new (ELeave) CExifDecoder();
	CleanupStack::PushL(self);
	self->ConstructL(aData, aApp1Size);
	CleanupStack::Pop(self);
	return self;	
	}

CExifDecoder::CExifDecoder()
	{
	}
	
void CExifDecoder::ConstructL(const TUint8* aData, TUint aApp1Size)
	{
	// use const_cast since the shared implementation beneath is also used
	// for the encoder and transform, and hence cannot be const
	TUint8* dataPtr = const_cast<TUint8*>(aData);
	InitializeIfdsL(dataPtr, aApp1Size);
	}

CExifDecoder::~CExifDecoder()
	{
	}
	
void CExifDecoder::SetIfd1L()
	{
	TUint offsetToFirstIfd = 0;
	
	CIfdGeneral* ifd0 = GetIfd(EZeroth);
	ASSERT(ifd0);

	if (ifd0->GetOffsetToFirstIfd(offsetToFirstIfd) == KErrNone)
		{
		if ((offsetToFirstIfd != 0) && (CheckExifOffset(offsetToFirstIfd) == KErrNone))
			{
			CIfd1* ifd1 = CIfd1::NewLC(offsetToFirstIfd, iBase, iIntel, iExifDataLength);
			iIfds.AppendL(ifd1);
			CleanupStack::Pop(ifd1);

			SetThumbnailDataBaseL(ifd1, EFalse);	
			}		
		}
	}
	
