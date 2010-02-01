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

#ifndef __IMAGE_PROCESSOR_IMPL_H__
#define __IMAGE_PROCESSOR_IMPL_H__

#include <imageconversion.h>

#include <imageprocessor/imageprocessor.h>
#include <imageprocessor/imageprocessorpluginobserver.h>

namespace ImageProcessor
	{
	class CImgProcessor;
	class MImgProcessorPlugin;
	class MImgProcessorObserver;
	class CImageProcessorCallback;
	class TEffect;
	class TPreview;
	class TOverlay;
	class TProgressInfo;
	class TInputInfo;
	class TOutputInfo;
	namespace Plugin
		{
		class MEffect;
		class MPreview;
		class MOverlay;
		class MProgressInfo;
		class MInputInfo;
		class MOutputInfo;
		}

NONSHARABLE_CLASS(CImageProcessorDrmInput) : public CBase  
	{
public:
	static CImageProcessorDrmInput* NewL(TMMSource& aDrmFile);
		
	public:
		void ExecuteIntentL();
		~CImageProcessorDrmInput();
		
		inline HBufC8* Buffer();
	
	private:
		void ConstructL(TMMSource& aDrmFile);
		
	private:
		HBufC8* iDecryptedBuffer;
		ContentAccess::CContent* iContent;
		ContentAccess::CData* iData;
		ContentAccess::TIntent iIntent;
	};

inline HBufC8* CImageProcessorDrmInput::Buffer()
	{
	return iDecryptedBuffer;
	}

NONSHARABLE_CLASS(CImageProcessorImpl) : public CBase, public MImgProcessorPluginObserver
   {
private:
	static const TBool iValidStates[CImgProcessor::EStatesCount][CImgProcessor::EStatesCount];

public:
	static CImageProcessorImpl* NewL(RFs& aFileServerSession,CImgProcessor& aImageProcessor,MImgProcessorObserver& aObserver,TUid aPluginUid);
	void Cleanup();
	
	void SupportedEffectsL(RArray<TUid>& aEffects) const;
	
	void SupportedInputFormatsL(RArray<TUid>& aFormats) const;
	void SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const;
	void SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats) const;
	void SupportedInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const;
	
	void SupportedOutputFormatsL(RArray<TUid>& aFormats) const;
	void SupportedOutputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const;
	void SupportedOutputImageFrameFormatsL(RArray<TUid>& aFormats) const;
	void SupportedOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const;
	
	TUint64 SupportedOptions() const;
	TUint SupportedOperations() const;

	CImgProcessor::TState State() const;
	TSize CurrentSizeL() const;
	TRgb BackgroundColorL() const;
	
	TInt CalculatePixelBufferSizeL(TSize aSizeInPixels, TDisplayMode aDisplayMode, TUint32 aScanLineLength = 0) const;
	TInt CalculatePixelBufferSizeL(TSize aSizeInPixels, const TUid& aFormat, TUint32 aScanLineLength = 0) const;

	void CreateInputL(CFbsBitmap& aBitmap);
	void CreateInputL(CImageFrame& aPixelBuffer);
	void CreateInputL(const TSize& aSize, const TRgb& aColor);

	void SetBackgroundColorL(const TRgb& aColor);
	
	void SetInputRectL(const TRect& aRect);
	
	void SetInputL(const TDesC& aFilename, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetInputL(RFile& aFile, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetInputL(TMMSource& aDrmFile, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetInputL(const TDesC8& aBuffer, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask = NULL);
	void SetInputL(const CImageFrame& aPixelBuffer);
	void SetInputL(CImagePanorama& aPanorama);


	void SetOptionsL(TUint64 aOptions);
	void ApplyOperationL(CImgProcessor::TOperation aOperation);

	void InputUpdatedL();
	void ResetL();

	void InitializeL(TUint64 aOptions);
   
	void ProcessL(const TSize& aSize, TBool aMaintainAspectRatio);
	void Cancel();

	TEffect* EffectL(TUid aEffect);

	TBool CanUndoL() const;
	void UndoL();
	void UndoAllL();
	
	TBool CanRedoL() const;
	void RedoL();
	void RedoAllL();

	TPreview* PreviewL(TInt aPreviewId);

	TOverlay* OverlayL();

	TProgressInfo* ProgressInfoL();
	TInputInfo* InputInfoL();
	TOutputInfo* OutputInfoL();

	void SetOutputL(const TDesC& aFilename, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetOutputL(RFile& aFile, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetOutputL(RBuf8& aBuffer, const TUid& aFormat = KNullUid, const TUid& aSubFormat = KNullUid);
	void SetOutputL(CImageFrame& aPixelBuffer);
	void SetOutputL(CFbsBitmap& aBitmap, CFbsBitmap* aMask = NULL);

	TAny* Extension(TUid aExtension);
   
	// framework utility functions
	void ConvertMimeTypeToUidL(const TDesC8& aMimeType, TUid& aFormat, TUid& aSubFormat);
	void ConvertFileExtensionToUidL(const TDesC& aFileExtension, TUid& aFormat, TUid& aSubFormat);
	void ConvertUidToMimeTypeL(TDes8& aMimeType, const TUid& aFormat, const TUid& aSubFormat);
	void ConvertUidToFileExtensionL(TDes& aFileExtension, const TUid& aFormat, const TUid& aSubFormat);
	
	~CImageProcessorImpl();
      
	void PostEvent(TInt aEventId, TUid aUid, TInt aId, TInt aError);
	void ProcessEventL(TInt aEventId, TUid aUid, TInt aId, TInt aError);
	void SetStateL(CImgProcessor::TState aState);
	void RestoreStateL();

	TBool IsBusy(CImgProcessor::TState aState) const;
	void CheckStateL(CImgProcessor::TState aState) const;
	
	void AppendOverlayDrmInputL();

	inline RArray<TUid>& PreviewOutputImageFrameFormatsL();
	inline RArray<TDisplayMode>& PreviewOutputDisplayModesL();
	inline RArray<TUid>& OverlayInputFormatsL();
	inline RArray<TUid>& OverlayInputImageFrameFormatsL();
	inline RArray<TDisplayMode>& OverlayInputDisplayModesL();
	
	inline void SetOverlayDrmInput(CImageProcessorDrmInput* aOverlayDrmInput);
	inline void ResetOverlayDrmInput();

	inline TBool IsInputSet() const;
	inline TBool IsOutputSet() const;
	inline TUint64 Options() const;
   
private:
	CImageProcessorImpl(RFs& aFileServerSession, CImgProcessor& aImageProcessor, TUid aPluginUid);
	void ConstructL(MImgProcessorObserver& aObserver);

	void CheckInputFormatL(const TUid& aFormat, const TUid& aSubFormat);
	void CheckOutputFormatL(const TUid& aFormat, const TUid& aSubFormat);
	TBool IsFrameworkEffect(TUid aEffect) const;
	TBool IsPluginEffect(TUid aEffect) const;
	void InitializeFrameworkEffectsL();
   
	TEffect* FindEffect(TUid aEffect) const;    
	TEffect* CreateEffectL(TUid aEffect); 
	TEffect* EffectFactoryL(TUid aEffect, Plugin::MEffect* aPluginEffect);
	
	TPreview* FindPreview(TInt aPreviewId) const;
   
private:
	RFs& iFileServerSession;
	CImgProcessor& iImageProcessor;
	TUid iPluginUid;

	CImgProcessor::TState iState;
	CImgProcessor::TState iPreviousState;
   
	CImageProcessorCallback* iCallbackHandler;
   
	MImgProcessorPlugin* iPlugin;
	TUid iPluginDtorKey;

	TOverlay* iOverlay;
	TProgressInfo* iProgressInfo;
	TInputInfo* iInputInfo;
	TOutputInfo* iOutputInfo;
	   
	CImageProcessorDrmInput* iDrmInput;
	CImageProcessorDrmInput* iOverlayDrmInput;

	RPointerArray<TEffect> iEffects;
	RPointerArray<TPreview> iPreviews;
	RPointerArray<CImageProcessorDrmInput> iOverlayDrmInputs;// to keep all decrypted buffer during session

	RArray<TUid> iFrameworkEffects;
	RArray<TUid> iPluginEffects;
	RArray<TUid> iInputFormats;
	RArray<TUid> iInputImageFrameFormats;
	RArray<TDisplayMode> iInputDisplayModes;
	RArray<TUid> iOutputFormats;
	RArray<TUid> iOutputImageFrameFormats;
	RArray<TDisplayMode> iOutputDisplayModes;
	
	RArray<TUid> iPreviewOutputImageFrameFormats;
	RArray<TDisplayMode> iPreviewOutputDisplayModes;
	RArray<TUid> iOverlayInputFormats;
	RArray<TUid> iOverlayInputImageFrameFormats;
	RArray<TDisplayMode> iOverlayInputDisplayModes;

	RFileExtensionMIMETypeArray iTypesUtilityArray;

	TUint64 iSupportedOptions;
	TUint64 iOptions;
	TUint iSupportedOperations;
	
	TBool iIsInputSet;
	TBool iIsOutputSet;
	};

inline RArray<TUid>& CImageProcessorImpl::PreviewOutputImageFrameFormatsL()
	{
	return iPreviewOutputImageFrameFormats;
	}

inline RArray<TDisplayMode>& CImageProcessorImpl::PreviewOutputDisplayModesL()
	{
	return iPreviewOutputDisplayModes;
	}

inline RArray<TUid>& CImageProcessorImpl::OverlayInputFormatsL()
	{
	return iOverlayInputFormats;
	}

inline RArray<TUid>& CImageProcessorImpl::OverlayInputImageFrameFormatsL()
	{
	return iOverlayInputImageFrameFormats;
	}

inline RArray<TDisplayMode>& CImageProcessorImpl::OverlayInputDisplayModesL()
	{
	return iOverlayInputDisplayModes;
	}

inline TBool CImageProcessorImpl::IsInputSet() const
	{
	return iIsInputSet;
	}

inline TBool CImageProcessorImpl::IsOutputSet() const
	{
	return iIsOutputSet;
	}

inline TUint64 CImageProcessorImpl::Options() const
	{
	return iOptions;
	}

inline void CImageProcessorImpl::SetOverlayDrmInput(CImageProcessorDrmInput* aOverlayDrmInput)
	{
	ResetOverlayDrmInput();
	iOverlayDrmInput = aOverlayDrmInput;
	}

inline void CImageProcessorImpl::ResetOverlayDrmInput() 
	{
	delete iOverlayDrmInput;
	iOverlayDrmInput = NULL;
	}

	} // ImageProcessor
		
#endif //__IMAGE_PROCESSOR_IMPL_H__
