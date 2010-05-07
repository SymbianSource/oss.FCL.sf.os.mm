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

#ifndef OMXILIMAGEPORT_H
#define OMXILIMAGEPORT_H

#include <e32base.h>

#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>

#include "omxilport.h"

class COmxILImagePort : public COmxILPort
	{

public:

	IMPORT_C ~COmxILImagePort();
	
	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const = 0;

	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const = 0;

	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE GetParameter(OMX_INDEXTYPE aParamIndex,
							   TAny* apComponentParameterStructure) const = 0;
	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE SetParameter(OMX_INDEXTYPE aParamIndex,
							   const TAny* apComponentParameterStructure,
							   TBool& aUpdateProcessingFunction) = 0;
							  
protected:

	IMPORT_C COmxILImagePort(const TOmxILCommonPortData& aCommonPortData,
							 const RArray<OMX_IMAGE_CODINGTYPE>& aSupportedImageFormats,
							 const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats);
				
	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE SetFormatInPortDefinition(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
		TBool& aUpdateProcessingFunction) = 0;

	// From COmxILPort
	IMPORT_C TBool IsTunnelledPortCompatible(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const = 0;
		
protected:

	RArray<OMX_IMAGE_CODINGTYPE> iSupportedImageFormats;
	RArray<OMX_COLOR_FORMATTYPE> iSupportedColorFormats;
	OMX_IMAGE_PARAM_PORTFORMATTYPE iParamImagePortFormat;
	};

#endif // OMXILIMAGEPORT_H
