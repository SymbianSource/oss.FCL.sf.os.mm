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
#include <eikcfdlg.h>
#include <eikdoc.h>

#include <Mda\Client\Resource.h>
#include <MdaImageConverter.h>

#include <TVideo.rsg>
#include "TVideo.hrh"

const TUid KUidTVideo={ 268450181 };

_LIT(KVideoFileFormatBmp,"Windows bmp");
_LIT(KVideoFileFormatGif,"Compuserve gif");
_LIT(KVideoFileFormatJpeg,"Jpeg (jfif)");
_LIT(KVideoFileFormatMbm,"Symbian mbm");

// class CVideoAppView

class CVideoAppView : public CCoeControl, public MCoeControlBrushContext
    {
public:
	CVideoAppView();
	~CVideoAppView();
    void ConstructL(const TRect& aRect);
	void DrawFrame() const;
	void Refresh();
	void Clear();
	void Move(const TPoint& aMove);
private:
	// from CCoeControl
	void Draw(const TRect& /*aRect*/) const;
private:
	CFbsBitmap* iBitmap;
	CFbsBitmap* iMask;
	TPoint iPos;
	TPoint iOffset;
	friend class CVideoAppUi;
    };

// CVideoAppUi

class TFileSaveInfo
	{
public:
	TInt iType;
	TInt iBpp;
	TBool iColor;
	TInt iQualityFactor;
	TInt iSampling;
	};

class CVideoAppUi : public CEikAppUi, public MMdaImageUtilObserver
    {
public:
	enum TFileType
		{
		EFileTypeUnknown,
		EFileTypeWbmp,
		EFileTypeOta
		};
    void ConstructL();
	~CVideoAppUi();
private:
	// From CCoeAppUi
	virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	// From CEikAppUi
	virtual void HandleCommandL(TInt aCommand);
	virtual TBool ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail);
	// New functions
	void OpenFileL(TFileType aFileType);
	void LoadFileL();
	void SaveAsL();
	void DisplayModeL();
	void BackgroundColorL();
	void ExtractFrameL();
	void PlayClipL();
	static TInt NextFrame(TAny* aThis);
	void NextFrameL();
	void FrameRotateL(TBool aClockwise);
	void FrameFlipL();
	void ZoomFrameL(TBool aZoomIn);
	void ResizeFrameL(const TSize& aNewSize, TDisplayMode aNewMode);
	void SetFrameSizeAndPosL(const TRect& aFrameRect,const TSize& aOverallSize);

	// From MMdaImageUtilObserver
	virtual void MiuoCreateComplete(TInt aError);
	virtual void MiuoOpenComplete(TInt aError);
	virtual void MiuoConvertComplete(TInt aError);

	void HandleMiuoCreateCompleteL();
	void HandleMiuoOpenCompleteL();
	void HandleMiuoConvertCompleteL(TInt aError);
private:
	enum TState
		{
		EIdle,
		ELoading,
		EPlaying,
		ESaving
		};
private:
	CWsBitmap iFrame;
	CWsBitmap iMask;
	TInt iFrameNumber;
    CVideoAppView* iAppView;
	TInt iZoomFactor;
	TMdaFileClipLocation iLoadFileName;
	TMdaFileClipLocation iSaveFileName;
	TFileSaveInfo iSaveInfo;
	TTime iTime;
	TInt iBackgroundColor;
	TSize iOverallSize;

	CMdaBitmapRotator* iRotator;
	CMdaBitmapScaler* iScaler;
	CMdaImageDataReadUtility* iLoadUtil;
	CMdaImageDataWriteUtility* iSaveUtil;
	TState iState;

	TMdaBmpClipFormat iBmpFormat;
	TMda1BppBmpCodec iBmp1BppCodec;
	TMda4BppBmpCodec iBmp4BppCodec;
	TMda8BppBmpCodec iBmp8BppCodec;
	TMda24BppBmpCodec iBmp24BppCodec;
	TMdaGif87aClipFormat iGifFormat;
	TMdaGifCodec iGifCodec;
	TMdaJfifClipFormat iJfifFormat;
	TMdaMbmClipFormat iMbmFormat;
	TMdaWbmpClipFormat iWbmpFormat;
	TMdaOtaClipFormat iOtaFormat;

	TFileType iLastFileType;
	TUint32 iLastFrameFlags;

    TReal iDisplayXTwipsPerPixel;
	TReal iDisplayYTwipsPerPixel;
	};

// CVideoDisplayModeDialog

class CVideoDisplayModeDialog : public CEikDialog
	{
public:
	CVideoDisplayModeDialog(TDisplayMode& aDisplayMode);
	// From CEikDialog
	virtual void PreLayoutDynInitL();
	virtual TBool OkToExitL(TInt aButtonId);
private:
	TDisplayMode& iDisplayMode;
	};

// CVideoBackgroundColorDialog

class CVideoBackgroundColorDialog : public CEikDialog
	{
public:
	CVideoBackgroundColorDialog(TInt& aColor16);
	// From CEikDialog
	virtual void PreLayoutDynInitL();
	virtual TBool OkToExitL(TInt aButtonId);
private:
	TInt& iColor16;
	};

// CVideoCurrentFrameDialog

class CVideoCurrentFrameDialog : public CEikDialog
	{
public:
	CVideoCurrentFrameDialog(TInt& aCurrentFrame,TInt aNumberOfFrames);
protected:
	// From CEikDialog
	virtual void PreLayoutDynInitL();
	virtual TBool OkToExitL(TInt aButtonId);
private:
	TInt& iCurrentFrame;
	TInt iNumberOfFrames;
	};

// CVideoSaveAsDialog

class CVideoSaveAsDialog : public CEikFileSaveAsDialog
	{
public:
	CVideoSaveAsDialog(TDes* aFileName,TFileSaveInfo& aSaveInfo);
protected:
	// from CEikDialog
	virtual void PreLayoutDynInitL();
	virtual TBool OkToExitL(TInt aButtonId);
	// new functions
	void SetTypeL();
private:
	TFileSaveInfo& iSaveInfo;
	};

// CVideoFormatDialog

class CVideoFormatDialog : public CEikDialog
	{
public:
	CVideoFormatDialog(TFileSaveInfo& aSaveInfo);
protected: // from CEikDialog
	virtual void PreLayoutDynInitL();
	virtual void HandleControlStateChangeL(TInt aControlId);
	virtual TBool OkToExitL(TInt aButtonId);
	// new functions
	void ValidateControlState();
private:
	TFileSaveInfo& iSaveInfo;
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

