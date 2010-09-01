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
// This file contains test step class declaration for streamed decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_STREAM_H
#define TSI_ICL_DECODE_STREAM_H

// Test user includes
#include "tsi_icl_decode_stepbase.h"


// Test Step base class for streamed decode tests
class RStepBaseStreamedDecode : public RStepBaseImageDecoder
    {
protected:
    RStepBaseStreamedDecode(const TDesC& aStepName, const TDesC& aTestCaseDescription);

    // Read input from the configuration file
    TInt ReadCommonInputFromConfigFile();

    // Initialize stream encoding
	void InitializeStreamedEncodingL();

	// Appending blocks to destination image
	void StreamedEncoding();

	// Stream encoding complete
	void StreamedEncodingComplete();

    // Initialize stream decoding
    void InitializeStreamedDecodingL();

    // Streamed decoding of a JPEG image to image frame
    void StreamDecodeJpegImageToImageFrameL();

	// Create stream decoder, initialize stream decoding and stream decode the stream to image frame
	void StreamDecodeFilesL();

    // override from RStepBaseImageDecoder
    void DoKickoffTestL();

protected:
    TInt  iMultBlockPosCount;   // to hold the multiple block position count
    TInt  iBlockPosCount;       // to keep track of the blockposition count, same as iMultBlockPosCount
    };

// Stream decoding covering getting all blocks, getting random blocks, getting last block etc
// MM-ICL-DEC-STRM-I-0070-HP, MM-ICL-DEC-STRM-I-0080-HP, MM-ICL-DEC-STRM-I-0085-HP, MM-ICL-DEC-STRM-I-0090-HP, 
// MM-ICL-DEC-STRM-I-0100-HP, MM-ICL-DEC-STRM-I-0105-HP, MM-ICL-DEC-STRM-I-0110-HP, MM-ICL-DEC-STRM-I-0115-HP, 
// MM-ICL-DEC-STRM-I-0120-HP, MM-ICL-DEC-STRM-I-0125-HP, MM-ICL-DEC-STRM-I-0126-HP, MM-ICL-DEC-STRM-I-0130-HP,
// MM-ICL-DEC-STRM-I-0131-HP, MM-ICL-DEC-STRM-I-0132-HP, MM-ICL-DEC-STRM-I-0135-HP, MM-ICL-DEC-STRM-I-0140-HP, 
// MM-ICL-DEC-STRM-I-0150-HP, MM-ICL-DEC-STRM-I-0160-HP, MM-ICL-DEC-STRM-I-0170-HP, MM-ICL-DEC-STRM-I-0180-HP, 
// MM-ICL-DEC-STRM-I-0190-HP, MM-ICL-DEC-STRM-I-0200-HP, MM-ICL-DEC-STRM-I-0205-HP, MM-ICL-DEC-STRM-I-0210-HP,
// MM-ICL-DEC-STRM-I-0215-HP, MM-ICL-DEC-STRM-I-0220-HP, 
// MM-ICL-DEC-STRM-I-0230-HP, MM-ICL-DEC-STRM-I-0280-HP, MM-ICL-DEC-STRM-I-0290-HP, MM-ICL-DEC-STRM-I-0300-HP, 
// MM-ICL-DEC-STRM-I-0310-HP, MM-ICL-DEC-STRM-I-0320-HP, MM-ICL-DEC-STRM-I-0330-HP, MM-ICL-DEC-STRM-I-0340-HP, 
// MM-ICL-DEC-STRM-I-0345-HP, MM-ICL-DEC-STRM-I-0350-HP, MM-ICL-DEC-STRM-I-0352-HP, MM-ICL-DEC-STRM-I-0355-HP, 
// MM-ICL-DEC-STRM-I-0360-HP, MM-ICL-DEC-STRM-I-0365-HP, MM-ICL-DEC-STRM-I-0370-HP, MM-ICL-DEC-STRM-I-0380-HP, 
// MM-ICL-DEC-STRM-I-0385-HP, MM-ICL-DEC-STRM-I-0390-HP, MM-ICL-DEC-STRM-I-0400-HP, MM-ICL-DEC-STRM-I-0410-HP, 
// MM-ICL-DEC-STRM-I-0510-HP
// sequential block from JPEG image to YUV pixel buffer, forward random block from JPEG image to YUV pixel buffer,
// backward random block from JPEG image to YUV pixel buffer, sequential block from JPEG image to YUV buffer in separate thread,
// forward random block from JPEG image to YUV pixel buffer in separate thread, backward random block from JPEG image to YUV pixel buffer in separate thread,
// sequential block from JPEG image to invalid destination YUV format not supported, random block from JPEG image to invalid destination YUV format not supported,
// sequential block from main image in JPEG file which contains EXIF and thumbnail, forward random block from main image in JPEG file which contains EXIF and thumbnail
// backward random block from main image in JPEG file which contains EXIF and thumbnail, Sequential streamed decoding of thumbnail in JPEG image with color sampling 422,
// Sequential streamed decoding of thumbnail in JPEG image error reported,test GetSupportedFormasL, Sequential streamed decoding of thumbnail in JPEG image error reported,test initframeL,
// Random streamed decoding of thumbnail in JPEG image error reported, Progressive JPEG image, error 'Not Supported' reported,
// Progressive JPEG image, progressive flag invalid, Streamed decoding of empty JPEG image, JPEG image when JPEG data is very corrupt and can't be opened,
// a JPEG image during decode when JPEG data has minor corruption so that a distorted image can be displayed, corrupt thumbnail in JPEG file,
// sequential block from JPEG file with colour sample Monochrome, random block from JPEG file with colour sample Monochrome, 
// forward random block from JPEG file with colour sample 4:2:0 (default), backward random block from JPEG file with colour sample 4:2:0 (default),
// sequential block from JPEG file with colour sample 4:2:2, backward random block from JPEG file with colour sample 4:2:2, 
// first sequential block output to a YUV pixel buffer, second sequential block output to a YUV pixel buffer, 
// last sequential block output to a YUV pixel buffer buffer size matches data remaining in JPEG file ,
// last sequential block output to a YUV pixel buffer buffer size greater then data remaining in JPEG file,
// sequential block output to a YUV pixel buffer, start at top left, block taken from single row of JPEG image,
// sequential block output to a YUV pixel buffer, start at top left, blocks taken from two rows of JPEG image,
// first block using forward random access output to a YUV pixel buffer, first block using backward random access output to a YUV pixel buffer,
// any block using forward random access output to a YUV pixel bffer, multiple blocks using forward random access,
// any block using backward random access output to a YUV pixel buffer, last block using forward random access to blocks output to a YUV pixel buffer buffer size matches data remaining in JPEG file
// last block using backward random access to output to a YUV pixel buffer, size matches data remaining in JPEG file,
// last block using forward random access, output to a YUV pixel buffer size greater then data remaining in JPEG file,
// block using forward random access to blocks output to a YUV pixel buffer, block from within a single row,
// block using forward random access output to a YUV pixel buffer, block from with multiple rows,
// multiple blocks using forward and backward random access, largest possible JPEG, smallest possible JPEG,
// Confirm that there is no loss of image quality between stream decode and decode JPEG image
class RStepStreamedDecodeToImage : public RStepBaseStreamedDecode
	{
public:
    static RStepStreamedDecodeToImage* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
	RStepStreamedDecodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};


// Stream decoding test cases to verfiy the support for downscale 
// MM-ICL-DEC-STRM-I-0240-HP, MM-ICL-DEC-STRM-I-0250-HP, MM-ICL-DEC-STRM-I-0260-HP,
// MM-ICL-DEC-STRM-I-0270-HP
// Stream decode of JPEG image, downscale to size ½,
// Stream decode of JPEG image, downscale to size ¼,
// Stream decode of JPEG image, downscale to size 1/8,
// Stream decode of JPEG image, downscale to size smaller than 1/8
class RStepStreamedDecodeToImageScale : public RStepBaseStreamedDecode
	{
public:
    static RStepStreamedDecodeToImageScale* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
	RStepStreamedDecodeToImageScale(const TDesC& aStepName, const TDesC& aTestCaseDescription);
	};


// Stream decoding test cases for performance measurement
// MM-ICL-DEC-STRM-I-0420-HP, MM-ICL-DEC-STRM-I-0430-HP, MM-ICL-DEC-STRM-I-0440-HP,
// MM-ICL-DEC-STRM-I-0450-HP
// Record Time taken and memory usage required to stream decode from small image,
// Record Time taken and memory usage required to stream decode average size JPEG image,
// Record Time taken and memory usage required to stream decode large JPEG image,
// Record Time taken and memory usage required to stream decode super-size JPEG image
class RStepStreamedDecodeToImagePerf : public RStepBaseStreamedDecode
	{
public:
    static RStepStreamedDecodeToImagePerf* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

private:
	RStepStreamedDecodeToImagePerf(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
	};


// Stream Decoding test cases for Realtime events
// MM-ICL-DEC-STRM-I-0470-HP, MM-ICL-DEC-STRM-I-0480-HP, MM-ICL-DEC-STRM-I-0490-HP,
// MM-ICL-DEC-STRM-I-0500-HP
// Cancel sequential stream decode of JPEG image,
// Cancel sequential stream decode of JPEG image then start again,
// Cancel sequential stream decode of JPEG image, which is run in a separate thread,
// Cancel sequential stream decode of JPEG image, which is run in a separate thread. Then start again
class RStepStreamedDecodeToImageRealTimeTests : public RStepBaseStreamedDecode, public MTimerObserver
	{
public:
    static RStepStreamedDecodeToImageRealTimeTests* NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    void FsmL();

    // from MTimerObserver
	void TimerExpired();

private:
	RStepStreamedDecodeToImageRealTimeTests(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    TInt ReadTestStepInputFromConfigFile();
	};

#endif // TSI_ICL_DECODE_STREAM_H
