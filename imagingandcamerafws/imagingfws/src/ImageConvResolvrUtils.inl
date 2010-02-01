
// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

inline TUid COpaqueDataParse::PluginClassUid() const
	{
	return iPluginClassUid;
	}

inline TBool COpaqueDataParse::CompareUids(TUid aType, TUid aSubType) const
	{
	return ImageTypeUid()==aType && ImageSubTypeUid()==aSubType;
	}

inline TBool COpaqueDataParse::CompareUids(COpaqueDataParse* aParse) const
	{
	return CompareUids(aParse->ImageTypeUid(), aParse->ImageSubTypeUid());
	}

// true if only uids are available - ie neither properties nor extns nor mime types are provided
inline TBool COpaqueDataParse::OnlyUidsAvail() const
	{
	return iOpaqueData.Length()<=iStartOfNumberOfExtns;
	}

// true if this is an image "sub-type" and we should NOT include this 
// implementation when matching using the mime type or when listing all 
// available mime types
inline TBool COpaqueDataParse::IsSubCodec() const
	{
	return ((iFlags & ESubCodec)!=EFalse);
	}

// true if we need to open a decoder to be confident enough about 
// recognizing the header data. This can be set for codecs which have
// very few magic number bytes.
inline TBool COpaqueDataParse::IsOpenNeededToRecognize() const
	{
	return ((iFlags & EOpenNeededToRecognize)!=EFalse);
	}

// true if the plugin allow opening based on file extension
inline TBool COpaqueDataParse::IsOpenAgainstSuffix() const
	{
	return ((iFlags & EOpenAgainstSuffix)!=EFalse);
	}

// true if the plugin allow recognition on file extension
inline TBool COpaqueDataParse::IsRecognizeAgainstSuffix() const
	{
	return ((iFlags & ERecognizeAgainstSuffix)!=EFalse);
	}
	
inline TInt COpaqueDataParse::BinaryPropertiesCount() const
	{
	ASSERT(iState >= EBinPropertiesProcessed);
	return iBinaryPropertiesArray.Count();
	}
