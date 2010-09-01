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

inline 
void CAsyncTransformer::GetBitmap(CFbsBitmap*& aImage) const
	{
	aImage	= iState==EDone? iCurrentSource: NULL;
	}

inline 
CTransformerSharedData& CAsyncTransformer::Singleton()
	{
	return *iSingleton;
	}

inline
void CTransformerSharedData::SetTransformOptions(TUint aOptions)
	{
	iTransformOptions = aOptions;
	}

inline
void CTransformerSharedData::SetClipRect(const TRect& aClipRect)
	{
	iClippingSet= ETrue;
	iClipRect	= aClipRect;
	}

inline 
void CTransformerSharedData::SetTrueSrcSize(const TSize& aSize)
	{
	iTrueSrcSize = aSize;
	}

inline 
const TSize& CTransformerSharedData::TrueSrcSize() const
	{
	return iTrueSrcSize;
	}

inline
TUint CTransformerSharedData::TransformOptions() const
	{
	return iTransformOptions;
	}

inline 
TBool CTransformerSharedData::ClippingSet() const
	{
	return iClippingSet;
	}

inline const TRect& CTransformerSharedData::ClipRect() const
	{
	return iClipRect;
	}


