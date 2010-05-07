/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "omxiladpcmdecoderport.h"
#include "omxilsymbianadpcmdecoderextensions.h"
#include "log.h"
#include "omxilutil.h"


COmxILAdPcmDecoderPort* COmxILAdPcmDecoderPort::NewL(
			const TOmxILCommonPortData& aCommonPortData,
			const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats,
			const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmDecoderPort::NewL"));

    COmxILAdPcmDecoderPort* self = new(ELeave)COmxILAdPcmDecoderPort(aCommonPortData, aPcmModeType);

    CleanupStack::PushL(self);
    self->ConstructL(aSupportedAudioFormats);
    CleanupStack::Pop();

    return self;
    }


COmxILAdPcmDecoderPort::COmxILAdPcmDecoderPort(const TOmxILCommonPortData& aCommonPortData,
		 const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType)
	:	COmxILAdPcmCodecPort(aCommonPortData, aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmDecoderPort::NewL"));	
	}


COmxILAdPcmDecoderPort::~COmxILAdPcmDecoderPort()
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmDecoderPort::~COmxILAdPcmDecoderPort"));
	}


OMX_ERRORTYPE COmxILAdPcmDecoderPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmDecoderPort::GetLocalOmxParamIndexes"));

		// Always collect param indexes from parent
	OMX_ERRORTYPE omxRetValue = COmxILAdPcmCodecPort::GetLocalOmxParamIndexes(aIndexArray);
	if (OMX_ErrorNone != omxRetValue)
		{
		return omxRetValue;
		}

    TInt err = aIndexArray.InsertInOrder(OMX_SymbianIndexParamAudioAdPcmDecoderBlockAlign);
    if (KErrNone != err && KErrAlreadyExists != err)    // Note that index duplication is OK.
        {
        return OMX_ErrorInsufficientResources;
        }

	return OMX_ErrorNone;
	}


OMX_ERRORTYPE COmxILAdPcmDecoderPort::SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure, TBool& aUpdateProcessingFunction)
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmDecoderPort::SetParameter"));

    // we only have OMX_SymbianIndexParamAudioAdPcmDecoderSamplesPerBlock - pass all other requests to our parent
    switch (aParamIndex)
        {
        case OMX_SymbianIndexParamAudioAdPcmDecoderBlockAlign:
            {
            aUpdateProcessingFunction = ETrue;
            }
            break;

        default:
            return COmxILAdPcmCodecPort::SetParameter(aParamIndex, apComponentParameterStructure, aUpdateProcessingFunction);
		}

	return OMX_ErrorNone;
	}
