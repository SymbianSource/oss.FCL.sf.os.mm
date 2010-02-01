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

#if (!defined __TESTSTEP_FAST_SCALE_H__)
#define __TESTSTEP_FAST_SCALE_H__

#include "TestStepBitmapTransforms.h"

class  TScaleBitmapUnitTestParams
	{
	public:
	const TText*  iInputFilename;         // input bitmap
	const TText*  iScaleUpFileName;	  // output Scale XP Bitmap
	const TText*  iScaleDownFileName;     // output Scale Y Bitmap
	CBitmapScaler::TQualityAlgorithm iQualityAlgo;//input Scaling Algorithm 
	TDisplayMode  iBitmapType;    
	};

/*
 *
 * CTestStepFastScaleXY
 * This class provides a test step to exercise the scale the bitmap
 * in x and y axis.
 * 
 */

class CTestStepFastScaleXY:public CTestStepBitmapTransforms
	{
public:
	CTestStepFastScaleXY(TUint aTestIndex,const TText*  iTestName);
	~CTestStepFastScaleXY();
	virtual TVerdict DoTestStepL();
private:
	TVerdict DoTestScaleL(CActiveListener* aActiveListener,
					CFbsBitmap* aSrcBmp,
					CBitmapScaler* aScaler,
					const TSize aSize,
					CBitmapScaler::TQualityAlgorithm aQualityAlgo);
private:
	const TScaleBitmapUnitTestParams *iTestParameters;
	};

/*
 *
 * CTestStepFastScaleX
 * This class provides a test step to exercise the scale the bitmap
 * in x axis.
 * 
 */

class CTestStepFastScaleX:public CTestStepBitmapTransforms
	{
public:
	CTestStepFastScaleX(TUint aTestIndex,const TText*  iTestName);
	~CTestStepFastScaleX();
	virtual TVerdict DoTestStepL();
private:
	TVerdict DoTestScaleL(CActiveListener* aActiveListener,
					CFbsBitmap* aSrcBmp,
					CBitmapScaler* aScaler,
					const TSize aSize,
					CBitmapScaler::TQualityAlgorithm aQualityAlgo);
private:
	const TScaleBitmapUnitTestParams *iTestParameters;
	};

/*
 *
 * CTestStepFastScaleY
 * This class provides a test step to exercise the scale the bitmap
 * in y axis.
 * 
 */


class CTestStepFastScaleY:public CTestStepBitmapTransforms
	{
public:
	CTestStepFastScaleY(TUint aTestIndex,const TText*  iTestName);
	~CTestStepFastScaleY();
	virtual TVerdict DoTestStepL();
private:
	TVerdict DoTestScaleL(CActiveListener* aActiveListener,
					CFbsBitmap* aSrcBmp,
					CBitmapScaler* aScaler,
					const TSize aSize,
					CBitmapScaler::TQualityAlgorithm aQualityAlgo);
private:
	const TScaleBitmapUnitTestParams *iTestParameters;
	};

/*
 *
 * CTestStepFastScaleCopy
 * This class provides a test step to exercise the scale the bitmap
 * to the same width and height as the original.
 * 
 */

class CTestStepFastScaleCopy:public CTestStepBitmapTransforms
	{
public:
	CTestStepFastScaleCopy(TUint aTestIndex,const TText*  iTestName);
	~CTestStepFastScaleCopy();
	virtual TVerdict DoTestStepL();
private:
	TVerdict DoTestScaleL(CActiveListener* aActiveListener,
					CFbsBitmap* aSrcBmp,
					CBitmapScaler* aScaler,
					const TSize aSize,
					CBitmapScaler::TQualityAlgorithm aQualityAlgo);
private:
	const TScaleBitmapUnitTestParams *iTestParameters;
	};
	
	
class CTestStepScale16MA:public CTestStepBitmapTransforms
	{
public:
	CTestStepScale16MA(const TText*  iTestName, const TDesC& aRgbFile, const TDesC& aAlphaFile);
	~CTestStepScale16MA();
	virtual TVerdict DoTestStepL();
private:
	CFbsBitmap* CreateRefBitmapL(CFbsBitmap& aRgbSrc,CFbsBitmap& aAlphaSrc,
									const TSize& aDestSize,CBitmapScaler::TQualityAlgorithm aQualityAlgo,
									CBitmapScaler& aScaler,
									TBool aLowMemAlg);
	TVerdict DoTestScaleL(CActiveListener* aActiveListener,
					CFbsBitmap* aSrcBmp,
					CFbsBitmap* aRgbSrc,
					CFbsBitmap* aAlphaSrc,
					CBitmapScaler* aScaler);
private:
	const TPtrC 	iRgbFile;
	const TPtrC 	iAlphaFile;
	};	

/*
 *
 * CTestStepFastDownScale
 * This class provides a test step to exercise the fast down scale the bitmap
 * and compare the results.
 * 
 */
class CTestStepFastDownScale : public CTestStepBitmapTransforms
	{
public:
	CTestStepFastDownScale(const TText*  aTestName);
	~CTestStepFastDownScale();
	virtual  TVerdict DoTestStepL();	
	};
#endif //__TESTSTEP_FAST_SCALE_H__



