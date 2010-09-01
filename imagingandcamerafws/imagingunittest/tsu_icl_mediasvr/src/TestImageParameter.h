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
// This file contains the TTestImage class which is used to store the name of images and format
// parameters for tests
// 
//

#ifndef __TESTIMAGEPARAMETER_H__
#define __TESTIMAGEPARAMETER_H__

#include <mdaimageconverter.h>


// literals for test images used
_LIT(KMediaSvrTest1BppMonoMbm,"mbm1bpp.mbm");
_LIT(KMediaSvrTest2BppMonoMbm,"mbm2bpp.mbm");
_LIT(KMediaSvrTest4BppMonoMbm,"mbm4bppm.mbm");
_LIT(KMediaSvrTest8BppMonoMbm,"mbm8bppm.mbm");
_LIT(KMediaSvrTest4BppColorMbm,"mbm4bppc.mbm");
_LIT(KMediaSvrTest8BppColorMbm,"mbm8bppc.mbm");
_LIT(KMediaSvrTest12BppColorMbm,"mbm12bpp.mbm");
_LIT(KMediaSvrTest16BppColorMbm,"mbm16bpp.mbm");
_LIT(KMediaSvrTest24BppColorMbm,"mbm24bpp.mbm");

_LIT(KMediaSvrTestJfifMono,"jfifMono.jpg");
_LIT(KMediaSvrTestJfif420,"jfif420.jpg");
_LIT(KMediaSvrTestJfif422,"jfif422.jpg");
_LIT(KMediaSvrTestJfif444,"jfif444.jpg");
_LIT(KMediaSvrTestJfif10,"jfif10.jpg");
_LIT(KMediaSvrTestJfif30,"jfif30.jpg");
_LIT(KMediaSvrTestJfif60,"jfif60.jpg");
_LIT(KMediaSvrTestJfif100,"jfif100.jpg");

_LIT(KMediaSvrTest1BppBmp,"bmp1bpp.bmp");
_LIT(KMediaSvrTest4BppBmp,"bmp4bpp.bmp");
_LIT(KMediaSvrTest8BppBmp,"bmp8bpp.bmp");
_LIT(KMediaSvrTest24BppBmp,"bmp24bpp.bmp");

_LIT(KMediaSvrTestPng1,"png1.png");
_LIT(KMediaSvrTestPng2,"png2.png");
_LIT(KMediaSvrTestPng3,"png3.png");
_LIT(KMediaSvrTestPng4,"png4.png");

_LIT(KMediaSvrTestTif1,"tif1.tif");
_LIT(KMediaSvrTestTif2,"tif2.tif");
_LIT(KMediaSvrTestTif3,"tif3.tif");


_LIT(KMediaSvrTestGif1,"trans.gif");
_LIT(KMediaSvrTestTransparentGif,"trans.gif");

_LIT(KMediaSvrTestTxt, "test.txt");

_LIT(KMediaSvrTestGifRef,"ref.gif");
_LIT(KMediaSvrTestPngRef,"ref.png");

_LIT(KMediaSvrGifCommentFile,"GifComment.gif");
_LIT(KMediaSvrGifComment,"Header Comment 1 Block 0 Block 1Header Comment 2 Block 0 Block 1Footer Comment 1 Block 0 Block 1Footer Comment 2 Block 0 Block 1");

_LIT(KMediaSvrInvalidTempFile,"h:\temptest.bmp");
_LIT(KMediaSvrBmpTempFile,"temptest.bmp");
_LIT(KMediaSvrJfifTempFile,"temptest.jpg");
_LIT(KMediaSvrMbmTempFile,"temptest.mbm");
_LIT(KMediaSvrPngTempFile,"temptest.png");
_LIT(KMediaSvrGifTempFile,"temptest.gif");

/** 
 *
 * class to store image parameters for tests
 *
 */
class TTestImage 
	{
public:
	TTestImage();
	TTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec);
	TTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec, const TDesC& aRefImage);

	TTestImage(const TTestImage& aTestImage);
	void SetTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec);
	void SetTestImage(const TDesC& aSourceFile,const TMdaClipFormat* aFormat,const TMdaPackage* aCodec, const TDesC& aRefImage);

	inline TDesC& SourceFile(); 
	inline TDesC& RefFile();
	inline TMdaClipFormat* Format();
	inline TMdaPackage* Codec();

private:
	TDesC* iSourceFile;
	TDesC* iRefFile;
	TMdaClipFormat* iFormat;
	TMdaPackage* iCodec;

	};

inline TDesC& TTestImage::SourceFile() { return *iSourceFile;}
inline TDesC& TTestImage::RefFile() { return *iRefFile;}
inline TMdaClipFormat* TTestImage::Format() { return iFormat; }
inline TMdaPackage* TTestImage::Codec() { return iCodec;}


#endif // __TESTIMAGEPARAMETER_H__
