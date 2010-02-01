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

#define KFram1_Countdown _S("FRAM1.mng")
#define KFram2_Countdown _S("FRAM2.mng")
#define KFram3_Countdown _S("FRAM3.mng")
#define KFram4_Countdown _S("FRAM4.mng")
#define VAR_FRAM1_TERM3_0_3_2 _S("VAR-FRAM1-TERM3-0-3-2.mng")
#define DEF_INT_FRAME_DELAY_SUB_OPT_2 _S("DEF_INT_FRAME_DELAY_SUB_OPT_2.mng")
#define KBackFram1 _S("flag3col.mng")

const TFrameCheckPoint KMngFram1_TestPoints[]={
		{
			1,										// absolute frame number
			_S("fram1_1.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("fram1_2.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("fram1_3.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			4,										// absolute frame number
			_S("fram1_4.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			5,										// absolute frame number
			_S("fram1_5.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			6,										// absolute frame number
			_S("fram1_6.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			7,										// absolute frame number
			_S("fram1_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			8,										// absolute frame number
			_S("fram1_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("fram1_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KLastFrameReadyStatus					// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngFram2_TestPoints[]={
		{
			1,										// absolute frame number
			_S("fram1_3.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("fram1_6.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,								// absolute frame number  - Defect::DEF073809
			_S("fram1_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KLastFrameReadyStatus					// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngFram3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("fram3_black.mbm"),					// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("fram1_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("fram3_3.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			4,										// absolute frame number
			_S("fram3_4.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			5,										// absolute frame number
			_S("fram3_black.mbm"),					// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			6,										// absolute frame number
			_S("fram3_6.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			7,										// absolute frame number
			_S("fram3_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			8,										// absolute frame number
			_S("fram3_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("fram3_black.mbm"),					// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("fram3_10.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("fram3_11.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("fram3_12.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KLastFrameReadyStatus					// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngFram4_TestPoints[]={
		{
			1,										// absolute frame number
			_S("fram3_black.mbm"),					// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("fram1_3.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("fram3_black.mbm"),					// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			4,										// absolute frame number
			_S("fram4_4.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			5,										// absolute frame number
			_S("fram3_black.mbm"),					// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			6, 										// absolute frame number
			_S("fram4_6.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KLastFrameReadyStatus					// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KVar_Fram1_Term3_0_3_2_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			4,										// absolute frame number
			_S("digit_6.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			5,										// absolute frame number
			_S("digit_5.mbm"),						// reference bitmap name
			3*KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			6,										// absolute frame number
			_S("digit_4.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			7,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			8,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			3*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			13,										// absolute frame number
			_S("digit_6.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			14,										// absolute frame number
			_S("digit_5.mbm"),						// reference bitmap name
			3*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			15,										// absolute frame number
			_S("digit_4.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			16,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			17,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			18,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};



const TFrameCheckPoint KMng_with_interframe_delay_TestPoints[]={
		{
			1,										// absolute frame number
			NULL,									// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			4,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			5,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			6,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			7,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			8,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			13,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			14,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			15,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			16,										// absolute frame number
			NULL,									// reference bitmap name
			200000,									// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KLastFrameReadyStatus					// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngBackFram_TestPoints[]={
		{
			1,										// absolute frame number
			_S("flag3col.mbm"),						// reference bitmap name
			0,										// no delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KLastFrameReadyStatus					// plug-in status 
		},
	ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KMngFram1Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0030-HP"), 
		_S("MNG FRAM 1 method using 9-0 countdown.9 frames with 1 second delay"), 
		KFram1_Countdown,					// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		KMngFram1_TestPoints					// checkpoints. 
	};


const TImageDisplayUnitTestParams KMngFram2Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0031-HP"), 
		_S("MNG FRAM 2 method using 9-0 countdown.3 frames with 1 second delay"), 
		KFram2_Countdown,					// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		KMngFram2_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngFram3Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0032-HP"), 
		_S("MNG FRAM 3 method using 9-0 countdown.12 frames with 1 second delay"), 
		KFram3_Countdown,					// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		KMngFram3_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngFram4Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0033-HP"), 
		_S("MNG FRAM 4 method using 9-0 countdown. 6 frames with 1 second delay"), 
		KFram4_Countdown,					// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		KMngFram4_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngVAR_FRAM1_TERM3_0_3_2 = 
	{	
		_S("MM-ICL-MNGIMGDISP-U-0034-HP"), 
		_S("MNG FRAM 1 with TERM3 method using 9-0 countdown. 18 frames with 3 or 1/2 second delay"), 
		VAR_FRAM1_TERM3_0_3_2,					// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type	
		KVar_Fram1_Term3_0_3_2_TestPoints			// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngWITH_DEF_INTER_FRM_DELAY = 
	{	
		_S("MM-ICL-MNGIMGDISP-U-0035-HP"), 
		_S("MNG Image with defined Interframe Delay of 200 with SubOption ForUpcomingSubFrameOnly "), 
		DEF_INT_FRAME_DELAY_SUB_OPT_2,				// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		KMng_with_interframe_delay_TestPoints		// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngBackFramTest = 
	{	
		_S("MM-ICL-MNGIMGDISP-U-0044-HP"), 
		_S("MNG Image with only Back and Fram chunks "), 
		KBackFram1,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		KMngBackFram_TestPoints						// checkpoints. 
	};
