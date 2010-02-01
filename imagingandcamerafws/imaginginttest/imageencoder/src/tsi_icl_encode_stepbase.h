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
// This file contains test step base class declaration for image encoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


#ifndef TSI_ICL_ENCODE_STEPBASE_H
#define TSI_ICL_ENCODE_STEPBASE_H

// EPOC includes
#include <hal.h>
#include <dptest.h>                     // to check which demand paging is in use
#include <iclexif.h>
#include <iclexifimageframe.h>
#include <imagetransform.h>
#include <imageconversion.h>
#include <bitmaptransforms.h>
#include "ImageUtils.h"

// Test system includes
#include <testframework.h>


//  --- Color Sampling followed within the code ---
//	0 - EMonochrome
//	1 - EColor420
//	2 - EColor422
//	3 - EColor444


// Constants
const TInt KSectionNameLen = 7;         // last seven characters of the test case number is the name of the section in the ini file
                                        // example, in MM-ICL-ENC-STRM-I-xxxx-xP, last 7 chars define name of the section fully
const TInt KBufferLen = 256;
const TInt KSmallBufferLen = 33;
const TInt KDefaultSampling = 1;
const TInt KDefaultQualityFactor = 85;

_LIT8(KMimeTypeJpeg, "image/jpeg");
_LIT8(KMimeTypeJpg, "image/jpg");

const TUid KImageFramePluginUid = {0x101F7C60};
const TUid KICLExtensionUidValue = {0x102833D8};
const TInt KChunkDataOffset = 0;        //the data offset from where the image frame starts

// Check the status of encoding/transformations and if error, log the error msg and leave with the error
#define CHECK_STATUS_LEAVE_IF_ERROR(err, msg) \
                           if (err != KErrNone) \
                               { \
                               TBuf<KBufferLen> errMsg(msg); \
                               errMsg.Append(_L(", error = %d")); \
                               ERR_PRINTF2(errMsg, err); \
                               User::Leave(err); \
                               }

// forward declaration
class RStepBaseImageEncoder;

// for handling asynchronous events
class CActiveListener : public CActive
    {
public:
    CActiveListener(RStepBaseImageEncoder* aBase);
    void InitialiseActiveListener();

    // from CActive
    virtual void RunL();
    virtual void DoCancel();

public:
    RStepBaseImageEncoder* iBase;
    };


// MTimerObserver: mixin class to handle callbacks from a CUtilityTimer object
class MTimerObserver
	{
public:
	virtual void TimerExpired() = 0;
	};

// CUtilityTimer
class CUtilityTimer : public CTimer
	{
public:
	static CUtilityTimer* NewL(TTimeIntervalMicroSeconds32 aDelay, MTimerObserver& aTimerObserver);
	~CUtilityTimer();

	void InitializeTimer();

private:
    CUtilityTimer(MTimerObserver& aTimerObserver);
    void ConstructL(TTimeIntervalMicroSeconds32& aDelay);

	// from CActive
    void RunL();
	void DoCancel();

private:
    TTimeIntervalMicroSeconds32 iTimerDelay;
	MTimerObserver& iTimerObserver;
    };


// Test Step super base class for all derived test steps
class RStepBaseImageEncoder : public RAsyncTestStep
    {
public:
    // Specifies the underlying hardware under test
    enum TDriveType
        {
        EDstMediaRamDrive = 0,
        EDstMediaMMCDrive = 1
        };
        
    // FsmL() implementation based on the state
    virtual void FsmL() { };
    virtual void TimerExpired() { };
    // Read common input from the configuration file
    virtual TInt ReadCommonInputFromConfigFile() { return KErrNone;};

protected:
    RStepBaseImageEncoder(const TDesC& aStepName, const TDesC& aTestCaseDescription);

 	// Comparing the output bitmap with the refernce bitmap byte by byte
	void CompareFileL();

    // Check performance based on time and log the result; also, compare and save the output
    void CheckPerformanceOutputAndSaveL();

    // Get scale execution time according to the current hardware. Currently supported: H2, H4 and emulator
    TInt64 ScaleTimeByMachineL(TInt64 aExecTime, TInt aDestMedia = EDstMediaRamDrive);

	TInt GetEncodeOptions();

    // Get actual rotation/mirroring/flip values, from the values read from config file
    TInt GetMirrorAngle(CBitmapRotator::TRotationAngle& aAngle);

 	TFileName& DestFilename();

 	// Create output directory 
 	TInt CreateOutputDirectory();

    // Retain the final output of the encoding based on iHwRetainOutputImage flag
    void RetainEncodedImage();

	// Change the state to next
    void StateChange(); 

	// Add color sampling and quality factor to the image
    void PrepareEncodeDataL();
	
	// Add color sampling and quality factor to the image for streamed decoding
	void PrepareDecodeDataL();

    // Add extension interface operations to encoder
    void AddOperationL();
    
    // Create image decoder
    void CreateImageDecoderL();
    
    // Create image frame Decoder
    void CreateImageFrameDecoderL();
    
    // Create image frame Encoder
    void CreateImageFrameEncoderL();
    
	// Create image decoder
    void CreateImageEncoderL();
    
    // Set the thread for encoding - client OR separate thread; can be combined with other options
    void SetThreadForEncoding(CImageEncoder::TOptions& aOptions);

	//get the imagetypeuid based the input file extension
	TUid ImageType();
	
	// Set Fun: EXIF metadata setters
	TInt SetExifAndThumbnailData(MExifMetadata* aMetadata);
	
	// Set YCbCrPositioning and ColorSpace tag of the EXIF data
	TInt SetShort(MExifMetadata* aMetaData);
	
	// Set user comment tag of the EXIF data
	TInt Set16bitBuffer(MExifMetadata* aMetaData);
	
	// Set model version tag of the exif data
	TInt SetModelVersion(MExifMetadata* aMetaData);	

    //To log the test case description
    void LogTestCaseDescription();

	// Start the test
    virtual void KickoffTestL();            // from RAsyncTestStep
    
    // Cleanup at the end
    virtual void CloseTest();
    
    // derived test step class should implement
    virtual void DoKickoffTestL() = 0;

protected:
    TBuf<KSmallBufferLen> iSectionName;
    TBuf<KBufferLen> iTestCaseDescription;
    
    CImageEncoder*          iImageEncoder;
    CImageDecoder*          iImageDecoder;
    
    CJPEGImageFrameDecoder* iJPEGImageDecoder;
    CJPEGImageFrameEncoder* iJPEGImageEncoder;
    
    CJPEGExifEncoder*       iExifEncoder;
	MExifMetadata*          iExifMetaData;

    CActiveListener* iActiveListener;       // for handling asynchronous events
	CUtilityTimer*   iTimer;
    RFs iFs;

	RChunk              iChunk;             // used for decoding the image to YUV pixel buffer
    CImageFrame* 		iImageFrame;
	CFrameImageData*	iFrameEnImageData;
	CFrameImageData*	iFrameDeImageData;
    TImageConvStreamedEncode* iStreamEncode;// used for stream encoding to append stream of data
    TImageConvStreamedDecode* iStreamDecode;// used for stream decoding to get stream of data

	CFbsBitmap*     iSrcBitmap;
	CFbsBitmap*     iRefBitmap;
	CBitmapRotator* iBmpRotator;
	

	CImageEncoder::TOptions         iOptions;
	CBitmapRotator::TRotationAngle  iMirrorAxis;
	CBitmapRotator::TRotationAngle  iRotationAngle;

    // state of the test; when asynchronous events are completed, 
    // with or without error, 
    // next state is called and the error is handled
    TInt        	iState;
    // to hold the data read from configuration file
    TBool       	iHwRetainOutputImage;       // if true(1) -> output image will be saved, if false(0) -> O/P image will not be saved it will be deleted This is for the purpose of h4 board memory limitation
	TBool       	iWriteThumbnail;            // if true(1) -> set thumbnail while encoding
	TBool       	iWriteExif;                 // if true(1) -> set exif data while encoding
    TInt        	iOptionsIndex;
    TInt        	iMirroringIndex;            //Mirroraxis index
    TBool 			iBitmaploaded;              // ETrue if bitmap is already loaded while mirror or rotate operation
    TInt        	iMirrorIndex1;              // When we want to perform rotate, mirror horizontal & vertical then encoding we use these 2 indexes
	TInt        	iMirrorIndex2;
    TInt 			iIndex;                     // used to indicate the CBitmapRotator operation
    TInt        	iRotationIndex;             // Rotateangle index
    TInt        	iImplementationUidIndex;
	TInt        	iOperationIndex;            // to hold the extension interface operation index
	TBool       	iConvCancelTest;            // if true(1) -> Cancel test cases
	TInt        	iInputImageCount;           //indicates the no of files to read for the streamed encoding
	RUidDataArray 	iPropertiesArray;   		// plugin properties supported by implementation uid
	RArray<TPtrC> 	iInputImageArray;       	// to hold the inputfiles for streamed encoding in an array
	TTime       	iStartTime, iEndTime;       // Start and endtime of encoding
    TInt        	iExpectedTime;              // expected encoding time in microseconds for performance measurement

    TFileName   	iInputImage;                // input image to be encoded
	TFileName   	iReferenceImage;            // to compare encoded bitmap with the reference file
    TFileName   	iOutputImage;               // to save encoded output bitmap to a file
	RFile       	iFile;
		
    TBool      		iAlwaysThread;              // this indicates whether the process is working in seperate thread or client thread
	TUid        	iImageTypeUid;              // image type UID

	TPtrC       	iOutputPath;                // path to save the destination file

	TUint32     	iBeginEncodeCounter;        // Used to save the start of the counter to check how much encoding ahs happened
	TUint32     	iEndEncodeCounter;          // Used to save the end of the counter before cancel or stop to check how much encoding ahs happened
	TSize 			iBlockSizeInPixels;         // used for streamed encoding
    
    //Streaming decoding and encoding		
	TInt        	iIterateblocks;             //To get the num of blocks to add and provide streaming effect thru loop
	TInt			iNumBlocksToAdd;            // To hold num of blocks to be added for each iteration
	TInt 			iTotalblocks;               // To hold the totalblocks the image is having
	TInt 			iImageWidth;                // framesize width
	TInt			iImageHeight;               // framesize height
	TInt			iOPImageWidth;              // To hold output image height and width while appending many images to single output image
	TInt			iOPImageHeight;
	TBool			iMidStream;                 // if 1 append block midstream else if 0 append block start
	TUid 			iOptimalFormat;             // YUV support for colorsampling 420, 422, monochrome
	TFrameInfo  	iFrameInfo;                 // streamed decoder frameinfo
	TInt        	iNumBlocksToGet;            // no of blocks to get from stream decoding
	TInt        	iFrameCount;                // no of frames decoded while streaming
	TInt        	iNumBlocksRead;             // no of blocks to get from stream decoding
	TInt        	iStreammode;                // Stream mode 1= Sequential,2=random forward, 3= random backward
	TBool       	iHaveMoreBlocks;            // while stream decoding iHaveMoreBlocks= 1, then blocks still remiaining, if 0 no blocks left
	HBufC8*     	iBuffer;
    // ClearOpStack Datamembersfrom config file
    TBool       	iClearOperations;           // if true (1)->clear the addoperation stack, if false(0)-> don't clear the stack
    TBool       	iClearedstack;              // set true (1)-> if stack cleared, set false(0)-> if stack not cleared
    TBool       	iReapplyOperations;         // if 1=> reapply the operations(same or different) during encoding
    TInt        	iOperationIndex2;           // reapply the operations specified in the index
    
	// Data types for mirror during encoding
	TBuf8<KSmallBufferLen> iMimeType;                  // maximum size of the buffer
	TBool       	iConvCancelled;             // set to true if encode conversion of is cancelled
	TBool       	iRestartConvAfterCancel;    // indicates if conversion is to restart if it had been cancelled
	TTimeIntervalMicroSeconds32 iTimerExpire;// To hold the time, when to call cancel while encoding
    };
#endif // TSI_ICL_ENCODE_STEPBASE_H
