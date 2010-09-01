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
// This file contains test step class implementation for scaling during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_scale.h"


// RStepBaseScaleDuringDecode
RStepBaseScaleDuringDecode::RStepBaseScaleDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    }


// Read common input from the configuration file
TInt RStepBaseScaleDuringDecode::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of the section in the configuration file
    iSectionName = iTestStepName.Right(KSectionNameLen);

    TPtrC tmpKeyValue;
    if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iUM->iAlwaysThread)) || 
        (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex)))
        {
        return KErrNotFound;
        }

    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
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

    if (ReadCommonExtensionScalingInputFromConfigFile() == KErrNotFound)
        {
        return KErrNotFound;
        }
    return ReadCommonInputImageFromConfigFile();
    }


// Check the output image
//      - compare scale size of the scaled image (bitmap) with the input scale size
//      - compare output bitmap with reference image
//      - save the output bitmap to a file
void RStepBaseScaleDuringDecode::CheckScaleOutputAndSaveL()
    {
    User::LeaveIfError(SaveDecodedImage()); // save the output image

    INFO_PRINTF3(_L("Scaled image size: width = %d, height = %d"), 
        iUM->iDestBitmap->SizeInPixels().iWidth, iUM->iDestBitmap->SizeInPixels().iHeight);

    TInt roundup = 0;

    // calculate the output image size for comparison
    if (!iUM->iScalerExtUseDesiredSizeMethod)
        {
        iUM->iReductionFactor = -(iUM->iScalingCoeff + 1);
        if (iUM->iReductionFactor > 0)
            {
            roundup = iUM->iReductionFactor > 0 ? (1 << iUM->iReductionFactor) - 1: 0;
            iUM->iExtScaleSize.SetSize(((ImageSize().iWidth + roundup) >> iUM->iReductionFactor), 
                ((ImageSize().iHeight + roundup) >> iUM->iReductionFactor));
            }
        else
            {
            // no scaling
            iUM->iExtScaleSize = ImageSize();
            }

        if (iUM->iExtScaleSize == iUM->iDestBitmap->SizeInPixels())
            {
            INFO_PRINTF1(_L("Scaled image size correct"));
            }
        else
            {
            ERR_PRINTF1(_L("Scaled image size incorrect"));
            User::Leave(KErrGeneral);
            }
        }

    if ((iTestStepName.Compare(_L("MM-ICL-DEC-SCAL-I-0200-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-SCAL-I-0205-HP")) != 0))
        {
        BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
        }
    }


// override from RStepBaseImageDecoder
void RStepBaseScaleDuringDecode::DoKickoffTestL()
    {
    // read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());
    User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());
    // If ETrue-Apply autorotate while decoding, else proceed as is
    if (CheckAutoRotate())
        {
        iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtScaling |
                                                 CImageDecoder::EOptionAutoRotate |
                                                 CImageDecoder::EPreferFastDecode);
        }
    else
        {
        iUM->iDeOptions = static_cast<CImageDecoder::TOptions> (CImageDecoder::EOptionExtScaling | 
                                                             CImageDecoder::EPreferFastDecode);
        }

    SetThreadForDecoding(iUM->iDeOptions); // set the option if decoding is performed in a separate thread
    User::LeaveIfError(CreateImageDecoder());

    LogTestCaseDescription();

    iUM->iActiveListener = new (ELeave) CActiveListener(this); // create an active listener

    TRAPD(err, FsmL());
    if (err != KErrNone)
        {
        StopTest(err, EFail);
        }
    }


RStepScaleDuringDecode* RStepScaleDuringDecode::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepScaleDuringDecode(aStepName, aTestCaseDescription));
    }

RStepScaleDuringDecode::RStepScaleDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseScaleDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepScaleDuringDecode::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            TRAP(err, SetImageTypeL());
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));
            LogFrameDisplayMode();
            SetExtensionL(CUtilityMembers::EExtensionScale, iUM->iDestSize);
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

            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Scale during decode unsuccessful"));
            INFO_PRINTF1(_L("Scale during decode successful!"));
            CheckScaleOutputAndSaveL();
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }
