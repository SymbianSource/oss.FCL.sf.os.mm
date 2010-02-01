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

#include "tphotoeditor.h"
#include "tphotoeditor.hrh"
#include "tphotoeditordialogs.h"

//Message Dialog    
//Constructor
CMessageDialog::CMessageDialog()
	{
	}

CMessageDialog* CMessageDialog::NewL(TDesC& aMessage)
	{
	CMessageDialog* self = new(ELeave) CMessageDialog();
	CleanupStack::PushL(self);
	self->ConstructL(aMessage);
	CleanupStack::Pop();	// self
	return self;
	}

void CMessageDialog::ConstructL(TDesC& aMessage)
	{
	iMessage = aMessage.AllocL();
	}

CMessageDialog::~CMessageDialog()
	{
	delete iMessage;
	}

// Initializes layout
void CMessageDialog::PreLayoutDynInitL()
	{
	SetTitleL(*iMessage);
	} 

//Handles a dialog button press for the OK button
TBool CMessageDialog::OkToExitL(TInt /*aButtonId*/)
    {
    return ETrue;
    }

// Input dialog to confirm whether save the applied effect or not    
// Constructor
CConfirmInputDialog::CConfirmInputDialog(TInt aTitleId)
:	iTitleId(aTitleId)
	{
	}

// Initializes layout
void CConfirmInputDialog::PreLayoutDynInitL()
	{
	SetTitleL(iTitleId);
	} 

// Handles a dialog button press for the OK button
// Sets parameter value
TBool CConfirmInputDialog::OkToExitL(TInt /*aButtonId*/)
    {
    return ETrue;
    }


// Input dialog to get encode options 
CEncodeOptionInputDialog::CEncodeOptionInputDialog(CDesCArrayFlat* aOutputFormatList, TInt& aOutputFormatIndex, TInt& aWidth, TInt& aHeight, TInt& aJpegQuality, TBool& aMaintainAspectRatio, TInt& aOriginalWidth, TInt& aOriginalHeight)
:	iOutputFormatList(aOutputFormatList),
	iOutputFormatIndex(aOutputFormatIndex),
	iWidth(aWidth),
	iHeight(aHeight),
	iJpegQuality(aJpegQuality),
	iMaintainAspectRatio(aMaintainAspectRatio),
	iOriginalWidth(aOriginalWidth),
	iOriginalHeight(aOriginalHeight)
	{
	}

//initializes layout
void CEncodeOptionInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* formatList = STATIC_CAST(CEikChoiceList*, Control(EEncodeOptionOutputFormat));
	formatList->SetArrayExternalOwnership(ETrue);
	
	//Add the available output format to the dialog
	formatList->SetArrayL(iOutputFormatList); 

	static_cast<CEikNumberEditor*>(Control(EEncodeOptionWidth))->SetNumber(iOriginalWidth);
	static_cast<CEikNumberEditor*>(Control(EEncodeOptionHeight))->SetNumber(iOriginalHeight);
	static_cast<CEikNumberEditor*>(Control(EEncodeOptionJpegQuality))->SetNumber(iJpegQuality);
	SetChoiceListCurrentItem(EEncodeOptionMaintainAspectRatio, iMaintainAspectRatio);
	} 
	
//Handles a dialog button press for the OK button
TBool CEncodeOptionInputDialog::OkToExitL(TInt /*aButtonId*/)
    {
    iOutputFormatIndex = ChoiceListCurrentItem(EEncodeOptionOutputFormat);
    
    iWidth = NumberEditorValue(EEncodeOptionWidth);
    iHeight = NumberEditorValue(EEncodeOptionHeight);
    iJpegQuality = NumberEditorValue(EEncodeOptionJpegQuality);
    iMaintainAspectRatio = ChoiceListCurrentItem(EEncodeOptionMaintainAspectRatio);
    return ETrue;
    }


//
// Input dialog to get number parameters for effects.
//
CNumberInputDialog::CNumberInputDialog(ImageProcessor::TEffect& aEffect, CTPhotoEditorAppUi& aUi, TInt aTitleId, CDesCArrayFlat* aNumberList)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iTitleId(aTitleId),
 	iNumberList(aNumberList)
	{
	}

void CNumberInputDialog::PreLayoutDynInitL()
	{
	SetTitleL(iTitleId);
	
	CEikChoiceList* numberList = STATIC_CAST(CEikChoiceList*, Control(EEffectLevel));
	numberList->SetArrayExternalOwnership(ETrue);
	numberList->SetArrayL(iNumberList); 
		
	switch (iEffect.InputType())
		{
		case ImageProcessor::TEffect::EEffectInputTypeInt:
			{
			TInt min = 0;
			TInt max = 0;
			TInt step = 0;
												
			iEffect.LevelSettings(min, max, step);
			TInt level = 0;
			iEffect.LevelL(level);
			
			SetChoiceListCurrentItem(EEffectLevel, (level-min));
							
			iEffect.SetLevelL(level);
			}
			break;
		
		case ImageProcessor::TEffect::EEffectInputTypeReal32:
			{
			TReal32 min = 0.0f;
			TReal32 max = 0.0f;
			TReal32 step = 0.0f;
											
			iEffect.LevelSettings(min, max, step);	
			TReal32 level = 0.0f;
			iEffect.LevelL(level);

			SetChoiceListCurrentItem(EEffectLevel, ((TInt)((level-min)*100)));
						
			iEffect.SetLevelL(level);
			}
			break;
			
		default:
			User::Leave(KErrNotSupported);
		}
	
	iUi.RefreshL();
	} 

void CNumberInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/) 
	{
	switch (iEffect.InputType())
		{
		case ImageProcessor::TEffect::EEffectInputTypeInt:
			{
			TInt min = 0;
			TInt max = 0;
			TInt step = 0;
									
			iEffect.LevelSettings(min, max, step);
			iEffect.SetLevelL(ChoiceListCurrentItem(EEffectLevel)+ min);
			}
			break;
			
		case ImageProcessor::TEffect::EEffectInputTypeReal32:
			{
			TReal32 min = 0.0f;
			TReal32 max = 0.0f;
			TReal32 step = 0.0f;
									
			iEffect.LevelSettings(min, max, step);	
			
			TReal32 level = TReal32((ChoiceListCurrentItem(EEffectLevel)+ min*100) / 100.0f);
			iEffect.SetLevelL(level);
			}
			break;
			
		default:
			User::Leave(KErrNotSupported);
		}
	
	iUi.RefreshL();
	}


//
// Input dialog to get Mode(enumeration) parameters for effects.
//
CModeInputDialog::CModeInputDialog(ImageProcessor::TEffect& aEffect, TInt aEffectChoice, CTPhotoEditorAppUi& aUi, TInt aTitleId)
 :	iEffect(aEffect),
 	iEffectChoice(aEffectChoice),
 	iUi(aUi),
 	iTitleId(aTitleId)
	{
	}

//
// Initializes layout.
//
void CModeInputDialog::PreLayoutDynInitL()
	{
	SetTitleL(iTitleId);

	TInt level = 0;
	iEffect.LevelL(level);
	iEffect.SetLevelL(level);
	
	iUi.RefreshL();
	} 

void CModeInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TInt mode = ChoiceListCurrentItem(iEffectChoice) + 1;
	
	iEffect.SetLevelL(mode);
	iUi.RefreshL();
	}



CSketchModeInputDialog::CSketchModeInputDialog(ImageProcessor::TEffect& aEffect, CTPhotoEditorAppUi& aUi)
 :	CModeInputDialog(aEffect, EEffectSketchMode, aUi, R_SKETCH_TEXT_TITLE)
	{
	}

void CSketchModeInputDialog::CurrentSketchLevelApplyL()
	{
	using namespace ImageProcessor;
		
	TInt intMode = ChoiceListCurrentItem(iEffectChoice);
	
	TEffectSketch::TSketchMode mode = static_cast<TEffectSketch::TSketchMode>(intMode);
	static_cast<ImageProcessor::TEffectSketch&>(iEffect).SetSketchModeL(mode);
	
	iUi.RefreshL();
	}
//
// Initializes layout.
//
void CSketchModeInputDialog::PreLayoutDynInitL()
	{
	SetTitleL(iTitleId);
	
	CurrentSketchLevelApplyL();
	} 

void CSketchModeInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	CurrentSketchLevelApplyL();
	}


//Input dialog to get Cartoon effect parameters  
CCartoonModeInputDialog::CCartoonModeInputDialog(ImageProcessor::TEffect& aEffect, CTPhotoEditorAppUi& aUi)
 :	CModeInputDialog(aEffect, EEffectCartoonMode, aUi, R_CARTOON_TEXT_TITLE)
	{
	}


void CCartoonModeInputDialog::CurrentCartoonLevelApplyL()
	{
	using namespace ImageProcessor;
	
	TInt intMode = ChoiceListCurrentItem(iEffectChoice);
	
	TBool mode = static_cast<TBool>(intMode);
	static_cast<ImageProcessor::TEffectCartoon&>(iEffect).SetDistinctEdgesL(mode);
	
	iUi.RefreshL();
	} 

//
// Initializes layout.
//
void CCartoonModeInputDialog::PreLayoutDynInitL()
	{
	SetTitleL(iTitleId);
	
	CurrentCartoonLevelApplyL();
	} 

void CCartoonModeInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	CurrentCartoonLevelApplyL();
	}



//Input dialog to get Insert Clipart effect parameters  
CClipartInputDialog::CClipartInputDialog(TInt& aScale, TInt& aAx, TInt& aAy, TInt& aAngle, TInt& aMaxWidth, TInt& aMaxHeight)
 :	iScale(aScale),
 	iAx(aAx),
 	iAy(aAy),
 	iAngle(aAngle),
 	iMaxWidth(aMaxWidth),
 	iMaxHeight(aMaxHeight)
	{
	}

//initializes layout
void CClipartInputDialog::PreLayoutDynInitL()
	{
	SetNumberEditorMinAndMax(EEffectClipartAx, 0, iMaxWidth);
	SetNumberEditorMinAndMax(EEffectClipartAy, 0, iMaxHeight);
	static_cast<CEikNumberEditor*>(Control(EEffectClipartScale))->SetNumber(iScale);
	static_cast<CEikNumberEditor*>(Control(EEffectClipartAx))->SetNumber(iAx);
	static_cast<CEikNumberEditor*>(Control(EEffectClipartAy))->SetNumber(iAy);
	static_cast<CEikNumberEditor*>(Control(EEffectClipartAngle))->SetNumber(iAngle);
	} 

//Handles a dialog button press for the OK button
//Sets parameter value
TBool CClipartInputDialog::OkToExitL(TInt /*aButtonId*/)
    {
    iScale = NumberEditorValue(EEffectClipartScale);
    iAx = NumberEditorValue(EEffectClipartAx);
    iAy = NumberEditorValue(EEffectClipartAy);
    iAngle = NumberEditorValue(EEffectClipartAngle);
    return ETrue;
    }



//
// Input dialog to get rotation effect parameters.
//
CRotationInputDialog::CRotationInputDialog(ImageProcessor::TEffectRotation& aRotation, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aRotationAngleList)
 :	iRotation(aRotation),
 	iUi(aUi),
 	iRotationAngleList(aRotationAngleList)
	{
	}


void CRotationInputDialog::PreLayoutDynInitL()
	{
	using namespace ImageProcessor;
	
	CEikChoiceList* angleList = STATIC_CAST(CEikChoiceList*, Control(EEffectRotationAngle));
	angleList->SetArrayExternalOwnership(ETrue);
	angleList->SetArrayL(iRotationAngleList); 
	
	SetChoiceListCurrentItem(EEffectRotationAngle, KDefaultRotationAngle);
	SetChoiceListCurrentItem(EEffectRotationMode, iRotation.ScaleModeL());
	
	iRotation.SetRotationL(static_cast<TEffectRotation::TRotationScaleMode>(iRotation.ScaleModeL()), KDefaultRotationAngle);
	iUi.RefreshL(ETrue);
	}
	
void CRotationInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	using namespace ImageProcessor;
	
	TInt newAngle = ChoiceListCurrentItem(EEffectRotationAngle);
	TInt newMode = ChoiceListCurrentItem(EEffectRotationMode);
	
	iRotation.SetRotationL(static_cast<TEffectRotation::TRotationScaleMode>(newMode), TReal32(newAngle));
	iUi.RefreshL(ETrue);
	}


//
// Input dialog to get RgbColorAdjust effect parameters.
//
CRgbColorAdjustInputDialog::CRgbColorAdjustInputDialog(ImageProcessor::TEffectRgbColorAdjust& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aRgbFactorList)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iRgbFactorList(aRgbFactorList)
	{	
	}

//
// Initializes layout.
//
void CRgbColorAdjustInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* redFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectRgbColorAdjustRed));
	redFactorList->SetArrayExternalOwnership(ETrue);
	redFactorList->SetArrayL(iRgbFactorList); 
	
	CEikChoiceList* greenFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectRgbColorAdjustGreen));
	greenFactorList->SetArrayExternalOwnership(ETrue);
	greenFactorList->SetArrayL(iRgbFactorList); 
		
	CEikChoiceList* blueFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectRgbColorAdjustBlue));
	blueFactorList->SetArrayExternalOwnership(ETrue);
	blueFactorList->SetArrayL(iRgbFactorList); 
	
	TReal32 red = iEffect.RedLevelL();
	TReal32 green = iEffect.GreenLevelL();
	TReal32 blue = iEffect.BlueLevelL();
			
	SetChoiceListCurrentItem(EEffectRgbColorAdjustRed, ((TInt)(red* 100.0f)+100));
	SetChoiceListCurrentItem(EEffectRgbColorAdjustGreen, ((TInt)(green* 100.0f)+100));
	SetChoiceListCurrentItem(EEffectRgbColorAdjustBlue, ((TInt)(blue* 100.0f)+100));
		
	iEffect.SetRGBLevelL(red, green, blue);
	iUi.RefreshL();
	}
	

void CRgbColorAdjustInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TInt red = ChoiceListCurrentItem(EEffectRgbColorAdjustRed)-100;
	TInt green = ChoiceListCurrentItem(EEffectRgbColorAdjustGreen)-100;
	TInt blue = ChoiceListCurrentItem(EEffectRgbColorAdjustBlue)-100;
		
	iEffect.SetRGBLevelL(
			TReal32(red) / 100.0f ,
			TReal32(green) / 100.0f,
			TReal32(blue) / 100.0f);
	
	iUi.RefreshL();
	}


//
// Input dialog to get Blur effect parameters.
//
CBlurInputDialog::CBlurInputDialog(ImageProcessor::TEffectBlur& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aAxFactorList, CDesCArrayFlat* aAyFactorList, CDesCArrayFlat* aBxFactorList, CDesCArrayFlat* aByFactorList, TSize& aBlurArea)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iAxFactorList(aAxFactorList), iAyFactorList(aAyFactorList), iBxFactorList(aBxFactorList), iByFactorList(aByFactorList), iBlurArea(aBlurArea)
	{
	}

//
// Initializes layout.
//
void CBlurInputDialog::PreLayoutDynInitL()
	{
	TInt level = 0;
	iEffect.LevelL(level);

	SetChoiceListCurrentItem(EEffectBlurMode, level);
	
	CEikChoiceList* axFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectBlurAx));
	axFactorList->SetArrayExternalOwnership(ETrue);
	axFactorList->SetArrayL(iAxFactorList); 
	
	CEikChoiceList* ayFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectBlurAy));
	ayFactorList->SetArrayExternalOwnership(ETrue);
	ayFactorList->SetArrayL(iAyFactorList); 
	
	CEikChoiceList* bxFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectBlurBx));
	bxFactorList->SetArrayExternalOwnership(ETrue);
	bxFactorList->SetArrayL(iBxFactorList); 
			
	CEikChoiceList* byFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectBlurBy));
	byFactorList->SetArrayExternalOwnership(ETrue);
	byFactorList->SetArrayL(iByFactorList); 
	
	TInt imagewidth = iBlurArea.iWidth;
	TInt imageheight = iBlurArea.iHeight;
	
	TInt centerpointx = imagewidth/2;
	TInt centerpointy = imageheight/2;
	
	TPoint tl,br;
	
	tl.iX = centerpointx - imagewidth/8;
	tl.iY = centerpointy - imageheight/8;
	br.iX = centerpointx + imagewidth/8;
	br.iY = centerpointy + imageheight/8;
	
	SetChoiceListCurrentItem(EEffectBlurAx, tl.iX);
	SetChoiceListCurrentItem(EEffectBlurAy, tl.iY);
	SetChoiceListCurrentItem(EEffectBlurBx, br.iX);
	SetChoiceListCurrentItem(EEffectBlurBy, br.iY);
	
	TRect rect(tl.iX,tl.iY,br.iX,br.iY);
	
	iEffect.SetRectL(rect);
	iEffect.SetLevelL(level);
	iUi.RefreshL();
	} 


void CBlurInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TRect rect(
			ChoiceListCurrentItem(EEffectBlurAx),
			ChoiceListCurrentItem(EEffectBlurAy),
			ChoiceListCurrentItem(EEffectBlurBx),
			ChoiceListCurrentItem(EEffectBlurBy));
	
	iEffect.SetRectL(rect);
	iEffect.SetLevelL(ChoiceListCurrentItem(EEffectBlurMode) + 1);
	
	iUi.RefreshL();
	}


CColorizationInputDialog::CColorizationInputDialog(ImageProcessor::TEffectColorization& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aLevelFactorList, CDesCArrayFlat* aRgbFactorList)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iLevelFactorList(aLevelFactorList), iRgbFactorList(aRgbFactorList)
	{
	}

//
// Initializes layout
//
void CColorizationInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* lumFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectColorizationLuminance));
	lumFactorList->SetArrayExternalOwnership(ETrue);
	lumFactorList->SetArrayL(iLevelFactorList); 
	
	CEikChoiceList* chromFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectColorizationChrominance));
	chromFactorList->SetArrayExternalOwnership(ETrue);
	chromFactorList->SetArrayL(iLevelFactorList); 
	
	CEikChoiceList* redFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectColorizationRed));
	redFactorList->SetArrayExternalOwnership(ETrue);
	redFactorList->SetArrayL(iRgbFactorList); 
	
	CEikChoiceList* greenFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectColorizationGreen));
	greenFactorList->SetArrayExternalOwnership(ETrue);
	greenFactorList->SetArrayL(iRgbFactorList); 
	
	CEikChoiceList* blueFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectColorizationBlue));
	blueFactorList->SetArrayExternalOwnership(ETrue);
	blueFactorList->SetArrayL(iRgbFactorList); 	
	
	TRgb rgb = iEffect.ReferenceColorL();
	TInt lum = iEffect.LuminanceStrengthL();
	TInt chrom = iEffect.ChrominanceStrengthL();
	
	SetChoiceListCurrentItem(EEffectColorizationLuminance, lum+100);
	SetChoiceListCurrentItem(EEffectColorizationChrominance, chrom+100);
	SetChoiceListCurrentItem(EEffectColorizationRed, rgb.Red());
	SetChoiceListCurrentItem(EEffectColorizationGreen, rgb.Green());
	SetChoiceListCurrentItem(EEffectColorizationBlue, rgb.Blue());
			
	iEffect.SetColorizationL(rgb, lum, chrom);
	iUi.RefreshL();
	} 


void CColorizationInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TRgb rgb;
	
	TInt lum = ChoiceListCurrentItem(EEffectColorizationLuminance)-100;
	TInt chrom = ChoiceListCurrentItem(EEffectColorizationChrominance)-100;
	
	rgb.SetRed(ChoiceListCurrentItem(EEffectColorizationRed));
	rgb.SetGreen(ChoiceListCurrentItem(EEffectColorizationGreen));
	rgb.SetBlue(ChoiceListCurrentItem(EEffectColorizationBlue));
	
	iEffect.SetColorizationL(rgb, lum, chrom);
	iUi.RefreshL();
	}


//
// Input dialog to get spotlight effect parameters
//
CSpotLightInputDialog::	CSpotLightInputDialog(ImageProcessor::TEffectSpotLight& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aXFactorList, CDesCArrayFlat* aYFactorList, CDesCArrayFlat* aRadiusFactorList, CDesCArrayFlat* aFractionFactorList, TSize& aSpotAreaSize)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iXFactorList(aXFactorList),iYFactorList(aYFactorList),iRadiusFactorList(aRadiusFactorList),iFractionFactorList(aFractionFactorList),iSpotAreaSize(aSpotAreaSize)
	{
	}

//
// Initializes layout
//
void CSpotLightInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* xFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectCenterX));
	xFactorList->SetArrayExternalOwnership(ETrue);
	xFactorList->SetArrayL(iXFactorList); 
	
	CEikChoiceList* yFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectCenterY));
	yFactorList->SetArrayExternalOwnership(ETrue);
	yFactorList->SetArrayL(iYFactorList); 
	
	CEikChoiceList* radiusFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectRadius));
	radiusFactorList->SetArrayExternalOwnership(ETrue);
	radiusFactorList->SetArrayL(iRadiusFactorList); 
		
	CEikChoiceList* fractionFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectTransSize));
	fractionFactorList->SetArrayExternalOwnership(ETrue);
	fractionFactorList->SetArrayL(iFractionFactorList); 
	
	TUint32 spotradius = Max(iSpotAreaSize.iWidth, iSpotAreaSize.iHeight)/8;
		
	SetChoiceListCurrentItem(EEffectCenterX, iSpotAreaSize.iWidth/2);
	SetChoiceListCurrentItem(EEffectCenterY, iSpotAreaSize.iHeight/2);
	SetChoiceListCurrentItem(EEffectRadius, spotradius);
	SetChoiceListCurrentItem(EEffectTransSize, 100);
	
	TPoint pos;
	pos.iX = iSpotAreaSize.iWidth/2;
	pos.iY = iSpotAreaSize.iHeight/2;
	
	iEffect.SetSpotLightOrientationL(pos, spotradius, 1.0f);
	iUi.RefreshL();
	} 


void CSpotLightInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TPoint pos;
	
	pos.iX = ChoiceListCurrentItem(EEffectCenterX);
	pos.iY = ChoiceListCurrentItem(EEffectCenterY);
	
	TUint32 radius = ChoiceListCurrentItem(EEffectRadius);
	TReal32 trans = ChoiceListCurrentItem(EEffectTransSize);
	
	iEffect.SetSpotLightOrientationL(pos, radius, trans / 100.0f);
	iUi.RefreshL();
	}


//
// Input dialog to get Stamp effect parameters
//
CStampInputDialog::CStampInputDialog(ImageProcessor::TEffectStamp& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aSmoothFactorList, CDesCArrayFlat* aThresholdFactorList)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iSmoothFactorList(aSmoothFactorList), iThresholdFactorList(aThresholdFactorList)
	{
	}

//
// Initializes layout
//
void CStampInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* smoothFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectStampSmoothness));
	smoothFactorList->SetArrayExternalOwnership(ETrue);
	smoothFactorList->SetArrayL(iSmoothFactorList); 
		
	CEikChoiceList* thresholdFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectStampThreshold));
	thresholdFactorList->SetArrayExternalOwnership(ETrue);
	thresholdFactorList->SetArrayL(iThresholdFactorList); 
	
	TInt smoothness = iEffect.SmoothnessLevelL();
	TInt threshold = iEffect.ThresholdLevelL();
	
	SetChoiceListCurrentItem(EEffectStampSmoothness, smoothness);
	SetChoiceListCurrentItem(EEffectStampThreshold, threshold);
	
	iEffect.SetStampLevelL(smoothness, threshold);
	iUi.RefreshL();
	} 
	
void CStampInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TInt smoothness = ChoiceListCurrentItem(EEffectStampSmoothness);
	TInt threshold = ChoiceListCurrentItem(EEffectStampThreshold);
	
	iEffect.SetStampLevelL(smoothness, threshold);
	iUi.RefreshL();
	}


//
// Input dialog to get Whitebalance effect parameters
//
CWhiteBalanceInputDialog::CWhiteBalanceInputDialog(ImageProcessor::TEffectWhiteBalance& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aRgbFactorList)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iRgbFactorList(aRgbFactorList)
	{
	}

//
// Initializes layout
//
void CWhiteBalanceInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* redFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectWhiteBalanceRed));
	redFactorList->SetArrayExternalOwnership(ETrue);
	redFactorList->SetArrayL(iRgbFactorList); 
		
	CEikChoiceList* greenFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectWhiteBalanceGreen));
	greenFactorList->SetArrayExternalOwnership(ETrue);
	greenFactorList->SetArrayL(iRgbFactorList); 
			
	CEikChoiceList* blueFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectWhiteBalanceBlue));
	blueFactorList->SetArrayExternalOwnership(ETrue);
	blueFactorList->SetArrayL(iRgbFactorList); 
		
	TRgb rgb = iEffect.ReferenceWhiteColorL();
	
	SetChoiceListCurrentItem(EEffectWhiteBalanceRed, rgb.Red());
	SetChoiceListCurrentItem(EEffectWhiteBalanceGreen, rgb.Green());
	SetChoiceListCurrentItem(EEffectWhiteBalanceBlue, rgb.Blue());
		
	ImageProcessor::TEffectWhiteBalance::TWhiteBalanceMode mode = iEffect.WhiteBalanceModeL();
	SetChoiceListCurrentItem(EEffectWhiteBalanceMode, TInt(mode));
	
	iEffect.SetWhiteBalanceL(mode, rgb);
	iUi.RefreshL();
	} 

void CWhiteBalanceInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	using namespace ImageProcessor;
	
	TInt mode = ChoiceListCurrentItem(EEffectWhiteBalanceMode);
	
	TRgb rgb;
	rgb.SetRed(ChoiceListCurrentItem(EEffectWhiteBalanceRed));
	rgb.SetGreen(ChoiceListCurrentItem(EEffectWhiteBalanceGreen));
	rgb.SetBlue(ChoiceListCurrentItem(EEffectWhiteBalanceBlue));
	
	iEffect.SetWhiteBalanceL(static_cast<TEffectWhiteBalance::TWhiteBalanceMode>(mode), rgb);	
	iUi.RefreshL();
	}


//
// Input dialog to get Levels effect parameters
//
CLevelsInputDialog::CLevelsInputDialog(ImageProcessor::TEffectLevels& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aLevelFactorList)
 :	iEffect(aEffect),
 	iUi(aUi),
 	iLevelFactorList(aLevelFactorList)
	{
	}

//
// Initializes layout
//
void CLevelsInputDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* whiteFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectLevelsWhite));
	whiteFactorList->SetArrayExternalOwnership(ETrue);
	whiteFactorList->SetArrayL(iLevelFactorList); 
		
	CEikChoiceList* grayFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectLevelsGray));
	grayFactorList->SetArrayExternalOwnership(ETrue);
	grayFactorList->SetArrayL(iLevelFactorList); 
				
	CEikChoiceList* blackFactorList = STATIC_CAST(CEikChoiceList*, Control(EEffectLevelsBlack));
	blackFactorList->SetArrayExternalOwnership(ETrue);
	blackFactorList->SetArrayL(iLevelFactorList); 
		
	TReal32 white = iEffect.WhiteLevelL();
	TReal32 gray = iEffect.GrayLevelL();
	TReal32 black = iEffect.BlackLevelL();
	
	SetChoiceListCurrentItem(EEffectLevelsWhite, white*100);
	SetChoiceListCurrentItem(EEffectLevelsGray, gray*100);
	SetChoiceListCurrentItem(EEffectLevelsBlack, black*100);
		
	iEffect.SetWGBLevelL(white, gray, black);
	iUi.RefreshL();
	} 
	
void CLevelsInputDialog::HandleControlEventL(CCoeControl* /*aControl*/, TCoeEvent /*aEvent*/)
	{
	TReal32 white = TReal32(ChoiceListCurrentItem(EEffectLevelsWhite)) / 100.0f;
	TReal32 gray = TReal32(ChoiceListCurrentItem(EEffectLevelsGray)) / 100.0f;
	TReal32 black = TReal32(ChoiceListCurrentItem(EEffectLevelsBlack)) / 100.0f;
	
	iEffect.SetWGBLevelL(white, gray, black);
	iUi.RefreshL();
	}

