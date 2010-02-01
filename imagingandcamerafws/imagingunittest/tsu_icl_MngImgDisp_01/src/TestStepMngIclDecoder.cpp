// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <imageconversion.h>
#include <bitdev.h>  //Defect:DEF073809
#include <bitstd.h>  //Defect:DEF073809

// Test system includes
#include "TestSuiteMngImageDisplay.h"
#include "TestStepMngImageDisplay.h"
#include "TestStepMngIclDecoder.h"

const TInt KHeapSize			= 0x100000;

const TInt KCancelDelayStep			= 1000;		// Microseconds
const TInt KCancelResponseTreshold	= 200000;	// Microseconds

const TInt KTimingCheckError		= -102;
const TInt KDecodeFlagCheckError	= -103;

class CActiveListener : public CActive
   {
public:
	CActiveListener();
	void InitialiseActiveListener();
	TRequestStatus* ReqStatus();
	TInt Result() const;	
protected:
   // From CActive
	virtual void RunL();
   };

CActiveListener::CActiveListener() : CActive(CActive::EPriorityIdle) 
	{ 
	CActiveScheduler::Add(this); 
	}
TRequestStatus* CActiveListener::ReqStatus()
	{
	return &iStatus;
	}

TInt CActiveListener::Result() const
	{
	return iStatus.Int();
	}

void CActiveListener::InitialiseActiveListener() 
	{ 
	SetActive(); 
	}

void CActiveListener::RunL() 
	{ 
	CActiveScheduler::Stop(); 
	}

// variant which properly cancels the decoder, where defined
class CActiveDecodeListener : public CActiveListener
	{
public:
	void InitialiseActiveListener(CImageDecoder* aDecoder);

protected:
// From CActive
	virtual void DoCancel();

protected:
	CImageDecoder* iDecoder; // not owned
	};
	
void CActiveDecodeListener::InitialiseActiveListener(CImageDecoder* aDecoder) 
  { 
  iDecoder = aDecoder;
  CActiveListener::InitialiseActiveListener(); 
  }

void CActiveDecodeListener::DoCancel() 
  { 
  if (iDecoder) 
	  {
	  iDecoder->Cancel(); 
	  }
	  
  }
//DEF080823 ->
	
class CActiveDecodeCancel : public CActiveListener
	{
public:
	CActiveDecodeCancel();
	void InitialiseActiveListener();
	~CActiveDecodeCancel();

protected:
// From CActive
	virtual void RunL();
	virtual void DoCancel();

public:
	CImageDecoder* iDecoder;  
	TBool iIsPassed;
	TBool iIsReadyToExit;
	};

CActiveDecodeCancel::CActiveDecodeCancel(): iIsPassed(ETrue),
											iIsReadyToExit(EFalse)
	{
	}

void CActiveDecodeCancel::InitialiseActiveListener() 
	{ 
	CActiveListener::InitialiseActiveListener(); 
	}

void CActiveDecodeCancel::RunL() 
	{ 
	iIsReadyToExit = ETrue;
	CActiveScheduler::Stop(); 
	}

void CActiveDecodeCancel::DoCancel() 
	{ 
	if (iDecoder) 
		{
		iDecoder->Cancel(); 
		}
		
	if(Result() != KErrCancel)
		{
		iIsPassed = EFalse;
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrCancel); 
		}
	}
  
CActiveDecodeCancel::~CActiveDecodeCancel()
	{
	Cancel();
	  	
  	if(iDecoder)
		{
		delete iDecoder;
		iDecoder = NULL;
		}
	}

class CTestTimer : public CTimer
   {
public:
	CTestTimer();	
	void ConstructL();
	void InitialiseActiveListener(CActiveDecodeCancel* aDecodeCancel);
protected:
   // From CActive
    virtual void DoCancel();
	virtual void RunL();
	
protected:
    CActiveDecodeCancel* iDecodeCancel;
   };
   
CTestTimer::CTestTimer(): CTimer(CActive::EPriorityIdle)
	{ 
	CActiveScheduler::Add(this); 
	}
   
void CTestTimer::ConstructL()
	{
	CTimer::ConstructL();
	}

void CTestTimer::InitialiseActiveListener(CActiveDecodeCancel* aDecodeCancel) 
    { 
	iDecodeCancel = aDecodeCancel;
    }
	
void CTestTimer::DoCancel()
	{
	CTimer::DoCancel();
	}
	
   
void CTestTimer::RunL() 
	{ 
 	iDecodeCancel->Cancel();
 	
	if(iDecodeCancel->iDecoder)
		{
		delete iDecodeCancel->iDecoder;
		iDecodeCancel->iDecoder = NULL;
		}	
	 
	CActiveScheduler::Stop();
    }
//DEF080823 <-

CTestStepMngIclDecoder::~CTestStepMngIclDecoder()
	{
	delete iActListener;
	iActListener = NULL;
	if(iIclDecoder)
		{
		delete iIclDecoder;
		iIclDecoder = NULL;
		}
	delete iFrame;
	delete iMask;
	}



CTestStepMngIclDecoder::CTestStepMngIclDecoder(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName):
													CTestStepMngImageDisplay(aTestParams),
													iNextCheckPoint(aTestParams.iCheckPoints)
	{
	iHeapSize		= KHeapSize;
	iTestStepName	= aTestName;
	}


void CTestStepMngIclDecoder::Cleanup()
	{
	if(iIclDecoder)
		{
		delete iIclDecoder;
		iIclDecoder = NULL;
		}

	delete iFrame; iFrame = NULL;
	delete iMask; iMask=NULL;
	
	delete iCurrentFrame; //Defect:DEF073809
	delete iCurrentMask; //Defect:DEF073809
	iCurrentFrame = NULL; //Defect:DEF073809
	iCurrentMask = NULL; //Defect:DEF073809
	
	CTestStepMngImageDisplay::Cleanup();
	}

TVerdict CTestStepMngIclDecoder::DoTestStepPostambleL()
	{
	delete iActListener;
	iActListener = NULL;
	
	delete iTestTimer;
	iTestTimer = NULL;
		
	return CTestStepMngImageDisplay::DoTestStepPostambleL();
	}

TVerdict CTestStepMngIclDecoder::DoTestStepPreambleL()
	{
	TVerdict res;
	if ((res=CTestStepMngImageDisplay::DoTestStepPreambleL())!=EPass)
		{
		return res;
		}

	iActListener = new (ELeave) CActiveDecodeListener();
	
	return res;
	}


void CTestStepMngIclDecoder::SetupDecoderL(const TDesC& aFileName, TBool aOptions)
	{
	iFrameNumber	= 0;
	iPreviousDelay	= 0;
	if(aOptions == EFalse)
		{
		iIclDecoder = CImageDecoder::FileNewL(iFs, aFileName);
		}
	else
		{
		iIclDecoder = CImageDecoder::FileNewL(iFs, aFileName,CImageDecoder::EOptionMngSubframesNoLoops);
		}
		
	const TFrameInfo& info=iIclDecoder->FrameInfo(0);

	delete iFrame; iFrame = NULL;
	iFrame	= new (ELeave) CFbsBitmap();
	User::LeaveIfError( iFrame->Create(info.iOverallSizeInPixels, EColor16M) );
	delete iMask; iMask=NULL;
	if ((info.iFlags&TFrameInfo::EAlphaChannel)!=0)
		{
		iMask	= new (ELeave) CFbsBitmap();
		User::LeaveIfError( iMask->Create(info.iOverallSizeInPixels, EGray256) );
		}
	}

void CTestStepMngIclDecoder::CheckDecodedFrameL(TBool& aMoreToCheck)
	{
	aMoreToCheck = EFalse;
	if (iNextCheckPoint==NULL || !iNextCheckPoint->IsValid())
		{
		return;
		}

	if (iNextCheckPoint->iAbsoluteFrameNumber != iFrameNumber)
		{
		aMoreToCheck = ETrue;
		iPreviousDelay = iFrameInfo.iDelay;
		return;
		}

	INFO_PRINTF2(_L("Checkpoint Frame# %d"), iFrameNumber);
// perform binary image checking //
	if (NULL != iNextCheckPoint->iReferenceFileName)
		{
		TBuf<32> fileName ;
		fileName.AppendFormat(_L("frame_%d.mbm"),iFrameNumber);
		SetDestFilenameL(fileName);

		SetRefFilename(TPtrC(iNextCheckPoint->iReferenceFileName));
		TInt error;
		TRAP(error, error=CompareBitmapWithFileL(*iFrame,  RefFilename() ));
		if (error != KErrNone )
			{
			INFO_PRINTF4(_L("  --Images Do not match (Ref=%s  Got=%S Err=%d)"), iNextCheckPoint->iReferenceFileName, &fileName, error);
			iFrame->Save(DestFilename());
			User::Leave(error);
			}
		}
// perform binary mask checking //		
	if (NULL != iNextCheckPoint->iReferenceMaskName)
		{
		if (NULL == iMask)
			{
			INFO_PRINTF1(_L("  --Error: Mask bitmap is NULL"));
			User::Leave(KErrGeneral);
			}
		TBuf<32> fileName ;
		fileName.AppendFormat(_L("mask_%d.mbm"),iFrameNumber);
		SetDestFilenameL(fileName);

		SetRefFilename(TPtrC(iNextCheckPoint->iReferenceMaskName));
		TInt error;
		TRAP(error, error=CompareBitmapWithFileL(*iMask,  RefFilename() ));
		if (error != KErrNone )
			{
			INFO_PRINTF4(_L("  --Images Do not match (Ref=%s  Got=%S Err=%d)"), iNextCheckPoint->iReferenceMaskName, &fileName, error);
			iMask->Save(DestFilename());
			User::Leave(error);
			}
		}		
// perfrom timing checking //
	if (iNextCheckPoint->iDelayFromPreviuos != TFrameCheckPoint::KNodelayChecking)
		{
		
		if (iPreviousDelay.Int64() != MAKE_TINT64(0,iNextCheckPoint->iDelayFromPreviuos) )
			{

			INFO_PRINTF4(_L("  --Timing checking failed (Got=%d Expected=%d Diff=%d)"), I64INT(iPreviousDelay.Int64()), 
								iNextCheckPoint->iDelayFromPreviuos, 
								I64INT(MAKE_TINT64(0,iNextCheckPoint->iDelayFromPreviuos )- iPreviousDelay.Int64()));
			User::Leave(KTimingCheckError);
			}
		}
	//get the frame information for next frame
	iFrameInfo = iIclDecoder->FrameInfo(0);
	const TBool continueDecodeFlag=((iFrameInfo.iFlags&TFrameInfo::EMngMoreFramesToDecode)!=0);
	if (continueDecodeFlag !=  ((iNextCheckPoint->iPluginStatusValue&CImageDisplayPlugin::EStatusNoMoreToDecode)==0) )
		{
		INFO_PRINTF3(_L("  --Continue decode flag check failed (Got=%d Expected=%d"), continueDecodeFlag, !continueDecodeFlag);
		User::Leave(KDecodeFlagCheckError);
		}
	iPreviousDelay = iFrameInfo.iDelay;
	aMoreToCheck = (++iNextCheckPoint && iNextCheckPoint->IsValid());
	}


CTestStepMngIclCheckpoint::CTestStepMngIclCheckpoint(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName, TBool aOptions):
													CTestStepMngIclDecoder(aTestParams, aTestName),
													iOptions(aOptions)
	{
	}

TVerdict CTestStepMngIclCheckpoint::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	iNextCheckPoint = iTestParams->iCheckPoints;
	SetupDecoderL(SourceFilename(), iOptions);	
	TBool continueTest;
	TBool hasMask;  //Defect:DEF073809
	do 
		{
		iFrameInfo = iIclDecoder->FrameInfo(0); //Defect:DEF073809
		hasMask = ((iFrameInfo.iFlags&TFrameInfo::EAlphaChannel) != 0); //Defect:DEF073809
				
	    delete iCurrentFrame; iCurrentFrame = NULL;
		iCurrentFrame = new (ELeave) CFbsBitmap();
		User::LeaveIfError(iCurrentFrame->Create(iFrameInfo.iFrameCoordsInPixels.Size(),EColor16M));
		delete iCurrentMask; iCurrentMask=NULL;
		if (hasMask)
			{
			iCurrentMask = new (ELeave) CFbsBitmap();
			User::LeaveIfError(iCurrentMask->Create(iFrameInfo.iFrameCoordsInPixels.Size(),EGray256));
			}
		//
		
		iActListener->InitialiseActiveListener(iIclDecoder);
		if (!hasMask)
			{
			iIclDecoder->Convert(iActListener->ReqStatus(),*iCurrentFrame); //Defect:DEF073809
			}
		else
			{
			iIclDecoder->Convert(iActListener->ReqStatus(),*iCurrentFrame,*iCurrentMask); //Defect:DEF073809
			}
		
		CActiveScheduler::Start();
		User::LeaveIfError( iActListener->Result() );
		
		ComposeFrame(iFrameInfo);  //Defect:DEF073809
		
		++iFrameNumber;
		CheckDecodedFrameL(continueTest);
		} while(continueTest);
		
	if (!(iNextCheckPoint==NULL || !iNextCheckPoint->IsValid()))
		{
		INFO_PRINTF1(_L("  --Error: Haven't decoded all the frames"));
		return EFail;
		}
	return EPass;
	}


//DEF080823 starts
CTestStepMngIclCancelCheckpoint::CTestStepMngIclCancelCheckpoint(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName, TBool aOptions):
														CTestStepMngIclDecoder(aTestParams, aTestName),
														iOptions(aOptions)
	{
	}

TVerdict CTestStepMngIclCancelCheckpoint::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	TBool exitFlag = EFalse;
	TInt delta_time = 50;
	TInt inc=0;
	do
		{
		
		iTestTimer = new (ELeave) CTestTimer();
		iTestTimer->ConstructL();

		SetSourceFilename(TPtrC(iTestParams->iInputFilename));
		iNextCheckPoint = iTestParams->iCheckPoints;
		
		CActiveDecodeCancel* activeCancel = new (ELeave) CActiveDecodeCancel;
		TFileName fileName = SourceFilename();
		
		iFrameNumber	= 0;
		iPreviousDelay	= 0;
		CleanupStack::PushL(activeCancel);
		
		if(iOptions == EFalse)
			{
			activeCancel->iDecoder = CImageDecoder::FileNewL(iFs, fileName);
			}
		else
			{
			activeCancel->iDecoder = CImageDecoder::FileNewL(iFs, fileName,CImageDecoder::EOptionMngSubframesNoLoops);
			}
			
		const TFrameInfo& frameInfo = activeCancel->iDecoder->FrameInfo(0);

		delete iFrame; iFrame = NULL;
		iFrame	= new (ELeave) CFbsBitmap();
		User::LeaveIfError( iFrame->Create(frameInfo.iOverallSizeInPixels, EColor16M) );
		delete iMask; iMask=NULL;
		if ((frameInfo.iFlags&TFrameInfo::EAlphaChannel)!=0)
			{
			iMask	= new (ELeave) CFbsBitmap();
			User::LeaveIfError( iMask->Create(frameInfo.iOverallSizeInPixels, EGray256) );
			}

		TBool hasMask; 

		hasMask = ((frameInfo.iFlags&TFrameInfo::EAlphaChannel) != 0); 
				
	    delete iCurrentFrame; iCurrentFrame = NULL;
		iCurrentFrame = new (ELeave) CFbsBitmap();
		User::LeaveIfError( iCurrentFrame->Create(frameInfo.iFrameCoordsInPixels.Size(), EColor16M) );
		delete iCurrentMask; iCurrentMask=NULL;
		if (hasMask)
			{
			iCurrentMask = new (ELeave) CFbsBitmap();
			User::LeaveIfError( iCurrentMask->Create(frameInfo.iFrameCoordsInPixels.Size(), EGray256) );
			}

		activeCancel->InitialiseActiveListener();
		
		TTimeIntervalMicroSeconds32 time(200 + inc*delta_time); 
		iTestTimer->InitialiseActiveListener(activeCancel);
		iTestTimer->After(time);
		if (!hasMask)
			{
			activeCancel->iDecoder->Convert(activeCancel->ReqStatus(), *iCurrentFrame);  
			}
		else
			{
			activeCancel->iDecoder->Convert(activeCancel->ReqStatus(), *iCurrentFrame, *iCurrentMask); 
			}
		
		CActiveScheduler::Start();
		exitFlag = activeCancel->iIsReadyToExit;
		
	    if(!activeCancel->iIsPassed)
			{
			INFO_PRINTF1(_L("  --Error: Request still alive inside the ICL Decoder"));
		
			delete iTestTimer;
			iTestTimer = NULL;
			
			CleanupStack::PopAndDestroy(activeCancel);
			
			return EFail; 
			}
		// DEF113719. Comments out the INFO_PRINTF to avoid the redundant information into log file
		//INFO_PRINTF2(_L("  --Delay specified: %d"),200 + inc*delta_time);

		delete iTestTimer;
		iTestTimer = NULL;
		
		CleanupStack::PopAndDestroy(activeCancel);
			
		inc++;
		}while(!exitFlag);
		
	return EPass;
	}
//DEF080823 ends

CTestStepMngMagnCheck::CTestStepMngMagnCheck(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName,
											 const TUint* aColorRef, const TUint* aAlphaRef):
																			CTestStepMngIclDecoder(aTestParams, aTestName),
																			iColorRef(aColorRef),
																			iAlphaRef(aAlphaRef)
	{
	}

	
TVerdict CTestStepMngMagnCheck::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	SetupDecoderL(SourceFilename());
	
	iActListener->InitialiseActiveListener(iIclDecoder);
	iIclDecoder->Convert(iActListener->ReqStatus(), *iFrame, *iMask);
	CActiveScheduler::Start();
	User::LeaveIfError( iActListener->Result() );
	++iFrameNumber;

	if (!(CompareBitmapToRefBuffer(iFrame, iColorRef) == EPass && 
				CompareBitmapToRefBuffer(iMask, iAlphaRef) == EPass))
		{
		return EFail;
		}
	return EPass;
	}

CTestStepMngMagn32BitSingleDestinationCheck::CTestStepMngMagn32BitSingleDestinationCheck(const TImageDisplayUnitTestParams& aTestParams,
														const TDesC& aTestName, TDisplayMode aDisplayMode,
														const TUint* aColorRef)
	:CTestStepMngIclDecoder(aTestParams, aTestName),
	iColorRef(aColorRef),
	iDisplayMode(aDisplayMode)
	{
	}
	
TVerdict CTestStepMngMagn32BitSingleDestinationCheck::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	switch(iDisplayMode)
		{
		case EColor16MA:
			INFO_PRINTF1(_L("Testing destination bitmap with displaymode EColor16MA"));
			break;
		case EColor16MU:
			INFO_PRINTF1(_L("Testing destination bitmap with displaymode EColor16MU"));
			break;
		default:
			INFO_PRINTF1(_L("This test does not support this display mode - Leaving"));
			User::Leave(KErrNotSupported);
		}
	
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	iIclDecoder = CImageDecoder::FileNewL(iFs, SourceFilename());
	const TFrameInfo& info=iIclDecoder->FrameInfo(0);

	// Single destination bitmap
	delete iFrame; iFrame = NULL;
	iFrame	= new (ELeave) CFbsBitmap();
	User::LeaveIfError(iFrame->Create(info.iOverallSizeInPixels, iDisplayMode));
	
	iActListener->InitialiseActiveListener(iIclDecoder);
	iIclDecoder->Convert(iActListener->ReqStatus(), *iFrame);
	CActiveScheduler::Start();
	User::LeaveIfError(iActListener->Result());

	if (Compare32BitBitmapToRefBuffer(iFrame, iColorRef) == EFail)
		{
		return EFail;
		}
		
	return EPass;
	}

CTestStepMngMagn32BitDestinationAndMaskCheck::CTestStepMngMagn32BitDestinationAndMaskCheck(const TImageDisplayUnitTestParams& aTestParams,
														const TDesC& aTestName, TDisplayMode aDisplayMode,
														const TUint* aColorRef, const TUint* aAlphaRef)
	:CTestStepMngIclDecoder(aTestParams, aTestName),
	iColorRef(aColorRef),
	iAlphaRef(aAlphaRef),
	iDisplayMode(aDisplayMode)
	{
	}
	
TVerdict CTestStepMngMagn32BitDestinationAndMaskCheck::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	
	switch(iDisplayMode)
		{
		case EColor16MA:
			INFO_PRINTF1(_L("Testing destination bitmap with displaymode EColor16MA and EGray256 mask bitmap"));
			break;
		case EColor16MU:
			INFO_PRINTF1(_L("Testing destination bitmap with displaymode EColor16MU and EGray256 mask bitmap"));
			break;
		default:
			INFO_PRINTF1(_L("This test does not support this display mode - Leaving"));
			User::Leave(KErrNotSupported);
		}

	SetSourceFilename(TPtrC(iTestParams->iInputFilename));	
	iIclDecoder = CImageDecoder::FileNewL(iFs, SourceFilename());
	const TFrameInfo& info=iIclDecoder->FrameInfo(0);
	
	TInt error = KErrNone;
	
	// Destination plus mask bitmaps
	delete iFrame; iFrame = NULL;
	iFrame	= new (ELeave) CFbsBitmap();
	User::LeaveIfError(iFrame->Create(info.iOverallSizeInPixels, iDisplayMode));
	delete iMask; iMask = NULL;
	iMask	= new (ELeave) CFbsBitmap();
	User::LeaveIfError(iMask->Create(info.iOverallSizeInPixels, EGray256));
	
	iActListener->InitialiseActiveListener(iIclDecoder);
	iIclDecoder->Convert(iActListener->ReqStatus(), *iFrame, *iMask);
	CActiveScheduler::Start();
	User::LeaveIfError(iActListener->Result());
	
	if (Compare32BitBitmapToRefBuffer(iFrame, iColorRef) == EFail)
		{
		error += 1; // allow test to continue
		}
	
	if (CompareBitmapToRefBuffer(iMask, iAlphaRef) == EFail)
		{
		error += 1; // allow test to continue
		}
	
	if(error != KErrNone)
		{
		return EFail;
		}
	
	return EPass;
	}

CTestStepMngIclNegative::CTestStepMngIclNegative(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName):
																				CTestStepMngIclDecoder(aTestParams, aTestName)
	{
	}


TVerdict CTestStepMngIclNegative::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	TInt error;
	TRAP( error, SetupDecoderL(SourceFilename()) );
	if (error == KErrNone)
		{

		TBool continueTest;
		do 
			{
			iActListener->InitialiseActiveListener(iIclDecoder);
			iIclDecoder->Convert(iActListener->ReqStatus(), *iFrame);
			CActiveScheduler::Start();
			if ( iActListener->Result() != KErrNone)
				{
				break;
				}
			++iFrameNumber;
			continueTest=((iIclDecoder->FrameInfo().iFlags&TFrameInfo::EMngMoreFramesToDecode)!=0);
			} while(continueTest);
		error = iActListener->Result();
		}
	
	if (error != iTestParams->iErrorCode)
		{
		INFO_PRINTF3(_L("Negative test: Got wrong error code=%d, Expected=%d"),error, iTestParams->iErrorCode);
		return EFail;
		}
	return EPass;
	}


CTestStepMngIclResponse::CTestStepMngIclResponse(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName):
																	CTestStepMngIclDecoder(aTestParams, aTestName)
	{
	}

TVerdict CTestStepMngIclResponse::TestL()
	{
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	iTimer = CCallbackTimer::NewLC();
	SetupDecoderL(SourceFilename());
	iCancelDelay = 0;
	TBool continueTest;
	TBool hasMask;  //Defect:DEF073809
	TFrameInfo frameInfo;  //Defect:DEF073809
	TTime completedAt;
	do 
		{

		TInt res;
		do 
			{
			//Defect:DEF073809
			frameInfo = iIclDecoder->FrameInfo(0); //Defect:DEF073809
			hasMask = ((frameInfo.iFlags&TFrameInfo::EAlphaChannel) != 0); //Defect:DEF073809
				
	    	delete iCurrentFrame; iCurrentFrame = NULL;
			iCurrentFrame = new (ELeave) CFbsBitmap();
			User::LeaveIfError( iCurrentFrame->Create(frameInfo.iFrameCoordsInPixels.Size(), EColor16M) );
			delete iCurrentMask; iCurrentMask=NULL;
			if (hasMask)
				{
				iCurrentMask = new (ELeave) CFbsBitmap();
				User::LeaveIfError( iCurrentMask->Create(frameInfo.iFrameCoordsInPixels.Size(), EGray256) );
				}
			
			// DEF113719. Comments out the INFO_PRINTF to avoid the redundant information into log file
			//INFO_PRINTF2(_L("  --Cancel Delay would be %d"),iCancelDelay);
			iActListener->InitialiseActiveListener(iIclDecoder);
					
			if (!hasMask) //Defect:DEF073809
				{
				iIclDecoder->Convert(iActListener->ReqStatus(), *iCurrentFrame);  //Defect:DEF073809
				}
			else
				{
				iIclDecoder->Convert(iActListener->ReqStatus(), *iCurrentFrame, *iCurrentMask);  //Defect:DEF073809
				}
			iTimer->After(TTimeIntervalMicroSeconds32(iCancelDelay), TCallBack(TimerGate, this) );
			CActiveScheduler::Start();
			completedAt.HomeTime();
			iTimer->Cancel();
			res=iActListener->Result();
			
			if (res != KErrCancel && res != KErrNone )
				{
				INFO_PRINTF2(_L(" --Convert() resulted in error %d"), res);
				User::Leave( res );
				}

			if (res==KErrCancel)
				{
				TInt64 msDiff=completedAt.Int64() - iCancelRequestedAt.Int64();
				
				// Comments out the INFO_PRINTF to avoid the redundant information into log file
				//INFO_PRINTF3(_L("Delay requested %d Got response after %d"),iCancelDelay, I64INT(msDiff));
				iFrameNumber =0;
				iNextCheckPoint = iTestParams->iCheckPoints;
				iCancelDelay += KCancelDelayStep;
				if (I64INT(msDiff) > KCancelResponseTreshold)
					{
					INFO_PRINTF1(_L(" --Response is BAD"));
					User::Leave(KErrGeneral);
					}
				}
			
			} while (res==KErrCancel);
		
		frameInfo = iIclDecoder->FrameInfo(0); //Defect:DEF073809	
        ComposeFrame(frameInfo);  //Defect:DEF073809
		++iFrameNumber;
		CheckDecodedFrameL(continueTest);

		} while(continueTest);

	CleanupStack::PopAndDestroy(iTimer);
	if (!(iNextCheckPoint==NULL || !iNextCheckPoint->IsValid()))
		{
		INFO_PRINTF1(_L("  --Error: Haven't decoded all the frames"));
		return EFail;
		}

	return EPass;
	}

void CTestStepMngIclDecoder::ComposeFrame(const TFrameInfo& aFrameInfo)  //Defect:DEF073809
	{
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(iFrame);
	CleanupStack::PushL(device);

	CFbsBitGc* gc = CFbsBitGc::NewL();
	CleanupStack::PushL(gc);
	gc->Activate(device);
	
	gc->BitBlt(aFrameInfo.iFrameCoordsInPixels.iTl, iCurrentFrame);
	
	CleanupStack::PopAndDestroy(2, device);
	
	const TBool hasMask = ((aFrameInfo.iFlags&TFrameInfo::EAlphaChannel) != 0); //Defect:DEF073809
	if(hasMask)
		{
		device = CFbsBitmapDevice::NewL(iMask);
		CleanupStack::PushL(device);
		
		gc = CFbsBitGc::NewL();
		CleanupStack::PushL(gc);
		gc->Activate(device);
		
		gc->BitBlt(aFrameInfo.iFrameCoordsInPixels.iTl, iCurrentMask);
		
		CleanupStack::PopAndDestroy(2, device);
		}
	}


/*static*/
TInt CTestStepMngIclResponse::TimerGate(TAny* aPtr)
	{
	CTestStepMngIclResponse* self=reinterpret_cast<CTestStepMngIclResponse*>(aPtr);
	self->iIclDecoder->Cancel();
	self->iCancelRequestedAt.HomeTime();
	return KErrNone;
	}

//DEF085248 n DEF085250 starts
CTestStepMngIclMimeType::CTestStepMngIclMimeType(const TImageDisplayUnitTestParams& aTestParams, const TDesC& aTestName):
														CTestStepMngIclDecoder(aTestParams, aTestName)
	{
	}

TVerdict CTestStepMngIclMimeType::TestL()
	{
	iTestStepResult = EFail;
	ASSERT(iTestParams->iSourceType == TImageDisplayUnitTestParams::ETypeFileName);
	
	SetSourceFilename(TPtrC(iTestParams->iInputFilename));
	TFileName fileName = SourceFilename();
	TBuf8<128> mimeType;
	
	if (iTestStepName.Compare(_L("MM-ICL-MNGICLDEC-U-0224-HP"))==0)	
		{
		TRAPD(err, CImageDecoder::GetMimeTypeFileL(iFs, fileName, mimeType));
		
		TBuf8<16> expMime;
		expMime.Copy(iTestParams->iMimeType);
		if(err == KErrNone && (mimeType.Compare(expMime)==0))
			{
			iTestStepResult = EPass;
			}
		}
	else
		{
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
		TInt fileSize;
		RFile file;
		User::LeaveIfError(file.Open(fs, fileName, EFileShareReadersOnly));
		CleanupClosePushL(file);
		User::LeaveIfError(file.Size(fileSize));
			
		HBufC8* imageData = HBufC8::NewLC(fileSize);
		TPtr8 dataPtr = imageData->Des();
		User::LeaveIfError(file.Read(0, dataPtr));
		TRAPD( err, CImageDecoder::GetMimeTypeDataL(imageData->Des(), mimeType));
		TBuf8<64> expMime;
		expMime.Copy(iTestParams->iMimeType);
		if(err == KErrNone && (mimeType.Compare(expMime)==0))
			{
			iTestStepResult = EPass;
			}
		
		CleanupStack::PopAndDestroy(3, &fs);
		}	
	return iTestStepResult;
	}
//DEF085248 n DEF085250 ends
