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

#include "omxilvideofilesourceopb0port.h"
#include "omxilfilesourceprocessingfunction.h"
#include <uri8.h>

COmxILVideoFileSourceOPB0Port* COmxILVideoFileSourceOPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData,
	                                                 const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedCodings,
	                                                 const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColourFormats,
													 const COmxILFileSourceProcessingFunction& aFileSourcePF)
	{
	COmxILVideoFileSourceOPB0Port* self = new (ELeave) COmxILVideoFileSourceOPB0Port(aCommonPortData,
                                                                                aSupportedCodings,
                                                                                aSupportedColourFormats,
                                                                                aFileSourcePF);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void COmxILVideoFileSourceOPB0Port::ConstructL()
	{
	iParamPortDefinition.eDomain = OMX_PortDomainVideo;
	}

COmxILVideoFileSourceOPB0Port::COmxILVideoFileSourceOPB0Port(const TOmxILCommonPortData& aCommonPortData,
                                                        const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedCodings,
                                                        const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColourFormats,
														const COmxILFileSourceProcessingFunction& aFileSourcePF)
	: COmxILVideoPort(aCommonPortData, aSupportedCodings, aSupportedColourFormats),
	iFileSourcePF(aFileSourcePF)
	{
	}

COmxILVideoFileSourceOPB0Port::~COmxILVideoFileSourceOPB0Port()
	{
	CleanUpPort();
	}

OMX_ERRORTYPE COmxILVideoFileSourceOPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILVideoPort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILVideoFileSourceOPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILVideoPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILVideoFileSourceOPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex,
														TAny* apComponentParameterStructure) const
	{
	return COmxILVideoPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILVideoFileSourceOPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex,
														const TAny* apComponentParameterStructure,
														TBool& aUpdateProcessingFunction)
	{
	return COmxILVideoPort::SetParameter(aParamIndex,
										 apComponentParameterStructure,
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILVideoFileSourceOPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
																TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.video = aPortDefinition.format.video;
	return OMX_ErrorNone;
	}

TBool COmxILVideoFileSourceOPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
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
