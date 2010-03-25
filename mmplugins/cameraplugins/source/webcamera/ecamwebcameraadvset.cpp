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

#include <w32std.h>
#include <ecamerrors.h>
#include "ecamwebcameraadvset.h"


CWebCameraAdvSet::CWebCameraAdvSet(CWebCamera& aOwner)
			   :iOwner(aOwner)
	{
	}
	
CWebCameraAdvSet::~CWebCameraAdvSet()
	{
	iSupportedISORates.Close();
	}

CWebCameraAdvSet* CWebCameraAdvSet::NewL(CWebCamera& aOwner)
	{
	CWebCameraAdvSet* self = new(ELeave) CWebCameraAdvSet(aOwner);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

void CWebCameraAdvSet::Release()
	{
	}
	
void CWebCameraAdvSet::Init()
	{
	}
	
void CWebCameraAdvSet::ConstructL()
	{
	iSupportedISORates.Reset();
	iSupportedISORates.AppendL(KDefaultIsoRate);
	}

CCamera::CCameraAdvancedSettings::TCameraType CWebCameraAdvSet::CameraType() const
	{
	return CCamera::CCameraAdvancedSettings::ECameraPluggable;
	}

CCamera::CCameraAdvancedSettings::TCameraType CWebCameraAdvSet::CameraType(TInt /*aCameraIndex*/) const
	{
	return CCamera::CCameraAdvancedSettings::ECameraUnknown;
	}
	
TBool CWebCameraAdvSet::IsCameraPresent() const
	{
	return EFalse;
	}

TBool CWebCameraAdvSet::IsCameraPresent(TInt /*aCameraIndex*/) const
	{
	return EFalse;
	}
	
TInt CWebCameraAdvSet::CameraIndex() const
	{
	return KErrNotSupported;
	}
	
TInt CWebCameraAdvSet::SupportedStabilizationModes() const
	{
	return 0;
	}
	
CCamera::CCameraAdvancedSettings::TStabilizationMode CWebCameraAdvSet::StabilizationMode() const
	{
	return CCamera::CCameraAdvancedSettings::EStabilizationModeOff;
	}
	
void CWebCameraAdvSet::SetStabilizationMode(CCamera::CCameraAdvancedSettings::TStabilizationMode /*aStabilizationMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedFocusModes() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TFocusMode CWebCameraAdvSet::FocusMode() const
	{
	return CCamera::CCameraAdvancedSettings::EFocusModeAuto;
	}
	
void CWebCameraAdvSet::SetFocusMode(CCamera::CCameraAdvancedSettings::TFocusMode /*aFocusMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedFocusRanges() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TFocusRange CWebCameraAdvSet::FocusRange() const
	{
	return CCamera::CCameraAdvancedSettings::EFocusRangeAuto;
	}
	
void CWebCameraAdvSet::SetFocusRange(CCamera::CCameraAdvancedSettings::TFocusRange /*aFocusRange*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedAutoFocusTypes() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TAutoFocusType CWebCameraAdvSet::AutoFocusType() const
	{
    return CCamera::CCameraAdvancedSettings::EAutoFocusTypeOff;
	}
	
void CWebCameraAdvSet::SetAutoFocusType(CCamera::CCameraAdvancedSettings::TAutoFocusType /*aAutoFocusType*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedAutoFocusAreas() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TAutoFocusArea CWebCameraAdvSet::AutoFocusArea() const
	{
	return CCamera::CCameraAdvancedSettings::EAutoFocusTypeAuto;
	}
	
void CWebCameraAdvSet::SetAutoFocusArea(CCamera::CCameraAdvancedSettings::TAutoFocusArea /*aAutoFocusArea*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::FocusDistance() const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetFocusDistance(TInt /*aDistance*/) 
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::GetMinFocalLength() const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::GetSupportedIsoRatesL(RArray<TInt>& aSupportedIsoRates) const
	{
	if(iSupportedISORates.Count() == 0)
		{
		User::Leave(KErrNotSupported);
		}
	
	for(TInt index=0; index < iSupportedISORates.Count(); index++)
		{
		aSupportedIsoRates.AppendL(iSupportedISORates[index]);
		}
	}
	
TInt CWebCameraAdvSet::IsoRate() const
	{
	return KDefaultIsoRate;
	}

void CWebCameraAdvSet::SetIsoRate(TInt /*aRate*/)
	{
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::GetSupportedISORateTypeL(TInt& /*aSupportedISORateTypes*/) const
	{
	User::Leave(KErrNotSupported);
	}
		
void CWebCameraAdvSet::SetISORate(CCamera::CCameraAdvancedSettings::TISORateType /*aISORateType*/, TInt /*aParam*/)
	{
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::GetISORateL(CCamera::CCameraAdvancedSettings::TISORateType& /*aISORateType*/, TInt& /*aParam*/, TInt& /*aISORate*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetAperturesL(RArray<TInt>& /*aFStops*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::Aperture() const
	{
	return KErrNotSupported;
	}

void CWebCameraAdvSet::SetAperture(TInt /*aFStop*/)
	{	
	User::Leave(KErrNotSupported);
	}	
	
void CWebCameraAdvSet::GetShutterSpeedsL(RArray<TInt>& /*aShutterSpeeds*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::ShutterSpeed() const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetShutterSpeed(TInt /*aShutterSpeed*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedMeteringModes() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TMeteringMode CWebCameraAdvSet::MeteringMode() const
	{
	return CCamera::CCameraAdvancedSettings::EMeteringModeAuto;
	}
	
void CWebCameraAdvSet::SetMeteringMode(CCamera::CCameraAdvancedSettings::TMeteringMode /*aMeteringMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedDriveModes() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TDriveMode CWebCameraAdvSet::DriveMode() const
	{
	return CCamera::CCameraAdvancedSettings::EDriveModeAuto;
	}
	
void CWebCameraAdvSet::SetDriveMode(CCamera::CCameraAdvancedSettings::TDriveMode /*aDriveMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedBracketModes() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TBracketMode CWebCameraAdvSet::BracketMode() const
	{
	return CCamera::CCameraAdvancedSettings::EBracketModeOff;
	}
	
void CWebCameraAdvSet::SetBracketMode(CCamera::CCameraAdvancedSettings::TBracketMode /*aBracketMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedBracketParameters() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TBracketParameter CWebCameraAdvSet::BracketParameter() const
	{
	return CCamera::CCameraAdvancedSettings::EBracketParameterNone;
	}
	
void CWebCameraAdvSet::SetBracketParameter(CCamera::CCameraAdvancedSettings::TBracketParameter /*aBracketParameter*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedBracketSteps() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TBracketStep CWebCameraAdvSet::BracketStep() const
	{
	return CCamera::CCameraAdvancedSettings::EBracketStepNonConfig;
	}
	
void CWebCameraAdvSet::SetBracketStep(CCamera::CCameraAdvancedSettings::TBracketStep /*aBracketStep*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetBracketMerge(TInt& /*aStartIndex*/, TInt& /*aFrames*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetBracketMerge(TInt /*aStartIndex*/ , TInt /*aFrames*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedFlashModes() const
	{
	return KErrNotSupported;
	}
	
CCamera::TFlash CWebCameraAdvSet::FlashMode() const
	{
	return CCamera::EFlashNone;
	}
	
void CWebCameraAdvSet::SetFlashMode(CCamera::TFlash /*aMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TBool CWebCameraAdvSet::RedEyeReduceOn() const
	{
	return EFalse;
	}
	
void CWebCameraAdvSet::SetRedEyeReduceOn(TBool /*aState*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetFlashCompensationStepsL(RArray<TInt>& /*aFlashCompensationSteps*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::FlashCompensationStep() const
	{
	return KErrNotSupported;
	}

TInt CWebCameraAdvSet::GetFlashCompensationStep(TInt& /*aFlashCompensationStep*/) const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::GetFlashCompensationRangeInSteps(TInt& /*aNegativeCompensation*/, TInt& /*aPositiveCompensation*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetFlashCompensationStep(TInt /*aFlashCompensationStep*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::FlashCompensation() const
	{
	return KErrNotSupported;
	}
	
TInt CWebCameraAdvSet::GetFlashCompensation(TInt& /*aFlashCompensation*/) const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetFlashCompensation(TInt /*aFlashCompensation*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TBool CWebCameraAdvSet::IsExternalFlashPresent() const
	{
	return EFalse;
	}
	
void CWebCameraAdvSet::GetManualFlashPowerLevelsL(RArray<TInt>& /*aManualFlashPowerLevels*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::ManualFlashPowerLevel() const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetManualFlashPowerLevel(TInt /*aManualFlashPowerLevel*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedExposureModes() const
	{
	return CCamera::EExposureAuto;
	}
	
CCamera::TExposure CWebCameraAdvSet::ExposureMode() const
	{
	return CCamera::EExposureAuto;
	}

void CWebCameraAdvSet::SetExposureMode(CCamera::TExposure /*aExposureMode*/)
	{
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::GetExposureCompensationStepsL(RArray<TInt>& aExposureCompensationSteps, TValueInfo& aInfo) const
	{
	aExposureCompensationSteps.Reset();
	aInfo = ENotActive;
	}

void CWebCameraAdvSet::GetExposureCompensationRangeInSteps(TInt& /*aNegativeCompensation*/, TInt& /*aPositiveCompensation*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::ExposureCompensationStep() const
	{
	return 0;
	}
	
TInt CWebCameraAdvSet::GetExposureCompensationStep(TInt& /*aExposureCompensationStep*/) const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetExposureCompensationStep(TInt /*aExposureCompensationStep*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::ExposureCompensation() const
	{
	return KErrNotSupported;
	}
	
TInt CWebCameraAdvSet::GetExposureCompensation(TInt& /*aExposureCompensation*/) const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetExposureCompensation(TInt /*aExposureCompensation*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedWhiteBalanceModes() const
	{
	return KErrNotSupported;
	}
	
CCamera::TWhiteBalance CWebCameraAdvSet::WhiteBalanceMode() const
	{
	return CCamera::EWBAuto;
	}
	
void CWebCameraAdvSet::SetWhiteBalanceMode(CCamera::TWhiteBalance /*aWhiteBalanceMode*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TBool CWebCameraAdvSet::ApertureExposureLockOn() const
	{
	return EFalse;
	}
	
void CWebCameraAdvSet::SetApertureExposureLockOn(TBool /*aAELock*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TBool CWebCameraAdvSet::ShootClickOn() const
	{
	return EFalse;
	}
	
void CWebCameraAdvSet::SetShootClickOn(TBool /*aShootClickOn*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetTimerIntervalsL(RArray<TInt>& /*aTimerIntervals*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}

TInt CWebCameraAdvSet::TimerInterval() const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetTimerInterval(TInt /*aTimerInterval*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetTimeLapsePeriodRange(TTime& /*aTimeLapseMin*/, TTime& /*aTimeLapseMax*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetTimeLapse(TTime& /*aStart*/, TTime& /*aEnd*/, TTime& /*aInterval*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetTimeLapse(const TTime& /*aStart*/, const TTime& /*aEnd*/, const TTime& /*aInterval*/)
	{
	User::Leave(KErrNotSupported);
	}
	
CCamera::CCameraAdvancedSettings::TPictureOrientation CWebCameraAdvSet::PictureOrientation() const
	{
	return CCamera::CCameraAdvancedSettings::EPictureOrientationUnknown;
	}
	
void CWebCameraAdvSet::SetPictureOrientation(CCamera::CCameraAdvancedSettings::TPictureOrientation /*aOrientation*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedPixelAspectRatios() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TPixelAspectRatio CWebCameraAdvSet::PixelAspectRatio() const
	{
	return CCamera::CCameraAdvancedSettings::EPixelAspectUnknown;
	}
	
void CWebCameraAdvSet::SetPixelAspectRatio(CCamera::CCameraAdvancedSettings::TPixelAspectRatio /*aPixelAspectRatio*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::SupportedYuvRanges() const
	{
	return KErrNotSupported;
	}
	
CCamera::CCameraAdvancedSettings::TYuvRange CWebCameraAdvSet::YuvRange() const
	{
	return CCamera::CCameraAdvancedSettings::EYuvRangeUnknown;
	}
	
void CWebCameraAdvSet::SetYuvRange(CCamera::CCameraAdvancedSettings::TYuvRange /*aYuvRange*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::BurstImages() const
	{
	return KErrNotSupported;
	}
	
void CWebCameraAdvSet::SetBurstImages(TInt /*aImages*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetOpticalZoomStepsL(RArray<TInt>& /*aOpticalZoomSteps*/, TValueInfo& /*aInfo*/) const
	{	
	User::Leave(KErrNotSupported);
	}
	
TInt CWebCameraAdvSet::OpticalZoom() const
	{
	return KECamFineResolutionFactor;
	}
	
void CWebCameraAdvSet::SetOpticalZoom(TInt /*aOpticalZoom*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetDigitalZoomStepsL(RArray<TInt>& aDigitalZoomSteps, TValueInfo& aInfo) const
	{
	aDigitalZoomSteps.Reset();
	aDigitalZoomSteps.AppendL(KECamFineResolutionFactor);
	aInfo = EDiscreteSteps;
	}
	
void CWebCameraAdvSet::GetDigitalZoomStepsForStillL(RArray<TInt>& aDigitalZoomSteps, TValueInfo& aInfo, TInt /*aSizeIndex*/, 
								 CCamera::TFormat /*aFormat*/, TBool& /*aIsInfluencePossible*/) const
	{
	aDigitalZoomSteps.Reset();
	aDigitalZoomSteps.AppendL(KECamFineResolutionFactor);
	aInfo = EDiscreteSteps;
	}

void CWebCameraAdvSet::GetDigitalZoomStepsForVideoL(RArray<TInt>& aDigitalZoomSteps, TValueInfo& aInfo, TInt /*aFrameRateIndex*/,
								 TInt /*aSizeIndex*/, CCamera::TFormat /*aFormat*/, TBool& /*aIsInfluencePossible*/, CCamera::TExposure /*aExposure*/) const
	{
	aDigitalZoomSteps.Reset();
	aDigitalZoomSteps.AppendL(KECamFineResolutionFactor);
	aInfo = EDiscreteSteps;
	}

TInt CWebCameraAdvSet::DigitalZoom() const
	{
	return KErrNotSupported;
	}

void CWebCameraAdvSet::SetDigitalZoom(TInt /*aDigitalZoom*/)
	{
	User::Leave(KErrNotSupported);
	}
	
TBool CWebCameraAdvSet::ExposureLockOn() const
	{
	return EFalse;
	}

void CWebCameraAdvSet::SetExposureLockOn(TBool /*aState*/)
	{
	User::Leave(KErrNotSupported);
	}
		
TBool CWebCameraAdvSet::AutoFocusLockOn() const
	{
	return EFalse;
	}

void CWebCameraAdvSet::SetAutoFocusLockOn(TBool /*aState*/)
	{	
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::GetSupportedSettingsL(RArray<TUid>& /*aSettings*/) const
	{
	User::Leave(KErrNotSupported);
	}
    
void CWebCameraAdvSet::GetActiveSettingsL(RArray<TUid>& /*aActiveSettings*/) const
	{
	User::Leave(KErrNotSupported);
	}
 
void CWebCameraAdvSet::GetDisabledSettingsL(RArray<TUid>& /*aDisbledSettings*/) const
	{
	User::Leave(KErrNotSupported);
	}
	    
void CWebCameraAdvSet::SetAutomaticSizeSelectionChangeOn(TBool /*aSetOn*/)
	{
	}
	
TBool CWebCameraAdvSet::AutomaticSizeSelectionChangeOn() const
	{
	return EFalse;
	}

void CWebCameraAdvSet::GetSupportedContinuousAutoFocusTimeoutsL(RArray<TInt>& /*aTimeouts*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetContinuousAutoFocusTimeout(TInt /*aTimeout*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetStabilizationEffect(CCamera::CCameraAdvancedSettings::TStabilizationEffect /*aEffect*/)
	{
	User::Leave(KErrNotSupported);
	}
	
CCamera::CCameraAdvancedSettings::TStabilizationEffect CWebCameraAdvSet::StabilizationEffect() const
	{
	return CCamera::CCameraAdvancedSettings::EStabilizationOff;
	}

TInt CWebCameraAdvSet::SupportedStabilizationEffects() const
	{
	return CCamera::CCameraAdvancedSettings::EStabilizationOff;
	}

TInt CWebCameraAdvSet::SupportedStabilizationComplexityValues() const
	{
	return CCamera::CCameraAdvancedSettings::EStabilizationOff;
	}
	
CCamera::CCameraAdvancedSettings::TStabilizationAlgorithmComplexity CWebCameraAdvSet::StabilizationComplexity() const
	{
	return CCamera::CCameraAdvancedSettings::EStabilizationComplexityAuto;
	}
	
void CWebCameraAdvSet::SetStabilizationComplexity(CCamera::CCameraAdvancedSettings::TStabilizationAlgorithmComplexity /*aComplexity*/)
	{
	User::Leave(KErrNotSupported);
	}
	
CCamera::CCameraAdvancedSettings::TWBUnits CWebCameraAdvSet::SupportedWBUnits() const
	{
	return CCamera::CCameraAdvancedSettings::EWBUnknown;
	}
	
void CWebCameraAdvSet::SetWBRgbValue(const TRgb& /*aValue*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetWBRgbValue(TRgb& /*aValue*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetWBSupportedColorTemperaturesL(RArray<TInt>& /*aWBColorTemperatures*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetWBColorTemperature(TInt /*aColorTemperature*/)
	{
	User::Leave(KErrNotSupported);
	}

TInt CWebCameraAdvSet::WBColorTemperature() const
	{
	return KErrNotSupported;
	}

TInt CWebCameraAdvSet::ContinuousAutoFocusTimeout() const
	{
	return KErrNotSupported;
	}

TInt CWebCameraAdvSet::IsFlashReady(TBool& aReady) const
	{
	aReady = 0;
	return KErrNone;
	}

void CWebCameraAdvSet::GetCurrentFocusModeStepsL(RArray<TInt>& /*aFocusModeSteps*/, TValueInfo& /*aInfo*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::SetReferenceScreen(CWsScreenDevice& /*aScreenDevice*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetPreCaptureWarningSupportedL(CCamera::CCameraAdvancedSettings::TCameraMode /*aCameraMode*/, TInt& /*aPreCaptureWarningSupported*/) const
	{
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::SubscribeToPreCaptureWarningL(TInt /*aPreCaptureWarning*/)
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::UnSubscribePreCaptureWarningL()
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetPreCaptureWarningL(TInt& /*aPreCaptureWarning*/) const
	{
	User::Leave(KErrNotSupported);
	}
	
void CWebCameraAdvSet::GetSupportedAFAssistantLightL(TInt& /*aSupportedAFAssistantLight*/) const
	{
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::GetAFAssistantLightL(CCamera::CCameraAdvancedSettings::TAFAssistantLight& /*aAFAssistantLight*/) const
	{
	User::Leave(KErrNotSupported);
	}

void CWebCameraAdvSet::SetAFAssistantLight(CCamera::CCameraAdvancedSettings::TAFAssistantLight /*aAFAssistantLight*/)
	{
	User::Leave(KErrNotSupported);
	}
	

