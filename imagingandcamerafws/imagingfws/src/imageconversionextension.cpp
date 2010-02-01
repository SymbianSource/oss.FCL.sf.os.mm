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

#include <imageframe.h>
#include <icl/imageconversionextension.h>
#include <icl/imageconversionextensionintf.h>
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "ImageClientMain.h"
#include "fwextconstants.h"

const TUint KVersionMask = 0x000000FF;
const TUint KSizeMask = 0xFFFFFF00;

const TUint KVersionZero = 0x00000000;

EXPORT_C TScalerCaps::TScalerCaps()
:iMaxUpscaleLimit(1), iMustPreserveAspectRatio(ETrue), iMaxDownscaleLimit(-4), 
iPowerOfTwoScalingOnly(ETrue), iSizeVersion(KVersionZero|(sizeof(TScalerCaps)<<8)),
iReserved(0)
	{
	}
	
EXPORT_C TScalerCaps::TScalerCaps(TInt aMaxUpscaleLimit,TInt aMaxDownscaleLimit,TBool aPreserveAspectRatioIsNeeded)
:iMaxUpscaleLimit(aMaxUpscaleLimit), iMustPreserveAspectRatio(aPreserveAspectRatioIsNeeded),
iMaxDownscaleLimit(aMaxDownscaleLimit), iSizeVersion(KVersionZero|(sizeof(TScalerCaps)<<8)),
iReserved(0)
	{
	}

EXPORT_C TInt TScalerCaps::MaxUpscaleLimit() const
	{
	return iMaxUpscaleLimit;
	}
	
EXPORT_C TInt TScalerCaps::MaxDownscaleLimit() const
	{
	return iMaxDownscaleLimit;
	}
	
EXPORT_C TBool TScalerCaps::PowerOfTwoScalingOnly() const
	{
	return iPowerOfTwoScalingOnly;
	}
	
EXPORT_C TBool TScalerCaps::MustPreserveAspectRatio() const
	{
	return iMustPreserveAspectRatio;		
	}
	
EXPORT_C TUint TScalerCaps::Size() const
	{
	return (KSizeMask & iSizeVersion) >> 8;
	}

EXPORT_C TUint TScalerCaps::Version() const
	{
	return (KVersionMask & iSizeVersion);
	}

EXPORT_C TDecodeStreamCaps::TDecodeStreamCaps()
:iMaxBlocksPerRequest(1), iMinBlockSizeInPixels(1,1), iOptimalBlocksPerRequest(1),
iStreamSizeInBlocks(1), iNavigation(TDecodeStreamCaps::ENavigationSequentialForward),
iSizeVersion(KVersionZero|(sizeof(TDecodeStreamCaps)<<8)), iReserved(0)
	{
	}

EXPORT_C TDecodeStreamCaps::TDecodeStreamCaps(TInt aMaxBlocksPerRequest, const TSize& aMinBlockSizeInPixels, 
				  TInt aOptimalBlocksPerRequest, TInt aStreamSizeInBlocks,
				  TDecodeStreamCaps::TNavigation aNavigation)
:iMaxBlocksPerRequest(aMaxBlocksPerRequest), iMinBlockSizeInPixels(aMinBlockSizeInPixels),
iOptimalBlocksPerRequest(aOptimalBlocksPerRequest) ,iStreamSizeInBlocks(aStreamSizeInBlocks),
iNavigation(aNavigation), iSizeVersion(KVersionZero|(sizeof(TDecodeStreamCaps)<<8)), iReserved(0)
	{
	}
		
EXPORT_C TInt TDecodeStreamCaps::MaxBlocksPerRequest() const
	{
	return iMaxBlocksPerRequest;
	}

EXPORT_C const TSize& TDecodeStreamCaps::MinBlockSizeInPixels() const
	{
	return iMinBlockSizeInPixels;
	}

EXPORT_C TInt TDecodeStreamCaps::OptimalBlocksPerRequest() const
	{
	return iOptimalBlocksPerRequest;
	}

EXPORT_C TInt TDecodeStreamCaps::StreamSizeInBlocks() const
	{
	return iStreamSizeInBlocks;
	}

EXPORT_C TDecodeStreamCaps::TNavigation TDecodeStreamCaps::Navigation() const
	{
	return iNavigation;
	}
	
EXPORT_C TUint TDecodeStreamCaps::Size() const
	{
	return (KSizeMask & iSizeVersion) >> 8;
	}

EXPORT_C TUint TDecodeStreamCaps::Version() const
	{
	return (KVersionMask & iSizeVersion);
	}

EXPORT_C TImageConvOperation::TImageConvOperation()
:iExtension(NULL), iReserved(0)
	{
	}
	
EXPORT_C TUint TImageConvOperation::Capabilities() const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	return iExtension->Capabilities();
	}

EXPORT_C void TImageConvOperation::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->AddOperationL(aOperation);
	}

EXPORT_C void TImageConvOperation::ClearOperationStack()
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->ClearOperationStack();
	}
	
void TImageConvOperation::SetExtension(MImageConvExtension* aExtension)
	{
	__ASSERT_ALWAYS(aExtension->Uid() == KICLOperationUid, Panic(EInvalidFwExtensionUid));
	iExtension = static_cast<MImageConvOperation*>(aExtension);
	}

EXPORT_C TImageConvScaler::TImageConvScaler()
:iExtension(NULL), iReserved(0)
	{
	}	

EXPORT_C void TImageConvScaler::GetCapabilities(TScalerCaps& aCaps) const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetCapabilities(aCaps);
	}
	
EXPORT_C void TImageConvScaler::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->SetScalingL(aDesiredSize, aQuality, aLockAspectRatio);
	}
	
EXPORT_C void TImageConvScaler::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->SetScalingL(aScalingCoeff, aScalingQuality);
	}

void TImageConvScaler::SetExtension(MImageConvExtension* aExtension)
	{
	__ASSERT_ALWAYS(aExtension->Uid() == KICLScalerUid, Panic(EInvalidFwExtensionUid));
	iExtension = static_cast<MImageConvScaler*>(aExtension);
	}
	
EXPORT_C TImageConvStreamedDecode::TImageConvStreamedDecode()
:iExtension(NULL), iReserved(0)	
	{
	}	
	
EXPORT_C void TImageConvStreamedDecode::GetBlocks(TRequestStatus& aStatus, CImageFrame& aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt& aNumBlocksRead)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetBlocks(&aStatus, &aFrame, aSeqPosition, aNumBlocksToGet, &aNumBlocksRead);
	}

EXPORT_C TInt TImageConvStreamedDecode::GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	return iExtension->GetBufferSize(aFormat, aBlockSizeInPixels, aNumBlocks);
	}
	
EXPORT_C void TImageConvStreamedDecode::GetNextBlocks(TRequestStatus& aStatus, CImageFrame& aFrame, TInt aNumBlocksToGet, TInt& aNumBlocksRead, TBool& aHaveMoreBlocks)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetNextBlocks(&aStatus, &aFrame, aNumBlocksToGet, &aNumBlocksRead, &aHaveMoreBlocks);
	}
	
EXPORT_C void TImageConvStreamedDecode::GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetCapabilities(aFormat, aFrameNumber, aCaps);
	}
	
EXPORT_C void TImageConvStreamedDecode::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetSupportedFormatsL(aFormats, aOptimalFormat);
	}

EXPORT_C void TImageConvStreamedDecode::InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->InitFrameL(aFormat, aFrameNumber, aNavigation);
	}

void TImageConvStreamedDecode::SetExtension(MImageConvExtension* aExtension)
	{
	__ASSERT_ALWAYS(aExtension->Uid() == KICLStreamedDecodeUid, Panic(EInvalidFwExtensionUid));
	iExtension = static_cast<MImageConvStreamedDecode*>(aExtension);
	}

EXPORT_C TEncodeStreamCaps::TEncodeStreamCaps()
:iMaxBlocksPerRequest(1), iMinBlockSizeInPixels(1,1), iOptimalBlocksPerRequest(1),
iNavigation(TEncodeStreamCaps::ENavigationSequentialForward),
iSizeVersion(KVersionZero|(sizeof(TEncodeStreamCaps)<<8)), iReserved(0)
	{
	}

EXPORT_C TEncodeStreamCaps::TEncodeStreamCaps(TInt aMaxBlocksPerRequest, const TSize& aMinBlockSizeInPixels,
									 TInt aOptimalBlocksPerRequest,
					  				 TEncodeStreamCaps::TNavigation aNavigation)
:iMaxBlocksPerRequest(aMaxBlocksPerRequest), iMinBlockSizeInPixels(aMinBlockSizeInPixels),
iOptimalBlocksPerRequest(aOptimalBlocksPerRequest), iNavigation(aNavigation),
iSizeVersion(KVersionZero|(sizeof(TEncodeStreamCaps)<<8)), iReserved(0)
	{
	}

EXPORT_C TInt TEncodeStreamCaps::MaxBlocksPerRequest() const
	{
	return iMaxBlocksPerRequest;
	}

EXPORT_C const TSize& TEncodeStreamCaps::MinBlockSizeInPixels() const
	{
	return iMinBlockSizeInPixels;
	}

EXPORT_C TInt TEncodeStreamCaps::OptimalBlocksPerRequest() const
	{
	return iOptimalBlocksPerRequest;
	}

EXPORT_C TEncodeStreamCaps::TNavigation TEncodeStreamCaps::Navigation() const
	{
	return iNavigation;
	}

EXPORT_C TUint TEncodeStreamCaps::Size() const
	{
	return (KSizeMask & iSizeVersion) >> 8;
	}

EXPORT_C TUint TEncodeStreamCaps::Version() const
	{
	return (KVersionMask & iSizeVersion);	
	}

EXPORT_C void TImageConvStreamedEncode::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetSupportedFormatsL(aFormats, aOptimalFormat);
	}

EXPORT_C void TImageConvStreamedEncode::GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->GetCapabilities(aFormat, aCaps);
	}

EXPORT_C void TImageConvStreamedEncode::InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->InitFrameL(aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aNavigation, aFrameImageData);
	}

EXPORT_C void TImageConvStreamedEncode::AppendBlocks(TRequestStatus& aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->AppendBlocks(&aStatus, aBlocks, aNumBlocksToAdd);
	}

EXPORT_C void TImageConvStreamedEncode::AddBlocks(TRequestStatus& aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->AddBlocks(&aStatus, aBlocks, aSeqPosition);
	}

EXPORT_C void TImageConvStreamedEncode::Complete(TRequestStatus& aStatus)
	{
	__ASSERT_ALWAYS(iExtension != NULL, Panic(ENullImageConvExtension));
	iExtension->Complete(&aStatus);
	}

EXPORT_C TImageConvStreamedEncode::TImageConvStreamedEncode()
:iExtension(NULL), iReserved(0)
	{
	}

void TImageConvStreamedEncode::SetExtension(MImageConvExtension* aExtension)
	{
	iExtension = static_cast<MImageConvStreamedEncode*>(aExtension);	
	}


