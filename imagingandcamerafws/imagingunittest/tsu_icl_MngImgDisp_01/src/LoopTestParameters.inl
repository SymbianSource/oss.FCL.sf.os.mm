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

#define KLoop_9_1x2x2_Term3	_S("LOOP-9-1x2x2-TERM3.mng")
#define KLoop_9_6x2_5_4_1x2_Term3 _S("LOOP-9-6x2-5-4-1x2-TERM3.mng")

const TFrameCheckPoint KMngLoop_9_1x2x2_Term3TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
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
			KOneSecond/2,								// delay from the previuos frame
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
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
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
			KOneSecond/2,							// delay from the previuos frame
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
			KFrameReadyStatus				// plug-in status 
		},
		{
			19,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			26,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			27,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			28,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			35,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			36,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			37,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			38,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond+KOneSecond/2,				// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			45,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			46,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			47,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			48,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			54,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			55,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			56,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			71,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			72,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			73,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			74,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};


const TFrameCheckPoint KMngLoop_9_6x2_5_4_1x2_Term3TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
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
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond/2,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			6,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			7,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			8,										// absolute frame number
			_S("digit_6.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_5.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_4.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			13,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			14,										// absolute frame number
			_S("digit_4.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			15,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			16,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			17,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			18,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus				// plug-in status 
		},
		{
			19,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond+KOneSecond/2,			// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20, 									// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			21, 									// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			22,										// absolute frame number
			_S("digit_6.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			23,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond/2,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			24,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			25,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			26,										// absolute frame number
			_S("digit_6.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			27,										// absolute frame number
			_S("digit_5.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			28,										// absolute frame number
			_S("digit_4.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,		// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			31,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			32,										// absolute frame number
			_S("digit_4.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			33,										// absolute frame number
			_S("digit_3.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			34,										// absolute frame number
			_S("digit_2.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			35,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			36,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond/2,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};


const TImageDisplayUnitTestParams KMngLoop_9_1x2x2_Term3 =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0050-HP"), 
		_S("MNG LOOP-9-1x2x2 with TERM3 method using 9-0 countdown.Repeat 2x2 times with 1/2 second delay"), 
		KLoop_9_1x2x2_Term3,					// source file name
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
		KMngLoop_9_1x2x2_Term3TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngLoop_9_6x2_5_4_1x2_Term3 =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0051-HP"), 
		_S("MNG LOOP 9-6x2-5-4-1x2 with TERM3 method using 9-6x2 5 4-1x2 0 countdown.Repeat 2 times with 1/2 second delay"), 
		KLoop_9_6x2_5_4_1x2_Term3,					// source file name
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
		KMngLoop_9_6x2_5_4_1x2_Term3TestPoints		// checkpoints. 
	};

