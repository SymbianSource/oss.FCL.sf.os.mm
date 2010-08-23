// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TESTSUITEEXIFGIMAGEDISPLAY.cpp
// 
//

// EPOC includes
#include <bitdev.h>
// Test system includes
#include <testframework.h>
#include <icl/imagecodecdata.h>
#include <icl/geniclimagedisplay.hrh>
#include <bitmaptransforms.h>

#include "TestStepExifImageDisplay.h"

#include "TestSuiteExifImageDisplay.h"

#include "TeststepGenIclimagedisplay.h"

#define KMimeTypeExif	_S8("image/jpeg")

#define KSmokeTestFile 		_S("clip_test.jpeg")
#define KSmokeRefMainFr2	_S("clip_frame_2.mbm")
#define KSmokeRefMainFinal	_S("clip_frame_final.mbm")

// constant table of parameters for tests

#include "AutoRotateTestParameters.inl"
#include "Responsetestparams.inl"
#include "AllocExifImgDispParams.inl"
#include "Negativetestparams.inl"
#include "GenericIclTestParams.inl"

const TFrameCheckPoint KExifSmokeTest_TestPoints[]=
	{
		{
		2,										// absolute frame number
		KSmokeRefMainFr2,						// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KPartialFrameReadyStatus				// plug-in status 
		},
		{
		3,										// absolute frame number
		KSmokeRefMainFinal,						// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},		
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KSmokeFileNameTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0001-HP"), 
		_S("The Exif smoke test using filename (clip, rotate 90, scale*0.5)"), 
		KSmokeTestFile,								// source file name
		129, 173, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionRotateCw90,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifSmokeTest_TestPoints,						// checkpoints
		KQuaterPercent
	};



/**
*
* Create an instance of this test suite.
*
* NewTestSuiteExifImageDisplay is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteExifImageDisplay*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteExifImageDisplay* NewTestSuiteExifImageDisplay() 
    { 
	CTestSuiteExifImageDisplay* self = new (ELeave) CTestSuiteExifImageDisplay;
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteExifImageDisplay destructor
*
* 
*/
CTestSuiteExifImageDisplay::~CTestSuiteExifImageDisplay()
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
TPtrC CTestSuiteExifImageDisplay::GetVersion( ) const
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
void CTestSuiteExifImageDisplay::AddTestStepL(CTestStepIclImageDisplay* aPtrTestStep)
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
* inside CTestSuiteExifImageDisplay
* 
*/
void CTestSuiteExifImageDisplay::InitialiseL( )
	{
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError( RFbsSession::Connect() );
	iFBSSession = ETrue;
	
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("ExifImageDisplay");
	
	// smoke test
	AddTestStepL(new (ELeave) CTestStepExifGeneric(KSmokeFileNameTest));

// autoroatation tests//
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot1_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot2_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot3_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot4_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot5_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot6_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot7_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot8_Test));

	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot1Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot2Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot3Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot4Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot5Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot6Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot7Thumb_Test));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KExifAutoRot8Thumb_Test));

// alloc tests // 
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KAllocFileNameTest));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KAllocFileHandleTest));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KAllocDataBufTest));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KAllocThumbFileNameTest));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KAllocThumbFileHandleTest));
	AddTestStepL(new (ELeave) CTestStepWithCheckpoint(KAllocThumbDataBufTest));

// Pause()/StopPlay() response test  // 	
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KExifAutoRot1Pause_Test));
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KExifAutoRot7Pause_Test));
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KExifAutoRot7ThumbPause_Test));
	AddTestStepL(new (ELeave) CTestStepStopResponse(KStopResponseTest));
	
// Negative tests // 
	AddTestStepL(new (ELeave) CTestStepNegative(KEmptyFileTest));
	AddTestStepL(new (ELeave) CTestStepNegativeReduction(KWrongReductionTest1));
	AddTestStepL(new (ELeave) CTestStepBigReduction(KWrongReductionTest2));
	AddTestStepL(new (ELeave) CTestStepNegative(KWrongImageOptions));	
	AddTestStepL(new (ELeave) CTestStepUnsupExt(KUnsupExtension));		
// Panic tests //

	AddTestStepL(new (ELeave) CTestStepPanicNotInit(KPanicNotInit));
	AddTestStepL(new (ELeave) CTestStepPanicInitFailed(KPanicInitFailed));

// Generic plug-in tests //
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KIcoSmokeTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KBmpSmokeTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KTransGif1Test));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KTransGif8Test));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KTif3_4Test));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KJfif444Test));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KClockTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KOtaTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KPng4_8Test));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KWbmpTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KWmf3_8Test));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KWmfWithMaskTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KGifAnimatedCatTest));
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KPngWithAlphaTest));

// Generic plug-in Alloc tests //
	AddTestStepL(new (ELeave) CTestStepIclWrapperGeneric(KGifAnimatedCatAllocTest));
	
// Generic plug-in Pause()/StopPlay() response test  // 	
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KGifAnimatedPauseResponseTest));
	AddTestStepL(new (ELeave) CTestStepStopResponse(KGifAnimatedStopResponseTest));
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KGifAnimatedPostProcPauseResponseTest));
	AddTestStepL(new (ELeave) CTestStepStopResponse(KGifAnimatedPostProcStopResponseTest));	
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KGifAnimatedPostProcPauseResponseTest2x));
	AddTestStepL(new (ELeave) CTestStepStopResponse(KGifAnimatedPostProcStopResponseTest2x));		
	}
	
void CTestSuiteExifImageDisplay::DefaultPath(TFileName& aName)
	{
	aName.Copy(iDefaultPath);
	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteExifImageDisplay::InitSystemPath()
	{
	_LIT(KTestSourcePathOnF,"f:\\tsu_icl_exifimgdisp_01\\");
	_LIT(KTestSourcePathOnE,"e:\\tsu_icl_exifimgdisp_01\\");
	_LIT(KTestSourcePathOnC,"c:\\tsu_icl_exifimgdisp_01\\");
	_LIT(KTestSourcePathOnD,"d:\\tsu_icl_exifimgdisp_01\\");

	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	//[ take the first drive where it exists or can be made ]
	TInt err = fs.MkDir(KTestSourcePathOnC);
	if (( err == KErrNone ) || (err == KErrAlreadyExists))
		{
		iDefaultPath.Copy(KTestSourcePathOnC);
		}
	else 
		{
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
				TInt err = fs.MkDir(KTestSourcePathOnF);
				if (( err == KErrNone ) || (err == KErrAlreadyExists))
					{
					iDefaultPath.Copy(KTestSourcePathOnF);
					}				
				else
					{
					User::Panic(_L("Could not make Test directory"),KErrGeneral);
					}
				}
			}
		}


	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));
	fs.Close();
	}
