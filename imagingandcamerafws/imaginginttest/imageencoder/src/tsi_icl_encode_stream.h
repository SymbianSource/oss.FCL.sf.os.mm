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
// This file contains test step class declaration for streamed encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


#ifndef TSI_ICL_ENCODE_STREAM_H
#define TSI_ICL_ENCODE_STREAM_H

// Test user includes
#include "tsi_icl_encode_stepbase.h"


/*
iStreammode = 1 ->sequestialforward streaming
iStreammode = 2 ->randomforward streaming
iStreammode = 3 ->randombackward streaming
*/

// Test Step base class for streamed encoding tests
class RStepStreamedEncodeBase : public RStepBaseImageEncoder
    {
protected:
    RStepStreamedEncodeBase(const TDesC& aStepName, const TDesC& aTestCaseDescription) : RStepBaseImageEncoder(aStepName, aTestCaseDescription) { };

    // read input from the configuration file
    TInt ReadCommonInputFromConfigFile();
    // streamed encoding of a bitmap to JPEG format
    void StreamedEncodeNegTestL(); 
    //Decoding to imageframe buffer   
    void DecodeJpegImageToImageFrameL();
    //stream decoding initialization
    void InitializeStreamDecodingL();
    //stream decoding to imageframe buffer blockwise based on streammode
    void StreamDecodeJpegImageToImageFrameL();
    //Stream encoding initilization
	void InitializeStreamedEncodingL();
	// stream encode (appending)imageframe to jpeg image 
	void StreamedEncoding();
	// stream encoding complete
	void StreamedEncodingComplete();
	// Decoding jpeg image to imageframe
	void DecodeFiles();
	//stream decoding jpeg image to imageframe
	void StreamDecodeFiles();
    // override from RStepBaseImageEncoder
    void DoKickoffTestL();
    
protected:
		TInt 	iCount;             //To hold the count of append blocks call in stream encode
		TInt	iEncodedImageCount; //For functional test cases there will be more than 1 inputimage each having MCU height
		TInt	iRemainblocks;       //to hold remainingblocks while stream encoding for functional test cases, 
		TInt    iBlockPosition;     //to hold blockposition while streamed random decoding from where the stream decoding starts
		
    };

/*
*   Base class for stream encode jpeg image. Functional test cases
*   These tests use normal decoding to generate the imageframe and then
*   stream encoded to generate the jpeg image.
*   MM-ICL-ENC-STRM-I-0080-HP TO MM-ICL-ENC-STRM-I-0110-HP
*   MM-ICL-ENC-STRM-I-0210-HP TO MM-ICL-ENC-STRM-I-0310-HP
*/
class RStepStreamedEncodeToImage : public RStepStreamedEncodeBase
	{
public:
    static RStepStreamedEncodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
	RStepStreamedEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};

/*
*   Base class for Stream encode Neg tests. Functional test cases
*   MM-ICL-ENC-STRM-I-0120-HP TO MM-ICL-ENC-STRM-I-0200-HP
*/
class RStepStreamedEncodeToImageNegTest : public RStepStreamedEncodeBase
	{
public:
    static RStepStreamedEncodeToImageNegTest* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
	RStepStreamedEncodeToImageNegTest(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};

/*
*	Base class for Real Time events test. Nonfunctional test cases
*   These tests use stream decode to generate the imageframe and 
*   then stream encode to jpeg image.
*	MM-ICL-ENC-STRM-I-0430-HP TO MM-ICL-ENC-STRM-I-0450-HP
*/
class RStepStreamedEncodeToImageRealTimeTests : public RStepStreamedEncodeBase, public MTimerObserver
	{
public:
    static RStepStreamedEncodeToImageRealTimeTests* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();
    TInt ReadInputFromConfigFile();

    // from MTimerObserver
	void TimerExpired();

private:
	RStepStreamedEncodeToImageRealTimeTests(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};
#endif // TSI_ICL_ENCODE_STREAM_H
