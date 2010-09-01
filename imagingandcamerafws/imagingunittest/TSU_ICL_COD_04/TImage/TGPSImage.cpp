// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TGPSImage.h"
#include <iclexif.h>
#include <exifutility.h>
	
// file names
_LIT(KImageTestGPSExifDecode, 	"c:\\TSU_ICL_cod_04\\DecodeGPSTestSource1.jpg");
_LIT(KImageTestGPSExifDecode2, 	"c:\\TSU_ICL_cod_04\\DecodeGPSTestSource2.jpg");
_LIT(KImageTestGPSExifDecode3, 	"c:\\TSU_ICL_cod_04\\DecodeGPSTestSource3.jpg");
_LIT(KImageTestGPSExifDecode4, 	"c:\\TSU_ICL_cod_04\\DecodeGPSTestSource4.jpg");
_LIT(KGPSEncodeSourceFileName, 	"c:\\TSU_ICL_cod_04\\EncodeTestSource.mbm");
_LIT(KImageTestExifGPSEncode, 	"c:\\TSU_ICL_cod_04\\ExifGPSEncode.jpg");
_LIT(KImageTestExifGPSTransformDest, "c:\\TSU_ICL_cod_04\\ExifGPSTransform.jpg");
_LIT(KImageTestExifGPSTransformDest2, "c:\\TSU_ICL_cod_04\\ExifGPSTransform2.jpg");
_LIT(KImageTestNoGPSTag, 	"c:\\TSU_ICL_cod_04\\TransformSource.jpg");
_LIT(KImageTestThumblessExifTransformSource,	"c:\\TSU_ICL_cod_04\\ExifNoThumb.jpg");

_LIT(KImageTestExifTransformSource, 	"c:\\TSU_ICL_cod_04\\ExifTransformSource.jpg");
_LIT(KImageTestExifTransformDest, 	"c:\\TSU_ICL_cod_04\\ExifTransform.jpg");
_LIT(KImageTestExifTransformDest2, 	"c:\\TSU_ICL_cod_04\\ExifTransform2.jpg");
_LIT(KImageTestGPSExifTransformRefTN, 	"c:\\TSU_ICL_cod_04\\ExifGPSTransformRefTN.jpg");
_LIT(KImageTestGPSExifTransformRefFN, 	"c:\\TSU_ICL_cod_04\\ExifGPSTransformRefFN.jpg");
_LIT(KImageTestGPSExifTransformRefTT, 	"c:\\TSU_ICL_cod_04\\ExifGPSTransformRefTT.jpg");
_LIT(KImageTestGPSExifTransformRefFT, 	"c:\\TSU_ICL_cod_04\\ExifGPSTransformRefFT.jpg");
_LIT(KImageTestGPSExifUnsupportedTags, 	"c:\\TSU_ICL_cod_04\\DecodeGPSUnsupportedTags.jpg");

//No code generated for any of these!
TGPSImage::TGPSImage(const TPerformanceTestParams* aPerfTstParams, const TDesC& aTestName)
	: TImage(aPerfTstParams, aTestName)
	{
	}

void TGPSImage::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iActiveListener = new (ELeave) CActiveListener;
	iImageFiles.AppendL(&KImageTestExifGPSEncode);
	iImageFiles.AppendL(&KImageTestExifGPSTransformDest);
	iImageFiles.AppendL(&KImageTestExifTransformDest);
	iImageFiles.AppendL(&KImageTestExifTransformDest2);
	iImageFiles.AppendL(&KImageTestExifGPSTransformDest);
	}

void TGPSImage::Test()
	{
	__ASSERT_ALWAYS(iActiveListener, User::Invariant());	

	TRAPD(err,TestL());
	if (err != KErrNone)
		{
		TestFailed(_L("\r\nTest failed, error code: %d"),err);
		}
	delete iActiveListener;
	iActiveListener = NULL;
	}

void TGPSImage::End()
	{
	Close();
	delete iActiveListener;
	iActiveListener = NULL;
	for(TInt i =0; i<iImageFiles.Count();i++)
		{	
		iFs.Delete(*iImageFiles[i]);
		}	
	iImageFiles.Close();
	iFs.Close();
	}

//
//
//
//		Getter tests
//

// GetGPSExifData:
// This test gets the following tag values from CJPEGExifDecoder metadata obtained from
// KImageTestGPSExifDecode:
//	0x0001 - GpsLatitudeRef
//	0x0002 - GpsLatitude
//	0x0003 - GpsLongitudeRef
//	0x0004 - GpsLongitude
//	0x0007 - GpsTimeStamp
//
// The test image is a real (non-generated) image.
void TGPSImage::GetGPSExifData(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetGPSExifData test %S"), &KImageTestGPSExifDecode);
	
	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifDecode);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();
	
	TInt finalResult = KErrNone;
		
	_LIT8(KGpsLatitudeRefTestResult, "N");
	testResult = GetGpsLatitudeRef(metaData, KGpsLatitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TRational KGpsLatitudeDegTestResult={54, 1};
	const TRational KGpsLatitudeMinTestResult={5938, 100};
	const TRational KGpsLatitudeSecTestResult={0, 1};		
	testResult = GetGpsLatitude(metaData, KGpsLatitudeDegTestResult, KGpsLatitudeMinTestResult, KGpsLatitudeSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	_LIT8(KGpsLongitudeRefTestResult, "W");	
	testResult = GetGpsLongitudeRef(metaData, KGpsLongitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TRational KGpsLongitudeDegTestResult={1, 1};
	const TRational KGpsLongitudeMinTestResult={5485, 100};
	const TRational KGpsLongitudeSecTestResult={0, 1};		
	testResult = GetGpsLongitude(metaData, KGpsLongitudeDegTestResult, KGpsLongitudeMinTestResult, KGpsLongitudeSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TRational KGpsTimeStampHourTestResult={14, 1};
	const TRational KGpsTimeStampMinTestResult={58, 1};
	const TRational KGpsTimeStampSecTestResult={24, 1};		
	testResult = GetGpsTimeStamp(metaData, KGpsTimeStampHourTestResult, KGpsTimeStampMinTestResult, KGpsTimeStampSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}	

	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	}

// GetGPSExifData2:
// This test gets the following tag values from CJPEGExifDecoder metadata obtained from
// KImageTestGPSExifDecode2:
//	0x0000 - GpsVersionId
//	0x0005 - GpsAltitudeRef
//	0x0006 - GpsAltitude
//	0x0010 - GetGpsImgDirectionRef
//	0x0011 - GetGpsImgDirection
//	0x0012 - GpsMapDatum
//
// The test image is a real (non-generated) image.
void TGPSImage::GetGPSExifData2(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetGPSExifData2 test %S"), &KImageTestGPSExifDecode2);
	
	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifDecode2);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();
	
	TInt finalResult = KErrNone;
	
	_LIT8(KGpsVersionTestResult, "\2\0\0\0");
	testResult = GetGpsVersionId(metaData, KGpsVersionTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TUint8 KGpsAltitudeRefTestResult = 0;
	testResult = GetGpsAltitudeRef(metaData, KGpsAltitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TInt KGpsAltitudeNumeratorTestResult=935;
	const TInt KGpsAltitudeDenominatorTestResult=1;
	testResult = GetGpsAltitude(metaData, KGpsAltitudeNumeratorTestResult, KGpsAltitudeDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsImgDirectionRefTestResult, "T");
	testResult = GetGpsImgDirectionRef(metaData, KGpsImgDirectionRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TInt KGpsImgDirectionNumeratorTestResult=1000;
	const TInt KGpsImgDirectionDenominatorTestResult=100;
	testResult = GetGpsImgDirection(metaData, KGpsImgDirectionNumeratorTestResult, KGpsImgDirectionDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsMapDatumTestResult, "WGS84");
	testResult = GetGpsMapDatum(metaData, KGpsMapDatumTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
				
	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	}
	
// GetGPSExifData3:
// This test gets the following tag values from CJPEGExifDecoder metadata obtained from
// KImageTestGPSExifDecode3:
//	0x0009 - GpsStatus
//	0x000B - GpsDop
//	0x000C - GpsSpeedRef
//	0x000D - GpsSpeed
//	0x000E - GpsTrackRef
//	0x000F - GpsTrack
//
// The test image is a real (non-generated) Motorolla/Big-Endian image.
void TGPSImage::GetGPSExifData3(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetGPSExifData3 test %S (Motorolla/big endian)"), &KImageTestGPSExifDecode3);

	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifDecode3);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();
	
	TInt finalResult = KErrNone;
	
	// GetGpsStatus implicitly tests DEF082159 - it reads a big endian ascii
	// string of two bytes.
	_LIT8(KGpsStatusTestResult, "A");
	testResult = GetGpsStatus(metaData, KGpsStatusTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
	
	const TInt KGpsDopNumeratorTestResult=272;
	const TInt KGpsDopDenominatorTestResult=10;	
	testResult = GetGpsDop(metaData, KGpsDopNumeratorTestResult, KGpsDopDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsSpeedRefTestResult, "K");
	testResult = GetGpsSpeedRef(metaData, KGpsSpeedRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TInt KGpsSpeedNumeratorTestResult=0;
	const TInt KGpsSpeedDenominatorTestResult=1;
	testResult = GetGpsSpeed(metaData, KGpsSpeedNumeratorTestResult, KGpsSpeedDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	_LIT8(KGpsTrackRefTestResult, "T");		
	testResult = GetGpsTrackRef(metaData, KGpsTrackRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TInt KGpsTrackNumeratorTestResult=0;
	const TInt KGpsTrackDenominatorTestResult=100;	
	testResult = GetGpsTrack(metaData, KGpsTrackNumeratorTestResult, KGpsTrackDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
			
	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	}
	
// GetGPSExifData4:
// This test gets the following tag values from CJPEGExifDecoder metadata obtained from
// KImageTestGPSExifDecode4:
//	0x0008 - GpsSatellites
//	0x000A - GpsMeasureMode
//
// The test image is a real (non-generated) image.
void TGPSImage::GetGPSExifData4(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetGPSExifData4 test %S"), &KImageTestGPSExifDecode4);
	
	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifDecode4);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();
	
	TInt finalResult = KErrNone;
	
	_LIT8(KGpsSatellitesTestResult, "22,15,09,,,,,,,,,                              ");
	testResult = GetGpsSatellites(metaData, KGpsSatellitesTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsMeasureModeTestResult, "2");
	testResult = GetGpsMeasureMode(metaData, KGpsMeasureModeTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
			
	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	}

// GetGPSExifData5:
// This test gets the "Dest" tag values from CJPEGExifDecoder metadata obtained from
// KImageTestGPSExifTransformRefTT.  That is, the values for tags:
//	0x0013 - GpsDestLatitudeRef
//	0x0014 - GpsDestLatitude
//	0x0015 - GpsDestLongitudeRef
//	0x0016 - GpsDestLongitude
//	0x0017 - GpsDestBearingRef
//	0x0018 - GpsDestBearing
//	0x0019 - GpsDestDistanceRef
//	0x001A - GpsDestDistanceRef
//
// The test image is a generated reference image.	
void TGPSImage::GetGPSExifData5(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetGPSExifData 5 test %S"), &KImageTestGPSExifTransformRefTT);
	
	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifTransformRefTT);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();
	
	// Gets the "Dest" tags values
	testResult=TestGpsDestTagGetters(metaData);
					
	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		Log the result
	//
	}

// GetGPSExifData6:
// This test gets the values of the tags specific to Exif ver2.2 from CJPEGExifDecoder 
// metadata obtained from KImageTestGPSExifTransformRefTT.  That is, the values 
// for tags:
//	0x001B - GpsProcessingMethod
//	0x001C - GpsAreaInformation
//	0x001D - GpsDateStamp
//	0x001E - GpsDifferential
//
// The test image is a generated reference image.
void TGPSImage::GetGPSExifData6(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetGPSExifData6 test %S"), &KImageTestGPSExifTransformRefTT);
	
	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifTransformRefTT);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();
	
	// Gets the GPS tags specific to the Exif 2.2 Spec.
	testResult=TestGpsVer22TagGetters(metaData);
			
	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		Log the result
	//
	}


// GetGPSTransformExif:
// This test gets the following tag values from CImageTransform metadata obtained from
// KImageTestGPSExifDecode2 (KImageTestGPSExifDecode1 is not used as this is non 
// conformant having both a APP0 & APP1 marker):
//	0x0000 - GpsVersionId
//	0x0005 - GpsAltitudeRef
//	0x0006 - GpsAltitude
//	0x0010 - GpsImgDirectionRef
//	0x0011 - GpsImgDirection
//	0X0012 - GpsMapDatum
// The test image is a real (non-generated) image.
void TGPSImage::GetGPSTransformExif(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform GPS IFD Getters test1 --- %S"),&KImageTestGPSExifDecode2);

	__MM_HEAP_MARK;
	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the metadata
	//
	
	//
	//		Test the data
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	_LIT8(KGpsVersionTestResult, "\2\0\0\0");
	testResult = GetGpsVersionId(metaData, KGpsVersionTestResult);
	if (testResult != KErrNone)
		{
		finalResult=testResult;
		}
		
	const TUint8 KGpsAltitudeRefTestResult = 0;
	testResult = GetGpsAltitudeRef(metaData, KGpsAltitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult=testResult;
		}		

	const TInt KGpsAltitudeNumeratorTestResult=935;
	const TInt KGpsAltitudeDenominatorTestResult=1;
	testResult = GetGpsAltitude(metaData, KGpsAltitudeNumeratorTestResult, KGpsAltitudeDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult=testResult;
		}
		

	_LIT8(KGpsImgDirectionRefTestResult, "T");
	testResult = GetGpsImgDirectionRef(metaData, KGpsImgDirectionRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult=testResult;
		}
		
	const TInt KGpsImgDirectionNumeratorTestResult=1000;
	const TInt KGpsImgDirectionDenominatorTestResult=100;
	testResult = GetGpsImgDirection(metaData, KGpsImgDirectionNumeratorTestResult, KGpsImgDirectionDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult=testResult;
		}
		
	_LIT8(KGpsMapDatumTestResult, "WGS84");
	testResult = GetGpsMapDatum(metaData, KGpsMapDatumTestResult);
	if (testResult != KErrNone)
		{
		finalResult=testResult;
		}
	
	delete imageTransform;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	__MM_HEAP_MARKEND;
	}

// GetGPSTransformExif2:
// This test gets the following tag values from CImageTransform metadata obtained from
// KImageTestGPSExifDecode3:
//	0x0001 - GpsLatitudeRef
//	0x0002 - GpsLatitude
//	0x0003 - GpsLongitudeRef
//	0x0004 - GpsLongitude
//	0x0007 - GpsTimeStamp
//	0x0009 - GpsStatus
//	0x000B - GpsDop
//	0x000C - GpsSpeedRef
//	0x000D - GpsSpeed
//	0x000E - GpsTrackRef
//	0x000F - GpsTrack
//
// Note: DEF082159 is implicitly tested in this test step.
//
// The test image is a real (non-generated) image.
void TGPSImage::GetGPSTransformExif2(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform GPS IFD Getters test2 --- %S"),&KImageTestGPSExifDecode3);

	__MM_HEAP_MARK;
	//
	//		Get the metadata
	
	// This image uses Motorolla (big endian) encoding
	TFileName sourceFileName(KImageTestGPSExifDecode3);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the metadata
	//
	
	//
	//		Test the data
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	_LIT8(KGpsLatitudeRefTestResult, "N");
	testResult = GetGpsLatitudeRef(metaData, KGpsLatitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TRational KGpsLatitudeDegTestResult={35, 1};
	const TRational KGpsLatitudeMinTestResult={1, 1};
	const TRational KGpsLatitudeSecTestResult={9, 10};		
	testResult = GetGpsLatitude(metaData, KGpsLatitudeDegTestResult, KGpsLatitudeMinTestResult, KGpsLatitudeSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	_LIT8(KGpsLongitudeRefTestResult, "E");	
	testResult = GetGpsLongitudeRef(metaData, KGpsLongitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TRational KGpsLongitudeDegTestResult={138, 1};
	const TRational KGpsLongitudeMinTestResult={48, 1};
	const TRational KGpsLongitudeSecTestResult={411, 10};		
	testResult = GetGpsLongitude(metaData, KGpsLongitudeDegTestResult, KGpsLongitudeMinTestResult, KGpsLongitudeSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TRational KGpsTimeStampHourTestResult={0, 1};
	const TRational KGpsTimeStampMinTestResult={39, 1};
	const TRational KGpsTimeStampSecTestResult={16, 1};		
	testResult = GetGpsTimeStamp(metaData, KGpsTimeStampHourTestResult, KGpsTimeStampMinTestResult, KGpsTimeStampSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}	
	
	// GetGpsStatus implicitly tests DEF082159 - it reads a big endian ascii
	// string of two bytes.
	_LIT8(KGpsStatusTestResult, "A");
	testResult = GetGpsStatus(metaData, KGpsStatusTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
	
	const TInt KGpsDopNumeratorTestResult=272;
	const TInt KGpsDopDenominatorTestResult=10;	
	testResult = GetGpsDop(metaData, KGpsDopNumeratorTestResult, KGpsDopDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsSpeedRefTestResult, "K");
	testResult = GetGpsSpeedRef(metaData, KGpsSpeedRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TInt KGpsSpeedNumeratorTestResult=0;
	const TInt KGpsSpeedDenominatorTestResult=1;
	testResult = GetGpsSpeed(metaData, KGpsSpeedNumeratorTestResult, KGpsSpeedDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	_LIT8(KGpsTrackRefTestResult, "T");		
	testResult = GetGpsTrackRef(metaData, KGpsTrackRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	const TInt KGpsTrackNumeratorTestResult=0;
	const TInt KGpsTrackDenominatorTestResult=100;	
	testResult = GetGpsTrack(metaData, KGpsTrackNumeratorTestResult, KGpsTrackDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	delete imageTransform;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	__MM_HEAP_MARKEND;
	}

// GetGPSTransformExif3:
// This test gets the following tag values from CImageTransform metadata obtained from
// KImageTestGPSExifDecode4:
//	0x0008 - GpsSatellites
//	0x000A - GpsMeasureMode
//
// The test image is a real (non-generated) image.
void TGPSImage::GetGPSTransformExif3(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform GPS IFD Getters test3 --- %S"),&KImageTestGPSExifDecode4);

	__MM_HEAP_MARK;
	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestGPSExifDecode4);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the metadata
	//
	
	//
	//		Test the data
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	_LIT8(KGpsSatellitesTestResult, "22,15,09,,,,,,,,,                              ");
	testResult = GetGpsSatellites(metaData, KGpsSatellitesTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsMeasureModeTestResult, "2");
	testResult = GetGpsMeasureMode(metaData, KGpsMeasureModeTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
	
	delete imageTransform;
	//		Test the metadata
	//

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	__MM_HEAP_MARKEND;
	}

// GetGPSTransformExif4:
// This test gets the "Dest" tag values from CImageTransform metadata obtained from
// KImageTestGPSExifTransformRefTT.  That is, the values for tags:
//	0x0013 - GpsDestLatitudeRef
//	0x0014 - GpsDestLatitude
//	0x0015 - GpsDestLongitudeRef
//	0x0016 - GpsDestLongitude
//	0x0017 - GpsDestBearingRef
//	0x0018 - GpsDestBearing
//	0x0019 - GpsDestDistanceRef
//	0x001A - GpsDestDistanceRef
//
// The test image is a generated reference image.
void TGPSImage::GetGPSTransformExif4(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform GPS IFD Getters test 4 --- %S"),&KImageTestGPSExifTransformRefTT);

	__MM_HEAP_MARK;
	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestGPSExifTransformRefTT);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the metadata
	//
	
	//
	//		Test the metadata
	
	TInt testResult=TestGpsDestTagGetters(metaData);
			
	delete imageTransform;
	//		Test the metadata
	//

	//
	//		Log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		Log the result
	//
	__MM_HEAP_MARKEND;
	}

// GetGPSTransformExif5:
// This test gets the values of the tags specific to Exif ver2.2 from CImageTransform 
// metadata obtained from KImageTestGPSExifTransformRefTT.  That is, the values 
// for tags:
//	0x001B - GpsProcessingMethod
//	0x001C - GpsAreaInformation
//	0x001D - GpsDateStamp
//	0x001E - GpsDifferential
//
// The test image is a generated reference image.
void TGPSImage::GetGPSTransformExif5(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform GPS IFD Getters test5 --- %S"),&KImageTestGPSExifTransformRefTT);

	__MM_HEAP_MARK;
	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestGPSExifTransformRefTT);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the metadata
	//
	
	//
	//		Test the data
	// Gets the GPS tags specific to the Exif 2.2 Spec.
	TInt testResult=TestGpsVer22TagGetters(metaData);
			
	delete imageTransform;
	//		Test the metadata
	//

	//
	//		Log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		Log the result
	//
	__MM_HEAP_MARKEND;
	}

//		
//
// Getter Utility Methods
//
//

TInt TGPSImage::GetGpsVersionId(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	
	
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsVersionId(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GpsVersionId"));
			}
		else
			{
			Printf(_L("Get GPS Version Id succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Version Id %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
	
TInt TGPSImage::GetGpsLatitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsLatitudeRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Latitude Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Latitude Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Lat Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}

	
TInt TGPSImage::GetGpsLatitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected)
	{	
	TInt degNumerator=0;
	TInt degDenominator=0;
	TInt minNumerator=0;
	TInt minDenominator=0;
	TInt secNumerator=0;
	TInt secDenominator=0;
		
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsLatitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);

	if(err==KErrNone)
		{
		if(	 (degNumerator != aDegExpected.iNumerator)		||
			 (degDenominator != aDegExpected.iDenominator)	||
			 (minNumerator != aMinExpected.iNumerator)		||
			 (minDenominator != aMinExpected.iDenominator)	||
			 (secNumerator != aSecExpected.iNumerator)		||
			 (secDenominator != aSecExpected.iDenominator)  )	
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Latitude Ref"));
			}
		else
			{
			Printf(_L("Get GPS Latitude succeeded"));
			}	
		}

	return err;
	}
	
TInt TGPSImage::GetGpsLongitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	
	
	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsLongitudeRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Longitude Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Longitude Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Longitude Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsLongitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected)
	{
	TInt degNumerator=0;
	TInt degDenominator=0;
	TInt minNumerator=0;
	TInt minDenominator=0;
	TInt secNumerator=0;
	TInt secDenominator=0;
		
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsLongitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);

	if(err==KErrNone)
		{
		if(	 (degNumerator != aDegExpected.iNumerator)		||
			 (degDenominator != aDegExpected.iDenominator)	||
			 (minNumerator != aMinExpected.iNumerator)		||
			 (minDenominator != aMinExpected.iDenominator)	||
			 (secNumerator != aSecExpected.iNumerator)		||
			 (secDenominator != aSecExpected.iDenominator)  )	
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Longitude Ref"));
			}
		else
			{
			Printf(_L("Get GPS Longitude succeeded"));
			}	
		}

	return err;
	}
	

TInt TGPSImage::GetGpsAltitudeRef(MExifMetadata* aMetaData, const TUint8 aExpected)
	{
	TUint8 altRef = 0;	
	
	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsAltitudeRef(altRef);
	
	// test the result
	if (err==KErrNone)
		{
		if (altRef != aExpected)
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Altitude Ref %d"), altRef);
			}
		else
			{
			Printf(_L("Get GPS Altitude Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Altitude Ref Id %d"), err);
		}		
	return err;	
	}

	
TInt TGPSImage::GetGpsAltitude(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	TInt valueNum=0;
	TInt valueDen=0;
	
	// get the data
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsAltitude(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Altitude"));
		}
	else
		{
		Printf(_L("Get GPS Altitude succeeded "));
		}
		
	return err;
	}	

TInt TGPSImage::GetGpsTimeStamp(MExifMetadata* aMetaData, const TRational& aHourExpected, const TRational& aMinExpected, const TRational& aSecExpected)
	{	
	TInt hourNumerator=0;
	TInt hourDenominator=0;
	TInt minNumerator=0;
	TInt minDenominator=0;
	TInt secNumerator=0;
	TInt secDenominator=0;
		
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsTimeStamp(hourNumerator,hourDenominator,minNumerator,minDenominator,secNumerator,secDenominator);

	if(err==KErrNone)
		{
		if(	 (hourNumerator != aHourExpected.iNumerator)	||
			 (hourDenominator != aHourExpected.iDenominator)||
			 (minNumerator != aMinExpected.iNumerator)		||
			 (minDenominator != aMinExpected.iDenominator)	||
			 (secNumerator != aSecExpected.iNumerator)		||
			 (secDenominator != aSecExpected.iDenominator)  )	
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Timestamp"));
			}
		else
			{
			Printf(_L("Get GPS Timestamp succeeded"));
			}	
		}

	return err;
	}
	
TInt TGPSImage::GetGpsSatellites(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	
	
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsSatellites(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Satellites %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Satellites succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Satellites %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsStatus(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	
	
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsStatus(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Status %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Status succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Status %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}

TInt TGPSImage::GetGpsMeasureMode(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsMeasureMode(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Measure Mode %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Measure Mode succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Measure Mode %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsDop(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	// get the data
	TInt valueNum=0;
	TInt valueDen=0;
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsDop(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Dop"));
		}
	else
		{
		Printf(_L("Get GPS Dop succeeded "));
		}
		
	return err;
	}

TInt TGPSImage::GetGpsSpeedRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsSpeedRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Speed Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Speed Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Speed Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsSpeed(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	// get the data
	TInt valueNum=0;
	TInt valueDen=0;
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsSpeed(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Speed"));
		}
	else
		{
		Printf(_L("Get GPS Speed succeeded "));
		}
		
	return err;
	}
	
TInt TGPSImage::GetGpsTrackRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsTrackRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Track Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Track Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Track Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsTrack(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	// get the data
	TInt valueNum=0;
	TInt valueDen=0;
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsTrack(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Track"));
		}
	else
		{
		Printf(_L("Get GPS Track succeeded "));
		}
		
	return err;
	}

TInt TGPSImage::GetGpsImgDirectionRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsImgDirectionRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Img Direction Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Direction Ref Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Direction ref Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	

TInt TGPSImage::GetGpsImgDirection(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	// get the data
	TInt valueNum=0;
	TInt valueDen=0;
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsImgDirection(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Img Direction"));
		}
	else
		{
		Printf(_L("Get GPS Img Direction succeeded "));
		}
		
	return err;
	}
		
TInt TGPSImage::GetGpsMapDatum(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsMapDatum(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Map Datum %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Map Datum succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Map Datum %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}

TInt TGPSImage::GetGpsDestLatitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsDestLatitudeRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Dest Latitude Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Dest Latitude Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Dest Latitude Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	

TInt TGPSImage::GetGpsDestLatitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected)
	{
	TInt degNumerator=0;
	TInt degDenominator=0;
	TInt minNumerator=0;
	TInt minDenominator=0;
	TInt secNumerator=0;
	TInt secDenominator=0;
		
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsDestLatitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);

	if(err==KErrNone)
		{
		if(	 (degNumerator != aDegExpected.iNumerator)		||
			 (degDenominator != aDegExpected.iDenominator)	||
			 (minNumerator != aMinExpected.iNumerator)		||
			 (minDenominator != aMinExpected.iDenominator)	||
			 (secNumerator != aSecExpected.iNumerator)		||
			 (secDenominator != aSecExpected.iDenominator)  )	
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Des Latitude"));
			}
		else
			{
			Printf(_L("Get GPS Des Latitude succeeded"));
			}	
		}

	return err;
	}
	
TInt TGPSImage::GetGpsDestLongitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsDestLongitudeRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Dest Longitude Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Dest Longitude Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Dest Longitude Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	

TInt TGPSImage::GetGpsDestLongitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected)
	{
	TInt degNumerator=0;
	TInt degDenominator=0;
	TInt minNumerator=0;
	TInt minDenominator=0;
	TInt secNumerator=0;
	TInt secDenominator=0;
		
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsDestLongitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);

	if(err==KErrNone)
		{
		if(	 (degNumerator != aDegExpected.iNumerator)		||
			 (degDenominator != aDegExpected.iDenominator)	||
			 (minNumerator != aMinExpected.iNumerator)		||
			 (minDenominator != aMinExpected.iDenominator)	||
			 (secNumerator != aSecExpected.iNumerator)		||
			 (secDenominator != aSecExpected.iDenominator)  )	
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect Gps Dest Longitude"));
			}
		else
			{
			Printf(_L("Get GPS Dest Longitude succeeded"));
			}	
		}

	return err;
	}
	
TInt TGPSImage::GetGpsDestBearingRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsDestBearingRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Dest Bearing Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Dest Bearing Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Dest Bearing Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	

TInt TGPSImage::GetGpsDestBearing(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	// get the data
	TInt valueNum=0;
	TInt valueDen=0;
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsDestBearing(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Dest Bearing"));
		}
	else
		{
		Printf(_L("Get GPS Img Dest Bearing"));
		}
		
	return err;
	}
	
TInt TGPSImage::GetGpsDestDistanceRef(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsDestDistanceRef(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Dest Distance Ref %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Dest Distance Ref succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Dest Distance Ref %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	

TInt TGPSImage::GetGpsDestDistance(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected)
	{
	// get the data
	TInt valueNum=0;
	TInt valueDen=0;
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetGpsDestDistance(valueNum, valueDen);
	
	// test the result
	if( (err==KErrNone) && ((valueNum!=aNumExpected) || (valueDen!=aDemExpected)) )
		{
		err=KErrCorrupt;
		Printf(_L("Incorrect GPS Dest Distance"));
		}
	else
		{
		Printf(_L("Get GPS Dest Distance succeeded"));
		}
		
	return err;
	}
	
TInt TGPSImage::GetGpsProcessingMethod(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsProcessingMethod(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Processing Method %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Processing Method succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Processing Method %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsAreaInformation(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsAreaInformation(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Area Information %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Area Information succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Area Information %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsDateStamp(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	

	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsDateStamp(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Date Stamp %S"), buffer8Bit);
			}
		else
			{
			Printf(_L("Get GPS Date Stamp succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Date Stamp %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
TInt TGPSImage::GetGpsDifferential(MExifMetadata* aMetaData, const TUint16 aExpected)
	{
	TUint16 dx = 0;
	TExifReaderUtility reader(aMetaData);	
	TInt err=reader.GetGpsDifferential(dx);
	
	// test the result
	if (err==KErrNone)
		{
		if (dx != aExpected)
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect GPS Differential %d"), dx);
			}
		else
			{
			Printf(_L("Get GPS Differential successful"));
			}
		}
	else
		{
		Printf(_L("Failed to get GPS Differential %d"), err);
		}
		
	return err;	
	}

TInt TGPSImage::GetExifVersionId(MExifMetadata* aMetaData, const TDesC8& aExpected)
	{
	HBufC8* buffer8Bit=NULL;	
	
	TExifReaderUtility reader(aMetaData);
	TInt err=reader.GetExifVersion(buffer8Bit);
	
	// test the result
	if (err==KErrNone)
		{
		if (buffer8Bit->Compare(aExpected))
			{
			err=KErrCorrupt;
			Printf(_L("Incorrect ExifVersionId"));
			}
		else
			{
			Printf(_L("Get Exif Version Id succeeded"));
			}
		}
	else
		{
		Printf(_L("Failed to get Exif Version Id %d"), err);
		}
		
	delete buffer8Bit;
		
	return err;	
	}
	
//
//
//
//		Setter tests
//

//
// AddGPSExifData:
//
// Tests adding every GPS tag to metadata from CJPEGExifEncoder.
//
void TGPSImage::AddGPSExifData(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- AddGPSExifData test--- %S"), &KImageTestExifGPSEncode);

	//
	//		Get the exif metadata
	TFileName fileName(KImageTestExifGPSEncode);
	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(testResult, CreateEncoderFromReferenceBitmapFileL(KGPSEncodeSourceFileName, bitmap, fileName, encoder));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder from reference bitmap %S"), &KGPSEncodeSourceFileName);
		TestFailed(testResult);
		return;
		}	
	CleanupStack::PushL(bitmap);
	CleanupStack::PushL(encoder);

	MExifMetadata* metaData=encoder->ExifMetadata();
	//		Get the exif metadata
	//

	//
	//		encode the image
	// This adds new gps tags to the metaData.
	testResult = TestSetGPSData(EAddData, metaData);
	if(testResult!=KErrNone)
		{
		Printf(_L("unable to add data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	//encoder
		return;
		}

	// launch the conversion
	testResult=DoImageEncode(encoder, *bitmap);
	CleanupStack::PopAndDestroy(2, bitmap); //encoder
	if(testResult!=KErrNone)
		{
		Printf(_L("Encoding failed"));
		TestFailed(testResult);
		return;
		}
	metaData=NULL;	
	//		encode the image
	//

	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);
		
	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}

//
// SetGPSExifData:
//
// Tests setting (i.e. overwritting) every GPS tag to metadata from CJPEGExifEncoder.
//
void TGPSImage::SetGPSExifData(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- SetGPSExifData test--- %S"), &KImageTestExifGPSEncode);

	//
	//		Get the exif metadata
	TFileName fileName(KImageTestExifGPSEncode);
	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(testResult, CreateEncoderFromReferenceBitmapFileL(KGPSEncodeSourceFileName, bitmap, fileName, encoder));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder from reference bitmap %S"), &KGPSEncodeSourceFileName);
		TestFailed(testResult);
		return;
		}	
	CleanupStack::PushL(bitmap);
	CleanupStack::PushL(encoder);

	MExifMetadata* metaData=encoder->ExifMetadata();
	//		Get the exif metadata
	//

	//
	//		encode the image
	//
	// First add new gps tags to the metaData.
	testResult = TestSetGPSData(EAddData, metaData);	
	if(testResult!=KErrNone)
		{
		Printf(_L("unable to add data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	//encoder
		return;
		}

	// Next set the gps tags.
	// This will overwrite existing gps tags in the metaData.
	testResult = TestSetGPSData(ESetData, metaData);	
	if(testResult!=KErrNone)
		{
		Printf(_L("unable to set data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	//encoder
		return;
		}

	// launch the conversion
	testResult=DoImageEncode(encoder, *bitmap);
	CleanupStack::PopAndDestroy(2, bitmap); //encoder
	if(testResult!=KErrNone)
		{
		Printf(_L("Encoding failed"));
		TestFailed(testResult);
		return;
		}
	metaData=NULL;
	//		encode the image
	//

	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);
		
	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}

//
// SetGPSTransformExif:
//
// Tests setting (i.e. overwritting) every GPS tag to metadata from CImageTransform.
//
void TGPSImage::SetGPSTransformExif(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Setters test --- %S"), &KImageTestGPSExifDecode2);

	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();


	//
	//		encode the image
	// Test the data
	TInt testResult=TestSetGPSData(ESetData, metaData);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);

	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}

//
// AddGPSTransformExif:
//
// Tests adding every GPS tag to metadata from CImageTransform.
//
void TGPSImage::AddGPSTransformExif(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Adders test --- %S"), &KImageTestExifTransformSource);

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;	
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();

	//
	//		encode the image
	TInt testResult=TestSetGPSData(EAddData, metaData);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while adding data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	// launch the conversion
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;		
		}
	//		encode the image
	//

	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);

	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
//
//
//
// Setter Utility Methods
//
//
//

// Note: This method implicitly tests
// DEF082164: Exif - Setting BYTE formatted tags not handled correctly
TInt TGPSImage::SetGpsVersionId(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource1.jpg
	_LIT8(K8BitBufferTestResult, "\2\2\0\0");
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{
		// set the data in the encoder
		HBufC8* buffer=K8BitBufferTestResult().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult=writer.SetGpsVersionId(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetGpsVersionId(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsLatitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource1.jpg
	_LIT8(K8BitBufferTestResult, "N");
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsLatitudeRef, "N\0");
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsLatitudeRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsLatitudeRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsLatitudeRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsLatitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource1.jpg
	TInt KDegNumerator = 54;
	TInt KDegDenominator = 1;
	TInt KMinNumerator = 5938;
	TInt KMinDenominator = 100;
	TInt KSecNumerator = 0;
	TInt KSecDenominator = 1;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsLatitude(KDegNumerator,KDegDenominator,KMinNumerator,KMinDenominator,KSecNumerator,KSecDenominator);
		}
	else
		{	
		// get the encoded data
		TInt degNumerator=0;
		TInt degDenominator=0;
		TInt minNumerator=0;
		TInt minDenominator=0;
		TInt secNumerator=0;
		TInt secDenominator=0;
			
		TExifReaderUtility reader(aMetaData);
		testResult = reader.GetGpsLatitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);
		if(testResult==KErrNone)
			{
			if(	 (degNumerator != KDegNumerator)		||
				 (degDenominator != KDegDenominator)	||
				 (minNumerator != KMinNumerator)		||
				 (minDenominator != KMinDenominator)	||
				 (secNumerator != KSecNumerator)		||
				 (secDenominator != KSecDenominator)  )	
				{
				testResult = KErrCorrupt;
				}

			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsLongitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result:
	_LIT8(K8BitBufferTestResult, "E");
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsLongitudeRef, "E\0");
		
	TInt testResult=KErrNone; 

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsLongitudeRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsLongitudeRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsLongitudeRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsLongitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{	
	// Expected result: DecodeGPSTestSource1.jpg
	TInt KDegNumerator = 12;
	TInt KDegDenominator = 1;
	TInt KMinNumerator = 3456;
	TInt KMinDenominator = 100;
	TInt KSecNumerator = 0;
	TInt KSecDenominator = 1;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsLongitude(KDegNumerator,KDegDenominator,KMinNumerator,KMinDenominator,KSecNumerator,KSecDenominator);
		}
	else
		{	
		// get the encoded data
		TInt degNumerator=0;
		TInt degDenominator=0;
		TInt minNumerator=0;
		TInt minDenominator=0;
		TInt secNumerator=0;
		TInt secDenominator=0;
			
		TExifReaderUtility reader(aMetaData);
		testResult = reader.GetGpsLongitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);
		if(testResult==KErrNone)
			{
			if(	 (degNumerator != KDegNumerator)		||
				 (degDenominator != KDegDenominator)	||
				 (minNumerator != KMinNumerator)		||
				 (minDenominator != KMinDenominator)	||
				 (secNumerator != KSecNumerator)		||
				 (secDenominator != KSecDenominator)  )	
				{
				testResult = KErrCorrupt;
				}

			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsAltitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource2.jpg
	const TUint8 KAltRefTestResult = 1; //sea level reference
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsAltitudeRef(KAltRefTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TUint8 altRef=0;
	testResult = reader.GetGpsAltitudeRef(altRef);
	if(testResult==KErrNone)
		{
		if(altRef!=KAltRefTestResult)
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}

TInt TGPSImage::SetGpsAltitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource2.jpg
	const TInt KRationalNumeratorTestResult=1234;
	const TInt KRationalDenominatorTestResult=5678;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsAltitude(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsAltitude(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsTimeStamp(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource1.jpg
	TInt KHourNumerator = 0;
	TInt KHourDenominator = 10;
	TInt KMinNumerator = 0;
	TInt KMinDenominator = 45;
	TInt KSecNumerator = 0;
	TInt KSecDenominator = 55;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsTimeStamp(KHourNumerator,KHourDenominator,KMinNumerator,KMinDenominator,KSecNumerator,KSecDenominator);
		}
	else
		{	
		// get the encoded data
		TInt hourNumerator=0;
		TInt hourDenominator=0;
		TInt minNumerator=0;
		TInt minDenominator=0;
		TInt secNumerator=0;
		TInt secDenominator=0;
			
		TExifReaderUtility reader(aMetaData);
		testResult = reader.GetGpsTimeStamp(hourNumerator,hourDenominator,minNumerator,minDenominator,secNumerator,secDenominator);
		if(testResult==KErrNone)
			{
			if(	 (hourNumerator != KHourNumerator)		||
				 (hourDenominator != KHourDenominator)	||
				 (minNumerator != KMinNumerator)		||
				 (minDenominator != KMinDenominator)	||
				 (secNumerator != KSecNumerator)		||
				 (secDenominator != KSecDenominator)  )	
				{
				testResult = KErrCorrupt;
				}

			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsSatellites(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource4.jpg
	_LIT8(KSatelliteInfo, "Satellite Info"); //22,15,09,,,,,,,                              ");
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsSatellites, "Satellite Info"); //22,15,09,,,,,,,                              \0"); 
		
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsSatellites().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsSatellites(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsSatellites(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(KSatelliteInfo))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;
	return testResult;
	}
	
TInt TGPSImage::SetGpsStatus(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource3.jpg
	_LIT8(K8BitBufferTestResult, "A");
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsStatus, "A\0"); 
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	// set the data in the encoder
		HBufC8* buffer=KGpsStatus().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsStatus(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsStatus(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;
	return testResult;
	}
	
TInt TGPSImage::SetGpsMeasureMode(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource4.jpg
	_LIT8(K8BitBufferTestResult, "3"); //3D measurement
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsMeasureMode, "3\0"); 
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsMeasureMode().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);	
		testResult = writer.SetGpsMeasureMode(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;	
	testResult = reader.GetGpsMeasureMode(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsDop(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource3.jpg
	const TInt KRationalNumeratorTestResult=5678;
	const TInt KRationalDenominatorTestResult=1234;
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsDop(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsDop(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}	

TInt TGPSImage::SetGpsSpeedRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource3.jpg
	_LIT8(K8BitBufferTestResult, "M"); //mph
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsSpeedRef, "M\0"); 
		
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsSpeedRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsSpeedRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetGpsSpeedRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;		
	return testResult;
	}
	
TInt TGPSImage::SetGpsSpeed(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource3.jpg
	const TInt KRationalNumeratorTestResult=70;
	const TInt KRationalDenominatorTestResult=5;

	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsSpeed(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsSpeed(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsTrackRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource3.jpg
	_LIT8(K8BitBufferTestResult, "T"); //true direction
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsTrackRef, "T\0");
	
	TInt testResult=KErrNone; 

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsTrackRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsTrackRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;	
	testResult = reader.GetGpsTrackRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;
	return testResult;
	}
	
TInt TGPSImage::SetGpsTrack(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource3.jpg
	const TInt KRationalNumeratorTestResult=359;
	const TInt KRationalDenominatorTestResult=99;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsTrack(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsTrack(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsImgDirectionRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource2.jpg
	_LIT8(K8BitBufferTestResult, "M"); //magnetic direction
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsImgDirectionRef, "M\0"); 
		
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsImgDirectionRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsImgDirectionRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetGpsImgDirectionRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}

TInt TGPSImage::SetGpsImgDirection(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource2.jpg
	const TInt KRationalNumeratorTestResult=180;
	const TInt KRationalDenominatorTestResult=30;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsImgDirection(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsImgDirection(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsMapDatum(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: DecodeGPSTestSource2.jpg
	_LIT8(KMapDatum, "TOKYO");
		
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KMapDatum().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsMapDatum(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetGpsMapDatum(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(KMapDatum))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestLatitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	_LIT8(K8BitBufferTestResult, "S");//south
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsDestLatitudeRef, "S\0"); 
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsDestLatitudeRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDestLatitudeRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsDestLatitudeRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestLatitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	TInt KDegNumerator = 30;
	TInt KDegDenominator = 1;
	TInt KMinNumerator = 5938;
	TInt KMinDenominator = 100;
	TInt KSecNumerator = 0;
	TInt KSecDenominator = 1;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDestLatitude(KDegNumerator,KDegDenominator,KMinNumerator,KMinDenominator,KSecNumerator,KSecDenominator);
		}
	else
		{	
		// get the encoded data
		TInt degNumerator=0;
		TInt degDenominator=0;
		TInt minNumerator=0;
		TInt minDenominator=0;
		TInt secNumerator=0;
		TInt secDenominator=0;
			
		TExifReaderUtility reader(aMetaData);
		testResult = reader.GetGpsDestLatitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);
		if(testResult==KErrNone)
			{
			if(	 (degNumerator != KDegNumerator)		||
				 (degDenominator != KDegDenominator)	||
				 (minNumerator != KMinNumerator)		||
				 (minDenominator != KMinDenominator)	||
				 (secNumerator != KSecNumerator)		||
				 (secDenominator != KSecDenominator)  )	
				{
				testResult = KErrCorrupt;
				}

			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestLongitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	_LIT8(K8BitBufferTestResult, "W"); //west
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsDestLongitudeRef, "W\0"); 
		
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsDestLongitudeRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDestLongitudeRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsDestLongitudeRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestLongitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	TInt KDegNumerator = 60;
	TInt KDegDenominator = 1;
	TInt KMinNumerator = 3456;
	TInt KMinDenominator = 100;
	TInt KSecNumerator = 0;
	TInt KSecDenominator = 1;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDestLongitude(KDegNumerator,KDegDenominator,KMinNumerator,KMinDenominator,KSecNumerator,KSecDenominator);
		}
	else
		{	
		// get the encoded data
		TInt degNumerator=0;
		TInt degDenominator=0;
		TInt minNumerator=0;
		TInt minDenominator=0;
		TInt secNumerator=0;
		TInt secDenominator=0;
			
		TExifReaderUtility reader(aMetaData);
		testResult = reader.GetGpsDestLongitude(degNumerator,degDenominator,minNumerator,minDenominator,secNumerator,secDenominator);
		if(testResult==KErrNone)
			{
			if(	 (degNumerator != KDegNumerator)		||
				 (degDenominator != KDegDenominator)	||
				 (minNumerator != KMinNumerator)		||
				 (minDenominator != KMinDenominator)	||
				 (secNumerator != KSecNumerator)		||
				 (secDenominator != KSecDenominator)  )	
				{
				testResult = KErrCorrupt;
				}

			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestBearingRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	_LIT8(K8BitBufferTestResult, "T");//true direction
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsDestBearingRef, "T\0"); 
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsDestBearingRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDestBearingRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsDestBearingRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestBearing(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	const TInt KRationalNumeratorTestResult=180;
	const TInt KRationalDenominatorTestResult=30;
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsDestBearing(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsDestBearing(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestDistanceRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	_LIT8(K8BitBufferTestResult, "K");//kilometers
	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsDestDistanceRef, "K\0"); 
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KGpsDestDistanceRef().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDestDistanceRef(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsDestDistanceRef(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetGpsDestDistance(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	const TInt KRationalNumeratorTestResult=50;
	const TInt KRationalDenominatorTestResult=5;

	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsDestDistance(KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	TExifReaderUtility reader(aMetaData);
	TInt num=0;
	TInt den=0;
	testResult = reader.GetGpsDestDistance(num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}		

TInt TGPSImage::SetGpsProcessingMethod(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TDesC8& aData)
	{
	HBufC8* param=0;		
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		param=aData.Alloc();
		TExifWriterUtility writer(aMetaData);
		testResult=writer.SetGpsProcessingMethod(param);
		}
	else
		{
		TExifReaderUtility reader(aMetaData);
		testResult=reader.GetGpsProcessingMethod(param);
		if(testResult==KErrNone)
			{
			if (param->Compare(aData) != 0)
				{
				testResult=KErrCorrupt;
				}		
			}
		}
		
	delete param;
	return testResult;
	}

TInt TGPSImage::SetGpsAreaInformation(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TDesC8& aData)
	{
	HBufC8* param=0;		
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		param=aData.Alloc();
		TExifWriterUtility writer(aMetaData);
		testResult=writer.SetGpsAreaInformation(param);
		}
	else
		{
		TExifReaderUtility reader(aMetaData);
		testResult=reader.GetGpsAreaInformation(param);
		if(testResult==KErrNone)
			{
			if (param->Compare(aData) != 0)
				{
				testResult=KErrCorrupt;
				}		
			}
		}
		
	delete param;
	return testResult;
	}
	
TInt TGPSImage::SetGpsDateStamp(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	_LIT8(KDate, "2006:02:18\0"); //field must be 11 chars so need null term
	_LIT8(KDateCompare, "2006:02:18"); //but comparison does not include null
	
	TInt testResult=KErrNone;
	
	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		HBufC8* buffer=KDate().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult = writer.SetGpsDateStamp(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=0;
	testResult = reader.GetGpsDateStamp(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(KDateCompare))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;
	return testResult;
	}

	
TInt TGPSImage::SetGpsDifferential(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result: ExifGPSTransformRefTT.jpg
	const TUint16 KGPSDifferential=1; //differential correction applied
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	
		// set the data in the encoder
		TExifWriterUtility writer(aMetaData);
		return writer.SetGpsDifferential(KGPSDifferential);
		}

	TExifReaderUtility reader(aMetaData);
	TUint16 param=0; 
	testResult = reader.GetGpsDifferential(param);
	if(testResult==KErrNone)
		{
		if((param!=KGPSDifferential))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}
	
TInt TGPSImage::SetExifVersionId(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	_LIT8(K8BitBufferTestResult, "9876");
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	// set the data in the encoder
		HBufC8* buffer=K8BitBufferTestResult().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult=writer.SetExifVersion(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetExifVersion(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;
	}
	
TInt TGPSImage::SetInteroperabilityIndex(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	_LIT8(K8BitBufferTestResult, "ABC");
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	// set the data in the encoder
		HBufC8* buffer=K8BitBufferTestResult().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult=writer.SetInteroperabilityIndex(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetInteroperabilityIndex(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;		
	}
	
TInt TGPSImage::SetInteroperabilityVersion(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	_LIT8(K8BitBufferTestResult, "4321");
	
	TInt testResult=KErrNone;

	if(aEncodeStep!=EVerifyData)
		{	// set the data in the encoder
		HBufC8* buffer=K8BitBufferTestResult().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		TExifWriterUtility writer(aMetaData);
		testResult=writer.SetInteroperabilityVersion(buffer);
		delete buffer;
		return testResult;
		}

	TExifReaderUtility reader(aMetaData);
	HBufC8* param=NULL;
	testResult = reader.GetInteroperabilityVersion(param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;	
	return testResult;		
	}

	
//
//
//
// Misc. GPS Tests
//
//
//


// Test for:
//		DEF085006: ExifUtility functions GetPixelX/YDimention assume param is LONG
//	
void TGPSImage::ShortPixelXYDimension(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- ShortPixelXYDimension test %S"), &KImageTestGPSExifDecode2);
	
	//
	//		Get the metadata
	TFileName fileName(KImageTestGPSExifDecode2); //this image uses SHORT Pixel XY dimension
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	MExifMetadata* metaData=decoder->ExifMetadata();

	testResult = GetPixelXYDimension(metaData);

	delete decoder;
	//		Test the metadata
	//

	//
	//		Log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		Log the result
	//
	}
	
	
void TGPSImage::GPSExifDataRegression(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- GPSExifDataRegression test--- %S"), &KImageTestExifGPSEncode);
	
	//This test creates a JPEG with GPS IFD data and also sets and reads non GPS IFD data.
	//The test then checks that the GPS data does not effect the reading of the non GPS IFD data

	//
	//		Load the reference image
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	TInt bmpErr=bitmap->Load(KGPSEncodeSourceFileName);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref bitmap file"));
		TestFailed(bmpErr);
		return;
		}
	//		Load the reference image
	//


	//
	//		Get the exif metadata
	TFileName fileName(KImageTestExifGPSEncode);
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(testResult, encoder=CreateEncoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}
	CleanupStack::PushL(encoder);

	MExifMetadata* metaData=encoder->ExifMetadata();
	//		Get the exif metadata
	//


	//
	//		encode the image
	testResult=SetGpsVersionId(ESetData, metaData);
	if(testResult==KErrNone)
		{
		testResult=Set8bitBuffer(ESetData, metaData, ESetParam); //TImage test for 0 IFD model
		}
	if (testResult==KErrNone)
		{
		testResult=SetInteger(ESetData, metaData, ESetParam); //TImage test for Exif IFD Pixel X Dimention
		}
	
	if(testResult!=KErrNone)
		{
		Printf(_L("unable to set data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
		return;
		}

	// launch the conversion
	CActiveEncodeListener* listener= NULL;
	TRAP(testResult, listener= new (ELeave) CActiveEncodeListener);
	if(testResult)
		{
		Printf(_L("Could not create encode listener"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
		return;
		}
	listener->InitialiseActiveListener(encoder);
	encoder->Convert(& listener->iStatus, *bitmap);
	
	CActiveScheduler::Start();

	// destroy the encoder
	delete listener;
	CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
	metaData=NULL;	
	//		encode the image
	//


	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();


	testResult=SetGpsVersionId(EVerifyData, metaData);
	if(testResult==KErrNone)
		{
		Printf(_L("GPS IFD param verified OK"));
		testResult=Set8bitBuffer(EVerifyData, metaData, ESetParam); //0 IFD
		}
	if(testResult==KErrNone)
		{
		Printf(_L("0 IFD param verified OK"));
		testResult=SetInteger(EVerifyData, metaData, ESetParam); //Exif IFD
		}
	if (testResult==KErrNone)
		{
		Printf(_L("Exif IFD param verified OK"));
		}
	// destroy the decoder
	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
//DEF082163: Exif - Copyright tag not handled correctly with NULLs
void TGPSImage::CopyrightNullTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- CopyrightNullTest--- %S"), &KImageTestExifGPSEncode);

	//
	//		Load the reference image
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	TInt bmpErr=bitmap->Load(KGPSEncodeSourceFileName);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref bitmap file"));
		TestFailed(bmpErr);
		return;
		}
	//		Load the reference image
	//


	//
	//		Get the exif metadata
	TFileName fileName(KImageTestExifGPSEncode);
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(testResult, encoder=CreateEncoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}
	CleanupStack::PushL(encoder);

	MExifMetadata* metaData=encoder->ExifMetadata();	


	testResult=SetCopyright(ESetData, metaData);
	if(testResult)
		{
		Printf(_L("unable to set data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
		return;
		}

	// launch the conversion
	CActiveEncodeListener* listener= NULL;
	TRAP(testResult, listener= new (ELeave) CActiveEncodeListener);
	if(testResult)
		{
		Printf(_L("Could not create encode listener"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
		return;
		}
	listener->InitialiseActiveListener(encoder);
	encoder->Convert(& listener->iStatus, *bitmap);
	
	CActiveScheduler::Start();

	// destroy the encoder
	delete listener;
	CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
	metaData=NULL;	
	//		encode the image
	//


	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();

	testResult=SetCopyright(EVerifyData, metaData);
	
		// destroy the decoder
	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
TInt TGPSImage::SetCopyright(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	HBufC8* param=0;
	const TUint KCopyrightTagID = 0x8298;
	
	TInt testResult=KErrNone;
	
	// Expected result:
	_LIT8(KCopyright, "Photographer\0Editor\0");

	//to keep consitency with other ascii gets the null is removed:
	_LIT8(KCopyrightResult, "Photographer\0Editor");


	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		HBufC8* buffer=KCopyright().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		testResult= aMetaData->SetParam8(KCopyrightTagID, 0, buffer);
		delete buffer;
		return testResult;
		}

	testResult = aMetaData->GetParam8(KCopyrightTagID, 0, param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(KCopyrightResult))
			{
			testResult=KErrCorrupt;
			}
		delete param;
		}
		
	return testResult;
	}
	
// DEF082163: Exif - Copyright tag not handled correctly with NULLs
void TGPSImage::CopyrightNullTestTransform(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Copyright Null Test Transform --- %S"), &KImageTestGPSExifDecode2);

	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();


	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	TInt testResult=KErrNone;
	testResult=SetCopyrightEditor(ESetData, metaData); //existing

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();

	testResult=SetCopyrightEditor(EVerifyData, metaData); //not set

	// destroy the decoder
	delete decoder;

	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
TInt TGPSImage::SetCopyrightEditor(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	HBufC8* param=0;
	const TUint KCopyrightTagID = 0x8298;
	
	TInt testResult=KErrNone;
	
	// Expected result:
	_LIT8(KCopyright, " \0Editor\0");//Space+NULL+editor+NULL
	//to keep consitency with other ascii gets the null is removed.
	_LIT8(KCopyrightResult, " \0Editor");

	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		HBufC8* buffer=KCopyright().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		testResult= aMetaData->SetParam8(KCopyrightTagID, 0, buffer);
		delete buffer;
		return testResult;
		}

	testResult = aMetaData->GetParam8(KCopyrightTagID, 0, param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(KCopyrightResult))
			{
			testResult=KErrCorrupt;
			}
		delete param;
		}
		
	return testResult;
	}
	

void TGPSImage::SetGPSTransformNoGpsTag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Using source file with no GPS tag --- %S"), &KImageTestNoGPSTag);

	TFileName sourceFileName(KImageTestNoGPSTag);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();


	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	TInt testResult=KErrNone;
	testResult=SetGpsLatitude(ESetData, metaData); //RATIONAL ARRAY

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult=SetGpsLatitude(EVerifyData, metaData); //RATIONAL ARRAY

	// destroy the decoder
	delete decoder;

	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
void TGPSImage::SetGPSTransformNoGpsBadCountTag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Using source file with no GPS tag setting value higher than the count limitation--- %S"), &KImageTestNoGPSTag);
	
	//This test is to verify a new IFD entry for the GPS IFD where the count value is invalid
	//there was a defect on this reported - which resulted in a bad cell 42 panic.

	TFileName sourceFileName(KImageTestNoGPSTag);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();


	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	TInt testResult=KErrNone;

	HBufC8* param=0;
	const TUint KGPSVersionTagID = 0x0;

	// Expected result:
	_LIT8(KGPSVersionID5Byte, "\xFF\xFF\xFF\xFF\xFF");

	// set the data in the encoder
	HBufC8* buffer=KGPSVersionID5Byte().Alloc();
	if(buffer==NULL)
		{
		testResult = KErrNoMemory;
		}
	else
		{
		testResult= metaData->SetParam8(KGPSVersionTagID, 0, buffer);
		}

	delete buffer;

	if(testResult!=KErrNotSupported)
		{
		Printf(_L("An error occured while setting data - expected KErrNotSupported"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	testResult = KErrNone;
	
	const TUint KGPSLongitudeTagID=0x4;

	// Expected result:
	TRational KRational1TestResult= {1, 2};
	TRational KRational2TestResult={3, 4};  
	TRational KRational3TestResult={5, 6};
	TRational KRational4TestResult={7, 8};
	
	CArrayFixFlat<TRational>* value=NULL;
	TRAP(testResult, value=new (ELeave) CArrayFixFlat<TRational>(4));
	
	if (!testResult)
		{
		// set the data in the encoder
		TRAP(testResult, 
			{
			value->SetReserveL(3);			
			value->AppendL(KRational1TestResult);
			value->AppendL(KRational2TestResult);
			value->AppendL(KRational3TestResult);
			value->AppendL(KRational4TestResult);
			});
		}
	if(testResult==KErrNone)
		{
		testResult=metaData->SetRationalArrayParam(KGPSLongitudeTagID, 0, *value);
		}
		
	delete value;
	
	if(testResult!=KErrNotSupported)
		{
		Printf(_L("An error occured while setting data - expected KErrNotSupported"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	
	testResult = metaData->GetParam8(KGPSVersionTagID, 0, param);
	if (testResult!=KErrNotFound)
		{
		Printf(_L("Expected KErrNotFound"));
		TestFailed(testResult);
		return;
		}
	
	testResult = metaData->GetRationalArrayParam(KGPSLongitudeTagID, 0, *value);
	if (testResult!=KErrNotFound)
		{
		Printf(_L("Expected KErrNotFound"));
		TestFailed(testResult);
		return;
		}
	else
		{
		testResult = KErrNone;
		}
	
	// destroy the decoder
	delete decoder;

	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}

void TGPSImage::CommonTransformGetPixelSizeL(const TDesC& aSrcFileName,TSize& aBitmapSize)	
	{
	CJPEGExifDecoder* decoder = NULL;

	decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aSrcFileName, CImageDecoder::EOptionNone, KImageTypeJPGUid));

	// store the frame information and frame count
	TFrameInfo frameInfo = decoder->FrameInfo(0);
	TRect bitmapSize = frameInfo.iFrameCoordsInPixels;
	aBitmapSize.iWidth = bitmapSize.iBr.iX-bitmapSize.iTl.iX;
	aBitmapSize.iHeight = bitmapSize.iBr.iY-bitmapSize.iTl.iY;

	delete decoder;	
	}
		
void TGPSImage::SetGPSTransformNoGpsBadCountTagOptionNone(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Using source file with no GPS tag setting value higher than the count limitation--- %S"), &KImageTestNoGPSTag);
	
	//This test is to verify a new IFD entry for the GPS IFD where the count value is invalid
	//there was a defect on this reported - which resulted in a bad cell 42 panic.

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	TSize pixelSize;//(640,480);
	CommonTransformGetPixelSizeL(sourceFileName,pixelSize);
		
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=CImageTransform::NewL(iFs););
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not do Create imageTransform"));
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	
	TRAPD(testErr, imageTransform->SetSourceFilenameL(sourceFileName));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set the source file in imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	// set the destination file		
	TRAP(testErr, imageTransform->SetDestFilenameL(destFileName));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest file in imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	TRAP(testErr, imageTransform->SetDestSizeInPixelsL(pixelSize, ETrue));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest size in pixels imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	TRAP(testErr, imageTransform->SetOptionsL(CImageTransform::EOptionNone));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set options imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
		
	imageTransform->SetPreserveImageData(ETrue);

	TRAP(testErr, imageTransform->SetupL());
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();

	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	// Exif Write Test
	const TUint KGPSVersionTagID = 0x0;
	const TUint8 GpsVersionId[] = { 0xFF,0xFF,0xFF,0xFF,0xFF };//5byte
	HBufC8* param = NULL;
	HBufC8* buffer = NULL;
	TRAPD(testResult, buffer = HBufC8::NewL(sizeof(GpsVersionId)));
	if(testResult==KErrNone)
		{
		TPtr8 stringPtr(buffer->Des());
		stringPtr.Append(GpsVersionId, sizeof(GpsVersionId));
		testResult = metaData->SetParam8(KGPSVersionTagID, 0x00040000, buffer);
		delete buffer;
		buffer=NULL;
		}

	if(testResult!=KErrNotSupported)
		{
		Printf(_L("An error occured while setting data - expected KErrNotSupported"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	
	testResult = metaData->GetParam8(KGPSVersionTagID, 0, param);
	if (testResult!=KErrNotFound)
		{
		Printf(_L("Expected KErrNotFound"));
		TestFailed(testResult);
		return;
		}
	else
		{
		testResult = KErrNone;
		}

	// destroy the decoder
	delete decoder;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
	
void TGPSImage::GPSTransformDeleteAndSetup(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Create ImageTransform delete and setp again using big endian--- %S"), &KImageTestGPSExifDecode3);

	TFileName sourceFileName(KImageTestGPSExifDecode3);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	TFileName sourceFileName2(KImageTestExifGPSTransformDest);//same as dest filename for transform1
	TFileName destFileName2(KImageTestExifGPSTransformDest2);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
		
	// destroy the image transform
	delete imageTransform;	
	imageTransform=NULL;
	
	TRAP(allocErr,imageTransform=PrepareImageTransformL(sourceFileName2,destFileName2,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	// destroy the image transform
	delete imageTransform;
	}
	
void TGPSImage::GPSLatitudeRefCount(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Test GPSLatitudeRef with invalid count --- %S"), &KImageTestGPSExifDecode2);

	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();


	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	TInt testResult=KErrNone;
	
	const TUint KGPSLatitudeRefTagID = 0x1;
	//since both interop and GPS sub IFDs both have tags of 1 
	//we need to specify GPS sub IFD.

	//note we need to have the NULL string termination as the count is 2 so the NULL terminator is included
	_LIT8(KGpsLatitudeRefCount3, "NN\0"); 

	// set the data in the encoder
	HBufC8* buffer=KGpsLatitudeRefCount3().Alloc();
	if(buffer==NULL)
		{
		testResult = KErrNoMemory;
		}
	else
		{
		testResult= metaData->SetParam8(KGPSLatitudeRefTagID, KExifGpsIfd, buffer);
		delete buffer;
		}
	
	//the length 3 is invalid (max length is 2) so should be KErrArgument	
	if(testResult!=KErrArgument)
		{
		Printf(_L("An error occured while setting data %d"),testResult);
		if (testResult == KErrNone)
			{
			testResult = KErrArgument;
			}
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	else
		{//since we are expected KErrNotSupported
		testResult = KErrNone;
		}
		
	CleanupStack::PopAndDestroy(imageTransform);

	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
// DEF082161: Exif - Thumbnail EOI duplicated
void TGPSImage::ThumbnailEOI(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- ThumbnailEOI --- %S"), &KImageTestExifTransformSource);
	
	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);
	TFileName destFileName2(KImageTestExifTransformDest2);
	TInt err = ThumbnailEOITransform(sourceFileName, destFileName);
	if (!err)
		{//output file of first transform is the input to the second transform
		err = ThumbnailEOITransform(destFileName, destFileName2);
		}	
	if (!err)
		{
		RFile transform1File;
		RFile transform2File;
		err = transform1File.Open(iFs,KImageTestExifTransformDest,EFileRead);
		if (!err)
			{
			err = transform2File.Open(iFs,KImageTestExifTransformDest2,EFileRead);
			}
		if (!err)
			{
			TInt transform1FileSize = 0;
			TInt transform2FileSize = 0;
			err = transform1File.Size(transform1FileSize);
			err = transform2File.Size(transform2FileSize);
			if ((transform1FileSize == transform2FileSize) && transform1FileSize && !err)
				{
				Printf(_L("Size match %d bytes"),transform1FileSize);
				}
			else
				{
				Printf(_L("Size mismatch"));
				err = KErrCorrupt;
				}
			}
		}
	if (err)
		{
		TestFailed(err);
		}
	}
	
TInt TGPSImage::ThumbnailEOITransform(TFileName& aSourceFileName, TFileName& aDestFileName)
	{
	__MM_HEAP_MARK;
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr, imageTransform=CImageTransform::NewL(iFs););
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not do Create imageTransform"));
		return allocErr;
		}
	CleanupStack::PushL(imageTransform);
	
	TRAPD(testErr, imageTransform->SetSourceFilenameL(aSourceFileName));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set the source file in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}

	// set the destination file		
	TRAP(testErr, imageTransform->SetDestFilenameL(aDestFileName));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest file in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
	
	CJPEGExifDecoder* decoder = NULL;
	TRAP(testErr, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aSourceFileName, CImageDecoder::EOptionNone, KImageTypeJPGUid)));
    if (testErr != KErrNone)
		{
		Printf(_L("Could not create CJPEGExifDecoder"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
    TFrameInfo frameInfo = decoder->FrameInfo(0);
    TRect pixelSize = frameInfo.iFrameCoordsInPixels;
    TSize size(0,0);
    size.iWidth = pixelSize.iBr.iX-pixelSize.iTl.iX;
    size.iHeight = pixelSize.iBr.iY-pixelSize.iTl.iY;
    delete decoder;
    decoder = NULL;
	
	TRAP(testErr, imageTransform->SetDestSizeInPixelsL(TSize(size.iWidth, size.iHeight), ETrue));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest size in pixels imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetOptionsL(CImageTransform::EThumbnail));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set options imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
		
	imageTransform->SetPreserveImageData(ETrue);

	TRAP(testErr, imageTransform->SetupL());
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}

	testErr=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testErr!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testErr);
		return testErr;
		}
	//		encode the image
	//

	__MM_HEAP_MARKEND;
	return testErr;
	}
	
// Test for:
//		DEF083701: Image Transform CancelTransform can cause code to hang
//
void TGPSImage::CancelTransform(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Cancel Transform: file source & dest --- "));
	
	//this test cancels a transform - the test will keep waiting and timeout if not cancelled
	
	__MM_HEAP_MARK;

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	DoCancelTransformTest(sourceFileName, destFileName);

	REComSession::FinalClose();
	RFbsSession::Disconnect();	
	__MM_HEAP_MARKEND;	
	}


// Does Cancel Transform test - the test will keep waiting and timeout if not cancelled
void TGPSImage::DoCancelTransformTest(const TExifTransformSourceDest& aSource, const TExifTransformSourceDest& aDest)
	{
	// create the Transformer
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(aSource,aDest,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	
	// launch the conversion
	CActiveTransformListener* listener= new (ELeave) CActiveTransformListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener(imageTransform);
	imageTransform->Transform(listener->iStatus);
	
	// cancel it
	imageTransform->CancelTransform();
	CActiveScheduler::Start();
	
	TInt transformResult=listener->iStatus.Int();
	
	if(transformResult!=KErrCancel)
		{
		Printf(_L("Transform was not cancelled"));
		TestFailed(transformResult);
		CleanupStack::PopAndDestroy(2, imageTransform);	// listener
		return;		
		}
	
	listener->InitialiseActiveListener(imageTransform);
	
	//try another transform after the cancel - let it proceed this time
	imageTransform->Transform(listener->iStatus);
	CActiveScheduler::Start();
	
	transformResult=listener->iStatus.Int();
	if(transformResult!=KErrNone)
		{
		Printf(_L("Transform error"));
		TestFailed(transformResult);
		CleanupStack::PopAndDestroy(2, imageTransform);	// listener
		return;		
		}
	
	CleanupStack::PopAndDestroy(2, imageTransform);	// listener, imageTransform
	}
	
void TGPSImage::SetGPSTransformExifDescriptorSrcTN(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source preserve image True; EOptionNone %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorSrc(ETrue ,EFalse);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefTN);
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

	
void TGPSImage::SetGPSTransformExifDescriptorSrcFN(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source preserve image False; EOptionNone %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorSrc(EFalse ,EFalse);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest, KImageTestGPSExifTransformRefFN);

	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

	
void TGPSImage::SetGPSTransformExifDescriptorSrcTT(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source preserve image True; EThumbnail %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorSrc(ETrue,ETrue);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefTT);

	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}


void TGPSImage::SetGPSTransformExifDescriptorSrcFT(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source preserve image False; EThumbnail %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorSrc(EFalse,ETrue);

	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefFT);
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}


void TGPSImage::SetGPSTransformExifDescriptorSrc(TBool aPreserveImageData ,TBool aThumbnail)
	{
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	//read the source file into a descriptor
	HBufC8* sourceFileBuffer = NULL;
	ReadFileIntoBufferL(sourceFileName, sourceFileBuffer);
	CleanupStack::PushL(sourceFileBuffer);
	TPtr8 sourceDescriptor(sourceFileBuffer->Des());
	
	//
	//		Get the image transform
	CImageTransform* imageTransform=NULL;	
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceDescriptor,destFileName,aThumbnail,aPreserveImageData));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the image transform
	//

	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	
	TInt testResult = TestSetGPSData(ESetData, metaData);

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, sourceFileBuffer); // imageTransform
		return;
		}

	// launch the conversion
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(2, sourceFileBuffer); // imageTransform
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);

	// destroy the decoder
	delete decoder;

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}


void TGPSImage::SetGPSTransformExifDescriptorSrcDstTN(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source and destination preserve image True; EOptionNone %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	SetGPSTransformExifDescriptorSrcDst(ETrue,EFalse,sourceFileName);

	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefTN);

	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}


void TGPSImage::SetGPSTransformExifDescriptorSrcDstFN(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source and destination preserve image False; EOptionNone %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	SetGPSTransformExifDescriptorSrcDst(EFalse,EFalse,sourceFileName);

	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefFN);
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

	
void TGPSImage::SetGPSTransformExifDescriptorSrcDstTT(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source and destination preserve image True; EThumbnail %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	SetGPSTransformExifDescriptorSrcDst(ETrue,ETrue,sourceFileName);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefTT);

	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}


void TGPSImage::SetGPSTransformExifDescriptorSrcDstFT(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor source and destination preserve image False; EThumbnail %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	SetGPSTransformExifDescriptorSrcDst(EFalse,ETrue,sourceFileName);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefFT);
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}
	
	
void TGPSImage::SetGPSTransformExifDescriptorSrcDst(TBool aPreserveImageData, TBool aThumbnail, TFileName& aSourceFileName)
	{
	TFileName destFileName(KImageTestExifGPSTransformDest);
	HBufC8* destDescriptor=NULL;
	
	//read the source file into a descriptor
	HBufC8* sourceFileBuffer = NULL;
	ReadFileIntoBufferL(aSourceFileName, sourceFileBuffer);
	CleanupStack::PushL(sourceFileBuffer);
	TPtr8 sourceDescriptor(sourceFileBuffer->Des());
	
	//
	//		Get the image transform
	CImageTransform* imageTransform=NULL;	
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceDescriptor,destDescriptor,aThumbnail,aPreserveImageData));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the image transform
	//

	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	TInt testResult = TestSetGPSData(ESetData, metaData);

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2,sourceFileBuffer); // imageTransform
		return;
		}

	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(2,sourceFileBuffer); // imageTransform
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
		
	//now copy the descriptor into the destination
	TInt createFileFromDestDescriptorError = KErrBadDescriptor;
	if (destDescriptor)
		{
		if (destDescriptor->Length())
			{
			Printf(_L("destination descriptor created length %d"), destDescriptor->Length());
			RFile destinationFile;
			if(destinationFile.Open(iFs, destFileName, EFileWrite)==KErrNone)
				{
				//the the destination file already exists so replace it
				destinationFile.Close();
				createFileFromDestDescriptorError = iFs.Delete(destFileName);
				}
			createFileFromDestDescriptorError = destinationFile.Create(iFs, destFileName, EFileWrite);
			if (!createFileFromDestDescriptorError)
				{
				createFileFromDestDescriptorError = destinationFile.Write(*destDescriptor);
				}
			destinationFile.Close();
			}
		}
	delete destDescriptor; //allocated by CImageTransform but deleted by the client
	destDescriptor = NULL;

	if (createFileFromDestDescriptorError)
		{
		Printf(_L("problem creating file from dest descriptor %d"), createFileFromDestDescriptorError);
		TestFailed(createFileFromDestDescriptorError);
		return;
		}
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);

	// destroy the decoder
	delete decoder;

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}


void TGPSImage::SetGPSTransformExifDescriptorDstTN(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);	
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor destination preserve image True; EOptionNone %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorDst(ETrue,EFalse);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefTN);
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

	
void TGPSImage::SetGPSTransformExifDescriptorDstFN(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor destination preserve image False; EOptionNone %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorDst(EFalse,EFalse);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefFN);

	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

	
void TGPSImage::SetGPSTransformExifDescriptorDstTT(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor destination preserve image True; EThumbnail %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorDst(ETrue,ETrue);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefTT);
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}


void TGPSImage::SetGPSTransformExifDescriptorDstFT(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setters test using descriptor destination preserve image False; EThumbnail %S"), &KImageTestGPSExifDecode2);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	SetGPSTransformExifDescriptorDst(EFalse,ETrue);
	
	VerifyAgainstReferenceFile(KImageTestExifGPSTransformDest,KImageTestGPSExifTransformRefFT);

	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}


void TGPSImage::SetGPSTransformExifDescriptorDst(TBool aPreserveImageData, TBool aThumbnail)
	{
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	HBufC8* destDescriptor=NULL;
	//
	//		Get the image transform
	CImageTransform* imageTransform=NULL;	
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destDescriptor,aThumbnail,aPreserveImageData));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the image transform
	//

	//
	//		encode the image
	//		Test the data
	// use test image 2 as image one has an APP0 marker which prevents the transform.
	TInt testResult = TestSetGPSData(ESetData, metaData);

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
		
	//now copy the descriptor into the destination
	TInt createFileFromDestDescriptorError = KErrBadDescriptor;
	if (destDescriptor)
		{
		if (destDescriptor->Length())
			{
			Printf(_L("destination descriptor created length %d"), destDescriptor->Length());
			RFile destinationFile;
			if(destinationFile.Open(iFs, destFileName, EFileWrite)==KErrNone)
				{
				//the the destination file already exists so replace it
				destinationFile.Close();
				createFileFromDestDescriptorError = iFs.Delete(destFileName);
				}
			createFileFromDestDescriptorError = destinationFile.Create(iFs, destFileName, EFileWrite);
			if (!createFileFromDestDescriptorError)
				{
				createFileFromDestDescriptorError = destinationFile.Write(*destDescriptor);
				}
			destinationFile.Close();
			}
		}
	delete destDescriptor; //allocated by CImageTransform but deleted by the client
	destDescriptor = NULL;

	if (createFileFromDestDescriptorError)
		{
		Printf(_L("problem creating file from dest descriptor %d"), createFileFromDestDescriptorError);
		TestFailed(createFileFromDestDescriptorError);
		return;
		}
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	
	testResult = TestSetGPSData(EVerifyData, metaData);
	// destroy the decoder
	delete decoder;

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}

void TGPSImage::CancelTransformExifDescriptorSrc(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Cancel Transform: desc source, file dest --- "));
		
	__MM_HEAP_MARK;

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	//read the source file into a descriptor
	HBufC8* sourceFileBuffer = NULL;
	ReadFileIntoBufferL(sourceFileName, sourceFileBuffer);
	CleanupStack::PushL(sourceFileBuffer);
	TPtr8 sourceDescriptor(sourceFileBuffer->Des());
	
	DoCancelTransformTest(sourceDescriptor, destFileName);
	
	CleanupStack::PopAndDestroy(sourceFileBuffer);

	REComSession::FinalClose();
	RFbsSession::Disconnect();	
	__MM_HEAP_MARKEND;		
	}
	
void TGPSImage::CancelTransformExifDescriptorSrcDst(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Cancel Transform: desc source & dest --- "));
		
	__MM_HEAP_MARK;

	TFileName sourceFileName(KImageTestExifTransformSource);
	HBufC8* destDescriptor=NULL;	
	
	//read the source file into a descriptor
	HBufC8* sourceFileBuffer = NULL;
	ReadFileIntoBufferL(sourceFileName, sourceFileBuffer);
	CleanupStack::PushL(sourceFileBuffer);
	TPtr8 sourceDescriptor(sourceFileBuffer->Des());
	
	DoCancelTransformTest(sourceDescriptor, destDescriptor);
	
	delete destDescriptor;
	CleanupStack::PopAndDestroy(sourceFileBuffer);

	REComSession::FinalClose();
	RFbsSession::Disconnect();	
	__MM_HEAP_MARKEND;			
	}
	
void TGPSImage::CancelTransformExifDescriptorDst(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Cancel Transform: file source, desc dest --- "));
		
	__MM_HEAP_MARK;

	TFileName sourceFileName(KImageTestExifTransformSource);
	HBufC8* destDescriptor=NULL;
	
	DoCancelTransformTest(sourceFileName, destDescriptor);
	
	delete destDescriptor;

	REComSession::FinalClose();
	RFbsSession::Disconnect();	
	__MM_HEAP_MARKEND;		
	}

void TGPSImage::TransformJpegDescSource(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Jpeg Descriptor Source --- "));
		
	__MM_HEAP_MARK;

	TFileName sourceFileName(KImageTestNoGPSTag);
	
	GenericTransformDescSrcTest(EFalse, ETrue, sourceFileName);

	REComSession::FinalClose();
	RFbsSession::Disconnect();	
	__MM_HEAP_MARKEND;		
	}
	
void TGPSImage::TransformThumblessExifDescSource(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Thumbnail-less Exif Descriptor Source --- "));
		
	__MM_HEAP_MARK;

	TFileName sourceFileName(KImageTestThumblessExifTransformSource);
	
	GenericTransformDescSrcTest(EFalse, ETrue, sourceFileName);

	REComSession::FinalClose();
	RFbsSession::Disconnect();	
	__MM_HEAP_MARKEND;		
	}
	
//Tests various desc sources based on given filename
void TGPSImage::GenericTransformDescSrcTest(TBool aPreserveImageData, TBool aThumbnail, TFileName& aSourceFileName)
	{
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	//read the source file into a descriptor
	HBufC8* sourceFileBuffer = NULL;
	ReadFileIntoBufferL(aSourceFileName, sourceFileBuffer);
	CleanupStack::PushL(sourceFileBuffer);
	TPtr8 sourceDescriptor(sourceFileBuffer->Des());
	
	//
	//		Get the image transform
	CImageTransform* imageTransform=NULL;	
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceDescriptor,destFileName,aThumbnail,aPreserveImageData));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	//		Get the image transform
	//

	//
	//		encode the image
	// launch the conversion
	TInt testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(2, sourceFileBuffer); // imageTransform
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	
	// destroy the decoder
	delete decoder;
	}

//
// Tests Setting all GPS tags
//
TInt TGPSImage::TestSetGPSData(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{	
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	testResult=SetGpsVersionId(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsVersionId"), aEncodeStep, finalResult);
		
	testResult=SetGpsLatitudeRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsLatitudeRef"), aEncodeStep, finalResult);
			
	testResult=SetGpsLatitude(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsLatitude"), aEncodeStep, finalResult);
		
	testResult=SetGpsLongitudeRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsLongitudeRef"), aEncodeStep, finalResult);		
		
	testResult=SetGpsLongitude(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsLongitude"), aEncodeStep, finalResult);

	testResult=SetGpsAltitudeRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsAltitudeRef"), aEncodeStep, finalResult);
	
	testResult=SetGpsAltitude(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsAltitude"), aEncodeStep, finalResult);
	
	testResult=SetGpsTimeStamp(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsTimeStamp"), aEncodeStep, finalResult);

	testResult=SetGpsSatellites(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsSatellites"), aEncodeStep, finalResult);

	testResult=SetGpsStatus(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsStatus"), aEncodeStep, finalResult);

	testResult=SetGpsMeasureMode(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsMeasureMode"), aEncodeStep, finalResult);

	testResult=SetGpsDop(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDop"), aEncodeStep, finalResult);

	testResult=SetGpsSpeedRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsSpeedRef"), aEncodeStep, finalResult);

	testResult=SetGpsSpeed(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsSpeed"), aEncodeStep, finalResult);

	testResult=SetGpsTrackRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsTrackRef"), aEncodeStep, finalResult);

	testResult=SetGpsTrack(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsTrack"), aEncodeStep, finalResult);

	testResult=SetGpsImgDirectionRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsImgDirectionRef"), aEncodeStep, finalResult);

	testResult=SetGpsImgDirection(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsImgDirection"), aEncodeStep, finalResult);

	testResult=SetGpsMapDatum(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsMapDatum"), aEncodeStep, finalResult);

	testResult=SetGpsDestLatitudeRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestLatitudeRef"), aEncodeStep, finalResult);

	testResult=SetGpsDestLatitude(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestLatitude"), aEncodeStep, finalResult);

	testResult=SetGpsDestLongitudeRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestLongitudeRef"), aEncodeStep, finalResult);

	testResult=SetGpsDestLongitude(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestLongitude"), aEncodeStep, finalResult);

	testResult=SetGpsDestBearingRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestBearingRef"), aEncodeStep, finalResult);

	testResult=SetGpsDestBearing(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestBearing"), aEncodeStep, finalResult);

	testResult=SetGpsDestDistanceRef(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestDistanceRef"), aEncodeStep, finalResult);

	testResult=SetGpsDestDistance(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDestDistance"), aEncodeStep, finalResult);
	
	// Test SetGpsProcessingMethod with Unicode
	_LIT16(KGpsProcessingMethod16, "Gps processing method");
	TBuf8<64> processingMethod(KUnicode);
	processingMethod.Append(TPtrC8((const TText8*)KGpsProcessingMethod16().Ptr(),KGpsProcessingMethod16().Size()) );
	testResult=SetGpsProcessingMethod(aEncodeStep, aMetaData, processingMethod);
	ProcessGPSSetError(testResult, _L("SetGpsProcessingMethod Unicode"), aEncodeStep, finalResult);

	// Test SetGpsAreaInformation with Unicode
	_LIT16(KGpsAreaInformation16, "Gps Area Name");
	TBuf8<64> areaInfo(KUnicode);
	areaInfo.Append(TPtrC8((const TText8*)KGpsAreaInformation16().Ptr(),KGpsAreaInformation16().Size()) );
	testResult=SetGpsAreaInformation(aEncodeStep, aMetaData, areaInfo);
	ProcessGPSSetError(testResult, _L("SetGpsAreaInformation Unicode"), aEncodeStep, finalResult);	

	testResult=SetGpsDateStamp(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDateStamp"), aEncodeStep, finalResult);

	testResult=SetGpsDifferential(aEncodeStep, aMetaData);
	ProcessGPSSetError(testResult, _L("SetGpsDifferential"), aEncodeStep, finalResult);

	return finalResult;
	}
	
// Test setting the Undefined Gps Tags using Transformer metadata
void TGPSImage::SetTransformUndefinedGpsTags(const TExifDataType& aDataType, const TExifOperation& aOperation)
	{
	Printf(_L("\n"));
	Printf(_L("--- SetTransformUndefinedGpsTags test--- %d %d %S"), aDataType, aOperation, &KImageTestExifTransformSource);
		
	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	TExifEncodeStep encodeStep=ExifOpToExifEncodeStepL(aOperation);
	
	//
	//		Get the image transform
	CImageTransform* imageTransform=NULL;	
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the image transform
	//
	
	//
	//		Set/Add the Undefiend Gps Tags in the Encoder Metadata
	TInt testResult=SetUndefinedGpsTags(encodeStep, aDataType, extension->ExifMetadata());
	if (testResult!=KErrNone)
		{
		CleanupStack::PopAndDestroy(imageTransform);
		TestFailed(testResult);
		return;
		}
	//		Set/Add the Undefiend Gps Tags in the Encoder Metadata
	//
	
	//
	//		transform the image
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;		
		}
	//		transform the image
	//
	
	//
	//		verify the result
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}			
	testResult=SetUndefinedGpsTags(EVerifyData, aDataType, decoder->ExifMetadata());
	delete decoder;
	if (testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		verify the result
	//		
	}
	
// Test setting the Undefined Gps Tags using Encoder metadata
void TGPSImage::SetEncoderUndefinedGpsTags(const TExifDataType& aDataType, const TExifOperation& aOperation)
	{
	Printf(_L("\n"));
	Printf(_L("--- SetEncoderUndefinedGpsTags test--- %d %d %S"), aDataType, aOperation, &KImageTestExifGPSEncode);
	
	TExifEncodeStep encodeStep=ExifOpToExifEncodeStepL(aOperation);
	
	//
	//		Get the exif encoder
	TFileName fileName(KImageTestExifGPSEncode);
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(testResult, encoder=CreateEncoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		return;
		}
	CleanupStack::PushL(encoder);
	//		Get the exif encoder
	//
	
	//
	//		Set/Add the Undefiend Gps Tags in the Encoder Metadata
	testResult=SetUndefinedGpsTags(encodeStep, aDataType, encoder->ExifMetadata());
	if (testResult!=KErrNone)
		{
		CleanupStack::PopAndDestroy(encoder);
		TestFailed(testResult);
		return;
		}
	//		Set/Add the Undefiend Gps Tags in the Encoder Metadata
	//
		
	//
	//		Load the reference image
	CFbsBitmap* bitmap=NULL;	
	TRAPD(bmpErr, bitmap=LoadReferenceBitmapL(TFileName(KGPSEncodeSourceFileName)));
	if(bmpErr!=KErrNone)
		{
		CleanupStack::PopAndDestroy(encoder);
		TestFailed(bmpErr);
		return;
		}
	CleanupStack::PushL(bitmap);
	//		Load the reference image
	//
	
	//
	//		encode the image
	testResult=DoImageEncode(encoder, *bitmap);
	CleanupStack::PopAndDestroy(2, encoder); // bitmap
	if (testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//
		
	//
	//		verify the result
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}		
	testResult=SetUndefinedGpsTags(EVerifyData, aDataType, decoder->ExifMetadata());
	delete decoder;
	if (testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	//		verify the result
	//		
	}
	
TInt TGPSImage::SetUndefinedGpsTags(const TExifEncodeStep& aEncodeStep, const TExifDataType& aDataType, MExifMetadata* aMetaData)
	{	
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;

	//These TBuf8s hold the data to be set into the tags.	
	TBuf8<64> gpsProcessingMethod;
	TBuf8<64> gpsAreaInformation;
	
	//These TBufCs hold a description of the test for error message display
	TBufC<64> gpsProcessingMethodErrorMsg;
	TPtr gpsProcessingMethodErrorMsgPtr=gpsProcessingMethodErrorMsg.Des();
	TBufC<64> gpsAreaInformationErrorMsg;
	TPtr gpsAreaInformationErrorMsgPtr=gpsAreaInformationErrorMsg.Des();
		
	switch (aDataType)
		{
		case EAsciiBuffer:
			_LIT8(KGpsProcessingMethod8, "Gps processing method");
			_LIT8(KGpsAreaInformation8, "Gps Area Name");
			
			//Prefix the data to set with the ASCII code
			gpsProcessingMethod.Append(KAscii);
			gpsProcessingMethod.Append(KGpsProcessingMethod8);
			gpsAreaInformation.Append(KAscii);
			gpsAreaInformation.Append(KGpsAreaInformation8);
			
			//Build the error message strings
			gpsProcessingMethodErrorMsgPtr.Append(_L("SetGpsProcessingMethod Ascii"));
			gpsAreaInformationErrorMsgPtr.Append(_L("SetAreaInformation Ascii"));				
			break;
		case EUnicodeBuffer:
			_LIT16(KGpsProcessingMethod16, "Gps processing method");
			_LIT16(KGpsAreaInformation16, "Gps Area Name");
			
			//Prefix the data to set with the UNICODE code
			gpsProcessingMethod.Append(KUnicode);
			gpsProcessingMethod.Append(TPtrC8((const TText8*)KGpsProcessingMethod16().Ptr(),KGpsProcessingMethod16().Size()));
			gpsAreaInformation.Append(KUnicode);
			gpsAreaInformation.Append(TPtrC8((const TText8*)KGpsAreaInformation16().Ptr(),KGpsAreaInformation16().Size()));

			//Build the error message strings
			gpsProcessingMethodErrorMsgPtr.Append(_L("SetGpsProcessingMethod Unicode"));
			gpsAreaInformationErrorMsgPtr.Append(_L("SetAreaInformation Unicode"));
			break;
		default:
			Printf(_L("Unsupported data type %d"), aDataType);
			return KErrArgument;
		}	
		
	if (aEncodeStep==ESetData || aEncodeStep==EAddData)
		{		
		// Add the Undefined Gps Tag values to the metadata
		testResult=SetGpsProcessingMethod(aEncodeStep, aMetaData, gpsProcessingMethod);
		ProcessGPSSetError(testResult, gpsProcessingMethodErrorMsgPtr, aEncodeStep, finalResult);

		testResult=SetGpsAreaInformation(aEncodeStep, aMetaData, gpsAreaInformation);
		ProcessGPSSetError(testResult, gpsAreaInformationErrorMsgPtr, aEncodeStep, finalResult);

		// Check if we should overwrite (Set) the Undefined Gps Tag values.
		if (finalResult==KErrNone && aEncodeStep==ESetData)
			{
			//Do it again - this time it will overwrite (Set) the data.
			testResult=SetGpsProcessingMethod(aEncodeStep, aMetaData, gpsProcessingMethod);
			ProcessGPSSetError(testResult, gpsProcessingMethodErrorMsgPtr, aEncodeStep, finalResult);

			testResult=SetGpsAreaInformation(aEncodeStep, aMetaData, gpsAreaInformation);
			ProcessGPSSetError(testResult, gpsAreaInformationErrorMsgPtr, aEncodeStep, finalResult);
			}
		}
	else
		{
		//Check if the data previously set is what we expect.
		testResult=SetGpsProcessingMethod(EVerifyData, aMetaData, gpsProcessingMethod);
		ProcessGPSSetError(testResult, gpsProcessingMethodErrorMsgPtr, aEncodeStep, finalResult);

		testResult=SetGpsAreaInformation(EVerifyData, aMetaData, gpsAreaInformation);
		ProcessGPSSetError(testResult, gpsAreaInformationErrorMsgPtr, aEncodeStep, finalResult);
		}
		
	return finalResult;
	}
	
// Test for:
//		DEF085007: Exif - CImageTransform cannot process source file with unsupported exif tags
//
void TGPSImage::UnsupportedTags(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt err = KErrNone;
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	Printf(_L("\n"));
	Printf(_L("Transform Setting using source file with unsupported tags %S"), &KImageTestGPSExifUnsupportedTags);
	Printf(_L("Initial thread count %d"), initialThreadCount);
	TFileName sourceFileName(KImageTestGPSExifUnsupportedTags);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName, destFileName, EFalse, ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();

	//
	//		encode the image

	TInt testResult = TestSetGPSData(ESetData, metaData);

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//
	
	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	testResult = TestSetGPSData(EVerifyData, metaData);

	// destroy the decoder
	delete decoder;

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (finalThreadCount != initialThreadCount)
		{
		err = KErrBadHandle;
		TestFailed(err);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

// Test related to:
//		DEF085006: ExifUtility functions GetPixelX/YDimension assume param is LONG
//
// This verifies that one can get XYDim with the low-level GetShortParam interfaces.
TInt TGPSImage::GetPixelXYDimension(MExifMetadata* aMetaData)
	{
	//use with KImageTestGPSExifDecode2 uses short XY tags
	
	const TUint KTagPixelXDimensionID=0xA002;
	const TUint KTagPixelYDimensionID=0xA003;
	// Expected result:
	const TUint16 KPixelXDimention=640;
	const TUint16 KPixelYDimention=480;
	
	TInt testResult=KErrNone;
	TUint16 param = 0;
	
	testResult = aMetaData->GetShortParam(KTagPixelXDimensionID, 0, param);
	if (!testResult)
		{
		if (param != KPixelXDimention)
			{
			testResult = KErrCorrupt;
			}
		}
	if (!testResult)
		{
		testResult = aMetaData->GetShortParam(KTagPixelYDimensionID, 0, param);
		if (!testResult)
			{
			if (param != KPixelYDimention)
				{
				testResult = KErrCorrupt;
				}
			}
		}
	return testResult;	
	}
	
// Test related to:
//		DEF085006: ExifUtility functions GetPixelX/YDimension assume param is LONG
//
// This verifies that one can set XYDim with the low-level SetIntegerParam interfaces.
TInt TGPSImage::SetPixelXYDimension(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	// Expected result:
	const TUint KPixelXDimention=888;
	const TUint KPixelYDimention=444;
	const TUint KTagPixelXDimensionID=0xA002;
	const TUint KTagPixelYDimensionID=0xA003;
	
	TInt testResult=KErrNone;
	TInt param= KPixelXDimention;
	
	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		testResult=aMetaData->SetIntegerParam(KTagPixelXDimensionID, 0, param);
		if (!testResult)
			{
			param = KPixelYDimention;
			testResult=aMetaData->SetIntegerParam(KTagPixelYDimensionID, 0, param);
			}
		}
	else
		{
		testResult=aMetaData->GetIntegerParam(KTagPixelXDimensionID, 0, param);
		if (!testResult)
			{
			if (param != KPixelXDimention)
				{
				testResult = KErrCorrupt;
				}
			}
		if (!testResult)
			{
			testResult=aMetaData->GetIntegerParam(KTagPixelYDimensionID, 0, param);
			if (!testResult)
				{
				if (param != KPixelYDimention)
					{
					testResult = KErrCorrupt;
					}
				}
			}
		}	
	return testResult;
	}
	
// Test related to:
//		DEF085006: ExifUtility functions GetPixelX/YDimention assume param is LONG
//
// Tests that short XYDims can be read and then transformed to longs using the low-level API
void TGPSImage::SetShortPixelXYDimensionTransform(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- XY dimension set transform where source file has SHORT XY dimention tags--- %S"), &KImageTestGPSExifDecode2);
	
	TFileName sourceFileName(KImageTestGPSExifDecode2);//this image uses SHORT Pixel XY dimension
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	
	//first check we can read the existing PixelXY dimention tags which are stored as shorts in the
	//source file.
	TInt testResult = GetPixelXYDimension(metaData);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while getting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	testResult = SetPixelXYDimension(ESetData, metaData);

	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	
	testResult = SetPixelXYDimension(EVerifyData, metaData);
	
	// destroy the decoder
	delete decoder;

	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
// Test for:
//		DEF085006: ExifUtility functions GetPixelX/YDimention assume param is LONG
//
// Verifies that one can read both long and short XYDim using the high-level API.	
void TGPSImage::ExifUtilityPixelXYDimension(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- XY dimension get/set transform where source file has SHORT XY dimention tags using high level exif utility API --- %S"), &KImageTestGPSExifDecode2);
	
	TFileName sourceFileName(KImageTestGPSExifDecode2);//this image uses SHORT Pixel XY dimension
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	
	//first check we can read the existing PixelXY dimention tags which are stored as shorts in the
	//source file.
	//use high level TExifReaderUtility
	// Expected result:
	const TUint KPixelXDimention=640;
	const TUint KPixelYDimention=480;
	
	TInt paramX = 0;
	TInt paramY = 0;
	TExifReaderUtility exifReaderUtility(metaData);
	TInt testResult = exifReaderUtility.GetPixelXDimension(paramX);
	if((testResult!=KErrNone)||(paramX != KPixelXDimention))
		{
		Printf(_L("An error occured while getting Pixel X Dim"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	testResult = exifReaderUtility.GetPixelYDimension(paramY);
	if((testResult!=KErrNone)||(paramY != KPixelYDimention))
		{
		Printf(_L("An error occured while getting Pixel Y Dim"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	
	//now we'll write to it - this should force make XY tags LONG
	const TUint KPixelXDimentionNew=0x888888;
	const TUint KPixelYDimentionNew=0x444444;
	paramX = KPixelXDimentionNew;
	paramY = KPixelYDimentionNew;
	TExifWriterUtility exifWriterUtility(metaData);
	
	testResult = exifWriterUtility.SetPixelXDimension(paramX);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting Pixel X Dim"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	testResult = exifWriterUtility.SetPixelYDimension(paramY);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting Pixel Y Dim"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}		
	testResult = exifReaderUtility.GetPixelXDimension(paramX);
	if((testResult!=KErrNone)||(paramX != KPixelXDimentionNew))
		{
		Printf(_L("An error occured while getting Pixel X Dim"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	testResult = exifReaderUtility.GetPixelYDimension(paramY);
	if((testResult!=KErrNone)||(paramY != KPixelYDimentionNew))
		{
		Printf(_L("An error occured while getting Pixel Y Dim"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}
	
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	metaData=decoder->ExifMetadata();
	
	paramX = 0;
	paramY = 0;
	TExifReaderUtility exifReaderUtilityDecode(metaData);
	testResult = exifReaderUtilityDecode.GetPixelXDimension(paramX);
	if((testResult!=KErrNone)||(paramX != KPixelXDimentionNew))
		{
		Printf(_L("An error occured while getting Pixel X Dim"));
		TestFailed(testResult);
		delete decoder;
		return;
		}
	testResult = exifReaderUtilityDecode.GetPixelYDimension(paramY);
	if((testResult!=KErrNone)||(paramY != KPixelYDimentionNew))
		{
		Printf(_L("An error occured while getting Pixel Y Dim"));
		TestFailed(testResult);
		delete decoder;
		return;
		}
	
	// destroy the decoder
	delete decoder;

	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}

// Gets each gps tag value related to "Dest" (0x13-0x1A) and compares 
// them to the values found in the (fabricated) reference image 
// ExifGPSTransformRefTT.jpg	
TInt TGPSImage::TestGpsDestTagGetters(MExifMetadata* aMetaData)
	{
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	_LIT8(KGpsDestLatitudeRefTestResult, "S");
	testResult = GetGpsDestLatitudeRef(aMetaData, KGpsDestLatitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TRational KGpsDestLatitudeDegTestResult={30, 1};
	const TRational KGpsDestLatitudeMinTestResult={5938, 100};
	const TRational KGpsDestLatitudeSecTestResult={0, 1};
	testResult = GetGpsDestLatitude(aMetaData, KGpsDestLatitudeDegTestResult, KGpsDestLatitudeMinTestResult, KGpsDestLatitudeSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsDestLongitudeRefTestResult, "W");
	testResult = GetGpsDestLongitudeRef(aMetaData, KGpsDestLongitudeRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	const TRational KGpsDestLongitudeDegTestResult={60, 1};
	const TRational KGpsDestLongitudeMinTestResult={3456, 100};
	const TRational KGpsDestLongitudeSecTestResult={0, 1};
	testResult = GetGpsDestLongitude(aMetaData, KGpsDestLongitudeDegTestResult, KGpsDestLongitudeMinTestResult, KGpsDestLongitudeSecTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	_LIT8(KGpsDestBearingRefTestResult, "T");		
	testResult = GetGpsDestBearingRef(aMetaData, KGpsDestBearingRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
	
	const TInt KGpsDestBearingNumeratorTestResult=180;
	const TInt KGpsDestBearingDenominatorTestResult=30;
	testResult = GetGpsDestBearing(aMetaData, KGpsDestBearingNumeratorTestResult, KGpsDestBearingDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}

	_LIT8(KGpsDestDistanceRefTestResult, "K");
	testResult = GetGpsDestDistanceRef(aMetaData, KGpsDestDistanceRefTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
	
	const TInt KGpsDestDistanceNumeratorTestResult=50;
	const TInt KGpsDestDistanceDenominatorTestResult=5;
	testResult = GetGpsDestDistance(aMetaData, KGpsDestDistanceNumeratorTestResult, KGpsDestDistanceDenominatorTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}	
		
	return finalResult;
	}
	
// Gets each gps tag value specific to Exif ver2.2 (0x1B-0x1E) and compares 
// them to the values found in the (fabricated) reference image 
// ExifGPSTransformRefTT.jpg	
TInt TGPSImage::TestGpsVer22TagGetters(MExifMetadata* aMetaData)
	{
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	//Tests getting UNICODE data for this tag, so we expect to find the UNICODE
	//prefix at the start of the data.
	TBuf8<64> expectedGpsProcessingMethod;
	_LIT16(KGpsProcessingMethod16, "Gps processing method");
	expectedGpsProcessingMethod.Append(KUnicode);
	expectedGpsProcessingMethod.Append(TPtrC8((const TText8*)KGpsProcessingMethod16().Ptr(),KGpsProcessingMethod16().Size()));
	testResult = GetGpsProcessingMethod(aMetaData, expectedGpsProcessingMethod);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	//Tests getting UNICODE data for this tag
	TBuf8<64> expectedGpsAreaInformation;
	_LIT16(KGpsAreaInformation16, "Gps Area Name");
	expectedGpsAreaInformation.Append(KUnicode);
	expectedGpsAreaInformation.Append(TPtrC8((const TText8*)KGpsAreaInformation16().Ptr(),KGpsAreaInformation16().Size()));
	testResult = GetGpsAreaInformation(aMetaData, expectedGpsAreaInformation);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	_LIT8(KGpsDateStampTestResult, "2006:02:18");
	testResult = GetGpsDateStamp(aMetaData, KGpsDateStampTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	TUint16 KGpsDifferentialTestResult = 1;
	testResult = GetGpsDifferential(aMetaData, KGpsDifferentialTestResult);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	return finalResult;	
	}

// Test for: DEF085004: Exif - Big endian byte and undefined tag data not handled correctly.
//
// Transforms a Big Endian source and verifies that tags of format "byte" or "undefined"
// of four bytes or less are not mangled on their conversion to Little Endian.
void TGPSImage::TestTransformExifBigEndian(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform GPS Big Endian --- %S"),&KImageTestGPSExifDecode3);

	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestGPSExifDecode3);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();
	//		Get the metadata
	//
	
	//
	//		Check that the data in the source is as expected
	
	// GpsVersionId is of byte format, length 4
	_LIT8(KSourceGpsVersion, "\2\0\0\0");
	testResult = GetGpsVersionId(metaData, KSourceGpsVersion);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	// ExifVersionId is of undefined format, length 4
	_LIT8(KSourceExifVersion, "0210");
	testResult = GetExifVersionId(metaData, KSourceExifVersion);
	if (testResult != KErrNone)
		{
		finalResult = testResult;
		}
		
	if(finalResult!=KErrNone)
		{
		CleanupStack::PopAndDestroy(imageTransform);
		TestFailed(finalResult);
		return;
		}	
	//	Check that the data in the source is as expected
	//
		
	//
	//		Modify the data
	testResult = SetGpsVersionId(ESetData, metaData);
	ProcessGPSSetError(testResult, _L("SetGpsVersionId"), ESetData, finalResult);
		
	testResult = SetExifVersionId(ESetData, metaData);
	ProcessGPSSetError(testResult, _L("SetExifVersionId"), ESetData, finalResult);
		
	if(finalResult!=KErrNone)
		{
		CleanupStack::PopAndDestroy(imageTransform);
		TestFailed(finalResult);
		return;
		}
		
	//Do the transform - the dest image will be encoded Little Endian
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;		
		}
	//		Modify the data
	//
	
	
	//	
	// 		verify the data with the destination metadata
	
	// get the destination metadata
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(destFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	metaData=decoder->ExifMetadata();

	// Verify that it has been set as expected.
	testResult = SetGpsVersionId(EVerifyData, metaData);
	ProcessGPSSetError(testResult, _L("SetGpsVersionId"), EVerifyData, finalResult);
		
	testResult = SetExifVersionId(EVerifyData, metaData);
	ProcessGPSSetError(testResult, _L("SetExifVersionId"), EVerifyData, finalResult);
		
	delete decoder;
	//	verify the data with the destination metadata
	//		

	//
	//		Log the result
	if(finalResult!=KErrNone)
		{
		TestFailed(finalResult);
		}
	//		Log the result
	//
	}
	
// Test for:
//		DEF082166: Exif : User comment tag incorrect descriptor length
// Verifies that the the length of the Unicode GPS Processing method is correctly read.
void TGPSImage::TestUnicodeDescriptorLengthGpsTag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Unicode Descriptor Length Test GPS --- %S"),&KImageTestGPSExifTransformRefFN);
	// Tag == 0x001b == GPS Processing Method.
	// Expected length of unicode data == 25
	TestUnicodeDescriptorLength(TFileName(KImageTestGPSExifTransformRefFN), 0x001b, 25);	
	}

// Test for:
//		DEF082166: Exif : User comment tag incorrect descriptor length
// Verifies that the the length of the Unicode User Comment is correctly read.	
void TGPSImage::TestUnicodeDescriptorLengthUserComment(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	Printf(_L("\n"));
	Printf(_L("--- Unicode Descriptor Length Test User Comment --- %S"),&KImageTestExifTransformSource);
	// Tag == 0x9286 == User Comment.
	// Expected length of unicode data == 13
	TestUnicodeDescriptorLength(TFileName(KImageTestExifTransformSource), 0x9286, 13);	
	}
	
void TGPSImage::TestUnicodeDescriptorLength(const TFileName aSourceFileName, const TInt aTag, const TInt aExpected)	
	{	
	//
	//		Get the transform
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(aSourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the transform
	//

	Printf(_L("--- Transform ---"));
	DoUnicodeDescriptorLengthTest(extension->ExifMetadata(), aTag, aExpected);
	delete imageTransform;
	
	//
	//		Get the decoder
	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(aSourceFileName));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	//		Get the decoder
	//
			
	Printf(_L("--- Decoder ---"));
	DoUnicodeDescriptorLengthTest(decoder->ExifMetadata(), aTag, aExpected);
	delete decoder;
	}
	
void TGPSImage::DoUnicodeDescriptorLengthTest(MExifMetadata* aMetaData, const TInt aTag, const TInt aExpected)
	{
	ASSERT(aMetaData!=NULL);
	
	TInt ifd = 0;
	HBufC16* param = NULL;
	
	TInt testResult=aMetaData->GetParam16(aTag, ifd, param);
	if (testResult!=KErrNone)
		{
		Printf(_L("GetParam16 failed %d"), testResult);
		TestFailed(testResult);
		return;
		}
		
	if (param->Length() != aExpected)
		{
		Printf(_L("Wrong length %d"), param->Length());
		TestFailed(KErrCorrupt);			
		}
	delete param;
	}
	
// Test for:
//		DEF082167: Exif : Incorrectly assumes Tag IDs are unique.
//		
void TGPSImage::TestConflictingTags(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	//
	//		Get the exif encoder
	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
	TFileName fileName(KImageTestExifGPSTransformDest);
	
	TRAPD(testResult,
		CreateEncoderFromReferenceBitmapFileL(KGPSEncodeSourceFileName, bitmap, fileName, encoder);
		)
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		return;
		}
	//		Get the exif encoder
	//
	
	//Set the data
	testResult=DoConflictingTagsTest(ESetData, encoder->ExifMetadata());
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}
							
	//
	//		encode the image
	//.
	testResult=DoImageEncode(encoder, *bitmap);
	delete bitmap;
	delete encoder;
	bitmap=NULL;
	encoder=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("Encoding failed."));
		TestFailed(testResult);
		return;
		}		
	//		encode the image
	//
	
	//Verify the data
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	testResult=DoConflictingTagsTest(EVerifyData, decoder->ExifMetadata());
	delete decoder;
	decoder=NULL;
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}
	}
	
// Test for:
//		DEF082167: Exif : Incorrectly assumes Tag IDs are unique.
//		
void TGPSImage::TestConflictingTagsTransform(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)	
	{
	//
	//		Get the transform
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the transform
	//
	
	//Set the data
	TInt testResult=DoConflictingTagsTest(ESetData, extension->ExifMetadata());
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}
		
	//
	//		transform the image
	//.
	testResult=DoImageTransformL(imageTransform);
	delete imageTransform;
	imageTransform=NULL;
	extension=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("ImageTransform failed."));
		TestFailed(testResult);
		return;
		}		
	//		transform the image
	//
	
	//To verify the data we use a new imageTransform with source equal
	//to the image we just created in the last transform.
	//
	//		Get the transform
	TFileName dummyDestFileName(KImageTestExifGPSTransformDest2);

	TRAP(allocErr,imageTransform=PrepareImageTransformL(destFileName,dummyDestFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	
	extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the transform
	//
	
	//Verify the data
	testResult=DoConflictingTagsTest(EVerifyData, extension->ExifMetadata());
	delete imageTransform;
	imageTransform=NULL;
	extension=NULL;
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}	
	}
	
// Tries to Set or Get (verify) the non-unique Tags 0x0001 & 0x0002 for
// each supported subIfd.  For the InterOp Ifd, these tags correspond to
// interoperabilityIndex and InteroperabilityVersion.  For the Gps Ifd, the
// correspond to LatitudeRef and Latitude.
TInt TGPSImage::DoConflictingTagsTest(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;
	
	testResult=SetInteroperabilityIndex(aEncodeStep, aMetaData);
	if(testResult!=KErrNone)
		{
		finalResult=testResult;
		}
	testResult=SetInteroperabilityVersion(aEncodeStep, aMetaData);
	if(testResult!=KErrNone)
		{
		finalResult=testResult;
		}
	testResult=SetGpsLatitudeRef(aEncodeStep, aMetaData);
	if(testResult!=KErrNone)
		{
		finalResult=testResult;
		}
	testResult=SetGpsLatitude(aEncodeStep, aMetaData);
	if(testResult!=KErrNone)
		{
		finalResult=testResult;
		}	

	return finalResult;
	}
	
//
// Test for:
//		DEF085002: Exif : setting a short array exif tag can cause code to panic
//
void TGPSImage::TestShortArrayTag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Short Array Tag ---"));

	//
	//		Get the exif encoder
	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
	TFileName fileName(KImageTestExifGPSTransformDest);
	
	TRAPD(testResult,
		CreateEncoderFromReferenceBitmapFileL(KGPSEncodeSourceFileName, bitmap, fileName, encoder);
		)
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		return;
		}
	//		Get the exif encoder
	//

	//This tag is not present in any of our images, so we have to Set it twice.
	//The first time the tag and data are created and added.  The second time,
	//the data is modified (and this is where the defect occurs).
	testResult = SetSubjectLocation(ESetData, encoder->ExifMetadata());
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while adding data"));
		TestFailed(testResult);
		delete bitmap;
		delete encoder;
		return;
		}
	testResult = SetSubjectLocation(ESetData, encoder->ExifMetadata());
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		delete bitmap;
		delete encoder;
		return;
		}	

	//
	//		encode the image
	//.
	testResult=DoImageEncode(encoder, *bitmap);
	delete bitmap;
	delete encoder;
	bitmap=NULL;
	encoder=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("Encoding failed."));
		TestFailed(testResult);
		return;
		}		
	//		encode the image
	//

	//
	//		verify the result
	
	// open the resulting file 
	CJPEGExifDecoder* decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify the data
	testResult = SetSubjectLocation(EVerifyData, decoder->ExifMetadata());
	
	// destroy the decoder
	delete decoder;
	decoder=NULL;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
//
// Test for:
//		DEF085002: Exif : setting a short array exif tag can cause code to panic
//
void TGPSImage::TestShortArrayTagTransform(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Short Array Tag Transform---"));

	//
	//		Get the transform
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	TFileName destFileName(KImageTestExifGPSTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the transform
	//

	//This tag is not present in any of our images, so we have to Set it twice.
	//The first time the tag and data are created and added.  The second time,
	//the data is modified (and this is where the defect occurs).
	TInt testResult = SetSubjectLocation(ESetData, extension->ExifMetadata());
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while adding data"));
		TestFailed(testResult);
		delete imageTransform;
		return;
		}
	testResult = SetSubjectLocation(ESetData, extension->ExifMetadata());
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		delete imageTransform;
		return;
		}	
	//
	//		transform the image
	//.
	testResult=DoImageTransformL(imageTransform);
	delete imageTransform;
	imageTransform=NULL;
	extension=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("ImageTransform failed."));
		TestFailed(testResult);
		return;
		}		
	//		transform the image
	//	

	//
	//		Get the transform for verification
	TFileName dummyDestFileName(KImageTestExifGPSTransformDest2);

	imageTransform=NULL;
	TRAP(allocErr,imageTransform=PrepareImageTransformL(destFileName,dummyDestFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	
	extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the transform for verification
	//
	
	// verify the data
	testResult = SetSubjectLocation(EVerifyData, extension->ExifMetadata());
	
	delete imageTransform;
	imageTransform=NULL;
	extension=NULL;

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}
	
TInt TGPSImage::SetSubjectLocation(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData)
	{
	CArrayFixFlat<TUint16>* value=NULL;
	TInt err=KErrNone;
	TRAP(err, value=new (ELeave) CArrayFixFlat<TUint16>(2));
	if(err!=KErrNone)
		{
		return err;
		}
	const TUint KSubjectLocationTagID=0xA214;

	// Expected result:
	TUint16 KShort1TestResult= 0x99; 
	TUint16 KShort2TestResult= 0x66;
	
	if(aEncodeStep==ESetData)
		{
		// set the data
		TRAP(err, 
			{
			value->SetReserveL(2);			
			value->AppendL(KShort1TestResult);
			value->AppendL(KShort2TestResult);
			});
		if(err==KErrNone)
			{
			err=aMetaData->SetShortArrayParam(KSubjectLocationTagID, 0, *value);
			}
		}
	else
		{	// get the encoded data
		err=aMetaData->GetShortArrayParam(KSubjectLocationTagID, 0, *value);
		if(	(err==KErrNone)		&&
			((value->At(0) != KShort1TestResult)		||
			 (value->At(1) != KShort2TestResult)	)
			)	
			{
			err=KErrCorrupt;
			}
		}	
	delete value;

	return err;
	}

//
// Verify that encoding a jpeg with GPS IFD does not affect the thumbnail
//
void TGPSImage::ValidateThumbnailAndGpsIfd(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Validate Thumbnail+GPS over encode ---"));
	
	//The image to encode 
	_LIT(KEncodeSourceFileName, 	"c:\\TSU_ICL_cod_04\\EncodeTestSource.mbm");
	//The expected thumbnail.
	_LIT(KDecodeThumbnailFileName, "c:\\TSU_ICL_cod_04\\EncodeThumbnail.mbm");

	//
	//		Get the exif encoder
	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
	TFileName fileName(KImageTestExifGPSTransformDest);
	
	TRAPD(testResult,
		CreateEncoderFromReferenceBitmapFileL(KEncodeSourceFileName, bitmap, fileName, encoder);
		)
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		return;
		}
	//		Get the exif encoder
	//
	
	//Set any GPS tag in order to force GPS IFD in image
	testResult=SetGpsVersionId(ESetData, encoder->ExifMetadata());
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}
							
	//
	//		encode the image
	//.
	testResult=DoImageEncode(encoder, *bitmap);
	delete bitmap;
	delete encoder;
	bitmap=NULL;
	encoder=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("Encoding failed."));
		TestFailed(testResult);
		return;
		}		
	//		encode the image
	//

	//Verify the result
	//This will attempt to extract the thumbnail from filename and compare
	//that it is the same as KDecodeThumbnailFileName
	TestGetThumbnail(fileName, KDecodeThumbnailFileName);
	}	
	
void TGPSImage::ProcessGPSSetError(const TInt& aError, const TDesC& aTestDescription, const TExifEncodeStep& aEncodeStep, TInt& aResult)
	{
	TBufC<256> errorMsg;
	TPtr msgPtr=errorMsg.Des();
	msgPtr.Append(aTestDescription);
	
	switch(aEncodeStep)
		{
		case EVerifyData:
			msgPtr.Append(_L(" Verify "));
			break;
		case ESetData:
			msgPtr.Append(_L(" Set "));
			break;
		case EAddData:
			msgPtr.Append(_L(" Add "));
			break;
		}
	
	switch(aError)
		{
		case KErrNone:
			//msgPtr.Append(_L("OK"));
			//break;
			return;	
		case KErrNotFound:
			msgPtr.Append(_L("failed (Not found)"));
			break;
		case KErrNotSupported:
			msgPtr.Append(_L("failed (Not supported)"));
			break;
		case KErrArgument:
			msgPtr.Append(_L("failed (invalid argument)"));
			break;
		case KErrOverflow:
			msgPtr.Append(_L("failed (overflow)"));
			break;
		case KErrUnderflow:
			msgPtr.Append(_L("failed (UnderFlow)"));
			break;
		}
		
	Printf(errorMsg);
	
	if (aError!=KErrNone)
		{
		aResult=aError;
		}
	}
	
CFbsBitmap* TGPSImage::LoadReferenceBitmapL(const TFileName& aSourceFileName)
	{
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	TInt bmpErr=bitmap->Load(aSourceFileName);
	if(bmpErr!=KErrNone)
		{
		delete bitmap;
		Printf(_L("could not load ref bitmap file"));
		User::Leave(bmpErr);
		}
	return bitmap;
	}

TExifEncodeStep ExifOpToExifEncodeStepL(const TInt aExifOperation)
	{
	TExifEncodeStep result=ESetData;
	switch (aExifOperation)
		{
		case ESetParam:
			result=ESetData;
			break;
		case EAddParam:
			result=EAddData;
			break;
		case EGetParam:
			result=EVerifyData;
			break;
		default:
			User::Leave(KErrArgument);
		}
	return result;
	}

void TGPSImage::FileOpenMode(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	//MM-ICL-COD-U-1200-HP
	//This test tests the file open mode fix to verify that a CImageTransform 
	//can be perfomed after a CImageDecode using the same source file as the CImageDecode - see INC094070
	//1. Create the decoder
	//2. Create the ImageTransform before calling convert() on decoder
	//3. Setup and Call Transform()
	//4. Call Convert() on decoder
	__MM_HEAP_MARK;
	Printf(_L("\n"));
	Printf(_L("--- File Open mode test1 %S"), &KImageTestGPSExifDecode2);
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	TInt err = KErrNone;
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	err = FileOpenModeTest(sourceFileName, EFalse ,CImageTransform::EOptionNone);
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (err != KErrNone)
		{
		Printf(_L("Expected KErrNone Got  %d"), err);
		TestFailed(err);
		}
	else if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}		

	
TInt TGPSImage::FileOpenModeTest(TFileName& aSourceFileName, TBool aPreserveImageData, TUint aOptions)
	{
	TFileName sourceFileName(aSourceFileName);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	//Create the decoder
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(sourceFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		return testResult;
		}
	CleanupStack::PushL(decoder);
	TRAP(testResult, decoder->SetImageTypeL(CImageDecoder::EImageTypeMain))
	if(testResult!=KErrNone)
		{
		delete decoder;
		Printf(_L("Could not set image type on the decoder"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(bitmap);
		return testResult;
		}
 
   	User::LeaveIfError(bitmap->Create(decoder->FrameInfo().iOverallSizeInPixels, EColor16));

    //Create the ImageTransform without calling Convert() on decoder
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr, 
		{
		imageTransform=CImageTransform::NewL(iFs);
		});
	
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not do Create imageTransform"));
		if(imageTransform)
			{
			delete imageTransform;
			}
		return allocErr;
		}
	CleanupStack::PushL(imageTransform);
	
	TRAPD(testErr, imageTransform->SetSourceFilenameL(sourceFileName));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set the source descriptor in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		CleanupStack::PopAndDestroy(decoder);
		CleanupStack::PopAndDestroy(bitmap);
		return testErr;
		}
		
	// Setup the imagetransform
	TRAP(testErr, imageTransform->SetDestFilenameL(destFileName));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest descriptor in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		CleanupStack::PopAndDestroy(decoder);
		CleanupStack::PopAndDestroy(bitmap);
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetDestSizeInPixelsL(TSize(160, 120), ETrue));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest size in pixels imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		CleanupStack::PopAndDestroy(decoder);
		CleanupStack::PopAndDestroy(bitmap);
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetOptionsL(aOptions));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set options imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		CleanupStack::PopAndDestroy(decoder);
		CleanupStack::PopAndDestroy(bitmap);
		return testErr;
		}
		
	imageTransform->SetPreserveImageData(aPreserveImageData);
	TRAP(testErr, imageTransform->SetupL());
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		CleanupStack::PopAndDestroy(decoder);
		CleanupStack::PopAndDestroy(bitmap);
		return testErr;
		}

	// Call Transform()
	CActiveTransformListener* listener= new (ELeave) CActiveTransformListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener(imageTransform);
	imageTransform->Transform(listener->iStatus);
	CActiveScheduler::Start();
	
	TInt transformResult=listener->iStatus.Int();
	if(transformResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		return transformResult;		
		}
		
	// Call Convert() on decoder
	CActiveDecodeListener decodeListener;
	decodeListener.InitialiseActiveListener(decoder);
	decoder->Convert(& decodeListener.iStatus, *bitmap);
	CActiveScheduler::Start();
	testResult = decodeListener.iStatus.Int();
	if(testResult!=KErrNone)
		{
		Printf(_L("error decoding the file"));
		TestFailed(decodeListener.iStatus.Int());
		CleanupStack::PopAndDestroy(bitmap);
		return testResult;
		}
		
	CleanupStack::PopAndDestroy(4, bitmap);	// listener, imageTransform, decoder, bitmap
	return testResult;
	}


void TGPSImage::FileTransformMode1(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{//MM-ICL-COD-U-1201-HP
	//This test tests the file open mode fix to verify that a CImageTransform 
	//can be perfomed after a CImageDecode using the same source file as the CImageDecode - see INC094070
	//1. Create and set up the CImageTransform
	//2. Call the Transform
	//3. Create the decoder
	//4. Call Convert on decoder
	__MM_HEAP_MARK;
	Printf(_L("\n"));
	Printf(_L("--- File Trasnform mode test1 %S"), &KImageTestGPSExifDecode2);
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	TInt err = KErrNone;
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	err = FileTransformTest1(sourceFileName, EFalse ,CImageTransform::EOptionNone);
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (err != KErrNone)
		{
		Printf(_L("Expected KErrNone Got  %d"), err);
		TestFailed(err);
		}
	else if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}		

	
TInt TGPSImage::FileTransformTest1(TFileName& aSourceFileName, TBool aPreserveImageData, TUint aOptions)
	{
	TFileName sourceFileName(aSourceFileName);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	//Create the Transform
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr, 
		{
		imageTransform=CImageTransform::NewL(iFs);
		});
	
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not do Create imageTransform"));
		if(imageTransform)
			{
			delete imageTransform;
			}
		return allocErr;
		}
	CleanupStack::PushL(imageTransform);
	
	//Setup the transform
	TRAPD(testErr, imageTransform->SetSourceFilenameL(sourceFileName));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set the source descriptor in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
			
	TRAP(testErr, imageTransform->SetDestFilenameL(destFileName));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest descriptor in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetDestSizeInPixelsL(TSize(160, 120), ETrue));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest size in pixels imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetOptionsL(aOptions));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set options imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
		
	imageTransform->SetPreserveImageData(aPreserveImageData);
	TRAP(testErr, imageTransform->SetupL());
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}

	// launch Transform()
	CActiveTransformListener* listener= new (ELeave) CActiveTransformListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener(imageTransform);
	imageTransform->Transform(listener->iStatus);
	CActiveScheduler::Start();
	
	TInt transformResult=listener->iStatus.Int();
	if(transformResult!=KErrNone)
		{
		Printf(_L("error decoding the file"));
		TestFailed(listener->iStatus.Int());
		CleanupStack::PopAndDestroy(imageTransform);
		return transformResult;
		}
	
	//Create the decoder
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(sourceFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		return testResult;
		}
	CleanupStack::PushL(decoder);
		
	TRAP(testResult, decoder->SetImageTypeL(CImageDecoder::EImageTypeMain))
	if(testResult!=KErrNone)
		{
		delete decoder;
		Printf(_L("Could not set image type on the decoder"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(bitmap);
		CleanupStack::PopAndDestroy(imageTransform);
		return testResult;
		}
   	
   	User::LeaveIfError(bitmap->Create(decoder->FrameInfo().iOverallSizeInPixels, EColor16));
   	
   	// Call Convert() on decoder
	CActiveDecodeListener decodeListener;
	decodeListener.InitialiseActiveListener(decoder);
	decoder->Convert(& decodeListener.iStatus, *bitmap);
	CActiveScheduler::Start();
	
	testResult = decodeListener.iStatus.Int();
	CleanupStack::PopAndDestroy(4, imageTransform);	// listener, imageTransform, decoder, bitmap	
	
	if(testResult!=KErrNone)
		{
		Printf(_L("error decoding the file"));
		TestFailed(decodeListener.iStatus.Int());
		return testResult;
		}

	return testResult;
	}

void TGPSImage::FileTransformMode2(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{//MM-ICL-COD-U-1202-HP
	//This test tests the file open mode fix to verify that a CImageTransform 
	//can be perfomed after a CImageDecode using the same source file as the CImageDecode - see INC094070
	//1. Create and set up the CImageTransform
	//2. Create the decoder 
	//3. Call the Transform
	//4. Call Convert on decoder
	__MM_HEAP_MARK;
	Printf(_L("\n"));
	Printf(_L("--- File Transform mode test2 %S"), &KImageTestGPSExifDecode2);
	TInt initialThreadCount = 0;
	TInt finalThreadCount = 0;
	TInt processCount = 0;
	TInt err = KErrNone;
	TFileName sourceFileName(KImageTestGPSExifDecode2);
	RThread thisThread;	
	thisThread.HandleCount(processCount, initialThreadCount);
	err = FileTransformTest1(sourceFileName, EFalse ,CImageTransform::EOptionNone);
	REComSession::FinalClose();
	thisThread.HandleCount(processCount, finalThreadCount);
	Printf(_L("Final thread count %d"), finalThreadCount);
	if (err != KErrNone)
		{
		Printf(_L("Expected KErrNone Got  %d"), err);
		TestFailed(err);
		}
	else if (finalThreadCount != initialThreadCount)
		{
		TestFailed(KErrBadHandle);
		}
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}		

	
TInt TGPSImage::FileTransformTest2(TFileName& aSourceFileName, TBool aPreserveImageData, TUint aOptions)
	{
	TFileName sourceFileName(aSourceFileName);
	TFileName destFileName(KImageTestExifGPSTransformDest);
	
	//Create the transform
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr, 
		{
		imageTransform=CImageTransform::NewL(iFs);
		});
	
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not do Create imageTransform"));
		if(imageTransform)
			{
			delete imageTransform;
			}
		return allocErr;
		}
	CleanupStack::PushL(imageTransform);
	
	TRAPD(testErr, imageTransform->SetSourceFilenameL(sourceFileName));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set the source descriptor in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
		
	// Setup the Transform
	TRAP(testErr, imageTransform->SetDestFilenameL(destFileName));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest descriptor in imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);	
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetDestSizeInPixelsL(TSize(160, 120), ETrue));
	if(testErr != KErrNone)
		{
		Printf(_L("Could not set the dest size in pixels imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
	
	TRAP(testErr, imageTransform->SetOptionsL(aOptions));
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not set options imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
		
	imageTransform->SetPreserveImageData(aPreserveImageData);


	TRAP(testErr, imageTransform->SetupL());
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testErr;
		}
	
	//Create the decoder
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(sourceFileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		return testResult;
		}
	CleanupStack::PushL(decoder);
		
	TRAP(testResult, decoder->SetImageTypeL(CImageDecoder::EImageTypeMain))
	if(testResult!=KErrNone)
		{
		delete decoder;
		Printf(_L("Could not set image type on the decoder"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(bitmap);
		CleanupStack::PopAndDestroy(imageTransform);
		return testResult;
		}
   	
   	User::LeaveIfError(bitmap->Create(decoder->FrameInfo().iOverallSizeInPixels, EColor16));
   	
   	// Call Transform()
	CActiveTransformListener* listener= new (ELeave) CActiveTransformListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener(imageTransform);
	imageTransform->Transform(listener->iStatus);
	CActiveScheduler::Start();
	
	TInt transformResult=listener->iStatus.Int();
	if(transformResult!=KErrNone)
		{
		Printf(_L("error decoding the file"));
		TestFailed(listener->iStatus.Int());
		CleanupStack::PopAndDestroy(imageTransform);
		return transformResult;
		}
		
   //Call Covert() on decoder
	CActiveDecodeListener decodeListener;
	decodeListener.InitialiseActiveListener(decoder);
	decoder->Convert(& decodeListener.iStatus, *bitmap);
	CActiveScheduler::Start();
	
	testResult = decodeListener.iStatus.Int();
	CleanupStack::PopAndDestroy(4, imageTransform);	// listener, imageTransform, decoder, bitmap	
	
	if(testResult!=KErrNone)
		{
		Printf(_L("error decoding the file"));
		TestFailed(decodeListener.iStatus.Int());
		return testResult;
		}

	return testResult;
	}
