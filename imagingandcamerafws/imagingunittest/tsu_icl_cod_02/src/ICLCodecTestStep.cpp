
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
// ICL Codec Tests - see "V:\Dev\GT\GT 0137 Multimedia codec - Msrvr\Testing\Unit Test Specifications\Typhoon\"
// "SGL.GT0137.xxx Rev0.01 MMF ICL COD Unit Test Specification.xls"
// 
//

#include "ICLCodecTestStep.h"
#include "PngRefScanlines.h"
#include "ICLCodecTestSuite.h"
#include "ICLFbsSessionTest.h"

// Test system includes
#include <imageconversion.h>

#include <ecom/ecom.h>

//from  imagingandcamerafws/imagingfws/src/Recognizer/RecIcl.h
#include "RecIcl.h"

class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {}
	};

// Added since we implement MApaAppStarter, which has been future-proofed.
EXPORT_C MApaAppStarter::MApaAppStarter()
	{
	}

EXPORT_C void MApaAppStarter::MApaAppStarter_Reserved1()
	{
	}
	
EXPORT_C void MApaAppStarter::MApaAppStarter_Reserved2()
	{
	}	

// table of tests

//const TInt KDefaultHeapSize=16*1024*1024; // 16M - what we used for TImage	
const TInt KBufferSize = 40;

/**
 * Set the test step result
 * @param	"TVerdict aVerdict"
 *			Set the test step result to aVerdict if it is worse than the current result
 */
void RTestStepCompare::SetVerdict(TVerdict aVerdict)
	{
	if(aVerdict>iTestStepResult)
		iTestStepResult = aVerdict;
	}

/**
 * Compare two integers and set the test step result to EFail if they do not match
 * @param	"TInt aActual"
 *			The value to test
 * @param	"TInt aExpect"
 *			What aActual is expected to be
 * @param	"const TDesC& aErrorMessage"
 *			An error message if the values do not match.
 */
void RTestStepCompare::Compare(TInt aActual, TInt aExpect, const TDesC& aErrorMessage)
	{
	if(aActual!=aExpect)
		{
		ERR_PRINTF1(aErrorMessage);
		ERR_PRINTF3(_L("Value is:%d Expected:%d"),aActual,aExpect);
		SetVerdict(EFail);
		}
	}

/**
 * Compare two UID's and set the test step result to EFail if they do not match
 * @param	"TUid aActual"
 *			The value to test
 * @param	"TUid aExpect"
 *			What aActual is expected to be
 * @param	"const TDesC& aErrorMessage"
 *			An error message if the values do not match.
 */
void RTestStepCompare::Compare(TUid aActual, TUid aExpect, const TDesC& aErrorMessage)
	{
	if(aActual!=aExpect)
		{
		ERR_PRINTF1(aErrorMessage);
		ERR_PRINTF3(_L("Value is:%X Expected:%X"),aActual.iUid,aExpect.iUid);
		SetVerdict(EFail);
		}
	}

/**
 * Compare two descriptors and set the test step result to EFail if they do not match
 * @param	"const TDesC& aActual"
 *			The descriptor to test
 * @param	"const TDesC& aExpect"
 *			What aActual is expected to be
 * @param	"const TDesC& aErrorMessage"
 *			An error message if the values do not match.
 */
void RTestStepCompare::Compare(const TDesC& aActual, const TDesC& aExpect, const TDesC& aErrorMessage)
	{
	if(aActual.Ptr()==aExpect.Ptr())
		{
		ERR_PRINTF1(_L("Comparing same pointer!"));
		SetVerdict(EFail);
		}

	if(aActual!=aExpect)
		{
		ERR_PRINTF1(aErrorMessage);
		ERR_PRINTF3(_L("Value is:[%S] Expected:[%S]"),&aActual,&aExpect);
		SetVerdict(EFail);
		}
	}

/**
 * Compare two descriptors and set the test step result to EFail if they do not match
 * @param	"const TDesC8& aActual"
 *			The descriptor to test
 * @param	"const TDesC8& aExpect"
 *			What aActual is expected to be
 * @param	"const TDesC& aErrorMessage"
 *			An error message if the values do not match.
 */
void RTestStepCompare::Compare(const TDesC8& aActual, const TDesC8& aExpect, const TDesC& aErrorMessage)
	{
	if(aActual.Ptr()==aExpect.Ptr())
		{
		ERR_PRINTF1(_L("Comparing same pointer!"));
		SetVerdict(EFail);
		}

	if(aActual!=aExpect)
		{
		TInt err = KErrNone;
		TInt err2 = KErrNone;
		ERR_PRINTF1(aErrorMessage);

		HBufC* actual = NULL;
		TRAP(err, actual = HBufC::NewL(aActual.Length()));
		
		HBufC* expect = NULL;
		TRAP(err2, expect = HBufC::NewL(aExpect.Length()));

		//if we could not allocate memory simply ignore
		if((err == KErrNone) && (err2 == KErrNone))
			{
			actual->Des().Copy(aActual);
			expect->Des().Copy(aExpect);
			ERR_PRINTF3(_L("Value is:[%S] Expected:[%S]"),actual,expect);
			}
		delete actual; actual = NULL;
		delete expect; expect = NULL;

		SetVerdict(EFail);
		}
	}
	
// dummy Hal call for heap balance on target
// Need to do this BEFORE doing any allocation testing !	
void RTestStepCompare::DummyHalCallL()
	{
	TInt err = KErrNone;
	TInt displayMode = EColor256;
	CFbsScreenDevice* screenDevice = NULL;	
	while(displayMode < EColorLast) 
		{
		TRAP(err, screenDevice = CFbsScreenDevice::NewL(_L("NotUsed"),(TDisplayMode)displayMode));
		delete screenDevice;
		screenDevice = NULL;	
		if(err == KErrNone)
			{
			break;
			}
		displayMode++;
		}			
	User::LeaveIfError(err);
	}	

//
// RPngWriteCodecTest
RPngWriteCodecTest* RPngWriteCodecTest::NewL(const TDesC& aTestName, TPngWriteCodecTestId aPngTestId,
											 const TInt aHeapSize /* = 0 */)
	{
	RPngWriteCodecTest* self = new(ELeave) RPngWriteCodecTest(aTestName, aPngTestId, aHeapSize);
	return self;
	}

RPngWriteCodecTest::RPngWriteCodecTest(const TDesC& aTestName, TPngWriteCodecTestId aPngTestId, const TInt aHeapSize /* = 0 */)
	: iPngTestId(aPngTestId)
	{
	iTestStepName = aTestName;

	if (aHeapSize)
		iHeapSize = aHeapSize;
	}

TVerdict RPngWriteCodecTest::OpenL()
	{
	TVerdict verdict = EPass;

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
        User::LeaveIfError(errCode);
        }
	return verdict;
	}

void RPngWriteCodecTest::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	}

TVerdict RPngWriteCodecTest::DoTestStepL()
	{
     User::LeaveIfError(FbsStartup());
     TInt errCode = RFbsSession::Connect();
     if( errCode != KErrNone)
         {
         User::LeaveIfError(errCode);
         }
	switch (iPngTestId)
		{
		case ECOD_0201:
			return MM_ICL_COD_U_0201_L();
		case ECOD_0202:
			return MM_ICL_COD_U_0202_L();
		case ECOD_0203:
			return MM_ICL_COD_U_0203_L();
		case ECOD_0204:
			return MM_ICL_COD_U_0204_L();
		case ECOD_0205:
			return MM_ICL_COD_U_0205_L();
// ECOD_0206 not used
//		case ECOD_0206:
//			return MM_ICL_COD_U_0206_L();
		case ECOD_0207:
			return MM_ICL_COD_U_0207_L();
		case ECOD_0208:
			return MM_ICL_COD_U_0208_L();
		case ECOD_0209:
			return MM_ICL_COD_U_0209_L();
		case ECOD_0210:
			return MM_ICL_COD_U_0210_L();
		case ECOD_0211:
			return MM_ICL_COD_U_0211_L();
		case ECOD_0212:
			return MM_ICL_COD_U_0212_L();
		// Negative tests
		case ECOD_0401:
			return MM_ICL_COD_U_0401_L();
		case ECOD_0402:
			return MM_ICL_COD_U_0402_L();
		case ECOD_0403:
			return MM_ICL_COD_U_0403_L();
		case ECOD_0404:
			return MM_ICL_COD_U_0404_L();
		case ECOD_0501:
			return MM_ICL_COD_U_0501_L();
		case ECOD_0502:
			return MM_ICL_COD_U_0502_L();
		case ECOD_0503:
			return MM_ICL_COD_U_0503_L();
		default:
			break;
		}
	ERR_PRINTF1(_L("Invalid test ID"));
	return EFail;
	} 

// Constructor tests
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0201_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder,1, ETrue, ETrue, TPngEncodeData::EDefaultCompression);

	if ((png->iCompressionLevel != TPngEncodeData::EDefaultCompression) &&
		(png->iImageInfo.iBitsPerPixel != 1) &&
		(png->iImageInfo.iBitDepth != 1) &&
		(png->iImageInfo.iColorType != TPngImageInformation::EIndexedColor))
		{
		ERR_PRINTF1(_L("CPngWriteCodec constructor error"));
		verdict = EFail;
		}
		
	delete png;
	png = NULL;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0202_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 2, ETrue, EFalse, TPngEncodeData::EDefaultCompression);

	if ((png->iCompressionLevel != TPngEncodeData::EDefaultCompression) &&
		(png->iImageInfo.iBitsPerPixel != 2) &&
		(png->iImageInfo.iBitDepth != 2) &&
		(png->iImageInfo.iColorType != TPngImageInformation::EDirectColor))
		{
		ERR_PRINTF1(_L("CPngWriteCodec constructor error"));
		verdict = EFail;
		}
	delete png;
	png = NULL;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0203_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 4, EFalse, EFalse, TPngEncodeData::EDefaultCompression);

	if ((png->iCompressionLevel != TPngEncodeData::EDefaultCompression) &&
		(png->iImageInfo.iBitsPerPixel != 4) &&
		(png->iImageInfo.iBitDepth != 4) &&
		(png->iImageInfo.iColorType != TPngImageInformation::EGrayscale))
		{
		ERR_PRINTF1(_L("CPngWriteCodec constructor error"));
		verdict = EFail;
		}
	delete png;
	png = NULL;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0204_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, EFalse, EFalse, TPngEncodeData::EDefaultCompression);

	if ((png->iCompressionLevel != TPngEncodeData::EDefaultCompression) &&
		(png->iImageInfo.iBitsPerPixel != 8) &&
		(png->iImageInfo.iBitDepth != 8) &&
		(png->iImageInfo.iColorType != TPngImageInformation::EGrayscale))
		{
		ERR_PRINTF1(_L("CPngWriteCodec constructor error"));
		verdict = EFail;
		}
	delete png;
	png = NULL;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

// InitFrameL tests
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0205_L()
	{
	__MM_HEAP_MARK;	  
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, EFalse, EFalse, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	HBufC8* buffer = HBufC8::NewMaxLC(4096);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(TSize(32, 48), EColor16M);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}

	if (verdict != EFail)
		{
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());

		// Call twice
		for (TInt i=0; (i < 2) && (verdict == EPass); i++)
			{
			png->InitFrameL(bufptr, *bitmap);

			if (png->Source() != bitmap)
				{
				ERR_PRINTF1(_L("iSource set incorrectly"));
				if (i==1)
					ERR_PRINTF1(_L("Second iteration"));
				verdict = EFail;
				}
			else
				{
				if ((png->iDestStartPtr != bufptr.Ptr()) ||
					(png->iDestPtrLimit != bufptr.Ptr() + bufptr.MaxLength()))
					{
					ERR_PRINTF1(_L("Dest ptrs set incorrectly"));
					if (i==1)
						ERR_PRINTF1(_L("Second iteration"));
					verdict = EFail;
					}
				else
					{
					if ((png->iEncoder == NULL) ||
						(png->iCompressor == NULL))
						{
						ERR_PRINTF1(_L("Encoder or compressor are NULL"));
						if (i==1)
							ERR_PRINTF1(_L("Second iteration"));
						verdict = EFail;
						}
					else
						{
						if ((png->iEncoderState != CPngWriteCodec::EPngInit) ||
							(!png->iCallAgain))
							{
							ERR_PRINTF1(_L("Initial state incorrect"));
							if (i==1)
								ERR_PRINTF1(_L("Second iteration"));
							verdict = EFail;
							}
						}
					}
				}
			}
		}

	CleanupStack::PopAndDestroy(5, fbsSession); //bitmap, buffer, png, encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

/* Not used
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0206_L()
	{
	__MM_HEAP_MARK;
	User::LeaveIfError(RFbsSession::Connect());
	CleanupStack::PushL(TCleanupItem(TCleanupOperation(&RFbsSession::Disconnect)));
	
	CleanupStack::PopAndDestroy(); // RFbsSession::Disconnect()
	__MM_HEAP_MARKEND;
	return EPass;
	}
*/

// CRC function tests
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0207_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 24, ETrue, ETrue, TPngEncodeData::EDefaultCompression);

	png->CalcCrcTable();
	if (!png->iCrcTableCalculated)
		{
		ERR_PRINTF1(_L("CRC table calculated flag"));
		verdict = EFail;
		}
	else
		{
		for (TInt i=0; i < KPngCrcTableLength; i++)
			{
			if ((i != 0) &&
				(png->iCrcTable[i] == 0))
				{
				ERR_PRINTF1(_L("CRC value 0"));
				verdict = EFail;
				break;
				}
			}
		}

	delete png;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0208_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);
	
	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 24, ETrue, ETrue, TPngEncodeData::EDefaultCompression);

	TUint32 crc = 0;
	_LIT8(KCrcTestData, "ABCDE");
	png->GetCrc(crc, KCrcTestData().Ptr(), 5);

	if (crc != 0xb4f1edc8)
		{
		ERR_PRINTF1(_L("Bad CRC result"));
		verdict = EFail;
		}

	delete png;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

// WritePngChunk test
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0209_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 24, ETrue, ETrue, TPngEncodeData::EDefaultCompression);

	_LIT8(KTestChunkId, "ABCD");
	_LIT8(KTestData, "1234567890!*£$");

	TInt length = 0;
	TBuf8<128> destBuf;
	TUint8* ptr = const_cast<TUint8*>(destBuf.Ptr());
	png->WritePngChunk(ptr, KTestChunkId, KTestData, length);

	if (length != (TPtrC8(KTestData).Length() + 12))	// 12 == KPngChunkLengthSize + KPngChunkIdSize + KPngChunkCRCSize
		{
		ERR_PRINTF1(_L("WritePngChunk: Length returned incorrect"));
		verdict = EFail;
		}
	else
		{
		static const TUint8 expected[] = { 0x0, 0x0, 0x0, 0x0e, 0x41, 0x42, 0x43, 0x44, 0x31, 0x32, 0x33, \
									0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x21, 0x2a, 0xa3, \
									0x24, 0x87, 0x2b, 0x2e, 0x25 };	// length + chunk id + KTestData + CRC
		TPtrC8 expectedPtr(expected, length);
		destBuf.SetLength(length);
		if (destBuf.Compare(expectedPtr) != 0)
			{
			ERR_PRINTF1(_L("Unexpected output from WritePngChunk"));
			verdict = EFail;
			}
		}

	delete png;

	CleanupStack::PopAndDestroy(2, fbsSession); //encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

// WriteHeaderChunk test
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0210_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 24, ETrue, ETrue, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(TSize(32,40), EColor16M);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}
	if (verdict != EFail)
		{
		HBufC8* buffer = HBufC8::NewMaxLC(128);
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());
		png->InitFrameL(bufptr, *bitmap);

		png->WriteHeaderChunk(bufptr);

		// Check buffer length
		// 33 = KPngFileSignatureLength + KPngChunkLengthSize + KPngChunkIdSize + KPngIHDRChunkSize + KPngChunkCRCSize
		if (bufptr.Length() != 33)
			{
			ERR_PRINTF1(_L("Header buffer length incorrect"));
			verdict = EFail;
			}
		else
			{
			// Compare signature
			const TUint8 sig[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
			if (bufptr.Left(KPngFileSignatureLength).Compare(TPtrC8(sig, 8)) != 0)
				{
				ERR_PRINTF1(_L("Signature not as expected"));
				verdict = EFail;
				}
			else
				{
				// Compare header with expected data
				static const TUint8 header[] = { 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, \
										  0x20, 0x00, 0x00, 0x00, 0x28, 0x08, 0x03, 0x00, 0x00, 0x00, 0xA8, \
										  0xF7, 0x08, 0xAB };
				if (bufptr.Mid(KPngFileSignatureLength).Compare(TPtrC8(header, 25)) != 0)
					{
					ERR_PRINTF1(_L("Header not as expected"));
					verdict = EFail;
					}
				}
			}
		CleanupStack::PopAndDestroy(buffer);
		}
	
	CleanupStack::PopAndDestroy(4,fbsSession);//bitmap, png, encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0211_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 24, ETrue, ETrue, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	HBufC8* buffer = HBufC8::NewLC(128);
	TBufPtr8 bufptr;
	bufptr.Set(buffer->Des());
	// iDestPtr must be set for WriteEndChunk to work
	png->iDestPtr = const_cast<TUint8*>(bufptr.Ptr());

	png->WriteEndChunk(bufptr);

	// Check buffer length
	// 12 = KPngChunkLengthSize + KPngChunkIdSize + KPngChunkCRCSize
	if (bufptr.Length() != 12)
		{
		ERR_PRINTF1(_L("End chunk buffer length incorrect"));
		verdict = EFail;
		}
	else
		{
		// Check buffer contents is as expected
		const TUint8 end[] = { 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82 };
		if (bufptr.Compare(TPtrC8(end, 12)) != 0)
			{
			ERR_PRINTF1(_L("End chunk not as expected"));
			verdict = EFail;
			}
		}

	CleanupStack::PopAndDestroy(4,fbsSession);//bitmap, png, encoder, fbsSession
	__MM_HEAP_MARKEND;
	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0212_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, ETrue, ETrue, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(TSize(32,40), EColor16M);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}
	if (verdict != EFail)
		{
		HBufC8* buffer = HBufC8::NewMaxLC(1024);
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());
		png->InitFrameL(bufptr, *bitmap);
		// Header chunk will have been written by calling InitFrameL, so reset buffer pointers
		bufptr.SetLength(0);
		png->iDestPtr = const_cast<TUint8*>(bufptr.Ptr());

		png->WritePLTEChunk(bufptr);

		CPalette* palette = png->iEncoder->Palette();
		// Check length is as expected
		if (bufptr.Length() != (KPngChunkLengthSize + KPngChunkIdSize + KPngChunkCRCSize + 
								(palette->Entries() * 3)))
			{
			ERR_PRINTF1(_L("Palette chunk length incorrect"));
			verdict = EFail;
			}
		else
			{
			const TUint8 palHead[] = { 0x00, 0x00, 0x03, 0x00, 0x50, 0x4C, 0x54, 0x45 };
			// Check chunk length and chunk ID
			if (bufptr.Left(8).Compare(TPtrC8(palHead, 8)) != 0)
				{
				ERR_PRINTF1(_L("Palette chunk length and/or chunk ID not as expected"));
				verdict = EFail;
				}
			else
				{
				// Check the RGB bytes against each palette entry
				TUint8* ptr = const_cast<TUint8*>(bufptr.Ptr()) + 8;
				const TInt count = palette->Entries();
				for (TInt i=0; i < count; i++)
					{
					TRgb rgb((*ptr) | (*(ptr+1) << 8) | (*(ptr+2) << 16));
					if (rgb != palette->GetEntry(i))
						{
						ERR_PRINTF2(_L("PNG palette entry '%d' does not match the CPalette entry"), i);
						verdict = EFail;
						break;
						}
					ptr += 3;
					}

				if (verdict != EFail)
					{
					// Check CRC
					// (N.B. CRC depends on the chunk data (excluding the bytes used for length))
					const TUint8 palCRC[] = { 0xAF, 0x53, 0x4A, 0x47 };
					if (TPtrC8(ptr, 4).Compare(TPtrC8(palCRC, 4)) != 0)
						{
						ERR_PRINTF1(_L("Palette CRC not as expected"));
						verdict = EFail;
						}
					}
				}
			}
		CleanupStack::PopAndDestroy(buffer);
		}
	
	CleanupStack::PopAndDestroy(4,fbsSession);//bitmap, png, encoder, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

//
// Negative tests
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0401_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);
	
	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, ETrue, EFalse, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	HBufC8* buffer = HBufC8::NewMaxLC(4096);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(TSize(32, 48), EColor16M);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}

	if (verdict != EFail)
		{
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());

		// Bit depth 0
		png->iImageInfo.iBitDepth = 0;
		TRAP(err, png->InitFrameL(bufptr, *bitmap));
		if (err != KErrNotSupported)
			{
			ERR_PRINTF2(_L("Unexpected error %d"), err);
			verdict = EFail;
			}
		else
			{
			// Bit depth 1
			png->iImageInfo.iBitDepth = 1;
			png->iImageInfo.iColorType = TPngImageInformation::EDirectColor;
			TRAP(err, png->InitFrameL(bufptr, *bitmap));
			if (err != KErrNotSupported)
				{
				ERR_PRINTF2(_L("Unexpected error %d"), err);
				verdict = EFail;
				}
			else
				{
				// Bit depth 2
				png->iImageInfo.iBitDepth = 2;
				png->iImageInfo.iColorType = TPngImageInformation::EDirectColor;
				TRAP(err, png->InitFrameL(bufptr, *bitmap));
				if (err != KErrNotSupported)
					{
					ERR_PRINTF2(_L("Unexpected error %d"), err);
					verdict = EFail;
					}
				else
					{
					// Bit depth 4
					png->iImageInfo.iBitDepth = 4;
					png->iImageInfo.iColorType = TPngImageInformation::EDirectColor;
					TRAP(err, png->InitFrameL(bufptr, *bitmap));
					if (err != KErrNotSupported)
						{
						ERR_PRINTF2(_L("Unexpected error %d"), err);
						verdict = EFail;
						}
					}
				}
			}
		}

	CleanupStack::PopAndDestroy(5, fbsSession);//bitmap, buffer, png, encoder, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0402_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, ETrue, EFalse, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	HBufC8* buffer = HBufC8::NewLC(0);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(TSize(32, 48), EColor16M);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}

	if (verdict != EFail)
		{
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());

		TRAP(err, png->InitFrameL(bufptr, *bitmap));
		if (err != KErrArgument)
			{
			ERR_PRINTF2(_L("Unexpected error %d"), err);
			verdict = EFail;
			}
		}

	CleanupStack::PopAndDestroy(5, fbsSession);//bitmap, buffer, png, encoder, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0403_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, ETrue, EFalse, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	HBufC8* buffer = HBufC8::NewMaxLC(4096);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);

	if (verdict != EFail)
		{
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());

		TRAPD(err, png->InitFrameL(bufptr, *bitmap));
		if (err != KErrArgument)
			{
			ERR_PRINTF2(_L("Unexpected error %d"), err);
			verdict = EFail;
			}
		}

	CleanupStack::PopAndDestroy(5, fbsSession);//bitmap, buffer, png, encoder, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0404_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	TVerdict verdict = EPass;

	CPngEncoder* encoder = CPngEncoder::NewL();
	CleanupStack::PushL(encoder);

	CPngWriteCodec* png = new(ELeave) CPngWriteCodec(*encoder, 8, ETrue, EFalse, TPngEncodeData::EDefaultCompression);
	CleanupStack::PushL(png);
	HBufC8* buffer = HBufC8::NewMaxLC(0);
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(TSize(32, 48), EColor16M);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}

	if (verdict != EFail)
		{
		TBufPtr8 bufptr;
		bufptr.Set(buffer->Des());

		TRAPD(err, png->ProcessFrameL(bufptr));
		if (err != KErrArgument)
			{
			ERR_PRINTF2(_L("Unexpected error %d"), err);
			verdict = EFail;
			}
		}

	CleanupStack::PopAndDestroy(5, fbsSession);//bitmap, buffer, png, encoder, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0501_L()
{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	INFO_PRINTF1(_L("Test for opening png files with invalid color type and/or invalid bit depth"));

	TVerdict verdict = EPass;

	CImageDecoder* decoder = NULL;

	_LIT(KFile22, "\\ICL\\Validation\\png\\00000022.png");
	TPtrC file22(KFile22);
	
	RFs fSession;
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(fSession.Connect());
	CleanupClosePushL(fSession);

	INFO_PRINTF2(_L("Opening the file %S with color type - EGrayScale and Bit depth - 3 (invalid)"), &file22);

	TRAPD(error22,decoder = CImageDecoder::FileNewL(fSession, file22));
	if (error22 != KErrNotFound)
		{
		INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error22, &file22);
		verdict = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Opening the file %S returned KErrCorrupt. Result - Pass"),&file22);
		}

	delete decoder;
	decoder = NULL;

	_LIT(KFile64, "\\ICL\\Validation\\png\\00000064.png");
	TPtrC file64(KFile64);

	INFO_PRINTF2(_L("Opening the file %S with color type - EAlphaDirectColor and Bit depth - 4 (invalid)"), &file64);

	TRAPD(error64,decoder = CImageDecoder::FileNewL(fSession, file64));
	if (error64 != KErrNotFound)
		{
		INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error64, &file64);
		verdict = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Opening the file %S returned KErrCorrupt. Result - Pass"),&file64);
		}

	delete decoder;
	decoder = NULL;

	_LIT(KFile316, "\\ICL\\Validation\\png\\00000316.png");
	TPtrC file316(KFile316);

	INFO_PRINTF2(_L("Opening the file %S with color type - EIndexedColor and Bit depth - 16 (invalid)"), &file316);

	TRAPD(error316,decoder = CImageDecoder::FileNewL(fSession, file316));
	if (error316 != KErrNotFound)
		{
		INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error316, &file316);
		verdict = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Opening the file %S returned KErrCorrupt. Result - Pass"),&file316);
		}

	delete decoder;
	decoder = NULL;

	_LIT(KFile55, "\\ICL\\Validation\\png\\00000055.png");
	TPtrC file55(KFile55);

	INFO_PRINTF2(_L("Opening the file %S with color type - 1 (invalid) and Bit depth - 8"), &file55);

	TRAPD(error55,decoder = CImageDecoder::FileNewL(fSession, file55));
	if (error55 != KErrNotFound)
		{
		INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error55, &file55);
		verdict = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Opening the file %S returned KErrCorrupt. Result - Pass"),&file55);
		}

	delete decoder;
	decoder = NULL;
	
	CleanupStack::PopAndDestroy(&fSession);
	CleanupStack::PopAndDestroy(fbsSession);
	__MM_HEAP_MARKEND;

	return verdict;
	}

// Accessor class for test MM_ICL_COD_U_0502
class CImageDecoderPluginAccessor : public CImageDecoderPlugin
	{
public:
	void HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)
		{
		return CImageDecoderPlugin::HandleProcessFrameResult(aErrCode, aCodecState);
		};
	};

// Accessor class for test MM_ICL_COD_U_0502
class CImageDecoderAccessor : public CImageDecoder
	{
public:
	CImageDecoderPlugin* Plugin() const
		{
		return CImageDecoder::Plugin();
		};			
	};
	
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0502_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	INFO_PRINTF1(_L("Test for cancelling decoder at the same time as it finishes with an error"));

	TVerdict verdict = EPass;

	CImageDecoder* decoder = NULL;

	_LIT(KFile1, "\\TSU_ICL_COD_02\\jfif100_8.jpg");
	TPtrC file1(KFile1);
	
	RFs fSession;
	User::LeaveIfError(fSession.Connect());
	CleanupClosePushL(fSession);

	INFO_PRINTF2(_L("Opening the file %S"), &file1);

	TRAPD(error1, decoder = CImageDecoder::FileNewL(fSession, file1, CImageDecoder::EOptionAlwaysThread));
	if (error1 != KErrNone)
		{
		INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error1, &file1);
		verdict = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Opening the file %S returned KErrNone"),&file1);
		
		CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
		CleanupStack::PushL(bmp);
		User::LeaveIfError(bmp->Create(TSize(350, 208), EGray2));

		CActiveListener* listener = new (ELeave) CActiveListener;
		CleanupStack::PushL(listener);
		listener->InitialiseActiveListener();
		
		decoder->Convert(&listener->iStatus,*bmp);
		decoder->Cancel();	// calls CImageDecoderPriv::RequestComplete
		CActiveScheduler::Start();
		
		if (listener->iStatus.Int() != KErrCancel)
			{
			INFO_PRINTF1(_L("CImageDecoder::Convert failed to cancel cleanly"));
			verdict = EFail;		
			}
		else
			{
			// further calls to CImageDecoderPriv::RequestComplete should not complete this thread again
			// use a chain of accessor classes to call RequestComplete again
			CImageDecoderPlugin* plugin = static_cast<CImageDecoderAccessor*>(decoder)->Plugin();
			static_cast<CImageDecoderPluginAccessor*>(plugin)->HandleProcessFrameResult(KErrAbort, EFrameIncomplete); // calls CImageDecoderPriv::RequestComplete

			if (listener->iStatus.Int() != KErrCancel)
				{
				// a change in the error code indicates a second complete
				INFO_PRINTF1(_L("CImageDecoderPriv::RequestComplete has completed this thread twice"));
				verdict = EFail;
				}
			}

		CleanupStack::PopAndDestroy(listener);
		CleanupStack::PopAndDestroy(bmp);
		}

	delete decoder;
	decoder = NULL;

	if (verdict != EFail)
		{
		// check that calling in the reverse order is safe too
		TRAPD(error2, decoder = CImageDecoder::FileNewL(fSession, file1, CImageDecoder::EOptionAlwaysThread));
		if (error2 != KErrNone)
			{
			INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error2, &file1);
			verdict = EFail;
			}
		else 
			{
			INFO_PRINTF2(_L("Opening the file %S returned KErrNone"),&file1);
			
			CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
			CleanupStack::PushL(bmp);
			User::LeaveIfError(bmp->Create(TSize(350, 208), EGray2));

			CActiveListener* listener = new (ELeave) CActiveListener;
			CleanupStack::PushL(listener);
			listener->InitialiseActiveListener();
			
			decoder->Convert(&listener->iStatus,*bmp);
			// use a chain of accessor classes to call RequestComplete
			CImageDecoderPlugin* plugin = static_cast<CImageDecoderAccessor*>(decoder)->Plugin();
			static_cast<CImageDecoderPluginAccessor*>(plugin)->HandleProcessFrameResult(KErrAbort, EFrameIncomplete); // calls CImageDecoderPriv::RequestComplete
			CActiveScheduler::Start();
			
			if (listener->iStatus.Int() != KErrAbort)
				{
				INFO_PRINTF1(_L("CImageDecoder::Convert failed to complete properly"));
				verdict = EFail;		
				}
			else
				{
				// further calls to CImageDecoderPriv::RequestComplete should not complete this thread again
				decoder->Cancel();	// calls CImageDecoderPriv::RequestComplete
				if (listener->iStatus.Int() != KErrAbort)
					{
					// a change in the error code indicates a second complete
					INFO_PRINTF1(_L("CImageDecoderPriv::RequestComplete has completed this thread twice"));
					verdict = EFail;
					}
				}
			
			CleanupStack::PopAndDestroy(listener);
			CleanupStack::PopAndDestroy(bmp);
			}
			
		delete decoder;
		decoder = NULL;
		}
		
	CleanupStack::PopAndDestroy(&fSession);
	CleanupStack::PopAndDestroy(fbsSession);
	__MM_HEAP_MARKEND;

	return verdict;
	}

// Accessor class for test MM_ICL_COD_U_0503
class CImageEncoderPluginAccessor : public CImageEncoderPlugin
	{
public:
	void HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)
		{
		return CImageEncoderPlugin::HandleProcessFrameResult(aErrCode, aCodecState);
		};
	};

// Accessor class for test MM_ICL_COD_U_0503
class CImageEncoderAccessor : public CImageEncoder
	{
public:
	CImageEncoderPlugin* Plugin() const
		{
		return CImageEncoder::Plugin();
		};			
	};
	
TVerdict RPngWriteCodecTest::MM_ICL_COD_U_0503_L()
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	INFO_PRINTF1(_L("Test for cancelling encoder at the same time as it finishes with an error"));

	TVerdict verdict = EPass;

	CImageEncoder* encoder = NULL;

	_LIT(KFile1, "c:\\imagebmptemp");
	TPtrC file1(KFile1);
	
	RFs fSession;
	User::LeaveIfError(fSession.Connect());
	CleanupClosePushL(fSession);

	INFO_PRINTF2(_L("Creating the file %S"), &file1);

	RFile file;
	User::LeaveIfError(file.Replace(fSession, KFile1, EFileWrite | EFileShareAny));
	CleanupClosePushL(file);

	TRAPD(error1, encoder = CImageEncoder::FileNewL(fSession, file1, CImageEncoder::EOptionAlwaysThread, KImageTypeBMPUid));
	if (error1 != KErrNone)
		{
		INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error1, &file1);
		verdict = EFail;
		}
	else 
		{
		INFO_PRINTF2(_L("Opening the file %S returned KErrNone"),&file1);
		
		CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
		CleanupStack::PushL(bmp);
		User::LeaveIfError(bmp->Create(TSize(350, 208), EGray2));

		CActiveListener* listener = new (ELeave) CActiveListener;
		CleanupStack::PushL(listener);
		listener->InitialiseActiveListener();
		
		encoder->Convert(&listener->iStatus,*bmp);
		encoder->Cancel();	// calls CImageEncoderPriv::RequestComplete
		CActiveScheduler::Start();
		
		if (listener->iStatus.Int() != KErrCancel)
			{
			INFO_PRINTF1(_L("CImageEncoder::Convert failed to cancel cleanly"));
			verdict = EFail;		
			}
		else
			{
			// further calls to CImageEncoderPriv::RequestComplete should not complete this thread again
			// use a chain of accessor classes to call RequestComplete again
			CImageEncoderPlugin* plugin = static_cast<CImageEncoderAccessor*>(encoder)->Plugin();
			static_cast<CImageEncoderPluginAccessor*>(plugin)->HandleProcessFrameResult(KErrAbort, EFrameIncomplete); // calls CImageEncoderPriv::RequestComplete

			if (listener->iStatus.Int() != KErrCancel)
				{
				// a change in the error code indicates a second complete
				INFO_PRINTF1(_L("CImageEncoderPriv::RequestComplete has completed this thread twice"));
				verdict = EFail;
				}
			}

		CleanupStack::PopAndDestroy(listener);
		CleanupStack::PopAndDestroy(bmp);
		}
		
	delete encoder;
	encoder = NULL;
		
	if (verdict != EFail)
		{		
		// check that calling in the reverse order is safe too
		TRAPD(error2, encoder = CImageEncoder::FileNewL(fSession, file1, CImageEncoder::EOptionAlwaysThread, KImageTypeBMPUid));
		if (error2 != KErrNone)
			{
			INFO_PRINTF3(_L("Unexpected error %d while opening the file %S"), error2, &file1);
			verdict = EFail;
			}
		else 
			{
			INFO_PRINTF2(_L("Opening the file %S returned KErrNone"),&file1);
			
			CFbsBitmap* bmp = new (ELeave) CFbsBitmap;
			CleanupStack::PushL(bmp);
			User::LeaveIfError(bmp->Create(TSize(350, 208), EGray2));

			CActiveListener* listener = new (ELeave) CActiveListener;
			CleanupStack::PushL(listener);
			listener->InitialiseActiveListener();
			
			encoder->Convert(&listener->iStatus,*bmp);
			// use a chain of accessor classes to call RequestComplete
			CImageEncoderPlugin* plugin = static_cast<CImageEncoderAccessor*>(encoder)->Plugin();
			static_cast<CImageEncoderPluginAccessor*>(plugin)->HandleProcessFrameResult(KErrAbort, EFrameIncomplete); // calls CImageEncoderPriv::RequestComplete
			CActiveScheduler::Start();
			
			if (listener->iStatus.Int() != KErrAbort)
				{
				INFO_PRINTF1(_L("CImageEncoder::Convert failed to complete properly"));
				verdict = EFail;		
				}
			else
				{
				// further calls to CImageEncoderPriv::RequestComplete should not complete this thread again
				encoder->Cancel();	// calls CImageEncoderPriv::RequestComplete
				if (listener->iStatus.Int() != KErrAbort)
					{
					// a change in the error code indicates a second complete
					INFO_PRINTF1(_L("CImageEncoderPriv::RequestComplete has completed this thread twice"));
					verdict = EFail;
					}
				}
			
			CleanupStack::PopAndDestroy(listener);
			CleanupStack::PopAndDestroy(bmp);
			}
			
		delete encoder;
		encoder = NULL;
		}
		
	CleanupStack::PopAndDestroy(&file);
	CleanupStack::PopAndDestroy(&fSession);
	CleanupStack::PopAndDestroy(fbsSession);
	__MM_HEAP_MARKEND;

	return verdict;
	}
//
// RPngWriteSubCodecTest

_LIT(KSubCodecTestFileName, "wolf.mbm");

RPngWriteSubCodecTest* RPngWriteSubCodecTest::NewL(CTestSuite* aSuite, const TDesC& aTestName, TPngWriteSubCodecTestId aTestId)
	{
	return new(ELeave) RPngWriteSubCodecTest(aSuite, aTestName, aTestId);
	}
	
RPngWriteSubCodecTest::RPngWriteSubCodecTest(CTestSuite* aSuite, const TDesC& aTestName, TPngWriteSubCodecTestId aTestId)
	: iTestId(aTestId)
	{
	iSuite = aSuite;
	iTestStepName = aTestName;
	}

TVerdict RPngWriteSubCodecTest::OpenL()
	{
	TVerdict verdict = EPass;

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	    User::LeaveIfError(FbsStartup());
	    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
            User::LeaveIfError(errCode);
        }
	return verdict;
	}

void RPngWriteSubCodecTest::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	}

void RPngWriteSubCodecTest::SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecSuite*)iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();
	}

TVerdict RPngWriteSubCodecTest::DoTestStepL()
	{
	switch (iTestId)
		{
		case ECOD_0230:
			return MM_ICL_COD_U_0230_L();
		case ECOD_0231:
			return MM_ICL_COD_U_0231_L();
		case ECOD_0232:
			return MM_ICL_COD_U_0232_L();
		case ECOD_0233:
			return MM_ICL_COD_U_0233_L();
		case ECOD_0234:
			return MM_ICL_COD_U_0234_L();
		case ECOD_0235:
			return MM_ICL_COD_U_0235_L();
		case ECOD_0236:
			return MM_ICL_COD_U_0236_L();
		case ECOD_0237:
			return MM_ICL_COD_U_0237_L();
		case ECOD_0238:
			return MM_ICL_COD_U_0238_L();
		case ECOD_0239:
			return MM_ICL_COD_U_0239_L();
		case ECOD_0240:
			return MM_ICL_COD_U_0240_L();
		case ECOD_0241:
			return MM_ICL_COD_U_0241_L();
		case ECOD_0242:
			return MM_ICL_COD_U_0242_L();
		case ECOD_0243:
			return MM_ICL_COD_U_0243_L();
		case ECOD_0244:
			return MM_ICL_COD_U_0244_L();
		case ECOD_0245:
			return MM_ICL_COD_U_0245_L();
		case ECOD_0250:
			return MM_ICL_COD_U_0250_L();
		default:
			break;
		}
	ERR_PRINTF1(_L("Invalid test ID"));
	return EFail;
	} 

TVerdict RPngWriteSubCodecTest::TestSubCodecL(const TInt aBitDepth, TPngImageInformation::TColorType aColorType,
											  const TSize& aSize, TDisplayMode aMode, const TInt aScanlineBufferSize)
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();

	TVerdict verdict = EPass;
	TPngImageInformation info;
	Mem::FillZ(&info, sizeof(info));
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Create(aSize, aMode);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating bitmap (%d)"), err);
		verdict = EFail;
		}

	if (verdict != EFail)
		{
		info.iBitDepth = aBitDepth;
		info.iColorType = aColorType;
		info.iSize = bitmap->SizeInPixels();

		CPngWriteSubCodec* sub = CPngWriteSubCodec::NewL(info, bitmap);
		CleanupStack::PushL(sub);

		if (sub->iSource != bitmap)
			{
			ERR_PRINTF1(_L("iSource bitmap not set correctly"));
			verdict = EFail;
			}
		else
			{
			const TInt bufSize = sub->ScanlineBufferSize(info.iSize.iWidth);
			if ((bufSize != aScanlineBufferSize) ||
				(sub->iScanlineBufferSize != bufSize))
				{
				ERR_PRINTF2(_L("Scanline buffer size incorrect (%d)"), bufSize);
				verdict = EFail;
				}
			else
				{
				if ((sub->iScanlineBuffer->Des().Length() != bufSize) ||
					(sub->iScanlineDes.Length() != bufSize) ||
					(sub->iScanlineDes.MaxLength() != bufSize))
					{
					ERR_PRINTF2(_L("Scanline descriptors incorrect size (%d)"), bufSize);
					verdict = EFail;
					}
				else
					{
					if ((aColorType != TPngImageInformation::EIndexedColor) &&
						(sub->iPalette || sub->iInfo.iPalettePresent))
						{
						// Palette should not exist with the given aColorType
						ERR_PRINTF1(_L("Error: palette exists"));
						verdict = EFail;
						}
					else if (aColorType == TPngImageInformation::EIndexedColor)
						{
						// Check number of palette entries
						TInt entries = 0;
						switch (aBitDepth)
							{
							case 1:
								entries = 2;
								break;
							case 2:
								entries = 4;
								break;
							case 4:
								entries = 16;
								break;
							case 8:
								entries = 256;
								break;
							default:
								ASSERT(0);
								break;
							}
						if (sub->iPalette->Entries() != entries)
							{
							ERR_PRINTF2(_L("Palette entry count incorrect (%d)"), sub->iPalette->Entries());
							verdict = EFail;
							}
						}
					}
				}
			}
		CleanupStack::PopAndDestroy(sub);
		}
	
	CleanupStack::PopAndDestroy(2, fbsSession); //bitmap, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

TVerdict RPngWriteSubCodecTest::TestSubCodecDecodeL(const TInt aBitDepth, TPngImageInformation::TColorType aColorType,
													const TDesC& aFileName)
	{
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;
	TPngImageInformation info;
	Mem::FillZ(&info, sizeof(info));
	TFileName fileName;
	SetTestFileNameL(fileName, aFileName);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Load(fileName);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error loading bitmap (%d)"), err);
		verdict = EFail;
		}

	if (verdict != EFail)
		{
		info.iBitDepth = aBitDepth;
		info.iColorType = aColorType;
		info.iSize = bitmap->SizeInPixels();
		info.iFilterMethod = TPngImageInformation::EAdaptiveFiltering;

		CPngWriteSubCodec* sub = CPngWriteSubCodec::NewL(info, bitmap);
		CleanupStack::PushL(sub);

		TPtrC8 scanline(sub->iScanlineBuffer->Des().Ptr(), sub->iScanlineBufferSize);
		const TUint8* ptrLimit = scanline.Ptr() + sub->iScanlineBufferSize;
		ASSERT(bitmap->SizeInPixels().iHeight > 70);	// test bitmap should be at least 71 pixels in height
		// Choose line 70 as our sample scanline as this (in wolf.mbm) has some different coloured pixels
		sub->DoEncode(bitmap, 70, const_cast<TUint8*>(scanline.Ptr()), ptrLimit);

		// Compare encoded scanline with reference scanline
		TUint8* refPtr = NULL;
		TInt refLen = 0;
		switch (aBitDepth)
			{
			case 1:
				switch (aColorType)
					{
					case TPngImageInformation::EDirectColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_1bit_t0[0]);
						refLen = KSubCodecRef_1bit_t0_Length;
						break;
					case TPngImageInformation::EIndexedColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_1bit_t3[0]);
						refLen = KSubCodecRef_1bit_t3_Length;
						break;
					default:
						ASSERT(0);
						break;
					}
				break;
			case 2:
				switch (aColorType)
					{
					case TPngImageInformation::EDirectColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_2bit_t0[0]);
						refLen = KSubCodecRef_2bit_t0_Length;
						break;
					case TPngImageInformation::EIndexedColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_2bit_t3[0]);
						refLen = KSubCodecRef_2bit_t3_Length;
						break;
					default:
						ASSERT(0);
						break;
					}
				break;
			case 4:
				switch (aColorType)
					{
					case TPngImageInformation::EDirectColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_4bit_t0[0]);
						refLen = KSubCodecRef_4bit_t0_Length;
						break;
					case TPngImageInformation::EIndexedColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_4bit_t3[0]);
						refLen = KSubCodecRef_4bit_t3_Length;
						break;
					default:
						ASSERT(0);
						break;
					}
				break;
			case 8:
				switch (aColorType)
					{
					case TPngImageInformation::EGrayscale:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_8bit_t0[0]);
						refLen = KSubCodecRef_8bit_t0_Length;
						break;
					case TPngImageInformation::EDirectColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_8bit_t2[0]);
						refLen = KSubCodecRef_8bit_t2_Length;
						break;
					case TPngImageInformation::EIndexedColor:
						refPtr = const_cast<TUint8*>(&KSubCodecRef_8bit_t3[0]);
						refLen = KSubCodecRef_8bit_t3_Length;
						break;
					default:
						ASSERT(0);
						break;
					}
				break;
			default:
				ASSERT(0);
				break;
			}
		TPtrC8 reference(refPtr, refLen);
		if (scanline.Compare(reference) != 0)
			{
			ERR_PRINTF1(_L("Encoded scanline does not match reference scanline"));
			verdict = EFail;
			}
		CleanupStack::PopAndDestroy(sub);
		}

	CleanupStack::PopAndDestroy(2, fbsSession); //bitmap, fbsSession
	__MM_HEAP_MARKEND;

	return verdict;
	}

// Sub-codec construction tests
TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0230_L()
	{
	return TestSubCodecL(1, TPngImageInformation::EDirectColor, TSize(33, 47), EGray2, 6);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0231_L()
	{
	return TestSubCodecL(1, TPngImageInformation::EIndexedColor, TSize(33, 47), EGray2, 6);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0232_L()
	{
	return TestSubCodecL(2, TPngImageInformation::EIndexedColor, TSize(33, 47), EGray4, 10);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0233_L()
	{
	return TestSubCodecL(4, TPngImageInformation::EIndexedColor, TSize(33, 47), EGray16, 18);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0234_L()
	{
	return TestSubCodecL(8, TPngImageInformation::EGrayscale, TSize(33, 47), EColor16M, 34);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0235_L()
	{
	return TestSubCodecL(8, TPngImageInformation::EIndexedColor, TSize(33, 47), EColor16M, 34);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0236_L()
	{
	return TestSubCodecL(8, TPngImageInformation::EDirectColor, TSize(33, 47), EColor16M, 100);
	}

// Sub-codec DoEncode tests
TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0237_L()
	{
	return TestSubCodecDecodeL(1, TPngImageInformation::EDirectColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0238_L()
	{
	return TestSubCodecDecodeL(1, TPngImageInformation::EIndexedColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0239_L()
	{
	return TestSubCodecDecodeL(2, TPngImageInformation::EDirectColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0240_L()
	{
	return TestSubCodecDecodeL(2, TPngImageInformation::EIndexedColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0241_L()
	{
	return TestSubCodecDecodeL(4, TPngImageInformation::EDirectColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0242_L()
	{
	return TestSubCodecDecodeL(4, TPngImageInformation::EIndexedColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0243_L()
	{
	return TestSubCodecDecodeL(8, TPngImageInformation::EGrayscale, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0244_L()
	{
	return TestSubCodecDecodeL(8, TPngImageInformation::EDirectColor, KSubCodecTestFileName);
	}

TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0245_L()
	{
	return TestSubCodecDecodeL(8, TPngImageInformation::EIndexedColor, KSubCodecTestFileName);
	}

// Other sub-codec tests
TVerdict RPngWriteSubCodecTest::MM_ICL_COD_U_0250_L()
	{
	// Test ReverseBits function

	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TVerdict verdict = EPass;
	TPngImageInformation info;
	Mem::FillZ(&info, sizeof(info));
	info.iBitDepth = 1;
	CPngWriteSubCodec* sub = CPngWriteSubCodec::NewL(info, NULL);

	const TUint8 testData[] = { 0xFF,	// 1111 1111	Test
								0xFF,	// 1111 1111	Expected
								0xAA,	// 1010 1010	Test
								0x55,	// 0101 0101	Expected
								0xB2,	// 1011 0010	Test
								0x4D,	// 0100 1101	Expected
								0xF0,	// 1111 0000	Test
								0x0F,	// 0000 1111	Expected
								0x00,	// 0000 0000	Test
								0x00	// 0000 0000	Expected
								};
	const TInt count = sizeof(testData) / sizeof(TUint8);
	for (TInt i=0; i < count; i+=2)
		{
		if (sub->ReverseBits(testData[i]) != testData[i+1])
			{
			ERR_PRINTF3(_L("ReverseBits did not match expected results (index %d and %d)"), i, i+1);
			verdict = EFail;
			break;
			}
		}

	delete sub;
	CleanupStack::PopAndDestroy(fbsSession);
	__MM_HEAP_MARKEND;

	return verdict;
	}
	
//
// CRecogWrapper - wrap opening of recognizer - either ecom or .mdl
//

class CRecogWrapper : public CBase
	{
public:
	static CRecogWrapper* NewLC(CLog* aLog, TBool aAssumeOOM=EFalse);
	~CRecogWrapper();
	CApaIclRecognizer* Recognizer() { return iRecognizer; }
private:
	CRecogWrapper(CLog* aLog, TBool aAssumeOOM);
	void ConstructL();
private:
	CApaIclRecognizer* iRecognizer;
	CLog*const iLog;
	const TBool iAssumeOOM;
	TUid iDtorKey;
	};
	
CRecogWrapper::CRecogWrapper(CLog* aLog, TBool aAssumeOOM):
	iLog(aLog),
	iAssumeOOM(aAssumeOOM)
	{
	}
	
CRecogWrapper* CRecogWrapper::NewLC(CLog* aLog, TBool aAssumeOOM)
	{
	CRecogWrapper* result = new (ELeave) CRecogWrapper(aLog, aAssumeOOM);
	CleanupStack::PushL(result);
	result->ConstructL();
	return result;
	}
	
void CRecogWrapper::ConstructL()
	{
	const TUid KIclRecImp = {0x101F7C40};
	
	iRecognizer = static_cast<CApaIclRecognizer*>(
					  reinterpret_cast<CApaDataRecognizerType*>(
						  REComSession::CreateImplementationL(KIclRecImp, iDtorKey)));
	}
	
CRecogWrapper::~CRecogWrapper()
	{
	delete iRecognizer;
	REComSession::DestroyedImplementation(iDtorKey);
//	REComSession::Close();
	}
	
//
// RCodecDataTest
/**
 * Constructor for TCodecDataType helper class
 *
 * @xxxx
 */
RCodecDataTest::TCodecDataType::TCodecDataType(const TDesC& aFileName, const TDesC8& aMimeType, TUid aImageType, TUid aImageSubType, CApaDataRecognizerType::TRecognitionConfidence aConfidence) :
	iFileName(aFileName),
	iMimeType(aMimeType),
	iImageType(aImageType),
	iImageSubType(aImageSubType),
	iConfidence(aConfidence)
	{
	}

/**
 * Add an info string to the data
 * @param	"const TDesC& aInfoString"
 *			The string to add
 */
TInt RCodecDataTest::TCodecDataType::AddInfoString(const TDesC& aInfoString)
	{
	return iInfoStrings.Append(aInfoString);
	}

_LIT(KImageGifFileName, "trans_1.gif");
_LIT8(KMimeTypeGif,"image/gif");
_LIT(KImageGifFrameInfo1, "Decoder: Gif decoder V1.0");
_LIT(KImageGifFrameInfo2, "Format: Gif89a");
_LIT(KImageGifFrameInfo3, "Dimensions: 32x32 pixels");
_LIT(KImageGifFrameInfo4, "Depth: 1bpp colour");

_LIT(KImageIcoFileName, "icoepoc.ico");
_LIT8(KMimeTypeIco,"image/ico");
_LIT(KImageIcoFrameInfo1, "Decoder: Ico decoder V1.0");
_LIT(KImageIcoFrameInfo2, "Format: Ico");
_LIT(KImageIcoFrameInfo3, "Dimensions: 32x32 pixels");
_LIT(KImageIcoFrameInfo4, "Depth: 4bpp colour");

_LIT(KImageBmpFileName, "bmp1bpp_1.bmp");
_LIT8(KMimeTypeBmp,"image/bmp");
_LIT(KImageBmpFrameInfo1, "Decoder: Bmp decoder V1.0");
_LIT(KImageBmpFrameInfo2, "Format: Bmp");
_LIT(KImageBmpFrameInfo3, "Dimensions: 193x151 pixels");
_LIT(KImageBmpFrameInfo4, "Depth: 1bpp b&w");
_LIT(KImageBmpFrameInfo5, "Details: Uncompressed");

_LIT(KImageJpgFileName, "jfif100_8.jpg");
_LIT8(KMimeTypeJpeg,"image/jpeg");
_LIT8(KMimeTypeJpg,"image/jpg");
_LIT(KImageJpgFrameInfo1, "Decoder: Jpg decoder V1.0");
_LIT(KImageJpgFrameInfo2, "Format: Jpg");
_LIT(KImageJpgFrameInfo3, "Dimensions: 193x151 pixels");
_LIT(KImageJpgFrameInfo4, "Depth: 24bpp colour");
_LIT(KImageJpgFrameInfo5, "Details: Sequential encoding");

_LIT(KImageMbmFileName, "wolf.mbm");
_LIT8(KMimeTypeMbm,"image/x-epoc-mbm");
_LIT(KImageMbmFrameInfo1, "Decoder: Mbm decoder V1.0");
_LIT(KImageMbmFrameInfo2, "Format: Mbm");
_LIT(KImageMbmFrameInfo3, "Dimensions: 193x151 pixels");
_LIT(KImageMbmFrameInfo4, "Depth: 24bpp colour");
_LIT(KImageMbmFrameInfo5, "Details: Uncompressed");

_LIT(KImageOtaFileName, "otabitmap");
_LIT(KImageOtaSuffixFileName, "otabitmap.ota");
_LIT8(KMimeTypeOta,"image/x-ota-bitmap");
_LIT(KImageOtaFrameInfo1, "Decoder: SMS/OTA decoder V1.0");
_LIT(KImageOtaFrameInfo2, "Format: OTA");
_LIT(KImageOtaFrameInfo3, "Dimensions: 72x14 pixels");
_LIT(KImageOtaFrameInfo4, "Depth: 1bpp b&w");

_LIT(KImagePngFileName, "png2_2.png");
_LIT8(KMimeTypePng,"image/png");
_LIT(KImagePngFrameInfo1, "Decoder: Png decoder V1.0");
_LIT(KImagePngFrameInfo2, "Format: Png");
_LIT(KImagePngFrameInfo3, "Dimensions: 380x287 pixels");
_LIT(KImagePngFrameInfo4, "Depth: 24bpp colour");
_LIT(KImagePngFrameInfo5, "Details: Image includes alpha channel");

_LIT(KImageTifFileName, "tif3_4.tif");
_LIT8(KMimeTypeTif,"image/tiff");
_LIT(KImageTifFrameInfo1, "Decoder: Tiff decoder V1.0");
_LIT(KImageTifFrameInfo2, "Format: Group3 Fax (1D)");
_LIT(KImageTifFrameInfo3, "Dimensions: 600x214 pixels");
_LIT(KImageTifFrameInfo4, "Depth: 1bpp b&w");

_LIT(KImageWbmpFileName, "WirelessBitmap");
_LIT(KImageWbmpSuffixFileName, "WirelessBitmap.wbmp");
_LIT8(KMimeTypeWbmp,"image/vnd.wap.wbmp");
_LIT(KImageWbmpFrameInfo1, "Decoder: WBmp decoder V1.0");
_LIT(KImageWbmpFrameInfo2, "Format: WBmp");
_LIT(KImageWbmpFrameInfo3, "Dimensions: 108x102 pixels");
_LIT(KImageWbmpFrameInfo4, "Depth: 1bpp b&w");

_LIT(KImageWmfFileName, "wmf2.wmf");
_LIT8(KMimeTypeWmf,"image/x-wmf");
_LIT(KImageWmfFrameInfo1, "Decoder: Wmf decoder V1.0");
_LIT(KImageWmfFrameInfo2, "Format: Apm");
//WMF report dimensions in twips
_LIT(KImageWmfFrameInfo3, "Dimensions: 2504x3029 twips");
_LIT(KImageWmfFrameInfo4, "Depth: 24bpp colour");

const CApaDataRecognizerType::TRecognitionConfidence KConfidenceProbable = CApaDataRecognizerType::EProbable;
const CApaDataRecognizerType::TRecognitionConfidence KConfidencePossible = CApaDataRecognizerType::EPossible;
const CApaDataRecognizerType::TRecognitionConfidence KConfidenceNotRecognized = CApaDataRecognizerType::ENotRecognized;

RCodecDataTest* RCodecDataTest::NewL(CTestSuite* aSuite, const TDesC& aTestName, TCodecDataTestId aTestId)
	{
	return new(ELeave) RCodecDataTest(aSuite, aTestName, aTestId);
	}
	
RCodecDataTest::RCodecDataTest(CTestSuite* aSuite, const TDesC& aTestName, TCodecDataTestId aTestId) :
	iTestId(aTestId)
	{
	iSuite = aSuite;
	iTestStepName = aTestName;

	// need to up the default heap size to handle the Application Architecture Server
	// Start App server
	iHeapSize = 0x40000;	
	}

TVerdict RCodecDataTest::OpenL()
	{
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
            User::LeaveIfError(errCode);
        }
	// Connect to file server
	User::LeaveIfError(iFs.Connect());

	//BMP
	{
	TCodecDataType frameInfo(KImageBmpFileName,KMimeTypeBmp,KImageTypeBMPUid,KNullUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageBmpFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageBmpFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageBmpFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageBmpFrameInfo4()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageBmpFrameInfo5()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//GIF
	{
	TCodecDataType frameInfo(KImageGifFileName,KMimeTypeGif,KImageTypeGIFUid,KNullUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageGifFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageGifFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageGifFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageGifFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//ICO
	{
	TCodecDataType frameInfo(KImageIcoFileName,KMimeTypeIco,KImageTypeICOUid,KNullUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageIcoFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageIcoFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageIcoFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageIcoFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//JPEG
	{
	TCodecDataType frameInfo(KImageJpgFileName,KMimeTypeJpeg,KImageTypeJPGUid,KNullUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageJpgFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageJpgFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageJpgFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageJpgFrameInfo4()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageJpgFrameInfo5()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//MBM
	{
	TCodecDataType frameInfo(KImageMbmFileName,KMimeTypeMbm,KImageTypeMBMUid,KNullUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageMbmFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageMbmFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageMbmFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageMbmFrameInfo4()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageMbmFrameInfo5()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//OTA (expect not to be recognized)
	{
	TCodecDataType frameInfo(KImageOtaFileName,KNullDesC8(),KImageTypeOTAUid,KNullUid,KConfidenceNotRecognized);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//OTA with suffix
	{
	TCodecDataType frameInfo(KImageOtaSuffixFileName,KMimeTypeOta,KImageTypeOTAUid,KNullUid,KConfidencePossible);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageOtaFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//PNG
	{
	TCodecDataType frameInfo(KImagePngFileName,KMimeTypePng,KImageTypePNGUid,KNullUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImagePngFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImagePngFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImagePngFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImagePngFrameInfo4()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImagePngFrameInfo5()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//TIFF
	{
	TCodecDataType frameInfo(KImageTifFileName,KMimeTypeTif,KImageTypeTIFFUid,KImageTypeTIFFSubTypeLittleEndianUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageTifFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageTifFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageTifFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageTifFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//WBMP 
	{
	TCodecDataType frameInfo(KImageWbmpFileName,KNullDesC8(),KImageTypeWBMPUid,KNullUid,KConfidenceNotRecognized);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//WBMP with suffix
	{
	TCodecDataType frameInfo(KImageWbmpSuffixFileName,KMimeTypeWbmp,KImageTypeWBMPUid,KNullUid,KConfidencePossible);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWbmpFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}
	//WMF
	{
	TCodecDataType frameInfo(KImageWmfFileName,KMimeTypeWmf,KImageTypeWMFUid,KImageTypeWMFSubTypeApmUid,KConfidenceProbable);
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWmfFrameInfo1()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWmfFrameInfo2()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWmfFrameInfo3()));
	User::LeaveIfError(frameInfo.iInfoStrings.Append(KImageWmfFrameInfo4()));
	User::LeaveIfError(iPluginData.Append(frameInfo));
	}

	return iTestStepResult;
	}

void RCodecDataTest::Close()
	{
	for(TInt cnt=0;cnt<iPluginData.Count();cnt++)
		iPluginData[cnt].iInfoStrings.Reset();
	iPluginData.Reset();
	iFs.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	}

void RCodecDataTest::SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecSuite*)iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();
	}

/** 
 *
 *	Open image, retrieve frame info strings & verify against expected result
 *
 * @param		"TCodecDataType& aCodecData"
 *
 */
void RCodecDataTest::VerifyFrameInfoStringsL(const TCodecDataType& aCodecData)
	{
	TFileName fileName;
	SetTestFileNameL(fileName, aCodecData.iFileName);

	CImageDecoder* decoder = NULL;
	// OTA and WBMP without file extensions has to be called by image type
	if((aCodecData.iFileName == KImageOtaFileName) ||
	   (aCodecData.iFileName == KImageWbmpFileName))
	   decoder = CImageDecoder::FileNewL(iFs, fileName, CImageDecoder::EOptionAlwaysThread,aCodecData.iImageType,aCodecData.iImageSubType);
	else
		decoder = CImageDecoder::FileNewL(iFs, fileName);
	CleanupStack::PushL(decoder);

	CFrameInfoStrings* theStr = decoder->FrameInfoStringsL(0);
	CleanupStack::PushL(theStr);

	Compare(theStr->Count(),aCodecData.iInfoStrings.Count(),_L("FrameInfoString count wrong"));

	for(TInt index = 0; index < theStr->Count(); index++)
		Compare(theStr->String(index),aCodecData.iInfoStrings[index],_L("Strings does not match"));

    CleanupStack::PopAndDestroy(2,decoder); // theStr,decoder 
	}

/** 
 *
 *	Open image, retrieve ImageType and SubType & verify against expected result
 *
 * @param		"TCodecDataType& aCodecData"
 *
 */
void RCodecDataTest::VerifyImageTypesL(const TCodecDataType& aCodecData)
	{
	TFileName fileName;
	SetTestFileNameL(fileName, aCodecData.iFileName);

	CImageDecoder* decoder = NULL;
	// OTA and WBMP without file extensions has to be called by image type
	if((aCodecData.iFileName == KImageOtaFileName) ||
	   (aCodecData.iFileName == KImageWbmpFileName))
	   decoder = CImageDecoder::FileNewL(iFs, fileName, CImageDecoder::EOptionAlwaysThread,aCodecData.iImageType,aCodecData.iImageSubType);
	else
		decoder = CImageDecoder::FileNewL(iFs, fileName);
	CleanupStack::PushL(decoder);

	TUid imageType;
	TUid imageSubType;
	decoder->ImageType(0,imageType, imageSubType);

	Compare(imageType,aCodecData.iImageType,_L("ImageType incorrect"));
	Compare(imageSubType,aCodecData.iImageSubType,_L("ImageSubType incorrect"));

	CleanupStack::PopAndDestroy(decoder);
	}

/** 
 *
 * Retrieve MIMEType by file and descriptor & verify against expected result
 *
 * @param		"TCodecDataType& aCodecData"
 *
 */
void RCodecDataTest::VerifyMimeTypesL(const TCodecDataType& aCodecData, TMimeTest aTestType)
	{
	TBuf8<KBufferSize> mimeType;

	TFileName fileName;
	SetTestFileNameL(fileName, aCodecData.iFileName);

	switch(aTestType)
		{
		case EMIME_TEST_FILE:
			//ignore OTA and WBMP without extensions
			if((aCodecData.iFileName == KImageOtaFileName) ||
			   (aCodecData.iFileName == KImageWbmpFileName))
				{
				INFO_PRINTF1(_L("    - Not tested -"));
				return;
				}

			CImageDecoder::GetMimeTypeFileL(iFs, fileName, mimeType);
			break;
		
		case EMIME_TEST_DATA:
			{
			//ignore OTA and WBMP
			if((aCodecData.iFileName == KImageOtaFileName) ||
			   (aCodecData.iFileName == KImageOtaSuffixFileName) ||
			   (aCodecData.iFileName == KImageWbmpFileName) ||
			   (aCodecData.iFileName == KImageWbmpSuffixFileName))
				{
				INFO_PRINTF1(_L("    - Not tested -"));
				return;
				}

			RFile file;
			User::LeaveIfError(file.Open(iFs,fileName,EFileRead|EFileShareAny));
			CleanupClosePushL(file);
			TInt size;
			User::LeaveIfError(file.Size(size));
			HBufC8* imageData = HBufC8::NewLC(size);
			TPtr8 imageDataPtr(imageData->Des());
			User::LeaveIfError(file.Read(imageDataPtr));

			CImageDecoder::GetMimeTypeDataL(*imageData, mimeType);

			CleanupStack::PopAndDestroy(2,&file); // imageData, file
			}
			break;
		
		// code to access the ICL recognizer DLL via RApaLsSession::AppForDocument()
		case EMIME_TEST_RECOG:
			{
			TInt ret;

			RApaLsSession myApaLsSession;
			User::LeaveIfError(myApaLsSession.Connect());
			CleanupClosePushL(myApaLsSession);
			
			TUid uid;
			uid.iUid = 0;
			TDataType dataType;
 		    RFile recFile;
 		    
 			ret = iFs.ShareProtected();
 
			User::LeaveIfError(recFile.Open(iFs, fileName, EFileRead|EFileShareAny));
 			CleanupClosePushL(recFile);
 
 			ret = myApaLsSession.AppForDocument(recFile, uid, dataType);

			TBuf16<KBufferSize> strReturned;
			TBuf16<KBufferSize> strExpected;
			strReturned = dataType.Des();
			strExpected.Copy(aCodecData.iMimeType);
			INFO_PRINTF4(_L("    Expected Mime type  \"%S\", returned MIME type = \"%S\", returned app UID = %X)"), &strExpected, &strReturned, uid);
			
			if (ret == KErrNone)
				{
				// get a local copy of the the mime type for Compare()...
				mimeType = dataType.Des8();
				}
			else
				{
				INFO_PRINTF1(_L("    Recognizer failed to recognize file type"));
				User::Leave(KErrNotFound	);
				}


			CleanupStack::PopAndDestroy(2,&myApaLsSession); // recFile and myApaLsSession
			}
			break;

		// code to directly access the ICL recognizer DLL
		case EMIME_TEST_RECOG_DIRECT:
			{
			CRecogWrapper* wrapper = CRecogWrapper::NewLC(iSuite->LogSystem());
			
			CApaIclRecognizer* myRecog = wrapper->Recognizer();
			TUint bufSize = myRecog->PreferredBufSize();

			RFile file;
			User::LeaveIfError(file.Open(iFs, fileName, EFileRead|EFileShareAny));
			CleanupClosePushL(file);
			
			HBufC8* imageData = HBufC8::NewLC(bufSize);
			TPtr8 imageDataPtr(imageData->Des());
			User::LeaveIfError(file.Read(imageDataPtr));
	
			myRecog->DoRecognizeL(fileName, imageDataPtr);
			
			// get a local copy of the the mime type for Compare()...
			mimeType = myRecog->MimeType().Des8();

			//Verify the confidence
			Compare(myRecog->Confidence(),aCodecData.iConfidence,_L("Incorrect Confidence returned"));

			CleanupStack::PopAndDestroy(3, wrapper);		// wrapper, myRecog, file, imageData
	
			}
			break;

		}

	Compare(mimeType,aCodecData.iMimeType,_L("MIME Type incorrect"));

	}

/**
 * 
 * Open images, retrieve frame info & verify
 * 
 */
TVerdict RCodecDataTest::DoTestStepL()
	{
	
	CICLFbsSessionTest* tempFbsSession = CICLFbsSessionTest::NewLC();
  	DummyHalCallL();
	CleanupStack::PopAndDestroy(tempFbsSession);
  
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	TInt noOfPlugins = iPluginData.Count();
	TInt error = KErrNone;


	for(TInt pluginNo = 0; pluginNo < noOfPlugins; pluginNo++)
		{
		INFO_PRINTF2(_L("%S"),&iPluginData[pluginNo].iFileName);
		switch(iTestId)
			{
		case ECOD_2000:
				{//Test FrameInfoStringsL()
				TRAP(error,VerifyFrameInfoStringsL(iPluginData[pluginNo]));
				}
				break;
		case ECOD_2001:
				{//Test ImageType()
				TRAP(error,VerifyImageTypesL(iPluginData[pluginNo]));
				}
				break;
		case ECOD_2002:
				{//Test GetMimeTypeFileL()
				TRAP(error,VerifyMimeTypesL(iPluginData[pluginNo],EMIME_TEST_FILE));
				}
				break;
		case ECOD_2003:
				{//Test GetMimeTypeDataL()
				TRAP(error,VerifyMimeTypesL(iPluginData[pluginNo],EMIME_TEST_DATA));
				}
				break;
		case ECOD_2004:
				{//Test Mime Type against RApaLsSession::AppForDocument()
				TRAP(error,VerifyMimeTypesL(iPluginData[pluginNo],EMIME_TEST_RECOG));
				}
				break;
		case ECOD_2005:
				{//Test Mime Type against CApaIclRecognizer::DoRecognizeL()
				TRAP(error,VerifyMimeTypesL(iPluginData[pluginNo],EMIME_TEST_RECOG_DIRECT));
				}
				break;
		default:
				{
				ERR_PRINTF2(_L("Undefined test %d"),iTestId);
				iTestStepResult = EFail;
				}
			}

			Compare(error,KErrNone,_L("Unexpected error code"));
		}

	CleanupStack::PopAndDestroy(fbsSession);
	__MM_HEAP_MARKEND;

	return iTestStepResult;
	} 


// Recognizer Alloc test

RRecogAllocTest::RRecogAllocTest(const TDesC& aTestName, TCodecDataTestId aTestId) :
	iTestId(aTestId)
	{
	iTestStepName = aTestName;
	}


RRecogAllocTest* RRecogAllocTest::NewL(const TDesC& aTestName, TCodecDataTestId aTestId)
	{
	RRecogAllocTest* self = new(ELeave) RRecogAllocTest(aTestName, aTestId);
	return self;
	}

TVerdict RRecogAllocTest::OpenL() 
	{

	iScheduler = new(ELeave)CActiveScheduler;
    //[ push the scheduler on the stack ]
	CleanupStack::PushL( iScheduler );
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iScheduler );
	
	// [Pop iScheduler ] 
	CleanupStack::Pop( iScheduler);
	
//    User::LeaveIfError(FbsStartup());
//    TInt errCode = RFbsSession::Connect();
//    if( errCode != KErrNone)
//        {
//        INFO_PRINTF1(_L("Shiva 5"));
//            User::LeaveIfError(errCode);
//        }
	// Connect to file server
	User::LeaveIfError(iFs.Connect());

	iFileName.Set(KImageGifFileName);
	iMimeType.Set(KMimeTypeGif);

	return iTestStepResult = EPass;
	}

void RRecogAllocTest::Close() 
	{
	iFs.Close();

	//Destroy the scheduler
	delete iScheduler ;
	iScheduler = NULL;
	}

/** 
 *
 * Retrieve MIMEType by recognizer API & verify against expected result
 *
 * @param		"TTestFileInfo& aCodecData"
 *
 */
void RRecogAllocTest::VerifyMimeTypesL(
	const TDesC& aFileName, 
	TDes8& aMimeType,
	TInt& aConfidenceLevel)
	{
	
	// code to access the Icl recognizer DLL directly
	CRecogWrapper* wrapper = CRecogWrapper::NewLC(iSuite->LogSystem(), ETrue);
	CApaIclRecognizer* myRecog= wrapper->Recognizer();

	TUint bufSize = myRecog->PreferredBufSize();

	TFileName fileName = ((CICLCodecSuite*)iSuite)->DefaultPath();
	fileName.Append(aFileName);
	
	RFile file;
	User::LeaveIfError(file.Open(iFs, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);
	
	HBufC8* imageData = HBufC8::NewLC(bufSize);
	TPtr8 imageDataPtr(imageData->Des());
	User::LeaveIfError(file.Read(imageDataPtr));

	// call some extra function for good measure
	myRecog->SupportedDataTypeL(0);

	myRecog->DoRecognizeL(aFileName, imageDataPtr);
	
	// return the mime type and the confidence level to the caller
	aMimeType = myRecog->MimeType().Des8();
	aConfidenceLevel = myRecog->Confidence();

	CleanupStack::PopAndDestroy(3, wrapper);		// wrapper, file, imageData
	}

TVerdict RRecogAllocTest::DoTestStepL()
	{
	TBool testComplete = EFalse;
	TBuf8<KMaxDataTypeLength> mimeType;
	TInt confidenceLevel = CApaDataRecognizerType::ENotRecognized;

	INFO_PRINTF2(_L("%S"),&iFileName);

	for (TInt failCount = 1; !testComplete; failCount++)
		{
		TInt err = 0;
		__UHEAP_FAILNEXT(failCount);
		__MM_HEAP_MARK;
		
		User::LeaveIfError(FbsStartup());
		err = RFbsSession::Connect();
	    if( err != KErrNone)
	        {

	            User::LeaveIfError(err);
	        }
		
		if (err == KErrNone)
			{
			TRAP(err, VerifyMimeTypesL(iFileName, mimeType, confidenceLevel));
			}

		switch (err)
			{
			case KErrNoMemory :
				break;
			case KErrNone :
				{
				// first check the result
				if (confidenceLevel <= CApaDataRecognizerType::EUnlikely || iMimeType.Compare(mimeType)!=0)
					{
					INFO_PRINTF2(_L("Bad result returned - failcount=%d"), failCount);
					INFO_PRINTF1(_L("    - recognizer failed to recognize file type -"));
					TBuf16<KMaxDataTypeLength> strReturned;
					TBuf16<KMaxDataTypeLength> strExpected;
					strReturned.Copy(mimeType);
					strExpected.Copy(iMimeType);
					INFO_PRINTF3(_L("Mime type not the same (returned = \"%S\", expected = \"%S\")"), &strReturned, &strExpected);
					testComplete = ETrue;
					iTestStepResult = EFail;
					}
				else 
					{

					//attempt to allocate another cell. If this fails, we have tested all 
					//of the memory allocs carried out by NewL.
					TAny *testAlloc = User::Alloc(1);
					if (testAlloc==NULL)
						{
						testComplete = ETrue;
						failCount --;

						TBuf16<KMaxDataTypeLength> strReturned;
						strReturned.Copy(mimeType);
						INFO_PRINTF3(_L("Expected mime type returned (\"%S\") with %d iterations"), &strReturned, failCount);
						}
					else
						{
						delete testAlloc;
						testAlloc = NULL;
						}
					}
				}
				break;

			default:
				{
				INFO_PRINTF3(_L("Unexpected error(%d) returned - failcount=%d"), err, failCount);
				testComplete = ETrue;
				iTestStepResult = EFail;
				break;
				}
			}
		
		RFbsSession::Disconnect();
		__MM_HEAP_MARKEND;
		__UHEAP_RESET;
		}

	return iTestStepResult;

	}


//
// RRecogNegTest

_LIT(KImageFileNameA, "a.txt");
_LIT(KImageFileNameB, "b.txt");
_LIT(KImageFileNameG, "g.txt");
_LIT(KImageFileNameI, "i.txt");
_LIT(KImageFileNameM, "m.txt");
_LIT(KImageFileNameR, "r.txt");
_LIT(KImageFileNameGif, "gif.txt");

RRecogNegTest* RRecogNegTest::NewL(CTestSuite* aSuite, const TDesC& aTestName)
	{
	return new(ELeave) RRecogNegTest(aSuite, aTestName);
	}

RRecogNegTest::RRecogNegTest(CTestSuite* aSuite, const TDesC& aTestName) 
	{
	iSuite = aSuite;
	iTestStepName = aTestName;
	}

TVerdict RRecogNegTest::OpenL()
	{
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
            User::LeaveIfError(errCode);
        }
	// Connect to file server
	User::LeaveIfError(iFs.Connect());

    iFileNames.Append(&KImageFileNameA);
    iFileNames.Append(&KImageFileNameB);
	iFileNames.Append(&KImageFileNameG);
	iFileNames.Append(&KImageFileNameI);
	iFileNames.Append(&KImageFileNameM);
	iFileNames.Append(&KImageFileNameR);
	iFileNames.Append(&KImageFileNameGif);


    return iTestStepResult;
	}

void RRecogNegTest::Close()
	{
	iFileNames.Reset();
	iFs.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	}

void RRecogNegTest::SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecSuite*)iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();
	}

/** 
 *
 * Retrieve MIMEType by file and descriptor & verify against expected result
 *
 * @param		"TCodecDataType& aCodecData"
 *
 */
void RRecogNegTest::VerifyMimeTypesL(const TDesC& aFileName)
	{
	TFileName fileName;
	SetTestFileNameL(fileName, aFileName);
	
	CRecogWrapper* wrapper = CRecogWrapper::NewLC(iSuite->LogSystem());

	CApaIclRecognizer* myRecog=wrapper->Recognizer();
		
	TUint bufSize = myRecog->PreferredBufSize();

	RFile file;
	User::LeaveIfError(file.Open(iFs, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);
	
	HBufC8* imageData = HBufC8::NewLC(bufSize);
	TPtr8 imageDataPtr(imageData->Des());
	User::LeaveIfError(file.Read(imageDataPtr));

	// pad the data out otherwise the recognizer won't even look at it
    imageDataPtr.SetLength(bufSize);	

	myRecog->DoRecognizeL(fileName, imageDataPtr);
	
	if (myRecog->Confidence() != CApaDataRecognizerType::ENotRecognized)
		{
		INFO_PRINTF1(_L("    - RECICL.MDL: incorrectly recognized file type -"));
		TBuf16<KBufferSize> strReturned;
		strReturned.Copy(myRecog->MimeType().Des8());
    	INFO_PRINTF2(_L("Mime type returned = \"%S\")"), &strReturned);
		User::Leave(KErrGeneral);
		}

	CleanupStack::PopAndDestroy(3, wrapper);		// wrapper, file, imageData

	}

/**
 * 
 * Open images, retrieve frame info & verify
 * 
 */
TVerdict RRecogNegTest::DoTestStepL()
	{
	// dummy Hal call for heap balance on target  
	CICLFbsSessionTest* tempFbsSession = CICLFbsSessionTest::NewLC();
  	DummyHalCallL();
	CleanupStack::PopAndDestroy(tempFbsSession);
	
	__MM_HEAP_MARK;
	CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC();
	
	TInt noOfPlugins = iFileNames.Count();
	TInt error = KErrNone;

	for(TInt pluginNo = 0; pluginNo < noOfPlugins; pluginNo++)
		{
		INFO_PRINTF2(_L("%S"),iFileNames[pluginNo]);
		TRAP(error,VerifyMimeTypesL(*iFileNames[pluginNo]));
		Compare(error,KErrNone,_L("Unexpected error code"));
		}
	CleanupStack::PopAndDestroy(fbsSession);
	
	__MM_HEAP_MARKEND;

	return iTestStepResult;
	} 

//
// RRecogNonStandardMimeTest
RRecogNonStandardMimeTest* RRecogNonStandardMimeTest::NewL(CTestSuite* aSuite, const TDesC& aTestName)
	{
	return new(ELeave) RRecogNonStandardMimeTest(aSuite, aTestName);
	}
	
RRecogNonStandardMimeTest::RRecogNonStandardMimeTest(CTestSuite* aSuite, const TDesC& aTestName)
	{
	iSuite = aSuite;
	iTestStepName = aTestName;

	// need to up the default heap size to handle the Application Architecture Server
	// Start App server
	iHeapSize = 0x40000;	
	}

TVerdict RRecogNonStandardMimeTest::OpenL()
	{
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
            User::LeaveIfError(errCode);
        }
	// Connect to file server
	User::LeaveIfError(iFs.Connect());
	
	return iTestStepResult;
	}

void RRecogNonStandardMimeTest::Close()
	{
	iFs.Close();
	RFbsSession::Disconnect();
	delete iScheduler;	
	}
	
void RRecogNonStandardMimeTest::SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecSuite*)iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();	
	}

	
//
// RRecogNonStandardWBMTest	
RRecogNonStandardWBMTest* RRecogNonStandardWBMTest::NewL(CTestSuite* aSuite, const TDesC& aTestName)
	{
	return new(ELeave) RRecogNonStandardWBMTest(aSuite, aTestName);
	}
	
RRecogNonStandardWBMTest::RRecogNonStandardWBMTest(CTestSuite* aSuite, const TDesC& aTestName)
	{
	iSuite = aSuite;
	iTestStepName = aTestName;

	// need to up the default heap size to handle the Application Architecture Server
	// Start App server
	iHeapSize = 0x40000;	
	}

TVerdict RRecogNonStandardWBMTest::OpenL()
	{
	iTestStepResult = EPass;
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
    User::LeaveIfError(FbsStartup());
    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
            User::LeaveIfError(errCode);
        }
	// Connect to file server
	User::LeaveIfError(iFs.Connect());
	return iTestStepResult;
	}

void RRecogNonStandardWBMTest::Close()
	{
	iFs.Close();
	RFbsSession::Disconnect();
	delete iScheduler;	
	}
	
TVerdict RRecogNonStandardWBMTest::DoTestStepL()
	{
	// Assume the worst
	iTestStepResult = EFail;
  
	TRAPD(err, VerifyWBM());
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Failed to verify non-standard \"image/jpg\" MIME type, error: %d"), err);
		}
		
	return iTestStepResult;
	} 
	
void RRecogNonStandardWBMTest::VerifyWBM()
	{
	TUid aCodecUid = TUid::Uid(0);
	_LIT(aFilename,"wirelessbitmap.wbm");
	TInt theRes = KErrNone; 

	TFileName theSourceFilename = ((CICLCodecSuite*)iSuite)->DefaultPath();//dir
	theSourceFilename.Append(aFilename);//dir appends filname

	INFO_PRINTF2(_L("Opening the file : %S"), &theSourceFilename);
	
	CImageDecoder *theImageDecoder = NULL;
	
	if(aCodecUid.iUid == 0)
		{
		//There should be no reliance on RFbsSession in a call to FileNewL
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename));
		}
	else
		{
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, aCodecUid));
		}
		
	if(theRes != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open file %S"), &theSourceFilename);
		}
	else 
		{
		iTestStepResult = EPass;		
		}
		
	if (theImageDecoder)
		{
		delete theImageDecoder;
		}
	}


/**
 * Tests that the RApaLsSession supports the MIME type "image/jpg"
 * and that an image of that type can be successfully decoded.
 */
void RRecogNonStandardMimeTest::VerifyNonStandardMimeTypeL()
	{
	// Is "image/jpg" supported?
	RApaLsSession apaLsSession;
	User::LeaveIfError(apaLsSession.Connect());
	CleanupClosePushL(apaLsSession);

	CDataTypeArray* array = new (ELeave) CDataTypeArray(10);
	CleanupStack::PushL(array);
	apaLsSession.GetSupportedDataTypesL(*array);
	TInt count = array->Count();
	TDataType dataType;
	for (TInt i = 0; i < count; i++)
		{
		dataType = (*array)[i];
		if (dataType.Des8().Compare(KMimeTypeJpg) == 0)
			{
			// System supports it!
			INFO_PRINTF1(_L("System supports \"image/jpg\" MIME type"));
			iTestStepResult = EPass;
			break;
			}
		}
	CleanupStack::PopAndDestroy(2, &apaLsSession); 	// array, apaLsSession

	if (iTestStepResult == EPass)
		{
		// second part of the test: try to open / decode an image of non-standard MIME type
		CImageDecoder* decoder = NULL;
		
		TFileName fileName;
		SetTestFileNameL(fileName, KImageJpgFileName);
		
		TRAPD(err, decoder = CImageDecoder::FileNewL(iFs, fileName, KMimeTypeJpg));
		delete decoder;
		if (err != KErrNone)
			{
			iTestStepResult = EFail;
			ERR_PRINTF2(_L("Failed to open an image decoder for the \"image/jpg\" MIME type, error: %d"), err);
			}
		else
			{
			INFO_PRINTF1(_L("Successfully opened an image decoder for the \"image/jpg\" MIME type."));
			}
		}
	else
		{
		// failed the first part
		ERR_PRINTF1(_L("System doesn't support \"image/jpg\" MIME type"));
		}
	}

TVerdict RRecogNonStandardMimeTest::DoTestStepL()
	{
	// Assume the worst
	iTestStepResult = EFail;
  
	__MM_HEAP_MARK;
	
	TRAPD(err, VerifyNonStandardMimeTypeL());
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Failed to verify non-standard \"image/jpg\" MIME type, error: %d"), err);
		}

	__MM_HEAP_MARKEND;

	return iTestStepResult;
	} 
//
// RRecogNonStandardWMFTest	
RRecogNonStandardWMFTest* RRecogNonStandardWMFTest::NewL(CTestSuite* aSuite, const TDesC& aTestName)
	{
	return new(ELeave) RRecogNonStandardWMFTest(aSuite, aTestName);
	}
	
RRecogNonStandardWMFTest::RRecogNonStandardWMFTest(CTestSuite* aSuite, const TDesC& aTestName)
	{
	iSuite = aSuite;
	iTestStepName = aTestName;

	// need to up the default heap size to handle the Application Architecture Server
	// Start App server
	iHeapSize = 0x40000;	
	}

TVerdict RRecogNonStandardWMFTest::OpenL()
	{
	iTestStepResult = EPass;
	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
    User::LeaveIfError(FbsStartup());
    TInt errCode = RFbsSession::Connect();
    if( errCode != KErrNone)
        {
            User::LeaveIfError(errCode);
        }
	// Connect to file server
	User::LeaveIfError(iFs.Connect());
	return iTestStepResult;
	}

void RRecogNonStandardWMFTest::Close()
	{
	iFs.Close();
	RFbsSession::Disconnect();
	delete iScheduler;	
	}
	
TVerdict RRecogNonStandardWMFTest::DoTestStepL()
	{
	// Assume the worst
	iTestStepResult = EFail;
  
	TRAPD(err, VerifyWMFWithoutServer());
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Failed to verify non-standard \"image/jpg\" MIME type, error: %d"), err);
		}
		
	return iTestStepResult;
	} 
	
void RRecogNonStandardWMFTest::VerifyWMFWithoutServer()
	{
	TUid aCodecUid = TUid::Uid(0);
	_LIT(aFilename,"wmf2.wmf");
	TInt theRes = KErrNone; 

	TFileName theSourceFilename = ((CICLCodecSuite*)iSuite)->DefaultPath();//dir
	theSourceFilename.Append(aFilename);//dir appends filname

	INFO_PRINTF2(_L("Openning the file : %S"), &theSourceFilename);
	
	CImageDecoder *theImageDecoder = NULL;
	
	if (iTestStepName.Compare(_L("MM-ICL-COD-U-2011-HP")) == 0)
		{
		// >>>>>>>>>  WMF ONLY  <<<<<<<<<<<<<<<<
		// WMF Plugin must not rely on Session with RFbsServer being present
		RFbsSession::Disconnect(); 
		}
		
	if(aCodecUid.iUid == 0)
		{
		//There should be no reliance on RFbsSession in a call to FileNewL
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename));
		}
	else
		{
		TRAP(theRes, theImageDecoder=CImageDecoder::FileNewL(iFs, theSourceFilename, CImageDecoder::EOptionNone, aCodecUid));
		}
		
	if(theRes != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open file %S"), &theSourceFilename);
		}
	else 
		{
		iTestStepResult = EPass;		
		}

	if(theImageDecoder)
		{
		delete theImageDecoder;
		}
	}


