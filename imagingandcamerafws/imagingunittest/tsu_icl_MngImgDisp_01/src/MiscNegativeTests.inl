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

#define KMhdr_NegativeSize			_S("MHdr-NegativeSize.mng")
#define KMhdr_JngPresent			_S("MHdr-JngPresent.mng")
#define KDefi_InvalidLength			_S("DEFI-InvalidLength.mng")
#define KDefi_FilppedRect			_S("DEFI-FilppedRect.mng")
#define KTerm_InvalidOptions		_S("TERM-InvalidOptions.mng")
#define KTerm_InvalidLength			_S("TERM-InvalidLength.mng")
#define KBack_InvalidOptions		_S("BACK-InvalidOptions.mng")
#define KBack_InvalidLength			_S("BACK-InvalidLength.mng")
#define KMagm_InvalidMethod			_S("MAGN-InvalidMethod.mng")
#define KFram_InvalidOption			_S("FRAM-InvalidOption.mng")
#define KFram_InvalidNegativeDelay	_S("FRAM-InvalidNegativeDelay.mng")
#define KFram_InvalidChangeOption	_S("FRAM-InvalidChangeOption.mng")
#define KLoop_InvalidNestLevel		_S("LOOP-InvalidNestLevel.mng")
#define KLoop_NoENDL				_S("LOOP-NoENDL.mng")
#define KEndl_WithoutLoop_Negative	_S("ENDL-WithoutLOOP-Negative.mng")

const TImageDisplayUnitTestParams KMngMhdr_NegativeSize =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0300-HP"), 
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
		KErrCorrupt									// expected error code
	};

const TImageDisplayUnitTestParams KMngMhdr_JngPresent =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0301-HP"), 
		_S("MHDR chunk with Jng presence defined"),
		KMhdr_JngPresent,					// source file name
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
		KErrNotSupported							// expected error code
	};


const TImageDisplayUnitTestParams KMngDefi_InvalidLength =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0302-HP"), 
		_S("DEFI chunk with invalid length"),
		KDefi_InvalidLength,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngDefi_FilppedRect =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0303-HP"), 
		_S("DEFI chunk with flipped rect"),
		KDefi_FilppedRect,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngTerm_InvalidOptions =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0304-HP"), 
		_S("TERM chunk with invalid termination options"),
		KTerm_InvalidOptions,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngTerm_InvalidLength =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0305-HP"), 
		_S("TERM chunk with invalid length"),
		KTerm_InvalidLength,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngBack_InvalidOptions =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0306-HP"), 
		_S("BACK chunk with invalid options"),
		KBack_InvalidOptions,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngBack_InvalidLength =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0307-HP"), 
		_S("BACK chunk with invalid length"),
		KBack_InvalidLength,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngMagm_InvalidMethod =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0308-HP"), 
		_S("MAGN chunk with invalid method"),
		KMagm_InvalidMethod,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngFram_InvalidOption =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0309-HP"), 
		_S("FRAM chunk with invalid options"),
		KFram_InvalidOption,					// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngFram_InvalidNegativeDelay =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0310-HP"), 
		_S("FRAM chunk with invalid(negative) delay"),
		KFram_InvalidNegativeDelay,				// source file name
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
		KErrCorrupt							// expected error code
	};


const TImageDisplayUnitTestParams KMngFram_InvalidChangeOption =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0311-HP"), 
		_S("FRAM chunk with invalid option"),
		KFram_InvalidChangeOption,				// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngLoop_InvalidNestLevel =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0312-HP"), 
		_S("LOOP chunk with invalid nesting level"),
		KLoop_InvalidNestLevel,				// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		2,				// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENegative,		// test type
		NULL,										// checkpoints. 
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngLoop_NoENDL =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0313-HP"), 
		_S("LOOP chunk without corresponding ENDL"),
		KLoop_NoENDL,				// source file name
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
		KErrCorrupt							// expected error code
	};

const TImageDisplayUnitTestParams KMngEndl_WithoutLoop_Negative =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0314-HP"), 
		_S("ENDLchunk without corresponding LOOP "),
		KEndl_WithoutLoop_Negative,				// source file name
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
		KErrCorrupt							// expected error code
	};

			

