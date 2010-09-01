// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This feature is documented in ICL_Design_EXIF.doc
// 
//

/**
 @file
 @publishedAll
 @released
*/

#include <iclexifimageframe.h>
#include "ImageClientMain.h"

_LIT(KImageFramePluginPanicCategory, "JPEGImageFramePlugin");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KImageFramePluginPanicCategory, aError);
	}


//
// CJpegImageFrameDecoder //
//

/**
A factory function that constructs a JPEG CImageFrame decoder as an extension of the ICL EXIF decoder.

A leave occurs if there is insufficient memory available.

@return A pointer to the new JPEG CImageFrame decoder.
*/
EXPORT_C CJPEGImageFrameDecoder* CJPEGImageFrameDecoder::NewL()
	{
	CJPEGImageFrameDecoder* self;
	self = new (ELeave) CJPEGImageFrameDecoder;
	CleanupStack::PushL(self);
	self->CJPEGExifDecoder::ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	

/**
Constructor for the CJPEGImageFrameDecoder class.
*/	
EXPORT_C CJPEGImageFrameDecoder::CJPEGImageFrameDecoder()
	{
	}

/**
Destructor for the CJPEGImageFrameDecoder class.
*/
EXPORT_C CJPEGImageFrameDecoder::~CJPEGImageFrameDecoder()
	{
	}
	
/**
Basic decode convert call for destinations of type CImageFrame.

@pre
The destination CImageFrame, must be created before the call to ConvertFrame() is made. 

aDestFrame must be large enough to contain the decoded frame.
CJPEGImageFrameDecoder::RecommendedBufferSize() can be used to obtain the required buffer size in the CImageFrame.

This convert call is a standard asynchronous operation. It takes a pointer to the TRequestStatus object 
of the waiting active object in the main application which will be signalled when the operation is completed.

If the operation completes with KErrUnderflow, then there is insufficient 
information in the source file or descriptor. In this situation, ContinueConvertFrame() should be 
called repeatedly until the source file or descriptor has accumulated enough information 
for ContinueConvertFrame() to complete with KErrNone.

@param  aRequestStatus
        Pointer to the TRequestStatus object that is signalled on completion.
        Contains an error code.
        KErrNone if frame was decoded successfully,
        otherwise another of the system-wide error codes.
@param  aDestFrame
        A CImageFrame that will contain the decoded frame.
*/
EXPORT_C void CJPEGImageFrameDecoder::ConvertFrame(TRequestStatus* aRequestStatus, CImageFrame& aDestFrame)
	{
	// Call ConvertFrame with Frame Number equal to 0
	ConvertFrame(aRequestStatus, aDestFrame, 0);
	}

/**
Basic decode convert call for destinations of type CImageFrame when frame number is specified.

@pre
The destination CImageFrame, must be created before the call to ConvertFrame() is made.

aDestFrame must be large enough to contain the decoded frame.
CJPEGImageFrameDecoder::RecommendedBufferSize() can be used to obtain the required buffer size in the CImageFrame.

This convert call is a standard asynchronous operation. It takes a pointer to the TRequestStatus object 
of the waiting active object in the main application which will be signalled when the operation is completed.

If the operation completes with KErrUnderflow, then there is insufficient 
information in the source file or descriptor. In this situation, ContinueConvertFrame() should be 
called repeatedly until the source file or descriptor has accumulated enough information 
for ContinueConvertFrame() to complete with KErrNone.

@param  aRequestStatus
        Pointer to the TRequestStatus object that is signalled on completion.
        Contains an error code.
        KErrNone if frame was decoded successfully,
        otherwise another of the system-wide error codes.
@param  aDestFrame
        A CImageFrame that will contain the decoded frame.
@param	aFrameNumber
        The number of the frame in a multi-frame image to decode.
*/
EXPORT_C void CJPEGImageFrameDecoder::ConvertFrame(TRequestStatus* aRequestStatus, CImageFrame& aDestFrame, TInt aFrameNumber)
	{
	CJPEGImageFrameDecoderPlugin* plugin;
	plugin = static_cast<CJPEGImageFrameDecoderPlugin*>(Plugin());
	plugin->SetConvertParameters (aDestFrame, aFrameNumber);

	CustomAsync(aRequestStatus, KOptionConvertFrameUidValue);	
	}

/**
Continue decoding for destinations of type CImageFrame after new image data was added to
the source file or descriptor and a previous call to ConvertFrame() or
ContinueConvertFrame() returned KErrUnderflow.

@param  aRequestStatus
        Request status. This returns an error code on completion.
        KErrNone if frame was successfully decoded, 
        KErrUnderflow if the frame was partially decoded, 
        KErrNotReady if the destination is not valid, 
        otherwise another of the system-wide error codes.
*/
EXPORT_C void CJPEGImageFrameDecoder::ContinueConvertFrame(TRequestStatus* aRequestStatus)
	{
	CustomAsync(aRequestStatus, KOptionContinueConvertFrameUidValue);	
	}

/**
Returns the size (in bytes) of the buffer required to decode the image supplied to the decoder. 
The size returned will be the required size when using the plugin's default format code.

The client should supply the buffer size in the aMaxBufferSize parameter of CImageFrame::NewL() when creating the 
destination CImageFrame before calling CJPEGImageFrameDecoder::ConvertFrame().

@param  aBufferSize
		On return contains recommended buffer size to be used, in bytes.
		Size is 0 if the codec cannot decode the image.

@return A boolean indicating whether or not the codec supports decoding of that image.
*/
EXPORT_C TBool CJPEGImageFrameDecoder::RecommendedBufferSize(TInt& aBufferSize)
	{
	return RecommendedBufferSize(KNullUid, aBufferSize);
	}

/**
Returns the size (in bytes) of the buffer required to decode the image supplied to the decoder using a particular format.

The client should supply the buffer size in the aMaxBufferSize parameter of CImageFrame::NewL() when creating the 
destination CImageFrame before calling CJPEGImageFrameDecoder::ConvertFrame().

@param  aFormatCode
		The UID of a format code that the client intends to use to decode the image. 
@param  aBufferSize
		On return contains recommended buffer size to be used, in bytes.
		Size is 0 if the codec cannot decode the image to the specified format.

@return A boolean indicating whether or not the codec can decode the image to the requested format.	
*/
EXPORT_C TBool CJPEGImageFrameDecoder::RecommendedBufferSize(TUid aFormatCode, TInt& aBufferSize)
	{
	CJPEGImageFrameDecoderPlugin* plugin;
	plugin = static_cast<CJPEGImageFrameDecoderPlugin*>(Plugin());
	plugin->SetFormatCode(aFormatCode);

	TRAPD(error, CustomSyncL(KOptionRecommendBufferSizeUidValue));			
	if (error!=KErrNone)
		{
		aBufferSize = 0;
		return EFalse;
		}
	else
		{
		aBufferSize = plugin->RecommendedBufferSize();	
		return ETrue;
		}
	}

	
//
// CJPEGImageFrameEncoder //
//

/**
A factory function that constructs a JPEG CImageFrame encoder as an extension of the ICL EXIF encoder.

A leave occurs if there is insufficient memory available.

@return A pointer to the new JPEG CImageFrame encoder.
*/
EXPORT_C CJPEGImageFrameEncoder* CJPEGImageFrameEncoder::NewL()
	{
	CJPEGImageFrameEncoder* self;
	self = new (ELeave) CJPEGImageFrameEncoder;
	CleanupStack::PushL(self);
	self->CJPEGExifEncoder::ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Constructor for the CJPEGImageFrameEncoder class.
*/
EXPORT_C CJPEGImageFrameEncoder::CJPEGImageFrameEncoder()
	{
	}

/**
Destructor for the CJPEGImageFrameEncoder class.
*/
EXPORT_C CJPEGImageFrameEncoder::~CJPEGImageFrameEncoder()
	{
	}
	
/**
Basic encode convert call for sources of type CImageFrame.

This convert call is a standard asynchronous operation. It takes a pointer to the TRequestStatus object 
of the waiting active object in the main application which will be signalled when the operation is completed.

@param  aRequestStatus
        Pointer to the TRequestStatus object that is signalled on completion.
        Contains an error code.
        KErrNone if frame was decoded successfully,
        otherwise another of the system-wide error codes.
@param  aSrcFrame
        A CImageFrame wrapping the image data to encode.
*/
EXPORT_C void CJPEGImageFrameEncoder::ConvertFrame(TRequestStatus* aRequestStatus, const CImageFrame& aSrcFrame)
	{

	CJPEGImageFrameEncoderPlugin* plugin;
	plugin = static_cast<CJPEGImageFrameEncoderPlugin*>(Plugin());
	plugin->SetConvertParameters (aSrcFrame, NULL);

	CustomAsync(aRequestStatus, KOptionConvertFrameUidValue);	
 	}

/**
Basic encode convert call for sources of type CImageFrame.

This convert call is a standard asynchronous operation. It takes a pointer to the TRequestStatus object 
of the waiting active object in the main application which will be signalled when the operation is completed.

Allows the client to provide the frame image data in the same way as standard
CImageEncoder::Convert() calls.

The aFrameImageData parameter in this call provides additional jpeg image data that can be used
by the plugin to provide transcoding to a different format to the one used in the source CImageFrame.
How this information is used is plugin implementation specific.

@param  aRequestStatus
        Pointer to the TRequestStatus object that is signalled on completion.
        Contains an error code.
        KErrNone if frame was decoded successfully,
        otherwise another of the system-wide error codes.
@param  aSrcFrame
        A CImageFrame wrapping the image data to encode.
@param  aFrameImageData
        The frame image data.
*/
EXPORT_C void CJPEGImageFrameEncoder::ConvertFrame(TRequestStatus* aRequestStatus, const CImageFrame& aSrcFrame, const CFrameImageData* aFrameImageData)
	{

	CJPEGImageFrameEncoderPlugin* plugin;
	plugin = static_cast<CJPEGImageFrameEncoderPlugin*>(Plugin());
	plugin->SetConvertParameters (aSrcFrame, aFrameImageData);

	CustomAsync(aRequestStatus, KOptionConvertFrameUidValue);	
 	}


//	
// CJPEGImageFrameDecoderPlugin //
//	
	
/**
Constructor for the CJPEGImageFrameDecoderPlugin class.
*/
EXPORT_C CJPEGImageFrameDecoderPlugin::CJPEGImageFrameDecoderPlugin()
	{
	iFormatCode = KNullUid;
	}

/**
Destructor for the CJPEGImageFrameDecoderPlugin class.
*/
EXPORT_C CJPEGImageFrameDecoderPlugin::~CJPEGImageFrameDecoderPlugin()
	{
	}

/**
Returns the destination CImageFrame stored in this plugin extension.

@return A reference to the destination CImageFrame. 
*/
EXPORT_C CImageFrame& CJPEGImageFrameDecoderPlugin::DstImageFrame()
	{
	return *iDstImageFrame;
	}

/**
Returns the frame number (in a multi-frame image to be decoded) stored in this plugin extension.

@return A frame number. 
*/
EXPORT_C TInt CJPEGImageFrameDecoderPlugin::FrameNumber()	const
	{
	return iFrameNumber;
	}

/**
Returns a format code stored in this plugin extension. 
This format code can be used to calculate the recommended buffer size needed to decode the image
and it may not be the format code finally used for decoding.

@see imageframeformats.hrh

@return A format code UID. 
*/
EXPORT_C TUid CJPEGImageFrameDecoderPlugin::FormatCode() const
	{
	return iFormatCode;	
	}

/**
Stores in this plugin extension the recommended buffer size to use when decoding the image contained
in a file or descriptor.

@param  aBufferSize
        The recommended buffer size to use, in bytes.
*/
EXPORT_C void CJPEGImageFrameDecoderPlugin::SetRecommendedBufferSize(TInt aBufferSize)
	{
	iRecommendedBufferSize = aBufferSize;
	}

/**
@internalTechnology

Sets the destination CImageFrame and the Frame Number	in the CImageFrame decoder plugin extension
*/
void CJPEGImageFrameDecoderPlugin::SetConvertParameters(CImageFrame& aDestFrame, TInt aFrameNumber)
	{
	iDstImageFrame = &aDestFrame;
	iFrameNumber = aFrameNumber;
	}

/**
@internalTechnology

Sets the format code	in the CImageFrame decoder plugin extension
*/
void CJPEGImageFrameDecoderPlugin::SetFormatCode(TUid aFormatCode)
	{
	iFormatCode = aFormatCode;
	}

/**
@internalTechnology

Returns the recommended buffer size stored in the CImageFrame decoder plugin extension.
*/
TInt CJPEGImageFrameDecoderPlugin::RecommendedBufferSize()
	{
	return iRecommendedBufferSize;
	}
	
/**
@internalTechnology

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CJPEGImageFrameDecoderPlugin::Reserved1()
	{
	Panic(EReservedCall);
	}
	
/**
@internalTechnology

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CJPEGImageFrameDecoderPlugin::Reserved2()
	{
	Panic(EReservedCall);
	}
	
/**
@internalTechnology

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CJPEGImageFrameDecoderPlugin::Reserved3()
	{
	Panic(EReservedCall);
	}
	
	
//	
// CJPEGImageFrameEncoderPlugin //
//	

/**
Constructor for the CJPEGImageFrameEncoderPlugin class.
*/
EXPORT_C CJPEGImageFrameEncoderPlugin::CJPEGImageFrameEncoderPlugin()
	{
	}

/**
Destructor for the CJPEGImageFrameEncoderPlugin class.
*/
EXPORT_C CJPEGImageFrameEncoderPlugin::~CJPEGImageFrameEncoderPlugin()
	{
	}

/**
Returns the source CImageFrame to encode stored in this plugin extension.

@return A reference to the source CImageFrame. 
*/	
EXPORT_C const CImageFrame& CJPEGImageFrameEncoderPlugin::SrcImageFrame() const
	{
	return *iSrcImageFrame;
	}

/**
Returns the frame image data stored in this plugin extension.

@return A reference to the frame image data. 
*/
EXPORT_C const CFrameImageData& CJPEGImageFrameEncoderPlugin::FrameImageData() const	
	{
	return *iFrameImageData;
	}

/**
@internalTechnology

Sets the source CImageFrame and FrameImageData in the encoder plugin extension	
*/
void CJPEGImageFrameEncoderPlugin::SetConvertParameters(const CImageFrame& aSrcFrame, const CFrameImageData* aFrameImageData)
	{
	iSrcImageFrame = &aSrcFrame;
	iFrameImageData = aFrameImageData;
	}
	
/**
@internalTechnology

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CJPEGImageFrameEncoderPlugin::Reserved1()
	{
	Panic(EReservedCall);
	}
	
/**
@internalTechnology

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CJPEGImageFrameEncoderPlugin::Reserved2()
	{
	Panic(EReservedCall);
	}
	
/**
@internalTechnology

Intended for future proofing - will panic if called.

@panic  EReservedCall
*/
EXPORT_C void CJPEGImageFrameEncoderPlugin::Reserved3()
	{
	Panic(EReservedCall);
	}
