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

#include "tmdatest.h"

/*

Video Test 5 - Client Utilities
===============================

1. Bitmap -> file then file -> bitmap (including checking cancelling)
2. Cached file -> bitmap
3. Bitmap -> descriptor then descriptor -> bitmap
4. Cached descriptor -> bitmap
5. *** Bitmap -> bitmap (we no longer do this)
6. Anonymous file -> bitmap using explicit format
7. File -> bitmap with mask for transparent gif 
8. Rotation
9. Scaling with results compared to \tmdavid\ref\scaledN.mbm where N = 0,1,...

*/

void CMdaVideo5Test::DoTestL()
	{
	iReferenceScaledBitmap = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(iReferenceScaledBitmap);

	TestUtilitiesL();
	TestRotationL(); 
	TestScalingL();

	CleanupStack::PopAndDestroy(); // iReferenceScaledBitmap
	}

void CMdaVideo5Test::TestUtilitiesL()
	{
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Load(iEngine->iSourceFileName));

	CMdaServer* server = NULL; // jf 19/11/01. real objects no longer supported. CMdaServer::NewL();
	CleanupStack::PushL(server);

	FileL(*bmp,server); 
	FileCancelL(*bmp,server); 
	FileBmpL(*bmp,server); 
//	FileClippedL(*bmp,server); // don't call anymore (we don't support clipped save)
	FileCacheL(*bmp,server); 
	FileBmpCacheL(*bmp,server);
	DescL(*bmp,server); 
	DescCacheL(*bmp,server); 
	BitmapL(*bmp);
	AnonymousFileL(server); 
	TransparentL(server); 

	CleanupStack::PopAndDestroy(2); // server, bmp
	}

void CMdaVideo5Test::FileL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("File utility"));
	TMdaMbmClipFormat format;
	format.iDisplayMode = EColor16M;

	CMdaImageBitmapToFileUtility* btof = CMdaImageBitmapToFileUtility::NewL(*this,aServer);
	CleanupStack::PushL(btof);
	btof->CreateL(KVideoTestTempFileName,&format,NULL,NULL);
	WaitAndCheckL();
	btof->ConvertL(aBitmap);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // btof

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);
	ftob->OpenL(KVideoTestTempFileName);
	WaitAndCheckL();
	ftob->ConvertL(*bmp,0);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // ftob

	iEngine->iFs.Delete(KVideoTestTempFileName);

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(); // bmp
	}

void CMdaVideo5Test::FileCancelL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("File utility cancelling"));
	TMdaMbmClipFormat format;
	format.iDisplayMode = EColor16M;

	CMdaImageBitmapToFileUtility* btof = CMdaImageBitmapToFileUtility::NewL(*this,aServer);
	CleanupStack::PushL(btof);
	btof->CreateL(KVideoTestTempFileName,&format,NULL,NULL);
	WaitAndCheckL();
	btof->ConvertL(aBitmap); // check we can cancel before carrying on
	btof->CancelConvertL();
	btof->ConvertL(aBitmap);
	btof->CancelConvertL();
	btof->ConvertL(aBitmap);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // btof

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);
	ftob->OpenL(KVideoTestTempFileName);
	WaitAndCheckL();
	ftob->ConvertL(*bmp,0); // check we can cancel before carrying on
	ftob->CancelConvertL();
	ftob->ConvertL(*bmp,0);
	ftob->CancelConvertL();
	ftob->ConvertL(*bmp,0);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // ftob

	iEngine->iFs.Delete(KVideoTestTempFileName);

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(); // bmp
	}

void CMdaVideo5Test::FileBmpL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("File utility Bmp"));
	TMdaBmpClipFormat format;
	TMda24BppBmpCodec codec;

	CMdaImageBitmapToFileUtility* btof = CMdaImageBitmapToFileUtility::NewL(*this,aServer);
	CleanupStack::PushL(btof);
	btof->CreateL(KVideoTestTempBmpFileName,&format,&codec,NULL);
	WaitAndCheckL();
	btof->ConvertL(aBitmap);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // btof

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this);
	CleanupStack::PushL(ftob);
	ftob->OpenL(KVideoTestTempBmpFileName);
	WaitAndCheckL();
	ftob->ConvertL(*bmp,0);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // ftob

	iEngine->iFs.Delete(KVideoTestTempBmpFileName);

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(); // bmp
	}

TBool MagentaCheck(CFbsBitmap& aBitmap)
	{
	TSize size(aBitmap.SizeInPixels());
	TInt pitch = aBitmap.ScanLineLength(size.iWidth,EColor16M);
	TUint8* ptr = (TUint8*)aBitmap.DataAddress();
	TInt ptrSkip = pitch-size.iWidth*3;
	TBool flag = ETrue;

	for(TInt y=0; y<size.iHeight; y++)
		{
		for(TInt x=0; x<size.iWidth; x++)
			{
			if(ptr[0]==255 && ptr[1]==0 && ptr[2]==255)
				flag = EFalse;
			ptr[0] = 255;
			ptr[1] = 0;
			ptr[2] = 255;
			ptr += 3;
			}
		ptr += ptrSkip;
		}

	return flag;
	}

void CMdaVideo5Test::FileClippedL(CFbsBitmap& /*aBitmap*/,CMdaServer* /*aServer*/)
	{
	ASSERT(EFalse); // should not get here anymore
	}

void CMdaVideo5Test::FileCacheL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("File utility (cached)"));

	RFile srcFile;
	User::LeaveIfError(srcFile.Open(iEngine->iFs,KVideoTestSourceFileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(srcFile);

	TFileName tempFileName;
	RFile tempFile;
	User::LeaveIfError(tempFile.Temp(iEngine->iFs,_L("c:\\"),tempFileName,EFileShareAny|EFileStream|EFileWrite));
	CleanupClosePushL(tempFile);

	TBuf8<128> buffer;
	buffer.SetLength(128);

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this,aServer);
	CleanupStack::PushL(ftob);

	User::LeaveIfError(srcFile.Read(buffer));
	User::LeaveIfError(tempFile.Write(buffer));

	ftob->OpenL(tempFileName);
	WaitL();

	if (iError != KErrNone)
		iEngine->TestFailed(iError);
		
	do	{
		User::LeaveIfError(srcFile.Read(buffer));
		User::LeaveIfError(tempFile.Write(buffer));
		ftob->ConvertL(*bmp,0);
		WaitL();
		}
	while (iError == KErrUnderflow);

	if (iError != KErrNone)
		iEngine->TestFailed(iError);

	CleanupStack::PopAndDestroy(); // ftob

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(3); // bmp,tempFile,srcFile
	iEngine->iFs.Delete(tempFileName);
	}

void CMdaVideo5Test::FileBmpCacheL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("File utility Bmp (cached)"));

	// NB assumes that FileBmpL has passed OK and we can save OK to BMP file

	// 1st save passed bitmap to .bmp file
	TMdaBmpClipFormat format;
	TMda24BppBmpCodec codec;
	CMdaImageBitmapToFileUtility* btof = CMdaImageBitmapToFileUtility::NewL(*this,aServer);
	CleanupStack::PushL(btof);
	btof->CreateL(KVideoTestTempBmpFileName,&format,&codec,NULL);
	WaitL();
	btof->ConvertL(aBitmap);
	WaitL();
	CleanupStack::PopAndDestroy(); // btof

	RFile srcFile;
	User::LeaveIfError(srcFile.Open(iEngine->iFs,KVideoTestTempBmpFileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(srcFile);

	TFileName tempFileName;
	RFile tempFile;
	User::LeaveIfError(tempFile.Temp(iEngine->iFs,_L("c:\\"),tempFileName,EFileShareAny|EFileStream|EFileWrite));
	CleanupClosePushL(tempFile);

	TBuf8<128> buffer;
	buffer.SetLength(128);

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this,aServer);
	CleanupStack::PushL(ftob);

	User::LeaveIfError(srcFile.Read(buffer));
	User::LeaveIfError(tempFile.Write(buffer));

	ftob->OpenL(tempFileName);
	WaitL();

	if (iError != KErrNone)
		iEngine->TestFailed(iError);
		
	do	{
		User::LeaveIfError(srcFile.Read(buffer));
		User::LeaveIfError(tempFile.Write(buffer));
		ftob->ConvertL(*bmp,0);
		WaitL();
		}
	while (iError == KErrUnderflow);

	if (iError != KErrNone)
		iEngine->TestFailed(iError);

	CleanupStack::PopAndDestroy(); // ftob

	iEngine->iFs.Delete(KVideoTestTempBmpFileName);

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(3); // bmp,tempFile,srcFile
	iEngine->iFs.Delete(tempFileName);
	}

void CMdaVideo5Test::DescL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("Desc utility"));
	const TSize size = aBitmap.SizeInPixels();
	const TInt descSize = (size.iWidth * size.iHeight * 3) + 1024;
	HBufC8* descBuf = HBufC8::NewMaxLC(descSize);
	TPtr8 descDes(descBuf->Des());

	TMdaBmpClipFormat format;
	TMda24BppBmpCodec codec;

	CMdaImageBitmapToDescUtility* btod = CMdaImageBitmapToDescUtility::NewL(*this);
	CleanupStack::PushL(btod);
	btod->CreateL(descDes,&format,&codec,NULL);
	WaitAndCheckL();
	btod->ConvertL(aBitmap);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // btod

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageDescToBitmapUtility* dtob = CMdaImageDescToBitmapUtility::NewL(*this,aServer);
	CleanupStack::PushL(dtob);
	dtob->OpenL(*descBuf);
	WaitAndCheckL();
	dtob->ConvertL(*bmp,0);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // dtob

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(2); // bmp, descBuf
	}

void CMdaVideo5Test::DescCacheL(CFbsBitmap& aBitmap,CMdaServer* aServer)
	{
	iEngine->Print(_L("Desc utility (cached)"));

	RFile srcFile;
	User::LeaveIfError(srcFile.Open(iEngine->iFs,KVideoTestSourceFileName,EFileShareAny|EFileStream|EFileRead));
	CleanupClosePushL(srcFile);

	TInt size = 0;
	User::LeaveIfError(srcFile.Size(size));
	HBufC8* srcBuffer = HBufC8::NewLC(size);
	TPtr8 srcDes(srcBuffer->Des());
	User::LeaveIfError(srcFile.Read(srcDes));
	srcDes.SetLength(128);

	CleanupStack::Pop(); // srcBuffer
	CleanupStack::PopAndDestroy(); // srcFile
	CleanupStack::PushL(srcBuffer);

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageDescToBitmapUtility* dtob = CMdaImageDescToBitmapUtility::NewL(*this,aServer);
	CleanupStack::PushL(dtob);

	dtob->OpenL(srcDes);
	WaitL();

	if (iError != KErrNone)
		iEngine->TestFailed(iError);
		
	do	{
		srcDes.SetLength(Min(srcDes.Length() + 128,size));
		dtob->ConvertL(*bmp,0);
		WaitL();
		}
	while (iError == KErrUnderflow && srcDes.Length() < size);

	if (iError != KErrNone)
		iEngine->TestFailed(iError);

	CleanupStack::PopAndDestroy(); // dtob

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(2); // bmp,srcBuffer
	}

void CMdaVideo5Test::BitmapL(CFbsBitmap& aBitmap)
	{
	iEngine->Print(_L("Bitmap utility"));
	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(aBitmap.SizeInPixels(),EColor16M));

	CMdaImageBitmapToBitmapUtility* btob = CMdaImageBitmapToBitmapUtility::NewL(*this);
	CleanupStack::PushL(btob);
	btob->OpenL(aBitmap);
	WaitAndCheckL();

	btob->ConvertL(*bmp,0);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // btob

	CheckBitmaps(aBitmap,*bmp);

	CleanupStack::PopAndDestroy(); // bmp
	}

void CMdaVideo5Test::AnonymousFileL(CMdaServer* aServer)
	{
	iEngine->Print(_L("File utility with specified format"));

	CFbsBitmap* source = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(source);

	TFileName fileName;
	iEngine->iFs.DefaultPath(fileName);
	fileName.Append(KVideoTestOtaSourceFileName);

	User::LeaveIfError(source->Load(fileName));

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Create(source->SizeInPixels(),EGray2));

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this,aServer);
	CleanupStack::PushL(ftob);

	iEngine->iFs.DefaultPath(fileName);
	fileName.Append(KVideoTestOtaFileName);
	
	ftob->OpenL(fileName,&iOtaClip);
	WaitAndCheckL();
	ftob->ConvertL(*bmp,0);
	WaitAndCheckL();

	CheckBitmaps(*source,*bmp);

	iEngine->iFs.DefaultPath(fileName);
	fileName.Append(KVideoTestWbmpSourceFileName);

	User::LeaveIfError(source->Load(fileName));
	User::LeaveIfError(bmp->Resize(source->SizeInPixels()));

	iEngine->iFs.DefaultPath(fileName);
	fileName.Append(KVideoTestWbmpFileName);

	ftob->OpenL(fileName,&iWbmpClip);
	WaitAndCheckL();
	ftob->ConvertL(*bmp,0);
	WaitAndCheckL();
	CleanupStack::PopAndDestroy(); // ftob

	CheckBitmaps(*source,*bmp);

	CleanupStack::PopAndDestroy(2); // bmp, source
	}

void CMdaVideo5Test::TransparentL(CMdaServer* aServer)
	{
	iEngine->Print(_L("Mask testing"));

	CFbsBitmap* bmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bmp);
	CFbsBitmap* mask = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(mask);

	CMdaImageFileToBitmapUtility* ftob = CMdaImageFileToBitmapUtility::NewL(*this,aServer);
	CleanupStack::PushL(ftob);
	ftob->OpenL(KVideoTestMaskFileName);
	WaitAndCheckL();

	TFrameInfo info;
	ftob->FrameInfo(0,info);
	User::LeaveIfError(bmp->Create(info.iFrameCoordsInPixels.Size(),EGray2));
	User::LeaveIfError(mask->Create(info.iFrameCoordsInPixels.Size(),EGray2));

	ftob->ConvertL(*bmp,*mask,0);
	WaitAndCheckL();

	CheckBitmaps(*bmp,*mask);

	CleanupStack::PopAndDestroy(3); // ftob,bmp,mask
	}

void CMdaVideo5Test::TestRotationL()
	{
	iEngine->Print(_L("Rotation utility"));

	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Load(iEngine->iSourceFileName));

	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor16M));

	CMdaBitmapRotator* rotator = CMdaBitmapRotator::NewL();
	CleanupStack::PushL(rotator);

    rotator->RotateL(*this,*srcBmp,*tgtBmp,CMdaBitmapRotator::TRotationAngle(0));
	WaitAndCheckL();
	CheckBitmaps(*srcBmp,*tgtBmp,1);
	rotator->RotateL(*this,*tgtBmp,CMdaBitmapRotator::TRotationAngle(2));
	WaitAndCheckL();
	CheckBitmaps(*srcBmp,*tgtBmp,0);

    rotator->RotateL(*this,*srcBmp,*tgtBmp,CMdaBitmapRotator::TRotationAngle(1));
	WaitAndCheckL();
	CheckBitmaps(*srcBmp,*tgtBmp,2);
	rotator->RotateL(*this,*tgtBmp,CMdaBitmapRotator::TRotationAngle(1));
	WaitAndCheckL();
	CheckBitmaps(*srcBmp,*tgtBmp,0);

    rotator->RotateL(*this,*srcBmp,*tgtBmp,CMdaBitmapRotator::TRotationAngle(2));
	WaitAndCheckL();
	CheckBitmaps(*srcBmp,*tgtBmp,3);
	rotator->RotateL(*this,*tgtBmp,CMdaBitmapRotator::TRotationAngle(0));
	WaitAndCheckL();
	CheckBitmaps(*srcBmp,*tgtBmp,0);

    rotator->RotateL(*this,*srcBmp,*tgtBmp,CMdaBitmapRotator::TRotationAngle(2));
	rotator->CancelRotation();
	rotator->RotateL(*this,*tgtBmp,CMdaBitmapRotator::TRotationAngle(0));
	rotator->CancelRotation();

	CleanupStack::PopAndDestroy(3); // rotator, tgtBmp, srcBmp
	}

void CMdaVideo5Test::TestScalingL()
	{
	iEngine->Print(_L("Scaling utility"));

	CFbsBitmap* srcBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(srcBmp);
	User::LeaveIfError(srcBmp->Load(iEngine->iSourceFileName));
	const TSize size(srcBmp->SizeInPixels());

	CFbsBitmap* tgtBmp = new(ELeave) CFbsBitmap;
	CleanupStack::PushL(tgtBmp);
	User::LeaveIfError(tgtBmp->Create(TSize(0,0),EColor4K));

	CMdaBitmapScaler* scaler = CMdaBitmapScaler::NewL();
	CleanupStack::PushL(scaler);

	tgtBmp->Resize(TSize(size.iWidth / 2,size.iHeight / 2));
	scaler->ScaleL(*this,*srcBmp,*tgtBmp);
	WaitAndCheckL();
	CheckBitmapL(*tgtBmp);

	scaler->ScaleL(*this,*tgtBmp,TSize(size.iWidth * 2,size.iHeight * 2));
	WaitAndCheckL();
	CheckBitmapL(*tgtBmp);

	tgtBmp->Resize(TSize(size.iWidth,size.iHeight / 2));
	scaler->ScaleL(*this,*srcBmp,*tgtBmp);
	WaitAndCheckL();
	CheckBitmapL(*tgtBmp);

	scaler->ScaleL(*this,*tgtBmp,TSize(size.iWidth,size.iHeight / 2),EFalse);
	WaitAndCheckL();
	CheckBitmapL(*tgtBmp);

	tgtBmp->Resize(TSize(size.iWidth / 2,size.iHeight));
	scaler->ScaleL(*this,*srcBmp,*tgtBmp);
	WaitAndCheckL();
	CheckBitmapL(*tgtBmp);

	scaler->ScaleL(*this,*tgtBmp,TSize(size.iWidth / 2,size.iHeight),EFalse);
	WaitAndCheckL();
	CheckBitmapL(*tgtBmp);

	scaler->ScaleL(*this,*srcBmp,*tgtBmp);
	scaler->CancelScaling();
    scaler->ScaleL(*this,*tgtBmp,srcBmp->SizeInPixels());
	scaler->CancelScaling();
	scaler->ScaleL(*this,*srcBmp,*tgtBmp,EFalse);
	scaler->CancelScaling();
    scaler->ScaleL(*this,*tgtBmp,srcBmp->SizeInPixels(),EFalse);
	scaler->CancelScaling();

	CleanupStack::PopAndDestroy(3); // scaler, tgtBmp, srcBmp
	}

void CMdaVideo5Test::MiuoCreateComplete(TInt aError)
	{
	Signal(aError,_L("Create complete"));
	}

void CMdaVideo5Test::MiuoOpenComplete(TInt aError)
	{
	Signal(aError,_L("Open complete"));
	}

void CMdaVideo5Test::MiuoConvertComplete(TInt aError)
	{
	Signal(aError,_L("Convert complete"));
	}

void CMdaVideo5Test::WaitL()
	{
	CActiveScheduler::Start();
	}

void CMdaVideo5Test::WaitAndCheckL()
	{
	CActiveScheduler::Start();
	if (iError != KErrNone)
		iEngine->TestFailed(iError);
	}

void CMdaVideo5Test::Signal(TInt aError,const TDesC& aMsg)
	{
	iError = aError;
	if (iError == KErrNone)
		iEngine->Print(aMsg);
	CActiveScheduler::Stop();
	}

void CMdaVideo5Test::CheckBitmapL(CFbsBitmap& aBitmap)
	{
	TFileName fileName;
	User::LeaveIfError(iEngine->iFs.DefaultPath(fileName));
	fileName.Append(KVideoTestSourceScaledFileName);
	fileName.AppendNum(iScaledIndex++);
	fileName.Append(KVideoTestRefExt);

	User::LeaveIfError(iReferenceScaledBitmap->Load(fileName));

	CheckBitmaps(aBitmap,*iReferenceScaledBitmap);

	iReferenceScaledBitmap->Reset();
	}
