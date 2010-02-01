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

#include <imageprocessor/imageprocessorinputinfointf.h>
#include <imageprocessor/imageprocessorinputinfo.h>
#include "imageprocessorimpl.h"
#include <imageprocessor/imageprocessor.h>

namespace ImageProcessor
	{

TInputInfo::TInputInfo(Plugin::MInputInfo& aInputInfoImpl, CImageProcessorImpl& aImageProcessorImpl)
:iImageProcessorImpl(aImageProcessorImpl), 
	iInputInfoImpl(aInputInfoImpl),
	iReserved(0)
	{
	}

/**
Gets the size of input image

@return	size of the image
*/
EXPORT_C TSize TInputInfo::SizeL() const
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.SizeL();
	}

/**
Gets the input image format

@param	aFormat
        Returns the Uid of the format.
@param	aSubFormat
        Returns the sub Uid of the format.
@leave	KErrNotReady
		The effect is not active.
*/
EXPORT_C void TInputInfo::FormatL(TUid& aFormat, TUid& aSubFormat) const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	iInputInfoImpl.FormatL(aFormat, aSubFormat);
	}

/**
Gets the display mode of input image.

@return	display mode of image.
*/
EXPORT_C TDisplayMode TInputInfo::DisplayModeL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.DisplayModeL();
	}
	
/**
Gets the uid of frame format of input image.
When calling TInputInfo::DisplayModeL() returns ENone, this ImageFrameFormatL() 
could be used to get image frame format.

@return	uid of frame format of the image.
*/
EXPORT_C TUid TInputInfo::ImageFrameFormatL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.ImageFrameFormatL();
	}

/**
Gets the sampling of input image.

@return	sampling of image.
*/
EXPORT_C TUid TInputInfo::SamplingL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.SamplingL();
	}
	
/**
Gets the bits per pixel of input image.

@return	bits per pixel of image.
*/
EXPORT_C TUint32 TInputInfo::BitsPerPixelL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.BitsPerPixelL();
	}
	
/**
Gets the source image information and scans
whether the source image allows progressive decoding or not.

@return	ETrue if the source image allows progressive decoding otherwise EFalse.
*/
EXPORT_C TBool TInputInfo::IsProgressiveL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.IsProgressiveL();
	}
	
/**
Gets the source image information and scans whether the source image is interlaced or not. 

@return	ETrue if the source image is interlaced otherwise EFlase.
*/
EXPORT_C TBool TInputInfo::IsInterlacedL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.IsInterlacedL();
	}
	
/**
Gets the the image alpha information.

@return	true if input image has alpha channel.
*/
EXPORT_C TBool TInputInfo::HasAlphaL() const
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iInputInfoImpl.HasAlphaL();
	}

	}//namespace ImageProcessor

//EOF
