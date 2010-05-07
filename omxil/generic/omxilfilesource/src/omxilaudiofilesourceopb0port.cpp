// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * @file
 * @internalTechnology
 */

#include "omxilaudiofilesourceopb0port.h"
#include "omxilfilesourceprocessingfunction.h"
#include <uri8.h>

COmxILAudioFileSourceOPB0Port* COmxILAudioFileSourceOPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData,
													 const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats,
													 const COmxILFileSourceProcessingFunction& aFileSourcePF)
	{
	COmxILAudioFileSourceOPB0Port* self = new (ELeave) COmxILAudioFileSourceOPB0Port(aCommonPortData, aFileSourcePF);
	CleanupStack::PushL(self);
	self->ConstructL(aSupportedAudioFormats);
	CleanupStack::Pop(self);
	return self;
	}

void COmxILAudioFileSourceOPB0Port::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	iParamPortDefinition.eDomain = OMX_PortDomainAudio;

	// base
	COmxILAudioPort::ConstructL(aSupportedAudioFormats);
	}

COmxILAudioFileSourceOPB0Port::COmxILAudioFileSourceOPB0Port(const TOmxILCommonPortData& aCommonPortData,
														const COmxILFileSourceProcessingFunction& aFileSourcePF)
	: COmxILAudioPort(aCommonPortData),
	iFileSourcePF(aFileSourcePF)
	{
	}

COmxILAudioFileSourceOPB0Port::~COmxILAudioFileSourceOPB0Port()
	{
	CleanUpPort();
	}

OMX_ERRORTYPE COmxILAudioFileSourceOPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILAudioPort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILAudioFileSourceOPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILAudioPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILAudioFileSourceOPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex,
														TAny* apComponentParameterStructure) const
	{
	return COmxILAudioPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILAudioFileSourceOPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex,
														const TAny* apComponentParameterStructure,
														TBool& aUpdateProcessingFunction)
	{
	return COmxILAudioPort::SetParameter(aParamIndex,
										 apComponentParameterStructure,
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILAudioFileSourceOPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
																TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.audio = aPortDefinition.format.audio;
	return OMX_ErrorNone;
	}

TBool COmxILAudioFileSourceOPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
	
	if(aPortDefinition.eDomain != iParamPortDefinition.eDomain)
	    {
	    return EFalse;
	    }

	if (aPortDefinition.format.audio.eEncoding == OMX_AUDIO_CodingMax)
        {
        return EFalse;
        }
	
	return ETrue;
	}
