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
// This file contains test step class declaration for multiple operations 
// (crop, scale and rotate/mirror in this order) during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_MULTIPLEOP_H
#define TSI_ICL_DECODE_MULTIPLEOP_H


// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for multiple operations during decoding tests
class RStepBaseMultipleExtensionsDuringDecode : public RStepBaseImageDecoder
    {
protected:
    RStepBaseMultipleExtensionsDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    // read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();
    TInt ReadCommonInputFromConfigFileAfterClearOperations();

    // add multiple extensions
    void AddMultipleExtensionsL(TSize& aDestSize);

    // clear all operations
    void ClearAllOperationsL(TSize& aDestSize);

    // check the output image
    void CheckMultipleExtOutputAndSaveL();

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();

protected:
    // desired set of operations to be performed
    // any or all operations can be performed in the order - crop, scale and rotate/mirror
    TBool iCropChoice;
    TBool iScaleChoice;
    TBool iMirrorChoice;
    TBool iRotationChoice;

    // indicates tests which clear all operations and continue with old or new values for each operation
    TBool iClearOperationsTest;
    };


// MM-ICL-DEC-MUOP-I-0010-HP to 0200-HP
// MM-ICL-DEC-MUOP-I-0210-HP to 0260-HP
// Multiple operations during decode of image or thumbnail; decode with display mode
// Multiple operations during decode of image, clear operations; decode with display mode
class RStepMultipleExtDuringDecode : public RStepBaseMultipleExtensionsDuringDecode
    {
public:
    static RStepMultipleExtDuringDecode* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepMultipleExtDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


#endif // TSI_ICL_DECODE_MULTIPLEOP_H
