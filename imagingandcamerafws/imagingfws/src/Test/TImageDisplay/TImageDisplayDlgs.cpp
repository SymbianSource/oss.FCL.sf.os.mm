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
//

#include <techview/eikon.rsg>
#include <techview/eikchlst.h>
#include "plugininfoarray.h"

#include "TImageDisplayDlgs.h"
//
// CDisplayModeDialog
//

CDisplayModeDialog::CDisplayModeDialog(TDisplayMode& aDisplayMode):
	iDisplayMode(aDisplayMode)
	{
	}

const TInt KNumDispMode = 10;
const TDisplayMode KDispMode[KNumDispMode] = 
	{ EGray2, EGray4, EGray16, EGray256, EColor16, 
		EColor256, EColor4K, EColor64K, EColor16M , EColor16MU 
	};

void CDisplayModeDialog::PreLayoutDynInitL()
	{
	TInt choice = -1;

	for (TInt index=0; index<KNumDispMode; index++)
		{
		if (KDispMode[index]==iDisplayMode)
			{
			choice = index;
			break;
			}
		}

	ASSERT(choice>=0); // should always match something
	SetChoiceListCurrentItem(EVideoIdDisplayMode,choice);
	}

TBool CDisplayModeDialog::OkToExitL(TInt /*aButtonId*/)
	{
	const TInt chosenIndex = ChoiceListCurrentItem(EVideoIdDisplayMode);
	ASSERT(chosenIndex<KNumDispMode);
	iDisplayMode = KDispMode[chosenIndex];
	return ETrue;
	}


CMaxReductionDialog::CMaxReductionDialog(TInt& aMaxReduction ):iMaxReduction(aMaxReduction)
	{
	}

void CMaxReductionDialog::PreLayoutDynInitL()
	{
	SetNumberEditorValue(EImageMaxAllowedReduction, iMaxReduction);
	}

TBool CMaxReductionDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iMaxReduction = NumberEditorValue(EImageMaxAllowedReduction);
	return ETrue;
	}

CScalingQualityDialog::CScalingQualityDialog(TInt& aQualityValueRef):
																	iQualityValueRef(aQualityValueRef)
	{
	}
	
void CScalingQualityDialog::PreLayoutDynInitL()
	{
	ASSERT(iQualityValueRef>=0); // should always match something
	SetChoiceListCurrentItem(EImageScalingQuality, iQualityValueRef);
	}

TBool CScalingQualityDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iQualityValueRef = ChoiceListCurrentItem(EImageScalingQuality);
	return ETrue;
	}

CDrmContentIdDialog::CDrmContentIdDialog(TDes& aContentId):
												iContentId(aContentId)
	{
	}

void CDrmContentIdDialog::PreLayoutDynInitL()
	{
	SetEdwinTextL(EImageDrmContentId, &iContentId);
	}

TBool CDrmContentIdDialog::OkToExitL(TInt /*aButtonId*/)
	{
	GetEdwinText(iContentId, EImageDrmContentId);
	return ETrue;
	}

CClipRectParamsDialog::CClipRectParamsDialog(TBool& aEnabled, TRect& aClipRect):
												iEnabled(aEnabled), iClipRect(aClipRect)
	{
	}

void CClipRectParamsDialog::PreLayoutDynInitL()
	{
	SetCheckBoxState(EImageUseClipRect, CEikButtonBase::TState(iEnabled));
	SetNumberEditorValue(EImageClipRectTlx,iClipRect.iTl.iX);
	SetNumberEditorValue(EImageClipRectTly,iClipRect.iTl.iY);
	SetNumberEditorValue(EImageClipRectBrx,iClipRect.iBr.iX);
	SetNumberEditorValue(EImageClipRectBry,iClipRect.iBr.iY);
	HandleControlStateChangeL(EImageUseClipRect);
	}

TBool CClipRectParamsDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iEnabled = CheckBoxState(EImageUseClipRect);
	iClipRect.iTl.iX = NumberEditorValue(EImageClipRectTlx);
	iClipRect.iTl.iY = NumberEditorValue(EImageClipRectTly);
	iClipRect.iBr.iX = NumberEditorValue(EImageClipRectBrx);
	iClipRect.iBr.iY = NumberEditorValue(EImageClipRectBry);
	return ETrue;
	}

void CClipRectParamsDialog::HandleControlStateChangeL(TInt aControlId)
	{
	if (aControlId != EImageUseClipRect)
		{
		return;
		}
	const TBool disabled=(CEikButtonBase::EClear==CheckBoxState(EImageUseClipRect));
	SetLineDimmedNow(EImageClipRectTlx,disabled);
	SetLineDimmedNow(EImageClipRectTly,disabled);
	SetLineDimmedNow(EImageClipRectBrx,disabled);
	SetLineDimmedNow(EImageClipRectBry,disabled);
	}
//
// CVideoBackgroundColorDialog
//

CVideoBackgroundColorDialog::CVideoBackgroundColorDialog(TInt& aColor16, TBool& aOverride):
	iColor16(aColor16), iOverride(aOverride)
	{
	}

void CVideoBackgroundColorDialog::PreLayoutDynInitL()
	{
	CEikButtonBase::TState state = iOverride ? CEikButtonBase::ESet : CEikButtonBase::EClear;
	SetCheckBoxState(EVideoIdOverrideBackgroundColor, state);
	SetChoiceListCurrentItem(EVideoIdBackgroundColor,iColor16);
	if (!iOverride)
		SetLineDimmedNow(EVideoIdBackgroundColor,ETrue);
	}

TBool CVideoBackgroundColorDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iColor16 = ChoiceListCurrentItem(EVideoIdBackgroundColor);
	CEikButtonBase::TState state = CheckBoxState(EVideoIdOverrideBackgroundColor);
	iOverride = state != CEikButtonBase::EClear;
	return ETrue;
	}

void CVideoBackgroundColorDialog::HandleControlStateChangeL(TInt aControlId)
	{
	if (aControlId == EVideoIdOverrideBackgroundColor)
		{
		CEikButtonBase::TState state = CheckBoxState(EVideoIdOverrideBackgroundColor);
		TBool isSet = state != CEikButtonBase::EClear;
		SetLineDimmedNow(EVideoIdBackgroundColor,!isSet);
		}
	}

//
// CVideoCurrentFrameDialog
//

CVideoCurrentFrameDialog::CVideoCurrentFrameDialog(TInt& aCurrentFrame,TInt aNumberOfFrames):
	iCurrentFrame(aCurrentFrame),
	iNumberOfFrames(aNumberOfFrames)
	{}

void CVideoCurrentFrameDialog::PreLayoutDynInitL()
	{
	SetNumberEditorValue(EVideoIdNumberOfFrames,iNumberOfFrames);
	SetLineDimmedNow(EVideoIdNumberOfFrames,ETrue);
	const TInt lastFrame = iNumberOfFrames - 1;
	SetNumberEditorMinAndMax(EVideoIdCurrentFrameNumber,0,lastFrame);
	SetNumberEditorValue(EVideoIdCurrentFrameNumber,Min(iCurrentFrame,lastFrame));
	}

TBool CVideoCurrentFrameDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iCurrentFrame = NumberEditorValue(EVideoIdCurrentFrameNumber);
	return ETrue;
	}

//
// CVideoSaveAsDialog
//

CVideoSaveAsDialog::CVideoSaveAsDialog(TDes* aFileName,TFileSaveInfo& aSaveInfo):
	CEikFileSaveAsDialog(aFileName),
	iSaveInfo(aSaveInfo)
	{}

void CVideoSaveAsDialog::PreLayoutDynInitL()
	{
	SetTypeL();
	CEikFileSaveAsDialog::PreLayoutDynInitL();
	}

void CVideoSaveAsDialog::SetTypeL()
	{
	const TUid imageType = iSaveInfo.iImageTypeUid;

	if(imageType == KImageTypeBMPUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_BMP);
	else if(imageType == KImageTypeGIFUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_GIF);
	else if(imageType == KImageTypeJPGUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_JPEG);
	else if(imageType == KImageTypeMBMUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_MBM);
	else if(imageType == KImageTypePNGUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_PNG);
	else
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_CUSTOM);

	}

TBool CVideoSaveAsDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EVideoIdSaveAsFormat)
		{
		CEikDialog* dialog = new(ELeave) CVideoFormatDialog(iSaveInfo);
		if (dialog->ExecuteLD(R_VIDEO_FILE_FORMAT_DIALOG))
			SetTypeL();

		return EFalse;
		}

	return CEikFileSaveAsDialog::OkToExitL(aButtonId);
	}

//
// CVideoFormatDialog
//

CVideoFormatDialog::CVideoFormatDialog(TFileSaveInfo& aSaveInfo):
	iSaveInfo(aSaveInfo)
	{}

void CVideoFormatDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* formatList = STATIC_CAST(CEikChoiceList*,Control(EVideoIdFileFormatType));

	//Add the available encoders to the dialog
	iEncoderList = CPluginInfoArray::NewL();
	formatList->SetArrayL(iEncoderList); //ownership of iEncoderList passed to dialog

	const TInt noOfEncoders = iEncoderList->MdcaCount();
	if(noOfEncoders == 0)
		User::Leave(KErrNotFound);
	
	//Find the index for the selected encoder (via ImageTypeUid)
	//if it is not found use the first
	TInt index;
	for(index=noOfEncoders-1; index > 0; index--)
		{
		if(iEncoderList->ImageType(index) == iSaveInfo.iImageTypeUid)
			break;
		}
	iSaveInfo.iImageTypeUid = iEncoderList->ImageType(index);

	SetChoiceListCurrentItem(EVideoIdFileFormatType,index);
	SetChoiceListCurrentItem(EVideoIdFileFormatBpp,iSaveInfo.iBpp);
	SetChoiceListCurrentItem(EVideoIdFileFormatColor,iSaveInfo.iColor);
	SetNumberEditorValue(EVideoIdFileFormatFactor,iSaveInfo.iQualityFactor);
	SetChoiceListCurrentItem(EVideoIdFileFormatSampling,iSaveInfo.iSampling);
	SetChoiceListCurrentItem(EVideoIdFileFormatCompression,iSaveInfo.iCompression);

	ValidateControlState();
	}

void CVideoFormatDialog::HandleControlStateChangeL(TInt /*aControlId*/)
	{
	ValidateControlState();
	}

void CVideoFormatDialog::ValidateControlState()
	{
	TInt type = ChoiceListCurrentItem(EVideoIdFileFormatType);
	TInt bpp = ChoiceListCurrentItem(EVideoIdFileFormatBpp);
	TInt color = ChoiceListCurrentItem(EVideoIdFileFormatColor);
	TInt compression = ChoiceListCurrentItem(EVideoIdFileFormatCompression);

	TBool bppVisible = ETrue;
	TBool colorVisible = ETrue;
	TBool factorVisible = EFalse;
	TBool samplingVisible = EFalse;
	TBool compressionVisible = EFalse;


	const TUid imageType = iEncoderList->ImageType(type);

	if(imageType == KImageTypeBMPUid)
		{
		colorVisible = EFalse;
		if (bpp == 1)
			bpp = 2;
		else if ((bpp > 3) && (bpp < 6))
			bpp = 6;
		else if (bpp > 6)
			bpp = 0;
		}
	else if(imageType == KImageTypeGIFUid)
		{
		bppVisible = EFalse;
		colorVisible = EFalse;
		}
	else if(imageType == KImageTypeJPGUid)
		{
		bppVisible = EFalse;
		factorVisible = ETrue;
		if (color)
			samplingVisible = ETrue;
		}
	else if(imageType == KImageTypeMBMUid)
		{
		if (color == 0)
			{
			if (bpp > 3)
				bpp = 0;
			}
		else
			{
			if (bpp < 2)
				bpp = 2;
			}
		}
	else if(imageType == KImageTypePNGUid)
		{
		if ((bpp > 3) && (bpp < 6))
			bpp = 6;
		else if (bpp > 6)
			bpp = 0;
		compressionVisible = ETrue;
		}
	else //Custom encoder
		{
		bppVisible = EFalse;
		colorVisible = EFalse;
		}

	SetChoiceListCurrentItem(EVideoIdFileFormatBpp,bpp);
	SetChoiceListCurrentItem(EVideoIdFileFormatColor,color);
	SetChoiceListCurrentItem(EVideoIdFileFormatCompression,compression);

	MakeLineVisible(EVideoIdFileFormatBpp,bppVisible);
	MakeLineVisible(EVideoIdFileFormatColor,colorVisible);
	MakeLineVisible(EVideoIdFileFormatFactor,factorVisible);
	MakeLineVisible(EVideoIdFileFormatSampling,samplingVisible);
	MakeLineVisible(EVideoIdFileFormatCompression,compressionVisible);
	}

TBool CVideoFormatDialog::OkToExitL(TInt /*aButtonId*/)
	{
	TInt type = ChoiceListCurrentItem(EVideoIdFileFormatType);
	iSaveInfo.iImageTypeUid = iEncoderList->ImageType(type);
	iSaveInfo.iBpp = ChoiceListCurrentItem(EVideoIdFileFormatBpp);
	iSaveInfo.iColor = ChoiceListCurrentItem(EVideoIdFileFormatColor);
	iSaveInfo.iQualityFactor = NumberEditorValue(EVideoIdFileFormatFactor);
	iSaveInfo.iSampling = ChoiceListCurrentItem(EVideoIdFileFormatSampling);
	iSaveInfo.iCompression = ChoiceListCurrentItem(EVideoIdFileFormatCompression);

	return ETrue;
	}
