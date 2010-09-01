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
#include "exifreaderwriter.h"

#include "ImageUtils.h"
#include "ExifTagHelper.h"
#include "ExifGeneralConsts.h"
#include "ifdgeneral.h"
#include "ExifTagDescriptions.h"

// used for ordering exif tags when writing them
const TInt KNumIfds = 5;
const TInt KIfdOrder[KNumIfds] = 
	{
	EZeroth, 
	EExifSub,
	EInterop,
	EGpsSub,
	EFirst
	};
	
HBufC8* CExifReaderWriter::CreateExifHeaderBaseL()
	{
	//write the GPS IFD tag in IFD 0 - need to write it twice to ensure correct offset
	//the second write we have the correct offset
	for (TInt i = 0; i < iIfds.Count(); i++)
		{
		if ((iIfds)[i]->Ifd() == EGpsSub)
			{
			if ((iIfds)[i]->Size())
				{
				TUint gpsIFDoffset = KIfdOffset;
				TInt result = SetIntegerParam(KTag8825[ETagValue], EZeroth, gpsIFDoffset);
				if (result != KErrNone)
					{
					User::Leave(result);
					}
				//loop through IFDs
				for (TUint j = 0; j < iIfds.Count(); j++)
	 				{
	 				//calculate the offset to the GPS IFD 
	 				TInt ifd = (iIfds)[j]->Ifd();
	 				if (ifd <= EInterop)
	 					{
		 				gpsIFDoffset += (iIfds)[j]->Size() + KSizeOfEntryCount;
	 					gpsIFDoffset += KIfdOffsetByteCount; // for size of pointer to next Ifd.
	 					}
	 				}
	 			//write the offset to IFD0 8825 GPS tag
				result = SetIntegerParam(KTag8825[ETagValue], EZeroth, gpsIFDoffset);
				if (result != KErrNone)
					{
					User::Leave(result);
					}
				}
			}
		}
		
	// Need to obtain the size of each IFD object in order to determine initial
	// alloc size of the total Exif data block.
	TUint allocSize = 0;
	
	// Account for the Exif file header data.
	allocSize = allocSize + KExifFileHeaderLength;
	
	
	// Account for the thumbnail
	if(iJpegThumbnailData)
		{
		allocSize += iJpegThumbnailData->Length();
		}
	
	// Account for each Ifd.
	TInt i;
	for (i = 0; i < iIfds.Count(); i++)
		{
		TUint size = iIfds[i]->Size();
		if (size > 0)
			{
			allocSize += KSizeOfEntryCount; 	// Need to allocate room for the Ifd entry count also.
			allocSize += size;
			allocSize += KIfdOffsetByteCount;	// Account for the offset to the next Ifd.
			}
		}

	
	// Maximum legal size of APP1 header is 64K.
	if (allocSize > KMaxApp1Size)
		{
		User::Leave(KErrTooBig);
		}
		
	// Allocate required buffer.
	HBufC8* buffer = HBufC8::NewL(allocSize);
	CleanupStack::PushL(buffer);
	iData = const_cast<TUint8*>(buffer->Des().Ptr());
	TUint8* tmpPtr = iData;
	
	WriteExifFileHeaderBaseL(allocSize - KBlockEndMarkerByteCount, tmpPtr);
		
	// Write each Ifd in turn.
	TUint currentOffset = KIfdOffset;
	for (TInt i = 0; i < KNumIfds; i++)
		{
		CIfdGeneral* ifd = GetIfd(KIfdOrder[i]);
		if (ifd != NULL) 
			{
			TUint16 entryCount = ifd->EntryCount();
			if (entryCount) 
				{
				TUint8* tempIfdBlock = ifd->CreateIfdBlockL(currentOffset);
				
				// Write Ifd entry count.		
				tmpPtr = Mem::Copy(tmpPtr, &entryCount, KSizeOfEntryCount);
				
				// Write the Ifd to the Exif data block.
				TInt length = ifd->Size();
				length += KIfdOffsetByteCount; // for size of pointer to next Ifd.
				tmpPtr = Mem::Copy(tmpPtr, tempIfdBlock, length);
						
				// Free data block created for this Ifd.
				User::Free(tempIfdBlock);

				currentOffset += length + KSizeOfEntryCount;
				}		
			}
		} // end for
	
	SetFirstIfdOffsetBase();
	
	// finally, embed the thumbnail
	if (iJpegThumbnailData)
		{
		// Now know the size of the other Ifd's, can set the offset to the 1st Ifd.
		
		TUint thumbnailLength = iJpegThumbnailData->Length();
		//return value the copy operation is not used, so discarded
		Mem::Copy(tmpPtr, iJpegThumbnailData->Ptr(), thumbnailLength);
	
		// No need to account for EOI as it is written elsewhere.

		// If the source does not have JPEGInterchangeFormatLength (tag 0x0202) then
		// ThumbnailLengthOffset will return an error. 0x0202 is mandatory with thumbs
		// so this should not happen except with corrupt sources.
		TUint thumbnailLengthOffset = 0;
		if (GetThumbnailLengthOffsetBase(thumbnailLengthOffset) == KErrNone)
			{
			Mem::Copy(iData + thumbnailLengthOffset, &thumbnailLength, sizeof(TUint));
			}
		}

	buffer->Des().SetLength(allocSize);
	CleanupStack::Pop();
	return buffer;
	}
	
CExifReaderWriter::CExifReaderWriter() 
	{
	}

// Set 8-bit data.
TInt CExifReaderWriter::SetParam8(TUint aTag, TUint aIfd,  HBufC8* aParam)
	{
	TInt err = DoSetParam8(aTag, aIfd, aParam);
	if(err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}

// Set 16-bit data.  Only used for Unicode 'User Comment' tag.
TInt CExifReaderWriter::SetParam16(TUint aTag, TUint aIfd,  HBufC16* aParam)
	{
	TInt err = DoSetParam16(aTag, aIfd, aParam);
	if(err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}

TInt CExifReaderWriter::SetIntegerParam(TUint aTag, TUint aIfd,  TInt aParam)
	{
	TInt err = DoSetIntegerParam(aTag, aIfd, aParam);
	if(err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}


TInt CExifReaderWriter::SetRationalParam(TUint aTag, TUint aIfd, TInt aNumerator, TInt aDenominator)
	{
	TInt err = DoSetRationalParam(aTag, aIfd, aNumerator, aDenominator);
	if( err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}
	
TInt CExifReaderWriter::SetShortParam(TUint aTag, TUint aIfd, TUint16 aParam)
	{
	TInt err = DoSetShortParam(aTag, aIfd, aParam);
	if( err == KErrNone )
		{
		iExifDataModified = ETrue;
		}
	return err;
	}
	
TInt CExifReaderWriter::SetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam)
	{
	TInt err = DoSetIntegerArrayParam(aTag, aIfd, aParam);
	if(err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}

TInt CExifReaderWriter::SetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam)
	{
	TInt err = DoSetRationalArrayParam(aTag, aIfd, aParam);
	if(err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}
	
TInt CExifReaderWriter::SetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam)
	{
	TInt err = DoSetShortArrayParam(aTag, aIfd, aParam);
	if(err == KErrNone)
		{
		iExifDataModified = ETrue;
		}
	return err;
	}

	
// Set 8-bit data.
TInt CExifReaderWriter::DoSetParam8(TUint aTag, TUint aIfd,  HBufC8* aParam)
	{
	if(aParam == NULL)
		{
		return KErrArgument;
		}
	
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
		
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		return internalIfd->SetParam8(aTag, aParam);	
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);		
		if (err == KErrNone) 
			{
			TRAP(err, internalIfd->AddParam8L(aTag, format, aParam->Length(), aParam));
			
			if (internalIfdNumber == EGpsSub)
				{
				ProcessGpsIfdAddParamResult(internalIfd, err);
				}				
			}
			
		return err;
		}		
	}

// Set 16-bit data.  Only used for Unicode 'User Comment' tag.
TInt CExifReaderWriter::DoSetParam16(TUint aTag, TUint aIfd,  HBufC16* aParam)
	{
	if(aParam == NULL)
		{
		return KErrArgument;
		}	
	
	// The only tags which may contain 16-bit data are 0x9286 (UserComment),
	// 0x001B (GpsProcessingMethod) or 0x001C (GpsAreaInformation).
	if(aTag != KTag9286[ETagValue] && aTag != KTagGPS001B[ETagValue] && aTag != KTagGPS001C[ETagValue])
		{
		return KErrArgument;
		}
		
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
		
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		return internalIfd->SetParam16(aTag, aParam);	
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			TRAP(err, internalIfd->AddParam16L(aTag, format, aParam->Length(), aParam));

			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}

		return err;
		}
	}
	
// Sets single integer data (including short, long and integer).
TInt CExifReaderWriter::DoSetIntegerParam(TUint aTag, TUint aIfd,  TInt aParam)
	{
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
		
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		return internalIfd->SetIntegerParam(aTag, aParam);	
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			// Sets a single integer value.
			TRAP(err, internalIfd->AddIntegerParamL(aTag, format, 1, aParam));
			
			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}
			
		return err;
		}
	}

// Sets single rational data (i.e. one numerator and one denominator).
TInt CExifReaderWriter::DoSetRationalParam(TUint aTag, TUint aIfd, TInt aNumerator, TInt aDenominator)
	{
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}

	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		return internalIfd->SetRationalParam(aTag, aNumerator, aDenominator);	
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			TRAP(err, internalIfd->AddRationalParamL(aTag, format, 1, aNumerator, aDenominator));

			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}
		return err;
		}

	}

TInt CExifReaderWriter::DoSetShortParam(TUint aTag, TUint aIfd, TUint16 aParam)
	{
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
		
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		return internalIfd->SetShortParam(aTag, aParam);	
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			// Sets a single integer value.
			TRAP(err, internalIfd->AddShortParamL(aTag, format, 1, aParam));
			
			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}
		return err;
		}
	}

TInt CExifReaderWriter::DoSetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam)
	{	
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
	
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		TRAP(err, internalIfd->SetIntegerArrayParamL(aTag, aParam));	
		return err;
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			// Sets a single integer value.
			TRAP(err, internalIfd->AddIntegerArrayParamL(aTag, format, aParam));
			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}
		return err;
		}
	}
	
TInt CExifReaderWriter::DoSetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam)
	{
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
	
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		TRAP(err, internalIfd->SetRationalArrayParamL(aTag, aParam));	
		return err;
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			// Sets a single integer value.
			TRAP(err, internalIfd->AddRationalArrayParamL(aTag, format, aParam));
			
			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}
			
		return err;
		}
	}

TInt CExifReaderWriter::DoSetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam)
	{
	TInt internalIfdNumber = 0;
	CIfdGeneral* internalIfd = NULL;
	TInt err = KErrNone;
	TRAP(err, FindInternalIfdDataL(aTag, aIfd, internalIfdNumber, internalIfd));
	if(err!=KErrNone)
		{
		return err;
		}
			
	if(internalIfd->EntryExists(aTag))
		{
		// Set the existing entry.
		TRAP(err, internalIfd->SetShortArrayParamL(aTag, aParam));	
		return err;
		}
	else
		{
		// Get the required parameters.
		TUint format = 0;
		err = internalIfd->GetFormat(aTag, format);
		if (err == KErrNone)
			{
			// Sets a single integer value.
			TRAP(err, internalIfd->AddShortArrayParamL(aTag, format, aParam));
			if (internalIfdNumber == EGpsSub)
				{
				err = ProcessGpsIfdAddParamResult(internalIfd, err);
				}
			}
		return err;
		}
	}


CExifReaderWriter::~CExifReaderWriter()
	{
	iIfds.ResetAndDestroy();
	delete iJpegThumbnailData;
	}

TInt CExifReaderWriter::GetIntegerParam(TUint aTag, TUint aIfd, TInt& aParam) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		err = ifd->GetIntegerParam(aTag, aParam);	
		}
	return err;
	}

TInt CExifReaderWriter::GetShortParam(TUint aTag, TUint aIfd, TUint16& aParam) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		err = ifd->GetShortParam(aTag, aParam);	
		}
	return err;
	}

TInt CExifReaderWriter::GetRationalParam(TUint aTag, TUint aIfd, TInt& aNumer, TInt& aDenom) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		err = ifd->GetRationalParam(aTag, aNumer, aDenom);	
		}
	return err;
	}

TInt CExifReaderWriter::GetParam8(TUint aTag, TUint aIfd, HBufC8*& aParam) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		err = ifd->GetParam8(aTag, aParam);	
		}
	return err;
	}

TInt CExifReaderWriter::GetParam16(TUint aTag, TUint aIfd, HBufC16*& aParam) const
	{
	// The only tags which may contain 16-bit data are 0x9286 (UserComment),
	// 0x001B (GpsProcessingMethod) or 0x001C (GpsAreaInformation).
	if(aTag != KTag9286[ETagValue] && aTag != KTagGPS001B[ETagValue] && aTag != KTagGPS001C[ETagValue])
		{
		return KErrArgument;
		}
		
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		err = ifd->GetParam16(aTag, aParam);	
		}
	return err;
	}

TInt CExifReaderWriter::GetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		TRAP(err, ifd->GetIntegerArrayParamL(aTag, aParam));	
		}
	return err;
	}

TInt CExifReaderWriter::GetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		TRAP(err, ifd->GetShortArrayParamL(aTag, aParam));	
		}
	return err;
	}

TInt CExifReaderWriter::GetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam) const
	{
	TInt err = KErrNotFound;
	CIfdGeneral* ifd = FindIfd(aTag, aIfd);
	
	if (ifd != NULL)
		{
		TRAP(err, ifd->GetRationalArrayParamL(aTag, aParam));	
		}
	return err;
	}

void CExifReaderWriter::InitializeIfdsL(TBool aAlwaysCreateIfd1, TBool aCreateGpsIfd)
	{
	// All of the IFD's should be present, because each has at least one Mandatory Ifd entry.	
	CIfd0* ifd0 = CIfd0::NewLC();
	iIfds.AppendL(ifd0);
	CleanupStack::Pop(ifd0);
	
	CExifIfd* exifIfd = CExifIfd::NewLC();
	iIfds.AppendL(exifIfd);
	CleanupStack::Pop(exifIfd);
	
	CInteropIfd* interopIfd = CInteropIfd::NewLC();
	iIfds.AppendL(interopIfd);
	CleanupStack::Pop(interopIfd);
	 
	if(iEncodeThumbnail || aAlwaysCreateIfd1)
		{
		CIfd1* ifd1 = CIfd1::NewLC();
		iIfds.AppendL(ifd1);
		CleanupStack::Pop(ifd1);
		}	
		
	if (aCreateGpsIfd) 
		{
		CGpsIfd* gpsIfd = CGpsIfd::NewLC();
		iIfds.AppendL(gpsIfd);
		CleanupStack::Pop(gpsIfd);
		}
	}


void CExifReaderWriter::InitializeIfdsL(TUint8* aData, TUint aApp1Size)
	{
	iData = aData; 
	
	// Note that iApp1Size includes 2bytes for the APP1 length field and 6bytes for the
	// EXIF header so these both need to be subtracted to determine the end of the EXIF
	// data.
	iExifDataLength = aApp1Size - KOffsetFromExifHeaderToByteAlignment - KApp1SizeLength;
	if(iData)
		{
		ReadHeaderL();
		SetUpIfdsL();
		}
	}
	
// Returns the internal Ifd (i.e. EIfd values) from the given external Ifd (i.e. 0th or 1st)
// that contains the provided tag.  This is because the Exif Sub Ifd and the Interoperability
// Ifd are both considered to be part of the 0th Ifd.
// Returns KErrArgument if not valid.
TInt CExifReaderWriter::InternalIfd(const TUint aTag, const TInt aExternalIfd) const
	{
	if(aExternalIfd == 0)
		{
		// Can be either 0th, Exif Sub or Interoperability Ifd.
		if(TExifTagHelper::IsCorrectParentIfd(aTag, EZeroth))
			{
			return EZeroth;			
			}
		if(TExifTagHelper::IsCorrectParentIfd(aTag, EExifSub))
			{
			return EExifSub;
			}
		if(TExifTagHelper::IsCorrectParentIfd(aTag, EInterop))
			{
			return EInterop;
			}
		if(TExifTagHelper::IsCorrectParentIfd(aTag, EGpsSub))
			{
			return EGpsSub;
			}		
		}
	else if(aExternalIfd == 1)
		{
		if(TExifTagHelper::IsCorrectParentIfd(aTag, EFirst))
			{
			return EFirst;
			}
		}

	//Check if upper 16 bits are used to specifiy a sub-IFD.
 	//Only IFD 0 sub IFDs can currently be checked for.
 	else if (TExifIfdHelper::IfdContainsIfd0SubIfd(aExternalIfd, EGpsSub))
 		{
 		return EGpsSub;
 		}
	return KErrNotFound;
	}
	
TInt CExifReaderWriter::CheckTagL(const TUint aTag, const TUint aIfd) const
	{
	TInt realIfd = InternalIfd(aTag, aIfd);
	
	if (!TExifIfdHelper::IsValidInternalIfd(realIfd))
		{
		User::Leave(KErrNotFound);
		}
	return realIfd;
	}


// Updates aIfdIndex with the index of the given IFD if found.
TBool CExifReaderWriter::FindIfdIndex(TInt aRealIfd, TInt& aIfdIndex) const
	{
	for(TInt i = 0; i < iIfds.Count(); ++i)
		{
		if(iIfds[i]->Ifd() == aRealIfd)
			{
			aIfdIndex = i;
			return ETrue;
			}
		}
	return EFalse;
	}

// Returns the CIfdGeneral* for this tag/ifd pair if it exists, else NULL
CIfdGeneral* CExifReaderWriter::FindIfd(const TUint aTag, const TUint aExternalIfd) const
	{
	TInt realIfd=0;
	TInt err = KErrNone;
	TRAP(err, realIfd=CheckTagL(aTag, aExternalIfd));
	if(err==KErrNone)
		{		
		TInt ifdIndex=0;
		if(FindIfdIndex(realIfd, ifdIndex))
			{
			return iIfds[ifdIndex];
			}
		}
	return NULL;
	}
	
// This is the preamble for most Set functions.  Its purpose is to fetch the Internal
// Ifd information for the given aTag/aExternalIfdNumber pair.  The Internal Ifd 
// information fetched consists of a pointer to the CIfd object and the Internal Ifd 
// Number.
//
// Note: If aExternalIfdNumber corresponds to the GPS Ifd, then it is possible that the 
// Ifd object for it does not exist yet.  This is because the GPS Ifd has no mandatory
// tags so it need not be present in metadata.  In this case, this method will create
// the Ifd object for GPS & add it to the iIfds array before returning its information
// to the caller.
//
// If it can not return an internal representation for the given aTag/aExternalIfdNumber 
// pair, then this method leaves with KErrNotSupported.
void CExifReaderWriter::FindInternalIfdDataL(const TUint aTag, const TUint aExternalIfdNumber, TInt& aInternalIfdNumber, CIfdGeneral*& aInternalIfd)
	{
	aInternalIfdNumber = CheckTagL(aTag, aExternalIfdNumber);

	//All the CExifReaderWriter::Set() methods used to use the realIfd as the array index.
	//However, since the GPS IFD is optional this is invalid & now we check to get 
	//the correct array index.	
	TInt ifdIndex = 0;
	if (!FindIfdIndex(aInternalIfdNumber, ifdIndex))
		{
		//ifd does not yet exist internally
		if (aInternalIfdNumber == EGpsSub)
			{
			//Currently this can only be GPS IFD since the other IFDs have madatory entries.
			//We need to create an empty GPS ifd. First create GPS IFD pointer in 0 IFD
			//We don't care about offset value at this point - this gets written in TExifAccessor::CreateExifHeaderL
			CGpsIfd* gpsIfd = CGpsIfd::NewLC(0, NULL, ETrue);
			iIfds.AppendL(gpsIfd);
			CleanupStack::Pop(gpsIfd);
			aInternalIfd = gpsIfd;
			}
		else
			{
			//ifd is real but there are no entries
			User::Leave(KErrNotFound);
			}
		}
	else
		{
		aInternalIfd = iIfds[ifdIndex];		
		}	
	}

// Helper Function
/*static*/
TUint CExifReaderWriter::ReadUint32(const TBool aIntelByteAlign, const TUint8* aPtr)
	{
	if(aIntelByteAlign)
		{
		return PtrReadUtil::ReadUint32(aPtr);
		}
	else // Motorola
		{
		return PtrReadUtil::ReadBigEndianUint32(aPtr);
		}
	}

// Helper Function	
/*static*/
TUint16 CExifReaderWriter::ReadUint16(const TBool aIntelByteAlign, const TUint8* aPtr)
	{
	if(aIntelByteAlign)
		{
		return PtrReadUtil::ReadUint16(aPtr);
		}
	else // Motorola
		{
		return PtrReadUtil::ReadBigEndianUint16(aPtr);
		}
	}


void CExifReaderWriter::ReadHeaderL()
	{
	// Check integrity of header.
	TPtrC8 exifHeaderPtr(iData, KExifHeaderLength);
	if(KExifHeader().Compare(exifHeaderPtr))
		{
		User::Leave(KErrCorrupt);
		}
	
	TUint16 byteAlignment = 0;	
	Mem::Copy(&byteAlignment, iData+KOffsetFromExifHeaderToByteAlignment, sizeof(byteAlignment));
	if(byteAlignment == KIntelByteAlignment)
		{
		iIntel = ETrue;		
		}
	else if(byteAlignment == KMotorolaByteAlignment)
		{
		iIntel = EFalse;
		}
	else
		{
		User::Leave(KErrCorrupt);
		}
			
	iBase = iData+KOffsetFromExifHeaderToByteAlignment;
	
	
	TUint16 readFortyTwo = ReadUint16(iIntel, iData + KOffsetFromExifHeaderToByteAlignment + sizeof(byteAlignment));
	if (readFortyTwo != KFortyTwo)
		{
		readFortyTwo = ReadUint16(!iIntel, iData + KOffsetFromExifHeaderToByteAlignment + sizeof(byteAlignment));
		if (readFortyTwo != KFortyTwo)
			{
			User::Leave(KErrArgument);
			}
		}
		
	iOffsetToZerothIfd = ReadUint32(iIntel, iData + KOffsetToZerothOffset);
	if ( CheckExifOffset(iOffsetToZerothIfd) != KErrNone )
		{
		iOffsetToZerothIfd = ReadUint32(!iIntel, iData + KOffsetToZerothOffset);
		if ( CheckExifOffset(iOffsetToZerothIfd) != KErrNone )
			{
			// If we've got this far then it probably really is an exif file
			// but the offset is corrupt
			iOffsetToZerothIfd = KIfdOffset; //so we'll try a default value
			}
		}
	}

/**
	Attempts to create a sub-IFD whose offset is given by an entry in another IFD and append
	it to iIfds.
	However, if the given tag is not found, the offset is wrong or the IFD data is corrupt
	then nothing will be appended to iIfds and NULL shall be returned.
	
	@param	aIfd	The IFD containing the entry with the offset to the sub-IFD.
	@param	aTag	The tag of the entry containing the offset.

	@return	Pointer to the sub-IFD that was created and appended to iIFds
			or NULL if it could not be created. (This does not transfer ownership
			of the IFD since it is owned by iIfds!)
			
	@leave	KErrNoMemory	If the sub-IFD could not be created due to OOM. (This
							is the only type of leave this function can make!)
	
*/
CIfdGeneral* CExifReaderWriter::CreateAndAppendSubIfdL(CIfdGeneral& aIfd, TUint aTag)
	{
	TInt offsetValue = 0;
	CIfdGeneral* subIfd = NULL;
	
	TInt err = aIfd.GetIntegerParam( aTag, offsetValue );
	if(err == KErrNone)
		{		
		// offset tag found	
		if((err = CheckExifOffset( offsetValue )) != KErrNone)
			{
			// offset wrong
			aIfd.RemoveEntryL( aTag );
			}
		else
			{
			// offset OK - try to create IFD
			if( aTag == KTag8769[ETagValue] )
				{
				// EXIF sub-IFD
				TRAP(err, subIfd = CExifIfd::NewL(offsetValue, iBase, iIntel, iExifDataLength) );
				}
			else if( aTag == KTagA005[ETagValue] )
				{
				// Interoperability sub-IFD
				TRAP(err, subIfd = CInteropIfd::NewL(offsetValue, iBase, iIntel, iExifDataLength) );
				}
			else if( aTag == KTag8825[ETagValue] )
				{
				// GPS sub-IFD
				TRAP(err, subIfd = CGpsIfd::NewL(offsetValue, iBase, iIntel, iExifDataLength) );
				}
			else
				{
				return NULL;
				}
				
			if( err != KErrNone )
				{
				// could not create IFD
				if( err == KErrNoMemory )
					{
					User::Leave( err );
					}
				else
					{
					aIfd.RemoveEntryL( aTag );
					}
				}
			else
				{
				// IFD created successfully
				CleanupStack::PushL(subIfd);
				iIfds.AppendL(subIfd);
				CleanupStack::Pop(subIfd);
				}
			}
		}
		return subIfd;
	}

/**
	Extracts IFDs and their entries from the EXIF data.
	If entries in an IFD are corrupt they will be ignored. If many corrupt
	entries occur in sequence the(sub-)IFD that contains them is considered
	corrupt and ignored. (See CIfdGeneral::AddAllIfdEntriesL() for more details)
	
	If IFD0 is corrupt this function will leave with KErrCorrupt, if any of the other
	(sub-)IFDs are corrupt they are ignored and the corresponding entry that points
	to them is removed. (e.g.: If the EXIF IFD is found to be corrupt then the EXIF IFD
	offset tag (0x8769) in IFD0 will be removed)

	@see 	CIfdGeneral::AddAllIfdEntriesL() 
	
	@leave	KErrNoMemory	If there is insufficient memory to process the data
	@leave	KErrCorrupt		If IFD0 is too corrupted for anything to be reliably
							extracted from it.
*/
void CExifReaderWriter::SetUpIfdsL()
	{
	// IFD0
	CIfd0* ifd0 = CIfd0::NewLC(iOffsetToZerothIfd, iBase, iIntel, iExifDataLength);
	iIfds.AppendL(ifd0);
	CleanupStack::Pop(ifd0);
	
	// EXIF sub-IFD
	CIfdGeneral* exifIfd = CreateAndAppendSubIfdL( *ifd0, KTag8769[ETagValue]);
	if( exifIfd )
		{
		// Interoperability sub-IFD
		CreateAndAppendSubIfdL( *exifIfd, KTagA005[ETagValue]);
		}
		
	// GPS sub-IFD
	CreateAndAppendSubIfdL( *ifd0, KTag8825[ETagValue]);

	// IFD1	
	TRAPD(err, SetIfd1L());
	if( err == KErrNoMemory )
		{
		User::Leave(err);
		}
	// else ignore
	}

/**
	Check that an offset relative to the EXIF data is valid.
	Note that offsets in the EXIF data are relative to iBase (the
	start of the EXIF data contained within the APP1 block) and not 
	iData (the start of the APP1 block).
	
	@param	aOffset	An offset pointing to an IFD or value within
					the EXIF data.
					
	@return	KErrNone	If the offset is between KIfdOffset (the 8 bytes of
						the TIFF header) and the end of the EXIF data (the
						end of the APP1 block minus the APP1 headers that
						precede the EXIF data)
			KErrCorrupt	If the offset is outside that range.
	
*/
TInt CExifReaderWriter::CheckExifOffset(const TUint aOffset)
	{
	TInt err = KErrNone;
	// ensure that aOffset is within a valid section of the EXIF data
	if ((aOffset < KIfdOffset) || (aOffset >= iExifDataLength))
		{
		err = KErrCorrupt;
		}
	return err;
	}
	
// This method is to be called after at attempt was made to add a param to a GPS Ifd.
// It must only be called with the GPS Ifd.
// 
// Its purpose is to process the result (aAddParamResult) of a CIfd::AddParamXXX() 
// operation on a GPS Ifd.
//
// It will add the GPS Ifd pointer to Ifd 0 if a param was sucessfully added to a 
// brand new GPS Ifd. (i.e. if aErr==KErrNone && aGpsIfd->EntryCount() == 1).
//
// If an attempt to add a param to a brand new GPS Ifd failed, or if the GPS Ifd pointer
// could not be added to Ifd 0, then the GPS Ifd is deleted and is returned.
TInt CExifReaderWriter::ProcessGpsIfdAddParamResult(CIfdGeneral*& aGpsIfd, TInt aAddParamResult)
	{
	ASSERT(aGpsIfd != NULL);
	ASSERT(aGpsIfd->Ifd() == EGpsSub);

	TInt result = KErrNone;
	if ((aAddParamResult==KErrNone) && (aGpsIfd->EntryCount() == 1))
		{
		// The very first GPS tag has just been added to this Ifd, so add GPS Ifd pointer 
		// to Ifd 0.  Don't care about offset value at this point - this gets written in 
		// TExifAccessor::CreateExifHeaderL()
		result = SetIntegerParam(KTag8825[ETagValue],EZeroth, 0);
		}
		
	if ((result != KErrNone) || (aGpsIfd->EntryCount() == 0))
		{
		// Either it was not possible to add a GPS Ifd Pointer to Ifd 0, or
		// The GPS Ifd has no tags.  In either case, it should be removed.
		TInt gpsIndex = 0;
		if (FindIfdIndex(EGpsSub, gpsIndex))
			{
			delete iIfds[gpsIndex];
			iIfds.Remove(gpsIndex);
			}
		if (result == KErrNone)
			{
			//Can only be here if aAddParamResult != KErrNone.
			result = aAddParamResult;
			}
		}
	return result;
	}
	
	
void CExifReaderWriter::RemoveThumbnailData()
	{	
	for(TInt i=0; i<iIfds.Count(); i++)
		{
		if((iIfds)[i]->Ifd()==EFirst)
			{
			delete iIfds[i];
			iIfds.Remove(i);
			return;
			}
		}
	}
	
	
CIfdGeneral* CExifReaderWriter::GetIfd(TInt aIfd)
	{
	TInt index;
	if (FindIfdIndex(aIfd, index))
		return iIfds[index];	
	else
		return NULL;
	}



// Will only set the thumbnail if it is JPEG encoded.
void CExifReaderWriter::SetThumbnailDataBaseL(CIfdGeneral* ifd1, TBool aKeepIfd1)
	{
	ASSERT(ifd1);
	
	delete iJpegThumbnailData;
	iJpegThumbnailData = NULL;
	
	// Check for tag 0x0103 (Compression). It must indicate JPEG.
	TUint16 jpegCompressionValue = 0;	
	TInt err = ifd1->GetShortParam(KThumbTag0103[ETagValue], jpegCompressionValue);
	if(err == KErrNone && jpegCompressionValue == KJPEGEncodedThumbnail)
		{
		// This is a JPEG thumbnail. Check for tag 0x0201 (offset to the JPEG thumbnail SOI).
 		TInt offset = 0;	
 
 		err = ifd1->GetIntegerParam(KThumbTag0201[ETagValue], offset);
		if(err == KErrNone)
 			{
 			// Check for tag 0x0202 (size of the JPEG thumbnail).
 			TInt thumbSize = 0;
 			
 			err = ifd1->GetIntegerParam(KThumbTag0202[ETagValue], thumbSize);
 			if (err == KErrNone)
 				{
				TInt sizeTIFFHeader = KByteAlignmentByteCount + KFortyTwoByteCount + KIfdOffsetByteCount; 
			                                                
				if (thumbSize != (iExifDataLength - offset))
					{
					// incorrect offset/thumbSize - try correcting
					if ((offset >= sizeTIFFHeader)	&& (offset < iExifDataLength))
						{
						// offset looks good so adjust the size
						thumbSize = iExifDataLength - offset;
						}
					else if ((thumbSize >= 0) && (thumbSize < (iExifDataLength - sizeTIFFHeader)))
						{
						// size looks good so adjust the offset
						offset = iExifDataLength - thumbSize;
						}
					else
						{
						// can't be corrected - ignore thumbnail
						thumbSize = 0;
						}				
					}
 				// If the size is valid, read thumbSize bytes at offset from iBase.
				if (thumbSize > 0)
 					{			
 					iJpegThumbnailData = HBufC8::NewL(thumbSize);
 					iJpegThumbnailData->Des().Copy(&iBase[offset], thumbSize);
					return;	
					}			
				} // Check for 0x0202
			} // Check for 0x0201
		} // JPEG thumbnail

	// Either:
	// -	The source IFD1 did not contain tag 0x0103 (Compression);
	// -	We have an uncompressed (bitmap) thumbnail (jpegCompressionValue == 1);
	// -	We have a thumbnail of an unknown/unsupported compression type. 
	// -	There were unrecoverable errors with tags 0x0201 (offset to thumbnail SOI) and 0x0202 (size of thumbnail).
	//
	// Bitmap thumbnails are supported by the Exif spec, but we do not currently have 
	// the means to support them fully in our software. Likewise we have no way of 
	// dealing with unknown compression types or images with badly corrupt 0x0201 and/or 0x0202.
	// The only thing to do is remove the corrupt copy of the source IFD1 from iIfds and
	// replace it with a default (which will be populated or removed as necessary later on).

	// Remove the old IFD1 that describes the source thumbnail image
	RemoveThumbnailData();

	
	if (aKeepIfd1)
		{
		// Add a new default IFD1 to describe the thumbnail image that we will generate.
		ifd1 = CIfd1::NewLC(0, NULL, iIntel);
		iIfds.AppendL(ifd1);
		CleanupStack::Pop(ifd1);		
		}
	}




// Copies the standard Exif file header stuff to the given buffer.
void CExifReaderWriter::WriteExifFileHeaderBaseL(const TUint aApp1Size, TUint8*& aPtr)
	{
	if(aApp1Size < KExifFileHeaderLength)
		{
		User::Leave(KErrNoMemory);
		}
	aPtr = Mem::Copy(aPtr, &KApp1Marker, KApp1MarkerByteCount);
	TUint8 appSizePtr[KApp1SizeLength];
	PtrWriteUtil::WriteBigEndianInt16(appSizePtr,aApp1Size);
	aPtr = Mem::Copy(aPtr, &appSizePtr, KApp1SizeLength);
	aPtr = Mem::Copy(aPtr, &KExifMarker, KExifMarkerByteCount);
	aPtr = Mem::Copy(aPtr, &KExifMarkerEnd, KExifMarkerEndByteCount);
	aPtr = Mem::Copy(aPtr, &KIntelByteAlignment, KByteAlignmentByteCount);
	aPtr = Mem::Copy(aPtr, &KFortyTwo, KFortyTwoByteCount);
	aPtr = Mem::Copy(aPtr, &KIfdOffset, KIfdOffsetByteCount);	
	}
	
void CExifReaderWriter::SetFirstIfdOffsetBase()
	{
	// Only set the first ifd offset if we have thumbnail data, or we will encode it
	if (iEncodeThumbnail || iJpegThumbnailData) 
		{
		// Locate the 1st Ifd offset - immediately after all tag data in the 0th Ifd (and before offset data).
		TUint offsetToFirstOffsetField = KExifFileHeaderLength;	
		CIfdGeneral* ifd = GetIfd(EZeroth);
		ASSERT(ifd);
		TUint16 size = ifd->EntryCount() * KMinimumIfdEntrySize;
		offsetToFirstOffsetField += size + KSizeOfEntryCount; // Need to account for the Ifd entry count.
		
		TInt offsetValue = KExifFileHeaderLength;
		// get the sizes of the ifds
		for(TInt i = 0; i < iIfds.Count(); i++)
			{
			CIfdGeneral* iFd = (iIfds)[i];
			TInt id = iFd->Ifd();
			if ((id == EZeroth)	||
				(id == EExifSub) ||
				(id == EInterop) ||
				(id == EGpsSub))
				{
				if (iFd->Size() != 0) 
					{
					offsetValue += KSizeOfEntryCount + iFd->Size() + KIfdOffsetByteCount;	
					}
				}
			}

		// Number of bytes from the start of the Exif header to the 1st Ifd.
		// Since all offsets are calculated from the byte alignment, deduct this.
		offsetValue -= KByteCountToByteAlignment;	
		Mem::Copy(iData + offsetToFirstOffsetField, &offsetValue, sizeof(offsetValue));
		}

	}
	
	
TInt CExifReaderWriter::GetThumbnailLengthOffsetBase(TUint& aThumbnailLengthOffset)
	{
	TUint offsetToLengthField = 0;
	TUint numFirstIfdEntries = 0;

	offsetToLengthField += KExifFileHeaderLength;
	
	for (TInt i = 0; i < iIfds.Count(); i++)
		{
		TUint16 size = (iIfds)[i]->Size();
		if ((size > 0) && ((iIfds)[i]->Ifd() != EFirst))
			{
			offsetToLengthField += KSizeOfEntryCount; // Need to account for the Ifd entry count.
			offsetToLengthField += KIfdOffsetByteCount; // Need to account for the offset to the 1st Ifd.
			offsetToLengthField += size;
			}
		else if ((iIfds)[i]->Ifd() == EFirst)
			{
			numFirstIfdEntries = (iIfds)[i]->EntryCount(); 	
			}
		}
		
	offsetToLengthField += KSizeOfEntryCount; // For first Ifd's entry count.
	
	TUint8* tempPtr = iData + offsetToLengthField; // point to first entry in 1st Ifd.
	
	// Now step every 12 bytes to find location of the tag.
	TUint16 jpegLengthTag = KThumbTag0202[ETagValue];
	for (TUint j = 0; j < numFirstIfdEntries; j++)
		{
		TUint16 tag = PtrReadUtil::ReadUint16(tempPtr);
		if (tag == jpegLengthTag)
			{						
			aThumbnailLengthOffset = offsetToLengthField+KValueOffsetFieldPosition;
			return KErrNone;
			}
		offsetToLengthField += KMinimumIfdEntrySize;
		tempPtr += KMinimumIfdEntrySize;
		}	
	return KErrNotFound;
	}
	
HBufC8* CExifReaderWriter::GetJpegThumbnailData()
	{
	// don't delete previous parameter, this makes no sense in this case
	return iJpegThumbnailData;
	}
	
void CExifReaderWriter::SetThumbnailData(HBufC8* aJpegData)
	{
	delete iJpegThumbnailData;
	iJpegThumbnailData= aJpegData;
	if(iJpegThumbnailData != NULL) //Ensure new thumbnail data is added
		{
		iExifDataModified = ETrue;
		}
	}

TBool CExifReaderWriter::IsExifDataModified() 
	{
	return iExifDataModified;
	}
	
void CExifReaderWriter::ResetExifDataModified() 
	{
	iExifDataModified = EFalse;
	}
