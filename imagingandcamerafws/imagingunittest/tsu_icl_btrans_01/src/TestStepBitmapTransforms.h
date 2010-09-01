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
//

#if (!defined __BITMAP_TRANSFORMS_TESTSTEP_H__)
#define __BITMAP_TRANSFORMS_TESTSTEP_H__

#include <bitmaptransforms.h>

#include <e32base.h>
/**
 *
 * CActiveListener provides the asynchronous operation
 * of an active object
 *
 * @lib "TS_BitmapTransforms.lib"
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
	TInt Result() const;
	
	};

class CTestSuite;
class CTestSuiteBitmapTransforms;

/**
 *
 * CTestStepBitmapTransforms, which is the base class for all 
 * the BitmapTransforms suite test steps, providing common functionality.
 *
 * @lib "TS_BitmapTransforms.lib"
 *
 */
class CTestStepBitmapTransforms : public CTestStep
{
public:
	CTestStepBitmapTransforms(TBool aConnectToFbs = ETrue);
	~CTestStepBitmapTransforms();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
	void SetSuite( CTestSuiteBitmapTransforms* aPtr );
 	CTestSuiteBitmapTransforms* Suite();

	static void FillBitmapL(CFbsBitmap& srcBmp, const TRgb& rgb);
	
	TVerdict CheckBitmaps( CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2,
		                  TInt aQuarterTurns, TInt aCheckForAllWhite=EFalse );
	
	TVerdict CheckBitmapL(CFbsBitmap& aBitmap );
	
    void SetTestFileNameL( const TDesC& aPathNameAndExtn );
	TFileName& FileName() ;

	TVerdict RotateAndMirrorABitmap( const TFileName& iSourceFileName, CBitmapRotator::TRotationAngle aAngle, TDisplayMode aDisplayMode=ENone );
	void RotateAndMirrorABitmapL( const TFileName& iSourceFileName, CBitmapRotator::TRotationAngle aAngle, TDisplayMode aDisplayMode=ENone  );

protected:
	static CFbsBitmap* Create16MABitmapL(CFbsBitmap& aColorBmp, CFbsBitmap& aAlphaBmp);
	static void Split16MABitmapL(const CFbsBitmap& a16MABmp, CFbsBitmap*& aColorBmp, CFbsBitmap*& aAlphaBmp);
	static CFbsBitmap* LoadAndCreate16MABitmapL(const TDesC& aRgb, const TDesC& aAlpha);
	static void DuplicateContentL(const CFbsBitmap& aSrcBmp, CFbsBitmap& aDestBmp);
	
private:
  	 CTestSuiteBitmapTransforms* iBitmapTransformsSuite;
	 RFs                         iFs;
	 TFileName                   iSourceFileName;
	 CActiveScheduler*           iScheduler ;
	 TBool						 iConnectToFbs;
};

#endif /* __BITMAP_TRANSFORMS_TESTSTEP_H__ */

