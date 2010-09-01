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

#include "ImageStraightRelay.h"
#include "icl/ImageConstruct.h"
#include "icl/ImagePlugin.h"
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "fwextconstants.h"
#include "ImageClientMain.h"
#include "iclextproxystubutil.h"

//
// CImageDecoderStraightRelay
//

CImageDecoderStraightRelay* CImageDecoderStraightRelay::NewL(CImageDecodeConstruct* aConstruct)
	{
	CImageDecoderStraightRelay* self = new (ELeave) CImageDecoderStraightRelay(aConstruct);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageDecoderStraightRelay::CImageDecoderStraightRelay(CImageDecodeConstruct* aConstruct):
	iConstruct(aConstruct)
	{}

void CImageDecoderStraightRelay::ConstructL()
	{
	iBody = CImageDecoderPriv::NewL(iConstruct, this);
	iBody->CreatePluginL();
	iExtensionCache = CImageConvExtensionCache::NewL();
	}

CImageDecoderStraightRelay::~CImageDecoderStraightRelay()
	{
	Cancel();
	delete iExtensionCache;
	delete iBody;
	if (iOwnConstruct)
		{
		delete iConstruct;
		}
	}

void CImageDecoderStraightRelay::Cancel()
	{
	if (iBody)
		iBody->Cancel();
	}

// forwarding functions

void CImageDecoderStraightRelay::SetFileL(RFs& aFs, const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions)
	{
	iBody->SetFileL(aFs, aSourceFilename, aOptions);
	}

void CImageDecoderStraightRelay::SetFileL(RFile& aFile, const CImageDecoder::TOptions aOptions)
	{
	iBody->SetFileL(aFile, aOptions);
	}

void CImageDecoderStraightRelay::SetDataL(RFs& aFs, const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions)
	{
	iBody->SetDataL(aFs, aSourceData, aOptions);
	}

CImageDecoderPlugin* CImageDecoderStraightRelay::Plugin() const
	{
	return iBody->Plugin();
	}

void CImageDecoderStraightRelay::HandleNewlyOpenedImageL()
	{
	iBody->HandleNewlyOpenedImageL();
	}

void CImageDecoderStraightRelay::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)
	{
	iBody->Convert(iMyThread, aRequestStatus, aDestination, aFrameNumber);
	}

void CImageDecoderStraightRelay::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber)
	{
	iBody->Convert(iMyThread, aRequestStatus, aDestination, aDestinationMask, aFrameNumber);
	}

void CImageDecoderStraightRelay::SetImageTypeL(TInt aImageType)
	{
	iBody->SetImageTypeL(aImageType);
	}

void CImageDecoderStraightRelay::ContinueConvert(TRequestStatus* aRequestStatus)
	{
	iBody->ContinueConvert(iMyThread, aRequestStatus);
	}

void CImageDecoderStraightRelay::ContinueProcessingHeaderL()
	{
	iBody->ContinueProcessingHeaderL();
	}

const TFrameInfo& CImageDecoderStraightRelay::FrameInfo(TInt aFrameNumber) const
	{
	return iBody->FrameInfo(aFrameNumber);
	}

CFrameInfoStrings* CImageDecoderStraightRelay::FrameInfoStringsLC(TInt aFrameNumber)
	{
	return iBody->FrameInfoStringsLC(aFrameNumber);
	}

TUid CImageDecoderStraightRelay::ImplementationUid() const
	{
	return iBody->ImplementationUid();
	}

TInt CImageDecoderStraightRelay::FrameCount() const
	{
	return iBody->FrameCount();
	}

TBool CImageDecoderStraightRelay::IsImageHeaderProcessingComplete() const
	{
	return iBody->IsImageHeaderProcessingComplete();
	}

const CFrameImageData& CImageDecoderStraightRelay::FrameData(TInt aFrameNumber) const
	{
	return iBody->FrameData(aFrameNumber);
	}

TInt CImageDecoderStraightRelay::NumberOfImageComments() const
	{
	return Plugin()->NumberOfImageComments();
	}

HBufC* CImageDecoderStraightRelay::ImageCommentL(TInt aCommentNumber) const
	{
	return Plugin()->ImageCommentL(aCommentNumber);
	}

TInt CImageDecoderStraightRelay::NumberOfFrameComments(TInt aFrameNumber) const
	{
	return Plugin()->NumberOfFrameComments(aFrameNumber);
	}

HBufC* CImageDecoderStraightRelay::FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const
	{
	return Plugin()->FrameCommentL(aFrameNumber, aCommentNumber);
	}

void CImageDecoderStraightRelay::CustomSyncL(TInt aParam)
	{
	iBody->CustomSyncL(aParam);
	}

void CImageDecoderStraightRelay::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	iBody->CustomAsync(iMyThread, aRequestStatus, aParam);
	}

void CImageDecoderStraightRelay::TransferConstructOwnership()
	{
	iOwnConstruct = ETrue;
	}

TBool CImageDecoderStraightRelay::MustUseBufferWithDescriptor() const
	{
	return EFalse;
	}

void CImageDecoderStraightRelay::CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize)
	{
	ASSERT(EFalse); // should never get here
	aBuffer = aSource.Mid(aPosition, aSize); // but this is the required behaviour if we were to
	}

TBool CImageDecoderStraightRelay::AmInThread() const
	{
	return EFalse;
	}

TBool CImageDecoderStraightRelay::ShouldAbort() const
	{
	return EFalse;
	}

void CImageDecoderStraightRelay::Lock()
	{
	}

void CImageDecoderStraightRelay::Unlock()
	{
	}

void CImageDecoderStraightRelay::SaveFrameCount(TInt /*aFrameCount*/)
	{
	}

void CImageDecoderStraightRelay::SetIntent(TIntent aIntent)
	{
	iBody->SetIntent(aIntent);
	}

void CImageDecoderStraightRelay::SetUniqueIdL(const TDesC& aUniqueId)
	{
	iBody->SetUniqueIdL(aUniqueId);
	}

TInt CImageDecoderStraightRelay::SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)
	{
	return iBody->SetAgentProperty(aProperty, aValue);
	}

TInt CImageDecoderStraightRelay::ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const
	{
	return iBody->ReductionFactor(aOriginalSize, aReducedSize);
	}
	
TInt CImageDecoderStraightRelay::ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const	
	{
	return iBody->ReducedSize(aOriginalSize, aReductionFactor, aReducedSize);
	}
	
TInt CImageDecoderStraightRelay::SetDecoderThreadPriority(TThreadPriority /*aPriority*/)
	{
	return KErrNotSupported;
	}
	
void CImageDecoderStraightRelay::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	__ASSERT_ALWAYS(aExtPtr == NULL, Panic(ENonNullImageConvExtension));
	
	CImageConvProxyBase* proxy = ProxyStubUtility::GetNewStraightProxyL(aExtUid, iBody, iMyThread);
	CleanupStack::PushL(proxy);
	iBody->GetExtensionL(aExtUid, aExtPtr);
	proxy->SetupExtension(aExtPtr);
	iExtensionCache->SetProxyL(proxy);
	CleanupStack::Pop(proxy);
	//aExtPtr contains pointer to the proxy
	}

void CImageDecoderStraightRelay::SetClippingRectL(const TRect* aClipRect)
	{
	iBody->SetClippingRectL(aClipRect);
	}

TInt CImageDecoderStraightRelay::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	return iBody->GetDestinationSize(aSize, aFrameNumber);
	}
	
CImageConvExtensionCache& CImageDecoderStraightRelay::ExtensionCache()
	{
	return *iExtensionCache;
	}

//
// CImageEncoderStraightRelay
//

CImageEncoderStraightRelay* CImageEncoderStraightRelay::NewL(CImageEncodeConstruct* aConstruct)
	{
	CImageEncoderStraightRelay* self = new (ELeave) CImageEncoderStraightRelay(aConstruct);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImageEncoderStraightRelay::CImageEncoderStraightRelay(CImageEncodeConstruct* aConstruct):
	iConstruct(aConstruct)
	{}

void CImageEncoderStraightRelay::ConstructL()
	{
	iBody = CImageEncoderPriv::NewL(iConstruct, this);
	iBody->CreatePluginL();
	iExtensionCache = CImageConvExtensionCache::NewL();
	}

CImageEncoderStraightRelay::~CImageEncoderStraightRelay()
	{
	Cancel();
	delete iExtensionCache;
	delete iBody;
	if (iOwnConstruct)
		delete iConstruct;
	}

void CImageEncoderStraightRelay::Cancel()
	{
	if (iBody)
		iBody->Cancel();
	}

// forwarding functions

void CImageEncoderStraightRelay::SetFileL(RFs& aFs, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions)
	{
	iBody->SetFileL(aFs, aDestinationFilename, aOptions);
	}
void CImageEncoderStraightRelay::SetFileL(RFile& aFile, const CImageEncoder::TOptions aOptions)
	{
	iBody->SetFileL(aFile, aOptions);
	}

void CImageEncoderStraightRelay::SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions)
	{
	iBody->SetDataL(aDestinationData, aOptions);
	}

CImageEncoderPlugin* CImageEncoderStraightRelay::Plugin() const
	{
	return iBody->Plugin();
	}

void CImageEncoderStraightRelay::Convert(TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData)
	{
	iBody->Convert(iMyThread, aRequestStatus, aSource, aFrameImageData);
	}

TUid CImageEncoderStraightRelay::ImplementationUid() const
	{
	return iBody->ImplementationUid();
	}

void CImageEncoderStraightRelay::CustomSyncL(TInt aParam)
	{
	iBody->CustomSyncL(aParam);
	}

void CImageEncoderStraightRelay::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	iBody->CustomAsync(iMyThread, aRequestStatus, aParam);
	}

void CImageEncoderStraightRelay::TransferConstructOwnership()
	{
	iOwnConstruct = ETrue;
	}

TBool CImageEncoderStraightRelay::AmInThread() const
	{
	return EFalse;
	}

TBool CImageEncoderStraightRelay::ShouldAbort() const
	{
	return EFalse;
	}

void CImageEncoderStraightRelay::SetThumbnail(TBool aDoGenerateThumbnail)
	{
	iBody->SetThumbnail(aDoGenerateThumbnail);
	}

TInt CImageEncoderStraightRelay::SetEncoderThreadPriority(TThreadPriority /*aPriority*/)
	{
	return KErrNotSupported;
	}

void CImageEncoderStraightRelay::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	__ASSERT_ALWAYS(aExtPtr == NULL, Panic(ENonNullImageConvExtension));
	
	CImageConvProxyBase* proxy = ProxyStubUtility::GetNewStraightProxyL(aExtUid, iBody, iMyThread);
	CleanupStack::PushL(proxy);
	iBody->GetExtensionL(aExtUid, aExtPtr);
	proxy->SetupExtension(aExtPtr);
	iExtensionCache->SetProxyL(proxy);
	CleanupStack::Pop(proxy);
	//aExtPtr contains pointer to the proxy
	}

CImageConvExtensionCache& CImageEncoderStraightRelay::ExtensionCache()
	{
	return *iExtensionCache;
	}



