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
// This file contains test step class declaration for scaling during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_SCALE_H
#define TSI_ICL_DECODE_SCALE_H


// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for scaling during decoding tests
class RStepBaseScaleDuringDecode : public RStepBaseImageDecoder
    {
protected:
    RStepBaseScaleDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    // read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // check the output image - compare size, compare output bitmap and save the output to a file
    void CheckScaleOutputAndSaveL();

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();
    };


// MM-ICL-DEC-SCAL-I-0110-HP, MM-ICL-DEC-SCAL-I-0120-HP, MM-ICL-DEC-SCAL-I-0130-HP
// MM-ICL-DEC-SCAL-I-0140-HP, MM-ICL-DEC-SCAL-I-0150-HP, MM-ICL-DEC-SCAL-I-0160-HP
// MM-ICL-DEC-SCAL-I-0170-HP, MM-ICL-DEC-SCAL-I-0180-HP, MM-ICL-DEC-SCAL-I-0190-HP
// MM-ICL-DEC-SCAL-I-0195-HP, MM-ICL-DEC-SCAL-I-0200-HP, MM-ICL-DEC-SCAL-I-0205-HP
// MM-ICL-DEC-SCAL-I-0210-HP, MM-ICL-DEC-SCAL-I-0220-HP, MM-ICL-DEC-SCAL-I-0230-HP
// MM-ICL-DEC-SCAL-I-0240-HP, MM-ICL-DEC-SCAL-I-0710-HP, MM-ICL-DEC-SCAL-I-0720-HP
// MM-ICL-DEC-SCAL-I-0730-HP
// MM-ICL-DEC-ARSCAL-I-0080-HP, MM-ICL-DEC-ARSCAL-I-0090-HP
// Downscale during decode of an image to full size; decode with display mode
// Downscale during decode of an image to size ½; decode with display mode
// Downscale during decode of an image to size ¼; decode with display mode
// Downscale during decode of an image to size 1/8; decode with display mode
// Downscale during decode of a thumbnail to full size; decode with display mode
// Downscale during decode of a thumbnail thumbnail to size ½; decode with display mode
// Downscale during decode of a thumbnail thumbnail to size ¼; decode with display mode
// Downscale during decode of a thumbnail thumbnail to size 1/8; decode with default display mode
// Downscale during decode of an image to full size; decode with display mode
// Downscale during decode of an image to desired size; decode with display mode
// Downscale during decode of a thumbnail thumbnail to size ½; decode with display mode
// Downscale during decode of a thumbnail thumbnail to desired size; decode with display mode
// Downscale during decode of an image to desired size; decode with display mode
// Downscale during decode of a thumbnail thumbnail to desired size; decode with display mode
// Downscale during decode of an image to full size; decode with display mode
// Downscale during decode of a thumbnail thumbnail to full size; decode with display mode
// Downscale during decode of a progressive image to size ½
// Downscale during decode of image with DRI (Define Restart Interoperability) marker to size ½
// Downscale during decode of an image to full size; decode with display mode
// Autorotate and Downscale during decode of a thumbnail to full size; decode with display mode
// Autorotate and Downscale during decode of an image to full size; decode with display mode
class RStepScaleDuringDecode : public RStepBaseScaleDuringDecode
    {
public:
    static RStepScaleDuringDecode* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepScaleDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


#endif // TSI_ICL_DECODE_SCALE_H
