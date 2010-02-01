// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "imageprocessorimpl.h"

using namespace ImageProcessor;

/**
Constructs a new image processor object.

@param  aFileServerSession
        A file server session for the image processor to use. 
@param  aObserver
        The image processor observer to recieve callbacks.
@param  aImageProcessorPluginUid
        The UID of the image processor plugin to load. Can be set to KUidNull which will load the 
        highest version of the plugin.
        
@leave KErrNoMemory	
	   There is insufficient memory available.

@return A pointer to the new image processor object.
*/
EXPORT_C CImgProcessor* CImgProcessor::NewL(RFs& aFileServerSession, MImgProcessorObserver& aObserver, TUid aImageProcessorPluginUid)
	{
	CImgProcessor* self = new (ELeave) CImgProcessor;
	CleanupStack::PushL(self);
	self->ConstructL(aFileServerSession, aObserver, aImageProcessorPluginUid);
	CleanupStack::Pop(self);
	return self;
	}

CImgProcessor::CImgProcessor()
	{
   
	}

void CImgProcessor::ConstructL(RFs& aFileServerSession, MImgProcessorObserver& aObserver, TUid aImageProcessor)
	{
	iImplementation = CImageProcessorImpl::NewL(aFileServerSession,*this,aObserver,aImageProcessor);
	}

EXPORT_C CImgProcessor::~CImgProcessor()
	{
	delete iImplementation;
	}

/**
Returns a list of effect UIDs which can be applied by this image processor.

@param  aEffects
        Array to be filled by the list of the supported effects.

*/
EXPORT_C void CImgProcessor::SupportedEffectsL(RArray<TUid>& aEffects) const
	{
	iImplementation->SupportedEffectsL(aEffects);
	}
	
/**
Returns a list of compressed image formats which are supported (e.g. KImageTypeJPGUid etc).

@param  aFormats
        Array to be filled by the list of the supported input formats.

*/
EXPORT_C void CImgProcessor::SupportedInputFormatsL(RArray<TUid>& aFormats) const
	{
	iImplementation->SupportedInputFormatsL(aFormats);
	}
	
//(e.g. ?????) 
/**
Returns a list of the input sub-formats supported.

@param  aFormat
        Supported input format.

@param  aSubFormats
        Array to be filled by the list of the supported input sub-formats.

*/
EXPORT_C void CImgProcessor::SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const
	{
	iImplementation->SupportedInputSubFormatsL(aFormat, aSubFormats);
	}
	
/**
Returns a list of supported image frame formats (e.g. KUidFormat32BitRGB888Interleaved).

@param  aFormats
        Array to be filled in by the list of the supported input formats.

*/
EXPORT_C void CImgProcessor::SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats) const
	{
	iImplementation->SupportedInputImageFrameFormatsL(aFormats);
	}

/**
Returns a list of supported input display modes (e.g. EColor16M).

@param  aDisplayModes
        Array to be filled in by the list of the supported input display modes.

*/
EXPORT_C void CImgProcessor::SupportedInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const
	{
	iImplementation->SupportedInputDisplayModesL(aDisplayModes);
	}

/**
Returns a list of supported compressed output image formats (e.g. KImageTypeJPGUid etc.).

@param  aFormats
        Array to be filled in by the list of the supported output formats.

*/
EXPORT_C void CImgProcessor::SupportedOutputFormatsL(RArray<TUid>& aFormats) const
	{
	iImplementation->SupportedOutputFormatsL(aFormats);
	}
	
//(e.g. ?????)
/**
Returns a list of the output sub-formats supported.

@param  aFormat
        The supported format of ImageProcessor.
        
@param	aSubFormats
		Array to be filled in by the list of the supported sub formats of ImageProcessor.        
*/
EXPORT_C void CImgProcessor::SupportedOutputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const
	{
	iImplementation->SupportedOutputSubFormatsL(aFormat, aSubFormats);
	}
	
/**
Returns a list of image frame formats to which the output can be rendered (e.g. KUidFormat32BitRGB888Interleaved).

@param  aFormats
        Array to be filled in by the list of the supported output frame formats.

*/
EXPORT_C void CImgProcessor::SupportedOutputImageFrameFormatsL(RArray<TUid>& aFormats) const
	{
	iImplementation->SupportedOutputImageFrameFormatsL(aFormats);
	}

/**
Returns a list of supported output display modes (e.g. EColor16M).

@param  aDisplayModes
        Array to be filled in by the list of the supported output display modes.

*/
EXPORT_C void CImgProcessor::SupportedOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const
	{
	iImplementation->SupportedOutputDisplayModesL(aDisplayModes);
	}

/**
Returns the options supported by this image processor as a set of flags.

@return A TUint64 containing the supported options.

@see	CImgProcessor:TOptions
*/
EXPORT_C TUint64 CImgProcessor::SupportedOptions() const
	{
	return iImplementation->SupportedOptions();
	}
   
/**
Sets the desired options. Options can be combined using bitwise inclusive OR.

@param	aOptions
		The new options to set to this image processor.
		
@see	CImgProcessor:TOptions
*/
EXPORT_C void CImgProcessor::SetOptionsL(TUint64 aOptions)
	{
	iImplementation->SetOptionsL(aOptions);
	}

/**
Gets the set of operations supported.

@return A TUint containing the supported operations.

@see	CImgProcessor:TOperation

*/
EXPORT_C TUint CImgProcessor::SupportedOperations() const
	{
	return iImplementation->SupportedOperations();
	}

/**
Gets the set of the options set in the image processor.

@return A TUint containing the image processor options.

@see	CImgProcessor:TOperation

*/
EXPORT_C TUint64 CImgProcessor::Options() const
	{
	return iImplementation->Options();
	}
   
/**
Applies a geometrical operation to the image.
The image processor needs to be in an initialized state.

@param	aOperation
		The geometrical operation to be applied. 

*/
EXPORT_C void CImgProcessor::ApplyOperationL(CImgProcessor::TOperation aOperation)
	{
	iImplementation->ApplyOperationL(aOperation);
	}

/**
Returns the state of the image processor.

@return The image processor's state.

@see CImgProcessor::TState
*/
EXPORT_C CImgProcessor::TState CImgProcessor::State() const
	{
	return iImplementation->State();
	}
	
/**
Calculates the required pixel buffer size in bytes for the destination image given display mode and scan line length. 

@param  aSizeInPixels
        The size in pixels of the image to calculate buffer size for.
@param  aDisplayMode
        The display mode of the destination image to calculate buffer size for.
section for details.
@param  aScanLineLength
        Number of bytes per image row.

@return The calculated pixel buffer size in bytes.

@leave  KErrNotReady
		The image processor is not in EUninitialized state.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TInt CImgProcessor::CalculatePixelBufferSizeL(TSize aSizeInPixels, TDisplayMode aDisplayMode, TUint32 aScanLineLength) const
	{
	return iImplementation->CalculatePixelBufferSizeL(aSizeInPixels, aDisplayMode, aScanLineLength);
	}

/**
Calculates the required pixel buffer size in bytes for the destination image given destination uncompressed image format and scan line length. 

@param  aSizeInPixels
        The size in pixels of the raw image to calculate buffer size for.
@param  aFormat
        The format of the raw image to calculate buffer size for (one of the valid CImageFrame formats).
section for details.
@param  aScanLineLength
        Number of bytes per image row.

@return The calculated pixel buffer size.

@leave  KErrNotReady
		The image processor is not in EUninitialized state.

@leave	Other
		A range of system wide error codes.

*/
EXPORT_C TInt CImgProcessor::CalculatePixelBufferSizeL(TSize aSizeInPixels, const TUid& aFormat, TUint32 aScanLineLength) const
	{
	return iImplementation->CalculatePixelBufferSizeL(aSizeInPixels, aFormat, aScanLineLength);
	}

/**
Creates internal pixel buffer for the input and copies data from source bitmap to internal buffer

@param  aBitmap
        The bitmap to copy.

@leave  KErrNotReady
		The image processor is not initialized.
		
@leave  KErrArgument
		The input bitmap size is 0.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::CreateInputL(CFbsBitmap& aBitmap)
	{
	iImplementation->CreateInputL(aBitmap);
	}

/**
Creates internal pixel buffer the for the source image and copies data from the image frame to the internal buffer

@param  aPixelBuffer
        The image frame to copy. 

@leave  KErrNotReady
		The image processor is not initialized.
		
@leave  KErrArgument
		The input bitmap size is 0.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::CreateInputL(CImageFrame& aPixelBuffer)
	{
	iImplementation->CreateInputL(aPixelBuffer);
	}

/**
Creates internal pixel buffer for the source image using the given size and initializes source image with the given color. 

@param  aSize
        The size of the new source image.
@param  aColor
        The color of the new source image.

@leave  KErrNotReady
		The image processor is not initialized.
		
@leave  KErrArgument
		The input bitmap size is 0.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::CreateInputL(const TSize& aSize, const TRgb& aColor)
	{
	iImplementation->CreateInputL(aSize, aColor);
	}

/**
Sets or updates the source image to the specified file name.

@param  aFilename
        The filename of the new source image to use.
@param  aFormat
        The format of the new source image to use. If KNullUid is passed then an attempt is made to recognize its format.
@param  aSubFormat
        The sub-format of the new source image to use. If KNullUid is passed then an attempt is made to recognize its sub-format.
        
@leave  KErrNotReady
		The image processor is not initialized.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetInputL(aFilename, aFormat, aSubFormat);
	}
	
/**
The source image is set or updated to the given file.

@param  aFile
        The file handle of the new source image to use.
@param  aFormat
        The format of the new source image to use.
@param  aSubFormat
        The sub-format of the new source image to use.
        
@leave  KErrNotReady
		The image processor is not initialized.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetInputL(aFile, aFormat, aSubFormat);
	}
	
/**
The source image is set or updated. The source image may be DRM protected.

@param  aDrmFile
        The DRM file of the new source image to use.
@param  aFormat
        The format of the new source image to use.
@param  aSubFormat
        The sub-format of the new source image to use.

@leave  KErrNotReady
		The image processor is not initialized.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetInputL(aDrmFile, aFormat, aSubFormat);
	}
	
//aBuffer (Is this both compressed and uncompressed data?)
/**
The source image is set or updated to the content of the buffer provided.

@param  aBuffer
        The buffer containing the new source image to use. 
@param  aFormat
        The format of the new source image to use.
@param  aSubFormat
        The sub-format of the new source image to use.

@leave  KErrNotReady
		The image processor is not initialized.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetInputL(aBuffer, aFormat, aSubFormat);
	}

/**
The source image is set or updated to the bitmap provided.

@param  aBitmap
        The bitmap of the new source image to use.
@param  aMask
        The bitmap mask of the new source image to use.

@leave  KErrNotReady
		The image processor is not initialized.
		        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask)
	{
	iImplementation->SetInputL(aBitmap, aMask);
	}

/**
The source image is set or updated to the image frame provided.

@param  aPixelBuffer
        The image frame of the new source image to use.

@leave  KErrNotReady
		The image processor is not initialized.
		        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(const CImageFrame& aPixelBuffer)
	{
	iImplementation->SetInputL(aPixelBuffer);
	}
   
/**
The input is set or updated to the panorama object provided. Any existing options are reset.

@param  aPanorama
        The panorama object to use.

@leave  KErrNotReady
		The image processor is not initialized.
		        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputL(CImagePanorama& aPanorama)
	{
	iImplementation->SetInputL(aPanorama);
	}

/**
This method needs to be called when the input image source data has been updated externally. 

@leave  KErrNotReady
		The image processor is not initialized or input image source data hasn't been set.
		        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::InputUpdatedL()
	{
	iImplementation->InputUpdatedL();
	}

/**
Resets the image processor. image processor's input is reset and image processor's state is set to EInitialized.

@leave  KErrNotReady
		The image processor is not initialized or is initializing. 
		   
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ResetL() 
	{
	iImplementation->ResetL();
	}
	
/**
Initializes the image processor asynchronously. After this asynchronous call completes,the image processor's state should 
normally be EInitialized.

@leave  KErrNotReady
		The image processor has already been initialized.
		  
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor:: InitializeL()
	{
	InitializeL(EOptionNone);
	}

/**
Initializes the image processor. After the initialization completes, the image processor's state should 
normally be EInitialized.

@param	aOptions
		The options to set to this image processor with the initialization.

@leave  KErrNotReady
		The image processor has already been initialized.

@leave  KErrNotSupported
		The option to set to the image processor is not supported.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor:: InitializeL(TUint64 aOptions)
	{
	TRAPD(err, iImplementation->InitializeL(aOptions));
	
	if (err != KErrNone) 
		{
		iImplementation->Cleanup();
		User::Leave(err);
		}
	}
	
/**
All operations and effects are performed on the source image which is then rendered to the output specified. 
The size for the output image being rendered is the same as the size for the input image. Aspect ratio is 
maintained.

The image processor behaves differently according to if ImageProcessor::EOptionSyncProcessing is set or not @see ImageProcessor::CImgProcessor::SetOptionsL.
In the asynchronous case the call leaves with KErrNorReady immediately if the image processor is not initialised. 
If an error occurs during asynchronous processing then the error is reported back to the client via the observer @see ImageProcessor::MImgProcessorObserver.

In the synchronous case i.e. when ImageProcessor::EOptionSyncProcessing is set then the same errors are reported 
except this time by leaving with the error code directly from ProcessL. In this case no callback is made to the 
observer.

@leave  KErrNotReady
		The image processor is not initialized or input is not set or output is not set.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ProcessL()
	{
	iImplementation->ProcessL(TSize(0,0), ETrue);
	}
	
/**
All operations and effects are performed on the source image which is then rendered to the output specified. 
The aspect ratio can be controlled.

@param  aSize
        The proposed size for the output image being rendered. If a size of 0, 0 is passed then the rendered output size is the same as the input size.

@param  aMaintainAspectRatio
        ETrue to maintain the aspect ratio, EFalse otherwise.
        Must be ETrue if aSize is passed as 0,0

@leave  KErrNotReady
		The image processor is not initialized or input is not set or output is not set.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ProcessL(const TSize& aSize, TBool aMaintainAspectRatio)
	{
	iImplementation->ProcessL(aSize, aMaintainAspectRatio);
	}
	
//(Better to have CancelRender() etc?)
/**
Cancels any current asynchronous operation, for example preview or output rendering. 
Ignored if no asynchronous operation is in progress.
*/
EXPORT_C void CImgProcessor::Cancel()
	{
	return iImplementation->Cancel();
	}
	
/**
Sets the background color to use for the output image. Any area of the output image which is not rendered to is set to this colour.

@param  aColor
        The background color to set.

@leave  KErrNotReady
		The image processor is not initialized or input is not set.
        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetBackgroundColorL(const TRgb& aColor)
	{
	iImplementation->SetBackgroundColorL(aColor);
	}
	
/**
Retrieves the current background color.

@return	The current background color.
  
@leave  KErrNotReady
		The input of the image processor is not set.
        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TRgb CImgProcessor::BackgroundColorL() const
	{
	return iImplementation->BackgroundColorL();
	}

/**
Retrieves the interface to an specific effect (e.g. Sepia effect) given an effect UID.

@return	The effect interface corresponding to the given effect UID.

@leave  KErrNotReady
		The image processor is not initialized or is initializing.

@leave  KErrNotSupported
		The given effect is not supported.
 
@leave  KErrNoMemory
		Not enough memory.   
        
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C ImageProcessor::TEffect* CImgProcessor::EffectL(TUid aEffect)
	{
	return iImplementation->EffectL(aEffect);
	}

/**
Determines if there are operations on the operation stack which can be 'undone'.

@return	ETrue if at least one operation applied to the image can be undone; EFalse otherwise.

@leave  KErrNotReady
		The image processor is not initialized or input is not set.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TBool CImgProcessor::CanUndoL() const
	{
	return iImplementation->CanUndoL();
	}

/**
Reverts the most recent operation which can be undone.

@leave  KErrNotReady
		The image processor is not initialized or input is not set.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::UndoL()
	{
	iImplementation->UndoL();
	}

/**
Reverts all operations which can be undone.	

@leave  KErrNotReady
		The image processor is not initialized or input is not set.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::UndoAllL()
	{
	iImplementation->UndoAllL();
	}
	
/**
Determines if there are operations on the operation stack which can be re-applied.

@leave  KErrNotReady
		The image processor is not initialized or input is not set.
		
@return	ETrue if at least one operation applied to the image can be redone; EFalse otherwise.
*/
EXPORT_C TBool CImgProcessor::CanRedoL() const
	{
	return iImplementation->CanRedoL();
	}

/**
Re-applies the most recent redoable operation.

@leave  KErrNotReady
		The image processor is not initialized or input is not set.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::RedoL()
	{
	iImplementation->RedoL();
	}
	
/**
Re-applies all re-doable operations previously undone.	

@leave  KErrNotReady
		The image processor is not initialized or input is not set.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::RedoAllL()
	{
	iImplementation->RedoAllL();
	}

/**
Displays a low resolution preview of the output image with the effects and/or 
operations applied before rendering to a high resolution output image. Multiple previews are possible.

If the requested preview already exists, it will be returned, otherwise a new preview with id aPreviewId will be created 
and returned.

@param	aPreviewId
		The unique id for preview image. It is not the index of the preview of Image Processor. Instead any value 
		can be given here to either return an preview previously created or will create a new one with that id.

@return	A TPreview object.

@leave  KErrNotReady
		The image processor is not initialized or is initializing.
 
@leave  KErrNoMemory
		Not enough memory.   
				
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C ImageProcessor::TPreview* CImgProcessor::PreviewL(TInt aPreviewId)
	{
	return iImplementation->PreviewL(aPreviewId);
	}

//(? are multiple overlays supported?)
/**
Retrieves this image processor's TOverlay object. TOverlay provides functions to overlay a border or clipart onto an image.

@return	The image processor's overlay object.

@leave  KErrNotReady
		The image processor is not initialized or is initializing.
 
@leave  KErrNoMemory
		Not enough memory.   
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C ImageProcessor::TOverlay* CImgProcessor::OverlayL()
	{
	return iImplementation->OverlayL();
	}

/**
The progress infomation of the rendering function which is executed iteratively to enable asynchronous operation. 
This gives the possibility to show a progress bar in the GUI when performing time consuming renderings. Rendering functions
are synchronous if EOptionSyncProcessing option is set on CImgProcessor. 

Retrieves the available progress information in a TProgressInfo obect. 

@return	a TProgressInfo object containing all the progress information.

@leave  KErrNotReady
		The image processor is not initialized or is initializing.
 
@leave  KErrNoMemory
		Not enough memory.   
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C ImageProcessor::TProgressInfo* CImgProcessor::ProgressInfoL() 
	{
	return iImplementation->ProgressInfoL();
	}

/**
Retrieves information about the current source image. 

@return	a TInputInfo filled with all available information about the current source image.

@leave  KErrNotReady
		The image processor is not initialized or is initializing.
 
@leave  KErrNoMemory
		Not enough memory.   
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C ImageProcessor::TInputInfo* CImgProcessor::InputInfoL() 
	{
	return iImplementation->InputInfoL();
	}

/**
Retrieves information about the output image. 

@return	a TOutputInfo filled with all available information about the output image.

@leave  KErrNotReady
		The image processor is not initialized or is initializing.
 
@leave  KErrNoMemory
		Not enough memory.   
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C ImageProcessor::TOutputInfo* CImgProcessor::OutputInfoL() 
	{
	return iImplementation->OutputInfoL();
	}

/**
Sets the area of interest of the source image to be used for processing.

@param	aRect
		A reference to a TRect that specifies the location and size of the region to be used for the source image.

@leave  KErrNotReady
		The image processor is not initialized or input image source data hasn't been set.

@leave  KErrArgument
		The area size is 0 or there is no overlap between aRect and source image.
 
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetInputRectL(const TRect& aRect)
	{
	return iImplementation->SetInputRectL(aRect);
	}
	
/**
Retrieves the size of the current source image.

@return	The size of the current source image.

@leave  KErrNotReady
		The input image source data hasn't been set.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TSize CImgProcessor::CurrentSizeL() const
	{
	return iImplementation->CurrentSizeL();
	}
	
/**
Changes the output image to the image specified in the file given. Rendering is not performed.

@param  aFilename
        The filename of the new output image to use.
@param  aFormat
        The format of the new output image to use.
@param  aSubFormat
        The sub-format of the new output image to use.

@leave  KErrNotReady
		The image processor is not initialized.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetOutputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetOutputL(aFilename, aFormat, aSubFormat);
	}
	
/**
Changes the output image to the image specified in the file given. Rendering is not performed.

@param  aFile
        The file handle of the new output image to use.
@param  aFormat
        The format of the new output image to use.
@param  aSubFormat
        The sub-format of the new output image to use.

@leave  KErrNotReady
		The image processor is not initialized.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetOutputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetOutputL(aFile, aFormat, aSubFormat);
	}
	
/**
Changes the output image to the image specified in the buffer given. Rendering is not performed.

@param  aBuffer
        The buffer containing the new output image to use. This may be re-allocated when rendering is performed if more memory is required.
@param  aFormat
        The format of the new output image to use.
@param  aSubFormat
        The sub-format of the new output image to use.

@leave  KErrNotReady
		The image processor is not initialized.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetOutputL(RBuf8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	{
	iImplementation->SetOutputL(aBuffer, aFormat, aSubFormat);
	}
	
/**
Changes the output image to the image specified in the buffer given. Rendering is not performed.

@param  aPixelBuffer
        The image frame of the new output image to use.

@leave  KErrNotReady
		The image processor is not initialized.

@leave  KErrArgument
		The buffer is empty.	
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetOutputL(CImageFrame& aPixelBuffer)
	{
	iImplementation->SetOutputL(aPixelBuffer);
	}
	
/**
Changes the output image to the image specified in the bitmap given. Rendering is not performed.

@param  aBitmap
        The bitmap of the new output image to use.
@param  aMask
		The bitmap mask of the new output image to use.

@leave  KErrNotReady
		The image processor is not initialized.

@leave  KErrArgument
		The bitmap is empty or given mask is empty.
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::SetOutputL(CFbsBitmap& aBitmap, CFbsBitmap* aMask)// not sure about mask, CAPS doesn't support it
	{
	iImplementation->SetOutputL(aBitmap, aMask);
	}

/**
Retrieves a non-standard extension on the image processor.

@param	aExtension
		The UID of the extension to be retrieved

@return	Extension corresponding to the UID given as a parameter.
*/
EXPORT_C TAny* CImgProcessor::Extension(TUid aExtension)
	{
	return iImplementation->Extension(aExtension);
	}

/**
Converts a given mime type to format and sub-format UIDs.

@param	aMimeType
		The mime type to convert.
@param	aFormat
		The format UID corresponding to the given mime type.
@param	aSubFormat
		The sub-format UID corresponding to the given mime type.

@leave  KErrNotFound
		The given mime type is not found in existing formats.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ConvertMimeTypeToUidL(const TDesC8& aMimeType, TUid& aFormat, TUid& aSubFormat) const
	{
	iImplementation->ConvertMimeTypeToUidL(aMimeType, aFormat, aSubFormat);
	}

/**
Converts a given file extension to format and sub-format UIDs.

@param	aFileExtension
		The file extension to convert.
@param	aFormat
		The format UID corresponding to the given file extension.
@param	aSubFormat
		The sub-format UID corresponding to the given file extension.

@leave  KErrNotFound
		The given file extension is not found in existing formats.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ConvertFileExtensionToUidL(const TDesC& aFileExtension, TUid& aFormat, TUid& aSubFormat) const
	{
	iImplementation->ConvertFileExtensionToUidL(aFileExtension, aFormat, aSubFormat);
	}	

/**
Converts a format and sub-format UID to a mime type.

@param	aMimeType
		Returns the mime type corresponding to the given format and sub-format UIDs. (Consider re-ordering these parameters)
@param	aFormat
		The format UID to convert.
@param	aSubFormat
		The sub-format UID to convert.

@leave  KErrNotFound
		The given format UID is not found in existing mime types.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ConvertUidToMimeTypeL(TDes8& aMimeType, const TUid& aFormat, const TUid& aSubFormat) const
	{
	iImplementation->ConvertUidToMimeTypeL(aMimeType, aFormat, aSubFormat);
	}

//(Is this one or more extension?)
/**
Converts a format and sub-format UIDs to a file extension. 

@param	aFileExtension 
		The file extension corresponding to the given format and sub-format UIDs. (Consider re-ordering these parameters)
@param	aFormat
		The format UID to convert.
@param	aSubFormat
		The sub-format UID to convert.

@leave  KErrNotFound
		The given format UID is not found in existing file extensions.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void CImgProcessor::ConvertUidToFileExtensionL(TDes& aFileExtension, const TUid& aFormat, const TUid& aSubFormat) const
	{
	iImplementation->ConvertUidToFileExtensionL(aFileExtension, aFormat, aSubFormat);
	}
   
//EOF
