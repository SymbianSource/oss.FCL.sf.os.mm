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
// This file contains test step class declaration for crop during decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_CROP_H
#define TSI_ICL_DECODE_CROP_H


// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for crop during decoding tests
class RStepBaseCropDuringDecode : public RStepBaseImageDecoder
    {
protected:
    RStepBaseCropDuringDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    // read common input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // check the output image - compare size, compare output bitmap and save the output to a file
    void CheckCropOutputAndSaveL(const TSize aOutputSize);

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();
    };


// MM-ICL-DEC-CROP-I-0050-HP, MM-ICL-DEC-CROP-I-0070-HP, MM-ICL-DEC-CROP-I-0105-HP
// MM-ICL-DEC-CROP-I-0400-HP, MM-ICL-DEC-CROP-I-0401-HP, MM-ICL-DEC-CROP-I-0402-HP
// MM-ICL-DEC-CROP-I-0403-HP, MM-ICL-DEC-CROP-I-0404-HP, MM-ICL-DEC-CROP-I-0405-HP
// MM-ICL-DEC-CROP-I-0406-HP
// MM-ICL-DEC-CROP-I-0700-HP, MM-ICL-DEC-CROP-I-0710-HP, MM-ICL-DEC-CROP-I-0720-HP
// MM-ICL-DEC-CROP-I-0730-HP
// MM-ICL-DEC-ARCROP-I-0070-HP
// Crop during decode of image, output as a bitmap (CFbsBitmap)
// Crop during decode of image, output as a bitmap (CFbsBitmap), run in a separate thread
// Crop during decode of image, cancel the crop, output the original image as a bitmap (CFbsBitmap)
// Crop during decode of image, which is the smallest possible; tests from 0400-HP to 0406-HP
// Crop during decode of multiscan image
// Crop during decode of progressive image
// Crop during decode of image with DRI (Define Restart Interoperability) marker
// Crop during decode of image, output as a bitmap (CFbsBitmap)
// Autorotate and Crop during decode of image, cancel the crop, output the original image as a bitmap (CFbsBitmap)
class RStepCropDuringDecodeAsBitmap : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeAsBitmap* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeAsBitmap(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-CROP-I-0060-HP, MM-ICL-DEC-CROP-I-0080-HP, MM-ICL-DEC-CROP-I-0100-HP
// Crop during decode of image, output as a YUV pixel buffer
// Crop during decode of image, output as a YUV pixel buffer, run in a separate thread
// Crop during decode of image, attempt to output to unsupported YUV format
class RStepCropDuringDecodeAsYuvPixelBuffer : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeAsYuvPixelBuffer* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeAsYuvPixelBuffer(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-CROP-I-0090-HP, MM-ICL-DEC-CROP-I-0095-HP, MM-ICL-DEC-CROP-I-0098-HP
// MM-ICL-DEC-ARCROP-I-0060-HP
// Crop during decode of image, attempt to output to invalid sized bitmap (size of the output bitmap less than 1/8 of crop size)
// Crop during decode of image, attempt to output to bitmap larger than original image
// Crop during decode of image, attempt to output to invalid sized bitmap (size of the output bitmap greater than crop size)
// Autorotate and Crop during decode of image, attempt to output to invalid sized bitmap (size of the output bitmap greater than crop size
class RStepCropDuringDecodeInvalidDestSize : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeInvalidDestSize* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeInvalidDestSize(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-CROP-I-0110-HP, MM-ICL-DEC-CROP-I-0120-HP, MM-ICL-DEC-CROP-I-0130-HP
// MM-ICL-DEC-CROP-I-0135-HP, MM-ICL-DEC-CROP-I-0690-HP
// Crop during decode of image with colour sample Monochrome (block size 8 x 8)
// Crop during decode of image with colour sample 4:2:0 (block size 16 x 16)
// Crop during decode of image with colour sample 4:2:2 (block size 8 x 16)
// Crop during decode of image with colour sample 4:4:4 (block size 8 x 8)
// Crop during decode of image, confirm output contains no loss of image quality
class RStepCropDuringDecodeColourSample : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeColourSample* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeColourSample(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-CROP-I-0140-HP, MM-ICL-DEC-CROP-I-0150-HP, MM-ICL-DEC-CROP-I-0155-HP
// MM-ICL-DEC-CROP-I-0160-HP, MM-ICL-DEC-CROP-I-0165-HP, MM-ICL-DEC-CROP-I-0170-HP
// MM-ICL-DEC-CROP-I-0180-HP, MM-ICL-DEC-CROP-I-0190-HP, MM-ICL-DEC-CROP-I-0200-HP
// MM-ICL-DEC-CROP-I-0210-HP, MM-ICL-DEC-CROP-I-0212-HP, MM-ICL-DEC-CROP-I-0213-HP
// MM-ICL-DEC-CROP-I-0220-HP, MM-ICL-DEC-CROP-I-0230-HP, MM-ICL-DEC-CROP-I-0240-HP
// MM-ICL-DEC-CROP-I-0260-HP, MM-ICL-DEC-CROP-I-0370-HP, MM-ICL-DEC-CROP-I-0380-HP
// Crop during decode of image, whole image; decode with display mode
// Crop during decode of image, left hand top corner; with display mode
// Crop during decode of image, left hand bottom corner; with display mode
// Crop during decode of image, right hand bottom corner; with display mode
// Crop during decode of image, right hand top corner; with display mode
// Crop during decode of image, centre; with display mode
// Crop during decode of image, origin of region of interest starts on MCU block boundary; with display mode
// Crop during decode of image, x coord of origin for region of interest does not start on MCU block boundary; decode with default display mode
// Crop during decode of image, y coordinate of origin for region of interest does not start on MCU block boundary; with display mode
// Crop during decode of image, x & y coordinates of origin for region of interest do not start on MCU block boundary; with display mode
// Crop during decode of image, x & y coordinates of origin for region of interest do not start on MCU block boundary; with display mode
// Crop during decode of image, x & y coordinates of origin for region of interest do not start on MCU block boundary; with display mode
// Crop during decode of image, width of region of interest ends on MCU block boundary and depth does not
// Crop during decode of image, width of region of interest does not end on MCU block boundary and depth does
// Crop during decode of image, width and depth of region of interest end on MCU block boundary
// Crop during decode of image, both width and depth of region of interest do not end on a MCU block boundary
// Crop during decode of thumbnail image
// Crop during decode of thumbnail image which is different to main image
class RStepCropDuringDecodeDiffRegion : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeDiffRegion* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeDiffRegion(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-CROP-I-0270-HP, MM-ICL-DEC-CROP-I-0275-HP, MM-ICL-DEC-CROP-I-0280-HP
// MM-ICL-DEC-CROP-I-0290-HP, MM-ICL-DEC-CROP-I-0295-HP, MM-ICL-DEC-CROP-I-0300-HP
// MM-ICL-DEC-CROP-I-0310-HP, MM-ICL-DEC-CROP-I-0315-HP, MM-ICL-DEC-CROP-I-0320-HP
// MM-ICL-DEC-CROP-I-0330-HP, MM-ICL-DEC-CROP-I-0340-HP, MM-ICL-DEC-CROP-I-0345-HP
// MM-ICL-DEC-CROP-I-0350-HP
// Crop during decode of image, invalid region of interest, with dimensions of zero
// Crop during decode of image, invalid region of interest, with negative dimensions
// Crop during decode of image, invalid region of interest, (outside) left of image frame
// Crop during decode of image, invalid region of interest, origin (outside) right of image frame
// Crop during decode of image, invalid region of interest, width (outside) right of image frame
// Crop during decode of image, invalid region of interest, (outside) above the image frame
// Crop during decode of image, invalid region of interest, origin (outside) below the image frame
// Crop during decode of image, invalid region of interest, depth (outside) below the image frame
// Crop during decode of image, invalid region of interest, (outside) left and above image frame
// Crop during decode of image, invalid region of interest, (outside) right and above image frame
// Crop during decode of image, invalid region of interest, origin (outside) right and below image frame
// Crop during decode of image, invalid region of interest, width and depth (outside) right and below image frame
// Crop during decode of image, invalid region of interest, origin (outside) left and below image frame
class RStepCropDuringDecodeInvalidRect : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeInvalidRect* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeInvalidRect(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-CROP-I-0360-HP
// Crop during mask decode of image
class RStepCropDuringDecodeMask : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeMask* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeMask(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-CROP-I-0410-HP, MM-ICL-DEC-CROP-I-0420-HP, MM-ICL-DEC-CROP-I-0430-HP
// Crop during decode of image, using valid region of interest, repeat again
// Crop during decode of image, using valid region of interest, repeat again with different region of interest
// Crop during decode of image, using valid region of interest, repeat multiple times with same and different region of interest
class RStepCropDuringDecodeRepeatCrop : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeRepeatCrop* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeRepeatCrop(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    void CropL();

private:
    // number of times crop is repeated on the same image
    TInt iCropRepeatCount;

    // counter to maintain number of times crop is performed
    TInt iIndex;
    };


// MM-ICL-DEC-CROP-I-0440-HP, MM-ICL-DEC-CROP-I-0450-HP, MM-ICL-DEC-CROP-I-0460-HP
// MM-ICL-DEC-CROP-I-0470-HP, MM-ICL-DEC-CROP-I-0480-HP
// Crop during decode of image, downscale to size of valid bitmap
// Crop during decode of image, downscale to size ½ of region of interest
// Crop during decode of image, downscale to size ¼ of region of interest
// Crop during decode of image, downscale to size 1/8 of region of interest
// Crop during decode of image, downscale to size smaller than 1/8 of region of interest
class RStepCropDuringDecodeScale : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeScale* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeScale(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-CROP-I-0500-HP, MM-ICL-DEC-CROP-I-0510-HP, MM-ICL-DEC-CROP-I-0520-HP
// MM-ICL-DEC-CROP-I-0530-HP, MM-ICL-DEC-CROP-I-0540-HP, MM-ICL-DEC-CROP-I-0550-HP
// MM-ICL-DEC-CROP-I-0560-HP, MM-ICL-DEC-CROP-I-0570-HP, MM-ICL-DEC-CROP-I-0580-HP, 
// MM-ICL-DEC-CROP-I-0590-HP
// Record time taken to crop the whole of image during decode
// Record time taken to crop left hand top corner of image during decode
// Record time taken to crop right hand bottom corner of image during decode
// Record time taken to crop region of interest that maps to block boundary of image during decode
// Record time taken to crop region of interest that does not map to block boundaries of image during decode
// Record taken to crop region of interest that is 50% area of the image during decode
// Record taken to crop region of interest that is 25% area of the image during decode
// Record taken to crop region of interest that is 10% area of the image during decode
// Record taken to crop region of interest from a small image during decode
// Record taken to crop region of interest from an average image during decode
class RStepCropDuringDecodePerf : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    };


// MM-ICL-DEC-CROP-I-0610-HP, MM-ICL-DEC-CROP-I-0615-HP, MM-ICL-DEC-CROP-I-0630-HP
// Crop region of interest from an image during decode when data has minor corruption so that a distorted image can be displayed
// Crop region of interest and mirror over the vertical axis an image during decode when data has minor corruption so that a distorted image can be displayed
// Crop region of interest from an image during decode when image contains a corrupt thumbnail image
class RStepCropDuringDecodeInterop : public RStepBaseCropDuringDecode
    {
public:
    static RStepCropDuringDecodeInterop* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
    RStepCropDuringDecodeInterop(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


// MM-ICL-DEC-CROP-I-0650-HP, MM-ICL-DEC-CROP-I-0660-HP, MM-ICL-DEC-CROP-I-0670-HP
// MM-ICL-DEC-CROP-I-0680-HP
// Cancel crop during decode of an image
// Cancel crop during decode of an image then start again
// Cancel crop during decode of an image, which is run in a separate thread
// Cancel crop during decode of an image, which is run in a separate thread, then start again
class RStepCropDuringDecodeCancel : public RStepBaseCropDuringDecode, public MTimerObserver
    {
public:
    static RStepCropDuringDecodeCancel* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

    // from MTimerObserver
	void TimerExpired();

private:
    RStepCropDuringDecodeCancel(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
    };


#endif // TSI_ICL_DECODE_CROP_H
