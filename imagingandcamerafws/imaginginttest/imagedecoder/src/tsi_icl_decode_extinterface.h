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
// This file contains test step class declaration for confirming which extension interface(s) are supported.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_EXTINTERFACE_H
#define TSI_ICL_DECODE_EXTINTERFACE_H

// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for extension interface tests for decoder
class RStepBaseImageDecoderExtensionInterface : public RStepBaseImageDecoder
    {
protected:
    RStepBaseImageDecoderExtensionInterface(const TDesC& aStepName, const TDesC& aTestCaseDescription): RStepBaseImageDecoder(aStepName, aTestCaseDescription)
    {
    }

    // read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // get the ImplementationUidValue for extension interface(s)
    TUid DecoderImplementationUidValue();

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();

    virtual void DoTestL() = 0;
    };


// MM-ICL-DEC-CROP-I-0010-HP, MM-ICL-DEC-CROP-I-0015-HP, MM-ICL-DEC-CROP-I-0020-HP
// MM-ICL-DEC-CROP-I-0025-HP
// MM-ICL-DECSDC-I-0010-HP, MM-ICL-DEC-RTMR-I-0010-HP, MM-ICL-DEC-ARTMR-I-0150-HP
// Confirm that a codec, which supports extension interface during decoding, 
// correctly reports support (or no support) of the feature through 'plug-in properties'
// (codec's resource file)
// Confirm whether the FrameInfo returns frameinfo during decoding taking into account orientation tag set
class RStepImageDecodeExtInterfacePluginProp : public RStepBaseImageDecoderExtensionInterface
    {
public:
    static RStepImageDecodeExtInterfacePluginProp* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void DoTestL();

private:
    RStepImageDecodeExtInterfacePluginProp(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-CROP-I-0030-HP, MM-ICL-DEC-CROP-I-0035-HP, MM-ICL-DEC-CROP-I-0040-HP
// MM-ICL-DEC-CROP-I-0045-HP, MM-ICL-DEC-CROP-I-0046-HP
// MM-ICL-DECSDC-I-0020-HP, MM-ICL-DEC-RTMR-I-0012-HP, MM-ICL-DEC-RTMR-I-0014-HP,
// MM-ICL-DEC-RTMR-I-0016-HP, MM-ICL-DEC-RTMR-I-0020-HP
// MM-ICL-DEC-ARTMR-I-0120-HP, MM-ICL-DEC-ARTMR-I-0130-HP, MM-ICL-DEC-ARTMR-I-0140-HP
// Confirm that a codec, which supports extension interface during decoding, 
// correctly reports support (or no support) of the feature by loading the codec
// Autorotate and mirror horizontal input image during decode with JPEG codec which doesn't support autorotate
// Confirm whether the loaded codec supports autorotate during decoding using plug-in properties
// Confirm whether the available codec supports autorotate during decoding using inetrfaceimplementations
// 
class RStepImageDecodeExtInterfaceLoadCodec : public RStepBaseImageDecoderExtensionInterface
    {
public:
    static RStepImageDecodeExtInterfaceLoadCodec* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void DoTestL();

private:
    RStepImageDecodeExtInterfaceLoadCodec(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


#endif // TSI_ICL_DECODE_EXTINTERFACE_H
