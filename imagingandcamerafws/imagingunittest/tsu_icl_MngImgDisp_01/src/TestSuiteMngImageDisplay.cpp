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
//

// EPOC includes
#include <bitdev.h>
// Test system includes
#include <testframework.h>
#include <icl/imagecodecdata.h>

#include "TestSuiteMngImageDisplay.h"

#include "TestStepMngImageDisplay.h"
#include "TestStepMngIclDecoder.h"

#define KJumpingBall	_S("simple_b.mng")
#define KClippingTestFileName _S("ForClippingFRAM1.mng")
#define KEmptyFile		_S("empty.mng")

#define KMimeTypeMng	_S8("image/mng")
#define KCAFBall	_S("z:\\Resource\\ForClippingFRAM1.mng")


// constant table of parameters for tests

#include "TermTestParameters.inl"
#include "FramTestParameters.inl"
#include "MagnTestParameters.inl"
#include "LoopTestParameters.inl"
#include "MiscNegativeTests.inl"
#include "AllocModeTestParameters.inl"
#include "IclDecodertestparameters.inl"
#include "misctests.inl"

const TImageDisplayUnitTestParams KSmokeFileNameTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0011-HP"), 
		_S("The MNG smoke test using filename (Red Jumping ball)"), 
		KJumpingBall,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		NULL									// checkpoints
	};

const TImageDisplayUnitTestParams KSmokeRotationTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0015-HP"), 
		_S("The MNG rotation 90cw smoke test using filename (Red Jumping ball)"), 
		KJumpingBall,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionRotateCw90,	// image option, 90cw rotation

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		NULL									// checkpoints
	};


const TImageDisplayUnitTestParams KClippingTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0060-HP"), 
		_S("The plug-in clipping ability test"),
		KClippingTestFileName,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		NULL									// checkpoints
	};


const TImageDisplayUnitTestParams KStopResponseTest  =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0400-HP"), 
		_S("MNG Stop response test"), 
		KJumpingBall,					// source file name
		50,  50, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage | CImageDisplay::EOptionMirrorVertical,		// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		4,											// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

const TImageDisplayUnitTestParams KPauseResponseTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0401-HP"), 
		_S("MNG Pause() response test"), 
		KJumpingBall,					// source file name
		50,  50, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage | CImageDisplay::EOptionMirrorVertical,		// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		4,											// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

const TImageDisplayUnitTestParams KEmtpyFileNegative =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0315-HP"), 
		_S("Empty file test"),
		KEmptyFile,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeMng,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		KErrEof							// expected error code
	};
	
const TImageDisplayUnitTestParams KPanicNotInit =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0501-HP"), 
		_S("EPanicNotInitialized panic test test"),
		KJumpingBall,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeMng,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		KErrEof							// expected error code
	};	

const TImageDisplayUnitTestParams KFileNameCAF =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0402-HP"), 
		_S("The MNG caf test using filename (Red Jumping ball)"), 
		KCAFBall,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		NULL									// checkpoints
	};


/**
*
* Create an instance of this test suite.
*
* NewTestSuiteMngImageDisplay is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteMngImageDisplay*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteMngImageDisplay* NewTestSuiteMngImageDisplay() 
    { 
	CTestSuiteMngImageDisplay* self = new (ELeave) CTestSuiteMngImageDisplay;
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteMngImageDisplay destructor
*
* 
*/
CTestSuiteMngImageDisplay::~CTestSuiteMngImageDisplay()
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
TPtrC CTestSuiteMngImageDisplay::GetVersion( ) const
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
void CTestSuiteMngImageDisplay::AddTestStepL(CTestStepMngImageDisplay* aPtrTestStep)
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
* inside CTestSuiteMngImageDisplay
* 
*/
void CTestSuiteMngImageDisplay::InitialiseL( )
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError( RFbsSession::Connect() );
	iFBSSession = ETrue;
	
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("MngImageDisplay");
	
	// smoke test
	AddTestStepL(new (ELeave) CTestStepImgDisplayGeneric(KSmokeFileNameTest));
	AddTestStepL(new (ELeave) CTestStepImgDisplayGeneric(KSmokeRotationTest));

	// TERMination tests
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm0Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm1Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm2Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_0_0_1_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_0_0_3_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_0_3_3_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_1_0_1_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_1_0_3_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_1_0_3_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_1_3_3_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_2_0_1_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_2_0_3_Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngTerm_3_2_3_3_Test));
	// FRAMing tests
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngFram1Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngFram2Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngFram3Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngFram4Test));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngVAR_FRAM1_TERM3_0_3_2));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngWITH_DEF_INTER_FRM_DELAY));
	// LOOP tests
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngLoop_9_1x2x2_Term3));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngLoop_9_6x2_5_4_1x2_Term3));
	// MAGN tests
	AddTestStepL(new (ELeave) CTestStepMagn(KMngMagn1_MX5_MY3, KMagn1_MX5_MY3RefCol, KMagn1_MX5_MY3RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMagn(KMngMagn2_MX4_MY8, KMagn2_MX8_MY4RefCol, KMagn2_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMagn(KMngMagn3_MX4_MY8, KMagn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));

	// Clipping test //
	AddTestStepL(new (ELeave) CTestStepClipping(KClippingTest));

// alloc tests // 
	AddTestStepL(new (ELeave) CTestStepImgDisplayGeneric(KAllocFileNameTest));
	AddTestStepL(new (ELeave) CTestStepMagn(KAllocMagnTest, KMagn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepImgDisplayGeneric(KAllocFileNameTestDbuf));
	AddTestStepL(new (ELeave) CTestStepMagn(KAllocMagnTestDbuf, KMagn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepImgDisplayGeneric(KAllocFileNameTestFh));
	AddTestStepL(new (ELeave) CTestStepMagn(KAllocMagnTestFh, KMagn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	
// StopPlay() response test  // 	
	AddTestStepL(new (ELeave) CTestStepStopResponse(KStopResponseTest));
	AddTestStepL(new (ELeave) CTestStepPauseResponse(KPauseResponseTest));
	
// Negative tests // 
	AddTestStepL(new (ELeave) CTestStepNegative(KMngMhdr_NegativeSize));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngMhdr_JngPresent));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngDefi_InvalidLength));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngDefi_FilppedRect));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngTerm_InvalidOptions));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngTerm_InvalidLength));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngBack_InvalidOptions));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngBack_InvalidLength));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngMagm_InvalidMethod));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngFram_InvalidOption));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngFram_InvalidNegativeDelay));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngFram_InvalidChangeOption));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngLoop_InvalidNestLevel));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngLoop_NoENDL));
	AddTestStepL(new (ELeave) CTestStepNegative(KMngEndl_WithoutLoop_Negative));
	AddTestStepL(new (ELeave) CTestStepNegative(KEmtpyFileNegative));
// Panic tests //
	AddTestStepL(new (ELeave) CTestStepPanicNotInit(KPanicNotInit));
// CAF Tests //	
    AddTestStepL(new (ELeave) CTestStepImgDisplayCAF(KFileNameCAF));
//Mng ICL decoder tests//
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm0Test, _L("MM-ICL-MNGICLDEC-U-0011-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm1Test, _L("MM-ICL-MNGICLDEC-U-0012-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm2Test, _L("MM-ICL-MNGICLDEC-U-0013-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_0_0_1_Test, _L("MM-ICL-MNGICLDEC-U-0014-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_0_0_3_Test, _L("MM-ICL-MNGICLDEC-U-0015-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_0_3_3_Test, _L("MM-ICL-MNGICLDEC-U-0016-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_1_0_1_Test, _L("MM-ICL-MNGICLDEC-U-0017-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_1_0_3_Test, _L("MM-ICL-MNGICLDEC-U-0018-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_1_3_3_Test, _L("MM-ICL-MNGICLDEC-U-0020-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_2_0_1_Test, _L("MM-ICL-MNGICLDEC-U-0021-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_2_0_3_Test, _L("MM-ICL-MNGICLDEC-U-0022-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_2_3_3_Test, _L("MM-ICL-MNGICLDEC-U-0023-HP")) );
	// FRAMing tests
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram1Test, _L("MM-ICL-MNGICLDEC-U-0024-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram2Test, _L("MM-ICL-MNGICLDEC-U-0025-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram3Test, _L("MM-ICL-MNGICLDEC-U-0026-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram4Test, _L("MM-ICL-MNGICLDEC-U-0027-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngVAR_FRAM1_TERM3_0_3_2, _L("MM-ICL-MNGICLDEC-U-0028-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngWITH_DEF_INTER_FRM_DELAY, _L("MM-ICL-MNGICLDEC-U-0029-HP")) );
	// LOOP tests
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngLoop_9_1x2x2_Term3, _L("MM-ICL-MNGICLDEC-U-0030-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngLoop_9_6x2_5_4_1x2_Term3, _L("MM-ICL-MNGICLDEC-U-0031-HP")) );

	// MAGN tests
	AddTestStepL(new (ELeave) CTestStepMngMagnCheck(KMngMagn1_MX5_MY3, _L("MM-ICL-MNGICLDEC-U-0041-HP"), KMagn1_MX5_MY3RefCol, KMagn1_MX5_MY3RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagnCheck(KMngMagn2_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0042-HP"), KMagn2_MX8_MY4RefCol, KMagn2_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagnCheck(KMngMagn3_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0043-HP"), KMagn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitSingleDestinationCheck(K32BitDestinationMngMagn1_MX5_MY3, _L("MM-ICL-MNGICLDEC-U-0045-HP"), EColor16MA, K16MA_Standalone_Magn1_MX5_MY3RefCol));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitSingleDestinationCheck(K32BitDestinationMngMagn2_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0046-HP"), EColor16MA, K16MA_Standalone_Magn2_MX8_MY4RefCol));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitSingleDestinationCheck(K32BitDestinationMngMagn3_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0047-HP"), EColor16MA, K16MA_Standalone_Magn3_MX8_MY4RefCol));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitDestinationAndMaskCheck(K32BitDestinationMngMagn1_MX5_MY3, _L("MM-ICL-MNGICLDEC-U-0048-HP"), EColor16MA, K32Bit_Destination_Magn1_MX5_MY3RefCol, KMagn1_MX5_MY3RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitDestinationAndMaskCheck(K32BitDestinationMngMagn2_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0049-HP"), EColor16MA, K32Bit_Destination_Magn2_MX8_MY4RefCol, KMagn2_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitDestinationAndMaskCheck(K32BitDestinationMngMagn3_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0050-HP"), EColor16MA, K32Bit_Destination_Magn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitSingleDestinationCheck(K32BitDestinationMngMagn1_MX5_MY3, _L("MM-ICL-MNGICLDEC-U-0051-HP"), EColor16MU, K32Bit_Destination_Magn1_MX5_MY3RefCol));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitSingleDestinationCheck(K32BitDestinationMngMagn2_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0052-HP"), EColor16MU, K32Bit_Destination_Magn2_MX8_MY4RefCol));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitSingleDestinationCheck(K32BitDestinationMngMagn3_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0053-HP"), EColor16MU, K32Bit_Destination_Magn3_MX8_MY4RefCol));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitDestinationAndMaskCheck(K32BitDestinationMngMagn1_MX5_MY3, _L("MM-ICL-MNGICLDEC-U-0054-HP"), EColor16MU, K32Bit_Destination_Magn1_MX5_MY3RefCol, KMagn1_MX5_MY3RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitDestinationAndMaskCheck(K32BitDestinationMngMagn2_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0055-HP"), EColor16MU, K32Bit_Destination_Magn2_MX8_MY4RefCol, KMagn2_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngMagn32BitDestinationAndMaskCheck(K32BitDestinationMngMagn3_MX4_MY8, _L("MM-ICL-MNGICLDEC-U-0056-HP"), EColor16MU, K32Bit_Destination_Magn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	
	// For PDEF122301: Test for MNG image with only Back and Fram chunks
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngBackFramTest, _L("MM-ICL-MNGICLDEC-U-0044-HP")));

	//MNG ICL DECODER TEST WITH MNG WITH NO LOOPS OPTION SET//
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_subframes_Test, _L("MM-ICL-MNGICLDEC-U-0222-HP"),ETrue) );
	
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm0Test, _L("MM-ICL-MNGICLDEC-U-0111-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm1Test, _L("MM-ICL-MNGICLDEC-U-0112-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_0_0_1_Test, _L("MM-ICL-MNGICLDEC-U-0114-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm_3_1_0_1_Test, _L("MM-ICL-MNGICLDEC-U-0117-HP"),ETrue) );
	// FRAMing tests
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram1Test, _L("MM-ICL-MNGICLDEC-U-0124-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram2Test, _L("MM-ICL-MNGICLDEC-U-0125-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram3Test, _L("MM-ICL-MNGICLDEC-U-0126-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngFram4Test, _L("MM-ICL-MNGICLDEC-U-0127-HP"),ETrue) );

// alloc tests // 
	
	AddTestStepL(new (ELeave) CTestStepMngMagnCheck(KAllocMagnTest,		_L("MM-ICL-MNGICLDEC-U-0141-HP"), KMagn3_MX8_MY4RefCol, KMagn3_MX8_MY4RefAlpha));
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngTerm0AllocTest, _L("MM-ICL-MNGICLDEC-U-0142-HP") ) );

// Negative tests // 
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngMhdr_NegativeSize_Icl, _L("MM-ICL-MNGICLDEC-U-0201-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngMhdr_JngPresent, _L("MM-ICL-MNGICLDEC-U-0202-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngDefi_InvalidLength, _L("MM-ICL-MNGICLDEC-U-0203-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngDefi_FilppedRect, _L("MM-ICL-MNGICLDEC-U-0204-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngTerm_InvalidOptions, _L("MM-ICL-MNGICLDEC-U-0205-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngTerm_InvalidLength, _L("MM-ICL-MNGICLDEC-U-0206-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngBack_InvalidOptions, _L("MM-ICL-MNGICLDEC-U-0207-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngBack_InvalidLength, _L("MM-ICL-MNGICLDEC-U-0208-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngMagm_InvalidMethod, _L("MM-ICL-MNGICLDEC-U-0209-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngFram_InvalidOption, _L("MM-ICL-MNGICLDEC-U-0210-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngFram_InvalidNegativeDelay, _L("MM-ICL-MNGICLDEC-U-0211-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngFram_InvalidChangeOption, _L("MM-ICL-MNGICLDEC-U-0212-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngLoop_InvalidNestLevel, _L("MM-ICL-MNGICLDEC-U-0213-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngLoop_NoENDL, _L("MM-ICL-MNGICLDEC-U-0214-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMngEndl_WithoutLoop_Negative, _L("MM-ICL-MNGICLDEC-U-0215-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KEmtpyFileNegativeIcl, _L("MM-ICL-MNGICLDEC-U-0216-HP")));
	// For PDEF122301: Negative test for an MNG image with a BASI chunk which is not supported with MNG-LC
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KBasiFileNegativeIcl, _L("MM-ICL-MNGICLDEC-U-0226-HP")));
	// For PDEF122149: Negative test for an MNG image with a MOVE and gAMA chunks which are not supported with MNG-LC
	AddTestStepL(new (ELeave) CTestStepMngIclNegative(KMoveGamaFileNegativeIcl, _L("MM-ICL-MNGICLDEC-U-0227-HP")));

// Cancel() response tests // 
	AddTestStepL(new (ELeave) CTestStepMngIclResponse(KMngTerm0IclResponse, _L("MM-ICL-MNGICLDEC-U-0251-HP")));
// defect fixe tests //	
	// DEF070863
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngAdvisoryBgTest));
	AddTestStepL(new (ELeave) CTestStepMngCheckpoint(KMngNoAnyBgTest));
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngAdvisoryBgTest, _L("MM-ICL-MNGICLDEC-U-0601-HP")));
	AddTestStepL(new (ELeave) CTestStepMngIclCheckpoint(KMngNoAnyBgTest, _L("MM-ICL-MNGICLDEC-U-0602-HP")));
	
	//DEF080823 
	AddTestStepL(new (ELeave) CTestStepMngIclCancelCheckpoint(KMngTerm_subframes_Cancel_Test, _L("MM-ICL-MNGICLDEC-U-0223-HP"),ETrue) );
	AddTestStepL(new (ELeave) CTestStepMngIclMimeType(KMngTerm_24bit_MimeType_Test, _L("MM-ICL-MNGICLDEC-U-0224-HP")) );
	AddTestStepL(new (ELeave) CTestStepMngIclMimeType(KMngTerm_24bit_MimeType_Test, _L("MM-ICL-MNGICLDEC-U-0225-HP")) );
	}
	
void CTestSuiteMngImageDisplay::DefaultPath(TFileName& aName)
	{
	aName.Copy(iDefaultPath);
	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteMngImageDisplay::InitSystemPath()
	{
	_LIT(KTestSourcePathOnF,"f:\\tsu_icl_mngimgdisp_01\\");
	_LIT(KTestSourcePathOnE,"e:\\tsu_icl_mngimgdisp_01\\");
	_LIT(KTestSourcePathOnC,"c:\\tsu_icl_mngimgdisp_01\\");
	_LIT(KTestSourcePathOnD,"d:\\tsu_icl_mngimgdisp_01\\");

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

