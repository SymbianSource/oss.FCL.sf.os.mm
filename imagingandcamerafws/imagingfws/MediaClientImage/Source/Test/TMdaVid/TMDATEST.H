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
#include <MdaImageConverter.h>
#endif

class CMdaVideo5Test : public CMdaCoreTest, public MMdaImageUtilObserver
	{
private:
	virtual void DoTestL();
	virtual void MiuoCreateComplete(TInt aError);
	virtual void MiuoOpenComplete(TInt aError);
	virtual void MiuoConvertComplete(TInt aError);
	void TestUtilitiesL();
	void FileL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void FileCancelL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void FileBmpL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void FileClippedL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void FileCacheL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void FileBmpCacheL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void DescL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void DescCacheL(CFbsBitmap& aBitmap,CMdaServer* aServer);
	void BitmapL(CFbsBitmap& aBitmap);
	void AnonymousFileL(CMdaServer* aServer);
	void TransparentL(CMdaServer* aServer);
	void TestRotationL();
	void TestScalingL();
	void WaitL();
	void WaitAndCheckL();
	void Signal(TInt aError,const TDesC& aMsg);
	void CheckBitmapL(CFbsBitmap& aBitmap);
private:
	TInt iError;
	TInt iScaledIndex;
	TMdaOtaClipFormat iOtaClip;
	TMdaWbmpClipFormat iWbmpClip;
	CFbsBitmap* iReferenceScaledBitmap;
	};

_LIT(KVideoTestSourceFileName,"tmdavid.mbm");
_LIT(KVideoTestSourceScaledFileName,"ref\\scaled");

_LIT(KVideoTestRefDir,"ref\\");
_LIT(KVideoTestRefExt,".mbm");
_LIT(KVideoTestTempFileName,"c:\\tmdavidtemp.mbm");
_LIT(KVideoTestTempBmpFileName,"c:\\tmdavidtemp.bmp");

_LIT(KVideoTestOtaSourceFileName,"anon\\otabitmap.mbm");
_LIT(KVideoTestOtaFileName,"anon\\otabitmap");
_LIT(KVideoTestWbmpSourceFileName,"anon\\wirelessbitmap.mbm");
_LIT(KVideoTestWbmpFileName,"anon\\wirelessbitmap");
_LIT(KVideoTestMaskFileName,"trans.gif");

_LIT(KVideoTest1BppMonoMbm,"mbm1bpp.mbm");
_LIT(KVideoTest2BppMonoMbm,"mbm2bpp.mbm");
_LIT(KVideoTest4BppMonoMbm,"mbm4bppm.mbm");
_LIT(KVideoTest8BppMonoMbm,"mbm8bppm.mbm");
_LIT(KVideoTest4BppColorMbm,"mbm4bppc.mbm");
_LIT(KVideoTest8BppColorMbm,"mbm8bppc.mbm");
_LIT(KVideoTest12BppColorMbm,"mbm12bpp.mbm");
_LIT(KVideoTest16BppColorMbm,"mbm16bpp.mbm");
_LIT(KVideoTest24BppColorMbm,"mbm24bpp.mbm");

_LIT(KVideoTestJfifMono,"jfifMono.jpg");
_LIT(KVideoTestJfif420,"jfif420.jpg");
_LIT(KVideoTestJfif422,"jfif422.jpg");
_LIT(KVideoTestJfif444,"jfif444.jpg");
_LIT(KVideoTestJfif10,"jfif10.jpg");
_LIT(KVideoTestJfif30,"jfif30.jpg");
_LIT(KVideoTestJfif60,"jfif60.jpg");
_LIT(KVideoTestJfif100,"jfif100.jpg");

_LIT(KVideoTest1BppBmp,"bmp1bpp.bmp");
_LIT(KVideoTest4BppBmp,"bmp4bpp.bmp");
_LIT(KVideoTest8BppBmp,"bmp8bpp.bmp");
_LIT(KVideoTest24BppBmp,"bmp24bpp.bmp");

class CMdaServer;

#endif

