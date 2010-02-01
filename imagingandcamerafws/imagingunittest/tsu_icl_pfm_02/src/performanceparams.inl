// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// performancetest.inl
// 
//

#define KPNGImg16M	_S("c:\\tsu_icl_pfm_02\\png-16M.png")
#define KPNGImg16MA _S("c:\\tsu_icl_pfm_02\\png-16MA.png")

#define KGIFImg480x640		_S("c:\\tsu_icl_pfm_02\\vga.gif")
#define KGIFImg240x320		_S("c:\\tsu_icl_pfm_02\\gif256colours.gif")

#define K16Mto16M		_S("16M to 16M")
#define K16Mto16MU		_S("16M to 16MU")
#define K16Mto16MA		_S("16M to 16MA")
#define K16Mto16MAP		_S("16M to 16MAP")
#define K16MAto16MA		_S("16MA to 16MA")
#define K16MAto16MAP	_S("16MA to 16MAP")
#define K16MAto16MU		_S("16MA to 16MU")

#define KIDXto16MU		_S("Index to 16MU")
#define KIDXto16M		_S("Index to 16M")
#define KIDXto64K		_S("Index to 64K")
#define KIDXto64Kdoff		_S("Index to 64K dithering off")

// NOTE: PNG doesn't use premultiplied alpha, 
//       so there is no reason to have a 16MAP to 16MAP


/*
These are codec performance test parameters
*/

const TPerformanceTestParams KPNG16m_to16mSize = 
	{
	KPNGImg16M,							// source file name
	NULL,								// reference file name
	EColor16M,							// decoding display mode
	0,									// reduction factor
	70000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16Mto16M
	};
	
const TPerformanceTestParams KPNG16m_to16muSize = 
	{
	KPNGImg16M,							// source file name
	NULL,								// reference file name
	EColor16MU,							// decoding display mode
	0,									// reduction factor
	70000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16Mto16MU
	};

const TPerformanceTestParams KPNG16m_to16maSize = 
	{
	KPNGImg16M,							// source file name
	NULL,								// reference file name
	EColor16MA,							// decoding display mode
	0,									// reduction factor
	70000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16Mto16MA
	};

const TPerformanceTestParams KPNG16m_to16mapSize = 
	{
	KPNGImg16M,							// source file name
	NULL,								// reference file name
	EColor16MAP,						// decoding display mode
	0,									// reduction factor
	70000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16Mto16MAP
	};

const TPerformanceTestParams KPNG16ma_to16maSize = 
	{
	KPNGImg16MA,						// source file name
	NULL,								// reference file name
	EColor16MA,							// decoding display mode
	0,									// reduction factor
	150000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16MAto16MA
	};

const TPerformanceTestParams KPNG16ma_to16mapSize = 
	{
	KPNGImg16MA,						// source file name
	NULL,								// reference file name
	EColor16MAP,						// decoding display mode
	0,									// reduction factor
	150000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16MAto16MAP
	};

const TPerformanceTestParams KPNG16ma_to16muSize = 
	{
	KPNGImg16MA,						// source file name
	NULL,								// reference file name
	EColor16MU,							// decoding display mode
	0,									// reduction factor
	180000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	K16MAto16MU
	};

const TPerformanceTestParams KGIF_to64kSize = 
	{
	KGIFImg480x640,						// source file name
	NULL,								// reference file name
	EColor64K,							// decoding display mode
	0,									// reduction factor
	400000,								// in microseconds for H4 board (was 964125 ms)
	CImageDecoder::EOptionNone,	// TOptions parameter
	KIDXto64K
	};

const TPerformanceTestParams KGIF_to64kSize_doff = 
	{
	KGIFImg480x640,						// source file name
	NULL,								// reference file name
	EColor64K,							// decoding display mode
	0,									// reduction factor
	220000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	KIDXto64Kdoff
	};

const TPerformanceTestParams KGIF_to16mSize = 
	{
	KGIFImg480x640,						// source file name
	NULL,								// reference file name
	EColor16M,							// decoding display mode
	0,									// reduction factor
	220000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	KIDXto16M
	};

const TPerformanceTestParams KGIF_to16muSize = 
	{
	KGIFImg480x640,						// source file name
	NULL,								// reference file name
	EColor16MU,							// decoding display mode
	0,									// reduction factor
	220000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	KIDXto16MU
	};

const TPerformanceTestParams KGIF2_to64kSize = 
	{
	KGIFImg240x320,						// source file name
	NULL,								// reference file name
	EColor64K,							// decoding display mode
	0,									// reduction factor
	125000,								// in microseconds for H4 board (was 328598 ms)
	CImageDecoder::EOptionNone,	// TOptions parameter
	KIDXto64K
	};

const TPerformanceTestParams KGIF2_to64kSize_doff = 
	{
	KGIFImg240x320,						// source file name
	NULL,								// reference file name
	EColor64K,							// decoding display mode
	0,									// reduction factor
	100000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	KIDXto64Kdoff
	};

const TPerformanceTestParams KGIF2_to16mSize = 
	{
	KGIFImg240x320,						// source file name
	NULL,								// reference file name
	EColor16M,							// decoding display mode
	0,									// reduction factor
	100000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	KIDXto16M
	};

const TPerformanceTestParams KGIF2_to16muSize = 
	{
	KGIFImg240x320,						// source file name
	NULL,								// reference file name
	EColor16MU,							// decoding display mode
	0,									// reduction factor
	100000,								// in microseconds for H4 board
	CImageDecoder::EPreferFastDecode,	// TOptions parameter
	KIDXto16MU
	};

