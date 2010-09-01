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
// This file contains test step class declaration for baseline performance integration tests for image decoding.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_PERF_H
#define TSI_ICL_DECODE_PERF_H


// EPOC includes
#include <utf.h>
#include <iclexif.h>

// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for baseline performance tests for image decoder
class RStepBaseImageDecoderPerformance : public RStepBaseImageDecoder
    {
protected:
    RStepBaseImageDecoderPerformance(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    // read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // read and log some EXIF tags but the data cannot be saved to the output image
    // reading EXIF data will not count towards time to be measured for decoding
    void ReadExifDataFromImageL();

    // decode the image to a bitmap
    void DecodeToBitmapL(const TSize aDestSize);

    // scale the bitmap that is decoded from the image
    // this method is not called when decoder downscaling capabilities are used
    void ScaleBitmapL(const TSize aScaleSize);

    // rotate the bitmap that is decoded from the image; can also mirror the bitmap
    void RotateBitmapL(const CBitmapRotator::TRotationAngle aAngle);

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();
    };


// MM-ICL-DEC-PERF-I-0100-HP, MM-ICL-DEC-PERF-I-0110-HP, MM-ICL-DEC-PERF-I-0120-HP
// Record time taken to decode thumbnail image
// Record time taken to decode small image
// Record time taken to decode average size image
class RStepImageDecodePerf : public RStepBaseImageDecoderPerformance
    {
public:
    static RStepImageDecodePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepImageDecodePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-PERF-I-0130-HP, MM-ICL-DEC-PERF-I-0140-HP, MM-ICL-DEC-PERF-I-0150-HP
// MM-ICL-DEC-PERF-I-0160-HP, MM-ICL-DEC-PERF-I-0170-HP, MM-ICL-DEC-PERF-I-0180-HP
// Record time taken to decode small image, and downscale to ½
// Record time taken to decode average size image, and downscale to ½
// Record time taken to decode small image, and downscale to ¼
// Record time taken to decode average size image, and downscale to ¼
// Record time taken to decode small image, and downscale to 1/8
// Record time taken to decode average size image, and downscale to 1/8
class RStepImageDecodeScalePerf : public RStepBaseImageDecoderPerformance
    {
public:
    static RStepImageDecodeScalePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepImageDecodeScalePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-PERF-I-0700-HP, MM-ICL-DEC-PERF-I-0710-HP
// Record time taken to decode small image to downscale to 1/8 and bitmap downscale by ½
// Record time taken to decode average size image to downscale to 1/8 and bitmap downscale by ½
class RStepImageDecodeDoubleScalePerf : public RStepBaseImageDecoderPerformance
    {
public:
    static RStepImageDecodeDoubleScalePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepImageDecodeDoubleScalePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();

private:
    TSize iScaleSize2;       // second size of the destination bitmap scaling
    TInt  iReductionFactor2; // second reduction factor for image scaling
    };


// MM-ICL-DEC-PERF-I-0200-HP, MM-ICL-DEC-PERF-I-0210-HP, MM-ICL-DEC-PERF-I-0220-HP
// MM-ICL-DEC-PERF-I-0300-HP, MM-ICL-DEC-PERF-I-0310-HP, MM-ICL-DEC-PERF-I-0320-HP
// MM-ICL-DEC-PERF-I-0330-HP, MM-ICL-DEC-PERF-I-0340-HP, MM-ICL-DEC-PERF-I-0350-HP
// Record time taken to decode thumbnail image and rotate 90 degrees clockwise
// Record time taken to decode small image and rotate 180 degrees clockwise
// Record time taken to decode average size image and rotate 270 degrees clockwise
// Record time taken to decode thumbnail image and mirror the bitmap over the vertical axis
// Record time taken to decode small image and mirror the bitmap over the vertical axis
// Record time taken to decode average size image and mirror the bitmap over the vertical axis
// Record time taken to decode thumbnail image and mirror the bitmap over the horizontal axis
// Record time taken to decode small image and mirror the bitmap over the horizontal axis
// Record time taken to decode average size image and mirror the bitmap over the horizontal axis
class RStepImageDecodeRotatePerf : public RStepBaseImageDecoderPerformance
    {
public:
    static RStepImageDecodeRotatePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepImageDecodeRotatePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-PERF-I-0400-HP, MM-ICL-DEC-PERF-I-0410-HP, MM-ICL-DEC-PERF-I-0420-HP
// MM-ICL-DEC-PERF-I-0500-HP, MM-ICL-DEC-PERF-I-0510-HP, MM-ICL-DEC-PERF-I-0520-HP
// Record time taken to decode thumbnail image then rotate and mirror the bitmap over the vertical axis
// Record time taken to decode small image then rotate and mirror the bitmap over the horizontal axis
// Record time taken to decode average size image then rotate and mirror the bitmap over the vertical axis
// Record time taken to decode thumbnail image then mirror over the vertical axis and rotate the bitmap
// Record time taken to decode small image then mirror over the horizontal axis and rotate the bitmap
// Record time taken to decode average size image then mirror over the vertical axis and rotate the bitmap
class RStepImageDecodeRotateMirrorPerf : public RStepBaseImageDecoderPerformance
    {
public:
    static RStepImageDecodeRotateMirrorPerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepImageDecodeRotateMirrorPerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();

private:
    // order of performing transformation operations
    // true => rotation first and mirroring next, false => mirroring first and rotation next
    TBool iOperationOrder;
    };


#endif // TSI_ICL_DECODE_PERF_H
