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
// This file contains the test steps ICL YUV testing  for Bravo - PREQ 747.
// 
//

// EPOC includes
#include <testframework.h>
#include <icl/icl_uids.hrh>
#include "TestStepYUVConversion.h"
#include "ICLFbsSessionTest.h"
#include <iclexifimageframe.h>

#define INFO_PRINTF1_IF_LOGS_ENABLED( a ) if(iEnableLogs) 		\
											{					\
											INFO_PRINTF1( a );	\
											}											
											
#define INFO_PRINTF2_IF_LOGS_ENABLED( a, b ) if(iEnableLogs)					\
												{								\
												INFO_PRINTF2( ( a ), ( b ) );	\
												}
											
#define INFO_PRINTF3_IF_LOGS_ENABLED( a, b, c ) if(iEnableLogs)								\
													{										\
													INFO_PRINTF3( ( a ), ( b ), ( c ) );	\
													}
											
#define ERR_PRINTF1_IF_LOGS_ENABLED( a ) if(iEnableLogs)		\
											{					\
											ERR_PRINTF1( a );	\
											}											
											
#define ERR_PRINTF2_IF_LOGS_ENABLED( a, b ) if(iEnableLogs)						\
												{								\
												ERR_PRINTF2( ( a ), ( b ) );	\
												}
											
#define ERR_PRINTF3_IF_LOGS_ENABLED( a, b, c ) if(iEnableLogs)							\
													{									\
													ERR_PRINTF3( ( a ), ( b ), ( c ) );	\
													}											
										
																																	
const TUid KImageFramePluginUid  = {0x101F7C60};
const TUid KTestImageFramePluginUid = {0x101f7c61};

const TInt KRChunkSize  = 0;

//
// POSITIVE TESTS
//

//
// CTestConvertYuvRchunk
//

 CTestStepYUVConversion::CTestStepYUVConversion(const TDesC& aTestName) : CTestStepConversion(aTestName)
 {
 	iHeapSize = 500000;
 	iEnableLogs = ETrue;//By deafult log the messages    
 }

/*
*
* OpenImageAndYUVDecodeToDesc
* MM-ICL-FRM-I-1001-CP,  MM-ICL-FRM-I-1008-HP
*
*/

TInt CTestStepYUVConversion::DecodeToRChunkAndEncodeToDescL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());
    CleanupClosePushL(iFs);
    
    INFO_PRINTF2_IF_LOGS_ENABLED(_L("Opening the file %S for "), &theSourceFilename);
    // Open the image and create an image imageDecoder
    CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err,imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	    
    if (err != KErrNone)
        {
        INFO_PRINTF3_IF_LOGS_ENABLED(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        User::Leave(err);
        }
    if (imageDecoder == NULL)
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }
    CleanupStack::PushL(imageDecoder);
	INFO_PRINTF2_IF_LOGS_ENABLED(_L("The file %S had been opened."), &theSourceFilename);
    
    // create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = imageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		INFO_PRINTF1_IF_LOGS_ENABLED(_L("Codec does not support this image format"));
		User::Leave(KErrNotSupported);
		}
		
	TInt res = chunk.CreateGlobal(_L("aRchunkICL"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
	User::LeaveIfError(res);
	
	CleanupClosePushL(chunk);
  		
  	// create an empty imageframe   
  	CImageFrame* theImageFrame = NULL;
	TRAP(err, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KRChunkSize));
	if (err != KErrNone)
		{
		INFO_PRINTF2_IF_LOGS_ENABLED(_L("Could not do Create imageFrame"),err);
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
		CleanupStack::PushL(theImageFrame);
	// Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    
    activeListener->InitialiseActiveListener();
    
    // Decode the image
    imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err != KErrNone)
        {
        ERR_PRINTF2_IF_LOGS_ENABLED(_L("Error during Decoding of the file: %S"), &theSourceFilename);
        User::Leave(err);
        }
        
    // Encode //
    
	// Create the destination buffer
	HBufC8 *theDestinationImage = NULL;
	//CleanupStack::PushL(theDestinationImage);
	
	// Create an object of CJPEGImageFrameEncoder
    CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::DataNewL(theDestinationImage, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		delete theDestinationImage;
		theDestinationImage = NULL;
		ERR_PRINTF3_IF_LOGS_ENABLED(_L("Error during creating Image Encoder UId %d to file %S" ), iImageTypeUid, &iFileNameDest);			 		
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2_IF_LOGS_ENABLED(_L("Path %S not found" ), &iFileNameDest);
    		}
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Access denied" ));
    		}
		User::Leave(err);
		}
	CleanupStack::PushL(imageEncoder);

    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
  
  	
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
    if (err != KErrNone)
		{
		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Error during conversion"));
		User::Leave(err);
		}
	
	// Cleanup
	CleanupStack::PopAndDestroy(6); //active scheduler

    //compare two file into a flat buffer
	err = CompareFileL(aFileName, theDestinationImage, iImageTypeUid, EReference); 
	User::LeaveIfError(err);
    
    delete theDestinationImage;
	theDestinationImage = NULL;
	CleanupStack::PopAndDestroy(1);
	
	return err;
    }

/*
*
* OpenImageAndYUVDecodeToRChunkL
* MM-ICL-FRM-I-1002-CP, MM-ICL-FRM-I-1009-HP 
*
*/

TInt CTestStepYUVConversion::DecodeToRChunkAndEncodeToFileL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);
	
	INFO_PRINTF2_IF_LOGS_ENABLED(_L("Opening the file %S for "), &theSourceFilename);
    
    // Open the image and create an image imageDecoder
    CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err,imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	    
    if (err != KErrNone)
        {
        INFO_PRINTF3_IF_LOGS_ENABLED(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        User::Leave(err);
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }
    CleanupStack::PushL(imageDecoder);

	INFO_PRINTF2_IF_LOGS_ENABLED(_L("The file %S had been opened."), &theSourceFilename);
    
	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = imageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		INFO_PRINTF1_IF_LOGS_ENABLED(_L("Codec does not support this image format"));
		User::Leave(KErrNotSupported);
		}
		
	TInt res = chunk.CreateGlobal(_L("aRchunkICL"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
	User::LeaveIfError(res);
	
	CleanupClosePushL(chunk);
  		
	// create an empty imageframe   
  	CImageFrame* theImageFrame = NULL;
	TRAP(err, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes,KRChunkSize));
	if (err != KErrNone)
		{
		INFO_PRINTF2_IF_LOGS_ENABLED(_L("Could not do Create imageFrame"),err);
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
		CleanupStack::PushL(theImageFrame);
	 	
	// Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    activeListener->InitialiseActiveListener();
    
    // Decode the image
    imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err != KErrNone)
        {
        ERR_PRINTF2_IF_LOGS_ENABLED(_L("Error during Decoding of the file: %S"), &theSourceFilename);
        User::Leave(err);
        }
        
    // Encode //
    
	// Create the destination filename from the source filename
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, iFileNameDest);
	iFileNameDest.Append(aFileName.Left(aFileName.Length() - 4));
	iFileNameDest.Append(_L("_OUT."));

	GetImageExtension(iImageTypeUid, theExtension);
	iFileNameDest.Append(theExtension);
	
	// Create an object of CJPEGImageFrameEncoder
    CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iFileNameDest, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3_IF_LOGS_ENABLED(_L("Error during creating Image Encoder UId %d to file %S" ), iImageTypeUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2_IF_LOGS_ENABLED(_L("Path %S not found" ), &iFileNameDest);
    		}
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Access denied" ));
    		}
		User::Leave(err);
		}
	CleanupStack::PushL(imageEncoder);

    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL(activeEncodeListener);

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
  
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
    if (err != KErrNone)
		{
		ERR_PRINTF2_IF_LOGS_ENABLED(_L("Error during conversion"),err);
		User::Leave(err);
		}
	
	// Cleanup
	CleanupStack::PopAndDestroy(6); //active scheduler
	
	//compare two file into a flat buffer
	TInt fileSize;
	
	iFile.Open(iFs, iFileNameDest, EFileRead);
	CleanupClosePushL(iFile);
	
	iFile.Size(fileSize);
	INFO_PRINTF2_IF_LOGS_ENABLED(_L("File Size: %d"),fileSize);
	HBufC8 *theCreatedImage = HBufC8::NewMaxLC(fileSize);
			
	TPtr8 imageFromFilePtr = theCreatedImage->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();
	
	err = CompareFileL(aFileName, theCreatedImage, iImageTypeUid, EReference); 
    User::LeaveIfError(err);
    
	// Cleanup
	CleanupStack::PopAndDestroy(3); //theCreatedImage
	
    return err;
    }

/**
*
* DecodeToDescAndEncodeToDescL
* MM-ICL-FRM-I-1003-CP,  MM-ICL-FRM-I-1010-HP
*
*/


TInt CTestStepYUVConversion::DecodeToDescAndEncodeToDescL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());
    CleanupClosePushL(iFs);

	INFO_PRINTF2_IF_LOGS_ENABLED(_L("Opening the file %S for "), &theSourceFilename);
    
    // Open the image and create an image imageDecoder
    CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err, imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

    if (err != KErrNone)
        {
        INFO_PRINTF3_IF_LOGS_ENABLED(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        User::Leave(err);
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }
    CleanupStack::PushL(imageDecoder);

	INFO_PRINTF2_IF_LOGS_ENABLED(_L("The file %S had been opened for Decode."), &theSourceFilename);
	
	// create destination Desc
	TInt imageSizeInBytes;
	TBool supported = imageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
	{
		INFO_PRINTF1_IF_LOGS_ENABLED(_L("Codec does not support this image format"));
		User::Leave(KErrNotSupported);
	}
	
	HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
	CleanupStack::PushL(buffer);
	TPtr8 ptr = buffer->Des();
  	
	// create an empty imageframe   
	CImageFrame* theImageFrame = NULL;
	TRAP(err, theImageFrame =	CImageFrame::NewL(ptr, imageSizeInBytes));
	if (err != KErrNone)
		{
		INFO_PRINTF2_IF_LOGS_ENABLED(_L("Could not do Create imageFrame"),err);
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
	CleanupStack::PushL(theImageFrame);
	
	// Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    activeListener->InitialiseActiveListener();

    // Decode the image
    imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
    CActiveScheduler::Start();
    err = activeListener->iStatus.Int();
    
    if (err != KErrNone)
        {
        ERR_PRINTF2_IF_LOGS_ENABLED(_L("Error during conversion of the file: %S"), &theSourceFilename);
        User::Leave(err);
        }
    
    // Encode //
    
	// Create the destination buffer
	HBufC8 *theDestinationImage = NULL;
	
	// Create an object of CJPEGImageFrameEncoder
    CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::DataNewL(theDestinationImage, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3_IF_LOGS_ENABLED(_L("Error during creating Image Encoder UId %d to file %S" ), iImageTypeUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2_IF_LOGS_ENABLED(_L("Path %S not found" ), &iFileNameDest);
    		}
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Access denied" ));
    		}
		User::Leave(err);
		}
	CleanupStack::PushL(imageEncoder);

    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
  
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
    if (err != KErrNone)
		{
		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Error during conversion"));
		delete theDestinationImage;
		theDestinationImage = NULL;
		
		User::Leave(err);
		}
	
	// Cleanup
	CleanupStack::PopAndDestroy(6); //active sheduler
	
    //compare two file into a flat buffer
	err = CompareFileL(aFileName, theDestinationImage , iImageTypeUid, EReference); 
    User::LeaveIfError(err);
    
    delete theDestinationImage;
	theDestinationImage = NULL;
	CleanupStack::PopAndDestroy(1);
	
	return err;
    }

/**
*
* DecodeToDescAndEncodeToFileL
* MM-ICL-FRM-I-1004-CP,  MM-ICL-FRM-I-1011-HP
*
*/


TInt CTestStepYUVConversion::DecodeToDescAndEncodeToFileL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());
	CleanupClosePushL(iFs);

	INFO_PRINTF2_IF_LOGS_ENABLED(_L("Opening the file %S for "), &theSourceFilename);
    
    // Open the image and create an image imageDecoder
    CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err, imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

    if (err != KErrNone)
        {
        INFO_PRINTF3_IF_LOGS_ENABLED(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        User::Leave(err);
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }
    CleanupStack::PushL(imageDecoder);

	INFO_PRINTF2_IF_LOGS_ENABLED(_L("The file %S had been opened for Decode."), &theSourceFilename);
    
    // create destination Desc
	TInt imageSizeInBytes;
	TBool supported = imageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
	{
		INFO_PRINTF1_IF_LOGS_ENABLED(_L("Codec does not support this image format"));
		User::Leave(KErrNotSupported);
	}
	
	HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
	CleanupStack::PushL(buffer);
	TPtr8 ptr = buffer->Des();
  	
	// create an empty imageframe   
	CImageFrame* theImageFrame = NULL;
	TRAP(err, theImageFrame =	CImageFrame::NewL(ptr, imageSizeInBytes));
	if (err != KErrNone)
		{
		INFO_PRINTF2_IF_LOGS_ENABLED(_L("Could not do Create imageFrame"),err);
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
	CleanupStack::PushL(theImageFrame);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    activeListener->InitialiseActiveListener();

    // Decode the image
    imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
    CActiveScheduler::Start();
    err = activeListener->iStatus.Int();
    
    if (err != KErrNone)
        {
        ERR_PRINTF2_IF_LOGS_ENABLED(_L("Error during conversion of the file: %S"), &theSourceFilename);
        User::Leave(err);
        }
    
    // Encode //
    
	// Create the destination filename from the source filename
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, iFileNameDest);
	iFileNameDest.Append(aFileName.Left(aFileName.Length() - 4));
	iFileNameDest.Append(_L("_OUT."));

	GetImageExtension(iImageTypeUid, theExtension);
	iFileNameDest.Append(theExtension);
	
	// Create an object of CJPEGImageFrameEncoder
    CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iFileNameDest, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3_IF_LOGS_ENABLED(_L("Error during creating Image Encoder UId %d to file %S" ), iImageTypeUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2_IF_LOGS_ENABLED(_L("Path %S not found" ), &iFileNameDest);
    		}
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Access denied" ));
    		}
		User::Leave(err);
		}
	CleanupStack::PushL(imageEncoder);

    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
  
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
    if (err != KErrNone)
		{
		ERR_PRINTF1_IF_LOGS_ENABLED(_L("Error during conversion"));
		User::Leave(err);
		}
	
	// Cleanup
	CleanupStack::PopAndDestroy(6); //active sheduler
		
    //compare two file into a flat buffer
	TInt fileSize;
	
	iFile.Open(iFs, iFileNameDest, EFileRead);
	CleanupClosePushL(iFile);
	
	iFile.Size(fileSize);
	HBufC8 *theCreatedImage = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theCreatedImage->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();

	err = CompareFileL(aFileName, theCreatedImage, iImageTypeUid, EReference); 
    User::LeaveIfError(err);
    
	// Cleanup
	CleanupStack::PopAndDestroy(3);
	
    return err;
    }
		
/*
*
* DecodeToRChunkAndEncodeToFileXtraParamsL
* MM-ICL-FRM-I-1005-LP
*
*/

TInt CTestStepYUVConversion::DecodeToRChunkAndEncodeToFileXtraParamsL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file %S for "), &theSourceFilename);
    
    // Open the image and create an image imageDecoder
    CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err,imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionAlwaysThread, KNullUid, KNullUid, KImageFramePluginUid)));
	    
    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        return err;
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        return err;
        }
    CleanupStack::PushL(imageDecoder);

	INFO_PRINTF2_IF_LOGS_ENABLED(_L("The file %S had been opened."), &theSourceFilename);
    
	// create destination RChunk
		RChunk chunk;
		TInt imageSizeInBytes;
		TBool supported = imageDecoder->RecommendedBufferSize(KUidFormatYUV420Planar, imageSizeInBytes);
		if (supported == EFalse)
			{
			ERR_PRINTF1(_L("The format 'KUidFormatYUV420Planar' is not Supported"));
			User::Leave(KErrNotSupported);
			}
		
		TInt res = chunk.CreateGlobal(_L("aRchunkICL"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
		User::LeaveIfError(res);
	
	  	CleanupClosePushL(chunk);
  	
		// create an empty imageframe   
		CImageFrame* theImageFrame = NULL;
		TRAP(err, theImageFrame =	CImageFrame::NewL(&chunk, imageSizeInBytes,KRChunkSize));
		if (err != KErrNone)
			{
			INFO_PRINTF1(_L("Could not do Create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			return EFail;
			}
		CleanupStack::PushL(theImageFrame);

	// Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    activeListener->InitialiseActiveListener();
    
    // Decode the image
    imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Error during Decoding of the file: %S"), &theSourceFilename);
        User::Leave(err);
        }
        
    // Encode //
    
	// Create the destination filename from the source filename
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, iFileNameDest);
	iFileNameDest.Append(aFileName.Left(aFileName.Length() - 4));
	iFileNameDest.Append(_L("_OUT."));

	GetImageExtension(iImageTypeUid, theExtension);
	iFileNameDest.Append(theExtension);
	
	// Create an object of CJPEGImageFrameEncoder
    CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iFileNameDest, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), iImageTypeUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &iFileNameDest); 
    		}
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		return err;
		}
	CleanupStack::PushL(imageEncoder);
	
	// Create CFrameImageData 
    CFrameImageData* frameImageData = CFrameImageData::NewL();
    CleanupStack::PushL(frameImageData);
        
    TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
	CleanupStack::PushL(jpegFormat);

	jpegFormat->iSampleScheme = TJpegImageData::EColor420;
	jpegFormat->iQualityFactor = 100;
	User::LeaveIfError(frameImageData->AppendImageData(jpegFormat));
	CleanupStack::Pop(jpegFormat);

    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
  
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame, frameImageData);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
    if (err != KErrNone)
		{
		ERR_PRINTF1(_L("Error during conversion"));
		return err;
		}
	
	// Cleanup
	CleanupStack::PopAndDestroy(7, imageDecoder); 
	
    //compare two file into a flat buffer
	TInt fileSize;
	
	iFile.Open(iFs, iFileNameDest, EFileRead);
	
	CleanupClosePushL(iFile);
	
	iFile.Size(fileSize);
	HBufC8 *theCreatedImage = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theCreatedImage->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();

	err = CompareFileL(aFileName, theCreatedImage, iImageTypeUid, EReference); 
    User::LeaveIfError(err);
    
	// Cleanup
	CleanupStack::PopAndDestroy(2);
	
    return err;
    }

/**
*
* EnquireYUVDecoderPluginUIDL
* MM-ICL-FRM-I-1006-LP
*
*/


TInt CTestStepYUVConversion::EnquireYUVDecoderPluginUIDL(const TDesC& aFileName)
	{
	
	User::LeaveIfError(iFs.Connect());
    
    // - Load from File
	TFileName	theSourceFilename;
	Directory(EInput, theSourceFilename);
	theSourceFilename.Append(aFileName);

	CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err, imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	
    if (err != KErrNone)
	    {
	    INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
	    return err;
	    }
    
	if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        return err;
        }
    CleanupStack::PushL(imageDecoder);

	TUid uid1 = imageDecoder->ImplementationUid();
	INFO_PRINTF3(_L("UID of ImageDecoder loaded from file %S = 0x%08x\n"), &theSourceFilename, uid1);
	if (uid1 == KTestImageFramePluginUid)
	{
	INFO_PRINTF2(_L("UID of ImageDecoder loaded from buffer is valid"),uid1);
	}
		
	CleanupStack::PopAndDestroy(1);
	
	// - Load from Descriptor
	TInt fileSize;

	iFile.Open(iFs, theSourceFilename, EFileRead);
	iFile.Size(fileSize);
	HBufC8 *theImageFromFile = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();
	
	TRAP(err, imageDecoder = static_cast<CJPEGImageFrameDecoder *>(CImageDecoder::DataNewL(iFs, imageFromFilePtr)));
	if(imageDecoder == NULL)
		{
		return KErrUnknown;
		}
		
	CleanupStack::PushL(imageDecoder);
	
	uid1 = imageDecoder->ImplementationUid();
	INFO_PRINTF2(_L("UID of ImageDecoder loaded from buffer = 0x%08x\n"), uid1);
	
	if (uid1 == KTestImageFramePluginUid)
	{
	INFO_PRINTF1(_L("UID of ImageDecoder loaded from buffer is valid"));
	}

    // Clean-up
	 //imageDecoder, theImageFromFile
	CleanupStack::PopAndDestroy(2, theImageFromFile);
	return err;
	}

/* 
 *
 * Enquire UID of a loaded encoder 
 * MM-ICL-FRM-I-1006-LP
 *
 */
 
 
TInt CTestStepYUVConversion::EnquireYUVEncoderPluginUIDL(const TDesC& aFileName)
	{

	User::LeaveIfError(iFs.Connect());

	TFileName	iFileNameDest;
	Directory(EInput, iFileNameDest);
	iFileNameDest.Append(aFileName);
	
	CJPEGImageFrameDecoder* imageEncoder = NULL;
	TRAPD(err, imageEncoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, iFileNameDest, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot create file with Uid  = 0x%08x\n"), iImageTypeUid);
		if (err == KErrPathNotFound)
			{
			INFO_PRINTF2(_L("Path %S not found"), &iFileNameDest);
			}
		return err;
		}
	CleanupStack::PushL(imageEncoder);	
	
	TUid uid1 = imageEncoder->ImplementationUid();
	INFO_PRINTF3(_L("UID of ImageEncoder loaded by file %S = 0x%08x\n"), &iFileNameDest, uid1);
	if (uid1 == KTestImageFramePluginUid)
	{
	INFO_PRINTF1(_L("UID of ImageEncoder loaded from buffer is valid"));
	}

	CleanupStack::PopAndDestroy(imageEncoder);
	return err;
	}

/* 
 *
 * ListYUVDecoderSupportedFormats
 * MM-ICL-FRM-I-1007-CP
 *
 */
 
 
TInt CTestStepYUVConversion::ListYUVDecoderSupportedFormats()
	{
	TInt	err;
	RFileExtensionMIMETypeArray	theFileExtensionArray;
	
	TRAP(err, CJPEGImageFrameDecoder::GetFileTypesL(theFileExtensionArray));	
	
	if (err != KErrNone)
		{
		theFileExtensionArray.ResetAndDestroy();
		return err;
		}

	
	INFO_PRINTF1(_L("A List of imageDecoder file extensions and MIME types"));
	
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
	
	return err;
	}


/* 
 *
 * ListYUVEncoderSupportedFormats
 * MM-ICL-FRM-I-1007-CP
 *
 */
TInt CTestStepYUVConversion::ListYUVEncoderSupportedFormats()
	{
	TInt	err = KErrNone;
	
	RFileExtensionMIMETypeArray	theFileExtensionArray;

	TRAP(err, CJPEGImageFrameEncoder::GetFileTypesL(theFileExtensionArray));	
	
	if (err != KErrNone)
		{
		theFileExtensionArray.ResetAndDestroy();
		return err;
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
	
	return err; 
	}


//
// NEGATIVE TESTS
//

/* 
 * YUVEncodeUnsupportedFrameL
 * MM-ICL-FRM-I-1112-CP
 *
 */

TInt CTestStepYUVConversion::YUVEncodeUnsupportedFrameL(const TDesC& aFileName)
	{
		 // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file %S for "), &theSourceFilename);
    
    // Open the image and create an image imageDecoder
    CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err,imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	    
    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        User::Leave(err);
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }
    CleanupStack::PushL(imageDecoder);
	
	INFO_PRINTF2_IF_LOGS_ENABLED(_L("The file %S had been opened."), &theSourceFilename);
    
    	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = imageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		INFO_PRINTF1(_L("Codec does not support this image format"));		
		User::Leave(KErrNotSupported);
		}
		
	TInt res = chunk.CreateGlobal(_L("aRchunkICL"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
	User::LeaveIfError(res);
	
	CleanupClosePushL(chunk);
  		
	// create an empty imageframe   
  	CImageFrame* theImageFrame = NULL;
	TRAP(err, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes,KRChunkSize));
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Could not do Create imageFrame"),err);
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
		CleanupStack::PushL(theImageFrame);
	
	// Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    activeListener->InitialiseActiveListener();
    
    // Decode the image
    imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Error during Decoding of the file: %S"), &theSourceFilename);
        User::Leave(err);
        }
        
    // ENCODING //
	// Create the destination filename from the source filename
	
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, iFileNameDest);
	iFileNameDest.Append(aFileName.Left(aFileName.Length() - 4));
	iFileNameDest.Append(_L("_OUT."));

	GetImageExtension(iImageTypeUid, theExtension);
	iFileNameDest.Append(theExtension);
	
	// Create an object of CJPEGImageFrameEncoder
	CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, 
	    imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iFileNameDest, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
    
    if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), KImageTypeGIFUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &iFileNameDest); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		User::Leave(err);
		}
	CleanupStack::PushL(imageEncoder);
    
    // Create CFrameImageData 
    CFrameImageData* frameImageData = CFrameImageData::NewL();
    CleanupStack::PushL(frameImageData);
		
    TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
	CleanupStack::PushL(jpegFormat);

	jpegFormat->iSampleScheme = TJpegImageData::EColor444;
	User::LeaveIfError(frameImageData->AppendImageData(jpegFormat));
	CleanupStack::Pop(jpegFormat);
	
    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
	
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame, frameImageData);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
	
	INFO_PRINTF2(_L("Encode an YUV to unsuppored image format returned Error : %d..."), err);
    User::LeaveIfError(err);
    
    // Cleanup
	CleanupStack::PopAndDestroy(7, imageDecoder); //active sheduler
	return err; 
	}


/**
 * 
 * YUVEncodeCorruptRChunkL
 * MM-ICL-FRM-I-1113-CP
 *
 */
 
 TInt CTestStepYUVConversion::YUVEncodeCorruptRChunkL(const TDesC& aFileName)
	{
    User::LeaveIfError(iFs.Connect());

    // create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes = 1000;
	TInt res = chunk.CreateGlobal(_L("aRchunkICL"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
	User::LeaveIfError(res);
	
	CleanupClosePushL(chunk);
  		
	// create an empty imageframe   
  	CImageFrame* theImageFrame = NULL;
	TRAPD(err, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes,KRChunkSize));
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Could not do Create imageFrame"),err);
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
		CleanupStack::PushL(theImageFrame);
    
    // here we pass the RChunk as it is without decoding. So, it is Not a valid data.

    // ENCODING //
    
	// Create the destination filename from the source filename
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, iFileNameDest);
	iFileNameDest.Append(aFileName.Left(aFileName.Length() - 4));
	iFileNameDest.Append(_L("_OUT."));

	GetImageExtension(iImageTypeUid, theExtension);
	iFileNameDest.Append(theExtension);
	
	// Create an object of CJPEGImageFrameEncoder
	CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, 
	    imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iFileNameDest, CImageEncoder::EOptionNone, KNullUid, KNullUid,KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), KImageTypeGIFUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &iFileNameDest); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		User::Leave(err);
		}
	CleanupStack::PushL(imageEncoder);
    
      
    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
	
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
	INFO_PRINTF2(_L("Encode an YUV to unsuppored image format returned Error : %d..."), err);
	User::LeaveIfError(err);
	
    // Cleanup
	CleanupStack::PopAndDestroy(4, theImageFrame); //active sheduler
	return err; 
	}


/* 
 * YUVEncodeCorruptDescriptorL
 * MM-ICL-FRM-I-1114-CP
 * 
 */

TInt CTestStepYUVConversion::YUVEncodeCorruptDescriptorL(const TDesC& aFileName)
	{
   	User::LeaveIfError(iFs.Connect());

    // create destination Desc
	TInt imageSizeInBytes = 1000;
	HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
	CleanupStack::PushL(buffer);
	TPtr8 ptr = buffer->Des();

	// create an empty imageframe   
  	CImageFrame* theImageFrame = NULL;
	TRAPD(err, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
	if (err != KErrNone)
		{
		INFO_PRINTF1(_L("Could not do Create imageFrame"));
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		User::Leave(err);
		}
		CleanupStack::PushL(theImageFrame);

	// here we pass the Descriptor frame as it is without decoding. So, it is Not a valid data.
	
    // ENCODING //
    
	// Create the destination filename from the source filename
	
	TBuf<KLenBuffer> theExtension;

	Directory(EOutput, iFileNameDest);
	iFileNameDest.Append(aFileName.Left(aFileName.Length() - 4));
	iFileNameDest.Append(_L("_OUT."));

	GetImageExtension(iImageTypeUid, theExtension);
	iFileNameDest.Append(theExtension);
	
	// Create an object of CJPEGImageFrameEncoder
	CJPEGImageFrameEncoder* imageEncoder = NULL;
	TRAP(err, 
	    imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iFileNameDest, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image Encoder UId %d to file %S" ), KImageTypeGIFUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &iFileNameDest); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		User::Leave(err);	
		}
	CleanupStack::PushL(imageEncoder);
    
      
    // Create CActiveListener
	CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeEncodeListener );

	iActiveScheduler->SetEncoder(imageEncoder, activeEncodeListener, 0);
	iActiveScheduler->SetStep(this);

	activeEncodeListener->InitialiseActiveListener();
	
	// Do the Encode from YUV to JPEG
	imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeEncodeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
	INFO_PRINTF2(_L("Encode an YUV to unsuppored image format returned Error : %d..."), err);
	User::LeaveIfError(err);
		                                                                                                                                                                                  
    // Cleanup
	CleanupStack::PopAndDestroy(4, buffer);
	return err; 
	}

/* 
 * JPEGDecodeUnsupportedYUVL
 * MM-ICL-FRM-I-1117-CP
 *
 */

TInt CTestStepYUVConversion::JPEGDecodeUnsupportedYUVL(const TDesC& aFileName)
	{
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file %S for "), &theSourceFilename);
	
	// Create an object of CJPEGImageFrameDecoder
	CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err, 
	    imageDecoder = static_cast<CJPEGImageFrameDecoder*> (CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image imageDecoder UId %d to file %S" ), KImageTypeGIFUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &theSourceFilename); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		User::Leave(err);	
		}
	CleanupStack::PushL(imageDecoder);

		// create destination RChunk
		RChunk chunk;
		TInt imageSizeInBytes;
		TBool supported = imageDecoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
		{
			// We want to test transcoding, create chunk anyway with a dummy size
			imageSizeInBytes = 1000;
		}
		
		TInt res = chunk.CreateGlobal(_L("aRchunkICL"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
		User::LeaveIfError(res);
	
	  	CleanupClosePushL(chunk);
  	
		// create an empty imageframe   
		CImageFrame* theImageFrame = NULL;
		TRAPD(allocErr, theImageFrame =	CImageFrame::NewL(&chunk, imageSizeInBytes,KRChunkSize));
		if (allocErr != KErrNone)
			{
			INFO_PRINTF1(_L("Could not do Create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			return EFail;
			}
		CleanupStack::PushL(theImageFrame);

			TFrameFormat the444FrameFormat = TFrameFormat(KUidFormatYUV444Interleaved );
			theImageFrame->SetFrameFormatL(the444FrameFormat);			
		
      
    // Create CActiveListener
	CActiveListener* activeListener = new(ELeave)CActiveListener;
	CleanupStack::PushL( activeListener );

	iActiveScheduler->SetDecoder(imageDecoder, activeListener, 0);
	iActiveScheduler->SetStep(this);

	activeListener->InitialiseActiveListener();
	
	// Do the Decode from JPEG to YUV
	imageDecoder->ConvertFrame(&activeListener->iStatus, *theImageFrame);
	CActiveScheduler::Start();

	err = activeListener->iStatus.Int();
	iActiveScheduler->ResetValues();
    
	INFO_PRINTF2(_L("JPEG Decode to unsuppored YUV returned Error : %d..."), err);                                                                                                                                                                                  
	User::LeaveIfError(err);
	
    // Cleanup
	CleanupStack::PopAndDestroy(4, imageDecoder); //active sheduler
	return err; 
	}
	

/* 
 * CorruptJPEGDecodeYUVL
 * MM-ICL-FRM-I-1118-CP
 *
 */


TInt CTestStepYUVConversion::CorruptJPEGDecodeYUVL(const TDesC& aFileName)
	{
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file %S for "), &theSourceFilename);

	// Create an object of CJPEGImageFrameDecoder
	CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err, 
	    imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
    
    if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image Decoder UId 0x%08x to file %S" ), KImageTypeGIFUid, &theSourceFilename);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &theSourceFilename); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		User::Leave(err);	
		}
	CleanupStack::PushL(imageDecoder);
    
    // Cleanup
	CleanupStack::PopAndDestroy(4, imageDecoder); //active sheduler
	return err; 
	}


/* 
 * CorruptJPEGDecodeDescYUVL
 * MM-ICL-FRM-I-1119-HP
 *
 */


TInt CTestStepYUVConversion::CorruptJPEGDecodeDescYUVL(const TDesC& aFileName)
	{
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file %S for "), &theSourceFilename);
    
	// Pass in a corrupt Descriptor
	TInt fileSize;

	iFile.Open(iFs, theSourceFilename, EFileRead);
	iFile.Size(fileSize);
	HBufC8 *theImageFromFile = HBufC8::NewMaxLC(fileSize);

	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();
		
	// Create an object of CJPEGImageFrameDecoder
	CJPEGImageFrameDecoder* imageDecoder = NULL;
	TRAPD(err, 
	imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::DataNewL(iFs, imageFromFilePtr, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	
	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image Decoder UId 0x%08x to file %S" ), KImageTypeGIFUid, &theSourceFilename);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &theSourceFilename); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
    	CleanupStack::PopAndDestroy(1);
		User::Leave(err);	
		}
	CleanupStack::PushL(imageDecoder);
                                                                                                                                                                                    
    // Cleanup
	CleanupStack::PopAndDestroy(5, theImageFromFile); //active sheduler
	return err; 
	}


/* 
 * GIFDecodeYUVL
 * MM-ICL-FRM-I-1120-HP
 *
 */

TInt CTestStepYUVConversion::GIFDecodeYUVL(const TDesC& aFileName)
	{
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file %S for "), &theSourceFilename);
	
	// Create an object of CJPEGImageFrameDecoder
	CJPEGImageFrameDecoder* imageDecoder = NULL;
    TRAPD(err, 
	imageDecoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	
	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error during creating Image decoder UId 0x%08x to file %S" ), KImageTypeGIFUid, &iFileNameDest);
		if (err == KErrPathNotFound)
    		{
    		ERR_PRINTF2(_L("Path %S not found" ), &theSourceFilename); 
    		}
		
		if (err == KErrAccessDenied)
    		{
    		ERR_PRINTF1(_L("Access denied" )); 
    		}
		User::Leave(err);	
		}
	CleanupStack::PushL(imageDecoder);
	
    // Cleanup
	CleanupStack::PopAndDestroy(4, imageDecoder); //active sheduler
	return err; 
	}

/**
*
* EnableLogs()
*
* @param TBool aEnable
*
*/
inline void CTestStepYUVConversion::EnableLogs(TBool aEnable)
{
	iEnableLogs = aEnable;	
}
//
// CTestConvertYUVRChunk
//

/**
 * 
 * Class Constructor
 *
 */
CTestConvertYUVRChunk::CTestConvertYUVRChunk(const TDesC& aTestName) : 
		 CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 * NewL Method
 *
 */
CTestConvertYUVRChunk *CTestConvertYUVRChunk::NewL(const TDesC& aTestName)
	{
	CTestConvertYUVRChunk* self = new(ELeave) CTestConvertYUVRChunk(aTestName);
	return self;
	}



/**
 * 
 * Do the test step.
 * Each test step must supply an implementation for DoTestStepL.
 *
 */
TVerdict CTestConvertYUVRChunk::TestStepL()
	{
    TInt err = KErrNone;
    
	// Call the Encoder function, whether to file or descriptor based on the test case id.
    if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1001-CP")) == 0) || (iTestStepName.Compare(_L("MM-ICL-FRM-I-1008-HP")) == 0))
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("Decode JPEG to YUV RChunk and Encode the YUV RChunk to a JPEG descriptor."));
        // Decode to RChunk and Encode to a JPEG descriptor
        TRAP(err, DecodeToRChunkAndEncodeToDescL(iFileNameSrc));    
        }
    else if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1002-CP")) == 0) || (iTestStepName.Compare(_L("MM-ICL-FRM-I-1009-HP")) == 0))
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("encode the YUV RChunk to a JPEG file."));
        // Encode the YUV RChunk to a JPEG file
        TRAP(err, DecodeToRChunkAndEncodeToFileL(iFileNameSrc));
        }
  
    // Error Checking
    if (err != KErrNone)
        {
        INFO_PRINTF2_IF_LOGS_ENABLED(_L("Encode FAILED with error %d..."), err);
        User::Leave(err);
        }
	return EPass;
	}

/**
 * 
 * Do the test step.
 * Each test step must supply an implementation for DoTestStepL.
 *
 */
TVerdict CTestConvertYUVRChunk::DoTestStepL()
    {
    TVerdict testResult = EPass;
    TInt err;
    
    if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1001-CP")) == 0) ||
        (iTestStepName.Compare(_L("MM-ICL-FRM-I-1002-CP")) == 0))
        {
        TRAP(err, testResult = TestStepL());
        if (err != KErrNone)
	        {
	        INFO_PRINTF2(_L("Encode FAILED with error %d..."), err);
	        testResult = EFail;
	        }
        }
     else if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1008-HP")) == 0) ||
        (iTestStepName.Compare(_L("MM-ICL-FRM-I-1009-HP")) == 0))
        {
    	//Check for Alloc Test Support
    	__UHEAP_SETFAIL(RHeap::EFailNext, 1);
    	TAny *testAllocSupport = User::Alloc(1);
    	if (testAllocSupport!=NULL)
    		{
    		delete testAllocSupport;
    		testAllocSupport=NULL;
    		INFO_PRINTF1(_L("This build does not support ALLOC testing"));
    		return EInconclusive;
    		}
    	
    	//Initialise Variables
    	TVerdict iAllocTestStepResult=EPass;
    	TInt err = KErrNone;
    	TBool badResult = EFalse;
    	
    	//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
    	INFO_PRINTF1(_L("Executing the Test Method call Outside the FOR LOOP."));
    	TRAP(err, iAllocTestStepResult = TestStepL());
    	if (err != KErrNone)
    		{
    		INFO_PRINTF2(_L("Decoding error, returned error code =  %d"), err);
    		return EFail;
    		}
    	else
    		{
    		//Check the iAllocTestStepResult
    		if (iAllocTestStepResult != EPass)
    			{
    			badResult = ETrue;
    			}
    		}
    	
    	TInt failCount = 1;
    	TBool completed = EFalse;
    	iAllocTestStepResult = EPass; // reinitialise
    	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
    	EnableLogs(EFalse);
    	for(;;)	
    		{
    		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
    		//__MM_HEAP_MARK;

    		//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
    		INFO_PRINTF2_IF_LOGS_ENABLED(_L("Executing the Test Method call #%d."), failCount);	
      		TRAP(err, iAllocTestStepResult = TestStepL());
    		
    		completed = EFalse;
    		if (err == KErrNone)
    			{
    			TAny *testAlloc = NULL;
    			testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
    			if (testAlloc == NULL)
    				{
    				reachedEnd = ETrue;
    				failCount -= 1;
    				}
    			else
    				{
    				User::Free(testAlloc);
    				testAlloc=NULL;
    				}			
    			
    			//Check the iAllocTestStepResult
    			if (iAllocTestStepResult != EPass)
    				{
    				badResult = ETrue;
    				}
    			
    			completed = reachedEnd || badResult;
    			}
    		else if (err != KErrNoMemory) // bad error code
    			{
    			completed = ETrue;
    			badResult = EFail;
    			}			

    		//__MM_HEAP_MARKEND;
    		__UHEAP_SETFAIL(RHeap::ENone, 0);

    		if (completed)
    			{
    			break; // exit loop
    			}

    		failCount++;
    		}

    	//failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc

    	if (err != KErrNone || badResult)
    		{
    		if (badResult)
    			{
    			INFO_PRINTF3(_L("  Bad result with %d memory allocations tested. The Error returned is %d"), failCount, err);
    			}
    		else
    			{
    			INFO_PRINTF3(_L("  Error(%d) with %d memory allocations tested\n"), err, failCount);
    			}
    		iAllocTestStepResult = EFail;
    		}
    	else 
    		{
    		INFO_PRINTF2(_L("  Completed OK with %d memory allocations tested\n"), failCount);
    		iAllocTestStepResult = EPass;
    		}
    		testResult = iAllocTestStepResult;
        }
    return testResult;
	}

//
// CTestConvertYUVDesc
//
/**
 * 
 *
 * Class Constructor
 *
 */
CTestConvertYUVDesc::CTestConvertYUVDesc(const TDesC& aTestName) : 
		 CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 *
 * NewL Method
 *
 */
CTestConvertYUVDesc *CTestConvertYUVDesc::NewL(const TDesC& aTestName)
	{
	CTestConvertYUVDesc* self = new(ELeave) CTestConvertYUVDesc(aTestName);
	return self;
	}

/**
 * 
 *
 * DoTestStepL Method
 *
 */
TVerdict CTestConvertYUVDesc::TestStepL()
	{
    TInt err = KErrNone;
    
    // Call the Encoder function, whether to file or descriptor based on the test case id.
    if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1003-CP")) == 0) || (iTestStepName.Compare(_L("MM-ICL-FRM-I-1010-HP")) == 0))
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("Decode to YUV Desc and Encode to a JPEG descriptor."));
        // Encode the YUV RChunk to a JPEG descriptor
        TRAP(err, DecodeToDescAndEncodeToDescL(iFileNameSrc));
        }
    else if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1004-CP")) == 0) || (iTestStepName.Compare(_L("MM-ICL-FRM-I-1011-HP")) == 0))
        {
        INFO_PRINTF1_IF_LOGS_ENABLED(_L("Decode to YUV Desc and Encode to a JPEG file."));
        // Encode the YUV RChunk to a JPEG descriptor
        TRAP(err, DecodeToDescAndEncodeToFileL(iFileNameSrc));
        }
    
    // Error Checking
    if (err != KErrNone)
        {
        INFO_PRINTF2_IF_LOGS_ENABLED(_L("Encode FAILED with error %d..."), err);
        User::Leave(err);
        }
	return EPass;
	}

/**
 * 
 *
 * DoTestStepL Method
 *
 */
TVerdict CTestConvertYUVDesc::DoTestStepL()
	{
	TVerdict testResult = EPass;
    if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1003-CP")) == 0) ||
        (iTestStepName.Compare(_L("MM-ICL-FRM-I-1004-CP")) == 0))
        {
        testResult = TestStepL();
        }
     else if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-1010-HP")) == 0) ||
        (iTestStepName.Compare(_L("MM-ICL-FRM-I-1011-HP")) == 0))
        {
    	//Check for Alloc Test Support
    	__UHEAP_SETFAIL(RHeap::EFailNext, 1);
    	TAny *testAllocSupport = User::Alloc(1);
    	if (testAllocSupport!=NULL)
    		{
    		delete testAllocSupport;
    		testAllocSupport=NULL;
    		INFO_PRINTF1(_L("This build does not support ALLOC testing"));
    		return EInconclusive;
    		}
    	
    	//Initialise Variables
    	TVerdict iAllocTestStepResult=EPass;
    	TInt err = KErrNone;
    	TBool badResult = EFalse;
    	
    	//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
    	INFO_PRINTF1(_L("Executing the Test Method call Outside the FOR LOOP."));
    	TRAP(err, iAllocTestStepResult = TestStepL());
    	if (err != KErrNone)
    		{
    		INFO_PRINTF2(_L("Decoding error, returned error code =  %d"), err);
    		return EFail;
    		}
    	else
    		{
    		//Check the iAllocTestStepResult
    		if (iAllocTestStepResult != EPass)
    			{
    			badResult = ETrue;
    			}
    		}
    	
    	TInt failCount = 1;
    	TBool completed = EFalse;
    	iAllocTestStepResult = EPass; // reinitialise
    	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
    	EnableLogs(EFalse);
    	for(;;)	
    		{
    		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
    		//__MM_HEAP_MARK;

    		//>>>>>>>>>>>>>>>>>>>>>>>>Test Method Call<<<<<<<<<<<<<<<<<<<<<<<<<<
    		INFO_PRINTF2_IF_LOGS_ENABLED(_L("Executing the Test Method call #%d."), failCount);
    		TRAP(err, iAllocTestStepResult = TestStepL());
    		
    		completed = EFalse;
    		if (err == KErrNone)
    			{
    			TAny *testAlloc = NULL;
    			testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
    			if (testAlloc == NULL)
    				{
    				reachedEnd = ETrue;
    				failCount -= 1;
    				}
    			else
    				{
    				User::Free(testAlloc);
    				testAlloc=NULL;
    				}			
    			
    			//Check the iAllocTestStepResult
    			if (iAllocTestStepResult != EPass)
    				{
    				badResult = ETrue;
    				}
    			
    			completed = reachedEnd || badResult;
    			}
    		else if (err != KErrNoMemory) // bad error code
    			{
    			completed = ETrue;
    			badResult = EFail;
    			}			

    		//__MM_HEAP_MARKEND;
    		__UHEAP_SETFAIL(RHeap::ENone, 0);

    		if (completed)
    			{
    			break; // exit loop
    			}

    		failCount++;
    		}

    	//failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc

    	if (err != KErrNone || badResult)
    		{
    		if (badResult)
    			{
    			INFO_PRINTF3(_L("  Bad result with %d memory allocations tested. The Error returned is %d"), failCount, err);
    			}
    		else
    			{
    			INFO_PRINTF3(_L("  Error(%d) with %d memory allocations tested\n"), err, failCount);
    			}
    		iAllocTestStepResult = EFail;
    		}
    	else 
    		{
    		INFO_PRINTF2(_L("  Completed OK with %d memory allocations tested\n"), failCount);
    		iAllocTestStepResult = EPass;
    		}
    		testResult = iAllocTestStepResult;
        }
        return testResult;
	}


//
// CTestCSEncodeYUVExtraParams
//

/**
 *
 * Class Constructor
 *
 */
CTestCSEncodeYUVExtraParams::CTestCSEncodeYUVExtraParams(const TDesC& aTestName) : 
		 CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 * NewL Method
 *
 */
CTestCSEncodeYUVExtraParams *CTestCSEncodeYUVExtraParams::NewL(const TDesC& aTestName)
	{
	CTestCSEncodeYUVExtraParams* self = new(ELeave) CTestCSEncodeYUVExtraParams(aTestName);
	return self;
	}

/**
 *
 * DoTestStepL Method
 *
 */
TVerdict CTestCSEncodeYUVExtraParams::DoTestStepL()
	{
    TInt err1 = KErrNone;
    
    // Decode a Jpeg file to YUV Descriptor
    TRAPD(err, err1 = DecodeToRChunkAndEncodeToFileXtraParamsL(iFileNameSrc));
	if (err1 != KErrNone)
		{
		err = err1;
		}
	
    // Error Checking
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Conversion Failed with error %d..."), err);
        return EFail;
        }
    return EPass;  
	}


/**
 * 
 *
 * Class Constructor
 *
 */
CTestCSGetPluginUID::CTestCSGetPluginUID(const TDesC& aTestName) : 
		 CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 *
 * NewL Method
 *
 */
CTestCSGetPluginUID *CTestCSGetPluginUID::NewL(const TDesC& aTestName)
	{
	CTestCSGetPluginUID* self = new(ELeave) CTestCSGetPluginUID(aTestName);
	return self;
	}

/**
 * 
 *
 * DoTestStepL Method
 *
 */
TVerdict CTestCSGetPluginUID::DoTestStepL()
	{
	TInt err = KErrNone;
    
    // Enquire the encoder UID
    TRAP(err, EnquireYUVDecoderPluginUIDL(iFileNameSrc));
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Enquire the imageDecoder UID FAILED with error %d..."), err);
        return EFail;
        }
          
    // Enquire about the encoder UID
    TRAP(err, EnquireYUVEncoderPluginUIDL(iFileNameSrc));
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Enquire the Encoder UID FAILED with error %d..."), err);
        return EFail;
        }
    return EPass;
	}

/**
 * 
 *
 * Class Constructor
 *
 */
CTestCSGetSupportedYUVFormats::CTestCSGetSupportedYUVFormats(const TDesC& aTestName) : 
		 CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 *
 * NewL Method
 *
 */
CTestCSGetSupportedYUVFormats *CTestCSGetSupportedYUVFormats::NewL(const TDesC& aTestName)
	{
	CTestCSGetSupportedYUVFormats* self = new(ELeave) CTestCSGetSupportedYUVFormats(aTestName);
	return self;
	}

/**
 * 
 *
 * DoTestStepL Method
 *
 */
TVerdict CTestCSGetSupportedYUVFormats::DoTestStepL()
	{
		TInt err = KErrNone;
    
    // Enquire  the list of YUV Formats supported for encoding
    err = ListYUVEncoderSupportedFormats();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Enquire the imageDecoder UID FAILED with error %d..."), err);
        return EFail;
        }
          
    // Enquire  the list of YUV Formats supported decoding
    err = ListYUVDecoderSupportedFormats();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Enquire the Encoder UID FAILED with error %d..."), err);
        return EFail;
        }
    return EPass;
	}

/**
 * 
 *
 * Class Constructor
 *
 */
 CTestCSYUVEncoderNegative:: CTestCSYUVEncoderNegative(const TDesC& aTestName) : 
		  CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 *
 * NewL Method
 *
 */
 CTestCSYUVEncoderNegative * CTestCSYUVEncoderNegative::NewL(const TDesC& aTestName)
	{
	 CTestCSYUVEncoderNegative* self = new(ELeave)  CTestCSYUVEncoderNegative(aTestName);
	return self;
	}

/**
 * 
 *
 * DoTestStepL Method
 *
 */
TVerdict  CTestCSYUVEncoderNegative::DoTestStepL()
	{
	    
    TInt err = KErrNone;
    
    if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1112-CP")) == 0)
	    {     
	    //Encode an YUV to supported Image but unsuppored Frame format
	    TRAP(err, YUVEncodeUnsupportedFrameL(iFileNameSrc));
	    if (err != KErrNotSupported)
	        {
	        ERR_PRINTF2(_L("Encode an YUV to unsuppored image format - Error: %d..."), err);
	        return EFail;
	        }
	    }
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1113-CP")) == 0)
	    {        
	    //Encode with valid YUV file but corrupt RChunk
	    TRAP(err, YUVEncodeCorruptRChunkL(iFileNameSrc));
	    if (err != KErrNotSupported)
	        {
	        ERR_PRINTF2(_L("Encode an YUV to unsuppored image format - Error: %d..."), err);
	        return EFail;
	        }
        }  
	else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1114-CP")) == 0)
	    {        
	    //Encode with valid YUV file but corrupt Descriptor
	    TRAP(err, YUVEncodeCorruptDescriptorL(iFileNameSrc));
	    if (err != KErrNotSupported)
	        {
	        ERR_PRINTF2(_L("Encode an YUV to unsuppored image format - Error: %d..."), err);
	        return EFail;
	        }
		}
	return EPass;
	}

/**
 * 
 *
 * Class Constructor
 *
 */
CTestCSYUVDecoderNegative::CTestCSYUVDecoderNegative(const TDesC& aTestName) : 
		 CTestStepYUVConversion(aTestName)
	{
	
	}

/**
 * 
 *
 * NewL Method
 *
 */
CTestCSYUVDecoderNegative *CTestCSYUVDecoderNegative::NewL(const TDesC& aTestName)
	{
	CTestCSYUVDecoderNegative* self = new(ELeave) CTestCSYUVDecoderNegative(aTestName);
	return self;
	}

/**
 * 
 *
 * DoTestStepL Method
 *
 */
TVerdict CTestCSYUVDecoderNegative::DoTestStepL()
	{
    	TInt err = KErrNone;
    
	    if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1117-CP")) == 0)
	    {
	    //Decode from JPEG to unsupported image.
	    TRAP(err, JPEGDecodeUnsupportedYUVL(iFileNameSrc));
	    if (err != KErrNotSupported)
	        {
	        INFO_PRINTF2(_L("Decode from JPEG to unsupported image - Error: %d..."), err);
	        return EFail;
	        }
	    }
	    else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1118-CP")) == 0)
	    {
	    //Decode from corrupt JPEG to YUV.
	    TRAP(err, CorruptJPEGDecodeYUVL(iFileNameSrc));
	    if (err != KErrNotFound)
	        {
	        INFO_PRINTF2(_L("Decode from corrupt JPEG to YUV - Error: %d..."), err);
	        return EFail;
	        }
	    }
	    else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1119-CP")) == 0)
	    {
	    //Decode from corrupt JPEG descriptor to YUV
	    TRAP(err, CorruptJPEGDecodeDescYUVL(iFileNameSrc));
	    if (err != KErrNotFound)
	        {
	        INFO_PRINTF2(_L("Decode from corrupt JPEG descriptor to YUV - Error: %d..."), err);
	        return EFail;
	        }  
	     }
	    else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-1120-CP")) == 0)
	    {
	    //Decode from GIF to YUV
	    TRAP(err, GIFDecodeYUVL(iFileNameSrc));
	    if (err != KErrNotFound)
	        {
	        INFO_PRINTF2(_L("Decode from GIF to YUV - Error: %d..."), err);
	        return EFail;
	        } 
	    }
	return EPass;            
	}
	

