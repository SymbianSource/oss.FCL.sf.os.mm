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

#ifndef TESTIMAGEIAGENTDATA_H
#define TESTIMAGEIAGENTDATA_H

#include <caf/caf.h>
#include <caf/agentinterface.h>
#include <caf/bitset.h>

using namespace ContentAccess;

class CTestIMAGEIAgentData : public CAgentData
	{
public:
	static CTestIMAGEIAgentData* NewL(const TDesC& aURI, TContentShareMode aMode);
	static CTestIMAGEIAgentData* NewLC(const TDesC& aURI, TContentShareMode aMode);
	~CTestIMAGEIAgentData();		

	// From CAgentData
	virtual void GetAttributeL(const CBitset &aQuerySet,
							   CBitset &aResponseSet);
	virtual void DataSizeL(TInt &aSize);
	virtual TInt EvaluateIntent(TIntent aIntent);
	virtual TInt ExecuteIntent(TIntent aIntent);		
	virtual TInt Read(TDes8& aDes);
	virtual TInt Read(TDes8& aDes, TInt aLength);
	virtual void Read(TDes8& aDes, TRequestStatus& aStatus);
	virtual void Read(TDes8& aDes,
					  TInt aLength,
					  TRequestStatus& aStatus);
	virtual TInt Seek(TSeek aMode, TInt& aPos);
	virtual TInt SetProperty(TAgentProperty aProperty, TInt aValue);
	virtual TInt GetAttribute(TInt aAttribute, TInt& aValue);
	virtual TInt GetAttributeSet(RAttributeSet& aAttributeSet);
	virtual TInt GetStringAttribute(TInt aAttribute, TDes& aValue);
	virtual TInt GetStringAttributeSet(RStringAttributeSet& aStringAttributeSet);						
private:
	CTestIMAGEIAgentData();
	void ConstructL(const TDesC& aURI, TContentShareMode aMode);
	void InvertDes(TDes8& aDes);
	
private:
	/* Handle to the filesystem */
	RFs iFs;
	
	/* RFile object */
	RFile iFile;
	};

#endif // TESTIMAGEIAGENTDATA_H
