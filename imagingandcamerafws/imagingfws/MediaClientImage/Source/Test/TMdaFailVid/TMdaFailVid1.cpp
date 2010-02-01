// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TMDAVID6.CPP
// 
//

#include "tmdatest.h"

/*

Tests:
File -> Bitmap with alloc failure
File -> Bitmap streaming with alloc failure
Bitmap -> File with alloc failure

*/

//
// CTestImageIO 
// walking class that does image reads and writes. Exploits asynchronous behaviour
// of own properties. Calls are essentially synchronous using AS recursion
//

const TInt KViaDesIncrement = 0x1000;
const TInt KTempHeapSize = 0x100000; // 1M 0x20000; // 128K

class CTestImageIO : public CBase, public MMdaImageUtilObserver
	{
// New data types
protected:
	enum TOperation {ERead, EReadViaDes, EReadViaFile, EWrite};
	enum TState {EStateIdle, EStateOpening, EStateConverting};
// Construction, Destruction
public:
	static CTestImageIO *NewL(RFs &aFs);
	static CTestImageIO *NewLC(RFs &aFs);
	~CTestImageIO();
protected:
	CTestImageIO(RFs &aFs);
	void ConstructL();
// New public functions
public:
	void ReadL(CFbsBitmap& aBitmap, const TDesC& aFileName, TInt aZoomFactor);
	void ReadViaDesL(CFbsBitmap& aBitmap, const TDesC& aFileName, TInt aZoomFactor);
	void ReadViaFileL(CFbsBitmap& aBitmap, const TDesC& aFileName, TInt aZoomFactor);
	void WriteL(CFbsBitmap& aBitmap, const TDesC& aFileName, TMdaClipFormat* aFormat,TMdaPackage* aCodec);
// Virtual functions from MMdaImageUtilObserver
protected:
	virtual void MiuoCreateComplete(TInt aError);
	virtual void MiuoOpenComplete(TInt aError);
	virtual void MiuoConvertComplete(TInt aError);
// Helper functions
protected:
	void Leave(TInt aError);
	void OpenOrigAndTempFilesL();
	void CloseOrigAndTempFiles();
	void CopyFileToTempL(TInt aSize);
	void ResetStreamBufferedSize();
	void IncrementStreamBufferedSize();
	TInt StreamBufferedSize() const { return iBufferedSize; }
// Data properties:
protected:
	CMdaImageUtility*	iUtility;
	TOperation			iOperation;
	TState				iState;
	TInt				iReturnedError;
	TPtr8				iBuffer;		// used in via des streaming
	TPtr8				iCopyBuffer;
	TInt				iBufferedSize;	// running required buffer size for streaming
	TInt				iFileSize;
	TInt				iZoomFactor;
	RFile				iOrigFile;		// used in ReadViaFileL
	RFile				iTempFile;
	TInt				iTempFileSize;
// Linked data
	CFbsBitmap*			iBitmap;
	TPtrC				iFileName;
	RFs&				iFs;
	};

CTestImageIO* CTestImageIO::NewL(RFs &aFs)
	{
	CTestImageIO* result = NewLC(aFs);
	CleanupStack::Pop(); 
	return result;
	}

CTestImageIO* CTestImageIO::NewLC(RFs &aFs)
	{
	CTestImageIO* result = new (ELeave) CTestImageIO(aFs);
	CleanupStack::PushL(result);
	result->ConstructL();
	return result;
	}

CTestImageIO::CTestImageIO(RFs &aFs):
	iBuffer(NULL, 0, 0),
	iCopyBuffer(NULL, 0, 0),
	iFs(aFs)
	{
	}

void CTestImageIO::ConstructL()
	{
	const TInt copySize=KViaDesIncrement;
	iCopyBuffer.Set(STATIC_CAST(TUint8*, User::AllocL(copySize)), 0, copySize);
	}

CTestImageIO::~CTestImageIO()
	{
	delete iUtility;
	User::Free(REINTERPRET_CAST(TAny*,CONST_CAST(TUint8*, iBuffer.Ptr())));
	User::Free(REINTERPRET_CAST(TAny*,CONST_CAST(TUint8*, iCopyBuffer.Ptr())));
	CloseOrigAndTempFiles();
	}

void CTestImageIO::Leave(TInt aError)
	{
	iReturnedError = aError;
	CActiveScheduler::Stop();
	}

void CTestImageIO::OpenOrigAndTempFilesL()
	{
	User::LeaveIfError(iOrigFile.Open(iFs, iFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	User::LeaveIfError(iTempFile.Replace(iFs, KFailVideoTempTestFileName, EFileShareAny|EFileStream|EFileWrite));
	iTempFileSize = 0;
	}

void CTestImageIO::CloseOrigAndTempFiles()
	{
	iOrigFile.Close();
	iTempFile.Close();
	}

void CTestImageIO::CopyFileToTempL(TInt aSize)
	{
	TInt copySize = aSize - iTempFileSize;
	ASSERT(copySize >= 0); // should not occur

	if(copySize == 0)
		return;

	User::LeaveIfError(iOrigFile.Read(iCopyBuffer, copySize));

	ASSERT(iCopyBuffer.Length() == copySize); // should not have any shortfall

	User::LeaveIfError(iTempFile.Write(iCopyBuffer));

	iTempFileSize += copySize;
	}

void CTestImageIO::ResetStreamBufferedSize()
	{
	ASSERT(iFileSize>0); // don't copy with real empty files
	iBufferedSize = 1; // start stream at size 1
	}

void CTestImageIO::IncrementStreamBufferedSize()
	{
	TInt newSize;
	if (iBufferedSize < 100)
		newSize = iBufferedSize + 1;
	else
		newSize = iBufferedSize + KViaDesIncrement;
	iBufferedSize = Min(iFileSize, newSize);
#if defined(_DEBUG) && 0
	RDebug::Print(_L("IncrementStreamBuffer->%d"), iBufferedSize); 
#endif defined(_DEBUG)
	}


void CTestImageIO::ReadL(CFbsBitmap& aBitmap, const TDesC& aFileName, TInt aZoomFactor)
	{
	ASSERT(iState==EStateIdle); // ensure in idle state
	delete iUtility; iUtility=NULL; // could be non-NULL if previous error

	iFileName.Set(aFileName); // remember parameters
	iBitmap = &aBitmap;
	iZoomFactor = aZoomFactor;

	iOperation = ERead;

	iUtility = CMdaImageFileToBitmapUtility::NewL(*this, NULL);
	CMdaImageFileToBitmapUtility* utility = STATIC_CAST(CMdaImageFileToBitmapUtility*, iUtility);
	utility->OpenL(iFileName);
	iState = EStateOpening;
	CActiveScheduler::Start(); // recurse into AO

	ASSERT(iState == EStateIdle); // we should have come back here
	delete iUtility; iUtility = NULL;

	User::LeaveIfError(iReturnedError);
	}

void CTestImageIO::ReadViaDesL(CFbsBitmap& aBitmap, const TDesC& aFileName, TInt aZoomFactor)
	{
	ASSERT(iState==EStateIdle); // ensure in idle state
	delete iUtility; iUtility=NULL; // could be non-NULL if previous error

	iFileName.Set(aFileName); // remember parameters
	iBitmap = &aBitmap;
	iZoomFactor = aZoomFactor;

	iOperation = EReadViaDes;

	// read the whole of the original file into a buffer of the correct size
	RFile file;
	User::LeaveIfError(file.Open(iFs, aFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Size(iFileSize));
	iBuffer.Set(STATIC_CAST(TUint8 *, User::AllocL(iFileSize)), 0, iFileSize);
	User::LeaveIfError(file.Read(iBuffer));
	CleanupStack::PopAndDestroy(); // file

	ResetStreamBufferedSize(); // initial bit we try to read
	iBuffer.SetLength(StreamBufferedSize()); 

	iUtility = CMdaImageDescToBitmapUtility::NewL(*this, NULL);
	CMdaImageDescToBitmapUtility* utility = STATIC_CAST(CMdaImageDescToBitmapUtility*, iUtility);
	utility->OpenL(iBuffer);
	iState = EStateOpening;
	CActiveScheduler::Start(); // recurse into AO

	ASSERT(iState == EStateIdle); // we should have come back here
	delete iUtility; iUtility = NULL;

	User::LeaveIfError(iReturnedError);
	}

void CTestImageIO::ReadViaFileL(CFbsBitmap& aBitmap, const TDesC& aFileName, TInt aZoomFactor)
	{
	ASSERT(iState==EStateIdle); // ensure in idle state
	delete iUtility; iUtility=NULL; // could be non-NULL if previous error

	iFileName.Set(aFileName); // remember parameters
	iBitmap = &aBitmap;
	iZoomFactor = aZoomFactor;

	iOperation = EReadViaFile;

	TEntry entry;
	User::LeaveIfError(iFs.Entry(iFileName, entry));
	iFileSize = entry.iSize;

	// copy bytes from the original file, as required
	OpenOrigAndTempFilesL();
	ResetStreamBufferedSize(); // initial bit we try to read
	CopyFileToTempL(StreamBufferedSize()); 

	iUtility = CMdaImageFileToBitmapUtility::NewL(*this, NULL);
	CMdaImageFileToBitmapUtility* utility = STATIC_CAST(CMdaImageFileToBitmapUtility*, iUtility);
	TPtrC tempFileName(KFailVideoTempTestFileName);
	utility->OpenL(tempFileName);
	iState = EStateOpening;
	CActiveScheduler::Start(); // recurse into AO

	ASSERT(iState == EStateIdle); // we should have come back here
	delete iUtility; iUtility = NULL;

	CloseOrigAndTempFiles();
	(void) iFs.Delete(KFailVideoTempTestFileName); // delete temp file

	User::LeaveIfError(iReturnedError);
	}

void CTestImageIO::WriteL(CFbsBitmap& aBitmap, const TDesC& aFileName, TMdaClipFormat* aFormat,TMdaPackage* aCodec)
	{
	ASSERT(iState==EStateIdle); // ensure in idle state
	delete iUtility; iUtility=NULL; // could be non-NULL if previous error

	iFileName.Set(aFileName); // remember parameters
	iBitmap = &aBitmap;

	iOperation = EWrite;

	iUtility = CMdaImageBitmapToFileUtility::NewL(*this, NULL);
	CMdaImageBitmapToFileUtility* utility = STATIC_CAST(CMdaImageBitmapToFileUtility*, iUtility);
	utility->CreateL(iFileName, aFormat, aCodec, aCodec); 
		// note some of the standard codec's look at the standard codec info and some the extra
	iState = EStateOpening;
	CActiveScheduler::Start(); // recurse into AO

	ASSERT(iState == EStateIdle); // we should have come back here
	delete iUtility; iUtility = NULL;

	User::LeaveIfError(iReturnedError);
	}

void CTestImageIO::MiuoOpenComplete(TInt aError)
	{
	ASSERT(iOperation == ERead || iOperation == EReadViaDes || iOperation == EReadViaFile); // only these operations should get here

	ASSERT(iState == EStateOpening); // to be expected

	if (aError!=KErrNone)
		{
		if ((iOperation==EReadViaDes || iOperation==EReadViaFile) &&
			aError == KErrUnderflow && StreamBufferedSize() < iFileSize)
			{
			// doing stream operation and not yet enough data to open the file
			IncrementStreamBufferedSize();
			TInt error = KErrNone;
			if (iOperation == EReadViaDes)
				{
				iBuffer.SetLength(StreamBufferedSize());
				CMdaImageDescToBitmapUtility* utility = STATIC_CAST(CMdaImageDescToBitmapUtility*, iUtility);
				TRAP(error, utility->OpenL(iBuffer));
				}
			else
				{
				ASSERT(iOperation == EReadViaFile);
				TRAP(error, CopyFileToTempL(StreamBufferedSize()));
				CMdaImageFileToBitmapUtility* utility = STATIC_CAST(CMdaImageFileToBitmapUtility*, iUtility);
				TPtrC tempFileName(KFailVideoTempTestFileName);
				if (error==KErrNone)
					TRAP(error,utility->OpenL(tempFileName));
				}
			if (error != KErrNone)
				{
				iState = EStateIdle;
				Leave(error);
				}
			}
		else
			{
			iState = EStateIdle;
			Leave(aError);
			}
		}
	else
		{
		// note for readviades and viafile we only have to resize once - not on subsequent tries
		TFrameInfo frameInfo;
		iUtility->FrameInfo(0, frameInfo);
		TSize frameSize = frameInfo.iFrameCoordsInPixels.Size();
		frameSize.iWidth = (frameSize.iWidth+iZoomFactor-1) / iZoomFactor;
		frameSize.iHeight = (frameSize.iHeight+iZoomFactor-1) / iZoomFactor;
		TInt error = iBitmap->Resize(TSize(0,0));
		if (error==KErrNone)
			error = iBitmap->Resize(frameSize);
		if (error==KErrNone)
			TRAP(error, iUtility->ConvertL(*iBitmap));
		if (error!=KErrNone)
			{
			iState = EStateIdle;
			Leave(error);
			}
		else
			iState = EStateConverting;
		}
	}

void CTestImageIO::MiuoCreateComplete(TInt aError)
	{
	ASSERT(iOperation == EWrite); // only called on write operation

	ASSERT(iState == EStateOpening); // to be expected

	if (aError!=KErrNone)
		{
		iState = EStateIdle;
		Leave(aError);
		}
	else
		{
		TRAPD(error, iUtility->ConvertL(*iBitmap));
		if (error!=KErrNone)
			{
			iState = EStateIdle;
			Leave(error);
			}
		else
			iState = EStateConverting;
		}
	}

void CTestImageIO::MiuoConvertComplete(TInt aError)
	{
	ASSERT(iState == EStateConverting); // to be expected

	if (iOperation == ERead || iOperation == EWrite)
		{
		iState = EStateIdle;
		Leave(aError);
		}
	else
		{
		ASSERT(iOperation == EReadViaDes || iOperation == EReadViaFile);

		if (aError == KErrUnderflow && StreamBufferedSize() < iFileSize)
			// partial decode event - try to decode a bit more
			{
			IncrementStreamBufferedSize();
			TInt error = KErrNone;
			if (iOperation == EReadViaDes)
				iBuffer.SetLength(StreamBufferedSize());
			else
				{
				ASSERT(iOperation == EReadViaFile);
				TRAP(error, CopyFileToTempL(StreamBufferedSize()));
				}
			if (error==KErrNone)
				TRAP(error, iUtility->ConvertL(*iBitmap));
			if (error != KErrNone)
				{
				iState = EStateIdle;
				Leave(error);
				}
			}
		else
			{
			iState = EStateIdle;
			Leave(aError);
			}
		}
	}

//
// CMdaFailVid1
//

void CMdaFailVidTest1::DoTestL()
	{
	User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);

	CDir* dir = NULL;
	iFs.GetDir(KNullDesC,0,0,dir);
	CleanupStack::PushL(dir);

	TInt error=KErrNone;

	__UHEAP_MARK;
	TRAP(error,Test1L(*dir)); 
	iEngine->Print(_L("After Test1"));
	__UHEAP_MARKEND; 

	User::LeaveIfError(error);

	__UHEAP_MARK;
	TRAP(error,Test2L(*dir));
	iEngine->Print(_L("After Test2"));
	__UHEAP_MARKEND; 

	User::LeaveIfError(error);

	// preload and destroy CFbsBitmap to avoid "memory leak" - really side effect of
	// CFbsBitmap::Load() that is cleaned up properly later
		{
		TFileName fileName;
		User::LeaveIfError(iFs.DefaultPath(fileName));
		fileName.Append(KVideoTestSourceFileName);

		CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(srcBmp);
		User::LeaveIfError(srcBmp->Load(fileName));
		CleanupStack::PopAndDestroy(); // srcBmp
		}

	__UHEAP_MARK;
	TRAP(error,Test3L());
	iEngine->Print(_L("After Test3"));
	__UHEAP_MARKEND; 

	User::LeaveIfError(error);

	CleanupStack::PopAndDestroy(2); // dir, iFs
	}

void CMdaFailVidTest1::Test1L(CDir& aDir)
	{
	CFbsBitmap *bmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(TSize(0,0),EColor16M));
	CFbsBitmap *src = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(src);
	User::LeaveIfError(src->Create(TSize(0,0),EColor16M));

	TInt entries = aDir.Count();

	for (TInt count = 0; count < entries; count++)
		{
		__UHEAP_MARK;
		const TEntry& entry = aDir[count];

		TFileName fileName;
		User::LeaveIfError(iFs.DefaultPath(fileName));
		fileName.Append(entry.iName);

		TBuf<80> text;
		text.Zero();
		text.Append(_L("Load with alloc fail - "));
		text.Append(entry.iName);
		iEngine->Print(text);

		LoadImageL(*src,fileName,1);
		LoadImageWithAllocFailureL(*bmp,fileName,1);
		CheckBitmaps(*src,*bmp);

		LoadImageL(*src,fileName,8);
		LoadImageWithAllocFailureL(*bmp,fileName,8);
		CheckBitmaps(*src,*bmp);
		__UHEAP_MARKEND;
		}

	CleanupStack::PopAndDestroy(2); // src and bmp
	}

void CMdaFailVidTest1::Test2L(CDir& aDir)
	{
	CFbsBitmap *bmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(TSize(0,0),EColor16M));
	CFbsBitmap *bmp2 = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp2);
	User::LeaveIfError(bmp2->Create(TSize(0,0),EColor16M));
	CFbsBitmap *src = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(src);
	User::LeaveIfError(src->Create(TSize(0,0),EColor16M));

	TInt entries = aDir.Count();

	for (TInt count = 0; count < entries; count++)
		{
		__UHEAP_MARK;
		const TEntry& entry = aDir[count];

		TBuf<64> text;
		text.Zero();
		text.Append(_L("Stream with alloc fail - "));
		text.Append(entry.iName);
		iEngine->Print(text);

		TFileName fileName;
		User::LeaveIfError(iFs.DefaultPath(fileName));
		fileName.Append(entry.iName);

		StreamImageWithAllocFailureL(*bmp,fileName,1,EStreamViaDes);
		LoadImageL(*src,fileName,1);
		CheckBitmaps(*src,*bmp);
		StreamImageWithAllocFailureL(*bmp2,fileName,1,EStreamViaFile);
		CheckBitmaps(*bmp, *bmp2);

		StreamImageWithAllocFailureL(*bmp,fileName,8,EStreamViaDes);
		LoadImageL(*src,fileName,8);
		CheckBitmaps(*src,*bmp);
		StreamImageWithAllocFailureL(*bmp2,fileName,8,EStreamViaFile);
		CheckBitmaps(*bmp, *bmp2);
		__UHEAP_MARKEND;
		}

	CleanupStack::PopAndDestroy(3); // src, bmp2 and bmp
	}

void CMdaFailVidTest1::Test3L()
	{
	TFileName fileName;
	User::LeaveIfError(iFs.DefaultPath(fileName));
	fileName.Append(KVideoTestSourceFileName);

	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Load(fileName));

	for (TInt formatIndex = 0; ; formatIndex++)
		{
		TMdaClipFormat* format = NULL;
		TMdaPackage* codec = NULL;
		ConfigDestPortL(format,codec,formatIndex);

		if (format == NULL && codec == NULL)
			break;

		TBuf<64> text(_L("Save with alloc fail - "));
		text.Append(SaveFileName(formatIndex));
		iEngine->Print(text);

		SaveImageWithAllocFailureL(*srcBmp,format,codec); 

		iFs.Delete(KFailVideoTempTestFileName);
		}

	CleanupStack::PopAndDestroy(); // srcBmp
	}

void CMdaFailVidTest1::LoadImageWithAllocFailureL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor)
	{
	TInt failCount = 1;
	TInt err;

	__UHEAP_MARK;
	TRAP(err,LoadImageL(aBitmap,aFileName,aZoomFactor));
	if (err != KErrNone)
		iEngine->TestFailed(err);
	__UHEAP_MARKEND; 

	for(;;)	{
#if defined(_DEBUG) && 0 
		RDebug::Print(_L("CMdaFailVidTest1::LoadImageWithAllocFailureL(%S,%d) failCount=%d"), &aFileName, aZoomFactor, failCount);
#endif defined(_DEBUG)
		__UHEAP_SETFAIL(RHeap::EDeterministic, failCount);
		__UHEAP_MARK;

		TRAP(err,LoadImageL(aBitmap,aFileName,aZoomFactor));
		if (err != KErrNoMemory && err != KErrNone)
			iEngine->TestFailed(err);

		__UHEAP_MARKEND; 
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (err!=KErrNoMemory)
			break;
		failCount++;
		}
	failCount -= 1; // we are one over

	TBuf<80> format;
	format.Zero();
	format.AppendFormat(_L("  Completed OK at zoom factor %d with %d memory allocations tested"),aZoomFactor,failCount);
	iEngine->Print(format);
	}

void CMdaFailVidTest1::StreamImageWithAllocFailureL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor,TStreamMethod aStreamMethod)
	{
	TInt failCount = 1;
	TInt err;

#if defined(_DEBUG) && 0 
		RDebug::Print(_L("CMdaFailVidTest1::StreamImageWithAllocFailureL(%S,%d,%d)"), &aFileName, aZoomFactor, aStreamMethod);
#endif defined(_DEBUG)

	__UHEAP_MARK;
	TRAP(err,StreamImageL(aBitmap,aFileName,aZoomFactor,aStreamMethod));
  	if (err != KErrNone)
  		iEngine->TestFailed(err);
	__UHEAP_MARKEND; 
  
	for(;;)	{
#if defined(_DEBUG) && 0 
		RDebug::Print(_L("CMdaFailVidTest1::StreamImageWithAllocFailureL(%S,%d,%d) failCount=%d"), &aFileName, aZoomFactor, aStreamMethod, failCount);
#endif defined(_DEBUG)
		__UHEAP_SETFAIL(RHeap::EDeterministic, failCount);
		__UHEAP_MARK;

		TRAP(err,StreamImageL(aBitmap,aFileName,aZoomFactor,aStreamMethod));
		if (err != KErrNoMemory && err != KErrNone)
			iEngine->TestFailed(err);

		__UHEAP_MARKEND; 
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (err!=KErrNoMemory)
			break;
		failCount++;
		}
	failCount -= 1; // we are one over

	TBuf<80> format;
	format.Zero();
	TPtrC method;
	if (aStreamMethod==EStreamViaDes)
		method.Set(_L("Desc"));
	else
		method.Set(_L("File"));
	format.AppendFormat(_L("  Completed OK (%S) at zoom factor %d with %d memory allocations tested"),&method,aZoomFactor,failCount);
	iEngine->Print(format);
	}

void CMdaFailVidTest1::LoadImageL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor)
	{
	CTestImageIO* testIO = CTestImageIO::NewLC(iFs);
	testIO->ReadL(aBitmap, aFileName, aZoomFactor);
	CleanupStack::PopAndDestroy(); // testIO
	}

void CMdaFailVidTest1::StreamImageL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor, TStreamMethod aStreamMethod)
	{
	CTestImageIO* testIO = CTestImageIO::NewLC(iFs);
	if (aStreamMethod == EStreamViaDes)	
		testIO->ReadViaDesL(aBitmap, aFileName, aZoomFactor);
	else
		{
		ASSERT(aStreamMethod==EStreamViaFile);
		testIO->ReadViaFileL(aBitmap, aFileName, aZoomFactor);
		}
	CleanupStack::PopAndDestroy(); // testIO
	}

void CMdaFailVidTest1::SaveImageWithAllocFailureL(CFbsBitmap& aBitmap,TMdaClipFormat* aFormat,TMdaPackage* aCodec)
	{
	TInt failCount = 1;
	TInt err;

	__UHEAP_MARK;
	TRAP(err,SaveImageL(aBitmap,aFormat,aCodec));
	if (err != KErrNone)
		iEngine->TestFailed(err);
	__UHEAP_MARKEND; 

	for(;;)	{
		__UHEAP_SETFAIL(RHeap::EDeterministic, failCount);
		__UHEAP_MARK;

		TRAP(err,SaveImageL(aBitmap,aFormat,aCodec));
		if (err != KErrNoMemory && err != KErrNone)
			iEngine->TestFailed(err);

		__UHEAP_MARKEND; 
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (err != KErrNoMemory)
			break;

		failCount++;
		}
	failCount -= 1; // we are one over

	TBuf<80> format;
	format.Zero();
	format.AppendFormat(_L("  Completed OK with %d memory allocations tested"),failCount);
	iEngine->Print(format);
	}


void CMdaFailVidTest1::SaveImageL(CFbsBitmap& aBitmap,TMdaClipFormat* aFormat,TMdaPackage* aCodec)
	{
	ASSERT(aFormat);

	iFs.Delete(KFailVideoTempTestFileName);

#if 0
	RMdaSourceStreamPort srcPort;
	srcPort.OpenLC(iSession);
	TMdaFbsBitmapDevice bmpDev;
	srcPort.ResourceConfigL(bmpDev);
	TMdaFbsBitmapHandle bmpHandle;
	bmpHandle.iBitmapHandle = aBitmap.Handle();
	srcPort.PortConfigL(bmpHandle);

	RMdaDestinationClipPort dstPort;
	dstPort.OpenLC(iSession);
	TMdaFileClipLocation location;
	location.iName = KFailVideoTempTestFileName;
	dstPort.ResourceConfigL(location);
	dstPort.ResourceConfigL(*aFormat);
	if (aCodec)
		dstPort.ResourceConfigL(*aCodec);

	RMdaController controller;
	controller.OpenLC(iSession);
	TMdaConnection connection(srcPort,dstPort,KUidMdaMediaTypeVideo);
	controller.ConnectL(connection);
	controller.PrepareL();
	controller.PrimeL();

	CleanupStack::PopAndDestroy(3); // controller, dstPort, srcPort
#else 0
	CTestImageIO* testIO = CTestImageIO::NewLC(iFs);
	testIO->WriteL(aBitmap, KFailVideoTempTestFileName, aFormat, aCodec);
	CleanupStack::PopAndDestroy(); // testIO
#endif 0
	}

const TDesC& CMdaFailVidTest1::SaveFileName(TInt aFormatIndex)
	{
	switch (aFormatIndex)
		{
	case 0:		return KVideoTest1BppMonoMbm;
	case 1:		return KVideoTest2BppMonoMbm;
	case 2:		return KVideoTest4BppMonoMbm;
	case 3:		return KVideoTest8BppMonoMbm;
	case 4:		return KVideoTest4BppColorMbm;
	case 5:		return KVideoTest8BppColorMbm;
	case 6:		return KVideoTest12BppColorMbm;
	case 7:		return KVideoTest16BppColorMbm;
	case 8:		return KVideoTest24BppColorMbm;
	case 9:		return KVideoTestMono10Jfif;
	case 10:	return KVideoTestMono30Jfif;
	case 11:	return KVideoTestMono60Jfif;
	case 12:	return KVideoTestMono100Jfif;
	case 13:	return KVideoTest420C10Jfif;
	case 14:	return KVideoTest420C30Jfif;
	case 15:	return KVideoTest420C60Jfif;
	case 16:	return KVideoTest420C100Jfif;
	case 17:	return KVideoTest422C10Jfif;
	case 18:	return KVideoTest422C30Jfif;
	case 19:	return KVideoTest422C60Jfif;
	case 20:	return KVideoTest422C100Jfif;
	case 21:	return KVideoTest444C10Jfif;
	case 22:	return KVideoTest444C30Jfif;
	case 23:	return KVideoTest444C60Jfif;
	case 24:	return KVideoTest444C100Jfif;
	case 25:	return KVideoTest1BppBmp;
	case 26:	return KVideoTest4BppBmp;
	case 27:	return KVideoTest8BppBmp;
	case 28:	return KVideoTest24BppBmp;
	default:	return KNullDesC;
		}
	}

void CMdaFailVidTest1::ConfigDestPortL(TMdaClipFormat*& aFormat,TMdaPackage*& aCodec,TInt aFormatIndex)
	{
	aFormat = NULL;
	aCodec = NULL;

	if (aFormatIndex >= 0 && aFormatIndex <= 8)
		{
		switch (aFormatIndex)
			{
		case 0:		iMbmFormat.iDisplayMode = EGray2;		break;
		case 1:		iMbmFormat.iDisplayMode = EGray4;		break;
		case 2:		iMbmFormat.iDisplayMode = EGray16;		break;
		case 3:		iMbmFormat.iDisplayMode = EGray256;		break;
		case 4:		iMbmFormat.iDisplayMode = EColor16;		break;
		case 5:		iMbmFormat.iDisplayMode = EColor256;	break;
		case 6:		iMbmFormat.iDisplayMode = EColor4K;		break;
		case 7:		iMbmFormat.iDisplayMode = EColor64K;	break;
		case 8:		iMbmFormat.iDisplayMode = EColor16M;	break;
		default:	User::Invariant();
			}
		aFormat = &iMbmFormat;
		}
	else if (aFormatIndex >= 9 && aFormatIndex <= 24)
		{
		switch (aFormatIndex)
			{
		case 9:		case 10:	case 11:	case 12:
			iJfifFormat.iSettings.iSampleScheme = TMdaJpgSettings::EMonochrome;
			break;
		case 13:	case 14:	case 15:	case 16:
			iJfifFormat.iSettings.iSampleScheme = TMdaJpgSettings::EColor420;
			break;
		case 17:	case 18:	case 19:	case 20:
			iJfifFormat.iSettings.iSampleScheme = TMdaJpgSettings::EColor422;
			break;
		case 21:	case 22:	case 23:	case 24:
			iJfifFormat.iSettings.iSampleScheme = TMdaJpgSettings::EColor444;
			break;
		default:	User::Invariant();
			}
		switch (aFormatIndex)
			{
		case 9:		case 13:	case 17:	case 21:
			iJfifFormat.iSettings.iQualityFactor = 10;
			break;
		case 10:	case 14:	case 18:	case 22:
			iJfifFormat.iSettings.iQualityFactor = 30;
			break;
		case 11:	case 15:	case 19:	case 23:
			iJfifFormat.iSettings.iQualityFactor = 60;
			break;
		case 12:	case 16:	case 20:	case 24:
			iJfifFormat.iSettings.iQualityFactor = 100;
			break;
		default:	User::Invariant();
			}
		aFormat = &iJfifFormat;
		}
	else if (aFormatIndex >= 25 && aFormatIndex <= 28)
		{
		switch (aFormatIndex)
			{
		case 25:	aCodec = &iBmp1BppCodec;	break;
		case 26:	aCodec = &iBmp4BppCodec;	break;
		case 27:	aCodec = &iBmp8BppCodec;	break;
		case 28:	aCodec = &iBmp24BppCodec;	break;
		default:	User::Invariant();
			}
		aFormat = &iBmpFormat;
		}
	}

