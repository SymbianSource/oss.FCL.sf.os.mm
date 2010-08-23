// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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

//#include <fbs.h>
#include <imagetransform.h>
#include <hal.h>
#include "../../testcodec/inc/TestUids.hrh"
#include "ICLCodecDefectTestStep.h"
#include "ICLCodecDefectTestSuite.h"
#include "ICLFbsSessionTest.h"
#include <bitdev.h>
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "testiclresolver.hrh"

_LIT(KCorruptPngFile,"png_pal1024.png");

_LIT(KGif0x25,"gif0x25.gif");
const TInt KGif0x25Width = 0;
const TInt KGif0x25Height = 25;

_LIT(KGif25x0,"gif25x0.gif");
const TInt KGif25x0Width = 25;
const TInt KGif25x0Height = 0;

_LIT(KGifPalette,"gifPalette.gif");
_LIT(KGifPaletteRef,"gifPalette.mbm");
const TInt KGifPaletteFrameNo = 1;

_LIT(KInterlacedGif,"Clinton.gif");
_LIT(KInterlacedGifRef,"Clinton.mbm");
const TInt KInterlacedGifFrameNo = 2;

_LIT(KAnimatedGif,"AnmatCat.gif");
_LIT(KAnimatedGifRef,"AnmatCat.mbm");
const TInt KAnimatedGifFrameNo = 0;

_LIT(KAnimatedGifZeroDelay,"__scale__1_444834624.gif");
const TInt KAnimatedGifZeroDelaySize = 90;
_LIT(KCorruptWMFFile,"corrupt_set_window_ext.wmf");

_LIT(KExtensionGif,"am475.gif");
_LIT(KExtensionGifRef,"am475.mbm");
_LIT(KExtensionGifHeaderComment1,"Header Comment 1 Block 0 Block 1");
_LIT(KExtensionGifHeaderComment2,"Header Comment 2 Block 0 Block 1");
_LIT(KExtensionGifFooterComment1,"Footer Comment 1 Block 0 Block 1");
_LIT(KExtensionGifFooterComment2,"Footer Comment 2 Block 0 Block 1");
const TInt KExtensionGifNoOfComments = 4;
const TInt KExtensionGifFrameNo = 0;

_LIT(KRestartJpeg,"Restart0.jpg");
_LIT(KRestartJpegRef,"Restart0.mbm");
const TInt KRestartJpegFrameNo = 0;

_LIT(KResizeInterlacGif,"Interlac.gif");
_LIT(KResizeInterlacGifRef,"InterlacResize.mbm");
const TInt KResizeInterlacGifSize = 7118;
const TInt KResizeInterlacGifFrameNo = 0;
const TInt KResizeInterlacGifZoom = 2;

_LIT(KUserInputGif,"UserInput.gif");
const TInt KUserInputGifFrameNo = 0;
_LIT(K7IterationsAnimatedGif,"mathmouse.gif"); 
_LIT(KCancelGif,"Cancel.gif");
_LIT(KCancelGifRef,"Cancel.mbm");
const TInt KCancelGifFrameNo = 0;
const TInt KCancelGifZoomFactor = 2;

_LIT(KTstExtension,".tst");
_LIT8(KTstMimeType,"image/tst");
_LIT8(KTestMimeType,"image/test");
_LIT(KTstDisplayName,"TST");
_LIT(KJpgExtension,".jpg");
_LIT8(KJpgMimeType,"image/jpg");
_LIT8(KJpegMimeType,"image/jpeg");
_LIT(KJPEGDisplayName,"JPEG");
const TUid KTestImageType = { 0x101F7BF7 };
const TUid KTestImageSubType = { 0x101F7BF8 };
const TUid KTestDecoderUid = { 0x101F7BF6 };
const TUid KTestFailDecoderUid = { 0x101F7C08 };
const TUid KTestEncoderUid = { 0x101F7C01 };
const TUid KExtendedTestDecoder = { 0x101F7C04 };
const TUid KExtendedTestEncoder = { 0x101F7C05 };
const TUid KJPGImageType = { 0x101F45D8 };
const TUid KJPGImageSubType = { 0x00000000 };
const TUid KJPGDecoderUid = { 0x101F45D7 };

_LIT(KCorruptBmp,"Corrupt.bmp");
_LIT(KTruncatedBmp,"Truncated.bmp");
_LIT(KCompressionType3Bmp, "cat.bmp");

_LIT(KCommentJpg,"comment.jpg");
_LIT(KCommentJpgRef,"comment.mbm");
_LIT(KCommentJpgComment1,"First Comment");
_LIT(KCommentJpgComment2,"Second Comment");
const TInt KCommentJpgNoOfComments = 2;
const TInt KCommentJpgFrameNo = 0;

_LIT(KAdobeMonoJpg,"AdobeMono.jpg");
_LIT(KAdobeMonoJpgRef,"AdobeMono.mbm");
const TInt KAdobeMonoJpgFrameNo = 0;

_LIT(KJFIFJpg,"NGImage1.jpg");
_LIT(KJFIFJpgRef,"NGImage1.mbm");
_LIT(KRGBJpg,"NGImage2.jpg");
_LIT(KYCCJpg,"NGImage3.jpg");

_LIT(KDecodeBmp,"bmp1bpp.bmp");
_LIT(KDecodeBmpRef,"bmp1bpp.mbm");

_LIT(KOS2v1HeaderBmp,"os2v1header.bmp");
_LIT(KOS2v1HeaderBmpRef,"os2v1header.mbm");
_LIT(KOS2v1Depth1Bmp,"valuesv1d1.bmp");
_LIT(KOS2v1Depth4Bmp,"valuesv1d4.bmp");
_LIT(KOS2v1Depth8Bmp,"valuesv1d8.bmp");
_LIT(KOS2v1Depth24Bmp,"valuesv1d24.bmp");
_LIT(KOS2v2Depth1Bmp,"valuesv2d1.bmp");
_LIT(KOS2v2Depth4Bmp,"valuesv2d4.bmp");
_LIT(KOS2v2Depth8Bmp,"valuesv2d8.bmp");
_LIT(KOS2v2Depth24Bmp,"valuesv2d24.bmp");
_LIT(KOS2Depth1BmpRef,"valuesd1.mbm");
_LIT(KOS2Depth4BmpRef,"valuesd4.mbm");
_LIT(KOS2Depth8BmpRef,"valuesd8.mbm");
_LIT(KOS2Depth24BmpRef,"valuesd24.mbm");
_LIT(KWINv3Depth24withPaletteBmp, "winv3depth24withpalettebmp.bmp");
_LIT(KWINv3Depth24withPaletteBmpRef, "winv3depth24withpalettebmp.mbm");

_LIT(KCorruptGif, "Corrupt.gif");

_LIT(KGif, "Anibdog.gif");
_LIT(KGifRef, "Anibdog.mbm");

_LIT(KReductionFactorTestGif,"RFTest.gif");
_LIT(KWithEXIFHead,"ExifHeadFile.png");

_LIT(KCorruptJpeg287, "00000287.jpg");
_LIT(KCorruptJpeg290, "00000290.jpg");

_LIT(KWhiteBlockBRCornerJpg, "Image(078).jpg");
_LIT(KWhiteBlockBRCornerJpgRef, "Image(078).mbm");

_LIT(KPeace0Mask, "ref_mask_peace.mbm");
_LIT(KPeace0Image, "ref_frame_peace.mbm");
_LIT(KPeaceGifImage, "peace.gif");
const TInt KGifFrameNo = 0;

_LIT(KNotCorruptGif, "240_320_xy_15_15.gif");
_LIT(KNotCorruptGifRef1, "240_320_xy_15_15(1).mbm");
_LIT(KNotCorruptGifRef2, "240_320_xy_15_15(2).mbm");
_LIT(KNotCorruptGifRef3, "240_320_xy_15_15(3).mbm");

_LIT(KJFIFWithThumbnailMbm,"jfifthumb.mbm");
_LIT(KJFIFWithThumbnailRef,"jfifthumbref.jpg");
_LIT(KJFIFWithThumbnail,"jfifthumb.jpg");

_LIT(KErrArgumentGif, "151_181_xy_7_7.gif");
_LIT(KErrArgumentThumbNailImage, "ref_nail_peace.mbm");

_LIT(KFirstFrameBiggerThanScreenSizeGif, "144_174_overx_15_15.gif");

_LIT(KReductionFactorTestWfm, "WMF\\DibPatternBrush.wmf");

_LIT(KExtraBitsAfterEOIJpg,"Kuva(33).jpg");
_LIT(KCorruptExifJpg, "JPEG.jpg");
_LIT(KCommentMarkerExifJpg, "050521_1142~0.jpg");
_LIT(KBadThumbnailOffsetSizeExifJpg1, "MISS0001.JPG");
_LIT(KBadThumbnailOffsetSizeExifJpg2, "MISS0006.JPG");
_LIT(KBadThumbnailOffsetSizeExifJpg3, "MISS0007.JPG");
_LIT(KUncompleteImageJpg,"1359694_[JPG]_Broken_01.JPG");

_LIT(KSetCommentsLimitSizeGif, "00001290.gif");

_LIT(KWrongCommentBlock1,"WrongCommentBlock1.jpg"); // Wrong comment block, only signature and length, no content.
_LIT(KWrongCommentBlock2,"WrongCommentBlock2.jpg"); // Original file provided in INC76787 - One extra byte
_LIT(KWrongCommentBlock3,"WrongCommentBlock3.jpg"); // 10 extra bytes in comment block
_LIT(KCorruptSizeMultibyteWBmp,"MultibyteCurruptSize.wbmp");
_LIT(KSmallWBmp, "dib1.wbmp");

_LIT(KLzwStreamCorruptGif,"Waterdrop.gif");
_LIT(KCorruptGifFirstFrameRef,"ref_waterdrop_firstframe.mbm");
const TInt KCorruptLzwGifFrameNo = 0;

_LIT(KSmallFirstFrameGif, "640X480.gif");
_LIT(KSmallFirstFrameGifRef, "640X480.mbm");
_LIT(KSmallFirstFrameGifSizeRef, "160x120_5080.mbm");
_LIT(KSmallFirstFrameGifHalfSizeRef, "80x60_5080.mbm");
_LIT(KFrameGifSizeAnim, "anim_640x480_5080.gif");
_LIT(KFrameGifSizeRefAnim1, "anim_1_160x120_5080.mbm");
_LIT(KFrameGifSizeRefAnim2, "anim_2_160x120_5080.mbm");
_LIT(KFrameGifSizeRefAnim3, "anim_3_160x120_5080.mbm");

_LIT(KCorruptRLEBitmap, "rle8-corrupt.bmp");
_LIT(KClockZeroSubBlockGif, "clock-zero-sub-block.gif");
_LIT(KClockZeroSubBlockMbm, "clock-zero-sub-block.mbm");

_LIT(KBadOffSetGif,"STIL1002.GIF");
_LIT(KBadOffSetGifRef,"STIL1002.mbm");
_LIT(KCorruptIFDExifJpg, "qvga.jpg");

// CR910: ICL JPEG Support for non conformant exif data
_LIT(KWrongInteropOffsetExifJpg, "f1000001-2.jpg"); // from MOAP
_LIT(KCorruptExifJpgFromMoap1, "ina3510a.jpg");
_LIT(KCorruptExifJpgFromMoap2, "image02.jpg");
_LIT(KCorruptExifJpgFromMoap3, "image10_exif.jpg");
_LIT(KCorruptExifJpgFromMoap4, "exif01.jpg");
_LIT(KGpsButNoInteropExifJpg, "input.jpg"); // from DEF089682
_LIT(KInvalidTagValueOffsetExifJpg, "moap_create_unsupported_tags_corrupt.jpg"); // from PDEF085171
_LIT(KCorruptExifIfd0EntryCountTooBig, "exif-ifd0-entry_count_too_big.jpg"); // IFD0 corrupt because entry count is too large
_LIT(KCorruptExifIfd0MakeSizeTooBig, "exif-ifd0-make_size_too_big.jpg"); // size of the 'Make' entry's value too big
_LIT(KCorruptExifIfd0OffsetTooBig, "exif-offset_to_ifd0_too_big.jpg"); // offset to IFD0 too big

_LIT(KNonDefaultPaletteGif, "gif256colours.gif");
_LIT(KNonDefaultPaletteGifOutput, "gif256colours2.gif");

_LIT(KPngCRCOnBufferBoundary, "arrow.png");

// DEF101406: Changes made in CR910 (EXIF) cause corrupt JPEG files to Panic with KERN_EXEC3
_LIT(KCorruptExifInvalidEntryType, "corrupt_jpeg01.jpg");
_LIT(KStreamFFFFJpg,"StreamFFFF.jpg");
_LIT(KRefStreamFFFFJpg,"ref_streamFFFFjpg.mbm");
_LIT(K24BppPng, "screenbackground.png");

_LIT(KProgJpgWithSeqThumb1,"p_jpg160x120.jpg");
_LIT(KProgJpgWithSeqThumb2,"p_jpg320x240.jpg");
_LIT(KProgJpgWithSeqThumb3,"p_jpg640x480.jpg");

_LIT(KZeroLenCommentBlock1, "zero-len-comment-1.gif");
_LIT(KZeroLenCommentBlock2, "zero-len-comment-2.gif");

_LIT(KSetImageTypeProblemJpg,"P999090350.jpg");

_LIT(KWmfFormatString, "mso290(%d).wmf");
_LIT(KWmfRefFormatString, "mso290(%d)_%d.mbm");
_LIT(KWmfNegWinExtension, "24bit.wmf");

//DEF109125: CJpgReadCodec::RestartStateL() logic problem 
_LIT(KBrokenJpegMissing0xff,"brokenjpgmissing0xff.jpg");
_LIT(KPngNoIEND, "error.png");					// no IEND chunk
_LIT(KPngNoIEND2, "error2.png");				// only the 4 byte length present in IEND chunk
_LIT(KPngiTXtNoIEND, "itxt.png");				// no IEND chunk, iTXt chunk after last IDAT chunk
_LIT(KPngDataChunkAlign, "datachunkalign.png");	// no IEND chunk, IDAT chunk data is ICL data block aligned
												// ie IDAT data ends at file offset 4104 (4096 plus the 8 byte png signature)
_LIT(KPerformanceEnhanceTestJPG, "211.jpg");

_LIT(KImageTestTempFileName,"c:\\imagebmptemp");
_LIT(KImageTestSourceMbm1, "testsource1.mbm");
_LIT(KImageTestSourceMbm2, "testsource2.mbm");
_LIT(KImageTestTargetJpg1, "testtarget1.jpg");
_LIT(KImageTestTargetJpg2, "testtarget2.jpg");
_LIT(KImageJpegTemp, "c:\\testjpeg.jpg");

//DEF113733: Monochrome JFIF Thumbnail
_LIT(KJFIFThumbnailBW, "jfifthumb_bw.jpg");
_LIT(KJFIFThumbnailBWRef, "jfifthumb_bw_ref.jpg");

//INC116806: Media files: Open image folder containing gif89 images results in a Main crash
_LIT(KGif89,"anim1_gif89.gif");
_LIT(KGif89RefReducedSizeMask,"anim1_gif89_Reduced_Size_Mask.mbm");

//PDEF118662: 5.0 LWS: Browser crashes while loading the page 
_LIT(KBkgd,"banner_2_bkgd.gif");

//
//CCancelScheduler
//
/*
 * Cancel a decoding process after a set interval
 */
void CCancelScheduler::WaitForAnyRequest()
	{
	User::WaitForAnyRequest();
	iCurrentCount++;
	if(iDecoder)
		{
		if(iCurrentCount == iCancelCount)
			iDecoder->Cancel();
		}
	}

void CCancelScheduler::SetDecoder(CImageDecoder& aDecoder)
	{
	iDecoder = &aDecoder;
	}

void CCancelScheduler::SetCancel(TInt aCount)
	{
	iCancelCount = aCount;
	iCurrentCount = 0;
	}

TInt CCancelScheduler::CurrentCount() const
	{
	return iCurrentCount;
	}

//
//RICLCodecDefectTestStep
//
/**
 * Add the path to the testfiles to the filename
 *
 * @param	"RFs& aFs"
 *			The file server session to use.
 * @param	"TDes& aFileName"
 *			Returns the full path for the file.
 * @param	"const TDesC& aPathNameAndExtn"
 *			The filename and extention of the file
 * @xxxx
 */
void RICLCodecDefectTestStep::GetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecDefectTestSuite*)iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();
	}

void RICLCodecDefectTestStep::SetVerdict(TVerdict aVerdict)
	{
	if(aVerdict > iTestStepResult)
		iTestStepResult = aVerdict;
	}

void RICLCodecDefectTestStep::Compare(const TDesC& aDescriptor1, const TDesC& aDescriptor2, const TDesC& aErrorMessage)
	{
	//For debug
	TPtrC ptr1;
	ptr1.Set(aDescriptor1);

	TPtrC ptr2;
	ptr2.Set(aDescriptor2);

	if(aDescriptor1.Ptr() == aDescriptor2.Ptr())
		SetFail(_L("Comparing same Pointer"));

	if(aDescriptor1 != aDescriptor2)
		SetFail(aErrorMessage);
	}

void RICLCodecDefectTestStep::Compare(const TDesC8& aDescriptor1, const TDesC8& aDescriptor2, const TDesC& aErrorMessage)
	{
	//For debug
	TPtrC8 ptr1;
	ptr1.Set(aDescriptor1);

	TPtrC8 ptr2;
	ptr2.Set(aDescriptor2);

	if(aDescriptor1.Ptr() == aDescriptor2.Ptr())
		SetFail(_L("Comparing same Pointer"));

	if(aDescriptor1 != aDescriptor2)
		SetFail(aErrorMessage);
	}

void RICLCodecDefectTestStep::ComparePointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage)
	{
	if(aPointer1 != aPointer2)
		SetFail(aErrorMessage);
	}

void RICLCodecDefectTestStep::CompareDiffPointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage)
	{
	if(aPointer1 == aPointer2)
		SetFail(aErrorMessage);
	}

void RICLCodecDefectTestStep::Compare(TInt aNumber1, TInt aNumber2, const TDesC& aErrorMessage)
	{
	if(aNumber1 != aNumber2)
		SetFail(aErrorMessage);
	}

void RICLCodecDefectTestStep::Compare(TUid aUid1, TUid aUid2, const TDesC& aErrorMessage)
	{
	if(aUid1 != aUid2)
		SetFail(aErrorMessage);
	}

void RICLCodecDefectTestStep::Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2, const TDesC& aErrorMessage)
	{

	if(aBitmap1.SizeInPixels() != aBitmap2.SizeInPixels())
		{
		SetFail(aErrorMessage);
		return;
		}

	const TSize size = aBitmap1.SizeInPixels();
	
	for (TInt y = 0; y < size.iHeight; y++)
		{
		for (TInt x = 0; x < size.iWidth; x++)
			{
			TPoint pt(x,y);
			TRgb color1,color2;
			aBitmap1.GetPixel(color1,pt);
			aBitmap2.GetPixel(color2,pt);
				
			if (color1 != color2)
				{
				SetFail(aErrorMessage);
				return;
				}
			}
		} // end of comparison for-loop
	}


TInt RICLCodecDefectTestStep::Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2)
	{
	const TSize size1 = aBitmap1.SizeInPixels();
	const TSize size2 = aBitmap2.SizeInPixels();

	if (size1 != size2)
		{
		ERR_PRINTF5(_L("Bitmaps differ in size: {%d x %d} vs {%d x %d}"),
				size1.iWidth, size1.iHeight,
				size2.iWidth, size2.iHeight);
		return KErrCorrupt;
		}

	TPoint pt(0, 0);
	for (TInt y = 0; y < size1.iHeight; y++)
		{
		for (TInt x = 0; x < size1.iWidth; x++)
			{
			pt.iX = x;
			pt.iY = y;
			
			TRgb color1, color2;
			aBitmap1.GetPixel(color1, pt);
			aBitmap2.GetPixel(color2, pt);
				
			if (color1 != color2)
				{
				ERR_PRINTF3(_L("Differing pixel at (%d, %d)"), x, y);
				return KErrCorrupt;
				}
			}
		} // end of comparison for-loop
	
	return KErrNone;
	}


void RICLCodecDefectTestStep::SetFail(const TDesC& aErrorMessage)
	{
	ERR_PRINTF1(aErrorMessage);
	SetVerdict(EFail);
	}

/**
 * Decode a file 
 * @xxxx
 */
CImageDecoder* RICLCodecDefectTestStep::DecodeFileL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber, const TInt aReductionFactor, TBool aDecodeThumbnail, CImageDecoder::TOptions aOptions)
	{
	TInt err;
	
	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	CImageDecoder* decoder = NULL;
	TRAP(err, decoder = CImageDecoder::FileNewL(aFs, aFileName, aOptions));
	if (err != KErrNone)
		{
		RDebug::Print(_L("FileNewL left with %d (%S)"), err, &aFileName);
		User::Leave(err);
		}
	CleanupStack::PushL(decoder);

	if (!aDecodeThumbnail)
		{
		// decode the main image		
		const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
		TSize reducedSize; //Size after applying reduction factor, default reduction factor is 0
		decoder->ReducedSize(frameInfo.iOverallSizeInPixels, aReductionFactor, reducedSize);
		TRAP(err, aBitmap.Create(reducedSize, frameInfo.iFrameDisplayMode));
		if (err != KErrNone)
			{
			User::Leave(err);
			}
		decoder->SetImageTypeL(CImageDecoder::EImageTypeMain);
		
		listener->InitialiseActiveListener();
		decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
		CActiveScheduler::Start();
		TInt error = listener->iStatus.Int(); 

		INFO_PRINTF2(_L("Main image decoded with error %d"), error);
		User::LeaveIfError(error);
		}
	else
		{
		INFO_PRINTF1(_L("Decoding thumbnail, main image, thumbnail"));
		CImageDecoder::TImageType imageType = CImageDecoder::EImageTypeThumbnail;
		for (TInt i = 0; i < 3; i++)
			{
			decoder->SetImageTypeL(imageType);

			const TFrameInfo& thumbnailFrameInfo = decoder->FrameInfo(aFrameNumber);
			User::LeaveIfError(aBitmap.Create(thumbnailFrameInfo.iOverallSizeInPixels, thumbnailFrameInfo.iFrameDisplayMode));

			listener->InitialiseActiveListener();
			decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
			CActiveScheduler::Start();
			TInt error = listener->iStatus.Int(); 

			if (imageType == CImageDecoder::EImageTypeThumbnail)
				{
				INFO_PRINTF2(_L("Thumbnail decoded with error %d"), error);
				imageType = CImageDecoder::EImageTypeMain;
				}
			else
				{
				INFO_PRINTF2(_L("Main image decoded with error %d"), error);
				imageType = CImageDecoder::EImageTypeThumbnail;
				}
			User::LeaveIfError(error);
			}			
			
		INFO_PRINTF1(_L("Decoding main image, thumbnail, main image"));
		CleanupStack::PopAndDestroy(decoder);
		decoder = NULL;
		decoder = CImageDecoder::FileNewL(aFs, aFileName);
		CleanupStack::PushL(decoder);
		imageType = CImageDecoder::EImageTypeMain;
		for (TInt i = 0; i < 3; i++)
			{
			const TFrameInfo& thumbnailFrameInfo = decoder->FrameInfo(aFrameNumber);
			User::LeaveIfError(aBitmap.Create(thumbnailFrameInfo.iOverallSizeInPixels, thumbnailFrameInfo.iFrameDisplayMode));

			listener->InitialiseActiveListener();
			decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
			CActiveScheduler::Start();
			TInt error = listener->iStatus.Int(); 

			if (imageType == CImageDecoder::EImageTypeThumbnail)
				{
				INFO_PRINTF2(_L("Thumbnail decoded with error %d"), error);
				imageType = CImageDecoder::EImageTypeMain;
				}
			else
				{
				INFO_PRINTF2(_L("Main image decoded with error %d"), error);
				imageType = CImageDecoder::EImageTypeThumbnail;
				}
			User::LeaveIfError(error);
			decoder->SetImageTypeL(imageType);
			}
		}

	CleanupStack::Pop(decoder);
	CleanupStack::PopAndDestroy(listener);
	return decoder;
	}

/**
 * Decode a file with mask
 * @xxxx
 */
CImageDecoder* RICLCodecDefectTestStep::DecodeFileWithMaskL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, CFbsBitmap& aMaskBitmap, TInt aFrameNumber, CImageDecoder::TOptions aOptions)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(aFs, aFileName, aOptions);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
	User::LeaveIfError(aBitmap.Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));
	User::LeaveIfError(aMaskBitmap.Create(frameInfo.iOverallSizeInPixels, EGray2));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aMaskBitmap, aFrameNumber);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;
	}

CImageDecoder* RICLCodecDefectTestStep::DecodeDescL(RFs& aFs, const TDesC8& aBufferDes,CFbsBitmap& aBitmap, const TInt aFrameNumber, const TInt aZoomFactor)
	{
	CImageDecoder* decoder = CImageDecoder::DataNewL(aFs, aBufferDes);
	CleanupStack::PushL(decoder);

	TFrameInfo frameInfo(decoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());

	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;

	User::LeaveIfError(aBitmap.Create(frameSize, frameInfo.iFrameDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);

	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
	CActiveScheduler::Start();
	if(listener->iStatus!=KErrNone)
		User::Leave(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;
	}

TInt RICLCodecDefectTestStep::WaitForResult(const TRequestStatus* aStatus) const
	{
	ASSERT(aStatus!=NULL);
	CActiveScheduler::Start();
	return aStatus->Int();
	}
	
//
// RPngDecodeFailureTest
//
RPngDecodeFailureTest* RPngDecodeFailureTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RPngDecodeFailureTest(aSuite);
	}

RPngDecodeFailureTest::RPngDecodeFailureTest(CTestSuite* aSuite):
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5000-HP");
	}

TVerdict RPngDecodeFailureTest::OpenL()
	{
	INFO_PRINTF1(_L("Defect DEF000707: PNG 1024 palette entries test"));

	iTestStepResult = EPass;

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());

	User::LeaveIfError(RFbsSession::Connect());
	//Open and load the file
	TFileName fileName;
	GetTestFileNameL(fileName, KCorruptPngFile);

	INFO_PRINTF2(_L("%S"),&fileName);

	RFile file;
	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead));
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);

	return iTestStepResult;
	}

TVerdict RPngDecodeFailureTest::DoTestStepL()
	{
	//We expect the test to leave with KErrNotFound
	CImageDecoder* imageDecoder = NULL;
	TRAPD(error, imageDecoder = CImageDecoder::DataNewL(iFileSession, iSourceDataPtr));
	delete imageDecoder; imageDecoder = NULL;
	Compare(error, KErrNotFound, _L("PNG with 1024 entries in palette should leave with KErrNotFound"));
	return iTestStepResult;
	}

void RPngDecodeFailureTest::Close()
	{
    RFbsSession::Disconnect();
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	}

//
// RTestStep_5002
//
/**
 * Constructor
 *
 * @xxxx
 */
RTestStep_5002::TTestFile::TTestFile(const TDesC& aFileName, TInt aWidth, TInt aHeight):
	iWidth(aWidth),
	iHeight(aHeight)
	{
	iFileName = &aFileName;
	}

/**
 * Copy constructor
 *
 * @xxxx
 */
RTestStep_5002::TTestFile::TTestFile(const TTestFile& aTestFile):
	iFileName(aTestFile.iFileName),
	iWidth(aTestFile.iWidth),
	iHeight(aTestFile.iHeight)
	{
	}

/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5002*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5002* RTestStep_5002::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5002(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5002::RTestStep_5002(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5002-HP");
	}

/**
 * Setup the ActiveScheduler, FileServerSession BitmapServer
 * and Test Files to use.
 * 
 * @return	"TVerdict"
 *			EPass if the setup is successfull.			
 * @xxxx
 */
TVerdict RTestStep_5002::OpenL()
	{
	INFO_PRINTF1(_L("DEF012043 - Zero size GIFs generate an unhandled exception."));


	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	//Setup the test files
	User::LeaveIfError(iTestFiles.Append(TTestFile(KGif0x25, KGif0x25Width, KGif0x25Height)));
	User::LeaveIfError(iTestFiles.Append(TTestFile(KGif25x0, KGif25x0Width, KGif25x0Height)));

	return iTestStepResult;
	}

/**
 * Decode the test images and verify
 * 1) Reported dimensions are correct.
 * 2) Conversion returns normally
 *
 * @return	"TVerdict"
 *			EPass if the test succeed
 * @xxxx
 */
TVerdict RTestStep_5002::DoTestStepL()
	{
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);

	TInt noOfImages = iTestFiles.Count();
	for(TInt cnt=0; cnt <noOfImages; cnt++)
		{
		TFileName fileName;
		GetTestFileNameL(fileName, iTestFiles[cnt].FileName());

		INFO_PRINTF2(_L("%S"),&fileName);

		//Decode the image
		CImageDecoder* decoder = DecodeFileL(iFileSession, fileName, *bitmap);
		CleanupStack::PushL(decoder);

		const TFrameInfo& frameInfo = decoder->FrameInfo(0);
		//Verify that the dimensions are correct
		Compare(frameInfo.iFrameSizeInPixels.iWidth, iTestFiles[cnt].Width(), _L("Width wrong"));
		Compare(frameInfo.iFrameSizeInPixels.iHeight, iTestFiles[cnt].Height(), _L("Height wrong"));

		//Cleanup
		bitmap->Reset();
		CleanupStack::PopAndDestroy(decoder);
		}
	CleanupStack::PopAndDestroy(bitmap);

	return iTestStepResult;
	}
/**
 * Cleanup of members
 *
 * @xxxx
 */
void RTestStep_5002::Close()
	{
	iTestFiles.Reset();
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}
//
//RTestStep_5004
//
/**
 * Constructor for TFileMimeType helper class
 *
 * @xxxx
 */
RTestStep_5004::TFileMimeType::TFileMimeType(const TDesC& aExtn, 
											 const TDesC8& aMimeType,
											 const TDesC& aDisplayName,
											 const TUid aImageType,
											 const TUid aImageSubType,
											 const TUid aImplementationUid,
											 TBool aShouldBeFound) :
	iExtn(aExtn),
	iMimeType(aMimeType),
	iDisplayName(aDisplayName),
	iImageType(aImageType),
	iImageSubType(aImageSubType),
	iImplementationUid(aImplementationUid),
	iShouldBeFound(aShouldBeFound)
	{
	iFound = EFalse;
	iVerified = EFalse;
	}

/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5004*"
 *			A pointer to the constructed test step
 * @xxxx
 */
RTestStep_5004* RTestStep_5004::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5004(aSuite);
	}


/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5004::RTestStep_5004(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5004-HP");
	}

/**
 * Setup the ActiveScheduler and FileServerSession and test data
 * 
 * @return	"TVerdict"
 *			EPass if the setup is successfull.			
 * @xxxx
 */
TVerdict RTestStep_5004::OpenL()
	{
	INFO_PRINTF1(_L("DEF012242 Test the list returned by CImageEn/Decoder::GetFileTypesL()"));
	INFO_PRINTF1(_L("PDEF105123:Lily + DRM V1+ Picture gallery: The .bmp files with stateful rights (count/in..."));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());

	// We expect to find these plugins
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KTstExtension,KTestMimeType,KTstDisplayName,KTestImageType,KTestImageSubType,KTestFailDecoderUid,ETrue)));
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KTstExtension,KTstMimeType,KTstDisplayName,KTestImageType,KTestImageSubType,KTestDecoderUid,ETrue)));
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KTstExtension,KTestMimeType,KTstDisplayName,KTestImageType,KTestImageSubType,KTestEncoderUid,ETrue)));

	// These should not be found since their version no are lower
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KTstExtension,KTstMimeType,KTstDisplayName,KTestImageType,KTestImageSubType,KExtendedTestDecoder,EFalse)));
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KTstExtension,KTestMimeType,KTstDisplayName,KTestImageType,KTestImageSubType,KExtendedTestEncoder,EFalse)));
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KJpgExtension,KJpgMimeType,KJPEGDisplayName,KJPGImageType,KJPGImageSubType,KJPGDecoderUid,ETrue)));
	User::LeaveIfError(iPluginData.Append(TFileMimeType(KJpgExtension,KJpegMimeType,KJPEGDisplayName,KJPGImageType,KJPGImageSubType,KJPGDecoderUid,ETrue)));


	return iTestStepResult;
	}

/**
 * Get a file and mime list of all available en/decoders.
 * Verify certain test plugins are found and verify the data,
 * also verify certain plugins are not in the list, since they
 * should be replaced by higher version plugins.
 *
 * @return	"TVerdict"
 *			EPass if the test succeed
 * @xxxx
 */
TVerdict RTestStep_5004::DoTestStepL()
	{
	RFileExtensionMIMETypeArray mimeArray;

	// 0=decoders, 1=encoders
	for(TInt coder=0;coder<2;coder++)
		{
		if(coder)
			{
			INFO_PRINTF1(_L("Encoders:"));
			CImageEncoder::GetFileTypesL(mimeArray);
			}
		else
			{
			INFO_PRINTF1(_L("Decoders:"));
			CImageDecoder::GetFileTypesL(mimeArray);
			}

		TInt noOfPlugins = mimeArray.Count();
		for(TInt cnt=0;cnt<noOfPlugins;cnt++)
			{
			CFileExtensionMIMEType* plugin = mimeArray[cnt];

			VerifyPlugin(*plugin);

			//Convert MimeType to TDesC16
			HBufC* mimeType = HBufC::NewLC(plugin->MIMEType().Length());
			TPtr mimePtr(mimeType->Des());
			mimePtr.Copy(plugin->MIMEType());
			INFO_PRINTF7(_L("|%S|%S|%S|%08X|%08X|%08X|"),
				         &plugin->FileExtension(),
						 &mimePtr,
						 &plugin->DisplayName(),
						 plugin->ImageType(),
						 plugin->ImageSubType(),
						 plugin->ImplementationUid());

			CleanupStack::PopAndDestroy(mimeType);
			}
		mimeArray.ResetAndDestroy();
		}

	VerifyResults();

	return iTestStepResult;
	}

/**
 * Compare the plugin against the set list of plugins
 *
 * @param	"CFileExtensionMIMEType& aPlugin"
 *			The plugin to compare.
 * @xxxx
 */
void RTestStep_5004::VerifyPlugin(CFileExtensionMIMEType& aPlugin)
	{
	TInt noOfPlugins = iPluginData.Count();
	for(TInt pluginNo = 0; pluginNo < noOfPlugins; pluginNo++)
		{
		if((iPluginData[pluginNo].iExtn).CompareF(aPlugin.FileExtension()) == KErrNone)
			{
			TFileMimeType& refPlugin = iPluginData[pluginNo];
			if (!refPlugin.iVerified)
				{
				refPlugin.iFound = ETrue;
				TInt match = refPlugin.iMimeType.CompareF(aPlugin.MIMEType());
				match |= refPlugin.iDisplayName.Compare(aPlugin.DisplayName());
				match |= refPlugin.iImageType != aPlugin.ImageType();
				match |= refPlugin.iImageSubType != aPlugin.ImageSubType();
				if(!match)
					{
					refPlugin.iVerified = ETrue;
					break;					
					}
				}
			}
		}
	}

/**
 * Check that all plugins that were expected was found and verified,
 * and the ones not expected was not found. iTestStepResult is set
 * appropriately.
 *
 * @xxxx
 */
void RTestStep_5004::VerifyResults(void)
	{
	TInt noOfPlugins = iPluginData.Count();
	for(TInt pluginNo = 0; pluginNo < noOfPlugins; pluginNo++)
		{
		TFileMimeType& plugin = iPluginData[pluginNo];
		Compare(plugin.iShouldBeFound,plugin.iFound,_L("Incorrect plugin found"));
		if(plugin.iShouldBeFound)
			Compare(plugin.iVerified,ETrue,_L("Plugin data incorrect"));
		}
	}
/**
 * Cleanup of members
 *
 * @xxxx
 */
void RTestStep_5004::Close()
	{
	iPluginData.Reset();
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	}
//
// RTestStep_5006
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5006*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5006* RTestStep_5006::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5006(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5006::RTestStep_5006(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5006-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * 5) Load the reference bitmap
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5006::OpenL()
	{
	INFO_PRINTF1(_L("INC012846 GIF decoder use wrong palette for multi-frame images with mixed global/local color tables (palettes)"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KGifPalette);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KGifPaletteRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}
/**
 * Decode a selected frame from the source file.
 * The frame being decoded must use a different color map
 * from the last frame header processed.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5006::DoTestStepL()
	{
	//Convert a frame which use a different color space from the last frame header processed
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifPaletteFrameNo);
	CleanupStack::PushL(decoder);
	
	//Make sure the last frame's header was processed
	Compare(decoder->IsImageHeaderProcessingComplete(),ETrue,_L("Last frame header not processed"));

	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5006::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5008
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5008*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5008* RTestStep_5008::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5008(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5008::RTestStep_5008(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5008-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5008::OpenL()
	{
	INFO_PRINTF1(_L("INC011852 - Animated gif crash the emulator"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KInterlacedGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KInterlacedGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}
/**
 * Decode a frame from an interlaced GIF with an offset.
 * Compare the decoded frame with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5008::DoTestStepL()
	{
	// Convert a frame with an offset
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KInterlacedGifFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5008::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5010
//
/**
 * Factory function for this test
 * @return	"RTestStep_5010*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5010* RTestStep_5010::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5010(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5010::RTestStep_5010(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5010-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5010::OpenL()
	{
	INFO_PRINTF1(_L("DEF018286 - The first frame of an animated GIF should be the same size as the complete image"));
	INFO_PRINTF1(_L(" PDEF098002: Animated GIF logical screen size is incorrect"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	iSourceBitmap = new (ELeave) CFbsBitmap;
	iReferenceBitmap = new (ELeave) CFbsBitmap;

	return iTestStepResult;
	}
/**
 * Decode the first frame from an animated GIF.
 * Compare the decoded frame with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5010::DoTestStepL()
	{
	GetTestFileNameL(iSourceFile, KAnimatedGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KAnimatedGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	// Convert a frame with an offset
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KAnimatedGifFrameNo);
	
	// Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	delete decoder;
	decoder = NULL;


	GetTestFileNameL(iSourceFile, KAnimatedGifZeroDelay);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	// Convert a frame from an animated gif with zero delay
	decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KAnimatedGifFrameNo);

	// Check the overall size
	TFrameInfo frameInfo = decoder->FrameInfo(KAnimatedGifFrameNo);
	if (frameInfo.iOverallSizeInPixels != TSize(KAnimatedGifZeroDelaySize, KAnimatedGifZeroDelaySize))
		{
		SetFail(_L("iOverallSizeInPixels is incorrect"));
		}

	delete decoder;
	decoder = NULL;

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5010::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5012
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5012*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5012* RTestStep_5012::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5012(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5012::RTestStep_5012(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5012-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5012::OpenL()
	{
	INFO_PRINTF1(_L("DEF018361 - Application/Comment Extension blocks not interpreted correctly"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KExtensionGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KExtensionGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	User::LeaveIfError(iReferenceComments.Append(&KExtensionGifHeaderComment1));
	User::LeaveIfError(iReferenceComments.Append(&KExtensionGifHeaderComment2));
	User::LeaveIfError(iReferenceComments.Append(&KExtensionGifFooterComment1));
	User::LeaveIfError(iReferenceComments.Append(&KExtensionGifFooterComment2));

	return iTestStepResult;
	}
/**
 * Decode the first frame from a GIF with application and comment extensions.
 * Compare the decoded frame with a reference image and verify the comments
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5012::DoTestStepL()
	{
	// Convert a frame with application and comment extensions
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KExtensionGifFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	//Verify the number of comments is correct
	TInt noOfImageComments = decoder->NumberOfImageComments();
	Compare(noOfImageComments,KExtensionGifNoOfComments,_L("Number of comments not correct"));
	
	//Verify the comments
	for(TInt commentNo = 0; commentNo < KExtensionGifNoOfComments; commentNo++)
		{
		HBufC* comment = decoder->ImageCommentL(commentNo);
		CleanupStack::PushL(comment);
		Compare(*comment,*iReferenceComments[commentNo],_L("Comment not correct"));
		CleanupStack::PopAndDestroy(comment);
		}

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5012::Close()
	{
	iReferenceComments.Reset();
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5014
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5014*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5014* RTestStep_5014::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5014(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5014::RTestStep_5014(CTestSuite* aSuite)
	{
	iSuite 		= aSuite;
	iHeapSize	= 128*1024;
	iTestStepName = _L("MM-ICL-COD-U-5014-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5014::OpenL()
	{
	INFO_PRINTF1(_L("DEF021187  - JPEG restart interval of 0 causes decoder to leave with KErrNotFound"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KRestartJpeg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KRestartJpegRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}
/**
 * Decode a frame from a JPEG with a restart interval of 0.
 * Compare the decoded frame with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5014::DoTestStepL()
	{
	// Convert a frame with a restart interval of 0
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KRestartJpegFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5014::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5016
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5016*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5016* RTestStep_5016::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5016(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5016::RTestStep_5016(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5016-HP");
	// Increase the heap size for this test
	iHeapSize = 0x30000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5016::OpenL()
	{
	INFO_PRINTF1(_L("DEF021196 - Interlaced GIF leave blank lines when streaming resized image."));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	TFileName fileName;
	GetTestFileNameL(fileName, KResizeInterlacGif);
	INFO_PRINTF2(_L("Source: %S"),&fileName);

	//Read the file into a descriptor
	TInt size;
	RFile file;
	User::LeaveIfError(file.Open(iFileSession,fileName,EFileRead));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Size(size));
	iSource = HBufC8::NewMaxL(size);
	TPtr8 sourcePtr(iSource->Des());
	User::LeaveIfError(file.Read(sourcePtr));
	CleanupStack::PopAndDestroy(&file);

	//Limit to only the first and part of the second scan
	sourcePtr.SetLength(KResizeInterlacGifSize);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KResizeInterlacGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}
/**
 * Decode the first and part of the second scan of a resized interlaced GIF.
 * Expect the decoding to leave with KErrUnderflow and compare the result
 * with a reference image. Expect the interlaced lines to be duplicated to
 * fill lines.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5016::DoTestStepL()
	{
	// Decode the first and part of the second scan of an interlaced GIF
	CImageDecoder* decoder = NULL;
	TRAPD(error,decoder = DecodeDescL(iFileSession, *iSource, *iSourceBitmap, KResizeInterlacGifFrameNo, KResizeInterlacGifZoom));
	if(error!=KErrUnderflow)
		{
		delete decoder;
		INFO_PRINTF1(_L("Decoding should leave with KErrUnderflow"));
		User::Leave(error);
		}

	ASSERT(decoder==NULL);

	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5016::Close()
	{
	delete iSource; iSource = NULL;
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5018
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5018*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5018* RTestStep_5018::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5018(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5018::RTestStep_5018(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5018-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Test file to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5018::OpenL()
	{
	INFO_PRINTF1(_L("INC021620 - GIF Graphic Control Extension is not correctly extracted (\"User input required\" field)"));
	INFO_PRINTF1(_L("INC112911: Opera: Indefinitely looped animated Gif only animates once"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KUserInputGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	GetTestFileNameL(iSourceFile2, K7IterationsAnimatedGif);
    INFO_PRINTF2(_L("Source: %S"),&iSourceFile2);

	return iTestStepResult;
	}

/**
 * Open a GIF image and verify the TGifImageControl::iUserInputFlag is correctly set.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5018::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession,iSourceFile);
	CleanupStack::PushL(decoder);

	const CFrameImageData& frameImageData = decoder->FrameData(KUserInputGifFrameNo);

	TInt noFrameData = frameImageData.FrameDataCount();

	TInt blocksFound = 0;
	for(TInt dataNo = 0; dataNo < noFrameData; dataNo++)
		{
		const TFrameDataBlock*  frameData = frameImageData.GetFrameData(dataNo);
 
		//Verify the TGifImageControl::iUserInputFlag is correctly set
		if(frameData->DataType() == KGIFImageControlUid)
			{
			const TGifImageControl* gifImageControl = static_cast<const TGifImageControl*>(frameData);
			Compare(gifImageControl->iUserInputFlag, ETrue, _L("User Input Flag Incorrect"));
			blocksFound++;
			}
		}

	//Verify that we found exactly one block
	Compare(blocksFound,1,_L("Number of FrameDataBlocks not correct"));

	CleanupStack::PopAndDestroy(decoder);
	
	decoder = NULL;
    
    // load the 7 iterations animated gif
    decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile2);
    CleanupStack::PushL(decoder);
    
    const CFrameImageData& frameImageData2 = decoder->FrameData(0);
    noFrameData = frameImageData2.ImageDataCount();
    
    iTestStepResult = EFail;
    for (TInt dataNo = 0; dataNo < noFrameData; dataNo++)
        {
        const TImageDataBlock* imageData = frameImageData2.GetImageData(dataNo);
 
        // verify that TGifLoopIterations::iLoopIterations is correctly set
        if(imageData->DataType() == KGIFLoopIterationsUid)
            {
            const TGifLoopIterations* gifLoopIterations = static_cast<const TGifLoopIterations*>(imageData);
            if (gifLoopIterations->iLoopIterations == 7)
                {
                iTestStepResult = EPass;
                break;
                }
            }
        }

    CleanupStack::PopAndDestroy(decoder);

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5018::Close()
	{
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	}

//
// RTestStep_5019
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5019*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5019* RTestStep_5019::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5019(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5019::RTestStep_5019(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5019-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5019::OpenL()
	{
	INFO_PRINTF1(_L("INC021621 - Opening corrupt and truncated .BMP files"));
	INFO_PRINTF1(_L("INC100214: BMP compression type 3 (RGB) doesn't work"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFileCorrupt, KCorruptBmp);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFileCorrupt);
	GetTestFileNameL(iSourceFileTruncated, KTruncatedBmp);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFileTruncated);
	GetTestFileNameL(iSourceFileCompressionType3, KCompressionType3Bmp);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFileCompressionType3);

	return iTestStepResult;
	}

/**
 * Open the two BMP images and check the returned result
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5019::DoTestStepL()
	{
	TInt error;
	
	CImageDecoder* decoder = NULL;

	// open a BMP file with a length of FFFFFFFF and expect KErrNotFound
	TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFileCorrupt));
	if (error != KErrNotFound)
		{
		SetFail(_L("FileNewL did not return KErrNotFound"));
		return iTestStepResult;
		}

	// open a BMP file with a length of FF but no data after the image header
	// and expect KErrUnderflow
	TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFileTruncated));
	if (error != KErrUnderflow)
		{
		SetFail(_L("FileNewL did not return KErrUnderflow"));
		return iTestStepResult;
		}

	// open a BMP file with a length of FF but no data after the image header
	// with EOptionAllowZeroFrameOpen set and expect KErrNone
	TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFileTruncated, CImageDecoder::EOptionAllowZeroFrameOpen));
	if (error != KErrNone)
		{
		SetFail(_L("FileNewL did not return KErrNone"));
		return iTestStepResult;
		}
	delete decoder;
	decoder = NULL;

	// open a BMP file with compression type 3 and expect KErrNone
	TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFileCompressionType3, CImageDecoder::EOptionAllowZeroFrameOpen));
	if (error != KErrNone)
		{
		SetFail(_L("FileNewL did not return KErrNone"));
		return iTestStepResult;
		}
	delete decoder;
	decoder = NULL;

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5019::Close()
	{
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	}

//
// RTestStep_5020
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5020*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5020* RTestStep_5020::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5020(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5020::RTestStep_5020(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5020-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Test file to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5020::OpenL()
	{
	INFO_PRINTF1(_L("INC021911  CImageDecoder:ConvertL() is working incorrectly when called again after Cancel()"));

	iScheduler = new (ELeave) CCancelScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iListener = new (ELeave) CActiveListener;
	iStatus = &iListener->iStatus;
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KCancelGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName refFile;
	GetTestFileNameL(refFile, KCancelGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&refFile);

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(refFile));
	return iTestStepResult;
	}

/**
 * Open a GIF image for decoding. Convert to 1/2 size, Cancelling while in progress.
 * Clear the bitmap. Restart the conversion and verify the converted image match the
 * reference.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5020::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession,iSourceFile);
	CleanupStack::PushL(decoder);

	iScheduler->SetDecoder(*decoder);

	const TFrameInfo& frameInfo = decoder->FrameInfo(KCancelGifFrameNo);
	const TSize frameSize(frameInfo.iOverallSizeInPixels.iWidth/KCancelGifZoomFactor,frameInfo.iOverallSizeInPixels.iHeight/KCancelGifZoomFactor);

	//Create a 1/2 size bitmap
	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(frameSize,frameInfo.iFrameDisplayMode));

	//Convert to find the RunL count from iScheduler->CurrentCount()
	iListener->InitialiseActiveListener();
	decoder->Convert(iStatus,bitmap,KCancelGifFrameNo);
	TInt error = WaitForResult(iStatus);

	//Verify the conversion was successfull
	Compare(error,KErrNone,_L("Initial conversion failed"));

	//Clear the bitmap
	User::LeaveIfError(bitmap.Resize(TSize(0,0)));
	User::LeaveIfError(bitmap.Resize(frameSize));

	//Cancel in middle of conversion
	const TInt cancelCount = iScheduler->CurrentCount() / 2;
	Compare(cancelCount>0,ETrue,_L("Cancel count must be positive"));
	iScheduler->SetCancel(cancelCount);

	iListener->InitialiseActiveListener();
	decoder->Convert(iStatus,bitmap,KCancelGifFrameNo);
	error = WaitForResult(iStatus);
	//Verify the conversion was cancelled
	Compare(error,KErrCancel,_L("Conversion not cancelled"));

	//Clear the bitmap
	User::LeaveIfError(bitmap.Resize(TSize(0,0)));
	User::LeaveIfError(bitmap.Resize(frameSize));

	//Do not cancel this conversion
	iScheduler->SetCancel(0);
	iListener->InitialiseActiveListener();
	decoder->Convert(iStatus,bitmap,KCancelGifFrameNo);
	error = WaitForResult(iStatus);
	//Verify the conversion was sucessfull
	Compare(error,KErrNone,_L("Conversion not failed"));

	//Compare the bitmap against the reference
	Compare(bitmap,*iReferenceBitmap,_L("Bitmaps does not match"));

	CleanupStack::PopAndDestroy(decoder);

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5020::Close()
	{
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iListener; iListener = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5021
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5021*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5021* RTestStep_5021::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5021(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5021::RTestStep_5021(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5021-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5021::OpenL()
	{
	INFO_PRINTF1(_L("DEF024748 Jpeg decoder no londer interprets comment blocks correctly "));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	


    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KCommentJpg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KCommentJpgRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	User::LeaveIfError(iReferenceComments.Append(&KCommentJpgComment1));
	User::LeaveIfError(iReferenceComments.Append(&KCommentJpgComment2));

	return iTestStepResult;
	}
/**
 * Decode a frame from a JPG with multiple comments.
 * Compare the decoded frame with a reference image and verify the comments
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5021::DoTestStepL()
	{
	// Convert a frame with application and comment extensions
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KCommentJpgFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	//Verify the number of comments is correct
	TInt noOfImageComments = decoder->NumberOfFrameComments(KCommentJpgFrameNo);
	Compare(noOfImageComments,KCommentJpgNoOfComments,_L("Number of comments not correct"));
	
	//Verify the comments
	for(TInt commentNo = 0; commentNo < KCommentJpgNoOfComments; commentNo++)
		{
		HBufC* comment = decoder->FrameCommentL(KCommentJpgFrameNo,commentNo);
		CleanupStack::PushL(comment);
		INFO_PRINTF2(_L("%S"),comment);
		Compare(*comment,*iReferenceComments[commentNo],_L("Comment not correct"));
		CleanupStack::PopAndDestroy(comment);
		}

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5021::Close()
	{
	iReferenceComments.Reset();
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5022
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5022*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5022* RTestStep_5022::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5022(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5022::RTestStep_5022(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5022-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5022::OpenL()
	{
	INFO_PRINTF1(_L("DEF024756 Mono Adobe Jpeg images should be supported by ICL"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KAdobeMonoJpg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KAdobeMonoJpgRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}

/**
 * Decode a frame from a Adobe Monochrome JPG.
 * Compare the decoded frame with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5022::DoTestStepL()
	{
	// Convert a frame with application and comment extensions
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KAdobeMonoJpgFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5022::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5023
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5023*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5023* RTestStep_5023::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5023(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5023::RTestStep_5023(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5023-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
_LIT(KWmfTestFiles, "WMF\\*.wmf");

#if defined (__WINS__) || defined (__X86GCC__)
_LIT(KWmfRefDir, "WMFRef\\");
_LIT(KWmfRefMaskDir, "WMFRefMask\\");
#else
_LIT(KWmfRefDir, "WMFArmRef\\");
_LIT(KWmfRefMaskDir, "WMFArmRefMask\\");
#endif

TVerdict RTestStep_5023::OpenL()
	{
	INFO_PRINTF1(_L("Image Transparency test (CR PHAR-5J5LP4)"));

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestDirNameL(iSourceFiles, KWmfTestFiles);
	User::LeaveIfError(iFileSession.GetDir(iSourceFiles, 0, 0, iDir));

    // there is an error on H2, it seems ROP2 & ROP2NoBkgd in iDir is in a reversed order
    // as in iRefDir & iRefMaskDir, so we now change to use file name for all source and 
    // reference and mask files, this will make sure we don't use the wrong file.
	return iTestStepResult;
	}

void RTestStep_5023::GetTestDirNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecDefectTestSuite*)iSuite)->DefaultPath();

	aFileName = defaultPath;
	aFileName.Append(aPathNameAndExtn);
	}

/**
 * Decode WMF images with/without mask generation.
 * Compare the decoded frame and mask with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5023::DoTestStepL()
	{
	TInt maxCount = iDir->Count();
	TInt count = 0;
	TVerdict verdict = EFail;
	TInt machineUID = 0;
	iIsNaviEngine = EFalse;
	iIsH6 = EFalse;
	TInt err = HAL::Get(HAL::EMachineUid, machineUID);
 	if (err != KErrNone)
 		{
 		INFO_PRINTF2(_L(" HAL::Get() call failed with %d"), err);
  	     	return verdict;
 		}
  	if(machineUID==HAL::EMachineUid_NE1_TB)
  		{
  		iIsNaviEngine = ETrue;
  		}
  	else if (machineUID==HAL::EMachineUid_OmapH6)
  		{
  		iIsH6 = ETrue;
  		}
	do
		{
		// Create the bitmaps.
		iSourceBitmap = new(ELeave) CFbsBitmap;
		iSourceMaskBitmap = new(ELeave) CFbsBitmap;
		iReferenceBitmap = new(ELeave) CFbsBitmap;
		iReferenceMaskBitmap = new(ELeave) CFbsBitmap;

		// Convert the image without mask generation.
		TParse sourceFile;
		sourceFile.Set((*iDir)[count].iName, &iSourceFiles, NULL);

		TPtrC sourceFilePtr = sourceFile.NameAndExt();
		INFO_PRINTF2(_L("Source: %S"),&sourceFilePtr);

		CImageDecoder* decoder = DecodeFileWithMaskL(iFileSession, sourceFile.FullName(), *iSourceBitmap, *iSourceMaskBitmap, 0);
		CleanupStack::PushL(decoder);

		// Check that transparency flag is not set.
		TFrameInfo frameInfo = decoder->FrameInfo(0);
		if (frameInfo.iFlags & TFrameInfo::ETransparencyPossible)
			{
			INFO_PRINTF1(_L("Test failed: Transparency possible but not asked for."));
			iTestStepResult = EFail;
			}

		// Load the reference image.
		TFileName fileName;
		GetTestDirNameL(fileName, KWmfRefDir);
		fileName.Append(sourceFile.Name() );
       	if ( !sourceFile.Name().CompareF(_L("the_rest")) || !sourceFile.Name().CompareF(_L("text")) ) 
       		{
	   		if(iIsNaviEngine) 
				{
				//DTW-MM00382 - Reference images changed for NaviEngine
				fileName.Append(_L("_ne") );	
				}
			else if (iIsH6)
				{
				//DTW-MM00500 - Reference images changed for OMAP 3430/H6
				fileName.Append(_L("_h6") );	
				}
			}
		fileName.Append(_L(".mbm") );
		
		TParse referenceFile;
		referenceFile.Set(fileName, NULL, NULL);
		User::LeaveIfError(iReferenceBitmap->Load(referenceFile.FullName()));
		INFO_PRINTF1(referenceFile.FullName());

		// Create a dummy mask reference.
		User::LeaveIfError(iReferenceMaskBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

		//Compare the bitmaps
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same - without mask"));
		Compare(*iSourceMaskBitmap, *iReferenceMaskBitmap, _L("Mask bitmaps not the same - without mask"));
		CleanupStack::PopAndDestroy(decoder);

		// Delete all the bitmaps, except for the reference.
		delete iSourceBitmap; iSourceBitmap = NULL;
		delete iSourceMaskBitmap; iSourceMaskBitmap = NULL;
		delete iReferenceMaskBitmap; iReferenceMaskBitmap = NULL;
	
		// Create new bitmaps.
		iSourceBitmap = new(ELeave) CFbsBitmap;
		iSourceMaskBitmap = new(ELeave) CFbsBitmap;
		iReferenceMaskBitmap = new(ELeave) CFbsBitmap;

		// Convert the image with mask generation.
		decoder = DecodeFileWithMaskL(iFileSession, sourceFile.FullName(), *iSourceBitmap, *iSourceMaskBitmap, 0, CImageDecoder::EAllowGeneratedMask);
		CleanupStack::PushL(decoder);
	
		// Check that transparency flag is set.
		frameInfo = decoder->FrameInfo(0);
		if (!(frameInfo.iFlags & TFrameInfo::ETransparencyPossible))
			{
			INFO_PRINTF1(_L("Test failed: Transparency not possible but it should be."));
			iTestStepResult = EFail;
			}

		// Load the mask reference image.
		GetTestDirNameL(fileName, KWmfRefMaskDir);
		fileName.Append(sourceFile.Name() );
		if ( !sourceFile.Name().CompareF(_L("the_rest")) || !sourceFile.Name().CompareF(_L("text")) ) 
	       		{
       		if(iIsNaviEngine) 
				{
				//DTW-MM00382 - Reference images changed for NaviEngine
				fileName.Append(_L("_ne") );	
				}
			else if (iIsH6)
				{
				//DTW-MM00500 - Reference images changed for OMAP 3430/H6
				fileName.Append(_L("_h6") );	
				}
			}
		fileName.Append(_L(".mbm") );

		referenceFile.Set(fileName, NULL, NULL);
		User::LeaveIfError(iReferenceMaskBitmap->Load(referenceFile.FullName()));

		//Compare the bitmaps
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same - with mask"));
		Compare(*iSourceMaskBitmap, *iReferenceMaskBitmap, _L("Mask bitmaps not the same - with mask"));
		CleanupStack::PopAndDestroy(decoder);

		// Delete all the bitmaps.
		delete iSourceBitmap; iSourceBitmap = NULL;
		delete iSourceMaskBitmap; iSourceMaskBitmap = NULL;
		delete iReferenceBitmap; iReferenceBitmap = NULL;
		delete iReferenceMaskBitmap; iReferenceMaskBitmap = NULL;

		count++;
		} while(count < maxCount);

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5023::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iSourceMaskBitmap; iSourceMaskBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}


//
// RTestStep_5024
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5024*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5024* RTestStep_5024::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5024(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5024::RTestStep_5024(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5024-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5024::OpenL()
	{
	INFO_PRINTF1(_L("Image Transparency test (CR PHAR-5J5LP4)"));

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(iFileSession.Connect());

	GetTestDirNameL(iSourceFiles, KWmfTestFiles);
	User::LeaveIfError(iFileSession.GetDir(iSourceFiles, 0, 0, iDir));

	// same change as in step 5023
	return iTestStepResult;
	}

void RTestStep_5024::GetTestDirNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecDefectTestSuite*)iSuite)->DefaultPath();

	aFileName = defaultPath;
	aFileName.Append(aPathNameAndExtn);
	}

/**
 * Decode WMF images with/without mask generation.
 * Compare the decoded frame and mask with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5024::DoTestStepL()
	{
	TInt maxCount = iDir->Count();
	TInt count = 0;

	TVerdict verdict = EFail;
	TInt machineUID = 0;
	TInt err = HAL::Get(HAL::EMachineUid, machineUID);
 	 if (err != KErrNone)
 		{
 		INFO_PRINTF2(_L(" HAL::Get() call failed with %d"), err);
  	     	return verdict;
 		}
  	 if(machineUID==HAL::EMachineUid_NE1_TB)
  		{
  		iIsNaviEngine = ETrue;
  		}
  	  else if (machineUID==HAL::EMachineUid_OmapH6)
  		{
  		iIsH6 = ETrue;
  		}
	do
		{
		__MM_HEAP_MARK;
		// Make connection to RFbsSession
		CICLFbsSessionTest* fbsSession = CICLFbsSessionTest::NewLC(); 
		// Create the bitmaps.
		iSourceBitmap = new(ELeave) CFbsBitmap;
		iSourceMaskBitmap = new(ELeave) CFbsBitmap;
		iReferenceBitmap = new(ELeave) CFbsBitmap;
		iReferenceMaskBitmap = new(ELeave) CFbsBitmap;

		// Convert the image without mask generation.
		TParse sourceFile;
		sourceFile.Set((*iDir)[count].iName, &iSourceFiles, NULL);

		TPtrC sourceFilePtr = sourceFile.NameAndExt();
		INFO_PRINTF2(_L("Source: %S"),&sourceFilePtr);

		CImageDecoder* decoder = DecodeFileWithMaskL(iFileSession, sourceFile.FullName(), *iSourceBitmap, *iSourceMaskBitmap, 0);
		CleanupStack::PushL(decoder);

		// Check that transparency flag is not set.
		TFrameInfo frameInfo = decoder->FrameInfo(0);
		if (frameInfo.iFlags & TFrameInfo::ETransparencyPossible)
			{
			INFO_PRINTF1(_L("Test failed: Transparency possible but not asked for."));
			iTestStepResult = EFail;
			}

		// Load the reference image.
		TFileName fileName;
		GetTestDirNameL(fileName, KWmfRefDir);
		fileName.Append(sourceFile.Name() );
	
		if ( !sourceFile.Name().CompareF(_L("the_rest")) || !sourceFile.Name().CompareF(_L("text")) ) 
	       		{
	       		if(iIsNaviEngine) 
				{
				//DTW-MM00382 - Reference images changed for NaviEngine
				fileName.Append(_L("_ne") );	
				}
			else if (iIsH6)
				{
				//DTW-MM00500 - Reference images changed for OMAP 3430/H6
				fileName.Append(_L("_h6") );	
				}
			}
		fileName.Append(_L(".mbm") );
		
		TParse referenceFile;
		referenceFile.Set(fileName, NULL, NULL);
		User::LeaveIfError(iReferenceBitmap->Load(referenceFile.FullName()));

		// Create a dummy mask reference.
		User::LeaveIfError(iReferenceMaskBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

		//Compare the bitmaps
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));
		Compare(*iSourceMaskBitmap, *iReferenceMaskBitmap, _L("Bitmaps not the same"));
		CleanupStack::PopAndDestroy(decoder);

		// Delete all the bitmaps, except for the reference.
		delete iSourceBitmap; iSourceBitmap = NULL;
		delete iSourceMaskBitmap; iSourceMaskBitmap = NULL;
		delete iReferenceMaskBitmap; iReferenceMaskBitmap = NULL;
	
		// Create new bitmaps.
		iSourceBitmap = new(ELeave) CFbsBitmap;
		iSourceMaskBitmap = new(ELeave) CFbsBitmap;
		iReferenceMaskBitmap = new(ELeave) CFbsBitmap;

		// Convert the image with mask generation.
		decoder = DecodeFileWithMaskL(iFileSession, sourceFile.FullName(), *iSourceBitmap, *iSourceMaskBitmap, 0, CImageDecoder::EAllowGeneratedMask);
		CleanupStack::PushL(decoder);
	
		// Check that transparency flag is set.
		frameInfo = decoder->FrameInfo(0);
		if (!(frameInfo.iFlags & TFrameInfo::ETransparencyPossible))
			{
			INFO_PRINTF1(_L("Test failed: Transparency not possible but it should be."));
			iTestStepResult = EFail;
			}

		// Load the mask reference image.
		GetTestDirNameL(fileName, KWmfRefMaskDir);
		fileName.Append(sourceFile.Name() );
		if ( !sourceFile.Name().CompareF(_L("the_rest")) || !sourceFile.Name().CompareF(_L("text")) ) 
	       		{
	       		if(iIsNaviEngine) 
				{
				//DTW-MM00382 - Reference images changed for NaviEngine
				fileName.Append(_L("_ne") );	
				}
			else if (iIsH6)
				{
				//DTW-MM00500 - Reference images changed for OMAP 3430/H6
				fileName.Append(_L("_h6") );	
				}
			}
		fileName.Append(_L(".mbm") );
		
		referenceFile.Set(fileName, NULL, NULL);
		User::LeaveIfError(iReferenceMaskBitmap->Load(referenceFile.FullName()));

		//Compare the bitmaps
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));
		Compare(*iSourceMaskBitmap, *iReferenceMaskBitmap, _L("Bitmaps not the same"));
		CleanupStack::PopAndDestroy(decoder);

		// Delete all the bitmaps.
		delete iSourceBitmap; iSourceBitmap = NULL;
		delete iSourceMaskBitmap; iSourceMaskBitmap = NULL;
		delete iReferenceBitmap; iReferenceBitmap = NULL;
		delete iReferenceMaskBitmap; iReferenceMaskBitmap = NULL;

		// Dissconnect From RFbsSession
		CleanupStack::PopAndDestroy(fbsSession);
		__MM_HEAP_MARKEND;
		count++;
		} while(count < maxCount);

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5024::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iSourceMaskBitmap; iSourceMaskBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	}


//
// RTestStep_5025
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5025*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5025* RTestStep_5025::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5025(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5025::RTestStep_5025(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5025-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5025::OpenL()
	{
	INFO_PRINTF1(_L("DEF037251 - CMdaJfifClipFormat cannot decode a near-JFIF file"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KJFIFJpg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KJFIFJpgRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}

/**
 * Decode frames from non-JFIF or Adobe JPGs.
 * Compare the decoded frame with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5025::DoTestStepL()
	{
	// Convert a frame with non RGB/YCC ids.
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));
	delete decoder ; decoder = NULL;

	// Convert a frame with RGB ids.
	GetTestFileNameL(iSourceFile, KRGBJpg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TRAPD(err, decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap));
	if (err != KErrNotFound)
		{
		INFO_PRINTF2(_L("Decode did not return KErrNotFound: %d"), err);
		if (decoder)
			{
			INFO_PRINTF2(_L("DecodeFileL returned decoder: %08X"), decoder->ImplementationUid().iUid);
			}
		delete decoder;

		return EFail;
		}

	// Convert a frame with non YCC ids.
	GetTestFileNameL(iSourceFile, KYCCJpg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TRAP(err, decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap));
	if (err != KErrNotFound)
		{
		//SetFail(_L("Decode did not return KErrNotFound: %d"), err);
		INFO_PRINTF2(_L("Decode did not return KErrNotFound: %d"), err);
		if (decoder)
			{
			INFO_PRINTF2(_L("DecodeFileL returned decoder: %08X"), decoder->ImplementationUid().iUid);
			}
		delete decoder;
		return EFail;
		}

	return EPass;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5025::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5026
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5026*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5026* RTestStep_5026::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5026(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5026::RTestStep_5026(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5026-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2)Bitmap Server connection
 * 3) File Server session
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5026::OpenL()
	{
	INFO_PRINTF1(_L("INC039757 - 2-colors bitmap decoding problem in ICL BMPCodec"));
	INFO_PRINTF1(_L("PDEF112752: V&V:IOP:Content: The phone is not able to display .bmp files (TE Triage comme... "));
	INFO_PRINTF1(_L("DEF134184: [ICL] Regression in BMP codec"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());
	
	TFileName sourceFile;
	GetTestFileNameL(sourceFile, KDecodeBmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v1HeaderBmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v1Depth1Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v1Depth4Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v1Depth8Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v1Depth24Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v2Depth1Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v2Depth4Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v2Depth8Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KOS2v2Depth24Bmp);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KWINv3Depth24withPaletteBmp);
	iSourceFiles.AppendL(sourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KDecodeBmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2v1HeaderBmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth1BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth4BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth8BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth24BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth1BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth4BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth8BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KOS2Depth24BmpRef);
	iRefFiles.AppendL(referenceFile);
	GetTestFileNameL(referenceFile, KWINv3Depth24withPaletteBmpRef);
	iRefFiles.AppendL(referenceFile);


	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	
	return iTestStepResult;
	}

/**
 * Decode BMP images and compare the decoded image with a reference image..
 *
 * @return	"TVerdict"
 *			EPass if the test is succeeded
 * @xxxx
 */

TVerdict RTestStep_5026::DoTestStepL()
	{
	for (TInt i = 0; i < iSourceFiles.Count(); i++)
		{
		INFO_PRINTF2(_L("Source: %S"), &iSourceFiles[i]);
	
		CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFiles[i], *iSourceBitmap);

		User::LeaveIfError(iReferenceBitmap->Load(iRefFiles[i]));

		//Compare the bitmaps
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

		delete decoder;
		decoder = NULL;
		}

	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5026::Close()
	{
	iSourceFiles.Close();
	iRefFiles.Close();
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5027
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5027*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5027* RTestStep_5027::NewL(CTestSuite* aSuite, TInt aTestCase)
	{
	return new (ELeave) RTestStep_5027(aSuite, aTestCase);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5027::RTestStep_5027(CTestSuite* aSuite, TInt aTestCase)
	: iTestCase(KTest5027_TestCase[aTestCase])
	{
	iSuite = aSuite;
	iTestStepName = iTestCase.iTestName;

	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5027::OpenL()
	{
	INFO_PRINTF1(_L("DEF039297/DEF042393 - Background color is not extracted correctly for monochrome PNG images"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	TFileName fileName(iTestCase.iFileName);
	GetTestFileNameL(iSourceFile, fileName);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	fileName = iTestCase.iRefName;
	GetTestFileNameL(fileName, fileName);
	INFO_PRINTF2(_L("Reference: %S"),&fileName);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(fileName));

	return iTestStepResult;
	}

/**
 * Decode monochrome PNG frame with background color.
 * Compare the decoded frame with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5027::DoTestStepL()
	{
	// Open the image.
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	// Check the background color.
	const TFrameInfo& frameInfo = decoder->FrameInfo();
	if (frameInfo.iBackgroundColor != iTestCase.iBackgroundColor)
		iTestStepResult = EFail;

	delete decoder;
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5027::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}


//
// RTestStep_5028
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5028*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5028* RTestStep_5028::NewL(CTestSuite* aSuite, TInt aTestCase)
	{
	return new (ELeave) RTestStep_5028(aSuite, aTestCase);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5028::RTestStep_5028(CTestSuite* aSuite, TInt aTestCase)
	: iTestCase(KTest5028_TestCase[aTestCase])
	{
	iSuite = aSuite;
	iTestStepName = iTestCase.iTestName;
	
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5028::OpenL()
	{
	INFO_PRINTF1(_L("INC041474 - Multiple vulnerabilities in TIFF decoder"));
	INFO_PRINTF1(iTestStepName);

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	iSourceBitmap = new (ELeave) CFbsBitmap;

	TFileName testFileName(iTestCase.iFileName);
	GetTestFileNameL(iSourceFile, testFileName);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	return iTestStepResult;
	}

/**
 * Decode a selection of TIFF files.
 * Compare the error code with the expected value.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5028::DoTestStepL()
	{
	// Open the image.
	CImageDecoder* decoder = NULL;
	TRAPD(err, decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap));
	delete decoder;
	
	// Compare the error code.
	if (err != iTestCase.iErr)
		iTestStepResult = EFail;
		
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5028::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}


/**
 Factory function for this test
 
 @return	"RTestStep_5029*"
 			A pointer to the constructed test step.
 */
RTestStep_5029* RTestStep_5029::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5029(aSuite);
	}

/**
 Constructor set the test step name according to the
 the test that was requested.

 */
RTestStep_5029::RTestStep_5029(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5057-HP");
	}

/**
 Initialise
 1) ActiveScheduler
 2) File Server session
 3) Test files to use
 @return	"TVerdict"
			EPass if the setup was successful
 */
TVerdict RTestStep_5029::OpenL()
	{
	INFO_PRINTF1(_L("INC042385 - AV21 PSS: ICL Gif Decoder panics while opening a corrupted Gif image"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFileCorrupt, KCorruptGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFileCorrupt);

	return iTestStepResult;
	}

/**
 Open the corrupt gif and check the returned result
 
 @return	"TVerdict"
			EPass if the test succeeded
 */
TVerdict RTestStep_5029::DoTestStepL()
	{
	CImageDecoder* decoder = NULL;

	// open the gif (expect KErrNotFound)
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFileCorrupt));
	if (error != KErrNotFound)
		{
		SetFail(_L("FileNewL did not return KErrNotFound"));

		delete decoder;
		return iTestStepResult;
		}

	return iTestStepResult;
	}

/**
 Free resources allocated in SetupL()

 */
void RTestStep_5029::Close()
	{
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	}

//
// RTestStep_5058
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5058*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5058* RTestStep_5058::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5058(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5058::RTestStep_5058(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5058-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * 4) Test file to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5058::OpenL()
	{
	INFO_PRINTF1(_L("DEF042382  CGifDecoder::ReadFormatL doesn't set the background color field of frame info"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KGifPalette);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KGifPaletteRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}

/**
 * Open a GIF image for decoding and test it sets the correct background colour and
 * compare the bitmaps are same when decoded.Also check the background colour is same as expected.  
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5058::DoTestStepL()
	{
	//Decode the image to test the background colour is set correctly.
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifPaletteFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));

	const TFrameInfo& frameInfo = decoder->FrameInfo(KGifPaletteFrameNo);

	//Check the background colour
	if(frameInfo.iBackgroundColor !=  TRgb(KRgbWhite))
		{
		INFO_PRINTF2(_L("The Background colour is set wrong : 0x%x"),frameInfo.iBackgroundColor.Value());
		iTestStepResult = EFail;
		}
	else
		{
		INFO_PRINTF2(_L("The Background colour is set correct : 0x%x"),frameInfo.iBackgroundColor.Value());
		iTestStepResult = EPass;
		}

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	
	}


/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5058::Close()
	{
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}


//
// RTestStep_5059
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5059*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5059* RTestStep_5059::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5059(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5059::RTestStep_5059(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5059-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * 4) Test file to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5059::OpenL()
	{
	INFO_PRINTF1(_L("INC043752 - CImageDecoder can not open certain AGIF and TIFF files"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));

	return iTestStepResult;
	}

/**
 * Opening  a non-standard GIF image of ICL and checks it decodes correctly.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5059::DoTestStepL()
	{
	//Decode the image to test the file is decoded correctly.

	const TInt KGifFrameNo = 0;

	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifFrameNo);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));
	
	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5059::Close()
	{
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}



//
// RTestStep_5060
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5060*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5060* RTestStep_5060::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5060(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5060::RTestStep_5060(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5060-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5060::OpenL()
	{
	INFO_PRINTF1(_L("INC045490 - AV21 PR3 Corrupted GIF images crash image decoder"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	return iTestStepResult;
	}

/**
 * To test Gif files with wrong ClearCode leaves with KErrCorrupt
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5060::DoTestStepL()
	{
	//Gif with wrong Clearcode should leave with KErrCorrupt

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);		

	RArray<TPtrC> array;
	array.Append(_L("Corrupt1.gif"));
	array.Append(_L("Corrupt2.gif"));
	
	TInt noOfImages = array.Count();
	for(TInt cnt=0; cnt <noOfImages; cnt++)
		{
		TFileName fileName;
		GetTestFileNameL(fileName, array[cnt]);
		INFO_PRINTF2(_L("%S"),&fileName);
		CImageDecoder* imageDecoder = NULL;
		TRAPD(error, imageDecoder = DecodeFileL(iFileSession, fileName, *bitmap));
		if(error != KErrCorrupt)
			{
			INFO_PRINTF2(_L("Gif file %S with wrong Clearcode should leave with KErrCorrupt"),&fileName);
			iTestStepResult = EFail;
			}
		bitmap->Reset();
		delete imageDecoder;
		imageDecoder=NULL;
		}
	CleanupStack::PopAndDestroy(bitmap);
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5060::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5061
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5061*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5061* RTestStep_5061::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5061(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5061::RTestStep_5061(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iHeapSize = 128*1024; // 128k
	iTestStepName = _L("MM-ICL-COD-U-5061-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5061::OpenL()
	{
	INFO_PRINTF1(_L("INC045490 - AV21 PR3 Corrupted GIF images crash image decoder"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	iSourceBitmap = new (ELeave) CFbsBitmap;
	iReferenceBitmap = new (ELeave) CFbsBitmap;
	
	return iTestStepResult;
	}

/**
 * To test the decoding of correct.Gif files
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5061::DoTestStepL()
	{
	// Added Gif files should get decoded with KErrNone 
	RArray<TPtrC> imagesArray;
	const TInt KGifFrameNo = 0;

	imagesArray.Append(_L("bit1.gif"));
	imagesArray.Append(_L("bit1.mbm"));
	imagesArray.Append(_L("bit2.gif"));
	imagesArray.Append(_L("bit2.mbm"));
	imagesArray.Append(_L("bit3.gif"));
	imagesArray.Append(_L("bit3.mbm"));
	imagesArray.Append(_L("bit4.gif"));
	imagesArray.Append(_L("bit4.mbm"));
	imagesArray.Append(_L("bit5.gif"));
	imagesArray.Append(_L("bit5.mbm"));
	imagesArray.Append(_L("bit6.gif"));
	imagesArray.Append(_L("bit6.mbm"));
	imagesArray.Append(_L("bit7.gif"));

	imagesArray.Append(_L("bit7.mbm"));
	imagesArray.Append(_L("bit8.gif"));
	imagesArray.Append(_L("bit8.mbm"));
	
	imagesArray.Append(KBadOffSetGif());
	imagesArray.Append(KBadOffSetGifRef());


	TInt noOfImages = imagesArray.Count();
		
	for(TInt count= 0; count <noOfImages; count++)
		{
		GetTestFileNameL(iSourceFile, imagesArray[count]);
		INFO_PRINTF2(_L("SourceFile  %S"),&iSourceFile);

		GetTestFileNameL(iReferenceFile, imagesArray[count+=1]);
		INFO_PRINTF2(_L("ReferenceFile  %S"),&iReferenceFile);
		User::LeaveIfError(iReferenceBitmap->Load(iReferenceFile));
		
		CImageDecoder* imageDecoder = NULL;
		TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifFrameNo));
		if(error != KErrNone)
			{
			INFO_PRINTF3(_L("Gif %S decoding failed with error %d"),&iSourceFile, error);
			iTestStepResult = EFail;
			}
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps are not equal"));
		delete imageDecoder;
		imageDecoder=NULL;
		}
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5061::Close()
	{
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();

	}

//
// RTestStep_5062
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5062*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5062* RTestStep_5062::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5062(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5062::RTestStep_5062(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5062-HP");
	// Increase the heap size for this test
	iHeapSize = 0x20000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5062::OpenL()
	{
	INFO_PRINTF1(_L("INC046305, INC046308 - 16 bpp grayscale PNG not supported; invalid bpp for colour-indexed images"));
	INFO_PRINTF1(_L("INC099484: PNG image file crashes Browser"));
	INFO_PRINTF1(_L("PDEF107407: PNG decoding error when it has IEND tag missing at the end"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());
	    
	iListener = new (ELeave) CActiveListener;
	iStatus = &iListener->iStatus;
			

	return (iTestStepResult=EPass);
	}

/**
 * To test the decoding of correct.Gif files
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5062::DoTestStepL()
	{
	// Added Gif files should get decoded with KErrNone 
	iTestStepResult = EFail;

	GetTestFileNameL(iSourceFile, _L("basi0g16.png"));
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	TFrameInfo frameInfo = decoder->FrameInfo(0);
	delete decoder;
	decoder = NULL;
	INFO_PRINTF3(_L("SourceFile  %S Display mode %d"),&iSourceFile, frameInfo.iFrameDisplayMode);
	if (frameInfo.iFrameDisplayMode != EGray256)
		{
		return iTestStepResult;
		}

	GetTestFileNameL(iSourceFile, _L("png8bpp_g0_b.png"));
	decoder		= CImageDecoder::FileNewL(iFileSession, iSourceFile);
	frameInfo	= decoder->FrameInfo(0);
	delete decoder;
	decoder = NULL;
	INFO_PRINTF3(_L("SourceFile  %S Display mode %d"),&iSourceFile, frameInfo.iFrameDisplayMode);
 	if (frameInfo.iFrameDisplayMode != EGray256)
 		{
		return iTestStepResult;
		}

	GetTestFileNameL(iSourceFile, _L("basn3p04.png"));
	decoder		= CImageDecoder::FileNewL(iFileSession, iSourceFile);
	frameInfo	= decoder->FrameInfo(0);
	delete decoder;
	decoder = NULL;
	INFO_PRINTF3(_L("SourceFile  %S BPP %d (expect 4)"),&iSourceFile, frameInfo.iBitsPerPixel);
 	if (frameInfo.iBitsPerPixel != 4)
 		{
		return iTestStepResult;
 		}
 
 	GetTestFileNameL(iSourceFile, KPngCRCOnBufferBoundary);
	INFO_PRINTF2(_L("SourceFile  %S"), &iSourceFile);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);

	decoder	= CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);
	frameInfo = decoder->FrameInfo(0);	

	User::LeaveIfError(bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));
	
	iListener->InitialiseActiveListener();
	decoder->Convert(iStatus, *bitmap, 0);
	TInt error = WaitForResult(iStatus);
	CleanupStack::PopAndDestroy(decoder);
	decoder = NULL;
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(bitmap);
		return iTestStepResult;
		}
	for (TInt i = 0; i < 4; i++)
		{
		// ensure that various png files with no IEND chunks decode correctly
		switch (i)
			{
		case 0:
 			GetTestFileNameL(iSourceFile, KPngNoIEND);
			break;
		case 1:
 			GetTestFileNameL(iSourceFile, KPngNoIEND2);
			break;
		case 2:
			GetTestFileNameL(iSourceFile, KPngiTXtNoIEND);
			break;
		case 3:
		default:
			GetTestFileNameL(iSourceFile, KPngDataChunkAlign);
			break;
			}	
 		
		INFO_PRINTF2(_L("SourceFile  %S"), &iSourceFile);
		decoder	= CImageDecoder::FileNewL(iFileSession, iSourceFile);
		CleanupStack::PushL(decoder);
		frameInfo = decoder->FrameInfo(0);	

		User::LeaveIfError(bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));
	
		iListener->InitialiseActiveListener();
		decoder->Convert(iStatus, *bitmap, 0);
		error = WaitForResult(iStatus);
		CleanupStack::PopAndDestroy(decoder);
		decoder = NULL;
		if (error != KErrNone)
			{
			CleanupStack::PopAndDestroy(bitmap);
			return iTestStepResult;
			}

		// now ensure that KErrUnderflow occurs when EOptionPngMissingiENDFail is specified
		decoder	= CImageDecoder::FileNewL(iFileSession, iSourceFile, CImageDecoder::EOptionPngMissingiENDFail);
		CleanupStack::PushL(decoder);
		frameInfo = decoder->FrameInfo(0);	

		User::LeaveIfError(bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));
	
		iListener->InitialiseActiveListener();
		decoder->Convert(iStatus, *bitmap, 0);
		error = WaitForResult(iStatus);
		CleanupStack::PopAndDestroy(decoder);
		decoder = NULL;
		if (error != KErrUnderflow)
			{
			CleanupStack::PopAndDestroy(bitmap);
			return iTestStepResult;
			}
		}

	CleanupStack::PopAndDestroy(bitmap);
	return (iTestStepResult = EPass);
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5062::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	delete iListener; 
	iListener = NULL;	
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5063
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5063*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5063* RTestStep_5063::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5063(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5063::RTestStep_5063(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5063-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5063::OpenL()
	{
	INFO_PRINTF1(_L("INC047703 - PNG-decoder panics when decoding a corrupt PNG-file"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
	User::LeaveIfError(iFileSession.Connect());

	return (iTestStepResult=EPass);
	}

/**
 * To test the decoding of correct.Gif files
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5063::DoTestStepL()
	{
	// Added Gif files should get decoded with KErrNone 
	iTestStepResult = EFail;

	GetTestFileNameL(iSourceFile, _L("bad_RNS.png"));
	CImageDecoder* decoder = NULL;
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile));
	delete decoder;
	if (error == KErrNotFound )
		iTestStepResult = EPass;
	return  iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5063::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	}


//
// RTestStep_5064
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5064*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5064* RTestStep_5064::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5064(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5064::RTestStep_5064(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5064-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Test file to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5064::OpenL()
	{
	INFO_PRINTF1(_L("DEF051731 - To check that frame sizes are consistent"));
	
	iAS = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iAS);

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());
	    
	iListener = new (ELeave) CActiveListener;
	iStatus = &iListener->iStatus;

	GetTestFileNameL(iSourceFile, KReductionFactorTestGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	return iTestStepResult;
	}

/**
 * DEF051731 - ICL CImageDecoder and inconsistent reduced frame sizes 
 * This test checks if the Size returned by ReducedSize() is same as 
 * that of the output bitmap returned by convert()
 * (Added newly ReductionFactor() & ReducedSize() to CImageDecoder)
 * 
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
 
TVerdict RTestStep_5064::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);

	const TFrameInfo& frameInfo = decoder->FrameInfo(KCancelGifFrameNo);
	const TSize fullFrameSize(frameInfo.iOverallSizeInPixels);

	// Make reducedFrameSize(240,219) given in defect problem 
	TSize desiredFrameSize(240,219);
	TSize reducedFrameSize;
	
	TInt reductionFactor = decoder->ReductionFactor(fullFrameSize, desiredFrameSize);
	User::LeaveIfError(decoder->ReducedSize(fullFrameSize, reductionFactor, reducedFrameSize));
	
	if(reducedFrameSize.iHeight > desiredFrameSize.iHeight || reducedFrameSize.iWidth > desiredFrameSize.iWidth )
		{
		iTestStepResult = EFail;
		return iTestStepResult;
		}
		
	//Create a bitmap of size ReducedFrameSize 
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(reducedFrameSize, frameInfo.iFrameDisplayMode));
	
	// Set iListener Active
	iListener->InitialiseActiveListener();
	
	// Call Convert
	decoder->Convert(iStatus,*bitmap, KCancelGifFrameNo);
	TInt error = WaitForResult(iStatus);
	
	//Verify the conversion was sucessfull
	Compare(error,KErrNone,_L("Conversion failed"));
	
	TSize newReducedSize = bitmap->SizeInPixels();
	
	//Check if newReducedSize is same as the size returned by CImageDecoder::ReducedSize()
	if((newReducedSize.iHeight != reducedFrameSize.iHeight) || (newReducedSize.iWidth != reducedFrameSize.iWidth ))
		{
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(2, decoder); //bitmap, decoder;
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5064::Close()
	{
	delete iListener; 
	iListener = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}




//
// RTestStep_5065
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5065*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5065* RTestStep_5065::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5065(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5065::RTestStep_5065(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5065-HP");
	
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5065::OpenL()
	{
	INFO_PRINTF1(_L("DEF053021 - Decode PNG Images with EXIF Headers"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
	
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());
    
	return (iTestStepResult=EPass);
	}

/**
 * DEF053021 - PNG's that have extra header information (EXIF fields in this case) 
 * causes the image conversion library (ICL) never complete even though other 
 * implementations of PNG image converters do handle it as expected
 * 
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5065::DoTestStepL()
	{
	iTestStepResult = EFail;
	
		
	TFileName SourceFile;
	GetTestFileNameL(SourceFile, KWithEXIFHead);
	CFbsBitmap* SourceBitmap = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(SourceBitmap);
		
	// ensure that KErrUnderflow doesn't occur with normal png files when using EOptionPngMissingiENDFail
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, SourceFile, CImageDecoder::EOptionPngMissingiENDFail);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(SourceBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *SourceBitmap, 0);
	CActiveScheduler::Start();
	User::LeaveIfError(listener->iStatus.Int());
	
	CleanupStack::PopAndDestroy(3, SourceBitmap); //listener, decoder, SourceBitmap
		
	return (iTestStepResult = EPass);
	}


/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5065::Close()
	{
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	}



//
// RTestStep_5066
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5066*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5066* RTestStep_5066::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5066(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5066::RTestStep_5066(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5066-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5066::OpenL()
	{
	INFO_PRINTF1(_L("INC062084 - NTT multiple vulnerabilities in JPEG decoding"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	return (iTestStepResult=EPass);
	}

/**
 * To test the decoding of corrupt JPEG files
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded (ie the decoder returned KErrCorrupt!)
 * @xxxx
 */

TVerdict RTestStep_5066::DoTestStepL()
	{
	iTestStepResult = EFail;

	GetTestFileNameL(iSourceFile, KCorruptJpeg287);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError( bitmap->Create(TSize(300,122), EColor16M) );
	
	//Decode the image
	TFileName fileName = iSourceFile;
	CImageDecoder* decoder = NULL;
	TRAPD(err, decoder = DecodeFileL(iFileSession, fileName, *bitmap));
	delete decoder;

	if (err != KErrCorrupt)
		{
		ERR_PRINTF2(_L("Unexpected error code when decoding 00000287.jpg: %d"), err);
		}
	else
		{
		INFO_PRINTF1(_L("Successfully determined that 00000287.jpg is corrupt"));
		// Try the other image too
		GetTestFileNameL(iSourceFile, KCorruptJpeg290);
		TRAP(err, decoder=DecodeFileL(iFileSession, fileName, *bitmap));
		delete decoder;
		if (err != KErrCorrupt)
			{
			ERR_PRINTF2(_L("Unexpected error code when decoding 00000290.jpg: %d"), err);
			}
		else
			{
			INFO_PRINTF1(_L("Successfully determined that 00000290.jpg is corrupt"));
			iTestStepResult = EPass;
			}
		}
		
	CleanupStack::PopAndDestroy(bitmap);
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5066::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5067
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5067*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5067* RTestStep_5067::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5067(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5067::RTestStep_5067(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5067-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5067::OpenL()
	{
	INFO_PRINTF1(_L("DEF063125 - JPEG decoder makes white block to certain images"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	iSourceBitmap = new (ELeave) CFbsBitmap;
	iReferenceBitmap = new (ELeave) CFbsBitmap;
		
	return iTestStepResult;
	}

/**
 * To test the decoding of a corrupt JPEG file
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5067::DoTestStepL()
	{
	// Added JPEG file should get decoded with KErrNone 
	RArray<TPtrC> imagesArray;
	CleanupClosePushL(imagesArray);
	imagesArray.AppendL(KWhiteBlockBRCornerJpg());
	imagesArray.AppendL(KWhiteBlockBRCornerJpgRef());
	
	TInt noOfImages = imagesArray.Count();
		
	for(TInt count = 0; count < noOfImages; count++)
		{
		GetTestFileNameL(iSourceFile, imagesArray[count]);
		INFO_PRINTF2(_L("SourceFile  %S"), &iSourceFile);

		GetTestFileNameL(iReferenceFile, imagesArray[count+=1]);
		INFO_PRINTF2(_L("ReferenceFile  %S"), &iReferenceFile);
		User::LeaveIfError(iReferenceBitmap->Load(iReferenceFile));
		
		CImageDecoder* imageDecoder = NULL;
		TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap));
		if(error != KErrNone)
			{
			INFO_PRINTF3(_L("JPEG %S decoding failed with error %d"), &iSourceFile, &error);
			iTestStepResult = EFail;
			}
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps are not equal"));
		delete imageDecoder;
		imageDecoder = NULL;
		}
		
	CleanupStack::PopAndDestroy(&imagesArray);
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5067::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5068*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5068* RTestStep_5068::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5068(aSuite);
	}
	
/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5068::RTestStep_5068(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5068-HP");
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * 5) Load the reference bitmap
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxx */
TVerdict RTestStep_5068::OpenL()
	{
	INFO_PRINTF1(_L("DEF065298 - GIF decoder offset defect"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KPeaceGifImage);
	iSourceImage = new(ELeave) CFbsBitmap;
	iSourceMask = new(ELeave) CFbsBitmap;
	iReference = new (ELeave) CFbsBitmap;
	
	return iTestStepResult;
	}
	
	
/**
 * Decode a selected frame from the source file.
 * The frame being decoded must compare frame image with contents and 
 	they should be equal size.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5068::DoTestStepL()
	{
	TFileName RefImageFile;
	GetTestFileNameL(RefImageFile, KPeace0Image);
	INFO_PRINTF2(_L("Source: %S"),&RefImageFile);

	TFileName RefMaskFile;
	GetTestFileNameL(RefMaskFile, KPeace0Mask);
	INFO_PRINTF2(_L("Reference: %S"),&RefMaskFile);
		
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceImage, *iSourceMask, KGifFrameNo);
	CleanupStack::PushL(decoder);			
	
	//Make sure the last frame's header was processed
	Compare(decoder->IsImageHeaderProcessingComplete(),ETrue,_L("Last frame header not processed"));
	
	User::LeaveIfError(iReference->Load(RefImageFile));
	//Compare Source Image and Reference Image 
	Compare(*iSourceImage, *iReference, _L("Bitmaps not the same"));
		
	User::LeaveIfError(iReference->Load(RefMaskFile));
	//Compare Source Mask and Reference Mask 
	Compare(*iSourceMask, *iReference, _L("Bitmaps not the same"));
	
	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}
	
/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5068::Close()
	{
	delete iReference; iReference = NULL;
	delete iScheduler; iScheduler = NULL;
	delete iSourceImage; iSourceImage = NULL;
	delete iSourceMask; iSourceMask = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

CImageDecoder* RTestStep_5068::DecodeFileL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, CFbsBitmap& aBitmapMask, const TInt aFrameNumber)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(aFs, aFileName);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
	User::LeaveIfError(aBitmap.Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));
	User::LeaveIfError(aBitmapMask.Create(frameInfo.iOverallSizeInPixels, EGray256));
	
	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aBitmapMask, aFrameNumber);
	CActiveScheduler::Start();
	
	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;
	}

//
// RTestStep_5069
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5069*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5069* RTestStep_5069::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5069(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5069::RTestStep_5069(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5069-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5069::OpenL()
	{
	INFO_PRINTF1(_L("DEF067576 - Animated GIF file returns KErrCorrupt"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	iSourceBitmap = new (ELeave) CFbsBitmap;
	iReferenceBitmap = new (ELeave) CFbsBitmap;
		
	return iTestStepResult;
	}

/**
 * To test the decoding of a GIF file previously reported incorrectly as corrupt
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5069::DoTestStepL()
	{
	// Added GIF file should get decoded with KErrNone 
	RArray<TPtrC> imagesArray;
	CleanupClosePushL(imagesArray);
	imagesArray.AppendL(KNotCorruptGif());	// a frame from the original "corrupt" animated gif
	imagesArray.AppendL(KNotCorruptGifRef1());	// frame 1
	imagesArray.AppendL(KNotCorruptGif());		// the original "corrupt" animated gif
	imagesArray.AppendL(KNotCorruptGifRef2());	// frame 2
	imagesArray.AppendL(KNotCorruptGif());		// the original "corrupt" animated gif
	imagesArray.AppendL(KNotCorruptGifRef3());	// frame 3
	
	TInt noOfImages = imagesArray.Count();
		
	TInt frame = 0;
	for(TInt count = 0; count < noOfImages; count++)
		{
		GetTestFileNameL(iSourceFile, imagesArray[count]);
		INFO_PRINTF2(_L("SourceFile  %S"), &iSourceFile);

		GetTestFileNameL(iReferenceFile, imagesArray[count+=1]);
		INFO_PRINTF2(_L("ReferenceFile  %S"), &iReferenceFile);
		User::LeaveIfError(iReferenceBitmap->Load(iReferenceFile));
		
		CImageDecoder* imageDecoder = NULL;
		TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, frame));
		if(error != KErrNone)
			{
			INFO_PRINTF4(_L("GIF %S decoding frame %d failed with error %d"), &iSourceFile, frame + 1, &error);
			iTestStepResult = EFail;
			}
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps are not equal"));
		delete imageDecoder;
		imageDecoder = NULL;
		frame++;
		}
		
	CleanupStack::PopAndDestroy(&imagesArray);
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5069::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}
	

//
// RTestStep_5070
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5070*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5070* RTestStep_5070::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5070(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5070::RTestStep_5070(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5070-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5070::OpenL()
	{
	INFO_PRINTF1(_L("INC067487: Symbian's ICL JPEG encoder produces incorrect JFIF format."));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KJFIFWithThumbnailMbm);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	GetTestFileNameL(iSavedFileName, KJFIFWithThumbnail);
	INFO_PRINTF2(_L("Saved File: %S"),&iSavedFileName);

	return iTestStepResult;
	}

/**
 * Test to check that encode/decode of JFIF jpg file with thumbnail works
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5070::DoTestStepL()
	{
	// encode the mbm as a JFIF jpg
	TFileName createFileName;
	GetTestFileNameL(createFileName, KJFIFWithThumbnail);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, createFileName, EFileWrite | EFileShareAny));
	file.Close();
		
	CImageEncoder* encoder = NULL;
	TUid jpgEncUid = {0x101F45D9};	// JPG encoder UID!
	encoder = CImageEncoder::FileNewL(iFileSession, createFileName, CImageEncoder::EOptionNone, KImageTypeJPGUid, KNullUid, jpgEncUid);
	CleanupStack::PushL(encoder);
	// include a thumbnail
	encoder->SetThumbnail(ETrue);
			
	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener();
		
	iSourceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iSourceBitmap->Load(iSourceFile));
	encoder->Convert(&listener->iStatus, *iSourceBitmap);
	CActiveScheduler::Start();
				
	if (listener->iStatus.Int() != KErrNone)
		{
		INFO_PRINTF1(_L("CImageEncoder::Convert failed"));
		iTestStepResult = EFail;		
		}
	else
		{
			// check the new file against a reference file
		RFile testFile;
		User::LeaveIfError(testFile.Open(iFileSession,createFileName,EFileShareAny|EFileStream|EFileRead));
		CleanupClosePushL(testFile);
		
		TFileName referenceFile;
		GetTestFileNameL(referenceFile, KJFIFWithThumbnailRef);
		
		RFile refFile;
		User::LeaveIfError(refFile.Open(iFileSession,referenceFile,EFileShareAny|EFileStream|EFileRead));
		CleanupClosePushL(refFile);
		
		const TInt KDefaultSize = 0x1000; // 4096
		HBufC8* testData = HBufC8::NewMaxLC(KDefaultSize);
		HBufC8* refData = HBufC8::NewMaxLC(KDefaultSize);
		
		TPtr8 testDes(testData->Des());
		TPtr8 refDes(refData->Des());
		
		TUint count = 0;
		
		do	{
			User::LeaveIfError(testFile.Read(testDes, KDefaultSize) );
			User::LeaveIfError(refFile.Read(refDes, KDefaultSize) );
			count++;
			if (testData->Des() != refData->Des() )
			{
				INFO_PRINTF1(_L("The created file doesn't match the reference file"));
				iTestStepResult = EFail;
				break;
			}
		}
		while (testData->Length() == KDefaultSize);
		
		CleanupStack::PopAndDestroy(4); // refData,testData,refFile,testFile
		}

	CleanupStack::PopAndDestroy(2); // listener,encoder

	if (iTestStepResult == EPass)
		{
		// now check that the new file can be read
		CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, createFileName);
		CleanupStack::PushL(decoder);
		CActiveListener* listener = new (ELeave) CActiveListener;
		CleanupStack::PushL(listener);
		listener->InitialiseActiveListener();
		
		delete iSourceBitmap;
		iSourceBitmap = NULL;
		iSourceBitmap = new (ELeave) CFbsBitmap;
			
		TFrameInfo frameInfo = decoder->FrameInfo();
		User::LeaveIfError(iSourceBitmap->Create(frameInfo.iFrameCoordsInPixels.Size(), frameInfo.iFrameDisplayMode));

		decoder->Convert(&listener->iStatus, *iSourceBitmap);
		CActiveScheduler::Start();

		if (listener->iStatus.Int() != KErrNone)
			{
			INFO_PRINTF1(_L("CImageDecoder::Convert failed"));
			iTestStepResult = EFail;		
			}
		else
			{
			INFO_PRINTF1(_L("JFIF with thumbnail encoded and decoded successfully"));				
			}

		CleanupStack::PopAndDestroy(2);	//listener,decoder		
		}
		
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5070::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Delete(iSavedFileName);
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5070
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5071*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5071* RTestStep_5071::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5071(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5071::RTestStep_5071(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5071-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Test file to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5071::OpenL()
	{
	INFO_PRINTF1(_L("DEF068367 - To Check the ReducedSize for WMF Images for reduction factor 1,2,3"));
	
	iAS = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iAS);
	
	iListener = new (ELeave) CActiveListener;
	iStatus = &iListener->iStatus;
	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestDirNameL(iSourceFile, KReductionFactorTestWfm);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	return iTestStepResult;
	}

/**
 * DEF068367 - ReducedSize() for WMF images returns incorrect values 
 * 
 * (Added newly ReducedSize() and Reduction Factor to CWmfReadCodec)
 * 
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
 
TVerdict RTestStep_5071::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);
	
	for(int iLoop = 0,iSize = 33;iLoop < 4 ;iLoop++,iSize++)
		{
		const TSize fullFrameSize(iSize,iSize);

		// For reduction factor 2 
		TSize desiredFrameSize(16,16);
		TSize reducedFrameSize;
		
		TInt reductionFactor = decoder->ReductionFactor(fullFrameSize, desiredFrameSize);
		if(reductionFactor != 2)
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}
		User::LeaveIfError(decoder->ReducedSize(fullFrameSize, reductionFactor, reducedFrameSize));
		
		if(reducedFrameSize.iHeight > desiredFrameSize.iHeight || reducedFrameSize.iWidth > desiredFrameSize.iWidth )
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}
			
		INFO_PRINTF3(_L("Original(h,w) - %d %d"),fullFrameSize.iHeight,fullFrameSize.iWidth);	
		INFO_PRINTF3(_L("Desired(h,w) - %d %d"),desiredFrameSize.iHeight,desiredFrameSize.iWidth);
		
		INFO_PRINTF2(_L("Reduction Factor - %d" ),reductionFactor);
		INFO_PRINTF3(_L("ReducedSize(h,w) - %d %d"),reducedFrameSize.iHeight,reducedFrameSize.iWidth);
		
		//for reduction factor 3 
		
		desiredFrameSize.SetSize(7,7);

		reductionFactor = decoder->ReductionFactor(fullFrameSize, desiredFrameSize);
		if(reductionFactor != 3)
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}
		User::LeaveIfError(decoder->ReducedSize(fullFrameSize, reductionFactor, reducedFrameSize));
		
		if(reducedFrameSize.iHeight > desiredFrameSize.iHeight || reducedFrameSize.iWidth > desiredFrameSize.iWidth )
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}
			
		INFO_PRINTF3(_L("Original(h,w) - %d %d"),fullFrameSize.iHeight,fullFrameSize.iWidth);	
		INFO_PRINTF3(_L("Desired(h,w) - %d %d"),desiredFrameSize.iHeight,desiredFrameSize.iWidth);
		INFO_PRINTF2(_L("Reduction Factor - %d" ),reductionFactor);
		INFO_PRINTF3(_L("ReducedSize(h,w) - %d %d"),reducedFrameSize.iHeight,reducedFrameSize.iWidth);
		
		// for reduction factor 1
		
		desiredFrameSize.SetSize(23,23);
		
		
		reductionFactor = decoder->ReductionFactor(fullFrameSize, desiredFrameSize);
		if(reductionFactor != 1)
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}
		User::LeaveIfError(decoder->ReducedSize(fullFrameSize, reductionFactor, reducedFrameSize));
		
		if(reducedFrameSize.iHeight > desiredFrameSize.iHeight || reducedFrameSize.iWidth > desiredFrameSize.iWidth )
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}

		INFO_PRINTF3(_L("Original(h,w) - %d %d"),fullFrameSize.iHeight,fullFrameSize.iWidth);	
		INFO_PRINTF3(_L("Desired(h,w) - %d %d"),desiredFrameSize.iHeight,desiredFrameSize.iWidth);
		INFO_PRINTF2(_L("Reduction Factor - %d" ),reductionFactor);
		INFO_PRINTF3(_L("ReducedSize(h,w) - %d %d"),reducedFrameSize.iHeight,reducedFrameSize.iWidth);
		
		
		// for reduction factor 0
		
		desiredFrameSize.SetSize(iSize,iSize);
		
		
		reductionFactor = decoder->ReductionFactor(fullFrameSize, desiredFrameSize);
		
		if(reductionFactor != 0)
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}
		User::LeaveIfError(decoder->ReducedSize(fullFrameSize, reductionFactor, reducedFrameSize));
		
		if(reducedFrameSize.iHeight > desiredFrameSize.iHeight || reducedFrameSize.iWidth > desiredFrameSize.iWidth )
			{
			iTestStepResult = EFail;
			CleanupStack::PopAndDestroy(1, decoder); //decoder;
			return iTestStepResult;
			}

		INFO_PRINTF3(_L("Original(h,w) - %d %d"),fullFrameSize.iHeight,fullFrameSize.iWidth);	
		INFO_PRINTF3(_L("Desired(h,w) - %d %d"),desiredFrameSize.iHeight,desiredFrameSize.iWidth);
		INFO_PRINTF2(_L("Reduction Factor - %d" ),reductionFactor);
		INFO_PRINTF3(_L("ReducedSize(h,w) - %d %d"),reducedFrameSize.iHeight,reducedFrameSize.iWidth);
		
		}
	CleanupStack::PopAndDestroy(1, decoder); // decoder;
	return iTestStepResult;
	}

void RTestStep_5071::GetTestDirNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLCodecDefectTestSuite*)iSuite)->DefaultPath();
	aFileName = defaultPath;
	aFileName.Append(aPathNameAndExtn);
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5071::Close()
	{
	delete iListener; 
	iListener = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5072
//
	
/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5072*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5072* RTestStep_5072::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5072(aSuite);
	}
	
/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5072::RTestStep_5072(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5072-HP");
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxx */
TVerdict RTestStep_5072::OpenL()
	{
	INFO_PRINTF1(_L("DEF069034  Animated GIF returns KErrArgument "));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(FbsStartup());
	 User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KErrArgumentGif);
	iSourceBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}
	
	
/**
 * Decode the first rogue frame from the source file. Was failing with KErrArgument
 * 
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5072::DoTestStepL()
	{
	CImageDecoder* imageDecoder = NULL;
	TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifFrameNo));
	if(error != KErrNone)
		{
		INFO_PRINTF3(_L("GIF %S decoding frame 0 failed with error %d"), &iSourceFile, &error);
		iTestStepResult = EFail;
		}
	delete imageDecoder;
	return iTestStepResult;
	}
	
/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5072::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
//  RTestStep_5073
//
/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5073*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5073* RTestStep_5073::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5073(aSuite);
	}
	
/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5073::RTestStep_5073(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5073-HP");
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * 5) Load the reference bitmap
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxx */
TVerdict RTestStep_5073::OpenL()
	{
	INFO_PRINTF1(_L("DEF069034  Animated GIF returns KErrArgument"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KPeaceGifImage);
	iSourceImage = new(ELeave) CFbsBitmap;
	iReference = new (ELeave) CFbsBitmap;
	
	return iTestStepResult;
	}
	
	
/**
 * Decode a selected frame from the source file.
 * The frame being decoded must compare frame image with contents and 
 	they should be equal size.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5073::DoTestStepL()
	{
	TFileName RefImageFile;
	GetTestFileNameL(RefImageFile, KErrArgumentThumbNailImage);
	INFO_PRINTF2(_L("Source: %S"),&RefImageFile);
			
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceImage, KGifFrameNo);
	CleanupStack::PushL(decoder);			
	
	User::LeaveIfError(iReference->Load(RefImageFile));
//	Compare Source Image and Reference Image 
	Compare(*iSourceImage, *iReference, _L("Bitmaps not the same"));
		
	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}
	
/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5073::Close()
	{
	delete iReference; iReference = NULL;
	delete iScheduler; iScheduler = NULL;
	delete iSourceImage; iSourceImage = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

CImageDecoder* RTestStep_5073::DecodeFileL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(aFs, aFileName);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
	//Decode a thumbnail
	TSize nailSize = frameInfo.iOverallSizeInPixels;
	nailSize.iWidth/=2;
	nailSize.iHeight/=2;
	User::LeaveIfError(aBitmap.Create(nailSize, frameInfo.iFrameDisplayMode));
		
	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
	CActiveScheduler::Start();
	
	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;
	}

//
// RTestStep_5074
//
	
/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5074*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5074* RTestStep_5074::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5074(aSuite);
	}
	
/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5074::RTestStep_5074(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5074-HP");
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxx */
TVerdict RTestStep_5074::OpenL()
	{
	INFO_PRINTF1(_L("DEF070867: Some more animated GIFs fail"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KFirstFrameBiggerThanScreenSizeGif);
	iSourceBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}
	
	
/**
 * Decode the first frame from the source file. Was failing with KErrArgument as
 * it was bigger than the global image size. Now this treated with lenience
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5074::DoTestStepL()
	{
	CImageDecoder* imageDecoder = NULL;
	TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifFrameNo));
	if(error != KErrNone)
		{
		INFO_PRINTF3(_L("GIF %S decoding frame 0 failed with error %d"), &iSourceFile, &error);
		iTestStepResult = EFail;
		}
	delete imageDecoder;
	return iTestStepResult;
	}
	
/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5074::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}
	
//
// RTestStep_5075
//
	
/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5075*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5075* RTestStep_5075::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5075(aSuite);
	}
	
/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5075::RTestStep_5075(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5075-HP");
	iHeapSize = 1024*512; // 256K
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxx */
TVerdict RTestStep_5075::OpenL()
	{
	INFO_PRINTF1(_L("INC070821  Image viewer is unable to open jpg images "));
	INFO_PRINTF1(_L("INC089954  Image Viewer crash on some jpeg with User 47. JPEG decoding problem "));
	INFO_PRINTF1(_L("PDEF088079: Exif - Comment marker in exif header causes panic "));
	INFO_PRINTF1(_L("PDEF088084: Exif - Invalid JPEGInterchangeFormat/Length tags cause Panic"));
	INFO_PRINTF1(_L("PDEF096485: JPEGs cause \"Not Found\" error and do not display"));
	INFO_PRINTF1(_L("CR910: ICL JPEG Support for non conformant exif data"));
	INFO_PRINTF1(_L("PDEF099883: Half broken jpeg shows nothing. (N80 shows unbroken half parts) "));
	INFO_PRINTF1(_L("DEF101406: Changes made in CR910 (EXIF) cause corrupt JPEG files to Panic with KERN_EXEC"));
	INFO_PRINTF1(_L("PDEF105127:OA W950-Nina Vodafone(DE) issue 7614: The preview for prog jpg images is not ... "));
	INFO_PRINTF1(_L("PDEF105343: Panic User 19 appears in CQikImageCapture"));
	INFO_PRINTF1(_L("DEF109125: CJpgReadCodec::RestartStateL() logic problem "));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	TFileName sourceFile;
	GetTestFileNameL(sourceFile, KExtraBitsAfterEOIJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCommentMarkerExifJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KBadThumbnailOffsetSizeExifJpg1);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KBadThumbnailOffsetSizeExifJpg2);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KBadThumbnailOffsetSizeExifJpg3);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptIFDExifJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KWrongInteropOffsetExifJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifJpgFromMoap1);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifJpgFromMoap2);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifJpgFromMoap3);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifJpgFromMoap4);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KGpsButNoInteropExifJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KInvalidTagValueOffsetExifJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifIfd0EntryCountTooBig);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifIfd0MakeSizeTooBig);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifIfd0OffsetTooBig);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KUncompleteImageJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KCorruptExifInvalidEntryType);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KSetImageTypeProblemJpg);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KBrokenJpegMissing0xff);
	iSourceFiles.AppendL(sourceFile);
	
	//Up to here real images are being appended, after here
	//thunbnails are being appended. If you are adding another 
	//real image please increment the size of the index i>XX at
	//the end of DecodeFileL in RTestStep_5075::DoTestStepL() 
	
	// append thumbnail test images end of array
	GetTestFileNameL(sourceFile, KProgJpgWithSeqThumb1);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KProgJpgWithSeqThumb2);
	iSourceFiles.AppendL(sourceFile);
	GetTestFileNameL(sourceFile, KProgJpgWithSeqThumb3);
	iSourceFiles.AppendL(sourceFile);	
		
	iSourceBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}

/**
 * Decode the files that were previously regarded as corrupt
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5075::DoTestStepL()
	{
	CImageDecoder* imageDecoder = NULL;
	for (TInt i = 0; i < iSourceFiles.Count(); i++)
		{
		TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFiles[i], *iSourceBitmap, 0,0, i>20));
		if(error != KErrNone)
			{
			INFO_PRINTF3(_L("Jpeg %S decoding failed with error %d"), &iSourceFiles[i], error);
			iTestStepResult = EFail;
			}
		else
			{
			INFO_PRINTF2(_L("Jpeg %S decoding succeeded"), &iSourceFiles[i]);			
			}
		delete imageDecoder;
		imageDecoder = NULL;
		}
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5075::Close()
	{
	iSourceFiles.Reset();
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5076
//
/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5076*"
 *			a Pointer to the constructed test step
 * @xxxx
 */
RTestStep_5076* RTestStep_5076::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5076(aSuite);
	}
	
/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 * @xxxx
 */
RTestStep_5076::RTestStep_5076(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5076-HP");
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxx */
TVerdict RTestStep_5076::OpenL()
	{
	INFO_PRINTF1(_L("INC069900: Problems with memory usage when opening corrupted gif images"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KSetCommentsLimitSizeGif);
	iSourceBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}
	
/**
 * Decode the frame from the source file. Was failing with KErrNoMemory as
 * it has more number of comments(Comment length is large).
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5076::DoTestStepL()
	{
	CImageDecoder* imageDecoder = NULL;
	TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, KGifFrameNo));
	if(error != KErrNone)
		{
		INFO_PRINTF3(_L("GIF %S decoding frame 0 failed with error %d"), &iSourceFile, &error);
		iTestStepResult = EFail;
		}
	delete imageDecoder;
	return iTestStepResult;
	}
	
/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5076::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5077
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5077*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5077* RTestStep_5077::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5077(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5077::RTestStep_5077(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5077-HP");
	iHeapSize	= 128*1024;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) Bitmap Server connection
 * 3) File Server session
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5077::OpenL()
	{
	INFO_PRINTF1(_L("INC076787: ICL cannot decode JPEG with incorrect comment block length"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	iSourceBitmap = new (ELeave) CFbsBitmap;
		
	return iTestStepResult;
	}

/**
 * To test successful decoding of a JPEG file with incorrect comment block length.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5077::DoTestStepL()
	{
	// Added JPEG file should get decoded with KErrNone 
	RArray<TPtrC> imagesArray;
	CleanupClosePushL(imagesArray);
	imagesArray.AppendL(KWrongCommentBlock1());
	imagesArray.AppendL(KWrongCommentBlock2());
	imagesArray.AppendL(KWrongCommentBlock3());

	TInt noOfImages = imagesArray.Count();

	for(TInt count = 0; count < noOfImages; count++)
		{
		GetTestFileNameL(iSourceFile, imagesArray[count]);
		INFO_PRINTF2(_L("SourceFile  %S"), &iSourceFile);

		CImageDecoder* imageDecoder = NULL;
		TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap));
		if(error != KErrNone)
			{
			INFO_PRINTF3(_L("JPEG %S decoding failed with error %d"), &iSourceFile, &error);
			iTestStepResult = EFail;
			}
		delete imageDecoder;
		}
		
	CleanupStack::PopAndDestroy(&imagesArray);
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5077::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5078
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5078*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5078* RTestStep_5078::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5078(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5078::RTestStep_5078(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5078-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5078::OpenL()
	{
	INFO_PRINTF1(_L("  DEF083440: CImageDecoder incorrect behaviour on finding no codec"));
	INFO_PRINTF1(_L("  PDEF100237: WBMP image doesn't open"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());
		
	return iTestStepResult;
	}

/**
 * To test successful decoding of a JPEG file with incorrect comment block length.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5078::DoTestStepL()
	{
	_LIT8(KWBmpMimeType, "image/vnd.wap.wbmp");
	const TDesC8& mimeType(KWBmpMimeType);
	//We expect the test to leave with KErrNotFound
	INFO_PRINTF2(_L("Creating decoder for MIME type: %S"),&mimeType);
	
	GetTestFileNameL(iSourceFile, KCorruptSizeMultibyteWBmp);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	CImageDecoder* decoder = NULL;
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFileSession,iSourceFile,mimeType));
	if (error == KErrNotFound)
		{
		INFO_PRINTF2(_L("As expected failed to create decoder for MIME type: %S"),&mimeType);
		iTestStepResult = EPass;
		}
	else
		{
		INFO_PRINTF1(_L(" Able to find decoder for corrupt WBmp file"));
		iTestStepResult = EFail;
		}	
	delete decoder; decoder = NULL;

	GetTestFileNameL(iSourceFile, KSmallWBmp);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile));
	if (error)
		{
		INFO_PRINTF2(_L("Failed with error: %d"), error);
		iTestStepResult = EFail;
		}
	delete decoder; decoder = NULL;

	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5078 ::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

/**
 Factory function for this test
 
 @return	"RTestStep_5079*" A pointer to the constructed test step.
 */
RTestStep_5079* RTestStep_5079::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5079(aSuite);
	}

/**
 Constructor set the test step name according to the
 the test that was requested.

 */
RTestStep_5079::RTestStep_5079(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5079-HP");
	}

/**
 Initialise
 1) ActiveScheduler
 2) File Server session
 3) Test files to use
 @return	"TVerdict"
			EPass if the setup was successful
 */
TVerdict RTestStep_5079::OpenL()
	{
	INFO_PRINTF1(_L("DEF082926: Open a gif image corrupt in LZW stream"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFileCorrupt,KLzwStreamCorruptGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFileCorrupt);

	return iTestStepResult;
	}

/**
 Open the corrupt gif and check the returned result
 
 @return	"TVerdict"
			EPass if the test succeeded
 */
TVerdict RTestStep_5079::DoTestStepL()
	{
	CImageDecoder* imageDecoder = NULL;
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CFbsBitmap* refbitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	CleanupStack::PushL(refbitmap);
	
	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KCorruptGifFirstFrameRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);
	User::LeaveIfError(refbitmap->Load(referenceFile));
	
	TRAPD(error,imageDecoder = DecodeFileL(iFileSession, iSourceFileCorrupt, *bitmap, KCorruptLzwGifFrameNo));
	CleanupStack::PushL(imageDecoder);	
	
	if(error == KErrCorrupt)
		{
		SetFail(_L("Unable to recover from corrupt LZW stream"));
		}
	
	//Compare the bitmaps
	Compare(*bitmap, *refbitmap, _L("Bitmaps dont match"));
	
	CleanupStack::PopAndDestroy(imageDecoder);
	CleanupStack::PopAndDestroy(refbitmap);
	CleanupStack::PopAndDestroy(bitmap);
	return iTestStepResult;
	}

/**
 Free resources allocated in OpenL()
 */
void RTestStep_5079::Close()
	{
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}
	
//
// RTestStep_5080
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5080*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5080* RTestStep_5080::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5080(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5080::RTestStep_5080(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5080-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5080::OpenL()
	{
	INFO_PRINTF1(_L(" DEF086789: P: Can't resolve GIF's first frame size using TFrameInfo"));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KSmallFirstFrameGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KSmallFirstFrameGifRef);
	INFO_PRINTF2(_L("Reference: %S"),&referenceFile);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(referenceFile));
			
	return iTestStepResult;
	}

/**
 * To test successful decoding and correct iFrameSizeInPixels of a GIF with first frame size
 * smaller than the overall size
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5080::DoTestStepL()
	{
	CImageDecoder* decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, 0);
	CleanupStack::PushL(decoder);
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps not the same"));
	if (!(decoder->FrameInfo().iFlags & TFrameInfo::EUsesFrameSizeInPixels))
		{	
		INFO_PRINTF1(_L("Can't retrieve frame size"));
		iTestStepResult = EFail;
		}
	
	if (decoder->FrameInfo().iFrameSizeInPixels != TSize(160, 120))
		{	
		INFO_PRINTF1(_L("iFrameSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(decoder);

	// check that iFlags is correct for existing codecs
	decoder = NULL;
	
	GetTestFileNameL(iSourceFile, KRestartJpeg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	decoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, 0);
	CleanupStack::PushL(decoder);

	if (decoder->FrameInfo().iFlags & TFrameInfo::EUsesFrameSizeInPixels)
		{	
		INFO_PRINTF1(_L("TFrameInfo::iFlags is incorrect for pre- iFrameSizeInPixels codecs"));
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(decoder);
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5080::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReferenceBitmap;
	iReferenceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5081
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5081*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5081* RTestStep_5081::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5081(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5081::RTestStep_5081(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5081-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5081::OpenL()
	{
	INFO_PRINTF1(_L(" INC086953: WCDMA - OTC - The display position of AnimatedGIF file is incorrect."));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KSmallFirstFrameGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile1;
	GetTestFileNameL(referenceFile1, KSmallFirstFrameGifSizeRef);
	INFO_PRINTF2(_L("Reference 1: %S"),&referenceFile1);

	TFileName referenceFile2;
	GetTestFileNameL(referenceFile2, KSmallFirstFrameGifHalfSizeRef);
	INFO_PRINTF2(_L("Reference 2: %S"),&referenceFile2);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap1 = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap1->Load(referenceFile1));
			
	iReferenceBitmap2 = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap2->Load(referenceFile2));

	return iTestStepResult;
	}

/**
 * To test successful frame positioning of a GIF with first frame size
 * smaller than the overall size
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5081::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile, CImageDecoder::EOptionUseFrameSizeInPixels);
	CleanupStack::PushL(decoder);
	
	TFrameInfo frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(iSourceBitmap->Create(frameInfo.iFrameSizeInPixels, frameInfo.iFrameDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *iSourceBitmap, 0);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	
	// Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap1, _L("Bitmaps not the same"));
	if (!(decoder->FrameInfo().iFlags & TFrameInfo::EUsesFrameSizeInPixels))
		{	
		INFO_PRINTF1(_L("Can't retrieve frame size"));
		iTestStepResult = EFail;
		}
	
	if (decoder->FrameInfo().iOverallSizeInPixels != TSize(640,480))
		{	
		INFO_PRINTF1(_L("iOverallSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo().iFrameSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iFrameSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo().iFrameCoordsInPixels.iTl != TPoint(240,180))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iTl is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo().iFrameCoordsInPixels.iBr != TPoint(400,300))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iBr is incorrect"));
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(decoder);
	
	// Repeat, this time rendering into a half-size frame (80 x 60).
	// The frame should match our half-size reference frame BUT the pixel sizes
	// in the FrameInfo should remain at actual size
	
	decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile, CImageDecoder::EOptionUseFrameSizeInPixels);
	CleanupStack::PushL(decoder);
	
	frameInfo = decoder->FrameInfo(0);
	TSize thisFrameSize = frameInfo.iFrameSizeInPixels;
	thisFrameSize.iHeight /= 2;
	thisFrameSize.iWidth /= 2;
	
	User::LeaveIfError(iSourceBitmap->Create(thisFrameSize, frameInfo.iFrameDisplayMode));

	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *iSourceBitmap, 0);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	
	// Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap2, _L("Bitmaps not the same"));
	if (!(decoder->FrameInfo().iFlags & TFrameInfo::EUsesFrameSizeInPixels))
		{	
		INFO_PRINTF1(_L("Can't retrieve frame size"));
		iTestStepResult = EFail;
		}
	
	if (decoder->FrameInfo().iOverallSizeInPixels != TSize(640,480))
		{	
		INFO_PRINTF1(_L("iOverallSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo().iFrameSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iFrameSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo().iFrameCoordsInPixels.iTl != TPoint(240,180))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iTl is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo().iFrameCoordsInPixels.iBr != TPoint(400,300))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iBr is incorrect"));
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(decoder);

	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5081::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReferenceBitmap1;
	iReferenceBitmap1 = NULL;
	delete iReferenceBitmap2;
	iReferenceBitmap2 = NULL;
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5082
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5082*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5082* RTestStep_5082::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5082(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5082::RTestStep_5082(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5082-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5082::OpenL()
	{
	INFO_PRINTF1(_L("INC086953: WCDMA - OTC - The display position of AnimatedGIF file is incorrect."));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KFrameGifSizeAnim);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName referenceFile1;
	GetTestFileNameL(referenceFile1, KFrameGifSizeRefAnim1);
	INFO_PRINTF2(_L("Reference 1: %S"),&referenceFile1);

	TFileName referenceFile2;
	GetTestFileNameL(referenceFile2, KFrameGifSizeRefAnim2);
	INFO_PRINTF2(_L("Reference 2: %S"),&referenceFile2);

	TFileName referenceFile3;
	GetTestFileNameL(referenceFile3, KFrameGifSizeRefAnim3);
	INFO_PRINTF2(_L("Reference 3: %S"),&referenceFile3);

	iSourceBitmap = new (ELeave) CFbsBitmap;

	iReferenceBitmap1 = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap1->Load(referenceFile1));
			
	iReferenceBitmap2 = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap2->Load(referenceFile2));

	iReferenceBitmap3 = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap3->Load(referenceFile3));

	return iTestStepResult;
	}

/**
 * To test successful frame positioning of a GIF with first frame size
 * smaller than the overall size
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5082::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile, CImageDecoder::EOptionUseFrameSizeInPixels);
	CleanupStack::PushL(decoder);
	
	// Load all frames of a 3-frame animation
	
	// frame 0
	TFrameInfo frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(iSourceBitmap->Create(frameInfo.iFrameSizeInPixels, frameInfo.iFrameDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *iSourceBitmap, 0);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	
	// Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap1, _L("Bitmaps not the same"));
	if (!(decoder->FrameInfo(0).iFlags & TFrameInfo::EUsesFrameSizeInPixels))
		{	
		INFO_PRINTF1(_L("Can't retrieve frame size"));
		iTestStepResult = EFail;
		}
	
	if (decoder->FrameInfo(0).iOverallSizeInPixels != TSize(640,480))
		{	
		INFO_PRINTF1(_L("iOverallSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(0).iFrameSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iFrameSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(0).iFrameCoordsInPixels.iTl != TPoint(150,150))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iTl is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(0).iFrameCoordsInPixels.iBr != TPoint(310,270))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iBr is incorrect"));
		iTestStepResult = EFail;
		}

	// frame 1
	frameInfo = decoder->FrameInfo(1);
	User::LeaveIfError(iSourceBitmap->Create(frameInfo.iFrameSizeInPixels, frameInfo.iFrameDisplayMode));

	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *iSourceBitmap, 1);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	
	// Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap2, _L("Bitmaps not the same"));
	if (!(decoder->FrameInfo(1).iFlags & TFrameInfo::EUsesFrameSizeInPixels))
		{	
		INFO_PRINTF1(_L("Can't retrieve frame size"));
		iTestStepResult = EFail;
		}
	
	if (decoder->FrameInfo(1).iOverallSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iOverallSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(1).iFrameSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iFrameSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(1).iFrameCoordsInPixels.iTl != TPoint(166,166))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iTl is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(1).iFrameCoordsInPixels.iBr != TPoint(326,286))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iBr is incorrect"));
		iTestStepResult = EFail;
		}

	// frame 2
	frameInfo = decoder->FrameInfo(2);
	User::LeaveIfError(iSourceBitmap->Create(frameInfo.iFrameSizeInPixels, frameInfo.iFrameDisplayMode));

	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *iSourceBitmap, 2);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	
	// Compare the bitmaps
	Compare(*iSourceBitmap, *iReferenceBitmap3, _L("Bitmaps not the same"));
	if (!(decoder->FrameInfo(2).iFlags & TFrameInfo::EUsesFrameSizeInPixels))
		{	
		INFO_PRINTF1(_L("Can't retrieve frame size"));
		iTestStepResult = EFail;
		}
	
	if (decoder->FrameInfo(2).iOverallSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iOverallSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(2).iFrameSizeInPixels != TSize(160,120))
		{	
		INFO_PRINTF1(_L("iFrameSizeInPixels is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(2).iFrameCoordsInPixels.iTl != TPoint(182,182))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iTl is incorrect"));
		iTestStepResult = EFail;
		}
	if (decoder->FrameInfo(2).iFrameCoordsInPixels.iBr != TPoint(342,302))
		{	
		INFO_PRINTF1(_L("iFrameCoordsInPixels.iBr is incorrect"));
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(decoder);

	// check that frame size is not set for existing codecs
	GetTestFileNameL(iSourceFile, KRestartJpeg);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile, CImageDecoder::EOptionUseFrameSizeInPixels);
	CleanupStack::PushL(decoder);
	
	frameInfo = decoder->FrameInfo(0);
	User::LeaveIfError(iSourceBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *iSourceBitmap, 0);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	
	if (decoder->FrameInfo().iFlags & TFrameInfo::EUsesFrameSizeInPixels)
		{	
		INFO_PRINTF1(_L("TFrameInfo::iFlags is incorrect for pre- iFrameSizeInPixels codecs"));
		iTestStepResult = EFail;
		}

	CleanupStack::PopAndDestroy(decoder);
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5082::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iReferenceBitmap1;
	iReferenceBitmap1 = NULL;
	delete iReferenceBitmap2;
	iReferenceBitmap2 = NULL;
	delete iReferenceBitmap3;
	iReferenceBitmap3 = NULL;
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}
//
// INC091941  16x16 pixel BMP image loads 15-20 seconds and is displayed incorrectly 
//
//
// RTestStep_5082
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5083*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5083* RTestStep_5083::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5083(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5083::RTestStep_5083(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5083-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5083::OpenL()
	{
	INFO_PRINTF1(_L("INC091941  16x16 pixel BMP image loads 15-20 seconds and is displayed incorrectly "));

	iScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL( iScheduler );		
	CActiveScheduler::Install( iScheduler );
	CleanupStack::Pop(iScheduler);
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KCorruptRLEBitmap);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	return EPass;
	}
	
TVerdict RTestStep_5083::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);	
	
	TFrameInfo frameInfo = decoder->FrameInfo();
	iDestBitmap = new (ELeave) CFbsBitmap();
	User::LeaveIfError(iDestBitmap->Create(frameInfo.iFrameCoordsInPixels.Size(), frameInfo.iFrameDisplayMode));	
	
	// Convert to convert a frame
	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	listener->InitialiseActiveListener();
	TTime begin;
	begin.HomeTime();
	decoder->Convert(&listener->iStatus, *iDestBitmap);
	CActiveScheduler::Start();
	TTime end;
	end.HomeTime();

	INFO_PRINTF2(_L("---Decode result is : %i"), listener->iStatus.Int());
	if (listener->iStatus.Int() != KErrCorrupt && listener->iStatus.Int() != KErrNone)
	    {
	    INFO_PRINTF1(_L("---ERROR: unexpected error code"));
	    User::Leave(listener->iStatus.Int());
	    }
    const TInt64 microsecsTaken = end.Int64() - begin.Int64();
    const TInt KOneSecond = 1000 * 1000;
    if (microsecsTaken > KOneSecond)
        {
        INFO_PRINTF2(_L("---ERROR: Image Decode took too much time %d"),I64LOW(microsecsTaken));
	    User::Leave(KErrGeneral);
        }
	CleanupStack::PopAndDestroy(2,decoder);
	return EPass;
	}

void RTestStep_5083::Close()
	{
	delete iDestBitmap;
	delete iScheduler;
	iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}


//
// RTestStep_5086
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5086*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5086* RTestStep_5086::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5086(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5086::RTestStep_5086(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5086-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */

TVerdict RTestStep_5086::OpenL()
	{
	INFO_PRINTF1(_L("DEF091243: T: ICL decode GIF with sub-block data length zero"));

    iScheduler = new (ELeave) CCancelScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iListener = new (ELeave) CActiveListener;
	iStatus = &iListener->iStatus;

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KClockZeroSubBlockGif);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);

	TFileName refFile;
	GetTestFileNameL(refFile, KClockZeroSubBlockMbm);
	INFO_PRINTF2(_L("Reference: %S"),&refFile);

	iReferenceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmap->Load(refFile));
	return iTestStepResult;
	}

/**
 * To test successful decoding of a GIF file whose sub block length is zero.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */

TVerdict RTestStep_5086::DoTestStepL()
	{
	INFO_PRINTF2(_L("Openning the file : %S"), &iSourceFile);
	CImageDecoder* decoder = NULL;
	
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFileSession,iSourceFile,CImageDecoder::EOptionNone));
	if (error != KErrNone)
		{
		iFileSession.Close();
		INFO_PRINTF2(_L("Cannot open file %S"), &iSourceFile);
		return EFail;
		}
	CleanupStack::PushL(decoder);
	INFO_PRINTF2(_L("The file : %S had been opened"), &iSourceFile);

	iScheduler->SetDecoder(*decoder);

	CFbsBitmap bitmap;
	const TFrameInfo* frameInfo = &decoder -> FrameInfo();
	User::LeaveIfError(bitmap.Create(frameInfo->iOverallSizeInPixels,frameInfo->iFrameDisplayMode));//EColor256));//

	//Convert to find the RunL count from iScheduler->CurrentCount()
	iListener->InitialiseActiveListener();
	decoder->Convert(iStatus,bitmap,0);
	error = WaitForResult(iStatus);

	//Verify the conversion was successfull
	Compare(error,KErrNone,_L("conversion failed"));
	INFO_PRINTF2(_L("Conversion of file %S was successfull"), &iSourceFile);

    INFO_PRINTF1(_L("Comparing the converted file with the reference bitmap file"));
	//Compare the bitmap against the reference
	Compare(bitmap,*iReferenceBitmap,_L("Bitmaps does not match"));
	INFO_PRINTF1(_L("Bitmaps are identical"));

	CleanupStack::PopAndDestroy(decoder);
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5086 ::Close()
	{
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iListener; iListener = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// RTestStep_5088
//
RTestStep_5088* RTestStep_5088::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5088(aSuite);
	}

RTestStep_5088::RTestStep_5088(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5088-HP");
	}

void RTestStep_5088::KickoffTestL()
	{
	INFO_PRINTF1(_L("INC097489: SWSV_633_91_R6A01_Image: GIF 256 color picture \"Damaged\" after editing in Pic..."));
		
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	TFileName defaultPath = static_cast<CICLCodecDefectTestSuite*>(iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(KNonDefaultPaletteGif, &defaultPath, NULL));
	iSourceFileName = parse.FullName().AllocL();
	INFO_PRINTF2(_L("Source: %S"),iSourceFileName);

	User::LeaveIfError(parse.SetNoWild(KNonDefaultPaletteGifOutput, &defaultPath, NULL));
	iSavedFileName = parse.FullName().AllocL();
	INFO_PRINTF2(_L("Reload: %S"),iSavedFileName);

	iSourceBitmap = new (ELeave) CFbsBitmap;
	iSavedBitmap = new (ELeave) CFbsBitmap;
	iDecoderEncoder = CImageDecoderEncoder_5088::NewL(iFileSession, *this);
	iPalette = CPalette::NewL(KGifColorTableMaxEntries);
	iFrameImageData = CFrameImageData::NewL();

	iTestState = EStateLoadSourceImage;
	iDecoderEncoder->DecodeFromFile(*iSourceFileName, *iSourceBitmap, CImageDecoder::EOptionUseFrameSizeInPixels);
	}

void RTestStep_5088::DecodeEncodeFinished(TInt aErrorCode)
	{
	if (aErrorCode != KErrNone)
		{
		StopTest(aErrorCode);
		}
	else
		{
		Fsm();
		}
	}
	
TBool RTestStep_5088::CompareBitmaps(const CFbsBitmap* aBitmap1, const CFbsBitmap* aBitmap2, CPalette* aPalette)
	{
	if ((!aBitmap1) || (!aBitmap2))
		{
		INFO_PRINTF1(_L("Bitmap encode/decode failed"));
		return EFalse;
		}

	if(aBitmap1->SizeInPixels() != aBitmap2->SizeInPixels())
		{
		INFO_PRINTF1(_L("Bitmap sizes are different"));
		return EFalse;
		}

	const TSize size = aBitmap1->SizeInPixels();
	
	TInt currentPaletteIndex = 0;
	for (TInt y = 0; y < size.iHeight; y++)
		{
		for (TInt x = 0; x < size.iWidth; x++)
			{
			TPoint pt(x, y);
			TRgb color1, color2;
			aBitmap1->GetPixel(color1, pt);

			// build up a palette based on iSourceBitmap's image data
			if (aPalette && aPalette->NearestEntry(color1) != color1)
				{
				aPalette->SetEntry(currentPaletteIndex, color1);
				currentPaletteIndex++;
				}
				
			aBitmap2->GetPixel(color2,pt);
			if (color1 != color2)
				{
				INFO_PRINTF1(_L("Bitmaps are different"));
				return EFalse;
				}
			}
		}

	return ETrue;
	}

void RTestStep_5088::Fsm()
	{
	switch (iTestState)
		{
	case EStateLoadSourceImage:
		{	
		// encode the image	
		iTestState = EStateSaveImageWithAdaptivePalette;
		iDecoderEncoder->EncodeToFile(*iSourceBitmap, *iSavedFileName, CImageEncoder::TOptions(CImageEncoder::EOptionGenerateAdaptivePalette), KImageTypeGIFUid);	
		break;
		}
	case EStateSaveImageWithAdaptivePalette:
		{
		// load the encoded image				
		iTestState = EStateLoadSavedImage1;
		iDecoderEncoder->DecodeFromFile(*iSavedFileName, *iSavedBitmap, CImageDecoder::EOptionUseFrameSizeInPixels);
		break;
		}
	case EStateLoadSavedImage1:
		{
		TBool compareResult = CompareBitmaps(iSourceBitmap, iSavedBitmap, iPalette);
		if (!compareResult)
			{
			StopTest(KErrNone, EFail);
			return;
			}

		TGifColorTable* gifColorTable = new TGifColorTable;
		if (!gifColorTable)
			{
			StopTest(KErrNoMemory, EFail);
			return;
			}
		for (TInt i = 0; i < KGifColorTableMaxEntries; i++)
			{
			gifColorTable->iPalette[i] = iPalette->GetEntry(i);
			}
		iFrameImageData->AppendFrameData(gifColorTable);	// takes ownership

		iTestState = EStateSaveImageWithForcedPalette;
		iDecoderEncoder->EncodeToFile(*iSourceBitmap, *iSavedFileName, CImageEncoder::TOptions(CImageEncoder::EOptionAlwaysThread), KImageTypeGIFUid, iFrameImageData);
		break;
		}
	case EStateSaveImageWithForcedPalette:
		{
		// load the encoded image		
		iTestState = EStateLoadSavedImage2;
		iDecoderEncoder->DecodeFromFile(*iSavedFileName, *iSavedBitmap, CImageDecoder::EOptionUseFrameSizeInPixels);
		break;
		}
	case EStateLoadSavedImage2:
		{
		TBool compareResult = CompareBitmaps(iSourceBitmap, iSavedBitmap, iPalette);
		if (!compareResult)
			{
			StopTest(KErrNone, EFail);
			return;
			}

		StopTest(); // success
		break;
		}
	default:
		INFO_PRINTF1(_L("State machine error"));
		StopTest(KErrNone, EFail);
		break;
		}
	}

/**
 * Free resources allocated in OpenL()
 *
 * @xxxx
 */
void RTestStep_5088::CloseTest()
	{
	if (iDecoderEncoder)
		{
		iDecoderEncoder->Cancel();
		}
	delete iDecoderEncoder;
	delete iFrameImageData;
	iFileSession.Delete(*iSavedFileName);
	delete iSourceFileName;
	delete iSavedFileName;
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iSavedBitmap;
	iSavedBitmap = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

//
// CImageDecoderEncoder_5088
//
CImageDecoderEncoder_5088* CImageDecoderEncoder_5088::NewL(RFs& aFs, MImageDecoderEncoder_5088_Observer& aObserver)
	{
	return new(ELeave) CImageDecoderEncoder_5088(aFs, aObserver);
	}
	
CImageDecoderEncoder_5088::~CImageDecoderEncoder_5088()
	{
	Cancel();
	delete iDecoder;
	delete iEncoder;
	}

void CImageDecoderEncoder_5088::SelfComplete(TInt aError)
	{
	if (!IsActive())
		{
		SetActive();
		}
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aError);
	}

void CImageDecoderEncoder_5088::EncodeToFile(const CFbsBitmap& aSourceBitmap, const TDesC& aTargetFileName, const CImageEncoder::TOptions aOptions, const TUid aImageTypeUid, const CFrameImageData* aFrameImageData)
	{
	delete iDecoder;
	iDecoder = NULL;
		
	delete iEncoder;
	iEncoder = NULL;

	TRAPD(err, iEncoder = CImageEncoder::FileNewL(iFs, aTargetFileName, aOptions, aImageTypeUid));
	if (err != KErrNone)
		{
		SelfComplete(err);
		return;
		}

	SetActive();
	iEncoder->Convert(&iStatus, aSourceBitmap, aFrameImageData);
	}

void CImageDecoderEncoder_5088::DecodeFromFile(const TDesC& aSourceFileName, CFbsBitmap& aTargetBitmap, const CImageDecoder::TOptions aOptions)
	{
	delete iDecoder;
	iDecoder = NULL;

	delete iEncoder;
	iEncoder = NULL;

	TRAPD(err, iDecoder = CImageDecoder::FileNewL(iFs, aSourceFileName, aOptions));
	if (err != KErrNone)
		{
		SelfComplete(err);
		return;
		}
	TFrameInfo frameInfo = iDecoder->FrameInfo(0);
	err = aTargetBitmap.Create(frameInfo.iFrameSizeInPixels, frameInfo.iFrameDisplayMode);
	if (err != KErrNone)
		{
		SelfComplete(err);
		return;
		}

	SetActive();
	iDecoder->Convert(&iStatus, aTargetBitmap, 0);
	}
	
void CImageDecoderEncoder_5088::RunL()
	{
	iObserver.DecodeEncodeFinished(iStatus.Int());
	}

void CImageDecoderEncoder_5088::DoCancel()
	{
	// cancel everything that might be active
	if (iDecoder)
		{
		iDecoder->Cancel();
		}
	if (iEncoder)
		{
		iEncoder->Cancel();
		}
	}
	
CImageDecoderEncoder_5088::CImageDecoderEncoder_5088(RFs& aFs, MImageDecoderEncoder_5088_Observer& aObserver)
: CActive(CActive::EPriorityIdle), iFs(aFs), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

TInt CompareBufToFileL(RFs& aFileSession,const TDesC& aRefFile,const TDesC8& aBuf)
    {
    RFile f;
    TInt res=KErrNone;
    User::LeaveIfError( f.Open(aFileSession, aRefFile, EFileRead) );
    CleanupClosePushL(f);
    TBuf8<256> buf;
    TInt pos=0;
    do
        {
        User::LeaveIfError( f.Read(buf) );
        if (buf.Length() > aBuf.Length() - pos)
            {
            res = KErrGeneral;
            break;
            }
        if (buf.Compare( aBuf.Mid(pos, buf.Length()) ) != 0 )
            {
            res=KErrGeneral;
            break;
            }
        pos += buf.Length();

        } while ( pos < aBuf.Length() );
    CleanupStack::PopAndDestroy(&f);
    
    return res;
    }

/*static*/
RTestStep_5089* RTestStep_5089::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTestStep_5089(aSuite);
    }

RTestStep_5089::RTestStep_5089(CTestSuite* aSuite) 
    {
    iHeapSize   = 1024 * 256; // 256K
    iSuite      = aSuite;
    iTestStepName.Copy(_L("MM-ICL-COD-U-5089-HP"));
    }

	// From RICLCodecDefectTestStep
TVerdict RTestStep_5089::OpenL()
    {
    CActiveScheduler* iScheduler = new (ELeave) CActiveScheduler();
    CActiveScheduler::Install(iScheduler);
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError( iFileSession.Connect() );
    User::LeaveIfError( RFbsSession::Connect() );
    iFbsConnected = ETrue;
    return EPass;
    }
    
TVerdict RTestStep_5089::DoTestStepL()
    {
    _LIT(KRefFileName1, "transformed-wrong-cmt-ref.jpg");
    TVerdict result = EPass;
    RArray<TPtrC> testFiles;
    CleanupClosePushL(testFiles);
    
    User::LeaveIfError( testFiles.Append( KWrongCommentBlock1() ) );
    User::LeaveIfError( testFiles.Append( KWrongCommentBlock2() ) );
    User::LeaveIfError( testFiles.Append( KWrongCommentBlock3() ) );
    
    iTransformer = CImageTransform::NewL(iFileSession);
    
    iListener = new (ELeave) CActiveListener();
    
    for (TInt i=0;  i < testFiles.Count(); i++)
        {
        GetTestFileNameL(iSourceFile, testFiles[i]);
        
        iTransformer->SetSourceFilenameL( iSourceFile );
        
        iTransformer->SetDestDataL( iDestBuf );
        
        iTransformer->SetOptionsL( CImageTransform::EThumbnail );
        iTransformer->SetPreserveImageData(ETrue);
        iTransformer->SetDestSizeInPixelsL( TSize(1,1), ETrue );
        iTransformer->SetupL();
        
        iListener->InitialiseActiveListener();
        iTransformer->Transform( iListener->iStatus );
        CActiveScheduler::Start();
        
        if (iListener->iStatus != KErrNone)
            {
            INFO_PRINTF3(_L("--Error (%d) while transforming the Source: %S"),iListener->iStatus.Int(), &iSourceFile);
            result = EFail;
            }
            
        if (i==0 && result != EFail)
            {
            TFileName refFile;
            GetTestFileNameL(refFile, KRefFileName1);
            if ( KErrNone != CompareBufToFileL(iFileSession, refFile, *iDestBuf) )
                {
                INFO_PRINTF2(_L("--Error Transform output doesn't match REF: %S"),&refFile);
                result = EFail;                
                }
            }
        
        delete iDestBuf;
        iDestBuf = NULL;
        }
    CleanupStack::PopAndDestroy(&testFiles);
    return result;
    }
    
void RTestStep_5089::Close()
    {
    
    delete iTransformer;
    iTransformer = NULL;

    delete iDestBuf;
    iDestBuf = NULL;
    
    delete iListener;    
    iListener = NULL;
    
    if (iFbsConnected)
        {
        RFbsSession::Disconnect();        
        }
    iFileSession.Close();
    
    delete iScheduler;
    iScheduler = NULL;
    }

/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5090*"
 *			a Pointer to the constructed test step
 */
RTestStep_5090* RTestStep_5090::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5090(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 */
RTestStep_5090::RTestStep_5090(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5090-HP");
	iHeapSize = 1024*256; // 256K
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5090::OpenL()
	{
	INFO_PRINTF1(_L("PDEF102815: JPEG with 0xFF 0xFF bytes sequence in image byte stream not decoded fully"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KStreamFFFFJpg);
		
	iSourceBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}

/**
 * Decode the files that were previously regarded as corrupt
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5090::DoTestStepL()
	{
	CImageDecoder* imageDecoder = NULL;
	
	TFileName referenceFile;
	GetTestFileNameL(referenceFile, KRefStreamFFFFJpg);
	INFO_PRINTF2(_L("Reference image: %S"),&referenceFile);
	
	CFbsBitmap* refbitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(refbitmap);
	User::LeaveIfError(refbitmap->Load(referenceFile));
	
	//Do the decoding with reduction factor 3
	TRAPD(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, 0, 3));
	if(error != KErrNone)
		{
		SetFail(_L("Jpeg Decoding Failed"));
		delete imageDecoder;
		imageDecoder = NULL;
		CleanupStack::PopAndDestroy(refbitmap);
		return iTestStepResult;
		}
	
	//Compare the bitmaps
	Compare(*iSourceBitmap, *refbitmap, _L("Jpeg decoding Failed"));
	
	delete imageDecoder;
	imageDecoder = NULL;
	CleanupStack::PopAndDestroy(refbitmap);
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5090::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5091*"
 *			a Pointer to the constructed test step
 */
RTestStep_5091* RTestStep_5091::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5091(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 */
RTestStep_5091::RTestStep_5091(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5091-HP");
	iHeapSize = 1024*256; // 256K
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5091::OpenL()
	{
	INFO_PRINTF1(_L("DEF099679: CImageDecoder::ReducedSize() does not check upper boundary"));
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());
    
	iSourceBitmap = new(ELeave) CFbsBitmap;
	iDestnBitmap = new(ELeave) CFbsBitmap;
	return iTestStepResult;
	}

/**
 * Check if decoders support reduction factor > 3, except Jpeg and Mng.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5091::DoTestStepL()
	{
	TVerdict overallResult = EPass;
	TFileName srcname;
	TFileName destname;
	TBuf<50> decoderName;

	//mbm - decode with reduction factor 4
	if(!ApplyRf(_L("scaled1.mbm"), _L("rf4_scaled1_mbm.mbm"), 4, _L("Mbm")))
		{
		overallResult = EFail;			
		}

	//gif - decode with reduction factor 4
	if(!ApplyRf(_L("anim_640x480_5080.gif"), _L("rf4_anim_640x480_5080_gif.mbm"), 4, _L("Gif")))
		{
		overallResult = EFail;		
		}

	//bmp - decode with reduction factor 4
	if(!ApplyRf(_L("bmp8bpp_8.bmp"), _L("rf4_bmp8bpp_8_bmp.mbm"), 4, _L("Bmp")))
		{
		overallResult = EFail;
		}

	//ico -	decode with reduction factor 4
	if(!ApplyRf(_L("icoepoc.ico"), _L("rf4_icoepoc_ico.mbm"), 4, _L("Ico")))
		{
		overallResult = EFail;	
		}

	//ota - decode with reduction factor 4
	if(!ApplyRf(_L("operator.ota"), _L("rf4_operator_ota.mbm"), 4, _L("Ota")))
		{
		overallResult = EFail;			
		}

	//png - decode with reduction factor 4
	if(!ApplyRf(_L("png8bpp_g0_b.png"), _L("rf4_png8bpp_g0_b_png.mbm"), 4, _L("Png")))
		{
		overallResult = EFail;			
		}

	//wbmp - decode with reduction factor 4
	if(!ApplyRf(_L("sun.wbmp"), _L("rf4_sun_wbmp.mbm"), 4, _L("Wbmp")))
		{
		overallResult = EFail;			
		}

	//tif - decode with reduction factor 4
	if(!ApplyRf(_L("tif3_4.tif"), _L("rf4_tif3_4_tif.mbm"), 4, _L("Tif")))
		{
		overallResult = EFail;			
		}

	//check if jpeg supports reduction factor 3.
	if(!ApplyRf(_L("MISS0001.JPG"), _L("rf4_MISS0001_JPG.mbm"), 3, _L("Jpeg")))
		{
		overallResult = EFail;			
		}

	//check for unsupported reduction factors
	TSize reducedSize(0,0);
	TInt res;
	
	//jpeg cannot support reduction factor > 3. Try with 4.
	CImageDecoder* decoder = NULL;
	GetTestFileNameL(iSourceFile, _L("MISS0001.JPG"));	
	decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	const TFrameInfo& frameInfoJpg = decoder->FrameInfo(0);
	res = decoder->ReducedSize(frameInfoJpg.iOverallSizeInPixels, 4, reducedSize);
	delete decoder;decoder = NULL;
	if(res != KErrArgument)
		{
		INFO_PRINTF1(_L("Failure: Jpeg decoder, for reduction factor 4, doesn't return error."));
		overallResult = EFail;			
		}
	else
		{
		INFO_PRINTF1(_L("Jpeg: as expected, ReducedSize() with reduction factor 4 returned error"));	
		}	
	
	//Mng cannot decoder doesn't support reduction factor at all. Try with 1
	GetTestFileNameL(iSourceFile, _L("simple_b.mng"));	
	decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	//CleanupStack::PushL(decoder);
	const TFrameInfo& frameInfoMng = decoder->FrameInfo(0);
	res = decoder->ReducedSize(frameInfoMng.iOverallSizeInPixels, 1, reducedSize);
	delete decoder;decoder = NULL;
	if(res != KErrArgument)
		{
		INFO_PRINTF1(_L("Failure: Mng decoder, for reduction factor 1, doesn't return error."));
		overallResult = EFail;			
		}
	else
		{
		INFO_PRINTF1(_L("Mng: as expected, ReducedSize() with reduction factor 1 returned error"));
		}
	return iTestStepResult = overallResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5091::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iDestnBitmap;
	iDestnBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}


TBool RTestStep_5091::ApplyRf(const TDesC& aSourceFileName, const TDesC& aRefFileName, TInt aRf, const TDesC& aDecoderName)
	{
	CImageDecoder* imageDecoder = NULL;
	TInt error = KErrNone;
	
	INFO_PRINTF2(_L("%S: applying reduction factor"), &aDecoderName);
	
	TRAP(error, GetTestFileNameL(iSourceFile, aSourceFileName));
	if(error != KErrNone)	
		{
		INFO_PRINTF1(_L("Leave in getting the source file path"));
		return EFalse;
		}
	TRAP(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, 0, aRf));
	delete imageDecoder; imageDecoder = NULL;
		
	if(error != KErrNone)
		{
		INFO_PRINTF3(_L("%S decoding error: %d"), &aDecoderName, error);
		iTestStepResult = EFail;
		return EFalse;
		}	
	
	TRAP(error, GetTestFileNameL(iDestnFile, aRefFileName));
	if(error != KErrNone)	
		{
		INFO_PRINTF1(_L("Leave in getting the reference file path"));
		return EFalse;
		}
	error = iDestnBitmap->Load(iDestnFile);
	if(error != KErrNone)
		{
		INFO_PRINTF2(_L("Error in loading reference bitmap: %d"),error);
		return EFalse;
		}
	
	//prepare error string
	TBuf<200> buf;
	buf.Format(_L("%S decoder failed to support reduction factor %d"), &aDecoderName, aRf);
	Compare(*iSourceBitmap, *iDestnBitmap, buf);
	if(iTestStepResult != EPass)
		{		
		return EFalse;		
		}
	return ETrue;	
	}

CImageDecoder* RTestStep_5091::DecodeFileL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber, const TInt aReductionFactor)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(aFs, aFileName);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
	TSize actualSize(frameInfo.iOverallSizeInPixels);
	TSize reducedSize(0, 0); //Size after applying reduction factor
	
	//in case of wmf file, to use same reference file across platforms
	TParsePtrC parse (aFileName);
	TPtrC extension = parse.Ext();
	if(extension.Compare(_L(".wmf")) == 0)
		{
		//approximate twips/pixel = 12. To give margin take 20, this corresponds to 5% of twips
		actualSize.SetSize((frameInfo.iFrameSizeInTwips.iWidth*5)/100,
				  	  		(frameInfo.iFrameSizeInTwips.iHeight*5)/100);
		}		
	User::LeaveIfError(decoder->ReducedSize(actualSize, aReductionFactor, reducedSize));
	User::LeaveIfError(aBitmap.Create(reducedSize, frameInfo.iFrameDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;	
	}

/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5092*"
 *			a Pointer to the constructed test step
 */
RTestStep_5092* RTestStep_5092::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5092(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 */
RTestStep_5092::RTestStep_5092(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5092-HP");
	iHeapSize = 1024*256; // 256K
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5092::OpenL()
	{
	INFO_PRINTF1(_L("PDEF103195: PNGs BitBlt:ed from Ecolor16MU to EColor16MA becomes transparent"));	

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, K24BppPng);
	i16MuBitmap = new(ELeave) CFbsBitmap;
	i16MaBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}

/**
 * Decode the file to a 16MU bitmap and blit onto a white 16MA bitmap, then check
 * that the 16MA bitmap contains the picture.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5092::DoTestStepL()
	{
	TVerdict result = EFail;
	
	// Open test file & decode into 16MU bitmap
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);
	
	
	const TFrameInfo& frameInfo = decoder->FrameInfo();
	User::LeaveIfError(i16MuBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16MU));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *i16MuBitmap);
	CActiveScheduler::Start();
	User::LeaveIfError(listener->iStatus.Int());
	
	CleanupStack::PopAndDestroy(listener); // listener

	// store top left pixel as reference
	TRgb refPixel;
	i16MuBitmap->GetPixel( refPixel, TPoint(0,0) );
	INFO_PRINTF2(_L("Reference pixel internal value is: 0x%08x"), refPixel.Internal() );
	
	// Create 16MA bitmap and paint it opaque white
	User::LeaveIfError(i16MaBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16MA));
	
	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL( i16MaBitmap );
	CleanupStack::PushL(device);

	CFbsBitGc* context = NULL;	
	User::LeaveIfError(device->CreateContext( context ));
	CleanupStack::PushL( context );
	
	TRgb white( 255, 255, 255, 255 ); // opaque white
	context->SetBrushColor( white );
	context->Clear();
	
	
	// BitBlt 16MU onto 16MA
	context->BitBlt( TPoint(0,0), i16MuBitmap );
	
	
	// Now check a pixel of the 16MA bitmap and ensure it is no longer white.
	// The test image does not contain white pixels at this position so this
	// confirms whether or not it was blitted correctly.
	TRgb testPixel;
	i16MaBitmap->GetPixel( testPixel, TPoint(0,0) );
	
	INFO_PRINTF2(_L("Blitted pixel internal value is: 0x%08x"), testPixel.Internal() );
	
	if( (testPixel.Color16M() == refPixel.Color16M()) && (testPixel.Alpha() == white.Alpha()) )
	{
		result = EPass;
	}

		
	// Cleanup
	CleanupStack::PopAndDestroy(3, decoder); // context, device, decoder
	
	return result;
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5092::Close()
	{
	delete i16MuBitmap;
	i16MuBitmap = NULL;
	delete i16MaBitmap;
	i16MaBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	}


/**
 * Test for:
 * PDEF104507: CGifCodec::DoProcessExtensionL() assumes that the block is not zero length
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */

/*static*/
RTestStep_5093* RTestStep_5093::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTestStep_5093(aSuite);
    }
    
RTestStep_5093::RTestStep_5093(CTestSuite* aSuite)
    {
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5093-HP");
    }    
    
TVerdict RTestStep_5093::OpenL()
    {
    INFO_PRINTF1(_L("PDEF104507: CGifCodec::DoProcessExtensionL() assumes that the block is not zero length"));	
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());    
    return EPass;
    }
    
TVerdict RTestStep_5093::DoTestStepL()
    {
    TVerdict result = EPass;
    RArray<TPtrC> imagesArray;
    CleanupClosePushL(imagesArray);
    imagesArray.AppendL(KZeroLenCommentBlock1());
    imagesArray.AppendL(KZeroLenCommentBlock2());
    
    CActiveListener* listener = new (ELeave) CActiveListener();
    CleanupStack::PushL( listener );

	for (TInt i=0; i < imagesArray.Count(); i++)
	    {
    	TFileName testFile;
    	GetTestFileNameL(testFile, imagesArray[i]);
    	
    	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, testFile);
    	CleanupStack::PushL( decoder );
    	
    	if (! decoder->IsImageHeaderProcessingComplete())
    	    {
    	    INFO_PRINTF2(_L("--Error: Headers are not parsed: %S"), &testFile);
    	    result = EFail;
    	    }
    	CFbsBitmap* dest = new (ELeave) CFbsBitmap();
    	CleanupStack::PushL( dest );
    	User::LeaveIfError( dest->Create(decoder->FrameInfo().iFrameSizeInPixels, EColor16M ) );
    	
    	listener->InitialiseActiveListener();
    	
    	decoder->Convert(&listener->iStatus, *dest);
    	
    	CActiveScheduler::Start();
    	
    	if (listener->iStatus.Int() != KErrNone )
    	    {
    	    INFO_PRINTF3(_L("--Error: (%d) during decoding: %S"), listener->iStatus.Int(), &testFile);
    	    result = EFail;    	    
    	    }
    	
    	CleanupStack::PopAndDestroy(2, decoder);
	    }
	    
	CleanupStack::PopAndDestroy(2, &imagesArray);
    return result;
    }
    
void RTestStep_5093::Close()
    {
    RFbsSession::Disconnect();
    iFileSession.Close();
    delete iScheduler;
    }


/**
 * Factory function for this test step
 * 
 * @return	"RTestStep_5094*"
 *			a Pointer to the constructed test step
 */
RTestStep_5094* RTestStep_5094::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5094(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 * 
 */
RTestStep_5094::RTestStep_5094(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5094-HP");
	iHeapSize = 1024*256; // 256K
	}
	
/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5094::OpenL()
	{
	INFO_PRINTF1(_L("CR1111: PNGs behaviour with Mask and 16M/16MU/16MA/16MAP bitmaps"));	
	INFO_PRINTF1(_L("CR1468: Add support for 16MA bitmaps"));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, K24BppPng);
	i16MxBitmap = new(ELeave) CFbsBitmap;
	iMaskBitmap = new(ELeave) CFbsBitmap;
			
	return iTestStepResult;
	}

/**
 * Decode the file to a 16M bitmap passing a mask bitmap
 * Decode the file to a 16MU bitmap passing a mask bitmap  
 * Decode the file to a 16MA bitmap passing a mask bitmap
 * Decode the file to a 16MAP bitmap passing a mask bitmap
 * 
 * Check that for 16M/16MU/16MA cases the returned error is KErrNone
 * Check that for 16MAP cases the returned error is KErrArgument
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5094::DoTestStepL()
	{
	TVerdict result = EFail;
	
	// Open test file
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);


	// get the frame information
	
	const TFrameInfo& frameInfo = decoder->FrameInfo();

	// create the mask bitmap
	
	User::LeaveIfError(iMaskBitmap->Create(frameInfo.iOverallSizeInPixels, EGray256));	

	// active listener to handle async operations

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);


	// convert to 16M with mask and check that the returned error is KErrNone

	User::LeaveIfError(i16MxBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16M));

	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *i16MxBitmap, *iMaskBitmap);
	CActiveScheduler::Start();
	TInt errorM = listener->iStatus.Int();
	
	i16MxBitmap->Reset();


	// convert to 16MU with mask and check that the returned error is KErrNone

	User::LeaveIfError(i16MxBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16MU));

	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *i16MxBitmap, *iMaskBitmap);
	CActiveScheduler::Start();
	TInt errorMU = listener->iStatus.Int();
	
	i16MxBitmap->Reset();


	// convert to 16MA with mask and check that the returned error is KErrNone

	User::LeaveIfError(i16MxBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16MA));

	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *i16MxBitmap, *iMaskBitmap);
	CActiveScheduler::Start();
	TInt errorMA = listener->iStatus.Int();
	
	i16MxBitmap->Reset();


	// convert to 16MAP with mask and check that the returned error is KErrNotSupported

	User::LeaveIfError(i16MxBitmap->Create(frameInfo.iOverallSizeInPixels, EColor16MAP));

	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, *i16MxBitmap, *iMaskBitmap);
	CActiveScheduler::Start();
	TInt errorMAP = listener->iStatus.Int();
	
	i16MxBitmap->Reset();


	CleanupStack::PopAndDestroy(listener); // listener


	CleanupStack::PopAndDestroy(decoder); // decoder


	// test pass criteria checking

	if( errorM == KErrNone && errorMU == KErrNone &&
		errorMA == KErrNone && errorMAP == KErrNone )
		{
		result = EPass;
		}


	return result;
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5094::Close()
	{
	delete i16MxBitmap;
	i16MxBitmap = NULL;
	delete iMaskBitmap;
	iMaskBitmap = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	delete iScheduler;
	iScheduler = NULL;
	}

/**
 * Test for:
 * PDEF102796: WMF rendering problem
 * PDEF108544: MM-ICL-DECDE-PublicApi.script fails with -4 for step MM-ICL-DECDE-PublicApi-0054

 * @lib "TSU_ICL_COD_03.LIB"
 *
 */

/*static*/
RTestStep_5095* RTestStep_5095::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5095(aSuite);
	}
    
RTestStep_5095::RTestStep_5095(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5095-HP");
	}    
    
TVerdict RTestStep_5095::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iScheduler);

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());  
	return EPass;
	}
    
TVerdict RTestStep_5095::DoTestStepL()
    {
	TVerdict result = EPass;

	TFileName testFileName;
	TFileName testFile;
	for (TInt i = 0; i < 7; i++)
		{
		testFileName.Format(KWmfFormatString, i + 2);
		GetTestFileNameL(testFile, testFileName);

		CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, testFile);
		CleanupStack::PushL(decoder);

		if (!decoder->IsImageHeaderProcessingComplete())
			{
			INFO_PRINTF2(_L("--Error: Headers are not parsed: %S"), &testFile);
			result = EFail;
			}
		TSize correctImageSize(757, 181);
		if (i > 4)
			{
			correctImageSize.iWidth *= 2;
			correctImageSize.iHeight *= 2;
			}

		if (decoder->FrameInfo().iOverallSizeInPixels != correctImageSize)
		    {
	    	INFO_PRINTF4(_L("Incorrect image size reported for %S, w:%d, h:%d"), 
					&testFile,
					decoder->FrameInfo().iOverallSizeInPixels.iWidth,
					decoder->FrameInfo().iOverallSizeInPixels.iHeight);
	    	result = EFail;    	    
	    	}   	    

		CleanupStack::PopAndDestroy(decoder);
		}
		
	// This last test is for a wmf file with no placeable header and a negative value in Window Extension parameters
	GetTestFileNameL(testFile, KWmfNegWinExtension);
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, testFile);
	CleanupStack::PushL(decoder);
	if (!decoder->IsImageHeaderProcessingComplete())
		{
		INFO_PRINTF2(_L("--Error: Headers are not parsed: %S"), &testFile);
		result = EFail;
		}
	TSize correctImageSize(1764, 1647);
	if (decoder->FrameInfo().iOverallSizeInPixels != correctImageSize)
	    {
	    INFO_PRINTF4(_L("Incorrect image size reported for %S, w:%d, h:%d"), 
					&testFile,
					decoder->FrameInfo().iOverallSizeInPixels.iWidth,
					decoder->FrameInfo().iOverallSizeInPixels.iHeight);
	    result = EFail;    	    
	    }
	CleanupStack::PopAndDestroy(decoder);

	return result;
	}
    
void RTestStep_5095::Close()
	{
	RFbsSession::Disconnect();
	iFileSession.Close();
	delete iScheduler;
	iScheduler = NULL;
	}

/**
  * Factory function for this test step
  */
RTestStep_5096* RTestStep_5096::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5096(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 */
RTestStep_5096::RTestStep_5096(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5096-HP");
	iHeapSize = 1024*256; // 256K
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * 5) Active Listener
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5096::OpenL()
	{
	INFO_PRINTF1(_L("PDEF111303 AQP: Coverity tool for ICL - FORWARD NULL Reference in ICL"));
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());
	GetTestFileNameL(iSourceFile, KCorruptWMFFile);
	INFO_PRINTF2(_L("%S"),&iSourceFile);
	iListener = new (ELeave) CActiveListener;		
	return iTestStepResult;
	}

/**
 * Open and convert the corrupt WMF file that previously caused a panic
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5096::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
	CleanupStack::PushL(decoder);
	
	TFrameInfo frameInfo = decoder->FrameInfo(0);	
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));
	
	iListener->InitialiseActiveListener();
	decoder->Convert(&iListener->iStatus, *bitmap, 0);
	//start the convertion
  	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(2,decoder);
  	//check that the result is KErrCorrupt and that the conversion didn't panic 
	iTestStepResult = iListener->iStatus==KErrCorrupt ?  EPass : EFail;
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5096::Close()
	{
	delete iListener;
	iListener=NULL;   
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

/**
 * Test for PDEF106475: Jpeg encoding in rect 100*75 from res 220*294 grey scale
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
//_LIT8(KJPEGMIMEType, "image/jpeg");

/*static*/
RTestStep_5097* RTestStep_5097::NewL(CTestSuite* aSuite)
    {
    return new (ELeave) RTestStep_5097(aSuite);
    }
    
RTestStep_5097::RTestStep_5097(CTestSuite* aSuite)
    {
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5097-HP");
    }    
    
TVerdict RTestStep_5097::OpenL()
    {
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());  
    return EPass;
    }
    
TVerdict RTestStep_5097::DoTestStepL()
    {
    TVerdict result = EPass;
	iActiveListener = new (ELeave) CActiveListener;
	HBufC8 *buf = NULL;

	CFbsBitmap *SourceBmp = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(SourceBmp);	
	User::LeaveIfError(SourceBmp->Create(TSize(1,74), EGray256));

	for (TInt i=2 ; i<65 ; i++)
		{	
		iEncoder = CImageEncoder::DataNewL( buf, KJpegMimeType );

		iActiveListener->InitialiseActiveListener();
	    iEncoder->Convert( &(iActiveListener->iStatus), *SourceBmp);
		CActiveScheduler::Start();
		
		delete buf;
		buf = NULL;

		if(iActiveListener->iStatus!=KErrNone)
			{
			User::Leave(iActiveListener->iStatus.Int());			
			}

		delete iEncoder;
		iEncoder = NULL;

		User::LeaveIfError(SourceBmp->Resize(TSize(i,74)));
		}
		
	CleanupStack::Pop(); // SourceBmp
	delete SourceBmp;
	delete iActiveListener;
	iActiveListener = NULL;
    return result;
    }
    
void RTestStep_5097::Close()
    {
	delete iEncoder;
    iEncoder = NULL;
    delete iActiveListener;
    iActiveListener = NULL;
   
    RFbsSession::Disconnect();
    iFileSession.Close();
    delete iScheduler;
    iScheduler = NULL;
    }

/**
 * Factory function for this test step
 */
RTestStep_5098* RTestStep_5098::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5098(aSuite);
	}

/**
 * Constructor for this test step.
 * Initialises the test step name, which will be used in the script.
 */
RTestStep_5098::RTestStep_5098(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5098-HP");
	iHeapSize = 1024*256; // 256K
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use 
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5098::OpenL()
	{
	INFO_PRINTF1(_L("PDEF108785:Symbian JPEG decoder cannot decode multi-scan JPEG images"));
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());
		
	iSourceBitmap = new(ELeave) CFbsBitmap;
	iDestnBitmap = new(ELeave) CFbsBitmap;		
	return iTestStepResult;
	}

/**
 * Decode the files that were previously regarded as corrupt
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5098::DoTestStepL()
	{
	TVerdict overallResult = EPass;
	iTestStepResult = EFail;

	if(!CheckDecoding(_L("multiscan.jpg"), _L("multiscan_EColor16M_0rf.mbm"), 0, EColor16M))
		{
		overallResult = EFail;
		}
	if(!CheckDecoding(_L("multiscan.jpg"), _L("multiscan_EColor16M_2rf.mbm"), 2, EColor16M))
		{
		overallResult = EFail;
		}	
	if(!CheckDecoding(_L("multiscan.jpg"), _L("multiscan_EColor4K_1rf.mbm"), 1, EColor4K))
		{
		overallResult = EFail;
		}
	if(!CheckDecoding(_L("multiscan.jpg"), _L("multiscan_EGray2_0rf.mbm"), 0, EGray2))
		{
		overallResult = EFail;	
		}
	if(!CheckDecoding(_L("multiscan.jpg"), _L("multiscan_EGray256_3rf.mbm"), 3, EGray256))
		{
		overallResult = EFail;	
		}
	if(!CheckDecoding(_L("multiscan_v_deleted_at_end.jpg"), _L("multiscan_v_deleted_at_end.mbm"), 0, EColor16M))
		{
		overallResult = EFail;	
		}
	if(!CheckDecoding(_L("multiscan_uv_few_deleted_at_end.jpg"), _L("multiscan_uv_few_deleted_at_end.mbm"), 0, EColor16M))	
		{
		overallResult = EFail;		
		}
	if(!CheckDecoding(_L("multiscan1.jpg"), _L("multiscan1_EColor16M_0rf.mbm"), 0, EColor16M))	
		{
		overallResult = EFail;		
		}
	if(!CheckDecoding(_L("multiscan_exif.jpg"), _L("multiscan_exif_EColor16M_1rf.mbm"), 1, EColor16M))	
		{
		overallResult = EFail;		
		}			
	iTestStepResult = overallResult;
	return iTestStepResult;
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5098::Close()
	{
	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iDestnBitmap;
	iDestnBitmap = NULL;
	delete iScheduler;
	iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	}

CImageDecoder* RTestStep_5098::DecodeFileL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber, const TInt aReductionFactor, TDisplayMode aDisplayMode)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(aFs, aFileName);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
	TSize actualSize(frameInfo.iOverallSizeInPixels);
	TSize reducedSize(0, 0); //Size after applying reduction factor
	
	//in case of wmf file, to use same reference file across platforms
	TParsePtrC parse (aFileName);
	TPtrC extension = parse.Ext();
	if(extension.Compare(_L(".wmf")) == 0)
		{
		//approximate twips/pixel = 12. To give margin take 20, this corresponds to 5% of twips
		actualSize.SetSize((frameInfo.iFrameSizeInTwips.iWidth*5)/100,
				  	  		(frameInfo.iFrameSizeInTwips.iHeight*5)/100);
		}		
	User::LeaveIfError(decoder->ReducedSize(actualSize, aReductionFactor, reducedSize));
	User::LeaveIfError(aBitmap.Create(reducedSize, aDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;	
	}

TBool RTestStep_5098::CheckDecoding(const TDesC& aSourceFileName, const TDesC& aRefFileName, TInt aRf, TDisplayMode aDisplayMode)
	{
	CImageDecoder* imageDecoder = NULL;
	TInt error = KErrNone;
	iTestStepResult = EFail;
	
	TRAP(error, GetTestFileNameL(iSourceFile, aSourceFileName));
	if (error != KErrNone)	
		{
		INFO_PRINTF1(_L("Leave in getting the source file path"));
		return EFalse;
		}
	TRAP(error, imageDecoder = DecodeFileL(iFileSession, iSourceFile, *iSourceBitmap, 0, aRf, aDisplayMode));
	delete imageDecoder; imageDecoder = NULL;
		
	if (error != KErrNone)
		{
		INFO_PRINTF2(_L("DecodeFileL left with %d"), error);
		return EFalse;
		}	
	
	TRAP(error, GetTestFileNameL(iDestnFile, aRefFileName));
	if(error != KErrNone)	
		{
		INFO_PRINTF1(_L("Leave in getting the reference file path"));
		return EFalse;
		}
	error = iDestnBitmap->Load(iDestnFile);
	if (error != KErrNone)
		{
		INFO_PRINTF2(_L("Error in loading reference bitmap: %d"),error);
		return EFalse;
		}
	
	//prepare error string
	TBuf<200> buf;
	buf.Format(_L("failed to decode %S successfully, with reduction factor:%d display-mode:%d"), &aSourceFileName, aRf, aDisplayMode);
	iTestStepResult = EPass;
	Compare(*iSourceBitmap, *iDestnBitmap, buf);
	if(iTestStepResult != EPass)
		{
		INFO_PRINTF1(_L("Bitmap comparison failed"));
		return EFalse;		
		}
	return ETrue;				
	}


RTestStep_5100* RTestStep_5100::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5100(aSuite);
	}
	
RTestStep_5100::RTestStep_5100(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5100-HP");
	}

	
TVerdict RTestStep_5100::OpenL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);
	CleanupStack::Pop(scheduler);
	
	INFO_PRINTF1(_L("PDEF111193: iDecoder = CImageDecoder::FileNewL(iFs, aFile); is slow"));
	
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());
    
	GetTestFileNameL(iSourceFile, KPerformanceEnhanceTestJPG);	
	return iTestStepResult;
	}

// counter wrap around subtraction
TUint32 RTestStep_5100::ElaspedCounterTicks(TUint32 aStart, TUint32 aEnd)
	{
	if (aEnd >= aStart)
		{
		return (aEnd - aStart);
		}
	else
		{
		return (KMaxTUint32 - aStart + aEnd + 1);
		}
	}

TVerdict RTestStep_5100::DoTestStepL()
	{
	//UID for test resolvers
	TInt KResolverCnt = 4;
	TUid tstResolvers[] = {KUidTestIclResolverPlugin1, KUidTestIclResolverPlugin2,
							KUidTestIclResolverPlugin3, KUidTestIclResolverPlugin4};

	TUint32 startTime = 0;
	TUint32 endTime = 0;
	TUint32 durationPrevMethod = 0;
	TUint32 durationImprovMethod = 0;
	_LIT8(KDummyData,"dummy");
	for (TInt i = 0; i < 500; i++)
		{
		startTime = User::FastCounter();
		CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile);
		endTime = User::FastCounter();
		durationPrevMethod += ElaspedCounterTicks(startTime, endTime);
		delete decoder;
		decoder = NULL;
		//Load four different test resolvers so that ECom loses its cached icl custom plugin resolver
		TEComResolverParams resolverparams;
		//Set any resolver data type as it will never reach the resolving part
		resolverparams.SetDataType(KDummyData);
		RImplInfoPtrArray decoderList;
		for(TInt j = 0; j < KResolverCnt; j++)
			{
			REComSession::ListImplementationsL(TUid::Uid(KDecoderInterfaceUidValue), resolverparams, tstResolvers[j], decoderList);
			decoderList.ResetAndDestroy();
			}
		}
	
	for (TInt i = 0; i < 500; i++)
		{
		startTime = User::FastCounter();
		CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iSourceFile, CImageDecoder::EOptionNone, KNullUid, KNullUid, KJPGDecoderUid);
		endTime = User::FastCounter();
		durationImprovMethod += ElaspedCounterTicks(startTime, endTime);
		delete decoder;
		decoder = NULL;
		}

	INFO_PRINTF2(_L("Elapsed time for improved CImageDecoder::FileNewL() = %d"), durationImprovMethod);
	INFO_PRINTF2(_L("Elapsed time for previous CImageDecoder::FileNewL() = %d"), durationPrevMethod);
	
	if (durationImprovMethod >= durationPrevMethod)
		{
		INFO_PRINTF1(_L("Improved method was slower than previous method"));
		iTestStepResult = EFail;
		}
	else
		{
		INFO_PRINTF1(_L("Improved method was faster than previous method"));
		}
		
	return iTestStepResult;

	}
	
void RTestStep_5100::Close()
	{
	iFileSession.Close();
	}

RTestStep_5101* RTestStep_5101::NewL(CTestSuite* aSuite)
	{
	return new (ELeave)RTestStep_5101(aSuite);
	}

RTestStep_5101::RTestStep_5101(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName =_L("MM-ICL-COD-U-5101-HP");
	iHeapSize = 1024*256; // 256K
	}

TVerdict RTestStep_5101::OpenL()
	{
	INFO_PRINTF1(_L("PDEF112298: Mbm Bitmap is not encoded to JPEG correctly using CImageEncoder"));
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFs.Connect());

	iListener = new (ELeave) CActiveListener;		
	return iTestStepResult;
	}

TVerdict RTestStep_5101::DoTestStepL()
	{
	for (TInt i = 0; i < 2; i++)
		{
		TFileName refFileName;

		switch (i)
			{
		case 0:
			{
			GetTestFileNameL(iSourceFile, KImageTestSourceMbm1);
			GetTestFileNameL(refFileName, KImageTestTargetJpg1);
			break;
			}
		case 1:
		default:
			{
			GetTestFileNameL(iSourceFile, KImageTestSourceMbm2);
			GetTestFileNameL(refFileName, KImageTestTargetJpg2);
			break;
			}
			}

		INFO_PRINTF1(iSourceFile);

		CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(bitmap);
		User::LeaveIfError(bitmap->Load(iSourceFile));

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);
		TUid imageType;
		imageType.iUid = KImageTypeJPGUid.iUid;
		TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
		jpegFormat->iSampleScheme  = TJpegImageData::EColor422;
		jpegFormat->iQualityFactor = 90;
		TInt error = frameImageData->AppendImageData(jpegFormat);
		if (error != KErrNone)
			{
			CleanupStack::PopAndDestroy(frameImageData);
			CleanupStack::PopAndDestroy(bitmap); 
			iTestStepResult = EFail;
			break;
			}

		CImageEncoder* encoder = CImageEncoder::FileNewL(iFs, KImageTestTempFileName, CImageEncoder::EOptionNone, imageType);
		CleanupStack::PushL(encoder);

		iListener->InitialiseActiveListener();
		encoder->Convert(&iListener->iStatus, *bitmap, frameImageData);
		CActiveScheduler::Start();
		error = iListener->iStatus.Int(); 
 		User::LeaveIfError(error);
		CleanupStack::PopAndDestroy(encoder); 

		TVerdict matchesReferenceImage = EFail;
		TRAP(error, matchesReferenceImage = CompareFilesL(KImageTestTempFileName, refFileName));

		iFs.Delete(KImageTestTempFileName);
		CleanupStack::PopAndDestroy(frameImageData);
		CleanupStack::PopAndDestroy(bitmap); 

		if ((matchesReferenceImage == EFail) || (error != KErrNone))
			{
			INFO_PRINTF1(_L("Failed!"));
			iTestStepResult = EFail;
			}
		}

	return iTestStepResult;
	}

TVerdict RTestStep_5101::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2)
	{
	RFile file1, file2;

	User::LeaveIfError(file1.Open(iFs,aFile1,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file1);

	User::LeaveIfError(file2.Open(iFs,aFile2,EFileShareAny|EFileStream|EFileRead));
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

		while (length<fileSize1 && result == EPass)
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
			User::LeaveIfError(file1.Read(buf1Ptr,copySize));
			User::LeaveIfError(file2.Read(buf2Ptr,copySize));

			if (Mem::Compare(buf1Ptr.Ptr(),copySize,buf2Ptr.Ptr(),copySize)!=0)
				{
				result = EFail;
				}

			length += copySize;
			}
		CleanupStack::PopAndDestroy(2); // buf1, buf2
		}
	else
		{
		result = EFail;
		}

	CleanupStack::PopAndDestroy(2); //file1, file2

	return result;
	}

void RTestStep_5101::Close()
	{
	delete iListener;
	iListener=NULL;   
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	RFbsSession::Disconnect();
	}

//
// RTestStep_5110
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5110*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5110* RTestStep_5110::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5110(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5110::RTestStep_5110(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5110-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5110::OpenL()
	{
	INFO_PRINTF1(_L("WMF Zooming test (PDEF111932)"));

	iScheduler = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFileSession.Connect());

	return iTestStepResult;
	}

/**
 * Decode WMF images with/without mask generation.
 * Compare the decoded frame and mask with a reference image.
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5110::DoTestStepL()
	{
	TFileName testFileName;
	TFileName testFile;
	
	for (TInt i = 0; i < 7; i++)
		{
		testFileName.Format(KWmfFormatString, i + 2);
		GetTestFileNameL(testFile, testFileName);
		
		// Create the bitmaps.
		iSourceBitmap = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(iSourceBitmap);
		iReferenceBitmap = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(iReferenceBitmap);

		CImageDecoder* decoder = DecodeFileL(iFileSession, testFile, *iSourceBitmap, 0, i%4, EFalse, CImageDecoder::EOptionWmfIgnoreViewportMetaData);	// So we have the 4 different reduction rates
		CleanupStack::PushL(decoder);

		// Load the reference image.
		TFileName refFileName;
		TFileName refFile;
		refFileName.Format(KWmfRefFormatString,i+2,i%4);
		GetTestFileNameL(refFile, refFileName);
		
		User::LeaveIfError(iReferenceBitmap->Load(refFile));
		INFO_PRINTF1(refFile);

		//Compare the bitmaps
		Compare(*iSourceBitmap, *iReferenceBitmap, _L("Bitmaps are not the same"));
		CleanupStack::PopAndDestroy(decoder);

		// Delete all the bitmaps.
		CleanupStack::PopAndDestroy(2);
		}

	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5110::Close()
	{
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

// RTestStep_5111
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5111*"
 *			A pointer to the constructed test step.
 */
RTestStep_5111* RTestStep_5111::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5111(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 */
RTestStep_5111::RTestStep_5111(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5111-HP");
	// Increase the heap size for this test
	iHeapSize = 0x20000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 */
TVerdict RTestStep_5111::OpenL()
	{
	INFO_PRINTF1(_L("PDEF113475:Thumbnail is not displayed for DRM image of mime type image/x-png "));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	
	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	_LIT(KPng,"screenbackground.png");
	GetTestFileNameL(iFileName, KPng);
	INFO_PRINTF2(_L("Source: %S"),&iFileName);

	//Read the file into a descriptor
	TInt size;
	RFile file;
	User::LeaveIfError(file.Open(iFileSession, iFileName, EFileRead));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Size(size));
	iSource = HBufC8::NewMaxL(size);
	TPtr8 sourcePtr(iSource->Des());
	User::LeaveIfError(file.Read(sourcePtr));
	CleanupStack::PopAndDestroy(&file);
	return iTestStepResult;
	}

/**
 * Check if PNG decoder is found for MIME types image/x-png and image/png.
 * 
 * @return	"TVerdict"
 *			EPass if the test succeeded
 */
TVerdict RTestStep_5111::DoTestStepL()
	{
	iTestStepResult = EFail;
	_LIT8(KMimeXpng,"image/x-png");
	if(!CheckForDecoder(KMimeXpng))
		{
		INFO_PRINTF1(_L("No PNG decoder found for image/x-png"));
		return iTestStepResult;	
		}
	INFO_PRINTF1(_L("Checked with MIME image/x-png"));
	_LIT8(KMimePng,"image/png");
	if(!CheckForDecoder(KMimePng))
		{
		INFO_PRINTF1(_L("No PNG decoder found for image/png"));
		return iTestStepResult;	
		}
	INFO_PRINTF1(_L("Checked with MIME image/png"));
	iTestStepResult = EPass;
	return iTestStepResult;	
	}

/**
 * Free resources allocated in OpenL()
 *
 */
void RTestStep_5111::Close()
	{
	delete iSource; iSource = NULL;
	delete iScheduler; iScheduler = NULL;
	RFbsSession::Disconnect();
	iFileSession.Close();
	}

/**
 Checks if the decoder is found for the given MIME type.
 */
TBool RTestStep_5111::CheckForDecoder(const TDesC8 &aMIMEType)
	{
	TInt error;
	CImageDecoder* decoder = NULL;
	TBuf<200> buf;
	TRAP(error, decoder = CImageDecoder::DataNewL(iFileSession, *iSource, aMIMEType));
	delete decoder;
	decoder = NULL;
	if(error != KErrNone)
		{
		return EFalse;	
		}
	TRAP(error, decoder = CImageDecoder::FileNewL(iFileSession, iFileName, aMIMEType));
	delete decoder;
	decoder = NULL;
	if(error != KErrNone)
		{
		return EFalse;
		}
	return ETrue;		
	}

RTestStep_5099* RTestStep_5099::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5099(aSuite);
	}

RTestStep_5099::RTestStep_5099(CTestSuite* /*aSuite*/)
	{
	iTestStepName =_L("MM-ICL-COD-U-5099-HP");
	}

TVerdict RTestStep_5099::OpenL()
	{
	iScheduler = new(ELeave) CActiveScheduler();
	User::LeaveIfError(iFs.Connect());
	
	CActiveScheduler::Install(iScheduler);
	return EPass;
	}

void RTestStep_5099::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	}


typedef struct 
	{
	TInt iExtensionBits;	// A bitfield of extension options.
	TInt iExpectedUid;		// The expected implementation Uid.
	}
TTestExtensionsDataStruct;

#ifdef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS

const TUid KDummyImageType = { 0x1028340C };

static const TTestExtensionsDataStruct KTest5099Data[] =
	{
		// The first looks for a decoder that supports all extensions.
		// No decoder should be returned.
		{	0xff00,
			NULL	
		},
		{	CImageDecoder::EOptionExtCrop,
			KTestDummyDecoderExtCropUidValue
		},
		{	CImageDecoder::EOptionExtStream,
			KTestDummyDecoderExtStreamUidValue
		},
		{	CImageDecoder::EOptionExtRotation,
			KTestDummyDecoderExtRotationUidValue
		},
		{	CImageDecoder::EOptionExtMirrorHorizontalAxis,
			KTestDummyDecoderExtMirrorHorizontalVerticalAxisUidValue
		},
		{	CImageDecoder::EOptionExtMirrorVerticalAxis,
			KTestDummyDecoderExtMirrorHorizontalVerticalAxisUidValue
		},
		{	CImageDecoder::EOptionExtScaling,
			KTestDummyDecoderExtScalingUidValue
		},
		{	CImageDecoder::EOptionExtMirrorHorizontalAxis |
			CImageDecoder::EOptionExtMirrorVerticalAxis,
			KTestDummyDecoderExtMirrorHorizontalVerticalAxisUidValue
		},
		{	CImageDecoder::EOptionNone,
			KTestDummyDecoderExtStreamUidValue // This is not the first in the list of matched codecs but then the sort function puts it at the top of the equally matched codecs!
		}
	};

#define KTestCount5099	sizeof(KTest5099Data) / sizeof(TTestExtensionsDataStruct)

TVerdict RTestStep_5099::DoTestStepL()
	{
	_LIT(KDummyDecodeFile, "c:\\tsu_icl_cod_03\\test.dummy");
	_LIT8(KMimeDummy, "image/dummy");
	TBool failed = EFalse;

	for (TInt i = 0; i < KTestCount5099; i++)
		{
		TTestExtensionsDataStruct testdata = KTest5099Data[i];
		TInt expected = testdata.iExpectedUid;
		TInt loaded = 0;
		TInt err;
		CImageDecoder::TOptions options = static_cast<CImageDecoder::TOptions>(testdata.iExtensionBits);

		for (TInt function = 0; function < 5; function++)	// for each CImageDecoder::FileNewL/DataNewL function.
			{
			CImageDecoder* pDecoder = NULL;
				
			switch (function)
				{
				case 0:
					// Load by extension / source data
					TRAP(err, pDecoder = CImageDecoder::FileNewL(iFs, KDummyDecodeFile, options));
					break;
						
				case 1:
					// Load by MIME-type
					TRAP(err, pDecoder = CImageDecoder::FileNewL(iFs,KDummyDecodeFile, KMimeDummy, options));
					break;
						
				case 2:
					// Load by ImageTypeUid
					TRAP(err, pDecoder = CImageDecoder::FileNewL(iFs, KDummyDecodeFile, options, KDummyImageType));
					break;
				
				case 3:
				case 4:
					// Load by file data.
					{
					TBuf8<1024> buffer;
					RFile file;
					
					err = file.Open(iFs, KDummyDecodeFile, EFileRead);
					if (err != KErrNone)
						{
						ERR_PRINTF3(_L("Failed to open file \"%S\". Reason %d"), &KDummyDecodeFile, err);
						failed = ETrue;
						continue;	
						}
					
					buffer.Zero();
					err = file.Read(buffer);
					file.Close();
					if (err != KErrNone)
						{
						ERR_PRINTF3(_L("Failed to read file \"%S\". Readson %d"), &KDummyDecodeFile, err);
						failed = ETrue;
						continue;
						}
					
					switch (function)
						{
						case 3:
							TRAP(err, pDecoder = CImageDecoder::DataNewL(iFs, buffer, KMimeDummy, options));
							break;
							
						case 4:
							TRAP(err, pDecoder = CImageDecoder::DataNewL(iFs, buffer, options, KDummyImageType));
							break;
						}
					}
					break;
						
				default:
					ERR_PRINTF2(_L("Non-existent CImageDecoder::FileNewL function (%d)"), function);
					continue;
					}
					
			// Check the results.
			if (err == KErrNone)
				{
				loaded = pDecoder->ImplementationUid().iUid;
				delete pDecoder;
				
				// Make sure we have the right encoder.
				if (loaded != expected)
					{
					ERR_PRINTF3(_L("Failed to load decoder %08X, got %08X"), expected, loaded);
					failed = ETrue;
					}
				}
			else
				{
				// Check for cases where we expect no decoder to be returned.
				if ((expected == NULL) && (err == KErrNotFound))
					{
					// It's ok!
					continue;
					}
				
				ERR_PRINTF3(_L("Failed to load decoder %08X, reason %d"), expected, err);
				failed = ETrue;
				}
			}

		} // for each test case.

	return (failed ? EFail : EPass);
	}
#else
TVerdict RTestStep_5099::DoTestStepL()
	{
	TVerdict verdict = EPass;

	// ensure that we cannot obtain extensions on jpeg decoder codec
	_LIT(KJpegDecodeFile, "c:\\tsu_icl_cod_03\\qvga.jpg");
	CImageDecoder* decoder = NULL;
	TRAPD(err, decoder = CImageDecoder::FileNewL(iFs,KJpegDecodeFile));
	if ( decoder )
		{
		TInt loaded = decoder->ImplementationUid().iUid;
		if ( !(loaded == KJPGDecoderImplementationUidValue || loaded == KJPGDecoderImplementation2UidValue) )
			{
			ERR_PRINTF2(_L("Failed to load decoder got %08X"), loaded);
			verdict = EFail;
			}
		TRAP(err,decoder->SetClippingRectL(NULL));
		if ( err != KErrNotSupported )
			{
			verdict = EFail;
			}
		TImageConvScaler* scaler = NULL;
		TRAP(err,scaler = decoder->ScalerL());
		if ( (scaler != NULL) || (err != KErrNotSupported) )
			{
			verdict = EFail;
			}			
		TImageConvOperation* operation = NULL;
		TRAP(err,operation = decoder->OperationL());
		if ( (operation != NULL) || (err != KErrNotSupported) )
			{
			verdict = EFail;
			}			
		TImageConvStreamedDecode* blockStreamer = NULL;
		TRAP(err,blockStreamer = decoder->BlockStreamerL());
		if ( (blockStreamer != NULL) || (err != KErrNotSupported) )
			{
			verdict = EFail;
			}			
		delete decoder; decoder = NULL;
		}
	else
		{
		verdict = EFail;
		}
	return verdict;
	}
#endif
	
RTestStep_5102* RTestStep_5102::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5102(aSuite);
	}

RTestStep_5102::RTestStep_5102(CTestSuite* /*aSuite*/)
	{
	iTestStepName =_L("MM-ICL-COD-U-5102-HP");
	}

TVerdict RTestStep_5102::OpenL()
	{
	iScheduler = new(ELeave) CActiveScheduler();
	User::LeaveIfError(iFs.Connect());
	
	CActiveScheduler::Install(iScheduler);
	return EPass;
	}

void RTestStep_5102::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	}

#ifdef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
static const TTestExtensionsDataStruct KTest5102Data[] =
	{
		{	0xff00,
			NULL
		},
		{	CImageEncoder::EOptionExtStream,
			KTestDummyEncoderExtStreamUidValue
		},
		{	CImageEncoder::EOptionExtRotation,
			KTestDummyEncoderExtRotationUidValue
		},
		{	CImageEncoder::EOptionExtMirrorHorizontalAxis,
			KTestDummyEncoderExtMirrorHorizontalVerticalAxisUidValue
		},
		{	CImageEncoder::EOptionExtMirrorVerticalAxis,
			KTestDummyEncoderExtMirrorHorizontalVerticalAxisUidValue
		},
		{	CImageEncoder::EOptionExtUseWithSetThumbnail,
			KTestDummyEncoderExtUseWithSetThumbnailUidValue
		},
		{	CImageEncoder::EOptionExtMirrorHorizontalAxis |
			CImageEncoder::EOptionExtMirrorVerticalAxis,
			KTestDummyEncoderExtMirrorHorizontalVerticalAxisUidValue
		},
		{	CImageEncoder::EOptionNone,
			KTestDummyEncoderExtRotationUidValue // This is not the first in the list of matched codecs but then the sort function puts it at the top of the equally matched codecs!
		}
	};

#define KTestCount5102	sizeof(KTest5102Data) / sizeof(TTestExtensionsDataStruct)

TVerdict RTestStep_5102::DoTestStepL()
	{
	_LIT(KDummyEncodeFile, "c:\\tsu_icl_cod_03\\temp.dat");
	_LIT8(KMimeDummy, "image/dummy");
	TBool failed = EFalse;

	for (TInt i = 0; i < KTestCount5102; i++)
		{
		TTestExtensionsDataStruct testdata = KTest5102Data[i];
		TInt expected = testdata.iExpectedUid;
		TInt loaded = 0;
		TInt err;
		CImageEncoder::TOptions options = static_cast<CImageEncoder::TOptions>(testdata.iExtensionBits);

		for (TInt function = 0; function < 2; function++)	// for each CImageEncoder::FileNewL function.
			{
			CImageEncoder* pEncoder = NULL;
			
			switch (function)
				{
				case 0:
					// Load by MIME-type
					// Encoder extensions are not marked as ESubCodecs in
					// the RSS file so these calls should work.
					TRAP(err, pEncoder = CImageEncoder::FileNewL(iFs,
																KDummyEncodeFile,
																KMimeDummy,
																options
																));
					break;
						
				case 1:
					// Load by ImageTypeUid
					TRAP(err, pEncoder = CImageEncoder::FileNewL(iFs,
																KDummyEncodeFile,
																options,
																KDummyImageType
																));
					break;
					
				default:
					ERR_PRINTF2(_L("Non-existent CImageEncoder::FileNewL function (%d)"), function);
					continue;
					}
					
			// Check the results.
			if (err == KErrNone)
				{
				loaded = pEncoder->ImplementationUid().iUid;
				delete pEncoder;
					
				// Make sure we have the right encoder.
				if (loaded != expected)
					{
					ERR_PRINTF3(_L("Failed to load encoder %08X, got %08X"), expected, loaded);
					failed = ETrue;
					}
				}
			else
				{
				// Check if no encoder is expected
				if ((expected == NULL) && (err == KErrNotFound))
					{
					continue;
					}
				
				ERR_PRINTF3(_L("Failed to load encoder %08X, reason %d"), expected, err);
				failed = ETrue;
				}
			}

		} // for each test case.

	return (failed ? EFail : EPass);
	}
#else
TVerdict RTestStep_5102::DoTestStepL()
	{
	TVerdict verdict = EPass;
	_LIT(KJpegEncodeFile, "c:\\tsu_icl_cod_03\\temp.jpg");
	_LIT8(KMimeJpeg, "image/jpeg");
	CImageEncoder* encoder = NULL;
	TRAPD(err, encoder = CImageEncoder::FileNewL(iFs,KJpegEncodeFile,KMimeJpeg));
	// ensure that we cannot obtain extensions on jpeg encoder codec
	if ( encoder )
		{
		TInt loaded = encoder->ImplementationUid().iUid;
		if ( !(loaded == KJPGEncoderImplementationUidValue || loaded == KJPGEncoderImplementation2UidValue) )
			{
			ERR_PRINTF2(_L("Failed to load encoder got %08X"), loaded);
			verdict = EFail;
			}
		TImageConvOperation* operation = NULL;
		TRAP(err,operation = encoder->OperationL());
		if ( (operation != NULL) || (err != KErrNotSupported) )
			{
			verdict = EFail;
			}			
		TImageConvStreamedEncode* blockStreamer = NULL;
		TRAP(err,blockStreamer = encoder->BlockStreamerL());
		if ( (blockStreamer != NULL) || (err != KErrNotSupported) )
			{
			verdict = EFail;
			}			
		delete encoder; encoder = NULL;
		}
	else
		{
		verdict = EFail;
		}
	return verdict;
	}
#endif



//
// RTestStep_5103
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5102*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5103* RTestStep_5103::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTestStep_5103(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5103::RTestStep_5103(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5103-HP");
	// Increase the heap size for this test
	iHeapSize = 0x40000;
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files and data to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5103::OpenL()
	{
	INFO_PRINTF1(_L("DEF113733: Encoding a color display mode bitmap to a jpeg monochrome thumbnail ."));

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFileSession.Connect());

	GetTestFileNameL(iSourceFile, KOS2Depth24BmpRef);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	GetTestFileNameL(iSavedFileName, KJFIFThumbnailBW);
	INFO_PRINTF2(_L("Saved File: %S"),&iSavedFileName);

	return iTestStepResult;
	}

/**
 * Test to check that encode/decode of JFIF jpg file with thumbnail works
 *
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
TVerdict RTestStep_5103::DoTestStepL()
	{
	// encode the mbm as a JFIF jpg
	TFileName createFileName;
	GetTestFileNameL(createFileName, KJFIFThumbnailBW);
	TFileName refFileName;
	GetTestFileNameL(refFileName, KJFIFThumbnailBWRef);

	RFile file;
	User::LeaveIfError(file.Replace(iFileSession, createFileName, EFileWrite | EFileShareAny));
	file.Close();
		
	CImageEncoder* encoder = NULL;
	TUid jpgEncUid = {0x101F45D9};	// JPG encoder UID!
	encoder = CImageEncoder::FileNewL(iFileSession, createFileName, CImageEncoder::EOptionNone, KImageTypeJPGUid, KNullUid, jpgEncUid);
	CleanupStack::PushL(encoder);
	// include a thumbnail
	encoder->SetThumbnail(ETrue);
			
	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	listener->InitialiseActiveListener();
		
	iSourceBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iSourceBitmap->Load(iSourceFile));
	CFrameImageData* frameImageData = CFrameImageData::NewL();
	CleanupStack::PushL(frameImageData);
	TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
	jpegFormat->iSampleScheme  = TJpegImageData::EMonochrome;
	User::LeaveIfError(frameImageData->AppendImageData(jpegFormat));

	encoder->Convert(&listener->iStatus, *iSourceBitmap,frameImageData);
	CActiveScheduler::Start();
				
	if (listener->iStatus.Int() != KErrNone)
		{
		INFO_PRINTF1(_L("CImageEncoder::Convert failed"));
		iTestStepResult = EFail;		
		}

	CleanupStack::PopAndDestroy(3); // listener,encoder, frameImageData

	if (iTestStepResult == EPass)
		{
		TVerdict matchesReferenceImage = EFail;
		TInt error;
		TRAP(error, matchesReferenceImage = CompareFilesL(createFileName, refFileName));
		
		if ((matchesReferenceImage == EFail) || (error != KErrNone))
			{
			INFO_PRINTF1(_L("Encoded file does not match reference"));
			iTestStepResult = EFail;
			}
		}

	return iTestStepResult;
	}

TVerdict RTestStep_5103::CompareFilesL(const TDesC& aFile1,const TDesC& aFile2)
	{
	RFile file1, file2;

	User::LeaveIfError(file1.Open(iFileSession,aFile1,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(file1);

	User::LeaveIfError(file2.Open(iFileSession,aFile2,EFileShareAny|EFileStream|EFileRead));
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

		while (length<fileSize1 && result == EPass)
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
			User::LeaveIfError(file1.Read(buf1Ptr,copySize));
			User::LeaveIfError(file2.Read(buf2Ptr,copySize));

			if (Mem::Compare(buf1Ptr.Ptr(),copySize,buf2Ptr.Ptr(),copySize)!=0)
				{
				result = EFail;
				}

			length += copySize;
			}
		CleanupStack::PopAndDestroy(2); // buf1, buf2
		}
	else
		{
		result = EFail;
		}

	CleanupStack::PopAndDestroy(2); //file1, file2

	return result;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5103::Close()
	{
	delete iSourceBitmap; iSourceBitmap = NULL;
	delete iReferenceBitmap; iReferenceBitmap = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Delete(iSavedFileName);
	iFileSession.Close();
	RFbsSession::Disconnect();
	}


//
// DEF114840: Panic observed while saving an image
//
	
RTestStep_5104* RTestStep_5104::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5104(aSuite);
	}

RTestStep_5104::RTestStep_5104(CTestSuite* aSuite)
	{
	if (!aSuite)
		{
		User::Leave(KErrArgument);
		}

	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5104-HP");
	}


TVerdict RTestStep_5104::OpenL()
	{
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFs.Connect());

	iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	return EPass;
	}


void RTestStep_5104::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	RFbsSession::Disconnect();
	}


TVerdict RTestStep_5104::DoTestStepL()
	{
	TInt err = KErrNone;
		
	CImageEncoder* encoder = NULL;
	
	TRAP(err, encoder = CImageEncoder::FileNewL(iFs, KImageJpegTemp, CImageEncoder::EOptionNone, KImageTypeJPGUid));
	CleanupStack::PushL(encoder);
	
	CFbsBitmap* source = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(source);
	
	err = source->Create(TSize(0, 0), EColor16M);
	if (err != KErrNone)
		{
		INFO_PRINTF2(_L("Failed to create bitmap: %d"), err);
		User::Leave(err);
		}
	
	CActiveListener* listener = new(ELeave) CActiveListener();
	CleanupStack::PushL(listener);
	
	listener->InitialiseActiveListener();	
	encoder->Convert(&listener->iStatus, *source);
	CActiveScheduler::Start();
	err = listener->iStatus.Int();
	
	CleanupStack::PopAndDestroy(); // listener
	CleanupStack::PopAndDestroy(); // source
	CleanupStack::PopAndDestroy(); // encoder
	
	return (err == KErrArgument ? EPass : EFail);
	}

//
// RTestStep_5112
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5112*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5112* RTestStep_5112::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5112(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5112::RTestStep_5112(CTestSuite* aSuite)
	{
	if (!aSuite)
		{
		User::Leave(KErrArgument);
		}

	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5112-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5112::OpenL()
	{
	INFO_PRINTF1(_L("INC116806: Media files: Open image folder containing gif89 images results in a Main crash"));
	
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFs.Connect());

	iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	iListener = new (ELeave) CActiveListener;
	iStatus = &iListener->iStatus;
	
	GetTestFileNameL(iSourceFile, KGif89);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	TFileName referenceBitmapMaskFile;
	GetTestFileNameL(referenceBitmapMaskFile, KGif89RefReducedSizeMask);
	INFO_PRINTF2(_L("Reference Bitmap Mask: %S"),&referenceBitmapMaskFile);

	iReferenceBitmapMask = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iReferenceBitmapMask->Load(referenceBitmapMaskFile));
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5112::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	RFbsSession::Disconnect();
	}


TVerdict RTestStep_5112::DoTestStepL()
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, iSourceFile);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo();
	const TSize fullFrameSize(frameInfo.iOverallSizeInPixels);
	
	//half size 
	TInt reductionFactor=1;
	
	TSize reducedFrameSize;

 	INFO_PRINTF2(_L("Reduction Factor: %d"),reductionFactor);
	User::LeaveIfError(decoder->ReducedSize(fullFrameSize, reductionFactor, reducedFrameSize));
	
	//Create a bitmap of size ReducedFrameSize 
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(reducedFrameSize, frameInfo.iFrameDisplayMode));
	
	//Create a bitmap mask of size ReducedFrameSize 
	CFbsBitmap* bitmapMask = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmapMask);
	User::LeaveIfError(bitmapMask->Create(reducedFrameSize, EGray256));
	
	// Set iListener Active
	iListener->InitialiseActiveListener();
	
	// Call Convert
	decoder->Convert(iStatus, *bitmap, *bitmapMask);
	TInt error = WaitForResult(iStatus);
	
	//Verify the conversion was sucessful
	Compare(error,KErrNone,_L("Conversion failed"));
	
	//Compare the bitmap mask
	Compare(*bitmapMask, *iReferenceBitmapMask, _L("Bitmap mask not the same"));

	INFO_PRINTF1(_L("Bitmap decode finished"));
	
	CleanupStack::PopAndDestroy(3, decoder);

	return iTestStepResult;
	}

//
// RTestStep_5113
//
/**
 * Factory function for this test
 *
 * @return	"RTestStep_5113*"
 *			A pointer to the constructed test step.
 * @xxxx
 */
RTestStep_5113* RTestStep_5113::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5113(aSuite);
	}

/**
 * Constructor set the test step name according to the
 * the test that was requested.
 * 
 * @xxxx
 */
RTestStep_5113::RTestStep_5113(CTestSuite* aSuite)
	{
	if (!aSuite)
		{
		User::Leave(KErrArgument);
		}

	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5113-HP");
	}

/**
 * Initialise
 * 1) ActiveScheduler
 * 2) File Server session
 * 3) Bitmap Server connection
 * 4) Test files to use
 * @return	"TVerdict"
 *			EPass if the setup was successful
 * @xxxx
 */
TVerdict RTestStep_5113::OpenL()
	{
	INFO_PRINTF1(_L("INC117891: 5.0 LWS: Browser crashes while loading the page"));
	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFs.Connect());

	iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	GetTestFileNameL(iSourceFile, KBkgd);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	
	return iTestStepResult;
	}

/**
 * Free resources allocated in SetupL()
 *
 * @xxxx
 */
void RTestStep_5113::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	RFbsSession::Disconnect();
	}


TVerdict RTestStep_5113::DoTestStepL()
	{
	
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	
	
	//Decode the image
	CImageDecoder* decoder = DecodeFileL(iFs, iSourceFile, *bitmap);
	CleanupStack::PushL(decoder);
		
	//Make sure the last frame's header was processed
	Compare(decoder->IsImageHeaderProcessingComplete(),ETrue,_L("Last frame header not processed"));
	
	INFO_PRINTF1(_L("Bitmap decode finished"));
	
	CleanupStack::PopAndDestroy(2, bitmap);

	return iTestStepResult;
	}


//
// RTestStep_5114
//
_LIT(KNotCorruptGif2, "refbut4.gif");
_LIT(KNotCorruptGif2Ref, "refbut4.mbm");

RTestStep_5114* RTestStep_5114::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5114(aSuite);
	}


RTestStep_5114::RTestStep_5114(CTestSuite* aSuite)
	{
	if (!aSuite)
		{
		User::Leave(KErrArgument);
		}

	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5114-HP");
	}


TVerdict RTestStep_5114::OpenL()
	{
	INFO_PRINTF1(_L("PDEF123012: 5.0 :Browser crashes while loading the homepage for www.refdesk.com"));
	
	User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());
	User::LeaveIfError(iFs.Connect());

	iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	GetTestFileNameL(iSourceFile, KNotCorruptGif2);
	INFO_PRINTF2(_L("Source: %S"),&iSourceFile);
	
	return iTestStepResult;
	}


void RTestStep_5114::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	RFbsSession::Disconnect();
	}


TVerdict RTestStep_5114::DoTestStepL()
	{
	TInt err;
	
	iTestStepResult = EFail;
	
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	
	// Decode the image
	CImageDecoder* decoder = NULL;
	TRAP(err, decoder = DecodeFileL(iFs, iSourceFile, *bitmap));
	delete decoder;
	
	if (err == KErrNone)
		{
		TFileName refFile;
		GetTestFileNameL(refFile, KNotCorruptGif2Ref);

		CFbsBitmap* refBmp = new(ELeave) CFbsBitmap();
		CleanupStack::PushL(refBmp);
		
		err = refBmp->Load(refFile);
		if (err != KErrNone)
			{
			ERR_PRINTF3(_L("Error %d loading reference image %S"), err, &refFile);
			User::Leave(err);
			}
		
		if (Compare(*bitmap, *refBmp) == KErrNone)
			{
			iTestStepResult = EPass;
			}
		
		CleanupStack::PopAndDestroy();	// refBmp
		}
	else
		{
		ERR_PRINTF3(_L("Error %d loading image %S"), err, &iSourceFile);
		}
	
	CleanupStack::PopAndDestroy();	// bitmap

	return iTestStepResult;
	}

//
// PDEF127500 : Error decoding a specific .gif
// =========================================================
//
RTestStep_5106::RTestStep_5106(CTestSuite* aSuite)
	{
	if (!aSuite)
		{
		User::Leave(KErrArgument);
		}

	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-COD-U-5106-HP");
	}


void RTestStep_5106::Close()
	{
	delete iScheduler;
	iScheduler = NULL;
	iFs.Close();
	RFbsSession::Disconnect();
	}


RTestStep_5106* RTestStep_5106::NewL(CTestSuite* aSuite)
	{
	return new(ELeave) RTestStep_5106(aSuite);
	}


TVerdict RTestStep_5106::OpenL()
	{
    User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFs.Connect());

	iScheduler = new(ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	return EPass;
	}


_LIT(KAlermGif, "c:\\tsu_icl_cod_03\\alerm.gif");
TVerdict RTestStep_5106::DoTestStepL()
	{
	CImageDecoder* decoder = NULL;
	TRAPD(err, decoder = CImageDecoder::FileNewL(iFs, KAlermGif));
	if (err != KErrNone)
		{
		ERR_PRINTF3(_L("Error %d loading image %S"), err, &KAlermGif);
		User::Leave(err);
		}
	
	CleanupStack::PushL(decoder);
	
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	
	const TFrameInfo& frame(decoder->FrameInfo(0));
	err = bitmap->Create(frame.iOverallSizeInPixels, frame.iFrameDisplayMode);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error %d creating bitmap for image"), err);
		User::Leave(err);
		}
	
	CActiveListener listener;
	listener.InitialiseActiveListener();
	
	decoder->Convert(&listener.iStatus, *bitmap);
	CActiveScheduler::Start();
	err = listener.iStatus.Int();
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error %d decoding image"), err);
		User::Leave(err);
		}
	
	CleanupStack::PopAndDestroy(bitmap);
	CleanupStack::PopAndDestroy(decoder);
	
	return EPass;
	}


//
// PDEF138486 : Error decoding a specific .gif
// =========================================================
//
RTestStep_5107::RTestStep_5107(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-COD-U-5107-HP");
    }


void RTestStep_5107::Close()
    {
    delete iScheduler;
    iScheduler = NULL;
    iFs.Close();
    RFbsSession::Disconnect();
    }


RTestStep_5107* RTestStep_5107::NewL(CTestSuite* aSuite)
    {
    if (!aSuite)
        {
        User::Leave(KErrArgument);        
        }
    return new(ELeave) RTestStep_5107(aSuite);
    }


TVerdict RTestStep_5107::OpenL()
    {
	User::LeaveIfError(FbsStartup());
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFs.Connect());

    iScheduler = new(ELeave) CActiveScheduler();
    CActiveScheduler::Install(iScheduler);
    
    return EPass;
    }


_LIT(KSpecificGif, "c:\\tsu_icl_cod_03\\00000077.gif");
TVerdict RTestStep_5107::DoTestStepL()
    {
    INFO_PRINTF2(_L("Attempting to decode %S"), &KSpecificGif);
    CBufferedImageDecoder *decoder = NULL;
    const TInt KChunkSize = 32;
    TInt err; 
            
    // Open the file containing the image 
    RFile file;
    TUint flags = EFileShareReadersOnly | EFileStream | EFileRead;
    err = file.Open(iFs, KSpecificGif, flags);
    if(err != KErrNone)
        {
        INFO_PRINTF1(_L("Failed to open file"));
        User::Leave(err);
        }
    CleanupClosePushL(file);
    
    // Allocate the buffer for the file read in chunks
    HBufC8* buffer = HBufC8::NewLC(KChunkSize);
    TPtr8 bufferPtr(buffer->Des());

    // Read the first chunk from the file
    err = file.Read(bufferPtr, KChunkSize);
    if(err != KErrNone)
        {
        INFO_PRINTF1(_L("File read error encountered"));
        User::Leave(err);
        }
    
    // Create the decoder passing the File Server session and attempt to open the decoder
    TRAP(err, decoder = CBufferedImageDecoder::NewL(iFs));
    if(err != KErrNone)
        {
        INFO_PRINTF1(_L("CBufferedImageDecoder::NewL() failed"));
        User::Leave(err);
        }
    CleanupStack::PushL(decoder);

    TRAP(err, decoder->OpenL(bufferPtr));
    if (err != KErrNone)
        {
        INFO_PRINTF1(_L("CBufferedImageDecoder::OpenL() failed"));
        User::Leave(err);
        }

    while (!decoder->ValidDecoder())
        {
        err = file.Read(bufferPtr, KChunkSize);
        if(err != KErrNone || 0 == bufferPtr.Length())
            {
            INFO_PRINTF1(_L("File read error encountered"));
            User::Leave(err);
            }
        TRAP(err, decoder->AppendDataL(bufferPtr));
        if(err != KErrNone)
            {
            INFO_PRINTF1(_L("CBufferedImageDecoder::AppendDataL() failed"));
            User::Leave(err);
            }
        TRAP(err, decoder->ContinueOpenL());
        if (err != KErrNone)
            {
            INFO_PRINTF1(_L("CBufferedImageDecoder::ContinueOpenL() failed"));
            User::Leave(err);
            }
        }
    
    // Make sure the header has been fully processed.
    while (!decoder->IsImageHeaderProcessingComplete())
        {
        err = file.Read(bufferPtr, KChunkSize);
        if(err != KErrNone ||  0 == bufferPtr.Length())
            {
            INFO_PRINTF1(_L("File read error encountered"));
            User::Leave(err);
            }
        TRAP(err, decoder->AppendDataL(bufferPtr));
        if(err != KErrNone)
            {
            INFO_PRINTF1(_L("CBufferedImageDecoder::AppendDataL() failed"));
            User::Leave(err);
            }
        TRAP(err, decoder->ContinueProcessingHeaderL());
        if(err != KErrNone)
            {
            INFO_PRINTF1(_L("CBufferedImageDecoder::ContinueProcessingHeaderL() failed"));
            User::Leave(err);
            }   
        }
    
    CleanupStack::PopAndDestroy(3); // file, buffer, decoder 

    return EPass;
    }

//AKAL-7YHLPR : Crash found in Themes Thread.
RTestStep_5108::RTestStep_5108(CTestSuite* aSuite)
    {
    iSuite = aSuite;
    iTestStepName = _L("MM-ICL-COD-U-5108-HP");
    }


void RTestStep_5108::Close()
    {
    delete iScheduler;
    iScheduler = NULL;
    iFs.Close();
    RFbsSession::Disconnect();
    }


RTestStep_5108* RTestStep_5108::NewL(CTestSuite* aSuite)
    {
    if (!aSuite)
        {
        User::Leave(KErrArgument);        
        }
    return new(ELeave) RTestStep_5108(aSuite);
    }


TVerdict RTestStep_5108::OpenL()
    {
    User::LeaveIfError(RFbsSession::Connect());
    User::LeaveIfError(iFs.Connect());

    iScheduler = new(ELeave) CActiveScheduler();
    CActiveScheduler::Install(iScheduler);
    
    return EPass;
    }


/*
 * Test added for AKAL-7YHLPR : Crash found in Themes Thread.
 * The test is to check the possible memory corruption on decoding a test Gif image. The gif image has transparency
 * color index of 128. Decode option is Fast decode and with EColor64k. With these combination, the memory corruption 
 * happens. The test decodes all the frames in the image.
 * If memory corruption happens, the test will panic and fail. No panic means the problem is fixed, and the test passes.
 * The test also fails if all frames are not decoded successfully. 
 */
TVerdict RTestStep_5108::DoTestStepL()
    {
    INFO_PRINTF1(_L("AKAL-7YHLPR : Crash found in Themes Thread."));
    INFO_PRINTF1(_L("Test checks for memory corruption on decoding Gif image with Tranparency index = 128, with EColor64K and in Fast decode mode"));
    _LIT(KGifTranpIdx128, "c:\\tsu_icl_cod_03\\secret.gif");
    CImageDecoder* decoder = NULL;
    //create decoder with Fast decode 
    decoder = CImageDecoder::FileNewL(iFs, KGifTranpIdx128, CImageDecoder::EPreferFastDecode);
    CleanupStack::PushL(decoder);
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap;
    //bitmap with EColor64K
    TInt err = bitmap->Create(TSize(0,0), EColor64K);
    CleanupStack::PushL(bitmap);
    CActiveListener* listener = new (ELeave) CActiveListener;
    CleanupStack::PushL(listener);//get the no. of frames in the image.
    TInt frameCount = decoder->FrameCount();
    TVerdict result = EPass;
    //decode all the frames.
    for(TInt i = 0; i < frameCount; i++)
        {
        const TFrameInfo& frameInfo = decoder->FrameInfo(i);
        const TSize frameSize(frameInfo.iOverallSizeInPixels);
        listener->InitialiseActiveListener();
        err = bitmap->Resize(frameSize);
        decoder->Convert(&listener->iStatus, *bitmap,i);
        CActiveScheduler::Start();
        err = listener->iStatus.Int();
        if(err != KErrNone)
            {
            result = EFail; //fail if couldn't decode all frames.
            break;
            }
        err = bitmap->Resize(TSize(0,0));
        }
    CleanupStack::PopAndDestroy(3); //listener, bitmap, decoder
    return result;
    }

//DEF144646 : Genius Case #144754: Nokia N97-1: Israel: N97 TIFF files not displayed properly. 
    RTestStep_5109::RTestStep_5109(CTestSuite* aSuite) 
        { 
        iSuite = aSuite; 
        iTestStepName = _L("MM-ICL-COD-U-5109-HP"); 
        } 
      
    void RTestStep_5109::Close() 
        { 
        delete iScheduler; 
        iScheduler = NULL; 
        iFs.Close(); 
        RFbsSession::Disconnect(); 
        } 
      
    RTestStep_5109* RTestStep_5109::NewL(CTestSuite* aSuite) 
        { 
        if (!aSuite) 
            { 
            User::Leave(KErrArgument);         
            } 
        return new(ELeave) RTestStep_5109(aSuite); 
        } 
      
    TVerdict RTestStep_5109::OpenL() 
        { 
        User::LeaveIfError(RFbsSession::Connect()); 
        User::LeaveIfError(iFs.Connect()); 
      
        iScheduler = new(ELeave) CActiveScheduler(); 
        CActiveScheduler::Install(iScheduler); 
         
        return EPass; 
        } 
      
    /* 
     * Test added for PDEF144975: TIFF files not displayed properly 
     * The test is to check the decoding a test tif image.  
     */ 
    TVerdict RTestStep_5109::DoTestStepL() 
        { 
        INFO_PRINTF1(_L("PDEF144975: TIFF files not displayed properly")); 
        INFO_PRINTF1(_L("Test is to check the decoding of below tif image")); 
        _LIT(KTifImage, "c:\\tsu_icl_cod_03\\3178500m.tif"); 
        CImageDecoder* decoder = NULL; 
        //create decoder  
        decoder = CImageDecoder::FileNewL(iFs, KTifImage, CImageDecoder::EOptionNone); 
        CleanupStack::PushL(decoder); 
         
        //Get frameinformation 
        TInt frameNo = 0; 
        const TFrameInfo& frameInfo = decoder->FrameInfo(frameNo); 
         
        //Create a destination bitmap 
        CFbsBitmap* bitmap = new (ELeave) CFbsBitmap; 
        TInt err = bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode); 
        User::LeaveIfError(err); 
        CleanupStack::PushL(bitmap); 
         
        CActiveListener* listener = new (ELeave) CActiveListener; 
        CleanupStack::PushL(listener); 
        TVerdict result = EPass; 
         
        //decode the frame. 
        listener->InitialiseActiveListener(); 
        decoder->Convert(&listener->iStatus, *bitmap, frameNo); 
        CActiveScheduler::Start(); 
        err = listener->iStatus.Int(); 
        if(err != KErrNone) 
          { 
          result = EFail; //fail if couldn't decode a frame. 
          } 
        CleanupStack::PopAndDestroy(3); //listener, bitmap, decoder 
        return result; 
        }
