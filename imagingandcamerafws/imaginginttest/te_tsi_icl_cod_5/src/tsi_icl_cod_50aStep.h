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
 @file tsi_icl_cod_50aStep.h
 @internalTechnology
*/
#if (!defined TSI_ICL_COD_50A_STEP_H)
#define TSI_ICL_COD_50A_STEP_H
#include <test/testexecutestepbase.h>
#include "Te_tsi_icl_cod_5SuiteStepBase.h"

class Ctsi_icl_cod_50aStep : public CTe_tsi_icl_cod_5SuiteStepBase
	{
public:
	Ctsi_icl_cod_50aStep();
	~Ctsi_icl_cod_50aStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	void DecodeFileL(TUid aDecoderUid, const TDesC& aSourceFilename, CImageDecoder::TOptions aDecoderOptions);

private:
	CFbsBitmap* iDestinationBitmap;
	CFbsBitmap* iReferenceBitmap;
	CActiveScheduler* iActiveScheduler;
	CActiveListener* iActiveListener;
	RFs iFs;
	TBool iIsScalingTest; // for ICO scaling
	TInt iReductionFactor;
	TInt iWidth;
	TInt iHeight;
	};

_LIT(Ktsi_icl_cod_50aStep,"tsi_icl_cod_50aStep");

#endif // TSI_ICL_COD_50A_STEP_H
