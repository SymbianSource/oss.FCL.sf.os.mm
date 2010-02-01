// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>

#include "JpegTypes.h"

/**
   @file
   @internalComponent
*/

// shareable static data to go to this file

extern
const TAlignedZigZag KZigZagSequence = 
    {
	0,	1,	8,	16,	9,	2,	3,	10,
	17,	24,	32,	25,	18,	11,	4,	5,
	12,	19,	26,	33,	40,	48,	41,	34,
	27,	20,	13,	6,	7,	14,	21,	28,
	35,	42,	49,	56,	57,	50,	43,	36,
	29,	22,	15,	23,	30,	37,	44,	51,
	58,	59,	52,	45,	38,	31,	39,	46,
	53,	60,	61,	54,	47,	55,	62,	63	
	};
	

