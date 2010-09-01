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
// This file contains the implementation of the unit test steps for
// the MediaSvr backwards compatibility layer
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

// Specific includes for this test suite
#include <mdaimageconverter.h>
#include "TestStepMediaSvr.h"
#include "TestSuiteMediaSvr.h"

#include "MediaSvrTest.h"
#include "TMDATEST.H"
#include "ICLFbsSessionTest.h"

const TInt KArrayGranularity5 = 5;

TMdaInvalidClipFormat::TMdaInvalidClipFormat()
	:TMdaClipFormat(KUidMdaVideoCurrentFrame,sizeof(TMdaInvalidClipFormat))
	{	
	} 


// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest* CMediaSvrTest::NewL()
	{
	CMediaSvrTest* self = new(ELeave) CMediaSvrTest;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest::CMediaSvrTest()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-001");

	// Heap size necessary for PNG and JPEG decoders
	iHeapSize = 200000;
	}

/**
 *
 * Pre-test setup. Create array of test images
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);

	iTestImages->AppendL(TTestImage(KMediaSvrTest1BppBmp,&iBmpFormat,&iBmp1BppCodec));
	iTestImages->AppendL(TTestImage(KMediaSvrTest4BppBmp,&iBmpFormat,&iBmp4BppCodec));
	iTestImages->AppendL(TTestImage(KMediaSvrTest8BppBmp,&iBmpFormat,&iBmp8BppCodec));
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));

	iTestImages->AppendL(TTestImage(KMediaSvrTestJfifMono,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif420,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif422,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif444,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif10,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif30,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif60,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestJfif100,&iJfifFormat,NULL));

	iTestImages->AppendL(TTestImage(KMediaSvrTest1BppMonoMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest2BppMonoMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest4BppMonoMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest8BppMonoMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest4BppColorMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest8BppColorMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest12BppColorMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest16BppColorMbm,&iMbmFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppColorMbm,& iMbmFormat,NULL));

	iTestImages->AppendL(TTestImage(KMediaSvrTestPng1,& iPngFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestPng2,& iPngFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestPng3,& iPngFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestPng4,& iPngFormat,NULL));

	iTestImages->AppendL(TTestImage(KMediaSvrTestTif1,NULL,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestTif2,NULL,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestTif3,NULL,NULL));


	iTestImages->AppendL(TTestImage(KMediaSvrTestGif1,& iGifFormat,NULL));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test loading all supported images
 *
 * @return	"TVerdict"
 *			Whether the images loaded successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("this test loads images in each supported format"));
	CFbsBitmap* bmp=NULL;
	iTestStepResult = EPass;

	for (int i=0;i<iTestImages->Count();i++) 
		{
		TRAPD(err, bmp = LoadImageL((*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format(),(*iTestImages)[i].Codec()));		
		if (err==KErrNone) 
			delete bmp;
		else
			{
			LogError((*iTestImages)[i].SourceFile(),_L("Problem loading file"));
			iTestStepResult = EFail;
			}
		}

	return iTestStepResult;
	}

/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest2* CMediaSvrTest2::NewL()
	{
	CMediaSvrTest2* self = new(ELeave) CMediaSvrTest2;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest2::CMediaSvrTest2()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-002");
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest2::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,NULL,NULL));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Attempt to load an image, without specifying the format of the image
 *
 * @return	"TVerdict"
 *			Whether the load completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest2::DoTestStepL()
	{
	INFO_PRINTF1(_L("this test attempts to load an image without specifying a format"));

	CFbsBitmap* bmp = NULL;

	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		TRAPD(err, bmp = LoadImageL((*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format(),(*iTestImages)[i].Codec()));		
		if (err==KErrNone) 
			delete bmp;
		else
			{
			LogError((*iTestImages)[i].SourceFile(),_L("Problem loading file"));
			iTestStepResult = EFail;
			}
		}
	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest2::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest3* CMediaSvrTest3::NewL()
	{
	CMediaSvrTest3* self = new(ELeave) CMediaSvrTest3;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest3::CMediaSvrTest3()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-003");

	// Heap size necessary for WriteImageToDescriptorL call
	iHeapSize = 800000;
	}
/**
 *
 * Pre-test setup. store the images to be used in the test in an array
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest3::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test storing an image in a descriptor and restoring it from the descriptor
 *
 * @return	"TVerdict"
 *			Whether the final image matches the original image
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest3::DoTestStepL()
	{
	if (DescriptorSize()==0)
		INFO_PRINTF1(_L("Writes a file to a descriptor and reads it to check format"));
	else
		INFO_PRINTF1(_L("Writes a file to a small descriptor and checks error result"));

	CFbsBitmap* bmp = NULL;
	CFbsBitmap* checkbmp = NULL;

	bmp = LoadImageL((*iTestImages)[0].SourceFile(),(*iTestImages)[0].Format(),(*iTestImages)[0].Codec());
	CleanupStack::PushL(bmp);


	iTestStepResult = EPass;
	HBufC8* imageBuffer = NULL;

	TInt err;
	TRAP(err, imageBuffer = WriteImageToDescriptorL(bmp, (*iTestImages)[0].Format(),(*iTestImages)[0].Codec(), DescriptorSize()));
	CleanupStack::PushL(imageBuffer);
	if (ExpectedResult()==KErrNone)
		{
		if (err!=KErrNone) 
			{
			INFO_PRINTF2(_L("Unexpected leave(%d)"), err);
			iTestStepResult = EFail;
			}
		else
			{
			TPtr8 buf = imageBuffer->Des();
			TRAP(err,checkbmp = ReadImageFromDescriptorL(buf,NULL,NULL));
			if (err==KErrNone)
				{
				TVerdict compareBitmaps;
				compareBitmaps = CheckBitmaps(*checkbmp,*bmp);
				if (compareBitmaps == EFail) 
					{
					iTestStepResult = EFail;
					}
				}
			else 
				{
				iTestStepResult = EFail;
				}
			}
		}
	else
		{
		if (err!=ExpectedResult()) 
			{
			INFO_PRINTF3(_L("Unexpected leave(%d - expected %d)"), err, ExpectedResult());
			iTestStepResult = EFail;
			}
		}

	CleanupStack::PopAndDestroy(2); //  bmp, imageBuffer
	return iTestStepResult;
	}

/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest3::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest4.
 *
 *
 * @return	"CMediaSvrTest4*"
 *			The constructed CMediaSvrTest4
 *
 * @xxxx
 * 
 */
CMediaSvrTest4* CMediaSvrTest4::NewL()
	{
	CMediaSvrTest4* self = new(ELeave) CMediaSvrTest4;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest4::CMediaSvrTest4()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-004");
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest4::DoTestStepPreambleL()
	{
	iInitialImage.SetTestImage(KMediaSvrTest24BppBmp,NULL,NULL);
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrGifTempFile,&iGifFormat,&iGifCodec,KMediaSvrTestGifRef));

	iTestImages->AppendL(TTestImage(KMediaSvrBmpTempFile,&iBmpFormat,&iBmp1BppCodec, KMediaSvrTest1BppBmp));
	iTestImages->AppendL(TTestImage(KMediaSvrBmpTempFile,&iBmpFormat,&iBmp4BppCodec, KMediaSvrTest4BppBmp));
	iTestImages->AppendL(TTestImage(KMediaSvrBmpTempFile,&iBmpFormat,&iBmp8BppCodec, KMediaSvrTest8BppBmp));
	iTestImages->AppendL(TTestImage(KMediaSvrBmpTempFile,&iBmpFormat,&iBmp24BppCodec, KMediaSvrTest24BppBmp));

	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatMonoQ50,NULL,KMediaSvrTestJfifMono));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor420Q50,NULL,KMediaSvrTestJfif420));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor422Q50,NULL,KMediaSvrTestJfif422));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor444Q50,NULL,KMediaSvrTestJfif444));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor444Q10,NULL,KMediaSvrTestJfif10));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor444Q30,NULL,KMediaSvrTestJfif30));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor444Q60,NULL,KMediaSvrTestJfif60));
	iTestImages->AppendL(TTestImage(KMediaSvrJfifTempFile,&iJfifFormatColor444Q100,NULL,KMediaSvrTestJfif100));

	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatGray2,NULL, KMediaSvrTest1BppMonoMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatGray4,NULL, KMediaSvrTest2BppMonoMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatGray16,NULL, KMediaSvrTest4BppMonoMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatGray256,NULL, KMediaSvrTest8BppMonoMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatColor16,NULL, KMediaSvrTest4BppColorMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatColor256,NULL, KMediaSvrTest8BppColorMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatColor4K,NULL, KMediaSvrTest12BppColorMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatColor64K,NULL, KMediaSvrTest16BppColorMbm));
	iTestImages->AppendL(TTestImage(KMediaSvrMbmTempFile,&iMbmFormatColor16M,NULL, KMediaSvrTest24BppColorMbm));

	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Save images in each supported format and compare the saved bitmaps to known reference files
 *
 * @return	"TVerdict"
 *			Whether the saved images match the reference bitmaps
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest4::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("MediaSvr Test 4"));
	INFO_PRINTF1(_L("this test attempt to save images in all supported formats"));

	CFbsBitmap* bmp = NULL;
	
	bmp = LoadImageL(iInitialImage.SourceFile(),iInitialImage.Format(),iInitialImage.Codec());
	CleanupStack::PushL(bmp);

	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		TInt err;
		TRAP(err, SaveImageL(bmp,(*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format(),(*iTestImages)[i].Codec()));
		if (err==KErrNone) 
			{
			TVerdict result = CompareFilesL((*iTestImages)[i].SourceFile(),(*iTestImages)[i].RefFile());
			if (result == EFail)
				{
				iTestStepResult = EFail;
				LogError((*iTestImages)[i].RefFile(), _L("Saved file does not match reference file"));
				}	
			}
		else
			iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(); // bmp
	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest4::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest5* CMediaSvrTest5::NewL()
	{
	CMediaSvrTest5* self = new(ELeave) CMediaSvrTest5;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest5::CMediaSvrTest5()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-005");

	// Heap size necessary for WriteImageToDescriptorL call
	iHeapSize = 800000;
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest5::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test streaming an image from a descriptor
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest5::DoTestStepL()
	{
	INFO_PRINTF1(_L("Stream a file from a descriptor"));

	CFbsBitmap* bmp = NULL;
	CFbsBitmap* checkbmp = NULL;

	bmp = LoadImageL((*iTestImages)[0].SourceFile(),(*iTestImages)[0].Format(),(*iTestImages)[0].Codec());
	CleanupStack::PushL(bmp);

	iTestStepResult = EPass;
	HBufC8* imageBuffer = NULL;

	TInt err;
	TRAP(err, imageBuffer = WriteImageToDescriptorL(bmp, (*iTestImages)[0].Format(),(*iTestImages)[0].Codec()));
	CleanupStack::PushL(imageBuffer);
	if (err!=KErrNone) 
		{
		iTestStepResult = EFail;
		}
	TPtr8 buf = imageBuffer->Des();
	TRAP(err,checkbmp = StreamImageFromDescriptorL(buf,NULL,NULL));
	if (err==KErrNone)
		{
		TVerdict compareBitmaps;
		compareBitmaps = CheckBitmaps(*checkbmp,*bmp);
		if (compareBitmaps == EFail) 
			{
			iTestStepResult = EFail;
			}
		delete checkbmp;
		}
	else 
		{
		iTestStepResult = EFail;
		}
	CleanupStack::PopAndDestroy(2); //  bmp, imageBuffer
	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest5::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest6* CMediaSvrTest6::NewL()
	{
	CMediaSvrTest6* self = new(ELeave) CMediaSvrTest6;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest6::CMediaSvrTest6()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-006");
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest6::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}

/**
 *
 * Test streaming an image from a file
 *
 * @return	"TVerdict"
 *			Whether the image was correctly retrieved
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest6::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("Stream an image from a file"));

	CFbsBitmap* bmp = NULL;
	CFbsBitmap* checkbmp = NULL;
	
	bmp = LoadImageL((*iTestImages)[0].SourceFile(),(*iTestImages)[0].Format(),(*iTestImages)[0].Codec());
	CleanupStack::PushL(bmp);

	iTestStepResult = EPass;

	TInt err;
	TRAP(err,checkbmp = StreamImageL((*iTestImages)[0].SourceFile(),(*iTestImages)[0].Format(),(*iTestImages)[0].Codec()));
	if (err==KErrNone)
		{
		TVerdict compareBitmaps;
		compareBitmaps = CheckBitmaps(*checkbmp,*bmp);
		if (compareBitmaps == EFail) 
			{
			iTestStepResult = EFail;
			}
		delete checkbmp; // nothing can leave between creation of bitmap and deletion, so don't use cleanup stack
		}
	else 
		{
		iTestStepResult = EFail;
		}
	CleanupStack::PopAndDestroy(); //bmp
	return iTestStepResult;
	}

/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest6::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest7.
 *
 *
 * @return	"CMediaSvrTest7*"
 *			The constructed CMediaSvrTest7
 *
 * @xxxx
 * 
 */
CMediaSvrTest7* CMediaSvrTest7::NewL()
	{
	CMediaSvrTest7* self = new(ELeave) CMediaSvrTest7;
	return self;
	}

CMediaSvrTest7::CMediaSvrTest7()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-007");
	}

/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest7::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest1BppBmp,&iJfifFormat,NULL));
	
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Attempt to load a valid image, specifying the incorrect format.
 *
 * @return	"TVerdict"
 *			Whether CMdaImageFileToBitmapUtility correctly refuses to load the image
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest7::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("this test attempts to load an image, specifying the incorrect format. The open should not succeed"));
	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		TMdaClipFormat *clipFormat = (TMdaClipFormat*) (*iTestImages)[i].Format();
		TMdaPackage* codec = (TMdaPackage*) (*iTestImages)[i].Codec();

		CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
		CleanupStack::PushL(ftob);
		TFileName fileName = DefaultPath();
		fileName.Append((*iTestImages)[i].SourceFile());
		ftob->OpenL(fileName,clipFormat,codec);
			
		WaitForResult();
		if (iError!=KErrNotFound) 
			{
			iTestStepResult = EFail;	
			}
		CleanupStack::PopAndDestroy(); //ftob
		}
	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest7::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest8.
 *
 *
 * @return	"CMediaSvrTest8*"
 *			The constructed CMediaSvrTest8
 *
 * @xxxx
 * 
 */
CMediaSvrTest8* CMediaSvrTest8::NewL()
	{
	CMediaSvrTest8* self = new(ELeave) CMediaSvrTest8;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest8::CMediaSvrTest8()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-008");
	}

/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest8::DoTestStepPreambleL()
	{
	iInitialImage.SetTestImage(KMediaSvrTest24BppBmp,NULL,NULL);
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrInvalidTempFile,&iBmpFormat,&iBmp24BppCodec));

	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test saving to an invalid file name
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest8::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("MM-ICL-MEDIASVR-U-008"));
	INFO_PRINTF1(_L("this test attempt to save an image to an invalid file"));
	// read in a bool value from the config file

	CFbsBitmap* bmp = NULL;

	bmp = LoadImageL(iInitialImage.SourceFile(),iInitialImage.Format(),iInitialImage.Codec());
	CleanupStack::PushL(bmp);

	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		TInt err;
		TRAP(err, SaveImageL(bmp, (*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format(),(*iTestImages)[i].Codec()));
		if (err==KErrNone)  // should not be able to open the invalid file
			{
			iTestStepResult = EFail;
			}
		}
	CleanupStack::PopAndDestroy(); // bmp
	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest8::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest9* CMediaSvrTest9::NewL()
	{
	CMediaSvrTest9* self = new(ELeave) CMediaSvrTest9;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest9::CMediaSvrTest9()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-009");
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest9::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iInvalidFormat,NULL));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test passing an invalid format to CMdaImageFileToBitmapUtility
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest9::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("this test attempts to load an image, specifying an invalid format"));

	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
		CleanupStack::PushL(ftob);

		ftob->OpenL((*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format());
			
		if (WaitForResult()==KErrNone)
			{
			iTestStepResult = EFail;	
			} 
		CleanupStack::PopAndDestroy(); //ftob
		}

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest9::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest10* CMediaSvrTest10::NewL()
	{
	CMediaSvrTest10* self = new(ELeave) CMediaSvrTest10;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest10::CMediaSvrTest10()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-010");
	}

/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest10::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);

	iTestImages->AppendL(TTestImage(KMediaSvrTestTxt,NULL,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestTxt,&iJfifFormat,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrTestTxt,&iMbmFormat,NULL));

	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test trying to load an invalid file as an image
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest10::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("this test attempts to load a text file as an image"));

	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
		CleanupStack::PushL(ftob);

		ftob->OpenL((*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format());
			
		if (WaitForResult()==KErrNone)
			{
			iTestStepResult = EFail;	
			} 
		CleanupStack::PopAndDestroy(); //ftob
		}

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest10::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest11* CMediaSvrTest11::NewL()
	{
	CMediaSvrTest11* self = new(ELeave) CMediaSvrTest11;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest11::CMediaSvrTest11()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-011");
	}

/**
 *
 * Pre-test setup. Create an instance of the stress test class
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest11::DoTestStepPreambleL()
	{
	iStressTest = new (ELeave) CMdaStressTest;
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Multiple thread stress testing. The test creates multiple threads which attempt to load
 * images. When a thread finishes, it is restarted. The test continues for a predetermined length
 * of time
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest11::DoTestStepL()
	{
	INFO_PRINTF1(_L("Multiple thread stress testing"));

	iTestStepResult = EPass;

	TRAPD(err, iStressTest->DoTestL(DefaultPath()));
	if (err!=KErrNone)
		iTestStepResult = EFail;

	return iTestStepResult;
	}

/**
 *
 * Cleanup memory allocated for Stress Test
 *
 */
TVerdict CMediaSvrTest11::DoTestStepPostambleL()
	{
	delete iStressTest;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest12* CMediaSvrTest12::NewL()
	{
	CMediaSvrTest12* self = new(ELeave) CMediaSvrTest12;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest12::CMediaSvrTest12()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-012");
	}
/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest12::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}

/**
 *
 * Alloc failure testing while loading an image 
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest12::DoTestStepL()
	{

	// need to disconnect from the Font & Bitmap server
	// and then reconnect for alloc testing as it allocates memory on the fly
	RFbsSession::Disconnect();
	
	__MM_HEAP_MARK;

	// connect to the bitmap server and check the error code
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = DoTestStepFbsL();
	CleanupStack::PopAndDestroy(fbsSession); // RFbsSession::Disconnect()
	
	__MM_HEAP_MARKEND; 

	return verdict;
	}
/**
 *
 * Alloc failure testing while loading an image 
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest12::DoTestStepFbsL()
	{
	INFO_PRINTF1(_L("Loads an image with alloc failure"));
	CFbsBitmap* bmp = NULL;
	iTestStepResult = EPass;
	
	TRAPD(err, bmp = LoadImageWithAllocFailL((*iTestImages)[0].SourceFile(),NULL,NULL));
	if (err!=KErrNone) 
		iTestStepResult = EFail;

	delete bmp;

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest12::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest13* CMediaSvrTest13::NewL()
	{
	CMediaSvrTest13* self = new(ELeave) CMediaSvrTest13;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest13::CMediaSvrTest13()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-013");
	}

/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest13::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,NULL,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrBmpTempFile,&iBmpFormat,&iBmp24BppCodec));

	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Alloc failure testing while saving an image
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest13::DoTestStepL()
	{
	INFO_PRINTF1(_L("Saves an image with alloc failure"));
	CFbsBitmap* bmp = NULL;

	bmp = LoadImageL((*iTestImages)[0].SourceFile(),NULL,NULL);

	iTestStepResult = EPass;

	TInt err;
	TRAP(err,SaveImageWithAllocFailL(bmp,(*iTestImages)[1].SourceFile(),(*iTestImages)[1].Format(),(*iTestImages)[1].Codec()));
	if (err!=KErrNone) 
		iTestStepResult = EFail;

	delete bmp;

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest13::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest14* CMediaSvrTest14::NewL()
	{
	CMediaSvrTest14* self = new(ELeave) CMediaSvrTest14;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest14::CMediaSvrTest14()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-014");

	// Heap size necessary for WriteImageToDescriptorL call
	iHeapSize = 800000;
	}
/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest14::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);

	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,NULL,NULL));
	iTestImages->AppendL(TTestImage(KMediaSvrBmpTempFile,&iBmpFormat,&iBmp24BppCodec));

	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Alloc failure testing while streaming an image
 *
 * @return	"TVerdict"
 *			The result of the test
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest14::DoTestStepL()
	{
	INFO_PRINTF1(_L("Streams an image with alloc failure"));
	CFbsBitmap* bmp = NULL;
	iTestStepResult = EPass;

	bmp = LoadImageL((*iTestImages)[0].SourceFile(),NULL,NULL);

	HBufC8 * descBuf = WriteImageToDescriptorL(bmp,(*iTestImages)[1].Format(),(*iTestImages)[1].Codec());

	TInt err;
	TRAP(err, bmp = StreamImageFromDescWithAllocFailL(descBuf->Des(),NULL,NULL));
	if (err!=KErrNone) 
		iTestStepResult = EFail;

	delete bmp;

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest14::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest15.
 *
 *
 * @return	"CMediaSvrTest15*"
 *			The constructed CMediaSvrTest15
 *
 * @xxxx
 * 
 */
CMediaSvrTest15* CMediaSvrTest15::NewL()
	{
	CMediaSvrTest15* self = new(ELeave) CMediaSvrTest15;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest15::CMediaSvrTest15()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-015");
	}

/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest15::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}

/**
 *
 * Test cancelling and restarting decoding
 *
 * @return	"TVerdict"
 *			The result of the test
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest15::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("Test cancelling during image decoding"));

	CFbsBitmap* bmp=NULL;

	iTestStepResult = EPass;
	
	for (int i=0;i<iTestImages->Count();i++) 
		{
		TRAPD(err, bmp = CancelLoadImageL((*iTestImages)[i].SourceFile(),(*iTestImages)[i].Format(),(*iTestImages)[i].Codec()));		
		delete bmp;

		if (err!=KErrNone) 
			{
			iTestStepResult = EFail;
			}
		}
	return iTestStepResult;
	}

/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest15::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}
//--------------------------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest16.
 *
 *
 * @return	"CMediaSvrTest16*"
 *			The constructed CMediaSvrTest16
 *
 * @xxxx
 * 
 */
CMediaSvrTest16* CMediaSvrTest16::NewL()
	{
	CMediaSvrTest16* self = new(ELeave) CMediaSvrTest16;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest16::CMediaSvrTest16()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-016");
	iHeapSize = 1024*128;
	}
/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest16::DoTestStepPreambleL()
	{
	iInitialImage.SetTestImage(KMediaSvrTest24BppBmp,NULL,NULL);
	iOutputImage.SetTestImage(KMediaSvrJfifTempFile,&iJfifFormatMonoQ50,NULL,KMediaSvrTestJfifMono);
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test cancelling and restarting encoding
 *
 * @return	"TVerdict"
 *			The result of the test
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest16::DoTestStepL()
	{
	INFO_PRINTF1(_L("Test cancelling during image encoding"));

	CFbsBitmap* bmp = NULL;
	CFbsBitmap*checkbmp =NULL;
	CFbsBitmap*refbmp = NULL;
	
	bmp = LoadImageL(iInitialImage.SourceFile(),iInitialImage.Format(),iInitialImage.Codec());
	CleanupStack::PushL(bmp);

	iTestStepResult = EPass;
	
	TInt err;
	TRAP(err, CancelSaveImageL(bmp, iOutputImage.SourceFile(),iOutputImage.Format(),iOutputImage.Codec()));
	if (err!=KErrNone) 
		{
		iTestStepResult = EFail;
		}
	TRAP(err,checkbmp = LoadImageL(iOutputImage.SourceFile(),iOutputImage.Format(),iOutputImage.Codec()));
	CleanupStack::PushL(checkbmp);
	if (err!=KErrNone)
		{
		iTestStepResult = EFail;
		}
	// leave from the following error, as it is an indication that the test is not setup correctly
	refbmp = LoadImageL(iOutputImage.RefFile(),iOutputImage.Format(),iOutputImage.Codec());
	CleanupStack::PushL(refbmp);
	TVerdict compareBitmaps;
	compareBitmaps = CheckBitmaps(*checkbmp,*refbmp);
	if (compareBitmaps == EFail) 
		{
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(3); // checkbmp, refbmp, bmp

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest16::DoTestStepPostambleL()
	{
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}

//--------------------------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest17.
 *
 *
 * @return	"CMediaSvrTest17*"
 *			The constructed CMediaSvrTest17
 *
 * @xxxx
 * 
 */
CMediaSvrTest17* CMediaSvrTest17::NewL()
	{
	CMediaSvrTest17* self = new(ELeave) CMediaSvrTest17;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest17::CMediaSvrTest17()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-017");
	}
/**
 *
 * Pre-test setup. Create array with details of the source images to be used
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest17::DoTestStepPreambleL()
	{
	iTestImage.SetTestImage(KMediaSvrTest24BppBmp,NULL,NULL);
	iExpectedFrameInfo.iOverallSizeInPixels.iWidth = 193;
	iExpectedFrameInfo.iOverallSizeInPixels.iHeight = 151;
	iExpectedFrameInfo.iBitsPerPixel = 24;
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test cancelling and restarting encoding
 *
 * @return	"TVerdict"
 *			The result of the test
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest17::DoTestStepL()
	{
	INFO_PRINTF1(_L("Check frame information returned by OpenL"));

	TFrameInfo frameInfo;

	iTestStepResult = EPass;

	TBuf<256> messagebuf;

	TRAPD(err, frameInfo = GetFrameInfoL(iTestImage.SourceFile(),NULL,NULL));		
	if (err==KErrNone) 
		{
		if (frameInfo.iOverallSizeInPixels.iWidth != iExpectedFrameInfo.iOverallSizeInPixels.iWidth ||
			frameInfo.iOverallSizeInPixels.iHeight != iExpectedFrameInfo.iOverallSizeInPixels.iHeight)
			{
			messagebuf.Format(_L("size (%d,%d) instead of expected (%d,%d)"),frameInfo.iOverallSizeInPixels.iWidth,
																			frameInfo.iOverallSizeInPixels.iHeight,
																			iExpectedFrameInfo.iOverallSizeInPixels.iWidth,
																			iExpectedFrameInfo.iOverallSizeInPixels.iHeight);
			LogError(_L("FrameInfo incorrect"),messagebuf);
			iTestStepResult = EFail;
			}
/* Can no longer test this property as it doesn't exist in the MDA TFrameInfo
		if (frameInfo.iFrameDisplayMode != iExpectedFrameInfo.iFrameDisplayMode)
			{
			LogError(_L("FrameInfo incorrect"),_L("incorrect display mode"));
			iTestStepResult = EFail;
			} */
		if (frameInfo.iBitsPerPixel != iExpectedFrameInfo.iBitsPerPixel)
			{
			messagebuf.Format(_L("%d bpp instead of expected %d "),frameInfo.iBitsPerPixel,
																	iExpectedFrameInfo.iBitsPerPixel);
			LogError(_L("FrameInfo incorrect"),messagebuf);
			iTestStepResult = EFail;
			}
		}
	else
		{
		iTestStepResult = EFail;
		LogError(iTestImage.SourceFile(),_L("Problem opening file"));
		}
	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest17::DoTestStepPostambleL()
	{
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}

// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest18* CMediaSvrTest18::NewL()
	{
	CMediaSvrTest18* self = new(ELeave) CMediaSvrTest18;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest18::CMediaSvrTest18()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-018");
	// Increase the heap size for this test
	iHeapSize = 0x20000;
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest18::DoTestStepPreambleL()
	{
	iTestImage.SetTestImage(KMediaSvrTestTransparentGif,NULL,NULL);
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test streaming an image from a descriptor
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest18::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("Load transparent image"));

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	CFbsBitmap* mask = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(mask);

	iTestStepResult = EPass;

	TInt err;
	TRAP(err,LoadTransparentImageL(bitmap,mask,iTestImage.SourceFile(),NULL,NULL));

	if (err==KErrNone) 
		{
		TVerdict compareBitmaps = CheckBitmaps(*bitmap,*mask);
		if (compareBitmaps == EFail) 
			{
			iTestStepResult = EFail;
			}
		}
	else 
		{
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(2); //mask, bitmap

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest18::DoTestStepPostambleL()
	{
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}


// --------------------------------------------

/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest19* CMediaSvrTest19::NewL()
	{
	CMediaSvrTest19* self = new(ELeave) CMediaSvrTest19;
	return self;
	}

/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest19::CMediaSvrTest19()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-019");
	}
/**
 *
 * Pre-test setup. Create array with the source images to be used and the various formats
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest19::DoTestStepPreambleL()
	{
	iTestImage.SetTestImage(KMediaSvrTestTransparentGif,NULL,NULL);
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test trying to decode a transparent image, passing incompatible bitmaps as the destination
 * of the decode
 *
 * @return	"TVerdict"
 *			The result of the test step
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest19::DoTestStepL()
	{
	
	INFO_PRINTF1(_L("Pass incorrectly formatted images to ConvertL"));

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	CFbsBitmap* mask = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(mask);

	iTestStepResult = EPass;

	TInt err;
	TRAP(err,FailLoadTransparentImageL(bitmap,mask,iTestImage.SourceFile(),NULL,NULL));

	if (err!=KErrArgument) 
		{
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(2); //mask, bitmap

	return iTestStepResult;
	}
/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest19::DoTestStepPostambleL()
	{
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}

// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest20* CMediaSvrTest20::NewL()
	{
	CMediaSvrTest20* self = new(ELeave) CMediaSvrTest20;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest20::CMediaSvrTest20()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-020");
	
	// Heap size necessary for source buffer 
	iHeapSize = 200000;
	}

/**
 *
 * Pre-test setup. store the images to be used in the test in an array
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest20::DoTestStepPreambleL()
	{
	iTestImages = new (ELeave) CArrayFixFlat<TTestImage>(KArrayGranularity5);
	iTestImages->AppendL(TTestImage(KMediaSvrTest24BppBmp,&iBmpFormat,&iBmp24BppCodec));
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}
/**
 *
 * Test re-using CMdaImageDescToBitmapUtility after a previous ConvertL()
 * returned KErrUnderflow (Defect INC011980). If the test does not panic
 * (EIllegalContinueConvert) the test pass.
 *
 * @return	"TVerdict"
 *			EPass if the test did not panic
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest20::DoTestStepL()
	{

   	INFO_PRINTF1(_L("re-using CMdaImageDescToBitmapUtility after a previous ConvertL() returned KErrUnderflow (Defect INC011980)"));
	
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	RFile srcFile;
	TFileName fileName = DefaultPath();
	fileName.Append((*iTestImages)[0].SourceFile()); // use full name now
	User::LeaveIfError(srcFile.Open(fs,fileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(srcFile);

	TInt fileSize;	
	User::LeaveIfError(srcFile.Size(fileSize));
	HBufC8* buffer = HBufC8::NewMaxLC(fileSize);
	TPtr8 bufPtr(buffer->Des());

	// Read the file into a descriptor
	User::LeaveIfError(srcFile.Read(bufPtr,fileSize));

	//Create an instance of CMdaImageDescToBitmapUtility
	CMdaImageDescToBitmapUtility* dtob = CMdaImageDescToBitmapUtility::NewL(*this);
	CleanupStack::PushL(dtob);

	// Open with complete image
	dtob->OpenL(*buffer);
	WaitForResult();
	// We do not expect any error
	User::LeaveIfError(iError);

	// Create a bitmap from the frameInfo
	TFrameInfo frameInfo;
	dtob->FrameInfo(0, frameInfo);
	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));

	// Use a small buffer to force ConvertL() to return KErrUnderflow
	bufPtr.SetLength(0);

	// Expect ConvertL() to return KErrUnderflow
	dtob->ConvertL(*bmp,0);
	WaitForResult();
	if(iError!=KErrUnderflow)
		User::Leave(iError);

	// Reset the buffer length and reuse CMdaImageDescToBitmapUtility
	bufPtr.SetLength(fileSize);
	dtob->OpenL(*buffer);
	WaitForResult();
	User::LeaveIfError(iError);

	// If ConvertL() does not panic with EIllegalContinueConvert the test pass
	dtob->ConvertL(*bmp,0);
	WaitForResult();
	User::LeaveIfError(iError);

	CleanupStack::PopAndDestroy(5,bmp); // dtob, buffer, srcFile, fs, bmp

	return iTestStepResult;
	}

/**
 *
 * Cleanup memory allocated for test images
 *
 */
TVerdict CMediaSvrTest20::DoTestStepPostambleL()
	{
	delete iTestImages;
	return CTestStepMediaSvr::DoTestStepPostambleL();
	}

// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest21* CMediaSvrTest21::NewL()
	{
	CMediaSvrTest21* self = new(ELeave) CMediaSvrTest21;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest21::CMediaSvrTest21()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-021");
	}

/**
 *
 * Pre-test setup. store the images to be used in the test in an array
 *
 * @return	"TVerdict"
 *			Whether the test preamble completed successfully
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest21::DoTestStepPreambleL()
	{
	iSourceFile = KMediaSvrGifCommentFile;
	return CTestStepMediaSvr::DoTestStepPreambleL();
	}

/**
 *
 * Test CMdaImageDecoder::FrameCommentL()
 * Decode a file with comments and verify the comments is as expected.
 * 
 * DEF029548  ICL breaks BC with 7.0 
 *
 * @return	"TVerdict"
 *			EPass if the test did not panic
 *
 * @xxxx
 * 
 */
TVerdict CMediaSvrTest21::DoTestStepL()
	{
	INFO_PRINTF1(_L("Test CMdaDataReadUtility::FrameCommentL()"));
	INFO_PRINTF1(_L("DEF029548  ICL breaks BC with 7.0"));
	INFO_PRINTF2(_L("%S"),&iSourceFile);

	iTestStepResult = EFail;

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);

	//Create an instance of CMdaImageDescToBitmapUtility
	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);

	// Open with complete image
	TFileName fileName = DefaultPath();
	fileName.Append(iSourceFile);
	ftob->OpenL(fileName);
	WaitForResult();
	// We do not expect any error
	User::LeaveIfError(iError);

	// Create a bitmap from the frameInfo
	TFrameInfo frameInfo;
	ftob->FrameInfo(0, frameInfo);
	User::LeaveIfError(bmp->Create(frameInfo.iOverallSizeInPixels,EColor16M));

	// Convert the frame
	ftob->ConvertL(*bmp,0);
	WaitForResult();
	// We do not expect any error
	User::LeaveIfError(iError);

	//Get the frame comment for the first frame
	TPtrC comment(ftob->FrameCommentL(0));
	INFO_PRINTF2(_L("Comment: [%S]"),&comment);

	//Verify the comment
	if(!comment.Compare(KMediaSvrGifComment))
		iTestStepResult = EPass;

	CleanupStack::PopAndDestroy(2,bmp); // ftob, bmp

	return iTestStepResult;
	}

// --------------------------------------------
/**
 *
 * Static constructor for CMediaSvrTest.
 *
 *
 * @return	"CMediaSvrTest*"
 *			The constructed CMediaSvrTest
 *
 * @xxxx
 * 
 */
CMediaSvrTest22* CMediaSvrTest22::NewL()
	{
	CMediaSvrTest22* self = new(ELeave) CMediaSvrTest22;
	return self;
	}
/**
 *
 * Test step constructor.
 * Each test step initialises its own name.
 *
 * @xxxx
 * 
 */
CMediaSvrTest22::CMediaSvrTest22()
	{
	// store the name of this test case
	// this is the name that is used by the script file
	iTestStepName = _L("MM-ICL-MEDIASVR-U-022");

	// Heap size necessary for WriteImageToDescriptorL call
	iHeapSize = 800000;
	}
