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

#include <imageprocessor/imageprocessorpreview.h>
#include <imageprocessor/imageprocessorpreviewintf.h>
#include "imageprocessorimpl.h"

#include <imageframe.h>

namespace ImageProcessor
	{

const TBool TPreview::iValidStates[TPreview::EStatesCount][TPreview::EStatesCount] = 
	{
		//	EUninitialized	EInitializing	EInitialized	ERendering	 
		{	
			ETrue, 			ETrue, 			EFalse, 		EFalse		//EUninitialized
		},		
		{	
			ETrue, 			ETrue, 			ETrue,			EFalse		//EInitializing
		},
		{	
			ETrue, 			EFalse,			ETrue, 			ETrue		//EInitialized
		},		
		{	
			EFalse, 		EFalse, 		ETrue,			ETrue		//ERendering
		}
	};

/**
Gets the supported output formats for the preview.

@return	The supported output image formats.
*/
EXPORT_C void TPreview::SupportedImageFrameFormatsL(RArray<TUid>& aFormats) const 
	{
	aFormats = iImageProcessorImpl.PreviewOutputImageFrameFormatsL();
	}

/**
Gets the supported output display modes for the preview.

@return	The supported output display modes.
*/
EXPORT_C void TPreview::SupportedOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const 
	{
	aDisplayModes = iImageProcessorImpl.PreviewOutputDisplayModesL();
	}

/**
Sets CFbsBitmap as the output bitmap of the preview.

@param	aBitmap
        The output bitmap

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy

@leave	A range of system wide error codes

*/
EXPORT_C void TPreview::SetOutputL(CFbsBitmap& aBitmap) 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));

	if (aBitmap.SizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	    {
	    User::Leave(KErrNotSupported);
	    }
	
	iPreviewImpl.SetOutputL(aBitmap);
	iIsOutputSet = ETrue;
	}

/**
Sets an image frame as the output bitmap of the preview.

@param	aPixelBuffer
        The CImageFrame buffer.

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::SetOutputL(CImageFrame& aPixelBuffer) 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));

	if (aPixelBuffer.FrameSizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}

	iPreviewImpl.SetOutputL(aPixelBuffer);
	iIsOutputSet = ETrue;
	}

/**
Creates an internal pixel buffer for output. Internal buffer size is calculated using aPixelBuffer properties (size, scanline length, display mode).

@param	aFrameSize
		A reference to a TSize object that defines the frame size in pixels of the CImageFrame object.

@param	aFrameFormat
		A reference to a TFrameFormatBase object that defines the format of the CImageFrame object.		

@param	aFrameLayout
		A reference to a TFrameLayoutBase object that defines the memory layout of the CImageFrame object.

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy

@leave	Other
		A range of system wide error codes.

@return	CImageFrame instance containing newly created pixel buffer. The image frame is owned by the client and deleting it does not delete the pixel buffer which is owned by the image processor framework.
The pixel buffer becomes invalid if TPreview::CreateOutputL(const TSize &aFrameSize, const TFrameFormatBase &aFrameFormat, const TFrameLayoutBase &aFrameLayout) or TPreview::SetOutputL(CFbsBitmap& aBitmap) or TPreview::SetOutputL(CImageFrame& aPixelBuffer) calls are made.
*/
EXPORT_C CImageFrame* TPreview::CreateOutputL(const TSize &aFrameSize, const TFrameFormatBase &aFrameFormat, const TFrameLayoutBase &aFrameLayout) 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));
	
	if (aFrameSize == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	CImageFrame* imageFrame = iPreviewImpl.CreateOutputL(aFrameSize, aFrameFormat, aFrameLayout);
	
	iIsOutputSet = ETrue;
	return imageFrame;
	}

/**
Initializes the preview.

@leave KErrNorReady if preview is not in state TPreview::EUninitialized

@leave A range of system wide error code.
*/
EXPORT_C void TPreview::InitializeL()
	{
	SetStateL(TPreview::EInitializing);
	
	TRAPD(err, iPreviewImpl.InitializeL());

	if (err != KErrNone || (iImageProcessorImpl.Options() & CImgProcessor::EOptionSyncProcessing) != 0) 
		{
		if (err != KErrNone) 
			{
			iState = TPreview::EUninitialized;
			}
		else 
			{
			iState = TPreview::EInitialized;
			}
		
		iImageProcessorImpl.RestoreStateL();
		if (err != KErrNone) 
			{
			User::Leave(err);
			}
		}
	}

/**
Uninitializes the preview.

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy
		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::UninitializeL()
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));
	iPreviewImpl.UninitializeL();
	iState = TPreview::EUninitialized;
	iIsOutputSet = EFalse;
	}

/**
Gets the current state of the preview.

@return	The current state of the preview.
*/
EXPORT_C TPreview::TState TPreview::State() const
	{
	return iState;
	}

/**
Gets the id of the preview.

@return	The id of the preview.
*/
EXPORT_C TInt TPreview::PreviewId() const
	{
	return iPreviewImpl.PreviewId();
	}

/**
Specifies the id of the preview.

@param	aPreviewId
        The id of the current preview
*/
EXPORT_C void TPreview::SetPreviewId(TInt aPreviewId)
	{
	iPreviewImpl.SetPreviewId(aPreviewId);
	}

/**
Starts the rendering. The preview state is set to ERendering

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy or input is not set or output is not set.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::RenderL()
	{
	__ASSERT_ALWAYS(((!iImageProcessorImpl.IsBusy(iImageProcessorImpl.State())) && 
					(iImageProcessorImpl.IsInputSet()) && 
					(iState == TPreview::EInitialized) && 
					(iIsOutputSet)), 
					User::Leave(KErrNotReady));

	SetStateL(TPreview::ERendering);

	TRAPD(err, iPreviewImpl.RenderL());
	
	if (err != KErrNone || (iImageProcessorImpl.Options() & CImgProcessor::EOptionSyncProcessing) != 0) 
		{
		iState = TPreview::EInitialized;
		iImageProcessorImpl.RestoreStateL();
		if (err != KErrNone) 
			{
			User::Leave(err);
			}
		}
	}

/**
Cancels the preview rendering if the preview state is EInitializing or
ERendering, unconditionally aborts the operation.
*/
EXPORT_C void TPreview::Cancel() 
	{
	if (iState == TPreview::EInitializing || iState == TPreview::ERendering) 
		{
		iPreviewImpl.Cancel();
		iState = TPreview::EInitialized;
		TRAPD(err, iImageProcessorImpl.RestoreStateL());
		if (err)//ignore error for now
			{
			}
		}
	}

/**
Resets the preview parameters and state.
*/
EXPORT_C void TPreview::ResetL() 
	{
	Cancel();
	iIsOutputSet = EFalse;
	iPreviewImpl.ResetL();
	}

/**
Retrieves the zoom level range. 

@param	aMinimumLevel
        The minimum value of the zoom factor.
        
@param	aMaximumLevel
        The maximum value of the zoom factor.        
*/
EXPORT_C void TPreview::GetZoomLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel)
	{
	iPreviewImpl.GetZoomLevelSettings(aMinimumLevel, aMaximumLevel);
	}

/**
Specifies the zoom factor for the preview screen. The screen will be zoomed after rendering.

@param	aZoom
        The zoom factor for the current preview, 1.0f to infinite.

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::SetZoomL(TReal32 aZoom) 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));
	iPreviewImpl.SetZoomL(aZoom);
	}

/**
Gets the current zoom factor of the preview screen.

@return	The current zoom factor of the preview screen.

@leave	KErrNotReady
		The current state is EUninitialized or the ImageProcessor is busy.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TReal32 TPreview::ZoomL() const
	{
	__ASSERT_ALWAYS((iState != TPreview::EUninitialized), User::Leave(KErrNotReady));
	return iPreviewImpl.ZoomL();
	}

/**
Retrieves the pan level range. 

@param	aMinimumLevel
        The minimum value of the pan factor.
        
@param	aMaximumLevel
        The maximum value of the pan factor.        
*/
EXPORT_C void TPreview::GetPanLevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel)
	{
	iPreviewImpl.GetPanLevelSettings(aMinimumLevel, aMaximumLevel);
	}

/**
Specifies the pan factor for the preview screen. The zoomed screen will be panned to the new 
coordinate after rendering.

@param	aPanX
        The horizontal pan factor for the current preview, -1.0f to 1.0f.

@param	aPanY
        The vertical pan factor for the current preview, -1.0f to 1.0f.

@leave	KErrNotReady
		The current state is not EInitialized or the ImageProcessor is busy
		
@leave	KErrArgument
		The pan factor is out of range.	

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::SetPanL(TReal32 aPanX, TReal32 aPanY) 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized),		User::Leave(KErrNotReady));
	iPreviewImpl.SetPanL(aPanX, aPanY);
	}

/**
Gets the current pan factor of the preview screen.

@leave	KErrNotReady
		The current state is EUninitialized or the ImageProcessor is busy.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::PanL(TReal32& aPanX, TReal32& aPanY) const 
	{
	__ASSERT_ALWAYS((iState != TPreview::EUninitialized), User::Leave(KErrNotReady));
	iPreviewImpl.PanL(aPanX, aPanY);
	}

/**
Gets the current size of the preview screen.

@return	The current size of the preview screen.

@leave	KErrNotReady
		The current state is EUninitialized.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TSize TPreview::SizeL() const 
	{
	__ASSERT_ALWAYS((iState != TPreview::EUninitialized), User::Leave(KErrNotReady));
	return iPreviewImpl.SizeL();
	}

/**
Gets the current canvas  as TRect in the given screen

@return	The current canvas of the preview screen.

@leave	KErrNotReady
		The current state is EUninitialized.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TRect TPreview::CanvasAreaL() const 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));
	return iPreviewImpl.CanvasAreaL();
	}

/**
Converts a coordinate from preview screen to current coordinate system. 

@param	aPreviewPoint
        A TPoint pointer specifying the preview screen from which to convert the coordinate.

@param	aCurrentPoint
        Pointer to a TPoint object with the coordinate to convert. The converted coordinate is stored back into the object.

@leave	KErrNotReady
		The current state is EUninitialized.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::PreviewToCurrentCoordL(const TPoint& aPreviewPoint, TPoint& aCurrentPoint) const 
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));
	iPreviewImpl.PreviewToCurrentCoordL(aPreviewPoint, aCurrentPoint);
	}

/**
Converts a coordinate from canvas to current coordinate system. 

@param	aCanvasPoint
        A TPoint pointer specifying a screen in which the canvas from which to convert the coordinate is located.

@param	aCurrentPoint
        Pointer to a TPoint object with the coordinate to convert. The converted coordinate is stored back into the object.

@leave	KErrNotReady
		The current state is EUninitialized.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TPreview::CanvasToCurrentCoordL(const TPoint& aCanvasPoint, TPoint& aCurrentPoint) const
	{
	__ASSERT_ALWAYS((iState == TPreview::EInitialized), User::Leave(KErrNotReady));
	iPreviewImpl.CanvasToCurrentCoordL(aCanvasPoint, aCurrentPoint);
	}

// internal functions
TPreview::TPreview(Plugin::MPreview& aPreviewImpl, CImageProcessorImpl& aImageProcessorImpl) : 
	iImageProcessorImpl(aImageProcessorImpl),
	iPreviewImpl(aPreviewImpl), 
	iState(TPreview::EUninitialized),
	iIsOutputSet(EFalse),
	iReserved(0)
	{
	}

void TPreview::SetStateL(TPreview::TState aState)
	{
	if(iState != aState)
		{
		if (!iValidStates[iState][aState]) 
			{
			User::Leave(KErrNotReady);
			}
		
		if (aState == TPreview::EInitializing) 
			{
			iImageProcessorImpl.SetStateL(CImgProcessor::EPreviewInitializing);
			}
		else if (aState == TPreview::ERendering)
			{
			iImageProcessorImpl.SetStateL(CImgProcessor::EPreviewRendering);
			}
	
		iState = aState;
		}
	}
   
	}//namespace ImageProcessor

//EOF
