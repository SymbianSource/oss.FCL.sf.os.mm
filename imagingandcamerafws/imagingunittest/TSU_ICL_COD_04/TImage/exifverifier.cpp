// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ImageUtils.h"
#include "ifdgeneral.h"
#include "ifdgeneralentry.h"
#include "exifverifier.h"

/*static*/
CExifVerifier* CExifVerifier::NewL(const TDesC& aFileName, CExifVerifier::TFileType aFileType)
	{
	CExifVerifier* self = new (ELeave) CExifVerifier();
	CleanupStack::PushL(self);
	self->ConstructL(aFileName, aFileType);
	CleanupStack::Pop(self);
	return self;	
	}

CExifVerifier::~CExifVerifier() 
	{
	delete iDecoder;
	iFs.Close();
	}
	
void CExifVerifier::ConstructL(const TDesC& aFileName, TFileType aFileType)
	{
	// use const_cast since the shared implementation beneath is also used
	// for the encoder and transform, and hence cannot be const
	if (aFileType == EJpegFile)
		{
		// intitialize the decoder
		iDecoder = static_cast<CJPEGExifDecoder*>(CImageDecoder::FileNewL(iFs, aFileName, CImageDecoder::EOptionNone, KImageTypeJPGUid));
		iExifReader = static_cast<CExifReaderWriter*>(iDecoder->ExifMetadata());
		}
	else
		{
		// Currently we only know about Jpeg files, but this may not always be the case
		User::Leave(KErrNotSupported);
		}
	}


MExifMetadata* CExifVerifier::ExifMetadata() 
	{
	return iDecoder->ExifMetadata();
	}
	
	
CExifVerifier::TVerifyResult CExifVerifier::VerifyIfdOffsets() 
	{
	// walk through the Ifds and check the 16 bit alignment
	if (!iExifReader) 
		{
		return EExifBlockNotPresent;
		}
		
	RPointerArray<CIfdGeneral>& ifds = iExifReader->iIfds;
	TInt count = ifds.Count();
	TVerifyResult result;
	for (TInt i=0;i<count;i++) 
		{
		CIfdGeneral& ifd = *ifds[i];	
		result = CheckOffset(ifd.iOffsetToIfd);
		if (result != EOk)
			{
			return result;
			}
			
		TInt entryCount = ifd.EntryCount();
		// walk through each Ifd Entry and check that the directory entry 
		// and any extra data is 16 bit word aligned
		for (TInt j =0; j<entryCount ; j++) 
			{
			CIfdGeneralEntry& entry = *ifd.iIfdEntries[j];

			if (entry.ExtraSize()>0) 
				{
				// if data is stored outside the directory entry, check the offset
				result = CheckOffset(entry.ValueOffset());
				if (result != EOk)
					{
					return result;
					}
				}

			}
			
		}
	return EOk;
	}


CExifVerifier::TVerifyResult CExifVerifier::CheckOffset(TUint aOffset) const
	{
	ASSERT(iExifReader);
	if (aOffset < iExifReader->iExifDataLength)
		{
		if (aOffset % 2 == 1)
			{
			return EOffsetNotWordAligned;
			}
		else
			{
			return EOk;			
			}
		}
	else
		{
		return EOffsetOutOfRange;
		}
	}
