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
// This file contains test step class implementation for rotation and/or mirroring during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_rotation.h"



// RStepBaseRotateAndMirrorDuringDecode
RStepBaseRotateAndMirrorDuringDecode::RStepBaseRotateAndMirrorDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    }


// Read common input from the configuration file
TInt RStepBaseRotateAndMirrorDuringDecode::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of the section
    iSectionName = iTestStepName.Right(KSectionNameLen);

    if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iUM->iAlwaysThread)) || 
        (!GetIntFromConfig(iSectionName, _L("operationindex"), iUM->iOperationIndex)))
        {
        return KErrNotFound;
        }
	
	//if the decoderuid present, then  pass it to decoder, else decoder created using default uid
	if ((GetHexFromConfig(iSectionName, _L("decoderuid"), iUM->iDecUid)))
		{
		if(iUM->iDecUid != 0)
			{
			iUM-> iDecoderUid.iUid = iUM->iDecUid;
			}
		}

	return ReadCommonInputImageFromConfigFile();
    }


// override from RStepBaseImageDecoder
void RStepBaseRotateAndMirrorDuringDecode::DoKickoffTestL()
    {
    TInt err = KErrNone;
    // read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());
	
	// If ETrue-Apply autorotate while decoding, else proceed as is
	if (CheckAutoRotate())
		{
		iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtRotation | 
                                         CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                         CImageDecoder::EOptionExtMirrorVerticalAxis |
                                         CImageDecoder::EOptionAutoRotate |
                                         CImageDecoder::EPreferFastDecode);		
		}
    else 
    	{
    	iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtRotation | 
                                         CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                         CImageDecoder::EOptionExtMirrorVerticalAxis |
                                         CImageDecoder::EPreferFastDecode);
    	}

    SetThreadForDecoding(iUM->iDeOptions); // set the option if decoding is performed in a separate thread
    if (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0110-HP")) != 0)
        {
        err = CreateImageDecoder();
   
        // Rotate and mirror during decode of image, output as a YUV pixel buffer is not supported
        // When support is given, the following checking is to be removed
        if ((iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0040-HP")) == 0) || 
            (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0060-HP")) == 0) ||
            (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0050-HP")) == 0))
            {
            if (err == KErrNotFound)
                {
                INFO_PRINTF2(_L("Autorotate, Rotate and mirror during decode of image, output as a YUV pixel buffer - not supported, expected error = %d"), err);
                StopTest(EPass);
                return;
                }
            }
        User::LeaveIfError(err);
        LogTestCaseDescription();
        }
    iUM->iActiveListener = new (ELeave) CActiveListener(this); // create an active listener

    TRAP(err, FsmL());
    if (err != KErrNone)
        {
        StopTest(err, EFail);
        }
    }


RStepRotateOrMirrorDuringDecodeRotateMirror* RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeRotateMirror(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeRotateMirror::RStepRotateOrMirrorDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepRotateOrMirrorDuringDecodeRotateMirror::ReadTestStepInputFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

    if (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepRotateOrMirrorDuringDecodeRotateMirror::FsmL()
    {
    TInt err = KErrNone;
    TPtrC tmpKeyValue;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            if (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0110-HP")) == 0)
                {
                TPtrC tmpKeyValue;
                if (!GetStringFromConfig(iSectionName, _L("outputimage1"), tmpKeyValue))
                   {
                   User::LeaveIfError(KErrNotFound);
                   }
                iUM->iOutputImage1.Append(tmpKeyValue);
                EncodeAndSetExifL();
                StateChange();
                break;
                }
            TRAP(err, SetImageTypeL());
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));

            LogFrameDisplayMode();
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            if ((iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0110-HP")) == 0 || iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0160-HP")) == 0) && !iUM->iSetExif)
                {
                if (err != KErrNone)
                    {
                    INFO_PRINTF2(_L("Encoding with setting of exif tag leave with unexpected error: %d"), err);
                    StopTest(EFail);
                    break;
                    }
                else
                    {
                    if (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0160-HP")) == 0)
                        {
                        if (iUM->iIterations == 0)
                            {
                            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
                            User::LeaveIfError(SaveDecodedImage()); // save the output image
                            BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
                            delete iUM->iImageDecoder;
                            iUM->iImageDecoder = NULL;
                            if (err != KErrNone)
                                {
                                INFO_PRINTF2(_L("Decoding  with exif tag leave with unexpected error: %d"), err);
                                StopTest(EFail);
                                break;
                                }
                            iUM->iInputImage.Copy(iUM->iOutputImage);
                            if (!GetStringFromConfig(iSectionName, _L("outputimage1"), tmpKeyValue))
                               {
                               User::LeaveIfError(KErrNotFound);
                               }
                            iUM->iOutputImage1.Append(tmpKeyValue);
                            EncodeAndSetExifL();
                            iUM->iIterations = 1;
                            iUM->iState = 1;
                            break;
                            }
                        else if (iUM->iIterations ==1)
                            {
                            if (err != KErrNone)
                                {
                                INFO_PRINTF2(_L("Decoding  with exif tag leave with unexpected error: %d"), err);
                                StopTest(EFail);
                                break;
                                }
                            INFO_PRINTF1(_L("Encoding  with exif tag successful"));
                            iUM->iInputImage.Copy(iUM->iOutputImage1);
                            TPtrC tmpKeyValue;
                            if (!GetStringFromConfig(iSectionName, _L("outputimage2"), tmpKeyValue))
                                {
                                User::LeaveIfError(KErrNotFound);
                                }
                            iUM->iOutputImage.Copy(tmpKeyValue);
                            }
                        }
                    else
                        {
                        iUM->iInputImage.Copy(iUM->iOutputImage1);
                        }
                    iUM->iSetExif = ETrue;
                    err = CreateImageDecoder();
                    User::LeaveIfError(err);                                    
                    TRAP(err, SetImageTypeL());
                    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));
                    
                    LogFrameDisplayMode();
                    SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
                    DecodeToBitmapDuringDecodeL(iUM->iDestSize);
                    iUM->iState = 1;
                    break;
                    }
                }
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0730-HP")) == 0)
                {
                if (err != iUM->iExpectedError)
                    {
                    INFO_PRINTF2(_L("Prepare leave with unexpected error: %d"), err);
                    StopTest(EFail);
                    break;
                    }
				INFO_PRINTF2(_L("Prepare Leave with expected error: %d"), err);
				StopTest(EPass);
                break;
                }
            if (FrameDisplayMode() == EColor16MAP)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF1(_L("Display mode not supported, expected"));
                    StopTest(EPass);
                    break;
                    }
                }
			
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0710-HP")) == 0)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Rotate and mirror during decode of progressive image not supported, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Rotate and mirror  during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0160-HP")) != 0)
                {
                if (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0160-HP")) == 0)
                    {
                    if (!GetStringFromConfig(iSectionName, _L("referenceimage1"), tmpKeyValue))
                        {
                        User::LeaveIfError(KErrNotFound);
                        }
                    iUM->iReferenceImage.Copy(tmpKeyValue);
                    }
                BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
                }
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }

RStepRotateOrMirrorBufferDuringDecodeRotateMirror* RStepRotateOrMirrorBufferDuringDecodeRotateMirror::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorBufferDuringDecodeRotateMirror(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorBufferDuringDecodeRotateMirror::RStepRotateOrMirrorBufferDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    iDescriptorTest = ETrue;
    }

TInt RStepRotateOrMirrorBufferDuringDecodeRotateMirror::ReadTestStepInputFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

    if (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepRotateOrMirrorBufferDuringDecodeRotateMirror::FsmL()
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
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeBufferToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            
            if((err != KErrNone) && (err != KErrUnderflow))
            	{
            	User::Leave(err);	
            	}
			
			while(err == KErrUnderflow)
				{
				IncrementBufferSize();
				iUM->iActiveListener->InitialiseActiveListener();
				iUM->iImageDecoder->ContinueConvert(&iUM->iActiveListener->iStatus);
				CActiveScheduler::Start();
				err = iUM->iActiveListener->iStatus.Int();
				if((err != KErrNone) && (err != KErrUnderflow))
					{
					User::Leave(err);
					}					
				}
            if (FrameDisplayMode() == EColor16MAP)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF1(_L("Display mode not supported, expected"));
                    StopTest(EPass);
                    break;
                    }
                }

            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0710-HP")) == 0)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Rotate and mirror during decode of progressive image not supported, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Rotate and mirror  during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0160-HP")) != 0)
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

RStepRotateOrMirrorIntentDuringDecodeRotateMirror* RStepRotateOrMirrorIntentDuringDecodeRotateMirror::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorIntentDuringDecodeRotateMirror(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorIntentDuringDecodeRotateMirror::RStepRotateOrMirrorIntentDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    iIntentTest = ETrue;
    }

TInt RStepRotateOrMirrorIntentDuringDecodeRotateMirror::ReadTestStepInputFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

    if (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepRotateOrMirrorIntentDuringDecodeRotateMirror::FsmL()
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
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
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

            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0710-HP")) == 0)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Rotate and mirror during decode of progressive image not supported, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Rotate and mirror  during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0160-HP")) != 0)
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


RStepRotateOrMirrorDRMDuringDecodeRotateMirror* RStepRotateOrMirrorDRMDuringDecodeRotateMirror::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDRMDuringDecodeRotateMirror(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDRMDuringDecodeRotateMirror::RStepRotateOrMirrorDRMDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    iDRMTest = ETrue;
    }

TInt RStepRotateOrMirrorDRMDuringDecodeRotateMirror::ReadTestStepInputFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

    if (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepRotateOrMirrorDRMDuringDecodeRotateMirror::FsmL()
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
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
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

            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0710-HP")) == 0)
                {
                if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Rotate and mirror during decode of progressive image not supported, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                else if (err == KErrNone)
                    {
                    ERR_PRINTF1(_L("Rotate and mirror  during decode successful, unexpected"));
                    StopTest(EFail);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0160-HP")) != 0)
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

RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer* RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    iYuvTest = ETrue; // indicates tests related to YUV pixel buffer
    }

TInt RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::ReadTestStepInputFromConfigFile()
    {
    if (!GetIntFromConfig(iSectionName, _L("yuvformatindex"), iUM->iYUVFormatIndex))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            // added for coverage purpose
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0045-HP")) != 0)
            	{
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            	}            
            DecodeToYuvPixelBufferDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(CreateOutputDirectory());
            EncodeYuvPixelBufferToAnImageL();
            StateChange();
            break;

        case 2:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Encoding YUV pixel buffer to an image unsuccessful"));
            INFO_PRINTF1(_L("Encoding YUV pixel buffer to an image successful!"));
            CompareImagesL();
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepRotateOrMirrorDuringDecodeColourSample* RStepRotateOrMirrorDuringDecodeColourSample::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeColourSample(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeColourSample::RStepRotateOrMirrorDuringDecodeColourSample(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepRotateOrMirrorDuringDecodeColourSample::ReadTestStepInputFromConfigFile()
    {
    if ((!GetIntFromConfig(iSectionName, _L("colorsamplingindex"), iUM->iColorSamplingIndex)) || 
        (!GetIntFromConfig(iSectionName, _L("qualityfactor"), iUM->iQualityFactor)))
        {
        return KErrNotFound;
        }
    return KErrNone;
    }

void RStepRotateOrMirrorDuringDecodeColourSample::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            SetImageDataL(); // color sampling and quality factor
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepRotateOrMirrorDuringDecodeScale* RStepRotateOrMirrorDuringDecodeScale::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeScale(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeScale::RStepRotateOrMirrorDuringDecodeScale(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepRotateOrMirrorDuringDecodeScale::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0390-HP")) != 0)
                {
                User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
                }
            User::LeaveIfError(ReadCommonExtensionScalingInputFromConfigFile());

            TRAP(err, SetExtensionL(CUtilityMembers::EExtensionScaleOperation, iUM->iDestSize));

            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0390-HP")) == 0)
                {
                 if (err == KErrNotSupported)
                    {
                    INFO_PRINTF2(_L("Scaling desired size is unsuccessful, expected error = %d"), err);
                    StopTest(EPass);
                    break;
                    }
                }
            INFO_PRINTF3(_L("Downscale size: width = %d, height = %d"), iUM->iDestSize.iWidth, iUM->iDestSize.iHeight);
            INFO_PRINTF1(_L("Order of operation: Scale and Rotation and/or Mirror"));
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepRotateOrMirrorDuringDecodePerf* RStepRotateOrMirrorDuringDecodePerf::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodePerf(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodePerf::RStepRotateOrMirrorDuringDecodePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepRotateOrMirrorDuringDecodePerf::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            iUM->iExtPerfTest = ETrue; // indicates tests related to performance measurements
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadCommonInputExpectedPerfTimeFromConfigFile());
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            iUM->iEndTime.HomeTime(); // record end time for decoding
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            CheckPerformanceOutputAndSaveL();

            // do not bitmap-compare for output images of tests for which 
            // reference images are large and hence not added to the branch
            if ((iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0430-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0440-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0450-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0470-HP")) != 0) && 
                (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0490-HP")) != 0))
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


RStepRotateOrMirrorDuringDecodeInterop* RStepRotateOrMirrorDuringDecodeInterop::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeInterop(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeInterop::RStepRotateOrMirrorDuringDecodeInterop(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepRotateOrMirrorDuringDecodeInterop::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0590-HP")) == 0)
                {
                User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
                }
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            if (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0600-HP")) == 0)
                {
                if (err == KErrUnderflow)
                    {
                    INFO_PRINTF2(_L("Rotate and/or mirror during decode unsuccessful, expected error = %d"), KErrUnderflow);
                    StopTest(EPass);
                    break;
                    }
                }

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepRotateOrMirrorDuringDecodeCancel* RStepRotateOrMirrorDuringDecodeCancel::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeCancel(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeCancel::RStepRotateOrMirrorDuringDecodeCancel(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepRotateOrMirrorDuringDecodeCancel::ReadTestStepInputFromConfigFile()
    {
    TInt time;
    if (!GetIntFromConfig(iSectionName, _L("timerexpire"), time))
        {
        return KErrNotFound;
        }
	iUM->iTimerExpire = TTimeIntervalMicroSeconds32(time);
	return KErrNone;		
    }

void RStepRotateOrMirrorDuringDecodeCancel::TimerExpired()
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

void RStepRotateOrMirrorDuringDecodeCancel::FsmL()
    {
    TInt err = KErrNone;
    TUint32 decodeCounter = 0;
    switch (iUM->iState)
        {
        case 0:
            iUM->iConvCancelTest = ETrue;
            if ((iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0630-HP")) == 0) || 
                (iTestStepName.Compare(_L("MM-ICL-DEC-RTMR-I-0650-HP")) == 0) ||
                (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0170-HP")) == 0))
                {
                iUM->iRestartConvAfterCancel = ETrue;
                }

            User::LeaveIfError(ReadTestStepInputFromConfigFile());
            if (iUM->iRestartConvAfterCancel)
                {
                User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
                }
            iUM->iTimer = CUtilityTimer::NewL(iUM->iTimerExpire, *this);
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            iUM->iEndDecodeCounter = User::FastCounter();
            decodeCounter = iUM->iEndDecodeCounter - iUM->iBeginDecodeCounter;
            iUM->iActiveListener->Cancel();
			if (!iUM->iConvCancelled)
				{
				ERR_PRINTF1(_L("Rotate and/or mirror during decode unsuccessful"));
				StopTest(EFail);
				break;
				}
            else
                {
                INFO_PRINTF2(_L("%d fast counters since the start of decode"), decodeCounter);
                INFO_PRINTF1(_L("Cancelled rotate and/or mirror during decoding successfully"));
                }

            if (!iUM->iRestartConvAfterCancel)
                {
                // test is not required to start after cancellation
                StopTest(EPass);
                break;
                }

            INFO_PRINTF1(_L("Restarted rotate and/or mirror during decode"));
            iUM->iActiveListener->InitialiseActiveListener();
            iUM->iImageDecoder->Convert(&iUM->iActiveListener->iStatus, *iUM->iDestBitmap); // start conversion after cancellation
            StateChange();
            break;

        case 2:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }


RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage* RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage(aStepName, aTestCaseDescription));
    }

RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage::RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseRotateAndMirrorDuringDecode(aStepName, aTestCaseDescription)
    {
    }

TInt RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage::ReadTestStepInputFromConfigFile()
    {
    TPtrC tmpKeyValue;
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

    if (!GetStringFromConfig(iSectionName, _L("referenceimage2"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iReferenceImage2.Append(tmpKeyValue);

    if (!GetStringFromConfig(iSectionName, _L("outputimage2"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iOutputImage2.Append(tmpKeyValue);
    return KErrNone;
    }

void RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
            User::LeaveIfError(ReadTestStepInputFromConfigFile());

            TRAP(err, SetImageTypeL());
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));

            if (iUM->iImageType.Compare(_L("thumbnail")) == 0)
                {
                // decode thumbnail first
                INFO_PRINTF1(_L("Thumbnail set"));
                }
            else
                {
                // decode main image first
                INFO_PRINTF1(_L("Main image set"));
                }

            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            User::LeaveIfError(SaveDecodedImage()); // save the output image

            // if thumbnail is read from ini file, it means it is already decoded, hence set main image now
            if (iUM->iImageType.Compare(_L("thumbnail")) == 0)
                {
                // decode main image next
                iUM->iImageType.Copy(_L("main"));
                INFO_PRINTF1(_L("Setting main image..."));
                }
            else
                {
                // decode thumbnail next
                iUM->iImageType.Copy(_L("thumbnail"));
                INFO_PRINTF1(_L("Setting thumbnail..."));
                }

            TRAP(err, SetImageTypeL());
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));
            SetExtensionL(CUtilityMembers::EExtensionOperation, iUM->iDestSize);
            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 2:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Rotate and/or mirror during decode unsuccessful"));
            INFO_PRINTF1(_L("Rotate and/or mirror during decode successful!"));
            iUM->iReferenceImage = iReferenceImage2;
            iUM->iOutputImage = iOutputImage2;
            User::LeaveIfError(SaveDecodedImage()); // save the output image
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }
