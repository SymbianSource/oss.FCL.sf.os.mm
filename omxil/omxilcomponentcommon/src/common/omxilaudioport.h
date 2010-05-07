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

#ifndef OMXILAUDIOPORT_H
#define OMXILAUDIOPORT_H

#include <e32base.h>

#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>

#include "omxilport.h"

class COmxILAudioPort : public COmxILPort
	{

public:

	IMPORT_C ~COmxILAudioPort();

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

	IMPORT_C COmxILAudioPort(const TOmxILCommonPortData& aCommonPortData);
	IMPORT_C virtual void ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats);


	// From COmxILPort
	IMPORT_C OMX_ERRORTYPE SetFormatInPortDefinition(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
		TBool& aUpdateProcessingFunction) = 0;

	// From COmxILPort
	IMPORT_C TBool IsTunnelledPortCompatible(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const = 0;

protected:

	RArray<OMX_AUDIO_CODINGTYPE> iSupportedAudioFormats;
	OMX_AUDIO_PARAM_PORTFORMATTYPE iParamAudioPortFormat;

	};

#endif // OMXILAUDIOPORT_H

