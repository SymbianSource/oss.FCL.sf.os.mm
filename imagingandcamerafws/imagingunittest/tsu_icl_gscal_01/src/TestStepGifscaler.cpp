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
#include <imageconversion.h>

// Test system includes
#include <testframework.h>

#include "TestStepGifScaler.h"
#include "TestSuiteGifScaler.h"
#include "ICLFbsSessionTest.h"

// Heap size for these tests.
const TInt KHeapSize = 0x20000;

_LIT(KIclPanicCategory, "TS_GifScaler");

#ifndef _DEBUG
_LIT(KDebugTestOnly, "Panic, DebugTest");
#endif
/** 
 This function raises a panic

@param TS_GifScalerMain::TS_GifScalerPanic one of the several panics codes
       that may be raised by this dll
@panic  ENoSourceBitmap is raised when the src bitmap supplied to one of the api's
	    has not been created ie has a null handle

*/

GLDEF_C void Panic(TS_GifScalerPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

	
/*
CActiveListener
*/
CActiveListener::CActiveListener()
 : CActive(CActive::EPriorityIdle)
	{
	CActiveScheduler::Add(this);
	}

CActiveListener::~CActiveListener()
	{
	}

/*
InitialiseActiveListener
*/
void CActiveListener::InitialiseActiveListener()
	{
	SetActive();
	}

/*
RunL
*/

void CActiveListener::RunL()
	{
	CActiveScheduler::Stop(); 
	}

/*
  DoCancel()
  */
void CActiveListener::DoCancel()
	{
	}


/**
*
* CTestStepGifScaler constructor 
*
* 
*/
CTestStepGifScaler::CTestStepGifScaler() 
:  iFs(), iGifScalerSuite(NULL), iSourceFileName(), iScheduler(NULL)
	{
	}

/**
*
* CTestStepGifScaler destructor
*
* 
*/
CTestStepGifScaler::~CTestStepGifScaler()
	{
	}

/**
* SetSuite is a set function
* @param  aPtr is a ptr to a CTestSuiteGifScaler
* @lib "TSU_ICL_GSCAL.lib"
*/

void CTestStepGifScaler::SetSuite(CTestSuiteGifScaler* aPtr)
	{
	iGifScalerSuite = aPtr;
	}


/**
DoTestStepPreambleL is the preamble function for test steps
It creates an active scheduler and installs it. This is necessary
since we are multithreading and each test step runs in a separate thread.
@returns TVerdict which is either EPass or Efail
The function can leave if there is insufficient memeory
@lib "TSU_ICL_GSCAL.lib"
*/
TVerdict CTestStepGifScaler::DoTestStepPreambleL()
	{
	//[ mark the heap and unmark at the end of postamble ]
	__MM_HEAP_MARK;
	User::LeaveIfError(FbsStartup());
	// [ connect to the bitmap server and check the error code]
	TInt errCode = RFbsSession::Connect();
	if( errCode != KErrNone )
		{
		//[ log failed to connect to bitmap server ]
		_LIT(KFailConnect, "Failed to connect to bitmap server in teststep preamble = %d");
		Log(KFailConnect, errCode );
		return EFail;
		}
	
	iScheduler = new(ELeave)CActiveScheduler;
    //[ push the scheduler on the stack ]
	CleanupStack::PushL( iScheduler );
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
	
	// [Pop iScheduler ] 
	CleanupStack::Pop( iScheduler);
	
	return EPass;
	}

/**
The post amble step cleans up the allocated memory in the preamble
@returns TVerdict EPass or EFail
@leaves can leave due to insufficient memory
@lib "TSU_ICL_GSCAL.lib"
*/

TVerdict CTestStepGifScaler::DoTestStepPostambleL()
	{
	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	
	// [ since we are good citizens let's disconnect from the bitmap server]
	RFbsSession::Disconnect();

    // [ umark the heap which was marked in preamble ]
	__MM_HEAP_MARKEND;

	return EPass;
	}


/**
*
* CTestStepGifScaler Suite accessor function
* @returns CTestSuiteGifScaler*  
* @lib "TSU_ICL_GSCAL.lib"
*/

CTestSuiteGifScaler* CTestStepGifScaler::Suite()
	{
	return iGifScalerSuite;
	}


/**
*
* CTestStepGifScaler SetTestFileName
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&  afull file and path name
* @returns void
* @lib "TSU_ICL_GSCAL.lib"
*/

void CTestStepGifScaler::SetTestFileNameL(const TDesC& aPathNameAndExtn)
	{
	//[ append the filename relative to the default path ]
	TFileName fileName = iGifScalerSuite->DefaultPath();
	User::LeaveIfError(iFs.Connect());
	fileName.Append(aPathNameAndExtn);
	iSourceFileName = fileName ;
	}


/**
*
* CTestStepGifScaler CheckBitmapL
*
* Checks a bitmap against the current filename 
* @param aBitmap a reference to a bitmap 
* @leaves memory allocation failure
* @leaves failure to load the current file
* @returns void
* @lib "TSU_ICL_GSCAL.lib"
*/

TVerdict CTestStepGifScaler::CheckBitmapL(CFbsBitmap& aBitmap)
	{
	CFbsBitmap *refBitmap = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(refBitmap);
	
	// Load the current bitmap pointed to by filename into memory.
	User::LeaveIfError(refBitmap->Load(FileName()));
	
	// Check against the supplied bitmap.
	TVerdict result = CheckBitmaps(aBitmap, *refBitmap);
	
	CleanupStack::PopAndDestroy(refBitmap);
	return result;
	}


/**
*
* CTestStepGifScaler CheckBitmaps
* This function checks that two bitmaps are equivalent
* where the one has been derived from the other by a rotate/mirror transformation
*@param aBitmap1 first bitmap for comparison
*@param aBitmap2 secon bitmap for comparison
*@returns TVerdict which is either EPass or EFail. The bitmaps are equivalent if
*         the result is Epass
*/

TVerdict CTestStepGifScaler::CheckBitmaps(CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2)
	{
	TVerdict result = EFail;
	
	const TSize size = aBitmap1.SizeInPixels();
	for (TInt y = 0; y < size.iHeight; y++)
		{
		for (TInt x = 0; x < size.iWidth; x++)
			{
			TPoint pt(x,y);
			TRgb color1,color2;
			aBitmap1.GetPixel(color1,pt);
			aBitmap2.GetPixel(color2,pt);
				
			if (color1 != color2)
				return result;
			}
		}
	
	result = EPass;
	return result;
	}


/**
*
* CTestStepGifScaler GetFileName
*
* 
* 
*/
TFileName& CTestStepGifScaler::FileName() 
	{
	return  iSourceFileName;
	}


/**
*
* CTestStepGifScaler ApplyPalette
* This function takes a source bitmap containing palette indices
* and looks up each pixel palette index in a given palette to create
* a full colour target bitmap
*@param aSrcBitmap bitmap containing a palette index at each pixel
*@param aPalette palette to apply
*@param aTgtBitmap the bitmap that will contain the output
*@returns void
*/
void CTestStepGifScaler::ApplyPaletteL(CFbsBitmap& aSrcBitmap, CPalette& aPalette, CFbsBitmap& aTgtBitmap) const
	{
	TSize size1 = aSrcBitmap.SizeInPixels();
	TSize size2 = aTgtBitmap.SizeInPixels();
	
	ASSERT(aSrcBitmap.SizeInPixels() == aTgtBitmap.SizeInPixels());

	TColor256Util* util = new (ELeave) TColor256Util;
	CleanupStack::PushL(util);
	CPalette* palette = CPalette::NewDefaultL(EColor256);
	CleanupStack::PushL(palette);
	util->Construct(*palette);

	aTgtBitmap.LockHeap();
	TSize srcSize = aSrcBitmap.SizeInPixels();
	TInt scanlineLength = CFbsBitmap::ScanLineLength(srcSize.iWidth, aTgtBitmap.DisplayMode());
	TUint8* basePos = reinterpret_cast<TUint8*>(aTgtBitmap.DataAddress());

	TPoint pixelPos(0, 0);
	TRgb pixel;
	for (; pixelPos.iY < srcSize.iHeight; pixelPos.iY++)
		{
		for (pixelPos.iX = 0; pixelPos.iX < srcSize.iWidth; pixelPos.iX++)
			{
			aSrcBitmap.GetPixel(pixel, pixelPos);
			TInt index = util->Color256(pixel);
			TRgb finalColor = aPalette.GetEntry(index);
			basePos[pixelPos.iX * 4] = finalColor.Blue();
			basePos[pixelPos.iX * 4 + 1] = finalColor.Green();
			basePos[pixelPos.iX * 4 + 2] = finalColor.Red();
			basePos[pixelPos.iX * 4 + 3] = 0;
			}
		basePos += scanlineLength;
		}
	aTgtBitmap.UnlockHeap();			

	CleanupStack::PopAndDestroy(palette);
	CleanupStack::PopAndDestroy(util);
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale1::CTestStepScale1()
	{
	// Store the name of this test case
	// This is the name that is used by the script file
	_LIT(KTestStep001, "MM-ICL-GSCAL-U-001-HP");
	iTestStepName = KTestStep001; 
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale1::~CTestStepScale1()
	{
	}

/**
*
* Do the test step.
* Attempt to create a GifScaler without creating the source bitmap. (Panic test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/

TVerdict CTestStepScale1::DoTestStepL()
	{
	_LIT(KTestScale1, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-001-HP");
	Log(KTestScale1);
	
	_LIT(KTestStepScale1, "This test panics: the source bitmap must be created");
	Log(KTestStepScale1);
		
	// Allocate the src bitmap.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
		
	// Attempt to create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	// Fail the test if we reach this point.
	iTestStepResult = EFail;

	CleanupStack::PopAndDestroy(2, srcBmp);
	return iTestStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale2::CTestStepScale2()
	{
	// Store the name of this test case
	// this is the name that is used by the script file.
	_LIT(KTestStep002, "MM-ICL-GSCAL-U-002-HP");
	iTestStepName = KTestStep002;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale2::~CTestStepScale2()
	{
	}

/**
*
* Do the test step.
* Attempt to create a Gifscaler without creating the source mask bitmap (Panic test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale2::DoTestStepL()
	{
	_LIT(KTestScale2, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-002-HP");
	Log(KTestScale2);
	
	_LIT(KTestStepScale2, "This test panics: the source bitmap mask must be created");
	Log(KTestStepScale2);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));
		
	// Allocate the src mask bitmap.
	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);

	// Attempt to create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp);
	CleanupStack::PushL(gifScaler);

	// Fail the test if we reach this point.
	iTestStepResult = EFail;

	CleanupStack::PopAndDestroy(3, srcBmp);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale3::CTestStepScale3()
	{
	// Store the name of this test case
	// this is the name that is used by the script file.
	_LIT(KTestStep003, "MM-ICL-GSCAL-U-003-HP");
	iTestStepName = KTestStep003;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale3::~CTestStepScale3()
	{
	}

/**
*
* Do the test step.
* Attempt to create a Gifscaler with a zero size source bitmap (KErrArgument test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale3::DoTestStepL()
	{
	_LIT(KTestScale3, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-003-HP");
	Log(KTestScale3);
	
	_LIT(KTestStepScale3, "This test returns KErrArgument: the source bitmap must not be zero size.");
	Log(KTestStepScale3);
		
	// Create the src bitmap.
	// Use a zero size width and arbitary color depth, because we want an invalid argument.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 0), EColor16M));

	// Attempt to create the gif scaler.
	CGifScaler* gifScaler = NULL;
	TRAPD(err, gifScaler = CGifScaler::NewL(*srcBmp));
	if (err != KErrArgument)
		{
		iTestStepResult = EFail;
		delete gifScaler;

		CleanupStack::PopAndDestroy(srcBmp);
		return iTestStepResult;
		}

	// Repeat the test with a zero height bitmap.
	User::LeaveIfError(srcBmp->Resize(TSize(0, 10)));
	TRAP(err, gifScaler = CGifScaler::NewL(*srcBmp));
	if (err != KErrArgument)
		iTestStepResult = EFail;

	delete gifScaler;
	CleanupStack::PopAndDestroy(srcBmp);
	return iTestStepResult;
	}

	
/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale4::CTestStepScale4()
	{
	_LIT(KTestStep004, "MM-ICL-GSCAL-U-004-HP");
	iTestStepName = KTestStep004;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale4::~CTestStepScale4()
	{
	}

/**
*
* Do the test step.
* Attempt to create a Gifscaler with a different size source and mask bitmaps (KErrArgument test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale4::DoTestStepL()
	{
	_LIT(KTestScale4, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-004-HP");
	Log(KTestScale4);
	
	_LIT(KTestStepScale4, "This test returns KErrArgument: the source and source mask bitmaps must be the same size.");
	Log(KTestStepScale4);
		
	// Create the src bitmap.
	// Use an arbitrary size, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the src mask bitmap.
	// Use an arbitrary but different size, because that's what we are testing.
	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);
	User::LeaveIfError(srcMaskBmp->Create(TSize(20, 20), EGray2));

	// Attempt to create the gif scaler.
	CGifScaler* gifScaler = NULL;
	TRAPD(err, gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp));
	if (err != KErrArgument)
		iTestStepResult = EFail;

	delete gifScaler;
	CleanupStack::PopAndDestroy(2, srcBmp);
	return iTestStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale5::CTestStepScale5()
	{
	_LIT(KTestStep005, "MM-ICL-GSCAL-U-005-HP");
	iTestStepName = KTestStep005;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale5::~CTestStepScale5()
	{
	}

/**
*
* Do the test step.
* Attempt to create a Gifscaler with a different size source and mask bitmaps (KErrArgument test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale5::DoTestStepL()
	{
	_LIT(KTestScale5, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-005-HP");
	Log(KTestScale5);
	
	_LIT(KTestStepScale5, "This test returns KErrArgument: the source mask bitmap must be EGray2 or EGray256.");
	Log(KTestStepScale5);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the src mask bitmap.
	// Use an arbitrary but invalid color depth, because that's what we are testing.
	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);
	User::LeaveIfError(srcMaskBmp->Create(TSize(10, 10), EColor16M));

	// Attempt to create the gif scaler.
	CGifScaler* gifScaler = NULL;
	TRAPD(err, gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp));
	if (err != KErrArgument)
		iTestStepResult = EFail;

	delete gifScaler;
	CleanupStack::PopAndDestroy(2, srcBmp);
	return iTestStepResult;
	}

	
/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale6::CTestStepScale6()
	{
	_LIT(KTestStep006, "MM-ICL-GSCAL-U-006-HP");
	iTestStepName = KTestStep006;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale6::~CTestStepScale6()
	{
	}

/**
*
* Do the test step.
* Attempt to create a Gifscaler with invalid options. (KErrArgument test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale6::DoTestStepL()
	{
	_LIT(KTestScale6, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-006-HP");
	Log(KTestScale6);
	
	_LIT(KTestStepScale6, "This test returns KErrArgument: the options flag must be valid.");
	Log(KTestStepScale6);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the src mask bitmap.
	// Use an arbitrary size and color depth.
	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);
	User::LeaveIfError(srcMaskBmp->Create(TSize(10, 10), EGray2));

	// Attempt to create the gif scaler using an arbitrary invalid option.
	CGifScaler* gifScaler = NULL;
	TRAPD(err, gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp, CGifScaler::TOptions(10)));
	if (err != KErrArgument)
		iTestStepResult = EFail;

	delete gifScaler;
	CleanupStack::PopAndDestroy(2, srcBmp);
	return iTestStepResult;
	}
	
	
/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale7::CTestStepScale7()
	{
	_LIT(KTestStep007, "MM-ICL-GSCAL-U-007-HP");
	iTestStepName = KTestStep007;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/

CTestStepScale7::~CTestStepScale7()
	{
	}

/**
*
* Do the test step.
* Test scaling with a NULL request status. (Panic test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale7::DoTestStepL()
	{
	_LIT(KTestScale7, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-007-HP");
	Log(KTestScale7);
	
	_LIT(KTestStepScale7, "This test panics: the TRequest status must not be NULL.");
	Log(KTestStepScale7);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create a valid palette.
	CPalette* palette = CPalette::NewL(256);
	CleanupStack::PushL(palette);

	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);
	gifScaler->Scale(NULL, *srcBmp, *palette);

	// Return fail if we get this far.
	iTestStepResult = EFail;
	
	CleanupStack::PopAndDestroy(3, srcBmp);
	return iTestStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale8::CTestStepScale8()
	{
	_LIT(KTestStep008, "MM-ICL-GSCAL-U-008-HP");
	iTestStepName = KTestStep008;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/

CTestStepScale8::~CTestStepScale8()
	{
	}

/**
*
* Do the test step.
* Test scaling with an uncreated destination bitmap. (Panic test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale8::DoTestStepL()
	{
	_LIT(KTestScale8, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-008-HP");
	Log(KTestScale8);
	
	_LIT(KTestStepScale8, "This test panics: the destination bitmap must have been created.");
	Log(KTestStepScale8);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Allocate the dst bitmap. (no creation)
	CFbsBitmap* dstBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(dstBmp);

	// Create a valid palette.
	CPalette* palette = CPalette::NewL(256);
	CleanupStack::PushL(palette);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);
	gifScaler->Scale(&listener->iStatus, *dstBmp, *palette);

	CleanupStack::PopAndDestroy(4, srcBmp);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale9::CTestStepScale9()
	{
	_LIT(KTestStep009, "MM-ICL-GSCAL-U-009-HP");
	iTestStepName = KTestStep009;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/

CTestStepScale9::~CTestStepScale9()
	{
	}

/**
*
* Do the test step.
* Test scaling with an invalid destination bitmap. (KErrArgument test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale9::DoTestStepL()
	{
	_LIT(KTestScale9, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-009-HP");
	Log(KTestScale9);
	
	_LIT(KTestStepScale9, "This test returns KErrArgument: the destination bitmap must be EColor256.");
	Log(KTestStepScale9);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the dst bitmap.
	// Arbitrary size, invalid color depth. We are not going to generating a result. 
	CFbsBitmap* dstBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(dstBmp);
	User::LeaveIfError(dstBmp->Create(TSize(10, 10), EColor16M));

	// Create a valid palette.
	CPalette* palette = CPalette::NewL(256);
	CleanupStack::PushL(palette);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *dstBmp, *palette);
	CActiveScheduler::Start();

	if (listener->iStatus != KErrArgument)
		iTestStepResult = EFail;

	CleanupStack::PopAndDestroy(5, srcBmp);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale10::CTestStepScale10()
	{
	_LIT(KTestStep010, "MM-ICL-GSCAL-U-010-HP");
	iTestStepName = KTestStep010;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/

CTestStepScale10::~CTestStepScale10()
	{
	}

/**
*
* Do the test step.
* Test scaling with an invalid palette. (KErrArgument test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale10::DoTestStepL()
	{
	_LIT(KTestScale10, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-010-HP");
	Log(KTestScale10);
	
	_LIT(KTestStepScale10, "This test returns KErrArgument: the palette must have 256 entries.");
	Log(KTestStepScale10);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the dst bitmap.
	// Arbitrary size. We are not going to generating a result. 
	CFbsBitmap* dstBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(dstBmp);
	User::LeaveIfError(dstBmp->Create(TSize(10, 10), EColor256));

	// Create an invalid palette.
	CPalette* palette = CPalette::NewL(255);
	CleanupStack::PushL(palette);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *dstBmp, *palette);
	CActiveScheduler::Start();

	if (listener->iStatus != KErrArgument)
		iTestStepResult = EFail;

	CleanupStack::PopAndDestroy(5, srcBmp);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale11::CTestStepScale11()
	{
	_LIT(KTestStep011, "MM-ICL-GSCAL-U-011-HP");
	iTestStepName = KTestStep011;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/

CTestStepScale11::~CTestStepScale11()
	{
	}

/**
*
* Do the test step.
* Test scaling while another scale is in progress. (KErrGeneral test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale11::DoTestStepL()
	{
	_LIT(KTestScale11, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-011-HP");
	Log(KTestScale11);
	
	_LIT(KTestStepScale11, "This test returns KGeneral: Only one scale operation at a time is possible.");
	Log(KTestStepScale11);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the dst bitmap.
	// Arbitrary size, valid color depth. 
	CFbsBitmap* dstBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(dstBmp);
	User::LeaveIfError(dstBmp->Create(TSize(10, 10), EColor256));

	// Create a valid palette.
	CPalette* palette = CPalette::NewL(256);
	CleanupStack::PushL(palette);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *dstBmp, *palette);

	gifScaler->Scale(&listener->iStatus, *dstBmp, *palette);
	CActiveScheduler::Start();

	if (listener->iStatus != KErrGeneral)
		iTestStepResult = EFail;

	CleanupStack::PopAndDestroy(5, srcBmp);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale12::CTestStepScale12()
	{
	_LIT(KTestStep012, "MM-ICL-GSCAL-U-012-HP");
	iTestStepName = KTestStep012;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/

CTestStepScale12::~CTestStepScale12()
	{
	}

/**
*
* Do the test step.
* Test cancelling scaling. (KErrCancel test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale12::DoTestStepL()
	{
	_LIT(KTestScale12, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-012-HP");
	Log(KTestScale12);
	
	_LIT(KTestStepScale12, "This test returns KErrCancel: Cancelling a scale should return KErrCancel.");
	Log(KTestStepScale12);
		
	// Create the src bitmap.
	// Use an arbitrary size and color depth, because we are not going to be scaling it.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));

	// Create the dst bitmap.
	// Arbitrary size, valid color depth. 
	CFbsBitmap* dstBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(dstBmp);
	User::LeaveIfError(dstBmp->Create(TSize(10, 10), EColor256));

	// Create a valid palette.
	CPalette* palette = CPalette::NewL(256);
	CleanupStack::PushL(palette);

	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *dstBmp, *palette);
	gifScaler->Cancel();
	CActiveScheduler::Start();

	if (listener->iStatus != KErrCancel)
		iTestStepResult = EFail;

	CleanupStack::PopAndDestroy(5, srcBmp);
	return iTestStepResult;
	}

	
/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale13::CTestStepScale13()
	{
	// Store the name of this test case
	// this is the name that is used by the script file.
	_LIT(KTestStep013, "MM-ICL-GSCAL-U-013-HP");
	iTestStepName = KTestStep013;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale13::~CTestStepScale13()
	{
	}

/**
*
* Do the test step.
* Scale a bitmap. (8bpp destination + palette)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale13::DoTestStepL()
	{
	_LIT(KTestScale13, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-013-HP");
	Log(KTestScale13);
	
	_LIT(KTestStepScale13, "This test checks scaling a bitmap to generate an 8bpp bitmap + palette.");
	Log(KTestStepScale13);
	
	// Load the source image.
	// png2_2.png is a suitable image for scaling using the gif scaler.
	// It is an image with many colors and contains a grayscale mask.
	_LIT(KImageTestSourceFileName, "png2_2.png");
	SetTestFileNameL(KImageTestSourceFileName);
	
	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Create the src bitmaps.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	// Load the test image.
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, FileName());
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(srcBmp->Create(frameInfo.iOverallSizeInPixels, EColor16M));
	
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *srcBmp);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder);
		
	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	// Create the target bitmap.
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(100, 76), EColor256));

	// Create the palette.
	CPalette* tgtPalette = CPalette::NewL(256);
	CleanupStack::PushL(tgtPalette);
	
	// Scale the source.
	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
	CActiveScheduler::Start();

	// Create the final bitmap.
	CFbsBitmap* finalBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(finalBmp);
	User::LeaveIfError(finalBmp->Create(tgtBmp->SizeInPixels(), EColor16MU));

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Compare the bitmap with the ref file.
	_LIT(KImageTestRefFileName, "png2_2_8p.mbm");
	SetTestFileNameL(KImageTestRefFileName);

	iTestStepResult = CheckBitmapL(*finalBmp);
	if (iTestStepResult == EFail)
		{
		CleanupStack::PopAndDestroy(6, listener);
		return iTestStepResult;
		}

	// Repeat the test, scaling to a small size bitmap.
	// Using a small bitmap causes small primes to be used for the quantization.
	User::LeaveIfError(tgtBmp->Resize(TSize(22, 22)));
	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
	CActiveScheduler::Start();
	User::LeaveIfError(finalBmp->Resize(tgtBmp->SizeInPixels()));

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Compare the bitmap with the ref file.
	_LIT(KImageTestRefSmallFileName, "png2_2_8pSmall.mbm");
	SetTestFileNameL(KImageTestRefSmallFileName);

	iTestStepResult = CheckBitmapL(*finalBmp);
	if (iTestStepResult == EFail)
		{
		CleanupStack::PopAndDestroy(6, listener);
		return iTestStepResult;
		}

	// Repeat the test, scaling to a smaller bitmap.
	// Using a smaller bitmap causes the fast color quantizer to be used.
	User::LeaveIfError(tgtBmp->Resize(TSize(18, 18)));
	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
	CActiveScheduler::Start();
	User::LeaveIfError(finalBmp->Resize(tgtBmp->SizeInPixels()));

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Compare the bitmap with the ref file.
	_LIT(KImageTestRefThmbFileName, "png2_2_8pThmb.mbm");
	SetTestFileNameL(KImageTestRefThmbFileName);

	iTestStepResult = CheckBitmapL(*finalBmp);
	CleanupStack::PopAndDestroy(6, listener);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale14::CTestStepScale14()
	{
	// Store the name of this test case
	// this is the name that is used by the script file.
	_LIT(KTestStep014, "MM-ICL-GSCAL-U-014-HP");
	iTestStepName = KTestStep014;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CTestStepScale14::~CTestStepScale14()
	{
	}

/**
*
* Do the test step.
* Scale a bitmap. (8bpp destination + palette + transparency)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CTestStepScale14::DoTestStepL()
	{
	_LIT(KTestScale14, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-014-HP");
	Log(KTestScale14);
	
	_LIT(KTestStepScale14, "This test checks scaling a bitmap to generate an 8bpp bitmap + palette + transparency.");
	Log(KTestStepScale14);
	
	// Load the source image.
	// png2_2.png is a suitable image for scaling using the gif scaler.
	// It is an image with many colors and contains a grayscale mask.
	_LIT(KImageTestSourceFileName, "png2_2.png");
	SetTestFileNameL(KImageTestSourceFileName);
	
	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Create the src bitmaps.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);

	// Load the test image.
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, FileName());
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(srcBmp->Create(frameInfo.iOverallSizeInPixels, EColor16M));
	User::LeaveIfError(srcMaskBmp->Create(frameInfo.iOverallSizeInPixels, EGray256));
	
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *srcBmp, *srcMaskBmp);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder);
		
	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp);
	CleanupStack::PushL(gifScaler);

	// Create the target bitmap.
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(100, 76), EColor256));

	// Create the palette.
	CPalette* tgtPalette = CPalette::NewL(256);
	CleanupStack::PushL(tgtPalette);
	
	// Scale the source. (Default transparency value)
	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
	CActiveScheduler::Start();
	
	// Create the final bitmap.
	CFbsBitmap* finalBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(finalBmp);
	User::LeaveIfError(finalBmp->Create(tgtBmp->SizeInPixels(), EColor16MU));

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Compare the bitmap with the ref file.
	_LIT(KImageTestRefFileName, "png2_2_8pm.mbm");
	SetTestFileNameL(KImageTestRefFileName);

	iTestStepResult = CheckBitmapL(*finalBmp);
	if (iTestStepResult == EFail)
		{
		CleanupStack::PopAndDestroy(7, listener);
		return iTestStepResult;
		}

	// Repeat the test, scaling to a smaller bitmap.
	// Using a smaller bitmap causes the fast color quantizer to be used.
	User::LeaveIfError(tgtBmp->Resize(TSize(18, 18)));

	listener->InitialiseActiveListener();
	gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
	CActiveScheduler::Start();
	User::LeaveIfError(finalBmp->Resize(tgtBmp->SizeInPixels()));

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Compare the bitmap with the ref file.
	_LIT(KImageTestRefThmbFileName, "png2_2_8pmThmb.mbm");
	SetTestFileNameL(KImageTestRefThmbFileName);

	iTestStepResult = CheckBitmapL(*finalBmp);
	if (iTestStepResult == EFail)
		{
		CleanupStack::PopAndDestroy(7, listener);
		return iTestStepResult;
		}

	// Repeat the test, using a different, lower than the default transpareny threshold.
	// This tests that the transparency threshold is correctly being used.

	// Reset bmp sizes.
	User::LeaveIfError(tgtBmp->Resize(TSize(100, 76)));

	listener->InitialiseActiveListener();
	gifScaler->ThresholdScale(&listener->iStatus, *tgtBmp, *tgtPalette, 16);
	CActiveScheduler::Start();
	User::LeaveIfError(finalBmp->Resize(tgtBmp->SizeInPixels()));

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Apply the palette
	ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);

	// Compare the bitmap with the ref file.
	_LIT(KImageTestRefThrshFileName, "png2_2_8pmThrsh.mbm");
	SetTestFileNameL(KImageTestRefThrshFileName);

	iTestStepResult = CheckBitmapL(*finalBmp);
	CleanupStack::PopAndDestroy(7, listener);
	return iTestStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CAllocTestStepScale1::CAllocTestStepScale1()
	{
	// Store the name of this test case
	// this is the name that is used by the script file
	_LIT(KTestStep101, "MM-ICL-GSCAL-U-101-HP");
	iTestStepName = KTestStep101;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CAllocTestStepScale1::~CAllocTestStepScale1()
	{
	}

/**
*
* Do the test step.
* Create a gif scaler under alloc testing conditions.
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/

TVerdict CAllocTestStepScale1::DoTestStepL()
	{
	#ifndef _DEBUG
	User::Panic(KDebugTestOnly, 0);
	#endif

	_LIT(KTestScale101, "GifScaler: Scale Bitmap and Mask Test MM-ICL-GSCAL-U-101-HP");
	Log(KTestScale101);
	
	_LIT(KTestStepScale101, "This test checks creating a gif scaler under alloc testing conditions (Alloc test)");
	Log(KTestStepScale101);
	
	// Create the src bitmaps.
	// Use arbitrary sizes as we are not going to do the scaling.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Create(TSize(10, 10), EColor16M));
	
	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);
	User::LeaveIfError(srcMaskBmp->Create(TSize(10, 10), EGray2));

	// Create the gif scaler (src bitmap only.
	CGifScaler* gifScaler = NULL;
	
	TInt err=KErrNone;
	TInt i;
	for (i = 1; ;i++)
		{
		__MM_HEAP_MARK;
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		TRAP(err, gifScaler = CGifScaler::NewL(*srcBmp));
		delete gifScaler;
		CleanupStack::PopAndDestroy(fbsSession);
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		
		}

	// Create the gif scaler.
	gifScaler = NULL;
	for (i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		TRAP(err, gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp));
		delete gifScaler;
		CleanupStack::PopAndDestroy(fbsSession);
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}
	
	CleanupStack::PopAndDestroy(2, srcBmp); //srcMaskBmp, srcBmp
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CAllocTestStepScale2::CAllocTestStepScale2()
	{
	// Store the name of this test case
	// this is the name that is used by the script file
	_LIT(KTestStep102, "MM-ICL-GSCAL-U-102-HP");
	iTestStepName = KTestStep102;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CAllocTestStepScale2::~CAllocTestStepScale2()
	{
	}

/**
*
* Do the test step.
* Scale a bitmap. (8bpp destination + alloc test)
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CAllocTestStepScale2::DoTestStepL()
	{
	#ifndef _DEBUG
	User::Panic(KDebugTestOnly, 0);
	#endif

	_LIT(KTestScale102, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-102-HP");
	Log(KTestScale102);
	
	_LIT(KTestStepScale102, "This test checks scaling a bitmap, and then cancelling the request under allocing testing conditions.");
	Log(KTestStepScale102);
	
	// Load the source image.
	// png2_2.png is a suitable image for scaling using the gif scaler.
	// It is an image with many colors and contains a grayscale mask.
	_LIT(KImageTestSourceFileName, "png2_2.png");
	SetTestFileNameL(KImageTestSourceFileName);
	
	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Create the src bitmap.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	// Load the test image.
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, FileName());
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(srcBmp->Create(frameInfo.iOverallSizeInPixels, EColor16M));
	
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *srcBmp);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder);
		
	// Create the target bitmap.
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(100, 76), EColor256));

	// Create the palette.
	CPalette* tgtPalette = CPalette::NewL(256);
	CleanupStack::PushL(tgtPalette);

	// Create the gif scaler.
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	// Scale the source and cancel.
	for (TInt i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		CICLFbsSessionTest* tempFbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		// Reset bmp sizes.
		User::LeaveIfError(tgtBmp->Resize(TSize(100, 76)));
		
		// Do the scaling.
		listener->InitialiseActiveListener();
		gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
		gifScaler->Cancel();
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(tempFbsSession);
		__MM_HEAP_MARKEND;

		TInt err = listener->iStatus.Int();
		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}

	if (!listener->RequestCancelled())
		{
		iTestStepResult = EFail;

		CleanupStack::PopAndDestroy(6, listener);
		return iTestStepResult;
		}

	CleanupStack::PopAndDestroy(2, fbsSession); //gifScaler, fbsSession
	__MM_HEAP_MARKEND;

	CleanupStack::PopAndDestroy(4, listener);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CAllocTestStepScale3::CAllocTestStepScale3()
	{
	// Store the name of this test case
	// this is the name that is used by the script file
	_LIT(KTestStep103, "MM-ICL-GSCAL-U-103-HP");
	iTestStepName = KTestStep103;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CAllocTestStepScale3::~CAllocTestStepScale3()
	{
	}

/**
*
* Do the test step.
* Scale a bitmap under alloc testing conditions.
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CAllocTestStepScale3::DoTestStepL()
	{
	#ifndef _DEBUG
	User::Panic(KDebugTestOnly, 0);
	#endif

	_LIT(KTestScale103, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-103-HP");
	Log(KTestScale103);
	
	_LIT(KTestStepScale103, "This test checks scaling a bitmap under alloc testing conditions.");
	Log(KTestStepScale103);
	
	// Load the source image.
	// png2_2.png is a suitable image for scaling using the gif scaler.
	// It is an image with many colors and contains a grayscale mask.
	_LIT(KImageTestSourceFileName, "png2_2.png");
	SetTestFileNameL(KImageTestSourceFileName);
	
	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Create the src bitmaps.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	// Load the test image.
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, FileName());
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(srcBmp->Create(frameInfo.iOverallSizeInPixels, EColor16M));
	
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *srcBmp);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder);
		
	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
	CleanupStack::PushL(gifScaler);

	// Create the target bitmap.
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(100, 76), EColor256));

	// Create the palette.
	CPalette* tgtPalette = CPalette::NewL(256);
	CleanupStack::PushL(tgtPalette);

	// Create the final bitmap.
	CFbsBitmap* finalBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(finalBmp);
	
	// Set up the ref file to compare with.
	_LIT(KImageTestRefFileName, "png2_2_8p.mbm");
	SetTestFileNameL(KImageTestRefFileName);

	// Scale the source.
	TInt i;
	TInt err=KErrNone;
	for (i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		// Reset bmp sizes.
		User::LeaveIfError(tgtBmp->Resize(TSize(100, 76)));

		listener->InitialiseActiveListener();
		gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(fbsSession); // RFbsSession::Disconnect()
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		err = listener->iStatus.Int();
		if (err == KErrNone)
			{
			__UHEAP_RESET;
			User::LeaveIfError(finalBmp->Create(tgtBmp->SizeInPixels(), EColor16MU));
			ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);
			iTestStepResult = CheckBitmapL(*finalBmp);
			if (iTestStepResult == EFail)
				{
				CleanupStack::PopAndDestroy(6, listener);
				return iTestStepResult;
				}
			}

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}

	// Repeat the test, scaling to a small size bitmap.
	// Using a small bitmap causes small primes to be used for the quantization.

	// Set up the ref file to compare with.
	_LIT(KImageTestRefSmallFileName, "png2_2_8pSmall.mbm");
	SetTestFileNameL(KImageTestRefSmallFileName);

	for (i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		// Connect to RFbsSession
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		// Reset bmp sizes.
		User::LeaveIfError(tgtBmp->Resize(TSize(22, 22)));

		listener->InitialiseActiveListener();
		gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(fbsSession); // RFbsSession::Disconnect()
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		err = listener->iStatus.Int();
		if (err == KErrNone)
			{
			__UHEAP_RESET;
			User::LeaveIfError(finalBmp->Resize(tgtBmp->SizeInPixels()));
			ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);
			iTestStepResult = CheckBitmapL(*finalBmp);
			if (iTestStepResult == EFail)
				{
				CleanupStack::PopAndDestroy(6, listener);
				return iTestStepResult;
				}
			}

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}

	// Repeat the test, scaling to a smaller bitmap.
	// Using a smaller bitmap causes the fast color quantizer to be used.
	// Set up the ref file to compare with.
	_LIT(KImageTestRefThmbFileName, "png2_2_8pThmb.mbm");
	SetTestFileNameL(KImageTestRefThmbFileName);

	for (i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		// Connect to RFbsSession
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		// Reset bmp sizes.
		User::LeaveIfError(tgtBmp->Resize(TSize(18, 18)));

		listener->InitialiseActiveListener();
		gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(fbsSession); // RFbsSession::Disconnect()
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		err = listener->iStatus.Int();
		if (err == KErrNone)
			{
			__UHEAP_RESET;
			User::LeaveIfError(finalBmp->Resize(tgtBmp->SizeInPixels()));
			ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);
			iTestStepResult = CheckBitmapL(*finalBmp);
			if (iTestStepResult == EFail)
				{
				CleanupStack::PopAndDestroy(6, listener);
				return iTestStepResult;
				}
			}

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}

	CleanupStack::PopAndDestroy(6, listener);
	return iTestStepResult;
	}


/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CAllocTestStepScale4::CAllocTestStepScale4()
	{
	// Store the name of this test case
	// this is the name that is used by the script file
	_LIT(KTestStep104, "MM-ICL-GSCAL-U-104-HP");
	iTestStepName = KTestStep104;
	
	// Increase the heap size for this test
	iHeapSize = KHeapSize;
	}

/**
*
* Test step destructor.
* 
*/
CAllocTestStepScale4::~CAllocTestStepScale4()
	{
	}

/**
*
* Do the test step.
* Scale a bitmap and its mask.
* @return	"TVerdict" the status of the test
*			 The result of the test step 
*/
TVerdict CAllocTestStepScale4::DoTestStepL()
	{
	#ifndef _DEBUG
	User::Panic(KDebugTestOnly, 0);
	#endif

	_LIT(KTestScale104, "GifScaler: Scale Bitmap Test MM-ICL-GSCAL-U-104-HP");
	Log(KTestScale104);
	
	_LIT(KTestStepScale104, "This test checks scaling a bitmap and mask under alloc testing conditions.");
	Log(KTestStepScale104);
	
	// Load the source image.
	// png2_2.png is a suitable image for scaling using the gif scaler.
	// It is an image with many colors and contains a grayscale mask.
	_LIT(KImageTestSourceFileName, "png2_2.png");
	SetTestFileNameL(KImageTestSourceFileName);
	
	// Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Create the src bitmaps.
	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);

	CFbsBitmap* srcMaskBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcMaskBmp);

	// Load the test image.
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, FileName());
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(srcBmp->Create(frameInfo.iOverallSizeInPixels, EColor16M));
	User::LeaveIfError(srcMaskBmp->Create(frameInfo.iOverallSizeInPixels, EGray256));
	
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *srcBmp, *srcMaskBmp);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder);
		
	// Create the gif scaler.
	CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp, *srcMaskBmp);
	CleanupStack::PushL(gifScaler);

	// Create the target bitmap.
	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(100, 76), EColor256));

	// Create the palette.
	CPalette* tgtPalette = CPalette::NewL(256);
	CleanupStack::PushL(tgtPalette);
	
	// Create the final bitmap.
	CFbsBitmap* finalBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(finalBmp);
			
	// Set up the ref file to compare with.
	_LIT(KImageTestRefFileName, "png2_2_8pm.mbm");
	SetTestFileNameL(KImageTestRefFileName);

	// Scale the source.
	TInt i;
	TInt err=KErrNone;
	for (i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		// Connect to RFbsSession
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		// Reset bmp sizes.
		User::LeaveIfError(tgtBmp->Resize(TSize(100, 76)));

		listener->InitialiseActiveListener();
		gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(fbsSession); // RFbsSession::Disconnect()
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		err = listener->iStatus.Int();
		if (err == KErrNone)
			{
			__UHEAP_RESET;
			User::LeaveIfError(finalBmp->Create(tgtBmp->SizeInPixels(), EColor16MU));
			ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);
			iTestStepResult = CheckBitmapL(*finalBmp);
			if (iTestStepResult == EFail)
				{
				CleanupStack::PopAndDestroy(7, listener);
				return iTestStepResult;
				}
			}

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}

	// Repeat the test, using a different, lower than the default transpareny threshold.
	// This tests that the transparency threshold is correctly being used.

	// Set up the ref file to compare with.
	_LIT(KImageTestRefThrshFileName, "png2_2_8pmThrsh.mbm");
	SetTestFileNameL(KImageTestRefThrshFileName);

	for (i = 1 ; ; i++)
		{
		__MM_HEAP_MARK;
		// Connect to RFbsSession
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
		__UHEAP_SETFAIL(RHeap::EFailNext, i);

		// Reset bmp sizes.
		User::LeaveIfError(tgtBmp->Resize(TSize(100, 76)));

		listener->InitialiseActiveListener();
		gifScaler->ThresholdScale(&listener->iStatus, *tgtBmp, *tgtPalette, 16);
		CActiveScheduler::Start();
		CleanupStack::PopAndDestroy(fbsSession); // RFbsSession::Disconnect()
		__MM_HEAP_MARKEND;

		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL);
		User::Free(testAlloc);

		err = listener->iStatus.Int();
		if (err == KErrNone)
			{
			__UHEAP_RESET;
			User::LeaveIfError(finalBmp->Create(tgtBmp->SizeInPixels(), EColor16MU));
			ApplyPaletteL(*tgtBmp, *tgtPalette, *finalBmp);
			iTestStepResult = CheckBitmapL(*finalBmp);
			if (iTestStepResult == EFail)
				{
				CleanupStack::PopAndDestroy(7, listener);
				return iTestStepResult;
				}
			}

		if ((err != KErrNoMemory) && heapTestingComplete)
			break;
		}

	CleanupStack::PopAndDestroy(7, listener);
	return iTestStepResult;
	}

/**
*
* Test step constructor.
* Each test step initialises its own name.
* @lib "TSU_ICL_GSCAL.lib" 
*
*/

CTestStepScale15::CTestStepScale15()
    {
    // Store the name of this test case
    // this is the name that is used by the script file.
    _LIT(KTestStep013, "MM-ICL-GSCAL-U-015-HP");
    iTestStepName = KTestStep013;
    
    // Increase the heap size for this test
    iHeapSize = KHeapSize;
    }

/**
*
* Test step destructor.
* 
*/
CTestStepScale15::~CTestStepScale15()
    {
    }

/**
*
* Do the test step.
* Scale a bitmap. (8bpp destination + palette)
* @return   "TVerdict" the status of the test
*            The result of the test step 
*/
TVerdict CTestStepScale15::DoTestStepL()
    {
    _LIT(KTestScale15, "GifScaler: Scale Extended Bitmap Test MM-ICL-GSCAL-U-015-HP");
    Log(KTestScale15);
    
    _LIT(KTestStepScale15, "This test checks scaling an extended bitmap. KErrArgument must be returned");
    Log(KTestStepScale15);
    
    // Load the source image.
    // png2_2.png is a suitable image for scaling using the gif scaler.
    // It is an image with many colors and contains a grayscale mask.
    _LIT(KImageTestSourceFileName, "png2_2.png");
    SetTestFileNameL(KImageTestSourceFileName);
    
    // Create an Active listener.
    CActiveListener* listener = new(ELeave) CActiveListener;
    CleanupStack::PushL(listener);
    
    // Create the src bitmaps.
    CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
    CleanupStack::PushL(srcBmp);

    // Load the test image.
    CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, FileName());
    CleanupStack::PushL(decoder);
    
    const TFrameInfo& frameInfo = decoder->FrameInfo(0);
    User::LeaveIfError(srcBmp->Create(frameInfo.iOverallSizeInPixels, EColor16M));
    
    listener->InitialiseActiveListener();
    decoder->Convert(&listener->iStatus, *srcBmp);
    CActiveScheduler::Start();

    User::LeaveIfError(listener->iStatus.Int());
    CleanupStack::PopAndDestroy(decoder);
        
    // Create the gif scaler.
    CGifScaler* gifScaler = CGifScaler::NewL(*srcBmp);
    CleanupStack::PushL(gifScaler);

    // Create the target bitmap.
    CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
    CleanupStack::PushL(tgtBmp);
    User::LeaveIfError(tgtBmp->Create(TSize(100, 76), EColor256));
    User::LeaveIfError(tgtBmp->CreateExtendedBitmap(
            TSize(100, 76),
                EColor256,KImageTypeJPGUid,29143,*this));

    // Create the palette.
    CPalette* tgtPalette = CPalette::NewL(256);
    CleanupStack::PushL(tgtPalette);
    
    // Scale the source.
    listener->InitialiseActiveListener();
    gifScaler->Scale(&listener->iStatus, *tgtBmp, *tgtPalette);
    CActiveScheduler::Start();
    
    TInt err = listener -> iStatus.Int();
   if (err == KErrNone)
       {
       INFO_PRINTF1(_L("Expected KErrNotSupported by now. Scale did not return any error."));
      
       CleanupStack::PopAndDestroy(5);
       User::Leave(KErrGeneral);
       }
   else
       {
       if(err== KErrNotSupported)
           {
           INFO_PRINTF1(_L("Got KErrNotSupported as expected"));
           }
       else
           {
           
           CleanupStack::PopAndDestroy(5);
           INFO_PRINTF1(_L("Unexpected error"));
           User::Leave(err);
           }
       }
   
   //Now try to pass extended bitmap as source
   // Create the gif scaler.
   INFO_PRINTF1(_L("Trying to create a gifScaler with extended bitmap"));
   CGifScaler* gifScaler2=NULL;
   TRAP(err, gifScaler2=CGifScaler::NewL(*tgtBmp));
   
   //dont need gifscaler2 anymore
   if(gifScaler2)
       {
       delete gifScaler2;
       }
   
   if (err == KErrNone)
      {
      INFO_PRINTF1(_L("Expected KErrNotSupported by now. NewL did not return any error."));
     
      CleanupStack::PopAndDestroy(5);
      User::Leave(KErrGeneral);
      }
  else
      {
      if(err== KErrNotSupported)
          {
          INFO_PRINTF1(_L("Got KErrNotSupported as expected"));
          }
      else
          {
          
          CleanupStack::PopAndDestroy(5);
          INFO_PRINTF1(_L("Unexpected error"));
          User::Leave(err);
          }
      }
   
   
    CleanupStack::PopAndDestroy(5);
    return iTestStepResult;
    }

TInt CTestStepScale15::InitExtendedBitmap(TAny* /*aData*/, TInt /*aDataSize*/)
    {
    return KErrNone;
    }

