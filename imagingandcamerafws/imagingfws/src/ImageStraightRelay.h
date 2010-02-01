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
// Abstract relay between Image{Decoder,Encoder} and Priv object - will shield any threads
// 
//

#ifndef __IMAGESTRAIGHTRELAY_H__
#define __IMAGESTRAIGHTRELAY_H__

#include <caf/caf.h>
using namespace ContentAccess;

#include "icl/ImageCodec.h"
#include "icl/ImageData.h"
#include "ImageRelay.h"
#include "ImageConversion.h"
#include "icl/imageconversionextension.h"

class CImageDecoderPriv;
class CImageEncoderPriv;

// Direct relay between CImageDecoder and CImageDecoderPriv
class CImageDecoderStraightRelay; // declared here
NONSHARABLE_CLASS( CImageDecoderStraightRelay ): public CBase, 
                                                 public MImageDecoderRelay,
                                                 public MImageDecoderPrivSupport
	{
public:
	static CImageDecoderStraightRelay* NewL(CImageDecodeConstruct *aConstruct);
	~CImageDecoderStraightRelay();

	// from MDecoderRelay
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

	void CustomSyncL(TInt aParam);
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);

	void TransferConstructOwnership();

	void SetImageTypeL(TInt aImageType);
	
	// from MImageDecoderPrivSupport
	TBool MustUseBufferWithDescriptor() const;
	void CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize);
	TBool AmInThread() const;
	TBool ShouldAbort() const;
	void Lock();
	void Unlock();
	void SaveFrameCount(TInt aFrameCount);

	// from MImageDecoderRelay
	void SetIntent(TIntent aIntent);
	void SetUniqueIdL(const TDesC& aUniqueId);
	TInt SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue);

	TInt ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const;
	TInt ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const;
	
	TInt SetDecoderThreadPriority(TThreadPriority aPriority);
	
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	void SetClippingRectL(const TRect* aClipRect);
	TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber = 0);
	CImageConvExtensionCache& ExtensionCache();

protected:
	CImageDecoderStraightRelay(CImageDecodeConstruct* aConstruct);
	void ConstructL();

private:
	CImageDecodeConstruct* iConstruct;
	TBool iOwnConstruct;
	CImageDecoderPriv* iBody;
	RThread iMyThread;
	CImageConvExtensionCache* iExtensionCache;
	};

// Straight relay between CImageEncoder and CImageEncoderPriv
class CImageEncoderStraightRelay; // declared here
NONSHARABLE_CLASS( CImageEncoderStraightRelay ): public CBase, 
                                                 public MImageEncoderRelay,
								                 public MImageEncoderPrivSupport
	{
public:
	static CImageEncoderStraightRelay* NewL(CImageEncodeConstruct* aConstruct);
	~CImageEncoderStraightRelay();

	// from MEncoderRelay
	void Cancel();

	void SetFileL(RFs& aFs, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions);
	void SetFileL(RFile& name, const CImageEncoder::TOptions aOptions);
	void SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions);

	CImageEncoderPlugin* Plugin() const;

	void Convert(TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData);

	TUid ImplementationUid() const;

	void CustomSyncL(TInt aParam);
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam);

	void TransferConstructOwnership();
	
	void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	CImageConvExtensionCache& ExtensionCache();

	// from MImageEncoderPrivSupport
	TBool AmInThread() const;
	TBool ShouldAbort() const;

	void SetThumbnail(TBool aDoGenerateThumbnail);
	
	TInt SetEncoderThreadPriority(TThreadPriority aPriority);

protected:
	CImageEncoderStraightRelay(CImageEncodeConstruct* aConstruct);
	void ConstructL();

private:
	CImageEncodeConstruct* iConstruct;
	TBool iOwnConstruct;
	CImageEncoderPriv* iBody;
	RThread iMyThread;
	CImageConvExtensionCache* iExtensionCache;
	};

#endif // __IMAGESTRAIGHTRELAY_H__

