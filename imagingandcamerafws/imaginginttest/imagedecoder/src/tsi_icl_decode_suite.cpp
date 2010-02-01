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
// This file contains test suite class implementation for image decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_suite.h"
#include "tsi_icl_decode_stepbase.h"
#include "tsi_icl_decode_perf.h"
#include "tsi_icl_decode_crop.h"
#include "tsi_icl_decode_scale.h"
#include "tsi_icl_decode_stream.h"
#include "tsi_icl_decode_rotation.h"
#include "tsi_icl_decode_multipleop.h"
#include "tsi_icl_decode_extinterface.h"


// Create an instance of this test suite, exported at ordinal 1.
// This provides the interface to allow the Test Framework
// to create instances of this test suite.
EXPORT_C CTestSuiteImageDecoder* NewTestSuiteL()
    {
    return CTestSuiteImageDecoder::NewL();
    }


CTestSuiteImageDecoder* CTestSuiteImageDecoder::NewL()
    {
    CTestSuiteImageDecoder* self = new (ELeave) CTestSuiteImageDecoder;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CTestSuiteImageDecoder::~CTestSuiteImageDecoder()
    {
    }


CTestSuiteImageDecoder::CTestSuiteImageDecoder()
    {
    // Name of this test suite
    iSuiteName = _L("tsi_icl_imagedecoder_suite");
    }


// Get version of the suite
TPtrC CTestSuiteImageDecoder::GetVersion() const
    {
    // update the version, with appropriate version number, when the suite is updated, after first MCL submission
    _LIT(KSuiteVersion, "v1.0");
    return KSuiteVersion();
    }


// This creates all the test steps and stores them inside CTestSuiteImageDecoder
void CTestSuiteImageDecoder::InitialiseL()
    {
    // Section Name is [xxxx-xP] in the configuration file

    // Baseline Performance----------------------------------------------------
    // Decode image
	AddTestStepL(RStepImageDecodePerf::NewL(_L("MM-ICL-DEC-PERF-I-0100-HP"), _L("Record time taken to decode thumbnail image")));
	AddTestStepL(RStepImageDecodePerf::NewL(_L("MM-ICL-DEC-PERF-I-0110-HP"), _L("Record time taken to decode small image")));
	AddTestStepL(RStepImageDecodePerf::NewL(_L("MM-ICL-DEC-PERF-I-0120-HP"), _L("Record time taken to decode average size image")));
    // Decode image and Scale
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0130-HP"), _L("Record time taken to decode small image, and downscale to 1/2")));
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0730-HP"), _L("Record time taken to decode small image, and downscale to 1/2")));
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0140-HP"), _L("Record time taken to decode average size image, and downscale to 1/2")));
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0150-HP"), _L("Record time taken to decode small image, and downscale to 1/4")));
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0160-HP"), _L("Record time taken to decode average size image, and downscale to 1/4")));
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0170-HP"), _L("Record time taken to decode small image, and downscale to 1/8")));
	AddTestStepL(RStepImageDecodeScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0180-HP"), _L("Record time taken to decode average size image, and downscale to 1/8")));
    // Decode image and Double Scale
	AddTestStepL(RStepImageDecodeDoubleScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0700-HP"), _L("Record time taken to decode small image to downscale to 1/8 and bitmap downscale by 1/2")));
	AddTestStepL(RStepImageDecodeDoubleScalePerf::NewL(_L("MM-ICL-DEC-PERF-I-0710-HP"), _L("Record time taken to decode average size image to downscale to 1/8 and bitmap downscale by 1/2")));
    // Decode image and Rotate bitmap
    AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0720-HP"), _L("Record time taken to decode thumbnail image and rotate 90 degrees clockwise")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0200-HP"), _L("Record time taken to decode thumbnail image and rotate 90 degrees clockwise")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0210-HP"), _L("Record time taken to decode small image and rotate 180 degrees clockwise")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0220-HP"), _L("Record time taken to decode average size image and rotate 270 degrees clockwise")));
	// Decode image and Mirror bitmap
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0300-HP"), _L("Record time taken to decode thumbnail image and mirror the bitmap over the vertical axis")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0310-HP"), _L("Record time taken to decode small image and mirror the bitmap over the vertical axis")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0320-HP"), _L("Record time taken to decode average size image and mirror the bitmap over the vertical axis")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0330-HP"), _L("Record time taken to decode thumbnail image and mirror the bitmap over the horizontal axis")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0340-HP"), _L("Record time taken to decode small image and mirror the bitmap over the horizontal axis")));
	AddTestStepL(RStepImageDecodeRotatePerf::NewL(_L("MM-ICL-DEC-PERF-I-0350-HP"), _L("Record time taken to decode average size image and mirror the bitmap over the horizontal axis")));
    // Decode image then Rotate and Mirror bitmap
	AddTestStepL(RStepImageDecodeRotateMirrorPerf::NewL(_L("MM-ICL-DEC-PERF-I-0400-HP"), _L("Record time taken to decode thumbnail image then rotate and mirror the bitmap over the vertical axis")));
	AddTestStepL(RStepImageDecodeRotateMirrorPerf::NewL(_L("MM-ICL-DEC-PERF-I-0410-HP"), _L("Record time taken to decode small image then rotate and mirror the bitmap over the horizontal axis")));
	AddTestStepL(RStepImageDecodeRotateMirrorPerf::NewL(_L("MM-ICL-DEC-PERF-I-0420-HP"), _L("Record time taken to decode average size image then rotate and mirror the bitmap over the vertical axis")));
	// Decode image then Mirror and Rotate bitmap
	AddTestStepL(RStepImageDecodeRotateMirrorPerf::NewL(_L("MM-ICL-DEC-PERF-I-0500-HP"), _L("Record time taken to decode thumbnail image then mirror over the vertical axis and rotate the bitmap")));
	AddTestStepL(RStepImageDecodeRotateMirrorPerf::NewL(_L("MM-ICL-DEC-PERF-I-0510-HP"), _L("Record time taken to decode small image then mirror over the horizontal axis and rotate the bitmap")));
	AddTestStepL(RStepImageDecodeRotateMirrorPerf::NewL(_L("MM-ICL-DEC-PERF-I-0520-HP"), _L("Record time taken to decode average size image then mirror over the vertical axis and rotate the bitmap")));


    // Extension Interface(s) for Image Decoder--------------------------------
    // Crop during decoding
    // Crop
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-CROP-I-0010-HP"), _L("Confirm whether the codec supports extension interface during decoding using plug-in properties")));
    // Crop, Rotate and Mirror over Horizontal and Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-CROP-I-0015-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Crop and Streamed decoding
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-CROP-I-0020-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Crop
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-CROP-I-0025-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Crop
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-CROP-I-0030-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // test case added for ImageCommentL 
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-CROP-I-0046-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // Crop, Rotate and Mirror over Horizontal and Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-CROP-I-0035-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));
    // Crop and Streamed decoding
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-CROP-I-0040-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));
    // Crop
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-CROP-I-0045-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));

    // Scale during decoding
    // Scale
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-SCAL-I-0010-HP"), _L("Confirm whether the codec supports extension interface during decoding using plug-in properties")));
    // Crop and Scale
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-SCAL-I-0020-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Crop, Scale, Rotate and Mirror over Horizontal and Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-SCAL-I-0030-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Scale and Streamed decoding
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-SCAL-I-0040-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Scale
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-SCAL-I-0045-HP"), _L("Confirm whether the codec supports extension interface during decoding using plug-in properties")));
    // Scale
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-SCAL-I-0050-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // Crop and Scale
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-SCAL-I-0060-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));
    // Crop, Scale, Rotate and Mirror over Horizontal and Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-SCAL-I-0070-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));
    // Scale and Streamed decoding
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-SCAL-I-0080-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));
    // Scale
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-SCAL-I-0085-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));

    // Rotate and/or Mirror image during decoding
    // Rotate and Mirror Horizontal and Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-RTMR-I-0010-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Rotation
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-RTMR-I-0012-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // Mirror Horizontal axis
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-RTMR-I-0014-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // Mirror Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-RTMR-I-0016-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // Rotate and Mirror Horizontal and Vertical axis
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-RTMR-I-0020-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));

    // Streamed decoding
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-STRM-I-0010-HP"), _L("Confirm whether the codec supports extension interface during decoding using plug-in properties")));
    // Streamed decoding and Rotation during decoding
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-STRM-I-0020-HP"), _L("Confirm whether the codec supports extension interfaces during decoding using plug-in properties")));
    // Streamed decoding with GIF image
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-STRM-I-0025-HP"), _L("Confirm whether the codec supports extension interface during decoding using plug-in properties")));
    // Streamed decoding
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-STRM-I-0030-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));
    // Streamed decoding and Rotation during decoding
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-STRM-I-0040-HP"), _L("Confirm whether the codec supports extension interfaces during decoding by loading the codec")));
    // Streamed decoding with GIF image
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-STRM-I-0045-HP"), _L("Confirm whether the codec supports extension interface during decoding by loading the codec")));

    // AutoRotate image during decoding based on  Exif Orientation tag
    // autorotate with different JPEG codec
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-ARTO-I-0120-HP"), _L("Autorotate and mirror horizontal input image during decode with JPEG codec which doesn't support autorotate")));
    // plugin properties
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-ARTO-I-0130-HP"), _L("Confirm whether the loaded codec supports autorotate during decoding using plug-in properties")));
    // interfaceimplementation
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-ARTO-I-0140-HP"), _L("Confirm whether the available codec supports autorotate during decoding using inetrfaceimplementations")));
    // FrameInfo with autorotate option set 
    AddTestStepL(RStepImageDecodeExtInterfaceLoadCodec::NewL(_L("MM-ICL-DEC-ARTO-I-0150-HP"), _L("Confirm whether the FrameInfo returns frameinfo during decoding taking into account orientation tag set")));
    // plugin properties with codec which doesn't support AutoRotate
    AddTestStepL(RStepImageDecodeExtInterfacePluginProp::NewL(_L("MM-ICL-DEC-ARTO-I-0180-HP"), _L("Confirm whether the loaded codec which doesn't supports autorotate during decoding using plug-in properties fails")));
        
    // Region of Interest (cropping) during decoding---------------------------
    // Destination and Source
	AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0730-HP"), _L("Crop during decode of image, output as a bitmap (CFbsBitmap)")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0050-HP"), _L("Crop during decode of image, output as a bitmap (CFbsBitmap)")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0070-HP"), _L("Crop during decode of image, output as a bitmap (CFbsBitmap), run in a separate thread")));
    AddTestStepL(RStepCropDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-CROP-I-0060-HP"), _L("Crop during decode of image, output as a YUV pixel buffer")));
    AddTestStepL(RStepCropDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-CROP-I-0080-HP"), _L("Crop during decode of image, output as a YUV pixel buffer, run in a separate thread")));
    AddTestStepL(RStepCropDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-CROP-I-0100-HP"), _L("Crop during decode of image, attempt to output to unsupported YUV format")));
    AddTestStepL(RStepCropDuringDecodeInvalidDestSize::NewL(_L("MM-ICL-DEC-CROP-I-0090-HP"), _L("Crop during decode of image, attempt to output to invalid sized bitmap (size of the output bitmap less than 1/8 of crop size)")));
    AddTestStepL(RStepCropDuringDecodeInvalidDestSize::NewL(_L("MM-ICL-DEC-CROP-I-0095-HP"), _L("Crop during decode of image, attempt to output to bitmap larger than original image")));
    AddTestStepL(RStepCropDuringDecodeInvalidDestSize::NewL(_L("MM-ICL-DEC-CROP-I-0098-HP"), _L("Crop during decode of image, attempt to output to invalid sized bitmap (size of the output bitmap greater than crop size)")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0105-HP"), _L("Crop during decode of image, cancel the crop, output the original image as a bitmap (CFbsBitmap)")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0700-HP"), _L("Crop during decode of multiscan image")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0710-HP"), _L("Crop during decode of progressive image")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0720-HP"), _L("Crop during decode of image with DRI (Define Restart Interoperability) marker")));
    // Boundary Cases
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0400-HP"), _L("Crop during decode of image, which is the smallest possible")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0401-HP"), _L("Crop during decode of image, which is the smallest possible")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0402-HP"), _L("Crop during decode of image, which is the smallest possible")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0403-HP"), _L("Crop during decode of image, which is the smallest possible")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0404-HP"), _L("Crop during decode of image, which is the smallest possible")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0405-HP"), _L("Crop during decode of image, which is the smallest possible")));
    AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-CROP-I-0406-HP"), _L("Crop during decode of image, which is the smallest possible")));
    // Colour Sample (Block Size)
    AddTestStepL(RStepCropDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-CROP-I-0110-HP"), _L("Crop during decode of image with colour sample Monochrome (block size 8 x 8)")));
    AddTestStepL(RStepCropDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-CROP-I-0120-HP"), _L("Crop during decode of image with colour sample 4:2:0 (block size 16 x 16)")));
    AddTestStepL(RStepCropDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-CROP-I-0130-HP"), _L("Crop during decode of image with colour sample 4:2:2 (block size 8 x 16)")));
    AddTestStepL(RStepCropDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-CROP-I-0135-HP"), _L("Crop during decode of image with colour sample 4:4:4 (block size 8 x 8)")));
    // Image Quality
    AddTestStepL(RStepCropDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-CROP-I-0690-HP"), _L("Crop during decode of image, confirm output contains no loss of image quality")));
    // Region of Interest
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0140-HP"), _L("Crop during decode of image, whole image; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0150-HP"), _L("Crop during decode of image, left hand top corner; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0155-HP"), _L("Crop during decode of image, left hand bottom corner; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0160-HP"), _L("Crop during decode of image, right hand bottom corner; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0165-HP"), _L("Crop during decode of image, right hand top corner; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0170-HP"), _L("Crop during decode of image, centre; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0180-HP"), _L("Crop during decode of image, origin of region of interest starts on MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0190-HP"), _L("Crop during decode of image, x coord of origin for region of interest does not start on MCU block boundary; decode with default display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0200-HP"), _L("Crop during decode of image, y coordinate of origin for region of interest does not start on MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0210-HP"), _L("Crop during decode of image, x & y coordinates of origin for region of interest do not start on MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0212-HP"), _L("Crop during decode of image, x & y coordinates of origin for region of interest do not start on MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0213-HP"), _L("Crop during decode of image, x & y coordinates of origin for region of interest do not start on MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0220-HP"), _L("Crop during decode of image, width of region of interest ends on MCU block boundary and depth does not; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0230-HP"), _L("Crop during decode of image, width of region of interest does not end on MCU block boundary and depth does; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0240-HP"), _L("Crop during decode of image, width and depth of region of interest end on MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0260-HP"), _L("Crop during decode of image, both width and depth of region of interest do not end on a MCU block boundary; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0270-HP"), _L("Crop during decode of image, invalid region of interest, with dimensions of zero")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0275-HP"), _L("Crop during decode of image, invalid region of interest, with negative dimensions")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0280-HP"), _L("Crop during decode of image, invalid region of interest, (outside) left of image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0290-HP"), _L("Crop during decode of image, invalid region of interest, origin (outside) right of image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0295-HP"), _L("Crop during decode of image, invalid region of interest, width (outside) right of image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0300-HP"), _L("Crop during decode of image, invalid region of interest, (outside) above the image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0310-HP"), _L("Crop during decode of image, invalid region of interest, origin (outside) below the image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0315-HP"), _L("Crop during decode of image, invalid region of interest, depth (outside) below the image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0320-HP"), _L("Crop during decode of image, invalid region of interest, (outside) left and above image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0330-HP"), _L("Crop during decode of image, invalid region of interest, (outside) right and above image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0340-HP"), _L("Crop during decode of image, invalid region of interest, origin (outside) right and below image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0345-HP"), _L("Crop during decode of image, invalid region of interest, width and depth (outside) right and below image frame")));
    AddTestStepL(RStepCropDuringDecodeInvalidRect::NewL(_L("MM-ICL-DEC-CROP-I-0350-HP"), _L("Crop during decode of image, invalid region of interest, origin (outside) left and below image frame")));
    AddTestStepL(RStepCropDuringDecodeMask::NewL(_L("MM-ICL-DEC-CROP-I-0360-HP"), _L("Crop during mask decode of image")));
    // Thumbnail
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0370-HP"), _L("Crop during decode of thumbnail image; decode with display mode")));
    AddTestStepL(RStepCropDuringDecodeDiffRegion::NewL(_L("MM-ICL-DEC-CROP-I-0380-HP"), _L("Crop during decode of thumbnail image which is different to main image; decode with display mode")));
    // Repeat and Multiple Crops on image
    AddTestStepL(RStepCropDuringDecodeRepeatCrop::NewL(_L("MM-ICL-DEC-CROP-I-0410-HP"), _L("Crop during decode of image, using valid region of interest, repeat again")));
    AddTestStepL(RStepCropDuringDecodeRepeatCrop::NewL(_L("MM-ICL-DEC-CROP-I-0420-HP"), _L("Crop during decode of image, using valid region of interest, repeat again with different region of interest")));
    AddTestStepL(RStepCropDuringDecodeRepeatCrop::NewL(_L("MM-ICL-DEC-CROP-I-0430-HP"), _L("Crop during decode of image, using valid region of interest, repeat multiple times with same and different region of interest")));
    // Downscale image
    AddTestStepL(RStepCropDuringDecodeScale::NewL(_L("MM-ICL-DEC-CROP-I-0440-HP"), _L("Crop during decode of image, downscale to size of valid bitmap")));
    AddTestStepL(RStepCropDuringDecodeScale::NewL(_L("MM-ICL-DEC-CROP-I-0450-HP"), _L("Crop during decode of image, downscale to size 1/2 of region of interest")));
    AddTestStepL(RStepCropDuringDecodeScale::NewL(_L("MM-ICL-DEC-CROP-I-0460-HP"), _L("Crop during decode of image, downscale to size 1/4 of region of interest")));
    AddTestStepL(RStepCropDuringDecodeScale::NewL(_L("MM-ICL-DEC-CROP-I-0470-HP"), _L("Crop during decode of image, downscale to size 1/8 of region of interest")));
    AddTestStepL(RStepCropDuringDecodeScale::NewL(_L("MM-ICL-DEC-CROP-I-0480-HP"), _L("Crop during decode of image, downscale to size smaller than 1/8 of region of interest")));
    // Performance
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0500-HP"), _L("Record time taken to crop the whole of image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0510-HP"), _L("Record time taken to crop left hand top corner of image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0520-HP"), _L("Record time taken to crop right hand bottom corner of image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0530-HP"), _L("Record time taken to crop region of interest that maps to block boundary of image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0540-HP"), _L("Record time taken to crop region of interest that does not map to block boundaries of image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0550-HP"), _L("Record taken to crop region of interest that is 50 percent area of the image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0560-HP"), _L("Record taken to crop region of interest that is 25 percent area of the image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0570-HP"), _L("Record taken to crop region of interest that is 10 percent area of the image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0580-HP"), _L("Record taken to crop region of interest from a small image during decode")));
    AddTestStepL(RStepCropDuringDecodePerf::NewL(_L("MM-ICL-DEC-CROP-I-0590-HP"), _L("Record taken to crop region of interest from an average image during decode")));
    // Interoperability
    AddTestStepL(RStepCropDuringDecodeInterop::NewL(_L("MM-ICL-DEC-CROP-I-0610-HP"), _L("Crop region of interest from an image during decode when data has minor corruption so that a distorted image can be displayed")));
    AddTestStepL(RStepCropDuringDecodeInterop::NewL(_L("MM-ICL-DEC-CROP-I-0615-HP"), _L("Crop region of interest, mirror image during decode over vertical axis,when data has minor corruption,display distorted image")));
    AddTestStepL(RStepCropDuringDecodeInterop::NewL(_L("MM-ICL-DEC-CROP-I-0630-HP"), _L("Crop region of interest from an image during decode when image contains a corrupt thumbnail image")));
    // Real-Time Events
    AddTestStepL(RStepCropDuringDecodeCancel::NewL(_L("MM-ICL-DEC-CROP-I-0650-HP"), _L("Cancel crop during decode of an image")));
    AddTestStepL(RStepCropDuringDecodeCancel::NewL(_L("MM-ICL-DEC-CROP-I-0660-HP"), _L("Cancel crop during decode of an image then start again")));
    AddTestStepL(RStepCropDuringDecodeCancel::NewL(_L("MM-ICL-DEC-CROP-I-0670-HP"), _L("Cancel crop during decode of an image, which is run in a separate thread")));
    AddTestStepL(RStepCropDuringDecodeCancel::NewL(_L("MM-ICL-DEC-CROP-I-0680-HP"), _L("Cancel crop during decode of an image, which is run in a separate thread, then start again")));


    // Streamed decoding-------------------------------------------------------
    // Source and Destination
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0070-HP"), _L("Streamed decoding of sequential block from JPEG image to YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0080-HP"), _L("Streamed decoding of forward random block from JPEG image to YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0085-HP"), _L("Streamed decoding of backward random block from JPEG image to YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0090-HP"), _L("Streamed decoding of sequential block from JPEG image to YUV buffer, in separate thread")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0100-HP"), _L("Streamed decoding of forward random block from JPEG image to YUV pixel buffer, in separate thread")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0105-HP"), _L("Streamed decoding of backward random block from JPEG image to YUV pixel buffer, in separate thread")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0110-HP"), _L("Streamed decoding of sequential block from JPEG image to invalid destination, YUV format not supported")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0115-HP"), _L("Streamed decoding of random block from JPEG image to invalid destination, YUV format not supported")));
    // Thumbnail and EXIF
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0120-HP"), _L("Streamed decoding of sequential block from main image in JPEG file which contains EXIF and thumbnail")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0125-HP"), _L("Streamed decoding of forward random block from main image in JPEG file which contains EXIF and thumbnail")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0126-HP"), _L("Streamed decoding of backward random block from main image in JPEG file which contains EXIF and thumbnail")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0130-HP"), _L("Sequential streamed decoding of thumbnail in JPEG image with colorsampling 422")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0131-HP"), _L("Sequential streamed transcoding of thumbnail in JPEG image with colorsampling 420 or 444, error reported - Test GetSupportedFormatsL()")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0132-HP"), _L("Sequential streamed transcoding of thumbnail in JPEG image with colorsampling 420 or 444, error reported - Test InitFrameL()")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0135-HP"), _L("Random streamed decoding of thumbnail in JPEG image, error reported")));
    // Interoperability
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0140-HP"), _L("Streamed decoding of Progressive JPEG image, error 'Not Supported' reported")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0150-HP"), _L("Streamed decoding of Progressive JPEG image, progressive flag invalid ")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0155-HP"), _L("Streamed decoding of multiscan JPEG image")));    
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0160-HP"), _L("Streamed decoding of empty JPEG image")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0170-HP"), _L("Streamed decoding of JPEG image when JPEG data is very corrupt and can't be opened")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0180-HP"), _L("Streamed decoding of a JPEG image during decode when JPEG data has minor corruption so that a distorted image can be displayed")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0190-HP"), _L("Streamed decoding of corrupt thumbnail in JPEG file")));
    // Colour Sampling
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0200-HP"), _L("Stream decode of sequential block from JPEG file with colour sample Monochrome")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0205-HP"), _L("Stream decode of random block from JPEG file with colour sample Monochrome")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0210-HP"), _L("Stream decode of forward random block from JPEG file with colour sample 4:2:0 (default)")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0215-HP"), _L("Stream decode of backward random block from JPEG file with colour sample 4:2:0 (default)")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0220-HP"), _L("Stream decode of sequential block from JPEG file with colour sample 4:2:2")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0230-HP"), _L("Stream decode of backward random block from JPEG file with colour sample 4:2:2")));
    // DownScale Image
    AddTestStepL(RStepStreamedDecodeToImageScale::NewL(_L("MM-ICL-DEC-STRM-I-0240-HP"), _L("Stream decode of JPEG image, downscale to size 1/2")));
    AddTestStepL(RStepStreamedDecodeToImageScale::NewL(_L("MM-ICL-DEC-STRM-I-0250-HP"), _L("Stream decode of JPEG image, downscale to size 1/4 ")));
    AddTestStepL(RStepStreamedDecodeToImageScale::NewL(_L("MM-ICL-DEC-STRM-I-0260-HP"), _L("Stream decode of JPEG image, downscale to size 1/8")));
    AddTestStepL(RStepStreamedDecodeToImageScale::NewL(_L("MM-ICL-DEC-STRM-I-0270-HP"), _L("Stream decode of JPEG image, downscale to size smaller than 1/8")));
    // Multiple Blocks, Sequential
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0280-HP"), _L("Stream decode JPEG first sequential block output to a YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0290-HP"), _L("Stream decode JPEG second sequential block output to a YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0300-HP"), _L("Stream decode JPEG last sequential block output to a YUV pixel buffer, buffer size matches data remaining in JPEG file")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0310-HP"), _L("Stream decode JPEG last sequential block output to a YUV pixel buffer, buffer size greater then data remaining in JPEG file")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0320-HP"), _L("Stream decode JPEG sequential block output to a YUV pixel buffer, start at top left, block taken from single row of JPEG image ")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0330-HP"), _L("Stream decode JPEG sequential block output to a YUV pixel buffer, start at top left, blocks taken from two rows of JPEG image ")));
    // Multiple Blocks, Random
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0340-HP"), _L("Stream decode JPEG first block using forward random access output to a YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0345-HP"), _L("Stream decode JPEG first block using backward random access output to a YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0350-HP"), _L("Stream decode JPEG any block using forward random access output to a YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0352-HP"), _L("Stream decode JPEG multiple blocks using forward random access")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0355-HP"), _L("Stream decode JPEG any block using backward random access output to a YUV pixel buffer")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0360-HP"), _L("Stream decode JPEG last block using forward random access to blocks output to a YUV pixel buffer, size matches data remaining in JPEG file")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0365-HP"), _L("Stream decode JPEG last block using backward random access to output to a YUV pixel buffer, size matches data remaining in JPEG file")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0370-HP"), _L("Stream decode JPEG last block using forward random access, output to a YUV pixel buffer, size greater then data remaining in JPEG file")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0380-HP"), _L("Stream decode JPEG block using forward random access to blocks output to a YUV pixel buffer, block from within a single row ")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0385-HP"), _L("Stream decode JPEG block using forward random access output to a YUV pixel buffer, block from with multiple rows")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0390-HP"), _L("Stream decode JPEG multiple blocks using forward and backward random access ")));
    // Boundary Cases
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0400-HP"), _L("Stream decode of largest possible JPEG")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0410-HP"), _L("Stream decode of smallest possible JPEG")));
    // Performance
    AddTestStepL(RStepStreamedDecodeToImagePerf::NewL(_L("MM-ICL-DEC-STRM-I-0420-HP"), _L("Record time taken to stream decode from small image")));
    AddTestStepL(RStepStreamedDecodeToImagePerf::NewL(_L("MM-ICL-DEC-STRM-I-0430-HP"), _L("Record time taken to stream decode average size JPEG image")));
    AddTestStepL(RStepStreamedDecodeToImagePerf::NewL(_L("MM-ICL-DEC-STRM-I-0440-HP"), _L("Record time taken to stream decode large JPEG image")));
    AddTestStepL(RStepStreamedDecodeToImagePerf::NewL(_L("MM-ICL-DEC-STRM-I-0450-HP"), _L("Record time taken to stream decode super-size JPEG image")));
    // RealTime Event
    AddTestStepL(RStepStreamedDecodeToImageRealTimeTests::NewL(_L("MM-ICL-DEC-STRM-I-0470-HP"), _L("Cancel sequential stream decode of JPEG image")));
    AddTestStepL(RStepStreamedDecodeToImageRealTimeTests::NewL(_L("MM-ICL-DEC-STRM-I-0480-HP"), _L("Cancel sequential stream decode of JPEG image then start again")));
    AddTestStepL(RStepStreamedDecodeToImageRealTimeTests::NewL(_L("MM-ICL-DEC-STRM-I-0490-HP"), _L("Cancel sequential stream decode of JPEG image, which is run in a separate thread")));
    AddTestStepL(RStepStreamedDecodeToImageRealTimeTests::NewL(_L("MM-ICL-DEC-STRM-I-0500-HP"), _L("Cancel sequential stream decode of JPEG image, which is run in a separate thread. Then start again")));
    // Image Quality
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0510-HP"), _L("Confirm that there is no loss of image quality between stream decode and decode JPEG image")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0520-HP"), _L("Streamed decoding of forward random block from JPEG image with DRI (Define Restart Interoperability) marker")));
    AddTestStepL(RStepStreamedDecodeToImage::NewL(_L("MM-ICL-DEC-STRM-I-0530-HP"), _L("Streamed decoding of sequential block from JPEG image with DRI (Define Restart Interoperability) marker")));
    
    
    // Rotation or Mirroring during decoding-----------------------------------
    // Destination and Source
    AddTestStepL(RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-RTMR-I-0040-HP"), _L("Rotate and mirror during decode of image, output as a YUV pixel buffer")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-RTMR-I-0045-HP"), _L("Decode the input image without applying operations and  output as a YUV pixel buffer")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-RTMR-I-0060-HP"), _L("Rotate and mirror during decode of image, output as a YUV pixel buffer, run in a separate thread")));
    // Destination and Source and Rotate and/or Mirror
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0740-HP"), _L("Passing Decoderuid to filenewl and call covert to apply the ImageConversion operations")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0030-HP"), _L("Rotate and mirror during decode of image, output as a bitmap (CFbsBitmap)")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0050-HP"), _L("Rotate and mirror during decode of image, output as a bitmap (CFbsBitmap), run in a separate thread")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0070-HP"), _L("Rotate image 90 degrees clockwise during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0080-HP"), _L("Rotate image 180 degrees clockwise during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0090-HP"), _L("Rotate image 270 degrees clockwise during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0100-HP"), _L("Mirror image over the horizontal axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0110-HP"), _L("Mirror image over the vertical axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0120-HP"), _L("Mirror image over the horizontal and vertical axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0130-HP"), _L("Rotate image 90 degrees clockwise and mirror over the horizontal axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0140-HP"), _L("Rotate image 180 degrees clockwise and mirror over the horizontal axis during decode of image; decode with default display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0150-HP"), _L("Rotate image 270 degrees clockwise and mirror over the horizontal axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0160-HP"), _L("Rotate image 90 degrees clockwise and mirror over the vertical axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0162-HP"), _L("Rotate image 90 degrees clockwise and mirror over the vertical axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0163-HP"), _L("Rotate image 90 degrees clockwise and mirror over the vertical axis during decode of image; decode with display mode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0170-HP"), _L("Rotate image 180 degrees clockwise and mirror over the vertical axis during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0180-HP"), _L("Rotate image 270 degrees clockwise and mirror over the vertical axis during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0190-HP"), _L("Mirror over the horizontal axis and rotate image 90 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0200-HP"), _L("Mirror over the horizontal axis and rotate image 180 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0210-HP"), _L("Mirror over the horizontal axis and rotate image 270 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0220-HP"), _L("Mirror over the vertical axis and rotate image 90 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0230-HP"), _L("Mirror over the vertical axis and rotate image 180 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0240-HP"), _L("Mirror over the vertical axis and rotate image 270 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0250-HP"), _L("Mirror over the horizontal and vertical axis and rotate image 90 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0260-HP"), _L("Mirror over the horizontal and vertical axis and rotate image 180 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0270-HP"), _L("Mirror over the horizontal and vertical axis and rotate image 270 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0700-HP"), _L("Rotate and mirror during decode of multiscan image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0710-HP"), _L("Rotate and mirror during decode of progressive image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0720-HP"), _L("Rotate and mirror during decode of image with DRI (Define Restart Interoperability) marker")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0730-HP"), _L("Checking the Prepare API before doing the convert")));
    // Colour Sample
    AddTestStepL(RStepRotateOrMirrorDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-RTMR-I-0280-HP"), _L("Mirror over the horizontal and vertical axis and rotate image 90 degrees clockwise during decode of image with colour sample Monochrome")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-RTMR-I-0290-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis image during decode of image with colour sample 4:2:0 (default)")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-RTMR-I-0300-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis image during decode of image with colour sample 4:2:2")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-RTMR-I-0310-HP"), _L("Mirror over the horizontal and vertical axis and rotate image 90 degrees clockwise during decode of image with colour sample 4:4:4")));
    // Image Quality
    AddTestStepL(RStepRotateOrMirrorDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-RTMR-I-0690-HP"), _L("Mirror and rotate during decode of image, confirm output contains no loss of image quality")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeColourSample::NewL(_L("MM-ICL-DEC-RTMR-I-0680-HP"), _L("Rotate and mirror during decode of image, confirm output contains no loss of image quality")));
    // Thumbnail
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0320-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis of the image during decode of thumbnail image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0330-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis of the image during decode of thumbnail image,different to main")));
    // Downscale image
    AddTestStepL(RStepRotateOrMirrorDuringDecodeScale::NewL(_L("MM-ICL-DEC-RTMR-I-0350-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to size of valid bitmap")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeScale::NewL(_L("MM-ICL-DEC-RTMR-I-0360-HP"), _L("Mirror over the horizontal and vertical axis and rotate 90 degrees clockwise during decode of image, downscale to 1/2 size")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeScale::NewL(_L("MM-ICL-DEC-RTMR-I-0370-HP"), _L("Rotate 180 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to 1/4 size")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeScale::NewL(_L("MM-ICL-DEC-RTMR-I-0380-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to 1/8 size")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeScale::NewL(_L("MM-ICL-DEC-RTMR-I-0390-HP"), _L("Rotate 90 degrees clockwise and mirror over the horizontal and vertical axis during decode of image, downscale to size smaller than 1/8")));
    // Performance
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0400-HP"), _L("Record time taken to rotate 90 degrees clockwise a small image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0410-HP"), _L("Record time taken to rotate 180 degrees clockwise a small image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0420-HP"), _L("Record time taken to rotate 270 degrees clockwise a small image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0430-HP"), _L("Record time taken to rotate 90 degrees clockwise an average image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0440-HP"), _L("Record time taken to rotate 180 degrees clockwise an average image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0450-HP"), _L("Record time taken to rotate 270 degrees clockwise an average image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0460-HP"), _L("Record time taken to mirror over the horizontal axis a small image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0470-HP"), _L("Record time taken to mirror over the horizontal axis an average image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0480-HP"), _L("Record time taken to mirror over the vertical axis a small image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0490-HP"), _L("Record time taken to mirror over the vertical axis an average image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0500-HP"), _L("Record time taken to rotate 90 degress clockwise and mirror over the horizontal axis during decode of a small image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0510-HP"), _L("Record time taken to rotate 180 degress clockwise and mirror over the vertical axis during decode of a small image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0520-HP"), _L("Record time taken to rotate 270 degress clockwise and mirror over the vertical and horizontal axis during decode of a small image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0530-HP"), _L("Record time taken to mirror over the vertical axis and rotate 90 degress clockwise during decode of a small image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0540-HP"), _L("Record time taken to mirror over the horizontal axis and rotate 180 degress clockwise during decode of a small image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodePerf::NewL(_L("MM-ICL-DEC-RTMR-I-0550-HP"), _L("Record time taken to mirror over the vertical and horizontal axis and rotate 270 degress clockwise during decode of a small image")));
    // Interoperability
    AddTestStepL(RStepRotateOrMirrorDuringDecodeInterop::NewL(_L("MM-ICL-DEC-RTMR-I-0590-HP"), _L("Mirror and rotate an image during decode, image contains data which is minor corrupted so that a distorted image can be produced")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeInterop::NewL(_L("MM-ICL-DEC-RTMR-I-0600-HP"), _L("Mirror and rotate an image during decode, image contains data which is corrupted and causes underflow error")));
    // Real-Time Events
    AddTestStepL(RStepRotateOrMirrorDuringDecodeCancel::NewL(_L("MM-ICL-DEC-RTMR-I-0620-HP"), _L("Cancel rotate and mirror during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeCancel::NewL(_L("MM-ICL-DEC-RTMR-I-0630-HP"), _L("Cancel mirror and rotate during decode of image, then start again")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeCancel::NewL(_L("MM-ICL-DEC-RTMR-I-0640-HP"), _L("Cancel rotate and mirror during decode of image, which is run in a separate thread")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeCancel::NewL(_L("MM-ICL-DEC-RTMR-I-0650-HP"), _L("Cancel mirror and rotate during decode of image, which is run in a separate thread, then start again")));
    // Thumbnail and second decode
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage::NewL(_L("MM-ICL-DEC-RTMR-I-0660-HP"), _L("Rotate thumbnail and image 90 degrees clockwise during decode of image")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirrorBothImage::NewL(_L("MM-ICL-DEC-RTMR-I-0670-HP"), _L("Mirror over the vertical axis and rotate image and thumbnail 180 degrees clockwise during decode of image")));
	
	AddTestStepL(RStepRotateOrMirrorBufferDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0750-HP"), _L("Rotate and mirror during decode of image with buffer")));
	AddTestStepL(RStepRotateOrMirrorBufferDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0760-HP"), _L("Rotate and mirror during decode of image with buffer with decoderuid")));
	AddTestStepL(RStepRotateOrMirrorIntentDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0770-HP"), _L("Rotate and mirror during decode of image with intent with decoderuid")));
	AddTestStepL(RStepRotateOrMirrorIntentDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0780-HP"), _L("Rotate and mirror during decode of image with intent with decoderuid")));
	AddTestStepL(RStepRotateOrMirrorDRMDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-RTMR-I-0790-HP"), _L("Rotate and mirror during decode of image with DRM source with decoderuid")));
	
    // Scaling during decoding-------------------------------------------------
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0730-HP"), _L("Downscale during decode of an image to full size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0110-HP"), _L("Downscale during decode of an image to full size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0120-HP"), _L("Downscale during decode of an image to size 1/2; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0130-HP"), _L("Downscale during decode of an image to size 1/4; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0140-HP"), _L("Downscale during decode of an image to size 1/8; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0150-HP"), _L("Downscale during decode of a thumbnail to full size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0160-HP"), _L("Downscale during decode of a thumbnail thumbnail to size 1/2; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0170-HP"), _L("Downscale during decode of a thumbnail thumbnail to size 1/4; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0180-HP"), _L("Downscale during decode of a thumbnail thumbnail to size 1/8; decode with default display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0190-HP"), _L("Downscale during decode of an image to full size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0195-HP"), _L("Downscale during decode of an image to desired size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0200-HP"), _L("Downscale during decode of a thumbnail thumbnail to size 1/2; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0205-HP"), _L("Downscale during decode of a thumbnail thumbnail to desired size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0210-HP"), _L("Downscale during decode of an image to desired size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0220-HP"), _L("Downscale during decode of a thumbnail thumbnail to desired size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0230-HP"), _L("Downscale during decode of an image to full size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0240-HP"), _L("Downscale during decode of a thumbnail thumbnail to full size; decode with display mode")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0710-HP"), _L("Downscale during decode of a progressive image to size 1/2")));
    AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-SCAL-I-0720-HP"), _L("Downscale during decode of image with DRI (Define Restart Interoperability) marker to size 1/2")));

    // Multiple extensions during decoding---------------------------------------------
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0010-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0020-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0030-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0040-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0050-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0060-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0070-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0080-HP"), _L("Multiple operations during decode of image; decode with default display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0090-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0100-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0110-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0120-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0130-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0140-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0150-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0160-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0170-HP"), _L("Multiple operations during decode of image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0180-HP"), _L("Multiple operations during decode of thumbnail; decode with default display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0190-HP"), _L("Multiple operations during decode of thumbnail; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0200-HP"), _L("Multiple operations during decode of thumbnail; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0210-HP"), _L("Multiple operations during decode of image, clear operations; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0220-HP"), _L("Multiple operations during decode of image, clear operations; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0230-HP"), _L("Multiple operations during decode of image, clear operations; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0240-HP"), _L("Multiple operations during decode of image, clear operations; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0250-HP"), _L("Multiple operations during decode of image, clear operations, decode the original image; decode with display mode")));
    AddTestStepL(RStepMultipleExtDuringDecode::NewL(_L("MM-ICL-DEC-MUOP-I-0260-HP"), _L("Multiple operations during decode of image, clear operations, decode the original image; decode with display mode")));
    
    // Autorotate orientation along with TImageCoversion operations during decoding
    //only autorotate
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0010-HP"), _L("Autorotate main image during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0020-HP"), _L("Autorotate thumbnail image during decode")));
    // autorotate & other transformations
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0030-HP"), _L("Autorotate then mirror horizontal & vertical during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0040-HP"), _L("Autorotate then mirror horizontal, vertical and rotate 90 degrees during decode")));
    AddTestStepL(RStepRotateOrMirrorDuringDecodeAsYuvPixelBuffer::NewL(_L("MM-ICL-DEC-ARTO-I-0050-HP"), _L("Autorotate then rotate 90 degrees and mirror vertical during decode and output as YUV pixel buffer")));
    // autorotate and crop
	AddTestStepL(RStepCropDuringDecodeInvalidDestSize::NewL(_L("MM-ICL-DEC-ARCR-I-0060-HP"), _L("Autorotate and Crop during decode of image, attempt to output to invalid sized bitmap (size of the output bitmap greater than crop size)")));
	AddTestStepL(RStepCropDuringDecodeAsBitmap::NewL(_L("MM-ICL-DEC-ARCR-I-0070-HP"), _L("Autorotate and Crop during decode of image, cancel the crop, output the original image as a bitmap (CFbsBitmap)")));    
	// autorotate and scale
	AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-ARSC-I-0080-HP"), _L("Autorotate and Downscale during decode of a thumbnail to full size; decode with display mode")));
	AddTestStepL(RStepScaleDuringDecode::NewL(_L("MM-ICL-DEC-ARSC-I-0090-HP"), _L("Autorotate and Downscale during decode of an image to full size; decode with display mode")));	
	// autorotate without exif data
	AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0100-HP"), _L("Autorotate and mirror horizontal, verticala nd rotate 90 degrees input image without exif tags during decode ")));
	// autorotate with invalid orientation
	AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0110-HP"), _L("Autorotate input image with exif tags during decode by setting invalid orientation tag")));
	// autorotate by modifying source orientation with valid tag
	AddTestStepL(RStepRotateOrMirrorDuringDecodeRotateMirror::NewL(_L("MM-ICL-DEC-ARTO-I-0160-HP"), _L("Autorotate input image with exif tags during decode by modifying the source image orientation with valid tag")));
	// RealTime Events: autorotate and other transformations during decode and cancel and restart decoding
	AddTestStepL(RStepRotateOrMirrorDuringDecodeCancel::NewL(_L("MM-ICL-DEC-ARTO-I-0170-HP"), _L("Cancel autorotate, mirror and/or rotate during decode of image, then start again")));
		    
    }
