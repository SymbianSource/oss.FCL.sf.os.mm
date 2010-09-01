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

const TImageDisplayUnitTestParams KExifAutoRot1Pause_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0201-HP"), 
		_S("The Exif-autorotate method 1 -pause response test"),
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

const TImageDisplayUnitTestParams KExifAutoRot7Pause_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0202-HP"), 
		_S("The Exif-autorotate method 7 -pause response test"),
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


const TImageDisplayUnitTestParams KExifAutoRot7ThumbPause_Test =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0203-HP"), 
		_S("The Exif-autorotate method 7 -pause response test"),
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



const TImageDisplayUnitTestParams KStopResponseTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0204-HP"), 
		_S("The Exif-autorotate method 7 - Stop() response test"),
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
