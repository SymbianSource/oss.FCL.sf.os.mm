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
#include "TestFastScale.h"
#include "TestSuiteBitmapTransforms.h"
#include <bitmaptransforms.h>

const TScaleBitmapUnitTestParams KTestParameters[] =
	{
	{
	_S("c:\\EGray2IclBtrans.mbm"),
	_S("c:\\btrans\\EGray2IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray2IclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EGray2
	},

	{ 
	_S("c:\\EGray4IclBtrans.mbm"),
	_S("c:\\btrans\\EGray4IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray4IclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EGray4
	},

	{ 
	_S("c:\\EGray16IclBtrans.mbm"),
	_S("c:\\btrans\\EGray16IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray16IclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EGray16
	},

	{ 
	_S("c:\\EGray256IclBtrans.mbm"),
	_S("c:\\btrans\\EGray256IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray256IclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EGray256
	},

	{ 
	_S("c:\\EColor16IclBtrans.mbm"),
	_S("c:\\btrans\\EColor16IclBtransUp.mbm"),
	_S("c:\\btrans\\EColor16IclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EColor16
	},

	{ 
	_S("c:\\EColor256IclBtrans.mbm"),
	_S("c:\\btrans\\EColor256IclBtransUp.mbm"),
	_S("c:\\btrans\\EColor256IclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EColor256
	},

	{ 
	_S("c:\\EColor64KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EColor64K
	},

	{ 
	_S("c:\\EColor16MIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EColor16M
	},

	{ 
	_S("c:\\EColor4KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EColor4K 
	},

	{ 
	_S("c:\\EColor16MUIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransDown.mbm"),
	CBitmapScaler::EMediumQuality,
	EColor16MU
	},
	
	{
	_S("c:\\EGray2IclBtrans.mbm"),
	_S("c:\\btrans\\EGray2IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray2IclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EGray2
	},

	{ 
	_S("c:\\EGray4IclBtrans.mbm"),
	_S("c:\\btrans\\EGray4IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray4IclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EGray4
	},

	{ 
	_S("c:\\EGray16IclBtrans.mbm"),
	_S("c:\\btrans\\EGray16IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray16IclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EGray16
	},

	{ 
	_S("c:\\EGray256IclBtrans.mbm"),
	_S("c:\\btrans\\EGray256IclBtransUp.mbm"),
	_S("c:\\btrans\\EGray256IclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EGray256
	},

	{ 
	_S("c:\\EColor16IclBtrans.mbm"),
	_S("c:\\btrans\\EColor16IclBtransUp.mbm"),
	_S("c:\\btrans\\EColor16IclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EColor16
	},

	{ 
	_S("c:\\EColor256IclBtrans.mbm"),
	_S("c:\\btrans\\EColor256IclBtransUp.mbm"),
	_S("c:\\btrans\\EColor256IclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EColor256
	},

	{ 
	_S("c:\\EColor64KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor64KIclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EColor64K
	},

	{ 
	_S("c:\\EColor16MIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor16MIclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EColor16M
	},

	{ 
	_S("c:\\EColor4KIclBtrans.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor4KIclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EColor4K 
	},

	{ 
	_S("c:\\EColor16MUIclBtrans.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransUp.mbm"),
	_S("c:\\btrans\\EColor16MUIclBtransDown.mbm"),
	CBitmapScaler::EMinimumQuality,
	EColor16MU
	}
	};	

/*
*
* CTestStepFastScaleXY
*
*/

CTestStepFastScaleXY::CTestStepFastScaleXY(TUint aTestIndex,const TText*  aTestName)
	{
	//[precondition valid index ]
	__ASSERT_DEBUG( (aTestIndex < (sizeof(KTestParameters)/sizeof(TScaleBitmapUnitTestParams))),Panic(EBadArgument));
	// store a pointer to the test parameters
	iTestParameters = &(KTestParameters[aTestIndex]);
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = aTestName;
	}

/*
*
* ~CTestStepFastScaleXY
*
*/

CTestStepFastScaleXY::~CTestStepFastScaleXY()
	{

	}

/*
*
* DoTestScaleL
* @param aActiveListener - the active listener
* @param aSrcBmp - the bitmap to scale
* @param aScaler - the scaler object
* @param aSize - the size to base our scaling on
* @param aQualityAlgo - EMinimumQuality | EMediumQuality algorithm for Quality of the image scaling.
* @returns TVerdict which is either EPass or EFail.
*/
TVerdict CTestStepFastScaleXY::DoTestScaleL(CActiveListener* aActiveListener,
										   CFbsBitmap* aSrcBmp,
										   CBitmapScaler* aScaler,
										   const TSize aSize,
										   CBitmapScaler::TQualityAlgorithm aQualityAlgo)
	{
	TAny* ptr = &aQualityAlgo;
	TVerdict result = EPass;
	
	// Setting the Quality factor
	TInt errCode = aScaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr );
	if( errCode != KErrNone )
		{
		return EFail;
		}
    
    //Get the reference bitmaps	for scale up and scale down
    TFileName refsrcfilename(iTestParameters->iInputFilename);
	TFileName refupfilename(iTestParameters->iScaleUpFileName);
	TFileName refdownfilename(iTestParameters->iScaleDownFileName);
	if(aQualityAlgo == CBitmapScaler::EMediumQuality)
		{
		refsrcfilename.Insert(3,_L("btrans\\Ref_ScaleXY_Medium"));
		refupfilename.Insert(10,_L("Ref_ScaleXY_Medium"));	
		refdownfilename.Insert(10,_L("Ref_ScaleXY_Medium"));	
		}
	else
		{
		refsrcfilename.Insert(3,_L("btrans\\Ref_ScaleXY_Minimum"));
		refupfilename.Insert(10,_L("Ref_scaleXY_Minimum"));
		refdownfilename.Insert(10,_L("Ref_scaleXY_Minimum"));
		}
		
	// Scale up by 2 to Target Size on both Axes.
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth * 2,aSize.iHeight * 2));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		return EFail;		
		}

	// Scale down by 2 to Target Size on both Axes.
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth/2,aSize.iHeight / 2));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		return EFail;
		}
	
	
	//Create a reference bitmap for comparison
	CFbsBitmap* RefBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(RefBmp);
	
	User::LeaveIfError(RefBmp->Load(refsrcfilename));
	//Check scaling has no effect
	result = CheckBitmaps(*aSrcBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(1);//[RefBmp]
		return result;
		}
		
	// Scale up to target Bitmap
	// Second API
	CFbsBitmap* upDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(upDestBmp);

	
	TFileName upFileName( iTestParameters->iScaleUpFileName );
	User::LeaveIfError(upDestBmp->Load( upFileName ));
	
	upDestBmp->Resize(TSize(aSize.iWidth * 2 ,aSize.iHeight * 2));
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *upDestBmp);
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);// [RefBmp,upDestBmp]
		return EFail;
		}
	
	User::LeaveIfError(RefBmp->Load(refupfilename));
	//Compare and check the bitmaps
	result = CheckBitmaps(*upDestBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(2);// [RefBmp,upDestBmp]
		return result;
		}
	
	//destroy upDestBmp as it is no longer needed
	CleanupStack::PopAndDestroy(1);//[upDestBmp]

	// down scale
	CFbsBitmap* downDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(downDestBmp);

	TFileName downFileName( iTestParameters->iScaleDownFileName );
	downDestBmp->Resize(TSize(aSize.iWidth / 2,aSize.iHeight / 2));
	
	User::LeaveIfError(downDestBmp->Load( downFileName));
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *downDestBmp);
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
		return EFail;
		}
	
	User::LeaveIfError(RefBmp->Load(refdownfilename));
	//Compare and check the bitmaps
	result = CheckBitmaps(*downDestBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
		return result;
		}
	
	CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
	return result;
	}

/*
*
* DoTestStepL
*
*/

TVerdict CTestStepFastScaleXY::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Fast Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "Testing bitmap scaling");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	TFileName inputFilename( iTestParameters->iInputFilename );
	User::LeaveIfError(srcBmp->Load(inputFilename));
	const TSize size( srcBmp->SizeInPixels() );
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	result = DoTestScaleL(activeListener, srcBmp, scaler, size,iTestParameters->iQualityAlgo);
	
	CleanupStack::PopAndDestroy( 3, activeListener );//[ activeListener, srcBmp, scaler ]

	return result;
	}

/*
*
* CTestStepFastScaleX
*
*/

CTestStepFastScaleX::CTestStepFastScaleX(TUint aTestIndex,const TText*  aTestName)
	{
	//[precondition valid index ]
	__ASSERT_DEBUG( (aTestIndex < (sizeof(KTestParameters)/sizeof(TScaleBitmapUnitTestParams))),Panic(EBadArgument));
	// store a pointer to the test parameters
	iTestParameters = &(KTestParameters[aTestIndex]);
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = aTestName;
	}

/*
*
* ~CTestStepFastScaleX
*
*/

CTestStepFastScaleX::~CTestStepFastScaleX()
	{

	}

/*
*
* DoTestScaleL
* @param aActiveListener - the active listener
* @param aSrcBmp - the bitmap to scale
* @param aScaler - the scaler object
* @param aSize - the size to base our scaling on
* @param aQualityAlgo - EMinimumQuality | EMediumQuality algorithm for Quality of the image scaling.
* @returns TVerdict which is either EPass or EFail.
*/

TVerdict CTestStepFastScaleX::DoTestScaleL(CActiveListener* aActiveListener,
										   CFbsBitmap* aSrcBmp,
										   CBitmapScaler* aScaler,
										   const TSize aSize,
										   CBitmapScaler::TQualityAlgorithm aQualityAlgo)
	{
	
	TAny* ptr = &aQualityAlgo;
	TVerdict result = EPass;
	
	// Setting the Quality factor
	TInt errCode = aScaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr );
	if( errCode != KErrNone )
		{
		return EFail;
		}
		
	//Get the reference bitmaps	for scale up and scale down
    TFileName refsrcfilename(iTestParameters->iInputFilename);
	TFileName refupfilename(iTestParameters->iScaleUpFileName);
	TFileName refdownfilename(iTestParameters->iScaleDownFileName);
	if(aQualityAlgo == CBitmapScaler::EMediumQuality)
		{
		refsrcfilename.Insert(3,_L("btrans\\Ref_ScaleX_Medium"));
		refupfilename.Insert(10,_L("Ref_ScaleX_Medium"));	
		refdownfilename.Insert(10,_L("Ref_ScaleX_Medium"));	
		}
	else
		{
		refsrcfilename.Insert(3,_L("btrans\\Ref_ScaleX_Minimum"));
		refupfilename.Insert(10,_L("Ref_scaleX_Minimum"));
		refdownfilename.Insert(10,_L("Ref_scaleX_Minimum"));
		}
	
	// Scale up by 2 to Target Size on both Axes.
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth * 2,aSize.iHeight));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		return EFail;
		}
		
	// Scale down by 2 to Target Size on both Axes.
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth/2,aSize.iHeight));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		return EFail;
		}
	
	//Create a reference bitmap for comparison
	CFbsBitmap* RefBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(RefBmp);

	User::LeaveIfError(RefBmp->Load(refsrcfilename));
	//Compare and make sure that transformations had no effect
	result = CheckBitmaps(*aSrcBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(1);//[RefBmp]
		return result;
		}
		
	// Scale up to target Bitmap
	// Second API

	CFbsBitmap* upDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(upDestBmp);

	// Scale to upTarget Bitmap
	TFileName upFileName( iTestParameters->iScaleUpFileName );
	User::LeaveIfError(upDestBmp->Load( upFileName ));
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *upDestBmp);
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);	// [RefBmp,upDestBmp]
		return EFail;
		}
	
	User::LeaveIfError(RefBmp->Load(refupfilename));
	//Compare and check the bitmaps
	result = CheckBitmaps(*upDestBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(2);	// [RefBmp,upDestBmp]
		return result;
		}
	
	//destroy upDestBmp as it is no longer needed
	CleanupStack::PopAndDestroy(1);//[upDestBmp]
	
	// Create DownTarget Bitmap
	CFbsBitmap* downDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(downDestBmp);
	
	// Scale to downTarget Bitmap
	TFileName downFileName( iTestParameters->iScaleDownFileName );
	User::LeaveIfError(downDestBmp->Load( downFileName ));
	
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *downDestBmp);
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
		return EFail;
		}
	
	User::LeaveIfError(RefBmp->Load(refdownfilename));
	//Compare and check the bitmaps
	result = CheckBitmaps(*downDestBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
		return result;
		}

	CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
	return result;
	}

/*
*
* DoTestStepL
*
*/

TVerdict CTestStepFastScaleX::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Fast Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "Testing bitmap scaling");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	TFileName inputFilename( iTestParameters->iInputFilename );
	User::LeaveIfError(srcBmp->Load(inputFilename));
	
	const TSize size( srcBmp->SizeInPixels() );
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	
	result = DoTestScaleL(activeListener, srcBmp, scaler, size,iTestParameters->iQualityAlgo);
	
	CleanupStack::PopAndDestroy( 3, activeListener );//[ activeListener, srcBmp, scaler ]

	return result;
	}

/*
*
* CTestStepFastScaleY
*
*/

CTestStepFastScaleY::CTestStepFastScaleY(TUint aTestIndex,const TText*  aTestName)
	{
	//[precondition valid index ]
	__ASSERT_DEBUG( (aTestIndex < (sizeof(KTestParameters)/sizeof(TScaleBitmapUnitTestParams))),Panic(EBadArgument));
	// store a pointer to the test parameters
	iTestParameters = &(KTestParameters[aTestIndex]);
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = aTestName;
	
	}

/*
*
* ~CTestStepFastScaleY
*
*/

CTestStepFastScaleY::~CTestStepFastScaleY()
	{

	}

/*
*
* DoTestScaleL
* @param aActiveListener - the active listener
* @param aSrcBmp - the bitmap to scale
* @param aScaler - the scaler object
* @param aSize - the size to base our scaling on
* @param aQualityAlgo - EMinimumQuality | EMediumQuality algorithm for Quality of the image scaling.
* @returns TVerdict which is either EPass or EFail.
*/

TVerdict CTestStepFastScaleY::DoTestScaleL(CActiveListener* aActiveListener,
										   CFbsBitmap* aSrcBmp,
										   CBitmapScaler* aScaler,
										   const TSize aSize,
										   CBitmapScaler::TQualityAlgorithm aQualityAlgo)
	{
	TAny* ptr = &aQualityAlgo;
	TVerdict result = EPass;
	// Setting the Quality factor
	TInt errCode = aScaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr );
	if( errCode != KErrNone )
		{
		return EFail;
		}
		
	//Get the reference bitmaps	for scale up and scale down
    TFileName refsrcfilename(iTestParameters->iInputFilename);
	TFileName refupfilename(iTestParameters->iScaleUpFileName);
	TFileName refdownfilename(iTestParameters->iScaleDownFileName);
	if(aQualityAlgo == CBitmapScaler::EMediumQuality)
		{
		refsrcfilename.Insert(3,_L("btrans\\Ref_ScaleY_Medium"));
		refupfilename.Insert(10,_L("Ref_ScaleY_Medium"));	
		refdownfilename.Insert(10,_L("Ref_ScaleY_Medium"));	
		}
	else
		{
		refsrcfilename.Insert(3,_L("btrans\\Ref_ScaleY_Minimum"));
		refupfilename.Insert(10,_L("Ref_scaleY_Minimum"));
		refdownfilename.Insert(10,_L("Ref_scaleY_Minimum"));
		}
	
	// Scale up by 2 to Target Size on both Axes.
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth,aSize.iHeight * 2));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		return EFail;
		}
	
	// Scale down by 2 to Target Size on both Axes.
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, TSize(aSize.iWidth,aSize.iHeight / 2));
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		return EFail;
		}
	
	//Create a reference bitmap for comparison
	CFbsBitmap* RefBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(RefBmp);
	
	User::LeaveIfError(RefBmp->Load(refsrcfilename));
	//Compare and make sure that transformations had no effect
	result = CheckBitmaps(*aSrcBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(1);//[RefBmp]
		return result;
		}

	// Scale up to target Bitmap
	// Second API
	CFbsBitmap* upDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(upDestBmp);


	TFileName upFileName( iTestParameters->iScaleUpFileName );

	User::LeaveIfError(upDestBmp->Load( upFileName ));
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *upDestBmp);
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);// [RefBmp,upDestBmp]
		return EFail;
		}
	
	User::LeaveIfError(RefBmp->Load(refupfilename));
	//Compare and check the bitmaps
	result = CheckBitmaps(*upDestBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(2);// [RefBmp,upDestBmp]
		return result;
		}
		
	//destroy upDestBmp as it is no longer needed
	CleanupStack::PopAndDestroy(1);//[upDestBmp]

	// Scale Y
	CFbsBitmap* downDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(downDestBmp);

	TFileName downFileName( iTestParameters->iScaleDownFileName );
	User::LeaveIfError(downDestBmp->Load( downFileName )) ;	
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *downDestBmp);
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
		return EFail;
		}
	
	User::LeaveIfError(RefBmp->Load(refdownfilename));
	//Compare and check the bitmaps
	result = CheckBitmaps(*downDestBmp,*RefBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
		}

	CleanupStack::PopAndDestroy(2);// [RefBmp,downDestBmp]
	return result;
	}

/*
*
* DoTestStepL
*
*/

TVerdict CTestStepFastScaleY::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Fast Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "Testing bitmap scaling");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	TFileName inputFilename( iTestParameters->iInputFilename );
	 
	User::LeaveIfError(srcBmp->Load(inputFilename));
	const TSize size( srcBmp->SizeInPixels() );
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	//Enable UseMediumQualityAlgorithm
	CBitmapScaler::TQualityAlgorithm qualityAlgo = CBitmapScaler::EMediumQuality;
	TAny* ptr = &qualityAlgo;

	// Setting the Quality factor
	TInt errCode = scaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr );
	if( errCode != KErrNone )
		{
		CleanupStack::PopAndDestroy( 3, activeListener );//[ activeListener, srcBmp, scaler ]
		return EFail;
		}

	result = DoTestScaleL(activeListener, srcBmp, scaler, size, iTestParameters->iQualityAlgo);
	
	//[ activeListener, srcBmp, scaler ]
	CleanupStack::PopAndDestroy( 3, activeListener );

	return result;
	}

CTestStepFastScaleCopy::CTestStepFastScaleCopy(TUint aTestIndex,const TText*  aTestName)
	{
	//[precondition valid index ]
	__ASSERT_DEBUG( (aTestIndex < (sizeof(KTestParameters)/sizeof(TScaleBitmapUnitTestParams))),Panic(EBadArgument));
	// store a pointer to the test parameters
	iTestParameters = &(KTestParameters[aTestIndex]);
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = aTestName;
	
	}

CTestStepFastScaleCopy::~CTestStepFastScaleCopy()
	{

	}

/*
*
* DoTestScaleL
* @param aActiveListener - the active listener
* @param aSrcBmp - the bitmap to scale
* @param aScaler - the scaler object
* @param aSize - the size to base our scaling on
* @param aQualityAlgo - EMinimumQuality | EMediumQuality algorithm for Quality of the image scaling.
* @returns TVerdict which is either EPass or EFail.
*/

TVerdict CTestStepFastScaleCopy::DoTestScaleL(CActiveListener* aActiveListener,
										   CFbsBitmap* aSrcBmp,
										   CBitmapScaler* aScaler,
										   const TSize aSize,
										   CBitmapScaler::TQualityAlgorithm aQualityAlgo)
	{
	TAny* ptr = &aQualityAlgo;
	TVerdict result = EPass;
	// Setting the Quality factor
	TInt errCode = aScaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr );
	if( errCode != KErrNone )
		{
		return EFail;
		}
		
	//Get the reference bitmaps	for scale up and scale down
	TFileName refupfilename(iTestParameters->iScaleUpFileName);
	if(aQualityAlgo == CBitmapScaler::EMediumQuality)
		{
		refupfilename.Insert(10,_L("Ref_FastScaleCopy_Medium"));	
		}
	else
		{
		refupfilename.Insert(10,_L("Ref_FastScaleCopy_Minimum"));
		}
	
	//Create a reference bitmap for comparison
	CFbsBitmap* RefBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(RefBmp);
	
	// Second API

	CFbsBitmap* upDestBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(upDestBmp);

	// Scale up and down
	TFileName upFileName( iTestParameters->iScaleUpFileName );
	
	User::LeaveIfError(upDestBmp->Load( upFileName )) ;
	
	upDestBmp->Resize(TSize(aSize.iWidth,aSize.iHeight));
	aActiveListener->InitialiseActiveListener();
	aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, *upDestBmp);
	
	//Compare and check the bitmaps
	User::LeaveIfError(RefBmp->Load(refupfilename));
	result = CheckBitmaps(*RefBmp,*upDestBmp,-1);
	if(result == EFail)
		{
		INFO_PRINTF1(_L("Bitmaps don't match"));
		}
	
	CActiveScheduler::Start();
	if(aActiveListener->iStatus!=KErrNone)
		{
		CleanupStack::PopAndDestroy(2);// [RefBmp,upDestBmp]
		return EFail;
		}
	CleanupStack::PopAndDestroy(2);// [RefBmp,upDestBmp]
	return result;
	}

/*
*
* DoTestStepL
*
*/

TVerdict CTestStepFastScaleCopy::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Fast Scale");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "Testing bitmap scaling");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	TFileName inputFilename( iTestParameters->iInputFilename );
	 
	User::LeaveIfError(srcBmp->Load(inputFilename)) ;
	const TSize size( srcBmp->SizeInPixels() );
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	//Enable UseMediumQualityAlgorithm
	CBitmapScaler::TQualityAlgorithm qualityAlgo = CBitmapScaler::EMediumQuality;
	TAny* ptr = &qualityAlgo;

	// Setting the Quality factor
	TInt errCode = scaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr );
	if( errCode != KErrNone )
		{
        CleanupStack::PopAndDestroy( 3, activeListener );	//[ activeListener, srcBmp, scaler ]
		return EFail;
		}
	result = DoTestScaleL(activeListener, srcBmp, scaler, size, iTestParameters->iQualityAlgo);

	CleanupStack::PopAndDestroy( 3, activeListener );//[ activeListener, srcBmp, scaler ]

	return result;
	}
	
	

CTestStepScale16MA::CTestStepScale16MA(const TText* aTestName, const TDesC& aRgbFile, const TDesC& aAlphaFile
										):
													iRgbFile (aRgbFile),
													iAlphaFile(aAlphaFile)
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = aTestName;
	}
	
CTestStepScale16MA::~CTestStepScale16MA()
	{
	}
	
TVerdict CTestStepScale16MA::DoTestStepL()
	{
	TVerdict result = EPass;
	
	_LIT( KTestScale, "Test Step Scale with Alpha Channel");
	Log( KTestScale );
	
	_LIT( KTestStepScale, "Testing bitmap scaling");
	Log( KTestStepScale );
	
	//[ Create an active Listener ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	CFbsBitmap* rgb		=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(rgb);
	User::LeaveIfError( rgb->Create(TSize(1,1), EColor16M) );
	User::LeaveIfError( rgb->Load(iRgbFile) );
	
	CFbsBitmap* alpha	=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(alpha);
	User::LeaveIfError( alpha->Create(TSize(1,1), EGray256) );
	User::LeaveIfError( alpha->Load(iAlphaFile) );	
	
	CFbsBitmap* srcBmp=Create16MABitmapL(*rgb, *alpha);
	
	CleanupStack::PushL(srcBmp);

	const TSize size( srcBmp->SizeInPixels() );
	
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	result = DoTestScaleL(activeListener, srcBmp, rgb, alpha, scaler);
	
	CleanupStack::PopAndDestroy( 5, activeListener );//[ activeListener, srcBmp, scaler ]

	return result;	
	}


CFbsBitmap* CTestStepScale16MA::CreateRefBitmapL(CFbsBitmap& aRgbSrc,CFbsBitmap& aAlphaSrc,
									const TSize& aDestSize,
									CBitmapScaler::TQualityAlgorithm aQualityAlgo,
									CBitmapScaler& aScaler,
									TBool aLowMemAlg)
	{
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );	
	
	CFbsBitmap* rgbScaled = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(rgbScaled);
	User::LeaveIfError( rgbScaled->Create( aDestSize, aRgbSrc.DisplayMode()) );
	
	User::LeaveIfError( aScaler.SetQualityAlgorithm(aQualityAlgo) );
	User::LeaveIfError( aScaler.UseLowMemoryAlgorithm( aLowMemAlg ) );
	activeListener->InitialiseActiveListener();
	aScaler.Scale(&activeListener->iStatus, aRgbSrc, *rgbScaled);
	CActiveScheduler::Start();
	User::LeaveIfError( activeListener->iStatus.Int() );
	
	CFbsBitmap* alphaScaled = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(alphaScaled);
	User::LeaveIfError( alphaScaled->Create( aDestSize, aAlphaSrc.DisplayMode()) );	
	
	User::LeaveIfError( aScaler.SetQualityAlgorithm(aQualityAlgo) );
	User::LeaveIfError( aScaler.UseLowMemoryAlgorithm( aLowMemAlg ) );
	activeListener->InitialiseActiveListener();
	aScaler.Scale(&activeListener->iStatus, aAlphaSrc, *alphaScaled);
	CActiveScheduler::Start();
	User::LeaveIfError( activeListener->iStatus.Int() );
	
	CFbsBitmap* refScaled = Create16MABitmapL(*rgbScaled, *alphaScaled);
	
	CleanupStack::PopAndDestroy(alphaScaled);
	CleanupStack::PopAndDestroy(rgbScaled);
	CleanupStack::PopAndDestroy(activeListener );
	return refScaled;
	}

TVerdict CTestStepScale16MA::DoTestScaleL(CActiveListener* aActiveListener,
					CFbsBitmap* aSrcBmp,
					CFbsBitmap* aRgbSrc,
					CFbsBitmap* aAlphaSrc,
					CBitmapScaler* aScaler)
	{
	const TBool KSaveErrorBmp=EFalse; // set to true to save improperly scaled bitmaps to c:
	const CBitmapScaler::TQualityAlgorithm  quality[]=
				{
				CBitmapScaler::EMinimumQuality, CBitmapScaler::EMediumQuality, CBitmapScaler::EMaximumQuality
				};
	static const TInt mulDiv[]=
		{
		0x17, 0x15, 0x14, 0x12, 		// 1/7 ,1/5, 1/4, 1/2
		0x93, 0x42, 0x32, 0x21			// 3/1, 3/2, 2/1
		};

	CFbsBitmap* srcDup = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(srcDup);
	User::LeaveIfError( srcDup->Create( aSrcBmp->SizeInPixels(), aSrcBmp->DisplayMode()) );
	DuplicateContentL(*aSrcBmp, *srcDup);
	
	TVerdict verdict=EPass;
	
	for (TInt lowMemMode=0; lowMemMode<=1; ++lowMemMode)
		{
		
		for (TInt md=0; md< sizeof(mulDiv)/sizeof(mulDiv[0]); md++)
			{
			
			TSize destSize(	srcDup->SizeInPixels().iWidth	*(mulDiv[md]>>4)/(mulDiv[md]&0xF), 
							srcDup->SizeInPixels().iHeight	*(mulDiv[md]>>4)/(mulDiv[md]&0xF)
						);
			for (TInt q=0; q< sizeof(quality)/sizeof(quality[0]); q++)
				{
				CBitmapScaler::TQualityAlgorithm qualityAlgo = quality[q];
				for (TInt oneParamApi=0; oneParamApi<=1; oneParamApi++)
					{
					TAny* ptr;
					ptr = &qualityAlgo;
					// Setting the Quality factor
					User::LeaveIfError( aScaler->CustomCommand( KICLUidSetQualityAlgorithmCommand, ptr ) );
					ptr = &lowMemMode;
					User::LeaveIfError( aScaler->CustomCommand( KICLUidUseLowMemoryAlgorithmCommand, ptr ) );

					CFbsBitmap* resBmp=NULL;
					CFbsBitmap* destBmp=new (ELeave) CFbsBitmap();
					CleanupStack::PushL(destBmp);
					//
					aActiveListener->InitialiseActiveListener();
					// Scale up by 2 to Target Size on both Axes.
					if (oneParamApi)
						{
						DuplicateContentL(*srcDup, *aSrcBmp);
						aScaler->Scale(&aActiveListener->iStatus, *aSrcBmp, destSize );
						resBmp = aSrcBmp;
						}
					else
						{
						User::LeaveIfError( destBmp->Create(destSize, aSrcBmp->DisplayMode() ) );
						resBmp=destBmp;
						aScaler->Scale(&aActiveListener->iStatus, *srcDup, *destBmp);
						}					
					
					CActiveScheduler::Start();
					User::LeaveIfError( aActiveListener->iStatus.Int() );
					
					CFbsBitmap* refBmp=CreateRefBitmapL(*aRgbSrc, *aAlphaSrc, destSize, qualityAlgo, *aScaler, lowMemMode);
				
					TVerdict v=CheckBitmaps(*resBmp,*refBmp, -1, ETrue);
					delete refBmp;
					if (v!=EPass)
						{
						Log(_L("-- Error: Bitmaps doesn't match API mode=%d, alg=%d Low Mem %d, dest(%d,%d)"),
									oneParamApi, qualityAlgo, lowMemMode, destSize.iWidth, destSize.iHeight);
						verdict = EFail;
						if (KSaveErrorBmp)
							{
							CFbsBitmap* rgb;
							CFbsBitmap* alpha;
							Split16MABitmapL(*resBmp, rgb, alpha);
							TBuf<128> bf;
							bf.Format(_L("c:\\bmp_RGB_md_%d_api_%d_lm_%d_q_%d.mbm"),md,oneParamApi,lowMemMode,q);
							rgb->Save(bf);
							delete rgb;
							bf.Format(_L("c:\\bmp_Grey_md_%d_api_%d_lm_%d_q_%d.mbm"),md,oneParamApi,lowMemMode,q);
							alpha->Save(bf);
							delete alpha;
							}
						}
					
					CleanupStack::PopAndDestroy(destBmp);		
					} // API
				}  // quality
			} // mulDiv
		} // low Mem mode
	CleanupStack::PopAndDestroy(srcDup);
	return verdict;
	}

// CBitmapScaler doesn't scale bitmaps properly in some cases
/**
CTestStepFastDownScale constructor.
Each test step initialises its own name.
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFastDownScale::CTestStepFastDownScale(const TText*  aTestName)
	{
	// store the name of this test case
	iTestStepName = aTestName;
	}

/**
destructor CTestStepFastDownScale
@lib "TS_BitmapTransforms.lib"
*/
CTestStepFastDownScale::~CTestStepFastDownScale()
	{
	}

/**
Do the test step.
This test checks fast down scaling of Bitmap and its results
@return	"enum TVerdict"
The result of the test step
@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepFastDownScale::DoTestStepL()
	{
	// Printing to the console and log file
	_LIT( KTestScale, "This test does fast down Scaling followed by compare.");
	Log( KTestScale );
	
	_LIT(KImageTestSourceFileName, "Fast_DownScale_Src.mbm");
		
	//[ Create an active Listener ]
	 CActiveListener* activeListener = new(ELeave)CActiveListener;
	
	//[ push the listener on the stack ]
	CleanupStack::PushL( activeListener );
	
	SetTestFileNameL( KImageTestSourceFileName);
	
	CFbsBitmap* refBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(refBmp);
		
	CFbsBitmap* srcMinBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMinBmp);
	srcMinBmp->Load(FileName());
				
	CBitmapScaler* scaler = CBitmapScaler::NewL();
	CleanupStack::PushL(scaler);
	
	TSize OrgSize(srcMinBmp->SizeInPixels());
	TSize DestSize(TSize(10,10));
	
	// For Low Quality
	scaler->SetQualityAlgorithm(CBitmapScaler::EMinimumQuality);
	scaler->UseLowMemoryAlgorithm(ETrue);
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *srcMinBmp, DestSize);
	
	CActiveScheduler::Start();
	User::LeaveIfError(  activeListener->iStatus.Int() );
	User::LeaveIfError( refBmp->Load(_L("c:\\Btrans\\Ref_Min_Fast_DownScale_Src.mbm")) );
	iTestStepResult = CheckBitmaps( *srcMinBmp, *refBmp, -1 );
	if (iTestStepResult == EFail)	
		{
		INFO_PRINTF1(_L("Minimum Quality Images are Not Identical"));	
		CleanupStack::PopAndDestroy(4, activeListener);
		return iTestStepResult;
		}
	INFO_PRINTF1(_L("Minimum Quality Images are Identical"));	
		
	// For Medium Quality
	CFbsBitmap* srcMedBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMedBmp);
	srcMedBmp->Load(FileName());

	scaler->SetQualityAlgorithm(CBitmapScaler::EMediumQuality);
	scaler->UseLowMemoryAlgorithm(ETrue);
	
	activeListener->InitialiseActiveListener();
	scaler->Scale(&activeListener->iStatus, *srcMedBmp, DestSize);
		
	CActiveScheduler::Start();
	User::LeaveIfError(  activeListener->iStatus.Int() );
	User::LeaveIfError( refBmp->Load(_L("c:\\Btrans\\Ref_Med_Fast_DownScale_Src.mbm")) );
	iTestStepResult = CheckBitmaps( *srcMedBmp, *refBmp, -1 );
	if(iTestStepResult == EFail)	
		{
		INFO_PRINTF1(_L("Medium Quality Images are Not Identical"));	
		CleanupStack::PopAndDestroy(5, activeListener);
		return iTestStepResult;
		}
	INFO_PRINTF1(_L("Medium Quality Images are Identical"));	
	
	//[ scaler, srcBmp, refbmp, activeListener ]
	CleanupStack::PopAndDestroy(5, activeListener);
	return iTestStepResult;
	}
