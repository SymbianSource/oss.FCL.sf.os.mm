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
// This defines the interface classes between the Image Conversion Library
// framework and its custom resolver
// 
//

#ifndef __IMAGERESOLVERAPI_H__
#define __IMAGERESOLVERAPI_H__

// ******************
// Not for public use
// ******************

#include <e32std.h>
#include <imageconversion.h>

const TInt KImageHeaderSize = 32; // This is a compromise. The longest match string we have
								  // yet found was 20 bytes (WMF Clp format) and we've left
								  // some spare in case of longer future formats. But we want
								  // to ensure that we don't exceed the possible minimum size
								  // of a complete image. We don't have to worry about WBMP
								  // and SMS OTA, two of the most compact formats, because
								  // they don't have image headers so won't go through the
								  // match string process.

const TInt KIclExtensionOptionsMask = 0x0000ff00;			// only 9th -> 16th bits are valid

enum TResolverMatchType
	{
	EMatchString,		// Resolver should match using plugin match strings
	EMatchUids,			// Resolver should match on image type and sub-type Uids
	EMatchMIMEType,		// Resolver should match on MIME type
	EListImageTypes,	// Resolver should return list of image base types or the sub-types of a particular base type
	EListFileTypes,		// Resolver should return list of file extensions and MIME types
	EMatchFileSuffix,	// Resolver should match using file extension
	EMatchReqUids		// Resolver should match using properties and UIDs
	};

class CCustomMatchData; // declared here
/*
 *
 * Used to pass parameters from the Image Conversion Library to the resolver
 *
 * Controls the identification, (resolution), of which implementation will be
 * used to satisfy an interface implementation instantiation. It is filled in
 * and then packaged up in a descriptor to be passed across to the custom
 * resolver in <code>TEComResolverParams</code>.
 *
 * @since 8.0
 */
NONSHARABLE_CLASS( CCustomMatchData ): public CBase
	{
// public methods
public:
	~CCustomMatchData();
	static CCustomMatchData* NewL();
	static CCustomMatchData* NewL(const TDesC8& aPackage);
	static CCustomMatchData* NewLC();
	static CCustomMatchData* NewLC(const TDesC8& aPackage);
	void ConstructL(const TDesC8& aPackage);
	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	void SetMatchStringL(const TDesC8& aMIMEType);
	void SetTypes(const TResolverMatchType& aMatchType, const TUid& aBaseType, const TUid& aSubType);
	void SetMatchType(const TResolverMatchType& aMatchType);
	void SetBaseType(const TUid& aBaseType);
	void SetSubType(const TUid& aSubType);
	void SetImplementationType(const TUid& aImplementationType);
	void SetFileSuffixL(const TDesC& aFileSuffix);
	TResolverMatchType MatchType() const;
	TUid BaseType() const;
	TUid SubType() const;
	TUid ImplementationType() const;
	const TPtrC8 MatchString() const;
	const TPtrC FileSuffix() const;
	void GetTypes(TResolverMatchType& aMatchType, TUid& aBaseType, TUid& aSubType) const;
	HBufC8* NewPackLC() const;
	void UnPackL(const TDesC8& aPackage);
	void SetMatchReqUidsL(const RUidDataArray& aMatchReqUidsArray);
	void GetMatchReqUidsL(RUidDataArray& aMatchReqUidsArray);
	void GetOptionsUidsL(RUidDataArray& aOptionsUidsArray);
	void SetExtensionOptions(TUint aExtensionBits);
	void SetOptions(CImageDecoder::TOptions aOptions);
	TUint ExtensionOptions();
	TUint Options();

// private methods
private:
	CCustomMatchData();

// private data
private:
	TResolverMatchType iMatchType;
	TUid iBaseType;
	TUid iSubType;
	TUid iImplementationType;
	HBufC8* iMatchString;
	HBufC* iFileSuffix;
	RUidDataArray iMatchReqUidsArray;
	TUint iExtensionOptions;
	RUidDataArray iOptionsUidsArray;
	TUint iOptions;
	};

#endif // __IMAGERESOLVERAPI_H__
