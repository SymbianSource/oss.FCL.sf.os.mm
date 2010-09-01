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

#ifndef __JPEGCONVERT_H__
#define __JPEGCONVERT_H__

#include "icl/ImagePlugin.h"
#include "icl/ImageCodecData.h"

#include "JPEGExifPlugin.h"
#include <iclexifimageframe.h>
#include "JpegTypes.h"
#include "imageframe.h"
#include "pluginextensionmanager.h"

#include <icl/imageconversionextension.h>
#include <icl/imageconversionextensionintf.h>


// Decoder.
class CJpgReadCodec;
class CExifDecoder;
class CStreamedDecodeExtension;

class CJpegDecoder : public CJPEGImageFrameDecoderPlugin
	{
public:
 	static CJpegDecoder* NewL();
	~CJpegDecoder();

	void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aCodecType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

	TInt NumberOfFrameComments(TInt aFrameNumber) const;
	HBufC* FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const;

	// Extension operations
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);

	TInt HandleStreamGetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const;
	void HandleStreamInitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation);
	void HandleStreamGetBlocks(CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);
	void HandleStreamGetNextBlocks(CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks);

	// From CJPEGExifDecoderPlugin
	virtual MExifMetadata* ExifMetadata();
	virtual void SetClippingRectL(const TRect* aRect);
	virtual TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber);
	
	TInt SamplingScheme(TJpegImageData::TColorSampling& aSamplingScheme) const;

protected:
	void ScanDataL();
	TInt FrameHeaderBlockSize(TInt aFrameNumber) const;
	TInt FrameBlockSize(TInt aFrameNumber) const;

	// From CImageDecoderPlugin
	void InitConvertL();
	void DoConvert();
	void HandleCustomSyncL(TInt aParam);
	void InitCustomAsyncL(TInt aParam);
	void NotifyComplete();

private:
	CJpegDecoder();

	// From CImageDecoderPlugin
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();

	// New functions
	void LoadBlockL();
	void ProcessApp0L();
	void ProcessApp1L();
	void ProcessAppEL();
	void ProcessHuffmanTableL();
	void ProcessQTableL();
	void ProcessStartOfFrameL();
	void ProcessStartOfScanL();
	void ProcessRestartInterval();
	void ProcessCommentL();
	void FinishedProcessing();

	// Fill in image data structures, to be returned in calls to ImageData.
	void InitialiseImageDataL();

	// Handle async command for EOptionConvertFrame
	void HandleConvertFrameL();

	// Handle async command for EOptionContinueConvertFrame
	void HandleContinueConvertFrameL();

	// Check real size of comment block and adjust data read
	void CheckCommentBlockL();

protected:
	TUint16 iBlockSignature;
	TInt iBlockLength;
	TInt iImageOffset;
	const TUint8* iPtr;

	TJpgFrameInfo iJpgFrameInfo;
	TJpgScanInfo iScanInfo;
	TDecHuffmanTable iDCHuffmanTable[KJpgMaxNumberOfTables];
	TDecHuffmanTable iACHuffmanTable[KJpgMaxNumberOfTables];
	TQTable iQTable[KJpgMaxNumberOfTables];

	CExifDecoder* iExifDecoder;

	// Derived from the base class CJPEGExifDecoderPlugin.
	void NotifyImageTypeChangeL(TInt aImageType);

private:
	enum TDecodeState
		{
		EStateStart,
		EStateDrawFrame
		};

	TDecodeState iState;
	TFrameState iCodecState;

	TBool iJFIFMarkerPresent;
	TBool iAdobeMarkerPresent;
	TInt iAdobeTransform;

	RPointerArray<HBufC8> iComment;

	TInt iImageType;
	CPluginExtensionManager* iExtensionManager;	// owned
	CStreamedDecodeExtension* iStreamedDecodeExt;
	TUint16 iAutoRotateFlag;
	};

// Concrete Streamed Decode Extension class
class CStreamedDecodeExtension : public CBase,
								 public MImageConvStreamedDecode
	{
public:
	static CStreamedDecodeExtension* NewL(CJpegDecoder* aJpegDecoder);
	~CStreamedDecodeExtension();

	// From MImageConvStreamedDecode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const;
	TInt GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation);
	void GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);
	void GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks);

protected:
	CStreamedDecodeExtension(CJpegDecoder* aJpegDecoder);

private:
	CJpegDecoder* iJpegDecoder;
	TInt iReferenceCount;
	};

// CJpegEncoder
class CJpgWriteCodec;
class CExifEncoder;
class CBitmapScaler;
class CEncodeOperationExtension;
class CStreamedEncodeExtension;
class CJpegEncoder : public CJPEGImageFrameEncoderPlugin
	{
public:
	static CJpegEncoder* NewL();
	~CJpegEncoder();
	// From CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aFrameData);
	void UpdateHeaderL();
	void SetThumbnail(TBool aDoGenerateThumbnail);
	void Cleanup();
	MExifMetadata* ExifMetadata();

	// entension operations
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	//enum used to check and exclude interoperability of stream and operation interfaces.
	enum TExtensionOperation
		{
		ENone,
		EOperationEncode,
		EStreamEncode
		};
	CJpegEncoder::TExtensionOperation GetActiveExtensionOperation() const;
	void SetActiveExtensionOperation(CJpegEncoder::TExtensionOperation aExtOperation);

	void HandleStreamInitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, const CFrameImageData* aFrameImageData);
	void HandleStreamAppendBlock(const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void HandleStreamAddBlock(const CImageFrame& aBlocks, const TInt& aSeqPosition);
	void HandleStreamCompleteFrame();

protected:
	void WriteThumbnailL();
	void WriteExifDataL(TRequestStatus*& aScaleCompletionStatus);
	void InitConvertL();
	void InitCustomAsyncL(TInt aParam);

private:
	CJpegEncoder();
	void ConstructL();

	TInt WriteImageHeaderL();
	void CreateScaledBitmapL(TRequestStatus*& aScaleCompletionStatus);
	TInt WriteThumbnailDataL();
	TInt WriteThumbnailAsJfifL(CFbsBitmap& aBitmap);
	TInt WriteThumbnailAsExifL(CFbsBitmap& aBitmap);
	TInt WriteEOIL(const TInt aPosition);
	TInt WriteSOIL(const TInt aPosition);
	void TransformBitmapL(CFbsBitmap& aSource, CFbsBitmap& aDest);

	// Handle async command for EOptionConvertFrame
	void HandleConvertFrameL();

protected:
	CExifEncoder* iExifEncoder;

private:
	TJpgFrameInfo iFrameInfo;
	TInt iQualityFactor;
	TQTable *iLumaTable;
	TQTable *iChromaTable;
	RPointerArray<HBufC8> iComment;

	CFbsBitmap* iThumbnailImage;
	CFbsBitmap* iSourceThumbnailImage;
	CBitmapScaler* iBitmapScaler;

	TInt iExifDataPosition;
	TUint iThumbnailSize;
	TUint iExifSize;

	TBool iGenerateThumbnail;
	TInt  iThumbnailStatus;
	TSize iGeneratedThumbnailSize;

	CEncodeOperationExtension* iOperationExtPtr;
	CStreamedEncodeExtension* iStreamedEncodeExt;

	TExtensionOperation iExtOperationActive;
	};

// Concrete Encode Transform Extension class
class CEncodeOperationExtension : public CBase,
								  public MImageConvOperation
	{
friend class CJpegEncoder;

public:
	static CEncodeOperationExtension* NewL(CJpegEncoder* aJpegEncoder);
	~CEncodeOperationExtension();

	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();

protected:
	CEncodeOperationExtension(CJpegEncoder* aJpegEncoder);

private:
	CJpegEncoder* iJpegEncoder;
	TInt iReferenceCount;
	TUint iOperationCaps;
	RArray<TUint> iImgConvOperations;
	};

// Concrete Streamed Encode Extension class
class CStreamedEncodeExtension : public CBase,
								 public MImageConvStreamedEncode
	{
public:
	static CStreamedEncodeExtension* NewL(CJpegEncoder* aJpegEncoder);
	~CStreamedEncodeExtension();

	// From MImageConvStreamedEncode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	void AppendBlocks(TRequestStatus* aReq, const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void AddBlocks(TRequestStatus* aReq, const CImageFrame& aBlocks, const TInt& aSeqPosition);
	void Complete(TRequestStatus* aReq);

	void ValidateInitParamsL(TUid& aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
protected:
	CStreamedEncodeExtension(CJpegEncoder* aJpegEncoder);

private:
	CJpegEncoder* iJpegEncoder;
	TInt iReferenceCount;
	TEncodeStreamCaps iEncodeCaps;
	};

#endif // __JPEGCONVERT_H__

