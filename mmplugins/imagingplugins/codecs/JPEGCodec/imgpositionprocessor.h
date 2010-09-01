// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalTechnology 
*/

#ifndef IMGPOSITIONPROCESSOR_H
#define IMGPOSITIONPROCESSOR_H

#include "JpegConsts.h"

const TInt KJpgImageCornerCount = 4;

const TUint8 KJpgTLCoord = 0x00;
const TUint8 KJpgTRCoord = 0x10;
const TUint8 KJpgBLCoord = 0x01;
const TUint8 KJpgBRCoord = 0x11;

/**
@internalComponent


// TPositionProcessor

This is a general class which has an outer PixelFrame subdivided into PixelBlocks. 
The class provides the ability to transform a PixelFrame and to navigate through that transformed PixelFrame.
Navigation of MCUs is achieved by stepping through PixelFrame, source rectangle image, at the resolution of the PixelBlock, MCU, size .
Navigation of pixels in DCT is achieved by stepping through PixelFrame, data unit, at the resolution of the PixelBlock, PixelBlocks is set to 1x1 so causing the navigation of individual pixels.
This class is not intended for public use.
*/

class TPositionProcessor
	{
public:
	TPositionProcessor();
	void SetDimensions(const TRect& aPixelFrame, const TSize& aPixelBlock);
	
	void RotateCoordinates();
	void HorFlipCoordinates();
	void VerFlipCoordinates();

	void MoveFirst();	
	void MoveNext();
	void SetEndOfImage(TBool aEndOfImage);
	
	inline void GetCurrentPosition(TPoint& aPos) const;
	inline TBool IsEndOfImage() const;
	inline TBool SwapDimensions() const;
	inline TBool IsTransformNeeded() const;

private:
	/**
	Flag used to determine the type of transformation which 
	is the result of single or multiple transformation operations executed on iImgCornerCoord.
	*/
	enum TFrameTransformOptions
		{
		/** Normal Encode
		*/
		ENormalEncode = 0x11011000,

		/** Rotate 90 degrees.
		*/
		EEncodeRotate90	= 0x01001110,

		/** Horizontal flip .
		*/
		EEncodeHorizontalFlip = 0x10001101,

		/** Vertical flip.
		*/
		EEncodeVerticalFlip	= 0x01110010,

		/** Rotate 180 degrees.
		*/
		EEncodeRotate180 = 0x00100111,

		/** Rotate 270 degrees.
		*/
		EEncodeRotate270 = 0x10110001,

		/** Horizontal flip and Rotate 90 degrees.
		*/
		EEncodeVerticalFlipRotate90 = 0x00011011,

		/** Vertical flip and Rotate 90 degrees.
		*/
		EEncodeHorizontalFlipRotate90 = 0x11100100,
		};

	TPoint iCurrentPosition;
	TRect iPixelFrame; //pixel frame
	TSize iPixelBlock; //pixel block
	TBool iIsEOF;
	TBool iSwapDimensions;
	
	union 
		{
		TUint32	iAlignWord[1];
		TUint8 iImgCornerCoord[KJpgImageCornerCount];
		};
	};

#include "imgpositionprocessor.inl"

#endif // __IMGPOSITIONPROCESSOR_H__

