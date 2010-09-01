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


/**
 @file
 @internalComponent
 */

#include "omxilimageport.h"
#include "omxilutil.h"
#include "log.h"

EXPORT_C
COmxILImagePort::COmxILImagePort(
		const TOmxILCommonPortData& aCommonPortData,
		const RArray<OMX_IMAGE_CODINGTYPE>& aSupportedImageFormats,
		const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats)
:COmxILPort(aCommonPortData),
//[YYC]: bitwise copy is unsafe, there should be a protected ConstructL or alike for array deep copy
iSupportedImageFormats(aSupportedImageFormats),
iSupportedColorFormats(aSupportedColorFormats)
	{
	DEBUG_PRINTF(_L8("COmxILImagePort::COmxILImagePort"));
	
	TInt numImageFormats = iSupportedImageFormats.Count();
	TInt numColorFormats = iSupportedColorFormats.Count();
	iParamImagePortFormat.nSize = sizeof(OMX_IMAGE_PARAM_PORTFORMATTYPE);
	iParamImagePortFormat.nVersion = aCommonPortData.iOmxVersion;
	iParamImagePortFormat.nPortIndex = aCommonPortData.iPortIndex;
	iParamImagePortFormat.nIndex = numImageFormats ? numImageFormats - 1 : 0;
	iParamImagePortFormat.eCompressionFormat = numImageFormats ? iSupportedImageFormats[0] : OMX_IMAGE_CodingUnused;
	iParamImagePortFormat.eColorFormat = numColorFormats ? iSupportedColorFormats[0] : OMX_COLOR_FormatUnused;
	}

EXPORT_C
COmxILImagePort::~COmxILImagePort()
	{
	DEBUG_PRINTF(_L8("COmxILImagePort::~COmxILImagePort"));
	iSupportedImageFormats.Close();
	iSupportedColorFormats.Close();
	}

EXPORT_C OMX_ERRORTYPE 
COmxILImagePort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	DEBUG_PRINTF(_L8("COmxILImagePort::GetLocalOmxParamIndexes"));

	// Always collect local indexes from parent
	OMX_ERRORTYPE omxRetValue = COmxILPort::GetLocalOmxParamIndexes(aIndexArray);
	
	if (OMX_ErrorNone != omxRetValue)
		{
		return omxRetValue;
		}
		
	TInt err = aIndexArray.InsertInOrder(OMX_IndexParamImagePortFormat);
	
	// Note that index duplication is OK.
	if (KErrNone != err && KErrAlreadyExists != err)
		{
		return OMX_ErrorInsufficientResources;
		}
	
	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE 
COmxILImagePort::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	DEBUG_PRINTF(_L8("COmxILImagePort::GetLocalOmxConfigIndexes"));

	// Always collect local indexes from parent
	return COmxILPort::GetLocalOmxConfigIndexes(aIndexArray);

	}

EXPORT_C OMX_ERRORTYPE COmxILImagePort::GetParameter(OMX_INDEXTYPE aParamIndex, TAny* apComponentParameterStructure) const
	{
	DEBUG_PRINTF(_L8("COmxILImagePort::GetParameter"));
	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	
	switch(aParamIndex)
		{
		case OMX_IndexParamImagePortFormat:
			{
			if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_IMAGE_PARAM_PORTFORMATTYPE))))
				{
				return omxRetValue;
				}
				
			OMX_IMAGE_PARAM_PORTFORMATTYPE* imagePortDefinition = static_cast<OMX_IMAGE_PARAM_PORTFORMATTYPE*>(apComponentParameterStructure);
			
			if(OMX_IMAGE_CodingUnused == iParamImagePortFormat.eCompressionFormat)
				{							
				if (imagePortDefinition->nIndex >= iSupportedColorFormats.Count())
					{
					return OMX_ErrorNoMore;
					}
				imagePortDefinition->eCompressionFormat = OMX_IMAGE_CodingUnused;
				imagePortDefinition->eColorFormat = iSupportedColorFormats[imagePortDefinition->nIndex];	
				}
			else
				{
				if (imagePortDefinition->nIndex >= iSupportedImageFormats.Count())
					{
					return OMX_ErrorNoMore;
					}
				imagePortDefinition->eCompressionFormat = iSupportedImageFormats[imagePortDefinition->nIndex];
				}
			break;
			}
		default:
			{
			// Try the parent's indexes
			return COmxILPort::GetParameter(aParamIndex, apComponentParameterStructure);
			}
		};

	return OMX_ErrorNone;
	}

EXPORT_C OMX_ERRORTYPE COmxILImagePort::SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure, TBool& aUpdateProcessingFunction)
	{
	DEBUG_PRINTF(_L8("COmxILImagePort::SetParameter"));
	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;

	switch(aParamIndex)
		{
		case OMX_IndexParamImagePortFormat:
			{
			if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_IMAGE_PARAM_PORTFORMATTYPE))))
				{
				return omxRetValue;
				}
							
			const OMX_IMAGE_PARAM_PORTFORMATTYPE *componentParameterStructure = static_cast<const OMX_IMAGE_PARAM_PORTFORMATTYPE*>(apComponentParameterStructure);
	
			if(OMX_IMAGE_CodingUnused == componentParameterStructure->eCompressionFormat)
				{
				if(OMX_COLOR_FormatUnused == componentParameterStructure->eColorFormat)
					{
					// Both Compression Format and Color can not be Unused at the same time.
					return OMX_ErrorBadParameter;
					}
				
				if(iParamImagePortFormat.eColorFormat != componentParameterStructure->eColorFormat)
					{
					if(KErrNotFound == iSupportedColorFormats.Find(componentParameterStructure->eColorFormat))
						{
						return OMX_ErrorUnsupportedSetting;
						}
					else
						{
						iParamImagePortFormat.eColorFormat = componentParameterStructure->eColorFormat;
						}
					aUpdateProcessingFunction = ETrue;
					}
				}
			else
				{
				// Data is compressed. Change relevant variables.
				if (OMX_COLOR_FormatUnused != componentParameterStructure->eColorFormat)
					{
					// Both Compression Format and Color can not be Unused at the same time.
					return OMX_ErrorBadParameter;
					}
						
				if (iParamImagePortFormat.eCompressionFormat != componentParameterStructure->eCompressionFormat)
					{
					if(KErrNotFound == iSupportedImageFormats.Find(componentParameterStructure->eCompressionFormat))
						{
						return OMX_ErrorUnsupportedSetting;
						}
					else
						{
						iParamImagePortFormat.eCompressionFormat = componentParameterStructure->eCompressionFormat;
						aUpdateProcessingFunction = ETrue;
						}
					}	
				}
			break;
			}
		default:
			{
			// Try the parent's indexes
			return COmxILPort::SetParameter(aParamIndex, apComponentParameterStructure, aUpdateProcessingFunction);
			}
		};
	return OMX_ErrorNone;
	}
