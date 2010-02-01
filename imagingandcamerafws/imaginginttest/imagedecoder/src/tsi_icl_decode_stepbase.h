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
// This file contains test step base class declaration for image decoding integration tests.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


#ifndef TSI_ICL_DECODE_STEPBASE_H
#define TSI_ICL_DECODE_STEPBASE_H


// EPOC includes
#include <hal.h>
#include <dptest.h> // to check which demand paging is in use
#include <e32cmn.h>
#include <e32base.h>
#include "imagetransform.h"
#include <imageconversion.h>
#include <bitmaptransforms.h>
#include <iclexifimageframe.h>
#include <icl/imageconversionextension.h>
#include <mm/mmcaf.h>

// Test system includes
#include <testframework.h>


// Constants
const TInt KSectionNameLen = 7;         // last seven characters of the test case number is the name of the section in the configuration file
                                        // example, in MM-ICL-DEC-CROP-I-xxxx-xP, last 7 chars define name of the section fully
const TInt KBufferLen = 256;
const TInt KSmallBufferLen = 33;

const TInt KFrameZero = 0;
const TInt KChunkDataOffset = 0;

const TUid KICLExtensionUidValue = {0x102833D8};
const TUid KUidJPGAutoRotateSupportValue = {0x10287049};
const TUid KImageFramePluginUid  = {0x101F7C60}; // #define KUidICLJpegImageFrameInterface 	0x101F7C60

// Stream modes
const TInt KSequentialStreamDecode = 1;
const TInt KRandomForwardStreamDecode = 2;
const TInt KRandomBackwardStreamDecode = 3;

// Forms for specifying cropping coordinates
const TInt KCroppingFormPointAAndB = 1;
const TInt KCroppingFormPointAndSize = 2;

// Forms for specifying scaling
const TInt KScalingFormReductionFactor = 1;
const TInt KScalingFormDesiredSize = 2;
_LIT8(KJpgMimeType,"image/jpeg");

// Unique Ids of the content file
_LIT(KRightsUniqueId001, "ICLTestjfif444Id001"); // Rights count 5

// Check the status of decoding/transformations and if error, log the error msg and leave with the error
#define CHECK_STATUS_LEAVE_IF_ERROR(err, msg) \
                           if (err != KErrNone) \
                               { \
                               TBuf<KBufferLen> errMsg(msg); \
                               errMsg.Append(_L(", error = %d")); \
                               ERR_PRINTF2(errMsg, err); \
                               User::Leave(err); \
                               }
                               
// Check the status of decoding/transformations and if error, log the error msg and leave with the error
#define CHECK_STATUS_LEAVE_IF_ERROR_STREAM(err, msg) \
                           if (err != KErrNone) \
                               { \
                               TBuf<KBufferLen> errMsg(msg); \
                               errMsg.Append(_L(", error = %d")); \
                               ERR_PRINTF2(errMsg, err); \
                               RetainEncodedImage(); \
                               User::Leave(err); \
                               }

// forward declaration
class RStepBaseImageDecoder;


// for handling asynchronous events
class CActiveListener : public CActive
    {
public:
    CActiveListener(RStepBaseImageDecoder* aBase);
    void InitialiseActiveListener();

    // from CActive
    virtual void RunL();
    virtual void DoCancel();

public:
    RStepBaseImageDecoder* iBase;
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


// Utility class with members used for almost all tests
// This approach is followed to avoid the KErrNoMemory while loading the suite/dll 
// i.e., when LOAD_SUITE from script or AddTestStepL() in CTestSuiteImageDecoder::InitialiseL() is called
// Due to large number of data members commonly used, that adds to the size of each test step object, 
// hence testframework encounters KErrNoMemory while loading the suite/dll
class CUtilityMembers : public CBase
    {
public:
    CUtilityMembers();
    ~CUtilityMembers();

public:
    // Specifies the underlying hardware under test
    enum TDriveType
        {
        EDstMediaRamDrive = 0,
        EDstMediaMMCDrive = 1
        };

    // Individual extensions or combination of extensions to perform the operation
    enum TExtensionType
        {
        EExtensionCrop, // = 0
        EExtensionScale,
        EExtensionOperation,
        EExtensionCropScale,
        EExtensionCropOperation,
        EExtensionScaleOperation,
        EExtensionCropScaleOperation,
        EExtensionStream,
        EExtensionScaleStream
        };

    RFs iFs;
    RFile iFile;

    CFbsBitmap*      iDestBitmap;      // decoded destination bitmap
    CFbsBitmap*      iDestMaskBitmap;  // destination mask bitmap
    CFbsBitmap*      iSrcBitmap;      // encode source bitmap
    CBitmapScaler*   iBmpScaler;       // for scaling the bitmap
    CBitmapRotator*  iBmpRotator;      // for rotating/mirroring the bitmap

    CImageDecoder*   iImageDecoder;
    CImageEncoder*   iImageEncoder;
    CJPEGImageFrameDecoder* iImageFrameDecoder;
    CJPEGImageFrameEncoder* iImageFrameEncoder;

    MExifMetadata*            iExifMetadata;
    CJPEGExifEncoder*         iExifImageEncoder;
    TImageConvStreamedEncode* iStreamEncode;
    TImageConvStreamedDecode* iStreamDecode;

    RChunk iChunk;                     // used for decoding the image to YUV pixel buffer
    CImageFrame*     iImageFrame;
    CFrameImageData* iFrameImageData;
    
    CActiveListener* iActiveListener;  // for handling asynchronous events
    CUtilityTimer*   iTimer;

    HBufC8* iBuffer;                   // for creating buffer for image frame to hold the stream of data

    TTimeIntervalMicroSeconds32 iTimerExpire;

    CImageDecoder::TOptions iDeOptions;
    CImageEncoder::TOptions iEnOptions;

    TSize iDestSize;                   // destination bitmap size, returned from a call to GetDestinationSize()
    TRect iCropRect;                   // cropping rectangle

    RUidDataArray iPropertiesArray;

    RArray<TSize>  iRepeatCropSizeArray;// only 3 crop tests use these, but moved here to avoid memory leak from its test step
    RArray<TPoint> iRepeatCropPointAArray;
    RArray<TPoint> iRepeatCropPointBArray;
    RArray<TFileName> iRepeatCropOutputImageArray;
    RArray<TFileName> iRepeatCropReferenceImageArray;


    // state of the test; when asynchronous events are completed, with or without error, 
    // next state is called and the error is handled
    TInt iState;


    TBool iExtPerfTest;           // ETrue indicates tests related to performance measurements which use extensions
    TBool iNegativeTest;          // ETrue indicates test is a negative test, expecting error or incorrect behaviour

	TUint32 iBeginDecodeCounter;  // to know no. of counters during decoding
	TUint32 iEndDecodeCounter;

    TBool iRestartConvAfterCancel;// indicates if conversion is to restart if it had been cancelled
    TBool iConvCancelled;         // set to true if conversion is cancelled
    TBool iConvCancelTest;        // conversion-cancel type of test
	TBool iAutorotate;			  // ETrue indicates autorotate applied for decoding
	TBool iSetExif;               // ETrue indicates  exif tag set before decoding

    /* holds the indexes/values read from the configuration file */
    TFileName iInputImage;        // input image to decode
    TFileName iOutputImage;       // save decoded bitmap to this file
    TFileName iOutputImage1;       // save decoded bitmap to this file FOR AUTOROTATE OF invalid exif tag
    TFileName iReferenceImage;    // compare decoded bitmap with the reference file

    TBool iHwSaveOutputToAFile;   // hardware test run only; true if decoded bitmap (with or without transformations) is saved to a file
    TBool iAlwaysThread;          // true to perform decoding in a separate thread

    TBuf<KSmallBufferLen> iImageType;// image being decoded is a thumbnail as source image or a main image

    TInt  iFrameDisplayModeIndex; // index as used in FrameDisplayMode() to return display mode

    TInt  iYUVFormatIndex;        // index as used in GetYUVFormat()
    TInt  iColorSamplingIndex;    // index as used in SetImageData()
    TInt  iQualityFactor;         // quality factor as described in TJpegImageData

    TInt  iDeOptionsIndex;        // index as used in GetDecodeOptions()
    TInt  iRotationIndex;         // index as used in GetRotationAngle()
    TInt  iMirroringIndex;        // index as used in GetRotationAngle()
    TInt  iOperationIndex;        // index to read operation or multiple operations

    TInt  iImplementationUidIndex;// index to indicate codec implementation Uid
    TUid  iDecoderImplementationUidValue; // codec implementation Uid

    // for extension scale use
    TInt  iScalingCoeff;          // refer TImageConvScaler::SetScalingL()
    TSize iExtScaleSize;          // destination bitmap size for scaling
    TBool iLockAspectRatio;
    TInt  iScalingQualityIndex;
    TBool iScalerExtUseDesiredSizeMethod; // if true uses SetScalingL(TSize&, TImageConvScaler::TScalerQuality, TBool) else uses SetScalingL(TInt, TImageConvScaler::TScalerQuality)

    // for non-extension scale use
    TInt  iScalingForm;           // KScalingFormReductionFactor(1) or KScalingFormDesiredSize(2)
    TInt  iReductionFactor;       // reduction factor for bitmap scaling
    TSize iScaleSize;             // desired size of the destination bitmap

    TInt   iCroppingRectForm;     // KCroppingFormPointAAndB(1) or KCroppingFormPointAndSize(2)
    TPoint iCropPointA;
    TPoint iCropPointB;           // four coordinates of the cropping rectangle
    TSize  iCropSize;             // width and height of the cropping rectangle

    TTime iStartTime, iEndTime;   // in microseconds
    TInt  iExpectedDecodeTime;    // time for decoding, in microseconds
    TInt  iExpectedEncodeTime;    // time for encoding, in microseconds


    // for stream decoding
    TUid    iOptimalFormat;       // format supported for decoding JPEG image based on color sampling
    TInt    iStreamMode;          // see global declaration above for stream modes
    TBool   iEnInitialiser;       // if true(1) -> initialise encoder, if false(0) -> no encoder intialization
    TSize 	iBlockSizeInPixels;   // used for streamed encoding
    TInt    iNumBlocksToGet;      // no. of blocks to get into image frame while decoding
    TFrameInfo iFrameInfo;        // decoder frameinfo
    TBool   iHwRetainOutputImage; // if true, retain output image after encoding; if false, delete encoded image after encoding (by default false for hw test run)
    TInt    iNumBlocksRead;       // no. of blocks read after calling GetNextBlocks()
    TInt    iBlockPosition;       // block position from where the random stream decoding should start
    TBool   iWriteExif;           // if true(1) -> write EXIF data, if false(0) -> don't add EXIF data while encoding
    TBool   iWriteThumbnail;      // if true(1) -> write thumbnail data, if false(0) -> don't add thumbnail data while encoding
    TBool   iHaveMoreBlocks;      // if true(1) -> still there are some blocks to be stream-decoded, if false(0) -> no blocks are left for decoding
    TInt    iOPImageHeight;       // output image height and width specified for stream decoding test cases
    TInt    iOPImageWidth;
    TInt    iBlocksToEncoder;     // no. of blocks to get from stream decoding and append to stream encoding
    TInt    iIterations;          // no. of iterations to be performed while streaming (decoding then encoding) to get the required blocks
    TInt64  iEncodeActualTime;    // total actual time taken for encoding
    TInt64  iDecodeActualTime;    // total actual time taken for decoding
    RArray<TInt> iBlockPosArray;  // to hold the multiple block positions in arry for random stream decoding
    
    // for reading and holding decuid from config file
    TInt 	iDecUid;              // To hold the integer value of the decoder uid
    TUid 	iDecoderUid;          // To hold the decoder value
    TPtr8	iSourceDataPtr;		  // To hold the inputimage in bufferpointer for DataNewL 
    HBufC8*	iSourceData;		  // To hold the inputimage in buffer for DataNewL
    TInt 	iSourceLength;		  // To hold the inputimage length for DataNewL
    TBuf8<KBufferLen>  iMimeType; // To hold the mimetype of jpeg for GetMimeTypeDataL function
    TInt 	iExpectedError; 	  // To hold the expected error for prepare API
    };


// Test Step super base class for all derived test steps
class RStepBaseImageDecoder : public RAsyncTestStep
    {
public:
    // FsmL() implementation based on the states
    virtual void FsmL()
        {
        };

    virtual void TimerExpired()
        {
        };

protected:
    // Ctor
    RStepBaseImageDecoder(const TDesC& aStepName, const TDesC& aTestCaseDescription);
    
    // Comparing the o/p bitmap with the refernce bitmap byte by byte
	void CompareFileL();
    
    // Read common input from the configuration file
    virtual TInt ReadCommonInputFromConfigFile()
        {
        return KErrNone;
        };
    TInt ReadCommonInputImageFromConfigFile();
    TInt ReadCommonScalingInputFromConfigFile();
    TInt ReadCommonExtensionScalingInputFromConfigFile();
    TInt ReadCommonInputExpectedPerfTimeFromConfigFile();
    TInt ReadCommonInputRefAndOutputImagesFromConfigFile();

    // Compare decoded bitmap with the reference image
    void BitmapComparisonL(const CFbsBitmap& aBitmap, const TDesC& aRefFilename);

    // Get execution time according to the current hardware. Currently supported: H2, H4 and emulator
    TInt64 ScaleTimeByMachineL(TInt64 aExecTime, TInt aDestMedia = CUtilityMembers::EDstMediaRamDrive);

    // Check performance based on time and log the result
    void CheckPerformanceOutputAndSaveL();

    // Check stream decoder and encoder performance based on time and log the result
    void CheckStreamPerformanceOutputAndSaveL();

    // Log display mode for the frame
    void LogFrameDisplayMode();

    // Save the output of the decoded image (after all operations are done) to a file
    TInt SaveDecodedImage();

    // Retain encoded output image
   	TInt RetainEncodedImage();

    // Create image decoder
    TInt CreateImageDecoder();

    // Create output directory for encoder tests
    TInt CreateOutputDirectory();

    // Add color sampling, blocksize in pixels based on colorsampling and quality factor for the bitmap
    void PrepareDecodeDataL();

    // Crop, rotate and/or mirror, scale, multiple operations on the image during decode
    void DecodeToBitmapDuringDecodeL(const TSize aDestSize);
    
    // Encode the input image and set exif orientation
    void EncodeAndSetExifL();
    
	// Crop, rotate and/or mirror, scale, multiple operations on the image during decode
    void DecodeBufferToBitmapDuringDecodeL(const TSize aDestSize);
    
    // Crop the image during decoding with mask
    void DecodeWithMaskDuringDecodeL(const TSize aDestSize);

    // Crop, rotate and/or mirror the image during decoding to YUV pixel buffer
    void DecodeToYuvPixelBufferDuringDecodeL(const TSize aDestSize);

    // Encode YUV pixel buffer (image frame) to an image
    void EncodeYuvPixelBufferToAnImageL();

    // Compare output from YUV pixel buffer tests (jpg)
    void CompareImagesL();

    // Change the state to next
    void StateChange();

    // Add operation(s)
    void AddOperationL();
	
	// Check autorotateflag status, if true apply for decoding else proceed as is
	TBool CheckAutoRotate();

    // All GET methods...
    // Flags to control how the image is decoded; these can be combined using an OR operation
    TInt GetDecodeOptions(CImageDecoder::TOptions& aOptions);

    // Get actual rotation angle/mirroring values, from the values read from config file
    TInt GetRotationAngle(const TInt aAngleIndex, CBitmapRotator::TRotationAngle& aAngle);

    // Get rectangle for cropping, from the values read from config file
    void GetCropRectangle(TRect& aCropRect);

    // Get reduced size, from the values read from config file
    void GetScalingSizeL(const TSize aOriginalSize, TSize& aReducedSize);

    // Get scaling quality when scale extension interface is used
    void GetScalingQualityL(TImageConvScaler::TScalerQuality& aScalingQuality);

    // Get YUV format
    TInt GetYUVFormat(TUid& aYUVFormat);

    // Get image type uid based on input file extension
    TUid ImageType();

    // Get image size - width and height
    TSize ImageSize();

    // Get destination filename
	TFileName& DestFilename();

    // Get display mode for the frame
    TDisplayMode FrameDisplayMode();


    // All SET methods...
    // Set the thread for decoding - client OR separate thread; can be combined with other options
    void SetThreadForDecoding(CImageDecoder::TOptions& aOptions);

    // Set the thread for encoding - client OR separate thread; can be combined with other options
    void SetThreadForEncoding(CImageEncoder::TOptions& aOptions);

    // Set extensions - crop, scale, operation and various combinations in the defined order
    void SetExtensionL(const CUtilityMembers::TExtensionType aExtensionType, TSize& aExtensionDestSize);

    // Set the source image type - thumbnail as source image OR main image
    void SetImageTypeL();

    // Set color sampling and quality factor information
    void SetImageDataL();

    // Set YCbCrPositioning and ColorSpace tag of the EXIF data
    TInt SetShort(); 

    // Set user comment tag of the EXIF data
    TInt Set16bitBuffer();

    // Set model version tag of the exif data	
    TInt SetModelVersion();
    
    // Set the orientation tag of the exif data
    TInt SetOrienetationTag();
            
    // Set the EXIF data for the encoded image
    TInt SetExifAndThumbnailData(); 
	
	//used for DataNewL Coverage
	void IncrementBufferSize();
	
    // Log the test case description, input image name and its size
    void LogTestCaseDescription();

    // from RAsyncTestStep
    virtual void KickoffTestL();       // Start the test
    virtual void CloseTest();          // Cleanup at the end of the test

    virtual void DoKickoffTestL() = 0; // derived test step class should implement

protected:
    TBuf<KSmallBufferLen> iSectionName;
    TBuf<KBufferLen> iTestCaseDescription;

    TBool iYuvTest;                    // ETrue indicates tests related to YUV pixel buffer
    TBool iScaleTest;                  // if true(1) -> scale test type, false(0) -> not a scale test type
    CUtilityMembers* iUM;              // use of data members for all tests, allocated memory for iUM on the heap
    TBool iDescriptorTest;
    TBool iIntentTest;
    TBool iDRMTest;
	};


#endif // TSI_ICL_DECODE_STEPBASE_H
