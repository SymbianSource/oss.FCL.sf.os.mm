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

#ifndef TSI_ICL_PLUGINSUPPORT_011_STEP_H
#define TSI_ICL_PLUGINSUPPORT_011_STEP_H

#include <test/testexecuteserverbase.h>
#include "te_tsi_icl_pluginsupport_01suitestepbase.h"

class Ctsi_icl_pluginsupport_011Step : public CTe_tsi_icl_pluginsupport_01SuiteStepBase
	{
public:
	Ctsi_icl_pluginsupport_011Step();
	~Ctsi_icl_pluginsupport_011Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CActiveScheduler* iActiveScheduler;
	RFs iFs;
	};

_LIT(Ktsi_icl_pluginsupport_011step,"tsi_icl_pluginsupport_011Step");

#endif // TSI_ICL_PLUGINSUPPORT_011_STEP_H
