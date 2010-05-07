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

#ifndef OMXILPCMPROCESSORPORT_H
#define OMXILPCMPROCESSORPORT_H

#include "OmxILPcmPortBase.h"

class COmxILPcmProcessorPort : public COmxILPcmPortBase
	{
	public:
		static COmxILPcmProcessorPort* NewL(const TOmxILCommonPortData& aCommonPortData, const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType); 
		~COmxILPcmProcessorPort();

	private:
		COmxILPcmProcessorPort(const TOmxILCommonPortData& aCommonPortData, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType);
		void ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats);
	};


#endif
