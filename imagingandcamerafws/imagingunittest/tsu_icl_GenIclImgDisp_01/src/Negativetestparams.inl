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

#define KEmptyFile		_S("empty.jpeg")

const TImageDisplayUnitTestParams KPanicInitFailed =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0501-HP"), 
		_S("EPanicInitFailed (1) panic test test"),
		KAutoRotFile7,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL										// checkpoints. 
	};
	
const TImageDisplayUnitTestParams KPanicNotInit =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0502-HP"), 
		_S("EPanicNotInitialized (2) panic test test"),
		KAutoRotFile7,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL										// checkpoints. 
	};	


const TImageDisplayUnitTestParams KEmptyFileTest =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0503-HP"), 
		_S("Attempt to decode an empty file"),
		KEmptyFile,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		0,											// tolerance
		KErrUnderflow							// expected error code
	};	

const TImageDisplayUnitTestParams KWrongReductionTest1 =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0504-HP"), 
		_S("Attempt to specify a negative reduction factor"),
		KAutoRotFile7,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		0,											// tolerance
		KErrArgument							// expected error code
	};	

const TImageDisplayUnitTestParams KWrongReductionTest2 =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0505-HP"), 
		_S("Attempt to specify a too big reduction factor"),
		KAutoRotFile7,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		0,											// tolerance
		KErrArgument							// expected error code
	};	

const TImageDisplayUnitTestParams KWrongImageOptions =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0506-HP"), 
		_S("Attempt to specify both the EOptionMainImage and EOptionThumbnail"),
		KAutoRotFile7,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionThumbnail,// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		0,											// tolerance
		KErrNotSupported							// expected error code
	};

const TImageDisplayUnitTestParams KUnsupExtension =
	{	
		_S("MM-ICL-EXIFIMGDISP-U-0507-HP"), 
		_S("Attempt to get unsupported extension interface"),
		KAutoRotFile7,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeExif,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		0,											// tolerance
		KErrNotSupported							// expected error code
	};




