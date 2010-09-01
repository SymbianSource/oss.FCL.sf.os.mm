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

#define KGifClock		_S("\\ICL\\TestImages\\clock.gif")
#define KEpocIcon		_S("\\ICL\\TestImages\\icoepoc.ico")
#define KTransGif1		_S("\\ICL\\TestImages\\trans_1.gif")
#define KTransGif8		_S("\\ICL\\TestImages\\trans_8.gif")
#define KBmp8bpp		_S("\\ICL\\TestImages\\bmp8bpp_8.bmp")
#define KTif3_4			_S("\\ICL\\TestImages\\tif3_4.tif")
#define KJFIF444		_S("\\ICL\\TestImages\\jfif444.jpg")
#define KOtaFile		_S("\\ICL\\TestImages\\operator.ota")
#define KPng4_8			_S("\\ICL\\TestImages\\png4_8.png")
#define KWbmpFile		_S("\\ICL\\TestImages\\sun.wbmp")
#define KWmf3_8File		_S("\\ICL\\TestImages\\wmf3_8.wmf")
#define KWmfWithMask	_S("\\TSU_ICL_COD_03\\WMF\\DibPatternBrush.wmf")
#define KGifAnimatedCat	_S("genIcl\\AnmatCat.gif")
#define KPngWithAlpha	_S("genIcl\\png2.png")

#if defined (__WINS__) || defined(__X86GCC__)
#define KWmfRefDir L"WMFRef"
#define KWmfRefMaskDir L"WMFRefMask"
#define KWmf3_8Ref _S("\\ICL\\RefImages\\wmf3_8.mbm")
#else
#define KWmfRefDir L"WMFArmRef"
#define KWmfRefMaskDir L"WMFArmRefMask"
#define KWmf3_8Ref _S("wmf3_8_Arm.mbm")
#endif // __WINS__,__X86GCC__

const TFrameCheckPoint KNo_TestPoints[]=
	{
		ENDCHECKPOINT
	};

const TFunctionalTstParams KSingleFrameNoMaskFuncParam=
		{
		1,													// expected number of frames
		CImageDisplay::EImageSingleFrame,					// ImageStatus() value
		{KUidGenIclImageDisplayPluginExt,0},				// Extesion i-face Uids
		TFunctionalTstParams::KScalingQualityNotSet,		// parameter for scaling quality
		TFunctionalTstParams::KReductionFactorNotSet,		// parameter values for max reduction factor
		-1,													// max unscaled size width
		-1													// max unscaled size height
		};

const TFunctionalTstParams KSingleFrameWithMaskFuncParam=
		{
		1,																// expected number of frames
		CImageDisplay::EImageSingleFrame|CImageDisplay::EImageMasked,	// ImageStatus() value
		{KUidGenIclImageDisplayPluginExt,0},								// Extesion i-face Uids
		CBitmapScaler::EMinimumQuality,						// parameter for scaling quality
		0,													// parameter values for max reduction factor
		-1,													// max unscaled size width
		-1													// max unscaled size height
		};

const TFunctionalTstParams KSingleScalableFrameWithMaskFuncParam=
		{
		1,																// expected number of frames
		CImageDisplay::EImageSingleFrame|CImageDisplay::EImageMasked|CImageDisplay::EImageIsFullyScalable,	// ImageStatus() value
		{KUidGenIclImageDisplayPluginExt,0},								// Extesion i-face Uids
		TFunctionalTstParams::KScalingQualityNotSet,		// parameter for scaling quality
		TFunctionalTstParams::KReductionFactorNotSet,		// parameter values for max reduction factor
		-1,													// max unscaled size width
		-1													// max unscaled size height
		};

const TFrameCheckPoint KIcoSmokeTest_TestPoints[]=
	{
		{
		1,										// absolute frame number
		_S("\\ICL\\RefImages\\icoepoc.mbm"),	// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus						// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KIcoSmokeTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0001-HP"), 
		_S("Windows .ico decoding test"),
		KEpocIcon,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KIcoSmokeTest_TestPoints,				// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KNoFunctionalParams					// values of getter functions which are expected
	};

const TFrameCheckPoint KBmpSmokeTest_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\bmp8bpp_8.mbm"),	// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KBmpSmokeTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0002-HP"), 
		_S("BMP 8 bpp decoding test"),
		KBmp8bpp,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KBmpSmokeTest_TestPoints,				// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected
	};


const TFrameCheckPoint KTransGif1Test_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\trans_1.mbm"),	// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KTransGif1Test =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0003-HP"), 
		_S("GIF 1 bpp decoding test"),
		KTransGif1,								// source file name
		-1, -1, 									// dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KTransGif1Test_TestPoints,				// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameWithMaskFuncParam			// values of getter functions which are expected
	};

const TFrameCheckPoint KTransGif8Test_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\trans_8.mbm"),	// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KTransGif8Test =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0004-HP"), 
		_S("GIF 8 bpp decoding test"),
		KTransGif8,								// source file name
		-1, -1, 									// dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type

		KTransGif8Test_TestPoints,				// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected
	};


const TFrameCheckPoint KTif3_4Test_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\tif3_4.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KTif3_4Test =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0005-HP"), 
		_S("TIFF decoding test"),
		KTif3_4,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KTif3_4Test_TestPoints,					// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected
	};

const TFrameCheckPoint KJfif444Test_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\jfif444.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KJfif444Test =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0006-HP"), 
		_S("JFIF 444 decoding test"),
		KJFIF444,									// source file name
		-1, -1, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KJfif444Test_TestPoints,					// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected		
	};

const TInt KClockInterframeDelay=1000000; // 1 second
const TFrameCheckPoint KClockTest_TestPoints[]=
	{
		{
		1,		// absolute frame number
		_S("clock_frame_1.mbm"),				// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus						// plug-in status 
		},
		{
		2,		// absolute frame number
		_S("clock_frame_2.mbm"),				// reference bitmap name
		KClockInterframeDelay,					// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus						// plug-in status 
		},
		{
		5,		// absolute frame number
		_S("clock_frame_5.mbm"),				// reference bitmap name
		KClockInterframeDelay,					// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus						// plug-in status 
		},
		{
		6,		// absolute frame number
		_S("clock_frame_6.mbm"),				// reference bitmap name
		KClockInterframeDelay,					// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus						// plug-in status 
		},
		{
		11,		// absolute frame number
		_S("clock_frame_11.mbm"),				// reference bitmap name
		KClockInterframeDelay,					// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus						// plug-in status 
		},
		{
		12,		// absolute frame number
		_S("clock_frame_12.mbm "),				// reference bitmap name
		KClockInterframeDelay,					// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KClockTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0007-HP"), 
		_S("Animated GIF with overcomposing showing clock"),
		KGifClock,									// source file name
		-1, -1, 									// dest size
		EColor64K,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KClockTest_TestPoints,					// checkpoints
		KZeroTolerance							// tolerance to per-byte image diffs
	};

const TFrameCheckPoint KOtaTest_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\operator.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KOtaTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0008-HP"), 
		_S("OTA decoding test"),
		KOtaFile,									// source file name
		-1, -1, 									// dest size
		-1,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KOtaTest_TestPoints,					// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected
	};

const TFrameCheckPoint KPng4_8Test_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\png4_8.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KPng4_8Test =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0009-HP"), 
		_S("PNG 4-8 decoding test"),
		KPng4_8,									// source file name
		-1, -1, 									// dest size
		-1,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KPng4_8Test_TestPoints,					// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected
	};

const TFrameCheckPoint KWbmpTest_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\ICL\\RefImages\\sun.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KWbmpTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0010-HP"), 
		_S("WBMP decoding test"),
		KWbmpFile,									// source file name
		-1, -1, 									// dest size
		-1,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KWbmpTest_TestPoints,					// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameNoMaskFuncParam			// values of getter functions which are expected
	};

const TFrameCheckPoint KWmf3_8Test_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		KWmf3_8Ref,								// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus					// plug-in status 
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KWmf3_8Test =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0011-HP"), 
		_S("Wmf 3-8 decoding test"),
		KWmf3_8File,								// source file name
		-1, -1, 									// dest size
		-1,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KWmf3_8Test_TestPoints,					// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleScalableFrameWithMaskFuncParam	// values of getter functions which are expected
	};

const TFrameCheckPoint KWmfWithMaskTest_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("\\TSU_ICL_COD_03\\" KWmfRefDir L"\\DibPatternBrush.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus,					// plug-in status 
		_S("\\TSU_ICL_COD_03\\" KWmfRefMaskDir L"\\DibPatternBrush.mbm")		// mask reference bitmap name
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KWmfWithMaskTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0012-HP"), 
		_S("Wmf with mask decoding test"),
		KWmfWithMask,								// source file name
		-1, -1, 									// dest size
		-1,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KWmfWithMaskTest_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleScalableFrameWithMaskFuncParam	// values of getter functions which are expected
	};

const TInt KAnimCatDelay=TFrameCheckPoint::KNodelayChecking;
const TFrameCheckPoint KGifAnimatedCatTest_TestPoints[]=
	{
		{
		1,								// absolute frame number
		_S("AnmatCat_frame_1.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus,					// plug-in status 
		_S("AnmatCat_mask_1.mbm")		// mask reference bitmap name
		},
		{
		2,								// absolute frame number
		_S("AnmatCat_frame_2.mbm"),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_2.mbm")		// mask reference bitmap name
		},
		{
		3,								// absolute frame number
		_S("AnmatCat_frame_3.mbm"),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_3.mbm")		// mask reference bitmap name
		},
		{
		20,								// absolute frame number
		_S("AnmatCat_frame_20.mbm"),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_20.mbm")		// mask reference bitmap name
		},
		{
		21,								// absolute frame number
		_S("AnmatCat_frame_21.mbm"),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_21.mbm")		// mask reference bitmap name
		},
		{
		37,								// absolute frame number
		_S("AnmatCat_frame_37.mbm"),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_37.mbm")		// mask reference bitmap name
		},
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("AnmatCat_frame_38.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus,					// plug-in status 
		_S("AnmatCat_mask_38.mbm")		// mask reference bitmap name
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KGifAnimatedCatTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0013-HP"), 
		_S("Nice gif animation with mask decoding test"),
		KGifAnimatedCat,								// source file name
		-1, -1, 									// dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatTest_TestPoints,			// checkpoints
		KZeroTolerance							// tolerance to per-byte image diffs
	};

const TFrameCheckPoint KPngWithAlphaTest_TestPoints[]=
	{
		{
		TFrameCheckPoint::KLastFrameCheck,		// absolute frame number
		_S("png2_frame_1.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KLastFrameReadyStatus,					// plug-in status 
		_S("png2_mask_1.mbm")		// mask reference bitmap name
		},
		ENDCHECKPOINT
	};

const TImageDisplayUnitTestParams KPngWithAlphaTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0014-HP"), 
		_S("Png image with alpha channel decoding test"),
		KPngWithAlpha,								// source file name
		-1, -1, 									// dest size
		-1,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KPngWithAlphaTest_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KSingleFrameWithMaskFuncParam			// values of getter functions which are expected
	};


const TImageDisplayUnitTestParams KGifAnimatedCatAllocTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0501-HP"), 
		_S("Nice gif animation with mask decoding test, Alloc mode"),
		KGifAnimatedCat,								// source file name
		-1, -1, 									// dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		5,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::EAlloc,	// test type
		KGifAnimatedCatTest_TestPoints,			// checkpoints
		KZeroTolerance							// tolerance to per-byte image diffs
	};


const TFunctionalTstParams KAnmCatRespFuncParam=
		{
		38,																// expected number of frames
		CImageDisplay::EImageMultiFrame|CImageDisplay::EImageAnimated|CImageDisplay::EImageMasked,	// ImageStatus() value
		{KUidGenIclImageDisplayPluginExt,0},								// Extesion i-face Uids
		CBitmapScaler::EMaximumQuality,		// parameter for scaling quality
		TFunctionalTstParams::KReductionFactorNotSet,		// parameter values for max reduction factor
		-1,													// max unscaled size width
		-1													// max unscaled size height
		};

const TFrameCheckPoint KGifAnimatedCatResp_TestPoints[]=
	{
		{
		1,								// absolute frame number
		_S("AnmatCat_frame_1_clp10-90-rcw-sc05.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus,					// plug-in status 
		_S("AnmatCat_mask_1_clp10-90-rcw-sc05.mbm")		// mask reference bitmap name
		},
		{
		2,								// absolute frame number
		_S("AnmatCat_frame_2_clp10-90-rcw-sc05.mbm "),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_2_clp10-90-rcw-sc05.mbm")		// mask reference bitmap name
		},
		{
		3,								// absolute frame number
		_S("AnmatCat_frame_3_clp10-90-rcw-sc05.mbm "),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_3_clp10-90-rcw-sc05.mbm")		// mask reference bitmap name
		},
		ENDCHECKPOINT	
	};

const TInt KAnmCatClipRect[]=
	{
	10,	10,
	90, 90
	};

const TImageDisplayUnitTestParams KGifAnimatedStopResponseTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0101-HP"), 
		_S("Nice gif animation with mask decoding test, Stop() Response Test"),
		KGifAnimatedCat,								// source file name
		-1, -1, 									// dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		3,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatTest_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KAnmCatRespFuncParam					// values of getter functions which are expected
	};
	
const TImageDisplayUnitTestParams KGifAnimatedPauseResponseTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0102-HP"), 
		_S("Nice gif animation with mask decoding test , Pause() Response Test"),
		KGifAnimatedCat,								// source file name
		-1, -1, 									// dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		4,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatTest_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KAnmCatRespFuncParam					// values of getter functions which are expected		
	};

const TImageDisplayUnitTestParams KGifAnimatedPostProcPauseResponseTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0111-HP"), 
		_S("Nice gif animation with mask decoding test and postprocessing, Pause() Test"),
		KGifAnimatedCat,								// source file name
		TImageDisplayUnitTestParams::KDestSizeIsSecondDefSize, -1, // dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionRotateCw90,		// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		3,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatResp_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KAnmCatRespFuncParam,					// values of getter functions which are expected		
		KAnmCatClipRect							//clipping rect, must point to 4 TInt's if not NULL		
	};

const TImageDisplayUnitTestParams KGifAnimatedPostProcStopResponseTest =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0112-HP"), 
		_S("Nice gif animation with mask decoding test and postprocessing, Stop() Test"),
		KGifAnimatedCat,								// source file name
		TImageDisplayUnitTestParams::KDestSizeIsSecondDefSize, -1, // dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionRotateCw90,		// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		3,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatResp_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KAnmCatRespFuncParam,					// values of getter functions which are expected		
		KAnmCatClipRect							//clipping rect, must point to 4 TInt's if not NULL		
	};
	


const TFrameCheckPoint KGifAnimatedCatResp2x_TestPoints[]=
	{
		{
		1,								// absolute frame number
		_S("AnmatCat_frame_1_clp10-90-rcw-sc2x.mbm"),		// reference bitmap name
		TFrameCheckPoint::KNodelayChecking,		// delay from the previuos frame
		KDecodeFinishedMask,					// plug-in status mask
		KFrameReadyStatus,					// plug-in status 
		_S("AnmatCat_mask_1_clp10-90-rcw-sc2x.mbm")		// mask reference bitmap name
		},
		{
		2,								// absolute frame number
		_S("AnmatCat_frame_2_clp10-90-rcw-sc2x.mbm "),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_2_clp10-90-rcw-sc2x.mbm")		// mask reference bitmap name
		},
		{
		3,								// absolute frame number
		_S("AnmatCat_frame_3_clp10-90-rcw-sc2x.mbm "),		// reference bitmap name
		KAnimCatDelay,					// delay from the previuos frame
		KDecodeFinishedMask,			// plug-in status mask
		KFrameReadyStatus,				// plug-in status 
		_S("AnmatCat_mask_3_clp10-90-rcw-sc2x.mbm")		// mask reference bitmap name
		},
		ENDCHECKPOINT	
	};


const TImageDisplayUnitTestParams KGifAnimatedPostProcPauseResponseTest2x =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0113-HP"), 
		_S("Nice gif animation with mask decoding test and postprocessing, Pause() Test"),
		KGifAnimatedCat,								// source file name
		TImageDisplayUnitTestParams::KDestSizeIsDoubleSize, -1, // dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionRotateCw90,		// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		3,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatResp2x_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KAnmCatRespFuncParam,					// values of getter functions which are expected		
		KAnmCatClipRect							//clipping rect, must point to 4 TInt's if not NULL		
	};

const TImageDisplayUnitTestParams KGifAnimatedPostProcStopResponseTest2x =
	{	
		_S("MM-ICL-GENICLIMGDISP-U-0114-HP"), 
		_S("Nice gif animation with mask decoding test and postprocessing, Stop() Test"),
		KGifAnimatedCat,								// source file name
		TImageDisplayUnitTestParams::KDestSizeIsDoubleSize, -1, // dest size
		EColor256,									// display mode
		CImageDisplay::EOptionMainImage|CImageDisplay::EOptionRotateCw90,		// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		3,										// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,	// test type
		KGifAnimatedCatResp2x_TestPoints,			// checkpoints
		KZeroTolerance,							// tolerance to per-byte image diffs
		KErrNone,								// expected error code
		&KAnmCatRespFuncParam,					// values of getter functions which are expected		
		KAnmCatClipRect							//clipping rect, must point to 4 TInt's if not NULL		
	};
