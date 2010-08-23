// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "icl/imageprocessor.h"
#include "ImageProcessorTest.h"

const TInt KImageWidth = 5;
const TInt KImageHeight = 1;

_LIT(KNameEGray2,		"EGray2");
_LIT(KNameEGray4,		"EGray4");
_LIT(KNameEGray16,		"EGray16");
_LIT(KNameEGray256,		"EGray256");
_LIT(KNameEColor16,		"EColor16");
_LIT(KNameEColor256,	"EColor256");
_LIT(KNameEColor4K,		"EColor4K");
_LIT(KNameEColor64K,	"EColor64K");
_LIT(KNameEColor16M,	"EColor16M");
_LIT(KNameEColor16MU,	"EColor16MU");

_LIT8(KResultEGray2,	"\xF4");
_LIT8(KResultEGray4,	"\x20\xFF");
_LIT8(KResultEGray16,	"\x30\x19\xFF");
_LIT8(KResultEGray256,	"\x00\x3F\x9F\x1F\xFF");
_LIT8(KResultEColor16,	"\x50\x97\xFF");
_LIT8(KResultEColor256,	"\x00\x05\x1E\xDC\xFF");
_LIT8(KResultEColor4K,	"\x00\x00\x00\x0F\xF0\x00\x0F\x00\xFF\x0F");
_LIT8(KResultEColor64K,	"\x00\x00\x00\xF8\xE0\x07\x1F\x00\xFF\xFF");
_LIT8(KResultEColor16M,	"\x00\x00\x00\x00\x00\xFF\x00\xFF\x00\xFF\x00\x00\xFF\xFF\xFF");

RCImageProcessorUtilityTest::TTestCase::TTestCase(TDisplayMode aDisplayMode, const TDesC8& aResultBuffer, const TDesC& aDisplayName):
	iDisplayMode(aDisplayMode),
	iResultBuffer(aResultBuffer),
	iDisplayName(aDisplayName)
	{
	}

RCImageProcessorUtilityTest::RCImageProcessorUtilityTest(const TDesC& aTestName)
	{
	iTestStepName = aTestName;
	}

TVerdict RCImageProcessorUtilityTest::OpenL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	iTestData.Reset();
	User::LeaveIfError(iTestData.Append(TTestCase(EGray2,KResultEGray2,KNameEGray2)));
	User::LeaveIfError(iTestData.Append(TTestCase(EGray4,KResultEGray4,KNameEGray4)));
	User::LeaveIfError(iTestData.Append(TTestCase(EGray16,KResultEGray16,KNameEGray16)));
	User::LeaveIfError(iTestData.Append(TTestCase(EGray256,KResultEGray256,KNameEGray256)));
	User::LeaveIfError(iTestData.Append(TTestCase(EColor16,KResultEColor16,KNameEColor16)));
	User::LeaveIfError(iTestData.Append(TTestCase(EColor256,KResultEColor256,KNameEColor256)));
	User::LeaveIfError(iTestData.Append(TTestCase(EColor4K,KResultEColor4K,KNameEColor4K)));
	User::LeaveIfError(iTestData.Append(TTestCase(EColor64K,KResultEColor64K,KNameEColor64K)));
	User::LeaveIfError(iTestData.Append(TTestCase(EColor16M,KResultEColor16M,KNameEColor16M)));

	return iTestStepResult;
	}
/*
 * Use CImageProcessor to write a sequence of standard pixels
 * to a bitmap for every display mode. Verify that the result
 * is as expected.
 */
TVerdict RCImageProcessorUtilityTest::DoTestStepL()
	{
	const TSize KImageSize = TSize(KImageWidth,KImageHeight);

	// Use a Black, Red, Green and White pixel stream
	TRgb KRgbBuffer[] = {KRgbBlack,KRgbRed,KRgbGreen,KRgbBlue,KRgbWhite};
	const TInt KRgbBufferSize = sizeof(KRgbBuffer)/sizeof(TRgb);
	
	// Add 16MU test data. Need to do this here since the KRgbBuffer definition
	// must stay in this function since SetPixels() expects it to be non const.
	const TPtrC8 KResultEColor16MU(reinterpret_cast<const TUint8*>(KRgbBuffer), sizeof(KRgbBuffer)); // construct from input TRgb buffer
	User::LeaveIfError(iTestData.Append(TTestCase(EColor16MU,KResultEColor16MU,KNameEColor16MU)));
	
	// Test for all displaymodes
	const TInt noOfDisplayModes = iTestData.Count();
	for(TInt testCase = 0;  testCase < noOfDisplayModes; testCase++)
		{
		TTestCase testData = iTestData[testCase];

		INFO_PRINTF2(_L("%S"),&testData.iDisplayName);

		CFbsBitmap bitmap;
		User::LeaveIfError(bitmap.Create(KImageSize,testData.iDisplayMode));
		ASSERT(bitmap.DisplayMode() == testData.iDisplayMode);

		// Use ImageProcessorUtility to create an appropriate pixel writer
		CImageProcessor* imageProc = ImageProcessorUtility::NewImageProcessorL(bitmap,KImageSize,bitmap.DisplayMode(),ETrue);
		CleanupStack::PushL(imageProc);

		// Write the pixels
		imageProc->PrepareL(bitmap,KImageSize);
		if(!imageProc->SetPixels(KRgbBuffer,KRgbBufferSize))
			{
			if(!imageProc->FlushPixels())
				{
				INFO_PRINTF1(_L("Pixel writing failed"));
				User::Leave(KErrGeneral);
				}
			}

		// Get the scanline
		const TInt scanLineLength = bitmap.ScanLineLength(KImageSize.iWidth,bitmap.DisplayMode());
		HBufC8* scanLine = HBufC8::NewL(scanLineLength);
		CleanupStack::PushL(scanLine);

		TPtr8 scanLinePtr(scanLine->Des());
		bitmap.GetScanLine(scanLinePtr,TPoint(0,0),KImageSize.iWidth,bitmap.DisplayMode());

		// Verify the result is as expected.
		Compare(*scanLine,testData.iResultBuffer,_L("Result does not match reference"));
		CleanupStack::PopAndDestroy(2,imageProc); //scanLine, imageProc
		}

	return iTestStepResult;
	}

void RCImageProcessorUtilityTest::Close()
	{
	iTestData.Reset();
	RFbsSession::Disconnect();
	}
