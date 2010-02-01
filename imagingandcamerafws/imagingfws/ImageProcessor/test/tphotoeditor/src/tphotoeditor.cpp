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

#include <apgtask.h>
#include <barsread.h>
#include <bidi.h>
#include <eikenv.h>
#include <techview/eikchlst.h>
#include <techview/eikinfo.h>
#include <techview/eikmenup.h>
#include <e32math.h>
#include <badesca.h>
#include <techview/eikon.rsg>	
#include <techview/eiktxlbx.h>
#include <techview/eikmfne.h>
#include <techview/eiktxlbm.h>
#include <iclexif.h>
#include <eikstart.h>
#include <eikfile.rsg>
#include <techview/eikmisdg.h>
#include <hal.h>
#include <caf/caf.h>
#include <caf/content.h>
#include <caf/data.h>

#include "tphotoeditor.h"


typedef struct
	{
	TUid iUid;
	TInt iPromptId;
	TInt iDialogId;
	}
TEffectEntry;

static const TEffectEntry KEffectTable[] =
	{
		{ImageProcessor::KEffectSepiaUid, R_PROMPT_TEXT_23, KErrNotFound},
		{ImageProcessor::KEffectGrayscaleUid, R_PROMPT_TEXT_24, KErrNotFound},
		{ImageProcessor::KEffectNegativeUid, R_PROMPT_TEXT_25, KErrNotFound},
		{ImageProcessor::KEffectGrayscaleNegativeUid, R_PROMPT_TEXT_26, KErrNotFound},
		{ImageProcessor::KEffectOilyUid, R_PROMPT_TEXT_27, KErrNotFound},
		{ImageProcessor::KEffectPaintUid, R_PROMPT_TEXT_28, KErrNotFound},
		{ImageProcessor::KEffectMilkyUid, R_PROMPT_TEXT_29, KErrNotFound},
		{ImageProcessor::KEffectFogUid, R_PROMPT_TEXT_30, KErrNotFound},
		{ImageProcessor::KEffectMirrorLeftToRightUid, R_PROMPT_TEXT_31, KErrNotFound},
		{ImageProcessor::KEffectMagicPenUid, R_PROMPT_TEXT_32, KErrNotFound},
		{ImageProcessor::KEffectAntiqueUid, R_PROMPT_TEXT_65, KErrNotFound},
		{ImageProcessor::KEffectAutoLevelsUid, R_PROMPT_TEXT_52, KErrNotFound}
	};

static const TInt KEffectTableSize = sizeof(KEffectTable) / sizeof(TEffectEntry);

//
// Effects that use a CNumberInputDialog.
//
static const TEffectEntry KNumberInputEffectTable[] =
	{
		{ImageProcessor::KEffectBrightnessUid, R_BRIGHTNESS_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectColorBoostUid, R_COLORBOOST_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectContrastUid, R_CONTRAST_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectEmbossUid, R_EMBOSS_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectExposureUid, R_EXPOSURE_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectMoonlightUid, R_MOONLIGHT_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectPosterizeUid, R_POSTERIZE_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectSharpnessUid, R_SHARPNESS_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG},
		{ImageProcessor::KEffectSolarizeUid, R_SOLARIZE_TEXT_TITLE, R_PHOTOEDITOR_LEVEL_DIALOG}
	};

//
// Effects that use a CModeInputDialog.
//
static const TEffectEntry KModeInputEffectTable[] =
	{
		{ImageProcessor::KEffectDespeckleUid, EEffectDespeckleMode, R_PHOTOEDITOR_DESPECKLE_DIALOG},
		{ImageProcessor::KEffectFrostedGlassUid, EEffectFrostedGlassMode, R_PHOTOEDITOR_FROSTEDGLASS_DIALOG},
		{ImageProcessor::KEffectLocalBoostUid, EEffectLocalBoostMode, R_PHOTOEDITOR_LOCALBOOST_DIALOG},
		{ImageProcessor::KEffectNoiseUid, EEffectNoiseMode, R_PHOTOEDITOR_NOISE_DIALOG}
	};


void CTPhotoEditorAppUi::ExecuteEffectL(TInt aCommand)
	{
	aCommand -= EPhotoEditorFirstEffectCmd;
	__ASSERT_ALWAYS((aCommand >= 0) && (aCommand < KEffectTableSize), User::Invariant());
	
	TUid uid(KEffectTable[aCommand].iUid);
	TInt resource = KEffectTable[aCommand].iPromptId;
	
	if (uid == ImageProcessor::KEffectSepiaUid)
		{
		ApplyEffectL<ImageProcessor::TEffectSepia*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectGrayscaleUid)
		{
		ApplyEffectL<ImageProcessor::TEffectGrayscale*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectNegativeUid)
		{
		ApplyEffectL<ImageProcessor::TEffectNegative*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectGrayscaleNegativeUid)
		{
		ApplyEffectL<ImageProcessor::TEffectGrayscaleNegative*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectOilyUid)
		{
		ApplyEffectL<ImageProcessor::TEffectOily*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectPaintUid)
		{
		ApplyEffectL<ImageProcessor::TEffectPaint*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectMilkyUid)
		{
		ApplyEffectL<ImageProcessor::TEffectMilky*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectFogUid)
		{
		ApplyEffectL<ImageProcessor::TEffectFog*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectMirrorLeftToRightUid)
		{
		ApplyEffectL<ImageProcessor::TEffectMirrorLeftToRight*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectMagicPenUid)
		{
		ApplyEffectL<ImageProcessor::TEffectMagicPen*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectAntiqueUid)
		{
		ApplyEffectL<ImageProcessor::TEffectAntique*>(uid, resource);
		}
	else if (uid == ImageProcessor::KEffectAutoLevelsUid)
		{
		ApplyEffectL<ImageProcessor::TEffectAutoLevels*>(uid, resource);
		}
	else
		{
		ASSERT(EFalse);
		}
	}

ImageProcessor::CImgProcessor::TOperation CTPhotoEditorAppUi::CommandToOperation(TInt aCommand, TInt& aResourceId)
	{
	switch (aCommand)
		{
		case EPhotoEditorCmdRotate90:
			aResourceId = R_PROMPT_TEXT_18;
			return ImageProcessor::CImgProcessor::EOperationRotate90;
			
		case EPhotoEditorCmdRotate180:
			aResourceId = R_PROMPT_TEXT_19;
			return ImageProcessor::CImgProcessor::EOperationRotate180;
			
		case EPhotoEditorCmdRotate270:
			aResourceId = R_PROMPT_TEXT_20;
			return ImageProcessor::CImgProcessor::EOperationRotate270;
			
		case EPhotoEditorCmdHorizontalFlip:
			aResourceId = R_PROMPT_TEXT_21;
			return ImageProcessor::CImgProcessor::EOperationMirrorHorizontalAxis;
			
		case EPhotoEditorCmdVerticalFlip:
			aResourceId = R_PROMPT_TEXT_22;
			return ImageProcessor::CImgProcessor::EOperationMirrorVerticalAxis;
			
		default:
			aResourceId = KErrNotFound;
			return ImageProcessor::CImgProcessor::EOperationNone;
		}
	}


void CTPhotoEditorAppUi::ExecuteOperationL(TInt aCommand)
	{
	using ImageProcessor::CImgProcessor;
	
	TInt resourceId = KErrNotFound;
	CImgProcessor::TOperation operation = CTPhotoEditorAppUi::CommandToOperation(aCommand, resourceId);
	if (operation != CImgProcessor::EOperationNone)
		{
		iImageProcessor->ApplyOperationL(operation);
		if (resourceId != KErrNotFound)
			{
			iEikonEnv->InfoMsg(resourceId);
			}
		
		RefreshL();
		}
	}

//
// constructL method that creates the AppView object
//
void CTPhotoEditorAppUi::ConstructL()
	{
	BaseConstructL();
	
	iRootDirectory = KDefPath().AllocL();
	iFolderFiles.ResetAndDestroy();
	iScreenSize = iCoeEnv->ScreenDevice()->SizeInPixels();
	isNavigate = ETrue;
	ASSERT(!iImageProcessor);
	iImageProcessor = ImageProcessor::CImgProcessor::NewL(iCoeEnv->FsSession(), *this);
	iImageProcessor->InitializeL();
	//iImageProcessor->InitializeL();
	}

//
// Destructor 
//
CTPhotoEditorAppUi::~CTPhotoEditorAppUi()
	{
	delete iImageProcessor;
	delete iRootDirectory;
	iFolderFiles.ResetAndDestroy();
	
	if (iAppView)
		{
		RemoveFromStack(iAppView);
		}
	
	delete iFileView;
	delete iGalleryView;
	}

void CTPhotoEditorAppUi::ClearBitmap(const TPoint& aPos, const TSize& aSize)
	{
	iAppView->ClearBitmap(aPos, aSize);
	}

//
// Actions to taken when the ImageProcessor completes its initialization.
//
void CTPhotoEditorAppUi::ImageProcessorInitializingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aError)
	{
	if (aError != KErrNone)
		{
		ErrorMessage(_L("ImageProcessor initialise error"), aError);
		}
	
	TUint64 options = iImageProcessor->Options();
	options |= ImageProcessor::CImgProcessor::EOptionSyncProcessing;
	
	// ignore the leave scan warnings
	
	TRAP_IGNORE(iImageProcessor->SetOptionsL(options));
	
	TRAP_IGNORE(SwitchViewL(EViewGallery));
	}

void CTPhotoEditorAppUi::ErrorMessage(const TDesC& aMessage, TInt aError)
	{
	_LIT(KErrorFormat, "*** %S: (%d)");

	TBuf<512> buffer;
	
	buffer.AppendFormat(KErrorFormat, &aMessage, aError);
	iEikonEnv->InfoMsg(buffer);
	}

//
// Actions to taken when the TPreview of ImageProcessor completes its initialization
//
void CTPhotoEditorAppUi::ImageProcessorPreviewInitializingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aPreviewId, TInt aError)
	{
	if (aError != KErrNone)
		{
		ErrorMessage(_L("Preview initialize error"), aError);
		}
	else
		{
		// ignore the leave scan warnings
		TRAP_IGNORE(iAppView->PreviewInitializedL(aPreviewId));
		}
	}

//
// Actions to taken when the ImageProcessor completes its procession
//
void CTPhotoEditorAppUi::ImageProcessingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aError) 
	{
#if defined(__PHOTOEDIT_PROFILING)
	if(iFlag)
	{
	iProcessEnd=User::FastCounter();			
	iFlag = EFalse;	
	}
	
	TInt freq=0;
	HAL::Get(HAL::EFastCounterFrequency, freq);
	DEBUG_OUTPUT_1("End",iProcessEnd);
	DEBUG_OUTPUT_1("Frequency",freq);
	DEBUG_OUTPUT_1("Time cost for ImageProcessing", (iProcessEnd - iProcessStart)/freq);
#endif	

	if (aError != KErrNone)
		{
		ErrorMessage(_L("Image processing error"), aError);
		}
	}

//
// Actions to taken when the TPreview of ImageProcessor completes its render
//
void CTPhotoEditorAppUi::ImageProcessorPreviewRenderingComplete(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aPreviewId, TInt aError)
	{
	if (aError != KErrNone)
		{
		ErrorMessage(_L("Preview rendering error"), aError);
		}
	else
		{
		TRAP_IGNORE(iAppView->PreviewRenderedL(aPreviewId));
		}
	}

//
// During image processing, after each iteration, this function will be notified
//
void CTPhotoEditorAppUi::ImageProcessorEvent(ImageProcessor::CImgProcessor& /*aImageProcessor*/, TInt aEventId, TUid /*aUid*/, TInt /*aId*/)
	{
#if defined(__PHOTOEDIT_PROFILING)
	if(iFlag)
	{
	iProcessEnd=User::FastCounter();			
	iFlag = EFalse;	
	}
#endif	

	if (!iProgressInfo)
		{
		return;
		}
	
	//output progress infor for encoding process
	if (aEventId == 2)
		{
		TInt previousProgress = iProgress;
		TInt currentIteration = 0;
		TInt numberOfIterations = 0; 
		
		TRAP_IGNORE({
			currentIteration = iProgressInfo->CurrentIterationL();
			numberOfIterations = iProgressInfo->NumberOfIterationsL();
			});
		
		if (numberOfIterations == 0)
			{
			return;
			}
		
		iProgress = 100 * currentIteration / numberOfIterations;

		if (iProgress > previousProgress)
		 	{
		 	ErrorMessage(_L("Encoding progress"), iProgress);
			}
		}
	}

void CTPhotoEditorAppUi::DimMenuItems(CEikMenuPane* aMenuPane, const TInt aFromCmd, const TInt aToCmd)
	{
	ASSERT(aMenuPane);
	ASSERT(aFromCmd <= aToCmd);
	
	for (TInt i = aFromCmd; i <= aToCmd; ++i)
		{
		aMenuPane->SetItemDimmed(i, ETrue);
		}
	}

//
// Set corresponding menu item dimmed if current state of ImageProcessor not allowed for certain actions
//
void CTPhotoEditorAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
	{
	ImageProcessor::CImgProcessor::TState state = iImageProcessor->State();
	
	// If ImageProcessor is rendering, dim all apply effect menus except Cancel
	if (state != ImageProcessor::CImgProcessor::EInitialized )
		{
		TInt from = 0;
		TInt to = 0;
		TBool doIt = ETrue;
		
		switch (aResourceId)
			{
			case R_PHOTOEDITOR_FILE_MENU:
				from = EPhotoEditorCmdOpenFile;
				to = EPhotoEditorCmdSaveAs;
				break;
				
			case R_PHOTOEDITOR_INSERT_MENU:
				from = EPhotoEditorCmdInsertFrame;
				to = EPhotoEditorCmdInsertDRMClipart;
				break;
				
			case R_PHOTOEDITOR_UNDO_MENU:
				from = EPhotoEditorCmdUndo;
				to = EPhotoEditorCmdUndoAll;
				break;
				
			case R_PHOTOEDITOR_VIEW_MENU:
				from = EPhotoEditorCmdZoomIn;
				to = EPhotoEditorCmdRefresh;
				break;
				
			case R_PHOTOEDITOR_PAN_MENU:
				from = EPhotoEditorCmdPanUp;
				to = EPhotoEditorCmdPanRight;
				break;
				
			case R_PHOTOEDITOR_GEOMETRICALOPERATION_MENU:
				from = EPhotoEditorCmdRotate90;
				to = EPhotoEditorCmdVerticalFlip;
				break;
				
			case R_PHOTOEDITOR_BASICPHOTOART_MENU:
				from = EPhotoEditorCmdSepia;
				to = EPhotoEditorCmdMagicPen;
				break;
			
			case R_PHOTOEDITOR_ADVANCEDPHOTOART_MENU:
				from = EPhotoEditorCmdBrightness;
				to = EPhotoEditorCmdSpotlight;
				break;
			
			case R_PHOTOEDITOR_FUNNYPHOTOART_MENU:
				from = EPhotoEditorCmdAntique;
				to = EPhotoEditorCmdCustomTransformation;
				break;
			
			case R_PHOTOEDITOR_CLEARSHOT_MENU:
				from = EPhotoEditorCmdColorBoost;
				to = EPhotoEditorCmdExposure;
				break;
				
			default:
				doIt = EFalse;
			}
		
		if (doIt)
			{
			DimMenuItems(aMenuPane, from, to);
			}
		}
	
	//Cancel (menu item) is disabled in EUninitialized/EInitialized/EEffectActive state of ImageProcessor
	if ((state == ImageProcessor::CImgProcessor::EUninitialized) ||
		(state == ImageProcessor::CImgProcessor::EInitialized) ||
		(state == ImageProcessor::CImgProcessor::EEffectActive))
		{
		if (aResourceId == R_PHOTOEDITOR_FILE_MENU)
			{
			aMenuPane->SetItemDimmed(EPhotoEditorCmdCancel, ETrue);
			}
		}
	}

//
// Disable corresponding hotkeys if current state of ImageProcessor not allowed for certain actions
//
TBool CTPhotoEditorAppUi::CheckHotKeyNotDimmedL(TInt aCommandId)
	{
	ImageProcessor::CImgProcessor::TState state = iImageProcessor->State();
		
	// If ImageProcessor is rendering, disable all hotkeys except Cancel
	if (state != ImageProcessor::CImgProcessor::EInitialized)
		{
		for (TInt cmd = EPhotoEditorCmdOpenFile; cmd <= EPhotoEditorCmdPanRight; cmd++)
			{
			if (aCommandId == cmd)
				{
				return EFalse;
				}
			}
		}
	
	// Cancel (hot key) is disabled in EUninitialized/EInitialized/EEffectActive state of ImageProcessor
	if ((state == ImageProcessor::CImgProcessor::EUninitialized) ||
		(state == ImageProcessor::CImgProcessor::EInitialized) ||
		(state == ImageProcessor::CImgProcessor::EEffectActive))
		{
		if (aCommandId == EPhotoEditorCmdCancel)
			{
			return EFalse;
			}
		}
	
	return ETrue;
	}

/**
 *	Handles the Menu events
 *	@param aCommand - command to be passed based on the menu item
 *						selected by the user
 *
 */  
void CTPhotoEditorAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EPhotoEditorCmdOpenFile:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_0);
			OpenFileL();
			break;
			
		case EPhotoEditorCmdOpenDRMFile:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_1);
			OpenDRMFileL();
			break;	
		
		case EPhotoEditorCmdOpenGallery:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_2);
			OpenGalleryL();
			break;
				
		case EPhotoEditorCmdSaveAs:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_3);
			SaveAsL();
			break;
		
		case EPhotoEditorCmdInsertFrame:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_4);
			InsertFrameL();
			break;
	
		case EPhotoEditorCmdInsertClipart:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_5);
			InsertClipartL();
			break;	
			
		case EPhotoEditorCmdInsertDRMClipart:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_6);
			InsertDRMClipartL();
			break;		
			
		case EPhotoEditorCmdCancel: 
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_7);
			Cancel();
			break;
			
		case EPhotoEditorCmdUndo:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_8);
			UndoL();
			break;
			
		case EPhotoEditorCmdUndoAll:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_9);
			UndoAllL();
			break;
		
		case EPhotoEditorCmdExit:
			Exit();
			break;
			
		case EPhotoEditorCmdZoomIn:
			iAppView->ZoomL(CTPhotoEditorAppView::EZoomIn);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_10);
			break;
		
		case EPhotoEditorCmdZoomOut:
			iAppView->ZoomL(CTPhotoEditorAppView::EZoomOut);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_11);
			break;
			
		case EPhotoEditorCmdZoomReset:
			iAppView->ZoomL(CTPhotoEditorAppView::EZoomReset);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_60);
			break;	
		
		case EPhotoEditorCmdPanUp:
			iAppView->PanL(CTPhotoEditorAppView::EPanUp);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_12);
			break;
		
		case EPhotoEditorCmdPanDown:
			iAppView->PanL(CTPhotoEditorAppView::EPanDown);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_13);
			break;	
		
		case EPhotoEditorCmdPanLeft:
			iAppView->PanL(CTPhotoEditorAppView::EPanLeft);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_14);
			break;	
		
		case EPhotoEditorCmdPanRight:
			iAppView->PanL(CTPhotoEditorAppView::EPanRight);
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_15);
			break;	
		
		case EPhotoEditorCmdForward:
			// Effects when in Gallery mode must be undone or they
			// will be applied to all images instead of the main one.
			DoNavigateL(ETrue);
			break;	
			
		case EPhotoEditorCmdBackward:
			// Effects when in Gallery mode must be undone or they
			// will be applied to all images instead of the main one.
			DoNavigateL(EFalse);
			break;		
		
		case EPhotoEditorCmdSwitchView: 
			if (IsGalleryView())
				{
				iEikonEnv->InfoMsg(R_PROMPT_TEXT_58);
				SwitchViewL(EViewFile);
				}
			else
				{
				iEikonEnv->InfoMsg(R_PROMPT_TEXT_59);
				SwitchViewL(EViewGallery);
				}
			break;
		
		case EPhotoEditorCmdRefresh:
					
			if (IsGalleryView())
				{
				ReDrawAllPreviewL();
				}
			else
				{
				iFileView->ReFreshScreenL(ETrue);					
				}	
			break;
				
		case EPhotoEditorCmdRotate90:
		case EPhotoEditorCmdRotate180:
		case EPhotoEditorCmdRotate270:
		case EPhotoEditorCmdHorizontalFlip:
		case EPhotoEditorCmdVerticalFlip:
			ExecuteOperationL(aCommand);
			break;		
			
		case EPhotoEditorCmdSepia:
		case EPhotoEditorCmdGrayscale:
		case EPhotoEditorCmdNegative:
		case EPhotoEditorCmdGrayscaleNegative:
		case EPhotoEditorCmdOily:
		case EPhotoEditorCmdPaint:
		case EPhotoEditorCmdMilky:
		case EPhotoEditorCmdFog:
		case EPhotoEditorCmdMirror:
		case EPhotoEditorCmdMagicPen:
		case EPhotoEditorCmdAntique:
		case EPhotoEditorCmdAutoLevels:
			ExecuteEffectL(aCommand);
			break;	
						
		case EPhotoEditorCmdBrightness:
		case EPhotoEditorCmdSharpness:
		case EPhotoEditorCmdContrast:
		case EPhotoEditorCmdEmboss:
		case EPhotoEditorCmdSolarize:
		case EPhotoEditorCmdPosterize:
		case EPhotoEditorCmdMoonlight:
		case EPhotoEditorCmdColorBoost:
		case EPhotoEditorCmdExposure:
			ApplyNumberInputEffectL(aCommand);
			break;	

		case EPhotoEditorCmdNoise:
		case EPhotoEditorCmdDespeckle:
		case EPhotoEditorCmdFrostedGlass:
		case EPhotoEditorCmdLocalBoost:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_38);
			ApplyModeInputEffectL(aCommand);
			break;	

		case EPhotoEditorCmdRotation:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_35);
			RotationL();
			break;	
		
		case EPhotoEditorCmdRgbColorAdjust:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_37);
			RgbColorAdjustL();
			break;	
		
		case EPhotoEditorCmdBlur:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_40);
			BlurL();
			break;	
			
		case EPhotoEditorCmdColorization:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_41);
			ColorizationL();
			break;		
			
		case EPhotoEditorCmdSpotlight:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_42);
			SpotlightL();
			break;	
	
		case EPhotoEditorCmdStamp:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_46);
			StampL();
			break;	
	
		case EPhotoEditorCmdSketch:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_49);
			SketchL();
			break;	
			
		case EPhotoEditorCmdCartoon:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_35);
			CartoonL();
			break;	
			
		case EPhotoEditorCmdCustomTransformation:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_51);
			CustomTransformationL();
			break;	
			
		case EPhotoEditorCmdLevels:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_55);
			LevelsL();
			break;
	
		case EPhotoEditorCmdWhiteBalance:
			iEikonEnv->InfoMsg(R_PROMPT_TEXT_57);
			WhiteBalanceL();
			break;
		}
	}

//
// Refreshes the screen.
//
void CTPhotoEditorAppUi::RefreshL(TBool aClearFirst)
	{
	ASSERT(iImageProcessor);
	ASSERT(iAppView);
	
	iAppView->RefreshMainPreviewL(aClearFirst);
	}

void CTPhotoEditorAppUi::SaveWorkingDirectoryL(const TDesC& aDir)
	{
	ASSERT(iRootDirectory);
	
	delete iRootDirectory;
	iRootDirectory = NULL;
	iRootDirectory = aDir.AllocL();
	}

//
// Opens File: File View
//
void CTPhotoEditorAppUi::OpenFileL()
	{
	QuerySaveChangesL();
	SwitchViewL(EViewFile);
	
	TFileName fn(*iRootDirectory);
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&fn);
	if (dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		SaveWorkingDirectoryL(fn);
		iFileView->LoadFileL(fn, iImageProcessor);
		}
	}

//
// Opens DRM File: File View
//
void CTPhotoEditorAppUi::OpenDRMFileL()
	{
	QuerySaveChangesL();
	SwitchViewL(EViewFile);
	
	iAppView->ClearScreen();
		    
	TFileName fn(*iRootDirectory);
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&fn);
	if (dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		SaveWorkingDirectoryL(fn);
		
		TMMFileSource drmFile(fn, KRightsUniqueId001, ContentAccess::EView);
		iFileView->LoadFileL(drmFile, iImageProcessor);
		}
	}

//
// Opens Gallery
//
void CTPhotoEditorAppUi::OpenGalleryL()
	{
	QuerySaveChangesL();
	SwitchViewL(EViewGallery);
	
	iAppView->ClearScreen();
	
	iFolderFiles.ResetAndDestroy();

	ASSERT(iRootDirectory);
	TFileName fn(*iRootDirectory);
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&fn);
	if (dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		SaveWorkingDirectoryL(fn);
		FilterFolderL(fn);
		iGalleryView->LoadGalleryL(iFolderFiles, *iImageProcessor);
		}
	}

//
// This function adds the supported files in a directory to an array.
//
void CTPhotoEditorAppUi::FilterFolderL(const TDesC& aDirName)
	{
	if (iViewType != EViewGallery)
		{
		ErrorMessage(_L("Not supported in this view"), KErrNotSupported);
		return;
		}

	iFolderFiles.ResetAndDestroy();
	
	CDir *dir = NULL;
	TParsePtrC parse(aDirName);
	TFileName dirName(parse.DriveAndPath());
	iEikonEnv->FsSession().GetDir(dirName, 0, 0, dir);
	CleanupStack::PushL(dir);
	
	if (dir->Count() == 0)
		{
		iEikonEnv->InfoMsg(R_GALLERY_EMPTY_FOLDER);
		CleanupStack::PopAndDestroy(dir);
		return;
		}
	
	TInt limit = Min(dir->Count(), KMaxNumberOfFilesInFolder);
	for (TInt i = 0; i < limit; i++)
		{
		const TEntry& entry = (*dir)[i];
		TFileName filename(dirName);
		filename.Append(entry.iName);
	
		HBufC* buf = filename.AllocLC();
		iFolderFiles.AppendL(buf);
		CleanupStack::Pop(buf);
		}
	
	CleanupStack::PopAndDestroy(dir);
	}

//
// Check whether user wants to save the changes applied to the current image.
//
void CTPhotoEditorAppUi::QuerySaveChangesL()
	{
	TBool canUndo = EFalse;
	TRAPD(err, canUndo = iImageProcessor->CanUndoL());	// I got tired of KErrNotReady messages.
	if (err != KErrNone)
		{
		return;
		}
	
	if (canUndo)
		{
		// Ask the user if they want to save changes.
		CConfirmInputDialog* dialog = new(ELeave) CConfirmInputDialog(R_CONFIRM_TEXT_TITLE);
		if (dialog->ExecuteLD(R_PHOTOEDITOR_SAVE_DIALOG))
			{
			SaveAsL();
			}
		
		iImageProcessor->UndoAllL();
		}
	if (isImageSaved)
		{
		isNavigate = EFalse;	
		}
	}

//
// Saves the image.
//
void CTPhotoEditorAppUi::SaveAsL()
	{
	// get output file name
	TFileName saveFileName(*iRootDirectory);
	CEikFileSaveAsDialog* dialog = new(ELeave) CEikFileSaveAsDialog(&saveFileName);
	if (!dialog->ExecuteLD(R_EIK_DIALOG_FILE_SAVEAS))
		{
		return;
		}
	
	TInt outputFormatIndex = 0;
	TSize size = iImageProcessor->CurrentSizeL();
	TInt originalWidth = size.iWidth;
	TInt originalHeight = size.iHeight;
	TInt width = 0;
	TInt height = 0;
	TInt jpegQuality = 100;
	TBool maintainAspectRatio = ETrue;
	
	// Get all supported output formats
	RArray<TUid> supportedOutputFormats;
	iImageProcessor->SupportedOutputFormatsL(supportedOutputFormats);
	
	// Assume the maximum number of output formats supported is 20.
	CDesCArrayFlat* outputFormatsName = new(ELeave) CDesCArrayFlat(20);
	CleanupStack::PushL(outputFormatsName);
	
	for (TInt i = 0; i < supportedOutputFormats.Count(); i++)
		{
		if (supportedOutputFormats[i] == KImageTypeBMPUid)
			{
			outputFormatsName->AppendL(KBMPFormat);
			}
		else if (supportedOutputFormats[i] == KImageTypeJPGUid)
			{
			outputFormatsName->AppendL(KJPGFormat);
			}
		else if (supportedOutputFormats[i] == KImageTypeGIFUid)
			{
			outputFormatsName->AppendL(KGIFFormat);
			}
		else if (supportedOutputFormats[i] == KImageTypePNGUid)
			{
			outputFormatsName->AppendL(KPNGFormat);
			}
		else if (supportedOutputFormats[i] == KImageTypeWBMPUid)
			{
			outputFormatsName->AppendL(KWBMPFormat);
			}
		else
			{
			outputFormatsName->AppendL(KCUSTOMFormat);
			}
		}
		
	CEncodeOptionInputDialog* encodeOptionsDialog = new(ELeave) CEncodeOptionInputDialog(outputFormatsName, outputFormatIndex, width, height, jpegQuality, maintainAspectRatio, originalWidth, originalHeight);
	TBool saveIt = encodeOptionsDialog->ExecuteLD(R_PHOTOEDITOR_ENCODEOPTIONS_DIALOG);
	CleanupStack::PopAndDestroy(outputFormatsName);
	if (!saveIt)
		{
		return;
		}
	
	iImageProcessor->SetOutputL(saveFileName, supportedOutputFormats[outputFormatIndex]);
		
	TSize destSize(width, height);
	TReal32 quality = (TReal32)jpegQuality / 100;
	ImageProcessor::TOutputInfo* outputInfo = iImageProcessor->OutputInfoL();
	outputInfo->SetJpegQualityL(quality);
	
	iProgressInfo = iImageProcessor->ProgressInfoL();
	iProgress = 0;	
	
	iImageProcessor->ProcessL(destSize, maintainAspectRatio);
	
	iImageProcessor->UndoAllL();
	
	if (IsGalleryView())
		{
		TFileName fn(*iRootDirectory);
		FilterFolderL(fn);
		ReSetImageProcessorL();
		iGalleryView->SetSavedFilesConfigurePreviewL(iFolderFiles,saveFileName);
		}
	isImageSaved = ETrue;		
	}

//
// Insert Frame to the image by TEffectBorder
//
void CTPhotoEditorAppUi::InsertFrameL()
	{
	using namespace ImageProcessor;
	
	TFileName fn(*iRootDirectory);
	
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&fn);
	TBool cancelled = !dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN);
	if (cancelled)
		{
		return;
		}
	
	SaveWorkingDirectoryL(fn);
	
	TEffectBorder* border = static_cast<TEffectBorder*>(iImageProcessor->EffectL(KEffectBorderUid));
	
	border->BeginL();
	border->SetInputL(fn);
	border->EndL();
	RefreshL();
	}

//Insert Clipart to the image by TEffectBorder
void CTPhotoEditorAppUi::InsertClipartL()
	{
	TFileName clipartFileName(KDefPath);
	
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&clipartFileName);
	TBool result =dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN);
	dialog = NULL;
	if (!result)
		return;
	
	using namespace ImageProcessor;
					
	TOverlay* overlay = iImageProcessor->OverlayL();
	
	//here use JPEG as input clipart format
	overlay->SetInputL(clipartFileName, (TUid) KImageTypeJPGUid);
	
	TInt maxWidth=iImageProcessor->CurrentSizeL().iWidth;
	TInt maxHeight=iImageProcessor->CurrentSizeL().iHeight;
	TInt aX=0, aY=0, scale=100, angle=0;
	
	CClipartInputDialog* parameterDialog = new(ELeave) CClipartInputDialog(scale, aX, aY, angle, maxWidth, maxHeight);
	if (!parameterDialog->ExecuteLD(R_PHOTOEDITOR_CLIPART_DIALOG))
		return; 
		
	TPoint clipartPosition(aX, aY);
		
	overlay->BeginL();
	overlay->SetL((TReal32)scale/100, (TReal32)scale/100, clipartPosition, (TReal32)angle);
	overlay->EndL();
	
	RefreshL();
	}


//Insert DRM clipart to the image by TEffectBorder
void CTPhotoEditorAppUi::InsertDRMClipartL()
	{
	TFileName DRMClipartFileName(KDefPath);
	
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&DRMClipartFileName);
	TBool result =dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN);
	dialog = NULL;
	if (!result)
		return;
	
	using namespace ImageProcessor;
		
	TOverlay* overlay = iImageProcessor->OverlayL();
	
	{
	using namespace ContentAccess;
	TMMFileSource fileSource(DRMClipartFileName, KRightsUniqueId001, EView);
	
	overlay->SetInputL(fileSource);
	}
	
	TInt maxWidth=iImageProcessor->CurrentSizeL().iWidth;
	TInt maxHeight=iImageProcessor->CurrentSizeL().iHeight;
	TInt aX=0, aY=0, scale=100, angle=0;
	
	CClipartInputDialog* parameterDialog = new(ELeave) CClipartInputDialog(scale, aX, aY, angle, maxWidth, maxHeight);
	if (!parameterDialog->ExecuteLD(R_PHOTOEDITOR_CLIPART_DIALOG))
		return; 
		
	TPoint clipartPosition(aX, aY);
		
	overlay->BeginL();
	overlay->SetL((TReal32)scale/100, (TReal32)scale/100, clipartPosition, (TReal32)angle);
	overlay->EndL();
	
	RefreshL();
	}

//Cancel current operation
void CTPhotoEditorAppUi::Cancel()
	{
	if (!iImageProcessor)
		{
		return;
		}
	
	iImageProcessor->Cancel();
	}

//
// Undo last applied effect
//
void CTPhotoEditorAppUi::UndoL()
	{
	TBool canUndo = EFalse;
	TRAP_IGNORE(canUndo = iImageProcessor->CanUndoL());
	
	if (canUndo)
		{
		iImageProcessor->UndoL();
		RefreshL();
		}
	}

//
// Undo all effect applied to current image.
//
void CTPhotoEditorAppUi::UndoAllL()
	{
	TBool canUndo = EFalse;
	TRAP_IGNORE(canUndo = iImageProcessor->CanUndoL());
	
	if (canUndo)
		{
		iImageProcessor->UndoAllL();
		RefreshL();
		}
	}

void CTPhotoEditorAppUi::DoNavigateL(TBool aFlag)
	{
	if (!isImageSaved)
		{
		QuerySaveChangesL();					
		}
	if(isNavigate)
		{
		iAppView->NavigateL(aFlag);											
		}

	isImageSaved = EFalse;
	isNavigate = ETrue;		
	}

void CTPhotoEditorAppUi::ReSetImageProcessorL()
	{
	iImageProcessor->ResetL();
	iImageProcessor->SetOptionsL(ImageProcessor::CImgProcessor::EOptionSyncProcessing);		
	}


void CTPhotoEditorAppUi::ReDrawAllPreviewL()
	{
	iGalleryView->ClearScreen();
	RPointerArray<TPreviewData> previews = iAppView->GetPreviewsL();
	for(TInt i = 0; i < previews.Count(); i++)
		{
		if (previews[i]->iFileName != NULL)
			{
			iAppView->BlitPreviewToScreenL(previews[i]->iPreviewId);				
			}
		else
			{
			iAppView->DrawRectangle(previews[i]->iLocation, previews[i]->iBitmap->SizeInPixels(), EFalse);				
			}	
		}

	}



//
// Switch between file view and gallery view
//
void CTPhotoEditorAppUi::SwitchViewL(TInt aViewType)
	{
	if (aViewType == EViewFile)
		{
		if (iAppView && (iAppView == iFileView))
			{
			return;
			}
		
		if (!iFileView)
			{
			iFileView = CTPhotoEditorFileView::NewL(ClientRect(), iScreenSize);
			}
		
		delete iGalleryView;
		iGalleryView = NULL;
		iAppView = iFileView;
		}
	else
		{
		if (iAppView && (iAppView == iGalleryView))
			{
			return;
			}
		
		if (!iGalleryView)
			{
			iGalleryView = CTPhotoEditorGalleryView::NewL(ClientRect(), iScreenSize);
			}
		
		delete iFileView;
		iFileView = NULL;
		iAppView = iGalleryView;
		}
	
	iViewType = TViewType(aViewType);
	QuerySaveChangesL();
	iAppView->ClearScreen();
	}

void CTPhotoEditorAppUi::ExecuteDialogL(CEikDialog* dialog, TInt aDialogId, ImageProcessor::TEffect& aEffect)
	{
	aEffect.BeginL();
	TBool cancelled = !dialog->ExecuteLD(aDialogId);
	aEffect.EndL();
	
	if (cancelled)
		{
		UndoL();
		}
	}


using namespace ImageProcessor;


template<class T>	// T is expected to be a pointer to a TEffect
void CTPhotoEditorAppUi::ApplyEffectL(TUid aUid, TInt aResourceId)
	{
	ASSERT(iImageProcessor);
	
	// Effects are applied the the main preview of whatever view is currently displaying.

	T effect = static_cast<T>(iImageProcessor->EffectL(aUid));
	effect->BeginL();
	effect->EndL();
	RefreshL();

	if (aResourceId != KErrNotFound)
		{
		iEikonEnv->InfoMsg(aResourceId);
		}
	}


void CTPhotoEditorAppUi::ApplyNumberInputEffectL(TInt aCommand)
	{
	using namespace ImageProcessor;
	
	aCommand -= EPhotoEditorFirstNumberEffect;
	__ASSERT_ALWAYS((aCommand >= 0) && (aCommand <= EPhotoEditorLastNumberEffect), User::Invariant());
	
	const TEffectEntry& entry = KNumberInputEffectTable[aCommand];
	
	TEffect* effect = iImageProcessor->EffectL(entry.iUid);

	CDesCArrayFlat* numberList = NULL;
		
	switch (effect->InputType())
			{
			case ImageProcessor::TEffect::EEffectInputTypeInt:
				{
				TInt min = 0;
				TInt max = 0;
				TInt step = 0;
						
				effect->LevelSettings(min, max, step);	
				if(step!=0)
					numberList = new(ELeave) CDesCArrayFlat((TInt)((max-min+1)/step));
				else
					numberList = new(ELeave) CDesCArrayFlat((TInt)(max-min+1));
				CleanupStack::PushL(numberList);
				
				FillParameterListL(*numberList, min, max);
				}
				break;
			case ImageProcessor::TEffect::EEffectInputTypeReal32:
				{
				TReal32 min = 0.0f;
				TReal32 max = 0.0f;
				TReal32 step = 0.0f;
						
				effect->LevelSettings(min, max, step);	
				if(step!=0)
					numberList = new(ELeave) CDesCArrayFlat((TInt)((max-min+1)/step));
				else
					numberList = new(ELeave) CDesCArrayFlat((TInt)((max-min)*100)+1);
				CleanupStack::PushL(numberList);
				
				FillParameterListL(*numberList, min*100, max*100);
				}
				break;
			default:
				User::Leave(KErrNotSupported);
			}	
				
	CNumberInputDialog* dialog = new(ELeave) CNumberInputDialog(*effect, *this, entry.iPromptId, numberList);
	ExecuteDialogL(dialog, entry.iDialogId, *effect);
	
	CleanupStack::PopAndDestroy(numberList);
	}


void CTPhotoEditorAppUi::ApplyModeInputEffectL(TInt aCommand)
	{
	using namespace ImageProcessor;
	
	aCommand -= EPhotoEditorFirstModeEffect;
	__ASSERT_ALWAYS((aCommand >= 0) && (aCommand <= EPhotoEditorLastModeEffect), User::Invariant());
	
	const TEffectEntry& entry = KModeInputEffectTable[aCommand];
	
	TEffect* effect = iImageProcessor->EffectL(entry.iUid);
	
	CModeInputDialog* dialog = new(ELeave) CModeInputDialog(*effect, entry.iPromptId, *this, R_PROMPT_TEXT_67);
	ExecuteDialogL(dialog, entry.iDialogId, *effect);
	}

void CTPhotoEditorAppUi::FillParameterListL(CDesCArrayFlat& aList, TInt aMin, TInt aMax)
	{
	for (TInt i = aMin; i <= aMax; i++)	
		{
		TBuf<8> temp;
		temp.AppendNum(i);
		aList.AppendL(temp);
		}
	}

//
// Cartoon Effect
//
void CTPhotoEditorAppUi::CartoonL()
	{
	using namespace ImageProcessor;

	TEffectCartoon* effect = static_cast<TEffectCartoon*>(iImageProcessor->EffectL(KEffectCartoonUid));
	
	CCartoonModeInputDialog* dialog = new(ELeave) CCartoonModeInputDialog(*effect, *this);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_CARTOON_DIALOG, *effect);
	}

//
// Rotation Effect
//
void CTPhotoEditorAppUi::RotationL()
	{
	using namespace ImageProcessor;

	TEffectRotation* effect = static_cast<TEffectRotation*>(iImageProcessor->EffectL(KEffectRotationUid));
	
	TReal32 min = 0.0f;
	TReal32 max = 0.0f;
	TReal32 step = 0.0f;
	effect->LevelSettings(min, max, step);
	
	CDesCArrayFlat* rotationAngleList;
	if(step!=0)
		rotationAngleList = new(ELeave) CDesCArrayFlat((TInt)((max-min+1)/step));
 	else
 		rotationAngleList = new(ELeave) CDesCArrayFlat((TInt)(max-min+1));

	CleanupStack::PushL(rotationAngleList);
		
	FillParameterListL(*rotationAngleList, min, max);
	
	CRotationInputDialog* dialog = new(ELeave) CRotationInputDialog(*effect, *this, rotationAngleList);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_ROTATION_DIALOG, *effect);
	
	CleanupStack::PopAndDestroy(rotationAngleList);
	}


//
// RgbColorAdjust Effect
//
void CTPhotoEditorAppUi::RgbColorAdjustL()
	{
	using namespace ImageProcessor;
	
	TEffectRgbColorAdjust* effect = static_cast<TEffectRgbColorAdjust*>(iImageProcessor->EffectL(KEffectRgbColorAdjustUid));

	CDesCArrayFlat* rgbFactorList = new(ELeave) CDesCArrayFlat(201);
	CleanupStack::PushL(rgbFactorList);
	
	FillParameterListL(*rgbFactorList, -100, 100);
			
	CRgbColorAdjustInputDialog* dialog = new(ELeave) CRgbColorAdjustInputDialog(*effect, *this, rgbFactorList);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_RGBCOLORADJUST_DIALOG, *effect);
	CleanupStack::PopAndDestroy(rgbFactorList);
	}

//
// Blur Effect
//
void CTPhotoEditorAppUi::BlurL()
	{
	using namespace ImageProcessor;
	
	TEffectBlur* effect = static_cast<TEffectBlur*>(iImageProcessor->EffectL(KEffectBlurUid));
	
	TSize size = iImageProcessor->CurrentSizeL();
	
	CDesCArrayFlat* axFactorList = new(ELeave) CDesCArrayFlat(size.iWidth+1);
	CleanupStack::PushL(axFactorList);
	
	CDesCArrayFlat* ayFactorList = new(ELeave) CDesCArrayFlat(size.iHeight+1);
	CleanupStack::PushL(ayFactorList);
	
	CDesCArrayFlat* bxFactorList = new(ELeave) CDesCArrayFlat(size.iWidth+1);
	CleanupStack::PushL(bxFactorList);
	
	CDesCArrayFlat* byFactorList = new(ELeave) CDesCArrayFlat(size.iHeight+1);
	CleanupStack::PushL(byFactorList);	
	
	FillParameterListL(*axFactorList, 0, size.iWidth);
	FillParameterListL(*ayFactorList, 0, size.iHeight);
	FillParameterListL(*bxFactorList, 0, size.iWidth);
	FillParameterListL(*byFactorList, 0, size.iHeight);
			
	CBlurInputDialog* dialog = new(ELeave) CBlurInputDialog(*effect, *this, axFactorList, ayFactorList, bxFactorList, byFactorList, size);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_BLUR_DIALOG, *effect);
	
	CleanupStack::PopAndDestroy(4);
	}

//
// Colorization Effect
//
void CTPhotoEditorAppUi::ColorizationL()
	{
	using namespace ImageProcessor;
	
	TEffectColorization* effect = static_cast<TEffectColorization*>(iImageProcessor->EffectL(KEffectColorizationUid));
	
	CDesCArrayFlat* levelFactorList = new(ELeave) CDesCArrayFlat(301);
	CleanupStack::PushL(levelFactorList);	
		
	CDesCArrayFlat* rgbFactorList = new(ELeave) CDesCArrayFlat(256);
	CleanupStack::PushL(rgbFactorList);
	
	FillParameterListL(*levelFactorList, -100, 200);
	FillParameterListL(*rgbFactorList, 0, 255);
	
	CColorizationInputDialog* dialog = new(ELeave) CColorizationInputDialog(*effect, *this, levelFactorList, rgbFactorList);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_COLORIZATION_DIALOG, *effect);
	
	CleanupStack::PopAndDestroy(2);
	}

//
// Spotlight Effect
//
void CTPhotoEditorAppUi::SpotlightL()
	{
	using namespace ImageProcessor;
	
	TEffectSpotLight* effect = static_cast<TEffectSpotLight*>(iImageProcessor->EffectL(KEffectSpotLightUid));
	
	TSize size = iImageProcessor->CurrentSizeL();
	
	CDesCArrayFlat* xFactorList = new(ELeave) CDesCArrayFlat(size.iWidth+1);
	CleanupStack::PushL(xFactorList);	
	
	CDesCArrayFlat* yFactorList = new(ELeave) CDesCArrayFlat(size.iHeight+1);
	CleanupStack::PushL(yFactorList);	
	
	CDesCArrayFlat* radiusFactorList = new(ELeave) CDesCArrayFlat(Max(size.iWidth, size.iHeight)+1);
	CleanupStack::PushL(radiusFactorList);	
	
	CDesCArrayFlat* fractionFactorList = new(ELeave) CDesCArrayFlat(101);
	CleanupStack::PushL(fractionFactorList);	
	
	FillParameterListL(*xFactorList, 0, size.iWidth);
	FillParameterListL(*yFactorList, 0, size.iHeight);
	FillParameterListL(*radiusFactorList, 0, Max(size.iWidth, size.iHeight));
	FillParameterListL(*fractionFactorList, 0, 100);
	
	CSpotLightInputDialog* dialog = new(ELeave) CSpotLightInputDialog(*effect, *this, xFactorList, yFactorList, radiusFactorList, fractionFactorList, size);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_SPOTLIGHT_DIALOG, *effect);
	
	CleanupStack::PopAndDestroy(4);
	}

//
// Stamp Effect
//
void CTPhotoEditorAppUi::StampL()
	{
	using namespace ImageProcessor;

	TEffectStamp* effect = static_cast<TEffectStamp*>(iImageProcessor->EffectL(KEffectStampUid));
	
	CDesCArrayFlat* smoothFactorList = new(ELeave) CDesCArrayFlat(7);
	CleanupStack::PushL(smoothFactorList);	
		
	CDesCArrayFlat* thresholdFactorList = new(ELeave) CDesCArrayFlat(256);
	CleanupStack::PushL(thresholdFactorList);	
	
	FillParameterListL(*smoothFactorList, 0, 6);
	FillParameterListL(*thresholdFactorList, 0, 255);
	
	CStampInputDialog* dialog = new(ELeave) CStampInputDialog(*effect, *this, smoothFactorList, thresholdFactorList);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_STAMP_DIALOG, *effect);
	
	CleanupStack::PopAndDestroy(2);
	}

//
// Sketch Effect.
//
void CTPhotoEditorAppUi::SketchL()
	{
	using namespace ImageProcessor;
	
	TEffectSketch* effect = static_cast<TEffectSketch*>(iImageProcessor->EffectL(KEffectSketchUid));	
	
	CSketchModeInputDialog* dialog = new(ELeave) CSketchModeInputDialog(*effect, *this);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_SKETCH_DIALOG, *effect);
	}

//
// CustomTransformation Effect
//
void CTPhotoEditorAppUi::CustomTransformationL()
	{
	using namespace ImageProcessor;
	
	TEffectCustomTransformation* effect = static_cast<TEffectCustomTransformation*>(
			iImageProcessor->EffectL(KEffectCustomTransformationUid));		
	
	TFileName fn(*iRootDirectory);
	
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&fn);
	if (dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		SaveWorkingDirectoryL(fn);
		
		effect->BeginL();
		effect->SetInputL(fn);
		effect->EndL();
		RefreshL();
		}
	}

//
// WhiteBalance Effect
//
void CTPhotoEditorAppUi::WhiteBalanceL()
	{
	using namespace ImageProcessor;

	TEffectWhiteBalance* effect = static_cast<TEffectWhiteBalance*>(iImageProcessor->EffectL(KEffectWhiteBalanceUid));
	
	CDesCArrayFlat* rgbFactorList = new(ELeave) CDesCArrayFlat(256);
	CleanupStack::PushL(rgbFactorList);
	FillParameterListL(*rgbFactorList, 0, 255);
		
	CWhiteBalanceInputDialog* dialog = new(ELeave) CWhiteBalanceInputDialog(*effect, *this, rgbFactorList);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_WHITEBALANCE_DIALOG, *effect);
	CleanupStack::PopAndDestroy(rgbFactorList);
	}

//
// Levels Effect
//
void CTPhotoEditorAppUi::LevelsL()
	{
	using namespace ImageProcessor;
	
	TEffectLevels* effect = static_cast<TEffectLevels*>(iImageProcessor->EffectL(KEffectLevelsUid));

	CDesCArrayFlat* levelFactorList = new(ELeave) CDesCArrayFlat(101);
	CleanupStack::PushL(levelFactorList);
	FillParameterListL(*levelFactorList, 0, 100);
		
	CLevelsInputDialog* dialog = new(ELeave) CLevelsInputDialog(*effect, *this, levelFactorList);
	ExecuteDialogL(dialog, R_PHOTOEDITOR_LEVELS_DIALOG, *effect);
	CleanupStack::PopAndDestroy(levelFactorList);
	}


/**
 *	Handles system events
 *	@param aEvent - event that has to be handled by the function
 *	
 */  
void CTPhotoEditorAppUi::HandleSystemEventL(const TWsEvent& aEvent)
	{
	switch (*(TApaSystemEvent*)(aEvent.EventData()))
		{
		case EApaSystemEventBroughtToForeground:
			RProcess::Rendezvous(KErrNone);
			break;

		default:
			break;
		}	
	}

/**
 *	Constructor that constructs a new document.
 *	@param aApp - The application instance that is creating the document. 
 *	
 */  
CTPhotoEditorDocument::CTPhotoEditorDocument(CEikApplication& aApp)
 :	CEikDocument(aApp)
	{
	}
	
/**
 *	Constructs the application UI
 *	@return A partially-constructed application UI object. 
 *	
 */  
CEikAppUi* CTPhotoEditorDocument::CreateAppUiL()
	{
    return new(ELeave) CTPhotoEditorAppUi;
	}

/**
 *	Gets the application specific UID.
 *	@return - UID of the TPhotoEditor application
 *	
 */  
TUid CTPhotoEditorApplication::AppDllUid() const
	{
	return KUidTPhotoEditor;
	}
	
/**
 *	Creates a document object for the application
 *	@return pointer to the newly created document
 *	
 */  
CApaDocument* CTPhotoEditorApplication::CreateDocumentL()
	{
	return new(ELeave) CTPhotoEditorDocument(*this);
	}

/**
 *	Creates and returns the object of the application
 *	@return pointer to the newly created application
 *	
 */  
LOCAL_C CApaApplication* NewApplication()
	{
	return new CTPhotoEditorApplication();
	}

/**
 *	Entry point for the application
 *	
 */  
GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	TInt err = EikStart::RunApplication(NewApplication);
	__UHEAP_MARKEND;
	return err;
	}
	

