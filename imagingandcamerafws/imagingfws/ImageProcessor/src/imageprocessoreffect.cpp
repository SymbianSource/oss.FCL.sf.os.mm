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


#include <imageprocessor/imageprocessoreffect.h>
#include <imageprocessor/imageprocessorplugineffect.h>
#include "imageprocessorimpl.h"
#include <imageprocessor/imageprocessor.h>

namespace ImageProcessor
   {

/**
Constructor.

@param aUid The uid of the effect.
@param aPluginEffect The effect plugin.
@param aImageProcessorImpl The image processor implementation.
*/	
TEffect::TEffect(const TUid& aUid, Plugin::MEffect& aPluginEffect, CImageProcessorImpl& aImageProcessorImpl): 
	iPluginEffect(aPluginEffect), 
	iUid(aUid),
	iIsActive(EFalse),
	iImageProcessorImpl(aImageProcessorImpl),
	iReserved(0)
	{
	}

/**
The begin function of effects, which is first part of the three stages/functions: "begin -> set -> end". .
*/
EXPORT_C void TEffect::BeginL()
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(),  User::Leave(KErrNotReady)); 
	__ASSERT_ALWAYS(!iIsActive, User::Leave(KErrNotReady));
	iImageProcessorImpl.SetStateL(CImgProcessor::EEffectActive);
	
	TRAPD(err, iPluginEffect.BeginL());
	
	if (err != KErrNone) 
		{
		iImageProcessorImpl.RestoreStateL();
		User::Leave(err);
		}
	
	iIsActive = ETrue;
	}

/**
The end function of effects, which is last part of the three stages/functions: "begin -> set -> end". .
*/
EXPORT_C void TEffect::EndL()
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	iImageProcessorImpl.CheckStateL(CImgProcessor::EEffectActive);
	
	TRAPD(err, iPluginEffect.EndL());

 	if (err != KErrNone)
 		{
 		iImageProcessorImpl.SetStateL(CImgProcessor::EInitialized);
 		iIsActive = EFalse;
 		User::Leave(err);			
 		}
	
	iIsActive = EFalse;
	iImageProcessorImpl.SetStateL(CImgProcessor::EInitialized);
	}

/**
Retrieves the input type for the effect.

@return	TEffectInputType corresponding to the level type. 
*/
EXPORT_C TEffect::TEffectInputType TEffect::InputType() const
	{
		{
		TInt minimumLevel = 0;
		TInt maximumLevel = 0;
		TInt levelStep = 0;

		LevelSettings(minimumLevel, maximumLevel, levelStep);
		if (minimumLevel != maximumLevel) 
			{
			return EEffectInputTypeInt;
			}
		}
		{
		TReal32 minimumLevel = 0.0f;
		TReal32 maximumLevel = 0.0f;
		TReal32 levelStep = 0.0f;

		LevelSettings(minimumLevel, maximumLevel, levelStep);
		if (minimumLevel != maximumLevel)// 0.0f == 0.0f - it is always true
			{
			return EEffectInputTypeReal32;
			}
		}
	
	return EEffectInputTypeCustom;
	}

/**
Sets the level to the effect.

@param	aLevel
		The level value to be set.

@leave  KErrNotReady
		The effect is not active. The effect should be set active by calling TEffect::BeginL() function. 
		@see TEffect::BeginL()

@leave  KErrArgument
		The level is not in the level range. The range can be queried by calling TEffect::LevelSettings(TInt& aMinimumLevel, TInt& aMaximumLevel, TInt& aLevelStep) function.
		@see TEffect::LevelSettings(TInt& aMinimumLevel, TInt& aMaximumLevel, TInt& aLevelStep)

@leave  KErrNotSupported
		The input type is not supported by the effect. The supported input type can be queried by calling TEffect::InputType function.
		@see TEffect::InputType()
*/
EXPORT_C void TEffect::SetLevelL(TInt aLevel)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	TInt minimumLevel = 0;
	TInt maximumLevel = 0;
	TInt levelStep = 0;

	LevelSettings(minimumLevel, maximumLevel, levelStep);
	
	if (minimumLevel != maximumLevel) 
		{
		__ASSERT_ALWAYS((minimumLevel <= aLevel) && (maximumLevel >= aLevel), User::Leave(KErrArgument));
		iPluginEffect.SetLevelL(aLevel);
		}
	else 
		{
		User::Leave(KErrNotSupported);
		}
	}

/**
Retrieves the effect level.

@param	aLevel
		The level value.

@leave  KErrNotSupported
		The input type is not supported by the effect. The supported input type can be queried by calling TEffect::InputType function.
		@see TEffect::InputType()
*/
EXPORT_C void TEffect::LevelL(TInt& aLevel) const
	{
	iPluginEffect.LevelL(aLevel);
	}

/**
Retrieves the effect level range. If minimum level equals the maximum level the particular input type is not supported.

@param	aMinimumLevel
		The minimum value of the level.

@param	aMaximumLevel 
		The maximum value of the level.

@param	aLevelStep
		The value of the level step. The level step defines the level range resolution between minimim and maximum
		levels.
*/
EXPORT_C void TEffect::LevelSettings(TInt& aMinimumLevel, TInt& aMaximumLevel, TInt& aLevelStep) const
	{
	iPluginEffect.LevelSettings(aMinimumLevel, aMaximumLevel, aLevelStep);
	}

/**
Sets the level to the effect.

@param	aLevel
		The level value to be set.

@leave  KErrNotReady
                The effect is not active. The effect should be set active by calling TEffect::BeginL() function. 
		@see TEffect::BeginL()

@leave  KErrArgument
                The level is not in the level range. The range can be queried by calling TEffect::LevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel, TReal32& aLevelStep) function.
		@see TEffect::LevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel, TReal32& aLevelStep)

@leave  KErrNotSupported
                The input type is not supported for the effect. The supported input type can be queried by calling TEffect::InputType function.
		@see TEffect::InputType()
*/
EXPORT_C void TEffect::SetLevelL(TReal32 aLevel)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	TReal32 minimumLevel = 0.0f;
	TReal32 maximumLevel = 0.0f;
	TReal32 levelStep = 0.0f;

	LevelSettings(minimumLevel, maximumLevel, levelStep);
	
	if (minimumLevel != maximumLevel) 
		{
		__ASSERT_ALWAYS((minimumLevel <= aLevel) && (maximumLevel >= aLevel), User::Leave(KErrArgument));
		iPluginEffect.SetLevelL(aLevel);
		}
	else 
		{
		User::Leave(KErrNotSupported);
		}
	iPluginEffect.SetLevelL(aLevel);
	}

/**
Retrieves the effect level.

@param	aLevel
		The level value.

@leave  KErrNotSupported
		The input type is not supported by the effect. The supported input type can be queried by calling TEffect::InputType function.
		@see TEffect::InputType()
*/
EXPORT_C void TEffect::LevelL(TReal32& aLevel) const
	{
	iPluginEffect.LevelL(aLevel);
	}

/**
Retrieves the effect level range. If minimum level equals the maximum level the particular input type is not supported.

@param	aMinimumLevel
		The minimum value of the level.

@param	aMaximumLevel 
		The maximum value of the level.

@param	aLevelStep
		The value of the level step. The level step defines the level range resolution between minimim and maximum
		levels.
*/
EXPORT_C void TEffect::LevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel, TReal32& aLevelStep) const
	{
	iPluginEffect.LevelSettings(aMinimumLevel, aMaximumLevel, aLevelStep);
	}

/**
The reset function of effects, which resets the effect parameters and state.
*/
EXPORT_C void TEffect::ResetL()
	{
	TInt err = KErrNone;
	if (iIsActive) 
		{
		EndL();
		//try to undo the effect as it was reset.
		TRAP(err, iImageProcessorImpl.UndoL());
		}
	
	iPluginEffect.ResetL();
	}

/**
Retrieves access to a custom extension.

@param	aExtension
		The UID of the extension to be retrieved

@return	Extension corresponding to the UID given as a parameter.
*/
EXPORT_C TAny* TEffect::Extension(TUid /*aExtension*/)
	{
	return NULL;	
	}

TEffectSepia::TEffectSepia(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectSepiaUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectGrayscale::TEffectGrayscale(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectGrayscaleUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectNegative::TEffectNegative(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectNegativeUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectGrayscaleNegative::TEffectGrayscaleNegative(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectGrayscaleNegativeUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectOily::TEffectOily(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectOilyUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectPaint::TEffectPaint(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectPaintUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectMilky::TEffectMilky(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectMilkyUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectFog::TEffectFog(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectFogUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectAntique::TEffectAntique(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectAntiqueUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectMirrorLeftToRight::TEffectMirrorLeftToRight(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectMirrorLeftToRightUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectMagicPen::TEffectMagicPen(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectMagicPenUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectBrightness::TEffectBrightness(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectBrightnessUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectSharpness::TEffectSharpness(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectSharpnessUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectRotation::TEffectRotation(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectRotationUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the current rotation scale mode

@return	The rotation scale mode
*/
EXPORT_C TEffectRotation::TRotationScaleMode TEffectRotation::ScaleModeL() const
	{
	return (reinterpret_cast<Plugin::MEffectRotation&>(iPluginEffect)).ScaleModeL();
	}

/**
Gets the current rotation angle

@return	The rotation angle
*/
EXPORT_C TReal32 TEffectRotation::AngleL() const
	{
	return (reinterpret_cast<Plugin::MEffectRotation&>(iPluginEffect)).AngleL();
	}

/**
Specifies the rotation scale mode and angle for the current rotation effect

@param	aScaleMode
        The rotation scale mode. Range (1) - (3).  
        
@param	aAngle
        The rotation amgle. Range (0.0f) - (360.0f).  

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The level is not in the TRotationScaleMode enumeration or angle is not in the acceptable range.
*/
EXPORT_C void TEffectRotation::SetRotationL(TEffectRotation::TRotationScaleMode aScaleMode, TReal32 aAngle)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectRotation&>(iPluginEffect)).SetRotationL(aScaleMode, aAngle);
	}

TEffectContrast::TEffectContrast(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectContrastUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectRgbColorAdjust::TEffectRgbColorAdjust(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectRgbColorAdjustUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the current red level of RgbColorAdjust effect 

@return	The red level
*/
EXPORT_C TReal32 TEffectRgbColorAdjust::RedLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectRgbColorAdjust&>(iPluginEffect)).RedLevelL();
	}

/**
Gets the current green level of RgbColorAdjust effect 

@return	The green level
*/
EXPORT_C TReal32 TEffectRgbColorAdjust::GreenLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectRgbColorAdjust&>(iPluginEffect)).GreenLevelL();
	}

/**
Gets the current blue level of RgbColorAdjust effect 

@return	The blue level
*/
EXPORT_C TReal32 TEffectRgbColorAdjust::BlueLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectRgbColorAdjust&>(iPluginEffect)).BlueLevelL();
	}
/**
Specifies the red, green and blue level for the current RgbColorAdjust effect

@param	aRedLevel
        The red channel adjustment. Range (-1.0) - (1.0), where 0.0 implies no adjustment.
@param	aGreenLevel
        The green channel adjustment. Range (-1.0) - (1.0), where 0.0 implies no adjustment.
@param	aBlueLevel
        The blue channel adjustment. Range (-1.0) - (1.0), where 0.0 implies no adjustment.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The red level, or green level, or blue level is not in the acceptable range.
*/   
EXPORT_C void TEffectRgbColorAdjust::SetRGBLevelL(TReal32 aRedLevel, TReal32 aGreenLevel, TReal32 aBlueLevel)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectRgbColorAdjust&>(iPluginEffect)).SetRGBLevelL(aRedLevel, aGreenLevel, aBlueLevel);
	}

TEffectEmboss::TEffectEmboss(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectEmbossUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectSolarize::TEffectSolarize(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectSolarizeUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectPosterize::TEffectPosterize(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectPosterizeUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectStamp::TEffectStamp(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectStampUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the smoothness level for the current stamp effect

@return	The smoothness level
*/
EXPORT_C TInt TEffectStamp::SmoothnessLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectStamp&>(iPluginEffect)).SmoothnessLevelL();
	}

/**
Gets the threshold level for the current stamp effect

@return	The threshold level
*/
EXPORT_C TInt TEffectStamp::ThresholdLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectStamp&>(iPluginEffect)).ThresholdLevelL();
	}

/**
Specifies the smoothness and threshold level for the current stamp effect

@param	aSmoothnessLevel
        The smoothness level. Range (0) - (6), where 0.0 implies no adjustment.
@param	aThresholdLevel
        The threshold level. Range (0) - (255), where 0.0 implies no adjustment.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The smoothness or threshold level is not in the acceptable range.
*/
EXPORT_C void TEffectStamp::SetStampLevelL(TInt aSmoothnessLevel, TInt aThresholdLevel)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectStamp&>(iPluginEffect)).SetStampLevelL(aSmoothnessLevel, aThresholdLevel);
	}

TEffectSketch::TEffectSketch(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectSketchUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Get the current sketch mode

@return	The sketch mode
*/ 
EXPORT_C TEffectSketch::TSketchMode TEffectSketch::SketchModeL() const
	{
	return (TEffectSketch::TSketchMode)(reinterpret_cast<Plugin::MEffectSketch&>(iPluginEffect)).SketchModeL();
	}

/**
Set the sketch mode.

@param	aSketchMode
        The sketch mode, ESketchModeGray or ESketchColor.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The mode is not in the TSketchMode enumeration.
*/
EXPORT_C void TEffectSketch::SetSketchModeL(TEffectSketch::TSketchMode aSketchMode) 
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	__ASSERT_ALWAYS((ESketchModeGray <= aSketchMode) && (ESketchColor >= aSketchMode),User::Leave(KErrArgument));
	(reinterpret_cast<Plugin::MEffectSketch&>(iPluginEffect)).SetSketchModeL(aSketchMode);
	}

TEffectNoise::TEffectNoise(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectNoiseUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectMoonlight::TEffectMoonlight(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectMoonlightUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectFrostedGlass::TEffectFrostedGlass(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectFrostedGlassUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectDespeckle::TEffectDespeckle(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectDespeckleUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectBlur::TEffectBlur(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectBlurUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the current blur effect rectangle

@return	The rectangle to apply blur effect
*/ 
EXPORT_C TRect TEffectBlur::RectL() const
	{
	return (reinterpret_cast<Plugin::MEffectBlur&>(iPluginEffect)).RectL();
	}
   
/**
Specifies the rectangle for the current blur effect

@param	aRect
        The blur effect rectangle. Set this parameter to NULL if the effect should be applied to the entire image.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The level is not in the TBlurLevel enumeration.
*/
EXPORT_C void TEffectBlur::SetRectL(TRect aRect)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectBlur&>(iPluginEffect)).SetRectL(aRect);
	}

TEffectColorization::TEffectColorization(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectColorizationUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the current rgb value of the reference color 

@return	The rgb value of the reference color 
*/
EXPORT_C TRgb TEffectColorization::ReferenceColorL() const
	{
	return (reinterpret_cast<Plugin::MEffectColorization&>(iPluginEffect)).ReferenceColorL();
	}

/**
Gets the current luminance level of colorization effect 

@return	Strength of luminance
*/ 
EXPORT_C TInt TEffectColorization::LuminanceStrengthL() const
	{
	return (reinterpret_cast<Plugin::MEffectColorization&>(iPluginEffect)).LuminanceStrengthL();
	}

/**
Gets the current chrominance level of colorization effect 

@return	Strength of chrominance
*/ 
EXPORT_C TInt TEffectColorization::ChrominanceStrengthL() const
	{
	return (reinterpret_cast<Plugin::MEffectColorization&>(iPluginEffect)).ChrominanceStrengthL();
	}

/**
Specifies the rgb value of reference color and specifies luminance, chrominance level for 
the colorization effect.

@param	aReferenceColor
        The rgb value of the reference color.
@param	aLuminanceStrength
        The strength of luminance. Range (-100) - (200), where 0.0 implies no adjustment.
@param	aChrominanceStrength
        The strength of chrominance. Range (-100) - (200), where 0.0 implies no adjustment.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The luminance or chrominance strength is not in the acceptable range.
*/
EXPORT_C void TEffectColorization::SetColorizationL(TRgb aReferenceColor, TInt aLuminanceStrength, TInt aChrominanceStrength) 
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectColorization&>(iPluginEffect)).SetColorizationL(aReferenceColor, aLuminanceStrength, aChrominanceStrength);
	}

TEffectCartoon::TEffectCartoon(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectCartoonUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Return whether the cartoon effect has distinct edges.

@return	ETrue if all detected edges in the image are rendered distinctively
*/
EXPORT_C TBool TEffectCartoon::DistinctEdgesL() const
	{
	return (reinterpret_cast<Plugin::MEffectCartoon&>(iPluginEffect)).DistinctEdgesL();
	}

/**
Specifies if the current cartoon effect should have distinct edges.

@param	aDistinctEdges
        If set to ETrue all detected edges in the image are rendered distinctively, if EFalse edges are ignored.
@leave	KErrNotReady
		The effect is not active.
*/
EXPORT_C void TEffectCartoon::SetDistinctEdgesL(TBool aDistinctEdges)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectCartoon&>(iPluginEffect)).SetDistinctEdgesL(aDistinctEdges);
	}

TEffectLocalBoost::TEffectLocalBoost(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectLocalBoostUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectColorBoost::TEffectColorBoost(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectColorBoostUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectWhiteBalance::TEffectWhiteBalance(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectWhiteBalanceUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the white balance mode 

@return	The value of the white balance mode 
*/
EXPORT_C TEffectWhiteBalance::TWhiteBalanceMode TEffectWhiteBalance::WhiteBalanceModeL() const
	{
	return (reinterpret_cast<Plugin::MEffectWhiteBalance&>(iPluginEffect)).WhiteBalanceModeL();
	}

/**
Gets the current rgb value of the reference white color 

@return	The rgb value of the reference white color 
*/
EXPORT_C TRgb TEffectWhiteBalance::ReferenceWhiteColorL() const
	{
	return (reinterpret_cast<Plugin::MEffectWhiteBalance&>(iPluginEffect)).ReferenceWhiteColorL();
	}
/**
Specifies the white balance mode and the value of reference white color.

@param	aWhiteBalanceMode
        The white balance mode.
@param	aReferenceWhiteColor
        The rgb value of the reference white color.
@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The whitebalance mode is not in the TWhiteBalanceMode enumeration
*/
EXPORT_C void TEffectWhiteBalance::SetWhiteBalanceL(TWhiteBalanceMode aWhiteBalanceMode, TRgb aReferenceWhiteColor) 
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectWhiteBalance&>(iPluginEffect)).SetWhiteBalanceL(aWhiteBalanceMode, aReferenceWhiteColor);
	}

TEffectAutoLevels::TEffectAutoLevels(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectAutoLevelsUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectLevels::TEffectLevels(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectLevelsUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the current bright saturating value.  

@return	The bright saturating value
*/
EXPORT_C TReal32 TEffectLevels::WhiteLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectLevels&>(iPluginEffect)).WhiteLevelL();
	}

/**
Gets the current middle gray value.

@return	The middle gray value
*/
EXPORT_C TReal32 TEffectLevels::GrayLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectLevels&>(iPluginEffect)).GrayLevelL();
	}

/**
Gets the current dark saturating value. 

@return	The dark saturating value
*/
EXPORT_C TReal32 TEffectLevels::BlackLevelL() const
	{
	return (reinterpret_cast<Plugin::MEffectLevels&>(iPluginEffect)).BlackLevelL();
	}

/**
Specifies the white/gray/black level. 

@param	aWhiteLevel
        The position of the bright saturating point. Range (0.0) - (1.0).
@param	aGrayLevel
        The relative position of the middle gray point. Range (0.0) - (1.0).
@param	aBlackLevel
        The position of the dark saturating point. Range (0.0) - (1.0).
@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The aWhiteLevel or aGrayLevel or aBlackLevel mode is not in the acceptable range.
*/
EXPORT_C void TEffectLevels::SetWGBLevelL(TReal32 aWhiteLevel, TReal32 aGrayLevel, TReal32 aBlackLevel)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectLevels&>(iPluginEffect)).SetWGBLevelL(aWhiteLevel, aGrayLevel, aBlackLevel);
	}

TEffectExposure::TEffectExposure(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectExposureUid, aEffect, aImageProcessorImpl)
	{
	}

TEffectSpotLight::TEffectSpotLight(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectSpotLightUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the current spotlight orientation.

@param	aPosition
        The center of the spotlight circle.
@param	aRadius
		The radius of the spotlight circle in pixels.
@param	aTransitionSize
		The size of the transition region given in fraction of the radius. 
@leave	KErrNotReady
		The effect is not active.
*/ 
EXPORT_C void TEffectSpotLight::SpotLightOrientationL(TPoint &aPosition, TUint32& aRadius, TReal32& aTransitionSize) const
	{
	(reinterpret_cast<Plugin::MEffectSpotLight&>(iPluginEffect)).SpotLightOrientationL(aPosition, aRadius, aTransitionSize);
	}

/**
Specifies the current spotlight orientation.

@param	aPosition
        The center of the spotlight circle.
@param	aRadius
		The radius of the spotlight circle in pixels. Range (0) - (65535 - 1024).
@param	aTransitionSize
		The size of the transition region given in fraction of the radius. Range (0) - (1.0).
@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The level is not in the acceptable range.
*/
EXPORT_C void TEffectSpotLight::SetSpotLightOrientationL(const TPoint& aPosition, TUint32 aRadius, TReal32 aTransitionSize)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectSpotLight&>(iPluginEffect)).SetSpotLightOrientationL(aPosition, aRadius, aTransitionSize);
	}

TEffectCustomTransformation::TEffectCustomTransformation(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectCustomTransformationUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Specifies the input file for the current CustomTransformation effect.

@param	aFilename
        Filename specifying the custom transformation file to use when applying the effect.
@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The file name is not correct.
*/
EXPORT_C void TEffectCustomTransformation::SetInputL(const TDesC& aFilename)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	if (aFilename.Length() > KMaxFileName) 
		{
		User::Leave(KErrArgument);
		}
	
	(reinterpret_cast<Plugin::MEffectCustomTransformation&>(iPluginEffect)).SetInputL(aFilename);
	}

/**
Specifies the input buffer for the current CustomTransformation effect.

@param	aBuffer
        Buffer containing the custom transformation to apply.
@leave	KErrNotReady
		The effect is not active.
*/
EXPORT_C void TEffectCustomTransformation::SetInputL(const TDesC8& aBuffer)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MEffectCustomTransformation&>(iPluginEffect)).SetInputL(aBuffer);
	}

TGeometricalOperation::TGeometricalOperation(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl):
	TEffect(KGeometricalOperationUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Gets the operation of the current GeometricalOperation effect. 

@return	The operation
*/
EXPORT_C CImgProcessor::TOperation TGeometricalOperation::Operation() const
	{
	return (reinterpret_cast<Plugin::MGeometricalOperation&>(iPluginEffect)).Operation();
	}

/**
Specifies the operation for the current GeometricalOperation effect.

@param	aOperation
        The operation. 
@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The operation is not in the CImgProcessor::TOperation enumeration.
*/
EXPORT_C void TGeometricalOperation::SetOperationL(CImgProcessor::TOperation aOperation)
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	(reinterpret_cast<Plugin::MGeometricalOperation&>(iPluginEffect)).SetOperationL(aOperation);
	}

TEffectBorder::TEffectBorder(Plugin::MEffect& aEffect, CImageProcessorImpl& aImageProcessorImpl) : 
	TEffect(KEffectBorderUid, aEffect, aImageProcessorImpl)
	{
	}

/**
Specifies the input file for the current Border effect.

@param	aFilename
        The file name of the border image.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The file name is not correct.
*/
EXPORT_C void TEffectBorder::SetInputL(const TDesC& aFilename) 
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	if (aFilename.Length() > KMaxFileName) 
		{
		User::Leave(KErrArgument);
		}
	
	(reinterpret_cast<Plugin::MEffectBorder&>(iPluginEffect)).SetInputL(aFilename);
	}

/**
Specifies the input buffer for the current Border effect.

@param	aBuffer
        The pointer to the buffer of the border image.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The buffer is empty.
*/
EXPORT_C void TEffectBorder::SetInputL(const TDesC8& aBuffer) 
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	if (aBuffer.Size() == 0) 
		{
		User::Leave(KErrArgument);
		}
	
	(reinterpret_cast<Plugin::MEffectBorder&>(iPluginEffect)).SetInputL(aBuffer);
	}

/**
Specifies the input bitmap for the current Border effect.

@param	aBitmap
        The pointer to the bitmap of the border image.
		The bitmap must have a display mode of EColor16MA.

@leave	KErrNotReady
		The effect is not active.
@leave	KErrArgument
		The bitmap is empty, or the bitmap has an unsupported display mode.

*/
EXPORT_C void TEffectBorder::SetInputL(const CFbsBitmap& aBitmap) 
	{
	__ASSERT_ALWAYS(iIsActive, User::Leave(KErrNotReady));
	if (aBitmap.SizeInPixels() == TSize(0,0)) 
		{
		User::Leave(KErrArgument);
		}
	
	if(aBitmap.Handle()!=0 && aBitmap.ExtendedBitmapType()!=KNullUid)
        {
        User::Leave(KErrNotSupported);
        }
	
	(reinterpret_cast<Plugin::MEffectBorder&>(iPluginEffect)).SetInputL(aBitmap);
	}

	}//namespace ImageProcessor

//EOF
