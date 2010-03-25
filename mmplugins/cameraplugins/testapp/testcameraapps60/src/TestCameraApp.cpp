// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Symbian Foundation License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <eikenv.h>
#include <TestCameraApp.rsg>
#include "TestCameraApp.h"
#include "TestCameraApp.hrh"

const TInt KMaxErrLength=50;
_LIT(KErrorFormat,"Error - %d (%S)");

//
//  CTCamAppUi
//

void CTCamAppUi::ConstructL()
	{
	RDebug::Print(_L("CTCamAppUi::ConstructL()_S"));

    BaseConstructL();
	iAppView = new(ELeave) CTCamAppView;
	iAppView->ConstructL(ClientRect());
	AddToStackL(iAppView);

	RDebug::Print(_L("CTCamAppUi::ConstructL() CCamera::NewL()_S"));
	iCamera = CCamera::NewL(*this,0);
	RDebug::Print(_L("CTCamAppUi::ConstructL() CCamera::NewL()_E"));

	RDebug::Print(_L("CTCamAppUi::ConstructL() iCamera->Reserve()_S"));
	iCamera->Reserve();
	RDebug::Print(_L("CTCamAppUi::ConstructL() iCamera->Reserve()_E"));

	RDebug::Print(_L("CTCamAppUi::ConstructL()_E"));
	}

CTCamAppUi::~CTCamAppUi()
	{
	RDebug::Print(_L("CTCamAppUi::~CTCamAppUi()_S"));

	RDebug::Print(_L("CTCamAppUi::~CTCamAppUi() iCamera->Release()_S"));
	iCamera->Release();
	RDebug::Print(_L("CTCamAppUi::~CTCamAppUi() iCamera->Release()_E"));

	RDebug::Print(_L("CTCamAppUi::~CTCamAppUi() delete iCamera_S"));
	delete iCamera;
	RDebug::Print(_L("CTCamAppUi::~CTCamAppUi() delete iCamera_E"));

	if (iAppView)
		{
		RemoveFromStack(iAppView);
		delete iAppView;
		}
	RDebug::Print(_L("CTCamAppUi::~CTCamAppUi()_E"));
	}

void CTCamAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case ETCamCmdPowerOn:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdPowerOn)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->PowerOn()_S"));
		iCamera->PowerOn();
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->PowerOn()_E"));
		break;

	case ETCamCmdPowerOff:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdPowerOff)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->PowerOff()_S"));
		iCamera->PowerOff();
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->PowerOff()_E"));
		break;

	case ETCamCmdViewFinder:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdViewFinder)"));

		ViewFinderL();
		break;

	case ETCamCmdCaptureImage:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdCaptureImage)"));

		CaptureImageL();
		break;

	case ETCamCmdCaptureVideo:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdCaptureVideo)"));

		CaptureVideoL();
		break;

	case ETCamCmdIncBrightness:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdIncBrightness)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetBrightnessL()_S"));
		iCamera->SetBrightnessL(iCamera->Brightness() + 10);
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetBrightnessL()_E"));
		break;

	case ETCamCmdIncContrast:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdIncContrast)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetContrastL()_S"));
		iCamera->SetContrastL(iCamera->Contrast() + 10);
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetContrastL()_E"));
		break;

	case ETCamCmdDecBrightness:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdDecBrightness)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetBrightnessL()_S"));
		iCamera->SetBrightnessL(iCamera->Brightness() - 10);
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetBrightnessL()_E"));
		break;

	case ETCamCmdDecContrast:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdDecContrast)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetContrastL()_S"));
		iCamera->SetContrastL(iCamera->Contrast() - 10);
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetContrastL()_E"));
		break;

	case ETCamCmdResetBrightness:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdResetBrightness)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetBrightnessL()_S"));
		iCamera->SetBrightnessL(0);
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetBrightnessL()_E"));
		break;

	case ETCamCmdResetContrast:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(ETCamCmdResetContrast)"));

		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetContrastL()_S"));
		iCamera->SetContrastL(0);
		RDebug::Print(_L("CTCamAppUi::HandleCommandL() iCamera->SetContrastL()_E"));
		break;

	case EAknSoftkeyBack:
		RDebug::Print(_L("CTCamAppUi::HandleCommandL(EAknSoftkeyBack)"));

		Exit();
		break;
		}
	}

void CTCamAppUi::ViewFinderL()
	{
	TSize imageSize;

	RDebug::Print(_L("CTCamAppUi::ViewFinderL() iCamera->EnumerateCaptureSizes()_S"));
	iCamera->EnumerateCaptureSizes(imageSize,1,CCamera::EFormatFbsBitmapColor16M);
	RDebug::Print(_L("CTCamAppUi::ViewFinderL() iCamera->EnumerateCaptureSizes()_E"));

	const TPoint pos = iAppView->DrawBorders(imageSize);
	TRect screenRect(pos,imageSize);
	TRect clipRect;

	if (!iCamera->ViewFinderActive())
		{
		RDebug::Print(_L("CTCamAppUi::ViewFinderL() iCamera->StartViewFinderDirectL()_S"));
		iCamera->StartViewFinderDirectL(iCoeEnv->WsSession(),*iCoeEnv->ScreenDevice(),*iAppView->DrawableWindow(),screenRect,clipRect);
		RDebug::Print(_L("CTCamAppUi::ViewFinderL() iCamera->StartViewFinderDirectL()_E"));
		}
	else
		{
		RDebug::Print(_L("CTCamAppUi::ViewFinderL() iCamera->StopViewFinder()_S"));
		iCamera->StopViewFinder();
		RDebug::Print(_L("CTCamAppUi::ViewFinderL() iCamera->StopViewFinder()_E"));
		}
	}

void CTCamAppUi::CaptureImageL()
	{
	TSize size;
	RDebug::Print(_L("CTCamAppUi::CaptureImageL() iCamera->EnumerateCaptureSizes()_S"));
	iCamera->EnumerateCaptureSizes(size,1,CCamera::EFormatFbsBitmapColor16M);
	RDebug::Print(_L("CTCamAppUi::CaptureImageL() iCamera->EnumerateCaptureSizes()_E"));

	RDebug::Print(_L("CTCamAppUi::CaptureImageL() iCamera->PrepareImageCaptureL()_S"));
	iCamera->PrepareImageCaptureL(CCamera::EFormatFbsBitmapColor16M,1);
	RDebug::Print(_L("CTCamAppUi::CaptureImageL() iCamera->PrepareImageCaptureL()_E"));

	RDebug::Print(_L("CTCamAppUi::CaptureImageL() iCamera->CaptureImage()_S"));
	iCamera->CaptureImage();
	RDebug::Print(_L("CTCamAppUi::CaptureImageL() iCamera->CaptureImage()_E"));
	}

void CTCamAppUi::CaptureVideoL()
	{
	if (!iCamera->VideoCaptureActive())
		{
		TSize size;
		TReal32 rate = 0;
		iCamera->EnumerateVideoFrameSizes(size,0,CCamera::EFormatFbsBitmapColor16M);
		iCamera->EnumerateVideoFrameRates(rate,0,CCamera::EFormatFbsBitmapColor16M,0);
		iCamera->PrepareVideoCaptureL(CCamera::EFormatFbsBitmapColor16M,0,0,2,1);
		iCamera->StartVideoCapture();
		}
	else
		iCamera->StopVideoCapture();
	}

void CTCamAppUi::ReserveComplete(TInt /*aError*/)
	{
	RDebug::Print(_L("CTCamAppUi::ReserveComplete()"));
	}

void CTCamAppUi::PowerOnComplete(TInt /*aError*/)
	{
	RDebug::Print(_L("CTCamAppUi::PowerOnComplete()"));
	}

void CTCamAppUi::ViewFinderFrameReady(CFbsBitmap& /*aFrame*/)
	{
	}

void CTCamAppUi::ImageReady(CFbsBitmap* aBitmap,HBufC8* /*aData*/,TInt aError)
	{
	RDebug::Print(_L("CTCamAppUi::ImageReady()_S"));

	TBuf<KMaxErrLength> msgBuffer;
	_LIT(KErrMessage,"CTCamAppUi::ImageReady");
	if (aBitmap)
		{
		iAppView->DrawImage(aBitmap);
		delete aBitmap;
		}
	if (aError)
		{
		msgBuffer.Format(KErrorFormat,aError,&KErrMessage);
		iEikonEnv->InfoMsg(msgBuffer);
		}

	RDebug::Print(_L("CTCamAppUi::ImageReady()_E"));
	}

void CTCamAppUi::FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError)
	{
	RDebug::Print(_L("CTCamAppUi::FrameBufferReady()_S"));

	TBuf<KMaxErrLength> msgBuffer;
	_LIT(KErrMessage,"CTCamAppUi::FrameBufferReady");
	if (aError)
		{
		msgBuffer.Format(KErrorFormat,aError,&KErrMessage);
		iEikonEnv->InfoMsg(msgBuffer);
		return;
		}
	TRAPD(error,iAppView->DrawImage(aFrameBuffer->FrameL(0)));
	aFrameBuffer->Release();
	if (error)
		{
		msgBuffer.Format(KErrorFormat,error,&KErrMessage);
		iEikonEnv->InfoMsg(msgBuffer);
		}

	RDebug::Print(_L("CTCamAppUi::FrameBufferReady()_E"));
	}

//_LIT(KContrastDialogTitle,"Contrast");
//_LIT(KBrightnessDialogTitle,"Brightness");

//
// CTCamAppView
//

CTCamAppView::CTCamAppView():
	CCoeControl()
	{}

void CTCamAppView::ConstructL(const TRect& aRect)
	{
	CreateWindowL();
    SetRect(aRect);
	EnableDragEvents();
	ActivateL();
	}

CTCamAppView::~CTCamAppView()
	{}

void CTCamAppView::DrawImage(CFbsBitmap* aImage) const
	{
	CWindowGc& gc = SystemGc();
	gc.Activate(Window());
	TRect drawRect(Rect());
	TPoint pos;
	pos.iX = (drawRect.iBr.iX - aImage->SizeInPixels().iWidth) / 2;
//	pos.iY = (drawRect.iBr.iY - aImage->SizeInPixels().iHeight) / 2;
	pos.iY = ((drawRect.iBr.iY + 122) - aImage->SizeInPixels().iHeight) / 2;
	gc.BitBlt(pos,aImage);
	gc.Deactivate();
	iCoeEnv->WsSession().Flush();
	}

void CTCamAppView::Draw(const TRect& /*aRect*/) const
	{
	CWindowGc& gc = SystemGc();
	TRect drawRect(Rect());
	drawRect.Shrink(1,1);
	gc.DrawRect(drawRect);
	gc.Clear();
	}

TPoint CTCamAppView::DrawBorders(const TSize& aSize) const
	{
	CWindowGc& gc = SystemGc();
	gc.Activate(Window());
	TRect drawRect(Rect());
	TPoint pos;
	pos.iX = (drawRect.iBr.iX - aSize.iWidth) / 2;
//	pos.iY = (drawRect.iBr.iY - aSize.iHeight) / 2;
	pos.iY = ((drawRect.iBr.iY + 122) - aSize.iHeight) / 2;
	TRect border(pos,aSize);
	border.Grow(1,1);
	gc.SetPenColor(KRgbBlack);
	gc.DrawRect(border);
	gc.Deactivate();
	iCoeEnv->WsSession().Flush();
	return pos;
	}

//
// CTCamDocument
//

CTCamDocument::CTCamDocument(CEikApplication& aApp)
		: CAknDocument(aApp)
	{
	}

CEikAppUi* CTCamDocument::CreateAppUiL()
	{
	RDebug::Print(_L("CTCamDocument::CreateAppUiL()"));
    return (static_cast<CEikAppUi*>(new (ELeave) CTCamAppUi));
	}

//
// CTCamApp
//

TUid CTCamApp::AppDllUid() const
	{
	return TUid::Uid(KUidTestCameraApp);
	}

CApaDocument* CTCamApp::CreateDocumentL()
	{
//	return new(ELeave) CTCamDocument(*this);
	RDebug::Print(_L("CTCamApp::CreateDocumentL()"));
	return (static_cast<CApaDocument*>(new(ELeave) CTCamDocument(*this)));
	}


//
// Base factory function
//

#include <eikstart.h>
LOCAL_C CApaApplication* NewApplication()
	{
	RDebug::Print(_L("CApaApplication* NewApplication()"));
	return new CTCamApp;
	}

//
// EXE Entry point
//

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}
