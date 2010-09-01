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

#include <bitdev.h>
#include <bautils.h>
#include <barsc.h>
#include "ImageConversion.h"
#include "ImageClientMain.h"
#include "ImageConversionPriv.h"
#include "icl/ImagePlugin.h"
#include "icl/ImageConstruct.h"
#include "EnDecoderUtils.h"

/**
Constructor for this class.
*/
EXPORT_C CImageDecoderPlugin::CImageDecoderPlugin()
	{
	}

/**
Destructor for this class.
*/
EXPORT_C CImageDecoderPlugin::~CImageDecoderPlugin()
	{
	}

/**
Called when the plugin is destroyed or a decode is cancelled. This may be
overriden in derived classes.

Note:
Derived classes must call this version after performing any plugin
specific cleanup.
*/
EXPORT_C void CImageDecoderPlugin::Cleanup()
	{
	if(ValidProperties())
		iProperties->Cleanup();
	}

/**
Initialises data structures prior to decoding a frame.

This function may be overriden in derived classes. Any override should also
call this version after performing any plugin initialistion.
*/
EXPORT_C void CImageDecoderPlugin::InitConvertL()
	{
	ASSERT(ValidProperties());
	iProperties->InitConvertL();
	}
/**
Forces initialization of data structures prior to decoding a frame.

@param	aFrameNumber
        The frame in a multi-frame image to decode.
*/
EXPORT_C void CImageDecoderPlugin::RequestInitL(TInt aFrameNumber)
	{
	ASSERT(ValidProperties());
	iProperties->RequestInitL(aFrameNumber);
	}

/**
Performs a decode step. This effectively forms the RunL() call of the decoder.

This call may be overriden in derived classes. However, if this
the case, then if the custom decode is not performed and the derived class should
either ensure that this base class's version is called or should completely replace
the base class's version calling PrepareForProcessFrameL(), ProcessFrameL() and
HandleProcessFrameResult() as appropriate. Unlike the standard version, an override
instance may choose to spread these calls over several RunL() instances.
*/
EXPORT_C void CImageDecoderPlugin::DoConvert()
	{
	ASSERT(ValidProperties());
	iProperties->DoConvert();
	}

/**
Initialises system for ProcessFrameL(). This reads in a new buffer for ProcessFrameL().
*/
EXPORT_C void CImageDecoderPlugin::PrepareForProcessFrameL()
	{
	ASSERT(ValidProperties());
	iProperties->PrepareForProcessFrameL();
	}

/**
Deals with result from ProcessFrameL(). This function processes the results of the standard
ProcessFrameL() call, feeding in the resultant error code from its TRAP and the status result.
It will call RequestComplete() or SelfComplete() as appropriate.

Note that if no data was consumed by ProcessFrameL(), HandleProcessFrameResult() assumes that
it requires more data and calls RequestComplete(KErrUnderflow). If this is not appropriate, an
overloaded DoConvert() should be made to handle it.

@param  aErrCode
        The error result of TRAP arround ProcessFrameL().
@param  aCodecState
        The result of ProcessFrameL() itself.
*/

EXPORT_C void CImageDecoderPlugin::HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)
	{
	ASSERT(ValidProperties());
	iProperties->HandleProcessFrameResult(aErrCode, aCodecState);
	}

/**
Value to be fed to CImageReadCodec::ProcessFrameL().

This value is setup by PrepareForProcessFrameL() - it returns the value that will be
fed to CImageReadCodec::ProcessFrameL(), and will be used by codecs that fully override
DoConvert().
*/
EXPORT_C TBufPtr8& CImageDecoderPlugin::SourceData()
	{
	ASSERT(ValidProperties());
	return iProperties->SourceData();
	}



/**
Returns the plugin's read codec.

@return Pointer to the plugin's read codec.
*/
EXPORT_C CImageReadCodec* CImageDecoderPlugin::ImageReadCodec() const
	{
	ASSERT(ValidProperties());
	return iProperties->ImageReadCodec();
	}

/**
Sets the plugin's read codec. Ownership of the codec is transferred to the plugin.

@param  aImageReadCodec
        Pointer to the codec.
*/
EXPORT_C void CImageDecoderPlugin::SetImageReadCodec(CImageReadCodec* aImageReadCodec)
	{
	ASSERT(ValidProperties());
	iProperties->SetImageReadCodec(aImageReadCodec);
	}

/**
Returns the maximum number of bytes of data that can be decoded.

@return The maximum number of bytes of data.
*/
EXPORT_C TInt CImageDecoderPlugin::DataLength() const
	{
	ASSERT(ValidProperties());
	return iProperties->DataLength();
	}

/**
Sets the maximum number of bytes of data that can be decoded.

@param  aDataLength
        The maximum number of bytes of data.
*/
EXPORT_C void CImageDecoderPlugin::SetDataLength(TInt aDataLength)
	{
	ASSERT(ValidProperties());
	iProperties->SetDataLength(aDataLength);
	}

/**
Returns the starting position of the frame to be decoded.

@return The starting position.
*/
EXPORT_C TInt CImageDecoderPlugin::StartPosition() const
	{
	ASSERT(ValidProperties());
	return iProperties->StartPosition();
	}

/**
Sets the starting position of the frame to be decoded.

@param  aStartPosition
        The starting position in the data.
*/
EXPORT_C void CImageDecoderPlugin::SetStartPosition(TInt aStartPosition)
	{
	ASSERT(ValidProperties());
	iProperties->SetStartPosition(aStartPosition);
	}

/**
Returns the image data block for the specified index.

@param	aIndex
        The index of the image data block to return.

@return The image data block.
*/
EXPORT_C const TImageDataBlock* CImageDecoderPlugin::ImageData(TInt aIndex) const
	{
	ASSERT(ValidProperties());
	return iProperties->ImageData(aIndex);
	}

/**
Inserts an image data block into the internally held array at the specified position.

@param  aEntry
        Pointer to the image data block to be inserted.
@param  aPos
        The position within the arrary to insert the data block.

@return KErrNone if the insertion was successful, otherwise a system
        wide error code.
*/
EXPORT_C TInt CImageDecoderPlugin::InsertImageData(const TImageDataBlock* aEntry, TInt aPos)
	{
	ASSERT(ValidProperties());
	return iProperties->InsertImageData(aEntry, aPos);
	}

/**
Removes a specified image data block from the internally held array.

@param  aPos
        The index of the image data block to be removed.
*/
EXPORT_C void CImageDecoderPlugin::RemoveImageData(TInt aPos)
	{
	ASSERT(ValidProperties());
	iProperties->RemoveImageData(aPos);
	}

/**
Appends a new image data data block to the end of the internally held array.

@param  aEntry
        The image data block to be appended.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
EXPORT_C TInt CImageDecoderPlugin::AppendImageData(const TImageDataBlock* aEntry)
	{
	ASSERT(ValidProperties());
	return iProperties->AppendImageData(aEntry);
	}

/**
Returns the number of image data blocks present in the image data.

@return The number of blocks.
*/
EXPORT_C TInt CImageDecoderPlugin::ImageDataCount() const
	{
	ASSERT(ValidProperties());
	return iProperties->ImageDataCount();
	}

/**
Appends a new image data buffer to the end of the internally held array

@param  aImageBuffer
        The data buffer to append.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
EXPORT_C TInt CImageDecoderPlugin::AppendImageDataBuffer(const HBufC8* aImageBuffer)
	{
	ASSERT(ValidProperties());
	return iProperties->AppendImageDataBuffer(aImageBuffer);
	}

/**
Returns the number of frames to be decoded.

@return The number of frames.
*/
EXPORT_C TInt CImageDecoderPlugin::NumberOfFrames() const
	{
	ASSERT(ValidProperties());
	return iProperties->NumberOfFrames();
	}

/**
Returns the current position within the data.

@return The current position.
*/
EXPORT_C TInt CImageDecoderPlugin::Position() const
	{
	ASSERT(ValidProperties());
	return iProperties->Position();
	}

/**
Sets the current position in the data.

@param  aPosition
        The current position in the data.
*/
EXPORT_C void CImageDecoderPlugin::SetPosition(const TInt aPosition)
	{
	ASSERT(ValidProperties());
	iProperties->SetPosition(aPosition);
	}

/**
Must be called on completion of decoding the image data.

@param  aReason
        KErrNone should be returned if the decoding completes successfully. If the
		request fails an appropriate error code should be returned.

@see    CImageDecoderPlugin::SelfComplete(TInt aReason)
*/
EXPORT_C void CImageDecoderPlugin::RequestComplete(TInt aReason)
	{
	ASSERT(ValidProperties());
	iProperties->RequestComplete(aReason);
	}

/**
Must be called at the end of a slice of decoding.

If successful specify KErrNone that results in a repeat call to DoConvert().

@param  aReason
        The error code giving the reason for completion, or KErrNone
        if no error occurred.

@see    CImageDecoderPlugin::RequestComplete(TInt aReason)
*/
EXPORT_C void CImageDecoderPlugin::SelfComplete(TInt aReason)
	{
	ASSERT(ValidProperties());
	iProperties->SelfComplete(aReason);
	}

/**
May be called at the start of a slice of decoding if the decoding
is expected to complete asynchronously. This sets the AO in CImageDecoderPriv
to active, but does not complete the request.

When decoding of the slice is complete, there must be a call to SelfComplete().

@see    CImageDecoderPlugin::SelfComplete(TInt aReason)
*/
EXPORT_C void CImageDecoderPlugin::SetSelfPending(void)
	{
	ASSERT(ValidProperties());
	iProperties->SetSelfPending();
	}

_LIT(KResourcePath,"\\Resource\\ICL\\");

_LIT(KExtraResourceExtension,"_extra.rsc");

/**
Locates the extra resource file for the decoder aUid_extra.rsc, opens
the resource file and pushes it on the cleanup stack.

If the resource file is not found the method leaves with KErrNotFound.
If more than one resource file is found, only the first one is opened.

@param	aFs
      	A reference to the file server.
@param	aUid
      	The decoder's UID.
@param	aResourceFile
      	A reference to the opened resource file.
*/
EXPORT_C void CImageDecoderPlugin::OpenExtraResourceFileLC(RFs& aFs, const TUid aUid, RResourceFile& aResourceFile) const
	{
	// Build the extra resource file name from the uid
	TFileName fileName;

	TUid implementationUid = iProperties->ImplementationUid();
	CImplementationInformationType* implementationInformation = CImplementationInformationType::NewLC();
	ImageEnDecoderUtils::DoGetImplementationInformationL(KImageDecoderInterfaceUid, *implementationInformation, implementationUid);

	fileName.Copy(implementationInformation->Drive().Name());
	CleanupStack::PopAndDestroy(implementationInformation);
	

	fileName.Append(KResourcePath);
	fileName.AppendNum(aUid.iUid,EHex);
	fileName.Append(KExtraResourceExtension);

	// Get the locale specific file
	BaflUtils::NearestLanguageFile(aFs,fileName);

	aResourceFile.OpenL(aFs,fileName);

	CleanupClosePushL(aResourceFile);
	}

/**
Reads a block of data into an internal buffer.

A block of data of size aLength is read from the position specified by aPosition
to an internal data buffer. After a successful read, aReadBuffer is set to point to
the internal buffer.

If an attempt is made to read past the end of data, all available data is read and the
descriptors length will indicate the actual number of bytes read.

@param  aPosition
        The start position from where data will be read.
@param  aReadBuffer
        Upon completion of a successful call, points to the internal buffer containing
        the data read from the source.
@param  aLength
        The size in bytes of the block of data to be read.
*/
EXPORT_C void CImageDecoderPlugin::ReadDataL(TInt aPosition, TPtrC8& aReadBuffer, TInt aLength)
	{
	ASSERT(ValidProperties());
	iProperties->ReadDataL(aPosition, aReadBuffer, aLength);
	}

/**
Returns image information such as colour depth, scaling support etc.

@return Image information.
*/
EXPORT_C const TFrameInfo& CImageDecoderPlugin::ImageInfo() const
	{
	ASSERT(ValidProperties());
	return iProperties->ImageInfo();
	}

/**
Sets the image information.

@param  aImageInfo
        The image information.
*/
EXPORT_C void CImageDecoderPlugin::SetImageInfo(const TFrameInfo& aImageInfo)
	{
	ASSERT(ValidProperties());
	iProperties->SetImageInfo(aImageInfo);
	}

/**
Returns the number of comments attatched to the image.

Some image formats allow comments to be attached to the entire image, others allow
comments to be attached to individual frames within the image. Use this function to
retrieve the number of comments in the image. Use NumberOfFrameComments() for the
frame equivalent.

@return The number of comments attatched to the image.
*/
EXPORT_C TInt CImageDecoderPlugin::NumberOfImageComments() const
	{
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	return 0;
	}

/**
Returns a particular comment attatched to the image.
Ownership of the returned buffer is transferred to the caller.

@param  aCommentNumber
        The comment number.

@return A buffer containing the specified comment.
*/
EXPORT_C HBufC* CImageDecoderPlugin::ImageCommentL(TInt /* aCommentNumber */) const
	{
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	Panic(ECommentsNotSupported);
	return NULL;
	}

/**
Returns the number of comments attatched to a given frame of the image.

Use NumberOfImageComments() for the image equivalent.

@param  aFrameNumber
        The frame number.

@return The number of comments attatched to a given frame of the image.
*/
EXPORT_C TInt CImageDecoderPlugin::NumberOfFrameComments(TInt aFrameNumber) const
	{
	ASSERT(ValidProperties());
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < iProperties->FrameCount()), Panic(EFrameNumberOutOfRange));
	return 0;
	}

/**
Returns a particular comment attatched to a given frame of the image.

Ownership of the returned buffer is transferred to the caller.

@param  aFrameNumber
        The index of the frame containing the comments.
@param  aCommentNumber
        The index of the comment to retrieve from the specified frame.

@return A buffer containing the specified comment.
*/
EXPORT_C HBufC* CImageDecoderPlugin::FrameCommentL(TInt aFrameNumber, TInt /*aCommentNumber*/) const
	{
	ASSERT(ValidProperties());
	__ASSERT_ALWAYS(IsImageHeaderProcessingComplete(), Panic(EHeaderProcessingNotComplete));
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < iProperties->FrameCount()), Panic(EFrameNumberOutOfRange));
	Panic(ECommentsNotSupported);
	return NULL;
	}

/**
Invokes the ReadFrameHeadersL method of the supplied plugin which
should process the frame headers contained within the image.
*/
EXPORT_C void CImageDecoderPlugin::ReadFrameHeadersL()
	{
	ASSERT(ValidProperties());
	iProperties->ReadFrameHeadersL();
	}

/**
Returns the block size used in the specified frame's header.
Always returns 4096 regardless of the frame number specified.
Should be overriden by codecs that use larger blocks.

@param  aFrameNumber
        The frame to which the header block size information applies.

@return The returned frame header block size (always 4096).
*/
EXPORT_C TInt CImageDecoderPlugin::FrameHeaderBlockSize(TInt /*aFrameNumber*/) const
	{
	return 4096;
	}

/**
Returns the block size used in the specified frame.
Always returns 4096 regardless of the frame number specified.
Should be overriden by codecs that use larger blocks.

@param  aFrameNumber
        The frame to which the block size information applies.

@return The returned frame block size (always 4096).
*/
EXPORT_C TInt CImageDecoderPlugin::FrameBlockSize(TInt /*aFrameNumber*/) const
	{
	return 4096;
	}

/**
Returns the destination bitmap.

@return A reference to the destination bitmap.
*/
EXPORT_C const CFbsBitmap& CImageDecoderPlugin::Destination() const
	{
	ASSERT(ValidProperties());
	ASSERT(ValidDestination());
	return iProperties->Destination();
	}

/**
Returns the validity of the destination bitmap.

@return A boolean indicating if the destination bitmap is valid. ETrue if the destination bitmap 
        is valid, EFalse otherwise.
*/
EXPORT_C TBool CImageDecoderPlugin::ValidDestination() const
	{
	return iProperties->ValidDestination();
	}

/**
Returns the destination bitmap mask.

@return A reference to the destination bitmap mask.
*/
EXPORT_C const CFbsBitmap& CImageDecoderPlugin::DestinationMask() const
	{
	ASSERT(ValidProperties());
	ASSERT(ValidDestinationMask());
	return iProperties->DestinationMask();
	}

/**
Indicates if the destination bitmap mask is valid.

@return A boolean indicating if the destination bitmap is valid. ETrue if the destination bitmap 
        mask is valid, otherwise EFalse.
*/
EXPORT_C TBool CImageDecoderPlugin::ValidDestinationMask() const
	{
	return iProperties->ValidDestinationMask();
	}


/**
Paints the entire bitmap aBitmap with the color supplied as aColor.

@param     aBitmap
           A reference to a fully constructed bitmap.
@param     aColor
           The color to use for painting.
*/
EXPORT_C void CImageReadCodec::ClearBitmapL(CFbsBitmap& aBitmap, TRgb aColor)
	{
	if (aBitmap.Handle())
		{
		if(aBitmap.ExtendedBitmapType()!=KNullUid)
		    {
		    User::Leave(KErrNotSupported);
		    }
		CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(&aBitmap);
		CleanupStack::PushL(device);
		CFbsBitGc* gc = NULL;
		User::LeaveIfError(device->CreateContext(gc));
		
		// with alpha channel addition, the best way to paint an entire 
		// bitmap is to set the draw mode to WriteAlpha
		TDisplayMode mode = aBitmap.DisplayMode();
		if( mode == EColor16MA || mode == EColor16MAP )
			{
			gc->SetDrawMode(CGraphicsContext::EDrawModeWriteAlpha);	
			}

		gc->SetBrushColor(aColor);
		gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		gc->Clear();
		delete gc;
		CleanupStack::PopAndDestroy(); // device
		}
	}

/**
Checks that the constructed decoder is valid.

This function is internal and not intended for use.

@return A boolean indicating if the decoders construction is valid. ETrue if valid, EFalse otherwise.
*/
TBool CImageDecoderPlugin::ValidProperties() const
	{
	return iProperties != NULL;
	}

/**
Returns the status of header processing. If the processing is incomplete or not
terminated correctly EFalse will be returned

@return The header processing status.
*/
EXPORT_C TBool CImageDecoderPlugin::IsImageHeaderProcessingComplete() const
	{
	return iProperties->IsImageHeaderProcessingComplete();
	}

/**
Returns the frame info for a specified frame of the image.

This function can be called immediately after the call to create the decoder,
thus enabling the caller to know about each frame in advance of decoding it.

@param  aFrameNumber
        The frame number for which information is requested (Optional, defaults to 0).

@return The information for the specified frame.
*/
EXPORT_C const TFrameInfo& CImageDecoderPlugin::FrameInfo(TInt aFrameNumber) const
	{
	// Return the frame info for a particular frame
	ASSERT(ValidProperties());
	return iProperties->FrameInfo(aFrameNumber);
	}

/**
Returns the frame data for a specified frame of the image.

@param  aFrameNumber
        The frame number from which to retreive the frame data (Optional, defaults to 0).

@return The data for the specified frame.
 */
EXPORT_C const CFrameImageData& CImageDecoderPlugin::FrameData(TInt aFrameNumber) const
	{
	// Return the frame image data for a particular frame.
	ASSERT(ValidProperties());
	return iProperties->FrameData(aFrameNumber);
	}

/**
Returns the length of the source data in bytes.

@return The length of the source data.
*/
EXPORT_C TInt CImageDecoderPlugin::SourceLength() const
	{
	ASSERT(ValidProperties());
	return iProperties->SourceLength();
	}

/**
This function must be called on completion of encoding the image data.

@param  aReason
        The error code giving the reason for completion, or KErrNone
        if no error occurred.

@see    CImageEncoderPlugin::SelfComplete(TInt aReason)
*/
EXPORT_C void CImageEncoderPlugin::RequestComplete(TInt aReason)
	{
	ASSERT(ValidProperties());
	iProperties->RequestComplete(aReason);
	}

/**
Must be called at the end of a slice of encoding. If called with
KErrNone will cause a repeat call to DoConvert().

@param aReason
       The error code giving the reason for completion, or KErrNone
       if no error occurred.

@see   CImageEncoderPlugin::RequestComplete(TInt aReason)
*/
EXPORT_C void CImageEncoderPlugin::SelfComplete(TInt aReason)
	{
	ASSERT(ValidProperties());
	iProperties->SelfComplete(aReason);
	}

/**
May be called at the start of a slice of encoding if the encoding
is expected to complete asynchronously. This sets the AO in CImageEncoderPriv
to active, but does not complete the request.

When the encoding of the slice is complete, there must be a call to SelfComplete()

@see   CImageEncoderPlugin::SelfComplete(TInt aReason)
*/
EXPORT_C void CImageEncoderPlugin::SetSelfPending(void)
	{
	ASSERT(ValidProperties());
	iProperties->SetSelfPending();
	}

/**
Called as a result of an associated CImageDecoder::CustomSyncL() function being called.
Plugins may override this to provide extended commands in CImageDecoder. Default version
leaves with KErrNotSupported.

@param  aParam
        Interpretation dependent on plugin.
*/
EXPORT_C void CImageDecoderPlugin::HandleCustomSyncL(TInt aParam)
	{
	ASSERT(ValidProperties());
	iProperties->BodyHandleCustomSyncL(aParam);
	}

/**
Called as a result of the associated CImageDecoder::CustomAsync() function being called.

If this function finishes normally, then a convert cycle is started - so that DoConvert()
will be subsequently started in the background - otherwise, if this function leaves then
the error result is immediately signalled back to the caller of CustomAsync().

Plugins may override this to provide extended commands in CImageDecoder.Users of
CImageEncoder can then use the extended encoder functions by calling CustomAsync, rather
than CImageEncoder::Convert().

By default this function leaves with KErrNotSupported unless overriden.

@param  aParam
        Interpretation dependent on plugin.
*/

EXPORT_C void CImageDecoderPlugin::InitCustomAsyncL(TInt aParam)
	{
	ASSERT(ValidProperties());
	iProperties->BodyInitCustomAsyncL(aParam);
	}

/**
Plugin defined actions resulting from a call by RequestComplete().

This function is called when a RequestComplete() is issued indicating that an
asynchronous command has finished. Plugins can extend this function to,
clear any custom command flags.
*/
EXPORT_C void CImageDecoderPlugin::NotifyComplete()
	{
	ASSERT(ValidProperties());
	iProperties->BodyNotifyComplete();
	}

/**
Indicates if this decoder is running in a separate thread.

@return ETrue if running in separate thread, otherwise EFalse
*/

EXPORT_C TBool CImageDecoderPlugin::AmInThread() const
	{
	return iProperties->AmInThread();
	}

/**
Passes a pointer to a descriptor containing the thumbnail from the 
plugin to the framework. The framework then owns this descriptor.

@param aThumbnailData
	   A pointer to a HBufC8 containing the thumbnail data
*/

EXPORT_C void CImageDecoderPlugin::SetThumbnailData(HBufC8* aThumbnailData)
	{
	ASSERT(ValidProperties());	
	iProperties->SetThumbnailData(aThumbnailData);
	}


/**
Indicates if the decoder should abort early ie. following a call to
Cancel().

Note: This function always returns false unless the decoder is running
in its own thread.

@return ETrue if it should abort early, otherwise EFalse
*/

EXPORT_C TBool CImageDecoderPlugin::ShouldAbort() const
	{
	return iProperties->ShouldAbort();
	}

/**
@internalComponent

Enables generation of an image mask (to be implemented by the plugin
if this feature is supported).
*/
EXPORT_C void CImageDecoderPlugin::EnableMaskGeneration()
	{
	}

/**
@internalComponent

Notify the plugin that the client has changed the requested source 
image type (e.g. Main to Thumb or vice-versa)
*/
EXPORT_C void CImageDecoderPlugin::NotifyImageTypeChangeL(TInt)
	{
	}

/**
@publishedAll
@released

Returns the decoding options specified by the client when it created the CImageDecoder object.

@return The decoding options.
*/
EXPORT_C CImageDecoder::TOptions CImageDecoderPlugin::DecoderOptions() const
	{
	return iProperties->DecoderOptions();
	}

/**

*/
EXPORT_C void CImageDecoderPlugin::GetExtensionL(TUid /*aExtUid*/, MImageConvExtension*& /*aExtPtr*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
Gets the size of the destination CFbsBitmap needed prior to a call to Convert().
Essential when using the ICL Framework Extensions such as SetClippingRectL() or
codec extensions obtained through the OperationL(), ScalerL() and BlockStreamerL()
calls.

@param  aSize	A reference to a TSize that specifies the size
				of the region that is to be decoded by a call to
				Convert().
@param 	aFrameNumber The number of the frame that is to be decoded.

@see	CImageDecoder::OperationL()
@see	CImageDecoder::ScalerL()
@see	CImageDecoder::BlockStreamerL()
@see	CImageDecoder::SetClippingRectL()
@see	CImageDecoder::Convert()
*/
EXPORT_C TInt CImageDecoderPlugin::GetDestinationSize(TSize& /*aSize*/, TInt /*aFrameNumber*/)
	{
	return KErrNotSupported;
	}

/**
Sets the area of interest of the image to be decoded.  This function can leave with
any of the system-wide error codes.

@param aClipRect	A pointer to a TRect that specifies the
					location and size of the region to be decoded.  This
					rectangle must have positive width and height values as
					per TRect::IsNormalized() and TRect::Normalize().
					Passing in a NULL value will clear the clipping rectangle.

@leave KErrNotSupported		This function is not supported.
@leave KErrArgument			Returned if the clipping rectangle:
							a) is empty (i.e. IsEmpty() returns ETrue)
							b) is not normalised (i.e. IsNormalized() returns EFalse)
							c) has coordinates that are not located within, or on,
							the coodinates of at least one frame of the original image.
							d) has a width or a height of 0

@see	TRect::IsEmpty()
@see	TRect::IsNormalized()
@see	TRect::Normalize()
*/
EXPORT_C void CImageDecoderPlugin::SetClippingRectL(const TRect* /*aClipRect*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
@internalComponent

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CImageDecoderPlugin::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/* IMAGE ENCODER */

/**
Constructor for this class.
*/
EXPORT_C CImageEncoderPlugin::CImageEncoderPlugin()
	{
	}

/**
Destructor for this class.
*/
EXPORT_C CImageEncoderPlugin::~CImageEncoderPlugin()
	{
	}

/**
Called when the plugin is destroyed or an encode is cancelled to perform
cleanup functions.

This function may be overriden in derived classes in which case the derived
class should ensure it calls this version after performing any plugin
specific cleanup.
*/
EXPORT_C void CImageEncoderPlugin::Cleanup()
	{
	if(ValidProperties())
		iProperties->Cleanup();
	}

/**
Initialises data structures prior to encoding a frame.

This may be overriden in derived classes in which case the derived class
should ensure it calls this version after performing any plugin
initialisation.
*/
EXPORT_C void CImageEncoderPlugin::InitConvertL()
	{
	ASSERT(ValidProperties());
	iProperties->InitConvertL();
	}

/**
Forces initialization of data structures prior to decoding a frame.
*/
EXPORT_C void CImageEncoderPlugin::RequestInitL()
	{
	ASSERT(ValidProperties());
	iProperties->RequestInitL();
	}
	
/**
Performs the encoding process.

This call may be overriden in derived classes. If this is not
the case and a custom decode is not performed, the derived class should
ensure that this base class's version is called.
*/
EXPORT_C void CImageEncoderPlugin::DoConvert()
	{
	ASSERT(ValidProperties());
	iProperties->DoConvert();
	}

/**
@internalComponent

Tells the encoder to generate the thumbnail

THis is used only for Exif format. If aDoGenerateThumbnail=EFalse, the 
thumbnail is not generated. The default value is ETrue

*/
EXPORT_C void CImageEncoderPlugin::SetThumbnail(TBool /*aDoGenerateThumbnail*/)
	{
	}

/**
Deals with the result from ProcessFrameL().

This processes the results of the standard ProcessFrameL() call, feeding in the resultant
error code from its TRAP and the status result. It will call RequestComplete() or
SelfComplete() as appropriate. Note that if no data was created by ProcessFrameL(),
HandleProcessFrameResult() assumes that the encoding process is complete.
If this is not appropriate, an overloaded DoConvert() should be used to handle it.

@param  aErrCode
        The error result of TRAP around ProcessFrameL().
@param  aCodecState
        The result of ProcessFrameL() itself.
*/

EXPORT_C void CImageEncoderPlugin::HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)
	{
	ASSERT(ValidProperties());
	iProperties->HandleProcessFrameResult(aErrCode, aCodecState);
	}

/**
Returns the value to be fed to CImageWriteCodec::ProcessFrameL(),
and will be used by codecs that fully override DoConvert().
*/
EXPORT_C TBufPtr8& CImageEncoderPlugin::DestinationData()
	{
	ASSERT(ValidProperties());
	return iProperties->DestinationData();
	}

/**
Writes a descriptor to the internal data buffer of the encoded image without 
incrementing the position in the buffer, and therefore a call to Position()
will return the same value before or after a call to this function.

@param	aPosition
		The start position in the internal data buffer of the encoded image from
		which point the data in aDes is written.
@param	aDes
		The descriptor containing the data to be written to the internal data buffer
		of the encoded image.
@see	Position()
*/
EXPORT_C void CImageEncoderPlugin::WriteDataL(TInt aPosition,const TDesC8& aDes)
	{
	ASSERT(ValidProperties());
	iProperties->WriteDataL(aPosition, aDes);
	}
	
/**
Writes a descriptor to the internal data buffer of the encoded image.  In addition,
the position in the buffer that is written to (obtained with Position()) will be
incremented returning aPosition + aDes.Length().

@param	aPosition
		The start position in the internal data buffer of the encoded image from
		which point the data in aDes is written.
@param	aDes
		The descriptor containing the data to be written to the internal data buffer
		of the encoded image.
@see	Position()
*/
EXPORT_C void CImageEncoderPlugin::WriteDataPositionIncL(TInt aPosition,const TDesC8& aDes)
	{
	ASSERT(ValidProperties());
	iProperties->WriteDataPositionIncL(aPosition, aDes);
	}	

/**
Returns the plugin's write codec.

@return A pointer to the plugin's write codec.
*/
EXPORT_C CImageWriteCodec* CImageEncoderPlugin::ImageWriteCodec() const
	{
	ASSERT(ValidProperties());
	return iProperties->ImageWriteCodec();
	}

/**
Sets the plugin's write codec.

Ownership of the codec is transferred to the plugin.

@param  aImageWriteCodec
        A pointer to the codec.
*/
EXPORT_C void CImageEncoderPlugin::SetImageWriteCodec(CImageWriteCodec* aImageWriteCodec) const
	{
	ASSERT(aImageWriteCodec != NULL);
	ASSERT(ValidProperties());
	iProperties->SetImageWriteCodec(aImageWriteCodec);
	}

/**
Returns the bitmap which is being encoded.

@return A reference to the source bitmap.
*/
EXPORT_C const CFbsBitmap& CImageEncoderPlugin::Source() const
	{
	ASSERT(ValidProperties());
	return iProperties->Source();
	}

/**
Checks the validity of the bitmap which is being encoded. Returns ETrue if the 
bitmap is valid, otherwise EFalse.

@return A boolean describing the validity of the bitamp.
*/
EXPORT_C TBool CImageEncoderPlugin::ValidSource() const
	{
	ASSERT(ValidProperties());
	return iProperties->ValidSource();
	}

/**
Returns the starting position of the internal data buffer of the encoded image
that is being written to.

@return The starting position.
*/
EXPORT_C TInt& CImageEncoderPlugin::StartPosition() const
	{
	ASSERT(ValidProperties());
	return iProperties->StartPosition();
	}

/**
Returns the current position within the internal data buffer that is being written to.

@return The current position.
@see	WriteDataPositionIncL(TInt aPosition,const TDesC8& aDes)
@see	WriteDataL(TInt aPosition,const TDesC8& aDes)
*/
EXPORT_C TInt& CImageEncoderPlugin::Position() const
	{
	ASSERT(ValidProperties());
	return iProperties->Position();
	}

/**
Returns the overall size of the image frame in pixels.

@return The size of the image frame.
*/
EXPORT_C const TSize& CImageEncoderPlugin::FrameInfoOverallSizeInPixels() const
	{
	ASSERT(ValidProperties());
	return iProperties->FrameInfoOverallSizeInPixels();
	}

/**
Checks that the constructed encoder is valid.

@return ETrue if valid, EFalse otherwise.
*/
TBool CImageEncoderPlugin::ValidProperties() const
	{
	return iProperties != NULL;
	}

/**
Returns the current size of the encoded image in bytes.

@return The current size of the encoded image in bytes.
*/
EXPORT_C TInt CImageEncoderPlugin::CurrentImageSizeL() const
	{
	ASSERT(ValidProperties());
	return iProperties->CurrentImageSizeL();
	}

/**
Called as a result of the associated CImageEncoder::CustomSyncL() function being called.
Plugins may override this to provide extended commands in CImageEncoder. Default version
leaves with KErrNotSupported.

@param  aParam
        Interpretation dependent on plugin.
*/

EXPORT_C void CImageEncoderPlugin::HandleCustomSyncL(TInt aParam)
	{
	ASSERT(ValidProperties());
	iProperties->BodyHandleCustomSyncL(aParam);
	}

/**
Called as a result of the associated CImageEncoder::CustomAsync() function being called.
If this function finishes normally, then a convert cycle is started - so that DoConvert()
will be subsequently started in the background - otherwise, if this function leaves then
the error result is immediately signalled back to the caller of CustomAsync().

The default version leaves with KErrNotSupported unless overridden to change this behaviour. Plugins may
override this to provide extended commands in CImageEncoder.

@param  aParam
        Interpretation dependent on plugin.
*/

EXPORT_C void CImageEncoderPlugin::InitCustomAsyncL(TInt aParam)
	{
	ASSERT(ValidProperties());
	iProperties->BodyInitCustomAsyncL(aParam);
	}

/**
Plugin defined actions resulting from a call by RequestComplete().

This function is called when a RequestComplete() is issued indicating that
an asynchronous command has finished. Plugins can extend this function to
clear any custom command flags.
*/
EXPORT_C void CImageEncoderPlugin::NotifyComplete()
	{
	ASSERT(ValidProperties());
	iProperties->BodyNotifyComplete();
	}

/**
Indicates if this encoder is running in a separate thread.

@return	A boolean indicating if the encoder is running in a seperate thread. ETrue if running in 
        separate thread, otherwise EFalse.
*/
EXPORT_C TBool CImageEncoderPlugin::AmInThread() const
	{
	ASSERT(ValidProperties());
	return iProperties->AmInThread();
	}

/**
Indicates if an encode should abort early (ie. following a Cancel).

Note:
This function always returns EFalse unless the encoder is running in its own thread.

@return A boolean indicating if the encode should abort early. ETrue if should abort early, 
        otherwise EFalse.
*/

EXPORT_C TBool CImageEncoderPlugin::ShouldAbort() const
	{
	ASSERT(ValidProperties());
	return iProperties->ShouldAbort();
	}

/**
Notifies the framework that the main frame encoding is complete, so it can tidy up. This
results in a call to UpdateHeaderL() and then either the descriptor is copied across
or the file is closed.

Note:
This function is only used if a decoder replaces the DoConvert() call - the default
version does this as part of its processing.
*/
EXPORT_C void CImageEncoderPlugin::FinishConvertL()
	{
	ASSERT(ValidProperties());
	iProperties->FinishConvertL();
	}

/**
@internalComponent

Sets the thumbnail in the encoded file

@panic  EReservedCall
*/
EXPORT_C void CImageEncoderPlugin::WriteThumbnailL()
	{
	
	}

/**
@internalComponent

Intended for future proofing - will panic if called

@param the status which will be notified once the scale operation on the thumbnail will be performed

@panic  EReservedCall
*/
EXPORT_C void CImageEncoderPlugin::WriteExifDataL(TRequestStatus*& /*aScaleCompletionStatus*/)
	{
	SelfComplete(KErrNone);
	}

/**
@publishedPartner
@released

Enables getting set of options which has been passed by client during encoder object creation.
Plugins should ignore unknown options.

@return Set of options which has been passed by client during encoder 
		object creation
*/
EXPORT_C CImageEncoder::TOptions CImageEncoderPlugin::EncoderOptions() const
	{
	return iProperties->EncoderOptions();
	}

/**
@internalComponent

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CImageEncoderPlugin::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
@internalComponent

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CImageEncoderPlugin::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
@internalComponent

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CImageEncoderPlugin::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

EXPORT_C void CImageEncoderPlugin::GetExtensionL(TUid /*aExtUid*/, MImageConvExtension*& /*aExtPtr*/)
	{
	User::Leave(KErrNotSupported);
	}

