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


#ifndef OMXILADPCMCODECPORT_H
#define OMXILADPCMCODECPORT_H
#include "omxilaudioport.h"

class COmxILAdPcmCodecPort : public COmxILAudioPort
	{
public:
	static COmxILAdPcmCodecPort* NewL(const TOmxILCommonPortData& aCommonPortData,
			 const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats,
			 const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType);

	~COmxILAdPcmCodecPort();

	// From COmxILPort
	OMX_ERRORTYPE GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const;
	OMX_ERRORTYPE GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const;
	OMX_ERRORTYPE GetParameter(OMX_INDEXTYPE aParamIndex,
							   TAny* apComponentParameterStructure) const;
	OMX_ERRORTYPE SetParameter(OMX_INDEXTYPE aParamIndex,
							   const TAny* apComponentParameterStructure,
							   TBool& aUpdateProcessingFunction);

	inline const OMX_AUDIO_PARAM_ADPCMTYPE& ModeType() const;

protected:
	COmxILAdPcmCodecPort(const TOmxILCommonPortData& aCommonPortData,
			 const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType);
	
	void ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats);

private:
	// From COmxILPort
	OMX_ERRORTYPE SetFormatInPortDefinition(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
		TBool& aUpdateProcessingFunction);
	TBool IsTunnelledPortCompatible(
		const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const;

	TBool CheckPcmModeParamsAreSane(const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType) const;

private:
	OMX_AUDIO_PARAM_ADPCMTYPE iParamAudioAdPcm;
	};

#include "omxiladpcmcodecport.inl"
#endif
