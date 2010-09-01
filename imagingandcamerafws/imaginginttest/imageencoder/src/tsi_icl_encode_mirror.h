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
// This file contains test step class declaration for mirror during encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


#ifndef TSI_ICL_ENCODE_MIRROR_H
#define TSI_ICL_ENCODE_MIRROR_H

// Test user includes
#include "tsi_icl_encode_stepbase.h"

// Test Step base class for mirror during encode tests
class RStepMirrorDuringEncodeBase : public RStepBaseImageEncoder
    {
protected:
    RStepMirrorDuringEncodeBase(const TDesC& aStepName, const TDesC& aTestCaseDescription) : RStepBaseImageEncoder(aStepName, aTestCaseDescription) { };

    // Read input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // Mirror during Encoding of a bitmap to JPEG format
    void MirrorABitmapDuringEncodeL();

    // Mirror during Encoding of a JPEG image from YUV pixel buffer
    void EncodeImageFrameWithYUVDataL();
    void DecodeJpegImageToYUVImageFrameL();
    void NegEncodeImageFrameWithYUVDataL();

    // override from RStepBaseImageEncoder
    void DoKickoffTestL();
    };

/*
*	Mirror during coding test cases
*	MM-ICL-ENC-RTMR-I-0120-HP,MM-ICL-ENC-RTMR-I-0140-HP, 
*	MM-ICL-ENC-RTMR-I-0190-HP to MM-ICL-ENC-RTMR-I-0350-HP
*/
class RStepMirrorDuringEncodeToImage : public RStepMirrorDuringEncodeBase
	{
public:
    static RStepMirrorDuringEncodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();
private:
	RStepMirrorDuringEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);	
	};

/*
*	Rotation and mirror during encoding test cases 
*	MM-ICL-ENC-RTMR-I-0540-HP to MM-ICL-ENC-RTMR-I-0650-HP
*/
class RStepRotateMirrorDuringEncodeToImage : public RStepMirrorDuringEncodeBase
	{
public:
    static RStepRotateMirrorDuringEncodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();

private:
	RStepRotateMirrorDuringEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);	
	};

/*
*	Rotation and mirror during encoding test cases with clearoperationstack
*	MM-ICL-ENC-RTMR-I-0655-HP to MM-ICL-ENC-RTMR-I-0657-HP
*/
class RStepRotateMirrorClearOpStackDuringEncodeToImage : public RStepMirrorDuringEncodeBase
	{
public:
    static RStepRotateMirrorClearOpStackDuringEncodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadInputFromConfigFile();
    void FsmL();

private:
	RStepRotateMirrorClearOpStackDuringEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);	
	};
	
/*
*	Mirror during encoding performance tests
*	MM-ICL-ENC-RTMR-I-0360-HP to MM-ICL-ENC-RTMR-I-0490-HP
*/
class RStepMirrorDuringEncodeToImagePerf : public RStepMirrorDuringEncodeBase
	{
public:
    static RStepMirrorDuringEncodeToImagePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();

    TInt ReadInputFromConfigFile();

private:
	RStepMirrorDuringEncodeToImagePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription); 
	};

/*
*	Mirror during encoding test cases with YUV pixel buffer as source
*	Decode image to YUV buffer image frame then mirror during encode image frame 
*	MM-ICL-ENC-RTMR-I-0130-HP TO MM-ICL-ENC-RTMR-I-0160-HP
*/
class RStepMirrorDuringEncodeYUVPixelBuffer : public RStepMirrorDuringEncodeBase
	{
public:
    static RStepMirrorDuringEncodeYUVPixelBuffer* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();

private:
	RStepMirrorDuringEncodeYUVPixelBuffer(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	
protected:
	TBool    iPositiveTest;              //ETrue for positive tests EFalse for neg tests	
	};


/*
*	Real Time events test
*	MM-ICL-ENC-RTMR-I-0500-HP TO MM-ICL-ENC-RTMR-I-0510-HP
*/
class RStepMirrorDuringEncodeToImageRealTimeTests : public RStepMirrorDuringEncodeBase, public MTimerObserver
	{
public:
    static RStepMirrorDuringEncodeToImageRealTimeTests* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();
    TInt ReadInputFromConfigFile();
    // from MTimerObserver
	void TimerExpired();

private:
	RStepMirrorDuringEncodeToImageRealTimeTests(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};
	
#endif // TSI_ICL_ENCODE_MIRROR_H
