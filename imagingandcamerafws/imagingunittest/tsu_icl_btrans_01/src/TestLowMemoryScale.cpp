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
// EPOC includes
// 
//

#include <e32base.h>

// Test system includes
#include <testframework.h>

#include "TestLowMemoryScale.h"
#include "TestSuiteBitmapTransforms.h"
#include <bitmaptransforms.h>

//[ Bitmap Transforms Unit tests structure 
// The unit tests shall use text files
// for small portions of test data 
// which should be stored in a simple format
// containing the relevant parameters for the test
//]
class TScaleBitmapUnitTestParams
	{	
public:
	const TText*  iTestName;              // name of the test
	const TText*  iInputFilename;         // input bitmap
	const TText*  iScaleDownFileName;     // output Scale up Bitmap
	const TText*  iScaleUpFileName;       // output Scale Down Bitmap
	TBool         iEnableLowMemory;		  // Low memory algorithm enabled 
	TInt          iExpectedResult;        // test result
	TDisplayMode  iBitmapType;            // bitmap type
	};

// constant table of parameters for tests
const TScaleBitmapUnitTestParams KTestParameters[] =
	{
	{
	_S("MM-ICL-BTRANS-U-100-HP"),   
	_S("c:\\EGray2IclBtrans.mbm"),
	_S("c:\\btrans\\EGray2IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray2IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray2
	},

	{ 
	_S("MM-ICL-BTRANS-U-101-HP"),   
	_S("c:\\EGray4IclBtrans.mbm"),
	_S("c:\\btrans\\EGray4IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray4IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray4
	},

	{ 
	_S("MM-ICL-BTRANS-U-102-HP"),   
	_S("c:\\EGray16IclBtrans.mbm"),
	_S("c:\\btrans\\EGray16IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray16IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray16
	},

	{ 
	_S("MM-ICL-BTRANS-U-103-HP"),   
	_S("c:\\EGray256IclBtrans.mbm"),
	_S("c:\\btrans\\EGray256IclBtransDown.mbm"),
	_S("c:\\btrans\\EGray256IclBtransUp.mbm"),
	EFalse,
	EPass,
	EGray256
	},

	{ 
	_S("MM-ICL-BTRANS-U-104-HP"),   
	_S("c:\\EColor16IclBtrans.mbm"),
	_S("c:\\btrans\\EColor16IclBtransDown.mbm"),
	_S("c:\\btrans\\EColor16IclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor16
	},

	{ 
	_S("MM-ICL-BTRANS-U-105-HP"),   
	_S("c:\\EColor256IclBtrans.mbm"),
	_S("c:\\btrans\\EColor256IclBtransDown.mbm"),
	_S("c:\\btrans\\EColor256IclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor256
	},

	{ 
	_S("MM-ICL-BTRANS-U-106-HP"),   
	_S("c:\\EColor64KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor64K
	},

	{ 
	_S("MM-ICL-BTRANS-U-107-HP"),   
	_S("c:\\EColor16MIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor16M
	},

	{ 
	_S("MM-ICL-BTRANS-U-108-HP"),   
	_S("c:\\EColor4KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor4K 
	},

	{ 
	_S("MM-ICL-BTRANS-U-109-HP"),   
	_S("c:\\EColor16MUIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransDown.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransUp.mbm"),
	EFalse,
	EPass,
	EColor16MU
	},

	{ 
	_S("MM-ICL-BTRANS-U-110-HP"),   
	_S("c:\\EGray2IclBtrans.mbm"),
	_S("c:\\btrans\\EGray2IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray2IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray2   
	},

	{ 
	_S("MM-ICL-BTRANS-U-111-HP"),   
	_S("c:\\EGray4IclBtrans.mbm"),
	_S("c:\\btrans\\EGray4IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray4IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray4
	},

	{ 
	_S("MM-ICL-BTRANS-U-112-HP"),   
	_S("c:\\EGray16IclBtrans.mbm"),
	_S("c:\\btrans\\EGray16IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray16IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray16
	},

	{ 
	_S("MM-ICL-BTRANS-U-113-HP"),   
	_S("c:\\EGray256IclBtrans.mbm"),
	_S("c:\\btrans\\EGray256IclBtransDownNp.mbm"),
	_S("c:\\btrans\\EGray256IclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EGray256
	},

	{ 
	_S("MM-ICL-BTRANS-U-114-HP"),   
	_S("c:\\EColor16IclBtrans.mbm"),
	_S("c:\\btrans\\EColor16IclBtransUpNp.mbm"),
	_S("c:\\btrans\\EColor16IclBtransDownNp.mbm"),
	ETrue,
	EPass,
	EColor16
	},

	{ 
	_S("MM-ICL-BTRANS-U-115-HP"),   
	_S("c:\\EColor256IclBtrans.mbm"),
	_S("c:\\btrans\\EColor256IclBtransUpNp.mbm"),
	_S("c:\\btrans\\EColor256IclBtransDownNp.mbm"),
	ETrue,
	EPass,
	EColor256
	},

	{ 
	_S("MM-ICL-BTRANS-U-116-HP"),   
	_S("c:\\EColor64KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransUpNp.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransDownNp.mbm"),
	ETrue,
	EPass,
	EColor64K
	},

	{ 
	_S("MM-ICL-BTRANS-U-117-HP"),   
	_S("c:\\EColor16MIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransDownNp.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransUpNp.mbm"),
	ETrue,
	EPass,
	EColor16M
	},
	
	{ 
	_S("MM-ICL-BTRANS-U-118-HP"),   
	_S("c:\\EColor4KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransScaleDownNP.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransScaleUpNp.mbm"),
	ETrue,
	EPass,
	EColor4K
	},
	
	{ 
	_S("MM-ICL-BTRANS-U-119-HP"),   
	_S("c:\\EColor16MUIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransScaleDownNP.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransScaleUpNP.mbm"),
	ETrue,
	EPass,
	EColor16MU
	}
	};

/*
*
* CTestStepLowMemoryScale
*
*/
CTestStepLowMemoryScale::CTestStepLowMemoryScale( TUint aTestIndex )
	{
	//[precondition valid index ]
	__ASSERT_DEBUG( (aTestIndex < (sizeof(KTestParameters)/sizeof(TScaleBitmapUnitTestParams))),Panic(EBadArgument));
	// store a pointer to the test parameters
	iTestParameters = &(KTestParameters[aTestIndex]);
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = iTestParameters->iTestName;
	}

/*
*
* ~CTestStepLowMemoryScale
*
*/
CTestStepLowMemoryScale::~CTestStepLowMemoryScale()
	{
	}

/*
*
* DoTestStepL
*
*/
TVerdict CTestStepLowMemoryScale::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Low Memory Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "This test checks scaling a bitmap");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	
	TFileName inputFilename( iTestParameters->iInputFilename );
	TInt err = srcBmp->Load(inputFilename);
	User::LeaveIfError(err);
	const TSize size( srcBmp->SizeInPixels() );
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);

	TBool lowMem = iTestParameters->iEnableLowMemory;
	TAny* aPtr = &lowMem;
	TInt errCode = scaler->CustomCommand( KICLUidUseLowMemoryAlgorithmCommand, aPtr );
	if( errCode != KErrNone )
		{
		User::Leave( errCode );
		}

	DoTestScaleL(activeListener, srcBmp, scaler, size, inputFilename, EFalse);
	if (!lowMem)
		{
		// Repeat the scaling tests with an allocation failure
		// to simulate use of the low memory algorithm
		DoTestScaleL(activeListener, srcBmp, scaler, size, inputFilename, ETrue);
		}
	
	//[ activeListener, srcBmp, scaler ]
	//
	CleanupStack::PopAndDestroy( 3, activeListener );

	return result;
	}

/*
*
* DoTestScaleL(
* @param CActiveListener* aActiveListener - the active listener
* @param CFbsBitmap* aSrcBmp - the bitmap to scale
* @param CBitmapScaler* aScaler - the scaler object
* @param const TSize aSize - the size to base our scaling on
* @param TFileName aFileName	- the name of the file we're scaling to
* @param const TBool aFailNext - ETrue if we want to test automatic use of
*					the low memory algorithm but only for the case of scaling a bitmap to a size
*
*/
void CTestStepLowMemoryScale::DoTestScaleL(CActiveListener* aActiveListener,
										   CFbsBitmap* aSrcBmp,
										   CBitmapScaler* aScaler,
										   const TSize aSize,
										   TFileName aFileName,
										   const TBool aFailNext)
	{
	// 
	// Scale up to a given size
	aActiveListener->InitialiseActiveListener();
	if (aFailNext)
		{
		// Force the scaler to fail to create a temporary bitmap
		__UHEAP_FAILNEXT(1);
		}
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth*2,aSize.iHeight*2));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		User::Leave(aActiveListener->iStatus.Int());
		}

	// Scale down to a given size
	aActiveListener->InitialiseActiveListener();
	if (aFailNext)
		{
		// Force the scaler to fail to create a temporary bitmap
		__UHEAP_FAILNEXT(1);
		}
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth/2,aSize.iHeight/2));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		User::Leave(aActiveListener->iStatus.Int());
		}
	
	// These tests are only valid if we're not going to fail the next allocation
	if (!aFailNext)
		{
		// Scale one bitmap to another..
		CFbsBitmap* destBmp = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(destBmp);
		// Scale up
		TFileName upFileName( iTestParameters->iScaleUpFileName );
		User::LeaveIfError( destBmp->Load( upFileName ) );
		
		aActiveListener->InitialiseActiveListener();
		aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *destBmp);
		CActiveScheduler::Start();
		if(aActiveListener->iStatus!=KErrNone)
			{
			User::Leave(aActiveListener->iStatus.Int());
			}

		// Scale down
		aFileName = TFileName( iTestParameters->iScaleDownFileName );
		User::LeaveIfError( destBmp->Load(aFileName));
			
		aActiveListener->InitialiseActiveListener();
		aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *destBmp);
		CActiveScheduler::Start();
		if(aActiveListener->iStatus!=KErrNone)
			{
			User::Leave(aActiveListener->iStatus.Int());
			}
		
		//[ destBmp ]
		//
		CleanupStack::PopAndDestroy(destBmp);
		}
	}
