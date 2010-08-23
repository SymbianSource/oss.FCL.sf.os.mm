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
// This file contains test step base class implementation for image encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


// Test user includes
#include "tsi_icl_encode_stepbase.h"


// Active Listener
CActiveListener::CActiveListener(RStepBaseImageEncoder* aBase) : CActive(CActive::EPriorityIdle)
    {
    iBase = aBase;
    CActiveScheduler::Add(this);
    }

void CActiveListener::InitialiseActiveListener()
    {
    iStatus = KRequestPending;
    SetActive();
    }

void CActiveListener::RunL()
    {
    iBase->FsmL(); // calls test step's FsmL()
    }

void CActiveListener::DoCancel()
    {
    }


// CUtilityTimer
CUtilityTimer* CUtilityTimer::NewL(TTimeIntervalMicroSeconds32 aDelay, MTimerObserver& aTimerObserver)
    {
    CUtilityTimer* self = new (ELeave) CUtilityTimer(aTimerObserver);
    CleanupStack::PushL(self);
    self->ConstructL(aDelay);
    CleanupStack::Pop(self);
    return self;
    }

CUtilityTimer::CUtilityTimer(MTimerObserver& aTimerObserver) :
	CTimer(CActive::EPriorityUserInput), iTimerObserver(aTimerObserver)
	{
    }

void CUtilityTimer::ConstructL(TTimeIntervalMicroSeconds32& aDelay)
    {
    CTimer::ConstructL();

    iTimerDelay = aDelay;
    CActiveScheduler::Add(this);
    }

CUtilityTimer::~CUtilityTimer()
    {
    Cancel();
    }

void CUtilityTimer::InitializeTimer()
    {
	// Request another wait - assume not active
	CTimer::After(iTimerDelay);
    }

void CUtilityTimer::RunL()
    {
	if (iStatus.Int() == KErrNone)
	    {
	    iTimerObserver.TimerExpired();
	    }
    }

void CUtilityTimer::DoCancel()
	{
	}


// Constructor
RStepBaseImageEncoder::RStepBaseImageEncoder(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    iTestStepName = aStepName;
	iTestCaseDescription = aTestCaseDescription;
	iClearedstack = EFalse;
	iBitmaploaded = EFalse;
	iConvCancelTest = EFalse;
	iHeapSize = 7000000;
    }

/* 
 *
 * Bitwise compare two file with buffer 
 *
 */
void RStepBaseImageEncoder::CompareFileL()
	{
    #if defined (__EABI__) || defined (__X86GCC__)
        // for hw test run, comparison of output and reference image is not done, 
        // this is due to lack of disk space for output and reference images
        return;
    #else
    INFO_PRINTF2(_L("Comparing output image with the reference image %S"),&iReferenceImage);
	TInt err = KErrNone;

	TInt outFileSize = 0;
	User::LeaveIfError(iFile.Open(iFs, iOutputImage, EFileRead));
	iFile.Size(outFileSize);
	HBufC8 *outImage = HBufC8::NewMaxLC(outFileSize);
	TPtr8 outImagePtr = outImage->Des();
	iFile.Read(outImagePtr);	
	iFile.Close();

    TInt refFileSize = 0;
    User::LeaveIfError(iFile.Open(iFs, iReferenceImage, EFileRead));
	iFile.Size(refFileSize);
	HBufC8 *refImage = HBufC8::NewMaxL(refFileSize);
    TPtr8 refImagePtr = refImage->Des();
    iFile.Read(refImagePtr);
    iFile.Close();

	if (outFileSize == refFileSize)
		{	
		err = Mem::Compare(outImagePtr.Ptr(), outFileSize, refImagePtr.Ptr(), refFileSize);
		if (err != KErrNone)
		    {
		    INFO_PRINTF1(_L("Files are not identical"));
		    err = KErrGeneral;
		    }
		else
			{
			INFO_PRINTF1(_L("Files are identical"));
    		}
		}
    else
        {
        INFO_PRINTF1(_L("File sizes are not identical"));
        err = KErrGeneral;
        }

    iFs.Close();
    CleanupStack::PopAndDestroy(1, outImage); // outImage, 
    delete refImage;

    if (err != KErrNone)
        {
        User::Leave(err);
        }
    
	#endif // __EABI__,__X86GCC__
	}
	
// Check performance based on time and log the result; also, compare and save the output
void RStepBaseImageEncoder::CheckPerformanceOutputAndSaveL()
    {
    TInt64 actualEncodeTime = I64INT(iEndTime.MicroSecondsFrom(iStartTime).Int64());
    INFO_PRINTF2(_L("Actual encode time: %d microS"), I64INT(actualEncodeTime));
    
    if (iExpectedTime <= 0)
        {
        ERR_PRINTF1(_L("Expected performance time incorrect"));
        User::Leave(KErrGeneral);
        }
/*    TInt64 scaledTime = ScaleTimeByMachineL(iExpectedTime);
    if (scaledTime < actualEncodeTime)
        {
        INFO_PRINTF2(_L("Desired encode time exceeded by %d microS"), I64INT(actualEncodeTime - scaledTime));
        #ifdef __WINS__
            INFO_PRINTF1(_L("WARNING: That may mean performance degradation"));
            INFO_PRINTF1(_L("         Check this test on a target hardware!"));
        #else
            ERR_PRINTF1(_L("ERROR: Performance degradation has been detected"));
            User::Leave(KErrGeneral);
        #endif
        }*/

    }
    
// Get scale execution time according to the current hardware. Currently supported: H2, H4 and emulator
TInt64 RStepBaseImageEncoder::ScaleTimeByMachineL(TInt64 aExecTime, TInt aDestMedia)
    {
    TInt value;
    User::LeaveIfError(HAL::Get(HAL::EMachineUid, value));
    TReal mulCoef = 1.0;

    switch (value)
        {
        case HAL::EMachineUid_OmapH2:
            mulCoef = 1.0;
            break;

        case HAL::EMachineUid_OmapH4:
            mulCoef = 0.7; // H2/H4 coef is in range 0.55<->0.66, so use 0.7
            break;

        case HAL::EMachineUid_Win32Emulator:
            mulCoef = 1.0;
            break;
            
        case HAL::EMachineUid_NE1_TB:
            mulCoef = 0.7;
            break;

        default:
            {
            User::Leave(KErrGeneral);
            // we're running on unknown hardware, we can't scale execution time
            // this test must be adjusted for that new hardware
            }
        }

    // Demand paging affects decode speed so account for it when measuring performance.
    // Allow another 10% to the encode/decode time.
    // Also, tests that access the MMC card need even more time.
    TUint32 flags = DPTest::ERomPaging | DPTest::ECodePaging | DPTest::EDataPaging;
    TBool demandpaging = (DPTest::Attributes() & flags);
    if (demandpaging)
        {
        if (aDestMedia == EDstMediaMMCDrive)
            {
            mulCoef *= 2.0;
            }
        else
            {
            mulCoef *= 1.9;
            }
        }

    return TInt64(mulCoef * TReal(aExecTime));
    }

/*
 * RetainEncodedImage()
 * Save the output of the encoded image (after all operations are done) to a file.
 * Retain images only in winscw test run and not on hw. 
 * So, for hw, image will be deleted after the test is complete.
 * But option is provided if the image is required to be retained.
 * This function is called from CloseTest to avoid the multiple calls in the FsmL()
 */
void RStepBaseImageEncoder::RetainEncodedImage()
    {
    TInt err = KErrNone;
    // retain the output, iHwRetainOutputImage: true => retain, false => do not retain
    #ifdef __EABI__
        if (!iHwRetainOutputImage)
          	{
    		delete iExifEncoder; iExifEncoder = NULL;
    	    delete iImageEncoder; iImageEncoder = NULL;
    	    delete iImageDecoder; iImageDecoder = NULL;
    	    delete iJPEGImageEncoder; iJPEGImageEncoder = NULL;
    		delete iJPEGImageDecoder; iJPEGImageDecoder = NULL;
                    		     	
           	err = iFs.Delete(DestFilename());
           	if (err != KErrNone && err != KErrNotFound)
    			{
    			ERR_PRINTF2(_L("Unable to delete output image, error = %d"), err);
    			StopTest(err, EFail);
    			return;
    			}
			INFO_PRINTF1(_L("Output image deleted "));
          	}
    #endif    
    }

/*
 *	Getting the encoder options which can be used while loading codec
 *
 */     
TInt RStepBaseImageEncoder::GetEncodeOptions()
    {
    switch (iOptionsIndex)
        {
        //	options to specify how the image should be encoded
        case 1: iOptions = CImageEncoder::EOptionNone; break;
        case 2: iOptions = CImageEncoder::EOptionAlwaysThread; break;
        case 3: iOptions = CImageEncoder::EOptionGenerateAdaptivePalette; break;

        // following options specify the extension interfaces
        case 4: iOptions = CImageEncoder::EOptionExtStream;
				break;

        case 5: iOptions = CImageEncoder::EOptionExtRotation;
          		break;

        case 6: iOptions = CImageEncoder::EOptionExtMirrorHorizontalAxis; 
        		break;

        case 7: iOptions = CImageEncoder::EOptionExtMirrorVerticalAxis;
        		break;

        case 8: iOptions = CImageEncoder::EOptionExtUseWithSetThumbnail;
        		break;

        case 9: iOptions = CImageEncoder::EOptionExtReserved1; 
        		 break;

        case 10: iOptions = CImageEncoder::EOptionExtReserved2; 
         		 break;
        
        case 11: iOptions = CImageEncoder::EOptionExtReserved3; 
        		 break;

		// performing multiple operations
		case 30: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtMirrorHorizontalAxis | CImageEncoder::EOptionExtMirrorVerticalAxis );
			 	 break;

		case 31: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtMirrorHorizontalAxis | CImageEncoder::EOptionExtMirrorVerticalAxis | CImageEncoder::EOptionExtUseWithSetThumbnail);
				 break;

		case 32: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtRotation | CImageEncoder::EOptionExtUseWithSetThumbnail);
				 break;

		case 33: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtRotation | CImageEncoder::EOptionExtMirrorHorizontalAxis | CImageEncoder::EOptionExtMirrorVerticalAxis );
				 break;

        case 34: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtStream | CImageEncoder::EOptionExtRotation );
				 break;

        case 35: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtStream | CImageEncoder::EOptionExtUseWithSetThumbnail );
				 break;

        case 36: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtStream | CImageEncoder::EOptionExtMirrorHorizontalAxis );
				 break;

        case 37: iOptions = CImageEncoder::TOptions(CImageEncoder::EOptionExtMirrorVerticalAxis | CImageEncoder::EOptionExtReserved1);
        		break;						 				 

        default: ERR_PRINTF2(_L("Invalid value for CImageEncoder::TOptions, %d"), iOptionsIndex);
        		return KErrGeneral;
        }
	return KErrNone;        
    }


// Get actual rotation/mirroring/flip values, from the values read from config file
TInt RStepBaseImageEncoder::GetMirrorAngle(CBitmapRotator::TRotationAngle& aAngle)
    {
			    	
    switch (iIndex)
        {
        case 0: aAngle = CBitmapRotator::ERotation90DegreesClockwise; 
        		break;
        case 1: aAngle = CBitmapRotator::ERotation180DegreesClockwise; 
        		break;
        case 2: aAngle = CBitmapRotator::ERotation270DegreesClockwise; 
        		break;
        case 3: aAngle = CBitmapRotator::EMirrorHorizontalAxis; 
        		break;
        case 4: aAngle = CBitmapRotator::EMirrorVerticalAxis; 
        		break;
        
        default: ERR_PRINTF2(_L("Invalid value for angle of rotation/mirroring/flipping, %d"), iMirroringIndex);
        		 return KErrGeneral;
        }
	return KErrNone;
    }

// Get the destination filename
TFileName& RStepBaseImageEncoder::DestFilename()
	{
	return iOutputImage;
	}

// To create the ouput directory as jpg images will be saved in
// jpg folder and mbm in mbm folder of output directory	
TInt RStepBaseImageEncoder::CreateOutputDirectory()
	{
	TInt err = iFs.MkDirAll(iOutputPath);
	if ((err != KErrAlreadyExists) && (err != KErrNone))
		{
		ERR_PRINTF3(_L("Cannot create output directory %S, error = %d"), &iOutputPath, err);
		StopTest(err, EFail);
		return err;
		}
	return KErrNone;
	}

/* 
 * Prepare encode data
 * applying coloursampling and quality factor
 * 
 */
void RStepBaseImageEncoder::PrepareEncodeDataL()
	{
	// read config file
	iFrameEnImageData = CFrameImageData::NewL();	
	TInt qualityfactor = KDefaultQualityFactor;
	TInt sampling = KDefaultSampling;
	if ((!GetIntFromConfig(iSectionName, _L("qualityfactor"), qualityfactor)) || 
	   (!GetIntFromConfig(iSectionName, _L("colorsampling"), sampling)))
        {
        User::Leave(KErrNotFound);
		}

	TJpegImageData* data = new (ELeave) TJpegImageData;
	data->iSampleScheme = TJpegImageData::TColorSampling(sampling);
	data->iQualityFactor = qualityfactor; 
	User::LeaveIfError(iFrameEnImageData->AppendImageData(data));
	switch (data->iSampleScheme)
		{
		case TJpegImageData::EColor422:
			iOptimalFormat = KUidFormatYUV422Interleaved;
			break;
		case TJpegImageData::EColor420:
			iOptimalFormat = KUidFormatYUV420Planar;
			break;
		case TJpegImageData::EMonochrome:
			iOptimalFormat = KUidFormatYUVMonochrome;
			break;
		case TJpegImageData::EColor444:
			iOptimalFormat = KUidFormatYUV444Planar;
			break;
		default:
			ERR_PRINTF1(_L("Unsupported streaming format requested."));
			User::Leave(KErrNotSupported);
		};
	
	if (iStreamEncode)
		{
		// Verify that the required sampling scheme is supported by the returned format(s)
		RArray<TUid> supportedFormats;
		CleanupClosePushL(supportedFormats);
		TUid optimalFormat;
		
		iStreamEncode->GetSupportedFormatsL(supportedFormats, optimalFormat);
	
		TInt index = supportedFormats.Find(iOptimalFormat);
		if (index < 0)
			{
			ERR_PRINTF1(_L("GetSupportedFormatsL() did not return required format."));
			User::Leave(KErrGeneral);
			}
		CleanupStack::PopAndDestroy(&supportedFormats);
		
		// Set up block size
		TEncodeStreamCaps streamCapabilities;
		iStreamEncode->GetCapabilities(iOptimalFormat, streamCapabilities);
		iBlockSizeInPixels = streamCapabilities.MinBlockSizeInPixels();
		}
	}
	
/* 
 * Prepare decode data
 * applying coloursampling and quality factor
 * 
 */
void RStepBaseImageEncoder::PrepareDecodeDataL()
	{
	// read config file
	iFrameDeImageData = CFrameImageData::NewL();	
	TInt qualityfactor = KDefaultQualityFactor;
	TInt sampling = KDefaultSampling;
	if ((!GetIntFromConfig(iSectionName, _L("qualityfactor"), qualityfactor)) || 
	   (!GetIntFromConfig(iSectionName, _L("colorsampling"), sampling)))
        {
        User::Leave(KErrNotFound);
		}

	TJpegImageData* data = new (ELeave) TJpegImageData;
	data->iSampleScheme = TJpegImageData::TColorSampling(sampling);
	data->iQualityFactor = qualityfactor; 
	User::LeaveIfError(iFrameDeImageData->AppendImageData(data));
	if (data->iSampleScheme == TJpegImageData::EColor422)
		{
		iOptimalFormat = KUidFormatYUV422Interleaved;
		iBlockSizeInPixels = TSize(16,8);
		}
	else if (data->iSampleScheme == TJpegImageData::EColor420)
		{
		iOptimalFormat = KUidFormatYUV420PlanarReversed;
		iBlockSizeInPixels = TSize(16,16);
		}
	else if (data->iSampleScheme == TJpegImageData::EMonochrome || data->iSampleScheme == TJpegImageData::EColor444)
		{
		if (data->iSampleScheme == TJpegImageData::EColor444)
			{
			iOptimalFormat = KUidFormatYUV444Planar;	
			}
		else
			{
			iOptimalFormat = KUidFormatYUVMonochrome;	
			}
		iBlockSizeInPixels = TSize(8,8);
		}			
	}

// Create image decoder
void RStepBaseImageEncoder::CreateImageDecoderL()
    {
    TInt error = KErrNone;
    // Create an image decoder
	if (iWriteExif)
		{
		TRAP(error, iImageDecoder = (CJPEGExifDecoder::FileNewL(iFs, iInputImage, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));	
		}
	else
		{
		TRAP(error, iImageDecoder = (CImageDecoder::FileNewL(iFs, iInputImage, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));	
		}
    
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create image frame decoder"));
    }

// Create image encoder
void RStepBaseImageEncoder::CreateImageEncoderL()
    {
    TInt error = KErrNone;
    
	if (iWriteExif)
		{
		TRAP(error, iExifEncoder = static_cast<CJPEGExifEncoder*>(CImageEncoder::FileNewL( iFs, iOutputImage, iOptions, KNullUid, KNullUid, KImageFramePluginUid)));	
		}
	else
		{
		TRAP(error, iImageEncoder = (CImageEncoder::FileNewL(iFs, iOutputImage, iOptions, KNullUid, KNullUid, KImageFramePluginUid)));
		}
    
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create image frame encoder"));
    }

// Create image frame Decoder
void RStepBaseImageEncoder::CreateImageFrameDecoderL()
    {
	TInt error = KErrNone;;
	TRAP(error, iJPEGImageDecoder = static_cast<CJPEGImageFrameDecoder*> (CImageDecoder::FileNewL(iFs, iInputImage, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
	
	CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create image frame decoder"));    
    }

// Create image frame Encoder
void RStepBaseImageEncoder::CreateImageFrameEncoderL()
    {
	TInt error = KErrNone;;
	TRAP(error, iJPEGImageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, iOutputImage, iOptions, KNullUid, KNullUid, KImageFramePluginUid)));
	
	CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create image frame encoder"));    
    }

// Set the thread for encoding - client OR separate thread; can be combined with other options
void RStepBaseImageEncoder::SetThreadForEncoding(CImageEncoder::TOptions& aOptions)
    {
    if (iAlwaysThread) // true (1) => separate thread, false (0) => client thread
        {
        aOptions = static_cast<CImageEncoder::TOptions> (aOptions | CImageEncoder::EOptionAlwaysThread);
        }
    }
    
// Add the extension interface operations to encoder
void RStepBaseImageEncoder::AddOperationL()
	{
	TInt err = KErrNone;
	TImageConvOperation* operation = NULL;
	if (iJPEGImageEncoder)
		{
		TRAP(err, operation = iJPEGImageEncoder->OperationL());
		CHECK_STATUS_LEAVE_IF_ERROR(err,_L("OpeartionL failed to initialize encoder"));
		}
	if (!iWriteExif && iImageEncoder)
		{
		TRAP(err, operation = iImageEncoder->OperationL());
		CHECK_STATUS_LEAVE_IF_ERROR(err,_L("OpeartionL failed to initialize encoder"));
		}
	else if (iExifEncoder)
		{
		TRAP(err, operation = iExifEncoder->OperationL());
		CHECK_STATUS_LEAVE_IF_ERROR(err,_L("OpeartionL failed to initialize encoder"));
		}

	INFO_PRINTF1(_L("OperationL() succeeded"));
	switch(iOperationIndex)
		{
		// single operation added
		case 1:
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				break;
		case 2:
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 3:
				operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
				break;
		case 4:
			    operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
			    break;
		case 5:
				operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				break;
		
		// Multiple combinations of opertaions supported
		case 30:
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 31:
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				//Set thumbnail
				break;
		case 32:
				operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				break;
		case 33:
				operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				break;
		case 34:
				operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				break;
		case 35:
				operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 36:
				operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 37:
				operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 38:
				operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 39:
				operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 40:
				operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
				operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
				break;
		case 41:
				operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
				break;				
		case 42:
				operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
				operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				break;

		default:
                ERR_PRINTF2(_L("Operation Index not supported, %d"), iOperationIndex);
                User::Leave(KErrGeneral);
		}
    if (iClearOperations && !iClearedstack)    
        {
        operation->ClearOperationStack();
        INFO_PRINTF1(_L("Removed all operations previously set from the stack"));
        iClearedstack = ETrue;
        }
	}


TUid RStepBaseImageEncoder::ImageType()
	{
	TUid imageType = KNullUid;
	TParse parse;
	parse.Set(iInputImage,0,0);
	if (parse.Ext().CompareF(_L(".BMP")) == 0)
		{
		imageType.iUid = KImageTypeBMPUid.iUid;			
		}
	else if (parse.Ext().CompareF(_L(".JPG")) == 0)
		{
		imageType.iUid = KImageTypeJPGUid.iUid;			
		}
	else if (parse.Ext().CompareF(_L(".MBM")) == 0)
		{
		imageType.iUid = KImageTypeMBMUid.iUid;		
		}
	else if (parse.Ext().CompareF(_L(".PNG")) == 0)
		{
		imageType.iUid = KImageTypePNGUid.iUid;		
		}
	else if (parse.Ext().CompareF(_L(".TIF")) == 0)
		{
		imageType.iUid = KImageTypeTIFFUid.iUid;		
		}
	else if (parse.Ext().CompareF(_L(".GIF")) == 0)
		{
		imageType.iUid = KImageTypeGIFUid.iUid;			
		}
	else if (parse.Ext().CompareF(_L(".WMF")) == 0)
		{
		imageType.iUid = KImageTypeWMFUid.iUid;			
		}
	else if (parse.Ext().CompareF(_L(".ICO")) == 0)
		{
		imageType.iUid = KImageTypeICOUid.iUid;		
		}
	return imageType;
	}

// Set the EXIF data for the encoded image
TInt RStepBaseImageEncoder::SetExifAndThumbnailData(MExifMetadata* aMetadata)
	{
	// Set the EXIF data
	TInt testResult = Set16bitBuffer(aMetadata);
	if (testResult != KErrNone)
		{
		return testResult;
		}
	else
		{
		testResult = SetModelVersion(aMetadata);
		if (testResult != KErrNone)
			{
			return testResult;
			}
		else
			{
			testResult = SetShort(aMetadata);
			if (testResult != KErrNone)
				{
				return testResult;
				}
			}
		}
	return testResult;
	}

// Set YCbCrPositioning and ColorSpace tag of the EXIF data
TInt RStepBaseImageEncoder::SetShort(MExifMetadata* aMetaData)
	{
	const TUint KTagID = 0xA001;
	const TUint16 KShortTestResult = 1;

	// Set the EXIF data Color Space in the encoder
	TInt testResult = aMetaData->SetShortParam(KTagID, 0, KShortTestResult);
	if (testResult != KErrNone)
		{
		return testResult;
		}
	else
		{
		// Set the EXIF data YCbCrPositioning in the encoder
		const TUint KTagID1 = 0x0213;
		testResult = aMetaData->SetShortParam(KTagID1, 0, KShortTestResult);
		if (testResult != KErrNone)
			{
			return testResult;
			}
		}
	return testResult;
	}

// Set user comment tag of the EXIF data
TInt RStepBaseImageEncoder::Set16bitBuffer(MExifMetadata* aMetaData)
	{
	const TUint KTagID = 0x9286;
	_LIT16(KUserCommentTestResult, "This is a Symbian JPEG Image");

	// Set the EXIF Data UserCommnet in the encoder
	HBufC16* buffer = KUserCommentTestResult().Alloc();
	if (buffer == NULL)
		{
		return KErrGeneral;
		}
	TInt testResult = aMetaData->SetParam16(KTagID, 0, buffer);
	delete buffer;
	return testResult;
	}

// Set Model Version for the EXIF data	
TInt RStepBaseImageEncoder::SetModelVersion(MExifMetadata* aMetaData)
	{
	const TUint KTagID = 0x0110;
	_LIT8(KModelTestResult, "Symbian OS v9.5");

	// Set the EXIF data Model in the encoder
	HBufC8* buffer = KModelTestResult().Alloc();
	if (buffer == NULL)
		{
		return KErrGeneral;
		}
	TInt testResult = aMetaData->SetParam8(KTagID, 0, buffer);
	delete buffer;
	return testResult;
	}

// Log test case description for each test
void RStepBaseImageEncoder::LogTestCaseDescription()
    {
    INFO_PRINTF2(_L("%S"), &iTestCaseDescription);
    INFO_PRINTF2(_L("Input image: %S"), &iInputImage);
    INFO_PRINTF2(_L("Output image :%S"), &iOutputImage);
    }

// Change the state to next
void RStepBaseImageEncoder::StateChange()
    {
    iState++;
    }
    
// Start the test
void RStepBaseImageEncoder::KickoffTestL()
	{
	__MM_HEAP_MARK	

	User::LeaveIfError(iFs.Connect());
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	// derived test step class to implement
	DoKickoffTestL();	
	}

// Cleanup at the end
void RStepBaseImageEncoder::CloseTest()
    {
    // retain the output
    RetainEncodedImage();
    
    delete iSrcBitmap; 
    delete iRefBitmap; 
    delete iBmpRotator;
	delete iImageFrame;
    delete iFrameEnImageData;
    delete iFrameDeImageData;
    delete iActiveListener;
	delete iJPEGImageDecoder;
	delete iBuffer;
	delete iExifEncoder;
  	delete iJPEGImageEncoder;
	delete iImageDecoder;
	delete iTimer;
    delete iImageEncoder;

	iInputImageArray.Close();
	iPropertiesArray.Close();
    RFbsSession::Disconnect();
    iFs.Close();
    __MM_HEAP_MARKEND
    }
