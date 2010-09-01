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

#ifndef __TIFFFAX_H__
#define __TIFFFAX_H__

#include "TIFFCodec.h"

// Base class for 1d Tiff fax decoders
enum TTiffFaxCodeType
{
	ETiffFaxBlackRun,
	ETiffFaxWhiteRun,
	ETiffFaxEndOfLineCode
};

enum TTiffFaxState
	{
	ETiffFaxUnderflow,
	ETiffFaxPixelRun,
	ETiffFaxEndOfLine,
	ETiffFaxEndOfFax,
	ETiffFaxInvalid
	};

class CTiffFax1dDecoder : public CTiffReadSubCodec
	{
protected:
	virtual ~CTiffFax1dDecoder();
	
	// From CTiffImageDecoder
	virtual void	DoNewFrameL(CFbsBitmap& aBitmap, TBool aDisableErrorDiffusion, TInt aReductionFactor);
	virtual void	NewStripL(TInt aNumBytes);
	virtual TBool	ProcessStripL(TBufPtr8* aSrc);

	// New
	void					ConstructL(const TTiffImageData& aImageData);
	virtual TTiffFaxState	ProcessNextCode() = 0;
	virtual void			NewRow();
	void					UseBits(TInt aNumBits);
	TTiffFaxState			Decode1d();
	TTiffFaxState			DecodeBlack();
	TTiffFaxState			DecodeWhite();
	TTiffFaxState			DecodeEndOfLine();
protected:
	// Info
	const TTiffImageData*	iImageData;

	// Input
	const TUint8*				iSourcePtr;
	const TUint8*				iSourcePtrLimit;
	TBool				iReverseBitsInByte;
	TInt32				iBitCache;
	TInt				iBitCacheLength;
	
	// Decoding
	TTiffFaxCodeType	iColor;
	TTiffFaxCodeType	iCodeType;
	TInt				iCodeCount;
	
	// Buffer
	TInt*				iRunData;
	TInt				iRunDataIndex;
	TInt				iRowWidth;
	TInt				iPixelsStoredInRow;
	
	// Output
	TInt				iRowNumber;
	TInt				iRowsRemaining;
	CImageProcessor* iImageProcessor;
	TRgb				iBlack;
	TRgb				iWhite;
	};

// Base class for 2d Tiff fax decoders
class CTiffFax2dDecoder : public CTiffFax1dDecoder
	{
protected:
	virtual ~CTiffFax2dDecoder();
	
	// From CTiffFax1dDecoder 
	virtual void			NewStripL(TInt aNumBytes);
	virtual void			NewRow();

	// New
	void					ConstructL(const TTiffImageData& aImageData);
	TTiffFaxState			Decode2d();
	TTiffFaxState			DecodeHorizontal();
	TTiffFaxState			DecodeVertical(TInt aDifference);
	TTiffFaxState			DecodePass();

protected:
	TInt*					iReferenceRunData;
	TInt					iReferenceRunDataIndex;
	TInt					iHorizontalCodeCount;
	TInt					iA0;
	};

// Group 3 fax, 1 dimentional decoder
class CTiffGroup3Fax1dDecoder : public CTiffFax1dDecoder
	{
public:
	static CTiffGroup3Fax1dDecoder* NewL(const TTiffImageData& aImageData);

protected:
	// From CTiffFax1dDecoder
	virtual TTiffFaxState	ProcessNextCode();
	};

// Group 3 fax, 2 dimentional decoder
class CTiffGroup3Fax2dDecoder : public CTiffFax2dDecoder
	{
public:
	static CTiffGroup3Fax2dDecoder* NewL(const TTiffImageData& aImageInfo);

	// From CTiffFax2dDecoder
	virtual void			NewStripL(TInt aNumBytes);
	virtual void			NewRow();

protected:
	virtual TTiffFaxState	ProcessNextCode();

private:
	enum TState
		{
		E1d,
		E2d,
		ESelect1d2d,
		};
	TState	iState;
	};

// Group 4 fax decoder
class CTiffGroup4FaxDecoder : public CTiffFax2dDecoder
	{
public:
	static CTiffGroup4FaxDecoder* NewL(const TTiffImageData& aImageData);

protected:
	// From CTiffFax1dDecoder
	virtual TTiffFaxState	ProcessNextCode();
	};

#endif

