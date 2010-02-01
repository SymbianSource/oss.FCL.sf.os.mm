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

// Test system includes
#include "TestSuiteMngImageDisplay.h"
#include "TestStepMngImageDisplay.h"

CTestStepMagn::CTestStepMagn(const TImageDisplayUnitTestParams& aTestParams, const TUint* aColorRef, 
																			const TUint* aAlphaRef)
																			:CTestStepImgDisplayGeneric(aTestParams),
																			iColorRefData(aColorRef),
																			iAlphaRefData(aAlphaRef)
	{
	}


TBool CTestStepMagn::ConsumeFrameL(const CFbsBitmap* /*aBitmap*/)
	{
	++iFrameNumber;
	TBool continueTest=ETrue;
	const CFbsBitmap* frame;
	const CFbsBitmap* mask;
	iImageDisplay->GetBitmap(frame, mask);
	if (frame==NULL || mask==NULL)
		{
		INFO_PRINTF1(_L("Error: Frame bitmap or Mask one is NULL"));
		iLastError = KErrGeneral;
		}
	else
		{
		if (!(CompareBitmapToRefBuffer(frame, iColorRefData) == EPass && 
				CompareBitmapToRefBuffer(mask, iAlphaRefData) == EPass))
			{
			iLastError = KErrGeneral;
			}
		}
	
	return continueTest;
	}

TVerdict CTestStepMagn::TestL()
	{
	ASSERT(iImageDisplay == NULL);
	TVerdict testStepResult=EPass;
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iImageDisplay->Play();
	CActiveScheduler::Start();
	if (iTestParams->iTestType==TImageDisplayUnitTestParams::EAlloc && KErrNoMemory == iLastError)
		{
		User::Leave(iLastError);
		}
	if (KErrNone != iLastError)
		{
		testStepResult = EFail;
		}
	return testStepResult;
	}
	
