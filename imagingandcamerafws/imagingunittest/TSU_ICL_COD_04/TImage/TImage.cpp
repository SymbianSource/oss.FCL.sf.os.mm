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

#include "TImage.h"
#include <apmstd.h>
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include <imagetransform.h>
#include <iclexif.h>
#include <iclexifimageframe.h>
#include <imageframe.h>
#include <imagetransform.h>
#include <exifutility.h>
#include <hal.h>
#include <hal_data.h>
#if defined(__ICL_PROFILING)
#include <ecom/ecom.h> // used when profiling
#endif //defined(__ICL_PROFILING)
// Test system includes
#include <testframework.h>
#include <icl/imagecodec.h>
#include <e32math.h>
#include "TestImageFrameDerived.h"
#include "TestFullImageFrame.h"
#include "exifverifier.h"
#include <bafl/sysutil.h>

// file names
_LIT(KImageTestExifDecode, 	"c:\\TSU_ICL_cod_04\\DecodeTestSource.jpg");
_LIT(KImageTestExifEncode, 	"c:\\TSU_ICL_cod_04\\ExifEncode.jpg");
_LIT(KEncodeSourceFileName, 	"c:\\TSU_ICL_cod_04\\EncodeTestSource.mbm");
_LIT(KEncodeThumbnailFileName, "c:\\TSU_ICL_cod_04\\EncodeThumbnail.mbm");
_LIT(KDecodeThumbnailFileName, "c:\\TSU_ICL_cod_04\\DecodeThumbnail.mbm");
_LIT(KImageTestExifTransformSource, 	"c:\\TSU_ICL_cod_04\\ExifTransformSource.jpg");
_LIT(KImageTestThumblessExifTransformSource,	"c:\\TSU_ICL_cod_04\\ExifNoThumb.jpg");
_LIT(KImageTestTransformSource, 	"c:\\TSU_ICL_cod_04\\TransformSource.jpg");
_LIT(KImageTestTransformSource2, 	 "c:\\TSU_ICL_cod_04\\1103018943-tazpvdft.jpg");
_LIT(KImageTestTransformSource2Ref,  "c:\\TSU_ICL_cod_04\\1103018943-tazpvdft_ref.jpg");
_LIT(KImageTestExifTransformDest, 	"c:\\TSU_ICL_cod_04\\ExifTransform.jpg");
_LIT(KImageTestExifTransformDest22, 	"c:\\TSU_ICL_cod_04\\ExifTransform22.jpg");
_LIT(KImageTestExifTransformDest33, 	"c:\\TSU_ICL_cod_04\\ExifTransform33.jpg");

_LIT(KImageTestExifTransformBig, 	"c:\\TSU_ICL_cod_04b\\07casio-ex-z3.jpg");
_LIT(KImageTestExifTransformBigDest, 	"c:\\TSU_ICL_cod_04b\\07casio-ex-z3_1500x1200.jpg");
_LIT(KImageTestExifTransformBigRef, 	"c:\\TSU_ICL_cod_04b\\07casio-ex-z3_1500x1200_ref.jpg");
_LIT(KImageTestExifTransformSquareDest, 	"c:\\TSU_ICL_cod_04b\\07casio-ex-z3_300x300.jpg");
_LIT(KImageTestExifTransformSquareRef, 	"c:\\TSU_ICL_cod_04b\\07casio-ex-z3_300x300_ref.jpg");
_LIT(KImageTestNonExifApp1, "c:\\TSU_ICL_cod_04\\Non_Exif_App1.jpg");
_LIT(KImageTestThumbnailDecode, "c:\\TSU_ICL_cod_04\\TestThumbnailDecode.jpg");
_LIT(KImageTestExifThumbnailDecode,	"c:\\TSU_ICL_cod_04\\TestExifThumbnailDecode.jpg");
_LIT(KImageTestExifNullAsciiEncoderRef,	"c:\\TSU_ICL_cod_04\\ExifAsciiNullTestsEncoderRef.jpg");
_LIT(KImageTestExifNullAsciiTransformRef,	"c:\\TSU_ICL_cod_04\\ExifAsciiNullTestsTransformRef.jpg");
_LIT(KImageTestThumbnailNoCommentsSource, 	"c:\\TSU_ICL_cod_04\\TestThumbnailNoCommentsSource.jpg");
_LIT(KImageTestThumbnailNoCommentsDest, 	"c:\\TSU_ICL_cod_04\\TestThumbnailNoCommentsDest.jpg");
_LIT(KImageGetThumbnail, 	"c:\\TSU_ICL_COD_04\\thumbnail.jpg");
_LIT(KExifTag0000, 	"c:\\TSU_ICL_cod_04\\exiftag-0000.jpg");
_LIT(KExifTag0000NoGPSTransformReference,  "c:\\tsu_icl_cod_04\\nogpstransformref.jpg");
_LIT(KExifTag0000NoInteropTransformReference, "c:\\tsu_icl_cod_04\\nointeroptransformref.jpg");
_LIT(KExifTag0000NoGPSTransform, 	"c:\\tsu_icl_cod_04\\exiftag0000nogpstransform.jpg");
_LIT(KExifInteropTag0000, "c:\\tsu_icl_cod_04\\interoptag-0000.jpg");


const TDisplayMode KThumbnailDispMode(EColor16M);
#define KTransformImageSize TSize(300, 200)
const TInt KMaxDelay = 5000; // max cancel/restart delay in ms

const TUid KExifDerivedDecoderUid = {0x102070C2};
const TUid KExifDerivedEncoderUid = {0x102070C3};
const TUid KImageFramePluginUid = {0x101F7C60};
const TUid KJpegImageTypeUid = {0x101F45D8};

const TInt KChunkDataOffset = 10;
const TInt KDummyImageSize = 1000;
const TInt KStream420ImgIdx = 2;

const TInt KMCUBlockSize = 16;

_LIT(KImageTestJfifMono, "c:\\TSU_ICL_cod_04\\jfifMono_2.jpg");
_LIT(KImageTestJfif420, "c:\\TSU_ICL_cod_04\\jfif420_2.jpg");
_LIT(KImageTestJfif422, "c:\\TSU_ICL_cod_04\\jfif422_4.jpg");
_LIT(KImageTestJfif444, "c:\\TSU_ICL_cod_04\\jfif444_8.jpg");

_LIT(KImageTestJfifMono_2MP, "c:\\TSU_ICL_cod_04\\2mp-samp-mono.jpg");
_LIT(KImageTestJfif420_2MP, "c:\\TSU_ICL_cod_04\\2mp-samp-420.jpg");
_LIT(KImageTestJfif422_2MP, "c:\\TSU_ICL_cod_04\\2mp-samp-422.jpg");

_LIT(KImageFrameTestRefMono_2MP, "c:\\TSU_ICL_cod_04\\2mp-samp-mono-ref.jpg");
_LIT(KImageFrameTestRef420_2MP, "c:\\TSU_ICL_cod_04\\2mp-samp-420-ref.jpg");
_LIT(KImageFrameTestRef422_2MP, "c:\\TSU_ICL_cod_04\\2mp-samp-422-ref.jpg");

_LIT(KImageFrameTestEncodeMonoc, "c:\\TSU_ICL_cod_04\\ImageFrameEncodeMonoc.jpg");
_LIT(KImageFrameTestEncode420c, "c:\\TSU_ICL_cod_04\\ImageFrameEncode420c.jpg");
_LIT(KImageFrameTestEncode422c, "c:\\TSU_ICL_cod_04\\ImageFrameEncode422c.jpg");
_LIT(KImageFrameTestEncode444c, "c:\\TSU_ICL_cod_04\\ImageFrameEncode444c.jpg");
_LIT(KImageFrameTestEncodeMonod, "c:\\TSU_ICL_cod_04\\ImageFrameEncodeMonod.jpg");

_LIT(KImageFrameTestEncode420d, "c:\\TSU_ICL_cod_04\\ImageFrameEncode420d.jpg");
_LIT(KImageFrameTestEncode422d, "c:\\TSU_ICL_cod_04\\ImageFrameEncode422d.jpg");
_LIT(KImageFrameTestEncode444d, "c:\\TSU_ICL_cod_04\\ImageFrameEncode444d.jpg");
_LIT(KImageFrameTestEncode420t, "c:\\TSU_ICL_cod_04\\ImageFrameEncode420t.jpg");
_LIT(KImageFrameTestEncode420ifd, "c:\\TSU_ICL_cod_04\\KImageFrameTestEncode420ifd.jpg");
_LIT(KImageFrameTestNegative, "c:\\TSU_ICL_cod_04\\ImageFrameNegativeTest.jpg");

_LIT(KImageFrameTestRefMono, "c:\\TSU_ICL_cod_04\\ImageFrameRefMono.jpg");
_LIT(KImageFrameTestRef420, "c:\\TSU_ICL_cod_04\\ImageFrameRef420.jpg");
_LIT(KImageFrameTestRef422, "c:\\TSU_ICL_cod_04\\ImageFrameRef422.jpg");
_LIT(KImageFrameTestRef444, "c:\\TSU_ICL_cod_04\\ImageFrameRef444.jpg");
_LIT(KImageFrameTestRefTranscode, "c:\\TSU_ICL_cod_04\\CamSunset2MP(transcode).jpg");

//PREQ 1630
_LIT(KImageStreamTestJfifMono, "c:\\TSU_ICL_cod_04\\streamMono.jpg");
_LIT(KImageStreamTestJfif420, "c:\\TSU_ICL_cod_04\\stream420.jpg");
_LIT(KImageStreamTestJfif422, "c:\\TSU_ICL_cod_04\\stream422.jpg");
_LIT(KImageStreamTestJfif444, "c:\\TSU_ICL_cod_04\\stream444.jpg");

_LIT(KImageStreamDecodeTestJfifMono, "c:\\TSU_ICL_cod_04\\streamMono-decode.jpg");
_LIT(KImageStreamDecodeTestJfif420, "c:\\TSU_ICL_cod_04\\stream420-decode.jpg");
_LIT(KImageStreamDecodeTestJfif422, "c:\\TSU_ICL_cod_04\\stream422-decode.jpg");
_LIT(KImageStreamDecodeTestJfif444, "c:\\TSU_ICL_cod_04\\stream444-decode.jpg");

_LIT(KImageFrameStreamTestRefMono, "c:\\TSU_ICL_cod_04\\ImageFrameStreamMono.jpg");
_LIT(KImageFrameStreamTestRef420, "c:\\TSU_ICL_cod_04\\ImageFrameStream420.jpg");
_LIT(KImageFrameStreamTestRef422, "c:\\TSU_ICL_cod_04\\ImageFrameStream422.jpg");
_LIT(KImageFrameStreamTestRef444, "c:\\TSU_ICL_cod_04\\ImageFrameStream444.jpg");

_LIT(KImageTempFileName, "c:\\TSU_ICL_cod_04\\imagetemp");
_LIT(KImageStreamToFile, "c:\\TSU_ICL_cod_04\\FrameToFile.jpg");

_LIT(KImageTestTranscode, "c:\\TSU_ICL_cod_04\\CamSunset2MP.jpg");
_LIT(KImageTestTranscodeProgressive, "c:\\TSU_ICL_cod_04\\2mp.jpg");
_LIT(KImageTestRefTranscodeProgressive, "c:\\TSU_ICL_cod_04\\2mp(transcode).jpg");
_LIT(KImageCorruptTestNo42TagInExif, "c:\\TSU_ICL_cod_04\\decode_corrupt42.jgp");
// DEF085157: P: CImageDecoder::SetImageTypeL() could leave in an ambiguous way
_LIT(KTestNoExifReference,"c:\\TSU_ICL_cod_04\\TestNoExifReference.mbm");
_LIT(KTestImageNoExif,"c:\\TSU_ICL_cod_04\\imageNoExif.jpg");
_LIT(KCorruptThumbnailHeaderReference,"c:\\TSU_ICL_cod_04\\TestCorruptThumbnailHeader.mbm");
_LIT(KTestImageCorruptThumbnailHeader,"c:\\TSU_ICL_cod_04\\imageCorruptThumbnailHeader.jpg");
_LIT(KTestImageCorruptMainHeader,"c:\\TSU_ICL_cod_04\\imageCorruptMainHeader.jpg");

//PDEF105650:Short order needs reversing in Exif retrieval function of JPEG codec
_LIT(KTestImageShortTagII,"c:\\TSU_ICL_cod_04\\exifshorttagii.jpg");
_LIT(KTestImageShortTagMM,"c:\\TSU_ICL_cod_04\\exifshorttagmm.jpg");

// CR910: ICL JPEG Support for non conformant exif data
_LIT(KTestImageExifIfd0MakeTypeWrong,		"c:\\TSU_ICL_cod_04\\exif-ifd0-make_wrong_type.jpg");
_LIT(KTestImageExifIfd0MakeOffsetWrong,		"c:\\TSU_ICL_cod_04\\exif-ifd0-make_offset_too_big.jpg");
_LIT(KTestImageExifIfd0OffsetToIfd1Wrong,	"c:\\TSU_ICL_cod_04\\exif-ifd0-offset_to_ifd1_too_big.jpg");
_LIT(KTestImageExifIfd0OffsetToIfd1TooBig,	"c:\\TSU_ICL_cod_04\\exif-ifd0-offset_to_ifd1_wrong.jpg");
_LIT(KTestImageExifExifIfdColSpaceWrongType,"c:\\TSU_ICL_cod_04\\exif-exif_ifd-color_space_wrong_type.jpg");
_LIT(KTestImageExifExifIfdOffsetToIopWrong,	"c:\\TSU_ICL_cod_04\\exif-exif_ifd-offset_to_interop_ifd_wrong.jpg");

// INC101733: Behaviour of Exif transform with successive invalid tags is not as expected
_LIT(KTestImageExifIfd0UnknownTags,			"c:\\TSU_ICL_cod_04\\invalid_input_exif_f1140539.jpg" );

// DEF101880: JPEG Encoder may produce distorted images
_LIT(KTestImageJpegDistorted,			"c:\\TSU_ICL_cod_04\\2mp-420.jpg" );
_LIT(KTestImageJpegDistortedEncoded,		"c:\\TSU_ICL_cod_04\\2mp-420-encoded.jpg" );
_LIT(KTestImageJpegDistortedReference,		"c:\\TSU_ICL_cod_04\\2mp-420-reference.jpg" );

//PDEF085003: Exif Output file corrupt if JPEGInterchangeFormatLength tag not present
//The following set of images all contain IFD1s that are in some way anomalous.
//They are used by used by TestIFD1Anomalies() (MM-ICL-COD-U-3040-HP) & DecodeTestIFD1Anomalies() (MM-ICL-COD-U-3041-HP).

//Structure identifying an image with an Anomalous IFD1
struct TAnomalousIFD1Source
	{
	const TText* iFilename;
	TBool iRecoverableIFD1;	//Indicates that it should be possible to overcome the anomalies.
	};

const TUint KNumAnomalousIFD1Sources = 9;
const TAnomalousIFD1Source KAnomalousIFD1Sources[] =
	{
	{_S("c:\\TSU_ICL_cod_04\\TransformSource.jpg"), EFalse},				//Not really anomalous, but included as a special case (not Exif, so no IFD1)
	{_S("c:\\TSU_ICL_cod_04\\ExifNoThumb.jpg"), EFalse},	 				//Not really anomalous, but included as a special case (no IFD1)
	{_S("c:\\TSU_ICL_cod_04\\exif_corrupt_bad_201.jpg"), ETrue}, 			//0x0201 points out-of-bounds
	{_S("c:\\TSU_ICL_cod_04\\exif_corrupt_no_201.jpg"), EFalse}, 			//No 0x0201 tag
	{_S("c:\\TSU_ICL_cod_04\\exif_corrupt_bad_202.jpg"), ETrue}, 			//0x0202 says jpeg is too big
	{_S("c:\\TSU_ICL_cod_04\\exif_corrupt_no_202.jpg"), EFalse}, 			//No 0x0202 tag
	{_S("c:\\TSU_ICL_cod_04\\pdef085003.jpg"), EFalse}, 					//The image attached to PDEF085003. 0x0103 says thumb is bitmap, but is really jpeg. IFD1 contains tags for both.
	{_S("c:\\TSU_ICL_cod_04\\bitmapthumb_motorola_nointerop.jpg"), EFalse},	//Valid (not corrupt) image but with a bitmap thumbnail, hence unsupported
	{_S("c:\\TSU_ICL_cod_04\\DecodeGPSTestSource2_Bad01IFDOffset.jpg"), EFalse}	//Invalid IFD1 offset
	};

//PDEF085003: Exif Output file corrupt if JPEGInterchangeFormatLength tag not present
//A set of sources with valid, jpeg IFD1s. Used to regression test the
//decoder after PDEF085003 changes.
const TUint KNumValidJpegIFD1Sources = 3;
const TText* const KValidJpegIFD1Sources[] =
	{
	_S("c:\\TSU_ICL_cod_04\\ExifTransformSource.jpg"),
	_S("c:\\TSU_ICL_cod_04\\ExifGPSTransformRefTT.jpg"),
	_S("c:\\TSU_ICL_cod_04\\TestExifThumbnailDecode.jpg")
	};

//DEF091418: T: Transforming a Nikon camera jpeg image returns KErrNotSupported.
//A set of sources on which to do all possible transform tests.
const TUint KNumTransformSources = 2;
const TText* const KTransformSources[] =
	{
	_S("c:\\TSU_ICL_cod_04\\def091418.jpg"), //Source with 3 QTables defined
	_S("c:\\TSU_ICL_cod_04\\three_qtables_exif_gps.jpg") //Another source with 3 QTables defined
	};

//DEF091185: T: Exif data is added after the transform to an image which had no exif
_LIT(KNonExifTransformSource,"c:\\TSU_ICL_cod_04\\datetree.jpg");
const TText* const KNonExifTransformedRefImages[] =
	{
	_S("c:\\TSU_ICL_cod_04\\datetree_ref1.jpg"),
	_S("c:\\TSU_ICL_cod_04\\datetree_ref2.jpg"),
	_S("c:\\TSU_ICL_cod_04\\datetree_ref3.jpg"),
	_S("c:\\TSU_ICL_cod_04\\datetree_ref4.jpg")
	};

//INC094065: RF CImageTransform output file has multiple DHT/DQT tables
_LIT(KNonExifTransformSource2,"c:\\TSU_ICL_cod_04\\input_jfif(multiple_dqt_dht_segs).jpg");
const TText* const KNonExifTransformedRefImages2[] =
	{
	_S("c:\\TSU_ICL_cod_04\\input_jfif(multiple_dqt_dht_segs)_ref1.jpg"),
	_S("c:\\TSU_ICL_cod_04\\input_jfif(multiple_dqt_dht_segs)_ref2.jpg"),
	_S("c:\\TSU_ICL_cod_04\\input_jfif(multiple_dqt_dht_segs)_ref3.jpg"),
	_S("c:\\TSU_ICL_cod_04\\input_jfif(multiple_dqt_dht_segs)_ref4.jpg")
	};

// Comments are arranged thus:
// Tag - Format - ComponentCount - (Required if hardware supports it),O(ptional),M(andatory)/Default value/Tag name/Tag description.
// tags

const TUint KTagList[][3]=
	{
	{0x010F, EAscii, 0}, //0x010F,2,any,M,none,Make,The manufacturer of the recording equipment.
	{0x0110, EAscii, 0}, //0x0110,2,any,M,none,Model,The model name or model number of the equipment.
	{0x011A, EUnsignedRational, 0}, //0x011A,5,1,M,72,Xresolution,n/a (pixel no. per ResolutionUnit in the ImageWidth direction).
	{0x011B, EUnsignedRational, 0}, //0x011B,5,1,M,72,Yresolution,n/a (pixel no. per ResolutionUnit in the ImageLength direction).
	{0x0128, EUnsignedShort, 0}, //0x0128,3,1,M,2,ResolutionUnit,"Units of resolution as used by Xresolution and YResolution (2 = inches, 3 = cm)."
	{0x0213, EUnsignedShort, 0}, //0x0213,3,1,M,1,YCbCrPositioning,"1 = centered, 2 = co-sited."
	{0x8769, EUnsignedLong, 0}, //0x8769,4,1,M,none,Pointer to Exif IFD.,n/a
	{0x9000, EUndefined, 0}, //0x9000,7,4,M,0220 - no null (in 2.2) or 0210 (in 2.1).,ExifVersion,The version of this standard supported. Non-existence = nonconformance.
	{0x9003, EAscii, 0}, //0x9003,2,20,M,none,DateTimeOriginal,"Date and time when the orginal image data was generated. (format is ""YYYY:MM:DD HH:MM:SS\0"" when unknown can be ""    :  :     :  :  \0"")."
	{0x9004, EAscii, 0}, //0x9004,2,20,M,none,DateTimeDigitized,"Date and time when the image was stored as digital data. (format is ""YYYY:MM:DD HH:MM:SS\0"" when unknown can be ""    :  :     :  :  \0"")."
	{0x9101, EUndefined, 0}, //0x9101,7,4,M,"4 5 6 0 (if RGB uncompressed - 4=R,5=G,6=B), 1 2 3 0 (other cases - 1=Y,2=Cb,3=Cr).",ComponentsConfiguration,"When compressed, the channels of each component are arranged in order from the 1st component to the 4th."
	{0xA000, EUndefined, 0}, //0xA000,7,4,M,0100 - no null,FlashpixVersion,Flashpix format version.
	{0xA002, EUnsignedLong, 0}, //0xA002,3 or 4,1,M,none,PixelXDimension,"When compressed, the valid width of the meaningful image."
	{0xA003, EUnsignedLong, 0}, //0xA003,3 or 4,1,M,none,PixelYDimension,"When compressed, the valid height of the meaningful image."
	{0xA005, EUnsignedLong, 0}, //0xA005,4,1,M,none,Interoperability IFD pointer,
	{0x0001, EAscii, 0}, //0x0001,2,any,M,"R98\0",InteroperabilityIndex,
	{0x0002, EUndefined, 0}, //0x0002,7,4,M,"0100",InteroperabilityVersion,
	{0x0103, EUnsignedShort, 1}, //0x0103,3,1,M,none,Compression,6 = thumb is JPEG compression. 1 = uncompressed. Not used in primary image
	{0x011A, EUnsignedRational, 1}, //0x011A,5,1,M,72,Xresolution,n/a (pixel no. per ResolutionUnit in the ImageWidth direction).
	{0x011B, EUnsignedRational, 1}, //0x011B,5,1,M,72,Yresolution,n/a (pixel no. per ResolutionUnit in the ImageLength direction).
	{0x0128, EUnsignedShort, 1}, //0x0128,3,1,M,2,ResolutionUnit,"Units of resolution as used by Xresolution and YResolution (2 = inches, 3 = cm)."
	{0x0201, EUnsignedLong, 1}, //0x0201,4,1,,none,JPEGInterchangeFormat,Offset to the SOI of the JPEG compressed thumbnail data. not used in primary JPEG data
	{0x0202, EUnsignedLong, 1}  //0x0202,4,1,,none,JPEGInterchangeFormatLength,Number of bytes of JPEG compressed thumbnail data. not used in primary JPEG data
	};
const TInt KNbMandatoryTags=23;

const TInt KUtilityTestNameMaxLength=32;
enum TExifUtilityMethods
	{
	EGetMake,
	EGetModel,
	EGetDateTime,
	EGetDateTimeOriginal,
	EGetDateTimeDigitized,
	EGetInteroperabilityIndex,
	EGetInteroperabilityVersion,
	EGetExifVersion,
	EGetComponentsConfiguration,
	EGetFlashPixVersion,
	EGetXResolution,
	EGetYResolution,
	EGetThumbXResolution,
	EGetThumbYResolution,
	EGetResolutionUnit,
	EGetYCbCrPositioning,
	EGetOrientation,
	EGetColorSpace,
	EGetPixelXDimension,
	EGetPixelYDimension,
	EGetThumbCompression,
	EGetThumbResolutionUnit,
	EGetThumbOrientation,
	EGetImageDescription,
	EGetCopyright,
	EGetMakerNote,
	EGetUserComment,
	EGetFlash,
	EGetFocalLength,
	EGetShutterSpeedValue,
	EGetJpegInterchangeFormatLength,
	EGetJpegInterchangeFormatLengthUint,
	EMaxUtilityIndex
	};

const TUint KUtilityTagIndexesList[][3]={
		{EGetMake, 0x010F, 0},
		{EGetModel, 0x0110, 0},
		{EGetDateTime, 0x0132},
		{EGetDateTimeOriginal, 0x9003, 0},
		{EGetDateTimeDigitized, 0x9004, 0},
		{EGetInteroperabilityIndex, 0x0001, 0},
		{EGetInteroperabilityVersion, 0x0002, 0},
		{EGetExifVersion, 0x9000, 0},
		{EGetComponentsConfiguration, 0x9101, 0},
		{EGetFlashPixVersion, 0xA000, 0},
		{EGetXResolution, 0x011A, 0},
		{EGetYResolution, 0x011B, 0},
		{EGetThumbXResolution,0x011A, 1},
		{EGetThumbYResolution,0x011B, 1},
		{EGetResolutionUnit, 0x0128, 0},
		{EGetYCbCrPositioning, 0x0213, 0},
		{EGetOrientation, 0x0112, 0},
		{EGetColorSpace, 0xA001, 0},
		{EGetPixelXDimension, 0xA002, 0},
		{EGetPixelYDimension, 0xA003, 0},
		{EGetThumbCompression, 0x0103, 1},
		{EGetThumbResolutionUnit, 0x0128, 1},
		{EGetThumbOrientation, 0x0112, 1},
		{EGetImageDescription, 0x010e, 0},
		{EGetCopyright, 0x8298, 0},
		{EGetMakerNote, 0x927C, 0},
		{EGetUserComment, 0x9286, 0},
		{EGetFlash, 0x9209, 0},
		{EGetFocalLength, 0x920A,  0},
		{EGetShutterSpeedValue, 0x9201, 0},
		{EGetJpegInterchangeFormatLength, 0x0202, 1},
		{EGetJpegInterchangeFormatLengthUint, 0x0202, 1}
		};


//
//TExifTransformSourceDest
//

//Construct an object representing a filename source or dest
TExifTransformSourceDest::TExifTransformSourceDest(const TDesC& aFilename)
	{
	iType = EFilename;
	iFilename = &aFilename;
	}

//Construct an object representing a const source descriptor
TExifTransformSourceDest::TExifTransformSourceDest(const TDesC8& aData)
	{
	iType = EData;
	iData = &aData;
	}

//Construct an object representing a dest descriptor
TExifTransformSourceDest::TExifTransformSourceDest(HBufC8*& aDataPtr)
	{
	iType = EDataPtr;
	iDataPtr = &aDataPtr;
	}

//True if and only if object represents a source or dest descriptor
TBool TExifTransformSourceDest::IsData() const
	{
	return ((iType == EData) || (iType == EDataPtr));
	}

//True if and only if object represents a source or dest filename
TBool TExifTransformSourceDest::IsFilename() const
	{
	return iType == EFilename;
	}

//Returns the filename. Panics if object does not represent a file
const TDesC& TExifTransformSourceDest::Filename() const
	{
	if (iType != EFilename)
		User::Panic(_L("TExifTransformSourceDest not a filename"),KErrArgument);
	return *iFilename;
	}

//Returns the source descriptor. Panics if object does not represent a source descriptor
const TDesC8& TExifTransformSourceDest::Data() const
	{
	if (iType != EData)
		User::Panic(_L("TExifTransformSourceDest not a source desc"),KErrArgument);
	return *iData;
	}

//Returns the dest descriptor. Panics if object does not represent a dest descriptor
HBufC8*& TExifTransformSourceDest::DataPtr() const
	{
	if (iType != EDataPtr)
		User::Panic(_L("TExifTransformSourceDest not a dest desc"),KErrArgument);
	return *iDataPtr;
	}



HBufC8* TImage::CreateUnicodePrefixedBufferL(const TDesC16& aUnicodeBuffer)
	{
	HBufC8* result = HBufC8::NewL((aUnicodeBuffer.Length()*2) + KUnicode().Length());
	result->Des().Append(KUnicode);
	result->Des().Append(TPtrC8((const TText8*)aUnicodeBuffer.Ptr(),aUnicodeBuffer.Size()));
	return result;
	}

HBufC8* TImage::CreateAsciiPrefixedBufferL(const TDesC8& aAsciiBuffer)
	{
	HBufC8* result = HBufC8::NewL((aAsciiBuffer.Length()) + KAscii().Length());
	result->Des().Append(KAscii);
	result->Des().Append(aAsciiBuffer);
	return result;
	}

TImage::TImage(const TPerformanceTestParams* aPerfTstParams, const TDesC& aTestName):
	RTestExtra(aTestName),
	iPerfTstParams(aPerfTstParams),
	iActiveListener(NULL),
	iUseThread(EFalse)
	{
	iUniqueNumber = 0;
	iDecoderOptions=CImageDecoder::EOptionNone;
	}

void TImage::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iImageFiles.AppendL(&KImageTestExifTransformDest);
	iImageFiles.AppendL(&KImageStreamToFile);
	iImageFiles.AppendL(&KImageFrameTestEncode420c);
	iImageFiles.AppendL(&KImageFrameTestEncode420d);
	iImageFiles.AppendL(&KImageFrameTestEncode420t);
	iImageFiles.AppendL(&KImageFrameTestEncode422c);
	iImageFiles.AppendL(&KImageFrameTestEncode422d);
	iImageFiles.AppendL(&KImageFrameTestEncodeMonoc);
	iImageFiles.AppendL(&KImageFrameTestEncodeMonod);
	iImageFiles.AppendL(&KImageFrameTestNegative);
	iImageFiles.AppendL(&KImageTempFileName);
	iImageFiles.AppendL(&KImageGetThumbnail);
	iImageFiles.AppendL(&KImageTestThumbnailNoCommentsDest);
	iImageFiles.AppendL(&KImageFrameTestEncode420ifd);
	iImageFiles.AppendL(&KImageTestExifTransformSquareDest);
	iImageFiles.AppendL(&KImageTestExifTransformBigDest);
	iImageFiles.AppendL(&KTestImageJpegDistortedEncoded);
	iImageFiles.AppendL(&KImageFrameTestRef444);
	iImageFiles.AppendL(&KImageFrameTestEncode420t);
	iImageFiles.AppendL(&KImageFrameTestEncode444d);
	iImageFiles.AppendL(&KImageFrameTestEncode444c);
	iImageFiles.AppendL(&KImageTestExifTransformDest);
	iImageFiles.AppendL(&KImageTestThumbnailNoCommentsDest);

	iActiveListener = new (ELeave) CActiveListener;
	}

void TImage::Test()
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

void TImage::End()
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
//		Helpers
//
CJPEGExifDecoder* TImage::CreateDecoderOnFileL(const TFileName& aFileName)
	{
	CJPEGExifDecoder* decoder = NULL;
	if(iDecoderUid.iUid != 0)
		{
		// intitialize the decoder
		#if defined(__ICL_PROFILING)
			RDebug::ProfileStart(1);
			TRAPD(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aFileName, DecoderOptions(), iDecoderUid)));
			RDebug::ProfileEnd(1);
			User::LeaveIfError(error);
		#else
			decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), KImageTypeJPGUid, KNullUid, iDecoderUid));
		#endif //defined(__ICL_PROFILING)
		}
	else
		{
		// intitialize the decoder
		#if defined(__ICL_PROFILING)
			RDebug::ProfileStart(1);
			TRAPD(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aFileName, DecoderOptions(), KImageTypeJPGUid)));
			RDebug::ProfileEnd(1);
			User::LeaveIfError(error);
		#else
			decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), KImageTypeJPGUid));
		#endif //defined(__ICL_PROFILING)			
		}		
	return decoder;
	}

CJPEGExifEncoder* TImage::CreateEncoderOnFileL(const TFileName& aFileName)
	{

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(aFileName,0,0,dir));

	const TInt entries = dir->Count();
	if(entries)
		{
		iFs.Delete(aFileName);
		}
	delete dir;


	// intitialize the decoder
#if defined(__ICL_PROFILING)
	RDebug::ProfileStart(1);
	CJPEGExifEncoder* encoder = NULL;
	TRAPD(error, encoder = STATIC_CAST(CJPEGExifEncoder*, CJPEGExifEncoder::FileNewL(iFs, aFileName, EncoderOptions(), KImageTypeJPGUid)));
	RDebug::ProfileEnd(1);
	User::LeaveIfError(error);
#else
	CJPEGExifEncoder* encoder = STATIC_CAST(CJPEGExifEncoder*, CJPEGExifEncoder::FileNewL(iFs, aFileName, EncoderOptions(), KImageTypeJPGUid));
#endif //defined(__ICL_PROFILING)

	// switch encoder from JFIF (default) to EXIF
	encoder->ExifMetadata();

	return encoder;
	}

TBool TImage::CompareBitmapsL(const CFbsBitmap& aBitmap1, const TDesC& aRefFileName)
	{
	CFbsBitmap* bmpRef=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bmpRef);
	TInt bmpErr= bmpRef->Load(aRefFileName);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref bitmap file (err=%d)  %S"),bmpErr, &aRefFileName);
		User::Leave(KErrGeneral);
		}

	const TBool r=CompareBitmaps(aBitmap1, *bmpRef);
	CleanupStack::PopAndDestroy(bmpRef);
	return r;
	}


TBool TImage::CompareBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2)
	{
	const TSize size = aBitmap1.SizeInPixels();

	if (size != aBitmap2.SizeInPixels())
		{
		Printf(_L("-- ERROR: Bitmap sizes differ!"));
		return EFalse;
		}

	if (aBitmap1.DisplayMode() != aBitmap2.DisplayMode())
		{
		Printf(_L("-- ERROR: Bitmap display modes differ!"));
		return EFalse;
		}

	const TInt scanLength = aBitmap1.ScanLineLength(size.iWidth, aBitmap1.DisplayMode() );

	// create two scanline desriptors of correct length
	TUint8* scanBuffer1 = STATIC_CAST(TUint8*, User::Alloc(scanLength));
	__ASSERT_ALWAYS(scanBuffer1!=NULL, User::Panic(_L("scan buffer 1 alloc failed"), KErrNoMemory));
	TPtr8 scanLine1 (scanBuffer1, 0, scanLength);

	TUint8* scanBuffer2 = STATIC_CAST(TUint8*, User::Alloc(scanLength));
	__ASSERT_ALWAYS(scanBuffer2!=NULL, User::Panic(_L("scan buffer 2 alloc failed"), KErrNoMemory));
	TPtr8 scanLine2 (scanBuffer2, 0, scanLength);

	for (TInt y = 0; y < size.iHeight; y++)
		{
		aBitmap1.GetScanLine(scanLine1,TPoint(0, y), size.iWidth, aBitmap1.DisplayMode() );
		aBitmap2.GetScanLine(scanLine2,TPoint(0, y), size.iWidth, aBitmap2.DisplayMode() );

		if (scanLine1.Compare(scanLine2)!=0)
			{
			User::Free(scanBuffer1);
			User::Free(scanBuffer2);
			return EFalse;
			}
		}

	User::Free(scanBuffer1);
	User::Free(scanBuffer2);

	return ETrue;
	}
//
//		Helpers
//
//
//



//
//
//
//		Getter tests
//
void TImage::GetExifData(const TExifDataType& aDataType, const TExifOperation& aOperation)
	{
	Printf(_L("\n"));
	Printf(_L("--- GetExifData test(%d) --- %S"), aDataType, &KImageTestExifDecode);


	//
	//		Get the metadata
	TFileName fileName(KImageTestExifDecode);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}

	MExifMetadata* metaData=decoder->ExifMetadata();
	//		Get the metadata
	//

	//
	//		Test the data
	switch(aDataType)
		{
		case E8BitBuffer:
			testResult=Get8bitBuffer(metaData, aOperation);
			break;
		case E16BitBuffer:
			testResult=Get16bitBuffer(metaData, aOperation);
			break;
		case EIntegerBuffer:
			testResult=GetInteger(metaData, aOperation);
			break;
		case ERationalBuffer:
			testResult=GetRational(metaData, aOperation);
			break;
		case ETagNotExistsError:
			testResult=GetNotExistError(metaData, aOperation);
			break;
		case EInvalidFormatError:
			testResult=GetInvalidFormat(metaData, aOperation);
			break;
		case ERationalArrayBuffer:
			testResult=GetRationalArray(metaData, aOperation);
			break;
		case EIntegerArrayBuffer:
			testResult=GetIntegerArray(metaData, aOperation);
			break;
		case EShortBuffer:
			testResult=GetShort(metaData, aOperation);
			break;
		case EShortArrayBuffer:
			testResult=GetShortArray(metaData, aOperation);
			break;
		default:
			break;
		}
	// destroy the decoder
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

void TImage::GetExifShortData(const TExifDataType& aDataType, const TExifOperation& aOperation)
	{
	//
	// Test the Motorola format...
	//

	Printf(_L("\n"));
	Printf(_L("--- GetExifShortData test(%d) --- %S"), aDataType, &KTestImageShortTagMM);


	//
	//		Get the metadata
	TFileName fileName(KTestImageShortTagMM);
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}

	MExifMetadata* metaData=decoder->ExifMetadata();
	//		Get the metadata
	//

	//
	//		Test the data
	switch(aDataType)
		{
		case EShortestShortArrayBuffer:
			testResult=GetShortestShortArray(metaData, aOperation);
			break;
		default:
			break;
		}
	// destroy the decoder
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


	//
	// Test the Intel format...
	//

	Printf(_L("\n"));
	Printf(_L("--- GetExifShortData test(%d) --- %S"), aDataType, &KTestImageShortTagII);


	//
	//		Get the metadata
	fileName = KTestImageShortTagII;
	decoder=NULL;
	TRAP(testResult, decoder=CreateDecoderOnFileL(fileName));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}

	metaData=decoder->ExifMetadata();
	//		Get the metadata
	//

	//
	//		Test the data
	switch(aDataType)
		{
		case EShortestShortArrayBuffer:
			testResult=GetShortestShortArray(metaData, aOperation);
			break;
		default:
			break;
		}
	// destroy the decoder
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


TInt TImage::Get8bitBuffer(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	HBufC8* buffer8Bit=NULL;

	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetParam8(0, 0, buffer8Bit);
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	_LIT8(K8BitBufferTestResult, "CYBERSHOT");
	const TUint KTagID=0x110;

	// get the data

	TInt err=aMetaData->GetParam8(KTagID, 0, buffer8Bit);

	// test the result
	if ( (err==KErrNone) && (buffer8Bit->Compare(K8BitBufferTestResult())) )
		{
		err=KErrCorrupt;
		}

	delete buffer8Bit;

	return err;
	}

TInt TImage::Get16bitBuffer(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	HBufC16* buffer16Bit=NULL;
	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetParam16(0, 0, buffer16Bit);
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}

	// Expected result:
	//_LIT16(K16BitBufferTestResult, "Canon DIGITAL IXUS");
	_LIT8(K8BitBufferTestResult, "ASCII\0\0\0Test");
	const TUint KTagID=0x9286; // The only tag that _may_ contain 16 bit data.

	// get the data
	TInt err=aMetaData->GetParam16(KTagID, 0, buffer16Bit);
	TPtrC ref( reinterpret_cast<const TText16*>(K8BitBufferTestResult().Ptr()), (K8BitBufferTestResult().Length()+1)>>1);

	// test the result
	if( (err==KErrNone) && (buffer16Bit->Compare(ref)) )
		{
		err=KErrCorrupt;
		}
	else
		{
		delete buffer16Bit;
		}
	return err;
	}

TInt TImage::GetInteger(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TInt integer=0;

	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetIntegerParam(0, 0, integer);
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	const TInt KIntegerTestResult=640;
	const TUint KTagID=0xa002;

	// get the data
	TInt err=aMetaData->GetIntegerParam(KTagID, 0, integer);

	// test the result
	if( (err==KErrNone) && (integer!=KIntegerTestResult) )
		{
		err=KErrCorrupt;
		}

	return err;
	}

TInt TImage::GetShort(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TUint16 param=0;

	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetShortParam(0, 0, param);
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	const TUint16 KShortTestResult=2;
	const TUint KTagID=0x0213;

	// get the data
	TInt err=aMetaData->GetShortParam(KTagID, 0, param);

	// test the result
	if( (err==KErrNone) && (param!=KShortTestResult) )
		{
		err=KErrCorrupt;
		}

	return err;
	}

TInt TImage::GetRational(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TInt valueNum=0, valueDen=0;

	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetRationalParam(0, 0, valueNum, valueDen);
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	const TInt KRationalNumeratorTestResult=72;
	const TInt KRationalDenominatorTestResult=1;
	const TUint KTagID=0x011a;

	// get the data
	TInt err=aMetaData->GetRationalParam(KTagID, 0, valueNum, valueDen);

	// test the result
	if( (err==KErrNone) && ((valueNum!=KRationalNumeratorTestResult) || (valueDen!=KRationalDenominatorTestResult)) )
		{
		err=KErrCorrupt;
		}

	return err;
	}

TInt TImage::GetIntegerArray(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	CArrayFixFlat<TInt>* value=NULL;
	TRAPD(allocErr, value=new (ELeave) CArrayFixFlat<TInt>(1));
	if(allocErr!=KErrNone)
		return allocErr;

	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetIntegerArrayParam(0, 0, *value);
		delete value;
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	const TInt KInteger1TestResult= 21;
	const TUint KTagID=0x1001; // Tag holds 'any' number of integers, so test for two.

	TInt err=aMetaData->GetIntegerArrayParam(KTagID, 0, *value);
	if(err==KErrNone)
		{
		if(value->Count()!=1)
			{
			err=KErrCorrupt;
			}
		else	if(	(value->At(0) != KInteger1TestResult)
			 	)
			{
			err=KErrCorrupt;
			}
		}
	delete value;

	return err;
	}

TInt TImage::GetRationalArray(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	CArrayFixFlat<TRational>* value=NULL;
	TRAPD(allocErr, value=new (ELeave) CArrayFixFlat<TRational>(3));
	if(allocErr!=KErrNone)
		return allocErr;
	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetRationalArrayParam(0, 0, *value);
		delete value;
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	TRational KRational1TestResult= {10000, 10000}; // 1.0
	TRational KRational2TestResult={2000, 10000};   // 0.2
	TRational KRational3TestResult={12999, 10000};  // 1.3
	const TUint KTagID=0x0211; //

	TInt err=aMetaData->GetRationalArrayParam(KTagID, 0, *value);
	if(err==KErrNone)
		{
		if(value->Count()!=3)
			{
			err=KErrCorrupt;
			}
		else	if(	(value->At(0).iNumerator != KRational1TestResult.iNumerator)		||
			 		(value->At(0).iDenominator != KRational1TestResult.iDenominator)	||
			 		(value->At(1).iNumerator != KRational2TestResult.iNumerator)		||
			 		(value->At(1).iDenominator != KRational2TestResult.iDenominator)	||
			 		(value->At(2).iNumerator != KRational3TestResult.iNumerator)		||
			 		(value->At(2).iDenominator != KRational3TestResult.iDenominator)
			)
			{
			err=KErrCorrupt;
			}
		}
	delete value;

	return err;
	}

TInt TImage::GetShortArray(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	CArrayFixFlat<TUint16>* value=NULL;
	TRAPD(allocErr, value=new (ELeave) CArrayFixFlat<TUint16>(3));
	if(allocErr!=KErrNone)
		return allocErr;


	if(aOperation==ESetParam)
		{
		TInt err=aMetaData->SetShortArrayParam(0, 0, *value);
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	TUint16 KShort1TestResult= 8;
	TUint16 KShort2TestResult= 5;
	TUint16 KShort3TestResult= 7;
	const TUint KTagID=0x9214; // Tag contains three shorts.

	TInt err=aMetaData->GetShortArrayParam(KTagID, 0, *value);
	if(err==KErrNone)
		{
		if(value->Count()!=3)
			{
			err=KErrCorrupt;
			}
		else	if(	(value->At(0) != KShort1TestResult)	||
			 		(value->At(1) != KShort2TestResult)	||
			 		(value->At(2) != KShort3TestResult)
			 )
			{
			err=KErrCorrupt;
			}
		}
	delete value;

	return err;
	}

TInt TImage::GetShortestShortArray(MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	CArrayFixFlat<TUint16>* value=NULL;
	TRAPD(allocErr, value=new (ELeave) CArrayFixFlat<TUint16>(3));
	if(allocErr!=KErrNone)
		return allocErr;


	if(aOperation==ESetParam)
		{
		return KErrNotSupported;
		}


	// Two short test...

	Printf(_L("\n"));
	Printf(_L("--- Array of two Shorts ---"));

	// Expected result:
	TUint16 KShort1TestResult= 3;
	TUint16 KShort2TestResult= 5;
	TUint KTagID=0xA214; // Tag contains two shorts in the array.

	TInt err=aMetaData->GetShortArrayParam(KTagID, 0, *value);
	if(err==KErrNone)
		{
		if(value->Count()!=2)
			{
			err=KErrCorrupt;
			}
		else	if(	(value->At(0) != KShort1TestResult)	||
			 		(value->At(1) != KShort2TestResult)
			 )
			{
			err=KErrNotFound;
			}
		}

	if (err != KErrNone)
		{
		delete value;
		return err;
		}

	// Single short test
	// Note that shorts are written differently in array as oppose to standalone.
	// So a single short appears differently to an array with one short!

	Printf(_L("\n"));
	Printf(_L("--- Array of one Short ---"));

	// Expected result:
	KShort1TestResult= 17;
	KTagID=0x8827; // Tag contains one short in the array.

	err = aMetaData->GetShortArrayParam(KTagID, 0, *value);
	if(err==KErrNone)
		{
		if(value->Count()!=1)
			{
			err=KErrCorrupt;
			}
		else	if(	(value->At(0) != KShort1TestResult)
			 )
			{
			err=KErrNotFound;
			}
		}

	delete value;

	return err;
	}

TInt TImage::GetNotExistError(MExifMetadata* aMetaData, const TExifOperation& /*aOperation*/)
	{
	// Expected result:
	const TUint KTagID=0xffffffff;

	// get the data
	TInt integer=0;
	TInt err=aMetaData->GetIntegerParam(KTagID, 0, integer);

	// test the result
	if(err!=KErrNotFound)
		{
		err=KErrCorrupt;
		}
	else
		{
		err=KErrNone;
		}

	return err;
	}


TInt TImage::GetInvalidFormat(MExifMetadata* aMetaData, const TExifOperation& /*aOperation*/)
	{
	// Expected result:
	const TUint KTagID=0x9205;

	// get the data
	TInt integer=0;
	TInt err=aMetaData->GetIntegerParam(KTagID, 0, integer);

	// test the result
	if(err!=KErrArgument)
		{
		err=KErrCorrupt;
		}
	else
		{
		err=KErrNone;
		}

	return err;
	}
//
//		Getter tests
//
//
//




//
//
//
//		Setter tests
//
void TImage::SetExifData(const TExifDataType& aDataType, const TExifOperation& aOperation)
	{
	Printf(_L("\n"));
	Printf(_L("--- SetExifData test(%d) --- %S"), aDataType, &KImageTestExifEncode);

	//
	//		Load the reference image
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	TInt bmpErr=bitmap->Load(KEncodeSourceFileName);
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
	TFileName fileName(KImageTestExifEncode);
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

	//TestSetData()	will ADD tags of each data type to the metadata, with
	//the exception of Integer data.  This is because all tags with Integer
	//values are mandatory and therefore will already be present in the
	//metadata.
	testResult=TestSetData(aDataType, metaData, ESetData, aOperation);
	if(testResult!=KErrNone)
		{
		Printf(_L("unable to set data pass 1"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
		return;
		}

	//If aOperation==ESetData, then we call TestSetData() a second time.
	//This will SET data for all the tags this time around.
	if (aOperation==ESetData)
		{
		testResult=TestSetData(aDataType, metaData, ESetData, aOperation);
		if(testResult!=KErrNone)
			{
			Printf(_L("unable to set data pass 2"));
			TestFailed(testResult);
			CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder
			return;
			}
		}

	if(aOperation==EGetParam)
		{
		// the test for KErrNot Supported has succeeded
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
	CExifVerifier* verifier=NULL;
	TRAP(testResult, verifier=CExifVerifier::NewL(fileName, CExifVerifier::EJpegFile));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify Ifd Offsets
	VerifyIfdOffsets(*verifier, fileName);
	// verify the data
	metaData=verifier->ExifMetadata();


	testResult=TestSetData(aDataType, metaData, EVerifyData, aOperation);
	// destroy the decoder
	delete verifier;
	//		verify the result
	//

	// log the result
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	}


TInt TImage::TestSetData(const TExifDataType& aDataType, MExifMetadata* aMetadata, const TExifEncodeStep& aEncodeStep, const TExifOperation& aOperation)
	{
	TInt testResult=KErrNone;
	// get the metadata


	// test the data
	switch(aDataType)
		{
		case E8BitBuffer:
			testResult=Set8bitBuffer(aEncodeStep, aMetadata, aOperation);
			break;
		case E16BitBuffer:
			testResult=Set16bitBuffer(aEncodeStep, aMetadata, aOperation);
			break;
		case EIntegerBuffer:
			testResult=SetInteger(aEncodeStep, aMetadata, aOperation);
			break;
		case EShortBuffer:
			testResult=SetShort(aEncodeStep, aMetadata, aOperation);
			break;
		case ERationalBuffer:
			testResult=SetRational(aEncodeStep, aMetadata, aOperation);
			break;
		case EShortArrayBuffer:
			testResult=SetShortArray(aEncodeStep, aMetadata, aOperation);
			break;
		case ERationalArrayBuffer:
			testResult=SetRationalArray(aEncodeStep, aMetadata, aOperation);
			break;
		case EIntegerArrayBuffer:
			testResult=SetIntegerArray(aEncodeStep, aMetadata, aOperation);
			break;
		case EDefaultTagSet:
			testResult=SetDefaultTagSet(aEncodeStep, aMetadata, aOperation);
			break;
		default:
			break;
		}
	return testResult;
	}


TInt TImage::SetDefaultTagSet(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TInt testResult=KErrNone;
	if(aEncodeStep==ESetData || aOperation==EGetParam)
		{	// set the data in the encoder
		return KErrNone;
		}

	// verify that all the encoded data is here
	// Might be good to check the values!? SF
	// get the number of tags to test:
	// size of the array, divided by sizeof(first element) to get the number of elements
	// divided by 2 because there are 2 elements in each row
	for(TInt i=0; i<KNbMandatoryTags; i++)
		{
		TInt localResult=KErrNone;
		switch(KTagList[i][1])
			{
			case EByte:
				break;
			case EAscii:
					{
					HBufC8* data=NULL;
					localResult=aMetaData->GetParam8(KTagList[i][0], KTagList[i][2], data);
					delete data;
					}
				break;
			case EUnsignedShort:
					{
					TUint16 data=0;
					localResult=aMetaData->GetShortParam(KTagList[i][0], KTagList[i][2], data);
					}
				break;
			case EUnsignedLong:
					{
					TInt data=0;
					localResult=aMetaData->GetIntegerParam(KTagList[i][0], KTagList[i][2], data);
					}
				break;
			case EUnsignedRational:
					{
					TInt num=0, den=0;
					localResult=aMetaData->GetRationalParam(KTagList[i][0], KTagList[i][2], num, den);
					}
				break;
			case EUndefined:
					{
					HBufC8* data=NULL;
					localResult=aMetaData->GetParam8(KTagList[i][0], KTagList[i][2], data);
					delete data;
					}
				break;
			case ESignedLong:
					{
					TInt data=0;
					localResult=aMetaData->GetIntegerParam(KTagList[i][0], KTagList[i][2], data);
					}
				break;
			case ESignedRational:
					{
					TInt num=0, den=0;
					localResult=aMetaData->GetRationalParam(KTagList[i][0], KTagList[i][2], num, den);
					}
				break;
			}
		ProcessSetError(localResult, KTagList[i][0]);
		if(localResult!=KErrNone)
			{
			testResult=KErrNotFound;
			}
		}

	return testResult;
	}

TInt TImage::SetRational(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TInt num=0;
	TInt den=0;
	const TUint KTagID=0x9205;
	TInt testResult=KErrNone;

	if(aOperation==EGetParam)
		{
		testResult = aMetaData->GetRationalParam(KTagID, 0, num, den);
		if(testResult==KErrNotSupported)
			{
			return KErrNone;
			}
		return testResult;
		}

	// Expected result:
	const TInt KRationalNumeratorTestResult=194698;
	const TInt KRationalDenominatorTestResult=65536;


	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		return aMetaData->SetRationalParam(KTagID, 0, KRationalNumeratorTestResult, KRationalDenominatorTestResult);
		}

	testResult = aMetaData->GetRationalParam(KTagID, 0, num, den);
	if(testResult==KErrNone)
		{
		if((num!=KRationalNumeratorTestResult) || (den!=KRationalDenominatorTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}

TInt TImage::SetInteger(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TInt param=0;
	const TUint KTagID=0xa002;
	TInt testResult=KErrNone;

	if(aOperation==EGetParam)
		{
		testResult = aMetaData->GetIntegerParam(KTagID, 0, param);
		if(testResult==KErrNotSupported)
			{
			return KErrNone;
			}
		return testResult;
		}

	// Expected result:
	const TInt KIntegerTestResult=640;

	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		return aMetaData->SetIntegerParam(KTagID, 0, KIntegerTestResult);
		}

	testResult = aMetaData->GetIntegerParam(KTagID, 0, param);
	if(testResult==KErrNone)
		{
		if((param!=KIntegerTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}

TInt TImage::SetShort(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	TUint16 param=0;
	const TUint KTagID=0x8822;
	TInt testResult=KErrNone;

	if(aOperation==EGetParam)
		{
		testResult = aMetaData->GetShortParam(KTagID, 0, param);
		if(testResult==KErrNotSupported)
			{
			return KErrNone;
			}
		return testResult;
		}

	// Expected result:
	const TUint16 KShortTestResult=2;


	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		return aMetaData->SetShortParam(KTagID, 0, KShortTestResult);
		}

	testResult = aMetaData->GetShortParam(KTagID, 0, param);
	if(testResult==KErrNone)
		{
		if((param!=KShortTestResult))
			{
			testResult=KErrCorrupt;
			}
		}
	return testResult;
	}

TInt TImage::Set16bitBuffer(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	HBufC16* param=NULL;
	const TUint KTagID=0x9286;
	TInt testResult=KErrNone;

	if(aOperation==EGetParam)
		{
		testResult = aMetaData->GetParam16(KTagID, 0, param);
		if(testResult==KErrNotSupported)
			{
			delete param;
			return KErrNone;
			}
		delete param;
		return testResult;
		}

	_LIT8(KUnicode, "UNICODE\0"); // 8 bit string at start of Unicode 16 bit data.
	_LIT16(K16BitBufferTestResult, "This is a 16Bit Buffer");

	if(aEncodeStep==ESetData)
		{
		// set the data in the encoder
		HBufC16* buffer=K16BitBufferTestResult().Alloc();
		if(buffer==NULL)
			{
			return KErrGeneral;
			}
		testResult=aMetaData->SetParam16(KTagID, 0, buffer);
		delete buffer;
		return testResult;
		}

	TBuf8<128> refString(KUnicode);
	refString.Append(TPtrC8((const TText8*)K16BitBufferTestResult().Ptr(),K16BitBufferTestResult().Size()) );

	testResult = aMetaData->GetParam16(KTagID, 0, param);
	if(testResult==KErrNone)
		{
		TPtrC8 result((const TText8*)param->Ptr(), param->Size());
		if( result.Compare(refString) != 0 )
			{
			testResult=KErrCorrupt;
			}
		}
	delete param;
	return testResult;
	}

TInt TImage::Set8bitBuffer(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	HBufC8* param=0;
	const TUint KTagID=0x8298;
	TInt testResult=KErrNone;

	if(aOperation==EGetParam)
		{
		testResult = aMetaData->GetParam8(KTagID, 0, param);
		if(testResult==KErrNotSupported)
			{
			return KErrNone;
			}
		return testResult;
		}


	_LIT8(K8BitBufferTestResult, "This is a 8Bit Buffer");

	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		HBufC8* buffer=K8BitBufferTestResult().Alloc();
		if(buffer==NULL)
			{
			return KErrNoMemory;
			}
		testResult= aMetaData->SetParam8(KTagID, 0, buffer);
		delete buffer;
		return testResult;
		}


	testResult = aMetaData->GetParam8(KTagID, 0, param);
	if(testResult==KErrNone)
		{
		if(param->Des().Compare(K8BitBufferTestResult))
			{
			testResult=KErrCorrupt;
			}
		delete param;
		}
	return testResult;
	}

TInt TImage::SetIntegerArray(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	CArrayFixFlat<TInt>* value=NULL;
	TRAPD(allocErr, value=new (ELeave) CArrayFixFlat<TInt>(1));
	if(allocErr!=KErrNone)
		return allocErr;

	const TUint KTagID=0x1001; // Tag holds 'any' number of integers, so test for two.
	TInt err=KErrNone;

	if(aOperation==EGetParam)
		{
		err=aMetaData->GetIntegerArrayParam(KTagID, 0, *value);
		delete value;
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	TInt KInteger1TestResult= 21;
	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		TRAP(err,
			{
			value->SetReserveL(1);
			value->AppendL(KInteger1TestResult);
			});
		if(err==KErrNone)
			{
			err=aMetaData->SetIntegerArrayParam(KTagID, 0, *value);
			}
		}
	else
		{
		TInt err=aMetaData->GetIntegerArrayParam(KTagID, 0, *value);
		if(	(err==KErrNone)		&&
			((value->At(0) != KInteger1TestResult))
			 )
			{
			err=KErrCorrupt;
			}
		}

	delete value;

	return err;
	}

TInt TImage::SetRationalArray(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	CArrayFixFlat<TRational>* value=NULL;
	TInt err=KErrNone;
	TRAP(err, value=new (ELeave) CArrayFixFlat<TRational>(3));
	if(err!=KErrNone)
		{
		return err;
		}
	const TUint KTagID=0x0211;
	if(aOperation==EGetParam)
		{
		err=aMetaData->GetRationalArrayParam(KTagID, 0, *value);
		delete value;
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	TRational KRational1TestResult= {325770, 65536};
	TRational KRational2TestResult={325770, 65536};
	TRational KRational3TestResult={325770, 65536};

	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		TRAP(err,
			{
			value->SetReserveL(3);
			value->AppendL(KRational1TestResult);
			value->AppendL(KRational2TestResult);
			value->AppendL(KRational3TestResult);
			});
		if(err==KErrNone)
			{
			err=aMetaData->SetRationalArrayParam(KTagID, 0, *value);
			}
		}
	else
		{	// get the encoded data
		err=aMetaData->GetRationalArrayParam(KTagID, 0, *value);
		if(	(err==KErrNone)		&&
			((value->At(0).iNumerator 	!= KRational1TestResult.iNumerator)		||
			 (value->At(0).iDenominator	!= KRational1TestResult.iDenominator)	||
			 (value->At(1).iNumerator 	!= KRational2TestResult.iNumerator)		||
			 (value->At(1).iDenominator	!= KRational2TestResult.iDenominator)	||
			 (value->At(2).iNumerator	!= KRational3TestResult.iNumerator)		||
			 (value->At(2).iDenominator	!= KRational3TestResult.iDenominator)	)
			)
			{
			err=KErrCorrupt;
			}
		}
	delete value;

	return err;
	}

TInt TImage::SetShortArray(const TExifEncodeStep& aEncodeStep, MExifMetadata* aMetaData, const TExifOperation& aOperation)
	{
	const TUint KTagID=0x9214; // Tag contains three shorts.

	CArrayFixFlat<TUint16>* value=NULL;
	TRAPD(allocErr, value=new (ELeave) CArrayFixFlat<TUint16>(3));
	if(allocErr!=KErrNone)
		return allocErr;

	TInt err=KErrNone;
	if(aOperation==EGetParam)
		{
		err=aMetaData->SetShortArrayParam(KTagID, 0, *value);
		delete value;
		if(err==KErrNotSupported)
			{
			return KErrNone;
			}
		return err;
		}


	// Expected result:
	TUint16 KShort1TestResult= 8;
	TUint16 KShort2TestResult= 5;
	TUint16 KShort3TestResult= 7;

	if(aEncodeStep==ESetData)
		{	// set the data in the encoder
		TRAP(err,
			{
			value->SetReserveL(3);
			value->AppendL(KShort1TestResult);
			value->AppendL(KShort2TestResult);
			value->AppendL(KShort3TestResult);
			});
		if(err==KErrNone)
			{
			err=aMetaData->SetShortArrayParam(KTagID, 0, *value);
			}

		return err;
		}
	else
		{
		err=aMetaData->GetShortArrayParam(KTagID, 0, *value);
		if(	(err==KErrNone)		&&
			((value->At(0) != KShort1TestResult)	||
			 (value->At(1) != KShort2TestResult)	||
			 (value->At(2) != KShort3TestResult))
			 )
			{
			err=KErrCorrupt;
			}
		}

	delete value;

	return err;
	}

void TImage::ProcessSetError(const TInt& aError, const TInt& aTag)
	{
	if(aError!=KErrNone)
		{
		TBufC<256> errorMsg=_L("Error on tag ");
		TPtr msgPtr=errorMsg.Des();
		msgPtr.AppendFormat(_L("%X "), aTag);

		switch(aError)
			{
			case KErrNotFound:
				msgPtr.Append(_L("Not found"));
				break;
			case KErrNotSupported:
				msgPtr.Append(_L("Not supported"));
				break;
			case KErrArgument:
				msgPtr.Append(_L("invalid argument"));
				break;
			case KErrOverflow:
				msgPtr.Append(_L("overflow"));
				break;
			case KErrUnderflow:
				msgPtr.Append(_L("UnderFlow"));
				break;
			}
		Printf(errorMsg);
		}
	}
//
//		Setter tests
//
//
//



//
//
//
//		Thumbnail tests
//
void TImage::TestThumbnail(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Exif thumbnail test(%d) --- %S"), aDataType, &KImageTestExifDecode);

	switch(aDataType)
		{
		case EGetThumbnail:
			TestGetThumbnail(TFileName(KImageTestExifDecode), KDecodeThumbnailFileName);
			break;
		case EGetThumbnailError:
			//ETrue in TestGetThumbnailError tells it to expect to find a thumbnail
			TestGetThumbnailError(TFileName(KImageTestExifDecode), ETrue);
			break;
		case ESetThumbnail:
			TestSetThumbnail();
			break;
		case ESetNoThumbnail:
			TestSetNoThumbnail();
			break;
		case EGetThumbnailHeader:
			TestThumbnailHeader();
			break;
		case EGetCorruptThumbnailHeader:
			TestSetImageTypeOnCorruptedHeaderFiles();
			break;
		}
	}

void TImage::TestGetThumbnail(const TFileName& aSourceFileName, const TDesC& aReferenceFile)
	{
	//
	//		Get the thumbnail
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError( bitmap->Create(KThumbnailSize, KThumbnailDispMode) );

	CJPEGExifDecoder* decoder=NULL;
	TRAPD(allocErr, decoder=CreateDecoderOnFileL(aSourceFileName));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}
	CleanupStack::PushL(decoder);
	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail))
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not set thumbnail getter on the decoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(2, bitmap);
		return;
		}

	// launch the conversion
	TInt decodeResult=DoImageDecodeL(decoder, *bitmap);

	// destroy the decoder
	CleanupStack::PopAndDestroy(decoder);

	if(decodeResult!=KErrNone)
		{
		Printf(_L("Unable to encode image"));
		TestFailed(decodeResult);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}

	//		Get the thumbnail
	//


	//
	//		test the result
	CFbsBitmap* thumbRef=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(thumbRef);
	TInt bmpErr=thumbRef->Load(aReferenceFile);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref thumbnail file"));
		TestFailed(bmpErr);
		CleanupStack::PopAndDestroy(2); // thumbRef, bitmap
		return;
		}

	TBool compResult=CompareBitmaps(*thumbRef, *bitmap);
	CleanupStack::PopAndDestroy(2); // thumbRef, bitmap
	if(compResult==EFalse)
		{
		Printf(_L("reference and read thumbnail files don't match"));
		TestFailed(KErrCorrupt);
		}
	//		test the result
	//
	}


TInt TImage::TestGetThumbnailError(const TExifTransformSourceDest& aDest, const TBool aThumbnailExpected)
	{
	//At the moment, only used with file dests
	if (!aDest.IsFilename())
		{
		TestFailed(KErrArgument);
		return KErrArgument;
		}
	//
	//		Get the thumbnail
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError( bitmap->Create(KThumbnailSize, KThumbnailDispMode));

	CJPEGExifDecoder* decoder=NULL;
	TRAPD(err, decoder=CreateDecoderOnFileL(aDest.Filename()));
	if(err!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(err);
		CleanupStack::PopAndDestroy(bitmap);
		return err;
		}
	CleanupStack::PushL(decoder);
	TRAP(err, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail));
	if (aThumbnailExpected)
		{
		if (err!=KErrNone)
			{
			Printf(_L("Could not set thumbnail getter on the decoder"));
			TestFailed(err);
			CleanupStack::PopAndDestroy(2, bitmap);
			return err;
			}
		}
	else
		{
		if (err!=KErrNotFound)
			{
			Printf(_L("found unexpected thumbnail"));
			TestFailed(KErrCorrupt);
			err=KErrCorrupt;
			}
		else
			{
			err = KErrNone;
			}
		CleanupStack::PopAndDestroy(2, bitmap);
		return err;
		}

	// launch the conversion
	err=DoImageDecodeL(decoder, *bitmap);
	CleanupStack::PopAndDestroy(2, bitmap);
	//		Get the thumbnail
	//
	if(err!=KErrNone)
		{
		TestFailed(err);
		}
	return err;
	}

void TImage::TestSetThumbnail()
	{
	//
	//		encode the image
	// load the bitmap
	CFbsBitmap* bitmap= new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);

	TInt bmpErr=bitmap->Load(KEncodeSourceFileName);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref bitmap file"));
		TestFailed(bmpErr);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}

	// convert it to jpeg format
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(allocErr, encoder=CreateEncoderOnFileL(TFileName(KImageTestExifEncode)));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create encoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}

	// launch the conversion
	CActiveEncodeListener encodeListener;
	encodeListener.InitialiseActiveListener(encoder);
	encoder->Convert(& encodeListener.iStatus, *bitmap);
	CActiveScheduler::Start();

	// destroy the encoder
	 delete encoder;
 	if(encodeListener.iStatus.Int()!=KErrNone)
		{
		Printf(_L("Unable to encode image"));
		TestFailed(encodeListener.iStatus.Int());
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}

	//		encode the image
	//


	//
	//		Get the thumbnail
	CFbsBitmap* thumbnail=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(thumbnail);
	User::LeaveIfError( thumbnail->Create(KThumbnailSize, KThumbnailDispMode) );

	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(TFileName(KImageTestExifEncode)));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(2);	// bitmap, thumbnail
		return;
		}
	CleanupStack::PushL(decoder);
	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail))
	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not set thumbnail getter on the decoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(3);	// bitmap, thumbnail, decoder
		return;
		}

	// launch the conversion
	TInt decodeResult=DoImageDecodeL(decoder, *thumbnail);

	// destroy the decoder
	CleanupStack::PopAndDestroy(decoder);

	if(decodeResult!=KErrNone)
		{
		Printf(_L("error encoding the file"));
		TestFailed(decodeResult);
		}
	//		Get the thumbnail
	//


	//
	//		test the result
	CFbsBitmap* thumbRef=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(thumbRef);

	bmpErr=thumbRef->Load(KEncodeThumbnailFileName);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref thumbnail file"));
		TestFailed(bmpErr);
		CleanupStack::PopAndDestroy(3);	// bitmap, thumbnail, thumbRef
		return;
		}


	TBool compResult=CompareBitmaps(*thumbRef, *thumbnail);

	CleanupStack::PopAndDestroy(3);	// bitmap, thumbnail, thumbRef
	if(compResult==EFalse)
		{
		Printf(_L("reference and generated thumbnail files don't match"));
		TestFailed(compResult);
		}
	//		test the result
	//
	}

void TImage::TestSetNoThumbnail()
	{
	//
	//		encode the image
	// load the bitmap
	CFbsBitmap* bitmap= new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	TInt bmpErr=bitmap->Load(KEncodeSourceFileName);
	if(bmpErr!=KErrNone)
		{
		Printf(_L("could not load ref bitmap file"));
		TestFailed(bmpErr);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}

	// convert it to jpeg format
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(allocErr, encoder=CreateEncoderOnFileL(TFileName(KImageTestExifEncode)));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create encoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}
	CleanupStack::PushL(encoder);
	encoder->SetThumbnail(EFalse);
	// launch the conversion
	CActiveEncodeListener* encodeListener=new (ELeave) CActiveEncodeListener;
	CleanupStack::PushL(encodeListener);

	encodeListener->InitialiseActiveListener(encoder);

	encoder->Convert(& encodeListener->iStatus, *bitmap);
	CActiveScheduler::Start();

	TInt encodeResult=encodeListener->iStatus.Int();

	// destroy the encoder
	CleanupStack::PopAndDestroy(3, bitmap);	// listener, encoder, bitmap
 	if(encodeResult!=KErrNone)
		{
		Printf(_L("Unable to encode image"));
		TestFailed(encodeResult);
		return;
		}

	//		encode the image
	//


	//
	//		Get the thumbnail
	CFbsBitmap* thumbnail=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(thumbnail);
	User::LeaveIfError( thumbnail->Create(KThumbnailSize, KThumbnailDispMode) );

	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(TFileName(KImageTestExifEncode)));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(thumbnail);
		return;
		}
	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail))
	if(allocErr!=KErrNotFound)
		{
		Printf(_L("a thumbnail has been found"));
		TestFailed(allocErr);
		}

	delete decoder;
	CleanupStack::PopAndDestroy(thumbnail);
	}
//
//		Thumbnail tests
//
//
//



//
//
//
//		Transform tests
//

void TImage::TransformExif(const TExifDataType& aDataType, const TExifOperation& aOperation)
	{
	switch(aOperation)
		{
		case ESetParam:
			TransformExifSet(aDataType);
			break;
		case EGetParam:
			TransformExifGet(aDataType);
			break;
		case ENoParam:
			TransformExifBasic(aDataType);
			break;
		default:
			break;
		}
	}

void TImage::TransformExifBasic(const TExifDataType& aDataType)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform test(%d) --- %S"), aDataType, &KImageTestTransformSource2);

	TFileName sourceFileName(KImageTestTransformSource2);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform = NULL;
	TRAPD(allocErr, imageTransform = PrepareImageTransformL(sourceFileName, destFileName, EFalse, ETrue));
	if (allocErr != KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData = extension->ExifMetadata();


	// encode the image
	TInt transformResult = DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);

	if (transformResult != KErrNone)
		{
		Printf(_L("An error occurred while transforming the image"));
		TestFailed(transformResult);
		return;
		}

	VerifyAgainstReferenceFile(destFileName, KImageTestTransformSource2Ref());
	}

void TImage::TransformExifGet(const TExifDataType& aDataType)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Getters test(%d) --- %S"), aDataType, &KImageTestExifDecode);


	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestExifDecode);
	TFileName destFileName(KImageTestExifTransformDest);

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
	//		Test the data
	TInt testResult=KErrNone;
	switch(aDataType)
		{
		case E8BitBuffer:
			testResult=Get8bitBuffer(metaData, EGetParam);
			break;
		case E16BitBuffer:
			testResult=Get16bitBuffer(metaData, EGetParam);
			break;
		case EIntegerBuffer:
			testResult=GetInteger(metaData, EGetParam);
			break;
		case ERationalBuffer:
			testResult=GetRational(metaData, EGetParam);
			break;
		case ETagNotExistsError:
			testResult=GetNotExistError(metaData, EGetParam);
			break;
		case EInvalidFormatError:
			testResult=GetInvalidFormat(metaData, EGetParam);
			break;
		case ERationalArrayBuffer:
			testResult=GetRationalArray(metaData, EGetParam);
			break;
		case EIntegerArrayBuffer:
			testResult=GetIntegerArray(metaData, EGetParam);
			break;
		case EShortBuffer:
			testResult=GetShort(metaData, EGetParam);
			break;
		case EShortArrayBuffer:
			testResult=GetShortArray(metaData, EGetParam);
			break;
		default:
			break;
		}

	metaData=NULL;
	CleanupStack::PopAndDestroy(imageTransform);
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

void TImage::TransformExifSet(const TExifDataType& aDataType)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Setters test(%d) --- %S"), aDataType, &KImageTestExifTransformSource);

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	if (aDataType == EDefaultTagSet)
		sourceFileName = KImageTestTransformSource;


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
	TInt testResult=TestSetData(aDataType, metaData, ESetData, ESetParam);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while setting data"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	TInt transformResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);

	//
	//		verify the result
	if(transformResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
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

	testResult=TestSetData(aDataType, metaData, EVerifyData, ESetParam);
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

void TImage::TransformThumbnailFromExifImage(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	TransformThumbnail(aDataType, TFileName(KImageTestExifTransformSource));
	}
void TImage::TransformThumbnailFromJpegImage(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	TransformThumbnail(aDataType, TFileName(KImageTestTransformSource));
	}
void TImage::TransformThumbnailFromThumblessExifImage(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	TransformThumbnail(aDataType, TFileName(KImageTestThumblessExifTransformSource));
	}

void TImage::TransformThumbnail(const TExifDataType& aDataType, const TFileName& aSourceFileName)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Thumbnail test(%d) --- %S"), aDataType, &aSourceFileName);

	__UHEAP_MARK;
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TBool addThumbnail=ETrue;
	switch(aDataType)
	{
	case ESetNoThumbnail:
		addThumbnail=EFalse;
		break;
	case EGetThumbnailError:
	case ESetThumbnail:
		break;
	default:
		Printf(_L("Unexpected DataType %d"), aDataType);
		TestFailed(KErrArgument);
		return;
	}

	TRAPD(allocErr,imageTransform=PrepareImageTransformL(aSourceFileName,destFileName,addThumbnail));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	if(aDataType==EGetThumbnailError)
		{
		delete imageTransform;
		return;
		}

	//
	//		encode the image
	CleanupStack::PushL(imageTransform);
	DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	//		encode the image
	//

	//
	//		Get the thumbnail
	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(destFileName));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(decoder);

	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail))

	if(aDataType==ESetNoThumbnail)
		{
		if(allocErr!=KErrNotFound)
			{
			Printf(_L("An unexpected thumbnail has been found"));
			TestFailed(allocErr);
			}
		CleanupStack::PopAndDestroy(decoder);
		return;
		}

	if(allocErr!=KErrNone)
		{
		Printf(_L("Could not set thumbnail getter on the decoder"));
		TestFailed(allocErr);
		CleanupStack::PopAndDestroy(decoder);
		return;
		}

	CFbsBitmap* thumbnail= new (ELeave) CFbsBitmap;
	CleanupStack::PushL(thumbnail);
	thumbnail->Create(KThumbnailSize, KThumbnailDispMode);

	// launch the conversion
	TInt decodeResult=DoImageDecodeL(decoder, *thumbnail);
	CleanupStack::PopAndDestroy(2, decoder);
	//		Get the thumbnail
	//

	if(aDataType==ESetNoThumbnail)
		{
		if(decodeResult==KErrNone)
			{
			Printf(_L("There is a thumbnail in the encoded file"));
			TestFailed(decodeResult);
			}
		}
	else
		{
		if(decodeResult!=KErrNone)
			{
			Printf(_L("Could not load image from the encoded file"));
			TestFailed(decodeResult);
			}
		}

	//Make sure any heap cells owned by REComSession are not included in the
	//heap cell count.
	REComSession::FinalClose();
	//We are testing for memory leaks here for DEF083700.
	//We use __UHEAP_MARKENDC(1) here because we expect there to be one cell
	//left allocated.  This is because memory for RFbsSession::iScanLineBuffer
	//will have been allocated during the transform.  This memory is owned by
	//and will be destroyed by RFbsSession when RFbsSession::Disconnect() is
	//called (during TImage::Close()), so it is not leaked memory.
	__UHEAP_MARKENDC(1);
	}

// INC083025: ICL Image Thumbnail Transform does not set thumbnail exif tags //
void TImage::VerifyIFD1FromThumblessExifSource(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	VerifyIFD1(aDataType, TFileName(KImageTestThumblessExifTransformSource));
	}

void TImage::VerifyIFD1FromThumbedExifSource(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	VerifyIFD1(aDataType, TFileName(KImageTestExifTransformSource));
	}

void TImage::VerifyIFD1(const TExifDataType& aDataType, const TFileName& aSourceFileName)
	{
	Printf(_L("\n"));
	Printf(_L("--- Verify IFD1 test (%d) --- %S"), aDataType, &aSourceFileName);

	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TBool addThumbnail=ETrue;
	switch(aDataType)
		{
		case ESetNoThumbnail:
			addThumbnail=EFalse;
			break;
		case ESetThumbnail:
			break;
		default:
			Printf(_L("Unexpected DataType %d"), aDataType);
			TestFailed(KErrArgument);
			return;
		}

	TRAPD(allocErr,imageTransform=PrepareImageTransformL(aSourceFileName,destFileName,addThumbnail));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	//
	//		encode the thumbnail
	CleanupStack::PushL(imageTransform);
	DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	//		encode the thumbnail
	//

	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(destFileName));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(decoder);

	//
	// Check the destination file for mandatory IFD1 tags.
	//
	TBool testFailed = EFalse;
	TInt localTestResult = KErrNone;
	MExifMetadata* metaData=decoder->ExifMetadata();

	// Check each mandatory IFD1 tag one-by-one

	// 0x0103 - Compression - should equal 6 for a JPEG thumbnail
	TUint16 shortData=0;
	localTestResult = metaData->GetShortParam(0x0103, 1, shortData);
	if ((aDataType == ESetNoThumbnail) && (localTestResult != KErrNotFound))
		{
		Printf(_L("Found unexpected IFD1 compression tag 0x0103"));
		testFailed = ETrue;
		}
	else if ((aDataType == ESetThumbnail) && (shortData != 6))
		{
		Printf(_L("Error with IFD1 compression tag 0x0103 (%d)"), shortData);
		testFailed = ETrue;
		}

	// 0x011A - XResolution - should be 72/1
	TInt num=0, den=0;
	localTestResult=metaData->GetRationalParam(0x011A, 1, num, den);
	if ((aDataType == ESetNoThumbnail) && (localTestResult != KErrNotFound))
		{
		Printf(_L("Found unexpected IFD1 XResolution tag 0x011A"));
		testFailed = ETrue;
		}
	else if ((aDataType == ESetThumbnail) && ((num != 72) || (den != 1)))
		{
		Printf(_L("Error with IFD1 XResolution tag 0x011A (%d/%d)"), num, den);
		testFailed = ETrue;
		}

	// 0x011B - YResolution - should be 72/1
	num=0;
	den=0;
	localTestResult=metaData->GetRationalParam(0x011B, 1, num, den);
	if ((aDataType == ESetNoThumbnail) && (localTestResult != KErrNotFound))
		{
		Printf(_L("Found unexpected IFD1 YResolution tag 0x011B"));
		testFailed = ETrue;
		}
	else if ((aDataType == ESetThumbnail) && ((num != 72) || (den != 1)))
		{
		Printf(_L("Error with IFD1 YResolution tag 0x011B (%d/%d)"), num, den);
		testFailed = ETrue;
		}

	// 0x0128 - Resolution Unit - should be 2
	shortData=0;
	localTestResult = metaData->GetShortParam(0x0128, 1, shortData);
	if ((aDataType == ESetNoThumbnail) && (localTestResult != KErrNotFound))
		{
		Printf(_L("Found unexpected IFD1 Resolution Unit tag 0x0128"));
		testFailed = ETrue;
		}
	else if ((aDataType == ESetThumbnail) && (shortData != 2))
		{
		Printf(_L("Error with IFD1 Resolution Unit tag 0x0128 (%d)"), shortData);
		testFailed = ETrue;
		}


	// 0x0201 - Offset to thumbnail SOI
	TInt longData=0;
	localTestResult = metaData->GetIntegerParam(0x0201, 1, longData);
	if ((aDataType == ESetNoThumbnail) && (localTestResult != KErrNotFound))
		{
		Printf(_L("Found unexpected IFD1 SOI offset tag 0x0201"));
		testFailed = ETrue;
		}
	else if ((aDataType == ESetThumbnail) && (longData == 0))
		{
		Printf(_L("Error with IFD1 SOI offset tag 0x0201"));
		testFailed = ETrue;
		}

	// 0x0202 - Number of bytes in thumbnail
	longData=0;
	localTestResult = metaData->GetIntegerParam(0x0202, 1, longData);
	if ((aDataType == ESetNoThumbnail) && (localTestResult != KErrNotFound))
		{
		Printf(_L("Found unexpected IFD1 tag 0x0202"));
		testFailed = ETrue;
		}
	else if ((aDataType == ESetThumbnail) && (longData == 0))
		{
		Printf(_L("Error with IFD1 tag 0x0202"));
		testFailed = ETrue;
		}

	if (testFailed)
		{
		TestFailed(KErrCorrupt);
		}
	//
	// Check the destination file for mandatory IFD1 tags.
	//

	CleanupStack::PopAndDestroy(decoder);
	}

// DEF084849: CScaledJpegGenerator does not add SOI marker to generated thumbnail
void TImage::VerifyTransformedThumbnailSOI(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Verify SOI marker in dest thumbnail ---"));

	TFileName sourceFileName(KImageTestThumblessExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	//
	//		encode the thumbnail
	CleanupStack::PushL(imageTransform);
	DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	//		encode the thumbnail
	//

	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(destFileName));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(decoder);

	//
	//	Check that the thumb in the destination file has an
	//	SOI marker.
	TInt testResult = KErrNone;
	MExifMetadata* metaData=decoder->ExifMetadata();

	// Get the offset to the thumbnail image (i.e. the value of tag 0x0201)
	TInt soiOffset=0;
	testResult=metaData->GetIntegerParam(0x0201, 1, soiOffset);
	CleanupStack::PopAndDestroy(decoder);

	if (testResult!=KErrNone)
		{
		// Reuse ProcessSetError to print an error message
		ProcessSetError(testResult, 0x0201);
		TestFailed(testResult);
		return;
		}

	// Open the dest file and read it into an HBufC
	RFile file;
	testResult=file.Open(iFs,KImageTestExifTransformDest,EFileShareReadersOnly|EFileStream|EFileRead);
	if (testResult!=KErrNone)
		{
		Printf(_L("Could not set open dest file"));
		TestFailed(testResult);
		return;
		}
	CleanupClosePushL(file);

	TInt size = 0;
	testResult=file.Size(size);
	if (testResult!=KErrNone)
		{
		Printf(_L("Could not get dest file size"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(); //file
		return;
		}

	HBufC8* buffer = HBufC8::NewMaxLC(size);
	TPtr8 bufferDes(buffer->Des());
	testResult=file.Read(bufferDes);
	if (testResult!=KErrNone)
		{
		Printf(_L("Could not read dest file"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2); //file, buffer
		return;
		}

	const TUint8 KOffsetToApp1Body = 12;
	const TUint16 KJpgSOISignature = 0xffd8;

	// Check that the offset to the thumb SOI is valid
	if (soiOffset+KOffsetToApp1Body+1 >= size)
		{
		Printf(_L("Offset to SOI out of range"));
		TestFailed(KErrCorrupt);
		CleanupStack::PopAndDestroy(2); //file, buffer
		return;
		}

	//	Get the bytes at the location where the SOI should be
	TUint16 soi = (bufferDes[soiOffset+KOffsetToApp1Body] << 8) | bufferDes[soiOffset+KOffsetToApp1Body+1];

	if (soi != KJpgSOISignature)
		{
		Printf(_L("Error: Dest thumbnail does not have SOI"));
		testResult=KErrCorrupt;
		}
	//	Check that the thumb in the destination file has an
	//	SOI marker.
	//

	if (testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	CleanupStack::PopAndDestroy(2); //file, buffer
	}

void TImage::TransformSetSize(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transform Thumbnail test(%d) --- %S"), aDataType, &KImageTestExifTransformSource);

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TBool addThumbnail=ETrue;
	switch(aDataType)
		{
		case ESetNoThumbnail:
			addThumbnail=EFalse;
			break;
		case ESetThumbnail:
			break;
		default:
			Printf(_L("Unexpected DataType %d"), aDataType);
			TestFailed(KErrArgument);
			return;
		}
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,addThumbnail));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	//
	//		encode the image
	CleanupStack::PushL(imageTransform);
	TInt transformResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	//		encode the image
	//

	//
	//		verify the size of the resulting encoded image
	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(TFileName(KImageTestExifTransformDest)));
	if(allocErr)
		{
		Printf(_L("could not create the decoder on the encoded file"));
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(decoder);
	TFrameInfo frameInfo= decoder->FrameInfo();
	if(frameInfo.iOverallSizeInPixels!=TSize(160, 120))
		{
		Printf(_L("the encoded image is not the size expected : %d, %d"), frameInfo.iOverallSizeInPixels.iWidth, frameInfo.iOverallSizeInPixels.iHeight);
		TestFailed(KErrCorrupt);
		}
	CleanupStack::PopAndDestroy(decoder);

	//		verify the size of the resulting encoded image
	//
	}


//DEF090653: Exif: Image comments incorrectly copied when generating Thumbnail
void TImage::TestThumbnailNoComments(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- DEF090653: Exif: Image comments incorrectly copied when generating Thumbnail ---"));

	TFileName sourceFileName(KImageTestThumbnailNoCommentsSource);
	TFileName destFileName(KImageTestThumbnailNoCommentsDest);


	CImageTransform* imageTransform=NULL;

	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName, ETrue, ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}

	//
	//		encode the image
	CleanupStack::PushL(imageTransform);
	TInt transformResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	//		encode the image
	//

	CJPEGExifDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(destFileName));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(decoder);

	//
	//	Get the thumbnail from the image
	TInt testResult1 = KErrNone;
	TInt testResult2 = KErrNone;
	MExifMetadata* metaData=decoder->ExifMetadata();

	// Get the offset to the thumbnail image (i.e. the value of tag 0x0201)
	TInt soiOffset=0;
	TInt thumbnaillength = 0;
	testResult1=metaData->GetIntegerParam(0x0201, 1, soiOffset);

	//Get the length of the thumbnail image	(i.e. the value of tag 0x0202)
	testResult2=metaData->GetIntegerParam(0x0202, 1, thumbnaillength);
	CleanupStack::PopAndDestroy(decoder);
	decoder = NULL;

	if (testResult1!=KErrNone)
		{
		// Reuse ProcessSetError to print an error message
		ProcessSetError(testResult1, 0x0201);
		TestFailed(testResult1);
		return;
		}

    if (testResult2!=KErrNone)
		{
		// Reuse ProcessSetError to print an error message
		ProcessSetError(testResult2, 0x0202);
		TestFailed(testResult2);
		return;
		}

	RFile file;
	TInt testResult = KErrNone;
	testResult=file.Open(iFs,destFileName,EFileShareReadersOnly|EFileStream|EFileRead);
	if (testResult!=KErrNone)
		{
		Printf(_L("Could not set open dest file"));
		TestFailed(testResult);
		return;
		}
	CleanupClosePushL(file);

	TInt size = 0;
	testResult=file.Size(size);
	if (testResult!=KErrNone)
		{
		Printf(_L("Could not get dest file size"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(); //file
		return;
		}

	HBufC8* buffer = HBufC8::NewMaxLC(size);
	TPtr8 bufferDes(buffer->Des());
	testResult=file.Read(bufferDes);
	if (testResult!=KErrNone)
		{
		Printf(_L("Could not read dest file"));
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(2); //file, buffer
		return;
		}

    const TInt KConstStartOffset = 12; //SOI+APP1+length+ExifHeaderLength = 12
	TPtrC8 thumbnailData = bufferDes.Mid(KConstStartOffset + soiOffset,thumbnaillength);

    RFile file2;
    TInt err = file2.Create(iFs, KImageGetThumbnail,EFileWrite);
    if (err != KErrNone)
    	{
    	err = file2.Replace(iFs, KImageGetThumbnail,EFileWrite);
    	if(err != KErrNone)
    		{
    		TestFailed(err);
	    	CleanupStack::PopAndDestroy(2); //file, buffer
		    return;
    		}
    	}

    file2.Write(thumbnailData);
    file2.Close();
    //	Get the thumbnail from the image
    //

  	CImageDecoder* thumbDecoder = CImageDecoder::FileNewL(iFs, KImageGetThumbnail);
    CleanupStack::PushL(thumbDecoder);

	// check for the comment block
	const CFrameImageData* frameImageData= &thumbDecoder->FrameData();
	TInt count = (frameImageData) ? frameImageData->ImageDataCount() : 0;
	for (TInt index = 0 ; index<count ; index++)
		{
		const TImageDataBlock& encoderData = *frameImageData->GetImageData(index);
		if (encoderData.DataType() == KJPGCommentUid)
			{
			// test case fails
			Printf(_L("Thumbnail should not have comments"));
			TestFailed(testResult);
		    CleanupStack::PopAndDestroy(3); //file, buffer,thumbDecoder
		    return;
			}
		}
	file.Close();
	Printf(_L("Test Pass"));
	CleanupStack::PopAndDestroy(3);
	}
//
//		Transform tests
//
//
//





//
//
//
//		ExifUtility tests
//
void TImage::TestExifUtility(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	// we use imagetransform to do all the tests: it allows to read and write
	// in the exif metadata without having to encode the file
	Printf(_L("\n"));
	Printf(_L("--- ExifUtility test --- %S"), &KImageTestExifTransformSource);

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue,ETrue));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData=extension->ExifMetadata();

	TInt testRes=DoTestExifUtility(metaData);
	if(testRes)
		{
		TestFailed(testRes);
		}

	CleanupStack::PopAndDestroy(imageTransform);
	}

TInt TImage::DoTestExifUtility(MExifMetadata* aMetaData)
	{
	TInt testRes=KErrNone;

	TBuf<KUtilityTestNameMaxLength> testName;
	TInt readTestRes=KErrNone;
	TInt writeTestRes=KErrNone;
	TExifWriterUtility exifWriteUtility(aMetaData);
	TExifReaderUtility exifReadUtility(aMetaData);

	_LIT8(K8BitBufferTestResultFormat, "buffer no %i");
	HBufC8* buf8ParamWrite=NULL;
	TInt intParam1Write=0;
	TInt intParam2Write=0;
	TUint16 shortParamWrite=0;


	HBufC8* buf8ParamRead=NULL;
	TInt intParam1Read=0;
	TUint uintParamRead=0;
	TInt intParam2Read=0;
	TUint16 shortParamRead=0;

	HBufC8* refBuf8Param=NULL;
	TInt refIntParam1=-1;
	TInt refIntParam2=-1;
	TUint16 refShortParam=0xffff;

	TInt tagID=0;
	TInt ifd=0;
	for(TInt i=0; i<EMaxUtilityIndex; i++)
		{
		TBuf8<32> tempBuf;
		tempBuf.Format(K8BitBufferTestResultFormat(), intParam1Write);
		buf8ParamWrite=tempBuf.Alloc();
		if(!buf8ParamWrite)
			{
			return KErrNoMemory;
			}
		buf8ParamRead=NULL;
		intParam1Read=0;
		uintParamRead=0;
		intParam2Read=0;
		shortParamRead=0;

		refBuf8Param=NULL;
		refIntParam1=0;
		refIntParam2=0;
		refShortParam=0;
		GetTagDesc(i, tagID, ifd);
		switch(i)
			{
			case EGetMake:
				testName=_L("Make");
				readTestRes=exifReadUtility.GetMake(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("SONY"));

				writeTestRes=exifWriteUtility.SetMake(buf8ParamWrite);
				readTestRes|=exifReadUtility.GetMake(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWrite, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);
				break;
			case EGetModel:
				testName=_L("Model");
				readTestRes=exifReadUtility.GetModel(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("CYBERSHOT"));

				writeTestRes=exifWriteUtility.SetModel(buf8ParamWrite);
				readTestRes|=exifReadUtility.GetModel(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWrite, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);
				break;
			case EGetDateTime:
				{
				testName=_L("DateTime");
				readTestRes=exifReadUtility.GetDateTime(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("2000:09:30 10:59:45"));

				_LIT8(KBuf8ParamDateTime, "3999:99:88 66:56:01");
				HBufC8*	buf8ParamDateTime=KBuf8ParamDateTime().Alloc();
				if (buf8ParamDateTime == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetDateTime(buf8ParamDateTime);
				readTestRes|=exifReadUtility.GetDateTime(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamDateTime, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamDateTime;
				}
				break;
			case EGetDateTimeOriginal:
				{
				testName=_L("DateTimeOriginal");
				readTestRes=exifReadUtility.GetDateTimeOriginal(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("2000:09:30 10:59:45"));

				_LIT8(KBuf8ParamDateTimeOriginal, "4666:99:88 66:56:27");
				HBufC8*	buf8ParamDateTimeOriginal=KBuf8ParamDateTimeOriginal().Alloc();
				if (buf8ParamDateTimeOriginal == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetDateTimeOriginal(buf8ParamDateTimeOriginal);
				readTestRes|=exifReadUtility.GetDateTimeOriginal(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamDateTimeOriginal, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamDateTimeOriginal;
				}
				break;
			case EGetDateTimeDigitized:
				{
				testName=_L("DateTimeDigitized");
				readTestRes=exifReadUtility.GetDateTimeDigitized(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("2000:09:30 10:59:45"));

				_LIT8(KBuf8ParamDateTimeDigitized, "2222:33:55 66:56:14");
				HBufC8*	buf8ParamDateTimeDigitized=KBuf8ParamDateTimeDigitized().Alloc();
				if (buf8ParamDateTimeDigitized == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetDateTimeDigitized(buf8ParamDateTimeDigitized);
				readTestRes|=exifReadUtility.GetDateTimeDigitized(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamDateTimeDigitized, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamDateTimeDigitized;
				}
				break;
			case EGetInteroperabilityIndex:
				{
				testName=_L("InteroperabilityIndex");
				readTestRes=exifReadUtility.GetInteroperabilityIndex(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("R98"));

				_LIT8(KBuf8ParamWriteInterOp, "THM");
				HBufC8*	buf8ParamWriteInterOp=KBuf8ParamWriteInterOp().Alloc();
				if (buf8ParamWriteInterOp == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetInteroperabilityIndex(buf8ParamWriteInterOp);
				readTestRes|=exifReadUtility.GetInteroperabilityIndex(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteInterOp, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteInterOp;
				}
				break;
			case EGetInteroperabilityVersion:
				{
				testName=_L("InteroperabilityVersion");
				readTestRes=exifReadUtility.GetInteroperabilityVersion(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("0100"));

				_LIT8(KBuf8ParamWriteInterOp, "0200");
				HBufC8*	buf8ParamWriteInterOp=KBuf8ParamWriteInterOp().Alloc();
				if (buf8ParamWriteInterOp == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetInteroperabilityVersion(buf8ParamWriteInterOp);
				readTestRes|=exifReadUtility.GetInteroperabilityVersion(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteInterOp, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteInterOp;
				}
				break;
			case EGetExifVersion:
				{
				testName=_L("ExifVersion");
				readTestRes=exifReadUtility.GetExifVersion(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("0210"));

				_LIT8(KBuf8ParamWriteVersion, "5713");
				HBufC8*	buf8ParamWriteVersion=KBuf8ParamWriteVersion().Alloc();
				if (buf8ParamWriteVersion == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetExifVersion(buf8ParamWriteVersion);
				readTestRes|=exifReadUtility.GetExifVersion(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteVersion, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteVersion;
				}
				break;
			case EGetComponentsConfiguration:
				{
				TUint8 compConfig[4] = { 1, 2, 3, 0 };
				testName=_L("ComponentsConfiguration");
				readTestRes=exifReadUtility.GetComponentsConfiguration(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, TPtrC8(compConfig, 4));

				_LIT8(KBuf8ParamWriteCompConfig, "\4\3\2\1");
				HBufC8*	buf8ParamWriteCompConfig=KBuf8ParamWriteCompConfig().Alloc();
				if (buf8ParamWriteCompConfig == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetComponentsConfiguration(buf8ParamWriteCompConfig);
				readTestRes|=exifReadUtility.GetComponentsConfiguration(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteCompConfig, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteCompConfig;
				}
				break;
			case EGetFlashPixVersion:
				{
				testName=_L("FlashPixVersion");
				readTestRes=exifReadUtility.GetFlashPixVersion(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("0100"));

				_LIT8(KBuf8ParamWriteFlashVer, "3855");
				HBufC8*	buf8ParamWriteFlashVer=KBuf8ParamWriteFlashVer().Alloc();
				if (buf8ParamWriteFlashVer == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetFlashPixVersion(buf8ParamWriteFlashVer);
				readTestRes|=exifReadUtility.GetFlashPixVersion(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteFlashVer, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteFlashVer;
				}
				break;
			case EGetXResolution:
				testName=_L("XResolution");
				readTestRes=exifReadUtility.GetXResolution(intParam1Read, intParam2Read);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, 72, 1);

				writeTestRes=exifWriteUtility.SetXResolution(intParam1Write, intParam2Write);
				readTestRes|=exifReadUtility.GetXResolution(intParam1Read, intParam2Read);
				aMetaData->GetRationalParam(tagID, ifd, refIntParam1, refIntParam2);
				writeTestRes|=CompareRationalValue(intParam1Write, intParam2Write, refIntParam1, refIntParam2);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, refIntParam1, refIntParam2);
				break;
			case EGetYResolution:
				testName=_L("YResolution");
				readTestRes=exifReadUtility.GetYResolution(intParam1Read, intParam2Read);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, 72, 1);

				writeTestRes=exifWriteUtility.SetYResolution(intParam1Write, intParam2Write);
				readTestRes|=exifReadUtility.GetYResolution(intParam1Read, intParam2Read);
				aMetaData->GetRationalParam(tagID, ifd, refIntParam1, refIntParam2);
				writeTestRes|=CompareRationalValue(intParam1Write, intParam2Write, refIntParam1, refIntParam2);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, refIntParam1, refIntParam2);
				break;
			case EGetThumbXResolution:
				testName=_L("ThumbXResolution");
				readTestRes=exifReadUtility.GetThumbXResolution(intParam1Read, intParam2Read);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, 72, 1);

				writeTestRes=exifWriteUtility.SetThumbXResolution(intParam1Write, intParam2Write);
				readTestRes|=exifReadUtility.GetThumbXResolution(intParam1Read, intParam2Read);
				aMetaData->GetRationalParam(tagID, ifd, refIntParam1, refIntParam2);
				writeTestRes|=CompareRationalValue(intParam1Write, intParam2Write, refIntParam1, refIntParam2);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, refIntParam1, refIntParam2);
				break;
			case EGetThumbYResolution:
				testName=_L("ThumbYResolution");
				readTestRes=exifReadUtility.GetThumbYResolution(intParam1Read, intParam2Read);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, 72, 1);

				writeTestRes=exifWriteUtility.SetThumbYResolution(intParam1Write, intParam2Write);
				readTestRes|=exifReadUtility.GetThumbYResolution(intParam1Read, intParam2Read);
				aMetaData->GetRationalParam(tagID, ifd, refIntParam1, refIntParam2);
				writeTestRes|=CompareRationalValue(intParam1Write, intParam2Write, refIntParam1, refIntParam2);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, refIntParam1, refIntParam2);
				break;
			case EGetResolutionUnit:
				testName=_L("ResolutionUnit");
				readTestRes=exifReadUtility.GetResolutionUnit(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 2);

				writeTestRes=exifWriteUtility.SetResolutionUnit(shortParamWrite);
				readTestRes|=exifReadUtility.GetResolutionUnit(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetYCbCrPositioning:
				testName=_L("YCbCrPositioning");
				readTestRes=exifReadUtility.GetYCbCrPositioning(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 2);

				writeTestRes=exifWriteUtility.SetYCbCrPositioning(shortParamWrite);
				readTestRes|=exifReadUtility.GetYCbCrPositioning(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetOrientation:
				testName=_L("Orientation");
				readTestRes=exifReadUtility.GetOrientation(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 1);

				writeTestRes=exifWriteUtility.SetOrientation(shortParamWrite);
				readTestRes|=exifReadUtility.GetOrientation(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetColorSpace:
				testName=_L("ColorSpace");
				readTestRes=exifReadUtility.GetColorSpace(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 1);

				writeTestRes=exifWriteUtility.SetColorSpace(shortParamWrite);
				readTestRes|=exifReadUtility.GetColorSpace(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetPixelXDimension:
				testName=_L("PixelXDimension");
				readTestRes=exifReadUtility.GetPixelXDimension(intParam1Read);
				readTestRes|=CompareIntegerValue(intParam1Read, 640);

				writeTestRes=exifWriteUtility.SetPixelXDimension(intParam1Write);
				readTestRes|=exifReadUtility.GetPixelXDimension(intParam1Read);
				aMetaData->GetIntegerParam(tagID, ifd, refIntParam1);
				writeTestRes|=CompareIntegerValue(intParam1Write, refIntParam1);
				readTestRes|=CompareIntegerValue(intParam1Read, refIntParam1);
				break;
			case EGetPixelYDimension:
				testName=_L("PixelYDimension");
				readTestRes=exifReadUtility.GetPixelYDimension(intParam1Read);
				readTestRes|=CompareIntegerValue(intParam1Read, 480);

				writeTestRes=exifWriteUtility.SetPixelYDimension(intParam1Write);
				readTestRes|=exifReadUtility.GetPixelYDimension(intParam1Read);
				aMetaData->GetIntegerParam(tagID, ifd, refIntParam1);
				writeTestRes|=CompareIntegerValue(intParam1Write, refIntParam1);
				readTestRes=CompareIntegerValue(intParam1Read, refIntParam1);
				break;
			case EGetThumbCompression:
				testName=_L("ThumbCompression");
				readTestRes=exifReadUtility.GetThumbCompression(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 6);

				writeTestRes=exifWriteUtility.SetThumbCompression(shortParamWrite);
				readTestRes|=exifReadUtility.GetThumbCompression(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetThumbResolutionUnit:
				testName=_L("ThumbResolutionUnit");
				readTestRes=exifReadUtility.GetThumbResolutionUnit(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 2);

				writeTestRes=exifWriteUtility.SetThumbResolutionUnit(shortParamWrite);
				readTestRes|=exifReadUtility.GetThumbResolutionUnit(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetThumbOrientation:
				testName=_L("ThumbOrientation");
				readTestRes=exifReadUtility.GetThumbOrientation(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 1);

				writeTestRes=exifWriteUtility.SetThumbOrientation(shortParamWrite);
				readTestRes|=exifReadUtility.GetThumbOrientation(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				break;
			case EGetImageDescription:
				{
				testName=_L("ImageDescription");
				readTestRes=exifReadUtility.GetImageDescription(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("Girl in orange top"));

				_LIT8(KBuf8ParamWriteVersion, "Gloomy day");
				HBufC8*	buf8ParamWriteVersion=KBuf8ParamWriteVersion().Alloc();
				if (buf8ParamWriteVersion == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetImageDescription(buf8ParamWriteVersion);
				readTestRes|=exifReadUtility.GetImageDescription(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteVersion, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteVersion;
				}
				break;
			case EGetCopyright:
				{
				testName=_L("Copyright");
				readTestRes=exifReadUtility.GetCopyright(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("The person who took the photo"));

				_LIT8(KBuf8ParamWriteCopyright, "Michael Jackson");
				HBufC8*	buf8ParamWriteCopyright=KBuf8ParamWriteCopyright().Alloc();
				if (buf8ParamWriteCopyright == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetCopyright(buf8ParamWriteCopyright);
				readTestRes|=exifReadUtility.GetCopyright(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteCopyright, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteCopyright;
				}
				break;
			case EGetMakerNote:
				{
				testName=_L("MakerNote");
				readTestRes=exifReadUtility.GetMakerNote(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, _L8("This is a maker note"));

				_LIT8(KBuf8ParamWriteMakerNote, "Please note that this is not a maker note");
				HBufC8*	buf8ParamWriteMakerNote=KBuf8ParamWriteMakerNote().Alloc();
				if (buf8ParamWriteMakerNote == NULL)
					return KErrNoMemory;

				writeTestRes=exifWriteUtility.SetMakerNote(buf8ParamWriteMakerNote);
				readTestRes|=exifReadUtility.GetMakerNote(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteMakerNote, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteMakerNote;
				}
				break;
			case EGetUserComment:
				{
				testName=_L("UserComment");
				_LIT8(KBuf8ParamUserComment, "UNICODE\0I\0 \0l\0i\0k\0e\0 \0i\0t\0");

				readTestRes=exifReadUtility.GetUserComment(buf8ParamRead);
				readTestRes|=CompareDescValue(buf8ParamRead, KBuf8ParamUserComment());

				HBufC8* buf8ParamWriteUserComment=NULL;
				TRAPD(err, buf8ParamWriteUserComment=CreateAsciiPrefixedBufferL(_L8("It's rubbish")));
				if (err!=KErrNone)
					{
					//No Memory
					return err;
					}
				writeTestRes=exifWriteUtility.SetUserComment(buf8ParamWriteUserComment);
				readTestRes|=exifReadUtility.GetUserComment(buf8ParamRead);
				aMetaData->GetParam8(tagID, ifd, refBuf8Param);
				writeTestRes|=CompareDescValue(buf8ParamWriteUserComment, refBuf8Param);
				readTestRes|=CompareDescValue(buf8ParamRead, refBuf8Param);

				delete buf8ParamWriteUserComment;
				}
				break;
			case EGetFlash:
				{
				testName=_L("Flash");
				readTestRes=exifReadUtility.GetFlash(shortParamRead);
				readTestRes|=CompareShortValue(shortParamRead, 5);

				writeTestRes=exifWriteUtility.SetFlash(shortParamWrite);
				readTestRes|=exifReadUtility.GetFlash(shortParamRead);
				aMetaData->GetShortParam(tagID, ifd, refShortParam);
				writeTestRes|=CompareShortValue(shortParamWrite, refShortParam);
				readTestRes|=CompareShortValue(shortParamRead, refShortParam);
				}
				break;
			case EGetFocalLength:
				{
				testName=_L("FocalLength");
				readTestRes=exifReadUtility.GetFocalLength(intParam1Read, intParam2Read);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, 216, 10);

				writeTestRes=exifWriteUtility.SetFocalLength(intParam1Write, intParam2Write);
				readTestRes|=exifReadUtility.GetFocalLength(intParam1Read, intParam2Read);
				aMetaData->GetRationalParam(tagID, ifd, refIntParam1, refIntParam2);
				writeTestRes|=CompareRationalValue(intParam1Write, intParam2Write, refIntParam1, refIntParam2);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, refIntParam1, refIntParam2);
				}
				break;
			case EGetShutterSpeedValue:
				{
				testName=_L("ShutterSpeedValue");
				readTestRes=exifReadUtility.GetShutterSpeedValue(intParam1Read, intParam2Read);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, 1, 125);

				writeTestRes=exifWriteUtility.SetShutterSpeedValue(intParam1Write, intParam2Write);
				readTestRes|=exifReadUtility.GetShutterSpeedValue(intParam1Read, intParam2Read);
				aMetaData->GetRationalParam(tagID, ifd, refIntParam1, refIntParam2);
				writeTestRes|=CompareRationalValue(intParam1Write, intParam2Write, refIntParam1, refIntParam2);
				readTestRes|=CompareRationalValue(intParam1Read, intParam2Read, refIntParam1, refIntParam2);
				}
				break;
			case EGetJpegInterchangeFormatLength:
				{
				testName=_L("JpegInterchangeFormatLength Int");
				readTestRes=exifReadUtility.GetJpegInterchangeFormatLength(intParam1Read);
				readTestRes|=CompareIntegerValue(intParam1Read, 2959);

				//Not writable
				writeTestRes=KErrNone;
				}
				break;
			case EGetJpegInterchangeFormatLengthUint:
				{
				testName=_L("JpegInterchangeFormatLength Uint");
				readTestRes=exifReadUtility.GetJpegInterchangeFormatLength(uintParamRead);
				readTestRes|=CompareUnsignedIntegerValue(uintParamRead, 2959);

				//Not writable
				writeTestRes=KErrNone;
				}
				break;
			}
		if(writeTestRes==KErrNone)
			{
			Printf(_L("write %S succeeded"), &testName);
			}
		else
			{
			Printf(_L("write %S Failed with error: %d"), &testName, writeTestRes);
			}

		if(readTestRes==KErrNone)
			{
			Printf(_L("read %S succeeded"), &testName);
			}
		else
			{
			Printf(_L("read %S Failed with error: %d"), &testName, readTestRes);
			}

		delete buf8ParamWrite;
		buf8ParamWrite=NULL;
		delete refBuf8Param;
		refBuf8Param=NULL;
		delete buf8ParamRead;
		buf8ParamRead=NULL;
		intParam1Write--;
		intParam2Write--;
		shortParamWrite--;

		testRes |= writeTestRes;
		testRes |= readTestRes;
		}
	return testRes;
	}

// DEF082361: Test that ExifJpegTransform plugin does not leak handles //
void TImage::TestExifJpegTransformHandleLeak(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	// This test involves creating and setting up a ExifJpegTransform plugin and
	// verifying that the number of open handles before its creation is the same
	// as the number open after its destruction.
	Printf(_L("\n"));
	Printf(_L("--- ExifJpegTransform handle leak test ---"));

	TInt initialHandlesInThread = 0;
	TInt initialHandlesInProcess = 0;
	RThread thisThread;

	// Count the number of open handles in this thread to start with
	thisThread.HandleCount(initialHandlesInProcess, initialHandlesInThread);

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

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
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	// Setup the preconditions to ensure that the call to SetupL()
	// below will succeed.
	TRAPD(testErr,
		{
		imageTransform->SetSourceFilenameL(sourceFileName);
		imageTransform->SetDestFilenameL(destFileName);
		imageTransform->SetDestSizeInPixelsL(TSize(160, 120), ETrue);
		imageTransform->SetOptionsL(CImageTransform::EThumbnail);
		});
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not Initialize imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy();	// imageTransform
		return;
		}

	// Setup the transformer - this should increase the open handles count
	TRAP(testErr, imageTransform->SetupL());
	if(testErr!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		TestFailed(testErr);
		CleanupStack::PopAndDestroy();	// imageTransform
		return;
		}

	// Destroy the transformer - this should close any open handles
	// owned by it.
	CleanupStack::PopAndDestroy(imageTransform);

	// Close any handles opened by ECom
	REComSession::FinalClose();

	TInt finalHandlesInThread = 0;
	TInt finalHandlesInProcess = 0;

	// Count the number of open handles in this thread now
	thisThread.HandleCount(finalHandlesInProcess, finalHandlesInThread);

	// The final number of open handles should equal the initial
	if (initialHandlesInThread != finalHandlesInThread)
		{
		Printf(_L("Handle leak detected"));
		TestFailed(KErrGeneral);
		return;
		}
	}

TInt TImage::CompareDescValue(TDesC8* aParam1, TPtrC8 aParam2)
	{
	return CompareDescValue(aParam1, &aParam2);
	}

TInt TImage::CompareDescValue(TDesC8* aParam1, TDesC8* aParam2)
	{
	if(!aParam1 || !aParam2)
		{
		return KErrGeneral;
		}
	if(aParam1->Compare(*aParam2))
		{
		return KErrGeneral;
		}
	return KErrNone;
	}
TInt TImage::CompareRationalValue(TInt aParam11, TInt aParam12, TInt aParam21, TInt aParam22)
	{
	if(	(aParam11 != aParam21)  &&  (aParam12 != aParam22))
		{
		return KErrGeneral;
		}
	return KErrNone;
	}
TInt TImage::CompareShortValue(TUint16 aParam1, TUint16 aParam2)
	{
	if(aParam1 != aParam2)
		{
		return KErrGeneral;
		}
	return KErrNone;
	}
TInt TImage::CompareIntegerValue(TInt aParam1, TInt aParam2)
	{
	if(aParam1 != aParam2)
		{
		return KErrGeneral;
		}
	return KErrNone;
	}
TInt TImage::CompareUnsignedIntegerValue(const TUint aParam1, const TUint aParam2) const
	{
	if(aParam1 != aParam2)
		{
		return KErrGeneral;
		}
	return KErrNone;
	}

void TImage::GetTagDesc(TInt aUtilityMethod, TInt& aTagID, TInt& aIfd)
	{
	for(TInt i=0; i<EMaxUtilityIndex; i++)
		{
		if(KUtilityTagIndexesList[i][0]==aUtilityMethod)
			{
			aTagID=KUtilityTagIndexesList[i][1];
			aIfd=KUtilityTagIndexesList[i][2];
			}
		}
	}
//
//		ExifUtility tests
//
//
//


//
//
//
//		Alloc tests
//
void TImage::AllocTest(const TExifDataType& aDataType, const TExifOperation& /*aOperation*/)
	{
	//Alloced version of :TransformExifSet stops before the transform stage
	TInt err;
	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse;
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	Printf(_L("\n"));
	Printf(_L("--- AllocTest Based on Transform Setters test(%d) --- %S"), aDataType, &KImageTestExifTransformSource);
	for (;;)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;
		CImageTransform* imageTransform = NULL;
		MExifMetadata* metaData = NULL;
		TRAP(err,AllocTestStage1(aDataType,imageTransform,metaData));
		completed = EFalse;
		if (err == KErrNone)
			{
			TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
			if (testAlloc==NULL)
				{
				reachedEnd = ETrue;
				failCount -= 1;
				}
			else
				User::Free(testAlloc);
			// see if valid result and break if wrong - might be premature result
			TInt result = TestSetData(aDataType, metaData, ESetData, ESetParam);
			if(result != EPass)
				{
				badResult = ETrue;
				TestFailed(result);
				}
			completed = reachedEnd || badResult;
			delete imageTransform;
			}
		else if (err != KErrNoMemory) // bad error code
			{
			__UHEAP_SETFAIL(RHeap::ENone, 0); // avoid problem in ERR_PRINTF
			completed = ETrue;
			Printf(_L("Bad error code %d"),err);
			// log the result
			TestFailed(err);
			}
		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);
		if (completed)
			break; // exit loop
		failCount += 1;
		}

	Printf(_L("Finish with %d allocation tests"),failCount);
	}

void TImage::AllocTestStage1(const TExifDataType& aDataType, CImageTransform*& aImageTransform, MExifMetadata*& aMetaData)
	{
	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	if (aDataType == EDefaultTagSet)
		sourceFileName = KImageTestTransformSource;

	aImageTransform=PrepareImageTransformL(sourceFileName,destFileName,ETrue);
	CleanupStack::PushL(aImageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(aImageTransform->Extension());
	aMetaData=extension->ExifMetadata();
	CleanupStack::Pop(aImageTransform);

	/* Note - there is still more alloc testing to write here to replicate
	:TransformExifSet - but it will have to be more clever as it involves
	active object callbacks */
	}

//
//		Alloc tests
//
//
//


//
//
// INC059847 Scalado CSemcImageEncoder/Decoder can not be casted to CJpegExifEncoder/Decode
//
//
void TImage::TestDerivedExifPluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	const MExifMetadata* const KTestPtrValue=reinterpret_cast<MExifMetadata*>(1);

	Printf(_L("\n"));
	Printf(_L("--- INC059847 Scalado CSemcImageEncoder/Decoder can not be casted to CJpegExifEncoder/Decode ---"));

// Decoder //
	CJPEGExifDecoder* decoder=NULL;
	TRAPD(error,
		decoder=static_cast<CJPEGExifDecoder*>( CImageDecoder::FileNewL(iFs, KImageTestExifDecode, DecoderOptions(), KNullUid, KNullUid, KExifDerivedDecoderUid));
		);
	if(error!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}
	MExifMetadata* metaData=decoder->ExifMetadata();
	delete decoder;
	if (KTestPtrValue!=metaData)
		{
		Printf(_L("returned ExifMetadata() is not equal to 1"));
		TestFailed(KErrGeneral);
		return;
		}
// Encoder //
	CJPEGExifEncoder* encoder=NULL;
	TRAP(error,
		encoder=static_cast<CJPEGExifEncoder*>( CImageEncoder::FileNewL(iFs, KImageTestExifEncode, CImageEncoder::EOptionNone, KNullUid, KNullUid, KExifDerivedEncoderUid));
		);
	if(error!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}
	metaData=encoder->ExifMetadata();
	delete encoder;
	if (KTestPtrValue!=metaData)
		{
		Printf(_L("returned ExifMetadata() is not equal to 1"));
		TestFailed(KErrGeneral);
		return;
		}
	}

//
//
// INC061409  NTT - JPEG decoder fails to decode images containing non-EXIF APP1 marker
//
//
void TImage::TestNonExifAPP1MarkerL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	Printf(_L("\n"));
	Printf(_L("--- INC061409  NTT - JPEG decoder fails to decode images containing non-EXIF APP1 marker ---"));

// Decoder //
	CImageDecoder* decoder= CImageDecoder::FileNewL(iFs, KImageTestNonExifApp1, DecoderOptions());
	CleanupStack::PushL(decoder);
	CFbsBitmap* target=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(target);
	if (!decoder->IsImageHeaderProcessingComplete())
		{
		User::Leave(KErrGeneral);
		}
	const TFrameInfo& frameInfo=decoder->FrameInfo(0);
	target->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode);

	TInt error=DoImageDecodeL(decoder, *target);
	CleanupStack::PopAndDestroy(2, decoder);

	if (error != KErrNone)
		{
		Printf(_L("failed to decode an image error=%d"),error);
		TestFailed(error);
		}
	}

//
//
//  DEF072039 ICLThread crashed (Cbase-46) in S60 3.0 wk36 emulator
//
//
void TImage::TestMultipleEncodeDecodesWithRandomCancelsL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	for(TInt i=0;i<KMaxHandlers;i++)
		{
		TFileName filename;
		filename.Format( _L("c:\\TSU_ICL_cod_04\\image%d.jpg"), i );
		if( i < KLoaders )
			{
			iImageLoader[i] = new ( ELeave ) CTestImageLoader( iFs, this, iSeed, i*200 );
			}
		else
			{
			iImageLoader[i] = new ( ELeave ) CTestImageSaver( iFs, this, iSeed, i*200 );
			}
		iImageLoader[i]->InitialiseL( filename );
		}
	for(TInt i=0;i<KMaxHandlers;i++)
		{
		iImageLoader[i]->StartL();
		}
	iListener = new (ELeave) CActiveListener();
	iListener->InitialiseActiveListener();
	CActiveScheduler::Start();
	for( TInt i=0;i<KMaxHandlers;i++ )
		{
		delete iImageLoader[i];
		}
	//Copy here the rest of the test code to print out test result
	}

TInt TImage::CountsChanged()
	{
	iLoaded=0;
	iCanceled=0;
	iStarted=0;
	for(TInt i=0;i<KMaxHandlers;i++)
		{
		iLoaded += iImageLoader[i]->iLoaded;
		iCanceled += iImageLoader[i]->iCanceled;
		iStarted += iImageLoader[i]->iStarted;
		}
	Printf(_L("Loader L %d C %d S %d \n"),iLoaded,iCanceled,iStarted);
	return iLoaded;
	}

void TImage::StopEncodeDecode()
	{
	TRequestStatus* status = &iListener->iStatus;
	User::RequestComplete(status,KErrNone);
	}

CTestImageHandler::CTestImageHandler( RFs& aFs, TImage* aUO, TInt64& aSeed )
: CActive( CActive::EPriorityIdle ),  iSeed( aSeed ), iFs( aFs )
    {
    CActiveScheduler::Add( this );
	iUO = aUO;
    }

CTestImageHandler::~CTestImageHandler()
    {
	delete iCanceler;
	delete iBitmap;
    }


CTestImageLoader::CTestImageLoader( RFs& aFs, TImage* aUO, TInt64& aSeed, TInt aId ) :
	 CTestImageHandler( aFs, aUO, aSeed )
    {
    iId = aId;
    }

CTestImageLoader::~CTestImageLoader()
    {
    if( IsActive() )
		{
		Cancel();
		}
	delete iDecoder;
    }


void CTestImageLoader::InitialiseL( const TDesC& aFileName )
    {
	iFileName=aFileName;
	iBitmap = new( ELeave ) CFbsBitmap();
    User::LeaveIfError( iBitmap->Create( TSize( 1152, 864 ), EColor16M ) );
	iCanceler = new( ELeave ) CTestRandomCancel( this, 100, iSeed );
    }


void CTestImageLoader::StartL()
	{
	iId++;
	if( iDecoder )
		{
        delete iDecoder;
		iDecoder = NULL;
		}
	iUO->Printf( _L("decoder Init %S.."), &iFileName );
	TRAPD( err, iDecoder = CImageDecoder::FileNewL( iFs, iFileName, CImageDecoder::EOptionAlwaysThread ) );//, CImageDecoder::EOptionAlwaysThread );
	if( err != KErrNone )
	    {
	    iUO->Printf( _L( "Decoder not created" ) );
	    }
	iUO->Printf( _L("loading..., decoder=%x, %d"), iDecoder, iId );
	iDecoder->Convert( &iStatus, *iBitmap );
	iStarted++;
	if( !IsActive() )
		{
		SetActive();
		}
	iCanceler->StartL();
	}


void CTestImageLoader::DoCancel()
    {
    iUO->Printf( _L( "Dec Canceled" ) );
	iDecoder->Cancel();
	iCanceled++;
    }

void CTestImageLoader::RunL()
    {
    TInt err = iStatus.Int();
	if( err!=KErrNone )
		{
		User::Panic( _L("loader"), err );
		}

	iLoaded++;
	TInt totalLoaded = iUO->CountsChanged();
	if (totalLoaded > 10)
		{
		delete iDecoder;
		iDecoder = NULL;
		iUO->StopEncodeDecode();
		return;
		}
	delete iDecoder;
	iDecoder = NULL;
	StartL(); // start loading again
    }


CTestImageSaver::CTestImageSaver( RFs& aFs, TImage* aUO, TInt64& aSeed, TInt aId ) :
	 CTestImageHandler( aFs, aUO, aSeed )
    {
    iId = aId;
    }

CTestImageSaver::~CTestImageSaver()
    {
    if( IsActive() )
		{
		Cancel();
		}
  	delete iEncoder;
    }


void CTestImageSaver::InitialiseL( const TDesC& aFileName )
    {
	iFileName=aFileName;
	iBitmap = new( ELeave ) CFbsBitmap();
    User::LeaveIfError( iBitmap->Create( TSize( 1152/2, 864/2 ), EColor16M ) );
  	iCanceler = new( ELeave ) CTestRandomCancel( this, 3000, iSeed );
    }


void CTestImageSaver::StartL()
	{
	iId++;
	if( iEncoder )
		{
	    delete iEncoder;
		iEncoder = NULL;
		}
	iUO->Printf( _L("encoder init %S.."), &iFileName );
	iEncoder = CImageEncoder::FileNewL( iFs, iFileName, CImageEncoder::EOptionAlwaysThread,
		KImageTypeJPGUid );
	iUO->Printf( _L("saving started, encoder=%x, %d"), iEncoder, iId );
	iEncoder->Convert( &iStatus, *iBitmap );
	iStarted++;
	if( !IsActive() )
		{
		SetActive();
		}

	iCanceler->StartL();
	}


void CTestImageSaver::DoCancel()
    {
    iUO->Printf( _L( "Enc Canceled" ) );
	iEncoder->Cancel();
	iCanceled++;

    }

void CTestImageSaver::RunL()
    {
    TInt err = iStatus.Int();

	// No errors should happen
	if( err!=KErrNone )
		{
		User::Panic( _L("loader"), err );
		}

	iLoaded++;
	TInt totalLoaded = iUO->CountsChanged();
	if (totalLoaded > 10)
		{
		delete iEncoder;
		iEncoder = NULL;
		iUO->StopEncodeDecode();
		return;
		}
	delete iEncoder;
	iEncoder = NULL;
	StartL(); // start loading again
    }

CTestRandomCancel::CTestRandomCancel( CTestImageHandler* aImageLoader, TInt aAfter, TInt64& aSeed )
: CTimer( EPriorityHigh ), iSeed( aSeed )
    {
    CActiveScheduler::Add( this );
    iImageLoader = aImageLoader;
    iAfter = aAfter;

	ConstructL();
    }

CTestRandomCancel::~CTestRandomCancel()
    {
    Cancel();
    }

void CTestRandomCancel::StartL()
    {
	if( !IsActive() )
		{
		if( iAfter < KMaxDelay/2 )
			{
			After( iAfter * 100 );
			}
		else
			{
			After( ( iAfter - KMaxDelay/2 ) * 1000 );
			}

		iAfter = Math::Rand( iSeed ) / ( KMaxTInt / KMaxDelay );
		}
	}
void CTestRandomCancel::RunL()
    {
    if ( iImageLoader->IsActive() )
        {
        iImageLoader->Cancel();
	    }
	else
		{
		// start again
		iImageLoader->StartL();
	    }
	StartL(); // restart the timer
    }

void CTestRandomCancel::DoCancel()
    {
    }

//
//
// Jpeg ImageFrame Encoder/Decode
//
//

/*
 * CR1051 Positive tests for the ImageFrame Plugin using Chunks:
 * For each of the supported formats by the reference jpg plugin, decode an image from a jpg file to a
 * Chunk wrapped by an ImageFrame and feed the image to the encoder to be encoded back to a jpg file,
 * Supported formats are YUV420,YUV422 and mono
 */
void TImage::TestJpgChunkImageFrameEncodePerfL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- CR1051 JPEG RChunk ImageFrame plugin ---"));
	TInt j;

	for (j = 16; j < 19; j++)
		{
		__MM_HEAP_MARK;
		TFileName refFileName = TFileName(TestFileName(j));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination RChunk
		RChunk chunk;
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			return;
			}

		TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
		User::LeaveIfError(res);

	  	CleanupClosePushL(chunk);

		// create an empty imageframe
  		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();

		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j));
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);
		delete decoder;

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestFileName(j-11));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EPreferFastEncode, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		// get start time and initiate encoding
		TUint beginDecode=User::FastCounter();			
		TTime start;
		start.HomeTime();

		encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame);
		CActiveScheduler::Start();

		// get end time 
		TTime end;
		end.HomeTime();	
		TUint endDecode=User::FastCounter();

		const TInt err2 = encodeListener->iStatus.Int();
		if (err2 != KErrNone)
			{
			Printf(_L("failed to encode image (%S)"), &TestFileName(j));
			TestFailed(err2);
			}
		else
			{
			VerifyAgainstReferenceFile(destFileName, TFileName(TestFileName(j+3)));
			}

		const TInt64 microSecDecodeTime = end.Int64() - start.Int64();
		const TUint decodeCounter = endDecode - beginDecode;
		
		Printf(_L("--Image encoded in %d microseconds %d counters"), I64INT(microSecDecodeTime), decodeCounter );

		CleanupStack::PopAndDestroy(3, &chunk);

		delete encoder;
		__MM_HEAP_MARKEND;
		}
	}



//
//
// Jpeg ImageFrame Encoder/Decode
//
//

/*
 * Positive tests for the ImageFrame Plugin using Chunks:
 * For each of the supported formats by the reference jpg plugin, decode an image from a jpg file to a
 * Chunk wrapped by an ImageFrame and feed the image to the encoder to be encoded back to a jpg file,
 * Supported formats are YUV420,YUV422 and mono
 */
void TImage::TestJpgChunkImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 JPEG RChunk ImageFrame plugin ---"));
	TInt j;

	for (j = 1; j < 4; j++)
		{
		__MM_HEAP_MARK;
		TFileName refFileName = TFileName(TestFileName(j));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, iDecoderUid));//KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination RChunk
		RChunk chunk;
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			return;
			}

		TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
		User::LeaveIfError(res);

	  	CleanupClosePushL(chunk);

		// create an empty imageframe
  		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();

		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j));
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);
		delete decoder;

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err2 = encodeListener->iStatus.Int();
		if (err2 != KErrNone)
			{
			Printf(_L("failed to encode image (%S)"), &TestFileName(j));
			TestFailed(err2);
			}
		else
			{
			VerifyAgainstReferenceFile(destFileName, TFileName(TestFileName(j+12)));
			}

		CleanupStack::PopAndDestroy(3, &chunk);

		delete encoder;
		__MM_HEAP_MARKEND;
		}
	}

/*
 * Positive tests for the ImageFrame Plugin using Descriptors:
 * For each of the supported formats by the reference jpg plugin, decode an image from a jpg file to a
 * Descriptor wrapped by an ImageFrame and feed the image to the encoder to be encoded back to a jpg file.
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 JPEG Desc ImageFrame plugin ---"));
	TInt j;

	for (j = 1; j < 4; j++)
		{
		__MM_HEAP_MARK;
		TFileName refFileName = TFileName(TestFileName(j));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination Desc
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			delete decoder;
			return;
			}

		RDebug::Printf("RecommendedBufferSize: %d", imageSizeInBytes);
		HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
		CleanupStack::PushL(buffer);
	  	TPtr8 ptr = buffer->Des();

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame =	CImageFrame::NewL(ptr, imageSizeInBytes));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j));			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);
		delete decoder;

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestFileName(j+8));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		// get start time and initiate encoding
		TUint beginDecode=User::FastCounter();			
		TTime start;
		start.HomeTime();

		encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame);
		CActiveScheduler::Start();
		
		// get end time 
		TTime end;
		end.HomeTime();	
		TUint endDecode=User::FastCounter();

		const TInt err2 = encodeListener->iStatus.Int();
		if (err2 != KErrNone)
			{
			Printf(_L("failed to encode image (%S)"), &TestFileName(j));
			TestFailed(err2);
			}
		else
			{
			VerifyAgainstReferenceFile(destFileName, TFileName(TestFileName(j+12)));
			}

		const TInt64 microSecDecodeTime = end.Int64() - start.Int64();
		const TUint decodeCounter = endDecode - beginDecode;
		
		Printf(_L("--Image encoded in %d microseconds %d counters"), I64INT(microSecDecodeTime), decodeCounter );
		
		CleanupStack::PopAndDestroy(3, buffer);

		delete encoder;

		__MM_HEAP_MARKEND;
		}
	}

//
//
// Jpeg ImageFrame Stream Encoder/Decode
//
//

#ifndef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
void TImage::TestJpgImageFrameBlockStreamerInteroperabilityL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{	
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
	
void TImage::TestJpgImageFrameBlockStreamerSetupL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}

void TImage::TestJpgDescStreamImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
	
void TImage::TestJpgDescStreamUnderFlowImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}

void TImage::TestJpgDescStreamOverFlowImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
	
void TImage::TestJpgDescSequentialStreamDecodeEncodeImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
	
void TImage::TestJpgDescSeqStreamDecEncBlockwiseImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
	
void TImage::TestJpgDescRandomStreamDecodeEncodeImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}

void TImage::TestJpgDescRandomBackStreamDecEncImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
	
void TImage::TestJpgDescRandomFwdBackStreamDecEncImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}

void TImage::TestJpgDescStreamNegativeTestsL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}

void TImage::TestStreamEncodeAllocL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}

void TImage::TestStreamEncodeDecodeAllocL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("--- PREQ1630 JPEG Stream tests not run in this configuration ---"));
	return;
	}
#else

const TInt KSampSchemeCount = 4;

/*
 * Interoperability tests:
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgImageFrameBlockStreamerInteroperabilityL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Streaming & Transformation Interoperability ImageFrame tests---"));

	TFileName destFileName = TFileName(TestStreamFileName(4));

	CJPEGImageFrameEncoder* encoder = NULL;
	TRAPD(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}

	TImageConvStreamedEncode* streamEncode = NULL;
	TRAP(error,
		streamEncode = encoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize block streamer"));
		TestFailed(error);
		return;
		}

	TUid optimalFormat = KUidFormatYUV420Planar;
	TInt frameNumber = 0;
	TSize frameSizeInPixels(96,96);
	TSize blockSizeInPixels(16,16);
	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

	CFrameImageData* frameImageData = CFrameImageData::NewL();

	TJpegImageData data;

	data.iSampleScheme = TJpegImageData::EColor420;
	data.iQualityFactor = 95;

	frameImageData->AppendImageData(&data);

	TRAP(error,
	streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
		);

	if (error != KErrNone)
		{
		Printf(_L("Could not initialize encoder for streaming"));
		TestFailed(error);
		return;
		}

	TImageConvOperation* operation = NULL;

	TRAP(error,
		operation = encoder->OperationL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize operations extension"));
		TestFailed(error);
		return;
		}

	Printf(_L("OperationL() succeeded."));

	TImageConvOperation::TOperation capsOperation;

	capsOperation = static_cast<TImageConvOperation::TOperation> (TImageConvOperation::ERotation90DegreesClockwise);
	if(operation->Capabilities() & capsOperation == capsOperation)
		{
		Printf(_L("Capabilities() returned correct result."));
		}
	else
		{
		Printf(_L("Capabilities() returned in-correct result."));
		return;
		}

	capsOperation = static_cast<TImageConvOperation::TOperation> (TImageConvOperation::EMirrorHorizontalAxis | TImageConvOperation::EMirrorVerticalAxis);
	if(operation->Capabilities() & capsOperation == capsOperation)
		{
		Printf(_L("Capabilities() returned correct result."));
		}
	else
		{
		Printf(_L("Capabilities() returned in-correct result."));
		return;
		}

	capsOperation = static_cast<TImageConvOperation::TOperation> (0x0F);
	if(operation->Capabilities() != capsOperation)
		{
		Printf(_L("Capabilities() returned correct result.-negative test"));
		}
	else
		{
		Printf(_L("Capabilities() returned in-correct result."));
		return;
		}

	TRAP(error,
	operation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
		);
	if (error != KErrNone)
		{
		Printf(_L("Interoperability Test Passed"));
		}
	else
		{
		Printf(_L("Interoperability Test Failed"));
		TestFailed(error);
		return;
		}

	delete encoder;
	}

/*
 * Block Streaming set up tests:
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgImageFrameBlockStreamerSetupL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Stream ImageFrame Set up tests---"));

	TFileName destFileName = TFileName(TestStreamFileName(4));

	//check extensions//
	TUid jpgImageEncoderImplementationUidValue;
	jpgImageEncoderImplementationUidValue.iUid = KJPGEncoderImplementationUidValue;
	CImageEncoder* jpegEncoder = NULL;
	TRAPD(error,
		jpegEncoder = CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KImageTypeJPGUid, KNullUid, jpgImageEncoderImplementationUidValue);
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create jpeg Encoder"));
		TestFailed(error);
		return;
		}
	CleanupStack::PushL(jpegEncoder);

	// Extracting plugin proeprties
	RUidDataArray propertiesArray;
	CleanupClosePushL(propertiesArray);

	Printf(_L("------- GetPluginProperties -------"));
	CImageEncoder::GetPluginPropertiesL(jpgImageEncoderImplementationUidValue, propertiesArray);
	if (propertiesArray.Count() == 0)
		{
		Printf(_L("Plugin properties count is 0"));
		User::Leave(KErrNotFound);
		}

	TUid iclExtensionUidValue = {0x102833D8};
	TUid optionRet;
	CImageEncoder::TOptions options;

	Printf(_L("Plugin properties count is greater than 0"));
	Printf(_L("Check for extension interface supoport"));
	for (TInt i = 0; i < propertiesArray.Count(); i++)
		{
		if(propertiesArray[i] == iclExtensionUidValue)
			{
			optionRet.iUid = propertiesArray[i+1].iUid;
			options = static_cast<CImageEncoder::TOptions> (CImageEncoder::EOptionExtRotation | CImageEncoder::EOptionExtStream);
			if((optionRet.iUid & options) == options)
				{
				Printf(_L("Extension interface is supported by the Codec-error"));
				TestFailed(error);
				return;
				}
			else
				{
				Printf(_L("Negative test Extension interface CImageEncoder::EOptionExtRotation | CImageEncoder::EOptionExtStream is not supported by the Codec"));
				}
			}
		}
	CleanupStack::PopAndDestroy();	//propertiesArray
	CleanupStack::PopAndDestroy(jpegEncoder);
	//

	CJPEGImageFrameEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}

	TImageConvStreamedEncode* streamEncode = NULL;
	TRAP(error,
		streamEncode = encoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize block streamer"));
		TestFailed(error);
		return;
		}

	RArray<TUid> formatsCheck;
	CleanupClosePushL(formatsCheck);
	formatsCheck.AppendL(KUidFormatYUVMonochrome);
	formatsCheck.AppendL(KUidFormatYUV422Interleaved);
	formatsCheck.AppendL(KUidFormatYUV420Planar);
	formatsCheck.AppendL(KUidFormatYUV420PlanarReversed);

	TUid optimalFormatCheck = KUidFormatYUV420Planar;

	RArray<TUid> formats;
	TUid optimalFormat;

	CleanupClosePushL(formats);
	streamEncode->GetSupportedFormatsL(formats, optimalFormat);
	
	if(optimalFormat != optimalFormatCheck)
		{
		Printf(_L("GetSupportedFormatsL() failed (aOptimalFormat incorrect)."));
		TestFailed(KErrNotFound);
		}

	if(formats.Count() == formatsCheck.Count())
		{
		for(TInt i = 0; i < formats.Count(); i++)
			{
			// Check all elements match
			if(formats[i] != formatsCheck[i])
				{
				Printf(_L("GetSupportedFormatsL() failed."));
				TestFailed(KErrNotFound);
				}
			}
		Printf(_L("GetSupportedFormatsL() succeeded."));
		}
	else
		{
		Printf(_L("GetSupportedFormatsL() failed (aFormats returned unexpected length)."));
		TestFailed(KErrNotFound);
		}

	CleanupStack::PopAndDestroy();	// formats
	CleanupStack::PopAndDestroy();	// formatsCheck

	TInt maxBlocksPerRequest = KMaxTInt;
	TSize minBlockSizeInPixels(16,16);
	TInt optimalBlocksPerRequest = 256;
	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

	TEncodeStreamCaps streamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
								optimalBlocksPerRequest, navigation);

	TEncodeStreamCaps caps;
	streamEncode->GetCapabilities(optimalFormat, caps);

	// Compare values
	if(caps.MaxBlocksPerRequest() == maxBlocksPerRequest
		&& caps.MinBlockSizeInPixels() == minBlockSizeInPixels
		&& caps.OptimalBlocksPerRequest() == optimalBlocksPerRequest
		&& caps.Navigation() == navigation
		&& caps.Size() == streamCaps.Size()
		&& caps.Version() == streamCaps.Version())
		{
		Printf(_L("GetCapabilities() succeeded in setting values."));
		}
	else
		{
		Printf(_L("GetCapabilities() failed."));
		TestFailed(KErrNotFound);
		}
	
	TInt frameNumber = 0;

	TSize frameSizeInPixels(96,96);
	TSize blockSizeInPixels(16,16);

	CFrameImageData* frameImageData = CFrameImageData::NewL();

	TJpegImageData data;

	data.iSampleScheme = TJpegImageData::EColor420;
	data.iQualityFactor = 95;

	frameImageData->AppendImageData(&data);

	TRAP(error,
	streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
		);

	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}

	delete encoder;
	
	//
	// Decode to an image frame //
	//
	TFileName refFileName = TFileName(TestStreamFileName(KStream420ImgIdx));
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAP(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}
	CleanupStack::PushL(decoder);

	TImageConvStreamedDecode* streamDecode = NULL;
	TRAP(error,
		streamDecode = decoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize block streamer"));
		TestFailed(error);
		CleanupStack::PopAndDestroy(decoder);
		return;
		}
	
	TDecodeStreamCaps::TNavigation nav = static_cast<TDecodeStreamCaps::TNavigation> (TDecodeStreamCaps::ENavigationSequentialForward | TDecodeStreamCaps::ENavigationRandomForward | TDecodeStreamCaps::ENavigationRandomBackwards);
	
	TDecodeStreamCaps decodeStreamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
								optimalBlocksPerRequest, 1, nav);

	TDecodeStreamCaps decodeCaps;
	streamDecode->GetCapabilities(optimalFormat, 0, decodeCaps);

	// Compare values
	if(decodeCaps.MaxBlocksPerRequest() == maxBlocksPerRequest
		&& decodeCaps.MinBlockSizeInPixels() == minBlockSizeInPixels
		&& decodeCaps.OptimalBlocksPerRequest() == optimalBlocksPerRequest
		&& decodeCaps.Navigation() == nav
		&& decodeCaps.StreamSizeInBlocks() == 1
		&& decodeCaps.Size() == decodeStreamCaps.Size()
		&& decodeCaps.Version() == decodeStreamCaps.Version())
		{
		Printf(_L("GetCapabilities() succeeded in setting values."));
		}
	else
		{
		Printf(_L("GetCapabilities() failed."));
		TestFailed(KErrNotFound);
		}

	TImageConvScaler* imageScaler = NULL;
	TRAP(error,
		imageScaler = decoder->ScalerL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize block streamer"));
		TestFailed(error);
		CleanupStack::PopAndDestroy(decoder);
		return;
		}
	
	TInt maxUpscaleLimit = 1;
	TInt maxDownscaleLimit = -4;
	TScalerCaps scalerCaps(maxUpscaleLimit, maxDownscaleLimit, ETrue);

	TScalerCaps decodeScalerCaps;
	imageScaler->GetCapabilities(decodeScalerCaps);

	// Compare values
	if(decodeScalerCaps.PowerOfTwoScalingOnly()
		&& decodeScalerCaps.Size() == scalerCaps.Size()
		&& decodeScalerCaps.Version() == scalerCaps.Version())
		{
		Printf(_L("GetCapabilities() succeeded in setting values."));
		}
	else
		{
		Printf(_L("GetCapabilities() failed."));
		TestFailed(KErrNotFound);
		}

		
	CleanupStack::PopAndDestroy(decoder);
	}

/*
 * Tests for the ImageFrame Plugin streaming:
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescStreamImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Streaming tests---"));

	TInt j;

	for (j = 1; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination Desc
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			return;
			}

		HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
		CleanupStack::PushL(buffer);
	  	TPtr8 ptr = buffer->Des();

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame =	CImageFrame::NewL(ptr, imageSizeInBytes));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j));			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TInt frameNumber = 0;
		TFrameInfo frameInfo = decoder->FrameInfo();

		TSize frameSizeInPixels(160,160);
		TSize blockSizeInPixels;

		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

		CFrameImageData* frameImageData = CFrameImageData::NewL();

		TJpegImageData data;
		TInt numBlocksToAdd = 0;

		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		if(j == 1)
			{
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			numBlocksToAdd = 40;
			}
		else if(j == 2)
			{
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			numBlocksToAdd = 20;
			}
		else if(j == 3)
			{
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			numBlocksToAdd = 80;
			}

		data.iQualityFactor = 95;
		frameImageData->AppendImageData(&data);

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}



		for(TInt i =0; i<5; i++)
			{
			encodeListener->InitialiseActiveListener(encoder);
			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToAdd);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				Printf(_L("failed to append image (%S)"), &TestFileName(j));
				TestFailed(err2);
				break;
				}
			}

		encodeListener->InitialiseActiveListener(encoder);
		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}

		CleanupStack::PopAndDestroy(3);

		delete decoder;
		delete encoder;

		}
	}

/*
 * Tests for the ImageFrame Plugin streaming under flow:
 * Supported formats are YUV420,YUV422 and mono.
 */

void TImage::TestJpgDescStreamUnderFlowImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Streaming UnderFlow tests---"));

	TInt j;

	for (j = 1; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination Desc
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			return;
			}

		HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
		CleanupStack::PushL(buffer);
	  	TPtr8 ptr = buffer->Des();

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame =	CImageFrame::NewL(ptr, imageSizeInBytes));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j));			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TInt frameNumber = 0;
		TFrameInfo frameInfo = decoder->FrameInfo();

		TSize frameSizeInPixels(160,160);
		TSize blockSizeInPixels;

		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

		CFrameImageData* frameImageData = CFrameImageData::NewL();

		TJpegImageData data;

		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		if(j == 1)
			{
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			}
		else if(j == 2)
			{
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			}
		else if(j == 3)
			{
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			}

		data.iQualityFactor = 95;
		frameImageData->AppendImageData(&data);

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, NULL);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		TInt numBlocksToAdd = 99;

		for(TInt i =0; i<1; i++)
			{
			encodeListener->InitialiseActiveListener(encoder);
			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToAdd);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				Printf(_L("failed to append image (%S)"), &TestFileName(j));
				TestFailed(err2);
				break;
				}
			}

		encodeListener->InitialiseActiveListener(encoder);
		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrUnderflow)
			{
			Printf(_L("Undeflow test failed image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}

		CleanupStack::PopAndDestroy(3);

		delete decoder;
		delete encoder;

		//__MM_HEAP_MARKEND;
		}
	}

/*
 * Tests for the ImageFrame Plugin streaming over flow:
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescStreamOverFlowImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Streaming Over Flow tests---"));

	TInt j;

	for (j = 1; j < KSampSchemeCount; j++)
		{
		//__MM_HEAP_MARK;
		TFileName refFileName = TFileName(TestStreamFileName(j));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination Desc
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			delete decoder;
			return;
			}

		HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
		CleanupStack::PushL(buffer);
	  	TPtr8 ptr = buffer->Des();

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame =	CImageFrame::NewL(ptr, imageSizeInBytes));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j));			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TInt frameNumber = 0;

		TFrameInfo frameInfo = decoder->FrameInfo();

		TSize frameSizeInPixels(160,160);
		TSize blockSizeInPixels;

		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

		CFrameImageData* frameImageData = CFrameImageData::NewL();

		TJpegImageData data;

		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		if(j == 1)
			{
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			}
		else if(j == 2)
			{
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			}
		else if(j == 3)
			{
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			}

		data.iQualityFactor = 95;
		frameImageData->AppendImageData(&data);

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		TInt numBlocksToAdd = 200;

		for(TInt i =0; i<6; i++)
			{
			encodeListener->InitialiseActiveListener(encoder);
			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToAdd);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				if (err2 != KErrOverflow)
					{
					Printf(_L("Overflow test failed image (%S)"), &TestFileName(j));
					TestFailed(err2);
					break;
					}
				break;
				}
			}

		encodeListener->InitialiseActiveListener(encoder);
		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}

		CleanupStack::PopAndDestroy(3);

		delete decoder;
		delete encoder;

		//__MM_HEAP_MARKEND;
		}
	}

/*
 * Tests for the ImageFrame Plugin sequential streaming. This test sequentially stream deacodes a file to imageframe and then stream encodes the imageframe buffer.
 * Supported formats are YUV420,YUV422 and mono.
 */

void TImage::TestJpgDescSequentialStreamDecodeEncodeImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Sequential Stream Decoding and Encoding tests---"));

	TInt j;

	for (j = 1; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j+8));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}
		CleanupStack::PushL(decoder);

		TImageConvStreamedDecode* streamDecode = NULL;
		TRAP(error,
			streamDecode = decoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			delete decoder;
			return;
			}

		TFrameInfo frameInfo = decoder->FrameInfo();

		TUid format = KUidFormatYUV422Interleaved;
		TUid optimalFormat = KUidFormatYUV422Interleaved;
		TInt imageSizeInBytes = 0;
		TInt numBlocksToGet = 0;
		TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
		TSize blockSizeInPixels;
		TSize streamBlockSizeInPixels;
		TJpegImageData data;
		TDecodeStreamCaps caps;

		TInt frameNumber = 0;
		if(j == 1)
			{
			format = KUidFormatYUV422Interleaved;
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			streamDecode->GetCapabilities(format,0, caps);
			blockSizeInPixels = TSize(caps.MinBlockSizeInPixels());
			numBlocksToGet = (frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth) * (frameInfo.iOverallSizeInPixels.iHeight/blockSizeInPixels.iHeight) ;
			}
		else if(j == 2)
			{
			format = KUidFormatYUV420PlanarReversed;
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			streamDecode->GetCapabilities(format,0, caps);
			blockSizeInPixels = TSize(caps.MinBlockSizeInPixels());
			numBlocksToGet = (frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth) * (frameInfo.iOverallSizeInPixels.iHeight/blockSizeInPixels.iHeight);
			}
		else if(j == 3)
			{
			format = KUidFormatYUVMonochrome;
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			streamDecode->GetCapabilities(format,0, caps);
			blockSizeInPixels = TSize(caps.MinBlockSizeInPixels());
			numBlocksToGet = (frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth) * (frameInfo.iOverallSizeInPixels.iHeight/blockSizeInPixels.iHeight);
			}

		data.iQualityFactor = 95;

		imageSizeInBytes = streamDecode->GetBufferSize(format, streamBlockSizeInPixels, numBlocksToGet);

		HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
		CleanupStack::PushL(buffer);
	  	TPtr8 ptr = buffer->Des();

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);



		TInt frameNum = 0;
		TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationSequentialForward;

		TRAP(error,
			streamDecode->InitFrameL(format, frameNum, nav);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize decoder"));
			TestFailed(error);
			return;
			}

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);

		TInt numBlocksRead;
		TBool haveMoreBlocks;
		theImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

		decodelistener->InitialiseActiveListener(decoder);
		streamDecode->GetNextBlocks(decodelistener->iStatus, *theImageFrame, numBlocksToGet, numBlocksRead, haveMoreBlocks);
		CActiveScheduler::Start();
		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(j+8));
			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}
		if(numBlocksToGet != numBlocksRead)
			{
			Printf(_L("Number of blocks read is less than the number of blocks to get"));
			}
		if(haveMoreBlocks != EFalse)
			{
			Printf(_L("More blocks available to decode"));
			}
		CleanupStack::PopAndDestroy(1, decodelistener);

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}
		CleanupStack::PushL(encoder);

		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		frameImageData->AppendImageData(&data);

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		encodeListener->InitialiseActiveListener(encoder);
		streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToGet);
		CActiveScheduler::Start();

		const TInt err2 = encodeListener->iStatus.Int();
		if (err2 != KErrNone)
			{
			Printf(_L("failed to append image (%S)"), &TestFileName(j));
			TestFailed(err2);
			break;
			}

		encodeListener->InitialiseActiveListener(encoder);
		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}

		CleanupStack::PopAndDestroy(5, decoder);

		}
	}

/*
 * Tests for the ImageFrame Plugin block wise sequential streaming
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescSeqStreamDecEncBlockwiseImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Blockwise Stream Decoding and Encoding tests---"));

	TInt j;

	for (j = 1; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j+8));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		//initialize decode block streamer
		TImageConvStreamedDecode* streamDecode = NULL;
		TRAP(error,
			streamDecode = decoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		//initialize encode block streamer
		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TFrameInfo frameInfo = decoder->FrameInfo();

		TUid format = KUidFormatYUV420PlanarReversed;
		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		TInt imageSizeInBytes;
		TSize streamBlockSizeInPixels;
		TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
		TSize blockSizeInPixels;
		TJpegImageData data;
		TInt numBlocksRead;

		TInt frameNumber = 0;
		TInt numBlocksToGet = 10;

		if(j == 1)
			{
			format = KUidFormatYUV422Interleaved;
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			}
		else if(j == 2)
			{
			format = KUidFormatYUV420PlanarReversed;
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			}
		else if(j == 3)
			{
			format = KUidFormatYUVMonochrome;
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			}

		data.iQualityFactor = 95;


		//initialize decoder frame
		TInt frameNum = 0;
		TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationSequentialForward;
		TRAP(error,
			streamDecode->InitFrameL(format, frameNum, nav);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize decoder"));
			TestFailed(error);
			return;
			}

		//initialize encoder frame
		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
		CFrameImageData* frameImageData = CFrameImageData::NewL();
		frameImageData->AppendImageData(&data);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		for(;;)
			{
			imageSizeInBytes = streamDecode->GetBufferSize(format, streamBlockSizeInPixels, numBlocksToGet);

			HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
			CleanupStack::PushL(buffer);
		  	TPtr8 ptr = buffer->Des();

			// create an empty imageframe
			TFrameFormat frameFormat(format);
			CImageFrame* theImageFrame = NULL;
			TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
			if (createErr != KErrNone)
				{
				Printf(_L("Could not create imageFrame"));
				if (theImageFrame)
					{
					delete theImageFrame;
					}
				TestFailed(createErr);
				return;
				}
			CleanupStack::PushL(theImageFrame);

			theImageFrame->SetFrameFormatL(frameFormat);
			theImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);
			TBool haveMoreBlocks = ETrue;

			//decoder get blocks
			CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
			CleanupStack::PushL(decodelistener);
			decodelistener->InitialiseActiveListener(decoder);

			streamDecode->GetNextBlocks(decodelistener->iStatus, *theImageFrame, numBlocksToGet, numBlocksRead, haveMoreBlocks);

			CActiveScheduler::Start();

			const TInt err1 = decodelistener->iStatus.Int();
			if (err1 != KErrNone)
				{
				Printf(_L("failed to decode image (%S)"), &TestFileName(j));
				Printf(_L("failed to decode an image error=%d"), err1);
				TestFailed(err1);
				}
			CleanupStack::PopAndDestroy(1, decodelistener);

			//encoder append blocks
			CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
			CleanupStack::PushL(encodeListener);
			encodeListener->InitialiseActiveListener(encoder);

			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksRead);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				Printf(_L("failed to append image (%S)"), &TestFileName(j));
				TestFailed(err2);
				break;
				}

			CleanupStack::PopAndDestroy(3);

			if(haveMoreBlocks == EFalse)
				{
				break;
				}
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}
		CleanupStack::PopAndDestroy(1, encodeListener);


		delete decoder;
		delete encoder;
		}
	}

/*
 * Tests for the ImageFrame Plugin random streaming. This test randomly stream deacodes a file to imageframe and then stream encodes the imageframe buffer.
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescRandomStreamDecodeEncodeImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Random Forward Stream Decoding and Encoding tests---"));

	TInt j;

	for (j = 1; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j+8));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		//initialize decode block streamer
		TImageConvStreamedDecode* streamDecode = NULL;
		TRAP(error,
			streamDecode = decoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		//initialize encode block streamer
		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TFrameInfo frameInfo = decoder->FrameInfo();

		TUid format = KUidFormatYUV420PlanarReversed;
		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		TInt imageSizeInBytes;
		TSize streamBlockSizeInPixels;
		TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
		TSize blockSizeInPixels;
		TJpegImageData data;
		TInt numBlocksRead;

		TInt frameNumber = 0;
		TInt numBlocksToGet = 5;

		if(j == 1)
			{
			format = KUidFormatYUV422Interleaved;
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}
		else if(j == 2)
			{
			format = KUidFormatYUV420PlanarReversed;
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}
		else if(j == 3)
			{
			format = KUidFormatYUVMonochrome;
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}

		data.iQualityFactor = 95;


		//initialize decoder frame
		TInt frameNum = 0;
		TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationRandomForward;
		TRAP(error,
			streamDecode->InitFrameL(format, frameNum, nav);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize frame for streaming"));
			TestFailed(error);
			return;
			}

		//initialize encoder frame
		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
		CFrameImageData* frameImageData = CFrameImageData::NewL();
		frameImageData->AppendImageData(&data);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		for(TInt n = 0; n<5; n++ )
			{
			imageSizeInBytes = streamDecode->GetBufferSize(format, streamBlockSizeInPixels, numBlocksToGet);

			HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
			CleanupStack::PushL(buffer);
		  	TPtr8 ptr = buffer->Des();

			// create an empty imageframe
			TFrameFormat frameFormat(format);
			CImageFrame* theImageFrame = NULL;
			TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
			if (createErr != KErrNone)
				{
				Printf(_L("Could not create imageFrame"));
				TestFailed(createErr);
				return;
				}
			CleanupStack::PushL(theImageFrame);

			theImageFrame->SetFrameFormatL(frameFormat);
			theImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

			//decoder get blocks
			CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
			CleanupStack::PushL(decodelistener);
			decodelistener->InitialiseActiveListener(decoder);

			streamDecode->GetBlocks(decodelistener->iStatus, *theImageFrame, (n*(frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth)) + 1, numBlocksToGet, numBlocksRead);
			CActiveScheduler::Start();

			const TInt err1 = decodelistener->iStatus.Int();
			if (err1 != KErrNone)
				{
				Printf(_L("failed to decode image (%S)"), &TestFileName(j));
				Printf(_L("failed to decode an image error=%d"), err1);
				TestFailed(err1);
				}
			CleanupStack::PopAndDestroy(1, decodelistener);

			//encoder append blocks
			CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
			CleanupStack::PushL(encodeListener);
			encodeListener->InitialiseActiveListener(encoder);

			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToGet);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				Printf(_L("failed to append image (%S)"), &TestFileName(j));
				TestFailed(err2);
				break;
				}

			CleanupStack::PopAndDestroy(3);
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}
		CleanupStack::PopAndDestroy(1, encodeListener);


		delete decoder;
		delete encoder;
		}
	}

/*
 * Tests for the ImageFrame Plugin random streaming. This test stream deacodes a file random backwards to imageframe and then stream encodes the imageframe buffer.
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescRandomBackStreamDecEncImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Random Backward Stream Decoding and Encoding tests---"));

	TInt j;

	for (j = 2; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j+8));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		//initialize decode block streamer
		TImageConvStreamedDecode* streamDecode = NULL;
		TRAP(error,
			streamDecode = decoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		//initialize encode block streamer
		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TFrameInfo frameInfo = decoder->FrameInfo();

		TUid format = KUidFormatYUV420PlanarReversed;
		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		TInt imageSizeInBytes;
		TSize streamBlockSizeInPixels;
		TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
		TSize blockSizeInPixels;
		TJpegImageData data;
		TInt numBlocksRead;

		TInt frameNumber = 0;
		TInt numBlocksToGet = 4;

		if(j == 1)
			{
			format = KUidFormatYUV422Interleaved;
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}
		else if(j == 2)
			{
			format = KUidFormatYUV420PlanarReversed;
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}
		else if(j == 3)
			{
			format = KUidFormatYUVMonochrome;
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}

		data.iQualityFactor = 95;


		//initialize decoder frame
		TInt frameNum = 0;
		TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationRandomBackwards;
		TRAP(error,
			streamDecode->InitFrameL(format, frameNum, nav);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize frame for streaming"));
			TestFailed(error);
			return;
			}

		//initialize encoder frame
		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
		CFrameImageData* frameImageData = CFrameImageData::NewL();
		frameImageData->AppendImageData(&data);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		for(TInt n = 3; n>-1; n-- )
			{
			imageSizeInBytes = streamDecode->GetBufferSize(format, streamBlockSizeInPixels, numBlocksToGet);

			HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
			CleanupStack::PushL(buffer);
		  	TPtr8 ptr = buffer->Des();

			// create an empty imageframe
			TFrameFormat frameFormat(format);
			CImageFrame* theImageFrame = NULL;
			TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
			if (createErr != KErrNone)
				{
				Printf(_L("Could not create imageFrame"));
				TestFailed(createErr);
				return;
				}
			CleanupStack::PushL(theImageFrame);

			theImageFrame->SetFrameFormatL(frameFormat);
			theImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

			//decoder get blocks
			CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
			CleanupStack::PushL(decodelistener);
			decodelistener->InitialiseActiveListener(decoder);

			streamDecode->GetBlocks(decodelistener->iStatus, *theImageFrame, (n*(frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth)) + 1, numBlocksToGet, numBlocksRead);
			CActiveScheduler::Start();

			const TInt err1 = decodelistener->iStatus.Int();
			if (err1 != KErrNone)
				{
				Printf(_L("failed to decode image (%S)"), &TestFileName(j));
				Printf(_L("failed to decode an image error=%d"), err1);
				TestFailed(err1);
				}
			CleanupStack::PopAndDestroy(1, decodelistener);

			//encoder append blocks
			CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
			CleanupStack::PushL(encodeListener);
			encodeListener->InitialiseActiveListener(encoder);

			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToGet);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				Printf(_L("failed to append image (%S)"), &TestFileName(j));
				TestFailed(err2);
				break;
				}

			CleanupStack::PopAndDestroy(3);
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}

		CleanupStack::PopAndDestroy(1, encodeListener);


		delete decoder;
		delete encoder;
		}
	}

/*
 * Tests for the ImageFrame Plugin random Forward and Backward streaming. This test stream deacodes a file random forward and backwards to imageframe and then stream encodes the imageframe buffer.
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescRandomFwdBackStreamDecEncImageFramePluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Random Forward & Backward Stream Decoding and Encoding tests---"));

	TInt j;

	for (j = 2; j < KSampSchemeCount; j++)
		{
		TFileName refFileName = TFileName(TestStreamFileName(j+8));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		//
		// Encode from an image frame //
		//
		TFileName destFileName = TFileName(TestStreamFileName(j+4));
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			return;
			}

		//initialize decode block streamer
		TImageConvStreamedDecode* streamDecode = NULL;
		TRAP(error,
			streamDecode = decoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		//initialize encode block streamer
		TImageConvStreamedEncode* streamEncode = NULL;
		TRAP(error,
			streamEncode = encoder->BlockStreamerL();
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize block streamer"));
			TestFailed(error);
			return;
			}

		TFrameInfo frameInfo = decoder->FrameInfo();

		TUid format = KUidFormatYUV420PlanarReversed;
		TUid optimalFormat = KUidFormatYUV420PlanarReversed;
		TInt imageSizeInBytes;
		TSize streamBlockSizeInPixels;
		TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
		TSize blockSizeInPixels;
		TJpegImageData data;
		TInt numBlocksRead;

		TInt frameNumber = 0;
		TInt numBlocksToGet = 5;

		if(j == 1)
			{
			format = KUidFormatYUV422Interleaved;
			optimalFormat = KUidFormatYUV422Interleaved;
			data.iSampleScheme = TJpegImageData::EColor422;
			blockSizeInPixels = TSize(16,8);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}
		else if(j == 2)
			{
			format = KUidFormatYUV420PlanarReversed;
			optimalFormat = KUidFormatYUV420PlanarReversed;
			data.iSampleScheme = TJpegImageData::EColor420;
			blockSizeInPixels = TSize(16,16);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}
		else if(j == 3)
			{
			format = KUidFormatYUVMonochrome;
			optimalFormat = KUidFormatYUVMonochrome;
			data.iSampleScheme = TJpegImageData::EMonochrome;
			blockSizeInPixels = TSize(8,8);
			frameSizeInPixels = TSize( numBlocksToGet*blockSizeInPixels.iWidth, numBlocksToGet*blockSizeInPixels.iHeight);
			}

		data.iQualityFactor = 95;


		//initialize decoder frame
		TInt frameNum = 0;
		TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationRandomBackwards;
		TRAP(error,
			streamDecode->InitFrameL(format, frameNum, nav);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not initialize frame for streaming"));
			TestFailed(error);
			return;
			}

		//initialize encoder frame
		TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
		CFrameImageData* frameImageData = CFrameImageData::NewL();
		frameImageData->AppendImageData(&data);

		TRAP(error,
			streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, frameImageData);
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not Initialize Frame"));
			TestFailed(error);
			return;
			}

		for(TInt n = 0; n<5; n++ )
			{
			imageSizeInBytes = streamDecode->GetBufferSize(format, streamBlockSizeInPixels, numBlocksToGet);

			HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
			CleanupStack::PushL(buffer);
		  	TPtr8 ptr = buffer->Des();

			// create an empty imageframe
			TFrameFormat frameFormat(format);
			CImageFrame* theImageFrame = NULL;
			TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
			if (createErr != KErrNone)
				{
				Printf(_L("Could not create imageFrame"));
				TestFailed(createErr);
				return;
				}
			CleanupStack::PushL(theImageFrame);

			theImageFrame->SetFrameFormatL(frameFormat);
			theImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

			//decoder get blocks
			CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
			CleanupStack::PushL(decodelistener);
			decodelistener->InitialiseActiveListener(decoder);
			TInt pos =0;
			if(n == 1)
				{
				pos = 2;
				}
			else if(n == 2)
				{
				pos = 4;
				}
			else if(n == 3)
				{
				pos = 1;
				}
			else if(n == 4)
				{
				pos = 3;
				}

			streamDecode->GetBlocks(decodelistener->iStatus, *theImageFrame, (pos*(frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth)) + 1, numBlocksToGet, numBlocksRead);
			CActiveScheduler::Start();

			const TInt err1 = decodelistener->iStatus.Int();
			if (err1 != KErrNone)
				{
				Printf(_L("failed to decode image (%S)"), &TestFileName(j));
				Printf(_L("failed to decode an image error=%d"), err1);
				TestFailed(err1);
				}
			CleanupStack::PopAndDestroy(1, decodelistener);

			//encoder append blocks
			CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
			CleanupStack::PushL(encodeListener);
			encodeListener->InitialiseActiveListener(encoder);

			streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToGet);
			CActiveScheduler::Start();

			const TInt err2 = encodeListener->iStatus.Int();
			if (err2 != KErrNone)
				{
				Printf(_L("failed to append image (%S)"), &TestFileName(j));
				TestFailed(err2);
				break;
				}

			CleanupStack::PopAndDestroy(3);
			}

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		streamEncode->Complete(encodeListener->iStatus);
		CActiveScheduler::Start();

		const TInt err4 = encodeListener->iStatus.Int();
		if (err4 != KErrNone)
			{
			Printf(_L("failed to complete image (%S)"), &TestFileName(j));
			TestFailed(err4);
			}

		CleanupStack::PopAndDestroy(1, encodeListener);


		delete decoder;
		delete encoder;
		}
	}

/*
 * Tests for the ImageFrame Plugin streaming:
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgDescStreamNegativeTestsL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 JPEG Streaming Decoding and Encoding negative tests---"));

	TFileName refFileName = TFileName(KImageTestTranscodeProgressive);
	//
	// Decode to an image frame //
	//
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}

	TImageConvStreamedDecode* streamDecode = NULL;
	TRAP(error,
		streamDecode = decoder->BlockStreamerL();
		);
	if (streamDecode==NULL || error != KErrNone)
		{
		Printf(_L("Negative test for progressive jpeg passed."));
		}
	else
		{
		Printf(_L("Failure: Initialized block streamer for progressive jpeg"));
		TestFailed(error);
		return;
		}

	delete decoder;
	decoder = NULL;
	refFileName = TFileName(KImageStreamTestJfif420);
	TRAP(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}

	streamDecode = NULL;
	TRAP(error,
		streamDecode = decoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize block streamer"));
		TestFailed(error);
		return;
		}

	RArray<TUid> formatsCheck;
	CleanupClosePushL(formatsCheck);
	formatsCheck.AppendL(KUidFormatYUV420Planar);
	formatsCheck.AppendL(KUidFormatYUV420PlanarReversed);

	TUid optimalFormatCheck = KUidFormatYUV420Planar;

	RArray<TUid> formats;
	TUid optimalFormat;

	CleanupClosePushL(formats);
	
	TRAP(error,
		streamDecode->GetSupportedFormatsL(formats, optimalFormat);
		);
	if (error != KErrNone)
		{
		Printf(_L("GetSupportedFormatsL failed!"));
		TestFailed(error);
		}

	if(optimalFormat != optimalFormatCheck)
		{
		Printf(_L("GetSupportedFormatsL() failed (aOptimalFormat incorrect)."));
		TestFailed(KErrNotFound);
		}

	if(formats.Count() == formatsCheck.Count())
		{
		for(TInt i = 0; i < formats.Count(); i++)
			{
			// Check all elements match
			if(formats[i] != formatsCheck[i])
				{
				Printf(_L("GetSupportedFormatsL() failed."));
				TestFailed(KErrNotFound);
				}
			}
		Printf(_L("GetSupportedFormatsL() succeeded."));
		}
	else
		{
		Printf(_L("GetSupportedFormatsL() failed (aFormats returned unexpected length)."));
		Printf(_L("The correct number of formats should be %d."), formatsCheck.Count());
		Printf(_L("The returned number of formats by GetSupportedFormatsL() is %d."), formats.Count());
		TestFailed(KErrNotFound);
		}

	CleanupStack::PopAndDestroy();	// formats
	CleanupStack::PopAndDestroy();	// formatsCheck

	TInt maxBlocksPerRequest = KMaxTInt;
	TInt minStreamSizeInBlocks = 1;
	TSize minBlockSizeInPixels(16,16);
	TInt optimalBlocksPerRequest = 256;

	TDecodeStreamCaps::TNavigation navDec = static_cast<TDecodeStreamCaps::TNavigation> (TDecodeStreamCaps::ENavigationSequentialForward | TDecodeStreamCaps::ENavigationRandomForward | TDecodeStreamCaps::ENavigationRandomBackwards);

	TDecodeStreamCaps streamCaps(maxBlocksPerRequest, minBlockSizeInPixels,
								optimalBlocksPerRequest, minStreamSizeInBlocks, navDec);

	TDecodeStreamCaps caps;
	streamDecode->GetCapabilities(optimalFormat,0, caps);

	// Compare values
	if(caps.MaxBlocksPerRequest() == maxBlocksPerRequest
		&& caps.MinBlockSizeInPixels() == minBlockSizeInPixels
		&& caps.OptimalBlocksPerRequest() == optimalBlocksPerRequest
		&& caps.Navigation() == navDec
		&& caps.StreamSizeInBlocks() == minStreamSizeInBlocks)
		{
		Printf(_L("GetCapabilities() succeeded in setting values."));
		}
	else
		{
		Printf(_L("GetCapabilities() failed."));
		TestFailed(KErrNotFound);
		}
	
	delete decoder;
	
	
	// Negative test for Encoder AddBlocks	
	//
	// Encode from an image frame //
	//
	TFileName destFileName = TFileName(KImageFrameStreamTestRef420);
	CImageEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}
	CleanupStack::PushL(encoder);

	TImageConvStreamedEncode* streamEncode = NULL;
	TRAP(error,
		streamEncode = encoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not initialize block streamer"));
		CleanupStack::PopAndDestroy(encoder);
		TestFailed(error);
		return;
		}
	
	CImageFrame* theImageFrame = NULL;

	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

	TJpegImageData data;
	data.iSampleScheme = TJpegImageData::EColor420;

	CFrameImageData* frameImageData = CFrameImageData::NewL();
	frameImageData->AppendImageData(&data);

	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
	
	TRAP(error,
		streamEncode->InitFrameL(optimalFormat, 0, TSize(160,160), TSize(16,16), navigation, frameImageData);
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not Initialize Frame"));
		CleanupStack::PopAndDestroy(2, encoder);
		TestFailed(error);
		return;
		}
	
	encodeListener->InitialiseActiveListener(encoder);
	streamEncode->AddBlocks(encodeListener->iStatus, *theImageFrame, 0);
	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();
	CleanupStack::PopAndDestroy(2, encoder);
	if (err2 != KErrNotSupported)
		{
		Printf(_L("AddBlocks is supported by Jpeg Encoder - negative test failed"));
		TestFailed(err2);
		return;
		}
	else
		{
		Printf(_L("AddBlocks expected to fail by Jpeg Encoder block streamer - negative test passed"));		
		}
	}

/*
 * Alloc Tests for the ImageFrame Plugin stream encoding
 */
void TImage::TestStreamEncodeAllocL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 Stream Encoding Alloc Tests---"));

	for (TInt i = 1 ; ; i++)
		{
		if (i % 5 == 0)
			{
			Printf(_L("Fail count = %d"), i);
			}
		
		TFileName refFileName = TFileName(TestStreamFileName(KStream420ImgIdx));
		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}
		CleanupStack::PushL(decoder);
		
		// create destination Desc
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			CleanupStack::PopAndDestroy(decoder);
			return;
			}

		HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
		CleanupStack::PushL(buffer);
	  	TPtr8 ptr = buffer->Des();

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			CleanupStack::PopAndDestroy(decoder);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode image (%S)"), &TestFileName(KStream420ImgIdx));			
			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}
		
		CleanupStack::PopAndDestroy(1, decodelistener);
				
		__UHEAP_SETFAIL(RHeap::EFailNext, i);
		__MM_HEAP_MARK;
		
		TRAPD(errAlloc, StreamEncodeImageFrameL(theImageFrame));
		
		if (!(errAlloc == KErrNone || errAlloc == KErrNoMemory )) 
			{
			 // [ we failed for some reason other than memory
			 //     allocation, so fail the test ]
			TestFailed(errAlloc);
			CleanupStack::PopAndDestroy(3, decoder);
			return;
			}
		
		TAny* testAlloc = User::Alloc(1);
		TBool heapTestingComplete = (testAlloc == NULL) && (errAlloc==KErrNone);
		User::Free(testAlloc);
		
		CleanupStack::PopAndDestroy(3, decoder);
		
		__MM_HEAP_MARKEND;
		__UHEAP_RESET;

		if ((errAlloc == KErrNone ) || heapTestingComplete)
			{
			Printf(_L("err = %d, Fail count = %d"), errAlloc, i);
			Printf(_L("Alloc testing completed successfully"));
			break;
			}
		}
	}

/*
 * Alloc Tests for the ImageFrame Plugin stream encoding
 */
void TImage::TestStreamEncodeDecodeAllocL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ1630 Stream Encode & Decode Alloc Tests---"));

	TBool isThreaded = EFalse;
	
	Printf(_L("Run decode and encode in a same thread"));
	
	for(TInt k = 0; k<2 ; k++)
		{
		if(k == 1)
			{
			Printf(_L("Run decode and encode in a separate thread"));
			isThreaded = ETrue;
			}
		for (TInt i = 1 ; ; i++)
			{
			if (i % 5 == 0)
				{
				Printf(_L("Fail count = %d"), i);
				}
			__UHEAP_SETFAIL(RHeap::EFailNext, i);
			__MM_HEAP_MARK;
			
			TRAPD(errAlloc, StreamEncodeDecodeImageFrameL(isThreaded));
			
			if (!(errAlloc == KErrNone || errAlloc == KErrNoMemory )) 
				{
				 // [ we failed for some reason other than memory
				 //     allocation, so fail the test ]
				TestFailed(errAlloc);
				return;
				}
			
			TAny* testAlloc = User::Alloc(1);
			TBool heapTestingComplete = (testAlloc == NULL) && (errAlloc==KErrNone);
			User::Free(testAlloc);
			
			__MM_HEAP_MARKEND;
			__UHEAP_RESET;
		
			if ((errAlloc == KErrNone ) || heapTestingComplete)
				{
				Printf(_L("err = %d, Fail count = %d"), errAlloc, i);
				Printf(_L("Alloc testing completed successfully"));
				break;
				}
			}
		}		
	}
#endif

void TImage::StreamEncodeImageFrameL(CImageFrame* theImageFrame)
	{
	//
	// Encode from an image frame //
	//
	TFileName destFileName = TFileName(TestStreamFileName(KStream420ImgIdx + 4));	
	CImageEncoder* encoder = NULL;
	encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		
	CleanupStack::PushL(encoder);
	
	TImageConvStreamedEncode* streamEncode = NULL;
	TRAPD(error, streamEncode = encoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(encoder);
		User::Leave(error);
		}
		
	TInt frameNumber = 0;
	TSize frameSizeInPixels(160,160);
	TSize blockSizeInPixels;

	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;

	TUid optimalFormat;	
	optimalFormat = KUidFormatYUV420PlanarReversed;
	blockSizeInPixels = TSize(16,16);

	TRAP(error,
		streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, NULL);
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(encoder);
		User::Leave(error);
		}
	
	TInt numBlocksToAdd = 100;
	
	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
	
	encodeListener->InitialiseActiveListener(encoder);
	
	streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToAdd);
	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();
	if (err2 != KErrNone)
		{
		CleanupStack::PopAndDestroy(2, encoder);
		User::Leave(err2);
		}

	encodeListener->InitialiseActiveListener(encoder);
	streamEncode->Complete(encodeListener->iStatus);
	CActiveScheduler::Start();

	const TInt err4 = encodeListener->iStatus.Int();
	if (err4 != KErrNone)
		{
		CleanupStack::PopAndDestroy(2, encoder);
		User::Leave(err4);
		}
	
	CleanupStack::PopAndDestroy(2, encoder);
	}

/*
 * Tests for the ImageFrame Plugin sequential streaming. This test sequentially stream deacodes a file to imageframe and then stream encodes the imageframe buffer.
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::StreamEncodeDecodeImageFrameL(const TBool aIsThreaded)
	{
	TUint decoderOption = CImageDecoder::EOptionNone;
	TUint encoderOption = CImageEncoder::EOptionNone;
	
	if(aIsThreaded)
		{
		decoderOption = CImageDecoder::EOptionAlwaysThread;
		encoderOption = CImageEncoder::EOptionAlwaysThread;
		}
	
	TFileName refFileName = TFileName(TestStreamFileName(KStream420ImgIdx+8));
	//
	// Decode to an image frame //
	//
	CImageDecoder* decoder = NULL;	
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, CImageDecoder::TOptions(decoderOption), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		User::Leave(error);			
		}
	CleanupStack::PushL(decoder);

	TImageConvStreamedDecode* streamDecode = NULL;
	TRAP(error,
		streamDecode = decoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(decoder);
		User::Leave(error);
		}
	
	TFrameInfo frameInfo = decoder->FrameInfo();

	TUid format = KUidFormatYUV420PlanarReversed;
	TUid optimalFormat = KUidFormatYUV420PlanarReversed;
	TInt imageSizeInBytes = 0;
	TInt numBlocksToGet = 0;
	TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
	TSize blockSizeInPixels;
	TSize streamBlockSizeInPixels;
	TJpegImageData data;
	TInt frameNumber = 0;
	TDecodeStreamCaps caps;

	data.iSampleScheme = TJpegImageData::EColor420;
	
	streamDecode->GetCapabilities(format,frameNumber, caps);
	blockSizeInPixels = TSize(caps.MinBlockSizeInPixels());
	numBlocksToGet = (frameInfo.iOverallSizeInPixels.iWidth/blockSizeInPixels.iWidth) * (frameInfo.iOverallSizeInPixels.iHeight/blockSizeInPixels.iHeight);

	data.iQualityFactor = 95;

	imageSizeInBytes = streamDecode->GetBufferSize(format, streamBlockSizeInPixels, numBlocksToGet);
	
	HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
	CleanupStack::PushL(buffer);
  	TPtr8 ptr = buffer->Des();

	// create an empty imageframe
	CImageFrame* theImageFrame = NULL;
	TRAPD(createErr, theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes));
	if (createErr != KErrNone)
		{
		CleanupStack::PopAndDestroy(2, decoder);
		User::Leave(error);
		}
	CleanupStack::PushL(theImageFrame);
	
	TInt frameNum = 0;
	TDecodeStreamCaps::TNavigation nav = TDecodeStreamCaps::ENavigationSequentialForward;
	
	TRAP(error,
		streamDecode->InitFrameL(format, frameNum, nav);
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(3, decoder);	
		User::Leave(error);
		}
	
	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);

	TInt numBlocksRead;
	TBool haveMoreBlocks;
	theImageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

	decodelistener->InitialiseActiveListener(decoder);
	streamDecode->GetNextBlocks(decodelistener->iStatus, *theImageFrame, numBlocksToGet, numBlocksRead, haveMoreBlocks);
	CActiveScheduler::Start();
	const TInt err1 = decodelistener->iStatus.Int();
	if (err1 != KErrNone)
		{
		CleanupStack::PopAndDestroy(4, decoder);
		User::Leave(error);
		}

	//
	// Encode from an image frame //
	//
	TFileName destFileName = TFileName(TestStreamFileName(KStream420ImgIdx+4));
	CImageEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::TOptions(encoderOption), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(4, decoder);
		User::Leave(error);
		}
	CleanupStack::PushL(encoder);
	
	TImageConvStreamedEncode* streamEncode = NULL;
	TRAP(error,
		streamEncode = encoder->BlockStreamerL();
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(5, decoder);
		User::Leave(error);
		}

	TEncodeStreamCaps::TNavigation navigation = TEncodeStreamCaps::ENavigationSequentialForward;
	
	TRAP(error,
		streamEncode->InitFrameL(optimalFormat, frameNumber, frameSizeInPixels, blockSizeInPixels, navigation, NULL);
		);
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(5, decoder);
		User::Leave(error);
		}

	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);

	encodeListener->InitialiseActiveListener(encoder);
	streamEncode->AppendBlocks(encodeListener->iStatus, *theImageFrame, numBlocksToGet);
	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();
	if (err2 != KErrNone)
		{
		CleanupStack::PopAndDestroy(6, decoder);	
		User::Leave(error);
		}
	
	encodeListener->InitialiseActiveListener(encoder);
	streamEncode->Complete(encodeListener->iStatus);
	CActiveScheduler::Start();

	const TInt err4 = encodeListener->iStatus.Int();
	CleanupStack::PopAndDestroy(6, decoder);
	if (err4 != KErrNone)
		{
		User::Leave(error);
		}
	}

/*
 * Negative tests for the encoder when using not supported formats:
 * For each supported YUV format, decode an image to an imageFrame and try to encode it back to a file using
 * a different YUV format to the one used in the imageFrame (Transcoding not supportted - KErrNotSupported)
 */
void TImage::NegTestJpgImageFrameEncoderPluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 Negative Test JPEG ImageFrame Encoder plugin ---"));
	TInt j;

	for (j = 1; j < 4; j++)
		{
		__MM_HEAP_MARK;
		TFileName refFileName = TFileName(TestFileName(j));

		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
			decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}
		CleanupStack::PushL(decoder);

		// create destination RChunk
		RChunk chunk;
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			Printf(_L("Codec does not support this image format"));
			TestFailed(error);
			CleanupStack::PopAndDestroy(decoder);
			return;
			}

		TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
		if (res != KErrNone)
			{
			CleanupStack::PopAndDestroy(decoder);
			User::Leave(res);
			}

	  	CleanupClosePushL(chunk);

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame =	CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			TestFailed(createErr);
			CleanupStack::PopAndDestroy(2, decoder);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodelistener);
		decodelistener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		const TInt err1 = decodelistener->iStatus.Int();
		if (err1 != KErrNone)
			{
			Printf(_L("failed to decode an image error=%d"), err1);
			TestFailed(err1);
			}

		CleanupStack::PopAndDestroy(1, decodelistener);

		//
		// Encode from an image frame //
		//
		CJPEGImageFrameEncoder* encoder = NULL;
		TRAP(error,
			encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, TFileName(KImageFrameTestNegative), CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
			);
		if (error!=KErrNone)
			{
			Printf(_L("Could not create encoder"));
			TestFailed(error);
			CleanupStack::PopAndDestroy(3, decoder);
			return;
			}
		CleanupStack::PushL(encoder);

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);

		TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
		CleanupStack::PushL(jpegFormat);

		// DEF083995: Verify that the default TJpegImageData constructor has
		// initialised the jpegFormat data members to sensible values
		if (jpegFormat->iSampleScheme != TJpegImageData::EColor420 && jpegFormat->iQualityFactor != 75)
			{
			Printf(_L("jpegFormat not initialised"));
			TestFailed(KErrGeneral);
			CleanupStack::PopAndDestroy(6, decoder);
			return;
			}
		jpegFormat->iSampleScheme = TJpegImageData::EColor444;

		User::LeaveIfError(frameImageData->AppendImageData(jpegFormat));

		CleanupStack::Pop(jpegFormat);

		CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);
		encodeListener->InitialiseActiveListener(encoder);

		encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame, frameImageData);

		CActiveScheduler::Start();

		// Check that the error return is KErrNotSupported
		const TInt err2 = encodeListener->iStatus.Int();
		if (err2 == KErrNotSupported)
			{
			Printf(_L("OK. KErrNotSupported - Transcoding not supported"));
			}
		else
			{
			TestFailed(err2);
			}

		CleanupStack::PopAndDestroy(6, decoder);

		__MM_HEAP_MARKEND;
		}
	}

/*
 * Negative tests for the decoder when using not supported formats:
 * - Try to decode a jpg file containing image in a not supported YUV format (KErrNotSupported)
 * - or if supported decode to a format different to the one used when the original image was
 * encoded (Transcoding not supportted - KErrNotSupported)
 */
void TImage::NegTestJpgImageFrameDecoderPluginL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 Negative Test JPEG ImageFrame Decoder plugin ---"));
	TInt j;

	for (j = 0; j < 3; j++)
		{
		__MM_HEAP_MARK;
		TFileName refFileName = TFileName(TestFileName(j));

		//
		// Decode to an image frame //
		//
		CJPEGImageFrameDecoder* decoder = NULL;
		TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid))
		);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}

		// create destination RChunk
		RChunk chunk;
		TInt imageSizeInBytes;
		TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
		if (supported == EFalse)
			{
			// We want to test transcoding, create chunk anyway with a dummy size
			imageSizeInBytes = KDummyImageSize;
			}

		TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
		User::LeaveIfError(res);

	  	CleanupClosePushL(chunk);

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame =	CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			return;
			}
		CleanupStack::PushL(theImageFrame);

		if (j == 0)
			{
			// Do nothing. This is the case to decode file KImageTestJfif444.
			// The codec should leave with error code: KErrNotSupported.
			}
		else if (j == 1)
			{
			// This is the case to decode file KImageTestJfif422.
			// Application is specifying a different format 4:2:0
			// The codec should leave with error code: KErrNotSupported,
			// since transcoding is not supported by this codec.
			TFrameFormat the420FrameFormat = TFrameFormat(KUidFormatYUV420Planar );
			theImageFrame->SetFrameFormatL(the420FrameFormat);
			}
		else
			{
			// This is the case to decode file KImageTestJfif420.
			// Application is specifying a different format 4:2:2
			// The codec should leave with error code: KErrNotSupported,
			// since transcoding is not supported by this codec.
			TFrameFormat the422FrameFormat = TFrameFormat(KUidFormatYUV422Interleaved);

			theImageFrame->SetFrameFormatL(the422FrameFormat);
			}

		CActiveDecodeListener* decodeListener = new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(decodeListener);
		decodeListener->InitialiseActiveListener(decoder);

		decoder->ConvertFrame(&decodeListener->iStatus, *theImageFrame);

		CActiveScheduler::Start();

		// Check that the error return is KErrNotSupported
		const TInt err = decodeListener->iStatus.Int();
		if (err == KErrNotSupported)
			{
			if (j == 0)
				{
				Printf(_L("OK. KErrNotSupported - 4:4:4 format not supported"));
				}
			else
				{
				Printf(_L("OK. KErrNotSupported - Transcoding not supported"));
				}
			}
		else
			{
			TestFailed(err);
			}

		CleanupStack::PopAndDestroy(3, &chunk);
		delete decoder;
		__MM_HEAP_MARKEND;
		}
	}
/*
 * Negative tests for decoder trying to use image frame reduction:
 * - Try to decode to a ImageFrame size smaller than the required image size.
 */
void TImage::NegTestJpgImageFramePluginReductionL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 Negative Test JPEG ImageFrame reduction ---"));

	__MM_HEAP_MARK;
	TFileName refFileName = TFileName(KImageTestJfif420);

	//
	// Decode to an image frame //
	//
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}


	// create destination RChunk
	RChunk chunk;
	TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), KDummyImageSize, KDummyImageSize, EOwnerProcess);
	User::LeaveIfError(res);

	CleanupClosePushL(chunk);

	// create an empty imageframe
  	CImageFrame* theImageFrame = NULL;
	TRAPD(createErr, theImageFrame = CImageFrame::NewL(&chunk, KDummyImageSize, KChunkDataOffset));
	if (createErr != KErrNone)
		{
		Printf(_L("Could not create imageFrame"));
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		TestFailed(createErr);
		return;
		}
	CleanupStack::PushL(theImageFrame);
	// set the size small so reduction factor is required
	theImageFrame->SetFrameSizeInPixels(KThumbnailSize);

	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);
	decodelistener->InitialiseActiveListener(decoder);

	decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err1 = decodelistener->iStatus.Int();
	if (err1 == KErrNotSupported)
		{
		Printf(_L("OK. KErrNotSupported - Image Size Reduction not supported"));
		}
	else
		{
		TestFailed(err1);
		}

	CleanupStack::PopAndDestroy(3, &chunk);

	delete decoder;
	__MM_HEAP_MARKEND;

	}


/*
 * Run decode and encode in a separate thread:
 * Decode a YUV420 image to a Chunk wrapped by ImageFrame from a jpg file
 * Feed the image to the encoder to be encoded back to a jpg file.
 * Both decoder and encoder are created to run encode/decode in a different
 * thread to the client's.
 */
void TImage::TestJpgImageFramePluginNewThreadL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 JPEG ImageFrame plugin - decoding in a separate thread ---"));

	//
	// Decode to an image frame //
	//
	__MM_HEAP_MARK;
	TFileName refFileName = TFileName(KImageTestJfif420);
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAPD(error,
	decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, CImageDecoder::EOptionAlwaysThread, KNullUid, KNullUid, KImageFramePluginUid));
	);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(KUidFormatYUV420Planar , imageSizeInBytes);
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(error);
		return;
		}

	TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	User::LeaveIfError(res);

  	CleanupClosePushL(chunk);

	// create an empty imageframe
	CImageFrame* theImageFrame = NULL;
	TRAPD(createErr,
		{
		theImageFrame =	CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset);
		});
	if (createErr != KErrNone)
		{
		Printf(_L("Could not create imageFrame"));
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		TestFailed(createErr);
		return;
		}
	CleanupStack::PushL(theImageFrame);

	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);
	decodelistener->InitialiseActiveListener(decoder);

	decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err1 = decodelistener->iStatus.Int();
	if (err1 != KErrNone)
		{
		Printf(_L("failed to decode image"));
		TestFailed(err1);
		}

	CleanupStack::PopAndDestroy(1, decodelistener);
	delete decoder;

	//
	// Encode from an image frame //
	//
	TFileName destFileName = TFileName(KImageFrameTestEncode420t);
	CJPEGImageFrameEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionAlwaysThread, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}

	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
	encodeListener->InitialiseActiveListener(encoder);

	encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();

	if (err2 != KErrNone)
		{
		Printf(_L("failed to encode image"));
		TestFailed(err2);
		}
	else
		{
		VerifyAgainstReferenceFile(destFileName, TFileName(KImageFrameTestRef420));
		}

	CleanupStack::PopAndDestroy(3, &chunk);
	delete encoder;
	__MM_HEAP_MARKEND;
	}

/*
 * Postive test to derive from CImageFrame:
 * - Pass to the codec a supported derived class of ImageFrame.
 * (The codec will ignore the extra features implemented by the derived class)
 */
void TImage::TestImageFrameDerivedL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 ImageFrame plugin using ImageFrame derived class ---"));

	__MM_HEAP_MARK;
	TFileName refFileName = TFileName(KImageTestJfif420);

	//
	// Decode to an image frame //
	//
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(error);
		return;
		}

	TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	User::LeaveIfError(res);

  	CleanupClosePushL(chunk);

	CFbsBitmap* aDummyBitmap=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(aDummyBitmap);

	// create an empty imageframe
	CImageFrameDerived* theImageFrame = NULL;
	TRAPD(createErr, theImageFrame = CImageFrameDerived::NewL(&chunk, imageSizeInBytes, KChunkDataOffset, *aDummyBitmap));
	if (createErr != KErrNone)
		{
		Printf(_L("Could not create imageFrame"));
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		TestFailed(createErr);
		return;
		}
	CleanupStack::PushL(theImageFrame);

	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);
	decodelistener->InitialiseActiveListener(decoder);

	decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err1 = decodelistener->iStatus.Int();
	if (err1 != KErrNone)
		{
		Printf(_L("failed to decode image"));
		TestFailed(err1);
		}

	CleanupStack::PopAndDestroy(1, decodelistener);
	delete decoder;

	//
	// Encode from an image frame //
	//
	TFileName destFileName = TFileName(KImageFrameTestEncode420ifd);
	CJPEGImageFrameEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		return;
		}

	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
	encodeListener->InitialiseActiveListener(encoder);

	encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();
	if (err2 != KErrNone)
		{
		Printf(_L("failed to encode image"));
		TestFailed(err2);
		}
	else
		{
		VerifyAgainstReferenceFile(destFileName, TFileName(KImageFrameTestRef420));
		}

	CleanupStack::PopAndDestroy(4, &chunk);

	delete encoder;
	__MM_HEAP_MARKEND;
	}

/*
 * Neative test to decode a not supported CImageFrame extension:
 * - Pass to the codec a non supported ImageFrame, which contains a non supported
 *  implementation of TFrameFormatBase and TFrameLayoutBase.
 */
void TImage::NegTestFullImageFrameL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 Negative Test: Full ImageFrame Replacement test ---"));

	__MM_HEAP_MARK;
	TFileName refFileName = TFileName(KImageTestJfif420);

	//
	// Decode to an image frame //
	//
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(error);
		return;
		}

	TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	User::LeaveIfError(res);

  	CleanupClosePushL(chunk);

	// create an empty imageframe
	CImageFrameRChunk* theImageFrame = NULL;
	TRAPD(createErr, theImageFrame = CImageFrameRChunk::NewL(chunk, KChunkDataOffset));
	if (createErr != KErrNone)
		{
		Printf(_L("Could not create CImageFrameDerived"));
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		TestFailed(createErr);
		return;
		}
	CleanupStack::PushL(theImageFrame);

	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);
	decodelistener->InitialiseActiveListener(decoder);

	decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	// Check that the error return is KErrNotSupported
	const TInt err = decodelistener->iStatus.Int();
	if (err == KErrNotSupported)
		{
		Printf(_L("OK. KErrNotSupported - This frame format is not supported"));
		}
	else
		{
		TestFailed(err);
		}

	CleanupStack::PopAndDestroy(3, &chunk);
	delete decoder;
	__MM_HEAP_MARKEND;
	}

const TInt KStreamBreakpoint=100; // up to this point use initialincrement. then use secondary
const TInt KStreamInitialIncrement=1;
const TInt KStreamSecondaryIncrement=128;
inline TInt StreamIncrement(TInt aValue)
	{
	return (aValue<KStreamBreakpoint)?KStreamInitialIncrement:KStreamSecondaryIncrement;
	}

/**
 * Positive test for the decoder when calling ContinueConvertFrame() when the required image is not always there;
 * the file one works by successively copying data from the main to a temp file.
 */
void TImage::StreamDecodeFileToImageFrameL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 Stream Decode to Image Frame test ---"));

	TInt aFrameNo = 1;

	RFile file;
	TFileName aFileName = TFileName(KImageTestJfif420);
	User::LeaveIfError(file.Open(iFs, aFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));

	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(1024)), 0, 1024);

	RFile tempFile;
	TPtrC tempFileName(KImageTempFileName);
	User::LeaveIfError(tempFile.Replace(iFs, tempFileName, EFileShareAny|EFileStream|EFileWrite));
	CleanupClosePushL(tempFile);

#if defined(_DEBUG)
		{
		TEntry entry;
		TInt error = iFs.Entry(tempFileName, entry);
		ASSERT(error==KErrNone);
		ASSERT(entry.iSize==0); // should be empty
		}
#endif //defined(_DEBUG)

	TInt wantedBufferSize=1;
	TInt tempFileLength = 0;

	if (wantedBufferSize>0)
		{
		User::LeaveIfError(file.Read(buffer, wantedBufferSize));
		ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += wantedBufferSize;
		}

 	// gradually increment buffer size until we can open the decoder
	CJPEGImageFrameDecoder* decoder = NULL;
	for (;;)
		{
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, tempFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
		if (error == KErrNone)
			{
			break;
			}
		if (error != KErrUnderflow || wantedBufferSize >= fileSize)
			{
			User::Leave(error);
			}
		TInt increment = StreamIncrement(wantedBufferSize);
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - tempFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(file.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += extra;
		}
	CleanupStack::PushL(decoder);

	// gradually increment buffer size until we get info about the given frame
	TBool frameIsReady = EFalse;
	while (!frameIsReady && wantedBufferSize<fileSize)
		{
		TInt increment = StreamIncrement(wantedBufferSize);
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - tempFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(file.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += extra;

		decoder->ContinueProcessingHeaderL();
		if ((decoder->FrameCount() > aFrameNo) && (decoder->FrameInfo(aFrameNo).CurrentFrameState() == TFrameInfo::EFrameInfoProcessingComplete))
			{
			frameIsReady = ETrue;
			}
		}

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(KErrNotSupported);
		CleanupStack::PopAndDestroy(4, &file); // decoder, tempFile, buffer and file
		}
	else
		{
		TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
		User::LeaveIfError(res);

	  	CleanupClosePushL(chunk);

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			CleanupStack::PopAndDestroy(5, &file); // chunk, decoder, tempFile, buffer and file
			}
		else
			{
			CleanupStack::PushL(theImageFrame);

			CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
			CleanupStack::PushL(decodelistener);

			TBool first = ETrue;
			for(;;)
				{
				decodelistener->InitialiseActiveListener(decoder);
				if (first)
				    {
					decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame,aFrameNo);
				    first = EFalse;
				    }

				else
				    {
					decoder->ContinueConvertFrame(&decodelistener->iStatus);
				    }

				CActiveScheduler::Start();
				const TInt error = decodelistener->iStatus.Int();
				if (error==KErrNone)
					{
					break;
					}
				if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
					{
					User::Leave(error);
					}
				TInt increment = StreamIncrement(wantedBufferSize);
				wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
				TInt extra = wantedBufferSize - tempFileLength;
				ASSERT(extra>0);
				User::LeaveIfError(file.Read(buffer, extra));
				ASSERT(buffer.Length()==extra); // should always work on emulator
				User::LeaveIfError(tempFile.Write(buffer));
				tempFileLength += extra;
				}

			// Create an object of CJPEGImageFrameEncoder
		    CJPEGImageFrameEncoder* imageEncoder = NULL;
		    TInt err;
			TRAP(err, imageEncoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, KImageStreamToFile, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));

			if (err != KErrNone)
				{
				Printf(_L("Error during creating Image Encoder to file %S"), &KImageStreamToFile);
				if (err == KErrPathNotFound)
		    		{
		    		Printf(_L("Path %S not found"), &KImageStreamToFile);
		    		}
				if (err == KErrAccessDenied)
		    		{
		    		Printf(_L("Access denied"));
		    		}
				TestFailed(err);
				CleanupStack::PopAndDestroy(7, &file); // decodeListener, theImageFrame, chunk, decoder, tempFile, buffer and file
				}
			else
				{
				CleanupStack::PushL(imageEncoder);

			    // Create CActiveListener
				CActiveListener* activeEncodeListener = new(ELeave)CActiveListener;
				CleanupStack::PushL( activeEncodeListener );

				activeEncodeListener->InitialiseActiveListener();

				// Do the Encode from YUV to JPEG
				imageEncoder->ConvertFrame(&activeEncodeListener->iStatus, *theImageFrame);
				CActiveScheduler::Start();

				err = activeEncodeListener->iStatus.Int();

			    if (err != KErrNone)
					{
					Printf(_L("Error during conversion"));
					User::Leave(err);
					}

				VerifyAgainstReferenceFile(KImageStreamToFile, TFileName(KImageFrameTestRef420));

				CleanupStack::PopAndDestroy(9, &file); // ativeEncodeListener, imageEncoder, decodeListener, theImageFrame, chunk, decoder, tempFile, buffer and file
				}
			}
		}
	}

/**
 * Negative test for the decoder when calling ContinueConvertFrame() without calling ConvertFrame() the first time,
 * so the destination is not valid, ContinueconvertFrame() should then set the status to KErrNotReady
 */
void TImage::NegTestStreamDecodeFileToImageFrameL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- PREQ747 Negative Test: Stream Decode to Image Frame test ---"));

	__MM_HEAP_MARK;

	TInt aFrameNo = 1;

	RFile file;
	TFileName aFileName = TFileName(KImageTestJfif420);
	User::LeaveIfError(file.Open(iFs, aFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));

	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(1024)), 0, 1024);

	RFile tempFile;
	TPtrC tempFileName(KImageTempFileName);
	User::LeaveIfError(tempFile.Replace(iFs, tempFileName, EFileShareAny|EFileStream|EFileWrite));
	CleanupClosePushL(tempFile);

#if defined(_DEBUG)
		{
		TEntry entry;
		TInt error = iFs.Entry(tempFileName, entry);
		ASSERT(error==KErrNone);
		ASSERT(entry.iSize==0); // should be empty
		}
#endif //defined(_DEBUG)

	TInt wantedBufferSize=1;
	TInt tempFileLength = 0;

	if (wantedBufferSize>0)
		{
		User::LeaveIfError(file.Read(buffer, wantedBufferSize));
		ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += wantedBufferSize;
		}

 	// gradually increment buffer size until we can open the decoder
	CJPEGImageFrameDecoder* decoder = NULL;
	for (;;)
		{
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, tempFileName, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));
		);
		if (error == KErrNone)
			{
			break;
			}
		if (error != KErrUnderflow || wantedBufferSize >= fileSize)
			{
			User::Leave(error);
			}
		TInt increment = StreamIncrement(wantedBufferSize);
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - tempFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(file.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += extra;
		}
	CleanupStack::PushL(decoder);

	// gradually increment buffer size until we get info about the given frame
	TBool frameIsReady = EFalse;
	while (!frameIsReady && wantedBufferSize<fileSize)
		{
		TInt increment = StreamIncrement(wantedBufferSize);
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - tempFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(file.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += extra;

		decoder->ContinueProcessingHeaderL();
		if ((decoder->FrameCount() > aFrameNo) && (decoder->FrameInfo(aFrameNo).CurrentFrameState() == TFrameInfo::EFrameInfoProcessingComplete))
			{
			frameIsReady = ETrue;
			}
		}

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(KErrNotSupported);
		CleanupStack::PopAndDestroy(4, &file); // decoder, tempFile, buffer and file
		}
	else
		{
		TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
		User::LeaveIfError(res);

	  	CleanupClosePushL(chunk);

		// create an empty imageframe
		CImageFrame* theImageFrame = NULL;
		TRAPD(createErr, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
		if (createErr != KErrNone)
			{
			Printf(_L("Could not create imageFrame"));
			if (theImageFrame)
				{
				delete theImageFrame;
				}
			TestFailed(createErr);
			CleanupStack::PopAndDestroy(5, &file); // chunk, decoder, tempFile, buffer and file
			}
		else
			{
			CleanupStack::PushL(theImageFrame);

			CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
			CleanupStack::PushL(decodelistener);

			decodelistener->InitialiseActiveListener(decoder);
			// Instead of calling ConvertFrame() the first time, we call directly ContinueConvertFrame()
			decoder->ContinueConvertFrame(&decodelistener->iStatus);
			CActiveScheduler::Start();
			const TInt error = decodelistener->iStatus.Int();
			if (error == KErrNotReady)
				{
				Printf(_L("Encoding image process canceled successfully failed with error KErrNotReady"));
				}
			else
				{
				Printf(_L("Encoding image process canceled returned unexpected error = %d, expected = %d"), error, KErrNotReady);
				TestFailed(error);
				}

			CleanupStack::PopAndDestroy(7, &file); //decodeListener, theImageFrame, chunk, decoder, tempFile, buffer and file
			}
		}

	__MM_HEAP_MARKEND;
	}

/**
 * Compare two binary files
 *
 * @param "const TDesC& aFile1"
 *		  The first file to compare
 * @param "const TDesC& aFile2"
 *		  The second file to compare
 * @leave ""
 *		  Will leave with appropriate system codes if a problem is encountered reading either file
 * @return "TVerdict"
 *		  EPass if the files are identical
 *		  EFail otherwise
 */
TVerdict TImage::CompareFilesL(const TDesC& aFile1, const TDesC& aFile2)
	{
	RFile file1, file2;
	RFs fs;
	TInt err;
	
	err = fs.Connect();
	if (err != KErrNone)
		{
		Printf(_L("RFs.Connect() failed with %d"), err);
		User::Leave(err);
		}
	
	CleanupClosePushL(fs);

	err = file1.Open(fs, aFile1, EFileShareAny | EFileStream | EFileRead);
	if (err != KErrNone)
		{
		Printf(_L("Opening %S failed with %d"), &aFile1, err);
		User::Leave(err);
		}
	CleanupClosePushL(file1);

	err = file2.Open(fs, aFile2, EFileShareAny | EFileStream | EFileRead);
	if (err != KErrNone)
		{
		Printf(_L("Opening %S failed with %d"), &aFile2, err);
		User::Leave(err);
		}
	CleanupClosePushL(file2);

	TInt fileSize1, fileSize2;

	TVerdict result = EPass;
	TInt blockSize = 1024;

	User::LeaveIfError(file1.Size(fileSize1));
	User::LeaveIfError(file2.Size(fileSize2));

	if (fileSize1 == fileSize2)
		{
		HBufC8* buf1 = HBufC8::NewMaxLC(blockSize);
		TPtr8 buf1Ptr(buf1->Des());
		HBufC8* buf2 = HBufC8::NewMaxLC(blockSize);
		TPtr8 buf2Ptr(buf2->Des());

		TInt length = 0;

		while ((length < fileSize1) && (result == EPass))
			{
			TInt copySize;

			if (fileSize1 - length > blockSize)
				{
				copySize = blockSize;
				}
			else
				{
				copySize = fileSize1 - length;
				}

			err = file1.Read(buf1Ptr, copySize);
			if (err != KErrNone)
				{
				Printf(_L("Reading from %S failed with %d"), &aFile1, err);
				User::Leave(err);
				}
			
			err = file2.Read(buf2Ptr, copySize);
			if (err != KErrNone)
				{
				Printf(_L("Reading from %S failed with %d"), &aFile2, err);
				User::Leave(err);
				}

			if (Mem::Compare(buf1Ptr.Ptr(), copySize, buf2Ptr.Ptr(), copySize) != 0)
				{
				Printf(_L("Contents differ"));
				result = EFail;
				}

			length += copySize;
			}
		CleanupStack::PopAndDestroy(2); // buf1, buf2
		}
	else
		{
		Printf(_L("File sizes differ: %d vs %d"), fileSize1, fileSize2);
		result = EFail;
		}

	file1.Close();
	file2.Close();
	fs.Close();

	CleanupStack::PopAndDestroy(3); //fs, file1, file2

	return result;
	}



// copy the failed destination file to a new file - so it can be compared later
void TImage::CopyOutputFileL(const TDesC& aFileToCopy, const TDesC& aDestName)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	CFileMan* fileMan = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMan);
	User::LeaveIfError(fileMan->Copy(aFileToCopy, aDestName, CFileMan::EOverWrite));
	Printf(_L("Copied output image to %S for comparison"), &aDestName);
	CleanupStack::PopAndDestroy(2, &fs);
	}

void TImage::VerifyIfdOffsets(CExifVerifier& aVerifier, const TDesC& aExifFile)
	{
	TInt err = KErrNone;
	TBool offsetsOk = EFalse;
	CExifVerifier::TVerifyResult result = aVerifier.VerifyIfdOffsets();
	if (result == CExifVerifier::EOffsetNotWordAligned)
		{
		Printf(_L("Offsets not word Aligned"));
		TestFailed(KErrCorrupt);
		}
	else if (result == CExifVerifier::EOffsetOutOfRange)
		{
		Printf(_L("Offsets out of range"));
		TestFailed(KErrCorrupt);
		}
	else
		{
		offsetsOk = ETrue;
		Printf(_L("Ifd Offsets in range and 16-bit Word Alignment Verified"));
		}
	if (err!=KErrNone || !offsetsOk)
		{
		TFileName copyName;
		iUniqueNumber++; // make sure this is unique to this test run
		copyName.Format(_L("%S.VerifyOffsetsFailed.%d.%S"), &aExifFile, iUniqueNumber, &iTestName);
		// It is of no importance to the test if copying the output file for later
		// reference fails
		TRAP_IGNORE(CopyOutputFileL(aExifFile, copyName));
		}

	}


void TImage::VerifyAgainstReferenceFile(const TDesC& aDstFile, const TDesC& aRefFileName)
{
	TVerdict verdict = EFail;
	TRAPD(err, verdict = CompareFilesL(aDstFile, aRefFileName));
	if ((err != KErrNone) || (verdict != EPass))
		{
		Printf(_L("Encoded image does not match reference image (err = %d)"), err);
		Printf(_L("(%S != %S)"), &aDstFile, &aRefFileName);
		TFileName copyName;
		iUniqueNumber++; // make sure this is unique to this test run
		copyName.Format(_L("%S.fail.%d.%S"), &aRefFileName, iUniqueNumber, &iTestName);
		// It is of no importance to the test if copying the output file for later
		// reference fails
		TRAP_IGNORE(CopyOutputFileL(aDstFile, copyName));

		TestFailed(err);
		}
	else
		{
		Printf(_L("Encoded image matches reference image"));
		Printf(_L("(%S == %S)"), &aDstFile, &aRefFileName);
		}
}

/**
 * Returns a file name for a particular index
 *
 * @param "TInt aIndex"
 *		  Index to search for file
 * @return "const TDesC& "
 *		  DesC containing filename
 */
const TDesC& TImage::TestFileName(TInt aIndex)
	{
	switch (aIndex)
		{
		case 0:		return KImageTestJfif444;
		case 1:		return KImageTestJfif422;
		case 2:		return KImageTestJfif420;
		case 3:		return KImageTestJfifMono;
		case 4:		return KImageFrameTestEncode444c;
		case 5:		return KImageFrameTestEncode422c;
		case 6:		return KImageFrameTestEncode420c;
		case 7:		return KImageFrameTestEncodeMonoc;
		case 8:		return KImageFrameTestEncode444d;
		case 9:		return KImageFrameTestEncode422d;
		case 10:	return KImageFrameTestEncode420d;
		case 11:	return KImageFrameTestEncodeMonod;
		case 12:	return KImageFrameTestRef444;
		case 13:	return KImageFrameTestRef422;
		case 14:	return KImageFrameTestRef420;
		case 15:	return KImageFrameTestRefMono;
		case 16:	return KImageTestJfif422_2MP;
		case 17:	return KImageTestJfif420_2MP;
		case 18:	return KImageTestJfifMono_2MP;
		case 19:	return KImageFrameTestRef422_2MP;
		case 20:	return KImageFrameTestRef420_2MP;
		case 21:	return KImageFrameTestRefMono_2MP;		
		default:	return KNullDesC;
		}
	}

/**
 * Returns a file name for a particular index
 *
 * @param "TInt aIndex"
 *		  Index to search for file
 * @return "const TDesC& "
 *		  DesC containing filename
 */
const TDesC& TImage::TestStreamFileName(TInt aIndex)
	{
	switch (aIndex)
		{
		case 0:		return KImageStreamTestJfif444;
		case 1:		return KImageStreamTestJfif422;
		case 2:		return KImageStreamTestJfif420;
		case 3:		return KImageStreamTestJfifMono;
		case 4:		return KImageFrameStreamTestRef444;
		case 5:		return KImageFrameStreamTestRef422;
		case 6:		return KImageFrameStreamTestRef420;
		case 7:		return KImageFrameStreamTestRefMono;
		case 8:		return KImageStreamDecodeTestJfif444;
		case 9:		return KImageStreamDecodeTestJfif422;
		case 10:	return KImageStreamDecodeTestJfif420;
		case 11:	return KImageStreamDecodeTestJfifMono;
		default:	return KNullDesC;
		}
	}

void TImage::TestThumbnailHeader()
	{
	//
	//		Get the thumbnail
	CImageDecoder* decoderthumbnail=NULL;
	TRAPD(allocErr, decoderthumbnail=CreateDecoderOnFileL(TFileName(KImageTestThumbnailDecode)));
	Printf(_L("step1"));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	TInt aFrameNumber = 0;
	const TFrameInfo& orgThumbnail = decoderthumbnail->FrameInfo(aFrameNumber);
	const TSize sizeReffThumbnail = orgThumbnail.iOverallSizeInPixels;//reference thumbnails TSize
	CImageDecoder* decoder=NULL;
	TRAP(allocErr, decoder=CreateDecoderOnFileL(TFileName(KImageTestExifThumbnailDecode)));
	Printf(_L("step2"));
	if(allocErr!=KErrNone)
		{
		Printf(_L("could not create decoder"));
		TestFailed(allocErr);
		return;
		}
	const TFrameInfo& decodMainImage = decoder->FrameInfo(aFrameNumber);
	const TSize sizeMainImage = decodMainImage.iOverallSizeInPixels;//Main Images TSize
	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail))//Setting the imagetype to thumbnail
	Printf(_L("step3"));
	if(allocErr!=KErrNone)
		{
		delete decoder;
		Printf(_L("Could not set thumbnail getter on the decoder"));
		TestFailed(allocErr);
		return;
		}
	const TFrameInfo& decodThumbnail= decoder->FrameInfo(aFrameNumber);//Thumbnails header info should be present now at FrameInfo(0)
	const TSize sizeOrgThumbnail = decodThumbnail.iOverallSizeInPixels;//Thumbnail images TSize
	if(sizeOrgThumbnail != sizeReffThumbnail)
		{
		Printf(_L("Thumb nails header info isn't at iFrameInfo(0)"));
		TestFailed(KErrNotFound);
		return;
		}
	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeMain))
	if(allocErr!=KErrNone)
		{
		delete decoder;
		Printf(_L("Could not set thumbnail getter on the decoder"));
		TestFailed(allocErr);
		return;
		}
	const TFrameInfo& decodMainSet = decoder->FrameInfo(aFrameNumber);
	const TSize sizeSetMainImage = decodMainSet.iOverallSizeInPixels;//Main Images TSize
	if(sizeSetMainImage != sizeMainImage)
		{
		Printf(_L("Main Images header info isn't at iFrameInfo(0)"));
		TestFailed(KErrNotFound);
		return;
		}
	delete decoder;
	delete decoderthumbnail;
	}

//
//
// CR0712   Relax JPEG and GIF decoding strictness
//
//
class TRelaxedJpgTestParam
	{
public:
	const TText*	iSrcFileName;
	const TText*	iRefFileName;
	};

void TImage::TestRelaxedJpgDecodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	static const TRelaxedJpgTestParam KTestFiles[]=
		{
			{_S("c:\\tsu_icl_cod_04\\289_8981100000300165521f.jpg"), _S("c:\\tsu_icl_cod_04\\289_8981100000300165521f.mbm")},
			{_S("c:\\tsu_icl_cod_04\\system-3128-3.jpg"), NULL},
			{_S("c:\\tsu_icl_cod_04\\system-3128-2.jpg"), NULL},
			{_S("c:\\tsu_icl_cod_04\\system-3128-1.jpg"), _S("c:\\tsu_icl_cod_04\\system-3128-1.mbm")},
			{_S("c:\\tsu_icl_cod_04\\no_trail.gif"), _S("c:\\tsu_icl_cod_04\\no_trail.mbm")},
			{NULL, NULL}
		};

	Printf(_L("\n"));
	Printf(_L("--- CR0712   Relax JPEG and GIF decoding strictness---"));

	const TRelaxedJpgTestParam* testParam=KTestFiles;
	while (testParam->iSrcFileName)
		{
		Printf(_L("--- TestFile %s"), testParam->iSrcFileName);
		CImageDecoder* decoder = NULL;
		if(iDecoderUid.iUid != 0)
			{
			decoder = CImageDecoder::FileNewL(iFs, TPtrC(testParam->iSrcFileName), DecoderOptions(), KNullUid, KNullUid, iDecoderUid);	
			}
		else
			{
			decoder= CImageDecoder::FileNewL(iFs, TPtrC(testParam->iSrcFileName), DecoderOptions());
			}
		CleanupStack::PushL(decoder);
		CFbsBitmap* target=new (ELeave) CFbsBitmap();
		CleanupStack::PushL(target);

		const TFrameInfo& frameInfo=decoder->FrameInfo(0);
		target->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode);

		const TInt error=DoImageDecodeL(decoder, *target);

		if (error != KErrNone)
			{
			Printf(_L("--- ERROR: failed to decode an image error=%d"),error);
			TestFailed(error);
			}
		else if (testParam->iRefFileName)
			{
			TBool r=CompareBitmapsL(*target, TPtrC(testParam->iRefFileName));
			if (!r)
				{
				Printf(_L("--- ERROR: the decoded bitmap doesn't match to the reference one."));
				TestFailed(KErrGeneral);
				}
			}

		CleanupStack::PopAndDestroy(2, decoder);
		++testParam;
		}
	}

void TImage::DoTestJpegImageFrameTranscodeL(const TDesC& aFileName, const TDesC& aRefFileName)
	{
	//
	// Decode to an image frame //
	//
	__MM_HEAP_MARK;
	TFileName refFileName(aFileName);
	CJPEGImageFrameDecoder* decoder = NULL;
	TInt error = 0;
	if(iDecoderUid.iUid != 0)
		{
		TRAP(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, CImageDecoder::EOptionAlwaysThread, KNullUid, KNullUid, iDecoderUid));
		);
		if (error != KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(error);
			return;
			}	
		}
	else
		{
		TRAP(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, CImageDecoder::EOptionAlwaysThread, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		return;
		}
		}
	CleanupStack::PushL(decoder);

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (!supported )
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(error);
		CleanupStack::PopAndDestroy(decoder);
		return;
		}

	TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	if (res != KErrNone)
		{
		Printf(_L("Unable to create chunk"));
		TestFailed(error);
		CleanupStack::PopAndDestroy(decoder);
		return;
		}

	CleanupClosePushL(chunk);


	// create an empty imageframe
	CImageFrame* theImageFrame = NULL;
	TRAPD(createErr,
		{
		theImageFrame =	CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset);
		});
	if (createErr != KErrNone)
		{
		Printf(_L("Could not create imageFrame"));
		TestFailed(createErr);
		CleanupStack::PopAndDestroy(2, decoder);
		return;
		}
	CleanupStack::PushL(theImageFrame);

	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);
	decodelistener->InitialiseActiveListener(decoder);

	decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err1 = decodelistener->iStatus.Int();
	if (err1 != KErrNone)
		{
		Printf(_L("failed to decode image"));
		TestFailed(err1);
		CleanupStack::PopAndDestroy(4, decoder);
		return;
		}

	//
	// Encode from an image frame //
	//
	TFileName destFileName(KImageTempFileName);
	CJPEGImageFrameEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionAlwaysThread, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		CleanupStack::PopAndDestroy(4, decoder);
		return;
		}

	CleanupStack::PushL(encoder);

	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
	encodeListener->InitialiseActiveListener(encoder);

	encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();

	if (err2 != KErrNone)
		{
		Printf(_L("failed to encode image"));
		TestFailed(err2);
		}
	else
		{
		VerifyAgainstReferenceFile(destFileName, TFileName(aRefFileName));
		}

	CleanupStack::PopAndDestroy(6, decoder);
	__MM_HEAP_MARKEND;
	}

void TImage::TestJpegImageFrameTranscodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- DEF083826: CJPEGImageFrameXXX transcoding doesn't work ---"));
	Printf(_L("--- DEF096682: CJPEGImageFrameDecoder fails with Kern-EXEC 3 when used with progressive JPEG ---"));

	DoTestJpegImageFrameTranscodeL(KImageTestTranscode, KImageFrameTestRefTranscode);
	DoTestJpegImageFrameTranscodeL(KImageTestTranscodeProgressive, KImageTestRefTranscodeProgressive);
    }

TInt TImage::TestCorruptedHeaderFileL(const TDesC& aSourceFileName, const TDesC& aRefBmpFileName)
	{
	//
	//		Get the thumbnail
	CFbsBitmap *bitmap = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(KThumbnailSize,KThumbnailDispMode));

	CJPEGExifDecoder* decoder=NULL;
	TRAPD(allocErr, decoder=CreateDecoderOnFileL(TFileName(aSourceFileName)));
	if (allocErr != KErrNone)
		{
		Printf(_L("Trying to create the decoder for file %S failed with error %d"), &aSourceFileName, allocErr);
		CleanupStack::PopAndDestroy(bitmap);
		delete decoder;
		return allocErr;
		}

	TInt frameNumber = 0;
	const TFrameInfo& origMainSize = decoder->FrameInfo(frameNumber);
	const TSize mainImageSize = origMainSize.iOverallSizeInPixels;//Main Images TSize

	TRAP(allocErr, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail))

	if ( (allocErr != KErrCorrupt) && (allocErr != KErrNotFound))
		{
		delete decoder;
		Printf(_L("Trying to set thumbnail getter on the decoder for the file %S failed with wrong error = %d"), &aSourceFileName, allocErr);
		CleanupStack::PopAndDestroy(bitmap);
		return allocErr;
		}

	const TFrameInfo& mainImageReff = decoder->FrameInfo(frameNumber);
	const TSize mainImageReffSize = mainImageReff.iOverallSizeInPixels;
	if (mainImageSize != mainImageReffSize)
		{
		Printf(_L("Main Images header info isn't at iFrameInfo(0)"));
		return KErrNotFound;
		}

	CActiveDecodeListener listener;
	listener.InitialiseActiveListener(decoder);
	decoder->Convert(&listener.iStatus, *bitmap);
	CActiveScheduler::Start();

	//destroy the decoder
	delete decoder;
	if (listener.iStatus.Int() != KErrNone)
		{
		Printf(_L("Unable to decode the image"));
		CleanupStack::PopAndDestroy(bitmap);
		return listener.iStatus.Int();
		}

	if(aRefBmpFileName.Length()>0)
		{
		CFbsBitmap *thumbRef = new (ELeave) CFbsBitmap;
		CleanupStack::PushL(thumbRef);
		TInt bmpErr = thumbRef->Load(aRefBmpFileName);
		if (bmpErr != KErrNone)
			{
			Printf(_L("Trying to load reference thumbnail file %S failed with error %d"), &aRefBmpFileName, bmpErr);
			CleanupStack::PopAndDestroy(2); // bitmap, thumbRef
			return bmpErr;
			}

		TBool compResult = CompareBitmaps(*thumbRef,*bitmap);
		CleanupStack::PopAndDestroy(2); //bitmap,thumbRef
		if(compResult == EFalse)
			{
			Printf(_L("Bitmap reference %S and bitmap created from %S do not match"), &aRefBmpFileName, &aSourceFileName);
			return KErrCorrupt;
			}
		}
	else
		{
		CleanupStack::PopAndDestroy(1); // bitmap
		}

	return KErrNone;
	}

// Test case for DEF085157:
// CImageDecoder::SetImageTypeL() could leave in an ambiguous way
void TImage::TestSetImageTypeOnCorruptedHeaderFiles()
	{
	if( (TestCorruptedHeaderFileL(KTestImageNoExif, KTestNoExifReference) == KErrNone)
		&& (TestCorruptedHeaderFileL(KTestImageCorruptMainHeader, KNullDesC) != KErrNone)
		&& (TestCorruptedHeaderFileL(KTestImageCorruptThumbnailHeader, KCorruptThumbnailHeaderReference) == KErrNone)
		)
		{
		Printf(_L("Testing of method SetImageTypeL() passed."));
		}
	else
		{
		Printf(_L("Testing of method SetImageTypeL() failed."));
		TestFailed(KErrGeneral);
		}
	}

// Alloc test for DEF085157:
// CImageDecoder::SetImageTypeL() could leave in an ambiguous way
void TImage::AllocSetImageTypeTestL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	TInt failCount = 1;
	TBool completed = EFalse;
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for(;;)
		{
		CJPEGExifDecoder* decoder = NULL;
	   	TRAPD(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, KTestImageCorruptThumbnailHeader, DecoderOptions(), KImageTypeJPGUid)));

	   	if (error != KErrNone)
			{
			Printf(_L("Trying to create the decoder for file %S failed with error %d"), &KTestImageCorruptThumbnailHeader, error);
			delete decoder;
			TestFailed(error);
			return;
			}

		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TInt err = 0;
		TRAP(err, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail));

		completed = EFalse;

		if ((err == KErrCorrupt) || (err == KErrNone))
			{
			TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
			if (testAlloc==NULL)
				{
				reachedEnd = ETrue;
				failCount -= 1;
				}
			else
				{
				User::Free(testAlloc);
				}

			completed = reachedEnd;
			}
		else if (err != KErrNoMemory) // bad error code
			{
			completed = ETrue;
			Printf(_L("Bad error code %d"), err);
			TestFailed(err);
			}

		delete decoder;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if(completed)
			{
			break; // exit loop
			}

		failCount += 1;
		}
	Printf(_L("Finish with %d allocation tests"),failCount);
	}


// TestAsciiNullConsistency tests how well Exif handles NULL terminated ASCII
// strings for the following combinations, for both Encoders and for Transforms:
// 		Adding a new tag with a NULL terminated string of ANY size (ImageDescription)
//		Setting (i.e. overwriting) an existing tag with a NULL terminated string of ANY size (Make)
//		Adding a new tag with a NULL terminated string of fixed size (DateTime)
//		Setting (i.e. overwriting) an existing tag with a NULL terminated string of fixed size (DateTimeOriginal)
void TImage::TestAsciiNullConsistency(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Ascii Null Consistency tests ---"));

	_LIT8(KNullTerminated1, "I am an ascii string\0");
	_LIT8(KNullTerminated2, "This is a test\0");
	_LIT8(KNullTerminatedDateTime, "2006:06:06 14:53:56\0");

	TestEncoderAsciiNullConsistency(KNullTerminated1, KNullTerminated2, KNullTerminatedDateTime);
	TestTransformAsciiNullConsistencyL(KNullTerminated1, KNullTerminated2, KNullTerminatedDateTime);
	}

// TestAsciiNonNullConsistency tests how well Exif handles non-NULL terminated ASCII
// strings for the following combinations, for both Encoders and for Transforms:
// 		Adding a new tag with non-NULL terminated string of ANY size (ImageDescription)
//		Setting (i.e. overwriting) an existing tag with a non-NULL terminated string of ANY size (Make)
//		Adding a new tag with a non-NULL terminated string of fixed size (DateTime)
//		Setting (i.e. overwriting) an existing tag with a non-NULL terminated string of fixed size (DateTimeOriginal)
void TImage::TestAsciiNonNullConsistency(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Ascii Non Null Consistency tests ---"));

	_LIT8(KNotNullTerminated1, "I am an ascii string");
	_LIT8(KNotNullTerminated2, "This is a test");
	_LIT8(KNotNullTerminatedDateTime, "2006:06:06 14:53:56");

	TestEncoderAsciiNullConsistency(KNotNullTerminated1, KNotNullTerminated2, KNotNullTerminatedDateTime);
	TestTransformAsciiNullConsistencyL(KNotNullTerminated1, KNotNullTerminated2, KNotNullTerminatedDateTime);
	}

// NegTestAsciiNonNullConsistency tests how well Exif handles non-NULL
// terminated ASCII strings for the following combinations:
//		An attempt is made to ADD a non-NULL terminated string of length 20
//		to the DateTime tag (which must be of length 20 including a NULL termination).
//		This is repeated with strings longer than 20 (both NULL & not-NULL terminated).
//
//		An attempt is made to SET a non-NULL terminated string of length 20
//		to the DateTimeOriginal tag (which must be of length 20 including a NULL termination)
//		This is repeated with strings longer than 20 (both NULL & not-NULL terminated).
//
//		Expected result is that the attempts fails gracefully.
void TImage::NegTestAsciiNonNullConsistency(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Neg Ascii Non Null Consistency tests ---"));

	_LIT8(KExactlyTwenty, "2006:06:06 14:53:566");
	_LIT8(KMoreThanTwentyNoNull, "2006:06:06 14:53:566ABCDE");
	_LIT8(KMoreThanTwentyNull, "2006:06:06 14:53:566ABCDE/0");
	_LIT8(KLessThanTwentyNoNull, "too short");
	_LIT8(KLessThanTwentyNull, "too short\0");

	//First test a string that is exactly the right length without a NULL termination
	NegTestEncoderAsciiNonNullConsistency(KExactlyTwenty);
	NegTestTransformAsciiNonNullConsistency(KExactlyTwenty);

	//Next test a string that is too long without NULL
	NegTestEncoderAsciiNonNullConsistency(KMoreThanTwentyNoNull);
	NegTestTransformAsciiNonNullConsistency(KMoreThanTwentyNoNull);

	//Next test a string that is too long with NULL
	NegTestEncoderAsciiNonNullConsistency(KMoreThanTwentyNull);
	NegTestTransformAsciiNonNullConsistency(KMoreThanTwentyNull);

	//Next test a string that is too short without NULL
	NegTestEncoderAsciiNonNullConsistency(KLessThanTwentyNoNull);
	NegTestTransformAsciiNonNullConsistency(KLessThanTwentyNoNull);

	//Next test a string that is too short with NULL
	NegTestEncoderAsciiNonNullConsistency(KLessThanTwentyNull);
	NegTestTransformAsciiNonNullConsistency(KLessThanTwentyNull);
	}

// NegTestEncoderAsciiNonNullConsistency runs DoNegTestAsciiNonNullConsistency
// with metadata taken from an Encoder
void TImage::NegTestEncoderAsciiNonNullConsistency(const TDesC8& aData)
	{
	Printf(_L("--- Encoder Test ---"));

	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
	TRAPD(testResult,
		CreateEncoderFromReferenceBitmapFileL(KEncodeSourceFileName, bitmap, TFileName(KImageTestExifTransformDest), encoder);
		)
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(testResult);
		return;
		}

	testResult=DoNegTestAsciiNonNullConsistency(encoder->ExifMetadata(), aData);
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}

	delete bitmap;
	delete encoder;
	}

// NegTestTransformAsciiNonNullConsistency runs DoNegTestAsciiNonNullConsistency
// with metadata taken from a Transformer.
void TImage::NegTestTransformAsciiNonNullConsistency(const TDesC8& aData)
	{
	Printf(_L("--- Transformer Test ---"));

	//
	//		Get the transform
	TFileName sourceFileName(KImageTestTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(testResult,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,EFalse));
	if (testResult!=KErrNone)
		{
		TestFailed(testResult);
		return;
		}
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the transform
	//

	testResult=DoNegTestAsciiNonNullConsistency(extension->ExifMetadata(), aData);
	if(testResult!=KErrNone)
		{
		TestFailed(testResult);
		}
	delete imageTransform;
	}


// DoNegTestAsciiNonNullConsistency tests how well Exif handles non-NULL
// terminated ASCII strings for the following combinations:
//		An attempt is made to ADD a non-NULL terminated string of length 20
//		to the DateTime tag (which must be of length 20 including a NULL termination).
//
//		An attempt is made to SET a non-NULL terminated string of length 20
//		to the DateTimeOriginal tag (which must be of length 20 including a NULL termination)
//
//		Expected result is that the attempts fails gracefully.
TInt TImage::DoNegTestAsciiNonNullConsistency(MExifMetadata* aMetaData, const TDesC8& aData)
	{
	//Printf(_L("Testing invalid string %S"), &aData);

	ASSERT(aMetaData!=NULL);

	TExifWriterUtility writer(aMetaData);
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;

	HBufC8* buffer=aData.Alloc();
	if(buffer==NULL)
		{
		Printf(_L("Could not alloc HBufC8"));
		return KErrNoMemory;
		}

	//
	// 		ADD the DateTime tag

	// N.B.: The Exif Spec states that DateTime is a recommended but
	// not mandatory tag, therefore it will not have been created by default.
	// This means that the following call to SetDateTime will result
	// in a new DateTime tag being ADDED to the metadata.
	testResult=writer.SetDateTime(buffer);
	if (testResult!=KErrNotSupported)
		{
		Printf(_L("SetDateTime did not behave as expected %d"), testResult);
		finalResult=KErrCorrupt;
		}
	// 		ADD the DateTime tag
	//

	//
	// 		SET the DateTimeOriginal tag

	// N.B.: Although the Exif Spec states that DateTimeOriginal is an optional
	// (not mandatory) tag, our Exif code treats it as mandatory and adds one
	// by default.  This means that the following call to SetDateTimeOriginal will
	// result in the existing DateTimeOriginal tag being SET (i.e. overwritten)
	// in the metadata.
	testResult=writer.SetDateTimeOriginal(buffer);
	if (testResult!=KErrArgument)
		{
		Printf(_L("SetDateTimeOriginal did not behave as expected %d"), testResult);
		finalResult=KErrCorrupt;
		}
	// 		SET the DateTimeOriginal tag
	//
	delete buffer;
	return finalResult;
	}

// TestEncoderAsciiNullConsistency runs DoAsciiNullConsistencyTests
// with metadata taken from an Encoder
void TImage::TestEncoderAsciiNullConsistency(const TDesC8& aImageDescription, const TDesC8& aMake, const TDesC8& aDateTime)
	{
	Printf(_L("--- Encoder Test ---"));

	//
	//		Get the exif encoder
	TFileName fileName(KImageTestExifTransformDest);
	CFbsBitmap* bitmap=NULL;
	CJPEGExifEncoder* encoder=NULL;
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

	//Set the data
	testResult=DoAsciiNullConsistencyTests(aImageDescription, aMake, aDateTime, encoder->ExifMetadata());
	if (testResult!=KErrNone)
		{
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
	if (testResult!=KErrNone)
		{
		Printf(_L("Encoding failed."));
		TestFailed(testResult);
		return;
		}
	//		encode the image
	//
	CExifVerifier* verifier=NULL;
	TRAP(testResult, verifier=CExifVerifier::NewL(fileName, CExifVerifier::EJpegFile));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify Ifd Offsets
	VerifyIfdOffsets(*verifier, fileName);
	delete verifier;
	VerifyAgainstReferenceFile(fileName, TFileName(KImageTestExifNullAsciiEncoderRef));

	}

// TestTransformAsciiNullConsistencyL runs DoAsciiNullConsistencyTests
// with metadata taken from an Transformer
void TImage::TestTransformAsciiNullConsistencyL(const TDesC8& aImageDescription, const TDesC8& aMake, const TDesC8& aDateTime)
	{
	Printf(_L("--- Transformer Test ---"));
	//
	//		Get the metadata
	TFileName sourceFileName(KImageTestTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,EFalse));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);

	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	//		Get the metadata
	//

	//Set the data
	TInt testResult=DoAsciiNullConsistencyTests(aImageDescription, aMake, aDateTime, extension->ExifMetadata());
	if (testResult!=KErrNone)
		{
		TestFailed(testResult);
		CleanupStack::PopAndDestroy(imageTransform);
		return;
		}

	//Do the transform
	testResult=DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(testResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(testResult);
		return;
		}

	CExifVerifier* verifier=NULL;
	TRAP(testResult, verifier=CExifVerifier::NewL(destFileName, CExifVerifier::EJpegFile));
	if(testResult!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(testResult);
		return;
		}
	// verify Ifd Offsets
	VerifyIfdOffsets(*verifier, destFileName);
	delete verifier;
	VerifyAgainstReferenceFile(destFileName, TFileName(KImageTestExifNullAsciiTransformRef));

	}

// DoAsciiNullConsistencyTests uses its arguments to test the following:
//		aImageDescription is ADDED to a new ImageDescription tag (which is of ANY size)
//		aMake is SET (i.e. overwrites) an existing Make tag (which is of ANY size)
//		aDateTime is ADDED to a new DateTime tag (which is of fixed size 20 including NULL termination)
//		aDateTime is SET (i.e. overwrites) an existing DateTimeOriginal tag (which is of fixed size 20 including NULL termination)
TInt TImage::DoAsciiNullConsistencyTests(const TDesC8& aImageDescription, const TDesC8& aMake, const TDesC8& aDateTime, MExifMetadata* aMetaData)
	{
	ASSERT(aMetaData!=NULL);

	TExifWriterUtility writer(aMetaData);
	TInt testResult=KErrNone;
	TInt finalResult=KErrNone;

	//
	//		ADD the ImageDescription tag
	HBufC8* buffer=aImageDescription.Alloc();
	if(buffer==NULL)
		{
		Printf(_L("Could not alloc HBufC8 for ImageDescription"));
		return KErrNoMemory;
		}

	// N.B.: The Exif Spec states that ImageDescription is a recommended but
	// not mandatory tag, therefore it will not have been created by default.
	// This means that the following call to SetImageDescription() will result
	// in a new ImageDescription tag being ADDED to the metadata.
	testResult=writer.SetImageDescription(buffer);
	delete buffer;
	buffer=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("SetImageDescription failed."));
		finalResult=testResult;
		}
	//		ADD the ImageDescription tag
	//

	//
	// 		SET the Make tag
	buffer=aMake.Alloc();
	if(buffer==NULL)
		{
		Printf(_L("Could not alloc HBufC8 for Make"));
		return KErrNoMemory;
		}

	// N.B.: Although the Exif Spec states that Make is a recommended
	// (not mandatory) tag, our Exif code treats it as mandatory and adds one
	// by default.  This means that the following call to SetMake will result
	// in the existing Make tag being SET (i.e. overwritten) in the metadata.
	testResult=writer.SetMake(buffer);
	delete buffer;
	buffer=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("SetMake failed."));
		finalResult=testResult;
		}
	// 		SET the Make tag
	//

	//
	// 		ADD the DateTime tag
	buffer=aDateTime.Alloc();
	if(buffer==NULL)
		{
		Printf(_L("Could not alloc HBufC8 for DateTime"));
		return KErrNoMemory;
		}

	// N.B.: The Exif Spec states that DateTime is a recommended but
	// not mandatory tag, therefore it will not have been created by default.
	// This means that the following call to SetDateTime will result
	// in a new DateTime tag being ADDED to the metadata.
	testResult=writer.SetDateTime(buffer);
	delete buffer;
	buffer=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("SetDateTime failed."));
		finalResult=testResult;
		}
	// 		ADD the DateTime tag
	//

	//
	// 		SET the DateTimeOriginal tag
	buffer=aDateTime.Alloc();
	if(buffer==NULL)
		{
		Printf(_L("Could not alloc HBufC8 for DateTimeOriginal"));
		return KErrNoMemory;
		}

	// N.B.: Although the Exif Spec states that DateTimeOriginal is an optional
	// (not mandatory) tag, our Exif code treats it as mandatory and adds one
	// by default.  This means that the following call to SetDateTimeOriginal will
	// result in the existing DateTimeOriginal tag being SET (i.e. overwritten)
	// in the metadata.
	testResult=writer.SetDateTimeOriginal(buffer);
	delete buffer;
	buffer=NULL;
	if (testResult!=KErrNone)
		{
		Printf(_L("SetDateTimeOriginal failed."));
		finalResult=testResult;
		}
	// 		SET the DateTimeOriginal tag
	//
	return finalResult;
	}

//PDEF085003: Exif Output file corrupt if JPEGInterchangeFormatLength tag not present
//
//Loops through all the images in KAnomalousIFD1Sources[]. For each one, does
//transforms for all possible combinations of options & verifies the results.
void TImage::TransformTestIFD1Anomalies(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Transforming IFD1 Anomalies test ---"));

	TFileName destFileName(KImageTestExifTransformDest);
	for (TInt i = 0;i < KNumAnomalousIFD1Sources;++i)
		{
		//Test all possible combinations of thumbnail/preserve main image
		DoTransformIFD1Anomaly(i, destFileName, ETrue, ETrue);
		DoTransformIFD1Anomaly(i, destFileName, ETrue, EFalse);
		DoTransformIFD1Anomaly(i, destFileName, EFalse, ETrue);
		DoTransformIFD1Anomaly(i, destFileName, EFalse, EFalse);
		}
	}

//Do a specific transform test on a specific source image
void TImage::DoTransformIFD1Anomaly(const TInt aSourceIndex, const TExifTransformSourceDest& aDest, const TBool aThumbnail, const TBool aPreserve)
	{
	TInt result=DoTransformTest(TFileName(KAnomalousIFD1Sources[aSourceIndex].iFilename), aDest, aThumbnail, aPreserve);
	if (result != KErrNone)
		{
		Printf(_L("Transform %d %d of image %d failed with error %d"), aThumbnail, aPreserve, aSourceIndex, result);
		TestFailed(result);
		}
	else
		{
		result=TestGetThumbnailError(aDest, aThumbnail);
		if (result != KErrNone)
			{
			Printf(_L("Verification of transform %d %d of image %d failed with error %d"), aThumbnail, aPreserve, aSourceIndex, result);
			}
		}
	}

//PDEF085003: Exif Output file corrupt if JPEGInterchangeFormatLength tag not present
//
//Loops through all the images in KAnomalousIFD1Sources[]. For each one, does
//a decode and checks that behaviour was as expected.
void TImage::DecodeTestIFD1Anomalies(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- Decoding IFD1 Anomalies test ---"));

	//Check that our decoder can handle anomalous IFD1 images
	for (TInt i = 0;i < KNumAnomalousIFD1Sources;++i)
		{
		TInt result=TestGetThumbnailError(TFileName(KAnomalousIFD1Sources[i].iFilename), KAnomalousIFD1Sources[i].iRecoverableIFD1);
		if (result != KErrNone)
			{
			Printf(_L("Decode of anomalous image %d failed with error %d"), i, result);
			}
		}

	//Check that our decoder can still handle normal IFD1s
	for (TInt i = 0;i < KNumValidJpegIFD1Sources;++i)
		{
		//We expect to find thumbnails in all of these.
		TInt result=TestGetThumbnailError(TFileName(KValidJpegIFD1Sources[i]), ETrue);
		if (result != KErrNone)
			{
			Printf(_L("Decode of normal image %d failed with error %d"), i, result);
			}
		}
	}



//DEF091418: T: Transforming a Nikon camera jpeg image returns KErrNotSupported.
//
//Does transforms for each iamge in a set with all possible options
//
void TImage::TransformTests(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- DEF091418: Transforming testing ---"));

	TFileName destFileName(KImageTestExifTransformDest);
	for (TInt i = 0;i < KNumTransformSources;++i)
		{
		//Test all possible combinations of thumbnail/preserve main image
		DoTransformTests(i, destFileName, ETrue, ETrue);
		DoTransformTests(i, destFileName, ETrue, EFalse);
		DoTransformTests(i, destFileName, EFalse, ETrue);
		DoTransformTests(i, destFileName, EFalse, EFalse);
		}
	}

//Do a specific transform test on a specific source image
void TImage::DoTransformTests(const TInt aSourceIndex, const TExifTransformSourceDest& aDest, const TBool aThumbnail, const TBool aPreserve)
	{
	TInt result=DoTransformTest(TFileName(KTransformSources[aSourceIndex]), aDest, aThumbnail, aPreserve);
	if (result != KErrNone)
		{
		Printf(_L("Transform %d %d of image %d failed with error %d"), aThumbnail, aPreserve, aSourceIndex, result);
		TestFailed(result);
		}
	else
		{
		//Test that transformed main image can be decoded
		CJPEGExifDecoder* decoder=NULL;
		TRAPD(err, decoder=CreateDecoderOnFileL(aDest.Filename()));
		if(err!=KErrNone)
			{
			Printf(_L("could not create decoder"));
			TestFailed(err);
			return;
			}
		CleanupStack::PushL(decoder);

		const TFrameInfo& frameInfo = decoder->FrameInfo();
		CFbsBitmap* bitmap= new (ELeave) CFbsBitmap;
		CleanupStack::PushL(bitmap);
		result=bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode);
		if(err!=KErrNone)
			{
			Printf(_L("could not create bitmap"));
			TestFailed(err);
			CleanupStack::PopAndDestroy(2, decoder); //bitmap
			return;
			}
	    result=DoImageDecodeL(decoder, *bitmap);
		CleanupStack::PopAndDestroy(2, decoder); //bitmap

	    if (result!=KErrNone)
	    	{
			Printf(_L("Decode of Transform %d %d of image %d failed with error %d"), aThumbnail, aPreserve, aSourceIndex, result);
			TestFailed(result);
			return;
	    	}

	    //Check the thumbnail of the transformed image.
		result=TestGetThumbnailError(aDest, aThumbnail);
		if (result != KErrNone)
			{
			//TestGetThumbnailError has called TestFailed() already.
			Printf(_L("Verification of transform %d %d of image %d failed with error %d"), aThumbnail, aPreserve, aSourceIndex, result);
			}
		}
	}

// DEF093013: T: Transform() of a jpeg of size 1536*2048 fails with KErrNoMemory

void TImage::TransformTests2(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- DEF093013: Transform() of a big jpeg fails with KErrNoMemory ---"));

	TFileName dstFileName(KImageTestExifTransformBigDest);
	TFileName srcFileName(KImageTestExifTransformBig);

	TInt result = DoTransformTest(srcFileName, dstFileName, EFalse, EFalse, EFalse, TSize(1500, 1200));
	if (result != KErrNone)
		{
		Printf(_L("Transform of image %S failed with error %d"), &srcFileName, result);
		TestFailed(result);
		}
	else
		{
		CExifVerifier* verifier = NULL;
		TRAP(result, verifier = CExifVerifier::NewL(dstFileName, CExifVerifier::EJpegFile));
		if(result!=KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(result);
			return;
			}
		// verify Ifd Offsets
		VerifyIfdOffsets(*verifier, dstFileName);
		delete verifier;
		VerifyAgainstReferenceFile(dstFileName, TFileName(KImageTestExifTransformBigRef));
		}

	}


// DEF092988: T: Aspect ratio of scaled image is maintained evenif iMaintainAspectRatio=EFalse
//
void TImage::TransformTests3(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("--- DEF092988: Do not maintain aspect ratio when requested so ---"));

	TFileName dstFileName(KImageTestExifTransformSquareDest);
	TFileName srcFileName(KImageTestExifTransformBig);

	TInt result = DoTransformTest(srcFileName, dstFileName, EFalse, EFalse, EFalse, TSize(300, 300));
	if (result != KErrNone)
		{
		Printf(_L("Transform of image %S failed with error %d"), &srcFileName, result);
		TestFailed(result);

		}
	else
		{
		CExifVerifier* verifier=NULL;
		TRAP(result, verifier=CExifVerifier::NewL(dstFileName, CExifVerifier::EJpegFile));
		if(result!=KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(result);
			return;
			}
		// verify Ifd Offsets
		VerifyIfdOffsets(*verifier, dstFileName);
		delete verifier;

		VerifyAgainstReferenceFile(dstFileName, TFileName(KImageTestExifTransformSquareRef));
		}

	return;
	}

// DEF091185: T: Exif data is added after the transform to an image which had no exif.
// INC094065: RF CImageTransform output file has multiple DHT/DQT tables

void TImage::TransformNonExifImageTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("------DEF091185: T: Exif data is added after the transform to an image which had no exif---"));
	Printf(_L("----------Transform NonExif image Tests-------------"));
	TFileName sourceFileName(KNonExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);
	//Test all possible combinations of thumbnail/preserve main image
	DoTransformNonExifImageTest(KNonExifTransformedRefImages, 0, sourceFileName, destFileName, ETrue, ETrue);
	DoTransformNonExifImageTest(KNonExifTransformedRefImages, 1, sourceFileName, destFileName, ETrue, EFalse);
	DoTransformNonExifImageTest(KNonExifTransformedRefImages, 2, sourceFileName, destFileName, EFalse, ETrue);
	DoTransformNonExifImageTest(KNonExifTransformedRefImages, 3, sourceFileName, destFileName, EFalse, EFalse);

	Printf(_L("------INC094065: RF CImageTransform output file has multiple DHT/DQT tables---"));
	Printf(_L("----------Transform NonExif image Tests-------------"));
	sourceFileName = KNonExifTransformSource2;
	destFileName = KImageTestExifTransformDest;
	//Test all possible combinations of thumbnail/preserve main image
	DoTransformNonExifImageTest(KNonExifTransformedRefImages2, 0, sourceFileName, destFileName, ETrue, ETrue);
	DoTransformNonExifImageTest(KNonExifTransformedRefImages2, 1, sourceFileName, destFileName, ETrue, EFalse);
	DoTransformNonExifImageTest(KNonExifTransformedRefImages2, 2, sourceFileName, destFileName, EFalse, ETrue);
	DoTransformNonExifImageTest(KNonExifTransformedRefImages2, 3, sourceFileName, destFileName, EFalse, EFalse);
	}

void TImage::DoTransformNonExifImageTest(const TText* const aRefImages[], const TInt aDestIndex, TFileName& aSourceFileName, TFileName& aDestFileName, const TBool aThumbNail, const TBool aPreserveMain)
	{
	CImageTransform* imageTransform=NULL;
	TRAPD(allocErr,imageTransform=PrepareImageTransformL(aSourceFileName,aDestFileName,aThumbNail,aPreserveMain));
	if (allocErr!=KErrNone)
		{
		TestFailed(allocErr);
		return;
		}
	CleanupStack::PushL(imageTransform);
	//Do the transform
	TInt err =DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if(err !=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(err);
		return;
		}
	if (aThumbNail)
		{
		// EXIF header only present if we wrote the thumbnail
		CExifVerifier* verifier=NULL;
		TRAP(err, verifier=CExifVerifier::NewL(aDestFileName, CExifVerifier::EJpegFile));
		if(err!=KErrNone)
			{
			Printf(_L("Could not create decoder"));
			TestFailed(err);
			return;
			}
		// verify Ifd Offsets
		VerifyIfdOffsets(*verifier, aDestFileName);
		delete verifier;
		}
	VerifyAgainstReferenceFile(aDestFileName, TFileName(aRefImages[aDestIndex]));
	}

//General purpose test method: Does a image transform (with given options) on the
//given source, writing the results to the given dest. Returns the result.
//This method does not set TestFailed in order to make it more general purpose.
TInt TImage::DoTransformTest(const TExifTransformSourceDest& aSource, const TExifTransformSourceDest& aDest, const TBool aThumbnail, const TBool aPreserveMain, const TBool aMaintainAspectRatio, const TSize aSize)
	{
	CImageTransform* imageTransform = NULL;
	TRAPD(err, imageTransform = PrepareImageTransformL(aSource, aDest, aThumbnail, aPreserveMain, aMaintainAspectRatio, aSize));
	if (err != KErrNone)
		{
		Printf(_L("PrepareImageTransformL failed with %d"), err);
		return err;
		}
	CleanupStack::PushL(imageTransform);

	err = DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);
	if (err != KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		}
	
	return err;
	}

// This method will attempt to create & setup a CImageTransform object with the
// given parameters.
CImageTransform* TImage::PrepareImageTransformL(const TExifTransformSourceDest& aSource, const TExifTransformSourceDest& aDest, const TBool aThumbnail,
												const TBool aPreserveMain, const TBool aMaintainAspectRatio, const TSize aDestSize,
												const TBool aIgnoreMetadataFlag)
	{
	TInt err = KErrNone;
	CImageTransform* localImageTransform = NULL;
	TRAP(err, localImageTransform = CImageTransform::NewL(iFs));
	if(err != KErrNone)
		{
		Printf(_L("Could not create imageTransform"));
		User::Leave(err);
		}

	CleanupStack::PushL(localImageTransform);

	//Set up source
	if (aSource.IsFilename())
		{
		TRAP(err, localImageTransform->SetSourceFilenameL(aSource.Filename()));
		}
	else if (aSource.IsData())
		{
		TRAP(err, localImageTransform->SetSourceDataL(aSource.Data()));
		}
	else
		{
		err = KErrArgument;
		}
	if (err!=KErrNone)
		{
		Printf(_L("Could not set the source in imageTransform"));
		User::Leave(err);
		}
	//Set up dest
	if (aDest.IsFilename())
		{
		TRAP(err, localImageTransform->SetDestFilenameL(aDest.Filename()));
		}
	else if (aDest.IsData())
		{
		TRAP(err, localImageTransform->SetDestDataL(aDest.DataPtr()));
		}
	else
		{
		err = KErrArgument;
		}
	if(err != KErrNone)
		{
		Printf(_L("Could not set the dest in imageTransform"));
		User::Leave(err);
		}


	TRAP(err, localImageTransform->SetDestSizeInPixelsL(aDestSize, aMaintainAspectRatio));
	if(err != KErrNone)
		{
		Printf(_L("Could not set the dest size in pixels imageTransform"));
		User::Leave(err);
		}
	CImageTransform::TOptions options = CImageTransform::EOptionNone;
	if (aThumbnail)
		{
		options = CImageTransform::TOptions(options | CImageTransform::EThumbnail);
		}
	if(aIgnoreMetadataFlag)
		{
		options = CImageTransform::TOptions(options | CImageTransform::EIgnoreExifMetadataProcessing);
		}
	TRAP(err, localImageTransform->SetOptionsL(options));
	if(err!=KErrNone)
		{
		Printf(_L("Could not set options on imageTransform"));
		User::Leave(err);
		}
	if (aPreserveMain)
		{
		localImageTransform->SetPreserveImageData(ETrue);
		}

	TRAP(err, localImageTransform->SetupL());
	if(err!=KErrNone)
		{
		Printf(_L("Could not do SetupL on imageTransform"));
		User::Leave(err);
		}

	CleanupStack::Pop(localImageTransform);
	return localImageTransform;
	}

// This method starts a Transform operation on the given CImageTransform and
// returns the status int of the operation.  The given CImageTransform must have
// been previously setup, or the transform will fail.
TInt TImage::DoImageTransformL(CImageTransform* aImageTransform)
	{
	ASSERT(aImageTransform);

	// launch the transform
	CActiveTransformListener* listener= new (ELeave) CActiveTransformListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener(aImageTransform);
	aImageTransform->Transform(listener->iStatus);
	CActiveScheduler::Start();

	TInt transformResult=listener->iStatus.Int();
	CleanupStack::PopAndDestroy(listener);
	return transformResult;
	}


// This method starts a Convert operation on the given CImageDecoder and updates
// the given CFbsBitmap with the result.  It returns the status int of the
// operation.
TInt TImage::DoImageDecodeL(CImageDecoder* aDecoder, CFbsBitmap& aBitmap)
	{
	ASSERT(aDecoder);

	// launch the conversion
	CActiveDecodeListener* listener= new (ELeave) CActiveDecodeListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener(aDecoder);
	aDecoder->Convert(& listener->iStatus, aBitmap);
	CActiveScheduler::Start();

	TInt decodeResult=listener->iStatus.Int();
	CleanupStack::PopAndDestroy(listener);
	return decodeResult;
	}

// This method starts a Convert operation on the given CImageEncoder using
// the given CFbsBitmap.  It returns the status int of the operation.
TInt TImage::DoImageEncode(CImageEncoder* aEncoder, CFbsBitmap& aBitmap)
	{
	ASSERT(aEncoder!=NULL);

	CActiveEncodeListener* listener=NULL;
	TRAPD(err,listener= new (ELeave) CActiveEncodeListener);
	if (err!=KErrNone)
		{
		return err;
		}

	listener->InitialiseActiveListener(aEncoder);
	aEncoder->Convert(&listener->iStatus, aBitmap);

	CActiveScheduler::Start();

	err=listener->iStatus.Int();
	delete listener;
	return err;
	}

void TImage::CreateEncoderFromReferenceBitmapFileL(const TDesC& aRefBitmapFileName, CFbsBitmap*& aBitmap, const TFileName& aDestFileName, CJPEGExifEncoder*& aEncoder)
	{
	ASSERT(aBitmap==NULL);
	ASSERT(aEncoder==NULL);

	// Load the reference image
	aBitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(aBitmap);
	User::LeaveIfError(aBitmap->Load(aRefBitmapFileName));

	// Create the encoder
	aEncoder=CreateEncoderOnFileL(aDestFileName);
	CleanupStack::Pop(aBitmap);
	}

void TImage::ReadFileIntoBufferL(const TFileName& aFileName, HBufC8*& aBuffer)
	{
	ASSERT(aBuffer==NULL);

	RFile file;
	TInt fileSize = 0;
	TInt err=file.Open(iFs,aFileName,EFileRead);
	if (err!=KErrNone)
		{
		Printf(_L("ReadFileIntoBufferLC could not open file"));
		User::Leave(err);
		}
	CleanupClosePushL(file);
	file.Size(fileSize);
	aBuffer = HBufC8::NewL(fileSize);
	TPtr8 descriptor(aBuffer->Des());
	file.Read(descriptor);
	CleanupStack::PopAndDestroy(); //file
	}


//
// CR910: ICL JPEG Support for non conformant exif data
// Test handling of partially corrupt EXIF data

// For packaging entry paramters to give to DoLoadAndCheckIfdEntriesL()
TIfdEntryDetails::TIfdEntryDetails( TUint aIfd, TUint aTag, TFormat aDataType )
	:iIfd(aIfd)
	,iTag(aTag)
	,iDataType(aDataType)
	{
	}

/**
	Tests to see if partially corrupted EXIF JPEG files are handled as
	required by CR910.
	Runs DoIgnoreBrokenIfdEntryTestL() and sets iVerdict to the result or
	to EInconclusive if an error prevented the test from running properly
	(e.g. out of memory).

	@see	DoIgnoreBrokenIfdEntryTest()

	@param	aDataType	Ignored (present for compatibility with RICLCodecStep).
	@param	aOperation	Ignored (present for compatibility with RICLCodecStep).

	@return	EPass if all tests passed.
			EFail if one or more failed.
			EInconclusive if an error prevented the test from running properly
			(e.g. out of memory).
*/
void TImage::IgnoreBrokenIfdEntryTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	// run the tests
	TVerdict verdict = EPass;
	TRAPD( err, verdict = DoIgnoreBrokenIfdEntryTestL() );
	if( err != KErrNone )
		{
		Printf(_L("Test aborted unexpectedly with error: %d"), err );
		verdict = EInconclusive;
		}

	*iVerdict = verdict;
	}

/**
	Test to verify that exif metadata is ignored when EOptionIgnoreExifMetaData is set and passed onto
	FileNewL API of the decoder.
	This also tests the case when SetImageType API is used to set image type to thumbnail, and the
	EOptionIgnoreExifMetaData flag is set. In this case, the expected result is that the API call will fail
	with result code KErrNotFound.
	Runs DoTestWithIgnoreExifMetadataFlagSetL().

	@param	aDataType	Ignored (present for compatibility with RICLCodecStep).
	@param	aOperation	Ignored (present for compatibility with RICLCodecStep).

	@return	EPass if test passed.
			EFail if test failed.
			EInconclusive if an error prevented the test from running properly
			(e.g. out of memory).
*/
void TImage::DecodeTestWithIgnoreExifMetadataFlag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	// run the tests
	TVerdict verdict = EPass;
	TInt err = KErrNone;
	TFileName decoderFile(KImageTestExifDecode);
	TRAP( err, verdict = DoTestWithIgnoreExifMetadataFlagSetL(decoderFile));
	if(err != KErrNone)
		{
		Printf(_L("DecodeTestWithIgnoreExifMetadataFlag() Test aborted unexpectedly with error: %d"), err );
		verdict = EInconclusive;
		}
	*iVerdict = verdict;
	}

/**
	Test to verify that exif metadata is ignored when EIgnoreExifMetadataProcessing is set and passed onto
	CImageTransform using SetOptionsL() API of the CImageTransform.
	Runs DoTransformTestWithIgnoreExifMetadataFlagL().
	DoTransformTestWithIgnoreExifMetadataFlagL tests the following use cases:
	1. Transform a jpeg image with EXIF data, with EIgnoreExifMetadataProcessing flag set. The result is verified
	by checking the metadata is null, after the transform, and when the decoder is constructed on the destination file.
	2. Transform a jpeg image with EXIF data, with EIgnoreExifMetadataProcessing flag set.
	Uses the descriptor data for the transform instead of the file, the results are verified in the same way as first use-case.
	3. Transform a jpeg image with no EXIF data present. The results are verified in the same way as first use-case.
	4. Transform a jpeg image with corrupt EXIF data. The destination should contain the same EXIF data, but the tests should pass.
	5. Transform a jpeg image with EXIF data and with both EIgnoreExifMetadataProcessing and EThumbnail set. The behaviour should be
		as if the flag EIgnoreExifMetadataProcessing was NOT set (the EThumbnail cancels the effect of EIgnoreExifMetadataProcessing
	@param	aDataType	Ignored (present for compatibility with RICLCodecStep).
	@param	aOperation	Ignored (present for compatibility with RICLCodecStep).

	@return	EPass if test passed.
			EFail if test failed.
			EInconclusive if an error prevented the test from running properly
			(e.g. out of memory).
*/
void TImage::TransformWithIgnoreExifMetadataFlag(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	// run the tests
	TVerdict verdict = EPass;
	TRAPD( err, verdict = DoTransformTestWithIgnoreExifMetadataFlagL() );
	if( err != KErrNone )
		{
		Printf(_L("TransformWithIgnoreExifMetadataFlag() Test aborted unexpectedly with error: %d"), err );
		verdict = EInconclusive;
		}

	*iVerdict = verdict;
	}

/**
	Tests to see if partially corrupted EXIF JPEG files are handled as
	required by CR910.
	It loads various test images where individual EXIF IFD entries or
	offsets are corrupted. The expected behaviour is that the broken
	entries are not present in the decoded EXIF meta data, but that the
	remaining un-corrupted entries are present.
	Detailed results of each test will be output to the log.

	@see	KTestImageExifIfd0MakeTypeWrong
			KTestImageExifIfd0MakeOffsetWrong
			KTestImageExifIfd0OffsetToIfd1Wrong
			KTestImageExifIfd0OffsetToIfd1TooBig
			KTestImageExifExifIfdColSpaceWrongType
			KTestImageExifExifIfdOffsetToIopWrong

	@return	EPass if all tests passed.
			EFail if one or more failed.

	@leave	KErrNoMemory
*/
TVerdict TImage::DoIgnoreBrokenIfdEntryTestL()
	{
	// DoIgnoreBrokenIfdEntryTestL for the test images
	RArray<TIfdEntryDetails> expectedEntries;
	CleanupClosePushL( expectedEntries );
	RArray<TIfdEntryDetails> unexpectedEntries;
	CleanupClosePushL( unexpectedEntries );


	// KTestImageExifIfd0MakeTypeWrong & KTestImageExifIfd0MakeOffsetWrong
	// Make entry in IFD0 should not be present
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x0112, EUnsignedShort ) ); // entry that follows make
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x011a, EUnsignedRational ) );
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x8769, EUnsignedLong ) ); // last entry in ifd0 (exif offset)

	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x010f, EAscii ) ); // make entry

	TVerdict verdict = DoLoadAndCheckIfdEntriesL
				(
				KTestImageExifIfd0MakeTypeWrong,
				expectedEntries,
				unexpectedEntries
				);

	TVerdict tmp = DoLoadAndCheckIfdEntriesL
				(
				KTestImageExifIfd0MakeOffsetWrong,
				expectedEntries,
				unexpectedEntries
				);
	if( verdict == EPass )
		{
		// if a previous test failed we don't want to lose that result
		verdict = tmp;
		}

	expectedEntries.Reset();
	unexpectedEntries.Reset();


	// KTestImageExifIfd0OffsetToIfd1Wrong & KTestImageExifIfd0OffsetToIfd1TooBig
	// IFD1 should not be present
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x010f, EAscii ) ); // 1st IFD0 entry
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x8769, EUnsignedLong ) ); // last IFD0 entry

	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdOne, 0x0103, EUnsignedShort ) ); // 1st IFD1 entry
	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdOne, 0x0202, EUnsignedLong ) ); // last IFD1 entry

	tmp = DoLoadAndCheckIfdEntriesL
				(
				KTestImageExifIfd0OffsetToIfd1Wrong,
				expectedEntries,
				unexpectedEntries
				);
	if( verdict == EPass )
		{
		verdict = tmp;
		}

	tmp = DoLoadAndCheckIfdEntriesL
				(
				KTestImageExifIfd0OffsetToIfd1TooBig,
				expectedEntries,
				unexpectedEntries
				);
	if( verdict == EPass )
		{
		verdict = tmp;
		}

	expectedEntries.Reset();
	unexpectedEntries.Reset();


	// KTestImageExifExifIfdColSpaceWrongType
	// EXIF IFD should be present, but Color Space entry should be missing
	// Note: EXIF and InterOp sub-IFDs are considered part of IFD0 externally, so
	// the IFD parameter is KExifIfdZero below.
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x9000, EUndefined ) ); // 1st EXIF IFD entry
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0xa005, EUnsignedLong ) ); // last EXIF IFD entry

	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0xa001, EUnsignedShort ) ); // color space entry

	tmp = DoLoadAndCheckIfdEntriesL
				(
				KTestImageExifExifIfdColSpaceWrongType,
				expectedEntries,
				unexpectedEntries
				);
	if( verdict == EPass )
		{
		verdict = tmp;
		}

	expectedEntries.Reset();
	unexpectedEntries.Reset();


	// KTestImageExifExifIfdOffsetToIopWrong
	// Interoperability IFD should be missing because offset to it in EXIF IFD is wrong
	// Note: EXIF and InterOp sub-IFDs are considered part of IFD0 externally, so
	// the IFD parameter is KExifIfdZero below.
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x9000, EUndefined ) ); // 1st EXIF IFD entry
	expectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0xa003, EUnsignedShort ) ); // normally penultimate EXIF IFD entry (but will be last in this case)

	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0xa005, EUnsignedLong ) ); // color space entry
	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x0001, EAscii ) ); // 1st Interoperability entry
	unexpectedEntries.AppendL( TIfdEntryDetails( KExifIfdZero, 0x0002, EUndefined ) ); // last Interoperability entry

	tmp = DoLoadAndCheckIfdEntriesL
				(
				KTestImageExifExifIfdOffsetToIopWrong,
				expectedEntries,
				unexpectedEntries
				);
	if( verdict == EPass )
		{
		verdict = tmp;
		}

	expectedEntries.Reset();
	unexpectedEntries.Reset();


	// cleanup
	CleanupStack::PopAndDestroy( 2 ); // expectedEntries & unexpectedEntries

	return verdict;
	}

/**
	Creates a decoder for a given JPEG EXIF file and checks the EXIF data for the
	presence of expected and/or unexpected tags.
	Detailed results of each tag check will be output to the test log.

	@param	aSourceFileName			The name of the JPEG EXIF file to load
	@param	aExpectedPresentTags	A list of tags that should be present in the
									EXIF data. If one or more are not present EFail
									will be returned.
	@param	aExpectedMissingTags	A list of tags that should NOT be present in the
									EXIF data. If one or more are present EFail will
									be returned.

	@return EPass / EFail depending on the tests.

	@leave	KErrNoMemory
			KErrArgument (if EXIF decoder could not be created)
*/
TVerdict TImage::DoLoadAndCheckIfdEntriesL( const TDesC& aSourceFileName, const RArray<TIfdEntryDetails>& aExpectedPresentTags, const RArray<TIfdEntryDetails>& aExpectedMissingTags )
	{
	TVerdict testResult = EPass;

	// open the given JPEG file and extract the EXIF data
	Printf(_L("Opening decoder on file: %S"), &aSourceFileName );
	CJPEGExifDecoder* decoder = CreateDecoderOnFileL(aSourceFileName);
	CleanupStack::PushL( decoder );

	Printf(_L("Creating EXIF accessor...") );
	MExifMetadata* metaData = decoder->ExifMetadata(); // NOT owned by us so no need to push onto cleanup stack
	if( metaData == NULL )
		{
		User::Leave(KErrArgument); // no exif data present - probably wrong file
		}

	// these IFD entries should be present
	TInt entries = aExpectedPresentTags.Count();
	TInt result = KErrNone;
	for( TInt i = 0; i < entries; ++i )
		{
		TIfdEntryDetails entry = aExpectedPresentTags[ i ];
		// try to get this entry
		result = GetIfdEntryL( entry, metaData );
		if( result != KErrNone )
			{
			Printf(_L("- Could not get expected entry (ifd%d, tag 0x%04x, format %d). Error: %d"), entry.iIfd, entry.iTag, entry.iDataType, result );
			testResult = EFail;
			}
		else
			{
			Printf(_L("+ Found expected entry (ifd%d, tag 0x%04x, format %d)"), entry.iIfd, entry.iTag, entry.iDataType );
			}
		}

	// these IFD entries should NOT be present
	entries = aExpectedMissingTags.Count();
	for( TInt i = 0; i < entries; ++i )
		{
		TIfdEntryDetails entry = aExpectedMissingTags[ i ];
		// try to get this entry
		result = GetIfdEntryL( entry, metaData );
		if( result == KErrNotFound )
			{
			Printf(_L("+ Did not find unexpected entry (ifd%d, tag 0x%04x, format %d)"), entry.iIfd, entry.iTag, entry.iDataType );
			}
		else
			{
			Printf(_L("- Found unexpected entry (ifd%d, tag 0x%04x, format %d). Error: %d"), entry.iIfd, entry.iTag, entry.iDataType, result );
			testResult = EFail;
			}
		}

	// cleanup
	CleanupStack::PopAndDestroy( decoder );

	return testResult;
	}

/**
	Attempts to fetch a given entry's value from an IFD.
	The value itself is ignored, this function is purely to test
	for the presence or absence of a particular entry.

	@param	aEntry		The entry to get
	@param	aMetaData	The EXIF data accessor

	@return	KErrNone 		if the entry was present
			KErrNotFound	if the entry exists
			KErrArgument	if the entry does not have the expected format

	@leave	KErrNoMemory
*/
TInt TImage::GetIfdEntryL( TIfdEntryDetails aEntry, MExifMetadata* aMetaData )
	{
	TInt result = KErrNone;

	switch( aEntry.iDataType )
		{
		case EByte:
		case EAscii:
		case EUndefined:
			{
			// get 8-bit data
			// length of this HBufC8 doesn't matter since GetParam8 will replace
			// this with a new HBufC8 anyway
			HBufC8* tmp = HBufC8::NewL( 1 );
			result = aMetaData->GetParam8( aEntry.iTag, aEntry.iIfd, tmp );
			delete tmp;
			break;
			}

		case EUnsignedShort:
			{
			// get short array
			// again, initial size of array doesn't matter since it gets resized anyway
			CArrayFixFlat<TUint16>* tmp = new(ELeave) CArrayFixFlat<TUint16>(1);
			result = aMetaData->GetShortArrayParam( aEntry.iTag, aEntry.iIfd, *tmp );
			delete tmp;
			break;
			}

		case EUnsignedLong:
		case ESignedLong:
			{
			// get integer array
			CArrayFixFlat<TInt>* tmp = new(ELeave) CArrayFixFlat<TInt>(1);
			result = aMetaData->GetIntegerArrayParam( aEntry.iTag, aEntry.iIfd, *tmp );
			delete tmp;
			break;
			}

		case EUnsignedRational:
		case ESignedRational:
			{
			// rational array
			CArrayFixFlat<TRational>* tmp = new(ELeave) CArrayFixFlat<TRational>(1);
			result = aMetaData->GetRationalArrayParam( aEntry.iTag, aEntry.iIfd, *tmp );
			delete tmp;
			break;
			}

		default:
			result = KErrArgument;
			break;
		}

		return result;
	}

// CRPCUG-6SSBQ7: ICL CImageDecoder / CImageTransform option to skip EXIF metadata decoding.
/**
@see DecodeTestWithIgnoreExifMetadataFlag
*/
TVerdict TImage::DoTestWithIgnoreExifMetadataFlagSetL(TFileName fileName)
	{
	TInt error = 0;
	TVerdict testResult = EPass;
	CJPEGExifDecoder* decoder = NULL;
	//TFileName fileName(KTestImageExifIfd0MakeTypeWrong);
	CImageDecoder::TOptions decoderOptions =
		CImageDecoder::TOptions(DecoderOptions() | CImageDecoder::EOptionIgnoreExifMetaData);
	// intitialize the decoder
if(iDecoderUid.iUid != 0)
	{
#if defined(__ICL_PROFILING)
	RDebug::ProfileStart(1);
	TRAPD(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, fileName,
		decoderOptions, KImageTypeJPGUid, KNullUid, iDecoderUid)));
	RDebug::ProfileEnd(1);
	#else
		TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, fileName,
			decoderOptions , KImageTypeJPGUid, KNullUid, iDecoderUid)));
		if(error != KErrNone)
			{
			User::LeaveIfError(error);
			}
	#endif //defined(__ICL_PROFILING)	
	}
else
	{
	#if defined(__ICL_PROFILING)
	RDebug::ProfileStart(1);
	TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, fileName,
		decoderOptions, KImageTypeJPGUid)));
	RDebug::ProfileEnd(1);
#else
		TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, fileName,
		decoderOptions , KImageTypeJPGUid)));
	if(error != KErrNone)
		{
		User::LeaveIfError(error);
		}
#endif //defined(__ICL_PROFILING)
	}
	// The following code tests if we have no meta-data (due to EOptionIgnoreExifMetaData set),
	// then setting Image Type to thumbnail must fail with code KErrNotFound.
	TRAP(error, decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail));
	if(error != KErrNotFound)
		{
		delete decoder;
		return EFail;
		}
	// Test if the decoder returns null pointer when asked for metadata.
	MExifMetadata* metaData=decoder->ExifMetadata();
	if(metaData == NULL)
		{
		testResult = EPass;
		}
	else
		{
		testResult = EFail;
		}
	delete decoder;

	return testResult;
	}

// CRPCUG-6SSBQ7: ICL CImageDecoder / CImageTransform option to skip EXIF metadata decoding.
/**
@see TransformWithIgnoreExifMetadataFlag
*/
TVerdict TImage::DoTransformTestWithIgnoreExifMetadataFlagL()
	{
	TVerdict testResult = EPass;
	Printf(_L("--- Transform With "));

	TFileName sourceFileName(KImageTestExifTransformSource);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform=NULL;
	TRAPD(creationError,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,EFalse,
		EFalse, ETrue, KThumbnailSize, /*aIgnoreMetadataFlag=*/ETrue));
	if (creationError!=KErrNone)
		{
		testResult = EFail;
		Printf(_L("Failed in PrepareImageTransformL(from file): DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	testResult = TransformAndVerifyIgnoreExifMetadataFlagL(imageTransform, destFileName, EFalse);
	if(testResult != EPass) return testResult; // return error, do not continue.
	//
 	// The second part of the test is same as first, but this time, the descriptor src is transformed instead
 	// of the file soure.
 	//

	//read the source file into a descriptor
	HBufC8* sourceFileBuffer = NULL;
	ReadFileIntoBufferL(sourceFileName, sourceFileBuffer);
	CleanupStack::PushL(sourceFileBuffer);
	TPtr8 sourceDescriptor(sourceFileBuffer->Des());
	//		Prepare the image transform
	TRAP(creationError, imageTransform = PrepareImageTransformL(sourceDescriptor, destFileName, EFalse,
		EFalse, ETrue, KThumbnailSize, /*aIgnoreMetadataFlag=*/ETrue));
	if (creationError!=KErrNone)
		{
		testResult = EFail;
		Printf(_L("Failed in PrepareImageTransformL(from descriptor): DoTransformTestWithIgnoreExifMetadataFlagL"));
		CleanupStack::PopAndDestroy(sourceFileBuffer);
		return testResult;
		}
	testResult = TransformAndVerifyIgnoreExifMetadataFlagL(imageTransform, destFileName, EFalse);
	CleanupStack::PopAndDestroy(sourceFileBuffer);
	if(testResult != EPass) return testResult; // return error, do not continue.
	//
 	// The third part of the test is same as first, but this time, the src does not contain any EXIF meta-data
 	//
 	sourceFileName = (KNonExifTransformSource);
 	imageTransform=NULL;
	TRAP(creationError,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,EFalse,
		EFalse, ETrue, KThumbnailSize, /*aIgnoreMetadataFlag=*/ETrue));
	if (creationError!=KErrNone)
		{
		testResult = EFail;
		Printf(_L("Failed in PrepareImageTransformL(from file): DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	testResult = TransformAndVerifyIgnoreExifMetadataFlagL(imageTransform, destFileName, ETrue);
	if(testResult != EPass)
		{
		return testResult; // return error, do not continue.
		}
	//
 	// The fourth part of the test. The src contains incorrect/unreadable EXIF meta-data.
 	// The destination should also contain the same incorrect/unreadable EXIF meta-data.
 	//
 	sourceFileName = (KImageCorruptTestNo42TagInExif);
 	imageTransform=NULL;
	TRAP(creationError,imageTransform=PrepareImageTransformL(sourceFileName,destFileName,EFalse,
		EFalse, ETrue, KThumbnailSize, /*aIgnoreMetadataFlag=*/ETrue));
	if (creationError!=KErrNone)
		{
		testResult = EFail;
		Printf(_L("Failed in PrepareImageTransformL(from file): DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	testResult = TransformAndVerifyIgnoreExifMetadataFlagL(imageTransform, destFileName, EFalse, ETrue);
	if(testResult != EPass)
		{
		Printf(_L("Failed to verify corrupt data is written as is when EIgnoreExifMetadataProcessing flag is set: DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	//
	// The fifth part of the test. Both EIgnoreExifMetadataProcessing and EThumbnail flags are set.
	// The expected result is that EIgnoreExifMetadataProcessing will not have any effect.
	// This is tested by seeing that the transform should contain valid EXIF meta-data.
	//
	sourceFileName = (KImageTestExifTransformSource);
 	imageTransform=NULL;
	TRAP(creationError,imageTransform=PrepareImageTransformL(sourceFileName, destFileName, ETrue,
		EFalse, ETrue, KThumbnailSize, /*aIgnoreMetadataFlag=*/ETrue));
	if (creationError!=KErrNone)
		{
		testResult = EFail;
		Printf(_L("Failed in PrepareImageTransformL(from file): DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	CleanupStack::PushL(imageTransform);
	TInt transformResult=DoImageTransformL(imageTransform);
	if(transformResult != KErrNone)
		{
		testResult = EFail;
		Printf(_L("Failed to transform when both flags (EThumbnail and EIgnoreExifMetadataProcessing) are set: DoTransformTestWithIgnoreExifMetadataFlagL"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testResult;
		}
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	MExifMetadata* metaData = extension->ExifMetadata();
	if(metaData == NULL)
		{
		testResult = EFail;
		Printf(_L("Failed to verify EThumbnail takes precedence over EIgnoreExifMetadataProcessing flag: DoTransformTestWithIgnoreExifMetadataFlagL"));
		CleanupStack::PopAndDestroy(imageTransform);
		return testResult;
		}
	CleanupStack::PopAndDestroy(imageTransform);
	return testResult;
	}

// Helper function for DoTransformTestWithIgnoreExifMetadataFlagL
TVerdict TImage::TransformAndVerifyIgnoreExifMetadataFlagL(CImageTransform* aImageTransform, TFileName& aDestFile,
	TBool aExpectNullDecoderMetadata, TBool aDecoderCrashTest /* = EFalse (default) */)
	{
	TVerdict testResult = EPass;
	TInt error = KErrNone;
	CleanupStack::PushL(aImageTransform);
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(aImageTransform->Extension());
	MExifMetadata* metaData = extension->ExifMetadata();
	if(metaData != NULL)
		{
		testResult = EFail;
		Printf(_L("ExifMetadata should have returned null: DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	TInt transformResult=DoImageTransformL(aImageTransform);
	metaData=extension->ExifMetadata();
	if(metaData != NULL)
		{
		testResult = EFail;
		Printf(_L("ExifMetadata should have returned null: DoTransformTestWithIgnoreExifMetadataFlagL"));
		return testResult;
		}
	//		verify the result
	if(transformResult!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image : DoTransformTestWithIgnoreExifMetadataFlagL"));
		testResult = EFail;
		return testResult;
		}
	CleanupStack::PopAndDestroy(aImageTransform); // neccessary for decoder to be created.
	// open the resulting file
	CJPEGExifDecoder* decoder=NULL;
	if(aDecoderCrashTest)
		{
		CImageDecoder::TOptions decoderOptions = CImageDecoder::TOptions(DecoderOptions());
		// intitialize the decoder
#if defined(__ICL_PROFILING)
		RDebug::ProfileStart(1);
		TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aDestFile,
			decoderOptions, KImageTypeJPGUid)));
		RDebug::ProfileEnd(1);
#else
		TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, aDestFile,
			decoderOptions , KImageTypeJPGUid)));
#endif //defined(__ICL_PROFILING)
		delete decoder;
		if(error == KErrNone)
			{
			testResult = EFail;
			Printf(_L("Decoder creation should have failed, but didn't: DoTransformTestWithIgnoreExifMetadataFlagL"));
			}
		else
			{
			decoderOptions = CImageDecoder::TOptions(DecoderOptions() |  CImageDecoder::EOptionIgnoreExifMetaData);
			// intitialize the decoder
#if defined(__ICL_PROFILING)
			RDebug::ProfileStart(1);
			TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aDestFile,
				decoderOptions, KImageTypeJPGUid)));
			RDebug::ProfileEnd(1);
#else
			TRAP(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, aDestFile,
				decoderOptions , KImageTypeJPGUid)));
#endif //defined(__ICL_PROFILING)
			if(error != KErrNone)
				{
				testResult = EFail;
				Printf(_L("Decoder creation should not have failed, but it did: DoTransformTestWithIgnoreExifMetadataFlagL"));
				}
			else
				{
				// verify the data
				metaData=decoder->ExifMetadata();
				if((metaData != NULL) && aExpectNullDecoderMetadata)
					{
					testResult = EFail;
					Printf(_L("ExifMetadata should have returned null: DoTransformTestWithIgnoreExifMetadataFlagL"));
					}
				}
			delete decoder;
			}
		}
	else
		{
		CImageDecoder::TOptions decoderOptions = CImageDecoder::TOptions(DecoderOptions());
		// intitialize the decoder
#if defined(__ICL_PROFILING)
		RDebug::ProfileStart(1);
		TRAPD(error, decoder = STATIC_CAST(CJPEGExifDecoder*, CJPEGExifDecoder::FileNewL(iFs, aDestFile,
			decoderOptions, KImageTypeJPGUid)));
		RDebug::ProfileEnd(1);
		User::LeaveIfError(error);
#else
		decoder = STATIC_CAST(CJPEGExifDecoder*, CImageDecoder::FileNewL(iFs, aDestFile,
			decoderOptions , KImageTypeJPGUid));
#endif //defined(__ICL_PROFILING)
		if(decoder == NULL)
			{
			Printf(_L("Could not create decoder : DoTransformTestWithIgnoreExifMetadataFlagL"));
			testResult = EFail;
			return testResult;
			}
		// verify the data
		metaData=decoder->ExifMetadata();
		if((metaData != NULL) && aExpectNullDecoderMetadata)
			{
			testResult = EFail;
			Printf(_L("ExifMetadata should have returned null: DoTransformTestWithIgnoreExifMetadataFlagL"));
			}
		else if ((metaData == NULL) && !aExpectNullDecoderMetadata)
			{
			testResult = EFail;
			Printf(_L("ExifMetadata should have been there, but isn't: DoTransformTestWithIgnoreExifMetadataFlagL"));
			}
		// destroy the decoder
		delete decoder;
		}
	return testResult;
	}

//
// INC101733: Behaviour of Exif transform with successive invalid tags is not as expected
// Test loading an image with successive entries with unknown tags in IFD0. The EXIF code should
// now consider this IFD corrupt and since it is IFD0 there should be no EXIF data available at
// all.

/**
	Tests to see if an IFD containing successive unknown tags is considered
	currput.
	Runs DoIfdWithUnknownTagsTestL() and sets iVerdict to the result or
	to EInconclusive if an error prevented the test from running properly
	(e.g. out of memory).

	@param	aDataType	Ignored (present for compatibility with RICLCodecStep).
	@param	aOperation	Ignored (present for compatibility with RICLCodecStep).
*/
void TImage::IfdWithUnknownTagsTest(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	// run the tests
	TVerdict verdict = EPass;
	TRAPD( err, verdict = DoIfdWithUnknownTagsTestL() );
	if( err != KErrNone )
		{
		Printf(_L("Test aborted unexpectedly with error: %d"), err );
		verdict = EInconclusive;
		}

	*iVerdict = verdict;
	}


/**
	Tests to see if an IFD containing successive unknown tags is considered
	currput.
	An EXIF JPEG, whose IFD0 contains 12 successive entries with unknown tags is
	decoded. The EXIF decoder should consider this IFD corrupt and exclude it from the
	decoded EXIF data. Since this is the 0th IFD there should be no decoded EXIF data
	at all in this case. This test checks that this is the case and fails if it is not.

	@see	KTestImageExifIfd0UnknownTags


	@return	EPass if no EXIF data is decoded.
			EFail if some EXIF data is decoded.

	@leave	KErrNoMemory
*/
TVerdict TImage::DoIfdWithUnknownTagsTestL()
{
	TVerdict result = EFail;

	CJPEGExifDecoder* decoder = CreateDecoderOnFileL( TFileName(KTestImageExifIfd0UnknownTags) );
	MExifMetadata* exifData = decoder->ExifMetadata();

	if( !exifData )
	{
		result = EPass;
		Printf(_L("IFD0 rejected - test passed."));
	}
	else
	{
		Printf(_L("IFD0 not rejected - test failed."));
		Printf(_L("NOTE: If you have modified the value of SYMBIAN_ICL_EXIF_CONSEC_INVALID_TAGS in the ExifLibStatic.mmp file and made it >12 then this test failure can be ignored."));
		// can't check the value of SYMBIAN_ICL_EXIF_CONSEC_INVALID_TAGS
		// or KMaxNumberOfConsecInvalidEntries here unfortunately since they
		// are internal to the Exif LIB.
	}

	delete decoder;
	return result;
}

//
// DEF101880: JPEG Encoder may produce distorted images
//


/**
	Decodes an image to an CImageFrame, encodes that to a file with JPEG quality set to 75% and then compares
	the new file against a reference file.
	If the files match the test passes otherwise it fails. The output file that gets generated by this test
	step is deleted at the end of the test.

	@param	aDataType	Ignored (present for compatibility with RICLCodecStep).
	@param	aOperation	Ignored (present for compatibility with RICLCodecStep).
 */
void TImage::TestJpgDistortedAfterEncodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{

	__MM_HEAP_MARK;
	TFileName refFileName = TFileName(KTestImageJpegDistorted);
	//
	// Decode to an image frame //
	//
	CJPEGImageFrameDecoder* decoder = NULL;
	TRAPD(error,
		decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::FileNewL(iFs, refFileName, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(error);
		User::Leave(KErrGeneral);
		}
	CleanupStack::PushL(decoder);

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(error);
		User::Leave(KErrGeneral);
		}

	TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
	User::LeaveIfError(res);

  	CleanupClosePushL(chunk);

	// create an empty imageframe
	CImageFrame* theImageFrame = NULL;
	TRAPD(createErr, theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset));
	if (createErr != KErrNone)
		{
		Printf(_L("Could not create imageFrame"));
		if (theImageFrame)
			{
			delete theImageFrame;
			}
		TestFailed(createErr);
		User::Leave(KErrGeneral);
		}
	CleanupStack::PushL(theImageFrame);

	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(decodelistener);
	decodelistener->InitialiseActiveListener(decoder);

	decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);

	CActiveScheduler::Start();

	const TInt err1 = decodelistener->iStatus.Int();

	if (err1 != KErrNone)
		{
		Printf(_L("failed to decode image (%S)"), &KTestImageJpegDistorted);
		TestFailed(err1);
		}

	CleanupStack::PopAndDestroy(1, decodelistener);
	delete decoder;

	//
	// Encode from an image frame //
	//
	TFileName destFileName = TFileName(KTestImageJpegDistortedEncoded);
	CJPEGImageFrameEncoder* encoder = NULL;
	TRAP(error,
		encoder = static_cast<CJPEGImageFrameEncoder*>( CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid));
		);
	if (error != KErrNone)
		{
		Printf(_L("Could not create encoder"));
		TestFailed(error);
		User::Leave(KErrGeneral);
		}

	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
	encodeListener->InitialiseActiveListener(encoder);


	// set output quality to 75
	CFrameImageData* imgData = CFrameImageData::NewL();
	CleanupStack::PushL( imgData );

	TJpegImageData* jpegData = new (ELeave) TJpegImageData;
	jpegData->iSampleScheme = TJpegImageData::EColor420;
	jpegData->iQualityFactor= 75;
	imgData->AppendImageData( jpegData );


	encoder->ConvertFrame(&encodeListener->iStatus, *theImageFrame, imgData);

	CActiveScheduler::Start();

	const TInt err2 = encodeListener->iStatus.Int();
	if (err2 != KErrNone)
		{
		Printf(_L("failed to encode image (%S)"), &KTestImageJpegDistortedEncoded);
		TestFailed(err2);
		}
	else
		{
		VerifyAgainstReferenceFile(destFileName, TFileName(KTestImageJpegDistortedReference));
		}

	CleanupStack::PopAndDestroy(5, decoder);

	// delete output file
	// won't leave if this fails since it is not critical
	RFs fs;
	if( (fs.Connect() == KErrNone ) && (fs.Delete(destFileName) == KErrNone) )
	{
		Printf(_L("Successfully deleted temporary output image."));
	}
	else
	{
		Printf(_L("Could not delete temporary output image %S."), &destFileName);
	}
	fs.Close();

	__MM_HEAP_MARKEND;

	}

//
// Test for:
//		PDEF110735: JPEG Exif IFDs with 0 entries can cause invalid JPEG
//

void TImage::CheckMandatoryEntriesInSubExif(CJPEGExifDecoder* aDecoder, TBool aCheckThumbnail)
	{
	_LIT8(KExifVersionDefault, "0220");
	_LIT8(KComponentsConfigurationDefault, "1230");
	_LIT8(KFlashPixVersionDefault, "0100");

	HBufC8* buf = NULL;
	TInt err;

	TExifReaderUtility utility(aDecoder->ExifMetadata());

	err = utility.GetExifVersion(buf);
	if ((err != KErrNone) || (buf->Compare(KExifVersionDefault)) )
		{
		Printf(_L("Incorrect ExifVersionId"));
		TestFailed(err);
		return;
		}
	delete buf;
	buf = NULL;

	err = utility.GetFlashPixVersion(buf);
	if ((err != KErrNone) || (buf->Compare(KFlashPixVersionDefault)))
		{
		Printf(_L("Incorrect Flash Pix Version Id"));
		TestFailed(err);
		return;
		}
	delete buf;
	buf = NULL;

	err = utility.GetComponentsConfiguration(buf);
	if ((err != KErrNone) || (buf->Compare(KComponentsConfigurationDefault)))
		{
		Printf(_L("Incorrect Components Configuration Id"));
		TestFailed(err);
		return;
		}
	delete buf;
	buf = NULL;

	TInt value = NULL;
	TSize imageSize = aDecoder->FrameInfo().iFrameCoordsInPixels.Size();
	err = utility.GetPixelXDimension(value);
	if ((err != KErrNone) || (value != imageSize.iWidth))
		{
		Printf(_L("Incorrect Width size"));
		TestFailed(err);
		return;
		}
	err = utility.GetPixelYDimension(value);
	if ((err != KErrNone) || (value != imageSize.iHeight))
		{
		Printf(_L("Incorrect Height size"));
		TestFailed(err);
		return;
		}
	if (aCheckThumbnail)
		{
		TUint16 res = NULL;
		err = utility.GetThumbCompression(res);
		if (err != KErrNone)
			{
			Printf(_L("Problem getting a mandatory tag in IFD1"));
			TestFailed(err);
			return;
			}
		}
	}

void TImage::GenericTransformDescSrc (	const TFileName& aSourceFileName,
										const TFileName& aDestFileName,
										const TBool aThumbnail,
										const TBool aPreserveMain,
										const TBool aMaintainAspectRatio,
										const TSize aSize,
										const TBool aIgnoreMetadataFlag)
	{
	TInt err;
	CImageTransform* imageTransform=NULL;
	TRAP(err,imageTransform=PrepareImageTransformL(aSourceFileName,
												   aDestFileName,
												   aThumbnail,
												   aPreserveMain,
												   aMaintainAspectRatio,
												   aSize,
												   aIgnoreMetadataFlag));
	if (err!=KErrNone)
		{
		TestFailed(err);
		return;
		}
	TRAP(err, DoImageTransformL(imageTransform));
	if(err!=KErrNone)
		{
		Printf(_L("An error occured while transforming the image"));
		TestFailed(err);
		delete imageTransform;
		imageTransform = NULL;
		return;
		}
	delete imageTransform;
	imageTransform = NULL;
	}

void TImage::IFDNoEntries(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	__MM_HEAP_MARK;
	TInt err;
	Printf(_L("PDEF110735: JPEG Exif IFDs with 0 entries can cause invalid JPEG file:%S"), &KExifTag0000);
	TFileName sourceFileName(KExifTag0000);
	TFileName sourceFileName2(KExifInteropTag0000);
	TFileName sourceFileName3(KExifTag0000NoGPSTransform);
	TFileName destFileName(KImageTestExifTransformDest);
	TFileName destFileName2(KImageTestExifTransformDest22);
	TFileName destFileName2Reference(KExifTag0000NoInteropTransformReference);
	TFileName destFileName3(KImageTestExifTransformDest33);
	TFileName destFileName3Reference(KExifTag0000NoGPSTransformReference);	
	CJPEGExifDecoder* decoder = NULL;

	Printf(_L("test Preserving Image"));
	// test : no thumbnail, preserving image, maintain aspect ratio, not ignoring metadata
	GenericTransformDescSrc(sourceFileName, destFileName, EFalse, ETrue);
	TRAP(err, decoder=CreateDecoderOnFileL(destFileName));
	if(err!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(err);
		return;
		}
	CheckMandatoryEntriesInSubExif(decoder);
	delete decoder;
	decoder = NULL;

	Printf(_L("test Preserving Image with thumbnail"));
	GenericTransformDescSrc(sourceFileName, destFileName, ETrue, ETrue);
	TRAP(err, decoder=CreateDecoderOnFileL(destFileName));
	if(err!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(err);
		return;
		}
	CheckMandatoryEntriesInSubExif(decoder, ETrue);
	delete decoder;
	decoder = NULL;

	Printf(_L("test not Preserving Image"));
	GenericTransformDescSrc(sourceFileName, destFileName, EFalse, EFalse);
	TRAP(err, decoder=CreateDecoderOnFileL(destFileName));
	if(err!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(err);
		return;
		}
	CheckMandatoryEntriesInSubExif(decoder);
	delete decoder;
	decoder = NULL;

	Printf(_L("test not Preserving Image with thumbnail"));
	GenericTransformDescSrc(sourceFileName, destFileName, ETrue, EFalse);
	TRAP(err, decoder=CreateDecoderOnFileL(destFileName));
	if(err!=KErrNone)
		{
		Printf(_L("Could not create decoder"));
		TestFailed(err);
		return;
		}
	CheckMandatoryEntriesInSubExif(decoder, ETrue);
	delete decoder;
	decoder = NULL;

//	Testing a transform on an Image with a Null Interoperability pointer tag
	
	Printf(_L("test Preserving Image with 0 Interoperability tags"));
	GenericTransformDescSrc(sourceFileName2, destFileName2, ETrue, ETrue);
	VerifyAgainstReferenceFile(destFileName2,destFileName2Reference);

//	Testing a transform on an Image with a Null GPS pointer tag
	
	Printf(_L("test Preserving Image with 0 GPS tags"));
	GenericTransformDescSrc(sourceFileName3, destFileName3, EFalse, ETrue);
	VerifyAgainstReferenceFile(destFileName3,destFileName3Reference);
	
	REComSession::FinalClose();	
	RFbsSession::Disconnect();
	__MM_HEAP_MARKEND;
	}

//
// Test for:
//		PDEF110735: JPEG Exif IFDs with 0 entries can cause invalid JPEG
//


//
// DEF110487: Decoding jpeg file with height of 17 pixels to image frame buffer errors with -9
//
// Attempts to decode jpegs of various sizes first to a descriptor and then to a RChunk, both wrapped by a CImageFrame object. Tests 
// several jpegs with heights and widths one above and one below the MCU block size (16x16 for images with 4:2:0 sampling ratio).

void TImage::TestJpegWithVariousFileSizesL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n"));
	Printf(_L("DEF110487: Decoding jpeg file with height of 17 pixels to image frame buffer errors with -9"));
	
	CFbsBitmap* aBitmap = new (ELeave) CFbsBitmap();
	CActiveDecodeListener* decodelistener = new (ELeave) CActiveDecodeListener();

	TRAPD(error, DoTestJpegDescWithVariousFileSizesL(aBitmap, decodelistener));
	if (error != KErrNone)
		{
		Printf(_L("Could not decode image using descriptors"));
		TestFailed(error);
		delete aBitmap;
		delete decodelistener;
		return;	
		}

	TRAP(error, DoTestJpegChunkWithVariousFileSizesL(aBitmap, decodelistener));
	if (error != KErrNone)
		{
		Printf(_L("Could not decode image using Chunks"));
		TestFailed(error);
		delete aBitmap;
		delete decodelistener;
		return;	
		}
	
	delete aBitmap;
	delete decodelistener;	
	}

void TImage::DoTestJpegDescWithVariousFileSizesL(CFbsBitmap* aBitmap, CActiveDecodeListener* decodelistener)
	{
	TInt i = 0;
	TInt j = 0;

	User::LeaveIfError(aBitmap->Create(TSize(1, 1), EColor16M));

	for (i = 641; i > 0; ((i-1)%KMCUBlockSize == 0) ? i-=2 : i-=14)
		{
		for (j = 641; j > 0; ((j-1)%KMCUBlockSize == 0) ? j-=2 : j-=14)
			{
			__MM_HEAP_MARK;

			TSize jpegSize(i, j);
			
			User::LeaveIfError(aBitmap->Resize(jpegSize));
			
			HBufC8* abuf = NULL;
			
			CImageEncoder* encoder = CImageEncoder::DataNewL(abuf, EncoderOptions(), KJpegImageTypeUid);

			TInt error = DoImageEncode(encoder, *aBitmap);
			delete encoder;
			
			User::LeaveIfError(error);
		
			TPtrC8 abufptr(abuf->Des());
			
			//
			// Decode to an image frame //
			//
			CJPEGImageFrameDecoder* decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::DataNewL(iFs, abufptr, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));;
			CleanupStack::PushL(decoder);

			TInt imageSizeInBytes;
			TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
			if (supported == EFalse)
				{
				User::Leave(KErrNotSupported);
				}
	
			HBufC8* buffer = HBufC8::NewMaxL(imageSizeInBytes);
			CleanupStack::PushL(buffer);
		  	TPtr8 ptr = buffer->Des();

			// create an empty imageframe
			CImageFrame* theImageFrame = CImageFrame::NewL(ptr, imageSizeInBytes);
			CleanupStack::PushL(theImageFrame);

			decodelistener->InitialiseActiveListener(decoder);

			decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);
			CleanupStack::PushL(abuf);
			
			CActiveScheduler::Start();

			User::LeaveIfError(decodelistener->iStatus.Int());

			CleanupStack::PopAndDestroy(4, decoder);
			}
		__MM_HEAP_MARKEND;
		}
	}

void TImage::DoTestJpegChunkWithVariousFileSizesL(CFbsBitmap* aBitmap, CActiveDecodeListener* decodelistener)
	{
	TInt i = 0;
	TInt j = 0;
	
	for (i = 641; i > 0; ((i-1)%KMCUBlockSize == 0) ? i-=2 : i-=14)
		{
		for (j = 641; j > 0; ((j-1)%KMCUBlockSize == 0) ? j-=2 : j-=14)
			{
			__MM_HEAP_MARK;

			TSize jpegSize(i, j);
			
			User::LeaveIfError(aBitmap->Create(jpegSize, EColor16M));
			
			HBufC8* abuf = NULL;
			
			CImageEncoder* encoder = CImageEncoder::DataNewL(abuf, EncoderOptions(), KJpegImageTypeUid);

			TInt error = DoImageEncode(encoder, *aBitmap);
			delete encoder;
			
			User::LeaveIfError(error);
			
			TPtrC8 abufptr(abuf->Des());
			//
			// Decode to an image frame //
			//
			CJPEGImageFrameDecoder* decoder = static_cast<CJPEGImageFrameDecoder*>( CImageDecoder::DataNewL(iFs, abufptr, DecoderOptions(), KNullUid, KNullUid, KImageFramePluginUid));;
			CleanupStack::PushL(decoder);

			// create destination RChunk
			RChunk chunk;
			TInt imageSizeInBytes;
			TBool supported = decoder->RecommendedBufferSize(imageSizeInBytes);
			if (supported == EFalse)
				{
				User::Leave(KErrNotSupported);
				}

			TInt res = chunk.CreateGlobal(_L("aChunkImageFrame"), imageSizeInBytes, imageSizeInBytes, EOwnerProcess);
			User::LeaveIfError(res);

		  	CleanupClosePushL(chunk);

			// create an empty imageframe
			CImageFrame* theImageFrame = CImageFrame::NewL(&chunk, imageSizeInBytes, KChunkDataOffset);
			CleanupStack::PushL(theImageFrame);

			decodelistener->InitialiseActiveListener(decoder);

			decoder->ConvertFrame(&decodelistener->iStatus, *theImageFrame);
			CleanupStack::PushL(abuf);
			
			CActiveScheduler::Start();

			User::LeaveIfError(decodelistener->iStatus.Int());

			CleanupStack::PopAndDestroy(4, decoder);
			}
		__MM_HEAP_MARKEND;
		}
	}

//CR1329
//Encodes a exif image. Checks if the default tag values for device information are from SysUtil API.
void TImage::CheckDeviceInfoTagsOnEncodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	Printf(_L("\n CR1329 test with encode"));

	//	Load the source bitmap
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Load(KEncodeSourceFileName);
	if(err != KErrNone)
		{
		Printf(_L("could not load ref bitmap file:%S error:%d"), &KEncodeSourceFileName, err);
		User::Leave(err);
		}

	//	Get the exif metadata
	TFileName fileName(KImageTestExifEncode);
	CJPEGExifEncoder* encoder=NULL;
	TRAP(err, encoder=CreateEncoderOnFileL(fileName));
	if(err != KErrNone)
		{
		Printf(_L("Could not create encoder; error: %d file:%S"), err, &KImageTestExifEncode );
		User::Leave(err);
		}
	CleanupStack::PushL(encoder);

	// launch the conversion; it is enough that the exif contains mandatory tags in IFD0
	CActiveEncodeListener* listener= NULL;
	listener= new (ELeave) CActiveEncodeListener;
	listener->InitialiseActiveListener(encoder);
	encoder->Convert(& listener->iStatus, *bitmap);

	CActiveScheduler::Start();

	// destroy the encoder
	delete listener;
	CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder

	// verify the result; open the resulting file
	CExifVerifier* verifier=NULL;
	TRAP(err, verifier=CExifVerifier::NewL(fileName, CExifVerifier::EJpegFile));
	if(err != KErrNone)
		{
		Printf(_L("Could not create decoder; error: %d"), err);
		TestFailed(err);
		return;
		}
	CleanupStack::PushL(verifier);
	// Get metadata
	MExifMetadata* metaData = verifier->ExifMetadata();
	if(metaData == NULL)
		{
		Printf(_L("Could not get MExifMetaData on the output image"));
		User::Leave(KErrGeneral);
		}
	//expected values; should be from SysUtil
	HBufC8* manufacture = NULL;
	HBufC8* model = NULL;
	HBufC8* uiPlatform = NULL;
	GetDeviceInfoFromSysUtilL(manufacture, model, uiPlatform);
	//the optional tag UI platform will not be present in the image.
	TBool result = VerifyDeviceInfoTags(metaData, manufacture->Des(), model->Des(), TPtr8(NULL, 0));
	CleanupStack::PopAndDestroy(verifier);
	delete manufacture;
	delete model;
	delete uiPlatform;
	// log the result
	if(!result)
		{
		TestFailed(KErrGeneral);
		}
	}

//Trasforms exif image. Checks if the default tag values for device information are from SysUtil API.
void TImage::CheckDeviceInfoTagsOnTransformL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	Printf(_L("\n"));
	Printf(_L("\n CR1329 test with Transform"));
	TInt err = KErrNone;
	TFileName sourceFileName(KImageTestJfif422_2MP);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform = NULL;
	TRAP(err, imageTransform = PrepareImageTransformL(sourceFileName, destFileName, EFalse, ETrue));
	if (err != KErrNone)
		{
		TestFailed(err);
		return;
		}
	CleanupStack::PushL(imageTransform);
	//set options so to result in Exif image; note that source image is JFIF.
	imageTransform->SetOptionsL(CImageTransform::EThumbnail);
	// encode the image
	TInt transformResult = DoImageTransformL(imageTransform);
	if (transformResult != KErrNone)
		{
		Printf(_L("An error occurred while transforming the image; error: %d"), transformResult);
		User::Leave(transformResult);
		}
	CleanupStack::PopAndDestroy(imageTransform);
	
	// verify the result; open the resulting file
	CExifVerifier* verifier=NULL;
	TRAP(err, verifier=CExifVerifier::NewL(destFileName, CExifVerifier::EJpegFile));
	if(err!=KErrNone || verifier == NULL)
		{
		Printf(_L("Could not create decoder; error: %d file:%S"), err, &KImageTestExifTransformDest);
		TestFailed(err);
		return;
		}
	CleanupStack::PushL(verifier);
	// Get metadata
	MExifMetadata* metaData=verifier->ExifMetadata();
	if(metaData == NULL)
		{
		Printf(_L("To verify the transform - cannot extract MExifMetadata from verifier"));
		User::Leave(KErrGeneral);
		}
	//expected values; should be from SysUtil
	HBufC8* manufacture = NULL;
	HBufC8* model = NULL;
	HBufC8* uiPlatform = NULL;
	GetDeviceInfoFromSysUtilL(manufacture, model, uiPlatform);
	//the optional tag UI platform will not be present in the image.
	TBool result = VerifyDeviceInfoTags(metaData, manufacture->Des(), model->Des(), TPtr8(NULL, 0));
	delete manufacture;
	delete model;
	delete uiPlatform;
	CleanupStack::PopAndDestroy(verifier);
	
	// log the result
	if(!result)
		{
		TestFailed(KErrGeneral);
		}
	}

//Checks for validity of all relevant tags - manufacuter, model and UI platform. Even if one is wrong, 
//the function checks for others.
TBool TImage::VerifyDeviceInfoTags(MExifMetadata* aExifMetadata, TPtr8 aManufacturer, TPtr8 aModel, TPtr8 aUIPlatform)
	{
	TBool result = ETrue;
	TInt err = KErrNone;
	HBufC8* param = NULL;
	const TUint KTag010F = 0x010F; //manufacturer
	const TUint KTag0110 = 0x110; //model
	const TUint KTag0131 = 0x0131; //UI platform
	
	//check for manufacturer name
	err = aExifMetadata->GetParam8(KTag010F, 0, param);
	if (err != KErrNone || param == NULL)
		{
		Printf(_L("Getting Manufacturer name failed %d"), err);
		result = EFalse;
		}
	else if(param->Des() != aManufacturer)
		{
		Printf(_L("Manufacturer doesn't match"));
		result = EFalse;					
		}
	param = NULL;
	
	//check for model name
	err = aExifMetadata->GetParam8(KTag0110, 0, param);
	if (err != KErrNone || param == NULL)
		{
		Printf(_L("Getting model name failed %d"), err);
		result = EFalse;
		}
	else if(param->Des() != aModel) 
		{
		Printf(_L("Model name doesn't match"));
		result = EFalse;					
		}		
	param = NULL;
	
	//check for UI Platform
	if(aUIPlatform.Ptr() != NULL)
		{
		err = aExifMetadata->GetParam8(KTag0131, 0, param);
		if (err != KErrNone || param == NULL)
			{
			Printf(_L("Getting UI platform failed %d"), err);
			result = EFalse;
			}
		else if(param->Des() != aUIPlatform) 
			{
			Printf(_L("UI platform doesn't match"));
			result = EFalse;					
			}		
		}
	return result;
	}

//Encodes exif image. Alters the default tag values of Device information. Checks if the resultant image's tag
// values match with set values.
void TImage::CheckDeviceInfoTagsOnModifyEncodeL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	Printf(_L("\n CR1329 test with modify and encode"));

	//	Load the source bitmap
	CFbsBitmap* bitmap=new (ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	TInt err = bitmap->Load(KEncodeSourceFileName);
	if(err != KErrNone)
		{
		Printf(_L("could not load ref bitmap file - %S error:%d"), &KEncodeSourceFileName, err);
		User::Leave(err);
		}

	//	Get the exif metadata
	TFileName fileName(KImageTestExifEncode);
	CJPEGExifEncoder* encoder=NULL;
	TRAP(err, encoder=CreateEncoderOnFileL(fileName));
	if(err != KErrNone)
		{
		Printf(_L("Could not create encoder; error: %d"), err);
		TestFailed(err);
		CleanupStack::PopAndDestroy(bitmap);
		return;
		}
	CleanupStack::PushL(encoder);

	//modify the default values of the tag - manufacturer, model and UI platform
	MExifMetadata* metaData = encoder->ExifMetadata();
	TUint8 KTestManufacturerName[] = {"SymbianTstManufact"}; //note: is null
	TInt stringLength = sizeof(KTestManufacturerName)/sizeof(TUint8)-1;//avoid the null character
	TPtr8 testManufacturer(KTestManufacturerName, stringLength, stringLength);
	TUint8 KTestModelName[] = {"SymbianTstModel"};
	stringLength = sizeof(KTestModelName)/sizeof(TUint8)-1; //avoid the null character
	TPtr8 testModel(KTestModelName, stringLength, stringLength);
	TUint8 KTestUI[] = {"SymbianTstUI"};
	stringLength = sizeof(KTestUI)/sizeof(TUint8)-1; //avoid the null character
	TPtr8 testUI(KTestUI, stringLength, stringLength);
	
	AlterDeviceInfoTagsL(metaData, testManufacturer, testModel, testUI);
	
	// launch the conversion;
	CActiveEncodeListener* listener= NULL;
	listener= new (ELeave) CActiveEncodeListener;
	listener->InitialiseActiveListener(encoder);
	encoder->Convert(& listener->iStatus, *bitmap);

	CActiveScheduler::Start();

	// destroy the encoder
	delete listener;
	CleanupStack::PopAndDestroy(2, bitmap);	// bitmap, encoder

	// verify the result
	// open the resulting file
	CExifVerifier* verifier=NULL;
	TRAP(err, verifier=CExifVerifier::NewL(fileName, CExifVerifier::EJpegFile));
	if(err != KErrNone)
		{
		Printf(_L("Could not create decoder; error:%d"), err);
		TestFailed(err);
		return;
		}
	
	// Get metadata
	metaData = verifier->ExifMetadata();
	//verify
	TBool result = VerifyDeviceInfoTags(metaData, testManufacturer, testModel, testUI);
	// destroy the verifier
	delete verifier;

	// log the result
	if(!result)
		{
		TestFailed(KErrGeneral);
		}
	}

//Transforms exif image. Alters the default tag values of Device information. Checks if the resultant image's tag
// values match with set values.
void TImage::CheckDeviceInfoTagsOnModifyTransformL(const TExifDataType& /*aDataType*/, const TExifOperation& /* aOperation */)
	{
	Printf(_L("\n CR1329 test Transform with modification of Deviceinfo tags"));
	TInt err = KErrNone;
	TFileName sourceFileName(KImageTestJfif422_2MP);
	TFileName destFileName(KImageTestExifTransformDest);

	CImageTransform* imageTransform = NULL;
	TRAP(err, imageTransform = PrepareImageTransformL(sourceFileName, destFileName, EFalse, ETrue));
	if (err != KErrNone)
		{
		TestFailed(err);
		return;
		}
	CleanupStack::PushL(imageTransform);
	//the source image is Jfif; hence to generate exif, set options.
	imageTransform->SetOptionsL(CImageTransform::EThumbnail);
	
	//get MExifmetadata to alter the deviceinfo tag values
	CJPEGExifTransformExtension* extension = static_cast<CJPEGExifTransformExtension*>(imageTransform->Extension());
	if(extension == NULL)
		{
		Printf(_L("\n Unable to get extension from CImageTransform"));
		User::Leave(KErrGeneral);
		}
	MExifMetadata* metaData=extension->ExifMetadata();
	if(metaData == NULL)
		{
		Printf(_L("\n Unable to get MExifMetadata from extesion"));
		User::Leave(KErrGeneral);
		}
	TUint8 KTestManufacturerName[] = {"SymbianTstManufact"}; //note: is null
	TInt stringLength = sizeof(KTestManufacturerName)/sizeof(TUint8)-1;//avoid the null character
	TPtr8 testManufacturer(KTestManufacturerName, stringLength, stringLength);
	TUint8 KTestModelName[] = {"SymbianTstModel"};
	stringLength = sizeof(KTestModelName)/sizeof(TUint8)-1; //avoid the null character
	TPtr8 testModel(KTestModelName, stringLength, stringLength);
	TUint8 KTestUI[] = {"SymbianTstUI"};
	stringLength = sizeof(KTestUI)/sizeof(TUint8)-1; //avoid the null character
	TPtr8 testUI(KTestUI, stringLength, stringLength);
	
	AlterDeviceInfoTagsL(metaData, testManufacturer, testModel, testUI);
	
	// encode the image
	TInt transformResult = DoImageTransformL(imageTransform);
	CleanupStack::PopAndDestroy(imageTransform);

	if (transformResult != KErrNone)
		{
		Printf(_L("An error occurred while transforming the image"));
		TestFailed(transformResult);
		return;
		}

	// verify the result; open the resulting file
	CExifVerifier* verifier=NULL;
	TRAP(err, verifier=CExifVerifier::NewL(destFileName, CExifVerifier::EJpegFile));
	if(err!=KErrNone)
		{
		Printf(_L("Could not create decoder: error: %d file:%S"), err, &KImageTestExifTransformDest);
		TestFailed(err);
		return;
		}
	CleanupStack::PushL(verifier);
	// Get metadata
	metaData = verifier->ExifMetadata();
	if(metaData == NULL)
		{
		Printf(_L("while verifying can't get MExifMetadata* for the output image"));
		User::Leave(KErrGeneral);
		}
	//verify
	TBool result = VerifyDeviceInfoTags(metaData, testManufacturer, testModel, testUI);
	
	// destroy the verifier
	CleanupStack::PopAndDestroy(verifier);

	// log the result
	if(!result)
		{
		TestFailed(KErrGeneral);
		}
	}

void TImage::AlterDeviceInfoTagsL(MExifMetadata* aExifMetadata, TPtr8& aManufacuter, TPtr8& aModel, TPtr8& aUIPlatform)
	{
	const TUint KTag010F = 0x010F; //manufacturer
	const TUint KTag0110 = 0x110; //model
	const TUint KTag0131 = 0x0131; //UI platform
	
	HBufC8* buffer=aManufacuter.AllocL();
	//modify the default manufacturer name
	TInt err = aExifMetadata->SetParam8(KTag010F, 0, buffer);
	delete buffer;
	buffer = NULL;
	if(err != KErrNone)
		{
		Printf(_L("Could not set manufacturer name on encoding; error:%d"), err);
		User::Leave(err);
		}
	
	buffer = aModel.AllocL();
	//modify the default model name
	err = aExifMetadata->SetParam8(KTag0110, 0, buffer);
	delete buffer;
	buffer = NULL;
	if(err != KErrNone)
		{
		Printf(_L("Could not set model name on encoding; error:%d"), err);
		User::Leave(err);
		}

	buffer = aUIPlatform.AllocL();
	//modify the default UI platform
	err = aExifMetadata->SetParam8(KTag0131, 0, buffer);
	delete buffer;
	buffer = NULL;
	if(err != KErrNone)
		{
		Printf(_L("Could not set UIplatform name on encoding; error:%d"), err);
		User::Leave(err);
		}
	}

//gets device information from SysUtil API. The HBufC8* memory in arguments is to be released by the client.
void TImage::GetDeviceInfoFromSysUtilL(HBufC8*& aManufacturer, HBufC8*& aModel, HBufC8*& aUIPlatform)
	{
	CDeviceTypeInformation* deviceAtts = NULL;
	aManufacturer = NULL;
	aModel = NULL;
	aUIPlatform = NULL;
	TInt err;
	deviceAtts = SysUtil::GetDeviceTypeInfoL();

	TPtrC16 ptr;
	//manufacturer
	err = deviceAtts->GetManufacturerName(ptr);
	if((err != KErrNone) && (err != CDeviceTypeInformation::KDefaultValue) && (err != KErrOverflow))
		{
		Printf(_L("Error in reading Device manufacturer from SysUtil; error:%d"), err);
		User::Leave(err);
		}

	aManufacturer = HBufC8::NewMaxL(ptr.Length() + 1);
	CleanupStack::PushL(aManufacturer);
	TPtr8 ptr8_man = aManufacturer->Des();
	ptr8_man.Copy(ptr);


	//model
	err = deviceAtts->GetModelName(ptr);
	if((err != KErrNone) && (err != CDeviceTypeInformation::KDefaultValue) && (err != KErrOverflow))
		{
		Printf(_L("Error in reading Device Model from SysUtil; error:%d"),err);
		User::Leave(err);
		}

	aModel = HBufC8::NewMaxL(ptr.Length() + 1);
	CleanupStack::PushL(aModel);
	TPtr8 ptr8_model = aModel->Des();
	ptr8_model.Copy(ptr);

	//UI platform
	err = deviceAtts->GetUIPlatformName(ptr); //ignore error
	if((err != KErrNone) && (err != CDeviceTypeInformation::KDefaultValue) && (err != KErrOverflow))
		{
		Printf(_L("Error in reading UIPlatform from SysUtil; error:%d"), err);
		User::Leave(err);
		}

	aUIPlatform = HBufC8::NewMaxL(ptr.Length() + 1);
	TPtr8 ptr8_UI = aUIPlatform->Des();
	ptr8_UI.Copy(ptr);
	
	CleanupStack::Pop(2); //pop aModel and aManufacturer
	}

//Transform a corrupt exif image with Preserve data option.
void TImage::TransformWithPreserveL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{
	Printf(_L("\n PDEF128742:Image transform can not handle illegal exif when original image is to preserved "));
	TInt err = KErrNone;
	_LIT(KSource, "c:\\TSU_ICL_cod_04\\Inc128220_source.jpg");
	TFileName sourceFileName(KSource);
	TFileName destFileName(KImageTestExifTransformDest);
	_LIT(KReference, "c:\\TSU_ICL_cod_04\\Inc128220_ref.jpg");
	TFileName refFileName(KReference);
	CImageTransform* imageTransform = NULL;
	TRAP(err, imageTransform = PrepareImageTransformL(sourceFileName, destFileName, EFalse, ETrue));
	if (err != KErrNone)
		{
		TestFailed(err);
		return;
		}
	CleanupStack::PushL(imageTransform);

	// encode the image
	TInt transformResult = DoImageTransformL(imageTransform);
	if (transformResult != KErrNone)
		{
		Printf(_L("An error occurred while transforming the image; error: %d"), transformResult);
		User::Leave(transformResult);
		}
	CleanupStack::PopAndDestroy(imageTransform);
	
	VerifyAgainstReferenceFile(destFileName, refFileName );
	}
