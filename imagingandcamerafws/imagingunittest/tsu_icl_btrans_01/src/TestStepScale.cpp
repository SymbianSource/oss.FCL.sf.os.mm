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
#include "TestStepScale.h"

#include "ICLFbsSessionTest.h"
#include <icl/imagecodecdata.h>


/**
TestStepScale constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScale::CTestStepScale()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// "GT0137-18-U-008"
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-008-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScale
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScale::~CTestStepScale()
	{
	
	}
//Callback for extended bitmap initialization
TInt CTestStepScale::InitExtendedBitmap(TAny* /*aData*/, TInt /*aDataSize*/)
    {
    return KErrNone;
    }


/**
Do the test step.
This test step is provided only as a test example
and serves no purpose in testing the bitmap transforms
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScale::DoTestStepL( )
	{
	// Printing to the console and log file
	_LIT( KTestScale, "Normal(Gewoon) Scale Test");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test checks scaling a bitmap");
	Log( KTestStepScale );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	_LIT(KImageTestSourceFileName2, "64x64.mbm");
	_LIT(KImageTestSourceFileName2Ref, "32x32.mbm");
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(	srcBmp->Load( FileName()) );
	iSrcSize = srcBmp->SizeInPixels();
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor4K));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	tgtBmp->Resize(TSize(iSrcSize.iWidth / 2,iSrcSize.iHeight / 2));

	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
  
	
	activeListener->InitialiseActiveListener();
     __MM_HEAP_MARK;
     scaler->Scale(&activeListener->iStatus, *tgtBmp, TSize(-1, -1));
     CActiveScheduler::Start();
     __MM_HEAP_MARKEND;
     activeListener->InitialiseActiveListener();
     __MM_HEAP_MARK;
     scaler->Scale(&activeListener->iStatus, *tgtBmp, TSize(0, 0));
     CActiveScheduler::Start();
     __MM_HEAP_MARKEND;
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *tgtBmp,TSize(iSrcSize.iWidth * 2,iSrcSize.iHeight * 2));
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
	
	tgtBmp->Resize(TSize(iSrcSize.iWidth,iSrcSize.iHeight / 2));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*tgtBmp,TSize(iSrcSize.iWidth,iSrcSize.iHeight / 2),EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
	
	tgtBmp->Resize(TSize(iSrcSize.iWidth / 2,iSrcSize.iHeight));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*tgtBmp,TSize(iSrcSize.iWidth / 2,iSrcSize.iHeight),EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	scaler->Cancel();
	activeListener->Cancel();
	activeListener->InitialiseActiveListener();
    scaler->Scale(&activeListener->iStatus,*tgtBmp,srcBmp->SizeInPixels());
	scaler->Cancel();
	activeListener->Cancel();
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp,EFalse);
	scaler->Cancel();
	activeListener->Cancel();
	activeListener->InitialiseActiveListener();
    scaler->Scale(&activeListener->iStatus,*tgtBmp,srcBmp->SizeInPixels(),EFalse);
	scaler->Cancel();
	activeListener->Cancel();

// [ Set aspect ratio to true.Supply (X,Y) destination bitmap and calculate (X,0) destination bitmap (X,Y) -> (X,0)  ]
	srcBmp->Resize(TSize(25,25));
	tgtBmp->Resize(TSize(1, 25));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	iTgtSize = tgtBmp->SizeInPixels();
	if(iTgtSize != TSize(1,1))
		{
		INFO_PRINTF3(_L("Expected output of (X,0)destination bitmap is (1,0).Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		iTestStepResult = EFail;
		}
	if(activeListener->iStatus!=KErrNone )
		{
		User::Leave( activeListener->iStatus.Int() );
		}
// [ Set aspect ratio to true.Supply (X,Y) destination bitmap and calculate (0,Y) destination bitmap (X, Y) -> (0,Y)  ]
	tgtBmp->Resize(TSize(25, 1));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	iTgtSize = tgtBmp->SizeInPixels();
	if(iTgtSize != TSize(1,1))
		{
		INFO_PRINTF3(_L("Expected output of (0,Y)destination bitmap is (0,1).Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight);		
		iTestStepResult = EFail;
		}
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}	
//	[ Set aspect ratio to false and check scale of (0,Y) bitmap ]
	srcBmp->Resize(TSize(0,25));
	tgtBmp->Resize(TSize(0,50));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp,EFalse);
	iTgtSize = tgtBmp->SizeInPixels();
	CActiveScheduler::Start();
	if(iTgtSize !=  TSize(0,50))
		{
		INFO_PRINTF3(_L("When aspect ratio false expected output of  (0,Y) destination bitmap is(0,50).Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight);
		iTestStepResult = EFail;
		}
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}

//	[ Set aspect ratio to false and check scale of(X,0) bitmap ]
	srcBmp->Resize(TSize(25,0));
	tgtBmp->Resize(TSize(50,0));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp,EFalse);
	iTgtSize = tgtBmp->SizeInPixels();
	CActiveScheduler::Start();
	if(iTgtSize !=  TSize(50,0))
		{
		INFO_PRINTF3(_L("When aspect ratio false expected output of (X,0) destination bitmap is (50,0).Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight);
		iTestStepResult = EFail;
		}
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave( activeListener->iStatus.Int() );
		}
//	[ Set aspect ratio to true and check scale of (0,Y) bitmap ]
	srcBmp->Resize(TSize(0,25));
	tgtBmp->Resize(TSize(0,50));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp );
	iTgtSize = tgtBmp->SizeInPixels();
	CActiveScheduler::Start();
	if(iTgtSize !=  TSize(0,50))
		{
		INFO_PRINTF3(_L("When aspect ratio true expected output of (0,Y) destination bitmap is (0,50).Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight);
		iTestStepResult = EFail;
		}
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}

//	[ Set aspect ratio to true and check scale of(X,0) bitmap ]
	srcBmp->Resize(TSize(25,0));
	tgtBmp->Resize(TSize(50,0));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	iTgtSize = tgtBmp->SizeInPixels();
	CActiveScheduler::Start();
	if(iTgtSize !=  TSize(50,0))
		{
		INFO_PRINTF3(_L("When aspect ratio true expected output of (X,0) destination bitmap is (50,0).Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight);
		iTestStepResult = EFail;
		}
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
// [ Set aspect ratio to false and check scale of (0,Y)->(X,Y) ]
	srcBmp->Resize(TSize(0,100));
	tgtBmp->Resize(TSize(100,100));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp,EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrArgument) 
		{
		User::Leave(activeListener->iStatus.Int());
		}

//	[ Set aspect ratio to false and check scale of (X,0)->(X,Y) ]
	srcBmp->Resize(TSize(100,0));
	tgtBmp->Resize( TSize(100,100));
	activeListener->InitialiseActiveListener();
	scaler->Scale( &activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	CActiveScheduler::Start();
	if( activeListener->iStatus != KErrArgument)
		{
		User::Leave( activeListener->iStatus.Int() );
		}

// [ Set aspect ratio to true and check scale of (0,Y)->(X,Y) ]
	srcBmp->Resize(TSize(0,100));
	tgtBmp->Resize(TSize(100,100));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrArgument) 
		{
		User::Leave(activeListener->iStatus.Int());
		}

//	[ Set aspect ratio to true and check scale of (X,0)->(X,Y) ]
	srcBmp->Resize(TSize(100,0));
	tgtBmp->Resize(TSize(100,100));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrArgument)
		{
		User::Leave(activeListener->iStatus.Int());
		}	

	// Test PDEF099310: Resizing the image under messaging results in distortion of the image
	SetTestFileNameL(KImageTestSourceFileName2);
	
	User::LeaveIfError(srcBmp->Load(FileName()));
	iSrcSize = srcBmp->SizeInPixels();

	tgtBmp->SetDisplayMode(EGray2);
	tgtBmp->Resize(TSize(iSrcSize.iWidth / 2, iSrcSize.iHeight / 2));

	activeListener->InitialiseActiveListener();
	scaler->DisablePostProcessing(ETrue);
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}

	SetTestFileNameL(KImageTestSourceFileName2Ref);
	User::LeaveIfError(srcBmp->Load(FileName()));	
	if (CheckBitmaps(*tgtBmp, *srcBmp, -1) == EFail)
		{
		INFO_PRINTF2(_L("Output does not match ref %s"), &KImageTestSourceFileName2Ref);		
		iTestStepResult = EFail;		
		}
	
	//Try to scale with an extended bitmap Destination
	CFbsBitmap* tgtBmpExtended = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmpExtended);
	
	User::LeaveIfError(tgtBmpExtended->CreateExtendedBitmap(
	                TSize(0,0),
	                EColor16M,KImageTypeJPGUid,29143,*this));
	
	//Scale with extended bitmap. Test the overload of scale that takes two bitmaps. 
	//start with new scaler object; don't reuse the scaler object.
	INFO_PRINTF1(_L("Scaling with extended bitmap - two bitmap inputs"));
	CBitmapScaler* scaler_twoExtBmp = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler_twoExtBmp);
	activeListener->InitialiseActiveListener();
    scaler_twoExtBmp->Scale(&activeListener->iStatus,*srcBmp,*tgtBmpExtended);
    CActiveScheduler::Start();
    if(activeListener->iStatus!=KErrNotSupported)
        {
        //Expected -5 for extended bitmap
        User::Leave(activeListener->iStatus.Int());
        }
	INFO_PRINTF1(_L("Scaling with extended bitmap - two bitmap inputs : successful"));

	//Scale with extended bitmap.Test overload of scale that takes single bitmap. Use new scaler object.
	INFO_PRINTF1(_L("Scaling with extended bitmap - single bitmap input"));
	CBitmapScaler* scaler_oneExtBmp = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler_oneExtBmp);
	const TSize tgtSize(0,0);
	activeListener->InitialiseActiveListener();
    scaler_oneExtBmp->Scale(&activeListener->iStatus,*tgtBmpExtended, tgtSize);
    CActiveScheduler::Start();
    if(activeListener->iStatus!=KErrNotSupported)
        {
        //Expected -5 for extended bitmap
        User::Leave(activeListener->iStatus.Int());
        }
	INFO_PRINTF1(_L("Scaling with extended bitmap - single bitmap input : successful"));

    //[scaler_oneExtBmp, scaler_twoExtBmp, tgtBmpExtended, scaler, tgtBmp, srcBmp, activeListener]
	CleanupStack::PopAndDestroy(7, activeListener);
	
	return iTestStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @xxxx
* 
*/

CTestSrcBitmapPanicAlloc::CTestSrcBitmapPanicAlloc()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	//"GT0137-18-U-009"
	_LIT( KSrcBitmapPanicAlloc, "MM-ICL-BTRANS-U-009-HP");
	iTestStepName = KSrcBitmapPanicAlloc ;
	}

/**
*
* Test step destructor.
*
* @xxxx
* 
*/

CTestSrcBitmapPanicAlloc::~CTestSrcBitmapPanicAlloc()
	{
	
	}

/**
Do the test step.
This Unit test, checks that a panic is raised when the src bitmap
supplied to a CBitmapScaler Scale operation has no space allocated.
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestSrcBitmapPanicAlloc::DoTestStepL( )
	{
	// [ Check a panic is raised when no memeory is allocated for the src bitmap]
	_LIT( KTestName,"CTestSrcBitmapPanicAlloc");
	_LIT( KTestMessage, "this test, checks that a panic is raised when the src bitmap has no space allocated" );
	Log( KTestName );
	Log( KTestMessage );
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	//[ Create a src bitmap but do not allocate any space
	// for it, and test rotates panic]
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	   
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	//[ set size to some arbitrary value since the src bitmap is not allocated anyway]
	const TSize size( 100, 100 );
	
	//[cause panic with src bit map ]
	activeListener->InitialiseActiveListener();
	
	// [ scale the bitmap ]
	scaler->Scale(&activeListener->iStatus,*srcBmp,TSize(size.iWidth,size.iHeight),EFalse);
	CActiveScheduler::Start();
	
	//[ should never get here, fail with any old error code ]
	iTestStepResult = EFail;
	
	//[ srcBmp, scaler activeListener ]
	CleanupStack::PopAndDestroy(3);
	
		
	return iTestStepResult;
	}



/**
Test step constructor.
Each test step initialises 
@lib "TS_BitmapTransforms.lib"
*/
CTestTgtBitmapPanicAlloc::CTestTgtBitmapPanicAlloc()
	{
	
	// store the name of this test case
	// this is the name that is used by the script file
	//"GT0137-18-U-010"
	_LIT( KTgtBitmapPanicAlloc, "MM-ICL-BTRANS-U-010-HP");
	iTestStepName = KTgtBitmapPanicAlloc ;
	}

/**
Test step destructor CTestTgtBitmapPanicAlloc
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestTgtBitmapPanicAlloc::~CTestTgtBitmapPanicAlloc()
	{
	
	}


/**
Do the test step.
This Unit test checks that a panic is raised when the target bitmap
supplied to a CBitmapScaler Scale operation has no space allocated.
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestTgtBitmapPanicAlloc::DoTestStepL( )
	{
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
    //[ Create a 100 x 100 size pixel bitmap of colour 128,128,128
    //  and a tgt bitmap with no soace ]
	const  TInt xDimension = 100;
	const  TInt yDimension = 100;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( xDimension, yDimension ),EColor16M));
	
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	
	//[ allocate a target bitmap but do not create it]
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	INFO_PRINTF2(_L("Target bitmap Handle = %X"), tgtBmp->Handle()  );

	activeListener->InitialiseActiveListener();
	scaler->Scale( &activeListener->iStatus, *srcBmp, *tgtBmp);
	CActiveScheduler::Start();
	
	INFO_PRINTF2(_L("Target bitmap Handle = %X"), tgtBmp->Handle() );

	// [ if we get here the test failed ]
	iTestStepResult = EFail;
	
	//[ srcBmp, activeListener, scaler, tgtBmp ]
	CleanupStack::PopAndDestroy(4);
	
	return iTestStepResult;
	}


/**
constructor CTestScaleAllocFailure
Each test step initialises its own name.
@return	"enum TVerdict"
The result of the test step
*/

CTestScaleAllocFailure::CTestScaleAllocFailure()
	{
	
	// store the name of this test case
	// this is the name that is used by the script file
	//"GT0137-18-U-011"
	_LIT( KTScaleAllocFailure, "MM-ICL-BTRANS-U-011-HP");
	iTestStepName = KTScaleAllocFailure ;
	}

/**
destructor. for CTestScaleAllocFailure
@lib "TS_BitmapTransforms.lib" 
*/

CTestScaleAllocFailure::~CTestScaleAllocFailure()
	{

	}


/**
This method is called by the test suite and tests memory alloc failures
by configuring the heap to fail and looking at the trapped error code
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib" 
*/
TVerdict CTestScaleAllocFailure::DoTestStepL( )
	{
	
	TInt err       = KErrNone;
	iTestStepResult = EPass;
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	//[ Create a 100 x 100 size pixel bitmap of colour 128,128,128
	//  rotate and mirror and compare with the original]
	const  TInt xDimension = 100;
	const  TInt yDimension = 100;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( xDimension, yDimension ),EColor16M));
	
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);	
	
	const TSize size(srcBmp->SizeInPixels());
	activeListener->InitialiseActiveListener();
	TRAP( err, scaler->Scale( &activeListener->iStatus,*tgtBmp,TSize(size.iWidth,size.iHeight / 2),EFalse));
	if (err != KErrNone)
		{
		iTestStepResult = EFail;
		User::LeaveIfError( err );
		}
	CActiveScheduler::Start();
	
	//[ activeListener  srcBmp, TgtBmp, scaler ]
	CleanupStack::PopAndDestroy(4);
		
	return iTestStepResult;
	} 

/**
constructor for CTestStepScaleXY<srcWidth, srcHeight, maintainAspectRatio, dstWidth, dstHeight>
template which acts as a base class and provides a template method to reduce code bloat
@lib "TS_BitmapTransforms.lib" 
*/

template <TInt srcWidth, TInt srcHeight, 
TBool maintainAspectRatio, TInt dstWidth, TInt dstHeight> 
CTestStepScaleXY<srcWidth, srcHeight, maintainAspectRatio, dstWidth, dstHeight>::CTestStepScaleXY()
	{
	// empty
	}


/**
destructor CTestStepScaleXY<srcWidth, srcHeight, maintainAspectRatio, dstWidth, dstHeight>
*/

template <TInt srcWidth, TInt srcHeight, 
TBool maintainAspectRatio, TInt dstWidth, TInt dstHeight> 
CTestStepScaleXY<srcWidth, srcHeight, maintainAspectRatio, dstWidth, dstHeight>::~CTestStepScaleXY()
	{
	// empty
	}


/**
This is a template method whose parameters are resolved by inheritance 
in the derived classes
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib" 
*/

template <TInt srcWidth, TInt srcHeight, 
TBool maintainAspectRatio, TInt dstWidth, TInt dstHeight> 
TVerdict CTestStepScaleXY<srcWidth, srcHeight, maintainAspectRatio, dstWidth, dstHeight>::DoTestStepL( )
	{
	TVerdict result = EPass;
	
    _LIT( KTestScale, "Normal(Gewoon) Panic Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test checks the attempted scaling of a zero size bitmap");
	Log( KTestStepScale );
	
    //[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( srcWidth, srcHeight ),EColor16M));
	
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	
	//[ allocate a target bitmap but do not create it]
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	
	//[ create a tgt bit map of the required size ]
	const TSize size( dstWidth, dstHeight);
	User::LeaveIfError( tgtBmp->Create( size,EColor16M));
	
	//[ create a scaler object]
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	// [ attempt to scale the src to the target ]
	activeListener->InitialiseActiveListener();
	
	scaler->Scale( &activeListener->iStatus, *srcBmp, *tgtBmp, maintainAspectRatio);
	CActiveScheduler::Start();
	if ( activeListener->iStatus != KErrArgument )
		{
		result = EFail;
		}
	
	//[ srcBmp, activeListener, scaler, tgtBmp ]
	CleanupStack::PopAndDestroy(4);
		
	return result;
	}


/*
constuctor for CTestStepScaleZeroByZeroSrc
@lib "TS_BitmapTransforms.lib" 
*/

CTestStepScaleZeroByZeroSrc::CTestStepScaleZeroByZeroSrc()
	{
	// [ set up test name ]
	//"GT0137-18-U-026"
	iTestStepName = _L( "MM-ICL-BTRANS-U-026-HP"); 
	}


/**
destructor for CTestStepScaleZeroByZeroSrc
@lib "TS_BitmapTransforms.lib" 
*/

CTestStepScaleZeroByNSrc::CTestStepScaleZeroByNSrc()
	{
	// [ set up test name ]
    //"GT0137-18-U-027"
	iTestStepName = _L( "MM-ICL-BTRANS-U-027-HP"); 
	}

/**
constructor for CTestStepScaleNByZeroSrc
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScaleNByZeroSrc::CTestStepScaleNByZeroSrc()
	{
	// [ set up test name ]
	//"GT0137-18-U-028"
	iTestStepName = _L( "MM-ICL-BTRANS-U-028-HP"); 
	}

/**
constructor CTestStepScaleOneToOne<width, height, maintainAspectRatio >
@lib "TS_BitmapTransforms.lib"
*/
template <TInt width, TInt height, TBool maintainAspectRatio> 
CTestStepScaleOneToOne<width, height, maintainAspectRatio >::CTestStepScaleOneToOne()
	{
	// nothing here ..
	}

/**
destructor CTestStepScaleOneToOne<width, height, maintainAspectRatio >
@lib "TS_BitmapTransforms.lib"
*/

template <TInt width, TInt height, TBool maintainAspectRatio> 
CTestStepScaleOneToOne<width, height, maintainAspectRatio >::~CTestStepScaleOneToOne()
	{
	// nothing here ..
	}

/**
This is a template method that is called by the test suite in derived classes
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/

template <TInt width, TInt height, 
TBool maintainAspectRatio > 
TVerdict CTestStepScaleOneToOne<width, height, maintainAspectRatio>::DoTestStepL( )
	{
	 
	//[ create a src and target bit map of Width and Height and scale src to target]
    TVerdict result = EPass;
	
    _LIT( KTestScale, "Normal(Gewoon) Scale 1:1 with compare");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test does a 1:1 scaling of a source bitmap and compares it to the target");
	Log( KTestStepScale );
	
    //[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( width, height ),EColor16M));
	
	//[ set the pixles of oneByN to gray]
	TRgb grey = KRgbGray;
	FillBitmapL( *srcBmp, grey );
	
	//[ allocate a target bitmap but do not create it]
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	
	//[ create a tgt bit map of the required size ]
	const TSize size( width, height);
	User::LeaveIfError( tgtBmp->Create( size, EColor16M));
	
	//[ create a scaler object]
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	// [ attempt to scale the src to the target ]
	activeListener->InitialiseActiveListener();
	
	scaler->Scale( &activeListener->iStatus, *srcBmp, *tgtBmp, maintainAspectRatio);
	CActiveScheduler::Start();
	User::LeaveIfError(activeListener->iStatus.Int());
   	
	// [compare the bitmaps ]
	//[ check bitmaps are the same, ie transforms have had not net effect]
	result = CheckBitmaps( *srcBmp, *tgtBmp, -1 );
	
	//[ srcBmp, activeListener, scaler, tgtBmp ]
	CleanupStack::PopAndDestroy(4);
	
	return result;
	}

/**
constructor CTestStepScaleNByOne
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScaleNByOne::CTestStepScaleNByOne()
	{
	// [ set up test name ]
	//"GT0137-18-U-029"
	iTestStepName = _L("MM-ICL-BTRANS-U-029-HP"); 
	}

/**
constructor CTestStepScaleOneByN
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScaleOneByN::CTestStepScaleOneByN()
	{
	// [ set up test name ]
	//"GT0137-18-U-030"
	iTestStepName = _L( "MM-ICL-BTRANS-U-030-HP" ); 
	}

/**
constructor CTestStepScaleUpAndDown
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScaleUpAndDown::CTestStepScaleUpAndDown()
	{
	// [ set up test name ]
    //"GT0137-18-U-031"
	iTestStepName = _L( "MM-ICL-BTRANS-U-031-HP"); 
	}

/**
destructor CTestStepScaleUpAndDown
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScaleUpAndDown::~CTestStepScaleUpAndDown()
	{
	// nothing here
	}

/**
[ generate a bitmap whose dimensions are divisible by 2 and
scale it up and down by 2 and 1/2 respectively
compare the dimesions with the original ]
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestStepScaleUpAndDown::DoTestStepL( )
	{
	 TVerdict result = EPass;
	
	// [ for this test to work the scale up and scale down must done
	// on a pattern which is invariant to these ops ie divisible by two
	// at the pattern level ]
	const TInt width  = 128;
	const TInt height = 96;
	
    _LIT( KTestScale, "Normal(Gewoon) Scale up and down with compare");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test does a  scaling up and down followed by a compare ");
	Log( KTestStepScale );
	
    //[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( width, height ),EColor16M));
	
	//[ build a checker board ]
	FillWithCheckerBoardL( *srcBmp );  
	
	//[ allocate a target bitmap but do not create it]
	CFbsBitmap* tgtScaleUpBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtScaleUpBmp);
	
	//[ create a tgt Scale up bit map of the required size ]
	const TSize sizeScaleUp( width*2, height*2);
	User::LeaveIfError( tgtScaleUpBmp->Create( sizeScaleUp, EColor16M));
	
	//[ allocate a target ScaleDown but do not create it]
	CFbsBitmap* tgtScaleDownBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtScaleDownBmp);
	
	//[ create a tgt bit map of the required size, ie the same as the original ]
	const TSize sizeScaleDown( width, height);
	User::LeaveIfError( tgtScaleDownBmp->Create( sizeScaleDown, EColor16M));
	
	//[ create a scaler object]
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	// [ attempt to scale the src to the target ]
	activeListener->InitialiseActiveListener();
	scaler->Scale( &activeListener->iStatus, *srcBmp, *tgtScaleUpBmp, EFalse );
	CActiveScheduler::Start();
	TInt errCode = KErrNone;
	if ( activeListener->iStatus != errCode )
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
	activeListener->InitialiseActiveListener();
	scaler->Scale( &activeListener->iStatus, *tgtScaleUpBmp, *tgtScaleDownBmp, EFalse );
	CActiveScheduler::Start();
	errCode = KErrNone;
	if ( activeListener->iStatus != errCode )
		{
		User::Leave(activeListener->iStatus.Int());
		}
		
	// [compare the bitmaps ]
	//[ check bitmaps are the same, ie transforms have had not net effect]
	//result = CheckBitmaps( *srcBmp, *tgtScaleDownBmp, -1 );
	// [bitmaps are different. The only comparison for now is that they 
	// are the same size]
	if( srcBmp->SizeInPixels() == tgtScaleDownBmp->SizeInPixels() )
		{
		result = EPass;
		}
	
	
	//[ srcBmp, activeListener, scaler, tgtScaleUpBmp tgtScaleDownBmp ]
	CleanupStack::PopAndDestroy(5);
	
	return result;
	}

/**
This method fills a bitmap with a checker board
@param srcBmp bitmap to be filled with checker board pattern
should have dimensions modulo 8 on each axis, otherwise
part of the bitmap will not be filled
@returns void
@internal This function does not work properly because of the 
method used to fill the bitmap. Probable cause is memory alignment
@lib "TS_BitmapTransforms.lib"
*/
void CTestStepScaleUpAndDown::FillWithCheckerBoardL( CFbsBitmap& srcBmp )
	{
	TInt err = KErrNone ;
	const TSize sizeBmp = srcBmp.SizeInPixels();
	const TInt bmpHeight = sizeBmp.iWidth;
	const TInt bmpWidth  = sizeBmp.iHeight;
	
	CFbsBitmapDevice* device = NULL;
	
	// [ create a bitmap device ]
	TRAP(err, device = CFbsBitmapDevice::NewL( &srcBmp ));
	User::LeaveIfError( err );
	
	CleanupStack::PushL( device);
	
	// [ create a device context ]
	CFbsBitGc* context = NULL;	
	User::LeaveIfError( device->CreateContext( context ) );
	
	CleanupStack::PushL( context);
	
	//[fill the bmp with a check board pattern ]
	const TInt blockHeight = 4;
	const TInt blockWidth  = 4;
	const TInt blockHeightByTwo = blockHeight * 2 ;
	const TInt blockWidthByTwo  = blockWidth * 2  ;
	
	for( TInt i = 0; i < bmpHeight; i+= blockHeightByTwo  )
		{
		for( TInt j = 0; j < bmpWidth ; j+= blockWidthByTwo )
			{
			// [ draw four rectangles of different colours ]
			context->SetBrushColor( KRgbWhite );
			TRect rect1( TPoint( i, j ), TSize( blockWidth, blockHeight ) );
			context->DrawRect( rect1 );
			
			//context->SetBrushColor( KRgbRed );
			TRect rect2( TPoint( i, j+ blockHeight ), TSize( blockWidth, blockHeight ) );
			context->DrawRect( rect2 );
			
			//context->SetBrushColor( KRgbBlue );
			TRect rect3( TPoint( i+blockWidth, j ), TSize( blockWidth, blockHeight ) );
			context->DrawRect( rect3 );
			
			//context->SetBrushColor( KRgbGreen );
			TRect rect4( TPoint( i+blockWidth, j+blockHeight ), TSize( blockWidth, blockHeight ) );
			context->DrawRect( rect4 );
			}
		}
	
	//[ context, device ]
	CleanupStack::PopAndDestroy(2);
	}

	   
/**
constructor CTestForceScaleMemoryAllocFailure
@lib "TS_BitmapTransforms.lib"
*/
	   
CTestForceScaleMemoryAllocFailure::CTestForceScaleMemoryAllocFailure()
	: CTestStepBitmapTransforms(EFalse)
	{
	// [ set up test name ]
	//"GT0137-18-U-032"
	iTestStepName = _L("MM-ICL-BTRANS-U-032-HP");
	}
	   
/**
destructor CTestForceScaleMemoryAllocFailure
@lib "TS_BitmapTransforms.lib"
*/

CTestForceScaleMemoryAllocFailure::~CTestForceScaleMemoryAllocFailure ()
	{
	
	}

TVerdict CTestForceScaleMemoryAllocFailure::DoTestStepFbsL()
	{
	TVerdict result = EPass;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	result = ScaleABitMapL();
	CleanupStack::PopAndDestroy(fbsSession);
	return result;
	}

/**
This method tests heap allocation failures for the scalor
@return	"enum TVerdict"
The result of the test step
*/
TVerdict  CTestForceScaleMemoryAllocFailure::DoTestStepL( )
	{
	TInt      err        = KErrNone;
	TVerdict  result     = EPass;

	// [ new algorithm adopted from M&G database ]
	//
	
	// Printing to the console and log file
	_LIT( KTestScaleEntry, "CTestForceScaleMemoryAllocFailure::DoTestStepL <Enter>");
	Log( KTestScaleEntry );

   	TInt  failCount  = 1;
	TBool completed  = EFalse;
	TBool reachedEnd = EFalse; 
	for( ;; )
		{
        __UHEAP_SETFAIL(RHeap::EDeterministic ,failCount);
		__MM_HEAP_MARK;

		//[ scale a bitmap and check the result ]
		TRAP(err, result = DoTestStepFbsL());
		if( err == KErrNone )
			{
			// [ check we have passed through all allocs in the test]
			TAny* testAlloc = User::Alloc(1);
			if( testAlloc == NULL )
				{
				reachedEnd = ETrue;
				failCount  = 1;
				}
			else 
				{
				User::Free( testAlloc );
				//[ this is unknown territory
				// we have not reached the end of allocs and
				// have succeeded ]

				// Even though we have passed there are still allocs to do,
				// so don't finish yet. (Line below removed SF: 14/01/04)
				// result = EPass;
                  
				}

			completed = reachedEnd || (result != EPass);
			}
		else if( err != KErrNoMemory ) 
			{
			 // [ we failed for some reason other than memory
			 //     allocation, so fail the test ]
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
	
	_LIT( KTestScaleExit, "CTestForceScaleMemoryAllocFailure::DoTestStepL <Exit>");
	Log( KTestScaleExit );
	
	return result;
	}


/**
@xxxx
This function scales a bitmap
*/

TVerdict CTestForceScaleMemoryAllocFailure::ScaleABitMapL()
	{
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	_LIT(KImageTestRefFileName, "imagevid_8Ref.mbm");
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(	srcBmp->Load( FileName()) );
	const TSize size(srcBmp->SizeInPixels());
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor4K));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	tgtBmp->Resize(TSize(size.iWidth / 2,size.iHeight / 2));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		User::Leave(activeListener->iStatus.Int());
	SetTestFileNameL(KImageTestRefFileName);
	TVerdict result = CheckBitmapL(*tgtBmp);
	//pop [ activeListener srcBmp, tgtBmp, scaler  ]
	CleanupStack::PopAndDestroy(4);
	return result;
	}

/**
@xxxx
TestError
Reports an error
*/

void CTestForceScaleMemoryAllocFailure::TestFailed( TInt& aErr )
	{
	Log(_L("(Memory Alloc Failure) Number of Failures: KeyTInt = %d"), aErr );
	}


/**
Test step constructor CTestStepDisplayModeScale
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestStepDisplayModeScale::CTestStepDisplayModeScale()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// "GT0137-18-U-062"
	_LIT( KDisplayModeScaleExample,"MM-ICL-BTRANS-U-062-HP" );
	iTestStepName = KDisplayModeScaleExample ;
	}

/**
Test step destructor CTestStepDisplayModeScale
@lib "TS_BitmapTransforms.lib"
*/

CTestStepDisplayModeScale::~CTestStepDisplayModeScale()
	{
	}

/**
Do the test step.
This unit test checks that scaling of a bitmap with  
display mode ECOLOR16MU.
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestStepDisplayModeScale::DoTestStepL( )
	{
	_LIT( KTestStepDisplayModeScale, "This test checks scaling a bitmap with 16MU display mode");
	 Log( KTestStepDisplayModeScale );
	
	_LIT(KImageTestSourceFileName, "imagevid_8.mbm");
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Load( FileName()) );
		
	//[ Create a target bitmap ]
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(50,50),EColor16MU));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();

	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}

	TDisplayMode mode = tgtBmp->DisplayMode();
	if( mode != EColor16MU)
		{
		INFO_PRINTF2(_L("Expected displaymode of target bitmap is EColor16MU(11).Actual displaymode received is %d"), mode);
		iTestStepResult = EFail;
		}
		
	//[ scaler,tgtBmp, srcBmp,activeListener ]
	CleanupStack::PopAndDestroy(4, activeListener);
	
	return iTestStepResult;
	}


/**
CTestStepScaleRoundoff constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestStepScaleRoundoff::CTestStepScaleRoundoff()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// "GT0137-18-U-008"
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-063-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleRoundoff
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleRoundoff::~CTestStepScaleRoundoff()
	{
	}

/**
Do the test step.
This test step scales the bitmap of size(288,216)
to (160,120) and checks if the Scalar rounds 
the target size correctly.
(DEF045698)	
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScaleRoundoff::DoTestStepL( )
	{
		
	iTestStepResult = EPass;
	
	INFO_PRINTF1(_L("This test step scales the bitmap of size(288,216) to (160,120)"));
	INFO_PRINTF1(_L("and checks if the Scalar rounds the target size correctly(DEF045698)"));	
	
		
	_LIT(KImageTestSourceFileName, "test.mbm");
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(	srcBmp->Load( FileName()) );
	iSrcSize = srcBmp->SizeInPixels();
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor4K));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	tgtBmp->Resize(TSize(160,120));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
  
	iTgtSize = tgtBmp->SizeInPixels();
	
	if(iTgtSize != TSize(160,120))
		{
		INFO_PRINTF3(_L("Expected output is (160,120) But Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		iTestStepResult = EFail;
		}
	//[ scaler, tgtBmp, srcBmp, activeListener ]
	CleanupStack::PopAndDestroy(4, activeListener);
	
	return iTestStepResult;
	
	}
	

/**
CTestStepScaleWhiteFillMaxQuality constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFillMaxQuality::CTestStepScaleWhiteFillMaxQuality()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// ""
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-200-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleWhiteFillMaxQuality
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFillMaxQuality::~CTestStepScaleWhiteFillMaxQuality()
	{
	}

/**
Do the test step.
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScaleWhiteFillMaxQuality::DoTestStepL( )
	{
	iTestStepResult = EPass;
	INFO_PRINTF1(_L("This test step scales the bitmap of size(162,122) to (648,486)"));
	INFO_PRINTF1(_L("and checks the whitefill (DEF056170) for Maximum Quality Algorithm "));	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	TInt srcX = 162;
	TInt srcY = 122;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( srcX, srcY ), EColor256));
	//[ set the pixles of oneByN to red]
	TRgb red = KRgbRed;
	FillBitmapL( *srcBmp, red );
	TInt tgtX = 648;
	TInt tgtY = 486;
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0, 0), EColor256));
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	tgtBmp->Resize(TSize(tgtX, tgtY));
	activeListener->InitialiseActiveListener();
	//Enable Maximum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMaximumQuality);
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	iTgtSize = tgtBmp->SizeInPixels();
	if(iTgtSize != TSize(tgtX, tgtY))
		{
		INFO_PRINTF3(_L("Expected output is (160, 120) But Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		CleanupStack::PopAndDestroy(4, activeListener);//[ scaler, tgtBmp, srcBmp, activeListener ]
		return EFail;
		}
	CFbsBitmap* redBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( redBmp );
	User::LeaveIfError( redBmp->Create(TSize( tgtX, tgtY ), EColor256));
	//[ set the pixles of oneByN to red]
	FillBitmapL( *redBmp, red );	
	// compare with red bitmap
	iTestStepResult = CheckBitmaps(*tgtBmp, *redBmp, -1 );	
	CleanupStack::PopAndDestroy(5, activeListener);//[ scaler, tgtBmp, srcBmp,redBmp, activeListener ]
	return iTestStepResult;
	}

/**
CTestStepScaleWhiteFillMedQuality constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFillMedQuality::CTestStepScaleWhiteFillMedQuality()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// ""
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-201-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleWhiteFillMedQuality
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFillMedQuality::~CTestStepScaleWhiteFillMedQuality()
	{
	}

/**
Do the test step.
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScaleWhiteFillMedQuality::DoTestStepL( )
	{
	iTestStepResult = EPass;
	INFO_PRINTF1(_L("This test step scales the bitmap of size(162,122) to (648,486)"));
	INFO_PRINTF1(_L("and checks the whitefill (DEF056170) for Medium Quality Algorithm "));	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	TInt srcX = 162;
	TInt srcY = 122;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( srcX, srcY ),EColor256));
	//[ set the pixles of oneByN to red]
	TRgb red = KRgbRed;
	FillBitmapL( *srcBmp, red );
	TInt tgtX = 648;
	TInt tgtY = 486;
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0, 0), EColor256));
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	tgtBmp->Resize(TSize(tgtX, tgtY));
	activeListener->InitialiseActiveListener();
	//Enable Medium Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	iTgtSize = tgtBmp->SizeInPixels();
	if(iTgtSize != TSize(tgtX, tgtY))
		{
		INFO_PRINTF3(_L("Expected output is (160, 120) But Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		CleanupStack::PopAndDestroy(4, activeListener);//[ scaler, tgtBmp, srcBmp, activeListener ]
		return EFail;
		}
	CFbsBitmap* redBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( redBmp );
	User::LeaveIfError( redBmp->Create(TSize( tgtX, tgtY ), EColor256));
	//[ set the pixles of oneByN to red]
	FillBitmapL( *redBmp, red );	
	// compare with red bitmap
	iTestStepResult = CheckBitmaps(*tgtBmp, *redBmp, -1 );	
	CleanupStack::PopAndDestroy(5, activeListener);//[ scaler, tgtBmp, srcBmp, redbmp, activeListener ]
	return iTestStepResult;
	}

/**
CTestStepScaleWhiteFillMiniQuality constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFillMiniQuality::CTestStepScaleWhiteFillMiniQuality()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// ""
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-202-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleWhiteFillMiniQuality
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFillMiniQuality::~CTestStepScaleWhiteFillMiniQuality()
	{
	}

/**
Do the test step.
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScaleWhiteFillMiniQuality::DoTestStepL( )
	{
	iTestStepResult = EPass;
	INFO_PRINTF1(_L("This test step scales the bitmap of size(162,122) to (648,486)"));
	INFO_PRINTF1(_L("and checks the whitefill (DEF056170) for Medium Quality Algorithm "));	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	TInt srcX = 162;
	TInt srcY = 122;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( srcX, srcY ), EColor256));
	//[ set the pixles of oneByN to red]
	TRgb red = KRgbRed;
	FillBitmapL( *srcBmp, red );
	TInt tgtX = 648;
	TInt tgtY = 486;
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0, 0), EColor256));
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	tgtBmp->Resize(TSize(tgtX, tgtY));
	activeListener->InitialiseActiveListener();
	//Enable Minimum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	iTgtSize = tgtBmp->SizeInPixels();
	if(iTgtSize != TSize(tgtX, tgtY))
		{
		INFO_PRINTF3(_L("Expected output is (160, 120) But Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		CleanupStack::PopAndDestroy(4, activeListener);//[ scaler, tgtBmp, srcBmp, activeListener ]
		return EFail;
		}
	CFbsBitmap* redBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( redBmp );
	User::LeaveIfError( redBmp->Create(TSize( tgtX, tgtY ), EColor256));
	//[ set the pixles of oneByN to red]
	FillBitmapL( *redBmp, red );	
	// compare with red bitmap
	iTestStepResult = CheckBitmaps(*tgtBmp, *redBmp, -1 );	
	CleanupStack::PopAndDestroy(5, activeListener);//[ scaler, tgtBmp, srcBmp, redbmp, activeListener ]
	return iTestStepResult;
	}
	
/**
CTestStepScaleWhiteFillMiniQuality constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFill64K::CTestStepScaleWhiteFill64K()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// ""
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-203-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleWhiteFillMiniQuality
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleWhiteFill64K::~CTestStepScaleWhiteFill64K()
	{
	}

/**
Do the test step.
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScaleWhiteFill64K::DoTestStepL( )
	{
	iTestStepResult = EPass;
	INFO_PRINTF1(_L("This test step scales the bitmap of size(162,122) to (648,486)"));
	INFO_PRINTF1(_L("and checks the whitefill (DEF056170) for Medium Quality Algorithm "));	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	TInt srcX = 162;
	TInt srcY = 122;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( srcX, srcY ), EColor64K));
	//[ set the pixles of oneByN to red]
	TRgb red = KRgbRed;
	FillBitmapL( *srcBmp, red );
	TInt tgtX = 648;
	TInt tgtY = 486;
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0, 0), EColor64K));
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	tgtBmp->Resize(TSize(tgtX, tgtY));
	activeListener->InitialiseActiveListener();
	//Enable Minimum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	iTgtSize = tgtBmp->SizeInPixels();
	if(iTgtSize != TSize(tgtX, tgtY))
		{
		INFO_PRINTF3(_L("Expected output is (160, 120) But Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		CleanupStack::PopAndDestroy(4, activeListener);//[ scaler, tgtBmp, srcBmp, activeListener ]
		return EFail;
		}
	CFbsBitmap* redBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( redBmp );
	User::LeaveIfError( redBmp->Create(TSize( tgtX, tgtY ), EColor64K));
	//[ set the pixles of oneByN to red]
	FillBitmapL( *redBmp, red );	
	// compare with red bitmap
	iTestStepResult = CheckBitmaps(*tgtBmp, *redBmp, -1 );	
	CleanupStack::PopAndDestroy(5, activeListener);//[ scaler, tgtBmp, srcBmp, redbmp, activeListener ]
	return iTestStepResult;
	}

/**
CTestStepScaleWhiteFillMiniQuality constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleSrcWhiteFill::CTestStepScaleSrcWhiteFill()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// ""
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-204-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleWhiteFillMiniQuality
@lib "TS_BitmapTransforms.lib"
*/
CTestStepScaleSrcWhiteFill::~CTestStepScaleSrcWhiteFill()
	{
	}

/**
Do the test step.
 * This test step scales the bitmap of size(162,122)
 * to (648,486) and checks if the Scalar WhiteFill 
 * (DEF056170)		
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepScaleSrcWhiteFill::DoTestStepL( )
	{
	iTestStepResult = EPass;
	INFO_PRINTF1(_L("This test step scales the bitmap of size(162,122) to (648,486)"));
	INFO_PRINTF1(_L("and checks the whitefill (DEF056170) for Medium Quality Algorithm "));	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	TInt srcX = 162;
	TInt srcY = 122;
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError( srcBmp->Create(TSize( srcX, srcY ), EColor64K));
	//[ set the pixles of oneByN to red]
	TRgb red = KRgbRed;
	FillBitmapL( *srcBmp, red );
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	activeListener->InitialiseActiveListener();
	//Enable Minimum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);
	TInt tgtX = 648;
	TInt tgtY = 486;
	const TSize aDestinationSize(tgtX,tgtY);
	scaler->Scale(&activeListener->iStatus, *srcBmp, aDestinationSize , EFalse);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	iSrcSize = srcBmp->SizeInPixels();
	if(iSrcSize != TSize(tgtX, tgtY))
		{
		INFO_PRINTF3(_L("Expected output is (160, 120) But Actual output is(%d, %d)"), iTgtSize.iWidth, iTgtSize.iHeight );		
		CleanupStack::PopAndDestroy(5, activeListener);//[ scaler, tgtBmp, srcBmp, redbmp, activeListener ]	
		return EFail;
		}	
	CFbsBitmap* redBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( redBmp );
	User::LeaveIfError( redBmp->Create(TSize( tgtX, tgtY ), EColor64K));
	//[ set the pixles of oneByN to red]
	FillBitmapL( *redBmp, red );	
	// compare with red bitmap
	iTestStepResult = CheckBitmaps(*srcBmp, *redBmp, -1 );	
	CleanupStack::PopAndDestroy(4, activeListener);//[ scaler, srcBmp, redbmp, activeListener ]
	return iTestStepResult;
	}
	
		
// INC057208 :: NTT - CBitMapScaler::Scale panics with USER 21

/**
CTestStepDEFScale constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestStepDEFScale::CTestStepDEFScale(const TText*  aTestName)
	{
	// store the name of this test case
	iTestStepName = aTestName;
	}

/**
destructor CTestStepDEFScale
@lib "TS_BitmapTransforms.lib"
*/
CTestStepDEFScale::~CTestStepDEFScale()
	{
	
	}

/**
Do the test step.
This test checks scaling of Bitmap without preserving AspectRatio. 
It scales a bitmap from (W,H) to (W*2, H/2)
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepDEFScale::DoTestStepL( )
	{
	// Printing to the console and log file
	_LIT( KTestScale, "Test scaling without preserving AspectRatio");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test checks scaling of Bitmap without preserving AspectRatio.");
	Log( KTestStepScale );
	
	_LIT(KImageTestSourceFileName, "320x240_c24.mbm"); 

	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);


	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(	srcBmp->Load( FileName()) );
	iSrcSize = srcBmp->SizeInPixels();
	
		
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M)); 
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	if(iTestStepName == _L("MM-ICL-BTRANS-U-205-HP"))
		tgtBmp->Resize(TSize(iSrcSize.iWidth*2 ,iSrcSize.iHeight/2 ));
	else if(iTestStepName == _L("MM-ICL-BTRANS-U-206-HP"))
		tgtBmp->Resize(TSize(iSrcSize.iWidth/2 ,iSrcSize.iHeight*2 ));
	else if(iTestStepName == _L("MM-ICL-BTRANS-U-207-HP"))
		tgtBmp->Resize(TSize(iSrcSize.iWidth ,iSrcSize.iHeight*2 ));
	else if(iTestStepName == _L("MM-ICL-BTRANS-U-208-HP"))
		tgtBmp->Resize(TSize(iSrcSize.iWidth*2 ,iSrcSize.iHeight ));
	else if(iTestStepName == _L("MM-ICL-BTRANS-U-209-HP"))
		tgtBmp->Resize(TSize(iSrcSize.iWidth ,iSrcSize.iHeight/2 ));
	else if(iTestStepName == _L("MM-ICL-BTRANS-U-210-HP"))
		tgtBmp->Resize(TSize(iSrcSize.iWidth/2 ,iSrcSize.iHeight ));
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*tgtBmp,EFalse);
		
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		User::Leave(activeListener->iStatus.Int());
	
	//[ scaler, tgtBmp, srcBmp, activeListener ]
	CleanupStack::PopAndDestroy(4, activeListener);

	return iTestStepResult;
	
	}


// DEF066157 :: Bitmap scaler produces wrong results at quality setting other than High

/**
CTestStepLowQtyScale constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestStepLowQtyScale::CTestStepLowQtyScale(const TText*  aTestName)
	{
	// store the name of this test case
	iTestStepName = aTestName;
	}

/**
destructor CTestStepLowQtyScale
@lib "TS_BitmapTransforms.lib"
*/
CTestStepLowQtyScale::~CTestStepLowQtyScale()
	{
	
	}

/**
Do the test step.
This test checks scaling of Bitmap with Quality other than High. 
For a particular Image under discussion, Only High quality scaled image to 64x64 
image is correct, and low is not correct.
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepLowQtyScale::DoTestStepL( )
	{
	// Printing to the console and log file
	_LIT( KTestScale, "Test Checks Scaling of Bitmap with Quality other than High.");
	Log( KTestScale );
	
	_LIT(KImageTestSourceFileName, "png24bpp_1_mask_1.mbm");
		
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);
	
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(refBmp);
		
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	srcBmp->Load(FileName());
				
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EGray256)); 
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	TSize OrgSize(srcBmp->SizeInPixels());
	TSize DestSize(TSize(64, 64));
	
	tgtBmp->Resize(DestSize);
	// For Low Quality
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, ETrue);
		
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		User::Leave(activeListener->iStatus.Int());
	
	refBmp->Load(_L("c:\\Btrans\\Ref_Min_png24bpp_mask.mbm"));
	iTestStepResult = CheckBitmaps( *tgtBmp, *refBmp, -1 );
	if(iTestStepResult == EFail)	
		{
		INFO_PRINTF1(_L("Minimum Quality Images are Not Identical"));	
		return iTestStepResult;
		}
	INFO_PRINTF1(_L("Minimum Quality Images are Identical"));	
	
	// For Medium Quality
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, ETrue);
		
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		User::Leave(activeListener->iStatus.Int());
	
	refBmp->Load(_L("c:\\Btrans\\Ref_Med_png24bpp_mask.mbm"));
	iTestStepResult = CheckBitmaps( *tgtBmp, *refBmp, -1 );
	if(iTestStepResult == EFail)	
		{
		INFO_PRINTF1(_L("Medium Quality Images are Not Identical"));	
		return iTestStepResult;
		}
	INFO_PRINTF1(_L("Medium Quality Images are Identical"));	
	
	// For Max quality
	scaler->SetQualityAlgorithm(CBitmapScaler::EMaximumQuality);
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, ETrue);
		
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		User::Leave(activeListener->iStatus.Int());
	
	refBmp->Load(_L("c:\\Btrans\\Ref_Max_png24bpp_mask.mbm"));
	iTestStepResult = CheckBitmaps( *tgtBmp, *refBmp, -1 );
	if(iTestStepResult == EFail)	
		{
		INFO_PRINTF1(_L("Maximum Quality Images are Not Identical"));
		return iTestStepResult;
		}
	INFO_PRINTF1(_L("Maximum Quality Images are the Identical"));	
			
	//[ scaler, tgtBmp, srcBmp, refbmp, activeListener ]
	CleanupStack::PopAndDestroy(5, activeListener);
	return iTestStepResult;
	
	}

/**
CTestBitmapScaleMaxMedMinSpeed constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/

CTestBitmapScaleMaxMedMinSpeed::CTestBitmapScaleMaxMedMinSpeed(const TText*  aTestName)
	{
	// store the name of this test case
	iTestStepName = aTestName;
	}

/**
destructor CTestBitmapScaleMaxMedMinSpeed
@lib "TS_BitmapTransforms.lib"
*/
CTestBitmapScaleMaxMedMinSpeed::~CTestBitmapScaleMaxMedMinSpeed()
	{
	
	}

/**
Do the test step.
Scale a bitmap up by 2 to the target bitmap with MaximumQuality Algorithm enabled. 
Then Enable MediumQuality Algorithm, MinimumQuality Algorithm and scale the image by 2 to itself. Check the time.
INC079309: Bitmap scaling performance may be increased by 36%
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestBitmapScaleMaxMedMinSpeed::DoTestStepL( )
	{
	//Scale a bitmap up by 2 to the target bitmap with UseMaximumQuality Algorithm enabled. Then Enable MediumQuality and MinimumQuality  Algorithm and scale the image by 2 to itself. Check the time
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
	TInt srcX = 576;
	TInt srcY = 432;	
	//Create source bitmap of size, say, 576 x 432
	User::LeaveIfError(srcBmp->Create(TSize(576, 432), EColor16M));
	
	//Create target bitmap twice the size of source bitmap i.e., 1152 x 864
	User::LeaveIfError(tgtBmp->Create(TSize(srcX * 2 , srcY * 2 ), EColor16M));
	
	//Create a reference bitmap of size 1152 x 864
	User::LeaveIfError(refBmp->Create(TSize(srcX * 2 , srcY * 2), EColor16M));
		
	//Set the pixles of source to red
	TRgb red = KRgbRed;
	TRgb blue = KRgbBlue;
	TRgb green = KRgbGreen;
	TRgb yellow = KRgbYellow;
	FillBitmapL( *srcBmp, red );
	FillBitmapL( *refBmp, red );
	//Set the pixles of target bitmap to blue
	FillBitmapL( *tgtBmp, blue );

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
		//scaler, tgtBmp, refBmp, srcBmp, activeListener
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}
	
	TTime endTime;
	endTime.HomeTime();
	//Check the bitmap against the original
	iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
	if( iTestStepResult != KErrNone)
		{
		//scaler, tgtBmp, refBmp, srcBmp, activeListener
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}

	TInt64 TimeTakenMaxQl = endTime.Int64() - startTime.Int64();		
	INFO_PRINTF4(_L("Start = %Lu End = %Lu Time to scale using Maximum Quality Algorithm = %Lu"),
	  	startTime.Int64(),endTime.Int64(), TimeTakenMaxQl);
   
	//Set the pixles of target bitmap to green
	FillBitmapL( *tgtBmp, green );

	//Enable Medium Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);

	activeListener->InitialiseActiveListener();
	
	//Scale using UseMediumQuality Algorithm to the target size 1152 x 864 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	startTime.HomeTime();
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		}
		
	endTime.HomeTime();
	
	//Check the bitmap against the original
	iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
	if( iTestStepResult != KErrNone)
		{
		//scaler, tgtBmp, refBmp, srcBmp, activeListener
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}
	
	TInt64 TimeTakenMedQl = endTime.Int64() - startTime.Int64();
	INFO_PRINTF4(_L("Start = %Lu End = %Lu Time to scale using Medium Quality Algorithm = %Lu"),
	  	startTime.Int64(),endTime.Int64(), TimeTakenMedQl);
	
	//Set the pixles of target bitmap to yellow
	FillBitmapL( *tgtBmp, yellow );
	//Enable Minimum Quality Algorithm
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);

	activeListener->InitialiseActiveListener();
	
	
	//Scale using UseMinimumQuality Algorithm to the target size 1152 x 864 and check the time taken to scale
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);
	
	startTime.HomeTime();
	
	CActiveScheduler::Start();
	status = activeListener->iStatus.Int();
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		iTestStepResult = EFail;
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}
		
	endTime.HomeTime();
	
	//Check the bitmap against the original
	iTestStepResult = CheckBitmaps(*tgtBmp, *refBmp, -1);
	if( iTestStepResult != KErrNone)
		{
		//scaler, tgtBmp, refBmp, srcBmp, activeListener
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}

	TInt64 TimeTakenMinQl = endTime.Int64() - startTime.Int64();	
	INFO_PRINTF4(_L("Start = %Lu End = %Lu Time to scale using Minimum Quality Algorithm = %Lu"),
	  	startTime.Int64(),endTime.Int64(), TimeTakenMinQl);
			
	if(TimeTakenMedQl <= TimeTakenMaxQl )		
		{
		Log(_L("Time taken using Medium Quality Algorithm is less than Maximum Quality Algorithm."));
		}
	else
		{
		Log(_L("Time taken using Medium Quality Algorithm is NOT less than Maximum Quality Algorithm."));
		iTestStepResult = EFail;
		//scaler, tgtBmp, refBmp, srcBmp, activeListener
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}
	if(TimeTakenMinQl <= TimeTakenMedQl)
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


/**
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestBitmapScalerOverFlow::CTestBitmapScalerOverFlow(const TText*  aTestName)
	{
	// store the name of this test case
	iTestStepName = aTestName;
	iHeapSize = 10000000; 
	}

/**
destructor CTestBitmapScalerOverFlow
@lib "TS_BitmapTransforms.lib"
*/
CTestBitmapScalerOverFlow::~CTestBitmapScalerOverFlow()
	{
	
	}

/**
Do the test step.
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestBitmapScalerOverFlow::DoTestStepL()
	{
		
	CActiveListener* activeListener = new(ELeave) CActiveListener;
	CleanupStack::PushL( activeListener );	
	
	CFbsBitmap* srcBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( srcBmp );
	User::LeaveIfError(srcBmp->Create(TSize(7333,1), EGray256));
	
	CFbsBitmap* tgtBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL( tgtBmp );
	User::LeaveIfError(tgtBmp->Create(TSize(44118,1), EGray256));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL( scaler );
	
	activeListener->InitialiseActiveListener();
	
	scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp, EFalse);	
		
	CActiveScheduler::Start();
	TInt status = activeListener->iStatus.Int();
	
	if (status != KErrNone)
		{
		INFO_PRINTF2(_L("Bitmap could not be scaled. Status =  %d"), status);
		if (status == KErrOverflow)
			{
			iTestStepResult = EPass;	
			INFO_PRINTF1(_L("Bitmap overflow "));
			}
		else
			{
			iTestStepResult = EFail;	
			}
		//scaler, tgtBmp, refBmp, srcBmp, activeListener
		CleanupStack::PopAndDestroy(4,activeListener);
		return iTestStepResult;
		}			
	else
	   {
		iTestStepResult = EFail;
		INFO_PRINTF2(_L("Expected error is Bitmap Overflow but the error is %d"),status);
		CleanupStack :: PopAndDestroy(4,activeListener);	   	
	   }
	
	return iTestStepResult;
	}


/**
CTestStepScaleMemoryLeak constructor.
Each test step initialises its own name.
*/
CTestStepScaleMemoryLeak::CTestStepScaleMemoryLeak()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	// ""
	_LIT( KScaleExample,"MM-ICL-BTRANS-U-305-HP" );
	iTestStepName = KScaleExample ;
	}

/**
destructor CTestStepScaleMemoryLeak
*/
CTestStepScaleMemoryLeak::~CTestStepScaleMemoryLeak()
	{
	}

/**
Do the test step.
Check scale allocations when an invalid target size produces KErrArgument,
or an 0, 0 target size causes an early completion which may cause memory leak.
(PDEF113923)		
@return	"enum TVerdict"
The result of the test step
*/
TVerdict CTestStepScaleMemoryLeak::DoTestStepL( )
	{
	TVerdict  result     = EPass;
	
	INFO_PRINTF1(_L("This test step check scale allocations when an invalid target size produces KErrArgument,"));
	INFO_PRINTF1(_L("or an 0, 0 target size causes an early completion,"));
	INFO_PRINTF1(_L("which may cause memory leak (PDEF113923)"));	
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	//[ Create an bitmap scaler ]
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	//[ push the scaler on the stack ]
	CleanupStack::PushL(scaler);
	
	//[ Create an source bitmap ]
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	//[ push the bitmap on the stack ]
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(640,480),EColor16M));
		
	// check Scale allocations when an invalid target size produces KErrArgument
	activeListener->InitialiseActiveListener();
	__MM_HEAP_MARK;
	scaler->Scale(&activeListener->iStatus, *srcBmp, TSize(-1, -1));
	CActiveScheduler::Start();
	__MM_HEAP_MARKEND;

	// check Scale allocations when an 0, 0 target size causes an early completion
	activeListener->InitialiseActiveListener();
	__MM_HEAP_MARK;
	scaler->Scale(&activeListener->iStatus, *srcBmp, TSize(0, 0));
	CActiveScheduler::Start();
	__MM_HEAP_MARKEND;
	
	CleanupStack::PopAndDestroy(3, activeListener);//[ activeListener, scaler, srcBmp ]
	
	RFbsSession::Disconnect();
	
	return result;	
	}
