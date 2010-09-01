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

#include "IntTestStepImageDisplay.h"

/*
 * CDecodeFileByNameByPluginUid::CDecodeFileByNameByPluginUid()
 *
 * Test step constructor.
 */

CDecodeFileByNameByPluginUid::CDecodeFileByNameByPluginUid(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeFileByNameByPluginUid::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeFileByNameByPluginUid::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	SetupPluginL();

	TRAP(iLastError, iImageDisplay->SetupL() );

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		return (testStepResult = EFail );
		}
	// try PluginLoaded -> (Processing <-> Paused) -> PluginLoaded two times
	for(TInt i=2; i && KErrNone == iLastError; --i)
		{
		// start decoding
		iImageDisplay->Play();
		CActiveScheduler::Start();
		}
	if (KErrNone != iLastError && 
			!(iTestParams->iTestType==TImageDisplayUnitTestParams::EAlloc && KErrNoMemory == iLastError))
		{
		testStepResult = EFail;
		}
		
	iImageDisplay->StopPlay();
	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeFileByNameByInvalidPluginUid::CDecodeFileByNameByInvalidPluginUid()
 *
 * Test step constructor.
 */

CDecodeFileByNameByInvalidPluginUid::CDecodeFileByNameByInvalidPluginUid(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeFileByNameByInvalidPluginUid::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeFileByNameByInvalidPluginUid::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotFound == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeEmptyFileByNameNoExt::CDecodeEmptyFileByNameNoExt()
 *
 * Test step constructor.
 */

CDecodeEmptyFileByNameNoExt::CDecodeEmptyFileByNameNoExt(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeEmptyFileByNameNoExt::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeEmptyFileByNameNoExt::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeEmptyFileByName::CDecodeEmptyFileByName()
 *
 * Test step constructor.
 */

CDecodeEmptyFileByName::CDecodeEmptyFileByName(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeEmptyFileByName::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeEmptyFileByName::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeEmptyFileByMimeType::CDecodeEmptyFileByMimeType()
 *
 * Test step constructor.
 */

CDecodeEmptyFileByMimeType::CDecodeEmptyFileByMimeType(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeEmptyFileByMimeType::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeEmptyFileByMimeType::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeEmptyFileByImageType::CDecodeEmptyFileByImageType()
 *
 * Test step constructor.
 */

CDecodeEmptyFileByImageType::CDecodeEmptyFileByImageType(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeEmptyFileByImageType::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeEmptyFileByImageType::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeBadHdrFileByNameNoExt::CDecodeBadHdrFileByNameNoExt()
 *
 * Test step constructor.
 */

CDecodeBadHdrFileByNameNoExt::CDecodeBadHdrFileByNameNoExt(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeBadHdrFileByNameNoExt::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeBadHdrFileByNameNoExt::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeBadHdrFileByName::CDecodeBadHdrFileByName()
 *
 * Test step constructor.
 */

CDecodeBadHdrFileByName::CDecodeBadHdrFileByName(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeBadHdrFileByName::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeBadHdrFileByName::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeBadHdrFileByMimeType::CDecodeBadHdrFileByMimeType()
 *
 * Test step constructor.
 */

CDecodeBadHdrFileByMimeType::CDecodeBadHdrFileByMimeType(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeBadHdrFileByMimeType::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeBadHdrFileByMimeType::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeBadHdrFileByImageType::CDecodeBadHdrFileByImageType()
 *
 * Test step constructor.
 */

CDecodeBadHdrFileByImageType::CDecodeBadHdrFileByImageType(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeBadHdrFileByImageType::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeBadHdrFileByImageType::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeFileClippedNoClipPlugins::CDecodeFileClippedNoClipPlugins()
 *
 * Test step constructor.
 */

CDecodeFileClippedNoClipPlugins::CDecodeFileClippedNoClipPlugins(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeFileClippedNoClipPlugins::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeFileClippedNoClipPlugins::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotFound == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeFileClipCancelNoClipPlugins::CDecodeFileClipCancelNoClipPlugins()
 *
 * Test step constructor.
 */

CDecodeFileClipCancelNoClipPlugins::CDecodeFileClipCancelNoClipPlugins(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeFileClipCancelNoClipPlugins::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeFileClipCancelNoClipPlugins::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotFound == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}

/*
 * CDecodeNoExistFileByName::CDecodeNoExistFileByName()
 *
 * Test step constructor.
 */

CDecodeNoExistFileByName::CDecodeNoExistFileByName(const TImageDisplayUnitTestParams& aTestParams)
	: CTestStepImgDisplayGeneric(aTestParams)
	{
	}

/*
 * TVerdict CDecodeNoExistFileByName::TestL()
 *
 * Test step specific code.
 */

TVerdict CDecodeNoExistFileByName::TestL()
	{
	TVerdict testStepResult = EPass;	// assume success

	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);

	// Opened -> Configured
	SetImageSourceL();
// "normal" operation using the file name
	iImageDisplay->SetSizeInPixels(TSize(iTestParams->iDestWidth, iTestParams->iDestHeight), iTestParams->iMaintainAspectRatio);
	iImageDisplay->SetOptions(CImageDisplay::EOptionMainImage);

	TRAPD( err, iImageDisplay->SetupL() );
	if( KErrNotSupported == err )
		{
		INFO_PRINTF2(_L("Leave from SetupL() %d"),err);
		return testStepResult;
		}

	if (KErrNone != iLastError)
		{
		INFO_PRINTF2(_L("Error from SetupL() %d after reuse"),iLastError);
		testStepResult = EFail;
		}

	delete iImageDisplay;
	iImageDisplay = NULL;
	return testStepResult;
	}
