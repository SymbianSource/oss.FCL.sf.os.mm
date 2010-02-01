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

/**
 @file
 @publishedPartner 
 @released 
*/

#ifndef __IMAGE_PROCESSOR_PLUGIN_H__
#define __IMAGE_PROCESSOR_PLUGIN_H__

#include <e32base.h>
#include <gdi.h>
#include <imageprocessor/imageprocessor.h>

class RFs;
class CFbsBitmap;
class CImageFrame;
class TMMSource;

namespace ImageProcessor
	{
	class MImgProcessorPluginObserver;

	/** @publishedPartner
	@released
	ECOM plugin namespace for ImageProcessor.
	*/
	namespace Plugin
		{
		class MEffect;
		class MPreview;
		class MOverlay;
		class MProgressInfo;
		class MInputInfo;
		class MOutputInfo;
		}
/**
Mixin class for implementation by providers of the Image Processor Plugin.

@see	CImgProcessor

@publishedPartner
@released
*/	
class MImgProcessorPlugin
	{
public:
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetFileServerSession(RFs& aFileServerSession)
	
	@see CImgProcessor::SetFileServerSession(RFs& aFileServerSession)
	*/	
	virtual void SetFileServerSession(RFs& aFileServerSession) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetObserver(MImgProcessorPluginObserver& aObserver)
	
	@see CImgProcessor::SetObserver(MImgProcessorPluginObserver& aObserver)
	*/	
	virtual void SetObserver(MImgProcessorPluginObserver& aObserver) = 0;
   
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedEffectsL(RArray<TUid>& aEffects)
	
	@see CImgProcessor::SupportedEffectsL(RArray<TUid>& aEffects)
	*/	
	virtual void SupportedEffectsL(RArray<TUid>& aEffects) const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedInputFormatsL(RArray<TUid>& aFormats)
	
	@see CImgProcessor::SupportedInputFormatsL(RArray<TUid>& aFormats)
	*/	
	virtual void SupportedInputFormatsL(RArray<TUid>& aFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats)
	
	@see CImgProcessor::SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats)
	*/	
	virtual void SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats)
	
	@see CImgProcessor::SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats)
	*/	
	virtual void SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats) const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedInputDisplayModesL((RArray<TDisplayMode>& aDisplayModes)
	
	@see CImgProcessor::SupportedInputDisplayModesL((RArray<TDisplayMode>& aDisplayModes)
	*/	
	virtual void SupportedInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOutputFormatsL(RArray<TUid>& aFormats)
	
	@see CImgProcessor::SupportedOutputFormatsL(RArray<TUid>& aFormats)
	*/	
	virtual void SupportedOutputFormatsL(RArray<TUid>& aFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOutputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats)
	
	@see CImgProcessor::SupportedOutputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats)
	*/	
	virtual void SupportedOutputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOutputImageFrameFormatsL(RArray<TUid>& aFormats
	
	@see CImgProcessor::SupportedOutputImageFrameFormatsL(RArray<TUid>& aFormats
	*/	
	virtual void SupportedOutputImageFrameFormatsL(RArray<TUid>& aFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOutputDisplayModesL((RArray<TDisplayMode>& aDisplayModes)
	
	@see CImgProcessor::SupportedOutputDisplayModesL((RArray<TDisplayMode>& aDisplayModes)
	*/	
	virtual void SupportedOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const = 0;
		
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedPreviewOutputImageFrameFormatsL(RArray<TUid>& aFormats)
	
	@see CImgProcessor::SupportedPreviewOutputImageFrameFormatsL(RArray<TUid>& aFormats)
	*/	
	virtual void SupportedPreviewOutputImageFrameFormatsL(RArray<TUid>& aFormats) const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedPreviewOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes)
	
	@see CImgProcessor::SupportedPreviewOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes)
	*/	
	virtual void SupportedPreviewOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOverlayInputFormatsL(RArray<TUid>& aFormats)
	
	@see CImgProcessor::SupportedOverlayInputFormatsL(RArray<TUid>& aFormats)
	*/	
	virtual void SupportedOverlayInputFormatsL(RArray<TUid>& aFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOverlayInputImageFrameFormatsL(RArray<TUid>& aFormats)
	
	@see CImgProcessor::SupportedOverlayInputImageFrameFormatsL(RArray<TUid>& aFormats)
	*/	
	virtual void SupportedOverlayInputImageFrameFormatsL(RArray<TUid>& aFormats) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOverlayInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes)
	
	@see CImgProcessor::SupportedOverlayInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes)
	*/	
	virtual void SupportedOverlayInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOptions()
	
	@see CImgProcessor::SupportedOptions()
	*/	
	virtual TUint64 SupportedOptions() const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SupportedOperations()
	
	@see CImgProcessor::SupportedOperations()
	*/	
	virtual TUint SupportedOperations() const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CurrentSizeL()
	
	@see CImgProcessor::CurrentSizeL()
	*/	
	virtual TSize CurrentSizeL() const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::BackgroundColorL()
	
	@see CImgProcessor::BackgroundColorL()
	*/	
	virtual TRgb BackgroundColorL() const = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CalculatePixelBufferSizeL(TSize aSizeInPixels, TDisplayMode aDisplayMode, TUint32 aScanLineLength)
	
	@see CImgProcessor::CalculatePixelBufferSizeL(TSize aSizeInPixels, TDisplayMode aDisplayMode, TUint32 aScanLineLength)
	*/	
	virtual TInt CalculatePixelBufferSizeL(TSize aSizeInPixels, TDisplayMode aDisplayMode, TUint32 aScanLineLength) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CalculatePixelBufferSizeL(TSize aSizeInPixels, const TUid& aFormat, TUint32 aScanLineLength = 0)
	
	@see CImgProcessor::CalculatePixelBufferSizeL(TSize aSizeInPixels, const TUid& aFormat, TUint32 aScanLineLength = 0)
	*/	
	virtual TInt CalculatePixelBufferSizeL(TSize aSizeInPixels, const TUid& aFormat, TUint32 aScanLineLength = 0) const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CreateInputL(CFbsBitmap& aBitmap)
	
	@see CImgProcessor::CreateInputL(CFbsBitmap& aBitmap)
	*/	
	virtual void CreateInputL(CFbsBitmap& aBitmap) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CreateInputL(CImageFrame& aPixelBuffer)
	
	@see CImgProcessor::CreateInputL(CImageFrame& aPixelBuffer)
	*/	
	virtual void CreateInputL(CImageFrame& aPixelBuffer) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CreateInputL(const TSize& aSize, const TRgb& aColor)
	
	@see CImgProcessor::CreateInputL(const TSize& aSize, const TRgb& aColor)
	*/	
	virtual void CreateInputL(const TSize& aSize, const TRgb& aColor) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetBackgroundColorL(const TRgb& aColor)
	
	@see CImgProcessor::SetBackgroundColorL(const TRgb& aColor)
	*/	
	virtual void SetBackgroundColorL(const TRgb& aColor) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputRectL(const TRect& aRect)
	
	@see CImgProcessor::SetInputRectL(const TRect& aRect)
	*/	
	virtual void SetInputRectL(const TRect& aRect) = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat) = 0;
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask)
	
	@see CImgProcessor::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask)
	*/	
	virtual void SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(const CImageFrame& aPixelBuffer)
	
	@see CImgProcessor::SetInputL(const CImageFrame& aPixelBuffer)
	*/	
	virtual void SetInputL(const CImageFrame& aPixelBuffer) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetOptionsL(TUint64 aOptions)
	
	@see CImgProcessor::SetOptionsL(TUint64 aOptions)
	*/	
	virtual void SetOptionsL(TUint64 aOptions) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::ApplyOperationL(CImgProcessor::TOperation aOperation)
	
	@see CImgProcessor::ApplyOperationL(CImgProcessor::TOperation aOperation)
	*/	
	virtual void ApplyOperationL(CImgProcessor::TOperation aOperation) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::InputUpdatedL()
	
	@see CImgProcessor::InputUpdatedL()
	*/	
	virtual void InputUpdatedL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::ResetL()
	
	@see CImgProcessor::ResetL()
	*/	
	virtual void ResetL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::InitializeL()
	
	@see CImgProcessor::InitializeL()
	*/	
	virtual void InitializeL() = 0;
   
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::ProcessL(const TSize& aSize, TBool aMaintainAspectRatio)
	
	@see CImgProcessor::ProcessL(const TSize& aSize, TBool aMaintainAspectRatio)
	*/	
	virtual void ProcessL(const TSize& aSize, TBool aMaintainAspectRatio) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::Cancel()
	
	@see CImgProcessor::Cancel()
	*/	
	virtual void Cancel() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CanUndoL()
	
	@see CImgProcessor::CanUndoL()
	*/	
	virtual TBool CanUndoL() const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::UndoL()
	
	@see CImgProcessor::UndoL()
	*/	
	virtual void UndoL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::UndoAllL()
	
	@see CImgProcessor::UndoAllL()
	*/	
	virtual void UndoAllL() = 0;
	
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::CanRedoL()
	
	@see CImgProcessor::CanRedoL()
	*/	
	virtual TBool CanRedoL() const = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::RedoL()
	
	@see CImgProcessor::RedoL()
	*/	
	virtual void RedoL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::RedoAllL()
	
	@see CImgProcessor::RedoAllL()
	*/	
	virtual void RedoAllL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetOutputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetOutputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetOutputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetOutputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetOutputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetOutputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetOutputL(RBuf8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	
	@see CImgProcessor::SetOutputL(RBuf8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	*/	
	virtual void SetOutputL(RBuf8& aBuffer, const TUid& aFormat, const TUid& aSubFormat) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetOutputL(CImageFrame& aPixelBuffer)
	
	@see CImgProcessor::SetOutputL(CImageFrame& aPixelBuffer)
	*/	
	virtual void SetOutputL(CImageFrame& aPixelBuffer) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetOutputL(CFbsBitmap& aBitmap, CFbsBitmap* aMask)
	
	@see CImgProcessor::SetOutputL(CFbsBitmap& aBitmap, CFbsBitmap* aMask)
	*/	
	virtual void SetOutputL(CFbsBitmap& aBitmap, CFbsBitmap* aMask) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::Extension(TUid aExtension)
	
	@see CImgProcessor::Extension(TUid aExtension)
	*/	
	virtual TAny* Extension(TUid aExtension) = 0;

   	//Ownership is not transferred
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::PreviewL(TInt aPreviewId)
	
	@see CImgProcessor::PreviewL(TInt aPreviewId)
	*/	
	virtual Plugin::MPreview* PreviewL(TInt aPreviewId) = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::OverlayL()
	
	@see CImgProcessor::OverlayL()
	*/	
	virtual Plugin::MOverlay* OverlayL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::ProgressInfoL()
	
	@see CImgProcessor::ProgressInfoL()
	*/	
	virtual Plugin::MProgressInfo* ProgressInfoL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::InputInfoL()
	
	@see CImgProcessor::InputInfoL()
	*/	
	virtual Plugin::MInputInfo* InputInfoL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::OutpputInfoL()
	
	@see CImgProcessor::OutputInfoL()
	*/	
	virtual Plugin::MOutputInfo* OutputInfoL() = 0;

	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::EffectL(TUid aEffectId)
	
	@see CImgProcessor::EffectL(TUid aEffectId)
	*/	
   	virtual Plugin::MEffect* EffectL(TUid aEffectId) = 0;
   	
	/**
	A derived class must provide an implementation of this method to destroy plugin and release all allocated memory
	*/	
	virtual void Destroy() = 0;

	/**
	A derived class must provide an implementation of this method to clean up unauthorized output.
	*/	
	virtual void CleanupOutput() = 0;

	/**
	A derived class must provide an implementation of this method to clean up effects.
	*/	
	virtual void CleanupEffects() = 0;
   	};
   	
   	/**
Mixin class for implementation by providers of extended functionality for Image Processor Plugin.
The class provides a function to set an panoramic image as input to Image Processor. The implementation of 
the class is provided through MImgProcessorPlugin::Extension(TUid aExtension).
@see	CImgProcessor
@see	MImgProcessorPlugin::Extension(TUid aExtension)

@publishedPartner
@released
*/	
class MImgProcessorPluginPanoramaInput
	{
public:
	/**
	A derived class must provide an implementation of this method as defined by CImgProcessor::SetInputL(CImagePanorama& aPanorama)
	
	@see CImgProcessor::SetInputL(CImagePanorama& aPanorama)
	*/	
	virtual void SetInputL(CImagePanorama& aPanorama) = 0;
	};	

	}//	namespace ImageProcessor

#endif //__IMAGE_PROCESSOR_PLUGIN_H__
