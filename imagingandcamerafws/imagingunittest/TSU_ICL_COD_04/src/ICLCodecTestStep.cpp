
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
// ICL Codec Tests - see ../UnitTest/ICL/Documentation/ICL Codecs Unit Test Specification.xls
// 
//

#include "ICLCodecTestStep.h"
#include "jpegperformanceparams.inl"


const TInt KHeapSize = 1024*1024;

struct TTestType
	{
	const TText* iTestName;
	TImageCall   iCallL;
	TExifDataType iDataType;
	TExifOperation iOperation;
	TBool iIsAllocTest;
	const TPerformanceTestParams* iPerfTstParams;
	};

// table of tests

const TTestType KTestDetails[] =
	{
	// exif decode test
	{ _S("MM-ICL-COD-U-0501-HP"), &TImage::GetExifData, E8BitBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0502-HP"), &TImage::GetExifData, E16BitBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0503-HP"), &TImage::GetExifData, EIntegerBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0504-HP"), &TImage::GetExifData, ERationalBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0505-HP"), &TImage::GetExifData, ETagNotExistsError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0506-HP"), &TImage::GetExifData, EInvalidFormatError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0507-HP"), &TImage::GetExifData, ERationalArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0508-HP"), &TImage::GetExifData, EIntegerArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0509-HP"), &TImage::GetExifData, EShortBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0510-HP"), &TImage::GetExifData, EShortArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0511-HP"), &TImage::GetExifData, E8BitBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0512-HP"), &TImage::GetExifData, E16BitBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0513-HP"), &TImage::GetExifData, EIntegerBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0514-HP"), &TImage::GetExifData, ERationalBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0515-HP"), &TImage::GetExifData, ERationalArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0516-HP"), &TImage::GetExifData, EIntegerArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0517-HP"), &TImage::GetExifData, EShortBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0518-HP"), &TImage::GetExifData, EShortArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0519-HP"), &TImage::GetExifShortData, EShortestShortArrayBuffer, EGetParam, EFalse},


	// exif encode test
	{ _S("MM-ICL-COD-U-0521-HP"), &TImage::SetExifData, E8BitBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0522-HP"), &TImage::SetExifData, E16BitBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0523-HP"), &TImage::SetExifData, EIntegerBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0524-HP"), &TImage::SetExifData, ERationalBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0525-HP"), &TImage::SetExifData, ERationalArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0526-HP"), &TImage::SetExifData, EIntegerArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0527-HP"), &TImage::SetExifData, EShortBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0528-HP"), &TImage::SetExifData, EShortArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0529-HP"), &TImage::SetExifData, EDefaultTagSet, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0530-HP"), &TImage::SetExifData, E8BitBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0531-HP"), &TImage::SetExifData, E16BitBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0532-HP"), &TImage::SetExifData, EIntegerBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0533-HP"), &TImage::SetExifData, ERationalBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0534-HP"), &TImage::SetExifData, ERationalArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0535-HP"), &TImage::SetExifData, EIntegerArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0536-HP"), &TImage::SetExifData, EShortBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0537-HP"), &TImage::SetExifData, EShortArrayBuffer, EGetParam, EFalse},

	// exif thumbnail test
	{ _S("MM-ICL-COD-U-0541-HP"), &TImage::TestThumbnail, EGetThumbnail, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0542-HP"), &TImage::TestThumbnail, ESetThumbnail, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0543-HP"), &TImage::TestThumbnail, EGetThumbnailError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0544-HP"), &TImage::TestThumbnail, ESetNoThumbnail, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0545-HP"), &TImage::TestThumbnail, EGetThumbnailHeader, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0546-HP"), &TImage::TestThumbnail, EGetCorruptThumbnailHeader, EGetParam, EFalse},

	{ _S("MM-ICL-COD-U-0562-HP"), &TImage::TransformThumbnailFromJpegImage, EGetThumbnailError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0563-HP"), &TImage::TransformThumbnailFromJpegImage, ESetThumbnail, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0564-HP"), &TImage::TransformThumbnailFromJpegImage, ESetNoThumbnail, ESetParam, EFalse},

	{ _S("MM-ICL-COD-U-0565-HP"), &TImage::TransformExif, E8BitBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0566-HP"), &TImage::TransformExif, E16BitBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0567-HP"), &TImage::TransformExif, EIntegerBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0568-HP"), &TImage::TransformExif, ERationalBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0569-HP"), &TImage::TransformExif, ERationalArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0570-HP"), &TImage::TransformExif, EIntegerArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0571-HP"), &TImage::TransformExif, EShortBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0572-HP"), &TImage::TransformExif, EShortArrayBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0573-HP"), &TImage::TransformExif, EDefaultTagSet, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0574-HP"), &TImage::TransformExif, E8BitBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0575-HP"), &TImage::TransformExif, E16BitBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0576-HP"), &TImage::TransformExif, EIntegerBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0577-HP"), &TImage::TransformExif, ERationalBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0578-HP"), &TImage::TransformExif, ETagNotExistsError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0579-HP"), &TImage::TransformExif, EInvalidFormatError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0580-HP"), &TImage::TransformExif, ERationalArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0581-HP"), &TImage::TransformExif, EIntegerArrayBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0582-HP"), &TImage::TransformExif, EShortBuffer, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0583-HP"), &TImage::TransformExif, EShortArrayBuffer, EGetParam, EFalse},

	{ _S("MM-ICL-COD-U-0584-HP"), &TImage::TransformSetSize, ESetNoThumbnail, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0585-HP"), &TImage::TransformSetSize, ESetThumbnail, EGetParam, EFalse},

	{ _S("MM-ICL-COD-U-0586-HP"), &TImage::TransformThumbnailFromExifImage, EGetThumbnailError, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0587-HP"), &TImage::TransformThumbnailFromExifImage, ESetThumbnail, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0588-HP"), &TImage::TransformThumbnailFromExifImage, ESetNoThumbnail, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0589-HP"), &TImage::TransformThumbnailFromThumblessExifImage, ESetThumbnail, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0590-HP"), &TImage::TransformThumbnailFromThumblessExifImage, ESetNoThumbnail, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-0591-HP"), &TImage::TransformExif, EGenericTest, ENoParam, EFalse},
	
	{ _S("MM-ICL-COD-U-0601-HP"), &TImage::TestExifUtility, EDefaultTagSet, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-0602-HP"), &TImage::TestExifJpegTransformHandleLeak, EGenericTest, ENoParam, EFalse},

	{ _S("MM-ICL-COD-U-0621-HP"), &TImage::AllocTest, EGetThumbnailError, EGetParam, ETrue},
	{ _S("MM-ICL-COD-U-0622-HP"), &TImage::AllocTest, ESetThumbnail, ESetParam, ETrue},
	{ _S("MM-ICL-COD-U-0623-HP"), &TImage::AllocTest, ESetNoThumbnail, ESetParam, ETrue},
	{ _S("MM-ICL-COD-U-0624-HP"), &TImage::AllocTest, E8BitBuffer, EGetParam, ETrue},
	{ _S("MM-ICL-COD-U-0625-HP"), &TImage::AllocTest, EDefaultTagSet, EGetParam, ETrue},
	{ _S("MM-ICL-COD-U-0626-HP"), &TImage::AllocSetImageTypeTestL, ESetImageType, ESetParam, ETrue},
	{ _S("MM-ICL-COD-U-1001-HP"), &TImage::TestDerivedExifPluginL, EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1002-HP"), &TImage::TestNonExifAPP1MarkerL, EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1003-HP"), &TImage::TestMultipleEncodeDecodesWithRandomCancelsL, EGenericTest, EGetParam, EFalse},

	//GPS IFD Tests
	// Getters using Decoder Metadata
	{ _S("MM-ICL-COD-U-1101-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSExifData), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1102-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSExifData2), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1103-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSExifData3), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1104-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSExifData4), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1105-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSExifData5), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1106-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSExifData6), EGenericTest, EGetParam, EFalse},

	// Getters using Transformer Metadata
	{ _S("MM-ICL-COD-U-1111-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSTransformExif), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1112-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSTransformExif2), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1113-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSTransformExif3), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1114-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSTransformExif4), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1115-HP"), static_cast<TImageCall>(&TGPSImage::GetGPSTransformExif5), EGenericTest, ESetParam, EFalse},

	// Setters using Encoder Metadata
	{ _S("MM-ICL-COD-U-1121-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSExifData), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1122-HP"), static_cast<TImageCall>(&TGPSImage::AddGPSExifData), EGenericTest, ESetParam, EFalse},

	// Setters using Transformer Metadata
	{ _S("MM-ICL-COD-U-1131-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExif), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1132-HP"), static_cast<TImageCall>(&TGPSImage::AddGPSTransformExif), EGenericTest, ESetParam, EFalse},

	{ _S("MM-ICL-COD-U-1133-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformNoGpsTag), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1134-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformNoGpsBadCountTag), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1135-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformNoGpsBadCountTagOptionNone), EGenericTest, EGetParam, EFalse},

	// Setting Undefined Tags using Encoder Metadata
	{ _S("MM-ICL-COD-U-1141-HP"), static_cast<TImageCall>(&TGPSImage::SetEncoderUndefinedGpsTags), EAsciiBuffer, EAddParam, EFalse},
	{ _S("MM-ICL-COD-U-1142-HP"), static_cast<TImageCall>(&TGPSImage::SetEncoderUndefinedGpsTags), EAsciiBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1143-HP"), static_cast<TImageCall>(&TGPSImage::SetEncoderUndefinedGpsTags), EUnicodeBuffer, EAddParam, EFalse},
	{ _S("MM-ICL-COD-U-1144-HP"), static_cast<TImageCall>(&TGPSImage::SetEncoderUndefinedGpsTags), EUnicodeBuffer, ESetParam, EFalse},

	// Setting Undefined Tags using Tansformer Meta Data
	{ _S("MM-ICL-COD-U-1151-HP"), static_cast<TImageCall>(&TGPSImage::SetTransformUndefinedGpsTags), EAsciiBuffer, EAddParam, EFalse},
	{ _S("MM-ICL-COD-U-1152-HP"), static_cast<TImageCall>(&TGPSImage::SetTransformUndefinedGpsTags), EAsciiBuffer, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1153-HP"), static_cast<TImageCall>(&TGPSImage::SetTransformUndefinedGpsTags), EUnicodeBuffer, EAddParam, EFalse},
	{ _S("MM-ICL-COD-U-1154-HP"), static_cast<TImageCall>(&TGPSImage::SetTransformUndefinedGpsTags), EUnicodeBuffer, ESetParam, EFalse},
	
	/* Descriptor source/dest tests */
	{ _S("MM-ICL-COD-U-1161-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcTN), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1162-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcFN), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1163-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcTT), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1164-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcFT), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1165-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcDstTN), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1166-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcDstFN), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1167-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcDstTT), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1168-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorSrcDstFT), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1169-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorDstTN), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1170-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorDstFN), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1171-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorDstTT), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1172-HP"), static_cast<TImageCall>(&TGPSImage::SetGPSTransformExifDescriptorDstFT), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1173-HP"), static_cast<TImageCall>(&TGPSImage::CancelTransformExifDescriptorSrc), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1174-HP"), static_cast<TImageCall>(&TGPSImage::CancelTransformExifDescriptorSrcDst), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1175-HP"), static_cast<TImageCall>(&TGPSImage::CancelTransformExifDescriptorDst), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1176-HP"), static_cast<TImageCall>(&TGPSImage::TransformJpegDescSource), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1177-HP"), static_cast<TImageCall>(&TGPSImage::TransformThumblessExifDescSource), EGenericTest, EGetParam, EFalse},

	// GPS Miscellaneous
	{ _S("MM-ICL-COD-U-1181-HP"), static_cast<TImageCall>(&TGPSImage::GPSExifDataRegression), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1182-HP"), static_cast<TImageCall>(&TGPSImage::GPSTransformDeleteAndSetup), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1183-HP"), static_cast<TImageCall>(&TGPSImage::ShortPixelXYDimension), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1184-HP"), static_cast<TImageCall>(&TGPSImage::CopyrightNullTest), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1185-HP"), static_cast<TImageCall>(&TGPSImage::CopyrightNullTestTransform), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1186-HP"), static_cast<TImageCall>(&TGPSImage::GPSLatitudeRefCount), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1187-HP"), static_cast<TImageCall>(&TGPSImage::ThumbnailEOI), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1188-HP"), static_cast<TImageCall>(&TGPSImage::CancelTransform), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1189-HP"), static_cast<TImageCall>(&TGPSImage::UnsupportedTags), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1190-HP"), static_cast<TImageCall>(&TGPSImage::TestUnicodeDescriptorLengthGpsTag), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1191-HP"), static_cast<TImageCall>(&TGPSImage::TestUnicodeDescriptorLengthUserComment), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1192-HP"), static_cast<TImageCall>(&TGPSImage::SetShortPixelXYDimensionTransform), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1193-HP"), static_cast<TImageCall>(&TGPSImage::ExifUtilityPixelXYDimension), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1194-HP"), static_cast<TImageCall>(&TGPSImage::TestTransformExifBigEndian), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1195-HP"), static_cast<TImageCall>(&TGPSImage::TestConflictingTags), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1196-HP"), static_cast<TImageCall>(&TGPSImage::TestConflictingTagsTransform), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1197-HP"), static_cast<TImageCall>(&TGPSImage::TestShortArrayTag), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1198-HP"), static_cast<TImageCall>(&TGPSImage::TestShortArrayTagTransform), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1199-HP"), static_cast<TImageCall>(&TGPSImage::ValidateThumbnailAndGpsIfd), EGenericTest, EGetParam, EFalse},
	{ _S("MM-ICL-COD-U-1200-HP"), static_cast<TImageCall>(&TGPSImage::FileOpenMode), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1201-HP"), static_cast<TImageCall>(&TGPSImage::FileTransformMode1), EGenericTest, ESetParam, EFalse},
	{ _S("MM-ICL-COD-U-1202-HP"), static_cast<TImageCall>(&TGPSImage::FileTransformMode2), EGenericTest, ESetParam, EFalse},
	
	{ _S("MM-ICL-COD-U-2000-HP"), &TImage::TestJpgChunkImageFrameEncodePerfL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2001-HP"), &TImage::TestJpgChunkImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2002-HP"), &TImage::TestJpgDescImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2003-HP"), &TImage::NegTestJpgImageFrameEncoderPluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2004-HP"), &TImage::NegTestJpgImageFrameDecoderPluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2005-HP"), &TImage::NegTestJpgImageFramePluginReductionL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2006-HP"), &TImage::TestJpgImageFramePluginNewThreadL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2007-HP"), &TImage::TestImageFrameDerivedL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2008-HP"), &TImage::NegTestFullImageFrameL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2009-HP"), &TImage::StreamDecodeFileToImageFrameL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2010-HP"), &TImage::NegTestStreamDecodeFileToImageFrameL, EGenericTest, ENoParam, EFalse},

	{ _S("MM-ICL-COD-U-2050-HP"), &TImage::TestJpgImageFrameBlockStreamerSetupL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2051-HP"), &TImage::TestJpgImageFrameBlockStreamerInteroperabilityL, EGenericTest, ENoParam, EFalse},	
	{ _S("MM-ICL-COD-U-2052-HP"), &TImage::TestJpgDescStreamImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2053-HP"), &TImage::TestJpgDescStreamUnderFlowImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2054-HP"), &TImage::TestJpgDescStreamOverFlowImageFramePluginL, EGenericTest, ENoParam, EFalse},
	
	{ _S("MM-ICL-COD-U-2055-HP"), &TImage::TestJpgDescSequentialStreamDecodeEncodeImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2056-HP"), &TImage::TestJpgDescRandomStreamDecodeEncodeImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2057-HP"), &TImage::TestJpgDescRandomBackStreamDecEncImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2058-HP"), &TImage::TestJpgDescStreamNegativeTestsL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2059-HP"), &TImage::TestJpgDescSeqStreamDecEncBlockwiseImageFramePluginL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2060-HP"), &TImage::TestJpgDescRandomFwdBackStreamDecEncImageFramePluginL, EGenericTest, ENoParam, EFalse},
	
	{ _S("MM-ICL-COD-U-2061-HP"), &TImage::TestStreamEncodeAllocL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-2062-HP"), &TImage::TestStreamEncodeDecodeAllocL, EGenericTest, ENoParam, EFalse},
	
	{ _S("MM-ICL-COD-U-3001-HP"), &TImage::JpegDecoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpColor16m_1to1Size},
	{ _S("MM-ICL-COD-U-3002-HP"), &TImage::JpegDecoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpColor16m_1to2Size},
	{ _S("MM-ICL-COD-U-3003-HP"), &TImage::JpegDecoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpColor16m_1to4Size},
	{ _S("MM-ICL-COD-U-3004-HP"), &TImage::JpegDecoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpColor16m_1to8Size},
	{ _S("MM-ICL-COD-U-3005-HP"), &TImage::JpegDecoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpColor64K_1to1Size},
	{ _S("MM-ICL-COD-U-3006-HP"), &TImage::JpegDecoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpColor64K_1to4Size},
	
	{ _S("MM-ICL-COD-U-3007-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM},
	{ _S("MM-ICL-COD-U-3008-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_EXT},
	{ _S("MM-ICL-COD-U-3009-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_C64K},
	
	{ _S("MM-ICL-COD-U-3010-HP"), &TImage::TestRelaxedJpgDecodeL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3011-HP"), &TImage::TestJpegImageFrameTranscodeL, EGenericTest, ENoParam, EFalse},
	
	// INC083025
	{ _S("MM-ICL-COD-U-3020-HP"), &TImage::VerifyIFD1FromThumblessExifSource, ESetThumbnail, ENoParam, EFalse}, 
	{ _S("MM-ICL-COD-U-3021-HP"), &TImage::VerifyIFD1FromThumblessExifSource, ESetNoThumbnail, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3022-HP"), &TImage::VerifyIFD1FromThumbedExifSource, ESetThumbnail, ENoParam, EFalse}, 
	{ _S("MM-ICL-COD-U-3023-HP"), &TImage::VerifyIFD1FromThumbedExifSource, ESetNoThumbnail, ENoParam, EFalse},

	// DEF084849
	{ _S("MM-ICL-COD-U-3024-HP"), &TImage::VerifyTransformedThumbnailSOI, EGenericTest, ENoParam, EFalse},

	// DEF082165
	{ _S("MM-ICL-COD-U-3030-HP"), &TImage::TestAsciiNullConsistency, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3031-HP"), &TImage::TestAsciiNonNullConsistency, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3032-HP"), &TImage::NegTestAsciiNonNullConsistency, EGenericTest, ENoParam, EFalse},

	//DEF090653
	{ _S("MM-ICL-COD-U-3033-HP"), &TImage::TestThumbnailNoComments, EGenericTest, ENoParam, EFalse},

	//PDEF085003: Exif Output file corrupt if JPEGInterchangeFormatLength tag not present
	{ _S("MM-ICL-COD-U-3040-HP"), &TImage::TransformTestIFD1Anomalies, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3041-HP"), &TImage::DecodeTestIFD1Anomalies, EGenericTest, ENoParam, EFalse},

	// DEF091418: T: Transforming a Nikon camera jpeg image returns KErrNotSupported.
	{ _S("MM-ICL-COD-U-3042-HP"), &TImage::TransformTests, EGenericTest, ENoParam, EFalse},

	// DEF093013: T: If Transform() fails with -4, a garbage destination image is left in C drive
	{ _S("MM-ICL-COD-U-3043-HP"), &TImage::TransformTests2, EGenericTest, ENoParam, EFalse},

	// DEF092988: T: Aspect ratio of scaled image is maintained evenif iMaintainAspectRatio=EFalse
	{ _S("MM-ICL-COD-U-3044-HP"), &TImage::TransformTests3, EGenericTest, ENoParam, EFalse},
		
	// DEF091185: T: Exif data is added after the transform to an image which had no exif
	{ _S("MM-ICL-COD-U-3045-HP"), &TImage::TransformNonExifImageTest, EGenericTest, ENoParam, EFalse},
	
	// CR910: ICL JPEG Support for non conformant exif data
	{ _S("MM-ICL-COD-U-3050-HP"), &TImage::IgnoreBrokenIfdEntryTest, EGenericTest, ENoParam, EFalse},
	
	// CRPCUG-6SSBQ7: ICL CImageDecoder / CImageTransform option to skip EXIF metadata decoding.
	{ _S("MM-ICL-COD-U-3051-HP"), &TImage::DecodeTestWithIgnoreExifMetadataFlag, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3052-HP"), &TImage::TransformWithIgnoreExifMetadataFlag, EGenericTest, ENoParam, EFalse},

	// INC101733: Behaviour of Exif transform with successive invalid tags is not as expected
	{ _S("MM-ICL-COD-U-3053-HP"), &TImage::IfdWithUnknownTagsTest, EGenericTest, ENoParam, EFalse},
	
	// DEF101880: JPEG Encoder may produce distorted images
	{ _S("MM-ICL-COD-U-3054-HP"), &TImage::TestJpgDistortedAfterEncodeL, EGenericTest, ENoParam, EFalse},
	
	// PDEF110735: JPEG Exif IFDs with 0 entries can cause invalid JPEG
	{ _S("MM-ICL-COD-U-3055-HP"), &TImage::IFDNoEntries, EGenericTest, EGetParam, EFalse},

	// DEF110487: Decoding jpeg file with height of 17 pixels to image frame buffer errors with -9
	{ _S("MM-ICL-COD-U-3056-HP"), &TImage::TestJpegWithVariousFileSizesL, EGenericTest, ENoParam, EFalse},
	
	//PDEF128742:Image transform can not handle illegal exif when original image is to preserved 
	{ _S("MM-ICL-COD-U-3057-HP"), &TImage::TransformWithPreserveL, EGenericTest, ENoParam, EFalse},

	// CR1051 : Performance improvement of jpegcodec
	{ _S("MM-ICL-COD-U-4000-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_FastEncode_EColor16M_422},
	{ _S("MM-ICL-COD-U-4001-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_FastEncode_EColor16M_Mono},
	{ _S("MM-ICL-COD-U-4002-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_FastEncode_EColor16M_420},	
	{ _S("MM-ICL-COD-U-4003-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_FastEncode_EColor64K_420},	
	{ _S("MM-ICL-COD-U-4004-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_FastEncode_EColor256_420},		
	{ _S("MM-ICL-COD-U-4005-HP"), &TImage::JpegEncoderPerformanceTestL,  EGenericTest, ENoParam, EFalse, &K2MpEncode_Q75_RAM_FastEncode_EGray256_420},
	
	//DEF122727: Jpeg block stream: Missing EOI (End of Image) 
	{ _S("MM-ICL-COD-U-3106-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KSmall32x32Mono},
	{ _S("MM-ICL-COD-U-3107-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KSmallMono},
	{ _S("MM-ICL-COD-U-3108-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KAverageMono},
	{ _S("MM-ICL-COD-U-3109-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KLargeMono},
	{ _S("MM-ICL-COD-U-3110-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KSmall420},
	{ _S("MM-ICL-COD-U-3111-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KAverage420},
	{ _S("MM-ICL-COD-U-3112-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KLarge420},
	{ _S("MM-ICL-COD-U-3113-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KSmall422},
	{ _S("MM-ICL-COD-U-3114-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KAverage422},
	{ _S("MM-ICL-COD-U-3115-HP"), &TImage::TestJpgSequentialStreamDecodeEncodePerformanceL,  EGenericTest, ENoParam, EFalse, &KLarge422},
	//CR1329
	{ _S("MM-ICL-COD-U-3060-HP"), &TImage::CheckDeviceInfoTagsOnEncodeL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3061-HP"), &TImage::CheckDeviceInfoTagsOnTransformL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3062-HP"), &TImage::CheckDeviceInfoTagsOnModifyEncodeL, EGenericTest, ENoParam, EFalse},
	{ _S("MM-ICL-COD-U-3063-HP"), &TImage::CheckDeviceInfoTagsOnModifyTransformL, EGenericTest, ENoParam, EFalse}
	};

		
RICLCodecStep::RICLCodecStep(TInt aIndex):
	iIndex(aIndex)
	{
	const TTestType& testDetails = KTestDetails[iIndex];
	iTestStepName = testDetails.iTestName;
	iCallL = testDetails.iCallL;
	iIsAllocTest = testDetails.iIsAllocTest;
	iDataType = testDetails.iDataType;
	iOperation = testDetails.iOperation;
	iHeapSize = KHeapSize;
	}

RICLCodecStep* RICLCodecStep::NewL(TInt aIndex)
	{
	const TInt KNumTestSteps = sizeof(KTestDetails) / sizeof(TTestType);
	if (aIndex>=KNumTestSteps)
		{
		User::Leave(KErrOverflow);
		}
	RICLCodecStep* result = new (ELeave) RICLCodecStep(aIndex);
	return result;
	}

TVerdict RICLCodecStep::OpenL()
	{
	if (iIsAllocTest)
		{
		// see if alloc fail stuff is enabled - ask if next alloc test will fail
		__UHEAP_SETFAIL(RHeap::EFailNext, 1);
		TAny *testAlloc = User::Alloc(1); 
		TBool workingOk = testAlloc == NULL;
		User::Free(testAlloc);
		__UHEAP_SETFAIL(RHeap::ENone ,0);

		if (!workingOk)
			{
			ERR_PRINTF1(_L("Alloc Testing does not work on this build"));
			return EInconclusive;
			}
		}

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

	iTImage = new (ELeave) TImage(KTestDetails[iIndex].iPerfTstParams, iTestStepName);
	iTImage->SetLog(iSuite->LogSystem());
	iTImage->SetLinkToVerdict(iTestResult);
	iTImage->ConstructL();
	return EPass;
	}

void RICLCodecStep::Close()
	{
	if (iTImage)
		{
		iTImage->End();
		delete iTImage; 
		iTImage = NULL;
		}
	
	delete iScheduler; 
	iScheduler = NULL;

	RFbsSession::Disconnect();
	}

TVerdict RICLCodecStep::DoTestStepL()
	{
	ASSERT(iTImage); // should have been created by now
	iTestResult = EPass;
	
	// last 7 chars of the test case number is the name of the section in the configuration file
    iSectionName = iTestStepName.Right(KSectionNameLen);
	//if true, read the decoderuid if false pick the default uid		
	if ((!GetHexFromConfig(iSectionName, _L("decoderuid"), iDecUid)))
		{
		if (iDecUid != 0)
			{
			iTImage->iDecoderUid.iUid = iDecUid;
			INFO_PRINTF2(_L("Decoder uid: %d"), iTImage->iDecoderUid.iUid);		
			}
		}


	(iTImage->*iCallL)(iDataType, iOperation);
	return iTestResult;
	}


//
// add missing bit of TImage
//
void TImage::TestL()
	{
	// this function should not be called in the testframework
	User::Panic(_L("TImage::TestL()"), 0);
	}

