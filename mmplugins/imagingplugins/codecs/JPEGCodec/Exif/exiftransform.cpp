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
#include "exiftransform.h"

#include "ImageUtils.h"
#include "ExifTagHelper.h"
#include "ExifGeneralConsts.h"
#include "ifdgeneral.h"
#include "ExifTagDescriptions.h"
	
/*static*/
CExifTransform* CExifTransform::NewL(const TUint8* aData, const TUint aApp1Size)
	{
	CExifTransform* self = new (ELeave) CExifTransform();
	CleanupStack::PushL(self);
	self->ConstructL(aData, aApp1Size);
	CleanupStack::Pop(self);
	return self;	
	}


CExifTransform::CExifTransform()
	{
	}

	
void CExifTransform::ConstructL(const TUint8* aData, const TUint aApp1Size)
	{
	if(aData)
		{
		TUint8* dataPtr = const_cast<TUint8*>(aData);
		InitializeIfdsL(dataPtr, aApp1Size);
		}
	else
		{
	    InitializeIfdsL(ETrue, EFalse);
		}
	}


CExifTransform::~CExifTransform()
	{
	}	

void CExifTransform::SetIfd1L()
	{
	TUint offsetToFirstIfd = 0;
	
	CIfdGeneral* ifd0 = GetIfd(EZeroth);
	ASSERT(ifd0);

	User::LeaveIfError(ifd0->GetOffsetToFirstIfd(offsetToFirstIfd));
	
	if (CheckExifOffset(offsetToFirstIfd) != KErrNone)
		{
		offsetToFirstIfd = 0;
		}

	CIfd1* ifd1 = CIfd1::NewLC(offsetToFirstIfd, offsetToFirstIfd ? iBase : NULL, iIntel, iExifDataLength);
	iIfds.AppendL(ifd1);
	CleanupStack::Pop(ifd1);
	
	if (offsetToFirstIfd)
		{
		SetThumbnailDataBaseL(ifd1, ETrue);
		}	
	}

TInt CExifTransform::CreateExifChunk(HBufC8*& aExifChunk)
	{
	TRAPD(err, CreateExifHeaderL(aExifChunk));
	return err;
	}
	
//	Creates the Exif Header proper, i.e. by pulling together all the appropriate data held in the IFD's.
void CExifTransform::CreateExifHeaderL(HBufC8*& aBuffer)
	{
	if(aBuffer != NULL)
		{
		delete aBuffer;
		aBuffer=NULL;
		}
	
	if(!GetJpegThumbnailData())
		{
		// we have to remove all the thumbnail data	
		RemoveThumbnailData();
		}
	
	aBuffer = CreateExifHeaderBaseL();
	}
	
void CExifTransform::CheckUpdateMandatoryTagsL()
	{
	CIfdGeneral* ifd = NULL;
	for (TInt i = 0; i < iIfds.Count(); i++)
		{
		ifd = iIfds[i];
		ifd->CheckMandatoryEntriesL();
		}
	}

TBool CExifTransform::CheckImageSizeTags()
	{
	CExifIfd* exifIfd = static_cast<CExifIfd*> (GetIfd(EExifSub));
	if (exifIfd)
		{
		return exifIfd->CheckImageSizeTags();
		}
	return EFalse;
	}

void CExifTransform::UpdateImageSizeTagsL(const TSize& aSize)
	{
	CExifIfd* exifIfd = static_cast<CExifIfd*> (GetIfd(EExifSub));
	if (exifIfd)
		{
		exifIfd->UpdateImageSizeTagsL(aSize);
		}
	}
