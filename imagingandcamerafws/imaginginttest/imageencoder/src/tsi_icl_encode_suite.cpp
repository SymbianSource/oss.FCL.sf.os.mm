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
// This file contains test suite class implementation for image encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


// Test user includes
#include "tsi_icl_encode_suite.h"
#include "tsi_icl_encode_stepbase.h"
#include "tsi_icl_encode_extinterface.h"
#include "tsi_icl_encode_mirror.h"
#include "tsi_icl_encode_stream.h"
#include "tsi_icl_encode_perf.h"


// Create an instance of this test suite, exported at ordinal 1.
// This provides the interface to allow the Test Framework
// to create instances of this test suite.
EXPORT_C CTestSuiteImageEncoder* NewTestSuiteL()
    {
    return CTestSuiteImageEncoder::NewL();
    }

CTestSuiteImageEncoder* CTestSuiteImageEncoder::NewL()
    {
    CTestSuiteImageEncoder* self = new (ELeave) CTestSuiteImageEncoder;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CTestSuiteImageEncoder::~CTestSuiteImageEncoder()
    {
    }

CTestSuiteImageEncoder::CTestSuiteImageEncoder()
    {
    // Name of this test suite
    iSuiteName = _L("tsi_icl_imageencoder_suite");
    }

// Get version of the suite
TPtrC CTestSuiteImageEncoder::GetVersion() const
    {
    _LIT(KSuiteVersion, "v0.7");
    return KSuiteVersion();
    }

// This creates all the test steps and stores them inside CTestSuiteImageEncoder
void CTestSuiteImageEncoder::InitialiseL()
    {
    // Section Name is [xxxx-xP] in the configuration file

    // Baseline Performance----------------------------------------------------
	// Encode image
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0010-HP"),_L("Record time taken to encode very small image to JPEG")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0020-HP"),_L("Record time taken to encode small image to JPEG")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0030-HP"),_L("Record time taken to encode average image to JPEG")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0035-HP"),_L("Record time taken to encode image to JPEG without EXIF or Thumbnail")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0036-HP"),_L("Record time taken to encode image to JPEG with EXIF ")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0037-HP"),_L("Record time taken to encode image to JPEG with EXIF and thumbnail")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0040-HP"),_L("Record time taken to encode small image to JPEG at quality 25")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0050-HP"),_L("Record time taken to encode small image to JPEG at quality 75")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0060-HP"),_L("Record time taken to encode small image to JPEG at quality 90")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0070-HP"),_L("Record time taken to encode small image to JPEG at quality 95")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0080-HP"),_L("Record time taken to encode small image to JPEG with color sample Monochrome")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0085-HP"),_L("Record time taken to encode small image to JPEG with color sample 4:2:0")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0090-HP"),_L("Record time taken to encode small image to JPEG with color sample 4:2:2")));
	AddTestStepL(RStepEncodeImageWithExifdata::NewL(_L("MM-ICL-ENC-PERF-I-0095-HP"),_L("Record time taken to encode small image to JPEG with colour sample 4:4:4")));
 	// Mirror bitmap then encode
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0600-HP"),_L("Record time taken to mirror over the vertical axis a very small image Bitmap then encode as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0610-HP"),_L("Record time taken to mirror over the vertical axis a small image Bitmap then encode image as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0620-HP"),_L("Record time taken to mirror over a vertical axis an average image Bitmap then encode image as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0630-HP"),_L("Record time taken to mirror over the horizontal axis then encode very small image as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0640-HP"),_L("Record time taken to mirror over the horizontal axis then encode small image as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0650-HP"),_L("Record time taken to mirror over the horizontal axis then encode average image as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0655-HP"),_L("Record time taken to mirror over the vertical axis then encode image to JPEG without EXIF or Thumbnail")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0656-HP"),_L("Record time taken to mirror over the horizontal axis then encode image to JPEG with EXIF ")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0657-HP"),_L("Record time taken to mirror over the vertical axis then encode image to JPEG with EXIF and thumbnail")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0660-HP"),_L("Record time taken to mirror over the vertical axis then encode small image as JPEG at quality level 25")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0670-HP"),_L("Record time taken to mirror over the horizontal axis then encode small image as JPEG at quality level 75")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0680-HP"),_L("Record time taken to mirror over the vertical axis then encode small image as JPEG at quality level 90")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0690-HP"),_L("Record time taken to mirror over the horizontal axis then encode small image as JPEG at quality level 95")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0691-HP"),_L("Record time taken to mirror over the vertical axis then encode small image as JPEG with colour sample Monochrome")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0692-HP"),_L("Record time taken to mirror over the horizontal axis then encode small image as JPEG with colour sample 4:2:0 (default)")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0693-HP"),_L("Record time taken to mirror over the vertical axis then encode small image as JPEG with colour sample 4:2:2")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0694-HP"),_L("Record time taken to mirror over the horizontal axis then encode small image as JPEG with colour sample 4:4:4")));

 	// Rotate and/or Mirror bitmap then encode
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0800-HP"),_L("Record time taken to rotate 90 degrees clockwise a small image then encode as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0810-HP"),_L("Record time taken to rotate 180 degrees clockwise a small image then encode as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0820-HP"),_L("Record time taken to rotate 270 degrees clockwise a small image  then encode as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0830-HP"),_L("Record time taken to rotate 90 degrees clockwise an average image then encode as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0840-HP"),_L("Record time taken to rotate 180 degrees clockwise an average image then encode as JPEG")));
	AddTestStepL(RStepMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0850-HP"),_L("Record time taken to rotate 270 degrees clockwise an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0860-HP"),_L("Record time taken to rotate 90 degrees clockwise and mirror about the horizontal axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0870-HP"),_L("Record time taken to rotate 180 degrees clockwise and mirror about the horizontal axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0880-HP"),_L("Record time taken to rotate 270 degrees clockwise and mirror about the horizontal axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0890-HP"),_L("Record time taken to rotate 90 degrees clockwise and mirror about the horizontal axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0900-HP"),_L("Record time taken to rotate 180 degrees clockwise and mirror about the horizontal axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0910-HP"),_L("Record time taken to rotate 270 degrees clockwise and mirror about the horizontal axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0920-HP"),_L("Record time taken to rotate 90 degrees clockwise and mirror about the vertical axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0930-HP"),_L("Record time taken to rotate 180 degrees clockwise and mirror about the vertical axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0940-HP"),_L("Record time taken to rotate 270 degrees clockwise and mirror about the vertical axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0950-HP"),_L("Record time taken to rotate 90 degrees clockwise and mirror about the vertical axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0960-HP"),_L("Record time taken to rotate 180 degrees clockwise and mirror about the vertical axis an average image Bitmap then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0970-HP"),_L("Record time taken to rotate 270 degrees clockwise and mirror about the vertical axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0980-HP"),_L("Record time taken to rotate 90 degrees clockwise and mirror about the vertical and horizontal axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-0990-HP"),_L("Record time taken to rotate 180 degrees clockwise and mirror about the vertical and horizontal axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-1000-HP"),_L("Record time taken to rotate 270 degrees clockwise and mirror about the vertical and horizontal axis a small image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-1010-HP"),_L("Record time taken to rotate 90 degrees clockwise and mirror about the vertical and horizontal axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-1020-HP"),_L("Record time taken to rotate 180 degrees clockwise and mirror about the vertical and horizontal axis an average image then encode as JPEG")));
	AddTestStepL(RStepRotateMirrorThenEncodeToImage::NewL(_L("MM-ICL-ENC-PERF-I-1030-HP"),_L("Record time taken to rotate 270 degrees clockwise and mirror about the vertical and horizontal axis an average image then encode as JPEG")));
	
    // Streamed Encoding-------------------------------------------------------
    //Extension Interface - Codec support for Streamed encoding
    AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-STRM-I-0010-HP"), _L("Confirm whether the codec supports extension interface during encoding using plug-in properties"))); 
    AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-STRM-I-0020-HP"), _L("Confirm that 'plug-in properties' correctly reports it does not support streamed encoding with thumbnail of JPEG image")));
    AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-STRM-I-0030-HP"), _L("Confirm that 'plug-in properties' correctly reports it does not support streamed encoding with rotation of JPEG image")));
    AddTestStepL(RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(_L("MM-ICL-ENC-STRM-I-0040-HP"), _L("Confirm whether the codec supports streamed during encoding by loading the Codec")));
    AddTestStepL(RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(_L("MM-ICL-ENC-STRM-I-0050-HP"), _L("Confirm whether the codec supports streamed encoding and rotation during encoding by loading the Codec")));
    AddTestStepL(RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(_L("MM-ICL-ENC-STRM-I-0060-HP"), _L("Confirm whether the codec supports streamed encoding and mirror horizontal during encoding by loading the Codec")));
    // Source and Destination
	AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0080-HP"),_L("Streamed encoding of JPEG image from a   YUV pixel buffer ")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0100-HP"),_L("Streamed encoding of JPEG image from a   YUV pixel buffer, in a separate thread ")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0110-HP"),_L("Attempt streamed encoding of JPEG image from a YUV pixel buffer, in a format not supported")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0120-HP"),_L("Streamed encoding of JPEG image from a YUV pixel buffer, send invalid block mid-stream with unsupported YUV format")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0130-HP"),_L("Streamed encoding of JPEG image, invalid source, block size to large")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0140-HP"),_L("Streamed encoding of JPEG image, invalid source mid stream, block size to large")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0150-HP"),_L("Streamed encoding of JPEG image, invalid source, block size to small")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0160-HP"),_L("Streamed encoding of JPEG image, invalid source mid stream, block size to small")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0170-HP"),_L("Streamed encoding of JPEG image, number of blocks does not match actual size input data ")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0180-HP"),_L("Streamed encoding of JPEG image, mid stream, number of blocks does not match actual size input data")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0190-HP"),_L("Streamed encoding of JPEG image with empty YUV buffer empty")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0200-HP"),_L("Streamed encoding of JPEG image with empty YUV buffer, mid stream")));
    AddTestStepL(RStepStreamedEncodeToImageNegTest::NewL(_L("MM-ICL-ENC-STRM-I-0201-HP"),_L("Streamed encoding of JPEG image with attempted transcode from mono to 420")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0220-HP"),_L("Streamed encoding of JPEG image, smallest possible source")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0225-HP"),_L("Streamed encoding of multiscan JPEG image")));
    // Input Stream
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0230-HP"),_L("Streamed encoding of JPEG image with pixel block starting at top left and sequence left to right")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0240-HP"),_L("Streamed encoding of JPEG image with blocks of pixels starting at top left and sequence left to right then top to bottom")));
    // EXIF Data and Thumbnail
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0270-HP"),_L("Stream encoding of JPEG image without EXIF or Thumbnail (default)")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0280-HP"),_L("Stream encoding of JPEG image with EXIF ")));
    // Colour Sampling (Block Size)
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0300-HP"),_L("Streamed encoding of JPEG image with colour sample Monochrome")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0310-HP"),_L("Streamed encoding of JPEG image with colour sample 4:2:0 (default)")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0320-HP"),_L("Streamed encoding of JPEG image with colour sample 4:2:2")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0330-HP"),_L("Streamed encoding of JPEG image with colour sample 4:4:4")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0340-HP"),_L("Stream encode JPEG image with quality level 25")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0350-HP"),_L("Stream encode JPEG image with quality level 75")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0360-HP"),_L("Stream encode JPEG image with quality level 90")));
    AddTestStepL(RStepStreamedEncodeToImage::NewL(_L("MM-ICL-ENC-STRM-I-0370-HP"),_L("Record time taken to stream encode JPEG image with quality level 95")));
    // Real-Time Events
    AddTestStepL(RStepStreamedEncodeToImageRealTimeTests::NewL(_L("MM-ICL-ENC-STRM-I-0430-HP"),_L("Cancel during streamed encoding of JPEG image")));
    AddTestStepL(RStepStreamedEncodeToImageRealTimeTests::NewL(_L("MM-ICL-ENC-STRM-I-0440-HP"),_L("Cancel during streamed encoding of JPEG image then start again")));
    AddTestStepL(RStepStreamedEncodeToImageRealTimeTests::NewL(_L("MM-ICL-ENC-STRM-I-0450-HP"),_L("Cancel during streamed encoding of JPEG image, which is run in a separate thread")));

    // Mirroring or flipping during encoding-----------------------------------
    // Extension Interface - Codec support for Mirror during encoding
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0010-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0020-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0030-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0040-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0050-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0060-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0070-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0080-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	AddTestStepL(RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(_L("MM-ICL-ENC-RTMR-I-0085-HP"),_L("Confirm whether the codec supports extension interface during encoding using plug-in properties")));
	
	AddTestStepL(RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(_L("MM-ICL-ENC-RTMR-I-0090-HP"),_L("Confirm whether the codec supports extension interface during encoding by loading the Codec")));
	AddTestStepL(RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(_L("MM-ICL-ENC-RTMR-I-0100-HP"),_L("Confirm whether the codec supports extension interface during encoding by loading the Codec")));
	AddTestStepL(RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(_L("MM-ICL-ENC-RTMR-I-0110-HP"),_L("Confirm whether the codec supports extension interface during encoding by loading the Codec")));
	// Source and Destination
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0120-HP"),_L("Mirror during encoding of JPEG image, from a RGB bitmap source")));
    AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0140-HP"),_L("Mirror during encoding of JPEG image, from a RGB bitmap source, in a separate thread ")));
	// YUV Pixel buffer
    AddTestStepL(RStepMirrorDuringEncodeYUVPixelBuffer::NewL(_L("MM-ICL-ENC-RTMR-I-0130-HP"),_L("Mirror during encoding of JPEG image, from a YUV pixel buffer ")));
    AddTestStepL(RStepMirrorDuringEncodeYUVPixelBuffer::NewL(_L("MM-ICL-ENC-RTMR-I-0150-HP"),_L("Mirror during encoding of JPEG image, from a YUV pixel buffer, in a separate thread ")));
    AddTestStepL(RStepMirrorDuringEncodeYUVPixelBuffer::NewL(_L("MM-ICL-ENC-RTMR-I-0160-HP"),_L("Mirror during encoding of JPEG image, from a YUV pixel buffer in a format not supported (4:4:4) ")));
    AddTestStepL(RStepMirrorDuringEncodeYUVPixelBuffer::NewL(_L("MM-ICL-ENC-RTMR-I-0165-HP"),_L("Rotate and mirror during encode of multiscan image, from YUV pixel buffer rotate 270 and mirror horizontal & vertical axis")));
    // Exif and Thumbnail
    AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0170-HP"),_L("Mirror during encoding of JPEG image without EXIF or Thumbnail (default)")));
    AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0180-HP"),_L("Mirror during encoding of JPEG image with EXIF and thumbnail")));
    AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0660-HP"),_L("Rotate and mirror during encode of JPEG Image with EXIF and thumbnail, and mirror about the horizontal and vertical axis")));
    // Mirror Actions
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0190-HP"),_L("Mirror during encode of JPEG Image, mirror about the vertical axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0200-HP"),_L("Mirror during encode of JPEG Image, mirror about the horizontal axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0210-HP"),_L("Mirror during encode of JPEG Image, mirror about the horizontal and vertical axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0220-HP"),_L("Mirror during encode of JPEG Image with single row of pixels and colour sample 4:2:0 (default) selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0230-HP"),_L("Mirror during encode of JPEG Image with single column of pixels and colour sample 4:2:0 (default) selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0240-HP"),_L("Mirror during encode of JPEG Image with block (16) row of pixels with colour sample 4:2:0 (default), mirror about the horizontal axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0250-HP"),_L("Mirror during encode of JPEG Image with block (16) column of pixels, with colour sample 4:2:0 (default), mirror over the vertical axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0260-HP"),_L("Mirror during encode of JPEG Image with block  (16) - 2 row of pixels and colour sample Monochrome selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0270-HP"),_L("Mirror during encode of JPEG Image with block (16) + 1 column of pixels and colour sample Monochrome selected ")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0280-HP"),_L("Mirror during encode of JPEG Image with block (16) row of pixels with colour sample 4:2:2, mirror about the horizontal axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0290-HP"),_L("Mirror during encode of JPEG Image with block (8) column of pixels with colour sample 4:2:2 selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0300-HP"),_L("Mirror during encode of JPEG Image with block (16) - 2 row of pixels and colour sample 4:2:2 selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0310-HP"),_L("Mirror during encode of JPEG Image with block (8) - 2 column of pixels and colour sample 4:2:2  selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0320-HP"),_L("Mirror during encode of JPEG Image with block (8) row of pixels with colour sample 4:4:4, mirror about the horizontal axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0330-HP"),_L("Mirror during encode of JPEG Image with block (8) column of pixels with colour sample 4:4:4, mirror about the vertical axis")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0340-HP"),_L("Mirror during encode of JPEG Image with block (8) + 1 row of pixels and colour sample 4:4:4 selected")));
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0350-HP"),_L("Mirror during encode of JPEG Image with block (8) - 2 column of pixels and colour sample 4:4:4  selected")));
	// Performance
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0360-HP"),_L("Record time taken to mirror over the vertical axis during encode from very small bitmap to JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0370-HP"),_L("Record time taken to mirror over the vertical axis during encode from small size bitmap to JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0380-HP"),_L("Record time taken to mirror the vertical axis during encode from an average bitmap to JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0390-HP"),_L("Record time taken to mirror over the horizontal axis during encode from very small bitmap to JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0400-HP"),_L("Record time taken to mirror over the horizontal axis during encode from small size bitmap to JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0410-HP"),_L("Record time taken to mirror the horizontal axis during encode from an average bitmap to JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0420-HP"),_L("Record time taken to Mirror over the vertical axis during encoding of small JPEG Image to quality level 25")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0430-HP"),_L("Record time taken to Mirror over the horizontal axis during encoding of small JPEG Image to quality level 75")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0440-HP"),_L("Record time taken to Mirror over the vertical axis during encoding of small JPEG Image to quality level 90")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0450-HP"),_L("Record time taken to Mirror over the horizontal axis during encoding of small JPEG Image to quality level 95")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0460-HP"),_L("Record time taken to Mirror over the vertical axis during encoding of small JPEG Image with colour sample Monochrome")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0470-HP"),_L("Record time taken to Mirror over the horizontal axis during encoding of small JPEG Image with colour sample 4:2:0 (default)")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0480-HP"),_L("Record time taken to Mirror over the vertical axis during encoding of small JPEG Image with colour sample 4:2:2")));
	AddTestStepL(RStepMirrorDuringEncodeToImagePerf::NewL(_L("MM-ICL-ENC-RTMR-I-0490-HP"),_L("Record time taken to Mirror over the horizontal axis during encoding of small JPEG Image with colour sample 4:4:4")));
	// Real-Time Events
	AddTestStepL(RStepMirrorDuringEncodeToImageRealTimeTests::NewL(_L("MM-ICL-ENC-RTMR-I-0500-HP"),_L("Cancel mirror during encode of JPEG image")));
	AddTestStepL(RStepMirrorDuringEncodeToImageRealTimeTests::NewL(_L("MM-ICL-ENC-RTMR-I-0510-HP"),_L("Cancel mirror during encode of JPEG then perform mirror during encode")));
	// Image Quality
	AddTestStepL(RStepMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0530-HP"),_L("Confirm that there is no loss of image quality after mirror during encode ")));
	// Rotation and Mirror during encoding
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0540-HP"),_L("Rotate during encode of JPEG Image, rotate by 90 degrees ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0550-HP"),_L("Rotate during encode of JPEG Image, rotate by 180 degrees ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0560-HP"),_L("Rotate during encode of JPEG Image, rotate by 270 degrees ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0570-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 90 degrees and mirror about the vertical axis ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0580-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 180 degrees and mirror about the vertical axis ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0590-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 270 degrees and mirror about the vertical axis ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0600-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 90 degrees and mirror about the horizontal axis ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0610-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 180 degrees and mirror about the horizontal axis ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0620-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 270 degrees and mirror about the horizontal axis ")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0625-HP"),_L("Rotate and mirror during encode of multiscan image, rotate 270 degrees and mirror about the horizontal & verical axis")));
    AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0630-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 90 degrees and mirror about the horizontal and vertical axis")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0640-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 180 degrees and mirror about the horizontal and vertical axis")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0650-HP"),_L("Rotate and mirror during encode of JPEG Image, rotate 270 degrees and mirror about the horizontal and vertical axis")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0670-HP"),_L("Rotate during encode of JPEG Image with DRI (Define Restart Interoperability) marker, rotate 90 & 180 degrees")));
	AddTestStepL(RStepRotateMirrorDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0680-HP"),_L("Mirror during encode of JPEG Image with DRI (Define Restart Interoperability) marker, mirror about the horizontal and vertical axis")));
	//Clear Operation Stack
	AddTestStepL(RStepRotateMirrorClearOpStackDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0655-HP"),_L("Rotate 270 degrees & mirror horizontal & vertical axis with ClearOpStack ")));
	AddTestStepL(RStepRotateMirrorClearOpStackDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0656-HP"),_L("Rotate 270 degrees & mirror horizontal & vertical axis with ClearOpStack & reapply the same operations during encoding")));
	AddTestStepL(RStepRotateMirrorClearOpStackDuringEncodeToImage::NewL(_L("MM-ICL-ENC-RTMR-I-0657-HP"),_L("Rotate 270 degrees & mirror horizontal & vertical axis with ClearOpStack & reapply the diff operations")));
    }
