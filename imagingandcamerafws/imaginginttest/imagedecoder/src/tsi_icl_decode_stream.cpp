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
// This file contains test step class implementation for streamed decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_stream.h"


// RStepBaseStreamedDecode
RStepBaseStreamedDecode::RStepBaseStreamedDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    iBlockPosCount = 0;
    }

// Read input from the configuration file
TInt RStepBaseStreamedDecode::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of section
    iSectionName = iTestStepName.Right(KSectionNameLen);
    if(ReadCommonInputImageFromConfigFile() == KErrNotFound)
	    {
		return KErrNotFound;
	    }
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("referenceimage"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iReferenceImage.Append(tmpKeyValue);

	if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iUM->iAlwaysThread)) || 
    	(!GetStringFromConfig(iSectionName, _L("outputimage"), tmpKeyValue)) || 
    	(!GetBoolFromConfig(iSectionName, _L("writeexif"), iUM->iWriteExif)) || 
    	(!GetIntFromConfig(iSectionName, _L("streammode"), iUM->iStreamMode)) || 
    	(!GetIntFromConfig(iSectionName, _L("outputimageheight"), iUM->iOPImageHeight)) || 
	    (!GetIntFromConfig(iSectionName, _L("outputimagewidth"), iUM->iOPImageWidth)) || 
    	(!GetBoolFromConfig(iSectionName, _L("hwretainoutputimage"), iUM->iHwRetainOutputImage)))
	 	{
		return KErrNotFound;
		}
	iUM->iOutputImage.Append(tmpKeyValue);
	

    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);
        	
	if (iUM->iWriteExif)
		{
		if ((!GetBoolFromConfig(iSectionName, _L("writethumbnail"), iUM->iWriteThumbnail)))
			{
			return KErrNotFound;
			}
    	}

    if (iUM->iStreamMode == KRandomForwardStreamDecode || iUM->iStreamMode == KRandomBackwardStreamDecode)
        {
		if ((!GetIntFromConfig(iSectionName, _L("numblockstoget"), iUM->iNumBlocksToGet)) || 
		   (!GetIntFromConfig(iSectionName, _L("multblockposcount"), iMultBlockPosCount)))
			{
			return KErrNotFound;
			}

        TBuf16<KSmallBufferLen> blockPos;
        for (TInt i = 0; i < iMultBlockPosCount; i++)
    	    {
    	    blockPos.Copy(_L("blockposition"));
    	    if (iMultBlockPosCount > 1)
    			{
    			blockPos.AppendNum(i);
    			}

            if (!GetIntFromConfig(iSectionName, blockPos, iUM->iBlockPosition))
                {
                return KErrNotFound;
                }
            iUM->iBlockPosArray.Append(iUM->iBlockPosition);
            }
        }

    if (iUM->iStreamMode == KSequentialStreamDecode || iUM->iStreamMode == KRandomBackwardStreamDecode)
        {
        if ((!GetIntFromConfig(iSectionName, _L("blockstoencoder"), iUM->iBlocksToEncoder)) || 
	       (!GetIntFromConfig(iSectionName, _L("iterations "), iUM->iIterations )))
            {
            return KErrNotFound;
            }
        }

    return KErrNone;
    }

// Initialize stream encoding
void RStepBaseStreamedDecode::InitializeStreamedEncodingL()
    {
    TInt err = KErrNone;
    if (iUM->iWriteExif)                // if true => set EXIF data
    	{
    	if (iUM->iWriteThumbnail)       // if true => set thumbnail data
    		{
    		iUM->iExifImageEncoder->SetThumbnail(ETrue);
    		}

    	// getting the EXIF metadata associated with the image being encoded
    	iUM->iExifMetadata = iUM->iExifImageEncoder->ExifMetadata();

    	// Add EXIF and thumbnail tags of each data type to the metadata
    	err = SetExifAndThumbnailData();
    	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Unable to set EXIF and/or thumbnail data to the image"));
    	}

    // Create stream encode object
    if (iUM->iWriteExif)
        {
        TRAP(err, iUM->iStreamEncode = iUM->iExifImageEncoder->BlockStreamerL());
        }
    else
        {
        TRAP(err, iUM->iStreamEncode = iUM->iImageEncoder->BlockStreamerL());
        }
    CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not create block streamer encoder"));
    INFO_PRINTF1(_L("Encoder BlockStreamerL() succeeded"));

    TInt frameNumber = 0;
    TSize frameSizeInPixels(iUM->iOPImageWidth, iUM->iOPImageHeight);

    // Set navigation mode within stream encoding
	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

    // Initialize the stream
	iUM->iStreamEncode->InitFrameL(iUM->iOptimalFormat, frameNumber, frameSizeInPixels, iUM->iBlockSizeInPixels, navigation, iUM->iFrameImageData);
    }

// Streamed encoding - appending blocks to destination image
void RStepBaseStreamedDecode::StreamedEncoding()
	{
	iUM->iActiveListener->InitialiseActiveListener();

	// record time and start decoding the image
    iUM->iStartTime.HomeTime();

	// append blocks to the stream
    iUM->iStreamEncode->AppendBlocks(iUM->iActiveListener->iStatus, *iUM->iImageFrame, iUM->iNumBlocksToGet);

    // decrement the iterations when blocks are appended
    iUM->iIterations--;
	}

// Complete the streamed encoding operation
void RStepBaseStreamedDecode::StreamedEncodingComplete()
	{
	iUM->iStreamEncode->Complete(iUM->iActiveListener->iStatus);
	}

// Initialize stream decoding
void RStepBaseStreamedDecode::InitializeStreamedDecodingL()
    {
    TInt err = KErrNone;

    // Set image type as thumbnail or main image
    TRAP(err, SetImageTypeL());
    CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not set image type"));
    // if scaling =>true then get the reduced size
    if (iScaleTest)
        {
        TRAP(err, SetExtensionL(CUtilityMembers::EExtensionScaleStream, iUM->iDestSize));
        CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not set scale size"));
        }

    // create stream decode object
    TRAP(err, iUM->iStreamDecode = iUM->iImageDecoder->BlockStreamerL());
    
    CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not create block stream decoder"));
    INFO_PRINTF1(_L("Decoder BlockStreamerL() succeeded"));

    // get the frame information of the decoder
    iUM->iFrameInfo = iUM->iImageDecoder->FrameInfo();

    // prepare decode data
    PrepareDecodeDataL();

    if (iUM->iStreamMode == KSequentialStreamDecode || iUM->iStreamMode == KRandomBackwardStreamDecode)
        {
        iUM->iNumBlocksToGet = iUM->iBlocksToEncoder;
        }

    // set navigation mode within stream decoding
    TInt frameNum = 0;
    TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationSequentialForward;
	if (iUM->iStreamMode == KRandomForwardStreamDecode)
        {
        nav = TDecodeStreamCaps::ENavigationRandomForward;
        }
    else if (iUM->iStreamMode == KRandomBackwardStreamDecode)
        {
        nav = TDecodeStreamCaps::ENavigationRandomBackwards;
        }

    // initialize the stream
    TRAP(err, iUM->iStreamDecode->InitFrameL(iUM->iOptimalFormat, frameNum, nav));
    if (err != KErrNone)
	    {
		User::Leave(err);	    	
	    }
    }

// Streamed decoding of a JPEG image to image frame
void RStepBaseStreamedDecode::StreamDecodeJpegImageToImageFrameL()
	{
    TInt err = KErrNone;
    TInt imageSizeInBytes = 0;
    TSize streamBlockSizeInPixels;

    // get the buffer size to hold the returned data
    imageSizeInBytes = iUM->iStreamDecode->GetBufferSize(iUM->iOptimalFormat, streamBlockSizeInPixels, iUM->iNumBlocksToGet);

    iUM->iBuffer = HBufC8::NewMaxL(imageSizeInBytes);
    TPtr8 ptr = iUM->iBuffer->Des();

    // create the image frame
    TRAP(err, iUM->iImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
    CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not create image frame"));

    // if scaling => true then set the image frame size as reduced size else set the returned streamblocksize
    if (iScaleTest)
        {
        iUM->iImageFrame->SetFrameSizeInPixels(iUM->iScaleSize);
        }
    else
        {
        iUM->iImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);
        }

    iUM->iActiveListener->InitialiseActiveListener();

    // for tests which cancel conversion and restart again after timer expires
	if (iUM->iConvCancelTest)
		{
		iUM->iTimer->InitializeTimer();
		}
    // record time and start decoding the image
    iUM->iStartTime.HomeTime();

    // for use by conversion cancellation tests, to know how much decode happended
    iUM->iBeginDecodeCounter = User::FastCounter();

	if (iUM->iStreamMode == KSequentialStreamDecode)
	    {
	    // get the blocks sequentialy from the stream and load in image frame
	    iUM->iStreamDecode->GetNextBlocks(iUM->iActiveListener->iStatus, *iUM->iImageFrame, iUM->iBlocksToEncoder, iUM->iNumBlocksRead, iUM->iHaveMoreBlocks);
	    }
    else if (iUM->iStreamMode == KRandomForwardStreamDecode)
        {
        TFrameFormat frameFormat(iUM->iOptimalFormat);
        iUM->iImageFrame->SetFrameFormatL(frameFormat);

        // get the blocks from stream and load in image frame by random forward stream decoding
        iUM->iStreamDecode->GetBlocks(iUM->iActiveListener->iStatus, *iUM->iImageFrame, iUM->iBlockPosition, iUM->iNumBlocksToGet, iUM->iNumBlocksRead);
        }
    else if (iUM->iStreamMode == KRandomBackwardStreamDecode)
        {
        TFrameFormat frameFormat(iUM->iOptimalFormat);
        iUM->iImageFrame->SetFrameFormatL(frameFormat);

        // get the blocks from stream and load in image frame by random backward stream decoding
        iUM->iStreamDecode->GetBlocks(iUM->iActiveListener->iStatus, *iUM->iImageFrame, (iUM->iIterations*(iUM->iFrameInfo.iOverallSizeInPixels.iWidth/iUM->iBlockSizeInPixels.iWidth)) + 1, iUM->iNumBlocksToGet, iUM->iNumBlocksRead);
        }
    else
        {
        ERR_PRINTF1(_L("Stream mode is not supported"));
        User::Leave(KErrGeneral);
        }        
	}

// Create stream decoder, initialize stream decoding and stream decode the stream to image frame
void RStepBaseStreamedDecode::StreamDecodeFilesL()
	{
	TRAPD(err, InitializeStreamedDecodingL()); // initialize stream decoding
	if (err != KErrNone)
		{
        // verify initialization for invalid data such as stream and thumbnail, 
        // scaling and streaming, YUV format not supported		
		if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0110-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0115-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0190-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0135-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0140-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0150-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0160-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0240-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0250-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0260-HP")) == 0 || 
            iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0270-HP")) == 0) && 
            (err == KErrNotSupported))
		    {
		    INFO_PRINTF2(_L("Could not initialize frame, expected error = %d"), err);
		    User::LeaveIfError(RetainEncodedImage());
		    StopTest(err, EPass);
		    return;
		    }
		if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0155-HP")) == 0) && (err == KErrNotSupported))
			{
		    INFO_PRINTF2(_L("Failure to stream multiscan image, expected error = %d"), err);
		    User::LeaveIfError(RetainEncodedImage());
		    StopTest(err, EPass);
		    return;
			}
		if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0131-HP")) == 0) && err == KErrGeneral)
			{
			INFO_PRINTF2(_L("GetSupportedFormatsL() returned different values. Test code returned %d"), err);
			User::LeaveIfError(RetainEncodedImage());
			StopTest(err, EPass);
		    return;
			}
		if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0132-HP")) == 0) && err == KErrNotSupported)
			{
			INFO_PRINTF2(_L("Could not initialize frame, InitFrameL returned expected error %d"), err);
			User::LeaveIfError(RetainEncodedImage());
			StopTest(err, EPass);
		    return;
			}			

        ERR_PRINTF2(_L("Could not initialize frame, error = %d"), err);		    
		StopTest(err, EFail);
		return;
		}

    // if multiple block positions are specified then pass each block position to get the stream data
    if (iMultBlockPosCount > 1)
        {
        iUM->iBlockPosition = iUM->iBlockPosArray[iBlockPosCount];
        iBlockPosCount++;
        }

    // stream decode to image frame
	TRAP(err, StreamDecodeJpegImageToImageFrameL());
	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Decoding of JPEG image to image frame failed"));
	}

// Start the tests
void RStepBaseStreamedDecode::DoKickoffTestL()
    {
	User::LeaveIfError(ReadCommonInputFromConfigFile());
	
    // Create output directory
    User::LeaveIfError(CreateOutputDirectory());

    iUM->iEnOptions = static_cast<CImageEncoder::TOptions> (CImageEncoder::EOptionExtStream);
    SetThreadForEncoding(iUM->iEnOptions); // set the option if encoding is performed in a separate thread

    // Create encoder
    TInt err = KErrNone;
    if (iUM->iWriteExif)
    	{
    	TRAP(err, iUM->iExifImageEncoder = static_cast<CJPEGExifEncoder*> (CImageEncoder::FileNewL(iUM->iFs, iUM->iOutputImage, iUM->iEnOptions, ImageType())));
    	}
    else
    	{
    	TRAP(err, iUM->iImageEncoder = CImageEncoder::FileNewL(iUM->iFs, iUM->iOutputImage, iUM->iEnOptions, ImageType()));
    	}
    CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not create image encoder"));

    iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtStream | CImageDecoder::EPreferFastDecode);
    SetThreadForDecoding(iUM->iDeOptions); // set the option if encoding is performed in a separate thread

    // Create decoder
    if (iUM->iWriteExif)
        {
        TRAP(err, iUM->iImageDecoder = CJPEGExifDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions));
        }
    else
        {
        TRAP(err, iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions));
        }
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0160-HP")) == 0) && err == KErrUnderflow)
        {
        ERR_PRINTF2(_L("Could not create image decoder, expected error = %d"), err);
        User::LeaveIfError(RetainEncodedImage());
        StopTest(err,EPass);
        return;
        }
	if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0170-HP")) == 0) && err == KErrNotFound)
        {
        ERR_PRINTF2(_L("Could not create image decoder, expected error = %d"), err);
        User::LeaveIfError(RetainEncodedImage());
        StopTest(err,EPass);
        return;
        }        
    CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not create image decoder"));

    // log test case description
    LogTestCaseDescription();

	iUM->iActiveListener = new (ELeave) CActiveListener(this);

    TRAP(err, FsmL());
    if (err != KErrNone)
    	{
        StopTest(err, EFail);
    	}
    }

RStepStreamedDecodeToImage* RStepStreamedDecodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedDecodeToImage(aStepName, aTestCaseDescription));
    }

RStepStreamedDecodeToImage::RStepStreamedDecodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseStreamedDecode(aStepName, aTestCaseDescription)
    {
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0400-HP")) == 0))
        {
        iHeapSize = 15000000; // 15MB this test cases uses a supersize image for boundary checking
        }
    }

void RStepStreamedDecodeToImage::FsmL()
	{
	TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            // Decode state
			StreamDecodeFilesL(); // initialize and stream decode the image to image frame
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Initilization and stream decode of JPEG image to image frame failed"));
			StateChange();
			break;

        case 1:// Encode decoded stream to output image
        	err = iUM->iActiveListener->iStatus.Int();
			if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0360-HP")) == 0 || 
			    iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0365-HP")) == 0 || 
			    iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0370-HP")) == 0) && 
			    (err == KErrOverflow))
			    {
			    INFO_PRINTF2(_L("Decoding JPEG image to image frame unsuccessful!block position greater than numberofblocks  error: %d"), err);
			    User::LeaveIfError(RetainEncodedImage());
			    StopTest(err, EPass);
			    return;
			    }
        	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Decoding JPEG image to image frame failed"));

			if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0290-HP")) == 0 || 
			    iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0300-HP")) == 0 || 
			    iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0310-HP")) == 0) && 
			    (iUM->iIterations == 2))
			    {
			    // if iUM->iIterations == 2 and getting the last block 
			    // then get the last (2nd iteration) blocks and append to dest image
                if (iUM->iHaveMoreBlocks != EFalse)
                    {
                    delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                    delete iUM->iBuffer; iUM->iBuffer = NULL;
                    StreamDecodeJpegImageToImageFrameL();
                    iUM->iIterations--;
                    }
			    }
            else
                {
                INFO_PRINTF1(_L("Decoding JPEG image to image frame successful!"));
                if (!iUM->iEnInitialiser)
                    {
                    // initialize stream encoding
					TRAP(err, InitializeStreamedEncodingL());
					if (iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0132-HP")) == 0 && err == KErrNotSupported)
						{
						INFO_PRINTF2(_L("InitFrame *correctly* returned error %d, as attempt at transcoding made (from 422 to 420)"), err);
						User::LeaveIfError(RetainEncodedImage());
						StopTest(err, EPass);
						return;
						}
					CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not initialize the frame"));
                    }
                // append blocks from image frame to dest image                    
                StreamedEncoding(); 
                iUM->iEnInitialiser = ETrue;
                StateChange();
                }
            break;

        case 2:// Get next blocks from decoder
        	err = iUM->iActiveListener->iStatus.Int();
        	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to append blocks"));
			INFO_PRINTF1(_L("Streamed encoding appended blocks"));

			// get next blocks from stream decode if iHaveMoreBlocks == true and 
			// no. of iterations for streaming is not over then append to dest image
            if (iUM->iIterations > 0)
                {
                if (iUM->iHaveMoreBlocks != EFalse || iUM->iStreamMode == KRandomBackwardStreamDecode)
                	{
                	delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                	delete iUM->iBuffer; iUM->iBuffer = NULL;
                   	StreamDecodeJpegImageToImageFrameL();
                	iUM->iState = 1;
                	}
                }
			// set the next blockPos for stream decoding if multiple blockPos are specified
            else  if (iBlockPosCount < iMultBlockPosCount && iMultBlockPosCount > 1)
                {
                iUM->iBlockPosition = iUM->iBlockPosArray.Append(iBlockPosCount);
                iBlockPosCount++;
                delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                delete iUM->iBuffer; iUM->iBuffer = NULL;
                StreamDecodeJpegImageToImageFrameL();
                iUM->iState = 1;
                }
            else
                {
                iUM->iActiveListener->InitialiseActiveListener();
                // complete stream encoding
                StreamedEncodingComplete(); 
                StateChange();
                }
			break;

		case 3:// Check streaming complete
			err = iUM->iActiveListener->iStatus.Int();
			if (err != KErrNone)
				{
                if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0310-HP")) == 0 && (err == KErrUnderflow)))
                    {
                    INFO_PRINTF2(_L("Streamed encoding failed to complete image,buffer size greater than no of blocks appended error = %d"), err);
                    User::LeaveIfError(RetainEncodedImage());
				    StopTest(err, EPass);
				    return;
                    }
				ERR_PRINTF3(_L("Streamed encoding failed to complete image (%S), error = %d"), &iUM->iOutputImage, err);
				StopTest(err, EFail);
				}
			else
			    {
			    //compare the output jpeg with reference jpeg
			    RStepBaseImageDecoder::CompareImagesL();
			    INFO_PRINTF1(_L("Streamed decoding completed"));
			    User::LeaveIfError(RetainEncodedImage());
			    StopTest(EPass);
			    }
			break;

        default:
       		ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
       		User::Leave(KErrGeneral);
        }
	}

RStepStreamedDecodeToImageScale* RStepStreamedDecodeToImageScale::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedDecodeToImageScale(aStepName, aTestCaseDescription));
    }

RStepStreamedDecodeToImageScale::RStepStreamedDecodeToImageScale(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseStreamedDecode(aStepName, aTestCaseDescription)
    {
    iScaleTest = ETrue;
    }

void RStepStreamedDecodeToImageScale::FsmL()
	{
	TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:// Decode state
            User::LeaveIfError(ReadCommonExtensionScalingInputFromConfigFile());// ReadScaleInputFromConfigFile()

            // initialize and stream decode the image to image frame
			StreamDecodeFilesL(); // initialize and stream decode the image to image frame
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Initilization and stream decode of JPEG image to image frame failed"));

            StateChange();
			break;

        case 1:// Encode decoded stream to output image
        	err = iUM->iActiveListener->iStatus.Int();
        	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Decoding JPEG image to image frame failed"));

			INFO_PRINTF1(_L("Decoding JPEG image to image frame successful!"));
			if (!iUM->iEnInitialiser)
			    {
			    // initialize stream encoding
			    InitializeStreamedEncodingL(); 
			    }
            // append blocks from image frame to dest image			    
			StreamedEncoding(); 
			iUM->iEnInitialiser = ETrue;
            StateChange();
            break;

        case 2:// Get next blocks from decoder
        	err = iUM->iActiveListener->iStatus.Int();
        	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to append blocks"));
			INFO_PRINTF1(_L("Streamed encoding appended blocks"));

			// get next blocks from stream decode if iHaveMoreBlocks == true
            if (iUM->iIterations > 0)
                {
                if (iUM->iHaveMoreBlocks != EFalse || iUM->iStreamMode == KRandomBackwardStreamDecode)
                	{
                	delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                	delete iUM->iBuffer; iUM->iBuffer = NULL;
                	StreamDecodeJpegImageToImageFrameL();
                	iUM->iState = 1;
                	}
                }
			// set the next blockPos for stream decoding if multiple blockPos are specified
            else if (iBlockPosCount < iMultBlockPosCount && iMultBlockPosCount > 1)
                {
                iUM->iBlockPosition = iUM->iBlockPosArray.Append(iBlockPosCount);
                iBlockPosCount++;
                delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                delete iUM->iBuffer; iUM->iBuffer = NULL; 
                StreamDecodeJpegImageToImageFrameL();
                iUM->iState = 1;
                }
            else
                {
                iUM->iActiveListener->InitialiseActiveListener();
                StreamedEncodingComplete(); // complete stream encoding
                StateChange();
                }
			break;

		case 3:// Check streaming complete
			err = iUM->iActiveListener->iStatus.Int();
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to complete image"));
			//compare the output jpeg with reference jpeg
			RStepBaseImageDecoder::CompareImagesL();
		    INFO_PRINTF1(_L("Streamed decoding completed"));
		    User::LeaveIfError(RetainEncodedImage());
		    StopTest(EPass);
			break;

        default:
       		ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
       		User::Leave(KErrGeneral);
        }
	}


RStepStreamedDecodeToImagePerf* RStepStreamedDecodeToImagePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedDecodeToImagePerf(aStepName, aTestCaseDescription));
    }

RStepStreamedDecodeToImagePerf::RStepStreamedDecodeToImagePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseStreamedDecode(aStepName, aTestCaseDescription)
    {
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0440-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0450-HP")) == 0))
        {
        iHeapSize = 15000000; // 15MB if supersize image is used then need more heapsize for stream decoding and encoding
        } 
    }

// Read input from the configuration file
TInt RStepStreamedDecodeToImagePerf::ReadTestStepInputFromConfigFile()
    {
	if ((!GetIntFromConfig(iSectionName, _L("expecteddecodetime"), iUM->iExpectedDecodeTime)) || 
	    (!GetIntFromConfig(iSectionName,_L("expectedencodetime"), iUM->iExpectedEncodeTime)))
		{
		return KErrNotFound;
		}
	return KErrNone;
    }

void RStepStreamedDecodeToImagePerf::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:// Decode state
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
			// initialize and streamdecode the image to image frame
			StreamDecodeFilesL(); // initialize and stream decode the image to image frame
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Initilization and stream decode of JPEG image to image frame failed"));

			StateChange();
            break;

        case 1:
            // Encode decoded stream to output image
        	err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Decoding JPEG image to image frame failed"));
			INFO_PRINTF1(_L("Decoding JPEG image to image frame successful!"));
	
			iUM->iEndTime.HomeTime(); // record end time for streamed decoding
			iUM->iDecodeActualTime = iUM->iDecodeActualTime + I64INT(iUM->iEndTime.MicroSecondsFrom(iUM->iStartTime).Int64());
			if (!iUM->iEnInitialiser)
			    {
			    // initialize stream encoding
			    InitializeStreamedEncodingL(); 
			    }
            // append blocks from image frame to dest image			    
			StreamedEncoding(); 
			iUM->iEnInitialiser = ETrue;
            StateChange();
            break;

        case 2:// Get next blocks from decoder
        	err = iUM->iActiveListener->iStatus.Int();
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to append blocks"));

			iUM->iEndTime.HomeTime(); // record end time for streamed encoding
			iUM->iEncodeActualTime = iUM->iEncodeActualTime + I64INT(iUM->iEndTime.MicroSecondsFrom(iUM->iStartTime).Int64());
			INFO_PRINTF1(_L("Streamed encoding appended blocks"));

			// get next blocks from stream decode if iHaveMoreBlocks = true
            if (iUM->iIterations > 0)
                {
                if (iUM->iHaveMoreBlocks != EFalse || iUM->iStreamMode == KRandomBackwardStreamDecode)
                	{
                	delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                    delete iUM->iBuffer; iUM->iBuffer = NULL;
                	StreamDecodeJpegImageToImageFrameL();
                	iUM->iState = 1;
                	}
                }
			// set the next blockPos for stream decoding if multiple blockPos are specified
            else  if (iBlockPosCount < iMultBlockPosCount && iMultBlockPosCount > 1)
                {
                iUM->iBlockPosition = iUM->iBlockPosArray.Append(iBlockPosCount);
                iBlockPosCount++;
                delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                delete iUM->iBuffer; iUM->iBuffer = NULL;
                StreamDecodeJpegImageToImageFrameL();
                iUM->iState = 1;
                }
            else
                {
                iUM->iActiveListener->InitialiseActiveListener();
                StreamedEncodingComplete(); // complete stream encoding
                StateChange();
                }
			break;

        case 3:// Check streaming complete
        	err = iUM->iActiveListener->iStatus.Int();
        	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to complete image"));
        	//compare the output jpeg with reference jpeg
        	RStepBaseImageDecoder::CompareImagesL();
			INFO_PRINTF1(_L("Streamed decoding completed"));
			CheckStreamPerformanceOutputAndSaveL();
			StopTest(EPass);
			break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }

RStepStreamedDecodeToImageRealTimeTests* RStepStreamedDecodeToImageRealTimeTests::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedDecodeToImageRealTimeTests(aStepName, aTestCaseDescription));
    }

RStepStreamedDecodeToImageRealTimeTests::RStepStreamedDecodeToImageRealTimeTests(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseStreamedDecode(aStepName, aTestCaseDescription)
    {
    }

// Read input from the configuration file
TInt RStepStreamedDecodeToImageRealTimeTests::ReadTestStepInputFromConfigFile()
    {
    TInt time;
	if ((!GetBoolFromConfig(iSectionName, _L("convcanceltest"), iUM->iConvCancelTest)) || 
		(!GetIntFromConfig(iSectionName, _L("timerexpire"), time)) || 
		(!GetBoolFromConfig(iSectionName, _L("restartconvaftercancel"), iUM->iRestartConvAfterCancel)))
    	{
		return KErrNotFound;
		}
	iUM->iTimerExpire = TTimeIntervalMicroSeconds32(time);
	return KErrNone;
    }

// Timer to cancel the decoding operation after some duration, used in cancel decoding tests
void RStepStreamedDecodeToImageRealTimeTests::TimerExpired()
	{
	if (iUM->iImageDecoder)
		{
		iUM->iImageDecoder->Cancel();
		TInt err = iUM->iActiveListener->iStatus.Int();
		if (err != KErrCancel)
			{
			StopTest(err, EFail);
			return;
			}
		iUM->iConvCancelled = ETrue;
		TRAP(err, FsmL());
		if (err != KErrNone)
			{
			StopTest(err, EFail);
			return;
			}
		}
	}

void RStepStreamedDecodeToImageRealTimeTests::FsmL()
    {
    TUint32 decodeCounter = 0;
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:// Decode state
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            iUM->iTimer = CUtilityTimer::NewL(iUM->iTimerExpire, *this);

			StreamDecodeFilesL(); // initialize and stream decode the image to image frame
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Initilization and stream decode of JPEG image to image frame failed"));

			StateChange();
            break;

        case 1:// Decoding to cancel in the middle
        	iUM->iEndDecodeCounter = User::FastCounter();
            iUM->iActiveListener->Cancel();
        	// confirm that it has at least decoded some part of the stream
        	decodeCounter = iUM->iEndDecodeCounter - iUM->iBeginDecodeCounter;

        	// if stream decoding is not cancelled then iUM->iConvCancelled == false
			if (!iUM->iConvCancelled)
				{
				ERR_PRINTF2(_L("Cancelling stream decoding unsuccessful, error = %d"), err);
				StopTest(EFail);
				return;
				}

			INFO_PRINTF2(_L("Image decoded %d counters since the start"), decodeCounter);
			INFO_PRINTF1(_L("Cancelling stream decoding successful!"));

			// if iRestartConvAfterCancel = false stop the test
			if (!iUM->iRestartConvAfterCancel)
				{
				User::LeaveIfError(RetainEncodedImage());
    			StopTest(EPass);
				break;
				}
            iUM->iConvCancelTest = EFalse;
            // if restart the test after cancellation
            delete iUM->iImageDecoder; iUM->iImageDecoder = NULL;
            delete iUM->iBuffer; iUM->iBuffer = NULL;
            delete iUM->iImageFrame; iUM->iImageFrame = NULL;
            delete iUM->iFrameImageData; iUM->iFrameImageData = NULL;

            INFO_PRINTF1(_L("Stream decoding with new decoder!"));
            TRAP(err, iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions));
            CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Could not create new image decoder"));            
   			StreamDecodeFilesL(); // initialize and stream decode the image to image frame
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Initilization and stream decode of JPEG image to image frame failed"));

            StateChange();
            break;

        case 2:// Encode decoded stream to output image
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Stream decoding with new decoder unsuccessful"));

			INFO_PRINTF1(_L("Stream decoding with new decoder successful!"));
			if (!iUM->iEnInitialiser)
			    {
			    // initialize stream encoding
			    InitializeStreamedEncodingL();
			    }

            // append blocks from image frame to dest image
			StreamedEncoding();
			iUM->iEnInitialiser = ETrue;
            StateChange();
            break;

        case 3:// Get next blocks from decoder
        	err = iUM->iActiveListener->iStatus.Int();
        	CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to append blocks"));
			INFO_PRINTF1(_L("Streamed encoding appended blocks"));

			// get next blocks from stream decode if iHaveMoreBlocks == true
            if (iUM->iIterations > 0)
                {
                if (iUM->iHaveMoreBlocks != EFalse || iUM->iStreamMode == KRandomBackwardStreamDecode)
                	{
                	delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                	delete iUM->iBuffer; iUM->iBuffer = NULL;
                	StreamDecodeJpegImageToImageFrameL();
                	iUM->iState = 2;
                	}
                }
			// set the next blockPos for stream decoding if multiple blockPos are specified
            else if (iBlockPosCount < iMultBlockPosCount && iMultBlockPosCount > 1)
                {
                iUM->iBlockPosition = iUM->iBlockPosArray.Append(iBlockPosCount);
                iBlockPosCount++;
                delete iUM->iImageFrame; iUM->iImageFrame = NULL;
                delete iUM->iBuffer; iUM->iBuffer = NULL;
                StreamDecodeJpegImageToImageFrameL();
                iUM->iState = 1;
                }
            else
                {
                iUM->iActiveListener->InitialiseActiveListener();
                StreamedEncodingComplete(); // complete stream encoding
                StateChange();
                }
	    	break;

        case 4:// Check streaming complete
			err = iUM->iActiveListener->iStatus.Int();
			CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, _L("Streamed encoding failed to complete image"));
			//compare the output jpeg with reference jpeg
			RStepBaseImageDecoder::CompareImagesL();
		    INFO_PRINTF1(_L("Streamed decoding completed"));
		    User::LeaveIfError(RetainEncodedImage());
		    StopTest(EPass);
			break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }
