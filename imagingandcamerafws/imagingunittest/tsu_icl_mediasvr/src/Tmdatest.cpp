// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// These tests are designed to perform multiple MMF operations by using multiple threads
// each containing many active objects.
// 
//

#include <testframework.h>
#include <icl/imagedata.h>
#include "TMDATEST.H"

// ----------------------------------------------------------------------------

void CStressTestMonitor::IncActiveCount()
	{
	iActiveCount++;
	}

void CStressTestMonitor::DecActiveCount()
	{
	iActiveCount--;
	if(iActiveCount==0)
		CActiveScheduler::Stop();
	}

void CStressTestMonitor::SetError(TInt aError)
	{
	// only store first error passed in
	if(iError==KErrNone)
		iError = aError;
	}

TInt CStressTestMonitor::Error()
	{
	return(iError);
	}

// ----------------------------------------------------------------------------

CStressTestActive::~CStressTestActive()
	{
	iMonitor->DecActiveCount();
	}

void CStressTestActive::SetPending()
	{
	if(!IsAdded())
		CActiveScheduler::Add(this);
	iStatus = KRequestPending;
	SetActive();
	}

TInt CStressTestActive::RunError(TInt aError)
	{
	iMonitor->SetError(aError);
	delete this;
	return KErrNone;
	}

// ----------------------------------------------------------------------------

CImageUtil::CImageUtil(CStressTestMonitor& aMonitor)
		 : CStressTestActive(aMonitor) 
	{
	}

void CImageUtil::MiuoCreateComplete(TInt aError)
	{
	Complete(aError);
	}

void CImageUtil::MiuoOpenComplete(TInt aError)
	{
	Complete(aError);
	}

void CImageUtil::MiuoConvertComplete(TInt aError)
	{
	Complete(aError);
	}

void CImageUtil::Complete(TInt aError)
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,aError);
	}

// ----------------------------------------------------------------------------

CImageLoadUtil* CImageLoadUtil::NewL(CStressTestMonitor& aMonitor,const TDesC& aFileName,const TDisplayMode aDisplayMode)
	{
	CImageLoadUtil* imageLoadUtil = new (ELeave) CImageLoadUtil(aMonitor);
	CleanupStack::PushL(imageLoadUtil);

	imageLoadUtil->iBitmap = new(ELeave) CFbsBitmap;
	imageLoadUtil->iDisplayMode = aDisplayMode;
	imageLoadUtil->iMdaUtil = CMdaImageFileToBitmapUtility::NewL(*imageLoadUtil);
	imageLoadUtil->iMessage.Append(_L("Loading image "));
	imageLoadUtil->iMessage.Append(aFileName);
	imageLoadUtil->iMdaUtil->OpenL(aFileName,NULL,NULL,NULL);

	CleanupStack::Pop(); // imageLoadUtil

	imageLoadUtil->SetPending();
	return imageLoadUtil;
	}

CImageLoadUtil::~CImageLoadUtil()
	{
	Cancel();
	delete iBitmap;
	delete iMdaUtil;
	}

void CImageLoadUtil::RunL()
	{
	TInt error = iStatus.Int();

	if(error==KErrNone)
		{
		if(!iState)
			{
			TFrameInfo frameInfo;
			iMdaUtil->FrameInfo(0,frameInfo);

			iBitmap->Reset();
			error = iBitmap->Create(frameInfo.iOverallSizeInPixels,iDisplayMode);
			if(error==KErrNone)
				{
				TRAP(error,iMdaUtil->ConvertL(*iBitmap));
				if(error==KErrNone)
					{
					iState++;
					SetPending();
					return;
					}
				}
			}
		}

	if(error!=KErrNone)
		{
//		ERR_PRINTF3(_L("%S - Failed with error %d"),&iMessage,error);
		User::Leave(error);
		}
	else
		{
//		INFO_PRINTF2(_L("%S - Finished OK"),&iMessage);		
		}
	delete this;
	}

void CImageLoadUtil::DoCancel()
	{
	iMdaUtil->Cancel();
	}

// ----------------------------------------------------------------------------

GLDEF_C TInt TestStartupThread(TAny* /*aParam*/)
	{
	return KErrNone;
	}

// ----------------------------------------------------------------------------

void DoTestLoadImagesL(TMdaStressParam* aParam)
	{
	TInt err;

	 // Install active scheduler
	CActiveScheduler* as = new CActiveScheduler;
	__ASSERT_ALWAYS(as,User::Invariant());	
	CActiveScheduler::Install(as);
	CleanupStack::PushL(as);

	// Connect to file server
	RFs fs;
	err = fs.Connect();
	__ASSERT_ALWAYS(err == KErrNone, User::Invariant());
	CleanupClosePushL(fs);

	// Connect to FBS
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	// Create monitor object
	CStressTestMonitor* monitor = new (ELeave) CStressTestMonitor;
	CleanupStack::PushL(monitor);

	CDir* dir = NULL;
	TFileName dirName = aParam->iDefaultPath;
	dirName.Append(_L("stress\\"));		
	fs.GetDir(dirName,0,0,dir);
	CleanupStack::PushL(dir);
	TInt entries = dir->Count();

	TDisplayMode displayMode;
	if(aParam->iId==CMdaStressTest::ELoadImages1)
		displayMode = EColor256;
	else
		displayMode = EGray4;

	entries--;
	while(entries>=0)
		{
		const TEntry& entry = (*dir)[entries];
		TFileName fileName;
		fileName.Append(dirName);
		fileName.Append(entry.iName);
		CImageLoadUtil::NewL(*monitor,fileName,displayMode);
		entries -= 2;	// only load every other image to keep memory usage down
		}
	CleanupStack::PopAndDestroy();	// dir

	// Run active objects
	CActiveScheduler::Start();

	// Check for errors
	User::LeaveIfError(monitor->Error());

	// Close everything
	CleanupStack::PopAndDestroy(); // monitor
	RFbsSession::Disconnect();
	CleanupStack::PopAndDestroy(2); // fs and as
	}

// ----------------------------------------------------------------------------

GLDEF_C TInt TestLoadImages(TAny* aParam)
	{
	CTrapCleanup* tc = CTrapCleanup::New();
	__ASSERT_ALWAYS(tc,User::Invariant());	

	TRAPD(err,DoTestLoadImagesL(REINTERPRET_CAST(TMdaStressParam*,aParam)));

	delete tc;
	return err;
	}

// ----------------------------------------------------------------------------

CMdaStressTest::CMdaStressTest()
	: iThreadList(_FOFF(CMdaStressTestThread,iLink)), iThreadIter(iThreadList), iRandomSeed(1)
	{
	}

CMdaStressTest::~CMdaStressTest()
	{
	// Delete all threads
	CMdaStressTestThread* thread;
	iThreadIter.SetToFirst();
	while ((thread=iThreadIter++)!=NULL)
		DeleteTestThread(thread);
	}

const TInt KThreadScanDelay = 200000; // .2 seconds
const TInt KTestDuration = 5000000; // 5 seconds for whole test

void CMdaStressTest::DoTestL(const TDesC& aDefaultPath)
	{
	TInt error = KErrNone;
	TInt time = KTestDuration;
	TInt testNumber;
	CMdaStressTestThread* thread;

	// Launch test threads
	for(testNumber=0; testNumber<ENumTestThreads; testNumber++)
	{
		iParam[testNumber].iId = testNumber;
		iParam[testNumber].iDefaultPath = aDefaultPath;
		StartTestThreadL(iParam[testNumber]);
	}

	// Scan thread list
	do
		{
		TInt numThreads = 0;

		iThreadIter.SetToFirst();
		while ((thread=iThreadIter++)!=NULL)
			{
			if(thread->iThread.ExitType()==EExitPending)
				numThreads++;
			else
				{
				TInt reason = thread->iThread.ExitReason();
				User::LeaveIfError(reason);

				testNumber = thread->iId;
				DeleteTestThread(thread);
				StartTestThreadL(iParam[testNumber]);	// Restart test
				}
			}

		if(error)
			break;

		User::After(KThreadScanDelay);
		time -= KThreadScanDelay;
		}
	while(time>0);

	// Wait for all threads to finish
	while(!iThreadList.IsEmpty())
		{
		User::After(KThreadScanDelay);
		// Delete all completed threads
		iThreadIter.SetToFirst();
		while ((thread=iThreadIter++)!=NULL)
			if(thread->iThread.ExitType()!=EExitPending)
				{
				TInt reason = thread->iThread.ExitReason();
				User::LeaveIfError(reason);
				DeleteTestThread(thread);
				}
		}


	}

void CMdaStressTest::StartTestThreadL(TMdaStressParam& aParam)
	{
	TThreadFunction function;

	switch(aParam.iId)
		{
		case ELoadImages1:
		case ELoadImages2:
			function = TestLoadImages;
			break;
		default:
			return;
		}

	TBuf<256> name;
	name.Append(_L("CMdaStressTestThread #"));
	name.AppendNum(aParam.iId,EDecimal);
	name.Append(_L("-"));
	name.AppendNum(iThreadNameCount++,EDecimal);

	CMdaStressTestThread* thread = new (ELeave) CMdaStressTestThread;
	CleanupStack::PushL(thread);

	User::LeaveIfError(thread->iThread.Create(name, function, KDefaultStackSize, 1024, 2048*1024, &aParam));

	CleanupStack::Pop();

	thread->iId = aParam.iId;
	iThreadList.AddLast(*thread);
	thread->iThread.Resume();
	}

void CMdaStressTest::DeleteTestThread(CMdaStressTestThread* aThread)
	{
	if(aThread->iThread.ExitType()==EExitPending)
		aThread->iThread.Terminate(KErrNone);
	aThread->iThread.Close();
	iThreadList.Remove(*aThread);
	delete aThread;
	}
