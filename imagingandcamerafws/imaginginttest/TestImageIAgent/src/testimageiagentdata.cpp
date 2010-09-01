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

#include <caf/attribute.h>
#include "testimageiagentdata.h"

using namespace ContentAccess;

CTestIMAGEIAgentData* CTestIMAGEIAgentData::NewL(const TDesC& aURI, TContentShareMode aMode)
	{
	CTestIMAGEIAgentData* self = NewLC(aURI, aMode);
	CleanupStack::Pop(self);
	return self;
	}

CTestIMAGEIAgentData* CTestIMAGEIAgentData::NewLC(const TDesC& aURI, TContentShareMode aMode)
	{
	CTestIMAGEIAgentData* self=new(ELeave) CTestIMAGEIAgentData();
	CleanupStack::PushL(self);
	self->ConstructL(aURI, aMode);
	return self;
	}

CTestIMAGEIAgentData::CTestIMAGEIAgentData()
	{
	}

CTestIMAGEIAgentData::~CTestIMAGEIAgentData()
	{
	// Tidy up RFile and RFs
	iFile.Close();
	iFs.Close();
	}



void CTestIMAGEIAgentData::ConstructL(const TDesC& aURI, TContentShareMode aMode)
	{
	User::LeaveIfError(iFs.Connect());
	// Make the file session shareable
	User::LeaveIfError(iFs.ShareAuto());
	User::LeaveIfError(iFile.Open(iFs, aURI, aMode));
	}


void CTestIMAGEIAgentData::GetAttributeL(const CBitset& /*aQuerySet*/,
									        CBitset& aResponseSet)
	{
	// Test attributes
	aResponseSet.Set(EIsProtected);
	aResponseSet.Unset(EIsForwardable);
	aResponseSet.Unset(EIsModifyable);
	aResponseSet.Set(EIsCopyable);
	}

void CTestIMAGEIAgentData::DataSizeL(TInt &aSize)
	{
	User::LeaveIfError(iFile.Size(aSize));
	}

TInt CTestIMAGEIAgentData::EvaluateIntent(TIntent /*aIntent*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentData::ExecuteIntent(TIntent /*aIntent*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentData::Read(TDes8& aDes)
	{
	TInt err = iFile.Read(aDes);
	
	if(err == KErrNone)
		{
		InvertDes(aDes);
		}
	
	return err;
	}

void CTestIMAGEIAgentData::InvertDes(TDes8& aDes)
	{
	TInt8 byteBuf;

	for(TInt index=0; index<aDes.Length(); index++)
		{
		byteBuf = aDes[index];
		byteBuf ^= 0xFF; //inverting the buf bits
		aDes[index] = byteBuf;
		}		
	}

TInt CTestIMAGEIAgentData::Read(TDes8& aDes,TInt aLength)
	{
	TInt err = iFile.Read(aDes,aLength);
	
	if(err == KErrNone)
		{
		InvertDes(aDes);
		}
	
	return err;
	}

void CTestIMAGEIAgentData::Read(TDes8& aDes,TRequestStatus& aStatus)
	{
	iFile.Read(aDes, aStatus);
	InvertDes(aDes);
	}

void CTestIMAGEIAgentData::Read(TDes8& aDes,
							 TInt aLength,
							 TRequestStatus& aStatus)
	{
	iFile.Read(aDes, aLength, aStatus);
	InvertDes(aDes);
	}

TInt CTestIMAGEIAgentData::Seek(TSeek aMode, TInt& aPos) 
	{
	return iFile.Seek(aMode, aPos);
	}

TInt CTestIMAGEIAgentData::SetProperty(TAgentProperty /*aProperty*/, TInt /*aValue*/)
	{
	return KErrCANotSupported;
	}

TInt CTestIMAGEIAgentData::GetAttribute(TInt /*aAttribute*/, TInt& /*aValue*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentData::GetAttributeSet(RAttributeSet& /*aAttributeSet*/)
	{
	return KErrNone;			
	}

TInt CTestIMAGEIAgentData::GetStringAttribute(TInt /*aAttribute*/, TDes& /*aValue*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentData::GetStringAttributeSet(RStringAttributeSet& /*aStringAttributeSet*/)
	{
	return KErrNone;
	}
