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

#include <e32base.h>
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "ThreadRelay.h"
#include "ImageClientMain.h"
#include "fwextconstants.h"
#include "fwextensionparams.h"

// Static utility class
CImageConvProxyBase* ProxyStubUtility::GetNewProxyL(TUid aUid, MImageConvExtensionThreadedSupport* aParent)
	{
	switch(aUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			return CImageConvOperationProxy::NewL(aParent);
			}
		case KUidImageConvExtScalerValue:
			{
			return CImageConvScalerProxy::NewL(aParent);
			}
		case KUidImageConvExtStreamedDecodeValue:
			{
			return CImageConvStreamedDecodeProxy::NewL(aParent);
			}
		case KUidImageConvExtStreamedEncodeValue:
			{
			return CImageConvStreamedEncodeProxy::NewL(aParent);
			}
		case KUidImageConvExtPrepareValue:
			{
			return CImageConvPrepareProxy::NewL(aParent);
			}
		default:
			{
			Panic(EInvalidFwExtensionUid);
			}
		}
	return NULL;
	}
	
CImageConvProxyBase* ProxyStubUtility::GetNewStraightProxyL(TUid aUid, MFrameworkExtAsyncInterface* aStatusOwner,
															RThread& aRequestThread)
	{
	switch(aUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			return CImageConvOperationStraightProxy::NewL(aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtScalerValue:
			{
			return CImageConvScalerStraightProxy::NewL(aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtStreamedDecodeValue:
			{
			return CImageConvStreamedDecodeStraightProxy::NewL(aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtStreamedEncodeValue:
			{
			return CImageConvStreamedEncodeStraightProxy::NewL(aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtPrepareValue:
			{
			return CImageConvPrepareStraightProxy::NewL(aStatusOwner, aRequestThread);
			}
		default:
			{
			Panic(EInvalidFwExtensionUid);
			}
		}
	return NULL;
	}

CImageConvStubBase* ProxyStubUtility::NewStubL(TUid aUid,
											   MImageConvExtension* aExtension,
											   MFrameworkExtAsyncInterface* aStatusOwner,
											   RThread& aRequestThread)
	{
	switch(aUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			return CImageConvOperationStub::NewL(aExtension, aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtScalerValue:
			{
			return CImageConvScalerStub::NewL(aExtension, aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtStreamedDecodeValue:
			{
			return CImageConvStreamedDecodeStub::NewL(aExtension, aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtStreamedEncodeValue:
			{
			return CImageConvStreamedEncodeStub::NewL(aExtension, aStatusOwner, aRequestThread);
			}
		case KUidImageConvExtPrepareValue:
			{
			return CImageConvPrepareStub::NewL(aExtension, aStatusOwner, aRequestThread);
			}
		default:
			{
			Panic(EInvalidFwExtensionUid);
			}
		}
	return NULL;
	}

// Operation straight proxy
CImageConvOperationStraightProxy* CImageConvOperationStraightProxy::NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvOperationStraightProxy(aStatusOwner, aRequestThread);
	}

CImageConvOperationStraightProxy::CImageConvOperationStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	}
	
CImageConvOperationStraightProxy::~CImageConvOperationStraightProxy()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}

void CImageConvOperationStraightProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	iExtension = static_cast<MImageConvOperation*>(aExtension);
	aExtension = this;
	}
	
TUid CImageConvOperationStraightProxy::Uid() const
	{
	return iExtension->Uid();
	}
	
void CImageConvOperationStraightProxy::IncrementRef()
	{
	iExtension->IncrementRef();
	}
	
void CImageConvOperationStraightProxy::Release()
	{
	iExtension->Release();
	}
	
TUint CImageConvOperationStraightProxy::Capabilities() const
	{
	return iExtension->Capabilities();
	}
	
void CImageConvOperationStraightProxy::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	iExtension->AddOperationL(aOperation);
	}
	
void CImageConvOperationStraightProxy::ClearOperationStack()
	{
	iExtension->ClearOperationStack();
	}

// Operation proxy
CImageConvOperationProxy* CImageConvOperationProxy::NewL(MImageConvExtensionThreadedSupport* aParent)
	{
	return new (ELeave) CImageConvOperationProxy(aParent);
	}
	
CImageConvOperationProxy::CImageConvOperationProxy(MImageConvExtensionThreadedSupport* aParent)
:iParent(aParent)
	{
	}

void CImageConvOperationProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	aExtension = this;
	}

TUid CImageConvOperationProxy::Uid() const
	{
	TUidExecCmdParams uid(KICLOperationUid, EImgConvExtUid);
	iParent->ExecuteCommand(uid);
	return uid.iReturn;
	}
	
void CImageConvOperationProxy::IncrementRef()
	{
	ASSERT(EFalse);
	}
	
void CImageConvOperationProxy::Release()
	{
	ASSERT(EFalse);
	}
	
TUint CImageConvOperationProxy::Capabilities() const
	{
	TCapabilitiesExecCmdParams capabilities(KICLOperationUid, EImgConvExtCapabilities);
	iParent->ExecuteCommand(capabilities);
	return capabilities.iReturn;
	}
	
void CImageConvOperationProxy::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	TAddOperationLExecCmdParams addOperationL(KICLOperationUid, EImgConvExtAddOperationL, aOperation);
	iParent->ExecuteCommandL(addOperationL);
	}
	
void CImageConvOperationProxy::ClearOperationStack()
	{
	TClearOperationStackExecCmdParams clearOperationStack(KICLOperationUid, EImgConvExtClearOperationStack);
	iParent->ExecuteCommand(clearOperationStack);
	}

// Operation stub
CImageConvOperationStub* CImageConvOperationStub::NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvOperationStub(aExtension, aStatusOwner, aRequestThread);
	}

CImageConvOperationStub::CImageConvOperationStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	iExtension = static_cast<MImageConvOperation*>(aExtension);
	}	

CImageConvOperationStub::~CImageConvOperationStub()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
TUid CImageConvOperationStub::Uid() const
	{
	return iExtension->Uid();
	}
	
void CImageConvOperationStub::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtUid:
			{
			// cast to appropriate extension TClass
			TUidExecCmdParams* uid = static_cast<TUidExecCmdParams*>(&aExecuteCommandBase);
			uid->iReturn = iExtension->Uid();
			break;
			}
		case EImgConvExtIncrementRef:
			{
			iExtension->IncrementRef();
			break;
			}
		case EImgConvExtRelease:
			{
			iExtension->Release();
			break;
			}
		case EImgConvExtCapabilities:
			{
			TCapabilitiesExecCmdParams* capabilities = static_cast<TCapabilitiesExecCmdParams*>(&aExecuteCommandBase);
			capabilities->iReturn = iExtension->Capabilities();
			break;
			}
		case EImgConvExtAddOperationL:
			{
			TAddOperationLExecCmdParams* addOperationL = static_cast<TAddOperationLExecCmdParams*>(&aExecuteCommandBase);
			iExtension->AddOperationL(addOperationL->iOperation);
			break;
			}
		case EImgConvExtClearOperationStack:
			{
			iExtension->ClearOperationStack();
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
	}
	
void CImageConvOperationStub::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& /*aExecuteCommandBase*/)
	{
	// No async calls for transform
	}

// Straight scaler proxy
CImageConvScalerStraightProxy* CImageConvScalerStraightProxy::NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvScalerStraightProxy(aStatusOwner, aRequestThread);
	}
	
CImageConvScalerStraightProxy::CImageConvScalerStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	}
	
CImageConvScalerStraightProxy::~CImageConvScalerStraightProxy()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
void CImageConvScalerStraightProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	iExtension = static_cast<MImageConvScaler*>(aExtension);
	aExtension = this;
	}
	
TUid CImageConvScalerStraightProxy::Uid() const
	{
	return iExtension->Uid();
	}
	
void CImageConvScalerStraightProxy::IncrementRef()
	{
	iExtension->IncrementRef();
	}
	
void CImageConvScalerStraightProxy::Release()
	{
	iExtension->Release();
	}
	
void CImageConvScalerStraightProxy::GetCapabilities(TScalerCaps& aCaps) const
	{
	iExtension->GetCapabilities(aCaps);
	}

void CImageConvScalerStraightProxy::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	iExtension->SetScalingL(aDesiredSize, aQuality, aLockAspectRatio);
	}
	
void CImageConvScalerStraightProxy::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	iExtension->SetScalingL(aScalingCoeff, aScalingQuality);
	}

// Scaler proxy
CImageConvScalerProxy* CImageConvScalerProxy::NewL(MImageConvExtensionThreadedSupport* aParent)
	{
	return new (ELeave) CImageConvScalerProxy(aParent);
	}

CImageConvScalerProxy::CImageConvScalerProxy(MImageConvExtensionThreadedSupport* aParent)
:iParent(aParent)
	{
	}
	
void CImageConvScalerProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	aExtension = this;
	}

TUid CImageConvScalerProxy::Uid() const
	{
	TUidExecCmdParams uid(KICLScalerUid, EImgConvExtUid);
	iParent->ExecuteCommand(uid);
	return uid.iReturn;
	}
	
void CImageConvScalerProxy::IncrementRef()
	{
	ASSERT(EFalse);
	}
	
void CImageConvScalerProxy::Release()
	{
	ASSERT(EFalse);
	}

void CImageConvScalerProxy::GetCapabilities(TScalerCaps& aCaps) const
	{
	TGetCapabilitiesScalerExecCmdParams scalerCaps(KICLScalerUid, EImgConvExtGetCapabilitiesScaler, aCaps);
	iParent->ExecuteCommand(scalerCaps);
	}
	
void CImageConvScalerProxy::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	TSetScalingLSizeExecCmdParams setScalingL(KICLScalerUid, EImgConvExtSetScalingLSize, aDesiredSize, aQuality, aLockAspectRatio);
	iParent->ExecuteCommandL(setScalingL);
	}

void CImageConvScalerProxy::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	TSetScalingLCoeffExecCmdParams setScaling(KICLScalerUid, EImgConvExtSetScalingLCoeff, aScalingCoeff, aScalingQuality);
	iParent->ExecuteCommandL(setScaling);
	}
	
// Scaler stub
CImageConvScalerStub* CImageConvScalerStub::NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvScalerStub(aExtension, aStatusOwner, aRequestThread);
	}

CImageConvScalerStub::CImageConvScalerStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	iExtension = static_cast<MImageConvScaler*>(aExtension);
	}

CImageConvScalerStub::~CImageConvScalerStub()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
TUid CImageConvScalerStub::Uid() const
	{
	return iExtension->Uid();
	}	
	
void CImageConvScalerStub::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtUid:
			{
			// cast to appropriate extension TClass
			TUidExecCmdParams* uid = static_cast<TUidExecCmdParams*>(&aExecuteCommandBase);
			uid->iReturn = iExtension->Uid();
			break;
			}
		case EImgConvExtIncrementRef:
			{
			iExtension->IncrementRef();
			break;
			}
		case EImgConvExtRelease:
			{
			iExtension->Release();
			break;
			}
		case EImgConvExtGetCapabilitiesScaler:
			{
			TGetCapabilitiesScalerExecCmdParams* scalerCaps = static_cast<TGetCapabilitiesScalerExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetCapabilities(*scalerCaps->iCaps);
			break;
			}
		case EImgConvExtSetScalingLSize:
			{
			TSetScalingLSizeExecCmdParams* setScalingLSize = static_cast<TSetScalingLSizeExecCmdParams*>(&aExecuteCommandBase);
			iExtension->SetScalingL(*setScalingLSize->iDesiredSize, setScalingLSize->iQuality, setScalingLSize->iLockAspectRatio);
			break;
			}
		case EImgConvExtSetScalingLCoeff:
			{
			TSetScalingLCoeffExecCmdParams* setScalingLCoeff = static_cast<TSetScalingLCoeffExecCmdParams*>(&aExecuteCommandBase);
			iExtension->SetScalingL(setScalingLCoeff->iScalingCoeff, setScalingLCoeff->iScalingQuality);
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
	}
	
void CImageConvScalerStub::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& /*aExecuteCommandBase*/)
	{
	// No async calls for scaler
	}

// Streamed decode straight proxy
CImageConvStreamedDecodeStraightProxy* CImageConvStreamedDecodeStraightProxy::NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvStreamedDecodeStraightProxy(aStatusOwner, aRequestThread);
	}
	
CImageConvStreamedDecodeStraightProxy::CImageConvStreamedDecodeStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	}

CImageConvStreamedDecodeStraightProxy::~CImageConvStreamedDecodeStraightProxy()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
void CImageConvStreamedDecodeStraightProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	iExtension = static_cast<MImageConvStreamedDecode*>(aExtension);
	aExtension = this;
	}
	
TUid CImageConvStreamedDecodeStraightProxy::Uid() const
	{
	return iExtension->Uid();
	}
	
void CImageConvStreamedDecodeStraightProxy::IncrementRef()
	{
	iExtension->IncrementRef();
	}
	
void CImageConvStreamedDecodeStraightProxy::Release()
	{
	iExtension->Release();
	}
	
void CImageConvStreamedDecodeStraightProxy::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	iExtension->GetSupportedFormatsL(aFormats, aOptimalFormat);
	}

void CImageConvStreamedDecodeStraightProxy::GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const
	{
	iExtension->GetCapabilities(aFormat, aFrameNumber, aCaps);
	}

TInt CImageConvStreamedDecodeStraightProxy::GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	return iExtension->GetBufferSize(aFormat, aBlockSizeInPixels, aNumBlocks);
	}

void CImageConvStreamedDecodeStraightProxy::InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	iExtension->InitFrameL(aFormat, aFrameNumber, aNavigation);
	}

void CImageConvStreamedDecodeStraightProxy::GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aStatus);
	iExtension->GetBlocks(NULL, aFrame, aSeqPosition, aNumBlocksToGet, aNumBlocksRead);
	if(*aStatus == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}
	
void CImageConvStreamedDecodeStraightProxy::GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aStatus);
	iExtension->GetNextBlocks(NULL, aFrame, aNumBlocksToGet, aNumBlocksRead, aHaveMoreBlocks);
	if(*aStatus == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}

// Streamed decode proxy
CImageConvStreamedDecodeProxy* CImageConvStreamedDecodeProxy::NewL(MImageConvExtensionThreadedSupport* aParent)
	{
	return new (ELeave) CImageConvStreamedDecodeProxy(aParent);
	}

CImageConvStreamedDecodeProxy::CImageConvStreamedDecodeProxy(MImageConvExtensionThreadedSupport* aParent)
:iParent(aParent)
	{
	}
	
void CImageConvStreamedDecodeProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	aExtension = this;
	}

TUid CImageConvStreamedDecodeProxy::Uid() const
	{
	TUidExecCmdParams uid(KICLStreamedDecodeUid, EImgConvExtUid);
	iParent->ExecuteCommand(uid);
	return uid.iReturn;
	}
	
void CImageConvStreamedDecodeProxy::IncrementRef()
	{
	ASSERT(EFalse);
	}
	
void CImageConvStreamedDecodeProxy::Release()
	{
	ASSERT(EFalse);
	}

void CImageConvStreamedDecodeProxy::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	TGetSupportedFormatsLStrDecExecCmdParams supportedFormats(KICLStreamedDecodeUid, EImgConvExtGetSupportedFormatsLStreamDec, aFormats, aOptimalFormat);
	iParent->ExecuteCommandL(supportedFormats);
	}	

void CImageConvStreamedDecodeProxy::GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const
	{
	TGetCapabilitiesStrDecExecCmdParams streamCaps(KICLStreamedDecodeUid, EImgConvExtGetCapabilitiesStreamDec, aFormat, aFrameNumber, aCaps);
	iParent->ExecuteCommand(streamCaps);
	}

TInt CImageConvStreamedDecodeProxy::GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	TGetBufferSizeExecCmdParams bufferSize(KICLStreamedDecodeUid, EImgConvExtGetBufferSize, aFormat, aBlockSizeInPixels, aNumBlocks);
	iParent->ExecuteCommand(bufferSize);
	return bufferSize.iReturn;
	}

void CImageConvStreamedDecodeProxy::InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	TInitFrameLStrDecExecCmdParams initFrame(KICLStreamedDecodeUid, EImgConvExtInitFrameLStreamDec, aFormat, aFrameNumber, aNavigation);
	iParent->ExecuteCommandL(initFrame);
	}

void CImageConvStreamedDecodeProxy::GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	TGetBlocksExecCmdParams getBlocks(KICLStreamedDecodeUid, EImgConvExtGetBlocks, aStatus, aFrame, aSeqPosition, aNumBlocksToGet, aNumBlocksRead);
	iParent->ExecuteAsyncCommand(getBlocks);
	}

void CImageConvStreamedDecodeProxy::GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	TGetNextBlocksExecCmdParams getNextBlocks(KICLStreamedDecodeUid, EImgConvExtGetNextBlocks, aStatus, aFrame, aNumBlocksToGet, aNumBlocksRead, aHaveMoreBlocks);
	iParent->ExecuteAsyncCommand(getNextBlocks);
	}

// Streamed decode stub
CImageConvStreamedDecodeStub* CImageConvStreamedDecodeStub::NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvStreamedDecodeStub(aExtension, aStatusOwner, aRequestThread);
	}

CImageConvStreamedDecodeStub::CImageConvStreamedDecodeStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	iExtension = static_cast<MImageConvStreamedDecode*>(aExtension);
	}

CImageConvStreamedDecodeStub::~CImageConvStreamedDecodeStub()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
TUid CImageConvStreamedDecodeStub::Uid() const
	{
	return iExtension->Uid();
	}		
	
void CImageConvStreamedDecodeStub::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtUid:
			{
			// cast to appropriate extension TClass
			TUidExecCmdParams* uid = static_cast<TUidExecCmdParams*>(&aExecuteCommandBase);
			uid->iReturn = iExtension->Uid();
			break;
			}
		case EImgConvExtIncrementRef:
			{
			iExtension->IncrementRef();
			break;
			}
		case EImgConvExtRelease:
			{
			iExtension->Release();
			break;
			}
		case EImgConvExtGetBufferSize:
			{
			TGetBufferSizeExecCmdParams* getBufferSize = static_cast<TGetBufferSizeExecCmdParams*>(&aExecuteCommandBase);
			getBufferSize->iReturn = iExtension->GetBufferSize(getBufferSize->iFormat, *getBufferSize->iBlockSizeInPixels, getBufferSize->iNumBlocks);
			break;
			}
		case EImgConvExtGetCapabilitiesStreamDec:
			{
			TGetCapabilitiesStrDecExecCmdParams* getStreamCaps = static_cast<TGetCapabilitiesStrDecExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetCapabilities(getStreamCaps->iFormat, getStreamCaps->iFrameNumber, *getStreamCaps->iCaps);
			break;
			}
		case EImgConvExtGetSupportedFormatsLStreamDec:
			{
			TGetSupportedFormatsLStrDecExecCmdParams* getSupportedFormats = static_cast<TGetSupportedFormatsLStrDecExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetSupportedFormatsL(*getSupportedFormats->iFormats, *getSupportedFormats->iOptimalFormat);
			break;
			}
		case EImgConvExtInitFrameLStreamDec:
			{
			TInitFrameLStrDecExecCmdParams* initFrame = static_cast<TInitFrameLStrDecExecCmdParams*>(&aExecuteCommandBase);
			iExtension->InitFrameL(initFrame->iFormat, initFrame->iFrameNumber, initFrame->iNavigation);
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
	}
	
void CImageConvStreamedDecodeStub::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aExecuteCommandBase.iRequestStatus);
	
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtGetBlocks:
			{
			TGetBlocksExecCmdParams* getBlocks = static_cast<TGetBlocksExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetBlocks(NULL,
									getBlocks->iFrame,
									getBlocks->iSeqPosition, 
									getBlocks->iNumBlocksToGet, 
									getBlocks->iNumBlocksRead);
			break;
			}
		case EImgConvExtGetNextBlocks:
			{		
			TGetNextBlocksExecCmdParams* getNextBlocks = static_cast<TGetNextBlocksExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetNextBlocks(NULL,
										getNextBlocks->iFrame,
										getNextBlocks->iNumBlocksToGet, 
										getNextBlocks->iNumBlocksRead,
										getNextBlocks->iHaveMoreBlocks);
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
	if(*(aExecuteCommandBase.iRequestStatus) == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}

// Straight streamed encode proxy
CImageConvStreamedEncodeStraightProxy* CImageConvStreamedEncodeStraightProxy::NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvStreamedEncodeStraightProxy(aStatusOwner, aRequestThread);
	}

CImageConvStreamedEncodeStraightProxy::CImageConvStreamedEncodeStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	}

CImageConvStreamedEncodeStraightProxy::~CImageConvStreamedEncodeStraightProxy()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}

void CImageConvStreamedEncodeStraightProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	iExtension = static_cast<MImageConvStreamedEncode*>(aExtension);
	aExtension = this;
	}

TUid CImageConvStreamedEncodeStraightProxy::Uid() const
	{
	return iExtension->Uid();
	}

void CImageConvStreamedEncodeStraightProxy::IncrementRef()
	{
	iExtension->IncrementRef();
	}

void CImageConvStreamedEncodeStraightProxy::Release()
	{
	iExtension->Release();
	}

void CImageConvStreamedEncodeStraightProxy::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	iExtension->GetSupportedFormatsL(aFormats, aOptimalFormat);
	}

void CImageConvStreamedEncodeStraightProxy::GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const
	{
	iExtension->GetCapabilities(aFormat, aCaps);
	}

void CImageConvStreamedEncodeStraightProxy::InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	iExtension->InitFrameL(aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aNavigation, aFrameImageData);
	}

void CImageConvStreamedEncodeStraightProxy::AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aStatus);
	iExtension->AppendBlocks(NULL, aBlocks, aNumBlocksToAdd);
	if(*aStatus == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}

void CImageConvStreamedEncodeStraightProxy::AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aStatus);
	iExtension->AddBlocks(NULL, aBlocks, aSeqPosition);
	if(*aStatus == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}

void CImageConvStreamedEncodeStraightProxy::Complete(TRequestStatus* aStatus)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aStatus);
	iExtension->Complete(NULL);
	if(*aStatus == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}

// Streamed encode proxy
CImageConvStreamedEncodeProxy* CImageConvStreamedEncodeProxy::NewL(MImageConvExtensionThreadedSupport* aParent)
	{
	return new (ELeave) CImageConvStreamedEncodeProxy(aParent);	
	}

CImageConvStreamedEncodeProxy::CImageConvStreamedEncodeProxy(MImageConvExtensionThreadedSupport* aParent)
:iParent(aParent)
	{
	}
	
void CImageConvStreamedEncodeProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	aExtension = this;
	}

TUid CImageConvStreamedEncodeProxy::Uid() const
	{
	TUidExecCmdParams uid(KICLStreamedEncodeUid, EImgConvExtUid);
	iParent->ExecuteCommand(uid);
	return uid.iReturn;
	}
	
void CImageConvStreamedEncodeProxy::IncrementRef()
	{
	ASSERT(EFalse);
	}
	
void CImageConvStreamedEncodeProxy::Release()
	{
	ASSERT(EFalse);
	}

void CImageConvStreamedEncodeProxy::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	TGetSupportedFormatsLStrEncExecCmdParams supportedFormats(KICLStreamedEncodeUid, EImgConvExtGetSupportedFormatsLStreamEnc, aFormats, aOptimalFormat);
	iParent->ExecuteCommandL(supportedFormats);
	}

void CImageConvStreamedEncodeProxy::GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const
	{
	TGetCapabilitiesStrEncExecCmdParams getCapabilities(KICLStreamedEncodeUid, EImgConvExtGetCapabilitiesStreamEnc, aFormat, aCaps);
	iParent->ExecuteCommand(getCapabilities);
	}

void CImageConvStreamedEncodeProxy::InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	TInitFrameLStrEncExecCmdParams initFrame(KICLStreamedEncodeUid, EImgConvExtInitFrameLStreamEnc, aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aNavigation, aFrameImageData);
	iParent->ExecuteCommandL(initFrame);
	}

void CImageConvStreamedEncodeProxy::AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	TAppendBlocksExecCmdParams appendBlocks(KICLStreamedEncodeUid, EImgConvExtAppendBlocks, aStatus, aBlocks, aNumBlocksToAdd);
	iParent->ExecuteAsyncCommand(appendBlocks);
	}

void CImageConvStreamedEncodeProxy::AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	TAddBlocksExecCmdParams addBlocks(KICLStreamedEncodeUid, EImgConvExtAddBlocks, aStatus, aBlocks, aSeqPosition);
	iParent->ExecuteAsyncCommand(addBlocks);
	}

void CImageConvStreamedEncodeProxy::Complete(TRequestStatus* aStatus)
	{
	TCompleteExecCmdParams complete(KICLStreamedEncodeUid, EImgConvExtComplete, aStatus);
	iParent->ExecuteAsyncCommand(complete);
	}

// Streamed encode stub
CImageConvStreamedEncodeStub* CImageConvStreamedEncodeStub::NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvStreamedEncodeStub(aExtension, aStatusOwner, aRequestThread);
	}

CImageConvStreamedEncodeStub::CImageConvStreamedEncodeStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	iExtension = static_cast<MImageConvStreamedEncode*>(aExtension);
	}

CImageConvStreamedEncodeStub::~CImageConvStreamedEncodeStub()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}

TUid CImageConvStreamedEncodeStub::Uid() const
	{
	return iExtension->Uid();	
	}

void CImageConvStreamedEncodeStub::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtUid:
			{
			// cast to appropriate extension TClass
			TUidExecCmdParams* uid = static_cast<TUidExecCmdParams*>(&aExecuteCommandBase);
			uid->iReturn = iExtension->Uid();
			break;
			}
		case EImgConvExtIncrementRef:
			{
			iExtension->IncrementRef();
			break;
			}
		case EImgConvExtRelease:
			{
			iExtension->Release();
			break;
			}
		case EImgConvExtGetSupportedFormatsLStreamEnc:
			{
			TGetSupportedFormatsLStrEncExecCmdParams* getSupportedFormats = static_cast<TGetSupportedFormatsLStrEncExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetSupportedFormatsL(*getSupportedFormats->iFormats, *getSupportedFormats->iOptimalFormat);
			break;
			}
		case EImgConvExtGetCapabilitiesStreamEnc:
			{
			TGetCapabilitiesStrEncExecCmdParams* getStreamCaps = static_cast<TGetCapabilitiesStrEncExecCmdParams*>(&aExecuteCommandBase);
			iExtension->GetCapabilities(getStreamCaps->iFormat, *getStreamCaps->iCaps);
			break;
			}
		case EImgConvExtInitFrameLStreamEnc:
			{
			TInitFrameLStrEncExecCmdParams* initFrame = static_cast<TInitFrameLStrEncExecCmdParams*>(&aExecuteCommandBase);
			iExtension->InitFrameL(initFrame->iFormat, initFrame->iFrameNumber,
									initFrame->iFrameSizeInPixels, initFrame->iBlockSizeInPixels,
									initFrame->iNavigation, initFrame->iFrameImageData);
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}	
	}

void CImageConvStreamedEncodeStub::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtAppendBlocks:
			{
			iStatusOwner->RegisterClientRequestStatus(*iThread, aExecuteCommandBase.iRequestStatus);
			TAppendBlocksExecCmdParams* appendBlocks = static_cast<TAppendBlocksExecCmdParams*>(&aExecuteCommandBase);
			iExtension->AppendBlocks(NULL,
										*appendBlocks->iBlocks,
										appendBlocks->iNumBlocksToAdd);
			break;	
			}
		case EImgConvExtAddBlocks:
			{
			iStatusOwner->RegisterClientRequestStatus(*iThread, aExecuteCommandBase.iRequestStatus);
			TAddBlocksExecCmdParams* addBlocks = static_cast<TAddBlocksExecCmdParams*>(&aExecuteCommandBase);
			iExtension->AddBlocks(NULL,
									*addBlocks->iBlocks,
									*addBlocks->iSeqPosition);
			break;
			}
		case EImgConvExtComplete:
			{
			iStatusOwner->RegisterClientRequestStatus(*iThread, aExecuteCommandBase.iRequestStatus);
			iExtension->Complete(NULL);
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
	if(*(aExecuteCommandBase.iRequestStatus) == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}



// Prepare straight proxy
CImageConvPrepareStraightProxy* CImageConvPrepareStraightProxy::NewL(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvPrepareStraightProxy(aStatusOwner, aRequestThread);
	}
	
CImageConvPrepareStraightProxy::CImageConvPrepareStraightProxy(MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	}

CImageConvPrepareStraightProxy::~CImageConvPrepareStraightProxy()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
void CImageConvPrepareStraightProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	iExtension = static_cast<MImageConvPrepare*>(aExtension);
	aExtension = this;
	}
	
TUid CImageConvPrepareStraightProxy::Uid() const
	{
	return iExtension->Uid();
	}
	
void CImageConvPrepareStraightProxy::IncrementRef()
	{
	iExtension->IncrementRef();
	}
	
void CImageConvPrepareStraightProxy::Release()
	{
	iExtension->Release();
	}
	
void CImageConvPrepareStraightProxy::Prepare(TRequestStatus* aStatus)
	{
	iStatusOwner->RegisterClientRequestStatus(*iThread, aStatus);
	iExtension->Prepare(NULL);
	if(*aStatus == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}
	
// Prepare proxy
CImageConvPrepareProxy* CImageConvPrepareProxy::NewL(MImageConvExtensionThreadedSupport* aParent)
	{
	return new (ELeave) CImageConvPrepareProxy(aParent);
	}

CImageConvPrepareProxy::CImageConvPrepareProxy(MImageConvExtensionThreadedSupport* aParent)
:iParent(aParent)
	{
	}
	
void CImageConvPrepareProxy::SetupExtension(MImageConvExtension*& aExtension)
	{
	aExtension = this;
	}

TUid CImageConvPrepareProxy::Uid() const
	{
	TUidExecCmdParams uid(KICLPrepareUid, EImgConvExtUid);
	iParent->ExecuteCommand(uid);
	return uid.iReturn;
	}
	
void CImageConvPrepareProxy::IncrementRef()
	{
	ASSERT(EFalse);
	}
	
void CImageConvPrepareProxy::Release()
	{
	ASSERT(EFalse);
	}

void CImageConvPrepareProxy::Prepare(TRequestStatus* aStatus)
	{
	TPrepareExecCmdParams prepare(KICLPrepareUid, EImgConvExtPrepare, aStatus);
	iParent->ExecuteAsyncCommand(prepare);
	}

// Prepare stub
CImageConvPrepareStub* CImageConvPrepareStub::NewL(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
	{
	return new (ELeave) CImageConvPrepareStub(aExtension, aStatusOwner, aRequestThread);
	}

CImageConvPrepareStub::CImageConvPrepareStub(MImageConvExtension* aExtension, MFrameworkExtAsyncInterface* aStatusOwner, RThread& aRequestThread)
:iStatusOwner(aStatusOwner), iThread(&aRequestThread)
	{
	iExtension = static_cast<MImageConvPrepare*>(aExtension);
	}

CImageConvPrepareStub::~CImageConvPrepareStub()
	{
	if(iExtension)
		{
		iExtension->Release();
		}
	}
	
TUid CImageConvPrepareStub::Uid() const
	{
	return iExtension->Uid();
	}		
	
void CImageConvPrepareStub::HandleExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtUid:
			{
			// cast to appropriate extension TClass
			TUidExecCmdParams* uid = static_cast<TUidExecCmdParams*>(&aExecuteCommandBase);
			uid->iReturn = iExtension->Uid();
			break;
			}
		case EImgConvExtIncrementRef:
			{
			iExtension->IncrementRef();
			break;
			}
		case EImgConvExtRelease:
			{
			iExtension->Release();
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
	}
	
void CImageConvPrepareStub::HandleExecuteAsyncCommand(TExecuteCommandParamsAsyncBase& aExecuteCommandBase)
	{
	switch(aExecuteCommandBase.iCmdNum)
		{
		case EImgConvExtPrepare:
			{
			iStatusOwner->RegisterClientRequestStatus(*iThread, aExecuteCommandBase.iRequestStatus);
			iExtension->Prepare(NULL);
			break;
			}
		default:
			{
			Panic(EInvalidFwExtensionCall);
			}
		}
		
	if(*(aExecuteCommandBase.iRequestStatus) == KRequestPending)
		{
		iStatusOwner->StartActivity();
		}
	}
