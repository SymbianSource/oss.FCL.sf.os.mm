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
// CImgDisplayAppView
// 
//

#ifndef TIMAGEDISPLAYAPPVIEW_H

#define TIMAGEDISPLAYAPPVIEW_H

#include <coecntrl.h>
#include <coeccntx.h>

class CImgDisplayAppView : public CCoeControl, public MCoeControlBrushContext
    {
public:
	static CImgDisplayAppView* NewL(const TRect& aRect);
	~CImgDisplayAppView();
	void Reset(TDrawNow aDrawNow=EDrawNow);
	void DrawImage(const CFbsBitmap* aBitmap, const TPoint& aOffset, TDrawNow aDrawNow=EDrawNow);
	void DrawImage(const CFbsBitmap* aBitmap, const CFbsBitmap* aMask, const TPoint& aOffset, TDrawNow aDrawNow=EDrawNow);
	TBool ResizeL(const TSize& aNewSize, TBool aClear=ETrue, TDrawNow aDrawNow=EDrawNow);
	void Clear(TBool aClearFull=ETrue, TDrawNow aDrawNow=EDrawNow);
	void MoveBy(const TPoint& aRelMove, TDrawNow aDrawNow=EDrawNow);
	void Center(TDrawNow aDrawNow=EDrawNow);
	TBool SetBackgroundColor(const TRgb& aColor, TDrawNow aDrawNow=EDrawNow);
	void SetDisplayModeL(TDisplayMode aDisplayMode, TDrawNow aDrawNow=EDrawNow);
	TDisplayMode DisplayMode() const { return iDisplayMode; }
protected:
	CImgDisplayAppView();
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

#endif // ndef TIMAGEDISPLAYAPPVIEW_H

