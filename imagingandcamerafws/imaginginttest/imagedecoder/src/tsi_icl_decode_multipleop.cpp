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
// This file contains test step class implementation for multiple operations 
// (crop, scale and rotate/mirror in this order) during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_multipleop.h"


// RStepBaseMultipleExtensionsDuringDecode
RStepBaseMultipleExtensionsDuringDecode::RStepBaseMultipleExtensionsDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    }


// Read common input from the configuration file
TInt RStepBaseMultipleExtensionsDuringDecode::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of the section in the configuration file
    iSectionName = iTestStepName.Right(KSectionNameLen);

    TPtrC tmpKeyValue;
    if ((!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iUM->iAlwaysThread)) || 
        (!GetBoolFromConfig(iSectionName, _L("clearoperations"), iClearOperationsTest)) || 
        (!GetIntFromConfig(iSectionName, _L("framedisplaymodeindex"), iUM->iFrameDisplayModeIndex)))
        {
        return KErrNotFound;
        }
	//if true, read the decoderuid if false pick the default uid
	if ((GetHexFromConfig(iSectionName, _L("decoderuid"), iUM->iDecUid)))
		{
		if(iUM->iDecUid != 0)
			{
			iUM->iDecoderUid.iUid = iUM->iDecUid;	
			}
		}		
		
    if (!GetStringFromConfig(iSectionName, _L("imagetype"), tmpKeyValue))
        {
        return KErrNotFound;
        }
    iUM->iImageType.Append(tmpKeyValue);

    // read choices, what extensions to add
    if ((!GetBoolFromConfig(iSectionName, _L("cropchoice"), iCropChoice)) ||  // true or false
        (!GetBoolFromConfig(iSectionName, _L("scalechoice"), iScaleChoice)) || 
        (!GetBoolFromConfig(iSectionName, _L("mirrorchoice"), iMirrorChoice)) || 
        (!GetBoolFromConfig(iSectionName, _L("rotationchoice"), iRotationChoice)))
        {
        return KErrNotFound;
        }

    if (iCropChoice) // true (1), false (0)
        {
        iUM->iCroppingRectForm = KCroppingFormPointAndSize; // read in this form for these tests
        if ((!GetIntFromConfig(iSectionName, _L("cropax"), iUM->iCropPointA.iX)) || 
            (!GetIntFromConfig(iSectionName, _L("cropay"), iUM->iCropPointA.iY)) || 
            (!GetIntFromConfig(iSectionName, _L("cropwidth"), iUM->iCropSize.iWidth)) || 
            (!GetIntFromConfig(iSectionName, _L("cropheight"), iUM->iCropSize.iHeight)))
            {
            return KErrNotFound;
            }
        }

    if (iScaleChoice)
        {
        // full size = -1, ½ size = -2, ¼ size = -3, 1/8 size = -4
        if ((!GetIntFromConfig(iSectionName, _L("scalingqualityindex"), iUM->iScalingQualityIndex)) || 
            (!GetIntFromConfig(iSectionName, _L("scalingcoeff"), iUM->iScalingCoeff)))
            {
            return KErrNotFound;
            }
        }

    // rotation and/or mirror
    if ((iMirrorChoice) || (iRotationChoice))
        {
        if (!GetIntFromConfig(iSectionName, _L("operationindex"), iUM->iOperationIndex))
            {
            return KErrNotFound;
            }
        }
    return ReadCommonInputImageFromConfigFile();
    }


TInt RStepBaseMultipleExtensionsDuringDecode::ReadCommonInputFromConfigFileAfterClearOperations()
    {
    // read choices, what operations to be performed
    if ((!GetBoolFromConfig(iSectionName, _L("cropchoice2"), iCropChoice)) ||  // true or false
        (!GetBoolFromConfig(iSectionName, _L("scalechoice2"), iScaleChoice)) || 
        (!GetBoolFromConfig(iSectionName, _L("mirrorchoice2"), iMirrorChoice)) || 
        (!GetBoolFromConfig(iSectionName, _L("rotationchoice2"), iRotationChoice)))
        {
        return KErrNotFound;
        }

    if (iCropChoice) // true (1), false (0)
        {
        if ((!GetIntFromConfig(iSectionName, _L("cropax2"), iUM->iCropPointA.iX)) || 
            (!GetIntFromConfig(iSectionName, _L("cropay2"), iUM->iCropPointA.iY)) || 
            (!GetIntFromConfig(iSectionName, _L("cropwidth2"), iUM->iCropSize.iWidth)) || 
            (!GetIntFromConfig(iSectionName, _L("cropheight2"), iUM->iCropSize.iHeight)))
            {
            return KErrNotFound;
            }
        }

    if (iScaleChoice)
        {
        // full size = -1, ½ size = -2, ¼ size = -3, 1/8 size = -4
        if ((!GetIntFromConfig(iSectionName, _L("scalingqualityindex2"), iUM->iScalingQualityIndex)) || 
            (!GetIntFromConfig(iSectionName, _L("scalingcoeff2"), iUM->iScalingCoeff)))
            {
            return KErrNotFound;
            }
        }

    // rotation and/or mirror
    if ((iMirrorChoice) || (iRotationChoice))
        {
        if (!GetIntFromConfig(iSectionName, _L("operationindex2"), iUM->iOperationIndex))
            {
            return KErrNotFound;
            }
        }
    return KErrNone;
    }


// Add multiple extensions
void RStepBaseMultipleExtensionsDuringDecode::AddMultipleExtensionsL(TSize& aDestSize)
    {
    // iChoiceArray contains iCropChoice, iScaleChoice, iMirrorChoice, iRotationChoice in the order
    // all extensions, true (1), false (0)
    if ((iCropChoice) && (iScaleChoice) && ((iMirrorChoice) || (iRotationChoice)))
        {
        SetExtensionL(CUtilityMembers::EExtensionCropScaleOperation, aDestSize); return;
        }

    // scale and rotation/mirror
    if ((iScaleChoice) && ((iMirrorChoice) || (iRotationChoice)))
        {
        SetExtensionL(CUtilityMembers::EExtensionScaleOperation, aDestSize); return;
        }

    // crop and rotation/mirror
    if ((iCropChoice) && ((iMirrorChoice) || (iRotationChoice)))
        {
        SetExtensionL(CUtilityMembers::EExtensionCropOperation, aDestSize); return;
        }

    // crop and scale
    if ((iCropChoice) && (iScaleChoice))
        {
        SetExtensionL(CUtilityMembers::EExtensionCropScale, aDestSize); return;
        }

    // rotation/mirror
    if ((iMirrorChoice) || (iRotationChoice))
        {
        SetExtensionL(CUtilityMembers::EExtensionOperation, aDestSize); return;
        }

    // scale
    if (iScaleChoice)
        {
        SetExtensionL(CUtilityMembers::EExtensionScale, aDestSize); return;
        }

    // crop
    if (iCropChoice)
        {
        SetExtensionL(CUtilityMembers::EExtensionCrop, aDestSize); return;
        }
    }


// Clear all operations
void RStepBaseMultipleExtensionsDuringDecode::ClearAllOperationsL(TSize& aDestSize)
    {
    // just clear all the operations
    // test code handles clearance of operations for only one time 

    // to avoid any issues with the way the data is read from the configuration file, 
    // after clearing the operations, same values are repeated or new values are read

    // the choices are read again after clearing the operations
    // example, iCropChoice = true first time and iCropChoice = false second time indicates no crop to perform

    // if an operation is still expected to be performed with same set of values or with a new set of values then
    // the old values are repeated or new values are specified
    // example, iCropChoice = true first time and iCropChoice = true second time indicates crop is with old or new set of values

    // this kind of approach makes clearing the operations and reading the data values simpler 
    // else more flags and checking is required to know what operation is expected to be cleared

    TRAPD(err, iUM->iImageDecoder->SetClippingRectL(NULL));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("SetClippingRectL() left"));

    TImageConvScaler* scaler = NULL;
    TImageConvScaler::TScalerQuality scalingQuality;
    TRAP(err, scaler = iUM->iImageDecoder->ScalerL());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("ScalerL() left"));
    GetScalingQualityL(scalingQuality);
    TRAP(err, scaler->SetScalingL(-1, scalingQuality));
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("SetScalingL() left"));

    TImageConvOperation* operation = NULL;
    TRAP(err, operation = iUM->iImageDecoder->OperationL());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("OperationL() left"));
    operation->ClearOperationStack();

	err = iUM->iImageDecoder->GetDestinationSize(aDestSize, KFrameZero);
	CHECK_STATUS_LEAVE_IF_ERROR(err, _L("GetDestinationSize() failed"));
    }


// Check the output image
//      - save the output bitmap to a file
//      - compare output bitmap with reference image
void RStepBaseMultipleExtensionsDuringDecode::CheckMultipleExtOutputAndSaveL()
    {
    User::LeaveIfError(SaveDecodedImage()); // save the output image

    if ((iTestStepName.Compare(_L("MM-ICL-DEC-MUOP-I-0100-HP")) != 0) && 
        (iTestStepName.Compare(_L("MM-ICL-DEC-MUOP-I-0200-HP")) != 0))
        {
        BitmapComparisonL(*iUM->iDestBitmap, iUM->iReferenceImage);
        }
    }


// override from RStepBaseImageDecoder
void RStepBaseMultipleExtensionsDuringDecode::DoKickoffTestL()
    {
    // read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());
    User::LeaveIfError(ReadCommonInputRefAndOutputImagesFromConfigFile());

    iUM->iDeOptions = static_cast<CImageDecoder::TOptions> 
                                 (CImageDecoder::EOptionExtCrop | 
                                  CImageDecoder::EOptionExtScaling | 
                                  CImageDecoder::EOptionExtRotation | 
                                  CImageDecoder::EOptionExtMirrorHorizontalAxis | 
                                  CImageDecoder::EOptionExtMirrorVerticalAxis | 
                                  CImageDecoder::EPreferFastDecode);

    SetThreadForDecoding(iUM->iDeOptions); // set the option if decoding is performed in a separate thread
    User::LeaveIfError(CreateImageDecoder());

    LogTestCaseDescription();

    iUM->iActiveListener = new (ELeave) CActiveListener(this); // create an active listener

    TRAPD(err, SetImageTypeL());
    CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Could not set image type"));

    LogFrameDisplayMode();

    INFO_PRINTF1(_L("Order of operations:"));
    if (iCropChoice) // true (1), false (0)
        {
        INFO_PRINTF1(_L("- Crop"));
        }
    if (iScaleChoice)
        {
        INFO_PRINTF1(_L("- Scale"));
        }
    if ((iMirrorChoice) && (iRotationChoice))
        {
        INFO_PRINTF1(_L("- Mirror and Rotation (in the specified order in config file)"));
        }
    else if (iMirrorChoice)
        {
        INFO_PRINTF1(_L("- Mirror"));
        }
    else if (iRotationChoice)
        {
        INFO_PRINTF1(_L("- Rotation"));
        }

    TRAP(err, FsmL());
    if (err != KErrNone)
        {
        StopTest(err, EFail);
        }
    }


RStepMultipleExtDuringDecode* RStepMultipleExtDuringDecode::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepMultipleExtDuringDecode(aStepName, aTestCaseDescription));
    }

RStepMultipleExtDuringDecode::RStepMultipleExtDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseMultipleExtensionsDuringDecode(aStepName, aTestCaseDescription)
    {
    }

void RStepMultipleExtDuringDecode::FsmL()
    {
    TInt err = KErrNone;
    switch (iUM->iState)
        {
        case 0:
            AddMultipleExtensionsL(iUM->iDestSize);

            if (iClearOperationsTest)
                {
                ClearAllOperationsL(iUM->iDestSize);
                INFO_PRINTF1(_L("Cancelled all operations... reading new or same values"));
                User::LeaveIfError(ReadCommonInputFromConfigFileAfterClearOperations());
                AddMultipleExtensionsL(iUM->iDestSize);
                }

            DecodeToBitmapDuringDecodeL(iUM->iDestSize);
            StateChange();
            break;

        case 1:
            err = iUM->iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Multiple operations during decode unsuccessful"));
            INFO_PRINTF1(_L("Multiple operations during decode successful!"));
            CheckMultipleExtOutputAndSaveL();
            StopTest(EPass);
            break;

        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iUM->iState);
            User::Leave(KErrGeneral);
        }
    }
