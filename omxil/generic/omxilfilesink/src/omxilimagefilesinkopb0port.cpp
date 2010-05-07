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

#include "omxilimagefilesinkopb0port.h"
#include "omxilfilesinkprocessingfunction.h"
#include <uri8.h>

COmxILImageFileSinkOPB0Port* COmxILImageFileSinkOPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData,
                                                             const RArray<OMX_IMAGE_CODINGTYPE>& aSupportedImageFormats,
                                                             const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats,
                                                             const COmxILFileSinkProcessingFunction& aFileSinkPF)
	{
	COmxILImageFileSinkOPB0Port* self = new (ELeave) COmxILImageFileSinkOPB0Port(aCommonPortData,
                                                                                aSupportedImageFormats,
                                                                                aSupportedColorFormats,
                                                                                aFileSinkPF);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void COmxILImageFileSinkOPB0Port::ConstructL()
	{
	iParamPortDefinition.eDomain = OMX_PortDomainImage;
	}

COmxILImageFileSinkOPB0Port::COmxILImageFileSinkOPB0Port(const TOmxILCommonPortData& aCommonPortData,
                                                        const RArray<OMX_IMAGE_CODINGTYPE>& aSupportedImageFormats,
                                                        const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats,
                                                        const COmxILFileSinkProcessingFunction& aFileSinkPF)
	: COmxILImagePort(aCommonPortData, aSupportedImageFormats, aSupportedColorFormats),
	iFileSinkPF(aFileSinkPF)
	{
	}

COmxILImageFileSinkOPB0Port::~COmxILImageFileSinkOPB0Port()
	{
	CleanUpPort();
	}
	
OMX_ERRORTYPE COmxILImageFileSinkOPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILImagePort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILImageFileSinkOPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILImagePort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILImageFileSinkOPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex, 
														TAny* apComponentParameterStructure) const
	{
	return COmxILImagePort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILImageFileSinkOPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex, 
														const TAny* apComponentParameterStructure, 
														TBool& aUpdateProcessingFunction)
	{
	return COmxILImagePort::SetParameter(aParamIndex, 
										 apComponentParameterStructure, 
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILImageFileSinkOPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, 
																TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.image = aPortDefinition.format.image;
	return OMX_ErrorNone;
	}

TBool COmxILImageFileSinkOPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
	if(aPortDefinition.eDomain != iParamPortDefinition.eDomain)
	    {
	    return EFalse;
	    }

	if (aPortDefinition.format.image.eCompressionFormat == OMX_IMAGE_CodingMax)
        {
        return EFalse;
        }	
	return ETrue;	
	}
