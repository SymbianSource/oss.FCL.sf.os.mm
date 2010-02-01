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

#include "TestSuiteExifImageDisplay.h"
#include "TestStepExifImageDisplay.h"
#include <icl/geniclimagedisplayext.h>

#include "TeststepGenIclimagedisplay.h"

CTestStepIclWrapperGeneric::CTestStepIclWrapperGeneric(const TImageDisplayUnitTestParams& aTestParams):
															CTestStepWithCheckpoint(aTestParams)
	{
	}

TVerdict CTestStepIclWrapperGeneric::TestL()
	{
	iLatestCheckpointVerdict = (AllocMode()?EFail:EPass);
	ASSERT(iImageDisplay == NULL);
	// create an ImageDisplay object
	ResetCheckPoints();
	iImageDisplay = CImageDisplay::NewL(*this, iFs);
	SetupPluginL();
	iImageDisplay->SetupL();
	if (KErrNone != (iLastError=FuncTestL()))
		{
		INFO_PRINTF2(_L("---Func test failed with error %d"),iLastError);
		return (iLatestCheckpointVerdict=EFail);
		}
	SetDefaultDestSize();
	MGenIclImageDisplayExtension* ext;
	iLastError=iImageDisplay->ExtensionInterface(KUidGenIclImageDisplayPluginExtUid, reinterpret_cast<TAny*&>(ext));
	if (iLastError != KErrNone)
		{
		return iLatestCheckpointVerdict;
		}
	iLastError=ext->SetMaximumReductionFactor(0);
	ext->Release();
	if (iLastError != KErrNone)
		{
		return iLatestCheckpointVerdict;
		}
	iImageDisplay->Play();
	CActiveScheduler::Start();
	iImageDisplay->StopPlay();
	if (iLastError==KErrNoMemory && AllocMode())
		{
		iLatestCheckpointVerdict = EPass;
		User::Leave(iLastError);
		}
	if (iLastError!=KErrNone)
		{
		INFO_PRINTF2(_L("---Test completed with error %d"),iLastError);
		return EFail;
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


