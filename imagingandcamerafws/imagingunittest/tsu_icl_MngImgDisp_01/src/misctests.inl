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

#define KAdvBgImage _S("advisory_bg.mng")
#define KNoBgImage _S("no_any_bg.mng")

const TFrameCheckPoint KMngNo_Any_Bg_TestPoints[]={
		{
			1,										// absolute frame number
			_S("no_any_bg_frame_1.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking,	// plug-in status mask
			0,
			_S("no_any_bg_mask_1.mbm")				// reference mask name
		},
		{
			2,										// absolute frame number
			_S("no_any_bg_frame_2.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status 
			_S("no_any_bg_mask_2.mbm")				// reference mask name
		},
		{
			3,										// absolute frame number
			_S("no_any_bg_frame_3.mbm"),			// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status
			_S("no_any_bg_mask_3.mbm")				// reference mask name
		},
	ENDCHECKPOINT
	};


const TImageDisplayUnitTestParams KMngNoAnyBgTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0601-HP"), 
		_S("The MNG image without BACK chunk test"), 
		KNoBgImage,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		4,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KMngNo_Any_Bg_TestPoints					// checkpoints
	};


const TFrameCheckPoint KMngAdvisoryBg_TestPoints[]={
		{
			1,										// absolute frame number
			_S("advisory_bg_frame_1.mbm"),						// reference bitmap name
			TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
			TFrameCheckPoint::KNoStatusChecking,	// plug-in status mask
			0,
			_S("advisory_bg_mask_1.mbm")				// reference mask name
		},
		{
			2,										// absolute frame number
			_S("advisory_bg_frame_2.mbm"),						// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status 
			_S("advisory_bg_mask_2.mbm")				// reference mask name
		},
		{
			3,										// absolute frame number
			_S("advisory_bg_frame_3.mbm"),			// reference bitmap name
			KOneSecond,								// delay from the previuos frame
			KDecodeFinishedMask,					// plug-in status mask
			KFrameReadyStatus,						// plug-in status
			_S("advisory_bg_mask_3.mbm")				// reference mask name
		},
	ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KMngAdvisoryBgTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0602-HP"), 
		_S("The MNG image with alpha channel and BACK chunk of advisory mode"), 
		KAdvBgImage,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,	// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		4,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KMngAdvisoryBg_TestPoints				// checkpoints
	};
