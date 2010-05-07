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

#include "omxilvideofilesinkopb0port.h"
#include "omxilfilesinkprocessingfunction.h"
#include <uri8.h>

COmxILVideoFileSinkOPB0Port* COmxILVideoFileSinkOPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData,
	                                                 const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedCodings,
	                                                 const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColourFormats,
													 const COmxILFileSinkProcessingFunction& aFileSinkPF)
	{
	COmxILVideoFileSinkOPB0Port* self = new (ELeave) COmxILVideoFileSinkOPB0Port(aCommonPortData,
                                                                                aSupportedCodings,
                                                                                aSupportedColourFormats,
                                                                                aFileSinkPF);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void COmxILVideoFileSinkOPB0Port::ConstructL()
	{
	iParamPortDefinition.eDomain = OMX_PortDomainVideo;
	}

COmxILVideoFileSinkOPB0Port::COmxILVideoFileSinkOPB0Port(const TOmxILCommonPortData& aCommonPortData,
                                                        const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedCodings,
                                                        const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColourFormats,
														const COmxILFileSinkProcessingFunction& aFileSinkPF)
	: COmxILVideoPort(aCommonPortData, aSupportedCodings, aSupportedColourFormats),
	iFileSinkPF(aFileSinkPF)
	{
	}

COmxILVideoFileSinkOPB0Port::~COmxILVideoFileSinkOPB0Port()
	{
	CleanUpPort();
	}
	
OMX_ERRORTYPE COmxILVideoFileSinkOPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILVideoPort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILVideoFileSinkOPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILVideoPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILVideoFileSinkOPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex, 
														TAny* apComponentParameterStructure) const
	{
	return COmxILVideoPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILVideoFileSinkOPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex, 
														const TAny* apComponentParameterStructure, 
														TBool& aUpdateProcessingFunction)
	{
	return COmxILVideoPort::SetParameter(aParamIndex, 
										 apComponentParameterStructure, 
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILVideoFileSinkOPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, 
																TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.video = aPortDefinition.format.video;
	return OMX_ErrorNone;
	}

TBool COmxILVideoFileSinkOPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
	if(aPortDefinition.eDomain != iParamPortDefinition.eDomain)
	    {
	    return EFalse;
	    }

	if (aPortDefinition.format.video.eCompressionFormat == OMX_VIDEO_CodingMax)
        {
        return EFalse;
        }	
	return ETrue;	
	}
