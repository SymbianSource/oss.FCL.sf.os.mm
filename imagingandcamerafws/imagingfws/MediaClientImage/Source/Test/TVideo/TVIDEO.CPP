// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <barsread.h>
#include <eikenv.h>
#include <eikinfo.h>
#include <techview/eikon.rsg>
#include <eikpriv.rsg>
#include <Mda\Client\Video.h>
#include "tvideo.h"

#include <hal.h> // for screen size info
#include <hal_data.h>

const TInt KVideoMinZoomFactor = -3;
const TInt KVideoMaxZoomFactor = 0;

// #define __USE_PURE_SCALING // fine scaling scales to 1/2, 1/4 and 1/8 - check non-universal scaling

//
//  CVideoAppUi
//

void CVideoAppUi::ConstructL()
    {
	CEikAppUi::ConstructL();
    iAppView = new(ELeave) CVideoAppView;
    iAppView->ConstructL(ClientRect());
	AddToStackL(iAppView);

	// find out the twips per pixel ratios for use on resizing.
	TSize displayPixels, displayTwips;
	User::LeaveIfError(HAL::Get(HALData::EDisplayXPixels, displayPixels.iWidth));
	User::LeaveIfError(HAL::Get(HALData::EDisplayYPixels, displayPixels.iHeight));
	User::LeaveIfError(HAL::Get(HALData::EDisplayXTwips, displayTwips.iWidth));
	User::LeaveIfError(HAL::Get(HALData::EDisplayYTwips, displayTwips.iHeight));
	
	iDisplayXTwipsPerPixel = TReal(displayTwips.iWidth) / displayPixels.iWidth;
	iDisplayYTwipsPerPixel = TReal(displayTwips.iHeight) / displayPixels.iHeight;

	ASSERT(iDisplayXTwipsPerPixel>0 && iDisplayXTwipsPerPixel>0);

	::new(&iFrame) CWsBitmap(CEikonEnv::Static()->WsSession());
	::new(&iMask) CWsBitmap(CEikonEnv::Static()->WsSession());
	iAppView->iBitmap = &iFrame;
	iAppView->iMask = &iMask;
	iLoadUtil = CMdaImageFileToBitmapUtility::NewL(*this,NULL);
	iSaveUtil = CMdaImageBitmapToFileUtility::NewL(*this,NULL);
	iRotator = CMdaBitmapRotator::NewL();
	iScaler = CMdaBitmapScaler::NewL();
	iBackgroundColor = KRgbWhite.Color16();

	iSaveInfo.iType = 2;
	iSaveInfo.iBpp = 6;
	iSaveInfo.iColor = ETrue;
	iSaveInfo.iQualityFactor = 40;
	iSaveInfo.iSampling = 2;
	}

CVideoAppUi::~CVideoAppUi()
	{
	iFrame.Reset();
	iMask.Reset();
	if (iAppView)
		{
		RemoveFromStack(iAppView);
		delete iAppView;
		}
	delete iLoadUtil;
	delete iSaveUtil;
	delete iRotator;
	delete iScaler;
	}

TKeyResponse CVideoAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
	{
	TKeyResponse ret = EKeyWasConsumed;

	switch (aKeyEvent.iCode)
		{
	case EKeyLeftArrow:
		iAppView->Move(TPoint(16,0));
		break;
	case EKeyRightArrow:
		iAppView->Move(TPoint(-16,0));
		break;
	case EKeyUpArrow:
		iAppView->Move(TPoint(0,16));
		break;
	case EKeyDownArrow:
		iAppView->Move(TPoint(0,-16));
		break;
	case EKeyEscape:
		iLoadUtil->Cancel();
		iSaveUtil->Cancel();
		CEikonEnv::Static()->BusyMsgCancel();
		CEikonEnv::Static()->InfoMsg(_L("Cancelled"));
		iState = EIdle;
		break;
	default:
		ret = EKeyWasNotConsumed;
		break;
		}
	return ret;
	}

void CVideoAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EVideoCmdOpenFile:
		OpenFileL(EFileTypeUnknown);
		break;
	case EVideoCmdOpenOpaFile:
		OpenFileL(EFileTypeOta);
		break;
	case EVideoCmdOpenWbmpFile:
		OpenFileL(EFileTypeWbmp);
		break;
	case EVideoCmdSaveAs:
		SaveAsL();
		break;
	case EVideoCmdMask:
		if (iMask.Handle())
			{
			CEikonEnv::Static()->InfoMsg(_L("Mask reset"));
			iMask.Reset();
			}
		else
			{
			CEikonEnv::Static()->InfoMsg(_L("Mask enabled"));
			User::LeaveIfError(iMask.Create(TSize(0,0),EGray256));
			}
		break;
	case EVideoCmdZoomIn:
		iZoomFactor++;
		if (iZoomFactor > KVideoMaxZoomFactor)
			iZoomFactor = KVideoMinZoomFactor;
		iAppView->Clear();
		LoadFileL();
		break;
	case EVideoCmdZoomOut:
		iZoomFactor--;
		if (iZoomFactor < KVideoMinZoomFactor)
			iZoomFactor = KVideoMaxZoomFactor;
		iAppView->Clear();
		LoadFileL();
		break;
	case EVideoCmdFineZoomIn:
		ZoomFrameL(ETrue);
		break;
	case EVideoCmdFineZoomOut:
		ZoomFrameL(EFalse);
		break;
	case EVideoCmdDisplayMode:
		DisplayModeL();
		break;
	case EVideoCmdBackgroundColor:
		BackgroundColorL();
		break;
	case EVideoCmdPlay:
		if (iState == EPlaying || iState == ELoading)
			CEikonEnv::Static()->InfoMsg(_L("Busy..."));
		else
			PlayClipL();
		break;
	case EVideoCmdStream:
		iEikonEnv->InfoMsg(R_VIDEO_NOT_SUPPORTED);
		break;
	case EVideoCmdRefresh:
		LoadFileL();
		break;
	case EVideoCmdExtractFrame:
		ExtractFrameL();
		break;
	case EVideoCmdFrameInfo:
		iEikonEnv->InfoMsg(R_VIDEO_NOT_SUPPORTED);
		break;
	case EVideoCmdRotateClockwise:
		FrameRotateL(ETrue);
		break;
	case EVideoCmdRotateAntiClockwise:
		FrameRotateL(EFalse);
		break;
	case EVideoCmdFlip:
		FrameFlipL();
		break;
	case EVideoCmdTest:
		iEikonEnv->InfoMsg(R_VIDEO_NOT_SUPPORTED);
		break;
	case EEikCmdExit:
/*		if (iLoadUtil)
			{
			iLoadUtil->Close();
			delete iLoadUtil;
			iLoadUtil = NULL;
			}
*/
		Exit();
		break;
	default:
		iEikonEnv->InfoMsg(R_VIDEO_TBUF_NOT_FOUND);
		}
	}

TBool CVideoAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail)
	{
	switch (aCommand)
		{
	case EApaCommandOpen:
		iLoadFileName.iName = aDocumentName;
		break;
	case EApaCommandCreate:
	case EApaCommandRun:
		break;
	case EApaCommandBackground:
	default:
		break;
		}

	return CEikAppUi::ProcessCommandParametersL(aCommand,aDocumentName,aTail);
	}

void CVideoAppUi::OpenFileL(TFileType aFileType)
	{
	if (iLoadFileName.iName.Length() < 4)
		iLoadFileName.iName = _L("c:\\");

	CEikDialog* dialog = new(ELeave) CEikFileOpenDialog(&iLoadFileName.iName);
	if (!dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		return;

	iLastFileType = aFileType;
	LoadFileL();
	}

void CVideoAppUi::LoadFileL()
	{
	if (iLoadFileName.iName.Length() < 4)
		return;

	iLoadUtil->Cancel();
	iAppView->Clear();
	iFrameNumber = 0;

	iTime.HomeTime();

	TMdaClipFormat* format;

	switch (iLastFileType)
		{
		case EFileTypeOta:
			format = &iOtaFormat;
			break;
		case EFileTypeWbmp:
			format = &iWbmpFormat;
			break;
		default:
			format = NULL;
		}

	STATIC_CAST(CMdaImageFileToBitmapUtility*,iLoadUtil)->OpenL(iLoadFileName.iName, format);

	CEikonEnv::Static()->BusyMsgL(_L("Loading..."));
	iState = EPlaying;
	}

void CVideoAppUi::ExtractFrameL()
	{
	if (iLoadFileName.iName.Length() < 4)
		return;

	TInt frameCount = iLoadUtil->FrameCount();

	iFrameNumber -= 1; // return to the previous one we looked at

	if (iFrameNumber < 0 || iFrameNumber >= frameCount)
		iFrameNumber = 0;

	CEikDialog* dialog = new(ELeave) CVideoCurrentFrameDialog(iFrameNumber,frameCount);
	if (!dialog->ExecuteLD(R_VIDEO_FRAME_DIALOG))
		return;

	iState = ELoading;
	MiuoOpenComplete(KErrNone);
	}

void CVideoAppUi::PlayClipL()
	{
	iAppView->Clear();
	iFrameNumber = 0;
	iTime.HomeTime();

	iState = EPlaying;
	MiuoOpenComplete(KErrNone);
	}

void CVideoAppUi::SaveAsL()
	{
	if (iSaveFileName.iName.Length() < 4)
		iSaveFileName.iName = _L("c:\\");
	CEikDialog* dialog = new(ELeave) CVideoSaveAsDialog(&iSaveFileName.iName,iSaveInfo);
	if (!dialog->ExecuteLD(R_VIDEO_FILE_SAVEAS_DIALOG))
		return;
	CEikonEnv::Static()->FsSession().Delete(iSaveFileName.iName);

	TMdaClipFormat* format = NULL;
	TMdaPackage* codec = NULL;

	switch (iSaveInfo.iType)
		{
	case 0:
		format = &iBmpFormat;
		switch (iSaveInfo.iBpp)
			{
		case 0:		codec = &iBmp1BppCodec;		break;
		case 2:		codec = &iBmp4BppCodec;		break;
		case 3:		codec = &iBmp8BppCodec;		break;
		case 6:		codec = &iBmp24BppCodec;	break;
		default:	User::Invariant();			break;
			}
		break;
	case 1:
		format = &iGifFormat;
		codec = &iGifCodec;
		break;
	case 2:
		if (!iSaveInfo.iColor)
			iJfifFormat.iSettings.iSampleScheme = TMdaJpgSettings::EMonochrome;
		else
			iJfifFormat.iSettings.iSampleScheme = TMdaJpgSettings::TColorSampling(3 - iSaveInfo.iSampling);
		iJfifFormat.iSettings.iQualityFactor = iSaveInfo.iQualityFactor;
		format = &iJfifFormat;
		break;
	case 3:
		switch (iSaveInfo.iBpp)
			{
		case 0:		iMbmFormat.iDisplayMode = EGray2;		break;
		case 1:		iMbmFormat.iDisplayMode = EGray4;		break;
		case 2:		iMbmFormat.iDisplayMode = iSaveInfo.iColor ? EColor16 : EGray16;	break;
		case 3:		iMbmFormat.iDisplayMode = iSaveInfo.iColor ? EColor256 : EGray256;	break;
		case 4:		iMbmFormat.iDisplayMode = EColor4K;		break;
		case 5:		iMbmFormat.iDisplayMode = EColor64K;	break;
		case 6:		iMbmFormat.iDisplayMode = EColor16M;	break;
		case 7:		iMbmFormat.iDisplayMode = EColor16MU;	break;
		default:	User::Invariant();						break;
			}
		format = &iMbmFormat;
		break;
		}

	CEikonEnv::Static()->BusyMsgL(_L("Saving..."));
	STATIC_CAST(CMdaImageBitmapToFileUtility*,iSaveUtil)->CreateL(iSaveFileName.iName,format,codec,NULL);
	}

void CVideoAppUi::DisplayModeL()
	{
	TDisplayMode displayMode = iAppView->Window().DisplayMode();
	CEikDialog* dialog = new(ELeave) CVideoDisplayModeDialog(displayMode);
	if (dialog->ExecuteLD(R_VIDEO_DISPLAY_MODE_DIALOG))
		{
		if (iFrame.Handle() != 0 && iFrame.SizeInPixels() != TSize(0,0))
			ResizeFrameL(iFrame.SizeInPixels(), displayMode);

		iAppView->Window().SetRequiredDisplayMode(displayMode);
		iAppView->Refresh();
		}
	}

void CVideoAppUi::BackgroundColorL()
	{
	CEikDialog* dialog = new(ELeave) CVideoBackgroundColorDialog(iBackgroundColor);
	if (dialog->ExecuteLD(R_VIDEO_BACKGROUND_COLOR_DIALOG))
		{
		if (iBackgroundColor == KRgbWhite.Color16())
			iAppView->Window().SetBackgroundColor();
		else
			iAppView->Window().SetBackgroundColor(TRgb::Color16(iBackgroundColor));

		iAppView->Clear();
		iAppView->Refresh();
		}
	}

void CVideoAppUi::FrameRotateL(TBool aClockwise)
	{
	CMdaBitmapRotator::TRotationAngle angle = aClockwise ?
			CMdaBitmapRotator::ERotation90DegreesClockwise :
			CMdaBitmapRotator::ERotation270DegreesClockwise;
	iMask.Reset();
	iRotator->RotateL(*this,iFrame,angle);
	}

void CVideoAppUi::FrameFlipL()
	{
	iMask.Reset();
	iRotator->RotateL(*this,iFrame,CMdaBitmapRotator::ERotation180DegreesClockwise);
	}

void CVideoAppUi::ZoomFrameL(TBool aZoomIn)
	{
	TSize size(iFrame.SizeInPixels());
	const TSize adjust(size.iWidth / 10,size.iHeight / 10);

	if (aZoomIn)
		size += adjust;
	else
		size -= adjust;

	iLoadUtil->Cancel();

	iAppView->Clear();

	TFrameInfo frameInfo;
	iLoadUtil->FrameInfo(iFrameNumber-1,frameInfo);

	if (!(frameInfo.iFlags&TFrameInfo::EFullyScaleable))
		iScaler->ScaleL(*this,iFrame,size);
	else
		{
		SetFrameSizeAndPosL(frameInfo.iFrameCoordsInPixels,size); 

		if (!(frameInfo.iFlags&TFrameInfo::ECanDither))
			{
			iFrame.Reset();
			User::LeaveIfError(iFrame.Create(TSize(0,0),EColor64K)); // We will dither later
			}

		if (iFrame.SizeInPixels() != size)
			{
			User::LeaveIfError(iFrame.Resize(TSize(0,0)));
			User::LeaveIfError(iFrame.Resize(size));
			}

		if (iMask.Handle() && iMask.SizeInPixels() != size)
			{
			User::LeaveIfError(iMask.Resize(TSize(0,0)));
			User::LeaveIfError(iMask.Resize(size));
			}

		if (iMask.Handle())
			iLoadUtil->ConvertL(iFrame,iMask,iFrameNumber-1); // jf 3/12/01. -1 because iFrameNumber is incremented on open
		else
			iLoadUtil->ConvertL(iFrame,iFrameNumber-1);

		iState = ELoading; // jf 4/12 was EPlaying but that made it do other frames too
		}
	}

// ResizeFrameL creates a new frame with the given size and mode, but will
// keep the original data - stretched if req. Arguably overkill if the size
// does not change
void CVideoAppUi::ResizeFrameL(const TSize& aNewSize, TDisplayMode aNewMode)
	{
	// create a new bitmap with these params
	CFbsBitmap* newFrame = new (ELeave) CWsBitmap(iCoeEnv->WsSession());
	CleanupStack::PushL(newFrame);
	User::LeaveIfError(newFrame->Create(aNewSize, aNewMode));

	// now create BitmapDevice and bitGc so we can modify it
	CFbsBitmapDevice* bmDevice = CFbsBitmapDevice::NewL(newFrame);
	CleanupStack::PushL(bmDevice);
	CFbsBitGc* bmGc = CFbsBitGc::NewL();
	CleanupStack::PushL(bmGc);
	bmGc->Activate(bmDevice);

	// finally we have to set the SizeInTwips of the two bitmaps, so we can use DrawBitmap
	TSize size = iFrame.SizeInPixels();
	size.iWidth = TInt (size.iWidth * iDisplayXTwipsPerPixel + 0.5);
	size.iHeight = TInt (size.iHeight * iDisplayYTwipsPerPixel + 0.5);
	iFrame.SetSizeInTwips(size);
	size = newFrame->SizeInPixels();
	size.iWidth = TInt (size.iWidth * iDisplayXTwipsPerPixel + 0.5);
	size.iHeight = TInt (size.iHeight * iDisplayYTwipsPerPixel + 0.5);
	newFrame->SetSizeInTwips(size);

	bmGc->DrawBitmap(TPoint(0,0), &iFrame); // resize iFrame to newFrame

	CleanupStack::PopAndDestroy(2); // bmGc and bmDevice

	iFrame.Reset(); // lose original bitmap
	iFrame.Duplicate(newFrame->Handle()); // just swap the bitmap handles
	newFrame->Reset();

	CleanupStack::PopAndDestroy(); // newFrame
	}

void CVideoAppUi::MiuoCreateComplete(TInt aError)
	{
	if (aError)
		iEikonEnv->HandleError(aError);
	else
		{
		TRAPD(err, HandleMiuoCreateCompleteL());
		if (err != KErrNone)
			iEikonEnv->HandleError(err);
		}
	}

void CVideoAppUi::HandleMiuoCreateCompleteL()
	{
	iSaveUtil->ConvertL(iFrame,0);
	}

void CVideoAppUi::MiuoOpenComplete(TInt aError)
	{
	if (aError)
		iEikonEnv->HandleError(aError);
	else
		{
		TRAPD(err, HandleMiuoOpenCompleteL());
		if (err != KErrNone)
			iEikonEnv->HandleError(err);
		}
	}

void CVideoAppUi::HandleMiuoOpenCompleteL()
	{
	if (iFrameNumber < iLoadUtil->FrameCount())
		{
		TFrameInfo frameInfo;
		iLoadUtil->FrameInfo(iFrameNumber,frameInfo);

		if (iFrameNumber == 0)
			{
			// jf 3/12/01. Assume frame 0 gives the main image characteristics
			iOverallSize = frameInfo.iOverallSizeInPixels;
			if (iOverallSize.iWidth==0 || iOverallSize.iHeight==0)
				iOverallSize = frameInfo.iFrameCoordsInPixels.Size();
			}

		if (frameInfo.iFlags & TFrameInfo::ECanDither || iAppView->Window().DisplayMode() > EColor256)
			User::LeaveIfError(iFrame.Create(TSize(0,0),iAppView->Window().DisplayMode()));
		else
			User::LeaveIfError(iFrame.Create(TSize(0,0),EColor64K)); // We will dither.

		if (frameInfo.iFlags & TFrameInfo::ETransparencyPossible)
			{
			if (iMask.Handle())
				User::LeaveIfError(iMask.Resize(TSize(0,0)));
			else
				User::LeaveIfError(iMask.Create(TSize(0,0),EGray256));
			}
		else
			{
			if (iMask.Handle())
				iMask.Reset();
			}

		SetFrameSizeAndPosL(frameInfo.iFrameCoordsInPixels,frameInfo.iOverallSizeInPixels);

		if (iMask.Handle())
			iLoadUtil->ConvertL(iFrame,iMask,iFrameNumber++);
		else
			iLoadUtil->ConvertL(iFrame,iFrameNumber++);
		}
	else
		{
		CEikonEnv::Static()->BusyMsgCancel();
		iState = EIdle;
		}
	}

void CVideoAppUi::MiuoConvertComplete(TInt aError)
	{
	TRAPD(err, HandleMiuoConvertCompleteL(aError));
	if (err != KErrNone)
		iEikonEnv->HandleError(err);
	}

void CVideoAppUi::HandleMiuoConvertCompleteL(TInt aError)
	{
	TFrameInfo frameInfo;
	iLoadUtil->FrameInfo(iFrameNumber - 1,frameInfo);

	if (iState == EPlaying)
		{
		if (aError == KErrNone)
			{
			if (!(frameInfo.iFlags&TFrameInfo::ECanDither))
				ResizeFrameL(iFrame.SizeInPixels(), iAppView->Window().DisplayMode());

			if (frameInfo.iDelay == TTimeIntervalMicroSeconds(0) ||
				iFrameNumber > 1 && iLastFrameFlags & TFrameInfo::ERestoreToBackground)
				iAppView->Clear();

			iAppView->Refresh();

			if (iFrameNumber <= 1)
				iAppView->DrawFrame();

			iLastFrameFlags = frameInfo.iFlags;
			if (frameInfo.iDelay > TTimeIntervalMicroSeconds(0))
				{
				TTime endTime;
				endTime.HomeTime();
				TTimeIntervalMicroSeconds timeTaken = endTime.MicroSecondsFrom(iTime);

				if (frameInfo.iDelay > timeTaken)
					{
					User::After(I64LOW(frameInfo.iDelay.Int64() - timeTaken.Int64()));
					}

				iTime.HomeTime();

				MiuoOpenComplete(KErrNone); // Try to load the next frame
				return;
				}
			}
		}
	else
		{
		TFrameInfo frameInfo;
		iLoadUtil->FrameInfo(iFrameNumber - 1,frameInfo);

		if (frameInfo.iDelay == TTimeIntervalMicroSeconds(0) ||
			frameInfo.iFlags & TFrameInfo::ERestoreToBackground)
			{
			iAppView->Clear();
			iAppView->Refresh();
			iAppView->DrawFrame();
			}
		else
			{
			iAppView->Refresh();
			if (iFrameNumber == 1)
				iAppView->DrawFrame();
			}
		}

	iState = EIdle;
	CEikonEnv::Static()->BusyMsgCancel();
	User::LeaveIfError(aError);
	}

void CVideoAppUi::SetFrameSizeAndPosL(const TRect& aFrameRect,const TSize& aOverallSize)
	{
	TRect zoomedFrame(aFrameRect);
	TSize zoomedSize(aOverallSize);
	TSize zoomedOverallSize(iOverallSize); // effectively size on frame 0
	if (aOverallSize.iWidth == 0 || aOverallSize.iHeight == 0)
		zoomedSize = aFrameRect.Size();

	if (iZoomFactor > 0)
		{
		zoomedFrame.iTl.iX <<= iZoomFactor;
		zoomedFrame.iTl.iY <<= iZoomFactor;
		zoomedFrame.iBr.iX <<= iZoomFactor;
		zoomedFrame.iBr.iY <<= iZoomFactor;
		zoomedSize.iWidth <<= iZoomFactor;
		zoomedSize.iHeight <<= iZoomFactor;
		zoomedOverallSize.iWidth <<= iZoomFactor;
		zoomedOverallSize.iHeight <<= iZoomFactor;
		}
	else if (iZoomFactor < 0)
		{
		const TInt absZoomFactor = -iZoomFactor;
#if defined(__USE_PURE_SCALING)
		const TInt roundingFactor = 0; // set 0 to get pure scaling
#else
		const TInt roundingFactor = (1 << absZoomFactor) - 1; 
#endif defined(__USE_PURE_SCALING)
		zoomedFrame.iTl.iX = (zoomedFrame.iTl.iX) >> absZoomFactor;
		zoomedFrame.iTl.iY = (zoomedFrame.iTl.iY) >> absZoomFactor;
		zoomedFrame.iBr.iX = (zoomedFrame.iBr.iX + roundingFactor) >> absZoomFactor;
		zoomedFrame.iBr.iY = (zoomedFrame.iBr.iY + roundingFactor) >> absZoomFactor;
		zoomedSize.iWidth = (zoomedSize.iWidth + roundingFactor) >> absZoomFactor;
		zoomedSize.iHeight = (zoomedSize.iHeight + roundingFactor) >> absZoomFactor;
		zoomedOverallSize.iWidth = (zoomedOverallSize.iWidth + roundingFactor) >> absZoomFactor;
		zoomedOverallSize.iHeight = (zoomedOverallSize.iHeight + roundingFactor) >> absZoomFactor;
		}

	if (iFrame.SizeInPixels() != zoomedFrame.Size())
		User::LeaveIfError(iFrame.Resize(zoomedSize));

	if (iMask.Handle() && iMask.SizeInPixels() != zoomedFrame.Size())
		User::LeaveIfError(iMask.Resize(zoomedSize));

	iAppView->iPos.iX = (iAppView->Rect().Width() - zoomedOverallSize.iWidth) / 2;
	iAppView->iPos.iY = (iAppView->Rect().Height() - zoomedOverallSize.iHeight) / 2;
	iAppView->iOffset = zoomedFrame.iTl;
	}

//
// CVideoAppView
//

CVideoAppView::CVideoAppView():
	CCoeControl()
	{}

void CVideoAppView::ConstructL(const TRect& /*aRect*/)
    {
	CreateWindowL();
#if defined(__WINS__)
	Window().SetRequiredDisplayMode(EColor256);
#endif
	EnableDragEvents();
    SetExtentToWholeScreen();
	ActivateL();
	}

CVideoAppView::~CVideoAppView()
	{}

void CVideoAppView::Draw(const TRect& /*aRect*/) const
	{
	CWindowGc& gc = SystemGc();
	TRect drawRect=Rect();

	if (iBitmap && iBitmap->Handle())
		{
		TSize bitmapSize(iBitmap->SizeInPixels());
		TPoint pt(iPos + iOffset);
		if (iMask->Handle())
			gc.BitBltMasked(pt,iBitmap,TRect(bitmapSize),iMask,EFalse);
		else
			gc.BitBlt(pt,iBitmap);
		}
	else
		{
		gc.Clear();
		drawRect.Shrink(10,10);
		gc.DrawRect(drawRect);
		gc.DrawLine(drawRect.iTl,drawRect.iBr);
		gc.DrawLine(TPoint(drawRect.iTl.iX,drawRect.iBr.iY),TPoint(drawRect.iBr.iX,drawRect.iTl.iY));
		}
	}

void CVideoAppView::DrawFrame() const
	{
	CWindowGc& gc = SystemGc();
	gc.Activate(Window());

	if (iBitmap && iBitmap->Handle())
		{
		TSize bitmapSize(iBitmap->SizeInPixels());
		TPoint pt(iPos + iOffset);
		gc.SetBrushStyle(CGraphicsContext::ENullBrush);
		gc.DrawRect(TRect(pt.iX - 1,pt.iY - 1,pt.iX + bitmapSize.iWidth + 1,pt.iY + bitmapSize.iHeight + 1));
		}

	gc.Deactivate();
	ControlEnv()->WsSession().Flush();
	}

void CVideoAppView::Refresh()
	{
	CWindowGc& gc = SystemGc();
	gc.Activate(Window());
	Draw(Rect());
	gc.Deactivate();
	ControlEnv()->WsSession().Flush();
	}

void CVideoAppView::Clear()
	{
	CWindowGc& gc = SystemGc();
	gc.Activate(Window());
	gc.Clear();
	gc.Deactivate();
	ControlEnv()->WsSession().Flush();
	}

void CVideoAppView::Move(const TPoint& aMove)
	{
	iPos += aMove;

	CWindowGc& gc = SystemGc();
	gc.Activate(Window());
	gc.Clear(Rect());
	Draw(Rect());
	gc.Deactivate();
	ControlEnv()->WsSession().Flush();
	}

//
// CVideoDisplayModeDialog
//

CVideoDisplayModeDialog::CVideoDisplayModeDialog(TDisplayMode& aDisplayMode):
	iDisplayMode(aDisplayMode)
	{}

void CVideoDisplayModeDialog::PreLayoutDynInitL()
	{
	TInt choice = 0;
	switch (iDisplayMode)
		{
	case EGray4:
		choice = 1;
		break;
	case EGray16:
		choice = 2;
		break;
	case EGray256:
		choice = 3;
		break;
	case EColor16:
		choice = 4;
		break;
	case EColor256:
		choice = 5;
		break;
	case EColor4K:
		choice = 6;
		break;
	case EColor64K:
		choice = 7;
		break;
	case EColor16M:
		choice = 8;
		break;
	case EColor16MU:
		choice = 9;
		break;
	default:
		break;
		}
	SetChoiceListCurrentItem(EVideoIdDisplayMode,choice);
	}

TBool CVideoDisplayModeDialog::OkToExitL(TInt /*aButtonId*/)
	{
	const TDisplayMode dispMode[] = { EGray2, EGray4, EGray16, EGray256, EColor16, EColor256, EColor4K, EColor64K, EColor16M, EColor16MU };
	iDisplayMode = dispMode[ChoiceListCurrentItem(EVideoIdDisplayMode)];
	return ETrue;
	}

//
// CVideoBackgroundColorDialog
//

CVideoBackgroundColorDialog::CVideoBackgroundColorDialog(TInt& aColor16):
	iColor16(aColor16)
	{}

void CVideoBackgroundColorDialog::PreLayoutDynInitL()
	{
	SetChoiceListCurrentItem(EVideoIdBackgroundColor,iColor16);
	}

TBool CVideoBackgroundColorDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iColor16 = ChoiceListCurrentItem(EVideoIdBackgroundColor);
	return ETrue;
	}

//
// CVideoCurrentFrameDialog
//

CVideoCurrentFrameDialog::CVideoCurrentFrameDialog(TInt& aCurrentFrame,TInt aNumberOfFrames):
	iCurrentFrame(aCurrentFrame),
	iNumberOfFrames(aNumberOfFrames)
	{}

void CVideoCurrentFrameDialog::PreLayoutDynInitL()
	{
	SetNumberEditorValue(EVideoIdNumberOfFrames,iNumberOfFrames);
	SetLineDimmedNow(EVideoIdNumberOfFrames,ETrue);
	const TInt lastFrame = iNumberOfFrames - 1;
	SetNumberEditorMinAndMax(EVideoIdCurrentFrameNumber,0,lastFrame);
	SetNumberEditorValue(EVideoIdCurrentFrameNumber,Min(iCurrentFrame,lastFrame));
	}

TBool CVideoCurrentFrameDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iCurrentFrame = NumberEditorValue(EVideoIdCurrentFrameNumber);
	return ETrue;
	}

//
// CVideoSaveAsDialog
//

CVideoSaveAsDialog::CVideoSaveAsDialog(TDes* aFileName,TFileSaveInfo& aSaveInfo):
	CEikFileSaveAsDialog(aFileName),
	iSaveInfo(aSaveInfo)
	{}

void CVideoSaveAsDialog::PreLayoutDynInitL()
	{
	SetTypeL();
	CEikFileSaveAsDialog::PreLayoutDynInitL();
	}

void CVideoSaveAsDialog::SetTypeL()
	{
	switch (iSaveInfo.iType)
		{
	case 0:
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_BMP);
		break;
	case 1:
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_GIF);
		break;
	case 2:
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_JPEG);
		break;
	case 3:
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_MBM);
		break;
		}
	}

TBool CVideoSaveAsDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId == EVideoIdSaveAsFormat)
		{
		CEikDialog* dialog = new(ELeave) CVideoFormatDialog(iSaveInfo);
		if (dialog->ExecuteLD(R_VIDEO_FILE_FORMAT_DIALOG))
			SetTypeL();
		return EFalse;
		}

	return CEikFileSaveAsDialog::OkToExitL(aButtonId);
	}

//
// CVideoFormatDialog
//

CVideoFormatDialog::CVideoFormatDialog(TFileSaveInfo& aSaveInfo):
	iSaveInfo(aSaveInfo)
	{}

void CVideoFormatDialog::PreLayoutDynInitL()
	{
	SetChoiceListCurrentItem(EVideoIdFileFormatType,iSaveInfo.iType);
	SetChoiceListCurrentItem(EVideoIdFileFormatBpp,iSaveInfo.iBpp);
	SetChoiceListCurrentItem(EVideoIdFileFormatColor,iSaveInfo.iColor);
	SetNumberEditorValue(EVideoIdFileFormatFactor,iSaveInfo.iQualityFactor);
	SetChoiceListCurrentItem(EVideoIdFileFormatSampling,iSaveInfo.iSampling);

	ValidateControlState();
	}

void CVideoFormatDialog::HandleControlStateChangeL(TInt /*aControlId*/)
	{
	ValidateControlState();
	}

void CVideoFormatDialog::ValidateControlState()
	{
	TInt type = ChoiceListCurrentItem(EVideoIdFileFormatType);
	TInt bpp = ChoiceListCurrentItem(EVideoIdFileFormatBpp);
	TInt color = ChoiceListCurrentItem(EVideoIdFileFormatColor);

	TBool bppVisible = ETrue;
	TBool colorVisible = ETrue;
	TBool factorVisible = EFalse;
	TBool samplingVisible = EFalse;

	switch (type)
		{
	case 0: // Bmp
		colorVisible = EFalse;
		if (bpp == 1)
			bpp = 2;
		else if (bpp > 3)
			bpp = 6;
		break;
	case 1: // Gif
		bppVisible = EFalse;
		colorVisible = EFalse;
		break;
	case 2: // Jpeg
		bppVisible = EFalse;
		factorVisible = ETrue;
		if (color)
			samplingVisible = ETrue;
		break;
	case 3: // Mbm
		if (color == 0)
			{
			if (bpp > 3)
				bpp = 3;
			}
		else
			{
			if (bpp < 2)
				bpp = 2;
			}
		break;
		}

	SetChoiceListCurrentItem(EVideoIdFileFormatBpp,bpp);
	SetChoiceListCurrentItem(EVideoIdFileFormatColor,color);

	MakeLineVisible(EVideoIdFileFormatBpp,bppVisible);
	MakeLineVisible(EVideoIdFileFormatColor,colorVisible);
	MakeLineVisible(EVideoIdFileFormatFactor,factorVisible);
	MakeLineVisible(EVideoIdFileFormatSampling,samplingVisible);
	}

TBool CVideoFormatDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iSaveInfo.iType = ChoiceListCurrentItem(EVideoIdFileFormatType);
	iSaveInfo.iBpp = ChoiceListCurrentItem(EVideoIdFileFormatBpp);
	iSaveInfo.iColor = ChoiceListCurrentItem(EVideoIdFileFormatColor);
	iSaveInfo.iQualityFactor = NumberEditorValue(EVideoIdFileFormatFactor);
	iSaveInfo.iSampling = ChoiceListCurrentItem(EVideoIdFileFormatSampling);

	return ETrue;
	}

//
// CVideoDocument
//

CVideoDocument::CVideoDocument(CEikApplication& aApp)
		: CEikDocument(aApp)
	{
	}

CEikAppUi* CVideoDocument::CreateAppUiL()
	{
    return new(ELeave) CVideoAppUi;
	}

//
// CVideoApp
//

TUid CVideoApp::AppDllUid() const
	{
	return KUidTVideo;
	}

CApaDocument* CVideoApp::CreateDocumentL()
	{
	return new(ELeave) CVideoDocument(*this);
	}


//
// Base factory function
//

#include <eikstart.h>
LOCAL_C CApaApplication* NewApplication()
	{
	return new CVideoApp;
	}

//
// EXE Entry point
//

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

