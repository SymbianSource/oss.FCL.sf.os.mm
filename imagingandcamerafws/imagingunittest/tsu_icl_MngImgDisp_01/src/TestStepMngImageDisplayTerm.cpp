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
#include "TestSuiteMngImageDisplay.h"
#include "TestStepMngImageDisplay.h"

CTestStepMngCheckpoint::CTestStepMngCheckpoint(const TImageDisplayUnitTestParams& aTestParams):
																		CTestStepImgDisplayGeneric(aTestParams),
																		iNextCheckPoint(aTestParams.iCheckPoints)
	{
	}

TBool CTestStepMngCheckpoint::ConsumeFrameL(const CFbsBitmap* aBitmap)
	{
	++iFrameNumber;
	TBool ContinueTest=EFalse;
	if (iNextCheckPoint && iNextCheckPoint->IsValid())
		{
		if (iNextCheckPoint->iAbsoluteFrameNumber == iFrameNumber)
			{
			iLatestCheckpointVerdict = EFail;
			TTime Now;
			Now.HomeTime();
			INFO_PRINTF3(_L("Checkpoint Frame# %d Plugin Status=0x%04x "), iFrameNumber, iLastPluginStatus);
// perform binary image checking //
			if (NULL != iNextCheckPoint->iReferenceFileName)
				{
				TBuf<32> fileName ;
				fileName.AppendFormat(_L("frame_%d.mbm"),iFrameNumber);
				TRAP(iLastError, SetDestFilenameL(fileName) );
				if (iLastError != KErrNone)
					{
					return EFalse;
					}
				SetRefFilename(TPtrC(iNextCheckPoint->iReferenceFileName));
				TRAP(iLastError, iLastError=CompareBitmapWithFileL(*aBitmap,  RefFilename() ));
				if (iLastError != KErrNone )
					{
					INFO_PRINTF4(_L("Images Do not match (Ref=%s  Got=%S Err=%d)"), iNextCheckPoint->iReferenceFileName, &fileName, iLastError);
					const_cast<CFbsBitmap*>(aBitmap)->Save(DestFilename());
					return EFalse;
					}
				}
// perform binary mask checking //		
			if (NULL != iNextCheckPoint->iReferenceMaskName)
				{
				const CFbsBitmap* frame;
				const CFbsBitmap* mask;
				iImageDisplay->GetBitmap(frame, mask);
				if (NULL == mask)
					{
					INFO_PRINTF1(_L("  --Error: Mask bitmap is NULL"));
					User::Leave(KErrGeneral);
					}
				TBuf<32> fileName ;
				fileName.AppendFormat(_L("mask_%d.mbm"),iFrameNumber);
				SetDestFilenameL(fileName);

				SetRefFilename(TPtrC(iNextCheckPoint->iReferenceMaskName));
				TInt error;
				TRAP(error, error=CompareBitmapWithFileL(*mask,  RefFilename() ));
				if (error != KErrNone )
					{
					INFO_PRINTF4(_L("  --Images Do not match (Ref=%s  Got=%S Err=%d)"), iNextCheckPoint->iReferenceMaskName, &fileName, error);
					const_cast<CFbsBitmap*>(mask)->Save(DestFilename());
					User::Leave(error);
					}
				}						
// perfrom timing checking //
			if (iNextCheckPoint->iDelayFromPreviuos != TFrameCheckPoint::KNodelayChecking)
				{
				if (EPass != CheckTiming(iPreviousFrameTime, Now, iNextCheckPoint->iDelayFromPreviuos, KAllowedTimePercentage) )
					{
					TInt64 Diff=Now.Int64() - iPreviousFrameTime.Int64();
					INFO_PRINTF4(_L("Timing checking failed (Got=%d Expected=%d Diff=%d)"), I64INT(Diff), 
										iNextCheckPoint->iDelayFromPreviuos, I64INT(MAKE_TINT64(0,iNextCheckPoint->iDelayFromPreviuos )- Diff));
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
					return EFalse;
					}
				}
			iLatestCheckpointVerdict = EPass;
			++iNextCheckPoint;
			}
		ContinueTest = ETrue;
		if (iTestParams->iMaxFramesToDecode != TImageDisplayUnitTestParams::KNoFrameLimit)
			{
			ContinueTest = iFrameNumber < iTestParams->iMaxFramesToDecode;
			}
		}
	else
		{
		if (iTestParams->iMaxFramesToDecode != TImageDisplayUnitTestParams::KNoFrameLimit)
			{
			ContinueTest = iFrameNumber < iTestParams->iMaxFramesToDecode;
			}
		}
	iPreviousFrameTime.HomeTime();
	return ContinueTest;
	}

TVerdict CTestStepMngCheckpoint::TestL()
	{
	iLatestCheckpointVerdict = EPass;
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	SetDefaultDestSize();
	iImageDisplay->Play();
	CActiveScheduler::Start();
	return (iLastError==KErrNone ? iLatestCheckpointVerdict : EFail );
	}
	
