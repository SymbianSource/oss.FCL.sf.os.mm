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

#ifndef __WMFCODEC_H__
#define __WMFCODEC_H__

#include <bitdev.h>
#include <icl/imageprocessor.h>
#include <icl/imagecodec.h>
#include <icl/imagecodecdata.h>

#include "WMFConvert.h"

// Constants.
// Record size (2 16-bit WORDS); Record identifier (1 16-bit WORD)
const TInt KWmfMinRecordSizeInWords = 3;
//Size of polyline, polygon and polypolygon records. Should be KWmfMinRecordSizeInWords + 1 
//word for number of points in polyline or number of points in polygon or number of polygons in polypolygon.
const TInt KWmfPolyRecordSizeInWords = 4; 

const TInt KWmfMaxGraphicsObjectCount = 256;
const TInt KWmfTerminateFunction = 0;
//KWmfMinRecordSizeInWords + 1 word each for string-length, Y-Position, X-Position
const TInt KMinTextoutRecordSizeInWords = KWmfMinRecordSizeInWords + 3;

const TInt KWmfGraphicsObjectPen = 0;
const TInt KWmfGraphicsObjectBrush = 1;
const TInt KWmfGraphicsObjectPatternBrush = 2;
const TInt KWmfGraphicsObjectFont = 3;
const TInt KWmfGraphicsObjectRegion = 4;
const TInt KWmfGraphicsObjectPalette = 5;
const TInt KWmfGraphicsObjectDummyBrush = 6;


// Helper classes.
class CPen;
class CBrush;
class CPatternBrush;
class CFbsBitGcWrapper : public CBase
	{
public:
	static CFbsBitGcWrapper* NewL(CFbsDevice& aDevice, CFbsBitGcWrapper* aGc = NULL);
	~CFbsBitGcWrapper();

	const CFbsFont* CurrentFont();

	virtual void SetPen(const CPen& aPen);
	virtual void SetBrush(CBrush& aBrush);
	virtual void RestorePenAndBrush();
	virtual void DrawPolygonL(const TPoint* aPointList,TInt aNumPoints,CGraphicsContext::TFillRule aFillRule);

	virtual void Clear();
	virtual void SetUnderlineStyle(TFontUnderline aUnderlineStyle);
	virtual void SetStrikethroughStyle(TFontStrikethrough aStrikethroughStyle);
	virtual void UseFont(const CFbsFont* aFont);
	virtual void SetClippingRegion(const TRegion* aRect);
	virtual void SetCharJustification(TInt aExcessWidth, TInt aNumGaps);
	virtual void SetWordJustification(TInt aExcessWidth, TInt aNumChars);
	virtual void DrawLineTo(const TPoint& aPoint);
	virtual void MoveTo(const TPoint& aPoint);
	virtual void DrawPolyLine(const TPoint* aPointList, TInt aNumPoints);
	virtual void DrawEllipse(const TRect& aRect);
	virtual void DrawRect(const TRect& aRect);
	virtual void DrawRoundRect(const TRect& aRect, const TSize& aEllipse);
	virtual void SetPenStyle(CGraphicsContext::TPenStyle aPenStyle);
	virtual void SetBrushStyle(CGraphicsContext::TBrushStyle aBrushStyle);
	virtual void SetBrushColor(const TRgb& aColor);
	virtual void SetDrawMode(CGraphicsContext::TDrawMode aDrawMode);
	virtual void SetPenColor(const TRgb& aColor);
	virtual void SetTextPenColor();
	virtual void Plot(const TPoint& aPoint);
	virtual void SetPenSize(const TSize& aSize);
	virtual void DrawArc(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd);
	virtual void DrawPie(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd);
	virtual void DrawBitmap(const TRect& aDestRect, const CFbsBitmap* aSource, const TRect& aSourceRect);
	virtual void DrawText(const TDesC& aText,const TPoint& aPosition);

	virtual void UseBrushPattern(const CFbsBitmap* aPatternBitmap);
	virtual void SetTextColor(TRgb aTextColor);
	virtual void SetBackgroundColor(TRgb aBackgroundColor);
	virtual void RealizeBrush();

protected:
	CFbsBitGcWrapper();
	void ConstructL(CFbsDevice& aDevice, CFbsBitGcWrapper* aGc);

protected:
	TRgb iBrushColor;
	CGraphicsContext::TBrushStyle iBrushStyle;

	TRgb iPenColor;
	CGraphicsContext::TPenStyle iPenStyle;
	TSize iPenSize;

	TRgb iTextColor;
	TRgb iBackgroundColor;
	CPatternBrush* iPatternBrush;

private:
	CFbsBitGc* iGc;
	CFbsBitGc* iPolygonGc;

	const CFbsFont* iCurrentFont; // Not owned.	
	};

class CFbsBitGcMaskWrapper : public CFbsBitGcWrapper
	{
public:
	static CFbsBitGcMaskWrapper* NewL(CFbsDevice& aDevice, CFbsDevice& aMaskDevice, CFbsBitGcMaskWrapper* aGc = NULL);
	~CFbsBitGcMaskWrapper();

	// From CFbsBitGcWrapper
	void SetPen(const CPen& aPen);
	void SetBrush(CBrush& aBrush);
	void DrawPolygonL(const TPoint* aPointList,TInt aNumPoints,CGraphicsContext::TFillRule aFillRule);

	void Clear();
	void SetUnderlineStyle(TFontUnderline aUnderlineStyle);
	void SetStrikethroughStyle(TFontStrikethrough aStrikethroughStyle);
	void UseFont(const CFbsFont* aFont);
	void SetClippingRegion(const TRegion* aRect);
	void SetCharJustification(TInt aExcessWidth, TInt aNumGaps);
	void SetWordJustification(TInt aExcessWidth, TInt aNumChars);
	void DrawLineTo(const TPoint& aPoint);
	void MoveTo(const TPoint& aPoint);
	void DrawPolyLine(const TPoint* aPointList, TInt aNumPoints);
	void DrawEllipse(const TRect& aRect);
	void DrawRect(const TRect& aRect);
	void DrawRoundRect(const TRect& aRect, const TSize& aEllipse);
	void SetPenStyle(CGraphicsContext::TPenStyle aPenStyle);
	void SetBrushStyle(CGraphicsContext::TBrushStyle aBrushStyle);
	void SetDrawMode(CGraphicsContext::TDrawMode aDrawMode);
	void Plot(const TPoint& aPoint);
	void SetPenSize(const TSize& aSize);
	void DrawArc(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd);
	void DrawPie(const TRect& aRect, const TPoint& aStart, const TPoint& aEnd);
	void DrawBitmap(const TRect& aDestRect, const CFbsBitmap* aSource, const TRect& aSourceRect);
	void DrawText(const TDesC& aText,const TPoint& aPosition);
	void UseBrushPattern(const CFbsBitmap* aPatternBitmap, TBool aHatchedBrush);

private:
	CFbsBitGcMaskWrapper();
	void ConstructL(CFbsDevice& aDevice, CFbsDevice& aMaskDevice, CFbsBitGcMaskWrapper* aGc);

	CFbsBitGc* iMaskGc;
	CFbsBitGc* iPolygonMaskGc;
	};


class CGraphicsObject : public CBase
	{
public:
	virtual void Apply(CFbsBitGcWrapper* aGc) = 0;
	virtual TInt Type() const = 0;
	};

class CPen : public CGraphicsObject
	{
public:
	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;
public:
	CGraphicsContext::TPenStyle iStyle;
	TSize iSize;
	TRgb iColor;
	};

class CDummyBrush : public CGraphicsObject
	{
public:
	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;
	};

class CBrush : public CGraphicsObject
	{
public:
	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;
public:
	CGraphicsContext::TBrushStyle iStyle;
	TRgb iColor;
	};

class CPatternBrush : public CBrush
	{
public:
	static CPatternBrush* NewL();
	~CPatternBrush();

	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;

	void SetBitmapBits(TUint8* aBitmapBits);
	void RealizeBrush(TRgb aTextColor, TRgb aBackgroundColor);

	CFbsBitmap* Bitmap();

private:
	CPatternBrush();
	void ConstructL();

private:
	CFbsBitmap* iBitmap;
	TUint8* iBitmapBits;

	TRgb iTextColor;
	TRgb iBackgroundColor;
	};

class CFontObject : public CGraphicsObject
	{
public:
	CFontObject(CFbsDevice& aDevice,CFbsFont& aFont,TBool aUnderline,TBool aStrikeOut);
	~CFontObject();
	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;
private:
	CFbsDevice& iDevice;
	CFbsFont& iFont;
	TFontUnderline iUnderline;
	TFontStrikethrough iStrikeOut;
	};

class CRectRegion : public CGraphicsObject
	{
public:
	CRectRegion(CFbsDevice& aDevice, TRegion& aClipRegion, TInt aLeft, TInt aTop, TInt aRight, TInt aBottom);
	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;
public:
	TRect iRect;

private:
	CFbsDevice& iDevice;
	TRegion& iClipRegion;
	};

class CPaletteObject : public CGraphicsObject
	{
public:
	virtual void Apply(CFbsBitGcWrapper* aGc);
	virtual TInt Type() const;
	};


// Read codec.
class CWmfDecoder;
class CWmfReadCodec : public CImageReadCodec
	{
public:
	//From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aBitmap, CFbsBitmap* aDestinationMask);
 
	static CWmfReadCodec* NewL(TInt aWordsExpected);
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	TInt ReducedSize(const TSize& aOriginalSize,TInt aReductionFactor, TSize& aReducedSize) const;
	TInt ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const;
	void SetIgnoreViewportMetaData(TBool aIgnoreViewportMetaData);
protected:
	~CWmfReadCodec();
	virtual void DoProcessL(TUint16*& aDataPtr,const TUint16* aDataPtrLimit);
	void DoProcess00L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess01L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess10L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess11L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0000L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0001L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0010L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0011L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0100L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0101L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0110L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess0111L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1000L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1001L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1010L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1011L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1100L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1101L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1110L(TInt aFunction,TUint16* aArgPtr);
	void DoProcess1111L(TInt aFunction,TUint16* aArgPtr);
	void DrawTextL(const TDesC8& aText, const TPoint& aPos, CFbsBitGcWrapper* aGc);
	void AddGraphicsObject(CGraphicsObject* aObject);
	CGraphicsObject* GraphicsObject(TInt aHandle);
	void RemoveGraphicsObject(TInt aHandle);
	CFbsBitGcWrapper* CloneGcL();
	TInt LogicalCoordToDeviceCoord(TInt aLogicalCoord,TBool aHorizontal);
	TInt LogicalSizeToDeviceSize(TInt aLogicalSize,TBool aHorizontal);
	void SetROP(CFbsBitGcWrapper* aGc, TInt aROP);
	void SetROP3(CFbsBitGcWrapper* aGc, TInt aROP);
	TRgb ExtractColor(const TUint16* aArgPtr);
	TPoint ExtractPoint(const TUint16* aArgPtr,TBool aXYOrder);
	TRect ExtractRect(const TUint16* aArgPtr);
	TInt ExtractDib(const TUint16* aArgPtr, CFbsBitmap* aBitmap);
	TInt ExtractDib(const TUint16* aArgPtr, CPatternBrush* aPatternBrush);
protected:
	enum TMapMode
		{
		EMapModeTEXT		= 1,
		EMapModeLOMETRIC	= 2,
		EMapModeHIMETRIC	= 3,
		EMapModeLOENGLISH	= 4,
		EMapModeHIENGLISH	= 5,
		EMapModeTWIPS		= 6,
		EMapModeISOTROPIC	= 7,
		EMapModeANISOTROPIC	= 8
		};
private:
	CWmfReadCodec(TInt aWordsExpected);
	void ConstructL();

private:
	TInt iWordsProcessed;
	TInt iWordsExpected;
	TBool iIgnoreViewportMetaData;

	CFbsBitmapDevice* iDevice;
	CFbsBitmapDevice* iMaskDevice;
	CFbsBitGcWrapper* iGc;

	RPointerArray<CFbsBitGcWrapper> iSavedGcStack;
	TBool iBkModeOpaque;
	TMapMode iMapMode;
	TPoint iWindowOrg;
	TSize iWindowExt;
	TPoint iViewportOrg;
	TSize iViewportExt;
	CGraphicsObject* iGraphicsObject[KWmfMaxGraphicsObjectCount];

	CPen* iDefaultPen;
	CBrush* iDefaultBrush;
	TRegionFix<1> iClipRegion;
	TRegionFix<1> iIntersectRegion;
	
	CGraphicsContext::TFillRule iFillRule;
	TPoint* iPointArray;
	TInt iPointArrayEntries;
	TUint32 iTextAlignFlags;

	// Record related information
	TUint16* iAccumulator;
	TUint16* iAccumulatorPtr;
	TInt iAccumulatorSizeInWords;
	TBool iLookingForRecord;
	TInt iRecordSizeInWords;
	TInt iFunction;
	TInt iWordsToRead;
	TBool iFinished;
	};


#endif // __WMFCODEC_H__

