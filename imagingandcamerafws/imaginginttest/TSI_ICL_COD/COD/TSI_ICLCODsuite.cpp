
// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This main DLL entry point for the TSI_ICL_COD.dll
// 
//


// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "TSI_ICLCODstep.h"
#include "TSI_ICLCODsuite.h"



/**
 * NewTestSuite is exported at ordinal 1
 * this provides the interface to allow schedule test
 * to create instances of this test suite
 */
EXPORT_C CTestSuite* NewTestSuiteL()
    {
	CTsiIclCodSuite* result = new (ELeave) CTsiIclCodSuite;
	CleanupStack::PushL(result);
	result->ConstructL();
	CleanupStack::Pop(); // result
	return result;
    }

/**
 * destructor
 */
CTsiIclCodSuite::~CTsiIclCodSuite()
	{
	}

/**
 * make a version string available for test system
 * @return version string
 */
TPtrC CTsiIclCodSuite::GetVersion()
	{
	_LIT(KTxtVersion,"1.0");
	return KTxtVersion();
	}

/**
 * Constructor for test suite
 * this creates all the test steps and 
 * stores them inside CTestMmfVclntSuite 
 */
void CTsiIclCodSuite::InitialiseL()
	{
	// store the name of this test suite 
	iSuiteName = _L("ICL_COD");

	// Load / decode a named pic file
	// See set of reported errors from Nile below

	// NB we derive a step name from the pic type and pic number
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 42 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 47 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 51 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 53 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 64 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 74 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 75 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 79 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 80 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 85 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 86 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 88 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 96 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 97 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 106 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 117 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 128 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 139 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 150 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 154 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 160 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeBMP, 161 ) );


	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 20 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 22 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 47 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 49 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 55 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 62 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 105 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 113 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 116 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 117 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 118 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 119 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 120 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 121 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 122 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 123 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 124 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 125 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 126 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 127 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 128 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 129 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 130 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 149 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 150 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 168 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 169 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 182 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 183 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 184 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 199 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 200 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 232 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 233 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 254 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 255 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 268 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 269 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 431 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 432 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 513 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 514 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 628 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 654 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 655 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 680 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 681 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 706 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 707 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 719 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 720 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 740 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 741 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 742 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 743 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 744 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypePNG, 745 ) );

	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 43 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 132 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 147 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 152 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 245 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 250 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 254 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 259 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 289 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 293 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 295 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 296 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 303 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 304 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 305 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 306 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 308 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 309 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 310 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 315 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 316 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 317 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 318 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 319 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 320 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 321 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 322 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 323 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 324 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 325 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 326 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 344 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 345 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 351 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 356 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 361 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 387 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 390 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 395 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 423 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 424 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 425 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 461 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 466 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 471 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 474 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 475 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 476 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 477 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 478 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 479 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 480 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 481 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 482 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 483 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 484 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 485 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 486 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 487 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 488 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 489 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 530 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 535 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 572 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 577 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 597 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 602 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 607 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 623 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 628 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 650 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 655 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 667 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 668 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 669 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 670 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 673 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 678 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 679 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 680 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 681 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 684 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 689 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 694 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 698 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 700 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 708 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 710 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 711 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 713 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 714 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 715 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 720 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 721 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 724 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 725 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 726 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 727 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 749 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 750 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 848 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 853 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 887 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 892 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 896 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 898 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 906 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 907 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 908 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 909 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 911 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 922 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 923 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 924 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 925 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 926 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 947 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 948 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 964 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 967 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 972 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 1000 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 1001 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 1002 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 1003 ) );
	AddTestStepL( new (ELeave) RIclCodStep( EPicTypeJPG, 1004 ) );	//DEF037577
	}


/* Nile reported errors on pic loading, from benchmark

BMP

42 - MediaServer USER 30
53 - MediaServer USER 30
64 - MediaServer USER 30
74 - MediaServer USER 30
75 - MediaServer USER 30
79 - MediaServer USER 30
80 - MediaServer USER 30
85 - MediaServer USER 30
86 - MediaServer USER 30
88 - MediaServer USER 44
96 - MediaServer USER 30
97 - MediaServer USER 44
106 - MediaServer USER 30
117 - MediaServer USER 30
128 - MediaServer USER 30
139 - MediaServer USER 30
150 - MediaServer USER 30
154 - MediaServer USER 30
160 - MediaServer USER 30
161 - MediaServer USER 30

PNG

105 - MediaServer USER 20
113 - MediaServer USER 20
116 - MediaServer USER 29
117 - MediaServer zlib 10
118 - MediaServer USER 29
119 - MediaServer zlib 10
120 - MediaServer zlib 10
121 - MediaServer zlib 10
122 - MediaServer zlib 10
123 - MediaServer zlib 10
124 - MediaServer zlib 10
125 - MediaServer zlib 10
126 - MediaServer zlib 10
127 - MediaServer zlib 10
128 - MediaServer zlib 10
129 - MediaServer zlib 10
149 - MediaServer KERN-EXEC 3
150 - MediaServer KERN-EXEC 3
168 - MediaServer KERN-EXEC 3
169 - MediaServer KERN-EXEC 3
182 - MediaServer KERN-EXEC 3
183 - MediaServer KERN-EXEC 3
184 - MediaServer KERN-EXEC 3
199 - MediaServer KERN-EXEC 3
200 - MediaServer KERN-EXEC 3
232 - MediaServer KERN-EXEC 3
233 - MediaServer KERN-EXEC 3
254 - MediaServer KERN-EXEC 3
255 - MediaServer KERN-EXEC 3
268 - MediaServer KERN-EXEC 3
269 - MediaServer KERN-EXEC 3
431 - MediaServer KERN-EXEC 3
432 - MediaServer KERN-EXEC 3
513 - MediaServer KERN-EXEC 3
514 - MediaServer KERN-EXEC 3
628 - MediaServer zlib 5
654 - MediaServer zlib 5
655 - MediaServer zlib 5
680 - MediaServer zlib 5
681 - MediaServer zlib 5
706 - MediaServer zlib 5
707 - MediaServer zlib 5
719 - MediaServer zlib 5
720 - MediaServer zlib 5
740 - MediaServer zlib 10
741 - MediaServer zlib 10
742 - MediaServer zlib 10
743 - MediaServer zlib 10

  JPG

Cameron 2.50

JPeg 

132 - MediaServer USER 23
147 - MediaServer USER 23
152 - MediaServer USER 23
254 - MediaServer USER 3
259 - MediaServer KERN-EXEC 3
289 - MediaServer USER 21
293 - MediaServer USER 21
295 - MediaServer KERN-EXEC 3
296 - MediaServer KERN-EXEC 3
303 - MediaServer KERN-EXEC 3
304 - MediaServer KERN-EXEC 3
305 - MediaServer KERN-EXEC 3
306 - MediaServer KERN-EXEC 3
308 - Freezes the phone
309 - Freezes the phone
310 - Freezes the phone
315 - Freezes the phone
316 - Freezes the phone
317 - Freezes the phone
318 - Freezes the phone
320 - Freezes the phone
321 - Freezes the phone
322 - Freezes the phone
323 - Freezes the phone
324 - Freezes the phone
325 - Freezes the phone
326 - Freezes the phone
. .
 .  huffman group skipped
351 - MediaServer USER 21
356 - MediaServer USER 21
361 - MediaServer USER 21
387 - Freezes the phone
390 - Freezes the phone
395 - Freezes the phone
423 - MediaServer KERN-EXEC 3
424 - MediaServer KERN-EXEC 3
425 - MediaServer KERN-EXEC 3
461 - MediaServer USER 21
466 - MediaServer USER 21
471 - MediaServer USER 21
530 - MediaServer USER 21
535 - MediaServer USER 21
572 - MediaServer USER 22
577 - MediaServer USER 22
597 - MediaServer USER 22
602 - MediaServer USER 21
607 - MediaServer USER 21
623 - MediaServer USER 21
628 - MediaServer USER 21
689 - MediaServer USER 21
694 - MediaServer USER 21
698 - MediaServer USER 21
700 - MediaServer KERN-EXEC 3
708 - MediaServer KERN-EXEC 3
710 - MediaServer KERN-EXEC 3
711 - MediaServer KERN-EXEC 3
713 - Freezes the phone
715 - Freezes the phone
720 - Freezes the phone
721 - Freezes the phone
.
.
. progressive huffman skipped
887 - MediaServer USER 21
892 - MediaServer USER 21
896 - MediaServer USER 21
898 - MediaServer KERN-EXEC 3
906 - MediaServer KERN-EXEC 3
907 - MediaServer KERN-EXEC 3
908 - MediaServer KERN-EXEC 3
909 - MediaServer KERN-EXEC 3
911 - Freezes the phone
.
.
. extended huffman skipped
964 - Freezes the phone
967 - Freezes the phone
972 - Freezes the phone
1000 - MediaServer KERN-EXEC 3
1001 - MediaServer KERN-EXEC 3
1002 - MediaServer KERN-EXEC 3



 */

 
