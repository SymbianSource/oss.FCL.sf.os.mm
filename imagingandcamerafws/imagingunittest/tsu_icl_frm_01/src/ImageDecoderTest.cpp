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

#include <ecom/ecom.h>
#include "ICLFrameWorkTestStep.h"
#include "ICLFrameworkTestSuite.h"
#include "ImageDecoderTest.h"
#include "icl/ICL_UIDS.hrh"
#include "icl/ICL_PropertyUIDS.hrh"
#include "imageframeformats.hrh"
#include "TestUids.hrh"
#include "icl/imageconversionextension.h"
#include "icl/imageconversionextensionintf.h"
#include "fwextconstants.h"

_LIT(KTiffFile,"TIF2.TIF");
_LIT(KTiffFileRef,"TIF2.MBM");
_LIT(KGifFile,"TRANS_1.GIF");
_LIT(KGifFileRef,"TRANS_1.MBM");
_LIT(KPngFile,"PNG24BPP_1.PNG");
_LIT(KTstFile,"TEST.TST");
_LIT(KOTBTestfile,"c:\\TSU_ICL_FRM_01\\operator.otb");
_LIT(KOTBTestReffile,"c:\\TSU_ICL_FRM_01\\operator.mbm");
_LIT8(KGifMimeType,"image/gif");
_LIT8(KTifMimeType,"image/tiff");
_LIT8(KTstMimeType,"image/test");

// Framework extension
_LIT(KTestTSTFile, "c:\\tsu_icl_frm_01\\test.tst");
_LIT(KTestPNGFile, "c:\\tsu_icl_frm_01\\png4_8.png");

const TUid KTSTImageTypeUid = {0x101F7BF7};
const TUid KTSTImageSubTypeUid = {0x101F7BF8};
const TUid KTestDecoderUid = {0x101F7BF6};
const TUid KFailTestDecoderUid = {0x101F7C08};
const TUid KExtTestDecoderUid = {0x101F7C04};
const TUid KInvalidUid = {0x05FFFFFF};
const TUid KDummyClassUid = {0x10273802}; // KTestDummyDecoderClassUidValue
const TUid KTestDummyDecoderImplementationSWUid = {0x10273801}; //KTestDummyDecoderImplementationSWUidValue
const TUid KUidJPGDecoderImplementation2 = {KJPGDecoderImplementation2UidValue};
const TUid KTestDummyZeroPropCountDecoderImplementationUid = {0x10273805}; //KTestDummyZeroPropCountDecoderImplementationUidValue
const TUid KBadVerTestEncoderImplementationUid = {0x101F7C11}; //KBadVerTestEncoderImplementationUidValue
const TUid KTestDummyOnlyPropDecoderImplementationUid= {0x1027380B}; //KTestDummyOnlyPropDecoderImplementationUidValue
const TUid KFwExtTestDecoder = {KTestFwExtDecoderImplementationUidValue};
const TUid KPNGDecoder = {KPNGDecoderImplementationUidValue};

const TInt KBufferSize = 0x1000;

//RCImageDecoderDecodeTest
RCImageDecoderDecodeTest* RCImageDecoderDecodeTest::NewL(CTestSuite* aSuite, TContentMode aContentMode, TBool aMaskDecode, TBool aAllocTest, TBool aCancelTest)
	{
	return new (ELeave) RCImageDecoderDecodeTest(aSuite, aContentMode, aMaskDecode, aAllocTest, aCancelTest);
	}

RCImageDecoderDecodeTest::RCImageDecoderDecodeTest(CTestSuite* aSuite, TContentMode aContentMode, TBool aMaskDecode, TBool aAllocTest, TBool aCancelTest):
	RAllocTest(aAllocTest),
	iContentMode(aContentMode),
	iMaskDecode(aMaskDecode),
	iCancelTest(aCancelTest),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	// Increase the heap size for this test
	iHeapSize = 0x20000;
	iMimeTest = EFalse;
	switch (iContentMode)
		{
	case EContentFile :
		//Use FileNewL()
		if(!iMaskDecode && !iAllocTest)
			{
			iMimeTest = ETrue;

			if (aCancelTest)
				iTestStepName = _L("MM-ICL-FRM-U-5340-HP");
			else
				iTestStepName = _L("MM-ICL-FRM-U-5300-HP");
			}
		if(!iMaskDecode && iAllocTest)		
			{
			iMimeTest = ETrue;
			iTestStepName = _L("MM-ICL-FRM-U-5301-HP");
			}
		if(iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5302-HP");
		if(iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5303-HP");
		break;
	case EContentDescriptor: 
		//Use DataNewL()
		if(!iMaskDecode && !iAllocTest)
			{
			iMimeTest = ETrue;
			iTestStepName = _L("MM-ICL-FRM-U-5304-HP");
			}
		if(!iMaskDecode && iAllocTest)		
			{
			iMimeTest = ETrue;
			iTestStepName = _L("MM-ICL-FRM-U-5305-HP");
			}
		if(iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5306-HP");
		if(iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5307-HP");
		break;
	case  EContentFileHandle:
		if(!iMaskDecode && !iAllocTest)
			{
			iMimeTest = ETrue;

			if (aCancelTest)
				iTestStepName = _L("MM-ICL-FRM-U-5390-HP");
			else
				iTestStepName = _L("MM-ICL-FRM-U-5350-HP");
			}
		if(!iMaskDecode && iAllocTest)		
			{
			iMimeTest = ETrue;
			iTestStepName = _L("MM-ICL-FRM-U-5351-HP");
			}
		if(iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5352-HP");
		if(iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5353-HP");
	
		break;
		}
	}

void RCImageDecoderDecodeTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CTestActiveScheduler;	
	CActiveScheduler::Install( iScheduler );	

	switch(iContentMode)
		{
	case EContentFileHandle:
	case EContentFile:
		{
		
		//Use SetFileL()
		if(!iMaskDecode && !iAllocTest)
			{
			if (iCancelTest)
				INFO_PRINTF1(_L("<<<   CImageDecoder FileNewL() Decode, cancel & continue Test   >>>"));
			else
				INFO_PRINTF1(_L("<<<   CImageDecoder FileNewL() Decode Test   >>>"));
			}
		if(!iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoder FileNewL() Decode Alloc Test   >>>"));
		if(iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoder FileNewL() Mask Decode Test   >>>"));
		if(iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoder FileNewL() Mask Decode Alloc Test   >>>"));

		User::LeaveIfError(iSourceFile.Temp(iFileSession, _L("C:\\"), iSourceFilename, EFileWrite|EFileShareAny));
		break;
		}
	case EContentDescriptor:
		{
		//Use SetDataL()
		if(!iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoder DataNewL() Decode Test   >>>"));
		if(!iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoder DataNewL() Decode Alloc Test   >>>"));
		if(iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoder DataNewL() Mask Decode Test   >>>"));
		if(iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoder DataNewL() Mask Decode Alloc Test   >>>"));
		break;
		}
			
		}

	if(iMimeTest)
		INFO_PRINTF1(_L("Using MIME type"));

	RFile file;
 	TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
 	if(iMaskDecode)
 		{
   		fileName.Append(KGifFile);
 		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
  		}
	else
		{
   		fileName.Append(KTiffFile);
		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
		}
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);
	iSourceLength = iSourceData->Length();

	if(iMaskDecode)
		{
		TFileName refFileName(KGifFileRef);
		GetTestFileNameL(fileName, refFileName);
		}
	else
		{
		TFileName refFileName(KTiffFileRef);
		GetTestFileNameL(fileName, refFileName);
		}

	iRefBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iRefBitmap->Load(fileName));

	if(iMimeTest)
		iMimeType.Copy(KTifMimeType);

	iTestBitmap = NULL;
	iTestBitmapMask = NULL;
	iImageDecoder = NULL;
	iImplementationUid.iUid = 0;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageDecoderDecodeTest::TestStepL()
	{
	iSourceDataPtr.SetLength(0);

	CImageDecoder::TOptions options = CImageDecoder::EOptionAllowZeroFrameOpen;

	if(iImageDecoder == NULL)
		{
		TInt status = KErrUnderflow;
		while(status == KErrUnderflow)
			{
			IncrementBufferSize();
			switch (iContentMode)
				{
			case EContentFile: 
				{
				if(iMimeTest)
					{
					TRAP(status,iImageDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, iMimeType, options));
					}
				else
					{
					TRAP(status,iImageDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, options));
					}
				break;
				}
			case EContentDescriptor:
				{
				if(iMimeTest)
					{
					TRAP(status, iImageDecoder = CImageDecoder::DataNewL(iFileSession, iSourceDataPtr, iMimeType, options));
					}
				else
					{
					TRAP(status, iImageDecoder = CImageDecoder::DataNewL(iFileSession, iSourceDataPtr, options));
					}
				break;
				}
			case EContentFileHandle:
				{
				RFile file;
				User::LeaveIfError(file.Open(iFileSession, iSourceFilename, EFileRead|EFileShareAny));
				if(iMimeTest)
					{
					TRAP(status, iImageDecoder = CImageDecoder::FileNewL(file, iMimeType, EView, options));
					}
				else
					{
					TRAP(status, iImageDecoder = CImageDecoder::FileNewL(file, EView, options));
					}
				file.Close();
				break;
				}
					
				}

			if((status != KErrNone) && (status != KErrUnderflow))
				User::Leave(status);
			}

		}

	TUid decoderUid = iImageDecoder->ImplementationUid();
	TUid expectedUid;
	if(iMaskDecode)
		expectedUid.iUid = KGIFDecoderImplementationUidValue;
	else
		expectedUid.iUid = KTIFFDecoderImplementationUidValue;

	Compare(decoderUid, expectedUid, _L("Decoder UID incorrect")); 

	while(!iImageDecoder->IsImageHeaderProcessingComplete())
		{
		iImageDecoder->ContinueProcessingHeaderL();
		IncrementBufferSize();
		}

	const TFrameInfo frameInfo = iImageDecoder->FrameInfo(0);

	if(iMaskDecode)
		{
		if(!(frameInfo.iFlags & TFrameInfo::ETransparencyPossible))
			{
			SetFail(_L("Image does not support masks"));
			iMaskDecode = EFalse;
			}
		}
	
	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iTestBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	if(iMaskDecode)
		{
		iTestBitmapMask = new (ELeave) CFbsBitmap;
		User::LeaveIfError(iTestBitmapMask->Create(frameInfo.iOverallSizeInPixels, EGray2));
		}

	if(iCancelTest)
		iScheduler->SetDecoder(
			iImageDecoder, 
			iListener, 
			iTestBitmap, 
			iMaskDecode?iTestBitmapMask:0, 
			KDefaultStepsBeforeCancel);
	iScheduler->SetStep(this);

	iListener->InitialiseActiveListener();
	if(iMaskDecode)
		iImageDecoder->Convert(&iListener->iStatus, *iTestBitmap, *iTestBitmapMask, 0);
	else
		iImageDecoder->Convert(&iListener->iStatus, *iTestBitmap, 0);
	CActiveScheduler::Start();

	TInt status;
	status = iListener->iStatus.Int();
	if((status != KErrNone) && (status != KErrUnderflow))
		User::Leave(status);

	while(status == KErrUnderflow)
		{
		IncrementBufferSize();
		iListener->InitialiseActiveListener();
		iImageDecoder->ContinueConvert(&iListener->iStatus);
		CActiveScheduler::Start();
		status = iListener->iStatus.Int();
		if((status != KErrNone) && (status != KErrUnderflow))
			User::Leave(status);
		}

//  if step was supposed to cancel and it didn't
// then return inconclusive (the number of steps to wait may ne too high)
	if(iCancelTest && !iCancelled)
		User::Leave(EInconclusive);
	}

void RCImageDecoderDecodeTest::CheckResultL()
	{
	Compare(*iTestBitmap, *iRefBitmap, _L("Bitmaps are not the same"));
	//The image and mask look the same
	if(iMaskDecode)
		Compare(*iTestBitmapMask, *iRefBitmap, _L("Mask bitmaps are not the same"));
	}

void RCImageDecoderDecodeTest::Cleanup()
	{
	delete iImageDecoder; iImageDecoder = NULL;
	if(iTestBitmap)
		{delete iTestBitmap; iTestBitmap = NULL;}
	if(iTestBitmapMask)
		{delete iTestBitmapMask; iTestBitmapMask = NULL;}
	if(iContentMode == EContentFile || iContentMode == EContentFileHandle)
		iSourceFile.SetSize(0);
	}

void RCImageDecoderDecodeTest::Close()
	{
	delete iRefBitmap; iRefBitmap = NULL;
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iSourceFile.Close();
	iFileSession.Delete(iSourceFilename);
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

void RCImageDecoderDecodeTest::IncrementBufferSize()
	{
	TInt position(iSourceDataPtr.Length());

	if (position < iSourceLength)
		{
		iSourceDataPtr.SetLength(position + 1);

		if(iContentMode == EContentFile || iContentMode == EContentFileHandle)
			{
			iSourceFile.Write(iSourceDataPtr.Mid(position, 1));
			}
		}
	}
//----------------------------------------------------------------------
//RCImageDecoderIntentTest
RCImageDecoderIntentTest* RCImageDecoderIntentTest::NewL(CTestSuite* aSuite, TBool aMimeTest)
	{
	return new (ELeave) RCImageDecoderIntentTest(aSuite, aMimeTest);
	}

RCImageDecoderIntentTest::RCImageDecoderIntentTest(CTestSuite* aSuite, TBool aMimeTest):
	RAllocTest(EFalse),
	iMimeTest(aMimeTest),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	if(iMimeTest)
		iTestStepName = _L("MM-ICL-FRM-U-5310-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5311-HP");
	}

void RCImageDecoderIntentTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CTestActiveScheduler;	
	CActiveScheduler::Install( iScheduler );	

	INFO_PRINTF1(_L("<<<   CImageDecoder FileNewL() Decode (with Intent) Test   >>>"));
	
	User::LeaveIfError(iSourceFile.Temp(iFileSession, _L("C:\\"), iSourceFilename, EFileWrite|EFileShareAny));
	
	if(iMimeTest)
		INFO_PRINTF1(_L("Using MIME type"));

	RFile file;
	TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(KTiffFile);
	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);
	iSourceLength = iSourceData->Length();

	TFileName refFileName(KTiffFileRef);
	GetTestFileNameL(fileName, refFileName);

	iRefBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iRefBitmap->Load(fileName));

	if(iMimeTest)
		iMimeType.Copy(KTifMimeType);

	iTestBitmap = NULL;
	iImageDecoder = NULL;
	iImplementationUid.iUid = 0;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageDecoderIntentTest::TestStepL()
	{
	iSourceDataPtr.SetLength(0);

	CImageDecoder::TOptions options = CImageDecoder::EOptionAllowZeroFrameOpen;

	if(iImageDecoder == NULL)
		{
		TInt status = KErrUnderflow;
		while(status == KErrUnderflow)
			{
			IncrementBufferSize();
			if(iMimeTest)
				{
				TRAP(status,iImageDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, iMimeType, EPeek, options));
				}
			else
				{
				TRAP(status,iImageDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, EPeek, options));
				}

			if((status != KErrNone) && (status != KErrUnderflow))
				User::Leave(status);
			}

		}

	TUid decoderUid = iImageDecoder->ImplementationUid();
	TUid expectedUid;

	if (iMimeTest)
		expectedUid.iUid = KTIFFDecoderImplementationUidValue;
	else
		expectedUid.iUid = KTIFFDecoderLittleEndianImplementationUidValue;

	Compare(decoderUid, expectedUid, _L("Decoder UID incorrect")); 

	while(!iImageDecoder->IsImageHeaderProcessingComplete())
		{
		iImageDecoder->ContinueProcessingHeaderL();
		IncrementBufferSize();
		}

	const TFrameInfo frameInfo = iImageDecoder->FrameInfo(0);

	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iTestBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	iListener->InitialiseActiveListener();
	
	iImageDecoder->Convert(&iListener->iStatus, *iTestBitmap);
	CActiveScheduler::Start();

	TInt status;
	status = iListener->iStatus.Int();
	if((status != KErrNone) && (status != KErrUnderflow))
		User::Leave(status);

	while(status == KErrUnderflow)
		{
		IncrementBufferSize();
		iListener->InitialiseActiveListener();
		iImageDecoder->ContinueConvert(&iListener->iStatus);
		CActiveScheduler::Start();
		status = iListener->iStatus.Int();
		if((status != KErrNone) && (status != KErrUnderflow))
			User::Leave(status);
		}
	}

void RCImageDecoderIntentTest::CheckResultL()
	{
	Compare(*iTestBitmap, *iRefBitmap, _L("Bitmaps are not the same"));
	//The image and mask look the same
	}

void RCImageDecoderIntentTest::Cleanup()
	{
	delete iImageDecoder; iImageDecoder = NULL;
	delete iTestBitmap; iTestBitmap = NULL;
	iSourceFile.SetSize(0);
	}

void RCImageDecoderIntentTest::Close()
	{
	delete iRefBitmap; iRefBitmap = NULL;
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iSourceFile.Close();
	iFileSession.Delete(iSourceFilename);
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

void RCImageDecoderIntentTest::IncrementBufferSize()
	{
	TInt position(iSourceDataPtr.Length());

	if (position < iSourceLength)
		{
		iSourceDataPtr.SetLength(position + 1);
		iSourceFile.Write(iSourceDataPtr.Mid(position, 1));
		}
	}
//-----------------------------------------------------------------------

//RCImageDecoderStaticTest
RCImageDecoderStaticTest* RCImageDecoderStaticTest::NewL(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderStaticTest(aSuite, aTestNumber, aAllocTest);
	}

RCImageDecoderStaticTest::RCImageDecoderStaticTest(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iTestNumber(aTestNumber),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	//GetImageTypesL()
	if((iTestNumber == 0) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5320-HP");
	if((iTestNumber == 0) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5321-HP");

	//GetImageSubTypesL()
	if((iTestNumber == 1) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5322-HP");
	if((iTestNumber == 1) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5323-HP");

	//GetFileTypesL()
	if((iTestNumber == 2) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5324-HP");
	if((iTestNumber == 2) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5325-HP");

	//GetMimeTypeFileL()
	if((iTestNumber == 3) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5326-HP");
	if((iTestNumber == 3) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5327-HP");

	//GetMimeTypeDataL()
	if((iTestNumber == 4) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5328-HP");
	if((iTestNumber == 4) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5329-HP");
	
	//GetPluginPropertiesL()
	if((iTestNumber == 5) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5342-HP");
	if((iTestNumber == 5) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5343-HP");
	
	//GetInterfaceImplementationsL()
	if((iTestNumber == 6) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5344-HP");
	if((iTestNumber == 6) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5345-HP");
	
	//GetInterfaceImplementationsL()
	if((iTestNumber == 7) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5346-HP");
	if((iTestNumber == 7) && iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5347-HP");
	
	if((iTestNumber == 8) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5348-HP");
	
	if((iTestNumber == 9) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5356-HP");
	
	if((iTestNumber == 10) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5358-HP");
	
	if((iTestNumber == 11) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5360-HP");
	
	if((iTestNumber == 12) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5362-HP");
	
	if((iTestNumber == 13) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5364-HP");

	if((iTestNumber == 14) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-7025-HP");
	
	if((iTestNumber == 15) && !iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-7026-HP");	
	}

void RCImageDecoderStaticTest::SetupL()
	{
	User::LeaveIfError(iFileSession.Connect());

	//GetImageTypesL()
	if((iTestNumber == 0) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetImageTypesL() Test   >>>"));
	if((iTestNumber == 0) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetImageTypesL() Alloc Test   >>>"));

	//GetImageSubTypesL()
	if((iTestNumber == 1) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetImageSubTypesL() Test   >>>"));
	if((iTestNumber == 1) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetImageSubTypesL() Alloc Test   >>>"));

	//GetFileTypesL()
	if((iTestNumber == 2) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetFileTypesL() Test   >>>"));
	if((iTestNumber == 2) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetFileTypesL() Alloc Test   >>>"));

	//GetMimeTypeFileL()
	if((iTestNumber == 3) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetMimeTypeFileL() Test   >>>"));
	if((iTestNumber == 3) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetMimeTypeFileL() Alloc Test   >>>"));

	//GetMimeTypeDataL()
	if((iTestNumber == 4) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetMimeTypeDataL() Test   >>>"));
	if((iTestNumber == 4) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetMimeTypeDataL() Alloc Test   >>>"));

	iSourceData = NULL;
	if(iTestNumber == 4)
		{
		RFile file;
		TInt size;
		TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
		fileName.Append(KGifFile);
		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
		CleanupClosePushL(file);
		User::LeaveIfError(file.Size(size));
		iSourceData = HBufC8::NewL(size);
		iSourceDataPtr.Set(iSourceData->Des());
		User::LeaveIfError(file.Read(iSourceDataPtr));
		CleanupStack::PopAndDestroy(&file);
		}
	
	//GetPluginPropertiesL()
	if((iTestNumber == 5) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetPluginPropertiesL() Test   >>>"));
	if((iTestNumber == 5) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetPluginPropertiesL() Alloc Test   >>>"));
	
	//GetInterfaceImplementationsL()
	if((iTestNumber == 6) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() with a RUidDataArray Test   >>>"));
	if((iTestNumber == 6) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() with a RUidDataArray Alloc Test   >>>"));
	
	//GetInterfaceImplementationsL() and GetPluginPropertiesL()
	if((iTestNumber == 7) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() with a TUid* Test   >>>"));
	if((iTestNumber == 7) && iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() with a TUid* Alloc Test   >>>"));
	
	if((iTestNumber == 8) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() Test   >>>"));
	
	if((iTestNumber == 9) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() Test   >>>"));
	
	if((iTestNumber == 10) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::GetInterfaceImplementationsL() Test   >>>"));
		
	if((iTestNumber == 11) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   Calling CImageDecoder::GetPluginPropertiesL() on a dummy plugin with no mime types and extns but with properties   >>>"));
	
	if((iTestNumber == 12) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   Test call to CImageDecoder::GetPluginPropertiesL() on a dummy plugin with a properties count equals to 0   >>>"));
	
	if((iTestNumber == 13) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   Test call to CImageDecoder::GetPluginPropertiesL() on a  dummy plugin with an invalid version number   >>>"));

	if((iTestNumber == 14) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   Negative test for CImageDecoder::GetPluginPropertiesL()   >>>"));

	if((iTestNumber == 15) && !iAllocTest)
		INFO_PRINTF1(_L("<<<   Negative test when calling CImageDecoder::GetInferfaceImplementationsL() with a invalid required UID   >>>"));
		
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	}

void RCImageDecoderStaticTest::TestStepL()
	{
	if(iTestNumber == 0)
		CImageDecoder::GetImageTypesL(iImageTypeArray);

	if(iTestNumber == 1)
		{
		TUid imageType(KImageTypeTIFFUid);
		CImageDecoder::GetImageSubTypesL(imageType, iImageSubTypeArray);
		}

	if(iTestNumber == 2)
		CImageDecoder::GetFileTypesL(iFileExtensionArray);

	if(iTestNumber == 3)
		{
		TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
		fileName.Append(KGifFile);
		CImageDecoder::GetMimeTypeFileL(iFileSession, fileName, iFileMimeType);
		}

	if(iTestNumber == 4)
		CImageDecoder::GetMimeTypeDataL(iSourceDataPtr, iDataMimeType);
	
	if(iTestNumber == 5)
		{		
		iPropertiesArray.Reset();
		CImageDecoder::GetPluginPropertiesL(KUidJPGDecoderImplementation2, iPropertiesArray);
		}
	
	if(iTestNumber == 6)
		{
		RUidDataArray properties;
		User::LeaveIfError(properties.Append(TUid::Uid(KFormatYUVMonochromeUidValue)));
		User::LeaveIfError(properties.Append(TUid::Uid(KFormatYUV420PlanarUidValue)));
		User::LeaveIfError(properties.Append(TUid::Uid(KFormatYUV422InterleavedUidValue)));
		User::LeaveIfError(properties.Append(TUid::Uid(KFormatYUV420PlanarReversedUidValue)));
		
		CleanupClosePushL(properties);
		
		iImplArray.Reset();
		CImageDecoder::GetInterfaceImplementationsL(properties, iImplArray);		
		CleanupStack::PopAndDestroy(1); //properties
		}
	
	if(iTestNumber == 7)
		{
		const TUid properties[] = {TUid::Uid(KFormatYUVMonochromeUidValue), TUid::Uid(KFormatYUV420PlanarUidValue), TUid::Uid(KFormatYUV422InterleavedUidValue), TUid::Uid(KFormatYUV420PlanarReversedUidValue)};
		
		iImplArray.Reset();
		CImageDecoder::GetInterfaceImplementationsL(properties, 4, iImplArray);
		}
	
	/* Use case where 2 codec plugins supporting the same features are available but one is
	 * implemented in hardware, and the other one in software. Client obtains implementation
	 * UID of hardware codec.
	 */
	if(iTestNumber == 8)
		{
		const TUid requiredUids[] = {TUid::Uid(KUidICLJpegImageFrameInterface), TUid::Uid(KFormatYUV422InterleavedUidValue), TUid::Uid(KExifSupportUidValue), TUid::Uid(KHwCodecUidValue)};
		
		iImplArray.Reset();
		CImageDecoder::GetInterfaceImplementationsL(requiredUids, 4, iImplArray);
		}
	
	/* 
	 * We want to get the default plugin from a group of codec plugins with the same class uid
	 */
	if(iTestNumber == 9)
		{
		const TUid requiredUids[] = {KDummyClassUid};
		
		iImplArray.Reset();
		CImageDecoder::GetInterfaceImplementationsL(requiredUids, 1, iImplArray);
		}
	
	/*
	 * Several codec plugins supporting the same image type uid are available, but each one
	 * supports different formats. We want to load a codec supporting a particular format.
	 */
	if(iTestNumber == 10)
		{
		const TUid requiredUids[] = {KImageTypeJPGUid};
		
		iImplArray.Reset();
		CImageDecoder::GetInterfaceImplementationsL(requiredUids, 1, iImplArray);
		}	
		
	if(iTestNumber == 11)
		{
		iPropertiesArray.Reset();
		CImageDecoder::GetPluginPropertiesL(KTestDummyOnlyPropDecoderImplementationUid, iPropertiesArray);
		}
		
	if(iTestNumber == 12)
		{
		iPropertiesArray.Reset();
		CImageDecoder::GetPluginPropertiesL(KTestDummyZeroPropCountDecoderImplementationUid, iPropertiesArray);				
		}

	if(iTestNumber == 13)
		{		
		iPropertiesArray.Reset();
		TRAPD(error, CImageDecoder::GetPluginPropertiesL(KBadVerTestEncoderImplementationUid, iPropertiesArray));
		iError = error;
		}
	
	if(iTestNumber == 14)
		{
		TRAPD(error, CImageDecoder::GetPluginPropertiesL(KInvalidUid, iPropertiesArray));
		iError = error;
		}
		
	if(iTestNumber == 15)
		{
		const TUid requiredUids[] = {KInvalidUid};
		iImplArray.Reset();
		TRAPD(error, CImageDecoder::GetInterfaceImplementationsL(requiredUids, 1, iImplArray));
		iError = error;
		}

	}

void RCImageDecoderStaticTest::CheckResultL()
	{
	if(iTestNumber == 0)
		{
		Compare(iImageTypeArray.Count() > 0, ETrue, _L("GetImageTypesL() returned 0 entries"));
		
		for(TInt i=0; i < iImageTypeArray.Count(); i++)
			{
			for(TInt j=0; j<i; j++)
				{
				TBuf<64> Desc(iImageTypeArray[j]->Description());
				if (iImageTypeArray[i]->Description().Compare(Desc) == 0)
					{
					TBuf<48> errMsg(_L("GetImageTypesL() has duplicate entrie for "));
					errMsg.Append(Desc);
					SetFail(errMsg);
					break;
					}
				}
			}
		}
		
	if(iTestNumber == 1)
		Compare(iImageSubTypeArray.Count() > 0, ETrue, _L("GetImageSubTypesL() returned 0 entries"));
	if(iTestNumber == 2)
		Compare(iFileExtensionArray.Count() > 0, ETrue, _L("GetFileTypesL() returned 0 entries"));
	if(iTestNumber == 3)
		Compare(iFileMimeType, KGifMimeType, _L("GetMimeTypeFileL() returned wrong MIME type"));
	if(iTestNumber == 4)
		Compare(iDataMimeType, KGifMimeType, _L("GetMimeTypeDataL() returned wrong MIME type"));
	if(iTestNumber == 5)
		{
		Compare(iPropertiesArray.Count() > 0, ETrue, _L("GetPluginPropertiesL() returned 0 entries"));
		}
	if(iTestNumber == 6)
		{
		Compare(iImplArray.Count() > 0, ETrue, _L("GetInterfaceImplementationsL() with RUidDataArray returned 0 entries"));
		}
	if(iTestNumber == 7)
		{
		Compare(iImplArray.Count() > 0, ETrue, _L("GetInterfaceImplementationsL() with TUid* returned 0 entries"));
		}
	if(iTestNumber == 8)
		{
		Compare(iImplArray.Count() > 0, ETrue, _L("GetInterfaceImplementationsL() returned 0 entries"));
		}
	if(iTestNumber == 9)
		{
		Compare((iImplArray.Count()>0 && iImplArray[0]==KTestDummyDecoderImplementationSWUid), ETrue, _L("GetInterfaceImplementationsL() didnt return the expected default codec"));
		}		
	if(iTestNumber == 10)
		{
		Compare(iImplArray.Count() > 0, ETrue, _L("GetInterfaceImplementationsL() returned 0 entries"));
		}
	if(iTestNumber == 11)
		{
		Compare(iPropertiesArray.Count() > 0, ETrue, _L("Call to GetPluginPropertiesL() on a dummy plugin with no extns, no mime types but prperties retuned 0 entries."));
		}
	if(iTestNumber == 12)
		{
		Compare(iPropertiesArray.Count() == 0, ETrue, _L("Test result: Call to GetPluginPropertiesL() didn't return 0 entries on a plugin with properties count field equals to 0."));
		}
	if(iTestNumber == 13)
		{
		Compare(iPropertiesArray.Count() == 0, ETrue, _L("Test result: Call to GetPluginPropertiesL() on a dummy plugin with invalid properties uids didn't leave with KErrNotSupported."));
		}
		
	if(iTestNumber == 14)
		{
		Compare(iError, KErrNotFound, _L("Negative test result: GetPluginPropertiesL() didn't leave with KErrNotFound with invalid plugin UID"));
		}

	if(iTestNumber == 15)
		{
		Compare(iError, KErrNotFound, _L("Negative test result: Call to GetInterfaceImplementationsL() with an invalid required UID"));
		}
	}

void RCImageDecoderStaticTest::Cleanup()
	{
	iImageTypeArray.ResetAndDestroy();
	iImageSubTypeArray.ResetAndDestroy();
	iFileExtensionArray.ResetAndDestroy();
	iPropertiesArray.Close();
	iImplArray.Close();
	}

void RCImageDecoderStaticTest::Close()
	{
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;	
	iFileSession.Close();
	RAllocTest::Close();
	}

//RCImageDecoderResolverTest
RCImageDecoderResolverTest* RCImageDecoderResolverTest::NewL(CTestSuite* aSuite, TContentMode aContentMode, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderResolverTest(aSuite, aContentMode, aAllocTest);
	}

RCImageDecoderResolverTest::RCImageDecoderResolverTest(CTestSuite* aSuite, TContentMode aContentMode, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iContentMode(aContentMode),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	switch (iContentMode)
		{
	case EContentFile: 
		{//Using FileNewL()
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5330-HP");
		else
			iTestStepName = _L("MM-ICL-FRM-U-5331-HP");
		break;
		}
	case EContentDescriptor:
		{//Using DataNewL()
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5332-HP");
		else
			iTestStepName = _L("MM-ICL-FRM-U-5333-HP");
		break;
		}
	case EContentFileHandle: 
		{//Using FileNewL() with file handle
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5380-HP");
		else
			iTestStepName = _L("MM-ICL-FRM-U-5381-HP");
		break;
		}			
			
		}

	}

void RCImageDecoderResolverTest::SetupL()
	{
	User::LeaveIfError(iFileSession.Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	switch(iContentMode)
		{//Using FileNewL()
	case EContentFile:
		{
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoder::FileNewL() Plugin Resolving Test   >>>"));
		else
			INFO_PRINTF1(_L("<<<   CImageDecoder::FileNewL() Plugin Resolving Alloc Test   >>>"));
		break;
		}
	case EContentDescriptor:
		{//Using DataNewL()
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoder::DataNewL() Plugin Resolving Test   >>>"));
		else
			INFO_PRINTF1(_L("<<<   CImageDecoder::DataNewL() Plugin Resolving Alloc Test   >>>"));
		break;
		}
	case EContentFileHandle:
		{
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoder::FileNewL() Plugin Resolving Test (file handle)  >>>"));
		else
			INFO_PRINTF1(_L("<<<   CImageDecoder::FileNewL() Plugin Resolving Alloc Test (file handle)  >>>"));
		break;
		}
		}
	GetTestFileNameL(iSourceFilename, KTstFile());
	INFO_PRINTF2(_L("%S"),&iSourceFilename);
	iMimeType = KTstMimeType;

	RFile file;
	User::LeaveIfError(file.Open(iFileSession, iSourceFilename, EFileRead|EFileShareAny));
	CleanupClosePushL(file);

	if(iContentMode == EContentDescriptor)
		{
		TInt size;
		User::LeaveIfError(file.Size(size));
		iSourceData = HBufC8::NewL(size);
		iSourceDataPtr.Set(iSourceData->Des());
		User::LeaveIfError(file.Read(iSourceDataPtr));
		}
	CleanupStack::PopAndDestroy(&file);

	CImplementationInformationType* decoderInfo = NULL;
	//See if the TestCodec is available
	TRAPD(error, decoderInfo = CImageDecoder::GetImplementationInformationL(KTestDecoderUid));
	delete decoderInfo; decoderInfo = NULL;
	Compare(error, KErrNone, _L("Test decoder not present"));

	//See if the FailTestCodec is available
	TRAP(error, decoderInfo = CImageDecoder::GetImplementationInformationL(KFailTestDecoderUid));
	delete decoderInfo; decoderInfo = NULL;
	Compare(error, KErrNone, _L("Fail test decoder not present"));

	//See if the ExtendedTestCodec is available
	TRAP(error, decoderInfo = CImageDecoder::GetImplementationInformationL(KExtTestDecoderUid));
	delete decoderInfo; decoderInfo = NULL;
	Compare(error, KErrNone, _L("Extended test decoder not present"));
	}

void RCImageDecoderResolverTest::TestStepL()
	{
	switch(iContentMode)
		{
	case EContentFile:
		{
		/*The decoders should be ordered according to MIME type position 
		 *in the resource file and then version number:
		 *1) FailTestDecoder - leave with KErrNotFound
		 *2) TestDecoder - should be selected
		 *3) ExtendedDecoder - should not be selected
		 */
		iMimeDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, iMimeType);

		/*The decoders should be ordered according to number of bytes matched from the match string 
		 *in the resource file and then version number:
		 *1) FailTestDecoder - leave with KErrNotFound
		 *2) TestDecoder - should be selected
		 *3) ExtendedDecoder - should not be selected
		 */
		iContentDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename);

		CImageDecoder* decoder = NULL;
		//Should leave with KErrNotFound
		TRAP(iError, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, CImageDecoder::EOptionAllowZeroFrameOpen, KTSTImageTypeUid, KTSTImageSubTypeUid, KFailTestDecoderUid));
		CleanupStack::PushL(decoder);
		if(iError!=KErrNotFound)
			User::Leave(iError);
		CleanupStack::PopAndDestroy(decoder);
		break;
		}
	
	case EContentDescriptor:
		{//Data test
		/*The decoders should be ordered according to MIME type position 
		 *in the resource file and then version number:
		 *1) FailTestDecoder - leave with KErrNotFound
		 *2) TestDecoder - should be selected
		 *3) ExtendedDecoder - should not be selected
		 */
		iMimeDecoder = CImageDecoder::DataNewL(iFileSession, iSourceDataPtr, iMimeType);

		/*The decoders should be ordered according to number of bytes matched from the match string 
		 *in the resource file and then version number:
		 *1) FailTestDecoder - leave with KErrNotFound
		 *2) TestDecoder - should be selected
		 *3) ExtendedDecoder - should not be selected
		 */
		iContentDecoder = CImageDecoder::DataNewL(iFileSession, iSourceDataPtr);

		CImageDecoder* decoder = NULL;
		//Should leave with KErrNotFound
		TRAP(iError, decoder = CImageDecoder::DataNewL(iFileSession, iSourceDataPtr, CImageDecoder::EOptionAllowZeroFrameOpen, KTSTImageTypeUid, KTSTImageSubTypeUid, KFailTestDecoderUid));
		CleanupStack::PushL(decoder);
		if(iError!=KErrNotFound)
			User::Leave(iError);
		CleanupStack::PopAndDestroy(decoder);
		break;
		}
	case EContentFileHandle:
		{
		RFile file;
		User::LeaveIfError(iFileSession.ShareProtected());
		User::LeaveIfError(file.Open(iFileSession, iSourceFilename, EFileRead|EFileShareAny));
		CleanupClosePushL(file);				
		/*The decoders should be ordered according to MIME type position 
		 *in the resource file and then version number:
		 *1) FailTestDecoder - leave with KErrNotFound
		 *2) TestDecoder - should be selected
		 *3) ExtendedDecoder - should not be selected
		 */
		iMimeDecoder = CImageDecoder::FileNewL(file, iMimeType, EView);

		/*The decoders should be ordered according to number of bytes matched from the match string 
		 *in the resource file and then version number:
		 *1) FailTestDecoder - leave with KErrNotFound
		 *2) TestDecoder - should be selected
		 *3) ExtendedDecoder - should not be selected
		 */
		 // temporary debug work around for base defect
		 TInt pos =0;
		 User::LeaveIfError(file.Seek(ESeekStart, pos));
		 // end temporary debug work around
		iContentDecoder = CImageDecoder::FileNewL(file, EView);

		CImageDecoder* decoder = NULL;
		//Should leave with KErrNotFound
		TRAP(iError, decoder = CImageDecoder::FileNewL(file, EView, CImageDecoder::EOptionAllowZeroFrameOpen, KTSTImageTypeUid, KTSTImageSubTypeUid, KFailTestDecoderUid));
		CleanupStack::PushL(decoder);
		if(iError!=KErrNotFound)
			User::Leave(iError);
		CleanupStack::PopAndDestroy(2, &file); //decoder, file
		break;
		}
		
		}
			
	}

void RCImageDecoderResolverTest::CheckResultL()
	{
	TUid decoderUid;

	decoderUid = iMimeDecoder->ImplementationUid();
	Compare(decoderUid, KTestDecoderUid, _L("MIME type resolving failed"));
	
	decoderUid = iContentDecoder->ImplementationUid();
	Compare(decoderUid, KTestDecoderUid, _L("Match string resolving failed"));

	Compare(iError, KErrNotFound, _L("Resolving should return KErrNotFound for codec"));
	}

void RCImageDecoderResolverTest::Cleanup()
	{
	delete iMimeDecoder; iMimeDecoder = NULL;
	delete iContentDecoder; iContentDecoder = NULL;
	}

void RCImageDecoderResolverTest::Close()
	{
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RAllocTest::Close();
	}

//RCImageDecoderMaskDecodeTest
RCImageDecoderMaskDecodeTest* RCImageDecoderMaskDecodeTest::NewL(CTestSuite* aSuite, TBool aAlphaTest)
	{
	return new (ELeave) RCImageDecoderMaskDecodeTest(aSuite, aAlphaTest);
	}

RCImageDecoderMaskDecodeTest::RCImageDecoderMaskDecodeTest(CTestSuite* aSuite, TBool aAlphaTest) :
	iAlphaTest(aAlphaTest)
	{
	iSuite = aSuite;
	if(!iAlphaTest)
		iTestStepName = _L("MM-ICL-FRM-U-5334-HP");
	else
		{
		iTestStepName = _L("MM-ICL-FRM-U-5336-HP");
		iHeapSize = 0x20000; //The PNG image need more memory
		}

	}

TVerdict RCImageDecoderMaskDecodeTest::OpenL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	if(!iAlphaTest)
		INFO_PRINTF1(_L("<<<   CImageDecoder::Convert() Mask Test   >>>"));
	else
		INFO_PRINTF1(_L("<<<   CImageDecoder::Convert() Alpha Mask Test   >>>"));


	if(!iAlphaTest)
		{
		TFileName refFileName(KGifFile);
		GetTestFileNameL(iSourceFilename, refFileName);
		}
	else
		{
		TFileName refFileName(KPngFile);
		GetTestFileNameL(iSourceFilename, refFileName);
		}

	TRAPD(error, iDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename));
	Compare(error, KErrNone, _L("Decoder not be created"));

	const TFrameInfo frameInfo = iDecoder->FrameInfo(0);

	TBool transparencyPossible = ((frameInfo.iFlags & TFrameInfo::ETransparencyPossible)==TFrameInfo::ETransparencyPossible);
	Compare(transparencyPossible, ETrue, _L("Image does not support masks"));

	iBitmapSize = frameInfo.iOverallSizeInPixels;
	iBitmapDisplayMode = frameInfo.iFrameDisplayMode;

	return iTestStepResult;
	}

TVerdict RCImageDecoderMaskDecodeTest::DoTestStepL()
	{
	CActiveListener* listener;
	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	CFbsBitmap bitmap;
	CFbsBitmap bitmapMask;

	TInt error = bitmap.Create(iBitmapSize, iBitmapDisplayMode);
	Compare(error, KErrNone, _L("Bitmap could not be created"));

	//Test with EGray2 mask
	error = bitmapMask.Create(iBitmapSize, EGray2);
	Compare(error, KErrNone, _L("Bitmap could not be created"));
	listener->InitialiseActiveListener();
	iDecoder->Convert(&listener->iStatus, bitmap, bitmapMask, 0);
	CActiveScheduler::Start();
	TInt result = listener->iStatus.Int();
	if(iAlphaTest)
		Compare(result, KErrArgument, _L("EGray2 mask with alpha channel should return KErrArgument"));
	else
		Compare(result, KErrNone, _L("EGray2 mask without alpha channel should return KErrNone"));

	//Test with EGray16 mask
	bitmapMask.Reset();
	error = bitmapMask.Create(iBitmapSize, EGray16);
	Compare(error, KErrNone, _L("Bitmap could not be created"));
	listener->InitialiseActiveListener();
	iDecoder->Convert(&listener->iStatus, bitmap, bitmapMask, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int();
	Compare(result, KErrArgument, _L("EGray16 mask should return KErrArgument"));

	//Test with EGray256 mask
	bitmapMask.Reset();
	error = bitmapMask.Create(iBitmapSize, EGray256);
	Compare(error, KErrNone, _L("Bitmap could not be created"));
	listener->InitialiseActiveListener();
	iDecoder->Convert(&listener->iStatus, bitmap, bitmapMask, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int();
	Compare(result, KErrNone, _L("EGray256 mask should return KErrNone"));

	//Test with too small mask
	error = bitmapMask.Resize(TSize(iBitmapSize.iWidth-1,iBitmapSize.iHeight-1));
	Compare(error, KErrNone, _L("Bitmap could not be resized"));
	listener->InitialiseActiveListener();
	iDecoder->Convert(&listener->iStatus, bitmap, bitmapMask, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int();
	Compare(result, KErrArgument, _L("Too small mask should return KErrArgument"));

	//Test with too large mask
	error = bitmapMask.Resize(TSize(iBitmapSize.iWidth+1,iBitmapSize.iHeight+1));
	Compare(error, KErrNone, _L("Bitmap could not be resized"));
	listener->InitialiseActiveListener();
	iDecoder->Convert(&listener->iStatus, bitmap, bitmapMask, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int();
	Compare(result, KErrArgument, _L("Too large mask should return KErrArgument"));

	//Test with EColor16 mask
	bitmapMask.Reset();
	error = bitmapMask.Create(iBitmapSize, EColor16);
	Compare(error, KErrNone, _L("Bitmap could not be created"));
	listener->InitialiseActiveListener();
	iDecoder->Convert(&listener->iStatus, bitmap, bitmapMask, 0);
	CActiveScheduler::Start();
	result = listener->iStatus.Int();
	Compare(result, KErrArgument, _L("EColor16 mask should return KErrArgument"));

	bitmapMask.Reset();
	bitmap.Reset();

	CleanupStack::PopAndDestroy(listener);

	return iTestStepResult;
	}

void RCImageDecoderMaskDecodeTest::Close()
	{
	delete iDecoder; iDecoder = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//RThreadedDecodeCancelTest
RThreadedDecodeCancelTest* RThreadedDecodeCancelTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RThreadedDecodeCancelTest(aSuite);
	}

RThreadedDecodeCancelTest::RThreadedDecodeCancelTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5338-HP");
	}

TVerdict RThreadedDecodeCancelTest::OpenL()
	{
	INFO_PRINTF1(_L("Threaded Decode Cancel Test"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	GetTestFileNameL(iSourceFilename, KTstFile());

	TRAPD(error, iDecoder = CImageDecoder::FileNewL(iFileSession, iSourceFilename, CImageDecoder::EOptionAlwaysThread, KTSTImageTypeUid, KTSTImageSubTypeUid, KTestDecoderUid));
	Compare(error, KErrNone, _L("Decoder not be created"));

	iBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iBitmap->Create(TSize(10,10), EGray2));

	return iTestStepResult;
	}

TVerdict RThreadedDecodeCancelTest::DoTestStepL()
	{

	TRequestStatus convertStatus = KRequestPending;

	//The TestDecoder is set up to return KErrDied on Cancel()
	iDecoder->Convert(&convertStatus, *iBitmap, 0);

	//Give the decoder time to call DoConvert()
	RTimer timer;
	timer.CreateLocal();
	TRequestStatus timerStatus = KRequestPending;;
	TTimeIntervalMicroSeconds32 interval = 1000000;
	timer.After(timerStatus, interval);
	User::WaitForRequest(timerStatus);
	timer.Close();

	//Cancel() the convert operation
	iDecoder->Cancel();

	User::WaitForRequest(convertStatus);
	TInt result = convertStatus.Int(); 
	Compare(result, KErrDied, _L("Cancel() on TestDecoder should return KErrDied"));
	
	return iTestStepResult;
	}

void RThreadedDecodeCancelTest::Close()
	{
	delete iDecoder; iDecoder = NULL;
	delete iBitmap; iBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler; iScheduler = NULL;
	}


void CTestActiveScheduler::DecodeCancelAndContinue(void)
{

	iDecoder->Cancel();	
	if (iTestBitmapMask == 0)
		iDecoder->Convert(&iListener->iStatus, *iTestBitmap, 0);
	else
		iDecoder->Convert(&iListener->iStatus, *iTestBitmap, *iTestBitmapMask, 0);

}

/* 
 *
 *	we will wait  iNumStep times until cancel decoding
 *
 * @return		"TAny"
 *					
 */
TAny CTestActiveScheduler::WaitForAnyRequest()
{
	User::WaitForAnyRequest();

	if(iDecoder)
		{
		
		iCurStep++;
		if(iCurStep > iNumStep && iListener->IsActive())
			{
			DecodeCancelAndContinue();
			
			if (iStep)
				iStep->SetCancelled();

			iDecoder = NULL;
			}
		}
}	

/**
*
* Ctor for RTImageOpenOTB
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RTImageDecDRMViaCAF*: constructed RTImageDecDRMViaCAF object
*
*/
RTImageOpenOTB* RTImageOpenOTB::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTImageOpenOTB(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RTImageOpenOTB::RTImageOpenOTB(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7015-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RTImageOpenOTB::DoTestStepL()
    {
    iFileName = KOTBTestfile;
    TMMFileSource fs(iFileName);
	TInt err;
	CImageDecoder* imageDecoder = NULL;
	User::LeaveIfError(FbsStartup());
    TInt error = RFbsSession::Connect();
    if (error != KErrNone)
        {
        INFO_PRINTF2(_L("RFbsSession::Connect() failed, err = %d"), error);
        return EInconclusive;
        }
    // Install the ActiveScheduler
    iActiveScheduler = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(iActiveScheduler);
    User::LeaveIfError(iFileSession.Connect());
    User::LeaveIfError(iFileSession.ShareProtected());
    INFO_PRINTF2(_L("Opening the file by passing FileSource : %S"), &iFileName);
    TRAP(err, imageDecoder = CImageDecoder::FileNewL(iFileSession, fs));
    // Open the image and create an image decoder
    if (err != KErrNone)
        {
        INFO_PRINTF3(_L("Cannot open file: %S. Unexpected Error: %d"),&iFileName , err);
        delete imageDecoder;
        imageDecoder = NULL;
        User::Leave(err);
        }
    if (imageDecoder == NULL)
        {
        INFO_PRINTF1(_L("ImageDecoder is NULL!"));
        User::Leave(err);
        }
    INFO_PRINTF2(_L("The file: %S had been opened."), &iFileName);
    
    // Create an active listener and push it on the cleanup stack
    CActiveListener* activeListener = new (ELeave) CActiveListener;
    CleanupStack::PushL(activeListener);
    const TFrameInfo* theFrameInfo = &imageDecoder->FrameInfo();

    // Create the destination bitmap
    CFbsBitmap* destBitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(destBitmap);

    // Create the destination bitmap with the specified size and display mode
    User::LeaveIfError(destBitmap->Create(
        theFrameInfo->iOverallSizeInPixels,
        theFrameInfo->iFrameDisplayMode));

    activeListener ->InitialiseActiveListener();

    // Decode the image
    imageDecoder->Convert(&activeListener->iStatus, *destBitmap, 0);
    CActiveScheduler::Start();

    err = activeListener -> iStatus.Int();
    if (err == KErrNone)
        {
        INFO_PRINTF2(_L("Conversion of file %S was successfull!"),&iFileName);
        }
    else
        {
        INFO_PRINTF2(_L("Failed during conversion of the file: %S"),&iFileName);
        delete imageDecoder;
        imageDecoder = NULL;
        // activeListener, destBitmap
        CleanupStack::PopAndDestroy(2, activeListener);
        User::Leave(err);
        }
    if (err == KErrNone)
        {
        TFileName refFileName(KOTBTestReffile);
        CFbsBitmap* refBitmap = new (ELeave) CFbsBitmap;
    	CleanupStack::PushL(refBitmap);
    	User::LeaveIfError(refBitmap->Load(refFileName));
        // Check the bitmaps
        INFO_PRINTF1(_L("Comparison of the decoded files"));
        Compare(*destBitmap, *refBitmap, _L("Bitmaps are not the same."));
        }      
    delete imageDecoder;
    imageDecoder = NULL;
    // activeListener, destBitmap
    CleanupStack::PopAndDestroy(3);
    User::Leave(err);
    if (err == KErrNone)
        {
        return EPass;
        }
    else
        {
        INFO_PRINTF2(_L("Error in opening/decoding the image : %d"), err);
        return EFail;
        }
    }
    



/**
*
* Ctor for RDecoderInvalidMimeTypeArg
*
* @param CTestSuite* aSuite: The name of this suite.
*
* @return RDecoderInvalidMimeTypeArg*: constructed RDecoderInvalidMimeTypeArg object
*
*/
RDecoderInvalidMimeTypeArg* RDecoderInvalidMimeTypeArg::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RDecoderInvalidMimeTypeArg(aSuite);
    }

/**
*
* Test step constructor.
* Each test step initialises its own name.
*
* @param CTestSuite* aSuite: The name of this suite.
*
*/
RDecoderInvalidMimeTypeArg::RDecoderInvalidMimeTypeArg(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-FRM-U-7027-HP");
    }

/**
*
* Do the test step.
* Each test step must supply an implementation for DoTestStepL.
*
* @return TVerdict: The result of the test step.
*
*/
TVerdict RDecoderInvalidMimeTypeArg::DoTestStepL()
    {
    INFO_PRINTF1(_L("Creating a new CImageDecoder with MIME Type, but TDesC8 length for MIME arg is null, should leave with KErrArgument\n"));

	CImageDecoder* decoder = NULL;
	TBuf8<1> invalidMimeType;
	TBuf8<1> dummyBuf;
	invalidMimeType.SetLength(0);	
	User::LeaveIfError(iFileSession.Connect());
	TRAPD(err, decoder = CImageDecoder::DataNewL(iFileSession, dummyBuf, invalidMimeType, CImageDecoder::EOptionNone));
	if (err == KErrArgument)
		return EPass;
	delete decoder;
	// Fail the test if we reach this point
	return EFail;
    }

/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the PNG Decoder.
	PNG does not support the framework extensions.

	@param	aSuite
				This suite.
	@param	aThreaded
				Is the decode to take place in a separate thread?
	@param	aMethod
				The method to be run (TFwExtMethod).
	@param	aTestStepName
				The name of this test step.
	@return RFwExtPNGDecNotSupportedTest*
				Constructed RFwExtPNGDecNotSupportedTest object
*/
RFwExtPNGDecNotSupportedTest* RFwExtPNGDecNotSupportedTest::NewL(CTestSuite* aSuite,
														TBool aThreaded,
    													TFwExtMethod aMethod,
    													const TDesC& aTestStepName)
	{
	return new (ELeave) RFwExtPNGDecNotSupportedTest(aSuite,
													aThreaded,
													aMethod,
													aTestStepName);
	}
	
RFwExtPNGDecNotSupportedTest::RFwExtPNGDecNotSupportedTest(CTestSuite* aSuite,
													TBool aThreaded,
    												TFwExtMethod aMethod,
    												const TDesC& aTestStepName)
	{
	iSuite = aSuite;
	iThreaded = aThreaded;
	iMethod = aMethod;
	iTestStepName = aTestStepName;
	}
	
TVerdict RFwExtPNGDecNotSupportedTest::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

  User::LeaveIfError(FbsStartup());
  User::LeaveIfError(iFileSession.Connect());
  TInt error = RFbsSession::Connect();
  if (error != KErrNone)
      {
      return EInconclusive;
      }
	if (iThreaded)
		{
		iOptions = CImageDecoder::EOptionAlwaysThread;
		}
		
	INFO_PRINTF2(_L("Testing PNG plugin: Implementation Uid = 0x%X"), KPNGDecoder);

	// Check that the test file is there.
	
	if (!CheckForFile(iFileSession, KTestPNGFile))
		{
		iTestStepResult = EFail;
		return iTestStepResult;
		}
	
	TRAPD(err, iImageDecoder = CImageDecoder::FileNewL(iFileSession, KTestPNGFile, iOptions, KNullUid, KNullUid, KPNGDecoder));
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CImageDecoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}	
	// Check that the plugin correctly supports threading or not		
	TInt threadErr = iImageDecoder->SetDecoderThreadPriority(EPriorityNormal);
	if(threadErr == KErrNotSupported && !iThreaded)
		{
		INFO_PRINTF1(_L("CImageDecoder not threaded"));
		}
	else if (threadErr == KErrNone && iThreaded)
		{
		INFO_PRINTF1(_L("CImageDecoder threaded"));
		}
	else
		{
		INFO_PRINTF1(_L("CImageDecoder threading issue"));
		iTestStepResult = EFail;
		}
	
	return iTestStepResult;
	}
	
TVerdict RFwExtPNGDecNotSupportedTest::DoTestStepL()
	{
	TInt err = KErrNone;
	TBool extensionPresent = EFalse;
	switch(iMethod)
		{
		case EOperationL:
			{
			INFO_PRINTF1(_L("OperationL()"));
			TRAP(err, iImageDecoder->OperationL());
			break;
			}
		case EScalerL:
			{
			INFO_PRINTF1(_L("GetScalerExtensionL()"));
			TRAP(err, iImageDecoder->ScalerL());
			break;
			}
		case EBlockStreamerL:
			{
			INFO_PRINTF1(_L("BlockStreamerL()"));
			TRAP(err, iImageDecoder->BlockStreamerL());
			break;
			}
		case EGetDestinationSize:
			{
			INFO_PRINTF1(_L("GetDestinationSize()"));
			TSize destinationSize;
			err = iImageDecoder->GetDestinationSize(destinationSize /*, 0*/);
			break;
			}
		case ESetClippingRectL:
			{
			INFO_PRINTF1(_L("SetClippingRectL()"));
			TRAP(err, iImageDecoder->SetClippingRectL(NULL));
			break;
			}
		default:
			{
			SetFail(_L("Called unsupported function"));
			}
		}
		
	if(err != KErrNotSupported && extensionPresent == EFalse)
		{
		SetFail(_L("Unsupported function returned unexpected result"));
		}
		
	return iTestStepResult;
	}
	
void RFwExtPNGDecNotSupportedTest::Close()
	{
	delete iImageDecoder;
	iImageDecoder = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	}
	
/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvOperation interface to allow callbacks
	from the actual plugin extension.
*/
CFwExtTestDecOperation* CFwExtTestDecOperation::NewL()
	{
	return new (ELeave) CFwExtTestDecOperation();
	}


CFwExtTestDecOperation::CFwExtTestDecOperation()
:iReferenceCount(1), iOperationCaps(0x0F) // All capabilities as per CDecodeOperationExtension
	{
	}

CFwExtTestDecOperation::~CFwExtTestDecOperation()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CFwExtTestDecOperation::Uid() const
	{
	return KICLOperationUid;
	}
	
void CFwExtTestDecOperation::IncrementRef()
	{
	iReferenceCount++;
	}
	
void CFwExtTestDecOperation::Release()
	{
	iReferenceCount--;
	}
	
TUint CFwExtTestDecOperation::Capabilities() const
	{
	return iOperationCaps;
	}

void CFwExtTestDecOperation::SetCapabilities(TUint aCaps)
	{
	iOperationCaps = aCaps;
	}
	
void CFwExtTestDecOperation::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	/*
	This bitwise OR is purely for testing purposes only.  An actual
	AddOperationL call would append the operation to a stack of operations.
	*/
	iOperations |= aOperation;
	}
	
TUint CFwExtTestDecOperation::Operations()
	{
	return iOperations;
	}

void CFwExtTestDecOperation::ClearOperationStack()
	{
	iOperations = 0;
	}

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvOperation interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CScalerExtension
*/
CFwExtTestScaler* CFwExtTestScaler::NewL()
	{
	return new (ELeave) CFwExtTestScaler();
	}

CFwExtTestScaler::CFwExtTestScaler()
:iReferenceCount(1)
	{
	}


CFwExtTestScaler::~CFwExtTestScaler()
	{
	ASSERT(iReferenceCount == 0);
	delete iScalerCaps;
	}

TUid CFwExtTestScaler::Uid() const
	{
	return KICLScalerUid;
	}
	
void CFwExtTestScaler::IncrementRef()
	{
	iReferenceCount++;
	}
	
void CFwExtTestScaler::Release()
	{
	iReferenceCount--;
	}
	
void CFwExtTestScaler::GetCapabilities(TScalerCaps& aCaps) const
	{
	aCaps = *iScalerCaps;
	}

void CFwExtTestScaler::SetCapabilitiesL(TInt aMaxUpscaleLimit,
										TInt aMaxDownscaleLimit,
										TBool aMustPreserveAspectRatio)
	{
	iScalerCaps = new (ELeave) TScalerCaps(aMaxUpscaleLimit, aMaxDownscaleLimit, aMustPreserveAspectRatio);
	}

void CFwExtTestScaler::SetScaledSize(TSize& aOriginalSize, TSize& aDesiredSize, TBool aLockAspectRatio)
	{
	iOriginalSize = aOriginalSize;
	iDesiredSize = aDesiredSize;
	iLockAspectRatio = aLockAspectRatio;
	}

void CFwExtTestScaler::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	iDesiredSizeSetScalingL = aDesiredSize;
	iQuality = aQuality;
	iLockAspectRatioSetScalingL = aLockAspectRatio;
	}

TInt CFwExtTestScaler::CheckSetScalingL(TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	if(aDesiredSize == iDesiredSizeSetScalingL
		&& aQuality == iQuality
		&& aLockAspectRatio == iLockAspectRatioSetScalingL)
		{
		return KErrNone;
		}
	return KErrGeneral;
	}

void CFwExtTestScaler::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	iScalingCoeff = aScalingCoeff;
	iScalingQuality = aScalingQuality;
	}

TInt CFwExtTestScaler::CheckSetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	if(iScalingCoeff == aScalingCoeff && iScalingQuality == aScalingQuality)
		{
		return KErrNone;
		}
	return KErrGeneral;
	}

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvStreamedDecode interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CStreamedDecodeExtension
*/
CFwExtTestStreamedDecode* CFwExtTestStreamedDecode::NewL()
	{
	return new (ELeave) CFwExtTestStreamedDecode();
	}


CFwExtTestStreamedDecode::CFwExtTestStreamedDecode()
:iReferenceCount(1)
	{
	}
	
CFwExtTestStreamedDecode::~CFwExtTestStreamedDecode()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CFwExtTestStreamedDecode::Uid() const
	{
	return KICLStreamedDecodeUid;
	}

void CFwExtTestStreamedDecode::IncrementRef()
	{
	iReferenceCount++;
	}
	
void CFwExtTestStreamedDecode::Release()
	{
	iReferenceCount--;
	}

void CFwExtTestStreamedDecode::GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const
	{
	aFormats = iFormats;
	aOptimalFormat = iOptimalFormat;
	}
	
void CFwExtTestStreamedDecode::SetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat)
	{
	iFormats = aFormats;
	iOptimalFormat = aOptimalFormat;
	}

void CFwExtTestStreamedDecode::GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const
	{
	if(iFormat == aFormat && iFrameNumber == aFrameNumber)
		{
		aCaps = iDecodeStreamCaps;	
		}
	}

void CFwExtTestStreamedDecode::SetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps)
	{
	iFormat = aFormat;
	iFrameNumber = aFrameNumber;
	iDecodeStreamCaps = aCaps;
	}
	
TInt CFwExtTestStreamedDecode::GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const
	{
	if(aFormat == iFormatGetBufferSizeL 
		&& iBlockSizeInPixels == aBlockSizeInPixels
		&& iNumBlocks == aNumBlocks)
		{
		aBlockSizeInPixels.iHeight = aBlockSizeInPixels.iHeight * 2;
		aBlockSizeInPixels.iWidth = aBlockSizeInPixels.iWidth * 2;
		return KErrNone;
		}
	return KErrGeneral;
	}

void CFwExtTestStreamedDecode::SetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks)
	{
	iFormatGetBufferSizeL = aFormat;
	iBlockSizeInPixels = aBlockSizeInPixels;
	iNumBlocks = aNumBlocks;
	}
	
void CFwExtTestStreamedDecode::InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	iFormatInitFrameL = aFormat;
	iFrameNumberInitFrameL = aFrameNumber;
	iNavigation = aNavigation;
	}

TInt CFwExtTestStreamedDecode::CheckInitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation)
	{
	if(iFormatInitFrameL == aFormat
		&& iFrameNumberInitFrameL == aFrameNumber
		&& iNavigation == aNavigation)
		{
		return KErrNone;
		}
	return KErrGeneral;
	}

	
void CFwExtTestStreamedDecode::GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead)
	{
	if(iSeqPosition == aSeqPosition
		&& aNumBlocksToGet == iNumBlocksToGet
		&& *aNumBlocksRead == iNumBlocksRead
		&& aStatus == NULL)
		{
		if((iFrame->FrameLayout()).Type() == (aFrame->FrameLayout()).Type())
			{
			const TFrameLayout& frameLayout = static_cast<const TFrameLayout&>(aFrame->FrameLayout());
			if(&frameLayout)
				{
				if(frameLayout.Planes() == KMaxPlanesInFrame)
					{
					*aNumBlocksRead++;
					}
				}		
			}
		}
	}

void CFwExtTestStreamedDecode::SetBlocks(TRequestStatus aStatus, CImageFrame& aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt aNumBlocksRead)
	{
	iStatus = aStatus;
	iFrame = &aFrame;
	iSeqPosition = aSeqPosition;
	iNumBlocksToGet = aNumBlocksToGet;
	iNumBlocksRead = aNumBlocksRead;
	}
	
void CFwExtTestStreamedDecode::GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks)
	{
	if(aNumBlocksToGet == iNumBlocksToGetGetNextBlocks
		&& *aNumBlocksRead == iNumBlocksReadGetNextBlocks
		&& aStatus == NULL)
		{
		if((iFrameGetNextBlocks->FrameLayout()).Type() == (aFrame->FrameLayout()).Type())
			{
			const TFrameLayout& frameLayout = static_cast<const TFrameLayout&>(aFrame->FrameLayout());
			if(&frameLayout)
				{
				if(frameLayout.Planes() == KMaxPlanesInFrame)
					{
					*aNumBlocksRead++;
					*aHaveMoreBlocks = EFalse;
					}
				}
			}
		}
	}

void CFwExtTestStreamedDecode::SetNextBlocks(TRequestStatus aStatus, CImageFrame& aFrame, TInt aNumBlocksToGet, TInt aNumBlocksRead, TBool aHaveMoreBlocks)
	{
	iStatusGetNextBlocks = aStatus;
	iFrameGetNextBlocks = &aFrame;
	iNumBlocksToGetGetNextBlocks = aNumBlocksToGet;
	iNumBlocksReadGetNextBlocks = aNumBlocksRead;
	iHaveMoreBlocks = aHaveMoreBlocks;
	}

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvPrepare interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CPrepareExtension
*/
CFwExtTestPrepare* CFwExtTestPrepare::NewL()
	{
	return new (ELeave) CFwExtTestPrepare();
	}


CFwExtTestPrepare::CFwExtTestPrepare()
:iReferenceCount(1)
	{
	}
	
CFwExtTestPrepare::~CFwExtTestPrepare()
	{
	ASSERT(iReferenceCount == 0);
	}

TUid CFwExtTestPrepare::Uid() const
	{
	return KICLPrepareUid;
	}

void CFwExtTestPrepare::IncrementRef()
	{
	iReferenceCount++;
	}
	
void CFwExtTestPrepare::Release()
	{
	iReferenceCount--;
	}

void CFwExtTestPrepare::Prepare(TRequestStatus* aStatus)
	{
	__ASSERT_ALWAYS((aStatus == NULL) && iSetPrepareCalled,User::Invariant());
	}

void CFwExtTestPrepare::SetPrepare(TBool aSetPrepareCalled)
	{
	iSetPrepareCalled = aSetPrepareCalled;
	}


/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the FwExtTest Decoder.
	FwExtTest codec supports the framework extensions but
	no actual Convert() call.  These tests exercise the
	Framework and its related classes.

	@param	aSuite
				This suite.
	@param	aThreaded
				Is the decode to take place in a separate thread?			
	@param	aMethod
				The method to be run (TFwExtMethod).
	@param	aTestStepName
				The name of this test step.
	@return RFwExtDecSupportedTest*
				Constructed RFwExtDecSupportedTest object
*/
RFwExtDecSupportedTest* RFwExtDecSupportedTest::NewL(CTestSuite* aSuite,
    											TBool aThreaded,
    											TFwExtMethod aMethod,
    											const TDesC& aTestStepName)
	{
	return new (ELeave) RFwExtDecSupportedTest(aSuite,
											aThreaded,
											aMethod,
											aTestStepName);
	}
	
RFwExtDecSupportedTest::RFwExtDecSupportedTest(CTestSuite* aSuite,
										TBool aThreaded,
										TFwExtMethod aMethod,
										const TDesC& aTestStepName)
	{
	iSuite = aSuite;
	iThreaded = aThreaded;
	iMethod = aMethod;
	iTestStepName = aTestStepName;
	}

TVerdict RFwExtDecSupportedTest::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());
	
	if(iThreaded)
		{
		iOptions = CImageDecoder::EOptionAlwaysThread;
		}
		
	INFO_PRINTF2(_L("Testing FwExtTest decoder plugin: Implementation Uid = 0x%X"), KFwExtTestDecoder);
	TRAPD(err, iImageDecoder = static_cast<CFwExtImageDecoder*>(CImageDecoder::FileNewL(iFileSession, KTestTSTFile, iOptions, KNullUid, KNullUid, KFwExtTestDecoder)));
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("CFwExtImageDecoder::FileNewL() left with %d"), err);
		iTestStepResult = EFail;
		return iTestStepResult;
		}
	
	// Check that the plugin correctly supports threading or not		
	TInt threadErr = iImageDecoder->SetDecoderThreadPriority(EPriorityNormal);
	if(threadErr == KErrNotSupported && !iThreaded)
		{
		INFO_PRINTF1(_L("CFwExtImageDecoder not threaded"));
		}
	else if (threadErr == KErrNone && iThreaded)
		{
		INFO_PRINTF1(_L("CFwExtImageDecoder threaded"));
		}
	else
		{
		INFO_PRINTF1(_L("CFwExtImageDecoder threading issue"));
		iTestStepResult = EFail;
		}
	
	return iTestStepResult;
	}

/**
Overview:
- Each case is a individual test named according to what the test is testing.
- Typically, an extension or extensions is/are created and functions called with good/bad args
to exercise those calls.
*/
TVerdict RFwExtDecSupportedTest::DoTestStepL()
	{
	TInt err = KErrNone;
	
	switch(iMethod)
		{
		case EOperationL:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestDecOperation* sharedTestExt = CFwExtTestDecOperation::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageDecoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvOperation* operation = NULL;
			operation = iImageDecoder->OperationL();
			INFO_PRINTF1(_L("OperationL() succeeded."));
			
			TUint caps = 0x0F;
			sharedTestExt->SetCapabilities(caps);
			
			// Compare capabilities
			if(operation->Capabilities() == caps)
				{
				INFO_PRINTF1(_L("Capabilities() returned correct result."));
				}
			else
				{
				INFO_PRINTF1(_L("Capabilities() returned unexpected result."));
				return EFail;
				}

			operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
			operation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
				
			TUint checkOperations = TImageConvOperation::EMirrorVerticalAxis | TImageConvOperation::ERotation270DegreesClockwise;
			
			// Compare set operations
			if(sharedTestExt->Operations() == checkOperations)
				{
				INFO_PRINTF3(_L("AddOperationL() has correctly set values. 0x%x == 0x%x"), sharedTestExt->Operations(), checkOperations);
				}
			else
				{
				INFO_PRINTF3(_L("AddOperationL() has incorrectly set values. 0x%x != 0x%x"), sharedTestExt->Operations(), checkOperations);
				return EFail;
				}
			
			// reset
			operation->ClearOperationStack();
			checkOperations = 0;
			
			// Compare reset values
			if(sharedTestExt->Operations() == checkOperations)
				{
				INFO_PRINTF1(_L("ClearOperationStack() was successful."));
				}
			else
				{
				INFO_PRINTF1(_L("ClearOperationStack() was not successful."));
				return EFail;
				}	
			
			break;
			}
		case EScalerL:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestScaler* sharedTestExt = CFwExtTestScaler::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageDecoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvScaler* scaler = iImageDecoder->ScalerL();
			INFO_PRINTF1(_L("ScalerL() succeeded."));
			
			TInt maxUpscaleLimit = 2;
			TInt maxDownscaleLimit = 1;
			TBool preserveAspectRatioIsNeeded = ETrue;
			sharedTestExt->SetCapabilitiesL(maxUpscaleLimit, maxDownscaleLimit, preserveAspectRatioIsNeeded);

			TScalerCaps scalerCaps;
			scaler->GetCapabilities(scalerCaps);
	
			// Check TScalerCaps value was set correctly
			if(scalerCaps.MaxUpscaleLimit() == maxUpscaleLimit
				&& scalerCaps.MaxDownscaleLimit() == maxDownscaleLimit
				&& scalerCaps.MustPreserveAspectRatio() == preserveAspectRatioIsNeeded)
				{
				INFO_PRINTF1(_L("GetCapabilities() functions correctly."));
				}
			else
				{
				INFO_PRINTF1(_L("GetCapabilities() functions incorrectly."));
				return EFail;
				}
			
			// Reset values
			TSize desiredSize(999,999);
			TImageConvScaler::TScalerQuality quality = TImageConvScaler::EMediumQuality;
			TBool lockAspectRatio = EFalse;
			scaler->SetScalingL(desiredSize, quality, lockAspectRatio);

			// Declare/reset check values
			err = sharedTestExt->CheckSetScalingL(desiredSize, quality, lockAspectRatio);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("SetScalingL(TSize&, TImageConvScaler::TScalerQuality, TBool) returned with %d."), err);
				return EFail;
				}

			TInt scalingCoeff = 2;
			scaler->SetScalingL(scalingCoeff, quality);
			err = sharedTestExt->CheckSetScalingL(scalingCoeff, quality);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("SetScalingL(TInt, TImageConvScaler::TScalerQuality) returned with %d."), err);
				return EFail;
				}
			
			break;
			}
		case EBlockStreamerLSetup:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestStreamedDecode* sharedTestExt = CFwExtTestStreamedDecode::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageDecoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvStreamedDecode* streamDecode = iImageDecoder->BlockStreamerL();
			INFO_PRINTF1(_L("BlockStreamerL() succeeded."));
			
				RArray<TUid> formatsCheck;
			formatsCheck.AppendL(TUid::Uid(0x11111111));
			formatsCheck.AppendL(TUid::Uid(0x22222222));
			formatsCheck.AppendL(TUid::Uid(0x99991234));
			TUid optimalFormatCheck = TUid::Uid(0x99991234);
			sharedTestExt->SetSupportedFormatsL(formatsCheck, optimalFormatCheck);

			RArray<TUid> formats;
			TUid optimalFormat;
			streamDecode->GetSupportedFormatsL(formats, optimalFormat);
			
			
			if(optimalFormat != optimalFormatCheck)
				{
				INFO_PRINTF1(_L("GetSupportedFormatsL() failed (aOptimalFormat incorrect)."));
				}
			
			if(formats.Count() == formatsCheck.Count())
				{
				for(TInt i = 0; i < formats.Count(); i++)
					{
					// Check all elements match
					if(formats[i] != formatsCheck[i])
						{
						return EFail;
						}
					}
				INFO_PRINTF1(_L("GetSupportedFormatsL() succeeded."));	
				}
			else
				{
				INFO_PRINTF1(_L("GetSupportedFormatsL() failed (aFormats returned unexpected length)."));
				return EFail;
				}
			TInt frameNumber = 0;
			TInt maxBlocksPerRequest = 3;
			TSize minBlockSizeInPixels(4,5);
			TInt optimalBlocksPerRequest = 256;
			TSize optimalBlockSizeInPixels(8,9);
			TInt streamSizeInBlocks = 7;
			TDecodeStreamCaps::TNavigation navigation = TDecodeStreamCaps::ENavigationSequentialForward;
			TDecodeStreamCaps streamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
										optimalBlocksPerRequest, streamSizeInBlocks,
										navigation);
			sharedTestExt->SetCapabilities(optimalFormat, frameNumber, streamCaps);
			
			TDecodeStreamCaps caps;
			streamDecode->GetCapabilities(optimalFormat, frameNumber, caps);
			
			// Compare values
			if(caps.MaxBlocksPerRequest() == maxBlocksPerRequest
				&& caps.MinBlockSizeInPixels() == minBlockSizeInPixels
				&& caps.OptimalBlocksPerRequest() == optimalBlocksPerRequest
				&& caps.StreamSizeInBlocks() == streamSizeInBlocks
				&& caps.Navigation() == navigation)
				{
				INFO_PRINTF1(_L("GetCapabilities() succeeded in setting values."));
				}
			else
				{
				INFO_PRINTF1(_L("GetCapabilities() failed."));
				return EFail;
				}

			sharedTestExt->SetBufferSize(optimalFormat, optimalBlockSizeInPixels, streamSizeInBlocks);
			TSize optimalBlockSizeInPixelsCheck = optimalBlockSizeInPixels;
			err = streamDecode->GetBufferSize(optimalFormat, optimalBlockSizeInPixelsCheck, streamSizeInBlocks);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("GetBufferSize() return incorrect value of %d"), err);
				return EFail;
				}
			if(optimalBlockSizeInPixelsCheck.iHeight == optimalBlockSizeInPixels.iHeight*2
				|| optimalBlockSizeInPixelsCheck.iWidth == optimalBlockSizeInPixels.iWidth*2)
				{
				INFO_PRINTF1(_L("GetBufferSize() correctly set the aBlockSizeInPixels"));
				}
			else
				{
				INFO_PRINTF1(_L("GetBufferSize() incorrectly set the aBlockSizeInPixels"));
				return EFail;
				}

			streamDecode->InitFrameL(optimalFormat, frameNumber, navigation);
			err = sharedTestExt->CheckInitFrameL(optimalFormat, frameNumber, navigation);
			if(err != KErrNone)
				{
				INFO_PRINTF2(_L("InitFrameL() return incorrect value of %d"), err);
				return EFail;
				}
			else
				{
				INFO_PRINTF1(_L("InitFrameL() call succeeded"));
				}
			
			break;
			}
		case EBlockStreamerLGetBlocks:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestStreamedDecode* sharedTestExt = CFwExtTestStreamedDecode::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageDecoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvStreamedDecode* streamDecode = iImageDecoder->BlockStreamerL();
			INFO_PRINTF1(_L("BlockStreamerL() succeeded."));
						
			HBufC8* buffer = HBufC8::NewMaxL(KBufferSize);
			CleanupStack::PushL(buffer);
	  		TPtr8 ptr = buffer->Des();

			CImageFrame* imageFrame = CImageFrame::NewL(ptr, KBufferSize);
			INFO_PRINTF1(_L("Successfully created CImageFrame for GetBlocks()."));
			CleanupStack::PushL(imageFrame);
			TFrameLayout frameLayout(KMaxPlanesInFrame);
			imageFrame->SetFrameLayoutL(frameLayout);
			
			TRequestStatus status = KRequestPending;
			TInt seqPosition = 4;
			TInt numBlocksToGet = 3;
			TInt numBlocksRead = 0;
			
			sharedTestExt->SetBlocks(status, *imageFrame, seqPosition, numBlocksToGet, numBlocksRead);
	
			iActiveListener = new (ELeave) CActiveListener();
			iActiveListener->InitialiseActiveListener();
			streamDecode->GetBlocks(iActiveListener->iStatus, *imageFrame, seqPosition, numBlocksToGet, numBlocksRead);
			CActiveScheduler::Start();
			TInt statusErr = iActiveListener->iStatus.Int();
			CleanupStack::PopAndDestroy(imageFrame);
			CleanupStack::PopAndDestroy(buffer);
			
			if(statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("GetBlocks() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				INFO_PRINTF2(_L("GetBlocks() async call passed with %d"), statusErr);
				}
			
			break;
			}
		case EBlockStreamerLGetNextBlocks:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestStreamedDecode* sharedTestExt = CFwExtTestStreamedDecode::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageDecoder->CustomSyncL(TInt(sharedTestExtBase));
			
			TImageConvStreamedDecode* streamDecode = iImageDecoder->BlockStreamerL();
			INFO_PRINTF1(_L("BlockStreamerL() succeeded."));
						
			HBufC8* buffer = HBufC8::NewMaxL(KBufferSize);
			CleanupStack::PushL(buffer);
	  		TPtr8 ptr = buffer->Des();

			CImageFrame* imageFrame = CImageFrame::NewL(ptr, KBufferSize);
			INFO_PRINTF1(_L("Successfully created CImageFrame for GetBlocks()."));
			CleanupStack::PushL(imageFrame);
			TFrameLayout frameLayout(KMaxPlanesInFrame);
			imageFrame->SetFrameLayoutL(frameLayout);
			
			TRequestStatus status = KRequestPending;
			TInt numBlocksToGet = 3;
			TInt numBlocksRead = 0;
			TBool haveMoreBlocks = ETrue;
			
			sharedTestExt->SetNextBlocks(status, *imageFrame, numBlocksToGet, numBlocksRead, haveMoreBlocks);
			
			iActiveListener = new (ELeave) CActiveListener();
			iActiveListener->InitialiseActiveListener();
			streamDecode->GetNextBlocks(iActiveListener->iStatus, *imageFrame, numBlocksToGet, numBlocksRead, haveMoreBlocks);
			CActiveScheduler::Start();
			TInt statusErr = iActiveListener->iStatus.Int();
			CleanupStack::PopAndDestroy(imageFrame);
			CleanupStack::PopAndDestroy(buffer);
			
			if(statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("GetNextBlocks() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				INFO_PRINTF2(_L("GetNextBlocks() async call passed with %d"), statusErr);
				}

			break;
			}
		case EGetDestinationSize:
			{
			INFO_PRINTF1(_L("GetDestinationSize()"));
			TSize destinationSize(1234, 5678);
			TSize destinationSizeCheck(destinationSize.iWidth/2,
										destinationSize.iHeight/2);
			TInt frameNumber = 9999;
			TInt frameNumberCheck = iImageDecoder->GetDestinationSize(destinationSize, frameNumber);
			if(frameNumber == frameNumberCheck
				&& destinationSize == destinationSizeCheck)
				{
				INFO_PRINTF1(_L("GetDestinationSize() succeeded."));
				}
			else
				{
				INFO_PRINTF1(_L("GetDestinationSize() failed."));
				return EFail;
				}

			break;
			}
		case ESetClippingRectL:
			{
			INFO_PRINTF1(_L("SetClippingRectL()"));
			
			TRect clippingRect(2,4,6,8);
			TRAP(err, iImageDecoder->SetClippingRectL(&clippingRect));
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("SetClippingRectL() failed."));
				return EFail;
				}
			INFO_PRINTF1(_L("SetClippingRectL() succeeded."));
			
			break;
			}
		case EPrepare:
			{
			// Set up the shared extension object to pass calls from extension
			// directly back to the test code.  Must be pointer to the base class
			// as we use reinterpret_cast in HandleCustomSync to cast to the 
			// appropriate C Extension
			CFwExtTestPrepare* sharedTestExt = CFwExtTestPrepare::NewL();
			MImageConvExtension* sharedTestExtBase = sharedTestExt;
			// Ownership passed to plugin so that it's now shared between both client and plugin
			iImageDecoder->CustomSyncL(TInt(sharedTestExtBase));
			
			iActiveListener = new (ELeave) CActiveListener();
			iActiveListener->InitialiseActiveListener();
			
			sharedTestExt->SetPrepare(ETrue);
	
			iImageDecoder->Prepare(&iActiveListener->iStatus);
			
			CActiveScheduler::Start();
			TInt statusErr = iActiveListener->iStatus.Int();
			
			if (statusErr != KErrNone)
				{
				INFO_PRINTF2(_L("Prepare() async call failed with %d"), statusErr);
				return EFail;
				}
			else
				{
				// Prepare processing complete, now do the conversion proper
				iTestBitmap = new (ELeave) CFbsBitmap;
				User::LeaveIfError(iTestBitmap->Create(TSize(10,10), EColor16M));
		
				iActiveListener->InitialiseActiveListener();

				iImageDecoder->Convert(&iActiveListener->iStatus, *iTestBitmap, 0);
				
				CActiveScheduler::Start();
				statusErr = iActiveListener->iStatus.Int();

				delete iTestBitmap; iTestBitmap = NULL;

				if (statusErr != KErrNone)
					{
					INFO_PRINTF2(_L("Convert after Prepare() async call failed with %d"), statusErr);
					return EFail;
					}
				else
					{
					INFO_PRINTF2(_L("Prepare() async call passed with %d"), statusErr);
					}
				}
			break;
			}
		default:
			{
			SetFail(_L("Called unsupported function"));
			}
		}
	
	return iTestStepResult;
	}

void RFwExtDecSupportedTest::Close()
	{
	delete iImageDecoder;
	iImageDecoder = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	delete iActiveListener;
	iActiveListener = NULL;
	RFbsSession::Disconnect();
	}


