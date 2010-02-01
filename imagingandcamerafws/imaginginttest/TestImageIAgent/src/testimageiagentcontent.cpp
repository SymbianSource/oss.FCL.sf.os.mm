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
#include "testimageiagentcontent.h"
#include <caf/bitset.h>

using namespace ContentAccess;

CTestIMAGEIAgentContent* CTestIMAGEIAgentContent::NewL(const TDesC& aURI, TContentShareMode aMode)
	{
	CTestIMAGEIAgentContent* self = NewLC(aURI, aMode);
	CleanupStack::Pop(self);
	return self;
	}

CTestIMAGEIAgentContent* CTestIMAGEIAgentContent::NewLC(const TDesC& aURI, TContentShareMode aMode)
	{
	CTestIMAGEIAgentContent* self=new(ELeave) CTestIMAGEIAgentContent();
	CleanupStack::PushL(self);
	self->ConstructL(aURI, aMode);
	return self;
	}

CTestIMAGEIAgentContent::CTestIMAGEIAgentContent()
	{
	}

CTestIMAGEIAgentContent::~CTestIMAGEIAgentContent()
	{
	}

void CTestIMAGEIAgentContent::ConstructL(const TDesC& /*aURI*/, TContentShareMode /*aMode*/)
	{
	}

TInt CTestIMAGEIAgentContent::OpenContainer(const TDesC& /*aUniqueId*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentContent::CloseContainer()
	{
	return KErrNone;
	}

void CTestIMAGEIAgentContent::GetEmbeddedObjectsL(RStreamablePtrArray<CEmbeddedObject>& /*aArray*/)
	{
	}

void CTestIMAGEIAgentContent::GetEmbeddedObjectsL(RStreamablePtrArray<CEmbeddedObject>& /*aArray*/, TEmbeddedType /*aType*/)
	{
	}

TInt CTestIMAGEIAgentContent::Search(RStreamablePtrArray<CEmbeddedObject>& /*aArray*/, const TDesC8& /*aMimeType*/, TBool /*aRecursive*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentContent::GetAttribute(TInt /*aAttribute*/, TInt& /*aValue*/, const TDesC& /*aUniqueId*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentContent::GetAttributeSet(RAttributeSet& aAttributeSet, const TDesC& /*aUniqueId*/)
	{
	TInt err = KErrNone;
	aAttributeSet.SetValue(EIsProtected, 1, err);
	aAttributeSet.SetValue(EIsForwardable, 0, err);
	aAttributeSet.SetValue(EIsModifyable, 0, err);
	aAttributeSet.SetValue(EIsCopyable, 1, err);
	return err;
	}

TInt CTestIMAGEIAgentContent::GetStringAttribute(TInt /*aAttribute*/, TDes& /*aValue*/, const TDesC& /*aUniqueId*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentContent::GetStringAttributeSet(RStringAttributeSet& /*aStringAttributeSet*/, const TDesC& /*aUniqueId*/)
	{
	return KErrNone;
	}

TInt CTestIMAGEIAgentContent::AgentSpecificCommand(TInt /*aCommand*/, const TDesC8& /*aInputBuffer*/, TDes8& /*aOutputBuffer*/)
	{
	return KErrNone;
	}

void CTestIMAGEIAgentContent::AgentSpecificCommand(TInt /*aCommand*/, const TDesC8& /*aInputBuffer*/, TDes8& /*aOutputBuffer*/, TRequestStatus& /*aStatus*/)
	{
	}

void CTestIMAGEIAgentContent::NotifyStatusChange(TEventMask /*aMask*/, TRequestStatus& /*aStatus*/, const TDesC& /*aUniqueId*/)
	{
	}

TInt CTestIMAGEIAgentContent::CancelNotifyStatusChange(TRequestStatus& /*aStatus*/, const TDesC& /*aUniqueId*/)
	{
	return KErrNone;
	}

void CTestIMAGEIAgentContent::RequestRights(TRequestStatus& /*aStatus*/, const TDesC& /*aUniqueId*/)
	{
	}

TInt CTestIMAGEIAgentContent::CancelRequestRights(TRequestStatus& /*aStatus*/, const TDesC& /*aUniqueId*/)
	{
	return KErrNone;
	}

void CTestIMAGEIAgentContent::DisplayInfoL(TDisplayInfo /*aInfo*/, const TDesC& /*aUniqueId*/)
	{
	}

TInt CTestIMAGEIAgentContent::SetProperty(TAgentProperty /*aProperty*/, TInt /*aValue*/)
	{
	return KErrNone;
	}
