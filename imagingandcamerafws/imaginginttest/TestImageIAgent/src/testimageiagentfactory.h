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

#ifndef TESTIMAGEIAGENTFACTORY_H
#define TESTIMAGEIAGENTFACTORY_H

#include <caf/caf.h>
#include <caf/agentfactory.h>
#include "testimageiagentdata.h"
#include "testimageiagentcontent.h"
#include "testimageiagentmanager.h"

using namespace ContentAccess;

class CTestIMAGEIAgentFactory : public CAgentFactory
	{
public:
	static CAgentFactory* NewL();
	virtual ~CTestIMAGEIAgentFactory();
	virtual CAgentContent* CreateContentBrowserL(const TDesC& aURI, TContentShareMode aShareMode);
	virtual CAgentContent* CreateContentBrowserL(RFile& aFile);
	virtual CAgentData* CreateDataConsumerL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode);
	virtual CAgentData* CreateDataConsumerL(RFile& aFile, const TDesC& aUniqueId);
	virtual CAgentManager*  CreateManagerL();			
	virtual CAgentImportFile* CreateImporterL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName);
	virtual CAgentImportFile* CreateImporterL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray);
	virtual CAgentRightsManager* CreateRightsManagerL();
	};

#endif // TESTIMAGEIAGENTFACTORY_H
