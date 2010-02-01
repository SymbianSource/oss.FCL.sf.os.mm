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

const TImageDisplayUnitTestParams KMngMhdr_NegativeSize_Icl =
	{	
		_S(""), 
		_S("MHDR chunk with negative frame sizes"), 
		KMhdr_NegativeSize,					// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		1,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		KErrNotFound								// expected error code
	};

const TImageDisplayUnitTestParams KEmtpyFileNegativeIcl =
	{	
		_S(""), 
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
		KErrUnderflow							// expected error code
	};


const TImageDisplayUnitTestParams KMngTerm0IclResponse =
	{	
		_S(" MM-ICL-MNGIMGDISP-U-0012-HP"), 
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

const TImageDisplayUnitTestParams KBasiFileNegativeIcl =
	{	
		_S(" MM-ICL-MNGIMGDISP-U-0226-HP"), 
		_S("Mng file with BASI chunk not supported"),
		KBasiFile,							// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeMng,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		KErrNotSupported							// expected error code
	};

const TImageDisplayUnitTestParams KMoveGamaFileNegativeIcl =
	{	
		_S(" MM-ICL-MNGIMGDISP-U-0227-HP"), 
		_S("Mng file with MOVE and/or gAMA chunks not supported"),
		KMoveGamaFile,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		KMimeTypeMng,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		KErrNotSupported							// expected error code
	};

