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

#define KMagn1_MX5_MY3		_S("MAGN1-MX5-MY3.mng")
#define KMagn2_MX8_MY4		_S("MAGN2-MX8-MY4.mng")
#define KMagn3_MX8_MY4		_S("MAGN3-MX8-MY4.mng")


const TUint KZer=0x00000000u;
const TUint KWhi=0x00ffffffu;
const TUint KRed=0x000000ffu;
const TUint KGre=0x0000ff00u;
const TUint KBlu=0x00ff0000u;

const TUint KC01=0x00010101u;
const TUint KC02=0x00020202u;
const TUint KC03=0x00030303u;
const TUint KC04=0x00040404u;
const TUint KC05=0x00050505u;

const TUint KC06=0x00060606u;
const TUint KC07=0x00070707u;
const TUint KC08=0x00080808u;
const TUint KC09=0x00090909u;
const TUint KC10=0x000a0a0au;

const TUint KC11=0x000b0b0bu;
const TUint KC12=0x000c0c0cu;
const TUint KC13=0x000d0d0du;
const TUint KC14=0x000e0e0eu;
const TUint KC15=0x000f0f0fu;

const TUint KC16=0x00101010u;
const TUint KC17=0x00111111u;

const TUint KFAl=0x000000ffu; // Full alpha
const TUint KHAl=0x00000082u; // Half alpha
const TUint KQAl=0x00000042u; // Quarter alpha

const TUint KMagn1_MX5_MY3RefCol[17*8] =
	{
		KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi,
		KWhi, KRed, KRed, KRed, KRed, KRed, KGre, KGre, KGre, KGre, KGre, KRed, KRed, KRed, KRed, KRed, KWhi,
		KWhi, KRed, KRed, KRed, KRed, KRed, KGre, KGre, KGre, KGre, KGre, KRed, KRed, KRed, KRed, KRed, KWhi,
		KWhi, KRed, KRed, KRed, KRed, KRed, KGre, KGre, KGre, KGre, KGre, KRed, KRed, KRed, KRed, KRed, KWhi,
		KWhi, KGre, KGre, KGre, KGre, KGre, KBlu, KBlu, KBlu, KBlu, KBlu, KGre, KGre, KGre, KGre, KGre, KWhi,
		KWhi, KGre, KGre, KGre, KGre, KGre, KBlu, KBlu, KBlu, KBlu, KBlu, KGre, KGre, KGre, KGre, KGre, KWhi,
		KWhi, KGre, KGre, KGre, KGre, KGre, KBlu, KBlu, KBlu, KBlu, KBlu, KGre, KGre, KGre, KGre, KGre, KWhi,
		KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi
	};

const TUint KMagn1_MX5_MY3RefAlpha[17*8] =
	{
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KQAl, KQAl, KQAl, KQAl, KQAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl,
		KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KQAl, KQAl, KQAl, KQAl, KQAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl,
		KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KQAl, KQAl, KQAl, KQAl, KQAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl
	};

const TUint KMagn2_MX8_MY4RefCol[19*7]=
	{
	//   1     2      3     4     5    6     7     8     9     10    11    12    13    14    15    16    17    18    19
		KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi,
		KWhi, KC01, KC02, KC03, KC04, KC05, KC06, KC07, KC08, KC09, KC08, KC07, KC06, KC05, KC04, KC03, KC02, KC01, KWhi,
		KWhi, KC03, KC04, KC05, KC06, KC07, KC08, KC09, KC10, KC11, KC10, KC09, KC08, KC07, KC06, KC05, KC04, KC03, KWhi,
		KWhi, KC05, KC06, KC07, KC08, KC09, KC10, KC11, KC12, KC13, KC12, KC11, KC10, KC09, KC08, KC07, KC06, KC05,	KWhi,
		KWhi, KC07, KC08, KC09, KC10, KC11, KC12, KC13, KC14, KC15, KC14, KC13, KC12, KC11, KC10, KC09, KC08, KC07, KWhi,
		KWhi, KC09, KC10, KC11, KC12, KC13, KC14, KC15, KC16, KC17, KC16, KC15, KC14, KC13, KC12, KC11, KC10, KC09, KWhi,
		KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi
	};

const TUint KMagn2_MX8_MY4RefAlpha[19*7] =
	{
	//   1     2      3     4     5    6     7     8     9     10    11    12    13    14    15    16    17    18    19
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, 1,    2,    3,    4,    5,    6,    7,    8,    9,    8,    7,    6,    5,    4,    3,    2,    1,    KFAl,
		KFAl, 3,    4,    5,    6,    7,    8,    9,    10,   11,   10,   9,    8,    7,    6,    5,    4,    3,    KFAl,
		KFAl, 5,    6,    7,    8,    9,    10,   11,   12,   13,   12,   11,   10,   9,    8,    7,    6,    5,	KFAl,
		KFAl, 7,    8,    9,    10,   11,   12,   13,   14,   15,   14,   13,   12,   11,   10,   9,    8,    7,    KFAl,
		KFAl, 9,    10,   11,   12,   13,   14,   15,   16,   17,   16,   15,   14,   13,   12,   11,   10,   9,    KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl
	};

const TUint KMagn3_MX8_MY4RefCol[19*7]=
	{
	//   1     2      3     4     5    6     7     8     9     10    11    12    13    14    15    16    17    18    19
		KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi,

		KWhi, KRed, KRed, KRed, KRed, KRed, KGre, KGre, KGre, KGre, KGre, KGre, KGre, KGre, KRed, KRed, KRed, KRed, KWhi,
		KWhi, KRed, KRed, KRed, KRed, KRed, KGre, KGre, KGre, KGre, KGre, KGre, KGre, KGre, KRed, KRed, KRed, KRed, KWhi,
		KWhi, KRed, KRed, KRed, KRed, KRed, KGre, KGre, KGre, KGre, KGre, KGre, KGre, KGre, KRed, KRed, KRed, KRed, KWhi,

		KWhi, KGre, KGre, KGre, KGre, KGre, KBlu, KBlu, KBlu, KBlu, KBlu, KBlu, KBlu, KBlu, KGre, KGre, KGre, KGre, KWhi,
		KWhi, KGre, KGre, KGre, KGre, KGre, KBlu, KBlu, KBlu, KBlu, KBlu, KBlu, KBlu, KBlu, KGre, KGre, KGre, KGre, KWhi,

		KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi, KWhi
	};

const TUint KMagn3_MX8_MY4RefAlpha[19*7] =
	{
	//   1     2      3     4     5    6     7     8     9     10    11    12    13    14    15    16    17    18    19
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl,

		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl, KFAl, KFAl, KFAl, KFAl,
		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KHAl, KFAl, KFAl, KFAl, KFAl, KFAl,

		KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KQAl, KQAl, KQAl, KQAl, KQAl, KQAl, KQAl, KQAl, KHAl, KHAl, KHAl, KHAl, KFAl,
		KFAl, KHAl, KHAl, KHAl, KHAl, KHAl, KQAl, KQAl, KQAl, KQAl, KQAl, KQAl, KQAl, KQAl, KHAl, KHAl, KHAl, KHAl, KFAl,


		KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl, KFAl
	};
	
const TUint K16MA_Standalone_Magn1_MX5_MY3RefCol[17*8] =
	{
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffffffff,
	0xffffffff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffffffff,
	0xffffffff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

const TUint K16MA_Standalone_Magn2_MX8_MY4RefCol[19*7] =
	{
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0x1010101,	0x2020202,	0x3030303,	0x4040404,	0x5050505,	0x6060606,	0x7070707,	0x8080808,	0x9090909,	0x8080808,	0x7070707,	0x6060606,	0x5050505,	0x4040404,	0x3030303,	0x2020202,	0x1010101,	0xffffffff,
	0xffffffff, 0x3030303, 	0x4040404,	0x5050505,	0x6060606,	0x7070707,	0x8080808,	0x9090909,	0xa0a0a0a,	0xb0b0b0b,	0xa0a0a0a,	0x9090909,	0x8080808,	0x7070707,	0x6060606,	0x5050505,	0x4040404,	0x3030303,	0xffffffff,
	0xffffffff, 0x5050505, 	0x6060606,	0x7070707,	0x8080808,	0x9090909,	0xa0a0a0a,	0xb0b0b0b,	0xc0c0c0c,	0xd0d0d0d,	0xc0c0c0c,	0xb0b0b0b,	0xa0a0a0a,	0x9090909,	0x8080808,	0x7070707,	0x6060606,	0x5050505,	0xffffffff,
	0xffffffff, 0x7070707, 	0x8080808,	0x9090909,	0xa0a0a0a,	0xb0b0b0b,	0xc0c0c0c,	0xd0d0d0d,	0xe0e0e0e,	0xf0f0f0f,	0xe0e0e0e,	0xd0d0d0d,	0xc0c0c0c,	0xb0b0b0b,	0xa0a0a0a,	0x9090909,	0x8080808,	0x7070707,	0xffffffff,
	0xffffffff, 0x9090909, 	0xa0a0a0a,	0xb0b0b0b,	0xc0c0c0c,	0xd0d0d0d,	0xe0e0e0e,	0xf0f0f0f,	0x10101010, 0x11111111, 0x10101010, 0xf0f0f0f,	0xe0e0e0e,	0xd0d0d0d,	0xc0c0c0c,	0xb0b0b0b,	0xa0a0a0a,	0x9090909,	0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

const TUint K16MA_Standalone_Magn3_MX8_MY4RefCol[19*7]=
	{
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffffffff,
	0xffffffff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x420000ff, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0x8200ff00, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

const TUint K32Bit_Destination_Magn1_MX5_MY3RefCol[17*8] =
	{
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffffff,
	0xffffffff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffffff,
	0xffffffff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

const TUint K32Bit_Destination_Magn2_MX8_MY4RefCol[19*7] =
	{	
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xff010101, 0xff020202, 0xff030303, 0xff040404, 0xff050505, 0xff060606, 0xff070707, 0xff080808, 0xff090909, 0xff080808, 0xff070707, 0xff060606, 0xff050505, 0xff040404, 0xff030303, 0xff020202, 0xff010101, 0xffffffff,
	0xffffffff, 0xff030303, 0xff040404, 0xff050505, 0xff060606, 0xff070707, 0xff080808, 0xff090909, 0xff0a0a0a, 0xff0b0b0b, 0xff0a0a0a, 0xff090909, 0xff080808, 0xff070707, 0xff060606, 0xff050505, 0xff040404, 0xff030303, 0xffffffff,
	0xffffffff, 0xff050505, 0xff060606, 0xff070707, 0xff080808, 0xff090909, 0xff0a0a0a, 0xff0b0b0b, 0xff0c0c0c, 0xff0d0d0d, 0xff0c0c0c, 0xff0b0b0b, 0xff0a0a0a, 0xff090909, 0xff080808, 0xff070707, 0xff060606, 0xff050505, 0xffffffff,
	0xffffffff, 0xff070707, 0xff080808, 0xff090909, 0xff0a0a0a, 0xff0b0b0b, 0xff0c0c0c, 0xff0d0d0d, 0xff0e0e0e, 0xff0f0f0f, 0xff0e0e0e, 0xff0d0d0d, 0xff0c0c0c, 0xff0b0b0b, 0xff0a0a0a, 0xff090909, 0xff080808, 0xff070707, 0xffffffff,
	0xffffffff, 0xff090909, 0xff0a0a0a, 0xff0b0b0b, 0xff0c0c0c, 0xff0d0d0d, 0xff0e0e0e, 0xff0f0f0f, 0xff101010, 0xff111111, 0xff101010, 0xff0f0f0f, 0xff0e0e0e, 0xff0d0d0d, 0xff0c0c0c, 0xff0b0b0b, 0xff0a0a0a, 0xff090909, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

const TUint K32Bit_Destination_Magn3_MX8_MY4RefCol[19*7]=
	{
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffffffff,
	0xffffffff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffffff,
	0xffffffff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	};

const TImageDisplayUnitTestParams KMngMagn1_MX5_MY3 =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0041-HP"), 
		_S("MNG MAGN method 1 with MX 5 MY 3"), 
		KMagn1_MX5_MY3,					// source file name
		17,  8, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngMagn2_MX4_MY8 =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0042-HP"), 
		_S("MNG MAGN method 2 with MX 8 MY 4"), 
		KMagn2_MX8_MY4,					// source file name
		19,  7, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

const TImageDisplayUnitTestParams KMngMagn3_MX4_MY8 =
	{	
		_S("MM-ICL-MNGIMGDISP-U-0043-HP"), 
		_S("MNG MAGN method 3 with MX 8 MY 4"), 
		KMagn3_MX8_MY4,					// source file name
		19,  7, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

/*
For DEF123773: 
Tests: MM-ICL-MNGIMGDISP-U-0045-HP, MM-ICL-MNGIMGDISP-U-0046-HP, MM-ICL-MNGIMGDISP-U-0047-HP, MM-ICL-MNGIMGDISP-U-0048-HP,
MM-ICL-MNGIMGDISP-U-0049-HP, MM-ICL-MNGIMGDISP-U-0050-HP, MM-ICL-MNGIMGDISP-U-0051-HP, MM-ICL-MNGIMGDISP-U-0052-HP,
MM-ICL-MNGIMGDISP-U-0053-HP, MM-ICL-MNGIMGDISP-U-0054-HP, MM-ICL-MNGIMGDISP-U-0055-HP, MM-ICL-MNGIMGDISP-U-0056-HP
*/
const TImageDisplayUnitTestParams K32BitDestinationMngMagn1_MX5_MY3 =
	{	
		_S(""), 
		_S("MNG MAGN method 1 with MX 5 MY 3"), 
		KMagn1_MX5_MY3,					// source file name
		17,  8, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

/*
For DEF123773: 
Tests: MM-ICL-MNGIMGDISP-U-0045-HP, MM-ICL-MNGIMGDISP-U-0046-HP, MM-ICL-MNGIMGDISP-U-0047-HP, MM-ICL-MNGIMGDISP-U-0048-HP,
MM-ICL-MNGIMGDISP-U-0049-HP, MM-ICL-MNGIMGDISP-U-0050-HP, MM-ICL-MNGIMGDISP-U-0051-HP, MM-ICL-MNGIMGDISP-U-0052-HP,
MM-ICL-MNGIMGDISP-U-0053-HP, MM-ICL-MNGIMGDISP-U-0054-HP, MM-ICL-MNGIMGDISP-U-0055-HP, MM-ICL-MNGIMGDISP-U-0056-HP
*/
const TImageDisplayUnitTestParams K32BitDestinationMngMagn2_MX4_MY8 =
	{	
		_S(""), 
		_S("MNG MAGN method 2 with MX 8 MY 4"), 
		KMagn2_MX8_MY4,					// source file name
		19,  7, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

/*
For DEF123773: 
Tests: MM-ICL-MNGIMGDISP-U-0045-HP, MM-ICL-MNGIMGDISP-U-0046-HP, MM-ICL-MNGIMGDISP-U-0047-HP, MM-ICL-MNGIMGDISP-U-0048-HP,
MM-ICL-MNGIMGDISP-U-0049-HP, MM-ICL-MNGIMGDISP-U-0050-HP, MM-ICL-MNGIMGDISP-U-0051-HP, MM-ICL-MNGIMGDISP-U-0052-HP,
MM-ICL-MNGIMGDISP-U-0053-HP, MM-ICL-MNGIMGDISP-U-0054-HP, MM-ICL-MNGIMGDISP-U-0055-HP, MM-ICL-MNGIMGDISP-U-0056-HP
*/
const TImageDisplayUnitTestParams K32BitDestinationMngMagn3_MX4_MY8 =
	{	
		_S(""), 
		_S("MNG MAGN method 3 with MX 8 MY 4"), 
		KMagn3_MX8_MY4,					// source file name
		19,  7, 									// dest size
		-1,											// display mode
		CImageDisplay::EOptionMainImage,			// image option

		ETrue,										// maintain aspect ratio
		TImageDisplayUnitTestParams::ETypeFileName,	// Source is a filename
		NULL,			// Mime type
		0, 0,			// image type & subtype
		0,				// plugin UID
		TImageDisplayUnitTestParams::KNoFrameLimit,	// maximum number of frames to decode
		TImageDisplayUnitTestParams::ENormal,		// test type
		NULL										// checkpoints. 
	};

