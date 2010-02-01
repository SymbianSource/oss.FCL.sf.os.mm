// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains test step class implementation for mirror during encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


// Test user includes
#include "tsi_icl_encode_mirror.h"


// Read input from the configuration file
TInt RStepMirrorDuringEncodeBase::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of section
    iSectionName = iTestStepName.Right(KSectionNameLen);
    TPtrC tmpFilename;
	if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iAlwaysThread))  || 
		(!GetBoolFromConfig(iSectionName, _L("hwretainoutputimage"), iHwRetainOutputImage)) ||
    	(!GetStringFromConfig(iSectionName, _L("outputimage"), tmpFilename))   || 
    	(!GetStringFromConfig(_L("directory"), _L("outputpath"), iOutputPath)) ||
	    (!GetBoolFromConfig(iSectionName, _L("writeexif"), iWriteExif)) ||
    	(!GetIntFromConfig(iSectionName, _L("operationindex"), iOperationIndex)))
		{
		return KErrNotFound;
		}
	iOutputImage.Append(tmpFilename);
	
	if (iWriteExif)
		{
		if ((!GetBoolFromConfig(iSectionName, _L("writethumbnail"), iWriteThumbnail)))
			{
			return KErrNotFound;			
			}
		}
	    	
    if (!GetStringFromConfig(iSectionName, _L("inputimage"), tmpFilename))
        {
        return KErrNotFound;
        }
    iInputImage.Append(tmpFilename);

    if (!GetStringFromConfig(iSectionName, _L("referenceimage"), tmpFilename))
        {
        return KErrNotFound;
        }        
    iReferenceImage.Append(tmpFilename);
    
	return KErrNone;
    }

// Mirror during Encoding of a bitmap to JPEG format
void RStepMirrorDuringEncodeBase::MirrorABitmapDuringEncodeL()
	{
	TInt res = KErrNone;	
	res = iSrcBitmap->Load(iInputImage);
	CHECK_STATUS_LEAVE_IF_ERROR(res,_L("Cannot load input file into a bitmap"));
	
	// Add extension interface operations to encoder
	TRAP(res,AddOperationL());
    CHECK_STATUS_LEAVE_IF_ERROR(res,_L("AddOpeartionL failed to add the operation to encoder"));	
	
	// For ClearOperationsStack test cases, if previously 
	// applied operations are cleared and want to reapply operations	
    if (iReapplyOperations)
        {
        INFO_PRINTF1(_L("Re-apply the operations to encoder"));
        iOperationIndex = iOperationIndex2;
        // Add extension interface operations to encoder
    	TRAP(res,AddOperationL());
    	CHECK_STATUS_LEAVE_IF_ERROR(res,_L("AddOpeartionL failed to re-apply the operations to encoder"));	
        }

	// Switch encoder from JFIF (default) to EXIF if iWriteExif = 1
	if (iWriteExif)
		{
		// Encode image with/without thumbnail
    	// true ==> add thumbnail data, false ==> don't add thumbnail data
	    if (iWriteThumbnail)
    		{
    		iExifEncoder->SetThumbnail(ETrue);
    		}
		iExifMetaData = iExifEncoder->ExifMetadata();
		
		// Add EXIF and Thumbnail tags of each data type to the metadata
		res = SetExifAndThumbnailData(iExifMetaData);		
        CHECK_STATUS_LEAVE_IF_ERROR(res,_L("Unable to set EXIF and/or thumbnail data to the image"));		
		}

	// Prepare encode data
	PrepareEncodeDataL();

	// Mirror the bitmap through the requisite axis
	iActiveListener->InitialiseActiveListener();

	if (iConvCancelTest)
		{
		iTimer->InitializeTimer();
		}

	iBeginEncodeCounter = User::FastCounter();
	iStartTime.HomeTime();
	if (!iWriteExif)
		{
		iImageEncoder->Convert(&iActiveListener->iStatus, *iSrcBitmap, iFrameEnImageData);
		}
	else
		{
		iExifEncoder->Convert(&iActiveListener->iStatus, *iSrcBitmap, iFrameEnImageData);
		}	
	}


// Mirror during Encoding of a JPEG image from YUV pixel buffer
void RStepMirrorDuringEncodeBase::EncodeImageFrameWithYUVDataL()
	{
	TInt error = KErrNone;
	// Prepare encode data
	PrepareEncodeDataL();

	// Add extension interface operations to encoder
	TRAP(error,AddOperationL());
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("AddOpeartionL failed to add the operation to encoder"));
	iActiveListener->InitialiseActiveListener();
	//
	iJPEGImageEncoder->ConvertFrame(&iActiveListener->iStatus, *iImageFrame, iFrameEnImageData);
	}

// Decoding of a JPEG image to YUV pixel buffer to craete an  image frame
void RStepMirrorDuringEncodeBase::DecodeJpegImageToYUVImageFrameL()
	{
	TInt error = KErrNone;
	TInt imageSizeInBytes;
	TBool supported = iJPEGImageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		INFO_PRINTF1(_L("Codec does not support this image format"));
		User::Leave(KErrNotSupported);
		}

	// Create destination RChunk
	error = iChunk.CreateLocal(imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	User::LeaveIfError(error);

	// create an empty imageframe
	TRAP(error, iImageFrame = CImageFrame::NewL(&iChunk, imageSizeInBytes, KChunkDataOffset));
	CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create imageframe"));

	iActiveListener->InitialiseActiveListener();
	iJPEGImageDecoder->ConvertFrame(&iActiveListener->iStatus, *iImageFrame);
	}
	
/*	
*	Mirror during encoding of a JPEG image from YUV pixel 
*	buffer with unsupported YUV format	
*/
void RStepMirrorDuringEncodeBase::NegEncodeImageFrameWithYUVDataL()
	{
	iFrameEnImageData = CFrameImageData::NewL();
	TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
	jpegFormat->iSampleScheme = TJpegImageData::EColor444;
	User::LeaveIfError(iFrameEnImageData->AppendImageData(jpegFormat));

	// Add extension interface operations to encoder
	TRAPD(error,AddOperationL());
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("AddOpeartionL failed to add the operation to encoder"));

	iActiveListener->InitialiseActiveListener();
	iJPEGImageEncoder->ConvertFrame(&iActiveListener->iStatus, *iImageFrame, iFrameEnImageData);
	}

//start the test
void RStepMirrorDuringEncodeBase::DoKickoffTestL()
    {
    TInt error = KErrNone;
    User::LeaveIfError(ReadCommonInputFromConfigFile());
    
	// Create output directory
	User::LeaveIfError(CreateOutputDirectory());

	TFileName tmpname;
	tmpname.Append(iOutputPath);
	tmpname.Append(iOutputImage);
	iOutputImage.Copy(tmpname);

	iActiveListener = new (ELeave) CActiveListener(this);
	iSrcBitmap = new(ELeave) CFbsBitmap;
    iOptions = static_cast<CImageEncoder::TOptions> (CImageEncoder::EOptionExtMirrorHorizontalAxis | CImageEncoder::EOptionExtMirrorVerticalAxis | CImageEncoder::EOptionExtRotation);

    SetThreadForEncoding(iOptions); // set the option if encoding is performed in a separate thread

    iMimeType.Copy(KMimeTypeJpeg);

	//Log the test case description based on the test case scenario
	LogTestCaseDescription();

    TRAP(error, FsmL());
    if (error != KErrNone)
    	{
        StopTest(error, EFail);
        return;
    	}
    }

/*
*	Mirror during coding test cases
*
*/
RStepMirrorDuringEncodeToImage* RStepMirrorDuringEncodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepMirrorDuringEncodeToImage(aStepName, aTestCaseDescription));
    }

RStepMirrorDuringEncodeToImage::RStepMirrorDuringEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepMirrorDuringEncodeBase(aStepName, aTestCaseDescription)
    {
    }

void RStepMirrorDuringEncodeToImage::FsmL()
    {
    TInt err = KErrNone;
    switch (iState)
        {
        case 0://Mirror during encoding with exif data
            CreateImageEncoderL();
			MirrorABitmapDuringEncodeL();       // Mirror during encoding
            StateChange();
            break;

        case 1://check the status of mirror during encoding
            err = iActiveListener->iStatus.Int();
            if (err != KErrNone)
				{				
				if (iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0220-HP")) == 0 || 
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0230-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0260-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0270-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0290-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0300-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0310-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0340-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0350-HP")) == 0 &&
			    err == KErrNotSupported )
			    	{
					ERR_PRINTF2(_L("Mirror during Encode unsuccessful, with expected error = %d"), err);
					StopTest(EPass);
                	return;
					}					
                ERR_PRINTF2(_L("Mirror during Encode unsuccessful, error = %d"), err);
                StopTest(err, EFail);
                return;
                }
			INFO_PRINTF1(_L("Mirror during Encode successful!"));
			if (iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0250-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0280-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0320-HP")) == 0 ||
			    iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0330-HP")) == 0)
			    {
			    CompareFileL();
			    }
			
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    } 

/*
*	Mirror during encoding performance tests
*	
*/
RStepMirrorDuringEncodeToImagePerf* RStepMirrorDuringEncodeToImagePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepMirrorDuringEncodeToImagePerf(aStepName, aTestCaseDescription));
    }

RStepMirrorDuringEncodeToImagePerf::RStepMirrorDuringEncodeToImagePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepMirrorDuringEncodeBase(aStepName, aTestCaseDescription)
    {    
    }

// Read performance measurement input from the config file
TInt RStepMirrorDuringEncodeToImagePerf::ReadInputFromConfigFile()
    {
    TPtrC tmpFilename;
	if ((!GetIntFromConfig(iSectionName, _L("expectedtime"), iExpectedTime)))
		{
		return KErrNotFound;
		}
	return KErrNone;
    }

void RStepMirrorDuringEncodeToImagePerf::FsmL()
    {
    switch (iState)
        {
        case 0:// Mirror during encoding
            User::LeaveIfError(ReadInputFromConfigFile());
            CreateImageEncoderL();
			MirrorABitmapDuringEncodeL();
            StateChange();
            break;

        case 1:// Check status and timetaken for mirror during encoding
        	{
            iEndTime.HomeTime(); // record end time for encoding
        	TInt err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Mirror during Encode unsuccessful"));
			INFO_PRINTF1(_L("Mirror During Encode successful!"));
			CompareFileL();
            CheckPerformanceOutputAndSaveL();               // Check performance based on time and log the result 
            StopTest(EPass);
            break;
        	}
			
        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }

//	Mirror during encoding test cases for Realtime events test cases
RStepMirrorDuringEncodeToImageRealTimeTests* RStepMirrorDuringEncodeToImageRealTimeTests::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepMirrorDuringEncodeToImageRealTimeTests(aStepName, aTestCaseDescription));
    }

RStepMirrorDuringEncodeToImageRealTimeTests::RStepMirrorDuringEncodeToImageRealTimeTests(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepMirrorDuringEncodeBase(aStepName, aTestCaseDescription)
    { 
    }

// Read input from the configuration file
TInt RStepMirrorDuringEncodeToImageRealTimeTests::ReadInputFromConfigFile()
    {
    TInt time;
	if ((!GetBoolFromConfig(iSectionName,_L("convcanceltest"), iConvCancelTest)) ||
		(!GetIntFromConfig(iSectionName, _L("timerexpire"), time)) ||
		(!GetBoolFromConfig(iSectionName,_L("restartconvaftercancel"), iRestartConvAfterCancel)))
	    	{
			return KErrNotFound;
			}
	iTimerExpire = TTimeIntervalMicroSeconds32(time);
	return KErrNone;		
    }
    
// Timer to cancel the encoding operation after some duration
// used for cancel encoding tests inbetween
void RStepMirrorDuringEncodeToImageRealTimeTests::TimerExpired()
	{
	if (iImageEncoder)
		{
		iImageEncoder->Cancel();
		TInt error = iActiveListener->iStatus.Int();
		if (error != KErrCancel)
			{
			StopTest(error, EFail);
			return;
			}
		iConvCancelled = ETrue;
		TRAP(error, FsmL());
		if (error != KErrNone)
			{
			StopTest(error, EFail);
			return;
			}
		}
	}
//State Machine
void RStepMirrorDuringEncodeToImageRealTimeTests::FsmL()
    {
	TUint32 encodeCounter = 0;
    TInt err = KErrNone; 
    switch (iState)
        {
        case 0:// Mirror during encoding
            User::LeaveIfError(ReadInputFromConfigFile());
            iTimer = CUtilityTimer::NewL(iTimerExpire, *this);
            CreateImageEncoderL();
			MirrorABitmapDuringEncodeL();
            StateChange();
            break;

        case 1:// Check status of mirror during encoding when cancelled, then restart if iRestartConvAfterCancel = true
        	iEndEncodeCounter = User::FastCounter();
        	encodeCounter = iEndEncodeCounter - iBeginEncodeCounter;
			if (!iConvCancelled)
				{
				ERR_PRINTF2(_L("Cancel Mirror During Encoding unsuccessful with error = %d"), err);
				StopTest(EFail);
				return;
				}
			INFO_PRINTF2(_L("Image Encoded, %d counters since the start"), encodeCounter );
			INFO_PRINTF1(_L("Cancelled Mirror During Encoding successfully"));
			iActiveListener->Cancel();
			if (!iRestartConvAfterCancel)
				{
				StopTest(EPass);
				return;
				}
			delete iImageEncoder;iImageEncoder = NULL;

		    // Create new encoder as its not possible to restart the convert using the existing encoder	
		    CreateImageEncoderL();
			INFO_PRINTF1(_L("Created new encoder successfully"));
			
			// Add extension interface operations to encoder
			TRAP(err,AddOperationL());
			CHECK_STATUS_LEAVE_IF_ERROR(err,_L("AddOpeartionL failed to add the operation to encoder"));
			INFO_PRINTF1(_L("Restarted mirror during encoding"));
			iActiveListener->InitialiseActiveListener();				
			iImageEncoder->Convert(&iActiveListener->iStatus, *iSrcBitmap,iFrameEnImageData);
			StateChange();
            break;

        case 2:// check status of mirror during encoding
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Mirror during Encode with new encoder unsuccessful"));
			INFO_PRINTF1(_L("Mirror during Encode with new encoder successful!"));
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }
    
/*
*	Mirror during encoding test cases with YUV pixel buffer as source
*	Decode image to YUV buffer image frame then mirror during encode image frame 
*	
*/

RStepMirrorDuringEncodeYUVPixelBuffer* RStepMirrorDuringEncodeYUVPixelBuffer::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepMirrorDuringEncodeYUVPixelBuffer(aStepName, aTestCaseDescription));
    }

RStepMirrorDuringEncodeYUVPixelBuffer::RStepMirrorDuringEncodeYUVPixelBuffer(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepMirrorDuringEncodeBase(aStepName, aTestCaseDescription)
    {
    if (aStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0160-HP")) == 0)
    	{
    	iPositiveTest = EFalse;
    	}
	else
		{
		iPositiveTest = ETrue;
		}    	
    }

// State Machine    
void RStepMirrorDuringEncodeYUVPixelBuffer::FsmL()
    {
    TInt err = KErrNone;;
    switch (iState)
        {
        case 0:// Decode to imageframe
            CreateImageFrameDecoderL();
            CreateImageFrameEncoderL();
            DecodeJpegImageToYUVImageFrameL();
            StateChange();
            break;

        case 1:// Mirror during encoding of imageframe
			err = iActiveListener->iStatus.Int();
			if (iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0165-HP")) == 0 && err == KErrOverflow)
			    {
			    INFO_PRINTF2(_L("Mirror during Encode for multiscan jpeg image with YUV pixel buffer unsuccessful, expected error %d"), err);
			    StopTest(err, EPass);
			    return;
			    }
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Failed to decode image"));
			if (iPositiveTest)
				{
				EncodeImageFrameWithYUVDataL();	        // Mirror during encoding of YUV frame
				}
			else
				{
				NegEncodeImageFrameWithYUVDataL();      // Mirror during encoding of YUV frame with invaild data
				}
        	StateChange();
			break;

        case 2:// Check status of mirror during encoding
        	if (iPositiveTest)
				{
				err = iActiveListener->iStatus.Int();
				CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Mirror during Encode with YUV pixel buffer unsuccessful"));
				INFO_PRINTF1(_L("Mirror during Encode with YUV pixel buffer successful!"));
    			StopTest(EPass);
				}
			else
				{
				err = iActiveListener->iStatus.Int();
                if (err == KErrNotSupported)
					{
					INFO_PRINTF2(_L("Mirror During encoding with unsupported YUV Pixel buffer returned expected error = %d"), err);
					StopTest(err, EPass);
					}
				else
					{
					ERR_PRINTF3(_L("Mirror During encoding with unsupported YUV Pixel buffer returned error = %d, expected error = %d"), err, KErrNotSupported);
					StopTest(err, EFail);
					return;
					}
				}
			break;
        	
        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }
/*
*	Rotation and mirror during encoding test cases 
*
*/
RStepRotateMirrorDuringEncodeToImage* RStepRotateMirrorDuringEncodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateMirrorDuringEncodeToImage(aStepName, aTestCaseDescription));
    }

RStepRotateMirrorDuringEncodeToImage::RStepRotateMirrorDuringEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepMirrorDuringEncodeBase(aStepName, aTestCaseDescription)
    {
    }

// State Machine
void RStepRotateMirrorDuringEncodeToImage::FsmL()
    {
    TInt err = KErrNone;
    switch (iState)
        {
        case 0:// Rotate/Mirror during encoding
            CreateImageEncoderL();
			MirrorABitmapDuringEncodeL();
            StateChange();
            break;

        case 1:// Check status of rotate/mirror during encoding
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Rotation and/or Mirror during Encode unsuccessful"));
			INFO_PRINTF1(_L("Rotation and/or Mirror during Encode successful!"));
		    CompareFileL();
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }

// Read clearopstack input from confg file    
TInt RStepRotateMirrorClearOpStackDuringEncodeToImage::ReadInputFromConfigFile()
    {
    if ((!GetBoolFromConfig(iSectionName, _L("clearoperations"), iClearOperations)))
		{
		return KErrNotFound;
		}
    if (iClearOperations)
        {
        if ((!GetBoolFromConfig(iSectionName, _L("reapplyoperations"), iReapplyOperations)))
            {
            return KErrNotFound;
            }
        if (iReapplyOperations)
            {
            if ((!GetIntFromConfig(iSectionName, _L("operationindex2"), iOperationIndex2)))
                {
                return KErrNotFound;
                }    
            }
        }
    return KErrNone;
    }
    
/*
*	Rotation and mirror during encoding test cases with ClearOperationSatck
*
*/
RStepRotateMirrorClearOpStackDuringEncodeToImage* RStepRotateMirrorClearOpStackDuringEncodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateMirrorClearOpStackDuringEncodeToImage(aStepName, aTestCaseDescription));
    }

RStepRotateMirrorClearOpStackDuringEncodeToImage::RStepRotateMirrorClearOpStackDuringEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepMirrorDuringEncodeBase(aStepName, aTestCaseDescription)
    {
    }

// State Machine
void RStepRotateMirrorClearOpStackDuringEncodeToImage::FsmL()
    {
    TInt err = KErrNone;
    switch (iState)
        {
        case 0:// Rotate/Mirror During encoding with operations stack cleared, if 
            User::LeaveIfError(ReadInputFromConfigFile());
            CreateImageEncoderL();
			MirrorABitmapDuringEncodeL();
            StateChange();
            break;

        case 1:// check status
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Rotation and/or Mirror during Encode unsuccessful"));
			INFO_PRINTF1(_L("Rotation and/or Mirror during Encode successful!"));
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }
