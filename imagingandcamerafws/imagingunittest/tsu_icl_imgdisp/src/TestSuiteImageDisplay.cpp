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
#include <icl/exifimagedisplay.hrh>

#include "TestStepImageDisplay.h"
#include "TestSuiteImageDisplay.h"
#include "TestPlugin/TestImageDisplay.hrh"

#define KDummyImage _S("Dummy.idp")
#define KJpgImage _S("wolf.jpg")
#define KMngImage 		_S("simple_b.mng")
#define KMngEmptyFile 	_S("empty.mng")
#define KDrmArchive _S("ImgDispl.content")

#define KMimeTypeJpg _S8("image/jpeg")
#define KMimeTypeMng _S8("image/mng")
#define KMimeTypeDummy _S8("image/imgdisp")
#define KMngUnlimUid _S("ImgDisplMngUnlim")
#define KJpgUnlimUid _S("ImgDisplJpgUnlimited")

#define KImageTypeJPGUidValue 0x101F45D8

// constant table of parameters for tests

const TImageDisplayUnitTestParams KSmokeFileNameTest =
	{	
		_S("MM-ICL-IMGDISP-U-0011-HP"), 
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
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};


const TImageDisplayUnitTestParams KSmokeFileHandleTest =
	{	
		_S("MM-ICL-IMGDISP-U-0012-HP"), 
		_S("The API smoke test using filehandle"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KSmokeDataBufTest =
	{	
		_S("MM-ICL-IMGDISP-U-0013-HP"), 
		_S("The API smoke test using DataBuffer"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a databuf
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KForceUidTest =
	{	
		_S("MM-ICL-IMGDISP-U-0020-HP"), 
		_S("Forcing the Plugin Uid test"), 
		KJpgImage,		// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		KTesPluginUidValue,// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams K2FramesTest =
	{	
		_S("MM-ICL-IMGDISP-U-0021-HP"), 
		_S("Compare generated frame"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

//*****************************************************
// ALLOC tests
//*****************************************************

const TImageDisplayUnitTestParams KAllocSmokeFileNameTest =
	{	
		_S("MM-ICL-IMGDISP-U-0101-HP"), 
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
 
		TImageDisplayUnitTestParams::EAlloc			// test type
	};


const TImageDisplayUnitTestParams KAllocSmokeFileHandleTest =
	{	
		_S("MM-ICL-IMGDISP-U-0102-HP"), 
		_S("The API smoke test using filehandle"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::EAlloc			// test type
	};

const TImageDisplayUnitTestParams KAllocSmokeDataBufTest =
	{	
		_S("MM-ICL-IMGDISP-U-0103-HP"), 
		_S("The API smoke test using DataBuffer"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a databuf
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::EAlloc			// test type
	};


const TImageDisplayUnitTestParams KPanicTest_1 =
	{	
		_S("MM-ICL-IMGDISP-U-0001-HP"), 
		_S("The API Panic test 1 (EIllegalCallSequence)"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a databuf
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KPanicTest_2 =
	{	
		_S("MM-ICL-IMGDISP-U-0002-HP"), 
		_S("The API Panic test 2 (EUndefinedSourceType)"), 
		NULL,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeUndefined,	// Source is undefined
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KPanicTest_3 =
	{	
		_S("MM-ICL-IMGDISP-U-0003-HP"), 
		_S("The API Panic test 3 (EUndefinedDestSize)"), 
		KDummyImage,	// source file name
		-1, -1, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a databuf
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KPanicTest_4 =
	{	
		_S("MM-ICL-IMGDISP-U-0004-HP"), 
		_S("The API Panic test 4 (EImageWrongType)"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a databuf
		KMimeTypeDummy,	// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

const TImageDisplayUnitTestParams KPanicTest_6 =
	{	
		_S("MM-ICL-IMGDISP-U-0006-HP"), 
		_S("The API Panic test 6 (EIllegalImageSubType)"), 
		KDummyImage,	// source file name
		100, 100, 		// dest size

		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0x10002344,	// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal			// test type
	};

// DRM negative tests // 
const TText* const KNoRightsUids[]={_S("ImgDisplNoRights"),NULL};
const TImageDisplayUnitTestParams KDrmNoRightsTst = 
	{	
		_S("MM-ICL-IMGDISP-U-0251-HP"), 
		_S("Open a protected  image having NO rights test"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KNoRightsUids,		// Unique Id
		ContentAccess::EView,		// Intent
		KErrCANoRights				// expected error
	};
	
const TText* const KUnlimitedUids[]={KMngUnlimUid, KJpgUnlimUid, NULL};
const TImageDisplayUnitTestParams KDrmNoPermTst =
	{	
		_S("MM-ICL-IMGDISP-U-0252-HP"), 
		_S("Open a protected image with Unknown intent"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedUids,		// Unique Id
		ContentAccess::EUnknown,	// Intent
		KErrCANoPermission				// expected error
	};
	
const TText* const KPlayTwiceUids[]={_S("ImgDisplJpgCnt01"), _S("ImgDisplMngCnt01"), NULL};
const TImageDisplayUnitTestParams KDrmLimitedCountTst =
	{	
		_S("MM-ICL-IMGDISP-U-0253-HP"), 
		_S("Open a protected image with View intent limited to 1 attempt, decode 2 times"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KPlayTwiceUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrCANoPermission,				// expected error
		2						// iPlayLoops
	};
	
const TText* const KUnlimitedJpgUids[]={KJpgUnlimUid, NULL};
const TImageDisplayUnitTestParams KDrmMimeTypeFileNameTest =
	{	
		_S("MM-ICL-IMGDISP-U-0201-HP"), 
		_S("Open a protected image with View specifying MIME type, Fname"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeJpg,		// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedJpgUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};
	
const TImageDisplayUnitTestParams KDrmMimeTypeFileHandleTest =
	{	
		_S("MM-ICL-IMGDISP-U-0202-HP"), 
		_S("Open a protected image with View specifying MIME type, Fhandle"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		KMimeTypeJpg,		// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedJpgUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};
	
const TText* const KUnlimitedMngUids[]={KMngUnlimUid, NULL};
const TImageDisplayUnitTestParams KDrmPluginUidTypeFileNameTest =
	{	
		_S("MM-ICL-IMGDISP-U-0203-HP"), 
		_S("Open a protected image with View specifying plug-in Uid, Fname"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,		// Mime type
		0, 0,			// image type & subtype
		KUidImageDisplayMngPluginImplUid,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedMngUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};
	
const TImageDisplayUnitTestParams KDrmPluginUidTypeFileHandleTest =
	{	
		_S("MM-ICL-IMGDISP-U-0204-HP"), 
		_S("Open a protected image with View specifying plug-in Uid, Fhandle"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		NULL,		// Mime type
		0, 0,			// image type & subtype
		KUidImageDisplayMngPluginImplUid,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedMngUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};
	
const TImageDisplayUnitTestParams KDrmImgTypeUidTypeFileNameTest =
	{	
		_S("MM-ICL-IMGDISP-U-0205-HP"), 
		_S("Open a protected image with View specifying image type Uid, Fname"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,		// Mime type
		KImageTypeJPGUidValue, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedJpgUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};

const TImageDisplayUnitTestParams KDrmImgTypeUidTypeFileHandleTest =
	{	
		_S("MM-ICL-IMGDISP-U-0206-HP"), 
		_S("Open a protected image with View specifying  image type Uid, Fhandle"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		NULL,		// Mime type
		KImageTypeJPGUidValue, 0,			// image type & subtype
		0,				// plugin UID
 
		TImageDisplayUnitTestParams::ENormal,			// test type
		KUnlimitedJpgUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};	

const TImageDisplayUnitTestParams KDrmAllocFileNameTest =
	{	
		_S("MM-ICL-IMGDISP-U-0111-HP"), 
		_S("Open a protected image with View, Alloc, Fname"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filehandle
		NULL,		// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 		TImageDisplayUnitTestParams::EAlloc,			// test type
		KUnlimitedMngUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};
	
const TImageDisplayUnitTestParams KDrmAllocFileHandleTest =
	{	
		_S("MM-ICL-IMGDISP-U-0112-HP"), 
		_S("Open a protected image with View, Alloc, Fhandle"), 
		KDrmArchive,	// source file name
		0, 0, 		// dest size
		-1,	// display mode
		CImageDisplay::EOptionMainImage,	// image option
		ETrue,			// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		NULL,		// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
 		TImageDisplayUnitTestParams::EAlloc,			// test type
		KUnlimitedJpgUids,		// Unique Id
		ContentAccess::EView,	// Intent
		KErrNone,				// expected error
		1						// iPlayLoops
	};	

const TMimeTypeRecogTestParam KMngRecogTestFileName = 
	{
	_S("MM-ICL-IMGDISP-U-0301-HP"), 				// name of the test
	_S("Open a MNG file and get its MIME type"),	// description
	TMimeTypeRecogTestParam::EParamIsFileName,		// what next parameter to use
	NULL,											// databuffer parameter value
	KMngImage,										// filename parameter value
	KMimeTypeMng,									// expected MIME type value
	CApaDataRecognizerType::EPossible				// minimum confidence
	};
	
const TMimeTypeRecogTestParam KMngRecogTestDataBuf = 
	{
	_S("MM-ICL-IMGDISP-U-0302-HP"), 				// name of the test
	_S("Open a MNG file and get its MIME type using its content"),	// description
	TMimeTypeRecogTestParam::EParamIsDataBuffer,	// what next parameter to use
	NULL,											// databuffer parameter value
	KMngImage,										// filename parameter value
	KMimeTypeMng,									// expected MIME type value
	CApaDataRecognizerType::EPossible				// minimum confidence
	};
	
const TMimeTypeRecogTestParam KMngRecogTestEmptyFile = 
	{
	_S("MM-ICL-IMGDISP-U-0303-HP"), 				// name of the test
	_S("Open an empty MNG file and get its MIME type"),	// description
	TMimeTypeRecogTestParam::EParamIsFileName,		// what next parameter to use
	NULL,											// databuffer parameter value
	KMngEmptyFile,									// filename parameter value
	KMimeTypeMng,									// expected MIME type value
	CApaDataRecognizerType::EPossible				// minimum confidence
	};
	
const TMimeTypeRecogTestParam KMngRecogIncompleteSign = 
	{
	_S("MM-ICL-IMGDISP-U-0304-HP"), 				// name of the test
	_S("Use the MNG incomplete signature and try to get its MIME type"),	// description
	TMimeTypeRecogTestParam::EParamIsDataBuffer,	// what next parameter to use
	_S8("\x8aMNG"),									// databuffer parameter value 
	_S("just_something"),							// filename parameter value
	_S8(""),											// expected MIME type value
	CApaDataRecognizerType::ENotRecognized				// minimum confidence
	};

const TMimeTypeRecogTestParam KDummyRecogTestFileName = 
	{
	_S("MM-ICL-IMGDISP-U-0305-HP"), 				// name of the test
	_S("Open a IDP dummy format file and get its MIME type"),	// description
	TMimeTypeRecogTestParam::EParamIsFileName,		// what next parameter to use
	NULL,											// databuffer parameter value
	KDummyImage,										// filename parameter value
	KMimeTypeDummy,									// expected MIME type value
	CApaDataRecognizerType::EPossible				// minimum confidence
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
	iFBSSession = ETrue;
	
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("ImageDisplay");
	
	// Add tests


	// smoke test
	AddTestStepL(new (ELeave) CTestStepImgDisplayGeneric(KSmokeFileNameTest));
	
	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KSmokeFileHandleTest));
	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KSmokeDataBufTest));
	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KForceUidTest));
	AddTestStepL(new(ELeave) CTestStepDecode2Frames(K2FramesTest));

// alloc tests // 
	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KAllocSmokeFileNameTest));
	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KAllocSmokeFileHandleTest));
	AddTestStepL(new(ELeave) CTestStepImgDisplayGeneric(KAllocSmokeDataBufTest));

// Panic tests // 
	AddTestStepL(new(ELeave) CTestStepPanic_1(KPanicTest_1));
	AddTestStepL(new(ELeave) CTestStepPanic_2(KPanicTest_2));
	AddTestStepL(new(ELeave) CTestStepPanic_3(KPanicTest_3));
	AddTestStepL(new(ELeave) CTestStepPanic_4(KPanicTest_4));
	AddTestStepL(new(ELeave) CTestStepPanic_6(KPanicTest_6));
	
// Drm positive tests // 
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmMimeTypeFileNameTest));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmMimeTypeFileHandleTest));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmPluginUidTypeFileNameTest));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmPluginUidTypeFileHandleTest));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmImgTypeUidTypeFileNameTest));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmImgTypeUidTypeFileHandleTest));
	
// Drm negative tests // 
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmNoRightsTst));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmNoPermTst));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmLimitedCountTst));
	
// Drm alloc tests // 	
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmAllocFileNameTest));
	AddTestStepL(new(ELeave) CTestStepDrm(KDrmAllocFileHandleTest));
// Mime type recognition tests // 
	AddTestStepL(new(ELeave) CTestStepImageDisplayRecog(KMngRecogTestFileName));
	AddTestStepL(new(ELeave) CTestStepImageDisplayRecog(KMngRecogTestDataBuf));
	AddTestStepL(new(ELeave) CTestStepImageDisplayRecog(KMngRecogTestEmptyFile));
	AddTestStepL(new(ELeave) CTestStepImageDisplayRecog(KMngRecogIncompleteSign));
	AddTestStepL(new(ELeave) CTestStepImageDisplayRecog(KDummyRecogTestFileName));
	
// Regression test case for defect: DEF082108: Browsing to www.mobil.se will crash the phone
	AddTestStepL(new(ELeave) CTestStepImageDisplayMimeCheck());
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
	_LIT(KTestSourcePathOnE,"e:\\tsu_icl_imgdisp_01\\");
	_LIT(KTestSourcePathOnD,"d:\\tsu_icl_imgdisp_01\\");
	_LIT(KTestSourcePathOnC,"c:\\tsu_icl_imgdisp_01\\");

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
