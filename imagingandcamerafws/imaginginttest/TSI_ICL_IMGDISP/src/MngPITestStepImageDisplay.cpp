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

// EPOC includes
#include <e32base.h>
#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>
#include "TestHelperIface.h"

// Test system includes
#include <testframework.h>

#include "TestStepImageDisplay.h"
#include "TestSuiteImageDisplay.h"

#include "MngPITestStepImageDisplay.h"

/******************************************************************************
 *
 * class CScaleToRecommendedSize
 *
 *****************************************************************************/

/*
 * CScaleToRecommendedSize::CScaleToRecommendedSize()
 *
 * Test step constructor.
 */

CScaleToRecommendedSize::CScaleToRecommendedSize(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}


TBool CScaleToRecommendedSize::ConsumeFrame(const CFbsBitmap& aBitmap)
	{
	TFileName fileName;
	++iFrameNumber;
	fileName.Format(_L("scaled_digit_%d.mbm "),10-iFrameNumber);
	SetRefFilename(fileName);
	if (CompareBitmapWithFileL(aBitmap, RefFilename() ) != EPass )
		{
		iLastError = KErrGeneral;
		}
	return ETrue;
	}

/*
 * TVerdict CScaleToRecommendedSize::TestL()
 *
 * Test step specific code.
 */

TVerdict CScaleToRecommendedSize::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	SetupPluginL();

	// Opened -> Configured
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	iImageDisplay->SetupL();

	TSize	scaleSize = TSize( 64, 64 );
	iImageDisplay->SetSizeInPixels( scaleSize );

	iImageDisplay->Play();
	CActiveScheduler::Start();

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Decoding error %d"),iLastError);
		return (testStepResult = EFail );
		}
	if (iFrameNumber !=2 )
		{
		testStepResult = EFail;
		}

	iImageDisplay->StopPlay();

	return testStepResult;
	}

/******************************************************************************
 *
 * class CScaleToNonRecommendedSize
 *
 *****************************************************************************/

/*
 * CScaleToNonRecommendedSize::CScaleToNonRecommendedSize()
 *
 * Test step constructor.
 */

CScaleToNonRecommendedSize::CScaleToNonRecommendedSize(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CScaleToNonRecommendedSize::TestL()
 *
 * Test step specific code.
 */

TBool CScaleToNonRecommendedSize::ConsumeFrame(const CFbsBitmap& aBitmap)
	{
	TFileName fileName;
	++iFrameNumber;
	fileName.Format(_L("scaled_digit_%d.mbm "),10-iFrameNumber);
	SetRefFilename(fileName);
	if (CompareBitmapWithFileL(aBitmap, RefFilename() ) != EPass )
		{
		iLastError = KErrGeneral;
		}
	return ETrue;
	}

TVerdict CScaleToNonRecommendedSize::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	SetupPluginL();

	// Opened -> Configured
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	iImageDisplay->SetupL();

	TSize	scaleSize = TSize( 64, 64 );
	iImageDisplay->SetSizeInPixels( scaleSize );

	iImageDisplay->Play();
	CActiveScheduler::Start();

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Decoding error %d"),iLastError);
		return (testStepResult = EFail );
		}
	if (iFrameNumber !=2 )
		{
		testStepResult = EFail;
		}

	iImageDisplay->StopPlay();

	return testStepResult;
	}
