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

#ifndef __TMDATEST_H__
#define __TMDATEST_H__

#ifndef __MDA_CLIENT_TEST_H__
#include <mda/client/test.h>
#endif
#ifndef __MDA_CLIENT_RESOURCE_H__
#include <mda/client/resource.h>
#endif
#ifndef __MDA_CLIENT_VIDEO_H__
#include <mda/client/video.h>
#endif
#ifndef __TMDACORE_H__
#include "tmdacore.h"
#endif
#ifndef __MDAIMAGECONVERTER_H__
#include <mdaimageconverter.h>
#endif

class CMdaFailVidTest1 : public CMdaCoreTest
	{
	enum TStreamMethod {EStreamViaDes, EStreamViaFile};
private:
	virtual void DoTestL();
	void Test1L(CDir& aDir);
	void LoadImageL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor);
	void LoadImageWithAllocFailureL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor);

	void Test2L(CDir& aDir);
	void StreamImageL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor, TStreamMethod aStreamMethod);
	void StreamImageWithAllocFailureL(CFbsBitmap& aBitmap,const TDesC& aFileName,TInt aZoomFactor, TStreamMethod aStreamMethod);

	void Test3L();
	void SaveImageL(CFbsBitmap& aBitmap,TMdaClipFormat* aFormat,TMdaPackage* aCodec);
	void SaveImageWithAllocFailureL(CFbsBitmap& aBitmap,TMdaClipFormat* aFormat,TMdaPackage* aCodec);
	void ConfigDestPortL(TMdaClipFormat*& aFormat,TMdaPackage*& aCodec,TInt aFormatIndex);
	const TDesC& SaveFileName(TInt aFormatIndex);
private:
	RFs iFs;
	TMdaMbmClipFormat iMbmFormat;
	TMdaJfifClipFormat iJfifFormat;
	TMdaBmpClipFormat iBmpFormat;
	TMda1BppBmpCodec iBmp1BppCodec;
	TMda4BppBmpCodec iBmp4BppCodec;
	TMda8BppBmpCodec iBmp8BppCodec;
	TMda24BppBmpCodec iBmp24BppCodec;
	};

_LIT(KVideoTestSourceFileName,"tmdavid.mbm");
_LIT(KFailVideoTempTestFileName,"c:\\tmdafailvid");

_LIT(KVideoTest1BppMonoMbm,"1 bpp mbm");
_LIT(KVideoTest2BppMonoMbm,"2 bpp mbm");
_LIT(KVideoTest4BppMonoMbm,"4 bpp mono mbm");
_LIT(KVideoTest8BppMonoMbm,"8 bpp mono mbm");
_LIT(KVideoTest4BppColorMbm,"4 bpp color mbm");
_LIT(KVideoTest8BppColorMbm,"8 bpp color mbm");
_LIT(KVideoTest12BppColorMbm,"12 bpp mbm");
_LIT(KVideoTest16BppColorMbm,"16 bpp mbm");
_LIT(KVideoTest24BppColorMbm,"24 bpp mbm");

_LIT(KVideoTestMono10Jfif,"mono jpg 10");
_LIT(KVideoTestMono30Jfif,"mono jpg 30");
_LIT(KVideoTestMono60Jfif,"mono jpg60 ");
_LIT(KVideoTestMono100Jfif,"mono jpg 100");
_LIT(KVideoTest420C10Jfif,"420 jpg 10");
_LIT(KVideoTest420C30Jfif,"420 jpg 30");
_LIT(KVideoTest420C60Jfif,"420 jpg 60");
_LIT(KVideoTest420C100Jfif,"420 jpg 100");
_LIT(KVideoTest422C10Jfif,"422 jpg 10");
_LIT(KVideoTest422C30Jfif,"422 jpg 30");
_LIT(KVideoTest422C60Jfif,"422 jpg 60");
_LIT(KVideoTest422C100Jfif,"422 jpg 100");
_LIT(KVideoTest444C10Jfif,"444 jpg 10");
_LIT(KVideoTest444C30Jfif,"444 jpg 30");
_LIT(KVideoTest444C60Jfif,"444 jpg 60");
_LIT(KVideoTest444C100Jfif,"444 jpg 100");

_LIT(KVideoTest1BppBmp,"1 bpp bmp");
_LIT(KVideoTest4BppBmp,"4 bpp bmp");
_LIT(KVideoTest8BppBmp,"8 bpp bmp");
_LIT(KVideoTest24BppBmp,"24 bpp bmp");

#endif

