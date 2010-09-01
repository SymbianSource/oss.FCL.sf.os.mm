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

#ifndef TIMAGEDISPLAYDLGS_H

#define TIMAGEDISPLAYDLGS_H


#include <techview/eikcfdlg.h>
#include <timagedisplay.rsg>
#include "TImageDisplay.hrh"

class CPluginInfoArray;

// CDisplayModeDialog
class CDisplayModeDialog : public CEikDialog
	{
public:
	CDisplayModeDialog(TDisplayMode& aDisplayMode);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TDisplayMode& iDisplayMode;
	};

class CScalingQualityDialog : public CEikDialog
	{
public:
	CScalingQualityDialog(TInt& aQualityValueRef);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TInt& iQualityValueRef;
	};

class CDrmContentIdDialog : public CEikDialog
	{
public:
	CDrmContentIdDialog(TDes& aContentId);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TDes& iContentId;
	};

class CClipRectParamsDialog : public CEikDialog
	{
public:
	CClipRectParamsDialog(TBool& aEnabled, TRect& aClipRect);
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	void HandleControlStateChangeL(TInt aControlId);

private:
	TBool& iEnabled;
	TRect& iClipRect;
	};

class CMaxReductionDialog: public CEikDialog
	{
public:
	CMaxReductionDialog(TInt& aMaxReduction );
protected:
	// From CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	TInt& iMaxReduction;
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

class CVideoSaveAsDialog : public CEikFileSaveAsDialog
	{
public:
	CVideoSaveAsDialog(TDes* aFileName,TFileSaveInfo& aSaveInfo);
protected:
	// from CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
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

#endif // ndef TIMAGEDISPLAYDLGS_H
