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
// This file contains test step class implementation for baseline performance integration tests for image decoding.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_perf.h"


// RStepBaseImageDecoderPerformance
RStepBaseImageDecoderPerformance::RStepBaseImageDecoderPerformance(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    }


// Read common input from the configuration file
TInt RStepBaseImageDecoderPerformance::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of the section in the configuration file
    iSectionName = iTestStepName.Right(KSectionNameLen);

    TPtrC tmpKeyValue;
    if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iUM->iAlwaysThread)) || 
        (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue)))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

	//if the decoderuid present, then  pass it to decoder, else decoder created using default uid
	if ((GetHexFromConfig(iSectionName, _L("decoderuid"), iUM->iDecUid)))
		{
		if(iUM->iDecUid != 0)
			{
			iUM->iDecoderUid.iUid = iUM->iDecUid;	
			}
		}		
		
    if ((ReadCommonInputExpectedPerfTimeFromConfigFile() == KErrNotFound) || 
        (ReadCommonInputRefAndOutputImagesFromConfigFile() == KErrNotFound))
        {
        return KErrNotFound;
        }
    return ReadCommonInputImageFromConfigFile();
    }


// Read and log some EXIF tags but the data cannot be saved to the output image
// Reading EXIF data will not count towards time to be measured for decoding
void RStepBaseImageDecoderPerformance::ReadExifDataFromImageL()
    {
    CJPEGExifDecoder* exifImageDecoder = NULL;
    TRAPD(err, exifImageDecoder = static_cast<CJPEGExifDecoder*> (CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, CImageDecoder::EOptionNone, KImageTypeJPGUid)));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not create EXIF image decoder"));

    CleanupStack::PushL(exifImageDecoder);

    iUM->iExifMetadata = exifImageDecoder->ExifMetadata();
    if (iUM->iExifMetadata == NULL)
        {
        ERR_PRINTF1(_L("Image does not contain EXIF info"));
        CleanupStack::PopAndDestroy(1, exifImageDecoder);
        return;
        }
    INFO_PRINTF1(_L("Some EXIF info of the image... will not count towards time measured for decoding"));

    TUint KTagID = 0;
    KTagID = 0x0110;
    HBufC8* buffer8Bit = NULL;
    err = iUM->iExifMetadata->GetParam8(KTagID, 0, buffer8Bit);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Model name or model number of the equipment, error = %d"), err);
        }
    else
        {
        HBufC* tempData = NULL;
        tempData = HBufC::NewL(buffer8Bit->Length());
        TPtr ptr = tempData->Des();
        CnvUtfConverter::ConvertToUnicodeFromUtf8(ptr, *buffer8Bit);
        INFO_PRINTF2(_L("Model name or model number of the equipment: %S"), &ptr);
        delete tempData; tempData = NULL;
        }
    delete buffer8Bit; buffer8Bit = NULL;

    KTagID = 0x0112;
    TUint16 unsignedIntParam = 0;
    err = iUM->iExifMetadata->GetShortParam(KTagID, 0, unsignedIntParam);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Orientation, error = %d"), err);
        }
    else
        {
        INFO_PRINTF2(_L("Orientation : %d"), unsignedIntParam);
        }

    KTagID = 0x011a;
    TInt numer = 0, denom = 0;
    err = iUM->iExifMetadata->GetRationalParam(KTagID, 0, numer, denom);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read X Resolution, error = %d"), err);
        }
    else
        {
        INFO_PRINTF3(_L("X Resolution: %d/%d"), numer, denom);
        }

    KTagID = 0x0213;
    unsignedIntParam = 0;
    err = iUM->iExifMetadata->GetShortParam(KTagID, 0, unsignedIntParam);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read YCbCr Positioning, error = %d"), err);
        }
    else
        {
        INFO_PRINTF2(_L("YCbCr Positioning: %d"), unsignedIntParam);
        }

    KTagID = 0xa002;
    TInt intParam = 0;
    err = iUM->iExifMetadata->GetIntegerParam(KTagID, 0, intParam);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Image Width, error = %d"), err);
        }
    else
        {
        INFO_PRINTF2(_L("Image Width : %d"), intParam);
        }

    KTagID = 0xa003;
    intParam = 0;
    err = iUM->iExifMetadata->GetIntegerParam(KTagID, 0, intParam);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Image Height, error = %d"), err);
        }
    else
        {
        INFO_PRINTF2(_L("Image Height: %d"), intParam);
        }

    KTagID = 0x8769;
    CArrayFixFlat<TInt>* valueArray = NULL;
    TRAP(err, valueArray = new (ELeave) CArrayFixFlat<TInt>(1));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("No memory while reading EXIF data"));
    err = iUM->iExifMetadata->GetIntegerArrayParam(KTagID, 0, *valueArray);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Exif Offset, error = %d"), err);
        }
    else
        {
        if (valueArray->Count() != 0)
            {
            INFO_PRINTF2(_L("Exif Offset : %d"), valueArray->At(0));
            }
        }
    delete valueArray; valueArray = NULL;

    KTagID = 0x920a;
    CArrayFixFlat<TRational>* valueRArray = NULL;
    TRAP(err, valueRArray = new (ELeave) CArrayFixFlat<TRational>(1));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("No memory while reading EXIF data"));
    err = iUM->iExifMetadata->GetRationalArrayParam(KTagID, 0, *valueRArray);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Focal Length, error = %d"), err);
        }
    else
        {
        if (valueRArray->Count() != 0)
            {
            INFO_PRINTF2(_L("Focal Length, Numerator  : %d"), valueRArray->At(0).iNumerator);
            INFO_PRINTF2(_L("Focal Length, Denominator: %d"), valueRArray->At(0).iDenominator);
            }
        }
    delete valueRArray; valueRArray = NULL;

    KTagID = 0x0103;
    CArrayFixFlat<TUint16>* valueUArray = NULL;
    TRAP(err, valueUArray = new (ELeave) CArrayFixFlat<TUint16>(1));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("No memory while reading EXIF data"));
    err = iUM->iExifMetadata->GetShortArrayParam(KTagID, 1, *valueUArray);
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("Tag not available or Could not read Compression, error = %d"), err);
        }
    else
        {
        INFO_PRINTF2(_L("Compression : %d"), valueUArray->At(0));
        }
    delete valueUArray; valueUArray = NULL;
    CleanupStack::PopAndDestroy(1, exifImageDecoder);
    }


// Decode the image to a bitmap, time is recorded before the conversion starts
void RStepBaseImageDecoderPerformance::DecodeToBitmapL(const TSize aDestSize)
    {
    // create a destination bitmap
    delete iUM->iDestBitmap;
    iUM->iDestBitmap = NULL;
    iUM->iDestBitmap = new (ELeave) CFbsBitmap();

    // create the destination bitmap with the specified size and display mode (default display mode - EColor16M)
    TInt err = iUM->iDestBitmap->Create(aDestSize, FrameDisplayMode());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not create destination bitmap"));

    iUM->iActiveListener->InitialiseActiveListener();

    // record time and start decoding the image
    iUM->iStartTime.HomeTime();
    iUM->iImageDecoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap);
    }


// Scale the bitmap that is decoded from the image; this method is not called when decoder downscaling capabilities are used
void RStepBaseImageDecoderPerformance::ScaleBitmapL(const TSize aScaleSize)
    {
    delete iUM->iBmpScaler;
    iUM->iBmpScaler = NULL;
    iUM->iBmpScaler = CBitmapScaler::NewL();
    iUM->iActiveListener->InitialiseActiveListener();
    iUM->iBmpScaler->Scale(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap, aScaleSize);
    }


// Rotate the bitmap that is decoded from the image; can also mirror the bitmap
void RStepBaseImageDecoderPerformance::RotateBitmapL(const CBitmapRotator::TRotationAngle aAngle)
    {
    delete iUM->iBmpRotator;
    iUM->iBmpRotator = NULL;
    iUM->iBmpRotator = CBitmapRotator::NewL();
    iUM->iActiveListener->InitialiseActiveListener();
    iUM->iBmpRotator->Rotate(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap, aAngle);
    }


// override from RStepBaseImageDecoder
void RStepBaseImageDecoderPerformance::DoKickoffTestL()
    {
    // read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());

    // for decoding performance tests, the following options is always set
    iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EPreferFastDecode | 
                                                     CImageDecoder::EOptionNoDither);

    SetThreadForDecoding(iUM->iDeOptions); // set the option if decoding is performed in a separate thread
    User::LeaveIfError(CreateImageDecoder());

    LogTestCaseDescription();

    iUM->iActiveListener = new (ELeave) CActiveListener(this); // create an active listener

    TRAPD(err, SetImageTypeL());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));

    // read some EXIF tags and log the data
    // reading EXIF data will not count towards time to be measured for decoding
    // proceed even if reading EXIF data is unsuccessful (except when leave occurs); we want to measure time
    ReadExifDataFromImageL();

    TRAP(err, FsmL());
    if (err != KErrNone)
        {
        StopTest(err, EFail);
        }
    }


RStepImageDecodePerf* RStepImageDecodePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodePerf(aStepName, aTestCaseDescription));
    }

RStepImageDecodePerf::RStepImageDecodePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderPerformance(aStepName, aTestCaseDescription)
    {
    }

void RStepImageDecodePerf::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            DecodeToBitmapL(ImageSize());
            StateChange();
            break;

        case 1:
            iUM->iEndTime.HomeTime(); // record end time of decoding
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Decode unsuccessful"));
            INFO_PRINTF1(_L("Decode successful!"));
            CheckPerformanceOutputAndSaveL();

            // do not bitmap-compare for output images of tests for which 
            // reference images are large and hence not added to the branch
            if (iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0120-HP")) != 0)
                {
                BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
                }

            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepImageDecodeScalePerf* RStepImageDecodeScalePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodeScalePerf(aStepName, aTestCaseDescription));
    }

RStepImageDecodeScalePerf::RStepImageDecodeScalePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderPerformance(aStepName, aTestCaseDescription)
    {
    }

void RStepImageDecodeScalePerf::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonScalingInputFromConfigFile());
            GetScalingSizeL(ImageSize(), iUM->iScaleSize);
            INFO_PRINTF3(_L("Downscale size: width = %d, height = %d"), iUM->iScaleSize.iWidth, iUM->iScaleSize.iHeight);
            DecodeToBitmapL(iUM->iScaleSize); // downscale image using decoder downscaling capabilities
            StateChange();
            break;

        case 1:
            iUM->iEndTime.HomeTime(); // record end time of decoding with downscaling
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Decode with downscaling unsuccessful"));
            INFO_PRINTF1(_L("Decode with downscaling successful!"));
            CheckPerformanceOutputAndSaveL();

            // do not bitmap-compare for output images of tests for which 
            // reference images are large and hence not added to the branch
            if (iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0140-HP")) != 0)
                {
                BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
                }

            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepImageDecodeDoubleScalePerf* RStepImageDecodeDoubleScalePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodeDoubleScalePerf(aStepName, aTestCaseDescription));
    }

RStepImageDecodeDoubleScalePerf::RStepImageDecodeDoubleScalePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderPerformance(aStepName, aTestCaseDescription)
    {
    }

TInt RStepImageDecodeDoubleScalePerf::ReadTestStepInputFromConfigFile()
    {
    if (iUM->iScalingForm == KScalingFormReductionFactor) // form is used when reduction factor is specified
        {
        if (!GetIntFromConfig(iSectionName, _L("scalereductionfactor2"), iReductionFactor2))
            {
            return KErrNotFound;
            }
        }
    else // form KScalingFormDesiredSize is used when destination width and height are specified
        {
        if ((!GetIntFromConfig(iSectionName, _L("desiredscalewidth2"), iScaleSize2.iWidth)) || 
            (!GetIntFromConfig(iSectionName, _L("desiredscaleheight2"), iScaleSize2.iHeight)))
            {
            return KErrNotFound;
            }
        }
    return KErrNone;
    }

void RStepImageDecodeDoubleScalePerf::FsmL()
    {
    TInt err = KErrNone;
    TSize tmpOrigSize;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonScalingInputFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            GetScalingSizeL(ImageSize(), iUM->iScaleSize);
            INFO_PRINTF3(_L("First downscale size: width = %d, height = %d"), iUM->iScaleSize.iWidth, iUM->iScaleSize.iHeight);
            DecodeToBitmapL(iUM->iScaleSize); // downscale image using decoder downscaling capabilities
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Decode with first downscaling unsuccessful"));

            tmpOrigSize = iUM->iScaleSize;

            // decoding with downscaling to 1/8 of original image size is successful
            // now downscale the image to 1/2 of 1/8 of original
            if (iUM->iScalingForm == KScalingFormReductionFactor)
                {
                iUM->iReductionFactor = iReductionFactor2;
                }
            else
                {
                iUM->iScaleSize = iScaleSize2; // KScalingFormDesiredSize
                }
            GetScalingSizeL(tmpOrigSize, iUM->iScaleSize);
            INFO_PRINTF3(_L("Second downscale size: width = %d, height = %d"), iUM->iScaleSize.iWidth, iUM->iScaleSize.iHeight);
            ScaleBitmapL(iUM->iScaleSize); // use bitmap scaler as decoder downscaling capabilities cannot be used second time
            StateChange();
            break;

        case 2:
            iUM->iEndTime.HomeTime(); // record end time of decoding with downscaling
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Decode with double downscaling unsuccessful"));
            INFO_PRINTF1(_L("Decode with double downscaling successful!"));
            CheckPerformanceOutputAndSaveL();
            BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepImageDecodeRotatePerf* RStepImageDecodeRotatePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodeRotatePerf(aStepName, aTestCaseDescription));
    }

RStepImageDecodeRotatePerf::RStepImageDecodeRotatePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderPerformance(aStepName, aTestCaseDescription)
    {
    }

TInt RStepImageDecodeRotatePerf::ReadTestStepInputFromConfigFile()
    {
    // mirroring values can also specified in the rotationindex, same index
    if (!GetIntFromConfig(iSectionName, _L("rotationindex"), iUM->iRotationIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepImageDecodeRotatePerf::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            DecodeToBitmapL(ImageSize());
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Decode unsuccessful"));
            INFO_PRINTF1(_L("Decode successful!"));

            // decoding is successful, rotate/mirror the bitmap
            CBitmapRotator::TRotationAngle angle;
            User::LeaveIfError(GetRotationAngle(iUM->iRotationIndex, angle));
            RotateBitmapL(angle);
            StateChange();
            break;

        case 2:
            iUM->iEndTime.HomeTime(); // record end time after rotation/mirror
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotation or Mirror unsuccessful"));
            INFO_PRINTF1(_L("Rotation or Mirror successful!"));
            CheckPerformanceOutputAndSaveL();

            // do not bitmap-compare for output images of tests for which 
            // reference images are large and hence not added to the branch
            if ((iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0220-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0320-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0350-HP")) != 0))
                {
                BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
                }

            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepImageDecodeRotateMirrorPerf* RStepImageDecodeRotateMirrorPerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodeRotateMirrorPerf(aStepName, aTestCaseDescription));
    }

RStepImageDecodeRotateMirrorPerf::RStepImageDecodeRotateMirrorPerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderPerformance(aStepName, aTestCaseDescription)
    {
    }

TInt RStepImageDecodeRotateMirrorPerf::ReadTestStepInputFromConfigFile()
    {
    // different indexes for rotation and mirror values
    if ((!GetIntFromConfig(iSectionName, _L("rotationindex"), iUM->iRotationIndex)) || 
        (!GetIntFromConfig(iSectionName, _L("mirroringindex"), iUM->iMirroringIndex)) || 
        (!GetBoolFromConfig(iSectionName, _L("operationorder"), iOperationOrder)))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepImageDecodeRotateMirrorPerf::FsmL()
    {
    TInt err = KErrNone;
    CBitmapRotator::TRotationAngle angle;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            DecodeToBitmapL(ImageSize());
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Decode unsuccessful"));
            INFO_PRINTF1(_L("Decode successful!"));

            // decoding is successful, rotate/mirror the bitmap
            if (iOperationOrder) // true (1) => rotation first and mirroring next
                {
                User::LeaveIfError(GetRotationAngle(iUM->iRotationIndex, angle));
                }
            else                      // mirroring first and rotation next
                {
                User::LeaveIfError(GetRotationAngle(iUM->iMirroringIndex, angle));
                }
            RotateBitmapL(angle);
            StateChange();
            break;

        case 2:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotation and/or Mirror unsuccessful"));

            // rotate/mirror the bitmap
            if (iOperationOrder) // mirroring
                {
                User::LeaveIfError(GetRotationAngle(iUM->iMirroringIndex, angle));
                }
            else                 // rotation
                {
                User::LeaveIfError(GetRotationAngle(iUM->iRotationIndex, angle));
                }
            RotateBitmapL(angle);
            StateChange();
            break;

        case 3:
            iUM->iEndTime.HomeTime(); // record end time after rotation and/or mirror
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotation and/or Mirror unsuccessful"));
            INFO_PRINTF1(_L("Rotation and/or Mirror successful!"));
            CheckPerformanceOutputAndSaveL();

            // do not bitmap-compare for output images of tests for which 
            // reference images are large and hence not added to the branch
            if ((iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0420-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-PERF-I-0520-HP")) != 0))
                {
                BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
                }

            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }
