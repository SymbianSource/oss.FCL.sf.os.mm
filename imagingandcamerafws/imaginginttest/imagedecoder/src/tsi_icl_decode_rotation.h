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
// This file contains test step class declaration for rotation and/or mirroring during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_ROTATION_H
#define TSI_ICL_DECODE_ROTATION_H


// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for rotation and/or mirroring during decode
class RStepBaseRotateAndMirrorDuringDecode : public RStepBaseImageDecoder
    {
protected:
    RStepBaseRotateAndMirrorDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    // read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();
    };


// MM-ICL-DEC-RTMR-I-0030-HP, MM-ICL-DEC-RTMR-I-0050-HP, MM-ICL-DEC-RTMR-I-0070-HP
// MM-ICL-DEC-RTMR-I-0080-HP, MM-ICL-DEC-RTMR-I-0090-HP, MM-ICL-DEC-RTMR-I-0100-HP
// MM-ICL-DEC-RTMR-I-0110-HP, MM-ICL-DEC-RTMR-I-0120-HP, MM-ICL-DEC-RTMR-I-0130-HP
// MM-ICL-DEC-RTMR-I-0140-HP, MM-ICL-DEC-RTMR-I-0150-HP, MM-ICL-DEC-RTMR-I-0160-HP
// MM-ICL-DEC-RTMR-I-0162-HP, MM-ICL-DEC-RTMR-I-0163-HP, MM-ICL-DEC-RTMR-I-0170-HP
// MM-ICL-DEC-RTMR-I-0180-HP, MM-ICL-DEC-RTMR-I-0190-HP, MM-ICL-DEC-RTMR-I-0200-HP
// MM-ICL-DEC-RTMR-I-0210-HP, MM-ICL-DEC-RTMR-I-0220-HP, MM-ICL-DEC-RTMR-I-0230-HP
// MM-ICL-DEC-RTMR-I-0240-HP, MM-ICL-DEC-RTMR-I-0250-HP, MM-ICL-DEC-RTMR-I-0260-HP
// MM-ICL-DEC-RTMR-I-0270-HP, MM-ICL-DEC-RTMR-I-0320-HP, MM-ICL-DEC-RTMR-I-0330-HP
// MM-ICL-DEC-RTMR-I-0700-HP, MM-ICL-DEC-RTMR-I-0710-HP, MM-ICL-DEC-RTMR-I-0720-HP
// MM-ICL-DEC-RTMR-I-0730-HP, MM-ICL-DEC-RTMR-I-0740-HP
// MM-ICL-DEC-ARTMR-I-0010-HP, MM-ICL-DEC-ARTMR-I-0020-HP, MM-ICL-DEC-ARTMR-I-0030-HP
// MM-ICL-DEC-ARTMR-I-0040-HP, MM-ICL-DEC-ARTMR-I-0100-HP, MM-ICL-DEC-ARTMR-I-0110-HP
// Rotate and mirror during decode of image, output as a bitmap (CFbsBitmap)
// Rotate and mirror during decode of image, output as a bitmap (CFbsBitmap), run in a separate thread
// Rotate image 90 degrees clockwise during decode of image; with display mode
// Rotate image 180 degrees clockwise during decode of image; with display mode
// Rotate image 270 degrees clockwise during decode of image; with display mode
// Mirror image over the horizontal axis during decode of image; with display mode
// Mirror image over the vertical axis during decode of image; with display mode
// Mirror image over the horizontal and vertical axis during decode of image; with display mode
// Rotate image 90 degrees clockwise and mirror over the horizontal axis during decode of image; with display mode
// Rotate image 180 degrees clockwise and mirror over the horizontal axis during decode of image; with default display mode
// Rotate image 270 degrees clockwise and mirror over the horizontal axis during decode of image; with display mode
// Rotate image 90 degrees clockwise and mirror over the vertical axis during decode of image; with display mode
// Rotate image 90 degrees clockwise and mirror over the vertical axis during decode of image; with display mode
// Rotate image 90 degrees clockwise and mirror over the vertical axis during decode of image; with display mode
// Rotate image 180 degrees clockwise and mirror over the vertical axis during decode of image
// Rotate image 270 degrees clockwise and mirror over the vertical axis during decode of image
// Mirror over the horizontal axis and rotate image 90 degrees clockwise during decode of image
// Mirror over the horizontal axis and rotate image 180 degrees clockwise during decode of image
// Mirror over the horizontal axis and rotate image 270 degrees clockwise during decode of image
// Mirror over the vertical axis and rotate image 90 degrees clockwise during decode of image
// Mirror over the vertical axis and rotate image 180 degrees clockwise during decode of image
// Mirror over the vertical axis and rotate image 270 degrees clockwise during decode of image
// Mirror over the horizontal and vertical axis and rotate image 90 degrees clockwise during decode of image
// Mirror over the horizontal and vertical axis and rotate image 180 degrees clockwise during decode of image
// Mirror over the horizontal and vertical axis and rotate image 270 degrees clockwise during decode of image
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis of the image during decode of thumbnail image
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis of the image during decode of thumbnail image which is different to main image
// Rotate and mirror during decode of multiscan image
// Rotate and mirror during decode of progressive image
// Rotate and mirror during decode of image with DRI (Define Restart Interoperability) marker
// Checking the Prepare API before doing the convert
// Passing Decoderuid to filenewl and call covert to apply the ImageConversion operations
// Autorotate main image during decode
// Autorotate thumbnail image during decode
// Autorotate then mirror horizontal & vertical during decode
// Autorotate then mirror horizontal, vertical and rotate 90 degrees during decode
// Autorotate and mirror horizontal, verticala nd rotate 90 degrees input image without exif tags during decode
// Autorotate input image with exif tags during decode with invalid orientation
class RStepRotateOrMirrorDuringDecodeRotateMirror : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodeRotateMirror* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// DataNewL coverage
// MM-ICL-DEC-RTMR-I-0750-HP, MM-ICL-DEC-RTMR-I-0760-HP
// Rotate and mirror during decode of image with buffer
// Rotate and mirror during decode of image with buffer with decoderuid
class RStepRotateOrMirrorBufferDuringDecodeRotateMirror : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorBufferDuringDecodeRotateMirror* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorBufferDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };

// FileNewL intent coverage
// MM-ICL-DEC-RTMR-I-0770-HP, MM-ICL-DEC-RTMR-I-0780-HP
// Rotate and mirror during decode of image with intent with decoderuid
// Rotate and mirror during decode of image with intent with decoderuid
class RStepRotateOrMirrorIntentDuringDecodeRotateMirror : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorIntentDuringDecodeRotateMirror* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorIntentDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };

//FileNewL DRM source coverage
// MM-ICL-DEC-RTMR-I-0790-HP
// Rotate and mirror during decode of image with DRM source with decoderuid
class RStepRotateOrMirrorDRMDuringDecodeRotateMirror : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDRMDuringDecodeRotateMirror* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDRMDuringDecodeRotateMirror(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };
    
// MM-ICL-DEC-RTMR-I-0040-HP, MM-ICL-DEC-RTMR-I-0060-HP, MM-ICL-DEC-RTMR-I-0045-HP
// MM-ICL-DEC-ARTMR-I-0050-HP
// Rotate and mirror during decode of image, output as a YUV pixel buffer
// Rotate and mirror during decode of image, output as a YUV pixel buffer, run in a separate thread
// Decode the input image without applying operations and  output as a YUV pixel buffer
// Autorotate then rotate 90 degrees and mirror vertical during decode and output as YUV pixel buffer
class RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-RTMR-I-0280-HP, MM-ICL-DEC-RTMR-I-0290-HP, MM-ICL-DEC-RTMR-I-0300-HP
// MM-ICL-DEC-RTMR-I-0310-HP, MM-ICL-DEC-RTMR-I-0680-HP, MM-ICL-DEC-RTMR-I-0690-HP
// Mirror over the horizontal and vertical axis and rotate image 90 degrees clockwise during decode of image with colour sample Monochrome
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis image during decode of image with colour sample 4:2:0 (default)
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis image during decode of image with colour sample 4:2:2
// Mirror over the horizontal and vertical axis and rotate image 90 degrees clockwise during decode of image with colour sample 4:4:4
// Rotate and mirror during decode of image, confirm output contains no loss of image quality
// Mirror and rotate during decode of image, confirm output contains no loss of image quality
class RStepRotateOrMirrorDuringDecodeColourSample : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodeColourSample* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodeColourSample(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-RTMR-I-0350-HP, MM-ICL-DEC-RTMR-I-0360-HP, MM-ICL-DEC-RTMR-I-0370-HP, 
// MM-ICL-DEC-RTMR-I-0380-HP, MM-ICL-DEC-RTMR-I-0390-HP
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to size of valid bitmap
// Mirror over the horizontal and vertical axis and rotate 90 degrees clockwise during decode of image, downscale to ½ size
// Rotate 180 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to ¼ size
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to 1/8 size
// Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to size smaller than 1/8
class RStepRotateOrMirrorDuringDecodeScale : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodeScale* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodeScale(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-RTMR-I-0400-HP, MM-ICL-DEC-RTMR-I-0410-HP, MM-ICL-DEC-RTMR-I-0420-HP
// MM-ICL-DEC-RTMR-I-0430-HP, MM-ICL-DEC-RTMR-I-0440-HP, MM-ICL-DEC-RTMR-I-0450-HP
// MM-ICL-DEC-RTMR-I-0460-HP, MM-ICL-DEC-RTMR-I-0470-HP, MM-ICL-DEC-RTMR-I-0480-HP
// MM-ICL-DEC-RTMR-I-0490-HP, MM-ICL-DEC-RTMR-I-0500-HP, MM-ICL-DEC-RTMR-I-0510-HP
// MM-ICL-DEC-RTMR-I-0520-HP, MM-ICL-DEC-RTMR-I-0530-HP, MM-ICL-DEC-RTMR-I-0540-HP
// MM-ICL-DEC-RTMR-I-0550-HP
// Record time taken to rotate 90 degrees clockwise a small image during decode
// Record time taken to rotate 180 degrees clockwise a small image during decode
// Record time taken to rotate 270 degrees clockwise a small image during decode
// Record time taken to rotate 90 degrees clockwise an average image during decode
// Record time taken to rotate 180 degrees clockwise an average image during decode
// Record time taken to rotate 270 degrees clockwise an average image during decode
// Record time taken to mirror over the horizontal axis a small image during decode
// Record time taken to mirror over the horizontal axis an average image during decode
// Record time taken to mirror over the vertical axis a small image during decode
// Record time taken to mirror over the vertical axis an average image during decode
// Record time taken to rotate 90 degress clockwise and mirror over the horizontal  axis during decode of a small image
// Record time taken to rotate 180 degress clockwise and mirror over the vertical axis during decode of a small image
// Record time taken to rotate 270 degress clockwise and mirror over the vertical and horizontal axis during decode of a small image
// Record time taken to mirror over the vertical axis and rotate 90 degress clockwise during decode of a small image
// Record time taken to mirror over the horizontal axis and rotate 180 degress clockwise during decode of a small image
// Record time taken to mirror over the vertical and horizontal axis and rotate 270 degress clockwise during decode of a small image
class RStepRotateOrMirrorDuringDecodePerf : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-RTMR-I-0590-HP, MM-ICL-DEC-RTMR-I-0600-HP
// Mirror and rotate an image during decode, image contains data which is minor corrupted so that a distorted image can be produced
// Mirror and rotate an image during decode, image contains data which is corrupted and causes underflow error
class RStepRotateOrMirrorDuringDecodeInterop : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodeInterop* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodeInterop(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-RTMR-I-0620-HP, MM-ICL-DEC-RTMR-I-0630-HP, MM-ICL-DEC-RTMR-I-0640-HP
// MM-ICL-DEC-RTMR-I-0650-HP
// Cancel rotate and mirror during decode of image
// Cancel mirror and rotate during decode of image, then start again
// Cancel rotate and mirror during decode of image, which is run in a separate thread
// Cancel mirror and rotate during decode of image, which is run in a separate thread, then start again
class RStepRotateOrMirrorDuringDecodeCancel : public RStepBaseRotateAndMirrorDuringDecode, public MTimerObserver
    {
public:
    static RStepRotateOrMirrorDuringDecodeCancel* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

    // from MTimerObserver
	void TimerExpired();

private:
    RStepRotateOrMirrorDuringDecodeCancel(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-RTMR-I-0660-HP, MM-ICL-DEC-RTMR-I-0670-HP
// Rotate thumbnail and image 90 degrees clockwise during decode of image
// Mirror over the vertical axis and rotate image and thumbnail 180 degrees clockwise during decode of image
class RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage : public RStepBaseRotateAndMirrorDuringDecode
    {
public:
    static RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();

private:
    TFileName iReferenceImage2;    // compare decoded bitmap with the reference file
    TFileName iOutputImage2;       // save decoded bitmap to this file
    };


#endif // TSI_ICL_DECODE_ROTATION_H
