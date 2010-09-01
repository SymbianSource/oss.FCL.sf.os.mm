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

#ifndef __IMAGERELAY_H__
#define __IMAGERELAY_H__

#include <caf/caf.h>
using namespace ContentAccess;

#include "icl/ImageCodec.h"
#include "icl/ImageData.h"
#include "ImageConversionPriv.h"
#include "ImageConversion.h"
#include "ImagePrivSupport.h"
#include "icl/imageconversionextension.h"
#include "fwextensionparams.h"

// Forward declaration
class CImageConvExtensionCache;
class CImageConvProxyBase;

class MImageDecoderRelay; // declared here
/**
 * Abstract relay between CImageDecoder and CImageDecoderPriv
 *
 */
NONSHARABLE_CLASS( MImageDecoderRelay ) 
	{
public:
	static MImageDecoderRelay* NewL(CImageDecodeConstruct* aConstruct, TBool aAlwaysThread); // this passes ownership of aConstruct
	virtual ~MImageDecoderRelay();
	virtual void Cancel()=0;

	virtual void SetFileL(RFs& aFs, const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone)=0;
	virtual void SetFileL(RFile& name, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone)=0;
	virtual void SetDataL(RFs& aFs, const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone)=0;

	virtual CImageDecoderPlugin* Plugin() const=0;

	virtual void HandleNewlyOpenedImageL()=0;

	virtual void SetImageTypeL(TInt aImageType)=0;	
	virtual void Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)=0;
	virtual void Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber = 0)=0;
	virtual void ContinueConvert(TRequestStatus* aRequestStatus)=0;

	virtual void ContinueProcessingHeaderL()=0;

	virtual const TFrameInfo& FrameInfo(TInt aFrameNumber) const=0;
	virtual CFrameInfoStrings* FrameInfoStringsLC(TInt aFrameNumber)=0;

	virtual TUid ImplementationUid() const=0;

	virtual TInt FrameCount() const=0;
	virtual TBool IsImageHeaderProcessingComplete() const=0;
	virtual const CFrameImageData& FrameData(TInt aFrameNumber) const=0;

	virtual TInt NumberOfImageComments() const=0;
	virtual HBufC* ImageCommentL(TInt aCommentNumber) const=0;
	virtual TInt NumberOfFrameComments(TInt aFrameNumber) const=0;
	virtual HBufC* FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const=0;

	// custom command support
	virtual void CustomSyncL(TInt aParam)=0;
	virtual void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)=0;

	virtual void TransferConstructOwnership()=0;
	virtual void SetIntent(TIntent aIntent)=0;

	virtual void SetUniqueIdL(const TDesC& aUniqueId)=0;
	virtual TInt SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)=0;

	virtual TInt ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const=0;
	virtual TInt ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const=0;
	virtual TInt SetDecoderThreadPriority(TThreadPriority aPriority) = 0;
	
	// Framework extension
	virtual void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)=0;
	virtual void SetClippingRectL(const TRect* aClipRect)=0;
	virtual TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber = 0)=0;
	virtual CImageConvExtensionCache& ExtensionCache()=0;
	};

// Abstract relay between CImageEncoder and CImageEncoderPriv
class MImageEncoderRelay; // declared here
NONSHARABLE_CLASS( MImageEncoderRelay ) 
	{
public:
	static MImageEncoderRelay* NewL(CImageEncodeConstruct* aConstruct, TBool aAlwaysThread); // this passes ownership of aConstruct
	virtual ~MImageEncoderRelay();
	virtual void Cancel()=0;

	virtual void SetFileL(RFs& aFs, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions)=0;
	virtual void SetFileL(RFile& name, const CImageEncoder::TOptions aOptions)=0;
	virtual void SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions)=0;

	virtual CImageEncoderPlugin* Plugin() const=0;

	virtual void Convert(TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData)=0;
	virtual void SetThumbnail(TBool aDoGenerateThumbnail)=0;

	virtual TUid ImplementationUid() const=0;

	// custom command support
	virtual void CustomSyncL(TInt aParam)=0;
	virtual void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)=0;

	virtual void TransferConstructOwnership()=0;
	
	virtual TInt SetEncoderThreadPriority(TThreadPriority aPriority) = 0;
	
	// Framework extension
	virtual void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)=0;
	virtual CImageConvExtensionCache& ExtensionCache()=0;
	};

// Interface required for framework extension support	
class MImageConvExtensionThreadedSupport
	{
public:
	virtual void ExecuteCommand(TExecuteCommandParamsBase& aExecuteCommandBase) = 0;
	virtual void ExecuteCommandL(TExecuteCommandParamsBase& aExecuteCommandBase) = 0;
	virtual void ExecuteAsyncCommand(TExecuteCommandParamsBase& aExecuteCommandBase) = 0;
	};	

/**
Prepare extension.
*/
NONSHARABLE_CLASS(TImageConvPrepare) 
	{
friend class CImageDecoder;
friend class CImageEncoder;

public:
	void Prepare(TRequestStatus* aRequestStatus);
	
private:
	TImageConvPrepare();
	void SetExtension(MImageConvExtension* aExtension);
	
private:
	MImageConvPrepare* iExtension;
	};

// Class used by relay to hold extension pointers
NONSHARABLE_CLASS( CImageConvExtensionCache ) : public CBase
	{
public:
	static CImageConvExtensionCache* NewL();
	~CImageConvExtensionCache();
	
	TImageConvOperation* Operation();
	TImageConvScaler* Scaler();
	TImageConvStreamedDecode* DecodeBlockStreamer();
	TImageConvStreamedEncode* EncodeBlockStreamer();
	TImageConvPrepare* Prepare();
	
	void SetOperationExtension(TImageConvOperation* aOperationExtension);
	void SetScalerExtension(TImageConvScaler* aScalerExtension);
	void SetBlockStreamerExtension(TImageConvStreamedDecode* aStreamedDecodeExtension);
	void SetBlockStreamerExtension(TImageConvStreamedEncode* aStreamedEncodeExtension);
	void SetPrepareExtension(TImageConvPrepare* aPrepareExtension);
	
	void SetProxyL(CImageConvProxyBase* aProxy);
	void SetExtensionL(TUid aExtUid, MImageConvExtension*& aExtension);

private:
	RPointerArray<CImageConvProxyBase> iProxyList;
	
	TImageConvOperation* iOperationExtension;
	TImageConvScaler* iScalerExtension;
	TImageConvStreamedDecode* iStreamedDecodeExtension;
	TImageConvStreamedEncode* iStreamedEncodeExtension;
	TImageConvPrepare* iPrepareExtension;
	};

#endif // __IMAGERELAY_H__

