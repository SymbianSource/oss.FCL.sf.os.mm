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
// TS_MM_ICL_FRM_CP.cpp
// This file contains the test steps ICL testing
// 
//

// EPOC includes
#include <e32base.h>
#include <caf/caf.h>

// Test system includes
#include <testframework.h>

// Specific includes for this test suite
#include "TestSuite_TSI_ICL_FRM_00.h"

// Specific includes for these test steps
#include "TestStepConversion.h"

#include "ICLFbsSessionTest.h"
// use classes in the CAF namespace directly
using namespace ContentAccess;

const TUid KImageTypeTestUid = {0x101f7bf7};
const TUid KImageSubTypeTestUid = {0x101f7bf8};


const TInt	KDefaultQualityFactor = 50;		//JPEG
const TInt	KDefaultSampling		= 3;	//JPEG

/* 
 *
 * Open and decoder image from file, then compare
 * with reference bitmap
 *
 * @param		"const TDesC& aFilename"
 *				destination file
 *				
 * @return		"TInt"
 *				Error	
 */
TInt	CTestStepConversion::OpenImageForDecodeFromFileL(const TDesC& aFilename, 
														 TUid aCodecUid)
	{

	TInt theRes = KErrNone; 

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());

	INFO_PRINTF2(_L("Openning the file : %S"), &theSourceFilename);
	
	CImageDecoder *theImageDecoder = NULL;
	if(aCodecUid.iUid == 0)
		{
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename));
		}
	else
		{
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, aCodecUid));
		}
	if(theRes != KErrNone)
		{
		iFs.Close();
		INFO_PRINTF2(_L("Cannot open file %S"), &theSourceFilename);
		return theRes;
		}
	
	if(theImageDecoder == NULL)
			return KErrUnknown;
	
	CleanupStack::PushL(theImageDecoder);
	INFO_PRINTF2(_L("The file : %S had been opened"), &theSourceFilename);

		//[ Create an active Listener and push it on the cleanup stack]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);
		
	const TFrameInfo*	theFrameInfo = &theImageDecoder -> FrameInfo();

	// >>>>>>>>>  WMF ONLY  <<<<<<<<<<<<<<<<
	// Instead of using the Frame info width and height (in pixels) we use fixed values
	// that we know are the correct values for the reference raster bitmap
	// which is used for comparison.
	// This is because the WMF is a vector image and its exact pixel size depends
	// on the screen device characteristics, i.e. the Twips to Pixel ratio.
	// In other words, different Dots Per Inch (DPI) screens will produce different
	// pixel size image from the same WMF file.

	// known target bitmap size in pixels. Size of the reference bitmap used for comparison.
	// this is the pixel frame size in wins platform, and it's different to the arm4 equivalent.
	// if iImageTypeUid == KImageTypeWMFUid and the particular test
	if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0009-CP")) == 0)
		{
		TSize fixedPixelSize(465, 454);

		User::LeaveIfError(theDestination->Create(fixedPixelSize,
							theFrameInfo->iFrameDisplayMode ));//EColor256));//
		}
	else
		{
		User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels,
							theFrameInfo->iFrameDisplayMode ));//EColor256));//
		}
		
	activeListener ->InitialiseActiveListener();
		
	theImageDecoder->Convert(& activeListener -> iStatus, *theDestination, 0);
	CActiveScheduler::Start();

	theRes = activeListener -> iStatus.Int();
	
	if(theRes == KErrNone)
		INFO_PRINTF2(_L("Conversion of file %S was successfull"), &theSourceFilename);
	else
		INFO_PRINTF2(_L("Fail during conversion the file : %S"), &theSourceFilename);

	TFileName				theReferenceFilename;
	TBuf<KLenBufferExtension>		theExtension;

	Directory(EReference, theReferenceFilename);
	TInt theSeparatorPos = aFilename.LocateReverse('.') + 1;
	theReferenceFilename.Append(aFilename.Left(theSeparatorPos));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theReferenceFilename.Append(theExtension);
	if(theRes == KErrNone)
		theRes = CheckBitmapL(*theDestination, theReferenceFilename);
	
	iFs.Close();
	
	theDestination->Reset();	  
	CleanupStack::PopAndDestroy(3,theImageDecoder);  // theDestination,CActiveListener, CImageDecoder
		
	return theRes;
	}
	
/* 
 *
 * Open and decode image from file
 *
 * @param		"const TDesC& aFilename"
 *				destination file
 *				
 * @return		"TInt"
 *				Error	
 */
TInt	CTestStepConversion::OpenPpmImageForDecodeFromFileL(const TDesC& aFilename, 
														 TUid aCodecUid,
														 const TUid aPpmDecodeUid)
	{
	TInt theRes = KErrNone; 

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());

	INFO_PRINTF2(_L("Openning the file : %S"), &theSourceFilename);
	
	CImageDecoder *theImageDecoder = NULL;
	if((aCodecUid.iUid == 0)&&(aPpmDecodeUid.iUid == 0))
		{
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename));
		}
	else
		{
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, aCodecUid, KNullUid, aPpmDecodeUid));
		}
	if(theRes != KErrNone)
		{
		iFs.Close();
		INFO_PRINTF2(_L("Cannot open file %S"), &theSourceFilename);
		return theRes;
		}
	
	if(theImageDecoder == NULL)
			return KErrUnknown;
	
	CleanupStack::PushL(theImageDecoder);
	INFO_PRINTF2(_L("The file : %S had been opened"), &theSourceFilename);

		//[ Create an active Listener and push it on the cleanup stack]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);
		
	const TFrameInfo*	theFrameInfo = &theImageDecoder -> FrameInfo();
	User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels,
							theFrameInfo->iFrameDisplayMode ));//EColor256));//
		
	activeListener ->InitialiseActiveListener();
		
	theImageDecoder->Convert(& activeListener -> iStatus, *theDestination, 0);
	CActiveScheduler::Start();

	theRes = activeListener -> iStatus.Int();
	
	if(theRes == KErrNone)
		{
		INFO_PRINTF2(_L("Conversion of file %S was successfull"), &theSourceFilename);
		}
	else
		{
		INFO_PRINTF2(_L("Fail during conversion the file : %S"), &theSourceFilename);
		}

	iFs.Close();
	
	theDestination->Reset();	  
	CleanupStack::PopAndDestroy(3,theImageDecoder);  // theDestination,CActiveListener, CImageDecoder
		
	return theRes;
	}

void CTestStepConversion::OpenImageForDecodeFromFileWithIntentL(const TDesC& aFilename, 
																	TIntent aIntent, 
																	const TDesC8& aMimeType)
	{

	TInt err = KErrNone; 

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());

	INFO_PRINTF2(_L("Openning the file : %S"), &theSourceFilename);

	CImageDecoder *theImageDecoder = NULL;

	if(aMimeType != KNullDesC8)
		{
		TRAP(err, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, aMimeType, aIntent));
		}
	else
		{
		TRAP(err, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, aIntent));
		}
	if(err != KErrNone)
		{
		iFs.Close();
		INFO_PRINTF2(_L("Cannot open file %S"), &theSourceFilename);
		User::Leave(err);
		}
	
	if(theImageDecoder == NULL)
		User::Leave(KErrUnknown);
	CleanupStack::PushL(theImageDecoder);
	INFO_PRINTF2(_L("The file : %S had been opened"), &theSourceFilename);

		//[ Create an active Listener and push it on the cleanup stack]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);
		
	const TFrameInfo*	theFrameInfo = &theImageDecoder -> FrameInfo();
	
	User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels,
						theFrameInfo->iFrameDisplayMode ));
		
	activeListener ->InitialiseActiveListener();
		
	theImageDecoder->Convert(& activeListener -> iStatus, *theDestination, 0);
	CActiveScheduler::Start();

	err = activeListener -> iStatus.Int();
	
	if(err == KErrNone)
		INFO_PRINTF2(_L("Conversion of file %S was successfull"), &theSourceFilename);
	else
		INFO_PRINTF2(_L("Fail during conversion the file : %S"), &theSourceFilename);

	TFileName theReferenceFilename;
	TBuf<KLenBufferExtension> theExtension;

	Directory(EReference, theReferenceFilename);
	TInt theSeparatorPos = aFilename.LocateReverse('.') + 1;
	theReferenceFilename.Append(aFilename.Left(theSeparatorPos));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theReferenceFilename.Append(theExtension);

	if(err == KErrNone)
		err = CheckBitmapL(*theDestination, theReferenceFilename);

	iFs.Close();
	theDestination->Reset();
	  
	CleanupStack::PopAndDestroy(3,theImageDecoder);  // theDestination,CActiveListener, CImageDecoder

	User::LeaveIfError(err);
		
	}

void CTestStepConversion::OpenImageForDecodeFromFileWithIntentL(const TDesC& aFilename, 
																	TIntent aIntent, 
																TUid aImageTypeUid,
																TUid aDecoderUid)
	{

	TInt err = KErrNone; 

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());

	INFO_PRINTF2(_L("Openning the file : %S"), &theSourceFilename);

	CImageDecoder *theImageDecoder = NULL;
	
	TRAP(err, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, aIntent, CImageDecoder::EOptionNone, aImageTypeUid, KNullUid, aDecoderUid));

	if(err != KErrNone)
		{
		iFs.Close();
		INFO_PRINTF2(_L("Cannot open file %S"), &theSourceFilename);
		User::Leave(err);
		}
	
	if(theImageDecoder == NULL)
		User::Leave(KErrUnknown);
	
	CleanupStack::PushL(theImageDecoder);
	INFO_PRINTF2(_L("The file : %S had been opened"), &theSourceFilename);

		//[ Create an active Listener and push it on the cleanup stack]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);
		
	const TFrameInfo*	theFrameInfo = &theImageDecoder -> FrameInfo();
	User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels,
						theFrameInfo->iFrameDisplayMode ));
		
	activeListener ->InitialiseActiveListener();
		
	theImageDecoder->Convert(& activeListener -> iStatus, *theDestination, 0);
	CActiveScheduler::Start();

	err = activeListener -> iStatus.Int();
	
	if(err == KErrNone)
		INFO_PRINTF2(_L("Conversion of file %S was successfull"), &theSourceFilename);
	else
		INFO_PRINTF2(_L("Fail during conversion the file : %S"), &theSourceFilename);

	TFileName theReferenceFilename;
	TBuf<KLenBufferExtension> theExtension;

	Directory(EReference, theReferenceFilename);
	TInt theSeparatorPos = aFilename.LocateReverse('.') + 1;
	theReferenceFilename.Append(aFilename.Left(theSeparatorPos));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theReferenceFilename.Append(theExtension);

	if(err == KErrNone)
		err = CheckBitmapL(*theDestination, theReferenceFilename);

	iFs.Close();
	theDestination->Reset();
		  //Active Listiner
	CleanupStack::PopAndDestroy(3,theImageDecoder);  // theDestination,CActiveListener, CImageDecoder

	User::LeaveIfError(err);
		
	}


/* 
 *
 * get size of step to increment
 *
 * @param		"TInt aValue"
 *				wanted buffer size
 *				
 * @return		"TInt"
 *				step	
 */
inline TInt CTestStepConversion::StreamIncrement(TInt aValue)
	{
	return (aValue<KStreamBreakpoint)?KStreamInitialIncrement:KStreamSecondaryIncrement;

	}

/* 
 *
 * Read and decode an image incrementally then compare with reference
 *
 * @param		"const TDesC& aFileName"
 *				"TInt aStepBeforeCancel"
 *				request cancel of load/decode from the client app before completion
 *				(only if aStepBeforeCancel > 0 )
 *
 * @return		"TInt"
 *				error	
 */
TInt CTestStepConversion::ReadAndDecodeIncrementallyL(const TDesC& aFileName, TInt aStepBeforeCancel)
	{
	TInt aFrameNo = 0;
	TInt	theRes;
	
	User::LeaveIfError(iFs.Connect());

	TFileName		sourceFileName;
	Directory(EInput, sourceFileName);
	sourceFileName.Append(aFileName);

	theRes = iFile.Open(iFs, sourceFileName, EFileShareReadersOnly|EFileStream|EFileRead);
	
	if(theRes != KErrNone)
		{
		iFs.Close();
		INFO_PRINTF2(_L("Cannot open file : %S"), &sourceFileName);

		return theRes;
		}
	else
		{
		INFO_PRINTF2(_L("File %S was opened successfully"), &sourceFileName);
		}
	
	CleanupClosePushL(iFile);
	TInt fileSize = 0;
	User::LeaveIfError(iFile.Size(fileSize));

	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(1024)), 0, 1024);

	RFile destFile;

	TFileName		destFileName;
	Directory(EOutput, destFileName);
	destFileName.Append(aFileName);
	TInt  destFileLength = 0;

	theRes = destFile.Create(iFs, destFileName, EFileShareAny|EFileStream|EFileWrite);
	//theRes = destFile.Create(iFs, destFileName, EFileShareAny);
	if(theRes == KErrAlreadyExists)
		User::LeaveIfError(theRes = destFile.Replace(iFs, destFileName, EFileShareAny|EFileStream|EFileWrite));
	else if(theRes)
		{
		INFO_PRINTF2(_L("Cannot create destination file %S"), &destFileName);
		User::LeaveIfError(theRes);
		}
	
	CleanupClosePushL(destFile);

	INFO_PRINTF2(_L("Destination file %S was created successfully"), &destFileName);

	TInt wantedBufferSize=1;

	User::LeaveIfError(iFile.Read(buffer, wantedBufferSize));
	ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
	User::LeaveIfError(destFile.Write(buffer));
	destFileLength += wantedBufferSize;
	CImageDecoder *theImageDecoder = NULL;
	// gradually increment buffer size until we can open the decoder
	for (;;)
		{
		TRAPD(error, theImageDecoder = CImageDecoder::FileNewL(iFs, destFileName, 
								CImageDecoder::EOptionNone));//, 
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			{
			INFO_PRINTF2(_L("Cannot create image decoder for file %S"), &destFileName);
			
			if(KErrAccessDenied == error)
				INFO_PRINTF1(_L("Access denied"));

			User::Leave(error);
			}
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(iFile.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(destFile.Write(buffer));
		destFileLength += extra;
		}
	CleanupStack::PushL(theImageDecoder);

	// gradually increment buffer size until we get info about the given frame
	
	while (theImageDecoder->IsImageHeaderProcessingComplete() == (TBool)EFalse && 
		wantedBufferSize<fileSize)
		{
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(iFile.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(destFile.Write(buffer));
		destFileLength += extra;

		theImageDecoder->ContinueProcessingHeaderL();
		}

	// resize the bitmap - since we now know the size
	TFrameInfo frameInfo(theImageDecoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),
			frameInfo.iFrameCoordsInPixels.Height());
	
	CFbsBitmap*	theBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( theBitmap );
	User::LeaveIfError(theBitmap->Create(frameSize, frameInfo.iFrameDisplayMode)); 

	// the conversion itself
	TBool first = ETrue;
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	if(aStepBeforeCancel > 0)
		iActiveScheduler->SetDecoder(theImageDecoder, activeListener, aStepBeforeCancel);
	for(;;)
		{
		activeListener->InitialiseActiveListener();
		if (first)
			theImageDecoder->Convert(&activeListener->iStatus,*theBitmap,aFrameNo);
		else
			theImageDecoder->ContinueConvert(&activeListener->iStatus);

		first = EFalse;

		CActiveScheduler::Start();
		TInt error = activeListener->iStatus.Int();
		if (error==KErrNone)
			break;
		
		if(error == KErrCancel)
			{
			//The incremental load was canceled at the client before completion
			theRes = KErrCancel;
			break;
			}
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			User::Leave(error);
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		
		ASSERT(extra>0);
		
		if(iFile.SubSessionHandle())
			User::LeaveIfError(iFile.Read(buffer, extra));
		
		ASSERT(buffer.Length()==extra); // should always work on emulator
		
		if(destFile.SubSessionHandle())
			{
			User::LeaveIfError(destFile.Write(buffer));
			destFileLength += extra;
			}
		}
	
	TFileName	refFileName;
	TBuf<KLenBufferExtension>		imageExtension;
	Directory(EReference, refFileName);
	refFileName.Append(aFileName.Left(aFileName.Length()-3));
	GetImageExtension(KImageTypeMBMUid, imageExtension);
	refFileName.Append(imageExtension);
	
	if(theRes == KErrNone && aStepBeforeCancel == 0)
		theRes = CheckBitmapL(*theBitmap, refFileName);
	
	theBitmap->Reset();
	CleanupStack::PopAndDestroy(6); // theImageDecoder, destFile, buffer, file, theBitmap, activeListener

	iFs.Close();
	
	return theRes;
	}


/* 
 *
 * Read and decode image from buffer
 *
 * @param		"const TDesC& aFileName"
 *				"TUid aUid"
 *				using for negative test
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::OpenImageForDecodeFromBufferL(const TDesC& aFilename, TUid aUid)
	{

	TInt theRes = KErrNone;

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());
	
	TInt fileSize = 0;

	theRes = iFile.Open(iFs, theSourceFilename, EFileRead);

	if(theRes == KErrNone)
		iFile.Size(fileSize);
	// if aFilename.Length() == 0 - we try negative test "Request data where no frame is loaded
	else if(aFilename.Length() != 0)
		{
		INFO_PRINTF2(_L("Cannot open source file %S"), &theSourceFilename);
		return theRes;
		}

	HBufC8 *theImageFromFile = HBufC8::NewMaxLC(fileSize);
	
	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	if(iFile.SubSessionHandle())
		iFile.Read(imageFromFilePtr);
	iFile.Close();
	
	CImageDecoder* theImageDecoder = NULL;
	
	if(aUid.iUid == 0)
		{
		TRAP(theRes, theImageDecoder = CImageDecoder::DataNewL(iFs, imageFromFilePtr));
		}
	else
		{//for negative testing
		TRAP(theRes, theImageDecoder = CImageDecoder::DataNewL(iFs, imageFromFilePtr, CImageDecoder::EOptionNone, KImageTypeWMFUid));
		}
	
	iFs.Close();
	
	if(theRes != KErrNone)
		{
		CleanupStack::PopAndDestroy(theImageFromFile);
		
		INFO_PRINTF1(_L("Cannot create image decoder from file's pointer"));
		return theRes;
		}
		
	if(theImageDecoder == NULL)
		{
		CleanupStack::PopAndDestroy(theImageFromFile);
		return KErrUnknown;
		}
		
	CleanupStack::PushL(theImageDecoder);	

	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);
		
	const TFrameInfo*	theFrameInfo = &theImageDecoder-> FrameInfo();
	User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels, 
							theFrameInfo->iFrameDisplayMode ));
		// Create an active Listener and push it on the cleanup stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	activeListener->InitialiseActiveListener();

	theImageDecoder->Convert(& activeListener -> iStatus, *theDestination);
	CActiveScheduler::Start();
	theRes = activeListener -> iStatus.Int();
	if(theRes == KErrNone)
		{
		INFO_PRINTF2(_L("File %S was converted successfully"), &theSourceFilename);
		}
	else
		{
		INFO_PRINTF2(_L("Fail during conversion file %S"), &theSourceFilename);
		}

	TFileName				theReferenceFilename;
	TBuf<KLenBufferExtension>		theExtension;

	Directory(EReference, theReferenceFilename);
	theReferenceFilename.Append(aFilename.Left(aFilename.Length()-3));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theReferenceFilename.Append(theExtension);

	if(theRes == KErrNone)
		theRes = CheckBitmapL(*theDestination, theReferenceFilename);
				
	theDestination->Reset();	  
	CleanupStack::PopAndDestroy(4,theImageFromFile); // CActiveListener, theDestination, theImageDecoder, theImageFromFile
	return theRes;
	}

/* 
	Read and decode a given frame from an image buffer with a specific codec (Uid) and decode options
	@param const TDesC&	aFileName	The file to be read into a data buffer (for use in CImageDecoder::DataNewL())
	@param TInt	aFrameNumber		The frame to be decoded (0 is the first frame).
	@param TUid	aUid				The Uid of the codec to be used for the decode.
	@param CImageDecoder::TOptions aOptions	Any decoder options (EOptionNone is the default).
 */
void CTestStepConversion::OpenBufferedImageL(const TDesC& aFilename, TInt aFrameNumber, TUid aUid, const CImageDecoder::TOptions aOptions)
	{
	TInt err = KErrNone;

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());
	
	TInt fileSize = 0;

	err = iFile.Open(iFs, theSourceFilename, EFileRead);

	if(err == KErrNone)
		{
		iFile.Size(fileSize);
		}
	else
		{
		INFO_PRINTF2(_L("Cannot open source file %S"), &theSourceFilename);
		User::Leave(err);
		}

	HBufC8* theImageFromFile = HBufC8::NewMaxLC(fileSize);
		
	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	if(iFile.SubSessionHandle())
		{
		iFile.Read(imageFromFilePtr);
		}
	iFile.Close();
	
	CImageDecoder* theImageDecoder = NULL;
	
	TRAP(err, theImageDecoder = CImageDecoder::DataNewL(iFs, imageFromFilePtr, aOptions, KNullUid, KNullUid, aUid));
	
	iFs.Close();
	
	if(err != KErrNone)
		{
		INFO_PRINTF1(_L("Cannot create image decoder from file's pointer"));
		User::Leave(err);
		}
	
	CleanupStack::PushL(theImageDecoder);
	
	if(aFrameNumber >= theImageDecoder->FrameCount())
		{
		INFO_PRINTF1(_L("Requested to decode unavailable image frame."));
		User::Leave(KErrArgument);
		}

	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);

	const TFrameInfo* theFrameInfo = &theImageDecoder->FrameInfo(aFrameNumber);
	User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels,
						theFrameInfo->iFrameDisplayMode));
	
	// Create an active Listener and push it on the cleanup stack
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL(activeListener);
	activeListener->InitialiseActiveListener();
	theImageDecoder->Convert(&activeListener->iStatus, *theDestination, aFrameNumber);
	
	CActiveScheduler::Start();
	
	err = activeListener->iStatus.Int();
	if(err == KErrNone)
		{
		INFO_PRINTF3(_L("Frame %d of file %S was converted successfully"), aFrameNumber, &theSourceFilename);
		}
	else
		{
		INFO_PRINTF3(_L("Fail during conversion of frame %d of file %S"), aFrameNumber, &theSourceFilename);
		User::Leave(err);
		}

	TFileName theReferenceFilename;
	TBuf<KLenBufferExtension> theExtension;

	Directory(EReference, theReferenceFilename);
	TInt theSeparatorPos = aFilename.LocateReverse('.') + 1;
	theReferenceFilename.Append(aFilename.Left(theSeparatorPos));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theReferenceFilename.Append(theExtension);

//  Uncomment to save the decoded bitmap	
//	User::LeaveIfError(theDestination->Save(theReferenceFilename));
	
	if(err == KErrNone)
		{
		TInt err1 = KErrNone;
		TRAP(err, err1 = CheckBitmapL(*theDestination, theReferenceFilename));
		if(err != KErrNone || err1 != KErrNone)
			{
			INFO_PRINTF1(_L("Bitmap check failed"));
			User::Leave(KErrGeneral);
			}
		}
	
	theDestination->Reset();	  
	CleanupStack::PopAndDestroy(4, theImageFromFile); // CActiveListener, theDestination, theImageDecoder, theImageFromFile
	}


/* 
 *
 * Read and encode image from file to memory
 *
 * @param		"const TDesC& aFileName"
 *				"TUid		aTypeImage"
 *				codec Uid 
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::EncodeImageToMemoryL(const TDesC& aFilename, 
												   TUid		aTypeImage)
	{
	TInt	theRes = KErrNone;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	TFileName	theSourceFileName;
	Directory( EReference, theSourceFileName);
	theSourceFileName.Append(aFilename);

	CFbsBitmap	*theBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theBitmap);
	theRes = theBitmap->Load(theSourceFileName);
	
	if(theRes == KErrNone)
		theRes = EncodeImageFromBitmapToMemoryL(aFilename, 
												    theBitmap,
													aTypeImage);
	else
		INFO_PRINTF2(_L("File : %S not found"), &theSourceFileName);

	CleanupStack::PopAndDestroy(2, fbsSession);

	return theRes;
	}


/* 
 *
 * Read and encode image from bitmap to memory
 *
 * @param		"const TDesC& aFileName"
 *				"CFbsBitmap	*theBitmap"
 *				"TUid		aTypeImage"
 *				codec Uid 
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::EncodeImageFromBitmapToMemoryL(const TDesC& aFilename, 
												    CFbsBitmap	*theBitmap,
													TUid		aImageTypeId)
	{

	TInt	theRes = KErrNone;
	
	TFileName	theSourceFileName;
	Directory( EReference, theSourceFileName);
	theSourceFileName.Append(aFilename);

	HBufC8 *theDestinationImage = NULL;
	CImageEncoder *theImageEncoder = NULL;
	
	// creating encoder
	TBuf<KLenBufferExtension>	theExtension;
	GetImageExtension(aImageTypeId, theExtension);
	ERR_PRINTF3(_L("Creating encoder %S, Uid = %d"), &theExtension, aImageTypeId); 
	
	TRAP(theRes, theImageEncoder = CImageEncoder::DataNewL( theDestinationImage, 
		CImageEncoder::EOptionNone, aImageTypeId ));
	if(theRes != KErrNone)
		{
		ERR_PRINTF2(_L("Cannot create image encoder buffer for Uid = %d"), 
			aImageTypeId.iUid);
		return theRes;
		
		}
	CleanupStack::PushL(theImageEncoder);
	
	ERR_PRINTF1(_L("Image buffer was created successfully")); 

	//prepare encode data
	TConvertInfo	theConvInfo;
	theConvInfo.iImageTypeId = iImageTypeUid;
	theConvInfo.iDisplayMode = theBitmap->DisplayMode();
	
	TImageDataBlock*	imageData = NULL;
	TFrameDataBlock*	frameData = NULL;
	
	CFrameImageData* theFrameImageData = CFrameImageData::NewL();
	CleanupStack::PushL(theFrameImageData);
		
	theRes	 = PrepareEncodeDataL(theConvInfo, &imageData, &frameData);
	//
	if(imageData)
		{
		CleanupStack::PushL(imageData);
		User::LeaveIfError(theFrameImageData->AppendImageData(imageData));
		CleanupStack::Pop(imageData);
		}
	if(frameData)
		{
		CleanupStack::PushL(frameData);	
		User::LeaveIfError(theFrameImageData->AppendFrameData(frameData));
		CleanupStack::Pop(frameData);
		}
	// do conversion
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );
	
	activeListener->InitialiseActiveListener();
	theImageEncoder->Convert(&activeListener->iStatus,*theBitmap, theFrameImageData);
	CActiveScheduler::Start();

	theRes = activeListener->iStatus.Int();

	if(theRes == KErrNone)
		ERR_PRINTF1(_L("Conversion was successful")); 
	else
		ERR_PRINTF1(_L("Fail during conversion")); 

	if(theRes == KErrNone)
		theRes = CompareFileL(aFilename, theDestinationImage, 
									aImageTypeId, 
									EInput); 
	//
	delete theDestinationImage;
	theDestinationImage = NULL;

	CleanupStack::PopAndDestroy(3, theImageEncoder);
	
	return theRes; 
	}


/* 
 *
 * Read and encode image from bitmap to file
 * After encoding load file into a flat buffer and do bitwise compare
 *
 * @param		"const TDesC& aFileName"
 *				"TUid		aTypeImage"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::EncodeImageFromBitmapToFileL(const TDesC& aFilename, 
												   TUid		aImageTypeId)
	{
	TFileName	theSourceFile;
	Directory(EReference, theSourceFile);
	theSourceFile.Append(aFilename);

	CFbsBitmap	*theBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theBitmap);
	TInt theRes = theBitmap->Load(theSourceFile);
	
	if(theRes != KErrNone)
		{
		CleanupStack::PopAndDestroy(theBitmap);

		ERR_PRINTF2(_L("File %S cannot be louded" ), &theSourceFile); 
		return theRes;
		}
	
	User::LeaveIfError(iFs.Connect());

	// call for a convertor
	//
	TFileName	theDestinationFileName; 
	TBuf<KLenBuffer>		theExtinsion;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(aFilename.Left(aFilename.Length()-3));

	GetImageExtension(aImageTypeId, theExtinsion);
	theDestinationFileName.Append(theExtinsion);
	
	CImageEncoder* theImageEncoder = NULL;
	TRAP(theRes, theImageEncoder = 
		CImageEncoder::FileNewL(iFs, theDestinationFileName, 
		CImageEncoder::EOptionNone, aImageTypeId));

	if(theRes != KErrNone)
		{
		CleanupStack::PopAndDestroy(theBitmap);

		iFs.Close();
		ERR_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), aImageTypeId, &theDestinationFileName); 
		if(KErrPathNotFound == theRes)
			ERR_PRINTF2(_L("Path %S not found" ), &theDestinationFileName); 
		
		if(theRes == KErrAccessDenied)
			ERR_PRINTF1(_L("Access denied" )); 

		return theRes;	
		}
	CleanupStack::PushL(theImageEncoder);
	//-----------
	//prepare encode data
	TConvertInfo	theConvInfo;
	theConvInfo.iImageTypeId = iImageTypeUid;
	theConvInfo.iDisplayMode = theBitmap->DisplayMode();
	
	//TJpegImageData::TColorSampling iSampling;
	//TInt iQualityFactor; // 0 to 100 inclusive
	TImageDataBlock*	imageData = NULL;
	TFrameDataBlock*	frameData = NULL;

	CFrameImageData* theFrameImageData = CFrameImageData::NewL();
	CleanupStack::PushL(theFrameImageData);
		
	theRes	 = PrepareEncodeDataL(theConvInfo, &imageData, &frameData);
	//
	if(imageData)
		{
		CleanupStack::PushL(imageData);
		User::LeaveIfError(theFrameImageData->AppendImageData(imageData));
		CleanupStack::Pop(imageData);
		}
	if(frameData)
		{
		CleanupStack::PushL(frameData);	
		User::LeaveIfError(theFrameImageData->AppendFrameData(frameData));
		CleanupStack::Pop(frameData);
		}
	//--------------
	// do conversion
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	iActiveScheduler->SetEncoder(theImageEncoder, activeListener, 0);
	iActiveScheduler->SetStep(this);

	activeListener->InitialiseActiveListener();
	theImageEncoder->Convert(&activeListener->iStatus, *theBitmap, theFrameImageData);
	CActiveScheduler::Start();
	

	theRes = activeListener->iStatus.Int();
	iActiveScheduler->SetStep(NULL);

	//  ! this function will be removed from API
	//TInt nSize = iImageEncoder->CurrentImageSizeL();
	//

	CleanupStack::PopAndDestroy(4, theBitmap); //active sheduler

	if(theRes != KErrNone)
		{
		ERR_PRINTF1(_L("Error during conversion" )); 
		return theRes;
		}
	else
		{
		ERR_PRINTF1(_L("Conversion was successful" )); 
		}
	//
	//compare two file into a flat buffer
	
	TInt fileSize;
	
	iFile.Open(iFs, theDestinationFileName, EFileRead);
	iFile.Size(fileSize);
	HBufC8 *theCreatedImage = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theCreatedImage->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();

	theRes = CompareFileL(aFilename, theCreatedImage, aImageTypeId, EInput); 

	CleanupStack::PopAndDestroy(theCreatedImage);
	//
	iFs.Close();

	return theRes; 
	}

 

/* 
 *
 * Prepare encode data
 *
 * @param		"TConvertInfo &aConvInfo"
 *				input information
 *				"TImageDataBlock**	imageData"
 *				"TFrameDataBlock**	frameData"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::PrepareEncodeDataL(TConvertInfo &aConvInfo, 
											   TImageDataBlock** aImageData, 
											   TFrameDataBlock** aFrameData)
	{
	TBuf<KLenBuffer>	nameSection;
	TInt theRes = KErrNone;
	TBool theColor = ETrue;
	if(aConvInfo.iDisplayMode == EGray2)
		theColor = EFalse;
	
	if(aConvInfo.iImageTypeId == KImageTypeMBMUid)
		{
		*aFrameData = new (ELeave) TMbmEncodeData;
		TMbmEncodeData*	data = STATIC_CAST(TMbmEncodeData*, *aFrameData);
		TInt	displaymode;
		data->iDisplayMode = aConvInfo.iDisplayMode;
			//
		if(GetIntFromConfig(nameSection, _L("displaymode"), displaymode) == (TBool)ETrue)
			{
				switch(displaymode)
					{
					case 1: data->iDisplayMode = EGray2; break;
					case 2: data->iDisplayMode = EGray4; break;
					case 3: data->iDisplayMode = EGray16;break;
					case 4: data->iDisplayMode = EGray256;break;
					case 5: data->iDisplayMode = EColor16;break;
					case 6: data->iDisplayMode = EColor256;break;
					case 7: data->iDisplayMode = EColor64K;break;
					case 8: data->iDisplayMode = EColor16M;break;
					default: break;
					}
			}
		}
	else if(aConvInfo.iImageTypeId == KImageTypeGIFUid)
		{
		//the encoder is not configurable	
		}
	else if(aConvInfo.iImageTypeId == KImageTypeBMPUid)
		{
		*aImageData= new(ELeave) TBmpImageData;
		TBmpImageData*	data = STATIC_CAST(TBmpImageData*, *aImageData);

		TPtrC ptr = iTestStepName;
		nameSection = ptr.Right(KLenNameInSection);
		TInt nBitsPerPixel = 0;
		if(GetIntFromConfig(nameSection, _L("BitsPerPixel"), nBitsPerPixel) == EFalse)
			{
		
			switch(aConvInfo.iDisplayMode)
				{
				case EGray2: data->iBitsPerPixel = 1; break;
				case EGray4: data->iBitsPerPixel = 2; break;
				case EGray16: data->iBitsPerPixel = 4; break;
				case EGray256: data->iBitsPerPixel = 8;break;
				case EColor16: data->iBitsPerPixel = 4; break;
				case EColor256: data->iBitsPerPixel = 8; break;
				case EColor64K: data->iBitsPerPixel = 16;break;
				case EColor16M: data-> iBitsPerPixel = 24;break;
				default: data->iBitsPerPixel = 0; break;
				}
			}
			else 
				data->iBitsPerPixel = nBitsPerPixel;
		}
	else if(aConvInfo.iImageTypeId == KImageTypeJPGUid)
		{
			// read config file
			TInt		theQualityFactor = KDefaultQualityFactor;
			TInt		theSampling = KDefaultSampling;

			TPtrC ptr = iTestStepName;
			nameSection = ptr.Right(KLenNameInSection);
			GetIntFromConfig(nameSection, _L("QualityFactor"), theQualityFactor);
			GetIntFromConfig(nameSection, _L("Sampling"), theSampling);
			//

			*aImageData = new (ELeave) TJpegImageData;
			TJpegImageData* data = STATIC_CAST(TJpegImageData*, *aImageData);

			if(!theColor)
				data->iSampleScheme = TJpegImageData::EMonochrome;
			else 
				data->iSampleScheme=TJpegImageData::TColorSampling(theSampling);
			data->iQualityFactor=theQualityFactor; 
		}

	return theRes;
	}
/* 
 *
 * List all available decoders MIME types
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::ListAvailableMIMEDecoders()
	{
	TInt	theRes;
	RFileExtensionMIMETypeArray		theFileExtensionArray;
	
	TRAP(theRes, CImageDecoder::GetFileTypesL(theFileExtensionArray));	
	
	if(theRes != KErrNone)
		{
		theFileExtensionArray.ResetAndDestroy();
		return theRes;
		}

	
	INFO_PRINTF1(_L("A List of decoder file extensions and MIME types"));
	
	for (TInt index=0; index<theFileExtensionArray.Count(); index++)
		{
		CFileExtensionMIMEType& fileExtAndMIMEType = *theFileExtensionArray[index];
		
		const TDesC& extension = fileExtAndMIMEType.FileExtension();
		const TDesC8& mimeType = fileExtAndMIMEType.MIMEType();
		TBuf<KLenBuffer>	mimeType16;

		mimeType16.Copy(mimeType);
		
		INFO_PRINTF3(_L("Extension : %S, MIME Type : %S"), &extension, &mimeType16);
		
		}
	
	theFileExtensionArray.ResetAndDestroy();
	
	return theRes;
	}


/* 
 *
 * List all available encoders MIME types
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::ListAvailableMIMEEncoders()
	{
	TInt	theRes = KErrNone;
	
	RFileExtensionMIMETypeArray		theFileExtensionArray;

	TRAP(theRes, CImageEncoder::GetFileTypesL(theFileExtensionArray));	
	
	if(theRes != KErrNone)
		{
		theFileExtensionArray.ResetAndDestroy();
		return theRes;
		}
	INFO_PRINTF1(_L("\n"));
	INFO_PRINTF1(_L("A List of encoder file extensions and MIME types"));
	
	for (TInt index=0; index<theFileExtensionArray.Count(); index++)
		{
		CFileExtensionMIMEType& fileExtAndMIMEType = *theFileExtensionArray[index];
		
		const TDesC& extension = fileExtAndMIMEType.FileExtension();
		const TDesC8& mimeType = fileExtAndMIMEType.MIMEType();
		TBuf<KLenBuffer> mimeType16;

		mimeType16.Copy(mimeType);
		
		INFO_PRINTF3(_L("Extension: %S, MIME Type : %S") , &extension, &mimeType16);
		
		}
	theFileExtensionArray.ResetAndDestroy();
	
	return theRes; 
	}

/* 
 *
 * List all available decoders 
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::ListAllAvailableDecoders()
	{
	TInt theRes = KErrNone;


	// find out UID and then name of the codec
	RImageTypeDescriptionArray imageDecTypeArray;
	TRAP(theRes, CImageDecoder::GetImageTypesL(imageDecTypeArray));
	// find a description with the same image type and, if subtype given, subtype too
	
	if(theRes != KErrNone)
		{
		return theRes;
		}
	
	TInt index;
	
	if(imageDecTypeArray.Count() > 0)
		INFO_PRINTF1(_L("A List of available Decoder plugins!"));

	for (index=0; index<imageDecTypeArray.Count(); index++)
		{
		CImageTypeDescription& description = *imageDecTypeArray[index];
		
		const TDesC& theBufDes = description.Description();
 
		INFO_PRINTF1(theBufDes);
		//
		TUid theUid = description.ImageType();
		RImageTypeDescriptionArray	theSubTypeArray;
		
		TInt theRes1;
		TRAP(theRes1, CImageDecoder::GetImageSubTypesL(theUid, theSubTypeArray));
		
		if(theRes1 == KErrNone)
			{
			if(theSubTypeArray.Count() > 0)
				INFO_PRINTF1(_L("A List of available Sub Types:"));
			TInt index1;
			for (index1=0; index1<theSubTypeArray.Count(); index1++)
				{
				CImageTypeDescription& description1 = *theSubTypeArray[index1];
				const TDesC& theBufDes1 = description1.Description();
 
				INFO_PRINTF2(_L("   %S"), &theBufDes1);
				
				}	
			theSubTypeArray.ResetAndDestroy();
			}
		//
		INFO_PRINTF1(_L("\n"));
		}
	imageDecTypeArray.ResetAndDestroy();

	return theRes; //
	}

/* 
 *
 * List all available encoders 
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::ListAllAvailableEncoders()
	{
	TInt theRes = KErrNone;

	// find out UID and then name of the codec
	RImageTypeDescriptionArray imageEncTypeArray;
	
	TRAP(theRes, CImageEncoder::GetImageTypesL(imageEncTypeArray));

	if(theRes != KErrNone)
		return theRes;
	// find a description with the same image type and, if subtype given, subtype too
	TInt index;
	
	if(imageEncTypeArray.Count() > 0)
		INFO_PRINTF1(_L("A List of available Encoder plugins!"));

	for (index=0; index<imageEncTypeArray.Count(); index++)
		{
		CImageTypeDescription& description = *imageEncTypeArray[index];
		
		const TDesC&	theBufDes = description.Description();
		
		INFO_PRINTF1(theBufDes);

		//
		TUid theUid = description.ImageType();
		RImageTypeDescriptionArray	theSubTypeArray;
		
		TInt theRes1;
		TRAP(theRes1, CImageEncoder::GetImageSubTypesL(theUid, theSubTypeArray));
		
		if(theRes1 == KErrNone)
			{
			if(theSubTypeArray.Count() > 0)
				INFO_PRINTF1(_L("A List of available Sub Types:"));
			TInt index1;
			for (index1=0; index1<theSubTypeArray.Count(); index1++)
				{
				CImageTypeDescription& description1 = *theSubTypeArray[index1];
				const TDesC& theBufDes1 = description1.Description();
 
				INFO_PRINTF2(_L("   %S"), &theBufDes1);
				
				}	
			theSubTypeArray.ResetAndDestroy();
			}
		//

		}
	imageEncTypeArray.ResetAndDestroy();

	return theRes; 
	}

/* 
 *
 * Enquire MIME type of an unopened image  
 *
 * @param		"const TDesC& aFileName"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::FindMimeTypeL(const TDesC& aFileName)
	{
	TInt theRes = KErrNone;

	TBuf8<KLenBuffer>	theMimeType;

	INFO_PRINTF1(_L("Enquire MIME type of an unopened images"));
	
	User::LeaveIfError(iFs.Connect());

	TFileName	theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFileName);

	CImageDecoder::GetMimeTypeFileL(iFs, theSourceFilename, theMimeType);
	TBuf<KLenBuffer>	mimeType;
	mimeType.Copy(theMimeType);
	INFO_PRINTF3(_L("File : %S, MimeType %S"), &theSourceFilename, &mimeType);
	//-------------
	TInt fileSize;

	iFile.Open(iFs, theSourceFilename, EFileRead);
	iFile.Size(fileSize);
	HBufC8 *theImageFromFile = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();

	iFs.Close();

	TBuf8<KLenBuffer>	theMimeType1;

	CImageDecoder::GetMimeTypeDataL(imageFromFilePtr, theMimeType1);
	
	mimeType.Copy(theMimeType1);
	INFO_PRINTF3(_L("Reading from buffer, File : %S, MimeType %S\n"), 
		&theSourceFilename, &mimeType);
	
	CleanupStack::PopAndDestroy();

	return theRes; 
	}


/* 
 *
 * Enquire UID of a loaded decoder & Implementation Info
 *
 * @param		"const TDesC& aFileName"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::EnquireDecoderUIDAndImplementationInfoL(const TDesC& aFileName)
	{
	TInt theRes = KErrNone;

	User::LeaveIfError(iFs.Connect());

	TFileName	theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFileName);

	CImageDecoder *theImageDecoder = CImageDecoder::FileNewL(iFs, theSourceFilename);
	if(theImageDecoder == NULL)
			return KErrUnknown;

	TUid uid = theImageDecoder->ImplementationUid();
	INFO_PRINTF3(_L("UID of a loaded decoder during reading file %S = %x\n"), 
		&theSourceFilename, uid);

	delete theImageDecoder;
	theImageDecoder = NULL;
	//------------------------
	TInt fileSize;

	iFile.Open(iFs, theSourceFilename, EFileRead);
	iFile.Size(fileSize);
	HBufC8 *theImageFromFile = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();
	iFs.Close();

	
	theImageDecoder = CImageDecoder::DataNewL(iFs, imageFromFilePtr);
	if(theImageDecoder == NULL)
			return KErrUnknown;
	CleanupStack::PushL(theImageDecoder);
	TUid uid1 = theImageDecoder->ImplementationUid();
	
	INFO_PRINTF2(_L("UID of a loaded decoder from buffer = 0x%08x\n"), 
		uid1);
//------
	//Request ImplementationInformation
	CImplementationInformationType* theImplementationInfo = 
		CImageDecoder::GetImplementationInformationL(uid1);
	CleanupStack::PushL(theImplementationInfo);
	TInt theVersion = theImplementationInfo->Version();
	const TDesC& dispName = theImplementationInfo->DisplayName();
	const TDesC8&  dataType8 = theImplementationInfo->DataType();
	const TDesC8&  opaqueData8 = theImplementationInfo->OpaqueData();
	
	TBuf<KLenBuffer>	dataType;
	TBuf<KLenBuffer>	opaqueData;

	dataType.Copy(dataType8);
	opaqueData.Copy(opaqueData8);
	//------------------
	TUid	theUid;
	TUid	theUidSubType;

	theImageDecoder->ImageType(0, theUid, theUidSubType);

	TInt versionNo = 0;
	TBuf<KLenBuffer>	bufDisplayName;
	const TUint8*		defaultDataPtr = NULL;		
	const TUint8*		opaqueDataPtr = NULL;
	TInt		lenDefaultData = 0;
	TInt		lenOpaqueData = 0;

	TInt versionNoJPEG = 1;
	const TUint8 defaultDataJPEG[] = {0xFF, 0xD8};
	static const TUint8 opaqueDataJPEG[] = {0, 0x10, 0x1F, 0x45, 0xD8, 0, 0, 0, 0, 1, 0x2E, 0x6A, 0x70, 0x67, 0x0D, 1, 0x69, 0x6D, 0x61, 0x67, 0x65, 0x2F, 0x6A, 0x70, 0x65, 0x67, 0x0D};
	
	TInt versionNoGIF = 1;
	const TUint8 defaultDataGIF[] = {0x47, 0x49, 0x46, 0x38, 0x3F, 0x61};
	static const TUint8 opaqueDataGIF[] = {0, 0x10, 0x1F, 0x45, 0xB3, 0, 0, 0, 0, 1, 0x2E, 0x67, 0x69, 0x66, 0x0D, 1, 0x69, 0x6D, 0x61, 0x67, 0x65, 0x2F, 0x67, 0x69, 0x66, 0x0D};

	TInt versionNoBMP = 1;
	const TUint8 defaultDataBMP[] = {0x42, 0x4D};
	static const TUint8 opaqueDataBMP[] = {0, 0x10, 0x1F, 0x45, 0xB0, 0, 0, 0, 0, 1, 0x2E, 0x62, 0x6D, 0x70, 0x0D, 2, 0x69, 0x6D, 0x61, 0x67, 0x65, 0x2F, 0x78, 0x2D, 0x62, 0x6D, 0x70, 0x0D, 0x69, 0x6D, 0x61, 0x67, 0x65, 0x2F, 0x62, 0x6D, 0x70, 0x0D};

	if(theUid == KImageTypeJPGUid)
		{
			versionNo = versionNoJPEG;
			bufDisplayName = _L("JPEG");
			defaultDataPtr = defaultDataJPEG;
			opaqueDataPtr  = opaqueDataJPEG;	
			lenDefaultData = sizeof(defaultDataJPEG) / sizeof(defaultDataJPEG[0]);
			lenOpaqueData = sizeof(opaqueDataJPEG) / sizeof(opaqueDataJPEG[0]);
		}
	else if(theUid == KImageTypeGIFUid)
		{
			versionNo = versionNoGIF;
			bufDisplayName = _L("GIF");
			defaultDataPtr = defaultDataGIF;
			opaqueDataPtr  = opaqueDataGIF;	
			lenDefaultData = sizeof(defaultDataGIF) / sizeof(defaultDataGIF[0]);
			lenOpaqueData = sizeof(opaqueDataGIF) / sizeof(opaqueDataGIF[0]);
		}
	else if(theUid == KImageTypeBMPUid)
		{
			versionNo = versionNoBMP;
			bufDisplayName = _L("BMP");
			defaultDataPtr = defaultDataBMP;
			opaqueDataPtr  = opaqueDataBMP;	
			lenDefaultData = sizeof(defaultDataBMP) / sizeof(defaultDataBMP[0]);
			lenOpaqueData = sizeof(opaqueDataBMP) / sizeof(opaqueDataBMP[0]);
		}
	
	if(versionNo != theVersion)
		theRes = KErrNotIdentical;

	if(theRes == KErrNone)
		theRes = Mem::Compare(defaultDataPtr, lenDefaultData, 
			dataType8.Ptr(), dataType8.Length());



	if(theRes == KErrNone)
		{
		theRes = Mem::Compare(opaqueDataPtr, lenOpaqueData, 
			opaqueData8.Ptr(), opaqueData8.Length());
		if (theUid == KImageTypeJPGUid && theRes!=KErrNone)
			{
			INFO_PRINTF1(_L("Known issue: EXIF plug-in has got different data, ignoring diffs."));
			theRes=KErrNone;
			}			
		}
	//------------
	INFO_PRINTF1(_L("Implementation information"));
	INFO_PRINTF2(_L("Display name : %S"), &dispName);
	INFO_PRINTF2(_L("Version : %d"), theVersion);
	//INFO_PRINTF2(_L("Data type : %S"), &dataType);
	//INFO_PRINTF2(_L("Opaque data : %S"), &opaqueData);

//-------
	CleanupStack::PopAndDestroy(3, theImageFromFile); //theImageDecoder, theImageFromFile, theImplementationInfo
	
	return theRes; //
	}

/* 
 *
 * Enquire UID of a loaded encoder 
 *
 * @param		"const TDesC& aFileName"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::EnquireEncoderUIDL(const TDesC& aFileName, 
											   TUid		aTypeImageUid)
	{
	TInt theRes = KErrNotSupported;


	User::LeaveIfError(iFs.Connect());

	TFileName	theDestinationFilename;
	Directory(EOutput, theDestinationFilename);
	theDestinationFilename.Append(aFileName);
	
	CImageEncoder *theImageEncoder = NULL;
	TRAP(theRes, theImageEncoder = CImageEncoder::FileNewL(iFs, theDestinationFilename, 
		CImageEncoder::EOptionNone, aTypeImageUid));
	if(theRes != KErrNone)
		{
		iFs.Close();
		INFO_PRINTF2(_L("Cannot create file with Uid  = %d\n"), 
				aTypeImageUid);
		
		if(KErrPathNotFound == theRes)
			INFO_PRINTF2(_L("Path %S not found"), &theDestinationFilename);
		return theRes;
		}
	CleanupStack::PushL(theImageEncoder);	
	TUid uid1 = theImageEncoder->ImplementationUid();
	INFO_PRINTF3(_L("UID of a loaded decoder during openning file %S = %d\n"), 
		&theDestinationFilename, uid1);

	CleanupStack::PopAndDestroy(theImageEncoder);

	return theRes; 
	}

/* 
 *
 * Request number frames in image 
 *
 * @param		"const TDesC& aFileName"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::RequestNumberFramesL(const TDesC& aFileName)
	{
	TInt	theRes 	= KErrNone;

	User::LeaveIfError(iFs.Connect());

	TFileName	theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFileName);

	CImageDecoder *theImageDecoder = CImageDecoder::FileNewL(iFs, theSourceFilename);
	CleanupStack::PushL(theImageDecoder);
	if(theImageDecoder == NULL)
			return KErrUnknown;

	TInt	theFrameCount = theImageDecoder->FrameCount();

	CleanupStack::PopAndDestroy(theImageDecoder);

	iFs.Close();

	INFO_PRINTF3(_L("Number of frames from a file %S = %d\n"), 
		&theSourceFilename, theFrameCount);

	return theRes;
	}


/* 
 *
 * Print frame information 
 *
 * @param		"const TFrameInfo& aFrameInfo"
 *
 */
TAny	CTestStepConversion::PrintFrameInfo(const TFrameInfo& aFrameInfo)
	{
	TBuf<KLenBuffer>	theBuf;
	//TRect iFrameCoordsInPixels;
	//TSize iFrameSizeInTwips;
	//TInt iBitsPerPixel;
	//TTimeIntervalMicroSeconds iDelay;
	//TUint32 iFlags;
	//TSize iOverallSizeInPixels;
	//TDisplayMode iFrameDisplayMode;
	//TRgb iBackgroundColor;

	INFO_PRINTF5(_L("Frame's coordinates (left, top, right, bottom)  = %d, %d, %d, %d\n"), 
			aFrameInfo.iFrameCoordsInPixels.iTl.iX, aFrameInfo.iFrameCoordsInPixels.iTl.iY,
			aFrameInfo.iFrameCoordsInPixels.iBr.iX, aFrameInfo.iFrameCoordsInPixels.iBr.iY);

	INFO_PRINTF3(_L("Frame's size in twips  = %d, %d\n"), 
			aFrameInfo.iFrameSizeInTwips.iWidth, aFrameInfo.iFrameSizeInTwips.iHeight);

	INFO_PRINTF2(_L("Bits per pixel  = %d\n"), 
				aFrameInfo.iBitsPerPixel);

	TInt64	int64 = aFrameInfo.iDelay.Int64();
	TInt	delay = I64INT(int64);

	INFO_PRINTF2(_L("Delay in microseconds = %d\n"), 
				delay);
	
	INFO_PRINTF3(_L("Overall size in pixel = %d, %d\n"),  
		aFrameInfo.iOverallSizeInPixels.iWidth, aFrameInfo.iOverallSizeInPixels.iHeight);
	
	FrameDisplayMode(aFrameInfo.iFrameDisplayMode, theBuf);
		INFO_PRINTF2(_L("Frame display mode  = %S\n"), 
				&theBuf);

	INFO_PRINTF4(_L("Background color Red, Green, Blue = %d, %d, %d \n"), 
				aFrameInfo.iBackgroundColor.Red(), 
				aFrameInfo.iBackgroundColor.Green(),
				aFrameInfo.iBackgroundColor.Blue());

	}

/* 
 *
 * Request decode information & options for frames in a loaded image
 *	and print it
 *
 * @param		"const TDesC& aFileName"
 *				"TInt	aParam"
 *				KShowFrameInformation - show Frame Information
 *				KShowFrameOption - show frame option 
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::RequestDecodeDataL(const TDesC& aFileName, TInt aParam)
	{
	TInt	theErr = KErrNone;

	User::LeaveIfError(iFs.Connect());

	TFileName	theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFileName);
	
	CImageDecoder *theImageDecoder = NULL;
	TRAP(theErr, theImageDecoder = CImageDecoder::FileNewL(iFs, theSourceFilename));
	CleanupStack::PushL(theImageDecoder);
	if(theErr != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open decoder for file %S\n"), &theSourceFilename);
		

		return theErr;
		}
	INFO_PRINTF2(_L("The file %S had been loaded successfully\n"), &theSourceFilename);

	//frames loop
	TInt	theFrameCount = theImageDecoder->FrameCount();
	for(TInt theCurFrame = 0; theCurFrame < theFrameCount;theCurFrame++ )
		{
		const TFrameInfo& theFrameInfo = theImageDecoder->FrameInfo(theCurFrame);
			
		INFO_PRINTF1(_L("------------\n"));
		INFO_PRINTF2(_L("The frame number %d\n"), 
				theCurFrame);
		if(aParam & KShowFrameInformation)
			{	
			PrintFrameInfo(theFrameInfo);

		//---------------------
			const CFrameImageData&	theFrameData = theImageDecoder->FrameData(theCurFrame);
			TInt theImageDataCount = theFrameData.ImageDataCount();
			TInt theFrameCount = theFrameData.FrameDataCount();
			INFO_PRINTF2(_L("Number frame data entries %d"), theFrameCount);
			TInt	index;
			for(index = 0; index < theFrameCount; index++)
				{
				const TFrameDataBlock* theFrameDataBlock = theFrameData.GetFrameData(index);
				INFO_PRINTF3(_L("Frame Data Block # %d, UId %d"),
					index, theFrameDataBlock->DataType());

				}

			INFO_PRINTF2(_L("Number image data entries %d"), theImageDataCount);
			for(index = 0; index < theImageDataCount; index++)
				{
				const TImageDataBlock* theImageDataBlock = theFrameData.GetImageData(index);
				INFO_PRINTF3(_L("Image Data Block # %d, UId %d"),
					index, theImageDataBlock->DataType());

				}
			}
		//--------------

		if(aParam & KShowFrameOption)
			PrintFrameOptions(theFrameInfo);
		}
	  
	CleanupStack::PopAndDestroy(theImageDecoder);
	iFs.Close();

	return theErr;
	}


/* 
 *
 * Print frame options 
 *
 * @param		"const TFrameInfo& aFrameInfo"
 *
 */
TAny CTestStepConversion::PrintFrameOptions(const TFrameInfo& aFrameInfo)
	{
	_LIT(theBufYes, "Yes");
	_LIT(theBufNo, "No");
	
	// iFlags;
			
			//EColor					= 0x00000001 Grayscale if not set
			//ETransparencyPossible	= 0x00000002 Fully opaque if not set
			//EFullyScaleable			= 0x00000004 Will only scale to 1/2,1/4 & 1/8th if not set
			//EConstantAspectRatio	= 0x00000008 Scaling need not maintain aspect ratio if not set
			//ECanDither				= 0x00000010 Will not use error diffusion if not set
			//EAlphaChannel			= 0x00000020 Set if the image contains alpha-blending information
			//ELeaveInPlace			= 0x00000040
			//ERestoreToBackground	= 0x00000080 Mutually exclusive image disposal methods
			//ERestoreToPrevious		= 0x00000100
			//EPartialDecodeInvalid   = 0x00000200   Where CImageDecoder::Convert fails on KErrUnderflow, image not suitable for display

	if(TFrameInfo::EColor & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Color = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Color = %S \n"),
								&theBufNo);
				
	if(TFrameInfo::ETransparencyPossible & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Transparency = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Transparency = %S \n"),
								&theBufNo);

	if(TFrameInfo::EFullyScaleable & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Fully scaleable = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Fully scaleable = %S \n"),
								&theBufNo);

	if(TFrameInfo::EConstantAspectRatio & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Aspect ratio = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Aspect ratio = %S \n"),
								&theBufNo);

	if(TFrameInfo::ECanDither & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Can dither = %S \n"),
						&theBufYes);
	else
		INFO_PRINTF2(_L("Can dither = %S \n"),
						&theBufNo);

	if(TFrameInfo::EAlphaChannel & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Alpha channel = %S \n"),
						&theBufYes);
	else
		INFO_PRINTF2(_L("Alpha channel = %S \n"),
						&theBufNo);

	if(TFrameInfo::ELeaveInPlace & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Leave in place = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Leave in place = %S \n"),
						&theBufNo);

	if(TFrameInfo::ERestoreToBackground & aFrameInfo.iFlags)
			INFO_PRINTF2(_L("Restore to background = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Restore to background = %S \n"),
							&theBufNo);

	if(TFrameInfo::ERestoreToPrevious & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Restore to previos = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Restore to previos = %S \n"),
								&theBufNo);

	if(TFrameInfo::EPartialDecodeInvalid & aFrameInfo.iFlags)
		INFO_PRINTF2(_L("Partial decode invalid = %S \n"),
								&theBufYes);
	else
		INFO_PRINTF2(_L("Partial decode invalid = %S \n"),
								&theBufNo);

	}

/* 
 *
 *	Open image and retrieve embedded image comments
 *
 * @param		"TUid aImageTypeId"
 *				Uid encoder
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::RetrieveEmbeddedCommentsL(const TDesC& aFilename)
	{

	TInt theRes = KErrNone; 
		
	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());

		//
	CImageDecoder *theImageDecoder = NULL;	
	TRAP(theRes, (theImageDecoder = CImageDecoder::FileNewL(iFs, theSourceFilename)));//,

	if(theRes != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open file %S"),
						&theSourceFilename);
		return theRes;
		
		}
	if(theImageDecoder == NULL)
			return KErrUnknown;
	CleanupStack::PushL(theImageDecoder);
	TInt	theNumComment=theImageDecoder->NumberOfImageComments();
	
	INFO_PRINTF3(_L("Number of image comments in the file %S  = %d\n"),
						&theSourceFilename,
						theNumComment);		
	for(TInt theCurComment = 0; theCurComment < theNumComment ;theCurComment++)
		{
		HBufC* commentBuf = theImageDecoder->ImageCommentL(theCurComment);
		if(commentBuf)
			{

			TPtr imageCommentPtr = commentBuf->Des();

			INFO_PRINTF3(_L("Frame %d, comment - %S \n"),
						theCurComment,		
						&imageCommentPtr);
					
			delete commentBuf;
			}
		}
	
	CleanupStack::PopAndDestroy(theImageDecoder);
	
	iFs.Close();
		
	return theRes;
	}

/* 
 *
 *	Open image and retrieve frame info
 *
 * @param		"const TDesC& aFilename"
 *
 * @return		"TInt"
 *				error	
 */
TInt	CTestStepConversion::RetrieveFrameInfoL(const TDesC& aFilename)
	{
	TInt theRes = KErrNone; 

	TFileName theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFilename);

	User::LeaveIfError(iFs.Connect());
		//
	CImageDecoder* theImageDecoder = NULL;
	TRAP(theRes, (theImageDecoder = CImageDecoder::FileNewL(iFs, theSourceFilename, 
		CImageDecoder::EOptionNone)));
	CleanupStack::PushL(theImageDecoder);
	if(theRes != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open file %S"),
						&theSourceFilename);
		return theRes;
		
		}
	if(theImageDecoder == NULL)
			return KErrUnknown;

	TInt theNumOfFrameComments = theImageDecoder->NumberOfFrameComments(0);
	INFO_PRINTF2(_L("Number of frame comments : %d"), theNumOfFrameComments);

// GIF doesn't support frame comments
//	HBufC*  theBuf = iImageDecoder->FrameCommentL(0, 0);
//	TPtr	theBufPtr = theBuf->Des();
//	INFO_PRINTF2(_L("Frame comments : %S\n"), &theBufPtr);
//
//	delete theBuf;
//	theBuf = NULL;

	TInt imageComments = theImageDecoder->NumberOfImageComments();
	INFO_PRINTF2(_L("Number of image comments : %d"), imageComments);

	HBufC*  theBuf = theImageDecoder->ImageCommentL(0);
	TPtr	theBufPtr = theBuf->Des();
	INFO_PRINTF2(_L("Image comments : %S\n"), &theBufPtr);

	delete theBuf;
	theBuf = NULL;
	

	INFO_PRINTF1(_L("Call function FrameInfoStringsL\n"));
	CFrameInfoStrings*	theStr = theImageDecoder->FrameInfoStringsL(0);

	TInt index;
	for(index = 0; index < theStr->Count(); index++)
		{
			const TPtrC str = theStr->String(index);
			INFO_PRINTF3(_L("String # %d : %S"),
						index, &str);
		}
	delete 	theStr;
	theStr = NULL;
	
	INFO_PRINTF1(_L("Call function FrameInfoStringsLC"));
	CFrameInfoStrings*	theStr1 = theImageDecoder->FrameInfoStringsLC(0);
	for(index = 0; index < theStr1->Count(); index++)
		{
			const TPtrC str = theStr1->String(index);
			INFO_PRINTF3(_L("String # %d : %S"),
						index, &str);
		}
	
	CleanupStack::Pop();
	delete 	theStr1;
	
	TInt theFrameNumber = 0;
	TUid theImageType;
	TUid theImageSubType;

	theImageDecoder->ImageType(theFrameNumber, theImageType, theImageSubType);
	INFO_PRINTF5(_L("Image type Uid: %d (expected %d), Image sub type Uid %d (expected %d)"),
						theImageType, KImageTypeTestUid, theImageSubType, 
						KImageSubTypeTestUid);
	
	CleanupStack::PopAndDestroy(theImageDecoder);
	iFs.Close();

	return theRes;
	}

// dummy Hal call for heap balance on target
// Need to do this BEFORE doing any allocation testing !
TInt CTestStepConversion::DummyHalCall()
	{
    TInt err1 = FbsStartup();
    if (err1 != KErrNone)
        {
        INFO_PRINTF2(_L("FbsStartup failed, err = %d"), err1);
        return EInconclusive;
        }

    err1 = RFbsSession::Connect();
    if (err1 != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err1);
        return EInconclusive;
        }
		
	TInt displayMode = EColor256;
	CFbsScreenDevice* screenDevice = NULL;	
	TInt err;
	while(displayMode < EColorLast) 
		{
		TRAP(err, screenDevice = CFbsScreenDevice::NewL(_L("NotUsed"),(TDisplayMode)displayMode));
		delete screenDevice;
		screenDevice = NULL;	
		if(err == KErrNone)
			{
			break;
			}
		displayMode++;
		}			
	
	RFbsSession::Disconnect();
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Unable to create CFbsScreenDevice, err = %d"), err);
		}
	return err;
  	}

//MS3.4 File Handles//
/* 
 *
 * Decode a Bitmap, supplied a Decoder Object
 * After encoding load file into a flat buffer and do bitwise compare
 *
 * @param		"CImageDecoder& aImageDecoder"
 *				"TDesC& aFileName"
 *
 * @return		"TInt"
 *				error	
 */
TInt CTestStepConversion::DecodeUsingDecoderL(const TDesC& aFileName)
	{
	TInt theRes = KErrNone;

	if(iImageDecoder == NULL)
		{
		return KErrUnknown;
		}

	//[ Create an active Listener and push it on the cleanup stack ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	//[ Create a destination Bitmap ]
	CFbsBitmap* theDestination = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theDestination);
	
	const TFrameInfo* theFrameInfo = &iImageDecoder->FrameInfo();
	User::LeaveIfError(theDestination->Create(theFrameInfo->iOverallSizeInPixels, theFrameInfo->iFrameDisplayMode));//EColor256));// 
	
	//[ Convert Method of the Decoder ]
	activeListener->InitialiseActiveListener();
	iImageDecoder->Convert(&activeListener->iStatus, *theDestination, 0);
		
	CActiveScheduler::Start();
	theRes = activeListener->iStatus.Int();

	if(theRes == KErrNone)
		{
		INFO_PRINTF2(_L("Decoding of file %S was successful"), &aFileName);
		}
	else
		{
		INFO_PRINTF2(_L("Failed to Decode the file : %S"), &aFileName);
		}

	//[ Checking with Reference Bitmaps at the end ]
	TFileName theReferenceFilename;
	TBuf<KLenBufferExtension> theExtension;

	Directory(EReference, theReferenceFilename);
	TInt theSeparatorPos = aFileName.LocateReverse('.') + 1;
	theReferenceFilename.Append(aFileName.Left(theSeparatorPos));
	GetImageExtension(KImageTypeMBMUid, theExtension);
	theReferenceFilename.Append(theExtension);
	
	if(theRes == KErrNone)
		{
		theRes = CheckBitmapL(*theDestination, theReferenceFilename);
		}
	theDestination->Reset();	
	CleanupStack::PopAndDestroy(2,activeListener);  // theDestination,CActiveListener
	
	return theRes;
	}

/* 
 *
 * Read and encode image from bitmap to file
 * After encoding load file into a flat buffer and do bitwise compare
 *
 * @param		"const TDesC& aFileName"
 *				"TUid		aTypeImage"
 *
 */
void CTestStepConversion::EncodeUsingEncoderL(const TDesC& aFileNameSrc)
	{
	TInt theRes = KErrNone; 

	if(iImageEncoder == NULL)
		{
		User::Leave(KErrUnknown);
		}

	//[ Open the Source Bitmap ]
	TFileName theSourceFile;
	Directory(EReference, theSourceFile);
	theSourceFile.Append(aFileNameSrc);

	CFbsBitmap *theBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(theBitmap);
	
	theRes = theBitmap->Load(theSourceFile);
	
	if(theRes != KErrNone)
		{
		ERR_PRINTF2(_L("File %S cannot be loaded." ), &theSourceFile); 
		User::Leave(theRes);
		}
	
	//[ Prepare Encode Data ]
	TConvertInfo theConvInfo;
	theConvInfo.iImageTypeId = iImageTypeUid;
	theConvInfo.iDisplayMode = theBitmap->DisplayMode();
	
	TImageDataBlock* imageData = NULL;
	TFrameDataBlock* frameData = NULL;
	
	CFrameImageData* theFrameImageData = CFrameImageData::NewL();
	CleanupStack::PushL(theFrameImageData);

	theRes	 = PrepareEncodeDataL(theConvInfo, &imageData, &frameData);
	//
	if(imageData)
		{
		CleanupStack::PushL(imageData);
		User::LeaveIfError(theFrameImageData->AppendImageData(imageData));
		CleanupStack::Pop(imageData);
		}
	if(frameData)
		{
		CleanupStack::PushL(frameData);	
		User::LeaveIfError(theFrameImageData->AppendFrameData(frameData));
		CleanupStack::Pop(frameData);
		}
		
	//[ Create an active Listener and push it on the cleanup stack ]
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	iActiveScheduler->SetEncoder(iImageEncoder, activeListener, 0);
	iActiveScheduler->SetStep(this);
	
	//[ Convert Method of the Encoder ]
	activeListener->InitialiseActiveListener();
	iImageEncoder->Convert(&activeListener->iStatus, *theBitmap, theFrameImageData);
	CActiveScheduler::Start();
	theRes = activeListener->iStatus.Int();
	iActiveScheduler->SetStep(NULL);

	if(theRes == KErrNone)
		{
		INFO_PRINTF2(_L("Encoding of file %S was successful"), &aFileNameSrc);
		}
	else
		{
		INFO_PRINTF2(_L("Failed to Encode the file : %S"), &aFileNameSrc);
		User::Leave(theRes);
		}

	//[ Compare two files into a flat buffer ]
	TFileName theDestinationFileName; 
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, theDestinationFileName);
	theDestinationFileName.Append(aFileNameSrc.Left(aFileNameSrc.Length()-3));
	GetImageExtension(iImageTypeUid, theExtension);
	theDestinationFileName.Append(theExtension);
	
	iFile.Close(); //- As a precaution.
	
	TInt fileSize;
	theRes = iFile.Open(iFs, theDestinationFileName, EFileWrite);
	if(theRes != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open file: %S to Compare"), &theDestinationFileName);
		User::Leave(theRes);
		}
	iFile.Size(fileSize);
	HBufC8 *theCreatedImage = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theCreatedImage->Des();
	iFile.Read(imageFromFilePtr);
	
	iFile.Close();
	
	//[ Call the CompareFileL Function ]
	theRes = CompareFileL(aFileNameSrc, theCreatedImage, iImageTypeUid, EInput);
	User::LeaveIfError(theRes);
	//[ Delete the Local Objects ]
	CleanupStack::PopAndDestroy(4,theBitmap); //CActiveListener
	}

