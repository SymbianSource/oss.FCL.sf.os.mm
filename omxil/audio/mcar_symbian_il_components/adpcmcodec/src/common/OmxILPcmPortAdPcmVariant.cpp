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

// Self
#include "OmxILPcmPortAdPcmVariant.h"

// Project (MMP)
/* None */

// Local (INF)
#include <openmax/il/khronos/v1_x/OMX_Component.h>

#include "log.h"
#include "omxilutil.h"


// System
/* None */

COmxILPcmPortAdPcmVariant* COmxILPcmPortAdPcmVariant::NewL(const TOmxILCommonPortData& aCommonPortData, const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILPcmPortAdPcmVariant::NewL"));
	
	COmxILPcmPortAdPcmVariant* self = new(ELeave)COmxILPcmPortAdPcmVariant(aCommonPortData, aPcmModeType);
	
	CleanupStack::PushL(self);
	self->ConstructL(aSupportedAudioFormats);
	CleanupStack::Pop();
	
	return self;
	}

COmxILPcmPortAdPcmVariant::COmxILPcmPortAdPcmVariant(const TOmxILCommonPortData& aCommonPortData, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType)
	:	COmxILPcmPortBase(aCommonPortData, aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILPcmPortAdPcmVariant::COmxILPcmPortAdPcmVariant"));
	
	// No action
	}
	 
void COmxILPcmPortAdPcmVariant::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	DEBUG_PRINTF(_L8("COmxILPcmPortAdPcmVariant::ConstructL"));
	
	// Call base class
	COmxILPcmPortBase::ConstructL(aSupportedAudioFormats);
	}

COmxILPcmPortAdPcmVariant::~COmxILPcmPortAdPcmVariant()
	{
	DEBUG_PRINTF(_L8("COmxILPcmPortAdPcmVariant::~COmxILPcmDecoderAPB0Port"));
	// No action
	}

TBool COmxILPcmPortAdPcmVariant::ValidatePcmModeParams(const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType) const
	{
	OMX_AUDIO_PARAM_PCMMODETYPE type = ModeType();
	DEBUG_PRINTF(_L8("COmxILPcmPortAdPcmVariant::ValidatePcmModeParams"));
	return (aPcmModeType.nBitPerSample == 16)
		&& (aPcmModeType.nPortIndex == ModeType().nPortIndex)					//port number changes are not allowed
		&& (aPcmModeType.nChannels == 1)										// only mono is supported by the core encoder/decoder functions
		&& (aPcmModeType.eChannelMapping[0] == OMX_AUDIO_ChannelLF)
		&& (aPcmModeType.eChannelMapping[1] == OMX_AUDIO_ChannelRF)				// interleaving is ignored since only mono is supported
		&& (static_cast<TInt>(aPcmModeType.nSamplingRate) > 0)					// sample rate must be positive
		;
	}
