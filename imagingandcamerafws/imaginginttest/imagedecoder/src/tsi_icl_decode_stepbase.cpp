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
// This file contains test step base class implementation for image decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_stepbase.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

using namespace ContentAccess;
const TInt KOptimalFormat = 0;

// CActiveListener
CActiveListener::CActiveListener(RStepBaseImageDecoder* aBase) : CActive(CActive::EPriorityIdle)
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
    iBase->FsmL(); // calls derived test step's FsmL() for changed state
    }

void CActiveListener::DoCancel()
    {
    }


// CUtilityTimer
CUtilityTimer* CUtilityTimer::NewL(TTimeIntervalMicroSeconds32 aTimerDelay, MTimerObserver& aTimerObserver)
    {
    CUtilityTimer* self = new (ELeave) CUtilityTimer(aTimerObserver);
    CleanupStack::PushL(self);
    self->ConstructL(aTimerDelay);
    CleanupStack::Pop(self);
    return self;
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

CUtilityTimer::CUtilityTimer(MTimerObserver& aTimerObserver) :
    CTimer(CActive::EPriorityHigh), iTimerObserver(aTimerObserver)
    {
    }

void CUtilityTimer::ConstructL(TTimeIntervalMicroSeconds32& aTimerDelay)
    {
    CTimer::ConstructL();

    iTimerDelay = aTimerDelay;
    CActiveScheduler::Add(this);
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


// CUtilityMembers
CUtilityMembers::CUtilityMembers() : iSourceDataPtr(NULL,0,0)
    {
    /* Following members will be set to these values as it is a C class
    iState = 0;

    iExtPerfTest = EFalse;
    iNegativeTest = EFalse;
    iConvCancelTest = EFalse;

    iEnInitialiser = EFalse;
    iHwRetainOutputImage = EFalse;

    iScalerExtUseDesiredSizeMethod = EFalse;

    iDeOptions = CImageDecoder::EOptionNone;
    iEnOptions = CImageEncoder::EOptionNone;
    */

    iFrameDisplayModeIndex = 8; // for tests which use default display mode EColor16M
    iDecUid = 0;
    iDecoderUid.iUid = 0;
    }


CUtilityMembers::~CUtilityMembers()
    {
    delete iImageDecoder;
    delete iActiveListener;
    delete iFrameImageData;
    delete iImageFrameDecoder;
    delete iImageFrameEncoder;
    delete iDestMaskBitmap;
    delete iDestBitmap;
    delete iSrcBitmap;
    delete iBmpRotator;
    delete iBmpScaler;
    delete iTimer;
    delete iBuffer;
    delete iImageFrame;
    delete iExifImageEncoder;
    delete iImageEncoder;
	delete iSourceData;

    iRepeatCropSizeArray.Close();
    iRepeatCropPointAArray.Close();
    iRepeatCropPointBArray.Close();
    iRepeatCropOutputImageArray.Close();
    iRepeatCropReferenceImageArray.Close();

    iPropertiesArray.Close();
    iBlockPosArray.Close();
    iFile.Close();
    }


// RStepBaseImageDecoder
RStepBaseImageDecoder::RStepBaseImageDecoder(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    iTestStepName = aStepName;
    iTestCaseDescription = aTestCaseDescription;

    iYuvTest = EFalse;
    iScaleTest = EFalse;

    iHeapSize = 5000000; // 5MB
    }

// Read inputs from the configuration file
TInt RStepBaseImageDecoder::ReadCommonInputImageFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("inputimage"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iInputImage.Append(tmpKeyValue);
    return KErrNone;
    }


TInt RStepBaseImageDecoder::ReadCommonScalingInputFromConfigFile()
    {
    if (!GetIntFromConfig(iSectionName, _L("scalingform"), iUM->iScalingForm))
        {
        return KErrNotFound;
        }

    if (iUM->iScalingForm == KScalingFormReductionFactor) // form is used when reduction factor is specified
        {
        if (!GetIntFromConfig(iSectionName, _L("scalereductionfactor"), iUM->iReductionFactor))
            {
            return KErrNotFound;
            }
        }
    else // form KScalingFormDesiredSize is used when destination width and height are specified
        {
        if ((!GetIntFromConfig(iSectionName, _L("desiredscalewidth"), iUM->iScaleSize.iWidth)) || 
            (!GetIntFromConfig(iSectionName, _L("desiredscaleheight"), iUM->iScaleSize.iHeight)))
            {
            return KErrNotFound;
            }
        }
    return KErrNone;
    }


TInt RStepBaseImageDecoder::ReadCommonExtensionScalingInputFromConfigFile()
    {
    if ((!GetBoolFromConfig(iSectionName, _L("scalerextusedesiredsizemethod"), iUM->iScalerExtUseDesiredSizeMethod)) || 
        (!GetIntFromConfig(iSectionName, _L("scalingqualityindex"), iUM->iScalingQualityIndex)))
        {
        return KErrNotFound;
        }

    if (iUM->iScalerExtUseDesiredSizeMethod) // true (1)
        {
        if ((!GetIntFromConfig(iSectionName, _L("desiredscalewidth"), iUM->iExtScaleSize.iWidth)) || 
            (!GetIntFromConfig(iSectionName, _L("desiredscaleheight"), iUM->iExtScaleSize.iHeight)) || 
            (!GetBoolFromConfig(iSectionName, _L("lockaspectratio"), iUM->iLockAspectRatio)))
            {
            return KErrNotFound;
            }
        }
    else
        {
        // full size = -1, ½ size = -2, ¼ size = -3, 1/8 size = -4
        if (!GetIntFromConfig(iSectionName, _L("scalingcoeff"), iUM->iScalingCoeff))
            {
            return KErrNotFound;
            }
        }
    return KErrNone;
    }


TInt RStepBaseImageDecoder::ReadCommonInputExpectedPerfTimeFromConfigFile()
    {
    // this time is in microseconds for H4 board
    if (!GetIntFromConfig(iSectionName, _L("expecteddecodetime"), iUM->iExpectedDecodeTime))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }


TInt RStepBaseImageDecoder::ReadCommonInputRefAndOutputImagesFromConfigFile()
    {
    // last 7 chars of the test case number is the name of the section in the configuration file
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("referenceimage"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iReferenceImage.Append(tmpKeyValue);

    if ((!GetBoolFromConfig(iSectionName, _L("hwsaveoutputtoafile"), iUM->iHwSaveOutputToAFile)) || 
        (!GetStringFromConfig(iSectionName, _L("outputimage"), tmpKeyValue)))
        {
        return KErrNotFound;
        }
    iUM->iOutputImage.Append(tmpKeyValue);
    return KErrNone;
    }


// Compare decoded bitmap with the reference image
void RStepBaseImageDecoder::BitmapComparisonL(const CFbsBitmap& aBitmap, const TDesC& aRefFilename)
    {
    #if defined __EABI__ || defined __X86GCC__
        // for hw test run, bitmap comparison of output and reference image is not done, 
        // this is due to lack of disk space for output and reference images
        return;
    #else
        // compare decoded bitmap with reference bitmap, for winscw test run only
        CFbsBitmap* refBitmap;
        refBitmap = new (ELeave) CFbsBitmap;
        CleanupStack::PushL(refBitmap);
        TInt err = refBitmap->Load(aRefFilename);
        CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not load reference image into a bitmap"));

        INFO_PRINTF2(_L("Comparing bitmap with reference image, %S"), &iUM->iReferenceImage);
        const TSize bitmapSize = aBitmap.SizeInPixels();
        const TSize refBitmapSize = refBitmap->SizeInPixels();
        if (bitmapSize != refBitmapSize)
            {
            ERR_PRINTF5(_L("Bitmap and reference image have different sizes, (%d, %d) != (%d, %d)"), 
                bitmapSize.iWidth, bitmapSize.iHeight, refBitmapSize.iWidth, refBitmapSize.iHeight);
            User::Leave(KErrGeneral);
            }

        TInt nAllPixel = 0;   // all pixels in picture
        TInt nIdentPixel = 0; // only identical pixels

        // compare bitmaps pixel by pixel
        for (TInt y = 0; y < bitmapSize.iHeight; y++)
            {
            for (TInt x = 0; x < bitmapSize.iWidth; x++)
                {
                TPoint pt(x, y);
                TRgb color1, color2;
                aBitmap.GetPixel(color1, pt);
                refBitmap->GetPixel(color2, pt);
                nAllPixel++;

                if (color1 == color2)
                    {
                    nIdentPixel++;
                    }
                }
            }

        if (nAllPixel == nIdentPixel)
            {
            INFO_PRINTF1(_L("Bitmaps are identical!"));
            }
        else
            {
            ERR_PRINTF1(_L("Bitmaps are NOT identical"));
            if (nAllPixel != 0)
                {
                ERR_PRINTF2(_L("Bitmaps identicalness (in percentage) = %3.3f"), ((TReal32) nIdentPixel / nAllPixel) * 100);
                }
            User::Leave(KErrGeneral);
            }
        CleanupStack::PopAndDestroy(1, refBitmap);
    #endif // __EABI__,__X86GCC__
    }


// Get scale execution time according to the current hardware. Currently supported: H2, H4 and emulator
TInt64 RStepBaseImageDecoder::ScaleTimeByMachineL(TInt64 aExecTime, TInt aDestMedia)
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
            
        case HAL::EMachineUid_X86PC:
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
        if (aDestMedia == CUtilityMembers::EDstMediaMMCDrive)
            {
            mulCoef *= 2.0;
            }
        else
            {
            mulCoef *= 1.7;
            }
        }

    return TInt64(mulCoef * TReal(aExecTime));
    }


// Check performance based on time and log the result; also, compare and save the output
void RStepBaseImageDecoder::CheckPerformanceOutputAndSaveL()
    {
    User::LeaveIfError(SaveDecodedImage()); // save the output image

    TInt64 actualDecodeTime = I64INT(iUM->iEndTime.MicroSecondsFrom(iUM->iStartTime).Int64());
    INFO_PRINTF2(_L("Actual decode time: %d microS"), I64INT(actualDecodeTime));

    if (iUM->iExpectedDecodeTime <= 0)
        {
        ERR_PRINTF1(_L("Expected performance time incorrect"));
        User::Leave(KErrGeneral);
        }

    /*TInt64 scaledTime = ScaleTimeByMachineL(iUM->iExpectedDecodeTime);
    if (scaledTime < actualDecodeTime)
        {
        INFO_PRINTF2(_L("Desired decode time exceeded by %d microS"), I64INT(actualDecodeTime - scaledTime));
        #ifdef __WINS__
            INFO_PRINTF1(_L("WARNING: That may mean performance degradation"));
            INFO_PRINTF1(_L("         Check this test on a target hardware!"));
        #else
            ERR_PRINTF1(_L("ERROR: Performance degradation has been detected"));
            User::Leave(KErrGeneral);
        #endif
        }*/
    }


// Check stream decoder and encoder performance based on time and log the result; also, compare and check if output is to be retained
void RStepBaseImageDecoder::CheckStreamPerformanceOutputAndSaveL()
    {
    User::LeaveIfError(RetainEncodedImage());

    // Streamed encoder and decoder peformance
    INFO_PRINTF2(_L("Actual encode time: %d microS"), I64INT(iUM->iEncodeActualTime));
    INFO_PRINTF2(_L("Actual decode time: %d microS"), I64INT(iUM->iDecodeActualTime));

    if (iUM->iExpectedDecodeTime <= 0 || iUM->iExpectedEncodeTime <= 0)
        {
        ERR_PRINTF1(_L("Expected performance time incorrect"));
        User::Leave(KErrGeneral);
        }

    /*TInt64 scaledTime = ScaleTimeByMachineL(iUM->iExpectedDecodeTime);
    if (scaledTime < iUM->iDecodeActualTime)
        {
        INFO_PRINTF2(_L("Desired decode time exceeded by %d microS"), I64INT(iUM->iDecodeActualTime - scaledTime));
        #ifdef __WINS__
            INFO_PRINTF1(_L("WARNING: That may mean performance degradation"));
            INFO_PRINTF1(_L("         Check this test on a target hardware!"));
        #else
            ERR_PRINTF1(_L("ERROR: Performance degradation has been detected"));
            User::Leave(KErrGeneral);
        #endif
        }

    scaledTime = ScaleTimeByMachineL(iUM->iExpectedEncodeTime);
    if (((scaledTime < iUM->iEncodeActualTime))
        {
        INFO_PRINTF2(_L("Desired Encode time exceeded by %d microS"), I64INT(iUM->iEncodeActualTime - scaledTime));
        #ifdef __WINS__
            INFO_PRINTF1(_L("WARNING: That may mean performance degradation"));
            INFO_PRINTF1(_L("         Check this test on a target hardware!"));
        #else
            ERR_PRINTF1(_L("ERROR: Performance degradation has been detected"));
            User::Leave(KErrGeneral);
        #endif
        }*/
    }


// Log display mode for the frame
void RStepBaseImageDecoder::LogFrameDisplayMode()
    {
    switch(iUM->iFrameDisplayModeIndex)
        {
        case 1: INFO_PRINTF1(_L("Display mode: EGray2")); break;
        case 2: INFO_PRINTF1(_L("Display mode: EGray4")); break;
        case 3: INFO_PRINTF1(_L("Display mode: EGray16")); break;
        case 4: INFO_PRINTF1(_L("Display mode: EGray256")); break;
        case 5: INFO_PRINTF1(_L("Display mode: EColor16")); break;
        case 6: INFO_PRINTF1(_L("Display mode: EColor256")); break;
        case 7: INFO_PRINTF1(_L("Display mode: EColor64K")); break;

        case 8:  break; // for tests which use default display mode EColor16M

        case 9: INFO_PRINTF1(_L("Display mode: ERgb")); break;
        case 10: INFO_PRINTF1(_L("Display mode: EColor4K")); break;
        case 11: INFO_PRINTF1(_L("Display mode: EColor16MU")); break;
        case 12: INFO_PRINTF1(_L("Display mode: EColor16MA")); break;
        case 13: INFO_PRINTF1(_L("Display mode: EColor16MAP")); break;
        default: ERR_PRINTF1(_L("Unsupported display mode"));
        }
    }


// Save the output of the decoded image (after all operations are done) to a file
TInt RStepBaseImageDecoder::SaveDecodedImage()
    {
    // output image is, anyway, saved for winscw test run
    #if defined(__EABI__) || defined(__X86GCC__)
        // save the output image for hardware test run, only if required
        if (iUM->iHwSaveOutputToAFile == EFalse) // true (1) => save, false (0) => do not save
            {
            return KErrNone;
            }
    #endif // __EABI__,__X86GCC__

    // create output directory, filename is ignored, only path is considered while creating the directory
    TInt err = iUM->iFs.MkDirAll(DestFilename());
    if (err != KErrNone && err != KErrAlreadyExists)
        {
        ERR_PRINTF2(_L("Could not create output folder, error = %d"), err);
        ERR_PRINTF2(_L("Output directory, %S"), &DestFilename());
        return err;
        }

    err = iUM->iDestBitmap->Save(DestFilename());
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Could not save decoded bitmap to a file, error = %d"), err);
        ERR_PRINTF2(_L("Output filename, %S"), &DestFilename());
        return err;
        }

    INFO_PRINTF2(_L("Output saved to %S"), &DestFilename());
    return KErrNone;
    }


// Retain images only in winscw test run and not for hw test run.
// So, for hw, image will be deleted after the test is complete.
// However, option is provided if the image is required to be retained.
TInt RStepBaseImageDecoder::RetainEncodedImage()
    {
	#if defined (__EABI__) || defined(__X86GCC__)
        if (!iUM->iHwRetainOutputImage)
          	{
    		delete iUM->iImageDecoder; iUM->iImageDecoder = NULL;
    		delete iUM->iImageEncoder; iUM->iImageEncoder = NULL;
    		delete iUM->iExifImageEncoder; iUM->iExifImageEncoder = NULL;
                            
           	TInt err = iUM->iFs.Delete(DestFilename());
           	if (err != KErrNone && err != KErrNotFound)
    			{
    			ERR_PRINTF2(_L("Unable to delete output file, error = %d"), err);
    			StopTest(err, EFail);
    			return err;
    			}
			INFO_PRINTF1(_L("Deleted output image"));
           	}
    #endif // __EABI__,__X86GCC__
    return KErrNone;
    }


// Create the ouput directory as JPEG images will be saved in jpg folder 
// and mbm images in mbm folder of output directory
TInt RStepBaseImageDecoder::CreateOutputDirectory()
	{
	TInt err = iUM->iFs.MkDirAll(DestFilename());
	if ((err != KErrAlreadyExists) && (err != KErrNone))
		{
		ERR_PRINTF3(_L("Cannot create output directory %S, error = %d"), &DestFilename(), err);
		StopTest(err, EFail);
		return err;
		}
	return KErrNone;
	}

// Create image decoder, open the image
TInt RStepBaseImageDecoder::CreateImageDecoder()
    {
    TInt err = KErrNone;
    iUM->iMimeType = KJpgMimeType;
    if (iYuvTest)
        {
        if (iUM->iDecoderUid.iUid != 0)
        	{
        	TRAP(err, iUM->iImageFrameDecoder = static_cast<CJPEGImageFrameDecoder*>(CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, CImageDecoder::EOptionNone, KNullUid, KNullUid, iUM->iDecoderUid)));		
        	}
        else
        	{
        	TRAP(err, iUM->iImageFrameDecoder = static_cast<CJPEGImageFrameDecoder*>(CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions, KNullUid, KNullUid, KImageFramePluginUid)));
        	}
        }
    else
        {
        // if passing the desciptor for creating the decoder call DataNewL
        if (iDescriptorTest)
        	{
        	err = iUM->iFile.Open(iUM->iFs, iUM->iInputImage, EFileRead|EFileShareAny);
        	if (err != KErrNone)
        		{
        		ERR_PRINTF2(_L("Could not open the inputimage for creating decoder, error = %d"), err);
        		INFO_PRINTF2(_L("Input image: %S"), &iUM->iInputImage);
        		return err;
        		}
 			TInt size;
			err = iUM->iFile.Size(size);
			if (err != KErrNone)
        		{
        		ERR_PRINTF2(_L("Could not create decoder, error = %d"), err);
        		return err;
        		}
			TRAP (err, iUM->iSourceData = HBufC8::NewL(size));
			if (err != KErrNone)
        		{
        		ERR_PRINTF2(_L("Could not allocate buffer for creating decoder, error = %d"), err);
        		return err;
        		}
			iUM->iSourceDataPtr.Set(iUM->iSourceData->Des());
			err = iUM->iFile.Read(iUM->iSourceDataPtr);
			if (err != KErrNone)
        		{
        		ERR_PRINTF2(_L("Could not read the inputimage into buffer for creating decoder, error = %d"), err);
        		INFO_PRINTF2(_L("Input image: %S"), &iUM->iInputImage);
        		return err;
        		}
			iUM->iSourceLength = iUM->iSourceData->Length();
			
	        //if true, pass the decoderuid to DataNewL, if false decoder created based on filetype
	        if(iUM->iDecoderUid.iUid != 0)
	        	{
	        	TRAP(err, iUM->iImageDecoder = CImageDecoder::DataNewL(iUM->iFs, iUM->iSourceDataPtr, CImageDecoder::EOptionAllowZeroFrameOpen,KNullUid, KNullUid, iUM->iDecoderUid));
	        	}
			else
				{
				TRAP(err, iUM->iImageDecoder = CImageDecoder::DataNewL(iUM->iFs, iUM->iSourceDataPtr, iUM->iMimeType, CImageDecoder::EOptionAllowZeroFrameOpen));
				}  
        	}
		else if(iIntentTest)
			{
			if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0770-HP")) == 0)
    			{
    			//if true, pass the decoderuid to FileNewL with intent, if false decoder created based on filetype
    			if(iUM->iDecoderUid.iUid != 0)
		        	{
					TRAP(err,iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, EPeek, iUM->iDeOptions,KNullUid, KNullUid, iUM->iDecoderUid));
					}
				else
					{
					TRAP(err,iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, EPeek, iUM->iDeOptions));
					}					
				}
			else if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0780-HP")) == 0)
				{
				err = iUM->iFile.Open(iUM->iFs, iUM->iInputImage, EFileRead|EFileShareAny);
				if (err != KErrNone)
	        		{
	        		ERR_PRINTF2(_L("Could not open the inputimage for creating decoder, error = %d"), err);
	        		INFO_PRINTF2(_L("Input image: %S"), &iUM->iInputImage);
	        		return err;
	        		}
				//if true, pass the decoderuid to FileNewL with intent and RFile as input image, if false decoder created based on filetype
				if(iUM->iDecoderUid.iUid != 0)
		        	{
					TRAP(err,iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFile, EPeek, iUM->iDeOptions,KNullUid, KNullUid, iUM->iDecoderUid));
					}
				else
					{
					TRAP(err,iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFile, EPeek, iUM->iDeOptions));
					}
				}
			}
		else if(iDRMTest)			       	
			{
			TMMFileSource filesource(iUM->iInputImage, KRightsUniqueId001, EView);
			
			if(iUM->iDecoderUid.iUid != 0)
	        	{
				TRAP(err,iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, filesource, iUM->iDeOptions,KNullUid, KNullUid, iUM->iDecoderUid));
	        	}
			else
				{
				TRAP(err,iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, filesource, iUM->iDeOptions));
	        	}	        	
			}
		else
			{
			//if true, pass the decoderuid to filenewl, if false decoder created using default decoderuid
	        if(iUM->iDecoderUid.iUid != 0)
	        	{
	        	TRAP(err, iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions,KNullUid, KNullUid, iUM->iDecoderUid));
	        	}
			else
				{
				TRAP(err, iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions));
				}
			}        	
                
         }

    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Could not create image decoder, error = %d"), err);
        INFO_PRINTF2(_L("Input image: %S"), &iUM->iInputImage);
        }
	else
		{
		if(iUM->iDecoderUid.iUid != 0)
		
			{
			INFO_PRINTF2(_L("Decoder Uid : %d"), iUM->iDecoderUid.iUid);
			if(iUM->iDecoderUid.iUid == KJPGDecoderImplementationUidValue)
				{
				INFO_PRINTF1(_L("Codec: JPG Decoder"));		
				}
			}
		else
			{
			INFO_PRINTF1(_L("Default decoder uid picked for creating decoder"));				
			}			
		}
    return err;
    }


// Prepare encode data, applying colour sampling and quality factor
void RStepBaseImageDecoder::PrepareDecodeDataL()
	{
	// read config file
	TInt qualityfactor = 0;
	TInt sampling = 0;
	TBool checksupportedformats = EFalse;
	if ((!GetIntFromConfig(iSectionName, _L("qualityfactor"), qualityfactor)) || 
	(!GetIntFromConfig(iSectionName, _L("colorsampling"), sampling)) ||
	(!GetBoolFromConfig(iSectionName, _L("checksupportedformats"), checksupportedformats)))
		{
		User::Leave(KErrNotFound);
		}

	TJpegImageData* data = new (ELeave) TJpegImageData;
	data->iSampleScheme = TJpegImageData::TColorSampling(sampling);
	data->iQualityFactor = qualityfactor;

	iUM->iFrameImageData = CFrameImageData::NewL();
	User::LeaveIfError(iUM->iFrameImageData->AppendImageData(data));

	// Create array of expected formats
	// Element 0 contains the 'optimal format'
	RArray<TUid> requiredUids;
	CleanupClosePushL(requiredUids);
	switch (data->iSampleScheme)
		{
		case TJpegImageData::EColor422:
			requiredUids.AppendL(KUidFormatYUV422Interleaved);
			requiredUids.AppendL(KUidFormatYUV422Interleaved);
			break;
		case TJpegImageData::EColor420:
			requiredUids.AppendL(KUidFormatYUV420Planar);
			requiredUids.AppendL(KUidFormatYUV420Planar);
			requiredUids.AppendL(KUidFormatYUV420PlanarReversed);
			break;
		case TJpegImageData::EMonochrome:
			requiredUids.AppendL(KUidFormatYUVMonochrome);
			requiredUids.AppendL(KUidFormatYUVMonochrome);
			break;
		case TJpegImageData::EColor444:
			requiredUids.AppendL(KUidFormatYUV444Planar);
			requiredUids.AppendL(KUidFormatYUV444Planar);
			break;
		default:
			ERR_PRINTF1(_L("Unsupported streaming format requested."));
			User::Leave(KErrNotSupported);
		};
	
	if (iUM->iStreamDecode)
		{
		TDecodeStreamCaps streamCapabilities;
		iUM->iStreamDecode->GetCapabilities(requiredUids[KOptimalFormat], 0, streamCapabilities);
		
		// Set up block size and optimal format
		iUM->iBlockSizeInPixels = streamCapabilities.MinBlockSizeInPixels();
		iUM->iOptimalFormat = requiredUids[KOptimalFormat];
		
		if (checksupportedformats)
			{
			// Verify that the known sampling scheme supports the returned format(s)
			RArray<TUid> supportedFormats;
			CleanupClosePushL(supportedFormats);
			TUid optimalFormat;
			iUM->iStreamDecode->GetSupportedFormatsL(supportedFormats, optimalFormat);
			
			// Check optimal format
			if (requiredUids[KOptimalFormat] != optimalFormat)
				{
				ERR_PRINTF1(_L("GetSupportedFormatsL() returned an unexpected aOptimalFormat TUid."));
				User::Leave(KErrGeneral);
				}

			// Check arrays match
			if ((requiredUids.Count() - 1 /*for optimal format*/ ) != supportedFormats.Count())
				{
				ERR_PRINTF1(_L("GetSupportedFormatsL() returned unexpected number of formats."));
				User::Leave(KErrGeneral);
				}

			for (TInt i = 0; i < requiredUids.Count(); i++)
				{
				TInt index = supportedFormats.Find(requiredUids[i]);
				if (index < 0)
					{
					ERR_PRINTF1(_L("GetSupportedFormatsL() returned unexpected format value."));
					User::Leave(KErrGeneral);
					}
				}
			CleanupStack::PopAndDestroy(&supportedFormats);
			}
		}
	CleanupStack::PopAndDestroy(&requiredUids);
    }


// Crop, rotate and/or mirror the image during decode
void RStepBaseImageDecoder::DecodeToBitmapDuringDecodeL(const TSize aDestSize)
    {
    // create a destination bitmap
    delete iUM->iDestBitmap;
    iUM->iDestBitmap = NULL;
    iUM->iDestBitmap = new (ELeave) CFbsBitmap();

    // create the destination bitmap with the specified size and display mode (default display mode - EColor16M)
    TInt err = iUM->iDestBitmap->Create(aDestSize, FrameDisplayMode());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not create destination bitmap"));

    iUM->iActiveListener->InitialiseActiveListener();

    // ---
    // note: iBeginDecodeCounter, iConvCancelTest, iExtPerfTest are applicable to only crop and rotation tests
    // for use by conversion cancellation tests
    iUM->iBeginDecodeCounter = User::FastCounter();

    // for tests which cancel conversion and restart again after timer expires
	if (iUM->iConvCancelTest)
		{
		iUM->iTimer->InitializeTimer();
		}

    if (iUM->iExtPerfTest)
        {
        // record start time for decoding, for performance tests
        iUM->iStartTime.HomeTime();
        }
    // ---
    // SetDecoderThreadPriority api coverage
    if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0030-HP")) == 0)
    	{
    	iUM->iImageDecoder->SetDecoderThreadPriority(EPriorityMore);	
    	}
	
    // Checking for prepare before convert
    if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0730-HP")) == 0)
    	{
	    if (!GetIntFromConfig(iSectionName, _L("expectederror"), iUM->iExpectedError))
	        {
	        User::Leave(KErrNotFound);
	        }
		iUM->iImageDecoder->Prepare(&iUM->iActiveListener->iStatus);
		return;
    	}    	

    // decode the image
    iUM->iImageDecoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap);
    }

void RStepBaseImageDecoder::EncodeAndSetExifL()
    {
    TInt error = KErrNone;
    iUM->iSrcBitmap = new (ELeave) CFbsBitmap();;
    TRAP(error, iUM->iExifImageEncoder = static_cast<CJPEGExifEncoder*>(CImageEncoder::FileNewL( iUM->iFs, iUM->iOutputImage1, iUM->iEnOptions, KNullUid, KNullUid,KImageFramePluginUid)));
    CHECK_STATUS_LEAVE_IF_ERROR(error,_L("Could not create image encoder for setting orientation tag"));
    
    error = iUM->iSrcBitmap->Load(iUM->iInputImage);
    if (error != KErrNone)
        {
        ERR_PRINTF2(_L("Cannot load input file into a bitmap, error = %d"), error);
        User::Leave(error);
        }
    // Switch encoder from JFIF (default) to EXIF
    iUM->iExifMetadata = iUM->iExifImageEncoder->ExifMetadata();
    // Update Exif orientation tag
    error = SetOrienetationTag();
    if (error != KErrNone)
        {
        ERR_PRINTF2(_L("Unable to set EXIF tag to the image, error = %d"), error);
        User::Leave(error);
        }
    // Prepare encode data
    // read config file
    TInt qualityfactor = 85;
    TInt sampling = 1;
    iUM->iFrameImageData = CFrameImageData::NewL();
    TJpegImageData* data = new (ELeave) TJpegImageData;
    data->iSampleScheme = TJpegImageData::TColorSampling(sampling);
    data->iQualityFactor = qualityfactor; 
    User::LeaveIfError(iUM->iFrameImageData->AppendImageData(data));
    // do conversion
    iUM->iActiveListener->InitialiseActiveListener();
    iUM->iExifImageEncoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iSrcBitmap, iUM->iFrameImageData);    
    }

// Crop, rotate and/or mirror the image during decode
void RStepBaseImageDecoder::DecodeBufferToBitmapDuringDecodeL(const TSize aDestSize)
    {
    // create a destination bitmap
    delete iUM->iDestBitmap;
    iUM->iDestBitmap = NULL;
    iUM->iDestBitmap = new (ELeave) CFbsBitmap();

	const TFrameInfo frameInfo = iUM->iImageDecoder->FrameInfo(0);
	
    // create the destination bitmap with the specified size and display mode (default display mode - EColor16M)
    TInt err = iUM->iDestBitmap->Create(aDestSize, frameInfo.iFrameDisplayMode);//aDestSize, FrameDisplayMode());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not create destination bitmap"));

    iUM->iActiveListener->InitialiseActiveListener();

    // ---
    // note: iBeginDecodeCounter, iConvCancelTest, iExtPerfTest are applicable to only crop and rotation tests
    // for use by conversion cancellation tests
    iUM->iBeginDecodeCounter = User::FastCounter();

    // for tests which cancel conversion and restart again after timer expires
	if (iUM->iConvCancelTest)
		{
		iUM->iTimer->InitializeTimer();
		}

    if (iUM->iExtPerfTest)
        {
        // record start time for decoding, for performance tests
        iUM->iStartTime.HomeTime();
        }
        
	//GetMimeTypeDataL api coverage
	TBuf8<KBufferLen>  theMimeType;
    iUM->iImageDecoder->GetMimeTypeDataL(iUM->iSourceDataPtr, theMimeType);
    if(theMimeType !=  KJpgMimeType)
    	{
    	INFO_PRINTF1(_L("GetMimeTypeDataL() returned wrong MIME type"));
    	}
	INFO_PRINTF1(_L("GetMimeTypeDataL() returned Correct MIME type"));    	

    // ---
	while(!iUM->iImageDecoder->IsImageHeaderProcessingComplete())
		{
		iUM->iImageDecoder->ContinueProcessingHeaderL();
		IncrementBufferSize();
		}
		
    // decode the image
    iUM->iImageDecoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap, 0);
    }

// used for DataNewL coverage
void RStepBaseImageDecoder::IncrementBufferSize()
	{
	TInt position(iUM->iSourceDataPtr.Length());

	if (position < iUM->iSourceLength)
		{
		iUM->iSourceDataPtr.SetLength(position + 1);
		iUM->iFile.Write(iUM->iSourceDataPtr.Mid(position, 1));
		}
	}
	
	
// Crop the image during decoding with mask
void RStepBaseImageDecoder::DecodeWithMaskDuringDecodeL(const TSize aDestSize)
    {
    // create a destination bitmap
    delete iUM->iDestBitmap;
    iUM->iDestBitmap = NULL;
    iUM->iDestBitmap = new (ELeave) CFbsBitmap();

    // create a destination mask bitmap
    delete iUM->iDestMaskBitmap;
    iUM->iDestMaskBitmap = NULL;
    iUM->iDestMaskBitmap = new (ELeave) CFbsBitmap();

    // create the destination bitmap and destination mask bitmap
    User::LeaveIfError(iUM->iDestBitmap->Create(aDestSize, FrameDisplayMode()));
    User::LeaveIfError(iUM->iDestMaskBitmap->Create(aDestSize, EGray2));

    iUM->iActiveListener->InitialiseActiveListener();

    // decode the image
    iUM->iImageDecoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap, *iUM->iDestMaskBitmap);
    }

// Crop, rotate and/or mirror the image during decoding to YUV pixel buffer
void RStepBaseImageDecoder::DecodeToYuvPixelBufferDuringDecodeL(const TSize aDestSize)
    {
    TInt imageSizeInBytes;
    TBool supported = iUM->iImageFrameDecoder->RecommendedBufferSize(imageSizeInBytes);
    if (supported == EFalse)
        {
        ERR_PRINTF2(_L("Codec does not support image format, error = %d"), KErrNotSupported);
        User::Leave(KErrNotSupported);
        }

    // create destination RChunk
    User::LeaveIfError(iUM->iChunk.CreateLocal(imageSizeInBytes, imageSizeInBytes, EOwnerProcess));

    // create an empty image frame
    TRAPD(err, iUM->iImageFrame = CImageFrame::NewL(&iUM->iChunk, imageSizeInBytes, KChunkDataOffset));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not create image frame"));

    iUM->iImageFrame->SetFrameSizeInPixels(aDestSize);

    TUid yuvFormat;
    User::LeaveIfError(GetYUVFormat(yuvFormat));
    TRAP(err, iUM->iImageFrame->SetFrameFormatL(TFrameFormat(yuvFormat)));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image frame format"));

    iUM->iActiveListener->InitialiseActiveListener();

    // decode the image to YUV
    iUM->iImageFrameDecoder->ConvertFrame(&iUM->iActiveListener->iStatus, *iUM->iImageFrame);
    }


// Encode YUV pixel buffer (image frame) to an image
void RStepBaseImageDecoder::EncodeYuvPixelBufferToAnImageL()
    {
    INFO_PRINTF2(_L("Encoding to output image %S"), &iUM->iOutputImage);
    TRAPD(err, iUM->iImageFrameEncoder = static_cast<CJPEGImageFrameEncoder*>(CImageEncoder::FileNewL(iUM->iFs, iUM->iOutputImage, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not create image encoder"));

    iUM->iActiveListener->InitialiseActiveListener();

    // encode YUV pixel buffer to an image
    iUM->iImageFrameEncoder->ConvertFrame(&iUM->iActiveListener->iStatus, *iUM->iImageFrame);
    }


// Compare output from YUV pixel buffer tests (jpg)
void RStepBaseImageDecoder::CompareImagesL()
    {
    #if defined (__EABI__) || defined (__X86GCC__)
        // for hw test run, image comparison of output and reference image is not done, 
        // this is due to lack of disk space for output and reference images
        return;
    #else
        INFO_PRINTF2(_L("Comparing output image with the reference image %S"), &iUM->iReferenceImage);

        TInt outFileSize = 0;
        User::LeaveIfError(iUM->iFile.Open(iUM->iFs, iUM->iOutputImage, EFileRead));
        iUM->iFile.Size(outFileSize);
        HBufC8* outImage = HBufC8::NewMaxLC(outFileSize);
        TPtr8 outImagePtr = outImage->Des();
        iUM->iFile.Read(outImagePtr);
        iUM->iFile.Close();

        TInt refFileSize = 0;
        User::LeaveIfError(iUM->iFile.Open(iUM->iFs, iUM->iReferenceImage, EFileRead));
        iUM->iFile.Size(refFileSize);
        HBufC8* refImage = HBufC8::NewMaxLC(refFileSize);
        TPtr8 refImagePtr = refImage->Des();
        iUM->iFile.Read(refImagePtr);
        iUM->iFile.Close();

        TInt err = Mem::Compare(outImagePtr.Ptr(), outFileSize, refImagePtr.Ptr(), refFileSize);

        if ((outFileSize != refFileSize) || (err != KErrNone))
            {
            ERR_PRINTF1(_L("Images are NOT identical!"));
            User::Leave(KErrGeneral);
            }
        else
            {
    		INFO_PRINTF1(_L("Images are identical!"));
            }

        CleanupStack::PopAndDestroy(2, outImage); // outImage, refImage
    #endif // __EABI__,__X86GCC__
    }


// Change the state to next
void RStepBaseImageDecoder::StateChange()
    {
    iUM->iState++;
    }


// Add an operation or multiple operations
void RStepBaseImageDecoder::AddOperationL()
    {
    TImageConvOperation* operation = NULL;
    if (iYuvTest)
        {
        operation = iUM->iImageFrameDecoder->OperationL();
        }
    else
        {
        operation = iUM->iImageDecoder->OperationL();
        }
    if (iUM->iAutorotate)
        {
        INFO_PRINTF1(_L("Operation: EOptionAutoRotate"));
        }    
    switch (iUM->iOperationIndex)
        {
        case 1:
            INFO_PRINTF1(_L("Operation: ERotation90DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
            break;

        case 2:
            INFO_PRINTF1(_L("Operation: ERotation180DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
            break;

        case 3:
            INFO_PRINTF1(_L("Operation: ERotation270DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
            break;

        case 4:
            INFO_PRINTF1(_L("Operation: EMirrorHorizontalAxis"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            break;

        case 5:
            INFO_PRINTF1(_L("Operation: EMirrorVerticalAxis"));
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            break;


        // Multiple operations
        case 51:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis & EMirrorVerticalAxis"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            break;

        case 52:
            INFO_PRINTF1(_L("Operations: ERotation90DegreesClockwise & EMirrorHorizontalAxis"));
            operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            break;

        case 53:
            INFO_PRINTF1(_L("Operations: ERotation180DegreesClockwise & EMirrorHorizontalAxis"));
            operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            break;

        case 54:
            INFO_PRINTF1(_L("Operations: ERotation270DegreesClockwise & EMirrorHorizontalAxis"));
            operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            break;

        case 55:
            INFO_PRINTF1(_L("Operations: ERotation90DegreesClockwise & EMirrorVerticalAxis"));
            operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            break;

        case 56:
            INFO_PRINTF1(_L("Operations: ERotation180DegreesClockwise & EMirrorVerticalAxis"));
            operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            break;

        case 57:
            INFO_PRINTF1(_L("Operations: ERotation270DegreesClockwise & EMirrorVerticalAxis"));
            operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            break;

        case 58:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis & ERotation90DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
            break;

        case 59:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis & ERotation180DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
            break;

        case 60:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis & ERotation270DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
            break;

        case 61:
            INFO_PRINTF1(_L("Operations: EMirrorVerticalAxis & ERotation90DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
            break;

        case 62:
            INFO_PRINTF1(_L("Operations: EMirrorVerticalAxis & ERotation180DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
            break;

        case 63:
            INFO_PRINTF1(_L("Operations: EMirrorVerticalAxis & ERotation270DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
            break;

        case 64:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis, EMirrorVerticalAxis & ERotation90DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
            // Same as ERotation90DegreesClockwise, EMirrorHorizontalAxis & EMirrorVerticalAxis
            // or ERotation90DegreesClockwise, EMirrorVerticalAxis & EMirrorHorizontalAxis
            break;

        case 65:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis, EMirrorVerticalAxis & ERotation180DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
            // Same as ERotation180DegreesClockwise, EMirrorHorizontalAxis & EMirrorVerticalAxis
            // or ERotation180DegreesClockwise, EMirrorVerticalAxis & EMirrorHorizontalAxis
            break;

        case 66:
            INFO_PRINTF1(_L("Operations: EMirrorHorizontalAxis, EMirrorVerticalAxis & ERotation270DegreesClockwise"));
            operation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
            operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
            operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
            // Same as ERotation270DegreesClockwise, EMirrorHorizontalAxis & EMirrorVerticalAxis
            // or ERotation270DegreesClockwise, EMirrorVerticalAxis & EMirrorHorizontalAxis
            break;

        default:
            ERR_PRINTF2(_L("Invalid value for operation index, %d"), iUM->iOperationIndex);
            User::Leave(KErrGeneral);
        }
    }


// Flags to control how the image is decoded; these can be combined using an OR operation
TInt RStepBaseImageDecoder::GetDecodeOptions(CImageDecoder::TOptions& aOptions)
    {
    switch (iUM->iDeOptionsIndex)
        {
        case 0:  aOptions = CImageDecoder::EOptionNone; break;
        case 1:  aOptions = CImageDecoder::EOptionNoDither; break;
        case 2:  aOptions = CImageDecoder::EOptionAlwaysThread; break;
        case 3:  aOptions = CImageDecoder::EOptionAllowZeroFrameOpen; break;
        case 4:  aOptions = CImageDecoder::EAllowGeneratedMask; break;
        case 5:  aOptions = CImageDecoder::EPreferFastDecode;   break;
        case 6:  aOptions = CImageDecoder::EOptionMngSubframesNoLoops;  break;
        case 7:  aOptions = CImageDecoder::EOptionUseFrameSizeInPixels; break;
        case 8:  aOptions = CImageDecoder::EOptionIgnoreExifMetaData;   break;
        case 9:  aOptions = CImageDecoder::EOptionExtCrop;      break;
        case 10: aOptions = CImageDecoder::EOptionExtStream;    break;
        case 11: aOptions = CImageDecoder::EOptionExtRotation;  break;
        case 12: aOptions = CImageDecoder::EOptionExtMirrorHorizontalAxis; break;
        case 13: aOptions = CImageDecoder::EOptionExtMirrorVerticalAxis;   break;
        case 14: aOptions = CImageDecoder::EOptionExtScaling;   break;
        case 15: aOptions = CImageDecoder::EOptionExtReserved7; break;
        case 16: aOptions = CImageDecoder::EOptionExtReserved8; break;
        case 17: aOptions = CImageDecoder::EOptionAutoRotate; break;
        
        // Multiple CImageDecoder::TOptions
        case 51: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtCrop | 
                                                    CImageDecoder::EOptionExtStream); break;

        case 52: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtRotation | 
                                                    CImageDecoder::EOptionExtMirrorHorizontalAxis); break;

        case 53: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtRotation | 
                                                    CImageDecoder::EOptionExtMirrorVerticalAxis); break;

        case 54: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtRotation | 
                                                    CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                                    CImageDecoder::EOptionExtMirrorVerticalAxis); break;

        case 55: aOptions = static_cast<CImageDecoder::TOptions>
                                                    (CImageDecoder::EOptionExtCrop | 
                                                    CImageDecoder::EOptionExtRotation | 
                                                    CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                                    CImageDecoder::EOptionExtMirrorVerticalAxis); break;

        case 56: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtStream | 
                                                    CImageDecoder::EOptionExtRotation); break;

        case 57: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtStream | 
                                                    CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                                    CImageDecoder::EOptionExtMirrorVerticalAxis); break;

        case 58: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtStream | 
                                                    CImageDecoder::EOptionExtScaling); break;

        case 59: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtCrop | 
                                                    CImageDecoder::EOptionExtScaling); break;

        case 60: aOptions = static_cast<CImageDecoder::TOptions>
                                                    (CImageDecoder::EOptionExtCrop | 
                                                    CImageDecoder::EOptionExtScaling | 
                                                    CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                                    CImageDecoder::EOptionExtMirrorVerticalAxis); break;
		
		case 61: aOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionAutoRotate | 
                                                    CImageDecoder::EOptionExtMirrorHorizontalAxis); break;
                                                    
        default:
            ERR_PRINTF2(_L("Invalid value for CImageDecoder::TOptions index, %d"), iUM->iDeOptionsIndex);
            return KErrGeneral;
        }
    return KErrNone;
    }


// Get actual rotation angle/mirroring values, from the values read from config file
TInt RStepBaseImageDecoder::GetRotationAngle(const TInt aAngleIndex, CBitmapRotator::TRotationAngle& aAngle)
    {
    switch (aAngleIndex)
        {
        case 90:  aAngle = CBitmapRotator::ERotation90DegreesClockwise;  break;
        case 180: aAngle = CBitmapRotator::ERotation180DegreesClockwise; break;
        case 270: aAngle = CBitmapRotator::ERotation270DegreesClockwise; break;
        case 3:   aAngle = CBitmapRotator::EMirrorHorizontalAxis; break;
        case 4:   aAngle = CBitmapRotator::EMirrorVerticalAxis;   break;

        default:
            ERR_PRINTF2(_L("Invalid value for angle of rotation/mirroring index, %d"), aAngleIndex);
            return KErrGeneral;
        }
    return KErrNone;
    }


// Get rectangle for cropping, from the values read from config file
void RStepBaseImageDecoder::GetCropRectangle(TRect& aCropRect)
    {
    if (iUM->iCroppingRectForm == KCroppingFormPointAAndB) // four coordinates are specified
        {
        aCropRect = TRect(iUM->iCropPointA, iUM->iCropPointB);
        INFO_PRINTF5(_L("Region of Interest: ax = %d, ay = %d, bx = %d, by = %d"), 
            iUM->iCropPointA.iX, iUM->iCropPointA.iY, iUM->iCropPointB.iX, iUM->iCropPointB.iY);
        }
    else // KCroppingFormPointAndSize, origin, width and height are specified
        {
        aCropRect = TRect(iUM->iCropPointA, iUM->iCropSize);
        INFO_PRINTF5(_L("Region of Interest: x = %d, y = %d, width = %d, height = %d"), 
            iUM->iCropPointA.iX, iUM->iCropPointA.iY, iUM->iCropSize.iWidth, iUM->iCropSize.iHeight);
        }
    }


// Get reduced size, from the values read from config file
void RStepBaseImageDecoder::GetScalingSizeL(const TSize aOriginalSize, TSize& aReducedSize)
    {
    TInt err = KErrNone;
    if (iUM->iScalingForm == KScalingFormReductionFactor) // form is used when reduction factor is specified
        {
        // calculate destination size
        err = iUM->iImageDecoder->ReducedSize(aOriginalSize, iUM->iReductionFactor, aReducedSize);
        }
    else // form KScalingFormDesiredSize is used when destination width and height are specified
        {
        TSize desiredDestSize;
        desiredDestSize.iWidth  = iUM->iScaleSize.iWidth;
        desiredDestSize.iHeight = iUM->iScaleSize.iHeight;

        // calculate destination size
        iUM->iReductionFactor = iUM->iImageDecoder->ReductionFactor(aOriginalSize, desiredDestSize);
        err = iUM->iImageDecoder->ReducedSize(aOriginalSize, iUM->iReductionFactor, aReducedSize);

        if (aReducedSize.iHeight > desiredDestSize.iHeight || aReducedSize.iWidth > desiredDestSize.iWidth )
            {
            err = KErrGeneral;
            }
        }

    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Scale: incorrect argument or reduced size"));
    }


// Get scaling quality when scale extension interface is used
void RStepBaseImageDecoder::GetScalingQualityL(TImageConvScaler::TScalerQuality& aScalingQuality)
    {
    switch (iUM->iScalingQualityIndex)
        {
        case 0: aScalingQuality = TImageConvScaler::EMinimumQuality; break;
        case 1: aScalingQuality = TImageConvScaler::EMediumQuality;  break;
        case 2: aScalingQuality = TImageConvScaler::EMaximumQuality; break;

        default:
            ERR_PRINTF2(_L("Invalid value for scaling quality index, %d"), iUM->iScalingQualityIndex);
            User::Leave(KErrGeneral);
        }
    }


// Get YUV format
TInt RStepBaseImageDecoder::GetYUVFormat(TUid& aYUVFormat)
    {
    switch (iUM->iYUVFormatIndex)
        {
        case 0:
            aYUVFormat = KUidFormatYUVMonochrome;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUVMonochrome"));
            break;

        case 1: aYUVFormat = KUidFormatYUV422Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV422Interleaved"));
            break;

        case 2: aYUVFormat = KUidFormatYUV420Planar;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV420Planar"));
            break;

        case 3: aYUVFormat = KUidFormatYUV420PlanarReversed;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV420PlanarReversed"));
            break;

        case 4: aYUVFormat = KUidFormat16bitRGB444Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormat16bitRGB444Interleaved"));
            break;

        case 5: aYUVFormat = KUidFormat16BitRGB565Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormat16BitRGB565Interleaved"));
            break;

        case 6: aYUVFormat = KUidFormat32BitRGB888Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormat32BitRGB888Interleaved"));
            break;

        case 7: aYUVFormat = KUidFormatYUV420Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV420Interleaved"));
            break;

        case 8: aYUVFormat = KUidFormatYUV422InterleavedReversed;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV422InterleavedReversed"));
            break;

        case 10: aYUVFormat = KUidFormatYYUV422Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYYUV422Interleaved"));
            break;

        case 11: aYUVFormat = KUidFormatYUV422Planar;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV422Planar"));
            break;

        case 12: aYUVFormat = KUidFormatYUV444Planar;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV444Planar"));
            break;

        case 13: aYUVFormat = KUidFormatYUV444Interleaved;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV444Interleaved"));
            break;

        case 14: aYUVFormat = KUidFormatYUV420SemiPlanar;
            INFO_PRINTF1(_L("YUV Format Uid: KUidFormatYUV420SemiPlanar"));
            break;

        default:
            ERR_PRINTF2(_L("Invalid value for YUV format index, %d"), iUM->iYUVFormatIndex);
            return KErrGeneral;
        }
    return KErrNone;
    }


// Get image type uid based on input file extension
TUid RStepBaseImageDecoder::ImageType()
	{
	TUid imageType = KNullUid;
	TParse parse;
	parse.Set(iUM->iInputImage, 0, 0);
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


// Get image size - width and height
TSize RStepBaseImageDecoder::ImageSize()
    {
    const TFrameInfo* frameInfo = NULL;
    if (iYuvTest)
        {
        frameInfo = &iUM->iImageFrameDecoder->FrameInfo();
        }
    else
        {
        frameInfo = &iUM->iImageDecoder->FrameInfo();
        }

    return frameInfo->iOverallSizeInPixels;
    }


// Get destination filename
TFileName& RStepBaseImageDecoder::DestFilename()
	{
	return iUM->iOutputImage;
	}


// Get display mode for the frame
TDisplayMode RStepBaseImageDecoder::FrameDisplayMode()
    {
    switch(iUM->iFrameDisplayModeIndex)
        {
        case 1: return EGray2;
        case 2: return EGray4;
        case 3: return EGray16;
        case 4: return EGray256;
        case 5: return EColor16;
        case 6: return EColor256;
        case 7: return EColor64K;

        case 8: return EColor16M; // for tests which use default display mode EColor16M

        // note that ERgb is not specified for tests which use all the display modes (for the reason that it is not an actual display mode)
        case 9: return ERgb;
        case 10: return EColor4K;
        case 11: return EColor16MU;
        case 12: return EColor16MA;
        case 13: return EColor16MAP;
        // case xxx: EColorLast;

        default: ERR_PRINTF2(_L("Invalid value for display mode index, %d"), iUM->iFrameDisplayModeIndex);
        }

    return ENone; // for any other display mode index, return ENone
    }


// Set the thread for decoding - client OR separate thread; can be combined with other options
void RStepBaseImageDecoder::SetThreadForDecoding(CImageDecoder::TOptions& aOptions)
    {
    if (iUM->iAlwaysThread) // true (1) => separate thread, false (0) => client thread
        {
        aOptions = static_cast<CImageDecoder::TOptions> (aOptions | CImageDecoder::EOptionAlwaysThread);
        }
    }


// Set the thread for encoding - client OR separate thread; can be combined with other options
void RStepBaseImageDecoder::SetThreadForEncoding(CImageEncoder::TOptions& aOptions)
    {
    if (iUM->iAlwaysThread) // true (1) => separate thread, false (0) => client thread
        {
        aOptions = static_cast<CImageEncoder::TOptions> (aOptions | CImageEncoder::EOptionAlwaysThread);
        }
    }


// Set extensions - crop, scale, operation and various combinations in the defined order
void RStepBaseImageDecoder::SetExtensionL(const CUtilityMembers::TExtensionType aExtensionType, TSize& aExtensionDestSize)
    {
    TInt err = KErrNone;
    TImageConvScaler* scaler = NULL;
    TImageConvScaler::TScalerQuality scalingQuality;

    // Crop
    if ((aExtensionType == CUtilityMembers::EExtensionCrop) || (aExtensionType == CUtilityMembers::EExtensionCropScale) || 
        (aExtensionType == CUtilityMembers::EExtensionCropOperation) || (aExtensionType == CUtilityMembers::EExtensionCropScaleOperation))
        {
        GetCropRectangle(iUM->iCropRect);
        if (iYuvTest)
            {
            TRAP(err, iUM->iImageFrameDecoder->SetClippingRectL(&iUM->iCropRect));
            }
        else
            {
            TRAP(err, iUM->iImageDecoder->SetClippingRectL(&iUM->iCropRect));
            }
        CHECK_STATUS_LEAVE_IF_ERROR(err, _L("SetClippingRectL() left"));
        }

    // Scale
    if ((aExtensionType == CUtilityMembers::EExtensionScale) || (aExtensionType == CUtilityMembers::EExtensionCropScale) || 
        (aExtensionType == CUtilityMembers::EExtensionScaleOperation) || (aExtensionType == CUtilityMembers::EExtensionCropScaleOperation) || 
        (aExtensionType == CUtilityMembers::EExtensionScaleStream))
        {
        if (iYuvTest)
            {
            TRAP(err, scaler = iUM->iImageFrameDecoder->ScalerL());
            }
        else
            {
            TRAP(err, scaler = iUM->iImageDecoder->ScalerL());
            }
        CHECK_STATUS_LEAVE_IF_ERROR(err, _L("ScalerL() left"));

        GetScalingQualityL(scalingQuality);
        if (iUM->iScalerExtUseDesiredSizeMethod)
            {
            INFO_PRINTF3(_L("Desired size: width = %d, height = %d"), iUM->iExtScaleSize.iWidth, iUM->iExtScaleSize.iHeight);
            TRAP(err, scaler->SetScalingL(iUM->iExtScaleSize, scalingQuality, iUM->iLockAspectRatio));
            }
        else
            {
            INFO_PRINTF2(_L("Scaling coefficient: %d"), iUM->iScalingCoeff);
            TRAP(err, scaler->SetScalingL(iUM->iScalingCoeff, scalingQuality));
            }
        CHECK_STATUS_LEAVE_IF_ERROR(err, _L("SetScalingL() left"));
        }

    // Operation(s)
    if ((aExtensionType == CUtilityMembers::EExtensionOperation) || (aExtensionType == CUtilityMembers::EExtensionCropOperation) || 
        (aExtensionType == CUtilityMembers::EExtensionScaleOperation) || (aExtensionType == CUtilityMembers::EExtensionCropScaleOperation))
        {
        if ((iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0010-HP")) != 0) && (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0020-HP")) != 0)) 
        	{
        	TRAP(err, AddOperationL());
        	CHECK_STATUS_LEAVE_IF_ERROR(err, _L("OperationL() or AddOperationL() left"));		
        	}
        else
			{
			INFO_PRINTF1(_L("Operation: AutoRotate Orientation Only"));	
			}        	
        }

    if (iYuvTest)
        {
        err = iUM->iImageFrameDecoder->GetDestinationSize(aExtensionDestSize, KFrameZero);
        }
    else
        {
        err = iUM->iImageDecoder->GetDestinationSize(aExtensionDestSize, KFrameZero);
        }

	CHECK_STATUS_LEAVE_IF_ERROR(err, _L("GetDestinationSize() failed"));
    }

// Check whether autorotate flag set, if true apply in the decode options, else proceed as is
TBool RStepBaseImageDecoder::CheckAutoRotate()
    {
    // last 7 chars of the test case number is the name of the section
    iSectionName = iTestStepName.Right(KSectionNameLen);
    if ((!GetBoolFromConfig(iSectionName, _L("autorotate"), iUM->iAutorotate)))
        {
        INFO_PRINTF1(_L("AutoRoate Option not found in the config file"));
        User::LeaveIfError (KErrNotFound);
        StopTest(EFail);
        }
    return iUM->iAutorotate;
    }

// Set the source image type - thumbnail as source image OR main image
void RStepBaseImageDecoder::SetImageTypeL()
    {
    if (iUM->iImageType.Compare(_L("thumbnail")) == 0)
        {
        iUM->iImageDecoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail);
        INFO_PRINTF3(_L("Thumbnail size: width = %d, height = %d"), ImageSize().iWidth, ImageSize().iHeight);
        }
    else
        {
        iUM->iImageDecoder->SetImageTypeL(CImageDecoder::EImageTypeMain);
        }
    }


// Set color sampling and quality factor information
void RStepBaseImageDecoder::SetImageDataL()
    {
    TInt colorSampling = 0;
    switch (iUM->iColorSamplingIndex)
        {
        case 0:
            colorSampling = TJpegImageData::EMonochrome;
            INFO_PRINTF1(_L("Color sampling: EMonochrome"));
            break;

        case 1:
            colorSampling = TJpegImageData::EColor420;
            INFO_PRINTF1(_L("Color sampling: EColor420"));
            break;

        case 2:
            colorSampling = TJpegImageData::EColor422;
            INFO_PRINTF1(_L("Color sampling: EColor422"));
            break;

        case 3:
            colorSampling = TJpegImageData::EColor444;
            INFO_PRINTF1(_L("Color sampling: EColor444"));
            break;

        default:
            ERR_PRINTF2(_L("Invalid value for TJpegImageData::TColorSampling index, %d"), iUM->iColorSamplingIndex);
            User::Leave(KErrGeneral);
        }

    iUM->iFrameImageData = CFrameImageData::NewL();

    TJpegImageData* imageData = new (ELeave) TJpegImageData;
    CleanupStack::PushL(imageData);

    imageData->iSampleScheme  = TJpegImageData::TColorSampling(colorSampling);
    imageData->iQualityFactor = iUM->iQualityFactor;
    INFO_PRINTF2(_L("Quality factor: %d"), iUM->iQualityFactor);
    User::LeaveIfError(iUM->iFrameImageData->AppendImageData(imageData));

    CleanupStack::Pop(imageData);
    }


// Set YCbCrPositioning and ColorSpace tag of the EXIF data
TInt RStepBaseImageDecoder::SetShort()
	{
	ASSERT(iUM->iExifMetadata);

	const TUint KTagID = 0xA001;
	const TUint16 KShortTestResult = 1;

	// Set the EXIF data Color Space in the encoder
	TInt testResult = iUM->iExifMetadata->SetShortParam(KTagID, 0, KShortTestResult);
	if (testResult != KErrNone)
		{
		return testResult;
		}
	else
		{
		// Set the EXIF data YCbCrPositioning in the encoder
		const TUint KTagID1 = 0x0213;
		testResult = iUM->iExifMetadata->SetShortParam(KTagID1, 0, KShortTestResult);
		if (testResult != KErrNone)
			{
			return testResult;
			}
		}
	return testResult;
	}


// Set user comment tag of the EXIF data
TInt RStepBaseImageDecoder::Set16bitBuffer()
	{
	ASSERT(iUM->iExifMetadata);

	const TUint KTagID = 0x9286;
	_LIT16(KUserCommentTestResult, "This is a Symbian JPEG Image");

	// Set the EXIF Data UserCommnet in the encoder
	HBufC16* buffer = KUserCommentTestResult().Alloc();
	if (buffer == NULL)
		{
		return KErrGeneral;
		}
	TInt testResult = iUM->iExifMetadata->SetParam16(KTagID, 0, buffer);
	delete buffer;
	return testResult;
	}


// Set model version of the EXIF data
TInt RStepBaseImageDecoder::SetModelVersion()
	{
	ASSERT(iUM->iExifMetadata);

	const TUint KTagID = 0x0110;
	_LIT8(KModelTestResult, "Symbian OS v9.5");

	// Set the EXIF data Model in the encoder
	HBufC8* buffer = KModelTestResult().Alloc();
	if (buffer == NULL)
		{
		return KErrGeneral;
		}
	TInt testResult = iUM->iExifMetadata->SetParam8(KTagID, 0, buffer);
	delete buffer;
	return testResult;
	}
TInt RStepBaseImageDecoder::SetOrienetationTag()
    {
    TInt setorientation;
    ASSERT(iUM->iExifMetadata);
    if (!GetIntFromConfig(iSectionName, _L("orientation"), setorientation))
       {
       User::LeaveIfError(KErrNotFound);
       }
    const TUint KTagID = 0x0112;
    const TUint16 KShortTestResult = setorientation;
    
    // Set the EXIF data Color Space in the encoder
    TInt testResult = iUM->iExifMetadata->SetShortParam(KTagID, 0, KShortTestResult);
    if (testResult != KErrNone)
        {
        return testResult;
        }
    return testResult;
    }

// Set the EXIF data for the encoded image
TInt RStepBaseImageDecoder::SetExifAndThumbnailData()
	{
	// Set the EXIF data
	TInt testResult = Set16bitBuffer();
	if (testResult != KErrNone)
		{
		return testResult;
		}
	else
		{
		testResult = SetModelVersion();
		if (testResult != KErrNone)
			{
			return testResult;
			}
		else
			{
			testResult = SetShort();
			if (testResult != KErrNone)
				{
				return testResult;
				}
			}
		}
	return testResult;
	}


// Log the test case description, input image name and its size
void RStepBaseImageDecoder::LogTestCaseDescription()
    {
    INFO_PRINTF2(_L("%S"), &iTestCaseDescription);
    INFO_PRINTF2(_L("Input image: %S"), &iUM->iInputImage);
    INFO_PRINTF2(_L("Output image: %S"), &iUM->iOutputImage);
    INFO_PRINTF3(_L("Image size: width = %d, height = %d"), ImageSize().iWidth, ImageSize().iHeight);
    }


// Start the test
void RStepBaseImageDecoder::KickoffTestL()
    {
    __MM_HEAP_MARK;

    // use of data members for most of the tests, allocated memory for iUM on the heap
    iUM = new (ELeave) CUtilityMembers();

    User::LeaveIfError(iUM->iFs.Connect());
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());

    // derived test step class to implement
    DoKickoffTestL();
    }


// Cleanup at the end of the test
void RStepBaseImageDecoder::CloseTest()
    {
    delete iUM;

    RFbsSession::Disconnect();
    iUM->iFs.Close();

    __MM_HEAP_MARKEND;
    }
