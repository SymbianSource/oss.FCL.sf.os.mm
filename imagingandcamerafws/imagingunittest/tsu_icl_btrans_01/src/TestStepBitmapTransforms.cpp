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

#include "TestStepBitmapTransforms.h"
#include "TestSuiteBitmapTransforms.h"

//_LIT(KImageTestSourceScaledFileName,"ref\\scaled");	// EABI warning removal
//_LIT(KImageTestRefExt,".mbm");						// EABI warning removal

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

TInt CActiveListener::Result() const
	{
	return iStatus.Int();
	}

/**
*
* CTestStepBitmapTransforms constructor 
*
* 
*/
CTestStepBitmapTransforms::CTestStepBitmapTransforms(TBool aConnectToFbs) 
:  iBitmapTransformsSuite(NULL), 
iSourceFileName(),
iScheduler( NULL ),
iConnectToFbs(aConnectToFbs)
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
* @lib "TS_BitmapTransforms.lib"
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

@lib "TS_BitmapTransforms.lib"
*/
TVerdict CTestStepBitmapTransforms::DoTestStepPreambleL()
	{
	//[ mark the heap and unmark at the end of postamble ]
	__MM_HEAP_MARK;

	if (iConnectToFbs)
		{
	        User::LeaveIfError(FbsStartup());
		// [ connect to the bitmap server and check the error code]
		TInt errCode = RFbsSession::Connect();
		if( errCode != KErrNone )
			{
			//[ log failed to connect to bitmap server ]
			Log(_L("Failed to connect to bitmap server in teststep preamble = %d"), errCode );
			return EFail;
			}
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
@lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestStepBitmapTransforms::DoTestStepPostambleL()
	{
	
	//[ Destroy the scheduler ]
	delete iScheduler ;
	iScheduler = NULL;
	
	if (iConnectToFbs)
		{
		// [ since we are good citizens let's disconnect from the bitmap server]
		RFbsSession::Disconnect();
		}

    // [ umark the heap which was marfked in preamble ]
	__MM_HEAP_MARKEND;

	return EPass;
	}


/**
*
* CTestStepBitmapTransforms Suite accessor function
* @returns CTestSuiteBitmapTransforms*  
* @lib "TS_BitmapTransforms.lib"
*/

CTestSuiteBitmapTransforms* CTestStepBitmapTransforms::Suite()
	{
	return iBitmapTransformsSuite;
	}


/**
*
* CTestStepBitmapTransforms SetTestFileName
* This method adds the filename to include the default path
* @param aPathNameAndExtn const TDesC&  afull file and path name
* @returns void
* @lib "TS_BitmapTransforms.lib"
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
* @param srcBmp the src bmp
* @param rgb    the rgb colour
* @returns void
* @lib "TS_BitmapTransforms.lib"
*/

void CTestStepBitmapTransforms::FillBitmapL( CFbsBitmap& srcBmp, const TRgb& rgb )
	{
	TInt err = KErrNone ;
	
    CFbsBitmapDevice* device = NULL;
	
	// [ create a bitmap device ]
	TRAP(err, device = CFbsBitmapDevice::NewL( &srcBmp ));
	User::LeaveIfError( err );
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
* @returns TVerdict
* @lib "TS_BitmapTransforms.lib"
* @leaves memory allocation failure
* @leaves failure to load the current file
* @returns TVerdict which is either EPass or EFail. The bitmaps are equivalent if
*         the result is Epass
* @lib "TS_BitmapTransforms.lib"
*/

TVerdict CTestStepBitmapTransforms::CheckBitmapL(CFbsBitmap& aBitmap )
	{
	CFbsBitmap *referenceScaledBitmap = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(referenceScaledBitmap);
	
	// [ load the current bitmap pointed to by filename into memory]
	User::LeaveIfError(referenceScaledBitmap->Load( FileName()));
	
	//[ check against the supplied bitmap ]
	TVerdict result = CheckBitmaps(aBitmap,*referenceScaledBitmap, -1);
	
	//[ pop referenceScaledBitmap]
	CleanupStack::PopAndDestroy( referenceScaledBitmap ); 
	return result;	
	}


/**
*
* CTestStepBitmapTransforms CheckBitmaps
* This function checks that two bitmaps are equivalent
* where the one has been derived from the other by a rotate/mirror transformation
*@param aBitmap1 first bitmap for comparison
*@param aBitmap2 secon bitmap for comparison
*@param aQuarterTurns determines the transfromation for equivalence
*       This can be -1 for 1 to 1 comparison or the enumeration
*       CBitmapRotator::
*@returns TVerdict which is either EPass or EFail. The bitmaps are equivalent if
*         the result is Epass
*/

TVerdict CTestStepBitmapTransforms::CheckBitmaps(CFbsBitmap& aBitmap1,CFbsBitmap& aBitmap2,
													TInt aQuarterTurns, TBool aCheckAllWhite)
	{
	TVerdict result = EFail ;
	if (aCheckAllWhite)
		{
		TBool allWhite=ETrue;
		TPoint p;
		for (p.iY=aBitmap2.SizeInPixels().iHeight; allWhite && --p.iY; )
			{
			for (p.iX=aBitmap2.SizeInPixels().iWidth; --p.iX; )
				{
				TRgb c;
				aBitmap2.GetPixel(c,p);
				if (c.Internal() != 0xFFFFFFFF)
					{
					allWhite = EFalse;
					break;
					}
				}
			}
		if (allWhite) // bitmap is all-white, there are some problems.
			{
			return EFail;
			}
		}
	
	const TSize size1 = aBitmap1.SizeInPixels();
	TRgb color1 = 0;
	TRgb color2 = 0;	
	TInt x = 0;
	TInt y = 0;
	if (aQuarterTurns == -1)
		{
		
		//For images without any rotation
		const TSize size2 = aBitmap2.SizeInPixels();
		if(size1 != size2)
			{
			return result;
			}
			
		for (y = 0; y < size1.iHeight; y++)
			{
			for (x = 0; x < size1.iWidth; x++)
				{
				TPoint pt(x,y);
				aBitmap1.GetPixel(color1,pt);
				aBitmap2.GetPixel(color2,pt);
				
				if (color1 != color2)
					{
					return result;
					}
				}
			}
		}
	else
		{
		for (y = 0; y < size1.iHeight; y++)
			{
			for (x = 0; x < size1.iWidth; x++)
				{
				TPoint pt(x,y);
				aBitmap1.GetPixel(color1,pt);
				if (aQuarterTurns == CBitmapRotator::ERotation90DegreesClockwise)
					{
					aBitmap2.GetPixel(color2,TPoint(size1.iHeight - y - 1,x));
					}
				else if (aQuarterTurns == CBitmapRotator::ERotation180DegreesClockwise)
					{
					aBitmap2.GetPixel(color2,TPoint(size1.iWidth - x - 1,size1.iHeight - y - 1));
					}
				else if (aQuarterTurns == CBitmapRotator::ERotation270DegreesClockwise)
					{
					aBitmap2.GetPixel(color2,TPoint(y,size1.iWidth - x - 1));
					}
				else if ( aQuarterTurns == CBitmapRotator::EMirrorHorizontalAxis)
					{
					aBitmap2.GetPixel(color2,TPoint(x, size1.iHeight - y - 1));
					}
				else if( aQuarterTurns == CBitmapRotator::EMirrorVerticalAxis)
					{
					aBitmap2.GetPixel(color2,TPoint( size1.iWidth - x - 1, y));
					}
				else
					{
					// Should not get here !
					ASSERT( EFalse );
					}
				
				
				if (color1 != color2)
					{
					return result;
					}
				}
			}
		}
	
	result = EPass;
	return result;
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
	return  iSourceFileName;
	}


/**
*
* CTestStepBitmapTransforms RotateAndMirrorABitmap
*
* 
* 
*/
TVerdict CTestStepBitmapTransforms::RotateAndMirrorABitmap( const TFileName& aSourceFileName, CBitmapRotator::TRotationAngle aAngle, TDisplayMode aDisplayMode)
	{
	TVerdict  result = EPass ;
	
	TRAPD(err, RotateAndMirrorABitmapL(aSourceFileName, aAngle, aDisplayMode));

	// Check for errors
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("RotateAndMirrorABitmap failed, error code %d"), err);
		result = EFail;
		}
	
	return result;
	}


/**
*
* CTestStepBitmapTransforms RotateAndMirrorABitmapL
*
* 
* 
*/
void CTestStepBitmapTransforms::RotateAndMirrorABitmapL( const TFileName& aSourceFileName, CBitmapRotator::TRotationAngle aAngle, TDisplayMode aDisplayMode )
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
		User::LeaveIfError(tgtBmp->Create(TSize(0,0),aDisplayMode));
		}
	else
		{
		User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));
		}
	
	//[ create and push a rotator]
	CBitmapRotator* rotator = CBitmapRotator::NewL();
	CleanupStack::PushL(rotator);
	
	//[ rotate or mirror the bitmap through the requisite angle ]
	activeListener->InitialiseActiveListener();
	rotator->Rotate(&activeListener->iStatus,*srcBmp,*tgtBmp, aAngle );
	CActiveScheduler::Start();
	User::LeaveIfError(activeListener->iStatus.Int());
	
	//[ check the bitmap against the original]
	if (CheckBitmaps(*srcBmp,*tgtBmp, aAngle )!=EPass)
		{
		ERR_PRINTF1(_L("Result data different from that expected, leaving with KErrCorrupt"));
		User::Leave(KErrCorrupt);
		}
	
	//[ Pop and Destroy activeListener, srcBmp, tgtBmp, rotator]
	CleanupStack::PopAndDestroy(4, activeListener);
	}

/*static*/
CFbsBitmap* CTestStepBitmapTransforms::Create16MABitmapL(CFbsBitmap& aColorBmp, CFbsBitmap& aAlphaBmp)
	{
	ASSERT( aColorBmp.SizeInPixels() == aAlphaBmp.SizeInPixels() );
	ASSERT( aAlphaBmp.DisplayMode() == EGray256);
	ASSERT( aColorBmp.DisplayMode() == EColor16M || aColorBmp.DisplayMode() == EColor16MU);
	
	CFbsBitmap* rgba	=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(rgba);
	
	User::LeaveIfError( rgba->Create(aColorBmp.SizeInPixels(), EColor16MA) );
	
	TInt len=rgba->ScanLineLength(aColorBmp.SizeInPixels().iWidth, EColor16MA);
	TUint32* scanLine= new (ELeave) TUint32[ Align4(len)/4 ];
	CleanupArrayDeletePushL( scanLine );
	
	for (TInt y=0; y<aColorBmp.SizeInPixels().iHeight; y++)
		{
		for (TInt x=0; x<aColorBmp.SizeInPixels().iWidth; x++)
			{
			TRgb rgbC, a;
			TPoint pnt(x,y);
			aColorBmp.GetPixel(rgbC, pnt);
			aAlphaBmp.GetPixel(a, pnt);
			rgbC.SetAlpha(a.Gray256());
			scanLine[x]=rgbC.Internal();
			}
		TPtr8 ptr(reinterpret_cast<TText8*>(scanLine), len, len);
		rgba->SetScanLine(ptr, y);
		}
	CleanupStack::PopAndDestroy();
	CleanupStack::Pop(rgba);
	return rgba;
	}

/*static*/
void CTestStepBitmapTransforms::Split16MABitmapL(const CFbsBitmap& a16MABmp, CFbsBitmap*& aColorBmp, CFbsBitmap*& aAlphaBmp)
	{
	aColorBmp = aAlphaBmp = NULL;
	aColorBmp		=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(aColorBmp);
	User::LeaveIfError( aColorBmp->Create( a16MABmp.SizeInPixels(), EColor16M) );
	
	aAlphaBmp	=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(aAlphaBmp);
	User::LeaveIfError( aAlphaBmp->Create(a16MABmp.SizeInPixels(), EGray256) );
	
	TInt a_len=a16MABmp.ScanLineLength(a16MABmp.SizeInPixels().iWidth, a16MABmp.DisplayMode());
	TUint32* alScanLine= new (ELeave) TUint32[ Align4(a_len)/4 ];
	CleanupArrayDeletePushL( alScanLine );
	
	HBufC8* rgbScan=HBufC8::NewMaxLC( aColorBmp->ScanLineLength(aColorBmp->SizeInPixels().iWidth, aColorBmp->DisplayMode()) );
	HBufC8* grScan=HBufC8::NewMaxLC( aAlphaBmp->ScanLineLength(aAlphaBmp->SizeInPixels().iWidth, aAlphaBmp->DisplayMode()) );
	for (TInt y=0; y<a16MABmp.SizeInPixels().iHeight; ++y)
		{
		TPtr8 ptr(reinterpret_cast<TText8*>(alScanLine), a_len, a_len);
		a16MABmp.GetScanLine(ptr, TPoint(0,y), a16MABmp.SizeInPixels().iWidth, a16MABmp.DisplayMode() );
		TText8* a_ptr=const_cast<TText8*>(grScan->Des().Ptr());
		TText8* r_ptr=const_cast<TText8*>(rgbScan->Des().Ptr());
		for (TInt x=0; x<a16MABmp.SizeInPixels().iWidth; ++x)
			{
			TRgb col;
			col.SetInternal( alScanLine[x] );
			a_ptr[x] = alScanLine[x]>>24;
			r_ptr[0] = col.Blue();
			r_ptr[1] = col.Green();
			r_ptr[2] = col.Red();
			r_ptr+=3;
			}
		TPtr8 p1(rgbScan->Des());
		aColorBmp->SetScanLine(p1, y);
		TPtr8 p2(grScan->Des());
		aAlphaBmp->SetScanLine(p2, y);
		}

	CleanupStack::PopAndDestroy(3);
	CleanupStack::Pop(2, aColorBmp);
	}

/*static*/
CFbsBitmap* CTestStepBitmapTransforms::LoadAndCreate16MABitmapL(const TDesC& aRgb, const TDesC& aAlpha)
	{
	CFbsBitmap* rgb		=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(rgb);
	User::LeaveIfError( rgb->Create(TSize(1,1), EColor16M) );
	User::LeaveIfError( rgb->Load(aRgb) );
	CFbsBitmap* alpha	=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(alpha);
	User::LeaveIfError( alpha->Create(TSize(1,1), EGray256) );
	User::LeaveIfError( alpha->Load(aAlpha) );
	CFbsBitmap* bmp16Ma=Create16MABitmapL(*rgb, *alpha);
	CleanupStack::PopAndDestroy(2, rgb);
	return bmp16Ma;
	}
	
/*static*/
void CTestStepBitmapTransforms::DuplicateContentL(const CFbsBitmap& aSrcBmp, CFbsBitmap& aDestBmp)
	{
	if (	aSrcBmp.SizeInPixels() != aDestBmp.SizeInPixels() ||
			aSrcBmp.DisplayMode() != aDestBmp.DisplayMode()
		)
		{
		User::LeaveIfError(aDestBmp.Create(aSrcBmp.SizeInPixels(), aSrcBmp.DisplayMode()) );
		}
	
	TInt len=aSrcBmp.ScanLineLength(aSrcBmp.SizeInPixels().iWidth, aSrcBmp.DisplayMode() );
	TUint32* scanLine= new (ELeave) TUint32[ Align4(len)/4 ];
	for (TInt y=0; y<aSrcBmp.SizeInPixels().iHeight; y++)
		{
		TPtr8 ptr(reinterpret_cast<TText8*>(scanLine), len, len);
		aSrcBmp.GetScanLine(ptr, TPoint(0,y), aSrcBmp.SizeInPixels().iWidth, aSrcBmp.DisplayMode() );
		aDestBmp.SetScanLine(ptr, y);
		}
	delete scanLine;
	}
	
_LIT(KIclPanicCategory, "TS_BitmapTransforms");

/** 
 This function raises a panic

@param TS_BitmapTransformsMain::TS_BitmapTransformsPanic one of the several panics codes
       that may be raised by this dll
@panic  ENoSourceBitmap is raised when the src bitmap supplied to one of the api's
	    has not been created ie has a null handle

@xxxx
*/

GLDEF_C void Panic(TS_BitmapTransformsPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}
