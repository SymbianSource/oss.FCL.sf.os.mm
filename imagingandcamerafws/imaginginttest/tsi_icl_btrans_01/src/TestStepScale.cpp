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

// Specific includes for this test suite
#include "TestStepBitmapTransforms.h"
#include "TestSuiteBitmapTransforms.h"

// Specific includes for these test steps
#include "TestStepScale.h"




/**
*
* Test step constructor.
* Each test step initialises its own name.
* 
*/

CMmIclBtransI0205Cp::CMmIclBtransI0205Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest) 
	{
	// store the name of this test case
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0205-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1205-CP");
	}

/**
Setup the test step
*/
void CMmIclBtransI0205Cp::Setup()
	{
	_LIT( KTestName,"CMmIclBtransI0205Cp");
	_LIT( KTestMessage, "this test, scales a CFbsBitmap by factor 1.5, replacing the original bitmap with the result" );
	Log( KTestName );
	Log( KTestMessage );
	}

/**
Do the test step.

Scale a CFbsBitmap by factor 1.5, replacing the original bitmap with the result, 
and verify the result matches a reference bitmap
  
@return "TVerdict"

@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0205Cp::TestStepL( )
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	//Create a src bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );

	const TSize srcSize( 10, 10 );
	User::LeaveIfError(srcBmp->Create(srcSize,EColor16M));

	//[ set the pixles of srcBmp to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );

	//Create a src bitmap this is used to compare the scaled bitmap against
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	const TSize tgtSize( 15, 15 );
	User::LeaveIfError(tgtBmp->Create(tgtSize,EColor16M));

	//[ set the pixles of tgtBmp to gray]
	FillBitmapL( *tgtBmp, grey );

	//to "prove" this approach works, set tgtBmp to cyan and it fails comparison
	//TRgb cyan = KRgbCyan;
	//FillBitmapL( *tgtBmp, cyan );

	   
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
		
	activeListener->InitialiseActiveListener();
	// [ scale the bitmap ]
	scaler->Scale(&activeListener->iStatus,*srcBmp,tgtSize,EFalse);
	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);

		//[ check the bitmap against the original]
	iTestStepResult = CheckBitmaps(*srcBmp,*tgtBmp, -1 );	


	//[ scaler, srcBmp, tgtBmp,  activeListener ]
	CleanupStack::PopAndDestroy(4);
	
		
	return iTestStepResult;
	}




/**
*
* Test step constructor.
* Each test step initialises its own name.
* 
*/

CMmIclBtransI0206Cp::CMmIclBtransI0206Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0206-CP");

	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1206-CP");
	}

/**
Setup the test step
*/
void CMmIclBtransI0206Cp::Setup()
	{
	_LIT( KTestName,"CMmIclBtransI0206Cp");
	_LIT( KTestMessage, "this test, scales a CFbsBitmap by factor 0.55, replacing the original bitmap with the result" );
	Log( KTestName );
	Log( KTestMessage );
	}

/**
Do the test step.

Scale a CFbsBitmap by factor 0.55, replacing the original bitmap with the result, 
and verify the result matches a reference bitmap
  
@return "TVerdict"

@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0206Cp::TestStepL( )
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	//Create a src bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );

	const TSize srcSize( 100, 100 );
	User::LeaveIfError(srcBmp->Create(srcSize,EColor16M));

	//[ set the pixles of srcBmp to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );

	//Create a src bitmap this is used to compare the scaled bitmap against
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	const TSize tgtSize( 55, 55 );
	User::LeaveIfError(tgtBmp->Create(tgtSize,EColor16M));

	//[set the pixles of tgtBmp to gray]
	FillBitmapL( *tgtBmp, grey );
   
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
		
	activeListener->InitialiseActiveListener();
	// [ scale the bitmap ]
	scaler->Scale(&activeListener->iStatus,*srcBmp,tgtSize,EFalse);
	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);

	//[ check the bitmap against the original]
	iTestStepResult = CheckBitmaps(*srcBmp,*tgtBmp, -1 );	


	//[ scaler, srcBmp, tgtBmp,  activeListener ]
	CleanupStack::PopAndDestroy(4);
	
		
	return iTestStepResult;
	}





/**
*
* Test step constructor.
* Each test step initialises its own name.
* 
*/

CMmIclBtransI0304Cp::CMmIclBtransI0304Cp(TBool aAllocTest)
:CTestStepBitmapTransforms(aAllocTest)
	{
	// store the name of this test case
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-BTRANS-I-0304-CP");
	else
		iTestStepName = _L("MM-ICL-BTRANS-I-1304-CP");
	}
/**
Setup the test step
*/
void CMmIclBtransI0304Cp::Setup()
	{
	_LIT( KTestMessage, "this test, scales a CFbsBitmap into a -1 by -1 TSize, replacing the original bitmap with the result" );
	Log( KTestMessage );
	}

/**
Do the test step.

Attempt to scale a CFbsBitmap into a -1 by -1 TSize, replacing the original bitmap 
with the result, and verify the correct error code is returned and the original 
bitmap not changed.

This scale is done into a 0x0 size

@return "TVerdict"

@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBtransI0304Cp::TestStepL( )
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	//Create a src bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );

	//Create a ref bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	const TSize srcSize( 100, 100 );
	User::LeaveIfError(srcBmp->Create(srcSize,EColor16M));
	User::LeaveIfError(refBmp->Create(srcSize,EColor16M));

	//[ set the pixels of srcBmp to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	FillBitmapL( *refBmp, grey );

	const TSize tgtSize( -1, -1 );

	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
		
	activeListener->InitialiseActiveListener();
	// [ scale the bitmap ]

	scaler->Scale(&activeListener->iStatus,*srcBmp,tgtSize,EFalse);
	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrArgument)
		User::Leave(status);
	else
		//[ check the bitmap against the original]
		iTestStepResult= CheckBitmaps(*srcBmp,*refBmp, -1); 

	//[ scaler, srcBmp, refBmp, activeListener ]
	CleanupStack::PopAndDestroy(4);

	
	return iTestStepResult;
	}




/**
*
* Test step constructor.
* Each test step initialises its own name.
* 
*/

CMmIclBitmapScaleLowMem1::CMmIclBitmapScaleLowMem1(TBool aAllocTest, TDisplayMode aDisplayMode, const TDesC& aTestStepName )
:CTestStepBitmapTransforms(aAllocTest)	
	{
	iTestStepName = aTestStepName;
	iDisplayMode = aDisplayMode;
	}


/**
* Setup the test step
* Print the appropriate display mode to the test log
*/
void CMmIclBitmapScaleLowMem1::Setup()
	{
	_LIT( KTestMessage, "This test scales a bitmap with old and new scaling algorithms. " );
	Log( KTestMessage );
	
		switch(iDisplayMode)
		{
		case EGray2:
			{
			INFO_PRINTF1(_L("Display mode: EGray2"));
			break;
			}
		case EGray4:
			{
			INFO_PRINTF1(_L("Display mode: EGray4"));
			break;
			}
		case EGray16:
			{
			INFO_PRINTF1(_L("Display mode: EGray16"));
			break;
			}
		case EGray256:
			{
			INFO_PRINTF1(_L("Display mode: EGray256"));
			break;
			}
		case EColor16:
			{
			INFO_PRINTF1(_L("Display mode: EColor16"));
			break;
			}
		case EColor256:
			{
			INFO_PRINTF1(_L("Display mode: EColor256"));
			break;
			}
		case EColor64K:
			{
			INFO_PRINTF1(_L("Display mode: EColor64K"));
			break;
			}
		case EColor16M:
			{
			INFO_PRINTF1(_L("Display mode: EColor16M"));
			break;
			}
		case EColor4K:
			{
			INFO_PRINTF1(_L("Display mode: EColor4K"));
			break;
			}
		case EColor16MU:
			{
			INFO_PRINTF1(_L("Display mode: EColor16MU"));
			break;
			}
		default:
			{
			INFO_PRINTF1(_L("Unknown display mode"));
			break;
			}
		}
	}


/**
Do the test step.

@return "TVerdict"

@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CMmIclBitmapScaleLowMem1::TestStepL( )
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a src bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );

	//Create a ref bitmap
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	const TSize srcSize( 100, 100 );
	TInt err = srcBmp->Create(srcSize,iDisplayMode);
	if(err != KErrNone)
		{
		INFO_PRINTF1(_L("Source bitmap could not be created."));
		return EFail; 
		}

	const TSize tgtSize( 50, 50 );
	err = tgtBmp->Create(tgtSize,iDisplayMode);
	if(err != KErrNone)
		{
		INFO_PRINTF1(_L("Target bitmap could not be created."));
		return EFail; 
		}

	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
		
	activeListener->InitialiseActiveListener();

	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp,EFalse);
	CActiveScheduler::Start();

	TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
		
	INFO_PRINTF1(_L("Bitmap is scaled successfully with old algorithm"));

	
	// enable low memory usage algorithm selection
	TBool lowMem = ETrue;
	TAny* ptr = &lowMem;
	

	TInt errCode = UseLowMemoryScaling(scaler, ptr, lowMem);
	
	if( errCode != KErrNone )
		{
		INFO_PRINTF2(_L("CustomCommand returned with error code =  %d"), errCode);
		iTestStepResult = EFail;
		}

	
	TSize tgtSize1( 150, 150 );
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *srcBmp, tgtSize1, EFalse);	
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if(status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		return EFail;
		}
	
	if(iTestStepResult == EFail)
		{
		INFO_PRINTF1(_L("Bitmap is scaled successfully with new algorithm"));	
		return iTestStepResult;
		}
	

	CleanupStack::PopAndDestroy(4, activeListener);
	
		
	return iTestStepResult;
	}



/*
*
*
* Scaling a 1152x864 size bitmap of type EColor16MU into 640x480 bitmap 
* with UseMediumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleSpeed1::CMmIclBitmapScaleSpeed1(const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	}

/*
*
* Setup the test step
*
* Print the appropriate display mode to the test log
* 
*/
void CMmIclBitmapScaleSpeed1::Setup()
	{
	_LIT( KTestMessage, "Scaling a 1152x864 size bitmap of type EColor16MU into 640x480 bitmap with UseMediumQuality Algorithm enabled." );
	Log( KTestMessage );	
	}

/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleSpeed1::TestStepL()
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );

	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );
	
	//Create a target bitmap
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );
	
	//Create a EColor16MU source bitmap of size 1152 x 864
	User::LeaveIfError(srcBmp->Create(TSize(1152, 864), EColor16MU));
	
	//Create a EColor16MU reference bitmap of size 640 x 480
	User::LeaveIfError(refBmp->Create(TSize(640, 480), EColor16M));
	
	//Create a EColor16MU target bitmap of size 640 x 480
	User::LeaveIfError(tgtBmp->Create(TSize(640, 480), EColor16MU));
	
	//Set the pixles of source, reference and target bitmap to gray
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	FillBitmapL( *refBmp, grey );
	FillBitmapL( *tgtBmp, grey );
	
	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Medium Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);
	
	activeListener->InitialiseActiveListener();
	
	//Scale the bitmap to the target bitmap of size 640x480 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	TTime startTime;
	startTime.HomeTime();	
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	
	TTime endTime;
	endTime.HomeTime();
	
	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}

	TInt64 expectedDuration = 7200000;
   	TInt64 actualDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF3(_L("Start time = %d End time = %d"), I64INT(startTime.Int64()), 
		I64INT(endTime.Int64()));
	  	
	INFO_PRINTF2(_L("ExpectedDuration = %d"), expectedDuration);
	INFO_PRINTF2(_L("ActualDuration = %d"), actualDuration);
	  	
	if (actualDuration <= expectedDuration)
		{
		INFO_PRINTF2(_L("Bitmap scaled in %d microsec."), actualDuration);
		}
	else
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled within 7.2 sec. Actual Duration is %d microsec."), actualDuration);
		iTestStepResult = EFail;
		}
	
	activeListener->InitialiseActiveListener();
	
	//Scaling the source bitmap to the target size of 640 x 480 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, TSize(640, 480), EFalse);
	
	startTime.HomeTime();
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
		
	endTime.HomeTime();
	
	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*srcBmp, *refBmp, -1);
		}

   	actualDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF3(_L("Start time = %d End time = %d"), I64INT(startTime.Int64()), 
		I64INT(endTime.Int64()));
	
	INFO_PRINTF2(_L("ExpectedDuration = %d"), expectedDuration);
	INFO_PRINTF2(_L("ActualDuration = %d"), actualDuration);
	  	
	if (actualDuration <= expectedDuration)
		{
		INFO_PRINTF2(_L("Bitmap scaled in %d microsec."), actualDuration);
		}
	else
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled within 7.2 sec. Actual Duration is %d microsec."), actualDuration);
		iTestStepResult = EFail;
		}
	
	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}
	
	

/*
*
* Scaling a 576x432 size bitmap of type EColor16MU into 640x480 bitmap 
* with UseMediumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleSpeed2::CMmIclBitmapScaleSpeed2(const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	}

/*
*
* Setup the test step
*
* Print the appropriate display mode to the test log
* 
*/
void CMmIclBitmapScaleSpeed2::Setup()
	{
	_LIT( KTestMessage, "Scaling a 576x432 size bitmap of type EColor16MU into 640x480 bitmap with UseMediumQuality Algorithm enabled." );
	Log( KTestMessage );	
	}

/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleSpeed2::TestStepL()
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );
	
	//Create a target bitmap
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a EColor16MU source bitmap of size 576 x 432
	User::LeaveIfError(srcBmp->Create(TSize(576, 432), EColor16MU));
	
	//Create a EColor16MU reference bitmap of size 640 x 480
	User::LeaveIfError(refBmp->Create(TSize(640, 480), EColor16M));
	
	//Create a EColor16MU target bitmap of size 640 x 480
	User::LeaveIfError(tgtBmp->Create(TSize(640, 480), EColor16MU));
	
	//Set the pixles of source, reference and target bitmap to gray
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	FillBitmapL( *refBmp, grey );
	FillBitmapL( *tgtBmp, grey );
	
	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Medium Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);
	
	activeListener->InitialiseActiveListener();
	
	//Scale the bitmap to the target bitmap of size 640x480 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	TTime startTime;
	startTime.HomeTime();	
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	
	TTime endTime;
	endTime.HomeTime();
	
	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}

	TInt64 expectedDuration = 3300000;
   	TInt64 actualDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF3(_L("Start time = %d End time = %d"), I64INT(startTime.Int64()), 
		I64INT(endTime.Int64()));
	
	INFO_PRINTF2(_L("ExpectedDuration = %d"), expectedDuration);
	INFO_PRINTF2(_L("ActualDuration = %d"), actualDuration);
	  	
	if (actualDuration <= expectedDuration)
		{
		INFO_PRINTF2(_L("Bitmap scaled in %d microsec."), actualDuration);
		}
	else
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled within 3.3 sec. Actual Duration is %d microsec."), actualDuration);
		iTestStepResult = EFail;
		}
	
	activeListener->InitialiseActiveListener();

	//Scaling the source bitmap to the target size 640 x 480 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, TSize(640, 480), EFalse);
	
	startTime.HomeTime();
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}

	endTime.HomeTime();

	if (status != KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*srcBmp, *refBmp, -1);
		}
		
   	actualDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF3(_L("Start time = %d End time = %d"), I64INT(startTime.Int64()), 
		I64INT(endTime.Int64()));
	
	INFO_PRINTF2(_L("ExpectedDuration = %d"), expectedDuration);
	INFO_PRINTF2(_L("ActualDuration = %d"), actualDuration);
	  	
	if (actualDuration <= expectedDuration)
		{
		INFO_PRINTF2(_L("Bitmap scaled in %d microsec."), actualDuration);
		}
	else
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled within 3.3 sec. Actual Duration is %d microsec."), actualDuration);
		iTestStepResult = EFail;
		}
	
	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}
	


/*
*
* This test checks the boundary condition of scaling a 100 x 1 bitmap 
* to another bitmap of size 100 x 100, with UseMediumQuality Algorithm 
* and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale100By1To100By100::CMmIclBitmapScale100By1To100By100(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale100By1To100By100::Setup()
	{
	_LIT( KTestMessage, "This test checks the boundary condition of scaling a 100 x 1 bitmap to another bitmap of size 100 x 100, with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale100By1To100By100::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 100 x 1 and target bitmap of size 100 x 100
	User::LeaveIfError(srcBmp->Create(TSize(100, 1), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(100, 100), EColor16MU));
	
	//Create a reference bitmap of size 100 x 100
	User::LeaveIfError(refBmp->Create(TSize(100, 100), EColor16M));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}



/*
*
* This test checks the boundary condition of scaling a 1 x 100 bitmap to 
* another bitmap of size 100 x 100, with UseMediumQuality Algorithm and 
* UseMinimumQuality Algorithm enabled.
*    
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale1By100To100By100::CMmIclBitmapScale1By100To100By100(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale1By100To100By100::Setup()
	{
	_LIT( KTestMessage, "This test checks the boundary condition of scaling a 1 x 100 bitmap to another bitmap of size 100 x 100, with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale1By100To100By100::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 1 x 100 and a target bitmap of size 100 x 100
	User::LeaveIfError(srcBmp->Create(TSize(1, 100), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(100, 100), EColor16MU));
	
	//Create a reference bitmap of size 100 x 100
	User::LeaveIfError(refBmp->Create(TSize(100, 100), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Scale a bitmap up by 2 and down by 2. It compares the size of the resultant bitmap 
* to the original, with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleUpAndDown::CMmIclBitmapScaleUpAndDown(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScaleUpAndDown::Setup()
	{
	_LIT( KTestMessage, "This test step scales a bitmap up by 2 and down by 2. It compares the size of the resultant bitmap to the original, with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}

/*
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleUpAndDown::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp1 = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp1 );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create source bitmap of size, say, 200 x 200 and target bitmap will be of size 400 x 400
	User::LeaveIfError(srcBmp->Create(TSize(200, 200), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(400, 400), EColor16MU));
	
	//Create reference bitmap of size 400 x 400 and 200 x 200
	User::LeaveIfError(refBmp->Create(TSize(400, 400), EColor16MU));
	User::LeaveIfError(refBmp1->Create(TSize(200, 200), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);
	
	//Set the pixles of source, target and reference bitmap to gray
	TRgb grey = KRgbGray;
	FillBitmapL(*srcBmp, grey);
	FillBitmapL(*refBmp, grey);
	FillBitmapL(*refBmp1, grey);
	FillBitmapL(*tgtBmp, grey);

	activeListener->InitialiseActiveListener();
	
	//Scale up by 2 i.e., to size 400x400
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

	activeListener->InitialiseActiveListener();
	
	//Scale down by 2 i.e., to size 200 x 200
	scaler->Scale(&activeListener->iStatus, *tgtBmp, TSize(200, 200), EFalse);
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check source bitmap and target bitmap
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp1, -1);	
		}

	//scaler, tgtBmp, refBmp1, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(6, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Scale a bitmap up by 2 to the target bitmap with UseMaximumQuality Algorithm enabled.
* Then enable UseMediumQuality Algorithm and scale the image by 2 to itself. Check the time.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleMaxMedSpeed::CMmIclBitmapScaleMaxMedSpeed(const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	}

/*
*
* Setup the test step
*
* Print the appropriate display mode to the test log
* 
*/
void CMmIclBitmapScaleMaxMedSpeed::Setup()
	{
	_LIT( KTestMessage, "Scale a bitmap up by 2 to the target bitmap with UseMaximumQuality Algorithm enabled. Then Enable UseMediumQuality Algorithm and scale the image by 2 to itself. Check the time." );
	Log( KTestMessage );
	}

/*
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleMaxMedSpeed::TestStepL()
	{	
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create source bitmap of size, say, 576 x 432
	User::LeaveIfError(srcBmp->Create(TSize(576, 432), EColor16MU));
	
	//Create target bitmap twice the size of source bitmap i.e., 1152 x 864
	User::LeaveIfError(tgtBmp->Create(TSize(1152, 864), EColor16MU));
	
	//Create a reference bitmap of size 1152 x 864
	User::LeaveIfError(refBmp->Create(TSize(1152, 864), EColor16MU));
		
	//Set the pixles of source, reference and target bitmap to gray
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	FillBitmapL( *refBmp, grey );
	FillBitmapL( *tgtBmp, grey );

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Maximum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMaximumQuality);
	
	activeListener->InitialiseActiveListener();

	//Scale the bitmap to target bitmap of size 1152x864 and check the time taken to scale	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	TTime startTime;
	startTime.HomeTime();	
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	
	TTime endTime;
	endTime.HomeTime();

	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

   	TInt64 maxQualityDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF4(_L("Start = %d End = %d Time to scale using Maximum Quality Algorithm = %d"),
	  	I64INT(startTime.Int64()), I64INT(endTime.Int64()), maxQualityDuration);	
	
	//Enable Medium Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);

	activeListener->InitialiseActiveListener();
	
	//Scale using UseMediumQuality Algorithm to the target size 1152 x 864 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, TSize(1152, 864), EFalse);
	
	startTime.HomeTime();
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
		
	endTime.HomeTime();
	
	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*srcBmp, *refBmp, -1);
		}

   	TInt64 medQualityDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF4(_L("Start = %d End = %d Time to scale using Medium Quality Algorithm = %d"),
	  	I64INT(startTime.Int64()), I64INT(endTime.Int64()), medQualityDuration);
	  	
	if (medQualityDuration <= maxQualityDuration)
		{
		Log(_L("Time taken using Medium Quality Algorithm is less than Maximum Quality Algorithm."));
		}
	else
		{
		Log(_L("Time taken using Medium Quality Algorithm is NOT less than Maximum Quality Algorithm."));
		iTestStepResult = EFail;
		}
	
	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}
	
	
	
/*
*
* Scale a bitmap up by 2 to the target bitmap with UseMediumQuality Algorithm enabled. 
* Then enable UseMinimumQualityAlgorithm and scale the image by 2 to itself. Check the time.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleMedMinSpeed::CMmIclBitmapScaleMedMinSpeed(const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	}

/*
*
* Setup the test step
*
* Print the appropriate display mode to the test log
* 
*/
void CMmIclBitmapScaleMedMinSpeed::Setup()
	{
	_LIT( KTestMessage, "Scale a bitmap up by 2 to the target bitmap with UseMediumQuality Algorithm enabled. Then Enable UseMinimumQuality Algorithm and scale the image by 2 to itself. Check the time." );
	Log( KTestMessage );
	}

/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleMedMinSpeed::TestStepL()
	{
	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create source bitmap of size, say, 576 x 432
	User::LeaveIfError(srcBmp->Create(TSize(576, 432), EColor16MU));
	
	//Create target bitmap twice the size of source bitmap i.e., 1152 x 864
	User::LeaveIfError(tgtBmp->Create(TSize(1152, 864), EColor16MU));
	
	//Create a reference bitmap of size 1152 x 864
	User::LeaveIfError(refBmp->Create(TSize(1152, 864), EColor16MU));
		
	//Set the pixles of source, reference and target bitmap to gray
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	FillBitmapL( *refBmp, grey );
	FillBitmapL( *tgtBmp, grey );

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	//Enable Medium Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);
		
	activeListener->InitialiseActiveListener();

	//Scale the bitmap to target bitmap of size 1152x864 and check the time taken to scale	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	TTime startTime;
	startTime.HomeTime();	
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	
	TTime endTime;
	endTime.HomeTime();

	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}
		
   	TInt64 medQualityDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF4(_L("Start = %d End = %d Time to scale using Medium Quality Algorithm = %d"),
	  	I64INT(startTime.Int64()), I64INT(endTime.Int64()), medQualityDuration);	
	
	//Enable Minimum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);

	activeListener->InitialiseActiveListener();

	//Scale the bitmap to the target size 1152 x 864 (itself) and check the time taken to scale	
	scaler->Scale(&activeListener->iStatus, *srcBmp, TSize(1152, 864), EFalse);
	
	startTime.HomeTime();
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
		
	endTime.HomeTime();

	if (status == KErrNone)
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*srcBmp, *refBmp, -1);
		}
		
   	TInt64 minQualityDuration = I64INT(endTime.MicroSecondsFrom(startTime).Int64());

	INFO_PRINTF4(_L("Start = %d End = %d Time to scale using Minimum Quality Algorithm = %d"),
	  	I64INT(startTime.Int64()), I64INT(endTime.Int64()), minQualityDuration);
	  	
	if (minQualityDuration < medQualityDuration)
		{
		Log(_L("Time taken using Minimum Quality Algorithm is less than Medium Quality Algorithm."));
		}
	else
		{
		Log(_L("Time taken using Minimum Quality Algorithm is NOT less than Medium Quality Algorithm."));
		iTestStepResult = EFail;
		}
	
	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}
	
	

/*
*
* Scale an EColor16MU Bitmap by simultaneous stretching and squashing on both the X and Y axes, 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*    
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleStretchAndSquash::CMmIclBitmapScaleStretchAndSquash(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScaleStretchAndSquash::Setup()
	{
	_LIT( KTestMessage, "Scale an EColor16MU Bitmap by simulaneous stretching and squashing on both the X and Y axes, with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleStretchAndSquash::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );
	
	//Create another reference bitmap
	CFbsBitmap* refBmp1 = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp1 );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create another target bitmap
	CFbsBitmap* tgtBmp1 = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp1 );
	
	//Create a source EColor16MU bitmap of size 100 x 100
	User::LeaveIfError(srcBmp->Create(TSize(100, 100), EColor16MU));
	
	//Create a target EColor16MU bitmap of size 50 x 200
	User::LeaveIfError(tgtBmp->Create(TSize(50, 200), EColor16MU));
	User::LeaveIfError(refBmp->Create(TSize(50, 200), EColor16MU));
	
	//Create another target EColor16MU bitmap of size 200 x 50
	User::LeaveIfError(tgtBmp1->Create(TSize(200, 50), EColor16MU));
	User::LeaveIfError(refBmp1->Create(TSize(200, 50), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}

   	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp1, EFalse);
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		if (iTestStepResult != EFail)
			{
			iTestStepResult = EFail;
			}
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp1, *refBmp1, -1);
		}	

	//scaler, tgtBmp1, tgtBmp, refBmp1, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(7, activeListener);
			
	return iTestStepResult;
	}



/*
*
* This test checks the boundary condition of scaling a 100 x 100 bitmap to 100 x 1 bitmap 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale100By100To100By1::CMmIclBitmapScale100By100To100By1(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale100By100To100By1::Setup()
	{
	_LIT( KTestMessage, "This test checks the boundary condition of scaling a 100 x 100 bitmap to 100 x 1 bitmap with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale100By100To100By1::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 100 x 100 and target bitmap of size 100 x 1
	User::LeaveIfError(srcBmp->Create(TSize(100, 100), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(100, 1), EColor16MU));

	//Create a reference bitmap of size 100 x 1
	User::LeaveIfError(refBmp->Create(TSize(100, 1), EColor16MU));
	
	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}



/*
*
* This test checks the boundary condition of scaling a 100 x 100 bitmap to 1 x 100 bitmap 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale100By100To1By100::CMmIclBitmapScale100By100To1By100(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale100By100To1By100::Setup()
	{
	_LIT( KTestMessage, "This test checks the boundary condition of scaling a 100 x 100 bitmap to 1 x 100 bitmap with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale100By100To1By100::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 100 by 100 and a target bitmap of size 1 x 100
	User::LeaveIfError(srcBmp->Create(TSize(100, 100), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(1, 100), EColor16MU));
	
	//Create a reference bitmap of size 1 x 100
	User::LeaveIfError(refBmp->Create(TSize(1, 100), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 200 x 0 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*	
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale0By100To200By0::CMmIclBitmapScale0By100To200By0(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale0By100To200By0::Setup()
	{
	_LIT( KTestMessage, "Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 200 x 0 with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale0By100To200By0::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 0 by 100 and a target bitmap of size 200 x 0
	User::LeaveIfError(srcBmp->Create(TSize(0, 100), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(200, 0), EColor16MU));

	//Create a reference bitmap of size 200 x 0
	User::LeaveIfError(refBmp->Create(TSize(200, 0), EColor16MU));
	
	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		}	

	//scaler, tgtBmp, refBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(5, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale0By100To100By100::CMmIclBitmapScale0By100To100By100(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale0By100To100By100::Setup()
	{
	_LIT( KTestMessage, "Create a 0 x 100 bitmap and attempt to scale it to a target bitmap of size 100 x 100 with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale0By100To100By100::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 0 x 100 and a target bitmap of size 100 x 100
	User::LeaveIfError(srcBmp->Create(TSize(0, 100), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(100, 100), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrArgument)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}

	//scaler, tgtBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(4, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Create a 0 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale0By0To100By100::CMmIclBitmapScale0By0To100By100(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale0By0To100By100::Setup()
	{
	_LIT( KTestMessage, "Create a 0 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale0By0To100By100::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 0 x 0 and a target bitmap of size 100 x 100
	User::LeaveIfError(srcBmp->Create(TSize(0, 0), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(100, 100), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrArgument)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}

	//scaler, tgtBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(4, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Create a 100 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 
* with UseMediumQuality Algorithm and UseMinimumQuality Algorithm enabled.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScale100By0To100By100::CMmIclBitmapScale100By0To100By100(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate quality alogorithm enabled to the test log.
* 
*/
void CMmIclBitmapScale100By0To100By100::Setup()
	{
	_LIT( KTestMessage, "Create a 100 x 0 bitmap and attempt to scale it to a target bitmap of size 100 x 100 with " );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}
	
/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScale100By0To100By100::TestStepL()
	{	
	//Create an Active Listener and push the listener on the stack
	CActiveListener* activeListener = new (ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a source bitmap
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a target bitmap
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );

	//Create a source bitmap of size 100 x 0 and a target bitmap of size 100 x 100
	User::LeaveIfError(srcBmp->Create(TSize(100, 0), EColor16MU));
	User::LeaveIfError(tgtBmp->Create(TSize(100, 100), EColor16MU));

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);

	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	if (status != KErrArgument)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}

	//scaler, tgtBmp, srcBmp, activeListener
	CleanupStack::PopAndDestroy(4, activeListener);
			
	return iTestStepResult;
	}



/*
*
* Tests heap allocation failures, during scaling a bitmap using UseMediumQuality 
* and UseMinimumQuality Algorithms.
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CBitmapScaler::TQualityAlgorithm aQualityAlgo
* @param const TDesC& aTestStepName
* 
*/
CMmIclBitmapScaleSpeedAlloc::CMmIclBitmapScaleSpeedAlloc(
	CBitmapScaler::TQualityAlgorithm aQualityAlgo, 
	const TDesC& aTestStepName)
	:CTestStepBitmapTransforms(EFalse)	
	{
	iTestStepName = aTestStepName;
	iQualityAlgo = aQualityAlgo;
	}

/*
*
* Setup the test step
*
* Print the appropriate display mode to the test log
* 
*/
void CMmIclBitmapScaleSpeedAlloc::Setup()
	{
	_LIT( KTestMessage, "This test step tests heap allocation failures, during scaling a bitmap, by checking the error returned after n heap allocations." );
	Log( KTestMessage );
	
	DisplayQualityAlgo(iQualityAlgo);
	}

/*
* 
* Scale the bitmap by marking heap
*
* @param CFbsBitmap* aSrcBmp
* @param CFbsBitmap* aTgtBmp
* 
*/
void CMmIclBitmapScaleSpeedAlloc::ScaleBitmapWithQualityAlgorithmL(
	CFbsBitmap* aSrcBmp, CFbsBitmap* aTgtBmp)
	{
	__MM_HEAP_MARK;

	//Create an active Listener and push the listener on the stack
	CActiveListener* activeListener = new(ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );

	//Create a bitmap scaler
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	//Enable Quality Algorithm
	scaler->SetQualityAlgorithm(iQualityAlgo);
		
	activeListener->InitialiseActiveListener();

	//Scale the bitmap to target bitmap of size 640x480	
	scaler->Scale(&activeListener->iStatus, *aSrcBmp, *aTgtBmp, EFalse);
	
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	User::LeaveIfError(status);
	
	//scaler, activeListener	
	CleanupStack::PopAndDestroy(2, activeListener);
	
	__MM_HEAP_MARKEND;
	}

/*
* 
* Do the test step.
* 
* @return "TVerdict"
* 
* @lib "TSI_ICL_BTRANS_01.lib"
* 
*/
TVerdict CMmIclBitmapScaleSpeedAlloc::TestStepL()
	{
	//Create a source bitmap
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	
	//Create a reference bitmap
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( refBmp );

	//Create a target bitmap
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );
	
	//Create a source and reference bitmap of size, say, 576 x 432
	User::LeaveIfError(srcBmp->Create(TSize(576, 432), EColor16MU));
	
	//Create a target bitmap of size, say, 640 x 480
	User::LeaveIfError(tgtBmp->Create(TSize(640, 480), EColor16MU));
	User::LeaveIfError(refBmp->Create(TSize(640, 480), EColor16MU));
		
	//Set the pixles of source, reference and target bitmap to gray
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	FillBitmapL( *refBmp, grey );
	FillBitmapL( *tgtBmp, grey );

	TInt err = KErrNone;
	TBool result = EFalse;
	
	TRAP(err, ScaleBitmapWithQualityAlgorithmL(srcBmp, tgtBmp));
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Scaling error, returned error code =  %d"), err);
		User::Leave(err);
		}
	else
		{
		//Check the bitmap against the original
		iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
		if (iTestStepResult != EPass)
			{
			result = ETrue;
			}
		}	
	
	TInt failCount = 1;
	TBool completed = EFalse;
	iTestStepResult = EPass; // TODO check?? assume pass
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for(;;)	
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TRAP(err, ScaleBitmapWithQualityAlgorithmL(srcBmp, tgtBmp));

		completed = EFalse;
		if (err == KErrNone)
			{
			TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
			if (testAlloc == NULL)
				{
				reachedEnd = ETrue;
				failCount -= 1;
				}
			else
				{
				User::Free(testAlloc);	
				}			
			
			//Check the bitmap against the original
			iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
			if (iTestStepResult != EPass)
				{
				result = ETrue;
				}
			
			completed = reachedEnd || result;
			}
		else if (err != KErrNoMemory) // bad error code
			{
			completed = ETrue;
			result = EFail;
			}			

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			{
			break; // exit loop
			}

		failCount++;
		}

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc

	if (err != KErrNone || result)
		{
		TBuf<80> format;
		if (result)
			{
			format.Format(_L("  Bad result with %d memory allocations tested\n"), failCount);
			}
		else
			{
			format.Format(_L("  Error(%d) with %d memory allocations tested\n"), err, failCount);
			}
		Log(format);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("  Completed OK with %d memory allocations tested\n"), failCount);
		Log(format);
		}

	//tgtBmp, refBmp, srcBmp
	CleanupStack::PopAndDestroy(3, srcBmp);
	
	return iTestStepResult;
	}
