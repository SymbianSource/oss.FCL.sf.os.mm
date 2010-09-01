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
#include "ExifTagHelper.h"
#include "ImageUtils.h"

const TUint KAnySize = 0; // For tags that have no upper limit on their content size.

GLDEF_C void Cleanup(TAny *aObject)
      {
      User::Free(aObject);
      }
 
// Sets the value/offset field (four bytes) of this Ifd entry.
void CIfdGeneralEntry::SetValueOffset(const TUint8* aValueOffset)
	{
	/*
	Need to check the type of data, and store accordingly - see Exif2.2 specification pp.14: 
	"If the value is < 4 bytes, the value is stored in the 4 byte area starting from the left,
	i.e. from the lower end of the byte offset.  For example, in big endian format, if the
	type is SHORT and the value is 1, it is recorded as 00010000.H."
	*/
	TUint totalSize = KDataFormatSize[Format()] * ComponentCount();
	
	switch(totalSize)
		{
		case (sizeof(TUint8)): // 1 byte in length.
			Mem::Copy(iDirEntryData+KValueOffsetFieldPosition, aValueOffset, sizeof(TUint8));
			Mem::FillZ(iDirEntryData+KValueOffsetFieldPosition+sizeof(TUint8), KThreeBytes); // Fill remaining 3 bytes with zeros.
			break;
		case (sizeof(TUint16)): // 2 bytes in length.
			{
			const TUint16* shortValue = reinterpret_cast<const TUint16*>(aValueOffset);
			Mem::Copy(iDirEntryData+KValueOffsetFieldPosition, shortValue, sizeof(TUint16)); // Fill remaining 2 bytes with zeros.
			Mem::FillZ(iDirEntryData+KValueOffsetFieldPosition+sizeof(TUint16), sizeof(TUint16));
			}
			break;
		case KThreeBytes: // 3 bytes in length.  Fall through.
		case (sizeof(TUint32)): // 4 bytes in length.
			{
			const TUint32* longValue = reinterpret_cast<const TUint32*>(aValueOffset);
			Mem::Copy(iDirEntryData+KValueOffsetFieldPosition, longValue, sizeof(TUint32));
			if(totalSize == KThreeBytes)
				{
				Mem::FillZ(iDirEntryData+KValueOffsetFieldPosition+KThreeBytes, sizeof(TUint8));
				}
			}
			break;
		}
	}


// Sets the tag field (two bytes) of this Ifd entry.
void CIfdGeneralEntry::SetTag(const TUint aTag)
	{
	Mem::Copy(iDirEntryData, &aTag, KSizeOfTagField);
	}

// Sets the format field (two bytes) of this Ifd entry.
void CIfdGeneralEntry::SetFormat(const TUint aFormat)
	{
	Mem::Copy(iDirEntryData+KFormatFieldPosition, &aFormat, KSizeOfFormatField);
	}

// Sets the component count field (four bytes) of this Ifd entry.
void CIfdGeneralEntry::SetComponentCount(const TUint aComponentCount)
	{
	Mem::Copy(iDirEntryData+KComponentCountFieldPosition, &aComponentCount, KSizeOfComponentCountField);
	}
	
// Returns the tag value of this Ifd entry.
TUint CIfdGeneralEntry::Tag()
	{
	return PtrReadUtil::ReadUint16(iDirEntryData);
	}

// Returns the format of this Ifd entry.
TUint CIfdGeneralEntry::Format()
	{
	return PtrReadUtil::ReadUint16(iDirEntryData+KFormatFieldPosition);
	}
	
// Returns the component count of this Ifd entry.
TUint CIfdGeneralEntry::ComponentCount()
	{
	return PtrReadUtil::ReadUint32(iDirEntryData+KComponentCountFieldPosition);
	}

// Returns the value/offset field of this Ifd entry.
TUint CIfdGeneralEntry::ValueOffset()
	{
	return PtrReadUtil::ReadUint32(iDirEntryData+KValueOffsetFieldPosition);
	}

// Returns a pointer to the data that the value/offset points to.
const TUint8* CIfdGeneralEntry::ActualValue()
	{
	// If there is data that the value/offset field points to, then return a pointer to it.
	return iActualData;
	}

TInt CIfdGeneralEntry::ExtraSize()
	{
	return TotalSize() - KMinimumIfdEntrySize;
	}

CIfdGeneralEntry* CIfdGeneralEntry::NewLC()
	{
	CIfdGeneralEntry* self = new (ELeave) CIfdGeneralEntry;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	

CIfdGeneralEntry* CIfdGeneralEntry::NewLC(const TUint& aTag, const TUint& aFormat, const TUint& aComponentCount, const TUint8* aParam, CIfdGeneral* aIfd)
	{
	TBool ignored; // NewLC is called by functions that don't require checking if the tag is known or not
	CIfdGeneralEntry* self = new (ELeave) CIfdGeneralEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aTag, aFormat, aComponentCount, aParam, aIfd, ignored);
	return self;
	}
	
CIfdGeneralEntry* CIfdGeneralEntry::NewL(const TUint& aTag, const TUint& aFormat, const TUint& aComponentCount, const TUint8* aParam, CIfdGeneral* aIfd, TBool& aUnknownTag)
	{
	CIfdGeneralEntry* self = new (ELeave) CIfdGeneralEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aTag, aFormat, aComponentCount, aParam, aIfd, aUnknownTag);
	CleanupStack::Pop(self);
	return self;
	}

CIfdGeneralEntry::CIfdGeneralEntry()
	{
	}

void CIfdGeneralEntry::ConstructL()
	{
	iDirEntryData = static_cast<TUint8*>(User::Alloc(KMinimumIfdEntrySize));
	if(iDirEntryData == NULL)
		{
		User::Leave(KErrNoMemory);
		}
	}

void CIfdGeneralEntry::ConstructL(const TUint& aTag, const TUint& aFormat, TUint aComponentCount, const TUint8* aValueOffset, CIfdGeneral* aIfd, TBool& aUnknownTag)
	{
	// the code here used to check if the tag was invalid and if so leave with KErrArgument
	// now if the tag is invalid it just bypasses the further checks
	// this is to future proof against versions of exif which introduce additional tags
	// to the exif 2.2 spec
	TUint expectedComponentCount = 0;

	ASSERT(aIfd);
		
	iIfd = aIfd;
	if(aIfd->FindTag(aTag))
		{
		// Known tag
		aUnknownTag = EFalse;
		
		TUint expectedFormat = 0;
		
		User::LeaveIfError(iIfd->GetFormat(aTag, expectedFormat));
		User::LeaveIfError(iIfd->GetComponentCount(aTag, expectedComponentCount));

		if ((aTag == KTagA002[ETagValue])||
			(aTag == KTagA003[ETagValue])||
			(aTag == KTag1001[ETagValue]) || 
			(aTag == KTag1002[ETagValue]))
			{
			//do nothing Tag A002 A003 can be short or long 
			// Interop Tag 1001 and 1002 can be unsigned short or long
			}
		else if(expectedFormat != aFormat)
			{
			User::Leave(KErrArgument);
			}
	
		if(expectedComponentCount != 0) // If 0, then size is undefined.
			{
			if(aComponentCount > expectedComponentCount)
				{
				User::Leave(KErrNotSupported);
				}
			}
		
		// Check overflow.
		if(aFormat >= KDataFormatSizeLength)
			{
			User::Leave(KErrOverflow);
			}
		}
	else
		{
		// Unknown tag
		// Create this entry anyway as it may be an extension to the EXIF standard
		// but inform the caller
		aUnknownTag = ETrue;
		}
	iDirEntryData = static_cast<TUint8*>(User::AllocL(KMinimumIfdEntrySize)); // Alloc standard entry size.
	Mem::FillZ(iDirEntryData, KMinimumIfdEntrySize);
	SetTag(aTag); // Simply set the tag.
	SetFormat(aFormat); // Set the format (associated with the tag).
	SetComponentCount(aComponentCount);	
		
	// Determine if we have a value or an offset to a value.
	TInt bytes = KDataFormatSize[aFormat];
	TUint size = aComponentCount * bytes;
	TUint8* tempValue = NULL;
	if(IsStringFormat())
		{
		tempValue = static_cast<TUint8*>(User::AllocL(size+sizeof(TUint8)));
		Mem::Copy(tempValue, aValueOffset, size);
		
		if (tempValue[size-1] != KNullCharacter)
			{	
			Mem::Copy(tempValue+size, &KNullCharacter, sizeof(KNullCharacter));	
			size++;
			aComponentCount++;
			SetComponentCount(aComponentCount);	
			}
			
		aValueOffset = tempValue;
		}				
	else if(IsRationalFormat())
		{
		//Since rational default values only store the numerator in the CExifTag constant we
		//must set the denominator.
		if(expectedComponentCount == 1)
			{
			tempValue = static_cast<TUint8*>(User::AllocL(KSizeOfRational));
			Mem::Copy(tempValue, aValueOffset, sizeof(TUint));
			TUint denominator = 1;
			Mem::Copy(tempValue+sizeof(TUint), &denominator, sizeof(TUint));
			aValueOffset = tempValue;		
			}
		}
	CleanupStack::PushL(TCleanupItem(Cleanup,tempValue));

	if(size > KSizeOfValueOffsetField)
		{
		// We have an Offset, so will set the actual value (i.e. that which the value/offset field points to).
		User::LeaveIfError(SetActualData(aValueOffset, aComponentCount, bytes));
		// Will not set the value/offset field now, as will be set later once the whole block of data is needed.
		}
	else
		{
		// We have an actual value, so will set this in the ValueOffset.		
		SetValueOffset(aValueOffset);
		// The actual value field will be left blank - no need to store the data twice.
		}	
	CleanupStack::Pop(tempValue);
	User::Free(tempValue);	
	}
	
CIfdGeneralEntry::~CIfdGeneralEntry()
	{
	// from encoder
	delete iDirEntryData;
	delete iActualData;
	}

// Sets the data that the value/offset field points to.
TInt CIfdGeneralEntry::SetActualData(const TAny* aActualData, const TUint aSize, const TUint aBytes)
	{
	/* 
	Pass aSize, since we don't know how many components the value has.
	Will need to check the format type, in order to overcome the problems with 'undefined' types.
	*/
	TInt size = aSize * aBytes;
	iActualData = static_cast<TUint8*>(User::ReAlloc(iActualData, size));

	if(iActualData == NULL)
		{
		return KErrNoMemory;
		}
	Mem::Copy(iActualData, aActualData, size);

	SetComponentCount(aSize);
	return KErrNone;
	}

// Updates a rational that has a component count of 1.	
TInt CIfdGeneralEntry::UpdateRational(TInt aNumerator, TInt aDenominator)
	{
	// Cannot change a rational entry with a component count > 1 or a non-rational entry.
	if(ComponentCount() > 1 || !IsRationalFormat())
		{		
		return KErrArgument;
		}
	// We're only dealing with a single rational value.
	
	// A rational is always > 4 bytes long, so we store it in the actual data field.	
	TUint8* rational = static_cast<TUint8*>(User::Alloc(KSizeOfRational));
	if(rational == NULL)
		{
		return KErrNoMemory;
		}
	TUint8* tempPtr = Mem::Copy(rational, &aNumerator, sizeof(aNumerator));
    Mem::Copy(tempPtr, &aDenominator, sizeof(aDenominator));    
	if(SetActualData(rational, sizeof(TUint8), KSizeOfRational) == KErrNoMemory)
		{
		User::Free(rational);
		return KErrNoMemory;
		}
	User::Free(rational);
	return KErrNone;
	}

// Updates a value of ASCII type when Ifd knowledge is needed.
TInt CIfdGeneralEntry::UpdateString(const HBufC8* aStringValue)
	{
	if(aStringValue == NULL)
		{
		return KErrArgument;
		}
		
	//Need to check format type of object.
	if(!IsStringFormat())
		{
		// Cannot change non-ASCII entry.
		return KErrArgument;
		}
			
	TUint legalCompCount = KAnySize;
	TInt err = KErrNone;

	err = iIfd->GetComponentCount(Tag(), legalCompCount);
	
	TUint stringLength=aStringValue->Length();	
	const TUint8* ptr = aStringValue->Ptr();		
	TBool nullTerminationNeeded = EFalse;
	
	//Do some string length checks.
	if (legalCompCount != KAnySize)
		{
		// We have a fixed length string type. Only accept strings that equal the
		// fixed length when NULL termianted.
		if (stringLength == legalCompCount-1 && ptr[stringLength-1] != KNullCharacter)
			{
			//If we add a NULL termination string will be legal
			nullTerminationNeeded = ETrue;
			}
		else if (stringLength == legalCompCount && ptr[stringLength-1] == KNullCharacter)
			{
			//Okay as it is.
			}
		else
			{
			return KErrArgument;
			}
		}
	else
		{
		//No length checks needed, but still might have to add a null terminator.
		nullTerminationNeeded = (ptr[stringLength-1] != KNullCharacter);
		}
		
	HBufC8* stringValueCopy = NULL;
	if (nullTerminationNeeded)
		{
		stringValueCopy = HBufC8::New(aStringValue->Length()+1);
		if(stringValueCopy == NULL)
			{
			return KErrNoMemory;
			}

		*stringValueCopy = *aStringValue;
		stringValueCopy->Des().Append(KNullCharacter);
		stringLength++;
		
		ptr = stringValueCopy->Ptr();			
		}
		
	if(stringLength > KSizeOfValueOffsetField)
		{
		// We have an Offset, so will set the actual value.		
		err = SetActualData(ptr, stringLength, sizeof(TUint8));
		// Will NOT set the value/offset field now, since it will no-doubt change.
		}
	else
		{
		// We have an actual value, so will set this in the ValueOffset.		
		SetComponentCount(stringLength);
		SetValueOffset(ptr);
		// The actualvalue field will be left blank - no need to store the data twice.
		err=KErrNone;
		}	
	delete stringValueCopy;	
	return err;
	}
	
/*
	Must have a catch-all update function since there are some anomolies to the standard rules.		
	Furthermore, this function may also be used to set data of the undefined format.
*/
TInt CIfdGeneralEntry::UpdateData(TUint aComponentCount, const TUint8* aData)
	{
	TUint maxSize = 0;
	// Get the maximum possible size.
	TUint tag = Tag();
		
	iIfd->GetComponentCount(tag, maxSize);
		
	if(maxSize != KAnySize) // If KAnySize, then size is undefined.
		{
		if(aComponentCount > maxSize)
			{
			return KErrNotSupported;
			}
		}

	// Check overflow.  Will indicate that the format-type is not recognised.
	TUint format = Format();
	if(format == 0)
		{
		return KErrCorrupt;
		}
	
	if(format >= KDataFormatSizeLength)
		{
		return KErrNotSupported;
		}
		
	// We are updating existing data, and so will already know the format type.
	// Determine if we have a value or an offset to a value.
	TInt bytes = KDataFormatSize[format];
	TUint size = aComponentCount * bytes;

	TUint8* tempValue = NULL;
	// Add NULL to the end if it's ASCII
	if(IsStringFormat())
		{
		// verify if the last character is NULL
		if(aData[aComponentCount-1]==KNullCharacter)
			{
			//String already NULL terminated.
			if ((maxSize != KAnySize) && (aComponentCount != maxSize))
				{
				return KErrNotSupported;
				}
			}
		else
			{
			// Note: This branch is only executed when reading an Exif source
			// that contains a non-NULL terminated ASCII string, which is 
			// against the Exif 2.2 Spec.

			// Since we will add a terminating NULL to the string,
			// must check max size of this particular tag.
			if((maxSize != KAnySize) && (aComponentCount != maxSize-1))
				{
				return KErrNotSupported;
				}
			
			//Add the NULL terminator
			tempValue = static_cast<TUint8*>(User::Alloc(size + sizeof(TUint8)));
			if(tempValue == NULL)
				{
				return KErrNoMemory;
				}
			Mem::Copy(tempValue, aData, size);
			Mem::Copy(tempValue+size, &KNullCharacter, sizeof(TUint8));
			aData = tempValue;
			size++;
			aComponentCount++;
			}
		}
	TInt err=KErrNone;
	if(size > KSizeOfValueOffsetField)
		{
		// We have an Offset, so will set the actual value (i.e. that which the value/offset field points to).
		err = SetActualData(aData, aComponentCount, bytes);
		// Will not set the value/offset field now, as will be set later once the whole block of data is needed.
		}
	else
		{
		// We have an actual value, so will set this in the ValueOffset.		
		SetComponentCount(aComponentCount);
		SetValueOffset(aData);
		// The actual value field will be left blank - no need to store the data twice.
		}
	User::Free(tempValue);
	return err;
	}
	

// Updates an integer value (including short, integer, long).
TInt CIfdGeneralEntry::UpdateInteger(const TInt aIntegerValue)
	{
	// Need to check format type of object, to avoid calling this on an entry that is not an integer.
	if(!IsIntegerFormat())
		{
		//first check if tag is 0xA002 or A003 - this could be short
		//but when setting it we can set it as long
		TUint16 tag = Tag();
		if ((tag == KTagA002[ETagValue])||(tag == KTagA003[ETagValue]))
			{
			//tag is A002 or A003 and is short - set entry to long
			SetFormat(EUnsignedLong);
			}
		else
			{
			// Cannot change non-integer entry.
			return KErrArgument;
			}
		}
	
	// Check overflow.
	TUint format = Format();
	if(format == NULL)
		{
		return KErrCorrupt;
		}
	
	if(format >= KDataFormatSizeLength)
		{
		return KErrNotSupported;
		}
	
	// Determine if we have a value or an offset to a value.
	TInt bytes = KDataFormatSize[format];
	TUint size = ComponentCount() * bytes;
	if(size > KSizeOfValueOffsetField)
		{
		// We have an Offset, so will set the actual value...
		return SetActualData(&aIntegerValue, ComponentCount(), bytes);
		// Will NOT set the value/offset field now, since it will no-doubt change.
		}
	else
		{
		// We have an actual value, so will set this in the ValueOffset.		
		SetValueOffset(reinterpret_cast<const TUint8*>(&aIntegerValue));
		return KErrNone;
		// The actualvalue field will be left blank - no need to store the data twice.
		}	
	}
	
// Updates a short value.
TInt CIfdGeneralEntry::UpdateShort(const TUint16 aShortValue)
	{
	// Need to check format type of object, to avoid calling this on an entry that is not a short.
	if(!IsShortFormat())
		{
		// Cannot change non-short entry.
		return KErrArgument;
		}
	
	// Check overflow.
	TUint format = Format();
	if(format == NULL)
		{
		return KErrCorrupt;
		}
	
	if(format >= KDataFormatSizeLength)
		{
		return KErrNotSupported;
		}
	
	// Determine if we have a value or an offset to a value.
	TInt bytes = KDataFormatSize[format]; // value is 2, since it is a short.
	TUint size = ComponentCount() * bytes;
	if(size > KSizeOfValueOffsetField)
		{
		// We have an Offset, so will set the actual value...
		return SetActualData(&aShortValue, ComponentCount(), bytes);
		// Will NOT set the value/offset field now, since it will no-doubt change.
		}
	else
		{
		// We have an actual value, so will set this in the ValueOffset.		
		SetValueOffset(reinterpret_cast<const TUint8*>(&aShortValue));
		return KErrNone;
		// The actualvalue field will be left blank - no need to store the data twice.
		}	
	}

// Returns the total size of this Ifd entry. The function should never return zero. 
// The minimum value returned should be KMinimumIfdEntrySize, in case of any invalidity with
// the entry such as invalid format.
TInt CIfdGeneralEntry::TotalSize()
	{
	TUint16 format = Format();
	TBool formatValidity = EFalse;
	if(format <= KDataFormatSizeLength-1)
		{
		//formats - 0, 6, 8, are invalid, which will have size 0.
		if(KDataFormatSize[format] != 0)
			{
			formatValidity = ETrue;
			}
		}
	ASSERT(formatValidity);
	//in case of invalid format, return minimum size
	if (!formatValidity) 
		{
		return KMinimumIfdEntrySize;
		}
		
	TUint size = (KDataFormatSize[format] * ComponentCount());
	if(size > KSizeOfValueOffsetField)
		{
		// We have an offset value, so include extra data length (i.e. past the KMinimumIfdEntrySize byte limit).
		if ((size%2)==1) 
			{
			size++;
			}
		return KMinimumIfdEntrySize + size;
		}
	return KMinimumIfdEntrySize;
	}
	
// Does this entry contain rational data?
TBool CIfdGeneralEntry::IsRationalFormat()
	{
	TUint format = Format();
	return format == EUnsignedRational || 
			format == ESignedRational;
	}
	
// Does this entry contain string data?
TBool CIfdGeneralEntry::IsStringFormat()
	{
	return Format() == EAscii;
	}
	
// Does this entry contain 'undefined' data?
TBool CIfdGeneralEntry::IsUndefinedFormat()
	{
	return Format() == EUndefined;
	}

// Does this entry contain integer data?
TBool CIfdGeneralEntry::IsIntegerFormat()
	{	
	switch(Format())
		{
		case EByte:		
		case EUnsignedLong:
		case ESignedLong:
			{
			return ETrue;
			}			
		default:
			{
			return EFalse;	
			}
		}		
	}
	
// Does this entry contain short data?
TBool CIfdGeneralEntry::IsShortFormat()
	{	
	return Format() == EUnsignedShort;	
	}
	
// Returns ETrue if the data is Unicode.
TBool CIfdGeneralEntry::IsUnicodeData()
	{
	TUint tag = Tag();
	//Only the following undefined tags may potentially be Unicode
	TBool tagIsPotentiallyUnicode=(	KTag9286[ETagValue] == tag		|| 
									KTagGPS001B[ETagValue] == tag 	||
									KTagGPS001C[ETagValue] == tag		);
								
	// Need to ensure that the correct component count is returned for the unicode data.
	if (tagIsPotentiallyUnicode && ExtraSize() >= KUnicode().Size())
		{
		return (0 == Mem::Compare(iActualData, KUnicode().Length() , KUnicode().Ptr(), KUnicode().Length()));
		}
	else
		{
		return EFalse;
		}
	}
