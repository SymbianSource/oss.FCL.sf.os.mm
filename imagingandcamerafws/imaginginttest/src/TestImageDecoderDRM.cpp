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
// Integration test.
// 
//

#include <testframework.h>

#include "TestImageDecoderDRM.h"

const TInt KMimeMaxLength = 256;

_LIT(KJpegRefFile, "jfif444.mbm"); // Reference filename
_LIT(KDefaultRightsUniqueId, "ICLTestjfif444Id001"); // Unique Id of the content

// Positive Test

/**
*
* Ctor for CTestImageDecDrmEnableAgentUI
*
* @param const TDesC& aTestName: Name of this test step used by the script file.
*
* @return CTestImageDecDrmEnableAgentUI*: 
*                           Constructed CTestImageDecDrmEnableAgentUI object
*
*/
CTestImageDecDrmEnableAgentUI* CTestImageDecDrmEnableAgentUI::NewL(const TDesC& aTestName)
    {
    return new (ELeave) CTestImageDecDrmEnableAgentUI(aTestName);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param const TDesC& aTestName: Name of this test step.
*
*/
CTestImageDecDrmEnableAgentUI::CTestImageDecDrmEnableAgentUI(const TDesC& aTestName): 
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
TVerdict CTestImageDecDrmEnableAgentUI::DoTestStepL()
    {
    TVerdict result = EPass;
    if (iTestStepName.Compare(_L("MM-ICL-FRM-I-0761-HP")) == 0)
        {
        result = TestStep_I_0761L();
        }

    return result;
    }

/**
*
* Test Step.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict CTestImageDecDrmEnableAgentUI::TestStep_I_0761L()
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
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileNameSrc, KDefaultRightsUniqueId, EView));

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
* Opens the image for 'the intent' and decodes.
*
* @param const TDesC& aFileName: Name of the data file.
* @param const TDesC& aUniqueId: Unique Id of the content file.
* @param ContentAccess::TIntent aIntent: Intent for opening the content file.
*
* @leave err: Leaves with 'err'.
*
*/
void CTestImageDecDrmEnableAgentUI::OpenImageAndDecodeFromFileSourceL(
    const TDesC& aFileName, 
    const TDesC& aUniqueId, 
    ContentAccess::TIntent aIntent)
    {
    // Get the path
    TFileName theSourceFilename;
    Directory(EInput, theSourceFilename);
    theSourceFilename.Append(aFileName);

    User::LeaveIfError(iFs.Connect());

    // Get the MIME type from ini file
    TBuf8<KMimeMaxLength> mimeType;
    ReadMimeTypeFromConfig(mimeType);

    INFO_PRINTF4(_L("Opening the file %S for intent %d, having Unique Id %S."), &theSourceFilename, aIntent, &aUniqueId);
    TMMFileSource fileSource(theSourceFilename, aUniqueId, aIntent);

    // Open the image and create an image decoder
    TInt err = KErrNone;
    CImageDecoder* imageDecoder = NULL;
    TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFs, fileSource, mimeType));

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

    INFO_PRINTF4(_L("The file %S for intent %d, having Unique Id %S had been opened."), &theSourceFilename, aIntent, &aUniqueId);

    INFO_PRINTF1(_L("Enabling Agent's user interface for errors and confirmation requests."));
    err = imageDecoder->SetAgentProperty(EAgentPropertyAgentUI, 1);

    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("SetAgentProperty(EAgentPropertyAgentUI, 1) failed with %d"), err);
        INFO_PRINTF1(_L("\tFailure is expected, as RTA does not support this feature."));
        INFO_PRINTF1(_L("\tContinue test."));
        }

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);

    const TFrameInfo* theFrameInfo = &imageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        theFrameInfo->iOverallSizeInPixels,
        theFrameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    imageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err == KErrNone)
        {
        INFO_PRINTF4(_L("Conversion of file %S for intent %d, having Unique Id %S was successfull!"), &theSourceFilename, aIntent, &aUniqueId);
        }
    else
        {
        INFO_PRINTF2(_L("Failed during conversion of the file: %S"), &theSourceFilename);

        // imageDecoder, activeListener, destBitmap
        CleanupStack::PopAndDestroy(3, imageDecoder);
        User::Leave(err);
        }

    // Get the reference filename
    TFileName refFilename;
    TBuf<KLenBufferExtension> extension;
    Directory(EReference, refFilename);
    refFilename.Append(KJpegRefFile);

    if (err == KErrNone)
        {
        // Check the bitmaps
        INFO_PRINTF2(_L("Comparison of the decoded file having Unique Id: %S."), &aUniqueId);
        err = CheckBitmapL(*destBitmap, refFilename);
        }

    // imageDecoder, activeListener, destBitmap
    CleanupStack::PopAndDestroy(3, imageDecoder);
    User::Leave(err);
    }
