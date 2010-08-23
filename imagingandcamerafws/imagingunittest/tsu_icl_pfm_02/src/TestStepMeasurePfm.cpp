// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TestStepMeasurePerformance.cpp
//

// EPOC includes
#include <e32base.h>
#include <ecom/ecom.h>

// Test system includes
#include <testframework.h>

#include <hal.h>
#include <dptest.h> // Required to check for demand paging.

// Specific includes for this test suite
#include "TestStepIclPfm.h"
#include "TestSuiteIclPfm.h"

// Specific includes for these test steps
#include "TestStepMeasurePfm.h"
#include "performanceparams.inl"

// Test steps and Files to be tested:
const TIclProfileTest KTestDetails[] =
	{
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16m_to16mSize},
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16m_to16maSize},
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16m_to16muSize},
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16m_to16mapSize},
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16ma_to16maSize},
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16ma_to16mapSize},
		{ _S("MM-ICL-PFM-001-LP"), EPNG, ENormal, &KPNG16ma_to16muSize},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF_to64kSize},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF_to64kSize_doff},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF_to16mSize},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF_to16muSize},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF2_to64kSize},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF2_to64kSize_doff},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF2_to16mSize},
		{ _S("MM-ICL-PFM-001-LP"), EGIF, ENormal, &KGIF2_to16muSize}

	};

/**
 This function is to scale execution time according to 
 the current hardware. 
 Currently supported: H2, H4 and emulator
 Base machine is H2
*/
TInt64 ScaleTimeByMachineL(TInt64 aExecTime, TInt aDestMedia = TPerformanceTestParams::EDstMediaRamDrive)
	{
	TInt value;
	User::LeaveIfError(HAL::Get(HAL::EMachineUid, value));
	TReal mulCoef = 1.0;
	
	switch (value)
		{
		case HAL::EMachineUid_OmapH2:
			mulCoef = 1.0;
			break;
			
		case HAL::EMachineUid_OmapH4:
			mulCoef = 0.7; // H2/H4 coef is in range 0.55<->0.66, so use 0.7
			break;
			
		case HAL::EMachineUid_Win32Emulator:
			mulCoef = 1.0;
			break;
			
		case HAL::EMachineUid_X86PC:
			mulCoef = 1.0;
			break;
			
		case HAL::EMachineUid_NE1_TB:
			mulCoef = 0.7;
			break;	
			
		case HAL::EMachineUid_OmapH6:
			mulCoef = 1.0;
			break;				
			

	         
		default:
			{
			User::Leave(KErrGeneral);
			// we're running on unknown hardware, we can't scale execution time
			// this test must be adjusted for that new hardware
			}
		}

	// Demand paging affects decode speed so account for it when measuring performance.
	// Allow another 10% to the encode/decode time.
	// Also, tests that access the MMC card need even more time.
	TUint32 flags = DPTest::ERomPaging | DPTest::ECodePaging | DPTest::EDataPaging;
	TBool demandpaging = (DPTest::Attributes() & flags);
	if (demandpaging)
		{
		if (aDestMedia == TPerformanceTestParams::EDstMediaMMCDrive)
			{
			mulCoef *= 2.8;
			}
		else
			{
			mulCoef *= 1.9;
			}
		}
	
	return TInt64(mulCoef * TReal(aExecTime));
	}

/**
  *
  * Test step constructor.
  * Each test step initialises its own name.
  *
  * 
  */

CMmIclPfmTest001::CMmIclPfmTest001()
	{
	// store the name of this test case
	_LIT( KPanicPfmTest, "MM-ICL-PFM-001-LP");
	iTestStepName = KPanicPfmTest;
	}

/**
  * Do the test step.
  * @return	"enum TVerdict"
  */

TVerdict CMmIclPfmTest001::DoTestStepL( )
	{
	REComSession& ecomSession = REComSession::OpenL();
	CleanupClosePushL(ecomSession);

	iTestStepResult = EPass;
	const TInt numTestSteps = sizeof(KTestDetails) / sizeof(TIclProfileTest);
	for(TInt i = 0; i < numTestSteps; i++)
		{
		const TIclProfileTest& testDetails = KTestDetails[i];
		iTestStepResult = DoProfileL(testDetails);//@@@ do what with it???
		if(iTestStepResult == EFail)
			{
			break;				
			}
		}

	CleanupStack::PopAndDestroy(&ecomSession);//ecomSession

	return iTestStepResult;
	}


/**
  * Do time profiling for particular file
  * This method allows us to measure the
  * total time taken for the process.
  */

TVerdict CMmIclPfmTest001::DoProfileL(const TIclProfileTest aProfileTest)
	{
	iTestStepName = aProfileTest.iTestName;
	
	TBuf<128> outBuf;
	outBuf.Format(_L("\n"));
	Log(outBuf);
	outBuf.Format(_L("---------------------------------------------"));
	Log(outBuf);

	TInt repeatCount = 1;
	TInt err=KErrNone;

	for (TInt i = 0; i < repeatCount; i++)
		{		
		TRAP(err, DecodeFileToBitmapL(aProfileTest));
		if (err!=KErrNone) // stop when we see error
			{
			outBuf.Format(_L("*** Test Failed... reason %d ***"), err);
			Log(outBuf);
			return EFail;
			}
		}
		
	return EPass;
	}



/**
  * DecodeFileToBitmapL
  * Opens the filename into a CImageDecoder* object
  * then converts to an EPOC CFbsBitmap* object
  * This method allows us to measure more precisely
  * the individual times taken for the process.
  */

void CMmIclPfmTest001::DecodeFileToBitmapL(const TIclProfileTest aProfileTest)
	{
	TBuf<128> outBuf;
		
	RBuf8 buf;	
	buf.CleanupClosePushL();
	TInt err=KErrNone;
	CImageDecoder* decoder = NULL;
	
	if (aProfileTest.iPerfTstParams->iUseMemBuffer)
	    {
	    RFile file;
	    User::LeaveIfError(file.Open(iFs, TPtrC(aProfileTest.iPerfTstParams->iSrcFileName), EFileRead) );
	    CleanupClosePushL(file);
	    TInt f_size;
	    User::LeaveIfError( file.Size(f_size));
	    buf.CreateMaxL(f_size);
	    User::LeaveIfError( file.Read(buf) );
	    CleanupStack::PopAndDestroy(&file);
	    
		TRAP(err, decoder = CImageDecoder::DataNewL(iFs, buf,aProfileTest.iPerfTstParams->iDecoderOptions));
		}
	else
	    {
		TRAP(err,decoder = CImageDecoder::FileNewL(iFs, TPtrC(aProfileTest.iPerfTstParams->iSrcFileName), aProfileTest.iPerfTstParams->iDecoderOptions));
		}
	
	User::LeaveIfError( err );
	
	CleanupStack::PushL(decoder);
	
	TSize destSize;
	// calculate destination size according to the test params
	User::LeaveIfError( decoder->ReducedSize(decoder->FrameInfo().iOverallSizeInPixels, 
												aProfileTest.iPerfTstParams->iReductionFactor, 
												destSize
											)
						);
						
	// choose a display mode for the dest. bitmap, 
	// use one provided wby the decoder if none specified.
	TDisplayMode displMode = (ENone == aProfileTest.iPerfTstParams->iDisplayMode ? 
											decoder->FrameInfo().iFrameDisplayMode :
											aProfileTest.iPerfTstParams->iDisplayMode
							);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(destSize, displMode));

	const TFileName fileName = TPtrC(aProfileTest.iPerfTstParams->iSrcFileName);
	
	outBuf.Format(_L("Filename: "));
	outBuf.AppendFormat(TPtrC(aProfileTest.iPerfTstParams->iSrcFileName));
	Log(outBuf);

	outBuf.Format(_L("Conversion Type: "));
	outBuf.AppendFormat(TPtrC(aProfileTest.iPerfTstParams->iConversionType));
	Log(outBuf);
	
	iActiveListener->InitialiseActiveListener();

	TUint beginDecode=User::FastCounter();
		
	TTime start;
	start.HomeTime();
	// get current time and initiate decoding
	
	decoder->Convert(&iActiveListener->iStatus, *bitmap);

	CActiveScheduler::Start();

	TTime end;
	end.HomeTime();
	
	TUint endDecode=User::FastCounter();

	if(iActiveListener->iStatus != KErrNone)
		{
		User::Leave(iActiveListener->iStatus.Int());
		}
	
	const TInt64 microSecDecodeTime = end.Int64() - start.Int64();
	const TUint decodeCounter = endDecode - beginDecode;
	
	outBuf.Format(_L("--Image decoded in %d microseconds %d counters"), I64INT(microSecDecodeTime), decodeCounter );
	Log(outBuf);
	
	// compare results to test params
	const TInt64 KScaledTime = ScaleTimeByMachineL(aProfileTest.iPerfTstParams->iMaxDecodeTime);
	
	if (aProfileTest.iPerfTstParams->iMaxDecodeTime && KScaledTime < microSecDecodeTime)
		{
		outBuf.Format(_L("--Desired decode time exceeded by %d microseconds"), 
						I64INT(microSecDecodeTime - KScaledTime) );
		Log(outBuf);		
		TInt err;
#if defined(__WINS__)
		outBuf.Format(_L("WARNING: That may mean performance degradation"));	
		Log(outBuf);
		outBuf.Format(_L("         check this test on a target hardware!"));
		Log(outBuf);
		err = KErrNone;
#else
		outBuf.Format(_L("ERROR: performance degradation has been detected."));				
		Log(outBuf);
		err = KErrGeneral;
#endif
		User::LeaveIfError( err );
		}	
	
	CleanupStack::PopAndDestroy(bitmap);
	CleanupStack::PopAndDestroy(decoder); 
	CleanupStack::PopAndDestroy(&buf);
	}
	
