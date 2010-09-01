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

// Specific includes for this test suite
#include "TestStepBitmapTransforms.h"
#include "TestSuiteBitmapTransforms.h"

// Specific includes for these test steps
#include "TestStepRotate.h"

#include "ICLFbsSessionTest.h"
#include <icl/imagecodecdata.h>

// --------------------------------------------

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TS_BitmapTransforms.lib" 
*
*/

CTestStepRotate::CTestStepRotate()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// old test name GT0137-18-U-003
	iTestStepName = _L("MM-ICL-BTRANS-U-003-HP");
	}

/**
*
* Test step destructor.
* 
*/
CTestStepRotate::~CTestStepRotate()
	{
	}

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
* This test does nothing more than exercise the rotate & scale code
* @return	"TVerdict" the status of the test
*			The result of the test step 
*/
TVerdict CTestStepRotate::DoTestStepL( )
	{
	
	TVerdict cmpResult = EFail;
	
	_LIT( KTestRotate, "Normal(Gewoon) Rotate Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap");
	Log( KTestStepRotate );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	SetTestFileNameL( KImageTestSourceFileName );
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Load( FileName() ));
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	activeListener->InitialiseActiveListener();
    rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise);
	CActiveScheduler::Start();
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp, CBitmapRotator::ERotation270DegreesClockwise);
	CActiveScheduler::Start();
	
	//[ check bitmaps are the same ]
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp,-1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
    rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp, CBitmapRotator::ERotation180DegreesClockwise);
	CActiveScheduler::Start();
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp, CBitmapRotator::ERotation180DegreesClockwise);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation180DegreesClockwise);
	CActiveScheduler::Start();
	
	//[ check the bitmaps are equivalent ]
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp,-1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
    rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::ERotation270DegreesClockwise);
	CActiveScheduler::Start();
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp, CBitmapRotator::ERotation270DegreesClockwise);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise);
	CActiveScheduler::Start();
	
	//[ check the bitmaps are equivalent ]
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp,-1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	
	// add image mirror test code
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::EMirrorHorizontalAxis );		                                                                                     	               
	CActiveScheduler::Start();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::EMirrorVerticalAxis);
	CActiveScheduler::Start();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation180DegreesClockwise);
	CActiveScheduler::Start();
	
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::EMirrorHorizontalAxis );		                                                                                     	               
	CActiveScheduler::Start();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::EMirrorVerticalAxis);
	CActiveScheduler::Start();
	
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation180DegreesClockwise);
	CActiveScheduler::Start();
	
	//[ check the bitmaps are equivalent ]
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp, -1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation270DegreesClockwise);
	CActiveScheduler::Start();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation90DegreesClockwise);
	CActiveScheduler::Start();
	
	//[ check the bitmaps are equivalent ]
	cmpResult = CheckBitmaps(*srcBmp,*tgtBmp,-1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::ERotation90DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate( &activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::ERotation180DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::ERotation270DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::EMirrorHorizontalAxis );
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::EMirrorVerticalAxis);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation90DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation180DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp,CBitmapRotator::ERotation270DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp,CBitmapRotator::EMirrorHorizontalAxis );
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp,CBitmapRotator::EMirrorVerticalAxis);
	rotator->Cancel();
	activeListener->Cancel();
	
	
	activeListener->InitialiseActiveListener();
    rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp,CBitmapRotator::ERotation270DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise);
	rotator->Cancel();
	activeListener->Cancel();
	
	
    //[ Create a bit map of zero size on both dimensions and rotate and Mirror it ]
	CFbsBitmap* zeroSrcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(zeroSrcBmp);
	User::LeaveIfError( zeroSrcBmp->Create(TSize(0,0),EColor16M));
	
	// [Rotate and Mirror ]
	RotateAndMirrorL( *rotator, *zeroSrcBmp, *tgtBmp, *activeListener );
	
	//[ Create a bit map of zero size along the x axis and rotate and Mirror it ]
	CFbsBitmap* xAxisSrcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( xAxisSrcBmp);
	User::LeaveIfError( xAxisSrcBmp->Create(TSize(0,100),EColor16M));
	   
	//[Rotate and mirror the bitmap ]
	RotateAndMirrorL( *rotator, *xAxisSrcBmp, *tgtBmp, *activeListener );
	   
	   
	//[ Create a bit map of zero size along y axis and rotate and Mirror ]
	CFbsBitmap* yAxisSrcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(yAxisSrcBmp);
	User::LeaveIfError( yAxisSrcBmp->Create(TSize(100,0),EColor16M));
	   
	//[Rotate and mirror the bitmap ]
	RotateAndMirrorL( *rotator, *yAxisSrcBmp, *tgtBmp, *activeListener );
	   
	//[ Create a 1 x 100 size pixel bitmap of colour 128,128,128
	//  rotate and mirror and compare with the original]
	const TInt xDimension = 1;
	const TInt yDimension = 100;
	CFbsBitmap* oneByN = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( oneByN );
	User::LeaveIfError( oneByN->Create(TSize( xDimension, yDimension ),EColor16M));
	   
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *oneByN, grey );
	   
	//[Rotate and Mirror ]
	RotateAndMirrorL( *rotator, *oneByN, *tgtBmp, *activeListener );
	   
	//[ check bitmaps are the same, ie transforms have had not net effect]
	cmpResult = CheckBitmaps( *oneByN, *tgtBmp, -1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	   
	   
	//[Create a Nx 1 size pixel bitmap, rotate and mirror it ]
	const TInt xxDimension = 100;
	const TInt yyDimension  = 1;
	CFbsBitmap* nByOne = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( nByOne );
	User::LeaveIfError( nByOne->Create(TSize( xxDimension, yyDimension ),EColor16M));
	   
	//[ Fill bit map with grey ]
	FillBitmapL( *oneByN, grey );
	   
	RotateAndMirrorL( *rotator, *nByOne, *tgtBmp, *activeListener );
	   
	//[ check bitmaps are the same, ie transforms have had not net effect]
	cmpResult = CheckBitmaps( *nByOne, *tgtBmp, -1);
	if( cmpResult == EFail )
		{
		return cmpResult ;
		}
	   
	   
	//[ pop activeListener
	//	srcBmp, tgtBmp,
	//      Rotator, zeroSrcBmp, 
	//      xAxisSrcBmp, yAxisSrcBmp, 
	//      oneByN, nByOne ]
	CleanupStack::PopAndDestroy(9);  
  
 
	// test steps return a result
	return iTestStepResult;
	 }

/**
This function rotates and mirrors aSrcBitmap
through all of the supplied operations resulting in a series of transformations
that should yield a tgtBmp that is equivalent to the original bitmap
@parama srcBmp a reference to the source bitmap 
@param  tgtBmp a reference to the target bitmap
@param  rotator a reference to CBitmapRotator
@param  aActiveListener 
@lib "TS_BitmapTransforms.lib"
*/

void  CTestStepRotate::RotateAndMirrorL( CBitmapRotator& rotator, CFbsBitmap& srcBmp,CFbsBitmap& tgtBmp, CActiveListener& aActiveListener  )
	{
	static const CBitmapRotator::TRotationAngle instructions[]=
		{
		CBitmapRotator::ERotation90DegreesClockwise,
		CBitmapRotator::ERotation180DegreesClockwise,
		CBitmapRotator::ERotation270DegreesClockwise,
		CBitmapRotator::EMirrorHorizontalAxis,
		CBitmapRotator::EMirrorVerticalAxis
		};
		
	//[ Rotate and mirror the bitmap and Compare src and destination]
	for (TInt singlePar=0; singlePar <2; singlePar++)
		{

		for (TInt i=0; i< sizeof(instructions)/sizeof(instructions[0]); i++ )
			{
			aActiveListener.InitialiseActiveListener();
			if (singlePar)
				{
				User::LeaveIfError( tgtBmp.Create(srcBmp.SizeInPixels(), srcBmp.DisplayMode()) );
				DuplicateContentL(srcBmp, tgtBmp);
				rotator.Rotate(&aActiveListener.iStatus, tgtBmp, instructions[i]);
				}
			else
				{
				rotator.Rotate(&aActiveListener.iStatus, srcBmp, tgtBmp, instructions[i]);
				}
	    	
			CActiveScheduler::Start();
			User::LeaveIfError( aActiveListener.Result() );
		
			if (CheckBitmaps( srcBmp, tgtBmp, instructions[i]) != EPass )
				{
				Log(_L("Btimap check failed, instruction %d"),instructions[i]);
				User::Leave( KErrGeneral );
				}
			}
		}
	}
	
CTestStepRotate16Ma::CTestStepRotate16Ma(const TDesC& aTestName, const TDesC& aRgbBmpName, const TDesC& aAlphaBmpName):
						iRgbBmpName(aRgbBmpName), iAlphaBmpName (aAlphaBmpName )
	{
	iTestStepName = aTestName;
	}
	
CTestStepRotate16Ma::~CTestStepRotate16Ma()
	{
	}
	
TVerdict CTestStepRotate16Ma::DoTestStepL()
	{
	_LIT( KTestStepRotate, "This test checks rotating/mirroring of a EColor16MA mode bitmap");
	Log( KTestStepRotate );	
	
	CFbsBitmap* bmp16MA= LoadAndCreate16MABitmapL(iRgbBmpName, iAlphaBmpName);
	CleanupStack::PushL( bmp16MA );
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);

    CFbsBitmap*	tgtBmp = new (ELeave) CFbsBitmap();
    CleanupStack::PushL( tgtBmp );
    User::LeaveIfError( tgtBmp->Create(TSize(1,1), EColor16MA) );

	RotateAndMirrorL( *rotator, *bmp16MA, *tgtBmp, *activeListener );
	
	CleanupStack::PopAndDestroy(4, bmp16MA);
	return EPass;
	}

/**
Constructor for CTestStepRotate90Degrees
Rotate a bitmap through 90 degrees n x n where n is > 0
@lib "TS_BitmapTransforms.lib"
*/

CTestStepRotate90Degrees::CTestStepRotate90Degrees()
:CTestStepBitmapTransforms()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// old test name "GT0137-18-U-001"
	iTestStepName = _L("MM-ICL-BTRANS-U-001-HP");
	}

/**
destructor for CTestStepRotate90Degrees
Rotate a bitmap through 90 degrees n x n where n is > 0
@lib "TS_BitmapTransforms.lib"
*/

CTestStepRotate90Degrees::~CTestStepRotate90Degrees()
	{
	}

/**
Rotate a bitmap through 90 degrees n x n where n is > 0
Read a bitmap from a source file and rotate it through 90 degrees
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/

enum TVerdict CTestStepRotate90Degrees::DoTestStepL( )
	{
    _LIT( KTestRotate, "Normal(Gewoon) Rotate90 Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap through 90 Degrees");
	Log( KTestStepRotate );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
    TFileName sourceFileName(KImageTestSourceFileName);

	//[ Mirror the bitmap about the horizontal axis ]
    TVerdict result = RotateAndMirrorABitmap( sourceFileName, CBitmapRotator::ERotation90DegreesClockwise );
	
	return result;		
	}


/**
constrcutor for 180 degrees rotation of an n by n bitmap
@lib "TS_BitmapTransforms.lib"
*/
CTestStepRotate180Degrees::CTestStepRotate180Degrees()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// "GT0137-18-U-002"
	iTestStepName = _L( "MM-ICL-BTRANS-U-002-HP");
	}


/**
destrcutor for 180 degrees rotation of an n by n bitmap
@lib "TS_BitmapTransforms.lib"
*/

CTestStepRotate180Degrees::~CTestStepRotate180Degrees()
	{
	}

/**
This method is called by the ets suite
and rotates a bitmap through 180 degrees
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/
enum TVerdict CTestStepRotate180Degrees::DoTestStepL( )
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate 180 Degree Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap 180 Degrees");
	Log( KTestStepRotate );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
	TFileName sourceFileName (KImageTestSourceFileName);
	//[ Mirror the bitmap about the horizontal axis ]
    TVerdict result = RotateAndMirrorABitmap( sourceFileName, CBitmapRotator::ERotation180DegreesClockwise );

	return result;
	}


/**
Constructor for teststep to rotate a bitmap through 270 degrees
@lib "TS_BitmapTransforms.lib"
*/

CTestStepRotate270Degrees::CTestStepRotate270Degrees()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	//  "GT0137-18-U-005"
	iTestStepName =  _L( "MM-ICL-BTRANS-U-005-HP");
	}

/**
destructor for teststep to rotate a n by n bitmap through 270 degrees
where n > 0
@lib "TS_BitmapTransforms.lib"
*/
CTestStepRotate270Degrees::~CTestStepRotate270Degrees()
	{
	
	}

/**
This method is called by the test suite to rotate a n by n bitmap 
through 270 degrees. The bitmap is read from file
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/
enum TVerdict CTestStepRotate270Degrees::DoTestStepL( )
	{
	_LIT( KTestRotate, "Normal(Gewoon) Rotate 270 Degrees Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks rotating a bitmap");
	Log( KTestStepRotate );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
	TFileName sourceFileName (KImageTestSourceFileName);
	//[ Mirror the bitmap about the horizontal axis ]
    TVerdict result = RotateAndMirrorABitmap( sourceFileName, CBitmapRotator::ERotation270DegreesClockwise );

	return result;	
	}


/*
Constructor for CTestStepMirrorVertical a teststep
to mirror a n by n bitmap about the vertical axis
@lib "TS_BitmapTransforms.lib"
*/
CTestStepMirrorVertical::CTestStepMirrorVertical()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	//"GT0137-18-U-007"
	iTestStepName = _L( "MM-ICL-BTRANS-U-007-HP") ;
	}

/*
destructor of CTestStepMirrorVertical
@lib "TS_BitmapTransforms.lib"
*/

CTestStepMirrorVertical::~CTestStepMirrorVertical()
	{
	// nothing doing	
	}

/**
This method is called by the test suite and mirrors a 
n by n bitmap about the vertical axis where n > 0
@returns "enum TVerdict" (EPass or EFail) the status of the test
@lib "TS_BitmapTransforms.lib"
*/

enum TVerdict CTestStepMirrorVertical::DoTestStepL( )
	{

	_LIT( KTestRotate, "Normal(Gewoon) Mirror Vertical Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks Mirroring a bitmap about vertical axis");
	Log( KTestStepRotate );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
	TFileName sourceFileName (KImageTestSourceFileName);
	//[ Mirror the bitmap about the horizontal axis ]
    TVerdict  result = RotateAndMirrorABitmap( sourceFileName, CBitmapRotator::EMirrorVerticalAxis );

	return result;
	}

/*
Constructor for CTestStepMirrorHorizontal a teststep
to mirror a n by n bitmap about the horizontal axis
@lib "TS_BitmapTransforms.lib"
*/

CTestStepMirrorHorizontal::CTestStepMirrorHorizontal()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	//"GT0137-18-U-006"
	iTestStepName = _L( "MM-ICL-BTRANS-U-006-HP");	
	}

/*
destructor for CTestStepMirrorHorizontal a teststep
to mirror a n by n bitmap about the horizontal axis
@lib "TS_BitmapTransforms.lib"
*/
CTestStepMirrorHorizontal::~CTestStepMirrorHorizontal()
	{
	
	}

/*
This function is called by the test suite
to mirror a n by n bitmap about the horizontal axis
@returns "enum TVerdict" (EPass or Efail ) status of the test
@lib "TS_BitmapTransforms.lib"
*/
enum TVerdict CTestStepMirrorHorizontal::DoTestStepL( )
	{
	TVerdict result = EFail;
	
    _LIT( KTestRotate, "Normal(Gewoon) Mirror Horizontal Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks Mirroring a bitmap about horizontal axis");
	Log( KTestStepRotate );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
	TFileName sourceFileName (KImageTestSourceFileName);

	//[ Mirror the bitmap about the horizontal axis ]
    result = RotateAndMirrorABitmap( sourceFileName, CBitmapRotator::EMirrorHorizontalAxis );

	return result;
	}

/**
constructor CTestStepZeroOnBothDimensions
@lib "TS_BitmapTransforms.lib"
*/

CTestStepZeroOnBothDimensions::CTestStepZeroOnBothDimensions()
	{
	// [ set up test name ]
	//"GT0137-18-U-013"
	iTestStepName = _L( "MM-ICL-BTRANS-U-013-HP" );
	}

/**
destructor ~CTestStepZeroOnBothDimensions
@lib "TS_BitmapTransforms.lib"
*/
CTestStepZeroOnBothDimensions::~CTestStepZeroOnBothDimensions()
	{
	
	}

/**
* Do the test step
* Each test step must supply an implementation for DoTestStepL.
*
* @return	"enum TVerdict"
*			The result of the test step
* @lib "TS_BitmapTransforms.lib"
*/

enum TVerdict CTestStepZeroOnBothDimensions::DoTestStepL( )
	{
	TVerdict result = EPass;
	
    _LIT( KTestRotate, "Normal(Gewoon) Rotate, Zero on Both Dimesions");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks attempted rotation of a zero sized bitmap");
	Log( KTestStepRotate );
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
    //[Create a rotator object ]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	//[ Create a bit map of zero size on both dimensions and rotate and Mirror it ]
	CFbsBitmap* zeroSrcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(zeroSrcBmp);
	User::LeaveIfError( zeroSrcBmp->Create(TSize(0,0),EColor16M));
	
    //[Rotate tgtBmp through 90 degrees clockwise ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus, *zeroSrcBmp, CBitmapRotator::ERotation90DegreesClockwise);
	CActiveScheduler::Start();
	
	//[ activeListener, rotator, zeroSrcBitmap ]
    CleanupStack::PopAndDestroy(3);  
	
	//[ if we wre here with out problems then pass]
	return result;
	}

/**
constructor CTestStepOneByY
@lib "TS_BitmapTransforms.lib"
*/

CTestStepOneByY::CTestStepOneByY()
	{
	// [ set up test name ]
	//"GT0137-18-U-014"
	iTestStepName = _L( "MM-ICL-BTRANS-U-014-HP" );
	}

/**
destructor CTestStepOneByY
@lib "TS_BitmapTransforms.lib"
*/
CTestStepOneByY::~CTestStepOneByY()
	{
	}


/**
Rotate a bitmap through 90 degrees 1 x n where n is > 0
Read a bitmap from a source file and rotate it through 90 degrees
and check it against the source. It is a boundary test
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/

enum TVerdict CTestStepOneByY::DoTestStepL( )
	{
	TVerdict result = EPass;
	
    _LIT( KTestRotate, "Normal(Gewoon) Rotate, One by N Bitmap");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks the rotation of a 1 by N bitmap");
	Log( KTestStepRotate );
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
    //[Create a rotator object ]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	
	//[ Create a 1 x 100 size pixel bitmap of colour 128,128,128
	//  rotate and mirror and compare with the original]
	const TInt xDimension = 1;
	const TInt yDimension = 100;
	CFbsBitmap* oneByN = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( oneByN );
	User::LeaveIfError( oneByN->Create(TSize( xDimension, yDimension ),EColor16M));
	   
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *oneByN, grey );
	   
	//[Rotate tgtBmp through 90 degrees clockwise ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus, *oneByN,*tgtBmp, CBitmapRotator::ERotation90DegreesClockwise);
	CActiveScheduler::Start();
	   
	//[ check bitmaps are the same, ie transforms have had not net effect]
	result = CheckBitmaps( *oneByN, *tgtBmp, CBitmapRotator::ERotation90DegreesClockwise );
	   
	//[ Pop activeListener, rotator, tgtBitmap, oneByN ]
	CleanupStack::PopAndDestroy(4); 
	
	return result;
	}

/**
Constructor CTestStepXbyOne:
*/
CTestStepXbyOne::CTestStepXbyOne()
	{
	// [ set up test name ]
	//"GT0137-18-U-015"
	_LIT( KTestName, "MM-ICL-BTRANS-U-015-HP" );
	iTestStepName = KTestName ;
	}

/**
Destructor CTestStepXbyOne
*/
CTestStepXbyOne::~CTestStepXbyOne()
	{
	}

/**
Rotate a bitmap through 90 degrees n by 1 where n is > 0
Read a bitmap from a source file and rotate it through 90 degrees
and check it against the source. It is a boundary test
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/	 
enum TVerdict CTestStepXbyOne::DoTestStepL( )
	{
	TVerdict result = EPass;
	
    _LIT( KTestRotate, "Normal(Gewoon) Rotate, One by N Bitmap");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks the rotation of a n by one bitmap");
	Log( KTestStepRotate );
	
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
    //[Create a rotator object ]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL( rotator );
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	
	//[Create a Nx 1 size pixel bitmap, rotate and mirror it ]
	const TInt xxDimension = 100;
	const TInt yyDimension  = 1;
	CFbsBitmap* nByOne = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( nByOne );
	User::LeaveIfError( nByOne->Create(TSize( xxDimension, yyDimension ),EColor16M));
	
	//[ Fill bit map with grey ]
	TRgb grey = KRgbGray;
	FillBitmapL( *nByOne, grey );
	
	//[Rotate tgtBmp through 90 degrees clockwise ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus, *nByOne, *tgtBmp, CBitmapRotator::ERotation90DegreesClockwise);
	CActiveScheduler::Start();
	   
	//[ check bitmaps are the same, ie transforms have had not net effect]
	result = CheckBitmaps( *nByOne, *tgtBmp, CBitmapRotator::ERotation90DegreesClockwise );
	   
	//[ Pop activeListener, rotator, tgtBitmap, oneByN ]
	CleanupStack::PopAndDestroy(4); 
	
	return result;
	}

/**
constructor for CTestFlipOneByX<T>
This class effectively provides a template DoTestStepL method
which saves code. The class is inherited from to provide boundary tests
@lib "TS_BitmapTransforms.lib"
*/
template <CBitmapRotator::TRotationAngle T>
CTestFlipOneByX<T>::CTestFlipOneByX()
	{
	}

/**
destructor for CTestFlipOneByX<T>
@lib "TS_BitmapTransforms.lib"
*/
template <CBitmapRotator::TRotationAngle T>
CTestFlipOneByX<T>::~CTestFlipOneByX()
	{
	}

/**
This method is a template method to provide rotation and mirroring a bitmap
@lib "TS_BitmapTransforms.lib"
*/
template <CBitmapRotator::TRotationAngle T>
TVerdict CTestFlipOneByX<T>::DoTestStepL( )
	{
	TVerdict result = EPass;
	
    _LIT( KTestRotate, "Normal(Gewoon) Rotate, One by N Bitmap");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks the rotation of a 1 by N bitmap");
	Log( KTestStepRotate );
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
    //[Create a rotator object ]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	
	//[ Create a 1 x 100 size pixel bitmap of colour 128,128,128
	//  rotate and mirror and compare with the original]
	const TInt xDimension = 1;
	const TInt yDimension = 100;
	CFbsBitmap* oneByN = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( oneByN );
	User::LeaveIfError( oneByN->Create(TSize( xDimension, yDimension ),EColor16M));
	   
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *oneByN, grey );
	   
	//[Rotate tgtBmp through 90 degrees clockwise ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus, *oneByN,*tgtBmp, T);
	CActiveScheduler::Start();
	   
	//[ check bitmaps are the same, ie transforms have had not net effect]
	result = CheckBitmaps( *oneByN, *tgtBmp, T );
	   
	//[ Pop activeListener, rotator, tgtBitmap, oneByN ]
	CleanupStack::PopAndDestroy(4); 
	
	return result ;
	}

/**
constructor for CTestStepFlipOneByN90
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipOneByN90::CTestStepFlipOneByN90() 
	{
	// [ set up test name ]
	//"GT0137-18-U-016"
	iTestStepName =  _L( "MM-ICL-BTRANS-U-016-HP");
	}

/**
constructor for CTestStepFlipOneByN180
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipOneByN180::CTestStepFlipOneByN180() 
	{
	// [ set up test name ]
	//"GT0137-18-U-017"
	iTestStepName = _L( "MM-ICL-BTRANS-U-017-HP");
	}

/**
constructor for CTestStepFlipOneByN270
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipOneByN270::CTestStepFlipOneByN270() 
	{
	// [ set up test name ]
    //"GT0137-18-U-018"
	iTestStepName = _L( "MM-ICL-BTRANS-U-018-HP");
	}

/**
constructor CTestStepFlipOneByNHorizontal
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipOneByNHorizontal::CTestStepFlipOneByNHorizontal()
	{
	// [ set up test name ]
    //"GT0137-18-U-019"
	iTestStepName = _L("MM-ICL-BTRANS-U-019-HP");
	}

/**
constructor for CTestStepFlipOnebyNVertical
@lib "TS_BitmapTransforms.lib"
*/

CTestStepFlipOnebyNVertical::CTestStepFlipOnebyNVertical()
	{
	// [ set up test name ]
	//"GT0137-18-U-020"
	iTestStepName = _L( "MM-ICL-BTRANS-U-020-HP");
	}

/**
constructor for CTestStepFlipNByOne90
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipNByOne90::CTestStepFlipNByOne90()
	{
	// [ set up test name ]
	//"GT0137-18-U-021"
	iTestStepName = _L( "MM-ICL-BTRANS-U-021-HP");
	}

/**
constructor for CTestStepFlipNByOne180
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipNByOne180::CTestStepFlipNByOne180()
	{
	// [ set up test name ]
	//"GT0137-18-U-022"
	iTestStepName = _L( "MM-ICL-BTRANS-U-022-HP");
	}

/**
constructor for CTestStepFlipNByOne270
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipNByOne270::CTestStepFlipNByOne270()
	{
	// [ set up test name ]
	//"GT0137-18-U-023"
	iTestStepName = _L(  "MM-ICL-BTRANS-U-023-HP");
	}

/**
constructor for CTestStepFlipNByOneVertical
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipNByOneVertical::CTestStepFlipNByOneVertical()
	{
	// [ set up test name ]
	// "GT0137-18-U-025"
	iTestStepName = _L( "MM-ICL-BTRANS-U-025-HP");
	}

/**
constructor for CTestStepFlipNByOneHorizontal
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFlipNByOneHorizontal::CTestStepFlipNByOneHorizontal()
	{
	// [ set up test name ]
	//"GT0137-18-U-024"
	iTestStepName = _L("MM-ICL-BTRANS-U-024-HP");
	}

/**
constructor for CTestStepFlipXbyOne <T>
@lib "TS_BitmapTransforms.lib"
*/

template <CBitmapRotator::TRotationAngle T>
CTestStepFlipXbyOne <T>::CTestStepFlipXbyOne ()
	{
	}

/**
destructor for CTestStepFlipXbyOne <T>
@lib "TS_BitmapTransforms.lib"
*/

template <CBitmapRotator::TRotationAngle T>
CTestStepFlipXbyOne <T>::~CTestStepFlipXbyOne ()
	{
	}

/**
template method for rotating and mirroring a n by one bitmap
where n > 1
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/

template <CBitmapRotator::TRotationAngle T>
TVerdict CTestStepFlipXbyOne <T>::DoTestStepL( )
	{
	TVerdict result = EPass;
	
    _LIT( KTestRotate, "Normal(Gewoon) Rotate, One by N Bitmap");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks the rotation of a n by one bitmap");
	Log( KTestStepRotate );
	
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
    //[Create a rotator object ]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL( rotator );
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	
	//[Create a Nx 1 size pixel bitmap, rotate and mirror it ]
	const TInt xxDimension = 100;
	const TInt yyDimension  = 1;
	CFbsBitmap* nByOne = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( nByOne );
	User::LeaveIfError( nByOne->Create(TSize( xxDimension, yyDimension ),EColor16M));
	
	//[ Fill bit map with grey ]
	const TRgb grey = KRgbGray;
	FillBitmapL( *nByOne, grey );
	
	//[Rotate tgtBmp through 90 degrees clockwise ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus, *nByOne, *tgtBmp, T );
	CActiveScheduler::Start();
	   
	//[ check bitmaps are the same, ie transforms have had not net effect]
	result = CheckBitmaps( *nByOne, *tgtBmp, T );
	   
	//[ Pop activeListener, rotator, tgtBitmap, oneByN ]
	CleanupStack::PopAndDestroy(4); 
    return result ;
	}


/**
constructor CTestForceRotateMemoryAllocFailure
@lib "TS_BitmapTransforms.lib"
*/

CTestForceRotateMemoryAllocFailure::CTestForceRotateMemoryAllocFailure ()
	: CTestStepBitmapTransforms(EFalse)
	{
	// [ set up test name ]
	//"GT0137-18-U-033"
	iTestStepName = _L("MM-ICL-BTRANS-U-033-HP");
	}

/**
destructor CTestForceRotateMemoryAllocFailure
@lib "TS_BitmapTransforms.lib"
*/

CTestForceRotateMemoryAllocFailure::~CTestForceRotateMemoryAllocFailure ()
	{
	// nothing
	}

void CTestForceRotateMemoryAllocFailure::DoTestStepFbsL( const TFileName& aSourceFileName, CBitmapRotator::TRotationAngle aAngle)
	{
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	RotateAndMirrorABitmapL(aSourceFileName, aAngle);
	CleanupStack::PopAndDestroy(fbsSession);
	}

/**
This method tests heap allocation failures and is called by the test step
@internal There is a problem with this method in that it loops only once !
@returns "enum TVerdict" the status of the test
@lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestForceRotateMemoryAllocFailure::DoTestStepL( )
	{
	TVerdict result = EPass;
    TInt err        = KErrNone;
	
	_LIT( KTestRotate, "Normal(Gewoon) Rotate 270 Degrees with Alloc Failure Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "This test checks allocation failure of the rotator");
	Log( KTestStepRotate );
	
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
	TFileName sourceFileName( KImageTestSourceFileName);
	
	// [ new algorithm adopted from M&G database ]
	//
	
   	TInt  failCount  = 1;
	TBool completed  = EFalse;
	TBool reachedEnd = EFalse; 
	
	for( ;; )
		{
        __UHEAP_SETFAIL(RHeap::EDeterministic ,failCount);
		__MM_HEAP_MARK;
		TRAP(err, DoTestStepFbsL(sourceFileName, CBitmapRotator::ERotation270DegreesClockwise));

		if( err == KErrNone )
			{
			// [ check we have passed through all allocs in the test]
			TAny* testAlloc = User::Alloc(1);
			if( testAlloc == NULL )
				{
				reachedEnd = ETrue;
				result = EPass;
				}
			else 
				{
				User::Free( testAlloc );
				
				}
			// unlike most tests, the check will be made within the test itself, which returns error on bad value
			
			completed = reachedEnd;
			
			}
		else if( err != KErrNoMemory ) 
			{
			// [ we failed for some reason other than memory
			//     allocation, so fail the test ]
			ERR_PRINTF3(_L("Unexpected error from rotate call (%d) after %d iterations"), err, failCount);
			completed = ETrue;
			result    = EFail;
			}
		
		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone ,0);
		
		// [ exit the loop ]
		if( completed )
			{
			break;
			}
		
		failCount +=1;
		}
	
	Log(_L("Number of Alloc errors = %d"), failCount );
	
	return result;
	}


/**
@xxxx
@param aError is the error return code from the Rotate and mirror operation
This function returns EFail if the error returned is neither 
KErrNoMemory or KErrNone
*/


TVerdict CTestForceRotateMemoryAllocFailure::TestFailed( TInt& aError )
	{
	TVerdict result = EPass;
	
	//[ check if there was an error ]
	if( aError != KErrNoMemory && aError != KErrNone )
		{
		result = EFail;
		Log(_L("Error fail code in CTestForceRotateMemoryAllocFailure = %d"), aError );
		}
	
	return result;
	}

/**
constructor CTestCustomCommandCheck
*/

CTestCustomCommandCheck::CTestCustomCommandCheck () : CTestStepBitmapTransforms(EFalse)
	{
	// [ set up test name ]
	//"MM-ICL-BTRANS-U-221-HP"
	iTestStepName = _L("MM-ICL-BTRANS-U-221-HP");
	}

/**
destructor CTestCustomCommandCheck
*/

CTestCustomCommandCheck::~CTestCustomCommandCheck ()
	{
	// nothing
	}

/**
This method checks that CBitmapRotator->CustomCommand return KErrNotSupported
@returns "enum TVerdict" the status of the test
*/

TVerdict CTestCustomCommandCheck::DoTestStepL( )
	{
	TVerdict result = EPass;
    TInt err        = KErrNone;
	
	_LIT( KTestRotate, "CTestCustomCommandCheck() check CBitmapRotator::CustomCommand() Defect INC074719");
	Log( KTestRotate );
	
	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	//[ Attempt to send a (dummy) custom command to the plugin (should return KErrNotSupported ) ]
	err = rotator->CustomCommand(TUid::Uid(101), NULL);
	
	if ( err == KErrNotSupported )
	{
		result = EPass;		
	}
	else
	{
		result = EFail;
	}
		
	//[ Pop and Destroy rotator]
	CleanupStack::PopAndDestroy();	

	return result;
	}

/**
constructor for 180 degrees rotation of a specific bitmap for DEF120342
*/
CTestStepRotate180DegreesMbm::CTestStepRotate180DegreesMbm()
	{
	iTestStepName = _L( "MM-ICL-BTRANS-U-306-HP");
	}


/**
destructor for 180 degrees rotation of a specific bitmap for DEF120342
*/

CTestStepRotate180DegreesMbm::~CTestStepRotate180DegreesMbm()
	{
	}

/**
This method is called by the test suite
and rotates DEF120342's bitmap through 180 degrees
@returns "enum TVerdict" the status of the test
*/
enum TVerdict CTestStepRotate180DegreesMbm::DoTestStepL( )
	{
	_LIT( KTestRotate, "Normal Rotate 180 Degree Test");
	Log( KTestRotate );
	
	_LIT( KTestStepRotate, "DEF120342: This test checks rotating a bitmap 180 Degrees");
	Log( KTestStepRotate );
		
	_LIT(KImageTestSourceFileName, "rotat180.mbm");
	
	TFileName sourceFileName (KImageTestSourceFileName);
	//[ Mirror the bitmap about the horizontal axis ]
    TVerdict result = RotateAndMirrorABitmap( sourceFileName, CBitmapRotator::ERotation180DegreesClockwise, EColor64K);
	
	return result;
	}

/**
constructor for Rotate with Extended Bitmap destination
*/
CTestStepRotateExtendedBitMap::CTestStepRotateExtendedBitMap()
    {
    iTestStepName = _L( "MM-ICL-BTRANS-U-307-HP");
    }


/**
destructor for Rotate with Extended Bitmap destination
*/

CTestStepRotateExtendedBitMap::~CTestStepRotateExtendedBitMap()
    {
    }

/**
@returns "enum TVerdict" the status of the test
*/
enum TVerdict CTestStepRotateExtendedBitMap::DoTestStepL( )
    {
    _LIT( KTestRotate, "Rotate with Extended Bitmap destination");
    Log( KTestRotate );
    
    _LIT( KTestStepRotate, "This test checks rotating a bitmap with Extended Bitmap Destination");
    Log( KTestStepRotate );
        
    _LIT(KImageTestSourceFileName, "rotat180.mbm");
    
    TFileName sourceFileName (KImageTestSourceFileName);
    //[ Mirror the bitmap about the horizontal axis ]
    RotateAndMirrorABitmapL( sourceFileName, CBitmapRotator::ERotation180DegreesClockwise, EColor64K);
    
    return EPass;
    }

/**
*
* CTestStepBitmapTransforms RotateAndMirrorABitmapL
*
* 
* 
*/
void CTestStepRotateExtendedBitMap::RotateAndMirrorABitmapL( const TFileName& aSourceFileName, CBitmapRotator::TRotationAngle aAngle, TDisplayMode aDisplayMode )
    {
    //[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
    
    //[ push the listener on the stack ]
    CleanupStack::PushL( activeListener );
    
    // [create and push a src bimap ]
    CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
    CleanupStack::PushL(srcBmp);

    SetTestFileNameL( aSourceFileName );
    
    User::LeaveIfError(srcBmp->Load( FileName() ));
    
    
    //[create and push a tgt bitmap ]
    CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
    CleanupStack::PushL(tgtBmp);
    
    if(aDisplayMode != ENone)
        {
        srcBmp->SetDisplayMode(aDisplayMode);
        User::LeaveIfError(tgtBmp->CreateExtendedBitmap(
                            TSize(0,0),
                            aDisplayMode,KImageTypeJPGUid,29143,*this));
        }
    else
        {
        User::LeaveIfError(tgtBmp->CreateExtendedBitmap(
                TSize(0,0),
                EColor16M,KImageTypeJPGUid,29143,*this));
        }
    
    //[ create and push a rotator]
    CBitmapRotator* rotator = CBitmapRotator::NewL();
    CleanupStack::PushL(rotator);
    
    //[ rotate or mirror the bitmap through the requisite angle ]
    activeListener->InitialiseActiveListener();
    rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp, aAngle );
    CActiveScheduler::Start();
   TInt err = activeListener->iStatus.Int();
   
    
    if(err!=KErrNotSupported)
        {
        INFO_PRINTF2(_L("Expecteed -5 by now but got  %d "), err);
        User::Leave(err);
        }
    
    INFO_PRINTF1(_L("Got KErrNotSupported as expected. Now trying the other overload"));
    
    activeListener->InitialiseActiveListener();
    rotator->Rotate(&activeListener->iStatus,*tgtBmp, aAngle );
    CActiveScheduler::Start();
    err = activeListener->iStatus.Int();
   
    
    if(err!=KErrNotSupported)
        {
        INFO_PRINTF2(_L("Expecteed -5 by now but got  %d "), err);
        User::Leave(err);
        }
    
    INFO_PRINTF1(_L("Got KErrNotSupported as expected."));
    
    INFO_PRINTF1(_L("Testing the other overload with new scaler object"));
    //test with other overload using new scaler object.
    //[ create and push a rotator]
    CBitmapRotator* rotator2 = CBitmapRotator::NewL();
    CleanupStack::PushL(rotator2);
    activeListener->InitialiseActiveListener();
    rotator2->Rotate(&activeListener->iStatus,*tgtBmp, aAngle );
    CActiveScheduler::Start();
    err = activeListener->iStatus.Int();
    if(err!=KErrNotSupported)
        {
        INFO_PRINTF2(_L("Expecteed -5 by now but got  %d "), err);
        User::Leave(err);
        }
    INFO_PRINTF1(_L("Got KErrNotSupported as expected."));

    CleanupStack::PopAndDestroy(5, activeListener);
    
    
    //[ Pop and Destroy activeListener, srcBmp, tgtBmp, rotator, rotator2]
    
    }

TInt CTestStepRotateExtendedBitMap::InitExtendedBitmap(TAny* /*aData*/, TInt /*aDataSize*/)
    {
    return KErrNone;
    }
