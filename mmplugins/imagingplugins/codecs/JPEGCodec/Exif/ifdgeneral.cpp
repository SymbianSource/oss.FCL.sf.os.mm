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

#include "ifdgeneral.h"
#include "ifdgeneralentry.h"
#include "ExifTagDescriptions.h"
#include "exifreaderwriter.h"
#include "ExifTagHelper.h"
#include <bafl/sysutil.h>

// Number of consecutive invalid IFD entries that may appear before the whole IFD is considered corrupt
// This value is set in the ExifLibStatic.mmp file, the default is 7.
const TUint KMaxNumberOfConsecInvalidEntries = SYMBIAN_ICL_EXIF_CONSEC_INVALID_TAGS;

CIfdGeneral::CIfdGeneral(const TInt aIfdNumber, const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	: iIfdNumber(aIfdNumber)
	, iIntel(aIntel)
	, iOffsetToIfd(aOffsetToIfd)
	, iBase(aBase)
	, iExifDataLength(aExifDataLength)
	{	
	}

CIfdGeneral::~CIfdGeneral()
	{
	iIfdEntries.ResetAndDestroy();
	}
	
void CIfdGeneral::ConstructL()
	{
	if(iBase)
		{
		// read the entries from the data block
		AddAllIfdEntriesL();
		}
	else
		{
		// create default entries
		SetupDirectoryEntriesL();
		}
	}

// Alters a directory in a given IFD - used when the directory is known to exist 
// (i.e. for mandatory entries) which are set/created upon initialisation.
TInt CIfdGeneral::SetParam8(const TUint aTag, HBufC8* aParam)
	{	
	ASSERT(aParam!=NULL);
		
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.
			// May be undefined type or ascii type.
			if(iIfdEntries[i]->IsStringFormat())
				{
				//pass in ifd number as the same ascii tag may be in more than one ifd
				return iIfdEntries[i]->UpdateString(aParam);				
				}
			else if(iIfdEntries[i]->IsUndefinedFormat())
				{
				return iIfdEntries[i]->UpdateData(aParam->Length(), aParam->Des().Ptr());	
				}
			else if (iIfdEntries[i]->Format()==EByte)
				{
				//we need to support byte entries too as they are Param8
				return iIfdEntries[i]->UpdateData(aParam->Length(), aParam->Des().Ptr());
				}			
			}
		}
	return KErrNotSupported;
	}
	
TInt CIfdGeneral::SetParam16(const TUint aTag, HBufC16* aParam)
	{	
	ASSERT(aParam!=NULL);
			
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.			
			HBufC8* extendedData=NULL;
			TRAPD(err, extendedData=TExifTagHelper::CreateUnicodePrefixedBufferL(*aParam));
			if(err!=KErrNone)
				{
				return KErrNoMemory;
				} 
			TInt componentCount=extendedData->Length();
			TInt updateErr = iIfdEntries[i]->UpdateData(componentCount, extendedData->Des().Ptr()); 
			delete extendedData;		
			return updateErr;
			}
		}
	return KErrNotSupported;
	}
	
TInt CIfdGeneral::SetIntegerParam(const TUint aTag, const TInt aParam)
	{
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.
			return iIfdEntries[i]->UpdateInteger(aParam);
			}
		}
	return KErrNotSupported;
	}
	
TInt CIfdGeneral::SetShortParam(const TUint aTag, const TUint16 aParam)
	{
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.
			return iIfdEntries[i]->UpdateShort(aParam);
			}
		}
	return KErrNotSupported;
	}

TInt CIfdGeneral::SetRationalParam(const TUint aTag, const TInt aNumerator, const TInt aDenominator)
	{
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.
			return iIfdEntries[i]->UpdateRational(aNumerator, aDenominator);
			}
		}
	return KErrNotSupported;
	}


/* 
Add a directory in a given IFD - used for the optional directories, since these are not
set/created upon initialisation.  Must check for existence of the tag along with validity
of adding the tag to this particular IFD.
*/
void CIfdGeneral::AddParam8L(const TUint aTag, const TUint aFormat, const TUint aComponentCount, HBufC8* aParam)
	{
	ASSERT(aParam!=NULL);
		
	TInt err = KErrNotSupported;
	
	// Check that aTag belongs in this IFD.
	TInt index = GetTagIndex(aTag);
	if(index != KErrNotFound)
		{
		//The length is checked, but really it should be done in the IfdEntry
		TUint paramLength=aParam->Length();
		const TUint* tag = GetTagInformation(index);
		TUint requiredComponentCount=tag[EComponentCountValue];
		TBool legal=EFalse;
		if((requiredComponentCount==0) || (aFormat!=EAscii))
			{
			//Either this tag can be of any length (so no further length checks needed)
			//or it is not ASCII, which means it simply has to be at least as long
			//as the required length
			legal=(paramLength >= requiredComponentCount * KDataFormatSize[aFormat]);	
			}
		else
			{
			//We have a fixed length ASCII string.  For such strings we must
			//consider NULL termination.  Considering that we add the NULL if the 
			//user forgets to add it for other tags, we should do it here as well 
			//to be consistent.
			if (paramLength==requiredComponentCount)
				{
				//The string is already of the max length so it is only legal if it is NULL
				//terminated
				legal=((*aParam)[paramLength-1] == KNullCharacter);	
				}
			else if (paramLength==requiredComponentCount-1)
				{
				//The string is one less than the required length.  If it is NULL terminated already,
				//then the string is too short (illegal).  If it is not NULL terminated, we let it
				//pass anyway and CEncoderIfdEntry software will add the NULL.  
				legal=((*aParam)[paramLength-1] != KNullCharacter);						
				}
			else
				{
				//Strings that pass the first two checks are only legal if exactly the
				//required length.
				legal=(paramLength == requiredComponentCount);	
				}
			}
		if (legal)				
			{
			// Entry is legal, so add entry.
			const TUint8* valueOffset = aParam->Des().Ptr();			
			CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aComponentCount, valueOffset, this);
			iIfdEntries.AppendL(entry);
			CleanupStack::Pop(entry);
			err = KErrNone;
			}
		else
			{
			err = KErrNotSupported;
			}
		}
	User::LeaveIfError(err);
	}
	
void CIfdGeneral::AddParam16L(const TUint aTag, const TUint aFormat, const TUint /*aComponentCount*/, HBufC16* aParam)
	{
	ASSERT(aParam!=NULL);
		
	TInt err = KErrNotSupported;
	
	// Check that aTag belongs in this IFD.
	TInt index = GetTagIndex(aTag);
	if(index != KErrNotFound)
		{	
		TUint paramLength=aParam->Length();
		const TUint* tag = GetTagInformation(index);
		if( (paramLength >= tag[EComponentCountValue] * KDataFormatSize[aFormat]))
			{				
			
			// Entry is legal, so add entry.
			// Must add UNICODE\0 to start of data.				
			TInt size = aParam->Length()*2 + KUnicode().Length();
			TUint8* data = static_cast<TUint8*>(User::AllocL(size));								
			CleanupStack::PushL(data);
			
			Mem::Copy(data, KUnicode().Ptr(), KUnicode().Length());
			Mem::Copy(data+KUnicode().Length(), aParam->Des().Ptr(), aParam->Length()*2);												
			
			CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, size, data, this);
			iIfdEntries.AppendL(entry);
			CleanupStack::Pop(entry);
			CleanupStack::PopAndDestroy(data);
			err = KErrNone;		
			}
		} 
	User::LeaveIfError(err);
	}

	
void CIfdGeneral::AddIntegerParamL(const TUint aTag, const TUint aFormat, const TUint aComponentCount, TInt aParam)
	{
	TInt err = KErrNotSupported;
	
	// Check that aTag belongs in this IFD.
	if(FindTag(aTag))
		{
		// Entry is legal, so add entry.
		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aComponentCount, reinterpret_cast<const TUint8*>(&aParam), this);
		iIfdEntries.AppendL(entry);
		CleanupStack::Pop(entry);
		err = KErrNone;		
		} 
	User::LeaveIfError(err);
	}
	
void CIfdGeneral::AddShortParamL(const TUint aTag, const TUint aFormat, const TUint aComponentCount, TUint16 aParam)
	{
	TInt err = KErrNotSupported;
	
	// Check that aTag belongs in this IFD.
	if(FindTag(aTag))
		{
		// Entry is legal, so add entry.
		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aComponentCount, reinterpret_cast<const TUint8*>(&aParam),this);
		iIfdEntries.AppendL(entry);
		CleanupStack::Pop(entry);
		err = KErrNone;		
		} 
	User::LeaveIfError(err);
	}
	
void CIfdGeneral::AddRationalParamL(const TUint aTag, const TUint aFormat, const TUint aComponentCount, const TInt aNumerator, const TInt aDenominator)
	{
	TInt err = KErrNotSupported;
	
	// Check that aTag belongs in this IFD.
	if(FindTag(aTag))
		{
		TUint8* valueOffset = static_cast<TUint8*>(User::AllocL(KSizeOfRational));
		CleanupStack::PushL(valueOffset);
		
		// Entry is legal, so add entry.
		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aComponentCount, valueOffset, this);
		User::LeaveIfError(entry->UpdateRational(aNumerator, aDenominator));
		iIfdEntries.AppendL(entry);
		CleanupStack::Pop(entry);
		CleanupStack::PopAndDestroy(valueOffset);
	
		err = KErrNone;			
		} 
	if(err != KErrNone)
		{
		User::LeaveIfError(err);			
		}
	return;
	}



void CIfdGeneral::SetIntegerArrayParamL(const TUint aTag, const CArrayFix<TInt>& aParam)
	{
	TInt err = KErrNotSupported;
	
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.
			HBufC8* ptr = HBufC8::NewL(aParam.Count()*K32BitIntegerByteCount/K8BitIntegerByteCount);
			CleanupStack::PushL(ptr);
			const TUint8* bytePtr = ptr->Des().Ptr();
			TInt* dataPtr = const_cast<TInt*>(reinterpret_cast<const TInt*>(bytePtr));								
			for(TInt j = 0; j < aParam.Count(); j++)
				{
				TInt value = aParam.At(j);
				dataPtr[j] = value;
				}
			User::LeaveIfError(iIfdEntries[i]->UpdateData(aParam.Count(), bytePtr));
			CleanupStack::PopAndDestroy(ptr);
			err = KErrNone;
			}
		}
	User::LeaveIfError(err);
	}

void CIfdGeneral::AddIntegerArrayParamL(const TUint aTag, const TUint aFormat, const CArrayFix<TInt>& aParam)
	{
	TInt err = KErrNotSupported;
	
	// Check that aTag belongs in this IFD.
	if(FindTag(aTag))
		{
		// Entry is legal, so add entry.
		HBufC8* ptr = HBufC8::NewL(aParam.Count()*K32BitIntegerByteCount/K8BitIntegerByteCount);
		CleanupStack::PushL(ptr);
		const TUint8* bytePtr = ptr->Des().Ptr();
		TInt* dataPtr = const_cast<TInt*>(reinterpret_cast<const TInt*>(bytePtr));								
		for(TInt i = 0; i < aParam.Count(); i++)
			{
			TInt value = aParam.At(i);
			dataPtr[i] = value;
			}

		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aParam.Count(), bytePtr, this);								
		iIfdEntries.AppendL(entry);
		CleanupStack::Pop(entry);
		CleanupStack::PopAndDestroy(ptr);
		err = KErrNone;					
		}
	User::LeaveIfError(err);
	}

void CIfdGeneral::AddRationalArrayParamL(const TUint aTag, const TUint aFormat, const CArrayFix<TRational>& aParam)
	{
	TInt err = KErrNotSupported;
	// Check that aTag belongs in this IFD.
	if(FindTag(aTag))
		{
		// Entry is legal, so add entry.
		HBufC8* ptr = HBufC8::NewL((aParam.Count()*2) * K32BitIntegerByteCount);
		CleanupStack::PushL(ptr);
		const TUint8* bytePtr = ptr->Des().Ptr();
		TInt* dataPtr = const_cast<TInt*>(reinterpret_cast<const TInt*>(bytePtr));
		TInt ptrInc = 0;
		for(TInt i = 0; i < aParam.Count(); i++)
			{
			TInt numer = (aParam.At(i)).iNumerator;
			TInt denom = (aParam.At(i)).iDenominator;
			dataPtr[ptrInc++] = numer;
			dataPtr[ptrInc++] = denom;
			}

		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aParam.Count(), bytePtr, this);								
		iIfdEntries.AppendL(entry);
		CleanupStack::Pop(entry);
		CleanupStack::PopAndDestroy(ptr);
		err = KErrNone;						
		} 
	User::LeaveIfError(err);
	}
	
void CIfdGeneral::SetRationalArrayParamL(const TUint aTag, const CArrayFix<TRational>& aParam)
	{
	TInt err = KErrNotSupported;
	// find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			// Found the tag.
			HBufC8* ptr = HBufC8::NewL((aParam.Count()*2) * K32BitIntegerByteCount);
			CleanupStack::PushL(ptr);
			const TUint8* bytePtr = ptr->Des().Ptr();
			TInt* dataPtr = const_cast<TInt*>(reinterpret_cast<const TInt*>(bytePtr));
			TInt ptrInc = 0;
			for(TInt j = 0; j < aParam.Count(); j++)
				{
				TInt numer = (aParam.At(j)).iNumerator;
				TInt denom = (aParam.At(j)).iDenominator;
				dataPtr[ptrInc++] = numer;
				dataPtr[ptrInc++] = denom;
				}
			User::LeaveIfError(iIfdEntries[i]->UpdateData(aParam.Count(), bytePtr));
			CleanupStack::PopAndDestroy(ptr);
			err = KErrNone;
			}
		}
	User::LeaveIfError(err);
	}
	
	
void CIfdGeneral::AddShortArrayParamL(const TUint aTag, const TUint aFormat, const CArrayFix<TUint16>& aParam)
	{
	TInt err = KErrNotSupported;
	// Check that aTag belongs in this IFD.
	if(FindTag(aTag))
		{
		// Entry is legal, so add entry.
		HBufC8* ptr = HBufC8::NewL(aParam.Count()*K16BitIntegerByteCount/K8BitIntegerByteCount);
		CleanupStack::PushL(ptr);
		const TUint8* bytePtr = ptr->Des().Ptr();
		TInt16* dataPtr = const_cast<TInt16*>(reinterpret_cast<const TInt16*>(bytePtr));								
		for(TInt i = 0; i < aParam.Count(); i++)
			{
			TInt value = aParam.At(i);
			dataPtr[i] = value;
			}

		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(aTag, aFormat, aParam.Count(), bytePtr, this);								
		iIfdEntries.AppendL(entry);
		CleanupStack::Pop(entry);
		CleanupStack::PopAndDestroy(ptr);
		err = KErrNone;							
		} 
	User::LeaveIfError(err);
	}

void CIfdGeneral::SetShortArrayParamL(const TUint aTag, const CArrayFix<TUint16>& aParam)
	{
	TInt err = KErrNotSupported;
	// Find the entry with the required tag.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			HBufC8* ptr = HBufC8::NewL(aParam.Count()*K16BitIntegerByteCount/K8BitIntegerByteCount);
			CleanupStack::PushL(ptr);
			const TUint8* bytePtr = ptr->Des().Ptr();
			TInt16* dataPtr = const_cast<TInt16*>(reinterpret_cast<const TInt16*>(bytePtr));								
			for(TInt j = 0; j < aParam.Count(); j++)
				{
				TInt value = aParam.At(j);
				dataPtr[j] = value;
				}
			User::LeaveIfError(iIfdEntries[i]->UpdateData(aParam.Count(), bytePtr));
			CleanupStack::PopAndDestroy(ptr);
			err = KErrNone;
			}		
		}
	User::LeaveIfError(err);
	}	

TInt CIfdGeneral::GetIntegerParam(const TUint aTag, TInt& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		return KErrNotFound;
		}
	
	// Check that we are dealing with an Integer format.
	switch(iIfdEntries[entry]->Format())
		{		
		case (EUnsignedLong):
		case (ESignedLong):
			break;
		default:
			return KErrArgument;
		}

	// Check the size.
	TUint totalSize = iIfdEntries[entry]->ComponentCount() * KDataFormatSize[iIfdEntries[entry]->Format()];
	if(totalSize > KSizeOfValueOffsetField)
		{
		// Data is offset, so we are dealing with more than one integer, so this function cannot deal with it.
		return KErrArgument;
		}
	
	aParam = iIfdEntries[entry]->ValueOffset();
	return KErrNone;
	}
	
	


TInt CIfdGeneral::GetShortParam(const TUint aTag, TUint16& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		return KErrNotFound;
		}
	
	// Check that we are dealing with an short format.
	if(iIfdEntries[entry]->Format() != EUnsignedShort)
		{
		return KErrArgument;
		}

	// Check the size.
	TUint totalSize = iIfdEntries[entry]->ComponentCount() * KDataFormatSize[iIfdEntries[entry]->Format()];

	 // Since 2 shorts could be in 4 byte field, we only return one!
	if(totalSize > KSizeOfValueOffsetField / 2)
		{
		// Data is offset, so we are dealing with more than one integer, so this function cannot deal with it.
		return KErrArgument;
		}
	
	aParam = iIfdEntries[entry]->ValueOffset();

	return KErrNone;
	}

TInt CIfdGeneral::GetRationalParam(const TUint aTag, TInt& aNumer, TInt& aDenom) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		return KErrNotFound;
		}
	
	// Check that we are dealing with an rational format.
	switch(iIfdEntries[entry]->Format())
		{
		case (EUnsignedRational):
		case (ESignedRational):
			break;
		default:
			return KErrArgument;
		}

	// Check the size.
	TUint totalSize = iIfdEntries[entry]->ComponentCount() * KDataFormatSize[iIfdEntries[entry]->Format()];

	// A single rational datum is 8 bytes, so we only deal with single values here.
	if(totalSize > KSizeOfValueOffsetField * 2)
		{
		return KErrArgument;
		}
	
	const TUint8* rationalData = iIfdEntries[entry]->ActualValue();
	if(!rationalData)
		{
		return KErrArgument;
		}
	Mem::Copy(&aNumer, rationalData, sizeof(aNumer));
	Mem::Copy(&aDenom, rationalData+sizeof(aNumer), sizeof(aDenom));
	return KErrNone;
	}

TInt CIfdGeneral::GetParam8(const TUint aTag, HBufC8*& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		return KErrNotFound;
		}
	
	// Check that we are dealing with correct format.
	TUint format = iIfdEntries[entry]->Format();
	switch(format)
		{
		case (EByte):
		case (EAscii):
		case (EUndefined):
			break;
		default:
			return KErrArgument;
		}

	// Check the size.
	TUint totalSize = iIfdEntries[entry]->ComponentCount();
	
	delete aParam;
	aParam = HBufC8::New(totalSize);				
	if(aParam == NULL)
		{
		return KErrNoMemory;
		}
		
	_LIT8(KStringFormat, "%s");
	if(totalSize > KSizeOfValueOffsetField)
		{
		const TUint8* theData=iIfdEntries[entry]->ActualValue();
		if(!theData)
			{
			return KErrArgument;
			}

		if (aTag == KTag8298[ETagValue])
			{
			//the copyright tag is a special case as it can have null mid string
			if (*(theData+totalSize-1) == NULL)
				{
				//we remove null at the end for consistency
				totalSize--;
				}
			aParam->Des().Copy(theData, totalSize);	
			}
		else if (format == EAscii)
			{
			aParam->Des().Format(KStringFormat, theData);
			}
		else
			{
			aParam->Des().Copy(theData, totalSize);	
			}
		}
	else if (totalSize>0)
		{
		// Get data from value/offset field.
		TInt value = iIfdEntries[entry]->ValueOffset();

		if (aTag == KTag8298[ETagValue])
			{
			//the copyright tag is a special case as it can have null mid string
			//note we would only get here if the copyright was <= 4 bytes
			if (*(reinterpret_cast<TUint8*>(&value)+totalSize-1) == NULL)
				{
				//we remove null at the end for consistency
				totalSize--;
				}
			aParam->Des().Copy(reinterpret_cast<TUint8*> (&value), totalSize);	
			}
		else if (format == EAscii)
			{
			aParam->Des().Format(KStringFormat, reinterpret_cast<TUint8*> (&value));
			}
		else
			{
			aParam->Des().Copy(reinterpret_cast<TUint8*> (&value), totalSize);
			}
		}
	return KErrNone;
	}

TInt CIfdGeneral::GetParam16(const TUint aTag, HBufC16*& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		return KErrNotFound;
		}
	
	// Check that we are dealing with the undefined format.
	switch(iIfdEntries[entry]->Format())
		{
		case (EUndefined):
			break;
		default:
			return KErrArgument;
		}

	// Check the size.
	const TInt totalByteSize = iIfdEntries[entry]->ComponentCount();
	
	delete aParam;
	aParam = HBufC16::NewMax((totalByteSize+1)>>1);
	if(aParam == NULL)
		{
		return KErrNoMemory;
		}

	TPtr ptr(aParam->Des());
	Mem::Copy(reinterpret_cast<TText8*>(&ptr[0]), iIfdEntries[entry]->ActualValue(), totalByteSize);

	return KErrNone;
	}

void CIfdGeneral::GetIntegerArrayParamL(const TUint aTag, CArrayFix<TInt>& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		User::Leave(KErrNotFound);
		}
	
	// Check that we are dealing with an Integer format.
	switch(iIfdEntries[entry]->Format())
		{
		case (EUnsignedLong):
		case (ESignedLong):
			break;
		default:
			User::Leave(KErrArgument);
		}

	// Check the size.
	TUint totalSize = iIfdEntries[entry]->ComponentCount() * KDataFormatSize[iIfdEntries[entry]->Format()];
	if(totalSize > KSizeOfValueOffsetField)
		{
		// Obviously have multiple entries.
		// However, as of Exif 2.2 there are no Integer Array tags defined that have
		// more than one entry.
		TInt numberOfInts = totalSize / KDataFormatSize[iIfdEntries[entry]->Format()];
		aParam.ResizeL(numberOfInts);
		const TUint8* dataStream = iIfdEntries[entry]->ActualValue();
		if(!dataStream)
			{
			User::Leave(KErrArgument);
			}

		TInt value = 0;
		for(TInt i = 0; i < numberOfInts; i++)
			{
			Mem::Copy(&value, dataStream, sizeof(value));
			aParam[i] = value;
			dataStream += sizeof(value);
			}
		}
	else
		{
		// A single entry
		aParam.ResizeL(1);
		aParam[0] = iIfdEntries[entry]->ValueOffset();
		}
	}

void CIfdGeneral::GetShortArrayParamL(const TUint aTag, CArrayFix<TUint16>& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		User::Leave(KErrNotFound);
		}

	// Check that we are dealing with an Short format.
	TUint format = iIfdEntries[entry]->Format();
	if(format != EUnsignedShort)
		{
		User::Leave(KErrArgument);
		}
		
	// Check the size.
	TUint componentCount = iIfdEntries[entry]->ComponentCount();
	TUint dataSize = KDataFormatSize[format];
	TUint totalSize = componentCount * dataSize;	
	if(totalSize > KSizeOfValueOffsetField)
		{
		// Obviously have multiple entries.
		aParam.ResizeL(componentCount);
		const TUint8* dataStream = iIfdEntries[entry]->ActualValue();
		if(dataStream==NULL)
			{
			User::Leave(KErrArgument);
			}

		TUint16 value = 0;
		for(TInt i = 0; i < componentCount; i++)
			{
			Mem::Copy(&value, dataStream, sizeof(value));
			aParam[i] = value;
			dataStream += sizeof(value);
			}
		}
	else
		{
		// Either one or two entries
		ASSERT((componentCount==1) || (componentCount==2));
		aParam.ResizeL(componentCount);
		TUint value = iIfdEntries[entry]->ValueOffset();
		
		// When component count is 1, we read a UShort as a UShort,
		// so the byte order is correct on return.
		//
		// When component count is 2, we read the UShorts as a UInt.
		// i.e. Data is stored on DISK as TWO Shorts but read as ONE TUint32.
		//
		// Example representation of data on Disk:
		//    0x0300 0x0500 (Intel II LittleEndian) - 0x0003 & 0x0005 (Motorola MM BigEndian)
		//
		// For Intel data, it is read in as an Intel UInt and the bytes reversed. 
		// This is important as a pointer is usually stored here:
		//    0x0300 0500  becomes  0x0005 0003
		//

		// For Big-endian formatted files
		TUint32 highOrderShortMask = 0xFFFF0000;
		TUint32 lowOrderShortMask  = 0x0000FFFF;
		
		if (componentCount==1)
			{
			aParam[0] = (value & lowOrderShortMask);
			}
		else
			{
			if(iIntel)	
				{ // II format
				aParam[0] = (value & lowOrderShortMask);				
				aParam[1] = (value & highOrderShortMask) >> 16;									
				}				
			else
				{ // MM format
				aParam[0] = (value & highOrderShortMask) >> 16;									
				aParam[1] = (value & lowOrderShortMask);				
				}
			}
		}
   	}

void CIfdGeneral::GetRationalArrayParamL(const TUint aTag, CArrayFix<TRational>& aParam) const
	{
	TInt entry = LocateEntry(aTag);
	if(entry == KErrNotFound)
		{
		User::Leave(KErrNotFound);
		}
	
	// Check that we are dealing with an Rational format.
	switch(iIfdEntries[entry]->Format())
		{
		case (EUnsignedRational):
		case (ESignedRational):
			break;
		default:
			User::Leave(KErrArgument);
		}

	// Check the size.
	TInt totalSize = iIfdEntries[entry]->ComponentCount() * KDataFormatSize[iIfdEntries[entry]->Format()];
	// Obviously have multiple entries.
	TInt numberOfRationalValues = totalSize / KDataFormatSize[iIfdEntries[entry]->Format()];
	aParam.ResizeL(numberOfRationalValues);
	const TUint8* dataStream = iIfdEntries[entry]->ActualValue();
	if(!dataStream)
		{
		User::Leave(KErrArgument);
		}

	TRational rational;
	for(TInt i = 0; i < numberOfRationalValues; i++)
		{
		Mem::Copy(&rational.iNumerator, dataStream, sizeof(rational.iNumerator));
		Mem::Copy(&rational.iDenominator, dataStream+sizeof(rational.iNumerator), sizeof(rational.iDenominator));
		aParam[i] = rational;
		dataStream += sizeof(rational.iNumerator) + sizeof(rational.iDenominator); // Move pointer on 8 bytes.
		}
	}

TInt CIfdGeneral::Size() const
	{
	TUint totalSize = 0;
	// Get the size of each directory entry.
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		TInt size = iIfdEntries[i]->TotalSize();
		totalSize+=size;				
		}
	return totalSize;
	}
	
	

// Can only be called by the 0th Ifd (otherwise returns KErrNotSupported).
TInt CIfdGeneral::GetOffsetToFirstIfd(TUint& aOffset) const
	{
	if(iIfdNumber == 0)
		{
		aOffset = CExifReaderWriter::ReadUint32(iIntel, iBase +
													(iIfdEntryCount * KMinimumIfdEntrySize)+
													sizeof(TUint16)+
													iOffsetToIfd);		
		return KErrNone;
		}
	return KErrNotSupported;
	}

/**
	Parses the buffer, extracting the data.
	Ignores corrupt IFD entries (e.g. one with a value offset that is beyond the buffer's boundaries)
	unless more than KMaxNumberOfConsecInvalidEntries invalid entries appear in succession. In that case
	the whole IFD is considered corrupt.
	
	@leave	KErrNoMemory	If there is insufficient memory to process the data
	@leave	KErrCorrupt		If there are more than KMaxNumberOfConsecInvalidEntries consecutive
							corrupt entries
*/
void CIfdGeneral::AddAllIfdEntriesL()
	{
	// Set the Ifd entry count.
	iIfdEntryCount = CExifReaderWriter::ReadUint16(iIntel, iBase + iOffsetToIfd);
	
	const TUint8* ifdPtr = iBase + iOffsetToIfd + sizeof(iIfdEntryCount);
	
	//check for the validity of the iIfdEntryCount
	if(!IsValidIfdEntryCount(iIfdEntryCount))
		{//most likely an invalid exif header
		User::Leave(KErrCorrupt);		
		}

	// records if an entry is corrupt
	TBool ifdEntryIsCorrupt = EFalse;
	
	// counter to track the number of consec corrupt ifd entries
	// if more than KMaxNumberOfConsecInvalidEntries corrupt ifd entries occurr in a
	// row then the entire IFD is deemed to be corrupt and all the iIfdEntries are to
	// be deleted
	TUint numberOfConsecutiveCorruptIfdEntries = 0;

	// initialise the offset, which acts as a pointer increment.
	TInt offset = 0;
	for(TInt ifdEntry = 0; ifdEntry < iIfdEntryCount; ifdEntry++)
		{
		ifdEntryIsCorrupt = EFalse;
		
		// Create the new CIfdEntry to populate.

		// Set the general data of the Ifd entry.
		TUint16 tag = CExifReaderWriter::ReadUint16(iIntel, ifdPtr + offset+KTagFieldPosition);
		TUint16 format = CExifReaderWriter::ReadUint16(iIntel, ifdPtr + offset+KFormatFieldPosition);
		TUint componentCount = CExifReaderWriter::ReadUint32(iIntel, ifdPtr + offset+KComponentCountFieldPosition);

		// Determine if the data is held in the value/offset or if it is pointed to.
		TUint totalSize = 0;
		
		TUint valueOffset;
		TBool endianess = iIntel;
		const TUint8* valuePtr = NULL;
			
		if (!(format < KDataFormatSizeLength) ||
			((totalSize = componentCount * KDataFormatSize[format]) > iExifDataLength) ||
			(totalSize == 0) ||
			(tag == NULL && !AllowNullTags())) //null tag is valid for GPS IFD
			{
			// if invalid format or size is unreasonable this IFD entry is corrupt
			// (second check is a bit crude but worthwhile since it
			//  can catch some obviously invalid entries early on)
			ifdEntryIsCorrupt = ETrue;
			}
		else
			{
			if (((format == EAscii)||(format == EByte)||(format == EUndefined)) && (totalSize <= KSizeOfValueOffsetField))
				{
				//If the above condition is true then the data is such that it needs to 
				//keep the byte order as is ie an ASCII field, a byte field or an undefined field
				//if this is the case we do not want to invert the bytes for big endian/motorola
				//ie order is unchanged
				//if the data is greater than 4 bytes then it should be copied across as is
				endianess = ETrue; //so set endianess to Intel even if the file is motorola
				}
			if(totalSize <= sizeof(TUint16))
				{
				valueOffset = CExifReaderWriter::ReadUint16(endianess, ifdPtr + offset+KValueOffsetFieldPosition);
				}
			else
				{
				valueOffset = CExifReaderWriter::ReadUint32(endianess, ifdPtr + offset+KValueOffsetFieldPosition);
				}
					
			if (totalSize>KSizeOfValueOffsetField)
				{
				if((valueOffset) && ((valueOffset + totalSize) <= iExifDataLength))
					{
					valuePtr = iBase+valueOffset;
					}
				else
					{
					// the offset is either null, pointing beyond the length of the exif data or
					// pointing to a value which goes beyond the length of the exif data
					// ie we are dealing with a corrupt entry
					ifdEntryIsCorrupt = ETrue;
					}
				}
			else
				{
				valuePtr = reinterpret_cast<const TUint8*> (&valueOffset);
				}
			}
			
		// try to make the entry
		CIfdGeneralEntry* entry = NULL;
		if(!ifdEntryIsCorrupt)
			{
			// entries with unknown tags are created but count towards corrupt entries
			// so that if many appear in succession we assume the IFD is corrupt
			entry = CreateIfdEntryLC(tag, format, componentCount, valuePtr, valueOffset, totalSize, ifdEntryIsCorrupt);
			
			if(!entry)
				{
				ifdEntryIsCorrupt = ETrue;
				}
			}		
			
		//an entry of all 0's could be padding so don't count it as corrupt
		if ((ifdEntryIsCorrupt) && !((tag == 0)&&(format == 0)&&(componentCount == 0)))
			{
			numberOfConsecutiveCorruptIfdEntries++;
			if ((numberOfConsecutiveCorruptIfdEntries == iIfdEntryCount)||(numberOfConsecutiveCorruptIfdEntries > KMaxNumberOfConsecInvalidEntries))
				{
				//if the above condition is true then it is likely that we are scanning the IFD from the offset
				//or that the data is non IFD
				//so we will skip further processing of the IFD and abandon
				User::Leave(KErrCorrupt);
				}
			}
		else
			{
			numberOfConsecutiveCorruptIfdEntries = 0; //reset
			}
			
		if (entry)
			{//always ensure entry is valid before appending - might not be if we have a null tag
			iIfdEntries.AppendL(entry);
			CleanupStack::Pop(entry);
			}

		offset += KMinimumIfdEntrySize;
		}
	
	// All entries have now been added.	
	}
	
/**
	Creates an IFD entry and pushes it onto the cleanup stack.
	If the entry cannot be created (most likely due to some
	corrupt data) NULL will be returned.
	
	@param	aTag			EXIF tag number for this entry
	@param	aFormat			Format of this entry's data
	@param	aComponentCount	Number of values in the data
	@param	aValuePtr		Pointer to the start of the data
	@param	aValueOffset	Offset from start of EXIF data to start of entry data
	@param	aTotalSize		Length of the data
	@param	aUnknownTag		Set to ETrue if the entry's tag is unknown (for this IFD),
							EFalse otherwise.
	
	@return	The newly created entry or NULL if it could not be created.
	
	@leave	KErrNoMemory	If there wasn't sufficient memory to create entry. This
							is the only type of leave this function can make.

*/
CIfdGeneralEntry* CIfdGeneral::CreateIfdEntryLC(const TUint& aTag, const TUint& aFormat, const TUint& aComponentCount, const TUint8* aValuePtr, TUint aValueOffset, TUint aTotalSize, TBool& aUnknownTag)
	{
	CIfdGeneralEntry* entry= NULL;
	
	TRAPD(err, entry = CIfdGeneralEntry::NewL(aTag, aFormat, aComponentCount, aValuePtr, this, aUnknownTag));
	if (err != KErrNone)
		{
		if (err == KErrNoMemory)
			{
			User::Leave(err); //only leave if no memory
			}
		}
	else
		{
		CleanupStack::PushL( entry );
		// no errors so far, so read the entry's values
		if(aTotalSize > KSizeOfValueOffsetField)
			{
			// Byte swap each component, if necessary.
			TUint8* byteSwappedDataBuffer = static_cast<TUint8*>(User::AllocL(aTotalSize));
			TUint8* tempPtr = byteSwappedDataBuffer;
			TInt updateErr=KErrNone;
			TInt i;
			switch(aFormat)
				{
				case(EByte):
				case(EAscii):
				case(EUndefined):
					updateErr = entry->SetActualData(iBase+aValueOffset, aComponentCount, KDataFormatSize[aFormat]);						
					break;
				case(EUnsignedRational):
				case(ESignedRational):
					// Comprised of 2 * 4-byte integers.
					{
					for(i = 0; i < aComponentCount; i++)
						{
						TUint numer = CExifReaderWriter::ReadUint32(iIntel, iBase + aValueOffset + 8*i);
						TUint denom = CExifReaderWriter::ReadUint32(iIntel, iBase + aValueOffset + 8*i + 4);
						tempPtr = Mem::Copy(tempPtr, &numer, sizeof(numer));
						tempPtr = Mem::Copy(tempPtr, &denom, sizeof(denom));
						}
					updateErr = entry->SetActualData(byteSwappedDataBuffer, aComponentCount, KDataFormatSize[aFormat]);						
					}
					break;
				case(EUnsignedLong):
				case(ESignedLong):
					{
					for(i = 0; i < aComponentCount; i++)
						{
						TUint value = CExifReaderWriter::ReadUint32(iIntel, iBase + aValueOffset+(i*sizeof(value)));
						tempPtr = Mem::Copy(tempPtr, &value, sizeof(value));
						}
					updateErr = entry->SetActualData(byteSwappedDataBuffer, aComponentCount, KDataFormatSize[aFormat]);						
					}
					break;
				case(EUnsignedShort):
					{
					for(i = 0; i < aComponentCount; i++)
						{
						TUint16 value = CExifReaderWriter::ReadUint16(iIntel, iBase + aValueOffset+(i*sizeof(value)));
						tempPtr = Mem::Copy(tempPtr, &value, sizeof(value));
						}
					updateErr = entry->SetActualData(byteSwappedDataBuffer, aComponentCount, KDataFormatSize[aFormat]);						
					}
					break;
				default:
					break;
				}
			User::Free(byteSwappedDataBuffer);
			if(updateErr)
				{
				if(updateErr == KErrNoMemory)
					{
					User::Leave(KErrNoMemory);
					}
				else
					{
					// if we get here entry object was made, but data could not be set
					// so we remove the entry object
					CleanupStack::PopAndDestroy( entry );
					entry = NULL;
					}
				}
			} // end if(aTotalSize > KSizeOfValueOffsetField)
		}

		return entry;	
	}
	

void CIfdGeneral::SetupDirectoryEntriesL()
	{
	// Loop through all legal tags.
	for(TInt i = 0; i < GetNumberOfValidTags(); i++)
		{
		const TUint* tagInfo = GetTagInformation(i);
		// Only initialise the default Ifd with Mandatory tags.
		if (tagInfo[ESupportLevelValue] == EMandatory)
			{
			TUint temp = tagInfo[EValueOffsetValue];
			TUint8* valueOffset = reinterpret_cast<TUint8*>(&temp);
			
			TUint format = tagInfo[EFormatValue];
			TUint componentCount = tagInfo[EComponentCountValue];
			if (format == EAscii)
				{
				valueOffset = reinterpret_cast<TUint8*> (temp);
				componentCount = 0;

				TUint8* strPtr = valueOffset;
				while(*strPtr++)
					componentCount++;
				}
			
			CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(tagInfo[ETagValue], format, componentCount, valueOffset, this);
			iIfdEntries.AppendL(entry); 	
			CleanupStack::Pop(entry);
			}			
		}
	}

TInt CIfdGeneral::LocateEntry(const TUint16 aTag) const
	{
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			return i;
			}
		}
	return KErrNotFound;
	}	
	
TInt CIfdGeneral::Ifd() const
	{
	return iIfdNumber;
	}

TUint CIfdGeneral::EntryCount() const
	{
	return iIfdEntries.Count();
	}
	
TUint8* CIfdGeneral::CreateIfdBlockL(TUint aOffset)
	{
	TUint allocSize = Size() + KIfdOffsetByteCount;
	aOffset += KIfdOffsetByteCount;
	TUint8* ifdBlock = static_cast<TUint8*>(User::AllocLC(allocSize)); // Allocate buffer for all IFD data.	
	WriteIfdDirEntriesL(ifdBlock, aOffset);
	TUint8* tempPtr = ifdBlock + EntryCount() * KMinimumIfdEntrySize;

	TInt temporary = 0;
	// The pointer to the next Ifd is temporarily zero-filled, and the actual value filled later.
	tempPtr = Mem::Copy(tempPtr, &temporary, sizeof(temporary));		

	WriteIfdData(tempPtr);
	CleanupStack::Pop();
	return ifdBlock;
	}	
	
TBool CIfdGeneral::AllowNullTags() 
	{
	// The general case
	return EFalse;
	}

TBool CIfdGeneral::IsValidIfdEntryCount(TUint16 aIfdEntryCount) const
	{
	// Check that aIfdEntryCount is reasonable
	// The check is fairly basic but we will check that the minimum size of the ifd entry 
	// based on the aIfdEntryCount does not exceed the exif data length
	// no check for minimum number of mandatory entries is done	
	if ( (iOffsetToIfd + sizeof(aIfdEntryCount) + (aIfdEntryCount * KMinimumIfdEntrySize)) > iExifDataLength)
		{
		return EFalse;
		}
	else
		{
		return ETrue;		
		}
	}


void CIfdGeneral::WriteIfdDirEntriesL(TUint8* aBlock, TUint aOffset)
	{
	TUint16 dirCount = EntryCount();

	//Address where data pointed to by offset values, starts.
	TUint offsetDataAddress = aOffset + sizeof(dirCount) + (dirCount * KMinimumIfdEntrySize); 
	
	// Copy each stored directory entry into the buffer.
	TInt i;
	TUint tag;
	TUint format;
	TUint sizeOfData;
	for(i = 0; i < dirCount; i++)
		{
		tag = iIfdEntries[i]->Tag();		
		format = iIfdEntries[i]->Format();
		
			
		TUint compCount = iIfdEntries[i]->ComponentCount();		
		TBool unicode = iIfdEntries[i]->IsUnicodeData();
		
		aBlock = Mem::Copy(aBlock, &tag, KSizeOfTagField); // Write tag.
		aBlock = Mem::Copy(aBlock, &format, KSizeOfFormatField); // Write format
		aBlock = Mem::Copy(aBlock, &compCount, KSizeOfComponentCountField); // Write component count.
		// tempPtr now points to the value/offset field.
	
		sizeOfData = iIfdEntries[i]->ExtraSize();
		
		// Here we must check  as to whether we are writing an offset to an IFD, since 
		// its component count is 1 and it is a long - thus check explicitly for tag number 0x8769
		// (Exif Sub Ifd), or 0xA005 (Interoperability Ifd), and write the offset.
		
		if((iIfdEntries[i]->Tag() == KTag8769[ETagValue]) ||
			(iIfdEntries[i]->Tag() == KTagA005[ETagValue]) ||
			(iIfdEntries[i]->Tag() == KThumbTag8769[ETagValue]) ||
			(iIfdEntries[i]->Tag() == KThumbTag0201[ETagValue]))
			{
			TUint address = Size() + aOffset + sizeof(dirCount);	
			// Write offset value.  The value to which this points will be written later.
			aBlock = Mem::Copy(aBlock, &address, KSizeOfValueOffsetField);			
			}				
		//Must check if we are writing an offset or an actual value.
		else if(sizeOfData > 0)
			{
			// Write offset value.  The value to which this points will be written later.
			aBlock = Mem::Copy(aBlock, &offsetDataAddress, KSizeOfValueOffsetField);
			// Move the offset address onward, ready for next time.
			offsetDataAddress += sizeOfData;
			}
		else
			{
			TUint valueOffset = iIfdEntries[i]->ValueOffset();
			aBlock = Mem::Copy(aBlock, &valueOffset, sizeof(valueOffset)); // Write actual value.				
			}
		// tempPtr now points to position after the value/offset field.
		}

	}
	
void CIfdGeneral::WriteIfdData(TUint8* aBlock)
	{
	TUint16 dirCount = EntryCount();

	TInt i;
	TUint format;
	TUint sizeOfData;
	
	// We have written the main blocks of data, so now need to write out the values that any offsets point to.
	for(i = 0; i < dirCount; i++)
		{
		format = iIfdEntries[i]->Format();
		sizeOfData = KDataFormatSize[format] * iIfdEntries[i]->ComponentCount();
		if(sizeOfData > KSizeOfValueOffsetField)
			{
			if(iIfdEntries[i]->ActualValue() != NULL)
				{
				aBlock = Mem::Copy(aBlock, iIfdEntries[i]->ActualValue(), sizeOfData);
				if ((sizeOfData%2) == 1) 
					{
					(*aBlock++) = '\0'; 
					}
				}
			}
		}
	}


// Does the given tag exist in this Ifd?
TBool CIfdGeneral::EntryExists(const TUint aTag) const
	{
	for(TInt i = 0; i < iIfdEntries.Count(); i++)
		{
		if(aTag == iIfdEntries[i]->Tag())
			{
			return ETrue;
			}
		}
	return EFalse;
	}
	
TBool CIfdGeneral::FindTag(const TUint aTag)
	{
	return GetTagIndex(aTag)!=KErrNotFound;
	}

	
TInt CIfdGeneral::GetFormat(TUint aTag, TUint& aFormat)
	{
	TInt index = GetTagIndex(aTag);
	if (index != KErrNotFound)
		{
		const TUint* tag = GetTagInformation(index);
		aFormat = tag[EFormatValue];
		return KErrNone;
		}
	return KErrNotFound;
	}
	
TInt CIfdGeneral::GetComponentCount(TUint aTag, TUint& aComponentCount)
	{
	TInt index = GetTagIndex(aTag);
	if (index != KErrNotFound)
		{
		const TUint* tag = GetTagInformation(index);
		aComponentCount = tag[EComponentCountValue];
		return KErrNone;
		}
	return KErrNotFound;
	}
	
TInt CIfdGeneral::GetTagIndex(const TUint aTag)
	{
	for (TInt i = 0 ; i< GetNumberOfValidTags(); i++ ) 
		{
		if (GetTagInformation(i)[ETagValue] == aTag)
			{
			return i;	
			}
		}	
	return KErrNotFound;
	}

void CIfdGeneral::RemoveEntryL(const TUint aTag)
	{
	TInt entry=LocateEntry(aTag);
	if(entry==KErrNotFound)
		{
		User::Leave(entry);
		}
	delete iIfdEntries[entry];
	iIfdEntries.Remove(entry);
	}

void CIfdGeneral::CheckMandatoryEntriesL()
	{
	}

// class CIfd0
CIfd0* CIfd0::NewLC(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CIfd0* self = new (ELeave) CIfd0(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;		
	}

	
TInt CIfd0::GetNumberOfValidTags()
	{
	return KIfd0NumberTags;		
	}
	
const TUint* CIfd0::GetTagInformation(TInt aIndex) 
	{
	if((aIndex < 0) || (aIndex >= KIfd0NumberTags))
		{//invalid argument
		return NULL;
		}
	else
		{
		TUint tagValue = KIfd0TagArray[aIndex][ETagValue];
		if(tagValue == KTag010F[ETagValue])
			{
			return iTag010F;
			}
		else if(tagValue == KTag0110[ETagValue])
			{
			return iTag0110;
			}
		else if(tagValue == KTag0131[ETagValue])
			{
			return iTag0131;
			}
		else 
			{
			return KIfd0TagArray[aIndex];
			}		
		}
	}

void CIfd0::ConstructL()
	{
	//For Device information tags - copy the contents from the constants and 
	//overwrite only the - EValueOffsetValue. 
	Mem::Copy(iTag010F, KTag010F, sizeof(KTag010F));
	Mem::Copy(iTag0110, KTag0110, sizeof(KTag0110));
	Mem::Copy(iTag0131, KTag0131, sizeof(KTag0131));
	//get the device information from SysUtil API.
	iManufacturerName = GetDeviceInfo(KTag010F[ETagValue]);
	iModelName = GetDeviceInfo(KTag0110[ETagValue]);
	iUIPlatform = GetDeviceInfo(KTag0131[ETagValue]);
	TUint8* data = NULL;
	//If reading from SysUtil API is not successful, let the tags carry the values from the constants.
	//Moreover, the tags manufacturer and model are mandatory. They should have non-NULL value for EValueOffsetValue.
	if(iManufacturerName != NULL)
		{
		data = const_cast<TUint8*>(iManufacturerName->Des().PtrZ());
		iTag010F[EValueOffsetValue] = reinterpret_cast<TUint>(data);
		}
	if(iModelName != NULL)
		{
		data = const_cast<TUint8*>(iModelName->Des().PtrZ());
		iTag0110[EValueOffsetValue] = reinterpret_cast<TUint>(data);
		}
	if(iUIPlatform != NULL)
		{
		data = const_cast<TUint8*>(iUIPlatform->Des().PtrZ());
		iTag0131[EValueOffsetValue] = reinterpret_cast<TUint>(data);
		}

	CIfdGeneral::ConstructL();
	}

//Reads device information from SysUtil API and makes a copy and returns the value.
//On failure returns NULL.If the tag aTagValue, does not belong to device information returns NULL.
HBufC8* CIfd0::GetDeviceInfo(TUint aTagValue)
	{
	HBufC8* buff = NULL;
	//if the tag is one of - manufacturer name, device model name and UI Platform - get 
	//the values from SysUtil.
	if((aTagValue == KTag010F[ETagValue]) || (aTagValue == KTag0110[ETagValue]) || (aTagValue == KTag0131[ETagValue]))
		{
		CDeviceTypeInformation* deviceAtts = NULL;
		TInt err = KErrNone;
		//Get device type information
		TRAP(err, deviceAtts = SysUtil::GetDeviceTypeInfoL());
		if(err == KErrNone && deviceAtts != NULL)
			{
			TPtrC16 ptr;
			if(aTagValue == KTag010F[ETagValue])
				{//manufacturer name
				err = deviceAtts->GetManufacturerName(ptr);
				}
			else if(aTagValue == KTag0110[ETagValue])
				{//model name
				err = deviceAtts->GetModelName(ptr);
				}
			else if(aTagValue == KTag0131[ETagValue])
				{//UI platform
				err = deviceAtts->GetUIPlatformName(ptr);
				}
			delete deviceAtts;
			//error code should be one of - KErrNone, KDefaultValue and KErrOverflow
			if((err == KErrNone) || (err == CDeviceTypeInformation::KDefaultValue) || (err == KErrOverflow))
				{
				buff = HBufC8::NewMax(ptr.Length() + 1);
				if(buff != NULL)
					{
					TPtr8 ptr8 = buff->Des();
					ptr8.Copy(ptr);
					}
				}			
			}
		}	
	return buff;
	}

CIfd0::~CIfd0()
	{
	delete iManufacturerName;
	delete iModelName;
	delete iUIPlatform;
	}

TBool CIfd0::IsValidIfdEntryCount(TUint16 aIfdEntryCount) const
	{
	//should have atleast one mandatory entry
	if(aIfdEntryCount >= 1)
		{
		return 	CIfdGeneral::IsValidIfdEntryCount(aIfdEntryCount);
		}
	else
		{
		return EFalse;			
		}
	}

CIfd0::CIfd0(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	: CIfdGeneral(EZeroth, aOffsetToIfd, aBase, aIntel, aExifDataLength)
	{
	}
	
//class CExifIfd
CExifIfd* CExifIfd::NewLC(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CExifIfd* self = new (ELeave) CExifIfd(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;		
	}
	
CExifIfd* CExifIfd::NewL(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CExifIfd* self = NewLC(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::Pop(self);
	return self;		
	}

TInt CExifIfd::GetNumberOfValidTags()
	{
	return KExifSubNumberTags;		
	}
	
const TUint* CExifIfd::GetTagInformation(TInt aIndex) 
	{
	return KExifSubTagArray[aIndex];
	}

void CExifIfd::CheckMandatoryEntriesL()
	{
	HBufC8* buffer = NULL;
	TInt err = GetParam8(KTag9000[ETagValue], buffer);
	if (err == KErrNotFound)
		{
		TUint temp = KTag9000[EValueOffsetValue];
		TUint8* valueOffset = reinterpret_cast<TUint8*>(&temp);
		TUint format = KTag9000[EFormatValue];
		TUint componentCount = KTag9000[EComponentCountValue];
		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(KTag9000[ETagValue], format, componentCount, valueOffset, this);
		iIfdEntries.AppendL(entry); 	
		CleanupStack::Pop(entry);
		}	
	delete buffer;
	buffer = NULL;	
	
	err = GetParam8(KTag9101[ETagValue], buffer);
	if (err == KErrNotFound)
		{
		_LIT8(KComponentsConfigurationDefault, "1230");
		buffer = KComponentsConfigurationDefault().AllocL();
		CleanupStack::PushL(buffer);
		TUint format = 0;
		err = GetFormat(KTag9101[ETagValue], format);		
		if (err == KErrNone) 
			{
			AddParam8L(KTag9101[ETagValue], format, buffer->Length(), buffer);
			}
		CleanupStack::Pop();
		}
	delete buffer;
	buffer = NULL;
			
	err = GetParam8(KTagA000[ETagValue], buffer);
	if (err == KErrNotFound)
		{
		TUint temp = KTagA000[EValueOffsetValue];
		TUint8* valueOffset = reinterpret_cast<TUint8*>(&temp);
		TUint format = KTagA000[EFormatValue];
		TUint componentCount = KTagA000[EComponentCountValue];
		CIfdGeneralEntry* entry = CIfdGeneralEntry::NewLC(KTagA000[ETagValue], format, componentCount, valueOffset, this);
		iIfdEntries.AppendL(entry); 	
		CleanupStack::Pop(entry);		
		}
	delete buffer;
	buffer = NULL;	
				
	TUint16 buffer16 = 0;
	err = GetShortParam(KTagA001[ETagValue], buffer16);
	if (err == KErrNotFound)
		{
		const TUint KColorSpaceDefault = 0xFFFF;
		TUint format = 0;
		err = GetFormat(KTagA001[ETagValue], format);		
		if (err == KErrNone) 
			{
			AddShortParamL(KTagA001[ETagValue], format, 1, KColorSpaceDefault);
			}
		}	
	}
	
TBool CExifIfd::CheckImageSizeTags()
	{
	TUint16 buffer16 = NULL;
	TInt err = GetShortParam(KTagA002[ETagValue], buffer16);
	if (err == KErrNotFound)
		{
		return EFalse;
		}
	err = GetShortParam(KTagA003[ETagValue], buffer16);
	if (err == KErrNotFound)
		{
		return EFalse;
		}
	return ETrue;
	}
	
void CExifIfd::UpdateImageSizeTagsL(const TSize& aSize)
	{
	TUint16 value = NULL;
	TInt err = GetShortParam(KTagA002[ETagValue], value);
	if (err == KErrNotFound)
		{
		TUint format = 0;
		err = GetFormat(KTagA002[ETagValue], format);		
		if (err == KErrNone) 
			{
			AddShortParamL(KTagA002[ETagValue], format, 1, aSize.iWidth);
			}
		}
	else
		{
		if (value != aSize.iWidth)
			{
			SetShortParam(KTagA002[ETagValue], aSize.iWidth);
			}
		}
	err = GetShortParam(KTagA003[ETagValue], value);
	if (err == KErrNotFound)
		{
		TUint format = 0;
		err = GetFormat(KTagA003[ETagValue], format);		
		if (err == KErrNone) 
			{
			AddShortParamL(KTagA003[ETagValue], format, 1, aSize.iHeight);
			}
		}
	else
		{
		if (value != aSize.iHeight)
			{
			SetShortParam(KTagA003[ETagValue], aSize.iHeight);
			}
		}
	}

CExifIfd::CExifIfd(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	: CIfdGeneral(EExifSub, aOffsetToIfd, aBase, aIntel, aExifDataLength)
	{	
	}


//class CInteropIfd
CInteropIfd* CInteropIfd::NewLC(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CInteropIfd* self = new (ELeave) CInteropIfd(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;		
	}

CInteropIfd* CInteropIfd::NewL(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CInteropIfd* self = NewLC(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::Pop(self);
	return self;	
	}
	
TInt CInteropIfd::GetNumberOfValidTags()
	{
	return KInteropNumberTags;		
	}
	
const TUint* CInteropIfd::GetTagInformation(TInt aIndex) 
	{
	return KInteropTagArray[aIndex];
	}

TBool CInteropIfd::IsValidIfdEntryCount(TUint16 aIfdEntryCount) const
	{
	//should have atleast one mandatory entry
	if(aIfdEntryCount >= 1)
		{
		return 	CIfdGeneral::IsValidIfdEntryCount(aIfdEntryCount);
		}
	else
		{
		return EFalse;			
		}
	}

CInteropIfd::CInteropIfd(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	: CIfdGeneral(EInterop, aOffsetToIfd, aBase, aIntel, aExifDataLength)
	{	
	}

//class CIfd1
CIfd1* CIfd1::NewLC(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CIfd1* self = new (ELeave) CIfd1(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;		
	}
	
	
TInt CIfd1::GetNumberOfValidTags()
	{
	return KIfd1NumberTags;		
	}
	
const TUint* CIfd1::GetTagInformation(TInt aIndex) 
	{
	return KIfd1TagArray[aIndex];
	}

CIfd1::CIfd1(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	: CIfdGeneral(EFirst, aOffsetToIfd, aBase, aIntel, aExifDataLength)
	{	
	}

//class CGpsIfd
CGpsIfd* CGpsIfd::NewLC(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CGpsIfd* self = new (ELeave) CGpsIfd(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;		
	}

CGpsIfd* CGpsIfd::NewL(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	{
	CGpsIfd* self = NewLC(aOffsetToIfd, aBase, aIntel, aExifDataLength);
	CleanupStack::Pop(self);
	return self;	
	}
	
TInt CGpsIfd::GetNumberOfValidTags()
	{
	return KGpsSubNumberTags;		
	}
	
const TUint* CGpsIfd::GetTagInformation(TInt aIndex) 
	{
	return KGpsSubTagArray[aIndex];
	}

TBool CGpsIfd::IsValidIfdEntryCount(TUint16 aIfdEntryCount) const
	{
	//should have atleast one mandatory entry
	if(aIfdEntryCount >= 1)
		{
		return 	CIfdGeneral::IsValidIfdEntryCount(aIfdEntryCount);
		}
	else
		{
		return EFalse;			
		}		
	}

CGpsIfd::CGpsIfd(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength)
	: CIfdGeneral(EGpsSub, aOffsetToIfd, aBase, aIntel, aExifDataLength)
	{	
	}

TBool CGpsIfd::AllowNullTags() 
	{
	// Null tags are allowed for GPS
	return ETrue;
	}
	
