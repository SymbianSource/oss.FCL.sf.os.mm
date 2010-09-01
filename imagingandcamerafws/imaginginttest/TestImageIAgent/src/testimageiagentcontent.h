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

#ifndef TESTIMAGEIAGENTCONTENT_H
#define TESTIMAGEIAGENTCONTENT_H

#include <caf/caf.h>
#include <caf/agentinterface.h>

using namespace ContentAccess;

class CTestIMAGEIAgentContent : public CAgentContent
	{
public:
	static CTestIMAGEIAgentContent* NewL(const TDesC& aURI, TContentShareMode aMode);
	static CTestIMAGEIAgentContent* NewLC(const TDesC& aURI, TContentShareMode aMode);
	~CTestIMAGEIAgentContent();

	// From CAgentContent
	virtual TInt OpenContainer(const TDesC& aUniqueId);
	virtual TInt CloseContainer();
	virtual void GetEmbeddedObjectsL(RStreamablePtrArray<CEmbeddedObject>& aArray);
	virtual void GetEmbeddedObjectsL(RStreamablePtrArray<CEmbeddedObject>& aArray, TEmbeddedType aType);
	virtual TInt Search(RStreamablePtrArray<CEmbeddedObject>& aArray, const TDesC8& aMimeType, TBool aRecursive);
	virtual TInt GetAttribute(TInt aAttribute, TInt& aValue, const TDesC& aUniqueId);
	virtual TInt GetAttributeSet(RAttributeSet& aAttributeSet, const TDesC& aUniqueId);		
	virtual TInt GetStringAttribute(TInt aAttribute, TDes& aValue, const TDesC& aUniqueId);		
	virtual TInt GetStringAttributeSet(RStringAttributeSet& aStringAttributeSet, const TDesC& aUniqueId);		
	virtual TInt AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer);
	virtual void AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer, TRequestStatus& aStatus);
	virtual void NotifyStatusChange(TEventMask aMask, TRequestStatus& aStatus, const TDesC& aUniqueId);
	virtual TInt CancelNotifyStatusChange(TRequestStatus& aStatus, const TDesC& aUniqueId);		
	virtual void RequestRights(TRequestStatus& aStatus, const TDesC& aUniqueId); 
	virtual TInt CancelRequestRights(TRequestStatus& aStatus, const TDesC& aUniqueId);
	virtual void DisplayInfoL(TDisplayInfo aInfo, const TDesC& aUniqueId);
	virtual TInt SetProperty(TAgentProperty aProperty, TInt aValue);										

private:
	CTestIMAGEIAgentContent();
	void ConstructL(const TDesC& aURI, TContentShareMode aMode);
		
	};

#endif // TESTIMAGEIAGENTCONTENT_H
