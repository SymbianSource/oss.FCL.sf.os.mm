/**
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file tsi_icl_cod_52Step.h
 @internalTechnology
*/
#if (!defined TSI_ICL_COD_59_STEP_H)
#define TSI_ICL_COD_59_STEP_H
#include <test/testexecutestepbase.h>
#include "Te_tsi_icl_cod_5SuiteStepBase.h"

const TRect KRefFrameInfo[] = {TRect(0, 0, 360, 240),TRect(0, 0, 360, 240),
								TRect(3, 60, 356, 191),TRect(73, 13, 108, 50),
								TRect(178, 15, 210, 49),TRect(56, 99, 213, 143),
								TRect(122, 20, 271, 141),TRect(54, 63, 275, 198),
								TRect(89, 24, 305, 197),TRect(53, 38, 308, 228)};

class Ctsi_icl_cod_59Step : public CTe_tsi_icl_cod_5SuiteStepBase
	{
public:
	Ctsi_icl_cod_59Step();
	~Ctsi_icl_cod_59Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CActiveScheduler* iActiveScheduler;
	CActiveListener* iActiveListener;
	CFbsBitmap* iDestBitmap;
	CFbsBitmap* iReference;
	RFs iFs;
	};

_LIT(Ktsi_icl_cod_59Step,"tsi_icl_cod_59Step");

#endif // TSI_ICL_COD_59_STEP_H
