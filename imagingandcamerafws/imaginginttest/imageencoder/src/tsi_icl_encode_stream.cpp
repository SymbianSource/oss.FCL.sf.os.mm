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
// This file contains test step class implementation for streamed encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


// Test user includes
#include "tsi_icl_encode_stream.h"
#include "jpgimageframeprocessor.h"

/*
*   Read Common input from config file
*/
TInt RStepStreamedEncodeBase::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of section
    iSectionName = iTestStepName.Right(KSectionNameLen);    
    TPtrC tmpFilename;
	if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iAlwaysThread)) || 
    	(!GetStringFromConfig(iSectionName, _L("outputimage"), tmpFilename)) || 
    	(!GetStringFromConfig(_L("directory"), _L("outputpath"), iOutputPath)) || 
    	(!GetBoolFromConfig(iSectionName, _L("writeexif"), iWriteExif)) ||
    	(!GetIntFromConfig(iSectionName, _L("totalblocks"), iTotalblocks)) || 
		(!GetIntFromConfig(iSectionName, _L("numblockstoadd"), iNumBlocksToAdd))|| 
		(!GetIntFromConfig(iSectionName, _L("iterateblocks"), iIterateblocks))||
	 	(!GetBoolFromConfig(iSectionName, _L("midstream"), iMidStream)) ||
	 	(!GetBoolFromConfig(iSectionName, _L("hwretainoutputimage"), iHwRetainOutputImage)) ||
	 	(!GetIntFromConfig(iSectionName, _L("inputimagecount"), iInputImageCount)))
	 	{   
		return KErrNotFound;
		}
	iOutputImage.Append(tmpFilename);
    
    if (!GetStringFromConfig(iSectionName, _L("referenceimage"), tmpFilename))
        {
        return KErrNotFound;
        }        
    iReferenceImage.Append(tmpFilename);
    	
	if (iWriteExif)
		{
		if ((!GetBoolFromConfig(iSectionName, _L("writethumbnail"), iWriteThumbnail)))
			{
			return KErrNotFound;			
			}
		}
	if (iInputImageCount > 1)
		{
		if ((!GetIntFromConfig(iSectionName, _L("outputimageheight"), iOPImageHeight)) || 
			(!GetIntFromConfig(iSectionName, _L("outputimagewidth"), iOPImageWidth)))
			{
			return KErrNotFound;
			}
		}

    TBuf16<15> inputimage;
	for (TInt i = 0; i < iInputImageCount; i++)				
		{
		inputimage.Copy(_L("inputimage"));
		if (iInputImageCount > 1)
			{
			inputimage.AppendNum(i);
			}
			
		if (!GetStringFromConfig(iSectionName, inputimage, tmpFilename))
	        {
	        return KErrNotFound;
	        }
		iInputImageArray.Insert(tmpFilename, i);
		}
			
    return KErrNone;
    }

// Initializing for streamed encoding
void RStepStreamedEncodeBase::InitializeStreamedEncodingL()
	{	
	// Encode from an image frame
	TInt error = KErrNone;
	if (!iEncodedImageCount)
		{
        CreateImageEncoderL();
		if (iWriteExif)
			{
			if (iWriteThumbnail)
				{
				iExifEncoder->SetThumbnail(ETrue);
				}
			// Switch encoder from JFIF (default) to EXIF
			iExifMetaData = iExifEncoder->ExifMetadata();

			// Add EXIF and Thumbnail tags of each data type to the metadata
			error = SetExifAndThumbnailData(iExifMetaData);		
            CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Unable to set EXIF and/or thumbnail data to the image"));
			
			TRAP(error, iStreamEncode = iExifEncoder->BlockStreamerL());
			CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not initialize block streamer encoder"));
			}
        else
            {
            TRAP(error, iStreamEncode = iImageEncoder->BlockStreamerL());
            CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not initialize block streamer encoder"));
            }			
        
		INFO_PRINTF1(_L("BlockStreamerL() succeeded"));			
		}

	TSize frameSizeInPixels = iImageFrame->FrameSizeInPixels();
	iImageWidth = frameSizeInPixels.iWidth;
	iImageHeight = frameSizeInPixels.iHeight;
	if (iInputImageCount > 1)
		{
		frameSizeInPixels.iWidth = iOPImageWidth;
		frameSizeInPixels.iHeight = iOPImageHeight;
		}
    //Set the stream encoding mode
	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

	// Prepare encode data
	PrepareEncodeDataL();

	TInt frameNumber = 0;

    // Initialise the stream.This is initialised only once at the start of the encoding
	if (!iEncodedImageCount)
		{
		TRAP(error,iStreamEncode->InitFrameL(iOptimalFormat, frameNumber,frameSizeInPixels, iBlockSizeInPixels,navigation, iFrameEnImageData));
		if (error != KErrNone)
		    {
		    User::Leave(error);
		    }        
		}
	}

// Streamed encoding of a bitmap to JPEG format
void RStepStreamedEncodeBase::StreamedEncoding()
	{
	iActiveListener->InitialiseActiveListener();
    if (iConvCancelTest)
		{
		iTimer->InitializeTimer();
		}			
	if (iCount == 0)
		{
		iBeginEncodeCounter = User::FastCounter();// to know how fast image is encoded
		iStartTime.HomeTime(); // record start time for streamed encoding	
		}
    TSize frameSizeInPixels = iImageFrame->FrameSizeInPixels();		
    //Append blocks to the stream
    iStreamEncode->AppendBlocks(iActiveListener->iStatus, *iImageFrame, iNumBlocksToAdd);
	iCount++;
	}

// Completing the streamed encoding operation
void RStepStreamedEncodeBase::StreamedEncodingComplete()
	{
	iStreamEncode->Complete(iActiveListener->iStatus);		
	}

// Streamed encoding of a bitmap to JPEG format for neg tests
void RStepStreamedEncodeBase::StreamedEncodeNegTestL()
	{
	TSize frameSizeInPixels;
	//Altering the imageframe size at midstream, i.e after append some valid blocks
	if (iMidStream)
		{
		INFO_PRINTF3(_L("Appended %d blocks for %d iterations"), iNumBlocksToAdd,iIterateblocks-1);
		//passing invalid YUV format
		if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0120-HP")) == 0)
			{
			TFrameFormat frameFormat444 = TFrameFormat(KUidFormatYUV444Planar);
			INFO_PRINTF1(_L("Setting Frame Format to 444"));
			TRAPD(err, iImageFrame->SetFrameFormatL(frameFormat444));
			CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Setting Frame Format to 444 failed"));
			INFO_PRINTF1(_L("Setting Frame Layout"));
			TFrameLayout frameLayout(KMaxPlanesInFrame);
			TRAP(err, iImageFrame->SetFrameLayoutL(frameLayout));
			CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Setting Frame Layout failed"));
			}
		else
			{
			//setting imageframe size as 0
			if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0200-HP")) == 0)
				{
				frameSizeInPixels.iWidth = 0;
				frameSizeInPixels.iHeight = 0;
				iImageFrame->SetFrameSizeInPixels(frameSizeInPixels);
				}
			else
				{
				frameSizeInPixels.iWidth = iImageWidth;
				//setting imageframe block size too large
				if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0140-HP")) == 0)
					{
					frameSizeInPixels.iHeight = iImageHeight + 1;
					}
				else if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0160-HP")) == 0)
					{
					//setting imageframe block size too small
					frameSizeInPixels.iHeight = iImageHeight - 1;
					}
				else 
					{
					frameSizeInPixels.iHeight = iImageHeight;
					}
				iImageFrame->SetFrameSizeInPixels(frameSizeInPixels);
                    
				if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0180-HP")) == 0)
					{
					iNumBlocksToAdd = 50;
					}
				}				
			}				
		}
	else
		{
		//passing invalid imageframe size at the start of streamencoding
		INFO_PRINTF3(_L("Appended %d blocks for %d iterations"), iNumBlocksToAdd,iIterateblocks);
		frameSizeInPixels.iWidth = iImageWidth;
		if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0190-HP")) == 0)
			{
			frameSizeInPixels.iHeight = 0;
			}		
		else
			{
			frameSizeInPixels.iHeight = iImageHeight;
			}
		iImageFrame->SetFrameSizeInPixels(frameSizeInPixels);
		}
	iActiveListener->InitialiseActiveListener();
	iStreamEncode->AppendBlocks(iActiveListener->iStatus, *iImageFrame, iNumBlocksToAdd);		
	}

//initilise stream decoder 
void RStepStreamedEncodeBase::InitializeStreamDecodingL()
    {
    // Decode to an image frame
    TInt error = KErrNone;
    CreateImageDecoderL();
    //create streamdecode object                
    TRAP(error,	iStreamDecode = iImageDecoder->BlockStreamerL());
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not initialize block streamer"));

    //get the frame information of decoder
    iFrameInfo = iImageDecoder->FrameInfo();
    PrepareDecodeDataL();

    if (iStreammode == 1 || iStreammode == 3)		
        {
        iNumBlocksToGet = iNumBlocksToAdd;
        }
    TInt frameNum = 0;
    //Navigation mode within stream decoding.
    TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationSequentialForward;
    if (iStreammode == 2)
        {
        nav = TDecodeStreamCaps::ENavigationRandomForward;
        }
    if (iStreammode == 3)
        {
        nav = TDecodeStreamCaps::ENavigationRandomBackwards;
        }            
    //Initialise the stream.
    TRAP(error,	iStreamDecode->InitFrameL(iOptimalFormat, frameNum, nav));
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not initialize decoder"));
    return;        
    }

//stream decode the image and get the stream in imageframe     
void RStepStreamedEncodeBase::StreamDecodeJpegImageToImageFrameL()
	{
    // Decode to an image frame
    TInt error = KErrNone;
    TInt imageSizeInBytes;
    TSize streamBlockSizeInPixels;
    //get the buffer size to hold the returned data
    imageSizeInBytes = iStreamDecode->GetBufferSize(iOptimalFormat, streamBlockSizeInPixels, iNumBlocksToGet);	

    iBuffer = HBufC8::NewMaxL(imageSizeInBytes);
    TPtr8 ptr = iBuffer->Des();
    //Create the imageframe
    TRAP(error, iImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
    CHECK_STATUS_LEAVE_IF_ERROR(error, _L("Could not create image frame"));

    // set the size of imageframe in pixels to image to whcih it refers
    iImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

    iActiveListener->InitialiseActiveListener();    
	if (iStreammode == 1)
	    {
	    // Ger the blocks sequentialy from the stream and load in imageframe
	    iStreamDecode->GetNextBlocks(iActiveListener->iStatus, *iImageFrame, iNumBlocksToAdd, iNumBlocksRead, iHaveMoreBlocks);    
	    }
    else if (iStreammode == 2)
        {
        TFrameFormat frameFormat(iOptimalFormat);
        iImageFrame->SetFrameFormatL(frameFormat);
        // Get the blocks from stream and load in image frame by random forward stream decoding
        iStreamDecode->GetBlocks(iActiveListener->iStatus, *iImageFrame, iBlockPosition, iNumBlocksToGet, iNumBlocksRead);
        }
    else if (iStreammode == 3)
        {
        TFrameFormat frameFormat(iOptimalFormat);
        iImageFrame->SetFrameFormatL(frameFormat);
        // Get the blocks from stream and load in image frame by random backward stream decoding
        iStreamDecode->GetBlocks(iActiveListener->iStatus, *iImageFrame, (iIterateblocks*(iFrameInfo.iOverallSizeInPixels.iWidth/iBlockSizeInPixels.iWidth)) + 1, iNumBlocksToGet, iNumBlocksRead);
        }
	}
	
// Create decoder and create the imageframe to hold the decoded image
void RStepStreamedEncodeBase::DecodeJpegImageToImageFrameL()
	{
	// Decode to an image frame
	TInt error = KErrNone;
    CreateImageFrameDecoderL();
	// check that destination imageframe is large enough to hold the decoded frame
	// and return the bufferszie for imageframe
	TInt imageSizeInBytes;
	TBool supported = iJPEGImageDecoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0330-HP")) == 0 || iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0110-HP")) == 0)
			{
			ERR_PRINTF1(_L("Codec does not support this image format:4:4:4"));
			StopTest(EPass); 
			return;
			}
		ERR_PRINTF1(_L("Codec does not support this image format"));			
		User::Leave(KErrNotSupported); 
		}

	// create local destination RChunk
	TInt res = iChunk.CreateLocal(imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	User::LeaveIfError(res);
    //Create imageframe
	TRAP(error, iImageFrame = CImageFrame::NewL(&iChunk, imageSizeInBytes, KChunkDataOffset));
	CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create image frame"));
	
	//Covert the image and load in imageframe
    if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0190-HP")) != 0)
		{
		iActiveListener->InitialiseActiveListener();
		iJPEGImageDecoder->ConvertFrame(&iActiveListener->iStatus, *iImageFrame);
		}
	else
		{
		InitializeStreamedEncodingL();//initialise stream encoding
		StreamedEncoding();//append imageframe 
		iState = 1;
		}		
	}

// Initilise and stream decode to imageframe
void RStepStreamedEncodeBase::StreamDecodeFiles()
	{
	TInt err = KErrNone;
    delete iImageDecoder; iImageDecoder = NULL;
    iInputImage = iInputImageArray[iEncodedImageCount];
    INFO_PRINTF2(_L("File to be encoded (%S)"), &iInputImage);
     //initialize stream decoding
	TRAP(err,InitializeStreamDecodingL());
	CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Initialiser failed"));

    //stream decode to imageframe   		   	
	TRAP(err,StreamDecodeJpegImageToImageFrameL());
	CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Stream decoding failed"));
	}
	
//decode the image to imageframe		
void RStepStreamedEncodeBase::DecodeFiles()
	{
	TInt err = KErrNone;
	// if i/p filecounter has appended all files then complete stream encoding
	if (iEncodedImageCount >= iInputImageCount)
		{
		iActiveListener->InitialiseActiveListener();
		StreamedEncodingComplete();
		iState = 4;
		return;
		}
	delete iJPEGImageDecoder; iJPEGImageDecoder = NULL;		
	iInputImage = iInputImageArray[iEncodedImageCount];
	INFO_PRINTF2(_L("Input image : %S"), &iInputImage);
	
	//decode to imageframe
	
	TRAP(err,DecodeJpegImageToImageFrameL());
	CHECK_STATUS_LEAVE_IF_ERROR(err,_L("DecodeJpegImageToImageFrameL() left"));
	StateChange();
    }
	
// Start the tests		
void RStepStreamedEncodeBase::DoKickoffTestL()
    {
	User::LeaveIfError(ReadCommonInputFromConfigFile());
	
	// Create output directory
	User::LeaveIfError(CreateOutputDirectory());

	TFileName tmpname;
	tmpname.Append(iOutputPath);
	tmpname.Append(iOutputImage);
	iOutputImage.Copy(tmpname);

	iActiveListener = new (ELeave) CActiveListener(this);

    iOptions = static_cast<CImageEncoder::TOptions> (CImageEncoder::EOptionExtStream);

    SetThreadForEncoding(iOptions); // set the option if encoding is performed in a separate thread

    iMimeType.Copy(KMimeTypeJpeg);

	//Log the test case description based on the test case scenario
	INFO_PRINTF2(_L("%S"), &iTestCaseDescription);
	INFO_PRINTF2(_L("Output image : %S"), &iOutputImage);
	// start the state machine
	TInt res = KErrNone;
    TRAP(res, FsmL());
    if (res != KErrNone)
    	{
        StopTest(res, EFail);
        return;
    	}
    }

/*
*   Class for Stream encoding test cases
*   This uses normal decoding to get the imageframe to be appened while encoding
*   MM-ICL-ENC-STRM-I-0080-HP TO MM-ICL-ENC-STRM-I-0110-HP
*   MM-ICL-ENC-STRM-I-0210-HP TO MM-ICL-ENC-STRM-I-0310-HP
*/
RStepStreamedEncodeToImage* RStepStreamedEncodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedEncodeToImage(aStepName, aTestCaseDescription));
    }

RStepStreamedEncodeToImage::RStepStreamedEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepStreamedEncodeBase(aStepName, aTestCaseDescription)
    {
    }

void RStepStreamedEncodeToImage::FsmL()
	{
	TInt err = KErrNone;
    switch (iState)
        {
        case 0:// Decode State
			DecodeFiles();                                              //decode the image to imageframe
			break;

        case 1:// Check decode status and then stream encode
        	err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Decoding JPEG image to image frame failed"));

			INFO_PRINTF1(_L("Decoding JPEG image to image frame successful!"));
			if (!iEncodedImageCount)
				{
				InitializeStreamedEncodingL();               //initialise stream encoding
				}
			StreamedEncoding();	                                        //append imageframe 
            StateChange();
            break;
           
        case 2:// Check stream encode status and append imageframe for next iteration
        	err = iActiveListener->iStatus.Int();
        	CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Streamed encoding failed to append blocks"));
			INFO_PRINTF1(_L("Streamed Encoding appended blocks"));
			//if iterateblocks is > 0 then 
			//append the same imageframe for iterateblocks no of times
			if (iIterateblocks != 0)
			    {
     			if (iCount < iIterateblocks)
					{
					StreamedEncoding();                             //append imageframe 
					break;
					}
                //if still some space is left in the output image then append the same imageframe
				iRemainblocks = iTotalblocks - (iIterateblocks * iNumBlocksToAdd);
				if (iRemainblocks > 0)
					{
					StreamedEncoding();                             //append imageframe 
					StateChange();	
					}
				else
					{
					//complete strean encoding
					iActiveListener->InitialiseActiveListener();
					StreamedEncodingComplete();
					iState = 4;	
					}						
			    }
			else
			    {
			    //if iterateblocks is 0 then there will be more than 1 input file 
			    //so read the next i/p file and generate the decoded imageframe
				iState = 0;
		    	iEncodedImageCount++;
		    	delete iImageFrame; iImageFrame = NULL;
		    	DecodeFiles();                                      //decode the image to imageframe
			    }								
			break;	
						
		case 3:// Check stream encoding status and complete the stream 
		    //if iterateblocks is > 0 then check whether remaining blocks are appended 
			if (iIterateblocks != 0)	
				{
				//remaining blocks are not appended then report error
				if (iRemainblocks != 0)
					{
					err = iActiveListener->iStatus.Int();
                    CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Failed to append remaining blocks to outputimage"));
					INFO_PRINTF2(_L("Appended %d remaining blocks"), iRemainblocks);					
					}
                //if remaining blcoks are appened successfully then complete stream
		       	iActiveListener->InitialiseActiveListener();
		       	StreamedEncodingComplete();
	            StateChange();
		       	}
			else 
				{
				err = iActiveListener->iStatus.Int();
                CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Failed to append blocks to outputimage"));
				INFO_PRINTF2(_L("Appended %d blocks"), iNumBlocksToAdd);
				StopTest(EPass);		
				return;		
				}
			break;
			
		case 4:// check stream complete
		    //check the status of stream complete and pass the test
			err = iActiveListener->iStatus.Int();
			CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Streamed encoding failed to complete outputimage"));
		    INFO_PRINTF1(_L("Streamed Encoding completed"));
		    //compare the output jpeg with reference jpeg
		    CompareFileL();
		    StopTest(EPass);
			break;

       default:
       		ERR_PRINTF2(_L("Invalid state, %d"), iState);
       		User::Leave(KErrGeneral);        	
        }
	}
/*
*   Stream encoding for negative tests.
*   Passing inavlid blocks midstream or at the start
*   MM-ICL-ENC-STRM-I-0120-HP TO MM-ICL-ENC-STRM-I-0200-HP
*/	
RStepStreamedEncodeToImageNegTest* RStepStreamedEncodeToImageNegTest::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedEncodeToImageNegTest(aStepName, aTestCaseDescription));
    }

RStepStreamedEncodeToImageNegTest::RStepStreamedEncodeToImageNegTest(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepStreamedEncodeBase(aStepName, aTestCaseDescription)
    {
    }

//start the state machine
void RStepStreamedEncodeToImageNegTest::FsmL()
    {
    TInt err = KErrNone;
    switch (iState)
        {
        case 0:// Decode state
			DecodeFiles();                                  //decode the image to imageframe
			break;

        case 1://Check decode status and then stream encode
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Decoding JPEG image to image frame failed"));

			INFO_PRINTF1(_L("Decoding JPEG image to image frame successful!"));
			if (!iEncodedImageCount)
				{
				TRAP(err, InitializeStreamedEncodingL());               //initialise stream encoding
                if (err != KErrNone)
                    {
                    if ((iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0130-HP")) == 0 || 
                        iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0150-HP")) == 0 ||
                        iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0201-HP")) == 0) && 
                        (err == KErrNotSupported))
                        {
                        INFO_PRINTF2(_L("Could not initialize frame, expected error = %d"), err);
                        StopTest(err, EPass);
                        return;
                        }
					
					INFO_PRINTF2(_L("Could not initialize frame, unexpected error = %d"), err);
					StopTest(err, EFail);
					return;
    				}
				}
			StreamedEncoding();                             //append imageframe 
            StateChange();
            break;

        case 2://Check stream encoding append status
            //check the status of appending invalid blocks to image
        	err = iActiveListener->iStatus.Int();
			if (err != KErrNone)
				{
				if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0190-HP")) == 0)
					{
					ERR_PRINTF2(_L("Streamed encoding failed to Appended when pixel block is empty, returned with expected error = %d"), err);
					StopTest(err,EPass);
					return;	
					}
				ERR_PRINTF2(_L("Streamed encoding failed to Appended valid block, returned with error = %d"), err);
				StopTest(err,EFail);
				return;
				}
			else
				{
				//append invalid blocks midstream i.e after appending some valid blocks try appending invalid blocks
				if (iMidStream)
					{
					StreamedEncodeNegTestL();               // append invalid block
					StateChange();	
					}
				else
					{
					iActiveListener->InitialiseActiveListener();
		       		StreamedEncodingComplete();             // stream encoding complete
	            	StateChange();
					}					
				}
			break;
			
        case 3://check status of stream encoding invalid block midstream
             //check the status of appending invalid blocks midstream to image
        	err = iActiveListener->iStatus.Int();
			if (err != KErrNone)
				{
				if (iMidStream)
					{
					if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0120-HP")) == 0)
						{
						ERR_PRINTF2(_L("Streamed encoding failed to Append invalid block Midstream, returned with expected error = %d"), err);
						StopTest(EPass);
						return;
						}
					else 
						{
						if (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0200-HP")) == 0)
							{
							ERR_PRINTF2(_L("Streamed encoding failed to Appended when pixel block is empty, returned with expected error = %d"), err);	
							}
						else
							{
							ERR_PRINTF2(_L("Streamed encoding failed to Appended invalid block, returned with expected error = %d"), err);
							}
    					StopTest(EPass);
    					return;
						}
					}
				else
					{
					INFO_PRINTF2(_L("Streamed encoding failed to complete encoding with expected error : %d"),err);
					StopTest(err,EPass);
					return;
					}					
				}
			ERR_PRINTF1(_L("Streamed encoding completed with invalid blocks"));
			StopTest(EFail);
			break;
			
        default: 
        		ERR_PRINTF2(_L("Invalid state, %d"), iState);
        		User::Leave(KErrGeneral);
        }
    } 
    
/*
*	Stream encoding test cases for Realtime events
*   This uses stream decoding to get the iamgeframe and also the nextblocks to append
*   MM-ICL-ENC-STRM-I-0430-HP TO MM-ICL-ENC-STRM-I-0450-HP
*/
RStepStreamedEncodeToImageRealTimeTests* RStepStreamedEncodeToImageRealTimeTests::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepStreamedEncodeToImageRealTimeTests(aStepName, aTestCaseDescription));
    }

RStepStreamedEncodeToImageRealTimeTests::RStepStreamedEncodeToImageRealTimeTests(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepStreamedEncodeBase(aStepName, aTestCaseDescription)
    { 
    }

// Read input from the configuration file
TInt RStepStreamedEncodeToImageRealTimeTests::ReadInputFromConfigFile()
    {
    TInt time;
	if ((!GetBoolFromConfig(iSectionName,_L("convcanceltest"), iConvCancelTest)) ||
		(!GetIntFromConfig(iSectionName, _L("timerexpire"), time)) ||
		(!GetBoolFromConfig(iSectionName,_L("restartconvaftercancel"), iRestartConvAfterCancel)) ||
		(!GetIntFromConfig(iSectionName, _L("streammode"), iStreammode)))
    	{
		return KErrNotFound;
		}
	iTimerExpire = TTimeIntervalMicroSeconds32(time);		
	return KErrNone;		
    }
    
// Timer to cancel the encoding operation after some duration
// used for cancel encoding tests inbetween
void RStepStreamedEncodeToImageRealTimeTests::TimerExpired()
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

//start the state machine 
void RStepStreamedEncodeToImageRealTimeTests::FsmL()
    {
    TUint32 encodeCounter = 0;
    TInt err = KErrNone;
    switch (iState)
        {
        case 0:// Stream decode state
            User::LeaveIfError(ReadInputFromConfigFile());
            iTimer = CUtilityTimer::NewL(iTimerExpire, *this);
			StreamDecodeFiles();                        //initialise and streandecode the image to imageframe
			StateChange();
            break;
            
        case 1:// Check status of stream decode and then stream encode
            // check the status of stream decoding
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Stream Decoding JPEG image to image frame failed"));

			INFO_PRINTF1(_L("Stream Decoding JPEG image to image frame successful!"));
			if (!iEncodedImageCount)
				{
				InitializeStreamedEncodingL();          //initialise stream encoding
				}
			StreamedEncoding();                         //append imageframe 
            StateChange();
            break;
            
        case 2:// Check status of stream encoding cancelled and then restart stream encoding if iRestartConvAfterCancel = true
        	iEndEncodeCounter = User::FastCounter();
        	//confirm that its has at least encoded some  part of the stream
        	encodeCounter = iEndEncodeCounter - iBeginEncodeCounter;
        	iActiveListener->Cancel();
			// if stream encoding is not cancelled then iConvCancelled = false
			if (!iConvCancelled)
				{
				INFO_PRINTF2(_L("Cancel Streamed Encoding unsuccessful with err %d"), err);
				StopTest(EFail);
				return;
				}
            //iConvCancelled = true and stream encoding cancelled after some time interval
			INFO_PRINTF2(_L("Image Encoded, %d counters since the start"), encodeCounter );
			INFO_PRINTF1(_L("Cancelled Streamed Encoding successfully"));
			// if iRestartConvAfterCancel =  false then stop the test
			if (!iRestartConvAfterCancel)
				{
				StopTest(EPass);
				return;
				}
            // if iRestartConvAfterCancel =  true then restart the test after cancellation				
            delete iImageEncoder; iImageEncoder = NULL;
            delete iFrameEnImageData; iFrameEnImageData = NULL;
			if (!iEncodedImageCount)
				{
				InitializeStreamedEncodingL();          //initialise stream encoding
				}
            INFO_PRINTF1(_L("Stream Encoding with new encoder!"));
            //Append blocks to the stream
        	iActiveListener->InitialiseActiveListener();
            iStreamEncode->AppendBlocks(iActiveListener->iStatus, *iImageFrame, iNumBlocksToAdd);
			StateChange();
            break;
            
        case 3://check status of stream encoding and get next blocks to append
            //check the status of stream encoding
        	err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Streamed encoding failed to append blocks"));
			
			INFO_PRINTF1(_L("Streamed Encoding appended blocks"));
            //get next blocks from stream decode if iHaveMoreblocks = true
            if (iIterateblocks > 0)
                {
                if (iHaveMoreBlocks != EFalse || iStreammode == 3)
                	{
                	delete iBuffer; iBuffer = NULL;
			    	delete iImageFrame; iImageFrame = NULL;
			    	iEncodedImageCount++;
                	StreamDecodeJpegImageToImageFrameL();
                	iState = 1;
                	}
                }
            else
                {
                iActiveListener->InitialiseActiveListener();
                StreamedEncodingComplete();             // stream encoding complete
                StateChange();                        
                }                        
			break;

        case 4:// check status of stream complete
			err = iActiveListener->iStatus.Int();
			CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Streamed encoding failed to complete outputimage"));
		    INFO_PRINTF1(_L("Streamed Encoding completed"));
		    //compare the output jpeg with reference jpeg
		    CompareFileL();
		    StopTest(EPass);
			break;
			
        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }
