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
 @file tsi_icl_cod_58Step.h
 @internalTechnology
*/
#if (!defined TSI_ICL_COD_58_STEP_H)
#define TSI_ICL_COD_58_STEP_H
#include <test/testexecutestepbase.h>
#include "Te_tsi_icl_cod_5SuiteStepBase.h"

class Ctsi_icl_cod_58Step : public CTe_tsi_icl_cod_5SuiteStepBase,
							public MIclImageDisplayObserver
	{
public:
	Ctsi_icl_cod_58Step();
	~Ctsi_icl_cod_58Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

	// from MIclImageDisplayObserver
	void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect &aUpdatedArea, TInt aError);

private:
	TBool ValidateBitmaps(const CFbsBitmap* aDestinationBitmap, const CFbsBitmap* aDestinationMask) const;
	TBool SaveBitmaps(const CFbsBitmap* aDestinationBitmap, const CFbsBitmap* aDestinationMask);
	void StopAndResetFrameCount();
	TBool DoCompareBitmaps(const CFbsBitmap* aDestinationBitmap, const CFbsBitmap* aDestinationMask);

private:
	RArray<TDisplayMode> iDisplayModes;
	TInt iDisplayModeIndex;
	CActiveScheduler* iActiveScheduler;
	CImageDisplay* iImageDisplay;
	RFs iFs;
	TInt iFrameCount;
	HBufC* iFilename;
	TInt iTotalFramesToDecode;
	TBool iMaskPresent;
	CFbsBitmap* iReference;
	
	// for performance figures
	TInt iNanokernelTickPeriod;
	TReal iNanokernelTickPeriodHiRes;	
	TUint32 iPlayStartTime;
	TUint32 iPlayEndTime;
	TUint32 iTotalImageTime;
	};

_LIT(Ktsi_icl_cod_58Step,"tsi_icl_cod_58Step");

#endif // TSI_ICL_COD_58_STEP_H
