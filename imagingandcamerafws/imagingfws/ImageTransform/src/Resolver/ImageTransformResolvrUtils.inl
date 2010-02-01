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

inline TInt COpaqueDataParse::ExtnsCount() const
	{
	ASSERT(iState >= EExtnsProcessed);
	return iExtnCount;
	}

inline TInt COpaqueDataParse::MIMETypesCount() const
	{
	ASSERT(iState >= EMIMETypesProcessed);
	return iArray.Count() - ExtnsCount();
	}

inline const TDesC8& COpaqueDataParse::Extn(TInt aIndex) const
	{
	ASSERT(aIndex < ExtnsCount());
	return iArray[aIndex];
	}

inline const TDesC8& COpaqueDataParse::MIMEType(TInt aIndex) const
	{
	ASSERT(aIndex < MIMETypesCount());
	return iArray[aIndex+ExtnsCount()];
	}

inline TInt COpaqueDataParse::Version() const
	{
	return iVersion;
	}

inline TInt COpaqueDataParse::Flags() const
	{
	return iFlags;
	}

inline TUid COpaqueDataParse::ImageTypeUid() const
	{
	return iImageTypeUid;
	}

inline TUid COpaqueDataParse::ImageSubTypeUid() const
	{
	return iImageSubTypeUid;
	}

inline TBool COpaqueDataParse::CompareUids(TUid aType, TUid aSubType) const
	{
	return ImageTypeUid()==aType && ImageSubTypeUid()==aSubType;
	}

inline TBool COpaqueDataParse::CompareUids(COpaqueDataParse* aParse) const
	{
	return CompareUids(aParse->ImageTypeUid(), aParse->ImageSubTypeUid());
	}

// true if only uids are available - ie neither extns nor mime types are provided
inline TBool COpaqueDataParse::OnlyUidsAvail() const
	{
	return iOpaqueData.Length()<=iStartOfNumberOfExtns;
	}

// true if the plugin allows opening based on file extension
inline TBool COpaqueDataParse::IsOpenAgainstSuffix() const
	{
	return ((iFlags & EOpenAgainstSuffix)!=EFalse);
	}

// true if the plugin supports SetSourceRect()
inline TBool COpaqueDataParse::IsSetSourceRectSupported() const
	{
	return ((iFlags & ESetSourceRectSupported)!=EFalse);
	}

// true if the plugin supports scaling
inline TBool COpaqueDataParse::IsSetScaleSupported() const
	{
	return ((iFlags & ESetScaleSupported)!=EFalse);
	}

// true if the plugin supports Squeeze Transform
inline TBool COpaqueDataParse::IsSetSqueezeSupported() const
	{
	return ((iFlags & ESetSqueezeSupported)!=EFalse);
	}

// true if the plugin supports changing image orientation
inline TBool COpaqueDataParse::IsSetOrientationSupported() const
	{
	return ((iFlags & ESetOrientationSupported)!=EFalse);
	}

// true if the plugin supports Overlay Transform
inline TBool COpaqueDataParse::IsSetOverlaySupported() const
	{
	return ((iFlags & ESetOverlaySupported)!=EFalse);
	}

// true if the plugin supports Exif extensions
inline TBool COpaqueDataParse::IsSetExifSupported() const
	{
	return ((iFlags & ESetExifSupported)!=EFalse);
	}

