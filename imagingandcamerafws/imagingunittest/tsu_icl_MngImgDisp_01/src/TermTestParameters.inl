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

#define KTerm0Countdown			_S("MNGTERM0.mng")
#define KTerm1Countdown			_S("MNGTERM1.mng")
#define KTerm2Countdown			_S("MNGTERM2.mng")

#define KTerm_3_0_0_1_Countdown	_S("MNGTERM3-0-0-1.mng")
#define KTerm_3_0_0_3_Countdown	_S("MNGTERM3-0-0-3.mng")
#define KTerm_3_0_3_3_Countdown	_S("MNGTERM3-0-3-3.mng")
#define KTerm_3_1_0_1_Countdown	_S("MNGTERM3-1-0-1.mng")
#define KTerm_3_1_0_3_Countdown	_S("MNGTERM3-1-0-3.mng")
#define KTerm_3_1_3_3_Countdown	_S("MNGTERM3-1-3-3.mng")
#define KTerm_3_2_0_1_Countdown	_S("MNGTERM3-2-0-1.mng")
#define KTerm_3_2_0_3_Countdown	_S("MNGTERM3-2-0-3.mng")
#define KTerm_3_2_3_3_Countdown	_S("MNGTERM3-2-3-3.mng")
#define KTerm_Subframe      	_S("subframes.mng")
#define KTerm_24BitMng      	_S("24bit.mng")
#define KBasiFile		_S("basi.mng")
#define KMoveGamaFile		_S("movegama.mng")

const TFrameCheckPoint KMngTerm0TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm1TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedEraseStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm2TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm_3_0_0_3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm_3_0_3_3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			21,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm_3_1_0_3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			21,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedEraseStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm_3_1_3_3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			21,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedEraseStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};

const TFrameCheckPoint KMngTerm_3_2_0_3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			21,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			31,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};


const TFrameCheckPoint KMngTerm_3_2_3_3_TestPoints[]={
		{
			1,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking		// plug-in status mask
		},
		{
			2,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("digit_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			9,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			10,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			11,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			12,										// absolute frame number
			_S("digit_8.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			20,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			21,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			29,										// absolute frame number
			_S("digit_1.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			30,										// absolute frame number
			_S("digit_0.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus						// plug-in status 
		},
		{
			31,										// absolute frame number
			_S("digit_9.mbm"),						// reference bitmap name
			4*KOneSecond,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus				// plug-in status 
		},
	ENDCHECKPOINT
	};
	
const TFrameCheckPoint KMngTerm_subframes_TestPoints[]={
		{
			1,										// absolute frame number
			_S("subframes_frame_1.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking,	// plug-in status mask
			KFrameReadyStatus,
			_S("subframes_mask_1.mbm")		
		},
		{
			2,										// absolute frame number
			_S("subframes_frame_2.mbm"),						// reference bitmap name
			KOneSecond/5,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,
			_S("subframes_mask_2.mbm")						// plug-in status 
		},
		{
			3,										// absolute frame number
			_S("subframes_frame_3.mbm"),						// reference bitmap name
			KOneSecond/5,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status 
			_S("subframes_mask_3.mbm")
		},
		{
			4,										// absolute frame number
			_S("subframes_frame_4.mbm"),						// reference bitmap name
			KOneSecond/5,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status
			_S("subframes_mask_4.mbm") 
		},
		{
			5,										// absolute frame number
			_S("subframes_frame_5.mbm"),						// reference bitmap name
			KOneSecond/5,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status 
			_S("subframes_mask_5.mbm")
		},
		{
			6,										// absolute frame number
			_S("subframes_frame_6.mbm"),						// reference bitmap name
			KOneSecond/5,							// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status 
			_S("subframes_mask_6.mbm")
		},
		{
			7,										// absolute frame number
			_S("subframes_frame_7.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KDecodeFinishedKeepStatus,						// plug-in status
			_S("subframes_mask_7.mbm") 
		},
	ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KMngTerm0Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0012-HP"), 
		_S("MNG TERM0 method using 9-0 countdown image with keep output flag "), 
		KTerm0Countdown,							// source file name
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
		KMngTerm0TestPoints							// checkpoints
	};

const TImageDisplayUnitTestParams KMngTerm0AllocTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-a012-HP"), 
		_S("MNG TERM0 method using 9-0 countdown image with keep output flag "), 
		KTerm0Countdown,							// source file name
		-1, -1, 									// dest size

		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,		// test type
		KMngTerm0TestPoints							// checkpoints
	};

const TImageDisplayUnitTestParams KMngTerm1Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0013-HP"), 
		_S("MNG TERM1 method using 9-0 countdown image with cleat output flag"), 
		KTerm1Countdown,							// source file name
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
		KMngTerm1TestPoints							// checkpoints
	};

const TImageDisplayUnitTestParams KMngTerm2Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0014-HP"), 
		_S("MNG TERM2 method using 9-0 countdown image with display 1st frame"), 
		KTerm2Countdown,							// source file name
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
		KMngTerm2TestPoints							// checkpoints
	};

const TImageDisplayUnitTestParams KMngTerm_3_0_0_1_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0020-HP"), 
		_S("MNG TERM 3-0-0-1 method using 9-0 countdown image with display the last frame"), 
		KTerm_3_0_0_1_Countdown,					// source file name
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
		KMngTerm0TestPoints							// checkpoints. For this test The same as TERM0
	};

const TImageDisplayUnitTestParams KMngTerm_3_0_0_3_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0021-HP"), 
		_S("MNG TERM 3-0-0-3 method using 9-0 countdown. Repeat 3 times, display keep the last frame"), 
		KTerm_3_0_0_3_Countdown,					// source file name
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
		KMngTerm_3_0_0_3_TestPoints					// checkpoints. For this test The same as TERM0
	};

const TImageDisplayUnitTestParams KMngTerm_3_0_3_3_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0022-HP"), 
		_S("MNG TERM 3-0-3-3 method using 9-0 countdown. Repeat 3 times, 3 secs delay b4 repeat.Display keep the last frame"), 
		KTerm_3_0_3_3_Countdown,					// source file name
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
		KMngTerm_3_0_3_3_TestPoints					// checkpoints.
	};

const TImageDisplayUnitTestParams KMngTerm_3_1_0_1_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0023-HP"), 
		_S("MNG TERM 3-1-0-1 method using 9-0 countdown. Erase last frame"), 
		KTerm_3_1_0_1_Countdown,					// source file name
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
		KMngTerm1TestPoints							// checkpoints. For this test The same as TERM1
	};

const TImageDisplayUnitTestParams KMngTerm_3_1_0_3_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0024-HP"), 
		_S("MNG TERM 3-1-0-3 method using 9-0 countdown. Repeat 3 times. Erase last frame"), 
		KTerm_3_1_0_3_Countdown,					// source file name
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
		KMngTerm_3_1_0_3_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngTerm_3_1_3_3_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0025-HP"), 
		_S("MNG TERM 3-1-0-3 method using 9-0 countdown. Repeat 3 times. 3 seconds before repeat. Erase last frame"), 
		KTerm_3_1_3_3_Countdown,					// source file name
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
		KMngTerm_3_1_3_3_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngTerm_3_2_0_1_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0026-HP"), 
		_S("MNG TERM 3-2-0-1 method using 9-0 countdown. Show the  first frame"), 
		KTerm_3_2_0_1_Countdown,					// source file name
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
		KMngTerm2TestPoints						// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngTerm_3_2_0_3_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0027-HP"), 
		_S("MNG TERM 3-2-0-3 method using 9-0 countdown.Repeat 3 times. Show the  first frame"), 
		KTerm_3_2_0_3_Countdown,					// source file name
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
		KMngTerm_3_2_0_3_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngTerm_3_2_3_3_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0028-HP"), 
		_S("MNG TERM 3-2-3-3 method using 9-0 countdown.Repeat 3 times. 3 seconds b4 repeat. Show the  first frame"), 
		KTerm_3_2_3_3_Countdown,					// source file name
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
		KMngTerm_3_2_3_3_TestPoints					// checkpoints. 
	};
	
const TImageDisplayUnitTestParams KMngTerm_subframes_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0222-HP"), 
		_S("MNG TERM Subframe test for loop and empty frame."), 
		KTerm_Subframe,					            // source file name
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
		KMngTerm_subframes_TestPoints					// checkpoints. 
	};
	
const TImageDisplayUnitTestParams KMngTerm_subframes_Cancel_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0223-HP"), 
		_S("MNG cancellation test while conversion still in progress  ."), 
		KTerm_Subframe,					            // source file name
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
		KMngTerm_subframes_TestPoints					// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngTerm_24bit_MimeType_Test =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0224-HP"), 
		_S("Getting MIME Type for 24 bit MNG file."), 
		KTerm_24BitMng,				            	// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		_S8("image/mng"),// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

