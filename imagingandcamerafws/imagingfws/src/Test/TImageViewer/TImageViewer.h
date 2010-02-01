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

#ifndef __TVIDEO_H__
#define __TVIDEO_H__

#include <eikappui.h>
#include <eikapp.h>
#include <techview/eikcfdlg.h>
#include <eikdoc.h>

#include <imageconversion.h>
#include <bitmaptransforms.h>

#include <timageviewer.rsg>
#include "TImageViewer.hrh"

const TUid KUidTVideo={ 0x101F45EA };

// class CVideoAppView

class CVideoAppView : public CCoeControl, public MCoeControlBrushContext
    {
public:
	static CVideoAppView* NewL(const TRect& aRect);
	~CVideoAppView();
	void Reset(TDrawNow aDrawNow=EDrawNow);
	void DrawImage(CFbsBitmap* aBitmap, const TPoint& aOffset, TDrawNow aDrawNow=EDrawNow);
	void DrawImage(CFbsBitmap* aBitmap, CFbsBitmap* aMask, const TPoint& aOffset, TDrawNow aDrawNow=EDrawNow);
	TBool ResizeL(const TSize& aNewSize, TBool aClear=ETrue, TDrawNow aDrawNow=EDrawNow);
	void Clear(TBool aClearFull=ETrue, TDrawNow aDrawNow=EDrawNow);
	void MoveBy(const TPoint& aRelMove, TDrawNow aDrawNow=EDrawNow);
	void Center(TDrawNow aDrawNow=EDrawNow);
	TBool SetBackgroundColor(const TRgb& aColor, TDrawNow aDrawNow=EDrawNow);
	void SetDisplayModeL(TDisplayMode aDisplayMode, CWsBitmap* aFrame, TDrawNow aDrawNow=EDrawNow);
	TDisplayMode DisplayMode() const { return iDisplayMode; }
protected:
	CVideoAppView();
    void ConstructL(const TRect& aRect);
	void ResizeBufferL(const TSize& aNewSize, TDisplayMode aDisplayMode);
	void EnsureSizeInTwipsSet(CFbsBitmap* aBitmap) const;
// from CCoeControl
	void Draw(const TRect& aRect) const;
protected:
	CFbsBitmap* iBmBuffer;
	CFbsDevice* iBmDevice;
	CFbsBitGc*	iBmGc;
	TRect		iBmRect;
	TRgb		iBackgroundColor;
	TBool		iBitmapValid;
	TDisplayMode iDisplayMode;
    };

class CPluginInfoArray : public CBase, public MDesCArray
	{
public:
	static CPluginInfoArray* NewL();
	virtual ~CPluginInfoArray();

	void Reset();

	void RefreshPluginListL();
	TUid ImageType(TInt aIndex);
	TUid SubType(TInt aIndex);

	//From MDesCArray
    TInt MdcaCount() const;
    TPtrC MdcaPoint(TInt aIndex) const;

private:
	CPluginInfoArray();
	void ConstructL();

public:
	RImageTypeDescriptionArray iPluginArray;
	};

class TFileSaveInfo
	{
public:
	TUid iImageTypeUid;
	TInt iBpp;
	TBool iColor;
	TInt iQualityFactor;
	TInt iSampling;
	TInt iCompression;
	};

class CVideoWalker;

// CVideoAppUi

class CVideoAppUi : public CEikAppUi
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
		EPlayingWait,
		EScalingMask,
		EScaling,
		ERotatingMask,
		ERotating,
		ESaving,
		EStreamOpening,
		EStreamWaitForFirstFrame,
		EStreamWaitForDetails,
		EStreamDecoding,
		EStreamWaitAndContinue,
		EStreamDecodeWait,
		EFolderWait
		};
public:
    void ConstructL();
	CVideoAppUi();
	~CVideoAppUi();
private:
	// From CCoeAppUi
	virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	// From CEikAppUi
	virtual void HandleCommandL(TInt aCommand);
	virtual TBool ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail);
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	TBool CheckHotKeyNotDimmedL(TInt aCommandId);
	// New functions
	void OpenFileL(TFileType aFileType);
	void LoadFileL();
	void SaveAsL();
	void DisplayModeL();
	void BackgroundColorL();
	void ExtractFrameL();
	void PlayClipL();
	void NextFrameL();
	void StreamPlayL(TFileType aFileType = EFileTypeUnknown, TBool aMultiFrameStreaming=EFalse);
	void FrameInfoL();
	void FrameRotateL(TBool aClockwise);
	void FrameMirrorL(TBool aHorizontalAxis);
	void ZoomFrameL(TBool aZoomIn);
	void SetFrameSizeAndPosL(const TRect& aFrameRect,const TSize& aOverallSize);
	void DecoderOptionsL();
	void ExtensionOptionsL();

	void RunL(CVideoWalker* aWalker, TInt aStatus);
	void DoCancel(CVideoWalker* aWalker);
	void Cancel();

	void HandleNewlyOpenedImageL();
	void HandleConvertCompleteL();
	void DrawConvertedFrameL();
	void ResizeBitmapsToSuitL();
	void StartFrameOpen(TState aNextState, TBool aSizeFrame=ETrue);
	void DoStartFrameOpenL(TBool aSizeFrame);
	void StartFrameSave();

	TInt ExtendStreamBuffer();
	void StreamOpenFrameIfPosL();
	void HandleRunError(CVideoWalker* aWalker, TInt aError);

	void OpenFolderL();
	void LoadFolderWait();
	void LoadFolderL(const TDesC& aFolderName);
	void OpenNextFolderEntry();
	
	TBool ExtensionIsSetup();

	void UpdateClippingRect(TUint aKeycode);
	
	static void ReplaceNewlinesWithLineBreaks(TDes& aDes);

private:
	CWsBitmap iFrame;
	CWsBitmap iMask;
	TSize iImageDisplaySize;
	TPoint iOffset;
	TInt iFrameNumber;
	TInt iPrevFrameNumber;
    CVideoAppView* iAppView;
	TInt iZoomFactor;
	TFileName iLoadFileName;
	TFileName iSaveFileName;
	TFileSaveInfo iSaveInfo;
	TTime iTime;
	TInt iBackgroundColor;
	TBool iOverrideBackgroundColor;
	TSize iOverallSize;
	TBool iAnimating;
	TFileType iLastFileType;
	TUint32 iLastFrameFlags;
	TSize iScaleSize;
	CBitmapRotator::TRotationAngle iRotateAngle;
	TBool iViewResized;
	TBool iDisableMask;
	TUint iDecoderOptions;
	TBool iCreateThumbnail;
	TBool iSaveAsEXIF;
	TBool iUseNativeDisplayMode;	
	
	CBitmapRotator* iRotator;
	CBitmapScaler* iScaler;
	CImageDecoder* iLoadUtil;
	CImageEncoder* iSaveUtil;
	CFrameImageData* iFrameImageData; 
	TState iState;
	CVideoWalker* iWalker;
	RTimer iTimer;

	// for streaming simulation
	CVideoWalker* iStreamGen;
	RTimer iStreamTimer;
	TPtr8 iStreamBuffer;
	TInt64 iStreamSeed;
	TBool iMultiFrameStreaming;
	TBool iSourceHasGrown;

	// for folder opens
	TFileName iDirName;
	CDir* iDir;
	TInt iDirIndex;
	TBool iOpeningFolder;

	// extensions
	TRect iClippingRect; // clipping rectangle
	RArray<TInt> iOperations; // list of operations
	TInt iScalingCoefficient; // scaling
	TImageConvScaler::TScalerQuality iScalingQuality; // scaling
	TBool iLockAspectRatio;   // scaling
	RArray<TInt> iEncodeOperations; // list of operations
	
	TBool iUseExtensions; 
    };

// CVideoDisplayModeDialog

class CVideoDisplayModeDialog : public CEikDialog
	{
public:
	CVideoDisplayModeDialog(TDisplayMode& aDisplayMode,TBool& aUseNativeDisplayMode);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TDisplayMode& iDisplayMode;
	TBool& iUseNativeDisplayMode;
	};

// CVideoBackgroundColorDialog

class CVideoBackgroundColorDialog : public CEikDialog
	{
public:
	CVideoBackgroundColorDialog(TInt& aColor16, TBool& aOverride);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	void HandleControlStateChangeL(TInt aControlId);
private:
	TInt& iColor16;
	TBool& iOverride;
	};

// CVideoCurrentFrameDialog

class CVideoCurrentFrameDialog : public CEikDialog
	{
public:
	CVideoCurrentFrameDialog(TInt& aCurrentFrame,TInt aNumberOfFrames);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TInt& iCurrentFrame;
	TInt iNumberOfFrames;
	};

// CVideoSaveAsDialog

class CVideoSaveAsDialog : public CEikFileSaveAsDialog
	{
public:
	CVideoSaveAsDialog(TDes* aFileName,TFileSaveInfo& aSaveInfo,RArray<TInt>& aEncodeOperations,TBool& aCreateThumbnail,TBool& aSaveAsEXIF);
	virtual SEikControlInfo CreateCustomControlL(TInt aControlType);
protected:
	// from CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	virtual void HandleControlStateChangeL(TInt aControlId);
	// new functions
	void SetTypeL();
private:
	TFileSaveInfo& iSaveInfo;
	RArray<TInt>& iEncodeOperations;
	TBool* iCreateThumbnail;
	TBool* iSaveAsEXIF;
	};


// CVideoFormatDialog

class CDecoderOptionsDialog : public CEikDialog
	{
public:
	CDecoderOptionsDialog(TUint& aOptions);
protected: 
	// from CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TUint& iOptions;
	};

class CExtensionOptionsDialog : public CEikDialog, public MEikDialogPageObserver
	{
public:
	CExtensionOptionsDialog(TRect& aClippingRect,
							RArray<TInt>& aOperations,
							TInt& aScalingCoefficient,
							TImageConvScaler::TScalerQuality& aQuality,
							TBool& aLockAspectRatio);
	CExtensionOptionsDialog(RArray<TInt>& aOperations);
							
	virtual void PrepareForFocusTransitionL();
	virtual void PageChangedL(TInt aPageId);
	virtual void LineChangedL(TInt /*aControlId*/) {};
	virtual SEikControlInfo CreateCustomControlL(TInt aControlType);
	virtual void GetCustomAutoValue(TAny* /*aReturnValue*/,TInt /* aControlType*/,const CCoeControl* /*aControl*/) {};
								
protected: 
	// from CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TRect* iClippingRect; // clipping rectangle
	RArray<TInt>& iOperations; // list of operations
	// scaling
	TInt* iScalingCoefficient; 
	TImageConvScaler::TScalerQuality* iScalingQuality;
	TBool* iLockAspectRatio;
	CDesCArrayFlat* iSelectedOperations;
	TBuf<128> operationName[5];
	TBool iOperationOnly; // only operations tab
	};
	
// CVideoFormatDialog

class CVideoFormatDialog : public CEikDialog
	{
public:
	CVideoFormatDialog(TFileSaveInfo& aSaveInfo);
protected: 
	// from CEikDialog
	void PreLayoutDynInitL();
	void HandleControlStateChangeL(TInt aControlId);
	TBool OkToExitL(TInt aButtonId);
	// new functions
	void ValidateControlState();
private:
	TFileSaveInfo& iSaveInfo;
	CPluginInfoArray* iEncoderList; //Not owned
	};

// CVideoDocument

class CVideoDocument : public CEikDocument
	{
public:
	CVideoDocument(CEikApplication& aApp);
private:
	 // from CEikDocument
	CEikAppUi* CreateAppUiL();
	};

// CVideoApp

class CVideoApp : public CEikApplication
	{
private:
	// from CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};

#endif

