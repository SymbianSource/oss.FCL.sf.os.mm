// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <fbs.h>
#include "ImageClientMain.h"
#include "TIFFCodec.h"
#include "TIFFFax.h"
#include "ImageUtils.h"

// CTiffReadCodec.
CTiffReadCodec::CTiffReadCodec(TTiffFormatInfo aFormatInfo, CTiffDecoder& aPlugin)
 :	iFormatInfo(aFormatInfo),
 	iValueReader(aFormatInfo.iEndianness),
 	iPlugin(aPlugin)
	{}

CTiffReadCodec::~CTiffReadCodec()
	{
	if (iFrameImageData && iFrameImageData->iStripInfo)
		delete iFrameImageData->iStripInfo;

	delete iFrameImageData;
	delete iIfdBuffer;
	delete iLongValuesBuffer;
	delete iDecoder;
	delete iRecordTable;
	}

CTiffReadCodec* CTiffReadCodec::NewL(TTiffFormatInfo aFormatInfo, CTiffDecoder& aPlugin)
	{
	CTiffReadCodec* self = new(ELeave) CTiffReadCodec(aFormatInfo, aPlugin);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}

void CTiffReadCodec::InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& aFrameImageData)
	{
	iImageInfo = &aFrameSettings;
	iImageData = &aFrameImageData;
	
	if (iRecordTable && (iProcessHeaderState == EReadIfd))
		{
		iRecordTable->Reset();
		}
	}

TFrameState CTiffReadCodec::ProcessFrameHeaderL(TBufPtr8& aData)
	{
	const TUint8* ptr	    = aData.Ptr();
	const TUint8* const ptrStart	= ptr;
	const TUint8* const ptrLimit	= ptr + aData.Length();

	if (iImageInfo->CurrentFrameState() == TFrameInfo::EFrameInfoUninitialised)
		{
		iImageInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrameHeader);

		if (ptrStart + 4 > ptrLimit)
			User::Leave(KErrUnderflow);

		TUint32 signature = PtrReadUtil::ReadUint32(ptr);

		if (iFormatInfo.iSignature == signature)
			return EFrameComplete;

		iProcessHeaderState = EReadIfd;
		iNumIfdEntries = 0;
		iLongValuesStartOffset = KMaxTInt;
		iLongValuesEndOffset = 0;
		iLoadedLongValuesSize = 0;
	
		if (iFrameImageData && iFrameImageData->iStripInfo)
			delete iFrameImageData->iStripInfo;

		delete iFrameImageData;
		iFrameImageData = NULL;

		iFrameImageData = new(ELeave) TTiffImageData;
		iFrameImageData->iNewSubfileType = 0;
		iFrameImageData->iSizeInPixels = TSize(0, 0);
		iFrameImageData->iBitsPerSample = 0;
		iFrameImageData->iSamplesPerPixel = 0;
		iFrameImageData->iCompression = 0;
		iFrameImageData->iT4Options = 0;
		iFrameImageData->iT6Options = 0;
		iFrameImageData->iPhotometricInterpretation = 0;
		iFrameImageData->iFillOrder = 0;
		iFrameImageData->iWidthInTwips = 0.0;
		iFrameImageData->iHeightInTwips = 0.0;
		iFrameImageData->iRowsPerStrip = 0;
		iFrameImageData->iNumStrips = 0;
		iFrameImageData->iStripInfo = NULL;
		}

	TInt err = KErrNone;
	
	FOREVER
		{
		switch(iProcessHeaderState)
			{
		case EReadIfd:
			if (!iRecordTable)
				{
				iRecordTable = CRecordTable::NewL();
				}
			
			iIfdSize = 0;
			iIfdOffset = iPlugin.CurrentFilePosition();
			err = ReadIfdL(ptr, ptrLimit, iIfdSize);

			if (err != KErrNone)
				{
				return EFrameIncomplete;
				}
			
			if (iLongValuesSize)
				{
				iProcessHeaderState = EReadLongValues;
				aData.Shift(aData.Length());
				iNewPosition = iLongValuesStartOffset;
				return EFrameIncompleteRepositionRequest;
				}

			iProcessHeaderState = EProcessIfd;
			break;

		case EReadLongValues:

			err = ReadLongValuesL(ptr, ptrLimit);
			aData.Shift(ptr-ptrStart);
			if (err != KErrNone)
				return EFrameIncomplete;

			iProcessHeaderState = EProcessIfd;
			break;

		case EProcessIfd:

			ProcessIfdL();
			aData.Shift(aData.Length());

			iProcessHeaderState = EFinish;
			break;

		case EFinish:
			{
			err = iRecordTable->InsertRecordL(iIfdOffset, iIfdSize);
			if (err != KErrNone && err != KErrAlreadyExists)
				{
				User::Leave(KErrCorrupt);
				}
			
			iIfdOffset = 0;
			iIfdSize = 0;

			// Check that we got strip info.
			if (iFrameImageData->iStripInfo == NULL)
				User::Leave(KErrCorrupt);
			
			err = iImageData->AppendImageBuffer(iFrameImageData->iStripInfo);
			if (err != KErrNone)
				{
				delete iFrameImageData->iStripInfo;
				delete iFrameImageData;
				iFrameImageData = NULL;

				User::Leave(err);
				}

			err = iImageData->AppendFrameData(iFrameImageData);
			if(err != KErrNone)
				{
				delete iFrameImageData;
				iFrameImageData = NULL;

				User::Leave(err);
				}

			iImageInfo->iFrameCoordsInPixels = iFrameImageData->iSizeInPixels;
			iImageInfo->iOverallSizeInPixels = iFrameImageData->iSizeInPixels;
			iImageInfo->iFrameSizeInTwips.SetSize(STATIC_CAST(TInt, iFrameImageData->iWidthInTwips), STATIC_CAST(TInt, iFrameImageData->iHeightInTwips));
			iImageInfo->iBitsPerPixel = 1;
			iImageInfo->iFrameDisplayMode = EGray2;
			iImageInfo->iDelay = 0;
			iImageInfo->iFlags = TFrameInfo::EPartialDecodeInvalid;
				// in all of the test files we've seen, the frame info follows the frame data
				// so it has not been possible to check to see if it would work
				// If this has changed, visit commented out ClearBitmap above
			iImageInfo->SetFrameDataOffset(iStripInfo[0].iOffset - iFormatInfo.iFirstIfd);

			iFrameImageData = NULL;
			iImageInfo->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
			if (iNextIfdOffset == 0)
				{
				delete iRecordTable;
				iRecordTable = NULL;
				return EFrameComplete;
				}
			else
				{
				iNewPosition = iNextIfdOffset;
				return EFrameIncompleteRepositionRequest;
				}
			}

		default:
			Panic(EUnknownHeaderState);
			}
		}
	}

TInt CTiffReadCodec::ReadIfdL(const TUint8*& aPtr, const TUint8* aPtrLimit, TInt& aIfdSizeInBytes)
	{
	const TUint8* ptr = aPtr;

	if (ptr + 4 > aPtrLimit)
		return(KErrUnderflow);

	iNumIfdEntries = iValueReader.ReadUint16(ptr);

	ptr += 2;

	if (ptr + iNumIfdEntries * KTiffIfdEntryLength + 4 > aPtrLimit)
		return(KErrUnderflow);

	aIfdSizeInBytes = iNumIfdEntries * sizeof(TTiffIfdEntry);
	
	if (iIfdBuffer)
		{
		if (iIfdBuffer->Length() < aIfdSizeInBytes)
			{
			delete iIfdBuffer;
			iIfdBuffer = NULL;
			}
		}

	if (!iIfdBuffer)
		iIfdBuffer = HBufC8::NewMaxL(aIfdSizeInBytes);

	TTiffIfdEntry* entry = REINTERPRET_CAST(TTiffIfdEntry*, CONST_CAST(TUint8*, iIfdBuffer->Des().Ptr()));
	iIfdEntries = entry;

	for (TInt i = 0; i < iNumIfdEntries; i++)
		{
		ReadIfdEntryL(*entry, ptr);
		entry++;
		ptr += KTiffIfdEntryLength;
		}

	iNextIfdOffset = iValueReader.ReadUint32(ptr);
	aPtr = ptr + 4;

	if (iLongValuesStartOffset < iLongValuesEndOffset)
		iLongValuesSize = iLongValuesEndOffset - iLongValuesStartOffset;
	else
		iLongValuesSize = 0;

	return(KErrNone);
	}

void CTiffReadCodec::ReadIfdEntryL(TTiffIfdEntry& aEntry, const TUint8*const aPtr)
	{
	aEntry.iId		= STATIC_CAST(TTiffIfdEntry::TId, iValueReader.ReadUint16(aPtr + KTiffIfdEntryTagOffset));
	aEntry.iType	= STATIC_CAST(TTiffIfdEntry::TType, iValueReader.ReadUint16(aPtr + KTiffIfdEntryTypeOffset));
	aEntry.iCount	= iValueReader.ReadUint32(aPtr + KTiffIfdEntryCountOffset);

	const TInt valueSize = aEntry.TypeSize()*aEntry.iCount;

	if (valueSize <= 4)
		{
		// read as if TUint32* - equiv to Mem::Copy(&aEntry.iValue, aPtr+KTiffIfdEntryValueOffset, 4);
		aEntry.iValue = PtrReadUtil::ReadUint32(aPtr+KTiffIfdEntryValueOffset);

		aEntry.iValuePtr = REINTERPRET_CAST(TUint8*, &aEntry.iValue);
		}
	else
		{
		TInt valueOffset = iValueReader.ReadUint32(aPtr + KTiffIfdEntryValueOffset);
		aEntry.iValue = valueOffset;

		aEntry.iValuePtr = NULL;

		if (valueOffset < iLongValuesStartOffset)
			iLongValuesStartOffset = valueOffset;
		if (valueOffset + valueSize > iLongValuesEndOffset)
			iLongValuesEndOffset = valueOffset + valueSize;
		}
	}

TInt CTiffReadCodec::ReadLongValuesL(const TUint8*& aPtr, const TUint8*const aPtrLimit)
	{
	if(!iLoadedLongValuesSize)
		{
		if (iLongValuesSize<0)
			User::Leave(KErrCorrupt);
				
		if (iLongValuesSize>=(KMaxTInt/2)) // User alloc limit.
			User::Leave(KErrNoMemory);

		if (iLongValuesBuffer)
			{
			if (iLongValuesBuffer->Length() < iLongValuesSize)
				{
				delete iLongValuesBuffer;
				iLongValuesBuffer = NULL;
				}
			}

		if (!iLongValuesBuffer)
			iLongValuesBuffer = HBufC8::NewMaxL(iLongValuesSize);

		iLongValues = CONST_CAST(TUint8*, iLongValuesBuffer->Des().Ptr());
		}

	TInt size = iLongValuesSize - iLoadedLongValuesSize;

	if (aPtr+size>aPtrLimit)
		size = aPtrLimit-aPtr;

	Mem::Copy(iLongValues + iLoadedLongValuesSize, aPtr, size);
	aPtr += size;

	iLoadedLongValuesSize += size;

	if (iLoadedLongValuesSize!=iLongValuesSize)
		return(KErrUnderflow);

	TTiffIfdEntry* entry = iIfdEntries;
	for(TInt i = 0 ; i < iNumIfdEntries; i+=1)
		{
		if (!entry->iValuePtr)
			{
			TInt entryPos = entry->iValue-iLongValuesStartOffset;
			if ((entryPos<0) || (entryPos>=iLongValuesSize))
				User::Leave(KErrCorrupt);

			TInt valueSize = entry->TypeSize()*entry->iCount;
			if ((entryPos+valueSize)>iLongValuesSize)
				User::Leave(KErrCorrupt);

			entry->iValuePtr = iLongValues+entryPos;
			}

		entry++;
		}

	return(KErrNone);
	}

void CTiffReadCodec::ProcessIfdL()
	{
	TTiffIfdEntry* entry = iIfdEntries;

	for (TInt i = 0; i < iNumIfdEntries; i+=1)
		{
		ProcessIfdEntryL(*entry);
		entry++;
		}
	}

void CTiffReadCodec::ProcessIfdEntryL(const TTiffIfdEntry& aEntry)
	{
	switch(aEntry.iId)
		{
	case TTiffIfdEntry::ENewSubfileType:

		iFrameImageData->iNewSubfileType = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::ESubfileType:
		{
		TUint32 type = IntegerIfdEntryValueL(aEntry) - 1;	//Convert SubfileType to NewSubfileType
		if (type < 3)
			iFrameImageData->iNewSubfileType = type;
		}
		break;

	case TTiffIfdEntry::EImageWidth:

		iFrameImageData->iSizeInPixels.iWidth = IntegerIfdEntryValueL(aEntry);
		if (iFrameImageData->iSizeInPixels.iWidth<0)
			User::Leave(KErrCorrupt);
		break;

	case TTiffIfdEntry::EImageLength:

		iFrameImageData->iSizeInPixels.iHeight = IntegerIfdEntryValueL(aEntry);
		if (iFrameImageData->iSizeInPixels.iHeight<0)
			User::Leave(KErrCorrupt);
		break;

	case TTiffIfdEntry::EBitsPerSample:

		iFrameImageData->iBitsPerSample = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::ECompression:

		iFrameImageData->iCompression = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::EPhotometricInterpretation:

		iFrameImageData->iPhotometricInterpretation = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::EFillOrder:

		iFrameImageData->iFillOrder = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::EStripOffsets:
		{
		if (aEntry.iCount<=0)
			User::Leave(KErrCorrupt);
		iFrameImageData->iNumStrips = aEntry.iCount;

		TInt stripBytes = iFrameImageData->iNumStrips * sizeof(TTiffImageStrip);
		if ((stripBytes<0) || (stripBytes>=(KMaxTInt/2))) // User alloc limit.
			User::Leave(KErrNoMemory);

		ASSERT(iFrameImageData->iStripInfo==NULL);
		iFrameImageData->iStripInfo = HBufC8::NewMaxL(stripBytes);

		TInt i;
		TPtr8 stripInfoPtr(iFrameImageData->iStripInfo->Des());
		iStripInfo = REINTERPRET_CAST(TTiffImageStrip*, CONST_CAST(TUint8*, stripInfoPtr.Ptr()));
		for(i = 0; i < iFrameImageData->iNumStrips; i++)
			iStripInfo[i].iOffset = IntegerIfdEntryValueL(aEntry,i);
		}
		break;

	case TTiffIfdEntry::ESamplesPerPixel:

		iFrameImageData->iSamplesPerPixel = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::ERowsPerStrip:

		iFrameImageData->iRowsPerStrip = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::EStripByteCounts:
		{
		TInt i;
		for(i = 0; i < iFrameImageData->iNumStrips; i++)
			iStripInfo[i].iLength = IntegerIfdEntryValueL(aEntry, i);
		}
		break;

	case TTiffIfdEntry::EXResolution:
		{		
		TReal resolution = RationalIfdEntryValueL(aEntry);
		if (resolution != 0)
			iFrameImageData->iWidthInTwips = STATIC_CAST(TReal, iFrameImageData->iSizeInPixels.iWidth) * STATIC_CAST(TReal, KTwipsPerInch) / resolution;
		}
		break;

	case TTiffIfdEntry::EYResolution:
		{
		TReal resolution = RationalIfdEntryValueL(aEntry);
		if (resolution != 0)
			iFrameImageData->iHeightInTwips = STATIC_CAST(TReal, iFrameImageData->iSizeInPixels.iHeight) * STATIC_CAST(TReal, KTwipsPerInch) / resolution;
		}
		break;

	case TTiffIfdEntry::ET4Options:

		iFrameImageData->iT4Options = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::ET6Options:

		iFrameImageData->iT6Options = IntegerIfdEntryValueL(aEntry);
		break;

	case TTiffIfdEntry::EResolutionUnit:

		if (IntegerIfdEntryValueL(aEntry) == 3)	// Resolution units are centimeters rather than inches
			{
			const TReal KInchesPerCentimeter = 1.0 / 2.54;
			iFrameImageData->iWidthInTwips *= KInchesPerCentimeter;
			iFrameImageData->iHeightInTwips *= KInchesPerCentimeter;
			}
		break;

	default:
		break;
		}
	}

TInt CTiffReadCodec::IntegerIfdEntryValueL(const TTiffIfdEntry& aEntry, TInt aIndex) const
	{
	if (aIndex >= aEntry.iCount)
		User::Leave(KErrCorrupt);

	TUint8* ptr = aEntry.iValuePtr;

	switch(aEntry.iType)
		{
	case TTiffIfdEntry::EByte:
		return(ptr[aIndex]);

	case TTiffIfdEntry::ESbyte:
		return(STATIC_CAST(TInt8, ptr[aIndex]));

	case TTiffIfdEntry::EShort:
		return(iValueReader.ReadUint16(ptr + aIndex * 2));
	
	case TTiffIfdEntry::ESshort:
		return(iValueReader.ReadInt16(ptr + aIndex * 2));
	
	case TTiffIfdEntry::ELong:
		return(iValueReader.ReadUint32(ptr + aIndex * 4));
	
	case TTiffIfdEntry::ESlong:
		return(iValueReader.ReadInt32(ptr + aIndex * 4));
	
	default:
		User::Leave(KErrCorrupt);
		return(0);
		}
	}

TInt CTiffReadCodec::IntegerIfdEntryValueL(const TTiffIfdEntry& aEntry) const
	{
	if (aEntry.iCount != 1)
		User::Leave(KErrCorrupt);

	return(IntegerIfdEntryValueL(aEntry, 0));
	}

TReal CTiffReadCodec::RationalIfdEntryValueL(const TTiffIfdEntry& aEntry) const
	{
	if (aEntry.iCount != 1)
		User::Leave(KErrCorrupt);

	TUint8* ptr = aEntry.iValuePtr;

	switch(aEntry.iType)
		{

	case TTiffIfdEntry::ERational:
		{
		TInt numerator = iValueReader.ReadUint32(ptr);
		TInt denominator = iValueReader.ReadUint32(ptr + 4);
		if (!denominator)
			User::Leave(KErrCorrupt);
		return(STATIC_CAST(TReal, numerator) / STATIC_CAST(TReal, denominator));
		}

	default:
		User::Leave(KErrCorrupt);
		return(0);

		}

	}
void CTiffReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aBitmap, CFbsBitmap* /*aDestinationMask*/)
	{
	iImageInfo = &aFrameInfo;
	iImageData = &aFrameImageData;
	iTiffImageData = STATIC_CAST(TTiffImageData*, iImageData->GetFrameData(0));

	TPtr8 stripInfoPtr(iTiffImageData->iStripInfo->Des());
	iStripInfo = REINTERPRET_CAST(TTiffImageStrip*, CONST_CAST(TUint8*, stripInfoPtr.Ptr()));

	delete iDecoder;
	iDecoder = NULL;

	switch(iTiffImageData->iCompression)
		{
	case TTiffIfdEntry::EGroup3FaxCompression:

		if (iTiffImageData->iT4Options & TTiffIfdEntry::ET4TwoDimentionalCoding)
			iDecoder = CTiffGroup3Fax2dDecoder::NewL(*iTiffImageData);
		else
			iDecoder = CTiffGroup3Fax1dDecoder::NewL(*iTiffImageData);
		break;

	case TTiffIfdEntry::EGroup4FaxCompression:

		iDecoder = CTiffGroup4FaxDecoder::NewL(*iTiffImageData);
		break;

	default: 
		User::Leave(KErrNotSupported);
		break;
		}

	const TSize destinationSize(aBitmap.SizeInPixels());
	TInt reductionFactor = ReductionFactor(iImageInfo->iOverallSizeInPixels, destinationSize);
	iDecoder->DoNewFrameL(aBitmap, aDisableErrorDiffusion, reductionFactor);

	// ClearBitmapL(aBitmap, KRgbWhite);
		// No need to do something sensible when we do partial decodes on streaming
		// as tiff cannot return a partial image

	iCurrentStrip = 0;
	iNewStrip = ETrue;
	}

void CTiffReadCodec::NewStripL()
	{
	iNewStrip = EFalse;
	iDecoder->NewStripL(iStripInfo[iCurrentStrip].iLength);
	}

TFrameState CTiffReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	if (iNewStrip)
		NewStripL();

	if (!iDecoder->ProcessStripL(&aSrc))
		return(EFrameIncomplete);

	iCurrentStrip++;
	if (iCurrentStrip >= iTiffImageData->iNumStrips)
		return (EFrameComplete);

	iNewStrip = ETrue;
	iNewPosition = iStripInfo[iCurrentStrip].iOffset;

	return(EFrameIncompleteRepositionRequest);
	}

void CTiffReadCodec::GetNewDataPosition(TInt& aPosition, TInt& /* aLength */ )
	{
	aPosition = iNewPosition;
	}


// TTiffIfdEntry.
TInt TTiffIfdEntry::TypeSize() const
	{
	switch(iType)
		{
	case EByte:
	case ESbyte:
	case EAscii:
	case EUndefined:
		return(1);
	case EShort:
	case ESshort:
		return(2);
	case ELong:
	case ESlong:
	case EFloat:
		return(4);
	case ERational:
	case ESrational:
	case EDouble:
		return(8);
	default:
		return(0);
		}
	}

