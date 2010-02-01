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

#ifndef __TEST_STEP_BITMAP_TRANSFORMS_H__
#define __TEST_STEP_BITMAP_TRANSFORMS_H__

#include <bitmaptransforms.h>

#include <e32base.h>
/**
 *
 * CActiveListener provides the asynchronous operation
 * of an active object
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 *
 */

class CActiveListener : public CActive
	{
public:
	CActiveListener();
	void InitialiseActiveListener();
	
	//From CActive
	virtual void RunL();
	virtual void DoCancel();
	
	};



class CTestSuite;
class CTestSuiteBitmapTransforms;

/*
 *
 * CTestStepBitmapTransforms, which is the base class for all 
 * the BitmapTransforms suite test steps, providing common functionality.
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */
class CTestStepBitmapTransforms : public CTestStep
	{
public:
	CTestStepBitmapTransforms(TBool aAllocTest);
	~CTestStepBitmapTransforms();
	
	virtual TVerdict TestStepL() = 0;
	virtual void Setup() {};

	//From CTestStep
	TVerdict DoTestStepPreambleL();
	TVerdict DoTestStepL();
	TVerdict DoTestStepPostambleL();

	TVerdict TestStepWithFbsSessionL();
	
	void SetSuite( CTestSuiteBitmapTransforms* aPtr );
	CTestSuiteBitmapTransforms* Suite();

	static void FillBitmapL(CFbsBitmap& srcBmp, const TRgb& rgb);
	
	TVerdict CheckBitmaps( CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2,TInt aQuarterTurns );
	
	void CheckBitmapL( CFbsBitmap& aBitmap );
	
	void SetTestFileNameL( const TDesC& aPathNameAndExtn );
	TFileName& FileName() ;

	TVerdict RotateAndMirrorABitmapL( const TFileName& iSourceFileName, CBitmapRotator::TRotationAngle aAngle , bool KeepOriginalBitmap = true );

	inline TInt UseLowMemoryScaling(CBitmapScaler* aScaler, TAny* aPtr, TBool aLowMem);
	
	void DisplayQualityAlgo(CBitmapScaler::TQualityAlgorithm aQualityAlgo);

protected:
	TBool iAllocTest;

private:
	CTestSuiteBitmapTransforms*  iBitmapTransformsSuite;
	TFileName					 iSourceFileName;
	CActiveScheduler*			 iScheduler ;
	};


inline TInt CTestStepBitmapTransforms::UseLowMemoryScaling(CBitmapScaler* aScaler, TAny* aPtr, TBool aLowMem)
	{
	aPtr = &aLowMem;
	TInt errCode = aScaler->CustomCommand( KICLUidUseLowMemoryAlgorithmCommand, aPtr );
	return errCode;
	}

#endif // __TEST_STEP_BITMAP_TRANSFORMS_H__
