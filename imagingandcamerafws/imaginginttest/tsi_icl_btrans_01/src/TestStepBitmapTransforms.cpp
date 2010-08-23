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

#include "ICLFbsSessionTest.h"
//_LIT(KImageTestSourceScaledFileName,"ref\\scaled");
//_LIT(KImageTestRefExt,".mbm");

/*
CActiveListener
*/
CActiveListener::CActiveListener()
 : CActive(CActive::EPriorityIdle)
	{
	CActiveScheduler::Add(this);
	}

/*
@xxxx
InitialiseActiveListener
*/

void CActiveListener::InitialiseActiveListener()
	{ 
	iStatus = KRequestPending; 
	SetActive(); 
	}

/*
@xxxx
RunL
*/
void CActiveListener::RunL()
	{ 
	CActiveScheduler::Stop(); 
	}

/*
@xxxx
  DoCancel()
  */
void CActiveListener::DoCancel()
	{
	}


/**
*
* CTestStepBitmapTransforms constructor 
*
* 
*/
CTestStepBitmapTransforms::CTestStepBitmapTransforms(TBool aAllocTest) 
:iAllocTest(aAllocTest),  
iBitmapTransformsSuite(NULL),
iSourceFileName(),
iScheduler( NULL )
	{
	}

/**
*
* CTestStepBitmapTransforms destructor
*
* 
*/
CTestStepBitmapTransforms::~CTestStepBitmapTransforms()
	{
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTestSuiteBitmapTransforms
* @lib "TSI_ICL_BTRANS_01.lib"
*/

void CTestStepBitmapTransforms::SetSuite( CTestSuiteBitmapTransforms* aPtr )
	{
	iBitmapTransformsSuite = aPtr;
	}


/**
DoTestStepPreambleL is the preamble function for test steps
It creates an active scheduler and installs it. This is necessary
since we are multithreading and each test step runs in a separate thread.
@returns TVerdict which is either EPass or Efail
 
The function can leave if there is insufficient memeory

@lib "TSI_ICL_BTRANS_01.lib"
*/
TVerdict CTestStepBitmapTransforms::DoTestStepPreambleL()
	{
	if(!iAllocTest)
	{
     User::LeaveIfError(FbsStartup());
     TInt errCode = RFbsSession::Connect();
     if( errCode != KErrNone)
         {
         //[ log failed to connect to bitmap server ]
         Log(_L("Failed to connect to bitmap server in teststep preamble = %d"), errCode );
         return EFail;
         }
	 }
	// [Test if this build support alloc testing]
	if(iAllocTest)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, 1);
		TAny *testAlloc = User::Alloc(1);
		if (testAlloc!=NULL)
			{
			delete testAlloc; testAlloc=NULL;
			Log(_L("This build does not support alloc testing"));
			return EInconclusive;
			}
		}

	iScheduler = new(ELeave)CActiveScheduler;
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
		
	// [Run the test step setup]
	Setup();

	return EPass;
	}

/**
Run the test step normally or as an alloc failure test depending
on the test selected. 

For alloc tests:
The test step is expected to leave with KErrNoMemory or KErrNone (when completed).
If the test return KErrNone the test step result is expected to be a pass,
and an test alloc is done to verify all allocations in the teststep has completed.

@lib "TSI_ICL_BTRANS_01.lib"
*/
TVerdict CTestStepBitmapTransforms::DoTestStepL()
	{
	if(iAllocTest)
		{ // Alloc test
		TInt err;
		TInt failCount = 1;
		TBool completed = EFalse;
		TBool badResult = EFalse; 
		TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
		for (;;)
			{
			__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
			__MM_HEAP_MARK;

			TRAP(err,iTestStepResult = TestStepWithFbsSessionL());

			completed = EFalse;
			if (err == KErrNone)
				{
				TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
				if (testAlloc==NULL)
					{
					reachedEnd = ETrue;
					failCount -= 1;
					}
				else
					User::Free(testAlloc);

				if(iTestStepResult != EPass)
					badResult = ETrue;

				completed = reachedEnd || badResult;
				}
			else if (err != KErrNoMemory) // bad error code
				{
				__UHEAP_SETFAIL(RHeap::ENone, 0); // avoid problem in ERR_PRINTF
				completed = ETrue;
				Log(_L("Bad error code %d"),err);
				iTestStepResult = EFail;
				}

			__MM_HEAP_MARKEND;
			__UHEAP_SETFAIL(RHeap::ENone, 0);

			if (completed)
				break; // exit loop

			failCount += 1;
			}
		Log(_L("Finish with %d allocation tests"),failCount);
		}
	else // Normal test
		iTestStepResult = TestStepWithFbsSessionL();

	return iTestStepResult;
	}

/*
Designed to be called from DoTestStepL, wraps FbsSession::Connect()/Disconnect() around the
TestStepL() call
*/
TVerdict CTestStepBitmapTransforms::TestStepWithFbsSessionL()
	{
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict result = TestStepL();
	CleanupStack::PopAndDestroy(fbsSession);
	return result;
	}

/**
The post amble step cleans up the allocated memory in the preamble
@returns TVerdict EPass or EFail
@leaves can leave due to insufficient memory
@lib "TSI_ICL_BTRANS_01.lib"
*/

TVerdict CTestStepBitmapTransforms::DoTestStepPostambleL()
	{
	
	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	return EPass;
	}


/**
*
* CTestStepBitmapTransforms Suite accessor function
* @returns CTestSuiteBitmapTransforms*	
* @lib "TSI_ICL_BTRANS_01.lib"
*/

CTestSuiteBitmapTransforms* CTestStepBitmapTransforms::Suite()
	{
	return iBitmapTransformsSuite;
	}


/**
*
* CTestStepBitmapTransforms SetTestFileName
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&	afull file and path name
* @returns void
* @lib "TSI_ICL_BTRANS_01.lib"
*/

void CTestStepBitmapTransforms::SetTestFileNameL( const TDesC& aPathNameAndExtn )
	{
	//[ append the filename relative to the default path ]
	TFileName fileName = iBitmapTransformsSuite->DefaultPath();
	fileName.Append(aPathNameAndExtn);
	iSourceFileName = fileName ;
	}


/**
* CTestStepBitmapTransforms FillBitmapL
* This function fills a bitmap with a rgb value
* @param srcBmp the source bitmap
* @param rgb	the rgb colour to fill the bitmap with
* @returns void
* @lib "TSI_ICL_BTRANS_01.lib"
*/

void CTestStepBitmapTransforms::FillBitmapL( CFbsBitmap& srcBmp, const TRgb& rgb )
	{
	CFbsBitmapDevice* device = NULL;
	
	// [ create a bitmap device ]
	device = CFbsBitmapDevice::NewL( &srcBmp );
	CleanupStack::PushL( device );

	// [ create a device context ]
	CFbsBitGc* context = NULL;	
	User::LeaveIfError(device->CreateContext( context ));
	CleanupStack::PushL( context );
	
	//[ flood fill the bitmap with rgb ]
	context->SetBrushColor( rgb );
	context->Clear();
	
	//[ pop device, context ]
	CleanupStack::PopAndDestroy(2); 
	}


/**
*
* CTestStepBitmapTransforms CheckBitmapL
*
* Checks a bitmap against the current filename 
* @param aBitmap a reference to a bitmap 
* @leaves memory allocation failure
* @leaves failure to load the current file
* @returns void
* @lib "TSI_ICL_BTRANS_01.lib"
*/

void CTestStepBitmapTransforms::CheckBitmapL(CFbsBitmap& aBitmap )
	{
	
	CFbsBitmap *referenceScaledBitmap = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(referenceScaledBitmap);
	
	// [ load the current bitmap pointed to by filename into memory]
	User::LeaveIfError(referenceScaledBitmap->Load( FileName()));
	
	//[ check against the supplied bitmap ]
	CheckBitmaps(aBitmap,*referenceScaledBitmap, 0);
	
	//[ pop referenceScaledBitmap]
	CleanupStack::PopAndDestroy( referenceScaledBitmap ); 
	
	}


/**
*
* CTestStepBitmapTransforms CheckBitmaps
* This function checks that two bitmaps are equivalent
* where the one has been derived from the other by a rotate/mirror transformation
*@param aBitmap1 first bitmap for comparison
*@param aBitmap2 secon bitmap for comparison
*@param aQuarterTurns determines the transfromation for equivalence
*		This can be -1 for 1 to 1 comparison or the enumeration
*		CBitmapRotator::
*@returns TVerdict which is either EPass or EFail. The bitmaps are equivalent if
*		  the result is Epass
*/

TVerdict CTestStepBitmapTransforms::CheckBitmaps(CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2,TInt aQuarterTurns)
	{
	
	const TSize Bitmap1Size = aBitmap1.SizeInPixels();
	const TSize Bitmap2Size = aBitmap2.SizeInPixels();
	if (aQuarterTurns == -1)
		{
		//confirm sizes are consistent
		if((Bitmap1Size.iHeight != Bitmap2Size.iHeight) || (Bitmap1Size.iWidth != Bitmap2Size.iWidth))
			{
			INFO_PRINTF1(_L("Bitmap size mismatch"));
			return EFail;
			}

		for (TInt y = 0; y < Bitmap1Size.iHeight; y++)
			{
			for (TInt x = 0; x < Bitmap1Size.iWidth; x++)
				{
				TPoint pt(x,y);
				TRgb color1,color2;
				aBitmap1.GetPixel(color1,pt);
				aBitmap2.GetPixel(color2,pt);
				
				if (color1 != color2)
					{
					INFO_PRINTF3(_L("Bitmap color mismatch  src bitmap point %d, %d"), x, y);
					return EFail;
					}
				}
			}
		}
	else
		{
		//confirm sizes are consistent
		switch (aQuarterTurns)
			{
		case CBitmapRotator::ERotation90DegreesClockwise:
		case CBitmapRotator::ERotation270DegreesClockwise:
			if((Bitmap1Size.iHeight != Bitmap2Size.iWidth) || (Bitmap1Size.iWidth != Bitmap2Size.iHeight))
				{
				INFO_PRINTF1(_L("Bitmap size mismatch"));
				return EFail;
				}
			break;

		case CBitmapRotator::ERotation180DegreesClockwise:
		case CBitmapRotator::EMirrorHorizontalAxis:
		case CBitmapRotator::EMirrorVerticalAxis:
			if((Bitmap1Size.iHeight != Bitmap2Size.iHeight) || (Bitmap1Size.iWidth != Bitmap2Size.iWidth))
				{
				INFO_PRINTF1(_L("Bitmap size mismatch"));
				return EFail;
				}
			break;
		default:
			// Should not get here !
			ASSERT( EFalse );
			}



		for (TInt y = 0; y < Bitmap1Size.iHeight; y++)
			{
			for (TInt x = 0; x < Bitmap1Size.iWidth; x++)
				{
				TPoint pt(x,y);
				TRgb color1,color2;
				aBitmap1.GetPixel(color1,pt);
				switch (aQuarterTurns)
					{
				case CBitmapRotator::ERotation90DegreesClockwise:
					aBitmap2.GetPixel(color2,TPoint(Bitmap1Size.iHeight - y - 1,x));
					break;
				case CBitmapRotator::ERotation180DegreesClockwise:
					aBitmap2.GetPixel(color2,TPoint(Bitmap1Size.iWidth - x - 1,Bitmap1Size.iHeight - y - 1));
					break;
				case CBitmapRotator::ERotation270DegreesClockwise:
					aBitmap2.GetPixel(color2,TPoint(y,Bitmap1Size.iWidth - x - 1));
					break;
				case CBitmapRotator::EMirrorHorizontalAxis:
					aBitmap2.GetPixel(color2,TPoint(x, Bitmap1Size.iHeight - y - 1));
					break;
				case CBitmapRotator::EMirrorVerticalAxis:
					aBitmap2.GetPixel(color2,TPoint( Bitmap1Size.iWidth - x - 1, y));
					break;
				default:
					// Should not get here !
					ASSERT( EFalse );
					}
				
				
				if (color1 != color2)
					{
					INFO_PRINTF3(_L("Bitmap color mismatch  src bitmap point %d, %d"), x, y);
					return EFail;
					}
				}
			}
		}

	return EPass;
	}


/**
*
* CTestStepBitmapTransforms GetFileName
*
* 
* 
*/
TFileName& CTestStepBitmapTransforms::FileName() 
	{
	return	iSourceFileName;
	}


/**
*
* CTestStepBitmapTransforms CheckBitmapL
*
* Checks a bitmap against the current filename 
* @param aSourceFileName A file containing the bitmap to rotate.
* @param aAngle The angle to rotate or mirror the btmap.
* @param KeepOriginalBitmap Tru if the test is to copy from one bitmap to another.
* @returns TVerdict which is either EPass or EFail. The rotation/mirror has 
*		  been successfull if if operation completes and bitmap has the correct rgb values.
*
* @leaves memory allocation failure
* @leaves failure to load the current file
* @lib "TSI_ICL_BTRANS_01.lib"
*/



TVerdict CTestStepBitmapTransforms::RotateAndMirrorABitmapL( const TFileName& aSourceFileName, CBitmapRotator::TRotationAngle aAngle, bool KeepOriginalBitmap)
	{
	TVerdict  result = EFail ;
	
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
	if(KeepOriginalBitmap)
		User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
	else
		User::LeaveIfError(tgtBmp->Load( FileName() ));

	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	//[ rotate or mirror the bitmap through the requisite angle ]
	activeListener->InitialiseActiveListener();
	if(KeepOriginalBitmap)
		rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp, aAngle );
	else
		rotator->Rotate(&activeListener->iStatus,*tgtBmp, aAngle );

	CActiveScheduler::Start();

	const TInt status = activeListener->iStatus.Int();
	if(status != KErrNone)
		User::Leave(status);
	
	//[ check the bitmap against the original]
	result = CheckBitmaps(*srcBmp,*tgtBmp, aAngle );	
	
	//[ Pop and Destroy activeListener, srcBmp, tgtBmp, rotator]
	CleanupStack::PopAndDestroy(4);  
	
	return result;
	}


_LIT(KIclPanicCategory, "TS_BitmapTransforms");

/** 
 This function raises a panic

@param TS_BitmapTransformsMain::TS_BitmapTransformsPanic one of the several panics codes
	   that may be raised by this dll
@panic	ENoSourceBitmap is raised when the src bitmap supplied to one of the api's
		has not been created ie has a null handle

@xxxx
*/

GLDEF_C void Panic(TS_BitmapTransformsPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

/*
* 
* Quality Algorithm display function
* 
* @param	CBitmapScaler::TQualityAlgorithm aQualityAlgo
*   		The algorithm that is used for scaling.
* 
*/
void CTestStepBitmapTransforms::DisplayQualityAlgo(CBitmapScaler::TQualityAlgorithm aQualityAlgo)
    {
    switch (aQualityAlgo)
		{
		case CBitmapScaler::EMaximumQuality:
			Log(_L("Quality Algorithm : EMaximumQuality"));
			break;

		case CBitmapScaler::EMediumQuality:
			Log(_L("Quality Algorithm : EMedimumQuality"));
			break;

		case CBitmapScaler::EMinimumQuality:
			Log(_L("Quality Algorithm : EMinimumQuality"));
			break;

		default:
			Log(_L("Invalid Quality Algorithm"));
	    }    
    }
    
    
