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

#include "recordtable.h"

//
//
//
CRecordTable::CRecordTable()
	{
	}

//
//
//
CRecordTable::~CRecordTable()
	{
	iTable.Reset();
	}

//
//
//
CRecordTable* CRecordTable::NewL()
	{
	return new(ELeave) CRecordTable();
	}

//
//
//
void CRecordTable::Reset()
	{
	iTable.Reset();
	}

//
// Inserts a record into the table making sure it does not overlap
// with any previously entered records.
//
TInt CRecordTable::InsertRecordL(TInt aRecordStart, TInt aRecordLength)
	{
	if ((aRecordStart < 0) || (aRecordLength <= 0))
		{
		User::Leave(KErrArgument);
		}

	CRecordTable::TRecord newEntry(aRecordStart, aRecordLength);
	
	for (TInt i = 0; i < iTable.Count(); i++)
		{
		const CRecordTable::TRecord& existing = iTable[i];
		
		if (newEntry.Overlaps(existing))
			{
			return KErrAlreadyExists;
			}
		}
	
	iTable.AppendL(newEntry);
	return KErrNone;
	}
