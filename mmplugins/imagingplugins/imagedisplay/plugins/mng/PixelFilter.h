// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PixelFilter_H__
#define __PixelFilter_H__

/** 	@file
	@internalTechnology */

class MRefCounted 
	{
public:
	virtual void AddRef()=0;
	virtual void Release()=0;
	};

class CRgbPixelConsumer;

class MPixelFilter : public MRefCounted 
	{
public:

	virtual void PrepareL(const TSize& aOriginalSize,CRgbPixelConsumer* aConsumer) = 0;
	virtual void Commit() = 0;

	virtual void SetPixel(const TPoint& aPosition, TUint aPixelColour) = 0;
	};


#endif // ndef __PixelFilter_H__
