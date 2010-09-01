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

#include "testimageiagentfactory.h"

using namespace ContentAccess;

CAgentFactory* CTestIMAGEIAgentFactory::NewL()
	{
	return new (ELeave) CTestIMAGEIAgentFactory();
	}
	
CTestIMAGEIAgentFactory::~CTestIMAGEIAgentFactory()
	{
	}
	
CAgentContent* CTestIMAGEIAgentFactory::CreateContentBrowserL(const TDesC& aURI, TContentShareMode aShareMode)
	{
	return CTestIMAGEIAgentContent::NewL(aURI, aShareMode);	
	}
	
CAgentContent* CTestIMAGEIAgentFactory::CreateContentBrowserL(RFile& /*aFile*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;	
	}
	
CAgentData* CTestIMAGEIAgentFactory::CreateDataConsumerL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode)
	{
	return CTestIMAGEIAgentData::NewL(aVirtualPath.URI(), aShareMode);	
	}
	
CAgentData* CTestIMAGEIAgentFactory::CreateDataConsumerL(RFile& /*aFile*/, const TDesC& /*aUniqueId*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;	
	}
	
CAgentManager*  CTestIMAGEIAgentFactory::CTestIMAGEIAgentFactory::CreateManagerL()
	{
	return CTestIMAGEIAgentManager::NewL();
	}
	
CAgentImportFile* CTestIMAGEIAgentFactory::CreateImporterL(const TDesC8& /*aMimeType*/, const CMetaDataArray& /*aMetaDataArray*/, const TDesC& /*aOutputDirectory*/, const TDesC& /*aSuggestedFileName*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;	
	}
	
CAgentImportFile* CTestIMAGEIAgentFactory::CreateImporterL(const TDesC8& /*aMimeType*/, const CMetaDataArray& /*aMetaDataArray*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;	
	}
	
CAgentRightsManager* CTestIMAGEIAgentFactory::CreateRightsManagerL()
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
	
