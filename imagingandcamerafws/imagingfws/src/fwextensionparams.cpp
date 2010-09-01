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

#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "fwextensionparams.h"
#include <imageframe.h>

TExecuteCommandParamsBase::TExecuteCommandParamsBase(TUid aUid, TUint aCmd)
:iUid(aUid), iCmdNum(aCmd)
	{
	}
	
TExecuteCommandParamsAsyncBase::TExecuteCommandParamsAsyncBase(TUid aUid, TUint aCmd, TRequestStatus* aRequestStatus)
:TExecuteCommandParamsBase(aUid, aCmd), iRequestStatus(aRequestStatus)
	{
	}

// Base extension param classes
TUidExecCmdParams::TUidExecCmdParams(TUid aUid, TImgConvExtCmd aCmd)
:TExecuteCommandParamsBase(aUid, aCmd), iReturn(TUid::Null())
	{
	}

// Transform param classes
TCapabilitiesExecCmdParams::TCapabilitiesExecCmdParams(TUid aUid, TImgConvTransformExtCmd aCmd)
:TExecuteCommandParamsBase(aUid, aCmd)
	{
	}
	
TAddOperationLExecCmdParams::TAddOperationLExecCmdParams(TUid aUid, TImgConvTransformExtCmd aCmd, TImageConvOperation::TOperation aOperation)
:TExecuteCommandParamsBase(aUid, aCmd), iOperation(aOperation)
	{
	}
	
TClearOperationStackExecCmdParams::TClearOperationStackExecCmdParams(TUid aUid, TImgConvTransformExtCmd aCmd)
:TExecuteCommandParamsBase(aUid, aCmd)
	{
	}

// Scaler param classes	
TGetCapabilitiesScalerExecCmdParams::TGetCapabilitiesScalerExecCmdParams(TUid aUid, TImgConvScalerExtCmd aCmd, TScalerCaps& aCaps)
:TExecuteCommandParamsBase(aUid, aCmd), iCaps(&aCaps)
	{
	}
	
TSetScalingLSizeExecCmdParams::TSetScalingLSizeExecCmdParams(TUid aUid, TImgConvScalerExtCmd aCmd, const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
:TExecuteCommandParamsBase(aUid, aCmd), iDesiredSize(&aDesiredSize), iQuality(aQuality), iLockAspectRatio(aLockAspectRatio)
	{
	}
	
TSetScalingLCoeffExecCmdParams::TSetScalingLCoeffExecCmdParams(TUid aUid, TImgConvScalerExtCmd aCmd, TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
:TExecuteCommandParamsBase(aUid, aCmd), iScalingCoeff(aScalingCoeff), iScalingQuality(aScalingQuality)
	{
	}
	
// Streamed decode param classes
TGetBlocksExecCmdParams::TGetBlocksExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TRequestStatus* aRequestStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
:TExecuteCommandParamsAsyncBase(aUid, aCmd, aRequestStatus), iFrame(aFrame), iSeqPosition(aSeqPosition), iNumBlocksToGet(aNumBlocksToGet), iNumBlocksRead(aNumBlocksRead)
	{
	}
	
TGetBufferSizeExecCmdParams::TGetBufferSizeExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks)
:TExecuteCommandParamsBase(aUid, aCmd), iReturn(KErrGeneral), iFormat(aFormat), iBlockSizeInPixels(&aBlockSizeInPixels), iNumBlocks(aNumBlocks)
	{
	}
	
TGetNextBlocksExecCmdParams::TGetNextBlocksExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TRequestStatus* aRequestStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
:TExecuteCommandParamsAsyncBase(aUid, aCmd, aRequestStatus), iFrame(aFrame), iNumBlocksToGet(aNumBlocksToGet), iNumBlocksRead(aNumBlocksRead), iHaveMoreBlocks(aHaveMoreBlocks)
	{
	}
	
TGetCapabilitiesStrDecExecCmdParams::TGetCapabilitiesStrDecExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps)
:TExecuteCommandParamsBase(aUid, aCmd), iFormat(aFormat), iFrameNumber(aFrameNumber), iCaps(&aCaps)
	{
	}

TGetSupportedFormatsLStrDecExecCmdParams::TGetSupportedFormatsLStrDecExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, RArray<TUid>& aFormats, TUid& aOptimalFormat)
:TExecuteCommandParamsBase(aUid, aCmd), iFormats(&aFormats), iOptimalFormat(&aOptimalFormat)
	{
	}
	
TInitFrameLStrDecExecCmdParams::TInitFrameLStrDecExecCmdParams(TUid aUid, TImgConvStreamedDecodeExtCmd aCmd, TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
:TExecuteCommandParamsBase(aUid, aCmd), iFormat(aFormat), iFrameNumber(aFrameNumber), iNavigation(aNavigation)
	{
	}

// Streamed encode param classes
TGetCapabilitiesStrEncExecCmdParams::TGetCapabilitiesStrEncExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TUid aFormat, TEncodeStreamCaps& aCaps)
:TExecuteCommandParamsBase(aUid, aCmd), iFormat(aFormat), iCaps(&aCaps)
	{
	}

TGetSupportedFormatsLStrEncExecCmdParams::TGetSupportedFormatsLStrEncExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, RArray<TUid>& aFormats, TUid& aOptimalFormat)
:TExecuteCommandParamsBase(aUid, aCmd), iFormats(&aFormats), iOptimalFormat(&aOptimalFormat)
	{
	}

TInitFrameLStrEncExecCmdParams::TInitFrameLStrEncExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
:TExecuteCommandParamsBase(aUid, aCmd), iFormat(aFormat), iFrameNumber(aFrameNumber)
, iFrameSizeInPixels(aFrameSizeInPixels), iBlockSizeInPixels(aBlockSizeInPixels), iNavigation(aNavigation), iFrameImageData(aFrameImageData)
	{
	}

TAppendBlocksExecCmdParams::TAppendBlocksExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
:TExecuteCommandParamsAsyncBase(aUid, aCmd, aStatus), iBlocks(&aBlocks), iNumBlocksToAdd(aNumBlocksToAdd)
	{
	}

TAddBlocksExecCmdParams::TAddBlocksExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition)
:TExecuteCommandParamsAsyncBase(aUid, aCmd, aStatus), iBlocks(&aBlocks), iSeqPosition(&aSeqPosition)
	{
	}

TCompleteExecCmdParams::TCompleteExecCmdParams(TUid aUid, TImgConvStreamedEncodeExtCmd aCmd, TRequestStatus* aStatus)
:TExecuteCommandParamsAsyncBase(aUid, aCmd, aStatus)
	{
	}

TPrepareExecCmdParams::TPrepareExecCmdParams(TUid aUid, TImgConvPrepareExtCmd aCmd, TRequestStatus* aStatus)
:TExecuteCommandParamsAsyncBase(aUid, aCmd, aStatus)
	{
	}


