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
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


#ifndef TSI_ICL_ENCODE_EXTINTERFACE_H
#define TSI_ICL_ENCODE_EXTINTERFACE_H

// Test user includes
#include "tsi_icl_encode_stepbase.h"


// Test Step base class for extension interface tests for encoder
class RStepBaseImageEncoderExtInterface : public RStepBaseImageEncoder
   {
protected:
   RStepBaseImageEncoderExtInterface(const TDesC& aStepName, const TDesC& aTestCaseDescription): RStepBaseImageEncoder(aStepName, aTestCaseDescription) { };
   // read input from the configuration file
   TInt ReadCommonInputFromConfigFile();
   //get the implementation uid
   TUid EncoderImplementationUidValue();
   // override from RStepBaseImageEncoder
   void DoKickoffTestL();
   virtual void DoTest() = 0;

protected:
	TUid	iImageEncoderImplementationUidValue;
	};

/*	
*	Confirm that a codec, which supports extension interface during encoding, 
*	correctly reports support(or no support) of the feature through 'plug-in properties' (codec's resource file)
* 	MM-ICL-ENC-RTMR-I-0010-HP TO MM-ICL-ENC-RTMR-I-0085-HP
*/ 
class RStepEncodeExtInterfaceSupportUsingPluginProp: public RStepBaseImageEncoderExtInterface
	{
public:
    static RStepEncodeExtInterfaceSupportUsingPluginProp* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void DoTest();
    
private:
    RStepEncodeExtInterfaceSupportUsingPluginProp(const TDesC& aStepName, const TDesC& aTestCaseDescription);
   
	};
	
/*
*	Confirm that a codec, which supports extension interface during encoding, 
*	correctly reports support of the feature by loading the codec
*	MM-ICL-ENC-RTMR-I-0090-HP TO MM-ICL-ENC-RTMR-I-0110-HP
*/ 
class RStepEncodeExtInterfaceSupportByLoadingCodec: public RStepBaseImageEncoderExtInterface
	{
public:
    static RStepEncodeExtInterfaceSupportByLoadingCodec* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void DoTest();
    
private:
    RStepEncodeExtInterfaceSupportByLoadingCodec(const TDesC& aStepName, const TDesC& aTestCaseDescription);
   
	};	
#endif  // TSI_ICL_ENCODE_EXTINTERFACE_H
