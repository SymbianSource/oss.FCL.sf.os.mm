// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TIMAGE_H__
#define __TIMAGE_H__

#include <fbs.h>
#include <bitstd.h>
#include <bitdev.h>
#include "TestExtra.h"

#include <imageconversion.h>
//#include <icl/imageprocessor.h>

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
	EWBP,
	EEXIF
	};


enum TUniqueTransform  // there are only 8 possible transformation possibilities which are combinations of 90deg rotation and flip over an axis
	{
	/** Normal Decode
	*/
	ENormal = 0x11011000,

	/** Rotate 90 degrees.
	*/
	ERotate90	= 0x10110001,

	/** Rotate 180 degrees.
	*/
	ERotate180 = 0x00100111,

	/** Rotate 270 degrees.
	*/
	ERotate270 = 0x01001110,
	
	/** Horizontal flip.
	*/
	EHorizontalFlip = 0x10001101,
	
	/** Horizontal flip and rotate 90 degrees.
	*/
	EHorizontalFlipRotate90 = 0x11100100,

	/** Vertical flip.
	*/
	EVerticalFlip	= 0x01110010,

	/** Vertical flip and rotate 90 degrees.
	*/
	EVerticalFlipRotate90 = 0x00011011
	};

class TDecodePostProcessing; 

class TImage : public RTestExtra
	{
public:
	TImage();
	void ConstructL();
	void Test();
	void End();
	void InitSystemPath();
	void InitSystemPath4();
	void InitSystemPath6();
	
	//
	void Image1L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image2L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image3L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image4L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image5L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image6L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image6BL(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image7L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image8L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image9L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image10L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image12L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image13L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image14L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image15L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image16L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image16BL(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image17L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image18L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image19L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image20L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image21L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void Image22L(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void ImageIncrL(TImageFormat aFormat=EANY,const TDesC& aTestFile=KNullDesC);
	void ImageFrameIncrL(TImageFormat aFormat, const TDesC& aTestFile);
	
	TBool UseThread() const;
	void SetUseThread(TBool aUseThread);
	TBool StreamedDecode() const;
	void SetStreamedDecode(TBool aStreamedDecode);
public:
	TBool iDecoderStat;
	TUid iDecoderUid;
	TFileName iStepName;
protected:

	CImageDecoder::TOptions iDecoderOptions;

private:
	void TestL();
	void CheckFileL(const TDesC& aTestFileName,const TDesC& aRefFileName);
	TBool CompareFileL(const TDesC& aTestFileName,const TDesC& aRefFileName);

	const TDesC& RefFileName(TInt aFormatIndex, TImageFormat aFormat);
	const TDesC& SaveFileName(TInt aFormatIndex, TImageFormat aFormat);
	const TDesC& ConfigImageDataAndTypeL(CFrameImageData& aFrameImageData, TUid& aImageType, TInt aFormatIndex, TImageFormat aFormat);
	void ConfigFailImageDataAndTypeL(CFrameImageData& aFrameImageData, TUid& aImageType, TInt aFormatIndex, TImageFormat aFormat);

	void DoImage5_10L(TImageFormat aFormat,const TDesC& aTestFile, TInt aReductionFactor);
	void DoImage19_10L(TImageFormat aFormat, const TDesC& aTestFile,TDisplayMode aDisplayMode);
	
	void EncodeBitmapToFileL(const TDesC& aFileName, CFbsBitmap* aBitmap, const CFrameImageData* aFrameImageData, const TUid aImageType, TUniqueTransform aTransformOption = ENormal,TBool aPrepare = EFalse, TImageFormat aFormat = EANY);
	void EncodeBitmapToDescL(HBufC8*& aBufferDes, CFbsBitmap* aBitmap, const CFrameImageData* aFrameImageData, const TUid aImageType);
	void EncodeImageWithAllocFailureL(const TDesC& aFileName,CFbsBitmap* aBitmap, const CFrameImageData* aFrameImageData, const TUid aImageType, const TDesC& aRefFileName, TUniqueTransform aTransformOption = ENormal,TBool aPrepare = EFalse, TImageFormat aFormat = EANY);
	void DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor = 1, const TUid aImageType = KNullUid);
	void DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor,const TDesC8& aMIMEType);
	void DecodeImageWithAllocFailureL(const TDesC& aFileName,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType = KNullUid);
	void DecodeImageWithAllocFailureL(const TDesC& aFileName,TInt aZoomFactor, const CFbsBitmap& aReference, const TDesC8& aMIMEType);
	void DecodeDescToBitmapL(const TDesC8& aBufferDes,CFbsBitmap& aBitmap,TInt aZoomFactor = 1,const TUid aImageType = KNullUid);
	void DecodeFileToBitmapWithCancelL(const TDesC& aFileName,
											   CFbsBitmap& aBitmap,
											   TInt aZoomFactor,
											   const TUid aImageType,
											   TUint& aMaxDelay);
	void DecodeDescToBitmapWithCancelL(const TDesC8& aBufferDes,
											   CFbsBitmap& aBitmap,
											   TInt aZoomFactor,
											   const TUid aImageType,
											   TUint& aMaxDelay);
	void EncodeBitmapToDescWithCancelL(HBufC8*& aBufferDes, 
									   CFbsBitmap* aBitmap, 
									   const CFrameImageData* aFrameImageData, 
									   const TUid aImageType,
									   TUint& aMaxDelay);
	void StreamImageWithAllocFailureL(const TDesC& aFileName,TInt aFrameNo,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType = KNullUid);
	void StreamImageViaDesWithAllocFailureL(const TDesC& aFileName,TInt aFrameNo,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType = KNullUid);
	void StreamDecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aFrameNo,TInt aZoomFactor = 1, const TUid aImageType = KNullUid);

	void StreamDecodeFileToBitmapV2L(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aFrameNo, TInt& aFailCount, TInt aZoomFactor = 1, const TUid aImageType = KNullUid, TBool aIncludeFileNewApi = ETrue);

	void StreamImageWithAllocFailureV2L(const TDesC& aFileName,TInt aFrameNo,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType = KNullUid,TBool aIncludeFileNewApi = ETrue);

	void StreamDecodeFileToBitmapViaDesL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aFrameNo,TInt aZoomFactor = 1, const TUid aImageType = KNullUid);
	void CheckBitmapL(const TDesC& aFileName, CFbsBitmap& aBitmap);
	void CheckBitmapL(CFbsBitmap& aBitmap);
	TBool CompareBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2,TInt aQuarterTurns = 0);
	TBool CheckBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2,TInt aQuarterTurns = 0);
	TInt SaveBitmapToFile(CFbsBitmap& aBitmap,const TDesC& aFileName);
	static TUid ImageType(const TDesC& aFileName);
	static const TDesC& ImageSuffix(TImageFormat aFormat);
	static void GetSearchString(TDes& aSearchString, TImageFormat aFormat, const TDesC& aTestFile);
	void SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;
	void Prof1L(TBool aWaitForUnload);
	void DecodeFileToBitmapUsingExtensionsL(const TDesC& aFileName,CFbsBitmap& aBitmap,const TUid aImageType,const TDecodePostProcessing& aPostProcessing, const TDecodePostProcessing& aRedecodePostProcessing,TBool aPrepare,TInt aFailCount,TInt& aRestartFailCount);
	void DecodeFileToBitmapUsingExtensionsWithAllocFailureL(const TDesC& aFileName,CFbsBitmap& aBitmap,const TUid aImageType,const TDecodePostProcessing& aPostProcessing, const TDecodePostProcessing& aRedecodePostProcessing,TBool aPrepare,TDisplayMode aDisplayMode,TInt& aRestartFailCount);
	void ApplyDecoderTransformL(CImageDecoder* aDecoder,const TDecodePostProcessing& aPostProcess);
	void DoEncoderAllocTest(TImageFormat aFormat, const TDesC& aTestFile, TBool aTestExtensions);
	void ApplyOperationTransformL(TImageConvOperation* aOperation,TUniqueTransform aTransformOption);

	CImageDecoder::TOptions DecoderOptions() const;
	CImageEncoder::TOptions EncoderOptions() const;

    TInt GetMimeType(TInt aFormat, TInt aIndex, TDes8& aMimeType, TUid& aDecoderImplementationUidValue);

private:
	CActiveListener* iActiveListener;
	CImageDecoder* iStreamDecoder;
	RFs iFs;
	TInt iScaledIndex;
	TFileName iSourceFileName;
	TBuf<50> iStr;
	TKeyCode iGetKey;
	TBool iUseThread;
	TBool iStreamedDecode;
	TFileName iDefaultPath; // fix for SetDefaultPath issue
	};

typedef void (TImage::*TImageCall)(TImageFormat aFormat, const TDesC& aTestFileName);

inline TBool TImage::UseThread() const
	{
	return iUseThread;
	}

inline void TImage::SetUseThread(TBool aUseThread)
	{
	iUseThread = aUseThread;
	}

inline TBool TImage::StreamedDecode() const
	{
	return iStreamedDecode;
	}

inline void TImage::SetStreamedDecode(TBool aStreamedDecode)
	{
	iStreamedDecode = aStreamedDecode;
	}

inline CImageDecoder::TOptions TImage::DecoderOptions() const
	{
	CImageDecoder::TOptions options = iDecoderOptions;
	if(UseThread())
		options = CImageDecoder::EOptionAlwaysThread;
	if(StreamedDecode())
		options = static_cast<CImageDecoder::TOptions>(options|CImageDecoder::EOptionAllowZeroFrameOpen);
	return options;
	}

inline CImageEncoder::TOptions TImage::EncoderOptions() const
	{
	return UseThread() ? CImageEncoder::EOptionAlwaysThread : CImageEncoder::EOptionNone;
	}

#endif // __TIMAGE_H__
