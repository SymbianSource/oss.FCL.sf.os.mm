// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <bidi.h>
#include <eikenv.h>
#include <techview/eikchlst.h>
#include <techview/eikinfo.h>
#include <techview/eikmenup.h>
#include <e32math.h>
#include <badesca.h>
#include <techview/eikon.rsg>
#include <eikpriv.rsg>
#include <bitmaptransforms.h>
#include <icl/imagedisplayplugin.h>
#include <icl/geniclimagedisplayext.h>

#include "plugininfoarray.h"
#include "TImageDisplayAppView.h"
#include "TImageDisplayDlgs.h"

#include "TImageDisplay.h"


const TInt KVideoMinZoomFactor = -3;
const TInt KVideoMaxZoomFactor = 3;

const TInt KDefaultFolderDelay = 2000000; // 2s. Used when playing a folder

const TInt KButtonMoveIncr = 16;

const TInt KInfoBufferSize = 1024;
const TInt KNoRotation=-1;
const TInt KAutoRotate=(1<<16);

_LIT(KDefPath, "C:\\");
const TInt KShortestPossiblePath=4;

// #define __CLEAR_BITMAPS_FIRST // for debugging purposes, get TImageViewer to clear bitmaps prior to
							  // decode conversion. Really the codecs should do this, but...

CPluginInfoArray* CPluginInfoArray::NewL()
	{
	CPluginInfoArray* self;
	self = new (ELeave) CPluginInfoArray;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPluginInfoArray::ConstructL()
	{
	RefreshPluginListL();
	}

CPluginInfoArray::CPluginInfoArray()
	{
	}

void CPluginInfoArray::Reset()
	{
	iPluginArray.ResetAndDestroy();
	}

void CPluginInfoArray::RefreshPluginListL()
	{
	Reset();
	CImageEncoder::GetImageTypesL(iPluginArray);
	}

CPluginInfoArray::~CPluginInfoArray()
	{
	Reset();
	}

TInt CPluginInfoArray::MdcaCount() const
	{
	return iPluginArray.Count();
	}

TPtrC CPluginInfoArray::MdcaPoint(TInt aIndex) const
	{
	return iPluginArray[aIndex]->Description(); 
	}

TUid CPluginInfoArray::ImageType(TInt aIndex)
	{
	return iPluginArray[aIndex]->ImageType(); 
	}

TUid CPluginInfoArray::SubType(TInt aIndex)
	{
	return iPluginArray[aIndex]->SubType(); 
	}

// CFrameInfoDialog
class CFrameInfoDialog : public CEikDialog
	{
public:
	CFrameInfoDialog(const TDesC& aPropertiesText);

protected:
	void PreLayoutDynInitL();

private:
	const TDesC& iPropertiesText;
	};

CFrameInfoDialog::CFrameInfoDialog(const TDesC& aPropertiesText)
	: iPropertiesText(aPropertiesText)
	{
	}

void CFrameInfoDialog::PreLayoutDynInitL()
	{
	// Grab each Edwin and set the text.
	CEikEdwin* page = static_cast<CEikEdwin*>(Control(EFramePropertiesPage));
	page->SetTextL(&iPropertiesText);

	}


//
// CVideoWalker
//

class CVideoWalker : public CActive
	{
public:
	static CVideoWalker* NewL(CImgDisplAppUi* aAppUi);
	~CVideoWalker();
	TRequestStatus& ActiveStatus();
	void SelfComplete(TInt aError);
protected:
	CVideoWalker(CImgDisplAppUi* aAppUi);
	// from CActive
	void RunL();
	void DoCancel();
protected:
	CImgDisplAppUi*const iAppUi; // not owned
	};


//
//  CImgDisplAppUi
//

CImgDisplAppUi::CImgDisplAppUi():
			iRotationAngle(KNoRotation),
			iDrmIntent(ContentAccess::EPeek),
			iScalingQuality(CBitmapScaler::EMinimumQuality)
	{
	}

void CImgDisplAppUi::ConstructL()
    {
	CEikAppUi::ConstructL();

    iAppView = CImgDisplayAppView::NewL(ClientRect());

	iBackgroundColor = KRgbWhite.Color16();
	(void) iAppView->SetBackgroundColor(TRgb::Color16(iBackgroundColor), ENoDrawNow);

	iWalker = CVideoWalker::NewL(this);
	User::LeaveIfError(iTimer.CreateLocal());

	// Note iStreamSeed deliberated left as 0 to give consistent behaviour

	iSaveInfo.iImageTypeUid = KImageTypeJPGUid;
	iSaveInfo.iBpp = 6;
	iSaveInfo.iColor = ETrue;
	iSaveInfo.iQualityFactor = 40;
	iSaveInfo.iSampling = 2;
	}

CImgDisplAppUi::~CImgDisplAppUi()
	{
	Cancel();

	if (iAppView)
		{
		RemoveFromStack(iAppView);
		delete iAppView;
		}
	delete iFrameImageData;
	delete iSaveUtil;
	delete iImageDisplay;
	delete iWalker;
	iTimer.Close();
	delete iDir;
	}

void CImgDisplAppUi::Cancel()
	{
	if (iWalker)
		{
		iWalker->Cancel(); // if active will callback on DoCancel()
		}
	iState = EIdle;
	}

TKeyResponse CImgDisplAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	TKeyResponse ret = EKeyWasConsumed;

	if (aType!=EEventKey)
		return ret;

	TUint code = aKeyEvent.iCode;

	switch (code)
		{
	case EKeyLeftArrow:
	case EKeyRightArrow:
	case EKeyUpArrow:
	case EKeyDownArrow:
		{
		TPoint moveBy;
		switch (code)
			{
		case EKeyLeftArrow:
			moveBy.SetXY(KButtonMoveIncr,0);
			break;
		case EKeyRightArrow:
			moveBy.SetXY(-KButtonMoveIncr,0);
			break;
		case EKeyUpArrow:
			moveBy.SetXY(0,KButtonMoveIncr);
			break;
		case EKeyDownArrow:
			moveBy.SetXY(0,-KButtonMoveIncr);
			break;
			}
		iAppView->MoveBy(moveBy);
		}
		break;
	case EKeyEscape:
		Cancel();
		ASSERT(iState==EIdle);
		iEikonEnv->BusyMsgCancel();
		iEikonEnv->InfoMsg(_L("Cancelled"));
		break;
	default:
		ret = EKeyWasNotConsumed;
		break;
		}
	return ret;
	}

void CImgDisplAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EVideoCmdOpenFile:
		OpenFileL(EFileTypeUnknown);
		break;
	case EVideoCmdOpenFolder:
		OpenFolderL();
		break;
	case EVideoCmdSaveAs:
		SaveAsL();
		break;
	case EVideoCmdMask:
		iDisableMask = ! iDisableMask;
		if (iDisableMask)
			{
			iEikonEnv->InfoMsg(_L("Mask reset"));
			}
		else
			{
			iEikonEnv->InfoMsg(_L("Mask enabled"));
			}

		break;
	case EVideoCmdToggleFrameDump:
		iDumpFrames = !iDumpFrames;
		if (iDumpFrames)
			{
			iEikonEnv->InfoMsg(_L("Frame dump is ON"));
			}
		else
			{
			iEikonEnv->InfoMsg(_L("Frame dump is OFF"));
			}
		break;
	case EVideoCmdDrmParams:
		DrmParamsL();
		break;
	case EVideoCmdClipRectParams:
		ClipRectParamsL();
		break;
	case EVideoCmdDecodeThumbnail:
		iUseThumbnail = !iUseThumbnail;
		LoadFileL();
		break;
	case EVideoCmdPauseAfterEachFrame:
		iPauseAfterEachFrame = !iPauseAfterEachFrame;
		if (iPauseAfterEachFrame)
			{
			iEikonEnv->InfoMsg(_L("Pause after each frame is ON"));
			}
		else
			{
			iEikonEnv->InfoMsg(_L("Pause after each frame is OFF"));
			}
		break;
	case EVideoCmdScalingQuality:
		ScalingQualityL();
		break;
	case EVideoCmdZoomIn:
		iZoomFactor++;
		if (iZoomFactor > KVideoMaxZoomFactor)
			iZoomFactor = KVideoMinZoomFactor;
		LoadFileL();
		break;
	case EVideoCmdZoomOut:
		iZoomFactor--;
		if (iZoomFactor < KVideoMinZoomFactor)
			iZoomFactor = KVideoMaxZoomFactor;
		LoadFileL();
		break;
	case EVideoCmdResetZoom:
		iZoomFactor=0;
		LoadFileL();
		break;
	case EVideoCmdDisplayMode:
		DisplayModeL();
		break;
	case EVideoCmdMaxReduction:
		MaxReductionL();
		break;
	case EVideoCmdBackgroundColor:
		BackgroundColorL();
		break;
	case EVideoCmdPlay:
		if (iState == ELoading)
			iEikonEnv->InfoMsg(_L("Busy..."));
		else
			PlayClipL();
		break;
	case EVideoCmdPause:
		Pause();
		break;
	case EVideoCmdRefresh:
		LoadFileL();
		break;
	case EVideoCmdExtractFrame:
		ExtractFrameL();
		break;
	case EVideoCmdFrameInfo:
		FrameInfoL(); 
		break;
	case EVideoCmdRotateClockwise:
		FrameRotateL(ETrue);
		LoadFileL();
		break;
	case EVideoCmdRotateAntiClockwise:
		FrameRotateL(EFalse);
		LoadFileL();
		break;
	case EVideoCmdFlipVertical:
		FrameFlipL(ETrue);
		LoadFileL();
		break;
	case EVideoCmdFlipHorizontal:
		FrameFlipL(EFalse);
		LoadFileL();
		break;
	case EVideoCmdAutoRotate:
		AutoRotateL();
		LoadFileL();
		break;
	case EEikCmdExit:
		Exit();
		break;
	default:
		iEikonEnv->InfoMsg(R_VIDEO_UNKNOWN);
		break;
		}
	}

TBool CImgDisplAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail)
	{
	switch (aCommand)
		{
	case EApaCommandOpen:
		iLoadFileName = aDocumentName;
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

void CImgDisplAppUi::OpenFileL(TFileType aFileType)
	{
	if (iLoadFileName.Length() < KShortestPossiblePath)
		{
		iLoadFileName = KDefPath;
		}

	Cancel();

	CEikDialog* dialog = new(ELeave) CEikFileOpenDialog(&iLoadFileName, 0);
	if (!dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		return;
		}

	iLastFileType = aFileType;
	iOpeningFolder = EFalse;
	LoadFileL();
	}

void CImgDisplAppUi::OpenFolderL()
	{
	Cancel();

	if (iLoadFileName.Length() < KShortestPossiblePath)
		iLoadFileName = KDefPath;

	TFileName folderName = iLoadFileName;

	CEikDialog* dialog = new(ELeave) CEikFileOpenDialog(&folderName, R_VIDEO_SELECT_FOLDER);
	if (!dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		return;

	iLastFileType = EFileTypeUnknown;
	iOpeningFolder = ETrue;
	LoadFolderL(folderName);
	}

void CImgDisplAppUi::LoadFileL()
	{
	if (iLoadFileName.Length() < KShortestPossiblePath)
		{
		return;
		}

	Cancel();

	TInt error=KErrNone;
	if (iImageDisplay==NULL)
		{
		TRAP(error,iImageDisplay = CImageDisplay::NewL(*this, iCoeEnv->FsSession()));
		}
	else
		{
		iImageDisplay->Reset();
		}
	iFrameNumber = 0;
	if (error!=KErrNone)
		{
		iEikonEnv->HandleError(error);
		iAppView->Reset(EDrawNow); // ensure we redraw whole screen - replace previous cross if required
		return;
		}

	HandleNewlyOpenedImageL();

	StartFrameOpen(EPlaying);
	}

void CImgDisplAppUi::LoadFolderL(const TDesC& aDirName)
	{
	delete iDir; iDir = NULL;

	TParsePtrC parse(aDirName);
	iDirName = parse.DriveAndPath();

	iEikonEnv->FsSession().GetDir(iDirName, 0, 0, iDir);

	if (iDir->Count()==0)
		{
		iEikonEnv->InfoMsg(R_VIDEO_EMPTY_FOLDER);
		return;
		}

	iDirIndex = 0; 
	OpenNextFolderEntry();
	}

void CImgDisplAppUi::OpenNextFolderEntry()
	{
	if (iDirIndex >= iDir->Count())
		{
		// end of processing - so stop
		iEikonEnv->BusyMsgCancel();
		Cancel(); 
		iState = EIdle;
		return;
		}

	const TEntry& entry = (*iDir)[iDirIndex];
	iLoadFileName.Copy(iDirName);
	iLoadFileName.Append(entry.iName);

	TRAPD(error, LoadFileL());

	if (error==KErrNone)
		iEikonEnv->InfoMsg(entry.iName);
	else
		{
		TBuf<64> errorMsg;
		errorMsg.Format(_L("%S - error %d"), &entry.iName, error);
		iEikonEnv->InfoMsg(errorMsg);
		LoadFolderWait();
		}
	}

void CImgDisplAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
	{
	if(aResourceId==R_VIDEO_CLIP_MENU)
		{
		if (iImageDisplay==NULL || iState!=EPlaying)
			{
			aMenuPane->SetItemDimmed(EVideoCmdPlay, (iImageDisplay==NULL) );
			aMenuPane->SetItemDimmed(EVideoCmdPause, (iImageDisplay==NULL || iState!=EPlaying) );
			aMenuPane->SetItemDimmed(EVideoCmdRefresh, (iImageDisplay==NULL) );
			aMenuPane->SetItemDimmed(EVideoCmdFrameInfo,(iImageDisplay==NULL ));
			}
		if (iPauseAfterEachFrame)
			{
			aMenuPane->SetItemButtonState(EVideoCmdPauseAfterEachFrame, EEikMenuItemSymbolOn);
			}
		if (iDumpFrames)
			{
			aMenuPane->SetItemButtonState(EVideoCmdToggleFrameDump, EEikMenuItemSymbolOn);
			}
		}
	else if (aResourceId==R_VIDEO_VIEW_MENU)
		{
		
		switch (iRotationAngle)
			{
			case  CBitmapRotator::ERotation90DegreesClockwise:
				aMenuPane->SetItemButtonState(EVideoCmdRotateClockwise,EEikMenuItemSymbolOn);
				break;
			case  CBitmapRotator::ERotation270DegreesClockwise:
				aMenuPane->SetItemButtonState(EVideoCmdRotateAntiClockwise,EEikMenuItemSymbolOn);
				break;
			case  CBitmapRotator::EMirrorHorizontalAxis:
				aMenuPane->SetItemButtonState(EVideoCmdFlipVertical,EEikMenuItemSymbolOn);
				break;
			case  CBitmapRotator::EMirrorVerticalAxis:
				aMenuPane->SetItemButtonState(EVideoCmdFlipHorizontal,EEikMenuItemSymbolOn);
				break;
			case KAutoRotate:
				aMenuPane->SetItemButtonState(EVideoCmdAutoRotate,EEikMenuItemSymbolOn);
				break;
			}
		if (iUseThumbnail)
			{
			aMenuPane->SetItemButtonState(EVideoCmdDecodeThumbnail, EEikMenuItemSymbolOn);
			}
			
		if (iImageDisplay==NULL || iState!=EPlaying)
			{
			
			}
		}
	else if (aResourceId==R_OPTIONS_MENU)
		{
		}
	}

TBool CImgDisplAppUi::CheckHotKeyNotDimmedL(TInt aCommandId)
	{
	TInt result = ETrue;
	switch (aCommandId)
		{
		case EVideoCmdRefresh:
			result = (iImageDisplay!=NULL);
			break;
		case EVideoCmdPause:
			result = (iImageDisplay!=NULL && iState==EPlaying);
			break;
		case EVideoCmdPlay:
			result = (iImageDisplay!=NULL && iState!=EPlaying);
			break;
		case EVideoCmdFrameInfo:
			result = (iImageDisplay!=NULL);
			break;
		}

	return result;
	}

void CImgDisplAppUi::LoadFolderWait()
	{
	ASSERT(!iWalker->IsActive()); // if we get here, should not be doing anything
	
	iTimer.After(iWalker->ActiveStatus(), KDefaultFolderDelay);
	iState = EFolderWait;
	}

TUint CImgDisplAppUi::ImageOptions() const
	{
	TUint options=0;
	switch (iRotationAngle)
		{
		case  CBitmapRotator::ERotation90DegreesClockwise:
			options |= CImageDisplay::EOptionRotateCw90;
		break;
		case  CBitmapRotator::ERotation180DegreesClockwise:
			options |= CImageDisplay::EOptionRotateCw180;
			break;
		case  CBitmapRotator::ERotation270DegreesClockwise:
			options |= CImageDisplay::EOptionRotateCw270;
			break;
		case  CBitmapRotator::EMirrorHorizontalAxis:
			options |= CImageDisplay::EOptionMirrorHorizontal;
			break;
		case  CBitmapRotator::EMirrorVerticalAxis:
			options |= CImageDisplay::EOptionMirrorVertical;
			break;
		case  KAutoRotate:
			options |= CImageDisplay::EOptionAutoRotate;
			break;
		}
	if (iUseThumbnail)
		{
		options |= CImageDisplay::EOptionThumbnail;
		}
	else
		{
		options |= CImageDisplay::EOptionMainImage;
		}
	return options;
	}

void CImgDisplAppUi::HandleNewlyOpenedImageL()
	{
	ASSERT(iImageDisplay); // should have already been opened

	iImageDisplay->Reset();
	iImageDisplay->SetImageSource(iDrmContentId.Length()==0? 
									TMMFileSource(iLoadFileName):
									TMMFileSource(iLoadFileName, iDrmContentId, iDrmIntent)
								);
	iImageDisplay->SetOptions(ImageOptions());
	iImageDisplay->SetDisplayMode( iAppView->DisplayMode() );
	if (iClippingSet)
		{
		iImageDisplay->SetSourceRect(iClipRect);
		}
	//
	iImageDisplay->SetupL();
	ASSERT( iImageDisplay->RecommendedImageSizes().Count() >0 ); // the mustn't be empty
	TSize original(iImageDisplay->RecommendedImageSizes()[0]);
	if (iZoomFactor>0)
		{
		iOverallSize.SetSize(original.iWidth<<iZoomFactor, original.iHeight<<iZoomFactor);
		}
	else
		{
		const TInt zoom=-iZoomFactor;
		if (iImageDisplay->RecommendedImageSizes().Count() > zoom)
			{
			iOverallSize = iImageDisplay->RecommendedImageSizes()[zoom];
			}
		else
			{
			iOverallSize.SetSize(original.iWidth>>zoom, original.iHeight>>zoom);
			}
		}
	
	MGenIclImageDisplayExtension* iFace;
	if (KErrNone == iImageDisplay->ExtensionInterface(KUidGenIclImageDisplayPluginExtUid, reinterpret_cast<TAny*&>(iFace)))
		{
		TInt err=iFace->SetScaleQuality(iScalingQuality);
		ASSERT(err==KErrNone);
		err=iFace->SetMaximumReductionFactor(iMaxReduction);
		ASSERT(err==KErrNone);
		iFace->Release();
		}
	iImageDisplay->SetSizeInPixels(iOverallSize);
	iFrameNumber	= 0;
	iViewResized	= EFalse;

	iTime.HomeTime();
	}

void CImgDisplAppUi::MakeName(const TDesC& aDir, TBool aIsMask, TDes& aFileName) const
	{
	TParse parse;
	parse.SetNoWild(iLoadFileName, NULL, NULL);
	aFileName.Copy(aDir);
	aFileName.Append(parse.Name());
	if (aIsMask)
		{
		_LIT(KMask,"_mask_");
		aFileName.Append(KMask);
		}
	else
		{
		_LIT(KFrame,"_frame_");
		aFileName.Append(KFrame);
		}
	aFileName.AppendNum(iFrameNumber);
	_LIT(KExt, ".mbm");
	aFileName.Append(KExt);
	}

void CImgDisplAppUi::MiidoImageReady(const CFbsBitmap* /*aBitmap*/, TUint aStatus, const TRect& /*aUpdatedArea*/, TInt aError)
	{
	iLatestError	= aError;
	iLatestStatus	= aStatus;
	if (aError!=KErrNone)
		{
		TBuf<64> err;
		err.Format(_L("Play error: %d"),aError);
		User::InfoPrint(err);
		iState = EIdle;
		return;
		}
	if (aStatus&CImageDisplayPlugin::EStatusFrameReady)
		{
		++iFrameNumber;
		iImageDisplay->GetBitmap(iFrame, iMask);
		if (iDumpFrames)
			{
			_LIT(KDumpDir, "c:\\temp\\");
			iEikonEnv->FsSession().MkDirAll(KDumpDir);
			TFileName nm;
			MakeName(KDumpDir, EFalse, nm);
			const_cast<CFbsBitmap*>(iFrame)->Save(nm);
			if (iMask && !iDisableMask)
				{
				MakeName(KDumpDir, ETrue, nm);
				const_cast<CFbsBitmap*>(iMask)->Save(nm);
				}
			}
		const TSize frameSize(iFrame->SizeInPixels());
		if (!iViewResized )
			{
			TRAPD(error, iAppView->ResizeL(frameSize) );
			iViewResized = ETrue;
			TBuf<64> err;
			err.Format(_L("Resize error: %d"),error);
			if (error != KErrNone)
				{
				User::InfoPrint(err);
				}
			iAppView->Center(ENoDrawNow);
			}

		DrawConvertedFrame();
		}
	else
		{
		TBuf<64> err;
		err.Format(_L("Strange status: %x"),aStatus);
		User::InfoPrint(err);
		iState = EIdle;
		return;
		}
	if ((aStatus&CImageDisplayPlugin::EStatusNoMoreToDecode)==0)
		{
		if (iPauseAfterEachFrame)
			{
			User::InfoPrint(_L("Paused"));
			iState = EPlayPaused;
			}
		else
			{
			iImageDisplay->Play();
			}
		}
	else
		{
		User::InfoPrint(_L("Completed"));
		iState = EIdle;
		}
	}

void CImgDisplAppUi::ExtractFrameL()
	{
#if 0
	if (iLoadFileName.Length() < KShortestPossiblePath)
		return;

	ASSERT(iImageDisplay);

	Cancel();

	TInt frameCount = iImageDisplay->FrameCount();

	if (iFrameNumber >= frameCount)
		iFrameNumber = 0;

	CEikDialog* dialog = new(ELeave) CVideoCurrentFrameDialog(iFrameNumber,frameCount);
	if (!dialog->ExecuteLD(R_VIDEO_FRAME_DIALOG))
		return;

	StartFrameOpen(ELoading);
#endif
	}

void CImgDisplAppUi::StartFrameOpen(TState aNextState)
	{
	iImageDisplay->Play();
	iState = aNextState; // what ever happens we go to next state, and will handle any error there
	}

void CImgDisplAppUi::PlayClipL()
	{
	if (iState != EPlayPaused)
		{
		iFrameNumber = 0;
		}
	iTime.HomeTime();

	StartFrameOpen(EPlaying);
	}

void CImgDisplAppUi::Pause()
	{
	if (iImageDisplay && iState == EPlaying)
		{
		iImageDisplay->Pause();
		iState = EPlayPaused;
		}
	}

void CImgDisplAppUi::SaveAsL()
	{
	if (iSaveFileName.Length() < KShortestPossiblePath)
		iSaveFileName = KDefPath;

	Cancel();

	CEikDialog* dialog = new(ELeave) CVideoSaveAsDialog(&iSaveFileName,iSaveInfo);
	if (!dialog->ExecuteLD(R_VIDEO_FILE_SAVEAS_DIALOG))
		return;
	iEikonEnv->FsSession().Delete(iSaveFileName);

	delete iFrameImageData; iFrameImageData = NULL;

	const TUid imageType = iSaveInfo.iImageTypeUid;
	TImageDataBlock *imageData = NULL;
	TFrameDataBlock *frameData = NULL;

	if(imageType == KImageTypeBMPUid)
		{
		imageData = new (ELeave) TBmpImageData;
		TBmpImageData* data = STATIC_CAST(TBmpImageData*, imageData);
		switch (iSaveInfo.iBpp)
			{
		case 0:		data->iBitsPerPixel = 1; break;
		case 2:		data->iBitsPerPixel = 4; break;
		case 3:		data->iBitsPerPixel = 8; break;
		case 6:		data->iBitsPerPixel = 24; break;
		default:	ASSERT(EFalse);			break;
			}
		}
	else if(imageType == KImageTypeGIFUid)
		{// gif does not have encoding options
		}
	else if(imageType == KImageTypeJPGUid)
		{
		imageData = new (ELeave) TJpegImageData;
		TJpegImageData* data = STATIC_CAST(TJpegImageData*, imageData);
		if (!iSaveInfo.iColor)
			data->iSampleScheme = TJpegImageData::EMonochrome;
		else
			data->iSampleScheme = TJpegImageData::TColorSampling(3 - iSaveInfo.iSampling);
		data->iQualityFactor = iSaveInfo.iQualityFactor;
		}
	else if(imageType == KImageTypeMBMUid)
		{
		frameData = new (ELeave) TMbmEncodeData;
		TMbmEncodeData* data = STATIC_CAST(TMbmEncodeData*, frameData);
		switch (iSaveInfo.iBpp)
			{
		case 0:		data->iDisplayMode = EGray2;		break;
		case 1:		data->iDisplayMode = EGray4;		break;
		case 2:		data->iDisplayMode = iSaveInfo.iColor ? EColor16 : EGray16;	break;
		case 3:		data->iDisplayMode = iSaveInfo.iColor ? EColor256 : EGray256;	break;
		case 4:		data->iDisplayMode = EColor4K;		break;
		case 5:		data->iDisplayMode = EColor64K;	break;
		case 6:		data->iDisplayMode = EColor16M;	break;
		case 7:		data->iDisplayMode = EColor16MU;	break;
		default:	ASSERT(EFalse);	break;
			}
		}
	else if(imageType == KImageTypePNGUid)
		{
		frameData = new (ELeave) TPngEncodeData;
		TPngEncodeData* data = STATIC_CAST(TPngEncodeData*, frameData);
		// bpp
		switch (iSaveInfo.iBpp)
			{
			case 0:		data->iBitsPerPixel = 1; break;		// 1 bpp 
			case 1:		data->iBitsPerPixel = 2; break;		// 2 bpp
			case 2:		data->iBitsPerPixel = 4; break;		// 4 bpp
			case 3:		data->iBitsPerPixel = 8; break;		// 8 bpp
			case 6:		data->iBitsPerPixel = 24; break;	// 24 bpp
			default:	ASSERT(EFalse);	break;				// unsupported bit depth
			}
		// colour or grayscale?
		data->iColor = iSaveInfo.iColor;
		// compression level
		switch (iSaveInfo.iCompression)
			{
			case 0:
				data->iLevel = TPngEncodeData::EDefaultCompression;
				break;
			case 1:
				data->iLevel = TPngEncodeData::ENoCompression;
				break;
			case 2:
				data->iLevel = TPngEncodeData::EBestSpeed;
				break;
			case 3:
				data->iLevel = TPngEncodeData::EBestCompression;
				break;
			default:			// unsupported compression
				ASSERT(EFalse);
				break;		
			}
		}
	else
		{
		//custom encoder
		}

	if (frameData)
		CleanupStack::PushL(frameData);

	if (imageData)
		CleanupStack::PushL(imageData);

	delete iSaveUtil; iSaveUtil = NULL;
	iSaveUtil = CImageEncoder::FileNewL(iCoeEnv->FsSession(), iSaveFileName, CImageEncoder::EOptionNone, imageType);

	ASSERT(iFrameImageData==NULL); // deleted above
	if (imageData)
		{
		iFrameImageData = CFrameImageData::NewL();

		User::LeaveIfError(iFrameImageData->AppendImageData(imageData));
		CleanupStack::Pop(); // imageData - ownership now passed to iFrameImageData
		}

	if (frameData)
		{
		if (iFrameImageData == NULL)
			iFrameImageData = CFrameImageData::NewL();

		User::LeaveIfError(iFrameImageData->AppendFrameData(frameData));
		CleanupStack::Pop(); // frameData - ownership now passed to iFrameImageData
		}

	StartFrameSave();
	}

void CImgDisplAppUi::DisplayModeL()
	{
	TDisplayMode displayMode = iAppView->DisplayMode();
	CEikDialog* dialog = new(ELeave) CDisplayModeDialog(displayMode);
	if (dialog->ExecuteLD(R_VIDEO_DISPLAY_MODE_DIALOG))
		{
		iAppView->SetDisplayModeL(displayMode);
		LoadFileL(); // redraw
		}
	}

void CImgDisplAppUi::ScalingQualityL()
	{
	CEikDialog* dialog = new(ELeave) CScalingQualityDialog(iScalingQuality);
	if (dialog->ExecuteLD(R_SCALING_QUALITY_DIALOG))
		{
		LoadFileL(); // redraw
		}
	}

void CImgDisplAppUi::DrmParamsL()
	{
	CEikDialog* dialog = new(ELeave) CDrmContentIdDialog(iDrmContentId);
	dialog->ExecuteLD(R_DRM_CONTENT_PARAMS_DIALOG);
	}

void CImgDisplAppUi::MaxReductionL()
	{
	CEikDialog* dialog = new(ELeave) CMaxReductionDialog(iMaxReduction);
	dialog->ExecuteLD(R_IMAGE_MAX_REDUCTION_FACTOR);
	}

void CImgDisplAppUi::ClipRectParamsL()
	{
	CEikDialog* dialog = new(ELeave) CClipRectParamsDialog(iClippingSet, iClipRect);
	dialog->ExecuteLD(R_IMAGE_CLIP_RECT_DIALOG);
	}

void CImgDisplAppUi::BackgroundColorL()
	{
	TInt backgroundColor = iBackgroundColor;
	TBool override = iOverrideBackgroundColor;
	CEikDialog* dialog = new(ELeave) CVideoBackgroundColorDialog(backgroundColor, override);
	if (dialog->ExecuteLD(R_VIDEO_BACKGROUND_COLOR_DIALOG))
		{
		iBackgroundColor = backgroundColor;
		iOverrideBackgroundColor = override;
		LoadFileL(); // redraw
		}
	}

void CImgDisplAppUi::FrameInfoL()
	{
	HBufC* info=HBufC::NewLC(KInfoBufferSize);
	TPtr commentTPtr(info->Des());

	_LIT(KImgStatus, "Img status value=0x%x (");
	TUint status=iImageDisplay->ImageStatus();
	commentTPtr.AppendFormat(KImgStatus, status);
	if (status&CImageDisplay::EImageSingleFrame)
		{
		commentTPtr.Append(_L("EImageSingleFrame "));
		}
	if (status&CImageDisplay::EImageMultiFrame)
		{
		commentTPtr.Append(_L("EImageMultiFrame "));
		}
	if (status&CImageDisplay::EImageAnimated)
		{
		commentTPtr.Append(_L("EImageAnimated "));
		}
	if (status&CImageDisplay::EImageMasked)
		{
		commentTPtr.Append(_L("EImageMasked "));
		}
	if (status&CImageDisplay::EImageHasThumbnail)
		{
		commentTPtr.Append(_L("EImageHasThumbnail "));
		}
	if (status&CImageDisplay::EImageTypeUnknown)
		{
		commentTPtr.Append(_L("EImageTypeUnknown "));
		}
	commentTPtr.Append(_L(")\n"));

	TInt numFrames;
	TInt err=iImageDisplay->NumFrames(numFrames);
	_LIT(KImgNumFrames, "Num Frames=%d (err=%d)\n");
	commentTPtr.AppendFormat(KImgNumFrames,numFrames,err);
	commentTPtr.Append(_L("---------\n"));
	commentTPtr.AppendFormat(_L("Frame number %d\n"),iFrameNumber);
	if (iFrame)
		{
		commentTPtr.AppendFormat(_L("Frame size %d x %d\n"),iFrame->SizeInPixels().iWidth,iFrame->SizeInPixels().iHeight);
		}
	commentTPtr.AppendFormat(_L("Latest status 0x%x\n"),iLatestStatus);
	commentTPtr.AppendFormat(_L("Latest error %d\n"),iLatestError);
	if (iState == EPlaying)
		{
		if (iImageDisplay->ValidBitmap())
			{
			commentTPtr.Append(_L("Bitmap status: Valid\n"));
			}
		else
			{
			commentTPtr.Append(_L("Bitmap status: NOT Valid\n"));
			}
		}
	
	ReplaceNewlinesWithLineBreaks(commentTPtr);
	TBool needPlay= (iState == EPlaying);
	Pause();	

	CEikDialog* dialog = new(ELeave) CFrameInfoDialog(commentTPtr);
	TRAP(err, dialog->ExecuteLD(R_VIDEO_INFO_DIALOG) );
	CleanupStack::PopAndDestroy(); //info
	if (needPlay)
		{
		PlayClipL();
		}
	}

void CImgDisplAppUi::FrameRotateL(TBool aClockwise)
	{
	TInt newAngle= aClockwise ?
			CBitmapRotator::ERotation90DegreesClockwise :
			CBitmapRotator::ERotation270DegreesClockwise;
	if (iRotationAngle  == newAngle )
		{
		iRotationAngle = KNoRotation;
		}
	else
		{
		iRotationAngle  = newAngle;
		}

	iViewResized = EFalse;
	}

void CImgDisplAppUi::FrameFlipL(TBool aVertical)
	{
	TInt newAngle=aVertical ?
						CBitmapRotator::EMirrorHorizontalAxis :
						CBitmapRotator::EMirrorVerticalAxis;

	if (iRotationAngle  == newAngle )
		{
		iRotationAngle = KNoRotation;
		}
	else
		{
		iRotationAngle  = newAngle;
		}
	iViewResized = EFalse;
	}
void CImgDisplAppUi::AutoRotateL()
	{
	if (iRotationAngle  == KAutoRotate )
		{
		iRotationAngle  = KNoRotation;
		}
	else
		{
		iRotationAngle  = KAutoRotate;
		}
	iViewResized = EFalse;
	}

void CImgDisplAppUi::StartFrameSave()
	{
	ASSERT(iSaveUtil); // should be true by now
	ASSERT(iState==EIdle); 
	ASSERT(!iWalker->IsActive()); // we are going to use it
#if 0
	iSaveUtil->Convert(&(iWalker->ActiveStatus()),iFrame,iFrameImageData); 
#endif
	iState = ESaving;
	}

void CImgDisplAppUi::DrawConvertedFrame()
	{
	if (iOverrideBackgroundColor)
		{
		iAppView->SetBackgroundColor(TRgb(iBackgroundColor), ENoDrawNow);
		}
	TBool fullRedrawReq=ETrue;
	if (iMask==NULL || iDisableMask)
		iAppView->DrawImage(iFrame, iOffset, fullRedrawReq?ENoDrawNow:EDrawNow);
	else
		iAppView->DrawImage(iFrame, iMask, iOffset, fullRedrawReq?ENoDrawNow:EDrawNow);

	if (fullRedrawReq)
		{
		iAppView->DrawNow();
		}
	}

void CImgDisplAppUi::HandleConvertCompleteL()
	{
	DrawConvertedFrame();

	iPrevFrameNumber = iFrameNumber;

	if (iState==EPlaying)
		{
		ASSERT(!iWalker->IsActive()); // we will use it again

		if (iState==EPlaying)
			{
			iState = EPlayingWait;
			}
			
		else 
			{
			ASSERT(EFalse);
			}
		}
	else if (iOpeningFolder)
		{
		LoadFolderWait();
		}
	else
		{
		iEikonEnv->BusyMsgCancel();
		Cancel(); // cancel any background streaming
		iState = EIdle;
		}
	}

void CImgDisplAppUi::RunL(CVideoWalker* aWalker, TInt aStatus)
	{
	ASSERT(aWalker==iWalker); // ones we know about

#if defined(_DEBUG) && defined(__ENABLE_DEBUG_OUTPUT)
	RDebug::Print(_L("RunL(%x,%d,%d)(%x,%x)"), aWalker, aStatus, iState, iWalker, iStreamGen);
#endif 

	switch (iState)
		{
	case EIdle:
		ASSERT(EFalse); // should not happen
		break;
	case ELoading:
	case EPlaying:
		{
		ASSERT(aWalker==iWalker);
		TInt error = aStatus;
		if (error==KErrNone)
			TRAP(error, HandleConvertCompleteL());
		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
		break;
	case EPlayingWait:
		{
		ASSERT(iWalker==aWalker);
		iTime.HomeTime(); // whatever reset the time here
		if (aStatus!=KErrNone)
			{
			HandleRunError(aWalker, aStatus);
			}
		else if (iOpeningFolder)
			{
			LoadFolderWait();
			}
		else
			{
			ASSERT(!iWalker->IsActive()); // should have stopped naturally
			iEikonEnv->BusyMsgCancel();
			iState = EIdle;
			}
		}
		break;
	case ESaving:
		{
		ASSERT(iWalker==aWalker);
		iEikonEnv->BusyMsgCancel();
		iState = EIdle;
		if (aStatus!=KErrNone)
			HandleRunError(aWalker, aStatus);
		}
		break;
	case EFolderWait:
		iDirIndex++;
		OpenNextFolderEntry();
		break;
	default:
		ASSERT(EFalse); // unknown state
		}
	}

void CImgDisplAppUi::DoCancel(CVideoWalker *aWalker)
	{
	ASSERT(aWalker==iWalker); // only ones we know about

#if defined(_DEBUG) && defined(__ENABLE_DEBUG_OUTPUT)
	RDebug::Print(_L("DoCancel(%x,%d)(%x,%x)"), aWalker, iState, iWalker, iStreamGen); 
#endif 

	if (aWalker==iWalker)
		{
		switch (iState)
			{
		case ELoading:
		case EPlaying:
			iImageDisplay->StopPlay();
			break;
		case ESaving:
			iSaveUtil->Cancel();
			break;
		case EPlayingWait: // ignore scenario where we self complete - will not get here
		case EFolderWait:
			iTimer.Cancel();
			break;
		default:
			ASSERT(EFalse); // unknown state or should not happen
			}
		}
	else
		{
		ASSERT(EFalse); // unknown state or should not happen
		}
	}

void CImgDisplAppUi::HandleRunError(CVideoWalker* /*aWalker*/, TInt aError)
	{
	Cancel();
	iEikonEnv->BusyMsgCancel();
	iEikonEnv->HandleError(aError);
	ASSERT(iState == EIdle);
	}

void CImgDisplAppUi::ReplaceNewlinesWithLineBreaks(TDes& aText)
	{
	TInt pos=0;

	for (;;)
		{
		if (pos >= aText.Length()) // will occur if last character in comment is new line
			break;
		const TPtrC restOfText(aText.Mid(pos));
		TInt posOfNextNewLine = restOfText.Locate(TChar('\n'));
		if (posOfNextNewLine<0) // no more new lines in text
			break;
		posOfNextNewLine += pos; // position relative to whole descriptor
		aText[posOfNextNewLine] = CEditableText::ELineBreak;
		pos = posOfNextNewLine + 1; // next cycle, start at next character
		}
	}


//
// CVideoWalker implementation
//

CVideoWalker* CVideoWalker::NewL(CImgDisplAppUi* aAppUi)
	{
	CVideoWalker* result = new (ELeave) CVideoWalker(aAppUi);
	return result;
	}

CVideoWalker::CVideoWalker(CImgDisplAppUi* aAppUi):
	CActive(CActive::EPriorityStandard),
	iAppUi(aAppUi)
	{
	CActiveScheduler::Add(this);
	}

CVideoWalker::~CVideoWalker()
	{
	Cancel();
	}

// used when we need to pass status variable - simultaneously calls SetActive
TRequestStatus& CVideoWalker::ActiveStatus()
	{
	SetActive();
	return iStatus;
	}

// for completion ASAP on this
void CVideoWalker::SelfComplete(TInt aError)
	{
	TRequestStatus* status = &ActiveStatus();
	User::RequestComplete(status, aError);
	}

// calls AppUi->RunL() which is expected to handle the call
void CVideoWalker::RunL()
	{
	iAppUi->RunL(this, iStatus.Int());
	}

// calls AppUi->DoCancel() which is expected to handle the call
void CVideoWalker::DoCancel()
	{
	iAppUi->DoCancel(this);
	}

//
// CImgDisplDocument
//

CImgDisplDocument::CImgDisplDocument(CEikApplication& aApp)
		: CEikDocument(aApp)
	{
	}

CEikAppUi* CImgDisplDocument::CreateAppUiL()
	{
    return new(ELeave) CImgDisplAppUi;
	}

//
// CImgDisplApp
//

TUid CImgDisplApp::AppDllUid() const
	{
	return KUidTVideo;
	}

CApaDocument* CImgDisplApp::CreateDocumentL()
	{
	return new(ELeave) CImgDisplDocument(*this);
	}

#include <eikstart.h>


CApaApplication* NewApplication()
   {
   return new CImgDisplApp;
   }

TInt E32Main()
   {
   return EikStart::RunApplication(NewApplication);
   }


