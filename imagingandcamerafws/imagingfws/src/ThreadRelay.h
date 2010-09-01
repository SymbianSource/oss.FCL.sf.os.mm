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
// Abstract threaded relay between Image{Decoder,Encoder} and Priv object
// 
//

#ifndef __THREAD_RELAY_H__
#define __THREAD_RELAY_H__

#include "GenThreadRelay.h"
#include "ImageRelay.h"
#include "iclextproxystubutil.h"

class TCustomSyncParams
	{
public:
	TRequestStatus* iRequestStatus;
	TInt iParam;
	};

class TDecodeSetFileParams
	{
public:
	const TDesC* iSourceFilename;
	CImageDecoder::TOptions iOptions;
	};
class TDecodeSetFileHandleParams
	{
public:
	RFile* iFile;
	CImageDecoder::TOptions iOptions;		
	};

class TDecodeSetDataParams
	{
public:
	const TDesC8* iSourceData;
	CImageDecoder::TOptions iOptions;	
	};

class TDecodeConvertParams
	{
public:
	TRequestStatus* iRequestStatus;
	TInt iDestinationHandle;
	TInt iDestinationMaskHandle;
	TInt iFrameNumber;
	};

class TEncodeSetFileParams
	{
public:
	const TDesC* iDestinationFilename;
	CImageEncoder::TOptions iOptions;
	};

class TEncodeSetFileHandleParams
	{
public:
	RFile* iFile;
	CImageEncoder::TOptions iOptions;		
	};

class TEncodeSetDataParams
	{
public:
	HBufC8** iDestinationData;
	CImageEncoder::TOptions iOptions;	
	};

class TEncodeConvertParams
	{
public:
	TRequestStatus* iRequestStatus;
	TInt iSourceHandle;
	const CFrameImageData* iFrameImageData;
	};

class CBufferCopyListener; // declared here
/**
 *
 * This class is not for public use
 * To avoid simultaneous update problems on buffer copy on descriptors,
 * this will be done in the main thread. This AO wait for buffer copy requests
 * and do the copy in the main thread.
 *
 */
NONSHARABLE_CLASS( CBufferCopyListener ): public CActive
	{
public:
	static CBufferCopyListener* NewL(const RThread* aSubThread);
	virtual ~CBufferCopyListener();

	void CopyBufferToDescriptor(TRequestStatus& aCallerStatus, TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize);

protected:
	//From CActive
	void RunL();
	void DoCancel();

private:
	CBufferCopyListener(const RThread* aSubThread);
	void Prime();
	void DoBufferCopy();

private:
	TRequestStatus* iCallerStatus; //not owned
	TDes8* iBuffer; //not owned
	const TDesC8* iSource; //not owned
	TInt iPosition;
	TInt iSize;
	const RThread* iSubThread; //not owned
	};

/**
 *
 * This class is not for public use
 * Implement client side thread interface for threaded De/Encoding
 *
 */
class CImageConvOperationStub;
class CImageConvScalerStub;
class CImageConvStreamedDecodeStub;
class CThreadDecoderRelay;
class CSubThreadDecoderRelay; // declared here
NONSHARABLE_CLASS( CSubThreadDecoderRelay ): public CSubThreadRelay,
							                 public MImageDecoderPrivSupport
	{
public:
	enum TFunctionCode
		{
		EFunctionTerminate = KTerminationFunction,
		EFunctionCancel,
		EFunctionSetFile,
		EFunctionSetData,
		EFunctionContinueHeader,
		EFunctionConvert,
		EFunctionConvertMask,
		EFunctionContinueConvert,
		EFunctionHandleNewImage,
		EFunctionCustomSync,
		EFunctionCustomAsync,
		EFunctionSetFileHandle,
		EFunctionGetExtension,
		EFunctionExecuteCommandL,
		EFunctionExecuteAsyncCommand
		};

public:
	static CSubThreadRelay* NewL(TThreadId aMainThreadId, TAny* aDecoderParam);
	virtual ~CSubThreadDecoderRelay();

	void SetFunctionInProgress();
	void ResetFunctionInProgress();

	void AbortDecode();
	void CancelBody();

	TInt FrameCount() const;

	void SetFileL(const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions);
	void SetFileL(RFile& aFile, const CImageDecoder::TOptions aOptions);
	void SetDataL(const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions);

	void ContinueProcessingHeaderL();

	void Convert(TRequestStatus* aRequestStatus, TInt aDestinationHandle, TInt aFrameNumber);
	void Convert(TRequestStatus* aRequestStatus, TInt aDestinationHandle, TInt aDestinationMaskHandle, TInt aFrameNumber);
	void ContinueConvert(TRequestStatus* aRequestStatus);

	void HandleNewlyOpenedImageL();

	void CustomSyncL(TInt aParam);
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);
	
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

	//From MImageDecoderPrivSupport
	TBool MustUseBufferWithDescriptor() const;
	void CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize);
	TBool AmInThread() const;
	TBool ShouldAbort() const;
	void Lock();
	void Unlock();
	void SaveFrameCount(TInt aFrameCount);

	//From MImageDecoderPriv
	void SetIntent(TIntent aIntent);
	void SetUniqueIdL(const TDesC& aUniqueId);
	TInt SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue);

protected:
	//From CThreadRelay
	void RunFunctionL(TInt aFunctionCode, TAny* aParameters);

private:
	CSubThreadDecoderRelay(CBufferCopyListener* aBufferCopyListener);
	void ConstructL(TThreadId aMainThreadId, CImageDecodeConstruct* aConstruct);

private:
	CImageDecoderPriv* iBody;

	CFbsBitmap* iDestination; //Duplicate the client bitmap in this thread
	CFbsBitmap* iDestinationMask; //Duplicate the client mask bitmap in this thread
	RFs iFileSession; //This thread uses its own file session
	TInt iFrameCount;
	TBool iAbortDecode;
	RMutex iFrameTableMutex;

	RMutex iBufferCopyMutex;
	TBool iFunctionCallInProgress; //ETrue if a function call to this thread is in progress

	CBufferCopyListener* iBufferCopyListener; //not owned 

	TRequestStatus* iCallerStatus; //not owned
	TDes8* iBuffer; //not owned
	const TDesC8* iSource; //not owned
	TInt iPosition;
	TInt iSize;
	
	// Framework Extension stubs
	RPointerArray<CImageConvStubBase> iStubs;
	
friend class CThreadDecoderRelay;
	};

/**
 *
 * This class is not for public use
 * Implement client side thread interface for threaded De/Encoding
 *
 */
class CThreadEncoderRelay;
class CSubThreadEncoderRelay;
NONSHARABLE_CLASS( CSubThreadEncoderRelay ): public CSubThreadRelay,
							                 public MImageEncoderPrivSupport
	{
public:
	enum TFunctionCode
		{
		EFunctionTerminate = KTerminationFunction,
		EFunctionCancel,
		EFunctionSetFile,
		EFunctionSetData,
		EFunctionConvert,
		EFunctionCustomSync,
		EFunctionCustomAsync,
		EFunctionSetFileHandle,
		EFunctionGetExtension,
		EFunctionExecuteCommandL,
		EFunctionExecuteAsyncCommand
		};

public:
	static CSubThreadRelay* NewL(TThreadId aMainThreadId, TAny* aEncoderParam);
	virtual ~CSubThreadEncoderRelay();

	void AbortEncode();
	void CancelBody();

	void SetFileL(const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions);
	void SetFileL(RFile& aFile, const CImageEncoder::TOptions aOptions);
	void SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions);
	void Convert(TRequestStatus* aRequestStatus, TInt aSourceHandle, const CFrameImageData* aFrameImageData);

	void CustomSyncL(TInt aParam);
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);
	
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

	//From MImageEncoderPrivSupport
	TBool AmInThread() const;
	TBool ShouldAbort() const;

protected:
	virtual void RunFunctionL(TInt aFunctionCode, TAny* aParameters);

private:
	CSubThreadEncoderRelay();
	void ConstructL(TThreadId aMainThreadId, CImageEncodeConstruct* aConstruct);

private:
	CImageEncoderPriv* iBody;

	CFbsBitmap* iSource;
	RFs iFileSession; //This thread use its own file session
	TBool iAbortEncode;
	
	// Framework Extension stubs
	RPointerArray<CImageConvStubBase> iStubs;

friend class CThreadEncoderRelay;
	};

class CThreadDecoderRelay;
/**
 *
 * This class is not for public use
 * Implement client side thread interface for threaded Decoding
 *
 */
NONSHARABLE_CLASS( CThreadDecoderRelay ): public CThreadRelay,
							              public MImageDecoderRelay,
							              public MImageConvExtensionThreadedSupport
	{
public:
	static CThreadDecoderRelay* NewL(CImageDecodeConstruct* aConstruct);
	virtual ~CThreadDecoderRelay();

	//From MImageDecoderRelay
	void Cancel();

	void SetFileL(RFs& aFs, const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);
	void SetFileL(RFile& name, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);
	void SetDataL(RFs& aFs, const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);

	CImageDecoderPlugin* Plugin() const;

	void HandleNewlyOpenedImageL();

	void Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber);
	void Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber = 0);
	void ContinueConvert(TRequestStatus* aRequestStatus);

	void ContinueProcessingHeaderL();

	const TFrameInfo& FrameInfo(TInt aFrameNumber) const;
	CFrameInfoStrings* FrameInfoStringsLC(TInt aFrameNumber);

	TUid ImplementationUid() const;

	TInt FrameCount() const;
	TBool IsImageHeaderProcessingComplete() const;
	const CFrameImageData& FrameData(TInt aFrameNumber) const;

	TInt NumberOfImageComments() const;
	HBufC* ImageCommentL(TInt aCommentNumber) const;
	TInt NumberOfFrameComments(TInt aFrameNumber) const;
	HBufC* FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const;

	// custom command support
	void CustomSyncL(TInt aParam);
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);

	void TransferConstructOwnership();

	void SetIntent(TIntent aIntent);
	void SetUniqueIdL(const TDesC& aUniqueId);
	TInt SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue);
	
	void SetImageTypeL(TInt aImageType);

	TInt ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const;
	TInt ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const;
	
	TInt SetDecoderThreadPriority(TThreadPriority aPriority);
	
	// from MImageDecoderRelay
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	void SetClippingRectL(const TRect* aClipRect);
	TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber = 0);
	CImageConvExtensionCache& ExtensionCache();
	
	// from MImageConvExtensionThreadedSupport
	void ExecuteCommand(TExecuteCommandParamsBase& aExecuteCommandBase);
	void ExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void ExecuteAsyncCommand(TExecuteCommandParamsBase& aExecuteCommandBase);

	
private:
	CThreadDecoderRelay(CImageDecodeConstruct* aConstruct);
	void ConstructL();

	void RunFunction(TInt aFunctionCode, TAny* aParameters);
	void RunFunctionL(TInt aFunctionCode, TAny* aParameters);

private:
	CSubThreadDecoderRelay* iSubThreadDecoderRelay; //not owned
	CImageDecoderPriv* iBody; //not owned (for straight calls)
	TInt iOwnConstruct;
	CImageDecodeConstruct* iConstruct;
	CBufferCopyListener* iBufferCopyListener; //AO in main thread that listen for requests from sub thread
	CImageConvExtensionCache* iExtensionCache;
	};

class CThreadEncoderRelay;
/**
 *
 * This class is not for public use
 * Implement client side thread interface for threaded Encoding
 *
 */
NONSHARABLE_CLASS( CThreadEncoderRelay ): public CThreadRelay,
							              public MImageEncoderRelay,
							              public MImageConvExtensionThreadedSupport
	{
public:
	static CThreadEncoderRelay* NewL(CImageEncodeConstruct* aConstruct);
	virtual ~CThreadEncoderRelay();

	//From MImageEncoderRelay
	void Cancel();

	void SetFileL(RFs& aFs, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions);
	void SetFileL(RFile& name, const CImageEncoder::TOptions aOptions);
	void SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions);

	CImageEncoderPlugin* Plugin() const;

	void Convert(TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData);

	TUid ImplementationUid() const;
	
	// Framework extension
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	CImageConvExtensionCache& ExtensionCache();

	// custom command support
	void CustomSyncL(TInt aParam);
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);

	void TransferConstructOwnership();

	void SetThumbnail(TBool aDoGenerateThumbnail);

	TInt SetEncoderThreadPriority(TThreadPriority aPriority);
	
	// from MImageConvExtensionThreadedSupport
	void ExecuteCommand(TExecuteCommandParamsBase& aExecuteCommandBase);
	void ExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void ExecuteAsyncCommand(TExecuteCommandParamsBase& aExecuteCommandBase);
	
private:
	CThreadEncoderRelay(CImageEncodeConstruct* aConstruct);
	void ConstructL();

	void RunFunction(TInt aFunctionCode, TAny* aParameters);
	void RunFunctionL(TInt aFunctionCode, TAny* aParameters);

private:
	TInt iOwnConstruct;
	CImageEncodeConstruct* iConstruct;
	CSubThreadEncoderRelay* iSubThreadEncoderRelay; //Not owned
	CImageEncoderPriv* iBody; //not owned (for straight calls)
	CImageConvExtensionCache* iExtensionCache;
	};

/**
 *
 * This class is not for public use
 *
 */
class TThreadDecoderRelayParam
	{
public:
	CImageDecodeConstruct* iConstruct;
	CBufferCopyListener* iBufferCopyListener;
	};

/**
 *
 * This class is not for public use
 *
 */
class TThreadEncoderRelayParam
	{
public:
	CImageEncodeConstruct* iConstruct;
	};

#endif // __THREAD_RELAY_H__

