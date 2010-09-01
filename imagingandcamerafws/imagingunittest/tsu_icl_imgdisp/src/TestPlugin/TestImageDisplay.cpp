// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <bitstd.h>

#include <icl/imagedisplayplugin.h>
#include <icl/imagedisplaypaniccodes.h>
#include "TestImageDisplay.h"

const TInt KMaxFrameNumber=10;

GLDEF_C
void Panic(TInt aCode)
	{
	User::Panic(_L("CTestImageDisplayPlugin"), aCode);
	}

// Test imagedisplay plug-in.

CTestImageDisplayPlugin* CTestImageDisplayPlugin::NewL()
	{
	CTestImageDisplayPlugin* self = new(ELeave) CTestImageDisplayPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CTestImageDisplayPlugin::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	}

CTestImageDisplayPlugin::~CTestImageDisplayPlugin()
	{
	iFs.Close();
	delete iBitGc;
	delete iBitmapDevice;
	delete iFrame;
	delete iMask;
	}

CTestImageDisplayPlugin::CTestImageDisplayPlugin()
	{
	}

void CTestImageDisplayPlugin::OpenL()
	{
	ASSERT(NULL == iFrame);
	iImageSizes.Reset();
	iImageSizes.AppendL( TSize(100,100) );
	}

void CTestImageDisplayPlugin::CompleteInitL()
	{
	iFrame=new (ELeave) CFbsBitmap();	
	TDisplayMode Mode=(DisplayMode()==ENone? EColor16M: DisplayMode() );
	User::LeaveIfError( iFrame->Create(DestinationSizeInPixels(), Mode) );
	iFrame->SetSizeInTwips(TSize(iFrame->SizeInPixels().iWidth * KTwipsPerPoint, iFrame->SizeInPixels().iHeight * KTwipsPerPoint));

	iMask=new (ELeave) CFbsBitmap();
	User::LeaveIfError( iMask->Create(DestinationSizeInPixels(), EGray2) );

	iBitmapDevice = CFbsBitmapDevice::NewL(iFrame);
	iBitGc=CFbsBitGc::NewL();
	iBitGc->Activate(iBitmapDevice);
	iOutputRect.iBr = DestinationSizeInPixels().AsPoint() - TPoint(1,1);	
	}

void CTestImageDisplayPlugin::Play()
	{
	if (iFrame==NULL)
		{
		TRAPD(err, CompleteInitL());
		if (err!=KErrNone)
			{
			AsyncCallbackImageReady(NULL, EStatusNoMoreToDecode, iOutputRect, err);
			return;
			}
		}
		
	if (iFrameNumber < KMaxFrameNumber)
		{
		++iFrameNumber;
		DrawFrame();
		AsyncCallbackImageReady(iFrame, EStatusFrameReady | EStatusPaused, 
									iOutputRect, KErrNone);
		}
	else
		{
		AsyncCallbackImageReady(iFrame, EStatusNoMoreToDecode | EStatusFrameReady, 
									iOutputRect, KErrNone);
		iFrameNumber = 0;
		}
	}

void CTestImageDisplayPlugin::DrawFrame()
	{
	iBitGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	iBitGc->SetBrushColor(KRgbGray);
	iBitGc->Clear();
	iBitGc->SetPenColor(KRgbBlack);

	iBitGc->DrawRect(iOutputRect);
	const TInt KYStep=DestinationSizeInPixels().iHeight / KMaxFrameNumber;
	for (TInt i=iFrameNumber; i ; --i)
		{
		const TInt y=i*KYStep;
		iBitGc->DrawLine(TPoint(0, y), TPoint(iOutputRect.iBr.iX,y) );
		}
	}

void CTestImageDisplayPlugin::Pause()
	{
	}

void CTestImageDisplayPlugin::StopPlay()
	{
	iFrameNumber = 0;
	}

TInt CTestImageDisplayPlugin::ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr)
	{
	if (aIFaceUid.iUid == MTestHelper::KIFaceUidValue)
		{
		aIFacePtr = static_cast<MTestHelper*>(this);
		return KErrNone;
		}
	aIFacePtr = NULL;
	return KErrNotSupported;	
	}
	
void CTestImageDisplayPlugin::GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const
	{
	aBitmap = iFrame;
	aMask	= iMask;
	}
	
TBool CTestImageDisplayPlugin::ValidBitmap() const
	{
	return (iPluginStatus & CImageDisplayPlugin::EStatusBusy) == CImageDisplayPlugin::EStatusBusy;
	}

const CImageDisplay::RImageSizeArray& CTestImageDisplayPlugin::RecommendedImageSizes() const
	{
	return iImageSizes;
	}


TInt CTestImageDisplayPlugin::CurrentFrameNumber(TInt& aFrameParam)
	{
	aFrameParam = iFrameNumber;
	return (iState==EIdle? KErrUnknown : KErrNone);
	}

void CTestImageDisplayPlugin::TriggerPanic_4()
	{
	switch (SourceType())
		{
		case EImgSrcNotDefined:
		case EImgSrcFileName:
			SourceFileHandle();
			break;
		case EImgSrcFileHandle:
			SourceData();
			break;
		case EImgSrcDescriptor:
			SourceFileHandle();
			break;
		}
	}

const TUid CTestImageDisplayPlugin::Uid() const
	{
	ASSERT(this != NULL);
	return TUid::Uid(KIFaceUidValue);
	}

void CTestImageDisplayPlugin::Release()
	{
	ASSERT(this != NULL);
	}
