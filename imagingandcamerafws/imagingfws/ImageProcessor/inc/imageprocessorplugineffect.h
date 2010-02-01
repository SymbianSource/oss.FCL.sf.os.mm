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

#ifndef __IMAGE_PROCESSOR_PLUGIN_EFFECT_H__
#define __IMAGE_PROCESSOR_PLUGIN_EFFECT_H__

#include <e32base.h>
#include <imageprocessor/imageprocessor.h>
#include <imageprocessor/imageprocessoreffect.h>

namespace ImageProcessor
	{

class TOperation;

namespace Plugin
	{

/**
Plugin API for the effects.

@see TEffect
@publishedPartner
@released
*/	
class MEffect
	{
public:
	/**
	This must provide an implementation as defined by TEffect::Extension(TUid aExtension)
	
	@see TEffect::Extension(TUid aExtension)
	*/	
	virtual TAny* Extension(TUid aExtension) = 0;
	
	/**
	This must provide an implementation as defined by TEffect::BeginL()
	
	@see TEffect::BeginL()
	*/	
	virtual void BeginL() = 0;
	
	/**
	This must provide an implementation as defined by TEffect::EndL()
	
	@see TEffect::EndL()
	*/	
	virtual void EndL() = 0;

	/**
	This must provide an implementation as defined by TEffect::SetLevelL(TInt aLevel)
	
	@see TEffect::SetLevelL(TInt aLevel)
	*/	
	virtual void SetLevelL(TInt aLevel) = 0;

	/**
	This must provide an implementation as defined by TEffect::LevelL(TInt& aLevel)
	
	@see TEffect::LevelL(TInt& aLevel)
	*/	
	virtual void LevelL(TInt& aLevel) = 0;

	/**
	This must provide an implementation as defined by TEffect::LevelSettingsL(TInt& aMinimumLevel, TInt& aMaximumLevel, TInt& aLevelStep)
	
	@see TEffect::LevelSettings(TInt& aMinimumLevel, TInt& aMaximumLevel, TInt& aLevelStep)
	*/	
	virtual void LevelSettings(TInt& aMinimumLevel, TInt& aMaximumLevel, TInt& aLevelStep) = 0;
		
	/**
	This must provide an implementation as defined by TEffect::SetLevelL(TReal32 aLevel)
	
	@see TEffect::SetLevelL(TReal32 aLevel)
	*/	
	virtual void SetLevelL(TReal32 aLevel) = 0;
	
	/**
	This must provide an implementation as defined by TEffect::LevelL(TReal32& aLevel)
	
	@see TEffect::LevelL(TReal32& aLevel)
	*/	
	virtual void LevelL(TReal32& aLevel) = 0;
	
	/**
	This must provide an implementation as defined by TEffect::LevelSettingsL(TReal32& aMinimumLevel, TReal32& aMaximumLevel, TReal32& aLevelStep)
	
	@see TEffect::LevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel, TReal32& aLevelStep)
	*/	
	virtual void LevelSettings(TReal32& aMinimumLevel, TReal32& aMaximumLevel, TReal32& aLevelStep) = 0;

	/**
	This must provide an implementation as defined by TEffect::ResetL()
	
	@see TEffect::ResetL()
	*/	
	virtual void ResetL() = 0;
	};

/**
Plugin API for the sepia photoart effect.
The TEffectSepia uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectSepia are passed
verbatim to this interface. For further description of required functionality, 
see TEffectSepia.

@see TEffectSepia
@publishedPartner
@released
*/
class MEffectSepia  : public MEffect
	{
	};	
	
/**
Plugin API for the grayscale photoart effect.
The TEffectGrayscale uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectGrayscale are passed
verbatim to this interface. For further description of required functionality, 
see TEffectGrayscale.

@see TEffectGrayscale
@publishedPartner
@released
*/
class MEffectGrayscale  : public MEffect
	{
	};		
	
/**
Plugin API for the negative photoart effect.
The TEffectNegative uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectNegative are passed
verbatim to this interface. For further description of required functionality, 
see TEffectNegative.

@see TEffectNegative
@publishedPartner
@released
*/
class MEffectNegative  : public MEffect
	{
	};	

/**
Plugin API for the grayscale and negative photoart effect.
The TEffectGrayscaleNegative uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectGrayscaleNegative are passed
verbatim to this interface. For further description of required functionality, 
see TEffectGrayscaleNegative.

@see TEffectGrayscaleNegative
@publishedPartner
@released
*/
class MEffectGrayscaleNegative  : public MEffect
	{
	};	
	
/**
Plugin API for the oily photoart effect.
The TEffectOily uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectOily are passed
verbatim to this interface. For further description of required functionality, 
see TEffectOily.

@see TEffectOily
@publishedPartner
@released
*/	
class MEffectOily  : public MEffect
	{
	};	

/**
Plugin API for the paint photoart effect.
The TEffectPaint  
Once this has been constructed, calls to method functions of TEffectPaint are passed
verbatim to this interface. For further description of required functionality, 
see TEffectPaint.

@see TEffectPaint
@publishedPartner
@released
*/
class MEffectPaint : public MEffect
	{
	};	
	
/**
Plugin API for the milky photoart effect.
The TEffectMilky uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectMilky are passed
verbatim to this interface. For further description of required functionality, 
see TEffectMilky.

@see TEffectMilky
@publishedPartner
@released
*/
class MEffectMilky : public MEffect
	{
	};		

/**
Plugin API for the fog photoart effect.
The TEffectFog uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectFog are passed
verbatim to this interface. For further description of required functionality, 
see TEffectFog.

@see TEffectFog
@publishedPartner
@released
*/
class MEffectFog : public MEffect
	{
	};		

/**
Plugin API for the mirror photoart effect.
The TEffectMirrorLeftToRight uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectMirrorLeftToRight are passed
verbatim to this interface. For further description of required functionality, 
see TEffectMirrorLeftToRight.

@see TEffectMirrorLeftToRight
@publishedPartner
@released
*/	
class MEffectMirrorLeftToRight : public MEffect
	{
	};	
	
/**
Plugin API for the magicpen photoart effect.
The TEffectMagicPen uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectMagicPen are passed
verbatim to this interface. For further description of required functionality, 
see TEffectMagicPen.

@see TEffectMagicPen
@publishedPartner
@released
*/	
class MEffectMagicPen : public MEffect
	{
	};	
	
/**
Plugin API for the antique photoart effect.
The TEffectAntique uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectAntique are passed
verbatim to this interface. For further description of required functionality, 
see TEffectAntique.

@see TEffectAntique
@publishedPartner
@released
*/
class MEffectAntique : public MEffect
	{
	};		

/**
Plugin API for the brightness photoart effect.
The TEffectBrightness uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectBrightness are passed
verbatim to this interface. For further description of required functionality, 
see TEffectBrightness.

@see TEffectBrightness
@publishedPartner
@released
*/		
class MEffectBrightness  : public MEffect
	{
	};		

/**
Plugin API for the sharpness photoart effect.
The TEffectSharpness uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectSharpness are passed
verbatim to this interface. For further description of required functionality, 
see TEffectSharpness.

@see TEffectSharpness
@publishedPartner
@released
*/
class MEffectSharpness  : public MEffect
	{
	};		
	
/**
Plugin API for the rotation photoart effect.
The TEffectRotation uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectRotation are passed
verbatim to this interface. For further description of required functionality, 
see TEffectRotation.

@see TEffectRotation
@publishedPartner
@released
*/
class MEffectRotation  : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectRotation::ScaleModeL()
	
	@see TEffectRotation::TRotationScaleMode ScaleModeL()
	*/
	virtual TEffectRotation::TRotationScaleMode ScaleModeL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectRotation::AngleL()
	
	@see TEffectRotation::AngleL()
	*/
	virtual TReal32 AngleL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectRotation::SetRotationL(TReal32 aScaleMode, TReal32 aAngle)
	
	@see TEffectRotation::SetRotationL(TEffectRotation::TRotationScaleMode aScaleMode, TReal32 aAngle)
	*/
	virtual void SetRotationL(TEffectRotation::TRotationScaleMode aScaleMode, TReal32 aAngle) = 0;
	};	
	
/**
Plugin API for the contrast photoart effect.
The TEffectContrast uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectContrast are passed
verbatim to this interface. For further description of required functionality, 
see TEffectContrast.

@see TEffectContrast
@publishedPartner
@released
*/
class MEffectContrast : public MEffect
	{
	};

/**
Plugin API for the rgbcoloradjust photoart effect.
The TEffectRgbColorAdjust uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectRgbColorAdjust are passed
verbatim to this interface. For further description of required functionality, 
see TEffectRgbColorAdjust.

@see TEffectRgbColorAdjust
@publishedPartner
@released
*/	
class MEffectRgbColorAdjust : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectRgbColorAdjust::RedLevelL()
	
	@see TEffectRgbColorAdjust::RedLevelL()
	*/
	virtual TReal32 RedLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectRgbColorAdjust::GreenLevelL()
	
	@see TEffectRgbColorAdjust::GreenLevelL()
	*/
	virtual TReal32 GreenLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectRgbColorAdjust::BlueLevelL()
	
	@see TEffectRgbColorAdjust::BlueLevelL()
	*/
	virtual TReal32 BlueLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectRgbColorAdjust::SetRGBLevelL(TReal32 aRedLevel, TReal32 aGreenLevel, TReal32 aBlueLevel)
	
	@see TEffectRgbColorAdjust::SetRGBLevelL(TReal32 aRedLevel, TReal32 aGreenLevel, TReal32 aBlueLevel)
	*/
	virtual void SetRGBLevelL(TReal32 aRedLevel, TReal32 aGreenLevel, TReal32 aBlueLevel) = 0;
	};

/**
Plugin API for the emboss photoart effect.
The TEffectEmboss uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectEmboss are passed
verbatim to this interface. For further description of required functionality, 
see TEffectEmboss.

@see TEffectEmboss
@publishedPartner
@released
*/		
class MEffectEmboss : public MEffect
	{
	};	
	
/**
Plugin API for the solarize photoart effect.
The TEffectSolarize uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectSolarize are passed
verbatim to this interface. For further description of required functionality, 
see TEffectSolarize.

@see TEffectSolarize
@publishedPartner
@released
*/	
class MEffectSolarize : public MEffect
	{
	};
	
/**
Plugin API for the posterize photoart effect.
The TEffectPosterize uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectPosterize are passed
verbatim to this interface. For further description of required functionality, 
see TEffectPosterize.

@see TEffectPosterize
@publishedPartner
@released
*/	
class MEffectPosterize : public MEffect
	{
	};	

/**
Plugin API for the stamp photoart effect.
The TEffectStamp uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectStamp are passed
verbatim to this interface. For further description of required functionality, 
see TEffectStamp.

@see TEffectStamp
@publishedPartner
@released
*/	
class MEffectStamp : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectStamp::SmoothnessLevelL()
	
	@see TEffectStamp::SmoothnessLevelL()
	*/
	virtual TInt SmoothnessLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectStamp::ThresholdLevelL()
	
	@see TEffectStamp::ThresholdLevelL()
	*/
	virtual TInt ThresholdLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectStamp::SetStampLevelL(TInt aSmoothnessLevel, TInt aThresholdLevel)
	
	@see TEffectStamp::SetStampLevelL(TInt aSmoothnessLevel, TInt aThresholdLevel)
	*/
	virtual void SetStampLevelL(TInt aSmoothnessLevel, TInt aThresholdLevel) = 0;
	};		

/**
Plugin API for the sketch photoart effect.
The TEffectSketch uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectSketch are passed
verbatim to this interface. For further description of required functionality, 
see TEffectSketch.

@see TEffectSketch
@publishedPartner
@released
*/		
class MEffectSketch : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectSketch::SketchModeL()
	
	@see TEffectSketch::SketchModeL()
	*/
	virtual TEffectSketch::TSketchMode SketchModeL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectSketch::SetSketchModeL(TSketchMode aSketchMode)
	
	@see TEffectSketch::SetSketchModeL(TSketchMode aSketchMode)
	*/
	virtual void SetSketchModeL(TEffectSketch::TSketchMode aSketchMode) = 0;
	};		

/**
Plugin API for the noise photoart effect.
The TEffectNoise uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectNoise are passed
verbatim to this interface. For further description of required functionality, 
see TEffectNoise.

@see TEffectNoise
@publishedPartner
@released
*/		
class MEffectNoise  : public MEffect
	{
	};			

/**
Plugin API for the moonlight photoart effect.
The TEffectMoonlight uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectMoonlight are passed
verbatim to this interface. For further description of required functionality, 
see TEffectMoonlight.

@see TEffectMoonlight
@publishedPartner
@released
*/		
class MEffectMoonlight : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectMoonlight::LevelL()
	
	@see TEffectMoonlight::ClockL()
	*/
	virtual TInt16 ClockL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectMoonlight::ApplyL(TInt aClock)
	
	@see TEffectMoonlight::SetClockL(TInt16 aClock)
	*/
	virtual void SetClockL(TInt16 aClock) = 0;
	};		
	
/**
Plugin API for the frostedglass photoart effect.
The TEffectFrostedGlass uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectFrostedGlass are passed
verbatim to this interface. For further description of required functionality, 
see TEffectFrostedGlass.

@see TEffectFrostedGlass
@publishedPartner
@released
*/	
class MEffectFrostedGlass  : public MEffect
	{
	};	

/**
Plugin API for the despeckle photoart effect.
The TEffectDespeckle uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectDespeckle are passed
verbatim to this interface. For further description of required functionality, 
see TEffectDespeckle.

@see TEffectDespeckle
@publishedPartner
@released
*/
class MEffectDespeckle  : public MEffect
	{
	};	

/**
Plugin API for the blur photoart effect.
The TEffectBlur uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectBlur are passed
verbatim to this interface. For further description of required functionality, 
see TEffectBlur.

@see TEffectBlur
@publishedPartner
@released
*/
class MEffectBlur  : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectBlur::RectL()
	
	@see TEffectBlur::RectL()
	*/
	virtual TRect RectL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectBlur::SetRectL(TRect aRect)
	
	@see TEffectBlur::SetRectL(TRect aRect)
	*/
	virtual void SetRectL(TRect aRect) = 0;
	};	
	
/**
Plugin API for the colorization photoart effect.
The TEffectColorization uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectColorization are passed
verbatim to this interface. For further description of required functionality, 
see TEffectColorization.

@see TEffectColorization
@publishedPartner
@released
*/	
class MEffectColorization : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectColorization::RgbL()
	
	@see TEffectColorization::ReferenceColorL()
	*/
	virtual TRgb ReferenceColorL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectColorization::LuminanceStrengthL()
	
	@see TEffectColorization::LuminanceStrengthL()
	*/
	virtual TInt LuminanceStrengthL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectColorization::ChrominanceStrengthL()
	
	@see TEffectColorization::ChrominanceStrengthL()
	*/
	virtual TInt ChrominanceStrengthL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectColorization::SetColorizationLevelL(TRgb aRgb, TInt aLuminanceStrength, TInt aChrominanceStrength)
	
	@see TEffectColorization::SetColorizationL(TRgb aReferenceColor, TInt aLuminanceStrength, TInt aChrominanceStrength)
	*/
	virtual void SetColorizationL(TRgb aReferenceColor, TInt aLuminanceStrength, TInt aChrominanceStrength) = 0;
	};	
	
/**
Plugin API for the cartoon photoart effect.
The TEffectCartoon uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectCartoon are passed
verbatim to this interface. For further description of required functionality, 
see TEffectCartoon.

@see TEffectCartoon
@publishedPartner
@released
*/	
class MEffectCartoon : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectCartoon::DistinctEdgesL()
	
	@see TEffectCartoon::DistinctEdgesL()
	*/	
	virtual TBool DistinctEdgesL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectCartoon::SetDistinctEdgesL(TBool aDistinctEdges)
	
	@see TEffectCartoon::SetDistinctEdgesL(TBool aDistinctEdges)
	*/
	virtual void SetDistinctEdgesL(TBool aDistinctEdges) = 0;
	};		
		
/**
Plugin API for the localboost effect.
The TEffectLocalBoost uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectLocalBoost are passed
verbatim to this interface. For further description of required functionality, 
see TEffectLocalBoost.

@see TEffectLocalBoost
@publishedPartner
@released
*/		
class MEffectLocalBoost : public MEffect
	{
	};		

/**
Plugin API for the colorboost effect.
The TEffectColorBoost uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectColorBoost are passed
verbatim to this interface. For further description of required functionality, 
see TEffectColorBoost.

@see TEffectColorBoost
@publishedPartner
@released
*/	
class MEffectColorBoost : public MEffect
	{
	};	

/**
Plugin API for the whitebalance effect.
The TEffectWhiteBalance uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectWhiteBalance are passed
verbatim to this interface. For further description of required functionality, 
see TEffectWhiteBalance.

@see TEffectWhiteBalance
@publishedPartner
@released
*/	
class MEffectWhiteBalance : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectWhiteBalance::BalanceModeL()
	
	@see TEffectWhiteBalance::BalanceModeL()
	*/
	virtual TEffectWhiteBalance::TWhiteBalanceMode WhiteBalanceModeL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectWhiteBalance::ReferenceWhiteColorL()
	
	@see TEffectWhiteBalance::ReferenceWhiteColorL()
	*/
	virtual TRgb ReferenceWhiteColorL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectWhiteBalance::SetWhiteBalanceL(TRgb aRgb)
	
	@see TEffectWhiteBalance::SetWhiteBalanceL(TWhiteBalanceMode aWhiteBalanceMode, TRgb aReferenceWhiteColor)
	*/
	virtual void SetWhiteBalanceL(TEffectWhiteBalance::TWhiteBalanceMode aWhiteBalanceMode, TRgb aReferenceWhiteColor) = 0;
	};		
	
/**
Plugin API for the autolevels effect.
The TEffectAutoLevels uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectAutoLevels are passed
verbatim to this interface. For further description of required functionality, 
see TEffectAutoLevels.

@see TEffectAutoLevels
@publishedPartner
@released
*/		
class MEffectAutoLevels  : public MEffect
	{
	};		

/**
Plugin API for the levels effect.
The TEffectLevels uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectLevels are passed
verbatim to this interface. For further description of required functionality, 
see TEffectLevels.

@see TEffectLevels
@publishedPartner
@released
*/	
class MEffectLevels : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectLevels::WhiteLevelL()
	
	@see TEffectLevels::WhiteLevelL()
	*/
	virtual TReal32 WhiteLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectLevels::GrayLevelL()
	
	@see TEffectLevels::GrayLevelL()
	*/
	virtual TReal32 GrayLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectLevels::BlackLevelL()
	
	@see TEffectLevels::BlackLevelL()
	*/
	virtual TReal32 BlackLevelL() const = 0;
	
	/**
	This must provide an implementation as defined by TEffectLevels::SetWGBLevelL(TReal32 aWhiteLevel, TReal32 aGrayLevel, TReal32 aBlackLevel)
	
	@see TEffectLevels::SetWGBLevelL(TReal32 aWhiteLevel, TReal32 aGrayLevel, TReal32 aBlackLevel)
	*/
	virtual void SetWGBLevelL(TReal32 aWhiteLevel, TReal32 aGrayLevel, TReal32 aBlackLevel) = 0;
	};		

/**
Plugin API for the exposure effect.
The TEffectExposure uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectExposure are passed
verbatim to this interface. For further description of required functionality, 
see TEffectExposure.

@see TEffectExposure
@publishedPartner
@released
*/	
class MEffectExposure : public MEffect
	{
	};

/**
Plugin API for the spotlight effect.
The TEffectSpotlight uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectSpotlight are passed
verbatim to this interface. For further description of required functionality, 
see TEffectSpotlight.

@see TEffectSpotlight
@publishedPartner
@released
*/	
class MEffectSpotLight : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectSpotlight::SpotLightOrientationL(TPoint &aPosition, TUint32& aRadius, TReal32& aTransitionSize)
	
	@see TEffectSpotlight::SpotLightOrientationL(TPoint &aPosition, TUint32& aRadius, TReal32& aTransitionSize)
	*/
	virtual void SpotLightOrientationL(TPoint &aPosition, TUint32& aRadius, TReal32& aTransitionSize) const = 0;
	
	/**
	This must provide an implementation as defined by TEffectSpotlight::SetSpotLightOrientationL(const TPoint& aPosition, TUint32 aRadius, TReal32 aTransitionSize)
	
	@see TEffectSpotlight::SetSpotLightOrientationL(const TPoint& aPosition, TUint32 aRadius, TReal32 aTransitionSize)
	*/
	virtual void SetSpotLightOrientationL(const TPoint& aPosition, TUint32 aRadius, TReal32 aTransitionSize) = 0;
	};

/**
Plugin API for the customized transformation effect.
The TEffectCustomTransformation uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectCustomTransformation are passed
verbatim to this interface. For further description of required functionality, 
see TEffectCustomTransformation.

@see TEffectCustomTransformation
@publishedPartner
@released
*/		
class MEffectCustomTransformation : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectCustomTransformation::SetInputL(const TDesC& aFilename)
	
	@see TEffectCustomTransformation::SetInputL(const TDesC& aFilename)
	*/
	virtual void SetInputL(const TDesC& aFilename) = 0;
	
	/**
	This must provide an implementation as defined by TEffectCustomTransformation::SetInputL(const TDesC8& aBuffer)
	
	@see TEffectCustomTransformation::SetInputL(const TDesC8& aBuffer)
	*/
	virtual void SetInputL(const TDesC8& aBuffer) = 0;
	};
	
/**
Plugin API for the Geometrical Operation
The TGeometricalOperation uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TGeometricalOperation are passed
verbatim to this interface. For further description of required functionality, 
see TGeometricalOperation.

@see TGeometricalOperation
@publishedPartner
@released
*/	
class MGeometricalOperation : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TGeometricalOperation::Operation()
	
	@see TGeometricalOperation::Operation()
	*/
	virtual CImgProcessor::TOperation Operation() const = 0;
	
	/**
	This must provide an implementation as defined by TGeometricalOperation::SetOperationL(CImgProcessor::TOperation aOperation)
	
	@see TGeometricalOperation::SetOperationL(CImgProcessor::TOperation aOperation)
	*/
	virtual void SetOperationL(CImgProcessor::TOperation aOperation) = 0;
	};

/**
Plugin API for the insert border effect.
The TEffectBorder uses this interface class to get access to the image prosessor plugin effect. 
Once this has been constructed, calls to method functions of TEffectBorder are passed
verbatim to this interface. For further description of required functionality, 
see TEffectBorder.

@see TEffectBorder
@publishedPartner
@released
*/		
class MEffectBorder : public MEffect
	{
public:	
	/**
	This must provide an implementation as defined by TEffectBorder::SetInputL(const TDesC& aFilename)
	
	@see TEffectBorder::SetInputL(const TDesC& aFilename)
	*/
	virtual void SetInputL(const TDesC& aFilename) = 0;
	
	/**
	This must provide an implementation as defined by TEffectBorder::SetInputL(const TDesC8& aBuffer)
	
	@see TEffectBorder::SetInputL(const TDesC8& aBuffer)
	*/
	virtual void SetInputL(const TDesC8& aBuffer) = 0;
	
	/**
	This must provide an implementation as defined by TEffectBorder::SetInputL(const CFbsBitmap& aBitmap)
	
	@see TEffectBorder::SetInputL(const TDesC8& aBitmap)
	*/
	virtual void SetInputL(const CFbsBitmap& aBitmap) = 0;
	};	
	
	}//namespace Plugin
	}//namespace ImageProcessor

#endif //__IMAGE_PROCESSOR_PLUGIN_EFFECT_H__
