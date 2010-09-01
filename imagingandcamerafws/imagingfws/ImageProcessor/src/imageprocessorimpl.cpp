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

#include <e32std.h>
#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
#include <caf/caf.h>
#include <caf/content.h>
#include <caf/data.h>
#include <mm/mmcleanup.h>
#include <mm/mmpluginutils.h>
#include <imageframe.h>
#include <imageprocessor/imageprocessor_uids.hrh>
#include "imageprocessorimpl.h"
#include <imageprocessor/imageprocessorobserver.h>
#include <imageprocessor/imageprocessorplugin.h>
#include <imageprocessor/imageprocessoreffect.h>
#include "imageprocessorcallback.h"
#include <imageprocessor/imageprocessorpreview.h>
#include <imageprocessor/imageprocessorpreviewintf.h>
#include <imageprocessor/imageprocessoroverlay.h>
#include <imageprocessor/imageprocessoroverlayintf.h>
#include <imageprocessor/imageprocessorprogressinfo.h>
#include <imageprocessor/imageprocessorprogressinfointf.h>
#include <imageprocessor/imageprocessorinputinfo.h>
#include <imageprocessor/imageprocessorinputinfointf.h>
#include <imageprocessor/imageprocessoroutputinfo.h>
#include <imageprocessor/imageprocessoroutputinfointf.h>
#include "imageprocessormain.h"

using namespace ImageProcessor;

// current state = row
// new state = column
const TBool CImageProcessorImpl::iValidStates[CImgProcessor::EStatesCount][CImgProcessor::EStatesCount] = 
	{
	
		//	EUninitialized	EInitializing	EInitialized	EEffectActive	EProcessing	PInit	PRend
		{	
			ETrue, 			ETrue, 			EFalse, 		EFalse, 		EFalse,		EFalse,	EFalse	//EUninitialized
		},		
		{	
			ETrue, 			ETrue, 			ETrue, 			EFalse,			EFalse,		EFalse,	EFalse	//EInitializing
		},
		{	
			ETrue,	 		EFalse,			ETrue, 			ETrue, 			ETrue,		ETrue,	ETrue	//EInitialized
		},		
		{	
			EFalse, 		EFalse,			ETrue,			ETrue, 			ETrue,		ETrue,	ETrue	//EEffectActive
		},
		{	
			EFalse, 		EFalse,			ETrue,			ETrue,			ETrue,		EFalse,	EFalse	//EProcessing
		},
		{	
			EFalse, 		EFalse,			ETrue,			ETrue,			EFalse,		ETrue,	EFalse	//EPreviewInitializing
		},
		{	
			EFalse, 		EFalse,			ETrue,			ETrue,			EFalse,		EFalse,	ETrue	//EPreviewRendering
		}
	};

CImageProcessorImpl* CImageProcessorImpl::NewL(RFs& aFileServerSession, CImgProcessor& aImageProcessor, MImgProcessorObserver& aObserver, TUid aPluginUid)
	{
	CImageProcessorImpl* self = new (ELeave) CImageProcessorImpl(aFileServerSession, aImageProcessor, aPluginUid);
	CleanupStack::PushL(self);
	self->ConstructL(aObserver);
	CleanupStack::Pop(self);
	return self;
	}

CImageProcessorImpl::CImageProcessorImpl(RFs& aFileServerSession, CImgProcessor& aImageProcessor, TUid aPluginUid):
	iFileServerSession(aFileServerSession),
	iImageProcessor(aImageProcessor),
	iPluginUid(aPluginUid)
	{
	}
   
void CImageProcessorImpl::ConstructL(MImgProcessorObserver& aObserver)
	{
	iCallbackHandler = CImageProcessorCallback::NewL(iImageProcessor,aObserver);
	}

void CImageProcessorImpl::Cleanup()
	{
	Cancel();
	
	TInt err = KErrNone;
	
	if (iOverlay) 
		{
		TRAP(err, iOverlay->ResetL());
		}
	
	for (TInt i=0; i < iEffects.Count(); i++ )
		{ 
		TRAP(err, iEffects[i]->ResetL());
		}
	
	for (TInt i=0; i < iPreviews.Count(); i++ )
		{ 
		TRAP(err, iPreviews[i]->ResetL());
		TRAP(err, iPreviews[i]->UninitializeL());
		}
	
	if (iPlugin)
		{
		TRAP(err, iPlugin->ResetL());
		}

	delete iCallbackHandler;
	iCallbackHandler = NULL;
	
	delete iOverlay;
	iOverlay = NULL;

	delete iProgressInfo;
	iProgressInfo = NULL;
	
	delete iInputInfo;
	iInputInfo = NULL;

	delete iOutputInfo;
	iOutputInfo = NULL;

	delete iDrmInput;
	iDrmInput = NULL;
	
	delete iOverlayDrmInput;
	iOverlayDrmInput = NULL;
	
	
	iEffects.ResetAndDestroy();
	iPreviews.ResetAndDestroy();
	iOverlayDrmInputs.ResetAndDestroy();
	iFrameworkEffects.Close();
	iPluginEffects.Close();
	iInputFormats.Close();
	iInputImageFrameFormats.Close();
	iInputDisplayModes.Close();
	iOutputFormats.Close();
	iOutputImageFrameFormats.Close();
	iOutputDisplayModes.Close();
	iPreviewOutputImageFrameFormats.Close();
	iPreviewOutputDisplayModes.Close();
	iOverlayInputFormats.Close();
	iOverlayInputImageFrameFormats.Close();
	iOverlayInputDisplayModes.Close();
	iTypesUtilityArray.ResetAndDestroy();
	
	if(iPlugin)
		{
		iPlugin->Destroy();
		iPlugin = NULL;
		REComSession::DestroyedImplementation(iPluginDtorKey);      
		}

	iState = CImgProcessor::EUninitialized;
	iPreviousState = CImgProcessor::EUninitialized;
	
	iSupportedOptions = CImgProcessor::EOptionNone;
	iOptions = CImgProcessor::EOptionNone;
	iSupportedOperations = CImgProcessor::EOperationNone;
	
	iIsInputSet = EFalse;
	iIsOutputSet = EFalse;
	}

CImageProcessorImpl::~CImageProcessorImpl()
	{
	Cleanup();
	}

void CImageProcessorImpl::SupportedEffectsL(RArray<TUid>& aEffects) const
	{
	aEffects = iPluginEffects;
	}

void CImageProcessorImpl::SupportedInputFormatsL(RArray<TUid>& aFormats) const
	{
	aFormats = iInputFormats;
	}

void CImageProcessorImpl::SupportedInputSubFormatsL(TUid aFormats, RArray<TUid>& aSubFormats) const
	{
	iPlugin->SupportedInputSubFormatsL(aFormats, aSubFormats);
	}

void CImageProcessorImpl::SupportedInputImageFrameFormatsL(RArray<TUid>& aFormats) const
	{
	aFormats = iInputImageFrameFormats;
	}

void CImageProcessorImpl::SupportedInputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const
	{
	aDisplayModes = iInputDisplayModes;
	}

void CImageProcessorImpl::SupportedOutputFormatsL(RArray<TUid>& aFormats) const
	{
	aFormats = iOutputFormats;
	}

void CImageProcessorImpl::SupportedOutputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const
	{
	iPlugin->SupportedOutputSubFormatsL(aFormat, aSubFormats);
	}

void CImageProcessorImpl::SupportedOutputImageFrameFormatsL(RArray<TUid>& aFormats) const
	{
	aFormats = iOutputImageFrameFormats;
	}

void CImageProcessorImpl::SupportedOutputDisplayModesL(RArray<TDisplayMode>& aDisplayModes) const
	{
	aDisplayModes = iOutputDisplayModes;
	}

TUint64 CImageProcessorImpl::SupportedOptions() const
	{
	return iSupportedOptions;
	}

TUint CImageProcessorImpl::SupportedOperations() const
	{	
	return iSupportedOperations;
	}
   
void CImageProcessorImpl::SetOptionsL(TUint64 aOptions)
	{
	if ((aOptions | iSupportedOptions) != iSupportedOptions) 
		{
		User::Leave(KErrNotSupported);
		}
	CheckStateL(CImgProcessor::EInitialized);
	iPlugin->SetOptionsL(aOptions);
	iOptions = aOptions;
	}

void CImageProcessorImpl::ApplyOperationL(CImgProcessor::TOperation aOperation)
	{
	if ((aOperation | iSupportedOperations) != iSupportedOperations) 
		{
		User::Leave(KErrNotSupported);
		}
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->ApplyOperationL(aOperation);
	}

void CImageProcessorImpl::CheckStateL(CImgProcessor::TState aState) const
	{
	if (iState != aState) 
		{
		User::Leave(KErrNotReady);
		}
	}

CImgProcessor::TState CImageProcessorImpl::State() const
	{
	return iState;
	}
   
void CImageProcessorImpl::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckInputFormatL(aFormat, aSubFormat);
	iPlugin->SetInputL(aFilename, aFormat, aSubFormat);

	delete iDrmInput;
	iDrmInput = NULL;
	
	iIsInputSet = ETrue;
	}
   
void CImageProcessorImpl::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckInputFormatL(aFormat, aSubFormat);
	iPlugin->SetInputL(aFile, aFormat, aSubFormat);

	delete iDrmInput;
	iDrmInput = NULL;

	iIsInputSet = ETrue;
	}

void CImageProcessorImpl::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckInputFormatL(aFormat, aSubFormat);
	
	CImageProcessorDrmInput* drmInput = CImageProcessorDrmInput::NewL(aDrmFile);
	
	CleanupStack::PushL(drmInput);
	SetInputL(*(drmInput->Buffer()), aFormat, aSubFormat);
	CleanupStack::Pop();

	iDrmInput = drmInput;
	}
   
void CImageProcessorImpl::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckInputFormatL(aFormat, aSubFormat);
	
	if (aBuffer.Size() == 0) 
		{
		User::Leave(KErrArgument);
		}

	iPlugin->SetInputL(aBuffer, aFormat, aSubFormat);

	delete iDrmInput;
	iDrmInput = NULL;

	iIsInputSet = ETrue;
	}
   
void CImageProcessorImpl::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	
	if (aBitmap.SizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	if((aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid) || (aMask!=NULL && aMask->Handle()!=0 && aMask->ExtendedBitmapType()!=KNullUid))
        {
        User::Leave(KErrNotSupported);
        }
	
	if ((aMask != NULL) && (aMask->SizeInPixels() == TSize(0,0))) 
		{
		User::Leave(KErrArgument);
		}

	iPlugin->SetInputL(aBitmap, aMask);

	delete iDrmInput;
	iDrmInput = NULL;

	iIsInputSet = ETrue;
	}
   
void CImageProcessorImpl::SetInputL(const CImageFrame& aPixelBuffer)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	
	if (aPixelBuffer.FrameSizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	iPlugin->SetInputL(aPixelBuffer);

	delete iDrmInput;
	iDrmInput = NULL;

	iIsInputSet = ETrue;
	}

void CImageProcessorImpl::SetInputL(CImagePanorama& aPanorama)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	
	//get the extenstion interface to panorama image as input
	TAny* any = iPlugin->Extension(TUid::Uid(KImageProcessorExtensionPanoramaInputUidValue));
	MImgProcessorPluginPanoramaInput* extension = static_cast<MImgProcessorPluginPanoramaInput*>(any);
	if(extension == NULL)
		{
		User::Leave(KErrNotSupported);
		}
	extension->SetInputL(aPanorama);
	
	delete iDrmInput;
	iDrmInput = NULL;

	iIsInputSet = ETrue;
	}

void CImageProcessorImpl::SetInputRectL(const TRect& aRect)
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	
	if (aRect == TRect(0,0,0,0) || aRect.Size() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	TSize size = iPlugin->CurrentSizeL();

	if (!TRect(size).Intersects(aRect)) 
		{
		User::Leave(KErrArgument);
		}

	iPlugin->SetInputRectL(aRect);
	}

TSize CImageProcessorImpl::CurrentSizeL() const
	{
	__ASSERT_ALWAYS(iIsInputSet, User::Leave(KErrNotReady));
	return iPlugin->CurrentSizeL();
	}

TInt CImageProcessorImpl::CalculatePixelBufferSizeL(TSize aSizeInPixels, TDisplayMode aDisplayMode, TUint32 aScanLineLength) const
	{
	__ASSERT_ALWAYS((iState != CImgProcessor::EUninitialized), User::Leave(KErrNotReady));
	return iPlugin->CalculatePixelBufferSizeL(aSizeInPixels, aDisplayMode, aScanLineLength);
	}

TInt CImageProcessorImpl::CalculatePixelBufferSizeL(TSize aSizeInPixels, const TUid& aFormat, TUint32 aScanLineLength) const
	{
	__ASSERT_ALWAYS((iState != CImgProcessor::EUninitialized), User::Leave(KErrNotReady));
	return iPlugin->CalculatePixelBufferSizeL(aSizeInPixels, aFormat, aScanLineLength);
	}

void CImageProcessorImpl::CreateInputL(CFbsBitmap& aBitmap)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	
	if (aBitmap.SizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
        {
        User::Leave(KErrNotSupported);
        }

	iPlugin->CreateInputL(aBitmap);
	
	delete iDrmInput;
	iDrmInput = NULL;
	
	iIsInputSet = ETrue;
	}

void CImageProcessorImpl::CreateInputL(CImageFrame& aPixelBuffer)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));

	if (aPixelBuffer.FrameSizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	iPlugin->CreateInputL(aPixelBuffer);
	
	delete iDrmInput;
	iDrmInput = NULL;
	
	iIsInputSet = ETrue;
	}

void CImageProcessorImpl::CreateInputL(const TSize& aSize, const TRgb& aColor)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	
	if (aSize == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	iPlugin->CreateInputL(aSize, aColor);
	
	delete iDrmInput;
	iDrmInput = NULL;
	
	iIsInputSet = ETrue;
	}

void CImageProcessorImpl::InputUpdatedL()
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->InputUpdatedL();
	}

void CImageProcessorImpl::ResetL() 
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));

	Cancel();
	
	if (iOverlay) 
		{
		iOverlay->ResetL();
		}
	
	delete iOverlayDrmInput;
	iOverlayDrmInput = NULL;
	
	for (TInt i=0; i < iEffects.Count(); i++ )
		{ 
		iEffects[i]->ResetL();
		}
	
	for (TInt i=0; i < iPreviews.Count(); i++ )
		{ 
		iPreviews[i]->ResetL();
		}
	
	iPlugin->ResetL();
	
	delete iDrmInput;
	iDrmInput = NULL;

	delete iProgressInfo;
	iProgressInfo = NULL;
	
	delete iInputInfo;
	iInputInfo = NULL;
	
	delete iOutputInfo;
	iOutputInfo = NULL;
	
	iPreviews.ResetAndDestroy();
	iOverlayDrmInputs.ResetAndDestroy();

	iIsInputSet = EFalse;
	iIsOutputSet = EFalse;
	
	iOptions = CImgProcessor::EOptionNone;
	}
   
void CImageProcessorImpl::InitializeL(TUint64 aOptions)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EUninitialized), User::Leave(KErrNotReady));
	
	// Client whishes to choose the plugin given by framework.
	if(iPluginUid == KNullUid)
		{
		TUid interfaceUid = {KImageProcessorPluginInterfaceUidValue};
		// Get the higher version of plugin
		iPlugin = static_cast<MImgProcessorPlugin*>(MmPluginUtils::CreateImplementationL(interfaceUid, iPluginDtorKey));   
		}
	// If client has passed the plugin uid.	
	else
		{
		iPlugin = static_cast<MImgProcessorPlugin*>(REComSession::CreateImplementationL(iPluginUid, iPluginDtorKey));   
		}	
		
	iPlugin->SetFileServerSession(iFileServerSession);
	iPlugin->SetObserver(*this);
	iPlugin->SupportedEffectsL(iPluginEffects);
	iPlugin->SupportedInputFormatsL(iInputFormats);
	iPlugin->SupportedInputImageFrameFormatsL(iInputImageFrameFormats);
	iPlugin->SupportedInputDisplayModesL(iInputDisplayModes);
	iPlugin->SupportedOutputFormatsL(iOutputFormats);
	iPlugin->SupportedOutputImageFrameFormatsL(iOutputImageFrameFormats);
	iPlugin->SupportedOutputDisplayModesL(iOutputDisplayModes);
	
	iSupportedOptions = iPlugin->SupportedOptions();
	if ((aOptions | iSupportedOptions) != iSupportedOptions) 
		{
		User::Leave(KErrNotSupported);
		}
	iPlugin->SetOptionsL(aOptions);
	iOptions = aOptions;
	
	iSupportedOperations = iPlugin->SupportedOperations();

	iPlugin->SupportedPreviewOutputImageFrameFormatsL(iPreviewOutputImageFrameFormats);
	iPlugin->SupportedPreviewOutputDisplayModesL(iPreviewOutputDisplayModes);
	iPlugin->SupportedOverlayInputFormatsL(iOverlayInputFormats);
	iPlugin->SupportedOverlayInputImageFrameFormatsL(iOverlayInputImageFrameFormats);
	iPlugin->SupportedOverlayInputDisplayModesL(iOverlayInputDisplayModes);

	CImageDecoder::GetFileTypesL(iTypesUtilityArray);	

	InitializeFrameworkEffectsL();
	
	SetStateL(CImgProcessor::EInitializing);

	iPlugin->InitializeL();

	if ((iOptions & CImgProcessor::EOptionSyncProcessing) != 0) 
		{
		SetStateL(CImgProcessor::EInitialized);
		}
	}

void CImageProcessorImpl::InitializeFrameworkEffectsL() 
	{
	//PhotoArt
	iFrameworkEffects.AppendL(ImageProcessor::KEffectSepiaUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectGrayscaleUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectNegativeUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectGrayscaleNegativeUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectOilyUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectPaintUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectMilkyUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectFogUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectMirrorLeftToRightUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectMagicPenUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectAntiqueUid);
	
	iFrameworkEffects.AppendL(ImageProcessor::KEffectBrightnessUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectSharpnessUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectRotationUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectContrastUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectRgbColorAdjustUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectEmbossUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectSolarizeUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectPosterizeUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectStampUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectSketchUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectNoiseUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectMoonlightUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectFrostedGlassUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectDespeckleUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectBlurUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectColorizationUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectCartoonUid);
		
	//ClearShot
	iFrameworkEffects.AppendL(ImageProcessor::KEffectLocalBoostUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectColorBoostUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectWhiteBalanceUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectAutoLevelsUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectLevelsUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectExposureUid);

	iFrameworkEffects.AppendL(ImageProcessor::KEffectSpotLightUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectCustomTransformationUid);
	
	//Others
	iFrameworkEffects.AppendL(ImageProcessor::KGeometricalOperationUid);
	iFrameworkEffects.AppendL(ImageProcessor::KEffectBorderUid);
	}
      
void CImageProcessorImpl::ProcessL(const TSize& aSize, TBool aMaintainAspectRatio)
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet) && (iIsOutputSet)), User::Leave(KErrNotReady));
	SetStateL(CImgProcessor::EProcessing);

	TRAPD(err, iPlugin->ProcessL(aSize, aMaintainAspectRatio));

	if (err != KErrNone) 
		{
		RestoreStateL();
		User::Leave(err);
		}

	if ((iOptions & CImgProcessor::EOptionSyncProcessing) != 0) 
		{
		if (iDrmInput) 
			{
			TRAPD(drmErr, iDrmInput->ExecuteIntentL());
			if (drmErr != KErrNone) 
				{
				iPlugin->CleanupOutput(); //should delete/cleanup unauthorised output
				User::Leave(drmErr);
				}
			}
		
		for (TInt i=0; i < iOverlayDrmInputs.Count(); i++ )
			{ 
			TRAPD(overlayDrmErr, iOverlayDrmInputs[i]->ExecuteIntentL());
			if (overlayDrmErr != KErrNone) 
				{
				iPlugin->CleanupOutput(); //should delete/cleanup unauthorised output
				User::Leave(overlayDrmErr);
				}
			}
		RestoreStateL();
		}

	}
   
void CImageProcessorImpl::Cancel()
	{
	if(iState == CImgProcessor::EInitializing || iState == CImgProcessor::EProcessing)
		{
		iState = iPreviousState;
		iPlugin->Cancel();
		}
	else if (iState == CImgProcessor::EPreviewInitializing || iState == CImgProcessor::EPreviewRendering) 
		{
		for (TInt i=0; i < iPreviews.Count(); i++ )
			{ 
			iPreviews[i]->Cancel();
			}
		// RestoreStateL should be called in preview cancel but double check here
		if (iState == CImgProcessor::EPreviewInitializing || iState == CImgProcessor::EPreviewRendering) 
			{
			iState = iPreviousState;
			}
		iPlugin->Cancel();
		}
	}
   
void CImageProcessorImpl::SetBackgroundColorL(const TRgb& aColor)
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->SetBackgroundColorL(aColor);
	}
   
TRgb CImageProcessorImpl::BackgroundColorL() const
	{
	__ASSERT_ALWAYS(iIsInputSet, User::Leave(KErrNotReady));
	return iPlugin->BackgroundColorL();
	}

void CImageProcessorImpl::CheckInputFormatL(const TUid& aFormat, const TUid& aSubFormat) 
	{
	if (aFormat == KNullUid && aSubFormat == aSubFormat) 
		{
		return;	
		}
	
	User::LeaveIfError(iInputFormats.Find(aFormat));
	if (aSubFormat != KNullUid) 
		{
		RArray<TUid> subFormats;
		iPlugin->SupportedInputSubFormatsL(aFormat, subFormats);
		TInt err = subFormats.Find(aSubFormat);
		subFormats.Close();
		User::LeaveIfError(err);
		}
	}

void CImageProcessorImpl::CheckOutputFormatL(const TUid& aFormat, const TUid& aSubFormat)
	{
	User::LeaveIfError(iOutputFormats.Find(aFormat));
	if (aSubFormat != KNullUid) 
		{
		RArray<TUid> subFormats;
		iPlugin->SupportedOutputSubFormatsL(aFormat, subFormats);
		TInt err = subFormats.Find(aSubFormat);
		subFormats.Close();
		User::LeaveIfError(err);
		}
	}

TBool CImageProcessorImpl::IsFrameworkEffect(TUid aEffect) const
	{
	return (iFrameworkEffects.Find(aEffect) != KErrNotFound);
	}

TBool CImageProcessorImpl::IsPluginEffect(TUid aEffect) const
	{
	return (iPluginEffects.Find(aEffect) != KErrNotFound);
	}

TBool FindEffectRelation(const TUid* aKey, const TEffect& aEffect)
	{
	return (*aKey==aEffect.Type());
	}

TEffect* CImageProcessorImpl::FindEffect(TUid aEffect) const
	{
	const TInt idx = iEffects.Find<TUid>(aEffect, FindEffectRelation);
	
	TEffect* effect=NULL;
	if(idx != KErrNotFound)
		{
		//Effect found
		effect = iEffects[idx];
		}
	return effect;
	}

TEffect* CImageProcessorImpl::CreateEffectL(TUid aEffect)
	{	
	//Ownership stays with plugin
	Plugin::MEffect* pluginEffect = iPlugin->EffectL(aEffect);
	ASSERT(pluginEffect);
	
	TEffect* effect = EffectFactoryL(aEffect, pluginEffect);
	
	if (!effect || (iEffects.Append(effect) != KErrNone)) 
		{
		delete effect;
		TInt err = KErrNone;
		TRAP(err, ResetL());
		iEffects.ResetAndDestroy();
		iPlugin->CleanupEffects();
		User::Leave(KErrNoMemory);
		}

	ASSERT(effect);
	return effect;
	}

TEffect* CImageProcessorImpl::EffectFactoryL(TUid aEffect, Plugin::MEffect* aPluginEffect)
	{
	if(aEffect == KEffectSepiaUid)
		{
		return new TEffectSepia(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectGrayscaleUid)
		{
		return new TEffectGrayscale(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectNegativeUid)
		{
		return new TEffectNegative(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectGrayscaleNegativeUid)
		{
		return new TEffectGrayscaleNegative(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectOilyUid)
		{
		return new TEffectOily(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectPaintUid)
		{
		return new TEffectPaint(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectMilkyUid)
		{
		return new TEffectMilky(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectFogUid)
		{
		return new TEffectFog(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectMirrorLeftToRightUid)
		{
		return new TEffectMirrorLeftToRight(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectMagicPenUid)
		{
		return new TEffectMagicPen(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectBrightnessUid)
		{
		return new TEffectBrightness(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectSharpnessUid)
		{
		return new TEffectSharpness(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectRotationUid)
		{
		return new TEffectRotation(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectContrastUid)
		{
		return new TEffectContrast(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectRgbColorAdjustUid)
		{
		return new TEffectRgbColorAdjust(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectEmbossUid)
		{
		return new TEffectEmboss(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectSolarizeUid)
		{
		return new TEffectSolarize(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectPosterizeUid)
		{
		return new TEffectPosterize(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectStampUid)
		{
		return new TEffectStamp(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectSketchUid)
		{
		return new TEffectSketch(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectNoiseUid)
		{
		return new TEffectNoise(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectMoonlightUid)
		{
		return new TEffectMoonlight(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectFrostedGlassUid)
		{
		return new TEffectFrostedGlass(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectDespeckleUid)
		{
		return new TEffectDespeckle(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectBlurUid)
		{
		return new TEffectBlur(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectColorizationUid)
		{
		return new TEffectColorization(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectCartoonUid)
		{
		return new TEffectCartoon(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectLocalBoostUid)
		{
		return new TEffectLocalBoost(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectColorBoostUid)
		{
		return new TEffectColorBoost(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectWhiteBalanceUid)
		{
		return new TEffectWhiteBalance(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectAutoLevelsUid)
		{
		return new TEffectAutoLevels(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectLevelsUid)
		{
		return new TEffectLevels(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectExposureUid)
		{
		return new TEffectExposure(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectSpotLightUid)
		{
		return new TEffectSpotLight(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectCustomTransformationUid)
		{
		return new TEffectCustomTransformation(*aPluginEffect, *this);
		}
	else if(aEffect == KGeometricalOperationUid)
		{
		return new TGeometricalOperation(*aPluginEffect, *this);
		}
	else if(aEffect == KEffectBorderUid)
		{
		return new TEffectBorder(*aPluginEffect, *this);
		}
	else if (aEffect == KEffectAntiqueUid)
		{
		return new TEffectAntique(*aPluginEffect, *this);
		}
	
	User::Leave(KErrNotSupported);
	return NULL;
	}

TEffect* CImageProcessorImpl::EffectL(TUid aEffect)
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));
	//All effects must be supported by the plugin
	__ASSERT_ALWAYS(IsPluginEffect(aEffect), User::Leave(KErrNotSupported));
	
	TEffect* effect=NULL;	
		
	if(IsFrameworkEffect(aEffect))
		{
		effect = FindEffect(aEffect);
		if(effect == NULL)
			{
			effect=CreateEffectL(aEffect);
			}		
		}
	else
		{
		Plugin::MEffect* pluginEffect = iPlugin->EffectL(aEffect);
		effect = new TEffect(aEffect, *pluginEffect, *this);
		
		if (!effect || (iEffects.Append(effect) != KErrNone)) 
			{
			delete effect;
			TInt err = KErrNone;
			TRAP(err, ResetL());
			iEffects.ResetAndDestroy();
			iPlugin->CleanupEffects();
			User::Leave(KErrNoMemory);
			}
		}
		
	ASSERT(effect);
	return effect;
	}

TBool CImageProcessorImpl::CanUndoL() const
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	return iPlugin->CanUndoL();
	}
   
void CImageProcessorImpl::UndoL()
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->UndoL();
	}
   
void CImageProcessorImpl::UndoAllL()
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->UndoAllL();
	}

TBool CImageProcessorImpl::CanRedoL() const
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	return iPlugin->CanRedoL();
	}
   
void CImageProcessorImpl::RedoL()
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->RedoL();
	}
   
void CImageProcessorImpl::RedoAllL()
	{
	__ASSERT_ALWAYS(((iState == CImgProcessor::EInitialized) && (iIsInputSet)), User::Leave(KErrNotReady));
	iPlugin->RedoAllL();
	}

TBool FindPreviewRelation(const TInt* aPreviewId, const TPreview& aPreview)
	{
	return (aPreview.PreviewId() == *aPreviewId);
	}

TPreview* CImageProcessorImpl::FindPreview(TInt aPreviewId) const
	{
	const TInt idx = iPreviews.Find<TInt>(aPreviewId, FindPreviewRelation);
	
	TPreview* preview = NULL;
	if(idx != KErrNotFound)
		{
		preview = iPreviews[idx];
		}
	return preview;
	}
   
TPreview* CImageProcessorImpl::PreviewL(TInt aPreviewId)
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));

	TPreview* preview = FindPreview(aPreviewId);
	if (preview == NULL) 
		{
		Plugin::MPreview* pluginPreview = iPlugin->PreviewL(aPreviewId);
		preview = new TPreview(*pluginPreview, *this);

		if (!preview || (iPreviews.Append(preview) != KErrNone)) 
			{
			delete preview;
			TInt err = KErrNone;
			TRAP(err, ResetL());
			User::Leave(KErrNoMemory);
			}		
		}
	return preview;
	}

TOverlay* CImageProcessorImpl::OverlayL()
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));

	if (iOverlay == NULL) 
		{
		Plugin::MOverlay* pluginOverlay = iPlugin->OverlayL();
		iOverlay = new TOverlay(*pluginOverlay, *this);
		
		if (!iOverlay) 
			{
			TInt err = KErrNone;
			TRAP(err, ResetL());
			User::Leave(KErrNoMemory);
			}
		}
	return iOverlay;
	}

TProgressInfo* CImageProcessorImpl::ProgressInfoL()
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));

	if (iProgressInfo == NULL) 
		{
		Plugin::MProgressInfo* pluginProgressInfo = iPlugin->ProgressInfoL();
		iProgressInfo = new TProgressInfo(*pluginProgressInfo, *this);
		
		if (!iProgressInfo) 
			{
			TInt err = KErrNone;
			TRAP(err, ResetL());
			User::Leave(KErrNoMemory);
			}
		}
	return iProgressInfo;
	}

TInputInfo* CImageProcessorImpl::InputInfoL()
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));
	
	if (iInputInfo == NULL) 
		{
		Plugin::MInputInfo* pluginInputInfo = iPlugin->InputInfoL();
		iInputInfo = new TInputInfo(*pluginInputInfo, *this);

		if (!iInputInfo) 
			{
			TInt err = KErrNone;
			TRAP(err, ResetL());
			User::Leave(KErrNoMemory);
			}
		}
	return iInputInfo;
	}

TOutputInfo* CImageProcessorImpl::OutputInfoL()
	{
	__ASSERT_ALWAYS(((iState != CImgProcessor::EUninitialized) && (iState != CImgProcessor::EInitializing)), User::Leave(KErrNotReady));
	
	if (iOutputInfo == NULL) 
		{
		Plugin::MOutputInfo* pluginOutputInfo = iPlugin->OutputInfoL();
		iOutputInfo = new TOutputInfo(*pluginOutputInfo, *this);

		if (!iOutputInfo) 
			{
			TInt err = KErrNone;
			TRAP(err, ResetL());
			User::Leave(KErrNoMemory);
			}
		}
	return iOutputInfo;
	}

void CImageProcessorImpl::SetOutputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckOutputFormatL(aFormat, aSubFormat);
	iPlugin->SetOutputL(aFilename, aFormat, aSubFormat);
	iIsOutputSet = ETrue;
	}
   
void CImageProcessorImpl::SetOutputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckOutputFormatL(aFormat, aSubFormat);
	iPlugin->SetOutputL(aFile, aFormat, aSubFormat);
	iIsOutputSet = ETrue;
	}
   
void CImageProcessorImpl::SetOutputL(RBuf8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	CheckOutputFormatL(aFormat, aSubFormat);
	iPlugin->SetOutputL(aBuffer, aFormat, aSubFormat);
	iIsOutputSet = ETrue;
	}
   
void CImageProcessorImpl::SetOutputL(CImageFrame& aPixelBuffer)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));
	
	if (aPixelBuffer.FrameSizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	iPlugin->SetOutputL(aPixelBuffer);
	iIsOutputSet = ETrue;
	}

void CImageProcessorImpl::SetOutputL(CFbsBitmap& aBitmap, CFbsBitmap* aMask)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), User::Leave(KErrNotReady));

	if (aBitmap.SizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	if((aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid) || (aMask!=NULL && aMask->Handle()!=0 && aMask->ExtendedBitmapType()!=KNullUid))
        {
        User::Leave(KErrNotSupported);
        }
	
	if ((aMask != NULL) && (aMask->SizeInPixels() == TSize(0,0))) 
		{
		User::Leave(KErrArgument);
		}
	
	iPlugin->SetOutputL(aBitmap, aMask);
	iIsOutputSet = ETrue;
	}

void CImageProcessorImpl::PostEvent(TInt aEventId, TUid aUid, TInt aId, TInt aError)
	{
	if (aEventId == CImgProcessor::EEventProcessingComplete && aError == KErrNone) 
		{
		if (iDrmInput != NULL) 
			{
			TRAPD(drmErr, iDrmInput->ExecuteIntentL());
			if (drmErr != KErrNone) 
				{
				iPlugin->CleanupOutput();//should delete/cleanup unauthorised output
				iCallbackHandler->AddCallback(aEventId, aUid, aId, drmErr);
				return;
				}
			}
		
		for (TInt i=0; i < iOverlayDrmInputs.Count(); i++ )
			{ 
			TRAPD(overlayDrmErr, iOverlayDrmInputs[i]->ExecuteIntentL());
			if (overlayDrmErr != KErrNone) 
				{
				iPlugin->CleanupOutput();//should delete/cleanup unauthorised output
				iCallbackHandler->AddCallback(aEventId, aUid, aId, overlayDrmErr);
				return;
				}
			}
		}
	
	TRAPD(err, ProcessEventL(aEventId, aUid, aId, aError));

	if (err != KErrNone) 
		{
		//strayed event?
		if (aError == KErrNone) 
			{
			aError = err;
			}
		}
	
	iCallbackHandler->AddCallback(aEventId, aUid, aId, aError);
	}

void CImageProcessorImpl::ProcessEventL(TInt aEventId, TUid /*aUid*/, TInt aId, TInt aError)
	{
	switch (aEventId) 
		{
		case CImgProcessor::EEventInitializingComplete:
			SetStateL((aError == KErrNone)? CImgProcessor::EInitialized: CImgProcessor::EUninitialized);
			break;
		
		case CImgProcessor::EEventProcessingComplete:
			RestoreStateL();
			break;
		
		case CImgProcessor::EEventPreviewInitializingComplete:
			PreviewL(aId)->SetStateL((aError == KErrNone)? TPreview::EInitialized: TPreview::EUninitialized);
			RestoreStateL();
			break;

		case CImgProcessor::EEventPreviewRenderingComplete:
			PreviewL(aId)->SetStateL(TPreview::EInitialized);
			RestoreStateL();
			break;
		}
	}

TAny* CImageProcessorImpl::Extension(TUid aExtension)
	{
	__ASSERT_ALWAYS((iState == CImgProcessor::EInitialized), Panic(EPanicNotInitialized));
	return iPlugin->Extension(aExtension);
	}

TBool CImageProcessorImpl::IsBusy(CImgProcessor::TState aState) const 
	{
	return (aState == CImgProcessor::EInitializing || 
		aState == CImgProcessor::EProcessing || 
		aState == CImgProcessor::EPreviewInitializing || 
		aState == CImgProcessor::EPreviewRendering);
	}

void CImageProcessorImpl::SetStateL(CImgProcessor::TState aState)
	{
	if(iState != aState)
		{
		if (!iValidStates[iState][aState]) 
			{
			User::Leave(KErrNotReady);
			}
		
		if (IsBusy(aState))
			{
			iPreviousState = iState;
			}

		iState = aState;
		}
	}

void CImageProcessorImpl::RestoreStateL()
	{
	SetStateL(iPreviousState);
	}

void CImageProcessorImpl::ConvertMimeTypeToUidL(const TDesC8& aMimeType, TUid& aFormat, TUid& aSubFormat)
	{
	for (TInt i = 0; i < iTypesUtilityArray.Count(); i++)
		{
		CFileExtensionMIMEType& fileExtAndMIMEType = *iTypesUtilityArray[i];
				
		if ( (fileExtAndMIMEType.MIMEType().Length() > 0)
		     && (aMimeType.Find(fileExtAndMIMEType.MIMEType()) != KErrNotFound) ) 
			{
			aFormat = fileExtAndMIMEType.ImageType(); 
			aSubFormat = fileExtAndMIMEType.ImageSubType();
			return;
			}
		}
	User::Leave(KErrNotFound);
	}

void CImageProcessorImpl::ConvertFileExtensionToUidL(const TDesC& aFileExtension, TUid& aFormat, TUid& aSubFormat)
	{
	for (TInt i = 0; i < iTypesUtilityArray.Count(); i++)
		{
		CFileExtensionMIMEType& fileExtAndMIMEType = *iTypesUtilityArray[i];
		
		if ( (fileExtAndMIMEType.FileExtension().Length() > 0 ) 
		     && (aFileExtension.Find(fileExtAndMIMEType.FileExtension()) != KErrNotFound) ) 
			{
			aFormat = fileExtAndMIMEType.ImageType(); 
			aSubFormat = fileExtAndMIMEType.ImageSubType();
			return;
			}
		}
	User::Leave(KErrNotFound);
	}

void CImageProcessorImpl::ConvertUidToMimeTypeL(TDes8& aMimeType, const TUid& aFormat, const TUid& /*aSubFormat*/)
	{
	for (TInt i = 0; i < iTypesUtilityArray.Count(); i++)
		{
		CFileExtensionMIMEType& fileExtAndMIMEType = *iTypesUtilityArray[i];
		
		if (fileExtAndMIMEType.ImageType() == aFormat) 
			{
			aMimeType = fileExtAndMIMEType.MIMEType();
			return;
			}
		}
	User::Leave(KErrNotFound);
	}

void CImageProcessorImpl::ConvertUidToFileExtensionL(TDes& aFileExtension, const TUid& aFormat, const TUid& /*aSubFormat*/)
	{
	for (TInt i = 0; i < iTypesUtilityArray.Count(); i++)
		{
		CFileExtensionMIMEType& fileExtAndMIMEType = *iTypesUtilityArray[i];
		
		if (fileExtAndMIMEType.ImageType() == aFormat) 
			{
			aFileExtension = fileExtAndMIMEType.FileExtension();
			return;
			}
		}
	User::Leave(KErrNotFound);
	}

void CImageProcessorImpl::AppendOverlayDrmInputL() 
	{
	if (iOverlayDrmInput != NULL) 
		{
		iOverlayDrmInputs.AppendL(iOverlayDrmInput);
		iOverlayDrmInput = NULL;
		}
	}

CImageProcessorDrmInput* CImageProcessorDrmInput::NewL(TMMSource& aDrmFile)
	{
	CImageProcessorDrmInput* self = new (ELeave) CImageProcessorDrmInput();
	CleanupStack::PushL(self);
	self->ConstructL(aDrmFile);
	CleanupStack::Pop(self);
	return self;
	}

void CImageProcessorDrmInput::ConstructL(TMMSource& aDrmFile)
	{
		{
		using namespace ContentAccess;
		
		TUid sourceType = aDrmFile.SourceType();
		const TDesC& uniqueId = aDrmFile.UniqueId();
		TIntent intent = aDrmFile.Intent();
		CContent* content = NULL;
		
		if (sourceType == KUidMMFileHandleSource)
			{
			const TMMFileHandleSource& source = static_cast<const TMMFileHandleSource&>(aDrmFile);
			content = CContent::NewL(source.Handle());
			}
		else if (sourceType == KUidMMFileSource) 
			{
			const TMMFileSource& fileSource = static_cast<const TMMFileSource&>(aDrmFile);
		
			TRAPD(err, content = CContent::NewL(fileSource.Name(), EContentShareReadOnly));
        
			if(err != KErrNone && err != KErrNoMemory) //We retry in case the client already has the file open.
				{
				content = CContent::NewL(fileSource.Name(), EContentShareReadWrite);
				}
			}
		else 
			{
			// unknown source type
			User::Leave(KErrNotSupported);
			}
        
		CleanupStack::PushL(content);

		CData* data = content->OpenContentL(intent, uniqueId);
		CleanupStack::PushL(data);

		User::LeaveIfError(data->EvaluateIntent(intent));
	
		TInt size = 0;
		data->DataSizeL(size);
		HBufC8 *readBuffer = HBufC8::NewMaxL(size);
		CleanupStack::PushL(readBuffer);
	
		TPtr8 ptr(readBuffer->Des());
		TInt pos = 0;
		User::LeaveIfError(data->Seek(ESeekStart, pos));
		User::LeaveIfError(data->Read(ptr, size));

		CleanupStack::Pop(3);

		iDecryptedBuffer = readBuffer;
		iContent = content;
		iData = data;
		iIntent = intent;
		}
	}

void CImageProcessorDrmInput::ExecuteIntentL()
	{
	User::LeaveIfError(iData->ExecuteIntent(iIntent));
	}

CImageProcessorDrmInput::~CImageProcessorDrmInput() 
	{
	delete iDecryptedBuffer;
	delete iContent;
	delete iData;
	}

//EOF
