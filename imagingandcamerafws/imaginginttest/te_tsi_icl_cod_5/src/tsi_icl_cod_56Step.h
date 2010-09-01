/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file tsi_icl_cod_56Step.h
 @internalTechnology
*/
#if (!defined TSI_ICL_COD_56_STEP_H)
#define TSI_ICL_COD_56_STEP_H
#include <test/testexecutestepbase.h>
#include "Te_tsi_icl_cod_5SuiteStepBase.h"

class Ctsi_icl_cod_56Step : public CTe_tsi_icl_cod_5SuiteStepBase
	{
public:
	Ctsi_icl_cod_56Step();
	~Ctsi_icl_cod_56Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CActiveScheduler* iActiveScheduler;
	CActiveListener* iActiveListener;
	CFbsBitmap* iSourceBitmap;
	RFs iFs;
	};

_LIT(Ktsi_icl_cod_56Step,"tsi_icl_cod_56Step");

#endif // TSI_ICL_COD_56_STEP_H
