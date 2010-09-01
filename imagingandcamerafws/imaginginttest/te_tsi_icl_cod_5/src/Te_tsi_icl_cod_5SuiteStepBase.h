/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file Te_tsi_icl_cod_5SuiteStepBase.h
 @internalTechnology
*/

#if (!defined TE_TSI_ICL_COD_5_STEP_BASE)
#define TE_TSI_ICL_COD_5_STEP_BASE
#include <test/testexecutestepbase.h>
#include <icl/icl_uids.hrh>
#include <imageconversion.h>
#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>
#include <mm/mmcaf.h>
#include <f32file.h>
#include <gdi.h>
#include <fbs.h>

const TUint KImagesMatch = 0;

class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

/****************************************************************************
* The reason to have a new step base is that it is very much possible
* that the all individual test steps have project related common variables 
* and members 
* and this is the place to define these common variable and members.
* 
****************************************************************************/
class CTe_tsi_icl_cod_5SuiteStepBase : public CTestStep
	{
public:
	virtual ~CTe_tsi_icl_cod_5SuiteStepBase();
	CTe_tsi_icl_cod_5SuiteStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();
	
	// For bitmap comparison.  Returns the number of different pixels
	TUint CompareBitmaps(const CFbsBitmap* aBitmap1, const CFbsBitmap* aBitmap2);

protected:
	HBufC8*		iReadData;
	HBufC8*		iWriteData;
	};

#endif // TE_TSI_ICL_COD_5_STEP_BASE
