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

#if (!defined __TESTSTEP_LOWMEMORY_SCALE_H__)
#define __TESTSTEP_LOWMEMORY_SCALE_H__

#include "TestStepBitmapTransforms.h"

class TScaleBitmapUnitTestParams;

class CTestStepLowMemoryScale : public CTestStepBitmapTransforms
	{
public:
	CTestStepLowMemoryScale( TUint aTestIndex );
	~CTestStepLowMemoryScale();
	virtual TVerdict DoTestStepL();
private:
	void DoTestScaleL(CActiveListener* aActiveListener,
						CFbsBitmap* aSrcBmp,
						CBitmapScaler* aScaler,
						const TSize aSize,
						TFileName aXx,
						const TBool aFailNext);
private:
	const TScaleBitmapUnitTestParams *iTestParameters;
	};

#endif // __TESTSTEP_LOWMEMORY_SCALE_H__
