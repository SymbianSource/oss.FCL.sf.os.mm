// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
	iSuiteName = _L("TSI_ICL_BTRANS_01");


	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0201Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0201Cp(ETrue)	);

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0202Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0202Cp(ETrue)	);
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0203Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0203Cp(ETrue)	);

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0204Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0204Cp(ETrue)	);

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0205Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0205Cp(ETrue)	);
							   
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0206Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0206Cp(ETrue)	);

	//[ add Scale test step to the test suite. Display Mode: EGray2 ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EGray2, _L("MM-ICL-BTRANS-I-0207-CP"))	);

	//[ add Scale test step to the test suite. Display Mode: EGray4 ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EGray4, _L("MM-ICL-BTRANS-I-0208-CP"))		);
	
	//[ add Scale test step to the test suite. Display Mode: EGray16 ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EGray16, _L("MM-ICL-BTRANS-I-0209-CP"))	);
	
	//[ add Scale test step to the test suite. Display Mode: EGray256 ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EGray256, _L("MM-ICL-BTRANS-I-0210-CP")	)	);

	//[ add Scale test step to the test suite. Display Mode: EColor16 ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EColor16, _L("MM-ICL-BTRANS-I-0211-CP")	)	);

	//[ add Scale test step to the test suite. Display Mode: EColor256 ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EColor256, _L("MM-ICL-BTRANS-I-0212-CP")	)	);

	//[ add Scale test step to the test suite. Display Mode: EColor64K ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EColor64K, _L("MM-ICL-BTRANS-I-0213-CP")	)	);

	//[ add Scale test step to the test suite. Display Mode: EColor16M ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EColor16M, _L("MM-ICL-BTRANS-I-0214-CP")	)	);

	//[ add Scale test step to the test suite. Display Mode: EColor4K ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EColor4K, _L("MM-ICL-BTRANS-I-0215-CP")	)	);

	//[ add Scale test step to the test suite. Display Mode: EColor16MU ]
	AddTestStepL( new(ELeave)  CMmIclBitmapScaleLowMem1(EFalse, EColor16MU, _L("MM-ICL-BTRANS-I-0216-CP")	)	);

	//[ Display Mode: EColor16MU is used for the following test steps.]
	
	//[ Scaling a 1152x864 size bitmap of type EColor16MU into 640x480 bitmap with UseMediumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScaleSpeed1(_L("MM-ICL-BTRANS-I-0217-HP")) );
	
    //[ Scaling a 576x432 size bitmap of type EColor16MU into 640x480 bitmap with UseMediumQuality Algorithm enabled. ]	
	AddTestStepL( new(ELeave) CMmIclBitmapScaleSpeed2(_L("MM-ICL-BTRANS-I-0218-HP")) );

    //[ This test checks the boundary condition of scaling a 100 x 1 bitmap to another bitmap of size 100 x 100, 
    //  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By1To100By100(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0219-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By1To100By100(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0220-HP")) );
	
    //[ This test checks the boundary condition of scaling a 1 x 100 bitmap to another bitmap of size 100 x 100, 
    //  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale1By100To100By100(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0221-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale1By100To100By100(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0222-HP")) );

    //[ Scale a bitmap up by 2 and down by 2. It compares the size of the resultant bitmap to the original, 
    //  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScaleUpAndDown(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0223-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScaleUpAndDown(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0224-HP")) );

    //[ Scale a bitmap up by 2 to the target bitmap with UseMaximumQuality Algorithm enabled.
    //  Then enable UseMediumQuality Algorithm and scale the image by 2 to itself. Check the time. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScaleMaxMedSpeed(_L("MM-ICL-BTRANS-I-0225-HP")) );
    
    //[ Scale a bitmap up by 2 to the target bitmap with UseMediumQuality Algorithm enabled. 
    //  Then enable UseMinimumQualityAlgorithm and scale the image by 2 to itself. Check the time. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScaleMedMinSpeed(_L("MM-ICL-BTRANS-I-0226-HP")) );

    //[ Scale an EColor16MU Bitmap by simultaneous stretching and squashing on both the X and Y axes, 
    //  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScaleStretchAndSquash(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0227-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScaleStretchAndSquash(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0228-HP")) );

	//[ This test checks the boundary condition of scaling a 100 x 100 bitmap to 100 x 1 bitmap 
	//  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By100To100By1(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0229-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By100To100By1(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0230-HP")) );

	//[ This test checks the boundary condition of scaling a 100 x 100 bitmap to 1 x 100 bitmap 
	//  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By100To1By100(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0231-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By100To1By100(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0232-HP")) );

	//[ Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 200 x 0 
	//  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale0By100To200By0(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0233-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale0By100To200By0(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0234-HP")) );

	//[ Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
	//  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale0By100To100By100(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0306-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale0By100To100By100(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0307-HP")) );

	//[ Create a 0 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
	//  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale0By0To100By100(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0308-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale0By0To100By100(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0309-HP")) );

	//[ Create a 100 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
	//  with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By0To100By100(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0310-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScale100By0To100By100(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0311-HP")) );

    //[ Tests heap allocation failures, during scaling a bitmap using UseMediumQuality 
    //  and UseMinimumQuality Algorithms. ]
	AddTestStepL( new(ELeave) CMmIclBitmapScaleSpeedAlloc(CBitmapScaler::EMediumQuality, _L("MM-ICL-BTRANS-I-0312-HP")) );
	AddTestStepL( new(ELeave) CMmIclBitmapScaleSpeedAlloc(CBitmapScaler::EMinimumQuality, _L("MM-ICL-BTRANS-I-0313-HP")) );
	
	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0301Cp(EFalse)	);

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0302Cp(EFalse) );
	AddTestStepL( new(ELeave)  CMmIclBtransI0302Cp(ETrue) );

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0303Hp(EFalse)	);

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0304Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0304Cp(ETrue)	);

	//[ addd Rotate test step to the test suite ]
	AddTestStepL( new(ELeave)  CMmIclBtransI0305Cp(EFalse)	);
	AddTestStepL( new(ELeave)  CMmIclBtransI0305Cp(ETrue)	);

	}



/**
*
* InitSystemPath
* This sets up the default path
* 
*/
void CTestSuiteBitmapTransforms::InitSystemPath()
	{

	_LIT(KVideoTestSourcePathOnE,"e:\\TImage\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\TImage\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\TImage\\");
	_LIT(KVideoTestSourcePathOnZ,"z:\\TImage\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnD;
		}
	else if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnE;
		}
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnZ;
		}
	else
		{
		User::Panic(_L("Test files not found"),KErrNotFound);
		}
		
	fs.Close();

	}

const TDesC& CTestSuiteBitmapTransforms::DefaultPath() const
	{
	return iDefaultPath;
	}
