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
// TestStepMeasurePerformance.cpp
//

// EPOC includes
#include <e32base.h>
#include <ecom/ecom.h>


// Test system includes
#include <testframework.h>

// Specific includes for this test suite
#include "TestStepIclPfm.h"
#include "TestSuiteIclPfm.h"

// Specific includes for these test steps
#include "TestStepMeasurePfm.h"


// Test steps and Files to be tested:
const TIclProfileTest KTestDetails[] =
	{
		{ _S("MM-ICL-PFM-001-LP"), _S("bmp1bpp.bmp"),	EBMP, ENormal},
		{ _S("MM-ICL-PFM-002-LP"), _S("bmp1bpp.bmp"),	EBMP, EWait},
		{ _S("MM-ICL-PFM-003-LP"), _S("bmp24bpp.bmp"),	EBMP, ENormal},
		{ _S("MM-ICL-PFM-004-LP"), _S("bmp24bpp.bmp"),	EBMP, EWait},
		{ _S("MM-ICL-PFM-005-LP"), _S("jfif10.jpg"),	EJPG, ENormal},
		{ _S("MM-ICL-PFM-006-LP"), _S("jfif10.jpg"),	EJPG, EWait},
		{ _S("MM-ICL-PFM-007-LP"), _S("jfifprog.jpg"),	EJPG, ENormal},
		{ _S("MM-ICL-PFM-008-LP"), _S("jfifprog.jpg"),	EJPG, EWait},
		{ _S("MM-ICL-PFM-009-LP"), _S("png1.png"),		EPNG, ENormal},
		{ _S("MM-ICL-PFM-010-LP"), _S("png1.png"),		EPNG, EWait},
		{ _S("MM-ICL-PFM-011-LP"), _S("png3.png"),		EPNG, ENormal},
		{ _S("MM-ICL-PFM-012-LP"), _S("png3.png"),		EPNG, EWait}
	};

const TIclProfileScalingTest KScalingTestDetails[] =
	{
// Scale Down
// ENormal
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 640, 480, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 640, 480, CBitmapScaler::EMaximumQuality},
// Scale Down
// EFast
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 640, 480, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 640, 480, CBitmapScaler::EMediumQuality},
// Scale Down
// EMinimumQuality
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 640, 480, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 640, 480, CBitmapScaler::EMinimumQuality},
// Scale Up
// ENormal
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 1600, 1200, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 1600, 1200, CBitmapScaler::EMaximumQuality},
// Scale Up
// CBitmapScaler::EMediumQuality
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 1600, 1200, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 1600, 1200, CBitmapScaler::EMediumQuality},
// Scale Up
// CBitmapScaler::EMinimumQuality
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 1600, 1200, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 1600, 1200, CBitmapScaler::EMinimumQuality},
// Same size
// ENormal
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 1152, 864, CBitmapScaler::EMaximumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 1152, 864, CBitmapScaler::EMaximumQuality},
// Same size
// CBitmapScaler::EMediumQuality
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 1152, 864, CBitmapScaler::EMediumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 1152, 864, CBitmapScaler::EMediumQuality},
// Same size
// CBitmapScaler::EMinimumQuality
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray2, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray4, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray16, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EGray256, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor256, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor64K, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16M, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor4K, 1152, 864, CBitmapScaler::EMinimumQuality},
		{ _S("MM-ICL-PFM-SCLG-001-LP"), _S("j1152x864.jpg"), EJPG, EWait, 1, EColor16MU, 1152, 864, CBitmapScaler::EMinimumQuality},
};

// Output (scaled) file name
_LIT(KScaledFileName, "scaled.jpg");

const TInt KTotalProfiles = 13;

/**
  *
  * Test step constructor.
  * Each test step initialises its own name.
  *
  * 
  */

CMmIclPfmTest001::CMmIclPfmTest001()
	{
	// store the name of this test case
	_LIT( KPanicPfmTest, "MM-ICL-PFM-001-LP");
	iTestStepName = KPanicPfmTest;
	}

/**
  * Do the test step.
  * @return	"enum TVerdict"
  */

TVerdict CMmIclPfmTest001::DoTestStepL( )
	{
	REComSession& ecomSession = REComSession::OpenL();
	CleanupClosePushL(ecomSession);

	const TInt numTestSteps = sizeof(KTestDetails) / sizeof(TIclProfileTest);
	for(TInt i = 0; i < numTestSteps; i++)
		{
		const TIclProfileTest& testDetails = KTestDetails[i];
		iTestStepResult = DoProfileL(testDetails);//@@@ do what with it???
		}
	iTestStepResult = EPass;

	CleanupStack::PopAndDestroy(&ecomSession);//ecomSession

	return iTestStepResult;
	}


/**
  * Do time profiling for particular file
  * This method allows us to measure the
  * total time taken for the process.
  */

TVerdict CMmIclPfmTest001::DoProfileL(const TIclProfileTest aProfileTest)
	{
	iTestStepName = aProfileTest.iTestName;
	TBuf<128> outBuf;
	outBuf.Format(_L("\n"));
	Log(outBuf);
	outBuf.Format(_L("---------------------------------------------"));
	Log(outBuf);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(TSize(0,0), EColor4K));

	const TFileName fileName = aProfileTest.iTestFileName;
	outBuf.Format(_L("Filename: "));
	outBuf.AppendFormat(fileName);
	Log(outBuf);

	TInt repeatCount = 100;
	if(aProfileTest.iTestType == EWait)
		{
		repeatCount = 10;
		}
	TInt err=KErrNone;
	

	for (TInt i = 0; i < repeatCount; i++)
		{

		TRAP(err, DecodeFileToBitmapL(fileName, *bitmap, 1, KNullUid));

		if (aProfileTest.iTestType==EWait)
			{
			User::After(5000000); // wait 5s for ECom to unload
			}

		if (err!=KErrNone) // stop when we see error
			{
			break;
			}
		}

	if(err)
		{
		outBuf.Format(_L("*** Test Failed... reason %d ***"), err);
		Log(outBuf);
		CleanupStack::PopAndDestroy(2); // bitmap and ecomSession
		return EFail;
		}
		
	TProfile profile[5];


	outBuf.Format(_L("Profile results"));
	Log(outBuf);
	
	for (TInt index=0; index<5; index++)
		{
		outBuf.Format(iTestStepName);
		outBuf.AppendFormat(_L(" profile(%d) time=%d count=%d"), index, profile[index].iTime, profile[index].iCount);
		Log(outBuf);
		}

	CleanupStack::PopAndDestroy(); // bitmap
	return EPass;
	}



/**
  * DecodeFileToBitmapL
  * Opens the filename into a CImageDecoder* object
  * then converts to an EPOC CFbsBitmap* object
  * This method allows us to measure more precisely
  * the individual times taken for the process.
  */

void CMmIclPfmTest001::DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor,const TUid aImageType)
	{

	CImageDecoder* decoder = NULL;
	
	TFileName fileName = Suite()->DefaultPath();
	fileName.Append(aFileName);
	
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFs, fileName, CImageDecoder::EOptionNone, aImageType));


	User::LeaveIfError(error);

	CleanupStack::PushL(decoder);
	TFrameInfo frameInfo(decoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));
	iActiveListener->InitialiseActiveListener();
	decoder->Convert(&iActiveListener->iStatus, aBitmap);


	CActiveScheduler::Start();


	if(iActiveListener->iStatus != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	CleanupStack::PopAndDestroy(decoder); 
	}

/**
  *
  * Test step constructor.
  * Each test step initialises its own name.
  *
  * 
  */
CMmIclPfmTest002::CMmIclPfmTest002()
	{
	// store the name of this test case
	_LIT( KPanicPfmTest, "MM-ICL-PFM-SCLG-001-LP");
	iTestStepName = KPanicPfmTest;
	}

/**
  * Do the test step.
  * @return	"enum TVerdict"
  */
TVerdict CMmIclPfmTest002::DoTestStepL( )
	{
	REComSession& ecomSession = REComSession::OpenL();
	CleanupClosePushL(ecomSession);

	const TInt numTestSteps = sizeof(KScalingTestDetails) / sizeof(TIclProfileScalingTest);
	for(TInt i = 0; i < numTestSteps; i++)
		{
		const TIclProfileScalingTest& testDetails = KScalingTestDetails[i];
		iTestStepResult = DoProfileL(testDetails);//@@@ do what with it???
		}
	iTestStepResult = EPass;

	CleanupStack::PopAndDestroy(&ecomSession);//ecomSession

	return iTestStepResult;
	}

/**
  * Do time profiling for particular file
  * This method allows us to measure the
  * total time taken for the process.
  */
TVerdict CMmIclPfmTest002::DoProfileL(const TIclProfileScalingTest aProfileTest)
	{
	iTestStepName = aProfileTest.iTestName;
	TBuf<128> outBuf;
	outBuf.Format(_L("\n"));
	Log(outBuf);
	outBuf.Format(_L("---------------------------------------------"));
	Log(outBuf);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(TSize(0,0), aProfileTest.iDisplayMode));

	const TFileName fileName = aProfileTest.iTestFileName;
	outBuf.Format(_L("Filename - fullsize: "));
	outBuf.AppendFormat(fileName);
	Log(outBuf);

	// Scaled file settings read in from profile
	TSize scale(aProfileTest.iDestWidth, aProfileTest.iDestHeight);

	outBuf.Format(_L("Scaled file size: %d x %d; Zoomfactor = %d."), aProfileTest.iDestWidth, aProfileTest.iDestHeight, aProfileTest.iZoomFactor);
	Log(outBuf);

	// Bitmap colour depth
	if (aProfileTest.iDisplayMode == EColor16M)
		{
		outBuf.Format(_L("Bitmap colour depth: EColor16M"));
		Log(outBuf);
		}
	else if (aProfileTest.iDisplayMode == EColor16MU)
		{
		outBuf.Format(_L("Bitmap colour depth: EColor16MU"));
	Log(outBuf);
		}

	if (aProfileTest.iQuality == CBitmapScaler::EMediumQuality)
		{
		outBuf.Format(_L("Using medium quality algorithm"));
		Log(outBuf);
		}
	else if (aProfileTest.iQuality == CBitmapScaler::EMinimumQuality)
		{
		outBuf.Format(_L("Using minimum quality algorithm"));
		Log(outBuf);
		}

	TInt repeatCount = 10;
	if(aProfileTest.iTestType == EWait)
		{
		//repeatCount = 10;
		repeatCount = 1;	// DEBUG
		}
	TInt err=KErrNone;


	for (TInt i = 0; i < repeatCount; i++)
		{
		// Zoom factor specified in TIclProfileTest
		TRAP(err, DecodeFileToBitmapL(fileName, *bitmap, aProfileTest.iZoomFactor, KNullUid));
		if (err == KErrNone)
			{
			TRAP(err, ScaleBitmapL(*bitmap, scale, aProfileTest.iQuality));
			}
		if (err == KErrNone)
			{
			TRAP(err, EncodeBitmapToFileL(KScaledFileName, *bitmap, KImageTypeJPGUid));
			}
		if (aProfileTest.iTestType==EWait)
			{
			User::After(5000000); // wait 5s for ECom to unload
			}

		if (err!=KErrNone) // stop when we see error
			{
			break;
			}
		}

	if(err)
		{
		outBuf.Format(_L("*** Test Failed... reason %d ***"), err);
		Log(outBuf);
		CleanupStack::PopAndDestroy(2); // bitmap and ecomSession
		return EFail;
		}
		
	TProfile profile[KTotalProfiles];


	outBuf.Format(_L("Profile results"));
	Log(outBuf);
	
	for (TInt index = 0; index < KTotalProfiles; index++)
		{
		outBuf.Format(iTestStepName);
		outBuf.AppendFormat(_L(" profile(%d) time=%d count=%d"), index, profile[index].iTime, profile[index].iCount);
		Log(outBuf);
		}

	CleanupStack::PopAndDestroy(); // bitmap
	return EPass;
	}

/**
  * DecodeFileToBitmapL
  * Opens the filename into a CImageDecoder* object
  * then converts to an EPOC CFbsBitmap* object
  * This method allows us to measure more precisely
  * the individual times taken for the process.
  */

void CMmIclPfmTest002::DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor,const TUid aImageType)
	{

	CImageDecoder* decoder = NULL;
	
	TFileName fileName = Suite()->DefaultPath();
	fileName.Append(aFileName);
	
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFs, fileName, CImageDecoder::EOptionNone, aImageType));
	if (error != KErrNone)
		{

		User::Leave(error);
		}

	CleanupStack::PushL(decoder);

	TFrameInfo frameInfo(decoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	error = aBitmap.Resize(TSize(0,0));
	if (error != KErrNone)
		{

		User::Leave(error);
		}
	error = aBitmap.Resize(frameSize);
	if (error != KErrNone)
		{

		User::Leave(error);
		}
	iActiveListener->InitialiseActiveListener();
	decoder->Convert(&iActiveListener->iStatus, aBitmap);
	CActiveScheduler::Start();


	if(iActiveListener->iStatus != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	CleanupStack::PopAndDestroy(decoder); 
	}

/**
  * ScaleBitmapL
  * Scales the bitmap to the specified size in pixels.
void CMmIclPfmTest002::ScaleBitmapL(CFbsBitmap& aBitmap,
									const TSize aSizeInPixels,
									TCustomCommand aCustomCommand)
  */
void CMmIclPfmTest002::ScaleBitmapL(CFbsBitmap& aBitmap,
									const TSize aSizeInPixels,
									CBitmapScaler::TQualityAlgorithm aQuality)
	{
	// Use the bitmap transform library's CBitmapScaler...

	CBitmapScaler* scaler = NULL;
	TRAPD(err, scaler = CBitmapScaler::NewL());
	if (err != KErrNone)
		{

		User::Leave(err);
		}

	TAny* value = &aQuality;
	scaler->CustomCommand(KICLUidSetQualityAlgorithmCommand, value);
	CleanupStack::PushL(scaler);				// --> scaler
	iActiveListener->InitialiseActiveListener();
	scaler->Scale(&iActiveListener->iStatus, aBitmap, aSizeInPixels, EFalse);
	CActiveScheduler::Start();


	if(iActiveListener->iStatus != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	CleanupStack::PopAndDestroy(scaler);		// <-- scaler

	}

/**
  * EncodeBitmapToFileL
  * Opens the jpeg file into a CImageEncoder* object
  * then converts it from the EPOC CFbsBitmap* object
  * This method allows us to measure more precisely
  * the individual times taken for the process.
  */

void CMmIclPfmTest002::EncodeBitmapToFileL(const TDesC& aFileName,
										   CFbsBitmap& aBitmap,
										   const TUid aImageType)
	{
	// Instantiate an encoder

	CImageEncoder* encoder = NULL;
	TUid jpgEncUid = {0x101F45D9};	// JPG encoder UID!

	TFileName fileName = Suite()->DefaultPath();
	fileName.Append(aFileName);

	TRAPD(err, encoder = CImageEncoder::FileNewL(iFs, fileName, CImageEncoder::EOptionNone, aImageType, KNullUid, jpgEncUid));
	if (err != KErrNone)
		{

		User::Leave(err);
		}

	CleanupStack::PushL(encoder);					// encoder -->
	iActiveListener->InitialiseActiveListener();
	encoder->Convert(&iActiveListener->iStatus, aBitmap);
	CActiveScheduler::Start();


	if(iActiveListener->iStatus != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	CleanupStack::PopAndDestroy(encoder);			// encoder <--
	}

