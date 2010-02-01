// CDecoderExifTag.inl
//    inline implementation for the CDecoderExifTag class.

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

#ifndef __CDecoderExifTag_INL__
#define __CDecoderExifTag_INL__

// CDecoderExifTag

// Set tag value
inline void CDecoderExifTag::SetTag(TUint aTag)
	{
	iTag = aTag;
	}
	
// Set format, of which there are ten types, each with a defined size
inline void CDecoderExifTag::SetFormat(TUint aFormat)
	{
	iFormat = aFormat;
	}
	
// Set the number of components in an entry (i.e. a single integer would
// be 1, whereas the NULL terminated string 'Camera' would be 6.
inline void CDecoderExifTag::SetComponentCount(TUint aComponentCount)
	{
	iComponentCount = aComponentCount;
	}
	
// Set the value/offset segment
inline void CDecoderExifTag::SetValueOffset(TUint aValueOffset)
	{
	iValueOffset = aValueOffset;
	}

inline void CDecoderExifTag::SetNumer(TInt aNumer)
	{
	iNumer = aNumer;
	}

inline void CDecoderExifTag::SetDenom(TInt aDenom)
	{
	iDenom = aDenom;
	}

inline const TUint CDecoderExifTag::Tag()
	{
	return iTag;
	}

inline const TUint CDecoderExifTag::Format()
	{
	return iFormat;
	}

inline const TUint CDecoderExifTag::ComponentCount()
	{
	return iComponentCount;
	}

inline const TUint CDecoderExifTag::ValueOffset()
	{
	return iValueOffset;
	}

inline const TInt CDecoderExifTag::Numer()
	{
	return iNumer;
	}
	
inline const TInt CDecoderExifTag::Denom()
	{
	return iDenom;
	}

#endif  // __CDecoderExifTag_INL__



