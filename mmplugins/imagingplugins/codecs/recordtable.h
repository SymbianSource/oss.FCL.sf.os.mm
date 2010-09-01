// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef ICL_RECORDTABLE_H
#define ICL_RECORDTABLE_H

#include <e32base.h>

/**
This class is used for keeping track of the location and size of structures
within container file formats to prevent infinite looping on corrupted files.

@internalComponent
*/
class CRecordTable : public CBase
	{
public:
	static CRecordTable* NewL();
	~CRecordTable();
	TInt InsertRecordL(TInt aRecordStart, TInt aRecordLength);
	void Reset();
	TInt Count()
		{
		return iTable.Count();
		}
	
protected:
	CRecordTable();
	
public:
	
	class TRecord
		{
	public:
		TInt iOffset;	// from start of file.
		TInt iLength;	// in bytes.
		
	public:
		TRecord(TInt aOffset, TInt aLength)
		 :	iOffset(aOffset),
		 	iLength(aLength)
			{
			}
		
		TBool Overlaps(const TRecord& aOther)
			{
			if (aOther.iOffset == iOffset)
				{
				return ETrue;
				}
	
			if (aOther.iOffset < iOffset)
				{
				return ((aOther.iOffset + aOther.iLength) >= iOffset);
				}

			return ((iOffset + iLength) > aOther.iOffset);
			}
		};
		
private:
	RArray<TRecord> iTable;
	};

#endif
