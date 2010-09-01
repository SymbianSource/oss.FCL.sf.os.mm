// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "JpegTypes.h"

/**
	@file
	@internalComponent
*/

// THuffmanTable
THuffmanTable::THuffmanTable():
	iLastCode(0),
	iState(EUnInitialised)
	{
	// current implementation doesn't support more than 8 bit
	ASSERT(KJpgHuffmanLookAhead <= sizeof(TUint8)*8 );
	}

TInt THuffmanTable::SetL(const TUint8* aData, const TUint8* aDataLimit)
	{
	//We assume that a complete table is passed
	//If there is not enougth data the table is corrupt
	if(aData+KJpgHuffmanTableBitsSize > aDataLimit)
		{
		User::Leave(KErrCorrupt);
		}
	
	Mem::Copy(iBits,aData,KJpgHuffmanTableBitsSize);
	aData += KJpgHuffmanTableBitsSize;

	TInt valuesSize = 0;
	for (TInt count = 0; count < KJpgHuffmanTableBitsSize; count++)
		{
		valuesSize += iBits[count];
		}

	if((valuesSize <= 0) || (valuesSize > KJpgHuffmanTableSize) || (aData+valuesSize > aDataLimit)) 
		{
		User::Leave(KErrCorrupt);
		}

	Mem::Copy(iValues,aData,valuesSize);
	Mem::FillZ(iValues + valuesSize,KJpgHuffmanTableSize - valuesSize);

	SetState(EInitialised);

	return KJpgHuffmanTableBitsSize + valuesSize;
	}

TInt THuffmanTable::GetL(TUint8* aData)
	{
	Mem::Copy(aData,iBits,KJpgHuffmanTableBitsSize);
	aData += KJpgHuffmanTableBitsSize;

	TInt valuesSize = 0;
	for (TInt count = 0; count < KJpgHuffmanTableBitsSize; count++)
		{
		valuesSize += iBits[count];
		}

	if ((valuesSize <= 0) || (valuesSize > KJpgHuffmanTableSize)) 
		{
		User::Leave(KErrCorrupt);
		}

	Mem::Copy(aData,iValues,valuesSize);

	return KJpgHuffmanTableBitsSize + valuesSize;
	}

void THuffmanTable::GenerateSizes(THuffmanCode* aCode)
	{
	TInt i = 0;
	TInt j = 1;
	TInt k = 0;

	while (i < KJpgHuffmanTableBitsSize)
		{
		if (j > iBits[i])
			{
			i++;
			j = 1;
			}
		else
			{
			if (k == KJpgHuffmanTableSize - 1 )
			    {
			    break; // to prevent table overrun by a malicious image
			    }
			aCode[k].iSize = i + 1;
			k++;
			j++;
			}
		}

	aCode[k].iSize = 0;
	iLastCode = k - 1;
	}

void THuffmanTable::GenerateCodes(THuffmanCode* aCode)
	{
	TInt k = 0;
	TUint16 code = 0;
	TInt si = aCode[0].iSize;

	FOREVER
		{
		do	{
			aCode[k].iCode = code;
			aCode[k].iValue = iValues[k];
			code++;
			k++;
			}
		while (aCode[k].iSize == si);

		if (aCode[k].iSize == 0 || k > iLastCode)
		    {
		    break;
		    }

		do	{
			code <<= 1;
			si++;
			}
		while (aCode[k].iSize != si || si > KJpgHuffmanTableBitsSize);
		}
	}
/**
	Implemetation of TKey parameter class
	for use in User::QuickSort. 
	Sort by "iValue" member of the THuffmanTable::THuffmanCode
*/	
class THuffmanValueKey : public TKey
	{
public:
	explicit THuffmanValueKey(THuffmanTable::THuffmanCode* aArray);
	TInt Compare(TInt aLeft, TInt aRight) const;
	TAny *At(TInt anIndex) const;
protected:
	THuffmanTable::THuffmanCode* iArray;
	};	

inline
THuffmanValueKey::THuffmanValueKey(THuffmanTable::THuffmanCode* aArray)
	{
	iArray = aArray;
	}

TInt THuffmanValueKey::Compare(TInt aLeft, TInt aRight) const
	{
	return iArray[aLeft].iValue - iArray[aRight].iValue;
	}
	
TAny* THuffmanValueKey::At(TInt anIndex) const
	{
	return iArray + anIndex;
	}

/**
	Implemetation of TKey parameter class
	for use in User::QuickSort. 
	Sort by "iIndex" member of the THuffmanTable::THuffmanCode
*/
class THuffmanIndexKey : public TKey
	{
public:
	explicit THuffmanIndexKey(THuffmanTable::THuffmanCode* aArray);
	TInt Compare(TInt aLeft, TInt aRight) const;
	TAny *At(TInt anIndex) const;
protected:
	THuffmanTable::THuffmanCode* iArray;
	};
	
inline
THuffmanIndexKey::THuffmanIndexKey(THuffmanTable::THuffmanCode* aArray)
	{
	iArray = aArray;
	}

TInt THuffmanIndexKey::Compare(TInt aLeft, TInt aRight) const
	{
	return iArray[aLeft].iIndex - iArray[aRight].iIndex;
	}
	
TAny* THuffmanIndexKey::At(TInt anIndex) const
	{
	return iArray + anIndex;
	}

/**
	Implemetation of TSwap parameter class
	for use in User::QuickSort. 
	Swaps two THuffmanTable::THuffmanCode variables
*/
class THuffmanCodeSwap : public TSwap
	{
public:
	explicit THuffmanCodeSwap(THuffmanTable::THuffmanCode* aArray);
	void Swap(TInt aLeft, TInt aRight) const;
protected:
	THuffmanTable::THuffmanCode* iArray;
	};
	
inline
THuffmanCodeSwap::THuffmanCodeSwap(THuffmanTable::THuffmanCode* aArray)
	{
	iArray = aArray;
	}
	
void THuffmanCodeSwap::Swap(TInt aLeft, TInt aRight) const
	{
	THuffmanTable::THuffmanCode tmp=iArray[aLeft];
	iArray[aLeft] 	= iArray[aRight];
	iArray[aRight] 	= tmp;
	}

void THuffmanTable::SortByCode(THuffmanTable::THuffmanCode* aCode, TUint8* aBitsToCodeIdxHash)
	{
	for (TInt count = 0; count <= iLastCode; count++)
		{
		THuffmanCode& codeEntry = aCode[count];
		aCode[count].iIndex = (1 << codeEntry.iSize) - 1 + codeEntry.iCode;
		}
		
	THuffmanIndexKey key(aCode);
	THuffmanCodeSwap swap(aCode);
	User::QuickSort(iLastCode+1, key, swap);

	aBitsToCodeIdxHash[0]=0;
	TInt sz=0;
	for (TInt j=0; j <= iLastCode; ++j)
		{
		if (aCode[j].iSize > sz)
			{
			TInt k=sz+1;
			sz = aCode[j].iSize;
			for (; k<=sz; ++k)
				{
				aBitsToCodeIdxHash[k]=j;
				}
			}
		}
	aBitsToCodeIdxHash[sz+1]=iLastCode;
	}

class THuffmanValueSwap : public TSwap
	{
public:
	explicit THuffmanValueSwap(THuffmanTable::THuffmanCode* aArray);
	void Swap(TInt aLeft, TInt aRight) const;
protected:
	THuffmanTable::THuffmanCode* iArray;
	};
	
inline
THuffmanValueSwap::THuffmanValueSwap(THuffmanTable::THuffmanCode* aArray)
	{
	iArray = aArray;
	}
	
void THuffmanValueSwap::Swap(TInt aLeft, TInt aRight) const
	{
	THuffmanTable::THuffmanCode tmp=iArray[aLeft];
	iArray[aLeft] 	= iArray[aRight];
	iArray[aRight] 	= tmp;
	}


void THuffmanTable::SortByValue(THuffmanTable::THuffmanCode* aArray)
	{
	THuffmanValueKey key(aArray);
	THuffmanValueSwap swap(aArray);
	User::QuickSort(iLastCode+1, key, swap);
	}

void TDecHuffmanTable::MakeDerivedTableL()
	{
	if(State()!=EInitialised)
		{
		return;
		}

	THuffmanCode* codes = (THuffmanCode*)User::AllocZ(KJpgHuffmanTableSize*sizeof(THuffmanCode));
	if (codes == NULL) 
		{
		User::Leave(KErrNoMemory);
		}

	CleanupStack::PushL(codes);

	Mem::FillZ(iLookupTable, KJpgHuffmanTableSize * sizeof(TUint16));
	Mem::FillZ(iValue, KJpgHuffmanTableSize * sizeof(TUint16));
	Mem::FillZ(iIndex, KJpgHuffmanTableSize * sizeof(TInt));
	
	GenerateSizes(codes);
	
	if (iLastCode < 0 || iLastCode >= KJpgHuffmanTableSize) 
		{
		User::Leave(KErrCorrupt);	
		}
	
	GenerateCodes(codes);
			
	TInt p = 0;
    for (TUint l = 1; l <= KJpgHuffmanLookAhead; l++)
    	{
    	for (TUint i = 1; i <= iBits[l-1]; i++, p++)
    		{
    		// l = current code's length, p = its index in huffcode[] & huffval[].
    		// Generate left-justified code followed by all possible bit sequences
    		TInt lookbits = codes[p].iCode << (KJpgHuffmanLookAhead-l);
    		for (TInt ctr = 1 << (KJpgHuffmanLookAhead-l); ctr > 0; ctr--)
    				{
    				iLookupTable[lookbits] = ((l << 8) | iValues[p]);
    				lookbits++;
    				}
    		}
		}

	SortByCode(codes, iBitsToCodeIdxHash);

	for (TInt i = 0; i < iLastCode+1; i++) 
		{
		iValue[i] = codes[i].iValue;
		iIndex[i] = codes[i].iIndex;
		}
	
	CleanupStack::PopAndDestroy();
	SetState(EParsed);
	}

template <TInt aTableSize> void TEncHuffmanTable<aTableSize>::MakeDerivedTableL()
	{
	if(State()!=EInitialised)
		{
		return;
		}

	THuffmanCode* codes = (THuffmanCode*)User::AllocZ(KJpgHuffmanTableSize*sizeof(THuffmanCode));
	if (codes == NULL) 
		{
		User::Leave(KErrNoMemory);
		}
	
	CleanupStack::PushL(codes);
	
	Mem::FillZ(codes, KJpgHuffmanTableSize * sizeof(THuffmanCode));
	Mem::FillZ(iLookupTable, aTableSize * sizeof(TUint32));
	
	GenerateSizes(codes);

	if (iLastCode < 0 || iLastCode >= KJpgHuffmanTableSize) 
		{
		User::Leave(KErrCorrupt);	
		}
	
	GenerateCodes(codes);
	SortByValue(codes);
	
	for (TInt i=iLastCode; i; --i)
		{
		ASSERT( codes[i].iValue <= aTableSize );
		codes[codes[i].iValue] = codes[i];
		}

	for (TInt i=0; i<=iLastCode; i++)
		{
		TInt value = iValues[i];
		iLookupTable[value] = (codes[value].iSize | (codes[value].iCode << 16));
		}

	CleanupStack::PopAndDestroy();
	SetState(EParsed);
	}

const TInt KEncDCHTSize = 11;// the defines from jpegwritecodec.h 
const TInt KEncACHTSize = 0xFA;

template class TEncHuffmanTable<KEncDCHTSize+1>;
template class TEncHuffmanTable<KEncACHTSize+1>; //templates for jpegwritecodec

void THuffmanTableProcessor::ProcessHuffmanTableL(const TUint8*& aDataPtr, const TUint8* aDataPtrLimit, TDecHuffmanTable* aDCTable, TDecHuffmanTable* aACTable)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		TInt index = *aDataPtr++;
		TBool dcTable = !(index & 0x10);
		index &= 0x0f;
		if (index >= KJpgMaxNumberOfTables)
			User::Leave(KErrCorrupt);
		TDecHuffmanTable& table = dcTable ? aDCTable[index] : aACTable[index];
		aDataPtr += table.SetL(aDataPtr, aDataPtrLimit);
		}
	//The block length and actual data did not match 
	if(aDataPtr != aDataPtrLimit)
		User::Leave(KErrCorrupt);	
	}

/**
 Before calling this function, the Huffman tables should be set.
 */
void TJpgScanInfoProcessor::ProcessStartOfScanL(const TUint8*& aPtr, const TJpgFrameInfo& aJpgFrameInfo, TJpgScanInfo& aScanInfo, TDecHuffmanTable* aDCTable, TDecHuffmanTable* aACTable)
	{
	aScanInfo.iNumberOfComponents = *aPtr++;

	//At least one component and not more than specified in the start of frame header
	if(aScanInfo.iNumberOfComponents < KJpgMinNumberOfComponents || aScanInfo.iNumberOfComponents > aJpgFrameInfo.iNumberOfComponents)
		User::Leave(KErrCorrupt);

	for (TInt count = 0; count < aScanInfo.iNumberOfComponents; count++)
		{
		aScanInfo.iComponent[count].iId = *aPtr++;

		TUint8 table = *aPtr++;
		TUint8 DCTable = static_cast<TUint8>(table >> 4);
		TUint8 ACTable = static_cast<TUint8>(table & 0x0f);

		if(DCTable > KJpgMaxNumberOfTables || ACTable > KJpgMaxNumberOfTables)
			User::Leave(KErrCorrupt);

		aScanInfo.iComponent[count].iDCCodingTable = DCTable;
		aScanInfo.iComponent[count].iACCodingTable = ACTable;

		aDCTable[DCTable].MakeDerivedTableL();
		aACTable[ACTable].MakeDerivedTableL();
		}

	aScanInfo.iStartSpectralSelection = *aPtr++;
	aScanInfo.iEndSpectralSelection = *aPtr++;
	TUint8 successiveApproximation = *aPtr;
	aScanInfo.iSuccessiveApproximationBitsHigh = successiveApproximation >> 4;
	aScanInfo.iSuccessiveApproximationBitsLow = successiveApproximation & 0x0f;
	}
