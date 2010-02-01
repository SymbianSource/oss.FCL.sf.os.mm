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
// These are structures for the Image Conversion Library
// to maintain future binary compatibility
// 
//

#ifndef IMAGECONVERSIONPRIV_INL
#define IMAGECONVERSIONPRIV_INL

//
// CImageDecoderPriv
//

inline CImageDecoderPlugin* CImageDecoderPriv::Plugin() const
	{
	return iPlugin;
	}

/**
 * Default version of HandleCustomSyncL - always leaves with KErrNotSupported
 *
 */

inline void CImageDecoderPriv::BodyHandleCustomSyncL(TInt /*aParam*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * Default version of InitCustomAsyncL - always leaves with KErrNotSupported
 *
 */

inline void CImageDecoderPriv::BodyInitCustomAsyncL(TInt /*aParam*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * Default version of NotifyComplete - NO-OP
 *
 */

inline void CImageDecoderPriv::BodyNotifyComplete()
	{
	}

/**
 *
 * Return the number of frames in the image being decoded. This function can
 * be called immediately after the call to create the decoder, thus enabling
 * the caller to know how many frames need to be converted.
 *
 * @return    "TInt"
 *            The number of frames.
 */
inline TInt CImageDecoderPriv::FrameCount() const
	{
	return iFrameInfo.Count();
	}

/**
 *
 * Return the status of the image.
 * If the image is incomplete or not terminated correctly EFalse will be returned
 *
 * @return    "TBool"
 *            Image status.
 *
 */
inline TBool CImageDecoderPriv::IsImageHeaderProcessingComplete() const
	{
	return iHaveCompleteImageHeaders;
	}

inline CImageReadCodec* CImageDecoderPriv::ImageReadCodec() const
	{
	return iImageReadCodec;
	}

inline void CImageDecoderPriv::SetImageReadCodec(CImageReadCodec* aImageReadCodec)
	{
	ASSERT(aImageReadCodec != NULL);
	iImageReadCodec = aImageReadCodec;
	}

inline TInt CImageDecoderPriv::DataLength() const
	{
	return iDataLength;
	}

inline void CImageDecoderPriv::SetDataLength(TInt aDataLength)
	{
	iDataLength = aDataLength;
	}

inline TInt CImageDecoderPriv::StartPosition() const
	{
	return iStartPosition;
	}

inline void CImageDecoderPriv::SetStartPosition(TInt aStartPosition)
	{
	iStartPosition = aStartPosition;
	}

inline const TImageDataBlock* CImageDecoderPriv::ImageData(TInt aIndex) const
	{
	return iImageData.GetImageData(aIndex);
	}

inline TInt CImageDecoderPriv::InsertImageData(const TImageDataBlock* aEntry, TInt aPos)
	{
	return iImageData.InsertImageData(aEntry, aPos);
	}

inline void CImageDecoderPriv::RemoveImageData(TInt aPos)
	{
	iImageData.RemoveImageData(aPos);
	}

inline TInt CImageDecoderPriv::AppendImageData(const TImageDataBlock* aEntry)
	{
	return iImageData.AppendImageData(aEntry);
	}

inline TInt CImageDecoderPriv::ImageDataCount() const
	{
	return iImageData.ImageDataCount();
	}

inline TInt CImageDecoderPriv::AppendImageDataBuffer(const HBufC8* aImageBuffer)
	{
	return iImageData.AppendImageBuffer(aImageBuffer);
	}

inline TInt CImageDecoderPriv::NumberOfFrames() const 
	{
	return iFrameInfo.Count();
	}

inline TInt CImageDecoderPriv::Position() const
	{
	return iPosition;
	}

inline void CImageDecoderPriv::SetPosition(const TInt aPosition)
	{
	iPosition = aPosition;
	}

inline const TFrameInfo& CImageDecoderPriv::ImageInfo() const
	{
	return iImageInfo;
	}

inline void CImageDecoderPriv::SetImageInfo(const TFrameInfo& aImageInfo)
	{
	iImageInfo = aImageInfo;
	}

inline const CFbsBitmap& CImageDecoderPriv::Destination() const
	{
	ASSERT(ValidDestination());
	return *iDestination;
	}

inline TBool CImageDecoderPriv::ValidDestination() const
	{
	return iDestination != NULL;
	}
	
inline const CFbsBitmap& CImageDecoderPriv::DestinationMask() const
	{
	ASSERT(ValidDestinationMask());
	return *iDestinationMask;
	}

inline TBool CImageDecoderPriv::ValidDestinationMask() const
	{
	return iDestinationMask != NULL;
	}

inline TInt CImageDecoderPriv::SourceLength() const
	{
	return iSrcDes.Length();
	}

inline TBool CImageDecoderPriv::MustUseBufferWithDescriptor() const
	{
	return iSupport->MustUseBufferWithDescriptor();
	}

inline void CImageDecoderPriv::CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize)
	{
	iSupport->CopyBufferToDescriptor(aBuffer, aSource, aPosition, aSize);
	}

inline CImageDecoder::TOptions CImageDecoderPriv::DecoderOptions() const
	{
	return iOptions;
	}

//
// CImageEncoderPriv
//

inline CImageEncoderPlugin* CImageEncoderPriv::Plugin() const
	{
	return iPlugin;
	}

/**
 * Default version of HandleCustomSyncL - always leaves with KErrNotSupported
 *
 */

inline void CImageEncoderPriv::BodyHandleCustomSyncL(TInt /*aParam*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * Default version of InitCustomAsyncL - always leaves with KErrNotSupported
 *
 */

inline void CImageEncoderPriv::BodyInitCustomAsyncL(TInt /*aParam*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * Default version of NotifyComplete - NO-OP
 *
 */

inline void CImageEncoderPriv::BodyNotifyComplete()
	{
	}

inline CImageWriteCodec* CImageEncoderPriv::ImageWriteCodec() const
	{
	return iImageWriteCodec;
	}

inline void CImageEncoderPriv::SetImageWriteCodec(CImageWriteCodec* aImageWriteCodec) 
	{
	ASSERT(aImageWriteCodec != NULL);
	iImageWriteCodec = aImageWriteCodec;
	}

inline const CFbsBitmap& CImageEncoderPriv::Source() const
	{
	ASSERT(ValidSource());
	return *iSource;
	}

inline TBool CImageEncoderPriv::ValidSource() const
	{
	return iSource != NULL;
	}

inline TInt& CImageEncoderPriv::StartPosition() 
	{
	return iStartPosition;
	}

inline TInt& CImageEncoderPriv::Position() 	
	{
	return iPosition;
	}

inline const TSize& CImageEncoderPriv::FrameInfoOverallSizeInPixels() const
	{
	return iFrameInfo.iOverallSizeInPixels;
	}

inline CImageEncoder::TOptions CImageEncoderPriv::EncoderOptions() const
	{
	return iOptions;
	}


#endif  // IMAGECONVERSIONPRIV_INL
