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

#define KAutoRotFile1		_S("auto_rot_1.jpeg")
#define KAutoRotFile2		_S("auto_rot_2.jpeg")
#define KAutoRotFile3		_S("auto_rot_3.jpeg")
#define KAutoRotFile4		_S("auto_rot_4.jpeg")
#define KAutoRotFile5		_S("auto_rot_5.jpeg")
#define KAutoRotFile6		_S("auto_rot_6.jpeg")
#define KAutoRotFile7		_S("auto_rot_7.jpeg")
#define KAutoRotFile8		_S("auto_rot_8.jpeg")

#define KAutoRotRefMain		_S("auto_rot_main.mbm")
#define KAutoRotRefThumb	_S("auto_rot_thumb.mbm")
#define KAutoRotRefThumb1	_S("auto_rot_thumb1.mbm")
#define KAutoRotRefThumb2	_S("auto_rot_thumb2.mbm")
#define KAutoRotRefThumb3	_S("auto_rot_thumb3.mbm")

const TFrameCheckPoint KExifAutoRotMain_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		KAutoRotRefMain,				// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KExifAutoRot1_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0021-HP"), 
		_S("The Exif-autorotate method 1"), 
		KAutoRotFile1,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot2_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0022-HP"), 
		_S("The Exif-autorotate method 2"), 
		KAutoRotFile2,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot3_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0023-HP"), 
		_S("The Exif-autorotate method 3"), 
		KAutoRotFile3,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot4_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0024-HP"), 
		_S("The Exif-autorotate method 4"), 
		KAutoRotFile4,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot5_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0025-HP"), 
		_S("The Exif-autorotate method 5"), 
		KAutoRotFile5,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot6_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0026-HP"), 
		_S("The Exif-autorotate method 6"), 
		KAutoRotFile6,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot7_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0027-HP"), 
		_S("The Exif-autorotate method 7"), 
		KAutoRotFile7,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot8_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0028-HP"), 
		_S("The Exif-autorotate method 8"), 
		KAutoRotFile8,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};


// thumbnail tests //

const TFrameCheckPoint KExifAutoRotThumb_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		KAutoRotRefThumb,				// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TFrameCheckPoint KExifAutoRotThumb_TestPoints1[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		KAutoRotRefThumb1,				// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TFrameCheckPoint KExifAutoRotThumb_TestPoints2[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		KAutoRotRefThumb2,				// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TFrameCheckPoint KExifAutoRotThumb_TestPoints3[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		KAutoRotRefThumb3,				// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};


const TImageDisplayUnitTestParams KExifAutoRot1Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0031-HP"), 
		_S("The Exif-autorotate method 1 (using thumbnail) "), 
		KAutoRotFile1,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot2Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0032-HP"), 
		_S("The Exif-autorotate method 2 (using thumbnail) "), 
		KAutoRotFile2,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot3Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0033-HP"), 
		_S("The Exif-autorotate method 3 (using thumbnail) "), 
		KAutoRotFile3,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints2,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot4Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0034-HP"), 
		_S("The Exif-autorotate method 4 (using thumbnail) "), 
		KAutoRotFile4,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints2,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot5Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0035-HP"), 
		_S("The Exif-autorotate method 5 (using thumbnail) "), 
		KAutoRotFile5,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints3,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot6Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0036-HP"), 
		_S("The Exif-autorotate method 6 (using thumbnail) "), 
		KAutoRotFile6,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints1,				// checkpoints
		KHalfPercent								// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot7Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0037-HP"), 
		_S("The Exif-autorotate method 7 (using thumbnail) "), 
		KAutoRotFile7,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints1,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};

const TImageDisplayUnitTestParams KExifAutoRot8Thumb_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0038-HP"), 
		_S("The Exif-autorotate method 8 (using thumbnail) "), 
		KAutoRotFile8,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KExifAutoRotThumb_TestPoints1,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
