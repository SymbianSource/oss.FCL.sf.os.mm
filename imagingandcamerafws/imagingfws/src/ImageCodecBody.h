// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// - Bodies of classes declared in ImageCodec.h
// 
//

#ifndef __ImageCodecBody_h
#define __ImageCodecBody_h

class CImageProcessorReadCodec;

class CImageProcessorReadCodecBody; // declared here
/**
 *
 * This class is not for public use
 *
 */

NONSHARABLE_CLASS( CImageProcessorReadCodecBody ): public CBase
	{
	friend class CImageProcessorReadCodec;
public:
	~CImageProcessorReadCodecBody();
protected:
	static CImageProcessorReadCodecBody* NewL();

	inline CImageProcessor* ImageProcessor() const;
	void SetImageProcessor(CImageProcessor* aImageProc);

	inline const TPoint& Pos() const;
	inline TPoint& Pos();
	inline void SetPos(const TPoint& aPos);
private:
	CImageProcessor* iImageProc;
	TPoint iPos;
	};

class CImageMaskProcessorReadCodecBody; // declared here
/**
 *
 * This class is not for public use
 *
 */

NONSHARABLE_CLASS( CImageMaskProcessorReadCodecBody ): public CBase
	{
	friend class CImageMaskProcessorReadCodec;
public:
	~CImageMaskProcessorReadCodecBody();
protected:
	static CImageMaskProcessorReadCodecBody* NewL();

	inline CImageProcessor* MaskProcessor() const;
	void SetMaskProcessor(CImageProcessor* aImageProc);

private:
	CImageProcessor* iMaskProc;
	};

#include "ImageCodecBody.inl"

#endif  // __ImageCodecBody_h

