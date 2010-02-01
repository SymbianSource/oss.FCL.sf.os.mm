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

#include <bitdev.h>
// Test system includes
#include <testframework.h>
#include <icl/imagecodecdata.h>
#include <icl/mngimagedisplay.hrh>

#include "TestStepImageDisplay.h"
#include "TestSuiteImageDisplay.h"
#include "TestImageDisplay.hrh"

#include "IntTestStepImageDisplay.h"
#include "MngPITestStepImageDisplay.h"

#define KDummyImage _S("Dummy.idp")
#define KJpgImage _S("wolf.jpg")

#define KEmptyImageFile _S("empty")
#define KEmptyJpgImage _S("empty.jpg")

#define KBadHdrImageFile _S("corruptheader")
#define KBadHdrJpgImage _S("corruptheader.jpg")

#define KNoExistJpgImage _S("thisfiledoesnotexist.jpg")

#define KMimeTypeJpg _S8("image/jpeg")
#define KMimeTypePng _S8("image/png")

#define KMimeTypeDummy _S8("image/imgdisp")

#define KForPostScale _S("ForPostScale.mng")

// constant table of parameters for tests

// Note: this entry can be removed once fully debugged
const TImageDisplayUnitTestParams KSmokeFileNameTest =
	{	
		_S("mm-icl-imgdisp-U-0011-HP"), 
		_S("The API smoke test using filename"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeFileByNameByPluginUid =
	{
		_S("MM-ICL-IMGDISP-I-0001-CP"), 
		_S("Decode by name specifying Plugin Uid"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeFileByNameByInvalidPluginUid =
	{
		_S("MM-ICL-IMGDISP-I-0003-CP"),
		_S("Process by name specifying invalid Plugin Uid"), 
		KJpgImage,		// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
// TO DO: use invalid UID
		KTesPluginUidValue,// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeEmptyFileByNameNoExt =
	{	
		_S("MM-ICL-IMGDISP-I-0005-CP"), 
		_S("Process empty file specifying filename (no extension)"), 
		KEmptyImageFile,	// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeEmptyFileByName =
	{	
		_S("MM-ICL-IMGDISP-I-0006-CP"), 
		_S("Process empty file specifying filename"), 
		KEmptyJpgImage,		// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeEmptyFileByMimeType =
	{	
		_S("MM-ICL-IMGDISP-I-0007-CP"),
		_S("Process empty file specifying MIME-type"), 
		KEmptyJpgImage,		// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeJpg,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeEmptyFileByImageType =
	{	
		_S("MM-ICL-IMGDISP-I-0008-CP"), 
		_S("Process empty file specifying image-type"), 
		KEmptyJpgImage,		// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
// TO DO: specify JPG image type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeBadHdrFileByNameNoExt =
	{	
		_S("MM-ICL-IMGDISP-I-0009-CP"), 
		_S("Process corrupt (bad hdr) file specifying filename (no extension)"), 
		KBadHdrImageFile,	// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeBadHdrFileByName =
	{	
		_S("MM-ICL-IMGDISP-I-0010-CP"),
		_S("Process corrupt (bad hdr) file specifying filename"), 
		KBadHdrJpgImage,	// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeBadHdrFileByMimeType =
	{
		_S("MM-ICL-IMGDISP-I-0011-CP"),
		_S("Process corrupt (bad hdr) file specifying MIME-type"), 
		KBadHdrJpgImage,	// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeJpg,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeBadHdrFileByImageType =
	{	
		_S("MM-ICL-IMGDISP-I-0012-CP"), 
		_S("Process corrupt (bad hdr) file specifying image-type"), 
		KBadHdrJpgImage,	// source file name
		100, 100, 			// dest size

		-1,					// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeDummy,	// Mime type
// TO DO: specify JPG image type
		0, 0,			// image type & subtype
		0,				// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeFileClippedNoClipPlugins =
	{
		_S("MM-ICL-IMGDISP-I-0100-CP"), 
		_S("Clip by name with no clipping Plugins"), 
		KJpgImage,		// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KTesPluginUidValue,// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeFileClipCancelNoClipPlugins =
	{
		_S("MM-ICL-IMGDISP-I-0101-CP"), 
		_S("Clip/cancel by name with no clipping Plugins"), 
		KJpgImage,		// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KTesPluginUidValue,// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KDecodeNoExistFileByName =
	{
		_S("MM-ICL-IMGDISP-I-0200-CP"),
		_S("Process non-existent file by name"), 
		KNoExistJpgImage,	// source file name
		100, 100, 			// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KTesPluginUidValue,// plugin UID
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KScaleToRecommendedSize =
	{
		_S("MM-ICL-IMGDISP-I-0300-CP"),
		_S("Scale to recommended size"), 
		KForPostScale,	// source file name
		64, 64, 			// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KUidImageDisplayMngPluginImplUid,// 
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KScaleToNonRecommendedSize =
	{
		_S("MM-ICL-IMGDISP-I-0301-CP"),
		_S("Scale to non-recommended size"), 
		KForPostScale,	// source file name
		64, 64, 			// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KUidImageDisplayMngPluginImplUid,// 
		EFalse,			// continue onto next test step
		TImageDisplayUnitTestParams::ENormal			// test type
	};


/**
*
* Create an instance of this test suite.
*
* NewTestSuiteImageDisplayL is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteImageDisplay*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteImageDisplay* NewTestSuiteImageDisplayL() 
    { 
	CTestSuiteImageDisplay* self = new (ELeave) CTestSuiteImageDisplay;
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteImageDisplay destructor
*
* 
*/
CTestSuiteImageDisplay::~CTestSuiteImageDisplay()
	{
	if (iFBSSession)
		{
		iFBSSession = EFalse;
		RFbsSession::Disconnect();
		}
	}

/**
*
* This test suite version string
*
*
*/
_LIT(KTxtVersion,"1.0");

/**
*
* Get test suite version.
*
* @return	"TPtrC"
*			The version string.
*
*
*/
TPtrC CTestSuiteImageDisplay::GetVersion( ) const
	{
	return KTxtVersion();
	}

/**
*
* Add a test step into the suite.
*
* @param	"CTestStepImageTransform* aPtrTestStep"
*			The test step to be added.
* 
*/
void CTestSuiteImageDisplay::AddTestStepL(CTestStepImageDisplay* aPtrTestStep)
	{
    // [ assert pre condition that a valid test ptr has been supplied ]
	__ASSERT_ALWAYS(aPtrTestStep, Panic(EBadArgument));
	
	// [ set back wards reference to test suite ]
	aPtrTestStep->SetSuite(this); 
	
	//[ add the step to the test suite ]
	CTestSuite::AddTestStepL(aPtrTestStep);
	}

/**
*
* Initialiser for test suite.
* This creates all the test steps and stores them
* inside CTestSuiteImageDisplay
* 
*/
void CTestSuiteImageDisplay::InitialiseL( )
	{
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("ImageDisplay-I");
	
	// Add tests

	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KSmokeFileNameTest));

	AddTestStepL(new(ELeave) CDecodeFileByNameByPluginUid(KDecodeFileByNameByPluginUid));
	AddTestStepL(new(ELeave) CDecodeFileByNameByInvalidPluginUid(KDecodeFileByNameByInvalidPluginUid));
	AddTestStepL(new(ELeave) CDecodeEmptyFileByNameNoExt(KDecodeEmptyFileByNameNoExt));
	AddTestStepL(new(ELeave) CDecodeEmptyFileByName(KDecodeEmptyFileByName));
	AddTestStepL(new(ELeave) CDecodeEmptyFileByMimeType(KDecodeEmptyFileByMimeType));
	AddTestStepL(new(ELeave) CDecodeEmptyFileByImageType(KDecodeEmptyFileByImageType));
	AddTestStepL(new(ELeave) CDecodeBadHdrFileByNameNoExt(KDecodeBadHdrFileByNameNoExt));
	AddTestStepL(new(ELeave) CDecodeBadHdrFileByName(KDecodeBadHdrFileByName));
	AddTestStepL(new(ELeave) CDecodeBadHdrFileByMimeType(KDecodeBadHdrFileByMimeType));
	AddTestStepL(new(ELeave) CDecodeBadHdrFileByImageType(KDecodeBadHdrFileByImageType));
	AddTestStepL(new(ELeave) CDecodeFileClippedNoClipPlugins(KDecodeFileClippedNoClipPlugins));
	AddTestStepL(new(ELeave) CDecodeFileClipCancelNoClipPlugins(KDecodeFileClipCancelNoClipPlugins));
	AddTestStepL(new(ELeave) CDecodeNoExistFileByName(KDecodeNoExistFileByName));
	// MNG tests
	AddTestStepL(new(ELeave) CScaleToRecommendedSize(KScaleToRecommendedSize));
	AddTestStepL(new(ELeave) CScaleToNonRecommendedSize(KScaleToNonRecommendedSize));
	}

void CTestSuiteImageDisplay::DefaultPath(TFileName& aName)
	{
	aName.Copy(iDefaultPath);
	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteImageDisplay::InitSystemPath()
	{
	_LIT(KTestSourcePathOnE,"e:\\tsi_icl_imgdisp_01\\");
	_LIT(KTestSourcePathOnD,"d:\\tsi_icl_imgdisp_01\\");
	_LIT(KTestSourcePathOnC,"c:\\tsi_icl_imgdisp_01\\");

	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	//[ take the first drive where it exists or can be made ]
	TInt err = fs.MkDir(KTestSourcePathOnC);
	if (( err == KErrNone ) || (err == KErrAlreadyExists))
		{
		iDefaultPath.Copy(KTestSourcePathOnC);
		}
	else{
        TInt err = fs.MkDir(KTestSourcePathOnD);
        if (( err == KErrNone ) || (err == KErrAlreadyExists))
			{
			iDefaultPath.Copy(KTestSourcePathOnD);
			}
		else
			{
			TInt err = fs.MkDir(KTestSourcePathOnE);
			if (( err == KErrNone ) || (err == KErrAlreadyExists))
				{
				iDefaultPath.Copy(KTestSourcePathOnE);
				}
			else
				{
				User::Panic(_L("Could not make TestTransform directory"),KErrGeneral);
				}			
			}
		}


	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));
	fs.Close();
	}
