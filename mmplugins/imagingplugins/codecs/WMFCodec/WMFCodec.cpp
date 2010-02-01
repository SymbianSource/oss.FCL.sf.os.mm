// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <fbs.h>
#include <hal.h>
#include "WMFCodec.h"

/*
Not supported:
0) Misc
	// 0x0105 - SETRELABS
	// 0x0231 - SETMAPPERFLAGS
	// 0x0419 - FLOODFILL
	// 0x0548 - EXTFLOODFILL
	// 0x0626 - ESCAPE
	// 0x0830 - CHORD
1) Blitting
	// 0x0107 - SETSTRETCHBLTMODE
	// 0x0922 - BITBLT
	// 0x0940 - DIBBITBLT
	// 0x0b23 - STRETCHBLT
	// 0x0d33 - SETDIBTODEV
2) Clipping
	// 0x0220 - OFFSETCLIPRGN
	// 0x0415 - EXCLUDECLIPRECT
3) Palettes
	// 0x0035 - REALIZEPALETTE
	// 0x0037 - SETPALENTRIES
	// 0x00f7 - CREATEPALETTE
	// 0x0139 - RESIZEPALETTE
	// 0x0234 - SELECTPALETTE
	// 0x0436 - ANIMATEPALETTE
*/

const TInt KFontNormalWeightLimit = 550;	// Halfway between FW_NORMAL and FW_BOLD
const TInt KFontFixedPitch = 1;				// FIXED_PITCH
const TInt KFontFamilyRoman = 1;			// FF_ROMAN
const TInt KFontFamilySwiss = 2;			// FF_SWISS
const TInt KFontFamilyMono = 3;				// FF_MODERN
const TInt KFontFamilyScript = 4;			// FF_SCRIPT
const TInt KFontFamilyDecorative = 5;		// FF_DECORATIVE

const TInt KExtTextOutFlagClipped = 4;		// ETO_CLIPPED

//const TInt KTextAlignFlagNoUpdateCP = 0;	// EABI warning removal
//const TInt KTextAlignFlagUpdateCP = 1; // Not supported		// EABI warning removal
//const TInt KTextAlignFlagLeft = 0;		// EABI warning removal
const TInt KTextAlignFlagRight = 2;
const TInt KTextAlignFlagCenter = 6;
//const TInt KTextAlignFlagTop = 0;			// EABI warning removal
const TInt KTextAlignFlagBottom = 8;
const TInt KTextAlignFlagBaseline = 24;

const TInt KTextAlignMaskHorz = 6;
const TInt KTextAlignMaskVert = 24;

const TUint KMaxProcessTime = 175000; //limit DoProcessL() to 175ms for liveliness

// Wrapper class for drawing to the bitmap.
// All drawing operations go through this class.
CFbsBitGcWrapper* CFbsBitGcWrapper::NewL(CFbsDevice& aDevice, CFbsBitGcWrapper* aGc)
	{
	CFbsBitGcWrapper* self = new(ELeave) CFbsBitGcWrapper;

	CleanupStack::PushL(self);
	self->ConstructL(aDevice, aGc);
	CleanupStack::Pop(self);
	return self;
	}

CFbsBitGcWrapper::CFbsBitGcWrapper():
	iBrushColor(KRgbWhite),
	iBrushStyle(CGraphicsContext::ENullBrush),
	iPenColor(KRgbBlack),
	iPenStyle(CGraphicsContext::ESolidPen),
	iPenSize(1,1),
	iTextColor(KRgbBlack),
	iBackgroundColor(KRgbWhite)
	{}

void CFbsBitGcWrapper::ConstructL(CFbsDevice& aDevice, CFbsBitGcWrapper* aGc)
	{
	User::LeaveIfError(aDevice.CreateContext(iGc));
	User::LeaveIfError(aDevice.CreateContext(iPolygonGc));
	if (aGc)
		{
		iGc->CopySettings(*(aGc->iGc));
		iGc->CancelClippingRegion();

		iPolygonGc->CopySettings(*(aGc->iGc));
		iPolygonGc->CancelClippingRegion();

		iBrushColor = aGc->iBrushColor;
		iBrushStyle = aGc->iBrushStyle;
		iPenColor = aGc->iPenColor;
		iPenStyle = aGc->iPenStyle;
		iPenSize = aGc->iPenSize;

		iTextColor = aGc->iTextColor;
		iBackgroundColor = aGc->iBackgroundColor;
		iPatternBrush = aGc->iPatternBrush;
		}
	}

CFbsBitGcWrapper::~CFbsBitGcWrapper()
	{
	delete iGc;
	delete iPolygonGc;
	}

void CFbsBitGcWrapper::SetPen(const CPen& aPen)
	{
	iPenStyle = aPen.iStyle;
	iPenSize = aPen.iSize;
	iPenColor = aPen.iColor;

	iGc->SetPenStyle(iPenStyle);
	iGc->SetPenSize(iPenSize);
	iGc->SetPenColor(iPenColor);
	}

void CFbsBitGcWrapper::SetBrush(CBrush& aBrush)
	{
	iBrushColor = aBrush.iColor;
	iBrushStyle = aBrush.iStyle;
	iPatternBrush = (iBrushStyle == CGraphicsContext::EPatternedBrush) ? static_cast<CPatternBrush*> (&aBrush) : NULL;

	iGc->SetBrushStyle(iBrushStyle);
	iGc->SetBrushColor(iBrushColor);
	}

void CFbsBitGcWrapper::RestorePenAndBrush()
	{
	iGc->SetPenStyle(iPenStyle);
	iGc->SetPenSize(iPenSize);
	iGc->SetPenColor(iPenColor);

	iGc->SetBrushStyle(iBrushStyle);
	iGc->SetBrushColor(iBrushColor);
	}

void CFbsBitGcWrapper::DrawPolygonL(const TPoint* aPointList,TInt aNumPoints,CGraphicsContext::TFillRule aFillRule)
	{
	RealizeBrush();
	if (iPenStyle == CGraphicsContext::ENullPen || iPenSize.iWidth == 0)
		{
		if (iBrushStyle != CGraphicsContext::ENullBrush)
			{
			iPolygonGc->CopySettings(*iGc);
			iPolygonGc->CancelClippingRegion();

			iPolygonGc->SetPenStyle(CGraphicsContext::ESolidPen);
			iPolygonGc->SetPenSize(TSize(1,1));
			iPolygonGc->SetPenColor(iBrushColor);

			User::LeaveIfError(iPolygonGc->DrawPolygon(aPointList,aNumPoints,aFillRule));
			}
		}
	else
		User::LeaveIfError(iGc->DrawPolygon(aPointList,aNumPoints,aFillRule));
	}

const CFbsFont* CFbsBitGcWrapper::CurrentFont()
	{
	return iCurrentFont;
	}

void CFbsBitGcWrapper::Clear()
	{
	iGc->Clear();
	}

void CFbsBitGcWrapper::SetUnderlineStyle(TFontUnderline aUnderlineStyle)
	{
	iGc->SetUnderlineStyle(aUnderlineStyle);
	}

void CFbsBitGcWrapper::SetStrikethroughStyle(TFontStrikethrough aStrikethroughStyle)
	{
	iGc->SetStrikethroughStyle(aStrikethroughStyle);
	}

void CFbsBitGcWrapper::UseFont(const CFbsFont* aFont)
	{
	iGc->UseFont(aFont);
	iCurrentFont = aFont;
	}

void CFbsBitGcWrapper::SetClippingRegion(const TRegion* aRegion)
	{
	iGc->SetClippingRegion(aRegion);
	}

void CFbsBitGcWrapper::SetCharJustification(TInt aExcessWidth, TInt aNumGaps)
	{
	iGc->SetCharJustification(aExcessWidth, aNumGaps);
	}

void CFbsBitGcWrapper::SetWordJustification(TInt aExcessWidth, TInt aNumChars)
	{
	iGc->SetWordJustification(aExcessWidth, aNumChars);
	}

void CFbsBitGcWrapper::DrawLineTo(const TPoint& aPoint)
	{
	iGc->DrawLineTo(aPoint);
	}

void CFbsBitGcWrapper::MoveTo(const TPoint& aPoint)
	{
	iGc->MoveTo(aPoint);
	}

void CFbsBitGcWrapper::DrawPolyLine(const TPoint* aPointList, TInt aNumPoints)
	{
	iGc->DrawPolyLine(aPointList, aNumPoints);
	}

void CFbsBitGcWrapper::DrawEllipse(const TRect& aRect)
	{
	RealizeBrush();
	iGc->DrawEllipse(aRect);
	}

void CFbsBitGcWrapper::DrawRect(const TRect& aRect)
	{
	RealizeBrush();
	iGc->DrawRect(aRect);
	}

void CFbsBitGcWrapper::DrawRoundRect(const TRect& aRect, const TSize& aEllipse)
	{
	RealizeBrush();
	iGc->DrawRoundRect(aRect, aEllipse);
	}

void CFbsBitGcWrapper::SetPenStyle(CGraphicsContext::TPenStyle aPenStyle)
	{
	iGc->SetPenStyle(aPenStyle);
	}

void CFbsBitGcWrapper::SetBrushStyle(CGraphicsContext::TBrushStyle aBrushStyle)
	{
	iGc->SetBrushStyle(aBrushStyle);
	}

void CFbsBitGcWrapper::SetBrushColor(const TRgb& aColor)
	{
	iGc->SetBrushColor(aColor);
	}

void CFbsBitGcWrapper::SetDrawMode(CGraphicsContext::TDrawMode aDrawMode)
	{
	iGc->SetDrawMode(aDrawMode);
	}

void CFbsBitGcWrapper::SetPenColor(const TRgb& aColor)
	{
	iGc->SetPenColor(aColor);
	}

void CFbsBitGcWrapper::SetTextPenColor()
	{
	iGc->SetPenColor(iTextColor);
	}

void CFbsBitGcWrapper::Plot(const TPoint& aPoint)
	{
	iGc->Plot(aPoint);
	}

void CFbsBitGcWrapper::SetPenSize(const TSize& aSize)
	{
	iGc->SetPenSize(aSize);
	}


void CFbsBitGcWrapper::DrawArc(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd)
	{
	iGc->DrawArc(aRect, aStart, aEnd);
	}

void CFbsBitGcWrapper::DrawPie(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd)
	{
	RealizeBrush();
	iGc->DrawPie(aRect, aStart, aEnd);
	}

void CFbsBitGcWrapper::DrawBitmap(const TRect& aDestRect, const CFbsBitmap* aSource, const TRect& aSourceRect)
	{
	iGc->DrawBitmap(aDestRect, aSource, aSourceRect);
	}

void CFbsBitGcWrapper::DrawText(const TDesC& aText,const TPoint& aPosition)
	{
	iGc->DrawText(aText, aPosition);
	}

void CFbsBitGcWrapper::UseBrushPattern(const CFbsBitmap* aPatternBitmap)
	{
	iGc->UseBrushPattern(aPatternBitmap);
	}

void CFbsBitGcWrapper::SetBackgroundColor(TRgb aBackgroundColor)
	{
	iBackgroundColor = aBackgroundColor;
	}

void CFbsBitGcWrapper::SetTextColor(TRgb aTextColor)
	{
	iTextColor = aTextColor;
	}

void CFbsBitGcWrapper::RealizeBrush()
	{
	if (iPatternBrush)
		iPatternBrush->RealizeBrush(iTextColor, iBackgroundColor);
	}

// Wrapper class for drawing to the bitmap and mask.
// All drawing operations are performed through this class when mask generation is requested.
CFbsBitGcMaskWrapper* CFbsBitGcMaskWrapper::NewL(CFbsDevice& aDevice, CFbsDevice& aMaskDevice, CFbsBitGcMaskWrapper* aGc)
	{
	CFbsBitGcMaskWrapper* self = new(ELeave) CFbsBitGcMaskWrapper;

	CleanupStack::PushL(self);
	self->ConstructL(aDevice, aMaskDevice, aGc);
	CleanupStack::Pop(self);
	return self;
	}

CFbsBitGcMaskWrapper::CFbsBitGcMaskWrapper()
	{
	}

void CFbsBitGcMaskWrapper::ConstructL(CFbsDevice& aDevice, CFbsDevice& aMaskDevice, CFbsBitGcMaskWrapper* aGc)
	{
	CFbsBitGcWrapper::ConstructL(aDevice, aGc);
	User::LeaveIfError(aMaskDevice.CreateContext(iMaskGc));
	User::LeaveIfError(aMaskDevice.CreateContext(iPolygonMaskGc));
	if (aGc)
		{
		iMaskGc->CopySettings(*(aGc->iMaskGc));
		iMaskGc->CancelClippingRegion();

		iPolygonMaskGc->CopySettings(*(aGc->iMaskGc));
		iPolygonMaskGc->CancelClippingRegion();
		}

	// Set the pen color to white.
	// (Ensures that all drawing operations will set pixels on the mask)
	iMaskGc->SetPenColor(KRgbWhite);
	iPolygonMaskGc->SetPenColor(KRgbWhite);
	}

CFbsBitGcMaskWrapper::~CFbsBitGcMaskWrapper()
	{
	delete iMaskGc;
	delete iPolygonMaskGc;
	}

void CFbsBitGcMaskWrapper::SetPen(const CPen& aPen)
	{
	CFbsBitGcWrapper::SetPen(aPen);

	iMaskGc->SetPenStyle(iPenStyle);
	iMaskGc->SetPenSize(iPenSize);
	}

void CFbsBitGcMaskWrapper::SetBrush(CBrush& aBrush)
	{
	CFbsBitGcWrapper::SetBrush(aBrush);

	if (!iPatternBrush)
		iMaskGc->SetBrushStyle(iBrushStyle);
	else
		iMaskGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	}

void CFbsBitGcMaskWrapper::DrawPolygonL(const TPoint* aPointList,TInt aNumPoints,CGraphicsContext::TFillRule aFillRule)
	{
	CFbsBitGcWrapper::DrawPolygonL(aPointList, aNumPoints, aFillRule);
	if (iPenStyle == CGraphicsContext::ENullPen || iPenSize.iWidth == 0)
		{
		if (iBrushStyle != CGraphicsContext::ENullBrush)
			{
			iPolygonMaskGc->CopySettings(*iMaskGc);
			iPolygonMaskGc->CancelClippingRegion();

			iPolygonMaskGc->SetPenStyle(CGraphicsContext::ESolidPen);
			iPolygonMaskGc->SetPenSize(TSize(1,1));

			User::LeaveIfError(iPolygonMaskGc->DrawPolygon(aPointList,aNumPoints,aFillRule));
			}
		}
	else
		User::LeaveIfError(iMaskGc->DrawPolygon(aPointList,aNumPoints,aFillRule));
	}

void CFbsBitGcMaskWrapper::Clear()
	{
	CFbsBitGcWrapper::Clear();

	// Clear the mask to black.
	iMaskGc->SetBrushColor(KRgbBlack);
	iMaskGc->Clear();

	// Reset the brush color to white.
	iMaskGc->SetBrushColor(KRgbWhite);
	}

void CFbsBitGcMaskWrapper::SetUnderlineStyle(TFontUnderline aUnderlineStyle)
	{
	CFbsBitGcWrapper::SetUnderlineStyle(aUnderlineStyle);
	iMaskGc->SetUnderlineStyle(aUnderlineStyle);
	}

void CFbsBitGcMaskWrapper::SetStrikethroughStyle(TFontStrikethrough aStrikethroughStyle)
	{
	CFbsBitGcWrapper::SetStrikethroughStyle(aStrikethroughStyle);
	iMaskGc->SetStrikethroughStyle(aStrikethroughStyle);
	}

void CFbsBitGcMaskWrapper::UseFont(const CFbsFont* aFont)
	{
	CFbsBitGcWrapper::UseFont(aFont);
	iMaskGc->UseFont(aFont);
	}

void CFbsBitGcMaskWrapper::SetClippingRegion(const TRegion* aRegion)
	{
	CFbsBitGcWrapper::SetClippingRegion(aRegion);
	iMaskGc->SetClippingRegion(aRegion);
	}

void CFbsBitGcMaskWrapper::SetCharJustification(TInt aExcessWidth, TInt aNumGaps)
	{
	CFbsBitGcWrapper::SetCharJustification(aExcessWidth, aNumGaps);
	iMaskGc->SetCharJustification(aExcessWidth, aNumGaps);
	}

void CFbsBitGcMaskWrapper::SetWordJustification(TInt aExcessWidth, TInt aNumChars)
	{
	CFbsBitGcWrapper::SetWordJustification(aExcessWidth, aNumChars);
	iMaskGc->SetWordJustification(aExcessWidth, aNumChars);
	}

void CFbsBitGcMaskWrapper::DrawLineTo(const TPoint& aPoint)
	{
	CFbsBitGcWrapper::DrawLineTo(aPoint);
	iMaskGc->DrawLineTo(aPoint);
	}

void CFbsBitGcMaskWrapper::MoveTo(const TPoint& aPoint)
	{
	CFbsBitGcWrapper::MoveTo(aPoint);
	iMaskGc->MoveTo(aPoint);
	}

void CFbsBitGcMaskWrapper::DrawPolyLine(const TPoint* aPointList, TInt aNumPoints)
	{
	CFbsBitGcWrapper::DrawPolyLine(aPointList, aNumPoints);
	iMaskGc->DrawPolyLine(aPointList, aNumPoints);
	}

void CFbsBitGcMaskWrapper::DrawEllipse(const TRect& aRect)
	{
	CFbsBitGcWrapper::DrawEllipse(aRect);
	iMaskGc->DrawEllipse(aRect);
	}

void CFbsBitGcMaskWrapper::DrawRect(const TRect& aRect)
	{
	CFbsBitGcWrapper::DrawRect(aRect);
	iMaskGc->DrawRect(aRect);
	}

void CFbsBitGcMaskWrapper::DrawRoundRect(const TRect& aRect, const TSize& aEllipse)
	{
	CFbsBitGcWrapper::DrawRoundRect(aRect, aEllipse);
	iMaskGc->DrawRoundRect(aRect, aEllipse);
	}

void CFbsBitGcMaskWrapper::SetPenStyle(CGraphicsContext::TPenStyle aPenStyle)
	{
	CFbsBitGcWrapper::SetPenStyle(aPenStyle);
	iMaskGc->SetPenStyle(aPenStyle);
	}

void CFbsBitGcMaskWrapper::SetBrushStyle(CGraphicsContext::TBrushStyle aBrushStyle)
	{
	CFbsBitGcWrapper::SetBrushStyle(aBrushStyle);
	iMaskGc->SetBrushStyle(aBrushStyle);
	}

void CFbsBitGcMaskWrapper::SetDrawMode(CGraphicsContext::TDrawMode aDrawMode)
	{
	CFbsBitGcWrapper::SetDrawMode(aDrawMode);
	}

void CFbsBitGcMaskWrapper::Plot(const TPoint& aPoint)
	{
	CFbsBitGcWrapper::Plot(aPoint);
	iMaskGc->Plot(aPoint);
	}

void CFbsBitGcMaskWrapper::SetPenSize(const TSize& aSize)
	{
	CFbsBitGcWrapper::SetPenSize(aSize);
	iMaskGc->SetPenSize(aSize);
	}

void CFbsBitGcMaskWrapper::DrawArc(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd)
	{
	CFbsBitGcWrapper::DrawArc(aRect, aStart, aEnd);
	iMaskGc->DrawArc(aRect, aStart, aEnd);
	}

void CFbsBitGcMaskWrapper::DrawPie(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd)
	{
	CFbsBitGcWrapper::DrawPie(aRect, aStart, aEnd);
	iMaskGc->DrawPie(aRect, aStart, aEnd);
	}

void CFbsBitGcMaskWrapper::DrawBitmap(const TRect& aDestRect, const CFbsBitmap* aSource, const TRect& aSourceRect)
	{
	CFbsBitGcWrapper::DrawBitmap(aDestRect, aSource, aSourceRect);
	iMaskGc->Clear(aDestRect);
	}

void CFbsBitGcMaskWrapper::DrawText(const TDesC& aText,const TPoint& aPosition)
	{
	CFbsBitGcWrapper::DrawText(aText, aPosition);
	iMaskGc->DrawText(aText, aPosition);
	}


void CPen::Apply(CFbsBitGcWrapper* aGc)
	{
	aGc->SetPen(*this);
	}

TInt CPen::Type() const
	{
	return KWmfGraphicsObjectPen;
	}

void CDummyBrush::Apply(CFbsBitGcWrapper* /* aGc */)
	{
	}

TInt CDummyBrush::Type() const
	{
	return KWmfGraphicsObjectDummyBrush;
	}

void CBrush::Apply(CFbsBitGcWrapper* aGc)
	{
	aGc->SetBrush(*this);
	}

TInt CBrush::Type() const
	{
	return KWmfGraphicsObjectBrush;
	}

CPatternBrush* CPatternBrush::NewL()
	{
	CPatternBrush* self = new(ELeave) CPatternBrush;

	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPatternBrush::ConstructL()
	{
	iBitmap = new(ELeave) CFbsBitmap;
	}

CPatternBrush::CPatternBrush()
: iTextColor(KRgbWhite), iBackgroundColor(KRgbWhite)
	{
	iStyle = CGraphicsContext::EPatternedBrush;
	}

CPatternBrush::~CPatternBrush()
	{
	delete iBitmap;
	delete[] iBitmapBits;
	}

void CPatternBrush::Apply(CFbsBitGcWrapper* aGc)
	{
	aGc->UseBrushPattern(iBitmap);
	CBrush::Apply(aGc);
	}

TInt CPatternBrush::Type() const
	{
	return KWmfGraphicsObjectPatternBrush;
	}

void CPatternBrush::SetBitmapBits(TUint8* aBitmapBits)
	{
	iBitmapBits = aBitmapBits;
	}

void CPatternBrush::RealizeBrush(TRgb aTextColor, TRgb aBackgroundColor)
	{
	if (!iBitmapBits)
		return;

	if ((aTextColor == iTextColor) && (aBackgroundColor == iBackgroundColor))
		return;

	iTextColor = aTextColor;
	iBackgroundColor = aBackgroundColor;

	TInt width = iBitmap->SizeInPixels().iWidth;
	TInt height = iBitmap->SizeInPixels().iHeight;
	TInt padding = ((width + 31) & ~31) - width;
	TUint pixelMask = 0x80;
	TUint8* bytePtr = iBitmapBits;
	TBitmapUtil util(iBitmap);
	for (TInt y = height - 1; y >= 0; y--)
		{
		util.Begin(TPoint(0,y));

		TInt x;
		for (x = 0; x < width; x++)
			{
			TRgb color((bytePtr[0] & pixelMask) ? iBackgroundColor : iTextColor);
			util.SetPixel(color.Color64K());
			util.IncXPos();

			pixelMask >>= 1;
			if (pixelMask == 0)
				{
				pixelMask = 0x80;
				bytePtr++;
				}
			}

		for (x = 0 ; x<padding ; x++)
			{
			pixelMask >>= 1;
			if (pixelMask == 0)
				{
				pixelMask = 0x80;
				bytePtr++;
				}
			}

		util.End();
		}
	}

CFbsBitmap* CPatternBrush::Bitmap()
	{
	return iBitmap;
	}


CFontObject::CFontObject(CFbsDevice& aDevice,CFbsFont& aFont,TBool aUnderline,TBool aStrikeOut):
	iDevice(aDevice),
	iFont(aFont),
	iUnderline(aUnderline ? EUnderlineOn : EUnderlineOff),
	iStrikeOut(aStrikeOut ? EStrikethroughOn : EStrikethroughOff)
	{}

CFontObject::~CFontObject()
	{
	iDevice.ReleaseFont(&iFont);
	}

void CFontObject::Apply(CFbsBitGcWrapper* aGc)
	{
	aGc->SetUnderlineStyle(iUnderline);
	aGc->SetStrikethroughStyle(iStrikeOut);
	aGc->UseFont(&iFont);
	}

TInt CFontObject::Type() const
	{
	return KWmfGraphicsObjectFont;
	}


CRectRegion::CRectRegion(CFbsDevice& aDevice, TRegion& aClipRegion, TInt aLeft, TInt aTop, TInt aRight, TInt aBottom):
	iRect(aLeft, aTop, aRight, aBottom), iDevice(aDevice), iClipRegion(aClipRegion)
	{}

void CRectRegion::Apply(CFbsBitGcWrapper* aGc)
	{
	iClipRegion.Clear();
	TRect bitmapRect(iDevice.SizeInPixels());
	iClipRegion.AddRect(bitmapRect);

	TRegionFix<1> rectRegion;
	rectRegion.AddRect(iRect);

	iClipRegion.Intersect(rectRegion);
	aGc->SetClippingRegion(&iClipRegion);
	}

TInt CRectRegion::Type() const
	{
	return KWmfGraphicsObjectRegion;
	}


void CPaletteObject::Apply(CFbsBitGcWrapper* /* aGc */)
	{
	}

TInt CPaletteObject::Type() const
	{
	return KWmfGraphicsObjectPalette;
	}


//
// CWmfReadCodec
//

CWmfReadCodec::CWmfReadCodec(TInt aWordsExpected)
	: iWordsExpected(aWordsExpected)
	{
	}

CWmfReadCodec::~CWmfReadCodec()
	{
	delete[] iPointArray;
	iSavedGcStack.ResetAndDestroy();

	for (TInt index = 0; index < KWmfMaxGraphicsObjectCount; index++)
		delete iGraphicsObject[index];

	delete iDefaultPen;
	delete iDefaultBrush;
	delete iGc;
	delete iDevice;
	delete iMaskDevice;
	delete[] iAccumulator;
	}

CWmfReadCodec* CWmfReadCodec::NewL(TInt aWordsExpected)
	{
	CWmfReadCodec* self = new(ELeave) CWmfReadCodec(aWordsExpected);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}

void CWmfReadCodec::ConstructL()
	{
	CImageReadCodec::ConstructL();

	iDefaultPen = new(ELeave) CPen;
	iDefaultBrush = new(ELeave) CBrush;
	}

void CWmfReadCodec::InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameImageData*/, TBool /*aDisableErrorDiffusion*/, CFbsBitmap& aBitmap, CFbsBitmap* aDestinationMask)
	{
	iWordsProcessed = 0;

	for (TInt index = 0; index < KWmfMaxGraphicsObjectCount; index++)
		{
		delete iGraphicsObject[index];
		iGraphicsObject[index] = NULL;
		}

	delete iGc; iGc = NULL;
	delete iDevice;	iDevice = NULL;
	delete iMaskDevice; iMaskDevice = NULL;
	iDevice = CFbsBitmapDevice::NewL(&aBitmap);

	if ((aFrameInfo.iFlags & TFrameInfo::ETransparencyPossible) && aDestinationMask)
		{
		iMaskDevice = CFbsBitmapDevice::NewL(aDestinationMask);
		iGc = CFbsBitGcMaskWrapper::NewL(*iDevice, *iMaskDevice);
		}
	else
		{
		iGc = CFbsBitGcWrapper::NewL(*iDevice);
		}
	
	if ((aFrameInfo.iFlags & TFrameInfo::ETransparencyPossible) && !aDestinationMask && aBitmap.DisplayMode() == EColor16MA)
		{
		// Special case of single 16MA destination bitmap.
		const TRgb transparentBlack(0x000000, 0x00);
		ClearBitmapL(aBitmap, transparentBlack);
		}
	else
		{
		iGc->Clear(); // Ensure bitmap is cleared. (Also clear the mask if we are generating one)	
		}
	
	iBkModeOpaque = EFalse;
	iMapMode = EMapModeTEXT;
	iWindowOrg.SetXY(0,0);
	iWindowExt = aBitmap.SizeInPixels();
	iViewportOrg.SetXY(0,0);
	iViewportExt = iWindowExt;

	iClipRegion.Clear();
	TRect bitmapRect(iDevice->SizeInPixels());
	iClipRegion.AddRect(bitmapRect);
		
	iFillRule = CGraphicsContext::EAlternate;

	delete[] iAccumulator; iAccumulator = NULL;
	iAccumulatorPtr = NULL;
	iAccumulatorSizeInWords = 0;
	iLookingForRecord = ETrue;
	iRecordSizeInWords = 0;
	iFunction = 0;
	iWordsToRead = 0;
	iFinished = EFalse;
	}

TFrameState CWmfReadCodec::ProcessFrameL(TBufPtr8& aSrc)
	{
	TUint16* startDataPtr = REINTERPRET_CAST(TUint16*,&aSrc[0]);
	TUint16* dataPtr = startDataPtr;
	TUint16* dataPtrLimit = dataPtr + Min(TInt(aSrc.Length() / sizeof(TUint16)),iWordsExpected - iWordsProcessed);

	DoProcessL(dataPtr,dataPtrLimit);

	TInt wordsUsed = dataPtr - startDataPtr;
	aSrc.Shift(wordsUsed * sizeof(TUint16));
	iWordsProcessed += wordsUsed;

	if (iWordsProcessed >= iWordsExpected || iFinished)
		return EFrameComplete;

	return EFrameIncomplete;
	}

void CWmfReadCodec::DoProcessL(TUint16*& aDataPtr,const TUint16* aDataPtrLimit)
	{
	const TUint16* safeDataPtrLimit = aDataPtrLimit - KWmfMinRecordSizeInWords;
	
	//Do not spend more than KMaxProcessTime here
	TInt systemTickPeriod;
	User::LeaveIfError(HAL::Get(HAL::ESystemTickPeriod,systemTickPeriod));

	TUint currentTick = User::TickCount();
	const TUint tickLimit = currentTick + (KMaxProcessTime / systemTickPeriod);

	//Disable tick check if tickLimit overflow
	TBool disableTickCheck = EFalse;
	if(tickLimit<=currentTick)
		disableTickCheck = ETrue;

	while (aDataPtr <= aDataPtrLimit && (disableTickCheck || currentTick < tickLimit))
		{
		if (iLookingForRecord) // Find record
			{
			if (aDataPtr > safeDataPtrLimit) // Ensures that a record header can always be safely read
				return;

			iRecordSizeInWords = (aDataPtr[1] << 16) | aDataPtr[0];
			if (iRecordSizeInWords < KWmfMinRecordSizeInWords || iRecordSizeInWords > iWordsExpected)
				User::Leave(KErrCorrupt);

			iFunction = aDataPtr[2];

			iWordsToRead = iRecordSizeInWords - KWmfMinRecordSizeInWords;
			if (iWordsToRead > aDataPtrLimit - aDataPtr - KWmfMinRecordSizeInWords) // Check to see if we need to accumulate data
				{
				if (iWordsToRead > iAccumulatorSizeInWords)
					{
					delete[] iAccumulator;
					iAccumulator = NULL;
					iAccumulatorSizeInWords = 0;

					iAccumulator = new(ELeave) TUint16[iWordsToRead];
					iAccumulatorSizeInWords = iWordsToRead;
					}

				iAccumulatorPtr = iAccumulator;
				}
			else
				iAccumulatorPtr = NULL;

			aDataPtr += KWmfMinRecordSizeInWords;
			iLookingForRecord = EFalse;
			}
		else if (iFunction == KWmfTerminateFunction) // Finished
			{
			iFinished = ETrue;
			return;
			}
		else if (iAccumulatorPtr && iWordsToRead > 0) // Accumulate data
			{
			const TInt wordsToAccumulate = Min(iWordsToRead,aDataPtrLimit - aDataPtr);
			Mem::Copy(iAccumulatorPtr,aDataPtr,wordsToAccumulate * sizeof(TUint16));
			aDataPtr += wordsToAccumulate;
			iAccumulatorPtr += wordsToAccumulate;
			iWordsToRead -= wordsToAccumulate;

			if (iWordsToRead > 0)
				return;
			}
		else // Process data
			{
			TUint16* dataPtr = iAccumulatorPtr ? iAccumulator : aDataPtr;

			if (iFunction < 0x0230)
				{
				if (iFunction < 0x0130)
					DoProcess00L(iFunction,dataPtr);
				else
					DoProcess01L(iFunction,dataPtr);
				}
			else
				{
				if (iFunction < 0x500)
					DoProcess10L(iFunction,dataPtr);
				else
					DoProcess11L(iFunction,dataPtr);
				}

			if (iAccumulatorPtr)
				{
				if (iAccumulatorSizeInWords > 0x800)
					{
					delete[] iAccumulator;
					iAccumulator = NULL;
					iAccumulatorSizeInWords = 0;
					}

				iAccumulatorPtr = NULL;
				}
			else
				aDataPtr += iWordsToRead;

			iLookingForRecord = ETrue;
			}
		currentTick = User::TickCount();
		}
	}

void CWmfReadCodec::DoProcess00L(TInt aFunction,TUint16* aArgPtr)
	{
	if (aFunction < 0x0107)
		{
		if (aFunction < 0x0103)
			DoProcess0000L(aFunction,aArgPtr);
		else
			DoProcess0001L(aFunction,aArgPtr);
		}
	else
		{
		if (aFunction < 0x012b)
			DoProcess0010L(aFunction,aArgPtr);
		else
			DoProcess0011L(aFunction,aArgPtr);
		}
	}

void CWmfReadCodec::DoProcess01L(TInt aFunction,TUint16* aArgPtr)
	{
	if (aFunction < 0x020d)
		{
		if (aFunction < 0x0205)
			DoProcess0100L(aFunction,aArgPtr);
		else
			DoProcess0101L(aFunction,aArgPtr);
		}
	else
		{
		if (aFunction < 0x0212)
			DoProcess0110L(aFunction,aArgPtr);
		else
			DoProcess0111L(aFunction,aArgPtr);
		}
	}

void CWmfReadCodec::DoProcess10L(TInt aFunction,TUint16* aArgPtr)
	{
	if (aFunction < 0x0413)
		{
		if (aFunction < 0x0300)
			DoProcess1000L(aFunction,aArgPtr);
		else
			DoProcess1001L(aFunction,aArgPtr);
		}
	else
		{
		if (aFunction < 0x041a)
			DoProcess1010L(aFunction,aArgPtr);
		else
			DoProcess1011L(aFunction,aArgPtr);
		}
	}

void CWmfReadCodec::DoProcess11L(TInt aFunction,TUint16* aArgPtr)
	{
	if (aFunction < 0x0820)
		{
		if (aFunction < 0x0620)
			DoProcess1100L(aFunction,aArgPtr);
		else
			DoProcess1101L(aFunction,aArgPtr);
		}
	else
		{
		if (aFunction < 0x0b00)
			DoProcess1110L(aFunction,aArgPtr);
		else
			DoProcess1111L(aFunction,aArgPtr);
		}
	}

void CWmfReadCodec::DoProcess0000L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0035 - REALIZEPALETTE
	// 0x0037 - SETPALENTRIES
	// 0x00f7 - CREATEPALETTE

	switch (aFunction)
		{
	case 0x001e: // SAVEDC
		{
		CFbsBitGcWrapper* gcCopy = CloneGcL();
		CleanupStack::PushL(gcCopy);

		User::LeaveIfError(iSavedGcStack.Append(gcCopy));
		CleanupStack::Pop(); // gcCopy
		}
		break;
	case 0x00f7: // CREATEPALETTE
		AddGraphicsObject(new(ELeave) CPaletteObject);
		// Create the object to maintain the handle list
		break;
	case 0x0102: // SETBKMODE
		// 1 = TRANSPARENT, 2 = OPAQUE
		iBkModeOpaque = (aArgPtr[0] == 2);
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0001L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0105 - SETRELABS

	switch (aFunction)
		{
	case 0x0103: // SETMAPMODE
		iMapMode = TMapMode(aArgPtr[0]);
		break;
	case 0x0104: // SETROP2
		SetROP(iGc,aArgPtr[0]);
		break;
	case 0x0106: // SETPOLYFILLMODE
		// 1 = ALTERNATE, 2 = WINDING
		iFillRule = (aArgPtr[0] == 2) ? CGraphicsContext::EWinding : CGraphicsContext::EAlternate;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0010L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0107 - SETSTRETCHBLTMODE

	switch (aFunction)
		{
	case 0x0108: // SETTEXTCHAREXTRA
		{
		const TInt extraSpace = LogicalSizeToDeviceSize(aArgPtr[0],ETrue);
		if (extraSpace != 0)
			{
			const TInt numChars = KMaxTInt / extraSpace; // Choose a suitably high number of chars to keep the char spacing going
			iGc->SetCharJustification(extraSpace * numChars,numChars);
			}
		}
		break;
	case 0x0127: // RESTOREDC
		{
		const TInt savedGcCount = iSavedGcStack.Count();
		if (savedGcCount > 0)
			{
			delete iGc;
			iGc = iSavedGcStack[savedGcCount - 1];
			iSavedGcStack.Remove(savedGcCount - 1);
			ASSERT(iGc);
			}
		}
		break;
	case 0x012a: // INVERTREGION
		{
		CGraphicsObject* object = GraphicsObject(aArgPtr[0]);
		if (object && object->Type() == KWmfGraphicsObjectRegion)
			{
			CRectRegion* rectRgn = STATIC_CAST(CRectRegion*,object);
			CFbsBitGcWrapper* gc = CloneGcL();

			gc->SetPenStyle(CGraphicsContext::ENullPen);
			gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
			gc->SetBrushColor(KRgbWhite);
			gc->SetDrawMode(CGraphicsContext::EDrawModeXOR);
			gc->DrawRect(rectRgn->iRect);

			delete gc;
			}
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0011L(TInt aFunction,TUint16* aArgPtr)
	{
	switch (aFunction)
		{
	case 0x012e: // SETTEXTALIGN
		iTextAlignFlags = aArgPtr[0];
		break;
	case 0x012b: // PAINTREGION
		{
		CGraphicsObject* object = GraphicsObject(aArgPtr[0]);
		if (object && object->Type() == KWmfGraphicsObjectRegion)
			{
			CRectRegion* rectRgn = STATIC_CAST(CRectRegion*,object);
			CFbsBitGcWrapper* gc = CloneGcL();

			gc->SetPenStyle(CGraphicsContext::ENullPen);
			gc->DrawRect(rectRgn->iRect);

			delete gc;
			}
		}
		break;
	case 0x012c: // SELECTCLIPREGION
	case 0x012d: // SELECTOBJECT
		{
		CGraphicsObject* object = GraphicsObject(aArgPtr[0]);
		if (object)
			object->Apply(iGc);
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0100L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0139 - RESIZEPALETTE

	switch (aFunction)
		{
	case 0x01f9: // CREATEPATTERNBRUSH
	case 0x0142: // DIBCREATEPATTERNBRUSH
		{
		CPatternBrush* patternBrush = CPatternBrush::NewL();

		TInt err = ExtractDib(aArgPtr + 2, patternBrush);
		if (err == KErrNone)
			AddGraphicsObject(patternBrush);
		else
			{
			// Coverity reports a leave_without_push error here because
			// CPatternBrush has a CFbsBitmap member which can panic
			// during its destructor. The panic has a trapped leave and
			// for some reason coverity does not recognise it.
			// Mark as false positive
			// coverity[leave_without_push : FALSE]
			delete patternBrush;

			if (err == KErrNotSupported) // Ignore KErrNotSupported
				AddGraphicsObject(new(ELeave) CDummyBrush); // Create the object to maintain the handle list
			else
				User::Leave(err);
			}
		}
		break;
	case 0x01f0: // DELETEOBJECT
		RemoveGraphicsObject(aArgPtr[0]);
		break;
	case 0x0201: // SETBKCOLOR
		iGc->SetBackgroundColor(ExtractColor(aArgPtr));
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0101L(TInt aFunction,TUint16* aArgPtr)
	{
	switch (aFunction)
		{
	case 0x0209: // SETTEXTCOLOR
		iGc->SetTextColor(ExtractColor(aArgPtr));
		break;
	case 0x020a: // SETTEXTJUSTIFICATION
		iGc->SetWordJustification(LogicalSizeToDeviceSize(aArgPtr[1],ETrue),aArgPtr[0]);
		break;
	case 0x020b: // SETWINDOWORG
		iWindowOrg.SetXY(REINTERPRET_CAST(TInt16*,aArgPtr)[1],REINTERPRET_CAST(TInt16*,aArgPtr)[0]);
		break;
	case 0x020c: // SETWINDOWEXT
		iWindowExt.SetSize(REINTERPRET_CAST(TInt16*,aArgPtr)[1],REINTERPRET_CAST(TInt16*,aArgPtr)[0]);
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0110L(TInt aFunction,TUint16* aArgPtr)
	{
	switch (aFunction)
		{
	case 0x020d: // SETVIEWPORTORG
		if (iIgnoreViewportMetaData == EFalse)
			iViewportOrg.SetXY(REINTERPRET_CAST(TInt16*,aArgPtr)[1],REINTERPRET_CAST(TInt16*,aArgPtr)[0]);
		break;
	case 0x020e: // SETVIEWPORTEXT
		if (iIgnoreViewportMetaData == EFalse)
			iViewportExt.SetSize(REINTERPRET_CAST(TInt16*,aArgPtr)[1],REINTERPRET_CAST(TInt16*,aArgPtr)[0]);
		break;
	case 0x020f: // OFFSETWINDOWORG
		iWindowOrg += TPoint(REINTERPRET_CAST(TInt16*,aArgPtr)[1],REINTERPRET_CAST(TInt16*,aArgPtr)[0]);
		break;
	case 0x0211: // OFFSETVIEWPORTORG
		if (iIgnoreViewportMetaData == EFalse)
			iViewportOrg += TPoint(REINTERPRET_CAST(TInt16*,aArgPtr)[1],REINTERPRET_CAST(TInt16*,aArgPtr)[0]);
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess0111L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0220 - OFFSETCLIPRGN

	switch (aFunction)
		{
	case 0x0213: // LINETO
		iGc->DrawLineTo(ExtractPoint(aArgPtr,EFalse));
		break;
	case 0x0214: // MOVETO
		iGc->MoveTo(ExtractPoint(aArgPtr,EFalse));
		break;
	case 0x0228: // FILLREGION
		{
		CGraphicsObject* regionObject = GraphicsObject(aArgPtr[0]);
		CGraphicsObject* brushObject = GraphicsObject(aArgPtr[1]);
		if (regionObject && (regionObject->Type() == KWmfGraphicsObjectRegion)
			&& brushObject && ((brushObject->Type() == KWmfGraphicsObjectBrush) || (brushObject->Type() == KWmfGraphicsObjectPatternBrush)))
			{
			CRectRegion* rectRgn = STATIC_CAST(CRectRegion*,regionObject);
			CFbsBitGcWrapper* gc = CloneGcL();

			gc->SetPenStyle(CGraphicsContext::ENullPen);
			brushObject->Apply(iGc);
			gc->DrawRect(rectRgn->iRect);

			delete gc;
			}
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1000L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0231 - SETMAPPERFLAGS
	// 0x0234 - SELECTPALETTE

	switch (aFunction)
		{
	case 0x02fa: // CREATEPENINDIRECT
		{
		TInt style = aArgPtr[0];

		CPen* pen = NULL;

		if (iDefaultPen)
			{
			pen = iDefaultPen;
			iDefaultPen = NULL;
			}
		else
			pen = new(ELeave) CPen;

		switch (style)
			{
		case 0: // PS_SOLID
			pen->iStyle = CGraphicsContext::ESolidPen;
			break;
		case 1: // PS_DASH
			pen->iStyle = CGraphicsContext::EDashedPen;
			break;
		case 2: // PS_DOT
			pen->iStyle = CGraphicsContext::EDottedPen;
			break;
		case 3: // PS_DASHDOT
			pen->iStyle = CGraphicsContext::EDotDashPen;
			break;
		case 4: // PS_DASHDOTDOT
			pen->iStyle = CGraphicsContext::EDotDotDashPen;
			break;
		case 5: // PS_NULL
			pen->iStyle = CGraphicsContext::ENullPen;
			break;
		case 6: // PS_INSIDEFRAME
		case 7: // PS_USERSTYLE
		case 8: // PS_ALTERNATE
		default:
			pen->iStyle = CGraphicsContext::ESolidPen;
			break;
			}
		pen->iSize.iWidth = Max(LogicalSizeToDeviceSize(aArgPtr[1],ETrue),1);
		pen->iSize.iHeight = Max(LogicalSizeToDeviceSize(aArgPtr[1],EFalse),1);
		pen->iColor = ExtractColor(aArgPtr + 3);

		AddGraphicsObject(pen);
		}
		break;
	case 0x02fb: // CREATEFONTINDIRECT
		{
		const TInt height = Abs(TInt((TInt16)aArgPtr[0]));
		const TInt weight = Abs(TInt(aArgPtr[4]));
		const TInt italic = Abs(aArgPtr[5] & 0xff);
		const TInt underline = Abs(aArgPtr[5] >> 8);
		const TInt strikeOut = Abs(aArgPtr[6] & 0xff);
		const TInt pitchAndFamily = Abs(aArgPtr[8] >> 8);

		TFontSpec fontSpec;
		fontSpec.iTypeface.SetIsProportional(((pitchAndFamily & 3) != KFontFixedPitch));

		switch (pitchAndFamily >> 4)
			{
		case KFontFamilyRoman:
		case KFontFamilyScript:
		case KFontFamilyDecorative:
			fontSpec.iTypeface.SetIsProportional(ETrue);
			fontSpec.iTypeface.SetIsSerif(ETrue);
			break;
		case KFontFamilySwiss:
			fontSpec.iTypeface.SetIsProportional(ETrue);
			fontSpec.iTypeface.SetIsSerif(EFalse);
			break;
		case KFontFamilyMono:
			fontSpec.iTypeface.SetIsProportional(EFalse);
			fontSpec.iTypeface.SetIsSerif(ETrue);
			break;
		default:
			break;
			}

		const TInt fontHeight = LogicalSizeToDeviceSize(height,EFalse);
		fontSpec.iHeight = Max(fontHeight,1);

		if (weight > KFontNormalWeightLimit)
			fontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightBold);
		if (italic)
			fontSpec.iFontStyle.SetPosture(EPostureItalic);

		CFbsFont* font = NULL;
		User::LeaveIfError(iDevice->GetNearestFontInPixels(font,fontSpec));

		CFontObject* fontObject = new CFontObject(*iDevice,*font,underline,strikeOut);

		if (!fontObject)
			{
			iDevice->ReleaseFont(font);
			User::Leave(KErrNoMemory);
			}
		else
			AddGraphicsObject(fontObject);
		}
		break;
	case 0x02fc: // CREATEBRUSHINDIRECT
		{
		TInt style = aArgPtr[0];
		TInt hatch = aArgPtr[3];

		CBrush* brush = NULL;

		if (iDefaultBrush)
			{
			brush = iDefaultBrush;
			iDefaultBrush = NULL;
			}
		else
			brush = new(ELeave) CBrush;

		switch (style)
			{
		case 0: // BS_SOLID
			brush->iStyle = CGraphicsContext::ESolidBrush;
			break;
		case 1: // BS_NULL, BS_HOLLOW           
			brush->iStyle = CGraphicsContext::ENullBrush;
			break;
		case 2: // BS_HATCHED
			switch (hatch)
				{
			case 0: // HS_HORIZONTAL
				brush->iStyle = CGraphicsContext::EHorizontalHatchBrush;
				break;
			case 1: // HS_VERTICAL
				brush->iStyle = CGraphicsContext::EVerticalHatchBrush;
				break;
			case 2: // HS_FDIAGONAL
				brush->iStyle = CGraphicsContext::EForwardDiagonalHatchBrush;
				break;
			case 3: // HS_BDIAGONAL
				brush->iStyle = CGraphicsContext::ERearwardDiagonalHatchBrush;
				break;
			case 4: // HS_CROSS
				brush->iStyle = CGraphicsContext::ESquareCrossHatchBrush;
				break;
			case 5: // HS_DIAGCROSS
				brush->iStyle = CGraphicsContext::EDiamondCrossHatchBrush;
				break;
			default:
				brush->iStyle = CGraphicsContext::ESolidBrush;
				break;
				}
			break;
		case 3: // BS_PATTERN
		case 4: // BS_INDEXED
		case 5: // BS_DIBPATTERN
		case 6: // BS_DIBPATTERNPT
		case 7: // BS_PATTERN8X8
		case 8: // BS_DIBPATTERN8X8
		case 9: // BS_MONOPATTERN
		default:
			brush->iStyle = CGraphicsContext::ESolidBrush;
			break;
			}
		brush->iColor = ExtractColor(aArgPtr + 1);

		AddGraphicsObject(brush);
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1001L(TInt aFunction,TUint16* aArgPtr)
	{
	switch (aFunction)
		{
	case 0x0324: // POLYGON
	case 0x0325: // POLYLINE
		{
		//check if the record size sufficient for polygon/polyline record.
		if(iRecordSizeInWords < KWmfPolyRecordSizeInWords)
			{
			User::Leave(KErrCorrupt);
			}

		const TInt numPoints = aArgPtr[0];
		if (numPoints <= 0)
			break;

		//check if the entire data about polygon/polyline is within the record.Each point needs 2 words
		if(iRecordSizeInWords < (KWmfPolyRecordSizeInWords  + (numPoints * 2)))
			{
			User::Leave(KErrCorrupt);
			}

		if (numPoints > iPointArrayEntries)
			{
			delete[] iPointArray;
			iPointArray = NULL;
			iPointArray = new(ELeave) TPoint[numPoints];
			iPointArrayEntries = numPoints;
			}

		TUint16* pointPtr = aArgPtr + 1;
		TPoint* pointArrayPtr = iPointArray;

		for (TInt count = 0; count < numPoints; count++)
			{
			*pointArrayPtr++ = ExtractPoint(pointPtr,ETrue);
			pointPtr += 2;
			}

		if (aFunction == 0x0324) // POLYGON
			iGc->DrawPolygonL(iPointArray,numPoints,iFillRule);
		else // POLYLINE
			iGc->DrawPolyLine(iPointArray, numPoints);
		}
		break;
	case 0x0410: // SCALEWINDOWEXT
		{
		TInt xMult = aArgPtr[3];
		TInt xDiv = aArgPtr[2];
		TInt yMult = aArgPtr[1];
		TInt yDiv = aArgPtr[0];

		if (xDiv != 0)
			iWindowExt.iWidth = (iWindowExt.iWidth * xMult) / xDiv;
		if (yDiv != 0)
			iWindowExt.iHeight = (iWindowExt.iHeight * yMult) / yDiv;
		}
		break;
	case 0x0412: // SCALEVIEWPORTEXT
		{
		if (iIgnoreViewportMetaData == EFalse)
			{
			TInt xMult = aArgPtr[3];
			TInt xDiv = aArgPtr[2];
			TInt yMult = aArgPtr[1];
			TInt yDiv = aArgPtr[0];
	
			if (xDiv != 0)
				iViewportExt.iWidth = (iViewportExt.iWidth * xMult) / xDiv;
			if (yDiv != 0)
				iViewportExt.iHeight = (iViewportExt.iHeight * yMult) / yDiv;
			}
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1010L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0415 - EXCLUDECLIPRECT
	// 0x0419 - FLOODFILL

	switch (aFunction)
		{
	case 0x0416: // INTERSECTCLIPRECT
		{
		iIntersectRegion.Clear();
		iIntersectRegion.AddRect(ExtractRect(aArgPtr));

		iIntersectRegion.Intersect(iClipRegion);
		iGc->SetClippingRegion(&iIntersectRegion);
		}
		break;
	case 0x0418: // ELLIPSE
		iGc->DrawEllipse(ExtractRect(aArgPtr));
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1011L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0436 - ANIMATEPALETTE

	switch (aFunction)
		{
	case 0x041b: // RECTANGLE
		iGc->DrawRect(ExtractRect(aArgPtr));
		break;
	case 0x041f: // SETPIXEL
		{
		CFbsBitGcWrapper* gc = CloneGcL();

		gc->SetPenColor(ExtractColor(aArgPtr));
		gc->Plot(ExtractPoint(aArgPtr + 2,EFalse));

		delete gc;
		}
		break;
	case 0x0429: // FRAMEREGION
		{
		CGraphicsObject* object1 = GraphicsObject(aArgPtr[0]);
		CGraphicsObject* object2 = GraphicsObject(aArgPtr[1]);

		if (object1 && object1->Type() == KWmfGraphicsObjectRegion &&
			object2 && ((object2->Type() == KWmfGraphicsObjectBrush) || (object2->Type() == KWmfGraphicsObjectPatternBrush)))
			{
			CRectRegion* region = STATIC_CAST(CRectRegion*,object1);
			CBrush* brush = STATIC_CAST(CBrush*,object2);

			if (brush->iStyle == CGraphicsContext::ESolidBrush)
				{
				CFbsBitGcWrapper* gc = CloneGcL();

				gc->SetPenStyle(CGraphicsContext::ESolidPen);
				gc->SetPenColor(brush->iColor);
				gc->SetPenSize(TSize(aArgPtr[3],aArgPtr[2]));
				gc->SetBrushStyle(CGraphicsContext::ENullBrush);

				gc->DrawRect(region->iRect);

				delete gc;
				}
			}
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1100L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0548 - EXTFLOODFILL

	switch (aFunction)
		{
	case 0x0521: // TEXTOUT
		{
		CFbsBitGcWrapper* gc = CloneGcL();
		CleanupStack::PushL(gc);

		gc->SetPenStyle(CGraphicsContext::ESolidPen);
		gc->SetTextPenColor();
		if (iBkModeOpaque)
			gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		else
			gc->SetBrushStyle(CGraphicsContext::ENullBrush);
		
		if(iRecordSizeInWords < KMinTextoutRecordSizeInWords)
			{
			// Invalid Textout record.
			User::Leave(KErrCorrupt);
			}
		//read the count of text-string bytes. The text is padded if necessary.
		//The count doesn't include the padded byte.
		const TInt count = *aArgPtr++;
		//convert into words
		TUint stringWords = (count+1) / 2;
		if ((KMinTextoutRecordSizeInWords + stringWords) > iRecordSizeInWords)
			{
			// Invalid Textout record.
			User::Leave(KErrCorrupt);
			}
		const TPtrC8 text((TUint8*)aArgPtr,count);

		DrawTextL(text, ExtractPoint(aArgPtr + stringWords, EFalse), gc);

		CleanupStack::PopAndDestroy(gc);
		}
		break;
	case 0x0538: // POLYPOLYGON
		{
		//check if the record size sufficient for polygon/polyline record.
		if(iRecordSizeInWords < KWmfPolyRecordSizeInWords)
			{
			User::Leave(KErrCorrupt);
			}
		const TInt numPolygons = *aArgPtr++;
		if (numPolygons <= 0)
			break;
		
		//check if the words that contain the number of points for each polygon are within the record
		if(iRecordSizeInWords < (KWmfPolyRecordSizeInWords  + numPolygons))
			{
			User::Leave(KErrCorrupt);
			}
		
		//index of word that has point for polygons
		TInt pointWord = KWmfPolyRecordSizeInWords  + numPolygons;
		TUint16* pointArgs = aArgPtr + numPolygons;
		for (TInt polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
			{
			const TInt numPoints = aArgPtr[polygonIndex];
			if (numPoints <= 0)
				break;

			//check if the point values are available within the record; each point needs 2 words
			pointWord += numPoints * 2;
			if(iRecordSizeInWords < pointWord )
				{
				User::Leave(KErrCorrupt);
				}

			if (numPoints > iPointArrayEntries)
				{
				delete[] iPointArray;
				iPointArray = NULL;
				iPointArray = new(ELeave) TPoint[numPoints];
				iPointArrayEntries = numPoints;
				}

			TPoint* pointArrayPtr = iPointArray;
			for (TInt pointIndex = 0; pointIndex < numPoints; pointIndex++)
				{
				*pointArrayPtr++ = ExtractPoint(pointArgs,ETrue);
				pointArgs += 2;
				}

			iGc->DrawPolygonL(iPointArray,numPoints,iFillRule);
			}
		}
		break;
	case 0x061c: // ROUNDRECT
		iGc->DrawRoundRect(ExtractRect(aArgPtr + 2),ExtractPoint(aArgPtr,EFalse).AsSize());
		break;
	case 0x061d: // PATBLT
		{
		TInt left = LogicalCoordToDeviceCoord(aArgPtr[5],ETrue);
		TInt top = LogicalCoordToDeviceCoord(aArgPtr[4],EFalse);
		TInt width = LogicalSizeToDeviceSize(aArgPtr[3],ETrue);
		TInt height = LogicalSizeToDeviceSize(aArgPtr[2],EFalse);

		CFbsBitGcWrapper* gc = CloneGcL();

		gc->SetPenStyle(CGraphicsContext::ENullPen);
		SetROP3(gc, aArgPtr[0] | (aArgPtr[1] << 16));
		gc->DrawRect(TRect(TPoint(left,top),TSize(width,height)));

		delete gc;
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1101L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0626 - ESCAPE

	switch (aFunction)
		{
	case 0x06ff: // CREATEREGION
		{
		TInt left = LogicalCoordToDeviceCoord(aArgPtr[7],ETrue);
		TInt top = LogicalCoordToDeviceCoord(aArgPtr[8],EFalse);
		TInt right = LogicalCoordToDeviceCoord(aArgPtr[9],ETrue);
		TInt bottom = LogicalCoordToDeviceCoord(aArgPtr[10],EFalse);

		AddGraphicsObject(new(ELeave) CRectRegion(*iDevice, iClipRegion, left, top, right, bottom));
		}
		break;
	case 0x0817: // ARC
		iGc->DrawArc(ExtractRect(aArgPtr + 4),ExtractPoint(aArgPtr + 2,EFalse),ExtractPoint(aArgPtr,EFalse));
		break;
	case 0x081a: // PIE
		iGc->DrawPie(ExtractRect(aArgPtr + 4),ExtractPoint(aArgPtr + 2,EFalse),ExtractPoint(aArgPtr,EFalse));
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1110L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0830 - CHORD
	// 0x0922 - BITBLT
	// 0x0940 - DIBBITBLT

	switch (aFunction)
		{
	case 0x0a32: // EXTTEXTOUT
		{
		CFbsBitGcWrapper* gc = CloneGcL();
		CleanupStack::PushL(gc);

		gc->SetPenStyle(CGraphicsContext::ESolidPen);
		gc->SetTextPenColor();
		if (iBkModeOpaque)
			gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		else
			gc->SetBrushStyle(CGraphicsContext::ENullBrush);

		const TPoint pos(ExtractPoint(aArgPtr,EFalse));
		const TInt count = aArgPtr[2];
		const TInt flag = aArgPtr[3];

		aArgPtr += (flag & KExtTextOutFlagClipped) ? 8 : 4;

		const TPtrC8 text((TUint8*)aArgPtr,count);

		DrawTextL(text,pos,gc);

		CleanupStack::PopAndDestroy(gc);
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DoProcess1111L(TInt aFunction,TUint16* aArgPtr)
	{
	// Not supported:
	// 0x0b23 - STRETCHBLT
	// 0x0d33 - SETDIBTODEV

	switch (aFunction)
		{
	case 0x0b41: // DIBSTRETCHBLT
		{
		const TInt16* argPtr = (TInt16*)aArgPtr;
		const TInt srcX = argPtr[5];
		const TInt srcY = argPtr[4];
		const TInt srcWidth = argPtr[3];
		const TInt srcHeight = argPtr[2];
		const TInt dstX = LogicalCoordToDeviceCoord(argPtr[9],ETrue);
		const TInt dstY = LogicalCoordToDeviceCoord(argPtr[8],EFalse);
		const TInt dstWidth = LogicalSizeToDeviceSize(argPtr[7],ETrue);
		const TInt dstHeight = LogicalSizeToDeviceSize(argPtr[6],EFalse);

		CFbsBitmap* dib = new(ELeave) CFbsBitmap;
		TInt err = ExtractDib(aArgPtr + 10, dib);

		CleanupStack::PushL(dib);
		if (err == KErrNone)
			{
			const TRect srcRect(TPoint(srcX,srcY),TSize(srcWidth,srcHeight));
			const TRect dstRect(TPoint(dstX,dstY),TSize(dstWidth,dstHeight));

			iGc->DrawBitmap(dstRect,dib,srcRect);
			}
		else if (err != KErrNotSupported) // Ignore KErrNotSupported
			User::Leave(err);

		CleanupStack::PopAndDestroy(dib);
		}
		break;
	case 0x0f43: // STRETCHDIB
		{
		const TInt16* argPtr = (TInt16*)aArgPtr;
		const TInt srcX = argPtr[6];
		const TInt srcY = argPtr[5];
		const TInt srcWidth = argPtr[4];
		const TInt srcHeight = argPtr[3];
		const TInt dstX = LogicalCoordToDeviceCoord(argPtr[10],ETrue);
		const TInt dstY = LogicalCoordToDeviceCoord(argPtr[9],EFalse);
		const TInt dstWidth = LogicalSizeToDeviceSize(argPtr[8],ETrue);
		const TInt dstHeight = LogicalSizeToDeviceSize(argPtr[7],EFalse);

		CFbsBitmap* dib = new(ELeave) CFbsBitmap;
		TInt err = ExtractDib(aArgPtr + 11, dib);

		CleanupStack::PushL(dib);
		if (err == KErrNone)
			{
			const TRect srcRect(TPoint(srcX,srcY),TSize(srcWidth,srcHeight));
			const TRect dstRect(TPoint(dstX,dstY),TSize(dstWidth,dstHeight));

			iGc->DrawBitmap(dstRect,dib,srcRect);
			}
		else if (err != KErrNotSupported) // Ignore KErrNotSupported
			User::Leave(err);

		CleanupStack::PopAndDestroy(dib);
		}
		break;
	default:
		break;
		}
	}

void CWmfReadCodec::DrawTextL(const TDesC8& aText, const TPoint& aPos, CFbsBitGcWrapper* aGc)
	{
	if(aText.Length()==0)
		return;

	HBufC* unicodeText = HBufC::NewMaxLC(aText.Length());
	TPtr16 text = unicodeText->Des();
	text.Copy(aText);

	TPoint pos(aPos);
	const CFbsFont* font = iGc->CurrentFont();

	if (font)
		{
		const TInt textAlignHorz = iTextAlignFlags & KTextAlignMaskHorz;
		const TInt textAlignVert = iTextAlignFlags & KTextAlignMaskVert;

		switch (textAlignHorz)
			{
		case KTextAlignFlagRight:
			pos.iX -= font->TextWidthInPixels(text);
			break;
		case KTextAlignFlagCenter:
			pos.iX -= font->TextWidthInPixels(text) / 2;
			break;
		default: // KTextAlignFlagLeft
			break;
			}

		// rather than use font->AscentInPixels(), font->DescentInPixels(),
		// which don't include the height of accents etc, 
		// use the maximum possible ascent/decent instead
		TInt ascent;
		TInt descent;
		TOpenFontMetrics fontMetrics;
		if (font->IsOpenFont() && font->GetFontMetrics(fontMetrics))
			{
			ascent = fontMetrics.MaxHeight();
			descent = fontMetrics.MaxDepth();
			}
		else
			{
			ascent = font->AscentInPixels();
			descent = font->DescentInPixels();
			}

		switch (textAlignVert)
			{
		case KTextAlignFlagBottom:
			pos.iY -= descent;
			break;
		case KTextAlignFlagBaseline:
			break;
		default: // KTextAlignFlagTop
			pos.iY += ascent;
			break;
			}
		aGc->DrawText(*unicodeText,pos);
		}

	CleanupStack::PopAndDestroy(unicodeText);
	}

void CWmfReadCodec::AddGraphicsObject(CGraphicsObject* aObject)
	{
	for (TInt index = 0; index < KWmfMaxGraphicsObjectCount; index++)
		{
		if (iGraphicsObject[index] == NULL)
			{
			iGraphicsObject[index] = aObject;
			return;
			}
		}

	delete aObject; // No more space so throw the object away
	}

CGraphicsObject* CWmfReadCodec::GraphicsObject(TInt aHandle)
	{
	if (aHandle < KWmfMaxGraphicsObjectCount)
		return iGraphicsObject[aHandle];

	return NULL;
	}

void CWmfReadCodec::RemoveGraphicsObject(TInt aHandle)
	{
	if (aHandle < KWmfMaxGraphicsObjectCount)
		{
		CGraphicsObject* object = iGraphicsObject[aHandle];

		if (object)
			{
			if (object->Type() == KWmfGraphicsObjectPen && !iDefaultPen)
				iDefaultPen = STATIC_CAST(CPen*,object);
			else if (object->Type() == KWmfGraphicsObjectBrush && !iDefaultBrush)
				iDefaultBrush = STATIC_CAST(CBrush*,object);
			else
				delete object;

			iGraphicsObject[aHandle] = NULL;
			}
		}
	}

CFbsBitGcWrapper* CWmfReadCodec::CloneGcL()
	{
	CFbsBitGcWrapper* gcCopy;
	if (iMaskDevice)
		gcCopy = CFbsBitGcMaskWrapper::NewL(*iDevice, *iMaskDevice, static_cast<CFbsBitGcMaskWrapper*> (iGc));
	else
		gcCopy = CFbsBitGcWrapper::NewL(*iDevice, iGc);

	return gcCopy;
	}

TInt CWmfReadCodec::LogicalCoordToDeviceCoord(TInt aLogicalCoord,TBool aHorizontal)
	{
	if (aHorizontal)
		{
		if (iWindowExt.iWidth != 0)
			return ((aLogicalCoord - iWindowOrg.iX) * iViewportExt.iWidth / iWindowExt.iWidth) + iViewportOrg.iX;
		}
	else
		{
		if (iWindowExt.iHeight != 0)
			return ((aLogicalCoord - iWindowOrg.iY) * iViewportExt.iHeight / iWindowExt.iHeight) + iViewportOrg.iY;
		}

	return 0;
	}

TInt CWmfReadCodec::LogicalSizeToDeviceSize(TInt aLogicalSize,TBool aHorizontal)
	{
	if (aHorizontal)
		{
		if (iWindowExt.iWidth != 0)
			return aLogicalSize * iViewportExt.iWidth / iWindowExt.iWidth;
		}
	else
		{
		if (iWindowExt.iHeight != 0)
			return aLogicalSize * iViewportExt.iHeight / iWindowExt.iHeight;
		}

	return 0;
	}

void CWmfReadCodec::SetROP(CFbsBitGcWrapper* aGc,TInt aROP)
	{
	CGraphicsContext::TDrawMode drawMode = CGraphicsContext::EDrawModePEN;

	switch (aROP)
		{
	case 1: // R2_BLACK
		aGc->SetPenColor(KRgbBlack);
		aGc->SetBrushColor(KRgbBlack);
		aGc->SetDrawMode(drawMode);
		return;

	case 11: // R2_NOP
		aGc->SetPenStyle(CGraphicsContext::ENullPen);
		aGc->SetBrushStyle(CGraphicsContext::ENullBrush);
		aGc->SetDrawMode(drawMode);
		return;

	case 16: // R2_WHITE
		aGc->SetPenColor(KRgbWhite);
		aGc->SetBrushColor(KRgbWhite);
		aGc->SetDrawMode(drawMode);
		return;

	default:
		break;
		}

	switch (aROP)
		{
	case 2:		drawMode = CGraphicsContext::EDrawModeNOTANDNOT;	break; // R2_NOTMERGEPEN
	case 3:		drawMode = CGraphicsContext::EDrawModeANDNOT;		break; // R2_MASKNOTPEN
	case 4:		drawMode = CGraphicsContext::EDrawModeNOTPEN;		break; // R2_NOTCOPYPEN
	case 5:		drawMode = CGraphicsContext::EDrawModeNOTAND;		break; // R2_MASKPENNOT
	case 6:		drawMode = CGraphicsContext::EDrawModeNOTSCREEN;	break; // R2_NOT
	case 7:		drawMode = CGraphicsContext::EDrawModeXOR;			break; // R2_XORPEN
	case 8:		drawMode = CGraphicsContext::EDrawModeNOTORNOT;		break; // R2_NOTMASKPEN
	case 9:		drawMode = CGraphicsContext::EDrawModeAND;			break; // R2_MASKPEN
	case 10:	drawMode = CGraphicsContext::EDrawModeNOTXOR;		break; // R2_NOTXORPEN

	case 12:	break; // R2_MERGENOTPEN (not supported)
	case 13:	break; // R2_COPYPEN
	case 14:	drawMode = CGraphicsContext::EDrawModeNOTOR;		break; // R2_MERGEPENNOT
	case 15:	drawMode = CGraphicsContext::EDrawModeOR;			break; // R2_MERGEPEN

	default:
		break;
		}

	aGc->RestorePenAndBrush();
	aGc->SetDrawMode(drawMode);
	}

void CWmfReadCodec::SetROP3(CFbsBitGcWrapper* aGc,TInt aROP)
	{
	CGraphicsContext::TDrawMode drawMode = CGraphicsContext::EDrawModePEN;
	switch (aROP)
		{
	case 0x005A0049: // PATINVERT
		drawMode = CGraphicsContext::EDrawModeXOR;
		break;
	case 0x00550009: // DSTINVERT
		drawMode = CGraphicsContext::EDrawModeNOTSCREEN;
		aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		break;
	case 0x00000042: // BLACKNESS
		aGc->SetBrushColor(KRgbBlack);
		aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		break;
	case 0x00FF0062: // WHITENESS
		aGc->SetBrushColor(KRgbWhite);
		aGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
		break;

	case 0x00F00021: // PATCOPY
	default:
		break;
		}

	aGc->SetDrawMode(drawMode);
	}

TRgb CWmfReadCodec::ExtractColor(const TUint16* aArgPtr)
	{
	return TRgb(aArgPtr[0] & 0xff,aArgPtr[0] >> 8,aArgPtr[1] & 0xff);
	}

TPoint CWmfReadCodec::ExtractPoint(const TUint16* aArgPtr,TBool aXYOrder)
	{
	TInt16* coordPtr = (TInt16*)aArgPtr;

	TInt left = coordPtr[0];
	TInt top = coordPtr[1];

	if (!aXYOrder)
		{
		TInt temp = left;
		left = top;
		top = temp;
		}

	left = LogicalCoordToDeviceCoord(left,ETrue);
	top = LogicalCoordToDeviceCoord(top,EFalse);

	return TPoint(left,top);
	}

TRect CWmfReadCodec::ExtractRect(const TUint16* aArgPtr)
	{
	TInt16* coordPtr = (TInt16*)aArgPtr;

	TInt left = LogicalCoordToDeviceCoord(coordPtr[3],ETrue);
	TInt top = LogicalCoordToDeviceCoord(coordPtr[2],EFalse);
	TInt right = LogicalCoordToDeviceCoord(coordPtr[1],ETrue);
	TInt bottom = LogicalCoordToDeviceCoord(coordPtr[0],EFalse);

	return TRect(left,top,right,bottom);
	}

TInt CWmfReadCodec::ExtractDib(const TUint16* aArgPtr, CPatternBrush* aPatternBrush)
	{
	TInt bitCount = aArgPtr[7];
	if (bitCount != 1)
		return ExtractDib(aArgPtr, aPatternBrush->Bitmap());

	TInt structSize = aArgPtr[0] | (aArgPtr[1] << 16);
	TInt width = aArgPtr[2] | (aArgPtr[3] << 16);
	TInt height = aArgPtr[4] | (aArgPtr[5] << 16);

	// Skip biPlanes half-word
	TInt compression = aArgPtr[8] | (aArgPtr[9] << 16);
	if (compression)
		return KErrNotSupported;

	// Skip biSizeImage word
	TInt xPixelsPerMeter = aArgPtr[12] | (aArgPtr[13] << 16);
	TInt yPixelsPerMeter = aArgPtr[14] | (aArgPtr[15] << 16);

	// Skip biClrUsed word
	// Skip biClrImportant word

	TUint8* bytePtr = (TUint8*)aArgPtr;
	bytePtr += structSize;

	CFbsBitmap* bitmap = aPatternBrush->Bitmap();
	TInt err = bitmap->Create(TSize(width,height), EColor64K);
	if (err != KErrNone)
		return err;

	TSize twipsSize(0,0);
	if (xPixelsPerMeter > 0)
		twipsSize.iWidth = KTwipsPerCm * 100 * width / xPixelsPerMeter;
	if (yPixelsPerMeter > 0)
		twipsSize.iHeight = KTwipsPerCm * 100 * height / yPixelsPerMeter;
	bitmap->SetSizeInTwips(twipsSize);

	// Skip the palette.
	bytePtr+= 8;

	// Set the pattern bits.
	TInt padding = ((width + 31) & ~31) - width;
	TInt byteTotal = ((width+padding)*height)/8;
	TUint8* bitmapBits = new TUint8[byteTotal];
	if (!bitmapBits)
		return KErrNoMemory;

	Mem::Copy(bitmapBits, bytePtr, byteTotal);
	aPatternBrush->SetBitmapBits(bitmapBits);
	return KErrNone;
	}

TInt CWmfReadCodec::ExtractDib(const TUint16* aArgPtr, CFbsBitmap* aBitmap)
	{
	const TInt structSize = aArgPtr[0] | (aArgPtr[1] << 16);
	const TInt width = aArgPtr[2] | (aArgPtr[3] << 16);
	const TInt height = aArgPtr[4] | (aArgPtr[5] << 16);

	// Skip biPlanes half-word
	const TInt bitCount = aArgPtr[7];
	const TInt compression = aArgPtr[8] | (aArgPtr[9] << 16);
	if (((bitCount != 24) && (bitCount != 8) && (bitCount != 1)) || compression)
		return KErrNotSupported;

	// Skip biSizeImage word
	const TInt xPixelsPerMeter = aArgPtr[12] | (aArgPtr[13] << 16);
	const TInt yPixelsPerMeter = aArgPtr[14] | (aArgPtr[15] << 16);

	// Skip biClrUsed word
	// Skip biClrImportant word

	TUint8* bytePtr = (TUint8*)aArgPtr;
	bytePtr += structSize;

	TInt err = aBitmap->Create(TSize(width,height), EColor64K);
	if (err != KErrNone)
		return err;

	TSize twipsSize(0,0);
	if (xPixelsPerMeter > 0)
		twipsSize.iWidth = KTwipsPerCm * 100 * width / xPixelsPerMeter;
	if (yPixelsPerMeter > 0)
		twipsSize.iHeight = KTwipsPerCm * 100 * height / yPixelsPerMeter;
	aBitmap->SetSizeInTwips(twipsSize);

	TBitmapUtil util(aBitmap);
	if (bitCount == 24)
		{
		TInt byteWidth = 3*width;
		TInt padding = ((byteWidth + 3) & ~3) - byteWidth;
		for (TInt y = height - 1; y >= 0; y--)
			{
			util.Begin(TPoint(0,y));
			for (TInt x = 0; x < width; x++)
				{
				TRgb color(bytePtr[2],bytePtr[1],bytePtr[0]);
				util.SetPixel(color.Color64K());
				util.IncXPos();
				bytePtr += 3;
				}
			bytePtr += padding;
			util.End();
			}
		}
	else if (bitCount == 8)
		{
		// Read the palette.
		TRgb* palette = new TRgb[256];
		if (!palette)
			return KErrNoMemory;

		for (TInt count = 0; count < 256; count++)
			{
			TInt blue = bytePtr[0];
			TInt green = bytePtr[1];
			TInt red = bytePtr[2];
			
			bytePtr += 4; // Skip rgbReserved
			palette[count] = TRgb(red, green, blue);
			}

		const TInt padding = ((width + 3) & ~3) - width;
		for (TInt y = height - 1; y >= 0; y--)
			{
			util.Begin(TPoint(0,y));
			for (TInt x = 0; x < width; x++)
				{
				TRgb color(palette[bytePtr[0]]);
				util.SetPixel(color.Color64K());
				util.IncXPos();
				bytePtr++;
				}
			bytePtr += padding;
			util.End();
			}

		delete[] palette;
		}
	else // 1bpp
		{
		// Read the palette.
		TRgb palette[2];
		for (TInt count = 0; count < 2; count++)
			{
			TInt blue = bytePtr[0];
			TInt green = bytePtr[1];
			TInt red = bytePtr[2];
			
			bytePtr += 4; // Skip rgbReserved
			palette[count] = TRgb(red, green, blue);
			}

		const TInt padding = ((width + 31) & ~31) - width;
		TUint pixelMask = 0x80;
		for (TInt y = height - 1; y >= 0; y--)
			{
			util.Begin(TPoint(0,y));

			TInt x;
			for (x = 0; x < width; x++)
				{
				TRgb color(palette[(bytePtr[0] & pixelMask) ? 1 : 0]);
				util.SetPixel(color.Color64K());
				util.IncXPos();

				pixelMask >>= 1;
				if (pixelMask == 0)
					{
					pixelMask = 0x80;
					bytePtr++;
					}
				}

			for (x = 0 ; x<padding ; x++)
				{
				pixelMask >>= 1;
				if (pixelMask == 0)
					{
					pixelMask = 0x80;
					bytePtr++;
					}
				}

			util.End();
			}
		}

	return KErrNone;
	}

TInt CWmfReadCodec::ReducedSize(const TSize& aOriginalSize,TInt aReductionFactor, TSize& aReducedSize) const
	{
	aReducedSize = aOriginalSize;
   	if (aReductionFactor<0)
   	   {
   	   return KErrArgument;
   	   }
   	TInt roundup = aReductionFactor>0? 1<<(aReductionFactor-1): 0;
   	aReducedSize.SetSize(((aOriginalSize.iWidth+roundup)>>aReductionFactor),
               ((aOriginalSize.iHeight+roundup)>>aReductionFactor) );
    return KErrNone;            
	}

TInt CWmfReadCodec::ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const
	{
	if( (aReducedSize.iWidth<=0) || (aReducedSize.iHeight<=0))
      	{
      	return 0;
		}
   	TInt reductionFactor = 0;
   	TInt roundup=0;
	while( ((aOriginalSize.iWidth+roundup)>>reductionFactor) > aReducedSize.iWidth || 
         ((aOriginalSize.iHeight+roundup)>>reductionFactor) > aReducedSize.iHeight)
    	{
      	reductionFactor++;
      	roundup=1<<(reductionFactor-1);
    	}
	return reductionFactor;
	}

void CWmfReadCodec::SetIgnoreViewportMetaData(TBool aIgnoreViewportMetaData)
	{
	iIgnoreViewportMetaData = aIgnoreViewportMetaData;
	}
