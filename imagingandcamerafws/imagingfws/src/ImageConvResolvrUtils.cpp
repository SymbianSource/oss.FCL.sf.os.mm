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

#include "ImageConvResolvrUtils.h"
#include <icl/icl_propertyuids.h>

//
// COpaqueDataParse
//    Class for reading and processing opaque data from the ECOM resource file
//    Note that the opaque data can not exceed 255 bytes.
//    We store this as follows:
//		Version (1 byte)
// 		Optional flags (2 bytes) if Version >= 1
//		Main Uid (4 bytes)
//		Sub Uid (4 bytes)
//		Number extensions (1 byte)
//		{ ".XXX" \r } (x number extensions)
//		Number mime types (1 byte)
//		[ "mimetype" \r ] (for each mimetype)
//		Number of binary properties (1 byte)
//		List of the binary properties uids (4 bytes each)
//

// Note to avoid overhead, we only read the extensions, mime types and binary properties on demand
// Until EnsureExtnsReadL() is called, ExtnsCount() and Extn is invalid. Similarly
// for mime types and plugin binary properties.

COpaqueDataParse* COpaqueDataParse::NewL(const TDesC8& aOpaqueData)
	{
	COpaqueDataParse* result = COpaqueDataParse::NewLC(aOpaqueData);
	CleanupStack::Pop(result);
	return result;
	}

COpaqueDataParse* COpaqueDataParse::NewLC(const TDesC8& aOpaqueData)
	{
	COpaqueDataParse* result = new (ELeave) COpaqueDataParse(aOpaqueData);
	CleanupStack::PushL(result);
	result->ConstructL();
	return result;
	}

COpaqueDataParse::COpaqueDataParse(const TDesC8& aOpaqueData):
	iOpaqueData(aOpaqueData)
	{
	}

void COpaqueDataParse::ConstructL()
	{
	ReadVersion();
	if (iVersion > KIclPluginFrameworkVersionMax)
		{
		User::Leave(KErrNotSupported);
		}
	ReadFlags();
	ReadUids();
	}

COpaqueDataParse::~COpaqueDataParse()
	{
	iArray.Close();
	iBinaryPropertiesArray.Close();
	}

// Extract the Version number 
void COpaqueDataParse::ReadVersion()
	{
	ASSERT(iState<=EVersionProcessed); // should only call this on construction
	ASSERT(iOpaqueData.Length() >= KIndexForVersion + 1);
	//The version is one byte
	iVersion = iOpaqueData[KIndexForVersion];
	iState = EVersionProcessed;
	}


// Extract the (optional) flags
void COpaqueDataParse::ReadFlags()
	{
	ASSERT(iState<EFlagsProcessed);

	switch(iVersion)
		{
		case KIclPluginFrameworkVersionZero:
			iLenOfFlags = 0;
			iLenOfUids = 8;
			break;
		case KIclPluginFrameworkVersionOne:
			ASSERT(iOpaqueData.Length() >= KIndexForFlags + 2); // opaque data not valid otherwise
			iFlags = (iOpaqueData[KIndexForFlags+0] << 8) | (iOpaqueData[KIndexForFlags+1]);
			iLenOfFlags = 2;
			iLenOfUids = 8;
			break;
		case KIclPluginFrameworkVersionTwo:
			ASSERT(iOpaqueData.Length() >= KIndexForFlags + 2); // opaque data not valid otherwise
			iFlags = (iOpaqueData[KIndexForFlags+0] << 8) | (iOpaqueData[KIndexForFlags+1]);
			iLenOfFlags = 2;
			iLenOfUids = 12;			
			break;
		}
	
	iStartOfUids = 1 + iLenOfFlags;
	iStartOfNumberOfExtns = 1 + iLenOfFlags + iLenOfUids;
	
	iState = EFlagsProcessed;
	}

// Extract the two Uids 
void COpaqueDataParse::ReadUids()
	{
	ASSERT(iState<EUidsProcessed);
	ASSERT(iOpaqueData.Length() >= iStartOfUids + 4); // opaque data not valid otherwise
	// A given OpaqueData should either contain one Uid, in 4 bytes,
	// highest to lowest, with an implied second Uid set to KNullUid,
	// or two consecutive Uids, in 4 bytes each - 8 in all - main type followed by sub-type.
	// The 8 bytes may be followed, for basic types, by info on the file extensions
	// and MIME typesassociated with this plugin but this info is ignored here.
	iImageTypeUid.iUid = (iOpaqueData[iStartOfUids] << 24) | (iOpaqueData[iStartOfUids+1] << 16) | (iOpaqueData[iStartOfUids+2] << 8) | (iOpaqueData[iStartOfUids+3]);
	
	// In versions 0 and 1, we allow omission of the sub-type, but this is mandatory in 
	// versions 2 and greater
	if ( (iVersion < 2) && (iOpaqueData.Length() == iStartOfUids + 4) )
		{
		iImageSubTypeUid = KNullUid;
		}
	else 
		{
		ASSERT(iOpaqueData.Length() >= iStartOfUids + 8);
		iImageSubTypeUid.iUid = (iOpaqueData[iStartOfUids+4] << 24) | (iOpaqueData[iStartOfUids+5] << 16) + (iOpaqueData[iStartOfUids+6] << 8) + (iOpaqueData[iStartOfUids+7]);
		
	// In version 2 and greater, the plugin class Uid can be supplied
	    if (iVersion == 2) 
	    	{	    	
			if (iOpaqueData.Length() == iStartOfUids + 8)
				{
				iPluginClassUid = KNullUid;
				}
			else	
				{		
				ASSERT(iOpaqueData.Length() >= iStartOfUids + 12);
				iPluginClassUid.iUid = (iOpaqueData[iStartOfUids+8] << 24) | (iOpaqueData[iStartOfUids+9] << 16) + (iOpaqueData[iStartOfUids+10] << 8) + (iOpaqueData[iStartOfUids+11]);
				}
	    	}
		}
	iState = EUidsProcessed;
	}

// read in the Extns from the opaque data
void COpaqueDataParse::EnsureExtnsReadL()
	{
	// if we've already read in the extensions, is a NOP
	if (iState>=EExtnsProcessed)
		return;

	ASSERT(iArray.Count()==0); // assume nothing in there at the moment

	iExtnCount = iOpaqueData[iStartOfNumberOfExtns];
	TInt pos = iStartOfNumberOfExtns+1; // start just after the extn count byte	

	for (TInt index=0; index<iExtnCount; index++)
		{
		TPtrC8 restOfData = iOpaqueData.Mid(pos);
		TInt endPos = restOfData.Locate(TChar('\r')); // \r is used to terminate each suffix
		ASSERT(endPos>0); // if we have -1, would not have found the CR
		TPtrC8 extn = restOfData.Left(endPos);
		User::LeaveIfError(iArray.Append(extn));
		pos += endPos + 1; // skip over the CR
		}
	iStartOfMimeTypes = pos;
	iState = EExtnsProcessed;
	ASSERT(iArray.Count()==iExtnCount);
	}

void COpaqueDataParse::EnsureMIMETypesReadL()
	{
	// if we've already read in the mime types, is a NOP
	if (iState>=EMIMETypesProcessed)
		return;

	EnsureExtnsReadL();
	ASSERT(iState==EExtnsProcessed);

	TInt pos = iStartOfMimeTypes;
	const TInt numMimeTypes = iOpaqueData[pos];
	pos += 1;

	for (TInt index=0; index<numMimeTypes; index++)
		{
		TPtrC8 restOfData = iOpaqueData.Mid(pos);
		TInt endPos = restOfData.Locate(TChar('\r')); // \r is used to terminate each suffix
		ASSERT(endPos>0); // if we have -1, would not have found the CR
		TPtrC8 mimeType = restOfData.Left(endPos);
		User::LeaveIfError(iArray.Append(mimeType));
		pos += endPos + 1; // skip over the CR
		}
	iStartOfNumberOfBinProperties = pos;
	iState = EMIMETypesProcessed;
	ASSERT(iArray.Count()==iExtnCount+numMimeTypes);
	}

// Read in the binary properties from the opaque data.
void COpaqueDataParse::EnsureBinaryPropertiesReadL()
	{
	// If we've already read in the binary properties or if 
	if (iState >= EBinPropertiesProcessed || iVersion < KIclPluginFrameworkVersionTwo)
		{
		return;
		}
	
	EnsureMIMETypesReadL();
	ASSERT(iState == EMIMETypesProcessed);
	ASSERT(iBinaryPropertiesArray.Count() == 0);
	
	//If we have a field for the binary properties
	if(iStartOfNumberOfBinProperties < iOpaqueData.Length())
		{
		TInt binPropertiesCount = iOpaqueData[iStartOfNumberOfBinProperties];
		
		TInt pos = iStartOfNumberOfBinProperties + 1; // start just after the binary properties count byte	
		
		if(binPropertiesCount > 0)
			{
			// The lentgh of the binary properties field sould be a multiple of the size of TUid
			if(((iOpaqueData.Length() - pos )/binPropertiesCount) != sizeof(TUid))
				{
				User::Leave(KErrNotSupported);
				}
			}
		
		TUid tempUid;
		for (TInt index = 0; (index < binPropertiesCount) && ( (pos + sizeof(TUid)) <= iOpaqueData.Length()) ; index++)
			{
			// Real uids cannot have first byte in range 0x01 through 0x0f.
			if(0x01 <= iOpaqueData[pos] && iOpaqueData[pos] <= 0x0f)
				{
				User::Leave(KErrNotSupported);
				}
			tempUid.iUid = (iOpaqueData[pos] << 24) | (iOpaqueData[pos+1] << 16) | (iOpaqueData[pos+2] << 8) | (iOpaqueData[pos+3]);
			User::LeaveIfError(iBinaryPropertiesArray.Append(tempUid));
			pos = pos + sizeof(TUid);
			}
		
		iState = EBinPropertiesProcessed;
		ASSERT(iBinaryPropertiesArray.Count() == binPropertiesCount);
		}
	}

TBool COpaqueDataParse::CompareMIMETypes(const TDesC8& aMimeType1, const TDesC8& aMimeType2)
	{
	if (aMimeType1.Length() != aMimeType2.Length())
		return EFalse;
	return aMimeType1.CompareF(aMimeType2)==0; // true if exactly the same, if we ignore the case
	}

TBool COpaqueDataParse::CompareFileSuffixL(const TDesC& aFileSuffix1, const TDesC8& aFileSuffix2)
	{
	//First do a quick compare
	if (aFileSuffix1.Length() != aFileSuffix2.Length())
		return EFalse;

	//Change aFileSuffix2 from TDesC8 to TDesC
	HBufC* fileSuffix = HBufC::NewL(aFileSuffix2.Length()); //Not on CleanupStack since nothing can leave
	TPtr fileSuffixPtr(fileSuffix->Des());
	fileSuffixPtr.Copy(aFileSuffix2);

	const TBool result = aFileSuffix1.CompareF(*fileSuffix)==0; // true if exactly the same, if we ignore the case
	delete fileSuffix;

	return result;
	}

void COpaqueDataParse::GetBinaryPropertiesArrayL(RUidDataArray& aBinaryPropertiesArray) const
	{
	aBinaryPropertiesArray.Reset();
	for(TInt index = 0 ; index < iBinaryPropertiesArray.Count() ; index++)
		{
		User::LeaveIfError(aBinaryPropertiesArray.Append(iBinaryPropertiesArray[index]));
		}
	}

TBool COpaqueDataParse::CheckRequiredUids(const RUidDataArray& aRequiredUids)
	{
	//First do a quick compare: we can't require more uids than provided, 
	if(aRequiredUids.Count() > (iBinaryPropertiesArray.Count() + 3)) // 3 for the image-type, image-subtype and class plugin UIDs.
		{
		return EFalse;
		}
	
	for(TInt index = 0 ; index < aRequiredUids.Count() ; index++)
		{
		if(
			(iBinaryPropertiesArray.Find(aRequiredUids[index]) == KErrNotFound)
			&& ( aRequiredUids[index]!=iImageTypeUid )
			&& ( aRequiredUids[index]!=iImageSubTypeUid )
			&& ( aRequiredUids[index]!=iPluginClassUid )
			)
			{
			return EFalse;
			}			
		}

	return ETrue;
	}

TBool COpaqueDataParse::CheckOptionsUids(const RUidDataArray& aOptionsUids)
	{
	//First do a quick compare: we can't require more uids than provided, 
	if(aOptionsUids.Count() > (iBinaryPropertiesArray.Count() + 3)) // 3 for the image-type, image-subtype and class plugin UIDs.
		{
		return EFalse;
		}
	
	for(TInt index = 0 ; index < aOptionsUids.Count() ; index++)
		{
		if(iBinaryPropertiesArray.Find(aOptionsUids[index]) == KErrNotFound)
			{
			return EFalse;
			}			
		}

	return ETrue;
	}

TBool COpaqueDataParse::IsDefaultCodec() const
	{
	return (iBinaryPropertiesArray.Find(KUidDefaultCodec) != KErrNotFound);
	}


/*
 * Checks opaque data for the pair KUidICLExtension <extension_flags_modifier> where all
 * bits set in aRequiredExtensions are also set in <extension_flags_modifier>.
 * This signifies that the extension interfaces required are supported by the codec.
 * 
 * Note that there may be more than one KUidICLExtension pair set in the opaque data.
 * 
 * @return TBool
 *    ETrue indicates that the extensions are supported (interoperably)
 */
TBool COpaqueDataParse::SupportsExtensionsL(TUint aRequiredExtensions)
	{
	if (!aRequiredExtensions)
		{
		return ETrue;
		}
	
	EnsureBinaryPropertiesReadL();
	
	// Search through the iBinaryPropertiesArray looking for KUidICLExtension.
	// The next entry should be the flags.
	
	TInt count = iBinaryPropertiesArray.Count();
	for (TInt i = 0; i < (count - 1); i++)
		{
		TUid uid = iBinaryPropertiesArray[i];
		if (uid == KUidICLExtension)
			{
			// It's stored in the array as a TUid but is really a TUint.
			TUint codecCaps = static_cast<TUint>(iBinaryPropertiesArray[++i].iUid);
			if ((codecCaps & aRequiredExtensions) == aRequiredExtensions)
				{
				return ETrue;
				}
			}
		}
	
	return EFalse;
	}


/*
 * 
 * Searches for a match of aPluginString in aClientString. Match returns ETrue
 * if aPluginString is found within aImplementationType according to the
 * following rules:
 * 1) All bytes in aClientString are exact but instances of '?' (0x3F) in
 *    aPluginString are treated as wildcards - matching any byte value at that
 *    position in aClientString.
 * 2) If aClientString is longer than aPluginString, ignore any excess bytes.
 * 3) aClientString is treated as a single descriptor - the '||' delimiters
 *    are not recognised.
 * 4) ETrue is returned if aPluginString matches the start of aClientString
 *    exactly, subject to wildcards.
 * 5) aBytesMatched will be filled in with the number of non-wildcard bytes in
 *    aPluginString. This is used to rank matches.
 *
 * @param     "const TDesC8& aClientString"
 *            The string from the client to search for a match
 * @param     "const TDesC8& aPluginString"
 *            The data to search for from the plugin
 * @param     "TInt& aBytesMatched"
 *            The number of non-wildcard bytes matched
 * @return    "TBool"
 *            ETrue if a match was found, EFalse otherwise
 */
TBool CImageConversionResolverUtils::Match(const TDesC8& aClientString, const TDesC8& aPluginString, TInt& aBytesMatched)
	{
	TInt pluginStringLength = aPluginString.Length();
	while (pluginStringLength>0 && (aPluginString[pluginStringLength - 1] == '?'))
		{ // Clip extraneous '?' wildcards off the end of the plugin match string
		pluginStringLength--;
		}
	if (pluginStringLength == 0)
		{ // Plugin has no match string (or just '?')
		return EFalse;
		}
	if (aClientString.Length() == 0)
		{ // Client string too short - can't match
		return EFalse;
		}
	// Now let's run along the string
	aBytesMatched = 0;
	TUint matchLength = Min(pluginStringLength, aClientString.Length());
	for (TInt index = 0; index < matchLength; index++)
		{
		const TUint8 pluginChar = aPluginString[index];
		if (pluginChar=='?') // just ignore a wildchar
			continue;
		if (aClientString[index] != pluginChar)
			return EFalse;
		// else we've matched expected byte, so keep going
		aBytesMatched += 1;
		}
	return aBytesMatched != 0;
	}

