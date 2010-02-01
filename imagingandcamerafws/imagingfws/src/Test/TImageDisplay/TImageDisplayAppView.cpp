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

#include <eikenv.h>

#include "TImageDisplayAppView.h"

CImgDisplayAppView* CImgDisplayAppView::NewL(const TRect& aRect)
	{
	CImgDisplayAppView* self = new (ELeave) CImgDisplayAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}

CImgDisplayAppView::CImgDisplayAppView():
	CCoeControl()
	{}

void CImgDisplayAppView::ConstructL(const TRect& /*aRect*/)
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

CImgDisplayAppView::~CImgDisplayAppView()
	{
	delete iBmGc;
	delete iBmDevice;
	delete iBmBuffer;
	}

void CImgDisplayAppView::Draw(const TRect& aRect) const
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

void CImgDisplayAppView::Reset(TDrawNow aDrawNow)
	{
	iBmBuffer->Reset();
	iBitmapValid = EFalse;
	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

void CImgDisplayAppView::DrawImage(const CFbsBitmap* aBitmap, const TPoint& aOffset, TDrawNow aDrawNow)
	{
	DrawImage(aBitmap, NULL, aOffset, aDrawNow);
	}

void CImgDisplayAppView::DrawImage(const CFbsBitmap* aBitmap, const CFbsBitmap* aMask, const TPoint& aOffset, TDrawNow aDrawNow)
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
	//ASSERT(intersection==bitmapRect);
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
		RWindow& window = Window();
		window.Invalidate(bitmapRect);
		window.BeginRedraw(bitmapRect);
		if (aMask)
			gc.BitBltMasked(screenOffset, aBitmap, TRect(aBitmap->SizeInPixels()), aMask, EFalse);
		else
			gc.BitBlt(screenOffset, aBitmap);
		window.EndRedraw();
		DeactivateGc();
		}
	}

TBool CImgDisplayAppView::ResizeL(const TSize& aNewSize, TBool aClear, TDrawNow aDrawNow)
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
		if (aClear)
			Clear(EFalse, aDrawNow);
		else if (aDrawNow!=ENoDrawNow)
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

void CImgDisplayAppView::ResizeBufferL(const TSize& aNewSize, TDisplayMode aDisplayMode)
	{
	delete iBmGc; iBmGc = NULL;
	delete iBmDevice; iBmDevice = NULL;
	User::LeaveIfError(iBmBuffer->Create(aNewSize, aDisplayMode));
	iBmDevice = CFbsBitmapDevice::NewL(iBmBuffer);
	iBmGc = CFbsBitGc::NewL();
	iBmGc->Activate(iBmDevice);
	}

void CImgDisplayAppView::EnsureSizeInTwipsSet(CFbsBitmap* aBitmap) const
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

void CImgDisplayAppView::Clear(TBool aClearFull, TDrawNow aDrawNow)
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


void CImgDisplayAppView::MoveBy(const TPoint& aRelMove, TDrawNow aDrawNow)
	{
	iBmRect.Move(aRelMove);

	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

void CImgDisplayAppView::Center(TDrawNow aDrawNow)
	{
	ASSERT(iBitmapValid && iBmRect.Size() == iBmBuffer->SizeInPixels()); // should only be called when size setup properly

	const TPoint center = Rect().Center();
	const TSize bitmapSize = iBmRect.Size();
	const TPoint requiredTl (center.iX-bitmapSize.iWidth/2, center.iY-bitmapSize.iHeight/2);
	const TRect newRect(requiredTl, bitmapSize);
	iBmRect = newRect;

	ASSERT(iBitmapValid && iBmRect.Size() == iBmBuffer->SizeInPixels()); // checked worked

	if (aDrawNow!=ENoDrawNow)
		DrawNow();
	}

TBool CImgDisplayAppView::SetBackgroundColor(const TRgb& aColor, TDrawNow aDrawNow)
	{
	TBool changed = iBackgroundColor!=aColor;

	iBackgroundColor = aColor;

	if (aDrawNow!=ENoDrawNow)
		{
		DrawNow();
		}

	return changed;
	}

void CImgDisplayAppView::SetDisplayModeL(TDisplayMode aDisplayMode, TDrawNow aDrawNow)
	{
	iDisplayMode = aDisplayMode; 

	Window().SetRequiredDisplayMode(iDisplayMode);

	if (aDrawNow!=ENoDrawNow)
		{
		DrawNow();
		}
	}

