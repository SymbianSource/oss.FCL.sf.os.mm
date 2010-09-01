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

#ifndef __TGPSIMAGE_H__
#define __TGPSIMAGE_H__

#include "TImage.h"
#include "iclexif.h"

class MExifMetadata;
class CJPEGExifDecoder;
class CJPEGExifEncoder;

class TGPSImage : public TImage//RTestExtra
	{
public:
	TGPSImage(const TPerformanceTestParams* aPerfTstParams, const TDesC& aTestName);
	void ConstructL();
	void Test();
	void End();
	
	// Tests getting Gps Tags from Decoder metadata
	void GetGPSExifData(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSExifData2(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSExifData3(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSExifData4(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSExifData5(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSExifData6(const TExifDataType& aDataType, const TExifOperation& aOperation);

	// Tests getting Gps Tags from Transformer metadata
	void GetGPSTransformExif(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSTransformExif2(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSTransformExif3(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSTransformExif4(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GetGPSTransformExif5(const TExifDataType& aDataType, const TExifOperation& aOperation);

	// Tests setting Gps Tags from Encoder metadata
	void SetGPSExifData(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void AddGPSExifData(const TExifDataType& aDataType, const TExifOperation& aOperation);
	
	// Tests setting Gps Tags from Transformer metadata
	void SetGPSTransformExif(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void AddGPSTransformExif(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformNoGpsTag(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformNoGpsBadCountTag(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformNoGpsBadCountTagOptionNone(const TExifDataType& aDataType, const TExifOperation& aOperation);

	// Tests setting those tags with "undefined" value types, for Encoder & Transformer metadata
	void SetTransformUndefinedGpsTags(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetEncoderUndefinedGpsTags(const TExifDataType& aDataType, const TExifOperation& aOperation);

	// Extra tests
	void GPSExifDataRegression(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void GPSTransformDeleteAndSetup(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void ValidateThumbnailAndGpsIfd(const TExifDataType& aDataType, const TExifOperation& aOperation);
	
	/* DEF082168 - Tests descriptor source/dests */
	void SetGPSTransformExifDescriptorSrcTN(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcFN(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcTT(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcFT(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcDstTN(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcDstFN(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcDstTT(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorSrcDstFT(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorDstTN(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorDstFN(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorDstTT(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetGPSTransformExifDescriptorDstFT(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void CancelTransformExifDescriptorSrc(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void CancelTransformExifDescriptorSrcDst(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void CancelTransformExifDescriptorDst(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TransformJpegDescSource(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	void TransformThumblessExifDescSource(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	
	// Misc Defect Tests
	void GPSLatitudeRefCount(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// DEF085007
	void UnsupportedTags(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// DEF085006
	void ShortPixelXYDimension(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void SetShortPixelXYDimensionTransform(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void ExifUtilityPixelXYDimension(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// DEF082163
	void CopyrightNullTest(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void CopyrightNullTestTransform(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// DEF082161
	void ThumbnailEOI(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// DEF083701
	void CancelTransform(const TExifDataType& aDataType, const TExifOperation& aOperation);
	// DEF085004
	void TestTransformExifBigEndian(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	// DEF082166		
	void TestUnicodeDescriptorLengthGpsTag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);	
	void TestUnicodeDescriptorLengthUserComment(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	// DEF082167
	void TestConflictingTags(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);			
	void TestConflictingTagsTransform(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/);
	// DEF085002
	void TestShortArrayTag(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void TestShortArrayTagTransform(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void FileOpenMode(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void FileTransformMode1(const TExifDataType& aDataType, const TExifOperation& aOperation);
	void FileTransformMode2(const TExifDataType& aDataType, const TExifOperation& aOperation);
private:
	TInt GetGpsVersionId(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsLatitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsLatitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected);
	TInt GetGpsLongitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsLongitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected);
	TInt GetGpsTimeStamp(MExifMetadata* aMetaData, const TRational& aHourExpected, const TRational& aMinExpected, const TRational& aSecExpected);
	TInt GetGpsAltitudeRef(MExifMetadata* aMetaData, const TUint8 aExpected);
	TInt GetGpsAltitude(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsSatellites(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsStatus(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsMeasureMode(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDop(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsSpeedRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsSpeed(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsTrackRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsTrack(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsImgDirectionRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsImgDirection(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsMapDatum(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDestLatitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDestLatitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected);
	TInt GetGpsDestLongitudeRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDestLongitude(MExifMetadata* aMetaData, const TRational& aDegExpected, const TRational& aMinExpected, const TRational& aSecExpected);
	TInt GetGpsDestBearingRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDestBearing(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsDestDistanceRef(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDestDistance(MExifMetadata* aMetaData, const TInt aNumExpected, const TInt aDemExpected);
	TInt GetGpsProcessingMethod(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsAreaInformation(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDateStamp(MExifMetadata* aMetaData, const TDesC8& aExpected);
	TInt GetGpsDifferential(MExifMetadata* aMetaData, const TUint16 aExpected);
	TInt GetExifVersionId(MExifMetadata* aMetaData, const TDesC8& aExpected);
	
	TInt SetGpsVersionId(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);	
	TInt SetGpsLatitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsLatitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsLongitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsLongitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsAltitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsAltitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsTimeStamp(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsSatellites(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsStatus(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsMeasureMode(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDop(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsSpeedRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsSpeed(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsTrackRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsTrack(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsImgDirectionRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsImgDirection(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsMapDatum(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestLatitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestLatitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestLongitudeRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestLongitude(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestBearingRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestBearing(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestDistanceRef(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDestDistance(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);		
	TInt SetGpsProcessingMethod(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TDesC8& aData);
	TInt SetGpsAreaInformation(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TDesC8& aData);
	TInt SetGpsDateStamp(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetGpsDifferential(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetExifVersionId(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetInteroperabilityIndex(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetInteroperabilityVersion(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);	

	TInt SetUndefinedGpsTags(const TExifEncodeStep& aEncodeStep, const TExifDataType& aDataType, MExifMetadata* aMetaData);
	TInt SetCopyright(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetCopyrightEditor(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetSubjectLocation(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt SetPixelXYDimension(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt GetPixelXYDimension(MExifMetadata* aMetaData);
	
	void CommonTransformGetPixelSizeL(const TDesC& aSrcFileName,TSize& aBitmapSize);
	TInt ThumbnailEOITransform(TFileName& aSourceFileName, TFileName& aDestFileName);

	/* DEF082168 - Tests descriptor source/dests */
	void SetGPSTransformExifDescriptorSrc(TBool aPreserveImageData, TBool aThumbnail);
	void SetGPSTransformExifDescriptorSrcDst(TBool aPreserveImageData, TBool aThumbnail, TFileName& aSourceFileName);
	void SetGPSTransformExifDescriptorDst(TBool aPreserveImageData, TBool aThumbnail);
	void GenericTransformDescSrcTest(TBool aPreserveImageData, TBool aThumbnail, TFileName& aSourceFileName);
	
	TInt TestSetGPSData(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	TInt TestGpsDestTagGetters(MExifMetadata* aMetaData);
	TInt TestGpsVer22TagGetters(MExifMetadata* aMetaData);
	
	void TestUnicodeDescriptorLength(const TFileName aSourceFileName, const TInt aTag, const TInt aExpected);
	void DoUnicodeDescriptorLengthTest(MExifMetadata* aMetaData, const TInt aTag, const TInt aExpected);

	TInt DoConflictingTagsTest(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData);
	void DoCancelTransformTest(const TExifTransformSourceDest& aSource, const TExifTransformSourceDest& aDest);

	void ProcessGPSSetError(const TInt& aError, const TDesC& aTestDescription, const TExifEncodeStep& aEncodeStep, TInt& aResult);
	CFbsBitmap* LoadReferenceBitmapL(const TFileName& aSourceFileName);
	TInt FileOpenModeTest(TFileName& aSourceFileName, TBool aPreserveImageData, TUint aOptions);
	TInt FileTransformTest1(TFileName& aSourceFileName, TBool aPreserveImageData, TUint aOptions);
	TInt FileTransformTest2(TFileName& aSourceFileName, TBool aPreserveImageData, TUint aOptions);
private:
	};

TExifEncodeStep ExifOpToExifEncodeStepL(const TInt aExifOperation);

#endif // __TGPSIMAGE_H__
