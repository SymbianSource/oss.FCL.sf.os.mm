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
 @internalTechnology
*/

#ifndef OMXILVIDEOPORT_H
#define OMXILVIDEOPORT_H

#include <e32base.h>

#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>

#include "omxilport.h"

class COmxILVideoPort : public COmxILPort
	{

public:

	IMPORT_C ~COmxILVideoPort();
	
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

	IMPORT_C COmxILVideoPort(const TOmxILCommonPortData& aCommonPortData,
							 const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedVideoFormats,
							 const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats);
				
	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE SetFormatInPortDefinition(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
		TBool& aUpdateProcessingFunction) = 0;

	// From COmxILPort
	IMPORT_C TBool IsTunnelledPortCompatible(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const = 0;
	
	IMPORT_C TBool UpdateColorFormat(OMX_COLOR_FORMATTYPE& aOldColor, OMX_COLOR_FORMATTYPE aNewColor, TBool& aUpdated);
	IMPORT_C TBool UpdateCodingType(OMX_VIDEO_CODINGTYPE& aOldCodingType, OMX_VIDEO_CODINGTYPE aNewCodingType, TBool& aUpdated);

protected:

	RArray<OMX_VIDEO_CODINGTYPE> iSupportedVideoFormats;
	RArray<OMX_COLOR_FORMATTYPE> iSupportedColorFormats;
	RArray<OMX_U32> iSupportedFrameRates;
	OMX_VIDEO_PARAM_PORTFORMATTYPE iParamVideoPortFormat;
	};

#endif // OMXILVIDEOPORT_H
