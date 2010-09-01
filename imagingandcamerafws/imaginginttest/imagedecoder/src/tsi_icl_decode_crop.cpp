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
// This file contains test step class implementation for crop during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_crop.h"


// RStepBaseCropDuringDecode
RStepBaseCropDuringDecode::RStepBaseCropDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    }


// Read common input from the configuration file
TInt RStepBaseCropDuringDecode::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of the section in the configuration file
    iSectionName = iTestStepName.Right(KSectionNameLen);

    // iUM->iCropPointA is common for both cropping rectangle forms
    if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iUM->iAlwaysThread)) || 
        (!GetIntFromConfig(iSectionName, _L("croppingrectform"), iUM->iCroppingRectForm)) || 
        (!GetIntFromConfig(iSectionName, _L("cropax"), iUM->iCropPointA.iX)) || 
        (!GetIntFromConfig(iSectionName, _L("cropay"), iUM->iCropPointA.iY)))
        {
        return KErrNotFound;
        }

    if (iUM->iCroppingRectForm == KCroppingFormPointAAndB)
        {
        if ((!GetIntFromConfig(iSectionName, _L("cropbx"), iUM->iCropPointB.iX)) || 
            (!GetIntFromConfig(iSectionName, _L("cropby"), iUM->iCropPointB.iY)))
            {
            return KErrNotFound;
            }
        }
    else // KCroppingFormPointAndSize
        {
        if ((!GetIntFromConfig(iSectionName, _L("cropwidth"), iUM->iCropSize.iWidth)) || 
            (!GetIntFromConfig(iSectionName, _L("cropheight"), iUM->iCropSize.iHeight)))
            {
            return KErrNotFound;
            }
        }
	
	//if the decoderuid present, then  pass it to decoder, else decoder created using default uid
	if ((GetHexFromConfig(iSectionName, _L("decoderuid"), iUM->iDecUid)))
		{
		if(iUM->iDecUid != 0)
			{
			iUM->iDecoderUid.iUid = iUM->iDecUid;	
			}
		}
		        
    return ReadCommonInputImageFromConfigFile();
    }


// Check the output image
//      - compare crop size of the cropped image (bitmap) with the input crop size
//      - compare output bitmap with reference image
//      - save the output bitmap to a file
void RStepBaseCropDuringDecode::CheckCropOutputAndSaveL(const TSize aOutputSize)
    {
    TSize tmpSize;
    if (iYuvTest)
        {
        tmpSize = iUM->iImageFrame->FrameSizeInPixels();
        }
    else
        {
        User::LeaveIfError(SaveDecodedImage()); // save the output image
        tmpSize = iUM->iDestBitmap->SizeInPixels();
        }

    INFO_PRINTF3(_L("Cropped image size: width = %d, height = %d"), tmpSize.iWidth, tmpSize.iHeight);

    if (!iUM->iScalerExtUseDesiredSizeMethod)
        {
        if (aOutputSize == tmpSize)
            {
            INFO_PRINTF1(_L("Cropped image size correct"));
            }
        else
            {
            ERR_PRINTF1(_L("Cropped image size incorrect"));
            User::Leave(KErrGeneral);
            }
        }

    // do not bitmap-compare for output images of tests for which 
    // reference images are large and hence not added to the branch
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0095-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0105-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0210-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0500-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0630-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0660-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0680-HP")) != 0))
        {
        BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
        }
    }


// override from RStepBaseImageDecoder
void RStepBaseCropDuringDecode::DoKickoffTestL()
    {
    // read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());
    // If ETrue-Apply autorotate while decoding, else proceed as is
    if (CheckAutoRotate())
        {
        iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtCrop |
                                                 CImageDecoder::EOptionAutoRotate |
                                                 CImageDecoder::EPreferFastDecode);
        }
    else
        {
        iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtCrop | 
                                                         CImageDecoder::EPreferFastDecode);        
        }


    SetThreadForDecoding(iUM->iDeOptions); // set the option if decoding is performed in a separate thread
    TInt err = CreateImageDecoder();

    // Crop during decode of image, output as a YUV pixel buffer is not supported
    // When support is given, the following checking is to be removed
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0060-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0080-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0100-HP")) == 0))
        {
        if (err == KErrNotFound)
            {
            INFO_PRINTF2(_L("Crop during decode of image, output as a YUV pixel buffer - not supported, expected error = %d"), err);
            StopTest(EPass);
            return;
            }
        }
    User::LeaveIfError(err);

    LogTestCaseDescription();

    iUM->iActiveListener = new (ELeave) CActiveListener(this); // create an active listener

    TRAP(err, FsmL());
    if (err != KErrNone)
        {
        StopTest(err, EFail);
        }
    }


RStepCropDuringDecodeAsBitmap* RStepCropDuringDecodeAsBitmap::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeAsBitmap(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeAsBitmap::RStepCropDuringDecodeAsBitmap(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepCropDuringDecodeAsBitmap::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);

            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0105-HP")) == 0) // no clip test
                {
                INFO_PRINTF1(_L("Cancelling crop during decode... output original image"));
                TRAPD(err, iUM->iImageDecoder->SetClippingRectL(NULL));// reset clipping rectangle, i.e., no cropping
                iUM->iCropSize = ImageSize(); // store original size for comparison later

            	err = iUM->iImageDecoder->GetDestinationSize(iUM->iDestSize, KFrameZero);
            	CHECK_STATUS_LEAVE_IF_ERROR(err, _L("GetDestinationSize() failed"));
                }
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0710-HP")) == 0)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Crop during decode of progressive image not supported, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Crop during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeAsYuvPixelBuffer* RStepCropDuringDecodeAsYuvPixelBuffer::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeAsYuvPixelBuffer(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeAsYuvPixelBuffer::RStepCropDuringDecodeAsYuvPixelBuffer(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    iYuvTest = ETrue; // indicates tests related to YUV pixel buffer
    }

TInt RStepCropDuringDecodeAsYuvPixelBuffer::ReadTestStepInputFromConfigFile()
    {
    if (!GetIntFromConfig(iSectionName, _L("yuvformatindex"), iUM->iYUVFormatIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepCropDuringDecodeAsYuvPixelBuffer::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0100-HP")) != 0)
                {
                User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
                }
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
            DecodeToYuvPixelBufferDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0100-HP")) == 0)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Crop during decode unsuccessful, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Crop during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            User::LeaveIfError(CreateOutputDirectory());
            EncodeYuvPixelBufferToAnImageL();
            StateChange();
            break;

        case 2:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Encoding YUV pixel buffer to an image unsuccessful"));
            INFO_PRINTF1(_L("Encoding YUV pixel buffer to an image successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            CompareImagesL();
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeInvalidDestSize* RStepCropDuringDecodeInvalidDestSize::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeInvalidDestSize(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeInvalidDestSize::RStepCropDuringDecodeInvalidDestSize(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepCropDuringDecodeInvalidDestSize::ReadTestStepInputFromConfigFile()
    {
    if ((!GetIntFromConfig(iSectionName, _L("destwidth"), iUM->iDestSize.iWidth)) || 
        (!GetIntFromConfig(iSectionName, _L("destheight"), iUM->iDestSize.iHeight)))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepCropDuringDecodeInvalidDestSize::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);

            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            INFO_PRINTF3(_L("Destination bitmap size: width = %d, height = %d"), iUM->iDestSize.iWidth, iUM->iDestSize.iHeight);

            if ((iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0095-HP")) == 0) || 
                (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0098-HP")) == 0) ||
                (iTestStepName.Compare(_L("MM-ICL-DEC-ARCR-I-0060-HP")) == 0))
                {
                User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
                iUM->iCropSize = iUM->iDestSize; // store original size for comparison later
                }

            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0090-HP")) == 0)
                {
                if (err == KErrArgument)
                    {
                    INFO_PRINTF2(_L("Crop during decode unsuccessful, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Crop during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeColourSample* RStepCropDuringDecodeColourSample::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeColourSample(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeColourSample::RStepCropDuringDecodeColourSample(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepCropDuringDecodeColourSample::ReadTestStepInputFromConfigFile()
    {
    if ((!GetIntFromConfig(iSectionName, _L("colorsamplingindex"), iUM->iColorSamplingIndex)) || 
        (!GetIntFromConfig(iSectionName, _L("qualityfactor"), iUM->iQualityFactor)))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepCropDuringDecodeColourSample::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
            SetImageDataL(); // color sampling and quality factor
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeDiffRegion* RStepCropDuringDecodeDiffRegion::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeDiffRegion(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeDiffRegion::RStepCropDuringDecodeDiffRegion(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepCropDuringDecodeDiffRegion::ReadTestStepInputFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue) || 
        (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex)))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);
    return KErrNone;
    }

void RStepCropDuringDecodeDiffRegion::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());

            TRAP(err, SetImageTypeL());
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));

            LogFrameDisplayMode();
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            if (FrameDisplayMode() == EColor16MAP)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF1(_L("Display mode not supported, expected"));
                    StopTest(EPass);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeInvalidRect* RStepCropDuringDecodeInvalidRect::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeInvalidRect(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeInvalidRect::RStepCropDuringDecodeInvalidRect(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepCropDuringDecodeInvalidRect::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            TRAP(err, SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize));
            if (err == KErrArgument) // while setting the crop rectangle
                {
                INFO_PRINTF2(_L("Invalid region of interest for crop, expected error = %d"), err);
                StopTest(EPass);
                break;
                }
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            ERR_PRINTF1(_L("Crop during decode successful, unexpected")); // should not decode if region of interest is incorrect
            StopTest(EFail);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeMask* RStepCropDuringDecodeMask::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeMask(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeMask::RStepCropDuringDecodeMask(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepCropDuringDecodeMask::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
            DecodeWithMaskDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during mask decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during mask decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeRepeatCrop* RStepCropDuringDecodeRepeatCrop::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeRepeatCrop(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeRepeatCrop::RStepCropDuringDecodeRepeatCrop(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepCropDuringDecodeRepeatCrop::ReadTestStepInputFromConfigFile()
    {
    iUM->iRepeatCropSizeArray.Reset();
    iUM->iRepeatCropPointAArray.Reset();
    iUM->iRepeatCropPointBArray.Reset();
    iUM->iRepeatCropOutputImageArray.Reset();
    iUM->iRepeatCropReferenceImageArray.Reset();

    // number of times crop is repeated
    if (!GetIntFromConfig(iSectionName, _L("croprepeatcount"), iCropRepeatCount))
        {
        return KErrNotFound;
        }

    // keynames for second and later set of values
    TBuf<KSmallBufferLen> aXCoord;
    TBuf<KSmallBufferLen> aYCoord;
    TBuf<KSmallBufferLen> bXCoord;
    TBuf<KSmallBufferLen> bYCoord;
    TBuf<KSmallBufferLen> widthKey;
    TBuf<KSmallBufferLen> heightKey;
    TBuf<KSmallBufferLen> outputImage;
    TBuf<KSmallBufferLen> referenceImage;

    TInt  x, y;
    TInt  width, height;
    TPtrC tmpKeyValue;

    // read second and later set of values for coordinates, width, height, output and reference images
    for (TInt i = 2; i < iCropRepeatCount + 1; i++)
        {
        aXCoord.Copy(_L("cropax"));
        aYCoord.Copy(_L("cropay"));
        bXCoord.Copy(_L("cropbx"));
        bYCoord.Copy(_L("cropby"));
        widthKey.Copy(_L("cropwidth"));
        heightKey.Copy(_L("cropheight"));
        outputImage.Copy(_L("outputimage"));
        referenceImage.Copy(_L("referenceimage"));

        // append the index to the keynames, read values from the configuration file
        // so the final keynames are cropax2...cropax10..., outputimage2...outputimage10... etc
        aXCoord.AppendNum(i);
        aYCoord.AppendNum(i);
        if ((!GetIntFromConfig(iSectionName, aXCoord, x)) || 
            (!GetIntFromConfig(iSectionName, aYCoord, y)))
            {
            return KErrNotFound;
            }
        iUM->iRepeatCropPointAArray.Append(TPoint(x, y));

        if (iUM->iCroppingRectForm == KCroppingFormPointAAndB)
            {
            bXCoord.AppendNum(i);
            bYCoord.AppendNum(i);
            if ((!GetIntFromConfig(iSectionName, bXCoord, x)) || 
                (!GetIntFromConfig(iSectionName, bYCoord, y)))
                {
                return KErrNotFound;
                }
            iUM->iRepeatCropPointBArray.Append(TPoint(x, y));
            }
        else // KCroppingFormPointAndSize
            {
            widthKey.AppendNum(i);
            heightKey.AppendNum(i);
            if ((!GetIntFromConfig(iSectionName, widthKey, width)) || 
                (!GetIntFromConfig(iSectionName, heightKey, height)))
                {
                return KErrNotFound;
                }
            iUM->iRepeatCropSizeArray.Append(TSize(width, height));
            }

        outputImage.AppendNum(i);
        if ((!GetStringFromConfig(iSectionName, outputImage, tmpKeyValue)))
            {
            return KErrNotFound;
            }
        iUM->iRepeatCropOutputImageArray.Append(tmpKeyValue);

        referenceImage.AppendNum(i);
        if (!GetStringFromConfig(iSectionName, referenceImage, tmpKeyValue))
            {
            return KErrNotFound;
            }
        iUM->iRepeatCropReferenceImageArray.Append(tmpKeyValue);
        }
    return KErrNone;
    }

void RStepCropDuringDecodeRepeatCrop::CropL()
    {
    INFO_PRINTF2(_L("Crop count: %d"), iIndex);
    SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
    DecodeToBitmapDuringDecodeL(iUM->iDestSize);
    StateChange();
    }

void RStepCropDuringDecodeRepeatCrop::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            iIndex++;
            CropL();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);

            if (iIndex != iCropRepeatCount)
                {
                // repeat crop with same image with same or different region of interest; codec loaded once
                iUM->iCropPointA = iUM->iRepeatCropPointAArray[iIndex - 1];

                if (iUM->iCroppingRectForm == KCroppingFormPointAAndB)
                    {
                    iUM->iCropPointB = iUM->iRepeatCropPointBArray[iIndex - 1];
                    }
                else // KCroppingFormPointAndSize
                    {
                    iUM->iCropSize = iUM->iRepeatCropSizeArray[iIndex - 1];
                    }

                iUM->iOutputImage = iUM->iRepeatCropOutputImageArray[iIndex - 1];
                iUM->iReferenceImage = iUM->iRepeatCropReferenceImageArray[iIndex - 1];

                iUM->iState = 0;
                iIndex++;
                CropL();
                }
            else
                {
                StopTest(EPass);
                }
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeScale* RStepCropDuringDecodeScale::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeScale(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeScale::RStepCropDuringDecodeScale(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepCropDuringDecodeScale::FsmL()
    {
    TInt roundup = 0;
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0480-HP")) != 0)
                {
                User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
                }
            User::LeaveIfError(ReadCommonExtensionScalingInputFromConfigFile());

            TRAP(err, SetExtensionL(CUtilityMembers::EExtensionCropScale, iUM->iDestSize));

            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0480-HP")) == 0)
                {
                 if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Scaling desired size is unsuccessful, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                }
            INFO_PRINTF3(_L("Downscale size: width = %d, height = %d"), iUM->iDestSize.iWidth, iUM->iDestSize.iHeight);
            INFO_PRINTF1(_L("Order of operation: Crop and Scale"));
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode with scaling unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode with scaling successful!"));

            // calculate the output image size for comparison
            if (!iUM->iScalerExtUseDesiredSizeMethod)
                {
                iUM->iReductionFactor = -(iUM->iScalingCoeff + 1);
                if (iUM->iReductionFactor > 0)
                    {
                    roundup = iUM->iReductionFactor > 0 ? (1 << iUM->iReductionFactor) - 1: 0;
                    iUM->iExtScaleSize.SetSize(((iUM->iCropSize.iWidth + roundup) >> iUM->iReductionFactor), 
                        ((iUM->iCropSize.iHeight + roundup) >> iUM->iReductionFactor));
                    }
                else
                    {
                    // no scaling
                    iUM->iExtScaleSize = iUM->iCropSize;
                    }
                }
            CheckCropOutputAndSaveL(iUM->iExtScaleSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodePerf* RStepCropDuringDecodePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodePerf(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodePerf::RStepCropDuringDecodePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepCropDuringDecodePerf::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            iUM->iExtPerfTest = ETrue; // indicates tests related to performance measurements
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadCommonInputExpectedPerfTimeFromConfigFile());
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            iUM->iEndTime.HomeTime(); // record end time of cropping during decoding
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            CheckPerformanceOutputAndSaveL();
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeInterop* RStepCropDuringDecodeInterop::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeInterop(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeInterop::RStepCropDuringDecodeInterop(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepCropDuringDecodeInterop::ReadTestStepInputFromConfigFile()
    {
    if (!GetIntFromConfig(iSectionName, _L("operationindex"), iUM->iOperationIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepCropDuringDecodeInterop::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());

            if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0615-HP")) == 0)
                {
                User::LeaveIfError(ReadTestStepInputFromConfigFile());
                SetExtensionL(CUtilityMembers::EExtensionCropOperation, iUM->iDestSize);
                }
            else
                {
                SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
                }
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepCropDuringDecodeCancel* RStepCropDuringDecodeCancel::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepCropDuringDecodeCancel(aStepName, aTestCaseDescription));
    }

RStepCropDuringDecodeCancel::RStepCropDuringDecodeCancel(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseCropDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepCropDuringDecodeCancel::ReadTestStepInputFromConfigFile()
    {
    TInt time;
    if (!GetIntFromConfig(iSectionName, _L("timerexpire"), time))
        {
        return KErrNotFound;
        }
	iUM->iTimerExpire = TTimeIntervalMicroSeconds32(time);
	return KErrNone;		
    }

void RStepCropDuringDecodeCancel::TimerExpired()
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

void RStepCropDuringDecodeCancel::FsmL()
    {
    TInt err = KErrNone;
    TUint32 decodeCounter = 0;
    switch (iUM->iState)
        {
        case 0:
            iUM->iConvCancelTest = ETrue;
            if ((iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0660-HP")) == 0) || 
                (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0680-HP")) == 0))
                {
                iUM->iRestartConvAfterCancel = ETrue;
                }

            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            if (iUM->iRestartConvAfterCancel)
            	{
            	User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            	}
            iUM->iTimer = CUtilityTimer::NewL(iUM->iTimerExpire, *this);
            SetExtensionL(CUtilityMembers::EExtensionCrop, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            iUM->iEndDecodeCounter = User::FastCounter();
            decodeCounter = iUM->iEndDecodeCounter - iUM->iBeginDecodeCounter;
            iUM->iActiveListener->Cancel();
			if (!iUM->iConvCancelled)
				{
				ERR_PRINTF1(_L("Cancel Crop during decode unsuccessful"));
				StopTest(EFail);
				break;
				}
            else
                {
                INFO_PRINTF2(_L("%d fast counters since the start of decode"), decodeCounter);
                INFO_PRINTF1(_L("Cancelled Crop during decode successfully"));
                }

            if (!iUM->iRestartConvAfterCancel)
                {
                // test is not required to start after cancellation
                StopTest(EPass);
                break;
                }

            INFO_PRINTF1(_L("Restarted Crop during decode"));
            iUM->iActiveListener->InitialiseActiveListener();
            iUM->iImageDecoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap); // start conversion after cancellation
            StateChange();
            break;

        case 2:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Crop during decode unsuccessful"));
            INFO_PRINTF1(_L("Crop during decode successful!"));
            CheckCropOutputAndSaveL(iUM->iCropSize);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }
