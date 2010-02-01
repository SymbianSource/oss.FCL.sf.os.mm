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
// TMDAVID1.CPP
// These tests are designed to perform multiple Media Server opertaions by using multiple threads
// each containing many active objects.
// 
//

#include "tmdatest.h"


/*********************************************************************************************/
class CStressTestMonitor : public CBase
	{
public:
	void IncActiveCount();
	void DecActiveCount();
	void SetError(TInt aError);
	TInt Error();
private:
	TInt iActiveCount;
	TInt iError;
	};

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
	if(iError==KErrNone)
		iError = aError;
	}

TInt CStressTestMonitor::Error()
	{
	return(iError);
	}

/*********************************************************************************************/



/*********************************************************************************************/
class CStressTestActive : public CActive
	{
public:
	inline CStressTestActive(CStressTestMonitor& aMonitor) : CActive(EPriorityStandard), iMonitor(&aMonitor) { aMonitor.IncActiveCount(); }
	~CStressTestActive();
protected:
	// From CActive
	TInt RunError(TInt aError);
	// New
	void SetPending();
public:
	CStressTestMonitor* iMonitor;
	};

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

/*********************************************************************************************/



/*********************************************************************************************/
class CImageUtil : public CStressTestActive, public MMdaImageUtilObserver
	{
public:
	inline CImageUtil(CStressTestMonitor& aMonitor) : CStressTestActive(aMonitor) {}
private:
	virtual void MiuoCreateComplete(TInt aError);
	virtual void MiuoOpenComplete(TInt aError);
	virtual void MiuoConvertComplete(TInt aError);
	};

void CImageUtil::MiuoCreateComplete(TInt aError)
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,aError);
	}

void CImageUtil::MiuoOpenComplete(TInt aError)
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,aError);
	}

void CImageUtil::MiuoConvertComplete(TInt aError)
	{
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,aError);
	}
/*********************************************************************************************/



/*********************************************************************************************/
class CImageLoadUtil : public CImageUtil
	{
public:
	static CImageLoadUtil* NewL(CStressTestMonitor& aMonitor,const TDesC& aFileName,const TDisplayMode aDisplayMode);
	~CImageLoadUtil();
private:
	inline CImageLoadUtil(CStressTestMonitor& aMonitor) : CImageUtil(aMonitor) {};
	void RunL();
	void DoCancel();
private:
	TBuf<255> iMessage;
	TDisplayMode iDisplayMode;
	CFbsBitmap* iBitmap;
	CMdaImageFileToBitmapUtility* iMdaUtil;
	TInt iState;
	};

CImageLoadUtil* CImageLoadUtil::NewL(CStressTestMonitor& aMonitor,const TDesC& aFileName,const TDisplayMode aDisplayMode)
	{
	CImageLoadUtil* ptr = new (ELeave) CImageLoadUtil(aMonitor);

	CleanupStack::PushL(ptr);

	ptr->iBitmap = new(ELeave) CFbsBitmap;
	ptr->iDisplayMode = aDisplayMode;

	ptr->iMdaUtil = CMdaImageFileToBitmapUtility::NewL(*ptr);

	ptr->iMessage.Append(_L("Loading image "));
	ptr->iMessage.Append(aFileName);

	ptr->iMdaUtil->OpenL(aFileName,NULL,NULL,NULL);

	CleanupStack::Pop();

	ptr->SetPending();

	return ptr;
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
		iMessage.AppendFormat(_L(" - Failed with error %d"),error);
		RDebug::Print(iMessage);
		User::Leave(error);
		}
	else
		{
		iMessage.AppendFormat(_L(" - Finished OK"));
		RDebug::Print(iMessage);
		}
	delete this;
	}

void CImageLoadUtil::DoCancel()
	{
	iMdaUtil->Cancel();
	}

/*********************************************************************************************/



/*********************************************************************************************/
GLDEF_C TInt TestStartupThread(TAny* /*aParam*/)
	{
	return KErrNone;
	}
/*********************************************************************************************/




/*********************************************************************************************/
void DoTestLoadImagesL(CMdaStressTestThread* aThread)
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
	fs.GetDir(KNullDesC,0,0,dir);
	CleanupStack::PushL(dir);
	TInt entries = dir->Count();

	TDisplayMode displayMode;
	if(aThread->iId==CMdaStressTest::ELoadImages1)
		displayMode = EColor256;
	else
		displayMode = EGray4;

	entries--;
	while(entries>=0)
		{
		const TEntry& entry = (*dir)[entries];
		TFileName fileName;
		User::LeaveIfError(fs.DefaultPath(fileName));
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
/*********************************************************************************************/



/*********************************************************************************************/
GLDEF_C TInt TestLoadImages(TAny* aParam)
	{
	CTrapCleanup* tc = CTrapCleanup::New();
	__ASSERT_ALWAYS(tc,User::Invariant());	

	TRAPD(err,DoTestLoadImagesL(REINTERPRET_CAST(CMdaStressTestThread*,aParam)));

	delete tc;
	return err;
	}
/*********************************************************************************************/



/*********************************************************************************************/
CMdaStressTest::CMdaStressTest()
	: CMdaCoreTest(), iThreadList(_FOFF(CMdaStressTestThread,iLink)), iThreadIter(iThreadList), iRandomSeed(1)
	{
	}
/*********************************************************************************************/



/*********************************************************************************************/
CMdaStressTest::~CMdaStressTest()
	{
	// Delete all threads
	CMdaStressTestThread* thread;
	iThreadIter.SetToFirst();
	while ((thread=iThreadIter++)!=NULL)
		DeleteTestThread(thread);
	}
/*********************************************************************************************/



const TInt KThreadScanDelay = 200000;

/*********************************************************************************************/
void CMdaStressTest::DoTestL()
	{
	TInt error = KErrNone;
	TInt time = 10000000;
	TInt testNumber;
	CMdaStressTestThread* thread;

	// Launch test threads
	for(testNumber=0; testNumber<ENumTestThreads; testNumber++)
		StartTestThreadL(STATIC_CAST(TTestThreadId,testNumber));

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
				if(reason!=KErrNone)
					{
					iEngine->TestFailed(reason);
					error = reason;
					break;
					}
				testNumber = thread->iId;
				DeleteTestThread(thread);
				StartTestThreadL(STATIC_CAST(TTestThreadId,testNumber));	// Restart test
				}
			}

		if(error)
			break;

		User::After(KThreadScanDelay);
		time -= KThreadScanDelay;
		}
	while(time>0);

//	iEngine->Print(_L("Waiting for test threads to end..."));

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
				if(reason!=KErrNone)
					iEngine->TestFailed(reason);
				DeleteTestThread(thread);
				}
		}


	}
/*********************************************************************************************/



/*********************************************************************************************/
void CMdaStressTest::StartTestThreadL(TTestThreadId aId)
	{
	TThreadFunction function;

	switch(aId)
		{
		case ELoadImages1:
		case ELoadImages2:
			function = TestLoadImages;
			break;
		default:
			return;
		}

	TBuf<40> name;
	name.Append(_L("CMdaStressTestThread #"));
	name.AppendNum(aId,EDecimal);
	name.Append(_L("-"));
	name.AppendNum(iThreadNameCount++,EDecimal);

	CMdaStressTestThread* thread = new (ELeave) CMdaStressTestThread;
	CleanupStack::PushL(thread);

	thread->iId = aId;
	User::LeaveIfError(thread->iThread.Create(name, function, KDefaultStackSize, 1024, 2048*1024, thread));

	CleanupStack::Pop();

	iThreadList.AddLast(*thread);
	thread->iThread.Resume();
	}
/*********************************************************************************************/



/*********************************************************************************************/
void CMdaStressTest::DeleteTestThread(CMdaStressTestThread* thread)
	{
	if(thread->iThread.ExitType()==EExitPending)
		thread->iThread.Terminate(KErrNone);
	thread->iThread.Close();
	iThreadList.Remove(*thread);
	delete thread;
	}
/*********************************************************************************************/
