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
//

#ifndef ICLEXTPROXYSTUBUTIL_H
#define ICLEXTPROXYSTUBUTIL_H

#include <icl/imageconversionextensionintf.h>

// forward declarations
class TExecuteCommandParamsBase;
class TExecuteCommandParamsAsyncBase;
class CThreadDecoderRelay;
class CThreadEncoderRelay;
class MFrameworkExtAsyncInterface;
class CImageConvProxyBase;
class CImageConvStubBase;
class MImageConvExtensionThreadedSupport;

/*
Static class that creates client-side proxy and plugin-side stub objects
required for marshalling data across the ICL framework.
*/
class ProxyStubUtility
	{
public:
	static CImageConvProxyBase* GetNewProxyL(TUid aUid, MImageConvExtensionThreadedSupport* aParent);
	static CImageConvProxyBase* GetNewStraightProxyL(TUid aUid, MFrameworkExtAsyncInterface* aStatusOwner,
												  	 RThread& aRequestThread);
	static CImageConvStubBase* NewStubL(TUid aUid,
										MImageConvExtension* aExtension,
										MFrameworkExtAsyncInterface* aStatusOwner,
										RThread& aRequestThread);
	};

/*
Used by the proxy/stub to identify the function call implementation of the
MImageConvExtension interface.
*/
enum TImgConvExtCmd
	{
	EImgConvExtUid = 0,
	EImgConvExtIncrementRef = 1,
	EImgConvExtRelease = 2
	};

/*
Used by the proxy/stub to identify the function call implementation of the
MImageConvOperation interface.
*/
enum TImgConvTransformExtCmd
	{
	EImgConvExtCapabilities = EImgConvExtRelease+1,
	EImgConvExtAddOperationL = EImgConvExtRelease+2,
	EImgConvExtClearOperationStack = EImgConvExtRelease+3
	};

/*
Used by the proxy/stub to identify the function call implementation of the
MImageConvScaler interface.
*/
enum TImgConvScalerExtCmd
	{
	EImgConvExtGetCapabilitiesScaler = EImgConvExtRelease+1,
	EImgConvExtSetScalingLSize = EImgConvExtRelease+2,
	EImgConvExtSetScalingLCoeff = EImgConvExtRelease+3
	};
	
/*
Used by the proxy/stub to identify the function call implementation of the
MImageConvStreamedDecode interface.
*/
enum TImgConvStreamedDecodeExtCmd
	{
	EImgConvExtGetBlocks = EImgConvExtRelease+1,
	EImgConvExtGetBufferSize = EImgConvExtRelease+2,
	EImgConvExtGetNextBlocks = EImgConvExtRelease+3,
	EImgConvExtGetCapabilitiesStreamDec = EImgConvExtRelease+4,
	EImgConvExtGetSupportedFormatsLStreamDec = EImgConvExtRelease+5,
	EImgConvExtInitFrameLStreamDec = EImgConvExtRelease+6
	};
	
/*
Used by the proxy/stub to identify the function call implementation of the
MImageConvStreamedEncode interface.
*/
enum TImgConvStreamedEncodeExtCmd
	{
	EImgConvExtGetSupportedFormatsLStreamEnc = EImgConvExtRelease+1,
	EImgConvExtGetCapabilitiesStreamEnc = EImgConvExtRelease+2,
	EImgConvExtInitFrameLStreamEnc = EImgConvExtRelease+3,
	EImgConvExtAppendBlocks = EImgConvExtRelease+4,
	EImgConvExtAddBlocks = EImgConvExtRelease+5,
	EImgConvExtComplete = EImgConvExtRelease+6
	};

/*
Used by the proxy/stub to identify the function call implementation of the
MImageConvPrepare interface.
*/
enum TImgConvPrepareExtCmd
	{
	EImgConvExtPrepare = EImgConvExtRelease+1
	};
	
/*
Abstract class that all Proxy classes inherit from.  Permits the
CImageConvExtensionCache to cleanup the Proxys correctly.

@see CImageConvExtensionCache
*/
NONSHARABLE_CLASS( CImageConvProxyBase ) : public CBase
	{
public:
	virtual ~CImageConvProxyBase() {};
	virtual void SetupExtension(MImageConvExtension*& aExtension) = 0;
	};

/*
Abstract class that all Stub classes inherit from.  Permits the
CSubThreadEncoderRelay to cleanup the Stubs correctly.

@see CSubThreadEncoderRelay
*/
NONSHARABLE_CLASS( CImageConvStubBase ) : public CBase
	{
public:
	virtual TUid Uid() const = 0;
	virtual void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase) = 0;
	virtual void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase) = 0;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvOperation implementation.

Used only in the single-threaded operations.

@see MImageConvOperation
*/
NONSHARABLE_CLASS( CImageConvOperationStraightProxy ) :	public CImageConvProxyBase,
								 						public MImageConvOperation
	{
public:
	static CImageConvOperationStraightProxy* NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvOperationStraightProxy();
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();

private:
	CImageConvOperationStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvOperation* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};	

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvOperation implementation.

Used only in the threaded operations.

@see MImageConvOperation
@see CImageConvOperationStub
*/
NONSHARABLE_CLASS( CImageConvOperationProxy ) :	public CImageConvProxyBase,
								 				public MImageConvOperation
	{
public:
	static CImageConvOperationProxy* NewL(MImageConvExtensionThreadedSupport* aParent);
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();

private:
	explicit CImageConvOperationProxy(MImageConvExtensionThreadedSupport* aParent);
	
private:
	MImageConvExtensionThreadedSupport* iParent;
	};

/*
Plugin-side stub class that relays calls from the operation proxy to the
MImageConvOperation implementation.

Used only in the threaded operations.

@see MImageConvOperation
@see CImageConvOperationProxy
*/
NONSHARABLE_CLASS( CImageConvOperationStub ) :	public CImageConvStubBase
	{	
public:
	static CImageConvOperationStub* NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvOperationStub();
	
	// From CImageConvStubBase
	TUid Uid() const;
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

private:
	explicit CImageConvOperationStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvOperation* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*

Supports ICL client side extensions in calling the plugin-side
MImageConvScaler implementation.

Used only in the single-threaded operations.

@see MImageConvScaler
*/
NONSHARABLE_CLASS( CImageConvScalerStraightProxy ) : public CImageConvProxyBase, 
							  				 		 public MImageConvScaler
	{
public:
	static CImageConvScalerStraightProxy* NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvScalerStraightProxy();
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvScaler
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetCapabilities(TScalerCaps& aCaps) const;
	void SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	void SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);

private:
	CImageConvScalerStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvScaler* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvScaler implementation.

Used only in the threaded operations.

@see MImageConvScaler
@see CImageConvScalerStub
*/
NONSHARABLE_CLASS( CImageConvScalerProxy ) : public CImageConvProxyBase, 
							  				 public MImageConvScaler
	{
public:
	static CImageConvScalerProxy* NewL(MImageConvExtensionThreadedSupport* aParent);
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvScaler
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetCapabilities(TScalerCaps& aCaps) const;
	void SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	void SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);

private:
	explicit CImageConvScalerProxy(MImageConvExtensionThreadedSupport* aParent);
	
private:
	MImageConvExtensionThreadedSupport* iParent;
	};

/*
Plugin-side stub class that relays calls from the scaler proxy to the
MImageConvScaler implementation.

Used only in the threaded operations.

@see MImageConvScaler
@see CImageConvScalerProxy
*/
NONSHARABLE_CLASS( CImageConvScalerStub ) :	public CImageConvStubBase
	{	
public:
	static CImageConvScalerStub* NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvScalerStub();
	
	// From CImageConvStubBase
	TUid Uid() const;
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

private:
	explicit CImageConvScalerStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvScaler* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvStreamedDecode implementation.

Used only in the single-threaded operations.

@see MImageConvStreamedDecode
*/
NONSHARABLE_CLASS( CImageConvStreamedDecodeStraightProxy ) : public CImageConvProxyBase,
									  				 		 public MImageConvStreamedDecode
	{
public:
	static CImageConvStreamedDecodeStraightProxy* NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvStreamedDecodeStraightProxy();
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
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

private:
	CImageConvStreamedDecodeStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvStreamedDecode* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvStreamedDecode implementation.

Used only in the threaded operations.

@see MImageConvStreamedDecode
@see CImageConvStreamedDecodeStub
*/
NONSHARABLE_CLASS( CImageConvStreamedDecodeProxy ) : public CImageConvProxyBase,
									  				 public MImageConvStreamedDecode
	{
public:
	static CImageConvStreamedDecodeProxy* NewL(MImageConvExtensionThreadedSupport* aParent);
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);	
	
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

private:
	explicit CImageConvStreamedDecodeProxy(MImageConvExtensionThreadedSupport* aParent);
	
private:
	MImageConvExtensionThreadedSupport* iParent;
	};

/*
Plugin-side stub class that relays calls from the streamed decode proxy to the
MImageConvStreamedDecode implementation.

Used only in the threaded operations.

@see MImageConvStreamedDecode
@see CImageConvStreamedDecodeProxy
*/
NONSHARABLE_CLASS( CImageConvStreamedDecodeStub ) :	public CImageConvStubBase
	{	
public:
	static CImageConvStreamedDecodeStub* NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvStreamedDecodeStub();
	
	// From CImageConvStubBase
	TUid Uid() const;
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

private:
	explicit CImageConvStreamedDecodeStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvStreamedDecode* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};
	
/*
Supports ICL client side extensions in calling the plugin-side
MImageConvStreamedEncode implementation.

Used only in the single-threaded operations.

@see MImageConvStreamedEncode
*/
NONSHARABLE_CLASS( CImageConvStreamedEncodeStraightProxy ) : public CImageConvProxyBase,
									  				 		 public MImageConvStreamedEncode
	{
public:
	static CImageConvStreamedEncodeStraightProxy* NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvStreamedEncodeStraightProxy();
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvStreamedEncode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	void AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition);
	void Complete(TRequestStatus* aStatus);

private:
	CImageConvStreamedEncodeStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvStreamedEncode* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvStreamedEncode implementation.

Used only in the threaded operations.

@see MImageConvStreamedEncode
@see CImageConvStreamedEncodeStub
*/
NONSHARABLE_CLASS( CImageConvStreamedEncodeProxy ) : public CImageConvProxyBase,
									  				 public MImageConvStreamedEncode
	{
public:
	static CImageConvStreamedEncodeProxy* NewL(MImageConvExtensionThreadedSupport* aParent);

	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvStreamedEncode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	void AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition);
	void Complete(TRequestStatus* aStatus);

private:
	explicit CImageConvStreamedEncodeProxy(MImageConvExtensionThreadedSupport* aParent);
	
private:
	MImageConvExtensionThreadedSupport* iParent;
	};

/*
Plugin-side stub class that relays calls from the streamed encode proxy to the
MImageConvStreamedEncode implementation.

Used only in the threaded operations.

@see MImageConvStreamedEncode
@see CImageConvStreamedEncodeProxy
*/
NONSHARABLE_CLASS( CImageConvStreamedEncodeStub ) :	public CImageConvStubBase
	{	
public:
	static CImageConvStreamedEncodeStub* NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvStreamedEncodeStub();
	
	// From CImageConvStubBase
	TUid Uid() const;
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

private:
	explicit CImageConvStreamedEncodeStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvStreamedEncode* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvPrepare implementation.

Used only in the single-threaded operations.

@see MImageConvPrepare
*/
NONSHARABLE_CLASS( CImageConvPrepareStraightProxy ) : public CImageConvProxyBase,
									  				  public MImageConvPrepare
	{
public:
	static CImageConvPrepareStraightProxy* NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvPrepareStraightProxy();
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);
	
	// From MImageConvPrepare
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void Prepare(TRequestStatus* aStatus);

private:
	CImageConvPrepareStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvPrepare* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

/*
Supports ICL client side extensions in calling the plugin-side
MImageConvPrepare implementation.

Used only in the threaded operations.

@see MImageConvPrepare
@see CImageConvPrepareStub
*/
NONSHARABLE_CLASS( CImageConvPrepareProxy ) : public CImageConvProxyBase,
											  public MImageConvPrepare
	{
public:
	static CImageConvPrepareProxy* NewL(MImageConvExtensionThreadedSupport* aParent);
	
	// From CImageConvProxyBase
	void SetupExtension(MImageConvExtension*& aExtension);	
	
	// From MImageConvPrepare
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void Prepare(TRequestStatus* aStatus);

private:
	explicit CImageConvPrepareProxy(MImageConvExtensionThreadedSupport* aParent);
	
private:
	MImageConvExtensionThreadedSupport* iParent;
	};

/*
Plugin-side stub class that relays calls from the Prepare proxy to the
MImageConvPrepare implementation.

Used only in the threaded operations.

@see MImageConvPrepare
@see CImageConvPrepareProxy
*/
NONSHARABLE_CLASS( CImageConvPrepareStub ) :	public CImageConvStubBase
	{	
public:
	static CImageConvPrepareStub* NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	~CImageConvPrepareStub();
	
	// From CImageConvStubBase
	TUid Uid() const;
	void HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase);
	void HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase);

private:
	explicit CImageConvPrepareStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread);
	
private:
	MImageConvPrepare* iExtension;
	MFrameworkExtAsyncInterface* iStatusOwner;
	RThread* iThread;
	};

#endif // ICLEXTPROXYSTUBUTIL_H
