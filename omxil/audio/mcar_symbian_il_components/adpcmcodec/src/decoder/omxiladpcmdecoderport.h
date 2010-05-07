/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef OMXILADPCMDECODERPORT_H
#define OMXILADPCMDECODERPORT_H

#include "omxiladpcmcodecport.h"

class COmxILAdPcmDecoderPort : public COmxILAdPcmCodecPort
	{
public:
	static COmxILAdPcmDecoderPort* NewL(const TOmxILCommonPortData& aCommonPortData,
			 const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats,
			 const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType);

	~COmxILAdPcmDecoderPort();

	// From COmxILPort
	OMX_ERRORTYPE GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const;
	OMX_ERRORTYPE SetParameter(OMX_INDEXTYPE aParamIndex,
							   const TAny* apComponentParameterStructure,
							   TBool& aUpdateProcessingFunction);

private:
	COmxILAdPcmDecoderPort(const TOmxILCommonPortData& aCommonPortData,
			 const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType);
	};

#endif // OMXILADPCMDECODERPORT_H
