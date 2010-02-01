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
// Not for public use
// These are structures for the Image Conversion Library
// to maintain future binary compatibility
// 
//

#ifndef IMAGECONVERSIONPRIV_H
#define IMAGECONVERSIONPRIV_H

#include "icl/ImageCodec.h"
#include "icl/ImageData.h"
#include "ImageConversion.h"
#include "ImagePrivSupport.h"
#include "icl/imageconversionextension.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/imagecodecdef.h>
#endif

#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)
#include "asyncfilewriter.h"
#endif

#include <caf/caf.h>
#include <caf/content.h>
#include <caf/data.h>

using namespace ContentAccess;

// incomplete forward declarations
class CImageDecoderPlugin;
class CImageEncoderPlugin;

class RCImageDecoderPrivContinueProcessingHeaderLTest; 

class CImageDataArray; // declared here
NONSHARABLE_CLASS( CImageDataArray ): public CBase
	{
public:
	~CImageDataArray();

	// Access fns.
	// Append/insert/delete
	TInt InsertImageData(const TImageDataBlock* aEntry, TInt aPos);
	TInt AppendImageData(const TImageDataBlock* aEntry);
	void RemoveImageData(TInt aIndex);

	// Get image data/local image data.
	const TImageDataBlock* GetImageData(TInt aIndex) const;
	TImageDataBlock* GetImageData(TInt aIndex);

	// Get num entries.
	TInt ImageDataCount() const;

	// Append image buffers.
	TInt AppendImageBuffer(const HBufC8* aImageBuffer);

private:
	// Image data for the frame. (Single frame formats)
	// Image data that is the same for all frames. (Multi frame formats)
	RPointerArray<TImageDataBlock> iImageData;

	// List of HBufC8 (Comments) owned by us.
	// Deleted when we are.
	RPointerArray<HBufC8> iImageBuffers;

friend class CFrameImageData;
	};

/*
Interface to allow proxy (from straight), or stub (from threaded) to set the
framework's TRequestStatus.
*/	
class MFrameworkExtAsyncInterface
	{
public:
	/*
	Used only in conjunction with asynchronous framework extension methods.
	It registers the calling thread's TRequestStatus with the framework, which is
	signalled when RequestComplete() is called, indicating an error code (including
	KErrNone) to the client.
	@param 	aRequestThread 	The calling thread.
	@param 	aRequestStatus	The calling thread's TRequestStatus.
	*/
	virtual void RegisterClientRequestStatus(RThread& aRequestThread, TRequestStatus* aRequestStatus) = 0;
	
	/*
	Used only in conjunction with asynchronous framework extension methods.
	Starts the framework AO, and thus the processing loop.
	*/
	virtual void StartActivity() = 0;
	};

// Properties for CImageDecoder
class CImageDecoderPriv; // declared here
NONSHARABLE_CLASS( CImageDecoderPriv ): public CActive,
										public MFrameworkExtAsyncInterface
	{
public:
	static CImageDecoderPriv* NewL(CImageDecodeConstruct *aConstruct, MImageDecoderPrivSupport* aSupport);
	virtual ~CImageDecoderPriv();

	void SetFileL(RFs& aFs, const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);
	void SetFileL(RFile& aFile, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);
	void SetDataL(RFs& aFs, const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);

	void CreatePluginL();
	CImageDecoderPlugin* Plugin() const;

	void HandleNewlyOpenedImageL();

	void InitConvertL();
	void RequestInitL(TInt aFrameNumber);
	void Convert(RThread& aRequestThread, TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber);
	void Convert(RThread& aRequestThread, TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber = 0);
	void ContinueConvert(RThread& aRequestThread, TRequestStatus* aRequestStatus);

	void DoConvert();
	void PrepareForProcessFrameL();
	void HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState);
	TBufPtr8& SourceData();

	void ContinueProcessingHeaderL();

	void ReadDataL(TInt aPosition, TPtrC8& aReadBuffer, TInt aLength);

	void CreateFrameInfoL();
	void DeleteFrameInfoL();
	void ReadFrameHeadersL();

	const TFrameInfo& FrameInfo(TInt aFrameNumber) const;
	CFrameInfoStrings* FrameInfoStringsLC(TInt aFrameNumber);

	void Cleanup();
	void RequestComplete(TInt aReason);
	void SelfComplete(TInt aReason);
	void SetSelfPending();

	TUid ImplementationUid() const;

	void SetImageTypeL(TInt aImageType);
	
	// custom command support
	void CustomSyncL(TInt aParam);
	void BodyHandleCustomSyncL(TInt aParam);
	void CustomAsync(RThread& aRequestThread, TRequestStatus* aRequestStatus, TInt aParam);
	void BodyInitCustomAsyncL(TInt aParam);
	void BodyNotifyComplete();

	// thread support to plugin
	TBool AmInThread() const;
	TBool ShouldAbort() const;

	// straight access functions decoder
	TInt FrameCount() const;
	TBool IsImageHeaderProcessingComplete() const;
	const CFrameImageData& FrameData(TInt aFrameNumber) const;

	// straight access functions from plugin
	CImageReadCodec* ImageReadCodec() const;
	void SetImageReadCodec(CImageReadCodec* aImageReadCodec);
	TInt DataLength() const;
	void SetDataLength(TInt aDataLength);
	TInt StartPosition() const;
	void SetStartPosition(TInt aStartPosition);
	const TImageDataBlock* ImageData(TInt aIndex) const;
	TInt InsertImageData(const TImageDataBlock* aEntry, TInt aPos);
	void RemoveImageData(TInt aPos);
	TInt AppendImageData(const TImageDataBlock* aEntry);
	TInt ImageDataCount() const;
	TInt AppendImageDataBuffer(const HBufC8* aImageBuffer);
	TInt NumberOfFrames() const;
	TInt Position() const;
	void SetPosition(const TInt aPosition);
	const TFrameInfo& ImageInfo() const;
	void SetImageInfo(const TFrameInfo& aImageInfo);
	const CFbsBitmap& Destination() const;
	TBool ValidDestination() const;
	const CFbsBitmap& DestinationMask() const;
	TBool ValidDestinationMask() const;
	TInt SourceLength() const;

	void SetIntent(TIntent aIntent);

	void SetUniqueIdL(const TDesC& aUniqueId);
	TInt SetAgentProperty(TAgentProperty aProperty, TInt aValue);

	void SetThumbnailData(HBufC8* aThumbnailData);
	
	TInt ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const;
	TInt ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const;
	
	inline CImageDecoder::TOptions DecoderOptions() const;
	
	// Framework extension
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	void SetClippingRectL(const TRect* aClipRect);
	TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber = 0);
	
	// From MFrameworkExtAsyncInterface
	void RegisterClientRequestStatus(RThread& aRequestThread, TRequestStatus* aRequestStatus);
	void StartActivity();
	
protected:
	TBool MustUseBufferWithDescriptor() const;
	void CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize);

private:

	CImageDecoderPriv(CImageDecodeConstruct* aConstruct, MImageDecoderPrivSupport* aSupport);

	void RunL(); // From CActive
	void DoCancel();

	void OpenExtraResourceFileLC(RFs& aFs, const TUid aUid, RResourceFile& aResourceFile) const;
	TInt DataSizeL();
	void SeekReadL(TInt aPosition, TDes8& aDest, TInt aSize);


private:
	// Frame info and frame data arrays. (Local frame settings)
	RPointerArray<TFrameInfo> iFrameInfo;
	RPointerArray<CFrameImageData> iFrameData;

	TFrameInfo iImageInfo;
	CImageDataArray iImageData;

	// Global frame settings 
	CFbsBitmap* iDestination;     // not owned
	CFbsBitmap* iDestinationMask; // not owned

	TInt iDataLength;
	TInt iStartPosition;
	TInt iPosition;
	TInt iSrcLength;
	TBufPtr8 iSrcDes;

	CImageReadCodec* iImageReadCodec;
	TBool iHaveCompleteImageHeaders;
	
	TInt iImageType;
	HBufC8* iThumbnailData;
	
private:
	RFs* iFs;
	CContent* iContent;
	CData* iData;
	TIntent iIntent;
	HBufC* iUniqueId;
	TImageParameterData iImageParameterData;
	TFileName iFileName;
	TInt iDestinationHandle;
	TInt iDestinationMaskHandle;

	HBufC8* iReadBuffer;

	TInt iNextFrameOffset;
	TBool iProcessingFrameInfo;

	TInt iBlockLength;
	TInt iCurrentBlockLength;
	TSize iDestinationSize; // used to check we don't continue with different bitmap
	TInt iOldSrcDesLength;

	CImageDecoder::TOptions iOptions;

	RThread* iRequestThread; // not owned
	TRequestStatus* iConvStatus; // not owned
	TInt iCurrentFrame;
	TBool iCurrentlyProcessingData;

	CImageDecodeConstruct* iConstruct; // not owned
	MImageDecoderPrivSupport* iSupport; // not owned
	CImageDecoderPlugin* iPlugin;
	TBool iIntentHasBeenExecuted;
	/** Size of data source in bytes.  Only valid if data source is read-only. */
	 TInt iCachedDataSize;
	 /** Start byte of last read from data file. */
	TInt iLastPos;
	/** Byte after last data block which was read from data source. */
	 TInt iLastExtent;
	 /** Flag to indicate if file input is read only or will grow  **/
	 TBool iReadOnly;
	 /** Flag to Check whether to Use Image buffer read optimization **/
	 TBool iUseBufferReadOptimization;

// allow direct access to unit tests.
friend class RCImageDecoderPrivTestBase;
friend class RCImageDecoderPrivContinueProcessingHeaderLTest;
	};

// forward declaration - gives access to internal destination buffer
class CImageEncoderPrivTestAccess;

// Properties for CImageEncoder
class CImageEncoderPriv; // declared here
NONSHARABLE_CLASS( CImageEncoderPriv ): public CActive,
										public MFrameworkExtAsyncInterface
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)
 ,protected MBufferWrittenObserver
#endif
	{
public:
	static CImageEncoderPriv* NewL(CImageEncodeConstruct* aConstruct, MImageEncoderPrivSupport* aSupport);
	virtual ~CImageEncoderPriv();

	void SetFileL(RFs& aFs, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions = CImageEncoder::EOptionNone);
	void SetFileL(RFile& aFile, const CImageEncoder::TOptions aOptions = CImageEncoder::EOptionNone);
	void SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions = CImageEncoder::EOptionNone);

	void CreatePluginL();
	CImageEncoderPlugin* Plugin() const;

	TInt CurrentImageSizeL() const;
	void Cleanup();
	void InitConvertL();
	void RequestInitL();
	void Convert(RThread& aRequestThread, TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData);
	void WriteDataL(TInt aPosition,const TDesC8& aDes);
	void WriteDataPositionIncL(TInt aPosition,const TDesC8& aDes);
	void DoConvert();
	void HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState);
	TBufPtr8& DestinationData();
	void DoCancel();
	void FinishConvertL();

	TUid ImplementationUid() const;

	// custom command support
	void CustomSyncL(TInt aParam);
	void BodyHandleCustomSyncL(TInt aParam);
	void CustomAsync(RThread& aRequestThread, TRequestStatus* aRequestStatus, TInt aParam);
	void BodyInitCustomAsyncL(TInt aParam);
	void BodyNotifyComplete();

	void RequestComplete(TInt aReason);
	void SelfComplete(TInt aReason);
	void SetSelfPending();

	// thread support to plugin
	TBool AmInThread() const;
	TBool ShouldAbort() const;

	// Data access from CImageEncoderPlugin
	CImageWriteCodec* ImageWriteCodec() const;
	void SetImageWriteCodec(CImageWriteCodec* aImageWriteCodec);
	const CFbsBitmap& Source() const;
	TBool ValidSource() const;
	TInt& StartPosition();
	TInt& Position();
	const TSize& FrameInfoOverallSizeInPixels() const;

	void SetThumbnail(TBool aDoGenerateThumbnail);
	
	inline CImageEncoder::TOptions EncoderOptions() const;
	
	// Framework extension
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	
	// From MFrameworkExtAsyncInterface
	void RegisterClientRequestStatus(RThread& aRequestThread, TRequestStatus* aRequestStatus);
	void StartActivity();
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)		
protected:
    void DoHandleProcessFrameResultL(TFrameState aCodecState);
    
    // from the MBufferWrittenObserver
    virtual void BufferWritten(const TPtrC8& aWritten, TInt aErrorCode);
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

private:
	CImageEncoderPriv(CImageEncodeConstruct* aConstruct, MImageEncoderPrivSupport* aSupport);

	void RunL();

private:
	RThread* iRequestThread; // not owned
	TRequestStatus* iConvStatus; // not owned
	TFrameInfo iFrameInfo;
	CImageWriteCodec* iImageWriteCodec;
	TBufPtr8 iDstDes;
	HBufC8* iDstBuffer;
	HBufC8* iFinalDstBuffer;
	HBufC8** iDestination;
	TInt iStartPosition;
	TInt iPosition;
	
	TBool iHeaderWritten;	// describes if the header has been written or not

	CImageEncoder::TOptions iOptions;

	RFs* iFs;
	RFile iFile;
	TFileName iFileName;

	const CFbsBitmap* iSource;
	TImageParameterData iImageParameterData;
	TBool iConvertStarted;

private:
	CImageEncodeConstruct* iConstruct; // not owned
	MImageEncoderPrivSupport* iSupport;
	CImageEncoderPlugin* iPlugin;
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	CAsyncFileWriter* iAsyncFileWriter;
	TInt iWriterError;
	
#else
	TBool iLastBuffer;
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

// allow this class access to internal buffers
friend class CImageEncoderPrivTestAccess;
	};

#include "ImageConversionPriv.inl"


#endif // IMAGECONVERSIONPRIV_H

