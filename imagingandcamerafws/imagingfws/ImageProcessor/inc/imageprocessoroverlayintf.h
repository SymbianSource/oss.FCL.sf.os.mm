// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @publishedPartner 
 @released 
*/

#ifndef __IMAGE_PROCESSOR_OVERLAY_INF_H__
#define __IMAGE_PROCESSOR_OVERLAY_INF_H__

#include <e32base.h>

class CFbsBitmap;
class CImageFrame;
class TMMSource;
class RFile;

namespace ImageProcessor
	{
namespace Plugin
	{

/**
Plugin API for the overlay effects.

@see TOverlay
@publishedPartner
@released
*/	
class MOverlay
	{
public:
	/**
	This must provide an implementation as defined by TOverlay::SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats)
	
	@see TOverlay::SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats)
	*/	
	virtual void SupportedInputSubFormatsL(TUid aFormat, RArray<TUid>& aSubFormats) const = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	
	@see TOverlay::SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat)
	*/
	virtual void SetInputL(const TDesC& aFilename, const TUid& aFormat, const TUid& aSubFormat) = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	
	@see TOverlay::SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat)
	*/
	virtual void SetInputL(RFile& aFile, const TUid& aFormat, const TUid& aSubFormat) = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	
	@see TOverlay::SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat)
	*/
	virtual void SetInputL(TMMSource& aDrmFile, const TUid& aFormat, const TUid& aSubFormat) = 0;
		
	/**
	This must provide an implementation as defined by TOverlay::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	
	@see TOverlay::SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat)
	*/
	virtual void SetInputL(const TDesC8& aBuffer, const TUid& aFormat, const TUid& aSubFormat) = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask = NULL)
	
	@see TOverlay::SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask = NULL)
	*/
	virtual void SetInputL(const CFbsBitmap& aBitmap, const CFbsBitmap* aMask = NULL) = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SetInputL(const CImageFrame& aPixelBuffer)
	
	@see TOverlay::SetInputL(const CImageFrame& aPixelBuffer)
	*/
	virtual void SetInputL(const CImageFrame& aPixelBuffer) = 0;

	
	/**
	This must provide an implementation as defined by TOverlay::BeginL()
	
	@see TOverlay::BeginL()
	*/
	virtual void BeginL() = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SetL(const TSize &aSize, const TPoint &aPosition, TReal32 aAngle)
	
	@see TOverlay::SetL(const TSize &aSize, const TPoint &aPosition, TReal32 aAngle)
	*/
	virtual void SetL(const TSize &aSize, const TPoint &aPosition, TReal32 aAngle) = 0;

	/**
	This must provide an implementation as defined by TOverlay::SetL(TReal32 aScaleX, TReal32 aScaleY, const TPoint &aPosition, TReal32 aAngle)
	
	@see TOverlay::SetL(TReal32 aScaleX, TReal32 aScaleY, const TPoint &aPosition, TReal32 aAngle)
	*/
	virtual void SetL(TReal32 aScaleX, TReal32 aScaleY, const TPoint &aPosition, TReal32 aAngle) = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::EndL()
	
	@see TOverlay::EndL()
	*/
	virtual void EndL() = 0;

	/**
	This must provide an implementation as defined by TOverlay::ResetL()
	
	@see TOverlay::ResetL()
	*/
	virtual void ResetL() = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::PositionL()
	
	@see TOverlay::PositionL()
	*/
	virtual TPoint	PositionL() const = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::SizeL()
	
	@see TOverlay::SizeL()
	*/
	virtual TSize	SizeL() const = 0;
	
	/**
	This must provide an implementation as defined by TOverlay::AngleL()
	
	@see TOverlay::AngleL()
	*/
	virtual TReal32 AngleL() const = 0;

	/**
	This must provide an implementation as defined by TOverlay::GetScaleL(TReal32& aScaleX, TReal32& aScaleY)
	
	@see TOverlay::GetScaleL(TReal32& aScaleX, TReal32& aScaleY)
	*/
	virtual void GetScaleL(TReal32& aScaleX, TReal32& aScaleY) const = 0;
	
private:
	TInt iReserved; // future proof
	};

	}
	}

#endif //__IMAGE_PROCESSOR_OVERLAY_H__
