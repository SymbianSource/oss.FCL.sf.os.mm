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

#include "omxilaudiofilesinkopb0port.h"
#include "omxilfilesinkprocessingfunction.h"
#include <uri8.h>

COmxILAudioFileSinkOPB0Port* COmxILAudioFileSinkOPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData, 
													 const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats,
													 const COmxILFileSinkProcessingFunction& aFileSinkPF)
	{
	COmxILAudioFileSinkOPB0Port* self = new (ELeave) COmxILAudioFileSinkOPB0Port(aCommonPortData, aFileSinkPF);
	CleanupStack::PushL(self);
	self->ConstructL(aSupportedAudioFormats);
	CleanupStack::Pop(self);
	return self;
	}

void COmxILAudioFileSinkOPB0Port::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	iParamPortDefinition.eDomain = OMX_PortDomainAudio;
	
	// base
	COmxILAudioPort::ConstructL(aSupportedAudioFormats);
	}

COmxILAudioFileSinkOPB0Port::COmxILAudioFileSinkOPB0Port(const TOmxILCommonPortData& aCommonPortData, 
														const COmxILFileSinkProcessingFunction& aFileSinkPF)
	: COmxILAudioPort(aCommonPortData),
	iFileSinkPF(aFileSinkPF)
	{
	}

COmxILAudioFileSinkOPB0Port::~COmxILAudioFileSinkOPB0Port()
	{
	CleanUpPort();
	}
	
OMX_ERRORTYPE COmxILAudioFileSinkOPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILAudioPort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILAudioFileSinkOPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILAudioPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILAudioFileSinkOPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex, 
														TAny* apComponentParameterStructure) const
	{
	return COmxILAudioPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILAudioFileSinkOPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex, 
														const TAny* apComponentParameterStructure, 
														TBool& aUpdateProcessingFunction)
	{
	return COmxILAudioPort::SetParameter(aParamIndex, 
										 apComponentParameterStructure, 
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILAudioFileSinkOPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, 
																TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.audio = aPortDefinition.format.audio;
	return OMX_ErrorNone;
	}

TBool COmxILAudioFileSinkOPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
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
