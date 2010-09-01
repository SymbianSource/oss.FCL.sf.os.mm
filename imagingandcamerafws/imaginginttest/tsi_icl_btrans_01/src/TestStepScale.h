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
// This file contains Test step implementations for
// the CBitmapScaler class.
//

#ifndef __TESTSTEP_SCALE_H__
#define __TESTSTEP_SCALE_H__


#include "TestStepBitmapTransforms.h"

class  CActiveListener;


/**
 *
 *	CMmIclBtransI0205Cp 
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */

class CMmIclBtransI0205Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0205Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	};


/**
 *
 *	CMmIclBtransI0206Cp 
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */

class CMmIclBtransI0206Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0206Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	};

/**
 *
 *	CMmIclBtransI0304Cp 
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */

class CMmIclBtransI0304Cp : public CTestStepBitmapTransforms
	{
public:
	CMmIclBtransI0304Cp(TBool aAllocTest);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	};


/**
 *
 *	CMmIclBitmapScaleLowMem1
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 */

class CMmIclBitmapScaleLowMem1 : public CTestStepBitmapTransforms
	{
public:
	//From CTestStepBitmapTransforms
	CMmIclBitmapScaleLowMem1(TBool aAllocTest, TDisplayMode aDisplayMode, const TDesC& aTestStepName);
	void Setup();
	TVerdict TestStepL();
	
private:
	TDisplayMode iDisplayMode;
		
};

/*
 *
 *	CMmIclBitmapScaleSpeed1
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Scaling a 1152x864 size bitmap of type EColor16MU into 640x480 bitmap 
 * with UseMediumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScaleSpeed1 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleSpeed1(const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
    };

/*
 *
 *	CMmIclBitmapScaleSpeed2
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Scaling a 576x432 size bitmap of type EColor16MU into 640x480 bitmap with 
 * UseMediumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScaleSpeed2 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleSpeed2(const TDesC& aTestStepName);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
    };

/*
 *
 *	CMmIclBitmapScale100By1To100By100
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * This test checks the boundary condition of scaling a 100 x 1 bitmap to 
 * another bitmap of size 100 x 100, with UseMediumQuality Algorithm and 
 * UseMinimumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScale100By1To100By100 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale100By1To100By100(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScale1By100To100By100
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * This test checks the boundary condition of scaling a 1 x 100 bitmap 
 * to another bitmap of size 100 x 100, with UseMediumQuality Algorithm 
 * and UseMinimumQuality Algorithm enabled
 *
 */

class CMmIclBitmapScale1By100To100By100 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale1By100To100By100(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScaleUpAndDown
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Scale a bitmap up by 2 and down by 2. It compares the size of the resultant bitmap 
 * to the original, with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScaleUpAndDown : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleUpAndDown(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);

	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;
    };

/*
 *
 *	CMmIclBitmapScaleMaxMedSpeed
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Scale a bitmap up by 2 to the target bitmap with UseMaximumQuality Algorithm enabled. 
 * Then Enable UseMediumQuality Algorithm and scale the image by 2 to itself. Check the time.
 *
 */

class CMmIclBitmapScaleMaxMedSpeed : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleMaxMedSpeed(const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
    };

/*
 *
 *	CMmIclBitmapScaleMedMinSpeed
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Scale a bitmap up by 2 to the target bitmap with UseMediumQuality Algorithm enabled. 
 * Then Enable UseMinimumQualityAlgorithm and scale the image by 2 to itself. Check the time.
 *
 */

class CMmIclBitmapScaleMedMinSpeed : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleMedMinSpeed(const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
    };

/*
 *
 *	CMmIclBitmapScaleStretchAndSquash
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Scale an EColor16MU Bitmap by simulaneous sretching and squashing on both the X and Y axes, 
 * with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScaleStretchAndSquash : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleStretchAndSquash(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;
    };

/*
 *
 *	CMmIclBitmapScale100By100To100By1
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * This test checks the boundary condition of scaling a 100 x 100 bitmap 
 * to 100 x 1 bitmap with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScale100By100To100By1 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale100By100To100By1(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScale100By100To1By100
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * This test checks the boundary condition of scaling a 100 x 100 bitmap 
 * to 1 x 100 bitmap with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled
 *
 */

class CMmIclBitmapScale100By100To1By100 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale100By100To1By100(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScale0By100To200By0
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 
 * 200 x 0 with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScale0By100To200By0 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale0By100To200By0(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScale0By100To100By100
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 
 * 100 x 100 with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
 *
 */

class CMmIclBitmapScale0By100To100By100 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale0By100To100By100(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScale0By0To100By100
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Create a 0 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
 * with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled
 *
 */

class CMmIclBitmapScale0By0To100By100 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale0By0To100By100(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScale100By0To100By100
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Create a 100 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
 * with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.

 *
 */

class CMmIclBitmapScale100By0To100By100 : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScale100By0To100By100(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	
	//From CTestStepBitmapTransforms	
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;		
    };

/*
 *
 *	CMmIclBitmapScaleSpeedAlloc
 *
 * @lib "TSI_ICL_BTRANS_01.lib"
 *
 * Tests heap allocation failures, during scaling a bitmap using UseMediumQuality 
 * and UseMinimumQuality Algorithms.
 *
 */

class CMmIclBitmapScaleSpeedAlloc : public CTestStepBitmapTransforms
	{
public:
	CMmIclBitmapScaleSpeedAlloc(CBitmapScaler::TQualityAlgorithm aQualityAlgo, const TDesC& aTestStepName);
	void ScaleBitmapWithQualityAlgorithmL(CFbsBitmap* aSrcBmp, CFbsBitmap* aTgtBmp);
	
	//From CTestStepBitmapTransforms
	void Setup();
	TVerdict TestStepL();
	
private:
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;
    };
    
#endif // __TESTSTEP_SCALE_H__
