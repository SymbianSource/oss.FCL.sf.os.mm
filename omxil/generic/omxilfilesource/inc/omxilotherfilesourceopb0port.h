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

#ifndef OMXILFILESOURCEOPB0PORT_H
#define OMXILFILESOURCEOPB0PORT_H

#include "omxilotherport.h"
#include "omxilfilesourceprocessingfunction.h"
#include <openmax/il/khronos/v1_x/OMX_Other.h>

class COmxILOtherFileSourceOPB0Port : public COmxILOtherPort
	{
public:
	static COmxILOtherFileSourceOPB0Port* NewL(const TOmxILCommonPortData& aCommonPortData, 
										const RArray<OMX_OTHER_FORMATTYPE>& aSupportedOtherFormats, 
										const COmxILFileSourceProcessingFunction& aFileSourcePF);
	~COmxILOtherFileSourceOPB0Port();
	
	OMX_ERRORTYPE GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const;
	OMX_ERRORTYPE GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const;

	OMX_ERRORTYPE GetParameter(OMX_INDEXTYPE aParamIndex, 
								TAny* apComponentParameterStructure) const;
	OMX_ERRORTYPE SetParameter(OMX_INDEXTYPE aParamIndex, 
								const TAny* apComponentParameterStructure, 
								TBool& aUpdateProcessingFunction);

	
private:
	COmxILOtherFileSourceOPB0Port(const TOmxILCommonPortData& aCommonPortData, 
								const COmxILFileSourceProcessingFunction& aFileSourcePF);
	void ConstructL(const RArray<OMX_OTHER_FORMATTYPE>& aSupportedOtherFormats);
	OMX_ERRORTYPE SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, 
											TBool& aUpdateProcessingFunction);
	TBool IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const;

private:
	const COmxILFileSourceProcessingFunction& iFileSourcePF;
	};

#endif // OMXILFILESOURCEOPB0PORT_H
