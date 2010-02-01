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


#ifndef TE_TSI_ICL_PLUGINSUPPORT_01_SERVER_H
#define TE_TSI_ICL_PLUGINSUPPORT_01_SERVER_H
#include <test/testexecuteserverbase.h>

class CTe_tsi_icl_pluginsupport_01Suite : public CTestServer
	{
public:
	static CTe_tsi_icl_pluginsupport_01Suite* NewL();
	// Base class pure virtual override
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

#endif // TE_TSI_ICL_PLUGINSUPPORT_01_SERVER_H
