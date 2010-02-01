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

const TImageDisplayUnitTestParams KAllocFileNameTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0401-HP"), 
		_S("The Exif-autorotate method 7, alloc file name"), 
		KAutoRotFile7,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
	

const TImageDisplayUnitTestParams KAllocFileHandleTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0402-HP"), 
		_S("The Exif-autorotate method 7, alloc, filehandle"), 
		KAutoRotFile7,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
	

const TImageDisplayUnitTestParams KAllocDataBufTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0403-HP"), 
		_S("The Exif-autorotate method 7, alloc, databuf"), 
		KAutoRotFile7,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a data buffer
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		KExifAutoRotMain_TestPoints,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
	

const TImageDisplayUnitTestParams KAllocThumbFileNameTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0404-HP"), 
		_S("The Exif-autorotate thumbnail method 7, alloc file name"), 
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
		TImageDisplayUnitTestParams::EAlloc,	// test type

		KExifAutoRotThumb_TestPoints1,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
	

const TImageDisplayUnitTestParams KAllocThumbFileHandleTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0405-HP"), 
		_S("The Exif-autorotate thumbnail method 7, alloc, filehandle"), 
		KAutoRotFile7,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filehandle
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		KExifAutoRotThumb_TestPoints1,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
	

const TImageDisplayUnitTestParams KAllocThumbDataBufTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0406-HP"), 
		_S("The Exif-autorotate thumbnail method 7, alloc, databuf"), 
		KAutoRotFile7,						// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionThumbnail|CImageDisplay::EOptionAutoRotate,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a data buffer
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		KExifAutoRotThumb_TestPoints1,				// checkpoints
		KQuaterPercent							// tolerance to per-byte image diffs
	};
	

