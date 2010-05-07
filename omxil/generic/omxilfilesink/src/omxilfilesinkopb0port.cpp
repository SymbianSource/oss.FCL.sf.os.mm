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
 * @file
 * @internalTechnology
 */

#include "omxilfilesinkopb0port.h"
#include "omxilfilesinkprocessingfunction.h"
#include <uri8.h>

COmxILOtherFileSinkOPB0Port* COmxILOtherFileSinkOPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData, 
													 const RArray<OMX_OTHER_FORMATTYPE>& aSupportedOtherFormats,
													 const COmxILFileSinkProcessingFunction& aFileSinkPF)
	{
	COmxILOtherFileSinkOPB0Port* self = new (ELeave) COmxILOtherFileSinkOPB0Port(aCommonPortData, aFileSinkPF);
	CleanupStack::PushL(self);
	self->ConstructL(aSupportedOtherFormats);
	CleanupStack::Pop(self);
	return self;
	}

void COmxILOtherFileSinkOPB0Port::ConstructL(const RArray<OMX_OTHER_FORMATTYPE>& aSupportedOtherFormats)
	{
	iParamPortDefinition.eDomain = OMX_PortDomainOther;
	iParamPortDefinition.format.other.eFormat = OMX_OTHER_FormatBinary;
	
	// base
	COmxILOtherPort::ConstructL(aSupportedOtherFormats);
	}

COmxILOtherFileSinkOPB0Port::COmxILOtherFileSinkOPB0Port(const TOmxILCommonPortData& aCommonPortData, 
											   const COmxILFileSinkProcessingFunction& aFileSinkPF)
	: COmxILOtherPort(aCommonPortData),
	iFileSinkPF(aFileSinkPF)
	{
	}

COmxILOtherFileSinkOPB0Port::~COmxILOtherFileSinkOPB0Port()
	{
	CleanUpPort();
	}
	
OMX_ERRORTYPE COmxILOtherFileSinkOPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILOtherPort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILOtherFileSinkOPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILOtherPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILOtherFileSinkOPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex, 
												   TAny* apComponentParameterStructure) const
	{
	return COmxILOtherPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILOtherFileSinkOPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex, 
		           								   const TAny* apComponentParameterStructure, 
		           								   TBool& aUpdateProcessingFunction)
	{
	return COmxILOtherPort::SetParameter(aParamIndex, 
										 apComponentParameterStructure, 
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILOtherFileSinkOPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, 
																TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.other = aPortDefinition.format.other;
	return OMX_ErrorNone;
	}

TBool COmxILOtherFileSinkOPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
	if(aPortDefinition.eDomain != iParamPortDefinition.eDomain)
	    {
	    return EFalse;
	    }

	if (aPortDefinition.format.other.eFormat == OMX_OTHER_FormatMax)
        {
        return EFalse;
        }	
	
	return ETrue;	
	}
