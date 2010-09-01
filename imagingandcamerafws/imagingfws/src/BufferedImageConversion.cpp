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

#include "ImageConversion.h"
#include "ImageClientMain.h"

// BUFFERED IMAGE DECODER

/**
Creates a buffered decoder.

The function leaves if the decoder object cannot be created or initialised.

@param  aFs
        A reference to a file server session for the decoder to use.

@return A pointer to the newly created decoder.
*/
EXPORT_C CBufferedImageDecoder* CBufferedImageDecoder::NewL(RFs& aFs)
	{
	CBufferedImageDecoder* self = new (ELeave) CBufferedImageDecoder(aFs);
	return self;
	}

/**
Creates a decoder for the image in the source buffer. The client supplies
a MIME type which will be used to try and select an appropriate plugin
decoder. If it finds a decoder it creates it and then goes on to use that
decoder to scan the beginning of the image file.

@param  aSourceData
        The buffer containing the image to be decoded.
@param  aMIMEType
        The MIME type of the image in the buffer.
@param  aOptions
        The options to use.

@leave  KEComErrNoInterfaceIdentified
        No plugin decoder could be found for the specified image.
*/
EXPORT_C void CBufferedImageDecoder::OpenL(const TDesC8& aSourceData, const TDesC8& aMIMEType, const CImageDecoder::TOptions aOptions)
	{
	__ASSERT_ALWAYS(iState == EStateCreated, Panic(EInvalidState));

	ASSERT(iMimeType.Ptr() == NULL);
	const TInt mimeLength = aMIMEType.Length();
	TUint8* ptr = STATIC_CAST(TUint8*, User::AllocL(mimeLength));
	iMimeType.Set(ptr, mimeLength, mimeLength);
	iMimeType.Copy(aMIMEType);

	iOptions = aOptions;
	iImageTypeOpen = EFalse;

	AppendDataL(aSourceData);

	iState = EStateOpening;

	ContinueOpenL();

	}

/**
Creates a decoder for the image in the source buffer. 

If the client supplies an image type (and sub-type, if applicable) or decoder uid, these will be
used to try and select an appropriate plugin decoder. If not, then the selection
will be done by matching the image header from the buffer. If it finds a decoder
it creates it and then goes on to use that decoder to scan the beginning of
the image buffer. If a decoder could not be created append data using AppendDataL()
and call ContinueOpenL().

@param  aSourceData
        The buffer containing the image to be decoded.
@param  aOptions
        The decoder options to use.
@param  aImageType
        The image type of the image in the file (optional).
@param  aImageSubType
        The image sub-type of the image in the file (optional).
@param  aDecoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface
@leave  KErrUnderflow
        Not enough data in descriptor to identify which plugin decoder to use.
@leave  KEComErrNoInterfaceIdentified
        No plugin decoder could be found for the specified image.
@leave  KErrNotFound
        The specified codec could not be found.
*/
EXPORT_C void CBufferedImageDecoder::OpenL(const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	__ASSERT_ALWAYS(iState == EStateCreated,Panic(EInvalidState));

	iOptions = aOptions;
	iImageType = aImageType;
	iImageSubType = aImageSubType;
	iDecoderUid = aDecoderUid;
	iImageTypeOpen = ETrue;

	AppendDataL(aSourceData);

	iState = EStateOpening;

	ContinueOpenL();
	}

/**
Call this function to retry to create a decoder after OpenL() returned with
KErrUnderFlow and extra data was added to the descriptor using AppendDataL().
This function can be recalled until ValidDecoder() return ETrue.

@leave  KErrUnderflow
        Not enough data in descriptor to identify which plugin decoder to use.
*/
EXPORT_C void CBufferedImageDecoder::ContinueOpenL()
	{

	__ASSERT_ALWAYS(iState == EStateOpening, Panic(EInvalidState));
	TInt error = KErrNone;

	if(!ValidDecoder())
		{
		if(iImageTypeOpen)
			{
			TRAP(error, iDecoder = CImageDecoder::DataNewL(iFileSession, iBuffer, iOptions, iImageType, iImageSubType, iDecoderUid));
			}
		else
			{
			TRAP(error, iDecoder = CImageDecoder::DataNewL(iFileSession, iBuffer, iMimeType, iOptions));
			}
		}

	if(error != KErrNone)
		{
		if(error != KErrUnderflow)
			User::Leave(error);
		}
	else
		iState = EStateOpen;

	}

/**
Destructor for this class.

Stops decoding if it is in progress and frees all resources owned by the object prior to its destruction.
*/
EXPORT_C CBufferedImageDecoder::~CBufferedImageDecoder()
	{
	Cancel();
	Reset();
	ASSERT(iDecoder == NULL); // make sure decoder is deleted
	}

/**
Starts decoding an image frame asynchronously.

When converting the operation can complete with KErrUnderflow, if there is insufficient
information in the descriptor. In this situation, ContinueConvert() should be called repeatedly
until the descriptor has accumulated enough information for ContinueConvert() to complete with KErrNone.

@param  aRequestStatus
        The request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
@param  aDestination
        A bitmap that will contain the decoded frame. The bitmap
        must be created before the call and must be large enough
        for the frame and set to the required display mode.
        FrameInfo() can be used to obtain the size and display mode
        of the frame.
@param  aFrameNumber
        The frame in multi-frame image to decode (optional).
*/
EXPORT_C void CBufferedImageDecoder::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)
	{
	__ASSERT_ALWAYS(iState == EStateOpen || iState == EStateConverting, Panic(EInvalidState));
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	iDecoder->Convert(aRequestStatus, aDestination, aFrameNumber);
	iState = EStateConverting;
	}

/**
Starts decoding an image frame and mask asynchronously.

When converting the operation can complete with KErrUnderflow, if there is insufficient
information in the descriptor. In this situation, ContinueConvert() should be called repeatedly
until the descriptor has accumulated enough information for ContinueConvert() to complete with KErrNone.

@param  aRequestStatus
        The request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
@param  aDestination
        A bitmap that will contain the decoded frame. The bitmap
        must be created before the call and must be large enough
        for the frame and set to the required display mode.
        FrameInfo() can be used to obtain the size and display mode
        of the frame.
@param  aDestinationMask
        A bitmap that will contain the decoded frame mask. The bitmap
        must be created before the call and must be large enough
        for the mask. The display mode must be EGray2 or EGray256 and
        must be EGray256 if the image contains alpha-blending information.
        This information can be obtained from the iFlags property and
        TFrameInfoFlags of TFrameInfo
        obtained from a FrameInfo() call.
@param  aFrameNumber
        The frame in multi-frame image to decode (optional).
*/
EXPORT_C void CBufferedImageDecoder::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber)
	{
	__ASSERT_ALWAYS(iState == EStateOpen || iState == EStateConverting, Panic(EInvalidState));
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	iDecoder->Convert(aRequestStatus, aDestination, aDestinationMask, aFrameNumber);
	iState = EStateConverting;
	}

/**
Continues decoding a frame and/or mask after new image data was added to
the source file or descriptor and a previous call to Convert() or
ContinueConvert() returned KErrUnderflow.

@param  aRequestStatus
        The request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
*/
EXPORT_C void CBufferedImageDecoder::ContinueConvert(TRequestStatus* aRequestStatus)
	{
	__ASSERT_ALWAYS(iState == EStateConverting, Panic(EInvalidState));
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	iDecoder->ContinueConvert(aRequestStatus);
	}

/**
Reset the decoder for reuse. 

Follow this call with OpenL() and new source data.
*/
EXPORT_C void CBufferedImageDecoder::Reset()
	{
	delete iDecoder; iDecoder = NULL;
	User::Free(STATIC_CAST(TAny*,CONST_CAST(TUint8*,iBuffer.Ptr())));
	iBuffer.Set(NULL, 0 ,0);
	User::Free(STATIC_CAST(TAny*,CONST_CAST(TUint8*,iMimeType.Ptr())));
	iMimeType.Set(NULL, 0 ,0);
	iImageType = KNullUid;
	iImageSubType = KNullUid;
	iDecoderUid = KNullUid;
	iOptions = CImageDecoder::EOptionNone;
	iState = EStateCreated;
	}

/**
Requests an asynchronous decode to terminate. 

Cancels any conversions currently in progress.
*/
EXPORT_C void CBufferedImageDecoder::Cancel()
	{
	if(iDecoder)
		iDecoder->Cancel();
	}

/**
Adds new image data. 

A copy of the data is held internally, and the caller does not need to retain the data.

@param  aData
        The new image data to append.
*/
EXPORT_C void CBufferedImageDecoder::AppendDataL(const TDesC8& aData)
	{
	// Resize the buffer
	TInt oldLength = iBuffer.Length();
	TInt newLength = oldLength + aData.Length();

	if(iBuffer.Ptr() == NULL)
		{
		TUint8* ptr = STATIC_CAST(TUint8*,User::AllocL(newLength));
		iBuffer.Set(ptr, newLength, newLength);
		iBuffer.Copy(aData);
		}

	if(iBuffer.MaxLength() < newLength)
		{
		TAny* oldBuffer  = STATIC_CAST(TAny*, CONST_CAST(TUint8*, iBuffer.Ptr()));
		TUint8* newBuffer = STATIC_CAST(TUint8*, User::ReAllocL(oldBuffer,newLength));
		iBuffer.Set(newBuffer,oldLength, newLength);
		iBuffer.Append(aData);
		}
	}

/**
Returns the number of frames in the image being decoded. This function can
be called immediately after the call to create the decoder, thus enabling
the caller to know how many frames need to be converted.

@return The number of frames.
*/
EXPORT_C TInt CBufferedImageDecoder::FrameCount() const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->FrameCount();
	}
/**
Returns the status of the image.

If the image is incomplete or not terminated correctly EFalse will be returned

@return A boolean indicating the image status. Returns ETrue if image header processing is complete,
        otherwise EFalse.
*/
EXPORT_C TBool CBufferedImageDecoder::IsImageHeaderProcessingComplete() const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->IsImageHeaderProcessingComplete();
	}

/**
Continues processing image headers after new image data was appended.

This function can be called until IsImageHeaderProcessingComplete()
return ETrue.
*/
EXPORT_C void CBufferedImageDecoder::ContinueProcessingHeaderL()
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	iDecoder->ContinueProcessingHeaderL();
	}

/**
Returns the frame info for a specified frame of the image.

This function can be called immediately after the call to create the decoder, thus enabling
the caller to know about each frame in advance of converting it.

@param  aFrameNumber
        The frame number.

@return The information for the specified frame.
*/
EXPORT_C const TFrameInfo& CBufferedImageDecoder::FrameInfo(TInt aFrameNumber) const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->FrameInfo(aFrameNumber);
	}

/**
Returns the image data for a specific frame.

Use FrameCount() to determine how many frames are contained in the image before using this function.

@param  aFrameNumber
        The frame number for which to return frame data.

@return The data for the specified frame.
*/
EXPORT_C const CFrameImageData& CBufferedImageDecoder::FrameData(TInt aFrameNumber) const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->FrameData(aFrameNumber);
	}
/**
Returns the number of comments attached to the image (as opposed to
a particular frame).

@return The number of comments attached to the image.
*/
EXPORT_C TInt CBufferedImageDecoder::NumberOfImageComments() const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->NumberOfImageComments();
	}

/**
Returns a particular comment attached to the image.
Ownership of the returned buffer is transferred to the caller.

Use NumberOfImageComments() to determine how many (if any) comments are contained within the image.

@param  aCommentNumber
        The comment number.

@return A buffer containing the comment.
*/
EXPORT_C HBufC* CBufferedImageDecoder::ImageCommentL(TInt aCommentNumber) const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->ImageCommentL(aCommentNumber);
	}

/**
Returns the number of comments attached to a given frame of the image
as opposed to the whole image.

Use FrameCount() to retrieve the number of frames in the image to ensure that the value you use for
aFrameNumber is valid.

@param  aFrameNumber
        The frame number.

@return The number of comments attached to a given frame of the image.
*/
EXPORT_C TInt CBufferedImageDecoder::NumberOfFrameComments(TInt aFrameNumber) const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->NumberOfFrameComments(aFrameNumber);
	}

/**
Returns a particular comment attached to a given frame of the image.
Ownership of the returned buffer is transferred to the caller.

@param  aFrameNumber
        The frame number.
@param  aCommentNumber
        The comment number.

@return A buffer containing the comment.
*/
EXPORT_C HBufC* CBufferedImageDecoder::FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->FrameCommentL(aFrameNumber, aCommentNumber);
	}

/**
Returns the formatted frame information strings for a specific frame
and leave it on the cleanup stack.

Ownership is transferred to the caller.

@param  aFrameNumber
        The frame number.

@return The formatted frame information strings.
*/
EXPORT_C CFrameInfoStrings* CBufferedImageDecoder::FrameInfoStringsLC(TInt aFrameNumber)
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->FrameInfoStringsLC(aFrameNumber);
	}

/**
Returns the formatted frame information strings for a specific frame.

Ownership is transferred to the caller.

@param  aFrameNumber
        The frame number.

@return The formatted frame information strings.
*/
EXPORT_C CFrameInfoStrings* CBufferedImageDecoder::FrameInfoStringsL(TInt aFrameNumber)
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->FrameInfoStringsL(aFrameNumber);
	}

/**
Returns the implementation UID of the decoder being used to decode the image.

@return The implementation UID of the decoder.
*/
EXPORT_C TUid CBufferedImageDecoder::ImplementationUid() const
	{
	__ASSERT_ALWAYS(ValidDecoder(),Panic(EDecoderNotCreated));
	return iDecoder->ImplementationUid();
	}

/**
Determine if enough data was available to determine which plugin decoder
to use. If this return EFalse append extra data using AppendDataL() and
and call ContinueOpenL()

@return A boolean indicating if the decoder plugin was successful. ETrue if a decoder plugin was 
        created, otherwise EFalse.
*/
EXPORT_C TBool CBufferedImageDecoder::ValidDecoder () const
	{
	return iDecoder != NULL;
	}
/**
Constructor for this class.
@internalTechnology
*/
CBufferedImageDecoder::CBufferedImageDecoder(RFs& aFs):
	iFileSession(aFs),
	iBuffer(NULL,0,0),
	iMimeType(NULL,0,0),
	iState(EStateCreated)
	{
	}
