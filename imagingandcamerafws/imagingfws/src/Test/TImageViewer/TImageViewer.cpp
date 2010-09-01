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
#include <bidi.h>
#include <eikenv.h>
#include <techview/eikchlst.h>
#include <techview/eikinfo.h>
#include <techview/eikmenup.h>
#include <e32math.h>
#include <badesca.h>
#include <techview/eikon.rsg>	
#include <eikpriv.rsg>
#include <techview/eiktxlbx.h>
#include <techview/eikmfne.h>
#include <techview/eiktxlbm.h>
#include <iclexif.h>

#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "TImageViewer.h"

const TInt KVideoMinZoomFactor = -3;
const TInt KVideoMaxZoomFactor = 0;

const TInt KDefaultStreamDelay = 1000000; // 1s. Used when streaming non-animated
const TInt KDefaultFolderDelay = 2000000; // 2s. Used when playing a folder

const TInt KButtonMoveIncr = 16;
const TInt KScaleFactor = 10;

const TInt KInfoBufferSize = 1024;

_LIT(KDefPath, "C:\\");
const TInt KShortestPossiblePath=4;

// #define __USE_PURE_SCALING // fine scaling scales to 1/2, 1/4 and 1/8 - check non-universal scaling

#define __CLEAR_BITMAPS_FIRST // for debugging purposes, get TImageViewer to clear bitmaps prior to
							  // decode conversion. Really the codecs should do this, but...

inline TBool IsMngImage(const CImageDecoder* aDecoder)
	{
	TUid type, subtype;
	aDecoder->ImageType(0, type, subtype);
	return (type.iUid==KMngMimeTypeUidValue);
	}

inline TBool HasMoreMngFrames(const CImageDecoder* aDecoder)
	{
	const TFrameInfo& info=aDecoder->FrameInfo(0);
	return (info.iFlags & TFrameInfo::EMngMoreFramesToDecode);
	}

CPluginInfoArray* CPluginInfoArray::NewL()
	{
	CPluginInfoArray* self = new (ELeave) CPluginInfoArray;
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
	CFrameInfoDialog(const TDesC& aPropertiesText, const TDesC& aImageCommentText, const TDesC& aFrameCommentText);

protected:
	void PreLayoutDynInitL();

private:
	const TDesC& iPropertiesText;
	const TDesC& iImageCommentText;
	const TDesC& iFrameCommentText;
	};

CFrameInfoDialog::CFrameInfoDialog(const TDesC& aPropertiesText, const TDesC& aImageCommentText, const TDesC& aFrameCommentText)
	: iPropertiesText(aPropertiesText), iImageCommentText(aImageCommentText), iFrameCommentText(aFrameCommentText)
	{
	}

void CFrameInfoDialog::PreLayoutDynInitL()
	{
	// Grab each Edwin and set the text.
	CEikEdwin* page = STATIC_CAST(CEikEdwin*, Control(EFramePropertiesPage));
	page->SetTextL(&iPropertiesText);

	page = STATIC_CAST(CEikEdwin*, Control(EImageCommentsPage));
	page->SetTextL(&iImageCommentText);

	page = STATIC_CAST(CEikEdwin*, Control(EFrameCommentsPage));
	page->SetTextL(&iFrameCommentText);
	}

//
// CVideoWalker
//

class CVideoWalker : public CActive
	{
public:
	static CVideoWalker* NewL(CVideoAppUi* aAppUi);
	~CVideoWalker();
	TRequestStatus& ActiveStatus();
	void SelfComplete(TInt aError);
protected:
	CVideoWalker(CVideoAppUi* aAppUi);
	// from CActive
	void RunL();
	void DoCancel();
protected:
	CVideoAppUi* const iAppUi; // not owned
	};


//
//  CVideoAppUi
//

CVideoAppUi::CVideoAppUi():
	iStreamBuffer(NULL,0,0)
	{
	iDecoderOptions = (CImageDecoder::EAllowGeneratedMask | CImageDecoder::EPreferFastDecode);
	}

void CVideoAppUi::ConstructL()
    {
	CEikAppUi::ConstructL();

    iAppView = CVideoAppView::NewL(ClientRect());

	::new(&iFrame) CWsBitmap(iEikonEnv->WsSession());
	::new(&iMask) CWsBitmap(iEikonEnv->WsSession());

	iRotator = CBitmapRotator::NewL();
	iScaler = CBitmapScaler::NewL();

	iBackgroundColor = KRgbWhite.Color16();

	iAppView->SetBackgroundColor(TRgb::Color16(iBackgroundColor), ENoDrawNow);

	iWalker = CVideoWalker::NewL(this);
	User::LeaveIfError(iTimer.CreateLocal());
	iStreamGen = CVideoWalker::NewL(this);
	User::LeaveIfError(iStreamTimer.CreateLocal());

	// Note iStreamSeed deliberated left as 0 to give consistent behaviour

	iSaveInfo.iImageTypeUid = KImageTypeJPGUid;
	iSaveInfo.iBpp = 6;
	iSaveInfo.iColor = ETrue;
	iSaveInfo.iQualityFactor = 40;
	iSaveInfo.iSampling = 2;
	
	iUseNativeDisplayMode = ETrue;
	
	iScalingCoefficient = 1; // Full size
	}

CVideoAppUi::~CVideoAppUi()
	{
	Cancel();

	iFrame.Reset();
	iMask.Reset();
	if (iAppView)
		{
		RemoveFromStack(iAppView);
		delete iAppView;
		}
	delete iFrameImageData;
	delete iLoadUtil;
	delete iSaveUtil;
	delete iRotator;
	delete iScaler;
	delete iWalker;
	iTimer.Close();
	iStreamTimer.Close();
	delete iStreamGen;
	User::Free(REINTERPRET_CAST(TAny*,CONST_CAST(TUint8*, iStreamBuffer.Ptr())));
	delete iDir;
	iOperations.Close();
	}

void CVideoAppUi::Cancel()
	{
	if (iWalker)
		iWalker->Cancel(); // if active will callback on DoCancel()
	if (iStreamGen)
		iStreamGen->Cancel();
	iState = EIdle;
	}

TKeyResponse CVideoAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
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
		if ( aKeyEvent.iModifiers & EModifierShift )
			{
			// update crop region
			UpdateClippingRect(code);
			}
		else
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

void CVideoAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EVideoCmdOpenFile:
		OpenFileL(EFileTypeUnknown);
		break;
	case EVideoCmdOpenOtaFile:
		OpenFileL(EFileTypeOta);
		break;
	case EVideoCmdOpenWbmpFile:
		OpenFileL(EFileTypeWbmp);
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
		if (iMask.Handle() && iDisableMask)
			{
			iMask.Reset();
			}
		else if (iMask.Handle()==0 && !iDisableMask)
			{
			User::LeaveIfError(iMask.Create(TSize(0,0),EGray256));
			}
		break;
	case EVideoCmdZoomIn:
		iZoomFactor++;
		if (iZoomFactor > KVideoMaxZoomFactor)
			iZoomFactor = KVideoMinZoomFactor;
		if ( iUseExtensions )
			{
			iScalingCoefficient = iZoomFactor - 1;
			}
		LoadFileL();
		break;
	case EVideoCmdZoomOut:
		iZoomFactor--;
		if (iZoomFactor < KVideoMinZoomFactor)
			iZoomFactor = KVideoMaxZoomFactor;
		if ( iUseExtensions )
			{
			iScalingCoefficient = iZoomFactor - 1;
			}
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
			iEikonEnv->InfoMsg(_L("Busy..."));
		else
			PlayClipL();
		break;
	case EVideoCmdStream:
		StreamPlayL(EFileTypeUnknown);
		break;
	case EVideoCmdOtaStream:
		StreamPlayL(EFileTypeOta);
		break;
	case EVideoCmdWbmpStream:
		StreamPlayL(EFileTypeWbmp);
		break;
	case EVideoCmdDecoderOptions:
		DecoderOptionsL();
		break;
	case EVideoCmdExtension:
		ExtensionOptionsL();
		break;
	case EVideoCmdUseExtensions:
	    iUseExtensions = !iUseExtensions;
		break;
	case EVideoCmdRefresh:
		LoadFileL();
		break;
	case EVideoCmdExtractFrame:
		ExtractFrameL();
		break;
	case EVideoCmdExtractStreamedFrame:
		StreamPlayL(EFileTypeUnknown, ETrue);
		break;
	case EVideoCmdFrameInfo:
		FrameInfoL(); 
		break;
	case EVideoCmdRotateClockwise:
		FrameRotateL(ETrue);
		break;
	case EVideoCmdRotateAntiClockwise:
		FrameRotateL(EFalse);
		break;
	case EVideoCmdMirrorHorizontalAxis:
		FrameMirrorL(ETrue);
		break;
	case EVideoCmdMirrorVerticalAxis:
		FrameMirrorL(EFalse);
		break;
	case EEikCmdExit:
		Exit();
		break;
	default:
		iEikonEnv->InfoMsg(R_VIDEO_UNKNOWN);
		break;
		}
	}

TBool CVideoAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail)
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

void CVideoAppUi::OpenFileL(TFileType aFileType)
	{
	if (iLoadFileName.Length() < KShortestPossiblePath)
		iLoadFileName = KDefPath;

	Cancel();
	delete iLoadUtil; iLoadUtil = NULL;

	TInt title;
	switch (aFileType)
		{
		default:
			title = 0;
			break;
		case EFileTypeWbmp:
			title = R_VIDEO_OPEN_WBMP;
			break;
		case EFileTypeOta:
			title = R_VIDEO_OPEN_OTA;
			break;
		}
	CEikDialog* dialog = new(ELeave) CEikFileOpenDialog(&iLoadFileName, title);
	if (!dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		return;

	iLastFileType = aFileType;
	iOpeningFolder = EFalse;
	LoadFileL();
	}

void CVideoAppUi::OpenFolderL()
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
	
void CVideoAppUi::DecoderOptionsL()
	{
	CEikDialog* dialog = new(ELeave) CDecoderOptionsDialog(iDecoderOptions);
	dialog->ExecuteLD(R_VIDEO_DECODER_OPTIONS_DIALOG);
	}

void CVideoAppUi::ExtensionOptionsL()
	{
	CEikDialog* dialog = new(ELeave) CExtensionOptionsDialog(iClippingRect,
															 iOperations,
															 iScalingCoefficient,
															 iScalingQuality,
															 iLockAspectRatio);
	TInt buttonPressed = dialog->ExecuteLD(R_VIDEO_DECODER_EXTENSION_DIALOG);
	if ( buttonPressed == EEikBidOk && iLoadUtil )
		{
		// re-convert the image e.g. if clipping rect has changed
		LoadFileL();
		}
	}
	
void CVideoAppUi::LoadFileL()
	{
	Cancel();
	if ( !iLoadUtil )
		{
		if (iLoadFileName.Length() < KShortestPossiblePath)
			return;

		TUid format;
		switch (iLastFileType)
			{
			case EFileTypeOta:
				format = KImageTypeOTAUid;
				break;
			case EFileTypeWbmp:
				format = KImageTypeWBMPUid;
				break;
			default:
				format = KNullUid;
			}

		TRAPD(error,iLoadUtil = CImageDecoder::FileNewL(iCoeEnv->FsSession(), iLoadFileName, CImageDecoder::TOptions(iDecoderOptions), format));
		if (error!=KErrNone)
			{
			iEikonEnv->HandleError(error);
			iAppView->Reset(EDrawNow); // ensure we redraw whole screen - replace previous cross if required
			return;
			}
		}

	HandleNewlyOpenedImageL();

	StartFrameOpen(EPlaying);
	}

void CVideoAppUi::LoadFolderL(const TDesC& aDirName)
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

void CVideoAppUi::OpenNextFolderEntry()
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

	delete iLoadUtil; 
	iLoadUtil = NULL;
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

void CVideoAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
	{
	if(aResourceId==R_VIDEO_CLIP_MENU)
		{
		if (iLoadUtil==NULL || iState!=EIdle) // nothing open or we are busy
			aMenuPane->SetItemDimmed(EVideoCmdPlay,ETrue);
		}
	else if (aResourceId==R_VIDEO_OPERATE_MENU)
		{
		if (iLoadUtil==NULL || iState!=EIdle || iUseExtensions)
			{
			// nothing open or we are busy
			aMenuPane->SetItemDimmed(EVideoCmdRotateClockwise,ETrue);
			aMenuPane->SetItemDimmed(EVideoCmdRotateAntiClockwise,ETrue);
			aMenuPane->SetItemDimmed(EVideoCmdMirrorHorizontalAxis,ETrue);
			aMenuPane->SetItemDimmed(EVideoCmdMirrorVerticalAxis,ETrue);
			}
		}
	else if (aResourceId==R_VIDEO_ZOOM_MENU)
		{
		if (iLoadUtil==NULL || iState!=EIdle || iUseExtensions)
			{
			aMenuPane->SetItemDimmed(EVideoCmdZoomIn,ETrue);
			aMenuPane->SetItemDimmed(EVideoCmdZoomOut,ETrue);
			}
		}
	else if ( aResourceId==R_VIDEO_VIEW_MENU )
		{
		aMenuPane->SetItemButtonState(EVideoCmdUseExtensions,(iUseExtensions) ? EEikMenuItemSymbolOn : 0);
		}
	}

TBool CVideoAppUi::CheckHotKeyNotDimmedL(TInt aCommandId)
	{
	TInt result = ETrue;
	switch (aCommandId)
		{
		case EVideoCmdPlay:
		case EVideoCmdRotateClockwise:
		case EVideoCmdRotateAntiClockwise:
		case EVideoCmdFineZoomIn:
		case EVideoCmdFineZoomOut:
		case EVideoCmdMirrorHorizontalAxis:
		case EVideoCmdMirrorVerticalAxis:
			if (iLoadUtil==NULL || iState!=EIdle || iUseExtensions)
				result = EFalse;
			break;
		default:
			// do nothing
			break;
		}
	return result;
	}

void CVideoAppUi::LoadFolderWait()
	{
	ASSERT(!iWalker->IsActive()); // if we get here, should not be doing anything
	
	iTimer.After(iWalker->ActiveStatus(), KDefaultFolderDelay);
	iState = EFolderWait;
	}

void CVideoAppUi::HandleNewlyOpenedImageL()
	{
	ASSERT(iLoadUtil); // should have already been opened

	TFrameInfo frameInfo = iLoadUtil->FrameInfo(0);

	// jf 3/12/01. Assume frame 0 gives the main image characteristics
	iOverallSize = frameInfo.iOverallSizeInPixels;
	if (iOverallSize.iWidth==0 || iOverallSize.iHeight==0)
		{
		iOverallSize = frameInfo.iFrameCoordsInPixels.Size();
		if (iOverallSize.iWidth==0 || iOverallSize.iHeight==0)
			{
			iEikonEnv->InfoMsg(_L("Invalid image dimensions."));
			User::Leave(KErrCorrupt);
			}
		}	
	
	iAnimating = (frameInfo.iDelay != TTimeIntervalMicroSeconds(0) || IsMngImage(iLoadUtil));

	iFrameNumber = 0;

	iTime.HomeTime();
	}

void CVideoAppUi::ExtractFrameL()
	{
	if (iLoadFileName.Length() < KShortestPossiblePath)
		return;

	ASSERT(iLoadUtil);

	Cancel();

	TInt frameCount = iLoadUtil->FrameCount();

	if (iFrameNumber >= frameCount)
		iFrameNumber = 0;

	CEikDialog* dialog = new(ELeave) CVideoCurrentFrameDialog(iFrameNumber,frameCount);
	if (!dialog->ExecuteLD(R_VIDEO_FRAME_DIALOG))
		return;

	StartFrameOpen(ELoading);
	}

void CVideoAppUi::StartFrameOpen(TState aNextState, TBool aSizeFrame)
	{
	TRAPD(error, DoStartFrameOpenL(aSizeFrame));
	if (error!=KErrNone)
		iWalker->SelfComplete(error);
	iState = aNextState; // what ever happens we go to next state, and will handle any error there
	}

void CVideoAppUi::DoStartFrameOpenL(TBool aSizeFrame)
	{
	ASSERT(iLoadUtil); // should be true by now
	ASSERT(!iWalker->IsActive()); // we are going to use it

	ASSERT(iFrameNumber>=0 && iFrameNumber<iLoadUtil->FrameCount());

	// Set up extensions
	if ( iClippingRect.IsEmpty() )
		{
		TRAPD(err, iLoadUtil->SetClippingRectL(NULL));
		// Plugin may not support the call at all
		if(err != KErrNone && err != KErrNotSupported)
			{
			User::Leave(err);
			}
		}
	else
		{
		TRAPD(err,iLoadUtil->SetClippingRectL((iUseExtensions) ? &iClippingRect : NULL ));
		// Plugin may not support the call at all
		if(err == KErrNotSupported)
			{
			iEikonEnv->InfoMsg(_L("Clipping Not Supported using this codec."));
			}
		}
	
	TImageConvScaler* scaler = NULL;
	TRAPD(err,scaler = iLoadUtil->ScalerL());
	if ( err == KErrNone )
		{
		scaler->SetScalingL( (iUseExtensions) ? iScalingCoefficient : -1 , iScalingQuality);	
		}
	else if ( iScalingCoefficient != -1 && err == KErrNotSupported )
		{
		iEikonEnv->InfoMsg(_L("Scaler extension Not Supported using this codec."));
		}
		
	
	if ( iOperations.Count() )
		{
		TImageConvOperation* operation = NULL;
		TRAPD(err,operation = iLoadUtil->OperationL());
		if ( err == KErrNone )
			{
			operation->ClearOperationStack();
			
			if ( iUseExtensions)
				{
				for ( TInt i = 0; i < iOperations.Count(); i++)
					{
					operation->AddOperationL(static_cast<TImageConvOperation::TOperation>(iOperations[i]));
					}
				}
			}
		else if ( err == KErrNotSupported )
			{
			iEikonEnv->InfoMsg(_L("Operations Not Supported using this codec."));
			}
		}

	if (aSizeFrame)
		{
		TFrameInfo frameInfo = iLoadUtil->FrameInfo(iFrameNumber);

		if (iFrame.Handle() == 0) // Only create the bitmap if we haven't done it already.
			{
			if ( !iUseNativeDisplayMode && (frameInfo.iFlags & TFrameInfo::ECanDither) )
				User::LeaveIfError(iFrame.Create(TSize(0,0),iAppView->DisplayMode())); 
			else
				User::LeaveIfError(iFrame.Create(TSize(0,0),frameInfo.iFrameDisplayMode)); 
			}

		// with 16MA and 16MAP we don't use the mask anymore, this since the introduction
		// with CR894 & CR1111 of alpha channel support in PNG decoding

		if (frameInfo.iFlags & TFrameInfo::ETransparencyPossible && !iDisableMask
			 && iAppView->DisplayMode() != EColor16MA && iAppView->DisplayMode() != EColor16MAP )
			{
			if (iMask.Handle() == 0)
				User::LeaveIfError(iMask.Create(TSize(0,0),EGray256));
			}
		else
			{
			if (iMask.Handle())
				iMask.Reset();
			}

		
		SetFrameSizeAndPosL(frameInfo.iFrameCoordsInPixels,frameInfo.iOverallSizeInPixels); 
		}

	ASSERT(iDisableMask && iMask.Handle()==0 || !iDisableMask); // iDisableMask -> iMask.Handle()==0
	ASSERT(iMask.Handle()==0 || iFrame.SizeInPixels()==iMask.SizeInPixels()); // if mask must be same size
	
	// DEF090667: T: SetDisplayMode causes another file to be opened with wrong size
	if(iFrameNumber == 0)
		{
		iAppView->Reset(ENoDrawNow);
		}

	if (iMask.Handle())
		iLoadUtil->Convert(&(iWalker->ActiveStatus()),iFrame,iMask,iFrameNumber); 
	else
		iLoadUtil->Convert(&(iWalker->ActiveStatus()),iFrame,iFrameNumber); 

	iViewResized = EFalse;
	}
	
TBool CVideoAppUi::ExtensionIsSetup()
	{
	return ( ( !iClippingRect.IsEmpty() ) || Abs(iScalingCoefficient) > 1 || iOperations.Count() );
	}

void CVideoAppUi::PlayClipL()
	{
	iFrameNumber = 0;
	iTime.HomeTime();

	StartFrameOpen(EPlaying);
	}

void CVideoAppUi::SaveAsL()
	{
	if (iSaveFileName.Length() < KShortestPossiblePath)
		iSaveFileName = KDefPath;

	Cancel();

	CEikDialog* dialog = new(ELeave) CVideoSaveAsDialog(&iSaveFileName,iSaveInfo,iEncodeOperations,iCreateThumbnail,iSaveAsEXIF);
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
	if ( iSaveAsEXIF && (imageType == KImageTypeJPGUid) )
		{
		iSaveUtil = CJPEGExifEncoder::FileNewL(iCoeEnv->FsSession(), iSaveFileName, CImageEncoder::EOptionNone, imageType);
		// force creation of metadata - this will cause save as EXIF rather than JFIF
	    MExifMetadataWriter* metaData=static_cast<CJPEGExifEncoder*>(iSaveUtil)->ExifMetadata(); 
		}
		else
		{
		iSaveUtil = CImageEncoder::FileNewL(iCoeEnv->FsSession(), iSaveFileName, CImageEncoder::EOptionNone, imageType);		
		}


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
		
	if (iCreateThumbnail)
	{
		iSaveUtil->SetThumbnail(iCreateThumbnail);
	}
		
	if ( iEncodeOperations.Count() )
		{
		TImageConvOperation* operation = NULL;
		TRAPD(err, operation = iSaveUtil->OperationL());
		if ( err == KErrNone )
			{
			operation->ClearOperationStack();
			
			for ( TInt i = 0; i < iEncodeOperations.Count(); i++)
				{
				operation->AddOperationL(static_cast<TImageConvOperation::TOperation>(iEncodeOperations[i]));
				}
			}
		}

	StartFrameSave();
	}

void CVideoAppUi::DisplayModeL()
	{
	Cancel();
	TDisplayMode displayMode = iAppView->DisplayMode();
	CEikDialog* dialog = new(ELeave) CVideoDisplayModeDialog(displayMode,iUseNativeDisplayMode);
	if (dialog->ExecuteLD(R_VIDEO_DISPLAY_MODE_DIALOG))
		{
		if ( iUseNativeDisplayMode && iLoadUtil )
			{
			TFrameInfo frameInfo = iLoadUtil->FrameInfo(0);
			displayMode = frameInfo.iFrameDisplayMode;
			}

		iAppView->SetDisplayModeL(displayMode, &iFrame);
		}
	}

void CVideoAppUi::BackgroundColorL()
	{
	Cancel();
	TInt backgroundColor = iBackgroundColor;
	TBool override = iOverrideBackgroundColor;
	CEikDialog* dialog = new(ELeave) CVideoBackgroundColorDialog(backgroundColor, override);
	if (dialog->ExecuteLD(R_VIDEO_BACKGROUND_COLOR_DIALOG))
		{
		iBackgroundColor = backgroundColor;
		iOverrideBackgroundColor = override;
		if (iOverrideBackgroundColor)
			{
			iAppView->SetBackgroundColor(TRgb::Color16(iBackgroundColor), ENoDrawNow);
			iAppView->Clear(ENoDrawNow);
			//redraw without reloading
			if (iMask.Handle()==0)
				iAppView->DrawImage(&iFrame, iOffset, EDrawNow);
			else
				iAppView->DrawImage(&iFrame, &iMask, iOffset, EDrawNow);
			}
		}
	}

void CVideoAppUi::StreamPlayL(TFileType aFileType, TBool aMultiFrameStreaming)
	{
	Cancel();
	iLastFileType = aFileType;
	iMultiFrameStreaming = aMultiFrameStreaming;

	if (iLoadFileName.Length() < 4)
		iLoadFileName = KDefPath;
	CEikDialog* dialog = new(ELeave) CEikFileOpenDialog(&iLoadFileName);
	if (!dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		return;

	delete iLoadUtil; iLoadUtil = NULL;

	User::Free(REINTERPRET_CAST(TAny*,CONST_CAST(TUint8*, iStreamBuffer.Ptr())));
	iStreamBuffer.Set(NULL, 0, 0);

	RFile file;
	User::LeaveIfError(file.Open(iEikonEnv->FsSession(), iLoadFileName, EFileShareReadersOnly|EFileStream|EFileRead));
	CleanupClosePushL(file);
	TInt fileSize;
	User::LeaveIfError(file.Size(fileSize));
	iStreamBuffer.Set(STATIC_CAST(TUint8 *, User::AllocL(fileSize)), 0, fileSize);
	User::LeaveIfError(file.Read(iStreamBuffer));
	CleanupStack::PopAndDestroy(); // file

	iEikonEnv->BusyMsgL(_L("Busy..."));

	// NB. State machine assumes no leaves after this point

	ASSERT(!iStreamGen->IsActive()); // no async behav should be going on at this point
	ASSERT(!iWalker->IsActive());

	// initialize the buffer
	iStreamBuffer.SetLength(0); // data is in buffer, and we increase length to simulate data arriving
	User::LeaveIfError(ExtendStreamBuffer());

	iState = EStreamOpening;
	}

TInt CVideoAppUi::ExtendStreamBuffer()
	{
//	ASSERT(iStreamBuffer.Length()<iStreamBuffer.MaxLength()); // assumed we don't call when no more streaming to do

	// We CAN get called when there's no more data if the decoders have failed
	// to detect end-of-data - e.g. when the input file's header contains a length
	// greater than the file size - so return KErrUnderflow if this happens
	ASSERT(iStreamBuffer.Length() <= iStreamBuffer.MaxLength()); 
	if (iStreamBuffer.Length() == iStreamBuffer.MaxLength())
		return KErrUnderflow;

	// 95% change of 128 bytes - else random 1..128
	TInt chance = Math::Rand(iStreamSeed) % 100;
	TInt increment;
	if (chance < 95)
		increment = 128;
	else
		increment = Math::Rand(iStreamSeed) % 127 + 1; // 1 .. 128
	TInt newSize = Min(iStreamBuffer.Length()+increment, iStreamBuffer.MaxLength());
	iStreamBuffer.SetLength(newSize);

	// random time is 99% chance between 0 and 1/20s, with remainder between 0 and 5s
	chance = Math::Rand(iStreamSeed) % 100;
	TInt waitTime;
	if (chance>=99)
		waitTime = Math::Rand(iStreamSeed) % 5000000;
	else
		waitTime = Math::Rand(iStreamSeed) % 50000;

	iStreamTimer.After(iStreamGen->ActiveStatus(), waitTime);
	iSourceHasGrown = ETrue;

#if defined(_DEBUG) && defined(__ENABLE_DEBUG_OUTPUT)
	RDebug::Print(_L("ExtendingStreamBuffer(%d,%d->%d)"), increment, waitTime, newSize); 
#endif 

	return KErrNone;
	}

void CVideoAppUi::FrameInfoL()
	{
	if (iFrame.Handle() == 0)
		return;

	Cancel();

	const TInt frameCount = iLoadUtil->FrameCount();
	if(iFrameNumber >= frameCount)
		iFrameNumber = frameCount-1;

	HBufC* infoBuffer = HBufC::NewLC(KInfoBufferSize);
	TPtr info(infoBuffer->Des());

	CFrameInfoStrings *frameInfoStrings = iLoadUtil->FrameInfoStringsLC(iFrameNumber);
	TInt count = frameInfoStrings->Count();
	TInt index;
	for (index=0; index<count; index++)
		{
		TPtrC frameInfoString = frameInfoStrings->String(index);
		info.Append(frameInfoString);
		info.Append(TChar(CEditableText::ELineBreak));
		}

	HBufC* imageComments = HBufC::NewLC(0);
	TInt numImageComments = iLoadUtil->NumberOfImageComments();
	for (index = 0; index<numImageComments; index++)
		{
		HBufC* nextComment = iLoadUtil->ImageCommentL(index);
		CleanupStack::PushL(nextComment);

		TPtr commentTPtr(nextComment->Des());
		ReplaceNewlinesWithLineBreaks(commentTPtr);

		if (index==0)
			{
			// first go through, so should have nothing in comments - just assign
			ASSERT(imageComments->Length()==0);
			imageComments = imageComments->ReAllocL(nextComment->Length());
			*imageComments = *nextComment;
			}
		else
			{
			// append line break and then new comments
			imageComments = imageComments->ReAllocL(imageComments->Length() + nextComment->Length() + 1); // 1 for linebreak character
			TPtr imageCommentsPtr(imageComments->Des());
			imageCommentsPtr.Append(TChar(CEditableText::ELineBreak));
			imageCommentsPtr.Append(*nextComment);
			}
		CleanupStack::PopAndDestroy(); // nextComment
		CleanupStack::Pop(); // old imageComments (already deleted by ReAlloc)
		CleanupStack::PushL(imageComments);
		}
	
	HBufC* frameComments = HBufC::NewLC(0);
	TInt numFrameComments = iLoadUtil->NumberOfFrameComments(iFrameNumber);
	for (index = 0; index<numFrameComments; index++)
		{
		HBufC* nextComment = iLoadUtil->FrameCommentL(iFrameNumber, index);
		CleanupStack::PushL(nextComment);

		TPtr commentTPtr(nextComment->Des());
		ReplaceNewlinesWithLineBreaks(commentTPtr);

		if (index==0)
			{
			// first go through, so should have nothing in comments - just assign
			ASSERT(frameComments->Length()==0);
			frameComments = frameComments->ReAllocL(nextComment->Length());
			*frameComments = *nextComment;
			}
		else
			{
			// append line break and then new comments
			frameComments->ReAllocL(frameComments->Length() + nextComment->Length() + 1); // 1 for linebreak character
			TPtr frameCommentsPtr(frameComments->Des());
			frameCommentsPtr.Append(TChar(CEditableText::ELineBreak));
			frameCommentsPtr.Append(*nextComment);
			}
		CleanupStack::PopAndDestroy(); // nextComment
		CleanupStack::Pop(); // old frameComments (already deleted by ReAlloc)
		CleanupStack::PushL(frameComments);
		}
	
	CEikDialog* dialog = new(ELeave) CFrameInfoDialog(info, *imageComments, *frameComments);
	dialog->ExecuteLD(R_VIDEO_INFO_DIALOG);

	CleanupStack::PopAndDestroy(4); // frameComments, imageComments, frameInfoStrings + infoBuffer
	}

void CVideoAppUi::FrameRotateL(TBool aClockwise)
	{
	ASSERT(iState == EIdle);
	CBitmapRotator::TRotationAngle angle = aClockwise ?
			CBitmapRotator::ERotation90DegreesClockwise :
			CBitmapRotator::ERotation270DegreesClockwise;
	if (iMask.Handle() != 0)
		{
		iRotateAngle = angle; //keep for second phase
		iRotator->Rotate(&(iWalker->ActiveStatus()), iMask, angle);
		iState = ERotatingMask;
		}
	else
		{
		iRotator->Rotate(&(iWalker->ActiveStatus()), iFrame, angle);
		iState = ERotating;
		}
	iViewResized = EFalse;
	}

void CVideoAppUi::FrameMirrorL(TBool aHorizontalAxis)
	{
	ASSERT(iState == EIdle);
	CBitmapRotator::TRotationAngle angle = aHorizontalAxis ? 
			CBitmapRotator::EMirrorHorizontalAxis : 
			CBitmapRotator::EMirrorVerticalAxis;
	if (iMask.Handle() != 0)
		{
		iRotateAngle = angle; //keep for second phase
		iRotator->Rotate(&(iWalker->ActiveStatus()), iMask,	angle);
		iState = ERotatingMask;
		}
	else
		{
		iRotator->Rotate(&(iWalker->ActiveStatus()), iFrame, angle);
		iState = ERotating;
		}
	iViewResized = EFalse;
	}

void CVideoAppUi::ZoomFrameL(TBool aZoomIn)
	{
	TSize size(iFrame.SizeInPixels());
	const TSize adjust(size.iWidth / KScaleFactor, size.iHeight / KScaleFactor);

	if (aZoomIn)
		size += adjust;
	else
		size -= adjust;

	Cancel();

	TFrameInfo frameInfo = iLoadUtil->FrameInfo(iFrameNumber);

	if (!(frameInfo.iFlags&TFrameInfo::EFullyScaleable))
		{
		if (iMask.Handle()!=0)
			{
			iScaleSize = size; // keep for second scale action
			iScaler->Scale(&(iWalker->ActiveStatus()),iMask,size);
			iViewResized = EFalse;
			iState = EScalingMask;
			}
		else
			{
			iScaler->Scale(&(iWalker->ActiveStatus()),iFrame,size);
			iViewResized = EFalse;
			iState = EScaling;
			}
		}
	else
		{
		SetFrameSizeAndPosL(frameInfo.iFrameCoordsInPixels,size);

		if (!(frameInfo.iFlags&TFrameInfo::ECanDither))
			{
			iFrame.Reset();
			User::LeaveIfError(iFrame.Create(TSize(0,0),frameInfo.iFrameDisplayMode)); // We will dither later
			}

#if !defined(__CLEAR_BITMAPS_FIRST)
		if (iFrame.SizeInPixels() != size)
#endif 
			{
			#if defined(__CLEAR_BITMAPS_FIRST)
				User::LeaveIfError(iFrame.Resize(TSize(0,0)));
			#endif 
			User::LeaveIfError(iFrame.Resize(size));
			}

		if (iMask.Handle())
			{
			if (iMask.SizeInPixels() != size)
				User::LeaveIfError(iMask.Resize(size));
			#if defined(__CLEAR_BITMAPS_FIRST)
			// set mask to black, so it is opaque and by default nothing is drawn
				CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(&iMask);
				CleanupStack::PushL(device);
				CFbsBitGc* bmGc = CFbsBitGc::NewL();
				CleanupStack::PushL(bmGc);
				bmGc->Activate(device);
				bmGc->SetPenStyle(CGraphicsContext::ENullPen);
				bmGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
				bmGc->SetBrushColor(KRgbBlack);
				const TRect drawRect(TPoint(0,0), iMask.SizeInPixels());
				bmGc->DrawRect(drawRect);
				CleanupStack::PopAndDestroy(2);
			#endif 
			}

		StartFrameOpen(ELoading, EFalse); 
		}
	}

void CVideoAppUi::StartFrameSave()
	{
	ASSERT(iSaveUtil); // should be true by now
	ASSERT(iState==EIdle); 
	ASSERT(!iWalker->IsActive()); // we are going to use it
	iSaveUtil->Convert(&(iWalker->ActiveStatus()),iFrame,iFrameImageData); 
	iState = ESaving;
	}

void CVideoAppUi::DrawConvertedFrameL()
	{
	ASSERT(iDisableMask && iMask.Handle()==0 || !iDisableMask); // iDisableMask -> iMask.Handle()==0
	ASSERT(iMask.Handle()==0 || iFrame.SizeInPixels()==iMask.SizeInPixels()); // if mask must be same size

	TBool fullRedrawReq = EFalse;
	if (!iViewResized)
		{
		TFrameInfo frameInfo = iLoadUtil->FrameInfo(iFrameNumber);
		TRgb backgroundColor = iOverrideBackgroundColor ? TRgb::Color16(iBackgroundColor) : frameInfo.iBackgroundColor;
		fullRedrawReq = iAppView->SetBackgroundColor(backgroundColor, ENoDrawNow);
		if (iState == EScaling || iState == ERotating)
			{
			// size and center based on bitmap, rather than the official image size
			TBool resized = iAppView->ResizeL(iFrame.SizeInPixels(), ETrue, ENoDrawNow);
			if (resized)
				iAppView->Center(ENoDrawNow);
			fullRedrawReq = fullRedrawReq || resized;
			}
		else if (!iAnimating || iFrameNumber==0)
			{
			TBool resized = iAppView->ResizeL(iImageDisplaySize, ETrue, ENoDrawNow);
				// implies Resize
			
			// PDEF088116: TImageViewer USER 0 with certain animated gifs
			if(frameInfo.iFlags & TFrameInfo::EUsesFrameSizeInPixels) 
				{
				if(iFrame.SizeInPixels() != frameInfo.iFrameSizeInPixels) 
					{
					iFrame.Resize(frameInfo.iFrameSizeInPixels);
					}
				}
			//Resize iFrame to iImageDisplaySize if they are different
			else if (iFrame.SizeInPixels() != iImageDisplaySize)
				{
				iFrame.Resize(iImageDisplaySize);
				}
				
			if (resized)
				iAppView->Center(ENoDrawNow);
			else if (IsMngImage(iLoadUtil) && (iLastFrameFlags & TFrameInfo::ERestoreToBackground))
				{
				iAppView->Clear(EFalse, EDrawNow);
				}
			fullRedrawReq = fullRedrawReq || resized;
			}
		else if (iFrameNumber > 0 && iLastFrameFlags & TFrameInfo::ERestoreToBackground
				 || iState != EPlaying && iState != EStreamDecoding && iFrameNumber-1!=iPrevFrameNumber)
			{
			iAppView->Clear(EFalse, EDrawNow);
			ASSERT(iAnimating); // should be true given above, avoids complicating below
			if (iState != EPlaying && iState != EStreamDecoding && iFrameNumber-1!=iPrevFrameNumber)
				fullRedrawReq = ETrue;
				// usually will want to redraw _whole_ bitmap area - otherwise we only
				// redraw that part of the bitmap we DrawImage to
				// exception is when we are extracting frames in the correct sequence
			}
		else if (iAnimating && (iState==EPlaying||iState==EStreamDecoding))
			{
			// if animating and playing, should have frame greater than 0 and no restore to background - or will trigger above
			ASSERT(! (iLastFrameFlags & TFrameInfo::ERestoreToBackground));
			ASSERT(iFrameNumber>0 && iFrameNumber-1==iPrevFrameNumber);
			fullRedrawReq = EFalse; // don't redraw full - even if background changed, want to just redraw our bit
			}
		iViewResized = ETrue;
		}

	if (iMask.Handle()==0)
		iAppView->DrawImage(&iFrame, iOffset, fullRedrawReq?ENoDrawNow:EDrawNow);
	else
		iAppView->DrawImage(&iFrame, &iMask, iOffset, fullRedrawReq?ENoDrawNow:EDrawNow);

	if (fullRedrawReq)
		iAppView->DrawNow();
	}

void CVideoAppUi::HandleConvertCompleteL()
	{
	DrawConvertedFrameL();

	TFrameInfo frameInfo = iLoadUtil->FrameInfo(iFrameNumber);

	iLastFrameFlags = frameInfo.iFlags;
	iPrevFrameNumber = iFrameNumber;

	if (iAnimating && iState==EPlaying ||
		iState==EStreamDecoding && (iMultiFrameStreaming||iAnimating))
		{
		ASSERT(!iWalker->IsActive()); // we will use it again
		TBool mustSelfComplete=EFalse;
		if ((iAnimating && frameInfo.iDelay > TTimeIntervalMicroSeconds(0))||iMultiFrameStreaming)
			{
			TTime endTime;
			endTime.HomeTime();

			TTimeIntervalMicroSeconds timeTaken = endTime.MicroSecondsFrom(iTime);
			if (frameInfo.iDelay > timeTaken)
				{
				TInt64 delay = frameInfo.iDelay.Int64() - timeTaken.Int64();
				TInt delayPeriod = I64LOW(delay);
				if (delayPeriod<0)
					delayPeriod=0; // can get negative if we took longer on the streaming than this
				iTimer.After(iWalker->ActiveStatus(), delayPeriod);
				}
			else if(iMultiFrameStreaming)
				{
				TInt delay = KDefaultStreamDelay - I64LOW(timeTaken.Int64());
				if (delay<0)
					delay=0; // can get negative if we took longer on the streaming than this
				iTimer.After(iWalker->ActiveStatus(), delay);
				}
			else
				mustSelfComplete=ETrue;
			}
		else
			mustSelfComplete=ETrue;
		if (mustSelfComplete)
			iWalker->SelfComplete(KErrNone);
		if (iState==EPlaying)
			iState = EPlayingWait;
		else 
			{
			ASSERT(iState==EStreamDecoding);
			iState = EStreamDecodeWait;
			}
		}
	else if (iOpeningFolder)
		LoadFolderWait();
	else
		{
		iEikonEnv->BusyMsgCancel();
		Cancel(); // cancel any background streaming
		iState = EIdle;
		}
	}

void CVideoAppUi::RunL(CVideoWalker* aWalker, TInt aStatus)
	{
	ASSERT(aWalker==iWalker || aWalker==iStreamGen); // ones we know about

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
	case EScaling:
	case ERotating:
		{
		ASSERT(aWalker==iWalker);
		TInt error = aStatus;
		if (error==KErrNone)
			TRAP(error, HandleConvertCompleteL());
		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
		break;
	case ERotatingMask:
		{
		ASSERT(aWalker == iWalker);
		if (aStatus==KErrNone)
			{
			// having done mask, kick off rotate the image itself
			iRotator->Rotate(&(iWalker->ActiveStatus()), iFrame, iRotateAngle);
			iState = ERotating;
			}
		else
			{
			HandleRunError(aWalker, aStatus);
			}
		break;
		}
	case EScalingMask:
		{
		ASSERT(aWalker==iWalker);
		if (aStatus==KErrNone)
			{
			// having done mask, kick off scale of normal
			iScaler->Scale(&iWalker->ActiveStatus(), iFrame, iScaleSize);
			iState = EScaling;
			}
		else
			HandleRunError(aWalker, aStatus);
		}
		break;
	case EPlayingWait:
		{
		ASSERT(iWalker==aWalker);
		iTime.HomeTime(); // whatever reset the time here
		if (aStatus!=KErrNone)
			HandleRunError(aWalker, aStatus);
		else if (iFrameNumber < iLoadUtil->FrameCount()-1 || (IsMngImage(iLoadUtil)&&HasMoreMngFrames(iLoadUtil)) )
			{
			iFrameNumber += ( IsMngImage(iLoadUtil)==EFalse );
			StartFrameOpen(EPlaying);
			}
		else if (iOpeningFolder)
			LoadFolderWait();
		else
			{
			ASSERT(!iWalker->IsActive() && !iStreamGen->IsActive()); // should have stopped naturally
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
	case EStreamOpening:
		{
		ASSERT(aWalker==iStreamGen); 
		TInt error = aStatus;

		if (error==KErrNone)
			{
			delete iLoadUtil; iLoadUtil = NULL;

			TUid format;
			switch (iLastFileType)
				{
				case EFileTypeOta:
					format = KImageTypeOTAUid;
					break;
				case EFileTypeWbmp:
					format = KImageTypeWBMPUid;
					break;
				default:
					format = KNullUid;
				}

			TRAP(error, iLoadUtil = CImageDecoder::DataNewL(iCoeEnv->FsSession(), iStreamBuffer, 
											CImageDecoder::TOptions(iDecoderOptions | CImageDecoder::EOptionAllowZeroFrameOpen), format));
			}

		if (error==KErrUnderflow)
			{
			error = KErrNone; // clear so we don't hit the error handler below
			if (!iStreamGen->IsActive())
				error = ExtendStreamBuffer(); // wait for more input. remain in current state
			}
		else if (error==KErrNone)
			{
			if (iLoadUtil->FrameCount()>0)
				{
				TRAP(error,HandleNewlyOpenedImageL());
				if (error==KErrNone)
					TRAP(error,StreamOpenFrameIfPosL());
				}
			else
				{
				// insufficient data, so wait until it comes along
				if (!iStreamGen->IsActive())
					error = ExtendStreamBuffer(); // wait for more input. switch to EStreamWaitForFirstFrame
				iState = EStreamWaitForFirstFrame;
				}
			}

		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
		break;
	case EStreamWaitAndContinue:
#if defined(__BYPASS_CONTINUE_CONVERT)
		ASSERT(EFalse); // should not happen when bypassing 
#else
		{
		ASSERT(aWalker==iStreamGen); 

		TInt error = aStatus;

		if (error==KErrNone)
			{
			if ((iStreamBuffer.Length()<iStreamBuffer.MaxLength()) && !iStreamGen->IsActive())
				error = ExtendStreamBuffer(); // "new data has arrived"
			ASSERT(iSourceHasGrown);
			ASSERT(iState==EStreamWaitAndContinue);
			iLoadUtil->ContinueConvert(&(iWalker->ActiveStatus()));
			iSourceHasGrown = EFalse;
			iState = EStreamDecoding;
			}

		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
#endif 
		break;
	case EStreamWaitForFirstFrame:
		{
		ASSERT(aWalker==iStreamGen); 

		TInt error = aStatus;

		if (error==KErrNone)
			{
			ASSERT(!iLoadUtil->IsImageHeaderProcessingComplete()); // should not be reading more than we need to know
			iLoadUtil->ContinueProcessingHeaderL();
			if (iLoadUtil->FrameCount()>0)
				{
				TRAP(error,HandleNewlyOpenedImageL());
				if (error==KErrNone)
					TRAP(error,StreamOpenFrameIfPosL());
				}
			else
				{
				// insufficient data, so wait until it comes along
				if (!iStreamGen->IsActive())
					error = ExtendStreamBuffer(); // wait for more input. stay in same state
				}
			}

		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
		break;
	case EStreamWaitForDetails:
		{
		ASSERT(aWalker==iStreamGen); 
		TInt error = aStatus;

		if (error==KErrNone)
			{
			TRAP(error, StreamOpenFrameIfPosL());
			}
		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
		break;
	case EStreamDecoding:
		{
		ASSERT(aWalker==iWalker || aWalker==iStreamGen);
		TInt error = aStatus;
		if (aWalker==iWalker)
			{
			TBool timerActive = iStreamGen->IsActive();
			if (error==KErrNone)
				{
				TRAP(error, HandleConvertCompleteL());
				}
			else if (error==KErrUnderflow && (timerActive || iSourceHasGrown))
				{
				// draw partial result to the screen if possible, else set error to KErrNone so we don't stop
				TFrameInfo frameInfo = iLoadUtil->FrameInfo(iFrameNumber);
				if (!(frameInfo.iFlags & TFrameInfo::EPartialDecodeInvalid))
					{
					TRAP(error, DrawConvertedFrameL());
					}
				else
					error = KErrNone;

				if (error==KErrNone)
					{
					// wait for more data to arrive before we try again
					#if defined(__BYPASS_CONTINUE_CONVERT)
						iState = EStreamWaitForDetails;
					#else
						iState = EStreamWaitAndContinue;
					#endif 
					if(!timerActive)
						{ // final buffer was added after last Convert()/ContinueConvert()
						ASSERT(iStreamBuffer.Length()==iStreamBuffer.MaxLength());
						ASSERT(iSourceHasGrown);
						// force a final decode
						iStreamTimer.After(iStreamGen->ActiveStatus(),0);
						}
					}
				}
			}
		else
			{
			ASSERT(aWalker==iStreamGen);
			if (error==KErrNone && iStreamBuffer.Length()<iStreamBuffer.MaxLength())
				error = ExtendStreamBuffer(); // data can arrive concurrent with decode
			}
		if (error!=KErrNone)
			HandleRunError(aWalker, error);
		}
		break;
	case EStreamDecodeWait:
		{
		ASSERT(aWalker==iWalker || aWalker==iStreamGen);
		TInt error = aStatus;
		if (aWalker==iWalker)
			{
			iTime.HomeTime(); // whatever reset the time here
			if (error==KErrNone)
				{
				if (iFrameNumber < iLoadUtil->FrameCount()-1 || !iLoadUtil->IsImageHeaderProcessingComplete())
					{
					iFrameNumber += 1;
					TRAP(error,StreamOpenFrameIfPosL());
					}
				else
					{
					Cancel(); // may be required true, eg if there is data after the frame, other AO may be busy
					iEikonEnv->BusyMsgCancel();
					iState = EIdle;
					}
				}
			}
		else
			{
			ASSERT(aWalker==iStreamGen);
			if (error==KErrNone && iStreamBuffer.Length()<iStreamBuffer.MaxLength())
				error = ExtendStreamBuffer(); // data can arrive concurrent with wait
			}
		if (error!=KErrNone)
			HandleRunError(aWalker, error);
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

void CVideoAppUi::DoCancel(CVideoWalker *aWalker)
	{
	ASSERT(aWalker==iWalker || aWalker==iStreamGen); // only ones we know about

#if defined(_DEBUG) && defined(__ENABLE_DEBUG_OUTPUT)
	RDebug::Print(_L("DoCancel(%x,%d)(%x,%x)"), aWalker, iState, iWalker, iStreamGen); 
#endif 

	if (aWalker==iWalker)
		{
		switch (iState)
			{
		case ELoading:
		case EPlaying:
		case EStreamDecoding:
			iLoadUtil->Cancel();
			break;
		case ESaving:
			iSaveUtil->Cancel();
			break;
		case EScaling:
		case EScalingMask:
			iScaler->Cancel();
			break;
		case ERotating:
		case ERotatingMask:
			iRotator->Cancel();
			break;
		case EStreamDecodeWait:
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
		ASSERT(aWalker==iStreamGen);
		switch (iState)
			{
		case EStreamOpening:
		case EStreamWaitForDetails:
		case EStreamWaitAndContinue:
		case EStreamDecoding:
		case EStreamDecodeWait:
		case EStreamWaitForFirstFrame:
			iStreamTimer.Cancel();
			break;
		default:
			ASSERT(EFalse); // unknown state or should not happen
			}
		}
	}

void CVideoAppUi::HandleRunError(CVideoWalker* /*aWalker*/, TInt aError)
	{
	Cancel();
	iEikonEnv->BusyMsgCancel();
	iEikonEnv->HandleError(aError);
	ASSERT(iState == EIdle);
	}

void CVideoAppUi::StreamOpenFrameIfPosL()
	{
	if (iFrameNumber >= iLoadUtil->FrameCount())
		{
		// Continue processing headers if necessary and only if we are not in the middle of an image conversion.
		ASSERT(!iLoadUtil->IsImageHeaderProcessingComplete()); // should not be reading more than we need to know
		iLoadUtil->ContinueProcessingHeaderL();
		}

	if (iFrameNumber >= iLoadUtil->FrameCount())
		{
		if (iLoadUtil->IsImageHeaderProcessingComplete())
			{
			Cancel(); // other AO could still be busy if there were bytes after last frame data
			iEikonEnv->BusyMsgCancel();
			iState = EIdle;
			}
		else
			{
			// not yet reached the frame header
			iState = EStreamWaitForDetails;
			if (!iStreamGen->IsActive())
				// this shuld cope with reaching end of stream prematurely :
				User::LeaveIfError(ExtendStreamBuffer()); 
			}
		}
	else
		{
		if ((iStreamBuffer.Length()<iStreamBuffer.MaxLength()) && !iStreamGen->IsActive())
			User::LeaveIfError(ExtendStreamBuffer()); // "new data has arrived"
		ASSERT(iSourceHasGrown);
		StartFrameOpen(EStreamDecoding);
		iSourceHasGrown = EFalse;
		}
	ASSERT(iState==EIdle||iStreamBuffer.Length()==iStreamBuffer.MaxLength()||iStreamGen->IsActive()); // either we've finished, reached the end or we expect more data
	}

void CVideoAppUi::SetFrameSizeAndPosL(const TRect& aFrameRect,const TSize& aOverallSize)
	{
	TRect zoomedFrame(aFrameRect);
	TSize zoomedSize(aOverallSize);
	TSize zoomedOverallSize(iOverallSize); // effectively size on frame 0
	if (aOverallSize.iWidth == 0 || aOverallSize.iHeight == 0)
		zoomedSize = aFrameRect.Size();

	if ( iUseExtensions )
		{
		// using PREQ1630 extensions
		TInt err = iLoadUtil->GetDestinationSize(zoomedSize,iFrameNumber);
		if ( err != KErrNone )
			{
			TBuf<256> buf;
			buf.Format(_L("GetDestinationSize Error %d"),err);
			iEikonEnv->InfoMsg(buf);
			}

		User::LeaveIfError(iFrame.Resize(zoomedSize));			
		}
	else
		{
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
#endif 
			zoomedFrame.iTl.iX = (zoomedFrame.iTl.iX) >> absZoomFactor;
			zoomedFrame.iTl.iY = (zoomedFrame.iTl.iY) >> absZoomFactor;
	//		zoomedFrame.iBr.iX = (zoomedFrame.iBr.iX + roundingFactor) >> absZoomFactor;
	//		zoomedFrame.iBr.iY = (zoomedFrame.iBr.iY + roundingFactor) >> absZoomFactor;

			zoomedSize.iWidth = (zoomedSize.iWidth + roundingFactor) >> absZoomFactor;
			zoomedSize.iHeight = (zoomedSize.iHeight + roundingFactor) >> absZoomFactor;

			zoomedFrame.iBr = zoomedFrame.iTl + zoomedSize;

			zoomedOverallSize.iWidth = (zoomedOverallSize.iWidth + roundingFactor) >> absZoomFactor;
			zoomedOverallSize.iHeight = (zoomedOverallSize.iHeight + roundingFactor) >> absZoomFactor;
			}

#if !defined(__CLEAR_BITMAPS_FIRST)
		if (iFrame.SizeInPixels() != zoomedFrame.Size())
#endif 
			{
#if defined(__CLEAR_BITMAPS_FIRST)
				// resize via 0 so that we clear original
				User::LeaveIfError(iFrame.Resize(TSize(0,0)));
#endif 
				User::LeaveIfError(iFrame.Resize(zoomedSize));
			}
		}

	if (iMask.Handle())
		{
		if (iMask.SizeInPixels() != zoomedFrame.Size())
			User::LeaveIfError(iMask.Resize(zoomedSize));
		#if defined(__CLEAR_BITMAPS_FIRST)
		// set mask to black, so it is opaque and by default nothing is drawn
			CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(&iMask);
			CleanupStack::PushL(device);
			CFbsBitGc* bmGc = CFbsBitGc::NewL();
			CleanupStack::PushL(bmGc);
			bmGc->Activate(device);
			bmGc->SetPenStyle(CGraphicsContext::ENullPen);
			bmGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
			bmGc->SetBrushColor(KRgbBlack);
			const TRect drawRect(TPoint(0,0), zoomedSize);
			bmGc->DrawRect(drawRect);
			CleanupStack::PopAndDestroy(2);
		#endif 
		}

	iImageDisplaySize = zoomedSize;
	iOffset = zoomedFrame.iTl;

	ASSERT(iMask.Handle()==0 || iFrame.SizeInPixels()==iMask.SizeInPixels()); // if mask must be same size
	}

void CVideoAppUi::ReplaceNewlinesWithLineBreaks(TDes& aText)
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

// update crop region
void CVideoAppUi::UpdateClippingRect(TUint code)
	{
	TFrameInfo frameInfo;
	if ( iLoadUtil )
		{
		switch (code)
			{
			case EKeyLeftArrow:
				iClippingRect.Move(-Min(iClippingRect.Width(),iClippingRect.iTl.iX),0);
				break;
			case EKeyRightArrow:
				{
				const TFrameInfo& frameInfo = iLoadUtil->FrameInfo(iFrameNumber);
				iClippingRect.Move(Min(iClippingRect.Width(),frameInfo.iOverallSizeInPixels.iWidth-iClippingRect.iBr.iX),0);
				}
				break;
			case EKeyUpArrow:
				iClippingRect.Move(0,-Min(iClippingRect.Height(),iClippingRect.iTl.iY));
				break;
			case EKeyDownArrow:
				{
				const TFrameInfo& frameInfo = iLoadUtil->FrameInfo(iFrameNumber);
				iClippingRect.Move(0,Min(iClippingRect.Height(),frameInfo.iOverallSizeInPixels.iHeight-iClippingRect.iBr.iY));
				}
				break;
			}
		}
		LoadFileL();
	}
//
// CVideoAppView
//

CVideoAppView* CVideoAppView::NewL(const TRect& aRect)
	{
	CVideoAppView* self = new (ELeave) CVideoAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}

CVideoAppView::CVideoAppView():
	CCoeControl()
	{}

void CVideoAppView::ConstructL(const TRect& /*aRect*/)
    {
	CreateWindowL();
#if defined(__WINS__)
	Window().SetRequiredDisplayMode(SystemGc().Device()->DisplayMode());
#endif
	iDisplayMode = Window().DisplayMode();
	EnableDragEvents();
    SetExtentToWholeScreen();

	iBmBuffer = new (ELeave) CWsBitmap(iCoeEnv->WsSession());
	ActivateL();
	}

CVideoAppView::~CVideoAppView()
	{
	delete iBmGc;
	delete iBmDevice;
	delete iBmBuffer;
	}

void CVideoAppView::Draw(const TRect& aRect) const
	{
	CWindowGc& gc = SystemGc();
	TRect drawRect=Rect();

	ASSERT(!iBitmapValid || iBmRect.Size() == iBmBuffer->SizeInPixels()); // either bitmap not valid or size of bmrect is same as the buffer

	if (iBitmapValid)
		{
		// if the required rect includes some background, then draw it
		// check is to see if the passed aRect is a pure subset of the bitmap rect
		TRect intersection(aRect);
		intersection.Intersection(iBmRect);
		if (intersection != aRect)
			{
			gc.SetPenStyle(CGraphicsContext::ENullPen); // solid background rect
			gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
			gc.SetBrushColor(KRgbWhite);
			gc.DrawRect(drawRect);
			TRect frame(iBmRect); // draw a frame one pixel larger than bitmap
			frame.Grow(1,1);
			gc.SetBrushStyle(CGraphicsContext::ENullBrush);
			gc.SetPenStyle(CGraphicsContext::ESolidPen);
			gc.DrawRect(frame);
			}
		// now if there is some bitmap to be drawn...
		if (!intersection.IsEmpty())
			{
			gc.BitBlt(iBmRect.iTl, iBmBuffer);
			}			
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

void CVideoAppView::Reset(TDrawNow aDrawNow)
	{
	iBmBuffer->Reset();
	iBitmapValid = EFalse;
	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

void CVideoAppView::DrawImage(CFbsBitmap* aBitmap, const TPoint& aOffset, TDrawNow aDrawNow)
	{
	DrawImage(aBitmap, NULL, aOffset, aDrawNow);
	}

void CVideoAppView::DrawImage(CFbsBitmap* aBitmap, CFbsBitmap* aMask, const TPoint& aOffset, TDrawNow aDrawNow)
	{
	ASSERT(iBitmapValid && iBmBuffer->Handle()); // should only be called when size setup properly
	ASSERT(aMask==NULL || aBitmap->SizeInPixels()==aMask->SizeInPixels());
		// if we have a mask, assumed to be the same size as the original

	const TPoint screenOffset = iBmRect.iTl + aOffset; // relative to screen instead of iBmRect
	const TRect bitmapRect (screenOffset, aBitmap->SizeInPixels()); // the rect for this bitmap

#if defined(_DEBUG)
	TRect intersection (bitmapRect); // check that this rect is same or smaller than the bitmap
	ASSERT(iBmRect.Size() == iBmBuffer->SizeInPixels());
	intersection.Intersection(iBmRect);
	ASSERT(intersection==bitmapRect);
#endif 

	// first draw to bitmap buffer
	if (aMask)
		iBmGc->BitBltMasked(aOffset, aBitmap, TRect(aBitmap->SizeInPixels()), aMask, EFalse);
	else
		iBmGc->BitBlt(aOffset, aBitmap);

	// if required, also draw to screen
	if (aDrawNow!=ENoDrawNow)
		{
		ActivateGc();
		CWindowGc& gc = SystemGc();
		if (aMask)
			gc.BitBltMasked(screenOffset, aBitmap, TRect(aBitmap->SizeInPixels()), aMask, EFalse);
		else
			gc.BitBlt(screenOffset, aBitmap);
		DeactivateGc();
		}
	}

TBool CVideoAppView::ResizeL(const TSize& aNewSize, TBool aClear, TDrawNow aDrawNow)
	{
	//Resize iBmBuffer to iBmRect where iBmBuffer holds the aNewSize. 
	if(iBmRect.Size() != aNewSize)
		{
		iBmBuffer->Resize(iBmRect.Size());
		}
	ASSERT(!iBitmapValid || iBmRect.Size() == iBmBuffer->SizeInPixels()); // either bitmap not valid or size of bmrect is same as the buffer
	if (iBitmapValid && aNewSize==iBmRect.Size())
		{
		// special cases where we don't actually modify the size
		if (aDrawNow!=ENoDrawNow)
			DrawNow();
		return EFalse;
		}

	CFbsBitmap* tempBuffer = NULL;

	TBool preserveOrig = !aClear && iBitmapValid;

	if (preserveOrig)
		{
		// tempBuffer becomes copy of original
		tempBuffer = new (ELeave) CWsBitmap(iCoeEnv->WsSession());
		CleanupStack::PushL(tempBuffer);
		User::LeaveIfError(tempBuffer->Duplicate(iBmBuffer->Handle()));
		}

	ResizeBufferL(aNewSize, iDisplayMode);
		// resize bitmap

	iBitmapValid = ETrue;
	iBmRect.SetRect(iBmRect.iTl, aNewSize); // rect with same Tl but new size
	ASSERT(iBmRect.Size() == iBmBuffer->SizeInPixels()); // check resized bitmap OK

	if (preserveOrig)
		{
		// draw original back at new size
		EnsureSizeInTwipsSet(tempBuffer);
		EnsureSizeInTwipsSet(iBmBuffer);
		iBmGc->DrawBitmap(TPoint(0,0), tempBuffer);
		CleanupStack::PopAndDestroy(); // tempBuffer
		}
	else
		Clear(EFalse, ENoDrawNow); // get background correct colour

	if (aDrawNow!=ENoDrawNow)
		DrawNow();

	return ETrue;
	}

void CVideoAppView::ResizeBufferL(const TSize& aNewSize, TDisplayMode aDisplayMode)
	{
	delete iBmGc; iBmGc = NULL;
	delete iBmDevice; iBmDevice = NULL;
	User::LeaveIfError(iBmBuffer->Create(aNewSize, aDisplayMode));
	iBmDevice = CFbsBitmapDevice::NewL(iBmBuffer);
	iBmGc = CFbsBitGc::NewL();
	iBmGc->Activate(iBmDevice);
	}

void CVideoAppView::EnsureSizeInTwipsSet(CFbsBitmap* aBitmap) const
	{
	// ensure the bitmap has twips size set - this allows us to use DrawBitmap
	// note this does not itself resize the bitmap - size in pixels remains unchanged
	TSize size = aBitmap->SizeInTwips();
	ASSERT(size.iWidth==0 && size.iHeight==0 || size.iWidth>0 && size.iHeight>0); 
		// assumption that if we've set the size it is properly formatted
	if (size==TSize(0,0))
		{
		CWsScreenDevice *const screenDevice = iEikonEnv->ScreenDevice();
		size = aBitmap->SizeInPixels();
		size.iWidth = screenDevice->HorizontalTwipsToPixels(size.iWidth);
		size.iHeight = screenDevice->VerticalTwipsToPixels(size.iHeight);
		aBitmap->SetSizeInTwips(size);
		}
	}

void CVideoAppView::Clear(TBool aClearFull, TDrawNow aDrawNow)
	{
	// if we have a bitmap buffer clear that. Otherwise clear the whole screen depending
	// on aClearFull
	if (iBmGc)
		{
		iBmGc->Reset();
		iBmGc->SetPenStyle(CGraphicsContext::ENullPen);
		iBmGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		iBmGc->SetBrushColor(iBackgroundColor);
		iBmGc->Clear();
		}
	if (aDrawNow!=ENoDrawNow)
		{
		if (aClearFull)
			DrawNow();
		else
			{
			ActivateGc();
			CWindowGc& gc = SystemGc();
			RWindow& window = Window();
			window.Invalidate(iBmRect);
			window.BeginRedraw(iBmRect);
			gc.SetPenStyle(CGraphicsContext::ENullPen);
			gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
			gc.SetBrushColor(iBackgroundColor);
			gc.Clear();
			window.EndRedraw();
			DeactivateGc();
			}
		}
	}


void CVideoAppView::MoveBy(const TPoint& aRelMove, TDrawNow aDrawNow)
	{
	iBmRect.Move(aRelMove);

	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

void CVideoAppView::Center(TDrawNow aDrawNow)
	{
	ASSERT(iBitmapValid && iBmRect.Size() == iBmBuffer->SizeInPixels()); // should only be called when size setup properly

#ifdef CENTRE_IMAGE
	const TPoint center = Rect().Center();
	const TSize bitmapSize = iBmRect.Size();
	const TPoint requiredTl (center.iX-bitmapSize.iWidth/2, center.iY-bitmapSize.iHeight/2);
	const TRect newRect(requiredTl, bitmapSize);
	iBmRect = newRect;
#endif

	ASSERT(iBitmapValid && iBmRect.Size() == iBmBuffer->SizeInPixels()); // checked worked

	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

TBool CVideoAppView::SetBackgroundColor(const TRgb& aColor, TDrawNow aDrawNow)
	{
	TBool changed = iBackgroundColor!=aColor;

	iBackgroundColor = aColor;

	if (aDrawNow!=ENoDrawNow)
		DrawNow();

	return changed;
	}

void CVideoAppView::SetDisplayModeL(TDisplayMode aDisplayMode, CWsBitmap* aFrame, TDrawNow aDrawNow)
	{
	
	TBool change=aDisplayMode!=iDisplayMode;

	iDisplayMode = aDisplayMode; 

	if (iBitmapValid && change)
		{
		ASSERT(iBmBuffer->Handle());

		// temp buffer becomes copy of original
		CFbsBitmap* tempBuffer = new (ELeave) CWsBitmap(iCoeEnv->WsSession());
		CleanupStack::PushL(tempBuffer);
		User::LeaveIfError(tempBuffer->Duplicate(iBmBuffer->Handle()));

		ASSERT(iBmRect.Size()==iBmBuffer->SizeInPixels()); // should be the same
		ResizeBufferL(iBmBuffer->SizeInPixels(), iDisplayMode);
			// change bitmap

		// bitblt original back
		iBmGc->BitBlt(TPoint(0,0), tempBuffer);
		CleanupStack::PopAndDestroy(); // tempBuffer

		aFrame->Reset(); // reset the frame
		aFrame->Duplicate(iBmBuffer->Handle()); // duplicate the aFrame bitmap handle 
		}

	Window().SetRequiredDisplayMode(iDisplayMode);

	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

//
// CVideoDisplayModeDialog
//

// list of supported display modes (it matches r_video_display_mode_array in TImageViewer.rss)

const TDisplayMode KDispMode[] = { EGray2, EGray4, EGray16, EGray256, EColor16, EColor256, EColor4K, EColor64K, EColor16M, EColor16MU, EColor16MA, EColor16MAP };
const TInt KNumDispMode = sizeof(KDispMode) / sizeof(KDispMode[0]);

CVideoDisplayModeDialog::CVideoDisplayModeDialog(TDisplayMode& aDisplayMode,TBool& aUseNativeDisplayMode):
	iDisplayMode(aDisplayMode), iUseNativeDisplayMode(aUseNativeDisplayMode)
	{}

void CVideoDisplayModeDialog::PreLayoutDynInitL()
	{
	TInt choice = -1;

	for (TInt index=0; index<KNumDispMode; index++)
		{
		if (KDispMode[index]==iDisplayMode)
			{
			choice = index;
			break;
			}
		}
		
	SetCheckBoxState(EVideoIdUseImageNativeMode, iUseNativeDisplayMode ? CEikButtonBase::ESet : CEikButtonBase::EClear);

	ASSERT(choice>=0); // should always match something
	SetChoiceListCurrentItem(EVideoIdDisplayMode,choice);
	}

TBool CVideoDisplayModeDialog::OkToExitL(TInt /*aButtonId*/)
	{
	const TInt chosenIndex = ChoiceListCurrentItem(EVideoIdDisplayMode);
	ASSERT(chosenIndex<KNumDispMode);
	iDisplayMode = KDispMode[chosenIndex];
	iUseNativeDisplayMode = CheckBoxState(EVideoIdUseImageNativeMode) == CEikButtonBase::ESet ? ETrue : EFalse;	
	return ETrue;
	}

//
// CVideoBackgroundColorDialog
//

CVideoBackgroundColorDialog::CVideoBackgroundColorDialog(TInt& aColor16, TBool& aOverride):
	iColor16(aColor16), iOverride(aOverride)
	{}

void CVideoBackgroundColorDialog::PreLayoutDynInitL()
	{
	CEikButtonBase::TState state = iOverride ? CEikButtonBase::ESet : CEikButtonBase::EClear;
	SetCheckBoxState(EVideoIdOverrideBackgroundColor, state);
	SetChoiceListCurrentItem(EVideoIdBackgroundColor,iColor16);
	if (!iOverride)
		SetLineDimmedNow(EVideoIdBackgroundColor,ETrue);
	}

TBool CVideoBackgroundColorDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iColor16 = ChoiceListCurrentItem(EVideoIdBackgroundColor);
	CEikButtonBase::TState state = CheckBoxState(EVideoIdOverrideBackgroundColor);
	iOverride = state != CEikButtonBase::EClear;
	return ETrue;
	}

void CVideoBackgroundColorDialog::HandleControlStateChangeL(TInt aControlId)
	{
	if (aControlId == EVideoIdOverrideBackgroundColor)
		{
		CEikButtonBase::TState state = CheckBoxState(EVideoIdOverrideBackgroundColor);
		TBool isSet = state != CEikButtonBase::EClear;
		SetLineDimmedNow(EVideoIdBackgroundColor,!isSet);
		}
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

CVideoSaveAsDialog::CVideoSaveAsDialog(TDes* aFileName,TFileSaveInfo& aSaveInfo,RArray<TInt>& aEncodeOperations,TBool& aCreateThumbnail,TBool& aSaveAsEXIF):
	CEikFileSaveAsDialog(aFileName),
	iSaveInfo(aSaveInfo),
	iEncodeOperations(aEncodeOperations),
	iCreateThumbnail(&aCreateThumbnail),
	iSaveAsEXIF(&aSaveAsEXIF)
	{}

void CVideoSaveAsDialog::PreLayoutDynInitL()
	{
	SetTypeL();
	SetCheckBoxState(EVideoIdCreateThumbnailChbx, *iCreateThumbnail ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	SetCheckBoxState(EVideoIdSaveAsEXIFChbx, *iSaveAsEXIF ? CEikButtonBase::ESet : CEikButtonBase::EClear);

	CEikFileSaveAsDialog::PreLayoutDynInitL();
	}

void CVideoSaveAsDialog::SetTypeL()
	{
	const TUid imageType = iSaveInfo.iImageTypeUid;

	if(imageType == KImageTypeBMPUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_BMP);
	else if(imageType == KImageTypeGIFUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_GIF);
	else if(imageType == KImageTypeJPGUid)
		{
		if ( *iSaveAsEXIF )
			{
			SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_EXIF);
			}
			else
			{
			SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_JPEG);
			}
		}
	else if(imageType == KImageTypeMBMUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_MBM);
	else if(imageType == KImageTypePNGUid)
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_PNG);
	else
		SetLabelL(EVideoIdSaveAsFormat,R_VIDEO_FILE_FORMAT_CUSTOM);

	}

void CVideoSaveAsDialog::HandleControlStateChangeL(TInt aControlId)
	{
	if (aControlId == EVideoIdSaveAsEXIFChbx)
		{
		*iSaveAsEXIF = CheckBoxState(EVideoIdSaveAsEXIFChbx) == CEikButtonBase::ESet ? ETrue : EFalse;	
		SetTypeL();
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
	else if (aButtonId == EVideoIdEncodeOperations)
		{
		CEikDialog* dialog = new(ELeave) CExtensionOptionsDialog(iEncodeOperations);
		dialog->ExecuteLD(R_VIDEO_ENCODER_EXTENSION_DIALOG);
		return EFalse;
		}
	else if (aButtonId == EEikBidOk)
		{
		*iCreateThumbnail = CheckBoxState(EVideoIdCreateThumbnailChbx) == CEikButtonBase::ESet ? ETrue : EFalse;	
		*iSaveAsEXIF = CheckBoxState(EVideoIdSaveAsEXIFChbx) == CEikButtonBase::ESet ? ETrue : EFalse;	
		}

	return CEikFileSaveAsDialog::OkToExitL(aButtonId);
	}

SEikControlInfo CVideoSaveAsDialog::CreateCustomControlL(TInt /*aControlType*/)
	{
	/* never called */
	SEikControlInfo info;
	info.iTrailerTextId = 0;
	info.iFlags = 0;
	info.iControl = new (ELeave) CEikTextListBox;
	return info;
	}
//
// CVideoFormatDialog
//

CVideoFormatDialog::CVideoFormatDialog(TFileSaveInfo& aSaveInfo):
	iSaveInfo(aSaveInfo)
	{}

void CVideoFormatDialog::PreLayoutDynInitL()
	{
	CEikChoiceList* formatList = STATIC_CAST(CEikChoiceList*,Control(EVideoIdFileFormatType));

	//Add the available encoders to the dialog
	iEncoderList = CPluginInfoArray::NewL();
	formatList->SetArrayL(iEncoderList); //ownership of iEncoderList passed to dialog

	const TInt noOfEncoders = iEncoderList->MdcaCount();
	if(noOfEncoders == 0)
		User::Leave(KErrNotFound);
	
	//Find the index for the selected encoder (via ImageTypeUid)
	//if it is not found use the first
	TInt index;
	for(index=noOfEncoders-1; index > 0; index--)
		{
		if(iEncoderList->ImageType(index) == iSaveInfo.iImageTypeUid)
			break;
		}
	iSaveInfo.iImageTypeUid = iEncoderList->ImageType(index);

	SetChoiceListCurrentItem(EVideoIdFileFormatType,index);
	SetChoiceListCurrentItem(EVideoIdFileFormatBpp,iSaveInfo.iBpp);
	SetChoiceListCurrentItem(EVideoIdFileFormatColor,iSaveInfo.iColor);
	SetNumberEditorValue(EVideoIdFileFormatFactor,iSaveInfo.iQualityFactor);
	SetChoiceListCurrentItem(EVideoIdFileFormatSampling,iSaveInfo.iSampling);
	SetChoiceListCurrentItem(EVideoIdFileFormatCompression,iSaveInfo.iCompression);

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
	TInt compression = ChoiceListCurrentItem(EVideoIdFileFormatCompression);

	TBool bppVisible = ETrue;
	TBool colorVisible = ETrue;
	TBool factorVisible = EFalse;
	TBool samplingVisible = EFalse;
	TBool compressionVisible = EFalse;


	const TUid imageType = iEncoderList->ImageType(type);

	if(imageType == KImageTypeBMPUid)
		{
		colorVisible = EFalse;
		if (bpp == 1)
			bpp = 2;
		else if ((bpp > 3) && (bpp < 6))
			bpp = 6;
		else if (bpp > 6)
			bpp = 0;
		}
	else if(imageType == KImageTypeGIFUid)
		{
		bppVisible = EFalse;
		colorVisible = EFalse;
		}
	else if(imageType == KImageTypeJPGUid)
		{
		bppVisible = EFalse;
		factorVisible = ETrue;
		if (color)
			samplingVisible = ETrue;
		}
	else if(imageType == KImageTypeMBMUid)
		{
		if (color == 0)
			{
			if (bpp > 3)
				bpp = 0;
			}
		else
			{
			if (bpp < 2)
				bpp = 2;
			}
		}
	else if(imageType == KImageTypePNGUid)
		{
		if ((bpp > 3) && (bpp < 6))
			bpp = 6;
		else if (bpp > 6)
			bpp = 0;
		compressionVisible = ETrue;
		}
	else //Custom encoder
		{
		bppVisible = EFalse;
		colorVisible = EFalse;
		}

	SetChoiceListCurrentItem(EVideoIdFileFormatBpp,bpp);
	SetChoiceListCurrentItem(EVideoIdFileFormatColor,color);
	SetChoiceListCurrentItem(EVideoIdFileFormatCompression,compression);

	MakeLineVisible(EVideoIdFileFormatBpp,bppVisible);
	MakeLineVisible(EVideoIdFileFormatColor,colorVisible);
	MakeLineVisible(EVideoIdFileFormatFactor,factorVisible);
	MakeLineVisible(EVideoIdFileFormatSampling,samplingVisible);
	MakeLineVisible(EVideoIdFileFormatCompression,compressionVisible);
	}

TBool CVideoFormatDialog::OkToExitL(TInt /*aButtonId*/)
	{
	TInt type = ChoiceListCurrentItem(EVideoIdFileFormatType);
	iSaveInfo.iImageTypeUid = iEncoderList->ImageType(type);
	iSaveInfo.iBpp = ChoiceListCurrentItem(EVideoIdFileFormatBpp);
	iSaveInfo.iColor = ChoiceListCurrentItem(EVideoIdFileFormatColor);
	iSaveInfo.iQualityFactor = NumberEditorValue(EVideoIdFileFormatFactor);
	iSaveInfo.iSampling = ChoiceListCurrentItem(EVideoIdFileFormatSampling);
	iSaveInfo.iCompression = ChoiceListCurrentItem(EVideoIdFileFormatCompression);

	return ETrue;
	}
	
CDecoderOptionsDialog::CDecoderOptionsDialog(TUint& aOptions):iOptions(aOptions)
	{
	}

	// from CEikDialog
void CDecoderOptionsDialog::PreLayoutDynInitL()
	{
	SetCheckBoxState(EVideoIdUseThreadForDecoder, iOptions&CImageDecoder::EOptionAlwaysThread ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	SetCheckBoxState(EVideoIdDecoderDisableDithering,  iOptions&CImageDecoder::EOptionNoDither ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	SetCheckBoxState(EVideoIdDecoderAutogenMask, iOptions&CImageDecoder::EAllowGeneratedMask ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	SetCheckBoxState(EVideoIdDecodeHighSpeedDecode, iOptions&CImageDecoder::EPreferFastDecode ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	SetCheckBoxState(EVideoIdAutoRotateDecode, iOptions&CImageDecoder::EOptionAutoRotate ? CEikButtonBase::ESet : CEikButtonBase::EClear);
	}
	
TBool CDecoderOptionsDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iOptions = CheckBoxState(EVideoIdUseThreadForDecoder) 		== CEikButtonBase::ESet ? CImageDecoder::EOptionAlwaysThread : 0;
	iOptions|= CheckBoxState(EVideoIdDecoderDisableDithering) 	== CEikButtonBase::ESet ? CImageDecoder::EOptionNoDither : 0;
	iOptions|= CheckBoxState(EVideoIdDecoderAutogenMask) 		== CEikButtonBase::ESet ? CImageDecoder::EAllowGeneratedMask : 0;
	iOptions|= CheckBoxState(EVideoIdDecodeHighSpeedDecode) 	== CEikButtonBase::ESet ? CImageDecoder::EPreferFastDecode : 0;
	iOptions|= CheckBoxState(EVideoIdAutoRotateDecode) 			== CEikButtonBase::ESet ? CImageDecoder::EOptionAutoRotate : 0;
	
	return ETrue;
	}

CExtensionOptionsDialog::CExtensionOptionsDialog(TRect& aClippingRect,
							RArray<TInt>& aOperations,
							TInt& aScalingCoefficient,
							TImageConvScaler::TScalerQuality& aScalingQuality,
							TBool& aLockAspectRatio) 
 : iClippingRect(&aClippingRect), iOperations(aOperations), iScalingCoefficient(&aScalingCoefficient), 
   iScalingQuality(&aScalingQuality), iLockAspectRatio(&aLockAspectRatio)
	{
	iSelectedOperations = new (ELeave) CDesCArrayFlat(20);
	}

CExtensionOptionsDialog::CExtensionOptionsDialog(RArray<TInt>& aOperations) 
 : iOperations(aOperations)
	{
	iSelectedOperations = new (ELeave) CDesCArrayFlat(20);
	iOperationOnly = ETrue;
	}

void CExtensionOptionsDialog::PageChangedL(TInt /*aPageId*/)
	{
	// enable/disable add and clear buttons which are only enabled on Operation Page
// The lines below panic on teh Control call. Looks like a dialog button is not a control
//	Control(EVideoIdAdd)->SetDimmed(aPageId != EDecoderExtensionOperationPage);
//	Control(EVideoIdClear)->SetDimmed(aPageId != EDecoderExtensionOperationPage);
	}

	// from CEikDialog
void CExtensionOptionsDialog::PreLayoutDynInitL()
	{
	if ( !iOperationOnly )
		{
		// crop rect number edits
		static_cast<CEikNumberEditor*>(Control(EVideoIdCropTopLeftX))->SetNumber(iClippingRect->iTl.iX);
		static_cast<CEikNumberEditor*>(Control(EVideoIdCropTopLeftY))->SetNumber(iClippingRect->iTl.iY);	
		static_cast<CEikNumberEditor*>(Control(EVideoIdCropWidth))->SetNumber(iClippingRect->Width());	
		static_cast<CEikNumberEditor*>(Control(EVideoIdCropHeight))->SetNumber(iClippingRect->Height());
		
		/* scaling
			coefficient number edit
			quality choicelist
			aspectratio checkbox
	    */
		static_cast<CEikNumberEditor*>(Control(EVideoIdScaleCoeff))->SetNumber(*iScalingCoefficient);	
		static_cast<CEikChoiceList*>(Control(EVideoIdScaleQuality))->SetCurrentItem(*iScalingQuality);	
		SetCheckBoxState(EVideoIdScalePreserveAspect, *iLockAspectRatio ? CEikButtonBase::ESet : CEikButtonBase::EClear);
		}
			
	/* operations
		fill selected operations
	*/
	CEikTextListBox* selectOpsListBox = static_cast<CEikTextListBox*>(Control(EVideoIdSelectedOperations));
	
	selectOpsListBox->CreateScrollBarFrameL();
	selectOpsListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOn);
	
	operationName[0] = _L("0x01 | ERotation90DegreesClockwise");
	operationName[1] = _L("0x02 | ERotation180DegreesClockwise");
	operationName[2] = _L("0x04 | ERotation270DegreesClockwise");
	operationName[3] = _L("0x08 | EMirrorHorizontalAxis");
	operationName[4] = _L("0x10 | EMirrorVerticalAxis");
		
	for (TInt i = 0; i < iOperations.Count(); i++)
		{
		if ( iOperations[i] > 0 )
			{
			TInt temp = iOperations[i];
			TInt shiftCount = 0;
			while ( temp )
				{
				temp = temp >> 1;
				++shiftCount;
				}
			iSelectedOperations->AppendL(operationName[shiftCount-1]);
			}
		}
	
	selectOpsListBox->Model()->SetItemTextArray(iSelectedOperations);
	selectOpsListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);
 	
	}

// DEF115967: Panic while editing Setup extensions Value
void CExtensionOptionsDialog::PrepareForFocusTransitionL()
	{
	// The dialog will check for blank input in the control that
	// has been edited. If it is blank the value 9999...9 will be
	// placed there to ensure control->Number() returns something.
	// There appears to be no way of getting a different value put
	// in a blank control (IsBlank function is protected).

	CEikDialog::PrepareForFocusTransitionL();
	}


SEikControlInfo CExtensionOptionsDialog::CreateCustomControlL(TInt /*aControlType*/)
	{
	/* never called */
	SEikControlInfo info;
	info.iTrailerTextId = 0;
	info.iFlags = 0;
	info.iControl = new (ELeave) CEikTextListBox;
	return info;
	}
		
TBool CExtensionOptionsDialog::OkToExitL(TInt aButtonId)
	{
	TBool exit = EFalse;
	
	switch ( aButtonId )
		{
	case EEikBidOk:
		{
		if ( !iOperationOnly )
			{
			// get crop rect 
			iClippingRect->iTl.iX = static_cast<CEikNumberEditor*>(Control(EVideoIdCropTopLeftX))->Number();
			iClippingRect->iTl.iY = static_cast<CEikNumberEditor*>(Control(EVideoIdCropTopLeftY))->Number();	
			iClippingRect->SetWidth(static_cast<CEikNumberEditor*>(Control(EVideoIdCropWidth))->Number());	
			iClippingRect->SetHeight(static_cast<CEikNumberEditor*>(Control(EVideoIdCropHeight))->Number());

			// get scaling
			*iScalingCoefficient = static_cast<CEikNumberEditor*>(Control(EVideoIdScaleCoeff))->Number();	
			*iScalingQuality = static_cast<TImageConvScaler::TScalerQuality>
				(static_cast<CEikChoiceList*>(Control(EVideoIdScaleQuality))->CurrentItem());	
			*iLockAspectRatio = (CheckBoxState(EVideoIdScalePreserveAspect) == CEikButtonBase::ESet) ? ETrue : EFalse;
			}
			
		// get operations 
		CTextListBoxModel * selectOpsModel = static_cast<CEikTextListBox*>(Control(EVideoIdSelectedOperations))->Model();
		iOperations.Reset();
		for (TInt i = 0; i < selectOpsModel->NumberOfItems(); i++)
			{
			TLex enumLex(selectOpsModel->ItemText(i).Mid(2,2)); // Hex value of enum
			TUint operation = 0;
			enumLex.Val(operation,EHex);
			iOperations.Append(operation);
			}
		}
		// deliberate fall through
	case EEikBidCancel:
		{
		// iOperations fill up from iSeletedOperations
		delete iSelectedOperations; 
		exit = ETrue;
		}
		break;

	case EVideoIdAdd:
		{
		TInt currentItem = static_cast<CEikChoiceList*>(Control(EVideoIdOperations))->CurrentItem();
		iSelectedOperations->AppendL(operationName[currentItem]);
		CEikTextListBox* selectOpsListBox = static_cast<CEikTextListBox*>(Control(EVideoIdSelectedOperations));
		selectOpsListBox->Model()->SetItemTextArray(iSelectedOperations);
		selectOpsListBox->HandleItemAdditionL();
		}
		break;				

	case EVideoIdClear:
		{
		iSelectedOperations->Reset();
		CEikTextListBox* selectOpsListBox = static_cast<CEikTextListBox*>(Control(EVideoIdSelectedOperations));
		selectOpsListBox->Model()->SetItemTextArray(iSelectedOperations);
		selectOpsListBox->HandleItemAdditionL();		
		}
		break;
		
	default:
		break;
		}

	return exit;
	}

//
// CVideoWalker implementation
//

CVideoWalker* CVideoWalker::NewL(CVideoAppUi* aAppUi)
	{
	CVideoWalker* result = new (ELeave) CVideoWalker(aAppUi);
	return result;
	}

CVideoWalker::CVideoWalker(CVideoAppUi* aAppUi):
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

