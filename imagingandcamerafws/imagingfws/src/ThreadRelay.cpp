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

#include <caf/caf.h>
using namespace ContentAccess;
#include "ThreadRelay.h"
#include "icl/ImageConstruct.h"
#include "icl/ImagePlugin.h"
#include "ImageClientMain.h"
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "fwextconstants.h"
#include "iclextproxystubutil.h"

//
// CBufferCopyListener
//
/**
 * Factory function for this class.
 *
 * @param	"const RThread* aSubThread"
 *			Thread to signal when buffer copy has been completed
 */
CBufferCopyListener* CBufferCopyListener::NewL(const RThread* aSubThread)
	{
	CBufferCopyListener* self;
	self = new (ELeave) CBufferCopyListener(aSubThread);
	return self;
	}

/**
 * Constructor for this class.
 *
 * @param	"const RThread* aSubThread"
 *			Thread to signal when buffer copy has been completed
 */
CBufferCopyListener::CBufferCopyListener(const RThread* aSubThread) :
	CActive(EPriorityIdle)
	{
	iSubThread = aSubThread;
	CActiveScheduler::Add(this);
	}
/**
 * Activate the buffer copy listener to listen for copy requests.
 *
 */
void CBufferCopyListener::Prime()
	{
	ASSERT(IsActive() == EFalse);
	iStatus = KRequestPending;
	SetActive();
	}

/**
 * Initilise the listener for a buffer copy.
 *
 * @param	"TRequestStatus& aCallerStatus"
 *			Request to signal when copy has been comlpeted. Signalled in thread
 *			passed to the listener at construction.
 * @param	"TDes8& aBuffer"
 *			Destination buffer for copy.
 * @param	"const TDesC8& aSource"
 *			Source descriptor for copy.
 * @param	"TInt aPosition"
 *			Position in buffer to start copy.
 * @param	"TInt aSize"
 *			Number of elements to copy.
 */
void CBufferCopyListener::CopyBufferToDescriptor(TRequestStatus& aCallerStatus, TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize)
	{
	Prime();
	iCallerStatus = &aCallerStatus;
	iBuffer = &aBuffer;
	iSource = &aSource;
	iPosition = aPosition;
	iSize = aSize;
	}

/**
 *
 * Perform the buffer copy.
 *
 */
void CBufferCopyListener::RunL()
	{
	ASSERT(iStatus==KErrNone);
	DoBufferCopy();
	}

/**
 *
 * Perform the buffer copy and signal the requesting thread when the copy has completed.
 *
 */
void CBufferCopyListener::DoBufferCopy()
	{
	ASSERT(iStatus == KErrNone);
	ASSERT(iSource != NULL);
	ASSERT(iBuffer != NULL);
	ASSERT(iSubThread != NULL);

	*iBuffer = iSource->Mid(iPosition, iSize);

	TRequestStatus *status = iCallerStatus;
	iSubThread->RequestComplete(status,KErrNone);
	}

/**
 *
 * Cancel an outstanding copy request.
 *
 */
void CBufferCopyListener::DoCancel()
	{
	}
/**
 *
 * Destructor for this class.
 *
 */
CBufferCopyListener::~CBufferCopyListener()
	{
	}

//
// CThreadDecoderRelay
//
/**
 * Actual factory function for this class
 *
 * @param	"CImageDecodeConstruct* aConstruct"
 *			A object to construct a decoder.
 * @return	"CThreadDecoderRelay*"
 *			A pointer to the constructed object
 */
CThreadDecoderRelay* CThreadDecoderRelay::NewL(CImageDecodeConstruct* aConstruct)
	{
	CThreadDecoderRelay* self;
	self = new (ELeave) CThreadDecoderRelay(aConstruct);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Second phase constructor for this class.
 * Create a buffer copy listener and send it and a factory function
 * to create a decoder as parameters to the thread.
 *
 */
void CThreadDecoderRelay::ConstructL()
	{
	//Start a buffer copy listener and prime it to listen for requests
	iBufferCopyListener = CBufferCopyListener::NewL(SubThread());

	//Package the parameters to create a CSubThreadDecoderRelay
	TThreadDecoderRelayParam params;

	//Send the status of the buffer copy AO to the sub thread
	params.iBufferCopyListener = iBufferCopyListener;

	//Add the CImageDecodeConstruct object that will create the decoder
	params.iConstruct = iConstruct;

	CThreadRelay::ConstructL(CSubThreadDecoderRelay::NewL, &params);

	iSubThreadDecoderRelay = STATIC_CAST(CSubThreadDecoderRelay*, SubThreadRelay());
	iBody = iSubThreadDecoderRelay->iBody;
	iExtensionCache = CImageConvExtensionCache::NewL();
	}

/**
 * Constructor for this class.
 *
 */
CThreadDecoderRelay::CThreadDecoderRelay(CImageDecodeConstruct* aConstruct) :
	iConstruct(aConstruct)
	{
	}

/**
 * Destructor for this class.
 * If a buffer copy listener was created cancel and delete it.
 * If ownership for the construct object was passed, delete it.
 *
 */
CThreadDecoderRelay::~CThreadDecoderRelay()
	{
	delete iExtensionCache;
	if(iBufferCopyListener)
		{
		iBufferCopyListener->Cancel();
		delete iBufferCopyListener;
		}

	if(iOwnConstruct)
		delete iConstruct;
	}

/**
 * Run a function that should not leave in the codec thread.
 * Let the thread know a function call will follow (to allow the thread to do 
 * local buffer copies) and request the function to run. If the function does leave
 * panic the current thread.
 * 
 * @param	"TInt aFunctionCode"
 *			The function to run in the codec thread.
 * @param	"TAny* aParameters"
 *			The parameters to the function.
 * @panic	"KInvalidFunctionLeave"
 *			If the function in the codec thread leaves.
 */
void CThreadDecoderRelay::RunFunction(TInt aFunctionCode, TAny* aParameters)
	{
	ASSERT(iSubThreadDecoderRelay!=NULL);
	iSubThreadDecoderRelay->SetFunctionInProgress();
	TInt error = CThreadRelay::RunFunction(aFunctionCode, aParameters);
	iSubThreadDecoderRelay->ResetFunctionInProgress();

	if(error!=KErrNone)
		Panic(EInvalidFunctionLeave);
	}

/**
 * Run a function that can leave in the codec thread.
 * Let the thread know a function call will follow (to allow the thread to do 
 * local buffer copies) and request the function ro run. If the function do leave
 * leave in this thread with the same error code.
 * 
 * @param	"TInt aFunctionCode"
 *			The function to run in the codec thread.
 * @param	"TAny* aParameters"
 *			The parameters to the function.
 */
void CThreadDecoderRelay::RunFunctionL(TInt aFunctionCode, TAny* aParameters)
	{
	ASSERT(iSubThreadDecoderRelay!=NULL);
	iSubThreadDecoderRelay->SetFunctionInProgress();
	TInt error = CThreadRelay::RunFunction(aFunctionCode, aParameters);
	iSubThreadDecoderRelay->ResetFunctionInProgress();

	if(error!=KErrNone)
		User::Leave(error);
	}
/**
 * Cancel the threaded decoding in progress.
 * Use a straight call to request plugins that do not use AO functionallity
 * to abort and then request a normal decoding cancel in the codec thread.
 */
void CThreadDecoderRelay::Cancel()
	{
	//Use straight call to inform decoder to cancel
	iSubThreadDecoderRelay->AbortDecode();
	//Up the decoding thread's priority for increased Cancel() response
	SetPriority(RThread().Priority());
	//Cancel need no parameters
	RunFunction(CSubThreadDecoderRelay::EFunctionCancel, NULL);
	//Restore the priority
	SetPriority(KSubThreadPriority);
	}

void CThreadDecoderRelay::SetFileL(RFs& /*aFs*/, const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions)
	{
	TDecodeSetFileParams params;
	params.iSourceFilename = &aSourceFilename;
	params.iOptions = aOptions;

	RunFunctionL(CSubThreadDecoderRelay::EFunctionSetFile, &params);
	}
	
void CThreadDecoderRelay::SetFileL(RFile& aFile, const CImageDecoder::TOptions aOptions)
	{
	TDecodeSetFileHandleParams params;
	params.iFile = &aFile;
	params.iOptions = aOptions;

	RunFunctionL(CSubThreadDecoderRelay::EFunctionSetFileHandle, &params);
	}


void CThreadDecoderRelay::SetDataL(RFs& /*aFs*/, const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions)
	{
	TDecodeSetDataParams params;
	params.iSourceData = &aSourceData;
	params.iOptions = aOptions;

	RunFunctionL(CSubThreadDecoderRelay::EFunctionSetData, &params);
	}

CImageDecoderPlugin* CThreadDecoderRelay::Plugin() const
	{
	return iBody->Plugin();
	}

void CThreadDecoderRelay::HandleNewlyOpenedImageL()
	{
	RunFunctionL(CSubThreadDecoderRelay::EFunctionHandleNewImage, NULL);
	}

void CThreadDecoderRelay::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)
	{
	*aRequestStatus = KRequestPending;
	TDecodeConvertParams params;	
	params.iRequestStatus = aRequestStatus;
	params.iDestinationHandle = aDestination.Handle();
	params.iFrameNumber = aFrameNumber;

	RunFunction(CSubThreadDecoderRelay::EFunctionConvert, &params);
	}

void CThreadDecoderRelay::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber)
	{
	*aRequestStatus = KRequestPending;
	TDecodeConvertParams params;
	params.iRequestStatus = aRequestStatus;
	params.iDestinationHandle = aDestination.Handle();
	params.iDestinationMaskHandle = aDestinationMask.Handle();
	params.iFrameNumber = aFrameNumber;

	RunFunction(CSubThreadDecoderRelay::EFunctionConvertMask, &params);
	}

void CThreadDecoderRelay::ContinueConvert(TRequestStatus* aRequestStatus)
	{
	*aRequestStatus = KRequestPending;
	TDecodeConvertParams params;
	params.iRequestStatus = aRequestStatus;

	RunFunction(CSubThreadDecoderRelay::EFunctionContinueConvert, &params);
	}

void CThreadDecoderRelay::ContinueProcessingHeaderL()
	{
	RunFunctionL(CSubThreadDecoderRelay::EFunctionContinueHeader, NULL);
	}

void CThreadDecoderRelay::SetImageTypeL(TInt aImageType)
	{
	iBody->SetImageTypeL(aImageType);
	}

const TFrameInfo& CThreadDecoderRelay::FrameInfo(TInt aFrameNumber) const
	{
	iSubThreadDecoderRelay->Lock();
	const TFrameInfo& frameInfo = iBody->FrameInfo(aFrameNumber);
	iSubThreadDecoderRelay->Unlock();
	return frameInfo;
	}

const CFrameImageData& CThreadDecoderRelay::FrameData(TInt aFrameNumber) const
	{
	iSubThreadDecoderRelay->Lock();
	const CFrameImageData& frameData = iBody->FrameData(aFrameNumber);
	iSubThreadDecoderRelay->Unlock();
	return frameData;
	}

TInt CThreadDecoderRelay::NumberOfImageComments() const
	{
	iSubThreadDecoderRelay->Lock();
	TInt noOfImageComments = iBody->Plugin()->NumberOfImageComments();
	iSubThreadDecoderRelay->Unlock();
	return noOfImageComments;
	}

HBufC* CThreadDecoderRelay::ImageCommentL(TInt aCommentNumber) const
	{
	iSubThreadDecoderRelay->Lock();
	HBufC* imageComment = iBody->Plugin()->ImageCommentL(aCommentNumber);
	iSubThreadDecoderRelay->Unlock();
	return imageComment;
	}

TInt CThreadDecoderRelay::NumberOfFrameComments(TInt aFrameNumber) const
	{
	iSubThreadDecoderRelay->Lock();
	TInt noOfFrameComments = iBody->Plugin()->NumberOfFrameComments(aFrameNumber);
	iSubThreadDecoderRelay->Unlock();
	return noOfFrameComments;
	}

HBufC* CThreadDecoderRelay::FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const
	{
	iSubThreadDecoderRelay->Lock();
	HBufC* frameComment =  iBody->Plugin()->FrameCommentL(aFrameNumber, aCommentNumber);
	iSubThreadDecoderRelay->Unlock();
	return frameComment;
	}

CFrameInfoStrings* CThreadDecoderRelay::FrameInfoStringsLC(TInt aFrameNumber)
	{
	return iBody->FrameInfoStringsLC(aFrameNumber);
	}

TUid CThreadDecoderRelay::ImplementationUid() const
	{
	return iBody->ImplementationUid();
	}

TInt CThreadDecoderRelay::FrameCount() const
	{
	return iSubThreadDecoderRelay->FrameCount();
	}

TBool CThreadDecoderRelay::IsImageHeaderProcessingComplete() const
	{
	return iBody->IsImageHeaderProcessingComplete();
	}

void CThreadDecoderRelay::CustomSyncL(TInt aParam)
	{
	TCustomSyncParams params;
	params.iParam = aParam;

	RunFunctionL(CSubThreadDecoderRelay::EFunctionCustomSync, &params);
	}

void CThreadDecoderRelay::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	*aRequestStatus = KRequestPending;
	TCustomSyncParams params;
	params.iRequestStatus = aRequestStatus;
	params.iParam = aParam;

	RunFunction(CSubThreadDecoderRelay::EFunctionCustomAsync, &params);
	}

void CThreadDecoderRelay::TransferConstructOwnership()
	{
	iOwnConstruct = ETrue;
	}

void CThreadDecoderRelay::SetIntent(TIntent aIntent)
	{
	iBody->SetIntent(aIntent);
	}

void CThreadDecoderRelay::SetUniqueIdL(const TDesC& aUniqueId)
	{
	iBody->SetUniqueIdL(aUniqueId);
	}

TInt CThreadDecoderRelay::SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)
	{
	return iBody->SetAgentProperty(aProperty, aValue);
	}

TInt CThreadDecoderRelay::ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const
	{
	return iBody->ReductionFactor(aOriginalSize, aReducedSize);
	}
	
TInt CThreadDecoderRelay::ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const
	{
	return iBody->ReducedSize(aOriginalSize, aReductionFactor, aReducedSize);
	}
	
TInt CThreadDecoderRelay::SetDecoderThreadPriority(TThreadPriority aPriority)
	{
	SetPriority(aPriority);
	return KErrNone;
	}
	
void CThreadDecoderRelay::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	__ASSERT_ALWAYS(aExtPtr == NULL, Panic(ENonNullImageConvExtension));
	
	CImageConvProxyBase* proxy = ProxyStubUtility::GetNewProxyL(aExtUid, this);
	CleanupStack::PushL(proxy);
	
	TGetExtensionParams params;
	params.iUid = aExtUid;
	params.iExtension = &aExtPtr;
	RunFunctionL(CSubThreadDecoderRelay::EFunctionGetExtension, &params);
	proxy->SetupExtension(aExtPtr);
	iExtensionCache->SetProxyL(proxy);
	CleanupStack::Pop(proxy);
	//aExtPtr contains pointer to the proxy
	}

void CThreadDecoderRelay::SetClippingRectL(const TRect* aClipRect)
	{
	iBody->SetClippingRectL(aClipRect);
	}

TInt CThreadDecoderRelay::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	return iBody->Plugin()->GetDestinationSize(aSize, aFrameNumber);
	}

CImageConvExtensionCache& CThreadDecoderRelay::ExtensionCache()
	{
	return *iExtensionCache;
	}
	
void CThreadDecoderRelay::ExecuteCommand(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	RunFunction(CSubThreadDecoderRelay::EFunctionExecuteCommandL, &aExecuteCommandBase);
	}	

void CThreadDecoderRelay::ExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	RunFunctionL(CSubThreadDecoderRelay::EFunctionExecuteCommandL, &aExecuteCommandBase);
	}

void CThreadDecoderRelay::ExecuteAsyncCommand(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	RunFunction(CSubThreadDecoderRelay::EFunctionExecuteAsyncCommand, &aExecuteCommandBase);
	}
	
//
// CThreadEncoderRelay
//
/**
 * Actual factory function for this class
 *
 * @param	"CImageEncodeConstruct* aConstruct"
 *			A object to construct an encoder.
 * @return	"CThreadEncoderRelay*"
 *			A pointer to the constructed object
 */
CThreadEncoderRelay* CThreadEncoderRelay::NewL(CImageEncodeConstruct* aConstruct)
	{
	CThreadEncoderRelay* self;
	self = new (ELeave) CThreadEncoderRelay(aConstruct);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Constructor for this class.
 *
 */
CThreadEncoderRelay::CThreadEncoderRelay(CImageEncodeConstruct* aConstruct) :
	iConstruct(aConstruct)
	{
	}

/**
 * Second phase constructor for this class.
 * Send a factory function to create an encoder is as a parameter to the thread.
 *
 */
void CThreadEncoderRelay::ConstructL()
	{
	//Package the parameters to create a CSubThreadDecoderRelay
	TThreadEncoderRelayParam params;

	//Add the CImageEncodeConstruct object that will create the encoder
	params.iConstruct = iConstruct;

	CThreadRelay::ConstructL(CSubThreadEncoderRelay::NewL, &params);

	iSubThreadEncoderRelay = STATIC_CAST(CSubThreadEncoderRelay*, SubThreadRelay());
	iBody = iSubThreadEncoderRelay->iBody;
	iExtensionCache = CImageConvExtensionCache::NewL();
	}

void CThreadEncoderRelay::Cancel()
	{
	//Use straight call to inform encoder to cancel
	iSubThreadEncoderRelay->AbortEncode();
	//Up the encoding thread's priority for increased Cancel() response
	SetPriority(RThread().Priority());
	//Cancel needs no parameters
	RunFunction(CSubThreadEncoderRelay::EFunctionCancel, NULL);
	//Restore the priority
	SetPriority(KSubThreadPriority);
	}

void CThreadEncoderRelay::SetFileL(RFs& /*aFs*/, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions)
	{
	TEncodeSetFileParams params;
	params.iDestinationFilename = &aDestinationFilename;
	params.iOptions = aOptions;

	RunFunctionL(CSubThreadEncoderRelay::EFunctionSetFile, &params);
	}

void CThreadEncoderRelay::SetFileL(RFile& aFile, const CImageEncoder::TOptions aOptions)
	{
	TEncodeSetFileHandleParams params;
	params.iFile = &aFile;
	params.iOptions = aOptions;
	RunFunctionL(CSubThreadEncoderRelay::EFunctionSetFileHandle, &params);
	}

void CThreadEncoderRelay::SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions)
	{
	TEncodeSetDataParams params;
	params.iDestinationData = &aDestinationData;
	params.iOptions = aOptions;

	RunFunctionL(CSubThreadEncoderRelay::EFunctionSetData, &params);
	}

CImageEncoderPlugin* CThreadEncoderRelay::Plugin() const
	{
	return iBody->Plugin();
	}

void CThreadEncoderRelay::Convert(TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData)
	{
	*aRequestStatus = KRequestPending;
	TEncodeConvertParams params;
	params.iRequestStatus = aRequestStatus;
	params.iSourceHandle = aSource.Handle();
	params.iFrameImageData = aFrameImageData;

	RunFunction(CSubThreadEncoderRelay::EFunctionConvert, &params);
	}

TUid CThreadEncoderRelay::ImplementationUid() const
	{
	return iBody->ImplementationUid();
	}

void CThreadEncoderRelay::CustomSyncL(TInt aParam)
	{
	TCustomSyncParams params;
	params.iParam = aParam;

	RunFunctionL(CSubThreadEncoderRelay::EFunctionCustomSync, &params);
	}

void CThreadEncoderRelay::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	*aRequestStatus = KRequestPending;
	TCustomSyncParams params;
	params.iRequestStatus = aRequestStatus;
	params.iParam = aParam;

	RunFunction(CSubThreadEncoderRelay::EFunctionCustomAsync, &params);
	}

void CThreadEncoderRelay::TransferConstructOwnership()
	{
	iOwnConstruct = ETrue;
	}

void CThreadEncoderRelay::RunFunction(TInt aFunctionCode, TAny* aParameters)
	{
	ASSERT(iSubThreadEncoderRelay!=NULL);
	TInt error = CThreadRelay::RunFunction(aFunctionCode, aParameters);
	if(error!=KErrNone)
		Panic(EInvalidFunctionLeave);
	}

void CThreadEncoderRelay::RunFunctionL(TInt aFunctionCode, TAny* aParameters)
	{
	ASSERT(iSubThreadEncoderRelay!=NULL);
	TInt error = CThreadRelay::RunFunction(aFunctionCode, aParameters);
	if(error!=KErrNone)
		User::Leave(error);
	}

CThreadEncoderRelay::~CThreadEncoderRelay()
	{
	delete iExtensionCache;
	if(iOwnConstruct)
		delete iConstruct;
	}
void CThreadEncoderRelay::SetThumbnail(TBool aDoGenerateThumbnail)
	{
	iBody->SetThumbnail(aDoGenerateThumbnail);
	}

TInt CThreadEncoderRelay::SetEncoderThreadPriority(TThreadPriority aPriority)
	{
	SetPriority( aPriority );
	return KErrNone;
	}

void CThreadEncoderRelay::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	__ASSERT_ALWAYS(aExtPtr == NULL, Panic(ENonNullImageConvExtension));
	
	CImageConvProxyBase* proxy = ProxyStubUtility::GetNewProxyL(aExtUid, this);
	CleanupStack::PushL(proxy);
	
	TGetExtensionParams params;
	params.iUid = aExtUid;
	params.iExtension = &aExtPtr;
	RunFunctionL(CSubThreadEncoderRelay::EFunctionGetExtension, &params);
	proxy->SetupExtension(aExtPtr);
	iExtensionCache->SetProxyL(proxy);
	CleanupStack::Pop(proxy);
	//aExtPtr contains pointer to the proxy
	}

CImageConvExtensionCache& CThreadEncoderRelay::ExtensionCache()
	{
	return *iExtensionCache;
	}

void CThreadEncoderRelay::ExecuteCommand(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	RunFunction(CSubThreadEncoderRelay::EFunctionExecuteCommandL, &aExecuteCommandBase);
	}	

void CThreadEncoderRelay::ExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	RunFunctionL(CSubThreadEncoderRelay::EFunctionExecuteCommandL, &aExecuteCommandBase);
	}

void CThreadEncoderRelay::ExecuteAsyncCommand(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	RunFunction(CSubThreadEncoderRelay::EFunctionExecuteAsyncCommand, &aExecuteCommandBase);
	}

//
// CSubThreadDecoderRelay
//
/**
 * Actual factory function for this class
 * Decode the parameters and call the constructors.
 *
 */
CSubThreadRelay* CSubThreadDecoderRelay::NewL(TThreadId aMainThreadId, TAny* aDecoderParam)
	{
	CSubThreadDecoderRelay* self;
	TThreadDecoderRelayParam* decoderParam;

	decoderParam = STATIC_CAST(TThreadDecoderRelayParam*, aDecoderParam);

	self = new (ELeave) CSubThreadDecoderRelay(decoderParam->iBufferCopyListener);
	CleanupStack::PushL(self);
	self->ConstructL(aMainThreadId, decoderParam->iConstruct);

	CleanupStack::Pop(self);
	return self;
	}

/**
 * Constructor for this class.
 *
 */
CSubThreadDecoderRelay::CSubThreadDecoderRelay(CBufferCopyListener* aBufferCopyListener) :
	iBufferCopyListener(aBufferCopyListener)
	{
	}

/**
 * Second phase constructor for this class.
 * Create a local file server session (shareable between threads) and mutexes
 * to lock the frame tables and buffer copies.
 *
 * @param	"TThreadId aMainThreadId"
 *			The id of the main thread. Open a local connection to the main thread.
 * @param	"CImageDecodeConstruct* aConstruct"
 *			A construct object to create the decoder.
 */
void CSubThreadDecoderRelay::ConstructL(TThreadId aMainThreadId, CImageDecodeConstruct* aConstruct)
	{
	CSubThreadRelay::ConstructL(aMainThreadId);

	//Open a file server session
	User::LeaveIfError(iFileSession.Connect());
	//Make the session shareable for all threads
#ifdef __IPC_V2_PRESENT__
	User::LeaveIfError(iFileSession.ShareAuto());
#else
	User::LeaveIfError(iFileSession.Share(RSessionBase::EAutoAttach));
#endif

	//Create a mutex for the frame tables
	User::LeaveIfError(iFrameTableMutex.CreateLocal());
	//Create a mutex for buffer copies in main thread
	User::LeaveIfError(iBufferCopyMutex.CreateLocal());

	iBody = CImageDecoderPriv::NewL(aConstruct, this);
	iBody->CreatePluginL();
	}

/**
 * Destructor for this class.
 *
 */
CSubThreadDecoderRelay::~CSubThreadDecoderRelay()
	{
	Cancel();
	iStubs.ResetAndDestroy();
	if(iBody)
		{
		CancelBody();
		delete iBody;
		}
	iFrameTableMutex.Close();
	iBufferCopyMutex.Close();
	delete iDestination;
	delete iDestinationMask;
	iFileSession.Close();
	}

/**
 * Wait on buffer copy mutex for buffer copy in main thread
 * to finish and change the state to a function call in progress.
 * The codec subthread will use local buffer copies until ResetFunctionInProgress()
 * is called.
 *
 */
void CSubThreadDecoderRelay::SetFunctionInProgress()
	{
	//Note: assumes that this executes in the main thread.
	ASSERT(RThread().Id() == iMainThread.Id());

	//this will block until codec subthread has primed the buffer copy AO
	iBufferCopyMutex.Wait();
	if (iBufferCopyListener->IsActive())
		{
		iBufferCopyListener->Cancel();

		//Do copy now
		*iBuffer = iSource->Mid(iPosition, iSize);

		//Signal codec subthread that the copy is complete
		TRequestStatus* status = iCallerStatus;
		iSubThread->RequestComplete(status, KErrNone);
		}
	iFunctionCallInProgress = ETrue;
	iBufferCopyMutex.Signal();
	}

/**
 * Wait on buffer copy mutex while the codec thread check in which thread to
 * do the buffer copy and cancel the function call state.
 * The thread will do buffer copies in the main thread, using the buffer copy
 * listener after this call.
 *
 */
void CSubThreadDecoderRelay::ResetFunctionInProgress()
	{
	iBufferCopyMutex.Wait();
	iFunctionCallInProgress = EFalse;
	iBufferCopyMutex.Signal();
	}

/**
 * Run a function in the decoder thread.
 *
 * @param	"TInt aFunctionCode"
 *			The function to execute.
 * @param	"TAny* aParameters"
 *			Parameters to the function.
 * @leave	"KErrArgument"
 *			Invalid function requested.
 */
void CSubThreadDecoderRelay::RunFunctionL(TInt aFunctionCode, TAny* aParameters)
	{

	union
	{
	TAny* iFunctionParams;
	TDecodeSetFileParams* iSetFile;
	TDecodeSetFileHandleParams* iSetFileHandle;
	TDecodeSetDataParams* iSetData;
	TDecodeConvertParams* iConvert;
	TCustomSyncParams* iCustomSync;
	TGetExtensionParams* iGetExtension;
	TExecuteCommandParamsBase* iExecuteCommandBase;
	TExecuteCommandParamsAsyncBase* iExecuteCommandAsyncBase;
	} functionParams;

	functionParams.iFunctionParams = aParameters;

	switch(aFunctionCode)
		{
		case EFunctionSetFile:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			SetFileL(*functionParams.iSetFile->iSourceFilename,
					 functionParams.iSetFile->iOptions);
			break;
			}

		case EFunctionSetData:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			SetDataL(*functionParams.iSetData->iSourceData,
					 functionParams.iSetData->iOptions);
			break;
			}

		case EFunctionContinueHeader:
			{
			ContinueProcessingHeaderL();
			break;
			}

		case EFunctionConvert:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			Convert(functionParams.iConvert->iRequestStatus,
					functionParams.iConvert->iDestinationHandle,
					functionParams.iConvert->iFrameNumber);
			break;
			}

		case EFunctionConvertMask:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			Convert(functionParams.iConvert->iRequestStatus,
					functionParams.iConvert->iDestinationHandle,
					functionParams.iConvert->iDestinationMaskHandle,
					functionParams.iConvert->iFrameNumber);
			break;
			}

		case EFunctionContinueConvert:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			ContinueConvert(functionParams.iConvert->iRequestStatus);
			break;
			}

		case EFunctionHandleNewImage:
			{
			HandleNewlyOpenedImageL();
			break;
			}

		case EFunctionCustomSync:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			CustomSyncL(functionParams.iCustomSync->iParam);
			break;
			}

		case EFunctionCustomAsync:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			CustomAsync(functionParams.iCustomSync->iRequestStatus,
						functionParams.iCustomSync->iParam);
			break;
			}

		case EFunctionCancel:
			{
			CancelBody();
			break;
			}
		case EFunctionSetFileHandle:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			SetFileL(*functionParams.iSetFileHandle->iFile,
					 functionParams.iSetFileHandle->iOptions);
			break;
			}
		case EFunctionGetExtension:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			GetExtensionL(functionParams.iGetExtension->iUid,
						  *functionParams.iGetExtension->iExtension);
			break;
			}
		case EFunctionExecuteCommandL:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			HandleExecuteCommandL(*functionParams.iExecuteCommandBase);
			break;
			}
		case EFunctionExecuteAsyncCommand:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			HandleExecuteAsyncCommand(*functionParams.iExecuteCommandAsyncBase);
			break;
			}

		default:
			{
			User::Leave(KErrArgument);
			}
		}
	}

/**
 * Always use a buffer for copies in thread.
 *
 */
TBool CSubThreadDecoderRelay::MustUseBufferWithDescriptor() const
	{
	return ETrue;
	}

/**
 * Do a buffer copy. If the main thread is busy with a function call to this
 * thread, do a local copy, else request a copy from the buffer copy listener
 * in the main thread. The call will wait on the buffer copy mutex if the main thread
 * is busy to change the state to a function call in progress.
 *
 * @param	"TDes8& aBuffer"
 *			The destination buffer for the copy.
 * @param	"const TDesC8& aSource"
 *			The source descriptor for the copy.
 * @param	"TInt aPosition"
 *			Position to start copy.
 * @param	"TInt aSize"
 *			Number of elements to copy.
 */
void CSubThreadDecoderRelay::CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize)
	{
	//Note: assumes that this executes in the codec subthread.
	ASSERT(RThread().Id() == iSubThread->Id());

	//Wait for iFunctionCallInProgress change to complete with 
	//SetFunctionInProgress() or ResetFunctionInProgress()
	iBufferCopyMutex.Wait(); 
	if(iFunctionCallInProgress)
		{//Do copy in this thread
		//We do not need to keep the mutex if copy is done locally
		iBufferCopyMutex.Signal();
		//Do a local copy
		aBuffer = aSource.Mid(aPosition, aSize);
		}
	else
		{
		//Do a copy in the main thread
		ASSERT(iBufferCopyListener != NULL);
		TRequestStatus status = KRequestPending;

		//Prime AO in the main thread to do a buffer copy
		iBufferCopyListener->CopyBufferToDescriptor(status, aBuffer, aSource, aPosition, aSize);

		//Remember parameters in case we need to bypass the copy in main thread's AO
		iCallerStatus = &status;
		iBuffer = &aBuffer;
		iSource = &aSource;
		iPosition = aPosition;
		iSize = aSize;

		// Force main thread AO to be eligible for RunL() next time control is returned to it's AS
		TRequestStatus *bufferCopyStatus = &iBufferCopyListener->iStatus; 
		iMainThread.RequestComplete(bufferCopyStatus, KErrNone);

		//If a function call is made (in the main thread) before the CBufferCopyListener AO completes
		//then, SetFunctionInProgress() waits for this signal in the main thread because we don't
		//want to run a function while a buffer copy is in progess.
		//
		//We MUST now release the mutex BEFORE we suspend (this codec subthread) to allow the buffer copy
		//to progress in the main thread - either via CBufferCopyListener AO or in SetFunctionInProgress().
		iBufferCopyMutex.Signal();

		//Wait until main thread signals that buffer copy is complete
		User::WaitForRequest(status);
		}
	}

/**
 * @return	"TBool"
 *			Always ETrue since we are in a thread.
 */
TBool CSubThreadDecoderRelay::AmInThread() const
	{
	return ETrue;
	}

/**
 * Inform the decoder to cancel decoding if it does not use
 * ActiveObject functionallity. The decoder should constantly 
 * check ShouldAbort() and cancel decoding as quickly possible
 * if the flag is set.
 *
 */
void CSubThreadDecoderRelay::AbortDecode()
	{
	iAbortDecode = ETrue;
	}

/**
 * Cancel the decoding operation if one is in progress
 *
 */
void CSubThreadDecoderRelay::CancelBody()
	{
	if(iBody)
		iBody->Cancel();
	}

/**
 * Indicate to the plugin that the client requested a decode to terminate.
 *
 * @return	"TBool"
 *			ETrue if the plugin must terminate the decode process.
 */
TBool CSubThreadDecoderRelay::ShouldAbort() const
	{
	return iAbortDecode;
	}

void CSubThreadDecoderRelay::Lock()
	{
	iFrameTableMutex.Wait();
	}

void CSubThreadDecoderRelay::Unlock()
	{
	iFrameTableMutex.Signal();
	}

TInt CSubThreadDecoderRelay::FrameCount() const
	{
	return iFrameCount;
	}

void CSubThreadDecoderRelay::SaveFrameCount(TInt aFrameCount)
	{
	iFrameCount = aFrameCount;
	}
/**
 * Run CImageDecoderPriv::SetFileL() in the decoder thread,
 * using the thread's local file server session.
 *
 */
void CSubThreadDecoderRelay::SetFileL(const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions)
	{
	//Use sub thread RFs
	iBody->SetFileL(iFileSession, aSourceFilename, aOptions);
	}
void CSubThreadDecoderRelay::SetFileL(RFile& aFile, const CImageDecoder::TOptions aOptions)
	{
	iBody->SetFileL(aFile, aOptions);
	}

/**
 * Run CImageDecoderPriv::SetDataL() in the decoder thread,
 * using the thread's local file server session.
 *
 */
void CSubThreadDecoderRelay::SetDataL(const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions)
	{
	//Use sub thread RFs
	iBody->SetDataL(iFileSession, aSourceData, aOptions);
	}

/**
 * Run CImageDecoderPriv::Convert() in the decoder thread,
 * duplicating the client's image bitmap.
 *
 */
void CSubThreadDecoderRelay::Convert(TRequestStatus* aRequestStatus, TInt aDestinationHandle, TInt aFrameNumber)
	{
	iAbortDecode = EFalse;

	//Create a bitmap in the sub thread
	delete iDestination; iDestination = NULL;
	iDestination = new CFbsBitmap;
	if(iDestination == NULL)
		{
		iMainThread.RequestComplete(aRequestStatus, KErrNoMemory);
		return;
		}

	//Duplicate the client bitmap
	TInt error = iDestination->Duplicate(aDestinationHandle);
	if(error != KErrNone)
		{
		iMainThread.RequestComplete(aRequestStatus, error);
		return;
		}

	iBody->Convert(iMainThread, aRequestStatus, *iDestination, aFrameNumber);

	}

/**
 * Run CImageDecoderPriv::Convert() in the decoder thread,
 * duplicating the client's image and mask bitmap.
 *
 */
void CSubThreadDecoderRelay::Convert(TRequestStatus* aRequestStatus, TInt aDestinationHandle, TInt aDestinationMaskHandle, TInt aFrameNumber)
	{
	iAbortDecode = EFalse;
	
	//Create a bitmap in the sub thread
	delete iDestination; iDestination = NULL;
	iDestination = new CFbsBitmap;
	if(iDestination == NULL)
		{
		iMainThread.RequestComplete(aRequestStatus, KErrNoMemory);
		return;
		}

	//Duplicate the client bitmap
	TInt error = iDestination->Duplicate(aDestinationHandle);
	if(error != KErrNone)
		{
		iMainThread.RequestComplete(aRequestStatus, error);
		return;
		}

	//Create a mask bitmap in the sub thread
	delete iDestinationMask; iDestinationMask = NULL;
	iDestinationMask = new CFbsBitmap;
	if(iDestinationMask == NULL)
		{
		iMainThread.RequestComplete(aRequestStatus, KErrNoMemory);
		return;
		}

	//Duplicate the mask client bitmap
	error = iDestinationMask->Duplicate(aDestinationMaskHandle);
	if(error != KErrNone)
		{
		iMainThread.RequestComplete(aRequestStatus, error);
		return;
		}

	iBody->Convert(iMainThread, aRequestStatus, *iDestination, *iDestinationMask, aFrameNumber);
	}

void CSubThreadDecoderRelay::ContinueConvert(TRequestStatus* aRequestStatus)
	{
	iAbortDecode = EFalse;
	iBody->ContinueConvert(iMainThread, aRequestStatus);
	}

void CSubThreadDecoderRelay::HandleNewlyOpenedImageL()
	{
	iBody->HandleNewlyOpenedImageL();
	}

void CSubThreadDecoderRelay::ContinueProcessingHeaderL()
	{
	iBody->ContinueProcessingHeaderL();
	}

void CSubThreadDecoderRelay::CustomSyncL(TInt aParam)
	{
	iBody->CustomSyncL(aParam);
	}

void CSubThreadDecoderRelay::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	iBody->CustomAsync(iMainThread, aRequestStatus, aParam);
	}

void CSubThreadDecoderRelay::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	// Get the actual extension
	iBody->GetExtensionL(aExtUid, aExtPtr);
	CImageConvStubBase* stub = NULL;
	TRAPD(err, stub = ProxyStubUtility::NewStubL(aExtUid, aExtPtr, iBody, iMainThread));
	if(err != KErrNone)
		{
		delete stub;
		aExtPtr->Release();
		aExtPtr = NULL;
		User::Leave(err);
		}
	CleanupStack::PushL(stub);
	iStubs.AppendL(stub);
	CleanupStack::Pop(stub);
	aExtPtr = NULL;
	}
	
void CSubThreadDecoderRelay::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	for(TInt i = 0; i < iStubs.Count(); i++)
		{
		if((iStubs[i])->Uid() == aExecuteCommandBase.iUid)
			{
			(iStubs[i])->HandleExecuteCommandL(aExecuteCommandBase);
			return;
			}
		}
	ASSERT(EFalse);
	}
	
void CSubThreadDecoderRelay::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase)
	{
	for(TInt i = 0; i < iStubs.Count(); i++)
		{
		if((iStubs[i])->Uid() == aExecuteCommandBase.iUid)
			{
			(iStubs[i])->HandleExecuteAsyncCommand(aExecuteCommandBase);
			return;
			}
		}
	ASSERT(EFalse);
	}

//
//CSubThreadEncoderRelay
//
CSubThreadRelay* CSubThreadEncoderRelay::NewL(TThreadId aMainThreadId, TAny* aEncoderParam)
	{
	CSubThreadEncoderRelay* self;
	TThreadEncoderRelayParam* encoderParam;

	encoderParam = STATIC_CAST(TThreadEncoderRelayParam*, aEncoderParam);

	self = new (ELeave) CSubThreadEncoderRelay();
	CleanupStack::PushL(self);
	self->ConstructL(aMainThreadId, encoderParam->iConstruct);

	CleanupStack::Pop(self);
	return self;

	}

CSubThreadEncoderRelay::CSubThreadEncoderRelay()
	{
	}

void CSubThreadEncoderRelay::ConstructL(TThreadId aMainThreadId, CImageEncodeConstruct* aConstruct)
	{
	CSubThreadRelay::ConstructL(aMainThreadId);

	//Open a file server session
	User::LeaveIfError(iFileSession.Connect());
	//Make the session shareable for all threads
#ifdef __IPC_V2_PRESENT__
	User::LeaveIfError(iFileSession.ShareAuto());
#else
	User::LeaveIfError(iFileSession.Share(RSessionBase::EAutoAttach));
#endif
	iBody = CImageEncoderPriv::NewL(aConstruct, this);
	iBody->CreatePluginL();
	}

/**
 * Inform the encoder to cancel encoding if it does not use
 * ActiveObject functionallity. The encoder should constantly 
 * check ShouldAbort() and cancel encoding as quickly possible
 * if the flag is set.
 *
 */
void CSubThreadEncoderRelay::AbortEncode()
	{
	iAbortEncode = ETrue;
	}

void CSubThreadEncoderRelay::CancelBody()
	{
	if(iBody)
		iBody->Cancel();
	}

CSubThreadEncoderRelay::~CSubThreadEncoderRelay()
	{
	iStubs.ResetAndDestroy();
	if(iBody)
		{
		CancelBody();
		delete iBody;
		}
	delete iSource;
	iFileSession.Close();
	}

TBool CSubThreadEncoderRelay::AmInThread() const
	{
	return ETrue;
	}

TBool CSubThreadEncoderRelay::ShouldAbort() const
	{
	return iAbortEncode;
	}

void CSubThreadEncoderRelay::RunFunctionL(TInt aFunctionCode, TAny* aParameters)
	{
	union
	{
	TAny* iFunctionParams;
	TEncodeSetFileParams* iSetFile;
	TEncodeSetFileHandleParams* iSetFileHandle;
	TEncodeSetDataParams* iSetData;
	TEncodeConvertParams* iConvert;
	TCustomSyncParams* iCustomSync;
	TGetExtensionParams* iGetExtension;
	TExecuteCommandParamsBase* iExecuteCommandBase;
	TExecuteCommandParamsAsyncBase* iExecuteCommandAsyncBase;
	} functionParams;

	functionParams.iFunctionParams = aParameters;

	switch(aFunctionCode)
		{
		case EFunctionSetFile:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			SetFileL(*functionParams.iSetFile->iDestinationFilename,
					 functionParams.iSetFile->iOptions);
			break;
			}

		case EFunctionSetData:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			SetDataL(*functionParams.iSetData->iDestinationData,
					 functionParams.iSetData->iOptions);
			break;
			}

		case EFunctionConvert:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			Convert(functionParams.iConvert->iRequestStatus,
					functionParams.iConvert->iSourceHandle,
					functionParams.iConvert->iFrameImageData);
			break;
			}

		case EFunctionCustomSync:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			CustomSyncL(functionParams.iCustomSync->iParam);
			break;
			}

		case EFunctionCustomAsync:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			CustomAsync(functionParams.iCustomSync->iRequestStatus,
						functionParams.iCustomSync->iParam);
			break;
			}

		case EFunctionCancel:
			{
			CancelBody();
			break;
			}
		case EFunctionSetFileHandle:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			SetFileL(*functionParams.iSetFileHandle->iFile,
					 functionParams.iSetFile->iOptions);
			break;
			}
		case EFunctionGetExtension:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			GetExtensionL(functionParams.iGetExtension->iUid,
						  *functionParams.iGetExtension->iExtension);
			break;
			}
		case EFunctionExecuteCommandL:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			HandleExecuteCommandL(*functionParams.iExecuteCommandBase);
			break;
			}
		case EFunctionExecuteAsyncCommand:
			{
			ASSERT(functionParams.iFunctionParams != NULL);
			HandleExecuteAsyncCommand(*functionParams.iExecuteCommandAsyncBase);
			break;
			}
		default:
			{
			User::Leave(KErrArgument);
			}
		}
	}

void CSubThreadEncoderRelay::SetFileL(const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions)
	{
	//Use sub thread RFs
	iBody->SetFileL(iFileSession, aDestinationFilename, aOptions);
	}
	
void CSubThreadEncoderRelay::SetFileL(RFile& aFile, const CImageEncoder::TOptions aOptions)
	{
	iBody->SetFileL(aFile, aOptions);
	}

void CSubThreadEncoderRelay::SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions)
	{
	iBody->SetDataL(aDestinationData, aOptions);
	}

void CSubThreadEncoderRelay::Convert(TRequestStatus* aRequestStatus, TInt aSourceHandle, const CFrameImageData* aFrameImageData)
	{
	TInt error = KErrNone;

	iAbortEncode = EFalse;

	//Create a bitmap in the sub thread
	delete iSource; iSource = NULL;
	TRAP(error,iSource = new (ELeave) CFbsBitmap);
	if(error != KErrNone)
		{
		iMainThread.RequestComplete(aRequestStatus, error);
		return;
		}

	//Duplicate the client bitmap
	error = iSource->Duplicate(aSourceHandle);
	if(error != KErrNone)
		{
		iMainThread.RequestComplete(aRequestStatus, error);
		return;
		}

	iBody->Convert(iMainThread, aRequestStatus, *iSource, aFrameImageData);
	}

void CSubThreadEncoderRelay::CustomSyncL(TInt aParam)
	{
	iBody->CustomSyncL(aParam);
	}

void CSubThreadEncoderRelay::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	iBody->CustomAsync(iMainThread, aRequestStatus, aParam);
	}

void CSubThreadEncoderRelay::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	// Get the actual extension
	iBody->GetExtensionL(aExtUid, aExtPtr);
	CImageConvStubBase* stub = NULL;
	TRAPD(err,  stub = ProxyStubUtility::NewStubL(aExtUid, aExtPtr, iBody, iMainThread));
	if(err != KErrNone)
		{
		delete stub;
		aExtPtr->Release();
		aExtPtr = NULL;
		User::Leave(err);
		}
	CleanupStack::PushL(stub);
	iStubs.AppendL(stub);
	CleanupStack::Pop(stub);
	aExtPtr = NULL;
	}
	
void CSubThreadEncoderRelay::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	for(TInt i = 0; i < iStubs.Count(); i++)
		{
		if((iStubs[i])->Uid() == aExecuteCommandBase.iUid)
			{
			(iStubs[i])->HandleExecuteCommandL(aExecuteCommandBase);
			return;
			}
		}
	ASSERT(EFalse);
	}
	
void CSubThreadEncoderRelay::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase)
	{
	for(TInt i = 0; i < iStubs.Count(); i++)
		{
		if((iStubs[i])->Uid() == aExecuteCommandBase.iUid)
			{
			(iStubs[i])->HandleExecuteAsyncCommand(aExecuteCommandBase);
			return;
			}
		}
	ASSERT(EFalse);
	}


