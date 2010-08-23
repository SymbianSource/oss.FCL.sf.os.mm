
// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TSI_MMFVCLNT.CPP
// This program is designed the test of the MMF_VCLNT.
// 
//

/**
 @file TSI_MMFVCLNT.cpp
*/

#include <fbs.h>
#include "TSI_ICLCODstep.h"
#include "imageconversion.h"


RIclCodStep::RIclCodStep(const TIclCodPicType aType, const TInt aNum)
	{
	// ensure heap is big enough to decode a pic
	iHeapSize  = 0x1000000;

	// derive a test step name
	TInt stepNum = aNum + (aType * 1000);
	TBuf<6> stepStr;
	stepStr.Num(stepNum);

	iTestStepName = _L("MM-ICL-COD-I-");
	iTestStepName.Append(stepStr);
	iTestStepName.Append(_L("-CP"));

	// strip the first digit off the step name, this will give us the
	// actual picture name (and incidentally save us having to recount the zeroes :)
	stepStr[0] = '0';

	// * NB this is ugly, and should probably be removed to a ConstructL
	// derive a filename
	switch(aType)
		{
		case EPicTypeBMP:
			iFileName.Format(_L("c:\\ICL\\Validation\\bmp\\%08d.bmp"),aNum);
			break;
		case EPicTypePNG:
			iFileName.Format(_L("c:\\ICL\\Validation\\png\\%08d.png"),aNum);
			break;
		case EPicTypeJPG:
			// DEF087103: We now need to read the .jpgi files, whose bits will be inverted during the reading process
			iFileName.Format(_L("c:\\ICL\\Validation\\jpg\\%08d.jpgi"),aNum);
			break;
		}
	} 

TVerdict RIclCodStep::OpenL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFs.Connect());
	iScheduler = new (ELeave) CActiveScheduler;	
	CActiveScheduler::Install( iScheduler );
	iListener = new (ELeave) CActiveListener;
	return EPass;
	}

TVerdict RIclCodStep::DoTestStepL()
	{
	INFO_PRINTF2(_L("File : %S "), &iFileName);

	TUint att;
	TInt err;
	if ((err = iFs.Att(iFileName, att)) != KErrNone)
		{
		INFO_PRINTF4(_L("File : %S error: %d, attribute: %u"), &iFileName, err, att);
		return EFail;
		}
		
	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0),EColor16M));

	CImageDecoder* decoder = NULL;
	TRAPD(error,decoder=CImageDecoder::FileNewL(iFs,iFileName));
	if(error==KErrNone)
		{
		CleanupStack::PushL(decoder);
		if (!(decoder->IsImageHeaderProcessingComplete()) )
			INFO_PRINTF1(_L("IsImageHeaderProcessingComplete() failed"));
		
		else
			{
				for(TInt i=0; i<decoder->FrameCount(); i++)
				{

				const TFrameInfo& frameInfo = decoder->FrameInfo(i);
				User::LeaveIfError(bitmap.Resize(TSize(0,0)));
				error = bitmap.Resize(frameInfo.iOverallSizeInPixels);
				if(error != KErrNone)
					{	
					INFO_PRINTF4(_L("Resize %d x %d : gave error %d"),
								frameInfo.iOverallSizeInPixels.iWidth,
								frameInfo.iOverallSizeInPixels.iHeight,
								error);
					}
				else
					{
					iListener->InitialiseActiveListener();
					decoder->Convert(&iListener->iStatus,bitmap,i);
					CActiveScheduler::Start();
					error = iListener->iStatus.Int();
					if(error != KErrNone)
						INFO_PRINTF2(_L("Convert : gave error %d"), error);
					} // end for
				} // end else
			}
		CleanupStack::PopAndDestroy(decoder);
		}
	else
		INFO_PRINTF2(_L("FileNewL : gave error %d"), error);

	// NB : we expect errors to happen as these are corrupt images we're testing
	// for the most part.
	// A test should always pass therefore, as long as it does not panic

	return iTestStepResult = EPass;
	}

void RIclCodStep::Close()
	{
	RFbsSession::Disconnect();
	iFs.Close();
	delete iScheduler;
	delete iListener;
	}

	
