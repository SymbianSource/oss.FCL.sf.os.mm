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

// MDA includes
#include <mdaimageconverter.h>
#include "MdaImageConverter.inl"

// Test system includes
#include <testframework.h>
#include "TestStepMediaSvr.h"
#include "TestSuiteMediaSvr.h"

// Directory that the test files will be stored on 
_LIT(KMediaSvrTestSourcePathOnC,"c:\\MediaSvrTest\\");
_LIT(KMediaSvrTestSourcePathOnD,"d:\\MediaSvrTest\\");
_LIT(KMediaSvrTestSourcePathOnE,"e:\\MediaSvrTest\\");
_LIT(KMediaSvrTestSourcePathOnZ,"z:\\MediaSvrTest\\");

/**
 *
 * CTestStepMediaSvr constructor
 *
 * @xxxx
 * 
 */
CTestStepMediaSvr::CTestStepMediaSvr() 
	{
	// set up parameters for member format functions
	// this is done in this way due to the awkward design of the API being tested
	iJfifFormatMonoQ50.iSettings.iSampleScheme = TMdaJpgSettings::EMonochrome;
	iJfifFormatColor420Q50.iSettings.iSampleScheme = TMdaJpgSettings::EColor420;
	iJfifFormatColor422Q50.iSettings.iSampleScheme = TMdaJpgSettings::EColor422;
	iJfifFormatColor444Q50.iSettings.iSampleScheme = TMdaJpgSettings::EColor444;
	iJfifFormatMonoQ50.iSettings.iQualityFactor = 50;
	iJfifFormatColor420Q50.iSettings.iQualityFactor = 50;
	iJfifFormatColor422Q50.iSettings.iQualityFactor = 50;
	iJfifFormatColor444Q50.iSettings.iQualityFactor = 50;

	iJfifFormatColor444Q10.iSettings.iQualityFactor = 10;
	iJfifFormatColor444Q30.iSettings.iQualityFactor = 30;
	iJfifFormatColor444Q60.iSettings.iQualityFactor = 60;
	iJfifFormatColor444Q100.iSettings.iQualityFactor = 100;
	
	iMbmFormatGray2.iDisplayMode = EGray2;
	iMbmFormatGray4.iDisplayMode = EGray4;
	iMbmFormatGray16.iDisplayMode = EGray16;
	iMbmFormatGray256.iDisplayMode = EGray256;
	iMbmFormatColor16.iDisplayMode = EColor16;
	iMbmFormatColor256.iDisplayMode = EColor256;
	iMbmFormatColor4K.iDisplayMode = EColor4K;
	iMbmFormatColor64K.iDisplayMode = EColor64K;
	iMbmFormatColor16M.iDisplayMode = EColor16M;
	}

TVerdict CTestStepMediaSvr::DoTestStepPreambleL()
	{
	// connect to the bitmap server and check the error code
    User::LeaveIfError(FbsStartup());
	TInt errCode = RFbsSession::Connect();
	if( errCode != KErrNone )
		{
		 //[failed to connect to bitmap server 
		 ERR_PRINTF2(_L("Failed to connect to bitmap server in teststep preamble = %d"), errCode );
		 return EFail;
		}
	
	iScheduler = new(ELeave)CActiveScheduler;
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
	
	InitSystemPath();
	
	return EPass;
	}

/**
 * The post amble step cleans up the allocated memory in the preamble
 * @returns TVerdict EPass or EFail
 * @leaves can leave due to insufficient memory
 * @lib "TS_BitmapTransforms.lib"
 * @xxxx
 */

TVerdict CTestStepMediaSvr::DoTestStepPostambleL()
	{
	// Destroy the scheduler
	CActiveScheduler::Install(NULL); // Uninstall the active scheduler
	delete iScheduler;
	iScheduler = NULL;
	// disconnect from the bitmap server
	RFbsSession::Disconnect();
	return EPass;
	}

void CTestStepMediaSvr::Complete(TInt aError) 
	{
	iError = aError;
	CActiveScheduler::Stop();
	}

void CTestStepMediaSvr::MiuoCreateComplete(TInt aError)
	{
	Complete(aError);
	}

void CTestStepMediaSvr::MiuoOpenComplete(TInt aError)
	{
	Complete(aError);
	}
void CTestStepMediaSvr::MiuoConvertComplete(TInt aError)
	{
	Complete(aError);
	}

TInt CTestStepMediaSvr::WaitForResult() 
	{
	CActiveScheduler::Start();
	return iError;
	}

/**
 *
 * Intialiases the default system path for the tests
 * 
 * @xxxx
 * 
 */
void CTestStepMediaSvr::InitSystemPath()
	{
	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KMediaSvrTestSourcePathOnE,att) == KErrNone)
		iDefaultPath = KMediaSvrTestSourcePathOnE;
	else if (fs.Att(KMediaSvrTestSourcePathOnD,att) == KErrNone)
		iDefaultPath = KMediaSvrTestSourcePathOnD;
	else if (fs.Att(KMediaSvrTestSourcePathOnC,att) == KErrNone)
		iDefaultPath = KMediaSvrTestSourcePathOnC;
	else if (fs.Att(KMediaSvrTestSourcePathOnZ,att) == KErrNone)
		iDefaultPath = KMediaSvrTestSourcePathOnZ;
	else
		User::Panic(_L("Test files not found"),KErrNotFound);

	fs.Close();
	}
	
const TDesC& CTestStepMediaSvr::DefaultPath() const
	{
	return iDefaultPath;
	}
	
/**
 *
 * Load an image from a file 
 *
 * @param "const TDesC& aFileName"
 *		  Name of file to load
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read from the file	
 * @xxxx
 *
 */
CFbsBitmap* CTestStepMediaSvr::LoadImageL(const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	
	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);
	
	TFileName fileName = iDefaultPath;
	fileName.Append(aFileName);
	
	ftob->OpenL(fileName,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);

	TFrameInfo frameInfo;

//  TODO: Note - doing it this way causes a corrupt iDecoder pointer to be
//  returned. Not sure why this happens at present time!
//  (CTestStepMediaSvr must be a friend of CMdaImageFileToBitmapUtility)
//
//	CMdaImageFileToBitmapUtilityPriv* test = ftob->Properties();
//	frameInfo = ftob->Properties()->iDecoder->FrameInfo(0);

	ftob->FrameInfo(0,frameInfo);

	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));
	ftob->ConvertL(*bmp,0);
	WaitForResult();
	User::LeaveIfError(iError);
	CleanupStack::PopAndDestroy(); //ftob
	CleanupStack::Pop(); //bmp;
	return bmp;
	}
/**
 *
 * Test loading a transparent image from a file 
 *
 * @param "CFbsBitmap* aBitmap"
 *		  Bitmap to store the image
 * @param "CFbsBitmap* aMask"
 *		  Bitmap to store the mask
 * @param "TDesC& aFileName"
 *		  Name of file to load
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 *
 * @xxxx
 *
 */
void CTestStepMediaSvr::LoadTransparentImageL(CFbsBitmap* aBitmap, CFbsBitmap* aMask,const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{	
	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);
	
	TFileName fileName = iDefaultPath;
	fileName.Append(aFileName);
	
	ftob->OpenL(fileName,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);

	TFrameInfo frameInfo;
	ftob->FrameInfo(0,frameInfo);
	User::LeaveIfError(aBitmap->Create(frameInfo.iOverallSizeInPixels,EGray2));
	User::LeaveIfError(aMask->Create(frameInfo.iOverallSizeInPixels,EGray2));
	ftob->ConvertL(*aBitmap,*aMask,0);
	WaitForResult();
	CleanupStack::PopAndDestroy(); //ftob
	User::LeaveIfError(iError);
	}
/**
 *
 * Test loading a transparent image from a file, specifying an incorrect format for the target
 * bitmaps. The Convert should fail gracefully
 *
 * @param "CFbsBitmap* aBitmap"
 *		  Bitmap to store the image
 * @param "CFbsBitmap* aMask"
 *		  Bitmap to store the mask
 * @param "TDesC& aFileName"
 *		  Name of file to load
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered opening
 *
 * @xxxx
 *
 */
void CTestStepMediaSvr::FailLoadTransparentImageL(CFbsBitmap* aBitmap, CFbsBitmap* aMask,const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{	
	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);

	TFileName fileName = iDefaultPath;
	fileName.Append(aFileName);

	ftob->OpenL(fileName,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);

	TFrameInfo frameInfo;
	ftob->FrameInfo(0,frameInfo);
	User::LeaveIfError(aBitmap->Create(frameInfo.iOverallSizeInPixels,EColor4K));
	User::LeaveIfError(aMask->Create(frameInfo.iOverallSizeInPixels,EColor4K));
	ftob->ConvertL(*aBitmap,*aMask,0);
	WaitForResult();
	CleanupStack::PopAndDestroy(); //ftob
	User::LeaveIfError(iError);
	}

/**
 *
 * Tests loading an image from a file, cancelling and restarting the operation several times before
 * allowing the load to complete
 *
 * @param "TDesC& aFileName"
 *		  Name of file to load
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read from the file	
 * @xxxx
 *
 */
CFbsBitmap* CTestStepMediaSvr::CancelLoadImageL(const TDesC& imageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
	
	TFrameInfo frameInfo;

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	
	
	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);

	TFileName fileName = iDefaultPath;
	fileName.Append(imageFileName);
	
	ftob->OpenL(fileName,aFormat,aCodec);

	WaitForResult();
	User::LeaveIfError(iError);

		
	ftob->FrameInfo(0,frameInfo);
	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));
	ftob->ConvertL(*bmp,0);
	ftob->CancelConvertL();
	ftob->ConvertL(*bmp,0);
	ftob->CancelConvertL();

	ftob->ConvertL(*bmp,0);

	WaitForResult();
	User::LeaveIfError(iError);
	CleanupStack::PopAndDestroy(); //ftob

	CleanupStack::Pop(); //bmp;
	return bmp;
	}
/**
 *
 * Save an image to a File
 *
 * @param "CFbsBitmap* aBitmap"
 *		  Bitmap to write to file
 * @param "const TDesC& aImageFileName"
 *		  Filename to write to
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered writing the file	
 * @xxxx
 *
 */
void CTestStepMediaSvr::SaveImageL(CFbsBitmap* aBitmap, const TDesC& aImageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
	CMdaImageBitmapToFileUtility* ftob = CMdaImageBitmapToFileUtility::NewL(*this);
	CleanupStack::PushL(ftob);

	TFileName fileName = iDefaultPath;
	fileName.Append(aImageFileName);
	ftob->CreateL(fileName,aFormat,aCodec,NULL);

	WaitForResult();
	User::LeaveIfError(iError);		
		
	ftob->ConvertL(*aBitmap,0);
	WaitForResult();
	User::LeaveIfError(iError);

	CleanupStack::PopAndDestroy(); //ftob
	}
/**
 *
 * Save an image to a File, cancelling and restarting the operation a few times before it
 * is allowed to complete
 *
 * @param "CFbsBitmap* aBitmap"
 *		  Bitmap to write to file
 * @param "const TDesC& aImageFileName"
 *		  Filename to write to
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered writing the file	
 * @xxxx
 *
 */
void CTestStepMediaSvr::CancelSaveImageL(CFbsBitmap* aBitmap, const TDesC& aImageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
	CMdaImageBitmapToFileUtility* ftob = CMdaImageBitmapToFileUtility::NewL(*this);
	CleanupStack::PushL(ftob);

	TFileName fileName = iDefaultPath;
	fileName.Append(aImageFileName);
	ftob->CreateL(fileName,aFormat,aCodec,NULL);

	WaitForResult();
	User::LeaveIfError(iError);		
		
	ftob->ConvertL(*aBitmap,0);
	ftob->CancelConvertL();
	ftob->ConvertL(*aBitmap,0);
	ftob->CancelConvertL();
	ftob->ConvertL(*aBitmap,0);

	WaitForResult();
	User::LeaveIfError(iError);

	CleanupStack::PopAndDestroy(); //ftob
	}

/**
 *
 * Tests streaming an image from a descriptor 
 *
 * @param "TPtr8 aImageBuffer"
 *		  Descriptor containing image
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read from the descriptor	
 * @xxxx
 *
 */
CFbsBitmap* CTestStepMediaSvr::StreamImageFromDescriptorL(const TPtr8& aImageBuffer, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{ 
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);

	CMdaImageDescToBitmapUtility* dtob = CMdaImageDescToBitmapUtility::NewL(*this);
	CleanupStack::PushL(dtob);
	dtob->OpenL(aImageBuffer,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);

	TFrameInfo frameInfo;
	dtob->FrameInfo(0,frameInfo);
	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));

	TPtr8& buf = CONST_CAST(TPtr8&,aImageBuffer); // we can't modify const parameter, so make a copy
	TInt len = buf.Length();
	TInt templen = 1;
	dtob->ConvertL(*bmp,0);
	WaitForResult();
	while (iError==KErrUnderflow && templen < len) 
		{
		if (len - templen <128)
			templen+=128;
		else
			templen = len;

		buf.SetLength(templen);
		dtob->ConvertL(*bmp,0);
		WaitForResult();
		}
	User::LeaveIfError(iError);
	CleanupStack::PopAndDestroy(); // dtob
	CleanupStack::Pop(); // bmp
	return bmp;
	}

/**
 *
 * Tests streaming an image from a file 
 *
 * @param "TDesC& aFileName"
 *		  Name of file to load
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read
 * @xxxx
 *
 */
CFbsBitmap* CTestStepMediaSvr::StreamImageL(const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{ 
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);

	TBuf<256> tempFileName;


	RFile srcFile, tempFile;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	TFileName fileName = iDefaultPath;
	fileName.Append(aFileName);
	User::LeaveIfError(srcFile.Open(fs,fileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(srcFile);

	User::LeaveIfError(tempFile.Temp(fs,_L("c:\\"),tempFileName,EFileShareAny|EFileStream|EFileWrite));
	CleanupClosePushL(tempFile);

	TInt fileSize;

	TInt copySize = 128;
	
	User::LeaveIfError(srcFile.Size(fileSize));
	HBufC8* copyBuf = HBufC8::NewMaxLC(copySize);
	TPtr8 copyBufPtr(copyBuf->Des());

	TInt templen = copySize;

	User::LeaveIfError(srcFile.Read(copyBufPtr,copySize));
	User::LeaveIfError(tempFile.Write(copyBufPtr,copySize));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);
	
	ftob->OpenL(tempFileName,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);
		
	TFrameInfo frameInfo;
	ftob->FrameInfo(0,frameInfo);
	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));

	
	ftob->ConvertL(*bmp,0);
	WaitForResult();
	while (iError==KErrUnderflow && templen < fileSize) 
		{
		TInt copySize;
		if (fileSize - templen >128)
			copySize=128;
		else
			copySize= fileSize - templen;

		User::LeaveIfError(srcFile.Read(copyBufPtr,copySize));
		User::LeaveIfError(tempFile.Write(copyBufPtr,copySize));
		templen+=copySize;
		
		ftob->ConvertL(*bmp,0);
		WaitForResult();
		}
	User::LeaveIfError(iError);

	srcFile.Close();

	CleanupStack::PopAndDestroy(); // ftob, must close in order to delete the temporary file
	
	tempFile.Close();
	fs.Delete(tempFileName);

	CleanupStack::PopAndDestroy(4); // tempFile,srcFile, copyBuf, fs
	CleanupStack::Pop(); // bmp
	return bmp;
	}
/**
 *
 * Tests loading an image from a File, with additional alloc failure testing
 *
 * @param "const TDesC& aImageFileName"
 *		  Filename of the image to load
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read from the descriptor	
 * @xxxx
 *
 */
CFbsBitmap* CTestStepMediaSvr::LoadImageWithAllocFailL(const TDesC& aImageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
	TInt failCount = 1;
	TInt err;

	CFbsBitmap* bmp = NULL;

	TRAP(err,bmp = LoadImageL(aImageFileName,aFormat,aCodec));
	CleanupStack::PushL(bmp);
	User::LeaveIfError(err);
	
	CleanupStack::PopAndDestroy();//bmp

	for(;;)	{
		__UHEAP_SETFAIL(RHeap::EDeterministic, failCount);

		TRAP(err,bmp = LoadImageL(aImageFileName,aFormat,aCodec));

		
		if (err != KErrNoMemory && err != KErrNone)
			User::Leave(err);

		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (err!=KErrNoMemory)
			break;
		
		++failCount;
		}
	--failCount; // we are one over
	INFO_PRINTF2(_L("Completed OK with %d memory allocations tested"),failCount);
	return bmp;
	}
/**
 *
 * Tests saving an image to a File, with additional alloc failure testing
 *
 * @param "CFbsBitmap* aBitmap"
 *		  Bitmap to write to file
 * @param "const TDesC& aImageFileName"
 *		  Filename to write to
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered writing the file	
 * @xxxx
 *
 */
void CTestStepMediaSvr::SaveImageWithAllocFailL(CFbsBitmap* aBitmap, const TDesC& aImageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
		TInt failCount = 1;
	TInt err;

	__MM_HEAP_MARK;
	TRAP(err, SaveImageL(aBitmap, aImageFileName,aFormat,aCodec));
	User::LeaveIfError(err);
	__MM_HEAP_MARKEND; 

	for(;;)	{
		__UHEAP_SETFAIL(RHeap::EDeterministic, failCount);
		__MM_HEAP_MARK;
		TRAP(err, SaveImageL(aBitmap, aImageFileName,aFormat,aCodec));

		if (err != KErrNoMemory && err != KErrNone)
			User::Leave(err);

		__MM_HEAP_MARKEND; 
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (err!=KErrNoMemory)
			break;
			
		++failCount;
		}
	--failCount; // we are one over

	INFO_PRINTF2(_L("Completed OK with %d memory allocations tested"),failCount);
	}
/**
 *
 * Tests streaming an image from a descriptor, with additional alloc failure testing
 *
 * @param "TPtr8 aImageBuffer"
 *		  Descriptor containing image
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read from the descriptor	
 * @xxxx
 *
 */
CFbsBitmap*  CTestStepMediaSvr::StreamImageFromDescWithAllocFailL(const TPtr8& aImageBuffer, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{
	TInt failCount = 1;
	TInt err;

	CFbsBitmap* bmp = NULL;

	__MM_HEAP_MARK;
	TRAP(err,bmp = StreamImageFromDescriptorL(aImageBuffer,aFormat,aCodec));
	CleanupStack::PushL(bmp);
	User::LeaveIfError(err);
	

	CleanupStack::PopAndDestroy(); //bmp
	__MM_HEAP_MARKEND; 

	for(;;)	
		{
		__UHEAP_SETFAIL(RHeap::EDeterministic, failCount);

		TRAP(err,bmp = StreamImageFromDescriptorL(aImageBuffer,aFormat,aCodec));
	
		if (err != KErrNoMemory && err != KErrNone)
			User::Leave(err);

		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (err!=KErrNoMemory)
			break;

		++failCount;
		}
	--failCount; // we are one over

	INFO_PRINTF2(_L("Completed OK with %d memory allocations tested"),failCount);
	return bmp;
	}
/**
 *
 * Check that the contents of the two bitmaps provided are identical
 *
 * @param "CFbsBitmap& aBitmap1"
 *		  The first bitmap to compare
 * @param "CFbsBitmap& aBitmap2"
 *		  The second bitmap to compare
 * @return "TVerdict"
 *		  EPass if the files are identical
 *		  EFail otherwise
 * @xxxx
 *
 */
TVerdict CTestStepMediaSvr::CheckBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2) const
	{
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
				{
				return EFail;
				}
			}
		}
	return EPass;
	}
/*
 *
 * Writes an image to a descriptor 
 *
 * @param "CFbsBitmap* aBitmap"
 *		  Bitmap to write to descriptor
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @param "TInt aDesSize"
 *		  Optional explicit setting of the descriptor size to use
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "HBufC8*"
 *		  The descriptor containing the image	
 * @xxxx
 *
 */
HBufC8* CTestStepMediaSvr::WriteImageToDescriptorL(CFbsBitmap* aBitmap, TMdaClipFormat* aFormat, TMdaPackage* aCodec, TInt aDesSize)
	{ 
	const TSize size = aBitmap->SizeInPixels();
	const TInt descSize = aDesSize>0 ? aDesSize : ((size.iWidth * size.iHeight * 3) + 10000);
	HBufC8* descBuf = HBufC8::NewLC(descSize);
	TPtr8 imageBuffer(descBuf->Des());

	CMdaImageBitmapToDescUtility* btod = CMdaImageBitmapToDescUtility::NewL(*this);
	CleanupStack::PushL(btod);
	btod->CreateL(imageBuffer,aFormat,aCodec,NULL);
	WaitForResult();
	User::LeaveIfError(iError);
	btod->ConvertL(*aBitmap);
	WaitForResult();
	User::LeaveIfError(iError);
	CleanupStack::PopAndDestroy(); // btod
	CleanupStack::Pop(); // descBuf
	return descBuf;
	}

/**
 *
 * Reads an image from a descriptor 
 *
 * @param "TPtr8 aImageBuffer"
 *		  Descriptor containing image
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading the file	
 * @return "CFbsBitmap*"
 *		  The bitmap read from the descriptor	
 * @xxxx
 *
 */
CFbsBitmap* CTestStepMediaSvr::ReadImageFromDescriptorL(const TPtr8& aImageBuffer, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{ 
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);

	CMdaImageDescToBitmapUtility* dtob = CMdaImageDescToBitmapUtility::NewL(*this);
	CleanupStack::PushL(dtob);
	dtob->OpenL(aImageBuffer,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);

	TFrameInfo frameInfo;
	dtob->FrameInfo(0,frameInfo);
	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));
	dtob->ConvertL(*bmp,0);
	WaitForResult();
	User::LeaveIfError(iError);
	CleanupStack::PopAndDestroy(); // dtob
	CleanupStack::Pop(); // bmp
	return bmp;
	}
/**
 *
 * Writes a message to the log. Intended for tests which have multiple test
 * cases
 *
 * @param "TDesC& aContext"
 *		  The context of the failure eg. The test file that gave rise to the message
 * @param "TDesC& aMessage"
 *		  A message to log
 * @xxxx
 *
 */
void CTestStepMediaSvr::LogMessage(const TDesC& aContext, const TDesC& aMessage) 
	{
	INFO_PRINTF3(_L("%S: %S"),&aContext,&aMessage);
	}

/**
 *
 * Writes a error to the log. Intended for tests which have multiple test
 * cases
 *
 * @param "TDesC& aContext"
 *		  The context of the failure eg. The test file that gave rise to the message
 * @param "TDesC& aMessage"
 *		  A message to log
 * @xxxx
 *
 */
void CTestStepMediaSvr::LogError(const TDesC& aContext, const TDesC& aMessage) 
	{
	ERR_PRINTF3(_L("%S: %S"),&aContext,&aMessage);	
	}

/**
 *
 * Compare two binary files
 *
 * @param "const TDesC& aFile1"
 *		  The first file to compare
 * @param "const TDesC& aFile2"
 *		  The second file to compare
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered reading either file 	
 * @return "TVerdict"
 *		  EPass if the files are identical
 *		  EFail otherwise
 * @xxxx
 *
 */
TVerdict CTestStepMediaSvr::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2) const
	{
	RFile file1, file2;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	TFileName fileName1 = iDefaultPath;
	fileName1.Append(aFile1);
	User::LeaveIfError(file1.Open(fs,fileName1,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file1);

	TFileName fileName2 = iDefaultPath;
	fileName2.Append(aFile2);
	User::LeaveIfError(file2.Open(fs,fileName2,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file2);

	TInt fileSize1, fileSize2;

	TVerdict result = EPass;
	TInt blockSize = 1024;
	
	User::LeaveIfError(file1.Size(fileSize1));
	User::LeaveIfError(file2.Size(fileSize2));

	if (fileSize1 == fileSize2) 
		{
		HBufC8* buf1 = HBufC8::NewMaxLC(blockSize);
		TPtr8 buf1Ptr(buf1->Des());
		HBufC8* buf2 = HBufC8::NewMaxLC(blockSize);
		TPtr8 buf2Ptr(buf2->Des());

		TInt length = 0;

		while (length<fileSize1 && result == EPass)
			{
			TInt copySize;

			if (fileSize1 - length > blockSize)
				copySize = blockSize;
			else
				copySize = fileSize1 - length;
			User::LeaveIfError(file1.Read(buf1Ptr,copySize));
			User::LeaveIfError(file2.Read(buf2Ptr,copySize));

			if (Mem::Compare(buf1Ptr.Ptr(),copySize,buf2Ptr.Ptr(),copySize)!=0)
				result = EFail;

			length += copySize;
			}
		CleanupStack::PopAndDestroy(2); // buf1, buf2
		}
	else
		result = EFail;

	file1.Close();
	file2.Close();
	fs.Close();

	CleanupStack::PopAndDestroy(3); //fs, file1, file2

	return result;
	}
/**
 *
 * Retrieves frame info from a file
 *
 * @param "TDesC& aFileName"
 *		  Name of file to open
 * @param "TMdaClipFormat* aFormat"
 *		  Format of the image
 * @param "TMdaPackage* aCodec"
 *		  Format of the image
 * @leave "" 
 *		  Will leave with appropriate system codes if a problem is encountered opening the file
 * @return "TFrameInfo"
 *		  The frame information
 * @xxxx
 *
 */
TFrameInfo CTestStepMediaSvr::GetFrameInfoL(const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec)
	{	
	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);

	TFileName fileName = iDefaultPath;
	fileName.Append(aFileName);

	ftob->OpenL(fileName,aFormat,aCodec);
	WaitForResult();
	User::LeaveIfError(iError);
	TFrameInfo frameInfo;
	ftob->FrameInfo(0,frameInfo);
	CleanupStack::PopAndDestroy(); //ftob
	return frameInfo;
	}
