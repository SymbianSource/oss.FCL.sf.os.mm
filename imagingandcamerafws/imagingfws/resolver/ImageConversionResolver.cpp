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


#include <ecom/ecom.h>
#include <ecom/ecomerrorcodes.h>
#include <ecom/implementationinformation.h>

#include "ImageConversionResolver.h"
#include "ImageConvResolvrUtils.h"

_LIT(KIclResolverPanicCategory, "ImageConversionResolver");

//Current version of plugin framework.
//Plugin will be ignored if the version from opaque data doesn't
//match the current framework version.
//const TInt KPluginFrameworkVersion = 0; 

GLDEF_C void Panic(TInt aReason)
	{
	User::Panic(KIclResolverPanicCategory, aReason);
	}

/** 
 * Constructs and initialises a new instance of CImageConversionResolver.
 *
 * @param     "MPublicRegistry& aRegistry"
 *            A reference to the registry on which it will operate
 * @return    "CImageConversionResolver*"
 *            A pointer to the newly constructed resolver
 */
CImageConversionResolver* CImageConversionResolver::NewL(MPublicRegistry& aRegistry)
	{
	return new(ELeave) CImageConversionResolver(aRegistry);
	}

/** 
 *
 * Default destructor
 *
 */
CImageConversionResolver::~CImageConversionResolver()
	{
	}

/** 
 *
 * Default constructor
 *
 * @param     "MPublicRegistry& aRegistry"
 *            A reference to the registry on which it will operate
 */
CImageConversionResolver::CImageConversionResolver(MPublicRegistry& aRegistry)
	: CResolver(aRegistry)
	{
	}

/** 
 *
 * Request that the resolver identify the most appropriate interface
 * implementation.
 *
 * @param     "TUid aInterfaceUid"
 *            The Uid of the interface you want to match against
 * @param     "const TEComResolverParams& aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "TUid"
 *            The implementation Uid of the single best match found
 */
TUid CImageConversionResolver::IdentifyImplementationL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const
	{
	RImplInfoArray& implementationsInfo = iRegistry.ListImplementationsL(aInterfaceUid);
	TUid found = KNullUid;
	if (implementationsInfo.Count())
		found = ResolveL(implementationsInfo, aAdditionalParameters);

	return found;
	}

/** 
 *
 * Called by IdentifyImplementationL to select an implementation from the
 * supplied implementation uid, ignoring any implementations that have their 
 * Disabled flag set or are not of the current framework version.
 * The image type and sub type is verified to ensure the implementation is of
 * expected type.
 * 
 * @param     "const RImplInfoArray& aImplementationsInfo"
 *            The list of plugins you want to match against
 * @param     "const TEComResolverParams& aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "TUid"
 *            The implementation Uid of plugin
 */
TUid CImageConversionResolver::ResolveL(const RImplInfoArray& aImplementationsInfo, const TEComResolverParams& aAdditionalParameters) const
	{
	// Retrieve the match data from the descriptor
	CCustomMatchData* customMatch = CCustomMatchData::NewLC(aAdditionalParameters.DataType());

	TResolverMatchType matchType = customMatch->MatchType();
	TUint requiredExtensions = customMatch->ExtensionOptions();
	TUint requiredOptions = customMatch->Options();
	TUid baseType = customMatch->BaseType();
	TUid subType = customMatch->SubType();
	TUid implementationUid = customMatch->ImplementationType();

	RUidDataArray requiredOptionsUids;
	if (requiredOptions) 
		{
		CleanupClosePushL(requiredOptionsUids);
		customMatch->GetOptionsUidsL(requiredOptionsUids);
		}

	// Select plugin on implementation uid
	if((matchType != EMatchUids) || (implementationUid == KNullUid))
		User::Leave(KErrArgument);

	TUid bestMatch = KNullUid;
	TInt version = 0;
	TBool requiredOptionsFound = EFalse;

	// Loop through the implementations searching for the implementation uid
	const TInt count = aImplementationsInfo.Count();
	for(TInt index = 0; index < count; index++)
		{
		const CImplementationInformation& impData = *aImplementationsInfo[index];
		COpaqueDataParse* parse = NULL;
		TRAPD(error, parse = COpaqueDataParse::NewL(impData.OpaqueData()));
		if (error!=KErrNone)
			{
			if (error==KErrNotSupported)
				{
				// means that the resource entry was not valid
				continue;
				}
			else
				User::Leave(error);
			}

		CleanupStack::PushL(parse);
		
		// we can assume the version is valid as it is checked in 
		// COpaqueDataParse::ConstructL() which leaves if not valid
		ASSERT(parse->Version() <= KIclPluginFrameworkVersionMax);

		if (!impData.Disabled())
			{
			if (implementationUid == impData.ImplementationUid())
				{ // Got a match
				// Moved here to optimize iteration through implementations
				// Check if extension options are required.
				if (requiredExtensions)
					{
					if (!parse->SupportsExtensionsL(requiredExtensions))
						{
						CleanupStack::PopAndDestroy(parse);
						continue;
						}
					}

				if ((baseType == parse->ImageTypeUid()) && (subType == parse->ImageSubTypeUid()))
					{// ImageType and SubType match
					if (requiredOptions) 
						{
						parse->EnsureBinaryPropertiesReadL();
						if(parse->CheckOptionsUids(requiredOptionsUids))
							{
							if (!requiredOptionsFound) 
								{
								bestMatch = impData.ImplementationUid();
								version = impData.Version();
								requiredOptionsFound = ETrue;
								CleanupStack::PopAndDestroy(parse);
								continue;
								}
							}
						else 
							{
							if (requiredOptionsFound)
								{
								CleanupStack::PopAndDestroy(parse);
								continue;
								}
							}
						}
										
					if (version < impData.Version())
						{ // Higher version match
						bestMatch = impData.ImplementationUid();
						version = impData.Version();
						}
					}
				}
			}
		CleanupStack::PopAndDestroy(parse);
		}

	if (requiredOptions) 
		{
		CleanupStack::PopAndDestroy();//requiredOptionsUids
		}
	CleanupStack::PopAndDestroy(customMatch);

	return bestMatch;
	}

/* 
 *
 * List all the implementations which satisfy the specified interface, ignoring 
 * any implementations that have their Disabled flag set or are not of the 
 * current framework version.
 *
 * The list is sorted:
 * 1) For EMatchString in desending order of number of bytes matched and version number.
 * 2) For EMatchMIMEType in asending order of the MIME position that matched and
 *    desending version number.
 * 3) All others - desending version numbers.
 *
 * @param     "TUid aInterfaceUid"
 *            The Uid of the interface you want to match against
 * @param     "const TEComResolverParams& aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "RImplInfoArray*"
 *            The list of matches found
 */
 RImplInfoArray* CImageConversionResolver::ListAllL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const
	{
	// Retrieve the match data from the descriptor
	CCustomMatchData* customMatch = CCustomMatchData::NewLC(aAdditionalParameters.DataType());

	TResolverMatchType matchType = customMatch->MatchType();
	TUint requiredExtensions = customMatch->ExtensionOptions();
	TUint requiredOptions = customMatch->Options();
	TUid baseType = customMatch->BaseType();
	TUid subType = customMatch->SubType();
	TUid implementationUid = customMatch->ImplementationType();
	HBufC8* matchString = customMatch->MatchString().AllocLC();
	HBufC* fileSuffix = customMatch->FileSuffix().AllocLC();
	
	RUidDataArray requiredUids;
	CleanupClosePushL(requiredUids);
	customMatch->GetMatchReqUidsL(requiredUids);
	
	RUidDataArray requiredOptionsUids;
	CleanupClosePushL(requiredOptionsUids);
	customMatch->GetOptionsUidsL(requiredOptionsUids);
		
	// Use the member var to create the array so that we get proper cleanup behaviour
	RImplInfoArray& fullList = iRegistry.ListImplementationsL(aInterfaceUid);

	RArray<TSortImplInfo> sortList;
	CleanupClosePushL(sortList);

	TInt matchLevel = 0;
	TInt optionsMatchLevel = 0;

	// Loop through the implementations matching on appropriate info
	const TInt count = fullList.Count();
	TInt index;

	for(index = 0; index < count; index++)
		{
		const CImplementationInformation& impData = *(fullList[index]);
		COpaqueDataParse* parse = NULL;
		TRAPD(error, parse = COpaqueDataParse::NewL(impData.OpaqueData()));
		if (error!=KErrNone)
			{
			if (error==KErrNotSupported)
				{
				// means that the resource entry was not valid
				continue;
				}
			else
				User::Leave(error);
			}

		CleanupStack::PushL(parse);
		
		// we can assume the version is valid as it is checked in 
		// COpaqueDataParse::ConstructL() which leaves if not valid
		ASSERT(parse->Version() <= KIclPluginFrameworkVersionMax);
		
		if (!impData.Disabled())
			{
			
			TBool matchFound = EFalse;
			switch(matchType)
				{
				case EListImageTypes:
					{
					if (baseType == KNullUid)
						{ // Looking for base image types
						if (parse->ImageSubTypeUid() == KNullUid)
							matchFound = ETrue;
						}
					else
						{ // Looking for sub-types of a particular base image type
						if ((baseType == parse->ImageTypeUid()) && (parse->ImageSubTypeUid() != KNullUid))
							matchFound = ETrue;
						}
					break;
					}

				case EListFileTypes:
					{ // Looking for entries with file extensions and/or MIME types
					if(!parse->OnlyUidsAvail() && !parse->IsSubCodec())
						matchFound = ETrue;
					break;
					}

				case EMatchMIMEType:
					{
					if(!parse->OnlyUidsAvail() && !parse->IsSubCodec())
						{ //If codec has no MIME types ignore it
						parse->EnsureMIMETypesReadL();
						const TInt numMimeTypes = parse->MIMETypesCount();
						for (TInt index2 = 0; index2 < numMimeTypes; index2++)
							{
							const TDesC8& mimeType = parse->MIMEType(index2);
							if (COpaqueDataParse::CompareMIMETypes(*matchString, mimeType))
								{
								matchFound = ETrue;
								matchLevel = index2;
								break;
								}
							}
						}

					break;
					}

				case EMatchString:
					{ // Match the match strings
					if (CImageConversionResolverUtils::Match(*matchString, impData.DataType(), matchLevel))
						matchFound = ETrue;
					break;
					}

				case EMatchUids:
					{ // match on UIDs
					if (implementationUid != KNullUid)
						{// We're matching on codec implementation uid
						if (implementationUid == impData.ImplementationUid())
							{
							matchFound = ETrue;
							}
						else
							{
							matchFound = (implementationUid == parse->PluginClassUid());
							}
						}
					else
						{ // We're matching on image type (and sub-type)
						if (parse->CompareUids(baseType, subType))
							matchFound = ETrue;
						}
					break;
					}

				case EMatchFileSuffix:
					{
					if(!parse->OnlyUidsAvail() && parse->IsOpenAgainstSuffix())
						{ //If codec has file extensions and allow matching on them
						parse->EnsureExtnsReadL();
						const TInt numExtns = parse->ExtnsCount();
						for (TInt index2 = 0; index2 < numExtns; index2++)
							{
							const TDesC8& extn = parse->Extn(index2);
							if (COpaqueDataParse::CompareFileSuffixL(*fileSuffix, extn))
								{
								matchFound = ETrue;
								matchLevel = index2;
								break;
								}
							}
						}

					break;
					}
					
				case EMatchReqUids:
					{
					if(!parse->OnlyUidsAvail())
						{
						parse->EnsureBinaryPropertiesReadL();
						if(parse->CheckRequiredUids(requiredUids))
							{
							matchFound = ETrue;
							if(parse->IsDefaultCodec())
								{
								matchLevel = 1;
								}
							else
								{
								matchLevel = 0;
								}
							}
						}					
					break;
					}

				default:
					{//unknown match type
					Panic(KErrArgument);
					}
				}
			
			if (matchFound)
				{
				// Moved here to reduce timing required to list all codecs plugins.
				// Check if certain extensions are required.
				if (requiredExtensions)
					{
					if (!parse->SupportsExtensionsL(requiredExtensions))
						{
						CleanupStack::PopAndDestroy(parse);
						continue;
						}
					}
				
				// Check if certain options are required
				if(requiredOptions)
					{
					optionsMatchLevel = 0;
					parse->EnsureBinaryPropertiesReadL();
					if(parse->CheckOptionsUids(requiredOptionsUids))
						{
						optionsMatchLevel = 1;// one for having required options
						if(parse->IsDefaultCodec())
							{
							optionsMatchLevel = 2; // two for being default codec and having required options
							}
						}
					}
				
				TSortImplInfo sortInfo(fullList[index],matchLevel,optionsMatchLevel);
				User::LeaveIfError(sortList.Append(sortInfo));
				}
			}
		CleanupStack::PopAndDestroy(parse);
		}

	TLinearOrder<TSortImplInfo>* sortFunction = NULL;
	if(matchType==EMatchMIMEType)
		sortFunction = new (ELeave) TLinearOrder<TSortImplInfo>(SortAsending);
	else
		sortFunction = new (ELeave) TLinearOrder<TSortImplInfo>(SortDesending);

	sortList.Sort(*sortFunction);
	delete sortFunction;

	RImplInfoArray* retList = new (ELeave) RImplInfoArray;
	CleanupStack::PushL(retList);
	// coverity[double_push]
	CleanupClosePushL(*retList); // note the double PushL - will Close and delete on PopAndDestroy(2)

	TInt noEntries = sortList.Count();
	for(index = 0; index < noEntries; index++)
		User::LeaveIfError(retList->Append(sortList[index].iImplInfo));
		
	if ((matchType == EMatchString) && (noEntries > 1))
		{
		// check that there is no ambiguity due to a short match string
		TBool matchStringIsTooShort = EFalse;
		for (TInt i = 0; i < noEntries; i++)
			{
			const CImplementationInformation& implementationInfo = *((*retList)[i]);
			TInt implementationStringLength = implementationInfo.DataType().Length();
			while ((implementationStringLength > 0) && (implementationInfo.DataType()[implementationStringLength - 1] == '?'))
				{ // Clip extraneous '?' wildcards off the end of the plugin match string
				implementationStringLength--;
				}
			if (matchString->Length() < implementationStringLength)
				{
				matchStringIsTooShort = ETrue;
				break;
				}
			}

			if (matchStringIsTooShort)
				{
				User::Leave(KErrUnderflow);
				}
			}



	CleanupStack::Pop(2, retList); // retList x2

	CleanupStack::PopAndDestroy(6, customMatch); // sortList, requiredOptionsUids, requiredUids, fileSuffix, matchString, customMatch

	return retList;
	}

/**
 * Function to sort an array of TSortImplInfo in asending order of the match level, options match level and
 * then in desending order of version numbers.
 *
 * @param	"const TSortImplInfo& aImpInfo1"
 *			First element.
 * @param	"const TSortImplInfo& aImpInfo2"
 *			Second element.
 * @return	"TInt"
 *			Indication of element swapping order.
 */
TInt CImageConversionResolver::SortAsending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2)
	{
	TInt result = aImpInfo1.iMatchLevel - aImpInfo2.iMatchLevel;

	if(result == 0)
		result = aImpInfo1.iOptionsMatchLevel - aImpInfo2.iOptionsMatchLevel;

	if(result == 0)
		result = aImpInfo2.iImplInfo->Version() - aImpInfo1.iImplInfo->Version();

	return result;
	}

/**
 * Function to sort an array of TSortImplInfo in desending order of the match level, options match level 
 * and version numbers.
 *
 * @param	"const TSortImplInfo& aImpInfo1"
 *			First element.
 * @param	"const TSortImplInfo& aImpInfo2"
 *			Second element.
 * @return	"TInt"
 *			Indication of element swapping order.
 */
TInt CImageConversionResolver::SortDesending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2)
	{
	TInt result = aImpInfo2.iMatchLevel - aImpInfo1.iMatchLevel;

	if(result == 0)
		result = aImpInfo2.iOptionsMatchLevel - aImpInfo1.iOptionsMatchLevel;
	
	if(result == 0)
		result = aImpInfo2.iImplInfo->Version() - aImpInfo1.iImplInfo->Version();

	return result;
	}

/**
 * Construtor for the TSortImplInfo class
 *
 * @param	"CImplementationInformation *const aImplInfo"
 *			A implementation information element to be sorted.
 * @param	"TInt aMatchLevel"
 *			The matching level of the entry
 */
TSortImplInfo::TSortImplInfo(CImplementationInformation *const aImplInfo, TInt aMatchLevel):
	iImplInfo(aImplInfo),
	iMatchLevel(aMatchLevel),
	iOptionsMatchLevel(0)	
	{
	}

/**
 * Construtor for the TSortImplInfo class
 *
 * @param	"CImplementationInformation *const aImplInfo"
 *			A implementation information element to be sorted.
 * @param	"TInt aMatchLevel"
 *			The matching level of the entry
 * @param	"TInt aOptionsMatchLevel"
 *			The options matching level of the entry
 */
TSortImplInfo::TSortImplInfo(CImplementationInformation *const aImplInfo, TInt aMatchLevel, TInt aOptionsMatchLevel):
	iImplInfo(aImplInfo),
	iMatchLevel(aMatchLevel),
	iOptionsMatchLevel(aOptionsMatchLevel)
	{
	}

