// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <icl/imageprocessor.h>
#include "ImageProcessorPriv.h"
#include "ImageUtils.h"
#include "ImageClientMain.h"

//
// ImageProcessorUtility
//

/**
Static function to calculate a reduction factor based on the input parameters.
This function is deprecated. It is recommended to use CImageDecoder::ReductionFactor  or CImageReadCodec::ReductionFactor(for Plugin writers only) to calculate
the reduction factor and then use CImageDecoder::ReducedSize to find the size of the decoded bitmap. 

@param  aOriginalSize
        A reference to the original size of an item.
@param  aReducedSize
        A reference to the new size of an item.
@deprecated - Replaced by CImageDecoder::ReductionFactor
@return The reduction factor.

@see CImageDecoder::ReductionFactor
@see CImageDecoder::ReducedSize
@see CImageReadCodec::ReductionFactor
*/
EXPORT_C TInt ImageProcessorUtility::ReductionFactor(const TSize& aOriginalSize,const TSize& aReducedSize)
	{
	if( (aReducedSize.iWidth<=0) || (aReducedSize.iHeight<=0))
		return 0;

	TInt reductionFactor = 0;

	while( ((aOriginalSize.iWidth-1)>>reductionFactor) >= aReducedSize.iWidth || ((aOriginalSize.iHeight-1)>>reductionFactor) >= aReducedSize.iHeight)
		reductionFactor++;

	return reductionFactor;
	}


/**
Static factory function for creating instances of CImageProcessor derived classes.
This fuction is deprecated. It is recommended for plugin writers to use default implementation of CImageReadCodec::ReductionFactor to calculate the
reduction factor and then use NewImageProcessorL(const CFbsBitmap& aBitmap,TInt aReductionFactor,TDisplayMode aImageDisplayMode, TBool aDisableErrorDiffusion)

@param  aBitmap
        A reference to the bitmap used.
@param  aImageSize
        The size of image to use.
@param  aImageDisplayMode
        The display mode to use.
@param  aDisableErrorDiffusion
        A flag indicating whether error diffusion should be disabled.
@deprecated - Replaced by CImageReadCodec::ReductionFactor & NewImageProcessorL(const CFbsBitmap& aBitmap,TInt aReductionFactor,TDisplayMode aImageDisplayMode, TBool aDisableErrorDiffusion)
@return A pointer to a fully constructed CImageProcessor derived object.

@see CImageReadCodec::ReductionFactor
@see ImageProcessorUtility::NewImageProcessorL(const CFbsBitmap& aBitmap,TInt aReductionFactor,TDisplayMode aImageDisplayMode, TBool aDisableErrorDiffusion)
*/
EXPORT_C CImageProcessor* ImageProcessorUtility::NewImageProcessorL(const CFbsBitmap& aBitmap,const TSize& aImageSize,TDisplayMode aImageDisplayMode, TBool aDisableErrorDiffusion)
	{
	TInt reductionFactor = ImageProcessorUtility::ReductionFactor(aImageSize,aBitmap.SizeInPixels());
	return NewImageProcessorL(aBitmap,reductionFactor, aImageDisplayMode, aDisableErrorDiffusion);
	}

/**
Static factory function for creating instances of CImageProcessor derived classes.

@param  aBitmap
        A reference to the bitmap used.
@param  aReductionFactor
        The value indicating how much to shrink the bitmap.
@param  aImageDisplayMode
        The display mode to use.
@param  aDisableErrorDiffusion
        A flag indicating whether error diffusion should be disabled.

@return A pointer to a fully constructed CImageProcessor derived object.
*/
EXPORT_C CImageProcessor* ImageProcessorUtility::NewImageProcessorL(const CFbsBitmap& aBitmap,TInt aReductionFactor,TDisplayMode aImageDisplayMode, TBool aDisableErrorDiffusion)
	{
	TBool useErrorDiffuser = EFalse;
	if(!aDisableErrorDiffusion)
		{
		useErrorDiffuser = ImageProcessorUtility::UseErrorDiffuser(aBitmap.DisplayMode(), aImageDisplayMode);
		}
	
	TBool monochrome = ImageProcessorUtility::IsMonochrome(aBitmap.DisplayMode(), aImageDisplayMode);
	
	CImageProcessorExtension* imageProcessor = NULL;
	
	if(monochrome)
		{
		if(useErrorDiffuser)
			{
			imageProcessor = CMonochromeErrorDiffuser::NewL();
			}
		else
			{
			imageProcessor = CMonochromePixelWriter::NewL();
			}
		}
	else
		{
		if(useErrorDiffuser)
			{
			imageProcessor = CErrorDiffuser::NewL();
			}
		else
			{
			imageProcessor = CPixelWriter::NewL();
			}
		}

	if(aReductionFactor)
		{
		CleanupStack::PushL(imageProcessor);

		if(monochrome)
			{
			imageProcessor = CMonochromeThumbnailProcessor::NewL(imageProcessor,aReductionFactor);
			}
		else
			{
			imageProcessor = CThumbnailProcessor::NewL(imageProcessor,aReductionFactor);
			}

		CleanupStack::Pop();
		}

	return imageProcessor;
	}

/**
@publishedAll
@released

Static factory function for creating instances of CImageProcessorExtension derived classes.

@param  aBitmap
        A reference to the bitmap used.
@param  aReductionFactor
        The value indicating how much to shrink the bitmap.
@param  aImageDisplayMode
        The display mode to use.
@param  aDisableErrorDiffusion
        A flag indicating whether error diffusion should be disabled.

@return A pointer to a fully constructed CImageProcessorExtension derived object.
*/
EXPORT_C CImageProcessorExtension* ImageProcessorUtility::NewImageProcessorExtensionL(const CFbsBitmap& aBitmap,TInt aReductionFactor,TDisplayMode aImageDisplayMode, TBool aDisableErrorDiffusion)
	{
	TBool useErrorDiffuser = EFalse;
	if(!aDisableErrorDiffusion)
		{
		useErrorDiffuser = ImageProcessorUtility::UseErrorDiffuser(aBitmap.DisplayMode(), aImageDisplayMode);
		}
	
	TBool monochrome = ImageProcessorUtility::IsMonochrome(aBitmap.DisplayMode(), aImageDisplayMode);
	
	CImageProcessorExtension* imageProcessor = NULL;
	
	if(monochrome)
		{
		if(useErrorDiffuser)
			{
			imageProcessor = CMonochromeErrorDiffuser::NewL();
			}
		else
			{
			imageProcessor = CMonochromePixelWriter::NewL();
			}
		}
	else
		{
		if(useErrorDiffuser)
			{
			imageProcessor = CErrorDiffuser::NewL();
			}
		else
			{
			imageProcessor = CPixelWriter::NewL();
			}
		}

	if(aReductionFactor)
		{
		CleanupStack::PushL(imageProcessor);

		if(monochrome)
			{
			imageProcessor = CMonochromeThumbnailProcessor::NewL(imageProcessor,aReductionFactor);
			}
		else
			{
			imageProcessor = CThumbnailProcessor::NewL(imageProcessor,aReductionFactor);
			}

		CleanupStack::Pop();
		}

	return imageProcessor;
	}

/**
Private static funcion to determine whether an error diffusing image processor is to be created.
*/
TBool ImageProcessorUtility::UseErrorDiffuser(const TDisplayMode& aBitmapDisplayMode, const TDisplayMode& aImageDisplayMode)
	{
	TBool useErrorDiffuser = EFalse;

	if((aBitmapDisplayMode != EGray256) && 
		(aBitmapDisplayMode != EColor16M) && 
		(aBitmapDisplayMode != ERgb) && 
		(aBitmapDisplayMode != EColor16MU) &&
		(aBitmapDisplayMode != EColor16MA))
		{
		switch(aImageDisplayMode)
			{
			case EGray2:
				break;

			case EGray4:
				if(aBitmapDisplayMode < aImageDisplayMode)
					{
					useErrorDiffuser = ETrue;
					}
				break;

			case EGray16:
				if((aBitmapDisplayMode < aImageDisplayMode)||(aBitmapDisplayMode == EColor16))
					{
					useErrorDiffuser = ETrue;
					}
				break;

			case EGray256:
				useErrorDiffuser = ETrue;
				break;

			case EColor16:
				if(aBitmapDisplayMode < aImageDisplayMode)
					{
					useErrorDiffuser = ETrue;
					}
				break;

			case EColor256:
				if(aBitmapDisplayMode < aImageDisplayMode)
					{
					useErrorDiffuser = ETrue;
					}
				break;

			case EColor64K:
				if(aBitmapDisplayMode == EColor4K)
					{
					useErrorDiffuser = ETrue;
					}
				if(aBitmapDisplayMode < aImageDisplayMode)
					{
					useErrorDiffuser = ETrue;
					}
				break;

			case EColor16M:
			case ERgb:
			case EColor16MU:
				useErrorDiffuser = ETrue;
				break;

			case EColor4K:
				if(aBitmapDisplayMode < EColor64K)
					{
					useErrorDiffuser = ETrue;
					}
				break;

			default:
				break;
			}
		}
	return useErrorDiffuser;
	}

/**
Private static funcion to determine whether a monochrome image processor is to be created.
*/
TBool ImageProcessorUtility::IsMonochrome(const TDisplayMode& aBitmapDisplayMode, const TDisplayMode& aImageDisplayMode)
	{
	return aBitmapDisplayMode <= EGray256 || aImageDisplayMode <= EGray256;
	}
	
/**
Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageProcessor::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageProcessor::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageProcessor::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageProcessor::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

//
// CErrorDiffuser
//

/**
 * @internalComponent
 * Static factory function to create CErrorDiffuser objects.
 *
 * @return  Pointer to a fully constructed CErrorDiffuser object. 
 */
//Exported for CBitmapConverter in MediaClientImage
EXPORT_C CErrorDiffuser* CErrorDiffuser::NewL()
	{
	return new(ELeave) CErrorDiffuser;
	}


