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

#include "log.h"
#include "omxilaudioport.h"
#include "omxilutil.h"

EXPORT_C
COmxILAudioPort::~COmxILAudioPort()
	{
    DEBUG_PRINTF(_L8("COmxILAudioPort::~COmxILAudioPort"));
    
    iSupportedAudioFormats.Close();
	}


EXPORT_C
COmxILAudioPort::COmxILAudioPort(const TOmxILCommonPortData& aCommonPortData)
:	COmxILPort(aCommonPortData)
	{
    DEBUG_PRINTF(_L8("COmxILAudioPort::COmxILAudioPort"));
	iParamAudioPortFormat.nSize		 = sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE);
	iParamAudioPortFormat.nVersion	 = aCommonPortData.iOmxVersion;
	iParamAudioPortFormat.nPortIndex = aCommonPortData.iPortIndex;
	iParamAudioPortFormat.nIndex	 = 0;
	}

	
// [YYC]: proposal for deep copy, virtual & called from derived CostructL
EXPORT_C void COmxILAudioPort::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	TUint count = aSupportedAudioFormats.Count();
	for (TInt i = 0; i < count; i++)
		{
		iSupportedAudioFormats.AppendL(aSupportedAudioFormats[i]);
		}
	iParamAudioPortFormat.eEncoding	 = count ? iSupportedAudioFormats[0] : OMX_AUDIO_CodingUnused;	
	}

EXPORT_C OMX_ERRORTYPE
COmxILAudioPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILAudioPort::GetLocalOmxParamIndexes"));

	// Always collect local indexes from parent
	OMX_ERRORTYPE omxRetValue =
		COmxILPort::GetLocalOmxParamIndexes(aIndexArray);

	if (OMX_ErrorNone != omxRetValue)
		{
		return omxRetValue;
		}

	TInt err = aIndexArray.InsertInOrder(OMX_IndexParamAudioPortFormat);

	// Note that index duplication is OK.
	if (KErrNone != err && KErrAlreadyExists != err)
		{
		return OMX_ErrorInsufficientResources;
		}

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILAudioPort::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILAudioPort::GetLocalOmxConfigIndexes"));

	// Always collect local indexes from parent
	return COmxILPort::GetLocalOmxConfigIndexes(aIndexArray);

	}

EXPORT_C OMX_ERRORTYPE
COmxILAudioPort::GetParameter(OMX_INDEXTYPE aParamIndex,
							  TAny* apComponentParameterStructure) const
	{
    DEBUG_PRINTF(_L8("COmxILAudioPort::GetParameter"));

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aParamIndex)
		{
	case OMX_IndexParamAudioPortFormat:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE))))
			{
			return omxRetValue;
			}

		OMX_AUDIO_PARAM_PORTFORMATTYPE* pParamAudioPortFormat
			= static_cast<OMX_AUDIO_PARAM_PORTFORMATTYPE*>(apComponentParameterStructure);

		if (pParamAudioPortFormat->nIndex >=
			iSupportedAudioFormats.Count())
			{
			return OMX_ErrorNoMore;
			}

		pParamAudioPortFormat->eEncoding =
			iSupportedAudioFormats[pParamAudioPortFormat->nIndex];
		}
		break;

	default:
		{
		// Try the parent's indexes
		return COmxILPort::GetParameter(aParamIndex, apComponentParameterStructure);
		}
		};

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILAudioPort::SetParameter(OMX_INDEXTYPE aParamIndex,
							  const TAny* apComponentParameterStructure,
							  TBool& aUpdateProcessingFunction)
	{
    DEBUG_PRINTF(_L8("COmxILAudioPort::SetParameter"));

	aUpdateProcessingFunction = EFalse;

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aParamIndex)
		{
	case OMX_IndexParamAudioPortFormat:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE))))
			{
			return omxRetValue;
			}

		const OMX_AUDIO_PARAM_PORTFORMATTYPE* pParamAudioPortFormat
			= static_cast<const OMX_AUDIO_PARAM_PORTFORMATTYPE*>(
				apComponentParameterStructure);

		// Check in case the specified format is not actually supported by this
		// port
		OMX_AUDIO_CODINGTYPE newDefaultCodingType =
			pParamAudioPortFormat->eEncoding;

		// OMX_AUDIO_CodingMIDI is the last of the supported values as of
		// v1.1.1
		if (newDefaultCodingType > OMX_AUDIO_CodingMIDI)
			{
			return OMX_ErrorBadParameter;
			}

		if (KErrNotFound == iSupportedAudioFormats.Find(newDefaultCodingType))
			{
			return OMX_ErrorUnsupportedSetting;
			}

		// Set the new default format, but check first that we are actually
		// changing something...
		if (iParamAudioPortFormat.eEncoding != newDefaultCodingType)
			{
			iParamAudioPortFormat.eEncoding = newDefaultCodingType;
			// This is an indication to the PortManager that the processing
			// function needs to get updated
			//
			aUpdateProcessingFunction = ETrue;
			}
		}
		break;

	default:
		{
		// Try the parent's indexes
		return COmxILPort::SetParameter(aParamIndex,
										apComponentParameterStructure,
										aUpdateProcessingFunction);
		}
		};

	return OMX_ErrorNone;

	}
