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

#include "fwexttestconvert.h"
#include "fwexttestcodec.h"
#include "imageframe.h"
#include <icl/icl_uids.hrh>
#include <e32property.h>
#include "fwextconstants.h"

// Concrete Decode Transform Extension class
CDecodeOperationExtension* CDecodeOperationExtension::NewL(CFwExtTestDecoderPlugin* aParent)
	{
	return new (ELeave) CDecodeOperationExtension(aParent);
	}

CDecodeOperationExtension::CDecodeOperationExtension(CFwExtTestDecoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1), iOperationCaps(0x0F)
	{	
	}

CDecodeOperationExtension::~CDecodeOperationExtension()
	{
	ASSERT(iReferenceCount == 0);
	}	
	
TUid CDecodeOperationExtension::Uid() const
	{
	return iParent->iTestOperationExtPtr->Uid();
	}
	
void CDecodeOperationExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestOperationExtPtr->IncrementRef();
	}
	
void CDecodeOperationExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestOperationExtPtr->Release();
	}
	
TUint CDecodeOperationExtension::Capabilities() const
	{
	return iOperationCaps;
	}
	
void CDecodeOperationExtension::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	/*
	This bitwise OR is purely for testing purposes only.  An actual
	AddOperationL call would append the operation to a stack of operations.
	*/
	iOperations |= aOperation;
	iParent->iTestOperationExtPtr->AddOperationL(aOperation);
	}

void CDecodeOperationExtension::ClearOperationStack()
	{
	iOperations = 0;
	iParent->iTestOperationExtPtr->ClearOperationStack();
	}

// Concrete Scaler Extension class
CScalerExtension* CScalerExtension::NewL(CFwExtTestDecoderPlugin* aParent)
	{
	return new (ELeave) CScalerExtension(aParent);
	}

CScalerExtension::CScalerExtension(CFwExtTestDecoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1)
	{
	}	

CScalerExtension::~CScalerExtension()
	{
	ASSERT(iReferenceCount == 0);
	}	
	
TUid CScalerExtension::Uid() const
	{
	return iParent->iTestScalerExtPtr->Uid();
	}

void CScalerExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestScalerExtPtr->IncrementRef();
	}
	
void CScalerExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestScalerExtPtr->Release();
	}

void CScalerExtension::GetCapabilities(TScalerCaps& aCaps) const
	{
	aCaps = iScalerCaps;
	iParent->iTestScalerExtPtr->GetCapabilities(aCaps);
	}

void CScalerExtension::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	iParent->iTestScalerExtPtr->SetScalingL(aDesiredSize, aQuality, aLockAspectRatio);
	}
	
void CScalerExtension::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	iParent->iTestScalerExtPtr->SetScalingL(aScalingCoeff, aScalingQuality);
	}

// Concrete Streamed Decode Extension class
CStreamedDecodeExtension* CStreamedDecodeExtension::NewL(CFwExtTestDecoderPlugin* aParent)
	{
	return new (ELeave) CStreamedDecodeExtension(aParent);
	}

CStreamedDecodeExtension::CStreamedDecodeExtension(CFwExtTestDecoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1)
	{
	}
	
CStreamedDecodeExtension::~CStreamedDecodeExtension()
	{
	ASSERT(iReferenceCount == 0);
	}	
	
TUid CStreamedDecodeExtension::Uid() const
	{
	return iParent->iTestStreamedDecodeExtPtr->Uid();
	}

void CStreamedDecodeExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestStreamedDecodeExtPtr->IncrementRef();
	}

void CStreamedDecodeExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestStreamedDecodeExtPtr->Release();
	}

void CStreamedDecodeExtension::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	iParent->iTestStreamedDecodeExtPtr->GetSupportedFormatsL(aFormats, aOptimalFormat);
	}
	
void CStreamedDecodeExtension::GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const
	{
	iParent->iTestStreamedDecodeExtPtr->GetCapabilities(aFormat, aFrameNumber, aCaps);
	}
	
TInt CStreamedDecodeExtension::GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	return iParent->iTestStreamedDecodeExtPtr->GetBufferSize(aFormat, aBlockSizeInPixels, aNumBlocks);
	}
	
void CStreamedDecodeExtension::InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	iParent->iTestStreamedDecodeExtPtr->InitFrameL(aFormat, aFrameNumber, aNavigation);
	}
	
void CStreamedDecodeExtension::GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	iParent->iTestStreamedDecodeExtPtr->GetBlocks(aStatus, aFrame, aSeqPosition, aNumBlocksToGet, aNumBlocksRead);
	}
	
void CStreamedDecodeExtension::GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	iParent->iTestStreamedDecodeExtPtr->GetNextBlocks(aStatus, aFrame, aNumBlocksToGet, aNumBlocksRead, aHaveMoreBlocks);
	}
	
// Concrete Prepare Extension class
CPrepareDecodeExtension* CPrepareDecodeExtension::NewL(CFwExtTestDecoderPlugin* aParent)
	{
	return new (ELeave) CPrepareDecodeExtension(aParent);
	}

CPrepareDecodeExtension::CPrepareDecodeExtension(CFwExtTestDecoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1)
	{
	}
	
CPrepareDecodeExtension::~CPrepareDecodeExtension()
	{
	ASSERT(iReferenceCount == 0);
	}	
	
TUid CPrepareDecodeExtension::Uid() const
	{
	return iParent->iTestPrepareExtPtr->Uid();
	}

void CPrepareDecodeExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestPrepareExtPtr->IncrementRef();
	}

void CPrepareDecodeExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestPrepareExtPtr->Release();
	}

void CPrepareDecodeExtension::Prepare(TRequestStatus* aStatus)
	{
	iParent->iTestPrepareExtPtr->Prepare(aStatus);
	iParent->iReadCodec->SetPrepareProcessing(); // indcate that doing Prepare processing
	}
	
// Concrete Encode Transform Extension class
CEncodeOperationExtension* CEncodeOperationExtension::NewL(CFwExtTestEncoderPlugin* aParent)
	{
	return new (ELeave) CEncodeOperationExtension(aParent);
	}

CEncodeOperationExtension::CEncodeOperationExtension(CFwExtTestEncoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1), iOperationCaps(0x0F)
	{	
	}

CEncodeOperationExtension::~CEncodeOperationExtension()
	{
	ASSERT(iReferenceCount == 0);
	}	
	
TUid CEncodeOperationExtension::Uid() const
	{
	return iParent->iTestOperationExtPtr->Uid();
	}
	
void CEncodeOperationExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestOperationExtPtr->IncrementRef();
	}
	
void CEncodeOperationExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestOperationExtPtr->Release();
	}
	
TUint CEncodeOperationExtension::Capabilities() const
	{
	return iOperationCaps;
	}
	
void CEncodeOperationExtension::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	/*
	This bitwise OR is purely for testing purposes only.  An actual
	AddOperationL call would append the operation to a stack of operations.
	*/
	iOperations |= aOperation;
	iParent->iTestOperationExtPtr->AddOperationL(aOperation);
	}

void CEncodeOperationExtension::ClearOperationStack()
	{
	iOperations = 0;
	iParent->iTestOperationExtPtr->ClearOperationStack();
	}

// Concrete Prepare Extension class for encoder
CPrepareEncodeExtension* CPrepareEncodeExtension::NewL(CFwExtTestEncoderPlugin* aParent)
	{
	return new (ELeave) CPrepareEncodeExtension(aParent);
	}

CPrepareEncodeExtension::CPrepareEncodeExtension(CFwExtTestEncoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1)
	{
	}
	
CPrepareEncodeExtension::~CPrepareEncodeExtension()
	{
	ASSERT(iReferenceCount == 0);
	}	
	
TUid CPrepareEncodeExtension::Uid() const
	{
	return iParent->iTestPrepareExtPtr->Uid();
	}

void CPrepareEncodeExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestPrepareExtPtr->IncrementRef();
	}

void CPrepareEncodeExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestPrepareExtPtr->Release();
	}

void CPrepareEncodeExtension::Prepare(TRequestStatus* aStatus)
	{
	// test code only - avoids processing framework msgs
	CFwExtWriteCodec* codec = CFwExtWriteCodec::NewL(*iParent);
	iParent->SetImageWriteCodec(codec); // Framework takes ownership
	iParent->SetCodec(codec); // plugin pointer to codec

	iParent->iTestPrepareExtPtr->Prepare(aStatus);
	iParent->iWriteCodec->SetPrepareProcessing(); // indicate that doing Prepare processing
	}

// Concrete Streamed Encode Extension class
CStreamedEncodeExtension* CStreamedEncodeExtension::NewL(CFwExtTestEncoderPlugin* aParent)
	{
	return new (ELeave) CStreamedEncodeExtension(aParent);
	}

CStreamedEncodeExtension::CStreamedEncodeExtension(CFwExtTestEncoderPlugin* aParent)
:iParent(aParent), iReferenceCount(1)
	{
	}

CStreamedEncodeExtension::~CStreamedEncodeExtension()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CStreamedEncodeExtension::Uid() const
	{
	return iParent->iTestStreamedEncodeExtPtr->Uid();
	}

void CStreamedEncodeExtension::IncrementRef()
	{
	iReferenceCount++;
	iParent->iTestStreamedEncodeExtPtr->IncrementRef();
	}

void CStreamedEncodeExtension::Release()
	{
	iReferenceCount--;
	iParent->iTestStreamedEncodeExtPtr->Release();
	}

void CStreamedEncodeExtension::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	iParent->iTestStreamedEncodeExtPtr->GetSupportedFormatsL(aFormats, aOptimalFormat);
	}

void CStreamedEncodeExtension::GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const
	{
	iParent->iTestStreamedEncodeExtPtr->GetCapabilities(aFormat, aCaps);
	}

void CStreamedEncodeExtension::InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData)
	{
	iParent->iTestStreamedEncodeExtPtr->InitFrameL(aFormat, aFrameNumber, aFrameSizeInPixels, aBlockSizeInPixels, aNavigation, aFrameImageData);
	
	// Have sufficient information to create the appropriate writecodec
	CFwExtWriteCodec* codec = CFwExtWriteCodec::NewL(*iParent);
	iParent->SetImageWriteCodec(codec); // Framework takes ownership
	iParent->SetCodec(codec); // plugin pointer to codec
	}

void CStreamedEncodeExtension::AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd)
	{
	iParent->iTestStreamedEncodeExtPtr->AppendBlocks(aStatus, aBlocks, aNumBlocksToAdd);
	}

void CStreamedEncodeExtension::AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition)
	{
	iParent->iTestStreamedEncodeExtPtr->AddBlocks(aStatus, aBlocks, aSeqPosition);
	}

void CStreamedEncodeExtension::Complete(TRequestStatus* aStatus)
	{
	iParent->iTestStreamedEncodeExtPtr->Complete(aStatus);
//	iParent->iWriteCodec->Complete(); // indicate complete to the write codec
	}

// Decoder Plugin
CFwExtTestDecoderPlugin* CFwExtTestDecoderPlugin::NewL()
	{
	return new (ELeave) CFwExtTestDecoderPlugin();
	}
	
void CFwExtTestDecoderPlugin::ImageType(TInt /*aFrameNumber*/, TUid& /*aImageType*/, TUid& /*aImageSubType*/) const
	{
	
	}
	
void CFwExtTestDecoderPlugin::ScanDataL()
	{
	// Just create codec and set in the Priv
	CFwExtReadCodec* imageReadCodec = CFwExtReadCodec::NewL(*this);
	SetImageReadCodec(imageReadCodec);
	iReadCodec = imageReadCodec;
	
	SetDataLength(10);
	
	ReadFrameHeadersL();
	}

CFrameInfoStrings* CFwExtTestDecoderPlugin::FrameInfoStringsL(RFs& /*aFs*/, TInt /*aFrameNumber*/)
	{
	return NULL;
	}

CFwExtTestDecoderPlugin::CFwExtTestDecoderPlugin()
:iDestSize(1234,1234)
	{
	}

CFwExtTestDecoderPlugin::~CFwExtTestDecoderPlugin()
	{
	// test extension shared with test code
	delete iTestOperationExtPtr;
	delete iTestScalerExtPtr;
	delete iTestStreamedDecodeExtPtr;
	
	// actual extensions that point to the above
	delete iOperationExt;
	delete iScalerExt;
	delete iStreamedDecodeExt;
	
	// Call last
	Cleanup();
	}

void CFwExtTestDecoderPlugin::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	switch(aExtUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			iOperationExt = CDecodeOperationExtension::NewL(this);
			aExtPtr = iOperationExt;
			break;
			}
		case KUidImageConvExtScalerValue:
			{
			iScalerExt= CScalerExtension::NewL(this);
			aExtPtr = iScalerExt;
			break;
			}
		case KUidImageConvExtStreamedDecodeValue:
			{
			iStreamedDecodeExt = CStreamedDecodeExtension::NewL(this);
			aExtPtr = iStreamedDecodeExt;
			break;
			}
		case KUidImageConvExtPrepareValue:
			{
			iPrepareExt = CPrepareDecodeExtension::NewL(this);
			aExtPtr = iPrepareExt;
			break;
			}
		default:
			{
			User::Leave(KErrNotSupported);	
			}
		}
	}

TInt CFwExtTestDecoderPlugin::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	aSize.iWidth = aSize.iWidth/2;
	aSize.iHeight = aSize.iHeight/2;
	return aFrameNumber;
	}

void CFwExtTestDecoderPlugin::SetClippingRectL(const TRect* aClipRect)
	{
	if(!aClipRect)
		{
		User::Leave(KErrArgument);
		}
	
	const TRect KClippingRectSize(2,4,6,8);

	if(*aClipRect != KClippingRectSize)
		{
		User::Leave(KErrArgument);
		}
	}

void CFwExtTestDecoderPlugin::HandleCustomSyncL(TInt aParam)
	{
	MImageConvExtension* tempExt = reinterpret_cast<MImageConvExtension*>(aParam);

	if(tempExt->Uid() == KICLOperationUid)
		{
		iTestOperationExtPtr = static_cast<CDecodeOperationExtension*>(tempExt);
		}
	else if(tempExt->Uid() == KICLScalerUid)
		{
		iTestScalerExtPtr = static_cast<CScalerExtension*>(tempExt);
		}
	else if(tempExt->Uid() == KICLStreamedDecodeUid)
		{
		iTestStreamedDecodeExtPtr = static_cast<CStreamedDecodeExtension*>(tempExt);
		}
	else if(tempExt->Uid() == KICLPrepareUid)
		{
		iTestPrepareExtPtr = static_cast<CPrepareDecodeExtension*>(tempExt);
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	}

void CFwExtTestDecoderPlugin::InitCustomAsyncL(TInt /*aParam*/)
	{
	}

// Encoder Plugin	
CFwExtTestEncoderPlugin* CFwExtTestEncoderPlugin::NewL()
	{
	return new (ELeave) CFwExtTestEncoderPlugin();
	}

CFwExtTestEncoderPlugin::CFwExtTestEncoderPlugin()
	{
	}

CFwExtTestEncoderPlugin::~CFwExtTestEncoderPlugin()
	{
	// Call last
	Cleanup();
	}

void CFwExtTestEncoderPlugin::SetCodec(CFwExtWriteCodec* aWriteCodec)
	{
	iWriteCodec = aWriteCodec;
	}

void CFwExtTestEncoderPlugin::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	}
	
void CFwExtTestEncoderPlugin::UpdateHeaderL()
	{
	}

void CFwExtTestEncoderPlugin::HandleCustomSyncL(TInt aParam)
	{
	MImageConvExtension* tempExt = reinterpret_cast<MImageConvExtension*>(aParam);

	if(tempExt->Uid() == KICLOperationUid)
		{
		iTestOperationExtPtr = static_cast<CEncodeOperationExtension*>(tempExt);
		}
	else if(tempExt->Uid() == KICLStreamedEncodeUid)
		{
		iTestStreamedEncodeExtPtr = static_cast<CStreamedEncodeExtension*>(tempExt);
		}
	else if(tempExt->Uid() == KICLPrepareUid)
		{
		iTestPrepareExtPtr = static_cast<CPrepareEncodeExtension*>(tempExt);
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	}	

void CFwExtTestEncoderPlugin::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	switch(aExtUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			iOperationExt = CEncodeOperationExtension::NewL(this);
			aExtPtr = iOperationExt;
			break;
			}
		case KUidImageConvExtStreamedEncodeValue:
			{
			iStreamedEncodeExt = CStreamedEncodeExtension::NewL(this);
			aExtPtr = iStreamedEncodeExt;
			break;
			}
		case KUidImageConvExtPrepareValue:
			{
			iPrepareExt = CPrepareEncodeExtension::NewL(this);
			aExtPtr = iPrepareExt;
			break;
			}
		default:
			{
			User::Leave(KErrNotSupported);	
			}
		}
	}

