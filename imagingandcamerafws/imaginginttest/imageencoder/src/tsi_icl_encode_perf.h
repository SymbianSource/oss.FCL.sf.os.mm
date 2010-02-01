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
// This file contains test step class declaration for baseline performance integration tests for image encoding.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


#ifndef TSI_ICL_ENCODE_PERF_H
#define TSI_ICL_ENCODE_PERF_H

// Test user includes
#include "tsi_icl_encode_stepbase.h"


// Test Step base class for baseline performance tests for encoder
class RStepBaseImageEncoderPerf : public RStepBaseImageEncoder
    {
protected:
	RStepBaseImageEncoderPerf(const TDesC& aStepName, const TDesC& aTestCaseDescription): RStepBaseImageEncoder(aStepName, aTestCaseDescription) { };

    // Read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // Encode without EXIF data
    void EncodeImageFromBitmapToFileL();

    // Encode with EXIF data and thumbnail or only EXIF data
    void EncodeImageFromBitmapToFileWithExifDataL();

    //Rotate and/or Mirror a Bitmap
    void RotateABitmapL();
    
    // override from RStepBaseImageEncoder
    void DoKickoffTestL();
    
    };


/*
*	Encode the bitmap with EXIF data and thumbnail or only EXIF data, 
*	output as JPEG format
*	MM-ICL-ENC-PERF-I-0010-HP to MM-ICL-ENC-PERF-I-0095-HP
*/
class RStepEncodeImageWithExifdata : public RStepBaseImageEncoderPerf
	{
public:
    static RStepEncodeImageWithExifdata* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();

private:
	RStepEncodeImageWithExifdata(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};


/*
*	 Mirror then encode a CFbsBitmap, output as a JPEG format
*	MM-ICL-ENC-PERF-I-0600-HP to MM-ICL-ENC-PERF-I-0694-HP
*/
class RStepMirrorThenEncodeToImage : public RStepBaseImageEncoderPerf
    {
public:
    static RStepMirrorThenEncodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();

    // read input from the configuration file
    TInt ReadInputFromConfigFile();

private:
    RStepMirrorThenEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };

/*
*	 Rotate and /or mirror  then encode a CFbsBitmap, output as a JPEG format
*	MM-ICL-ENC-PERF-I-0800-HP to MM-ICL-ENC-PERF-I-1030-HP
*/
class RStepRotateMirrorThenEncodeToImage : public RStepBaseImageEncoderPerf
    {
public:
    static RStepRotateMirrorThenEncodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    void FsmL();

    // read input from the configuration file
    TInt ReadInputFromConfigFile();

private:
    RStepRotateMirrorThenEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };
#endif
