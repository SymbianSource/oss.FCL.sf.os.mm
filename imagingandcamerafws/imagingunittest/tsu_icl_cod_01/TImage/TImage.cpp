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

#include <hal.h>
#include <hal_data.h>
#include <dptest.h> // Required to check for demand paging.
#if defined(__ICL_PROFILING)
#include <ecom/ecom.h> // used when profiling
#endif //defined(__ICL_PROFILING)
#include <icl/imageprocessor.h>
#include <iclexif.h>
#include <exifutility.h>
#include <e32math.h>
#include <e32std.h>
#include <iclexifimageframe.h>

//#define GENERATE_EXTENSION_OUTPUT 1 // create unique output files for extension testing

_LIT(KJpegFileName,"jfif444.jpg");
_LIT(KJpegFilePath,"c:\\icl\\testimages\\");
_LIT(KJpegFileOutPath,"c:\\icl\\");

_LIT8(KJpegMimeType,"image/jpeg");

_LIT(KImageTestSourceFileName, "\\TImage\\imagevid_8.mbm");
_LIT(KImageTestOtaSourceFileName, "\\TImage\\anon\\otabitmap.mbm");
_LIT(KImageTestOtaFileName, "\\TImage\\anon\\otabitmap");
_LIT(KImageTestWbmpSourceFileName, "\\TImage\\anon\\wirelessbitmap.mbm");
_LIT(KImageTestWbmpFileName, "\\TImage\\anon\\wirelessbitmap");
_LIT(KImageTestMaskFileName, "\\TImage\\trans_1.gif");
_LIT(KImageTestRefDir, "\\TImage\\ref\\");
_LIT(KImageTestOutDir, "\\icl\\outputimages\\");

_LIT(KImageTestTempFileName,"c:\\imagebmptemp");
_LIT(KImageTempFileName,"c:\\imagetemp");

_LIT(KImageTestRefExt,".mbm");
_LIT(KImageTestSourceScaledFileName,"ref\\scaled");


_LIT(KImageTest1BppMonoMbm,"mbm1bpp_2.mbm");
_LIT(KImageTest2BppMonoMbm,"mbm2bpp_8.mbm");
_LIT(KImageTest4BppMonoMbm,"mbm4bppm_2.mbm");
_LIT(KImageTest8BppMonoMbm,"mbm8bppm_8.mbm");
_LIT(KImageTest4BppColorMbm,"mbm4bppc_1.mbm");
_LIT(KImageTest8BppColorMbm,"mbm8bppc_4.mbm");
_LIT(KImageTest12BppColorMbm,"mbm12bpp_8.mbm");
_LIT(KImageTest16BppColorMbm,"mbm16bpp_1.mbm");
_LIT(KImageTest24BppColorMbm,"mbm24bpp_4.mbm");
_LIT(KImageTest32BppColorMbm,"mbm32bpp_1.mbm");

_LIT(KImageTestJfifMono,"jfifMono_2.jpg");
_LIT(KImageTestJfif420,"jfif420_2.jpg");
_LIT(KImageTestJfif422,"jfif422_4.jpg");
_LIT(KImageTestJfif444,"jfif444_8.jpg");
_LIT(KImageTestJfif10,"jfif10_4.jpg");
_LIT(KImageTestJfif30,"jfif30_1.jpg");
_LIT(KImageTestJfif60,"jfif60_1.jpg");
_LIT(KImageTestJfif100,"jfif100_8.jpg");

_LIT(KImageTest1BppMonoT0Png, "png1bpp_g0.png");
_LIT(KImageTest1BppMonoT3Png, "png1bpp_g3.png");
_LIT(KImageTest2BppGreyT0Png, "png2bpp_g0.png");
_LIT(KImageTest2BppGreyT3Png, "png2bpp_g3.png");
_LIT(KImageTest4BppGreyT0Png, "png4bpp_g0.png");
_LIT(KImageTest4BppGreyT3Png, "png4bpp_g3.png");
_LIT(KImageTest4BppColorT3Png, "png4bpp_c3.png");
_LIT(KImageTest8BppGreyT0Png, "png8bpp_g0.png");
_LIT(KImageTest8BppColorT2Png, "pngs8bpp_c2.png");
_LIT(KImageTest8BppColorT3Png, "png8bpp_c3.png");

_LIT(KImageTest1BppBmp,"bmp1bpp_1.bmp");
_LIT(KImageTest4BppBmp,"bmp4bpp_4.bmp");
_LIT(KImageTest8BppBmp,"bmp8bpp_8.bmp");
_LIT(KImageTest24BppBmp,"bmp24bpp_2.bmp");

//Fail image
_LIT(KImageFailTest1BppMonoMbm,"1 bpp mbm");
_LIT(KImageFailTest2BppMonoMbm,"2 bpp mbm");
_LIT(KImageFailTest4BppMonoMbm,"4 bpp mono mbm");
_LIT(KImageFailTest8BppMonoMbm,"8 bpp mono mbm");
_LIT(KImageFailTest4BppColorMbm,"4 bpp color mbm");
_LIT(KImageFailTest8BppColorMbm,"8 bpp color mbm");
_LIT(KImageFailTest12BppColorMbm,"12 bpp mbm");
_LIT(KImageFailTest16BppColorMbm,"16 bpp mbm");
_LIT(KImageFailTest24BppColorMbm,"24 bpp mbm");

_LIT(KImageFailTestMono10Jfif,"mono jpg 10");
_LIT(KImageFailTestMono30Jfif,"mono jpg 30");
_LIT(KImageFailTestMono60Jfif,"mono jpg60 ");
_LIT(KImageFailTestMono100Jfif,"mono jpg 100");
_LIT(KImageFailTest420C10Jfif,"420 jpg 10");
_LIT(KImageFailTest420C30Jfif,"420 jpg 30");
_LIT(KImageFailTest420C60Jfif,"420 jpg 60");
_LIT(KImageFailTest420C100Jfif,"420 jpg 100");
_LIT(KImageFailTest422C10Jfif,"422 jpg 10");
_LIT(KImageFailTest422C30Jfif,"422 jpg 30");
_LIT(KImageFailTest422C60Jfif,"422 jpg 60");
_LIT(KImageFailTest422C100Jfif,"422 jpg 100");
_LIT(KImageFailTest444C10Jfif,"444 jpg 10");
_LIT(KImageFailTest444C30Jfif,"444 jpg 30");
_LIT(KImageFailTest444C60Jfif,"444 jpg 60");
_LIT(KImageFailTest444C100Jfif,"444 jpg 100");

_LIT(KImageFailTest1BppBmp,"1 bpp bmp");
_LIT(KImageFailTest4BppBmp,"4 bpp bmp");
_LIT(KImageFailTest8BppBmp,"8 bpp bmp");
_LIT(KImageFailTest24BppBmp,"24 bpp bmp");

_LIT(KImageTestWmf64,"wmf6_4.wmf");

const TInt KTestFileNameLength=64;
//Assume 12 twips/pixel display for WMF to translate twips to pixels.
//This allow to use the same reference bitmaps for all platforms.
const TInt KWmfPixelSize=12; // Twips/Pixel

// Used as a size to create temporary buffers
const TInt KTempBufferSize=1024;

const TUid KImageTypeTestUid = {0x101F45D8};
const TInt KChunkDataOffset = 10;

TImage::TImage():
	RTestExtra(_L("TImage Test")),
	iActiveListener(NULL),
	iScaledIndex(0),
	iUseThread(EFalse)
	{
	iDecoderOptions = CImageDecoder::EOptionNone;
	}

void TImage::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iFs.SetSessionPath(iDefaultPath);
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
	}

void TImage::InitSystemPath()
	{
	_LIT(KVideoTestSourcePathOnE,"e:\\TImage\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\TImage\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\TImage\\");
	_LIT(KVideoTestSourcePathOnZ,"z:\\TImage\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnD;
		}
	else if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnE;
		}
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnZ;
		}
	else
		{
		User::Panic(_L("Test files not found"),KErrNotFound);
		}

	fs.Close();
	}
	
void TImage::InitSystemPath4()
	{
	_LIT(KVideoTestSourcePathOnE,"e:\\TImage\\00d4\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\TImage\\00d4\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\TImage\\00d4\\");
	_LIT(KVideoTestSourcePathOnZ,"z:\\TImage\\00d4\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnD;
		}
	else if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnE;
		}
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnZ;
		}
	else
		{
		User::Panic(_L("Test files not found"),KErrNotFound);
		}

	fs.Close();
	}
	
void TImage::InitSystemPath6()
	{
	_LIT(KVideoTestSourcePathOnE,"e:\\TImage\\00d6\\");
	_LIT(KVideoTestSourcePathOnD,"d:\\TImage\\00d6\\");
	_LIT(KVideoTestSourcePathOnC,"c:\\TImage\\00d6\\");
	_LIT(KVideoTestSourcePathOnZ,"z:\\TImage\\00d6\\");

	TUint att;
	RFs fs;
	TInt ret = fs.Connect();
	__ASSERT_ALWAYS(ret == KErrNone,User::Panic(_L("Sys path not setup"),ret));

	if (fs.Att(KVideoTestSourcePathOnC,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnC;
		}
	else if (fs.Att(KVideoTestSourcePathOnD,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnD;
		}
	else if (fs.Att(KVideoTestSourcePathOnE,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnE;
		}
	else if (fs.Att(KVideoTestSourcePathOnZ,att) == KErrNone)
		{
		iDefaultPath = KVideoTestSourcePathOnZ;
		}
	else
		{
		User::Panic(_L("Test files not found"),KErrNotFound);
		}

	fs.Close();
	}
	

void TImage::Image1L(TImageFormat aFormat, const TDesC& /*aTestFile*/)
	{
	Printf(_L("--- Image1 test(%d) ---"), aFormat);
	Printf(_L("\n"));

	SetTestFileNameL(iSourceFileName,KImageTestSourceFileName);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Load(iSourceFileName));

	for (TInt formatIndex = 0; ; formatIndex++)
		{
		if (RefFileName(formatIndex, aFormat) == KNullDesC)
			{
			break;
			}

		TUid imageType;
		imageType.iUid = 0;

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);
		ConfigImageDataAndTypeL(*frameImageData, imageType, formatIndex, aFormat);
		TRAPD(err, EncodeBitmapToFileL(KImageTestTempFileName,bitmap, frameImageData, imageType));
		CleanupStack::PopAndDestroy(frameImageData);;

		if (err==KErrNone)
			{
			TFileName refFileName = iDefaultPath;
			refFileName.Append(RefFileName(formatIndex, aFormat) );
			
			TRAP(err,CheckFileL(KImageTestTempFileName,refFileName));
			}

		if(err!=KErrNone)
			TestFailed(err);
		iFs.Delete(KImageTestTempFileName);
		}
	CleanupStack::PopAndDestroy(bitmap); 
	}

void TImage::Image2L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image2 test(%d) --- %S"), aFormat, &aTestFile);

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString, 0, 0, dir));
	CleanupStack::PushL(dir);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(TSize(0, 0), EColor4K));

	const TInt entries = dir->Count();

	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);
		
		// expect a filename along the lines of abcd_x.ext when x is 1, 2, 4 or 8 - the zoom factor to use
		TParsePtrC parse(fileName);
		TPtrC name = parse.Name();
		TInt zoomValue;
		TPtrC16 zoomfactor = name.Right(2);
		if (zoomfactor.Compare(_L("_8")) == 0)
			{
			zoomValue = 8;
			}
		else if (zoomfactor.Compare(_L("_4")) == 0)
			{
			zoomValue = 4;
			}
		else if (zoomfactor.Compare(_L("_2")) == 0)
			{
			zoomValue = 2;
			}
		else
			{
			zoomValue = 1;
			}

		Printf(_L(" - Opening file %S with zoom factor %d"), &fileName, zoomValue);

		TInt err;
		TRAP(err, DecodeFileToBitmapL(fileName, *bitmap, zoomValue, ImageType(fileName)));

		if (err)
			{
			TestFailed(err);
			continue;
			}

		if (iGetKey == EKeyF12)
			{
			TParse parse;
			User::LeaveIfError(parse.SetNoWild(fileName,NULL,NULL));
			TFileName refFileName = parse.Drive();
			refFileName.Append(KImageTestRefDir);
			refFileName.Append(parse.Name());
			refFileName.Append(KImageTestRefExt);
			bitmap->Save(refFileName);
			}

		CheckBitmapL(fileName, *bitmap);
		}

	CleanupStack::PopAndDestroy(2); // bitmap,dir
	}

void TImage::Image3L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image3 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	CFbsBitmap bmp;
	User::LeaveIfError(bmp.Create(TSize(0,0),EColor16M));
	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	const TInt entries = dir->Count();

	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];
		Printf(entry.iName);

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		// expect a filename along the lines of abcd_x.ext when x is 1, 2, 4 or 8 - the zoom factor to use
		TParsePtrC parse (fileName);
		TPtrC name = parse.Name();
		TInt zoomValue;
		TPtrC16 zoomfactor = name.Right(2);
		if(zoomfactor.Compare(_L("_8"))==0)
			{
			zoomValue = 8;
			}
		else if(zoomfactor.Compare(_L("_4"))==0)
			{
			zoomValue = 4;
			}
		else if(zoomfactor.Compare(_L("_2"))==0)
			{
			zoomValue = 2;
			}
		else
			{
			zoomValue = 1;
			}

		Printf(_L("\n"));

		TInt err;
		TUid imageType = ImageType(fileName);

		TRAP(err,DecodeFileToBitmapL(fileName,src,zoomValue,imageType));
		if(err)
			{
			TestFailed(err);
			continue;
			}
		RFile file;
		User::LeaveIfError(file.Open(iFs,fileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer->Des());
		User::LeaveIfError(file.Read(bufferDes));

		TRAP(err,DecodeDescToBitmapL(*buffer,bmp,zoomValue,imageType));
		if(err)
			{
			TestFailed(err);
			}
		else
			CheckBitmaps(src,bmp);
		CleanupStack::PopAndDestroy(2);//file,buffer
		}
	CleanupStack::PopAndDestroy(dir); 
	}

void TImage::Image4L(TImageFormat aFormat, const TDesC& /*aTestFile*/)
	{
	Printf(_L("--- Image4 test(%d) ---"), aFormat);
	Printf(_L("\n"));

	SetTestFileNameL(iSourceFileName, KImageTestSourceFileName);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Load(iSourceFileName));

	for (TInt formatIndex = 0; ;formatIndex++)
		{
		TUid imageType;
		imageType.iUid = 0;

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);

		const TDesC& configType = ConfigImageDataAndTypeL(*frameImageData, imageType, formatIndex, aFormat);
		if (configType==KNullDesC)
			{
			CleanupStack::PopAndDestroy(frameImageData); 
			break;
			}

		Printf(configType);
		Printf(_L("\n"));
		TInt err;
		TRAP(err,EncodeBitmapToFileL(KImageTestTempFileName,bitmap, frameImageData, imageType));
		if(err)
			{
			CleanupStack::PopAndDestroy(frameImageData); 
			TestFailed(err);
			continue;
			}
		RFile file;
		User::LeaveIfError(file.Open(iFs,KImageTestTempFileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer1 = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer1->Des());
		User::LeaveIfError(file.Read(bufferDes));

		HBufC8* buffer2 = NULL;
		TRAP(err, EncodeBitmapToDescL(buffer2, bitmap, frameImageData, imageType));
		if(err!=KErrNone)
			{
			if (buffer2)
				TestFailed(_L("Error (%d) - even worse, buffer was set"), err);
			else
				TestFailed(err);
			}
		else
			{
			if(buffer1->Compare(*buffer2)!=0) // ie. if buffer1 and buffer2 are different
				TestFailed(KErrCorrupt);
			}
		delete buffer2;

		iFs.Delete(KImageTestTempFileName);
		CleanupStack::PopAndDestroy(3); // file, buffer1, frameImageData
		}
	CleanupStack::PopAndDestroy(bitmap); 
	}

void TImage::Image5L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image5 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	DoImage5_10L(aFormat, aTestFile, 1); // /1 decode
	}

void TImage::Image10L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image10 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	DoImage5_10L(aFormat, aTestFile, 8); // /8 decode
	}

void TImage::DoImage5_10L(TImageFormat aFormat, const TDesC& aTestFile, TInt aReductionFactor)
	{
	Printf(_L("--- Image5_10 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));

	TInt entries = dir->Count();
	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];

		// Performance Gain: Skip WMF6_4.WMF. This file contains 985
		// polygons!!. We can skip it since all wmf primitives in this file are
		// already tested in the remaining wmf test images.
		if (entry.iName.CompareF(KImageTestWmf64) == 0)
			{
			continue;
			}

		TBuf<80> text;
		text.Zero();
		text.Append(_L("Load with alloc fail - "));
		text.Append(entry.iName);
		Printf(text);
		Printf(_L("\n"));

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);
		
		TInt err;
		TUid imageType = ImageType(fileName);

		TRAP(err,DecodeFileToBitmapL(fileName,src,aReductionFactor,imageType));
		if(err)
			{
			TestFailed(err);
			continue;
			}
		DecodeImageWithAllocFailureL(fileName,aReductionFactor,src,imageType);
		}
	CleanupStack::PopAndDestroy(1); // dir
	}

void TImage::Image6L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image6 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap bmp;
	User::LeaveIfError(bmp.Create(TSize(0,0),EColor16M));
	CFbsBitmap bmp2;
	User::LeaveIfError(bmp2.Create(TSize(0,0),EColor16M));
	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));

	TInt entries = dir->Count();
	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];

		TBuf<64> text;
		text.Zero();
		text.Append(_L("Stream Decode - "));
		text.Append(entry.iName);
		Printf(text);
		Printf(_L("\n"));

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		TUid imageType = ImageType(fileName);

		StreamDecodeFileToBitmapL(fileName,bmp,0,1,imageType);
		StreamDecodeFileToBitmapViaDesL(fileName,bmp2,0,1,imageType);

		RFile file;
		User::LeaveIfError(file.Open(iFs,fileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer->Des());
		User::LeaveIfError(file.Read(bufferDes));
		TInt err;	
		Printf(_L("\n"));
		TRAP(err,DecodeDescToBitmapL(bufferDes,src,1,imageType));
		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2); // buffer and file
			continue;
			}

		CheckBitmaps(src,bmp);
		CheckBitmaps(src,bmp2);

		//Test with zoom factor 8
		StreamDecodeFileToBitmapL(fileName,bmp,0,8,imageType);
		StreamDecodeFileToBitmapViaDesL(fileName,bmp2,0,8,imageType);
		TRAP(err,DecodeDescToBitmapL(bufferDes,src,8,imageType));
		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2); // buffer and file
			continue;
			}
		
		CheckBitmaps(src,bmp);
		CheckBitmaps(src,bmp2);

		CleanupStack::PopAndDestroy(2); // buffer and file
		}

	CleanupStack::PopAndDestroy(dir);
	}

void TImage::Image6BL(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image6b test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));

	TInt entries = dir->Count();
	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];

		TBuf<64> text;
		text.Zero();
		text.Append(_L("Stream with alloc fail - "));
		text.Append(entry.iName);
		Printf(text);
		Printf(_L("\n"));

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		RFile file;
		User::LeaveIfError(file.Open(iFs,fileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer->Des());
		User::LeaveIfError(file.Read(bufferDes));

		TUid imageType = ImageType(fileName);

		TRAPD(err,DecodeDescToBitmapL(bufferDes,src,1,imageType));
		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2); // buffer and file
			continue;
			}

		StreamImageWithAllocFailureL(fileName,0,1, src, imageType);
		StreamImageViaDesWithAllocFailureL(fileName,0,1, src, imageType);

		TRAP(err,DecodeDescToBitmapL(bufferDes,src,8,imageType));
		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2); // buffer and file
			continue;
			}

		StreamImageWithAllocFailureL(fileName,0,8, src, imageType);
		StreamImageViaDesWithAllocFailureL(fileName,0,8, src, imageType);

		CleanupStack::PopAndDestroy(2); // buffer and file
		}

	CleanupStack::PopAndDestroy(dir); 
	}

void TImage::Image7L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image7 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	SetTestFileNameL(iSourceFileName,KImageTestSourceFileName);

	DoEncoderAllocTest(aFormat,aTestFile,EFalse);
	}

void TImage::DoEncoderAllocTest(TImageFormat aFormat, const TDesC& aTestFile, TBool aTestExtensions)
	{
	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Load(iSourceFileName));

	for (TInt formatIndex =  0; ; formatIndex++)
		{
		if (RefFileName(formatIndex, aFormat) == KNullDesC)
			{
			break;
			}

		TUid imageType;
		imageType.iUid = 0;

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);

		ConfigImageDataAndTypeL(*frameImageData, imageType, formatIndex, aFormat);

		TBuf<64> text(_L("Save with alloc fail - "));
		text.Append(SaveFileName(formatIndex, aFormat));
		Printf(text);

		TFileName refFileName = iDefaultPath;
		refFileName.Append(RefFileName(formatIndex, aFormat) );

		if ( aTestExtensions )
			{
			TUniqueTransform transformOption = ENormal;
			for ( TInt i=0; i<=7; i++)
				{
				TBool prepare = EFalse;
				switch ( i )
					{
					case 0:
						transformOption = ERotate90;
						break;
					case 1:
						transformOption = ERotate180;
						break;
					case 2:
						transformOption = ERotate270;
						break;
					case 3:
						transformOption = EHorizontalFlip;
						break;
					case 4:
						transformOption = EHorizontalFlipRotate90;
						break;
					case 5:
						transformOption = EVerticalFlip;
						break;
					case 6:
						transformOption = EVerticalFlipRotate90;
						break;
					case 7:
						transformOption = ERotate90;
						prepare = ETrue;
						break;
					default:
						ASSERT(0);
						break;
					}
				TFileName outputName;
				outputName = KImageTestTempFileName;
#ifdef GENERATE_EXTENSION_OUTPUT
// generate unique o/p from tests
				outputName.Format(_L("%S_%d_%08x_%d.jpg"),&KImageTestTempFileName,formatIndex,transformOption,(aFormat == EEXIF));
#endif					
				EncodeImageWithAllocFailureL(outputName, srcBmp, frameImageData, imageType, refFileName, transformOption, prepare, aFormat);
				}
			}
		else
			{
			EncodeImageWithAllocFailureL(KImageTestTempFileName, srcBmp, frameImageData, imageType, refFileName);
			}

		iFs.Delete(KImageTestTempFileName);
		CleanupStack::PopAndDestroy(frameImageData); 
		}

	CleanupStack::PopAndDestroy(2); // dir, srcBmp
	}
	
void TImage::Image8L(TImageFormat aFormat, const TDesC& /*aTestFile*/)
	{
	Printf(_L("--- Image8 test(%d) ---"), aFormat);
	Printf(_L("\n"));

	Printf(_L("OTA/WBMP test\n"));

	CFbsBitmap* source = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(source);

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);

	for (TInt index=0; index<2; index++)
		{
		TFileName testFileName;
		TFileName testSourceFileName;
		TUid testUid = KNullUid;
		TInt error = KErrNone;

		switch (index)
			{
			case 0:
				if (!(aFormat==EANY || aFormat==EOTA))
					continue;
				SetTestFileNameL(testFileName, KImageTestOtaFileName);
				SetTestFileNameL(testSourceFileName, KImageTestOtaSourceFileName);
				testUid = KImageTypeOTAUid;
				break;
			case 1:
				if (!(aFormat==EANY || aFormat==EWBP))
					continue;
				SetTestFileNameL(testFileName, KImageTestWbmpFileName);
				SetTestFileNameL(testSourceFileName, KImageTestWbmpSourceFileName);
				testUid = KImageTypeWBMPUid;
				break;
			default:
				ASSERT(EFalse);
			}

		User::LeaveIfError(source->Load(testSourceFileName));
		User::LeaveIfError(bmp->Create(source->SizeInPixels(),EGray2));

		TParsePtrC parse (testFileName);
		TPtrC nameAndExt (parse.NameAndExt());
		Printf(_L("%S\n"), &nameAndExt);

		CImageDecoder* decoder = NULL;
		TRAP(error, decoder = CImageDecoder::FileNewL(iFs, testFileName, DecoderOptions(), testUid));
		if (error==KErrNone)
			{
			iActiveListener->InitialiseActiveListener();
			decoder->Convert(&iActiveListener->iStatus,*bmp);
			CActiveScheduler::Start();
			delete decoder;
			error = iActiveListener->iStatus.Int();
			}

		if (error==KErrNone)
			CheckBitmaps(*source,*bmp);
		else
			TestFailed(error);
		}

	CleanupStack::PopAndDestroy(2); // bmp, source	
	}

void TImage::Image9L(TImageFormat aFormat, const TDesC& /*aTestFile*/)
	{
	Printf(_L("--- Image9 test(%d) ---"), aFormat);
	Printf(_L("\n"));

	Printf(_L("Mask testing\n"));

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	CFbsBitmap* mask = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(mask);

	TFileName maskFileName;
	SetTestFileNameL(maskFileName, KImageTestMaskFileName);

	TParsePtrC parse (maskFileName);
	TPtrC nameAndExt (parse.NameAndExt());
	Printf(_L("%S\n"), &nameAndExt);

	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, maskFileName, DecoderOptions(), KImageTypeGIFUid);
	CleanupStack::PushL(decoder);

	TFrameInfo frameInfo(decoder->FrameInfo());
	User::LeaveIfError(bmp->Create(frameInfo.iFrameCoordsInPixels.Size(),EGray2));
	User::LeaveIfError(mask->Create(frameInfo.iFrameCoordsInPixels.Size(),EGray2));

	iActiveListener->InitialiseActiveListener();
	decoder->Convert(&iActiveListener->iStatus,*bmp,*mask);
	CActiveScheduler::Start();

	CheckBitmaps(*bmp,*mask);

	CleanupStack::PopAndDestroy(3); // decoder,bmp,mask
	}


void TImage::Image12L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image12 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	Printf(_L("File ext and MIME test\n"));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));

	RFileExtensionMIMETypeArray imageMIMEArray;
	CImageDecoder::GetFileTypesL(imageMIMEArray);
	TInt entries = dir->Count();

	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];

		TBuf<80> text;
		text.Zero();
		text.Append(_L("Load with alloc fail - "));
		text.Append(entry.iName);
		Printf(text);
		Printf(_L("\n"));

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);
		
		TParse parse;
		parse.Set(fileName, NULL, NULL);
		TPtrC ext = parse.Ext();

		const TDesC8* mimeType = NULL;
		for (TInt index = 0; index < imageMIMEArray.Count(); index++)
			{
			CFileExtensionMIMEType& fileExtAndMIMEType = *imageMIMEArray[index];
			if (ext.CompareF(fileExtAndMIMEType.FileExtension()) == 0)
				{
				mimeType = &fileExtAndMIMEType.MIMEType();
				break;
				}
			}

		if (mimeType == NULL)
			{
			TestFailed(KErrNotFound);
			continue;
			}

		TInt err;
		TRAP(err,DecodeFileToBitmapL(fileName,src,1,*mimeType));
		if(err)
			{
			TestFailed(err);
			continue;
			}

		DecodeImageWithAllocFailureL(fileName,1,src,*mimeType);

		TRAP(err,DecodeFileToBitmapL(fileName,src,8,*mimeType));
		if(err)
			{
			TestFailed(err);
			continue;
			}
		DecodeImageWithAllocFailureL(fileName,8,src,*mimeType);
		}
	CleanupStack::PopAndDestroy(1);//dir
	imageMIMEArray.ResetAndDestroy();
	}


void TImage::Image13L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image13 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));
	
	TInt entries = dir->Count();
	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];
		
		// image for test case MM-ICL-COD-U-00d4
		if (entry.iName.Compare(_L("jfifProg_4.jpg")) == 0)
			continue;
		
		// image for test case MM-ICL-COD-U-00d5
		if (entry.iName.Compare(_L("imagevid_8.mbm")) == 0)
			continue; 
		
		TBuf<64> text;
		text.Zero();
		
		text.Append(_L("Stream with alloc fail - "));
		text.Append(entry.iName);
		
		Printf(text);
		Printf(_L("\n"));
        
		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		RFile file;
		User::LeaveIfError(file.Open(iFs,fileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer->Des());
		User::LeaveIfError(file.Read(bufferDes));

		TUid imageType = ImageType(fileName);

		TRAPD(err,DecodeDescToBitmapL(bufferDes,src,1,imageType));
		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2); // buffer and file
			continue;
			}

		TBool includeFileNewLInTest = EFalse;
		if (0 == count ||
 			KImageTypePNGUid == imageType ||
 			KImageTypeJPGUid == imageType)
			{
			// Performance Gain: CImageDecoder::FileNewL api call is only
			// included in the alloc fail count for the first image. JPG and
			// PNG image formats will always include FileNewL in the test for
			// all images.
			includeFileNewLInTest = ETrue;
			}

		StreamImageWithAllocFailureV2L(fileName,0,1, src, imageType, includeFileNewLInTest);

		CleanupStack::PopAndDestroy(2); // buffer and file
		
		}// end of for loop

	CleanupStack::PopAndDestroy(dir); 
	
	}

void TImage::Image14L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image14 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor16M));

	TInt entries = dir->Count();
	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];

		TBuf<64> text;
		text.Zero();
		text.Append(_L("Stream with alloc fail - "));
		text.Append(entry.iName);
		Printf(text);
		Printf(_L("\n"));

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		RFile file;
		User::LeaveIfError(file.Open(iFs,fileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer->Des());
		User::LeaveIfError(file.Read(bufferDes));

		TUid imageType = ImageType(fileName);

		TRAPD(err,DecodeDescToBitmapL(bufferDes,src,1,imageType));
		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2); // buffer and file
			continue;
			}

		StreamImageViaDesWithAllocFailureL(fileName,0,1, src, imageType);

		CleanupStack::PopAndDestroy(2); // buffer and file
		}

	CleanupStack::PopAndDestroy(dir); 
	}

//const TInt KProfNormalCount = 100; // no of runs when not waiting
//const TInt KProfWaitingCount = 10; // no of runs when we don't wait
//const TInt KProfWaitTime = 5000000; // 5s

void TImage::Prof1L(TBool aWaitForUnload)
	{
#if defined(__ICL_PROFILING)
	Printf(_L("--- Profiling %d ---"), aWaitForUnload);
	Printf(_L("\n"));

	REComSession& ecomSession = REComSession::OpenL();
	CleanupClosePushL(ecomSession);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(KNullDesC,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(TSize(0,0),EColor4K));

	const TInt entries = dir->Count();
	TUid imageType;
	imageType.iUid = 0;

	for (TInt count = 0; count < entries; count++)
		{
		const TEntry& entry = (*dir)[count];
		Printf(entry.iName);

		// Debug points as follows:
		// 0 - around whole DecodeFileToBitmapL()
		// 1 - around associated FileNewL()
		// 2 - around associated ListImplementationsL()
		// 3 - around associated CreateImplementationL()
		// 4 - around associated Convert()
		RDebug::ProfileReset(0, 5);

		TInt repeatCount = aWaitForUnload ? 10 : 100;
		TInt err=KErrNone;

		for (TInt count2=0; count2<repeatCount; count2++)
			{
			RDebug::ProfileStart(0);
			TFileName fileName = iDefaultPath;
			fileName.Append(entry.iName);
			
			TRAP(err,DecodeFileToBitmapL(fileName,*bitmap,1,KNullUid));
			RDebug::ProfileEnd(0);

			if (aWaitForUnload)
				User::After(5000000); // wait 5s for ECom to unload

			if (err!=KErrNone) // stop when we see error
				break;
			}

		if(err)
			{
			TestFailed(err);
			continue;
			}
		Printf(_L("\n"));

			{
			TProfile profile[5];
			RDebug::ProfileResult(profile, 0, 5);
			// Replace RDebug::Print with RTest::Printf
			//RDebug::Print(_L("Profile results(%d)"), aWaitForUnload);
			Printf(_L("Profile results(%d)\n"), aWaitForUnload);
			for (TInt index=0; index<5; index++)
				Printf(_L("profile[%d]: time=%d count=%d \n"), index, profile[index].iTime, profile[index].iCount);
				// Replace RDebug::Print with RTest::Printf
				//RDebug::Print(_L("profile[%d]: time=%d count=%d"), index, profile[index].iTime, profile[index].iCount);

			}
		}

	CleanupStack::PopAndDestroy(3); // bitmap,dir and ecomSession
#else
	aWaitForUnload = aWaitForUnload; // old-style "stop unused warning" code
	User::Panic(_L("Prof1() only valid from TProfImag3"), 0);
#endif //defined(__ICL_PROFILING)
	}

void TImage::EncodeImageWithAllocFailureL(const TDesC& aFileName,CFbsBitmap* aBitmap, const CFrameImageData* aFrameImageData, const TUid aImageType, const TDesC& aRefFileName,TUniqueTransform aTransformOption,TBool aPrepare,TImageFormat aFormat)
	{
	TInt err = 0;

	__MM_HEAP_MARK;
	TRAP(err, EncodeBitmapToFileL(aFileName, aBitmap, aFrameImageData, aImageType, aTransformOption,aPrepare,aFormat));
	if (err != KErrNone)
		TestFailed(err);
	__MM_HEAP_MARKEND;

	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse; 
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for(;;)	
		{
		iFs.Delete(aFileName);

		__UHEAP_SETFAIL(RHeap::EFailNext ,failCount);
		__MM_HEAP_MARK;

		TRAP(err, EncodeBitmapToFileL(aFileName, aBitmap, aFrameImageData, aImageType, aTransformOption,aPrepare,aFormat));

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
			// only do this when testing without extensions (cuts down test matrix and ref files)
			if ( aTransformOption == ENormal )
				{
				if (!CompareFileL (aFileName, aRefFileName))
					{
					badResult = ETrue;	
					}
				}
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			completed = ETrue;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone ,0);

		if (completed)
			break; // exit loop

		failCount++;
		}

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			format.Format(_L("  Bad result with %d memory allocations tested"),failCount);
		else
			format.Format(_L("  Error(%d) with %d memory allocations tested"),err,failCount);
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("  Completed OK with %d memory allocations tested"),failCount);
		Printf(format);
		}
	}
	
void TImage::DecodeImageWithAllocFailureL(const TDesC& aFileName,TInt aZoomFactor, const CFbsBitmap& aReference, const TDesC8& aMIMEType)
	{
	TInt err;

	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0), EColor16M));

	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse; 
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for (;;)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TRAP(err,DecodeFileToBitmapL(aFileName,bitmap,aZoomFactor,aMIMEType));

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
			if (!CompareBitmaps (bitmap, aReference))
				badResult = ETrue;
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			completed = ETrue;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			break; // exit loop

		failCount += 1;
		}

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			format.Format(_L("  Bad result at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		else
			format.Format(_L("  Error(%d) at zoom factor %d with %d memory allocations tested\n"),err,aZoomFactor,failCount);
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("  Completed OK at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		Printf(format);
		}
	}

void TImage::DecodeImageWithAllocFailureL(const TDesC& aFileName,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType)
	{
	TInt err;

	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0), EColor16M));

	TRAP(err,DecodeFileToBitmapL(aFileName,bitmap,aZoomFactor,aImageType));
	if (err != KErrNone)
		TestFailed(err);

	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse; 
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for (;;)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TRAP(err,DecodeFileToBitmapL(aFileName,bitmap,aZoomFactor,aImageType));

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
			if (!CompareBitmaps (bitmap, aReference))
				badResult = ETrue;
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			completed = ETrue;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			break; // exit loop

		failCount += 1;
		}

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			format.Format(_L("  Bad result at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		else
			format.Format(_L("  Error(%d) at zoom factor %d with %d memory allocations tested\n"),err,aZoomFactor,failCount);
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("  Completed OK at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		Printf(format);
		}
	}

void TImage::StreamImageWithAllocFailureL(const TDesC& aFileName,TInt aFrameNo,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType)
	{
	TInt err;

	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0), EColor16M));

	TRAP(err,StreamDecodeFileToBitmapL(aFileName,bitmap,aFrameNo,aZoomFactor,aImageType));
	if (err != KErrNone)
		TestFailed(err);
	TTime loopStart;

	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse; 
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	loopStart.HomeTime();
	for (;;)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TRAP(err,StreamDecodeFileToBitmapL(aFileName,bitmap,aFrameNo,aZoomFactor,aImageType));

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
			{
				User::Free(testAlloc);
			}
    	    
    	    TTime compareStartTime;
    	    compareStartTime.HomeTime();
			// see if valid result and break if wrong - might be premature result
			if (!CompareBitmaps (bitmap, aReference))
				badResult = ETrue;
			
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			completed = ETrue;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			break; // exit loop

		failCount += 1;
		} // end of for loop

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			format.Format(_L("  Bad result (Via File) at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		else
			format.Format(_L("  Error(%d) (Via File) at zoom factor %d with %d memory allocations tested\n"),err,aZoomFactor,failCount);
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("  Completed OK (Via File) at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		Printf(format);
		}
	}

// Performance Gain: This is an improved version of
// TImage::StreamImageWithAllocFailureL that uses StreamDecodeFileToBitmapV2L
// instead of StreamDecodeFileToBitmapL
void TImage::StreamImageWithAllocFailureV2L(const TDesC& aFileName,
											TInt aFrameNo,
											TInt aZoomFactor,
											const CFbsBitmap& aReference,
											const TUid aImageType /* = KNullUid */,
											TBool aIncludeFileNewApi /* = ETrue */)
	{
	TInt err = KErrNone;

	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0), EColor16M));

	TRAP(err,StreamDecodeFileToBitmapL(aFileName,bitmap,aFrameNo,aZoomFactor,aImageType));
	if (err != KErrNone)
		{
		TestFailed(err);
		return;
		}

	TTime loopStart;

	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse;
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	loopStart.HomeTime();
	for (;;)
		{

		TRAP(err,StreamDecodeFileToBitmapV2L(aFileName, bitmap, aFrameNo, failCount, aZoomFactor, aImageType, aIncludeFileNewApi));

		if (err == KErrNone)
			{
			reachedEnd = ETrue;
			failCount -= 1;

			TTime compareStartTime;
			compareStartTime.HomeTime();
			// see if valid result and break if wrong - might be premature result
			if (!CompareBitmaps (bitmap, aReference))
				{
				badResult = ETrue;
				}

			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			{
			completed = ETrue;
			}

		if (completed)
			{
			break; // exit loop
			}

		failCount += 1;
		} // end of for loop

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			{
			format.Format(_L("	Bad result (Via File) at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
			}
		else
			{
			format.Format(_L("	Error(%d) (Via File) at zoom factor %d with %d memory allocations tested\n"),err,aZoomFactor,failCount);
			}
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("	Completed OK (Via File) at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		Printf(format);
		}
	}

void TImage::StreamImageViaDesWithAllocFailureL(const TDesC& aFileName,TInt aFrameNo,TInt aZoomFactor, const CFbsBitmap& aReference, const TUid aImageType)
	{
	TInt err;

	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0), EColor16M));

	TRAP(err,StreamDecodeFileToBitmapViaDesL(aFileName,bitmap,aFrameNo,aZoomFactor,aImageType));
	if (err != KErrNone)
		TestFailed(err);

	TInt failCount = 1;
	TBool completed = EFalse;
	TBool badResult = EFalse; 
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for (;;)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TRAP(err,StreamDecodeFileToBitmapViaDesL(aFileName,bitmap,aFrameNo,aZoomFactor,aImageType));

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
			if (!CompareBitmaps (bitmap, aReference))
				badResult = ETrue;
			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code
			completed = ETrue;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			break; // exit loop

		failCount += 1;
		}

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			format.Format(_L("  Bad result (Via Des) at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		else
			format.Format(_L("  Error(%d) (Via Des) at zoom factor %d with %d memory allocations tested\n"),err,aZoomFactor,failCount);
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	else 
		{
		TBuf<80> format;
		format.Format(_L("  Completed OK (Via Des) at zoom factor %d with %d memory allocations tested\n"),aZoomFactor,failCount);
		Printf(format);
		}
	}

void TImage::DecodeDescToBitmapL(const TDesC8& aBufferDes,CFbsBitmap& aBitmap,TInt aZoomFactor,const TUid aImageType)
	{
	CImageDecoder* streamDecoder = CImageDecoder::DataNewL(iFs, aBufferDes, DecoderOptions(), aImageType);
	CleanupStack::PushL(streamDecoder);
	TFrameInfo frameInfo(streamDecoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	//Workaround to use same WMF reference bitmaps on all platforms
	if(aImageType == KImageTypeWMFUid)
		frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;

	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));
	iActiveListener->InitialiseActiveListener();
	streamDecoder->Convert(&iActiveListener->iStatus,aBitmap);
	CActiveScheduler::Start();
	if(iActiveListener->iStatus!=KErrNone)
		User::Leave(iActiveListener->iStatus.Int());

	CleanupStack::PopAndDestroy(streamDecoder); 
	}

void TImage::DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor,const TUid aImageType)
	{
	TInt initAllocSize = 0;
	TInt finalAllocSize = 0;
	CImageDecoder* decoder = NULL;
	
	if(iDecoderUid.iUid != 0)
		{
#if defined(__ICL_PROFILING)
	RDebug::ProfileStart(1);
		TRAPD(error, decoder = CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), aImageType, KNullUid, iDecoderUid));
		RDebug::ProfileEnd(1);
		User::LeaveIfError(error);
		
		// INC083133: We want to be sure that for that progressive image, the memory needed for decoding
		// has been freeed.
		if(aFileName.Compare(_L("c:\\TImage\\jfifProg_4.jpg")) == 0)
			{
			initAllocSize = User::Heap().Size();
			}

#else
		decoder = CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), aImageType, KNullUid, iDecoderUid);
#endif //defined(__ICL_PRO FILING)
		}
	else
		{
#if defined(__ICL_PROFILING)
		RDebug::ProfileStart(1);
	TRAPD(error, decoder = CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), aImageType));
	RDebug::ProfileEnd(1);
	User::LeaveIfError(error);
	
	// INC083133: We want to be sure that for that progressive image, the memory needed for decoding
	// has been freeed.
	if(aFileName.Compare(_L("c:\\TImage\\jfifProg_4.jpg")) == 0)
		{
		initAllocSize = User::Heap().Size();
		}

#else
	decoder = CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), aImageType);
#endif //defined(__ICL_PROFILING)
		}		

	CleanupStack::PushL(decoder);
	TFrameInfo frameInfo(decoder->FrameInfo());

	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	//Workaround to use same WMF reference bitmaps on all platforms
	if(aImageType == KImageTypeWMFUid)
		frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));
	iActiveListener->InitialiseActiveListener();
	decoder->Convert(&iActiveListener->iStatus,aBitmap);
#if defined(__ICL_PROFILING)
	RDebug::ProfileStart(4);
#endif //defined(__ICL_PROFILING)
	CActiveScheduler::Start();
#if defined(__ICL_PROFILING)
	RDebug::ProfileEnd(4);
#endif //defined(__ICL_PROFILING)
	if(iActiveListener->iStatus!=KErrNone)
		User::Leave(iActiveListener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder); 
	
	// INC083133: We know that for this image, it would need 700KB memory, we make sure it hasn't been allocated on this thread's heap
	if(aFileName.Compare(_L("c:\\TImage\\jfifProg_4.jpg")) == 0)
		{
		finalAllocSize = User::Heap().Size();
		if(finalAllocSize - initAllocSize > 700000)
			{
			Printf(_L("More memory than expected has been used for this decoding."));
			User::Leave(KErrTooBig);
			}
		}
	}

void TImage::DecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aZoomFactor,const TDesC8& aMIMEType)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs,aFileName,aMIMEType, DecoderOptions());
	CleanupStack::PushL(decoder);
	TFrameInfo frameInfo(decoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));
	iActiveListener->InitialiseActiveListener();
	decoder->Convert(&iActiveListener->iStatus,aBitmap);
	CActiveScheduler::Start();
	if(iActiveListener->iStatus!=KErrNone)
		User::Leave(iActiveListener->iStatus.Int());
	CleanupStack::PopAndDestroy(decoder); 
	}

const TInt KStreamBreakpoint=100; // up to this point use initialincrement. then use secondary
const TInt KStreamInitialIncrement=1;
const TInt KStreamSecondaryIncrement=128;

inline TInt StreamIncrement(TInt aValue)
	{
	return (aValue<KStreamBreakpoint)?KStreamInitialIncrement:KStreamSecondaryIncrement;
	}

// streaming tests by seeing what happens when the required image is not always there
// the descriptor one works by reading image into a descriptor and then going through sequentially
void TImage::StreamDecodeFileToBitmapViaDesL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aFrameNo,TInt aZoomFactor,const TUid aImageType)
	{
	RFile file;
	User::LeaveIfError(file.Open(iFs, aFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));
	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(fileSize)), 0, fileSize);
	User::LeaveIfError(file.Read(buffer));
	CleanupStack::Pop(); // buffer
	CleanupStack::PopAndDestroy(&file); 
	CleanupStack::PushL(STATIC_CAST(TAny*, CONST_CAST(TUint8*,buffer.Ptr()))); // push buffer back on the stack

	TInt wantedBufferSize=1; // TODO should start at 0
	buffer.SetLength(wantedBufferSize); 

	SetStreamedDecode(ETrue);
	// gradually increment buffer size until we can open the decoder
	CImageDecoder* decoder = NULL;
	for (;;)
		{
		TRAPD(error, decoder = CImageDecoder::DataNewL(iFs, buffer, DecoderOptions(), aImageType));
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			User::Leave(error);
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		buffer.SetLength(wantedBufferSize);
		}
	CleanupStack::PushL(decoder);

	// gradually increment buffer size until we get info about the given frame
	TBool frameIsReady = EFalse;
	while (!frameIsReady && wantedBufferSize<fileSize)
		{
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		buffer.SetLength(wantedBufferSize);
		decoder->ContinueProcessingHeaderL();
		if ((decoder->FrameCount() > aFrameNo) && (decoder->FrameInfo(aFrameNo).CurrentFrameState() == TFrameInfo::EFrameInfoProcessingComplete))
			frameIsReady = ETrue;
		}

	// resize the bitmap - since we now know the size
	TFrameInfo frameInfo(decoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	//Workaround to use same WMF reference bitmaps on all platforms
	if(aImageType == KImageTypeWMFUid)
		frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));

	// the conversion itself
	TBool first = ETrue;
	for(;;)
		{
		iActiveListener->InitialiseActiveListener();
		if (first)
			decoder->Convert(&iActiveListener->iStatus,aBitmap,aFrameNo);
		else
			decoder->ContinueConvert(&iActiveListener->iStatus);
#if !defined(__BYPASS_CONTINUE_CONVERT)
		first = EFalse;
#endif //!defined(__BYPASS_CONTINUE_CONVERT)
		CActiveScheduler::Start();
		TInt error = iActiveListener->iStatus.Int();
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			User::Leave(error);
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		buffer.SetLength(wantedBufferSize);
		}

	CleanupStack::PopAndDestroy(2); //decoder and buffer
	}

// streaming tests by seeing what happens when the required image is not always there
// the file one works by successively copying data from the main to a temp file
void TImage::StreamDecodeFileToBitmapL(const TDesC& aFileName,CFbsBitmap& aBitmap,TInt aFrameNo,TInt aZoomFactor,const TUid aImageType)
	{
	RFile file;
	User::LeaveIfError(file.Open(iFs, aFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));

	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(1024)), 0, 1024);

	RFile tempFile;
	TPtrC tempFileName(KImageTempFileName);
	User::LeaveIfError(tempFile.Replace(iFs, tempFileName, EFileShareAny|EFileStream|EFileWrite));
	User::LeaveIfError(tempFile.SetSize(0));	// Defensive programming. If the file is opened elsewhere RFile::Replace()
												// won't reset the file length.
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

	if (wantedBufferSize>0) // TODO should be able to remove this
		{
		User::LeaveIfError(file.Read(buffer, wantedBufferSize));
		ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += wantedBufferSize;
		}

	SetStreamedDecode(ETrue);
	// gradually increment buffer size until we can open the decoder
	CImageDecoder* decoder = NULL;
	for (;;)
		{
		TRAPD(error, decoder = CImageDecoder::FileNewL(iFs, tempFileName, DecoderOptions(), aImageType));
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			User::Leave(error);
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

	// check incomplete flag
#if 0 //TO DO
	if ((wantedBufferSize<fileSize)!=(!decoder->IsImageHeaderProcessingComplete()))
		User::Leave(KErrUnknown);
#endif //0

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
			frameIsReady = ETrue;
		}

	// resize the bitmap - since we now know the size
	TFrameInfo frameInfo(decoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	//Workaround to use same WMF reference bitmaps on all platforms
	if(aImageType == KImageTypeWMFUid)
		frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));

	// the conversion itself
	TBool first = ETrue;
	for(;;)
		{
		iActiveListener->InitialiseActiveListener();
		if (first)
			decoder->Convert(&iActiveListener->iStatus,aBitmap,aFrameNo);
		else
			decoder->ContinueConvert(&iActiveListener->iStatus);
#if !defined(__BYPASS_CONTINUE_CONVERT)
		first = EFalse;
#endif //!defined(__BYPASS_CONTINUE_CONVERT)
		CActiveScheduler::Start();
		TInt error = iActiveListener->iStatus.Int();
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			User::Leave(error);
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - tempFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(file.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(tempFile.Write(buffer));
		tempFileLength += extra;
		}

	CleanupStack::PopAndDestroy(4); //decoder, tempFile, buffer and file
	}


// Performance Gain: Improved version TImage::StreamDecodeFileToBitmapL for
// alloc testing that tries to minimize temp file creation/destruction among
// other things.
void TImage::StreamDecodeFileToBitmapV2L(const TDesC& aFileName,
										 CFbsBitmap& aBitmap,
										 TInt aFrameNo,
										 TInt& aFailCount,
										 TInt aZoomFactor,
										 const TUid aImageType,
										 TBool aIncludeFileNewApi /* = ETrue */)
	{

	//
	// STEP 0 : Initialization of files for simulated streaming
	//

	RFile file;
	User::LeaveIfError(file.Open(iFs, aFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));

	HBufC8* pBuff = HBufC8::NewMaxLC(KTempBufferSize);
	TPtr8 buffer(pBuff->Des());

	RFile tempFile;
	TPtrC tempFileName(KImageTempFileName);
	User::LeaveIfError(tempFile.Replace(iFs, tempFileName, EFileShareAny|EFileStream|EFileWrite));
	User::LeaveIfError(tempFile.SetSize(0));	// Defensive programming. If the file is opened elsewhere RFile::Replace()
												// won't reset the file length.
	CleanupClosePushL(tempFile);

	TInt wantedBufferSize=1;
	TInt tempFileLength = 0;

	User::LeaveIfError(file.Read(buffer, wantedBufferSize));
	ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
	User::LeaveIfError(tempFile.Write(buffer));
	tempFileLength += wantedBufferSize;

	SetStreamedDecode(ETrue);

	//
	// STEP 1 : decoder = CImageDecoder::FileNewL
	//

	CImageDecoder* pCImageDecoder = NULL;
	TInt tempFileLengthStepOne = tempFileLength;
	TBool reachedEnd = EFalse;
	do
		{

		if (tempFileLengthStepOne != tempFileLength)
			{
			// Replace the temp file, to the initial state found at this step
			User::LeaveIfError(tempFile.Replace(iFs, tempFileName, EFileShareAny|EFileStream|EFileWrite));
			User::LeaveIfError(tempFile.SetSize(0));	// Defensive programming. If the file is opened elsewhere RFile::Replace()
														// won't reset the file length.
			TInt seekOffset = 0;
			User::LeaveIfError(file.Seek(ESeekStart,seekOffset));
			// Reset tempFileLength
			wantedBufferSize=1;
			tempFileLength = 0;
			TInt tmpWanted = tempFileLengthStepOne;
			while (tmpWanted > KTempBufferSize)
				{
				User::LeaveIfError(file.Read(buffer, KTempBufferSize));
				ASSERT(buffer.Length()==KTempBufferSize); // should always work on emulator
				User::LeaveIfError(tempFile.Write(buffer));
				tempFileLength += KTempBufferSize;
				tmpWanted -= KTempBufferSize;
				}
			User::LeaveIfError(file.Read(buffer, tmpWanted));
			ASSERT(buffer.Length() == tmpWanted); // should always work on emulator
			User::LeaveIfError(tempFile.Write(buffer));
			tempFileLength += tmpWanted;
			ASSERT(tempFileLengthStepOne == tempFileLength);
			}


		if (aIncludeFileNewApi)
			{
			__UHEAP_SETFAIL(RHeap::EFailNext, aFailCount++);
			__MM_HEAP_MARK;
			}

		TRAPD(stepOneError,
		   {
		   // gradually increment buffer size until we can open the decoder
		   for (;;)
			   {
			   TRAPD(error, pCImageDecoder = CImageDecoder::FileNewL(iFs, tempFileName, DecoderOptions(), aImageType));
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
		   }
			); // TRAPD


		if (stepOneError == KErrNone)
			{
			if (aIncludeFileNewApi)
				{
				// when this fails, we passed through all allocs within step
				TAny* pTestAlloc = User::Alloc(1);
				if (NULL == pTestAlloc)
					{
					reachedEnd = ETrue;
					}
				else
					{
					User::Free(pTestAlloc);
					}
				}
			}
		else if (stepOneError != KErrNoMemory) // bad error code
			{
			if (aIncludeFileNewApi)
				{
				__MM_HEAP_MARKEND;
				__UHEAP_SETFAIL(RHeap::ENone, 0);
				}
			User::Leave(stepOneError);
			}

		if (aIncludeFileNewApi)
			{
			delete pCImageDecoder;
			pCImageDecoder = NULL;

			__MM_HEAP_MARKEND;
			__UHEAP_SETFAIL(RHeap::ENone, 0);
			}

		} while (aIncludeFileNewApi && !reachedEnd);


	__MM_HEAP_MARK;


	if (aIncludeFileNewApi)
		{
		pCImageDecoder = CImageDecoder::FileNewL(iFs, tempFileName, DecoderOptions(), aImageType);
		}
	CleanupStack::PushL(pCImageDecoder);


	//
	// STEP 2: decoder->ContinueProcessingHeaderL()
	//
	TInt tempFileLengthStepTwo = tempFileLength;
	reachedEnd = false;
	TInt tempFailCountStepTwo = 0;
	TBool frameIsReady = EFalse;
	do
		{
		if (tempFileLengthStepTwo != tempFileLength)
			{
			// Replace the temp file, to the initial state found at this step
			User::LeaveIfError(tempFile.Replace(iFs, tempFileName, EFileShareAny|EFileStream|EFileWrite));
			User::LeaveIfError(tempFile.SetSize(0));	// Defensive programming. If the file is opened elsewhere RFile::Replace()
														// won't reset the file length.
			TInt seekOffset = 0;
			User::LeaveIfError(file.Seek(ESeekStart,seekOffset));
			// Reset tempFileLength
			wantedBufferSize=1;
			tempFileLength = 0;
			TInt tmpWanted = tempFileLengthStepTwo;
			while (tmpWanted > KTempBufferSize)
				{
				User::LeaveIfError(file.Read(buffer, KTempBufferSize));
				ASSERT(buffer.Length()==KTempBufferSize); // should always work on emulator
				User::LeaveIfError(tempFile.Write(buffer));
				tempFileLength += KTempBufferSize;
				tmpWanted -= KTempBufferSize;
				}
			User::LeaveIfError(file.Read(buffer, tmpWanted));
			ASSERT(buffer.Length() == tmpWanted); // should always work on emulator
			User::LeaveIfError(tempFile.Write(buffer));
			tempFileLength += tmpWanted;
 			ASSERT(tempFileLengthStepTwo == tempFileLength);
			}

		__UHEAP_SETFAIL(RHeap::EFailNext, tempFailCountStepTwo++);

		TRAPD(stepTwoError,
		   {
		   // gradually increment buffer size until we get info about the given frame
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

			   pCImageDecoder->ContinueProcessingHeaderL();
			   if ((pCImageDecoder->FrameCount() > aFrameNo) &&
				   (pCImageDecoder->FrameInfo(aFrameNo).CurrentFrameState() ==
					TFrameInfo::EFrameInfoProcessingComplete))
				   {
				   frameIsReady = ETrue;
				   }
			   } // while
		   }
			); // TRAPD

		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (stepTwoError == KErrNone)
			{
			reachedEnd = ETrue;
			}
		else if (stepTwoError != KErrNoMemory) // bad error code
			{
			User::Leave(stepTwoError);
			}

		} while (!reachedEnd);

	// Update the global failcount here
	aFailCount += tempFailCountStepTwo;

	CleanupStack::PopAndDestroy(1); // pCImageDecoder
	pCImageDecoder = NULL;

	__MM_HEAP_MARKEND;


	//
	// STEP	 3 : decoder->FrameInfo() ;	 decoder->convert()
	//

	__MM_HEAP_MARK;

	pCImageDecoder = CImageDecoder::FileNewL(iFs, tempFileName, DecoderOptions(), aImageType);
	CleanupStack::PushL(pCImageDecoder);


	reachedEnd = false;
	TInt tempFailCountStepThree = 1;
	do
		{

		__UHEAP_SETFAIL(RHeap::EFailNext, tempFailCountStepThree++);

		bool bypassContConv = EFalse;
#if defined(__BYPASS_CONTINUE_CONVERT)
		bypassContConv = ETrue;
#endif //defined(__BYPASS_CONTINUE_CONVERT)

		TRAPD(stepThreeError,
		    {
			// resize the bitmap - since we now know the size
			TFrameInfo frameInfo(pCImageDecoder->FrameInfo());
			TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
			//Workaround to use same WMF reference bitmaps on all platforms
			if(aImageType == KImageTypeWMFUid)
				{
				frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
				}
			frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
			frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
			User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
			User::LeaveIfError(aBitmap.Resize(frameSize));

			// the conversion itself
			TBool first = ETrue;
			for(;;)
				{
				iActiveListener->InitialiseActiveListener();
				if (first)
					{
					pCImageDecoder->Convert(&iActiveListener->iStatus,aBitmap,aFrameNo);
					if (!bypassContConv)
						{
						first = EFalse;
						}
					}
				else
					{
					pCImageDecoder->ContinueConvert(&iActiveListener->iStatus);
					}
				CActiveScheduler::Start();
				TInt error = iActiveListener->iStatus.Int();
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
				}; // for
			}
			);

		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (stepThreeError == KErrNone)
			{
			reachedEnd = ETrue;
			}
		else if (stepThreeError != KErrNoMemory) // bad error code
			{
			User::Leave(stepThreeError);
			}

		} while (!reachedEnd);


	// Update the global failcount here
	aFailCount += tempFailCountStepThree;

	CleanupStack::PopAndDestroy(1); // pCImageDecoder

	__MM_HEAP_MARKEND;

	CleanupStack::PopAndDestroy(3); // tempFile, buffer and file
	}


void TImage::EncodeBitmapToFileL(const TDesC& aFileName,CFbsBitmap* aBitmap, const CFrameImageData* aFrameImageData, const TUid aImageType, TUniqueTransform aTransformOption, TBool aPrepare, TImageFormat aFormat)
	{
	// vary threading for extended encoder
	SetUseThread((aTransformOption != ENormal) && (aTransformOption % 2));
		
	CImageEncoder* encoder = CImageEncoder::FileNewL(iFs, aFileName, EncoderOptions(), aImageType);
	CleanupStack::PushL(encoder);
	
	if ( aFormat == EEXIF )
		{
		// this causes the encoder to create an EXIF file
		// just happen to know this is one!
		CJPEGExifEncoder* exifencoder = static_cast<CJPEGExifEncoder*>(encoder);
	    
	    // Create a MExifMetadata object and initializes to metadata associated with CJPEGExifEncoder  
	    MExifMetadataWriter* metaData = exifencoder->ExifMetadata(); 
	    if (metaData)
	    	{
	    	// Create a TExifWriterUtility object to write the metadata in to the image
	    	TExifWriterUtility exifWriteUtility(metaData);
		
	    	_LIT8(KBuf8ParamWriteVersion,"ExifTest1.0");
	    	
	    	// Change to a leaving version.
	    	HBufC8* buf8ParamWriteVersion = KBuf8ParamWriteVersion().AllocLC();
	    	
	    	// If metaData is NULL, this call will return KErrOverflow.
			TInt err = exifWriteUtility.SetImageDescription(buf8ParamWriteVersion);
	    	CleanupStack::PopAndDestroy(); // buf8ParamWriteVersion.
   	    
	    	User::LeaveIfError(err);
	    	}
 		}

	if ( aPrepare )
		{
		iActiveListener->InitialiseActiveListener();
		encoder->Prepare(&iActiveListener->iStatus);
		CActiveScheduler::Start();
		if( iActiveListener->iStatus!=KErrNone && iActiveListener->iStatus!=KErrNotSupported )
			User::Leave(iActiveListener->iStatus.Int());
		}

	if ( aTransformOption != ENormal )
		{
		// extension test
		TImageConvOperation* operation = encoder->OperationL();
		ApplyOperationTransformL(operation,aTransformOption);
		
		if ( (aTransformOption & 0x11) == 0x11 ) // thumbnail generation on EVerticalFlipRotate90 and ERotate180
			{
			encoder->SetThumbnail(ETrue);
			}
		}
		
	iActiveListener->InitialiseActiveListener();
	encoder->Convert(&iActiveListener->iStatus,*aBitmap, aFrameImageData);
	CActiveScheduler::Start();
	if(iActiveListener->iStatus!=KErrNone)
		User::Leave(iActiveListener->iStatus.Int());
	CleanupStack::PopAndDestroy(encoder); 
	}

void TImage::EncodeBitmapToDescL(HBufC8*& aBufferDes, CFbsBitmap* aBitmap, const CFrameImageData* aFrameImageData, const TUid aImageType)
	{
	CImageEncoder* encoder = CImageEncoder::DataNewL(aBufferDes, EncoderOptions(), aImageType);
	CleanupStack::PushL(encoder);

	iActiveListener->InitialiseActiveListener();
	encoder->Convert(&iActiveListener->iStatus,*aBitmap,aFrameImageData);
	CActiveScheduler::Start();
	if(iActiveListener->iStatus!=KErrNone)
		User::Leave(iActiveListener->iStatus.Int());
	CleanupStack::PopAndDestroy(encoder); 
	}

void TImage::CheckBitmapL(const TDesC& aFileName, CFbsBitmap& aBitmap)
	{
	TParsePtrC parse(aFileName);

#ifndef __WINS__	
	if (parse.Name().CompareF(_L("wmf4_1")) == 0 || 
	    parse.Name().CompareF(_L("wmf7_1")) == 0 )
	    {
	    return;		
	    }
		
	// These metafiles contains text which is dot-pitch dependent
	// Its reference file is designed to work under the emulator
	// and may fail on devices with different dot-pitches, so skip it
#endif

	TFileName refFileName;
	refFileName = parse.Drive();
	refFileName.Append(KImageTestRefDir);
	refFileName.Append(parse.Name());
	refFileName.Append(KImageTestRefExt);

	CFbsBitmap* refBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(refBitmap);
	
	Printf(_L(" - Using reference bitmap: %S"), &refFileName);
	TInt err = refBitmap->Load(refFileName);
	if (err != KErrNone)
		{
		Printf(_L(" - *** Error %d loading reference bitmap"), err);
		if (err == KErrNotFound)
			{
			aBitmap.Save(_L("c:\\newreference.mbm"));
			Printf(_L(" - Saved sample reference image to c:\\newreference.mbm"));
			}
		
		User::Leave(err);
		}

	// Ignoring return value because CheckBitmaps() already signals a failure.
	// It can be used to Save() the bitmaps to see what's going on if
	// a failure happens at some point in the future.
	CheckBitmaps(aBitmap, *refBitmap);

	CleanupStack::PopAndDestroy(refBitmap); 
	}

void TImage::CheckBitmapL(CFbsBitmap& aBitmap)
	{
	TFileName refFileName = iDefaultPath;
	refFileName.Append(KImageTestSourceScaledFileName);
	refFileName.AppendNum(iScaledIndex++);
	refFileName.Append(KImageTestRefExt);

	CFbsBitmap *referenceScaledBitmap = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(referenceScaledBitmap);
	
	Printf(_L(" - Using reference bitmap: %S"), &refFileName);
	TInt err = referenceScaledBitmap->Load(refFileName);
	if (err != KErrNone)
		{
		Printf(_L(" - *** Error %d loading reference bitmap"), err);
		User::Leave(err);
		}

	CheckBitmaps(aBitmap,*referenceScaledBitmap);
	CleanupStack::PopAndDestroy(referenceScaledBitmap); 
	}

TBool TImage::CheckBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2, TInt aQuarterTurns)
	{
	if (!CompareBitmaps(aBitmap1, aBitmap2, aQuarterTurns))
		{
		TImage* mutableThis = MUTABLE_CAST(TImage*, this);
		mutableThis->Printf(_L(" - *** Bitmap comparison failed!"));
		mutableThis->TestFailed(KErrCorrupt);
		return EFalse;
		}
	
	return ETrue;
	}

TBool TImage::CompareBitmaps(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2, TInt aQuarterTurns)
	{
	const TSize size = aBitmap1.SizeInPixels();

	if (aQuarterTurns == 0)
		{
		if (size != aBitmap2.SizeInPixels())
			{
			Printf(_L(" - *** Sizes differ: (%d x %d) vs (%d x %d)"),
					size.iWidth, size.iHeight,
					aBitmap2.SizeInPixels().iWidth,
					aBitmap2.SizeInPixels().iHeight);
			return EFalse;
			}
		
		TInt scanLength = aBitmap1.ScanLineLength(size.iWidth, EColor16M);

		// create two scanline desriptors of correct length
		TUint8* scanBuffer1 = STATIC_CAST(TUint8*, User::Alloc(scanLength));
		__ASSERT_ALWAYS(scanBuffer1!=NULL, User::Panic(_L("scan buffer 1 alloc failed"), KErrNoMemory));
		TPtr8 scanLine1 (scanBuffer1, 0, scanLength);

		TUint8* scanBuffer2 = STATIC_CAST(TUint8*, User::Alloc(scanLength));
		__ASSERT_ALWAYS(scanBuffer2!=NULL, User::Panic(_L("scan buffer 2 alloc failed"), KErrNoMemory));
		TPtr8 scanLine2 (scanBuffer2, 0, scanLength);

		for (TInt y = 0; y < size.iHeight; y++)
			{
			aBitmap1.GetScanLine(scanLine1,TPoint(0, y), size.iWidth, EColor16M);
			aBitmap2.GetScanLine(scanLine2,TPoint(0, y), size.iWidth, EColor16M);

			if (scanLine1.Compare(scanLine2) != 0)
				{
				Printf(_L(" - *** Pixel differences on scanline %d"), y);
				User::Free(scanBuffer1);
				User::Free(scanBuffer2);
				return EFalse;
				}
			}

		User::Free(scanBuffer1);
		User::Free(scanBuffer2);
		} // end if(aQuarterTurns....
	else
		{
		for (TInt y = 0; y < size.iHeight; y++)
			{
			for (TInt x = 0; x < size.iWidth; x++)
				{
				TPoint pt(x,y);
				TRgb color1,color2;
				aBitmap1.GetPixel(color1,pt);
				if (aQuarterTurns == 1)
					aBitmap2.GetPixel(color2,TPoint(size.iHeight - y - 1,x));
				else if (aQuarterTurns == 2)
					aBitmap2.GetPixel(color2,TPoint(size.iWidth - x - 1,size.iHeight - y - 1));
				else if (aQuarterTurns == 3)
					aBitmap2.GetPixel(color2,TPoint(y,size.iWidth - x - 1));

				if (color1 != color2)
					{
					Printf(_L(" - *** Pixel(%d, %d) differs"), x, y);
					return EFalse;
					}
				}
			}
		}
	return ETrue;
	}

void TImage::CheckFileL(const TDesC& aTestFileName,const TDesC& aRefFileName)
	{
	Printf(aRefFileName);
	Printf(_L("\n"));

	if (!CompareFileL(aTestFileName, aRefFileName))
		TestFailed(KErrCorrupt);
	}

TBool TImage::CompareFileL(const TDesC& aTestFileName,const TDesC& aRefFileName) 
	{
	RFile testFile;
	User::LeaveIfError(testFile.Open(iFs,aTestFileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(testFile);

	RFile refFile;
	User::LeaveIfError(refFile.Open(iFs,aRefFileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(refFile);

	const TInt KDefaultSize = 0x1000; // 4096
	HBufC8* testData = HBufC8::NewMaxLC(KDefaultSize);
	HBufC8* refData = HBufC8::NewMaxLC(KDefaultSize);

	TPtr8 testDes(testData->Des());
	TPtr8 refDes(refData->Des());

	TBool result = ETrue;
	TUint count = 0;

	do	{
		User::LeaveIfError(testFile.Read(testDes, KDefaultSize) );
		User::LeaveIfError(refFile.Read(refDes, KDefaultSize) );
		count++;
		if (testData->Des() != refData->Des() )
			{
			result = EFalse;
			break;
			}
		}
	while (testData->Length() == KDefaultSize);
	Printf(_L("count = %d"), count);

	CleanupStack::PopAndDestroy(4); // refData,testData,refFile,testFile

	return result;
	}

const TDesC& TImage::ConfigImageDataAndTypeL(CFrameImageData& aFrameImageData, TUid& aImageType, TInt aFormatIndex, TImageFormat aFormat)
	{
	iStr.SetLength(0);
	TInt index=aFormatIndex;
	TInt count=0;

	// if we are specifying indiv formats, start to count from 0 over their range only
	switch (aFormat)
		{
		default:
			count=0;
			break;
		case EANY:
			count=33;
			break;
		case EMBM:
			count=10;
			break;
		case EBMP:
			index+=10; count=4;
			break;
		case EJPG:
		case EEXIF:
			index+=14; count=8;
			break;
		case EPNG:
			index+=22; count=10;
			break;
		case EGIF:
			index=32; count=1;
			break;
		}

	if (aFormatIndex>=count)
		return KNullDesC;

	if (index >= 0 && index <= 9) 
		{
		TMbmEncodeData* mbmFormat = new(ELeave) TMbmEncodeData;
		CleanupStack::PushL(mbmFormat);

		switch (index)
			{
		case 0:		mbmFormat->iDisplayMode = EGray2;    iStr = _L("MBM EGray2");    break;
		case 1:		mbmFormat->iDisplayMode = EGray4;    iStr = _L("MBM EGray4");	break;
		case 2:		mbmFormat->iDisplayMode = EGray16;   iStr = _L("MBM EGray16");	break;
		case 3:		mbmFormat->iDisplayMode = EGray256;	 iStr = _L("MBM EGray256");	break;
		case 4:		mbmFormat->iDisplayMode = EColor16;	 iStr = _L("MBM EColor16");	break;
		case 5:		mbmFormat->iDisplayMode = EColor256; iStr = _L("MBM EColor256"); break;
		case 6: 	mbmFormat->iDisplayMode = EColor4K;	 iStr = _L("MBM EColor4K");	break;
		case 7: 	mbmFormat->iDisplayMode = EColor64K; iStr = _L("MBM EColor64K");	break;
		case 8: 	mbmFormat->iDisplayMode = EColor16M; iStr = _L("MBM EColor16M");	break;
		case 9: 	mbmFormat->iDisplayMode = EColor16MU; iStr = _L("MBM EColor16MU");	break;
		default:	break;
			}

		aImageType.iUid = KImageTypeMBMUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendFrameData(mbmFormat));

		CleanupStack::Pop(mbmFormat); 
		}
	else if (index >= 10 && index <= 13)
		{
		TBmpImageData* bmpFormat = new(ELeave) TBmpImageData;
		CleanupStack::PushL(bmpFormat);

		switch (index)
			{
		case 10:		bmpFormat->iBitsPerPixel = 1;  iStr = _L("BMP 1bpp");	break;
		case 11:	bmpFormat->iBitsPerPixel = 4;  iStr = _L("BMP 4bpp");	break;
		case 12:	bmpFormat->iBitsPerPixel = 8;  iStr = _L("BMP 8bpp");	break;
		case 13:	bmpFormat->iBitsPerPixel = 24; iStr = _L("BMP 24bpp");	break;
		default:	break;
			}

		aImageType.iUid = KImageTypeBMPUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendImageData(bmpFormat));

		CleanupStack::Pop(bmpFormat); 
		}
	else if (index >= 14 && index <= 21)
		{
		TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
		CleanupStack::PushL(jpegFormat);

		jpegFormat->iSampleScheme = TJpegImageData::EColor420;
		jpegFormat->iQualityFactor = 50;
		switch (index)
			{
		case 14:	jpegFormat->iSampleScheme = TJpegImageData::EMonochrome; iStr = _L("JPEG mono qf50");		break;
		case 15:	jpegFormat->iSampleScheme = TJpegImageData::EColor420;	 iStr = _L("JPEG EColor420 qf50");	break;
		case 16:	jpegFormat->iSampleScheme = TJpegImageData::EColor422;	 iStr = _L("JPEG EColor422 qf50");	break;
		case 17:	jpegFormat->iSampleScheme = TJpegImageData::EColor444;	 iStr = _L("JPEG EColor444 qf50");	break;
		case 18:	jpegFormat->iQualityFactor = 10;  iStr = _L("JPEG EColor420 qf10");	break;
		case 19:	jpegFormat->iQualityFactor = 30;  iStr = _L("JPEG EColor420 qf30");	break;
		case 20:	jpegFormat->iQualityFactor = 60;  iStr = _L("JPEG EColor420 qf60");	break;
		case 21:	jpegFormat->iQualityFactor = 100; iStr = _L("JPEG EColor420 qf100");	break;
		default:	return (iStr = _L(""));
			}
		
		aImageType.iUid = KImageTypeJPGUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendImageData(jpegFormat));

		CleanupStack::Pop(jpegFormat); 
		}
	else if (index >= 22 && index <= 31)
		{
		TPngEncodeData* pngFormat = new(ELeave) TPngEncodeData;
		CleanupStack::PushL(pngFormat);

		switch (index)
			{
			case 22:	// 1-bit monochrome, color type 0
				iStr = _L("PNG 1-bit mono, color type 0");
				pngFormat->iBitsPerPixel = 1;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 23:	// 1-bit monochrome, color type 3
				iStr = _L("PNG 1-bit mono, color type 3");
				pngFormat->iBitsPerPixel = 1;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 24:	// 2-bit greyscale, color type 0
				iStr = _L("PNG 2-bit grayscale, color type 0");
				pngFormat->iBitsPerPixel = 2;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 25:	// 2-bit greyscale, color type 3
				iStr = _L("PNG 2-bit grayscale, color type 3");
				pngFormat->iBitsPerPixel = 2;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 26:	// 4-bit greyscale, color type 0
				iStr = _L("PNG 4-bit grayscale, color type 0");
				pngFormat->iBitsPerPixel = 4;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 27:	// 4-bit greyscale, color type 3
				iStr = _L("PNG 4-bit grayscale, color type 3");
				pngFormat->iBitsPerPixel = 4;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 28:	// 4-bit colour, color type 3
				iStr = _L("PNG 4-bit colour, color type 3");
				pngFormat->iBitsPerPixel = 4;
				pngFormat->iColor = ETrue;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 29:	// 8-bit greyscale, color type 0
				iStr = _L("PNG 8-bit grayscale, color type 0");
				pngFormat->iBitsPerPixel = 8;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 30:	// 8-bit colour, color type 2
				iStr = _L("PNG 8-bit color, color type 2");
				pngFormat->iBitsPerPixel = 8;
				pngFormat->iColor = ETrue;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 31:	// 8-bit colour, color type 3
				iStr = _L("PNG 8-bit color, color type 3");
				pngFormat->iBitsPerPixel = 8;
				pngFormat->iColor = ETrue;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;

			default:
				break;
			}

		aImageType.iUid = KImageTypePNGUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendFrameData(pngFormat));

		CleanupStack::Pop(pngFormat);
		}
	else if (index==32)
		{
		// GIF - no options
		aImageType.iUid = KImageTypeGIFUid.iUid;
		iStr = _L("GIF");
		}

	return iStr;
	}

void TImage::ConfigFailImageDataAndTypeL(CFrameImageData& aFrameImageData, TUid& aImageType, TInt aFormatIndex, TImageFormat aFormat)
	{
	TInt index=aFormatIndex;
	TInt count=0;

	// if we are specifying indiv formats, start to count from 0 over their range only
	switch (aFormat)
		{
		default:
			count=0;
			break;
		case EANY:
			count=40;
			break;
		case EMBM:
			count=10;
			break;
		case EBMP:
			index+=10; count=4;
			break;
		case EJPG:
		case EEXIF:
			index+=14; count=16;
			break;
		case EPNG:
			index+=30; count=10;
			break;
		}

	// __ASSERT_ALWAYS macro shuts up EABI warning! #550-D: variable "count" was set but never used

	__ASSERT_ALWAYS(aFormatIndex<count, User::Invariant());


	if (index >= 0 && index <= 9)
		{
		TMbmEncodeData* mbmFormat = new(ELeave) TMbmEncodeData;
		switch (index)
			{
		case 0:		mbmFormat->iDisplayMode = EGray2;		break;
		case 1:		mbmFormat->iDisplayMode = EGray4;		break;
		case 2:		mbmFormat->iDisplayMode = EGray16;		break;
		case 3:		mbmFormat->iDisplayMode = EGray256;		break;
		case 4:		mbmFormat->iDisplayMode = EColor16;		break;
		case 5:		mbmFormat->iDisplayMode = EColor256;	break;
		case 6: 	mbmFormat->iDisplayMode = EColor4K;		break;
		case 7: 	mbmFormat->iDisplayMode = EColor64K;	break;
		case 8: 	mbmFormat->iDisplayMode = EColor16M;	break;
		case 9: 	mbmFormat->iDisplayMode = EColor16MU;	break;
		default:	break;
			}

		aImageType.iUid = KImageTypeMBMUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendFrameData(mbmFormat));
		}
	else if (index >= 10 && index <= 13)
		{
		TBmpImageData* bmpFormat = new(ELeave) TBmpImageData;
		switch (index)
			{
		case 10:		bmpFormat->iBitsPerPixel = 1;	break;
		case 11:	bmpFormat->iBitsPerPixel = 4;	break;
		case 12:	bmpFormat->iBitsPerPixel = 8;	break;
		case 13:	bmpFormat->iBitsPerPixel = 24;	break;
		default:	User::Invariant();
			}

		aImageType.iUid = KImageTypeBMPUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendImageData(bmpFormat));
		}
	else if (index >= 14 && index <= 29)
		{
		TJpegImageData* jpegFormat = new(ELeave) TJpegImageData;
		switch (index)
			{
		case 14:		
			jpegFormat->iSampleScheme = TJpegImageData::EMonochrome;
			break;
		case 15:		
			jpegFormat->iSampleScheme = TJpegImageData::EColor420;
			break;
		case 16:		
			jpegFormat->iSampleScheme = TJpegImageData::EColor422;
			break;
		case 17:		
			jpegFormat->iSampleScheme = TJpegImageData::EColor444;
			break;
		case 18:	case 19:	case 20:	case 21:
			jpegFormat->iSampleScheme = TJpegImageData::EColor420;
			break;
		case 22:	case 23:	case 24:	case 25:
			jpegFormat->iSampleScheme = TJpegImageData::EColor422;
			break;
		case 26:	case 27:	case 28:	case 29:
			jpegFormat->iSampleScheme = TJpegImageData::EColor444;
			break;
		default:	User::Invariant();
			}

		switch (index)
			{
		case 14:	case 15:	case 16:	case 17:
			jpegFormat->iQualityFactor = 50;
			break;
		case 18:	case 22:	case 26:
			jpegFormat->iQualityFactor = 10;
			break;
		case 19:	case 23:	case 27:
			jpegFormat->iQualityFactor = 30;
			break;
		case 20:	case 24:	case 28:
			jpegFormat->iQualityFactor = 60;
			break;
		case 21:	case 25:	case 29:
			jpegFormat->iQualityFactor = 100;
			break;
		default:	User::Invariant();
			}

		aImageType.iUid = KImageTypeJPGUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendImageData(jpegFormat));
		}
	else if (index >= 30 && index <= 39)
		{
		TPngEncodeData* pngFormat = new(ELeave) TPngEncodeData;
		CleanupStack::PushL(pngFormat);

		switch (index)
			{
			case 30:	// 1-bit monochrome, color type 0
				pngFormat->iBitsPerPixel = 1;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 31:	// 1-bit monochrome, color type 3
				pngFormat->iBitsPerPixel = 1;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 32:	// 2-bit greyscale, color type 0
				pngFormat->iBitsPerPixel = 2;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 33:	// 2-bit greyscale, color type 3
				pngFormat->iBitsPerPixel = 2;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 34:	// 4-bit greyscale, color type 0
				pngFormat->iBitsPerPixel = 4;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 35:	// 4-bit greyscale, color type 3
				pngFormat->iBitsPerPixel = 4;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 36:	// 4-bit colour, color type 3
				pngFormat->iBitsPerPixel = 4;
				pngFormat->iColor = ETrue;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 37:	// 8-bit greyscale, color type 0
				pngFormat->iBitsPerPixel = 8;
				pngFormat->iColor = EFalse;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 38:	// 8-bit colour, color type 2
				pngFormat->iBitsPerPixel = 8;
				pngFormat->iColor = ETrue;
				pngFormat->iPaletted = EFalse;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 39:	// 8-bit colour, color type 3
				pngFormat->iBitsPerPixel = 8;
				pngFormat->iColor = ETrue;
				pngFormat->iPaletted = ETrue;
				pngFormat->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			default:
				break;
			}

		aImageType.iUid = KImageTypePNGUid.iUid;
		User::LeaveIfError(aFrameImageData.AppendFrameData(pngFormat));
		CleanupStack::Pop(pngFormat);
		}
	}

const TDesC& TImage::RefFileName(TInt aFormatIndex, TImageFormat aFormat)
	{
	TInt index = aFormatIndex;
	TInt count = 0;

	// if we are specifying indiv formats, start to count from 0 over their range only
	switch (aFormat)
		{
		default:
			count=0;
			break;
		case EANY:
			count=32;
			break;
		case EMBM:
			count=10;
			break;
		case EBMP:
			index+=10; count=4;
			break;
		case EJPG:
		case EEXIF:
			index+=14; count=8;
			break;
		case EPNG:
			index+=22; count=10;
			break;
		}

	if (aFormatIndex>=count)
		return KNullDesC;

	switch (index)
		{
	case 0:		return KImageTest1BppMonoMbm;
	case 1:		return KImageTest2BppMonoMbm;
	case 2:		return KImageTest4BppMonoMbm;
	case 3:		return KImageTest8BppMonoMbm;
	case 4:		return KImageTest4BppColorMbm;
	case 5:		return KImageTest8BppColorMbm;
	case 6: 	return KImageTest12BppColorMbm;
	case 7: 	return KImageTest16BppColorMbm;
	case 8: 	return KImageTest24BppColorMbm;
	case 9: 	return KImageTest32BppColorMbm;

	case 10:	return KImageTest1BppBmp;
	case 11:	return KImageTest4BppBmp;
	case 12:	return KImageTest8BppBmp;
	case 13:	return KImageTest24BppBmp;

	case 14: 	return KImageTestJfifMono;
	case 15:	return KImageTestJfif420;
	case 16:	return KImageTestJfif422;
	case 17:	return KImageTestJfif444;
	case 18:	return KImageTestJfif10;
	case 19:	return KImageTestJfif30;
	case 20:	return KImageTestJfif60;
	case 21:	return KImageTestJfif100;

	case 22:	return KImageTest1BppMonoT0Png;
	case 23:	return KImageTest1BppMonoT3Png;
	case 24:	return KImageTest2BppGreyT0Png;
	case 25:	return KImageTest2BppGreyT3Png;
	case 26:	return KImageTest4BppGreyT0Png;
	case 27:	return KImageTest4BppGreyT3Png;
	case 28:	return KImageTest4BppColorT3Png;
	case 29:	return KImageTest8BppGreyT0Png;
	case 30:	return KImageTest8BppColorT2Png;
	case 31:	return KImageTest8BppColorT3Png;

	default:	return KNullDesC;
		}
	}

const TDesC& TImage::SaveFileName(TInt aFormatIndex, TImageFormat aFormat)
	{
	TInt index = aFormatIndex;
	TInt count = 0;

	// if we are specifying indiv formats, start to count from 0 over their range only
	switch (aFormat)
		{
		default:
			count=0;
			break;
		case EANY:
			count=29;
			break;
		case EMBM:
			count=9;
			break;
		case EBMP:
			index+=9; count=4;
			break;
		case EJPG:
		case EEXIF:
			index+=13; count=16;
			break;
		}

	if (aFormatIndex>=count)
		return KNullDesC;

	switch (index)
		{
	case 0:		return KImageFailTest1BppMonoMbm;
	case 1:		return KImageFailTest2BppMonoMbm;
	case 2:		return KImageFailTest4BppMonoMbm;
	case 3:		return KImageFailTest8BppMonoMbm;
	case 4:		return KImageFailTest4BppColorMbm;
	case 5:		return KImageFailTest8BppColorMbm;
	case 6:		return KImageFailTest12BppColorMbm;
	case 7:		return KImageFailTest16BppColorMbm;
	case 8:		return KImageFailTest24BppColorMbm;

	case 9:		return KImageFailTest1BppBmp;
	case 10:	return KImageFailTest4BppBmp;
	case 11:	return KImageFailTest8BppBmp;
	case 12:	return KImageFailTest24BppBmp;

	case 13:	return KImageFailTestMono10Jfif;
	case 14:	return KImageFailTestMono30Jfif;
	case 15:	return KImageFailTestMono60Jfif;
	case 16:	return KImageFailTestMono100Jfif;
	case 17:	return KImageFailTest420C10Jfif;
	case 18:	return KImageFailTest420C30Jfif;
	case 19:	return KImageFailTest420C60Jfif;
	case 20:	return KImageFailTest420C100Jfif;
	case 21:	return KImageFailTest422C10Jfif;
	case 22:	return KImageFailTest422C30Jfif;
	case 23:	return KImageFailTest422C60Jfif;
	case 24:	return KImageFailTest422C100Jfif;
	case 25:	return KImageFailTest444C10Jfif;
	case 26:	return KImageFailTest444C30Jfif;
	case 27:	return KImageFailTest444C60Jfif;
	case 28:	return KImageFailTest444C100Jfif;
	default:	return KNullDesC;
		}
	}

TInt TImage::SaveBitmapToFile(CFbsBitmap& aBitmap,const TDesC& aFileName)
	{
	TInt err = aBitmap.Save(aFileName);
	return err;
	}

TUid TImage::ImageType(const TDesC& aFileName)
	{
	TUid imageType = KNullUid;
	TParse parse;
	parse.Set(aFileName,0,0);
	if(parse.Ext().CompareF(_L(".BMP"))==0)
		imageType.iUid = KImageTypeBMPUid.iUid;
	else if(parse.Ext().CompareF(_L(".JPG"))==0)
		imageType.iUid = KImageTypeJPGUid.iUid;
	else if(parse.Ext().CompareF(_L(".MBM"))==0)
		imageType.iUid = KImageTypeMBMUid.iUid;
	else if(parse.Ext().CompareF(_L(".PNG"))==0)
		imageType.iUid = KImageTypePNGUid.iUid;
	else if(parse.Ext().CompareF(_L(".TIF"))==0)
		imageType.iUid = KImageTypeTIFFUid.iUid;
	else if(parse.Ext().CompareF(_L(".GIF"))==0)
		imageType.iUid = KImageTypeGIFUid.iUid;
	else if(parse.Ext().CompareF(_L(".WMF"))==0)
		imageType.iUid = KImageTypeWMFUid.iUid;
	else if(parse.Ext().CompareF(_L(".ICO"))==0)
		imageType.iUid = KImageTypeICOUid.iUid;

	return imageType;
	}

const TDesC& TImage::ImageSuffix(TImageFormat aFormat)
	{
	switch (aFormat)
		{
		default:
		case EOTA:
		case EWBP:
		case EANY:
			return KNullDesC;
//			break;		// EABI warning removal
		case EGIF:
			{
			_LIT(KGifSuffix, ".gif");
			return KGifSuffix();
			}
//			break;		// EABI warning removal
		case EBMP:
			{
			_LIT(KBmpSuffix, ".bmp");
			return KBmpSuffix();
			}
//			break;		// EABI warning removal
		case EICO:
			{
			_LIT(KIcoSuffix, ".ico");
			return KIcoSuffix();
			}
//			break;		// EABI warning removal
		case EJPG:
		case EEXIF:
			{
			_LIT(KJpgSuffix, ".jpg");
			return KJpgSuffix();
			}
//			break;		// EABI warning removal
		case EMBM:
			{
			_LIT(KMbmSuffix, ".mbm");
			return KMbmSuffix();
			}
//			break;		// EABI warning removal
		case EPNG:
			{
			_LIT(KPngSuffix, ".png");
			return KPngSuffix();
			}
//			break;		// EABI warning removal
		case ETFF:
			{
			_LIT(KTiffSuffix, ".tif");
			return KTiffSuffix();
			}
//			break;		// EABI warning removal
		case EWMF:
			{
			_LIT(KWmfSuffix, ".Wmf");
			return KWmfSuffix();
			}
//			break;		// EABI warning removal
		}
	}

void TImage::GetSearchString(TDes& aSearchString, TImageFormat aFormat, const TDesC& aTestFile)
	{
	if (aTestFile.Length()>0)
		aSearchString = aTestFile;
	else
		{
		const TDesC& suffix = ImageSuffix(aFormat);
		if (suffix.Length()==0)
			aSearchString.Zero();
		else
			{
			aSearchString = _L("*");
			aSearchString.Append(suffix);
			}
		}
	}


// add the correct disk to the given filename
void TImage::SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const
	{
	TFileName defaultPath = iDefaultPath;
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();
	}


struct TMimeTypeInfo
	{
	const TText8* iMimeType;
	TInt32 iDecoderUid;
	};

const TMimeTypeInfo KMimeTypeInfo[] =
	{

	{_S8("image/bmp"), KBMPDecoderImplementationUidValue},
	{_S8("image/x-bmp"), KBMPDecoderImplementationUidValue},
	{_S8("image/x-MS-bmp"), KBMPDecoderImplementationUidValue},
	{_S8("image/gif"), KGIFDecoderImplementationUidValue},
	{_S8("image/jpeg"), KJPGDecoderImplementation2UidValue},
	{_S8("image/ico"), KICODecoderImplementationUidValue},
	{_S8("image/x-epoc-mbm"), KMBMDecoderImplementationUidValue},
	{_S8("image/x-ota-bitmap"), KOTADecoderImplementationUidValue},
	{_S8("image/vnd.nokia.ota-bitmap"), KOTADecoderImplementationUidValue},
	{_S8("image/png"), KPNGDecoderImplementationUidValue},
	{_S8("image/tiff"), KTIFFDecoderImplementationUidValue},
	{_S8("image/x-wmf"), KWMFDecoderImplementationUidValue},
	{_S8("application/x-msmetafile"), KWMFDecoderImplementationUidValue},
	{_S8("image/vnd.wap.wbmp"), KWBMPDecoderImplementationUidValue},
	};

void TImage::Image15L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image15 test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	Printf(_L("Create decoder by MIME type test\n"));

	TInt err = KErrNone;
	TInt maxCount = sizeof(KMimeTypeInfo) / sizeof(TMimeTypeInfo);
	for (TInt count = 0; err == KErrNone && count < maxCount; count++)
		{
		const TMimeTypeInfo* mimeTypeInfo = &KMimeTypeInfo[count];

    	TBuf8<KMaxDataTypeLength> mimeType;
		TUid implUidExpected;
    
		mimeType = mimeTypeInfo->iMimeType;
		implUidExpected.iUid = mimeTypeInfo->iDecoderUid;


		TBuf16<KMaxDataTypeLength> strMimeType;
		strMimeType.Copy(mimeType);
		Printf(_L("Creating decoder for MIME type \"%S\". expected Uid = %08X"), &strMimeType, implUidExpected);

		CImageDecoder* decoder = NULL;
		TRAP(err, decoder = CImageDecoder::DataNewL(iFs, KNullDesC8(), mimeType, CImageDecoder::EOptionAllowZeroFrameOpen));
		if (err != KErrNone)
			{
			Printf(_L("failed to create correct decoder for MIME type \"%S\", err = %d"), &strMimeType, err);
			TestFailed(err);
			break;
			}

		CleanupStack::PushL(decoder);

		TUid implUidReturned = decoder->ImplementationUid();
		if (implUidReturned != implUidExpected)
			{
			Printf(_L("failed to create correct decoder for MIME type \"%S\". Returned Uid = %8X, expected Uid = %8X"), &strMimeType, implUidReturned, implUidExpected);
			TestFailed(err = KErrGeneral);
			}

		CleanupStack::PopAndDestroy(decoder);
		}
	}

const TUint KMaxToleratedDelay=275000; // allow 275ms for the moment

void TImage::Image16L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("Image16 test"));
	Printf(_L(" - aFormat: %d"), aFormat);
	Printf(_L(" - aTestFile: \"%S\""), &aTestFile);

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(TSize(0,0),EColor4K));

	const TInt entries = dir->Count();
//	TUid imageType;			// EABI warning removal
//	imageType.iUid = 0;		// EABI warning removal

	for (TInt count = 0; count < entries; count++)
		{
#ifdef __EABI__
		//On HW choose four files randomly from the available files.
		if(count >= 4 )
			{
			break;
			}
		const TEntry& entry = (*dir)[(Math::Random() % entries)];
		Printf(_L("This is a SMOKE_TEST on HW testing 4 randomly selected file. This is full test on winscw"));		
#else
		const TEntry& entry = (*dir)[count];
#endif
		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		// expect a filename along the lines of abcd_x.ext when x is 1, 2, 4 or 8 - the zoom factor to use
		TParsePtrC parse (fileName);
		TPtrC name = parse.Name();
		TInt zoomValue;
		TPtrC16 zoomfactor = name.Right(2);
		if(zoomfactor.Compare(_L("_8"))==0)
			{
			zoomValue = 8;
			}
		else if(zoomfactor.Compare(_L("_4"))==0)
			{
			zoomValue = 4;
			}
		else if(zoomfactor.Compare(_L("_2"))==0)
			{
			zoomValue = 2;
			}
		else if(zoomfactor.Compare(_L("_1"))==0)
			{
			zoomValue = 1;
			}
		else
			{
			// Skip the file, filename is not in format abcd_x.ext where x is 1, 2, 4 or 8 - the zoom factor to use.
			continue;
			}

		Printf(_L(" - Opening file %S with zoom factor %d"), &fileName, zoomValue);
		TInt err;
		TUint maxDelay = 0;
		TRAP(err, DecodeFileToBitmapWithCancelL(fileName, *bitmap, zoomValue, ImageType(fileName), maxDelay));

		Printf(_L(" - Measured delay = %d"), maxDelay);

		if (err != KErrNone)
			{
			TestFailed(err);
			continue;
			}

		if (maxDelay > KMaxToleratedDelay)
			{
//Test does not fail under WINS, since other processes
//(like virus scanning etc) have an effect on emulator timing.
#if defined(__WINS__)
			Printf(_L(" - Liveliness problem: measured delay=%d (tolerated=%d)\n"), maxDelay, KMaxToleratedDelay);
#else
            TBool testFailed = ETrue;
            const TUint32 flags = DPTest::ERomPaging | DPTest::ECodePaging | DPTest::EDataPaging;
            // adjust Maximum if we're running DP ROM
	        if (DPTest::Attributes() & flags)
	            {
	            const TInt KNewTolerance = KMaxToleratedDelay * 5;
	            Printf(_L(" - WARNING: Tolerance increased due to DP ROM"),KNewTolerance);
	            if (maxDelay < KNewTolerance)
	                {
	                testFailed = EFalse;
	                }
	            }
	        
	        if (testFailed)    
                {
                TestFailed(_L(" - Liveliness problem: measured delay=%d, (tolerated=%d)\n"), maxDelay, KMaxToleratedDelay);    
                continue;
                }
#endif //defined(__WINS__)
			}

		CheckBitmapL(fileName, *bitmap);
		}

	CleanupStack::PopAndDestroy(2); // bitmap,dir
	}

void TImage::Image16BL(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image16B test(%d) --- %S"), aFormat, &aTestFile);
	Printf(_L("\n"));

	CFbsBitmap dest;
	User::LeaveIfError(dest.Create(TSize(0,0),EColor4K));
	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),EColor4K));

	TBuf<KTestFileNameLength> searchString;
	GetSearchString(searchString, aFormat, aTestFile);

	CDir* dir = NULL;
	User::LeaveIfError(iFs.GetDir(searchString,0,0,dir));
	CleanupStack::PushL(dir);

	const TInt entries = dir->Count();

	for (TInt count = 0; count < entries; count++)
		{
#ifdef __EABI__
		//On HW choose four files randomly from available files.
		if(count >= 4 )
			{
			break;
			}
		const TEntry& entry = (*dir)[(Math::Random() % entries)];
		Printf(_L("This is a SMOKE_TEST on HW testing 4 randomly selected file. This is full test on winscw"));		
#else
		const TEntry& entry = (*dir)[count];
#endif		
		Printf(entry.iName);

		TFileName fileName = iDefaultPath;
		fileName.Append(entry.iName);

		// expect a filename along the lines of abcd_x.ext when x is 1, 2, 4 or 8 - the zoom factor to use
		TParsePtrC parse (fileName);
		TPtrC name = parse.Name();
		TInt zoomValue;
		TPtrC16 zoomfactor = name.Right(2);
		if(zoomfactor.Compare(_L("_8"))==0)
			{
			zoomValue = 8;
			}
		else if(zoomfactor.Compare(_L("_4"))==0)
			{
			zoomValue = 4;
			}
		else if(zoomfactor.Compare(_L("_2"))==0)
			{
			zoomValue = 2;
			}
		else if(zoomfactor.Compare(_L("_1"))==0)
			{
			zoomValue = 1;
			}
		else
			{
			// Skip the file, filename is not in format abcd_x.ext where x is 1, 2, 4 or 8 - the zoom factor to use.
			continue;
			}
		Printf(_L(" - Zoom factor %d "),zoomValue);

		TInt err;
		TUid imageType = ImageType(fileName);

		TRAP(err,DecodeFileToBitmapL(fileName,src,zoomValue,imageType));
		if(err)
			{			
			TestFailed(err);
			continue;
			}
		RFile file;
		User::LeaveIfError(file.Open(iFs,fileName,EFileShareReadersOnly|EFileStream|EFileRead));
		CleanupClosePushL(file);
		TInt size = 0;
		User::LeaveIfError(file.Size(size));
		HBufC8* buffer = HBufC8::NewMaxLC(size);
		TPtr8 bufferDes(buffer->Des());
		User::LeaveIfError(file.Read(bufferDes));

		TUint maxDelay=0;
		TRAP(err,DecodeDescToBitmapWithCancelL(*buffer,dest,zoomValue,imageType, maxDelay));

		Printf(_L(" - Max delay = %d\n"), maxDelay);

		if(err)
			{
			TestFailed(err);
			CleanupStack::PopAndDestroy(2);//file,buffer
			continue;
			}

		if (maxDelay>KMaxToleratedDelay)
			{
//Test does not fail under WINS, since other processes
//(like virus scanning etc) have an effect on emulator timing.
#if defined(__WINS__)
			Printf(_L("Liveliness problem - maxDelay=%d\n"), maxDelay);
#else
			TestFailed(_L("Liveliness problem - maxDelay=%d\n"), maxDelay);
			continue;
#endif //defined(__WINS__)
			}

		CheckBitmapL(fileName, dest);
		CleanupStack::PopAndDestroy(2);//file,buffer
		}
	CleanupStack::PopAndDestroy(dir); 
	}

//
//
// Decode Cancel Tests...
//
//

//
// CDecoderCancelTimer - on RunL() cancel the a decoder and stop AS
//

class CCancelTimer : public CTimer
	{
public:
	static CCancelTimer* NewLC(TTimeIntervalMicroSeconds32& aCancelDelay, CActive* aListener);
	~CCancelTimer();

	void InitializeCancelTimer();

private:
    CCancelTimer(CActive* aListener);
    void ConstructL(TTimeIntervalMicroSeconds32& aCancelDelay);

	// from CActive
    void RunL();
	void DoCancel();

private:
    TTimeIntervalMicroSeconds32 iCancelDelay;
    CActive*const iListener; // not owned
    };

CCancelTimer::CCancelTimer(CActive* aListener)
	: CTimer(CActive::EPriorityUserInput),
    iListener(aListener)
	{
    }


CCancelTimer* CCancelTimer::NewLC(TTimeIntervalMicroSeconds32& aCancelDelay, CActive* aListener)
    {
    CCancelTimer* self = new (ELeave) CCancelTimer(aListener);
    CleanupStack::PushL(self);
    self->ConstructL(aCancelDelay);
    return self;
    }

void CCancelTimer::ConstructL(TTimeIntervalMicroSeconds32& aCancelDelay)
    {
    CTimer::ConstructL();

    iCancelDelay = aCancelDelay;
    CActiveScheduler::Add(this);
    }

CCancelTimer::~CCancelTimer()
    {
    Cancel();
    }


void CCancelTimer::InitializeCancelTimer()
    {
  // Request another wait - assume not active
	CTimer::After(iCancelDelay);
    }

void CCancelTimer::RunL()
    {
    iListener->Cancel();
    
    CActiveScheduler::Stop();
    }

void CCancelTimer::DoCancel()
	{
	iListener->Cancel();
	}

const TInt KCancelDelayIncrement = 10000; // 10ms

void TImage::DecodeFileToBitmapWithCancelL(const TDesC& aFileName,
										   CFbsBitmap& aBitmap,
										   TInt aZoomFactor,
										   const TUid aImageType,
										   TUint& aMaxDelay)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, aFileName, DecoderOptions(), aImageType);
	CleanupStack::PushL(decoder);

	TFrameInfo frameInfo(decoder->FrameInfo());

	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	//Workaround to use same WMF reference bitmaps on all platforms
	if(aImageType == KImageTypeWMFUid)
		frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));

	TUint cancelDelay = 0;
	aMaxDelay = 0;
    
    for(;;)
        {
        TTimeIntervalMicroSeconds32 tmpTime(cancelDelay);
        
        CActiveDecodeListener* activeListener = new(ELeave)CActiveDecodeListener;
        CleanupStack::PushL( activeListener );

        CCancelTimer* timer = CCancelTimer::NewLC(tmpTime, activeListener);
                 
        activeListener->InitialiseActiveListener(decoder);
        decoder->Convert(&activeListener->iStatus,aBitmap, NULL);

		timer->InitializeCancelTimer();
		TTime startTime; startTime.UniversalTime();

		// TODO::Temporary fix until we can find out what is
		// wrong with User::After(0)
		User::After(1);
 
        CActiveScheduler::Start();
        Printf(_L(" - result of decode: %d"), activeListener->iStatus.Int());
		TTime endTime; endTime.UniversalTime();
        
 		// get status
		TBool timerFinished = !timer->IsActive();
		ASSERT(!activeListener->IsActive()); // either decoder finished on its own accord, or timer canceled
 		TInt decoderStatus = activeListener->iStatus.Int();        

		// then cancel - so we can leave without issue
		timer->Cancel();

		// get rid of the two AOs anyway
		CleanupStack::PopAndDestroy(2, activeListener);

		// timer must expire for immediate cancel
		if(!cancelDelay && !timerFinished)
			{
			Printf(_L("Timer did not expire for immediate cancel"));
			User::Leave(KErrCorrupt);
			}

		TUint uSTime = I64INT(endTime.Int64() - startTime.Int64());
		// check that the result came back in an OK time
		if (uSTime < cancelDelay)
            {
			if (timerFinished)
				Printf(_L("timed delay(%d) was faster than the requested delay(%d)"), uSTime, cancelDelay);
            }
        else
            {
			uSTime -= cancelDelay;
			if (uSTime>aMaxDelay)
				aMaxDelay = uSTime;
            }

		// if we cancel'd (ie. timer fired) then we continue with extra delay, otherwise finish or leave with result

		if (timerFinished)
			{
			cancelDelay += KCancelDelayIncrement;
			}
		else
			{
			if (decoderStatus==KErrNone)
				{
				break; // exit loop - normal exit
				}
			else 
				User::Leave(decoderStatus);
			}
        }
	CleanupStack::PopAndDestroy(decoder);	
	}

void TImage::DecodeDescToBitmapWithCancelL(const TDesC8& aBufferDes,
										   CFbsBitmap& aBitmap,
										   TInt aZoomFactor,
										   const TUid aImageType,
										   TUint& aMaxDelay)
	{
	CImageDecoder* decoder = CImageDecoder::DataNewL(iFs, aBufferDes, DecoderOptions(), aImageType);
	CleanupStack::PushL(decoder);

	TFrameInfo frameInfo(decoder->FrameInfo());

	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),frameInfo.iFrameCoordsInPixels.Height());
	//Workaround to use same WMF reference bitmaps on all platforms
	if(aImageType == KImageTypeWMFUid)
		frameSize.SetSize(frameInfo.iFrameSizeInTwips.iWidth/KWmfPixelSize, frameInfo.iFrameSizeInTwips.iHeight/KWmfPixelSize);
	frameSize.iWidth = (frameSize.iWidth+aZoomFactor-1) / aZoomFactor;
	frameSize.iHeight = (frameSize.iHeight+aZoomFactor-1) / aZoomFactor;
	User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
	User::LeaveIfError(aBitmap.Resize(frameSize));

	TUint cancelDelay = 0;
	aMaxDelay = 0;
    
    for(;;)
        {
        TTimeIntervalMicroSeconds32 tmpTime(cancelDelay);
        
        CActiveDecodeListener* activeListener = new(ELeave)CActiveDecodeListener;
        CleanupStack::PushL( activeListener );

        CCancelTimer* timer = CCancelTimer::NewLC(tmpTime, activeListener);
                 
        activeListener->InitialiseActiveListener(decoder);
        decoder->Convert(&activeListener->iStatus,aBitmap, NULL);

		timer->InitializeCancelTimer();
		TTime startTime; startTime.UniversalTime();

		// TODO::Temporary fix until we can find out what is
		// wrong with User::After(0)
		User::After(1);
 
        CActiveScheduler::Start();
		TTime endTime; endTime.UniversalTime();
        
 		// get status
		TBool timerFinished = !timer->IsActive();
		ASSERT(!activeListener->IsActive()); // either decoder finished on its own accord, or timer canceled
 		TInt decoderStatus = activeListener->iStatus.Int();        

		// then cancel - so we can leave without issue
		timer->Cancel();

		// get rid of the two AOs anyway
		CleanupStack::PopAndDestroy(2, activeListener);

		// timer must expire for immediate cancel
		if(!cancelDelay && !timerFinished)
			{
			Printf(_L("Timer did not expire for immediate cancel"));
			User::Leave(KErrCorrupt);
			}

		TUint uSTime = I64INT(endTime.Int64() - startTime.Int64());
		// check that the result came back in an OK time
		if (uSTime < cancelDelay)
            {
			if (timerFinished)
				Printf(_L("timed delay(%d) was faster than the requested delay(%d)"), uSTime, cancelDelay);
            }
        else
            {
			uSTime -= cancelDelay;
			if (uSTime>aMaxDelay)
				aMaxDelay = uSTime;
            }

		// if we cancel'd (ie. timer fired) then we continue with extra delay, otherwise finish or leave with result

		if (timerFinished)
			{
			cancelDelay += KCancelDelayIncrement;
			}
		else
			{
			if (decoderStatus==KErrNone)
				{
				break; // exit loop - normal exit
				}
			else 
				User::Leave(decoderStatus);
			}
        }
	CleanupStack::PopAndDestroy(decoder);	
	}

const TInt KCancelEncodeWidth = 440;
const TInt KCancelEncodeHeight = 220;
void TImage::Image17L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image17 test(%d) ---"), aFormat);
	Printf(_L("\n"));

	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);

	if(aTestFile==KNullDesC)
		{
		// blank bitmap of given height and size
		User::LeaveIfError(bitmap->Create(TSize(0,0),EColor16M));
		User::LeaveIfError(bitmap->Resize(TSize(KCancelEncodeWidth, KCancelEncodeHeight))); 
		Printf(_L("Encoding Blank Bitmap"));
		}
	else
		{
		SetTestFileNameL(iSourceFileName, aTestFile);
		User::LeaveIfError(bitmap->Load(iSourceFileName));
		Printf(_L("Encoding %S"), &iSourceFileName);
		}

	for (TInt formatIndex = 0; ;formatIndex++)
		{
		TUid imageType;
		imageType.iUid = 0;

		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);

		const TDesC& configType = ConfigImageDataAndTypeL(*frameImageData, imageType, formatIndex, aFormat);
		if (configType==KNullDesC)
			{
			CleanupStack::PopAndDestroy(frameImageData); 
			break;
			}

		Printf(configType);
		Printf(_L("\n"));

		TInt err;
		HBufC8* buffer2 = NULL;
		TUint maxDelay=0;


		TRAP(err, EncodeBitmapToDescWithCancelL(buffer2, bitmap, frameImageData, imageType, maxDelay));

		Printf(_L("Max delay=%d\n"), maxDelay);


		CleanupStack::PopAndDestroy(frameImageData); 

		if(err!=KErrNone)
			{
			if (buffer2)
				TestFailed(_L("Error (%d) - even worse, buffer was set"), err);
			else
				TestFailed(err);
			}

		if (maxDelay>KMaxToleratedDelay)
			{
//Test does not fail under WINS, since other processes
//(like virus scanning etc) have an effect on emulator timing.
#if defined(__WINS__)
			Printf(_L("Liveliness problem - maxDelay=%d\n"), maxDelay);
#else
			TestFailed(_L("Liveliness problem - maxDelay=%d\n"), maxDelay);
			continue;
#endif //defined(__WINS__)
			}
		
		// assume this works - no reason to believe otherwise
		delete buffer2;
		}
	CleanupStack::PopAndDestroy(bitmap); 
	}



void TImage::EncodeBitmapToDescWithCancelL(HBufC8*& aBufferDes, 
										   CFbsBitmap* aBitmap, 
										   const CFrameImageData* aFrameImageData, 
										   const TUid aImageType,
										   TUint& aMaxDelay)
	{
	TUint cancelDelay = 0;
	aMaxDelay = 0;
    
    for(;;)
        {
        TTimeIntervalMicroSeconds32 tmpTime(cancelDelay);
        
        CActiveEncodeListener* activeListener = new(ELeave)CActiveEncodeListener;
        CleanupStack::PushL( activeListener );

		delete aBufferDes; aBufferDes=NULL; // delete bitmap here just in case - could be upset by loop
		CImageEncoder* encoder = CImageEncoder::DataNewL(aBufferDes, EncoderOptions(), aImageType);
		CleanupStack::PushL(encoder);

        CCancelTimer* timer = CCancelTimer::NewLC(tmpTime, activeListener);
                 
        activeListener->InitialiseActiveListener(encoder);
		encoder->Convert(&activeListener->iStatus,*aBitmap,aFrameImageData);

 
		TTime startTime; 
		startTime.UniversalTime();
        timer->InitializeCancelTimer();

        CActiveScheduler::Start();

		TTime endTime; endTime.UniversalTime();
        
 		// get status
		TBool timerFinished = !timer->IsActive();
		ASSERT(!activeListener->IsActive()); // either encoder finished on its own accord, or timer canceled
 		TInt encoderStatus = activeListener->iStatus.Int();        

		// then cancel - so we can leave without issue
		timer->Cancel();

		// get rid of the two AOs anyway and encoder
		CleanupStack::PopAndDestroy(3, activeListener);

		TUint uSTime = I64INT(endTime.Int64() - startTime.Int64());
		// check that the result came back in an OK time
		if (uSTime < cancelDelay)
            {
			if (timerFinished)
				Printf(_L("timed delay(%d) was faster than the requested delay(%d)"), uSTime, cancelDelay);
            }
        else
            {
			uSTime -= cancelDelay;
			if (uSTime>aMaxDelay)
				aMaxDelay = uSTime;
            }

		// if we cancel'd (ie. timer fired) then we continue with extra delay, otherwise finish or leave with result

		if (timerFinished)
			{
			cancelDelay += KCancelDelayIncrement;
			}
		else
			{
			if (encoderStatus==KErrNone)
				{
				break; // exit loop - normal exit
				}
			else 
				User::Leave(encoderStatus);
			}
 		}
	}
/**
	additional BMP tests
*/
class TBmpTestPar
	{
public:
	const TText* iTestFile;
	const TText* iRefFile;
	TInt iExpectedErr;
	};
	
/**
	Test of decoding of bitfield-encoded bmp formats
	such as 16bpp 565, 16bpp 555, 32bpp RGB and 32bpp BGR
	Also tests proper handling of "trimmed" files
*/
void TImage::Image18L(TImageFormat /*aFormat*/,const TDesC& /*aTestFile*/)
	{
	#define KTrueColorRef _S("\\TImage\\bmp\\mbm24bpp.mbm")
	static const TBmpTestPar KTestPairs[]=
		{
			{_S("32bppRgb.bmp"), KTrueColorRef, KErrNone},
			{_S("32bppBgr.bmp"), KTrueColorRef, KErrNone},
			{_S("16bpp565.bmp"), _S("\\TImage\\bmp\\16bpp565.mbm"), KErrNone},
			{_S("16bpp555.bmp"), _S("\\TImage\\bmp\\16bpp555.mbm"), KErrNone},
			{_S("16bpp565_badmask.bmp"), KTrueColorRef, KErrNotFound},
			{_S("32bppBgr_truncated.bmp"), KTrueColorRef, KErrUnderflow},
			
			{NULL,NULL}
		};
	#undef KTrueColorRef
		
	iDecoderOptions = CImageDecoder::EOptionNoDither; // we do not want error diffusion

	// iterate through the test files
	for (const TBmpTestPar* testPair=KTestPairs; testPair->iTestFile; ++testPair)
		{
		TFileName dir(_L("\\TImage\\bmp\\"));
		dir.Append(TPtrC(testPair->iTestFile));
		SetTestFileNameL(iSourceFileName,dir);
		
		TParse parse;
		User::LeaveIfError(parse.SetNoWild(iSourceFileName,NULL,NULL));
		
		dir.Copy( parse.Drive() );
		dir.Append(TPtrC(testPair->iRefFile));		
		
		CFbsBitmap* refBitmap = new(ELeave) CFbsBitmap();
		CleanupStack::PushL(refBitmap);
		User::LeaveIfError( refBitmap->Load(dir) );
		
		CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(bitmap);
		
		// use destination bitmap size and mode according to reference one
		User::LeaveIfError( bitmap->Create(refBitmap->SizeInPixels(), refBitmap->DisplayMode() ));
		
		TRAPD (err,
				DecodeFileToBitmapL(iSourceFileName, *bitmap);
			);
		// unexpected error encoutered
		if (err != testPair->iExpectedErr)
			{
			Printf(_L("Decode of (%s) failed with an unexpected error (%d)"),testPair->iTestFile, err);
			TestFailed(err);
			}
		else
			{
			// check result bitmap against reference one in case of successfull decoding completion
			if (err == KErrNone)
				{
				CheckBitmaps(*refBitmap, *bitmap);
				}
			if (*iVerdict != EPass)
				{
				Printf(_L("Image do match to ref. one (%s) (%s)"),testPair->iTestFile, testPair->iRefFile);
				}
			}
		CleanupStack::PopAndDestroy(2, refBitmap);
		refBitmap = bitmap = NULL;
		}
	}

/* Test extensions. 
4 JPEG input files: 24bit JFIF, Mono JFIF, 24bit+thumbnail JFIF, 24bit+thumbnail EXIF
	24bit image decodes to 
		EColor16M (raw image processor)
		EColor16 (pixel writer + error diffusion)
	Mono image decodes to
		EGray256 (mono pixel writer + error diffusion)
Each of above combinations
	Scale 1/2 Turns on thumbnail processor
Each of the above with post processing
	Clip
	Operate
	Scale through CImageConvScaler
	Combination of above three
Each of the above
	Redecode (varying post processing)
		Clip -> Operate
		Operate -> Scale
		Scale -> Clip
		Combination -> None
Each of the above
	Threaded / Non-threaded
*/
#ifndef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
void TImage::Image19L(TImageFormat , const TDesC& )
	{
	Printf(_L("--- PREQ1630 JPEG Alloc tests not run in this configuration ---"));
	return;
	}
#else
void TImage::Image19L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image19 test(%d) --- %S"), aFormat, &aTestFile);
	
#ifdef __EABI__
	switch(Math::Random() % 10)
		{
		case 0:		
			DoImage19_10L(aFormat, _L("\\TImage\\jfif100_8.jpg"),EColor16M); break;
		case 1:		
			DoImage19_10L(aFormat, _L("\\TImage\\jfif100_8.jpg"),EColor16); break;
		case 2:
			DoImage19_10L(aFormat, _L("\\TImage\\jfifmono_2.jpg"),EGray256); break;
		case 3:		
			DoImage19_10L(aFormat, _L("\\TImage\\jfifmono_2.jpg"),EGray16); break;
		case 4:
			DoImage19_10L(aFormat, _L("\\TImage\\qvga_jfif_thumb.jpg"),EColor16M); break;
		case 5:
			DoImage19_10L(aFormat, _L("\\TImage\\qvga_jfif_thumb.jpg"),EColor16); break;
		case 6:
			DoImage19_10L(aFormat, _L("\\TImage\\qvga_exif_thumb.jpg"),EColor16M); break;
		case 7:		
			DoImage19_10L(aFormat, _L("\\TImage\\qvga_exif_thumb.jpg"),EColor16); break;
		case 8:		
			DoImage19_10L(aFormat, _L("\\TImage\\jpeg_multiscan.jpg"),EColor16M); break;
		case 9:
			DoImage19_10L(aFormat, _L("\\TImage\\jpeg_multiscan.jpg"),EColor16); break;
		}
#else
	DoImage19_10L(aFormat, _L("\\TImage\\jfif100_8.jpg"),EColor16M);
	DoImage19_10L(aFormat, _L("\\TImage\\jfif100_8.jpg"),EColor16);

	DoImage19_10L(aFormat, _L("\\TImage\\jfifmono_2.jpg"),EGray256);
	DoImage19_10L(aFormat, _L("\\TImage\\jfifmono_2.jpg"),EGray16);

	DoImage19_10L(aFormat, _L("\\TImage\\qvga_jfif_thumb.jpg"),EColor16M);
	DoImage19_10L(aFormat, _L("\\TImage\\qvga_jfif_thumb.jpg"),EColor16);

	DoImage19_10L(aFormat, _L("\\TImage\\qvga_exif_thumb.jpg"),EColor16M);
	DoImage19_10L(aFormat, _L("\\TImage\\qvga_exif_thumb.jpg"),EColor16);
	
	DoImage19_10L(aFormat, _L("\\TImage\\jpeg_multiscan.jpg"),EColor16M);
	DoImage19_10L(aFormat, _L("\\TImage\\jpeg_multiscan.jpg"),EColor16);
#endif
	}
#endif

// struct to hold post processing options for decode
class TDecodePostProcessing
	{
public:
	TInt iClipFactor; // divisor for clip 2 implies half size etc.
	TUniqueTransform iTransformOption;
	TInt iScaleCoeff;
	TDecodePostProcessing(TInt aClipFactor,TUniqueTransform aTransformOption,TInt aScaleCoeff)
	  : iClipFactor(aClipFactor), iTransformOption(aTransformOption), iScaleCoeff(aScaleCoeff) {}
	};
	
// Alloc test for jpeg files using PREQ1630 extensions (clip, scale, rotate/mirror)
// Tests all files in \timage\*.jpg (JFIF images plus one image with thumbnail)
void TImage::DoImage19_10L(TImageFormat aFormat, const TDesC& aTestFile,TDisplayMode aDisplayMode)
	{
	Printf(_L("\n--- Image19_10 test(%d) --- file %S displaymode %d"), aFormat, &aTestFile, aDisplayMode);

	CFbsBitmap src;
	User::LeaveIfError(src.Create(TSize(0,0),aDisplayMode));

	for (TInt count = 0; count < 2; count++) // change thread model
		{
		TInt restartFailCount = 1; // prevents repeating alloc testing FileNewL on same file
		
#ifdef __EABI__
		//On HW this test should test randomly either with or without thread.
		if(count > 0)
			{
			break;
			}
		SetUseThread(Math::Random() % 2);
		Printf(_L("This is a SMOKE_TEST on HW testing randomly selected thread and postprocessing algorithm. This is full test on winscw"));		
#else	
		// alternate between threaded and non-threaded
		SetUseThread(count % 2);
#endif		
		TInt err = 0;
		TUid imageType = ImageType(aTestFile);

		for (TInt j = 0; j<6 ; j++)
			{
			TDecodePostProcessing postProcessing(0,ENormal,-1);
			TDecodePostProcessing redecodePostProcessing(0,ENormal,-1); // second convert
			TBool prepare = EFalse;
#ifdef __EABI__
			//On HW this test should test randomly only one combination of the postProessing.
			if(j > 0)
				{
				break;
				}
			switch ( Math::Random() % 6 ) 	
#else							
			switch ( j )
#endif			
				{
				case 0: // Clip -> Operate
					postProcessing = TDecodePostProcessing(2,ENormal,-1);
					redecodePostProcessing = TDecodePostProcessing(0,ERotate180,-1);
					break;
				case 1: // Operate -> Scale
					postProcessing = TDecodePostProcessing(0,ERotate90,-1);
					redecodePostProcessing = TDecodePostProcessing(0,ENormal,-3);
					break;
				case 2: // Scale -> Clip
					postProcessing = TDecodePostProcessing(0,ENormal,-2);
					redecodePostProcessing = TDecodePostProcessing(2,ENormal,-1);
					break;
				case 3: // Combination -> None
					postProcessing = TDecodePostProcessing(3,EVerticalFlipRotate90,-4);
					redecodePostProcessing = TDecodePostProcessing(0,ENormal,-1);
					break;
				case 4: // None -> Combination
					postProcessing = TDecodePostProcessing(0,ENormal,-1);
					redecodePostProcessing = TDecodePostProcessing(3,EVerticalFlipRotate90,-4);
					break;
				case 5:
					prepare = ETrue;
					break;
				default:
					ASSERT(0);
					break;
				}

			Printf(_L("Apply clip factor %d, scaling coeff %d, operation %08x threaded %d..."),postProcessing.iClipFactor,postProcessing.iScaleCoeff,postProcessing.iTransformOption,UseThread());
			Printf(_L("...then with clip factor %d, scaling coeff %d, operation %08x prepare %d"),redecodePostProcessing.iClipFactor,redecodePostProcessing.iScaleCoeff,redecodePostProcessing.iTransformOption,prepare);

			TInt dummy = 0;				
			TRAP(err,DecodeFileToBitmapUsingExtensionsL(aTestFile,src,imageType,postProcessing,redecodePostProcessing,prepare,0,dummy));
			if(err)
				{
				TestFailed(err);
				continue;
				}
			DecodeFileToBitmapUsingExtensionsWithAllocFailureL(aTestFile,src,imageType,postProcessing,redecodePostProcessing,prepare,aDisplayMode,restartFailCount);
			}
		}
	}


void TImage::DecodeFileToBitmapUsingExtensionsL(const TDesC& aFileName,CFbsBitmap& aBitmap,const TUid aImageType,const TDecodePostProcessing& aPostProcessing, const TDecodePostProcessing& aRedecodePostProcessing,TBool aPrepare,TInt aFailCount,TInt& aRestartFailCount)
	{
	CImageDecoder::TOptions options = static_cast<CImageDecoder::TOptions>
								   (CImageDecoder::EOptionExtCrop | CImageDecoder::EOptionExtRotation | 
									CImageDecoder::EOptionExtMirrorHorizontalAxis | CImageDecoder::EOptionExtMirrorVerticalAxis | 
									CImageDecoder::EOptionExtScaling | DecoderOptions());
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFs, aFileName, options, aImageType);
	CleanupStack::PushL(decoder);

	if ( aRestartFailCount == 1 )
		{
		aRestartFailCount = aFailCount - 1;	
		}

	if ( aPrepare )
		{
		iActiveListener->InitialiseActiveListener();
		decoder->Prepare(&iActiveListener->iStatus);
		CActiveScheduler::Start();
		if( iActiveListener->iStatus!=KErrNone && iActiveListener->iStatus!=KErrNotSupported )
			User::Leave(iActiveListener->iStatus.Int());
		}
		
	// look for thumbnail
	TRAPD(err,decoder->SetImageTypeL(CImageDecoder::EImageTypeThumbnail));
	if ( err == KErrNone )
		{
		// Testing thumbnail decode
		}
	else if ( err != KErrNotFound )
		{
		User::Leave(err);
		}
		
	for ( TInt i=0; i<2; i++ ) // decode, ten re-decode with new post processing
		{
		const TDecodePostProcessing* postProcess = ( i == 0 ) ? &aPostProcessing : &aRedecodePostProcessing;

		ApplyDecoderTransformL(decoder,*postProcess);
		
		TSize destSize;
		User::LeaveIfError(decoder->GetDestinationSize(destSize,0));
		User::LeaveIfError(aBitmap.Resize(TSize(0,0)));
		User::LeaveIfError(aBitmap.Resize(destSize));

		iActiveListener->InitialiseActiveListener();
		decoder->Convert(&iActiveListener->iStatus,aBitmap);
		CActiveScheduler::Start();
		if(iActiveListener->iStatus!=KErrNone)
			User::Leave(iActiveListener->iStatus.Int());
		}
	
	CleanupStack::PopAndDestroy(decoder); 
	}


void TImage::DecodeFileToBitmapUsingExtensionsWithAllocFailureL(const TDesC& aFileName,CFbsBitmap& aReference,const TUid aImageType,const TDecodePostProcessing& aPostProcessing, const TDecodePostProcessing& aRedecodePostProcessing,TBool aPrepare,TDisplayMode aDisplayMode,TInt& aRestartFailCount)
	{
	TInt err = 0;
	
	CFbsBitmap bitmap;
	User::LeaveIfError(bitmap.Create(TSize(0,0), aDisplayMode));

	TInt failCount = aRestartFailCount; // prevents repeating alloc test on FileNewL for same file
	TBool completed = EFalse;
	TBool badResult = EFalse; 
	TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
	for (;;)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
		__MM_HEAP_MARK;

		TRAP(err,DecodeFileToBitmapUsingExtensionsL(aFileName,bitmap,aImageType,aPostProcessing,aRedecodePostProcessing,aPrepare,failCount,aRestartFailCount));
			
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
			if (!CompareBitmaps (bitmap, aReference))
				{
				badResult = ETrue;
				}

			completed = reachedEnd || badResult;
			}
		else if (err != KErrNoMemory) // bad error code 
			completed = ETrue;

		__MM_HEAP_MARKEND;
		__UHEAP_SETFAIL(RHeap::ENone, 0);

		if (completed)
			break; // exit loop

		failCount += 1;
		}

	failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.

	if (err != KErrNone || badResult)
		{
		TBuf<80> format;
		if (badResult)
			format.Format(_L("  Bad result with %d memory allocations tested"),failCount);
		else
			format.Format(_L("  Error(%d) with %d memory allocations tested"),err,failCount);
		Printf(format);
		TestFailed(badResult ? KErrNone : err);
		}
	}

void TImage::ApplyDecoderTransformL(CImageDecoder* aDecoder,const TDecodePostProcessing& aPostProcess)
	{
	TFrameInfo frameInfo(aDecoder->FrameInfo());
	
	TRect clipRect;
	
	if ( aPostProcess.iClipFactor )
		{
		clipRect.SetRect(0,0,frameInfo.iOverallSizeInPixels.iWidth/aPostProcess.iClipFactor,
							 frameInfo.iOverallSizeInPixels.iHeight/aPostProcess.iClipFactor);
		aDecoder->SetClippingRectL(&clipRect);
		}
	else
		{
		aDecoder->SetClippingRectL(NULL);
		}
	
	TImageConvScaler* scaler = aDecoder->ScalerL();
	TImageConvScaler::TScalerQuality quality = TImageConvScaler::EMinimumQuality;
	switch ( aPostProcess.iScaleCoeff )
		{
		case -2:
			quality = TImageConvScaler::EMediumQuality;
			break;
		case -3:
			quality = TImageConvScaler::EMaximumQuality;
			break;
		default:
			break;
		}
	scaler->SetScalingL(aPostProcess.iScaleCoeff ,quality);

	TImageConvOperation* operation = aDecoder->OperationL();
	ApplyOperationTransformL(operation,aPostProcess.iTransformOption);		
	}

	
void TImage::ApplyOperationTransformL(TImageConvOperation* aOperation,TUniqueTransform aTransformOption)
	{
	switch ( aTransformOption )
		{
		case ENormal: 	// no transform
		default:
			aOperation->ClearOperationStack();
			break;

		case ERotate90:
			aOperation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
			break;

		case ERotate180:
			aOperation->AddOperationL(TImageConvOperation::ERotation180DegreesClockwise);
			break;

		case ERotate270:
			aOperation->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise);
			break;
		
		case EHorizontalFlip:
			aOperation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
			break;
		
		case EHorizontalFlipRotate90:
			aOperation->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis);
			aOperation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
			break;

		case EVerticalFlip:
			aOperation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
			break;

		case EVerticalFlipRotate90:
			aOperation->AddOperationL(TImageConvOperation::EMirrorVerticalAxis);
			aOperation->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise);
			break;
		}
	}

void TImage::Image20L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image20 test(%d) alloc fail on decoder plugin with no extensions --- %S"), aFormat, &aTestFile);

	TInt failCount = 0; 
	TInt err = KErrNone;
	TBool completed = EFalse;
	
	TFileName fileName = iDefaultPath;
	fileName.Append(aTestFile);

	for ( TInt i=0; i<2; i++ ) // alternate between thread and non thread
		{
		SetUseThread(i%2); 
		CImageDecoder* decoder = CImageDecoder::FileNewL(iFs,fileName,DecoderOptions());
 		CleanupStack::PushL(decoder);

		for ( TInt j=0; j<5; j++ ) // test each possible extension
			{
			failCount = 1;
			completed = EFalse;
			
			for (;;) // allow increasing memory allocations to pass until test fails with KErrNotSupported
				{
				__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
				__MM_HEAP_MARK;

				// test alloc failure 					
				switch (j)
					{
					case 0:
						{
						TRect clipRect(0,0,10,10);
						TRAP(err,decoder->SetClippingRectL(&clipRect));
						}
						break;
					case 1:
						TRAP(err,TImageConvScaler* scaler = decoder->ScalerL());
						break;
					case 2:
						TRAP(err,TImageConvOperation* operation = decoder->OperationL());
						break;
					case 3:
						TRAP(err,TImageConvStreamedDecode* streamer = decoder->BlockStreamerL());
						break;
					case 4:
						iActiveListener->InitialiseActiveListener();
						TRAP(err,decoder->Prepare(&iActiveListener->iStatus));
						if ( err == KErrNone )
							{
							CActiveScheduler::Start();
							err = iActiveListener->iStatus.Int();
							}
						else
							{
							User::Leave(err);
							}
						break;
					default:
						ASSERT(0);
						break;
					}
					
				if ( err == KErrNotSupported )
					{
					TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
					if (testAlloc==NULL)
						{
						completed = ETrue;
						failCount -= 1;
						}
					else
						User::Free(testAlloc);
					}
				else if (err != KErrNoMemory) // bad error code
					completed = ETrue;

				__MM_HEAP_MARKEND;
				__UHEAP_SETFAIL(RHeap::ENone, 0);

				if (completed)
					break; // exit loop

				failCount++;
				}
				
			if (err != KErrNotSupported)
				{
				TBuf<80> format;
				format.Format(_L("  Test %d threaded %d Error(%d) with %d memory allocations tested"),j,UseThread(),err,failCount);
				Printf(format);
				TestFailed(err);
				}
			else 
				{
				TBuf<80> format;
				format.Format(_L("  Test %d threaded %d Completed OK %d memory allocations tested"),j,UseThread(),failCount);
				Printf(format);
				}
			}
		CleanupStack::PopAndDestroy(decoder);
		decoder = NULL;
		}
	}

#ifndef SYMBIAN_ENABLE_1630_JPEG_EXTENSIONS
void TImage::Image21L(TImageFormat , const TDesC& )
	{
	Printf(_L("--- PREQ1630 JPEG Alloc tests not run in this configuration ---"));
	return;
	}
#else
void TImage::Image21L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	_LIT(KImageTestExtSourceFileName, "\\TImage\\ref\\qvga_24bpp.mbm");

	Printf(_L("--- Image21 test(%d) --- %S"), aFormat, &aTestFile);
	
	SetTestFileNameL(iSourceFileName,KImageTestExtSourceFileName);
	
	DoEncoderAllocTest(aFormat,aTestFile,ETrue);  
	}
#endif

void TImage::Image22L(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image22 test(%d) alloc fail on encoder plugin with no extensions --- %S"), aFormat, &aTestFile);

	TInt failCount = 0; 
	TInt err = KErrNone;
	TBool completed = EFalse;
	
	TFileName fileName;
	SetTestFileNameL(fileName, KImageTestOutDir);
	iFs.MkDirAll(fileName);
	
	fileName.Append(aTestFile);
	
	TUid imageTypeUid = ImageType(fileName);

	// threaded and non-threaded options
	for ( TInt i=0; i<2; i++ )
		{
		SetUseThread(i%2); // alternate between thread and non thread
 		CImageEncoder* encoder = CImageEncoder::FileNewL(iFs, fileName, EncoderOptions(), imageTypeUid);
 		CleanupStack::PushL(encoder);
 		
		// all possible extensions
		for ( TInt j=0; j<=2; j++ )
			{
			failCount = 1;
			completed = EFalse;
			
			// for each extension API
			for (;;)
				{
				__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
				__MM_HEAP_MARK;

				// test alloc failure 					
				switch (j)
					{
					case 0:
						TRAP(err,TImageConvOperation* operation = encoder->OperationL());
						break;
					case 1:
						TRAP(err,TImageConvStreamedEncode* streamer = encoder->BlockStreamerL());
						break;
					case 2:
						iActiveListener->InitialiseActiveListener();
						TRAP(err,encoder->Prepare(&iActiveListener->iStatus));
						if ( err == KErrNone )
							{
							CActiveScheduler::Start();
							err = iActiveListener->iStatus.Int();
							}
						else
							{
							User::Leave(err);
							}
						break;
					default:
						ASSERT(0);
						break;
					}
					
				if ( err == KErrNotSupported )
					{
					TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
					if (testAlloc==NULL)
						{
						completed = ETrue;
						failCount -= 1;
						}
					else
						User::Free(testAlloc);
					}
				else if (err != KErrNoMemory) // bad error code
					completed = ETrue;

				__MM_HEAP_MARKEND;
				__UHEAP_SETFAIL(RHeap::ENone, 0);

				if (completed)
					break; // exit loop

				failCount++;
				}

			if (err != KErrNotSupported)
				{
				TBuf<80> format;
				format.Format(_L("  Test %d threaded %d Error(%d) with %d memory allocations tested"),j,UseThread(),err,failCount);
				Printf(format);
				TestFailed(err);
				}
			else 
				{
				TBuf<80> format;
				format.Format(_L("  Test %d threaded %d Completed OK %d memory allocations tested"),j,UseThread(),failCount);
				Printf(format);
				}
			}
		
		CleanupStack::PopAndDestroy(encoder);
		encoder = NULL;
		}
		
	// tidy up the temp file (0 length)
	iFs.Delete(fileName);
	}
	
void TImage::ImageIncrL(TImageFormat aFormat, const TDesC& aTestFile)
	{
	Printf(_L("--- Image2 test(%d) --- %S"), aFormat, &aTestFile);
	TInt aFrameNo = 0;
	TInt	theRes;
	TFileName	sourceFileName;
	sourceFileName = KJpegFilePath;
	sourceFileName.Append(KJpegFileName);
	RFile theFile;
	
	theRes = theFile.Open(iFs, sourceFileName, EFileShareReadersOnly|EFileStream|EFileRead);
	
	if(theRes != KErrNone)
		{
		iFs.Close();
		Printf(_L("Cannot open file : %S"), &sourceFileName);

		return;
		}
	else
		{
		Printf(_L("File %S was opened successfully"), &sourceFileName);
		}
	
	CleanupClosePushL(theFile);
	TInt fileSize = 0;
	User::LeaveIfError(theFile.Size(fileSize));

	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(1024)), 0, 1024);

	RFile destFile;

	TFileName	destFileName;
	destFileName = KJpegFileOutPath;
	destFileName.Append(KJpegFileName);
	TInt  destFileLength = 0;

	theRes = destFile.Create(iFs, destFileName, EFileShareAny|EFileStream|EFileWrite);
	//theRes = destFile.Create(iFs, destFileName, EFileShareAny);
	if(theRes == KErrAlreadyExists)
		User::LeaveIfError(theRes = destFile.Replace(iFs, destFileName, EFileShareAny|EFileStream|EFileWrite));
	else if(theRes)
		{
		Printf(_L("Cannot create destination file %S"), &destFileName);
		User::LeaveIfError(theRes);
		}
	
	CleanupClosePushL(destFile);

	Printf(_L("Destination file %S was created successfully"), &destFileName);

	TInt wantedBufferSize=1;

	User::LeaveIfError(theFile.Read(buffer, wantedBufferSize));
	ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
	User::LeaveIfError(destFile.Write(buffer));
	destFileLength += wantedBufferSize;
	CImageDecoder *theImageDecoder = NULL;
	// gradually increment buffer size until we can open the decoder
	for (;;)
		{
		TRAPD(error, theImageDecoder = CImageDecoder::FileNewL(iFs, destFileName, 
								CImageDecoder::EOptionNone, KNullUid, KNullUid, iDecoderUid));//, 
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			{
			Printf(_L("Cannot create image decoder for file %S"), &destFileName);
			
			if(KErrAccessDenied == error)
				Printf(_L("Access denied"));

			User::Leave(error);
			}
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(theFile.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(destFile.Write(buffer));
		destFileLength += extra;
		}
	CleanupStack::PushL(theImageDecoder);
	TBuf8<KTempBufferSize> mimeType;
	
	Printf(_L("Call function GetMimeTypeFileL\n"));
	theImageDecoder->GetMimeTypeFileL(iFs, sourceFileName, mimeType);
	if(mimeType ==  KJpegMimeType)
    	{
    	Printf(_L("GetMimeTypeFileL() returned correct MIME type\n"));
    	}
	
	Printf(_L("Call function FrameInfoStringsL\n"));
	CFrameInfoStrings*	theStr = theImageDecoder->FrameInfoStringsL(0);

	TInt index;
	for(index = 0; index < theStr->Count(); index++)
		{
		const TPtrC str = theStr->String(index);
		Printf(_L("String # %d : %S"),index, &str);
		}
	delete 	theStr;
	theStr = NULL;
	
	Printf(_L("Call function FrameInfoStringsLC"));
	CFrameInfoStrings*	theStr1 = theImageDecoder->FrameInfoStringsLC(0);
	for(index = 0; index < theStr1->Count(); index++)
		{
		const TPtrC str = theStr1->String(index);
		Printf(_L("String # %d : %S"),index, &str);
		}
	
	CleanupStack::Pop();
	delete 	theStr1;
	
	Printf(_L("Call function ImageType"));
	TInt theFrameNumber = 0;
	TUid theImageType;
	TUid theImageSubType;

	theImageDecoder->ImageType(theFrameNumber, theImageType, theImageSubType);
	Printf(_L("Image type Uid: %d (expected %d)"),theImageType, KImageTypeTestUid);
						
	// gradually increment buffer size until we get info about the given frame
	while (theImageDecoder->IsImageHeaderProcessingComplete() == (TBool)EFalse && 
		wantedBufferSize<fileSize)
		{
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(theFile.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(destFile.Write(buffer));
		destFileLength += extra;

		theImageDecoder->ContinueProcessingHeaderL();
		}

	// resize the bitmap - since we now know the size
	TFrameInfo frameInfo(theImageDecoder->FrameInfo());
	TSize frameSize(frameInfo.iFrameCoordsInPixels.Width(),
			frameInfo.iFrameCoordsInPixels.Height());
	
	CFbsBitmap*	theBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL( theBitmap );
	User::LeaveIfError(theBitmap->Create(frameSize, frameInfo.iFrameDisplayMode)); 

	// the conversion itself
	TBool first = ETrue;
	for(;;)
		{
		iActiveListener->InitialiseActiveListener();
		if (first)
			theImageDecoder->Convert(&iActiveListener->iStatus,*theBitmap,aFrameNo);
		else
			theImageDecoder->ContinueConvert(&iActiveListener->iStatus);

		first = EFalse;

		CActiveScheduler::Start();
		TInt error = iActiveListener->iStatus.Int();
		if (error==KErrNone)
			break;
		
		if(error == KErrCancel)
			{
			//The incremental load was canceled at the client before completion
			theRes = KErrCancel;
			break;
			}
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			User::Leave(error);
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		
		ASSERT(extra>0);
		
		if(theFile.SubSessionHandle())
			User::LeaveIfError(theFile.Read(buffer, extra));
		
		ASSERT(buffer.Length()==extra); // should always work on emulator
		
		if(destFile.SubSessionHandle())
			{
			User::LeaveIfError(destFile.Write(buffer));
			destFileLength += extra;
			}
		}
	
	theBitmap->Reset();
	CleanupStack::PopAndDestroy(5); // theImageDecoder, destFile, buffer, file, theBitmap, 

	iFs.Close();
	
	return;
	}
	
void TImage::ImageFrameIncrL(TImageFormat aFormat, const TDesC& aTestFile)	
	{
	Printf(_L("--- Image2 test(%d) --- %S"), aFormat, &aTestFile);
	TInt aFrameNo = 0;
	TInt	theRes;
	TFileName	sourceFileName;
	sourceFileName = KJpegFilePath;
	sourceFileName.Append(KImageTestJfif420);
	RFile theFile;
	
	theRes = theFile.Open(iFs, sourceFileName, EFileShareReadersOnly|EFileStream|EFileRead);
	
	if(theRes != KErrNone)
		{
		iFs.Close();
		Printf(_L("Cannot open file : %S"), &sourceFileName);

		return;
		}
	else
		{
		Printf(_L("File %S was opened successfully"), &sourceFileName);
		}
	
	CleanupClosePushL(theFile);
	TInt fileSize = 0;
	User::LeaveIfError(theFile.Size(fileSize));

	TPtr8 buffer(STATIC_CAST(TUint8*, User::AllocLC(1024)), 0, 1024);

	RFile destFile;

	TFileName	destFileName;
	destFileName = KJpegFileOutPath;
	destFileName.Append(KJpegFileName);
	TInt  destFileLength = 0;

	theRes = destFile.Create(iFs, destFileName, EFileShareAny|EFileStream|EFileWrite);
	if(theRes == KErrAlreadyExists)
		User::LeaveIfError(theRes = destFile.Replace(iFs, destFileName, EFileShareAny|EFileStream|EFileWrite));
	else if(theRes)
		{
		Printf(_L("Cannot create destination file %S"), &destFileName);
		User::LeaveIfError(theRes);
		}
	
	CleanupClosePushL(destFile);

	Printf(_L("Destination file %S was created successfully"), &destFileName);

	TInt wantedBufferSize=1;

	User::LeaveIfError(theFile.Read(buffer, wantedBufferSize));
	ASSERT(buffer.Length()==wantedBufferSize); // should always work on emulator
	User::LeaveIfError(destFile.Write(buffer));
	destFileLength += wantedBufferSize;
	CJPEGImageFrameDecoder *theframeDecoder = NULL;
	// gradually increment buffer size until we can open the decoder
	for (;;)
		{
		TRAPD(error, theframeDecoder = static_cast<CJPEGImageFrameDecoder*>(CImageDecoder::FileNewL(iFs, destFileName, CImageDecoder::EOptionNone, KNullUid, KNullUid, iDecoderUid)));
		if (error==KErrNone)
			break;
		if (error!=KErrUnderflow || wantedBufferSize>=fileSize)
			{
			Printf(_L("Cannot create image decoder for file %S"), &destFileName);
			
			if(KErrAccessDenied == error)
				Printf(_L("Access denied"));

			User::Leave(error);
			}
		TInt increment = StreamIncrement(wantedBufferSize); 
		wantedBufferSize = Min(fileSize, wantedBufferSize+increment);
		TInt extra = wantedBufferSize - destFileLength;
		ASSERT(extra>0);
		User::LeaveIfError(theFile.Read(buffer, extra));
		ASSERT(buffer.Length()==extra); // should always work on emulator
		User::LeaveIfError(destFile.Write(buffer));
		destFileLength += extra;
		}
			
	CleanupStack::PushL(theframeDecoder);

	// create destination RChunk
	RChunk chunk;
	TInt imageSizeInBytes;
	TBool supported = EFalse;	
	if(iStepName.Compare(_L("MM-ICL-COD-U-029B-HP")) == 0)
		{
		supported = theframeDecoder->RecommendedBufferSize(imageSizeInBytes);		
		}
	else if(iStepName.Compare(_L("MM-ICL-COD-U-029C-HP")) == 0)
		{
		supported = theframeDecoder->RecommendedBufferSize(KUidFormatYUV420Planar , imageSizeInBytes);		
		}	
	
	if (supported == EFalse)
		{
		Printf(_L("Codec does not support this image format"));
		TestFailed(KErrNotSupported);
		CleanupStack::PopAndDestroy(4, &theFile); // decoder, tempFile, buffer and file
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
			CleanupStack::PopAndDestroy(5, &theFile); // chunk, decoder, tempFile, buffer and file
			}
		else
			{
			CleanupStack::PushL(theImageFrame);


			TBool first = ETrue;
			for(;;)
				{
				iActiveListener->InitialiseActiveListener();
				if (first)
				    {
					theframeDecoder->ConvertFrame(&iActiveListener->iStatus, *theImageFrame,aFrameNo);
				    first = EFalse;
				    }

				else
				    {
					theframeDecoder->ContinueConvertFrame(&iActiveListener->iStatus);
				    }

				CActiveScheduler::Start();
				const TInt error = iActiveListener->iStatus.Int();
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
				TInt extra = wantedBufferSize - destFileLength;
				ASSERT(extra>0);
				User::LeaveIfError(theFile.Read(buffer, extra));
				ASSERT(buffer.Length()==extra); // should always work on emulator
				User::LeaveIfError(destFile.Write(buffer));
				destFileLength += extra;
				}
			}
		}
	CleanupStack::PopAndDestroy(6); // theFrameDecoder, destFile, buffer, theFile, theImageFrame, chunk
	
	iFs.Close();
	
	return;
	}
