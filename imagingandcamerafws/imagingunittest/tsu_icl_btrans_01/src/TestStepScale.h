// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined __TESTSTEP_SCALE_H__)
#define __TESTSTEP_SCALE_H__


#include "TestStepBitmapTransforms.h"

class  CActiveListener;

/**
 *
 * TestStepScale
 * This test exercies the scaling code
 * and is simply intended not to crash
 *
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScale : public CTestStepBitmapTransforms, public  MFbsExtendedBitmapInitializer
	{
	
public:
	CTestStepScale();
	~CTestStepScale();
	virtual  TVerdict DoTestStepL();
	 TInt InitExtendedBitmap(TAny* aData, TInt aDataSize);
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};


/**
 *
 *  CTestTgtBitmapPanicAlloc 
 *
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestTgtBitmapPanicAlloc : public CTestStepBitmapTransforms
	{
public:
	CTestTgtBitmapPanicAlloc();
	~CTestTgtBitmapPanicAlloc();
	virtual  TVerdict DoTestStepL();
	};


/**
 *
 * CTestSrcBitmapPanicAlloc 
 * This test tests a panic with srcbitmap not allocated
 *
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestSrcBitmapPanicAlloc : public CTestStepBitmapTransforms
	{
public:
	CTestSrcBitmapPanicAlloc();
	~CTestSrcBitmapPanicAlloc();
	virtual  TVerdict DoTestStepL();		
	};


/**
 *
 * TestStepCScaleAllocFailure
 * This test tests alloc failures on scale
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestScaleAllocFailure : public CTestStepBitmapTransforms
	{
public:
	CTestScaleAllocFailure();
	~CTestScaleAllocFailure();
	virtual  TVerdict DoTestStepL();
	};

/**
 *
 * CTestScaleUpDown
 *
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestScaleUpDown : public CTestStepBitmapTransforms
	{
public:
	CTestScaleUpDown();
	virtual ~CTestScaleUpDown();
	virtual  TVerdict DoTestStepL();
	};


/**
 * CTestStepScaleXY
 * 
 * This test contructs a bitmap of sizewidth by height and scales
 * it according to whether the aspect ratio must be maintained
 * It is the base class for a series of tests
 * @lib "TS_BitmapTransforms.lib"
 *
 */

template <TInt srcWidth, TInt srcHeight, 
TBool maintainAspectRatio, TInt dstWidth, TInt dstHeight> 
class CTestStepScaleXY : public CTestStepBitmapTransforms
	{
public:
	CTestStepScaleXY();
	virtual  ~CTestStepScaleXY();
	virtual  TVerdict DoTestStepL();
	};


/**
 *
 * CTestStepScaleZeroByZeroSrc
 *
 * This test scales a bitmap of size 0 by 0 to size 100 by 100
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepScaleZeroByZeroSrc : public  CTestStepScaleXY< 0, 0, ETrue, 100, 100 > 
	{
	public:
		CTestStepScaleZeroByZeroSrc();
	};


/**
 *
 * CTestStepFlipOneByX
 *
 * This test tests scaling a 0 by 100 bitmap to a 100 by 100 mainting the aspect ratio
 * 
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleZeroByNSrc : public  CTestStepScaleXY< 0, 100, ETrue, 100, 100 > 
	{
public:
	CTestStepScaleZeroByNSrc();
	};

/**
 *
 * CTestStepScaleNByZeroSrc
 *
 * This test tests scaling a 100 by 0 to 100 by 100 bitmap
 * It should panic 
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 *
 */

class CTestStepScaleNByZeroSrc : public  CTestStepScaleXY< 100, 0, ETrue, 100, 100 > 
	{
public:
	CTestStepScaleNByZeroSrc();
	};


/**
 *
 * CTestStepScaleOneToOne 
 *
 * This test contructs a bitmap of unit size width by height
 * and scales it. The scaling is determined whether aspect ratio
 * is maintained
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

template <TInt width, TInt height, 
          TBool mainntainAspectRatio > 
class CTestStepScaleOneToOne : public CTestStepBitmapTransforms
	{
public:
              CTestStepScaleOneToOne();
	virtual  ~CTestStepScaleOneToOne();
	virtual  TVerdict DoTestStepL();
	};

/**
 *
 * CTestStepScaleNByOne
 *
 * This test scales a bitmap of size N by 1 to size 
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 * @xxxx
 *
 */

class CTestStepScaleNByOne : public  CTestStepScaleOneToOne < 100, 1, ETrue > 
	{
public:
	CTestStepScaleNByOne();
	};


/**
 *
 * CTestStepScaleOneByN
 *
 * This test scales a bitmap of size 1 by y whilst maintaining the aspect ratio
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepScaleOneByN : public  CTestStepScaleOneToOne < 1, 100, ETrue > 
	{
public:
	CTestStepScaleOneByN();
	};


/**
 *
 * TestStepCScaleUpAndDown
 * This test does a scale up and scale down. It save the files to disk
 * so that they may be compared. The average colour value moves by 1 step
 * in these two combined operations. The dimensions of the tgt bitmap remain
 * the same as the src. Further checking is not possible without detailed knowledge 
 * of the scaling algorithm
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestStepScaleUpAndDown : public CTestStepBitmapTransforms
	{
public:
     CTestStepScaleUpAndDown();
	~CTestStepScaleUpAndDown();
	virtual  TVerdict DoTestStepL();

 static void FillWithCheckerBoardL(CFbsBitmap& srcBmp );
	};

/**
 *
 * CTestForceScaleMemoryAllocFailure
 * This test forces heap failure and proper leave code errors are generated
 * @lib "TS_BitmapTransforms.lib"
 *
 */

class CTestForceScaleMemoryAllocFailure : public CTestStepBitmapTransforms
	{
public:
     CTestForceScaleMemoryAllocFailure ();
	~CTestForceScaleMemoryAllocFailure ();
	virtual TVerdict DoTestStepL();
	TVerdict DoTestStepFbsL();
private:
	TVerdict ScaleABitMapL();
	void TestFailed( TInt &aErr );
	};

/**
 *
 * TestStepDisplayModeScale
 * This test checks scaling a bitmap with
 * ECOLOR16MU displaymode.
 *
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepDisplayModeScale : public CTestStepBitmapTransforms
	{
public:
	CTestStepDisplayModeScale();
	~CTestStepDisplayModeScale();
	virtual  TVerdict DoTestStepL();
	};

/**
 *
 * CTestStepScaleRoundoff
 * This test step scales the bitmap of size(288,216)
 *to (160,120) and checks if the Scalar rounds 
 *the target size correctly.
 *(DEF045698)		
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleRoundoff : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepScaleRoundoff();
	~CTestStepScaleRoundoff();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};

/**
 *
 * CTestStepScaleWhiteFillMaxQuality
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleWhiteFillMaxQuality : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepScaleWhiteFillMaxQuality();
	~CTestStepScaleWhiteFillMaxQuality();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};

/**
 *
 * CTestStepScaleWhiteFillMedQuality
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleWhiteFillMedQuality : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepScaleWhiteFillMedQuality();
	~CTestStepScaleWhiteFillMedQuality();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};

/**
 *
 * CTestStepScaleWhiteFillMiniQuality
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleWhiteFillMiniQuality : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepScaleWhiteFillMiniQuality();
	~CTestStepScaleWhiteFillMiniQuality();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};

/**
 *
 * CTestStepScaleWhiteFill64K
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleWhiteFill64K : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepScaleWhiteFill64K();
	~CTestStepScaleWhiteFill64K();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};

/**
 *
 * CTestStepScaleSrcWhiteFill
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepScaleSrcWhiteFill : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepScaleSrcWhiteFill();
	~CTestStepScaleSrcWhiteFill();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	};
	


/**
 *
 * CTestStepDEFScale
 * This test checks scaling a bitmap without maintianing 
 * the aspect ratio
 * ECOLOR16MU displaymode.
 *
 * INC057208 :: NTT - CBitMapScaler::Scale panics with USER 21
 *
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepDEFScale : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepDEFScale(const TText*  aTestName);
	~CTestStepDEFScale();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	TSize iTgtSize;
	TDisplayMode iDisplayMode;
	};


/**
 *
 * CTestStepLowQtyScale
 * This test checks scaling of Bitmap with Quality other than High. 
 * For a particular Image under discussion, Only High quality scaled image to 64x64 
 * image is correct, and low is not correct.
 *
 * ECOLOR16MU displaymode.
 *
 * DEF066157 :: Bitmap scaler produces wrong results at quality setting other than High
 *
 * @lib "TS_BitmapTransforms.lib"
 */

class CTestStepLowQtyScale : public CTestStepBitmapTransforms
	{
	
public:
	CTestStepLowQtyScale(const TText*  aTestName);
	~CTestStepLowQtyScale();
	virtual  TVerdict DoTestStepL();
	};	
	
/*
 *
 *	CTestBitmapScaleMaxMedMinSpeed
 *
 * @lib TS_BitmapTransforms.lib
 *
 *
 * INC079309 :: Bitmap scaling performance may be increased by 36% 
 *
 * Scale a bitmap up by 2 to the target bitmap with MaximumQuality Algorithm enabled. 
 * Then Enable MediumQuality Algorithm, MinimumQuality Algorithm and scale the image by 2 to itself. Check the time.
 *
 */

class CTestBitmapScaleMaxMedMinSpeed : public CTestStepBitmapTransforms
	{
public:
	CTestBitmapScaleMaxMedMinSpeed(const TText*  aTestName);
	~CTestBitmapScaleMaxMedMinSpeed();
	virtual  TVerdict DoTestStepL();
private:
	TSize iSrcSize;
	};
	

/*
 *
 *	CTestBitmapScalerOverFlow
 *
 * @lib TS_BitmapTransforms.lib
 *
 * DEF095066: Overflow problem in bitmap scaler
 * 
 *
 * 
 * 
 *
 */

class CTestBitmapScalerOverFlow : public CTestStepBitmapTransforms
	{
public:
	CTestBitmapScalerOverFlow(const TText*  aTestName);
	~CTestBitmapScalerOverFlow();
	virtual  TVerdict DoTestStepL();
private:	
	};

/**
	 *
	 * CTestStepScaleMemoryLeak
	 * Check scale allocations when an invalid target size produces KErrArgument, 
	 * or an 0, 0 target size causes an early completion which may cause memory leak.
	 * PDEF113923: BitmapScalerPlugin.cpp : Leaking memory 	
*/

class CTestStepScaleMemoryLeak : public CTestStepBitmapTransforms
	{
public:
	CTestStepScaleMemoryLeak();
	~CTestStepScaleMemoryLeak();
	virtual  TVerdict DoTestStepL();
	};

/**
 *
 * CTestStepScaleExtendedDestination
 *
 * This test tries to scale a b bitmap with an extended bitmap destination
 * 
 * @lib "TS_BitmapTransforms.lib"
 *
 * @xxxx
 *
 */


class CTestStepScaleExtendedDestination : public CTestStepBitmapTransforms
    {
    
public:
    CTestStepScaleExtendedDestination();
    ~CTestStepScaleExtendedDestination();
    virtual  TVerdict DoTestStepL();
private:
    TSize iSrcSize;
    TSize iTgtSize;
    };


#endif //(__TESTSTEP_SCALE_H__)
