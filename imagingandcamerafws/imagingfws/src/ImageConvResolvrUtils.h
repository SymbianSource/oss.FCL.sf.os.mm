// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ImageConvResolvrUtils_h
#define __ImageConvResolvrUtils_h

#include <e32base.h>

#include "ImageConversion.h"

const TInt KIclPluginFrameworkVersionZero = 0;		// no flags
const TInt KIclPluginFrameworkVersionOne = 1;		// with flags
const TInt KIclPluginFrameworkVersionTwo = 2;		// with flags
const TInt KIclPluginFrameworkVersionMax = 2;		// max "legal" version

const TInt KIndexForVersion = 0; // index in opaque data for the version
const TInt KIndexForFlags = 1; // index for flags in opaque data

class COpaqueDataParse; // declared here
NONSHARABLE_CLASS( COpaqueDataParse ): public CBase
	{
public:
	static COpaqueDataParse* NewL(const TDesC8& aOpaqueData);
	static COpaqueDataParse* NewLC(const TDesC8& aOpaqueData);
	~COpaqueDataParse();
	TBool OnlyUidsAvail() const;
	TBool IsSubCodec() const;
    TBool IsOpenNeededToRecognize() const;
	TBool IsOpenAgainstSuffix() const;
	TBool IsRecognizeAgainstSuffix() const;	
	void EnsureExtnsReadL();
	void EnsureMIMETypesReadL();
	void EnsureBinaryPropertiesReadL();
	TInt BinaryPropertiesCount() const;
	TInt ExtnsCount() const;
	TInt MIMETypesCount() const;
	void GetBinaryPropertiesArrayL(RUidDataArray& aBinaryPropertiesArray) const;
	const TDesC8& Extn (TInt aIndex) const;
	const TDesC8& MIMEType (TInt aIndex) const;
	TInt Version() const;
	TInt Flags() const;
	TUid ImageTypeUid() const;
	TUid ImageSubTypeUid() const;
	TUid PluginClassUid() const;
	TBool CompareUids(COpaqueDataParse* aParse) const;
	TBool CompareUids(TUid aType, TUid aSubType = KNullUid) const;
	static TBool CompareMIMETypes(const TDesC8& aMimeType1, const TDesC8& aMimeType2);
	static TBool CompareFileSuffixL(const TDesC& aFileSuffix1, const TDesC8& aFileSuffix2);
	TBool CheckRequiredUids(const RUidDataArray& aRequiredUids);
	TBool CheckOptionsUids(const RUidDataArray& aOptionsUids);
	TBool IsDefaultCodec() const;
	TBool SupportsExtensionsL(TUint aRequiredExtensions);

protected:
	COpaqueDataParse(const TDesC8& aOpaqueData);
	void ConstructL();
	void ReadVersion();
	void ReadFlags();
	void ReadUids();
	
private:
	enum TProcessState
		{
		EVersionProcessed,
		EFlagsProcessed,
		EUidsProcessed,		
		EExtnsProcessed,
		EMIMETypesProcessed,
		EBinPropertiesProcessed
		};
		
	enum TPluginFlags
		{
		ESubCodec               = 0x00000001,
		EOpenNeededToRecognize  = 0x00000002,
		EOpenAgainstSuffix		= 0x00000004,
		ERecognizeAgainstSuffix	= 0x00000008
		};

	const TDesC8& iOpaqueData;
	TProcessState iState;
	TInt iVersion;
	TInt iFlags;
	TUid iImageTypeUid;
	TUid iImageSubTypeUid;
	TUid iPluginClassUid;
	TInt iExtnCount;

	TInt iLenOfFlags;				// len of flag field (0 or 2)
	TInt iLenOfUids;				// len of UIDs field
	TInt iStartOfUids;				// index for uids in opaque data	
	TInt iStartOfNumberOfExtns;		// index in Opaque data for the number of extensions
	TInt iStartOfMimeTypes;
	TInt iStartOfNumberOfBinProperties;	// Index in opaque data for the number of binary properties
	
	RArray<TPtrC8> iArray;
	RUidDataArray iBinaryPropertiesArray;
	};

// Utility class used in ImageConversionResolver.dll
// and ImageConversion.dll.
// Searches for a match of aClientString in aPluginString
// - aPluginString is normally from CImplementationInformation::DataType()
class CImageConversionResolverUtils : public CBase
	{
public:
	static TBool Match(const TDesC8& aClientString, const TDesC8& aPluginString, TInt& aBytesMatched);
	};

#include "ImageConvResolvrUtils.inl"

#endif  // __ImageConvResolvrUtils_h
