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
// jpegperformancetest.inl
// 
//

#define KSunsetOverCam	_S("c:\\TSU_ICL_cod_04\\CamSunset2MP.jpg")

const TInt KOneSec	=1000*1000;
const TInt KOneTenth=KOneSec/10;

#define KSmallExt32x32MonoPerf 		_S("c:\\tsu_icl_cod_04b\\32x32_mono.jpg")
#define KSmallExtMonoPerf 		_S("c:\\tsu_icl_cod_04b\\jpeg_small_mono_new.jpg")
#define KAverageExtMonoPerf 		_S("c:\\tsu_icl_cod_04b\\jpeg_avg_mono_new.jpg")
#define KLargeExtMonoPerf 		_S("c:\\tsu_icl_cod_04b\\jpeg_large_mono_new.jpg")
#define KSmallExt420Perf 		_S("c:\\tsu_icl_cod_04b\\jpeg_small_420_new.jpg")
#define KAverageExt420Perf 		_S("c:\\tsu_icl_cod_04b\\jpeg_avg_420_new.jpg")
#define KLargeExt420Perf 		_S("c:\\tsu_icl_cod_04b\\jpeg_large_420_new.jpg")
#define KSmallExt422Perf 		_S("c:\\tsu_icl_cod_04b\\jpeg_small_422_new.jpg")
#define KAverageExt422Perf 		_S("c:\\tsu_icl_cod_04b\\jpeg_avg_422_new.jpg")
#define KLargeExt422Perf 		_S("c:\\tsu_icl_cod_04b\\jpeg_large_422_new.jpg")

/*
These JPEG codec performance test parameters
DO NOT increase timmings unless these is a VERY GOOD reason for that
*/

const TPerformanceTestParams K2MpColor16m_1to1Size = 
	{
	KSunsetOverCam,					// source file name
	NULL,							// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	2*KOneSec + 2*KOneTenth,		// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode	// TOptions parameter
	};
	
	
const TPerformanceTestParams K2MpColor16m_1to2Size = 
	{
	KSunsetOverCam,					// source file name
	NULL,							// reference file name
	EColor16M,						// decoding display mode
	1,								// reduction factor
	KOneSec + 3*KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode	// TOptions parameter
	};

const TPerformanceTestParams K2MpColor16m_1to4Size = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\CamSunset2MPRed2.mbm"),		// reference file name
	EColor16M,						// decoding display mode
	2,								// reduction factor
	9*KOneTenth,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode	// TOptions parameter
	};

const TPerformanceTestParams K2MpColor16m_1to8Size = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\CamSunset2MPRed3.mbm"),		// reference file name
	EColor16M,						// decoding display mode
	3,								// reduction factor
	7*KOneTenth,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode	// TOptions parameter
	};

const TPerformanceTestParams K2MpColor64K_1to1Size = 
	{
	KSunsetOverCam,					// source file name
	NULL,							// reference file name
	EColor64K,						// decoding display mode
	0,								// reduction factor
	2*KOneSec + KOneSec/2,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode	// TOptions parameter
	};

const TPerformanceTestParams K2MpColor64K_1to4Size = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\CamSunset2MPRed2_64K.mbm"),		// reference file name
	EColor64K,						// decoding display mode
	2,								// reduction factor
	KOneSec - KOneTenth/2,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode	// TOptions parameter
	};


const TPerformanceTestParams K2MpEncode_Q75_RAM = 
	{
	KSunsetOverCam,					// source file name
	NULL,		                    // reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	2*KOneSec + KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EOptionAlwaysThread,		// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};
	
const TPerformanceTestParams K2MpEncode_Q75_EXT = 
	{
	KSunsetOverCam,					// source file name
	NULL,		                    // reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	3*KOneSec - KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EOptionAlwaysThread,		// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
#if defined(__WINS__) || defined(__X86GCC__)
    TPerformanceTestParams::EDstMediaRamDrive // destination media type
#else	
	TPerformanceTestParams::EDstMediaMMCDrive // destination media type
#endif // __WINS__,__X86GCC__
	};

const TPerformanceTestParams K2MpEncode_Q75_RAM_C64K = 
	{
	KSunsetOverCam,					// source file name
	NULL,		                    // reference file name
	EColor64K,						// decoding display mode
	0,								// reduction factor
	4*KOneSec - 2*KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EOptionAlwaysThread,		// TOptions parameter		
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};

const TPerformanceTestParams K2MpEncode_Q75_RAM_FastEncode_EColor16M_422 = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\camsunset2mp_ref_fastencode_ecolor16m_422.jpg"),		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	2*KOneSec + KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EPreferFastEncode,	// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor422,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};

const TPerformanceTestParams K2MpEncode_Q75_RAM_FastEncode_EColor16M_Mono = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\camsunset2mp_ref_fastencode_ecolor16m_mono.jpg"),		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	2*KOneSec + KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EPreferFastEncode,	// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EMonochrome,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};

const TPerformanceTestParams K2MpEncode_Q75_RAM_FastEncode_EColor16M_420 = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\camsunset2mp_ref_fastencode_ecolor16m_420.jpg"),		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	2*KOneSec + KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EPreferFastEncode,	// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};

const TPerformanceTestParams K2MpEncode_Q75_RAM_FastEncode_EColor64K_420 = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\camsunset2mp_ref_fastencode_ecolor64k_420.jpg"),		// reference file name
	EColor64K,						// decoding display mode
	0,								// reduction factor
	4*KOneSec - 2*KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EPreferFastEncode,	// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};

const TPerformanceTestParams K2MpEncode_Q75_RAM_FastEncode_EColor256_420 = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\camsunset2mp_ref_fastencode_ecolor256_420.jpg"),		// reference file name
	EColor256,						// decoding display mode
	0,								// reduction factor
	4*KOneSec - 2*KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EPreferFastEncode,	// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};
	
const TPerformanceTestParams K2MpEncode_Q75_RAM_FastEncode_EGray256_420 = 
	{
	KSunsetOverCam,					// source file name
	_S("c:\\TSU_ICL_cod_04\\camsunset2mp_ref_fastencode_egray256_420.jpg"),		// reference file name
	EGray256,						// decoding display mode
	0,								// reduction factor
	4*KOneSec - 2*KOneTenth,			// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	CImageEncoder::EPreferFastEncode,	// TOptions parameter	
	75,                                 // encoder quality settings
	TJpegImageData::EColor420,          // JPEG sampling format
	TPerformanceTestParams::EDstMediaRamDrive // destination media type
	};

const TPerformanceTestParams KSmall32x32Mono =
	{
	KSmallExt32x32MonoPerf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	16, //iCropW;
	16, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};
	
const TPerformanceTestParams KSmallMono =
	{
	KSmallExtMonoPerf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KAverageMono =
	{
	KAverageExtMonoPerf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KLargeMono =
	{
	KLargeExtMonoPerf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KSmall420 =
	{
	KSmallExt420Perf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KAverage420 =
	{
	KAverageExt420Perf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KLarge420 =
	{
	KLargeExt420Perf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KSmall422 =
	{
	KSmallExt422Perf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KAverage422 =
	{
	KAverageExt422Perf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};

const TPerformanceTestParams KLarge422 =
	{
	KLargeExt422Perf,					// source file name
	NULL,		// reference file name
	EColor16M,						// decoding display mode
	0,								// reduction factor
	0,					// in microseconds for H2 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	0,                                 // encoder quality settings
	0,          // JPEG sampling format
	0, // destination media type
	0,   //iCropX 
	0, 	 //iCropY;
	320, //iCropW;
	240, //iCropH;
	0,   //iNumRepeats;
	0, //iOperate1;   
	0, //mirror over vert axis iOperate2;  
	0,     //iScaleCoeff;
	0 ,    //iScaleQuality
	3,   // iNumZoom
	};
