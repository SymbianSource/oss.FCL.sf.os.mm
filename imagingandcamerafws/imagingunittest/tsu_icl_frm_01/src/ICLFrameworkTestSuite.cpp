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
//

#include <e32base.h>

// Test system includes
#include <testframework.h>
#include "ICLFrameworkTestSuite.h"
#include "ICLFrameWorkTestStep.h"
#include "ImageEncoderPrivTest.h"
#include "ImageDecoderTest.h"
#include "ImageEncoderTest.h"
#include "BufferedImageDecoderTest.h"
#include "ExtendedDecoderTest.h"
#include "ExtendedEncoderTest.h"
#include "ImageProcessorTest.h"
#include "ImageDecoderDRM.h"
#include "extendedjpegdecoderteststep.h"
#include "icl/ICL_UIDS.hrh"
#include "TestUids.hrh"

CICLFrameworkTestSuite::~CICLFrameworkTestSuite()
	{
	}

EXPORT_C CICLFrameworkTestSuite* CICLFrameworkTestSuite::NewL() 
    { 
	CICLFrameworkTestSuite* self = new (ELeave) CICLFrameworkTestSuite;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
    }

_LIT(KTxtVersion,"1.00 (build 06-Nov-2002)");

TPtrC CICLFrameworkTestSuite::GetVersion() const
	{
	return KTxtVersion();
	}

void CICLFrameworkTestSuite::InitialiseL()
	{

	// store the name of this test suite
	iSuiteName = _L("TSU_ICL_FRM_01");

	InitSystemPath();

	AddTestStepL(RTBufPtr8Test::NewL(this)); 
	AddTestStepL(RTImageParameterDataTest::NewL(this));
	AddTestStepL(RTBmpImageDataTest::NewL(this));
	AddTestStepL(RTBmpCompressionTest::NewL(this));
	AddTestStepL(RTJpegImageDataTest::NewL(this));
	AddTestStepL(RTJpegQTableTest::NewL(this));
	AddTestStepL(RTJpegCommentTest::NewL(this));
	AddTestStepL(RTGifBackgroundColorTest::NewL(this));
	AddTestStepL(RTGifImageDescriptorTest::NewL(this));
	AddTestStepL(RTGifColorTableTest::NewL(this));
	AddTestStepL(RTGifLZWInfoTest::NewL(this));
	AddTestStepL(RTGifImageControlTest::NewL(this));
	AddTestStepL(RTGifCommentTest::NewL(this));
	AddTestStepL(RTMbmDecodeDataTest::NewL(this));
	AddTestStepL(RTMbmEncodeDataTest::NewL(this));
	AddTestStepL(RTTiffImageDataTest::NewL(this));
	AddTestStepL(RCImageTypeDescriptionTest::NewL(this));
	AddTestStepL(RCImageTypeDescriptionTest::NewL(this, ETrue));
	AddTestStepL(RCFileExtensionMIMETypeTest::NewL(this));
	AddTestStepL(RCFileExtensionMIMETypeTest::NewL(this, ETrue));
	AddTestStepL(RCImplementationInformationTypeTest::NewL(this));
	AddTestStepL(RCImplementationInformationTypeTest::NewL(this, ETrue));
	AddTestStepL(RCFrameImageDataTest::NewL(this));
	AddTestStepL(RCFrameImageDataTest::NewL(this, ETrue));
	AddTestStepL(RCFrameImageDataAppendImageBufferTest::NewL(this));
	AddTestStepL(RCFrameImageDataAppendImageBufferTest::NewL(this, ETrue));
	AddTestStepL(RCFrameImageDataAllocLTest::NewL(this));
	AddTestStepL(RCFrameImageDataAllocLTest::NewL(this, ETrue));
	AddTestStepL(RCFrameInfoStringsTest::NewL(this));
	AddTestStepL(RCFrameInfoStringsTest::NewL(this, ETrue));
	AddTestStepL(RTFrameInfoTest::NewL(this));
	AddTestStepL(RCImageDecoderPrivTest::NewL(this));
	AddTestStepL(RCImageDecoderPrivTest::NewL(this, ETrue));
	AddTestStepL(RCImageDecoderPrivSetFileLTest::NewL(this));
	AddTestStepL(RCImageDecoderPrivSetDataLTest::NewL(this));
	AddTestStepL(RCImageDecoderPrivHandleNewlyOpenedImageLTest::NewL(this));
	AddTestStepL(RCImageDecoderPrivContinueProcessingHeaderLTest::NewL(this));
	AddTestStepL(RCImageDecoderPrivReadDataLTest::NewL(this, ETrue, EFalse));
	AddTestStepL(RCImageDecoderPrivReadDataLTest::NewL(this, ETrue, ETrue));
	AddTestStepL(RCImageDecoderPrivReadDataLTest::NewL(this, EFalse, EFalse));
	AddTestStepL(RCImageDecoderPrivReadDataLTest::NewL(this, EFalse, ETrue));
	AddTestStepL(RCImageDecoderPrivFrameInfoStringsLCTest::NewL(this));
	AddTestStepL(new (ELeave) RCImageProcessorUtilityTest(_L("MM-ICL-FRM-U-4034-HP")));

	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, ETrue, EFalse, EFalse));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, ETrue, EFalse, ETrue));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, ETrue, ETrue, EFalse));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, ETrue, ETrue, ETrue));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, EFalse, EFalse, EFalse));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, EFalse, EFalse, ETrue));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, EFalse, ETrue, EFalse));
	AddTestStepL(RCImageDecoderPrivDecodeTest::NewL(this, EFalse, ETrue, ETrue));

	AddTestStepL(RCImageDecoderPrivCustomTest::NewL(this, EFalse));

	AddTestStepL(RCImageEncoderPrivEncodeTest::NewL(this, ETrue, EFalse));
	AddTestStepL(RCImageEncoderPrivEncodeTest::NewL(this, ETrue, ETrue));
	AddTestStepL(RCImageEncoderPrivEncodeTest::NewL(this, EFalse, EFalse));
	AddTestStepL(RCImageEncoderPrivEncodeTest::NewL(this, EFalse, ETrue));
	AddTestStepL(RCImageEncoderPrivRAEncodeTest::NewL(this));

	AddTestStepL(RCImageEncoderPrivCustomTest::NewL(this, EFalse));

	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFile, EFalse, EFalse));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFile, EFalse, ETrue));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFile, ETrue, EFalse));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFile, ETrue, ETrue));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentDescriptor, EFalse, EFalse));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentDescriptor, EFalse, ETrue));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentDescriptor, ETrue, EFalse));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentDescriptor, ETrue, ETrue));
	
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFileHandle, EFalse, EFalse));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFileHandle, EFalse, ETrue));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFileHandle, ETrue, EFalse));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFileHandle, ETrue, ETrue));


	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFile, EFalse, EFalse, ETrue));
	AddTestStepL(RCImageDecoderDecodeTest::NewL(this, EContentFileHandle, EFalse, EFalse, ETrue));

	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 0,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 0,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 1,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 1,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 2,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 2,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 3,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 3,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 4,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 4,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 5,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 5,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 6,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 6,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 7,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 7,ETrue));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 8,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 9,EFalse));
	AddTestStepL(RCImageDecoderStaticTest::NewL(this, 10,EFalse));	
    AddTestStepL(RCImageDecoderStaticTest::NewL(this, 11,EFalse));
    AddTestStepL(RCImageDecoderStaticTest::NewL(this, 12,EFalse));
    AddTestStepL(RCImageDecoderStaticTest::NewL(this, 13,EFalse));
    
	AddTestStepL(RThreadedDecodeCancelTest::NewL(this));

	AddTestStepL(RCImageDecoderResolverTest::NewL(this, EContentFile, EFalse));
	AddTestStepL(RCImageDecoderResolverTest::NewL(this, EContentFile, ETrue));
	AddTestStepL(RCImageDecoderResolverTest::NewL(this, EContentDescriptor, EFalse));
	AddTestStepL(RCImageDecoderResolverTest::NewL(this, EContentDescriptor, ETrue));
	AddTestStepL(RCImageDecoderResolverTest::NewL(this, EContentFileHandle, EFalse));
	AddTestStepL(RCImageDecoderResolverTest::NewL(this, EContentFileHandle, ETrue));

	AddTestStepL(RCImageDecoderMaskDecodeTest::NewL(this, EFalse));
	AddTestStepL(RCImageDecoderMaskDecodeTest::NewL(this, ETrue));

	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFile,  EFalse, EFalse));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFile,  EFalse, ETrue));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFile,  ETrue,  EFalse));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFile,  ETrue,  ETrue));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentDescriptor, EFalse, EFalse));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentDescriptor, EFalse, ETrue));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentDescriptor, ETrue,  EFalse));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentDescriptor, ETrue,  ETrue));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFileHandle,  EFalse, EFalse));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFileHandle,  EFalse, ETrue));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFileHandle,  ETrue,  EFalse));
	AddTestStepL(RCImageEncoderEncodeTest::NewL(this, EContentFileHandle,  ETrue,  ETrue));

	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 0, EFalse));
	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 0, ETrue));
	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 1, EFalse));
	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 1, ETrue));
	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 2, EFalse));
	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 2, ETrue));
	AddTestStepL(RCImageEncoderStaticTest::NewL(this, 3, EFalse));

	AddTestStepL(RThreadedEncodeCancelTest::NewL(this));

	AddTestStepL(RCBufferedImageDecoderDecodeTest::NewL(this, EFalse, EFalse));
	AddTestStepL(RCBufferedImageDecoderDecodeTest::NewL(this, EFalse, ETrue));
	AddTestStepL(RCBufferedImageDecoderDecodeTest::NewL(this, ETrue,  EFalse));
	AddTestStepL(RCBufferedImageDecoderDecodeTest::NewL(this, ETrue,  ETrue));

	AddTestStepL(RCBufferedImageDecoderFunctionTest::NewL(this, EFalse));
	AddTestStepL(RCBufferedImageDecoderFunctionTest::NewL(this, ETrue));

	AddTestStepL(RCExtendedDecoderTest::NewL(this, EFalse));
	AddTestStepL(RCExtendedDecoderTest::NewL(this, ETrue));
	AddTestStepL(RCExtendedEncoderTest::NewL(this, EFalse));
	AddTestStepL(RCExtendedEncoderTest::NewL(this, ETrue));

	// DEF DEF110209 CJpegDecoder::HandleCustomSyncL() problem 
	AddTestStepL(RCExtendedJpegDecoderTest::NewL(this, EFalse));
	AddTestStepL(RCExtendedJpegDecoderTest::NewL(this, ETrue));
		
	AddTestStepL(RBadVerDecoderTestStep::NewL(this));
	AddTestStepL(RBadVerEncoderTestStep::NewL(this));

	// DEF039215 - ICL framework cannot handle properly asynchronous conversion operations 
	// Async decoder/encoder tests 

	// Async decoder unit test
	AddTestStepL(RCImageDecoderPrivAsyncDecodeTest::NewL(this));
	// Async decoder plugin normal test
	AddTestStepL(RCImageDecoderAsyncDecodeTest::NewL(this, EFalse, EFalse));
	// Async decoder plugin cancel test
	AddTestStepL(RCImageDecoderAsyncDecodeTest::NewL(this, EFalse, ETrue));
	// Async decoder plugin alloc test
	AddTestStepL(RCImageDecoderAsyncDecodeTest::NewL(this, ETrue, EFalse));

	// Async encoder plugin normal test
	AddTestStepL(RCImageEncoderAsyncEncodeTest::NewL(this, EFalse, EFalse));
	// Async encoder plugin cancel test
	AddTestStepL(RCImageEncoderAsyncEncodeTest::NewL(this, EFalse, ETrue));
	// Async encoder plugin alloc test
	AddTestStepL(RCImageEncoderAsyncEncodeTest::NewL(this, ETrue, EFalse));
	
	//DRM Intent methods
	AddTestStepL(RCImageDecoderIntentTest::NewL(this, EFalse)); // non-mime version
	AddTestStepL(RCImageDecoderIntentTest::NewL(this, ETrue)); // mime version

    // GT 0222 MS3.6, PREQ777 DRM Support
    // Positive Tests

    // MM-ICL-FRM-U-6001-HP, TMMFileSource and MIME type.
    AddTestStepL(RTImageDecDRMViewMthd1::NewL(this));

    // MM-ICL-FRM-U-6002-HP, TMMFileSource and MIME type.
    AddTestStepL(RTImageDecDRMPrintMthd1::NewL(this));

    // MM-ICL-FRM-U-6003-HP, TMMFileSource and MIME type.
    AddTestStepL(RTImageDecDRMViewAllocMthd1::NewL(this));

    // MM-ICL-FRM-U-6004-HP, All defaults. TMMFileHandleSource.
    AddTestStepL(RTImageDecDRMViewMthd2::NewL(this));

    // MM-ICL-FRM-U-6005-HP, Image type Uid. TMMFileSource.
    AddTestStepL(RTImageDecDRMViewImgTypeMthd2::NewL(this));

    // MM-ICL-FRM-U-6006-HP, Decoder Uid. TMMFileSource.
    AddTestStepL(RTImageDecDRMViewDecTypeMthd2::NewL(this));

    // MM-ICL-FRM-U-6007-HP, All defaults. TMMFileHandleSource.
    AddTestStepL(RTImageDecDRMViewAllocMthd2::NewL(this));

    // MM-ICL-FRM-U-6008-HP, TMMFileSource and MIME type.
    AddTestStepL(RTImageDecDRMViewUniqueIdMthd1::NewL(this));

    // Negative Tests

    // MM-ICL-FRM-U-7001-HP, TMMFileSource and MIME type
    AddTestStepL(RTImageDecOpenDRMNoRightsMthd1::NewL(this));

    // MM-ICL-FRM-U-7002-HP, TMMFileSource and MIME type
    AddTestStepL(RTImageDecOpenDRMUnknownMthd1::NewL(this));

    // MM-ICL-FRM-U-7003-HP, TMMFileSource and MIME type
    AddTestStepL(RTImageDecOpenDRMRightsCountMthd1::NewL(this));

    // MM-ICL-FRM-U-7004-HP, All defaults. TMMFileHandleSource
    AddTestStepL(RTImageDecOpenDRMNoRightsMthd2::NewL(this));

    // MM-ICL-FRM-U-7005-HP, All defaults. TMMFileHandleSource
    AddTestStepL(RTImageDecOpenDRMUnknownMthd2::NewL(this));

    // MM-ICL-FRM-U-7006-HP, All defaults. TMMFileHandleSource
    AddTestStepL(RTImageDecOpenDRMRightsCountMthd2::NewL(this));

    // MM-ICL-FRM-U-7007-HP, TMMFileSource and MIME type
    AddTestStepL(RTImageDecDRMViewInvalIdMthd1::NewL(this));
    // MM-ICL-FRM-U-7008-HP, Opening via CAF ,
    AddTestStepL(RTImageDecDRMViaCAF::NewL(this)); 
    
  // MM-ICL-FRM-U-7020-HP INC061633: NTT - CImageDecoder: KNullDesC should be KDefaultContentObject
    AddTestStepL(RTImageDecDRMDefaContentTest::NewL(this));
    
    // MM-ICL-FRM-U-7015-HP 
    AddTestStepL(RTImageOpenOTB::NewL(this));

		// MM-ICL-FRM-U-7021-HP  DEF079163
	AddTestStepL(RTImageMimeTypeTest::NewL(this));    

   // MM-ICL-FRM-U-7025-HP to MM-ICL-FRM-U-7026-HP - Negative testing of CImageDecoder::GetPluginPropertiesL() and CImageDecoder::GetInterfaceImplementationsL()
    AddTestStepL(RCImageDecoderStaticTest::NewL(this, 14,EFalse));
    AddTestStepL(RCImageDecoderStaticTest::NewL(this, 15,EFalse));
    
    // MM-ICL-FRM-U-7027-HP to MM-ICL-FRM-U-7028-HP - Negative testing of Negative testing of CImageDecoder::MimeTypeGetDecoderListL() and CImageEncoder::MimeTypeGetEncoderListL()
    AddTestStepL(RDecoderInvalidMimeTypeArg::NewL(this));
    AddTestStepL(REncoderInvalidMimeTypeArg::NewL(this));
    
    // MM-ICL-FRM-U-7022-HP CR0711
    AddTestStepL(RSetThreadPriorityTestStep::NewL(this));
    
    // MM-ICL-FRM-U-7035-HP CR0793
    AddTestStepL(REmulateSlowMediaTestStep::NewL(this));

    // MM-ICL-FRM-U-7106-HP DEF088101
    AddTestStepL(RTImageDecOpenDRMConsumeRights::NewL(this));
    
	// MM-ICL-FRM-U-7107-HP DEF090885
	AddTestStepL(RTImageDecOpenDRMConsumeRightsOnError::NewL(this));
	
	// MM-ICL-FRM-U-7108-HP DEF090885
	AddTestStepL(RTImageDecOpenDRMConsumeRightsOnError2::NewL(this));
	
	// MM-ICL-FRM-U-7109-HP DEF090885
	AddTestStepL(RTImageDecOpenDRMConsumeRightsOnError3::NewL(this));
	
    // PREQ1630: Call CImageDecoder OperationL() on the PNG plugin in client thread
	AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
												EFalse,
												RFwExtPNGDecNotSupportedTest::EOperationL,
    											(_L("MM-ICL-FRM-U-8001-HP"))));
	

	// PREQ1630: Call CImageDecoder OperationL() on the PNG plugin in separate thread
    AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
    											ETrue,
    											RFwExtPNGDecNotSupportedTest::EOperationL,
    											(_L("MM-ICL-FRM-U-8002-HP"))));
    
    // PREQ1630: Call CImageDecoder ScalerL() on the PNG plugin in client thread
    AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
    											EFalse,
    											RFwExtPNGDecNotSupportedTest::EScalerL,
    											(_L("MM-ICL-FRM-U-8003-HP"))));
    
    // PREQ1630: Call CImageDecoder ScalerL() on the PNG plugin in separate thread
    AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
    											ETrue,
    											RFwExtPNGDecNotSupportedTest::EScalerL,
    											(_L("MM-ICL-FRM-U-8004-HP"))));
    
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the PNG plugin in client thread
    AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
    											EFalse,
    											RFwExtPNGDecNotSupportedTest::EBlockStreamerL,
    											(_L("MM-ICL-FRM-U-8005-HP"))));
    
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the PNG plugin in separate thread
    AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
    											ETrue,
    											RFwExtPNGDecNotSupportedTest::EBlockStreamerL,
    											(_L("MM-ICL-FRM-U-8006-HP"))));
    											
    // PREQ1630: Call CImageDecoder GetDestinationSize() on the PNG plugin in client thread
	AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
												EFalse,
												RFwExtPNGDecNotSupportedTest::EGetDestinationSize,
    											(_L("MM-ICL-FRM-U-8007-HP"))));											
   	
   	// PREQ1630: Call CImageDecoder GetDestinationSize() on the PNG plugin in separate thread
	AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
												ETrue,
												RFwExtPNGDecNotSupportedTest::EGetDestinationSize,
    											(_L("MM-ICL-FRM-U-8008-HP"))));
    
    // PREQ1630: Call CImageDecoder SetClippingRectL() on the PNG plugin in client thread
	AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
												EFalse,
												RFwExtPNGDecNotSupportedTest::ESetClippingRectL,
    											(_L("MM-ICL-FRM-U-8009-HP"))));
    
    // PREQ1630: Call CImageDecoder SetClippingRectL() on the PNG plugin in separate thread
	AddTestStepL(RFwExtPNGDecNotSupportedTest::NewL(this,
												ETrue,
												RFwExtPNGDecNotSupportedTest::ESetClippingRectL,
    											(_L("MM-ICL-FRM-U-8010-HP"))));
    
    // PREQ1630: Call CImageDecoder OperationL() on the FwExtTest plugin in client thread.
	// Call all functions on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EOperationL,
    										(_L("MM-ICL-FRM-U-8011-HP"))));
    
    // PREQ1630: Call CImageDecoder OperationL() on the FwExtTest plugin in separate thread.
	// Call all functions on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EOperationL,
    										(_L("MM-ICL-FRM-U-8012-HP"))));																																																						
   	
   	// PREQ1630: Call CImageDecoder ScalerL() on the FwExtTest plugin in client thread.
	// Call all functions on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EScalerL,
    										(_L("MM-ICL-FRM-U-8013-HP"))));
    
    // PREQ1630: Call CImageDecoder ScalerL() on the FwExtTest plugin in separate thread.
	// Call all functions on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EScalerL,
    										(_L("MM-ICL-FRM-U-8014-HP"))));
    
   	// PREQ1630: Call CImageDecoder BlockStreamerL() on the FwExtTest plugin in client thread.
	// Call all synchronous functions on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EBlockStreamerLSetup,
    										(_L("MM-ICL-FRM-U-8015-HP"))));
    
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the FwExtTest plugin in separate thread.
	// Call all synchronous functions on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EBlockStreamerLSetup,
    										(_L("MM-ICL-FRM-U-8016-HP"))));
   
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the FwExtTest plugin in client thread.
	// Call asynchronous GetBlocks() function on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EBlockStreamerLGetBlocks,
    										(_L("MM-ICL-FRM-U-8017-HP"))));
    
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the FwExtTest plugin in separate thread.
	// Call asynchronous GetBlocks() function on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EBlockStreamerLGetBlocks,
    										(_L("MM-ICL-FRM-U-8018-HP"))));	
    										
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the FwExtTest plugin in client thread.
	// Call asynchronous GetNextBlocks() function on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EBlockStreamerLGetNextBlocks,
    										(_L("MM-ICL-FRM-U-8019-HP"))));
    
    // PREQ1630: Call CImageDecoder BlockStreamerL() on the FwExtTest plugin in separate thread.
	// Call asynchronous GetNextBlocks() function on the extension.
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EBlockStreamerLGetNextBlocks,
    										(_L("MM-ICL-FRM-U-8020-HP"))));
    				
    // PREQ1630: Call CImageDecoder GetDestinationSize() on the FwExtTest plugin in client thread
	AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EGetDestinationSize,
    										(_L("MM-ICL-FRM-U-8021-HP"))));											
   	
   	// PREQ1630: Call CImageDecoder GetDestinationSize() on the FwExtTest plugin in separate thread
	AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EGetDestinationSize,
    										(_L("MM-ICL-FRM-U-8022-HP"))));
    
    // PREQ1630: Call CImageDecoder SetClippingRectL() on the FwExtTest plugin in client thread
	AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::ESetClippingRectL,
    										(_L("MM-ICL-FRM-U-8023-HP"))));
    
    // PREQ1630: Call CImageDecoder SetClippingRectL() on the FwExtTest plugin in separate thread
	AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::ESetClippingRectL,
    										(_L("MM-ICL-FRM-U-8024-HP"))));
	
		
    // PREQ1630: Call CImageEncoder OperationL() on the PNG plugin in client thread
	AddTestStepL(RFwExtPNGEncNotSupportedTest::NewL(this,
												EFalse,
												RFwExtPNGEncNotSupportedTest::EOperationL,
    											(_L("MM-ICL-FRM-U-8025-HP"))));
	

	// PREQ1630: Call CImageEncoder OperationL() on the PNG plugin in separate thread
    AddTestStepL(RFwExtPNGEncNotSupportedTest::NewL(this,
    											ETrue,
    											RFwExtPNGEncNotSupportedTest::EOperationL,
    											(_L("MM-ICL-FRM-U-8026-HP"))));																		
   	
   	// PREQ1630: Call CImageEncoder BlockStreamerL() on the PNG plugin in client thread
   	AddTestStepL(RFwExtPNGEncNotSupportedTest::NewL(this,
											EFalse,
											RFwExtPNGEncNotSupportedTest::EBlockStreamerL,
    										(_L("MM-ICL-FRM-U-8027-HP"))));
    
    // PREQ1630: Call CImageEncoder BlockStreamerL() on the PNG plugin in separate thread
    AddTestStepL(RFwExtPNGEncNotSupportedTest::NewL(this,
											ETrue,
											RFwExtPNGEncNotSupportedTest::EBlockStreamerL,
    										(_L("MM-ICL-FRM-U-8028-HP"))));	
    										
      // PREQ1630: Call CImageEncoder OperationL() on the FwExtTest plugin in client thread
	AddTestStepL(RFwExtEncSupportedTest::NewL(this,
												EFalse,
												RFwExtEncSupportedTest::EOperationL,
    											(_L("MM-ICL-FRM-U-8029-HP"))));
	

	// PREQ1630: Call CImageEncoder OperationL() on the FwExtTest plugin in separate thread
    AddTestStepL(RFwExtEncSupportedTest::NewL(this,
    											ETrue,
    											RFwExtEncSupportedTest::EOperationL,
    											(_L("MM-ICL-FRM-U-8030-HP"))));																		
   	
   	// PREQ1630: Call CImageEncoder BlockStreamerL() on the FwExtTest plugin in client thread
   	// Test setup calls for encode streamer
   	AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											EFalse,
											RFwExtEncSupportedTest::EBlockStreamerLSetup,
    										(_L("MM-ICL-FRM-U-8031-HP"))));
    
    // PREQ1630: Call CImageEncoder BlockStreamerL() on the FwExtTest plugin in separate thread
    // Test setup calls for encode streamer
    AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											ETrue,
											RFwExtEncSupportedTest::EBlockStreamerLSetup,
    										(_L("MM-ICL-FRM-U-8032-HP"))));
    										
    // PREQ1630: Call CImageEncoder BlockStreamerL() on the FwExtTest plugin in client thread
    // Test async call AppendBlocks()
   	AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											EFalse,
											RFwExtEncSupportedTest::EBlockStreamerLAppendBlocks,
    										(_L("MM-ICL-FRM-U-8033-HP"))));
    
    // PREQ1630: Call CImageEncoder BlockStreamerL() on the FwExtTest plugin in separate thread
    // Test async call AppendBlocks()
    AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											ETrue,
											RFwExtEncSupportedTest::EBlockStreamerLAppendBlocks,
    										(_L("MM-ICL-FRM-U-8034-HP"))));
    										
    // PREQ1630: Call CImageEncoder BlockStreamerL() on the FwExtTest plugin in client thread
    // Test async call AddBlocks()
   	AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											EFalse,
											RFwExtEncSupportedTest::EBlockStreamerLAddBlocks,
    										(_L("MM-ICL-FRM-U-8035-HP"))));
    
    // PREQ1630: Call CImageEncoder BlockStreamerL() on the FwExtTest plugin in separate thread
    // Test async call AddBlocks()
    AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											ETrue,
											RFwExtEncSupportedTest::EBlockStreamerLAddBlocks,
    										(_L("MM-ICL-FRM-U-8036-HP"))));													

    // PREQ1630: Call asynch CImageDecoder Prepare() on the FwExtTest plugin in client thread
   	AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											EFalse,
											RFwExtDecSupportedTest::EPrepare,
    										(_L("MM-ICL-FRM-U-8037-HP"))));
    
    // PREQ1630: Call asynch CImageDecoder Prepare() on the FwExtTest plugin in separate thread
    AddTestStepL(RFwExtDecSupportedTest::NewL(this,
											ETrue,
											RFwExtDecSupportedTest::EPrepare,
    										(_L("MM-ICL-FRM-U-8038-HP"))));
    										
    // PREQ1630: Call asynch CImageEncoder Prepare() on the FwExtTest plugin in client thread
   	AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											EFalse,
											RFwExtEncSupportedTest::EPrepare,
    										(_L("MM-ICL-FRM-U-8039-HP"))));
    
    // PREQ1630: Call asynch CImageEncoder Prepare() on the FwExtTest plugin in separate thread
    AddTestStepL(RFwExtEncSupportedTest::NewL(this,
											ETrue,
											RFwExtEncSupportedTest::EPrepare,
    										(_L("MM-ICL-FRM-U-8040-HP"))));
    										
    // PREQ1630: Unit tests for TPosition Processor Rotate Transform
	AddTestStepL(RTPositionProcessorTest::NewL(this,
												RTPositionProcessorTest::ETransformRotate,
    											(_L("MM-ICL-FRM-U-9001-HP"))));
    
    // PREQ1630: Unit tests for TPosition Processor Vertical Flip Transform
	AddTestStepL(RTPositionProcessorTest::NewL(this,
												RTPositionProcessorTest::ETransformVerticalFlip,
    											(_L("MM-ICL-FRM-U-9002-HP"))));
    
    // PREQ1630: Unit tests for TPosition Processor Horizontal Flip Transform
	AddTestStepL(RTPositionProcessorTest::NewL(this,
												RTPositionProcessorTest::ETransformHorizontalFlip,
    											(_L("MM-ICL-FRM-U-9003-HP"))));
	
    
    // PREQ1630: Unit tests for TPosition Processor Rotate vertical & Horizontal Flip Transform
	AddTestStepL(RTPositionProcessorTest::NewL(this,
												RTPositionProcessorTest::ETransformRotVerHorFlip,
    											(_L("MM-ICL-FRM-U-9004-HP"))));
    											
	// DEF119087: After call CImageEncoder::Cancel, uncompleted file still remains on disk
	AddTestStepL(RThreadedFileEncodeCancelAfterConvertTest::NewL(this));
	AddTestStepL(RThreadedFileHandleEncodeCancelAfterConvertTest::NewL(this));
	AddTestStepL(RThreadedFileEncodeDeleteTest::NewL(this));
	AddTestStepL(RThreadedFileHandleEncodeDeleteTest::NewL(this));
   	}

void CICLFrameworkTestSuite::InitSystemPath()
	{
	_LIT(KTSU_ICL_FRM_01PathOnE,"e:\\TSU_ICL_FRM_01\\");
	_LIT(KTSU_ICL_FRM_01PathOnD,"d:\\TSU_ICL_FRM_01\\");
	_LIT(KTSU_ICL_FRM_01PathOnC,"c:\\TSU_ICL_FRM_01\\");
	_LIT(KTSU_ICL_FRM_01PathOnZ,"z:\\TSU_ICL_FRM_01\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KTSU_ICL_FRM_01PathOnE,att) == KErrNone)
		iDefaultPath = KTSU_ICL_FRM_01PathOnE;
	else if (fs.Att(KTSU_ICL_FRM_01PathOnD,att) == KErrNone)
		iDefaultPath = KTSU_ICL_FRM_01PathOnD;
	else if (fs.Att(KTSU_ICL_FRM_01PathOnC,att) == KErrNone)
		iDefaultPath = KTSU_ICL_FRM_01PathOnC;
	else if (fs.Att(KTSU_ICL_FRM_01PathOnZ,att) == KErrNone)
		iDefaultPath = KTSU_ICL_FRM_01PathOnZ;
	else
		User::Panic(_L("Test files not found"),KErrNotFound);
	
	fs.Close();
	}
	
const TDesC& CICLFrameworkTestSuite::DefaultPath() const
	{
	return iDefaultPath;
	}
