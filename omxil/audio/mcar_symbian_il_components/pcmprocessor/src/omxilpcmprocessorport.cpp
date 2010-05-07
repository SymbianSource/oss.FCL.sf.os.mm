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

#include <openmax/il/khronos/v1_x/OMX_Component.h>

#include "omxilpcmprocessorport.h"
#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/log.h>
#include <openmax/il/common/omxilutil.h>
#else
#include "log.h"
#include "omxilutil.h"
#endif


/**
 * Constructor.
 * @return 							Newly created COmxILPcmProcessorPort.
 */
COmxILPcmProcessorPort* COmxILPcmProcessorPort::NewL(const TOmxILCommonPortData& aCommonPortData, const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorPort::NewL"));
	
	COmxILPcmProcessorPort* self = new(ELeave)COmxILPcmProcessorPort(aCommonPortData, aPcmModeType);
	
	CleanupStack::PushL(self);
	self->ConstructL(aSupportedAudioFormats);
	CleanupStack::Pop();
	
	return self;
	}


COmxILPcmProcessorPort::COmxILPcmProcessorPort(const TOmxILCommonPortData& aCommonPortData, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType)
	:	COmxILPcmPortBase(aCommonPortData, aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorPort::NewL"));
	
	// No action
	}

/**
 * Second stage constructor.
 * See @C NewL for details.
 */
void COmxILPcmProcessorPort::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorPort::ConstructL"));
	
	// Call base class
	COmxILPcmPortBase::ConstructL(aSupportedAudioFormats);
	}

COmxILPcmProcessorPort::~COmxILPcmProcessorPort()
	{
	
	// No action
	}
