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
// DRM Unit tests.
// 
//

#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "ICLFrameworkTestSuite.h"
#include "ImageDecoderDRM.h"
#include "ImageConversion.h"
#include <iclexifimageframe.h>

// Content file which contains images
_LIT(KImageFile, "Image.content");
_LIT(KImageFileNoRights, "ImageContentNoRights.content");

// Reference files for comparison after decoding
_LIT(KJpegRefFile, "jfif444.mbm");
_LIT(KGifRefFile, "Trans_8.mbm");
_LIT(KPngRefFile, "png4_8.mbm");

// Mime Type
_LIT8(KJpegMimeType, "image/jpeg");
_LIT8(KGifMimeType, "image/gif");
_LIT8(KPngMimeType, "image/png");

// Unique Ids of the content file
_LIT(KRightsUniqueId001, "ICLTestjfif444Id001"); // Rights count 5
_LIT(KRightsUniqueId002, "\\Container001\\ICLTesttrans_8Id002"); // Rights count 6
_LIT(KRightsUniqueId003, "ICLTestpng4_8Id003");  // Rights count 3, Expiry 31-01-2005
_LIT(KRightsUniqueId004, "ICLTestjfif444Id004"); // Rights count 3, Expiry 31-01-2005
_LIT(KRightsUniqueIdDef088101, "ICLTestjfif444Id005"); // Rights count 3, Expiry 31-01-2005

_LIT(KRightsUniqueId005, "ICLTestNoRightsId001");
_LIT(KInvalidRightsUniqueId, "ICLTestInvalidUniqueId");
_LIT(KSingleImageArchive, "SingleImage.content");
_LIT(KRightsUniqueId006, "ICLTestjfif422Id001");   // Rights count 3, Expiry 31-01-2005
_LIT(KRightsUniqueId007, "ICLTestsimple_bId001");  // Rights count 3, Expiry 31-01-2005
_LIT(KRightsUniqueId008, "ICLTestclockId001");  // Rights count 3, Expiry 31-01-2005

const TUid KImageFramePluginUid = {0x101F7C60};
const TInt KDummyImageSize = 1000;
const TInt KChunkDataOffset = 10;

/**
*
* TMMFileSource
* Opens the image for 'the intent' and decodes.
*
* @param const TDesC& aFileName: Name of the data file.
* @param const TDesC& aUniqueId: Unique Id of the content file.
* @param ContentAccess::TIntent aIntent: Intent for opening the content file.
* @param aCompareToReference: Whether or not test should include a reference compare.
*
* @leave err: Leaves with 'err'.
*           : KErrCANoPermission for "MM-ICL-FRM-U-7003-HP"
*
*/
void RTImageOpenAndDecode::OpenImageAndDecodeFromFileSourceL(
    const TDesC& aFileName, 
    const TDesC& aUniqueId, 
    ContentAccess::TIntent aIntent,
    const TBool aCompareToReference)
    {
    TMMFileSource fileSource(aFileName, aUniqueId, aIntent);

    // Open the image and create an image decoder
    TInt err = KErrNone;
    CImageDecoder* imageDecoder = NULL;
    if (iTestStepName == _L("MM-ICL-FRM-U-6005-HP"))
        {
        TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fileSource, 
            CImageDecoder::EOptionNone, KImageTypeJPGUid));
        }
    else if (iTestStepName == _L("MM-ICL-FRM-U-6006-HP"))
        {
        TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fileSource, 
            CImageDecoder::EOptionNone, KImageTypeJPGUid, KNullUid, 
            TUid::Uid(KJPGDecoderImplementationUidValue)));
        }
    else
        {
        TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fileSource, GetMimeType()));
        }

    if (iTestStepName == _L("MM-ICL-FRM-U-7003-HP"))
        {
        if (err == KErrCANoPermission)
            {
            INFO_PRINTF2(_L("Cannot open file: %S. No permission!"), &aFileName);
            User::Leave(KErrCANoPermission);
            }
        }

    if (err != KErrNone)
        {
        if (err != KErrNoMemory)
        	{
        	INFO_PRINTF3(_L("Cannot open file: %S. Error: %d"), &aFileName, err);
        	}
        User::Leave(err);
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }

    CleanupStack::PushL(imageDecoder);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);

    const TFrameInfo* frameInfo = &imageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        frameInfo->iOverallSizeInPixels,
        frameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    imageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Failed during conversion of the file: %S"), &aFileName);

        // imageDecoder, activeListener, destBitmap
        CleanupStack::PopAndDestroy(3, imageDecoder);
        User::Leave(err);
        }

	if (aCompareToReference)
		{
	    // Get the reference filename
	    TFileName refFileName = aFileName;
	    GetTestFileNameL(refFileName, GetRefFilename());

	    // Load reference bitmap
	    CFbsBitmap* refBitmap = new (ELeave) CFbsBitmap;
	    CleanupStack::PushL(refBitmap);
	    TInt refErr = refBitmap->Load(refFileName);
	    if (refErr != KErrNone)
	    	{
	    	INFO_PRINTF3(_L("Error %d opening reference file %S"), refErr, &refFileName);
	    	User::Leave(refErr);
	    	}

	    if (err == KErrNone)
	        {
	        // Check the bitmaps
	        Compare(*destBitmap, *refBitmap, _L("Bitmaps are not the same."));
	        }
	    CleanupStack::PopAndDestroy(refBitmap);
		}

    // imageDecoder, activeListener, destBitmap
    CleanupStack::PopAndDestroy(3, imageDecoder);

    User::Leave(err);
    }


/**
*
* TMMFileHandleSource
* Opens the image for 'the intent' and decodes.
*
* @param const TDesC& aFileName: Name of the data file.
* @param const TDesC& aUniqueId: Unique Id of the content file.
* @param ContentAccess::TIntent aIntent: Intent for opening the content file.
*
* @leave err: Leaves with 'err'.
*           : KErrCANoPermission for "MM-ICL-FRM-U-7006-HP"
*
*/
void RTImageOpenAndDecode::OpenImageAndDecodeFromFileHandleSourceL(
    const TDesC& aFileName, 
    const TDesC& aUniqueId, 
    ContentAccess::TIntent aIntent)
    {
    INFO_PRINTF2(_L("Opening the file by RFile : %S"), &aFileName);
    RFile rFile;
    TInt err = rFile.Open(iFileSession, aFileName, EFileRead);
    if (err != KErrNone)
    	{
    	INFO_PRINTF3(_L("Error %d opening file %S"), err, &aFileName);
    	User::Leave(err);
    	}
    CleanupClosePushL(rFile);

    TMMFileHandleSource fileHandleSource(rFile, aUniqueId, aIntent);

    // Open the image and create an image decoder
    CImageDecoder* imageDecoder = NULL;
    TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fileHandleSource));

    if (iTestStepName == _L("MM-ICL-FRM-U-7006-HP"))
        {
        if (err == KErrCANoPermission)
            {
            INFO_PRINTF2(_L("Cannot open file: %S. No permission!"), &aFileName);
            rFile.Close();
            CleanupStack::PopAndDestroy(&rFile); // rFile
            User::Leave(KErrCANoPermission);
            }
        }

    if (err != KErrNone)
        {
        if (err != KErrNoMemory)
        	{
        	INFO_PRINTF3(_L("Cannot open file: %S. Error: %d"), &aFileName, err);
        	}
        rFile.Close();
        CleanupStack::PopAndDestroy(&rFile); // rFile
        User::Leave(err);
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        rFile.Close();
        CleanupStack::PopAndDestroy(&rFile); // rFile
        User::Leave(err);
        }

    CleanupStack::PushL(imageDecoder);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);

    const TFrameInfo* frameInfo = &imageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        frameInfo->iOverallSizeInPixels,
        frameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    imageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Failed during conversion of the file: %S"), &aFileName);

        rFile.Close();

        // rFile, imageDecoder, activeListener, destBitmap
        CleanupStack::PopAndDestroy(4, &rFile);
        User::Leave(err);
        }

    // Get the reference filename
    TFileName refFileName = aFileName;
    GetTestFileNameL(refFileName, GetRefFilename());

    // Load reference bitmap
    CFbsBitmap* refBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(refBitmap);
	TInt refErr = refBitmap->Load(refFileName);
	if (refErr != KErrNone)
		{
    	INFO_PRINTF3(_L("Error %d loading reference image %S"), refErr, &refFileName);
    	User::Leave(refErr);
    	}

    if (err == KErrNone)
        {
        // Check the bitmaps
        Compare(*destBitmap, *refBitmap, _L("Bitmaps are not the same."));
        }

    rFile.Close();

    // rFile, imageDecoder, activeListener, destBitmap, refBitmap
    CleanupStack::PopAndDestroy(5, &rFile);

    User::Leave(err);
    }


/**
*
* Get the MIME type.
*
* @return const TDesC8&: MIME type
*
*/
const TDesC8& RTImageOpenAndDecode::GetMimeType()
    {
    if (iTestStepName == _L("MM-ICL-FRM-U-6002-HP") || 
        (iTestStepName == _L("MM-ICL-FRM-U-6008-HP")) ||
        (iTestStepName == _L("MM-ICL-FRM-U-7109-HP")))
        {
        const TDesC8& mimeType(KGifMimeType);
        return mimeType;
        }

    if (iTestStepName == _L("MM-ICL-FRM-U-7003-HP"))
        {
        const TDesC8& mimeType(KPngMimeType);
        return mimeType;
        }

    const TDesC8& mimeType(KJpegMimeType);
    return mimeType;
    }


/**
*
* Get the reference filename.
*
* @return TFileName: reference filename
*
*/
TFileName RTImageOpenAndDecode::GetRefFilename()
    {
    if (iTestStepName == _L("MM-ICL-FRM-U-6002-HP") || 
        (iTestStepName == _L("MM-ICL-FRM-U-6008-HP")))
        {
        TFileName refFilename(KGifRefFile);
        return refFilename;
        }

    if (iTestStepName == _L("MM-ICL-FRM-U-7003-HP"))
        {
        TFileName refFilename(KPngRefFile);
        return refFilename;
        }

    TFileName refFilename(KJpegRefFile);
    return refFilename;
    }


/**
*
* OpenL()
*
* @return TVerdict
*
*/
TVerdict RTImageOpenAndDecode::OpenL()
    {
    User::LeaveIfError(FbsStartup());
    TInt err = RFbsSession::Connect();
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), err);
        return EInconclusive;
        }

    // Install the ActiveScheduler
    iActiveScheduler = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(iActiveScheduler);

    User::LeaveIfError(iFileSession.Connect());
    User::LeaveIfError(iFileSession.ShareProtected());

    // Get the path
    iFileName = reinterpret_cast<CICLFrameworkTestSuite*>(iSuite)->DefaultPath();
    iFileName.Append(KImageFile);

    return iTestStepResult;
    }

/**
*
* Close()
*
*/
void RTImageOpenAndDecode::Close()
    {
    iFileSession.Close();
    RFbsSession::Disconnect();

    delete iActiveScheduler;
    iActiveScheduler = NULL;
    }


// Positive Tests

/**
*
* Ctor for RTImageDecDRMViewMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewMthd1*: constructed RTImageDecDRMViewMthd1 object
*
*/
RTImageDecDRMViewMthd1* RTImageDecDRMViewMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewMthd1::RTImageDecDRMViewMthd1(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6001-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights, from a file for VIEW intent and specifying MIME type. Decode. (TMMFileSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId001, EView));

    if (err == KErrNone)
        {
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("Error %d in opening/decoding the image \"%S\""), err, &iFileName);
        return EFail;
        }
    }


/**
*
* Ctor for RTImageDecDRMPrintMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMPrintMthd1*: constructed RTImageDecDRMPrintMthd1 object
*
*/
RTImageDecDRMPrintMthd1* RTImageDecDRMPrintMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMPrintMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMPrintMthd1::RTImageDecDRMPrintMthd1(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6002-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMPrintMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected GIF image having rights, from a file for PRINT intent and specifying MIME type. Decode. (TMMFileSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId002, EPrint));

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
* Ctor for RTImageDecDRMViewAllocMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewAllocMthd1*: constructed RTImageDecDRMViewAllocMthd1 object
*
*/
RTImageDecDRMViewAllocMthd1* RTImageDecDRMViewAllocMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewAllocMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewAllocMthd1::RTImageDecDRMViewAllocMthd1(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6003-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewAllocMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Alloc test to open a protected JPEG image having rights, from a file for VIEW intent and specifying MIME type. Decode. (TMMFileSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId001, EView));

    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Error in opening/decoding the image : %d"), err);
        return EFail;
        }

    TVerdict result = EPass;

    TInt failCount = 1;
    TBool completed = EFalse;
    TBool reachedEnd = EFalse;
    TBool badResult = EFalse;

    for (;;)
        {
        __UHEAP_SETFAIL(RHeap::EFailNext, failCount);
        __MM_HEAP_MARK;

        // Open and decode the image
        TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId001, EView));

        completed = EFalse;
        if (err == KErrNone)
            {
            TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
            if (testAlloc == NULL)
                {
                reachedEnd = ETrue;
                failCount--;
                }
            else
                {
                User::Free(testAlloc);
                testAlloc = NULL;
                }

            if (result != EPass)
                {
                badResult = ETrue;
                }

            completed = reachedEnd || badResult;
            }
        else if (err != KErrNoMemory) // bad error code
            {
            __UHEAP_SETFAIL(RHeap::ENone, 0);
            ERR_PRINTF2(_L("Bad error code %d"), err);
            completed = ETrue;
            result = EFail;
            }

        __MM_HEAP_MARKEND;
        __UHEAP_SETFAIL(RHeap::ENone, 0);

        if (completed)
            {
            break; // exit loop
            }

        failCount++;
        }

	if (err != KErrNone || badResult)
		{
		if (badResult)
			{
			INFO_PRINTF3(_L("Bad result with %d memory allocations tested. The Error returned is %d."), failCount, err);
			}
		else
			{
			INFO_PRINTF3(_L("Error(%d) with %d memory allocations tested.\n"), err, failCount);
			}

		result = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Completed OK with %d memory allocations tested.\n"), failCount);
		result = EPass;
		}

    return result;
    }


/**
*
* Ctor for RTImageDecDRMViewMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewMthd2*: constructed RTImageDecDRMViewMthd2 object
*
*/
RTImageDecDRMViewMthd2* RTImageDecDRMViewMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewMthd2::RTImageDecDRMViewMthd2(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6004-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights, from a file for VIEW intent. Decode. (All defaults. TMMFileHandleSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileHandleSourceL(iFileName, KRightsUniqueId001, EView));

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
* Ctor for RTImageDecDRMViewImgTypeMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewImgTypeMthd2*: constructed RTImageDecDRMViewImgTypeMthd2 object
*
*/
RTImageDecDRMViewImgTypeMthd2* RTImageDecDRMViewImgTypeMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewImgTypeMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewImgTypeMthd2::RTImageDecDRMViewImgTypeMthd2(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6005-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewImgTypeMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights, from a file for VIEW intent and specifying image type Uid. Decode. (TMMFileSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId001, EView));

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
* Ctor for RTImageDecDRMViewDecTypeMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewDecTypeMthd2*: constructed RTImageDecDRMViewDecTypeMthd2 object
*
*/
RTImageDecDRMViewDecTypeMthd2* RTImageDecDRMViewDecTypeMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewDecTypeMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewDecTypeMthd2::RTImageDecDRMViewDecTypeMthd2(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6006-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewDecTypeMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights, from a file for VIEW intent and specifying decoder Uid. Decode. (TMMFileSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId001, EView));

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
* Ctor for RTImageDecDRMViewAllocMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewAllocMthd2*: constructed RTImageDecDRMViewAllocMthd2 object
*
*/
RTImageDecDRMViewAllocMthd2* RTImageDecDRMViewAllocMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewAllocMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewAllocMthd2::RTImageDecDRMViewAllocMthd2(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6007-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewAllocMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Alloc test to open a protected JPEG image having rights, from a file for VIEW intent. Decode. (All defaults. TMMFileHandleSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileHandleSourceL(iFileName, KRightsUniqueId001, EView));

    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Error in opening/decoding the image : %d"), err);
        return EFail;
        }

    TVerdict result = EPass;

    TInt failCount = 1;
    TBool completed = EFalse;
    TBool reachedEnd = EFalse;
    TBool badResult = EFalse;

    for (;;)
        {
        __UHEAP_SETFAIL(RHeap::EFailNext, failCount);
        __MM_HEAP_MARK;

        // Open and decode the image
        TRAP(err, OpenImageAndDecodeFromFileHandleSourceL(iFileName, KRightsUniqueId001, EView));

        completed = EFalse;
        if (err == KErrNone)
            {
            TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
            if (testAlloc == NULL)
                {
                reachedEnd = ETrue;
                failCount--;
                }
            else
                {
                User::Free(testAlloc);
                testAlloc = NULL;
                }

            if (result != EPass)
                {
                badResult = ETrue;
                }

            completed = reachedEnd || badResult;
            }
        else if (err != KErrNoMemory) // bad error code
            {
            __UHEAP_SETFAIL(RHeap::ENone, 0);
            ERR_PRINTF2(_L("Bad error code %d"), err);
            completed = ETrue;
            result = EFail;
            }

        __MM_HEAP_MARKEND;
        __UHEAP_SETFAIL(RHeap::ENone, 0);

        if (completed)
            {
            break; // exit loop
            }

        failCount++;
        }

	if (err != KErrNone || badResult)
		{
		if (badResult)
			{
			INFO_PRINTF3(_L("Bad result with %d memory allocations tested. The Error returned is %d."), failCount, err);
			}
		else
			{
			INFO_PRINTF3(_L("Error(%d) with %d memory allocations tested.\n"), err, failCount);
			}

		result = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Completed OK with %d memory allocations tested.\n"), failCount);
		result = EPass;
		}

    return result;
    }


/**
*
* Ctor for RTImageDecDRMViewUniqueIdMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewUniqueIdMthd1*: constructed RTImageDecDRMViewUniqueIdMthd1 object
*
*/
RTImageDecDRMViewUniqueIdMthd1* RTImageDecDRMViewUniqueIdMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewUniqueIdMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewUniqueIdMthd1::RTImageDecDRMViewUniqueIdMthd1(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-6008-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewUniqueIdMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected GIF image having rights, from a file for PRINT intent and specifying MIME type with Unique Id. Decode. (TMMFileSource)."));

    // Open and decode the image
    TInt err = KErrNone;
    TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId002, EPrint));

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



// Negative Tests

/**
*
* Ctor for RTImageDecOpenDRMNoRightsMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMNoRightsMthd1*: constructed RTImageDecOpenDRMNoRightsMthd1 object
*
*/
RTImageDecOpenDRMNoRightsMthd1* RTImageDecOpenDRMNoRightsMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMNoRightsMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMNoRightsMthd1::RTImageDecOpenDRMNoRightsMthd1(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7001-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMNoRightsMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image (by a user having NO rights to VIEW), from a file for VIEW intent and specifying MIME type. (TMMFileSource)."));

    // Get the path
    iFileName = KNullDesC;
    iFileName = reinterpret_cast<CICLFrameworkTestSuite*>(iSuite)->DefaultPath();
    iFileName.Append(KImageFileNoRights);

    INFO_PRINTF2(_L("Opening the file for VIEW intent: %S"), &iFileName);
    TMMFileSource fileSource(iFileName, KRightsUniqueId005, EView);

    // Open the image
    TInt err = KErrNone;
    TRAP(err, CImageDecoder::FileNewL(iFileSession, fileSource, GetMimeType()));

    if (err == KErrCANoRights)
        {
        INFO_PRINTF2(_L("Cannot open file: %S. No Rights for the user to view the image!"), &iFileName);
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &iFileName, err);
        return EFail;
        }
    }


/**
*
* Ctor for RTImageDecOpenDRMUnknownMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMUnknownMthd1*: constructed RTImageDecOpenDRMUnknownMthd1 object
*
*/
RTImageDecOpenDRMUnknownMthd1* RTImageDecOpenDRMUnknownMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMUnknownMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMUnknownMthd1::RTImageDecOpenDRMUnknownMthd1(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7002-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMUnknownMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights to VIEW, from a file for UNKNOWN intent and specifying MIME type. (TMMFileSource)."));

    INFO_PRINTF2(_L("Opening the file for Unknown intent: %S"), &iFileName);
    TMMFileSource fileSource(iFileName, KRightsUniqueId001, EUnknown);

    // Open the image
    TInt err = KErrNone;
    TRAP(err, CImageDecoder::FileNewL(iFileSession, fileSource, GetMimeType()));

    if (err == KErrCANoPermission)
        {
        INFO_PRINTF2(_L("Cannot open file: %S. No Permission to open the image for Unknown intent!"), &iFileName);
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &iFileName, err);
        return EFail;
        }
    }


/**
*
* Ctor for RTImageDecOpenDRMRightsCountMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMRightsCountMthd1*: constructed RTImageDecOpenDRMRightsCountMthd1 object
*
*/
RTImageDecOpenDRMRightsCountMthd1* RTImageDecOpenDRMRightsCountMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMRightsCountMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMRightsCountMthd1::RTImageDecOpenDRMRightsCountMthd1(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7003-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMRightsCountMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected PNG image for VIEW intent with rights count set to 3. Decode three times. Open image for the fourth time. (TMMFileSource)."));

    TVerdict result = EFail;

    // Open and decode the image for three times
    // The rights count for this protected image is set to 3.
    // Hence looped four times to check failure while opening for the fourth time.
    for (TInt rightscount = 1; rightscount <= 4; rightscount++)
        {
        INFO_PRINTF2(_L("Opening the file and decoding for rights count = %d "), rightscount);

        result = EFail;

        // Open and decode the image
        TInt err = KErrNone;
        TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId003, EView));

        if ((err == KErrCANoPermission) && (rightscount == 4))
            {
            result = EPass;
            }
        else if (err != KErrNone)
            {
            INFO_PRINTF3(_L("Error in opening/decoding the image for rights count: %d, Error: %d"), rightscount, err);
            break;
            }
        }

    return result;
    }


/**
*
* Ctor for RTImageDecOpenDRMNoRightsMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMNoRightsMthd2*: constructed RTImageDecOpenDRMNoRightsMthd2 object
*
*/
RTImageDecOpenDRMNoRightsMthd2* RTImageDecOpenDRMNoRightsMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMNoRightsMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMNoRightsMthd2::RTImageDecOpenDRMNoRightsMthd2(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7004-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMNoRightsMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image (by a user having NO rights to VIEW), from a file for VIEW intent. (All defaults. TMMFileHandleSource)."));

    // Get the path
    iFileName = KNullDesC;
    iFileName = reinterpret_cast<CICLFrameworkTestSuite*>(iSuite)->DefaultPath();
    iFileName.Append(KImageFileNoRights);

    INFO_PRINTF2(_L("Opening the file by RFile : %S"), &iFileName);
    RFile rFile;
    TInt err = rFile.Open(iFileSession, iFileName, EFileRead);
    if (err != KErrNone)
    	{
    	INFO_PRINTF3(_L("Error %d opening file %S"), err, &iFileName);
    	User::Leave(err);
    	}
    CleanupClosePushL(rFile);

    INFO_PRINTF2(_L("Opening the file for VIEW intent: %S"), &iFileName);
    TMMFileHandleSource fileHandleSource(rFile, KRightsUniqueId005, EView);

    // Open the image
    TRAP(err, CImageDecoder::FileNewL(iFileSession, fileHandleSource));

    rFile.Close();
    CleanupStack::PopAndDestroy(&rFile); // rFile

    if (err == KErrCANoRights)
        {
        INFO_PRINTF2(_L("Cannot open file: %S. No Rights for the user to view the image!"), &iFileName);
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &iFileName, err);
        return EFail;
        }
    }


/**
*
* Ctor for RTImageDecOpenDRMUnknownMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMUnknownMthd2*: constructed RTImageDecOpenDRMUnknownMthd2 object
*
*/
RTImageDecOpenDRMUnknownMthd2* RTImageDecOpenDRMUnknownMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMUnknownMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMUnknownMthd2::RTImageDecOpenDRMUnknownMthd2(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7005-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMUnknownMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights to VIEW, from a file for UNKNOWN intent. (All defaults. TMMFileHandleSource)."));

    INFO_PRINTF2(_L("Opening the file by RFile : %S"), &iFileName);
    RFile rFile;
    TInt err = rFile.Open(iFileSession, iFileName, EFileRead);
    if (err != KErrNone)
    	{
    	INFO_PRINTF3(_L("Error %d opening file %S"), err, &iFileName);
    	User::Leave(err);
    	}
    CleanupClosePushL(rFile);

    INFO_PRINTF2(_L("Opening the file for Unknown intent: %S"), &iFileName);
    TMMFileHandleSource fileHandleSource(rFile, KRightsUniqueId001, EUnknown);

    // Open the image
    TRAP(err, CImageDecoder::FileNewL(iFileSession, fileHandleSource));

    rFile.Close();
    CleanupStack::PopAndDestroy(&rFile); // rFile

    if (err == KErrCANoPermission)
        {
        INFO_PRINTF2(_L("Cannot open file: %S. No Permission to open the image for Unknown intent!"), &iFileName);
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &iFileName, err);
        return EFail;
        }
    }


/**
*
* Ctor for RTImageDecOpenDRMRightsCountMthd2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMRightsCountMthd2*: constructed RTImageDecOpenDRMRightsCountMthd2 object
*
*/
RTImageDecOpenDRMRightsCountMthd2* RTImageDecOpenDRMRightsCountMthd2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMRightsCountMthd2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMRightsCountMthd2::RTImageDecOpenDRMRightsCountMthd2(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7006-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMRightsCountMthd2::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image for VIEW intent with rights count set to 3. Decode three times."));
    INFO_PRINTF1(_L("Open image for the fourth time. (All defaults. TMMFileHandleSource)."));

    TVerdict result = EFail;

    // Open and decode the image for three times
    // The rights count for this protected image is set to 3.
    // Hence looped four times to check failure while opening for the fourth time.
    for (TInt rightscount = 1; rightscount <= 4; rightscount++)
        {
        INFO_PRINTF2(_L("Opening the file and decoding for rights count = %d "), rightscount);

        result = EFail;

        // Open and decode the image
        TInt err = KErrNone;
        TRAP(err, OpenImageAndDecodeFromFileHandleSourceL(iFileName, KRightsUniqueId004, EView));

        if ((err == KErrCANoPermission) && (rightscount == 4))
            {
            result = EPass;
            }
        else if (err != KErrNone)
            {
            INFO_PRINTF3(_L("Error in opening/decoding the image for rights count: %d, Error: %d"), rightscount, err);
            break;
            }
        }

    return result;
    }


/**
*
* Ctor for RTImageDecDRMViewInvalIdMthd1
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViewInvalIdMthd1*: constructed RTImageDecDRMViewInvalIdMthd1 object
*
*/
RTImageDecDRMViewInvalIdMthd1* RTImageDecDRMViewInvalIdMthd1::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViewInvalIdMthd1(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViewInvalIdMthd1::RTImageDecDRMViewInvalIdMthd1(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7007-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViewInvalIdMthd1::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a protected JPEG image having rights, from a file for VIEW intent and specifying MIME type with INVALID Unique Id. (TMMFileSource)."));

    INFO_PRINTF2(_L("Opening the file for VIEW intent with invalid Unique Id: %S"), &iFileName);
    TMMFileSource fileSource(iFileName, KInvalidRightsUniqueId, EView);

    // Open the image
    TInt err = KErrNone;
    TRAP(err, CImageDecoder::FileNewL(iFileSession, fileSource, GetMimeType()));

    if (err == KErrNotFound)
        {
        INFO_PRINTF2(_L("Cannot open file: %S. Invalid Unique Id!"), &iFileName);
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &iFileName, err);
        return EFail;
        }
    }


/**
*
* Ctor for RTImageDecDRMViaCAF
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViaCAF*: constructed RTImageDecDRMViaCAF object
*
*/
RTImageDecDRMViaCAF* RTImageDecDRMViaCAF::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecDRMViaCAF(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecDRMViaCAF::RTImageDecDRMViaCAF(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7008-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecDRMViaCAF::DoTestStepL()
    {
    INFO_PRINTF1(_L("Open a file Via CAF "));
    iFileName = _L("z:\\Resource\\jfif444.jpg");
    TInt err = KErrNone;
    INFO_PRINTF1(_L("Open a file Via ICL with KDefaultContentObject() , ContentAccess::EView"));	
	TMMFileSource fileName(iFileName,KDefaultContentObject(), ContentAccess::EView);
	CImageDecoder* decoder = NULL;
	TRAP(err, decoder = CImageDecoder::FileNewL(iFileSession, fileName, GetMimeType()));
	delete decoder; 
	decoder = NULL;
    if (err == KErrNone)
        {
        return EPass;
        }
    else
        {
        INFO_PRINTF3(_L("*** Error %d in opening/decoding the image: %S"), err, &fileName);
        return EFail;
        }
    }

//
// INC061633: NTT - CImageDecoder: KNullDesC should be KDefaultContentObject
//
RTImageDecDRMDefaContentTest::RTImageDecDRMDefaContentTest(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7020-HP");
    }
    
RTImageDecDRMDefaContentTest* RTImageDecDRMDefaContentTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTImageDecDRMDefaContentTest(aSuite);
	}
	
TVerdict RTImageDecDRMDefaContentTest::DoTestStepL()
	{
	iFileName.Copy( reinterpret_cast<CICLFrameworkTestSuite*>(iSuite)->DefaultPath() );
    iFileName.Append(KSingleImageArchive);
    TPtrC unId(KNullDesC());
    TMMFileSource fileSrcNullId(iFileName, unId, EView);
    CImageDecoder* decoder = NULL;
    TInt error = KErrNone;
    TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, fileSrcNullId));
    delete decoder;
    if (error == KErrNone)
    	{
    	INFO_PRINTF2(_L("Was expecting error - strange CAF agent?"), &iFileName);
    	return EInconclusive;
    	}
    unId.Set(KDefaultContentObject());
    TMMFileSource fileSrcDefId(iFileName, unId, EView);

    INFO_PRINTF1(_L("Open image using the TMMFileSource..."));
    decoder=CImageDecoder::FileNewL(iFileSession, fileSrcDefId);
	delete decoder;
	
	INFO_PRINTF1(_L("Open image using the CImageDecoder::FileNewL(iFileSession, iFileName, GetMimeType()..."));
	decoder=CImageDecoder::FileNewL(iFileSession, iFileName, GetMimeType());
	delete decoder;
	
	INFO_PRINTF1(_L("Open image using the CImageDecoder::FileNewL(iFileSession, iFileName, GetMimeType(), EView, CImageDecoder::EOptionNone)..."));
	decoder=CImageDecoder::FileNewL(iFileSession, iFileName, GetMimeType(), EView, CImageDecoder::EOptionNone);
	delete decoder;

	INFO_PRINTF1(_L("Open image using the CImageDecoder::FileNewL(iFileSession, iFileName, CImageDecoder::EOptionNone, KNullUid )..."));	
	decoder=CImageDecoder::FileNewL(iFileSession, iFileName, CImageDecoder::EOptionNone, KNullUid );
	delete decoder;

	INFO_PRINTF1(_L("Open image using the CImageDecoder::FileNewL(iFileSession, iFileName, EView, CImageDecoder::EOptionNone, KNullUid)..."));
	decoder=CImageDecoder::FileNewL(iFileSession, iFileName, EView, CImageDecoder::EOptionNone, KNullUid);
	delete decoder;
	
	RFile imgFile;
	if (KErrNone != imgFile.Open(iFileSession, iFileName, EFileRead|EFileShareAny))
		{
		TInt err = imgFile.Open(iFileSession, iFileName, EFileRead|EFileShareReadersOnly);
		if (err != KErrNone)
			{
			INFO_PRINTF3(_L("Error %d opening file %S"), err, &iFileName);
			User::Leave(err);
			}
		}
	CleanupClosePushL(imgFile);
	
	INFO_PRINTF1(_L("Open image using the CImageDecoder::FileNewL(imgFile, GetMimeType(), EView, CImageDecoder::EOptionNone)..."));
	decoder=CImageDecoder::FileNewL(imgFile, GetMimeType(), EView, CImageDecoder::EOptionNone);
	delete decoder;	
	
	INFO_PRINTF1(_L("Open image using the CImageDecoder::FileNewL(imgFile, EView, CImageDecoder::EOptionNone, KNullUid)..."));
	decoder=CImageDecoder::FileNewL(imgFile, EView, CImageDecoder::EOptionNone, KNullUid);
	delete decoder;		
	
	CleanupStack::PopAndDestroy();// imgFile
	
	return EPass;
	}

/**
*
* DEF088101: ICL CAF result of ExecuteIntent not checked for conversion
*
*/
RTImageDecOpenDRMConsumeRights* RTImageDecOpenDRMConsumeRights::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMConsumeRights(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMConsumeRights::RTImageDecOpenDRMConsumeRights(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7106-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMConsumeRights::DoTestStepL()
    {
    INFO_PRINTF1(_L("DEF088101: ICL CAF result of ExecuteIntent not checked for conversion"));

    TVerdict result = EFail;
    
    TIntent theIntent = EView;

    // Open the image locally (i.e. without using CImageDecoder)
    // Then open it again using CImageDecoder
    // Consume the rights : execute the intent 3 times on the local object
    // Then call Convert(); it should now fail with no rights.

	CContent* localContent = NULL;
	TRAPD(err, localContent = CContent::NewL(iFileName, EContentShareReadWrite));
	if(err != KErrNone || localContent == NULL)
		{
        INFO_PRINTF1(_L("Error : cannot open content file locally "));
		result = EInconclusive;
		return result;
		}

	// Open the image in CImageDecoder
    INFO_PRINTF2(_L("Opening the file by RFile : %S"), &iFileName);
    RFile rFile;
    err = rFile.Open(iFileSession, iFileName, EFileShareAny);
    if (err != KErrNone)
    	{
    	INFO_PRINTF3(_L("Error %d opening file %S"), err, &iFileName);
    	User::Leave(err);
    	}
    CleanupClosePushL(rFile);

    INFO_PRINTF4(_L("Opening the file %S for intent %d, having Unique Id %S."), &iFileName, theIntent, &KRightsUniqueIdDef088101);
    TMMFileHandleSource fileHandleSource(rFile, KRightsUniqueIdDef088101, theIntent);

    // Open the image and create an image decoder
    CImageDecoder* imageDecoder = NULL;
    TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fileHandleSource));

    if (err == KErrCANoPermission)
	    {
        INFO_PRINTF2(_L("Cannot open file: %S. No permission!"), &iFileName);
        rFile.Close();
        CleanupStack::PopAndDestroy(&rFile); // rFile
        return EInconclusive;
        }

    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"), &iFileName, err);
        rFile.Close();
        CleanupStack::PopAndDestroy(&rFile); // rFile
        return EInconclusive;
        }

    if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        rFile.Close();
        CleanupStack::PopAndDestroy(&rFile); // rFile
        return EInconclusive;
        }

    CleanupStack::PushL(imageDecoder);

    INFO_PRINTF4(_L("The file %S for intent %d, having Unique Id %S had been opened."), &iFileName, theIntent, &KRightsUniqueIdDef088101);

	// Now consume all the rights on the local object
	CData* localData = NULL;
	TRAP(err, localData = localContent->OpenContentL(theIntent, KRightsUniqueIdDef088101));
	if(err != KErrNone) 
		{
        INFO_PRINTF2(_L("Couldn't open local content, error %d"), err);
        rFile.Close();
        CleanupStack::PopAndDestroy(imageDecoder);
        CleanupStack::PopAndDestroy(&rFile); // rFile
        return EInconclusive;
		
		}
	do
		{
		INFO_PRINTF1(_L("Consuming rights..."));
		err = localData->ExecuteIntent(theIntent);	
		}
	while(err == KErrNone); 
	INFO_PRINTF1(_L("Rights exhausted"));
	delete localData;
	
	// Now attempt to decode the decoder object
	INFO_PRINTF1(_L("Attempting to decode the content..."));

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);

    const TFrameInfo* frameInfo = &imageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        frameInfo->iOverallSizeInPixels,
        frameInfo->iFrameDisplayMode));

    activeListener->InitialiseActiveListener();

    // Decode the image
    imageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    // we should receive an error here
    if (err != KErrNone)
        {
        INFO_PRINTF2(_L("Conversion (1) failed as expected, error %d"), err);        

		// now try the overloaded Convert (with destination mask) - this should fail too since
		// there are still insufficient rights
		
		CFbsBitmap* testBitmapMask = new (ELeave) CFbsBitmap;
		User::LeaveIfError( testBitmapMask->Create(frameInfo->iOverallSizeInPixels, EGray2) );
		
		activeListener->InitialiseActiveListener();
		
		imageDecoder->Convert(&activeListener->iStatus, *destBitmap, *testBitmapMask, 0);
		CActiveScheduler::Start();

		err = activeListener->iStatus.Int();

		if (err != KErrNone)
	    	{
	    	INFO_PRINTF2(_L("Conversion (2) failed as expected, error %d"), err);
	    	result = EPass;
	    	}
	    else
	    	{
	    	INFO_PRINTF1(_L("Error - conversion (2) succeeded"));	    	
	    	}
        }
    else
        {
        INFO_PRINTF1(_L("Error - conversion (1) succeeded"));
        }

    rFile.Close();

    // rFile, imageDecoder, activeListener, destBitmap
    CleanupStack::PopAndDestroy(4, &rFile);

    return result;
    }

/**
*
* Ctor for RTImageDecOpenDRMConsumeRightsOnError
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMConsumeRightsOnError*: constructed RTImageDecOpenDRMConsumeRightsOnError object
*
*/
RTImageDecOpenDRMConsumeRightsOnError* RTImageDecOpenDRMConsumeRightsOnError::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMConsumeRightsOnError(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMConsumeRightsOnError::RTImageDecOpenDRMConsumeRightsOnError(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7107-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMConsumeRightsOnError::DoTestStepL()
	{
    INFO_PRINTF1(_L("DEF090885: Rights can be cosumed prior to successful decoding of an image"));
	INFO_PRINTF1(_L("Open a protected MNG image for VIEW intent with rights count set to 3. Verify that a decoding failure does not consume rights."));

	TVerdict result = EFail;
	TInt err = KErrNone;
	
	// Open and decode the mng image to make sure we can
	TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId007, EView, EFalse));
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Error in opening/decoding the image, Error: %d"), err);
		return result;
		}
		
	//We have now used up 1 right to view.  We should have 2 left.
	//Cause the next attempt to fail due to no memory. This should not consume a right to view.
	TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId007, EView, ETrue));
	if (err == KErrUnknown)
		{
		INFO_PRINTF1(_L("Simulating no memory did not cause decoder to fail.(Possibly test is run in UREL mode.This test is required to be run only in UDEB mode.), Test is inconclusive"));
		return EInconclusive;		
		}
	else if(err != KErrNone)
		{
		INFO_PRINTF2(_L("Error in opening/decoding the image while simulating no memory, Error: %d"), err);
		return result;
		}	

	//If the defect has been fixed, we should still have two views left.
	//Try viewing it 3 times.  Should fail on the third attempt.
    for (TInt rightscount = 1; rightscount <= 3; rightscount++)
        {
        INFO_PRINTF2(_L("Opening the file and decoding for rights count = %d "), rightscount);

        result = EFail;

        // Open and decode the image
        TInt err = KErrNone;
        TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId007, EView, EFalse));

        if ((err == KErrCANoPermission) && (rightscount == 3))
            {
            result = EPass;
            }
        else if (err != KErrNone)
            {
            INFO_PRINTF3(_L("Error in opening/decoding the image for rights count: %d, Error: %d"), rightscount, err);
            break;
            }
        }	
    return result;
    }
    
/**
*
* OpenImageAndDecodeFromFileSourceL
* Opens the image for 'the intent' and decodes.
*
* @param const TDesC& aFileName: Name of the data file.
* @param const TDesC& aUniqueId: Unique Id of the content file.
* @param ContentAccess::TIntent aIntent: Intent for opening the content file.
* @param TBool aFail: Simulate a no memory decode failure.
*
* @leave err: Leaves with 'err'.
*
*/
void RTImageDecOpenDRMConsumeRightsOnError::OpenImageAndDecodeFromFileSourceL(
    const TDesC& aFileName, 
    const TDesC& aUniqueId, 
    ContentAccess::TIntent aIntent,
    TBool aFail)
    {
    INFO_PRINTF4(_L("Opening the file %S for intent %d, having Unique Id %S."), &aFileName, aIntent, &aUniqueId);
    TMMFileSource fileSource(aFileName, aUniqueId, aIntent);

    // Open the image and create an image decoder
    TInt err = KErrNone;
    CImageDecoder* imageDecoder = NULL;	
    TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fileSource));
    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Error: %d"), &aFileName, err);
        User::Leave(err);
        }
    CleanupStack::PushL(imageDecoder);

    INFO_PRINTF4(_L("The file %S for intent %d, having Unique Id %S had been opened."), &aFileName, aIntent, &aUniqueId);

    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);

    const TFrameInfo* frameInfo = &imageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        frameInfo->iOverallSizeInPixels,
        frameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    if (aFail)
    	{
    	__UHEAP_SETFAIL(RHeap::EDeterministic, 1);
    	}
    	
    // Decode the image
    imageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener->iStatus.Int();
    if (err == KErrNone)
        {
        if (aFail)
        	{
			INFO_PRINTF4(_L("Conversion of file %S for intent %d, having Unique Id %S did not fail as requested"), &aFileName, aIntent, &aUniqueId);
			err = KErrUnknown;
        	}
        else
        	{
			INFO_PRINTF4(_L("Conversion of file %S for intent %d, having Unique Id %S was successfull!"), &aFileName, aIntent, &aUniqueId);
        	}
        }
    else
        {
        if (aFail && err == KErrNoMemory)
        	{
			INFO_PRINTF4(_L("Conversion of file %S for intent %d, having Unique Id %S failed as requested."), &aFileName, aIntent, &aUniqueId);
			err = KErrNone;
        	}
        else
        	{
			INFO_PRINTF3(_L("Failed during conversion of the file: %S, Error: %d"), &aFileName, err);
        	}
        }

    if (aFail)
    	{
    	__UHEAP_RESET;
    	}
    	
    // imageDecoder, activeListener, destBitmap
    CleanupStack::PopAndDestroy(3, imageDecoder);
    User::Leave(err);
    }


/**
*
* Ctor for RTImageDecOpenDRMConsumeRightsOnError2
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMConsumeRightsOnError2*: constructed RTImageDecOpenDRMConsumeRightsOnError2 object
*
*/
RTImageDecOpenDRMConsumeRightsOnError2* RTImageDecOpenDRMConsumeRightsOnError2::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMConsumeRightsOnError2(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMConsumeRightsOnError2::RTImageDecOpenDRMConsumeRightsOnError2(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7108-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMConsumeRightsOnError2::DoTestStepL()
	{
    INFO_PRINTF1(_L("DEF090885: Rights can be cosumed prior to successful decoding of an image"));
	INFO_PRINTF1(_L("Open a protected JPEG image for VIEW intent with rights count set to 3. Verify that a frame decoding failure does not consume rights."));

	TVerdict result = EFail;
	TInt err = KErrNone;
	
	// Open and decode the mng image to make sure we can
	TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId006, EView, EFalse));
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Error in opening/decoding the image, Error: %d"), err);
		return result;
		}
		
	//We have now used up 1 right to view.  We should have 2 left.
	//Cause the next attempt to fail due to no memory. This should not consume a right to view.
	TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId006, EView, ETrue));
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Error in opening/decoding the image while simulating no memory, Error: %d"), err);
		return result;
		}

	//If the defect has been fixed, we should still have two views left.
	//Try viewing it 3 times.  Should fail on the third attempt.
    for (TInt rightscount = 1; rightscount <= 3; rightscount++)
        {
        INFO_PRINTF2(_L("Opening the file and decoding for rights count = %d "), rightscount);

        result = EFail;

        // Open and decode the image
        TInt err = KErrNone;
        TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId006, EView, EFalse));

        if ((err == KErrCANoPermission) && (rightscount == 3))
            {
            result = EPass;
            }
        else if (err != KErrNone)
            {
            INFO_PRINTF3(_L("Error in opening/decoding the image for rights count: %d, Error: %d"), rightscount, err);
            break;
            }
        }	
    return result;
    }
    
/**
*
* OpenImageAndDecodeFromFileSourceL
* Opens the image for 'the intent' and decodes.
*
* @param const TDesC& aFileName: Name of the data file.
* @param const TDesC& aUniqueId: Unique Id of the content file.
* @param ContentAccess::TIntent aIntent: Intent for opening the content file.
* @param TBool aFail: Simulate a decode failure.
*
* @leave err: Leaves with 'err'.
*
*/
void RTImageDecOpenDRMConsumeRightsOnError2::OpenImageAndDecodeFromFileSourceL(
    const TDesC& aFileName, 
    const TDesC& aUniqueId, 
    ContentAccess::TIntent aIntent,
    TBool aFail)
    {
    INFO_PRINTF4(_L("Opening the file %S for intent %d, having Unique Id %S."), &aFileName, aIntent, &aUniqueId);
    TMMFileSource fileSource(aFileName, aUniqueId, aIntent);

    TInt err = KErrNone;

    // Open the image and create an image decoder
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAP(err,decoder = static_cast<CJPEGImageFrameDecoder*>(CImageDecoder::FileNewL(iFileSession, fileSource, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)););
	if (err != KErrNone)
		{
        INFO_PRINTF3(_L("Cannot open file: %S. Error: %d"), &aFileName, err);
        User::Leave(err);
		}
    CleanupStack::PushL(decoder);

    INFO_PRINTF4(_L("The file %S for intent %d, having Unique Id %S has been opened."), &aFileName, aIntent, &aUniqueId);

	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		// We want to test transcoding, create chunk anyway with a dummy size
		imageSizeInBytes = KDummyImageSize;
		}
			
	// create destination RChunk
	RChunk chunk;
	err = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);	
	User::LeaveIfError(err);
  	CleanupClosePushL(chunk);

	// create an empty imageframe   
	CImageFrame* theImageFrame = NULL;
	TRAP(err, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
	if (err != KErrNone)
		{
        INFO_PRINTF2(_L("Could not create imageFrame. Error: %d"), err);
        User::Leave(err);
		}
	CleanupStack::PushL(theImageFrame);
	
	if (aFail)
		{
		// Application is specifying a different format 4:2:0
		// The codec should leave with error code: KErrNotSupported,
		// since transcoding is not supported by this codec.
		TFrameFormat the420FrameFormat = TFrameFormat(KUidFormatYUV420Planar );
		theImageFrame->SetFrameFormatL(the420FrameFormat);	
		}
	CActiveListener* decodeListener = new (ELeave) CActiveListener();
	CleanupStack::PushL(decodeListener);
	decodeListener->InitialiseActiveListener();

	decoder->ConvertFrame(&decodeListener->iStatus, *theImageFrame);	
	CActiveScheduler::Start();
	// Check that the error return is KErrNotSupported
	err = decodeListener->iStatus.Int();
    	
    if (err == KErrNone)
        {
        INFO_PRINTF4(_L("Conversion of file %S for intent %d, having Unique Id %S was successfull!"), &aFileName, aIntent, &aUniqueId);
        }
    else
        {
        if (aFail && err == KErrNotSupported)
        	{
			INFO_PRINTF4(_L("Conversion of file %S for intent %d, having Unique Id %S failed as requested."), &aFileName, aIntent, &aUniqueId);
			err = KErrNone;
        	}
        else
        	{
			INFO_PRINTF3(_L("Failed during conversion of the file: %S, Error: %d"), &aFileName, err);
        	}
        }

    User::Leave(err);
    }

/**
*
* Ctor for RTImageDecOpenDRMConsumeRightsOnError3
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecOpenDRMConsumeRightsOnError3*: constructed RTImageDecOpenDRMConsumeRightsOnError3 object
*
*/
RTImageDecOpenDRMConsumeRightsOnError3* RTImageDecOpenDRMConsumeRightsOnError3::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageDecOpenDRMConsumeRightsOnError3(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageDecOpenDRMConsumeRightsOnError3::RTImageDecOpenDRMConsumeRightsOnError3(CTestSuite* aSuite)
    {
    iHeapSize = 500000;

    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7109-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageDecOpenDRMConsumeRightsOnError3::DoTestStepL()
	{
    INFO_PRINTF1(_L("DEF090885: Rights can be cosumed prior to successful decoding of an image"));
	INFO_PRINTF1(_L("Open a protected animated GIF image for VIEW intent with rights count set to 3. Verify that convert consumes only 1 right per convert not per frame."));

    TVerdict result = EFail;

    // Open and decode the image for three times
    // The rights count for this protected image is set to 3.
    // Hence looped four times to check failure while opening for the fourth time.
    for (TInt rightscount = 1; rightscount <= 4; rightscount++)
        {
        INFO_PRINTF2(_L("Opening the file and decoding for rights count = %d "), rightscount);

        result = EFail;

        // Open and decode the image
        TInt err = KErrNone;
        TRAP(err, OpenImageAndDecodeFromFileSourceL(iFileName, KRightsUniqueId008, EView, EFalse));

        if ((err == KErrCANoPermission) && (rightscount == 4))
            {
            result = EPass;
            }
        else if (err != KErrNone)
            {
            INFO_PRINTF3(_L("Error in opening/decoding the image for rights count: %d, Error: %d"), rightscount, err);
            break;
            }
        }

    return result;
    }
    





