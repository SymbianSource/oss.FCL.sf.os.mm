// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TestStepMeasurePerformance.h
// This file contains Test step implementations for
// ICL performance assessment
//

#if (!defined __TESTSTEP_ICLPERFORMANCE_H__)
#define __TESTSTEP_ICLPERFORMANCE_H__


#include "TestStepIclPfm.h"
#include <bitmaptransforms.h>


enum TIclProfileTestType
{
	ENormal = 0,
	EWait = 1
};

struct TIclProfileTest
{
	const TText* iTestName;
	const TText* iTestFileName;
	TImageFormat iFormat;
	TIclProfileTestType iTestType;
};


enum TCustomCommand
	{
	ENull,
	EFast,
	EMinimumQuality
	};

struct TIclProfileScalingTest
	{
	const TText*					iTestName;
	const TText*					iTestFileName;
	TImageFormat					iFormat;
	TIclProfileTestType				iTestType;
	TInt							iZoomFactor;
	TDisplayMode					iDisplayMode;	// the bitmap's colour depth
	TInt							iDestWidth;		// the required width in pixels of the target image.
	TInt							iDestHeight;	// the required height in pixels of the target image.
	CBitmapScaler::TQualityAlgorithm iQuality;
	};


class  CActiveListener;


/**
  *  CMmIclPfmTest01
  */

class CMmIclPfmTest001 : public CTestStepIclPerformance
	{
public:
	CMmIclPfmTest001();
	virtual  TVerdict DoTestStepL();
	
	TVerdict DoProfileL(const TIclProfileTest aProfileTest);
	//void DoProfileL(TBool aWaitForUnload);
	void DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor,const TUid aImageType);
	};

/**
  *  CMmIclPfmTest002
  */
class CMmIclPfmTest002 : public CTestStepIclPerformance
	{
public:
	CMmIclPfmTest002();
	virtual  TVerdict DoTestStepL();
	
	TVerdict DoProfileL(const TIclProfileScalingTest aProfileTest);
	void DecodeFileToBitmapL( const TDesC& aFileName, CFbsBitmap& aBitmap, TInt aZoomFactor, const TUid aImageType);
	void ScaleBitmapL(CFbsBitmap& aBitmap, const TSize aSizeInPixels, CBitmapScaler::TQualityAlgorithm aQuality);
	void EncodeBitmapToFileL(const TDesC& aFileName, CFbsBitmap& aBitmap, const TUid aImageType);
	};


#endif // __TESTSTEP_ICLPERFORMANCE_H__
