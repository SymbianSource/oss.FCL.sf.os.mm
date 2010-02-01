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

#include <imageprocessor/imageprocessoroverlayintf.h>
#include <imageprocessor/imageprocessoroverlay.h>
#include "imageprocessorimpl.h"
#include <imageprocessor/imageprocessor.h>

#include <imageframe.h>

namespace ImageProcessor
	{

TOverlay::TOverlay(Plugin::MOverlay& aOverlayImpl, CImageProcessorImpl& aImageProcessorImpl)
:	iIsActive(EFalse),
	iIsInputSet(EFalse),
	iImageProcessorImpl(aImageProcessorImpl),
 	iOverlayImpl(aOverlayImpl),
	iReserved(0)
	{
	}

/**
Gets the supported overlay image formats.

@param	aFormats
		The supported formats of ImageProcessor of the overlay image.
*/
EXPORT_C void TOverlay::SupportedInputFormatsL(RArray<TUid>& aFormats) const
	{
	aFormats = iImageProcessorImpl.OverlayInputFormatsL();
	}

/**
Gets the supported overlay image sub formats.

@param	aFormat
		The supported format of ImageProcessor of the overlay image.
		
@param	aSubFormats
		The supported sub formats of ImageProcessor of the overlay image.
*/
EXPORT_C void TOverlay::SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const
	{
	iOverlayImpl.SupportedInputSubFormatsL(aFormat, aSubFormats);	
	}	

/**
Gets the supported overlay image frame formats.

@param	aFormats
		The supported formats of overlay image of ImageProcessor.
*/
EXPORT_C void TOverlay::SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats) const
	{
	aFormats = iImageProcessorImpl.OverlayInputImageFrameFormatsL();
	}

/**
Gets the supported overlay image display modes.

@param	aDisplayModes
		The supported display modes of overlay image of ImageProcessor.
*/
EXPORT_C void TOverlay::SupportedInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const
	{
	aDisplayModes = iImageProcessorImpl.OverlayInputDisplayModesL();
	}

/**
Specifies the file name and format of the overlay image. The input must be set only once and 
after the BeginL() is done. So, the SetInputL() must be called only once between BeginL() 
and EndL(). 

@param	aFilename
		The file name of the overlay image.
@param	aFormat
		The format of the overlay image.
@param	aSubFormat
		The sub format of the overlay image. Currently is not supported, shoud set to KNullUid.

@leave KErrNotReady	
	   BeginL() hasn't been called to create overlay.
	   	   
@leave KErrNotSupported	
	   The aSubFormat is not NULL.
	   
@leave KErrArgument	
	   The file name length is longer than KMaxFileName.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	{
	iOverlayImpl.SetInputL(aFilename, aFormat, aSubFormat);
	iImageProcessorImpl.ResetOverlayDrmInput();
	iIsInputSet = ETrue;
	}

/**
Specifies the file pointer and format of the overlay image. The input must be set only once and 
after the BeginL() is done. So, the SetInputL() must be called only once between BeginL() 
and EndL().  

@param	aFile
		The pointer to the file of the overlay image.
@param	aFormat
		The format of the overlay image.
@param	aSubFormat
		The sub format of the overlay image. Currently is not supported, shoud set to KNullUid.
	
@leave	KErrNotSupported
		Always. Current this function is not supported.
*/
EXPORT_C void TOverlay::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	iOverlayImpl.SetInputL(aFile, aFormat, aSubFormat);
	iImageProcessorImpl.ResetOverlayDrmInput();
	iIsInputSet = ETrue;
	}

/**
Specifies the file pointer and format of the overlay image which is a DRM protected content source.
The input must be set only once and after the BeginL() is done. So, the SetInputL() must be called
only once between BeginL() and EndL(). 

@param	aDrmFile
		The pointer to the DRM file of the overlay image.
@param	aFormat
		The format of the overlay image.
@param	aSubFormat
		The sub format of the overlay image. Currently is not supported, shoud set to KNullUid.
	
@leave	KErrNotSupported
		Always. Current this function is not supported.
*/
EXPORT_C void TOverlay::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	CImageProcessorDrmInput* drmInput = CImageProcessorDrmInput::NewL(aDrmFile);

	CleanupStack::PushL(drmInput);
	iOverlayImpl.SetInputL(*(drmInput->Buffer()), aFormat, aSubFormat);
	CleanupStack::Pop(drmInput);
	
	iImageProcessorImpl.ResetOverlayDrmInput();
	iImageProcessorImpl.SetOverlayDrmInput(drmInput);
	iIsInputSet = ETrue;
	}

/**
Specifies the buffer pointer and format of the overlay image. The input must be set only once and 
after the BeginL() is done. So, the SetInputL() must be called only once between BeginL() 
and EndL().  

@param	aBuffer
		The pointer to the buffer of the overlay image.
@param	aFormat
		The format of the overlay image.
@param	aSubFormat
		The sub format of the overlay image. Currently is not supported, shoud set to KNullUid.

@leave KErrNotReady	
	   BeginL() hasn't been called to create the overlay.
	   	   
@leave KErrNotSupported	
	   The aSubFormat is not NULL.
	   	
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	{
	if (aBuffer.Size() == 0) 
		{
		User::Leave(KErrArgument);
		}
	
	iOverlayImpl.SetInputL(aBuffer, aFormat, aSubFormat);
	iImageProcessorImpl.ResetOverlayDrmInput();
	iIsInputSet = ETrue;
	}

/**
Specifies the bitmap and mask of the overlay image. 
Source bitmap must not be deleted or changed during lifetime of TOverlay instance.
The input must be set only once and after the BeginL() is done. So, the SetInputL() must be called
only once between BeginL() and EndL(). 

@param	aBitmap
		The pointer to the bitmap of the overlay image.
@param	aMask
		The mask of the overlay image. Currently not supported, mush be NULL.

@leave KErrNotReady	
	   BeginL() hasn't been called to create the overlay.
	   	   
@leave KErrNotSupported	
	   The aMask is not NULL.
	   	
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask)
	{
	if (aBitmap.SizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
	    {
	    User::Leave(KErrNotSupported);
	    }
	
	if(aMask!=NULL && aMask->Handle()!=0 && aMask->ExtendedBitmapType()!=KNullUid)
	        {
	        User::Leave(KErrNotSupported);
	        }
	
	if ((aMask != NULL) && (aMask->SizeInPixels() == TSize(0,0))) 
		{
		User::Leave(KErrArgument);
		}

	iOverlayImpl.SetInputL(aBitmap, aMask);
	iImageProcessorImpl.ResetOverlayDrmInput();
	iIsInputSet = ETrue;
	}

/**
Specifies the binary image data of the overlay image.
Source image frame buffer must not be deleted or changed during lifetime of TOverlay instance.
The input must be set only once and after the BeginL() is done. So, the SetInputL() must be called
only once between BeginL() and EndL(). 

@param	aPixelBuffer
		The CImageFrame type pointer of the overlay image.

@leave KErrNotReady	
	   BeginL() hasn't been called to create the overlay.
	
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::SetInputL(const CImageFrame& aPixelBuffer) 
	{
	if (aPixelBuffer.FrameSizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}

	iOverlayImpl.SetInputL(aPixelBuffer);
	iImageProcessorImpl.ResetOverlayDrmInput();
	iIsInputSet = ETrue;
	}

/**
First step of the three stages overlay BeginL()/SetL()/EndL(). 
The state of the ImageProcessor is set to EEffectActive.
	
@leave	KErrNotReady
		The state of the ImageProcessor is not EInitialized or overlay is already begun.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::BeginL()
	{
	__ASSERT_ALWAYS(((iImageProcessorImpl.State() == CImgProcessor::EInitialized) && !iIsActive), User::Leave(KErrNotReady));
	iOverlayImpl.BeginL();
	iImageProcessorImpl.SetStateL(CImgProcessor::EEffectActive);
	iIsActive = ETrue;
	}

/**
Second step of the three stages overlay BeginL()/SetL()/EndL(). 
Specifies the size, geometric centre position and angle of the overlay image.

@param	aSize
		The size of the overlay image.
@param	aPosition
		The position of the overlay image geometric centre.
@param	aAngle
		The angle of the overlay image.
@leave 	KErrNotReady	
	   	BeginL() hasn't been called to create the overlay. 
@leave 	KErrArgument
		Width or height of aSize is less than 0.
@leave 	KErrArgument
		aSize does not preserve aspect ratio and the plugin implementation 
		supports scaling with aspect ratio preserved only.
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::SetL(const TSize &aSize, const TPoint &aPosition, TReal32 aAngle)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	__ASSERT_ALWAYS(((aSize.iWidth > 0) && (aSize.iHeight > 0)), User::Leave(KErrArgument));
	iOverlayImpl.SetL(aSize, aPosition, aAngle);
	}

/**
Second step of the three stages overlay BeginL()/SetL()/EndL(). 
Specifies the scale, geometric centre position and angle of the overlay image.

@param	aScaleX
		The scale of the overlay image width.
@param	aScaleY
		The scale of the overlay image height.
@param	aPosition
		The position of the overlay image geometric centre.
@param	aAngle
		The angle of the overlay image.	
@leave 	KErrArgument
		Scale is less than 0 or equal to 0
@leave 	KErrArgument
		aScaleX and aScaleY are not equal and the plugin implementation supports scaling with
		aspect ratio preserved only.
@leave 	KErrNotReady	
	   	BeginL() hasn't been called to create the overlay.
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::SetL(TReal32 aScaleX, TReal32 aScaleY, const TPoint &aPosition, TReal32 aAngle)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	__ASSERT_ALWAYS(((aScaleX > 0) && (aScaleY > 0)), User::Leave(KErrArgument));
	iOverlayImpl.SetL(aScaleX, aScaleY, aPosition, aAngle);
	}

/**
Last step of the three stages overlay BeginL()/SetL()/EndL(). 
The state of the ImageProcessor is set to EInitialized.

@leave 	KErrNotReady	
	   	BeginL() hasn't been called to create the overlay.
	   		
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::EndL()
	{
	__ASSERT_ALWAYS(((iImageProcessorImpl.State() == CImgProcessor::EEffectActive) && 
					(iIsActive) && 
					(iIsInputSet) &&
					(iImageProcessorImpl.IsInputSet())), User::Leave(KErrNotReady));
	
	
	iOverlayImpl.EndL();
	iImageProcessorImpl.SetStateL(CImgProcessor::EInitialized);
	iIsActive = EFalse;
	iIsInputSet = EFalse;
	iImageProcessorImpl.AppendOverlayDrmInputL();
	}

/**
Resets the overlay parameters and state.
	
@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::ResetL()
	{
	if (iIsActive) 
		{
		EndL();
		iImageProcessorImpl.UndoL();
		}
	
	iImageProcessorImpl.ResetOverlayDrmInput();
	iIsInputSet = EFalse;

	iOverlayImpl.ResetL();
	}

/**
Gets the geometric centre position of the overlay image.

@return	The position of the overlay image geometric centre.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TPoint	TOverlay::PositionL() const
	{
	return iOverlayImpl.PositionL();
	}

/**
Gets the size of the overlay image.

@return	The size of the overlay image.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TSize	TOverlay::SizeL() const
	{
	return iOverlayImpl.SizeL();
	}

/**
Gets the angle of the overlay image.

@return	The angle of the overlay image.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C TReal32 TOverlay::AngleL() const
	{
	return iOverlayImpl.AngleL();
	}

/**
Gets the scale of the overlay image.

@param	aScaleX
		The scale of the overlay image width.
@param	aScaleY
		The scale of the overlay image height.

@leave	Other
		A range of system wide error codes.
*/
EXPORT_C void TOverlay::GetScaleL(TReal32& aScaleX, TReal32& aScaleY) const
	{
	iOverlayImpl.GetScaleL(aScaleX, aScaleY);
	}

	}//namespace ImageProcessor

//EOF
