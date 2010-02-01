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
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

// Specific includes for this test suite
#include "TestStepBitmapTransforms.h"
#include "TestSuiteBitmapTransforms.h"

// Specific includes for these test steps
#include "TestStepRotate.h"






/**
Constructor for CMmIclBtransI0201Cp
Rotate a CFbsBitmap by 90 degrees, REPLACING the original bitmap 
with the result, and verify the result matches a reference bitmap

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0201Cp::CMmIclBtransI0201Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0201-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1201-CP");
	}

/**
Setup the test step
*/
void CMmIclBtransI0201Cp::Setup()
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate90 Test REPLACING original");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap through 90 Degrees");
	Log( KTestStepRotate );	
	}

/**
Rotate a bitmap through 90 degrees n x n where n is > 0
Rotate a CFbsBitmap by 90 degrees, REPLACING the original bitmap 
with the result, and verify the result matches a reference bitmap

@returns "TVerdict" the status of the test
@lib "TSI_ICL_BTRANS_01.lib"
*/
TVerdict CMmIclBtransI0201Cp::TestStepL( )
	{
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	TFileName sourceFileName(KImageTestSourceFileName);

	//false flag indicates that original bitmap should be overwritten with the answer
	TVerdict result = RotateAndMirrorABitmapL( sourceFileName, CBitmapRotator::ERotation90DegreesClockwise, false);
	
	return result;		
	}




	
/**
Constructor for CMmIclBtransI0202Cp
Rotate a CFbsBitmap by 180 degrees, REPLACING the original bitmap 
with the result, and verify the result matches a reference bitmap

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0202Cp::CMmIclBtransI0202Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0202-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1202-CP");
	}


/**
Setup the test step
*/
void CMmIclBtransI0202Cp::Setup()
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate180 Test REPLACING original");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap through 180 Degrees");
	Log( KTestStepRotate );
	}

/**
Rotate a bitmap through 180 degrees n x n where n is > 0
Rotate a CFbsBitmap by 180 degrees, REPLACING the original bitmap 
with the result, and verify the result matches a reference bitmap

@returns "TVerdict" the status of the test
@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0202Cp::TestStepL( )
	{	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	TFileName sourceFileName(KImageTestSourceFileName);

	//false flag indicates that original bitmap should be overwritten with the answer
	TVerdict result = RotateAndMirrorABitmapL( sourceFileName, CBitmapRotator::ERotation180DegreesClockwise, false );
	
	return result;		
	}






/**
Constructor for CMmIclBtransI0203Cp
Rotate a CFbsBitmap by 270 degrees, REPLACING the original bitmap 
with the result, and verify the result matches a reference bitmap

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0203Cp::CMmIclBtransI0203Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0203-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1203-CP");
	}


/**
Setup the test step
*/
void CMmIclBtransI0203Cp::Setup()
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate270 Test REPLACING original");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap through 270 Degrees");
	Log( KTestStepRotate );
	}

/**
Rotate a bitmap through 270 degrees n x n where n is > 0
Rotate a CFbsBitmap by 270 degrees, REPLACING the original bitmap 
with the result, and verify the result matches a reference bitmap

@returns "TVerdict" the status of the test
@lib "TSI_ICL_BTRANS_01.lib"
*/
TVerdict CMmIclBtransI0203Cp::TestStepL( )
	{	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	TFileName sourceFileName(KImageTestSourceFileName);

	//false flag indicates that original bitmap should be overwritten with the answer
	TVerdict result = RotateAndMirrorABitmapL( sourceFileName, CBitmapRotator::ERotation270DegreesClockwise, false );
	
	return result;		
	}





/**
Constructor for CMmIclBtransI0204Cp
Rotate a CFbsBitmap by 90 degrees, KEEPING the original bitmap 
with the result, and verify the result matches a reference bitmap.

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0204Cp::CMmIclBtransI0204Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0204-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1204-CP");
	}

/**
Setup the test step
*/
void CMmIclBtransI0204Cp::Setup()
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate90 Test KEEPING the original");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap through 90 Degrees");
	Log( KTestStepRotate );
	}

/**
Rotate a bitmap through 90 degrees n x n where n is > 0
Rotate a CFbsBitmap by 90 degrees, KEEPING the original bitmap 
with the result, and verify the result matches a reference bitmap

@returns "TVerdict" the status of the test
@lib "TSI_ICL_BTRANS_01.lib"
*/
TVerdict CMmIclBtransI0204Cp::TestStepL( )
	{	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	TFileName sourceFileName(KImageTestSourceFileName);

	TVerdict result = RotateAndMirrorABitmapL( sourceFileName, CBitmapRotator::ERotation90DegreesClockwise );
	
	return result;		
	}








/**
Constructor for CMmIclBtransI0301Cp
Attempt to rotate a CFbsBitmap which has not been initialised. 
Verify the correct error code is returned and the original bitmap not changed

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0301Cp::CMmIclBtransI0301Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// [This test does not support alloc testing]
	__ASSERT_ALWAYS(!iAllocTest,User::Panic(_L("CMmIclBtransI0301Cp"),KErrNotSupported));

	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-BTRANS-I-0301-CP");
	}

/**
Rotate a bitmap through 90 degrees n x n where n is > 0
Attempt to rotate a CFbsBitmap which has not been initialised. 
Verify the correct error code is returned and the original bitmap not changed.

@panic BitmapTransforms throws a 4 as rotation is by an unspecified angle.

@returns "TVerdict" the status of the test (should never happen).
@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0301Cp::TestStepL( )
	{
	TVerdict  result = EFail ;

	_LIT( KTestRotate, "Normal(Gewoon) Rotate90 Test replacing original");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating an uninitialized bitmap through 90 Degrees");
	Log( KTestStepRotate );
	
	//[ Create and push an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	// [create and push a src bimap ]
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	//[create and push a tgt bitmap ]
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);

	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);

	INFO_PRINTF1(_L("About to rotate"));

	//[ rotate the uninitialized bitmap through 90 degrees ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise );
	CActiveScheduler::Start();

	TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);

	INFO_PRINTF1(_L("Rotated unintialized"));

	//[ check the bitmap against the original]
	result = CheckBitmaps(*srcBmp,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise );	
	INFO_PRINTF1(_L("Checked Rotated unintialized"));

	
	//[ Pop and Destroy activeListener, srcBmp, tgtBmp, rotator]
	CleanupStack::PopAndDestroy(4);  
	
	return result;
	
	}











/**
Constructor for CMmIclBtransI0302Cp
Attempt to rotate a CFbsBitmap which contains corrupt data or data which is not 
in CFbsBitmap format. Verify the correct error code is returned and the original 
bitmap not changed.

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0302Cp::CMmIclBtransI0302Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0302-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1302-CP");
	}


/**
Setup the test step
*/
void CMmIclBtransI0302Cp::Setup()
	{
	_LIT( KTestStepRotate, "Normal(Gewoon) rotate of a corrupt CFbsBitmap replacing original");
	Log( KTestStepRotate );
	}

/**
Attempt to rotate a CFbsBitmap which contains corrupt data or data which is not 
in CFbsBitmap format. Verify the correct error code is returned and the original 
bitmap not changed

@returns "TVerdict" the status of the test
@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0302Cp::TestStepL( )
	{
	TVerdict  result = EFail ;	
	
	//[ Create and push an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	// [create and push a src bimap ]
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	// [create and push a src bimap ]
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(refBmp);

	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	TFileName sourceFileName(KImageTestSourceFileName);
	SetTestFileNameL( sourceFileName );

	User::LeaveIfError(srcBmp->Load( FileName() ));
	User::LeaveIfError(refBmp->Load( FileName() ));

	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);

	const TSize srcBmpSize = srcBmp->SizeInPixels();

	//these lines trash the data, rotation succeeds but comparison fails
	//TUint32* srcBmpDataPtr = srcBmp->DataAddress();
	//TUint32* refBmpDataPtr = refBmp->DataAddress();
	//for(int cnt=0; cnt<srcBmpSize.iHeight;cnt++, *srcBmpDataPtr++=KMaxTUint32, *refBmpDataPtr++=KMaxTUint32);

	//these lines resize the bitmap - all is OK if this is done
	TSize newSize(srcBmpSize.iHeight+20,srcBmpSize.iHeight+20);
	if(srcBmp->Resize(newSize) != KErrNone)
		return EFail;
	if(refBmp->Resize(newSize) != KErrNone)
		return EFail;

	//rotate the bitmap 
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*srcBmp, CBitmapRotator::ERotation90DegreesClockwise );
	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);
	
	// check the bitmap against the original to ensure it hasn't been altered
	result = CheckBitmaps(*refBmp,*srcBmp, CBitmapRotator::ERotation90DegreesClockwise);
	
	//[ Pop and Destroy activeListener, srcBmp, refBmp, rotator]
	CleanupStack::PopAndDestroy(4);  
	
	return result;
	}

/**
Constructor for CMmIclBtransI0303Hp
Rotate a bitmap by an unsupported number of degrees
Attempt to rotate a CFbsBitmap by N degrees, replacing the original bitmap with the result, 
and verify the correct error code is returned and the original bitmap not changed.

@lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0303Hp::CMmIclBtransI0303Hp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// [This test does not support alloc testing]
	__ASSERT_ALWAYS(!iAllocTest,User::Panic(_L("CMmIclBtransI0303Hp"),KErrNotSupported));

	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-BTRANS-I-0303-HP");
	}



/**
Rotate a bitmap by an unsupported number of degrees
Attempt to rotate a CFbsBitmap by N degrees, replacing the original bitmap with the result, 
and verify the correct error code is returned and the original bitmap not changed

@panic BitmapTransforms throws a 4 as rotation is by an unspecified angle.

@returns "TVerdict" the status of the test  (should never happen).
@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0303Hp::TestStepL( )
	{
	TVerdict  result = EFail ;

	_LIT( KTestStepRotate, "Normal(Gewoon) rotate of an unsupported number of degrees (99) replacing original");
	Log( KTestStepRotate );
	
	
	//[ Create and push an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	// [create and push a src bimap ]
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	// [create and push a src bimap ]
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(refBmp);

	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	TFileName sourceFileName(KImageTestSourceFileName);
	SetTestFileNameL( sourceFileName );

	User::LeaveIfError(srcBmp->Load( FileName() ));
	User::LeaveIfError(refBmp->Load( FileName() ));


	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);



	//[ rotate the bitmap through 99 degrees ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*srcBmp, (CBitmapRotator::TRotationAngle)99 );
	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);

	INFO_PRINTF2(_L("Rotated by 99 degrees iStatus=%d"), activeListener->iStatus.Int());
	
	if(activeListener->iStatus != KErrArgument)
		result = EFail;
	else
		// check the bitmap against the original to ensure it hasn't been altered
		result = CheckBitmaps(*refBmp,*srcBmp, -1); 

	
	//[ Pop and Destroy activeListener, srcBmp, refBmp, rotator]
	CleanupStack::PopAndDestroy(4);  
	
	return result;
	}





/**
Constructor for CMmIclBtransI0305Cp
Attempt to rotate a CFbsBitmap which contains nothing. Verify the 
correct error code is returned and the original bitmap not changed

  @lib "TSI_ICL_BTRANS_01.lib"
*/

CMmIclBtransI0305Cp::CMmIclBtransI0305Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0305-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1305-CP");
	}


/**
Setup the test step
*/
void CMmIclBtransI0305Cp::Setup()
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate90 Test replacing original");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a zero sized bitmap through 90 Degrees");
	Log( KTestStepRotate );
	}

/**
Attempt to rotate a CFbsBitmap which contains nothing. Verify the 
correct error code is returned and the original bitmap not changed

@returns "TVerdict" the status of the test
@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0305Cp::TestStepL( )
	{
	TVerdict  result = EFail ;
	
	//[ Create and push an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	// [create and push a src bimap ]
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	//[create and push a tgt bitmap ]
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);

	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);

	User::LeaveIfError(srcBmp->Create(TSize(0,0),EColor16M));
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));

	//[ rotate the empty bitmap through 90 degrees ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise );
	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);

	//[ check the bitmap against the original]
	result = CheckBitmaps(*srcBmp,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise );	
	
	//[ Pop and Destroy activeListener, srcBmp, tgtBmp, rotator]
	CleanupStack::PopAndDestroy(4);  
	
	return result;
	
	}














