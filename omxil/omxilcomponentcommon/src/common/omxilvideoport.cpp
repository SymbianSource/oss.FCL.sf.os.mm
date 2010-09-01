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

#include "omxilvideoport.h"
#include "omxilutil.h"
#include "log.h"

EXPORT_C
COmxILVideoPort::COmxILVideoPort(
		const TOmxILCommonPortData& aCommonPortData,
		const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedVideoFormats,
		const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats)
:
COmxILPort(aCommonPortData)
	{
	DEBUG_PRINTF(_L8("COmxILVideoPort::COmxILVideoPort"));

	// Performing a deep copy of these arrays inside a C++ constructor is unsafe.
	// Items should be added to these arrays directly inside the derived class' ConstructL().
	__ASSERT_ALWAYS(aSupportedVideoFormats.Count() == 0 && aSupportedColorFormats.Count() == 0, User::Invariant());
	TInt numVideoFormats = iSupportedVideoFormats.Count();
	TInt numColorFormats = iSupportedColorFormats.Count();
	iParamVideoPortFormat.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
	iParamVideoPortFormat.nVersion = aCommonPortData.iOmxVersion;
	iParamVideoPortFormat.nPortIndex = aCommonPortData.iPortIndex;
	iParamVideoPortFormat.nIndex = numVideoFormats ? numVideoFormats - 1 : 0;
	iParamVideoPortFormat.eCompressionFormat = numVideoFormats ? iSupportedVideoFormats[0] : OMX_VIDEO_CodingUnused;
	iParamVideoPortFormat.eColorFormat = numColorFormats ? iSupportedColorFormats[0] : OMX_COLOR_FormatUnused;
	iParamVideoPortFormat.xFramerate = 0;//aCommonPortData.xFramerate???;

	}

EXPORT_C
COmxILVideoPort::~COmxILVideoPort()
	{
	DEBUG_PRINTF(_L8("COmxILVideoPort::~COmxILVideoPort"));
	iSupportedVideoFormats.Close();
	iSupportedColorFormats.Close();
	}

EXPORT_C OMX_ERRORTYPE 
COmxILVideoPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	DEBUG_PRINTF(_L8("COmxILVideoPort::GetLocalOmxParamIndexes"));

	// Always collect local indexes from parent
	OMX_ERRORTYPE omxRetValue = COmxILPort::GetLocalOmxParamIndexes(aIndexArray);
	
	if (OMX_ErrorNone != omxRetValue)
		{
		return omxRetValue;
		}
		
	TInt err = aIndexArray.InsertInOrder(OMX_IndexParamVideoPortFormat);
	
	// Note that index duplication is OK.
	if (KErrNone != err && KErrAlreadyExists != err)
		{
		return OMX_ErrorInsufficientResources;
		}
	
	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE 
COmxILVideoPort::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	DEBUG_PRINTF(_L8("COmxILVideoPort::GetLocalOmxConfigIndexes"));

	// Always collect local indexes from parent
	return COmxILPort::GetLocalOmxConfigIndexes(aIndexArray);

	}

EXPORT_C OMX_ERRORTYPE COmxILVideoPort::GetParameter(OMX_INDEXTYPE aParamIndex, TAny* apComponentParameterStructure) const
	{
	DEBUG_PRINTF(_L8("COmxILVideoPort::GetParameter"));
	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	
	switch(aParamIndex)
		{
		case OMX_IndexParamVideoPortFormat:
			{
			if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE))))
				{
				return omxRetValue;
				}
				
			OMX_VIDEO_PARAM_PORTFORMATTYPE* videoPortDefinition = static_cast<OMX_VIDEO_PARAM_PORTFORMATTYPE*>(apComponentParameterStructure);
			
			if(OMX_VIDEO_CodingUnused == iParamVideoPortFormat.eCompressionFormat)
				{							
				if (videoPortDefinition->nIndex >= iSupportedColorFormats.Count())
					{
					return OMX_ErrorNoMore;
					}
				videoPortDefinition->eColorFormat = iSupportedColorFormats[videoPortDefinition->nIndex];	
				}
			else
				{
				if (videoPortDefinition->nIndex >= iSupportedVideoFormats.Count())
					{
					return OMX_ErrorNoMore;
					}
				videoPortDefinition->eCompressionFormat = iSupportedVideoFormats[videoPortDefinition->nIndex];
				}
			videoPortDefinition->xFramerate = iParamVideoPortFormat.xFramerate;	
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

EXPORT_C OMX_ERRORTYPE COmxILVideoPort::SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure, TBool& aUpdateProcessingFunction)
	{
	DEBUG_PRINTF(_L8("COmxILVideoPort::SetParameter"));
	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;

	switch(aParamIndex)
		{
		case OMX_IndexParamVideoPortFormat:
			{
			if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE))))
				{
				return omxRetValue;
				}
							
			const OMX_VIDEO_PARAM_PORTFORMATTYPE *componentParameterStructure = static_cast<const OMX_VIDEO_PARAM_PORTFORMATTYPE*>(apComponentParameterStructure);
	
			if(OMX_VIDEO_CodingUnused == componentParameterStructure->eCompressionFormat)
				{
				if(OMX_COLOR_FormatUnused == componentParameterStructure->eColorFormat)
					{
					// Both Compression Format and Color can not be Unused at the same time.
					return OMX_ErrorBadParameter;
					}
				
				if(iParamVideoPortFormat.eColorFormat != componentParameterStructure->eColorFormat)
					{
					if(KErrNotFound == iSupportedColorFormats.Find(componentParameterStructure->eColorFormat))
						{
						return OMX_ErrorUnsupportedSetting;
						}
					else
						{
						iParamVideoPortFormat.eColorFormat = componentParameterStructure->eColorFormat;
						}
					aUpdateProcessingFunction = ETrue;
					}
				
				if(iParamVideoPortFormat.xFramerate != componentParameterStructure->xFramerate)
					{
					iParamVideoPortFormat.xFramerate = componentParameterStructure->xFramerate;
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
						
				if (iParamVideoPortFormat.eCompressionFormat != componentParameterStructure->eCompressionFormat)
					{
					if(KErrNotFound == iSupportedVideoFormats.Find(componentParameterStructure->eCompressionFormat))
						{
						return OMX_ErrorUnsupportedSetting;
						}
					else
						{
						iParamVideoPortFormat.eCompressionFormat = componentParameterStructure->eCompressionFormat;
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
