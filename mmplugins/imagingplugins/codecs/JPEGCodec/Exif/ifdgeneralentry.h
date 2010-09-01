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

#ifndef IFDGENERALENTRY_H
#define IFDGENERALENTRY_H

#include <e32base.h>

const TUint8 KNullCharacter = '\0';

class CIfdGeneral;

class CIfdGeneralEntry : public CBase
	{
public:
	friend class CExifVerifier;	

public:
	static CIfdGeneralEntry* NewLC();
	static CIfdGeneralEntry* NewLC(const TUint& aTag, const TUint& aFormat, const TUint& aComponentCount, const TUint8* aParam, CIfdGeneral* aIfd);
	static CIfdGeneralEntry* NewL(const TUint& aTag, const TUint& aFormat, const TUint& aComponentCount, const TUint8* aParam, CIfdGeneral* aIfd, TBool& aUnknownTag);

	~CIfdGeneralEntry();

	TUint Tag();
	TUint Format();
	TUint ComponentCount();
	TUint ValueOffset();
	const TUint8* ActualValue(); 
	
	// Accessor functions.
	

	// Returns the number of additional data bytes stored for an IfdEntry (outside of the 12 byte directory entry)
	TInt ExtraSize();
	// returns the total size of the ifd entry
	TInt TotalSize();		

	// Alter fields.
	TInt UpdateRational(const TInt aNumerator, const TInt aDenominator);
	TInt UpdateString(const HBufC8* aStringValue);
	TInt UpdateInteger(const TInt aIntegerValue);
	TInt UpdateShort(const TUint16 aShortValue);
	TInt UpdateData(TUint aComponentCount, const TUint8* aData); // A catch-all update function.
	// Check whether this entry has data of a particular type.
	TBool IsRationalFormat();
	TBool IsStringFormat();
	TBool IsUndefinedFormat();
	TBool IsIntegerFormat();
	TBool IsShortFormat();
	TBool IsUnicodeData();

	// Set fields in a new directory entry.
	TInt SetActualData(const TAny* aActualData, const TUint aSize, const TUint aBytes);

private: // Functions
	void SetTag(const TUint aTag);
	void SetFormat(const TUint aFormat);
	void SetComponentCount(const TUint aCompCount);
	void SetValueOffset(const TUint8* aValueOffset);

	CIfdGeneralEntry();
	void ConstructL();
	void ConstructL(const TUint& aTag, const TUint& aFormat, TUint aComponentCount, const TUint8* aValueOffset, CIfdGeneral* aIfd, TBool& aUnknownTag);
	
private: // Member variables.
	TUint8* iDirEntryData; // The actual data in a buffer pointed to by this.
	TUint8* iActualData; // Stores n bytes of data pointed to by value offset field.
	CIfdGeneral* iIfd; // not owned
	};

#endif // IFDGENERALENTRY_H
