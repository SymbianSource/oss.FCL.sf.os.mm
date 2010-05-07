/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


/**
   @file
   @internalComponent
*/

#ifndef OMXILPCMPORTBASE_H
#define OMXILPCMPORTBASE_H
#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/omxilaudioport.h>
#else
#include "omxilaudioport.h"
#endif

/**
 * Base class for PCM port.
 */
class COmxILPcmPortBase : public COmxILAudioPort
	{
	public:
		~COmxILPcmPortBase();
		OMX_ERRORTYPE GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const;
		OMX_ERRORTYPE GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const;
		OMX_ERRORTYPE GetParameter(OMX_INDEXTYPE aParamIndex, TAny* apComponentParameterStructure) const;
		OMX_ERRORTYPE SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure, TBool& aUpdateProcessingFunction);
		inline const OMX_AUDIO_PARAM_PCMMODETYPE& ModeType() const;
	
	protected:
		COmxILPcmPortBase(const TOmxILCommonPortData& aCommonPortData, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType);
		virtual void ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats);

	private:
		OMX_ERRORTYPE SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, TBool& aUpdateProcessingFunction);
		TBool IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const;
		TBool CheckPcmModeParamsAreSane(const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType) const;
		virtual TBool ValidatePcmModeParams(const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType) const;

	protected:
		OMX_AUDIO_PARAM_PCMMODETYPE iParamAudioPcm;
	};

#include "omxilpcmportbase.inl"
#endif
