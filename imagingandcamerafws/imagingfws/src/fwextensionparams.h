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

#ifndef FWEXTENSIONPARAMS_H
#define FWEXTENSIONPARAMS_H

#include <imageframe.h>
#include "iclextproxystubutil.h"

/**
@file
@internalTechnology

Overview:
Declaration of message classes used by the framework for the extension
mechanism.  Each T Class is used to pack-up data in the client thread
and then is passed to the plugin thread where it is unpackaged.

For all classes except TGetExtensionParams, the packing occurs in the
proxy class of the extension mechanism, and unpacking in the paired stub
class.
*/

/*
Message class used by the client calls to CImageDecoder::GetExtension() and
CImageEncoder::GetExtension().
*/
class TGetExtensionParams
	{
public:
	TUid iUid;
	MImageConvExtension** iExtension;
	};

/*
Message base class used to store the Uid of the extension being called
and the appropriate function number as used by the proxy/stub pair (i.e.
the enums TImgConvExtCmd, TImgConvTransformExtCmd, TImgConvScalerExtCmd
and TImgConvStreamedDecodeExtCmd).
*/
class TExecuteCommandParamsBase
	{
protected:	
	TExecuteCommandParamsBase(TUid aUid, TUint aCmd);

public:
	TUid iUid;
	TUint iCmdNum;
	};
	
class TExecuteCommandParamsAsyncBase : public TExecuteCommandParamsBase
	{
protected:
	TExecuteCommandParamsAsyncBase(TUid aUid, TUint aCmd, TRequestStatus* aRequestStatus);
	
public:
	TRequestStatus* iRequestStatus;
	};

/*
Message class used when calling the plugin implementation of
MImageConvExtension::Uid()
*/
class TUidExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TUidExecCmdParams(TUid aUid, TImgConvExtCmd aCmd);
	
public:
	TUid iReturn; // Store the return value of the call.
	};

/*
Message class used when calling the plugin implementation of
MImageConvOperation::Capabilities()
*/
class TCapabilitiesExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TCapabilitiesExecCmdParams(TUid aUid, TImgConvTransformExtCmd aCmd);
	
public:
	TUint iReturn; // Store the return value of the call.
	};

/*
Message class used when calling the plugin implementation of
MImageConvOperation::AddOperationL()
*/	
class TAddOperationLExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TAddOperationLExecCmdParams(TUid aUid, TImgConvTransformExtCmd aCmd, TImageConvOperation::TOperation aOperation);
	
public:
	TImageConvOperation::TOperation iOperation;
	};

/*
Message class used when calling the plugin implementation of
MImageConvOperation::ClearOperationStack()
*/		
class TClearOperationStackExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TClearOperationStackExecCmdParams(TUid aUid, TImgConvTransformExtCmd aCmd);
	};

/*
Message class used when calling the plugin implementation of
MImageConvScaler::GetCapabilities()
*/
class TGetCapabilitiesScalerExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TGetCapabilitiesScalerExecCmdParams(TUid aUid, TImgConvScalerExtCmd aCmd, TScalerCaps& aCaps);
	
public:
	TScalerCaps* iCaps;
	};

/*
Message class used when calling the plugin implementation of
MImageConvScaler::SetScalingL(TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)

*/	
class TSetScalingLSizeExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TSetScalingLSizeExecCmdParams(TUid aUid, TImgConvScalerExtCmd aCmd, const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	
public:
	const TSize* iDesiredSize;
	TImageConvScaler::TScalerQuality iQuality;
	TBool iLockAspectRatio;
	};	

/*
Message class used when calling the plugin implementation of
MImageConvScaler::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)

*/	
class TSetScalingLCoeffExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TSetScalingLCoeffExecCmdParams(TUid aUid, TImgConvScalerExtCmd aCmd, TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);	
	
public:
	TInt iScalingCoeff;
	TImageConvScaler::TScalerQuality iScalingQuality;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedDecode::GetBlocks()
*/
class TGetBlocksExecCmdParams : public TExecuteCommandParamsAsyncBase
	{
public:
	TGetBlocksExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TRequestStatus* aRequestStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);
	
public:
	CImageFrame* iFrame;
	TInt iSeqPosition;
	TInt iNumBlocksToGet;
	TInt* iNumBlocksRead;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedDecode::GetBufferSize()
*/	
class TGetBufferSizeExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TGetBufferSizeExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks);
	
public:
	TInt iReturn;
	TUid iFormat;
	TSize* iBlockSizeInPixels;
	TInt iNumBlocks;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedDecode::GetNextBlocks()
*/
class TGetNextBlocksExecCmdParams : public TExecuteCommandParamsAsyncBase
	{
public:
	TGetNextBlocksExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TRequestStatus* aRequestStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks);	
	
public:
	CImageFrame* iFrame;
	TInt iNumBlocksToGet;
	TInt* iNumBlocksRead;
	TBool* iHaveMoreBlocks;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedDecode::GetCapabilities()
*/
class TGetCapabilitiesStrDecExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TGetCapabilitiesStrDecExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps);	
	
public:
	TUid iFormat;
	TInt iFrameNumber;
	TDecodeStreamCaps* iCaps;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedEncode::GetCapabilities()
*/
class TGetCapabilitiesStrEncExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TGetCapabilitiesStrEncExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TUid aFormat, TEncodeStreamCaps& aCaps);
	
public:
	TUid iFormat;
	TEncodeStreamCaps* iCaps;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedDecode::GetSupportedFormatsL()
*/
class TGetSupportedFormatsLStrDecExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TGetSupportedFormatsLStrDecExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, RArray<TUid>& aFormats, TUid& aOptimalFormat);	
	
public:
	RArray<TUid>* iFormats;
	TUid* iOptimalFormat;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedEncode::GetSupportedFormatsL()
*/
class TGetSupportedFormatsLStrEncExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TGetSupportedFormatsLStrEncExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, RArray<TUid>& aFormats, TUid& aOptimalFormat);
	
public:
	RArray<TUid>* iFormats;
	TUid* iOptimalFormat;
	};	

/*
Message class used when calling the plugin implementation of
MImageConvStreamedDecode::InitFrameL()
*/
class TInitFrameLStrDecExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TInitFrameLStrDecExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation);
	
public:
	TUid iFormat;
	TInt iFrameNumber;
	TDecodeStreamCaps::TNavigation iNavigation;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedEncode::InitFrameL()
*/
class TInitFrameLStrEncExecCmdParams : public TExecuteCommandParamsBase
	{
public:
	TInitFrameLStrEncExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	
public:
	TUid iFormat;
	TInt iFrameNumber;
	TSize iFrameSizeInPixels;
	TSize iBlockSizeInPixels;
	TEncodeStreamCaps::TNavigation iNavigation;
	const CFrameImageData* iFrameImageData;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedEncode::AppendBlocks()
*/
class TAppendBlocksExecCmdParams : public TExecuteCommandParamsAsyncBase
	{
public:
	TAppendBlocksExecCmdParams(TUid aFormat, TImgConvStreamedEncodeExtCmd aCmd, TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd);

public:
	const CImageFrame* iBlocks;
	TInt iNumBlocksToAdd;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedEncode::AddBlocks()
*/
class TAddBlocksExecCmdParams : public TExecuteCommandParamsAsyncBase
	{
public:
	TAddBlocksExecCmdParams(TUid aFormat, TImgConvStreamedEncodeExtCmd aCmd, TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition);

public:
	const CImageFrame* iBlocks;
	const TInt* iSeqPosition;
	};

/*
Message class used when calling the plugin implementation of
MImageConvStreamedEncode::Complete()
*/
class TCompleteExecCmdParams : public TExecuteCommandParamsAsyncBase
	{
public:
	TCompleteExecCmdParams(TUid aFormat, TImgConvStreamedEncodeExtCmd aCmd, TRequestStatus* aStatus);
	};

/*
Message class used when calling the plugin implementation of
MImageConvPrepare::Prepare()
*/
class TPrepareExecCmdParams : public TExecuteCommandParamsAsyncBase
	{
public:
	TPrepareExecCmdParams(TUid aFormat, TImgConvPrepareExtCmd aCmd, TRequestStatus* aRequestStatus);
	};

#endif // FWEXTENSIONPARAMS_H
