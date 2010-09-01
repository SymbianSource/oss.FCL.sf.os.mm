// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TESTSTEPGIF_SCALER_H
#define TESTSTEPGIF_SCALER_H

#include <gifscaler.h>
#include <e32base.h>

/**
 *
 * CActiveListener provides the asynchronous operation
 * of an active object
 *
 * @lib "TSU_ICL_GIF_SCALER.lib"
 *
 *
 */

class CActiveListener : public CActive
	{
public:
	CActiveListener();
	~CActiveListener();

    //From CActive
	virtual void RunL();
	virtual void DoCancel();

	void InitialiseActiveListener();

	TBool RequestCancelled()
	{ return iStatus == KErrCancel; }
	};

class CTestSuite;
class CTestSuiteGifScaler;

/**
 *
 * CTestStepGifScaler, which is the base class for all 
 * the GifScaler suite test steps, providing common functionality.
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 */
class CTestStepGifScaler : public CTestStep
{
public:
	CTestStepGifScaler();
	~CTestStepGifScaler();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
	void SetSuite(CTestSuiteGifScaler* aPtr );
 	CTestSuiteGifScaler* Suite();

	TVerdict CheckBitmaps(CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2);
	TVerdict CheckBitmapL(CFbsBitmap& aBitmap);
	
    void SetTestFileNameL(const TDesC& aPathNameAndExtn);
	TFileName& FileName();

	void ApplyPaletteL(CFbsBitmap& aSrcBitmap, CPalette& aPalette, CFbsBitmap& aTgtBitmap) const;

protected:
	RFs iFs;

private:
  	 CTestSuiteGifScaler* iGifScalerSuite;
	 TFileName iSourceFileName;
	 CActiveScheduler* iScheduler;
};


/**
 *
 * The following classes provide test steps to scale a bitmap and it's mask.
 * See TestStepGifScaler.cpp or the test specification for details of individual test cases.
 *
 */

/**
 *
 * TestStepScale1
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale1 : public CTestStepGifScaler
	{
public:
		CTestStepScale1();
		~CTestStepScale1();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale2
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale2 : public CTestStepGifScaler
	{
public:
		CTestStepScale2();
		~CTestStepScale2();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale3
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale3 : public CTestStepGifScaler
	{
public:
		CTestStepScale3();
		~CTestStepScale3();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale4
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale4 : public CTestStepGifScaler
	{
public:
		CTestStepScale4();
		~CTestStepScale4();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale5
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale5 : public CTestStepGifScaler
	{
public:
		CTestStepScale5();
		~CTestStepScale5();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale6
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale6 : public CTestStepGifScaler
	{
public:
		CTestStepScale6();
		~CTestStepScale6();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale7
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale7 : public CTestStepGifScaler
	{
public:
		CTestStepScale7();
		~CTestStepScale7();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale8
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale8 : public CTestStepGifScaler
	{
public:
		CTestStepScale8();
		~CTestStepScale8();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale9
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale9 : public CTestStepGifScaler
	{
public:
		CTestStepScale9();
		~CTestStepScale9();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale10
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale10 : public CTestStepGifScaler
	{
public:
		CTestStepScale10();
		~CTestStepScale10();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale11
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale11 : public CTestStepGifScaler
	{
public:
		CTestStepScale11();
		~CTestStepScale11();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale12
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale12 : public CTestStepGifScaler
	{
public:
		CTestStepScale12();
		~CTestStepScale12();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale13
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale13 : public CTestStepGifScaler
	{
public:
		CTestStepScale13();
		~CTestStepScale13();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale14
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale14 : public CTestStepGifScaler
	{
public:
		CTestStepScale14();
		~CTestStepScale14();
		virtual TVerdict DoTestStepL();
	};


/**
 *
 * AllocTestStepScale1
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CAllocTestStepScale1 : public CTestStepGifScaler
	{
public:
		CAllocTestStepScale1();
		~CAllocTestStepScale1();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * AllocTestStepScale2
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CAllocTestStepScale2 : public CTestStepGifScaler
	{
public:
		CAllocTestStepScale2();
		~CAllocTestStepScale2();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * AllocTestStepScale3
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CAllocTestStepScale3 : public CTestStepGifScaler
	{
public:
		CAllocTestStepScale3();
		~CAllocTestStepScale3();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * AllocTestStepScale4
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CAllocTestStepScale4 : public CTestStepGifScaler
	{
public:
		CAllocTestStepScale4();
		~CAllocTestStepScale4();
		virtual TVerdict DoTestStepL();
	};

/**
 *
 * TestStepScale14
 *
 * @lib "TSU_ICL_GSCALER.lib"
 *
 *
 */
class CTestStepScale15 : public CTestStepGifScaler, public MFbsExtendedBitmapInitializer
    {
public:
        CTestStepScale15();
        ~CTestStepScale15();
        virtual TVerdict DoTestStepL();
        TInt InitExtendedBitmap(TAny* /*aData*/, TInt /*aDataSize*/);
    };

#endif // TESTSTEPGIFSCALER_H

