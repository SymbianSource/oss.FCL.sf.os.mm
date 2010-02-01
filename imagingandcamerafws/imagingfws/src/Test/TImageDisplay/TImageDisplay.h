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

#ifndef TIMAGEDISPLAY_H
#define TIMAGEDISPLAY_H

#include <eikappui.h>
#include <eikapp.h>

#include <eikdoc.h>

#include <imageconversion.h>
#include <imagedisplay.h>
#include "app_uid.h"

const TUid KUidTVideo={ APPUID };

// class CImgDisplayAppView
class CImageDisplay;
class CDisplayModeDialog;
class CImgDisplayAppView;

class CVideoWalker;

// CImgDisplAppUi

class CImgDisplAppUi : public CEikAppUi, public MIclImageDisplayObserver
    {
	friend class CVideoWalker;
private:
	enum TFileType
		{
		EFileTypeUnknown,
		EFileTypeWbmp,
		EFileTypeOta
		};
	enum TState
		{
		EIdle,
		ELoading,
		EPlaying,
		EPlayPaused,
		EPlayingWait,
		ESaving,
		EFolderWait
		};
public:
    void ConstructL();
	CImgDisplAppUi();
	~CImgDisplAppUi();
protected:
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError) ;
private:
	// From CCoeAppUi
	virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	// From CEikAppUi
	virtual void HandleCommandL(TInt aCommand);
	virtual TBool ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail);
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	TBool CheckHotKeyNotDimmedL(TInt aCommandId);
	TUint ImageOptions() const;

	void OpenFileL(TFileType aFileType);
	void LoadFileL();
	void SaveAsL();
	void DisplayModeL();
	void ScalingQualityL();
	void ClipRectParamsL();
	void DrmParamsL();
	void BackgroundColorL();
	void ExtractFrameL();
	void Pause();
	void PlayClipL();
	void NextFrameL();
	void StreamPlayL(TFileType aFileType = EFileTypeUnknown, TBool aMultiFrameStreaming=EFalse);
	void FrameInfoL();
	void FrameRotateL(TBool aClockwise);
	void FrameFlipL(TBool aVertical);
	void AutoRotateL();
	void MaxReductionL();

	void RunL(CVideoWalker* aWalker, TInt aStatus);
	void DoCancel(CVideoWalker* aWalker);
	void Cancel();

	void HandleNewlyOpenedImageL();
	void HandleConvertCompleteL();
	void DrawConvertedFrame();
	void ResizeBitmapsToSuitL();
	void StartFrameOpen(TState aNextState);
	void StartFrameSave();

	void HandleRunError(CVideoWalker* aWalker, TInt aError);

	void OpenFolderL();
	void LoadFolderWait();
	void LoadFolderL(const TDesC& aFolderName);
	void OpenNextFolderEntry();

	void MakeName(const TDesC& aDir, TBool aIsMask, TDes& aFileName) const;

	static void ReplaceNewlinesWithLineBreaks(TDes& aDes);

private:
	CImageDisplay* iImageDisplay;
	const CFbsBitmap* iFrame;	// not owned
	const CFbsBitmap* iMask;	// not owned
	TSize iImageDisplaySize;
	TPoint iOffset;
	TInt iFrameNumber;
	TInt iPrevFrameNumber;
	TInt iRotationAngle;
	TUint iLatestStatus;
	TInt iLatestError;
	TBool iUseThumbnail;
	TBool iPauseAfterEachFrame;
	TFileName iDrmContentId;
	ContentAccess::TIntent iDrmIntent;
	TBool iClippingSet;
	TRect iClipRect;
	TInt  iScalingQuality;
    CImgDisplayAppView* iAppView;
	TInt iZoomFactor;
	TFileName iLoadFileName;
	TFileName iSaveFileName;
	TFileSaveInfo iSaveInfo;
	TTime iTime;
	TInt iBackgroundColor;
	TBool iOverrideBackgroundColor;
	TSize iOverallSize;
	TFileType iLastFileType;
	TUint32 iLastFrameFlags;
	TSize iScaleSize;
	TBool iViewResized;
	TBool iDisableMask;
	TBool iDumpFrames;
	TInt  iMaxReduction;

	CImageEncoder* iSaveUtil;
	CFrameImageData* iFrameImageData; 
	TState iState;
	CVideoWalker* iWalker;
	RTimer iTimer;

	// for folder opens
	TFileName iDirName;
	CDir* iDir;
	TInt iDirIndex;
	TBool iOpeningFolder;
    };

// CImgDisplDocument

class CImgDisplDocument : public CEikDocument
	{
public:
	CImgDisplDocument(CEikApplication& aApp);
private:
	 // from CEikDocument
	CEikAppUi* CreateAppUiL();
	};

// CImgDisplApp

class CImgDisplApp : public CEikApplication
	{
private:
	// from CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};

#endif

