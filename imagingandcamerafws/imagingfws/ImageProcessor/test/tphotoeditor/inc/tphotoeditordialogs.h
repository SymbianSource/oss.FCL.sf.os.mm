/**
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __TPHOTO_EDITOR_DIALOGS_H__
#define __TPHOTO_EDITOR_DIALOGS_H__

#include <techview/eikcfdlg.h>
#include <techview/eikmfne.h>
#include <techview/eikchlst.h>

#include "tphotoeditor.h"
#include "tphotoeditor.hrh"

// Numeric editor control start value
const TInt KNumeriEditorControlidStartValue = 2000;
const TInt KDefaultRotationAngle = 45;

class CTPhotoEditorAppUi;

//Message Dialog   
class CMessageDialog : public CEikDialog
    {
public:
	static CMessageDialog* NewL(TDesC& aMessage);
	~CMessageDialog();
    
private:
	CMessageDialog();
	void ConstructL(TDesC& aMessage);
 	void PreLayoutDynInitL();
    TBool OkToExitL(TInt aButtonId);
    
private:
	HBufC* iMessage;
    };
   
//Input dialog to confirm whether save the applied effect or not    
class CConfirmInputDialog : public CEikDialog
    {
public:
	CConfirmInputDialog(TInt aTitleId);
    
private:
 	void PreLayoutDynInitL();
    TBool OkToExitL(TInt aButtonId);
    
private:
	TInt iTitleId;
    };

	

//Input dialog to get encode options    
class CEncodeOptionInputDialog : public CEikDialog
    {
public:
	CEncodeOptionInputDialog(CDesCArrayFlat* aOutputFormatList, TInt& aOutputFormatIndex, TInt& aWidth, TInt& aHeight, TInt& aJpegQuality, TBool& aMaintainAspectRatio, TInt& aOriginalWidth, TInt& aOriginalHeight);
    
private:
 	void PreLayoutDynInitL();
 	TBool OkToExitL(TInt aButtonId);
    
private:
	CDesCArrayFlat* iOutputFormatList;
	TInt& iOutputFormatIndex;
	TInt& iWidth;
	TInt& iHeight;
	TInt& iJpegQuality;
	TBool& iMaintainAspectRatio;
	TInt& iOriginalWidth;
	TInt& iOriginalHeight;
	};

//
// Input dialog to get number parameters for effects.
//
class CNumberInputDialog : public CEikDialog
    {
public:
   CNumberInputDialog(ImageProcessor::TEffect& aEffect, CTPhotoEditorAppUi& aUi, TInt aTitleId, CDesCArrayFlat* aNumberList);
   
private:
 	void PreLayoutDynInitL();
	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
		
private:
	ImageProcessor::TEffect& iEffect;
	CTPhotoEditorAppUi& iUi;
	TInt iTitleId;
	CDesCArrayFlat* iNumberList;
    };

//
// Input dialog to get Mode(enumeration) parameters for effects.
//
class CModeInputDialog : public CEikDialog
    {
public:
	CModeInputDialog(ImageProcessor::TEffect& aEffect, TInt aEffectChoice, CTPhotoEditorAppUi& aUi, TInt aTitleId);
    
private:
	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
 	void PreLayoutDynInitL();
    
protected:
	ImageProcessor::TEffect& iEffect;
	TInt iEffectChoice;
	CTPhotoEditorAppUi& iUi;
	TInt iTitleId;
    };   
   
//
//Input dialog to get Sketch effect parameters  
//
class CSketchModeInputDialog : public CModeInputDialog
    {
public:
	CSketchModeInputDialog(ImageProcessor::TEffect& aEffect, CTPhotoEditorAppUi& aUi);
    
private:
	void CurrentSketchLevelApplyL();
	void PreLayoutDynInitL();
	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
	};  

//
//Input dialog to get Cartoon effect parameters  
//
class CCartoonModeInputDialog : public CModeInputDialog
    {
public:
	CCartoonModeInputDialog(ImageProcessor::TEffect& aEffect, CTPhotoEditorAppUi& aUi);
    
private:
	void CurrentCartoonLevelApplyL();
	void PreLayoutDynInitL();
	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
	};  


//Input dialog to get Insert Clipart effect parameters  	
class CClipartInputDialog : public CEikDialog
    {
public:
	CClipartInputDialog(TInt& aScale, TInt& aAx, TInt& aAy, TInt& aAngle, TInt& aMaxWidth, TInt& aMaxHeight);
    
private:
 	void PreLayoutDynInitL();
 	TBool OkToExitL(TInt aButtonId);
    
private:
	TInt& iScale;
	TInt& iAx;
	TInt& iAy;
	TInt& iAngle;
	TInt& iMaxWidth;
	TInt& iMaxHeight;
	}; 	
   
//
// Input dialog to get rotation effect parameters.
//
class CRotationInputDialog : public CEikDialog
	{
public:
	CRotationInputDialog(ImageProcessor::TEffectRotation& aRotation, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aRotationAngleList);

private:
	void PreLayoutDynInitL();
	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);

private:
	ImageProcessor::TEffectRotation& iRotation;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iRotationAngleList;
	};  

//
// Input dialog to get RgbColorAdjust effect parameters.
//
class CRgbColorAdjustInputDialog : public CEikDialog
    {
public:
	CRgbColorAdjustInputDialog(ImageProcessor::TEffectRgbColorAdjust& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aRgbFactorList);
    
private:
 	void PreLayoutDynInitL();
 	void HandleControlEventL(CCoeControl* aControl, TCoeEvent /*aEvent*/);

private:
	ImageProcessor::TEffectRgbColorAdjust& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iRgbFactorList;
	}; 


//
// Input dialog to get Blur effect parameters.
//
class CBlurInputDialog : public CEikDialog
	{
public:
	CBlurInputDialog(ImageProcessor::TEffectBlur& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aAxFactorList, CDesCArrayFlat* aAyFactorList, CDesCArrayFlat* aBxFactorList, CDesCArrayFlat* aByFactorList, TSize& aBlurArea);
	
private:
	void PreLayoutDynInitL();
	void HandleControlEventL(CCoeControl* aControl, TCoeEvent /*aEvent*/);

private:
	ImageProcessor::TEffectBlur& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iAxFactorList;
	CDesCArrayFlat* iAyFactorList;
	CDesCArrayFlat* iBxFactorList;
	CDesCArrayFlat* iByFactorList;
	TSize iBlurArea;
	}; 		

//
// Input dialog to get Colorization effect parameters
//
class CColorizationInputDialog : public CEikDialog
    {
public:
	CColorizationInputDialog(ImageProcessor::TEffectColorization& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aLevelFactorList, CDesCArrayFlat* aRgbFactorList);
	
private:
 	void PreLayoutDynInitL();
 	void HandleControlEventL(CCoeControl* aControl, TCoeEvent /*aEvent*/);
    
private:
	ImageProcessor::TEffectColorization& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iLevelFactorList;
	CDesCArrayFlat* iRgbFactorList;
	}; 

//
// Input dialog to get spotlight effect parameters
//
class CSpotLightInputDialog : public CEikDialog
    {
public:
	CSpotLightInputDialog(ImageProcessor::TEffectSpotLight& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aXFactorList, CDesCArrayFlat* aYFactorList, CDesCArrayFlat* aRadiusFactorList, CDesCArrayFlat* aFractionFactorList, TSize& aSpotAreaSize);
    
private:
 	void PreLayoutDynInitL();
 	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
    
private:
	ImageProcessor::TEffectSpotLight& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iXFactorList;
	CDesCArrayFlat* iYFactorList;
	CDesCArrayFlat* iRadiusFactorList;
	CDesCArrayFlat* iFractionFactorList;
	TSize iSpotAreaSize;
	}; 	

//
// Input dialog to get Stamp effect parameters
//
class CStampInputDialog : public CEikDialog
    {
public:
    CStampInputDialog(ImageProcessor::TEffectStamp& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aSmoothFactorList, CDesCArrayFlat* aThresholdFactorList);
    
private:
 	void PreLayoutDynInitL();
 	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
    
private:
	ImageProcessor::TEffectStamp& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iSmoothFactorList;
	CDesCArrayFlat* iThresholdFactorList;
	};		

//
// Input dialog to get Whitebalance effect parameters
//
class CWhiteBalanceInputDialog : public CEikDialog
    {
public:
	CWhiteBalanceInputDialog(ImageProcessor::TEffectWhiteBalance& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aRgbFactorList);
    
private:
 	void PreLayoutDynInitL();
 	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
    
private:
	ImageProcessor::TEffectWhiteBalance& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iRgbFactorList;
	}; 	

//
// Input dialog to get Levels effect parameters
//
class CLevelsInputDialog : public CEikDialog
    {
public:
	CLevelsInputDialog(ImageProcessor::TEffectLevels& aEffect, CTPhotoEditorAppUi& aUi, CDesCArrayFlat* aLevelFactorList);
    
private:
 	void PreLayoutDynInitL();
 	void HandleControlEventL(CCoeControl* aControl, TCoeEvent aEvent);
    
private:
	ImageProcessor::TEffectLevels& iEffect;
	CTPhotoEditorAppUi& iUi;
	CDesCArrayFlat* iLevelFactorList;
	}; 	

#endif //__TPHOTO_EDITOR_DIALOGS_H__
