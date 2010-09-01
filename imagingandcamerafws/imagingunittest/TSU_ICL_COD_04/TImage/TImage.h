// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef TIMAGE_H
#define TIMAGE_H

#include <fbs.h>
#include <bitstd.h>
#include <bitdev.h>
#include "TestExtra.h"

#include <imageconversion.h>
#include <imagetransform.h>
#include <iclexif.h>

#include <icl/imageconversionextension.h>
#include <icl/imageconversionextensionintf.h>

#include <iclexifimageframe.h>
#include <testframework.h>
_LIT8(KUnicode, "UNICODE\0");
_LIT8(KAscii, "ASCII\0\0\0");

#define KThumbnailSize TSize(160,120)
class CExifVerifier;
// TExifTransformSourceDest is used to encapsulate the source or dest
// to be used in an Exif transform.  It is used as a parameter to some
// of the test methods that can set up an image transform for file or
// data sources & dests.
class TExifTransformSourceDest
	{
public:
	//Construct an object representing a filename source or dest
	TExifTransformSourceDest(const TDesC& aFilename);

	//Construct an object representing a const source descriptor
	TExifTransformSourceDest(const TDesC8& aData);

	//Construct an object representing a dest descriptor
	TExifTransformSourceDest(HBufC8*& aDataPtr);

	//True if and only if object represents a source or dest descriptor
	TBool IsData() const;

	//True if and only if object represents a source or dest filename
	TBool IsFilename() const;

	//Returns the filename. Panics if object does not represent a file
	const TDesC& Filename() const;

	//Returns the source descriptor. Panics if object does not represent a source descriptor
	const TDesC8& Data() const;

	//Returns the dest descriptor. Panics if object does not represent a dest descriptor
	HBufC8*& DataPtr() const;
private:
	enum TExifTransformSourceDestType
		{
		EFilename,	// object contains a filename
		EData, 		// object contains a const source descriptor
		EDataPtr 	// object contains a pointer to a destination HBufC8
		};

	union
		{
		const TDesC* iFilename;	// not owned
		const TDesC8* iData;	// not owned
		HBufC8** iDataPtr;		// not owned
		};
	TExifTransformSourceDestType iType;
	};


class MExifMetadata;

class TPerformanceTestParams
	{
public:
    enum TDriveType
        {
        EDstMediaRamDrive,
        EDstMediaMMCDrive
        };
public:
	const TText*       		iSrcFileName;		// source file name
	const TText*       		iRefFileName;		// reference file name
	TDisplayMode			iDisplayMode;		// decoding display mode
	TInt					iReductionFactor;	// reduction factor
	TInt					iMaxDecodeTime;		// in microseconds
	TUint					iDecoderOptions;	// Decoder TOptions parameter
	TUint					iEncoderOptions;	// Encoder TOptions parameter	
	TInt                    iEncoderQuality;    // encoder quality settings
	TInt                    iEncoderFormat;     // JPEG sampling format
	TInt                    iDstMediaType;      // destination media type
	// extended
	TInt iCropX;
	TInt iCropY;
	TInt iCropW;
	TInt iCropH;
	TInt iNumRepeats;
	TInt iOperate1;   
	TInt iOperate2;
	TInt iScaleCoeff;
	TInt iScaleQuality;
	TInt iNumZoom;  
	TInt iUseExtEncode;  // use extension (rather than rotator) in encode
	};


const TInt KMaxHandlers = 4;  // max number of concurrent loaders/savers
const TInt KLoaders = 2;  // max number of concurrent loaders
class TImage;
class CTestRandomCancel;
class CTestImageHandler : public CActive
    {
    public:
        CTestImageHandler( RFs& aFs, TImage* aUO, TInt64& aSeed );
        ~CTestImageHandler();

        virtual void InitialiseL( const TDesC& aFileName ) = 0;
		virtual void StartL() = 0;

		void DebugPrint( const TDesC& aMsg );

		TInt iLoaded;
		TInt iCanceled;
		TInt iStarted;
		TInt64& iSeed;

    public:
        RFs& iFs;
		TFileName iFileName;
		CTestRandomCancel* iCanceler;
		CFbsBitmap* iBitmap;

		TImage* iUO;

		TInt iId;
    };

class CTestRandomCancel : public CTimer
    {
    public:
        CTestRandomCancel( CTestImageHandler* aImageLoader, TInt aAfter, TInt64& aSeed );
        ~CTestRandomCancel();
		void StartL();

        virtual void RunL();
        virtual void DoCancel();

        CTestImageHandler* iImageLoader;
        TInt iAfter;
		TInt64& iSeed;
    };

class CTestImageSaver : public CTestImageHandler
    {
    public:
        CTestImageSaver( RFs& aFs, TImage* aUO, TInt64& aSeed, TInt aId );
        ~CTestImageSaver();

        void InitialiseL( const TDesC& aFileName );
		void StartL();

        // From CActive
	    void RunL();
        void DoCancel();

    public:
		CImageEncoder* iEncoder;
    };

class CTestImageLoader : public CTestImageHandler
    {
    public:
        CTestImageLoader( RFs& aFs, TImage* aUO, TInt64& aSeed, TInt aId );
        ~CTestImageLoader();

        void InitialiseL( const TDesC& aFileName );
		void StartL();

        // From CActive
	    void RunL();
        void DoCancel();

    public:
		CImageDecoder* iDecoder;
    };
class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

// variant which properly cancels the decoder, where defined
class CActiveDecodeListener : public CActiveListener
	{
public:
	void InitialiseActiveListener(CImageDecoder* aDecoder)
		{
		iDecoder = aDecoder;
		CActiveListener::InitialiseActiveListener();
		}
	// From CActive
	virtual void DoCancel()
		{
		if (iDecoder)
			iDecoder->Cancel();
		}
protected:
	CImageDecoder* iDecoder; // not owned
	};

// variant which properly cancels the Encoder, where defined
class CActiveEncodeListener : public CActiveListener
	{
public:
	void InitialiseActiveListener(CImageEncoder* aEncoder)
		{
		iEncoder = aEncoder;
		CActiveListener::InitialiseActiveListener();
		}
	// From CActive
	virtual void DoCancel()
		{
		if (iEncoder)
			iEncoder->Cancel();
		}
protected:
	CImageEncoder* iEncoder; // not owned
	};

// variant which properly cancels the Encoder, where defined
class CActiveTransformListener : public CActiveListener
	{
public:
	void InitialiseActiveListener(CImageTransform* aTransform)
		{
		iTransform = aTransform;
		CActiveListener::InitialiseActiveListener();
		}
	// From CActive
	virtual void DoCancel()
		{
		if (iTransform)
			iTransform->CancelTransform();
		}
protected:
	CImageTransform* iTransform; // not owned
	};

// TImageFormat is used to restrict individual tests
enum TImageFormat
	{
	EANY,
	EBMP,
	EGIF,
	EICO,
	EJPG,
	EMBM,
	EPNG,
	ETFF,
	EWMF,
	EOTA,
	EWBP
	};

enum TExifDataType
	{
	E8BitBuffer,
	E16BitBuffer,
	EIntegerBuffer,
	EShortBuffer,
	ERationalBuffer,
	ERationalArrayBuffer,
	EIntegerArrayBuffer,
	EShortArrayBuffer,
    EShortestShortArrayBuffer,
	ETagNotExistsError,
	EInvalidFormatError,
	EDefaultTagSet,
	EGetThumbnail,
	EGetThumbnailError,
	ESetThumbnail,
	ESetNoThumbnail,
	EGenericTest,
	EGetThumbnailHeader,
	ESetImageType,
	EGetCorruptThumbnailHeader,
	EAsciiBuffer,
	EUnicodeBuffer
	};

enum TExifOperation
	{
	ESetParam,
	EAddParam,
	EGetParam,
	ENoParam
	};

enum TExifEncodeStep
	{
	ESetData,
	EAddData,
	EVerifyData,
	};

class MExifMetadata;
class CJPEGExifDecoder;
class CJPEGExifEncoder;
class TPerformanceTestParams;

enum TFormat
	{
	EByte = 1,
	EAscii = 2,
	EUnsignedShort = 3,
	EUnsignedLong = 4,
	EUnsignedRational = 5,
	EUndefined = 7,
	ESignedLong = 9,
	ESignedRational = 10
	};

// class to hold values identifying a particular IFD entry
class TIfdEntryDetails
	{
public:
	TIfdEntryDetails( TUint aIfd, TUint aTag, TFormat aDataType );

public:
	TUint			iIfd;
	TUint			iTag;
	TFormat			iDataType;
	};

class TImage : public RTestExtra
	{
public:
	TImage(const TPerformanceTestParams* aPerfTstParams, const TDesC& aTestName);
	void ConstructL();
	void Test();
	void End();

	void GetExifData(const TExifDataType& aDataType, const TExifOperation& aOperation);
    void GetExifShortData(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetExifData(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestThumbnail(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TransformExif(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TransformSetSize(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TransformThumbnailFromJpegImage(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TransformThumbnailFromExifImage(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TransformThumbnailFromThumblessExifImage(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestExifUtility(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void AllocTest(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void AllocSetImageTypeTestL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// extra tests //
	void TestDerivedExifPluginL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestNonExifAPP1MarkerL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestMultipleEncodeDecodesWithRandomCancelsL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	TInt CountsChanged();
	void StopEncodeDecode();

	// image frame tests //	
	void TestJpgChunkImageFrameEncodePerfL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestJpgChunkImageFramePluginL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestJpgDescImageFramePluginL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void NegTestJpgImageFrameEncoderPluginL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void NegTestJpgImageFrameDecoderPluginL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void NegTestJpgImageFramePluginReductionL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestJpgImageFramePluginNewThreadL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestImageFrameDerivedL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void NegTestFullImageFrameL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void StreamDecodeFileToImageFrameL(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void NegTestStreamDecodeFileToImageFrameL(const TExifDataType& aDataType, const TExifOperation& aOperation);

	// PREQ 1630 image frame block stream tests //
	void TestJpgDescStreamImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgDescStreamUnderFlowImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgDescStreamOverFlowImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgImageFrameBlockStreamerSetupL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgImageFrameBlockStreamerInteroperabilityL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	
	void TestJpgDescSequentialStreamDecodeEncodeImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgDescSeqStreamDecEncBlockwiseImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgDescRandomStreamDecodeEncodeImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestJpgDescRandomBackStreamDecEncImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);	
	void TestJpgDescRandomFwdBackStreamDecEncImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);	
	void TestJpgDescStreamNegativeTestsL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	
	void TestStreamEncodeAllocL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void StreamEncodeImageFrameL(CImageFrame* theImageFrame);
	
	void TestStreamEncodeDecodeAllocL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void StreamEncodeDecodeImageFrameL(const TBool aIsThreaded);
	// JPEG performance Tests //
	void JpegDecoderPerformanceTestL(const TExifDataType& aFormat, const TExifOperation& aOperation);
	void JpegEncoderPerformanceTestL(const TExifDataType& aFormat, const TExifOperation& aOperation);
	void TestJpgSequentialStreamDecodeEncodePerformanceL(const TExifDataType& aFormat, const TExifOperation& aOperation);

	// CR0712   Relax JPEG and GIF decoding strictness
	void TestRelaxedJpgDecodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */);

	// DEF082361: Test that ExifJpegTransform plugin does not leak handles //
	void TestExifJpegTransformHandleLeak(const TExifDataType& aDataType, const TExifOperation& aOperation);

	// DEF083826: CJPEGImageFrameXXX transcoding doesn't work //
	// DEF096682: CJPEGImageFrameDecoder fails with Kern-EXEC 3 when used with progressive JPEG
	void DoTestJpegImageFrameTranscodeL(const TDesC& aFileName, const TDesC& aRefFileName);
	void TestJpegImageFrameTranscodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// INC083025 ICL Image Thumbnail Transform does not set thumbnail exif tags //
	void VerifyIFD1FromThumblessExifSource(const TExifDataType& aDataType, const TExifOperation& /* aOperation */);
	void VerifyIFD1FromThumbedExifSource(const TExifDataType& aDataType, const TExifOperation& /* aOperation */);

	// DEF084849: CScaledJpegGenerator does not add SOI marker to generated thumbnail //
	void VerifyTransformedThumbnailSOI(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// DEF082165: Exif - Ambiguous Null termination of ascii string tags //
	void TestAsciiNullConsistency(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TestAsciiNonNullConsistency(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void NegTestAsciiNonNullConsistency(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	//PDEF085003: Exif Output file corrupt if JPEGInterchangeFormatLength tag not present
	void TransformTestIFD1Anomalies(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void DecodeTestIFD1Anomalies(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	//DEF090653: Exif: Image comments incorrectly copied when generating Thumbnail
	void TestThumbnailNoComments(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// DEF091418: T: Transforming a Nikon camera jpeg image returns KErrNotSupported.
	void TransformTests(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// DEF093013: T: If Transform() fails with -4, a garbage destination image is left in C drive
	void TransformTests2(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// DEF092988: T: Aspect ratio of scaled image is maintained evenif iMaintainAspectRatio=EFalse
	void TransformTests3(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// DEF091185: T: Exif data is added after the transform to an image which had no exif.
	void TransformNonExifImageTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// CR910: ICL JPEG Support for non conformant exif data
	void IgnoreBrokenIfdEntryTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// CRPCUG-6SSBQ7: ICL CImageDecoder / CImageTransform option to skip EXIF metadata decoding.
	void DecodeTestWithIgnoreExifMetadataFlag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// // CRPCUG-6SSBQ7: ICL CImageDecoder / CImageTransform option to skip EXIF metadata decoding.
	void TransformWithIgnoreExifMetadataFlag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// INC101733: Behaviour of Exif transform with successive invalid tags is not as expected
	void IfdWithUnknownTagsTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	
	// DEF101880: JPEG Encoder may produce distorted images
	void TestJpgDistortedAfterEncodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	
	// PDEF110735: JPEG Exif IFDs with 0 entries can cause invalid JPEG
	void IFDNoEntries(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);

	// DEF110487: Decoding jpeg file with height of 17 pixels to image frame buffer errors with -9
	void TestJpegWithVariousFileSizesL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void DoTestJpegDescWithVariousFileSizesL(CFbsBitmap* aBitmap, CActiveDecodeListener* decodeListener);
	void DoTestJpegChunkWithVariousFileSizesL(CFbsBitmap* aBitmap, CActiveDecodeListener* decodeListener);
//CR1329 tests
	void CheckDeviceInfoTagsOnEncodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */);
	void CheckDeviceInfoTagsOnTransformL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */);
	void CheckDeviceInfoTagsOnModifyEncodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */);
	void CheckDeviceInfoTagsOnModifyTransformL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */);
	TBool VerifyDeviceInfoTags(MExifMetadata* aExifMetadata, TPtr8 aManufacuter, TPtr8 aModel, TPtr8 aUIPlatform);
	void AlterDeviceInfoTagsL(MExifMetadata* aExifMetadata, TPtr8& aManufacuter, TPtr8& aModel, TPtr8& aUIPlatform);
	void GetDeviceInfoFromSysUtilL(HBufC8*& aManufacturer, HBufC8*& aModel, HBufC8*& aUIPlatform);
	//PDEF128742:Image transform can not handle illegal exif when original image is to preserved 
	void TransformWithPreserveL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
protected: //protected so that derived class TGPSImage has access
	void TestL();

	//
	// Exif metadata getters tests
	TInt Get8bitBuffer(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt Get16bitBuffer(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetInteger(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetShort(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetRational(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetIntegerArray(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetRationalArray(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetShortArray(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetShortestShortArray(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetNotExistError(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt GetInvalidFormat(MExifMetadata* aMetaData, const TExifOperation& aOperation);
	// Exif metadata getters tests
	//


	//
	// exif metadata setters tests
	TInt SetRational(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt SetInteger(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt SetShort(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt Set16bitBuffer(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt Set8bitBuffer(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt SetIntegerArray(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt SetRationalArray(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt SetShortArray(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	TInt SetDefaultTagSet(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation);
	// helpers
	void ProcessSetError(const TInt& aError, const TInt& aTag);
	TInt TestSetData(const TExifDataType& aDataType, MExifMetadata* aMetadata, const TExifEncodeStep& aEncodeStep, const TExifOperation& aOperation);
	// exif metadata setters tests
	//

	//
	// Exif Thumbnail tests
	void TransformThumbnail(const TExifDataType& aDataType, const TFileName& aSourceFileName);
	void TestGetThumbnail(const TFileName& aSourceFileName, const TDesC& aReferenceFile);
	void TestSetThumbnail();
	TInt TestGetThumbnailError(const TExifTransformSourceDest& aDest, const TBool aThumbnailExpected);
	void TestSetNoThumbnail();
	void TestThumbnailHeader();
	TInt TestCorruptedHeaderFileL(const TDesC& aSourceFileName, const TDesC& aRefBmpFileName);
	void TestSetImageTypeOnCorruptedHeaderFiles();
	// INC083025 ICL Image Thumbnail Transform does not set thumbnail exif tags //
	void VerifyIFD1(const TExifDataType& aDataType, const TFileName& aSourceFileName);
	void DoTransformIFD1Anomaly(const TInt aSourceIndex, const TExifTransformSourceDest& aDest, const TBool aThumbnail, const TBool aPreserve);
	// Exif Thumbnail tests
	//

	//
	// Exif Utility tests
	TInt DoTestExifUtility(MExifMetadata* aMetaData);

	// helpers
	void GetTagDesc(TInt UtilityMethod, TInt& tagID, TInt& ifd);
	TInt CompareDescValue(TDesC8* aParam1, TPtrC8 aParam2);
	TInt CompareDescValue(TDesC8* aParam1, TDesC8* aParam2);
	TInt CompareRationalValue(TInt aParam11, TInt aParam12, TInt aParam21, TInt aParam22);
	TInt CompareShortValue(TUint16 aParam1, TUint16 aParam2);
	TInt CompareIntegerValue(TInt aParam1, TInt aParam2);
	TInt CompareUnsignedIntegerValue(const TUint aParam1, const TUint aParam2) const;
	CImageTransform* PrepareImageTransformL(const TExifTransformSourceDest& aSource, const TExifTransformSourceDest& aDest,
		const TBool aThumbnail, const TBool aPreserveMain=EFalse, const TBool aMaintainAspectRatio=ETrue,
		const TSize aSize=KThumbnailSize, const TBool aIgnoreMetadataFlag=EFalse);
	TInt DoTransformTest(const TExifTransformSourceDest& aSource, const TExifTransformSourceDest& aDest, const TBool aThumbnail, const TBool aPreserveMain, const TBool aMaintainAspectRatio=ETrue, const TSize aSize=KThumbnailSize);
	TInt DoImageTransformL(CImageTransform* aImageTransform);
	TInt DoImageDecodeL(CImageDecoder* aDecoder, CFbsBitmap& aBitmap);
	TInt DoImageEncode(CImageEncoder* aEncoder, CFbsBitmap& aBitmap);
	void CreateEncoderFromReferenceBitmapFileL(const TDesC& aRefBitmapFileName, CFbsBitmap*& aBitmap, const TFileName& aDestFileName, CJPEGExifEncoder*& aEncoder);
	void ReadFileIntoBufferL(const TFileName& aFileName, HBufC8*& aBuffer);
	HBufC8* CreateUnicodePrefixedBufferL(const TDesC16& aUnicodeBuffer);
	HBufC8* CreateAsciiPrefixedBufferL(const TDesC8& aAsciiBuffer);
	// Exif Utility tests
	//
	void TransformExifGet(const TExifDataType& aDataType);
	void TransformExifSet(const TExifDataType& aDataType);
	void TransformExifBasic(const TExifDataType& aDataType);

	//
	// Helpers
	CJPEGExifDecoder* CreateDecoderOnFileL(const TFileName& aFileName);
	CJPEGExifEncoder* CreateEncoderOnFileL(const TFileName& aFileName);
	TBool CompareBitmapsL(const CFbsBitmap& aBitmap1, const TDesC& aRefFileName);
	TBool CompareBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2);
	void AllocTestStage1(const TExifDataType& aDataType, CImageTransform*& aImageTransform, MExifMetadata*& aMetaData);

	// DEF082165: Exif : Ambigous Null termination of ascii string tags //
	void TestEncoderAsciiNullConsistency(const TDesC8& aImageDescription, const TDesC8& aMake, const TDesC8& aDateTime);
	void TestTransformAsciiNullConsistencyL(const TDesC8& aImageDescription, const TDesC8& aMake, const TDesC8& aDateTime);
	TInt DoAsciiNullConsistencyTests(const TDesC8& aImageDescription, const TDesC8& aMake, const TDesC8& aDateTime, MExifMetadata* aMetaData);
	void NegTestEncoderAsciiNonNullConsistency(const TDesC8& aData);
	void NegTestTransformAsciiNonNullConsistency(const TDesC8& aData);
	TInt DoNegTestAsciiNonNullConsistency(MExifMetadata* aMetaData, const TDesC8& aData);
	void TestEncoderTooShortAscii(const TDesC8& aData);
	void TestTransformTooShortAscii(const TDesC8& aData);
	TInt DoAsciiNonNullConsistency(MExifMetadata* aMetaData, const TDesC8& aData);

	void DoTransformTests(const TInt aSourceIndex, const TExifTransformSourceDest& aDest, const TBool aThumbnail, const TBool aPreserve);
	void DoTransformNonExifImageTest(const TText* const aRefImages[], const TInt aDestIndex, TFileName& aSourceFileName, TFileName& aDestFileName, const TBool aThumbNail, const TBool aPreserveMain);

	// CR910: ICL JPEG Support for non conformant exif data
	TVerdict DoIgnoreBrokenIfdEntryTestL();
	TVerdict DoLoadAndCheckIfdEntriesL( const TDesC& aSourceFileName, const RArray<TIfdEntryDetails>& aExpectedPresentTags, const RArray<TIfdEntryDetails>& aExpectedMissingTags );
	TInt GetIfdEntryL( TIfdEntryDetails aEntry, MExifMetadata* aMetaData );

	// INC101733: Behaviour of Exif transform with successive invalid tags is not as expected
	TVerdict DoIfdWithUnknownTagsTestL();
	
	// CRPCUG-6SSBQ7: ICL CImageDecoder / CImageTransform option to skip EXIF metadata decoding.
	TVerdict DoTestWithIgnoreExifMetadataFlagSetL(TFileName aFileName);
	TVerdict DoTransformTestWithIgnoreExifMetadataFlagL();

	// Helpers
	//

	CImageDecoder::TOptions DecoderOptions() const;
	CImageEncoder::TOptions EncoderOptions() const;

	// image frame utilities //
	const TDesC& TestFileName(TInt aIndex);
	const TDesC& TestStreamFileName(TInt aIndex);


	void VerifyIfdOffsets(CExifVerifier& aVerifier, const TDesC& aExifFile);
	void VerifyAgainstReferenceFile(const TDesC& aDestFile, const TDesC& aRefFileName);
	
	// PDEF110735: JPEG Exif IFDs with 0 entries can cause invalid JPEG
	void CheckMandatoryEntriesInSubExif(CJPEGExifDecoder* aDecoder, TBool aCheckThumbnail = EFalse);
	void GenericTransformDescSrc(const TFileName& aSourceFileName,
								 const TFileName& aDestFileName, 
								 const TBool aThumbnail, 
								 const TBool aPreserveMain=EFalse, const TBool aMaintainAspectRatio=ETrue,
								 const TSize aSize=KThumbnailSize, const TBool aIgnoreMetadataFlag=EFalse);

public:
	TUid iDecoderUid;	
protected: //protected so that derived class TGPSImage has access
	TUint iDecoderOptions;
	const TPerformanceTestParams* const iPerfTstParams;
	CActiveListener* iActiveListener;
	CImageDecoder* iStreamDecoder;
	RFs iFs;
	TBool iUseThread;
	TBool iStreamedDecode;
    CTestImageHandler* iImageLoader[KMaxHandlers];
	TInt iLoaded;
	TInt iCanceled;
	TInt iStarted;
	TInt64 iSeed; // random seed (common for all objects)
	CActiveListener* iListener;
	RPointerArray<TDesC> iImageFiles;

private:
	int iUniqueNumber;
	// should only be used by VerifyAgainstReferenceImage
	TVerdict CompareFilesL(const TDesC& aFile1, const TDesC& aFile2);
	void CopyOutputFileL(const TDesC& aFileToCopy, const TDesC& aDestFileName);
	// helper function for DoTransformTestWithIgnoreExifMetadataFlagL.
	TVerdict TransformAndVerifyIgnoreExifMetadataFlagL(CImageTransform* aImageTransform, TFileName& aDestFile,
		TBool aExpectNullDecoderMetadata, TBool aDecoderCrashTest = EFalse);
	};

typedef void (TImage::*TImageCall)(const TExifDataType& aFormat, const TExifOperation& aOperation);

inline CImageDecoder::TOptions TImage::DecoderOptions() const
	{
	return CImageDecoder::TOptions(iDecoderOptions);
	}

inline CImageEncoder::TOptions TImage::EncoderOptions() const
	{
	return CImageEncoder::EOptionNone;
	}

#endif // TIMAGE_H
