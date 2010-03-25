/*
* Copyright (c) 2010 ISB.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* ISB - Initial contribution
*
* Contributors:
*
* Description:
*
*/

#ifndef ECAMWEBCAMERAADVSET_H
#define ECAMWEBCAMERAADVSET_H

#include <ecam/ecamadvsettingsintf.h>
#include "ecamwebcameraplugin.h"


const TInt KDefaultIsoRate = 200;

// Implementations of the MCameraAdvancedSettings class
class CWebCameraAdvSet : public CBase, 
						 public MCameraAdvancedSettings,
						 public MCameraAdvancedSettings2,
						 public MCameraAdvancedSettings3
	{
	friend class CWebCamera;
public:
	static CWebCameraAdvSet* NewL(CWebCamera& aOwner);
	~CWebCameraAdvSet();
	
protected:
	//from MCameraAdvancedSettings
	void Release();
  	CCamera::CCameraAdvancedSettings::TCameraType CameraType() const;
  	CCamera::CCameraAdvancedSettings::TCameraType CameraType(TInt aCameraIndex) const;
	TBool IsCameraPresent() const;
 	TBool IsCameraPresent(TInt aCameraIndex) const;
	TInt CameraIndex() const;	
    TInt SupportedStabilizationModes() const;
    CCamera::CCameraAdvancedSettings::TStabilizationMode StabilizationMode() const;
    void SetStabilizationMode(CCamera::CCameraAdvancedSettings::TStabilizationMode aStabilizationMode);
    TInt SupportedFocusModes() const;
    CCamera::CCameraAdvancedSettings::TFocusMode FocusMode() const;
  	void SetFocusMode(CCamera::CCameraAdvancedSettings::TFocusMode aFocusMode);
  	TInt SupportedFocusRanges() const;
    CCamera::CCameraAdvancedSettings::TFocusRange FocusRange() const;
    void SetFocusRange(CCamera::CCameraAdvancedSettings::TFocusRange aFocusRange);
    TInt SupportedAutoFocusTypes() const;
    CCamera::CCameraAdvancedSettings::TAutoFocusType AutoFocusType() const;
    void SetAutoFocusType(CCamera::CCameraAdvancedSettings::TAutoFocusType aAutoFocusType);
	TInt SupportedAutoFocusAreas() const;
    CCamera::CCameraAdvancedSettings::TAutoFocusArea AutoFocusArea() const;
    void SetAutoFocusArea(CCamera::CCameraAdvancedSettings::TAutoFocusArea aAutoFocusArea);
	TInt FocusDistance() const;
	void SetFocusDistance(TInt aDistance);
	TInt GetMinFocalLength() const;
	void GetSupportedIsoRatesL(RArray<TInt>& aSupportedIsoRates) const;
    TInt IsoRate() const;
    void SetIsoRate(TInt aRate);
    void GetAperturesL(RArray<TInt>& aFStops, TValueInfo& aInfo) const;
    TInt Aperture() const; 
    void SetAperture(TInt aFStop); 
	void GetShutterSpeedsL(RArray<TInt>& aShutterSpeeds, TValueInfo& aInfo) const; 
    TInt ShutterSpeed() const; 
    void SetShutterSpeed(TInt aShutterSpeed); 
    TInt SupportedMeteringModes() const;
    CCamera::CCameraAdvancedSettings::TMeteringMode MeteringMode() const;
    void SetMeteringMode(CCamera::CCameraAdvancedSettings::TMeteringMode aMeteringMode);
    TInt SupportedDriveModes() const;
    CCamera::CCameraAdvancedSettings::TDriveMode DriveMode() const;
    void SetDriveMode(CCamera::CCameraAdvancedSettings::TDriveMode aDriveMode);
    TInt SupportedBracketModes() const;
    CCamera::CCameraAdvancedSettings::TBracketMode BracketMode() const;
    void SetBracketMode(CCamera::CCameraAdvancedSettings::TBracketMode aBracketMode);
    TInt SupportedBracketParameters() const;
    CCamera::CCameraAdvancedSettings::TBracketParameter BracketParameter() const;
    void SetBracketParameter(CCamera::CCameraAdvancedSettings::TBracketParameter aBracketParameter);
    TInt SupportedBracketSteps() const;
    CCamera::CCameraAdvancedSettings::TBracketStep BracketStep() const;
    void SetBracketStep(CCamera::CCameraAdvancedSettings::TBracketStep aBracketStep);
	void GetBracketMerge(TInt& aStartIndex, TInt& aFrames) const;
	void SetBracketMerge(TInt aStartIndex, TInt aFrames);
    TInt SupportedFlashModes() const;
	CCamera::TFlash FlashMode() const;
	void SetFlashMode(CCamera::TFlash aMode);
    TBool RedEyeReduceOn() const;
    void SetRedEyeReduceOn(TBool aState);
	void GetFlashCompensationStepsL(RArray<TInt>& aFlashCompensationSteps, TValueInfo& aInfo) const; 
	TInt FlashCompensationStep() const;
	TInt GetFlashCompensationStep(TInt& aFlashCompensationStep) const;
	void SetFlashCompensationStep(TInt aFlashCompensationStep);
	void GetFlashCompensationRangeInSteps(TInt& aNegativeCompensation, TInt& aPositiveCompensation) const;
	TInt FlashCompensation() const;
	TInt GetFlashCompensation(TInt& aFlashCompensation) const;
	void SetFlashCompensation(TInt aFlashCompensationSteps);
	TBool IsExternalFlashPresent() const;
    void GetManualFlashPowerLevelsL(RArray<TInt>& aManualFlashPowerLevels, TValueInfo& aInfo) const;
    TInt ManualFlashPowerLevel() const;
    void SetManualFlashPowerLevel(TInt aManualFlashPowerLevel);
	TInt SupportedExposureModes() const;
	CCamera::TExposure ExposureMode() const;
	void SetExposureMode(CCamera::TExposure aExposureMode);
	void GetExposureCompensationStepsL(RArray<TInt>& aExposureCompensationSteps, TValueInfo& aInfo) const; 
	TInt ExposureCompensationStep() const;
	TInt GetExposureCompensationStep(TInt& aExposureCompensationStep) const;
	void SetExposureCompensationStep(TInt aExposureCompensationStep);
	void GetExposureCompensationRangeInSteps(TInt& aNegativeCompensation, TInt& aPositiveCompensation) const;
	TInt ExposureCompensation() const;
	TInt GetExposureCompensation(TInt& aExposureCompensation) const;
	void SetExposureCompensation(TInt aExposureCompensationSteps);
	TInt SupportedWhiteBalanceModes() const;
	CCamera::TWhiteBalance WhiteBalanceMode() const;
	void SetWhiteBalanceMode(CCamera::TWhiteBalance aWhiteBalanceMode);
	TBool ApertureExposureLockOn() const;
	void SetApertureExposureLockOn(TBool aAELock);
	TBool ShootClickOn() const;
	void SetShootClickOn(TBool aShootClickOn);
	void GetTimerIntervalsL(RArray<TInt>& aTimerIntervals, TValueInfo& aInfo) const;
    TInt TimerInterval() const;  
    void SetTimerInterval(TInt aTimerInterval); 
	void GetTimeLapsePeriodRange(TTime& aTimeLapseMin, TTime& aTimeLapseMax) const;
    void GetTimeLapse(TTime& aStart, TTime& aEnd, TTime& aInterval) const;  
    void SetTimeLapse(const TTime& aStart, const TTime& aEnd, const TTime& aInterval); 
    CCamera::CCameraAdvancedSettings::TPictureOrientation PictureOrientation() const;
    void SetPictureOrientation(CCamera::CCameraAdvancedSettings::TPictureOrientation aOrientation); 
    TInt SupportedPixelAspectRatios() const;
    CCamera::CCameraAdvancedSettings::TPixelAspectRatio PixelAspectRatio() const;
    void SetPixelAspectRatio(CCamera::CCameraAdvancedSettings::TPixelAspectRatio aPixelAspectRatio); 
    TInt SupportedYuvRanges() const;
    CCamera::CCameraAdvancedSettings::TYuvRange YuvRange() const;
    void SetYuvRange(CCamera::CCameraAdvancedSettings::TYuvRange aYuvRange);
    TInt BurstImages() const;
    void SetBurstImages(TInt aImages);	
	void GetOpticalZoomStepsL(RArray<TInt>& aOpticalZoomSteps, TValueInfo& aInfo) const;
	TInt OpticalZoom() const;
	void SetOpticalZoom(TInt aOpticalZoom);
	void GetDigitalZoomStepsL(RArray<TInt>& aDigitalZoomSteps, TValueInfo& aInfo) const;
	TInt DigitalZoom() const;
	void SetDigitalZoom(TInt aDigitalZoom);
	TBool ExposureLockOn() const;
	void SetExposureLockOn(TBool aState);
	TBool AutoFocusLockOn() const;
	void SetAutoFocusLockOn(TBool aState);
    void GetSupportedSettingsL(RArray<TUid>& aSettings) const;
    void GetActiveSettingsL(RArray<TUid>& aActiveSettings) const;
    void GetDisabledSettingsL(RArray<TUid>& aDisabledSettings) const;
	TBool AutomaticSizeSelectionChangeOn() const;	
	void SetAutomaticSizeSelectionChangeOn(TBool aSetOn);	
    void GetSupportedContinuousAutoFocusTimeoutsL(RArray<TInt>& aTimeouts, TValueInfo& aInfo) const;
    TInt ContinuousAutoFocusTimeout() const; 
    void SetContinuousAutoFocusTimeout(TInt aTimeout);
	TInt SupportedStabilizationEffects() const;
	CCamera::CCameraAdvancedSettings::TStabilizationEffect StabilizationEffect() const;
    void SetStabilizationEffect(CCamera::CCameraAdvancedSettings::TStabilizationEffect aEffect);
	TInt SupportedStabilizationComplexityValues() const;
	CCamera::CCameraAdvancedSettings::TStabilizationAlgorithmComplexity StabilizationComplexity() const;
	void SetStabilizationComplexity(CCamera::CCameraAdvancedSettings::TStabilizationAlgorithmComplexity aComplexity);
    CCamera::CCameraAdvancedSettings::TWBUnits SupportedWBUnits() const;
    void GetWBRgbValue(TRgb& aValue) const;
    void SetWBRgbValue(const TRgb& aValue);
  	void GetWBSupportedColorTemperaturesL(RArray<TInt>& aWBColorTemperatures, TValueInfo& aInfo) const;
 	TInt WBColorTemperature() const;   
	void SetWBColorTemperature(TInt aWBColorTemperature);
	
	//from MCameraAdvancedSettings2
	TInt IsFlashReady(TBool& aReady) const;
	void GetCurrentFocusModeStepsL(RArray<TInt>& aFocusModeSteps, TValueInfo& aInfo) const;
	
	//from MCameraAdvancedSettings3
	void GetSupportedISORateTypeL(TInt& aSupportedISORateTypes) const;
	void SetISORate(CCamera::CCameraAdvancedSettings::TISORateType aISORateType, TInt aParam);
	void GetISORateL(CCamera::CCameraAdvancedSettings::TISORateType& aISORateType, TInt& aParam, TInt& aISORate) const;
	void SetReferenceScreen(CWsScreenDevice& aScreenDevice);
	void GetDigitalZoomStepsForStillL(RArray<TInt>& aDigitalZoomSteps, TValueInfo& aInfo, TInt aSizeIndex, 
					CCamera::TFormat aFormat, TBool& aIsInfluencePossible) const;
	void GetDigitalZoomStepsForVideoL(RArray<TInt>& aDigitalZoomSteps, TValueInfo& aInfo, TInt aFrameRateIndex, 
					TInt aSizeIndex, CCamera::TFormat aFormat, TBool& aIsInfluencePossible, CCamera::TExposure aExposure) const;
	void GetPreCaptureWarningSupportedL(CCamera::CCameraAdvancedSettings::TCameraMode aCameraMode, TInt& aPreCaptureWarningSupported) const;
	void SubscribeToPreCaptureWarningL(TInt aPreCaptureWarning);
	void UnSubscribePreCaptureWarningL();
	void GetPreCaptureWarningL(TInt& aPreCaptureWarning) const;
	void GetSupportedAFAssistantLightL(TInt& aSupportedAFAssistantLight) const;
	void GetAFAssistantLightL(CCamera::CCameraAdvancedSettings::TAFAssistantLight& aAFAssistantLight) const;
	void SetAFAssistantLight(CCamera::CCameraAdvancedSettings::TAFAssistantLight aAFAssistantLight);

private:
	CWebCameraAdvSet(CWebCamera& aOwner);
	void Init();
	void ConstructL();
	
private:
	CWebCamera& iOwner;
	RArray<TInt> iSupportedISORates;
	};

#endif // TESTADVANCEDSETTINGS_H
