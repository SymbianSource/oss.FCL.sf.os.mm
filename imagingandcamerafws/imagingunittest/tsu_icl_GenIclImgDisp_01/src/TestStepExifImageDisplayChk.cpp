// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Test system includes
#include "TestSuiteExifImageDisplay.h"
#include "TestStepExifImageDisplay.h"
#include <icl/geniclimagedisplayext.h>

CTestStepWithCheckpoint::CTestStepWithCheckpoint(const TImageDisplayUnitTestParams& aTestParams):
																		CTestStepImgDisplayGeneric(aTestParams),
																		iNextCheckPoint(aTestParams.iCheckPoints),
																		iFirstCheckPoint(aTestParams.iCheckPoints)
	{
	}


void CTestStepWithCheckpoint::ResetCheckPoints()
	{
	iNextCheckPoint = iFirstCheckPoint;
	}

TVerdict CTestStepWithCheckpoint::CheckBitmap(const TDesC& aRefFileName, TBool aIsMask)
	{
	TBuf<64> fileName;
	TParse parse;
	parse.Set(aRefFileName, NULL, NULL);
	TPtrC name(parse.Name());
	fileName.AppendFormat(_L("%S_%s_%d.mbm"),&name,(aIsMask? _S("mask"):_S("frame")), iFrameNumber);
	TRAP(iLastError, SetDestFilenameL(fileName) );
	if (iLastError != KErrNone)
		{
		return EFail;
		}
	SetRefFilename(aRefFileName);
	if (!iImageDisplay->ValidBitmap())
		{
		iLastError = KErrGeneral;
		INFO_PRINTF1(_L("Error: got ValidBitmap()==EFalse"));
		return EFail;
		}
	const CFbsBitmap* mask=NULL;
	const CFbsBitmap* frame=NULL;
	iImageDisplay->GetBitmap(frame, mask);
	frame = aIsMask? mask : frame;
	if (frame == NULL)
		{
		iLastError = KErrGeneral;
		INFO_PRINTF1(_L("Error: GetBitmap() returned NULL"));
		return EFail;
		}
	TRAP(iLastError, iLastError=CompareBitmapWithFileL(*frame,  RefFilename(), aIsMask));
	if (iLastError != KErrNone )
		{
		if (!(iLastError==KErrNoMemory && AllocMode()))
			{
			INFO_PRINTF4(_L("Images Do not match (Ref=%s  Got=%S Err=%d)"), iNextCheckPoint->iReferenceFileName, &fileName, iLastError);
			const_cast<CFbsBitmap*>(frame)->Save(DestFilename());
			}
		return EFail;
		}
	return EPass;
	}

TBool CTestStepWithCheckpoint::ConsumeFrame(const CFbsBitmap* aBitmap, TBool aNoDelayCheck)
	{
	++iFrameNumber;
	TBool ContinueTest=(iTestParams->iMaxFramesToDecode==TImageDisplayUnitTestParams::KNoFrameLimit || iFrameNumber<iTestParams->iMaxFramesToDecode);
	if (!iImageDisplay->ValidBitmap())
		{
		iLatestCheckpointVerdict = EFail;
		INFO_PRINTF1(_L("---ERROR: Bitmap is said to be Invalid!"));
		return EFalse;
		}
	if (aBitmap==NULL)
		{
		iLatestCheckpointVerdict = EFail;
		INFO_PRINTF1(_L("---ERROR: Bitmap is NULL - aborting!"));
		return EFalse;
		}

	if (iNextCheckPoint && iNextCheckPoint->IsValid())
		{
		TBool framePoint=EFalse;
		if (iNextCheckPoint->LastFramePoint() && (iLastPluginStatus&KLastFrameReadyStatus)==KLastFrameReadyStatus)
			{
			framePoint=ETrue;
			}
		else
			{
			framePoint=(iNextCheckPoint->iAbsoluteFrameNumber == iFrameNumber);
			}

		if (framePoint)
			{
			iLatestCheckpointVerdict = EFail;
			TTime Now;
			Now.HomeTime();
			INFO_PRINTF3(_L("Checkpoint Frame# %d Plugin Status=0x%04x "), iFrameNumber, iLastPluginStatus);
// perform perpixel image checking //
			if (NULL != iNextCheckPoint->iReferenceFileName)
				{
				if(!iTestStepName.Compare(_L("MM-ICL-GENICLIMGDISP-U-0011-HP")))
					{
					if(iIsNaviEngine) 
						{
						_LIT(KNERefFileName, "wmf3_8_ne.mbm");
						if (EPass != (iLatestCheckpointVerdict = CheckBitmap(KNERefFileName, EFalse)))
							{
							return EFalse;
							}
						}
					else if (iIsH6)
						{
						//DTW-MM00501 - Reference images changed for OMAP 3430/H6
						_LIT(KNERefFileNameH6, "wmf3_8_h6.mbm");
						if (EPass != (iLatestCheckpointVerdict = CheckBitmap(KNERefFileNameH6, EFalse)))
							{
							return EFalse;
							}	
						}
					}
				else
					{
					if (EPass != (iLatestCheckpointVerdict = CheckBitmap(TPtrC(iNextCheckPoint->iReferenceFileName), EFalse)))
						{
						return EFalse;
						}
					}
				}
			if (NULL != iNextCheckPoint->iReferenceMaskFileName)
					{
					if (EPass != (iLatestCheckpointVerdict = CheckBitmap(TPtrC(iNextCheckPoint->iReferenceMaskFileName), ETrue)))
						{
						return EFalse;
						}
				}
// perfrom timing checking //
			if (iNextCheckPoint->iDelayFromPreviuos != TFrameCheckPoint::KNodelayChecking)
				{
				if (iTestParams->iTestType == TImageDisplayUnitTestParams::EAlloc)
					{
					INFO_PRINTF1(_L("---Timing checking skipped due to the ALLOC mode"));
					}
				else if (aNoDelayCheck)
					{
					INFO_PRINTF1(_L("---Timing checking skipped because aNoDelayCheck enforced"));
					}					
				else if (EPass != CheckTiming(iPreviousFrameTime, Now, iNextCheckPoint->iDelayFromPreviuos, KAllowedTimePercentage) )
					{
					TInt64 Diff=Now.Int64() - iPreviousFrameTime.Int64();
					INFO_PRINTF4(_L("Timing checking failed (Got=%d Expected=%d Diff=%d)"), I64INT(Diff), 
										iNextCheckPoint->iDelayFromPreviuos, I64INT(MAKE_TINT64(0,iNextCheckPoint->iDelayFromPreviuos )- Diff));
					iLatestCheckpointVerdict = EFail;
					return EFalse;
					}
				}
// perfrom status checking //
			if (iNextCheckPoint->iPluginStatusMask != TFrameCheckPoint::KNoStatusChecking)
				{
				const TUint EffectiveStatus=(iLastPluginStatus & iNextCheckPoint->iPluginStatusMask);
				if (EffectiveStatus != iNextCheckPoint->iPluginStatusValue)
					{
					INFO_PRINTF3(_L("Plug-in Status checking failed (Got=%b Expected=%b"), EffectiveStatus, iNextCheckPoint->iPluginStatusValue);
					iLatestCheckpointVerdict = EFail;
					return EFalse;
					}
				}
			iLatestCheckpointVerdict = EPass;
			++iNextCheckPoint;
			}
		}

	iPreviousFrameTime.HomeTime();
	return ContinueTest;
	}


TInt CTestStepWithCheckpoint::FuncTestL()
	{
	if (!(iTestParams->iFuncParams && iTestParams->iFuncParams->IsValid()))
		{
		iLastError=KErrNone;
		return iLastError;	
		}
	if (iTestParams->iFuncParams->iExpectedNumFrames != TFunctionalTstParams::KNoValueCheck)
		{
		TInt numFrames;
		User::LeaveIfError( iImageDisplay->NumFrames(numFrames) );
		if ( numFrames != iTestParams->iFuncParams->iExpectedNumFrames)
			{
			INFO_PRINTF3(_L("---Unexpected number of frames %d, expected %d"),numFrames, iTestParams->iFuncParams->iExpectedNumFrames);
			return (iLastError=KErrGeneral);
			}
		}
	if (iTestParams->iFuncParams->iExpectedImgStatus != TFunctionalTstParams::KNoValueCheck)
		{
		TUint imgStatus=iImageDisplay->ImageStatus();
		if (imgStatus != iTestParams->iFuncParams->iExpectedImgStatus)
			{
			INFO_PRINTF3(_L("---Unexpected  ImageStatus() value 0x%04x, expected 0x%04x"),imgStatus, iTestParams->iFuncParams->iExpectedImgStatus);
			return (iLastError=KErrGeneral);
			}
		}
	const TInt* uidValue=iTestParams->iFuncParams->iSupportedExtsUidValues;
	while (*uidValue)
		{
		MImageDisplayPluginExtension* iFacePtr=NULL;
		iLastError=iImageDisplay->ExtensionInterface(TUid::Uid(*uidValue), reinterpret_cast<TAny*&>(iFacePtr));
		if (iLastError != KErrNone || iFacePtr==NULL)
			{
			INFO_PRINTF3(_L("---Failed to obtain Ext Uid value 0x%x, error=%d"),*uidValue, iLastError);
			return iLastError?iLastError:(iLastError=KErrGeneral);
			}
		TUid extUid=iFacePtr->Uid();
		iFacePtr->Release();
		if (extUid.iUid != *uidValue)
			{
			INFO_PRINTF3(_L("---Junkie extension return different Uid value 0x%x, but asked for 0x%x"),extUid.iUid, *uidValue);
			return (iLastError=KErrGeneral);
			}
		++uidValue;
		}
// try to get non-existed I-face
	TAny* iFacePtr=NULL;
	if (KErrNone==iImageDisplay->ExtensionInterface(KNullUid, iFacePtr))
		{
		INFO_PRINTF1(_L("---ERROR: plug-in is broken"));
		return (iLastError=KErrGeneral);
		}
// use the generic extension to set some params
	MGenIclImageDisplayExtension* genExt;
	iLastError=iImageDisplay->ExtensionInterface(KUidGenIclImageDisplayPluginExtUid, reinterpret_cast<TAny*&>(genExt));
	if (iLastError != KErrNone || genExt==NULL)
		{
		INFO_PRINTF3(_L("---Failed to obtain Ext Uid value 0x%x, error=%d"),KUidGenIclImageDisplayPluginExtUid.iUid, iLastError);
		return iLastError?iLastError:(iLastError=KErrGeneral);
		}
	if (iTestParams->iFuncParams->iMaxReductionFactor != TFunctionalTstParams::KReductionFactorNotSet)
		{
		iLastError = genExt->SetMaximumReductionFactor(iTestParams->iFuncParams->iMaxReductionFactor);
		if (iLastError!=KErrNone)
			{
			INFO_PRINTF3(_L("---Failed to set reduction factor to value %d, error=%d"),iTestParams->iFuncParams->iMaxReductionFactor, iLastError);
			}
		}
	User::LeaveIfError(iLastError);

	if (iTestParams->iFuncParams->iScalingQuality != TFunctionalTstParams::KScalingQualityNotSet)
		{
		iLastError = genExt->SetScaleQuality(iTestParams->iFuncParams->iScalingQuality);
		if (iLastError!=KErrNone)
			{
			INFO_PRINTF3(_L("---Failed to set scaling quality to value %d, error=%d"),iTestParams->iFuncParams->iScalingQuality, iLastError);
			}
		}
	User::LeaveIfError(iLastError);

	if (iTestParams->iFuncParams->iMaxUnscaledSzWidth > -1)
		{
		iLastError = genExt->SetMaximumNonReducedSize(TSize(iTestParams->iFuncParams->iMaxUnscaledSzWidth, iTestParams->iFuncParams->iMaxUnscaledSzHeight));
		if (iLastError!=KErrNone)
			{
			INFO_PRINTF4(_L("---Failed to Set Maximum Non Reduced Size to value (%d,%d) error=%d"),
								iTestParams->iFuncParams->iMaxUnscaledSzWidth, iTestParams->iFuncParams->iMaxUnscaledSzHeight, iLastError);
			}
		}
	User::LeaveIfError(iLastError);

	genExt->Release();
	return iLastError;
	}

TVerdict CTestStepWithCheckpoint::TestL()
	{
	iLatestCheckpointVerdict = (AllocMode()?EFail:EPass);
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	ResetCheckPoints();
	SetDefaultDestSize();
	iImageDisplay->Play();
	CActiveScheduler::Start();
	iImageDisplay->StopPlay();
	if (iLastError==KErrNoMemory && AllocMode())
		{
		iLatestCheckpointVerdict = EPass;
		User::Leave(iLastError);
		}
	if (iNextCheckPoint && iNextCheckPoint->IsValid() && ((iTestParams->iMaxFramesToDecode>0 &&
		 iNextCheckPoint->iAbsoluteFrameNumber <= iTestParams->iMaxFramesToDecode) || iTestParams->iMaxFramesToDecode==TImageDisplayUnitTestParams::KNoFrameLimit)
		)
		{
		INFO_PRINTF1(_L("-- ERROR: Not all the checpoints have been passed"));
		iLatestCheckpointVerdict = EFail;
		}
	return iLatestCheckpointVerdict;
	}
	
