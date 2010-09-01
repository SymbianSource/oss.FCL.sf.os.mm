// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestPostProcessScale.h"
#include "TestSuiteBitmapTransforms.h"
#include <bitmaptransforms.h>

//[ Codec Unit tests  structure 
// The unit tests shall use text files
// for small portions of test data 
// which should be stored are stored in a simple format
// containing the relevant parameters for the test
//]
class TScaleBitmapUnitTestParams
	{	
public:
	const TText*  iTestName;              // name of the test
	const TText*  iInputFilename;         // input bitmap
	const TText*  iScaleDownFileName;     // output Scale up Bitmap
	const TText*  iScaleUpFileName;       // output Scale Down Bitmap
	TBool         iDisablePostProcessing; // Post Processing Enbaled 
	TInt          iExpectedResult;        // test result
	TDisplayMode  iBitmapType;            // bitmap type
	};

// constant table of parameters for tests
const TScaleBitmapUnitTestParams KTestParameters[] =
{	
	{
	_S("MM-ICL-BTRANS-U-041-HP"),   
	_S("c:\\EGray2IclBtrans.mbm"),
	_S("c:\\btrans\\EGray2IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray2IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray2
	},

	{ 
	_S("MM-ICL-BTRANS-U-042-HP"),   
	_S("c:\\EGray4IclBtrans.mbm"),
	_S("c:\\btrans\\EGray4IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray4IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray4
	},

	{ 
	_S("MM-ICL-BTRANS-U-043-HP"),   
	_S("c:\\EGray16IclBtrans.mbm"),
	_S("c:\\btrans\\EGray16IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray16IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray16
	},

	{ 
	_S("MM-ICL-BTRANS-U-044-HP"),   
	_S("c:\\EGray256IclBtrans.mbm"),
	_S("c:\\btrans\\EGray256IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray256IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray256
	},

	{ 
	_S("MM-ICL-BTRANS-U-045-HP"),   
	_S("c:\\EColor16IclBtrans.mbm"),
	_S("c:\\btrans\\EColor16IclBtransDown.mbm"),
	_S("c:\\btrans\\EColor16IclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor16
	},

	{ 
	_S("MM-ICL-BTRANS-U-046-HP"),   
	_S("c:\\EColor256IclBtrans.mbm"),
	_S("c:\\btrans\\EColor256IclBtransDown.mbm"),
	_S("c:\\btrans\\EColor256IclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor256
	},

	{ 
	_S("MM-ICL-BTRANS-U-047-HP"),   
	_S("c:\\EColor64KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor64K
	},

	{ 
	_S("MM-ICL-BTRANS-U-048-HP"),   
	_S("c:\\EColor16MIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor16M
	},

	{ 
	_S("MM-ICL-BTRANS-U-049-HP"),   
	_S("c:\\EColor4KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor4K 
	},

	{ 
	_S("MM-ICL-BTRANS-U-050-HP"),   
	_S("c:\\EColor16MUIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor4K   // Reserved for uncompressed format 
	},

	{ 
	_S("MM-ICL-BTRANS-U-051-HP"),   
	_S("c:\\EGray2IclBtrans.mbm"),
	_S("c:\\btrans\\EGray2IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray2IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray2   
	},

	{ 
	_S("MM-ICL-BTRANS-U-052-HP"),   
	_S("c:\\EGray4IclBtrans.mbm"),
	_S("c:\\btrans\\EGray4IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray4IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray4
	},

	{ 
	_S("MM-ICL-BTRANS-U-053-HP"),   
	_S("c:\\EGray16IclBtrans.mbm"),
	_S("c:\\btrans\\EGray16IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray16IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray16
	},

	{ 
	_S("MM-ICL-BTRANS-U-054-HP"),   
	_S("c:\\EGray256IclBtrans.mbm"),
	_S("c:\\btrans\\EGray256IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray256IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray256
	},

	{ 
	_S("MM-ICL-BTRANS-U-055-HP"),   
	_S("c:\\EColor16IclBtrans.mbm"),
	_S("c:\\btrans\\EColor16IclBtransUpNp.mbm"),
	_S("c:\\btrans\\EColor16IclBtransDownNp.mbm"),
	ETrue,
	EPass,
	EColor16
	},

	{ 
	_S("MM-ICL-BTRANS-U-056-HP"),   
	_S("c:\\EColor256IclBtrans.mbm"),
	_S("c:\\btrans\\EColor256IclBtransUpNp.mbm"),
	_S("c:\\btrans\\EColor256IclBtransDownNp.mbm"),
	ETrue,
	EPass,
	EColor256
	},

	{ 
	_S("MM-ICL-BTRANS-U-057-HP"),   
	_S("c:\\EColor64KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransUpNp.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransDownNp.mbm"),
	ETrue,
	EPass,
	EColor64K
	},

	{ 
	_S("MM-ICL-BTRANS-U-058-HP"),   
	_S("c:\\EColor16MIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransDownNp.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EColor16M
	},
	
	{ 
	_S("MM-ICL-BTRANS-U-059-HP"),   
	_S("c:\\EColor4KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransScaleDownNP.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransScaleUpNp.mbm"),
	ETrue,
	EPass,
	EColor4K
	},
	
	{ 
	_S("MM-ICL-BTRANS-U-060-HP"),   
	_S("c:\\EColor16MUIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransScaleDownNP.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransScaleUpNP.mbm"),
	ETrue,
	EPass,
	EColor16MU // reserved for 16m uncompressed
	}
};


/**
*
* CTestStepPostProcessScale
*
**/
CTestStepPostProcessScale::CTestStepPostProcessScale( TUint aTestIndex )
	{
	//[precondition valid index ]
	__ASSERT_DEBUG( (aTestIndex < (sizeof(KTestParameters)/sizeof(TScaleBitmapUnitTestParams))),Panic(EBadArgument));
	// store a pointer to the test parameters
	iTestParameters = &(KTestParameters[aTestIndex]);
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = iTestParameters->iTestName;
	}

/**
*
* ~CTestStepPostProcessScale
*
**/
CTestStepPostProcessScale::~CTestStepPostProcessScale()
	{
	}

/**
*
* DoTestStepL
*
**/
TVerdict CTestStepPostProcessScale::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Post Process Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test checks scaling a bitmap");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
    CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	
	TFileName  xx( iTestParameters->iInputFilename );
	User::LeaveIfError( srcBmp->Load(xx));
	const TSize size( srcBmp->SizeInPixels() );
	
	//[ create scale down bitmap]
	CFbsBitmap* scaleBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(scaleBmp);
	User::LeaveIfError(scaleBmp->Create(TSize(0,0), iTestParameters->iBitmapType ));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);

	TBool postProcess = iTestParameters->iDisablePostProcessing;
	TAny* aPtr = &postProcess;
	TInt errCode = scaler->CustomCommand( KICLUidPostProcessCommand, aPtr );
	if( errCode != KErrNone )
		User::Leave( errCode );
	
	scaleBmp->Resize(TSize(size.iWidth*2,size.iHeight*2));
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*scaleBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		User::Leave(activeListener->iStatus.Int());
	
#ifdef __SAVE_FILES
	TFileName scaleUpFileName( iTestParameters->iScaleUpFileName );
	
	//[ save scaled up bitmap to file ]
	User::LeaveIfError( scaleBmp->Save( scaleUpFileName) );
#endif
	
	scaleBmp->Resize(TSize(size.iWidth/2,size.iHeight/2));	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus,*srcBmp,*scaleBmp);
	CActiveScheduler::Start();
	if(activeListener->iStatus!=KErrNone)
		{
		User::Leave(activeListener->iStatus.Int());
		}
	
#ifdef __SAVE_FILES
	TPtrC16 scaleDownFileName( iTestParameters->iScaleDownFileName );
	
	//[ save scaled down bitmap to file ]
	User::LeaveIfError( scaleBmp->Save(scaleDownFileName));
#endif
	
	//[ activeListener,srcBmp, scaleBmp, scaler ]
	CleanupStack::PopAndDestroy( 4, activeListener );
	
	return result;
	}

/**
*
* CTestStepCancelScale
*
**/
CTestStepCancelScale::CTestStepCancelScale()
	{
	iTestStepName = _L( "MM-ICL-BTRANS-U-061-HP");
	}

/**
*
* ~CTestStepCancelScale
*
**/
CTestStepCancelScale::~CTestStepCancelScale()
	{
	// nothing to do
	}

/**
*
* DoTestStepL
*
**/
TVerdict CTestStepCancelScale::DoTestStepL()
	{
	TVerdict result = EPass;
	
    _L( "xfile.mbm");

	const TInt KCancelDelayIncrement=100000; // 100ms
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	
	_LIT( KXFile, "c:\\xfile.mbm");
	User::LeaveIfError(	srcBmp->Load( KXFile ));
	const TSize size(srcBmp->SizeInPixels());
	
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor4K));
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	tgtBmp->Resize(TSize(size.iWidth*2,size.iHeight*3));		
	
	TUint cancelDelay = 0;
	TUint maxDelay = 0;

	for ( ;; )
		{
		//[ Create an active Listener ]
		CActiveScaleListener* activeListener = new(ELeave)CActiveScaleListener;
	
	    //[ push the listener on the stack ]
	    CleanupStack::PushL( activeListener );

		TTimeIntervalMicroSeconds32 tmpTime(cancelDelay);
		CCancelTimer* timer = CCancelTimer::NewLC(tmpTime, activeListener);
		
		activeListener->InitialiseActiveListener(scaler);
		
		scaler->Scale(&activeListener->iStatus, *srcBmp, *tgtBmp);		
		timer->InitializeCancelTimer();
			
		User::After(1);
	    TTime startTime;
		      startTime.UniversalTime();
		CActiveScheduler::Start();
		TTime endTime;
		      endTime.UniversalTime();
		
		TInt scaleStatus = activeListener->iStatus.Int();
		
		// get status
		TBool timerFinished = !timer->IsActive();
		ASSERT(!activeListener->IsActive()); // either scaler finished on its own accord, or timer canceled
		
		// then cancel - so we can leave without issue
		timer->Cancel();

		CleanupStack::PopAndDestroy( 2, activeListener ); 		

		if( !cancelDelay  && !timerFinished )
			{
			User::Leave( KErrCorrupt);
			}

		TUint uSTime = I64INT(endTime.Int64() - startTime.Int64());
		// check that the result came back in an OK time
		if (uSTime < cancelDelay)
            {
			if (timerFinished)
				INFO_PRINTF3(_L("timed delay(%d) was faster than the requested delay(%d)"), uSTime, cancelDelay);
            }
        else
            {
			uSTime -= cancelDelay;
			if (uSTime>maxDelay)
				maxDelay = uSTime;
            }

		// if we cancel'd (ie. timer fired) then we continue with extra delay, otherwise finish or leave with result

		if (timerFinished)
			{
			cancelDelay += KCancelDelayIncrement;
			}
		else
			{
			if (scaleStatus==KErrNone)
				{
				break; // exit loop - normal exit
				}
			else 
				{
				User::Leave(scaleStatus);
				}
			}
		}


	CleanupStack::PopAndDestroy( 3,srcBmp ); 
	
    return result;
   }

/**
*
* NewLC
*
**/
CCancelTimer* CCancelTimer::NewLC(TTimeIntervalMicroSeconds32& aCancelDelay, CActive* aListener)
    {
    CCancelTimer* self = new (ELeave) CCancelTimer(aListener);
    CleanupStack::PushL(self);
    self->ConstructL(aCancelDelay);
    return self;
    }

/**
*
* ConstructL
*
**/
void CCancelTimer::ConstructL(TTimeIntervalMicroSeconds32& aCancelDelay)
    {
    CTimer::ConstructL();

    iCancelDelay = aCancelDelay;
    CActiveScheduler::Add(this);
    }

/**
*
* ~CCancelTimer
*
**/
CCancelTimer::~CCancelTimer()
    {
    Cancel();
    }

/**
*
* InitializeCancelTimer
*
**/
void CCancelTimer::InitializeCancelTimer()
    {
  // Request another wait - assume not active
	CTimer::After(iCancelDelay);
    }

/**
*
* RunL
*
**/
void CCancelTimer::RunL()
    {
    iListener->Cancel();    
    CActiveScheduler::Stop();
    }

/**
*
* DoCancel
*
**/
void CCancelTimer::DoCancel()
	{
	iListener->Cancel();
	}


CCancelTimer::CCancelTimer(CActive* aListener)
	: CTimer(CActive::EPriorityUserInput),
    iListener(aListener)
	{
    }
   
	
