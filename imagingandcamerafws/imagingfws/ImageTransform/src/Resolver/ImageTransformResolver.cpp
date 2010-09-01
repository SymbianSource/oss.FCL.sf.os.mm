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


#include <ecom/ecom.h>
#include <ecom/ecomerrorcodes.h>
#include <ecom/implementationinformation.h>

#include "ImageTransformResolver.h"
#include "ImageTransformResolvrUtils.h"

_LIT(KIclResolverPanicCategory, "ImageTransformResolver");

// To save some space, we don't support IdentifyImplementationL()

GLDEF_C void Panic(TInt aReason)
	{
	User::Panic(KIclResolverPanicCategory, aReason);
	}

/** 
 * Constructs and initialises a new instance of CImageTransformResolver.
 *
 * @param     "aRegistry"
 *            A reference to the registry on which it will operate
 * @return    "CImageTransformResolver*"
 *            A pointer to the newly constructed resolver
 */
CImageTransformResolver* CImageTransformResolver::NewL(MPublicRegistry& aRegistry)
	{
	return new(ELeave) CImageTransformResolver(aRegistry);
	}

/** 
 *
 * Default destructor
 *
 */
CImageTransformResolver::~CImageTransformResolver()
	{
	}

/** 
 *
 * Default constructor
 *
 * @param     "aRegistry"
 *            A reference to the registry on which it will operate
 */
CImageTransformResolver::CImageTransformResolver(MPublicRegistry& aRegistry)
	: CResolver(aRegistry)
	{
	}

/** 
 *
 * Request that the resolver identify the most appropriate interface
 * implementation.
 *
 * @param     "aInterfaceUid"
 *            The Uid of the interface you want to match against
 * @param     "aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "TUid"
 *            The implementation Uid of the single best match found
 */
TUid CImageTransformResolver::IdentifyImplementationL(TUid /*aInterfaceUid*/, const TEComResolverParams& /*aAdditionalParameters*/) const
	{
	// To save some space, we don't support IdentifyImplementationL()
	return KNullUid;
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
 * @param     "aInterfaceUid"
 *            The Uid of the interface you want to match against
 * @param     "aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "RImplInfoArray*"
 *            The list of matches found
 */
 RImplInfoArray* CImageTransformResolver::ListAllL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const
	{

	// Retrieve the match data from the descriptor
	CCustomMatchData* customMatch = CCustomMatchData::NewLC(aAdditionalParameters.DataType());

	TResolverMatchType matchType = customMatch->MatchType();
	// get the flags that the plugin must support
	TUint pluginFlagsNeeded = customMatch->PluginFlagsNeeded();

	TUid baseType = customMatch->BaseType();
	TUid subType = customMatch->SubType();
	TUid implementationUid = customMatch->ImplementationType();
	HBufC8* matchString = customMatch->MatchString().AllocLC();
	HBufC* fileSuffix = customMatch->FileSuffix().AllocLC();

	// Use the member var to create the array so that we get proper cleanup behaviour
	RImplInfoArray& fullList = iRegistry.ListImplementationsL(aInterfaceUid);

	RArray<TSortImplInfo> sortList;
	CleanupClosePushL(sortList);

	TInt matchLevel = 0;

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
		
		// get the plugin's flags
		TBool flagsSupported = ETrue;

		if( flagsSupported && (parse->Version() == 1) ) 
			{
			if((pluginFlagsNeeded & ESetSqueezeSupportNeeded) ||
				(pluginFlagsNeeded & ESetOrientationSupportNeeded)||
				(pluginFlagsNeeded & ESetOverlaySupportNeeded)||
				(pluginFlagsNeeded & ESetExifSupportNeeded))
				{
				// Squeeze/Rotate/Overlay/Exif not supported in version 1
				flagsSupported = EFalse;
				}
			}
		
		if (flagsSupported && (pluginFlagsNeeded & ESetScaleSupportNeeded))
			{
			// For version 1 plugins it is always supported. 
			// For version 2, do the check
			if( (parse->Version() == 2) && !parse->IsSetScaleSupported())
				{
				flagsSupported = EFalse;
				}
			}

		if (flagsSupported && (pluginFlagsNeeded & ESetSourceRectSupportNeeded) &&
			!parse->IsSetSourceRectSupported())
			{
			// Supported in both the versions
			flagsSupported = EFalse;
			}

		// Check if squeeze is required and supported
		if(flagsSupported && (pluginFlagsNeeded & ESetSqueezeSupportNeeded) &&
				!parse->IsSetSqueezeSupported())
			{
			flagsSupported = EFalse;		
			}

		// Check if orientations is required and supported
		if(flagsSupported && (pluginFlagsNeeded & ESetOrientationSupportNeeded) &&
				!parse->IsSetOrientationSupported())
			{
			flagsSupported = EFalse;		
			}

		// Check if overlay is required and supported
		if(flagsSupported && (pluginFlagsNeeded & ESetOverlaySupportNeeded) &&
				!parse->IsSetOverlaySupported())
			{
			flagsSupported = EFalse;		
			}
		
		// Check if exif is required and supported
		if(flagsSupported && (pluginFlagsNeeded & ESetExifSupportNeeded) &&
				!parse->IsSetExifSupported())
			{
			flagsSupported = EFalse;		
			}

		if (!impData.Disabled() && flagsSupported)
			{
			TBool matchFound = EFalse;
			switch(matchType)
				{
				case EMatchMIMEType:
					{
					if(!parse->OnlyUidsAvail())
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
					if (CImageTransformResolverUtils::Match(*matchString, impData.DataType(), matchLevel))
						matchFound = ETrue;
					break;
					}

				case EMatchUids:
					{ // match on UIDs
					if (implementationUid != KNullUid)
						{// We're matching on codec implementation uid
						if (implementationUid == impData.ImplementationUid())
							matchFound = ETrue;
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

				default:
					{//unknown match type
					Panic(KErrArgument);
					}
				}
			if (matchFound)
				{
				TSortImplInfo sortInfo(fullList[index],matchLevel);
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

	CleanupStack::Pop(2, retList); // retList x2

	CleanupStack::PopAndDestroy(4, customMatch); //sortList, fileSuffix, matchString, customMatch

	return retList;
	}

/**
 * Function to sort an array of TSortImplInfo in asending order of the match level and
 * then in desending order of version numbers.
 *
 * @param	"aImpInfo1"
 *			First element.
 * @param	"aImpInfo2"
 *			Second element.
 * @return	"TInt"
 *			Indication of element swapping order.
 */
TInt CImageTransformResolver::SortAsending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2)
	{
	TInt result = aImpInfo1.iMatchLevel - aImpInfo2.iMatchLevel;

	if(result == 0)
		result = aImpInfo2.iImplInfo->Version() - aImpInfo1.iImplInfo->Version();

	return result;
	}

/**
 * Function to sort an array of TSortImplInfo in desending order of the match level 
 * and version numbers.
 *
 * @param	"aImpInfo1"
 *			First element.
 * @param	"aImpInfo2"
 *			Second element.
 * @return	"TInt"
 *			Indication of element swapping order.
 */
TInt CImageTransformResolver::SortDesending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2)
	{
	TInt result = aImpInfo2.iMatchLevel - aImpInfo1.iMatchLevel;

	if(result == 0)
		result = aImpInfo2.iImplInfo->Version() - aImpInfo1.iImplInfo->Version();

	return result;
	}

/**
 * Construtor for the TSortImplInfo class
 *
 * @param	"aImplInfo"
 *			A implementation information element to be soreted.
 * @param	"aMatchLevel"
 *			The matching level of the entry
 */
TSortImplInfo::TSortImplInfo(CImplementationInformation *const aImplInfo, TInt aMatchLevel):
	iImplInfo(aImplInfo),
	iMatchLevel(aMatchLevel)
	{
	}

