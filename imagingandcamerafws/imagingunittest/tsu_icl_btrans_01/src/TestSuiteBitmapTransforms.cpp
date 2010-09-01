// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "TestStepBitmapTransforms.h"
#include "TestSuiteBitmapTransforms.h"
#include "TestStepRotate.h"
#include "TestStepScale.h"
#include "TestPostProcessScale.h"
#include "TestLowMemoryScale.h"
#include "TestFastScale.h"


_LIT(KRgbBmp, 	"c:\\EColor16MIclBtrans.mbm");
_LIT(KAlphaBmp,	"c:\\Btrans\\EGray256Btrans.mbm");

/**
*
* Create an instance of this test suite.
*
* NewTestSuiteBitmapTransformsL is exported at ordinal 1.
* This provides the interface to allow the Test Framework
* to create instances of this test suite.
*
* @return	"CTestSuiteBitmapTransforms*"
*			The newly created test suite
*/
EXPORT_C CTestSuiteBitmapTransforms* NewTestSuiteBitmapTransformsL() 
    { 
	CTestSuiteBitmapTransforms* self = new (ELeave) CTestSuiteBitmapTransforms;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
    }

/**
*
* CTestSuiteBitmapTransforms destructor
*
* 
*/
CTestSuiteBitmapTransforms::~CTestSuiteBitmapTransforms()
	{
	}

/**
*
* This test suite version string
*
*
*/
_LIT(KTxtVersion,"0.1");

/**
*
* Get test suite version.
*
* @return	"TPtrC"
*			The version string.
*
*
*/
TPtrC CTestSuiteBitmapTransforms::GetVersion( )
	{
	return KTxtVersion();
	}

/**
*
* Add a test step into the suite.
*
* @param	"CTestStepBitmapTransforms* aPtrTestStep"
*			The test step to be added.
* 
*/
void CTestSuiteBitmapTransforms::AddTestStepL( CTestStepBitmapTransforms* aPtrTestStep )
	{
    // [ assert pre condition that a valid test ptr has been supplied ]
	__ASSERT_ALWAYS( aPtrTestStep, Panic( EBadArgument ) );
	
	// [ set back wards reference to test suite ]
	aPtrTestStep->SetSuite(this); 
	
	//[ add the step to the test suite ]
	CTestSuite::AddTestStepL(aPtrTestStep);
	}

/**
*
* Initialiser for test suite.
* This creates all the test steps and stores them
* inside CTestSuiteBitmapTransforms
* 
*/
void CTestSuiteBitmapTransforms::InitialiseL( )
	{
	//[ set the default path ]
	InitSystemPath();

	// store the name of this test suite
	iSuiteName = _L("BitmapTransforms");
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CTestStepRotate90Degrees  );
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CTestStepRotate180Degrees  );
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CTestStepRotate270Degrees  );
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave) CTestStepMirrorHorizontal );
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave) CTestStepMirrorVertical );
	
	//[add scale test step to the test suite ]
	AddTestStepL( new(ELeave) CTestStepScale );
	
	AddTestStepL( new(ELeave) CTestSrcBitmapPanicAlloc );
	
	AddTestStepL( new(ELeave) CTestTgtBitmapPanicAlloc );
	
	AddTestStepL( new(ELeave) CTestScaleAllocFailure  );
	
	AddTestStepL( new(ELeave) CTestStepRotate16Ma(_L("MM-ICL-BTRANS-U-300-HP"),KRgbBmp,KAlphaBmp) );
	AddTestStepL( new(ELeave) CTestStepScale16MA(_S("MM-ICL-BTRANS-U-301-HP"),KRgbBmp,KAlphaBmp) ); 
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave) CTestStepRotate );
	
	//[ addd Rotate with zero sized bitmap ]
	AddTestStepL( new(ELeave) CTestStepZeroOnBothDimensions );
	
    //[ addd Rotate with zero sized bitmap ]
	AddTestStepL( new(ELeave) CTestStepOneByY  );
	
	
    //[ addd Rotate with zero sized bitmap ]
	AddTestStepL( new(ELeave) CTestStepXbyOne );
	
	// [ add tests for rotate and mirror on 1 by N bitmap ]
	AddTestStepL( new(ELeave) CTestStepFlipOneByN90 );
	AddTestStepL( new(ELeave) CTestStepFlipOneByN180 );
	AddTestStepL( new(ELeave) CTestStepFlipOneByN270 );
	AddTestStepL( new(ELeave) CTestStepFlipOneByNHorizontal );
	AddTestStepL( new(ELeave) CTestStepFlipOnebyNVertical );
	
	// [ add tests for rotate and mirror on N by One bitmap ]
	AddTestStepL( new(ELeave) CTestStepFlipNByOne90 );
	AddTestStepL( new(ELeave) CTestStepFlipNByOne180 );
	AddTestStepL( new(ELeave) CTestStepFlipNByOne270 );
	AddTestStepL( new(ELeave) CTestStepFlipNByOneHorizontal );
	AddTestStepL( new(ELeave) CTestStepFlipNByOneVertical );
	
	// add scale zero src bitmaps 3 off
	AddTestStepL( new(ELeave) CTestStepScaleZeroByZeroSrc );
	AddTestStepL( new(ELeave) CTestStepScaleZeroByNSrc );
	AddTestStepL( new(ELeave) CTestStepScaleNByZeroSrc );
	
	// [add scale 1by N and N by 1 1:1 and compare tests]
	AddTestStepL( new(ELeave) CTestStepScaleNByOne  );
	AddTestStepL( new(ELeave) CTestStepScaleOneByN );
	
	// [ add test to generate checker pattern 
	//   scale by 2 and 1/2 and compare with original ]
	AddTestStepL( new(ELeave) CTestStepScaleUpAndDown);
	
	//[ add force scale memory alloc failure test ]
	AddTestStepL( new(ELeave) CTestForceScaleMemoryAllocFailure );
	
	//[ add force rotate memory alloc failure test ]
	AddTestStepL( new(ELeave) CTestForceRotateMemoryAllocFailure );

	//[ add force rotate memory alloc failure test ]
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 0 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 1 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 2 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 3 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 4 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 5 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 6 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 7 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 8 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 9 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 10 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 11 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 12 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 13 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 14 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 15 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 16 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 17 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 18 ) );
	AddTestStepL( new(ELeave) CTestStepPostProcessScale( 19 ) );

	//[ add force rotate memory alloc failure test ]
	AddTestStepL( new(ELeave) CTestStepCancelScale );

	// [ add tests for scaling a bitmap with 16MU display mode ]
	AddTestStepL( new(ELeave) CTestStepDisplayModeScale );
	
	//DEF045698 - Test step to check if the scalar rounds the target size correctly
	AddTestStepL( new(ELeave) CTestStepScaleRoundoff );

	// [ add tests for scaling a bitmap using the low memory algorithm ]
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 0 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 1 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 2 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 3 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 4 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 5 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 6 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 7 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 8 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 9 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 10 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 11 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 12 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 13 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 14 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 15 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 16 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 17 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 18 ) );
	AddTestStepL( new(ELeave) CTestStepLowMemoryScale( 19 ) );
	
// fast scaling
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 0 ,_S("MM-ICL-BTRANS-U-120-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 1 ,_S("MM-ICL-BTRANS-U-121-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 2 ,_S("MM-ICL-BTRANS-U-122-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 3 ,_S("MM-ICL-BTRANS-U-123-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 4 ,_S("MM-ICL-BTRANS-U-124-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 5 ,_S("MM-ICL-BTRANS-U-125-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 6 ,_S("MM-ICL-BTRANS-U-126-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 7 ,_S("MM-ICL-BTRANS-U-127-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 8 ,_S("MM-ICL-BTRANS-U-128-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 9 ,_S("MM-ICL-BTRANS-U-129-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 10 ,_S("MM-ICL-BTRANS-U-130-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 11 ,_S("MM-ICL-BTRANS-U-131-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 12 ,_S("MM-ICL-BTRANS-U-132-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 13 ,_S("MM-ICL-BTRANS-U-133-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 14 ,_S("MM-ICL-BTRANS-U-134-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 15 ,_S("MM-ICL-BTRANS-U-135-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 16 ,_S("MM-ICL-BTRANS-U-136-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 17 ,_S("MM-ICL-BTRANS-U-137-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 18 ,_S("MM-ICL-BTRANS-U-138-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleXY( 19 ,_S("MM-ICL-BTRANS-U-139-HP")) );


//
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 0 ,_S("MM-ICL-BTRANS-U-140-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 1 ,_S("MM-ICL-BTRANS-U-141-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 2 ,_S("MM-ICL-BTRANS-U-142-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 3 ,_S("MM-ICL-BTRANS-U-143-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 4 ,_S("MM-ICL-BTRANS-U-144-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 5 ,_S("MM-ICL-BTRANS-U-145-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 6 ,_S("MM-ICL-BTRANS-U-146-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 7 ,_S("MM-ICL-BTRANS-U-147-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 8 ,_S("MM-ICL-BTRANS-U-148-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 9 ,_S("MM-ICL-BTRANS-U-149-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 10 ,_S("MM-ICL-BTRANS-U-150-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 11 ,_S("MM-ICL-BTRANS-U-151-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 12 ,_S("MM-ICL-BTRANS-U-152-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 13 ,_S("MM-ICL-BTRANS-U-153-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 14 ,_S("MM-ICL-BTRANS-U-154-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 15 ,_S("MM-ICL-BTRANS-U-155-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 16 ,_S("MM-ICL-BTRANS-U-156-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 17 ,_S("MM-ICL-BTRANS-U-157-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 18 ,_S("MM-ICL-BTRANS-U-158-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleX( 19 ,_S("MM-ICL-BTRANS-U-159-HP")) );

	
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 0 ,_S("MM-ICL-BTRANS-U-160-HP")) );
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 1 ,_S("MM-ICL-BTRANS-U-161-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 2 ,_S("MM-ICL-BTRANS-U-162-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 3 ,_S("MM-ICL-BTRANS-U-163-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 4 ,_S("MM-ICL-BTRANS-U-164-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 5 ,_S("MM-ICL-BTRANS-U-165-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 6 ,_S("MM-ICL-BTRANS-U-166-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 7 ,_S("MM-ICL-BTRANS-U-167-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 8 ,_S("MM-ICL-BTRANS-U-168-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 9 ,_S("MM-ICL-BTRANS-U-169-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 10 ,_S("MM-ICL-BTRANS-U-170-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 11 ,_S("MM-ICL-BTRANS-U-171-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 12 ,_S("MM-ICL-BTRANS-U-172-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 13 ,_S("MM-ICL-BTRANS-U-173-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 14 ,_S("MM-ICL-BTRANS-U-174-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 15 ,_S("MM-ICL-BTRANS-U-175-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 16 ,_S("MM-ICL-BTRANS-U-176-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 17 ,_S("MM-ICL-BTRANS-U-177-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 18 ,_S("MM-ICL-BTRANS-U-178-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleY( 19 ,_S("MM-ICL-BTRANS-U-179-HP")));


	// copy 
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 0 ,_S("MM-ICL-BTRANS-U-180-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 1 ,_S("MM-ICL-BTRANS-U-181-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 2 ,_S("MM-ICL-BTRANS-U-182-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 3 ,_S("MM-ICL-BTRANS-U-183-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 4 ,_S("MM-ICL-BTRANS-U-184-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 5 ,_S("MM-ICL-BTRANS-U-185-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 6 ,_S("MM-ICL-BTRANS-U-186-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 7 ,_S("MM-ICL-BTRANS-U-187-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 8 ,_S("MM-ICL-BTRANS-U-188-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 9 ,_S("MM-ICL-BTRANS-U-189-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 10 ,_S("MM-ICL-BTRANS-U-190-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 11 ,_S("MM-ICL-BTRANS-U-191-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 12 ,_S("MM-ICL-BTRANS-U-192-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 13 ,_S("MM-ICL-BTRANS-U-193-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 14 ,_S("MM-ICL-BTRANS-U-194-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 15 ,_S("MM-ICL-BTRANS-U-195-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 16 ,_S("MM-ICL-BTRANS-U-196-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 17 ,_S("MM-ICL-BTRANS-U-197-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 18 ,_S("MM-ICL-BTRANS-U-198-HP")));
	AddTestStepL( new(ELeave) CTestStepFastScaleCopy( 19 ,_S("MM-ICL-BTRANS-U-199-HP")));
	AddTestStepL( new(ELeave) CTestStepScaleWhiteFillMaxQuality());
	AddTestStepL( new(ELeave) CTestStepScaleWhiteFillMedQuality());
	AddTestStepL( new(ELeave) CTestStepScaleWhiteFillMiniQuality());
	AddTestStepL( new(ELeave) CTestStepScaleWhiteFill64K());
	AddTestStepL( new(ELeave) CTestStepScaleSrcWhiteFill());
	
	AddTestStepL( new(ELeave) CTestStepDEFScale(_S("MM-ICL-BTRANS-U-205-HP")));
	AddTestStepL( new(ELeave) CTestStepDEFScale(_S("MM-ICL-BTRANS-U-206-HP")));
	AddTestStepL( new(ELeave) CTestStepDEFScale(_S("MM-ICL-BTRANS-U-207-HP")));
	AddTestStepL( new(ELeave) CTestStepDEFScale(_S("MM-ICL-BTRANS-U-208-HP")));
	AddTestStepL( new(ELeave) CTestStepDEFScale(_S("MM-ICL-BTRANS-U-209-HP")));
	AddTestStepL( new(ELeave) CTestStepDEFScale(_S("MM-ICL-BTRANS-U-210-HP")));
	
	AddTestStepL( new(ELeave) CTestStepLowQtyScale(_S("MM-ICL-BTRANS-U-220-HP")));
	AddTestStepL( new(ELeave) CTestStepFastDownScale(_S("MM-ICL-BTRANS-U-302-HP")));
	AddTestStepL( new(ELeave) CTestCustomCommandCheck());
	AddTestStepL( new(ELeave) CTestBitmapScaleMaxMedMinSpeed(_S("MM-ICL-BTRANS-U-303-HP")));
	AddTestStepL( new(ELeave) CTestBitmapScalerOverFlow(_S("MM-ICL-BTRANS-U-304-HP")));
		
	AddTestStepL( new(ELeave) CTestStepScaleMemoryLeak());
	
	//DEF120342: CBitmap Rotator, banding occurs in a bitmap rotated 180 degress
	// MM-ICL-BTRANS-U-306-HP
	AddTestStepL( new(ELeave) CTestStepRotate180DegreesMbm());
	
	AddTestStepL( new(ELeave) CTestStepRotateExtendedBitMap());

	}

/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteBitmapTransforms::InitSystemPath()
	{
	_LIT(KVideoTestSourcePathOnE,"e:\\Btrans\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\Btrans\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\Btrans\\");

	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	//[ take the first drive where it exists or can be made ]
	TInt err = fs.MkDir(KVideoTestSourcePathOnC);
	if (( err == KErrNone ) || (err == KErrAlreadyExists))
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else{
        TInt err = fs.MkDir(KVideoTestSourcePathOnD);
        if (( err == KErrNone ) || (err == KErrAlreadyExists))
        	{
			iDefaultPath = KVideoTestSourcePathOnD;
        	}
		else
			{
			TInt err = fs.MkDir(KVideoTestSourcePathOnE);
			if (( err == KErrNone ) || (err == KErrAlreadyExists))
				{
				iDefaultPath = KVideoTestSourcePathOnE;
				}
			else
				{
				User::Panic(_L("Could not make Btrans directory"),KErrGeneral);
				}			
			}
		}

	fs.Close();

	}
	
const TDesC& CTestSuiteBitmapTransforms::DefaultPath() const
	{
	return iDefaultPath;
	}
	
	
