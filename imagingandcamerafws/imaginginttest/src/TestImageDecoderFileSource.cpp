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
// Integration tests.
// 
//

#include <testframework.h>

#include "TestImageDecoderFileSource.h"

// Positive Tests

/**
*
* Ctor for CTestImageDecFileSource
*
* @param The name of this test step used by the script file.
*
* @return CTestImageDecFileSource*: 
*                           Constructed CTestImageDecFileSource object
*
*/
CTestImageDecFileSource *CTestImageDecFileSource::NewL(const TDesC& aTestName)
    {
    return new (ELeave) CTestImageDecFileSource(aTestName);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param The name of this test step.
*
*/
CTestImageDecFileSource::CTestImageDecFileSource(const TDesC& aTestName) : 
    CTestStepConversion(aTestName)
    {
    iHeapSize = 500000;
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecFileSource::DoTestStepL()
    {
    TVerdict result = EPass;
    if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-0800-HP")) == 0) || // TMMFileSource
        (iTestStepName.Compare(_L("MM-ICL-FRM-I-0801-HP")) == 0) )  // TMMFileSource, mimeType
        {
        result = TestStep_I_0800_0801L();
        }

    return result;
    }

/**
*
* Read and decode an image, a JPEG, from a file.
*
* @return   TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecFileSource::TestStep_I_0800_0801L()
    {
    // Dummy Hal call for heap balance on target
    if (DummyHalCall() != KErrNone)
        {
        return EInconclusive;
        }

    __MM_HEAP_MARK;

    TInt err = KErrNone;
    err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

    // Open and decode the image
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileNameSrc));

    RFbsSession::Disconnect();
    
    __MM_HEAP_MARKEND; 

    if (err == KErrNone)
        {
        return EPass;
        }
    else
        {
        INFO_PRINTF2(_L("Error in opening/decoding the image : %d"), err);
        return EFail;
        }
    }

/**
*
* Read and decode an image, a JPEG, from a file passing FileSource.
*
* @param filename of the image
*
* @leave err: Leaves with 'err'.
*
*/
void CTestImageDecFileSource::OpenImageAndDecodeFromFileSourceL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    // Create TMMFileSource
    TMMFileSource fileSource(theSourceFilename);

    // Open the image and create an image decoder
    TInt err = KErrNone;
    if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0800-HP")) == 0)
        {
        INFO_PRINTF2(_L("Opening the file by passing FileSource : %S"), &theSourceFilename);
        TRAP(err, iImageDecoder = CImageDecoder::FileNewL(iFs, fileSource));
        }
    else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0801-HP")) == 0)
        {
        INFO_PRINTF2(_L("Opening the file by passing FileSource and MIME type : %S"), &theSourceFilename);
        TBuf8<KMaxMimeLength> mimeType;
        ReadMimeTypeFromConfig(mimeType); // MIME type
        TRAP(err, iImageDecoder = CImageDecoder::FileNewL(iFs, fileSource, mimeType));
        }

    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        delete iImageDecoder;
        iImageDecoder = NULL;

        User::Leave(err);
        }

    if (iImageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }

    INFO_PRINTF2(_L("The file: %S had been opened."), &theSourceFilename);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    
    const TFrameInfo* theFrameInfo = &iImageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        theFrameInfo->iOverallSizeInPixels,
        theFrameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    iImageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener -> iStatus.Int();
    if (err == KErrNone)
        {
        INFO_PRINTF2(_L("Conversion of file %S was successfull!"), &theSourceFilename);
        }
    else
        {
        INFO_PRINTF2(_L("Failed during conversion of the file: %S"), &theSourceFilename);
        delete iImageDecoder;
        iImageDecoder = NULL;

        // activeListener, destBitmap
        CleanupStack::PopAndDestroy(2, activeListener);

        User::Leave(err);
        }

    TFileName theReferenceFilename;
    TBuf<KLenBufferExtension> theExtension;

    Directory(EReference, theReferenceFilename);
    TInt theSeparatorPos = aFileName.LocateReverse('.') + 1;
    theReferenceFilename.Append(aFileName.Left(theSeparatorPos));
    GetImageExtension(KImageTypeMBMUid, theExtension);
    theReferenceFilename.Append(theExtension);

    if (err == KErrNone)
        {
        // Check the bitmaps
        err = CheckBitmapL(*destBitmap, theReferenceFilename);
        }

    delete iImageDecoder;
    iImageDecoder = NULL;

    // activeListener, destBitmap
    CleanupStack::PopAndDestroy(2, activeListener);

    User::Leave(err);
    }



/**
*
* Ctor for CTestImageDecFileHandleSource
*
* @param The name of this test step used by the script file.
*
* @return CTestImageDecFileHandleSource*: 
*                           Constructed CTestImageDecFileHandleSource object
*
*/
CTestImageDecFileHandleSource *CTestImageDecFileHandleSource::NewL(const TDesC& aTestName)
    {
    return new (ELeave) CTestImageDecFileHandleSource(aTestName);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param The name of this test step.
*
*/
CTestImageDecFileHandleSource::CTestImageDecFileHandleSource(const TDesC& aTestName) : 
    CTestStepConversion(aTestName)
    {
    iHeapSize = 500000;
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecFileHandleSource::DoTestStepL()
    {
    TVerdict result = EPass;
    if ((iTestStepName.Compare(_L("MM-ICL-FRM-I-0802-HP")) == 0) || // TMMFileHandleSource
        (iTestStepName.Compare(_L("MM-ICL-FRM-I-0803-HP")) == 0))   // TMMFileHandleSource, mimeType
        {
        result = TestStep_I_0802_0803L();
        }

    return result;
    }

/**
*
* Read and decode an image, a JPEG, from a file passing FileHandleSource.
*
* @return   TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecFileHandleSource::TestStep_I_0802_0803L()
    {
    // Dummy Hal call for heap balance on target
    if (DummyHalCall() != KErrNone)
        {
        return EInconclusive;
        }

    __MM_HEAP_MARK;

    TInt err = KErrNone;
    User::LeaveIfError(FbsStartup());
    err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

    // Open and decode the image
    TRAP(err, OpenImageAndDecodeFromFileHandleSourceL(iFileNameSrc));

    RFbsSession::Disconnect();
    
    __MM_HEAP_MARKEND; 

    if (err == KErrNone)
        {
        return EPass;
        }
    else
        {
        INFO_PRINTF2(_L("Error in opening/decoding the image : %d"), err);
        return EFail;
        }
    }

/**
*
* Read and decode an image, a JPEG, from a file passing FileHandleSource.
*
* @param filename of the image
*
* @leave err: Leaves with 'err'.
*
*/
void CTestImageDecFileHandleSource::OpenImageAndDecodeFromFileHandleSourceL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    INFO_PRINTF2(_L("Opening the file by RFile : %S"), &theSourceFilename);
    RFile rFile;
    TInt res = rFile.Open(iFs, theSourceFilename, EFileWrite);
    User::LeaveIfError(res);

    // Create TMMFileHandleSource
    TMMFileHandleSource fileHandleSource(rFile);

    // Open the image and create an image decoder
    TInt err = KErrNone;
    if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0802-HP")) == 0)
        {
        INFO_PRINTF2(_L("Opening the file by passing FileHandleSource : %S"), &theSourceFilename);
        TRAP(err, iImageDecoder = CImageDecoder::FileNewL(iFs, fileHandleSource));
        }
    else if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0803-HP")) == 0)
        {
        INFO_PRINTF2(_L("Opening the file by passing FileHandleSource and MIME type : %S"), &theSourceFilename);
        TBuf8<KMaxMimeLength> mimeType;
        ReadMimeTypeFromConfig(mimeType); // MIME type
        TRAP(err, iImageDecoder = CImageDecoder::FileNewL(iFs, fileHandleSource, mimeType));
        }

    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        delete iImageDecoder;
        iImageDecoder = NULL;

        User::Leave(err);
        }

    if (iImageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }

    INFO_PRINTF2(_L("The file: %S had been opened."), &theSourceFilename);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);

    const TFrameInfo* theFrameInfo = &iImageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        theFrameInfo->iOverallSizeInPixels,
        theFrameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    iImageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener -> iStatus.Int();
    if (err == KErrNone)
        {
        INFO_PRINTF2(_L("Conversion of file %S was successfull!"), &theSourceFilename);
        }
    else
        {
        INFO_PRINTF2(_L("Failed during conversion of the file: %S"), &theSourceFilename);
        delete iImageDecoder;
        iImageDecoder = NULL;

        // activeListener, destBitmap
        CleanupStack::PopAndDestroy(2, activeListener);

        User::Leave(err);
        }

    TFileName theReferenceFilename;
    TBuf<KLenBufferExtension> theExtension;

    Directory(EReference, theReferenceFilename);
    TInt theSeparatorPos = aFileName.LocateReverse('.') + 1;
    theReferenceFilename.Append(aFileName.Left(theSeparatorPos));
    GetImageExtension(KImageTypeMBMUid, theExtension);
    theReferenceFilename.Append(theExtension);

    if (err == KErrNone)
        {
        // Check the bitmaps
        err = CheckBitmapL(*destBitmap, theReferenceFilename);
        }

    delete iImageDecoder;
    iImageDecoder = NULL;

    // activeListener, destBitmap
    CleanupStack::PopAndDestroy(2, activeListener);

    User::Leave(err);
    }

// Test to check for bitmap created using CreateExtendedBitmap

CTestImageDecFileSourceExtendedBitmap *CTestImageDecFileSourceExtendedBitmap::NewL(const TDesC& aTestName)
    {
    return new (ELeave) CTestImageDecFileSourceExtendedBitmap(aTestName);
    }




/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param The name of this test step.
*
*/
CTestImageDecFileSourceExtendedBitmap::CTestImageDecFileSourceExtendedBitmap(const TDesC& aTestName) : 
    CTestStepConversion(aTestName)
    {
    iHeapSize = 500000;
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecFileSourceExtendedBitmap::DoTestStepL()
    {
    TVerdict result = EPass;
   
    result = TestStep_I_0804L();
        

    return result;
    }

/**
*
* Read and decode an image, a JPEG, from a file.
*
* @return   TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecFileSourceExtendedBitmap::TestStep_I_0804L()
    {
    // Dummy Hal call for heap balance on target
    if (DummyHalCall() != KErrNone)
        {
        return EInconclusive;
        }

    __MM_HEAP_MARK;

    TInt err = KErrNone;
    User::LeaveIfError(FbsStartup());
    err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

    // Open and decode the image
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileNameSrc));

    RFbsSession::Disconnect();
    
    __MM_HEAP_MARKEND; 

    if (err == KErrNone)
        {
        return EPass;
        }
    else
        {
        INFO_PRINTF2(_L("Error in opening/decoding the image : %d"), err);
        return EFail;
        }
    }


TInt CTestImageDecFileSourceExtendedBitmap::InitExtendedBitmap(TAny* /*aData*/, TInt /*aDataSize*/)
    {
    return KErrNone;
    }
/**
*
* Read and decode an image, a JPEG, from a file passing FileSource.
*
* @param filename of the image
*
* @leave err: Leaves with 'err'.
*
*/
void CTestImageDecFileSourceExtendedBitmap::OpenImageAndDecodeFromFileSourceL(const TDesC& aFileName)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    // Create TMMFileSource
    TMMFileSource fileSource(theSourceFilename);

    // Open the image and create an image decoder
    TInt err = KErrNone;
    
       
    INFO_PRINTF2(_L("Opening the file by passing FileSource : %S"), &theSourceFilename);
    TRAP(err, iImageDecoder = CImageDecoder::FileNewL(iFs, fileSource));
           

    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &theSourceFilename, err);
        delete iImageDecoder;
        iImageDecoder = NULL;

        User::Leave(err);
        }

    if (iImageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }

    INFO_PRINTF2(_L("The file: %S had been opened."), &theSourceFilename);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    
    const TFrameInfo* theFrameInfo = &iImageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    
    CleanupStack::PushL(destBitmap);

        
    User::LeaveIfError(destBitmap->CreateExtendedBitmap(
            theFrameInfo->iOverallSizeInPixels,
            theFrameInfo->iFrameDisplayMode,KImageTypeJPGUid,29143,*this));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    iImageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener -> iStatus.Int();
    if (err == KErrNone)
        {
        INFO_PRINTF1(_L("Expected KErrNotSupported by now. Convert did not return any error."));
        delete iImageDecoder;
        iImageDecoder = NULL;

        // activeListener, destBitmap
        CleanupStack::PopAndDestroy(2, activeListener);
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
            delete iImageDecoder;
            iImageDecoder = NULL;

            // activeListener, destBitmap
            CleanupStack::PopAndDestroy(2, activeListener);
            INFO_PRINTF1(_L("Unexpected error"));
            User::Leave(err);
            }
        }

   

    delete iImageDecoder;
    iImageDecoder = NULL;

    // activeListener, destBitmap
    CleanupStack::PopAndDestroy(2, activeListener);

    }

