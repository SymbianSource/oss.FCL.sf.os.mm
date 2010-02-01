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
		_S("MM-ICL-MNGIMGDISP-U-0201-HP"), 
		_S("The MNG smoke test using filename (Red Jumping ball)"), 
		KJumpingBall,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage | CImageDisplay::EOptionMirrorVertical | CImageDisplay::EOptionRotateCw180,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		NULL									// checkpoints
	};

const TImageDisplayUnitTestParams KAllocMagnTest =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0202-HP"), 
		_S("MNG MAGN method 3 with MX 8 MY 4"), 
		KMagn3_MX8_MY4,					// source file name
		3*19,  3*7, 								// dest size (scale by factor 3)
		-1,											// display mode
		CImageDisplay::EOptionMainImage , // image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,		// test type
		NULL										// checkpoints. 
	};

const TImageDisplayUnitTestParams KAllocFileNameTestDbuf =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0203-HP"), 
		_S("The MNG smoke test using databuffer (Red Jumping ball)"), 
		KJumpingBall,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		NULL									// checkpoints
	};

const TImageDisplayUnitTestParams KAllocMagnTestDbuf =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0204-HP"), 
		_S("MNG MAGN method 3 with MX 8 MY 4 using the databuffer"), 
		KMagn3_MX8_MY4,					// source file name
		19,  7, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeData,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,		// test type
		NULL										// checkpoints. 
	};


const TImageDisplayUnitTestParams KAllocFileNameTestFh =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0205-HP"), 
		_S("The MNG smoke test using file handle (Red Jumping ball)"), 
		KJumpingBall,								// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type

		NULL									// checkpoints
	};

const TImageDisplayUnitTestParams KAllocMagnTestFh =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0206-HP"), 
		_S("MNG MAGN method 3 with MX 8 MY 4 using the file handle "), 
		KMagn3_MX8_MY4,					// source file name
		19,  7, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileHandle,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,		// test type
		NULL										// checkpoints. 
	};

